/**
 * @file pmm.c
 * @author Seth McBee
 * @date 2018-5-7
 * @brief Physical memory management.
 */

#include <globals.h>

#include <string.h>

#include <kernel.h>
#include <arch/x86_64/multiboot2.h>
#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/pmm.h>

// Defined in linker script.
extern void *phys_end;

void pmm_init(struct multiboot_tag_mmap *mb_mmap)
{
    // Number of memory map entries.
    size_t mmap_entry_count = mb_mmap->size / mb_mmap->entry_size;

    // Point frame bitmap to end of kernel memory (or end of a kernel
    // module if one was loaded), aligned to next page.
    pmm_bitmap = (uint8_t*)&phys_end;
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

    // Align bitmap length to page boundary.
    pmm_bitmap_len = pmm_frames_available / 8;
    if (pmm_bitmap_len % 8 != 0)
    {
        pmm_bitmap_len += 1 - (pmm_bitmap_len % 8);
    }

    // Check if we have room to store the bitmap before the
    // kernel module appears.
    if (pmm_bitmap + pmm_bitmap_len >= (uint8_t*)kernel_module)
    {
        uintptr_t mod_end = (uintptr_t)kernel_module->mod_end;
        pmm_bitmap = (uint8_t*)mod_end;

        // Align to page boundary.
        if ((size_t)&pmm_bitmap % PAGE_SIZE != 0)
        {
            pmm_bitmap += PAGE_SIZE - ((size_t)&pmm_bitmap % PAGE_SIZE);
        }
    }

    // Set all bitmap entries to used.
    memset(pmm_bitmap, 0xFF, pmm_bitmap_len);

    // Minimum base address to consider a frame to be free.
    size_t base_min = (size_t)pmm_bitmap + pmm_bitmap_len;

    // Verify that bitmap fits below our initial paged memory.
    if (base_min >= (size_t)&phys_end + 0x200000)
    {
        kernel_panic("Not enough paged memory to allocate PMM bitmap.");
    }

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
                    pmm_frame_free((void*)addr);
                }
                addr += PAGE_SIZE;
                len -= PAGE_SIZE;
            }
        }
    }

    // Mark frames used by the loaded kernel module, if any.
    if (kernel_module != NULL)
    {
        size_t offset = kernel_module->mod_start;
        size_t len = kernel_module->size;

        // Align base address to page boundary, downwards.
        if (offset % PAGE_SIZE != 0)
        {
            offset -= PAGE_SIZE - (offset % PAGE_SIZE);
        }

        // Align length to page boundary, upwards.
        if (len % PAGE_SIZE != 0)
        {
            len += PAGE_SIZE - (len % PAGE_SIZE);
        }

        // Mark frames.
        while (len > 0)
        {
            pmm_frame_free((void*)offset);
            offset += PAGE_SIZE;
            len -= PAGE_SIZE;
        }
    }

    // TODO: Allocate structures to manage special unavailable memory
    // (like ACPI).
}

void pmm_frame_free(void* addr)
{
    size_t frame_num = (size_t)addr / PAGE_SIZE;
    size_t byte = frame_num / 8;
    uint8_t bit = frame_num % 8;

    pmm_bitmap[byte] = BIT_CLEAR(pmm_bitmap[byte], bit);

    pmm_frames_free++;
    pmm_frames_used--;
}

void* pmm_frame_alloc(void)
{
    // Check if any frames are available.
    if (pmm_frames_free == 0)
    {
        return (NULL);
    }

    // Find first empty page.
    // TODO: Optimize this search.
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
                    void *addr = (void*) (PAGE_SIZE * (8 * byte + bit));
                    return (addr);
                }
            }
        }
    }

    // Since we don't currently support swapping or adequate signaling,
    // just panic if we don't find a free frame.
    kernel_panic("Out Of Memory: PMM could not find an available frame.");

    // Else (this should never be reached).
    return (NULL);
}
