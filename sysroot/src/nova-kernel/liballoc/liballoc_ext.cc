/**
 * @file liballoc_ext.c
 * @author Seth McBee
 * @date 2018-5-11
 * @brief liballoc dependencies.
 */

#include <mutex>

#include <liballoc/liballoc.h>

#ifdef ARCH_X86_64
#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/vmm.h>
#endif // ARCH_X86_64

#ifdef ARCH_X86
#include <arch/x86/memory/paging.h>
#include <arch/x86/memory/vmm.h>
#endif // ARCH_X86

std::mutex liballoc_mtx;

extern "C" int liballoc_lock(void)
{
    liballoc_mtx.lock();
    return (0);
}

extern "C" int liballoc_unlock(void)
{
    liballoc_mtx.unlock();
    return (0);
}

extern "C" void* liballoc_alloc(int pages)
{
    if (pages == 0)
        return (NULL);

    void* ret = vmm_pages_alloc_kernel(pages);
    vmm_table_flags(ret, PG_PR | PG_RW | PG_U);

    return (ret);
}

// NOTE: Is not currently freeing pages.
extern "C" int liballoc_free(void* page,int pages)
{
    uint8_t* p = (uint8_t*)page;

    while (pages > 0)
    {
        vmm_page_free_kernel(p);
        p += PAGE_SIZE;
        pages--;
    }

    return (0);
}
