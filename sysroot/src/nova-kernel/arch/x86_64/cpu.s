# Authors: Seth McBee
# Created: 2018-9-14
# Description: Implements miscellaneous x86-64-specific functions.

.text

# Proc struct* stored in rdi
.global cpu_proc_asm
cpu_proc_asm:
    # Load registers with stored values.
    # rbx and rdi will be handled later.
    movq (%rdi), %rax
    # Skip rbx.
    movq 16(%rdi), %rcx
    movq 24(%rdi), %rdx
    movq 32(%rdi), %rbp
    # rsp handled by iret.
    movq 48(%rdi), %rsi
    # Skip rdi.
    movq 64(%rdi), %r8
    movq 72(%rdi), %r9
    movq 80(%rdi), %r10
    movq 88(%rdi), %r11
    movq 96(%rdi), %r12
    movq 104(%rdi), %r13
    movq 112(%rdi), %r14
    movq 120(%rdi), %r15
    # rip handled by iret.
    # flags handled by iret.

    # Handle data selector.
    movq 144(%rdi), %rbx
    movw %bx, %ds
    movw %bx, %es
    movw %bx, %fs
    movw %bx, %gs
    pushq %rbx

    # Handle stack pointer.
    movq 40(%rdi), %rbx
    pushq %rbx

    # Flags.
    movq 136(%rdi), %rbx
    pushq %rbx

    # sti upon context switch.
    #orq $0x200, (%rsp)

    # Handle code selector.
    movq 152(%rdi), %rbx
    pushq %rbx

    # rip to jump to.
    movq 128(%rdi), %rbx
    pushq %rbx

    # Load rbx and rdi with stored values.
    movq 8(%rdi), %rbx
    movq 56(%rdi), %rdi

    # Jump.
    iretq
