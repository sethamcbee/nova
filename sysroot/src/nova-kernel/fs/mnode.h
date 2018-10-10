/**
 * @file mnode.h
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS mount point node.
 */

#pragma once

typedef struct FS_Driver FS_Driver;
typedef struct Mnode Mnode;

struct Mnode
{
	/// File system driver.
	FS_Driver* fs;
	
	/// File system driver data.
	void* fs_data;
	
	/// Mount point directory path.
	char* path;
};
