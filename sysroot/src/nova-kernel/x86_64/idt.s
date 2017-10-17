# Authors: Seth McBee
# Created: 2017-10-17
# Description: IDT setup.

#include <src/nova-kernel/x86_64/idt.h>

.code64

.extern isr_0
.extern isr_1
.extern isr_2
.extern isr_3
.extern isr_4
.extern isr_5
.extern isr_6
.extern isr_7
.extern isr_8
.extern isr_9
.extern isr_10
.extern isr_11
.extern isr_12
.extern isr_13
.extern isr_14
.extern isr_15
.extern isr_16
.extern isr_17
.extern isr_18
.extern isr_19
.extern isr_20
.extern isr_21
.extern isr_22
.extern isr_23
.extern isr_24
.extern isr_25
.extern isr_26
.extern isr_27
.extern isr_28
.extern isr_29
.extern isr_30
.extern isr_31

.set IDT_SIZE, (Lidt_end - Lidt_start - 1) # Size of table minus 1.
.set ISR_0_LOW, (isr_0 & 0xFFFF)
.set ISR_0_MID, (isr_0 << 16)

# Define IDT pointer.
.section .data
.global idt_pointer
idt_pointer:
	.word IDT_SIZE
	.quad Lidt_start # Address of IDT.

# Define IDT.
.section .data
Lidt_start:
Lidt_0:
	.word ISR_0_LOW
Lidt_end:
