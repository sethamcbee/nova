/* Declare output format and architecture. */
OUTPUT_FORMAT(elf64-x86-64)

/* Declare entry function. */
ENTRY(start)

BOOT_OFFSET = 0x100000;
KERNEL_OFFSET = 0xFFFFFFFF80000000;

SECTIONS
{
	. = BOOT_OFFSET;
	phys_start = .;

	init_start = .;
	. = ALIGN(0x1000);
	.init :
	{
		KEEP(*(.init))
	}
	init_end = .;

	. += KERNEL_OFFSET;

	kernel_start = .;
	kernel_ro_start = . - KERNEL_OFFSET;
	.text : AT(ADDR(.text) - KERNEL_OFFSET)
	{
		*(.text)
	}

	.rodata : AT(ADDR(.rodata) - KERNEL_OFFSET)
	{
		*(.rodata)
	}
	. = ALIGN(0x1000);
	kernel_ro_end = . - KERNEL_OFFSET;

	.data : AT(ADDR(.data) - KERNEL_OFFSET)
	{
		. = ALIGN(0x1000);
		*(.data)
	}

	.bss : AT(ADDR(.bss) - KERNEL_OFFSET)
	{
		*(COMMON)
		*(.bss)
	}
	. = ALIGN(0x1000);
	kernel_end = .;
	phys_end = . - KERNEL_OFFSET;
}
