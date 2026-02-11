#include <linux/module.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/fd.h>
#include <linux/tty.h>
#include <linux/suspend.h>
#include <linux/root_dev.h>
#include <linux/security.h>
#include <linux/delay.h>
#include <linux/genhd.h>
#include <linux/mount.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/fdtable.h>
#include <linux/initrd.h>
#include <linux/async.h>
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <linux/crypto.h>

#include <linux/nfs_fs.h>
#include <linux/nfs_fs_sb.h>
#include <linux/nfs_mount.h>

#include <linux/device-mapper.h>
#include <linux/dm-ioctl.h>
#include <linux/dm-auth.h>

#include "do_mounts.h"

#define IMG_TYPE_INITFS_CRAMFS          (0x0A)

int __initdata rd_doload;	/* 1 = load RAM disk, 0 = don't load */

int root_mountflags = MS_RDONLY | MS_SILENT;
static char * __initdata root_device_name;
static char __initdata saved_root_name[64];
static int __initdata root_wait;
unsigned char roothash[32];
extern unsigned int dm_major;

dev_t ROOT_DEV;

static int __init load_ramdisk(char *str)
{
	rd_doload = simple_strtol(str,NULL,0) & 3;
	return 1;
}
__setup("load_ramdisk=", load_ramdisk);

static int __init readonly(char *str)
{
	if (*str)
		return 0;
	root_mountflags |= MS_RDONLY;
	return 1;
}

static int __init readwrite(char *str)
{
	if (*str)
		return 0;
	root_mountflags &= ~MS_RDONLY;
	return 1;
}

__setup("ro", readonly);
__setup("rw", readwrite);

static int __init hexstr_to_bin(char *hex, unsigned char *bin, int binlen)
{
	int i;

	for (i = 0; i < binlen; i++) {
		char hi = hex[2*i];
		char lo = hex[2*i+1];
		if (!isxdigit(hi) || !isxdigit(lo)) {
			printk(KERN_ERR "invalid hex char in roothash %s\n", roothash);
			return -EINVAL;
		}
		bin[i] = (hex_to_bin(hi) << 4) | hex_to_bin(lo);
	}
	return 0;
}

static int __init setup_roothash(char *str)
{
	hexstr_to_bin(str, roothash, sizeof(roothash));
	return 1;
}
__setup("roothash=", setup_roothash);


#ifdef CONFIG_BLOCK
/**
 * match_dev_by_uuid - callback for finding a partition using its uuid
 * @dev:	device passed in by the caller
 * @data:	opaque pointer to a 36 byte char array with a UUID
 *
 * Returns 1 if the device matches, and 0 otherwise.
 */
static int match_dev_by_uuid(struct device *dev, void *data)
{
	u8 *uuid = data;
	struct hd_struct *part = dev_to_part(dev);

	if (!part->info)
		goto no_match;

	if (memcmp(uuid, part->info->uuid, sizeof(part->info->uuid)))
			goto no_match;

	return 1;
no_match:
	return 0;
}


/**
 * devt_from_partuuid - looks up the dev_t of a partition by its UUID
 * @uuid:	36 byte char array containing a hex ascii UUID
 *
 * The function will return the first partition which contains a matching
 * UUID value in its partition_meta_info struct.  This does not search
 * by filesystem UUIDs.
 *
 * Returns the matching dev_t on success or 0 on failure.
 */
static dev_t devt_from_partuuid(char *uuid_str)
{
	dev_t res = 0;
	struct device *dev = NULL;
	u8 uuid[16];

	/* Pack the requested UUID in the expected format. */
	part_pack_uuid(uuid_str, uuid);

	dev = class_find_device(&block_class, NULL, uuid, &match_dev_by_uuid);
	if (!dev)
		goto done;

	res = dev->devt;
	put_device(dev);

done:
	return res;
}
#endif

/*
 *	Convert a name into device number.  We accept the following variants:
 *
 *	1) device number in hexadecimal	represents itself
 *	2) /dev/nfs represents Root_NFS (0xff)
 *	3) /dev/<disk_name> represents the device number of disk
 *	4) /dev/<disk_name><decimal> represents the device number
 *         of partition - device number of disk plus the partition number
 *	5) /dev/<disk_name>p<decimal> - same as the above, that form is
 *	   used when disk name of partitioned disk ends on a digit.
 *	6) PARTUUID=00112233-4455-6677-8899-AABBCCDDEEFF representing the
 *	   unique id of a partition if the partition table provides it.
 *
 *	If name doesn't have fall into the categories above, we return (0,0).
 *	block_class is used to check if something is a disk name. If the disk
 *	name contains slashes, the device name has them replaced with
 *	bangs.
 */

dev_t name_to_dev_t(char *name)
{
	char s[32];
	char *p;
	dev_t res = 0;
	int part;

#ifdef CONFIG_BLOCK
	if (strncmp(name, "PARTUUID=", 9) == 0) {
		name += 9;
		if (strlen(name) != 36)
			goto fail;
		res = devt_from_partuuid(name);
		if (!res)
			goto fail;
		goto done;
	}
#endif

	if (strncmp(name, "/dev/", 5) != 0) {
		unsigned maj, min;

		if (sscanf(name, "%u:%u", &maj, &min) == 2) {
			res = MKDEV(maj, min);
			if (maj != MAJOR(res) || min != MINOR(res))
				goto fail;
		} else {
			res = new_decode_dev(simple_strtoul(name, &p, 16));
			if (*p)
				goto fail;
		}
		goto done;
	}

	name += 5;
	res = Root_NFS;
	if (strcmp(name, "nfs") == 0)
		goto done;
	res = Root_RAM0;
	if (strcmp(name, "ram") == 0)
		goto done;

	if (strlen(name) > 31)
		goto fail;
	strcpy(s, name);
	for (p = s; *p; p++)
		if (*p == '/')
			*p = '!';
	res = blk_lookup_devt(s, 0);
	if (res)
		goto done;

	/*
	 * try non-existent, but valid partition, which may only exist
	 * after revalidating the disk, like partitioned md devices
	 */
	while (p > s && isdigit(p[-1]))
		p--;
	if (p == s || !*p || *p == '0')
		goto fail;

	/* try disk name without <part number> */
	part = simple_strtoul(p, NULL, 10);
	*p = '\0';
	res = blk_lookup_devt(s, part);
	if (res)
		goto done;

	/* try disk name without p<part number> */
	if (p < s + 2 || !isdigit(p[-2]) || p[-1] != 'p')
		goto fail;
	p[-1] = '\0';
	res = blk_lookup_devt(s, part);
	if (res)
		goto done;

fail:
	return 0;
done:
	return res;
}

static int __init root_dev_setup(char *line)
{
	strlcpy(saved_root_name, line, sizeof(saved_root_name));
	return 1;
}

__setup("root=", root_dev_setup);

static int __init rootwait_setup(char *str)
{
	if (*str)
		return 0;
	root_wait = 1;
	return 1;
}

__setup("rootwait", rootwait_setup);

static char * __initdata root_mount_data;
static int __init root_data_setup(char *str)
{
	root_mount_data = str;
	return 1;
}

static char * __initdata root_fs_names;
static int __init fs_names_setup(char *str)
{
	root_fs_names = str;
	return 1;
}

static unsigned int __initdata root_delay;
static int __init root_delay_setup(char *str)
{
	root_delay = simple_strtoul(str, NULL, 0);
	return 1;
}

__setup("rootflags=", root_data_setup);
__setup("rootfstype=", fs_names_setup);
__setup("rootdelay=", root_delay_setup);

static void __init get_fs_names(char *page)
{
	char *s = page;

#if 1 // ROKU
	// Always try squashfs first.
	strcpy(page, "squashfs");
	page += strlen(page)+1;
	s = page;
#endif

	if (root_fs_names) {
		strcpy(page, root_fs_names);
		while (*s++) {
			if (s[-1] == ',')
				s[-1] = '\0';
		}
	} else {
		int len = get_filesystem_list(page);
		char *p, *next;

		page[len] = '\0';
		for (p = page-1; p; p = next) {
			next = strchr(++p, '\n');
			if (*p++ != '\t')
				continue;
			while ((*s++ = *p++) != '\n')
				;
			s[-1] = '\0';
		}
	}
	*s = '\0';
}

struct dm_params { /* parameters for DM_ ioctls */
	struct dm_ioctl ioc;
	struct dm_target_spec target;
	char params[128];
};
static char *dm_ctl_name = "/dev/authroot_ctl";
static int mapper_minor = 250;

/* Initialize a dm_params struct. */
static void __init dm_params_init(struct dm_params* dm)
{
	memset(dm, 0, sizeof(*dm));
	dm->ioc.version[0] = DM_VERSION_MAJOR ;
	dm->ioc.version[1] = DM_VERSION_MINOR;
	dm->ioc.version[2] = DM_VERSION_PATCHLEVEL;
	dm->ioc.data_start = sizeof(dm->ioc);
	dm->ioc.data_size = sizeof(dm->ioc);
	dm->ioc.flags = DM_PERSISTENT_DEV_FLAG;
	strcpy(dm->ioc.name, "auth_root");
	dm->ioc.dev = new_encode_dev(MKDEV(MD_MAJOR, mapper_minor));
}

/* Delete the mapper device created by authenticate_root. */
static void __init unauthenticate_root(void)
{
	int ctl;
	struct dm_params dm;
	int err;
	dm_params_init(&dm);
	ctl = sys_open((const char __user __force *) dm_ctl_name, O_RDWR, 0);
	BUG_ON(ctl < 0);
	err = sys_ioctl(ctl, DM_DEV_REMOVE, (unsigned long) &dm);
	sys_close(ctl);
}

/* Set up an authentication device-mapper on top of the specified device.
 * Return the name of the new device in new_name.
 */
static int __init authenticate_root(char *name, char *fs, char *new_name, int new_name_len)
{
	int root;
	int ctl;
	int dev;
	struct file_system_type *fstype;
	struct file *file;
	struct MerkleHeader mheader;
	struct dm_params dm;
	int err;


	/* Read MerkleHeader from filesystem. */
	fstype = get_fs_type(fs);
	if (!fstype->get_authdata) {
		printk(KERN_INFO "%s filesystem does not support authentication\n", fs);
		return -EINVAL;
	}
	dev = sys_open(name, O_RDONLY, 0);
	if (dev < 0) {
		printk(KERN_ERR "cannot open %s: error %d\n", name, dev);
		return dev;
	}
	spin_lock(&current->files->file_lock);
	file = files_fdtable(current->files)->fd[dev];
	spin_unlock(&current->files->file_lock);
	err = fstype->get_authdata(file, IMG_TYPE_INITFS_CRAMFS, &mheader, sizeof(mheader), NULL, 0);
	sys_close(dev);
	if (err < 0) {
		printk(KERN_INFO "cannot get authentication data from %s\n", name);
		return err;
	}

	/* Create a control node to access device-mapper. */
	sys_mknod(dm_ctl_name, S_IFCHR|0600,
		new_encode_dev(MKDEV(MISC_MAJOR, MAPPER_CTRL_MINOR)));
	ctl = sys_open((const char __user __force *) dm_ctl_name, O_RDWR, 0);
	BUG_ON(ctl < 0);

	/* Create dm-auth device mapper on top of root device. */
	dm_params_init(&dm);
	err = sys_ioctl(ctl, DM_DEV_CREATE, (unsigned long) &dm);
	if (err < 0) {
		sys_close(ctl);
		printk(KERN_ERR "DM_DEV_CREATE failed: %d\n", err);
		return err;
	}
	dm.ioc.data_size = sizeof(dm);
	dm.ioc.target_count = 1;
	dm.target.length = to_sector(mheader.mh_udata_size);
	strcpy(dm.target.target_type, "auth");
	snprintf(dm.params, sizeof(dm.params), "%s 0", name);
	err = sys_ioctl(ctl, DM_TABLE_LOAD, (unsigned long) &dm);
	if (err < 0) {
		sys_close(ctl);
		printk(KERN_ERR "DM_TABLE_LOAD failed: %d\n", err);
		unauthenticate_root();
		return err;
	}
	err = sys_ioctl(ctl, DM_DEV_SUSPEND, (unsigned long) &dm);
	sys_close(ctl);

	/* Now create a node to access the mapped root. */
	strncpy(new_name, "/dev/auth_root", new_name_len);
	sys_mknod(new_name, S_IFBLK|0777,
		new_encode_dev(MKDEV(dm_major, mapper_minor)));

	/* Initialize the mapper with the MerkleHeader we got earlier. */
	root = sys_open(new_name, O_RDWR, 0);
	if (root < 0) {
		printk(KERN_ERR "cannot open new root %s\n", new_name);
		unauthenticate_root();
		return root;
	}
	err = sys_ioctl(root, DMAUTH_IOCTL_SET_MERKLE_STICKY, (unsigned long) &mheader);
	sys_close(root);
	if (err < 0) {
		unauthenticate_root();
		return err;
	}
	return 0;
}

