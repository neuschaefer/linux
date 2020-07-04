/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

  Copyright (C), 2001-2013, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_kernel_soft_qos.c
  Version       : Initial Draft
  Created       : 2013-01-23
  Last Modified :
  Description   : Hook function for QoS support Hisilicon software acceleration
  Function List :
  History       :
  1.Date        : 2013-01-23
    Modification: Created file

******************************************************************************/
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/imq.h>
#include <linux/netfilter.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter_ipv4.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/tcp.h>
#include <linux/kernel.h>
#include <asm/byteorder.h>
#include <linux/if_vlan.h>
#include <net/netfilter/nf_conntrack.h>
#include "hi_kernel_soft_qos.h"
#include "hi_kernel_qos_hook.h"
#include "atpconfig.h"



#define IS_HISI_WAN(name)  (('p' == name[0]) || ('n' == name[0]) || ('0' == name[0]) || ('i' == name[0] && 'p' == name[1]))


unsigned int hi_kernel_imq_mark_set(struct sk_buff *skb, unsigned int * mark)
{
   if((skb == NULL)
        ||(mark == NULL))
    {
        printk("the skb/mark is null.\n");
        return 1;
    }

    //mark 赋值
    skb->mark = *mark;
    skb->accelmark = *(mark+1);
    
    return 0;
}

unsigned int hi_kernel_imq_mark_get(struct sk_buff *skb, unsigned int * mark)
{
    struct nf_conn *ct=NULL;
    struct net_device *dev=NULL;
    unsigned int ulwmmQueue = 0;

    if((skb == NULL)
        ||(mark == NULL))
    {
        printk("the skb/mark is null.\n");
        return 1;
    }

    dev=skb->dev;
    ct = (struct nf_conn *)skb->nfct;

    if (ct&&(ct->mark)&&dev)
    {
        if(0==strncmp(dev->name,"br",2))
        {
            ulwmmQueue = QOS_WMM_QUEUE_NUM -((ct->mark &0xf00)>>8);
            if(QOS_WMM_QUEUE_NUM == ulwmmQueue)
            {   
                //没有匹配上任何规则时必须为0
                ulwmmQueue = 0;
            }
            skb->mark |=  (ct->mark&QOS_POLICER_MASK)|(ulwmmQueue<<16); 

            //printk("the downlink ct->mark is %0x/%0x.\n",ct->mark,skb->mark);
        }       
    }

    //printk("the ct->mark is %0x/%s/%0x.\n",skb->mark,dev->name,skb->accelmark);

    //mark学习
    *mark = skb->mark;
    *(mark+1) = skb->accelmark;

    return 0;
}

#ifdef  CONFIG_DOWN_RATE_CONTROL
unsigned int hi_kernel_imq_pro(struct sk_buff *skb, struct net_device *dev)
{
    
    int ret = 0;
    unsigned int ulLanLanPktFlag = 0;
    unsigned int ulDownQosFlag = 0;
    
    if((skb == NULL)
        ||(dev == NULL))
    {
        printk("the packet is null.\n");
        
        return 1;
    }

    //判断是否为lan侧发起
    ulLanLanPktFlag = !(skb->mark &0x200000);

    //printk("the mark/dev is %0x/%s/%d.\n",skb->mark,dev->name,ulLanLanPktFlag);

    ulDownQosFlag = downqos_enable && ulLanLanPktFlag&& IS_ETN_LAN(dev->name);
    
    if (ulDownQosFlag)
    { 
        //packet from non-br to imq
        if ((skb->mark & QOS_DOWNDEFAULT_MARK) != QOS_DOWNDEFAULT_MARK)
        {
            skb->imq_flags &= ~IMQ_F_ENQUEUE;
            skb->mark |= QOS_DOWNDEFAULT_MARK;
            skb->imq_flags |= QOS_DEV_IMQ4;

            skb->iif = dev->ifindex;

            ret = imq_nf_queue(skb);

            //进入imq正常处理
            if (1 != ret)
            {
                return 0;
            }       
        }
    }
    
    //上下行发包
    dev->netdev_ops->ndo_start_xmit(skb, dev);

    return 0;
}

