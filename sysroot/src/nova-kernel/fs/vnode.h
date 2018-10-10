/**
 * @file vnode.h
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS file node.
 */

#pragma once

typedef struct FS_Driver FS_Driver;
typedef struct Vnode Vnode;

/// Type of file.
typedef enum Vnode_Type
{
	VNODE_FILE,
	VNODE_SOFTLINK,
	VNODE_FIFO,
	VNODE_SPECIAL
} Vnode_Type;

/// VFS file node.
struct Vnode
{
	/// File system driver.
	FS_Driver* fs;
	
	/// File system driver data.
	void* fs_data;
	
	/// File name.
	char* name;
	
	/// Type of file node.
	Vnode_Type type;
};
