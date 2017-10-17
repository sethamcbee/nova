// Authors: Seth McBee
// Created: 2017-10-14
// Description: x86_64 initial C boot environment.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <x86_64/multiboot2.h>

void boot_main(multiboot_tag* multiboot_tag, unsigned int magic)
{
	// TODO: Setup permanent GDT, IDT, paging, etc.
	
    // TODO: Safely copy all needed Multiboot2 structures.

    // TODO: Initialize memory manager.

    // TODO: Initialize terminal.
    
    // Call kernel.
    kernel_main();
}
