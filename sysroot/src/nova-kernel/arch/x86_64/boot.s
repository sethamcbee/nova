# Authors: Seth McBee
# Created: 2017-10-11
# Description: x86-64 initial boot file: first code ran by the bootloader.

# Declare constants for the Multiboot2 header.
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
	.long Lend_tag_end - Lend_tag_start # Size
Lend_tag_end:
multiboot_end:

# Storage for Multiboot2 MAGIC and info struct pointer.
.section .bss
mb_info:
	.skip 4
mb_magic:
	.skip 4

# Define temporary paging structure. This will only be a stub, so
# real paging can be handled in C by kernel_main.
.section .tmp_paging
.align (0x1000) # align to page boundary
Lpaging_struct_start:
Lpml4:
	.skip 0x1000
Lpdp:
	.skip 0x1000
Lpd:
	.skip 0x1000
Lpt:
	.skip 0x1000
Lpaging_struct_end:

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
.code32
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

# Setup temporary page table. Some code from OSDev.org wiki.
    movl $0x1000, %edi # set first location to iterate from
    movl %edi, %cr3
    xorl %eax, %eax
    movl $0x1000, %ecx # set number of iterations to 4KiB
    rep stosl # clear all (sets each long to value of %eax)
    movl %cr3, %edi # set %edi to beginning again
    movl $0x2003, (%edi)
    addl $0x1000, %edi
    movl $0x3003, (%edi)
    addl $0x1000, %edi
    movl $0x4003, (%edi)
    addl $0x1000, %edi

    movl $00000003, %ebx # set pages to PRESENT, and RW
    movl $512, %ecx # loop 512 times
.Lset_entry:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $8, %edi # iterate eight bytes
    loop .Lset_entry

# Set CR4.PAE.
    movl %cr4, %eax
    orl $CR4_PAE_MASK, %eax
    movl %eax, %cr4

# Set long mode bit by setting the EFER.LME flag in MSR 0xC0000080.
    movl $LONG_MODE_MSR, %ecx
    rdmsr
    orl $LONG_MODE_MASK, %eax
    wrmsr

# Enable paging and thereby enter compatibility mode.
    movl %cr0, %eax
    orl $CR0_PG_MASK, %eax
    movl %eax, %cr0

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
    pushl $enter_64
    retf # We far jump to load the selector.
enter_64:
.code64
# Moves the address of the Multiboot info structure to %rdi
# so it can be the first argument to boot_main.
    xorq %rdi, %rdi
    movl mb_info, %edi

# Moves the value of MB_MAGIC_TEST to %rsi, so it can be the
# second argument to boot_main.
    xorq %rsi, %rsi
    movl mb_magic, %esi

# Calls C boot procedure to do additional setup before the kernel is
# called.
    call boot_main

# Loops infinitely in case an error causes boot_main to return.
	cli
1:	hlt
	jmp 1b
