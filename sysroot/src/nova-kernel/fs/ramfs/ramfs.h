/**
 * @file ramfs.h
 * @author Seth McBee
 * @date 2018-10-8
 * @brief Memory-only file system.
 */

#pragma once

#include <globals.h>

typedef struct FS_Driver FS_Driver;
typedef struct Ramfs_Data Ramfs_Data;
typedef struct Vnode Vnode;

/// In-memory representation of file.
struct Ramfs_Data
{
	void* buf;
	
	size_t cur_size;
	
	size_t max_size;
};

/// Return driver interface for this file system.
void ramfs_driver(FS_Driver* dest);

void ramfs_create(Vnode* file);

ssize_t ramfs_read(Vnode* file, void* buf, size_t n, size_t pos);

ssize_t ramfs_write(Vnode* file, const void* buf, size_t n, size_t pos);
