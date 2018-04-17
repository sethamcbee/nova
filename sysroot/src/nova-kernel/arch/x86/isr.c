// Authors: Seth McBee
// Created: 2018-4-12
// Description: Interrupt Service Routines - extended.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>
#include <stdlib.h>

#include <kernel.h>
#include <arch/x86_64/cpu.h>
#include <arch/x86_64/isr.h>
#include <arch/x86_64/pic.h>
#include <arch/x86_64/ps2.h>
#include <drivers/input/ps2_keyboard.h>

uint64_t irq_spurious_count = 0;
uint64_t irq_pit_count = 0;

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

    if (!BIT_CHECK(cpu_inb(0x20), 7))
    {
        irq_spurious_count++;
        kernel_log("IRQ: Spurious interrupt.\n");
        return;
    }

    // Else, handle interrupt.
    pic_eoi(IRQ_LPT1);
}
