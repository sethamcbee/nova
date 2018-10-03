/**
 * @file kernel.c
 * @author Seth McBee
 * @date 2017-10-14
 * @brief Kernel entry point.
 */

#include <globals.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>
#include <hal/tty.h>
#include <liballoc/liballoc.h>
#include <proc/process.h>
#include <proc/scheduler.h>

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

volatile int a;

void fun1(void)
{
    a = 100;

    while (1)
    {
        a++;
        for (size_t i = 0; i < 0x100000; i++)
            asm volatile ("nop \n" : : : "memory");
    }

    asm volatile ("int $0x10 \n");
}

void fun2(void)
{
    char s[100];
    while (1)
    {
        printf("user:/$ ");
        scanf("%s", s);
        if (strcmp(s, "show") == 0)
        {
            printf("%d\n", a);
        }
    }

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
            char* contents = vmm_page_alloc_kernel();
            vmm_page_map(kernel_module, contents, PG_PR | PG_RW | PG_U);
            printf("%s\n", contents);
        }
        if (strcmp(s, "test") == 0)
        {
            char* kstk1 = vmm_page_alloc_kernel();
            vmm_table_flags((void*)kstk1, PG_PR | PG_RW | PG_U);
            char* ustk1 = vmm_page_alloc_kernel();
            vmm_table_flags((void*)ustk1, PG_PR | PG_RW | PG_U);

            char* kstk2 = vmm_page_alloc_kernel();
            vmm_table_flags((void*)kstk2, PG_PR | PG_RW | PG_U);
            char* ustk2 = vmm_page_alloc_kernel();
            vmm_table_flags((void*)ustk2, PG_PR | PG_RW | PG_U);

            // Create process 1.
            Process* proc1 = malloc(sizeof(Process));
            proc1->reg.flags = cpu_get_flags();
            proc1->reg.ss = GDT_USER_DATA | RING3;
            proc1->reg.cs = GDT_USER_CODE | RING3;
            proc1->kernel_stack = (size_t)kstk1 + 0x1000;
            proc1->priv = 3;
            Task* task1 = malloc(sizeof(Task));
            task1->proc = proc1;
            task1->ticks = DEFAULT_TICKS;

            // Create process 2.
            Process* proc2 = malloc(sizeof(Process));
            proc2->reg.flags = cpu_get_flags();
            proc2->reg.ss = GDT_USER_DATA | RING3;
            proc2->reg.cs = GDT_USER_CODE | RING3;
            proc2->kernel_stack = (size_t)kstk2 + 0x1000;
            proc2->priv = 3;
            Task* task2 = malloc(sizeof(Task));
            task2->proc = proc2;
            task2->ticks = DEFAULT_TICKS;
            
            #ifdef ARCH_X86_64
            proc1->reg.rip = (size_t)fun1;
            proc1->reg.rsp = (size_t)ustk1 + 0x1000;
            proc2->reg.rip = (size_t)fun2;
            proc2->reg.rsp = (size_t)ustk2 + 0x1000;
            #endif // ARCH_X86_64
            #ifdef ARCH_X86
            proc1->reg.eip = (size_t)fun1;
            proc1->reg.esp = (size_t)ustk1 + 0x1000;
            proc2->reg.eip = (size_t)fun2;
            proc2->reg.esp = (size_t)ustk2 + 0x1000;
            #endif // ARCH_X86

            // Queue processes.
            task_add(task2);
            task_add(task1);
            
            // TEST.
            while (1)
            {
                a--;
                for (size_t i = 0; i < 0x100000; i++)
                    asm volatile ("nop \n" : : : "memory");
            }
        }
    }
    // The kernel is not intended to return; halt.
    fputs("\nEnd of kernel code. Halt.", stderr);
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
