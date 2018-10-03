/**
 * @file cpu.h
 * @author Seth McBee
 * @date 2018-4-11
 * @brief Miscellaneous CPU functions.
 */

#pragma once

#include <globals.h>

#include <liballoc/liballoc.h>
#include <proc/process.h>
#include <proc/scheduler.h>

#include <arch/x86/tss.h>
#include <arch/x86/registers.h>

/// Write a byte to an IO port.
static inline void cpu_outb(uint8_t val, uint16_t port)
{
    asm volatile
    (
        "outb %0, %1 \n"
        :
        : "a" (val), "Nd" (port)
        : "memory"
    );
}

/// Write a word to an IO port.
static inline void cpu_outw(uint16_t val, uint16_t port)
{
    asm volatile
    (
        "outw %0, %1 \n"
        :
        : "a" (val), "Nd" (port)
        : "memory"
    );
}

/// Write a long to an IO port.
static inline void cpu_outl(uint32_t val, uint16_t port)
{
    asm volatile
    (
        "outl %0, %1 \n"
        :
        : "a" (val), "Nd" (port)
        : "memory"
    );
}

/// Read a byte from an IO port.
static inline uint8_t cpu_inb(uint16_t port)
{
    uint8_t val;

    asm volatile
    (
        "inb %1, %0 \n"
        : "=a" (val)
        : "Nd" (port)
        : "memory"
    );

    return (val);
}

/// Read a word from an IO port.
static inline uint16_t cpu_inw(uint16_t port)
{
    uint16_t val;

    asm volatile
    (
        "inw %1, %0 \n"
        : "=a" (val)
        : "Nd" (port)
        : "memory"
    );

    return (val);
}

/// Read a long from an IO port.
static inline uint32_t cpu_inl(uint16_t port)
{
    uint32_t val;

    asm volatile
    (
        "inb %1, %0 \n"
        : "=a" (val)
        : "Nd" (port)
        : "memory"
    );

    return (val);
}

/// Wait for one IO cycle.
static inline void cpu_io_wait(void)
{
    asm volatile
    (
        "outb %%al, $0x80 \n"
        :
        : "a"(0)
        : "memory"
    );
}

/// Get the current RDTSC time-stamp.
static inline uint64_t cpu_rdtsc(void)
{
    uint64_t ret;
    asm volatile
    (
        "rdtsc \n"
        : "=A" (ret)
        :
        :
    );
    return (ret);
}

/// Get the contents of the RFLAGS register.
static inline uint32_t cpu_get_flags(void)
{
    uint32_t ret;
    asm volatile
    (
        "pushf \n"
        "popl %0 \n"
        : "=A" (ret)
        :
        :
    );
    return (ret);
}

/// Set the contents of the RFLAGS register.
static inline void cpu_set_flags(uint32_t flags)
{
	asm volatile
	(
		"pushl %0 \n"
		"popf \n"
		:
		: "A" (flags)
		:
	);
}

/// Handle the actual task switching.
void cpu_proc_asm(Registers* proc);

/// Switche context to a given process.
static inline void cpu_proc(Process* proc)
{
    // Set up the TSS.
    tss.esp0 = proc->kernel_stack;

    // Jump to code.
    cpu_proc_asm(&proc->reg);
}
