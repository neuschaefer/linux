/*
 * Compressed rom filesystem for Linux.
 *
 * Copyright (C) 1999 Linus Torvalds.
 *
 * This file is released under the GPL.
 */

/*
 * These are the VFS interfaces to the compressed rom filesystem.
 * The actual compression is based on zlib, see the other files.
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/blkdev.h>
#include <linux/cramfs_fs.h>
#include <linux/slab.h>
#include <linux/cramfs_fs_sb.h>
#include <linux/buffer_head.h>
#include <linux/vfs.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>

#include <asm/uaccess.h>

extern unsigned char roothash[];

static const struct super_operations cramfs_ops;
static const struct inode_operations cramfs_dir_inode_operations;
static const struct file_operations cramfs_directory_operations;
static const struct address_space_operations cramfs_aops;
static int custom_pkg_sideload = 0;

static DEFINE_MUTEX(read_mutex);

#define SIZEOF_AIMAGE_HEADER            256
#define IMG_MAGIC                       0x41676d69
#define IMG_MAGIC2                      0x43634d52
#define IMG_TYPE_APPFS_CRAMFS           (0x0D)
#define IMG_TYPE_AUTH_SIGNATURE         (0x16)
#define IMG_TYPE_CRAMFS_AUTH            (0x101)



/* These macros may change in future, to provide better st_ino semantics. */
#define OFFSET(x)	((x)->i_ino)

static unsigned long cramino(const struct cramfs_inode *cino, unsigned int offset)
{
	if (!cino->offset)
		return offset + 1;
	if (!cino->size)
		return offset + 1;

	/*
	 * The file mode test fixes buggy mkcramfs implementations where
	 * cramfs_inode->offset is set to a non zero value for entries
	 * which did not contain data, like devices node and fifos.
	 */
	switch (cino->mode & S_IFMT) {
	case S_IFREG:
	case S_IFDIR:
	case S_IFLNK:
		return cino->offset << 2;
	default:
		break;
	}
	return offset + 1;
}

static struct inode *get_cramfs_inode(struct super_block *sb,
	const struct cramfs_inode *cramfs_inode, unsigned int offset)
{
	struct inode *inode;
	static struct timespec zerotime;

	inode = iget_locked(sb, cramino(cramfs_inode, offset));
	if (!inode)
		return ERR_PTR(-ENOMEM);
	if (!(inode->i_state & I_NEW))
		return inode;

	switch (cramfs_inode->mode & S_IFMT) {
	case S_IFREG:
		inode->i_fop = &generic_ro_fops;
		inode->i_data.a_ops = &cramfs_aops;
		break;
	case S_IFDIR:
		inode->i_op = &cramfs_dir_inode_operations;
		inode->i_fop = &cramfs_directory_operations;
		break;
	case S_IFLNK:
		inode->i_op = &page_symlink_inode_operations;
		inode->i_data.a_ops = &cramfs_aops;
		break;
	default:
		init_special_inode(inode, cramfs_inode->mode,
				old_decode_dev(cramfs_inode->size));
	}

	inode->i_mode = cramfs_inode->mode;
	if( CRAMFS_SB(sb)->flags & (MS_NODEV|MS_NOSUID)) {
		inode->i_mode |= S_ISDIR(cramfs_inode->mode) ? (S_IRGRP | S_IXGRP) : S_IRGRP;
	}
	/* Unless the file is a special file, it should not writable. */
	if (!S_ISBLK(inode->i_mode) && !S_ISCHR(inode->i_mode) && !S_ISFIFO(inode->i_mode) && !S_ISSOCK(inode->i_mode))
		inode->i_mode &= ~S_IWUGO;
	inode->i_uid = CRAMFS_SB(sb)->uid == 0 ? cramfs_inode->uid : CRAMFS_SB(sb)->uid;
	inode->i_gid = CRAMFS_SB(sb)->gid == 0 ? cramfs_inode->gid : CRAMFS_SB(sb)->gid;

	/* if the lower 2 bits are zero, the inode contains data */
	if (!(inode->i_ino & 3)) {
		inode->i_size = cramfs_inode->size;
		inode->i_blocks = (cramfs_inode->size - 1) / 512 + 1;
	}

	/* Struct copy intentional */
	inode->i_mtime = inode->i_atime = inode->i_ctime = zerotime;
	/* inode->i_nlink is left 1 - arguably wrong for directories,
	   but it's the best we can do without reading the directory
	   contents.  1 yields the right result in GNU find, even
	   without -noleaf option. */

	unlock_new_inode(inode);

	return inode;
}

