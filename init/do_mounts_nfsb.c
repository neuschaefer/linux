/*
 * (c) 1997-2010 Netflix, Inc.  All content herein is protected by
 * U.S. copyright and other applicable intellectual property laws and
 * may not be copied without the express permission of Netflix, Inc.,
 * which reserves all rights.  Reuse of any of this content for any
 * purpose without the permission of Netflix, Inc. is strictly
 * prohibited.
 */
#include <linux/device-mapper.h>

#include "do_mounts.h"
#include "nfsb.h"

static char *DEV_LINEAR = "/dev/linear";
static char *DEV_NFSB = "/dev/nfsb";

/* RSA-2048 modulus, big-endian. Replace with your real value. */
static uint8_t rsa_modulus[256] =
{
    0xd2, 0x7c, 0x78, 0x2e, 0x53, 0x5f, 0x3c, 0x78,
	0x67, 0x72, 0x22, 0x4c, 0xc2, 0x60, 0xbd, 0xa9,
	0x6a, 0xc7, 0xf8, 0x25, 0x6c, 0x0c, 0x74, 0xf2,
	0x97, 0x32, 0x40, 0x2a, 0x30, 0x07, 0xb3, 0x3a,
	0x62, 0xb8, 0x3f, 0xa7, 0x11, 0xd9, 0x9c, 0xb6,
	0x69, 0x43, 0x1e, 0x97, 0xb4, 0x01, 0x1b, 0xc3,
	0xfa, 0xc1, 0x88, 0x85, 0x26, 0xa7, 0xf8, 0xd2,
	0xd5, 0x53, 0xf1, 0x28, 0x29, 0x3f, 0x19, 0xf9,
	0xa1, 0x67, 0x13, 0xe6, 0x12, 0x4a, 0x24, 0x2c,
	0x6e, 0x68, 0x55, 0xa6, 0x57, 0x44, 0x1b, 0x35,
	0xe1, 0x0c, 0x8a, 0xf1, 0x64, 0xd5, 0x27, 0x7b,
	0x74, 0x3a, 0x6b, 0x13, 0xb7, 0x92, 0x92, 0x3b,
	0xbc, 0x41, 0xf3, 0x0d, 0x0a, 0x1d, 0xaa, 0xa1,
	0x0f, 0xd3, 0x80, 0x75, 0xad, 0xf9, 0x82, 0x79,
	0xd5, 0xbc, 0x13, 0xb9, 0x07, 0x3b, 0x2c, 0xeb,
	0xc6, 0x23, 0xa8, 0xe5, 0x2d, 0x9a, 0xce, 0xd1,
	0x2c, 0xec, 0xed, 0x4d, 0xd6, 0x4a, 0x1b, 0x17,
	0x72, 0xba, 0xef, 0x0f, 0xd4, 0x40, 0x98, 0x05,
	0x76, 0xf1, 0x4e, 0x65, 0x37, 0x95, 0x5c, 0xe1,
	0x4a, 0x9b, 0x22, 0x66, 0x6e, 0xe3, 0x4b, 0x01,
	0xf9, 0xf0, 0x5a, 0xb3, 0xef, 0xc5, 0x82, 0x9a,
	0x43, 0xba, 0xa8, 0xbb, 0x0d, 0xbe, 0x16, 0xc6,
	0x41, 0x57, 0xc3, 0x01, 0x54, 0x65, 0x62, 0x50,
	0xfa, 0x0c, 0x9f, 0x8f, 0xa3, 0x46, 0x5c, 0x58,
	0x9f, 0xc1, 0xe0, 0xaa, 0xa1, 0x71, 0xc8, 0x9e,
	0x00, 0x7d, 0xa3, 0x11, 0xdd, 0xef, 0x1b, 0x1d,
	0x13, 0xad, 0x97, 0x03, 0xdf, 0xf5, 0x4a, 0xb5,
	0x73, 0xe1, 0x0e, 0xe3, 0x79, 0xea, 0x73, 0x51,
	0x6a, 0x72, 0x68, 0x8a, 0xf3, 0xa5, 0x64, 0xb2,
	0xf3, 0xd1, 0xe5, 0x6f, 0xc7, 0xaa, 0x7b, 0xef,
	0x3d, 0x5a, 0x91, 0x1c, 0x74, 0xda, 0x49, 0xc2,
	0x35, 0x51, 0xfa, 0xc7, 0x3d, 0xcd, 0xf1, 0x39
};

