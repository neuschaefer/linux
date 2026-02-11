/*****************************************************************************
*  Copyright 2012 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_fdt.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/broadcom/vc_dt.h>

struct vc_dt_data {
	int valid;
	uint32_t base;
	uint32_t size;
};
struct vc_dt_data vc_dt_data = {0};

int __init early_init_dt_scan_vc(unsigned long node, const char *uname,
				     int depth, void *data)
{
	const char *prop;
	unsigned long size;
	uint32_t *uprop;

	if (depth != 1 || strncmp(uname, "videocore", strlen("videocore")) != 0)
		return 0;

	prop = of_get_flat_dt_prop(node, "reg", &size);
	if (prop == NULL)
		printk(KERN_INFO "%s: videocore reg not found\n", __func__);
	else {
		uprop = (uint32_t *)prop;

		vc_dt_data.base = be32_to_cpu(uprop[0]);
		vc_dt_data.size = be32_to_cpu(uprop[1]);

		printk(KERN_INFO "videocore reg: @0x%x, 0x%x Mb\n",
			vc_dt_data.base, vc_dt_data.size);
	}

	vc_dt_data.valid = 1;
	return 1;
}

int vc_dt_get_load_and_size(uint32_t *load, uint32_t *size)
{
	if (!vc_dt_data.valid)
		return -EINVAL;

	if ((load == NULL) || (size == NULL))
		return -EINVAL;

	*load = vc_dt_data.base;
	*size = vc_dt_data.size;

	return 0;
}
EXPORT_SYMBOL_GPL(vc_dt_get_load_and_size);

