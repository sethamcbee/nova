/* Declare output format and architecture. */
OUTPUT_FORMAT(elf64-x86-64)
/* OUTPUT_FORMAT(elf32-i386) */
OUTPUT_ARCH(i386:x86-64)

/* Declare entry function. */
ENTRY(_start)

SECTIONS
{
	/* Start loading sections at 1MiB. */
	. = 1M;
	_sections_start = .;

	/* Multiboot header must be first so it can be found by the
	 * bootloader. */
	_section_text_start = .;
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
	_section_data_end = .;

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	_section_bss_end = .;

	_sections_end = .;
}
