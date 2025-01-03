/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/types.h>
#include <linux/syscalls.h>
#include <linux/mount.h>
#include <linux/slab.h>
#include "tz_mtk_crypto_api.h"
#include "skeyprovider.h"

#define KEY_PARTITION_MAGIC "SKEYPARTITION"
#define LEN_KEY_PARTITION_MAGIC 16
#define KEY_PARTITION "/dev/skey_partition"
#define KEY_MAGIC	"SKEY"
#define LEN_MAGIC	4
#define LEN_KEYLEN	4
#define LEN_KEYNAME 8
#define MAXKEY_LEN	32
#define MAXKEY_NUM	10
#define MAX_PARTFOUND_NUM 10

struct key_struct {
	char magic[LEN_MAGIC];
	char name[LEN_KEYNAME];
	int	len;
	unsigned char key[MAXKEY_LEN];
};

struct skey_partition_struct {
	char magic[LEN_KEY_PARTITION_MAGIC];
	struct key_struct keys[MAXKEY_NUM];
};

static inline int create_dev(char *name, dev_t dev)
{
	sys_unlink(name);
	return sys_mknod(name, S_IFBLK|0600, new_encode_dev(dev));
}

/* partition read function before rootfs mount */
static inline int root_read(char *name, char *buf, size_t count)
{
	dev_t skey_dev;
	int fd = -1;

	skey_dev = name_to_dev_t(name);
	create_dev(KEY_PARTITION, skey_dev);

	fd = sys_open(KEY_PARTITION, O_RDONLY, 0);
	if (fd < 0) {
		sys_unlink(KEY_PARTITION);
		return -1;
	}

	sys_read(fd, buf, count);

	sys_close(fd);
	sys_unlink(KEY_PARTITION);

	return 0;
}

/* partition read function after rootfs mount */
static inline int nonroot_read(char *name, char *buf, size_t count)
{
	ssize_t sz = -1;
	mm_segment_t oldfs;
	struct file *fp;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	fp = filp_open(name, O_RDONLY, 0);
	if (!fp) {
		pr_err("error:nonroot open %s failed\n", name);
		return -1;
	}

	vfs_read(fp, buf, count, &fp->f_pos);

	filp_close(fp, NULL);
	set_fs(oldfs);

	return 0;
}

static inline int part_read(char *name, char *buf, size_t count)
{
	if (root_read(name, buf, count) == 0)
		return 0;
	if (nonroot_read(name, buf, count) == 0)
		return 0;
	return -EFAULT;
}

int find_skeypartition(void)
{
	char *part_name_tmpl = "/dev/mmcblk0p%d";
	int idx = 1;

	for( idx = 1; idx < MAX_PARTFOUND_NUM; idx++) {
		char part_name[32] = {0};
		char h_buff[LEN_KEY_PARTITION_MAGIC] = {0};

		snprintf(part_name, 32, part_name_tmpl, idx);

		if(part_read(part_name, (char __user *)(h_buff), LEN_KEY_PARTITION_MAGIC))
			continue;

		if(memcmp(h_buff, KEY_PARTITION_MAGIC, strlen(KEY_PARTITION_MAGIC)))
			continue;

		return idx;
	}

	return -1;
}

/* keyname must be [keyname] or [keyname]@@@@@@@@.... */
#define MAX_KEYNAME_LEN 64
#define KEYNAME_PLACEHOLDER '@'
int check_keyname_pattern(unsigned char *keyname, int ph_s)
{
	int idx = 0;
	int slen = 0;

	slen = strnlen(keyname, MAX_KEYNAME_LEN);

	for(idx = ph_s; idx < slen; idx++) {
		if (keyname[idx] != KEYNAME_PLACEHOLDER)
			return -1;
	}

	return 0;
}

int is_mandatory_keyname_pattern(unsigned char *keyname)
{
	int idx = 0;
	int slen = 0;
	int ret = 1;

	slen = strnlen(keyname, MAX_KEYNAME_LEN);

	for( ; idx < slen; idx++) {
		if (keyname[idx] == KEYNAME_PLACEHOLDER)
			break;
	}

	for( ; idx < slen; idx++) {
		if (keyname[idx] != KEYNAME_PLACEHOLDER) {
			ret = 0;
			break;
		}
	}

	return ret;
}

/* return value:
 * 0 means success found
 * 1 means mandaory key not found
 * -1 means normal key not found (means it may not affect flow)
 */
int get_encrypted_key(unsigned char* keyname, unsigned char *key, int keylen)
{
	int fd;
	dev_t skey_dev;
	int part_idx = -1;
	char part_name[32] = {0};
	int idx = -1;
	int is_mandatory  = 0;
	struct skey_partition_struct* skeycontent;

	is_mandatory = is_mandatory_keyname_pattern(keyname);

	if(is_mandatory)
		pr_err("mandatory key mode!");
	else
		pr_err("normal key mode!");

	part_idx = find_skeypartition();
	if(part_idx == -1) {
		pr_err("error: can not found skeypartition\n");
		return is_mandatory ? MANDATORY_KEY_NOT_FOUND : NORMAL_KEY_NOT_FOUND;
	}

	snprintf(part_name, 32, "/dev/mmcblk0p%d", part_idx);

	skeycontent = kzalloc(sizeof(struct skey_partition_struct), GFP_KERNEL);
	part_read(part_name, (char __user *)(skeycontent), sizeof(struct skey_partition_struct));

	for( idx = 0; idx < MAXKEY_NUM; idx++) {
		if(memcmp(skeycontent->keys[idx].magic, KEY_MAGIC, strlen(KEY_MAGIC))){
			kfree(skeycontent);
			return is_mandatory ? MANDATORY_KEY_NOT_FOUND : NORMAL_KEY_NOT_FOUND;
		}
		if((!memcmp(skeycontent->keys[idx].name, keyname, strlen(skeycontent->keys[idx].name))) &&
			(skeycontent->keys[idx].len == keylen) &&
			(check_keyname_pattern(keyname, strlen(skeycontent->keys[idx].name)) == 0)) {
				memcpy(key, skeycontent->keys[idx].key, keylen);
				kfree(skeycontent);
				return KEY_FOUND_SUCCESS;
		}
	}

	kfree(skeycontent);
	return is_mandatory ? MANDATORY_KEY_NOT_FOUND : NORMAL_KEY_NOT_FOUND;
}


int get_derived_key(char *key, int keylen, char* dst)
{
	struct mtk_crypto_ctx *crypto_ctx;
	char *t_derived;
	int i = 0;
	
	crypto_ctx = kzalloc(sizeof(struct mtk_crypto_ctx), GFP_KERNEL);
	mtk_crypto_ctx_init(crypto_ctx);

	t_derived = kzalloc(keylen, GFP_KERNEL);
	for (; i < keylen; i++)
		t_derived[i] = i;
	
	mtk_crypto_aes_with_keymode(crypto_ctx, 1, 0,
						    AES_OP_MODE_DEC, NULL,
						    key, keylen,
						    t_derived, dst,
						    keylen);

	mtk_crypto_ctx_init(crypto_ctx);
	return 0;
}
