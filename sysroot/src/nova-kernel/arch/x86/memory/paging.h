/**
 * @file paging.h
 * @author Seth McBee
 * @date 2018-10-3
 * @brief Paging structure management.
 */

#pragma once

#include <globals.h>

#define PAGE_SIZE   (0x1000) // 4 KiB pages.
#define PAGE_COUNT  1024     // Entries per table.

// Flags for mapping entries.
#define PG_PR 0b000000001 // Present.
#define PG_RW 0b000000010 // Write-enabled.
#define PG_U  0b000000100 // Usermode.
#define PG_WT 0b000001000 // Write-through.
#define PG_CD 0b000010000 // Cache disabled.
#define PG_AC 0b000100000 // Accessed.
#define PG_D  0b001000000 // Dirty.
#define PG_S  0b010000000 // Page size.
#define PG_GL 0b100000000 // Global.
#define PG_PR_BIT 0
#define PG_RW_BIT 1
#define PG_U_BIT  2
#define PG_WT_BIT 3
#define PG_CD_BIT 4
#define PG_AC_BIT 5
#define PG_D_BIT  6
#define PG_S_BIT  7
#define PG_GL_BIT 8

/// Page directory entry.
typedef struct __attribute__((packed))
{
    uint8_t present : 1;
    uint8_t write_enabled : 1;
    uint8_t user : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t zero : 1;
    uint8_t size : 1;
    uint8_t ignored : 1;
    uint8_t available : 3;
    uint32_t table_addr : 20;
} Pde;

/// Page table entry.
typedef struct __attribute__((packed))
{
    uint8_t present : 1;
    uint8_t write_enabled : 1;
    uint8_t user : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t zero : 1;
    uint8_t global : 1;
    uint8_t available : 3;
    uint32_t page_addr : 20;
} Pte;

// Initial kernel paging structures.
Pde pd0[PAGE_COUNT] __attribute__((aligned(PAGE_SIZE)));
Pte pt0[PAGE_COUNT] __attribute__((aligned(PAGE_SIZE)));
