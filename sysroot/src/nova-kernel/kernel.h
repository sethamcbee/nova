// Authors: Seth McBee
// Created: 2017-10-14
// Description: Main kernel.

#ifndef KERNEL_H
#define KERNEL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Main kernel. Should only be called once.
void kernel_main(void);

// Kernel panic. Prints panic message and then halts infinitely.
void kernel_panic(char *str, size_t len);

// Function pointer to the environment-specific terminal write().
int (*kernel_write)(const void *, size_t);

#endif // KERNEL_H
