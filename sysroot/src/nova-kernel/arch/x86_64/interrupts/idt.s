# Authors: Seth McBee
# Created: 2018-4-11
# Description: Loads the IDT into the IDTR.

.set IDT_SIZE, (256 * 16 - 1)

# Define IDT pointer.
.text
.global idt_pointer
.align 16
idt_pointer:
    .word IDT_SIZE
    .quad idt

.global idt_load
idt_load:
    movabs $idt_pointer, %rax # Load pointer to IDT to %eax
    lidt (%rax) # Tell CPU to load descriptor
    ret
