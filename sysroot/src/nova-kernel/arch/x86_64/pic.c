// Authors: Seth McBee
// Created: 2018-4-11
// Description: PIC handling.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/pic.h>

void pic_initialize(void)
{
    uint8_t offset1 = 32;
    uint8_t offset2 = offset1 + 8;
    uint8_t mask1;
    uint8_t mask2;

    // Disable all IRQs.
    for (uint8_t i = 0; i < 16; i++)
    {
        pic_irq_disable(i);
    }

    // Backup IRQ masks.
    mask1 = cpu_inb(PIC1_DATA);
    mask2 = cpu_inb(PIC2_DATA);

    // Setup IRQ offsets.
    cpu_outb(ICW1_INIT + ICW1_ICW4, PIC1_CMD);
    cpu_io_wait();
    cpu_outb(ICW1_INIT + ICW1_ICW4, PIC2_CMD);
    cpu_io_wait();
    cpu_outb(offset1, PIC1_DATA);
    cpu_io_wait();
    cpu_outb(offset2, PIC2_DATA);
    cpu_io_wait();
    cpu_outb(4, PIC1_DATA);
    cpu_io_wait();
    cpu_outb(2, PIC2_DATA);
    cpu_io_wait();

    // Set mode.
    cpu_outb(ICW4_8086, PIC1_DATA);
    cpu_io_wait();
    cpu_outb(ICW4_8086, PIC2_DATA);
    cpu_io_wait();

    // Restore IRQ masks.
    cpu_outb(mask1, PIC1_DATA);
    cpu_io_wait();
    cpu_outb(mask2, PIC2_DATA);
    cpu_io_wait();
}

void pic_eoi(uint8_t irq)
{
    if (irq > 7)
    {
        cpu_outb(PIC_EOI, PIC2_CMD);
        cpu_io_wait();
    }

    cpu_outb(PIC_EOI, PIC1_CMD);
    cpu_io_wait();
}

void pic_irq_disable(uint8_t irq)
{
    uint16_t port;
    uint8_t val;

    // Determine which PIC to send to.
    if (irq > 7)
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    else
    {
        port = PIC1_DATA;
    }

    val = cpu_inb(port) | (1 << irq);
    cpu_outb(val, port);
    cpu_io_wait();
}

void pic_irq_enable(uint8_t irq)
{
    uint16_t port;
    uint8_t val;

    // Determine which PIC to send to.
    if (irq > 7)
    {
        port = PIC2_DATA;
        irq -= 8;
    }
    else
    {
        port = PIC1_DATA;
    }

    val = cpu_inb(port) & ~(1 << irq);
    cpu_outb(val, port);
    cpu_io_wait();
}

uint16_t pic_irr_get(void)
{
    uint16_t ret;

    cpu_outb(PIC_IRR_READ, PIC1_CMD);
    cpu_outb(PIC_IRR_READ, PIC2_CMD);
    ret = cpu_inb(PIC1_CMD) | (cpu_inb(PIC2_CMD) << 8);

    return (ret);
}

uint16_t pic_isr_get(void)
{
    uint16_t ret;

    cpu_outb(PIC_ISR_READ, PIC1_CMD);
    cpu_outb(PIC_ISR_READ, PIC2_CMD);
    ret = cpu_inb(PIC1_CMD) | (cpu_inb(PIC2_CMD) << 8);

    return (ret);
}
