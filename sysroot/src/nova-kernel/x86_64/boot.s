# Authors: Seth McBee, with code from OSDev Wiki
# Created: 2017-10-11
# Description: Initial boot file: first code ran by the bootloader.

# Declare constants for the Multiboot header.
.set ALIGN, (1 << 0)             # align loaded modules on page boundaries
.set MEMINFO, (1 << 1)             # provide memory map
.set FLAGS, (ALIGN | MEMINFO)  # this is the Multiboot 'flag' field
.set MAGIC, 0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above

# Declare constants for transition to long mode.
.set CR0_PG_MASK, (1 << 31)
.set CR4_PAE_MASK, (1 << 5)

# Define Multiboot header.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Define main kernel stack.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16kb
stack_top:

# Define entry function.
.section .text
.code32
_start:
	# Note: At this point, %ebx contains the address of the Multiboot
	# memory map. This will need to be passed to boot_main.
	
    # Points stack to main kernel stack.
    movl $stack_top, %esp
    
    # TODO: Detect whether long-mode is supported and abort if not.

	############
    # TODO: Change to 64-bit before calling boot_main, as all C code will
    # be compiled as 64-bit and we will trigger an exception if we try to
    # call any C code first.
    ############
    
    # Initialize GDT, IDT, and page tables.
    
    # Disable paging (clear CR0.PG).
    movl %cr0, %eax
    andl -CR0_PG_MASK, %eax
    movl %eax, %cr0
    
    # Set CR4.PAE.
    movl %cr4, %eax
    orl CR4_PAE_MASK, %eax
    movl %eax, %cr4
    
    # Load CR3 with physical address of the PML4.
    
    # Enable long mode by setting the EFER.LME flag in MSR 0xC0000080.
    
    # Enable paging.
    
    ###########

# 64-bit mode from here on out.
.code64

    # Pushes %ebx to pass address of the Multiboot memory map as a
    # parameter to boot_main
    pushq %rbx

    # Calls C boot procedure to do additional setup before the kernel is
    # called.
    call boot_main

	# Loops infinitely in case an error causes boot_main to return.
	cli
1:	hlt
	jmp 1b

// Set the size of the _start symbol to the current location '.' minus its start.
// This is useful when debugging or when you implement call tracing.
.size _start, . - _start
