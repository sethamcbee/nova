# Authors: Seth McBee
# Created: 2017-10-11
# Description: Initial boot file: first code ran by the bootloader.

# Declare constants for the Multiboot2 header.
.set MB_MAGIC_TEST, 0x36d76289 # %eax should equal this upon entry of _start
.set MB_MAGIC, 0xE85250D6 # Must be present for bootloader to find header
.set MB_ARCH, 0 # constant for x86 family
.set MB_LENGTH, (multiboot_end - multiboot_start) # equals length of Mulitboot2 header (bytes)
.set MB_CHECKSUM, -(MB_MAGIC + MB_ARCH + MB_LENGTH)

# Declare constants for transition to long mode.
.set CR0_PG_MASK, (1 << 31)
.set CR4_PAE_MASK, (1 << 5)
.set LONG_MODE_MSR, 0xC0000080
.set LONG_MODE_MASK, (1 << 8)

# Define Multiboot2 header.
.section .multiboot
.align 16
multiboot_start:
	.long MB_MAGIC
	.long MB_ARCH
	.long MB_LENGTH
	.long MB_CHECKSUM
Lend_tag_start:
	.word 0 # Type
	.word 0 # Flags
	.long 8 # Size - special code for last tag
Lend_tag_end:
multiboot_end:

# Define temporary paging structure.

# Define main kernel stack.
.section .bss
.align 16
kernel_stack_bottom:
.skip (1024 * 16) # 16 KiB
kernel_stack_top:

# Define entry function.
.section .text
.code32
.global _start
_start:
	# Points stack to main kernel stack.
    movl $kernel_stack_top, %esp

	# Note: At this point, %eax should contain MB_MAGIC_TEST and %ebx should
	# contain the address of the Multiboot memory map. These will need to
	# be passed to boot_main. Because %eax is used for writing to MSRs,
	# move %eax to %ebp to preserve its value.
	movl %eax, %ebp
    
    # TODO: Detect whether long-mode is supported and hang if not.
       
    # Setup temporary page table.
    
    # Set CR4.PAE.
    movl %cr4, %eax
    orl $CR4_PAE_MASK, %eax
    movl %eax, %cr4
    
    # Load CR3 with physical address of the PML4.
    
    # Set long mode bit by setting the EFER.LME flag in MSR 0xC0000080.
    movl $LONG_MODE_MSR, %ecx
    rdmsr
    orl $LONG_MODE_MASK, %eax
    wrmsr
    
    # Enable paging and thereby enter compatibility mode.
    movl %cr0, %eax
    orl $CR0_PG_MASK, %eax
    
    # Setup GDT and jump to long mode.
Lgdt_load:
    movl $gdt_pointer, %eax # Load pointer to GDT to %eax
    lgdt (%eax) # Tell CPU to load descriptor
    movw $0x10, %ax # Load data entry from GDT
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    pushl $0x08 # Load code entry from GDT
    pushl $Lgdt_load_cs
	retf # We far jump to load the selector.
Lgdt_load_cs:

	# Long mode from here on out.
.code64

    # Moves the address of the Mulitboot info structure to %rdi
    # so it can be the first argument to boot_main.
    movl %ebx, %ebx
    movq %rbx, %rdi
    
    # Moves the value of MB_MAGIC_TEST to %rsi, so it can be the
    # second argument to boot_main.
    movl %ebp, %ebp
    movq %rbp, %rsi

    # Calls C boot procedure to do additional setup before the kernel is
    # called.
    call boot_main

	# Loops infinitely in case an error causes boot_main to return.
	cli
1:	hlt
	jmp 1b
