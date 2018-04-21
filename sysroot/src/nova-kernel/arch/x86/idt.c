// Authors: Seth McBee
// Created: 2018-4-11
// Description: Initializes the IDT.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
#include <arch/x86/isr.h>

void idt_encode_entry(void *pos, Idt_Entry entry)
{
    // Used to iterate through the structure by byte.
    uint8_t* it = pos;

    *((uint16_t*)it) = (uint16_t) entry.offset;
    it = &it[2];
    *((uint16_t*)it) = entry.selector;
    it = &it[2];
    it[0] = 0;
    it[1] = entry.type | (1 << 7) | (entry.dpl << 5);
    it = &it[2];
    *((uint16_t*)it) = (uint16_t) (entry.offset >> 16);
}

void idt_initialize(void)
{
    Idt_Entry entry;

    //// Exceptions. ////

    // Divide-by-zero Error.
    entry.offset = (uint32_t) isr_0;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[0], entry);

    // Debug.
    entry.offset = (uint32_t) isr_1;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[1 * 2], entry);

    // NMI.
    entry.offset = (uint32_t) isr_2;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[2 * 2], entry);

    // Breakpoint.
    entry.offset = (uint32_t) isr_3;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[3 * 2], entry);

    // Overflow.
    entry.offset = (uint32_t) isr_4;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[4 * 2], entry);

    // Bound range exceeded.
    entry.offset = (uint32_t) isr_5;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[5 * 2], entry);

    // Invalid opcode.
    entry.offset = (uint32_t) isr_6;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[6 * 2], entry);

    // Device not available.
    entry.offset = (uint32_t) isr_7;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[7 * 2], entry);

    // Double fault.
    entry.offset = (uint32_t) isr_8;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[8 * 2], entry);

    // Coprocessor segment overrun.
    entry.offset = (uint32_t) isr_9;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[9 * 2], entry);

    // Invalid TSS.
    entry.offset = (uint32_t) isr_10;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[10 * 2], entry);

    // Segment not present.
    entry.offset = (uint32_t) isr_11;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[11 * 2], entry);

    // Stack-segment fault.
    entry.offset = (uint32_t) isr_12;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[12 * 2], entry);

    // GPF.
    entry.offset = (uint32_t) isr_13;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[13 * 2], entry);

    // Page fault.
    entry.offset = (uint32_t) isr_14;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[14 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_15;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[15 * 2], entry);

    // x86 floating-point exception.
    entry.offset = (uint32_t) isr_16;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[16 * 2], entry);

    // Alignment check.
    entry.offset = (uint32_t) isr_17;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[17 * 2], entry);

    // Machine check.
    entry.offset = (uint32_t) isr_18;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[18 * 2], entry);

    // SIMD floating-point exception.
    entry.offset = (uint32_t) isr_19;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[19 * 2], entry);

    // Virtualization exception.
    entry.offset = (uint32_t) isr_20;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[20 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_21;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[21 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_22;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[22 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_23;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[23 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_24;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[24 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_25;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[25 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_26;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[26 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_27;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[27 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_28;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[28 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_29;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[29 * 2], entry);

    // Security exception.
    entry.offset = (uint32_t) isr_30;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[30 * 2], entry);

    // Reserved.
    entry.offset = (uint32_t) isr_31;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_TRAP_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[31 * 2], entry);

    //// IRQs. ////

    // PIT.
    entry.offset = (uint32_t) isr_32;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[32 * 2], entry);

    // Keyboard IRQ.
    entry.offset = (uint32_t) isr_33;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[33 * 2], entry);

    // Cascade.
    entry.offset = (uint32_t) isr_34;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[34 * 2], entry);

    // COM2.
    entry.offset = (uint32_t) isr_35;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[35 * 2], entry);

    // COM1.
    entry.offset = (uint32_t) isr_36;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[36 * 2], entry);

    // LPT2.
    entry.offset = (uint32_t) isr_37;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[37 * 2], entry);

    // Floppy.
    entry.offset = (uint32_t) isr_38;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[38 * 2], entry);

    // LPT1.
    entry.offset = (uint32_t) isr_39;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[39 * 2], entry);

    // CMOS.
    entry.offset = (uint32_t) isr_40;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[40 * 2], entry);

    // IRQ 9.
    entry.offset = (uint32_t) isr_41;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[41 * 2], entry);

    // IRQ 10.
    entry.offset = (uint32_t) isr_42;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[42 * 2], entry);

    // IRQ 11.
    entry.offset = (uint32_t) isr_43;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[43 * 2], entry);

    // PS2 Mouse.
    entry.offset = (uint32_t) isr_44;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[44 * 2], entry);

    // FPU.
    entry.offset = (uint32_t) isr_45;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[45 * 2], entry);

    // ATA1.
    entry.offset = (uint32_t) isr_46;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[46 * 2], entry);

    // ATA2.
    entry.offset = (uint32_t) isr_47;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 0;
    idt_encode_entry(&idt[47 * 2], entry);

    //// Other. ////

    // Syscall.
    entry.offset = (uint32_t) isr_80;
    entry.selector = GDT_KERNEL_CODE;
    entry.type = IDT_INTERRUPT_GATE;
    entry.dpl = 3;
    idt_encode_entry(&idt[80 * 2], entry);

    // Load the descriptor.
    idt_load();
}
