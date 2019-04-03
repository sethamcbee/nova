/**
 * @file kernel.c
 * @author Seth McBee
 * @date 2017-10-14
 * @brief Kernel entry point.
 */

#include <globals.h>

#include <array>
#include <initializer_list>
#include <list>
#include <memory>
#include <type_traits>
#include <vector>

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

#include <arch/x86_64/devices/pit.h>

size_t ms_to_ticks(size_t ms)
{
    return (float)ms * PIT_REAL_FREQ / 1000.0f;
}

size_t ticks_to_ms(size_t t)
{
    return (float)t * 1000.0f / PIT_REAL_FREQ;
}

void wait_ticks(size_t t)
{
    auto start = irq_pit_count;
    auto volatile* end = &irq_pit_count;
    while (*end - start < t)
    {
        // Busy-wait.
    }
}

void wait_ms(size_t ms)
{
    auto ticks = ms_to_ticks(ms);
    wait_ticks(ticks);
}

ssize_t (*kernel_write)(const void*, size_t) = NULL;
struct multiboot_tag_module* kernel_module = NULL;

void kernel_test()
{
    pit_info();
    
    extern uint16_t rtc_sec_init;
    extern uint16_t rtc_min_init;
    extern uint16_t rtc_hour_init;
    extern uint8_t rtc_weekday_init;
    extern uint8_t rtc_day_init;
    extern uint8_t rtc_mon_init;
    extern uint8_t rtc_year_init;
    
    uint64_t uptime = ticks_to_ms(irq_pit_count) / 1000;
    uint64_t sec = rtc_sec_init;
    uint64_t min = rtc_min_init;
    uint64_t hour = rtc_hour_init;
    uint64_t day = rtc_day_init;
    uint64_t mon = rtc_mon_init;
    uint64_t year = rtc_year_init;
    
    sec += uptime;
    while (sec > 59)
    {
        sec -= 59;
        ++min;
    }
    while (min > 59)
    {
        min -= 59;
        ++hour;
    }
    while (hour > 23)
    {
        hour -= 23;
        ++day;
    }
    
    printf("20%d-%d-%d %d:%d:%d\n", year, mon, day, hour, min, sec);
}

void kernel_main()
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

    kernel_test();

    // Kernel loop.
    while (1)
    {
        printf("%s%s$ ", user, dir);
        fflush(stdout);

        // Get user input.
        scanf("%s", s);
        if (strcmp(s, "module") == 0)
        {
            char* module_page = (char*)vmm_page_alloc_kernel();
            vmm_page_map((void*)kernel_module, module_page, PG_PR | PG_RW | PG_U);
            
            printf("%s\n", module_page);
        }
        if (strcmp(s, "timer") == 0)
        {
            auto start = *&irq_pit_count;
            scanf("%s", s);
            auto end = *&irq_pit_count;
            
            auto ms = ticks_to_ms(end - start);
            printf("time: %d ms\n", ms);
        }
        if (strcmp(s, "ticks") == 0)
        {
            auto ticks = irq_pit_count;
            printf("ticks: %d\n", ticks);
            printf("uptime: %f sec\n", (double)ticks_to_ms(ticks) / 1000);
        }
        if (strcmp(s, "sleep") == 0)
        {
            float sec;
            printf("sec: ");
            scanf("%f", &sec);
            size_t i = 1;
            float whole = sec;
            while (whole > 1.0001)
            {
                wait_ms(1000);
                printf("%ds...\n", i);
                ++i;
                fflush(stdout);
                --whole;
            }
            wait_ms(whole * 1000);
            printf("%fs.\n", sec);
        }
        if (strcmp(s, "date") == 0)
        {
            extern uint16_t rtc_sec_init;
            extern uint16_t rtc_min_init;
            extern uint16_t rtc_hour_init;
            extern uint8_t rtc_weekday_init;
            extern uint8_t rtc_day_init;
            extern uint8_t rtc_mon_init;
            extern uint8_t rtc_year_init;
            
            uint64_t uptime = ticks_to_ms(irq_pit_count) / 1000;
            uint64_t sec = rtc_sec_init;
            uint64_t min = rtc_min_init;
            uint64_t hour = rtc_hour_init;
            uint64_t day = rtc_day_init;
            uint64_t mon = rtc_mon_init;
            uint64_t year = rtc_year_init;
            
            sec += uptime;
            while (sec > 59)
            {
                sec -= 59;
                ++min;
            }
            while (min > 59)
            {
                min -= 59;
                ++hour;
            }
            while (hour > 23)
            {
                hour -= 23;
                ++day;
            }
            
            printf("20%d-%d-%d %d:%d:%d\n", year, mon, day, hour, min, sec);
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
