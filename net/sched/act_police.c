/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 * net/sched/police.c	Input police filter.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 * 		J Hadi Salim (action changes)
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/rtnetlink.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <net/act_api.h>
#include <net/netlink.h>

#define L2T(p, L)   qdisc_l2t((p)->tcfp_R_tab, L)
#define L2T_P(p, L) qdisc_l2t((p)->tcfp_P_tab, L)

#define POL_TAB_MASK     15
static struct tcf_common *tcf_police_ht[POL_TAB_MASK + 1];
static u32 police_idx_gen;
static DEFINE_RWLOCK(police_lock);

static struct tcf_hashinfo police_hash_info = {
	.htab	=	tcf_police_ht,
	.hmask	=	POL_TAB_MASK,
	.lock	=	&police_lock,
};

/* old policer structure from before tc actions */
struct tc_police_compat {
	u32			index;
	int			action;
	u32			limit;
	u32			burst;
	u32			mtu;
	struct tc_ratespec	rate;
	struct tc_ratespec	peakrate;
};

/* Each policer is serialized by its individual spinlock */

static int tcf_act_police_walker(struct sk_buff *skb, struct netlink_callback *cb,
			      int type, struct tc_action *a)
{
	struct tcf_common *p;
	int err = 0, index = -1, i = 0, s_i = 0, n_i = 0;
	struct nlattr *nest;

	read_lock_bh(&police_lock);

	s_i = cb->args[0];

	for (i = 0; i < (POL_TAB_MASK + 1); i++) {
		p = tcf_police_ht[tcf_hash(i, POL_TAB_MASK)];

		for (; p; p = p->tcfc_next) {
			index++;
			if (index < s_i)
				continue;
			a->priv = p;
			a->order = index;
			nest = nla_nest_start(skb, a->order);
			if (nest == NULL)
				goto nla_put_failure;
			if (type == RTM_DELACTION)
				err = tcf_action_dump_1(skb, a, 0, 1);
			else
				err = tcf_action_dump_1(skb, a, 0, 0);
			if (err < 0) {
				index--;
				nla_nest_cancel(skb, nest);
				goto done;
			}
			nla_nest_end(skb, nest);
			n_i++;
		}
	}
done:
	read_unlock_bh(&police_lock);
	if (n_i)
		cb->args[0] += n_i;
	return n_i;

nla_put_failure:
	nla_nest_cancel(skb, nest);
	goto done;
}

static void tcf_police_destroy(struct tcf_police *p)
{
	unsigned int h = tcf_hash(p->tcf_index, POL_TAB_MASK);
	struct tcf_common **p1p;

	for (p1p = &tcf_police_ht[h]; *p1p; p1p = &(*p1p)->tcfc_next) {
		if (*p1p == &p->common) {
			write_lock_bh(&police_lock);
			*p1p = p->tcf_next;
			write_unlock_bh(&police_lock);
			gen_kill_estimator(&p->tcf_bstats,
					   &p->tcf_rate_est);
			if (p->tcfp_R_tab)
				qdisc_put_rtab(p->tcfp_R_tab);
			if (p->tcfp_P_tab)
				qdisc_put_rtab(p->tcfp_P_tab);
			/*
			 * gen_estimator est_timer() might access p->tcf_lock
			 * or bstats, wait a RCU grace period before freeing p
			 */
			kfree_rcu(p, tcf_rcu);
			return;
		}
	}
	WARN_ON(1);
}

static const struct nla_policy police_policy[TCA_POLICE_MAX + 1] = {
	[TCA_POLICE_RATE]	= { .len = TC_RTAB_SIZE },
	[TCA_POLICE_PEAKRATE]	= { .len = TC_RTAB_SIZE },
	[TCA_POLICE_AVRATE]	= { .type = NLA_U32 },
	[TCA_POLICE_RESULT]	= { .type = NLA_U32 },
};

static int tcf_act_police_locate(struct nlattr *nla, struct nlattr *est,
				 struct tc_action *a, int ovr, int bind)
{
	unsigned int h;
	int ret = 0, err;
	struct nlattr *tb[TCA_POLICE_MAX + 1];
	struct tc_police *parm;
	struct tcf_police *police;
	struct qdisc_rate_table *R_tab = NULL, *P_tab = NULL;
	int size;

	if (nla == NULL)
		return -EINVAL;

