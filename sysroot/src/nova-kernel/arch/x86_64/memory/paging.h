// Authors: Seth McBee
// Created: 2018-5-10
// Description: Paging structure management.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE   (0x1000) // 4 KiB pages.
#define PAGE_COUNT  512      // Entries per table.

// Flags for mapping entries.
#define PG_PR 0b000000001 // Present.
#define PG_RW 0b000000010 // Write-enabled.
#define PG_U  0b000000100 // Usermode.
#define PG_WT 0b000001000 // Write-through.
#define PG_CD 0b000010000 // Cache disabled.
#define PG_AC 0b000100000 // Accessed.
#define PG_DT 0b001000000 // Dirty.
#define PG_AT 0b010000000 // Attribute.
#define PG_GL 0b100000000 // Global.
#define PG_PR_BIT 0
#define PG_RW_BIT 1
#define PG_U_BIT  2
#define PG_WT_BIT 3
#define PG_CD_BIT 4
#define PG_AC_BIT 5
#define PG_DT_BIT 6
#define PG_AT_BIT 7
#define PG_GL_BIT 8

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
    uint8_t exists : 1;
    uint8_t available_low : 2;
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
    uint8_t exists : 1;
    uint8_t available_low : 2;
    uint32_t dir_addr_low : 20;
    uint32_t dir_addr_high : 20;
    uint16_t available_high : 11;
    uint8_t sign_extend : 1;
} Pdpte;

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
    uint8_t exists : 1;
    uint8_t available_low : 2;
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
    uint8_t exists : 1;
    uint8_t available_low : 2;
    uint32_t page_addr_low : 20;
    uint32_t page_addr_high : 20;
    uint16_t available_high : 11;
    uint8_t sign_extend : 1;
} __attribute__((packed)) Pte;

// Initial kernel paging structures.
Pml4e pml40[512] __attribute__((aligned(PAGE_SIZE)));
Pdpte pdpt0[512] __attribute__((aligned(PAGE_SIZE)));
Pde pd0[512] __attribute__((aligned(PAGE_SIZE)));
Pte pt0[512] __attribute__((aligned(PAGE_SIZE)));
Pte pt1[512] __attribute__((aligned(PAGE_SIZE)));
