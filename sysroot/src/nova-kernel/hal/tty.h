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

void tty_init(void);

// stdio interface.
FILE *tty_outs;
FILE *tty_ins;