	err = nla_parse_nested(tb, TCA_POLICE_MAX, nla, police_policy);
	if (err < 0)
		return err;

	if (tb[TCA_POLICE_TBF] == NULL)
		return -EINVAL;
	size = nla_len(tb[TCA_POLICE_TBF]);
	if (size != sizeof(*parm) && size != sizeof(struct tc_police_compat))
		return -EINVAL;
	parm = nla_data(tb[TCA_POLICE_TBF]);

	if (parm->index) {
		struct tcf_common *pc;

		pc = tcf_hash_lookup(parm->index, &police_hash_info);
		if (pc != NULL) {
			a->priv = pc;
			police = to_police(pc);
			if (bind) {
				police->tcf_bindcnt += 1;
				police->tcf_refcnt += 1;
			}
			if (ovr)
				goto override;
			return ret;
		}
	}

	police = kzalloc(sizeof(*police), GFP_KERNEL);
	if (police == NULL)
		return -ENOMEM;
	ret = ACT_P_CREATED;
	police->tcf_refcnt = 1;
	spin_lock_init(&police->tcf_lock);
	if (bind)
		police->tcf_bindcnt = 1;
override:
	if (parm->rate.rate) {
		err = -ENOMEM;
		R_tab = qdisc_get_rtab(&parm->rate, tb[TCA_POLICE_RATE]);
		if (R_tab == NULL)
			goto failure;

		if (parm->peakrate.rate) {
			P_tab = qdisc_get_rtab(&parm->peakrate,
					       tb[TCA_POLICE_PEAKRATE]);
			if (P_tab == NULL)
				goto failure;
		}
	}

	spin_lock_bh(&police->tcf_lock);
	if (est) {
		err = gen_replace_estimator(&police->tcf_bstats,
					    &police->tcf_rate_est,
					    &police->tcf_lock, est);
		if (err)
			goto failure_unlock;
	} else if (tb[TCA_POLICE_AVRATE] &&
		   (ret == ACT_P_CREATED ||
		    !gen_estimator_active(&police->tcf_bstats,
					  &police->tcf_rate_est))) {
		err = -EINVAL;
		goto failure_unlock;
	}

	/* No failure allowed after this point */
	if (R_tab != NULL) {
		qdisc_put_rtab(police->tcfp_R_tab);
		police->tcfp_R_tab = R_tab;
	}
	if (P_tab != NULL) {
		qdisc_put_rtab(police->tcfp_P_tab);
		police->tcfp_P_tab = P_tab;
	}

	if (tb[TCA_POLICE_RESULT])
		police->tcfp_result = nla_get_u32(tb[TCA_POLICE_RESULT]);
	police->tcfp_toks = police->tcfp_burst = parm->burst;
	police->tcfp_mtu = parm->mtu;
	if (police->tcfp_mtu == 0) {
		police->tcfp_mtu = ~0;
		if (police->tcfp_R_tab)
			police->tcfp_mtu = 255<<police->tcfp_R_tab->rate.cell_log;
	}
	if (police->tcfp_P_tab)
		police->tcfp_ptoks = L2T_P(police, police->tcfp_mtu);
	police->tcf_action = parm->action;

	if (tb[TCA_POLICE_AVRATE])
		police->tcfp_ewma_rate = nla_get_u32(tb[TCA_POLICE_AVRATE]);

	spin_unlock_bh(&police->tcf_lock);
	if (ret != ACT_P_CREATED)
		return ret;

	police->tcfp_t_c = psched_get_time();
	police->tcf_index = parm->index ? parm->index :
		tcf_hash_new_index(&police_idx_gen, &police_hash_info);
	h = tcf_hash(police->tcf_index, POL_TAB_MASK);
	write_lock_bh(&police_lock);
	police->tcf_next = tcf_police_ht[h];
	tcf_police_ht[h] = &police->common;
	write_unlock_bh(&police_lock);

	a->priv = police;
	return ret;

failure_unlock:
	spin_unlock_bh(&police->tcf_lock);
failure:
	if (P_tab)
		qdisc_put_rtab(P_tab);
	if (R_tab)
		qdisc_put_rtab(R_tab);
	if (ret == ACT_P_CREATED)
		kfree(police);
	return err;
}

static int tcf_act_police_cleanup(struct tc_action *a, int bind)
{
	struct tcf_police *p = a->priv;
	int ret = 0;

	if (p != NULL) {
		if (bind)
			p->tcf_bindcnt--;

		p->tcf_refcnt--;
		if (p->tcf_refcnt <= 0 && !p->tcf_bindcnt) {
			tcf_police_destroy(p);
			ret = 1;
		}
	}
	return ret;
}

static int tcf_act_police(struct sk_buff *skb, const struct tc_action *a,
			  struct tcf_result *res)
{
	struct tcf_police *police = a->priv;
	psched_time_t now;
	long toks;
	long ptoks = 0;

	spin_lock(&police->tcf_lock);

	bstats_update(&police->tcf_bstats, skb);

	if (police->tcfp_ewma_rate &&
	    police->tcf_rate_est.bps >= police->tcfp_ewma_rate) {
		police->tcf_qstats.overlimits++;
		if (police->tcf_action == TC_ACT_SHOT)
			police->tcf_qstats.drops++;
		spin_unlock(&police->tcf_lock);
		return police->tcf_action;
	}

#ifndef CONFIG_ATP_HYBRID
    /* 原修改在tc命令中加入了mtu参数且值为64KB,mtu会使得令牌桶表的精度变低,原因如下 */
    /* TBF使用的是空闲资源流控算法，该算法会将一个接口的发包能力转换为空闲资源，根据当前系统可用空闲资源和报文将要占用的空闲资源来决定报文的流向.
       tick_in_usec 为每微秒将会占用的空闲资源。一个长度为size报文在限制速率为rate的TBF中，理论上消耗的资源为 1000000*((double)size/rate)*tick_in_usec.
       但在令牌桶生效处，一个包对资源的消耗并不是直接通过 1000000*((double)size/rate)*tick_in_usec 计算的，而是使用令牌桶表来计算的。
       令牌桶表data[256]是一个包的长度与空闲资源消耗值的映射表上，该表仅有256个值。
       空闲资源流控算法会根据最大mtu，在长度在0-2^(log2(Max(mtu))+1)之间均匀的取 256 个长度，计算其对应的空闲资源消耗离散值，并将对应的值存入data[256]中，data[256]就可以将对应包长映射成空闲资源消耗值。
       这意味长度在data里步长为 per_datalen = (2^(log2(mtu)+1)/256)，在步长内的报文计算出的资源消耗值为同一个值，也即是说，该算法的精度由 per_datalen 决定。
       因此，Max(mtu)越大，则包长对应的资源消耗值精度就越低。
       data[i]内存的值为 (per_datalen * (i+1)的资源消耗值
       长度为tab_size的报文对应的 i = tab_size / per_datalen (向下取整)。
       而tab_size为实际的长度 size - 1，即 i = (size - 1) / per_datalen (向下取整)
       即真实长度为 (per_datalen * i)+1 到 (per_datalen * (i+1))-1 + 1 的报文的消耗值全部会取 data[i] 里的值，data[i] 为 (per_datalen * (i+1))的真实资源消耗值。

       若将mtu设置为 65536 ，取0到131072之间的256个长度均匀值的资源消耗值，
       则长度的步长为 per_datalen = (2^(log2(mtu)+1)/256) = 512.
       这意味着512长度及以下的值全部会映射到data[0]上，data[0]为512的真实资源消耗值。
       data[1]对应的长度为513-1024，data[1]为1024的真实资源消耗值。
       date[2]对应1025-1536以上，其值为1536的消耗值。
       这意味着整个令牌桶表里面仅有[0]、[1]、[2]在多数场景下有效，且存在失准。
       由上面可知，包长度为纯512、1024、1536的流量的overflow将最为准确。而513、1025的流量overflow将最不准。
       包长全为size大小的流量会在 (size/((size/512(向上取整)) * 512)) * rate 处overflow
       即长度为513的流量会在 513/1024 rate 处 overflow。
       长度为1025的流量会在 1025/1536 rate 处 overflow。
       长度越长，失准越小，向上越接近512的倍数，则越准确。

       若为缺省mtu。参与令牌桶计算mtu值为2047，则步长为8，精度为8字节。tcfp_mtu与令牌桶的mtu的值略有不同，缺省为2040。
       mtu的逻辑意义是最大mtu，故此处判断大于该mtu的报文不参与令牌桶计算，直接 overlimit。
       mtu的值除了会参与令牌桶表的计算外，还会参与峰值令牌桶的计算。
       由于Hybrid overflow功能未使用峰值令牌桶，mtu仅涉及到令牌桶表的计算。
       此处去掉该判断，会使得大于mtu的报文也参与进令牌桶算法的计算中来。
       在一般的情况下，报文的资源占用值直接由data[i]给出。
       若长度大于mtu，则由长度计算出来的i会大于255(注:i = (size - 1) / per_datalen)。
       这种情况下，资源占用值的算法为:
       data[255]*(i >> 8) + data[i & 0xFF];
       即资源消耗值依旧以步长向前递进，精准性并未改变，故去掉该判断从目前的分析来看，不会对overflow的行为造成影响。
    */
    /* 应该在此处进行修改 */
    /* 默认情况下，tcfp_mtu为2047，当skb的qdisc_pkt_len大于该值时，skb不会令牌桶计算，直接overlimit，在Hybrid中，无该判断的必要 */
	if (qdisc_pkt_len(skb) <= police->tcfp_mtu) {
#endif
		if (police->tcfp_R_tab == NULL) {
			spin_unlock(&police->tcf_lock);
			return police->tcfp_result;
		}

		now = psched_get_time();
		toks = psched_tdiff_bounded(now, police->tcfp_t_c,
					    police->tcfp_burst);
		if (police->tcfp_P_tab) {
			ptoks = toks + police->tcfp_ptoks;
			if (ptoks > (long)L2T_P(police, police->tcfp_mtu))
				ptoks = (long)L2T_P(police, police->tcfp_mtu);
			ptoks -= L2T_P(police, qdisc_pkt_len(skb));
		}
		toks += police->tcfp_toks;
		if (toks > (long)police->tcfp_burst)
			toks = police->tcfp_burst;
		toks -= L2T(police, qdisc_pkt_len(skb));
		if ((toks|ptoks) >= 0) {
			police->tcfp_t_c = now;
			police->tcfp_toks = toks;
			police->tcfp_ptoks = ptoks;
			spin_unlock(&police->tcf_lock);
			return police->tcfp_result;
		}
#ifndef CONFIG_ATP_HYBRID
	}
#endif

