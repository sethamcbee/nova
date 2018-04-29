// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>

void kernel_main(void)
{
    // Initialize STDIO.
    stdio_init();

    // Kernel loop.
    while (1)
    {
        ps2_keyboard_main();
    }

    // The kernel is not intended to return; halt.
    kernel_print("\nEnd of kernel code. Halt.");
    kernel_halt();
}

__attribute__((noreturn))
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
    size_t max_len = 100000;

    // Find the length of the string.
    for (len = 0; len < max_len; len++)
    {
        if (s[len] == '\0')
        {
            break;
        }

        if (len == max_len)
        {
            kernel_print("!!kernel_print timeout!!");
            return (-1);
        }
    }

    int ret = kernel_write(s, len);
    return (ret);
}

int kernel_log(const char *s)
{
    // Later on, this might be stored elsewhere, but for now
    // we'll just print it if debugging.
    int ret = 0;
    //ret = kernel_print(s);
    return (ret);
}

__attribute__((noreturn))
void kernel_halt(void)
{
    #if defined(ARCH_X86_64) || defined(ARCH_X86)
        // Disable interrupts.
        asm volatile ("cli \n");
    #endif

    while (1)
    {
        // Do nothing.
    }
}
