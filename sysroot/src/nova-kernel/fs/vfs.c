/**
 * @file vfs.c
 * @author Seth McBee
 * @date 2018-10-7
 * @brief Virtual file system abstraction.
 */

#include <globals.h>

#include <stdlib.h>

#include <fs/dnode.h>
#include <fs/fs_driver.h>
#include <fs/vfs.h>
#include <fs/vnode.h>
#include <fs/ramfs/ramfs.h>

void vfs_init(void)
{
	static bool initialized = false;
	
	if (initialized == false)
	{
		// Set up root directory.
		vfs_root = malloc(sizeof(Dnode));
		vfs_root->name = malloc(sizeof(char));
		*vfs_root->name = '\0';
		vfs_root->dir = NULL;
		vfs_root->files = NULL;
		vfs_root->parent = vfs_root;
		vfs_root->left = NULL;
		vfs_root->right = NULL;
		vfs_root->fs = ramfs_driver();
		ramfs_create_dnode(vfs_root, DNODE_MOUNT);
		
		initialized = true;
	}
	else
	{
		kernel_panic("Attempt to reinitialized VFS.");
	}
}

int vfs_mount(const char* device, const char* path)
{
	// Stub.
	return 0;
}

int vfs_create_vnode(const char* path, Vnode_Type type)
{
	
}

int vfs_create_dnode(const char* path, Dnode_Type type)
{
	
}

int vfs_remove(const char* path)
{
	
}

ssize_t vfs_read(const char* path, void* buf, size_t n, size_t pos)
{
	
}

ssize_t vfs_write(const char* path, const void* buf, size_t n, size_t pos)
{
	
}

Vnode* vfs_find_vnode(const char* path)
{
	
}

Dnode* vfs_find_dnode(const char* path)
{
	Dnode* dir = vfs_root->dir;
}
