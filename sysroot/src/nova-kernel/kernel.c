// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>

void kernel_main(void)
{
    // Set up terminal based on arch. Stub, only for kernel testing.
    kernel_write = vga_text_write;

    kernel_write("fuck", 4);
}

void kernel_panic(char* str, size_t len)
{
    // Ensures that the panic message is null-terminated.
    str[len] = '\0';

    // TODO: Print panic message and then halt.
    kernel_write(str, len);
}
