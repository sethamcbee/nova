// Authors: Seth McBee
// Created: 2018-4-11
// Description: PIC handling.

#ifndef PIC_H
#define PIC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// PIC IO addresses.
#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_CMD        PIC1
#define PIC1_DATA       (PIC1 + 1)
#define PIC2_CMD        PIC2
#define PIC2_DATA       (PIC2 + 1)
#define PIC_IRR_READ    0x0a
#define PIC_ISR_READ    0x0b

// PIC IRQs.
#define IRQ_PIT         0
#define IRQ_KEYBOARD    1
#define IRQ_CASCADE     2
#define IRQ_COM2        3
#define IRQ_COM1        4
#define IRQ_LPT2        5
#define IRQ_FLOPPY      6
#define IRQ_LPT1        7
#define IRQ_CMOS        8
#define IRQ_9           9
#define IRQ_10          10
#define IRQ_11          11
#define IRQ_MOUSE       12
#define IRQ_FPU         13
#define IRQ_ATA1        14
#define IRQ_ATA2        15

// PIC commands and modes.
#define PIC_EOI     0x20
#define ICW1_ICW4   0x01
#define ICW1_SING   0x02
#define ICW1_INT4   0x04
#define ICW1_LVL    0x08
#define ICW1_INIT   0x10
#define ICW4_8086   0x01
#define ICW4_AUTO   0x02
#define ICW4_BUF1   0x08
#define ICW4_BUF2   0x0C
#define ICW4_SFNM   0x10

// Initializes the PIC and remaps IRQs.
void pic_initialize(void);

// Sends End of Interrupt signal to the PIC.
void pic_eoi(uint8_t irq);

// Disables an IRQ line.
void pic_irq_disable(uint8_t irq);

// Enables an IRQ line.
void pic_irq_enable(uint8_t irq);

// Retrieves the IRR.
uint16_t pic_irr_get(void);

// Retrieves the ISR.
uint16_t pic_isr_get(void);

#endif // PIC_H
