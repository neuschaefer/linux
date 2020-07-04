/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/igmp.h>
#include "br_private.h"
#include "br_igmp_dev_list.h"
#include "br_netlink_mcpd.h"

//#define BR_DEV_DEBUG
#ifdef BR_DEV_DEBUG
#define br_igmp_debug(args...)   color_printk(__func__, __LINE__, _RET_IP_, ## args)
#else
#define br_igmp_debug(args...)
#endif



#define ROUTE_WAN   1
#define BRIDGE_WAN  2

#define MC_FDB_ADD_OP   (1)
#define MC_FDB_DEL_OP   (2)

static void br_igmp_dump_wan_info(t_MCPD_IGMP_SNOOP_ENTRY *p);

static void br_igmp_fill_wan_info(t_MCPD_IGMP_SNOOP_ENTRY *p);
static void br_igmpv12_process(struct net_bridge *br, struct sk_buff *skb, 
    struct iphdr *pip, struct igmphdr *pstIGMPHdr, int op);
static unsigned int br_igmpv3_rrd_process(struct net_bridge *br, 
    struct sk_buff *skb, struct iphdr *pip, struct igmpv3_grec *pstGrec, int op);
static void br_igmpv3_parse(struct net_bridge *br, struct sk_buff *skb, 
        struct iphdr *pip, struct igmpv3_report *pstRepV3);


#ifdef CONFIG_IGMP_SSM
static int br_IgmpSsmPackets(struct igmpv3_grec *pstGrec)
{
	/*RFC4604 abort Exclude mode in SSM group*/
	if ((IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type && 0 != pstGrec->grec_nsrcs)
        || (IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type && 0 != pstGrec->grec_nsrcs)
        || (IGMPV3_ALLOW_NEW_SOURCES == pstGrec->grec_type && 0 != pstGrec->grec_nsrcs)
        || (IGMPV3_BLOCK_OLD_SOURCES == pstGrec->grec_type && 0 != pstGrec->grec_nsrcs))
    {
    	return 1;
    }

    return 0;
}

static int br_IgmpAsmPackets(struct igmpv3_grec *pstGrec)
{
	if ((IGMPV3_MODE_IS_EXCLUDE == pstGrec->grec_type && 0 == pstGrec->grec_nsrcs) 
        || (IGMPV3_CHANGE_TO_EXCLUDE == pstGrec->grec_type && 0 == pstGrec->grec_nsrcs)
        || (IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type && 0 == pstGrec->grec_nsrcs)
        || (IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type && 0 == pstGrec->grec_nsrcs))
    {
    	return 1;
    }

    return 0;
}
static int br_igmpv3_ssm_validate_grec(struct igmpv3_grec *pstGrec)
{
	if (NULL == pstGrec)
    {
    	return 1;
    }
    
    if ((IGMPV3_MODE_IS_EXCLUDE == pstGrec->grec_type && 0 != pstGrec->grec_nsrcs)
        || (IGMPV3_CHANGE_TO_EXCLUDE == pstGrec->grec_type && 0 != pstGrec->grec_nsrcs))
    {
    	return 1;
    }

    if (ip_mc_is_ssm(pstGrec->grec_mca))
    {
    	if (br_IgmpAsmPackets(pstGrec))
		{
			return 1;
		}
    }
	else if (ip_mc_is_asm(pstGrec->grec_mca))
    {
    	if (br_IgmpSsmPackets(pstGrec))
        {
        	return 1;
        }
    }
    else if (IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type)
    {
    	return 1;
    }

    return 0;
}

static int br_igmpv12_ssm_validate_grec(__be32 group)
{
	if (ip_mc_is_ssm(group))
    {
    	return 1;
    }
    
    return 0;
}
#endif

static inline int br_igmp_is_route(struct net_device *dev)
{
    if (!dev)
    {
        br_igmp_debug("br_igmp_is_route\r\n");
        return ROUTE_WAN;
    }
    
    if (br_port_get_rcu(dev))
    {
        return BRIDGE_WAN;
    }
    return ROUTE_WAN;
}

static void br_igmp_dump_wan_info(t_MCPD_IGMP_SNOOP_ENTRY *p)
{
    int i = 0;
    printk("wan information:=====\r\n");
    for (i = 0; i < MCPD_MAX_IFS; i++)
    {
        printk("wan: %10s  type: %10s\r\n", 
            p->wan_info[i].if_name, p->wan_info[i].if_ops & MCPD_IF_TYPE_BRIDGED? "bridge":"route");
    }
    printk("=========\r\n");
}



