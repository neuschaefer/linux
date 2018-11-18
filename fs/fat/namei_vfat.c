/*
 *  linux/fs/vfat/namei.c
 *
 *  Written 1992,1993 by Werner Almesberger
 *
 *  Windows95/Windows NT compatible extended MSDOS filesystem
 *    by Gordon Chaffee Copyright (C) 1995.  Send bug reports for the
 *    VFAT filesystem to <chaffee@cs.berkeley.edu>.  Specify
 *    what file operation caused you trouble and if you can duplicate
 *    the problem, send a script that demonstrates it.
 *
 *  Short name translation 1999, 2001 by Wolfram Pienkoss <wp@bszh.de>
 *
 *  Support Multibyte characters and cleanup by
 *				OGAWA Hirofumi <hirofumi@mail.parknet.co.jp>
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <linux/buffer_head.h>
#include <linux/namei.h>
#include "fat.h"

#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
#include <linux/fs.h>
#endif

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
DEFINE_SPINLOCK(vfat_cmpu_lock);
static wchar_t uname_buf1[260], uname_buf2[260];
#endif

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
static int xlate_to_uni(const unsigned char *name, int len,
			unsigned char *outname, int *longlen, int *outlen,
			int escape, int utf8, struct nls_table *nls);
#endif

#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
#define	GET_INODE_FILE_TYPE(x)	(((x)->i_mode & S_IFMT) >> 12)
#define	GET_INODE_USER_ID(x)	((x)->i_uid ? 1 : 0)
#define	GET_INODE_DRV_MAJOR(x)	((x)->i_rdev >> MINORBITS)
#define	GET_INODE_DRV_MINOR(x)	((x)->i_rdev & MINORMASK)

static unsigned short  filetype_table[] = {
	DT_REG, /* place folder for backward compat */
	DT_LNK,
	DT_BLK,
	DT_CHR,
	DT_FIFO,
	DT_SOCK,
	0
};

/**
 * is_vfat_posix_symlink - check if posix file type from VFAT dir is symlink
 *
 * Returns
 *	        0 ... is not symlink or don't have posix attributes
 *	otherwise ... is symlink
 */
static int is_vfat_posix_symlink(struct inode *inode, struct msdos_dir_entry *dentry)
{
	if ((dentry->attr & ATTR_SYS) && get_pxattr_specf(dentry)) {
		int size = le16_to_cpu(dentry->size);
		if ((size <= PATH_MAX) && (size <= PAGE_SIZE))
			return filetype_table[get_pxattr_ftype(dentry)] == DT_LNK;
	}
	return 0;
}

/*
 * get_vfat_posix_attr - Retrieve posix attributes from VFAT dir entry
 *
 * Returns
 *	 0 ... posix_attr are get
 *	-1 ... posix_attr are not get
 */
static
int get_vfat_posix_attr(struct inode *inode, struct msdos_dir_entry *dentry)
{
	int px_uid, px_gid;
	struct msdos_sb_info *sbi;
	int ftype;
	int umode, gmode, omode;

	if (!(inode && dentry) || IS_ERR(inode)) goto not_get;
	sbi = MSDOS_SB(inode->i_sb);
	if ((!sbi->options.posix_attr) || (dentry->attr == ATTR_EXT) ||
	    (dentry->attr & ATTR_VOLUME)) goto not_get;

	/* File type : 0xF000 : 12 */
	ftype = -1;
	if (!(dentry->attr & ATTR_SYS) || (dentry->attr & ATTR_DIR)) {
		if (get_pxattr_regf(dentry))
			ftype = (dentry->attr & ATTR_DIR) ? DT_DIR : DT_REG;
	} else if (get_pxattr_specf(dentry)) {
		int size = le16_to_cpu(dentry->size);
		ftype = filetype_table[get_pxattr_ftype(dentry)];
		if (ftype == DT_LNK) {
			if ((size > PATH_MAX) || (size > PAGE_SIZE)) ftype = -1;
		} else {
			if (size) ftype = -1;
		}
	}
	if (ftype == -1)
		goto not_get;
	inode->i_mode = ftype << 12;
	inode->i_ctime = inode->i_mtime;

	/* User  : 0x01C0) : 6 */
	/* Group : 0x0038) : 3 */
	/* Other : 0x0007 */
	umode = (get_pxattr_ur(dentry) ? S_IRUSR : 0) |
		(get_pxattr_uw(dentry) ? S_IWUSR : 0) |
		(get_pxattr_ux(dentry) ? S_IXUSR : 0);
	gmode = (get_pxattr_gr(dentry) ? S_IRGRP : 0) |
		(get_pxattr_gw(dentry) ? S_IWGRP : 0) |
		(get_pxattr_gx(dentry) ? S_IXGRP : 0);
	omode = gmode >> 3;
	inode->i_mode |= (umode | gmode | omode);

	/* User & Group ID */
	px_uid = sbi->options.fs_uid ? sbi->options.fs_uid : (u16)-1;
	px_gid = sbi->options.fs_gid ? sbi->options.fs_gid : (u16)-1;
	inode->i_uid = get_pxattr_uid(dentry) ?  px_uid : 0;
	inode->i_gid = get_pxattr_uid(dentry) ?  px_gid : 0;

	/* Special file */
	if ((ftype==DT_BLK) || (ftype==DT_CHR)) {
		inode->i_rdev = ((get_pxattr_major(dentry) << MINORBITS) |
				  get_pxattr_minor(dentry));
		inode->i_mode &= ~S_IFMT;
		inode->i_mode |= (ftype == DT_BLK) ? S_IFBLK : S_IFCHR;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
	} else if ((ftype==DT_FIFO) || (ftype==DT_SOCK)) {
		inode->i_mode &= ~S_IFMT;
		inode->i_mode |= (ftype == DT_FIFO) ? S_IFIFO : S_IFSOCK;
		init_special_inode(inode, inode->i_mode, inode->i_rdev);
	}
	return 0;
not_get:
	return -1;

}

/**
 * set_vfat_posix_attr - set posix attributes to VFAT dir entry
 *
 * Returns
 *	 0 ... posix_attr are set
 *	-1 ... posix_attr are not set
 */
