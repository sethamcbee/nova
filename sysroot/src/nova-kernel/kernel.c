// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>

void kernel_main(void)
{
    asm volatile ("sti \n"); // We can safely enable interrupts now.

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
    #ifdef ARCH_X86_64
        // Disable interrupts.
        asm volatile ("sti \n");
    #endif

    #ifdef ARCH_X86
        asm volatile ("sti \n");
    #endif

    while (1)
    {
        // Do nothing.
    }
}