static void br_igmp_fill_wan_info(t_MCPD_IGMP_SNOOP_ENTRY *p)
{
    int i = 0;
    int wantype = 0;
    br_igmp_debug("br_igmp_fill_wan_info\r\n");    
    struct multicast_snooping_device_list *header = br_snooping_get_dev_header();
    struct multicast_snooping_device  *entry = NULL;

    br_snooping_dev_lock();
    list_for_each_entry(entry, &header->list, list)
    {
        br_igmp_debug("br_igmp_fill_wan_info i:%d\r\n",i);
        if (MCPD_MAX_IFS <= i)
        {
            break;
        }
        wantype = br_igmp_is_route(entry->netdev);
        if (ROUTE_WAN == wantype)
        {
            p->wan_info[i].if_ops = MCPD_IF_TYPE_ROUTED;
        }
        else if (BRIDGE_WAN == wantype)
        {
            p->wan_info[i].if_ops = MCPD_IF_TYPE_BRIDGED;
        }
        else
        {
            continue;
        }
        strncpy(p->wan_info[i].if_name, entry->WanName, IFNAMSIZ);
        i++;
    }
    br_snooping_dev_unlock();

#ifdef BR_DEV_DEBUG
    br_igmp_dump_wan_info(p);
#endif
    
}

static void br_igmpv12_process(struct net_bridge *br, struct sk_buff *skb, struct iphdr *pip, 
    struct igmphdr *pstIGMPHdr, int op)
{
    t_MCPD_IGMP_SNOOP_ENTRY stEntry;

    if (!skb->dev || !br_port_get_rcu(skb->dev) || !br_port_get_rcu(skb->dev)->br)
    {
        br_igmp_debug("error: dev is not bridge port\r\n");
        return ;
    }

#if 0
    int i = 0;
    for (i = 0; i < 40; i++)
    {
        printk("0x%x ",skb->data[i]);
        if (!((i+1)%10))
        {
            printk("\n");
        }
    }    

    if (skb->data_len < ((pip->ihl << 2) + sizeof(struct igmphdr)))
    {
        br_igmp_debug("error: skb data is not long enough, skb len: %d\r\n", skb->data_len);
        return ;
    }
#endif    

    memset(&stEntry, 0, sizeof(stEntry));
    strncpy(stEntry.br_name, br->dev->name, IFNAMSIZ);
    stEntry.port_no = br_port_get_rcu(skb->dev)->port_no;
    stEntry.grp.s_addr = pstIGMPHdr->group;
    stEntry.destGrp.s_addr = pstIGMPHdr->group;
    stEntry.src.s_addr = 0;
    stEntry.rep.s_addr = pip->saddr;
    stEntry.tci = 0;
    if (MC_FDB_ADD_OP == op)
    {
        stEntry.mode = MCPD_SNOOP_EX_ADD;
    }
    else
    {
        stEntry.mode = MCPD_SNOOP_EX_CLEAR;
    }
    
#ifdef CONFIG_IGMP_SSM
    if (br_igmpv12_ssm_validate_grec(pstIGMPHdr->group))
    {
    	return;
    }
#endif

    br_igmp_fill_wan_info(&stEntry);

    mcpd_nl_process_igmp_snoop_entry_api(&stEntry);

    return ;
}



static unsigned int br_igmpv3_rrd_process(struct net_bridge *br, struct sk_buff *skb, struct iphdr *pip,
    struct igmpv3_grec *pstGrec, int op)
{
    t_MCPD_IGMP_SNOOP_ENTRY stEntry;
    memset(&stEntry, 0, sizeof(stEntry));
    strncpy(stEntry.br_name, br->dev->name, IFNAMSIZ);
    stEntry.port_no = br_port_get_rcu(skb->dev)->port_no;
    stEntry.grp.s_addr = pstGrec->grec_mca;
    stEntry.destGrp.s_addr = pstGrec->grec_mca;
    stEntry.src.s_addr = 0;
    stEntry.rep.s_addr = pip->saddr;
    stEntry.tci = 0;
    
    if (MC_FDB_ADD_OP == op)
    {
        stEntry.mode = MCPD_SNOOP_EX_ADD;
    }
    else
    {
        stEntry.mode = MCPD_SNOOP_EX_CLEAR;
    }

    br_igmp_fill_wan_info(&stEntry);   
    mcpd_nl_process_igmp_snoop_entry_api(&stEntry);

    return 0;
}

