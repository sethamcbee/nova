/**
 * @file ramfs.h
 * @author Seth McBee
 * @date 2018-10-8
 * @brief Memory-only file system.
 */

#pragma once

#include <globals.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Dnode_Type Dnode_Type;
typedef enum Vnode_Type Vnode_Type;
typedef struct Dnode Dnode;
typedef struct FS_Driver FS_Driver;
typedef struct Ramfs_Data Ramfs_Data;
typedef struct Vnode Vnode;

/// In-memory representation of file.
struct Ramfs_Data
{
    /// Data stored.
    void* buf;

    /// Currently used buffer size.
    size_t size;

    /// Currently allocated buffer size.
    size_t max_size;
};

/// Return driver interface for this file system.
FS_Driver* ramfs_driver(void);

int ramfs_mount(Dnode* mountpoint);

int ramfs_create_vnode(Vnode* file, Vnode_Type);

int ramfs_create_dnode(Dnode* dir, Dnode_Type);

ssize_t ramfs_read(Vnode* file, void* buf, size_t n, size_t pos);

ssize_t ramfs_write(Vnode* file, const void* buf, size_t n, size_t pos);

#ifdef __cplusplus
}
#endif