static int __init do_mount_root(char *name, char *fs, int flags, void *data)
{
	int err;
	char root_name[128];
	
	if (strcmp(fs, "nfs") != 0) {
		printk(KERN_INFO "booting authenticated root %s using %s\n", name, fs);
		err = authenticate_root(name, fs, root_name, sizeof(root_name));
		if (err < 0) {
			if (err != -EINVAL) printk(KERN_ERR "authenticate error %d\n", err);
			return err;
		}
		name = root_name;
	} else {
		printk(KERN_INFO "booting unauthenticated root %s using %s\n", name, fs);
	}

	err = sys_mount((char __force_user *)name, (char __force_user *)"/root", (char __force_user *)fs, flags, (void __force_user *)data);
	if (err) {
		unauthenticate_root();
		return err;
	}

	sys_chdir((const char __force_user*)"/root");
	ROOT_DEV = current->fs->pwd.mnt->mnt_sb->s_dev;
	printk(KERN_INFO
	       "VFS: Mounted root (%s filesystem)%s on device %u:%u.\n",
	       current->fs->pwd.mnt->mnt_sb->s_type->name,
	       current->fs->pwd.mnt->mnt_sb->s_flags & MS_RDONLY ?
	       " readonly" : "", MAJOR(ROOT_DEV), MINOR(ROOT_DEV));
	return 0;
}

void __init mount_block_root(char *name, int flags)
{
	char *fs_names = __getname_gfp(GFP_KERNEL
		| __GFP_NOTRACK_FALSE_POSITIVE);
	char *p;
#ifdef CONFIG_BLOCK
	char b[BDEVNAME_SIZE];
#else
	const char *b = name;
#endif

	get_fs_names(fs_names);
retry:
	for (p = fs_names; *p; p += strlen(p)+1) {
		int err = do_mount_root(name, p, flags, root_mount_data);
		switch (err) {
			case 0:
				goto out;
			case -EACCES:
				flags |= MS_RDONLY;
				goto retry;
			case -EINVAL:
				continue;
		}
	        /*
		 * Allow the user to distinguish between failed sys_open
		 * and bad superblock on root device.
		 * and give them a list of the available devices
		 */
#ifdef CONFIG_BLOCK
		__bdevname(ROOT_DEV, b);
#endif
		printk("VFS: Cannot open root device \"%s\" or %s\n",
				root_device_name, b);
		printk("Please append a correct \"root=\" boot option; here are the available partitions:\n");

		printk_all_partitions();
#ifdef CONFIG_DEBUG_BLOCK_EXT_DEVT
		printk("DEBUG_BLOCK_EXT_DEVT is enabled, you need to specify "
		       "explicit textual name for \"root=\" boot option.\n");
#endif
		panic("VFS: Unable to mount root fs on %s", b);
	}

	printk("List of all partitions:\n");
	printk_all_partitions();
	printk("No filesystem could mount root, tried: ");
	for (p = fs_names; *p; p += strlen(p)+1)
		printk(" %s", p);
	printk("\n");
#ifdef CONFIG_BLOCK
	__bdevname(ROOT_DEV, b);
#endif
	panic("VFS: Unable to mount root fs on %s", b);
out:
	putname(fs_names);
}
 
#ifdef CONFIG_ROOT_NFS

#define NFSROOT_TIMEOUT_MIN	5
#define NFSROOT_TIMEOUT_MAX	30
#define NFSROOT_RETRY_MAX	5

static int __init mount_nfs_root(void)
{
	char *root_dev, *root_data;
	unsigned int timeout;
	int try, err;

	err = nfs_root_data(&root_dev, &root_data);
	if (err != 0)
		return 0;

	/*
	 * The server or network may not be ready, so try several
	 * times.  Stop after a few tries in case the client wants
	 * to fall back to other boot methods.
	 */
	timeout = NFSROOT_TIMEOUT_MIN;
	for (try = 1; ; try++) {
		err = do_mount_root(root_dev, "nfs",
					root_mountflags, root_data);
		if (err == 0)
			return 1;
		if (try > NFSROOT_RETRY_MAX)
			break;

		/* Wait, in case the server refused us immediately */
		ssleep(timeout);
		timeout <<= 1;
		if (timeout > NFSROOT_TIMEOUT_MAX)
			timeout = NFSROOT_TIMEOUT_MAX;
	}
	return 0;
}
#endif

#if defined(CONFIG_BLK_DEV_RAM) || defined(CONFIG_BLK_DEV_FD)
void __init change_floppy(char *fmt, ...)
{
	struct termios termios;
	char buf[80];
	char c;
	int fd;
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	fd = sys_open((char __user *)"/dev/root", O_RDWR | O_NDELAY, 0);
	if (fd >= 0) {
		sys_ioctl(fd, FDEJECT, 0);
		sys_close(fd);
	}
	printk(KERN_NOTICE "VFS: Insert %s and press ENTER\n", buf);
	fd = sys_open((__force const char __user *)"/dev/console", O_RDWR, 0);
	if (fd >= 0) {
		sys_ioctl(fd, TCGETS, (long)&termios);
		termios.c_lflag &= ~ICANON;
		sys_ioctl(fd, TCSETSF, (long)&termios);
		sys_read(fd, (char __user *)&c, 1);
		termios.c_lflag |= ICANON;
		sys_ioctl(fd, TCSETSF, (long)&termios);
		sys_close(fd);
	}
}
#endif

void __init mount_root(void)
{
#ifdef CONFIG_ROOT_NFS
	if (ROOT_DEV == Root_NFS) {
		if (mount_nfs_root())
			return;

		printk(KERN_ERR "VFS: Unable to mount root fs via NFS, trying floppy.\n");
		ROOT_DEV = Root_FD0;
	}
#endif
#ifdef CONFIG_BLK_DEV_FD
	if (MAJOR(ROOT_DEV) == FLOPPY_MAJOR) {
		/* rd_doload is 2 for a dual initrd/ramload setup */
		if (rd_doload==2) {
			if (rd_load_disk(1)) {
				ROOT_DEV = Root_RAM1;
				root_device_name = NULL;
			}
		} else
			change_floppy("root floppy");
	}
#endif
#ifdef CONFIG_BLOCK
	create_dev("/dev/root", ROOT_DEV);
	mount_block_root("/dev/root", root_mountflags);
#endif
}

/*
 * Prepare the namespace - decide what/where to mount, load ramdisks, etc.
 */
void __init prepare_namespace(void)
{
	int is_floppy;

	if (root_delay) {
		printk(KERN_INFO "Waiting %dsec before mounting root device...\n",
		       root_delay);
		ssleep(root_delay);
	}

	/*
	 * wait for the known devices to complete their probing
	 *
	 * Note: this is a potential source of long boot delays.
	 * For example, it is not atypical to wait 5 seconds here
	 * for the touchpad of a laptop to initialize.
	 */
	wait_for_device_probe();

	md_run_setup();

	if (saved_root_name[0]) {
		root_device_name = saved_root_name;
		if (!strncmp(root_device_name, "mtd", 3) ||
		    !strncmp(root_device_name, "ubi", 3)) {
			mount_block_root(root_device_name, root_mountflags);
			goto out;
		}
		ROOT_DEV = name_to_dev_t(root_device_name);
		if (strncmp(root_device_name, "/dev/", 5) == 0)
			root_device_name += 5;
	}

	if (initrd_load())
		goto out;

	/* wait for any asynchronous scanning to complete */
	if ((ROOT_DEV == 0) && root_wait) {
		printk(KERN_INFO "Waiting for root device %s...\n",
			saved_root_name);
		while (driver_probe_done() != 0 ||
			(ROOT_DEV = name_to_dev_t(saved_root_name)) == 0)
			msleep(100);
		async_synchronize_full();
	}

	is_floppy = MAJOR(ROOT_DEV) == FLOPPY_MAJOR;

	if (is_floppy && rd_doload && rd_load_disk(0))
		ROOT_DEV = Root_RAM0;

	mount_root();
out:
	devtmpfs_mount("dev");
	sys_mount((char __force_user *)".", (char __force_user *)"/", NULL, MS_MOVE, NULL);
	sys_chroot((const char __force_user *)".");
}
