/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/slab.h>
#include "br_private.h"
#include "br_igmp_dev_list.h"
#include "atp_interface.h"

//#define BR_DEV_DEBUG
#ifdef BR_DEV_DEBUG
#define br_dev_debug(args...)   color_printk(__func__, __LINE__, _RET_IP_, ## args)
#else
#define br_dev_debug(args...)
#endif
int br_igmp_snooping_init_done=0;


// 所有WAN口的接口列表，包括路由的，桥接的都包括
struct multicast_snooping_device_list multicast_snooping_dev_list;

static void br_snooping_dump_dev_list(void);
static int br_multicast_event(struct notifier_block *unused, unsigned long event, void *ptr);
static struct multicast_snooping_device *__find_device_in_list(char *device_name);
static struct multicast_snooping_device *find_device_in_list(char *device_name);
static int __add_device_into_list(struct net_device *netdev);
static int add_device_into_list(struct net_device *netdev);
static struct multicast_snooping_device *__remove_device_from_list(struct net_device *netdev);
static int remove_device_from_list(struct net_device *netdev);


static struct notifier_block br_multicast_snooping = {
	.notifier_call = br_multicast_event
};
static int br_multicast_event(struct notifier_block *unused, unsigned long event, void *ptr)
{    
	struct net_device *dev = ptr;

    if (!ptr)
    {
        br_dev_debug("error: args NULL\r\n");
        return NOTIFY_DONE;
    }

    // 不考虑LAN侧接口
    if ((IS_LAN_DEV(dev->name)) 
        || (IS_BRIDGE_DEV(dev->name))
        || (('i' == dev->name[0]) && (('m' == dev->name[1]) || ('f' == dev->name[1])))
        || (('d' == dev->name[0]) && ('s' == dev->name[1]))
        || ('s' == dev->name[0])
        || ('d' == dev->name[0])
        || ('l' == dev->name[0]))
    {        
        br_dev_debug("skip add %s into dev list\r\n", dev->name);
        return 0;
    }

    br_dev_debug("add %s into dev list, event:%d\r\n", dev->name,event);

    switch(event)
    {
        case NETDEV_UP:
            add_device_into_list(dev);
            break;
        
        case NETDEV_UNREGISTER:
            remove_device_from_list(dev);
            break;
            
        default:
            break;
    }

    return NOTIFY_DONE;
}

static struct multicast_snooping_device *__find_device_in_list(char *device_name)
{
    struct multicast_snooping_device *p = NULL;

    br_dev_debug("__find_device_in_list\r\n");        

    if (!multicast_snooping_dev_list.count)
    {
        br_dev_debug("__find_device_in_list, count is zero\r\n");
        return NULL;
    }

    br_dev_debug("find dev: %s in the list\r\n", device_name);
    
    list_for_each_entry(p, &multicast_snooping_dev_list.list, list)
    {
        if (!strncmp(p->WanName, device_name, IFNAMSIZ))
        {
            br_dev_debug("__find_device_in_list, wanname:%s\r\n", p->WanName);
            return p;
        }
    }

    br_dev_debug("find dev: NULL in the list\r\n");

    return NULL;
}

static struct multicast_snooping_device *find_device_in_list(char *device_name)
{
    struct multicast_snooping_device *p = NULL;
    spin_lock(&multicast_snooping_dev_list.lock);
    p = __find_device_in_list(device_name);
    spin_unlock(&multicast_snooping_dev_list.lock);

    return p;
}

static int __add_device_into_list(struct net_device *netdev)
{
    struct multicast_snooping_device*p = NULL;
    struct net_device *from_dev= NULL;
    
    p = kzalloc(sizeof(struct multicast_snooping_device), GFP_ATOMIC);
    if (!p)
    {
        br_dev_debug("malloc for dev: %s error\r\n", netdev->name);
        return (-1);
    }
    strncpy(p->WanName, netdev->name, (IFNAMSIZ - 1));
    p->netdev = netdev;
#if 0    
    from_dev = dev_get_by_name(&init_net,p->WanName);

    if(!from_dev)
    {
        br_dev_debug("Can't get dev by name %s\r\n", p->WanName);
        return -1;
    }
    p->netdev = from_dev;
#endif    
    multicast_snooping_dev_list.count++;
    INIT_LIST_HEAD(&p->list);
    list_add_tail(&p->list, &multicast_snooping_dev_list.list);

    return 0;
}