static int set_vfat_posix_attr(struct msdos_dir_entry *dentry, struct inode *inode)
{
	int     ftype;
	int     iftype;
	int	mode;

	if (!(inode && dentry) || IS_ERR(inode)) goto not_set;
	if (!MSDOS_SB(inode->i_sb)->options.posix_attr) goto not_set;

	/* File type */
	iftype = GET_INODE_FILE_TYPE(inode);
	switch (iftype) {
	case DT_DIR:
		dentry->attr |= ATTR_DIR;
		/* fall through */
	case DT_REG:
		set_pxattr_regf(dentry, 1);
		break;
	default:
		for(ftype=0; filetype_table[ftype]; ftype++){
			if (filetype_table[ftype] == iftype)
				break;
		}
		if (!filetype_table[ftype]) goto not_set;
		/* mark posix attr for special file */
		dentry->attr |= ATTR_SYS;
		set_pxattr_specf(dentry, 1);
		set_pxattr_ftype(dentry, ftype);
		break;
	}
	/* Permissions for Owner */
	mode = inode->i_mode;
	set_pxattr_ur(dentry, mode & S_IRUSR);
	set_pxattr_uw(dentry, mode & S_IWUSR);
	set_pxattr_ux(dentry, mode & S_IXUSR);
	/* Permissions for Group/Others */
	set_pxattr_gr(dentry, mode & S_IRGRP);
	set_pxattr_gw(dentry, mode & S_IWGRP);
	set_pxattr_gx(dentry, mode & S_IXGRP);
	/* User ID */
	set_pxattr_uid(dentry, GET_INODE_USER_ID(inode));

	/* Deivce number */
	if ((iftype==DT_BLK) || (iftype==DT_CHR)) {
		set_pxattr_major(dentry, GET_INODE_DRV_MAJOR(inode));
		set_pxattr_minor(dentry, GET_INODE_DRV_MINOR(inode));
	}
	return 0;
not_set:
	return -1;
}
#endif

/*
 * If new entry was created in the parent, it could create the 8.3
 * alias (the shortname of logname).  So, the parent may have the
 * negative-dentry which matches the created 8.3 alias.
 *
 * If it happened, the negative dentry isn't actually negative
 * anymore.  So, drop it.
 */
static int vfat_revalidate_shortname(struct dentry *dentry)
{
	int ret = 1;
	spin_lock(&dentry->d_lock);
	if (dentry->d_time != dentry->d_parent->d_inode->i_version)
		ret = 0;
	spin_unlock(&dentry->d_lock);
	return ret;
}

static int vfat_revalidate(struct dentry *dentry, struct nameidata *nd)
{
	if (nd && nd->flags & LOOKUP_RCU)
		return -ECHILD;

	/* This is not negative dentry. Always valid. */
	if (dentry->d_inode)
		return 1;
	return vfat_revalidate_shortname(dentry);
}

static int vfat_revalidate_ci(struct dentry *dentry, struct nameidata *nd)
{
	if (nd && nd->flags & LOOKUP_RCU)
		return -ECHILD;

	/*
	 * This is not negative dentry. Always valid.
	 *
	 * Note, rename() to existing directory entry will have ->d_inode,
	 * and will use existing name which isn't specified name by user.
	 *
	 * We may be able to drop this positive dentry here. But dropping
	 * positive dentry isn't good idea. So it's unsupported like
	 * rename("filename", "FILENAME") for now.
	 */
	if (dentry->d_inode)
		return 1;

	/*
	 * This may be nfsd (or something), anyway, we can't see the
	 * intent of this. So, since this can be for creation, drop it.
	 */
	if (!nd)
		return 0;

	/*
	 * Drop the negative dentry, in order to make sure to use the
	 * case sensitive name which is specified by user if this is
	 * for creation.
	 */
	if (!(nd->flags & (LOOKUP_CONTINUE | LOOKUP_PARENT))) {
		if (nd->flags & (LOOKUP_CREATE | LOOKUP_RENAME_TARGET))
			return 0;
	}

	return vfat_revalidate_shortname(dentry);
}

/* returns the length of a struct qstr, ignoring trailing dots */
static unsigned int __vfat_striptail_len(unsigned int len, const char *name)
{
	while (len && name[len - 1] == '.')
		len--;
	return len;
}

static unsigned int vfat_striptail_len(const struct qstr *qstr)
{
	return __vfat_striptail_len(qstr->len, qstr->name);
}

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
static wchar_t vfat_uni_tolower(wchar_t u)
{
	if (u >= 0x100)
		return u;
	return (wchar_t)tolower((unsigned char)u);
}

int vfat_uni_strnicmp(const wchar_t *u1, const wchar_t *u2, int len)
{
	while(len--)
		if (vfat_uni_tolower(*u1++) != vfat_uni_tolower(*u2++))
			return 1;
	return 0;
}
#endif

/*
 * Compute the hash for the vfat name corresponding to the dentry.
 * Note: if the name is invalid, we leave the hash code unchanged so
 * that the existing dentry can be used. The vfat fs routines will
 * return ENOENT or EINVAL as appropriate.
 */
static int vfat_hash(const struct dentry *dentry, const struct inode *inode,
		struct qstr *qstr)
{
	qstr->hash = full_name_hash(qstr->name, vfat_striptail_len(qstr));
	return 0;
}

/*
 * Compute the hash for the vfat name corresponding to the dentry.
 * Note: if the name is invalid, we leave the hash code unchanged so
 * that the existing dentry can be used. The vfat fs routines will
 * return ENOENT or EINVAL as appropriate.
 */
static int vfat_hashi(const struct dentry *dentry, const struct inode *inode,
		struct qstr *qstr)
{
	struct nls_table *t = MSDOS_SB(dentry->d_sb)->nls_io;
	const unsigned char *name;
	unsigned int len;
	unsigned long hash;

	name = qstr->name;
	len = vfat_striptail_len(qstr);

	hash = init_name_hash();
	while (len--)
		hash = partial_name_hash(nls_tolower(t, *name++), hash);
	qstr->hash = end_name_hash(hash);

	return 0;
}

/*
 * Case insensitive compare of two vfat names.
 */
static int vfat_cmpi(const struct dentry *parent, const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	struct nls_table *t = MSDOS_SB(parent->d_sb)->nls_io;
	unsigned int alen, blen;

	/* A filename cannot end in '.' or we treat it like it has none */
	alen = vfat_striptail_len(name);
	blen = __vfat_striptail_len(len, str);
	if (alen == blen) {
		if (nls_strnicmp(t, name->name, str, alen) == 0)
			return 0;
	}
	return 1;
}

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
static int vfat_cmpiu(const struct dentry *parent, const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	struct nls_table *t = MSDOS_SB(parent->d_inode->i_sb)->nls_io;
	int alen, blen, aulen, aunilen, bulen, bunilen;
	int utf8 = MSDOS_SB(parent->d_inode->i_sb)->options.utf8;
	int uni_xlate = MSDOS_SB(parent->d_inode->i_sb)->options.unicode_xlate;
	int matched = -1;

	/* A filename cannot end in '.' or we treat it like it has none */
	alen = name->len;
	blen = len;
	while (alen && name->name[alen-1] == '.')
		alen--;
	while (blen && str[blen-1] == '.')
		blen--;

	spin_lock(&vfat_cmpu_lock);
	if (xlate_to_uni(name->name, alen, (char *)uname_buf1, &aulen, &aunilen,
			 uni_xlate, utf8, t) < 0)
		goto end;
	if (xlate_to_uni(str, blen, (char *)uname_buf2, &bulen, &bunilen,
			 uni_xlate, utf8, t) < 0)
		goto end;

	if (aulen == bulen) {
		if (vfat_uni_strnicmp(uname_buf1, uname_buf2, aulen) == 0){
			matched = 0;
			goto end;
		}
	}
	matched = 1;
end:
	spin_unlock(&vfat_cmpu_lock);
	return matched;
}
#endif

/*
 * Case sensitive compare of two vfat names.
 */
static int vfat_cmp(const struct dentry *parent, const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	unsigned int alen, blen;

	/* A filename cannot end in '.' or we treat it like it has none */
	alen = vfat_striptail_len(name);
	blen = __vfat_striptail_len(len, str);
	if (alen == blen) {
		if (strncmp(name->name, str, alen) == 0)
			return 0;
	}
	return 1;
}

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
static int vfat_cmpu(const struct dentry *parent, const struct inode *pinode,
		const struct dentry *dentry, const struct inode *inode,
		unsigned int len, const char *str, const struct qstr *name)
{
	struct nls_table *t = MSDOS_SB(parent->d_inode->i_sb)->nls_io;
	int alen, blen, aulen, aunilen, bulen, bunilen;
	int utf8 = MSDOS_SB(parent->d_inode->i_sb)->options.utf8;
	int uni_xlate = MSDOS_SB(parent->d_inode->i_sb)->options.unicode_xlate;
	int matched = -1;

	/* A filename cannot end in '.' or we treat it like it has none */
	alen = name->len;
	blen = len;
	while (alen && name->name[alen-1] == '.')
		alen--;
	while (blen && str[blen-1] == '.')
		blen--;

	spin_lock(&vfat_cmpu_lock);
	if (xlate_to_uni(name->name, alen, (char *)uname_buf1, &aulen, &aunilen,
			 uni_xlate, utf8, t) < 0)
		goto end;
	if (xlate_to_uni(str, blen, (char *)uname_buf2, &bulen, &bunilen,
			 uni_xlate, utf8, t) < 0)
		goto end;

	if (aulen == bulen) {
		if (strncmp((char *)uname_buf1, (char *)uname_buf2,
			    aulen * sizeof(wchar_t)) == 0){
			matched = 0;
			goto end;
		}
	}
	matched = 1;
end:
	spin_unlock(&vfat_cmpu_lock);
	return matched;
}
#endif

static const struct dentry_operations vfat_ci_dentry_ops = {
	.d_revalidate	= vfat_revalidate_ci,
	.d_hash		= vfat_hashi,
	.d_compare	= vfat_cmpi,
};

static const struct dentry_operations vfat_dentry_ops = {
	.d_revalidate	= vfat_revalidate,
	.d_hash		= vfat_hash,
	.d_compare	= vfat_cmp,
};

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
static struct dentry_operations vfat_ciu_dentry_ops = {
	.d_revalidate	= vfat_revalidate_ci,
	.d_hash		= vfat_hashi,
	.d_compare	= vfat_cmpiu,
};

static struct dentry_operations vfat_cu_dentry_ops = {
	.d_revalidate	= vfat_revalidate,
	.d_hash		= vfat_hash,
	.d_compare	= vfat_cmpu,
};
#endif

/* Characters that are undesirable in an MS-DOS file name */

static inline wchar_t vfat_bad_char(wchar_t w)
{
	return (w < 0x0020)
	    || (w == '*') || (w == '?') || (w == '<') || (w == '>')
	    || (w == '|') || (w == '"') || (w == ':') || (w == '/')
	    || (w == '\\');
}

static inline wchar_t vfat_replace_char(wchar_t w)
{
	return (w == '[') || (w == ']') || (w == ';') || (w == ',')
	    || (w == '+') || (w == '=');
}

static wchar_t vfat_skip_char(wchar_t w)
{
	return (w == '.') || (w == ' ');
}

static inline int vfat_is_used_badchars(const wchar_t *s, int len)
{
	int i;

	for (i = 0; i < len; i++)
		if (vfat_bad_char(s[i]))
			return -EINVAL;

	if (s[i - 1] == ' ') /* last character cannot be space */
		return -EINVAL;

	return 0;
}

static int vfat_find_form(struct inode *dir, unsigned char *name)
{
	struct fat_slot_info sinfo;
	int err = fat_scan(dir, name, &sinfo);
	if (err)
		return -ENOENT;
	brelse(sinfo.bh);
	return 0;
}

/*
 * 1) Valid characters for the 8.3 format alias are any combination of
 * letters, uppercase alphabets, digits, any of the
 * following special characters:
 *     $ % ' ` - @ { } ~ ! # ( ) & _ ^
 * In this case Longfilename is not stored in disk.
 *
 * WinNT's Extension:
 * File name and extension name is contain uppercase/lowercase
 * only. And it is expressed by CASE_LOWER_BASE and CASE_LOWER_EXT.
 *
 * 2) File name is 8.3 format, but it contain the uppercase and
 * lowercase char, muliti bytes char, etc. In this case numtail is not
 * added, but Longfilename is stored.
 *
 * 3) When the one except for the above, or the following special
 * character are contained:
 *        .   [ ] ; , + =
 * numtail is added, and Longfilename must be stored in disk .
 */
struct shortname_info {
	unsigned char lower:1,
		      upper:1,
		      valid:1;
};
#define INIT_SHORTNAME_INFO(x)	do {		\
	(x)->lower = 1;				\
	(x)->upper = 1;				\
	(x)->valid = 1;				\
} while (0)

static inline int to_shortname_char(struct nls_table *nls,
				    unsigned char *buf, int buf_size,
				    wchar_t *src, struct shortname_info *info)
{
	int len;

	if (vfat_skip_char(*src)) {
		info->valid = 0;
		return 0;
	}
	if (vfat_replace_char(*src)) {
		info->valid = 0;
		buf[0] = '_';
		return 1;
	}

	len = nls->uni2char(*src, buf, buf_size);
	if (len <= 0) {
		info->valid = 0;
		buf[0] = '_';
		len = 1;
	} else if (len == 1) {
		unsigned char prev = buf[0];

		if (buf[0] >= 0x7F) {
			info->lower = 0;
			info->upper = 0;
		}

		buf[0] = nls_toupper(nls, buf[0]);
		if (isalpha(buf[0])) {
			if (buf[0] == prev)
				info->lower = 0;
			else
				info->upper = 0;
		}
	} else {
		info->lower = 0;
		info->upper = 0;
	}

	return len;
}

/*
 * Given a valid longname, create a unique shortname.  Make sure the
 * shortname does not exist
 * Returns negative number on error, 0 for a normal
 * return, and 1 for valid shortname
 */
static int vfat_create_shortname(struct inode *dir, struct nls_table *nls,
				 wchar_t *uname, int ulen,
				 unsigned char *name_res, unsigned char *lcase)
{
	struct fat_mount_options *opts = &MSDOS_SB(dir->i_sb)->options;
	wchar_t *ip, *ext_start, *end, *name_start;
	unsigned char base[9], ext[4], buf[5], *p;
	unsigned char charbuf[NLS_MAX_CHARSET_SIZE];
	int chl, chi;
	int sz = 0, extlen, baselen, i, numtail_baselen, numtail2_baselen;
	int is_shortname;
	struct shortname_info base_info, ext_info;

	is_shortname = 1;
	INIT_SHORTNAME_INFO(&base_info);
	INIT_SHORTNAME_INFO(&ext_info);

	/* Now, we need to create a shortname from the long name */
	ext_start = end = &uname[ulen];
	while (--ext_start >= uname) {
		if (*ext_start == 0x002E) {	/* is `.' */
			if (ext_start == end - 1) {
				sz = ulen;
				ext_start = NULL;
			}
			break;
		}
	}

	if (ext_start == uname - 1) {
		sz = ulen;
		ext_start = NULL;
	} else if (ext_start) {
		/*
		 * Names which start with a dot could be just
		 * an extension eg. "...test".  In this case Win95
		 * uses the extension as the name and sets no extension.
		 */
		name_start = &uname[0];
		while (name_start < ext_start) {
			if (!vfat_skip_char(*name_start))
				break;
			name_start++;
		}
		if (name_start != ext_start) {
			sz = ext_start - uname;
			ext_start++;
		} else {
			sz = ulen;
			ext_start = NULL;
		}
	}

	numtail_baselen = 6;
	numtail2_baselen = 2;
	for (baselen = i = 0, p = base, ip = uname; i < sz; i++, ip++) {
		chl = to_shortname_char(nls, charbuf, sizeof(charbuf),
					ip, &base_info);
		if (chl == 0)
			continue;

		if (baselen < 2 && (baselen + chl) > 2)
			numtail2_baselen = baselen;
		if (baselen < 6 && (baselen + chl) > 6)
			numtail_baselen = baselen;
		for (chi = 0; chi < chl; chi++) {
			*p++ = charbuf[chi];
			baselen++;
			if (baselen >= 8)
				break;
		}
		if (baselen >= 8) {
			if ((chi < chl - 1) || (ip + 1) - uname < sz)
				is_shortname = 0;
			break;
		}
	}
	if (baselen == 0) {
		return -EINVAL;
	}

	extlen = 0;
	if (ext_start) {
		for (p = ext, ip = ext_start; extlen < 3 && ip < end; ip++) {
			chl = to_shortname_char(nls, charbuf, sizeof(charbuf),
						ip, &ext_info);
			if (chl == 0)
				continue;

			if ((extlen + chl) > 3) {
				is_shortname = 0;
				break;
			}
			for (chi = 0; chi < chl; chi++) {
				*p++ = charbuf[chi];
				extlen++;
			}
			if (extlen >= 3) {
				if (ip + 1 != end)
					is_shortname = 0;
				break;
			}
		}
	}
	ext[extlen] = '\0';
	base[baselen] = '\0';

	/* Yes, it can happen. ".\xe5" would do it. */
	if (base[0] == DELETED_FLAG)
		base[0] = 0x05;

	/* OK, at this point we know that base is not longer than 8 symbols,
	 * ext is not longer than 3, base is nonempty, both don't contain
	 * any bad symbols (lowercase transformed to uppercase).
	 */

	memset(name_res, ' ', MSDOS_NAME);
	memcpy(name_res, base, baselen);
	memcpy(name_res + 8, ext, extlen);
	*lcase = 0;
	if (is_shortname && base_info.valid && ext_info.valid) {
		if (vfat_find_form(dir, name_res) == 0)
			return -EEXIST;

		if (opts->shortname & VFAT_SFN_CREATE_WIN95) {
			return (base_info.upper && ext_info.upper);
		} else if (opts->shortname & VFAT_SFN_CREATE_WINNT) {
			if ((base_info.upper || base_info.lower) &&
			    (ext_info.upper || ext_info.lower)) {
				if (!base_info.upper && base_info.lower)
					*lcase |= CASE_LOWER_BASE;
				if (!ext_info.upper && ext_info.lower)
					*lcase |= CASE_LOWER_EXT;
				return 1;
			}
			return 0;
		} else {
			BUG();
		}
	}

	if (opts->numtail == 0)
		if (vfat_find_form(dir, name_res) < 0)
			return 0;

	/*
	 * Try to find a unique extension.  This used to
	 * iterate through all possibilities sequentially,
	 * but that gave extremely bad performance.  Windows
	 * only tries a few cases before using random
	 * values for part of the base.
	 */

	if (baselen > 6) {
		baselen = numtail_baselen;
		name_res[7] = ' ';
	}
	name_res[baselen] = '~';
	for (i = 1; i < 10; i++) {
		name_res[baselen + 1] = i + '0';
		if (vfat_find_form(dir, name_res) < 0)
			return 0;
	}

	i = jiffies;
	sz = (jiffies >> 16) & 0x7;
	if (baselen > 2) {
		baselen = numtail2_baselen;
		name_res[7] = ' ';
	}
	name_res[baselen + 4] = '~';
	name_res[baselen + 5] = '1' + sz;
	while (1) {
		snprintf(buf, sizeof(buf), "%04X", i & 0xffff);
		memcpy(&name_res[baselen], buf, 4);
		if (vfat_find_form(dir, name_res) < 0)
			break;
		i -= 11;
	}
	return 0;
}

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
#define PLANE_SIZE	0x00010000

#define SURROGATE_PAIR	0x0000d800
#define SURROGATE_LOW	0x00000400
#define SURROGATE_BITS	0x000003ff

