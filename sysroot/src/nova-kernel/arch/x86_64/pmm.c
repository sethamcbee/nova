// Authors: Seth McBee
// Created: 2018-5-7
// Description: Physical memory manager.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>

#include <globals.h>
#include <kernel.h>
#include <arch/x86_64/multiboot2.h>
#include <arch/x86_64/pmm.h>

// Defined in linker script.
extern void* _sections_end;

// Initializes paging - identity maps first 8 MiB.
static void pmm_paging_init(void);

void pmm_init(struct multiboot_tag_mmap *mb_mmap)
{
    // Number of memory map entries.
    size_t mmap_entry_count = mb_mmap->size / mb_mmap->entry_size;

    // Point frame bitmap to end of kernel memory, aligned to next page.
    pmm_bitmap = (uint8_t*)&_sections_end;
    if ((size_t)&pmm_bitmap % PAGE_SIZE != 0)
    {
        pmm_bitmap += PAGE_SIZE - ((size_t)&pmm_bitmap % PAGE_SIZE);
    }

    pmm_frames_free = 0;
    pmm_frames_used = 0;
    pmm_frames_available = 0;
    pmm_frames_unavailable = 0;

    // Temporarily stores memory map.
    struct multiboot_mmap_entry multiboot2_mmap[mmap_entry_count];

    // Iterate over memory map and copy entries.
    for (size_t i = 0; i < mmap_entry_count; i++)
    {
        multiboot2_mmap[i].addr = mb_mmap->entries[i].addr;
        multiboot2_mmap[i].len = mb_mmap->entries[i].len;
        multiboot2_mmap[i].type = mb_mmap->entries[i].type;
        multiboot2_mmap[i].zero = 0;

        // Check if memory is available.
        if (multiboot2_mmap[i].type == 1)
        {
            size_t addr = multiboot2_mmap[i].addr;
            size_t len = multiboot2_mmap[i].len;

            // Align base address to page boundary, upwards.
            if (addr % PAGE_SIZE != 0)
            {
                size_t offset = PAGE_SIZE - (addr % PAGE_SIZE);
                addr += offset;
                len -= offset;
            }

            // Align length to page boundary, downwards.
            if (len % PAGE_SIZE != 0)
            {
                len -= PAGE_SIZE - (addr % PAGE_SIZE);
            }

            pmm_frames_available += (len / PAGE_SIZE);
        }
        else    // Track unavailable memory as well.
        {
            size_t addr = multiboot2_mmap[i].addr;
            size_t len = multiboot2_mmap[i].len;

            // Align base address to page boundary, downwards.
            size_t offset = addr % PAGE_SIZE;
            addr -= offset;
            len += offset;

            // Align length to page boundary, upwards.
            len += len % PAGE_SIZE;

            pmm_frames_unavailable += (len / PAGE_SIZE);
        }
    }

    // Set bitmap to full, and align length to page boundary.
    pmm_bitmap_len = pmm_frames_available / 8;
    if (pmm_bitmap_len % 8 != 0)
    {
        pmm_bitmap_len += 1 - (pmm_bitmap_len % 8);
    }
    memset(pmm_bitmap, 0xFF, pmm_bitmap_len);

    // Minimum base address to consider a frame to be free.
    uint64_t base_min = (uint64_t)pmm_bitmap + pmm_bitmap_len;

    // Allocate stack of pages that are available to processes,
    // starting at the end of kernel memory.
    for (size_t i = 0; i < mmap_entry_count; i++)
    {
        // If available.
        if (multiboot2_mmap[i].type == 1)
        {
            size_t addr = multiboot2_mmap[i].addr;
            size_t len = multiboot2_mmap[i].len;

            // Align base address to page boundary, upwards.
            if (addr % PAGE_SIZE != 0)
            {
                size_t offset = PAGE_SIZE - (addr % PAGE_SIZE);
                addr += offset;
                len -= offset;
            }

            // Align length to page boundary, downwards.
            if (len % PAGE_SIZE != 0)
            {
                len -= PAGE_SIZE - (len % PAGE_SIZE);
            }

            // Add frames to bitmap.
            while (len >= PAGE_SIZE)
            {
                if (addr >= base_min)
                {
                    pmm_bitmap_free(addr);
                }
                addr += PAGE_SIZE;
                len -= PAGE_SIZE;
            }
        }
    }

    // Initialize new paging struct.
    pmm_paging_init();

    // TODO: Allocate structures to manage special unavailable memory
    // (like ACPI).

    // TODO: Reclaim boot paging structure.
}

void pmm_paging_init(void)
{
    // Addresses of the initial paging structures.
    uint64_t pml4_addr = (uint64_t)&pml4;
    uint64_t pdp0_addr = (uint64_t)&pdp0;
    uint64_t pd0_addr = (uint64_t)&pd0;
    uint64_t pt0_addr = (uint64_t)&pt0;

    // Zero the paging structures.
    memset((void*) pml4, 0, PAGE_SIZE);
    memset((void*) pdp0, 0, PAGE_SIZE);
    memset((void*) pd0, 0, PAGE_SIZE);
    // pt0 will be completely filled so no need to zero.

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
    pml4[0].dir_ptr_addr_low = (pdp0_addr >> 12) & 0xFFFFF;
    pml4[0].dir_ptr_addr_high = (pdp0_addr >> 32) & 0xFFFFF;

    // First PDP entry.
    pdp0[0].present = 1;
    pdp0[0].write_enabled = 1;
    pdp0[0].user = 0;
    pdp0[0].write_through = 0;
    pdp0[0].cache_disabled = 0;
    pdp0[0].accessed = 0;
    pdp0[0].zero = 0;
    pdp0[0].sign_extend = 0;
    pdp0[0].dir_addr_low = (pd0_addr >> 12) & 0xFFFFF;
    pdp0[0].dir_addr_high = (pd0_addr >> 32) & 0xFFFFF;

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

    // Identity map first two megabytes.
    for (size_t i = 0; i < 512; i++)
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
        size_t pt_i_addr = pmm_bitmap_alloc();
        Pte *pt_i = (Pte*) pt_i_addr;

        // Build page table entry.
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

        // Identity map next two megabytes.
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
    pmm_bitmap_free(0x1000); // PML4
    pmm_bitmap_free(0x2000); // PDP
    pmm_bitmap_free(0x3000); // PD
    pmm_bitmap_free(0x4000); // PT0
    pmm_bitmap_free(0x5000); // PT1
}

void pmm_bitmap_free(size_t addr)
{
    size_t frame_num = addr / PAGE_SIZE;
    size_t byte = frame_num / 8;
    uint8_t bit = frame_num % 8;

    pmm_bitmap[byte] = BIT_CLEAR(pmm_bitmap[byte], bit);

    pmm_frames_free++;
    pmm_frames_used--;
}

size_t pmm_bitmap_alloc(void)
{
    // Find first empty page.
    for (size_t byte = 0; byte < pmm_bitmap_len; byte++)
    {
        // Check if byte contains an empty page.
        if (pmm_bitmap[byte] < 255)
        {
            uint8_t block = pmm_bitmap[byte];

            // Check each bit for a page.
            for (uint8_t bit = 0; bit < 8; bit++)
            {
                if (BIT_CHECK(block, bit) == 0)
                {
                    pmm_bitmap[byte] = BIT_SET(pmm_bitmap[byte], bit);
                    pmm_frames_free--;
                    pmm_frames_used++;
                    size_t addr = PAGE_SIZE * (8 * byte + bit);
                    return (addr);
                }
            }
        }
    }

    // Else.
    return (0);
}
