// Authors: Seth McBee
// Created: 2017-10-14
// Description: x86_64 initial C boot environment.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/graphics/vga_color_text_mode.h>
#include <kernel.h>
#include <ui/terminal.h>
#include <x86_64/cpu.h>
#include <x86_64/memory_map.h>
#include <x86_64/multiboot.h>

void boot_main(multiboot_info_t* mbd, unsigned int magic)
{
    /* Makes sure interrupts are disabled, since the interrupt handler hasn't
     * been enabled yet. */
    cpu_disable_interrupts();

    /* Make permanent copy of memory map. This is done first to preserve
     * the integrity of the structure, preventing the memory manager from
     * allocating it's memory. */
    multiboot_get_multiboot_info(mbd);

    /* Initialize memory manager. */
    memory_initialize();

    /* Set terminal driver. */
    terminal_driver_set(vga_color_text_mode_get_driver());

    /* Initialize terminal. */
    terminal_initialize();

    terminal_write_string("Booted.\n");

    /* Call kernel. */
    kernel_main();
}