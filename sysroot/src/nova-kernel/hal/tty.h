// Authors: Seth McBee
// Created: 2018-5-3
// Description: Generic terminal emulator driver.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

void tty_init(void);

// stdio interface.
extern FILE* tty_outs;
extern FILE* tty_ins;

#ifdef __cplusplus
}
#endif
