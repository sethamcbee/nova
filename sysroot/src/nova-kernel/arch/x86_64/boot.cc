/**
 * @file boot.c
 * @author Seth McBee
 * @date 2017-10-14
 * @brief x86-64 initial C boot environment.
 */

#include <globals.h>

#include <stdlib.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>
#include <proc/scheduler.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/multiboot2.h>
#include <arch/x86_64/tss.h>
#include <arch/x86_64/devices/cmos.h>
#include <arch/x86_64/devices/pic.h>
#include <arch/x86_64/devices/pit.h>
#include <arch/x86_64/interrupts/idt.h>
#include <arch/x86_64/memory/pmm.h>
#include <arch/x86_64/memory/vmm.h>

void multiboot2_parse(struct multiboot_tag *mb_tag);
void multiboot2_parse_mmap(struct multiboot_tag_mmap *mb_mmap);

extern "C" void boot_main(struct multiboot_tag *mb_tag, uint32_t magic)
{
    // Initialize terminal.
    kernel_write = vga_text_write;
    vga_text_initialize();

    // Verify that the Multiboot2 magic word was passed correctly.
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        kernel_panic("MULTIBOOT2 BOOTLOADER SIGNATURE IS INVALID.");
    }

    // Parse Multiboot2 structure.
    // Physical memory manager is initialized during this process.
    multiboot2_parse(mb_tag);

    gdt_init();
    idt_initialize();
    vmm_init();
    tss_init();

    // Initialize PIC (disables all IRQs).
    pic_initialize();
    pic_irq_enable(IRQ_PIT);
    pic_irq_enable(IRQ_KEYBOARD);

    ps2_keyboard_init();

    // Set up timer.
    cmos_init();
    pit_init();

    // Initialize scheduler.
    cur_task = NULL;

    asm volatile ("sti \n"); // We can safely enable interrupts now.

    kernel_main();
}

void multiboot2_parse(struct multiboot_tag *mb_tag)
{
    char s[20];
    struct multiboot_tag* mb_end;
    struct multiboot_tag_string* mb_tag_string;
    struct multiboot_tag_module* mb_tag_module;
    struct multiboot_tag_mmap* mb_mmap = NULL;
    bool memory_map_found = false;

    // Initialize kernel module to point to nothing.
    kernel_module.mod_start = NULL;

    mb_end = (struct multiboot_tag*) (((uint8_t*)mb_tag) + mb_tag->type);
    itoa(mb_tag->type, s);
    kernel_log(s);
    kernel_log("\n");
    mb_tag = (struct multiboot_tag*) (((uint8_t*)mb_tag) + 8);

    // Parse entries one at a time.
    while (!(mb_tag->type == 0 && mb_tag->size == 8))
    {
        // Check if we reach the end of the structure.
        if (mb_tag > mb_end)
        {
            break;
        }

        switch (mb_tag->type)
        {
        // Boot command line string.
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            mb_tag_string = (struct multiboot_tag_string*)mb_tag;
            break;

        // Bootloader name string.
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            mb_tag_string = (struct multiboot_tag_string*)mb_tag;
            break;

        // Memory map.
        case MULTIBOOT_TAG_TYPE_MMAP:
            memory_map_found = true;
            mb_mmap = (struct multiboot_tag_mmap*)mb_tag;
            break;

        // Module.
        case MULTIBOOT_TAG_TYPE_MODULE:
            mb_tag_module = (struct multiboot_tag_module*)mb_tag;
            kernel_module = *mb_tag_module;
            break;

        // Treat unsupported tag as basic tag.
        default:
            break;
        }

        // Calculate offset of next tag, including padding up to 8-byte
        // boundary.
        uint32_t offset = mb_tag->size;
        if (offset % 8 != 0)
        {
            offset += 8 - (mb_tag->size % 8);
        }

        mb_tag = (struct multiboot_tag*) (((uint8_t*)mb_tag) + offset);
    }

    // Abort if no memory map was found.
    if (memory_map_found == false)
    {
        kernel_panic("NO MULTIBOOT2 MEMORY MAP FOUND.");
    }

    // Initialize physical memory manager.
    pmm_init(mb_mmap);
}
