// Authors: Seth McBee
// Created: 2018-4-12
// Description: x86 Interrupt Service Routines - extended.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>
#include <stdlib.h>

#include <kernel.h>
#include <arch/x86/cpu.h>
#include <arch/x86/isr.h>
#include <arch/x86/pic.h>
#include <arch/x86/ps2.h>
#include <drivers/input/ps2_keyboard.h>

uint64_t irq_spurious_count = 0;
uint64_t irq_pit_count = 0;

void isr_13_ext(uint32_t error_code)
{
    // Check whether the exception was triggered externally.
    if (BIT_CHECK(error_code, ISR_ERROR_EXTERNAL_BIT))
    {
        kernel_log("\n\tGPF: Exception triggered externally.\n");
    }

    // Check which table the exception originated from.
    uint8_t table = (error_code >> ISR_ERROR_TABLE_BIT) & ISR_ERROR_TABLE_MASK;

    if (table == ISR_ERROR_TABLE_GDT)
    {
        kernel_log("\n\tGPF: Error occurred in the GDT.");
    }

    if (table == ISR_ERROR_TABLE_LDT)
    {
        kernel_log("\n\tGPF: Error occurred in the LDT.");
    }

    if (table == ISR_ERROR_TABLE_IDT1 || table == ISR_ERROR_TABLE_IDT2)
    {
        kernel_log("\n\tGPF: Error occurred in the IDT.");
    }

    // Check which selector the exception originated from.
    uint16_t selector = (error_code >> (ISR_ERROR_INDEX_BIT)) & ISR_ERROR_INDEX_MASK;
    char str[10];
    itoa(selector, str, 10);
    kernel_log("\n\tGPF: Selector index - ");
    kernel_log(str);
    kernel_log("\n");
}

void isr_32_ext(void)
{
    irq_pit_count++;
    pic_eoi(IRQ_PIT);
}

void isr_33_ext(void)
{
    if (ps2_keyboard_queue_count < PS2_KB_QUEUE_MAX)
    {
        ps2_keyboard_queue[ps2_keyboard_queue_count] = ps2_receive(PS2_DATA);
        ps2_keyboard_queue_count++;
    }
    else
    {
        ps2_receive(PS2_DATA);
    }
    pic_eoi(IRQ_KEYBOARD);
}

void isr_39_ext(void)
{

    // Check if the in-service register indicates any interrupt
    // was sent.
    cpu_outb(0x0B, 0x20);
    cpu_io_wait();

    if (!BIT_CHECK(cpu_inb(0x20), 7))
    {
        irq_spurious_count++;
        kernel_log("IRQ: Spurious interrupt.\n");
        return;
    }

    // Else, handle interrupt.
    pic_eoi(IRQ_LPT1);
}