static int add_device_into_list(struct net_device *netdev)
{
    int err = 0;
    br_dev_debug("add_device_into_list\r\n");        

#ifdef BR_DEV_DEBUG
    br_snooping_dump_dev_list();
#endif
    
    if (find_device_in_list(netdev->name))
    {
        if (netdev->name)
            br_dev_debug("device %s already in to list\r\n", netdev->name);
        return 0;
    }
    
    br_dev_debug("begin to add device %s into list\r\n", netdev->name);

    spin_lock(&multicast_snooping_dev_list.lock);
    err = __add_device_into_list(netdev);
    spin_unlock(&multicast_snooping_dev_list.lock);

#ifdef BR_DEV_DEBUG
        br_snooping_dump_dev_list();
#endif

    return err;
}

static struct multicast_snooping_device *__remove_device_from_list(struct net_device *netdev)
{
    struct multicast_snooping_device *pFind = NULL;
    list_for_each_entry(pFind, &multicast_snooping_dev_list.list, list)
    {
        br_dev_debug("WanName:%s devname:%s\r\n", pFind->WanName,netdev->name);
        if (!strncmp(netdev->name, pFind->WanName, IFNAMSIZ))
        {
            list_del(&pFind->list);
            return pFind;
        }
    }
    br_dev_debug("WanName:%s\r\n", pFind->WanName);
    return NULL;
}

static int remove_device_from_list(struct net_device *netdev)
{
    struct multicast_snooping_device *p = NULL;

#ifdef BR_DEV_DEBUG
    br_snooping_dump_dev_list();
#endif
    
    spin_lock(&multicast_snooping_dev_list.lock);
    p = __remove_device_from_list(netdev);
    spin_unlock(&multicast_snooping_dev_list.lock);

    if (p)
    {
        br_dev_debug("delete dev %s from the list\r\n", netdev->name);
        kfree(p);
    }

#ifdef BR_DEV_DEBUG
    br_snooping_dump_dev_list();
#endif
    
    return 0;
}

struct multicast_snooping_device_list *br_snooping_get_dev_header(void)
{
#ifdef BR_DEV_DEBUG
    br_snooping_dump_dev_list();
#endif
    return &multicast_snooping_dev_list;
}


static void br_snooping_dump_dev_list(void)
{
    struct multicast_snooping_device *pFind = NULL;
    br_dev_debug("br_snooping_dump_dev_list");    
    spin_lock(&multicast_snooping_dev_list.lock);
    list_for_each_entry(pFind, &multicast_snooping_dev_list.list, list)
    {
        if (pFind->WanName)
        {
            br_dev_debug("dev: %s", pFind->WanName);
        }
        else
        {
            br_dev_debug("WanName is NULL!");        
        }
    }    
    br_dev_debug("br_snooping_dump_dev_list");    
    spin_unlock(&multicast_snooping_dev_list.lock);
}

void br_snooping_dev_lock(void)
{
    spin_lock(&multicast_snooping_dev_list.lock);
}
void br_snooping_dev_unlock(void)
{
    spin_unlock(&multicast_snooping_dev_list.lock);
}

int br_snooping_init(void)
{
    int err = 0;
    if( br_igmp_snooping_init_done)
        return 0;
    br_dev_debug("==>br_snooping_init\r\n");    
    spin_lock_init(&multicast_snooping_dev_list.lock);
    INIT_LIST_HEAD(&multicast_snooping_dev_list.list);
    err = register_netdevice_notifier(&br_multicast_snooping);
    if (err)
    {
        br_dev_debug("error, register snooping notifier error\r\n");
        return -1;
    }
    br_igmp_snooping_init_done = 1;
    return 0;
}


