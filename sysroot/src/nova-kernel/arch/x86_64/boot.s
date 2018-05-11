# Authors: Seth McBee
# Created: 2017-10-11
# Description: x86-64 initial boot file: first code ran by the bootloader.

.extern gdt_init
.extern gdt_ptr

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

# Kernel paging structure.
.section .bss
.align 0x1000
.global pml4
pml4:
    .skip 0x1000
.global pdpt0
pdpt0:
    .skip 0x1000
.global pd0
pd0:
    .skip 0x1000
.global pt0
pt0:
    .skip 0x1000

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

# Setup boot paging structure.
    movl $0x1000, %edi   # Set location of PML4.
    movl %edi, %cr3	 # Set PML4 in %CR3 register.
    xorl %eax, %eax      # Zero %eax.
    movl $0x1000, %ecx   # Clear 16 KiB.
    rep stosl            # Clear all tables.
    movl %cr3, %edi      # Set %edi to PML4.
    movl $0x2003, (%edi) # Point PML4 to PDP.
    addl $0x1000, %edi   # Set %edi to PDP.
    movl $0x3003, (%edi) # Point PDP to PD.
    addl $0x1000, %edi   # Set %edi to PD.
    movl $0x4003, (%edi) # Point PD0 to PT0.
    movl $0x5003, 8(%edi)# Point PD1 to PT1.
    addl $0x1000, %edi   # Set %edi to PT0.

    movl $00000003, %ebx # Set pages to present and writeable.
    movl $512, %ecx      # Loop 512 times (number of entries in table).
fill_pt1:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $8, %edi # Iterate to next table entry.
        loop fill_pt1
    movl $512, %ecx
fill_pt2:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $8, %edi
        loop fill_pt2

# Enable PAE.
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
    movl $gdt_ptr, %eax		# Load pointer to GDT to %eax
    lgdt (%eax)			# Tell CPU to load descriptor
    movw $0x10, %ax		# Load data entry from GDT
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    pushl $0x08			# Load code entry from GDT
    pushl $enter_64
    retf			# We far jump to load the selector.
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
