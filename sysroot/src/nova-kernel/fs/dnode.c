/**
 * @file dnode.c
 * @author Seth McBee
 * @date 2018-10-10
 * @brief VFS directory node operations.
 */

#include <globals.h>

#include <string.h>

#include <fs/dnode.h>
#include <fs/vnode.h>

// Helper functions.
static int dnode_get_height(Dnode* root);
static void dnode_update_height(Dnode* root);
static int dnode_get_balance(Dnode* root);
static Dnode* dnode_balance(Dnode* root);
static Dnode* dnode_rotate_left(Dnode* root);
static Dnode* dnode_rotate_right(Dnode* root);

/// Compares two partial file names.
static int dnode_compare(const char* path, const char* filename)
{
	int ret;
	
	// Find partial directory name.
	size_t part_len;
	if (strchr(path, '/') != NULL)
	{
		part_len = strchr(path, '/') - path;
	}
	else
	{
		part_len = strlen(path);
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

Dnode* dnode_tree_insert(Dnode* root, const char* name)
{
	// Insert using normal BST rules.
	if (root == NULL)
	{
		root = malloc(sizeof(Dnode));
		root->name = malloc(strlen(name));
		strcpy(root->name, name);
		root->left = NULL;
		root->right = NULL;
		root->height = 1;
		
		return root;
	}
	else if (dnode_compare(name, root->name) > 0)
	{
		root->right = dnode_tree_insert(root->right, name);
	}
	else if (dnode_compare(name, root->name) < 0)
	{
		root->left = dnode_tree_insert(root->left, name);
	}
	else
	{
		// No duplicates allowed.
		return root;
	}
	
	// Balance tree if needed.
	dnode_update_height(root);
	root = dnode_balance(root);
	
	return root;
}

Dnode* dnode_tree_remove(Dnode* root, const char* name)
{
	
}

Dnode* dnode_tree_find(Dnode* root, const char* name)
{
	// Perform BST search.
	while (root != NULL)
	{
		if (dnode_compare(name, root->name) > 0)
		{
			root = root->right;
		}
		else if (dnode_compare(name, root->name) < 0)
		{
			root = root->left;
		}
		else
		{
			// Found the directory.
			return root;
		}
	}
	
	// Not found.
	return NULL;
}

static int dnode_get_height(Dnode* root)
{
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return root->height;
	}
}

static void dnode_update_height(Dnode* root)
{
	int left_height;
	int right_height;
	
	// Calculate heights of child branches.
	left_height = dnode_get_height(root->left);
	right_height = dnode_get_height(root->right);
	
	// height = 1 + max(lh, rh)
	if (left_height > right_height)
	{
		root->height = 1 + left_height;
	}
	else // right_height >= left_height
	{
		root->height = 1 + right_height;
	}
}

static int dnode_get_balance(Dnode* root)
{
	int left = dnode_get_height(root->left);
	int right = dnode_get_height(root->right);
	
	return left - right;
}

static Dnode* dnode_rotate_right(Dnode* root)
{
	Dnode* new_root = root->left;
	Dnode* new_left = new_root->right;
	
	// Rotate.
	new_root->right = root;
	root->left = new_left;
	
	dnode_update_height(root);
	dnode_update_height(new_root);
	
	return new_root;
}

static Dnode* dnode_rotate_left(Dnode* root)
{
	Dnode* new_root = root->right;
	Dnode* new_right = new_root->left;
	
	// Rotate.
	new_root->left = root;
	root->right = new_right;
	
	dnode_update_height(root);
	dnode_update_height(new_root);
	
	return new_root;
}

static Dnode* dnode_balance(Dnode* root)
{
	int balance = dnode_get_balance(root);
	
	if (balance > 1)
	{
		// L-R.
		if (dnode_get_balance(root->left) < 0)
		{
			root->left = dnode_rotate_left(root->left);
		}
		
		return dnode_rotate_right(root);
	}
	else if (balance < -1)
	{
		// R-L.
		if (dnode_get_balance(root->right) > 0)
		{
			root->right = dnode_rotate_right(root->right);
		}
		
		return dnode_rotate_left(root);
	}
	
	// Else, do nothing.
	return root;
}
