# Authors: Seth McBee
# Created: 2017-10-17
# Description: Interrupt Service Routines.

# Pushes registers.
.macro isr_push
	push %rax
	push %rcx
	push %rdx
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
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
	pop %rax
.endm

# Error strings.
.data
panic_0:
	.string "EXCEPTION: DIVIDE-BY-ZERO ERROR"
panic_1:
	.string "EXCEPTION: DEBUG"
panic_2:
	.string "EXCEPTION: NON-MASKABLE_INTERRUPT"
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

.code64
.text

#
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
	isr_push

	movq $panic_13, %rdi
	call kernel_panic

	isr_pop
	iretq

.global isr_14
isr_14:
	isr_push

	movq $panic_14, %rdi
	call kernel_panic

	isr_pop
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

.global isr_80
isr_80:
	isr_push

	isr_pop
	iretq
