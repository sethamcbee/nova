// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <globals.h>

// Main kernel. Should only be called once.
void kernel_main(void);

// Kernel panic. Prints panic message and then halts infinitely.
__attribute__((noreturn))
void kernel_panic(char *str);

// Function pointer to the environment-specific terminal write().
ssize_t (*kernel_write)(const void *, size_t);

// Finds the length of the string and then calls kernel_write.
int kernel_print(const char *s);

// Stores the string in the kernel log.
int kernel_log(const char *s);

// Safely stops all operation.
__attribute__((noreturn))
void kernel_halt(void);

#endif // KERNEL_H