static int utf8s_to_utf16s_len(const u8 *s, int len, wchar_t *pwcs, int max)
{
	u16 *op;
	int size;
	unicode_t u;

	op = pwcs;
	while (*s && len > 0 && (op - pwcs) < max) {
		if (*s & 0x80) {
			size = utf8_to_utf32(s, len, &u);
			if (size < 0)
				return -EINVAL;

			if (u >= PLANE_SIZE) {
				u -= PLANE_SIZE;
				*op++ = (wchar_t) (SURROGATE_PAIR |
						((u >> 10) & SURROGATE_BITS));
				*op++ = (wchar_t) (SURROGATE_PAIR |
						SURROGATE_LOW |
						(u & SURROGATE_BITS));
			} else {
				*op++ = (wchar_t) u;
			}
			s += size;
			len -= size;
		} else {
			*op++ = *s++;
			len--;
		}
	}
	return op - pwcs;
}
#endif

/* Translate a string, including coded sequences into Unicode */
static int
xlate_to_uni(const unsigned char *name, int len, unsigned char *outname,
	     int *longlen, int *outlen, int escape, int utf8,
	     struct nls_table *nls)
{
	const unsigned char *ip;
	unsigned char nc;
	unsigned char *op;
	unsigned int ec;
	int i, k, fill;
	int charlen;

	if (utf8) {
#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
		*outlen = utf8s_to_utf16s_len(name, len, (wchar_t *)outname, 260);
#else
		*outlen = utf8s_to_utf16s(name, len, (wchar_t *)outname);
#endif
		if (*outlen < 0)
			return *outlen;
		else if (*outlen > FAT_LFN_LEN)
			return -ENAMETOOLONG;

		op = &outname[*outlen * sizeof(wchar_t)];
	} else {
		if (nls) {
			for (i = 0, ip = name, op = outname, *outlen = 0;
			     i < len && *outlen <= FAT_LFN_LEN;
			     *outlen += 1)
			{
				if (escape && (*ip == ':')) {
					if (i > len - 5)
						return -EINVAL;
					ec = 0;
					for (k = 1; k < 5; k++) {
						nc = ip[k];
						ec <<= 4;
						if (nc >= '0' && nc <= '9') {
							ec |= nc - '0';
							continue;
						}
						if (nc >= 'a' && nc <= 'f') {
							ec |= nc - ('a' - 10);
							continue;
						}
						if (nc >= 'A' && nc <= 'F') {
							ec |= nc - ('A' - 10);
							continue;
						}
						return -EINVAL;
					}
					*(wchar_t *)op = ec & 0xFFFF;
					op += 2;
					ip += 5;
					i += 5;
				} else {
					if ((charlen = nls->char2uni(ip, len - i, (wchar_t *)op)) < 0)
						return -EINVAL;
					ip += charlen;
					i += charlen;
					op += 2;
				}
			}
			if (i < len)
				return -ENAMETOOLONG;
		} else {
			for (i = 0, ip = name, op = outname, *outlen = 0;
			     i < len && *outlen <= FAT_LFN_LEN;
			     i++, *outlen += 1)
			{
				*op++ = *ip++;
				*op++ = 0;
			}
			if (i < len)
				return -ENAMETOOLONG;
		}
	}

	*longlen = *outlen;
	if (*outlen % 13) {
		*op++ = 0;
		*op++ = 0;
		*outlen += 1;
		if (*outlen % 13) {
			fill = 13 - (*outlen % 13);
			for (i = 0; i < fill; i++) {
				*op++ = 0xff;
				*op++ = 0xff;
			}
			*outlen += fill;
		}
	}

	return 0;
}

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
int
vfat_xlate_to_uni(const char *name, int len, char *outname, int *longlen,
		   int *outlen, int escape, int utf8, struct nls_table *nls)
{
	return xlate_to_uni(name, len, outname, longlen, outlen, escape,
			    utf8, nls);
}
#endif

static int vfat_build_slots(struct inode *dir, const unsigned char *name,
			    int len, int is_dir, int cluster,
			    struct timespec *ts,
			    struct msdos_dir_slot *slots, int *nr_slots)
{
	struct msdos_sb_info *sbi = MSDOS_SB(dir->i_sb);
	struct fat_mount_options *opts = &sbi->options;
	struct msdos_dir_slot *ps;
	struct msdos_dir_entry *de;
	unsigned char cksum, lcase;
	unsigned char msdos_name[MSDOS_NAME];
	wchar_t *uname;
	__le16 time, date;
	u8 time_cs;
	int err, ulen, usize, i;
	loff_t offset;

	*nr_slots = 0;

	uname = __getname();
	if (!uname)
		return -ENOMEM;

	err = xlate_to_uni(name, len, (unsigned char *)uname, &ulen, &usize,
			   opts->unicode_xlate, opts->utf8, sbi->nls_io);
	if (err)
		goto out_free;

	err = vfat_is_used_badchars(uname, ulen);
	if (err)
		goto out_free;

	err = vfat_create_shortname(dir, sbi->nls_disk, uname, ulen,
				    msdos_name, &lcase);
	if (err < 0)
		goto out_free;
	else if (err == 1) {
		de = (struct msdos_dir_entry *)slots;
		err = 0;
		goto shortname;
	}

	/* build the entry of long file name */
	cksum = fat_checksum(msdos_name);

	*nr_slots = usize / 13;
	for (ps = slots, i = *nr_slots; i > 0; i--, ps++) {
		ps->id = i;
		ps->attr = ATTR_EXT;
		ps->reserved = 0;
		ps->alias_checksum = cksum;
		ps->start = 0;
		offset = (i - 1) * 13;
		fatwchar_to16(ps->name0_4, uname + offset, 5);
		fatwchar_to16(ps->name5_10, uname + offset + 5, 6);
		fatwchar_to16(ps->name11_12, uname + offset + 11, 2);
	}
	slots[0].id |= 0x40;
	de = (struct msdos_dir_entry *)ps;

shortname:
	/* build the entry of 8.3 alias name */
	(*nr_slots)++;
	memcpy(de->name, msdos_name, MSDOS_NAME);
	de->attr = is_dir ? ATTR_DIR : ATTR_ARCH;
	de->lcase = lcase;
	fat_time_unix2fat(sbi, ts, &time, &date, &time_cs);
	de->time = de->ctime = time;
	de->date = de->cdate = de->adate = date;
	de->ctime_cs = time_cs;
	de->start = cpu_to_le16(cluster);
	de->starthi = cpu_to_le16(cluster >> 16);
	de->size = 0;
out_free:
	__putname(uname);
	return err;
}

static int vfat_add_entry(struct inode *dir, struct qstr *qname, int is_dir,
			  int cluster, struct timespec *ts,
			  struct fat_slot_info *sinfo)
{
	struct msdos_dir_slot *slots;
	unsigned int len;
	int err, nr_slots;

	len = vfat_striptail_len(qname);
	if (len == 0)
		return -ENOENT;

	slots = kmalloc(sizeof(*slots) * MSDOS_SLOTS, GFP_NOFS);
	if (slots == NULL)
		return -ENOMEM;

