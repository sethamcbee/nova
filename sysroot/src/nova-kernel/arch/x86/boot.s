# Authors: Seth McBee
# Created: 2017-10-11
# Description: x86 initial boot file: first code ran by the bootloader.

# Declare constants for the Multiboot2 header.
.set MB_MAGIC, 0xE85250D6 # Must be present for bootloader to find header
.set MB_ARCH, 0 # constant for x86 family
.set MB_LENGTH, (multiboot_end - multiboot_start) # equals length of Mulitboot2 header (bytes)
.set MB_CHECKSUM, -(MB_MAGIC + MB_ARCH + MB_LENGTH)

# Paging information.
.set KERNEL_OFFSET, 0xC0000000
.set KERNEL_PD,   (KERNEL_OFFSET >> 22) & 1023
.set KERNEL_PT,   (KERNEL_OFFSET >> 12) & 1023
.set CR0_PG_MASK, (1 << 31)

# Define Multiboot2 header.
.section .init
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
mb_info:
    .skip 4
mb_magic:
    .skip 4

# Define entry function.
.global start
start:
# TODO: Detect whether long-mode is supported and hang if not.

# Points stack to physical address of kernel stack.
    movl $(kernel_stack_top - KERNEL_OFFSET), %esp

# Note: At this point, %eax should contain MB_MAGIC_TEST and %ebx should
# contain the address of the Multiboot memory map. These will need to
# be passed to boot_main.
    movl %eax, mb_magic
    movl %ebx, mb_info

# Set up GDT.
gdt_load:
    call gdt_init
    movw $0x10, %ax # Load data entry from GDT
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    pushl $0x08     # Load code entry from GDT
    pushl $1f
    retf # We far jump to load the code selector.

# Set up higher-half paging.
1:
    # Clear page directory.
    movl $(pd0 - KERNEL_OFFSET), %edi
    xorl %eax, %eax
    movl $1024, %ecx
    rep stosl
    
    # Clear first page table.
    movl $0x1000, %edi
    movl $1024, %ecx
    rep stosl
    
    # Clear kernel page table.
    movl $(pt0 - KERNEL_OFFSET), %edi
    movl $1024, %ecx
    rep stosl
    
    # Identity map first 4 MiB.
    movl $(pd0 - KERNEL_OFFSET), %edi   # Move to PD.
    movl %edi, %cr3                     # Set cr3 register to PD.
    movl $0x1003, (%edi)                # Point first PDe to first PT.
    movl $0x1000, %edi                  # Move to first PT.
    
    movl $0x3, %ebx                     # Set pages to PR | RW
    movl $1024, %ecx                    # Fill all 1024 entries.
fill_pt_low:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $4, %edi
        loop fill_pt_low
    
    # Fill kernel page directory entry.
    movl $(pd0 - KERNEL_OFFSET), %edi   # Move to PD.
    addl $(4*KERNEL_PD), %edi           # Move to PD entry.
    movl $(pt0 - KERNEL_OFFSET), %eax
    orl $0x3, %eax
    movl %eax, (%edi)                   # Point PD entry to PT0.
    
    # Fill kernel page table.
    movl $(pt0 - KERNEL_OFFSET), %edi
    addl $(4*KERNEL_PT), %edi
    #movl $(BOOT_OFFSET + 0x3), %ebx
    movl $0x3, %ebx
    movl $1024, %ecx
fill_pt0:
    movl %ebx, (%edi)
    addl $0x1000, %ebx
    addl $4, %edi
        loop fill_pt0
    
    # Enable paging.
    movl %cr0, %eax
    orl $CR0_PG_MASK, %eax
    movl %eax, %cr0
        
# Point stack to virtual address.
    movl $kernel_stack_top, %esp

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
1:  hlt
    jmp 1b

# Define main kernel stack.
.section .bss
.align 16
.global kernel_stack_bottom
kernel_stack_bottom:
    .skip (1024 * 16) # 16 KiB
.global kernel_stack_top
kernel_stack_top:
