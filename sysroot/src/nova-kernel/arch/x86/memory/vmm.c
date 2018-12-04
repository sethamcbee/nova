/**
 * @file vmm.c
 * @author Seth McBee
 * @date 2018-10-3
 * @brief Virtual memory management.
 */

#include <globals.h>

#include <stdlib.h>
#include <string.h>

#include <globals.h>
#include <arch/x86/memory/paging.h>
#include <arch/x86/memory/pmm.h>
#include <arch/x86/memory/vmm.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

Pde pd0[PAGE_COUNT] __attribute__((aligned(PAGE_SIZE)));
Pte pt0[PAGE_COUNT] __attribute__((aligned(PAGE_SIZE)));

// Maximum virtual address.
static const size_t MEM_MAX = (size_t)-1;

// Linker symbols.
extern void* pd_start;
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

static Pde* vmm_pd()
{
}

static Pte* vmm_pt(size_t pd_i)
{
}

static void vmm_flush(void)
{
}

void vmm_init(void)
{
}

void* vmm_phys_addr(void* virt)
{
}

void vmm_page_map(void* phys, void* virt, uint16_t flags)
{
}

void vmm_page_unmap(void* virt)
{
    Pte *pt;
    size_t virt_addr = (size_t)virt;

    // Clear lowest 12 bits.
    virt_addr &= ~0xFFF;

    // Calculate table entries.
    /// @warning STUB.

    // Mark the page as not present.
    //pt = vmm_pt(pml4_i, pdpt_i, pd_i);
    //pt[pt_i].present = 0;

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
        if (node->mem.base < node->r->mem.base)
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
            vmm_tree_delete(root->r, least->mem);
        }
        // If node has no children.
        else if (!root->l && !root->r)
        {
            free (root);
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