	err = vfat_build_slots(dir, qname->name, len, is_dir, cluster, ts,
			       slots, &nr_slots);
	if (err)
		goto cleanup;

	err = fat_add_entries(dir, slots, nr_slots, sinfo);
	if (err)
		goto cleanup;

	/* update timestamp */
	dir->i_ctime = dir->i_mtime = dir->i_atime = *ts;
	if (IS_DIRSYNC(dir))
		(void)fat_sync_inode(dir);
	else
		mark_inode_dirty(dir);
cleanup:
	kfree(slots);
	return err;
}

static int vfat_find(struct inode *dir, struct qstr *qname,
		     struct fat_slot_info *sinfo)
{
	unsigned int len = vfat_striptail_len(qname);
	if (len == 0)
		return -ENOENT;
	return fat_search_long(dir, qname->name, len, sinfo);
}

/*
 * (nfsd's) anonymous disconnected dentry?
 * NOTE: !IS_ROOT() is not anonymous (I.e. d_splice_alias() did the job).
 */
static int vfat_d_anon_disconn(struct dentry *dentry)
{
	return IS_ROOT(dentry) && (dentry->d_flags & DCACHE_DISCONNECTED);
}

static struct dentry *vfat_lookup(struct inode *dir, struct dentry *dentry,
				  struct nameidata *nd)
{
	struct super_block *sb = dir->i_sb;
	struct fat_slot_info sinfo;
	struct inode *inode;
	struct dentry *alias;
	int err;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return ERR_PTR(-EIO);
#endif

	lock_super(sb);

	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err) {
		if (err == -ENOENT) {
			inode = NULL;
			goto out;
		}
		goto error;
	}

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
	(void) MSDOS_SB(sb)->posix_ops.get_attr(inode, sinfo.de);
#endif
	brelse(sinfo.bh);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto error;
	}

	alias = d_find_alias(inode);
	if (alias && !vfat_d_anon_disconn(alias)) {
		/*
		 * This inode has non anonymous-DCACHE_DISCONNECTED
		 * dentry. This means, the user did ->lookup() by an
		 * another name (longname vs 8.3 alias of it) in past.
		 *
		 * Switch to new one for reason of locality if possible.
		 */
		BUG_ON(d_unhashed(alias));
		if (!S_ISDIR(inode->i_mode))
			d_move(alias, dentry);
		iput(inode);
		unlock_super(sb);
		return alias;
	} else
		dput(alias);

out:
	unlock_super(sb);
	dentry->d_time = dentry->d_parent->d_inode->i_version;
	dentry = d_splice_alias(inode, dentry);
	if (dentry)
		dentry->d_time = dentry->d_parent->d_inode->i_version;
	return dentry;

error:
	unlock_super(sb);
	return ERR_PTR(err);
}

static int vfat_create(struct inode *dir, struct dentry *dentry, int mode,
		       struct nameidata *nd)
{
	struct super_block *sb = dir->i_sb;
	struct inode *inode;
	struct fat_slot_info sinfo;
	struct timespec ts;
	int err;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	lock_super(sb);

	ts = CURRENT_TIME_SEC;
	err = vfat_add_entry(dir, &dentry->d_name, 0, 0, &ts, &sinfo);
	if (err)
		goto out;
	dir->i_version++;

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
	brelse(sinfo.bh);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out;
	}
	inode->i_version++;
	inode->i_mtime = inode->i_atime = inode->i_ctime = ts;
#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
	if (MSDOS_SB(sb)->options.posix_attr)  {
		inode->i_mode =	mode & VFAT_POSIX_ATTR_VALID_MODE;
		mark_inode_dirty(inode);
	}
#endif
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
	if (!IS_DIRSYNC(dir) &&  MSDOS_SB(sb)->options.batch_sync)
		mark_inode_dirty(inode);
#endif
	/* timestamp is already written, so mark_inode_dirty() is unneeded. */

	dentry->d_time = dentry->d_parent->d_inode->i_version;
	d_instantiate(dentry, inode);
out:
	unlock_super(sb);
	return err;
}

static int vfat_rmdir(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	struct super_block *sb = dir->i_sb;
	struct fat_slot_info sinfo;
	int err;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	lock_super(sb);

	err = fat_dir_empty(inode);
	if (err)
		goto out;
	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err)
		goto out;

#ifdef CONFIG_SNSC_FS_FAT_GC
	/*
	 * mark all cluster of this entry valid
	 * before erase it from the dir
	 */
        fat_gc_mark_valid_entries(inode);
#endif

	err = fat_remove_entries(dir, &sinfo);	/* and releases bh */
	if (err)
		goto out;
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
	/* remove dentry from disk */
	if (MSDOS_SB(inode->i_sb)->options.batch_sync) {
		err = fat_syncdir(dir);
		if (err)
			goto out;
	}
#endif
	drop_nlink(dir);

	clear_nlink(inode);
	inode->i_mtime = inode->i_atime = CURRENT_TIME_SEC;
	fat_detach(inode);
out:
	unlock_super(sb);

	return err;
}

static int vfat_unlink(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	struct super_block *sb = dir->i_sb;
	struct fat_slot_info sinfo;
	int err;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	lock_super(sb);

	err = vfat_find(dir, &dentry->d_name, &sinfo);
	if (err)
		goto out;

	err = fat_remove_entries(dir, &sinfo);	/* and releases bh */
	if (err)
		goto out;
	clear_nlink(inode);
	inode->i_mtime = inode->i_atime = CURRENT_TIME_SEC;
	fat_detach(inode);
out:
	unlock_super(sb);

	return err;
}

static int vfat_mkdir(struct inode *dir, struct dentry *dentry, int mode)
{
	struct super_block *sb = dir->i_sb;
	struct inode *inode;
	struct fat_slot_info sinfo;
	struct timespec ts;
	int err, cluster;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	lock_super(sb);

	ts = CURRENT_TIME_SEC;
	cluster = fat_alloc_new_dir(dir, &ts);
	if (cluster < 0) {
		err = cluster;
		goto out;
	}
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
	/* flush cluster chain & first cluster of the subdir*/
	if (MSDOS_SB(sb)->options.batch_sync) {
		err = fat_syncdir(dir);
		if (err)
			goto out_free;
	}
#endif
	err = vfat_add_entry(dir, &dentry->d_name, 1, cluster, &ts, &sinfo);
	if (err)
		goto out_free;
	dir->i_version++;
	inc_nlink(dir);

#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
	/* flush dentry*/
	if (MSDOS_SB(sb)->options.batch_sync) {
		if (fat_syncdir(dir))
			goto out_free;
	}
#endif
	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
	brelse(sinfo.bh);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		/* the directory was completed, just return a error */
		goto out;
	}
	inode->i_version++;
	inode->i_nlink = 2;
	inode->i_mtime = inode->i_atime = inode->i_ctime = ts;
#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
	if (MSDOS_SB(sb)->options.posix_attr) {
		inode->i_mode = (S_IFDIR | (mode & VFAT_POSIX_ATTR_VALID_MODE));
		mark_inode_dirty(inode);
	}
#endif
	/* timestamp is already written, so mark_inode_dirty() is unneeded. */

	dentry->d_time = dentry->d_parent->d_inode->i_version;
	d_instantiate(dentry, inode);

	unlock_super(sb);
	return 0;

out_free:
	fat_free_clusters(dir, cluster);
out:
	unlock_super(sb);
	return err;
}

static int vfat_rename(struct inode *old_dir, struct dentry *old_dentry,
		       struct inode *new_dir, struct dentry *new_dentry)
{
#if defined(CONFIG_SNSC_FS_FAT_BATCH_SYNC) || defined(CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK)
	struct msdos_sb_info *sbi = MSDOS_SB(old_dir->i_sb);
#endif
	struct buffer_head *dotdot_bh;
	struct msdos_dir_entry *dotdot_de;
	struct inode *old_inode, *new_inode;
	struct fat_slot_info old_sinfo, sinfo;
	struct timespec ts;
	loff_t dotdot_i_pos, new_i_pos;
	int err, is_dir, update_dotdot, corrupt = 0;
	struct super_block *sb = old_dir->i_sb;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	old_sinfo.bh = sinfo.bh = dotdot_bh = NULL;
	old_inode = old_dentry->d_inode;
	new_inode = new_dentry->d_inode;
	lock_super(sb);
	err = vfat_find(old_dir, &old_dentry->d_name, &old_sinfo);
	if (err)
		goto out;

	is_dir = S_ISDIR(old_inode->i_mode);
	update_dotdot = (is_dir && old_dir != new_dir);
	if (update_dotdot) {
		if (fat_get_dotdot_entry(old_inode, &dotdot_bh, &dotdot_de,
					 &dotdot_i_pos) < 0) {
			err = -EIO;
			goto out;
		}
	}

	ts = CURRENT_TIME_SEC;
	if (new_inode) {
		if (is_dir) {
			err = fat_dir_empty(new_inode);
			if (err)
				goto out;
		}
		new_i_pos = MSDOS_I(new_inode)->i_pos;
		fat_detach(new_inode);
	} else {
		err = vfat_add_entry(new_dir, &new_dentry->d_name, is_dir, 0,
				     &ts, &sinfo);
		if (err)
			goto out;
		new_i_pos = sinfo.i_pos;
	}
	new_dir->i_version++;

	fat_detach(old_inode);
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	if (sbi->options.avoid_dlink) {
		err = fat_remove_entries(old_dir, &old_sinfo);  /* and releases bh */
		old_sinfo.bh = NULL;
		if (err)
			goto error_inode;
		old_dir->i_version++;
		old_dir->i_ctime = old_dir->i_mtime = ts;
		if (IS_DIRSYNC(old_dir))
			(void)fat_sync_inode(old_dir);
		else
			mark_inode_dirty(old_dir);
	}
#endif
	fat_attach(old_inode, new_i_pos);
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	if (!sbi->options.avoid_dlink) {
#endif
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
	if (IS_DIRSYNC(new_dir) || sbi->options.batch_sync) {
#else
	if (IS_DIRSYNC(new_dir)) {
#endif
		err = fat_sync_inode(old_inode);
		if (err)
			goto error_inode;
	} else
		mark_inode_dirty(old_inode);
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	}
#endif

	if (update_dotdot) {
		int start = MSDOS_I(new_dir)->i_logstart;
		dotdot_de->start = cpu_to_le16(start);
		dotdot_de->starthi = cpu_to_le16(start >> 16);
		mark_buffer_dirty_inode(dotdot_bh, old_inode);
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
		if (IS_DIRSYNC(new_dir) || sbi->options.batch_sync) {
#else
		if (IS_DIRSYNC(new_dir)) {
#endif
			err = sync_dirty_buffer(dotdot_bh);
			if (err) {
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
				if (sbi->options.avoid_dlink)
					goto error_inode;
				else
#endif
				goto error_dotdot;
			}
		}
		drop_nlink(old_dir);
		if (!new_inode)
 			inc_nlink(new_dir);
	}

#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	if (sbi->options.avoid_dlink) {
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
		if (IS_DIRSYNC(new_dir) || sbi->options.batch_sync) {
#else
		if (IS_DIRSYNC(new_dir)) {
#endif
			err = fat_sync_inode(old_inode);
			if (err)
				goto error_dotdot;
		} else
			mark_inode_dirty(old_inode);
	} else {
#endif
	err = fat_remove_entries(old_dir, &old_sinfo);	/* and releases bh */
	old_sinfo.bh = NULL;
	if (err)
		goto error_dotdot;
	old_dir->i_version++;
	old_dir->i_ctime = old_dir->i_mtime = ts;
	if (IS_DIRSYNC(old_dir))
		(void)fat_sync_inode(old_dir);
	else
		mark_inode_dirty(old_dir);
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	}
#endif

	if (new_inode) {
		drop_nlink(new_inode);
		if (is_dir)
			drop_nlink(new_inode);
		new_inode->i_ctime = ts;
	}
out:
	brelse(sinfo.bh);
	brelse(dotdot_bh);
	brelse(old_sinfo.bh);
	unlock_super(sb);

	return err;

error_dotdot:
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	/*Revert the change*/
#else
	/* data cluster is shared, serious corruption */
#endif
	corrupt = 1;

	if (update_dotdot) {
		int start = MSDOS_I(old_dir)->i_logstart;
		dotdot_de->start = cpu_to_le16(start);
		dotdot_de->starthi = cpu_to_le16(start >> 16);
		mark_buffer_dirty_inode(dotdot_bh, old_inode);
		corrupt |= sync_dirty_buffer(dotdot_bh);
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
		if (sbi->options.avoid_dlink) {
			if (corrupt < 0){
				goto error_inode;
			}
			old_dir->i_nlink++;
			if (!new_inode)
				new_dir->i_nlink--;
		}
#endif
	}
error_inode:
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	if (!sbi->options.avoid_dlink)
#endif
	fat_detach(old_inode);
	fat_attach(old_inode, old_sinfo.i_pos);
	if (new_inode) {
		fat_attach(new_inode, new_i_pos);
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
		if (corrupt || sbi->options.avoid_dlink)
#else
		if (corrupt)
#endif
			corrupt |= fat_sync_inode(new_inode);
	} else {
#ifndef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
		/*
		 * If new entry was not sharing the data cluster, it
		 * shouldn't be serious corruption.
		 */
#endif
		int err2 = fat_remove_entries(new_dir, &sinfo);
		if (corrupt)
			corrupt |= err2;
		sinfo.bh = NULL;
	}
#ifdef CONFIG_SNSC_FS_VFAT_AVOID_DOUBLE_LINK
	/* There are not so fatal state in FAT FS. */
#endif
	if (corrupt < 0) {
		fat_fs_error(new_dir->i_sb,
			     "%s: Filesystem corrupted (i_pos %lld)",
			     __func__, sinfo.i_pos);
	}
	goto out;
}

#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
static
int vfat_symlink(struct inode *dir, struct dentry *dentry, const char *symname)
{
	/* base : vfat_create() */
	struct super_block *sb = dir->i_sb;
	struct inode *inode = NULL;
	struct fat_slot_info sinfo;
	struct timespec ts;
	int err;
	int len;

	if (!MSDOS_SB(sb)->options.posix_attr)
		return -EOPNOTSUPP;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	len = strlen (symname) + 1;
	if ((len > PATH_MAX) || (len > PAGE_SIZE)) {
		return -ENAMETOOLONG;
	}

	lock_super(sb);

	ts = CURRENT_TIME_SEC;
	err = vfat_add_entry(dir, &dentry->d_name, 0, 0, &ts, &sinfo);
	if (err)
		goto out;
	dir->i_version++;

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
	brelse(sinfo.bh);
	if (IS_ERR(inode)){
		err = PTR_ERR(inode);
		goto out;
	}
	inode->i_version++;
	inode->i_mode = (S_IFLNK | 0777);
	inode->i_mtime = inode->i_atime = inode->i_ctime = ts;
	inode->i_op = &fat_symlink_inode_operations;
	mark_inode_dirty(inode);

	dentry->d_time = dentry->d_parent->d_inode->i_version;
	d_instantiate(dentry,inode);

	err = page_symlink(dentry->d_inode, symname, len);
#ifdef CONFIG_SNSC_FS_FAT_BATCH_SYNC
	if (MSDOS_SB(sb)->options.batch_sync) {
		(void)fat_sync_inode(inode);
		mark_inode_clean(inode);
	}
#endif
out:
	unlock_super(sb);
	return err;
}

static
int vfat_mknod(struct inode *dir, struct dentry *dentry, int mode, dev_t rdev)
{
	/* base : vfat_create() */
	struct super_block *sb = dir->i_sb;
	struct inode *inode = NULL;
	struct fat_slot_info sinfo;
	struct timespec ts;
	int err;

	if (!MSDOS_SB(sb)->options.posix_attr)
		return -EOPNOTSUPP;

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (fat_check_disk(sb, 1))
		return -EIO;
#endif

	lock_super(sb);

	ts = CURRENT_TIME_SEC;
	err = vfat_add_entry(dir, &dentry->d_name, 0, 0, &ts, &sinfo);
	if (err)
		goto out;
	dir->i_version++;

	inode = fat_build_inode(sb, sinfo.de, sinfo.i_pos);
	brelse(sinfo.bh);
	if (IS_ERR(inode)){
		err = PTR_ERR(inode);
		goto out;
	}
	inode->i_version++;

	inode->i_mode =	mode & VFAT_POSIX_ATTR_VALID_MODE;
	inode->i_rdev = rdev;

	inode->i_mtime = inode->i_atime = inode->i_ctime = ts;
	init_special_inode(inode, mode, rdev);
	mark_inode_dirty(inode);

	dentry->d_time = dentry->d_parent->d_inode->i_version;
	d_instantiate(dentry, inode);

	err = 0;

out:
	unlock_super(sb);
	return err;
}
#endif

static const struct inode_operations vfat_dir_inode_operations = {
	.create		= vfat_create,
	.lookup		= vfat_lookup,
	.unlink		= vfat_unlink,
	.mkdir		= vfat_mkdir,
	.rmdir		= vfat_rmdir,
	.rename		= vfat_rename,
	.setattr	= fat_setattr,
	.getattr	= fat_getattr,
#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
	.symlink	= vfat_symlink,
	.mknod		= vfat_mknod,
#endif
};

#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
static struct fat_posix_ops vfat_posix_ops = {
	.is_symlink = is_vfat_posix_symlink,
	.set_attr = set_vfat_posix_attr,
	.get_attr = get_vfat_posix_attr,
};
#endif

static void setup(struct super_block *sb)
{
	if (MSDOS_SB(sb)->options.name_check != 's')
		sb->s_d_op = &vfat_ci_dentry_ops;
	else
		sb->s_d_op = &vfat_dentry_ops;
}

static int vfat_fill_super(struct super_block *sb, void *data, int silent)
{
	int res;
	res = fat_fill_super(sb, data, silent, &vfat_dir_inode_operations,
			     1, setup);
	if (res)
		return res;

#ifdef CONFIG_SNSC_FS_VFAT_COMPARE_UNICODE
	if (MSDOS_SB(sb)->options.compare_unicode) {
		if (MSDOS_SB(sb)->options.name_check != 's')
			sb->s_d_op = &vfat_ciu_dentry_ops;
		else
			sb->s_d_op = &vfat_cu_dentry_ops;
	}
#endif

#ifdef CONFIG_SNSC_FS_VFAT_POSIX_ATTR
	MSDOS_SB(sb)->posix_ops = vfat_posix_ops;
#endif

#ifdef CONFIG_SNSC_FS_VFAT_CHECK_DISK
	if (MSDOS_SB(sb)->options.check_disk &&
	    k3d_get_disk(sb->s_bdev->bd_disk) != 0) {
		printk("VFAT: disabled check_disk option\n");
		MSDOS_SB(sb)->options.check_disk = 0;
	}
#else
	if (MSDOS_SB(sb)->options.check_disk)
		MSDOS_SB(sb)->options.check_disk = 0;
#endif

	return 0;
}

static struct dentry *vfat_mount(struct file_system_type *fs_type,
		       int flags, const char *dev_name,
		       void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, vfat_fill_super);
}

static struct file_system_type vfat_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "vfat",
	.mount		= vfat_mount,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV,
};

static int __init init_vfat_fs(void)
{
	return register_filesystem(&vfat_fs_type);
}

static void __exit exit_vfat_fs(void)
{
	unregister_filesystem(&vfat_fs_type);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("VFAT filesystem support");
MODULE_AUTHOR("Gordon Chaffee");

module_init(init_vfat_fs)
module_exit(exit_vfat_fs)
