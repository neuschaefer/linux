/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : nf_conntrack_pri.c
  版    本  : v1.0
  创建日期  : 2013-7-19
  描    述  : 连接跟踪优先级处理模块
  函数列表  :
                find_lowest_ct
                nf_conntrack_del_used_by_pri
                nf_conntrack_pri_ct_need_alloc
                nf_conntrack_pri_ct_reserved
                nf_conntrack_pri_fini
                nf_conntrack_pri_init
                nf_conntrack_pri_register
                nf_conntrack_pri_unregister
                nf_pri_is_alg_skb
                nf_pri_set_alg_mark

  历史记录      :
   1.日    期   : 2013-7-19
     修改内容   : 完成初稿

*********************************************************************************/


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/ctype.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <linux/netfilter/nf_conntrack_pri.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Li Jian");
MODULE_DESCRIPTION("Conntrack priority");

extern int (*nf_ct_reserved_hook)(struct nf_conn *ct);
extern int (*nf_ct_high_pri_hook)(struct net *net, const struct nf_conntrack_tuple *orig);
extern int (*nf_ct_del_used_by_pri_hook)(struct net *net);
int nf_conntrack_pri_ct_reserved(struct nf_conn *ct);

static DEFINE_MUTEX(nf_ct_pri_mutex);
HLIST_HEAD(nf_ct_pri_list);


/*优先级从低到高为:0x000,0x900,0x800,0x700,0x600,0x500,0x400,0x300,0x200,0x100，由QOS管理*/
static struct nf_conn * find_lowest_ct(struct net *net)
{
	unsigned int hash = 0;
	struct nf_conntrack_tuple_hash *h = NULL;
	struct hlist_nulls_node *n = NULL;
	struct nf_conn *ct = NULL;
	struct nf_conn *find_ct = NULL;
	u_int32_t ct_qos_mark = 0;

	spin_lock_bh(&nf_conntrack_lock);
	for (hash = 0; hash < nf_conntrack_htable_size; hash++)
	{
		hlist_nulls_for_each_entry_rcu(h, n, &net->ct.hash[hash], hnnode)
		{
			ct = nf_ct_tuplehash_to_ctrack(h);
			if (NULL == ct)
			{
				continue;
			}

			/*被注册需要保留的ct不删除*/
			if (nf_conntrack_pri_ct_reserved(ct))
			{
				continue;
			}

			/*非确认的ct，不能保证ct mark正确，因此只删ASSURED CT*/
			if (!test_bit(IPS_ASSURED_BIT, &ct->status))
			{
				continue;
			}

			ct_qos_mark = ct->mark;// & QOS_PRI_MASK;

			/*默认优先级匹配成功率很高，先判断处理*/
			if (0)//QOS_DEFAULT_PRI == ct_qos_mark)
			{
				find_ct = ct;
				break;
			}

			/*高优先级以上级别不删除*/
			if (0)//QOS_HIGH_PRI >= ct_qos_mark)
			{
				continue;
			}

			if (NULL == find_ct)
			{
				find_ct = ct;
				continue;
			}

			/*当前ct优先级更低*/
			if (0)//ct_qos_mark > (find_ct->mark & QOS_PRI_MASK))
			{
				find_ct = ct;
			}

		}
	}

	spin_unlock_bh(&nf_conntrack_lock);

	return find_ct;
}


/*RTSP新建保障，返回1表示删除其他ct成功，允许新建*/
int nf_conntrack_del_used_by_pri(struct net *net)
{
	struct nf_conn *ct = NULL;

	/*删ct时，从最低优先级开始删除，最高次高(mark为0x100/0x200) 不进行删除*/
	ct = find_lowest_ct(net);
	if (NULL == ct)
	{
		return 0;
	}

	/*ct很可能正在使用，使用延迟删除的方式进行删除*/
	set_bit(IPS_DELETE_BIT, &ct->status);
	//nf_conntrack_clean = IPS_DELETE;

	return 1;
}


int nf_pri_is_alg_skb(struct sk_buff *skb)
{
	struct nf_conn *ct = NULL;

	if (NULL == skb)
	{
		return 0;
	}

	ct = (struct nf_conn *)skb->nfct;
	if (NULL == ct)
	{
		return 0;
	}

	if (ct->master)
	{
		return 1;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(nf_pri_is_alg_skb);


void nf_pri_set_alg_mark(struct sk_buff *skb)
{
	struct nf_conn *ct = NULL;

	if (NULL == skb)
	{
		return;
	}

	ct = (struct nf_conn *)skb->nfct;
	if (NULL == ct)
	{
		return;
	}

	/*从ct获取mark*/
	skb->mark = ct->mark;

	return;
}
EXPORT_SYMBOL_GPL(nf_pri_set_alg_mark);



/*检查ct是否被注册为优先保留ct*/
int nf_conntrack_pri_ct_reserved(struct nf_conn *ct)
{
	struct nf_conntrack_proto_pri *proto_pri = NULL;
	struct hlist_node *n = NULL;
	int ct_reserved = 0;

	if (NULL == ct) {
		return ct_reserved;
	}

	rcu_read_lock();

	hlist_for_each_entry_rcu(proto_pri, n, &nf_ct_pri_list, hnode) {
		if (proto_pri->check_by_ct(ct)) {
			ct_reserved = 1;
			break;
		}
	}

	rcu_read_unlock();

	return ct_reserved;
}


/*检查tuple是否属于需确保创建成功的高优先级连接*/
int nf_conntrack_pri_ct_need_alloc(struct net *net, const struct nf_conntrack_tuple *orig)
{
	struct nf_conntrack_proto_pri *proto_pri = NULL;
	struct hlist_node *n = NULL;
	int need_alloc = 0;

	if (NULL == orig) {
		return need_alloc;
	}

	rcu_read_lock();

	hlist_for_each_entry_rcu(proto_pri, n, &nf_ct_pri_list, hnode) {
		if (proto_pri->check_by_tuple(net, orig)) {
			need_alloc = 1;
			break;
		}
	}

	rcu_read_unlock();

	return need_alloc;
}


int nf_conntrack_pri_register(struct nf_conntrack_proto_pri *me)
{
	mutex_lock(&nf_ct_pri_mutex);
	hlist_add_head_rcu(&me->hnode, &nf_ct_pri_list);
	mutex_unlock(&nf_ct_pri_mutex);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_conntrack_pri_register);


int nf_conntrack_pri_unregister(struct nf_conntrack_proto_pri *me)
{
	mutex_lock(&nf_ct_pri_mutex);
	hlist_del_rcu(&me->hnode);
	mutex_unlock(&nf_ct_pri_mutex);

	return 0;
}
EXPORT_SYMBOL_GPL(nf_conntrack_pri_unregister);


static int __init nf_conntrack_pri_init(void)
{
	rcu_assign_pointer(nf_ct_reserved_hook, nf_conntrack_pri_ct_reserved);
	rcu_assign_pointer(nf_ct_high_pri_hook, nf_conntrack_pri_ct_need_alloc);
	rcu_assign_pointer(nf_ct_del_used_by_pri_hook, nf_conntrack_del_used_by_pri);
	return 0;
}

static void nf_conntrack_pri_fini(void)
{
	rcu_assign_pointer(nf_ct_reserved_hook, NULL);
	rcu_assign_pointer(nf_ct_high_pri_hook, NULL);
	rcu_assign_pointer(nf_ct_del_used_by_pri_hook, NULL);
	return;
}

module_init(nf_conntrack_pri_init);
module_exit(nf_conntrack_pri_fini);


