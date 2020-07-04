/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : atphooks.h
  版    本  : 1.0
  创建日期  : 2014-7-31
  描    述  : atp hooks头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2014-7-31
     修改内容   : 完成初稿

*********************************************************************************/


#ifndef __LINUX_ATPHOOKS_H
#define __LINUX_ATPHOOKS_H

#include <linux/list.h>

/* Responses from hook functions. */
#define ATP_CONTINUE 0
#define ATP_STOP 1

enum atp_hooks_num
{
    /*数据流程类hook*/
    ATP_DEV_RCV,					/*协议栈收包入口*/
    ATP_DEV_XMIT,					/*协议栈发包出口*/
    ATP_BR_LOCALIN_BF_CHG_DEV,      /*即将进入bridge local in，但还未修改dev*/
    ATP_CT_BEFOREINIT,              /*链接跟踪CT初始化前的hook点*/
    ATP_IP_BF_RCV,                  /*IP Receive收包入口*/
    ATP_DEV_PRE_XMIT,			    /*dev_queue_xmit入口*/
    ATP_AF_PKT_RCV,                 /*packet_rcv处理*/

    /*逻辑控制类hook*/
    ATP_DEV_MIRROR,					/*mirror ioctl 专用*/
    ATP_BR_INIT,                    /*bridge模块初始化*/
    ATP_BR_DEL_IF,                  /*bridge port 删除时*/
    ATP_BR_IOCTL_PORT_RELAY,        /*br ioctl for dhcp port relay*/
    ATP_BR_IOCTL_HOOK,              /*ATP 扩展的bridge ioctl命令*/
    ATP_CT_KILL_ALL,                /*连接跟踪kill_all中的hook，用于保留ct不被删除*/
    ATP_IPV6_NDISC_DEL_POLICYROUTE, /*ndisc用于delpolicyroute*/
    ATP_MAX_HOOKS
};

enum atp_hook_priorities
{
    ATP_PRI_FIRST = INT_MIN,
    ATP_PRI_OAM,
    ATP_PRI_DEFAULT,
    ATP_PRI_LAST = INT_MAX,
};

typedef int atp_hookfn(unsigned int hooknum,
                                void* arg1,
                                void* arg2,
                                void* arg3);

struct atp_hook_ops
{
    struct list_head list;

    atp_hookfn* hook;
    unsigned int hooknum;
    /* Hooks are ordered in ascending priority. */
    int priority;
};

#ifdef CONFIG_ATP_COMMON
/*ATP_HOOK init*/
void atphooks_init(void);

/* Functions to register/unregister hook points. */
int atp_register_hook(struct atp_hook_ops* reg);
void atp_unregister_hook(struct atp_hook_ops* reg);

/* Functions used in ATP_HOOK*/
int atp_hook_slow(unsigned int hook, void* arg1,
                  void* arg2, void* arg3);
int atp_hook_empty(unsigned int hook);
int atp_hook_once(unsigned int hook,
                  void* arg1,
                  void* arg2,
                  void* arg3);

#define ATP_HOOK(hook, arg1, arg2, arg3, ret)	       \
    ({								       \
        if (ATP_STOP == atp_hook_slow(hook, arg1, arg2, arg3))\
            return ret;						       \
    })

#define ATP_HOOK_VOID(hook, arg1, arg2, arg3)	       \
    ({								       \
        if (ATP_STOP == atp_hook_slow(hook, arg1, arg2, arg3))\
            return;						       \
    })

#define ATP_HOOK_WITH_RETURN(hook, arg1, arg2, arg3)	       \
    ({                                     \
        if (!atp_hook_empty(hook))    \
        {                                     \
            return atp_hook_once(hook, arg1, arg2, arg3);     \
        }                                          \
    })

#else /* !CONFIG_ATP_COMMON */
#define ATP_HOOK(hook, arg1, arg2, arg3, ret)
#define ATP_HOOK_VOID(hook, arg1, arg2, arg3)
#define ATP_HOOK_WITH_RETURN(hook, arg1, arg2, arg3)
#endif

#endif /*__LINUX_ATPHOOKS_H*/
