// Authors: Seth McBee
// Created: 2018-5-11
// Description: Liballoc dependencies.

#include <arch/x86_64/memory/pmm.h>
#include <liballoc/liballoc.h>

// STUB.

int liballoc_lock(void)
{
    asm volatile ("cli \n");
    return (0);
}

int liballoc_unlock(void)
{
    asm volatile ("sti \n");
    return (0);
}

void* liballoc_alloc(int pages)
{
    return ( vmm_page_alloc_kernel() );
}

int liballoc_free(void* page,int pages)
{
    pmm_frame_free(page);
    return (0);
}
