# Authors: Seth McBee
# Created: 2017-10-11
# Description: x86 initial boot file: first code ran by the bootloader.

# Declare constants for the Multiboot2 header.
.set MB_MAGIC, 0xE85250D6 # Must be present for bootloader to find header
.set MB_ARCH, 0 # constant for x86 family
.set MB_LENGTH, (multiboot_end - multiboot_start) # equals length of Mulitboot2 header (bytes)
.set MB_CHECKSUM, -(MB_MAGIC + MB_ARCH + MB_LENGTH)

# Define Multiboot2 header.
.section .multiboot
.align 16
multiboot_start:
	.long MB_MAGIC
	.long MB_ARCH
	.long MB_LENGTH
	.long MB_CHECKSUM
end_tag_start:
	.word 0 # Type
	.word 0 # Flags
	.long end_tag_end - end_tag_start # Size
end_tag_end:
multiboot_end:

# Storage for Multiboot2 MAGIC and info struct pointer.
.section .bss
mb_info:
	.skip 4
mb_magic:
	.skip 4

# Define main kernel stack.
.section .bss
.align 16
.global kernel_stack_bottom
kernel_stack_bottom:
.skip (1024 * 16) # 16 KiB
.global kernel_stack_top
kernel_stack_top:

# Define entry function.
.section .text
.global _start
_start:
# TODO: Detect whether long-mode is supported and hang if not.

# Points stack to main kernel stack.
    movl $kernel_stack_top, %esp

# Note: At this point, %eax should contain MB_MAGIC_TEST and %ebx should
# contain the address of the Multiboot memory map. These will need to
# be passed to boot_main.
	movl %eax, mb_magic
	movl %ebx, mb_info

# Setup GDT and jump to long mode.
gdt_load:
    movl $gdt_pointer, %eax # Load pointer to GDT to %eax
    lgdt (%eax) # Tell CPU to load descriptor
    movw $0x10, %ax # Load data entry from GDT
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    pushl $0x08 # Load code entry from GDT
    pushl $1f
    retf # We far jump to load the selector.
1:

# Pushes the Magic value onto the stack.
    movl mb_magic, %eax
    push %eax

# Pushes the address of the Multiboot stucture onto the stack.
    movl mb_info, %eax
    push %eax

# Calls C boot procedure to do additional setup before the kernel is
# called.
    call boot_main

# Loops infinitely in case an error causes boot_main to return.
	cli
1:	hlt
	jmp 1b