/*
 * We have our own block cache: don't fill up the buffer cache
 * with the rom-image, because the way the filesystem is set
 * up the accesses should be fairly regular and cached in the
 * page cache and dentry tree anyway..
 *
 * This also acts as a way to guarantee contiguous areas of up to
 * BLKS_PER_BUF*PAGE_CACHE_SIZE, so that the caller doesn't need to
 * worry about end-of-buffer issues even when decompressing a full
 * page cache.
 */
#define READ_BUFFERS (2)
/* NEXT_BUFFER(): Loop over [0..(READ_BUFFERS-1)]. */
#define NEXT_BUFFER(_ix) ((_ix) ^ 1)

/*
 * BLKS_PER_BUF_SHIFT should be at least 2 to allow for "compressed"
 * data that takes up more space than the original and with unlucky
 * alignment.
 */
#define BLKS_PER_BUF_SHIFT	(2)
#define BLKS_PER_BUF		(1 << BLKS_PER_BUF_SHIFT)
#define BUFFER_SIZE		(BLKS_PER_BUF*PAGE_CACHE_SIZE)

static unsigned char read_buffers[READ_BUFFERS][BUFFER_SIZE];
static unsigned buffer_blocknr[READ_BUFFERS];
static struct super_block * buffer_dev[READ_BUFFERS];
static int next_buffer;

/*
 * Returns a pointer to a buffer containing at least LEN bytes of
 * filesystem starting at byte offset OFFSET into the filesystem.
 */
static void *cramfs_read(struct super_block *sb, unsigned int offset, unsigned int len)
{
	struct address_space *mapping = sb->s_bdev->bd_inode->i_mapping;
	struct page *pages[BLKS_PER_BUF];
	unsigned i, blocknr, buffer;
	unsigned long devsize;
	char *data;

	if (!len)
		return NULL;
	blocknr = offset >> PAGE_CACHE_SHIFT;
	offset &= PAGE_CACHE_SIZE - 1;

	/* Check if an existing buffer already has the data.. */
	for (i = 0; i < READ_BUFFERS; i++) {
		unsigned int blk_offset;

		if (buffer_dev[i] != sb)
			continue;
		if (blocknr < buffer_blocknr[i])
			continue;
		blk_offset = (blocknr - buffer_blocknr[i]) << PAGE_CACHE_SHIFT;
		blk_offset += offset;
		if (blk_offset + len > BUFFER_SIZE)
			continue;
		return read_buffers[i] + blk_offset;
	}

	devsize = mapping->host->i_size >> PAGE_CACHE_SHIFT;

	/* Ok, read in BLKS_PER_BUF pages completely first. */
	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct page *page = NULL;

		if (blocknr + i < devsize) {
			page = read_mapping_page_async(mapping, blocknr + i,
									NULL);
			/* synchronous error? */
			if (IS_ERR(page))
				page = NULL;
		}
		pages[i] = page;
	}

	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct page *page = pages[i];
		if (page) {
			wait_on_page_locked(page);
			if (!PageUptodate(page)) {
				/* asynchronous error */
				page_cache_release(page);
				pages[i] = NULL;
			}
		}
	}

	buffer = next_buffer;
	next_buffer = NEXT_BUFFER(buffer);
	buffer_blocknr[buffer] = blocknr;
	buffer_dev[buffer] = sb;

	data = read_buffers[buffer];
	for (i = 0; i < BLKS_PER_BUF; i++) {
		struct page *page = pages[i];
		if (page) {
			memcpy(data, kmap(page), PAGE_CACHE_SIZE);
			kunmap(page);
			page_cache_release(page);
		} else
			memset(data, 0, PAGE_CACHE_SIZE);
		data += PAGE_CACHE_SIZE;
	}
	return read_buffers[buffer] + offset;
}

static int cramfs_read_and_uncompress(void* pgdata, struct super_block *sb, unsigned int offset, unsigned int len)
{
	struct address_space *mapping = sb->s_bdev->bd_inode->i_mapping;
	struct page *pages[3] = {0};
	void* srcdata[3] = {0};
	int srclen[4] = {0};
	int fail, total;
	unsigned i, first_blocknr, last_blocknr, block_count, orig_offset = offset, orig_len = len;
	unsigned long devsize;

	if (!len)
		return 0;
	
	first_blocknr = offset >> PAGE_CACHE_SHIFT;
	last_blocknr = (offset + len - 1) >> PAGE_CACHE_SHIFT;
	block_count = last_blocknr - first_blocknr + 1;
	if (block_count > 3) {
		printk(KERN_ERR "cramfs: compressed data too long: %d\n", len);
		return 0;
	}

	offset &= PAGE_CACHE_SIZE - 1;
	devsize = mapping->host->i_size >> PAGE_CACHE_SHIFT;
	if (last_blocknr >= devsize) {
		printk(KERN_ERR "cramfs: access past end: %lu %lu\n", orig_offset, orig_len);
		return 0;
	}

	fail = 0;

	for (i=0; i < block_count; i++) {
		struct page *page = NULL;

		page = read_mapping_page_async(mapping, i + first_blocknr, NULL);
		/* synchronous error? */
		if (!page || IS_ERR(page)) {
			page = NULL;
			fail = 1;
		}
		pages[i] = page;
	}

	for (i = 0; i < block_count; i++) {
		struct page *page = pages[i];
		if (page) {
			wait_on_page_locked(page);
			if (!PageUptodate(page)) {
				/* asynchronous error */
				page_cache_release(page);
				pages[i] = NULL;
				fail = 1;
			}
		}
	}

	if (fail) {
		for (i = 0; i < block_count; i++) {
			if (pages[i])
				page_cache_release(pages[i]);
			else
				printk(KERN_ERR "cramfs: error reading page %lu\n", first_blocknr+i);
		}
		return 0;
	}

	for (i = 0; i < block_count; i++) {
		srcdata[i] = kmap(pages[i]) + offset;
		srclen[i] = PAGE_CACHE_SIZE - offset;
		if (srclen[i] > len) srclen[i] = len;
		len -= srclen[i];
		offset = 0;
	}
		
	total = cramfs_uncompress_block(pgdata, PAGE_CACHE_SIZE, srcdata, srclen);
	if (total < 0)
		printk(KERN_ERR "cramfs: %lu %d %d %d\n", first_blocknr, block_count, orig_offset, orig_len);

	for (i = 0; i < block_count; i++) {
		kunmap(pages[i]);
		page_cache_release(pages[i]);
	}

	return total;
}

static void cramfs_put_super(struct super_block *sb)
{
	kfree(sb->s_fs_info);
	sb->s_fs_info = NULL;
}

static int cramfs_remount(struct super_block *sb, int *flags, char *data)
{
	*flags |= MS_RDONLY;
	return 0;
}

static int cramfs_fill_super(struct super_block *sb, void *data, int silent)
{
	int i;
	struct cramfs_super super;
	unsigned long root_offset;
	struct cramfs_sb_info *sbi;
	struct inode *root;

	sb->s_flags |= MS_RDONLY;

	sbi = kzalloc(sizeof(struct cramfs_sb_info), GFP_KERNEL);
	if (!sbi)
		return -ENOMEM;
	sb->s_fs_info = sbi;

	/* Invalidate the read buffers on mount: think disk change.. */
	mutex_lock(&read_mutex);
	for (i = 0; i < READ_BUFFERS; i++)
		buffer_blocknr[i] = -1;

	/* Read the first block and get the superblock from it */
	memcpy(&super, cramfs_read(sb, 0, sizeof(super)), sizeof(super));
	mutex_unlock(&read_mutex);

	/* Do sanity checks on the superblock */
	if (super.magic != CRAMFS_MAGIC) {
		/* check for wrong endianess */
		if (super.magic == CRAMFS_MAGIC_WEND) {
			if (!silent)
				printk(KERN_ERR "cramfs: wrong endianess\n");
			goto out;
		}

		/* check at 512 byte offset */
		mutex_lock(&read_mutex);
		memcpy(&super, cramfs_read(sb, 512, sizeof(super)), sizeof(super));
		mutex_unlock(&read_mutex);
		if (super.magic != CRAMFS_MAGIC) {
			if (super.magic == CRAMFS_MAGIC_WEND && !silent)
				printk(KERN_ERR "cramfs: wrong endianess\n");
			else if (!silent)
				printk(KERN_ERR "cramfs: wrong magic\n");
			goto out;
		}
	}

	/* get feature flags first */
	if (super.flags & ~CRAMFS_SUPPORTED_FLAGS) {
		printk(KERN_ERR "cramfs: unsupported filesystem features\n");
		goto out;
	}

	/* Check that the root inode is in a sane state */
	if (!S_ISDIR(super.root.mode)) {
		printk(KERN_ERR "cramfs: root is not a directory\n");
		goto out;
	}
	/* correct strange, hard-coded permissions of mkcramfs */
	super.root.mode |= (S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	root_offset = super.root.offset << 2;
	if (super.flags & CRAMFS_FLAG_FSID_VERSION_2) {
		sbi->size=super.size;
		sbi->blocks=super.fsid.blocks;
		sbi->files=super.fsid.files;
	} else {
		sbi->size=1<<28;
		sbi->blocks=0;
		sbi->files=0;
	}
	sbi->magic=super.magic;
	sbi->flags=super.flags;
	if (root_offset == 0)
		printk(KERN_INFO "cramfs: empty filesystem");
	else if (!(super.flags & CRAMFS_FLAG_SHIFTED_ROOT_OFFSET) &&
		 ((root_offset != sizeof(struct cramfs_super)) &&
		  (root_offset != 512 + sizeof(struct cramfs_super))))
	{
		printk(KERN_ERR "cramfs: bad root offset %lu\n", root_offset);
		goto out;
	}

	/* Did the mounter specify an overriding uid/gid for all files? */
	if (data) {
		const char* param = (const char*) data;
		const char *p = strstr(param, "uid=");
		if (p)
			CRAMFS_SB(sb)->uid = simple_strtoul(p+4, 0, 0);
		p = strstr(param, "gid=");
		if (p)
			CRAMFS_SB(sb)->gid = simple_strtoul(p+4, 0, 0);
	}
 
	/* Set it all up.. */
	sb->s_op = &cramfs_ops;
	root = get_cramfs_inode(sb, &super.root, 0);
	if (IS_ERR(root))
		goto out;
	sb->s_root = d_alloc_root(root);
	if (!sb->s_root) {
		iput(root);
		goto out;
	}
	return 0;
out:
	kfree(sbi);
	sb->s_fs_info = NULL;
	return -EINVAL;
}

static int cramfs_statfs(struct dentry *dentry, struct kstatfs *buf)
{
	struct super_block *sb = dentry->d_sb;
	u64 id = huge_encode_dev(sb->s_bdev->bd_dev);

	buf->f_type = CRAMFS_MAGIC;
	buf->f_bsize = PAGE_CACHE_SIZE;
	buf->f_blocks = CRAMFS_SB(sb)->blocks;
	buf->f_bfree = 0;
	buf->f_bavail = 0;
	buf->f_files = CRAMFS_SB(sb)->files;
	buf->f_ffree = 0;
	buf->f_fsid.val[0] = (u32)id;
	buf->f_fsid.val[1] = (u32)(id >> 32);
	buf->f_namelen = CRAMFS_MAXPATHLEN;
	return 0;
}

/*
 * Read a cramfs directory entry.
 */
static int cramfs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct inode *inode = filp->f_path.dentry->d_inode;
	struct super_block *sb = inode->i_sb;
	char *buf;
	unsigned int offset;
	int copied;

	/* Offset within the thing. */
	offset = filp->f_pos;
	if (offset >= inode->i_size)
		return 0;
	/* Directory entries are always 4-byte aligned */
	if (offset & 3)
		return -EINVAL;

	buf = kmalloc(CRAMFS_MAXPATHLEN, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	copied = 0;
	while (offset < inode->i_size) {
		struct cramfs_inode *de;
		unsigned long nextoffset;
		char *name;
		ino_t ino;
		mode_t mode;
		int namelen, error;

		mutex_lock(&read_mutex);
		de = cramfs_read(sb, OFFSET(inode) + offset, sizeof(*de)+CRAMFS_MAXPATHLEN);
		name = (char *)(de+1);

		/*
		 * Namelengths on disk are shifted by two
		 * and the name padded out to 4-byte boundaries
		 * with zeroes.
		 */
		namelen = de->namelen << 2;
		memcpy(buf, name, namelen);
		ino = cramino(de, OFFSET(inode) + offset);
		mode = de->mode;
		mutex_unlock(&read_mutex);
		nextoffset = offset + sizeof(*de) + namelen;
		for (;;) {
			if (!namelen) {
				kfree(buf);
				return -EIO;
			}
			if (buf[namelen-1])
				break;
			namelen--;
		}
		error = filldir(dirent, buf, namelen, offset, ino, mode >> 12);
		if (error)
			break;

		offset = nextoffset;
		filp->f_pos = offset;
		copied++;
	}
	kfree(buf);
	return 0;
}

/*
 * Lookup and fill in the inode data..
 */
static struct dentry * cramfs_lookup(struct inode *dir, struct dentry *dentry, struct nameidata *nd)
{
	unsigned int offset = 0;
	struct inode *inode = NULL;
	int sorted;

	mutex_lock(&read_mutex);
	sorted = CRAMFS_SB(dir->i_sb)->flags & CRAMFS_FLAG_SORTED_DIRS;
	while (offset < dir->i_size) {
		struct cramfs_inode *de;
		char *name;
		int namelen, retval;
		int dir_off = OFFSET(dir) + offset;

		de = cramfs_read(dir->i_sb, dir_off, sizeof(*de)+CRAMFS_MAXPATHLEN);
		name = (char *)(de+1);

		/* Try to take advantage of sorted directories */
		if (sorted && (dentry->d_name.name[0] < name[0]))
			break;

		namelen = de->namelen << 2;
		offset += sizeof(*de) + namelen;

		/* Quick check that the name is roughly the right length */
		if (((dentry->d_name.len + 3) & ~3) != namelen)
			continue;

		for (;;) {
			if (!namelen) {
				inode = ERR_PTR(-EIO);
				goto out;
			}
			if (name[namelen-1])
				break;
			namelen--;
		}
		if (namelen != dentry->d_name.len)
			continue;
		retval = memcmp(dentry->d_name.name, name, namelen);
		if (retval > 0)
			continue;
		if (!retval) {
			inode = get_cramfs_inode(dir->i_sb, de, dir_off);
			break;
		}
		/* else (retval < 0) */
		if (sorted)
			break;
	}
out:
	mutex_unlock(&read_mutex);
	if (IS_ERR(inode))
		return ERR_CAST(inode);
	d_add(dentry, inode);
	return NULL;
}

static int cramfs_readpage(struct file *file, struct page * page)
{
	struct inode *inode = page->mapping->host;
	u32 maxblock;
	int bytes_filled;
	void *pgdata;

	maxblock = (inode->i_size + PAGE_CACHE_SIZE - 1) >> PAGE_CACHE_SHIFT;
	bytes_filled = 0;
	pgdata = kmap(page);

	if (page->index < maxblock) {
		struct super_block *sb = inode->i_sb;
		u32 blkptr_offset = OFFSET(inode) + page->index*4;
		u32 start_offset, compr_len;

		start_offset = OFFSET(inode) + maxblock*4;
		mutex_lock(&read_mutex);
		if (page->index)
			start_offset = *(u32 *) cramfs_read(sb, blkptr_offset-4,
				4);
		compr_len = (*(u32 *) cramfs_read(sb, blkptr_offset, 4) -
			start_offset);
		mutex_unlock(&read_mutex);

		if (compr_len == 0)
			; /* hole */
		else if (unlikely(compr_len > (PAGE_CACHE_SIZE << 1))) {
			pr_err("cramfs: bad compressed blocksize %u\n",
				compr_len);
			goto err;
		} else {
			mutex_lock(&read_mutex);
			bytes_filled = cramfs_read_and_uncompress(pgdata,
				 sb, start_offset, compr_len);
			mutex_unlock(&read_mutex);
			if (unlikely(bytes_filled < 0))
				goto err;
		}
	}

	memset(pgdata + bytes_filled, 0, PAGE_CACHE_SIZE - bytes_filled);
	flush_dcache_page(page);
	kunmap(page);
	SetPageUptodate(page);
	unlock_page(page);
	return 0;

err:
	kunmap(page);
	ClearPageUptodate(page);
	SetPageError(page);
	unlock_page(page);
	return 0;
}

static const struct address_space_operations cramfs_aops = {
	.readpage = cramfs_readpage
};

/*
 * Our operations:
 */

/*
 * A directory can only readdir
 */
static const struct file_operations cramfs_directory_operations = {
	.llseek		= generic_file_llseek,
	.read		= generic_read_dir,
	.readdir	= cramfs_readdir,
};

static const struct inode_operations cramfs_dir_inode_operations = {
	.lookup		= cramfs_lookup,
};

static const struct super_operations cramfs_ops = {
	.put_super	= cramfs_put_super,
	.remount_fs	= cramfs_remount,
	.statfs		= cramfs_statfs,
};

static struct dentry *cramfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, cramfs_fill_super);
}

