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
#include <linux/types.h>
#include <linux/init.h>
#include <linux/input.h>

#include <linux/nbx_ec_ipc.h>
#include <linux/nbx_ec_ipc_lidsw.h>

static struct input_dev* nbx_lidsw_ec_dev;

static int only_once_after_resume;

static void nbx_lidsw_ec_changed(void)
{
	int lidsw_state;

	if(nbx_lidsw_ec_dev == NULL) return;

	lidsw_state = nbx_ec_ipc_lidsw_get_state();

	if(0 <= lidsw_state) {
		/* lid open & ec int factor is lid open then close -> open */
		if(only_once_after_resume) {
			if((lidsw_state == 0) && ec_ipc_get_ec_int_factor(EC_INT_LID) ){
				input_report_switch(nbx_lidsw_ec_dev, SW_LID, 1);
				input_sync(nbx_lidsw_ec_dev);
			}
		}
		only_once_after_resume = 0;

		input_report_switch(nbx_lidsw_ec_dev, SW_LID, lidsw_state);
		input_sync(nbx_lidsw_ec_dev);
	}
}

static int nbx_lidsw_ec_probe(struct nbx_ec_ipc_device* edev)
{
	int ret = 0;

	nbx_lidsw_ec_dev = input_allocate_device();
	if(nbx_lidsw_ec_dev == NULL) {
		pr_err("nbx_lidsw_ec:input_allocate_device() failed.\n");
		ret = -ENOMEM;
		goto error_exit;
	}

	nbx_lidsw_ec_dev->name = "nbx_lidsw";
	set_bit(EV_SW, nbx_lidsw_ec_dev->evbit);
	set_bit(SW_LID, nbx_lidsw_ec_dev->swbit);

	ret = input_register_device(nbx_lidsw_ec_dev);
	if(ret != 0) {
		pr_err("nbx_lidsw_ec:input_register_device() failed. %d\n", ret);
		goto error_exit;
	}

	ret = nbx_ec_ipc_lidsw_register_callback(nbx_lidsw_ec_changed);
	if(ret != 0) {
		pr_err("nbx_lidsw_ec:nbx_ec_ipc_lidsw_register_callback() failed. %d\n", ret);
		goto error_exit;
	}

	only_once_after_resume = 0;
	nbx_lidsw_ec_changed(); /* initial */

	return 0;

error_exit:
	input_unregister_device(nbx_lidsw_ec_dev);
	input_free_device(nbx_lidsw_ec_dev);
	nbx_ec_ipc_lidsw_unregister_callback(nbx_lidsw_ec_changed);
	nbx_lidsw_ec_dev = NULL;

	return ret;
}

static int nbx_lidsw_ec_remove(struct nbx_ec_ipc_device* edev)
{
	input_unregister_device(nbx_lidsw_ec_dev);
	input_free_device(nbx_lidsw_ec_dev);
	nbx_ec_ipc_lidsw_unregister_callback(nbx_lidsw_ec_changed);
	nbx_lidsw_ec_dev = NULL;

	return 0;
}

#ifdef CONFIG_SUSPEND

static int nbx_lidsw_ec_suspend(struct nbx_ec_ipc_device *edev)
{
	only_once_after_resume = 1;

	return 0;
}

#else /* !CONFIG_SUSPEND */
#define nbx_lidsw_ec_suspend NULL
#endif /* CONFIG_SUSPEND */

static struct nbx_ec_ipc_driver nbx_lidsw_ec_driver = {
	.probe   = nbx_lidsw_ec_probe,
	.remove  = nbx_lidsw_ec_remove,
	.suspend = nbx_lidsw_ec_suspend,
	.drv  = {
		.name = "nbx_lidsw",
	},
};

static int __init nbx_lidsw_ec_init(void)
{
	int ret;

	nbx_lidsw_ec_dev = NULL;

	ret = nbx_ec_ipc_driver_register(&nbx_lidsw_ec_driver);
	if (ret < 0) {
		pr_err("%s:nbx_ec_ipc_driver_register() failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}
static void  __exit nbx_lidsw_ec_exit(void)
{
	nbx_ec_ipc_driver_unregister(&nbx_lidsw_ec_driver);
}

module_init(nbx_lidsw_ec_init);
module_exit(nbx_lidsw_ec_exit);

MODULE_LICENSE("GPL");
