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

extern void* pml4_start;
extern void* kernel_end;

static Pml4e* vmm_pml4(void)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= RECURSIVE_INDEX << 30;
    addr |= RECURSIVE_INDEX << 21;
    addr |= RECURSIVE_INDEX << 12;

    Pml4e *pml4 = (Pml4e*)addr;
    return (pml4);
}

static Pdpte* vmm_pdpt(uint64_t pml4_i)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= RECURSIVE_INDEX << 30;
    addr |= RECURSIVE_INDEX << 21;
    addr |= pml4_i << 12;

    Pdpte *pdpt = (Pdpte*)addr;
    return (pdpt);
}

static Pde* vmm_pd(uint64_t pml4_i, uint64_t pdpt_i)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= RECURSIVE_INDEX << 30;
    addr |= pml4_i << 21;
    addr |= pdpt_i << 12;

    Pde *pd = (Pde*)addr;
    return (pd);
}

static Pte* vmm_pt(uint64_t pml4_i, uint64_t pdpt_i, uint64_t pd_i)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= pml4_i << 30;
    addr |= pdpt_i << 21;
    addr |= pd_i << 12;

    Pte *pt = (Pte*)addr;
    return (pt);
}

static void vmm_flush(void)
{
    size_t pml4_phys = (size_t)&pml40 - KERNEL_OFFSET;

    // Reload PML4.
    asm volatile
    (
        "movq %0, %%cr3 \n"
        :
        : "a" ((size_t) pml4_phys) // Physical address of the PML4.
        :
    );
}

void vmm_init(void)
{
    size_t KERNEL_PML4 = ((KERNEL_OFFSET >> 39) & 511);
    size_t KERNEL_PDPT = ((KERNEL_OFFSET >> 30) & 511);

    // Physical addresses of tables.
    size_t pml4_phys = (size_t)&pml40 - KERNEL_OFFSET;
    size_t pdpt0_phys = (size_t)&pdpt0 - KERNEL_OFFSET;
    size_t pd0_phys = (size_t)&pd0 - KERNEL_OFFSET;
    size_t pt0_phys = (size_t)&pt0 - KERNEL_OFFSET;
    size_t pt1_phys = (size_t)&pt1 - KERNEL_OFFSET;

    // Set up higher level paging structures.
    memset((void*)pml40, 0, PAGE_SIZE);
    memset((void*)pdpt0, 0, PAGE_SIZE);
    memset((void*)pd0, 0, PAGE_SIZE);
    memset((void*)pt0, 0, PAGE_SIZE);
    memset((void*)pt1, 0, PAGE_SIZE);
    pml40[KERNEL_PML4].present = 1;
    pml40[KERNEL_PML4].write_enabled = 1;
    pml40[KERNEL_PML4].dir_ptr_addr_high = (pdpt0_phys >> 32) & 0xFFFFF;
    pml40[KERNEL_PML4].dir_ptr_addr_low = (pdpt0_phys >> 12) & 0xFFFFF;
    pdpt0[KERNEL_PDPT].present = 1;
    pdpt0[KERNEL_PDPT].write_enabled = 1;
    pdpt0[KERNEL_PDPT].dir_addr_high = (pd0_phys >> 32) & 0xFFFFF;
    pdpt0[KERNEL_PDPT].dir_addr_low = (pd0_phys >> 12) & 0xFFFFF;
    pd0[0].present = 1;
    pd0[0].write_enabled = 1;
    pd0[0].table_addr_high = (pt0_phys >> 32) & 0xFFFFF;
    pd0[0].table_addr_low = (pt0_phys >> 12) & 0xFFFFF;
    pd0[1].present = 1;
    pd0[1].write_enabled = 1;
    pd0[1].table_addr_high = (pt1_phys >> 32) & 0xFFFFF;
    pd0[1].table_addr_low = (pt1_phys >> 12) & 0xFFFFF;

    // Map kernel pages (from 1 MiB to 4 MiB).
    // This will be remapped anyway after loading the new PML4.
    for (size_t i = (0x100000 / PAGE_SIZE); i < 512; i++)
    {
        size_t page_addr = i * PAGE_SIZE;
        pt0[i].present = 1;
        pt0[i].write_enabled = 1;
        pt0[i].page_addr_high = (page_addr >> 32) & 0xFFFFF;
        pt0[i].page_addr_low = (page_addr >> 12) & 0xFFFFF;
    }
    for (size_t i = 0; i < 512; i++)
    {
        size_t page_addr = i * PAGE_SIZE + 0x200000;
        pt1[i].present = 1;
        pt1[i].write_enabled = 1;
        pt1[i].page_addr_high = (page_addr >> 32) & 0xFFFFF;
        pt1[i].page_addr_low = (page_addr >> 12) & 0xFFFFF;
    }

    // Set up recursive mapping.
    pml40[RECURSIVE_INDEX].present = 1;
    pml40[RECURSIVE_INDEX].write_enabled = 1;
    pml40[RECURSIVE_INDEX].dir_ptr_addr_high = (pml4_phys >> 32) & 0xFFFFF;
    pml40[RECURSIVE_INDEX].dir_ptr_addr_low = (pml4_phys >> 12) & 0xFFFFF;

    // Reload PML4.
    vmm_flush();

    // Re-initialize physical memory manager.
    pmm_bitmap = pmm_bitmap + KERNEL_OFFSET;

    // Identity map lowest 1 MiB, except the first page.
    for (size_t addr = PAGE_SIZE; addr < 0x100000; addr += PAGE_SIZE)
    {
        vmm_page_map((void*)addr, (void*)addr, PG_PR | PG_RW);
    }

    // Remap kernel code and rodata.
    for (size_t addr = (size_t)&kernel_ro_start; addr < (size_t)&kernel_ro_end; addr += PAGE_SIZE)
    {
        vmm_page_map((void*)addr, (void*)(addr+KERNEL_OFFSET), PG_PR);
    }
    // Remap kernel data and bss.
    for (size_t addr = (size_t)&kernel_ro_end; addr < (size_t)&phys_end; addr += PAGE_SIZE)
    {
        vmm_page_map((void*)addr, (void*)(addr+KERNEL_OFFSET), PG_PR | PG_RW);
    }
}

