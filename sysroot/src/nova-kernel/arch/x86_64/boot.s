# Authors: Seth McBee
# Created: 2017-10-11
# Description: x86-64 initial boot file: first code ran by the bootloader.

.code32

.section .init

# Declare constants for the Multiboot2 header.
.set MB_MAGIC, 0xE85250D6 # Must be present for bootloader to find header
.set MB_ARCH, 0 # constant for x86 family
.set MB_LENGTH, (multiboot_end - multiboot_start) # equals length of Mulitboot2 header (bytes)
.set MB_CHECKSUM, -(MB_MAGIC + MB_ARCH + MB_LENGTH)

# Declare constants for transition to long mode.
.set CR0_PG_MASK, (1 << 31)
.set CR4_PSE_MASK, (1 << 1)
.set CR4_PAE_MASK, (1 << 5)
.set LONG_MODE_MSR, 0xC0000080
.set LONG_MODE_MASK, (1 << 8)

# Paging information.
.set KERNEL_OFFSET, 0xffffffff80000000
.set KERNEL_PML4,   (KERNEL_OFFSET >> 39) & 511
.set KERNEL_PDPT,   (KERNEL_OFFSET >> 30) & 511
.set KERNEL_PD,   (KERNEL_OFFSET >> 21) & 511
.set KERNEL_PT,   (KERNEL_OFFSET >> 12) & 511

# Define Multiboot2 header.
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

# Define initial stack.
.align 16
.global boot_stack_bottom
boot_stack_bottom:
    .skip (0x1000) # 4 KiB
.global boot_stack_top
boot_stack_top:

# Define initial GDT.
.align 16
gdt_boot_start:
# Null descriptor.
    .long 0
    .long 0
# Kernel code.
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10011010        # Access byte
    .byte 0b10101111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
# Kernel data.
    .word 0xFFFF            # Lowest 16 bits of limit = 0xFFFFF
    .word 0                 # Offset = 0
    .byte 0                 # Offset = 0
    .byte 0b10010010        # Access byte
    .byte 0b10101111        # Flags and upper 4 bits of limit
    .byte 0                 # Offset = 0
gdt_boot_end:

.set GDT_SIZE, (gdt_boot_end - gdt_boot_start - 1) # Size of table minus 1.

# Define GDT pointer.
.align 16
gdt_boot:
    .word GDT_SIZE
    .quad gdt_boot_start         # Address of GDT

# Storage for Multiboot parameters.
mb_info:
    .skip 4
mb_magic:
    .skip 4

# Define entry function.
.global start
start:

# Note: At this point, %eax should contain MB_MAGIC_TEST and %ebx should
# contain the address of the Multiboot memory map. These will need to
# be passed to boot_main.
    movl %eax, mb_magic
    movl %ebx, mb_info

# Set up boot stack.
    movl $boot_stack_top, %esp

# Identity map first two MiB.
    movl $0x1000, %edi   # Set location of PML4.
    movl %edi, %cr3	     # Set PML4 in %CR3 register.
    xorl %eax, %eax      # Zero %eax.
    movl $0x1000, %ecx   # Clear 16 KiB.
    rep stosl            # Clear all tables.
    movl $0x1000, %edi   # Move to PML4.
    movl $0x2003, (%edi) # Point PML4 to PDP.
    movl $0x2000, %edi   # Move to PDP.
    movl $0x3003, (%edi) # Point PDP to PD.
    movl $0x3000, %edi   # Move to PD.
    movl $0x4003, (%edi) # Point PD0 to PT0.
    movl $0x4000, %edi   # Move to PT0.

    movl $00000003, %ebx # Set pages to present and writeable.
    movl $512, %ecx      # Loop 512 times (number of entries in table).
fill_pt1:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $8, %edi # Iterate to next table entry.
        loop fill_pt1

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
gdt_load:
    movl $gdt_boot, %eax
    lgdt (%eax)
    movw $0x10, %ax		# Load data entry from GDT
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    pushl $0x08			# Load code entry from GDT
    pushl $enter64
    retf			# We far jump to load the selector.

.code64
enter64:
    # Set up higher-half paging.
    movl $0x5000, %edi            # Move to next page structure.
    xorl %eax, %eax
    movl $0x1000, %ecx
    rep stosl
    movl $0x1000, %edi            # Move to PML4.
    addl $(8*KERNEL_PML4), %edi   # Move to PML4 entry.
    movl $0x5003, (%edi)          # Point PML4 entry to PDPT.
    movl $0x5000, %edi            # Move to PDPT.
    addl $(8*KERNEL_PDPT), %edi   # Move to PDPT entry.
    movl $0x6003, (%edi)          # Point PDPT entry to PD.
    movl $0x6000, %edi            # Move to PD.
    addl $(8*KERNEL_PD), %edi     # Move to PD entry.
    movl $0x7003, (%edi)          # Point PD entry to PT0.
    addl $8, %edi
    movl $0x8003, (%edi)          # Point PD entry to PT1.
    movl $0x7000, %edi            # Move to PT0.

    movl $00000003, %ebx # Set pages to present and writeable.
fill_high_pt1:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $8, %edi
    movl $512, %ecx
fill_high_pt2:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $8, %edi
        loop fill_high_pt2

    # Reload %CR3.
    movq %cr4, %rax
    movq %rax, %cr4

    # Get paramaters.
    xorq %rdi, %rdi
    xorq %rsi, %rsi
    movl mb_info, %edi
    movl mb_magic, %esi

    # Set up new stack.
    movq $kernel_stack_top, %rsp

    call boot_main

    cli
1:      hlt
            jmp 1b

.bss
    .skip 0x2000 # 8 KiB.
.global kernel_stack_top
kernel_stack_top:
