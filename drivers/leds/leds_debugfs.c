/*
 * Copyright (c) 2014 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/debugfs.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/uaccess.h>

#include "leds_debugfs.h"

unsigned int mtk_leds_debug;

/* ------------------------------------------------------------------------- */
/* Debug Options */
/* ------------------------------------------------------------------------- */
static char STR_HELP[] =
	"\n"
	"USAGE\n"
	"        echo [ACTION]... > mtkleds\n"
	"\n"
	"ACTION\n"
	"\n"
	"        dump:\n"
	"             dump all hw registers\n"
	"\n"
	"        regw:addr=val\n"
	"             write hw register\n"
	"\n"
	"        regr:addr\n"
	"             read hw register\n";

/* ------------------------------------------------------------------------- */
/* Command Processor */
/* ------------------------------------------------------------------------- */
static void process_dbg_opt(const char *opt)
{
	if (strncmp(opt, "debug:", 6) == 0) {
		char *p = (char *)opt + 6;
		unsigned long debug_level;

		if (kstrtoul(p, 16, &debug_level))
			goto error;

		mtk_leds_debug = debug_level;
		printk("set mtk_leds_debug to 0x%X\n", mtk_leds_debug);
	} else {
	    goto error;
	}

	return;
 error:
	printk("Parse command error!\n\n%s", STR_HELP);
}

static void process_dbg_cmd(char *cmd)
{
	char *tok;

	printk("[mtkleds_dbg] %s\n", cmd);
	while ((tok = strsep(&cmd, " ")) != NULL)
		process_dbg_opt(tok);
}

/* ------------------------------------------------------------------------- */
/* Debug FileSystem Routines */
/* ------------------------------------------------------------------------- */
static int debug_open(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

static ssize_t debug_read(struct file *file, char __user *ubuf, size_t count,
			  loff_t *ppos)
{
	return simple_read_from_buffer(ubuf, count, ppos, STR_HELP,
				       strlen(STR_HELP));
}

static char dis_cmd_buf[512];
static ssize_t debug_write(struct file *file, const char __user *ubuf,
	size_t count, loff_t *ppos)
{
	const int debug_bufmax = sizeof(dis_cmd_buf) - 1;
	size_t ret;

	ret = count;

	if (count > debug_bufmax)
		count = debug_bufmax;

	if (copy_from_user(&dis_cmd_buf, ubuf, count))
		return -EFAULT;

	dis_cmd_buf[count] = 0;

	process_dbg_cmd(dis_cmd_buf);

	return ret;
}

struct dentry *mtk_leds_dbgfs;
static const struct file_operations debug_fops = {
	.read = debug_read,
	.write = debug_write,
	.open = debug_open,
};


void mtk_lp5523_debugfs_init(void)
{
	MTK_LEDS_DEBUG_DRIVER("\n");
	mtk_leds_dbgfs = debugfs_create_file("mtkleds", S_IFREG | S_IRUGO |
			S_IWUSR | S_IWGRP, NULL, (void *)0, &debug_fops);


	MTK_LEDS_DEBUG_DRIVER("..done\n");
}

void mtk_lp5523_debugfs_deinit(void)
{
	debugfs_remove(mtk_leds_dbgfs);
}

void mtk_lp5523_ut_debug_printk(const char *function_name, const char *format, ...)
{
	struct va_format vaf;
	va_list args;

	va_start(args, format);
	vaf.fmt = format;
	vaf.va = &args;

	printk(KERN_ERR "[leds:%s] %pV", function_name, &vaf);

	va_end(args);
}
