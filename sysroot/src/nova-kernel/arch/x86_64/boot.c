// Authors: Seth McBee
// Created: 2017-10-14
// Description: x86_64 initial C boot environment.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

#include <kernel.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/multiboot2.h>
#include <drivers/graphics/vga_text.h>

void boot_main(struct multiboot_header_tag* multiboot_tag, unsigned int magic)
{
    uint64_t time = cpu_rdtsc();

    // Initialize terminal.
    kernel_write = vga_text_write;
    vga_text_initialize();
    kernel_print("Terminal initialized.\n");

    idt_initialize();
    kernel_print("IDT initialized.\n");

    // TODO: Remap and set up IRQs.

    // TODO: Set up timer.

    // TODO: Set up paging.

    // TODO: Parse Multiboot2 structure.

    // TODO: Initialize memory manager.

    time = cpu_rdtsc() - time;
    char str[20];
    litoa(time, str, 10);
    kernel_print("\nTicks diff: ");
    kernel_print(str);
    kernel_print("\n");

    kernel_main();
}
