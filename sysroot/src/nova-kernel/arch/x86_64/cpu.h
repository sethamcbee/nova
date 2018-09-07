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

#include <arch/x86_64/tss.h>

// Writes a byte to an IO port.
static inline void cpu_outb(uint8_t val, uint16_t port)
{
    asm volatile
    (
        "outb %0, %1\n"
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
        "outw %0, %1\n"
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
        "outl %0, %1\n"
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
        "inb %1, %0\n"
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
        "inw %1, %0\n"
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
        "inb %1, %0\n"
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
        "outb %%al, $0x80"
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
        "rdtsc"
        : "=A" (ret)
        :
        :
    );
    return (ret);
}

// Enters ring 0.
static inline void cpu_ring0(Process* proc)
{
    // Set up TSS.
    //asm volatile
    //(
    //    "movq %%rsp, %0\n"
    //    : "=a" (tss.rsp0)
    //    :
    //    :
    //);
    //void* kstk = malloc(0x1000);
    //tss.rsp0 = (size_t)kstk;

    // Set up TSS.
    tss.rsp0 = proc->rsp0;

    // Jump to ring 0.
    asm volatile
    (
        // Load segment regs.
        "movw $0x10, %%ax \n"
        "movw %%ax, %%ds \n"
        "movw %%ax, %%es \n"
        "movw %%ax, %%fs \n"
        "movw %%ax, %%gs \n"

        // Kernel data selector.
        "pushq $0x10 \n"

        // Kernel stack.
        "pushq %1 \n"

        // Eflags.
        "pushfq \n"
        // sti upon context switch.
        "orq $0x200, (%%rsp) \n"

        // Kernel code selector.
        "pushq $0x08 \n"

        // RIP to jump to.
        "pushq %0 \n"
        "iretq \n"
        :
        : "g" (proc->reg.reg_rip), "g" (proc->reg.reg_rsp)
        : "rax", "memory"
    );
}

// Enters ring 3.
static inline void cpu_ring3(Process* proc)
{
    // Set up TSS.
    //asm volatile
    //(
    //    "movq %%rsp, %0\n"
    //    : "=a" (tss.rsp0)
    //    :
    //    :
    //);
    //void* kstk = malloc(0x1000);
    //tss.rsp0 = (size_t)kstk;

    // Set up TSS.
    tss.rsp0 = proc->rsp0;

    // Jump to ring 3.
    asm volatile
    (
        // Load segment regs.
        "movw $0x23, %%ax \n"
        "movw %%ax, %%ds \n"
        "movw %%ax, %%es \n"
        "movw %%ax, %%fs \n"
        "movw %%ax, %%gs \n"

        // User data selector.
        "pushq $0x23 \n"

        // User stack.
        "pushq %1 \n"

        // Eflags.
        "pushfq \n"
        // sti upon context switch.
        "orq $0x200, (%%rsp) \n"

        // User code selector.
        "pushq $0x1B \n"

        // RIP to jump to.
        "pushq %0 \n"
        "iretq \n"
        :
        : "g" (proc->reg.reg_rip), "g" (proc->reg.reg_rsp)
        : "rax", "memory"
    );
}

#endif // CPU_H
