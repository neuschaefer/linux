/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : atphooks.c
  版    本  : 1.0
  创建日期  : 2014-7-31
  描    述  : atp hooks模块
  函数列表  :

  历史记录      :
   1.日    期   : 2014-7-31
     修改内容   : 完成初稿

*********************************************************************************/

#include <linux/kernel.h>
#include <linux/mutex.h>
#include <net/net_namespace.h>
#include <linux/version.h>
#include <linux/atphooks.h>
#include <linux/netdevice.h>
#include <linux/export.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
extern void synchronize_net(void);
#endif
struct list_head atp_hooks[ATP_MAX_HOOKS] __read_mostly;
EXPORT_SYMBOL(atp_hooks);

static DEFINE_MUTEX(atp_hook_mutex);

int atp_register_hook(struct atp_hook_ops* reg)
{
    struct atp_hook_ops* elem;
    int err;

    err = mutex_lock_interruptible(&atp_hook_mutex);
    if (err < 0)
    {
        return err;
    }

    list_for_each_entry(elem, &atp_hooks[reg->hooknum], list)
    {
        if (reg->priority < elem->priority)
        {
            break;
        }
    }
    list_add_rcu(&reg->list, elem->list.prev);
    mutex_unlock(&atp_hook_mutex);
    return 0;
}
EXPORT_SYMBOL(atp_register_hook);

void atp_unregister_hook(struct atp_hook_ops* reg)
{
    mutex_lock(&atp_hook_mutex);
    list_del_rcu(&reg->list);
    mutex_unlock(&atp_hook_mutex);

    synchronize_net();
}
EXPORT_SYMBOL(atp_unregister_hook);


unsigned int atp_iterate(struct list_head* head,
                         unsigned int hook,
                         void* arg1,
                         void* arg2,
                         void* arg3)
{
    unsigned int verdict = ATP_CONTINUE;
    struct atp_hook_ops* elem = NULL;

    list_for_each_entry_rcu(elem, head, list)
    {
        verdict = elem->hook(hook, arg1, arg2, arg3);
        if (ATP_CONTINUE != verdict)
        {
            return verdict;
        }
    }

    return ATP_CONTINUE;
}


int atp_hook_slow(unsigned int hook,
                  void* arg1,
                  void* arg2,
                  void* arg3)
{
    unsigned int verdict;

    rcu_read_lock();

    verdict = atp_iterate(&atp_hooks[hook], hook, arg1, arg2, arg3);

    rcu_read_unlock();

    return verdict;
}
EXPORT_SYMBOL(atp_hook_slow);

int atp_hook_empty(unsigned int hook)
{
    int empty = 0;

    rcu_read_lock();
    empty = list_empty(&atp_hooks[hook]);
    rcu_read_unlock();

    return empty;
}

int atp_hook_once(unsigned int hook,
                  void* arg1,
                  void* arg2,
                  void* arg3)
{
    unsigned int ret;
    struct atp_hook_ops* elem = NULL;

    rcu_read_lock();
    elem = list_entry(rcu_dereference((&atp_hooks[hook])->next), struct atp_hook_ops, list);
    ret = elem->hook(hook, arg1, arg2, arg3);

    rcu_read_unlock();

    return ret;
}


void __init atphooks_init(void)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(atp_hooks); i++)
    {
        INIT_LIST_HEAD(&atp_hooks[i]);
    }

    return;
}

