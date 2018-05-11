// Authors: Seth McBee
// Created: 2018-5-10
// Description: Virtual memory manager.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <globals.h>
#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/pmm.h>
#include <arch/x86_64/memory/vmm.h>

void* vmm_phys_addr(void* virt)
{
    size_t phys_addr;
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
    phys_addr = ((size_t)pml4[pml4_i].dir_ptr_addr_high) << 32;
    phys_addr |= ((size_t)pml4[pml4_i].dir_ptr_addr_low) << 12;
    pdpt = (Pdpte*) phys_addr;
    if (pdpt[pdpt_i].present == false)
    {
        return (NULL);
    }

    // Find PD.
    phys_addr = ((size_t)pdpt[pdpt_i].dir_addr_high) << 32;
    phys_addr |= ((size_t)pdpt[pdpt_i].dir_addr_low) << 12;
    pd = (Pde*) phys_addr;
    if (pd[pd_i].present == false)
    {
        return (NULL);
    }

    // Find PT.
    phys_addr = ((size_t)pd[pd_i].table_addr_high) << 32;
    phys_addr |= ((size_t)pd[pd_i].table_addr_low) << 12;
    pt = (Pte*) phys_addr;
    if (pt[pt_i].present == false)
    {
        return (NULL);
    }

    // Find physical address of this page, and add offset.
    phys_addr = ((size_t)pt[pt_i].page_addr_high) << 32;
    phys_addr |= ((size_t)pt[pt_i].page_addr_low) << 12;
    phys_addr |= offset;

    return ((void*) phys_addr);
}

void vmm_init(void)
{
    // Addresses of the initial paging structures.
    uint64_t pml4_addr = (uint64_t)&pml4;

    // Zero the paging structures.
    memset((void*) pml4, 0, PAGE_SIZE);

    // Set up recursive PML4 mapping.
    vmm_page_map((void*)pml4, (void*)0xfffffffffffff000, PG_PR | PG_RW);

    // Identity map first 8 MiB, except for first page.
    for (size_t i = PAGE_SIZE; i < 0x800000; i += PAGE_SIZE)
    {
        vmm_page_map((void*)i, (void*)i, PG_PR | PG_RW);
    }

    // Load new PML4.
    asm volatile
    (
        "movq %0, %%cr3 \n"
        :
        : "a" (pml4_addr)
        :
    );

    // Reclaim boot paging structure.
    pmm_frame_free((void*) 0x1000); // PML4
    pmm_frame_free((void*) 0x2000); // PDP
    pmm_frame_free((void*) 0x3000); // PD
    pmm_frame_free((void*) 0x4000); // PT0
    pmm_frame_free((void*) 0x5000); // PT1
}