#endif


/*****************************************************************************
 Prototype    : set_accel
 Description  : Set acceleration flag.
 Input        : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
              : int accel_flag           : Acceleration flag.
 Output       : none.
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-05-03
    Modification : Created function
*****************************************************************************/
static void set_accel( struct sk_buff *skb, int accel_flag)
{
    // 设置加速标记位
    skb->accelmark &= ~SKB_ACCEL_MARK_ENABLE;
    skb->accelmark |= SKB_ACCEL_MARK_ENABLE;    

    // 设置实际加速模式
    skb->accelmark &= ~SKB_ACCEL_MARK_MASK;
    accel_flag &= SKB_ACCEL_MARK_MASK;
    skb->accelmark |= accel_flag;
}

/*****************************************************************************
 Prototype    : hi_kernel_qos_handler
 Description  : Hook function to support ip qos in hisi driver.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
                     struct net_device *dev : the device send the skb to.
 Output        : None
 Return Value : 0       : let the Hisi eth driver send the packet; 
                      others: drop the packet.
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-01-23
    Modification : Created function
*****************************************************************************/
unsigned int hi_kernel_qos_handler(struct sk_buff *skb, struct net_device *dev)
{
    int ret = 0;

    if (is_ack(skb))
    {
        set_highest_priority(skb);
    }

    return 0; 
}

/*****************************************************************************
 Prototype    : hi_kernel_qos_acceltype_handler
 Description  : Hook function to get acceleration type.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output        : None
 Return Value : HI_KERNEL_QOS_ACCEL_FLAG_NO_E       : need no acceleration; 
                      HI_KERNEL_QOS_ACCEL_FLAG_SW_E       : software acceleration
                      HI_KERNEL_QOS_ACCEL_FLAG_HW_E       : hardware acceleration
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-02-19
    Modification : Created function
*****************************************************************************/

unsigned int hi_kernel_qos_acceltype_handler(struct sk_buff *skb)
{
    
#ifdef  CONFIG_DOWN_RATE_CONTROL
    struct nf_conn *ct=NULL;
    struct net_device *dev=NULL;

    if(skb == NULL)
    {
        printk("the skb/mark is null.\n");
        return 1;
    }

    dev=skb->dev;
    ct = (struct nf_conn *)skb->nfct;
    
    if (downqos_enable&&ct&&dev&&(0==strncmp(dev->name,"br",2)))
    {   
        //printk("the mark is %0x/%0x/%s.\n",ct->mark,skb->mark,dev->name);
        
        if((!ct->mark)
#ifdef CONFIG_DPI_PARSE
            //如果DPI将该报文已经设别则可以进加速
            &&(!ct->bIsContinue)
#endif
            )
        {
            //dmz第二个报文不进加速
            set_accel(skb, SKB_ACCEL_MARK_NONE);
        }
#ifdef CONFIG_DPI_PARSE
        else if(!smartqos_enable)
#else
        else
#endif
        {
            //dmz第三个报文才进加速
            set_accel(skb, SKB_ACCEL_MARK_SW);
        }
    }

#ifndef CONFIG_DPI_PARSE
    /* 下行qos时，dmz 匹配不上qos规则的tcp 报文上行不能进入加速,否则导致下行连接不上；
    dpi有ack 不校验保护机制，取消此处能保证上下行加速 */
    if(downqos_enable&&ct&&(!ct->mark)&&dev&&IS_HISI_WAN(dev->name))
    {
        set_accel(skb, SKB_ACCEL_MARK_NONE);
    }
#endif

#endif


    if(skb->accelmark & SKB_ACCEL_MARK_ENABLE)
    {
        return ((skb->accelmark & SKB_ACCEL_MARK_MASK) >> SKB_ACCEL_MARK_OFFSET);
    }
    else
    {
        return HI_KERNEL_QOS_ACCEL_FLAG_HW_E;
    }
}

