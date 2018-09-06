# Authors: Seth McBee
# Created: 2017-10-17
# Description: x86-64 Interrupt Service Routines.

# Pushes registers.
.macro isr_push
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	cld
.endm

# Pops registers.
.macro isr_pop
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdi
	pop %rsi
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax
.endm

.section .rodata

# Error strings.
panic_0:
	.string "EXCEPTION: DIVIDE-BY-ZERO ERROR"
panic_1:
	.string "EXCEPTION: DEBUG"
panic_2:
	.string "EXCEPTION: NON-MASKABLE INTERRUPT"
panic_3:
	.string "EXCEPTION: BREAKPOINT"
panic_4:
	.string "EXCEPTION: OVERFLOW"
panic_5:
	.string "EXCEPTION: BOUND RANGE EXCEEDED"
panic_6:
	.string "EXCEPTION: INVALID OPCODE"
panic_7:
	.string "EXCEPTION: DEVICE NOT AVAILABLE"
panic_8:
	.string "EXCEPTION: DOUBLE FAULT"
panic_9:
	.string "EXCEPTION: COPROCESSOR SEGMENT OVERRUN"
panic_10:
	.string "EXCEPTION: INVALID TSS"
panic_11:
	.string "EXCEPTION: SEGMENT NOT PRESENT"
panic_12:
	.string "EXCEPTION: STACK-SEGMENT FAULT"
panic_13:
	.string "EXCEPTION: GENERAL PROTECTION FAULT"
panic_14:
	.string "EXCEPTION: PAGE FAULT"
panic_15:
	.string "EXCEPTION: RESERVED EXCEPTION 15"
panic_16:
	.string "EXCEPTION: X87 FLOATING-POINT EXCEPTION"
panic_17:
	.string "EXCEPTION: ALIGNMENT CHECK"
panic_18:
	.string "EXCEPTION: MACHINE CHECK"
panic_19:
	.string "EXCEPTION: SIMD FLOATING-POINT EXCEPTION"
panic_20:
	.string "EXCEPTION: VIRTUALIZATION EXCEPTION"
panic_21:
	.string "EXCEPTION: RESERVED EXCEPTION 21"
panic_22:
	.string "EXCEPTION: RESERVED EXCEPTION 22"
panic_23:
	.string "EXCEPTION: RESERVED EXCEPTION 23"
panic_24:
	.string "EXCEPTION: RESERVED EXCEPTION 24"
panic_25:
	.string "EXCEPTION: RESERVED EXCEPTION 25"
panic_26:
	.string "EXCEPTION: RESERVED EXCEPTION 26"
panic_27:
	.string "EXCEPTION: RESERVED EXCEPTION 27"
panic_28:
	.string "EXCEPTION: RESERVED EXCEPTION 28"
panic_29:
	.string "EXCEPTION: RESERVED EXCEPTION 29"
panic_30:
	.string "EXCEPTION: SECURITY EXCEPTION"
panic_31:
	.string "EXCEPTION: RESERVED EXCEPTION 31"
panic_32:
	.string "\n\tIRQ: PIT\n"
panic_33:
	.string "\n\tIRQ: Keyboard\n"
panic_34:
	.string "\n\tIRQ: Cascade\n"
panic_35:
	.string "\n\tIRQ: COM2\n"
panic_36:
	.string "\n\tIRQ: COM1\n"
panic_37:
	.string "\n\tIRQ: LPT2\n"
panic_38:
	.string "\n\tIRQ: Floppy\n"
panic_39:
	.string "\n\tIRQ: LPT1\n"
panic_40:
	.string "\n\tIRQ: CMOS\n"
panic_41:
	.string "\n\tIRQ: #9\n"
panic_42:
	.string "\n\tIRQ: #10\n"
panic_43:
	.string "\n\tIRQ: #11\n"
panic_44:
	.string "\n\tIRQ: Mouse\n"
panic_45:
	.string "\n\tIRQ: FPU\n"
panic_46:
	.string "\n\tIRQ: ATA1\n"
panic_47:
	.string "\n\tIRQ: ATA2\n"