	police->tcf_qstats.overlimits++;
	if (police->tcf_action == TC_ACT_SHOT)
		police->tcf_qstats.drops++;
	spin_unlock(&police->tcf_lock);
	return police->tcf_action;
}

static int
tcf_act_police_dump(struct sk_buff *skb, struct tc_action *a, int bind, int ref)
{
	unsigned char *b = skb_tail_pointer(skb);
	struct tcf_police *police = a->priv;
	struct tc_police opt = {
		.index = police->tcf_index,
		.action = police->tcf_action,
		.mtu = police->tcfp_mtu,
		.burst = police->tcfp_burst,
		.refcnt = police->tcf_refcnt - ref,
		.bindcnt = police->tcf_bindcnt - bind,
	};

	if (police->tcfp_R_tab)
		opt.rate = police->tcfp_R_tab->rate;
	if (police->tcfp_P_tab)
		opt.peakrate = police->tcfp_P_tab->rate;
	NLA_PUT(skb, TCA_POLICE_TBF, sizeof(opt), &opt);
	if (police->tcfp_result)
		NLA_PUT_U32(skb, TCA_POLICE_RESULT, police->tcfp_result);
	if (police->tcfp_ewma_rate)
		NLA_PUT_U32(skb, TCA_POLICE_AVRATE, police->tcfp_ewma_rate);
	return skb->len;

nla_put_failure:
	nlmsg_trim(skb, b);
	return -1;
}

MODULE_AUTHOR("Alexey Kuznetsov");
MODULE_DESCRIPTION("Policing actions");
MODULE_LICENSE("GPL");

static struct tc_action_ops act_police_ops = {
	.kind		=	"police",
	.hinfo		=	&police_hash_info,
	.type		=	TCA_ID_POLICE,
	.capab		=	TCA_CAP_NONE,
	.owner		=	THIS_MODULE,
	.act		=	tcf_act_police,
	.dump		=	tcf_act_police_dump,
	.cleanup	=	tcf_act_police_cleanup,
	.lookup		=	tcf_hash_search,
	.init		=	tcf_act_police_locate,
	.walk		=	tcf_act_police_walker
};

static int __init
police_init_module(void)
{
	return tcf_register_action(&act_police_ops);
}

static void __exit
police_cleanup_module(void)
{
	tcf_unregister_action(&act_police_ops);
}

module_init(police_init_module);
module_exit(police_cleanup_module);