/* Yet another partial definition of aimage_header_t. */
typedef struct {
    unsigned int v1, v2, magic, magic2,
                 re, pl, aimage_type, data_length,
                 dl, data_start_offset; } aimage_header_t;

static int aimage_field(void *vp, int user_bufs, int offset, unsigned int *p_value)
{
	if (user_bufs) {
		aimage_header_t __user *up = vp;
		unsigned char __user *ufield = ((unsigned char __user *)up) + offset;
		if (copy_from_user(p_value, ufield, sizeof(unsigned int)))
			return -EFAULT;
	} else {
		unsigned char *field = ((unsigned char *)vp) + offset;
		*p_value = *(unsigned int*) field;
	}
	return 0;
}

static off_t file_read(struct file *file, void *data, int datalen, loff_t pos)
{
	return file->f_op->read(file, data, datalen, &pos);
}

/* Read authdata (MerkleHeader) from an open file descriptor. */
static int cramfs_get_authdata2(struct file *file, unsigned int wtype, void *data, int datalen, void *sbuf, int sbuflen, int user_bufs)
{
	off_t pos;
	unsigned int data_length;
	unsigned int aimage_type;
	unsigned int data_start_offset;
	unsigned int magic, magic2;
	int partition_offset = -1;
	int err;

	for (pos = 0; ;  pos += data_length) {
		int nread = file_read(file, sbuf, SIZEOF_AIMAGE_HEADER, pos);
		if (nread != SIZEOF_AIMAGE_HEADER) {
			printk(KERN_ERR "auth read error [a] %d/%d\n", nread, SIZEOF_AIMAGE_HEADER);
			return -EIO;
		}

		#define GET_AIMAGE_FIELD(_name) \
			err = aimage_field(sbuf, user_bufs, offsetof(aimage_header_t, _name), &_name); \
			if (err < 0) return err;

		GET_AIMAGE_FIELD(magic);
		GET_AIMAGE_FIELD(magic2);
		if (magic != IMG_MAGIC || magic2 != IMG_MAGIC2) {
			printk(KERN_ERR "invalid magic at %lx: %x %x\n", (long) pos, magic, magic2);
			return -EINVAL;
		}

		GET_AIMAGE_FIELD(aimage_type);
		GET_AIMAGE_FIELD(data_length);
		GET_AIMAGE_FIELD(data_start_offset);
		if (partition_offset == -1) {
			/* We haven't found wtype yet. */
			if (aimage_type == wtype) { /* Here it is. */
				partition_offset = pos + data_start_offset;
			}
		} else if (aimage_type == IMG_TYPE_CRAMFS_AUTH) {
			/* Read the Merkle header. */
			nread = file_read(file, data, datalen, pos + data_start_offset);
			if (nread != datalen) {
				printk(KERN_ERR "auth read error [c] %d/%d\n", nread, datalen);
				return -EIO;
			}
			break;
		}
	}
	return partition_offset;
}

