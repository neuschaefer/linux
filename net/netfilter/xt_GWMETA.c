/* Kernel module to set flags in ti_gw_meta field of skb.
 * 
 *
 * 
 *
 * Copyright (C) 2011-2014, Intel Ltd.
 * All Rights Reserved.
 ** Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *  notice, this list of conditions, and the following disclaimer,
 *  without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *  derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *
 * 
 *
   You should have received a copy of the GNU General Public License 
   along with this program; if not, write to the Free Software 
   Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
   The full GNU General Public License is included in this distribution 
   in the file called LICENSE.GPL.
  
    Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
 * Description:
 *   This module provide extends netfilter feature to set a bitmask on a packets.
 *   In difference from netfiltyer this masks lasts till packet leaves GateWay TCP stack 
 *
 *   The module follows the Netfilter framework, called extended packet
 *   matching modules.
 * Usage :
 *  iptables -t mangle -I POSTROUTING 1 -o rndbr1 -p tcp --dport 6800:6866 -j GWMETA --gwmeta-gwmask 0x00000020
 *  iptables -t mangle -I OUTPUT 1 -o rndbr1 -p tcp --dport 6800:6866 -j GWMETA --gwmeta-gwmask 0x00000002
 *  Packet will have resulted ti_gw_meta ORed mask 0x22 
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter/xt_GWMETA.h>

//#define GWMETA_DEBUG
#ifdef GWMETA_DEBUG
    #define DEBUGP printk
    #define DEBUG_LEVEL KERN_CRIT
#else
    #define DEBUGP(DEBUG_LEVEL, format, args...)
#endif


static unsigned int
do_target_job(struct sk_buff *skb, const struct xt_action_param *par)
{

    const struct xt_gw_skb_rule_info *info = par->targinfo;

    if (info->type == XT_GW_META)
    {
        DEBUGP( DEBUG_LEVEL " %s: mark with i %x \n", __FUNCTION__, info->gwmask);
        skb->ti_gw_meta |= info->gwmask;
    }
    else if (info->type == XT_GW_DIS_PP)
    {
        /*DISABLE PP */
        TI_HIL_ADD_PP_INFO_FLAGS(skb, TI_HIL_PACKET_FLAG_PP_SESSION_BYPASS);
    }
    else if (info->type == XT_GW_EN_PP)
    {
        /*ENABLE PP */
        TI_HIL_SET_PP_INFO_FLAGS(skb, TI_HIL_GET_PP_INFO_FLAGS(skb) & ~(TI_HIL_PACKET_FLAG_PP_SESSION_BYPASS));
    }

    return XT_CONTINUE;
}

static int
check_usage(const struct xt_tgchk_param *par )
{
    //const struct ipt_entry *e = (struct ipt_entry*)e_void;
    //const struct ipt_gw_skb_rule_info *info = targinfo;

    /* this module might be used at any table and chain
       no check is needed */

    DEBUGP( DEBUG_LEVEL "%s: \n", __FUNCTION__);


    return 0;
}

static struct xt_target redirect_tg_reg[] __read_mostly = {
    {
        .name          = "GWMETA",
        .family      = NFPROTO_IPV4,
        .target      = do_target_job,
        .targetsize  = sizeof(struct xt_gw_skb_rule_info),
        .checkentry  = check_usage,
        .me          = THIS_MODULE,
    },
#if IS_ENABLED(CONFIG_IPV6)
    {
        .name          = "GWMETA",
        .family      = NFPROTO_IPV6,
        .target      = do_target_job,
        .targetsize  = sizeof(struct xt_gw_skb_rule_info),
        .checkentry  = check_usage,
        .me          = THIS_MODULE,
    }
#endif
};

static int __init ipt_gwmeta_init(void)
{
    return xt_register_targets(redirect_tg_reg,
                   ARRAY_SIZE(redirect_tg_reg));
}

static void __exit ipt_gwmeta_fini(void)
{
    xt_unregister_targets(redirect_tg_reg, ARRAY_SIZE(redirect_tg_reg));
}

module_init(ipt_gwmeta_init);
module_exit(ipt_gwmeta_fini);

MODULE_LICENSE("GPL");
MODULE_ALIAS("ip6t_GWMETA");
MODULE_ALIAS("ipt_GWMETA");

