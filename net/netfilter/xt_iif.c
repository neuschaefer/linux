/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 *	xt_connmark - Netfilter module to match connection mark values
 *
 *	Copyright (C) 2002,2004 MARA Systems AB <http://www.marasystems.com>
 *	by Henrik Nordstrom <hno@marasystems.com>
 *	Copyright © CC Computer Consultants GmbH, 2007 - 2008
 *	Jan Engelhardt <jengelh@computergmbh.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/netfilter/nf_conntrack.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter/xt_iif.h>

MODULE_AUTHOR("Kevert < >");
MODULE_DESCRIPTION("Xtables: in dev index match");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_iif");
MODULE_ALIAS("ip6t_iif");

static bool
iif_mt_v0(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct xt_iif_info *info = par->matchinfo;

	return ((skb->iif) == info->uiIf) ^ info->invert;
}

static bool iif_mt_check_v0(const struct xt_mtchk_param *par)
{
	const struct xt_iif_info *cm = par->matchinfo;

	if (0 >= cm->uiIf) {
		printk(KERN_WARNING "error: interface index error\n");
		return false;
	}
    
	return true;
}

static void iif_mt_destroy(const struct xt_mtdtor_param *par)
{
	return ;
}

#ifdef CONFIG_COMPAT
struct compat_xt_iif_info {
	unsigned int uiIf;
	u_int8_t	invert;
	u_int8_t	__pad1;
	u_int16_t	__pad2;
};

static void iif_mt_compat_from_user_v0(void *dst, void *src)
{
	const struct xt_iif_info *cm = src;
	struct xt_iif_info m = {
        .ulIf = cm->uiIf;
		.invert	= cm->invert,
	};
	memcpy(dst, &m, sizeof(m));
}

static int iif_mt_compat_to_user_v0(void __user *dst, void *src)
{
	const struct xt_iif_info *m = src;
	struct xt_iif_info cm = {
        .ulIf = cm->uiIf;
		.invert	= cm->invert,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}
#endif /* CONFIG_COMPAT */

static struct xt_match iif_mt_reg[] __read_mostly = {
	{
		.name		= "iif",
		.revision	= 0,
		.family		= NFPROTO_UNSPEC,
		.checkentry	= iif_mt_check_v0,
		.match		= iif_mt_v0,
		.destroy	= iif_mt_destroy,
		.matchsize	= sizeof(struct xt_iif_info),
#ifdef CONFIG_COMPAT
		.compatsize	= sizeof(struct compat_xt_iif_info),
		.compat_from_user = iif_mt_compat_from_user_v0,
		.compat_to_user	= iif_mt_compat_to_user_v0,
#endif
		.me		= THIS_MODULE
	},
};

static int __init iif_mt_init(void)
{
	return xt_register_matches(iif_mt_reg,
	       ARRAY_SIZE(iif_mt_reg));
}

static void __exit iif_mt_exit(void)
{
	xt_unregister_matches(iif_mt_reg, ARRAY_SIZE(iif_mt_reg));
}

module_init(iif_mt_init);
module_exit(iif_mt_exit);
