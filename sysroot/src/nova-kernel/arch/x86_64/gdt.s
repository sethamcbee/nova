# Authors: Seth McBee
# Created: 2017-10-17
# Description: x86-64 GDT setup.

.global gdt_ptr
.global gdt_init
.global gdt_start
.global gdt_entry

.set GDT_SIZE, (gdt_end - gdt_start - 1) # Size of table minus 1.

# Define GDT pointer.
.data
.align 16
gdt_ptr:
    .word GDT_SIZE
    .quad gdt_start         # Address of GDT

# Initialize GDT.
.text
gdt_init:
    movq $gdt_ptr, %rax
    lgdt (%rax)
    ret

# Define GDT.
.data
.align 16
gdt_entry:
gdt_start:
gdt_null:                   # Null descriptor.
    .long 0
    .long 0
gdt_kernel_code:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10011010        # Access byte
    .byte 0b10101111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_kernel_data:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10010010        # Access byte
    .byte 0b10101111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_user_code:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b11111010        # Access byte
    .byte 0b10101111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_user_data:
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b11110010        # Access byte
    .byte 0b10101111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_tss:                    # TSS will be set up during runtime.
    .long 0
    .long 0
    .long 0
    .long 0
gdt_end:
