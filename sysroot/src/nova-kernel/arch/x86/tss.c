/**
 * @file tss.c
 * @author Seth McBee
 * @date 2018-10-3
 * @brief x86 TSS management.
 */

#include <globals.h>

#include <arch/x86/gdt.h>
#include <arch/x86/tss.h>

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
    tss_descriptor->privilege = 3;
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
    tss.esp0 = 0;   // This should point to a per-process kernel stack.

    tss_load(RING3);
}

void tss_load(uint16_t mask)
{
    uint16_t selector = GDT_TSS;
    selector |= mask;
    asm volatile
    (
        "ltr %0 \n"
        :
        : "a" (selector)
        :
    );
}
