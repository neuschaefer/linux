/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*Start of viewed for qos function */
/*
*    Copyright (c) 2003-2012 Broadcom Corporation
*    All Rights Reserved
*
<:label-BRCM:2012:GPL/GPL:standard

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/

/*
 *  ebt_ftos
 *
 *	Authors:
 *	 Song Wang <songw@broadcom.com>
 *
 *  Feb, 2004
 *
 */

// The ftos target can be used in any chain
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>
#include <linux/if_vlan.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ftos_t.h>

#include <net/dsfield.h>

#define PPPOE_HLEN   6
#define PPP_TYPE_IPV4   0x0021  /* IPv4 in PPP */
#define PPP_TYPE_IPV6   0x0057  /* IPv6 in PPP */

/* START OF Add:   FOR 移植BCM Eth上行的QoS */
#ifdef CONFIG_IP_PREC_TOS_REMARK
/* start IP precedence and TOS remark  */
#define QOS_IPP_MARK_ZERO   0x100 
#define QOS_TOS_MARK_ZERO   0x200  
#define QOS_DSCP_MARK       0x1 /* 区分ebtables ftos 是dscp还是tos或者ipp */

#define IPTOS_IPP_MASK		0xE0
/* end IP precedence and TOS remark */
#define IPTOS_DSCP_MASK		0xFC
#define IPTOS_DSCP(tos)		((tos)&IPTOS_DSCP_MASK)
#endif
/* END OF Add  */

/*Start   2.4G 设置为20M WMM功能不生效 */
#if defined(CONFIG_ARCH_SD56XX) 
#define SKB_MARK_DSCP_REMARK_ENABLE    0x8 
#endif
/*End   2.4G 设置为20M WMM功能不生效 */

/* START OF Add:  FOR 移植BCM Eth上行的QoS配置命令 */
#ifdef CONFIG_IP_PREC_TOS_REMARK
/* start of protocol mark precedence 或者tos字段需要保留原值A36D02507 
iph->tos = ftosinfo->ftos;
** NOTE:          tos field
**                bit 7 ~ bit 5 = precedence (0 = normal, 7 = extremely high)
**                bit 4 = D bit (minimize delay)
**                bit 3 = T bit (maximize throughput)
**                bit 2 = R bit (maximize reliability)
**                bit 1 = C bit (minimize transmission cost)
**                bit 0 = not used
iph->tos = ftosinfo->ftos;
*/
/* start IP precedence and TOS remark */
void ebt_get_DSCP_ftos(struct iphdr *iph, const struct ebt_ftos_t_info *ftosinfo)
{
	if (QOS_DSCP_MARK == (QOS_DSCP_MARK & ftosinfo->ftos))
    {
		iph->tos = (iph->tos & (~IPTOS_DSCP_MASK)) | IPTOS_DSCP(ftosinfo->ftos);
    }
    else
    {
        if ((QOS_TOS_MARK_ZERO & ftosinfo->ftos) && (QOS_IPP_MARK_ZERO & ftosinfo->ftos))
        {
            iph->tos = (iph->tos & 0x1);
        }
        else if (QOS_TOS_MARK_ZERO & ftosinfo->ftos)
        {
            if (IPTOS_PREC(ftosinfo->ftos))
            {
                iph->tos = IPTOS_PREC(ftosinfo->ftos) | (iph->tos & 0x1);
            }
            else
            {
                iph->tos = IPTOS_PREC(iph->tos) | (iph->tos & 0x1);
            }
        }
        else if (QOS_IPP_MARK_ZERO & ftosinfo->ftos)
        {
            if (IPTOS_TOS(ftosinfo->ftos))
            {
                iph->tos = IPTOS_TOS(ftosinfo->ftos) | (iph->tos & 0x1);
            }
            else
            {
                iph->tos = IPTOS_TOS(iph->tos) | (iph->tos & 0x1);
            }            
        }
        else
        {
            if (IPTOS_TOS(ftosinfo->ftos) 
                && IPTOS_PREC(ftosinfo->ftos))
            {
                iph->tos = ftosinfo->ftos;
            }
            else if (IPTOS_TOS(ftosinfo->ftos))
            {
                iph->tos = (iph->tos & (~IPTOS_TOS_MASK)) | IPTOS_TOS(ftosinfo->ftos);
            }
            else if (IPTOS_PREC(ftosinfo->ftos))
            {
                iph->tos = (iph->tos & (~IPTOS_PREC_MASK)) | IPTOS_PREC(ftosinfo->ftos);
            }
        }
    }
}
    /* end IP precedence and TOS remark  */
	/* end of protocol mark precedence 或者tos字段需要保留原值A36D02507 */
