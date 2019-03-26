/**
 * @file kernel.c
 * @author Seth McBee
 * @date 2017-10-14
 * @brief Kernel entry point.
 */

#include <globals.h>

#include <memory>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>
#include <hal/tty.h>
#include <liballoc/liballoc.h>
#include <proc/process.h>
#include <proc/scheduler.h>
#include <fs/vfs.h>

#ifdef ARCH_X86_64
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/memory/vmm.h>
#endif // ARCH_X86_64

#ifdef ARCH_X86
#include <arch/x86/cpu.h>
#include <arch/x86/gdt.h>
#include <arch/x86/memory/vmm.h>
#endif // ARCH_X86

struct multiboot_tag_module* kernel_module = NULL;

void kernel_main(void)
{
    // Initialize VFS.
    vfs_init();

    // TEST.
    vfs_create_dnode("/bin", DNODE_DIR);
    vfs_find_dnode("/bin");
    vfs_create_dnode("/lib", DNODE_DIR);
    vfs_create_dnode("/var", DNODE_DIR);
    vfs_create_dnode("/usr", DNODE_DIR);
    vfs_create_dnode("/usr/bin", DNODE_DIR);

    // Initialize STDIO.
    stdio_init();
    tty_init();
    stdin = tty_ins;
    stdout = tty_outs;
    stderr = tty_outs;

    char s[10000];
    const char user[] = "kernel@nova:";
    const char dir[] = "/";

    // Set up scheduler.
    Process kernel_proc;
    kernel_proc.priv = 0;
    kernel_proc.kernel_stack = 0;
    Task kernel_task;
    kernel_task.proc = &kernel_proc;
    kernel_task.ticks = DEFAULT_TICKS;
    kernel_task.next = &kernel_task;
    cur_task = &kernel_task;

    // Kernel loop.
    while (1)
    {
        printf("%s%s$ ", user, dir);
        fflush(stdout);

        // Get user input.
        scanf("%s", s);
        if (strcmp(s, "module") == 0)
        {
            char* contents = (char*)vmm_page_alloc_kernel();
            vmm_page_map((void*)kernel_module, contents, PG_PR | PG_RW | PG_U);
            printf("%s\n", contents);
        }
    }

    // The kernel is not intended to return; halt.
    fputs("\nEnd of kernel code. Halt.", stderr);
    kernel_halt();
}

void kernel_panic(const char* str)
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
            kernel_write(s, len - 1);
            kernel_print("\n!!kernel_print timeout!!\n");
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
