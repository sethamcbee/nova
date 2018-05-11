// Authors: Seth McBee
// Created: 2018-5-10
// Description: Virtual memory manager.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Convert virtual address to physical address.
void* vmm_phys_addr(void* virt);

// Initializes virtual memory manager.
void vmm_init(void);

// Map a physical page to a virtual address.
void vmm_page_map(void* phys, void* virt, uint16_t flags);
