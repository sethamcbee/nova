/**
 * @file vfs.h
 * @author Seth McBee
 * @date 2018-10-7
 * @brief Virtual file system abstraction.
 */

#pragma once

#include <globals.h>

typedef enum Dnode_Type Dnode_Type;
typedef enum Vnode_Type Vnode_Type;
typedef struct Dnode Dnode;
typedef struct FS_Driver FS_Driver;
typedef struct Mnode Mnode;
typedef struct Vnode Vnode;

/// Root directory.
Dnode* vfs_root;

/// Initialize VFS.
void vfs_init(void);

/// Mount a file system.
int vfs_mount(const char* path, FS_Driver* fs);

/// Create a file.
int vfs_create_vnode(const char* path, Vnode_Type type);

/// Create a directory.
int vfs_create_dnode(const char* path, Dnode_Type type);

/// Remove a file or directory.
int vfs_remove(const char* path);

/// Write to a file.
ssize_t vfs_write(const char* path, const void* buf, size_t n);

/// Read from a file.
ssize_t vfs_read(const char* path, void* buf, size_t n);
