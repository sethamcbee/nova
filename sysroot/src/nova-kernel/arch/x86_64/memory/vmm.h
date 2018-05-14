// Authors: Seth McBee
// Created: 2018-5-10
// Description: Virtual memory manager.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Virtual address space constants.
static const size_t BOOT_OFFSET = 0x100000UL;
static const size_t KERNEL_OFFSET = 0xFFFFFFFF80000000UL;
static const size_t MEMORY_MAX = 0xFFFFFFFFFFFFFFFFUL;
static const size_t RECURSIVE_INDEX = 510;
extern void* phys_start;
extern void* phys_end;

// Macros for physical address calculation.
#define PML4_INDEX(addr)    ((((size_t)(addr)) >> 39) & 511UL)
#define PDPT_INDEX(addr)    ((((size_t)(addr)) >> 30) & 511UL)
#define PD_INDEX(addr)    ((((size_t)(addr)) >> 21) & 511UL)
#define PT_INDEX(addr)    ((((size_t)(addr)) >> 12) & 511UL)
#define PT_OFFSET(addr)    ((size_t)(addr) & 0xFFFUL)

// Convert virtual address to physical address.
void* vmm_phys_addr(void *virt);

// Initializes virtual memory manager.
void vmm_init(void);

// Map a physical page to a virtual address.
void vmm_page_map(void *phys, void *virt, uint16_t flags);

// Unmap a page.
void vmm_page_unmap(void *virt);

// Set flags for entry.
void vmm_table_flags(void *virt, uint16_t flags);

// Allocate and map a page for kernel use. Return virtual address,
// or NULL.
void* vmm_page_alloc_kernel(void);
