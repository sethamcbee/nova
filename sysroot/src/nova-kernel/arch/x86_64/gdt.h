// Authors: Seth McBee
// Created: 2017-10-17
// Description: GDT constants and declarations.

#ifndef _GDT_H_
#define _GDT_H_

// If any entries are modifed, assembly files may need to be updated
// manually.
#define GDT_KERNEL_CODE 0x08
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE 0x18
#define GDT_USER_DATA 0x20

#endif // _GDT_H_
