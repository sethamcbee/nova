// Authors: Seth McBee
// Created: 2017-10-17
// Description: x86-64 TSS layout.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/gdt.h>
#include <arch/x86_64/tss.h>

static void load_tss(void);

void tss_init(void)
{
    Tss_Descriptor *tss_descriptor = (Tss_Descriptor*)&gdt_entry[GDT_TSS / 8];
    uint64_t tss_addr = (uint64_t)&tss;
    uint32_t tss_size = sizeof(tss);

    // Define TSS entry in the GDT.
    tss_descriptor->limit_0 = 0xFFFF & (tss_size);
    tss_descriptor->base_0 = 0xFFFF & tss_addr;
    tss_descriptor->base_1 = 0xFF & (tss_addr >> 16);
    tss_descriptor->accessed = 1;
    tss_descriptor->rw = 0;
    tss_descriptor->dc = 0;
    tss_descriptor->ex = 1;
    tss_descriptor->reserved_0 = 0;
    tss_descriptor->privilege = 0;
    tss_descriptor->present = 1;
    tss_descriptor->limit_1 = 0xF & (tss_size >> 16);
    tss_descriptor->reserved_1 = 1;
    tss_descriptor->long_mode = 0;
    tss_descriptor->size = 0;
    tss_descriptor->granularity = 0;
    tss_descriptor->base_2 = 0xFF & (tss_addr >> 24);
    tss_descriptor->base_3 = 0xFFFFFFFF & (tss_addr >> 32);
    tss_descriptor->reserved_2 = 0;

    // Initialize TSS.
    tss.rsp0 = 1;   // This should point to a per-process kernel stack.
    tss.ist1 = 0;
    tss.ist2 = 0;
    tss.ist3 = 0;
    tss.ist4 = 0;
    tss.ist5 = 0;
    tss.ist6 = 0;
    tss.ist7 = 0;

    load_tss();
}

static void load_tss(void)
{
    uint16_t selector = GDT_TSS;
    asm volatile
    (
        "ltr %0 \n"
        :
        : "a" (selector)
        :
    );
}