int cramfs_get_authdata(struct file *file, unsigned int wtype, void *data, int datalen, void *sbuf, int sbuflen)
{
	int user_bufs = (sbuf != NULL);
	int sbufsize = CRAMFS_SUPER_SIZE;
	struct hash_desc desc;
	struct scatterlist sg;
	unsigned char const *stored_hash;
	unsigned char calc_hash[16];
	unsigned digestsize;
	void *udata;
	int position;
	int err;

	if (!user_bufs) {
		sbuf = kmalloc(sbufsize, GFP_KERNEL);
		sbuflen = sbufsize;
	}
	if (sbuflen < sbufsize)
		return -EINVAL;
	position = cramfs_get_authdata2(file, wtype, data, datalen, sbuf, sbuflen, user_bufs);
	if (!user_bufs)
		kfree(sbuf);
	if (position < 0)
		return position;

	desc.flags = 0;
	desc.tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(desc.tfm))
		return PTR_ERR(desc.tfm);
	if (!user_bufs) {
		udata = data;
	} else {
		udata = kmalloc(datalen, GFP_KERNEL);
		if (copy_from_user(udata, data, datalen)) {
			kfree(udata);
			return -EFAULT;
		}
	}
	err = crypto_hash_init(&desc);
	if (err < 0)
		return err;
	digestsize = crypto_hash_digestsize(desc.tfm);
	BUG_ON(digestsize != sizeof(calc_hash));
	sg_init_table(&sg, 1);
	sg_init_one(&sg, udata, datalen);
	crypto_hash_update(&desc, &sg, datalen);
	crypto_hash_final(&desc, calc_hash);
	if (user_bufs)
		kfree(udata);
	crypto_free_hash(desc.tfm);

	if (wtype == IMG_TYPE_APPFS_CRAMFS)
		stored_hash = &roothash[sizeof(calc_hash)];
	else
		stored_hash = &roothash[0];
	if (memcmp(stored_hash, calc_hash, sizeof(calc_hash))) {
		if ((strncmp(saved_command_line, "dev=1 ", 6) && !custom_pkg_sideload) || wtype != IMG_TYPE_APPFS_CRAMFS)
			return -EKEYREJECTED;
        printk("ignore roothash digest failure\n");
    }
	return position;
}
EXPORT_SYMBOL(cramfs_get_authdata);

static struct file_system_type cramfs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "cramfs",
	.mount		= cramfs_mount,
	.kill_sb	= kill_block_super,
	.get_authdata	= cramfs_get_authdata,
	.fs_flags	= FS_REQUIRES_DEV,
};

static int __init init_cramfs_fs(void)
{
	int rv;

	rv = cramfs_uncompress_init();
	if (rv < 0)
		return rv;
	rv = register_filesystem(&cramfs_fs_type);
	if (rv < 0)
		cramfs_uncompress_exit();
	return rv;
}

static void __exit exit_cramfs_fs(void)
{
	cramfs_uncompress_exit();
	unregister_filesystem(&cramfs_fs_type);
}

static int __init setup_custom_pkg_sideload(char *str)
{
	custom_pkg_sideload = (*str == '1');
	return 1;
}
__setup("custom_pkg_sideload=", setup_custom_pkg_sideload);

module_init(init_cramfs_fs)
module_exit(exit_cramfs_fs)
MODULE_LICENSE("GPL");
