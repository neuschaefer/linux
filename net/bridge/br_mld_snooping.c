/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/if_vlan.h>
#include <net/ipv6.h>
#include <asm/atomic.h>
#include "br_private.h"
#include "br_mld_snooping.h"

#define NIP6(addr) \
	ntohs((addr).s6_addr16[0]), \
	ntohs((addr).s6_addr16[1]), \
	ntohs((addr).s6_addr16[2]), \
	ntohs((addr).s6_addr16[3]), \
	ntohs((addr).s6_addr16[4]), \
	ntohs((addr).s6_addr16[5]), \
	ntohs((addr).s6_addr16[6]), \
	ntohs((addr).s6_addr16[7])
#define NIP6_FMT "%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x"
#define NIP6_SEQFMT "%04x%04x%04x%04x%04x%04x%04x%04x"

struct net_bridge_mld_snooping mld_snooping_list;

#if 0
#define MLD_SNOOPING_P(str, args...)                                \
do                                                                  \
{                                                                   \
    printk("[%s %s:%d]  ",__FILE__, __FUNCTION__, __LINE__);        \
    printk("\033[1;33;40m");                                        \
    printk(str, ## args);                                           \
    printk("\033[0;37;40m\n");                                      \
}while(0) 
#else
#define MLD_SNOOPING_P(str, args...)
#endif    



void br_mld_snooping_init(void)
{
	INIT_LIST_HEAD(&mld_snooping_list.mld_list);
	mld_snooping_list.mld_lock = SPIN_LOCK_UNLOCKED;
	mld_snooping_list.mld_start_timer = 0;
	/*默认开启mld snooping*/
	mld_snooping_list.mld_snooping_enable = 1;
}

static void br_mld_snooping_query_timeout(unsigned long ptr)
{
	struct net_bridge_mld_snooping_entry *dst_entry, *tmp_entry;
	struct net_bridge_mld_snooping *mld_list;

	mld_list = (struct net_bridge_mld_snooping *) ptr;

	spin_lock_bh(&mld_list->mld_lock);
    /*START MODIFY:Huawei 2012-11-20 FOR mld snooping cpu 0*/
    /*存在del和free操作的遍历，需要使用safe函数*/
    list_for_each_entry_safe(dst_entry, tmp_entry, &mld_list->mld_list, list) 
    {
	    if ((jiffies > dst_entry->time) 
                && ((jiffies - dst_entry->time) < (MLD_QUERY_TIMEOUT * HZ)))
	    {
    		list_del_rcu(&dst_entry->list);
    		kfree(dst_entry);
	    }
	}
    /*END MODIFY:Huawei 2012-11-20 FOR mld snooping cpu 0*/
	spin_unlock_bh(&mld_list->mld_lock);
		
	mod_timer(&mld_list->mld_timer, jiffies + MLD_TIMER_CHECK_TIMEOUT * HZ);	

    return ;
}

static int br_mld_snooping_update(struct net_bridge_port *dev_port, 
                const unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_mld_snooping_entry *dst_entry;
	int ret = 0;
    
	list_for_each_entry_rcu(dst_entry, &mld_snooping_list.mld_list, list)
	{
	    if (!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN))
	    {
    		if ((!memcmp(&dst_entry->host_mac, host_mac, ETH_ALEN)) 
                    && (dst_entry->dev_dst == dev_port))
    		{    		
                dst_entry->time = jiffies + (MLD_QUERY_TIMEOUT * HZ);
    		    ret = 1;
    		}
	    }
	}
	
	return ret;
}

static struct net_bridge_mld_snooping_entry *br_mld_snooping_get(
    struct net_bridge_port *dev_port, unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_mld_snooping_entry *dst_entry;

	list_for_each_entry_rcu(dst_entry, &mld_snooping_list.mld_list, list)
	{
	    if ((!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN)) 
            && (!memcmp(&dst_entry->host_mac, host_mac, ETH_ALEN))) 
	    {
		    if (dst_entry->dev_dst == dev_port)
		    {
    		    return dst_entry;
		    }
	    }
	}
	
	return NULL;
}

static int br_mld_snooping_add(struct net_bridge_port *dev_port, 
    const unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_mld_snooping_entry *mld_entry;

    // TODO: 特殊MAC地址过滤
		    
	if (br_mld_snooping_update(dev_port, grp_mac, host_mac))
	{
    	return 0;
	}
	
    /*START MODIFY:Huawei 2012-11-20 FOR schedule while atomic问题*/
    /*bh上下文被调用的内存分配函数应使用ATOMIC操作*/
    mld_entry = 
        kmalloc(sizeof(struct net_bridge_mld_snooping_entry), GFP_ATOMIC);
	if (!mld_entry)
	{
	    return ENOMEM;
	}	
    /*END MODIFY:Huawei 2012-11-20 FOR schedule while atomic问题*/
	memcpy(mld_entry->host_mac, host_mac, ETH_ALEN);	
	memcpy(mld_entry->grp_mac, grp_mac, ETH_ALEN);
	mld_entry->time = jiffies + MLD_QUERY_TIMEOUT*HZ;
    mld_entry->dev_dst = dev_port;
    
	spin_lock_bh(&mld_snooping_list.mld_lock);
	list_add_rcu(&mld_entry->list, &mld_snooping_list.mld_list);
	spin_unlock_bh(&mld_snooping_list.mld_lock);

	if (!mld_snooping_list.mld_start_timer) 
	{
    	init_timer(&mld_snooping_list.mld_timer);
	    mld_snooping_list.mld_timer.expires = jiffies + MLD_TIMER_CHECK_TIMEOUT*HZ;
	    mld_snooping_list.mld_timer.function = br_mld_snooping_query_timeout;
	    mld_snooping_list.mld_timer.data = (unsigned long)(&mld_snooping_list);
	    add_timer(&mld_snooping_list.mld_timer);
	    mld_snooping_list.mld_start_timer = 1;
	}

	return 1;
}


/* 从给定的grp_mac中删除给定的主机 host_mac */
static int br_mld_snooping_del(struct net_bridge_port *dev_port, 
            unsigned char *grp_mac, unsigned char *host_mac)
{
	struct net_bridge_mld_snooping_entry *dst_entry;
    
	if ((dst_entry = br_mld_snooping_get(dev_port, grp_mac, host_mac))) 
	{
    	spin_lock_bh(&mld_snooping_list.mld_lock);
	    list_del_rcu(&dst_entry->list);
	    kfree(dst_entry);
            
    	spin_unlock_bh(&mld_snooping_list.mld_lock);

	    return 1;
	}
	
	return 0;
}

int br_mld_snooping_del_by_nbp(struct net_bridge_port *dev_port)
{
	struct net_bridge_mld_snooping_entry *dst_entry, *tmp_entry;

	spin_lock_bh(&mld_snooping_list.mld_lock);
	list_for_each_entry_safe(dst_entry, tmp_entry, &mld_snooping_list.mld_list, list)
	{
		if (dst_entry->dev_dst == dev_port) 
		{
			    list_del_rcu(&dst_entry->list);
			    kfree(dst_entry);
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
			    blog_notify(MCAST_CONTROL_EVT, (void*)dst_entry,
							BLOG_PARAM1_MCAST_DEL, BLOG_PARAM2_MCAST_IPV6);
#endif   

		}
	}
    spin_unlock_bh(&mld_snooping_list.mld_lock);
    
	return 0;
}

static int br_mld_snooping_empty(unsigned char *grp_mac)
{
	struct net_bridge_mld_snooping_entry *dst_entry;
    
	spin_lock_bh(&mld_snooping_list.mld_lock);
	list_for_each_entry_rcu(dst_entry, &mld_snooping_list.mld_list, list)
	{
	    if ((!memcmp(&dst_entry->grp_mac, grp_mac, ETH_ALEN))) 
	    {
			spin_unlock_bh(&mld_snooping_list.mld_lock);
	    	return 0;
	    }
	}
	spin_unlock_bh(&mld_snooping_list.mld_lock);
	return 1;
}

void br_mld_snooping_show(void)
{
	struct net_bridge_mld_snooping_entry *dst_entry;

	printk("\nmld snooping: <%s>\n",
        (mld_snooping_list.mld_snooping_enable? "Enable":"Disable"));

	printk("\nbridge	device	group			source			timeout\n");
	list_for_each_entry_rcu(dst_entry, &mld_snooping_list.mld_list, list)
	{
	    if (NULL == dst_entry->dev_dst)
	    {
	        continue;
	    }
	    printk("%s   %s  	",dst_entry->dev_dst->br->dev->name, 
            dst_entry->dev_dst->dev->name);

        printk("%02x:%02x:%02x:%02x:%02x:%02x",dst_entry->grp_mac[0],
                                               dst_entry->grp_mac[1],
                                               dst_entry->grp_mac[2],
                                               dst_entry->grp_mac[3],
                                               dst_entry->grp_mac[4],
                                               dst_entry->grp_mac[5]);
	    printk("	");

        printk("%02x:%02x:%02x:%02x:%02x:%02x",dst_entry->host_mac[0],
                                               dst_entry->host_mac[1],
                                               dst_entry->host_mac[2],
                                               dst_entry->host_mac[3],
                                               dst_entry->host_mac[4],
                                               dst_entry->host_mac[5]);
	    printk("	%d\n", (int) (dst_entry->time - jiffies)/HZ);
	}
}


void br_mld_snooping_clear(void)
{
	struct net_bridge_mld_snooping_entry *dst_entry, *tmp_entry;
   
	spin_lock_bh(&mld_snooping_list.mld_lock);
    /*START MODIFY:Huawei 2012-11-20 FOR mld snooping cpu 0*/
	list_for_each_entry_safe(dst_entry, tmp_entry, &mld_snooping_list.mld_list, list)
	{
	    list_del_rcu(&dst_entry->list);
	    kfree(dst_entry);
	}
    /*END MODIFY:Huawei 2012-11-20 FOR mld snooping cpu 0*/
	del_timer(&(mld_snooping_list.mld_timer));
    
	spin_unlock_bh(&mld_snooping_list.mld_lock);
}

int br_mld_snooping_forward(struct sk_buff *skb, 
        struct net_bridge *br,const unsigned char *dest,int forward)
{
#ifndef CONFIG_QUALCOMM_CHIP
    int k;
    int status = 0;
    int iNum = 0;
    unsigned char mc_mac[ETH_ALEN];
    unsigned char *data = skb->data;
    unsigned short protocol ;
    struct sk_buff *skb2;
    struct net_bridge_mld_snooping_entry *dst_entry;        
    struct mld2_reportpkt *pstMLDv2Ptr = NULL;
    struct mld2_grecrd *pstRdPtr = NULL;
    char *pszPtr = NULL;
    struct ipv6hdr *pstIP6Hdr = NULL;
    int addr_type;
    int iMaddrType;

#ifdef CONFIG_ATP_BRIDGE_BIND
    struct bridgebind_group *pgroup = NULL;
#endif

    protocol = ((u16 *) data)[-1];

    if ((!mld_snooping_list.mld_snooping_enable)
        || (protocol != __constant_htons(ETH_P_IPV6)))
    {
        return 0;
    }

    pstIP6Hdr = (struct ipv6hdr *)(skb->data); 
    
    addr_type = ipv6_addr_type(&pstIP6Hdr->daddr);

    /* 如果不是多播地址那么退出 */
    if (!(addr_type & IPV6_ADDR_MULTICAST))
    {
        MLD_SNOOPING_P("Error: destination [ "NIP6_FMT" ] no multicast\r\n",NIP6(pstIP6Hdr->daddr));
        
        return 0;
    }
    
    // TODO:特殊MAC地址过滤
    
    if ((IPPROTO_ICMPV6 == data[40])
        && ((ICMPV6_MGM_REPORT == data[48])
             || (ICMPV6_MGM_REDUCTION == data[48]) 
             || (ICMPV6_MLD2_REPORT == data[48]))) 
    {
        if ((!skb->dev) || (NULL == br_port_get_rcu(skb->dev)))
        {
            return 0;
        }

        /* 报文长度校验 */
        if (MLD_PAKCET_LEN > ntohs(pstIP6Hdr->payload_len))
        {
            MLD_SNOOPING_P("PACKET length error: %d\r\n", 
                ntohs(pstIP6Hdr->payload_len));
            return 0;
        }

        /* 跳数校验 */
        if (MLD_HOP_LIMIT != pstIP6Hdr->hop_limit)
        {
            MLD_SNOOPING_P("Hoplimit error: %d must 1\r\n", pstIP6Hdr->hop_limit);
            return 0;
        }
        

        /* v1报告报文 */
        if (ICMPV6_MGM_REPORT == data[48])
        {
            MLD_SNOOPING_P("mld v1 report: [ "NIP6_FMT" ]-->[ "NIP6_FMT" ]\r\n",
                NIP6(pstIP6Hdr->saddr), NIP6(pstIP6Hdr->daddr));

            /* 因为多播地址具有不同的范围，所以需要根据范围来进行确认 */
            if (IN6_IS_ADDR_MC_NODELOCAL(&pstIP6Hdr->daddr)
                || IN6_IS_ADDR_MC_LINKLOCAL(&pstIP6Hdr->daddr))
            {
                MLD_SNOOPING_P("multicast scope error ["NIP6_FMT"]\r\n", NIP6(pstIP6Hdr->daddr));
                return 0;
            }
            
            br_mld_snooping_add(br_port_get_rcu(skb->dev), dest, eth_hdr(skb)->h_source);
        }
        else if (ICMPV6_MLD2_REPORT == data[48])        /* mldv2 报告报文 */
        {
            pstMLDv2Ptr = (struct mld2_reportpkt *)&data[48];
            pszPtr = (char *)(pstMLDv2Ptr + 1);
            iNum = ntohs(pstMLDv2Ptr->ngrec);

            MLD_SNOOPING_P("mld version 2 packet...\r\n");
            
            for (k = 0; k < iNum; k++)
            {
                pstRdPtr = (struct mld2_grecrd *)pszPtr;
                iMaddrType = ipv6_addr_type(&pstRdPtr->grec_mca);
                if (!(iMaddrType & IPV6_ADDR_MULTICAST)
                    || (IN6_IS_ADDR_MC_NODELOCAL(&pstRdPtr->grec_mca)
                    || IN6_IS_ADDR_MC_LINKLOCAL(&pstRdPtr->grec_mca)))
                {
                    MLD_SNOOPING_P("multicast scope error ["NIP6_FMT"]\r\n", NIP6(pstRdPtr->grec_mca));
                    pszPtr += sizeof(struct mld2_grecrd) 
                        + ntohs(pstRdPtr->grec_nsrcs) * sizeof(struct in6_addr);
                    continue;
                }
                
                mc_mac[0] = 0x33;
                mc_mac[1] = 0x33;
                mc_mac[2] = pstRdPtr->grec_mca.s6_addr[12];
                mc_mac[3] = pstRdPtr->grec_mca.s6_addr[13];
                mc_mac[4] = pstRdPtr->grec_mca.s6_addr[14];
                mc_mac[5] = pstRdPtr->grec_mca.s6_addr[15];
                
                if ((MLD_V2_MART_MODE_IS_EXCLUDE == pstRdPtr->grec_type) 
                    || (MLD_V2_MART_CHANGE_TO_EXCLUDE_MODE == pstRdPtr->grec_type) 
                    || ((ntohs(pstRdPtr->grec_nsrcs) != 0)
                            && (MLD_V2_MART_MODE_IS_INCLUDE == pstRdPtr->grec_type))
                    || ((ntohs(pstRdPtr->grec_nsrcs) != 0) 
                            && (MLD_V2_MART_CHANGE_TO_INCLUDE_MODE == pstRdPtr->grec_type)) 
                    || (MLD_V2_MART_ALLOW_NEW_SOURCES == pstRdPtr->grec_type)) 
                {
        		    br_mld_snooping_add(br_port_get_rcu(skb->dev), mc_mac, eth_hdr(skb)->h_source);
        		}
        		else if ((MLD_V2_MART_CHANGE_TO_INCLUDE_MODE == pstRdPtr->grec_type 
                            && ntohs(pstRdPtr->grec_nsrcs) == 0) 
                        || (ntohs(pstRdPtr->grec_nsrcs == 0)
                            && MLD_V2_MART_MODE_IS_INCLUDE == pstRdPtr->grec_type) 
                        || (MLD_V2_MART_BLOCK_OLD_SOURCES == pstRdPtr->grec_type)) 
                {
                    MLD_SNOOPING_P("br_mld_snooping_del\n");
        		    br_mld_snooping_del(br_port_get_rcu(skb->dev), mc_mac, eth_hdr(skb)->h_source);
        		}
                
                pszPtr += sizeof(struct mld2_grecrd) 
                    + ntohs(pstRdPtr->grec_nsrcs) * sizeof(struct in6_addr);
            }
        }
        else if (ICMPV6_MGM_REDUCTION == data[48]) 
        {             
            struct mld1_msg *pst1Rpt = NULL;
            pst1Rpt = (struct mld1_msg *)&data[48];
            mc_mac[0] = 0x33;
            mc_mac[1] = 0x33;
            mc_mac[2] = pst1Rpt->stMAddr.s6_addr[12];
            mc_mac[3] = pst1Rpt->stMAddr.s6_addr[13];
            mc_mac[4] = pst1Rpt->stMAddr.s6_addr[14];
            mc_mac[5] = pst1Rpt->stMAddr.s6_addr[15];
            MLD_SNOOPING_P("br_mld_snooping_del\n");
            br_mld_snooping_del(br_port_get_rcu(skb->dev), mc_mac, eth_hdr(skb)->h_source);

            /* 如果该组已经空了 */
            if (br_mld_snooping_empty(mc_mac))
            {
                skb2 = skb;
            
                if (forward)
                {
                    br_flood_forward(br, skb, skb2);
                }
                else
                {
                    br_flood_deliver(br, skb2);
                }
            }		    
            status = 1;
        }
        return status;
    }

    /* 非MLD报文则会直接走这里 */
    list_for_each_entry_rcu(dst_entry, &mld_snooping_list.mld_list, list)
    {        
        if (!memcmp(&dst_entry->grp_mac, dest, ETH_ALEN)) 
        {
            if (NULL == dst_entry->dev_dst)
            {
                continue;
            }
#ifdef CONFIG_ATP_BRIDGE_BIND
            if(br->group_enable)
            {
                pgroup = br_get_group(br, brgetgroup(br, skb));

                if(((NULL != pgroup)
                && (ISIN_GROUP(pgroup->ports, dst_entry->dev_dst->port_no)))
                || (!forward)) //路由WAN的组播流不判断绑定组
                {
                    skb2 = skb_clone(skb, GFP_ATOMIC);
                    
                    if (NULL == skb2)
                    {
                        return 0;
                    }                    
                
                    if (forward)
                    {
                        br_forward(dst_entry->dev_dst, skb2);
                    }
                    else
                    {
                        br_deliver(dst_entry->dev_dst, skb2);
                    }
                }

                if( NULL != pgroup ) 
                {
                    br_release_group(pgroup);
                }
            }
            else
            {
#endif

                if (forward) //桥接snooping
                {

                    if (0 == memcmp(dst_entry->dev_dst->br->dev->name,
                        br_port_get_rcu(skb->dev)->br->dev->name,
                        strlen(br_port_get_rcu(skb->dev)->br->dev->name)))
                    {
                        skb2 = skb;
                        br_forward(dst_entry->dev_dst, skb, skb2);
                    }
                }
                else  //路由snooping
                {

                    if (0 == memcmp(dst_entry->dev_dst->br->dev->name,skb->dev->name,strlen(skb->dev->name)))
                    {

                        skb2 = skb_clone(skb, GFP_ATOMIC);
                        if (NULL == skb2)
                        {
                            return 0;
                        }
                        br_deliver(dst_entry->dev_dst, skb2);
                    }
                } 
#ifdef CONFIG_ATP_BRIDGE_BIND                
            }
#endif            
            status = 1;
        }	    
    }

#if 0
    memset(mc_mac, 0xff, 6);
    if((1 == mld_snooping_list.mld_snooping_enable) 
          && memcmp(mc_mac, eth_hdr(skb)->h_dest, ETH_ALEN))
    {
        status = 1;
    }
#endif

    if ((IPPROTO_ICMPV6 == skb->data[6]) 
        || (0x02 == skb->data[40] && 0x22 == skb->data[41] && 0x02 == skb->data[42] && 0x23 == skb->data[43])
        || (0x07 == skb->data[42] && 0x6c == skb->data[43])
        || ((IPPROTO_ICMPV6 == data[40]) && (ICMPV6_MGM_QUERY == data[48])))
    {
        //一些组播报文不能被过滤，比如RA ,DHCPV6 SOLICIT, multicast query,upnp
    }
    else
    {
        if (mld_snooping_list.mld_snooping_enable && br_mld_snooping_empty(eth_hdr(skb)->h_dest))
        {
            status = 1;
        }
    }
    
    
    if ((!forward) && (status))
    {
        kfree_skb(skb);
    }

    return status;
#else
    return 0;
#endif
}

void br_mld_snooping_set_enable(int enable)
{
    MLD_SNOOPING_P("Set mld snooping switch [%d]\r\n", enable);
    
	spin_lock_bh(&mld_snooping_list.mld_lock);
    mld_snooping_list.mld_snooping_enable = enable;
	spin_unlock_bh(&mld_snooping_list.mld_lock);
}

int br_mld_snooping_get_enable(void)
{
    return mld_snooping_list.mld_snooping_enable;
}


