// Authors: Seth McBee
// Created: 2018-5-10
// Description: Paging structure management.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/pmm.h>

void paging_init(void)
{
    // Addresses of the initial paging structures.
    uint64_t pml4_addr = (uint64_t)&pml4;
    uint64_t pdpt0_addr = (uint64_t)&pdpt0;
    uint64_t pd0_addr = (uint64_t)&pd0;
    uint64_t pt0_addr = (uint64_t)&pt0;

    // Zero the paging structures.
    memset((void*) pml4, 0, PAGE_SIZE);
    memset((void*) pdpt0, 0, PAGE_SIZE);
    memset((void*) pd0, 0, PAGE_SIZE);
    memset((void*) pt0, 0, PAGE_SIZE);

    // Identity map first 8 MiB.

    // First PML4 entry.
    pml4[0].present = 1;
    pml4[0].write_enabled = 1;
    pml4[0].user = 0;
    pml4[0].write_through = 0;
    pml4[0].cache_disabled = 0;
    pml4[0].accessed = 0;
    pml4[0].zero_low = 0;
    pml4[0].zero_high = 0;
    pml4[0].sign_extend = 0;
    pml4[0].dir_ptr_addr_low = (pdpt0_addr >> 12) & 0xFFFFF;
    pml4[0].dir_ptr_addr_high = (pdpt0_addr >> 32) & 0xFFFFF;

    // First PDPT entry.
    pdpt0[0].present = 1;
    pdpt0[0].write_enabled = 1;
    pdpt0[0].user = 0;
    pdpt0[0].write_through = 0;
    pdpt0[0].cache_disabled = 0;
    pdpt0[0].accessed = 0;
    pdpt0[0].zero = 0;
    pdpt0[0].sign_extend = 0;
    pdpt0[0].dir_addr_low = (pd0_addr >> 12) & 0xFFFFF;
    pdpt0[0].dir_addr_high = (pd0_addr >> 32) & 0xFFFFF;

    // First PD entry.
    pd0[0].present = 1;
    pd0[0].write_enabled = 1;
    pd0[0].user = 0;
    pd0[0].write_through = 0;
    pd0[0].cache_disabled = 0;
    pd0[0].accessed = 0;
    pd0[0].zero = 0;
    pd0[0].sign_extend = 0;
    pd0[0].table_addr_low = (pt0_addr >> 12) & 0xFFFFF;
    pd0[0].table_addr_high = (pt0_addr >> 32) & 0xFFFFF;

    // Identity map first two megabytes, except for the first page.
    for (size_t i = 1; i < 512; i++)
    {
        uint64_t page_addr = i * PAGE_SIZE;
        pt0[i].present = 1;
        pt0[i].write_enabled = 1;
        pt0[i].user = 0;
        pt0[i].write_through = 0;
        pt0[i].cache_disabled = 0;
        pt0[i].accessed = 0;
        pt0[i].dirty = 0;
        pt0[i].attr = 0;
        pt0[i].global = 0;
        pt0[i].sign_extend = 0;
        pt0[i].page_addr_low = (page_addr >> 12) & 0xFFFFF;
        pt0[i].page_addr_high = (page_addr >> 32) & 0xFFFFF;
    }

    // Allocate more page tables as needed and map next 6 MiB.
    for (size_t pd_index = 1; pd_index < 4; pd_index++)
    {
        // Allocate new page for new page table.
        size_t pt_i_addr = pmm_frame_alloc();
        Pte *pt_i = (Pte*) pt_i_addr;

        // Build page directory entry.
        pd0[pd_index].present = 1;
        pd0[pd_index].write_enabled = 1;
        pd0[pd_index].user = 0;
        pd0[pd_index].write_through = 0;
        pd0[pd_index].cache_disabled = 0;
        pd0[pd_index].accessed = 0;
        pd0[pd_index].zero = 0;
        pd0[pd_index].sign_extend = 0;
        pd0[pd_index].table_addr_low = (pt_i_addr >> 12) & 0xFFFFF;
        pd0[pd_index].table_addr_high = (pt_i_addr >> 32) & 0xFFFFF;

        // Identity map two megabytes in new page table.
        for (size_t pt_index = 0; pt_index < 512; pt_index++)
        {
            uint64_t page_addr = (pt_index + pd_index*PAGE_SIZE)*PAGE_SIZE;
            pt_i[pt_index].present = 1;
            pt_i[pt_index].write_enabled = 1;
            pt_i[pt_index].user = 0;
            pt_i[pt_index].write_through = 0;
            pt_i[pt_index].cache_disabled = 0;
            pt_i[pt_index].accessed = 0;
            pt_i[pt_index].dirty = 0;
            pt_i[pt_index].attr = 0;
            pt_i[pt_index].global = 0;
            pt_i[pt_index].sign_extend = 0;
            pt_i[pt_index].page_addr_low = (page_addr >> 12) & 0xFFFFF;
            pt_i[pt_index].page_addr_high = (page_addr >> 32) & 0xFFFFF;
        }
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
    pmm_frame_free(0x1000); // PML4
    pmm_frame_free(0x2000); // PDP
    pmm_frame_free(0x3000); // PD
    pmm_frame_free(0x4000); // PT0
    pmm_frame_free(0x5000); // PT1
}
