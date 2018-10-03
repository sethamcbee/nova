/**
 * @file vmm.h
 * @author Seth McBee
 * @date 2018-10-3
 * @brief Virtual memory management.
 */

#pragma once

#include <globals.h>

#include <arch/x86/memory/paging.h>

// Virtual address space constants.
static const size_t BOOT_OFFSET = 0x100000UL;
static const size_t KERNEL_OFFSET = 0xFFFFFFFF80000000UL; ///< @warning
static const size_t MEMORY_MAX = 0xFFFFFFFFUL;
static const size_t RECURSIVE_INDEX = 1022;
extern void *phys_start;
extern void *phys_end;
extern void *kernel_ro_start;
extern void *kernel_ro_end;

typedef struct Vmm_Region Vmm_Region;
struct Vmm_Region
{
	void* base;
	size_t pages;
};

// Stores a region of virtual memory.
// Implemented as an AVL tree.
typedef struct Vmm_Node Vmm_Node;
struct Vmm_Node
{
	Vmm_Region mem;
	int height;
	Vmm_Node* l;
	Vmm_Node* r;
};

// Virtual memory trees.
Vmm_Node* vmm_tree_kernel_free;
Vmm_Node* vmm_tree_kernel_used;
Vmm_Node* vmm_tree_user_free;
Vmm_Node* vmm_tree_user_used;

// Convert virtual address to physical address.
void* vmm_phys_addr(void* virt);

// Initializes virtual memory manager.
void vmm_init(void);

// Map a physical page to a virtual address.
void vmm_page_map(void* phys, void* virt, uint16_t flags);

// Unmap a page.
void vmm_page_unmap(void* virt);

// Set flags for entry.
void vmm_table_flags(void* virt, uint16_t flags);

// Allocate and map a page for kernel use. Return virtual address,
// or NULL.
void* vmm_page_alloc_kernel(void);

// Allocate and map consecutive pages for kernel use. Return virtual
// address of first page, or NULL.
void* vmm_pages_alloc_kernel(size_t n);

// Frees a page that was used by the kernel. This unmaps the page, as
// well as marking it as free in the PMM.
void vmm_page_free_kernel(void* virt);

// Frees consecutive kernel pages.
void vmm_pages_free_kernel(void* virt, size_t n);

// Inserts a node and returns a pointer to the new root node.
Vmm_Node* vmm_tree_insert(Vmm_Node* root, Vmm_Region mem);

// Deletes a node and returns a pointer to the new root node.
Vmm_Node* vmm_tree_delete(Vmm_Node* root, Vmm_Region mem);

// Searches a tree for a sufficient amount of pages.
// return.pages == 0 when no sufficient region was found.
Vmm_Region vmm_tree_find_pages(Vmm_Node* root, size_t pages);

// Resizes a node by changing its page count (NOT its base).
Vmm_Node* vmm_tree_resize(Vmm_Node* root, Vmm_Region mem);

