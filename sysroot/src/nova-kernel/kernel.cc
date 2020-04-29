/**
 * @file kernel.c
 * @author Seth McBee
 * @date 2017-10-14
 * @brief Kernel entry point.
 */

#include <globals.h>

#include <array>
#include <deque>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <stack>
#include <thread>
#include <tuple>
#include <type_traits>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <drivers/graphics/vga_text.h>
#include <drivers/input/ps2_keyboard.h>
#include <hal/tty.h>
#include <liballoc/liballoc.h>
#include <proc/process.h>
#include <proc/thread.h>

#ifdef ARCH_X86_64
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/gdt.h>
#include <arch/x86_64/thread_state.h>
#include <arch/x86_64/memory/pmm.h>
#include <arch/x86_64/memory/vmm.h>
#endif // ARCH_X86_64

#ifdef ARCH_X86
#include <arch/x86/cpu.h>
#include <arch/x86/gdt.h>
#include <arch/x86/memory/vmm.h>
#endif // ARCH_X86

#include <arch/x86_64/devices/pit.h>

Thread* volatile current_thread;
Thread* null_thread;
Thread* kernel_thread;

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

void print_date()
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

    min += sec / 60;
    sec = sec % 60;

    hour += min / 60;
    min = min % 60;

    day += hour / 24;
    hour = hour % 24;

    printf("20%d-%d-%d %d:%d:%d", year, mon, day, hour, min, sec);
}

ssize_t (*kernel_write)(const void*, size_t) = NULL;
struct multiboot_tag_module kernel_module;

void kernel_test()
{
}

void kernel_start()
{
    // Initialize VFS.

    // Initialize STDIO.
    stdio_init();
    tty_init();
    stdin = tty_ins;
    stdout = tty_outs;
    stderr = tty_outs;

    // Set up scheduler and run main kernel process.
    null_thread = new Thread;
    current_thread = null_thread;
    kernel_thread = thread_spawn((void*)kernel_main);
    thread_switch(kernel_thread);

    // The kernel is not intended to return; halt.
    fputs("\nEnd of kernel code. Halt.", stdout);
    fflush(stdout);
    kernel_halt();
}

void kernel_main()
{
    kernel_test();

    char s[1000];
    const char user[] = "kernel@nova:";
    const char dir[] = "/";

    // Kernel loop.
    while (1)
    {
        printf("%s%s$ ", user, dir);
        fflush(stdout);

        // Get user input.
        scanf("%s", s);
        if (strcmp(s, "module") == 0)
        {
            static char* module_page = nullptr;

            // Only map module once.
            if (module_page == nullptr)
            {
                module_page = (char*)vmm_page_alloc_kernel();
                vmm_page_map((void*)(uintptr_t)kernel_module.mod_start, module_page, PG_PR | PG_RW | PG_U);
            }

            size_t mod_len = kernel_module.mod_end - kernel_module.mod_start;
            mod_len = std::min(mod_len, (size_t)PAGE_SIZE);
            kernel_write(module_page, mod_len);
            puts("");
        }
        else if (strcmp(s, "mem") == 0)
        {
            float conversion = 4096.0 / 1024.0 / 1024.0;
            float free_memory = ((float)pmm_frames_free) * conversion;
            float used_memory = ((float)pmm_frames_used) * conversion;
            float available_memory = ((float)pmm_frames_available) * conversion;
            float unavailable_memory = ((float)pmm_frames_unavailable) * conversion;
            printf("Usable memory:   %fMiB\n", available_memory);
            printf("Reserved memory: %fMiB\n", unavailable_memory);
            printf("Free memory:     %fMiB\n", free_memory);
            printf("Used memory:     %fMiB\n", used_memory);
        }
        else if (strcmp(s, "vector") == 0)
        {
            float n;
            printf("size: ");
            scanf("%f", &n);
            std::vector<uint8_t> v;
            v.resize((size_t)n);
            for (auto& a : v)
                a = 4;
            printf("done\n");
        }
        else if (strcmp(s, "malloc") == 0)
        {
            float mb;
            printf("mb: ");
            scanf("%f", &mb);
            size_t bytes = mb * 1024 * 1024;
            void* discard = malloc(bytes);
            printf("address: %ld\n", (size_t)discard);
        }
        else if (strcmp(s, "free") == 0)
        {
            size_t addr;
            printf("addr: ");
            scanf("%ld", &addr);
            free((void*)addr);
        }
        else if (strcmp(s, "timer") == 0)
        {
            auto start = *&irq_pit_count;
            scanf("%s", s);
            auto end = *&irq_pit_count;

            auto ms = ticks_to_ms(end - start);
            printf("time: %d ms\n", ms);
        }
        else if (strcmp(s, "ticks") == 0)
        {
            auto ticks = irq_pit_count;
            printf("ticks: %d\n", ticks);
            printf("uptime: %f sec\n", (double)ticks_to_ms(ticks) / 1000);
        }
        else if (strcmp(s, "sleep") == 0)
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
        else if (strcmp(s, "date") == 0)
        {
            print_date();
            puts("");
        }
        else
        {
            printf("Command not recognized.\n");
        }
    }
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
