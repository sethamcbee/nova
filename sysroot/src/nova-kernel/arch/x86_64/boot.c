// Authors: Seth McBee
// Created: 2017-10-14
// Description: x86_64 initial C boot environment.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

#include <kernel.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/multiboot2.h>
#include <arch/x86_64/pmm.h>
#include <arch/x86_64/devices/pic.h>
#include <arch/x86_64/interrupts/idt.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>

extern size_t _sections_end;

void multiboot2_parse(struct multiboot_tag *mb_tag);
void multiboot2_parse_mmap(struct multiboot_tag_mmap *mb_mmap);

void boot_main(struct multiboot_tag *mb_tag, uint32_t magic)
{
    // Initialize terminal.
    kernel_write = vga_text_write;
    vga_text_initialize();
    kernel_log("Terminal initialized.\n");

    // Verify that the Multiboot2 magic word was passed correctly.
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        kernel_log("MULTIBOOT2 BOOTLOADER SIGNATURE IS INVALID.");
    }

    idt_initialize();
    kernel_log("IDT initialized.\n");

    // Initialize PIC (disables all IRQs).
    pic_initialize();
    kernel_log("PIC initialized.\n");

    ps2_keyboard_init();
    kernel_log("Keyboard initialized.\n");

    // Enable supported IRQs.
    pic_irq_enable(IRQ_PIT);
    pic_irq_enable(IRQ_KEYBOARD);

    // TODO: Set up timer.

    // Parse Multiboot2 structure.
    // Physical memory manager info is gathered during this process.
    multiboot2_parse(mb_tag);

    pmm_init();

    // TODO: Set up paging.

    asm volatile ("sti \n"); // We can safely enable interrupts now.

    kernel_main();
}

void multiboot2_parse(struct multiboot_tag *mb_tag)
{
    char s[20];
    struct multiboot_tag *mb_end;
    struct multiboot_tag_string *mb_tag_string;
    struct multiboot_tag_mmap *mb_mmap;
    bool memory_map_found = false;

    mb_end = (struct multiboot_tag*) (((uint8_t*)mb_tag) + mb_tag->type);
    kernel_log("Multiboot2: Boot information struct\n\tSize: ");
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

        kernel_log("Multiboot2: "); // Formatting.

        switch (mb_tag->type)
        {
        // Boot command line string.
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            mb_tag_string = (struct multiboot_tag_string*)mb_tag;
            kernel_log("Boot command line:\n\t");
            kernel_log(mb_tag_string->string);
            break;

        // Bootloader name string.
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            mb_tag_string = (struct multiboot_tag_string*)mb_tag;
            kernel_log("Bootloader name:\n\t");
            kernel_log(mb_tag_string->string);
            break;

        // Memory map.
        case MULTIBOOT_TAG_TYPE_MMAP:
            memory_map_found = true;
            mb_mmap = (struct multiboot_tag_mmap*)mb_tag;
            kernel_log("Memory map:\n\tSize: ");
            sitoa(mb_mmap->size, s);
            kernel_log(s);
            kernel_log("\n\tEntry size: ");
            sitoa(mb_mmap->entry_size, s);
            kernel_log(s);
            kernel_log("\n");
            multiboot2_parse_mmap(mb_mmap);
            break;

        // Treat unsupported tag as basic tag.
        default:
            kernel_log("Basic tag\n\tType: ");
            itoa(mb_tag->type, s);
            kernel_log(s);
            kernel_log("\n\tSize: ");
            itoa(mb_tag->size, s);
            kernel_log(s);
            break;
        }

        kernel_log("\n");   // Formatting.

        // Calculate offset of next tag, including padding up to 8-byte
        // boundary.
        uint32_t offset = mb_tag->size;
        if (offset % 8 != 0)
        {
            offset += 8 - (mb_tag->size % 8);
        }
        if (offset == 0)
        {
            offset += 8;
        }

        mb_tag = (struct multiboot_tag*) (((uint8_t*)mb_tag) + offset);
    }

    // Abort if no memory map was found.
    if (memory_map_found == false)
    {
        kernel_panic("NO MULTIBOOT2 MEMORY MAP FOUND.");
    }
}


void multiboot2_parse_mmap(struct multiboot_tag_mmap *mb_mmap)
{
    // Initialize the physical memory manager.
    pmm_mem_start = 0;
    pmm_mem_max = 0;
    pmm_mem_free = 0;

    // TODO: Search for large enough space to store mb_mmap->size bytes
    // and allocate this space dynamically, to be used by the paging
    // structure.
    struct multiboot_mmap_entry multiboot2_mmap[mb_mmap->size / mb_mmap->entry_size];

    // Iterate over memory map and copy entries.
    for (size_t i = 0; i < mb_mmap->size / mb_mmap->entry_size; i++)
    {
        multiboot2_mmap[i].addr = mb_mmap->entries[i].addr;
        multiboot2_mmap[i].len = mb_mmap->entries[i].len;
        multiboot2_mmap[i].type = mb_mmap->entries[i].type;
        multiboot2_mmap[i].zero = 0;
    }

    // Pass info to the PMM.
    for (size_t i = 0; i < mb_mmap->size / mb_mmap->entry_size; i++)
    {
        if (multiboot2_mmap[i].type == 1)
        {
            pmm_mem_max = multiboot2_mmap[i].len;
            pmm_mem_start = multiboot2_mmap[i].addr;
            break;
        }
    }

    pmm_mem_start += 0x10000;
    pmm_mem_max -= 0x10000;
    pmm_mem_free = pmm_mem_max;

    // Log memory map entries.
    char s[30];
    for (size_t i = 0; i < mb_mmap->size / mb_mmap->entry_size; i++)
    {
        kernel_log("\nMemory map entry: ");
        sitoa(i, s);
        kernel_log(s);
        kernel_log("\n\tBase address: ");
        _sitoa(multiboot2_mmap[i].addr, s, 16);
        kernel_log(s);
        kernel_log("\n\tLength (bytes): ");
        _sitoa(multiboot2_mmap[i].len, s, 16);
        kernel_log(s);
        kernel_log("\n\tType: ");
        _sitoa(multiboot2_mmap[i].type, s, 10);
        kernel_log(s);
    }
}
