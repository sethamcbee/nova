# Authors: Seth McBee
# Created: 2017-10-17
# Description: GDT setup.

#include <src/nova-kernel/x86_64/gdt.h>

.set GDT_SIZE, (Lgdt_end - Lgdt_start - 1) # Size of table minus 1.

# Define GDT pointer.
.section .data
.global gdt_pointer
gdt_pointer:
    .word GDT_SIZE
    .quad Lgdt_start # Address of GDT

# Define GDT.
.section .data
Lgdt_start:
Lgdt_null: # Null descriptor.
    .long 0
    .long 0
Lgdt_kernel_code:
    .word 0xFFFF # Lowest 16 bits of limit = 0xFFFFF
    .word 0 # Offset = 0
    .byte 0 # Offset = 0
    .byte 0b10011010 # Access byte
    .byte 0b10101111 # Flags and upper 4 bits of limit
    .byte 0 # Offset = 0
Lgdt_kernel_data:
    .word 0xFFFF # Lowest 16 bits of limit = 0xFFFFF
    .word 0 # Offset = 0
    .byte 0 # Offset = 0
    .byte 0b10010010 # Access byte
    .byte 0b10101111 # Flags and upper 4 bits of limit
    .byte 0 # Offset = 0
Lgdt_user_code:
    .word 0xFFFF # Lowest 16 bits of limit = 0xFFFFF
    .word 0 # Offset = 0
    .byte 0 # Offset = 0
    .byte 0b11111010 # Access byte
    .byte 0b10101111 # Flags and upper 4 bits of limit
    .byte 0 # Offset = 0
Lgdt_user_data:
    .word 0xFFFF # Lowest 16 bits of limit = 0xFFFFF
    .word 0 # Offset = 0
    .byte 0 # Offset = 0
    .byte 0b11110010 # Access byte
    .byte 0b10101111 # Flags and upper 4 bits of limit
    .byte 0 # Offset = 0
Lgdt_end:
