// Authors: Seth McBee
// Created: 2018-5-10
// Description: Virtual memory manager.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/pmm.h>
#include <arch/x86_64/memory/vmm.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Maximum virtual address.
static const size_t MEM_MAX = (size_t)-1;

// Linker symbols.
extern void* pml4_start;
extern void* kernel_end;

// Used to iterate over nodes without recursion.
typedef struct Vmm_Node_Stack Vmm_Node_Stack;
struct Vmm_Node_Stack
{
    Vmm_Node** base;
    size_t count;
    size_t max;
};

Vmm_Node_Stack* vmm_node_stack_ctor(void)
{
    static const size_t DEFAULT_MAX = 5;

    Vmm_Node_Stack* stk = malloc(sizeof(Vmm_Node_Stack));
    stk->base = malloc(sizeof(Vmm_Node*) * DEFAULT_MAX);
    stk->count = 0;
    stk->max = DEFAULT_MAX;

    return (stk);
}

void vmm_node_stack_push(Vmm_Node_Stack* stk, Vmm_Node* node)
{
    // Resize if needed. Increases in increments of 5.
    if (stk->count == stk->max)
    {
        stk->max += 5;
        size_t new_size = sizeof(Vmm_Node*) * (stk->max);
        stk->base = (Vmm_Node**) realloc((void*)stk->base, new_size);
    }

    // Add to the stack and increment count.
    stk->base[stk->count] = node;
    stk->count++;
}

Vmm_Node* vmm_node_stack_pop(Vmm_Node_Stack* stk)
{
    // Check if the list is empty.
    if (stk->count == 0)
    {
        return (NULL);
    }

    // Retrieve the next node and remove it from the stack.
    stk->count--;

    return (stk->base[stk->count]);
}

void vmm_node_stack_dtor(Vmm_Node_Stack* stk)
{
    // Free the actual stack.
    free((void*)stk->base);

    // Free this object.
    free((void*)stk);
}

static Pml4e* vmm_pml4(void)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= RECURSIVE_INDEX << 30;
    addr |= RECURSIVE_INDEX << 21;
    addr |= RECURSIVE_INDEX << 12;

    Pml4e *pml4 = (Pml4e*)addr;
    return (pml4);
}

static Pdpte* vmm_pdpt(uint64_t pml4_i)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= RECURSIVE_INDEX << 30;
    addr |= RECURSIVE_INDEX << 21;
    addr |= pml4_i << 12;

    Pdpte *pdpt = (Pdpte*)addr;
    return (pdpt);
}

static Pde* vmm_pd(uint64_t pml4_i, uint64_t pdpt_i)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= RECURSIVE_INDEX << 30;
    addr |= pml4_i << 21;
    addr |= pdpt_i << 12;

    Pde *pd = (Pde*)addr;
    return (pd);
}

static Pte* vmm_pt(uint64_t pml4_i, uint64_t pdpt_i, uint64_t pd_i)
{
    uint64_t addr = (RECURSIVE_INDEX << 39);

    if ((addr & (111UL << 47)) > 0)
    {
        // Sign extend.
        addr |= 0xFFFF000000000000UL;
    }

    addr |= pml4_i << 30;
    addr |= pdpt_i << 21;
    addr |= pd_i << 12;

    Pte *pt = (Pte*)addr;
    return (pt);
}

static void vmm_flush(void)
{
    size_t pml4_phys = (size_t)&pml40 - KERNEL_OFFSET;

    // Reload PML4.
    asm volatile
    (
        "movq %0, %%cr3 \n"
        :
        : "a" ((size_t) pml4_phys) // Physical address of the PML4.
        :
    );
}

void vmm_init(void)
{
    // Stores the initial free node.
    Vmm_Node _vmm_tree_kernel_free;

    size_t KERNEL_PML4 = ((KERNEL_OFFSET >> 39) & 511);
    size_t KERNEL_PDPT = ((KERNEL_OFFSET >> 30) & 511);

    // Physical addresses of tables.
    size_t pml4_phys = (size_t)&pml40 - KERNEL_OFFSET;
    size_t pdpt0_phys = (size_t)&pdpt0 - KERNEL_OFFSET;
    size_t pd0_phys = (size_t)&pd0 - KERNEL_OFFSET;
    size_t pt0_phys = (size_t)&pt0 - KERNEL_OFFSET;
    size_t pt1_phys = (size_t)&pt1 - KERNEL_OFFSET;

    // Set up higher level paging structures.
    memset((void*)pml40, 0, PAGE_SIZE);
    memset((void*)pdpt0, 0, PAGE_SIZE);
    memset((void*)pd0, 0, PAGE_SIZE);
    memset((void*)pt0, 0, PAGE_SIZE);
    memset((void*)pt1, 0, PAGE_SIZE);
    pml40[KERNEL_PML4].present = 1;
    pml40[KERNEL_PML4].write_enabled = 1;
    pml40[KERNEL_PML4].dir_ptr_addr_high = (pdpt0_phys >> 32) & 0xFFFFF;
    pml40[KERNEL_PML4].dir_ptr_addr_low = (pdpt0_phys >> 12) & 0xFFFFF;
    pdpt0[KERNEL_PDPT].present = 1;
    pdpt0[KERNEL_PDPT].write_enabled = 1;
    pdpt0[KERNEL_PDPT].dir_addr_high = (pd0_phys >> 32) & 0xFFFFF;
    pdpt0[KERNEL_PDPT].dir_addr_low = (pd0_phys >> 12) & 0xFFFFF;
    pd0[0].present = 1;
    pd0[0].write_enabled = 1;
    pd0[0].table_addr_high = (pt0_phys >> 32) & 0xFFFFF;
    pd0[0].table_addr_low = (pt0_phys >> 12) & 0xFFFFF;
    pd0[1].present = 1;
    pd0[1].write_enabled = 1;
    pd0[1].table_addr_high = (pt1_phys >> 32) & 0xFFFFF;
    pd0[1].table_addr_low = (pt1_phys >> 12) & 0xFFFFF;

    // TEST
    pml40[KERNEL_PML4].user = 1;
    pdpt0[KERNEL_PDPT].user = 1;
    pd0[0].user = 1;
    pd0[1].user = 1;

    // Map kernel pages (from 1 MiB to 4 MiB).
    // This will be remapped anyway after loading the new PML4.
    for (size_t i = (0x100000 / PAGE_SIZE); i < 512; i++)
    {
        size_t page_addr = i * PAGE_SIZE;
        pt0[i].present = 1;
        pt0[i].write_enabled = 1;
        pt0[i].page_addr_high = (page_addr >> 32) & 0xFFFFF;
        pt0[i].page_addr_low = (page_addr >> 12) & 0xFFFFF;

        // TEST
        pt0[i].user = 1;
    }
    for (size_t i = 0; i < 512; i++)
    {
        size_t page_addr = i * PAGE_SIZE + 0x200000;
        pt1[i].present = 1;
        pt1[i].write_enabled = 1;
        pt1[i].page_addr_high = (page_addr >> 32) & 0xFFFFF;
        pt1[i].page_addr_low = (page_addr >> 12) & 0xFFFFF;

        // TEST
        pt1[i].user = 1;
    }

    // Set up recursive mapping.
    pml40[RECURSIVE_INDEX].present = 1;
    pml40[RECURSIVE_INDEX].write_enabled = 1;
    pml40[RECURSIVE_INDEX].dir_ptr_addr_high = (pml4_phys >> 32) & 0xFFFFF;
    pml40[RECURSIVE_INDEX].dir_ptr_addr_low = (pml4_phys >> 12) & 0xFFFFF;

    // TEST
    pml40[RECURSIVE_INDEX].user = 1;

    // Reload PML4.
    vmm_flush();

    // Re-initialize physical memory manager.
    pmm_bitmap = pmm_bitmap + KERNEL_OFFSET;

    // Identity map lowest 1 MiB, except the first page.
    for (size_t addr = PAGE_SIZE; addr < 0x100000; addr += PAGE_SIZE)
    {
        //vmm_page_map((void*)addr, (void*)addr, PG_PR | PG_RW);
        vmm_page_map((void*)addr, (void*)addr, PG_PR | PG_RW | PG_U);
    }

    // Remap kernel code and rodata.
    for (size_t addr = (size_t)&kernel_ro_start; addr < (size_t)&kernel_ro_end; addr += PAGE_SIZE)
    {
        //vmm_page_map((void*)addr, (void*)(addr+KERNEL_OFFSET), PG_PR);
        vmm_page_map((void*)addr, (void*)(addr+KERNEL_OFFSET), PG_PR | PG_U);
    }
    // Remap kernel data and bss.
    for (size_t addr = (size_t)&kernel_ro_end; addr < (size_t)&phys_end; addr += PAGE_SIZE)
    {
        //vmm_page_map((void*)addr, (void*)(addr+KERNEL_OFFSET), PG_PR | PG_RW);
        vmm_page_map((void*)addr, (void*)(addr+KERNEL_OFFSET), PG_PR | PG_RW | PG_U);
    }

    // Set up the state of the virtual memory tree.
    // First, we define a region that is free for kernel allocations
    // so that we can use malloc immediately afterwards to define
    // additional regions.
    Vmm_Region init_mem;
    init_mem.base = (void*)&kernel_end;
    init_mem.pages = (MEM_MAX - (size_t)&kernel_end) / PAGE_SIZE;
    vmm_tree_kernel_free = &_vmm_tree_kernel_free;
    vmm_tree_kernel_free->mem = init_mem;
    vmm_tree_kernel_free->height = 1;
    vmm_tree_kernel_free->l = NULL;
    vmm_tree_kernel_free->r = NULL;
    // Now we can allocate the new node on the heap.
    vmm_tree_kernel_free = malloc(sizeof(Vmm_Node*));
    *vmm_tree_kernel_free = _vmm_tree_kernel_free;

    void* a = vmm_page_alloc_kernel();
    void* b = vmm_page_alloc_kernel();
    vmm_page_free_kernel(a);
    vmm_page_free_kernel(b);
}

void* vmm_phys_addr(void* virt)
{
    size_t virt_addr = (size_t)virt;
    Pml4e *pml4;
    Pdpte *pdpt;
    Pde *pd;
    Pte *pt;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);
    uint16_t offset = PT_OFFSET(virt_addr);

    // Check for valid PML4 entry.
    pml4 = vmm_pml4();
    if (pml4[pml4_i].present == 0)
    {
        return (NULL);
    }

    // Check for valid PDPT entry.
    pdpt = vmm_pdpt(pml4_i);
    if (pdpt[pdpt_i].present == 0)
    {
        return (NULL);
    }

    // Check for valid PD entry.
    pd = vmm_pd(pml4_i, pdpt_i);
    if (pd[pd_i].present == 0)
    {
        return (NULL);
    }

    // Check for valid PT entry.
    pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    if (pt[pt_i].present == 0)
    {
        return (NULL);
    }

    // Find physical address of this page, and add offset.
    size_t phys_addr = ((size_t)pt[pt_i].page_addr_high) << 32;
    phys_addr |= ((size_t)pt[pt_i].page_addr_low) << 12;
    phys_addr |= offset;

    return ((void*) phys_addr);
}

void vmm_page_map(void* phys, void* virt, uint16_t flags)
{
    size_t phys_addr = (size_t)phys;
    size_t virt_addr = (size_t)virt;
    size_t tmp_addr;
    Pml4e *pml4;
    Pdpte *pdpt;
    Pde *pd;
    Pte *pt;

    // TEST.
    // Security risk.
    flags |= PG_U;

    // Clear lowest 12 bits of both addresses.
    phys_addr &= ~0xFFF;
    virt_addr &= ~0xFFF;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);

    // Check PML4 entry.
    pml4 = vmm_pml4();
    if (pml4[pml4_i].present == 0)
    {
        Pml4e tmp_pml4e = {0};

        // Set relevant flags.
        tmp_pml4e.present = 1;
        tmp_pml4e.write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            tmp_pml4e.user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            tmp_pml4e.write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            tmp_pml4e.cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            tmp_pml4e.accessed = 1;
        pml4[pml4_i] = tmp_pml4e;

        // Make new PDPT.
        tmp_addr = (size_t) pmm_frame_alloc();
        pml4[pml4_i].dir_ptr_addr_high = (tmp_addr >> 32) & 0xFFFF;
        pml4[pml4_i].dir_ptr_addr_low = (tmp_addr >> 12) & 0xFFFF;
        vmm_flush();
        tmp_addr = (size_t) vmm_pdpt(pml4_i);
        memset((void*)tmp_addr, 0, PAGE_SIZE);
    }

    // Check PDPT entry.
    pdpt = vmm_pdpt(pml4_i);
    if (pdpt[pdpt_i].present == 0)
    {
        Pdpte tmp_pdpte = {0};

        // Set relevant flags.
        tmp_pdpte.present = 1;
        tmp_pdpte.write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            tmp_pdpte.user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            tmp_pdpte.write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            tmp_pdpte.cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            tmp_pdpte.accessed = 1;
        pdpt[pdpt_i] = tmp_pdpte;

        // Make new PD.
        tmp_addr = (size_t) pmm_frame_alloc();
        pdpt[pdpt_i].dir_addr_high = (tmp_addr >> 32) & 0xFFFF;
        pdpt[pdpt_i].dir_addr_low = (tmp_addr >> 12) & 0xFFFF;
        vmm_flush();
        tmp_addr = (size_t) vmm_pd(pml4_i, pdpt_i);
        memset((void*)tmp_addr, 0, PAGE_SIZE);
    }

    // Check PD entry.
    pd = vmm_pd(pml4_i, pdpt_i);
    if (pd[pd_i].present == 0)
    {
        Pde tmp_pde = {0};

        // Set relevant flags.
        tmp_pde.present = 1;
        tmp_pde.write_enabled = 1;
        if (BIT_CHECK(flags, PG_U_BIT))
            tmp_pde.user = 1;
        if (BIT_CHECK(flags, PG_WT_BIT))
            tmp_pde.write_through = 1;
        if (BIT_CHECK(flags, PG_CD_BIT))
            tmp_pde.cache_disabled = 1;
        if (BIT_CHECK(flags, PG_AC_BIT))
            tmp_pde.accessed = 1;
        pd[pd_i] = tmp_pde;

        // Make new PT.
        tmp_addr = (size_t) pmm_frame_alloc();
        pd[pd_i].table_addr_high = (tmp_addr >> 32) & 0xFFFFF;
        pd[pd_i].table_addr_low = (tmp_addr >> 12) & 0xFFFFF;
        vmm_flush();
        tmp_addr = (size_t) vmm_pt(pml4_i, pdpt_i, pd_i);
        memset((void*)tmp_addr, 0, PAGE_SIZE);
    }

    // Make PT entry.
    pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    Pte tmp_pte = {0};
    if (BIT_CHECK(flags, PG_PR_BIT))
        tmp_pte.present = 1;
    if (BIT_CHECK(flags, PG_RW_BIT))
        tmp_pte.write_enabled = 1;
    if (BIT_CHECK(flags, PG_U_BIT))
        tmp_pte.user = 1;
    if (BIT_CHECK(flags, PG_WT_BIT))
        tmp_pte.write_through = 1;
    if (BIT_CHECK(flags, PG_CD_BIT))
        tmp_pte.cache_disabled = 1;
    if (BIT_CHECK(flags, PG_AC_BIT))
        tmp_pte.accessed = 1;
    if (BIT_CHECK(flags, PG_DT_BIT))
        tmp_pte.dirty = 1;
    if (BIT_CHECK(flags, PG_AT_BIT))
        tmp_pte.attr = 1;
    if (BIT_CHECK(flags, PG_GL_BIT))
        tmp_pte.global = 1;
    tmp_pte.page_addr_high = (phys_addr >> 32) & 0xFFFFF;
    tmp_pte.page_addr_low = (phys_addr >> 12) & 0xFFFFF;
    pt[pt_i] = tmp_pte;
    vmm_flush();
}

void vmm_page_unmap(void* virt)
{
    Pte *pt;
    size_t virt_addr = (size_t)virt;

    // Clear lowest 12 bits.
    virt_addr &= ~0xFFF;

    // Calculate table entries.
    uint16_t pml4_i = PML4_INDEX(virt_addr);
    uint16_t pdpt_i = PDPT_INDEX(virt_addr);
    uint16_t pd_i = PD_INDEX(virt_addr);
    uint16_t pt_i = PT_INDEX(virt_addr);

    // Mark the page as not present.
    pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    pt[pt_i].present = 0;

    // Probably unneccessary.
    vmm_flush();
}

void vmm_table_flags(void* entry, uint16_t flags)
{
    size_t phys_addr = (size_t) vmm_phys_addr(entry);

    vmm_page_map((void*)phys_addr, entry, flags);

    //vmm_flush();
}

void* vmm_page_alloc_kernel(void)
{
    return (vmm_pages_alloc_kernel(1));
}

void* vmm_pages_alloc_kernel(size_t n)
{
    // Check for invalid input.
    if (n == 0)
    {
        return (NULL);
    }

    Vmm_Region mem = vmm_tree_find_pages(vmm_tree_kernel_free, n);

    // Check if a region was actually found.
    if (mem.pages > 0)
    {
        void* virt_base;

        // Return any memory to the pool that we aren't using.
        if (mem.pages > n)
        {
            mem.pages -= n;
            virt_base = (void*)(size_t)mem.base + PAGE_SIZE * mem.pages;
            vmm_tree_resize(vmm_tree_kernel_free, mem);
        }
        else
        {
            // Delete the memory region from the tree.
            virt_base = mem.base;
            vmm_tree_kernel_free = vmm_tree_delete(vmm_tree_kernel_free, mem);
        }

        // Map the region.
        void* phys;
        size_t virt = (size_t) virt_base;
        for (size_t i = 0; i < n; i++)
        {
            phys = pmm_frame_alloc();
            //vmm_page_map(phys, (void*)virt, PG_PR | PG_RW);

            // TEST:
            // USER FLAG SAFETY RISK.
            vmm_page_map(phys, (void*)virt, PG_PR | PG_RW | PG_U);
            virt += PAGE_SIZE;
        }
        return (virt_base);
    }

    // Else.
    return (NULL);
}

void vmm_page_free_kernel(void* virt)
{
    vmm_pages_free_kernel(virt, 1);
}

void vmm_pages_free_kernel(void* virt, size_t n)
{
    // Modify trees.
    Vmm_Region region;
    region.base = virt;
    region.pages = n;
    vmm_tree_kernel_free = vmm_tree_insert(vmm_tree_kernel_free, region);

    // Modify paging tables.
    for (size_t i = 0; i < n; i++)
    {
        vmm_page_unmap(virt);
        pmm_frame_free(vmm_phys_addr(virt));
        virt = (void*)(size_t)virt + PAGE_SIZE;
    }
}

int vmm_tree_height(Vmm_Node* node)
{
    if (node == NULL)
    {
        return (0);
    }
    // Else.
    return (node->height);
}

void vmm_tree_update_height(Vmm_Node* node)
{
    int lh = vmm_tree_height(node->l);
    int rh = vmm_tree_height(node->r);
    node->height = 1 + MAX(lh, rh);
}

Vmm_Node* vmm_tree_rotate_left(Vmm_Node* root)
{
    Vmm_Node* new_root = root->r;

    // Rotate.
    root->r = new_root->l;
    new_root->l = root;

    vmm_tree_update_height(root);
    vmm_tree_update_height(new_root);

    return (new_root);
}

Vmm_Node* vmm_tree_rotate_right(Vmm_Node* root)
{
    Vmm_Node* new_root = root->l;

    // Rotate.
    root->l = new_root->r;
    new_root->r = root;

    vmm_tree_update_height(root);
    vmm_tree_update_height(new_root);

    return (new_root);
}

Vmm_Node* vmm_tree_balance(Vmm_Node* node)
{
    int balance = vmm_tree_height(node->l);
    balance -= vmm_tree_height(node->r);

    if (balance > 1)
    {
        // Left-left.
        if (node->mem.base < node->l->mem.base)
        {
            return (vmm_tree_rotate_right(node));
        }

        // Else, left-right.
        node->l = vmm_tree_rotate_left(node->l);
        return (vmm_tree_rotate_right(node));
    }

    if (balance < -1)
    {
        // Right-right.
        if (node->mem.base > node->r->mem.base)
        {
            return (vmm_tree_rotate_left(node));
        }

        // Else, right-left.
        node->r = vmm_tree_rotate_right(node->r);
        return (vmm_tree_rotate_left(node));
    }

    // Else.
    return (node);
}

Vmm_Node* vmm_tree_insert(Vmm_Node* root, Vmm_Region mem)
{
    if (root == NULL)
    {
        Vmm_Node* new_node = (Vmm_Node*) malloc(sizeof(Vmm_Node));

        new_node->mem = mem;
        new_node->height = 1;
        new_node->l = NULL;
        new_node->r = NULL;

        return (new_node);
    }

    if (mem.base < root->mem.base)
    {
        root->l = vmm_tree_insert(root->l, mem);
    }
    else if (mem.base > root->mem.base)
    {
        root->r = vmm_tree_insert(root->r, mem);
    }
    else
    {
        kernel_panic("VMM tree insert failed.");
    }

    // TODO: Figure out how to merge with left node correctly.

    // Check if we should merge with the right node.
    if (root->r)
    {
        void* end = (void*)(size_t)mem.base + mem.pages * PAGE_SIZE;
        if (end == root->r->mem.base)
        {
            mem.base = root->r->mem.base;
            mem.pages += root->r->mem.pages;
            root->r = vmm_tree_delete(root->r, root->r->mem);
            vmm_tree_resize(root, mem);
        }
    }

    vmm_tree_update_height(root);
    root = vmm_tree_balance(root);

    return (root);
}

Vmm_Node* vmm_tree_delete(Vmm_Node* root, Vmm_Region mem)
{
    if (root == NULL)
    {
        return (root);
    }

    if (mem.base < root->mem.base)
    {
        root->r = vmm_tree_delete(root->r, mem);
    }
    else if (mem.base > root->mem.base)
    {
        root->l = vmm_tree_delete(root->l, mem);
    }
    else
    {
        // If node has two children.
        if (root->l && root->r)
        {
            // Find least significant node in right subtree.
            Vmm_Node* least = root->r;
            while (least->l != NULL)
            {
                least = least->l;
            }

            // Copy this node.
            root->mem = least->mem;

            // Delete this node.
            root->r = vmm_tree_delete(root->r, least->mem);
        }
        // If node has no children.
        else if (!root->l && !root->r)
        {
            free(root);
            return (NULL);
        }
        // Node has one child.
        else
        {
            // Move the child.
            if (root->l)
            {
                root = root->l;
                free(root->l);
            }
            else
            {
                root = root->r;
                free(root->r);
            }
        }
    }

    if (root == NULL)
    {
        return (NULL);
    }

    // Update height.
    vmm_tree_update_height(root);

    // Balance node.
    vmm_tree_balance(root);

    return (root);
}

Vmm_Region vmm_tree_find_pages(Vmm_Node* root, size_t pages)
{
    Vmm_Region ret;
    ret.base = NULL;

    if (root == NULL)
    {
        // Not in this branch.
        ret.pages = 0;
        return (ret);
    }

    // Found a sufficient region.
    if (root->mem.pages >= pages)
    {
        return (root->mem);
    }

    // Else, check left branch.
    ret = vmm_tree_find_pages(root->l, pages);
    if (ret.pages > 0)
    {
        return (ret);
    }

    // Else, check right branch.
    ret = vmm_tree_find_pages(root->r, pages);
    if (ret.pages > 0)
    {
        return (ret);
    }

    // Else, not in this branch.
    return (ret);
}

Vmm_Node* vmm_tree_resize(Vmm_Node* root, Vmm_Region mem)
{
    Vmm_Node* ret;

    if (root == NULL)
    {
        return (NULL);
    }

    // Check if this is the region.
    if (root->mem.base == mem.base)
    {
        root->mem = mem;
        return (root);
    }

    // Check left branch.
    if (mem.base < root->mem.base)
    {
        ret = vmm_tree_resize(root->l, mem);

        if (ret)
        {
            ret->mem = mem;
            return (ret);
        }
    }

    // Else, check right branch.
    ret = vmm_tree_resize(root->r, mem);
    if (ret)
    {
        ret->mem = mem;
        return (ret);
    }

    // Else, region not found.
    return (NULL);
}
