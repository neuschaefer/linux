#ifndef _FAT_H
#define _FAT_H

#include <linux/buffer_head.h>
#include <linux/string.h>
#include <linux/nls.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/ratelimit.h>
#include <linux/msdos_fs.h>
#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
#include <linux/k3d.h>
#endif
#include "fat_tz.h"

/*
 * vfat shortname flags
 */
#define VFAT_SFN_DISPLAY_LOWER	0x0001 /* convert to lowercase for display */
#define VFAT_SFN_DISPLAY_WIN95	0x0002 /* emulate win95 rule for display */
#define VFAT_SFN_DISPLAY_WINNT	0x0004 /* emulate winnt rule for display */
#define VFAT_SFN_CREATE_WIN95	0x0100 /* emulate win95 rule for create */
#define VFAT_SFN_CREATE_WINNT	0x0200 /* emulate winnt rule for create */

#define FAT_ERRORS_CONT		1      /* ignore error and continue */
#define FAT_ERRORS_PANIC	2      /* panic on error */
#define FAT_ERRORS_RO		3      /* remount r/o on error */

#define FAT_LKUP_HINT_MAX     16
/* assume FAT_LKUP_HINT_MAX <= sizeof(long)*8 */

struct fat_lookup_hint {
	struct list_head lru_list;
	pid_t pid;
	struct inode *dir;
	loff_t last_pos;
#define FAT_LKUP_ST_RANDOM       1
#define FAT_LKUP_ST_LFN_RANDOM       2
	unsigned int state;
};

struct fat_mount_options {
	uid_t fs_uid;
	gid_t fs_gid;
	unsigned short fs_fmask;
	unsigned short fs_dmask;
	unsigned short codepage;  /* Codepage for shortname conversions */
	char *iocharset;          /* Charset used for filename input/display */
	unsigned short shortname; /* flags for shortname display/create rule */
	unsigned char name_check; /* r = relaxed, n = normal, s = strict */
	unsigned char errors;	  /* On error: continue, panic, remount-ro */
	unsigned short allow_utime;/* permission for setting the [am]time */
	unsigned quiet:1,         /* set = fake successful chmods and chowns */
		 showexec:1,      /* set = only set x bit for com/exe/bat */
		 sys_immutable:1, /* set = system files are immutable */
		 dotsOK:1,        /* set = hidden and system files are named '.filename' */
		 no_sect_bndry:1, /* set = avoid acrossing sector boundary on FAT12 alloc */
		 isvfat:1,        /* 0=no vfat long filename support, 1=vfat support */
		 posix_attr:1,	  /* 1= posix attribute mapping support */
		 utf8:1,	  /* Use of UTF-8 character set (Default) */
		 unicode_xlate:1, /* create escape sequences for unhandled Unicode */
		 numtail:1,       /* Does first alias have a numeric '~1' type tail? */
		 flush:1,	  /* write things quickly */
		 nocase:1,	  /* Does this need case conversion? 0=need case conversion*/
		 compare_unicode:1, /* compare file names with Unicode */
		 clnshutbit:1,	  /* FAT16, FAT32 ClnShutBit support */
		 ignore_crtime:1, /* do not read nor update creation date & time */
		 strict:1,	  /* enables strict check on boot sector */
		 check_disk:1,    /* check removable disk */
		 usefree:1,	  /* Use free_clusters for FAT32 */
		 tz_utc:1,	  /* Filesystem timestamps are in UTC */
		 rodir:1,	  /* allow ATTR_RO for directory */
 		 relax_sync:1,	  /* set = make sync behavior looser */
		 batch_sync:1,	  /* set = make sync in BATCH mode */
		 dirsync:1,
		 avoid_dlink:1,
		 gc:1,		  /* enable Garbage Collector */
		 discard:1;	  /* Issue discard requests on deletions */
	struct tz_rule timezone;
};

#define FAT_HASH_BITS	8
#define FAT_HASH_SIZE	(1UL << FAT_HASH_BITS)

struct fat_posix_ops {
	int (*is_symlink)(struct inode *, struct msdos_dir_entry *);
	int (*set_attr)(struct msdos_dir_entry *, struct inode *);
	int (*get_attr)(struct inode *inode, struct msdos_dir_entry *dentry);
};

/*
 * MS-DOS file system in-core superblock data
 */
struct msdos_sb_info {
	unsigned short sec_per_clus; /* sectors/cluster */
	unsigned short cluster_bits; /* log2(cluster_size) */
	unsigned int cluster_size;   /* cluster size */
	unsigned char fats,fat_bits; /* number of FATs, FAT bits (12 or 16) */
	unsigned short fat_start;
	unsigned long fat_length;    /* FAT start & length (sec.) */
	unsigned long dir_start;
	unsigned short dir_entries;  /* root dir start & entries */
	unsigned long data_start;    /* first data sector */
	unsigned long max_cluster;   /* maximum cluster number */
	unsigned long root_cluster;  /* first cluster of the root directory */
	unsigned long fsinfo_sector; /* sector number of FAT32 fsinfo */
	struct mutex fat_lock;
	unsigned int prev_free;      /* previously allocated cluster number */
	unsigned int free_clusters;  /* -1 if undefined */
	unsigned int free_clus_valid; /* is free_clusters valid? */
	struct fat_mount_options options;
	struct nls_table *nls_disk;  /* Codepage used on disk */
	struct nls_table *nls_io;    /* Charset used for input and display */
	const void *dir_ops;		     /* Opaque; default directory operations */
	int dir_per_block;	     /* dir entries per block */
	int dir_per_block_bits;	     /* log2(dir_per_block) */
	unsigned long vol_id;        /* volume ID */
	int clnshutbit;		     /* ClnShutBit */

	int fatent_shift;
	struct fatent_operations *fatent_ops;
	struct inode *fat_inode;
	struct fat_posix_ops posix_ops;

	struct ratelimit_state ratelimit;

	spinlock_t inode_hash_lock;
	struct hlist_head inode_hashtable[FAT_HASH_SIZE];

	/* dirent lookup hints */
	spinlock_t lkup_hint_lock;
	struct list_head lkup_hint_head;
	struct fat_lookup_hint lkup_hint[FAT_LKUP_HINT_MAX];
	unsigned lkup_hint_freemap:FAT_LKUP_HINT_MAX;

	/* GC fields */
	struct completion gc_com;
	u8 *gc_bitmap;
	int gc_quit;
};

#define FAT_CACHE_VALID	0	/* special case for valid cache */

/*
 * MS-DOS file system inode data in memory
 */
struct msdos_inode_info {
	spinlock_t cache_lru_lock;
	struct list_head cache_lru;
	int nr_caches;
	/* for avoiding the race between fat_free() and fat_get_cluster() */
	unsigned int cache_valid_id;

	/* NOTE: mmu_private is 64bits, so must hold ->i_mutex to access */
	loff_t mmu_private;	/* physically allocated size */

	int i_start;		/* first cluster or 0 */
	int i_logstart;		/* logical first cluster */
	int i_attrs;		/* unused attribute bits */
	loff_t i_pos;		/* on-disk position of directory entry or 0 */
	struct hlist_node i_fat_hash;	/* hash by i_location */

	/* for Fast Sync Operation*/
	int i_new; /*1: new inode && not SYNCed yet*/
	int i_last_dclus; /*not SYNCed last cluster or 0*/
	int i_new_dclus; /*new cluster to be linked at i_last*/

	struct inode vfs_inode;
};

struct fat_slot_info {
	loff_t i_pos;		/* on-disk position of directory entry */
	loff_t slot_off;	/* offset for slot or de start */
	int nr_slots;		/* number of slots + 1(de) in filename */
	struct msdos_dir_entry *de;
	struct buffer_head *bh;
};

static inline struct msdos_sb_info *MSDOS_SB(struct super_block *sb)
{
	return sb->s_fs_info;
}

static inline struct msdos_inode_info *MSDOS_I(struct inode *inode)
{
	return container_of(inode, struct msdos_inode_info, vfs_inode);
}

/*
 * If ->i_mode can't hold S_IWUGO (i.e. ATTR_RO), we use ->i_attrs to
 * save ATTR_RO instead of ->i_mode.
 *
 * If it's directory and !sbi->options.rodir, ATTR_RO isn't read-only
 * bit, it's just used as flag for app.
 */
static inline int fat_mode_can_hold_ro(struct inode *inode)
{
	struct msdos_sb_info *sbi = MSDOS_SB(inode->i_sb);
	mode_t mask;

	if (S_ISDIR(inode->i_mode)) {
		if (!sbi->options.rodir)
			return 0;
		mask = ~sbi->options.fs_dmask;
	} else
		mask = ~sbi->options.fs_fmask;

	if (!(mask & S_IWUGO))
		return 0;
	return 1;
}