/*****************************************************************************
 Prototype    : hi_kernel_qos_status_handler
 Description  : Hook function to get QoS status.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output        : None
 Return Value : 0       :  QoS disable; 
                      1       :  QoS enable;
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-02-19
    Modification : Created function
*****************************************************************************/

unsigned int hi_kernel_qos_status_handler(struct sk_buff *skb)
{

    return 0;

}

/*****************************************************************************
 Prototype    : hi_kernel_qos_queue_handler
 Description  : Hook function to get QoS queue id.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output        : None
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-02-19
    Modification : Created function
*****************************************************************************/
unsigned int hi_kernel_qos_queue_handler(struct sk_buff *skb)
{
#ifdef CONFIG_DOWN_RATE_CONTROL
    //上行tcp ack报文走最高优先级,此处为内核/加速必经之地
    if (is_ack(skb))
    {
        set_highest_priority(skb);
    }
#endif

    unsigned int ui_queue_id=0;
    ui_queue_id = (skb->mark & SKB_MARK_QUEUE_MASK) >> SKB_MARK_QUEUE_OFFSET;
    ui_queue_id -= 1;
    if( ui_queue_id > 7 )
    {
#ifdef CONFIG_DPI_PARSE
        //没有匹配smartqos规则的流优先级要高于P2P
        ui_queue_id = 4;
#else
        ui_queue_id = 7;
#endif
    }

    return ui_queue_id;
}

/*****************************************************************************
 Prototype    : hi_kernel_qos_car_handler
 Description  : Hook function to get QoS car id.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output        : None
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-02-19
    Modification : Created function
*****************************************************************************/
unsigned int hi_kernel_qos_car_handler(struct sk_buff *skb, unsigned int *pui_vlan_id, struct net_device *dev)
{
    unsigned int car_id=0;

    if (NULL==dev)
    {
        *pui_vlan_id = SKB_UPLINK_BASE_VLAN;  //car id 为0，不限速
        return 0;
    }

    if (skb->mark & SKB_MARK_POLICER_MASK)
    {
        car_id= (skb->mark & SKB_MARK_POLICER_MASK) >> SKB_MARK_POLICER_OFFSET;
    }
    
    
    if (IS_HISI_WAN(dev->name))
    {
#if defined(SUPPORT_ATP_TELMEX) && defined(SUPPORT_ATP_VOICE)
        if (dev->name[0] == 'n' 
            && dev->name[4] != 'p'
            &&((skb->mark & 0xf) == NFMARK_SIP
            || (skb->mark & 0xf) == NFMARK_RTP
            || (skb->mark & 0xf) == NFMARK_RTCP))
        {
            *pui_vlan_id = 5 << SKB_ACCEL_MARK_PRIORITY_OFFSET;
        }
        else
#endif
        {
            *pui_vlan_id=SKB_UPLINK_BASE_VLAN+car_id;
        }
    }
    else    
#if 0 //def  CONFIG_DOWN_RATE_CONTROL
    {
        if(!(skb->mark&0x200000))
        {
            *pui_vlan_id=SKB_DOWNLINK_BASE_VLAN+car_id;
        }
        else
        {
            *pui_vlan_id=SKB_DOWNLINK_BASE_VLAN;
        }
    }
#else
    {
        //lan->lan的报文，优先级mark 打上8，即进队列7
        if(skb->mark&0x200000)
        {
            skb->mark = (skb->mark & (~0x00000f00)) ;
            skb->mark = skb->mark |0x00000800;
        }
        //wan->lan的报文，非最高优先级报文，优先级mark 打上7，即进队列6
        else
        {
            if(0x100!=(skb->mark & 0x00000f00))
            {
                skb->mark = (skb->mark & (~0x00000f00)) ;
                skb->mark = skb->mark |0x00000700; 
            }
        }
        
        *pui_vlan_id = SKB_DOWNLINK_BASE_VLAN;  //不支持下行限速，不限速
    }
#endif


    return 0;
}

