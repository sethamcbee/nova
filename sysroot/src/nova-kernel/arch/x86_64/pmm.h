// Authors: Seth McBee
// Created: 2018-5-7
// Description: Physical memory manager.

#ifndef PMM_H
#define PMM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// STUB. We'll just use the largest chunk of available higher
// memory. Later, we should translate the memory map from the bootloader
// into a proper format.
size_t pmm_mem_start;
size_t pmm_mem_max;
size_t pmm_mem_free;

void pmm_init(void);

// Allocates a given number of consecutive bytes.
void* pmm_malloc(size_t size);

#endif // PMM_H
