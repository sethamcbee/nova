// Authors: Seth McBee
// Created: 2017-10-17
// Description: IDT constants and declarations.

#ifndef IDT_H
#define IDT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Gate types.
#define IDT_INTERRUPT_GATE  0b1110
#define IDT_TASK_GATE       0b0101
#define IDT_TRAP_GATE       0b1111

// 64-bit IDT entry.
typedef struct Idt_Entry
{
    uint64_t offset;
    uint16_t selector;
    uint8_t type;
    uint8_t dpl;
} Idt_Entry;

// Stores the IDT.
uint64_t idt[256 * 2];

// Encodes an IDT entry.
void idt_encode_entry(void *pos, Idt_Entry entry);

// Set up IDT.
void idt_initialize(void);

// Loads the idt.
void idt_load(void);

#endif // IDT_H
