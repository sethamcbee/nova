# Authors: Seth McBee
# Created: 2020-4-28

.data

.extern current_thread

.text

.global thread_switch
thread_switch:
    # Store all registers.
    # TODO: Handle FPU/MMX/SSE/AVX.
    pushq %rbx
    pushq %rbp
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    # Store rsp of thread we're switching from.
    movq current_thread, %rax
    movq %rsp, (%rax)

    # Get rsp of thread we're switching to.
    movq %rdi, current_thread
    movq (%rdi), %rsp

    # Restore state of the thread we're switching to.
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    popq %rbx
    ret
