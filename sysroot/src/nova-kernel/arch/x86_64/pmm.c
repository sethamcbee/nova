// Authors: Seth McBee
// Created: 2018-5-7
// Description: Physical memory manager.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel.h>
#include <arch/x86_64/pmm.h>

// Amount of free memory.
size_t pmm_mem_free;

void pmm_init(void)
{
    // STUB. Actual initialization is done by the Multiboot2 mmap
    // parser.
}

void* pmm_malloc(size_t size)
{
    void* ptr;

    // Check that there is enough memory left for this
    // allocation.
    if (pmm_mem_free < size)
        return NULL;

    ptr = (void*) (pmm_mem_start + pmm_mem_max - pmm_mem_free);
    pmm_mem_free -= size;
    return (ptr);
}
