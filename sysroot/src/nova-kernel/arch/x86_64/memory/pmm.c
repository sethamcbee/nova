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
#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/pmm.h>

// Defined in linker script.
extern void* _sections_end;

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

    // Align bitmap length to page boundary..
    pmm_bitmap_len = pmm_frames_available / 8;
    if (pmm_bitmap_len % 8 != 0)
    {
        pmm_bitmap_len += 1 - (pmm_bitmap_len % 8);
    }

    // Set all bitmap entries to used.
    memset(pmm_bitmap, 0xFF, pmm_bitmap_len);

    // Minimum base address to consider a frame to be free.
    uint64_t base_min = (uint64_t)pmm_bitmap + pmm_bitmap_len;

    // Verify that bitmap fits below our initial paged memory (4 MiB).
    if (base_min >= 0x400000)
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
                    pmm_frame_free(addr);
                }
                addr += PAGE_SIZE;
                len -= PAGE_SIZE;
            }
        }
    }

    // TODO: Allocate structures to manage special unavailable memory
    // (like ACPI).
}

void pmm_frame_free(size_t addr)
{
    size_t frame_num = addr / PAGE_SIZE;
    size_t byte = frame_num / 8;
    uint8_t bit = frame_num % 8;

    pmm_bitmap[byte] = BIT_CLEAR(pmm_bitmap[byte], bit);

    pmm_frames_free++;
    pmm_frames_used--;
}

size_t pmm_frame_alloc(void)
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
