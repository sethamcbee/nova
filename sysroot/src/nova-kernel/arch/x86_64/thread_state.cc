/**
 * @file thread_state.cc
 * @authors Seth McBee
 * @date 2020-4-28
 */

#include <globals.h>

#include <proc/thread.h>

#include <arch/x86_64/memory/vmm.h>
#include <arch/x86_64/thread_state.h>

// Default to 2MiB stack size.
#define STACK_PAGES 512

Thread* thread_spawn(void* entry)
{
    Thread* thread = new Thread;
    Thread_State* state = &thread->state;
    state->rsp = (size_t)vmm_pages_alloc_kernel(STACK_PAGES) + PAGE_SIZE*STACK_PAGES;

    asm volatile
    (
        // Save stack pointer.
        "movq %%rsp, %%rax \n"

        // Load new stack pointer.
        "movq %1, %%rsp \n"

        // Push entry address and dummy values.
        "pushq %2 \n"
        "subq $48, %%rsp \n"

        // Save new stack address.
        "movq %%rsp, %0 \n"

        // Restore stack pointer.
        "movq %%rax, %%rsp \n"
        : "=r" (state->rsp)
        : "r" (state->rsp), "r" (entry)
        : "rsp", "rax", "memory"
    );

    return thread;
}
