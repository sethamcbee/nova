// Authors: Seth McBee
// Created: 2017-10-14
// Description: x86_64 initial C boot environment.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

#include <kernel.h>
#include <arch/x86/cpu.h>
#include <arch/x86/idt.h>
#include <arch/x86/pic.h>
#include <arch/x86/multiboot2.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>

void boot_main(struct multiboot_header_tag* multiboot_tag, unsigned int magic)
{
    // Initialize terminal.
    kernel_write = vga_text_write;
    vga_text_initialize();
    kernel_print("Terminal initialized.\n");

    idt_initialize();
    kernel_print("IDT initialized.\n");

    // Initialize PIC (disables all IRQs).
    pic_initialize();
    kernel_print("PIC initialized.\n");

    ps2_keyboard_initialize();
    kernel_print("Keyboard initialized.\n");

    // Enable supported IRQs.
    pic_irq_enable(IRQ_PIT);
    pic_irq_enable(IRQ_KEYBOARD);

    // TODO: Set up timer.

    // TODO: Set up paging.

    // TODO: Parse Multiboot2 structure.

    // TODO: Initialize memory manager.

    asm volatile ("sti \n"); // We can safely enable interrupts now.

    kernel_main();
}
