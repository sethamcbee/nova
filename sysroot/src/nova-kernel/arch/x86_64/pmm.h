// Authors: Seth McBee
// Created: 2018-5-7
// Description: Physical memory manager.

#ifndef PMM_H
#define PMM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/multiboot2.h>

#define PAGE_SIZE (0x1000) // 4 KiB

// Page map level 4 entry.
typedef struct __attribute__((packed))
{
    uint8_t present : 1;
    uint8_t write_enabled : 1;
    uint8_t user : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t ignored : 1;
    uint8_t zero_low : 1;
    uint8_t zero_high : 1;
    uint8_t available_low : 3;
    uint32_t dir_ptr_addr_low : 20;
    uint32_t dir_ptr_addr_high : 20;
    uint16_t available_high : 11;
    uint8_t sign_extend : 1;
} Pml4e;

// Page directory pointer entry.
typedef struct __attribute__((packed))
{
    uint8_t present : 1;
    uint8_t write_enabled : 1;
    uint8_t user : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t ignored_low : 1;
    uint8_t zero : 1;
    uint8_t ignored_high : 1;
    uint8_t available_low : 3;
    uint32_t dir_addr_low : 20;
    uint32_t dir_addr_high : 20;
    uint16_t available_high : 11;
    uint8_t sign_extend : 1;
} Pdpe;

// Page directory entry.
typedef struct __attribute__((packed))
{
    uint8_t present : 1;
    uint8_t write_enabled : 1;
    uint8_t user : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t ignored_low : 1;
    uint8_t zero : 1;
    uint8_t ignored_high : 1;
    uint8_t available_low : 3;
    uint32_t table_addr_low : 20;
    uint32_t table_addr_high : 20;
    uint16_t available_high : 11;
    uint8_t sign_extend : 1;
} Pde;

// Page table entry.
typedef struct
{
    uint8_t present : 1;
    uint8_t write_enabled : 1;
    uint8_t user : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t attr : 1;
    uint8_t global : 1;
    uint8_t available_low : 3;
    uint32_t page_addr_low : 20;
    uint32_t page_addr_high : 20;
    uint16_t available_high : 11;
    uint8_t sign_extend : 1;
} __attribute__((packed)) Pte;

// Initial kernel paging structures.
extern Pml4e pml4[512] __attribute__((aligned(PAGE_SIZE)));
extern Pdpe pdp0[512] __attribute__((aligned(PAGE_SIZE)));
extern Pde pd0[512] __attribute__((aligned(PAGE_SIZE)));
extern Pte pt0[512] __attribute__((aligned(PAGE_SIZE)));

// Initializes physical memory manager using Multiboot2 memory map.
void pmm_init(struct multiboot_tag_mmap *mb_mmap);

// Marks a frame as free in the PMM bitmap.
void pmm_bitmap_free(size_t addr);

// Allocates a frame in the PMM bitmap.
size_t pmm_bitmap_alloc(void);

// Bitmap representation of the physical memory.
uint8_t *pmm_bitmap;
size_t pmm_bitmap_len;

// Number of page frames that are free to be allocated.
size_t pmm_frames_free;

// Number of page frames that have been allocated.
size_t pmm_frames_used;

// Number of available page frames.
size_t pmm_frames_available;

// Number of unavailable page frames.
size_t pmm_frames_unavailable;

#endif // PMM_H
