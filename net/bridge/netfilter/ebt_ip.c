/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/in.h>
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ip.h>
#include <linux/if_vlan.h>

struct tcpudphdr {
	__be16 src;
	__be16 dst;
};


#ifdef CONFIG_IP_PREC_TOS_REMARK
/* IP precedence and TOS remark  */
#define QOS_DSCP_MARK       0x1 /* 区分ebtables ftos 是dscp还是tos或者ipp */
#define QOS_IPP_MARK_ZERO   0x100 
#define QOS_TOS_MARK_ZERO   0x200 

#define IPTOS_IPP_MASK		0xE0
/* end IP precedence and TOS remark  */
#define IPTOS_DSCP_MASK		0xFC
#define IPTOS_DSCP(tos)		((tos)&IPTOS_DSCP_MASK)
#endif

static bool
ebt_ip_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct ebt_ip_info *info = par->matchinfo;
	const struct iphdr *ih;
	struct iphdr _iph;
	const struct tcpudphdr *pptr;
	struct tcpudphdr _ports;
    int ipoffset = 0;

#ifdef CONFIG_BRIDGE_EBT_QINQ
    /* Start of macfilter support QinQ 20060714 */
    unsigned char* p8021q = skb->data;
	struct vlan_hdr *vhdr = NULL;
	u16 vlan_id;
	u16 vlan_prio;
	u16 vlan_tci;

    if (skb->protocol != __constant_htons(ETH_P_8021Q))
    {
        if (info->bitmask & EBT_IP_8021P)
        {
            return false;
        }
    }
    
    if (skb->protocol == __constant_htons(ETH_P_8021Q))
    {
    	vhdr = (struct vlan_hdr *)skb->data;
    	vlan_tci = ntohs(vhdr->h_vlan_TCI);
    	vlan_id = vlan_tci & VLAN_VID_MASK;
        vlan_prio = (vlan_tci & 0xe000) >> 13;
        //printk("fangj : vlanid = %d, vlanprio = %d\n", vlan_id, vlan_prio);

        if (info->bitmask & EBT_IP_8021P &&
    	    FWINV(info->vlan_8021p != vlan_prio, EBT_IP_8021P))
    		return false;
        
    	if (info->bitmask & EBT_IP_8021Q &&
    	    FWINV(info->vlan_8021q != vlan_id, EBT_IP_8021Q))
    		return false;
        
        p8021q += 2;
        ipoffset = p8021q - skb->data;
        if (*(unsigned short*)p8021q == __constant_htons(ETH_P_PPP_SES))
        {
            if (*(unsigned short*)(p8021q + 2 + 6) == __constant_htons(0x0021))
            {
                p8021q += (2 + 6 + 2); // VLAN+PPP+IP
                ipoffset = p8021q - skb->data;
            }
            else
            {
                // 其他情况
		        return false;
            }
        }
        else if (*(unsigned short*)p8021q == __constant_htons(ETH_P_IP))
        {
            p8021q += 2; // VLAN+IP
            ipoffset = p8021q - skb->data;
        }
        else
        {
	        return false;
        }
    }
    else if(skb->protocol == __constant_htons(ETH_P_PPP_SES))
    {
        if(*(unsigned short*)(p8021q + 6) == __constant_htons(0x0021))
        {
            p8021q += (6 + 2); // PPP+IP
            ipoffset = p8021q - skb->data;
        }
        else
        {
	        return false;
        }
    }
    else if(skb->protocol == __constant_htons(ETH_P_IP))
    {
        ipoffset = 0;
    }
    else
    {
        return false;
    }
	ih = skb_header_pointer(skb, ipoffset, sizeof(_iph), &_iph);
    /* End of macfilter support QinQ 20060714 */
#else
	ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
#endif
	if (ih == NULL)
		return false;
    /* dscp check */
    /*Start of : support TOS class for QOS 20090710*/    
#if !defined CONFIG_IP_PREC_TOS_REMARK
	if (info->bitmask & EBT_IP_TOS &&
	   FWINV((info->tos & 0xFC) != (ih->tos & 0xFC), EBT_IP_TOS))
		return false;
