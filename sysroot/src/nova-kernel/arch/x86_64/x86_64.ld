/* Declare output format and architecture. */
OUTPUT_FORMAT(elf64-x86-64)
/*OUTPUT_ARCH(i386:x86-64)*/

/* Declare entry function. */
ENTRY(start)

BOOT_OFFSET = 0x100000;
KERNEL_OFFSET = 0xffffffff80000000;

SECTIONS
{
	. = BOOT_OFFSET;
	phys_start = .;

	.init :
	{
		KEEP(*(.init))
	}

	. += KERNEL_OFFSET;

	.text : AT(ADDR(.text) - KERNEL_OFFSET)
	{
		. = ALIGN(0x1000);
		*(.text)
	}

	.rodata : AT(ADDR(.rodata) - KERNEL_OFFSET)
	{
		. = ALIGN(0x1000);
		*(.rodata)
	}

	.data : AT(ADDR(.data) - KERNEL_OFFSET)
	{
		. = ALIGN(0x1000);
		*(.data)
	}

	.bss : AT(ADDR(.bss) - KERNEL_OFFSET)
	{
		. = ALIGN(0x1000);
		*(COMMON)
		*(.bss)
	}

	phys_end = . - KERNEL_OFFSET;
}