/* Convert attribute bits and a mask to the UNIX mode. */
static inline mode_t fat_make_mode(struct msdos_sb_info *sbi,
				   u8 attrs, mode_t mode)
{
	if (attrs & ATTR_RO && !((attrs & ATTR_DIR) && !sbi->options.rodir))
		mode &= ~S_IWUGO;

	if (attrs & ATTR_DIR)
		return (mode & ~sbi->options.fs_dmask) | S_IFDIR;
	else
		return (mode & ~sbi->options.fs_fmask) | S_IFREG;
}

/* Return the FAT attribute byte for this inode */
static inline u8 fat_make_attrs(struct inode *inode)
{
	u8 attrs = MSDOS_I(inode)->i_attrs;
	if (S_ISDIR(inode->i_mode))
		attrs |= ATTR_DIR;
	if (fat_mode_can_hold_ro(inode) && !(inode->i_mode & S_IWUGO))
		attrs |= ATTR_RO;
	return attrs;
}

static inline void fat_save_attrs(struct inode *inode, u8 attrs)
{
	if (fat_mode_can_hold_ro(inode))
		MSDOS_I(inode)->i_attrs = attrs & ATTR_UNUSED;
	else
		MSDOS_I(inode)->i_attrs = attrs & (ATTR_UNUSED | ATTR_RO);
}

static inline unsigned char fat_checksum(const __u8 *name)
{
	unsigned char s = name[0];
	s = (s<<7) + (s>>1) + name[1];	s = (s<<7) + (s>>1) + name[2];
	s = (s<<7) + (s>>1) + name[3];	s = (s<<7) + (s>>1) + name[4];
	s = (s<<7) + (s>>1) + name[5];	s = (s<<7) + (s>>1) + name[6];
	s = (s<<7) + (s>>1) + name[7];	s = (s<<7) + (s>>1) + name[8];
	s = (s<<7) + (s>>1) + name[9];	s = (s<<7) + (s>>1) + name[10];
	return s;
}

static inline sector_t fat_clus_to_blknr(struct msdos_sb_info *sbi, int clus)
{
	return ((sector_t)clus - FAT_START_ENT) * sbi->sec_per_clus
		+ sbi->data_start;
}

static inline void fat16_towchar(wchar_t *dst, const __u8 *src, size_t len)
{
#ifdef __BIG_ENDIAN
	while (len--) {
		*dst++ = src[0] | (src[1] << 8);
		src += 2;
	}
#else
	memcpy(dst, src, len * 2);
#endif
}

static inline void fatwchar_to16(__u8 *dst, const wchar_t *src, size_t len)
{
#ifdef __BIG_ENDIAN
	while (len--) {
		dst[0] = *src & 0x00FF;
		dst[1] = (*src & 0xFF00) >> 8;
		dst += 2;
		src++;
	}
#else
	memcpy(dst, src, len * 2);
#endif
}

#ifdef CONFIG_SNSC_FS_FAT_GC
static inline loff_t fat_make_i_pos(struct super_block *sb,
				    struct buffer_head *bh,
				    struct msdos_dir_entry *de)
{
	return ((loff_t)bh->b_blocknr << MSDOS_SB(sb)->dir_per_block_bits)
		| (de - (struct msdos_dir_entry *)bh->b_data);
}
#endif

/* fat/cache.c */
extern void fat_cache_inval_inode(struct inode *inode);
extern int fat_get_cluster(struct inode *inode, int cluster,
			   int *fclus, int *dclus);
extern int fat_bmap(struct inode *inode, sector_t sector, sector_t *phys,
		    unsigned long *mapped_blocks, int create);

/* fat/dir.c */
extern const struct file_operations fat_dir_operations;
extern int fat_search_long(struct inode *inode, const unsigned char *name,
			   int name_len, struct fat_slot_info *sinfo);
extern int fat_dir_empty(struct inode *dir);
extern int fat_subdirs(struct inode *dir);
extern int fat_scan(struct inode *dir, const unsigned char *name,
		    struct fat_slot_info *sinfo);
#ifdef CONFIG_SNSC_FS_FAT_GC
extern int fat_get_short_entry(struct inode *dir, loff_t *pos,
			       struct buffer_head **bh,
			       struct msdos_dir_entry **de);
#endif
extern int fat_get_dotdot_entry(struct inode *dir, struct buffer_head **bh,
				struct msdos_dir_entry **de, loff_t *i_pos);
extern int fat_alloc_new_dir(struct inode *dir, struct timespec *ts);
extern int fat_add_entries(struct inode *dir, void *slots, int nr_slots,
			   struct fat_slot_info *sinfo);
extern int fat_remove_entries(struct inode *dir, struct fat_slot_info *sinfo);

/* fat/fatent.c */
struct fat_entry {
	int entry;
	union {
		u8 *ent12_p[2];
		__le16 *ent16_p;
		__le32 *ent32_p;
	} u;
	int nr_bhs;
	struct buffer_head *bhs[2];
	struct inode *fat_inode;
};

static inline void fatent_init(struct fat_entry *fatent)
{
	fatent->nr_bhs = 0;
	fatent->entry = 0;
	fatent->u.ent32_p = NULL;
	fatent->bhs[0] = fatent->bhs[1] = NULL;
	fatent->fat_inode = NULL;
}

static inline void fatent_set_entry(struct fat_entry *fatent, int entry)
{
	fatent->entry = entry;
	fatent->u.ent32_p = NULL;
}

static inline void fatent_brelse(struct fat_entry *fatent)
{
	int i;
	fatent->u.ent32_p = NULL;
	for (i = 0; i < fatent->nr_bhs; i++)
		brelse(fatent->bhs[i]);
	fatent->nr_bhs = 0;
	fatent->bhs[0] = fatent->bhs[1] = NULL;
	fatent->fat_inode = NULL;
}

#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
static inline int fatent_uptodate(struct fat_entry *fatent)
{
	if (fatent->nr_bhs == 2)
		return (buffer_uptodate(fatent->bhs[0]) &&
			buffer_uptodate(fatent->bhs[1]));
	return buffer_uptodate(fatent->bhs[0]);
}

static inline void fatent_unlock(struct msdos_sb_info *sbi,
				 struct fat_entry *fatent)
{
	if (sbi->options.batch_sync) {
		int i;
		for (i = 0; i < fatent->nr_bhs; i++)
			unlock_buffer(fatent->bhs[i]);
	}
}

static inline int fatent_lock(struct msdos_sb_info *sbi,
			      struct fat_entry *fatent)
{
	if (sbi->options.batch_sync) {
		int i;
		for (i = 0; i < fatent->nr_bhs; i++)
			lock_buffer(fatent->bhs[i]);
		if (!fatent_uptodate(fatent)) {
			fatent_unlock(sbi, fatent);
			return 0;
		}
	}
	return 1;
}
#endif

extern void fat_ent_access_init(struct super_block *sb);
extern int fat_ent_read(struct inode *inode, struct fat_entry *fatent,
			int entry);
extern int fat_ent_write(struct inode *inode, struct fat_entry *fatent,
			 int new, int wait);
extern int fat_ent_raw_read(struct super_block *sb, struct fat_entry *fatent,
			    int entry);
extern int fat_ent_raw_write(struct super_block *sb, struct fat_entry *fatent,
			     int new);
extern int fat_alloc_clusters(struct inode *inode, int *cluster,
			      int nr_cluster);
extern int fat_free_clusters(struct inode *inode, int cluster);
extern int fat_count_free_clusters(struct super_block *sb);

/* fat/file.c */
extern long fat_generic_ioctl(struct file *filp, unsigned int cmd,
			      unsigned long arg);
extern const struct file_operations fat_file_operations;
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
extern const struct file_operations fat_file_operations_nosync;
#endif
extern const struct inode_operations fat_file_inode_operations;
#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
extern struct inode_operations fat_symlink_inode_operations;
#endif
extern int fat_setattr(struct dentry * dentry, struct iattr * attr);
extern void fat_truncate_blocks(struct inode *inode, loff_t offset);
extern int fat_getattr(struct vfsmount *mnt, struct dentry *dentry,
		       struct kstat *stat);
extern int fat_file_fsync(struct file *file, int datasync);

/* fat/inode.c */
extern void fat_attach(struct inode *inode, loff_t i_pos);
extern void fat_detach(struct inode *inode);
extern struct inode *fat_iget(struct super_block *sb, loff_t i_pos);
extern struct inode *fat_build_inode(struct super_block *sb,
			struct msdos_dir_entry *de, loff_t i_pos);
extern int fat_sync_inode(struct inode *inode);
extern int fat_fill_super(struct super_block *sb, void *data, int silent,
			const struct inode_operations *fs_dir_inode_ops,
			int isvfat, void (*setup)(struct super_block *));

