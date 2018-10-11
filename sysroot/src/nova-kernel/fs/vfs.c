/**
 * @file vfs.c
 * @author Seth McBee
 * @date 2018-10-7
 * @brief Virtual file system abstraction.
 */

#include <globals.h>

#include <string.h>
#include <stdlib.h>

#include <fs/dnode.h>
#include <fs/fs_driver.h>
#include <fs/vfs.h>
#include <fs/vnode.h>
#include <fs/ramfs/ramfs.h>

/// Compares two partial file names.
static int name_compare(const char* path, const char* filename)
{
	int ret;
	
	// Find partial directory name.
	size_t part_len = 0;
	while (path[part_len] != '/' && path[part_len] != '\0')
	{
		part_len++;
	}
	
	// Compare partial strings.
	ret = strncmp(path, filename, part_len);
	if (ret == 0)
	{
		// Verify that lengths are the same.
		if (strlen(filename) == part_len)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return ret;
	}
}

void vfs_init(void)
{
	static bool initialized = false;
	
	if (initialized == false)
	{
		// Set up root directory.
		vfs_root = malloc(sizeof(Dnode));
		vfs_root->name = malloc(sizeof(char));
		*vfs_root->name = '\0';
		vfs_root->height = 1;
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
	// Verify that this is an absolute path.
	if (path == NULL || path[0] != '/')
	{
		return -1;
	}
	
	// Build path to parent directory.
	size_t parent_path_len = strrchr(path, '/') - path;
	char parent_path[parent_path_len + 1];
	parent_path[0] = '\0';
	strncat(parent_path, path, parent_path_len + 1);
	
	// Find parent directory.
	Dnode* parent = vfs_find_dnode(parent_path);
	if (parent == NULL)
	{
		// Parent does not exist.
		return -1;
	}
	
	// Insert new subdirectory.
	path = strrchr(path, '/') + 1;
	parent->dir = dnode_tree_insert(parent->dir, path);
	
	return 0;
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
	// Verify that this is an absolute path.
	if (path == NULL || path[0] != '/')
	{
		return NULL;
	}
	
	Dnode* dir = vfs_root;
	
	while (dir != NULL && path != NULL)
	{	
		// Check if we found the dnode.
		if (name_compare(path, dir->name) == 0)
		{
			return dir;
		}
		
		// Build partial path.
		size_t part_len = 0;
		while (path[part_len] != '/' && path[part_len] != '\0')
		{
			part_len++;
		}
		char part_path[part_len + 1];
		part_path[0] = '\0';
		strncat(part_path, path, part_len);
		
		// Find next subdirectory.
		dir = dnode_tree_find(dir->dir, part_path);
		
		// Adjust path to next subdirectory.
		path += part_len + 1;
	}
	
	// Not found.
	return NULL;
}
