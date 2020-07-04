/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef __BR_MLD_SNOOPING_H
#define __BR_MLD_SNOOPING_H

#include <linux/netdevice.h>
#include <linux/miscdevice.h>
#include <linux/if_bridge.h>

#define MLD_TIMER_CHECK_TIMEOUT 10
#define MLD_QUERY_TIMEOUT 150

#ifndef ETH_ALEN
#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#endif


#define MLD_PAKCET_LEN         (32)

#define MLD_HOP_LIMIT           (1)

#define BR_MLD_SNOOPING_DISABLE 0
#define BR_MLD_SNOOPING_ENABLE 1


#define IN6_IS_ADDR_MC_NODELOCAL(a) \
	((((__const uint8_t *) (a))[1] & 0xf) == 0x1)
	
/* start by Huawei for HOMEGW-11879: Data IPv6 Link Local Multicast:FF02::1:20  not sent to clients  2012-04-09 */
#if 0
#define IN6_IS_ADDR_MC_LINKLOCAL(a) \
	((((__const uint8_t *) (a))[1] & 0xf) == 0x2)
#else    /*exclude   Reserved Multicast Addresses /All Nodes Addresses /All Routers Addresses   */
#define IN6_IS_ADDR_MC_LINKLOCAL(a)  \
	( (((__const uint16_t *) (a))[0] == 0xff02 ) \
        &&  (((__const uint16_t *) (a))[1] == 0x0000 ) \
        &&  (((__const uint16_t *) (a))[2] == 0x0000 ) \
        &&  (((__const uint16_t *) (a))[3] == 0x0000 ) \
        &&  (((__const uint16_t *) (a))[4] == 0x0000 ) \
        &&  (((__const uint16_t *) (a))[5] == 0x0000 ) \
        &&  (((__const uint16_t *) (a))[6] == 0x0000 ) \
        &&  ((((__const uint16_t *) (a))[7] == 0x0000 )  \
        || (((__const uint16_t *) (a))[7] == 0x0001 )     \
        || (((__const uint16_t *) (a))[7] == 0x0002 )) )
#endif 
 /* end by Huawei for HOMEGW-11879: Data IPv6 Link Local Multicast:FF02::1:20  not sent to clients  2012-04-09 */
	


struct mld1_msg
{
    uint8_t ucType;
    uint8_t ucCode;
    uint16_t usCheckSum;
    uint16_t usMRD;                  /* Maximum Response Delay  */
    uint16_t usReserved;
    struct in6_addr stMAddr;
};




struct mld2_grecrd {
	__u8		grec_type;
	__u8		grec_auxwords;
	__be16		grec_nsrcs;
	struct in6_addr	grec_mca;
	struct in6_addr	grec_src[0];
};

struct mld2_reportpkt {
	__u8	type;
	__u8	resv1;
	__sum16	csum;
	__be16	resv2;
	__be16	ngrec;
	struct mld2_grecrd grec[0];
};

#define     MLD_V2_MART_MODE_IS_INCLUDE         (1)
#define     MLD_V2_MART_MODE_IS_EXCLUDE         (2)

#define MLD_V2_MART_CHANGE_TO_INCLUDE_MODE      (3)
#define MLD_V2_MART_CHANGE_TO_EXCLUDE_MODE      (4)

#define MLD_V2_MART_ALLOW_NEW_SOURCES           (5)
#define MLD_V2_MART_BLOCK_OLD_SOURCES           (6)


struct net_bridge_mld_snooping_entry
{
    struct list_head 		  list;
	struct net_bridge_port	  *dev_dst;             /*目的桥端口对应的地址，如eth0.**/
	unsigned char			  grp_mac[ETH_ALEN];  /*组播地址对应的MAC地址，如33:33:xx:xx:xx:xx*/
	unsigned char			  host_mac[ETH_ALEN];   /*PC机的MAC地址*/
	unsigned long			  time;                  /*定时器*/
};

struct net_bridge_mld_snooping
{
	struct list_head		mld_list;         /*链表头*/
	struct timer_list 		mld_timer;        /*定时器*/
	int		                mld_snooping_enable;         /*0:表示IGMP Snooping开启*/
	spinlock_t		        mld_lock;         /*链表锁*/
	int		 	            mld_start_timer; /*定时器是否开启的标志*/
};

extern  void  br_mld_snooping_clear(void);
extern  int   br_mld_snooping_forward(struct sk_buff *skb, struct net_bridge *br,
                                    const unsigned char *dest,int forward);
/*start of  修改wifi组播crash问题  at  20100929*/
extern  int br_mld_snooping_del_by_nbp(struct net_bridge_port *dev_port);
/*end of  修改wifi组播crash问题  at  20100929*/
extern  void  br_mld_snooping_set_enable(int enable);
extern  void  br_mld_snooping_show(void);
extern  void  br_mld_snooping_init(void);

#endif

