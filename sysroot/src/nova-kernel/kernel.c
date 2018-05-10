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

#include <arch/x86_64/tss.h>

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

    extern pmm_frames_free;
    extern pmm_frames_available;
    extern pmm_frames_unavailable;
    printf("Free page frames: %ld\n", pmm_frames_free);
    printf("Available page frames: %ld\n", pmm_frames_available);
    printf("Unavailable page frames: %ld\n", pmm_frames_unavailable);

    // Kernel loop.
    while (1)
    {
        printf("%s%s$ ", user, dir);
        fflush(stdout);

        // Get user input.
        scanf("%s", s);

        if (strcmp(s, "alloc") == 0)
        {
            extern size_t pmm_bitmap_alloc(void);
            size_t addr = pmm_bitmap_alloc();
            fprintf(stdout, "Address: %ld\n", addr);
            for (size_t i = 0; i < 0x1000; i++)
                ((char*)addr)[i] = 0xFF;
        }
        if (strcmp(s, "free") == 0)
        {
            extern void pmm_bitmap_free(size_t addr);
            size_t addr;
            printf("Address to free: ");
            scanf("%ld", &addr);
            pmm_bitmap_free(addr);
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
