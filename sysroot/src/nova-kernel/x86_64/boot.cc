#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <boot/cpu.h>
#include <boot/memory_map.h>
#include <boot/multiboot.h>
#include <boot/drivers/graphics/vga_color_text_mode.h>
#include <boot/kernel/kernel.h>
#include <boot/ui/terminal.h>

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