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

    asm ("   \n");

    kernel_print("Help\n  me");
}

void kernel_panic(char* str)
{
    // Just pass the string to print for now.
    kernel_print("\n\nKERNEL PANIC:\n\n");
    kernel_print(str);

    // Infinite loop.
    volatile int loop = 1;
    while (loop == 1)
    {
        // Do nothing.
    }
}

int kernel_print(const char *s)
{
    size_t len;
    size_t timeout = 100000000;

    // Find the length of the string.
    for (len = 0; len < timeout; len++)
    {
        if (s[len] == '\0')
        {
            break;
        }
    }

    int ret = kernel_write(s, len);
    return (ret);
}
