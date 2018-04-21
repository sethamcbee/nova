# Authors: Seth McBee
# Created: 2017-10-17
# Description: GDT setup.

#include <arch/x86/gdt.h>

.set GDT_SIZE, (Lgdt_end - Lgdt_start - 1) # Size of table minus 1.

# Define GDT pointer.
.data
.global gdt_pointer
.align 16
gdt_pointer:
    .word GDT_SIZE
    .long Lgdt_start # Address of GDT

# Define GDT.
.align 16
Lgdt_start:
Lgdt_null:                  # Null descriptor.
    .long 0
    .long 0
Lgdt_kernel_code:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10011010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
Lgdt_kernel_data:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10010010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
Lgdt_user_code:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b11111010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
Lgdt_user_data:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b11110010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
Lgdt_end:
