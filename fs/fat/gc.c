/*
 * linux/fs/fat/gc.c
 *
 * Copyright 2007 Sony Corporation.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/completion.h>
#include <linux/slab.h>
#include "fat.h"

/*
  bit in FAT bitmap:
    0 - Invalid entry
    1 - Valid entry
 */

#define BITS_PER_BYTE 8

struct gc_dir {
	loff_t pos;

	struct inode *inode;
	struct buffer_head *bh;          /* the BH scanning */
	struct msdos_dir_entry *de;      /* current dentry  */
	char fn[MSDOS_NAME+2];
};

struct gc_entry {
	struct inode *inode;
	char fn[MSDOS_NAME+2];
};

static void gc_ent_init(struct gc_entry *ent)
{
	ent->inode = NULL;
}

static void gc_ent_destroy(struct gc_entry *ent)
{
	if (ent->inode)
		iput(ent->inode);
	gc_ent_init(ent);
}

static void gc_dir_destroy(struct gc_dir *dir)
{
	if (dir->inode) {
		iput(dir->inode);
		dir->inode = NULL;
	}
	if (dir->bh) {
		brelse(dir->bh);
		dir->bh = NULL;
	}
}

static void gc_dir_init(struct gc_dir *dir, struct gc_entry *ent)
{
	dir->pos = 0;
	dir->inode = ent?ent->inode:NULL;
	dir->bh = NULL;
	dir->de = NULL;
	if(ent) strncpy(dir->fn, ent->fn, MSDOS_NAME+2);
}

static void gc_dir_reinit(struct gc_dir *dir, struct gc_entry *ent)
{
	gc_dir_destroy(dir);
	gc_dir_init(dir, ent);
}

static char * gc_make_fn(struct msdos_dir_entry *de, char *buf) {
	memset(buf, 0, MSDOS_NAME + 2);
        if(de) {
		int i;
                strncpy(buf, (de)->name, 8);
		for (i=0; i<8; i++) {
			if (buf[i] == ' ') {
				buf[i] = '.';
				break;
			}
		}

                strncpy(buf+i+1, (de)->name + 8, 3);
		for (; i<MSDOS_NAME+2; i++) {
			if (buf[i] == ' ') {
				buf[i] = 0;
				break;
			}
		}
	}

	return buf;
}

static inline int gc_quit(struct super_block *sb)
{
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	return sbi->gc_quit;
}

static int gc_get_entry(struct super_block *sb, struct gc_dir *dir, struct gc_entry *ent)
{
	struct inode *inode;
	loff_t i_pos;
	int ret;

	mutex_lock(&dir->inode->i_mutex);

	do {
		cond_resched();

		if ((ret = fat_get_short_entry(dir->inode, &dir->pos, &dir->bh, &dir->de))) {
			ret = (ret == -ENOENT) ? 0:ret;
			goto out;
		}

		if (strncmp(dir->de->name, MSDOS_DOT   , MSDOS_NAME) &&
		    strncmp(dir->de->name, MSDOS_DOTDOT, MSDOS_NAME))
			break;
	} while(1);

	ret = 1;

	i_pos = fat_make_i_pos(sb, dir->bh, dir->de);
	inode = fat_build_inode(sb, dir->de, i_pos);
	if (IS_ERR(inode)) {
		pr_info("GC: fat_build_inode error failed - %d!\n", (int)inode);
		ret = (int)inode;
		goto out;
	}

	ent->inode = inode;
	gc_make_fn(dir->de, ent->fn);

 out:
	mutex_unlock(&dir->inode->i_mutex);

	return ret;
}

void fat_gc_mark_cluster_valid(struct msdos_sb_info *sbi, int clu)
{
	u8 *p = sbi->gc_bitmap;
	u8 mask = ((u8)1) << (clu & 7);

	if (!sbi->options.gc)
		return;

	p += (clu >> 3);
	*p |= mask;
}

EXPORT_SYMBOL(fat_gc_mark_cluster_valid);

static int fat_gc_is_cluster_valid(struct msdos_sb_info *sbi, int clu)
{
	u8 *p = sbi->gc_bitmap;
	u8 mask = ((u8)1) << (clu & 7);

	p += (clu >> 3);
	return (*p & mask);
}

