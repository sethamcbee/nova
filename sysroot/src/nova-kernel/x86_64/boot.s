// Authors: Seth McBee, with code from OSDev Wiki
// Created: 2017-10-11
// Description: Initial boot file; first code ran by the bootloader.

// Declare constants for the Multiboot header.
.set ALIGN,    1<<0             // align loaded modules on page boundaries
.set MEMINFO,  1<<1             // provide memory map
.set FLAGS,    ALIGN | MEMINFO  // this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       // 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) // checksum of above, to prove we are multiboot

// Declare Multiboot header.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// Declare main kernel stack.
.section .bss
.align 16
stack_bottom:
.skip 16384 // 16 KiB
stack_top:

// Declare entry function.
.section .text
.global _start
.type _start, @function
_start:
	// Points stack to main kernel stack.
	mov $stack_top, %esp
	
	// TODO: Change to 64-bit before calling boot_main, as all C code will
	// be compiled as 64-bit and we will trigger an exception if we try to
	// call any C code first.
	
	// Pushes %ebx to pass address of the Multiboot memory map as a
	// parameter to boot_main
    push %ebx

	// Calls C boot procedure to do additional setup before the kernel is
	// called.
	call boot_main

	// Loops infinitely in case an error causes boot_main to return.
	cli
1:	hlt
	jmp 1b

// Set the size of the _start symbol to the current location '.' minus its start.
// This is useful when debugging or when you implement call tracing.
.size _start, . - _start
