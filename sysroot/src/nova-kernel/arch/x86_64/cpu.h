// Authors: Seth McBee
// Created: 2018-4-11
// Description: Basic CPU functions.

#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <liballoc/liballoc.h>
#include <proc/process.h>
#include <proc/scheduler.h>

#include <arch/x86_64/tss.h>

// Writes a byte to an IO port.
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

// Writes a word to an IO port.
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

// Writes a long to an IO port.
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

// Reads a byte from an IO port.
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

// Reads a word from an IO port.
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

// Reads a long from an IO port.
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

// Waits for one IO cycle.
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

// Returns the current RDTSC time-stamp.
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

// Returns the contents of the RFLAGS register.
static inline uint64_t cpu_get_flags(void)
{
    uint64_t ret;
    asm volatile
    (
        "pushfq \n"
        "popq %0 \n"
        : "=A" (ret)
        :
        :
    );
    return (ret);
}

// Sets the contents of the RFLAGS register.
static inline void cpu_set_flags(uint64_t flags)
{
	asm volatile
	(
		"pushq %0 \n"
		"popfq \n"
		:
		: "A" (flags)
		:
	);
}

// Handles the actual task switching.
void cpu_proc_asm(Registers* proc);

// Switches context to a given process.
static inline void cpu_proc(Process* proc)
{
    // Set up the TSS.
    tss.rsp0 = proc->rsp0;

    // Jump to code.
    cpu_proc_asm(&proc->reg);
}

#endif // CPU_H
