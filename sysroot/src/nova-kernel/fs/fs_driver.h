/**
 * @file fs_driver.h
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS driver for derived file systems.
 */

#pragma once

#include <globals.h>

typedef enum Dnode_Type Dnode_Type;
typedef enum Vnode_Type Vnode_Type;
typedef struct Dnode Dnode;
typedef struct FS_Driver FS_Driver;
typedef struct Vnode Vnode;

/// Stores information for calling the file system driver.
struct FS_Driver
{
	int (*mount)(Dnode* mountpoint);
	int (*create_vnode)(Vnode* file, Vnode_Type type);
	int (*create_dnode)(Dnode* dir, Dnode_Type type);
	int (*remove_vnode)(Vnode* file);
	int (*remove_dnode)(Dnode* dir);
	ssize_t (*read)(Vnode* file, void* buf, size_t n, size_t pos);
	ssize_t (*write)(Vnode* file, const void* buf, size_t n, size_t pos);
};