panic_128:
	.string "SYSCALL \n"

.text

# Interrupt Service Routines.
.global isr_0
isr_0:
	isr_push

	movq $panic_0, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_1
isr_1:
	isr_push

	movq $panic_1, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_2
isr_2:
	isr_push

	movq $panic_2, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_3
isr_3:
	isr_push

	movq $panic_3, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_4
isr_4:
	isr_push

	movq $panic_4, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_5
isr_5:
	isr_push

	movq $panic_5, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_6
isr_6:
	isr_push

	movq $panic_6, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_7
isr_7:
	isr_push

	movq $panic_7, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_8
isr_8:
	isr_push

	movq $panic_8, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_9
isr_9:
	isr_push

	movq $panic_9, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_10
isr_10:
	isr_push

	movq $panic_10, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_11
isr_11:
	isr_push

	movq $panic_11, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_12
isr_12:
	isr_push

	movq $panic_12, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_13
isr_13:
	# Get error code.
	movq (%rsp), %r12

	isr_push

	# Handle error code.
	movq %r12, %rdi
	call isr_13_ext

	movq $panic_13, %rdi
	call kernel_panic

	isr_pop

	# Remove error code from stack.
	add $4, %rsp

	iretq

.global isr_14
isr_14:
	# Get error code.
	movq (%rsp), %r12

	isr_push

	# Get error address.
	movq %cr2, %rsi

	# Handle error code.
	movq %r12, %rdi
	call isr_14_ext

	movq $panic_14, %rdi
	call kernel_panic

	isr_pop

	# Remove error code from stack.
	add $4, %rsp

	iretq

.global isr_15
isr_15:
	isr_push

	movq $panic_15, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_16
isr_16:
	isr_push

	movq $panic_16, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_17
isr_17:
	isr_push

	movq $panic_17, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_18
isr_18:
	isr_push

	movq $panic_18, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_19
isr_19:
	isr_push

	movq $panic_19, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_20
isr_20:
	isr_push

	movq $panic_20, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_21
isr_21:
	isr_push

	movq $panic_21, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_22
isr_22:
	isr_push

	movq $panic_22, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_23
isr_23:
	isr_push

	movq $panic_23, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_24
isr_24:
	isr_push

	movq $panic_24, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_25
isr_25:
	isr_push

	movq $panic_25, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_26
isr_26:
	isr_push

	movq $panic_26, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_27
isr_27:
	isr_push

	movq $panic_27, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_28
isr_28:
	isr_push

	movq $panic_28, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_29
isr_29:
	isr_push

	movq $panic_29, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_30
isr_30:
	isr_push

	movq $panic_30, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_31
isr_31:
	isr_push

	movq $panic_31, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_32
isr_32:
	isr_push

	call isr_32_ext

	isr_pop
	iretq

.global isr_33
isr_33:
	isr_push

	#movq $panic_33, %rdi
	#call kernel_log

	call isr_33_ext

	isr_pop
	iretq

.global isr_34
isr_34:
	isr_push

	movq $panic_34, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_35
isr_35:
	isr_push

	movq $panic_35, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_36
isr_36:
	isr_push

	movq $panic_36, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_37
isr_37:
	isr_push

	movq $panic_37, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_38
isr_38:
	isr_push

	movq $panic_38, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_39
isr_39:
	isr_push

	# Check for spurious IRQ.
	call isr_39_ext

	movq $panic_39, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_40
isr_40:
	isr_push

	movq $panic_40, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_41
isr_41:
	isr_push

	movq $panic_41, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_42
isr_42:
	isr_push

	movq $panic_42, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_43
isr_43:
	isr_push

	movq $panic_43, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_44
isr_44:
	isr_push

	movq $panic_44, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_45
isr_45:
	isr_push

	movq $panic_45, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_46
isr_46:
	isr_push

	movq $panic_46, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_47
isr_47:
	isr_push

	movq $panic_47, %rdi
	call kernel_log

	isr_pop
	iretq

.global isr_128
isr_128:
	isr_push

	movq $panic_128, %rdi
	call kernel_print

	isr_pop
	iretq