void* vmm_phys_addr(void* virt)
{
    size_t virt_addr = (size_t)virt;
    Pml4e *pml4;
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
    pml4 = vmm_pml4();
    if (pml4[pml4_i].present == 0)
    {
        return (NULL);
    }

    // Check for valid PDPT entry.
    pdpt = vmm_pdpt(pml4_i);
    if (pdpt[pdpt_i].present == 0)
    {
        return (NULL);
    }

    // Check for valid PD entry.
    pd = vmm_pd(pml4_i, pdpt_i);
    if (pd[pd_i].present == 0)
    {
        return (NULL);
    }

    // Check for valid PT entry.
    pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    if (pt[pt_i].present == 0)
    {
        return (NULL);
    }

    // Find physical address of this page, and add offset.
    size_t phys_addr = ((size_t)pt[pt_i].page_addr_high) << 32;
    phys_addr |= ((size_t)pt[pt_i].page_addr_low) << 12;
    phys_addr |= offset;

    return ((void*) phys_addr);
}

void vmm_page_map(void* phys, void* virt, uint16_t flags)
{
    size_t phys_addr = (size_t)phys;
    size_t virt_addr = (size_t)virt;
    size_t tmp_addr;
    Pml4e *pml4;
    Pdpte *pdpt;
    Pde *pd;
    Pte *pt;

    // Clear lowest 12 bits of both addresses.
    phys_addr &= ~0xFFF;
    virt_addr &= ~0xFFF;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);

    // Check PML4 entry.
    pml4 = vmm_pml4();
    if (pml4[pml4_i].present == 0)
    {
        Pml4e tmp_pml4e = {0};

        // Set relevant flags.
        tmp_pml4e.present = 1;
        tmp_pml4e.write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            tmp_pml4e.user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            tmp_pml4e.write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            tmp_pml4e.cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            tmp_pml4e.accessed = 1;
        pml4[pml4_i] = tmp_pml4e;

        // Make new PDPT.
        tmp_addr = (size_t) pmm_frame_alloc();
        pml4[pml4_i].dir_ptr_addr_high = (tmp_addr >> 32) & 0xFFFF;
        pml4[pml4_i].dir_ptr_addr_low = (tmp_addr >> 12) & 0xFFFF;
        vmm_flush();
        tmp_addr = (size_t) vmm_pdpt(pml4_i);
        memset((void*)tmp_addr, 0, PAGE_SIZE);
    }

    // Check PDPT entry.
    pdpt = vmm_pdpt(pml4_i);
    if (pdpt[pdpt_i].present == 0)
    {
        Pdpte tmp_pdpte = {0};

        // Set relevant flags.
        tmp_pdpte.present = 1;
        tmp_pdpte.write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            tmp_pdpte.user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            tmp_pdpte.write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            tmp_pdpte.cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            tmp_pdpte.accessed = 1;
        pdpt[pdpt_i] = tmp_pdpte;

        // Make new PD.
        tmp_addr = (size_t) pmm_frame_alloc();
        pdpt[pdpt_i].dir_addr_high = (tmp_addr >> 32) & 0xFFFF;
        pdpt[pdpt_i].dir_addr_low = (tmp_addr >> 12) & 0xFFFF;
        vmm_flush();
        tmp_addr = (size_t) vmm_pd(pml4_i, pdpt_i);
        memset((void*)tmp_addr, 0, PAGE_SIZE);
    }

    // Check PD entry.
    pd = vmm_pd(pml4_i, pdpt_i);
    if (pd[pd_i].present == 0)
    {
        Pde tmp_pde = {0};

        // Set relevant flags.
        tmp_pde.present = 1;
        tmp_pde.write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            tmp_pde.user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            tmp_pde.write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            tmp_pde.cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            tmp_pde.accessed = 1;
        pd[pd_i] = tmp_pde;

        // Make new PT.
        tmp_addr = (size_t) pmm_frame_alloc();
        pd[pd_i].table_addr_high = (tmp_addr >> 32) & 0xFFFFF;
        pd[pd_i].table_addr_low = (tmp_addr >> 12) & 0xFFFFF;
        vmm_flush();
        tmp_addr = (size_t) vmm_pt(pml4_i, pdpt_i, pd_i);
        memset((void*)tmp_addr, 0, PAGE_SIZE);
    }

    // Make PT entry.
    pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    Pte tmp_pte = {0};
    if (BIT_CHECK(flags, PG_PR_BIT))
        tmp_pte.present = 1;
    if (BIT_CHECK(flags, PG_RW_BIT))
        tmp_pte.write_enabled = 1;
    if (BIT_CHECK(flags, PG_U_BIT))
        tmp_pte.user = 1;
    if (BIT_CHECK(flags, PG_WT_BIT))
        tmp_pte.write_through = 1;
    if (BIT_CHECK(flags, PG_CD_BIT))
        tmp_pte.cache_disabled = 1;
    if (BIT_CHECK(flags, PG_AC_BIT))
        tmp_pte.accessed = 1;
    if (BIT_CHECK(flags, PG_DT_BIT))
        tmp_pte.dirty = 1;
    if (BIT_CHECK(flags, PG_AT_BIT))
        tmp_pte.attr = 1;
    if (BIT_CHECK(flags, PG_GL_BIT))
        tmp_pte.global = 1;
    tmp_pte.page_addr_high = (phys_addr >> 32) & 0xFFFFF;
    tmp_pte.page_addr_low = (phys_addr >> 12) & 0xFFFFF;
    pt[pt_i] = tmp_pte;
    vmm_flush();
}

