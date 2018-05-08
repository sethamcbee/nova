// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>
#include <hal/tty.h>

#ifdef ARCH_X86_64
    #include <arch/x86_64/cpu.h>
#endif

#ifdef ARCH_X86
    #include <arch/x86/cpu.h>
#endif

extern uint64_t irq_spurious_count;
extern size_t pmm_mem_start;
extern size_t pmm_mem_free;
extern size_t pmm_mem_max;

void kernel_main(void)
{
    // Initialize STDIO.
    stdio_init();
    tty_init();
    stdout = tty_outs;
    stdin = tty_ins;

    char s[10000];
    const char user[] = "sethamcbee@nova:";
    const char dir[] = "/";

    // Kernel loop.
    while (1)
    {
        printf("%s%s$ ", user, dir);
        fflush(stdout);

        // Get user input.
        scanf("%s", s);
    }

    // The kernel is not intended to return; halt.
    fputs("\nEnd of kernel code. Halt.", stderr);
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
    for (len = 0; len <= max_len; len++)
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
    int ret = 0;
    //ret = kernel_print(s);
    return (ret);
}

__attribute__((noreturn))
void kernel_halt(void)
{
    #if defined(ARCH_X86_64) || defined(ARCH_X86)
        // Disable interrupts.
        asm volatile
        (
            "cli \n"
            "hlt \n"
        );
    #endif

    while (1)
    {
        // Do nothing.
    }
}