extern int fat_flush_inodes(struct super_block *sb, struct inode *i1,
		            struct inode *i2);
/* fat/misc.c */
extern void
__fat_fs_error(struct super_block *s, int report, const char *fmt, ...)
	__attribute__ ((format (printf, 3, 4))) __cold;
#define fat_fs_error(s, fmt, args...)		\
	__fat_fs_error(s, 1, fmt , ## args)
#define fat_fs_error_ratelimit(s, fmt, args...) \
	__fat_fs_error(s, __ratelimit(&MSDOS_SB(s)->ratelimit), fmt , ## args)
extern int fat_clusters_flush(struct super_block *sb);
extern int fat_chain_add(struct inode *inode, int new_dclus, int nr_cluster);
extern void fat_time_fat2unix(struct msdos_sb_info *sbi, struct timespec *ts,
			      __le16 __time, __le16 __date, u8 time_cs);
extern void fat_time_unix2fat(struct msdos_sb_info *sbi, struct timespec *ts,
			      __le16 *time, __le16 *date, u8 *time_cs);
extern int fat_sync_bhs(struct buffer_head **bhs, int nr_bhs);
#ifdef CONFIG_SNSC_FS_FAT_RELAX_SYNC
extern int fat_flush_bhs(struct buffer_head **bhs, int nr_bhs);
#endif

int fat_cache_init(void);
void fat_cache_destroy(void);

/* helper for printk */
typedef unsigned long long	llu;

#ifdef CONFIG_SNSC_FS_FAT_GC
/* fat/gc.c */
extern void fat_start_gc(struct super_block *);
extern void fat_stop_gc(struct super_block *);
extern void fat_gc_mark_cluster_valid(struct msdos_sb_info *, int);
extern int fat_gc_mark_valid_entries(struct inode *);
#endif

/* fat/namei_vfat.c */
extern int vfat_uni_strnicmp(const wchar_t *u1, const wchar_t *u2, int len);
extern int vfat_xlate_to_uni(const char *name, int len, char *outname,
			      int *longlen, int *outlen, int escape, int utf8,
			      struct nls_table *nls);

#ifdef CONFIG_SNSC_FS_FAT_TIMEZONE
/* fat/tz.c */
extern int fat_tz_set(struct tz_rule *rule, char *timezone);
extern void fat_tz_fat2unix(struct tz_rule *rule, u16 time, u16 date,
			    time_t *unix_date);
extern void fat_tz_unix2fat(struct tz_rule *rule, time_t unix_date,
			    __le16 *time, __le16 *date);
#endif

#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
static inline int fat_syncdir(struct inode *dir)
{
	struct super_block *sb = dir->i_sb;
	struct msdos_sb_info *sbi = MSDOS_SB(sb);
	int ret = 0;

	if (!IS_DIRSYNC(dir) && sbi->options.batch_sync)
		ret = sync_blockdev(sb->s_bdev);

	return ret;
}
#endif

#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
/*
 * vfat posix attr option "posix_attr" stuffs
 *
 * FEATURES
 *
 * Following attributes/modes are supported in posix attributea mapping
 * in VFAT.
 *
 *   - FileType
 * 	This supports following special files and it's attributes;
 * 		symbolic link,	block device node,
 * 		char device node, fifo,	socket
 * 	Regular files/dirs also may have POSIX attributes.
 *   - DeviceFile
 * 	Major and minor number would be held at ctime
 * 	and both values are limited  to 255.
 *   - Owner's User ID/Group ID
 * 	This can be used to distinguish root and others,
 * 	because this has just one bit width.
 * 	Value of UID/GID for non-root user will be taken from uid/gid
 * 	option on mounting. If nothing is specified, system uses
 *	(u16)-1 as last resort. That means change-uid may affect on gid.
 *   - Permission for Group/Other (rwx)
 * 	Those modes will be kept in ctime_cs.
 * 	Also permission modes for "others" will be
 * 	same as "group", due to lack of fields.
 *	That means set-group-mode may affect on other-mode.
 *	On the other hand, set-other-mode has no affect to group-mode.
 *
 *   - Permission for Owner (rwx)
 * 	These modes will be mapped to FAT attributes.
 * 	Just same as mapping under VFAT.
 *   - Others
 * 	no sticky, setgid nor setuid bits are not supported.
 *
 *ALGORITHM FOR MAPPING DECISION
 *   - Regular file/dir
 * 	To distinguish regular files/dirs, look if this fat dir
 * 	entry doesn't have ATTR_SYS, first. If it doesn't have
 * 	ATTR_SYS, then check if TYPE field (MSB 3bits) in ctime_cs
 * 	is equal to 7. If so, this regular file/dir is created and/or
 * 	modified under VFAT with "posix_attr". And posix attribute
 * 	mapping can be take place. Otherwise, conventional VFAT
 * 	attribute mapping is used.
 *
 *   - Special file
 * 	To distinguish special files, look if this fat dir entry
 * 	has ATTR_SYS, first. Also we need to check it not to have
 *	ATT_EXT.
 *	If it has ATTR_SYS, then check 1st. LSB bit in ctime_cs,
 *	referred as "special file flag".
 * 	If set,  this file is created under VFAT with "posix_attr".
 * 	Look up TYPE field to decide special file type.
 *
 * 	This special file detection method has some flaw to make
 * 	potential confusion. E.g. some system file created under
 *	dos/win may be treated as special file.  However in most case,
 *	user don't create system file under dos/win.
 *	To reduce possiblity of this confusion, system makes
 *	sure special files except symlink have size ZERO.
 *	For symlink, system checks it's size not to exceed page size
 *	and PATH_MAX.
 *
 *FAT DIR ENTRY FIELDS
 *
 *   - ctime_cs
 * 	    8bit byte
 * 	7 6 5 4 3 2 1 0
 * 	|===| | | | | |
 * 	TYPE  | | | | +- special file flag (valid if ATTR_SYS)
 * 	      | | | +--- User/Group ID(root or others)
 * 	      | | +----- !group X
 * 	      | +------- !group W
 * 	      +--------- !group R
 *
 * 	  special file flag
 * 		Indicate this entry has posix attribute mapping.
 * 		This field is valid for fat dir entry, which
 * 		have ATTR_SYS.
 *
 * 	  special file TYPE
 * 		val	type on VFS(val)	Description
 * 		------------------------------------------------
 * 		0 	(place folder for backward compat)
 * 		1 	DT_LNK (10)		symbolic link
 * 		2	DT_BLK (6)		block dev
 * 		3	DT_CHR (4)		char dev
 * 		4	DT_FIFO (1)		fifo
 * 		5	DT_SOCK	(12)		socket
 *
 * 		7*)	(reserved for DT_REG/DT_DIR)
 *
 * 		*)Value 7 is reserved for regular file/dir (DT_REG/DT_DIR).
 * 		Normally ctime_cs would have 0-199 value to stand for
 * 		up to 2sec. The value for DT_REG/DT_DIR is selected
 * 		to be over this range to distinguish if file was created
 * 		under POSIX_ATTR or not.
 *
 *   - attr
 * 	FAT attribute	(val)		mapped attribute
 * 	------------------------------------------------
 * 	ATTR_RO		0x01 		!owner W
 * 	ATTR_HIDDEN	0x02		!owner R
 * 	ATTR_SYS	0x04
 * 	ATTR_VOLUME	0x08
 * 	ATTR_DIR	0x10		DIR
 * 	ATTR_ARCH	0x20		!owner X
 *
 *   - ctime
 * 		16bit word
 * 	f e d c b a 9 8 7 6 5 4 3 2 1 0
 * 	|=============| |-------------|
 * 	  major		  minor
 *
 */

#define VFAT_POSIX_ATTR_VALID_MODE	(S_IFMT|S_IRWXU|S_IRWXG|S_IRWXO)

#define VFAT_CS_FMSK	0xe0
#define VFAT_CS_FSFT	5
#define VFAT_CS_FREG	0xe0

#define VFAT_CS_SPCF	0x01
#define VFAT_CS_UID	0x02
#define VFAT_CS_NXGRP	0x04
#define VFAT_CS_NWGRP	0x08
#define VFAT_CS_NRGRP	0x10

/* regular file/dir flag */
static inline int get_pxattr_regf(struct msdos_dir_entry *de)
{
	return (de->ctime_cs & VFAT_CS_FMSK) == VFAT_CS_FREG;
}
static inline void set_pxattr_regf(struct msdos_dir_entry *de, int val)
{
	val = val ? VFAT_CS_FMSK : 0;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_FMSK)));
}

