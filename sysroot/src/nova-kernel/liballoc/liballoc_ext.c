// Authors: Seth McBee
// Created: 2018-5-11
// Description: Liballoc dependencies.

#include <liballoc/liballoc.h>

#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/vmm.h>

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
    if (pages == 0)
        return (NULL);

    return ( vmm_pages_alloc_kernel(pages) );
}

int liballoc_free(void* page,int pages)
{
    while (pages > 0)
    {
        vmm_page_free_kernel(page);
        page += PAGE_SIZE;
        pages--;
    }

    return (0);
}
