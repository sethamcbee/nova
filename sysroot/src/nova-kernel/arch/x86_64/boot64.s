

KERNEL_OFFSET = 0xffffffff80000000;

.data
.global pml4
pml4:
    .skip 0x1000

.text
.global boot64
boot64:

# Calls C boot procedure to do additional setup before the kernel is
# called.
    call boot_main

# Loops infinitely in case an error causes boot_main to return.
	cli
1:	hlt
	jmp 1b
