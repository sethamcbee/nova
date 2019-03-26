/**
 * @file dnode.h
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS directory node.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Dnode Dnode;
typedef struct FS_Driver FS_Driver;
typedef struct Vnode Vnode;

/// Type of directory.
typedef enum Dnode_Type
{
    DNODE_DIR,
    DNODE_MOUNT,
    DNODE_SYMLINK,
    DNODE_SPECIAL
} Dnode_Type;

/// VFS directory node.
struct Dnode
{
    /// File system driver.
    FS_Driver* fs;

    /// File system driver data.
    void* fs_data;

    /// Name of this directory.
    char* name;

    /// Directory type.
    Dnode_Type type;

    /// Parent directory.
    Dnode* parent;

    /// AVL tree of subdirectories.
    Dnode* dir;

    /// AVL tree of files.
    Vnode* files;

    /// Height of node in AVL tree.
    int height;

    /// AVL tree children.
    Dnode* left;
    Dnode* right;
};

/// Insert a subdirectory.
Dnode* dnode_tree_insert(Dnode* root, const char* name);

/// Delete a subdirectory.
Dnode* dnode_tree_remove(Dnode* root, const char* name);

/// Find a subdirectory.
Dnode* dnode_tree_find(Dnode* root, const char* name);

#ifdef __cplusplus
}
#endif
