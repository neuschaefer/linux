/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#include <linux/nbx_ec_ipc.h>

static ssize_t ec_sysfs_version_show(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
	int ret;

#define EC_IPC_CID_VERSION 0x8F

	enum {
		EC_VERSION_RES_MAJOR = 0,
		EC_VERSION_RES_MINOR,
		NOOF_EC_VERSION_RES,
	};
	uint8_t res_buf[NOOF_EC_VERSION_RES];

	ret = ec_ipc_send_request(EC_IPC_PID_SYSFS, EC_IPC_CID_VERSION,
				NULL, 0,
				res_buf, sizeof(res_buf) );

	if(ret < (int)sizeof(res_buf)) {
		pr_err("ec_sysfs: ec_ipc_send_request(VERSION) failed. %d\n", ret);
		return -EIO;
	}

	return snprintf(buf, PAGE_SIZE, "%x.%x", res_buf[EC_VERSION_RES_MAJOR], res_buf[EC_VERSION_RES_MINOR]);
}
static struct kobj_attribute ec_sysfs_version_attr =
	__ATTR(version, 0440, ec_sysfs_version_show, NULL);

struct kobject* sysfs_firmware_ec_kobj = NULL;
EXPORT_SYMBOL(sysfs_firmware_ec_kobj);

static int __init ec_sysfs_init(void)
{
	int ret;

	if(sysfs_firmware_ec_kobj == NULL) {
		sysfs_firmware_ec_kobj = kobject_create_and_add("ec", firmware_kobj);
		if(sysfs_firmware_ec_kobj == NULL) {
			ret = -ENOMEM;
			goto err_exit;
		}
	}

	ret = sysfs_create_file(sysfs_firmware_ec_kobj, &ec_sysfs_version_attr.attr);
	if(ret != 0) {
		pr_err("ec_sysfs: sysfs_create_file() failed. %d\n", ret);
		goto err_exit;
	}

	return 0;

err_exit:
	sysfs_remove_file(sysfs_firmware_ec_kobj, &ec_sysfs_version_attr.attr);
	kobject_del(sysfs_firmware_ec_kobj);
	sysfs_firmware_ec_kobj = NULL;

	return ret;
}

static void __exit ec_sysfs_exit(void)
{
	sysfs_remove_file(sysfs_firmware_ec_kobj, &ec_sysfs_version_attr.attr);
}

module_init(ec_sysfs_init);
module_exit(ec_sysfs_exit);

MODULE_LICENSE("GPL");