static void br_igmpv3_parse(struct net_bridge *br, struct sk_buff *skb, 
        struct iphdr *pip, struct igmpv3_report *pstRepV3)
{
    int k;
    struct igmpv3_grec   *pstGrec;

    br_igmp_debug("process IGMPv3 packet\r\n");
    
   // pstRepV3 = (struct igmpv3_report *)(skb->data + (pip->ihl << 2));
    pstGrec = &pstRepV3->grec[0];
    // 遍历所有的组记录
    /*Start of Protocol :igmp无法播放问题 */
    for (k = 0; k < ntohs(pstRepV3->ngrec); k++)
    /*End of Protocol :igmp无法播放问题 */
    {
#ifdef CONFIG_IGMP_SSM
		if(br_igmpv3_ssm_validate_grec(pstGrec))
        {
        	pstGrec = (struct igmpv3_grec *)((char *)pstGrec + sizeof(struct igmpv3_grec) + ntohs(pstGrec->grec_nsrcs) * sizeof(struct in_addr)); 
            continue;
        }
#endif
        // IS_EXCLUDE/CHANGE_TO_EXCLUDE模式都是表示新增组的
        // 如果当前为IS_INCLUDE并且对应的INCLUDE不为NULL那么表示只是需要新增，如果为NULL那么可能就表示LEAVE消息了
        // 如果是CHANGE_TO_INCLUDE那么如果源不为NULL也表示是新增
        // 总之这几种模式都表示是需要新增，只有当INCLUDE并且源为NULL那么才是leave消息
        // 所以实际上，我们这里并没有做非常精确的判断，比如到底include哪个地址，EXCLUDE哪个地址等而是一股脑的全部允许
        // 实际上，某个PC可能只是需要INCLUDE某一个源地址的，其它的非该源地址的该PC也许不需要处理
        if ((IGMPV3_MODE_IS_EXCLUDE == pstGrec->grec_type) || 
            (IGMPV3_CHANGE_TO_EXCLUDE == pstGrec->grec_type) || 
            (pstGrec->grec_nsrcs != 0 && IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type) || 
            (pstGrec->grec_nsrcs != 0 && IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type) ||
            (IGMPV3_ALLOW_NEW_SOURCES == pstGrec->grec_type)) 
        {
            br_igmp_debug("add new IGMPv3 entry\r\n");
            br_igmpv3_rrd_process(br, skb, pip, pstGrec, MC_FDB_ADD_OP);
        }
        else if ((IGMPV3_CHANGE_TO_INCLUDE == pstGrec->grec_type && pstGrec->grec_nsrcs == 0) || 
                 (pstGrec->grec_nsrcs == 0 && IGMPV3_MODE_IS_INCLUDE == pstGrec->grec_type) || 
                 (IGMPV3_BLOCK_OLD_SOURCES == pstGrec->grec_type))  //前面两种表示LEAVE消息，后面一种也可能表示LEAVE消息
        {
            br_igmp_debug("delete IGMPv3 entry\r\n");
            br_igmpv3_rrd_process(br, skb, pip, pstGrec, MC_FDB_DEL_OP);
        }
        
        /*Start of Protocol :igmp无法播放问题 */
        pstGrec = (struct igmpv3_grec *)((char *)pstGrec + sizeof(struct igmpv3_grec) + ntohs(pstGrec->grec_nsrcs) * sizeof(struct in_addr)); 
        /*End of Protocol :igmp无法播放问题 */
    }
    return ;
}


void br_igmp_process(struct net_bridge *br, struct sk_buff *skb)
{
    struct igmpv3_report *pstRepV3 = NULL;
    struct iphdr *pip = NULL;
    
    pip = ip_hdr(skb);
    pstRepV3 = (struct igmpv3_report *)(skb->data + (pip->ihl << 2));

    br_igmp_debug("begin to process igmp packet skb len:%d\r\n",skb->data_len);

    switch(pstRepV3->type)
    {
        case IGMPV3_HOST_MEMBERSHIP_REPORT:
            br_igmpv3_parse(br, skb, pip, pstRepV3);
            break;
        case IGMP_HOST_LEAVE_MESSAGE:
            br_igmpv12_process(br, skb, pip, (struct igmphdr *)pstRepV3, MC_FDB_DEL_OP);
            break;
        case IGMP_HOST_MEMBERSHIP_REPORT:
        case IGMPV2_HOST_MEMBERSHIP_REPORT:
            br_igmpv12_process(br, skb, pip, (struct igmphdr *)pstRepV3, MC_FDB_ADD_OP);
            break;
        default:
            break;
    }

    return ;
}



