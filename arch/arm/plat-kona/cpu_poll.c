/************************************************************************************************/
/*                                                                                              */
/*  Copyright 2010  Broadcom Corporation                                                        */
/*                                                                                              */
/*     Unless you and Broadcom execute a separate written software license agreement governing  */
/*     use of this software, this software is licensed to you under the terms of the GNU        */
/*     General Public License version 2 (the GPL), available at                                 */
/*                                                                                              */
/*          http://www.broadcom.com/licenses/GPLv2.php                                          */
/*                                                                                              */
/*     with the following added to such license:                                                */
/*                                                                                              */
/*     As a special exception, the copyright holders of this software give you permission to    */
/*     link this software with independent modules, and to copy and distribute the resulting    */
/*     executable under terms of your choice, provided that you also meet, for each linked      */
/*     independent module, the terms and conditions of the license of that module.              */
/*     An independent module is a module which is not derived from this software.  The special  */
/*     exception does not apply to any modifications of the software.                           */
/*                                                                                              */
/*     Notwithstanding the above, under no circumstances may you combine this software in any   */
/*     way with any other Broadcom software provided under a license other than the GPL,        */
/*     without Broadcom's express prior written consent.                                        */
/*                                                                                              */
/************************************************************************************************/

#include <linux/module.h>
#include <linux/sysctl.h>

volatile int cpu_should_poll = 0;

static struct ctl_table_header *sysctl_header;

static struct ctl_table cpu_poll_sysctl[] = {
	{
		.procname      = "cpu-poll",
		.data          = &cpu_should_poll,
		.maxlen        = sizeof(cpu_should_poll),
		.mode          = 0644,
		.proc_handler  = &proc_dointvec,
	},
};

static int __init cpu_poll_sysctl_init(void)
{
	sysctl_header = register_sysctl_table(cpu_poll_sysctl);
	if (sysctl_header == NULL) {
		printk(KERN_ERR "%s: could not register sysctl table\n",
				__FUNCTION__);
	}
	return 0;
}

__initcall(cpu_poll_sysctl_init);