static int __gc_mark_valid_entries(struct inode *inode, int relax)
{
	struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);
	int cluster;
	struct fat_entry fatent;
	int res = 0;
	loff_t off = 0;

	cluster = MSDOS_I(inode)->i_start;
	if (!cluster || inode->i_size == 0)
		goto out;

	fatent_init(&fatent);
	while(cluster >= FAT_START_ENT &&
	      cluster < sbi->max_cluster) {
		if (fat_gc_is_cluster_valid(sbi, cluster)) { // this entry has been scanned?
			break;
		}

		if (gc_quit(inode->i_sb))
			break;

		if (relax)
			cond_resched();

		res ++;
		off += sbi->cluster_size;

		fat_gc_mark_cluster_valid(sbi, cluster);

		cluster = fat_ent_read(inode, &fatent, cluster);
		if (cluster < 0) {
			pr_info("GC: invalid FAT entry - minus value!\n");
			res = -EINVAL;
			break;
		}
		else if (cluster == FAT_ENT_FREE) {
			pr_info("GC: invalid FAT entry - free entry in FAT chain!\n");
			res = -EINVAL;
			break;
		}
		else if (cluster == FAT_ENT_EOF) {
			if (inode->i_size > off) {
				pr_info("GC: file size %llu > cluster chain length %llu!\n",
					 inode->i_size, off);
				pr_info("    Truncate it to cluster chain length!\n");

				inode->i_size = off;
				mark_inode_dirty(inode);
			}
			break;
		}

		if (off >= inode->i_size) {
			int ret;

			pr_info("GC: file size %llu < cluster chain length!\n",
				 inode->i_size);
			pr_info("    Truncate it to file size!\n");

			ret = fat_ent_write(inode, &fatent, FAT_ENT_EOF, 0);
			if (ret)
				res = ret;

			break;
		}
	}
	fatent_brelse(&fatent);

 out:
	return res;
}

/*
 * inode->i_mutex has to be taken before
 */
int fat_gc_mark_valid_entries(struct inode *inode)
{
	struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);

	if (!sbi->options.gc)
		return 0;

	return __gc_mark_valid_entries(inode, 0);

}

EXPORT_SYMBOL(fat_gc_mark_valid_entries);

static int gc_mark_valid_entries(struct inode *inode)
{
	int ret;

	mutex_lock(&inode->i_mutex);

	ret = __gc_mark_valid_entries(inode, 1);

	mutex_unlock(&inode->i_mutex);

	return ret;
}

static int gc_inode_scanned(struct inode *inode)
{
	return (i_size_read(inode) == 0) ||
		(fat_gc_is_cluster_valid(MSDOS_SB(inode->i_sb), MSDOS_I(inode)->i_start));
}

/* mark all inode occupied FAT entries VALID*/
static int gc_mark_all_inode_entries_valid(struct super_block *sb)
{
	struct gc_dir gc_dir;
	struct gc_entry gc_ent;
	struct gc_entry gc_root = {
		.inode = sb->s_root->d_inode,
		.fn = "/"
	};
	int progress, ret = 0, scanned = 0, validated = 0;

	pr_debug("GC: %s\n", __func__);

	gc_ent_init(&gc_ent);
	gc_dir_init(&gc_dir, NULL);
	do {
		progress = 0;

		if(!igrab(gc_root.inode))
			break;

		gc_dir_reinit(&gc_dir, &gc_root);
		do {
			if (gc_quit(sb)) {
				ret = 1;
				break;
			}

			cond_resched();

			gc_ent_destroy(&gc_ent);
			ret = gc_get_entry(sb, &gc_dir, &gc_ent);

			cond_resched();

			if (ret == 0) { /* end of directory */
				ret = gc_mark_valid_entries(gc_dir.inode);
				if (ret > 0) {
					pr_debug("GC: %d entries of dir %s(%lu) validated!\n",
						 ret, gc_dir.fn, gc_dir.inode->i_ino);
					progress += ret;
					validated += ret;
					scanned ++;
				}
				break;
			}
			else if(ret > 0) { /* entry got */
				if (gc_inode_scanned(gc_ent.inode))
					continue;

				if (S_ISDIR(gc_ent.inode->i_mode)) { /* this directory has not been scanned*/
					pr_debug("GC: enter directory - %s\n", gc_ent.fn);
					gc_dir_reinit(&gc_dir, &gc_ent);
					gc_ent_init(&gc_ent);

					continue;
				}

				ret = gc_mark_valid_entries(gc_ent.inode);
				if (ret > 0) {
					pr_debug("GC: %d entries of file %s(%lu) validated!\n",
						 ret, gc_ent.fn, gc_ent.inode->i_ino);
					progress += ret;
					validated += ret;
					scanned ++;

				}
				else if (ret < 0)
					break;
			}
			else { /* error happened during gc_get_entry */
				pr_info("GC: Error happened during gc_get_entry- %d!\n", ret);
				pr_info("GC: Quit!\n");
				break;
			}
		} while(1);
	} while(progress > 0);
	gc_dir_destroy(&gc_dir);
	gc_ent_destroy(&gc_ent);

	pr_info("GC: %s - %d(%d) file/dir[s] scanned!\n", __func__, scanned, validated);
	return ret;
}

