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
#include <liballoc/liballoc.h>
#include <hal/tty.h>

#include <arch/x86_64/memory/vmm.h>
#include <arch/x86_64/tss.h>

char user_stack[0x1000];

void userf(void)
{
    printf("We did it!\n");

    asm volatile ("int $0x10 \n");
}

void kernel_main(void)
{
    // Initialize STDIO.
    stdio_init();
    tty_init();
    stdin = tty_ins;
    stdout = tty_outs;
    stderr = tty_outs;

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

        if (strcmp(s, "test") == 0)
        {

            // Set up TSS.
            asm volatile
            (
                "movq %%rsp, %0\n"
                : "=a" (tss.rsp0)
                :
                :
            );

            tss.rsp2 = &user_stack + 4000 + KERNEL_OFFSET;

            // Jump to ring 3.
            asm volatile
            (
                "movw $0x23, %%ax \n"  // Load segment regs.
                "movw %%ax, %%ds \n"
                "movw %%ax, %%es \n"
                "movw %%ax, %%fs \n"
                "movw %%ax, %%gs \n"
                "pushq $0x23 \n"  // User data selector.
                "pushq $(user_stack + 4000) \n"  // User stack.
                "pushfq \n"
                "pushq $0x1B \n"   // User code selector.
                "pushq $userf \n"  // RIP to load.
                "iretq \n"
                "retloc: \n"
                :
                :
                :
            );

            // Test.
            printf("We're back from usermode.\n");
        }
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
    // Disable interrupts and halt.
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
