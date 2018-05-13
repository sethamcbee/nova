// Authors: Seth McBee
// Created: 2018-5-10
// Description: Virtual memory manager.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Virtual address space constants.
static const size_t RECURSIVE_INDEX = 510;
static const size_t BOOT_OFFSET = 0x100000;
static const size_t KERNEL_OFFSET = 0xFFFFFFFF80000000;
static const size_t MEMORY_MAX = 0xFFFFFFFFFFFFFFFF;
extern void* phys_start;
extern void* phys_end;

// Macros for physical address calculation.
#define PML4_INDEX(addr)    ((((size_t)(addr)) >> 39) & 511)
#define PDPT_INDEX(addr)    ((((size_t)(addr)) >> 30) & 511)
#define PD_INDEX(addr)    ((((size_t)(addr)) >> 21) & 511)
#define PT_INDEX(addr)    ((((size_t)(addr)) >> 12) & 511)
#define PT_OFFSET(addr)    ((size_t)(addr) & 0xFFF)

// Calculations for accessing paging tables at their higher-half
// mappings.
#define VMM_BASE     0xFFFF000000000000
#define PT_BASE      (VMM_BASE + (RECURSIVE_INDEX << 39))
#define PD_BASE      (PT_BASE + (RECURSIVE_INDEX << 30))
#define PDPT_BASE    (PD_BASE + (RECURSIVE_INDEX << 21))
#define PML4_BASE    (PDPT_BASE + (RECURSIVE_INDEX << 12))

// Virtual structure for address.
#define PT_OF(addr)   ((PT_BASE + ((((size_t)addr)>>9) & 0x7FFFFF000)))
#define PD_OF(addr)   (PD_BASE + ((((size_t)addr)>>18) & 0x003FFFF000))
#define PDPT_OF(addr) (PDPT_BASE + ((((size_t)addr)>>27) & 0x00001FF000))
#define PML4_OF(addr) (PML4_BASE)

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
