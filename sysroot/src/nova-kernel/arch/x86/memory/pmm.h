/**
 * @file pmm.h
 * @author Seth McBee
 * @date 2018-10-3
 * @brief Physical memory management.
 */

#pragma once

#include <globals.h>

#include <arch/x86/multiboot2.h>

// Initializes physical memory manager using Multiboot2 memory map.
void pmm_init(struct multiboot_tag_mmap* mb_mmap);

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