/* file type */
static inline int get_pxattr_ftype(struct msdos_dir_entry *de)
{
	return ((de->ctime_cs & VFAT_CS_FMSK) >> VFAT_CS_FSFT);
}
static inline void set_pxattr_ftype(struct msdos_dir_entry *de, int val)
{
	val = (val  << VFAT_CS_FSFT) & VFAT_CS_FMSK;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_FMSK)));
}

/* special file flag */
static inline int get_pxattr_specf(struct msdos_dir_entry *de)
{
	return de->ctime_cs & VFAT_CS_SPCF;
}
static inline void set_pxattr_specf(struct msdos_dir_entry *de, int val)
{
	val = val ? VFAT_CS_SPCF : 0;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_SPCF)));
}

/* user r */
static inline int get_pxattr_ur(struct msdos_dir_entry *de)
{
	return !(de->attr & ATTR_HIDDEN);
}
static inline void set_pxattr_ur(struct msdos_dir_entry *de, int val)
{
	val = val ? 0 : ATTR_HIDDEN;
	de->attr = (val | (de->attr & ~ATTR_HIDDEN));
}

/* user w */
static inline int get_pxattr_uw(struct msdos_dir_entry *de)
{
	return !(de->attr & ATTR_RO);
}
static inline void set_pxattr_uw(struct msdos_dir_entry *de, int val)
{
	val = val ? 0 : ATTR_RO;
	de->attr = (val | (de->attr & ~ATTR_RO));
}

