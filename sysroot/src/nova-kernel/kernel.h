/**
 * @file kernel.h
 * @author Seth McBee
 * @date 2017-10-14
 * @brief Kernel entry point and miscellaneous kernel functions.
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <globals.h>

/**
 * @brief Kernel entry point. Should never return.
 */
void kernel_main(void);

/**
 * @brief Print string to terminal and halts all operations. Should
 * never return.
 * 
 * @param str Error message to be printed. Must not exceed 10000
 * bytes in length.
 */
void kernel_panic(char *str);

/**
 * @brief Points to the write function for an environment-specific
 * file or device.
 * 
 * @param s Data to be written (likely to be char's, but this is
 * not necessarily the case). Data must be contiguous in virtual
 * memory.
 * @param n Length of the data stream to be written.
 * 
 * @return Number of bytes written, or an error code.
 */

ssize_t (*kernel_write)(const void*, size_t);

/**
 * @brief Writes a string using kernel_write() without a specified
 * length.
 * 
 * @param s Data to be written. Must be null-terminated.
 * 
 * @return Number of bytes written, or an error code.
 */
int kernel_print(const char *s);

/**
 * @brief Writes a string to a file, rather than to stdout or stderr.
 * 
 * @param s Data to be written. Must be null-terminated.
 * 
 * @return Number of bytes written, or an error code.
 */
int kernel_log(const char *s);

/**
 * @brief Safely halts all operations. Does not return.
 */
void kernel_halt(void);

/**
 * @brief Kernel module loaded by the bootloader, or NULL if no
 * module was loaded.
 */
void* kernel_module;

#endif // KERNEL_H