void vmm_page_unmap(void* virt)
{
    Pte *pt;
    size_t virt_addr = (size_t)virt;

    // Clear lowest 12 bits.
    virt_addr &= ~0xFFF;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);

    // Mark the page as not present.
    pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    pt[pt_i].present = 0;
}

void vmm_table_flags(void* entry, uint16_t flags)
{
    size_t phys_addr = (size_t) vmm_phys_addr(entry);
    Pte *pt = (Pte*)phys_addr;

    // Modify PT entry.
    Pte tmp_pte = {0};

    if (BIT_CHECK(flags, PG_PR_BIT))
        tmp_pte.present = 1;
    if (BIT_CHECK(flags, PG_RW_BIT))
        tmp_pte.write_enabled = 1;
    if (BIT_CHECK(flags, PG_U_BIT))
        tmp_pte.user = 1;
    if (BIT_CHECK(flags, PG_WT_BIT))
        tmp_pte.write_through = 1;
    if (BIT_CHECK(flags, PG_CD_BIT))
        tmp_pte.cache_disabled = 1;
    if (BIT_CHECK(flags, PG_AC_BIT))
        tmp_pte.accessed = 1;
    if (BIT_CHECK(flags, PG_DT_BIT))
        tmp_pte.dirty = 1;
    if (BIT_CHECK(flags, PG_AT_BIT))
        tmp_pte.attr = 1;
    if (BIT_CHECK(flags, PG_GL_BIT))
        tmp_pte.global = 1;
    tmp_pte.page_addr_high = (phys_addr >> 32) & 0xFFFFF;
    tmp_pte.page_addr_low = (phys_addr >> 12) & 0xFFFFF;
    *pt = tmp_pte;
}

void* vmm_page_alloc_kernel(void)
{
    // Search kernel memory for a free page.
    for (size_t virt = (size_t)&kernel_end; virt < MEMORY_MAX; virt += PAGE_SIZE)
    {
        // If free page is found.
        if (vmm_phys_addr((void*) virt) == NULL)
        {
            // Allocate frame for this page.
            void* frame = (void*) pmm_frame_alloc();
            vmm_page_map(frame, (void*)virt, PG_PR | PG_RW);

            void* ret = (void*)virt;

            return (ret);
        }
    }

    // Else.
    return (NULL);
}

void vmm_page_free_kernel(void* virt)
{
    void* phys = vmm_phys_addr(virt);

    // Check that the virtual address exists.
    if (phys != NULL)
    {
        vmm_page_unmap(virt);
        pmm_frame_free(phys);
    }
}
