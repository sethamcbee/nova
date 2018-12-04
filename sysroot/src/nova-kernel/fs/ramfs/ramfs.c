/**
 * @file ramfs.c
 * @author Seth McBee
 * @date 2018-10-8
 * @brief Memory-only file system.
 */

#include <globals.h>

#include <fs/dnode.h>
#include <fs/fs_driver.h>
#include <fs/vnode.h>
#include <fs/ramfs/ramfs.h>

/// Ramfs driver instance.
static FS_Driver driver;

FS_Driver* ramfs_driver(void)
{
    static bool initialized = false;

    if (initialized == false)
    {
        driver.mount = ramfs_mount;
        driver.create_dnode = ramfs_create_dnode;
        driver.create_vnode = ramfs_create_vnode;

        initialized = true;
    }

    return &driver;
}

int ramfs_mount(Dnode* mountpoint)
{
    // Nothing to do.
    return 0;
}

int ramfs_create_vnode(Vnode* file, Vnode_Type type)
{

}

int ramfs_create_dnode(Dnode* dir, Dnode_Type type)
{

}

ssize_t ramfs_read(Vnode* file, void* buf, size_t n, size_t pos)
{

}

ssize_t ramfs_write(Vnode* file, const void* buf, size_t n, size_t pos)
{

}