/* user x */
static inline int get_pxattr_ux(struct msdos_dir_entry *de)
{
	return !(de->attr & ATTR_ARCH);
}
static inline void set_pxattr_ux(struct msdos_dir_entry *de, int val)
{
	val = val ? 0 : ATTR_ARCH;
	de->attr = (val | (de->attr & ~ATTR_ARCH));
}

/* group r */
static inline int get_pxattr_gr(struct msdos_dir_entry *de)
{
	return !(de->ctime_cs & VFAT_CS_NRGRP);
}
static inline void set_pxattr_gr(struct msdos_dir_entry *de, int val)
{
	val = val ? 0 : VFAT_CS_NRGRP;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_NRGRP)));
}

/* group w */
static inline int get_pxattr_gw(struct msdos_dir_entry *de)
{
	return !(de->ctime_cs & VFAT_CS_NWGRP);
}
static inline void set_pxattr_gw(struct msdos_dir_entry *de, int val)
{
	val = val ? 0 : VFAT_CS_NWGRP;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_NWGRP)));
}

/* group x */
static inline int get_pxattr_gx(struct msdos_dir_entry *de)
{
	return !(de->ctime_cs & VFAT_CS_NXGRP);
}
static inline void set_pxattr_gx(struct msdos_dir_entry *de, int val)
{
	val = val ? 0 : VFAT_CS_NXGRP;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_NXGRP)));
}

/* user id */
static inline int get_pxattr_uid(struct msdos_dir_entry *de)
{
	return (de->ctime_cs & VFAT_CS_UID) != 0;
}
static inline void set_pxattr_uid(struct msdos_dir_entry *de, int val)
{
	val = val ? VFAT_CS_UID : 0;
	de->ctime_cs = (val | (de->ctime_cs & (~VFAT_CS_UID)));
}

/* driver major number */
static inline int get_pxattr_major(struct msdos_dir_entry *de)
{
	return ((le16_to_cpu(de->ctime) & 0xff00) >> 8);
}
static inline void set_pxattr_major(struct msdos_dir_entry *de, int val)
{
	val = (val & 0xff) << 8;
	de->ctime = cpu_to_le16((val | (le16_to_cpu(de->ctime) & 0x00ff)));
}

/* driver minor number */
static inline int get_pxattr_minor(struct msdos_dir_entry *de)
{
	return le16_to_cpu(de->ctime) & 0xff;
}
static inline void set_pxattr_minor(struct msdos_dir_entry *de, int val)
{
	val &= 0xff;
	de->ctime = cpu_to_le16(val | (le16_to_cpu(de->ctime) & 0xff00));
}
#endif

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
static inline int fat_check_disk(struct super_block *sb, int call_daemon)
{
	int changed;

	if (!MSDOS_SB(sb)->options.check_disk)
		return 0;

	changed = k3d_get_disk_change(sb->s_bdev->bd_disk, call_daemon);
	if (changed == 1 || changed == 2)
		return 1;

	return 0;
}
#endif
#endif /* !_FAT_H */
