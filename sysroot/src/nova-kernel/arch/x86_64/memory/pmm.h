// Authors: Seth McBee
// Created: 2018-5-7
// Description: Physical memory manager.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <arch/x86_64/multiboot2.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initializes physical memory manager using Multiboot2 memory map.
void pmm_init(struct multiboot_tag_mmap *mb_mmap);

// Marks a frame as free in the PMM bitmap.
void pmm_frame_free(void* addr);

// Allocates a frame in the PMM bitmap.
void* pmm_frame_alloc(void);

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

#ifdef __cplusplus
}
#endif
