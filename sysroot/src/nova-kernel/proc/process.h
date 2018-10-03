/**
 * @file process.h
 * @author Seth McBee
 * @date 2018-9-6
 * @brief Kernel and userspace process structs.
 */

#pragma once

#include <globals.h>

#ifdef ARCH_X86
#include <arch/x86/registers.h>
#endif // ARCH_X86

#ifdef ARCH_X86_64
#include <arch/x86_64/registers.h>
#endif // ARCH_X86_64

/**
 * @brief Stores process information.
 */
typedef struct
{	
	/**
	 * @brief Top of the kernel stack associated with this process.
	 */
    size_t kernel_stack;

	/**
	 * @brief Privilege level of this process.
	 * @note Kernel = 0, User = 3
	 */
    uint8_t priv;
    
    /**
     * @brief Stores architecture-specific register state for this
     * process.
     */
    Registers reg;
} Process;
