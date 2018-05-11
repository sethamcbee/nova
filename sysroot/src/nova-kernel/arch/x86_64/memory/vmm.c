// Authors: Seth McBee
// Created: 2018-5-10
// Description: Virtual memory manager.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/multiboot2.h>

#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/pmm.h>
#include <arch/x86_64/memory/vmm.h>

void* vmm_phys_addr(void* virt)
{
    size_t phys;
    size_t virt_addr = (size_t)virt;
    Pdpte *pdpt;
    Pde *pd;
    Pte *pt;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);
    uint16_t offset = PT_OFFSET(virt_addr);

    // Check for valid PML4 entry.
    if (pml4[pml4_i].present == false)
    {
        return (NULL);
    }

    // Find PDPT.
    phys = ((size_t)pml4[pml4_i].dir_ptr_addr_high) << 32;
    phys |= ((size_t)pml4[pml4_i].dir_ptr_addr_low) << 12;
    pdpt = (Pdpte*) phys;
    if (pdpt[pdpt_i].present == false)
    {
        return (NULL);
    }

    // Find PD.
    phys = ((size_t)pdpt[pdpt_i].dir_addr_high) << 32;
    phys |= ((size_t)pdpt[pdpt_i].dir_addr_low) << 12;
    pd = (Pde*) phys;
    if (pd[pd_i].present == false)
    {
        return (NULL);
    }

    // Find PT.
    phys = ((size_t)pd[pd_i].table_addr_high) << 32;
    phys |= ((size_t)pd[pd_i].table_addr_low) << 12;
    pt = (Pte*) phys;
    if (pt[pt_i].present == false)
    {
        return (NULL);
    }

    // Find physical address of this page, and add offset.
    phys = ((size_t)pt[pt_i].page_addr_high) << 32;
    phys |= ((size_t)pt[pt_i].page_addr_low) << 12;
    phys |= offset;

    return ((void*) phys);
}

void vmm_page_map(void* phys, void* virt, uint16_t flags)
{

}
