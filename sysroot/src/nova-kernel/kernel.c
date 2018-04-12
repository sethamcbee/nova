// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <arch/x86_64/cpu.h>
#include <drivers/graphics/vga_text.h>

void kernel_main(void)
{
    // The kernel is not intended to return; halt.
    kernel_print("\nEnd of kernel code. Halt.");
    kernel_halt();
}

void kernel_panic(char* str)
{
    // Just pass the string to print for now.
    kernel_print("\n\nKERNEL PANIC:\n\n");
    kernel_print(str);

    // Infinite loop.
    kernel_halt();
}

int kernel_print(const char *s)
{
    size_t len;
    size_t timeout = 100000;

    // Find the length of the string.
    for (len = 0; len <= timeout; len++)
    {
        if (s[len] == '\0')
        {
            break;
        }

        if (len == timeout)
        {
            kernel_print("!!kernel_print timeout!!");
            return (-1);
        }
    }

    int ret = kernel_write(s, len);
    return (ret);
}

void kernel_halt(void)
{
    volatile int loop = 1;
    while (loop == 1)
    {
        // Do nothing.
    }
}