/* RSA-2048 public key exponent, big-endian. Replace with your real value. */
static uint8_t rsa_pubkey[256] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};

static int __init dm_check_nfsb(struct nfsb_header *hdr, dev_t dev)
{
	int err = 0;
	int root_fd = -1;
	ssize_t sz = -1;
	mpuint *pubkey = NULL, *modulus = NULL;

	/* Create a temporary root device. */
	err = create_dev(DEV_NFSB, dev);
	if (err < 0) {
		printk(KERN_ERR "nfsb: create_dev failed\n");
		goto create_fail;
	}
	
	/* Try to read an NFSB header from the root device. */
	root_fd = sys_open(DEV_NFSB, O_RDONLY, 0);
	if (root_fd < 0) {
		printk(KERN_ERR "nfsb: open %s failed\n", DEV_NFSB);
		goto open_fail;
	}
	sz = nfsb_read(root_fd, hdr);
	if (sz == -1)
		goto read_fail;

	/* Verify the NFSB header. */
	pubkey = mpuint_alloc(sizeof(rsa_pubkey));
	if (!pubkey) {
		printk(KERN_ERR "out of memory\n");
		goto pubkey_fail;
	}
	pubkey = mpuint_setbytes(pubkey, rsa_pubkey, sizeof(rsa_pubkey));
	if (!pubkey) {
		printk(KERN_ERR "mpuint_setbytes failed\n");
		goto pubkey_fail;
	}
	modulus = mpuint_alloc(sizeof(rsa_modulus));
	if (!modulus) {
		printk(KERN_ERR "out of memory\n");
		goto modulus_fail;
	}
	modulus = mpuint_setbytes(modulus, rsa_modulus, sizeof(rsa_modulus));
	if (!modulus) {
		printk(KERN_ERR "mpuint_setbytes failed\n");
		goto modulus_fail;
	}
	if (nfsb_verify(hdr, pubkey, modulus) != 0) {
		printk(KERN_ERR "nfsb: header verification failed\n");
		goto verify_fail;
	}
	printk(KERN_INFO "NFSB header found and verified.\n");

	/* Success. */
	mpuint_delete(modulus);
	mpuint_delete(pubkey);
	sys_close(root_fd);
	sys_unlink(DEV_NFSB);
	return 1;

verify_fail:
modulus_fail:
	mpuint_delete(modulus);
pubkey_fail:
	mpuint_delete(pubkey);
read_fail:
	sys_close(root_fd);
open_fail:
	sys_unlink(DEV_NFSB);
create_fail:
	return 0;
}

/* From drivers/md/dm.h */
#define DM_SUSPEND_NOFLUSH_FLAG         (1 << 1)
void dm_destroy(struct mapped_device *md);
int dm_table_alloc_md_mempools(struct dm_table *t);
void dm_table_destroy(struct dm_table *t);
int dm_table_set_type(struct dm_table *t);

static int __init dm_setup_linear(struct nfsb_header *hdr, dev_t *dev)
{
	int linear_fd = -1;
	unsigned long linear_size = 0;
	struct mapped_device *md = NULL;
	struct dm_table *table = NULL, *old_map = NULL;
	char *target_params = NULL;
	ssize_t fs_offset = to_sector(nfsb_fs_offset(hdr));

	/* Create linear device. */
	if (create_dev(DEV_LINEAR, *dev) < 0) {
		printk(KERN_ERR "Failed to create linear device\n");
		goto create_dev_fail;
	}

	/* Size block device. */
	linear_fd = sys_open(DEV_LINEAR, O_RDONLY, 0);
	if (linear_fd < 0) {
		printk(KERN_ERR "Failed to open linear device\n");
		goto sys_open_fail;
	}
	if (sys_ioctl(linear_fd, BLKGETSIZE, (unsigned long)&linear_size) < 0) {
		printk(KERN_ERR "Failed to get linear device size\n");
		goto sys_ioctl_fail;
	}

	/* Create a new mapped device. */
	if (dm_create(DM_ANY_MINOR, &md)) {
		printk(KERN_ERR "Failed to create mapped device\n");
		goto create_fail;
	}

	/* Explicitly set read-only. */
	set_disk_ro(dm_disk(md), 1);

	/* Create a mapped device table. */
	if (dm_table_create(&table, FMODE_READ, 1, md)) {
		printk(KERN_ERR "Failed to create table\n");
		goto table_create_fail;
	}

	/* Add the linear target to the table. */
	target_params = kmalloc(128, GFP_KERNEL);
	snprintf(target_params, 128, "%s %u", DEV_LINEAR, fs_offset);
	if (dm_table_add_target(table, "linear", 0, linear_size - fs_offset, target_params)) {
		printk(KERN_ERR "Failed to add linear target to the table\n");
		goto add_target_fail;
	}

	/* Table is complete. */
	if (dm_table_complete(table)) {
		printk(KERN_ERR "Failed to complete NFSB table\n");
		goto table_complete_fail;
	}

	/* Suspend the device so that we can bind it to the table. */
	if (dm_suspend(md, DM_SUSPEND_NOFLUSH_FLAG)) {
		printk(KERN_ERR "Failed to suspend the device pre-bind\n");
		goto suspend_fail;
	}

	/* Bind the table to the device. This is the only way to associate
	 * md->map with the table and set the disk capacity. Ouch. */
	old_map = dm_swap_table(md, table);
	if (IS_ERR(old_map)) {
		printk(KERN_ERR "Failed to bind the device to the table\n");
		dm_table_destroy(table);
		goto swap_table_fail;
	}
	
	/* Finally, resume and the device should be ready. */
	if (dm_resume(md)) {
		printk(KERN_ERR "Failed to resume the device\n");
		goto resume_fail;
	}

	/* Success. */
	*dev = disk_devt(dm_disk(md));
	printk(KERN_INFO "dm: target linear of size %llu on %s(%i:%i) is ready\n",
		   dm_table_get_size(table), dm_device_name(md),
		   MAJOR(*dev), MINOR(*dev));
	if (old_map)
		dm_table_destroy(old_map);
	kfree(target_params);
	sys_close(linear_fd);
	return 1;

resume_fail:
	if (old_map)
		dm_table_destroy(old_map);
swap_table_fail:
suspend_fail:
table_complete_fail:
add_target_fail:
	kfree(target_params);
	dm_table_destroy(table);
table_create_fail:
	dm_put(md);
	dm_destroy(md);
create_fail:
sys_ioctl_fail:
	sys_close(linear_fd);
sys_open_fail:
create_dev_fail:
	return 0;
}

static dev_t __init dm_setup_nfsb(struct nfsb_header *hdr, dev_t *dev)
{
	struct mapped_device *md = NULL;
	struct dm_table *table = NULL, *old_map = NULL;
	char *target_params = NULL;
	ssize_t fs_size = nfsb_fs_size(hdr);
	uint32_t data_blocksize = nfsb_data_blocksize(hdr);
	uint32_t hash_blocksize = nfsb_hash_blocksize(hdr);

	/* Verify data and hash start block indexes. */
	if (fs_size % hash_blocksize) {
		printk(KERN_ERR "Hash start is not a multiple of the hash block size.\n");
		goto hash_start_fail;
	}

	/* Create NFSB device. */
	if (create_dev(DEV_NFSB, *dev) < 0) {
		printk(KERN_ERR "Failed to create NFSB root device\n");
		goto create_dev_fail;
	}
	
	/* Create a new mapped device. */
	if (dm_create(DM_ANY_MINOR, &md)) {
		printk(KERN_ERR "Failed to create mapped device\n");
		goto create_fail;
	}

	/* Explicitly set read-only. */
	set_disk_ro(dm_disk(md), 1);

	/* Create a mapped device table. */
	if (dm_table_create(&table, FMODE_READ, 1, md)) {
		printk(KERN_ERR "Failed to create table\n");
		goto table_create_fail;
	}

	/* Add the NFSB verity target to the table. */
	target_params = kmalloc(384, GFP_KERNEL);
	snprintf(target_params, 384, "%u %s %s %u %u %u %u %s %s %s",
			 nfsb_hash_type(hdr), DEV_NFSB, DEV_NFSB,
			 data_blocksize, hash_blocksize, nfsb_data_blockcount(hdr),
			 fs_size / hash_blocksize, nfsb_hash_algo(hdr),
			 nfsb_verity_hash(hdr), nfsb_verity_salt(hdr));
	if (dm_table_add_target(table, "verity", 0, to_sector(fs_size), target_params)) {
		printk(KERN_ERR "Failed to add NFSB target to the table\n");
		goto add_target_fail;
	}

	/* Table is complete. */
	if (dm_table_complete(table)) {
		printk(KERN_ERR "Failed to complete NFSB table\n");
		goto table_complete_fail;
	}

	/* Suspend the device so that we can bind it to the table. */
	if (dm_suspend(md, DM_SUSPEND_NOFLUSH_FLAG)) {
		printk(KERN_ERR "Failed to suspend the device pre-bind\n");
		goto suspend_fail;
	}

	/* Bind the table to the device. This is the only way to associate
	 * md->map with the table and set the disk capacity. Ouch. */
	old_map = dm_swap_table(md, table);
	if (IS_ERR(old_map)) {
		printk(KERN_ERR "Failed to bind the device to the table\n");
		dm_table_destroy(table);
		goto swap_table_fail;
	}
	
	/* Finally, resume and the device should be ready. */
	if (dm_resume(md)) {
		printk(KERN_ERR "Failed to resume the device\n");
		goto resume_fail;
	}

	/* Success. */
	*dev = disk_devt(dm_disk(md));
	printk(KERN_INFO "dm: target verity of size %llu on %s(%i:%i) is ready\n",
		   dm_table_get_size(table), dm_device_name(md),
		   MAJOR(*dev), MINOR(*dev));
	if (old_map)
		dm_table_destroy(old_map);
	kfree(target_params);
	return 1;

resume_fail:
	if (old_map)
		dm_table_destroy(old_map);
swap_table_fail:
suspend_fail:
table_complete_fail:
add_target_fail:
	kfree(target_params);
	dm_table_destroy(table);
table_create_fail:
	dm_put(md);
	dm_destroy(md);
create_fail:
create_dev_fail:
hash_start_fail:
data_start_fail:
	return 0;
}

dev_t __init dm_mount_nfsb(dev_t dev)
{
	struct nfsb_header *hdr = NULL;

	hdr = kmalloc(sizeof(struct nfsb_header), GFP_KERNEL);
	if (!hdr)
		panic("Failed to allocate NFSB header struct.");

	printk(KERN_INFO "Checking for NFSB mount.\n");
	if (!dm_check_nfsb(hdr, dev))
		panic("No NFSB image found.");
	if (!dm_setup_linear(hdr, &dev))
		panic("Linear setup failed.");
	if (!dm_setup_nfsb(hdr, &dev)) {
		sys_unlink(DEV_LINEAR);
		panic("NFSB setup failed.");
	}

	kfree(hdr);
	return dev;
}
