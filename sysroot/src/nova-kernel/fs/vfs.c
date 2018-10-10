/**
 * @file vfs.c
 * @author Seth McBee
 * @date 2018-10-7
 * @brief Virtual file system abstraction.
 */

#include <globals.h>

#include <fs/vfs.h>

void vfs_init(void)
{
	vfs_root = NULL;
}
