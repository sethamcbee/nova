# Authors: Seth McBee
# Created: 2018-4-11
# Description: Loads the IDT into the IDTR.

.set IDT_SIZE, (256 * 16 - 1)

# Define IDT pointer.
.data
.global idt_pointer
.align 16
idt_pointer:
    .word IDT_SIZE
    .long idt

.text
.global idt_load
idt_load:
    movl $idt_pointer, %eax # Load pointer to IDT to %eax
    lidt (%eax) # Tell CPU to load descriptor
    ret
