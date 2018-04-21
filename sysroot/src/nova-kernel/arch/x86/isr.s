# Authors: Seth McBee
# Created: 2017-10-17
# Description: Interrupt Service Routines.

# Pushes registers.
.macro isr_push
	push %eax
	push %ebx
	push %ecx
	push %edx
	push %esi
	push %edi
	cld
.endm

# Pops registers.
.macro isr_pop
	pop %edi
	pop %esi
	pop %edx
	pop %ecx
	pop %ebx
	pop %eax
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

.text

#
.global isr_0
isr_0:
	isr_push

	movl $panic_0, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_1
isr_1:
	isr_push

	movl $panic_1, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_2
isr_2:
	isr_push

	movl $panic_2, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_3
isr_3:
	isr_push

	movl $panic_3, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_4
isr_4:
	isr_push

	movl $panic_4, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_5
isr_5:
	isr_push

	movl $panic_5, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_6
isr_6:
	isr_push

	movl $panic_6, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_7
isr_7:
	isr_push

	movl $panic_7, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_8
isr_8:
	isr_push

	movl $panic_8, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_9
isr_9:
	isr_push

	movl $panic_9, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_10
isr_10:
	isr_push

	movl $panic_10, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_11
isr_11:
	isr_push

	movl $panic_11, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_12
isr_12:
	isr_push

	movl $panic_12, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_13
isr_13:
	isr_push

	movl $panic_13, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_14
isr_14:
	isr_push

	movl $panic_14, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_15
isr_15:
	isr_push

	movl $panic_15, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_16
isr_16:
	isr_push

	movl $panic_16, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_17
isr_17:
	isr_push

	movl $panic_17, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_18
isr_18:
	isr_push

	movl $panic_18, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_19
isr_19:
	isr_push

	movl $panic_19, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_20
isr_20:
	isr_push

	movl $panic_20, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_21
isr_21:
	isr_push

	movl $panic_21, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_22
isr_22:
	isr_push

	movl $panic_22, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_23
isr_23:
	isr_push

	movl $panic_23, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_24
isr_24:
	isr_push

	movl $panic_24, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_25
isr_25:
	isr_push

	movl $panic_25, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_26
isr_26:
	isr_push

	movl $panic_26, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_27
isr_27:
	isr_push

	movl $panic_27, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_28
isr_28:
	isr_push

	movl $panic_28, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_29
isr_29:
	isr_push

	movl $panic_29, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_30
isr_30:
	isr_push

	movl $panic_30, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_31
isr_31:
	isr_push

	movl $panic_31, %eax
	push %eax
	call kernel_panic

	isr_pop
	iret

.global isr_32
isr_32:
	isr_push

	call isr_32_ext

	isr_pop
	iret

.global isr_33
isr_33:
	isr_push

	call isr_33_ext

	isr_pop
	iret

.global isr_34
isr_34:
	isr_push

	movl $panic_34, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_35
isr_35:
	isr_push

	movl $panic_35, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_36
isr_36:
	isr_push

	movl $panic_36, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_37
isr_37:
	isr_push

	movl $panic_37, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_38
isr_38:
	isr_push

	movl $panic_38, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_39
isr_39:
	isr_push

	# Check for spurious IRQ.
	call isr_39_ext

	movl $panic_39, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_40
isr_40:
	isr_push

	movl $panic_40, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_41
isr_41:
	isr_push

	movl $panic_41, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_42
isr_42:
	isr_push

	movl $panic_42, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_43
isr_43:
	isr_push

	movl $panic_43, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_44
isr_44:
	isr_push

	movl $panic_44, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_45
isr_45:
	isr_push

	movl $panic_45, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_46
isr_46:
	isr_push

	movl $panic_46, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_47
isr_47:
	isr_push

	movl $panic_47, %eax
	push %eax
	call kernel_log

	isr_pop
	iret

.global isr_80
isr_80:
	isr_push

	isr_pop
	iret
