/**
 * @file liballoc_ext.c
 * @author Seth McBee
 * @date 2018-5-11
 * @brief liballoc dependencies.
 */

#include <liballoc/liballoc.h>

#ifdef ARCH_X86_64
#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/vmm.h>
#endif // ARCH_X86_64

#ifdef ARCH_X86
#include <arch/x86/memory/paging.h>
#include <arch/x86/memory/vmm.h>
#endif // ARCH_X86

// STUB.
// Security risk.

int liballoc_lock(void)
{
    //asm volatile ("cli \n");
    return (0);
}

int liballoc_unlock(void)
{
    //asm volatile ("sti \n");
    return (0);
}

void* liballoc_alloc(int pages)
{
    if (pages == 0)
        return (NULL);

    void* ret = vmm_pages_alloc_kernel(pages);
    vmm_table_flags(ret, PG_PR | PG_RW | PG_U);

    return ( ret );
}

int liballoc_free(void* page,int pages)
{
    uint8_t* p = page;

    while (pages > 0)
    {
        vmm_page_free_kernel(p);
        p += PAGE_SIZE;
        pages--;
    }

    return (0);
}
