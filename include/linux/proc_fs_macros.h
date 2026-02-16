/*
 * The proc filesystem macros 
 */

#ifndef _LINUX_PROC_FS_MACROS_H
#define _LINUX_PROC_FS_MACROS_H

#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define DECLARE_PROCFS_ENTRY(name, read_proc, write_proc) \
static int name##_proc_open(struct inode *inode, struct file *file) \
{ \
    return single_open(file, read_proc, PDE_DATA(inode)); \
} \
static const struct file_operations name##_proc_fops = { \
    .open     = name##_proc_open, \
    .read     = seq_read, \
    .llseek   = seq_lseek, \
    .release  = single_release, \
    .write    = write_proc \
};

#define DECLARE_PROCFS_READ_ENTRY(name, read_proc) \
static int name##_proc_open(struct inode *inode, struct file *file) \
{ \
    return single_open(file, read_proc, PDE_DATA(inode)); \
} \
static const struct file_operations name##_proc_fops = { \
    .open     = name##_proc_open, \
    .read     = seq_read, \
    .llseek   = seq_lseek, \
    .release  = single_release, \
};

#define DECLARE_PROCFS_WRITE_ENTRY(name, write_proc) \
static const struct file_operations name##_proc_fops = { \
    .write    = write_proc \
};

#endif /* _LINUX_PROC_FS_MACROS_H */
