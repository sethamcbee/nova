# Authors: Seth McBee, with code from OSDev Wiki
# Created: 2017-10-11
# Description: Initial boot file: first code ran by the bootloader.

# Declare constants for the Multiboot2 header.
.set MB_MAGIC_TEST, 0x36d76289 # %eax should equal this upon entry of _start
.set MB_MAGIC, 0xE85250D6 # Must be present for bootloader to find header
.set MB_ARCH, 0 # constant for x86 family
.set MB_LENGTH, 24 # equals length of Mulitboot2 header (bytes)
.set MB_CHECKSUM, -(MB_MAGIC + MB_ARCH + MB_LENGTH)

# Declare constants for transition to long mode.
.set CR0_PG_MASK, (1 << 31)
.set CR4_PAE_MASK, (1 << 5)

# Define Multiboot2 header.
.section .multiboot
.align 8
.long MB_MAGIC
.long MB_ARCH
.long MB_LENGTH
.long MB_CHECKSUM
# TODO: set tags

# Define temporary GDT.
.section .data
gdt_start:
gdt_null: # Null descriptor.
    .long 0
    .long 0
gdt_code:
    .word 0xFFFF # Lowest 16 bits of limit = 0xFFFFF
    .word 0 # Offset = 0
    .byte 0 # Offset = 0
    .byte 10011010b # Access byte
    .byte 11001111b # Flags and upper 4 bits of limit
    .byte 0 # Offset = 0
gdt_data:
    .word 0xFFFF # Lowest 16 bits of limit = 0xFFFFF
    .word 0 # Offset = 0
    .byte 0 # Offset = 0
    .byte 10011010b # Access byte
    .byte 11001111b # Flags and upper 4 bits of limit
    .byte 0 # Offset = 0
gdt_end:

# Define temporary GDT descriptor.
.section .data
gdt_descriptor:
    .word (gdt_end - gdt_start - 1) # Size of table minus 1
    .long $gdt_start # Address of GDT

# Define temporary IDT.

# Definte temporary paging structure.

# Define main kernel stack.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16kb
stack_top:

# Define entry function.
.section .text
.code32
.global _start
_start:
	# Note: At this point, %eax should contain MB_MAGIC_TEST and %ebx should
	# contain the address of the Multiboot memory map. These will need to
	# be passed to boot_main.
	
    # Points stack to main kernel stack.
    movl $stack_top, %esp
    
    # TODO: Detect whether long-mode is supported and abort if not.

	############
    # TODO: Change to 64-bit before calling boot_main, as all C code will
    # be compiled as 64-bit and we will trigger an exception if we try to
    # call any C code first.
    ############
    
    # Setup temporary GDT.
    movl 4(%esp), %ecx
    lgdt (%ecx)
    ret
    
    
    # Setup temporary IDT.
    
    # Setup temporary page table.
    
    # Paging needs to be disabled at this step, but Multiboot defines
    # CR0.PG to be cleared at this point already so it's not necessary.
    
    # Set CR4.PAE.
    movl %cr4, %ecx
    orl $CR4_PAE_MASK, %ecx
    movl %ecx, %cr4
    
    # Load CR3 with physical address of the PML4.
    
    # Enable long mode by setting the EFER.LME flag in MSR 0xC0000080.
    
    # Enable paging.
    movl %cr0, %ecx
    orl $CR0_PG_MASK, %ecx
    
    ###########

# 64-bit mode from here on out.
.code64

    # Pushes %eax to pass MB_MAGIC_TEST to boot_main.
    pushl %eax

    # Pushes %ebx to pass address of the Multiboot memory map as a
    # parameter to boot_main
    pushl %ebx

    # Calls C boot procedure to do additional setup before the kernel is
    # called.
    call boot_main

	# Loops infinitely in case an error causes boot_main to return.
	jmp hang

# Set the size of the _start symbol to the current location '.' minus its start.
# This is useful when debugging or when you implement call tracing.
.size _start, . - _start

# Hangs infinitely.
hang:
    cli
1:  hlt
    jmp 1b