#endif
/* END OF Add */

static unsigned int ebt_ftos_tg(struct sk_buff *skb, const struct xt_action_param *par)   
{
	//struct ebt_ftos_t_info *ftosinfo = (struct ebt_ftos_t_info *)data;
	const  struct ebt_ftos_t_info *ftosinfo = par->targinfo;
	struct iphdr *iph = NULL;
	struct ipv6hdr *ipv6h = NULL;
        /* Need to recalculate IP header checksum after altering TOS byte */
	u_int16_t diffs[2];

	/* if VLAN frame, we need to point to correct network header */
   if (skb->protocol == __constant_htons(ETH_P_IP))
      iph = (struct iphdr *)(skb->network_header);
   else if ((skb)->protocol == __constant_htons(ETH_P_IPV6))
      ipv6h = (struct ipv6hdr *)(skb->network_header);
   else if (skb->protocol == __constant_htons(ETH_P_8021Q)) {
      if (*(unsigned short *)(skb->network_header + VLAN_HLEN - 2) == __constant_htons(ETH_P_IP))
         iph = (struct iphdr *)(skb->network_header + VLAN_HLEN);
      else if (*(unsigned short *)(skb->network_header + VLAN_HLEN - 2) == __constant_htons(ETH_P_IPV6))
         ipv6h = (struct ipv6hdr *)(skb->network_header + VLAN_HLEN);
      else if (*(unsigned short *)(skb->network_header + VLAN_HLEN - 2) == __constant_htons(ETH_P_PPP_SES))
      {
         if (*(unsigned short *)(skb->network_header + VLAN_HLEN + PPPOE_HLEN) == PPP_TYPE_IPV4)
         {
             iph = (struct iphdr *)(skb->network_header + VLAN_HLEN + PPPOE_HLEN + 2);
         }
      }
   }
   else if (skb->protocol == __constant_htons(ETH_P_PPP_SES)) {
      if (*(unsigned short *)(skb->network_header + PPPOE_HLEN) == PPP_TYPE_IPV4)
         iph = (struct iphdr *)(skb->network_header + PPPOE_HLEN + 2);
      else if (*(unsigned short *)(skb->network_header + PPPOE_HLEN) == PPP_TYPE_IPV6)
         ipv6h = (struct ipv6hdr *)(skb->network_header + PPPOE_HLEN + 2);
   }
   /* if not IP header, do nothing. */
   if ((iph == NULL) && (ipv6h == NULL))
	   return ftosinfo->target;

   if ( iph != NULL ) //IPv4
   {
	if ((ftosinfo->ftos_set & FTOS_SETFTOS) && (iph->tos != ftosinfo->ftos)) {
                //printk("ebt_target_ftos:FTOS_SETFTOS .....\n");
		diffs[0] = htons(iph->tos) ^ 0xFFFF;
/* START OF Add:  2011-05-21 FOR 移植BCM Eth上行的QoS */
#ifdef CONFIG_IP_PREC_TOS_REMARK        
        ebt_get_DSCP_ftos(iph, ftosinfo);
#else
		iph->tos = ftosinfo->ftos;
#endif
/* END OF Add:  2011-05-21 */

		diffs[1] = htons(iph->tos);
		iph->check = csum_fold(csum_partial((char *)diffs,
		                                    sizeof(diffs),
		                                    iph->check^0xFFFF));		
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// member below is removed
//		(*pskb)->nfcache |= NFC_ALTERED;
	} else if (ftosinfo->ftos_set & FTOS_WMMFTOS) {
	    //printk("ebt_target_ftos:FTOS_WMMFTOS .....0x%08x\n", (*pskb)->mark & 0xf);
      diffs[0] = htons(iph->tos) ^ 0xFFFF;
      iph->tos |= ((skb->mark >> PRIO_LOC_NFMARK) & PRIO_LOC_NFMASK) << DSCP_MASK_SHIFT;
      diffs[1] = htons(iph->tos);
      iph->check = csum_fold(csum_partial((char *)diffs,
		                                    sizeof(diffs),
		                                    iph->check^0xFFFF));
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// member below is removed
//        (*pskb)->nfcache |= NFC_ALTERED;
	} else if ((ftosinfo->ftos_set & FTOS_8021QFTOS) && skb->protocol == __constant_htons(ETH_P_8021Q)) {
	    
      struct vlan_hdr *frame;	
      unsigned char prio = 0;
      unsigned short TCI;

      frame = (struct vlan_hdr *)(skb->network_header);
      TCI = ntohs(frame->h_vlan_TCI);
      prio = (unsigned char)((TCI >> 13) & 0x7);
        //printk("ebt_target_ftos:FTOS_8021QFTOS ..... 0x%08x\n", prio);
      diffs[0] = htons(iph->tos) ^ 0xFFFF;
      iph->tos |= (prio & 0xf) << DSCP_MASK_SHIFT;
      diffs[1] = htons(iph->tos);
      iph->check = csum_fold(csum_partial((char *)diffs,
		                                    sizeof(diffs),
		                                    iph->check^0xFFFF)); 
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// member below is removed
//        (*pskb)->nfcache |= NFC_ALTERED;
	}
   }
   else //IPv6
   {
      __u8 tos;

      /* TOS consists of priority field and first 4 bits of flow_lbl */
      tos = ipv6_get_dsfield((struct ipv6hdr *)(skb->network_header));

      if ((ftosinfo->ftos_set & FTOS_SETFTOS) && ((tos>>2) != (ftosinfo->ftos>>2)))
      {
         //printk("ebt_target_ftos:FTOS_SETFTOS .....\n");
#ifdef CONFIG_IP_PREC_TOS_REMARK
         // IPv6 remark, clear QOS_DSCP_MARK(0x1)
         unsigned char tmp_ftos = ftosinfo->ftos & (~QOS_DSCP_MARK);
		 tmp_ftos = tmp_ftos |(tos & 0x3);
         ipv6_change_dsfield((struct ipv6hdr *)(skb->network_header), 0, tmp_ftos);
         //printk("ebt_target_ftos: FTOS_SETFTOS set tos=%x\n", tmp_ftos);
#else
         ipv6_change_dsfield((struct ipv6hdr *)(skb->network_header), 0, ftosinfo->ftos);
#endif

      } 
      else if (ftosinfo->ftos_set & FTOS_WMMFTOS) 
      {
         //printk("ebt_target_ftos:FTOS_WMMFTOS .....0x%08x\n", 
	     tos |= ((skb->mark >> PRIO_LOC_NFMARK) & PRIO_LOC_NFMASK) << DSCP_MASK_SHIFT;
         ipv6_change_dsfield((struct ipv6hdr *)(skb->network_header), 0, tos);
      } 
      else if ((ftosinfo->ftos_set & FTOS_8021QFTOS) && 
               skb->protocol == __constant_htons(ETH_P_8021Q)) 
      {
         struct vlan_hdr *frame;	
         unsigned char prio = 0;
         unsigned short TCI;

         frame = (struct vlan_hdr *)(skb->network_header);
         TCI = ntohs(frame->h_vlan_TCI);
         prio = (unsigned char)((TCI >> 13) & 0x7);
         //printk("ebt_target_ftos:FTOS_8021QFTOS ..... 0x%08x\n", prio);
         tos |= (prio & 0xf) << DSCP_MASK_SHIFT;
         ipv6_change_dsfield((struct ipv6hdr *)(skb->network_header), 0, tos);
      }
   }

	return ftosinfo->target;
}

static int ebt_ftos_tg_check(const struct xt_tgchk_param *par)
{
	const struct ebt_ftos_t_info *info = par->targinfo;
/*
	if (datalen != sizeof(struct ebt_ftos_t_info))
		return -EINVAL;
*/
	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;
	
	//CLEAR_BASE_CHAIN_BIT;
	
	if (INVALID_TARGET)
		return -EINVAL;
	
	return 0;
}

static struct xt_target ebt_ftos_tg_reg = {
	.name       = EBT_FTOS_TARGET,
	.revision   = 0,
	.family     = NFPROTO_BRIDGE,
	.target     = ebt_ftos_tg,
	.checkentry = ebt_ftos_tg_check,
	.targetsize = XT_ALIGN(sizeof(struct ebt_ftos_t_info)),
	.me         = THIS_MODULE,
};

static int __init ebt_ftos_init(void)
{
	int ret;
	ret = xt_register_target(&ebt_ftos_tg_reg);
	if(ret == 0)
		printk(KERN_INFO "ebt_ftos registered\n");

	return ret;
}

static void __exit ebt_ftos_fini(void)
{
	xt_unregister_target(&ebt_ftos_tg_reg);
}

module_init(ebt_ftos_init);
module_exit(ebt_ftos_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Song Wang, songw@broadcom.com");
MODULE_DESCRIPTION("Target to overwrite the full TOS byte in IP header");
/*End of viewed for qos function */