/* make possible INVALID FAT entries FREE*/
static int gc_free_invalid_entries(struct super_block *sb)
{
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	struct inode *sb_inode = sb->s_root->d_inode;
	struct fat_entry fatent;
	int cluster = FAT_START_ENT;
	int freed = 0, valid = 0;
	int ret = 0;

	pr_debug("GC: %s\n", __func__);

	fatent_init(&fatent);
	for(; cluster < sbi->max_cluster; cluster ++) {
		int val;

		if (gc_quit(sb))
			break;

		if (cluster % 128 == 0)
			cond_resched();

		mutex_lock(&sbi->fat_lock);

		if (fat_gc_is_cluster_valid(sbi, cluster)) {
			valid ++;
			goto next;
		}

		val = fat_ent_read(sb_inode, &fatent, cluster);
		if (val == FAT_ENT_FREE)
			goto next;

		ret = fat_ent_write(sb_inode, &fatent, FAT_ENT_FREE, 0);
		if (ret) {
			mutex_unlock(&sbi->fat_lock);
			break;
		}

		freed ++;
	next:
		mutex_unlock(&sbi->fat_lock);
	}
	fatent_brelse(&fatent);

	pr_info("GC: %d(%d) entries freed!\n", freed, valid);

	if (freed) {
		sbi->free_clusters = -1;
		fat_count_free_clusters(sb);
	}

	return ret;
}

static int gc_main(void *arg)
{
	struct super_block *sb = arg;
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	int ret = 0;

	set_user_nice(current, 20);

	daemonize("fat-gc");

	pr_debug("GC: %s\n", __func__);

	if ((ret = gc_mark_all_inode_entries_valid(sb))) {
		if (ret < 0)
			pr_info("GC: error in mark_all_inode_entries_valid: %d!\n", ret);
		goto out;
	}

	ret = gc_free_invalid_entries(sb);
 out:
	pr_debug("GC: %s quit\n", __func__);

	sbi->gc_quit = 1;
	complete_and_exit(&sbi->gc_com, ret);

	return 0;
}

void fat_start_gc(struct super_block *sb)
{
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	int bitmap_size;

	if (!sbi->options.gc)
		return;

	pr_debug("GC: starting - %lu clusters\n", sbi->max_cluster);

	sbi->gc_quit = 0;
	init_completion(&sbi->gc_com);

	bitmap_size = (sbi->max_cluster + BITS_PER_BYTE - 1) / BITS_PER_BYTE;
	sbi->gc_bitmap = kmalloc(bitmap_size, GFP_KERNEL);
	if (!sbi->gc_bitmap) {
		printk(KERN_ERR "GC: FAT bitmap(%d) allocation failed!\n", bitmap_size);
		printk(KERN_ERR "GC: GC will not be started");

		return;
	}
	memset(sbi->gc_bitmap, 0, bitmap_size);

	if (kernel_thread(gc_main, sb, 0) < 0) {
		printk(KERN_ERR "GC: kernel thread startup failed!\n");
		complete(&sbi->gc_com);
	}
}

void fat_stop_gc(struct super_block *sb)
{
	struct msdos_sb_info *sbi = MSDOS_SB(sb);

	if (!sbi->options.gc)
		return;

	sbi->gc_quit = 1;

	wait_for_completion(&sbi->gc_com);
	if (sbi->gc_bitmap)
		kfree(sbi->gc_bitmap);
}
