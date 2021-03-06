// Authors: Seth McBee
// Created: 2018-4-12
// Description: x86-64 Interrupt Service Routines - extended.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>
#include <stdlib.h>

#include <kernel.h>
#include <drivers/input/ps2_keyboard.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/devices/pic.h>
#include <arch/x86_64/devices/ps2.h>
#include <arch/x86_64/interrupts/isr.h>

volatile uint64_t irq_spurious_count = 0;
volatile uint64_t irq_pit_count = 0;

void isr_1_ext(uint64_t ip)
{
    // Print instruction of fault.
    char str[20];
    _itoa(ip, str, 16);
    kernel_print("\n\tDebug exception: Error occurred at address ");
    kernel_print(str);
    kernel_print("\n");
}

void isr_13_ext(uint32_t error_code)
{
    // Check whether the exception was triggered externally.
    if (BIT_CHECK(error_code, ISR_ERROR_EXTERNAL_BIT))
    {
        kernel_print("\n\tGPF: Exception triggered externally.\n");
    }

    // Check which table the exception originated from.
    uint8_t table = (error_code >> ISR_ERROR_TABLE_BIT) & ISR_ERROR_TABLE_MASK;

    if (table == ISR_ERROR_TABLE_GDT)
    {
        kernel_print("\n\tGPF: Error occurred in the GDT.");
    }

    if (table == ISR_ERROR_TABLE_LDT)
    {
        kernel_print("\n\tGPF: Error occurred in the LDT.");
    }

    if (table == ISR_ERROR_TABLE_IDT1 || table == ISR_ERROR_TABLE_IDT2)
    {
        kernel_print("\n\tGPF: Error occurred in the IDT.");
    }

    // Check which selector the exception originated from.
    uint16_t selector = (error_code >> ISR_ERROR_INDEX_BIT) & ISR_ERROR_INDEX_MASK;
    char str[10];
    itoa(selector, str);
    kernel_print("\n\tGPF: Selector index ");
    kernel_print(str);
    kernel_print("\n");
}

void isr_14_ext(uint32_t error_code, uint64_t addr)
{
    kernel_print("\nPAGE FAULT:\n");

    // Check first bit.
    if (BIT_CHECK(error_code, ISR_PAGE_PR_BIT))
    {
        kernel_print("Attempted protection violation - ");
    }
    else
    {
        kernel_print("Attempted access on non-present page - ");
    }

    // Check RW.
    if (BIT_CHECK(error_code, ISR_PAGE_RW_BIT))
    {
        kernel_print("invalid write attempted from ");
    }
    else
    {
        kernel_print("invalid read attempted from ");
    }

    // Check privilege.
    if (BIT_CHECK(error_code, ISR_PAGE_U_BIT))
    {
        kernel_print("user mode.\n");
    }
    else
    {
        kernel_print("supervisor mode.\n");
    }

    // Check for reserved bit error.
    if (BIT_CHECK(error_code, ISR_PAGE_R_BIT))
    {
        kernel_print("Reserved bit(s) incorrectly set.\n");
    }

    // Check for No-Execute bit fault.
    if (BIT_CHECK(error_code, ISR_PAGE_I_BIT))
    {
        kernel_print("Attempted execution in memory marked No-Execute.\n");
    }

    // Print address of fault.
    char str[30];
    _litoa(addr, str, 16);
    kernel_print("Address of fault: ");
    kernel_print(str);
    kernel_print("\n");
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

    ps2_keyboard_main();
}

void isr_39_ext(void)
{

    // Check if the in-service register indicates any interrupt
    // was sent.
    cpu_outb(0x0B, 0x20);

    if (!BIT_CHECK(cpu_inb(0x20), 7))
    {
        irq_spurious_count++;
        kernel_log("IRQ: Spurious interrupt.\n");
        return;
    }

    // Else, handle interrupt.
    pic_eoi(IRQ_LPT1);
}
