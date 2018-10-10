/**
 * @file dnode.h
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS directory node.
 */
 
#pragma once

typedef struct Dnode Dnode;
typedef struct Vnode Vnode;

/// Type of directory.
typedef enum Dnode_Type
{
	DNODE_DIR,
	DNODE_MOUNT,
	DNODE_SOFTLINK,
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
	
	/// AVL tree of subdirectories.
	Dnode* dir;
	
	/// AVL tree of files.
	Vnode* files;
};
