/**
 * @file vfs.h
 * @author Seth McBee
 * @date 2018-10-7
 * @brief Virtual file system abstraction.
 */

#pragma once

#include <globals.h>

#include <fs/dnode.h>
#include <fs/vnode.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Root directory.
Dnode* vfs_root;

/// Initialize VFS.
void vfs_init(void);

/// Mount a file system.
int vfs_mount(const char* device, const char* path);

/// Create a file.
int vfs_create_vnode(const char* path, Vnode_Type type);

/// Create a directory.
int vfs_create_dnode(const char* path, Dnode_Type type);

/// Remove a file or directory.
int vfs_remove(const char* path);

/// Read from a file.
ssize_t vfs_read(const char* path, void* buf, size_t n, size_t pos);

/// Write to a file.
ssize_t vfs_write(const char* path, const void* buf, size_t n, size_t pos);

/// Find file if it exists.
Vnode* vfs_find_vnode(const char* path);

/// Find directory if it exists.
Dnode* vfs_find_dnode(const char* path);

#ifdef __cplusplus
}
#endif
