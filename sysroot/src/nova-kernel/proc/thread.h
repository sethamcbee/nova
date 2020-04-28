/**
 * @file thread.h
 * @author Seth McBee
 * @date 2020-4-28
 * @brief Kernel and userspace threads.
 */

#pragma once

#include <globals.h>

#ifdef ARCH_X86
#include <arch/x86/thread_state.h>
#endif

#ifdef ARCH_X86_64
#include <arch/x86_64/thread_state.h>
#endif

struct Thread
{
    // Architecture-specific.
    Thread_State state;

    // Whether this is a kernel thread or a userspace thread.
    bool privileged;

    // Unique id for this thread.
    size_t id;
};

extern Thread* volatile current_thread;
extern Thread* null_thread;
extern Thread* kernel_thread;

// Architecture-specific.
extern "C" void thread_switch(Thread* target);
Thread* thread_spawn(void* entry);