#else
    /* start IP precedence and TOS remark  */
    if (QOS_DSCP_MARK == (QOS_DSCP_MARK & info->tos))
    {
    	if (info->bitmask & EBT_IP_TOS &&
    	   FWINV((info->tos & 0xFC) != (ih->tos & 0xFC), EBT_IP_TOS))
    		return false;
    }
    else
    {
        if ((QOS_TOS_MARK_ZERO & info->tos) && (QOS_IPP_MARK_ZERO & info->tos))
        {
        	if (info->bitmask & EBT_IP_TOS &&
        	   FWINV(IPTOS_TOS(ih->tos) != 0, EBT_IP_TOS))
        		return false; 
            
        	if (info->bitmask & EBT_IP_TOS &&
        	   FWINV(IPTOS_PREC(ih->tos) != 0, EBT_IP_TOS))
        		return false;
        }
        else if (QOS_TOS_MARK_ZERO & info->tos)
        {
        	if (info->bitmask & EBT_IP_TOS &&
        	   FWINV(IPTOS_TOS(ih->tos) != 0, EBT_IP_TOS))
        		return false; 
            
        	if (info->bitmask & EBT_IP_TOS &&
        	   FWINV(IPTOS_PREC(info->tos) != IPTOS_PREC(ih->tos), EBT_IP_TOS))
        		return false;
        }
        else if (QOS_IPP_MARK_ZERO & info->tos)
        {
        	if (info->bitmask & EBT_IP_TOS &&
        	   FWINV(IPTOS_TOS(info->tos) != IPTOS_TOS(ih->tos), EBT_IP_TOS))
        		return false; 
            
        	if (info->bitmask & EBT_IP_TOS &&
        	   FWINV(IPTOS_PREC(ih->tos) != 0, EBT_IP_TOS))
        		return false;
        }
        else 
        {
            if (IPTOS_TOS(info->tos) 
                && IPTOS_PREC(info->tos))
            {
            	if (info->bitmask & EBT_IP_TOS &&
            	   FWINV(info->tos != ih->tos, EBT_IP_TOS))
            		return false;        
            }
            else if (IPTOS_TOS(info->tos))
            {
            	if (info->bitmask & EBT_IP_TOS &&
            	   FWINV(IPTOS_TOS(info->tos) != IPTOS_TOS(ih->tos), EBT_IP_TOS))
            		return false;          
            }
            else if (IPTOS_PREC(info->tos))
            {
            	if (info->bitmask & EBT_IP_TOS &&
            	   FWINV(IPTOS_PREC(info->tos) != IPTOS_PREC(ih->tos), EBT_IP_TOS))
            		return false;          
            }
        }
    }
    /* end IP precedence and TOS remark  */
#endif
    /*end of : support TOS class for QOS 20090710*/
#if defined(CONFIG_BCM_KF_NETFILTER) || !defined(CONFIG_BCM_IN_KERNEL)
	if (info->bitmask & EBT_IP_DSCP &&
	   FWINV(info->dscp != (ih->tos & 0xFC), EBT_IP_DSCP))
		return false;
#endif      
	if (info->bitmask & EBT_IP_SOURCE &&
	   FWINV((ih->saddr & info->smsk) !=
	   info->saddr, EBT_IP_SOURCE))
		return false;
	if ((info->bitmask & EBT_IP_DEST) &&
	   FWINV((ih->daddr & info->dmsk) !=
	   info->daddr, EBT_IP_DEST))
		return false;
#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE		
    if (info->bitmask & EBT_IP_SRANGE) {
        u32 saddr = ntohl(ih->saddr);
        //printk("Ritchie line:%d saddr %x, min_p:%x, max_ip:%x \n",__LINE__,saddr,(info->src).min_ip,(info->src).max_ip);
        if (FWINV(saddr < ntohl((info->src).min_ip) ||
              saddr > ntohl((info->src).max_ip),
              EBT_IP_SRANGE))
        return false;
    }
    if (info->bitmask & EBT_IP_DRANGE) {
        u32 daddr = ntohl(ih->daddr);
        if (FWINV(daddr < ntohl((info->dst).min_ip) ||
              daddr > ntohl((info->dst).max_ip),
              EBT_IP_DRANGE))
        return false;
    }
#endif	
	if (info->bitmask & EBT_IP_PROTO) {
		if (FWINV(info->protocol != ih->protocol, EBT_IP_PROTO))
			return false;
		if (!(info->bitmask & EBT_IP_DPORT) &&
		    !(info->bitmask & EBT_IP_SPORT))
			return true;
		if (ntohs(ih->frag_off) & IP_OFFSET)
			return false;
		
		/* Start of modified  for qos lan+vlan flow 2012-5-25 */
		//pptr = skb_header_pointer(skb, ih->ihl*4, sizeof(_ports), &_ports);
		pptr = skb_header_pointer(skb, ipoffset + ih->ihl*4,
    					  sizeof(_ports), &_ports);
		/* End of modified  for qos lan+vlan flow 2012-5-25 */
		
		if (pptr == NULL)
			return false;
		if (info->bitmask & EBT_IP_DPORT) {
			u32 dst = ntohs(pptr->dst);
			if (FWINV(dst < info->dport[0] ||
				  dst > info->dport[1],
				  EBT_IP_DPORT))
			return false;
		}
		if (info->bitmask & EBT_IP_SPORT) {
			u32 src = ntohs(pptr->src);
			if (FWINV(src < info->sport[0] ||
				  src > info->sport[1],
				  EBT_IP_SPORT))
			return false;
		}
	}
	return true;
}

static int ebt_ip_mt_check(const struct xt_mtchk_param *par)
{
	const struct ebt_ip_info *info = par->matchinfo;
	const struct ebt_entry *e = par->entryinfo;

	if (e->ethproto != htons(ETH_P_IP) ||
	   e->invflags & EBT_IPROTO)
		return -EINVAL;
	if (info->bitmask & ~EBT_IP_MASK || info->invflags & ~EBT_IP_MASK)
		return -EINVAL;
	if (info->bitmask & (EBT_IP_DPORT | EBT_IP_SPORT)) {
		if (info->invflags & EBT_IP_PROTO)
			return -EINVAL;
		if (info->protocol != IPPROTO_TCP &&
		    info->protocol != IPPROTO_UDP &&
		    info->protocol != IPPROTO_UDPLITE &&
		    info->protocol != IPPROTO_SCTP &&
		    info->protocol != IPPROTO_DCCP)
			 return -EINVAL;
	}
	if (info->bitmask & EBT_IP_DPORT && info->dport[0] > info->dport[1])
		return -EINVAL;
	if (info->bitmask & EBT_IP_SPORT && info->sport[0] > info->sport[1])
		return -EINVAL;
#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE				
	if (info->bitmask & EBT_IP_SRANGE && (info->src).min_ip > (info->src).max_ip)
    {   
        //printk("Ritchie minip(%x) bigger than maxip(%x)\n",(info->src).min_ip, (info->src).max_ip);
		return -EINVAL;
    }
	if (info->bitmask & EBT_IP_DRANGE && (info->dst).min_ip > (info->dst).max_ip)
    {   
        //printk("Ritchie minip(%x) bigger than maxip(%x)\n",(info->dst).min_ip, (info->dst).max_ip);    
		return -EINVAL;
    }
#endif	
	return 0;
}

static struct xt_match ebt_ip_mt_reg __read_mostly = {
	.name		= "ip",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_ip_mt,
	.checkentry	= ebt_ip_mt_check,
	.matchsize	= sizeof(struct ebt_ip_info),
	.me		= THIS_MODULE,
};

static int __init ebt_ip_init(void)
{
	return xt_register_match(&ebt_ip_mt_reg);
}

static void __exit ebt_ip_fini(void)
{
	xt_unregister_match(&ebt_ip_mt_reg);
}

module_init(ebt_ip_init);
module_exit(ebt_ip_fini);
MODULE_DESCRIPTION("Ebtables: IPv4 protocol packet match");
MODULE_LICENSE("GPL");