/*****************************************************************************
 Prototype    : hi_kernel_gre_ind_handler
 Description  : Hook function to get gre ind.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output        : None
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-02-19
    Modification : Created function
*****************************************************************************/
unsigned int hi_kernel_gre_ind_handler(struct sk_buff *skb)
{
    if (skb->accelmark & SKB_ACCEL_GRE_MARK)
    {
        return 1;
    }

    return 0;
}

/*****************************************************************************
 Prototype    : hi_kernel_qos_vlan_handler
 Description  : Hook function to set TCI.
 Input          : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
                  : unsigned short *vlan    : the current TCI.
 Output        : unsigned short *vlan   : the TCI after 802.1p Remark and CFI bit set.
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-02-19
    Modification : Created function
*****************************************************************************/
unsigned int hi_kernel_qos_vlan_handler(struct sk_buff *skb, unsigned short *vlan)
{
    unsigned short uspriority = 0;
#if defined(SUPPORT_ATP_TELMEX) && defined(SUPPORT_ATP_VOICE)
    if ((skb->mark & 0xf) == NFMARK_SIP
        || (skb->mark & 0xf) == NFMARK_RTP
        || (skb->mark & 0xf) == NFMARK_RTCP)
    {
        *vlan = (*vlan & (~SKB_ACCEL_MARK_PRIORITY_MASK)) | (5 << SKB_ACCEL_MARK_PRIORITY_OFFSET);
    }
    else
#elif defined (CONFIG_VLAN0)	
	if ((0 == (*vlan & (~SKB_ACCEL_MARK_PRIORITY_MASK))) && ((skb->mark & 0xf) == NFMARK_SIP || (skb->mark & 0xf) == NFMARK_RTP || (skb->mark & 0xf) == NFMARK_RTCP))
	{
        uspriority = 5;
        *vlan = (*vlan & (~SKB_ACCEL_MARK_PRIORITY_MASK)) | (uspriority << SKB_ACCEL_MARK_PRIORITY_OFFSET);		
	}
	else
#endif
    //需要remark
    if (skb->mark & SKB_MARK_8021P_REMARK_ENABLE)
    {
        uspriority = (skb->mark & SKB_MARK_8021P_REMARK_MASK) >> SKB_MARK_8021P_REMARK_OFFSET;
        *vlan = (*vlan & (~SKB_ACCEL_MARK_PRIORITY_MASK)) | (uspriority << SKB_ACCEL_MARK_PRIORITY_OFFSET);
    }
	/*Start for   不需要remark 时，需要返回1 */
    else
    {
        return 1;
    }
	/*End for   不需要remark 时，需要返回1 */
    

    return 0;
}

/*****************************************************************************
 Prototype    : hi_kernel_qos_dscp_handler
 Description  : 通知hisi是否做dscp remark
 Input          : struct sk_buff *skb
 Output        : 
                    
 Return Value : 1 : 不做dscp remark
                    2 : 做dscp remark

  1.Date         : 2014-03-12
    Modification : Created function
*****************************************************************************/
unsigned int hi_kernel_qos_dscp_handler(struct sk_buff *skb)
{
    if(NULL == skb)
    {
        return 1;
    }
    
    //需要dscp remark
    if (skb->accelmark & SKB_MARK_DSCP_REMARK_ENABLE)
    {
        return 0;
    }

    
    return 1;

}


/*****************************************************************************
 Prototype    : set_highest_priority
 Description  : Set mark flag to highest priority.
 Input        : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output       : none.
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-05-03
    Modification : Created function
*****************************************************************************/
static void set_highest_priority( struct sk_buff *skb)
{
    skb->mark &= 0xfffff0ff;
    skb->mark |= QOS_HIGHEST_PRI;
}

/*****************************************************************************
 Prototype    : set_accel
 Description  : Set CFI flag.
 Input        : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
              : int enable_cfi           : CFI flag.
 Output       : none.
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-05-03
    Modification : Created function
*****************************************************************************/
static void set_cfi( struct sk_buff *skb, int enable_cfi)
{
    enable_cfi &= SKB_ACCEL_MARK_CFI_ENABLE;
    skb->accelmark |= enable_cfi;    
}


/*****************************************************************************
 Prototype    : is_ack
 Description  : Check package if is ack without data.
 Input        : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output       : none.
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-05-03
    Modification : Created function
*****************************************************************************/
#define PPPOE_HLEN   6
#define PPP_TYPE_IPV4   0x0021  /* IPv4 in PPP */

static int is_ack(struct sk_buff* skb)
{
	struct iphdr       *ip_h   = NULL;
    struct tcphdr      *tcp_h  = NULL;
    int tcp_len = 0;

    // QinQ
    if (__constant_htons(ETH_P_8021Q) == *((short*)(&(skb->data[12]))))
    {
        if (__constant_htons(ETH_P_IP) == *((short*)(&(skb->data[16])))) 
        {   
            ip_h = (struct iphdr *)(&(skb->data[18]));
        }
        else if ((__constant_htons(ETH_P_PPP_DISC) == *((short*)(&(skb->data[16])))) || (__constant_htons(ETH_P_PPP_SES) == *((short*)(&(skb->data[16])))))
        {
            if (__constant_htons(PPP_TYPE_IPV4) == *((short*)(&(skb->data[24]))))
            {
                ip_h = (struct iphdr *)(&(skb->data[26]));
            }
        }
    }
    // IP hdr
    else if (__constant_htons(ETH_P_IP) == *((short*)(&(skb->data[12]))))
    {
        ip_h = (struct iphdr *)(&(skb->data[14]));
    }
    // PPP hdr
    else if ((__constant_htons(ETH_P_PPP_DISC) == *((short*)(&(skb->data[12])))) || (__constant_htons(ETH_P_PPP_SES) == *((short*)(&(skb->data[12])))))
    {
        if (__constant_htons(PPP_TYPE_IPV4) == *((short*)(&(skb->data[20]))))        
        {
            ip_h = (struct iphdr *)(&(skb->data[22]));
        }
    }

    if (NULL != ip_h)
    {
        if (IPPROTO_TCP == ip_h->protocol)
        {
            tcp_h = (struct tcphdr *)(((char *)ip_h) + ip_h->ihl * 4);
    		tcp_len =  __constant_htons(ip_h->tot_len) - tcp_h->doff * 4 - ip_h->ihl * 4; 	


            //dpi处理时,所有的无payload的tcp报文都走最高优先级,避免拥塞时tcp连接不上
    		if ((0 == tcp_len)
#ifndef CONFIG_DPI_PARSE
                //dpi将所有的tcp无payload报文都走最高优先级
                && (tcp_h->ack)
#endif
                )
    		{
                return 1;
    		}
    	}
    }

    return 0;
        
}

/*****************************************************************************
 Prototype    : tcp_ack_hook
 Description  : TCP package use soft acceleration.
 Input        : struct sk_buff *skb      : the soket buff structure, represent the packet to send out.
 Output       : none.
 Return Value : 
 Calls           : 
 Called By     : 
 
  History        :
  1.Date         : 2013-05-03
    Modification : Created function
*****************************************************************************/
unsigned int tcp_ack_hook(unsigned int hooknum,
			     struct sk_buff *skb,
			     const struct net_device *in,
			     const struct net_device *out,
			     int (*okfn)(struct sk_buff *))
{
	struct iphdr *ip_h = NULL;

    ip_h = ip_hdr(skb);

    if (ip_h && (IPPROTO_TCP == ip_h->protocol))
    {
        set_accel(skb, SKB_ACCEL_MARK_SW);
	}

	return NF_ACCEPT;
}


static struct nf_hook_ops tcp_ack_ops[] __read_mostly = {
    {
    	.hook		= tcp_ack_hook,
    	.pf		    = PF_INET,
    	.hooknum	= NF_INET_POST_ROUTING,
    	.priority	= NF_IP_PRI_FIRST,
    },
    {
        .hook       = tcp_ack_hook,
        .owner      = THIS_MODULE,
        .pf         = PF_BRIDGE,
        .hooknum    = NF_BR_POST_ROUTING,
        .priority   = NF_BR_PRI_LAST,
    },
};

static void __exit hi_kernel_vif_qos_exit(void)
{
#ifdef CONFIG_BUILD_SD5610T
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QOS_TX_E, NULL);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCEL_FLAG_E, NULL);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QUEUE_ID_E, NULL);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_CAR_ID_E, NULL);
	hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_VLAN_E, NULL);
	hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_DSCP_E, NULL);

    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_SET_E, NULL);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_GET_E, NULL);
#ifdef  CONFIG_DOWN_RATE_CONTROL
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCTX_E, NULL);
#endif
#else
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QOS_TX_E, NULL);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCEL_FLAG_E, NULL);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QUEUE_ID_E, NULL);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_CAR_ID_E, NULL);
	hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_VLAN_E, NULL);
	hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_DSCP_E, NULL);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_GRE_E, NULL);        	

    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_SET_E, NULL);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_GET_E, NULL);
#ifdef  CONFIG_DOWN_RATE_CONTROL
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCTX_E, NULL);
#endif
#endif

    return;
}
static int __init hi_kernel_vif_qos_init(void)

{
    int ret = 0;
    //printk ("%s %d ===[sizeof(struct nf_conn)]===[%d]=======\n", __FUNCTION__, __LINE__,sizeof(struct nf_conn));
    printk("\r\n Ip QoS Hook module install successfully! \r\n");
#ifdef CONFIG_BUILD_SD5610T
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QOS_TX_E, (void*)hi_kernel_qos_handler);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCEL_FLAG_E, (void*)hi_kernel_qos_acceltype_handler);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QUEUE_ID_E, (void*)hi_kernel_qos_queue_handler);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_CAR_ID_E, (void*)hi_kernel_qos_car_handler);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_VLAN_E, (void*)hi_kernel_qos_vlan_handler);
    
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_DSCP_E, (void*)hi_kernel_qos_dscp_handler);
    

    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_SET_E, (void*)hi_kernel_imq_mark_set);
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_GET_E, (void*)hi_kernel_imq_mark_get);
#ifdef  CONFIG_DOWN_RATE_CONTROL
    hi_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCTX_E, (void*)hi_kernel_imq_pro);
#endif

	hi_phy_mng_reg((void*)eth_link_change_hooker);
#else
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QOS_TX_E, (void*)hi_kernel_qos_handler);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCEL_FLAG_E, (void*)hi_kernel_qos_acceltype_handler);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_QUEUE_ID_E, (void*)hi_kernel_qos_queue_handler);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_CAR_ID_E, (void*)hi_kernel_qos_car_handler);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_VLAN_E, (void*)hi_kernel_qos_vlan_handler);
    
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_DSCP_E, (void*)hi_kernel_qos_dscp_handler);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_GRE_E, (void*)hi_kernel_gre_ind_handler);    	
    

    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_SET_E, (void*)hi_kernel_imq_mark_set);
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_MARK_GET_E, (void*)hi_kernel_imq_mark_get);
#ifdef  CONFIG_DOWN_RATE_CONTROL
    hi_kernel_srv_mark_hook_reg(HI_KERNEL_MARK_HOOK_ACCTX_E, (void*)hi_kernel_imq_pro);
#endif

	hi_kernel_phy_mng_reg((void*)eth_link_change_hooker);
#endif    

    return 0;    
    
err:
    nf_unregister_hooks(tcp_ack_ops, ARRAY_SIZE(tcp_ack_ops));
}



MODULE_LICENSE("Huawei Tech. Co., Ltd.");


module_init(hi_kernel_vif_qos_init); 

module_exit(hi_kernel_vif_qos_exit);