void vmm_page_map(void* phys, void* virt, uint16_t flags)
{
    size_t phys_addr = (size_t)phys;
    size_t virt_addr = (size_t)virt;
    size_t tmp_addr;
    Pdpte *pdpt;
    Pde *pd;
    Pte *pt;

    // Clear lowest 12 bits of both addresses.
    phys_addr ^= 0xFFF;
    virt_addr ^= 0xFFF;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);

    // TODO: Check if virtual address is already used.

    // Check if PML4 entry does not exist.
    if (pml4[pml4_i].present == 0)
    {
        // Set relevant flags.
        if (BIT_CHECK(flags, PG_PR_BIT))
            pml4[pml4_i].present = 1;
        if (BIT_CHECK(flags, PG_RW_BIT))
            pml4[pml4_i].write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            pml4[pml4_i].user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            pml4[pml4_i].write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            pml4[pml4_i].cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            pml4[pml4_i].accessed = 1;

        // Make new PDPT.
        tmp_addr = (size_t) pmm_frame_alloc();
        memset((void*)tmp_addr, 0, PAGE_SIZE);
        pml4[pml4_i].dir_ptr_addr_high = (tmp_addr >> 32) & 0xFFFFF;
        pml4[pml4_i].dir_ptr_addr_low = (tmp_addr >> 12) & 0xFFFFF;
        pdpt = (Pdpte*)tmp_addr;
    }
    else
    {
        tmp_addr = (size_t)pml4[pml4_i].dir_ptr_addr_high << 32;
        tmp_addr |= (size_t)pml4[pml4_i].dir_ptr_addr_low << 12;
        pdpt = (Pdpte*)tmp_addr;
    }

    // Check if PDPT entry does not exist.
    if (pdpt[pdpt_i].present == 0)
    {
        // Set relevant flags.
        if (BIT_CHECK(flags, PG_PR_BIT))
            pdpt[pdpt_i].present = 1;
        if (BIT_CHECK(flags, PG_RW_BIT))
            pdpt[pdpt_i].write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            pdpt[pdpt_i].user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            pdpt[pdpt_i].write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            pdpt[pdpt_i].cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            pdpt[pdpt_i].accessed = 1;

        // Make new PD.
        tmp_addr = (size_t) pmm_frame_alloc();
        memset((void*)tmp_addr, 0, PAGE_SIZE);
        pdpt[pdpt_i].dir_addr_high = (tmp_addr >> 32) & 0xFFFFF;
        pdpt[pdpt_i].dir_addr_low = (tmp_addr >> 12) & 0xFFFFF;
        pd = (Pde*)tmp_addr;
    }
    else
    {
        tmp_addr = (size_t)pdpt[pdpt_i].dir_addr_high << 32;
        tmp_addr |= (size_t)pdpt[pdpt_i].dir_addr_low << 12;
        pd = (Pde*)tmp_addr;
    }

    // Check if PD entry does not exist.
    if (pd[pd_i].present == 0)
    {
        // Set relevant flags.
        if (BIT_CHECK(flags, PG_PR_BIT))
            pd[pd_i].present = 1;
        if (BIT_CHECK(flags, PG_RW_BIT))
            pd[pd_i].write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            pd[pd_i].user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            pd[pd_i].write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            pd[pd_i].cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            pd[pd_i].accessed = 1;

        // Make new PT.
        tmp_addr = (size_t) pmm_frame_alloc();
        memset((void*)tmp_addr, 0, PAGE_SIZE);
        pd[pd_i].table_addr_high = (tmp_addr >> 32) & 0xFFFFF;
        pd[pd_i].table_addr_low = (tmp_addr >> 12) & 0xFFFFF;
        pt = (Pte*)tmp_addr;
    }
    else
    {
        tmp_addr = (size_t)pd[pd_i].table_addr_high << 32;
        tmp_addr |= (size_t)pd[pd_i].table_addr_low << 12;
        pt = (Pte*)tmp_addr;
    }

    // Modify PT entry.
    if (BIT_CHECK(flags, PG_PR_BIT))
        pt[pt_i].present = 1;
    if (BIT_CHECK(flags, PG_RW_BIT))
        pt[pt_i].write_enabled = 1;
    if (BIT_CHECK(flags, PG_U_BIT))
        pt[pt_i].user = 1;
    if (BIT_CHECK(flags, PG_WT_BIT))
        pt[pt_i].write_through = 1;
    if (BIT_CHECK(flags, PG_CD_BIT))
        pt[pt_i].cache_disabled = 1;
    if (BIT_CHECK(flags, PG_AC_BIT))
        pt[pt_i].accessed = 1;
    if (BIT_CHECK(flags, PG_DT_BIT))
        pt[pt_i].dirty = 1;
    if (BIT_CHECK(flags, PG_AT_BIT))
        pt[pt_i].attr = 1;
    if (BIT_CHECK(flags, PG_GL_BIT))
        pt[pt_i].global = 1;
    pt[pt_i].page_addr_high = (phys_addr >> 32) & 0xFFFFF;
    pt[pt_i].page_addr_low = (phys_addr >> 12) & 0xFFFFF;
}
