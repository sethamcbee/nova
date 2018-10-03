# Authors: Seth McBee
# Created: 2017-10-17
# Description: GDT setup.

.global gdt_ptr
.global gdt_init
.global gdt_start

.set GDT_SIZE, (gdt_end - gdt_start - 1) # Size of table minus 1.

# Define GDT pointer.
.section .init
.align 16
gdt_ptr:
    .word GDT_SIZE
    .long gdt_start # Address of GDT

# Initialize GDT.
gdt_init:
	movl $gdt_ptr, %eax
	lgdt (%eax)
	ret

# Define GDT.
.align 16
gdt_start:
gdt_null:                  # Null descriptor.
    .long 0
    .long 0
gdt_kernel_code:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10011010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_kernel_data:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10010010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_user_code:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b11111010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_user_data:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b11110010        # Access byte
    .byte 0b11001111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_tss:
	.long 0
	.long 0
	.long 0
	.long 0
gdt_end:
