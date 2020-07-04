/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef __BR_IGMP_DEV_LIST_H__
#define __BR_IGMP_DEV_LIST_H__

#include <linux/list.h>
#include <linux/spinlock_types.h>
#include <linux/if.h>
#include <linux/netdevice.h>
#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BLOG)
#include "br_mcast.h"
#endif


struct multicast_snooping_device
{
    unsigned char WanName[IFNAMSIZ];
    struct net_device *netdev;
    struct list_head list;
};

struct multicast_snooping_device_list
{
    int count;
    spinlock_t lock;
    struct list_head list;
};

struct multicast_snooping_device_list *br_snooping_get_dev_header(void);

void br_snooping_dev_lock(void);
void br_snooping_dev_unlock(void);
int br_snooping_init(void);


#endif
