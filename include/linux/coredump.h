#ifndef _LINUX_COREDUMP_H
#define _LINUX_COREDUMP_H

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/fs.h>

#if 1 // CONFIG_LITTLE_CORE_DUMP
#define LTCORE_MAX_PAGES 512
extern int little_core_dump;
extern unsigned ltcore_dump_cnt;
extern char ltcore_dump_file[];
#endif

#define LTCORE_USE_ZLIB 1
#ifdef LTCORE_USE_ZLIB 
#include <linux/zlib.h>
extern z_stream ltcore_dump_zstr;
// If we're going to use gzipped cores, we don't need nearly as much ltcore memory, so trim it down
#undef  LTCORE_MAX_PAGES
#define LTCORE_MAX_PAGES 160
#endif
/*
 * These are the only things you should do on a core-file: use only these
 * functions to write out all the necessary info.
 */
extern int dump_write(struct file *file, const void *addr, int nr);
extern int dump_seek(struct file *file, loff_t off);

#endif /* _LINUX_COREDUMP_H */
