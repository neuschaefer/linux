/*
  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution
  in the file called LICENSE.GPL.

  Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
*/

#include <linux/module.h>
#include <net/sock.h>
#include <linux/netfilter.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_forward.h>
#include <linux/rculist.h>
#include "../br_private.h"

static unsigned int
ebt_forward_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct ebt_forward_info *info = par->targinfo;
	struct net_bridge_port *p;

	if (!skb_make_writable(skb, 0))
		return EBT_DROP;

	list_for_each_entry_rcu(p, &br_port_get_rcu(par->in)->br->port_list, list) {
		if(strcmp(info->dst, p->dev->name) == 0) {
			skb->bridge_forward_port = p;
			goto out;
		}
	}
out:
	return info->target;
}

static int ebt_forward_tg_check(const struct xt_tgchk_param *par)
{
	const struct ebt_forward_info *info = par->targinfo;
	unsigned int hook_mask;

	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;

	hook_mask = par->hook_mask & ~(1 << NF_BR_NUMHOOKS);
	if ((strcmp(par->table, "nat") != 0 ||
	    (hook_mask & ~((1 << NF_BR_PRE_ROUTING) |
	    (1 << NF_BR_LOCAL_OUT)))) &&
	    (strcmp(par->table, "broute") != 0 ||
	    hook_mask & ~(1 << NF_BR_BROUTING)))
		return -EINVAL;
	if (INVALID_TARGET)
		return -EINVAL;
	return 0;
}

static struct xt_target ebt_forward_tg_reg __read_mostly = {
	.name		= "forward",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.hooks		= (1 << NF_BR_NUMHOOKS) | (1 << NF_BR_PRE_ROUTING) |
			  (1 << NF_BR_LOCAL_OUT) | (1 << NF_BR_BROUTING),
	.target		= ebt_forward_tg,
	.checkentry	= ebt_forward_tg_check,
	.targetsize	= sizeof(struct ebt_forward_info),
	.me		= THIS_MODULE,
};

static int __init ebt_forward_init(void)
{
	return xt_register_target(&ebt_forward_tg_reg);
}

static void __exit ebt_forward_fini(void)
{
	xt_unregister_target(&ebt_forward_tg_reg);
}

module_init(ebt_forward_init);
module_exit(ebt_forward_fini);
MODULE_DESCRIPTION("Ebtables: forwarding override extension");
MODULE_LICENSE("GPL");
