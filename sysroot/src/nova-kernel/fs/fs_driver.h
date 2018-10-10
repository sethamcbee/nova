/**
 * @file fs_driver.h
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS driver for derived file systems.
 */

#pragma once

#include <globals.h>

typedef struct FS_Driver FS_Driver;
typedef struct Vnode Vnode;

/// Stores information for calling the file system driver.
struct FS_Driver
{
	int (*create_vnode)(Vnode* file);
	int (*create_dnode)(Dnode* file);
	int (*remove)(void* file);
	ssize_t (*read)(Vnode* file, void* buf, size_t n, size_t pos);
	ssize_t (*write)(Vnode* file, const void* buf, size_t n, size_t pos);
};
