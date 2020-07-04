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
    /* ԭ�޸���tc�����м�����mtu������ֵΪ64KB,mtu��ʹ������Ͱ��ľ��ȱ��,ԭ������ */
    /* TBFʹ�õ��ǿ�����Դ�����㷨�����㷨�Ὣһ���ӿڵķ�������ת��Ϊ������Դ�����ݵ�ǰϵͳ���ÿ�����Դ�ͱ��Ľ�Ҫռ�õĿ�����Դ���������ĵ�����.
       tick_in_usec Ϊÿ΢�뽫��ռ�õĿ�����Դ��һ������Ϊsize��������������Ϊrate��TBF�У����������ĵ���ԴΪ 1000000*((double)size/rate)*tick_in_usec.
       ��������Ͱ��Ч����һ��������Դ�����Ĳ�����ֱ��ͨ�� 1000000*((double)size/rate)*tick_in_usec ����ģ�����ʹ������Ͱ��������ġ�
       ����Ͱ��data[256]��һ�����ĳ����������Դ����ֵ��ӳ����ϣ��ñ����256��ֵ��
       ������Դ�����㷨��������mtu���ڳ�����0-2^(log2(Max(mtu))+1)֮����ȵ�ȡ 256 �����ȣ��������Ӧ�Ŀ�����Դ������ɢֵ��������Ӧ��ֵ����data[256]�У�data[256]�Ϳ��Խ���Ӧ����ӳ��ɿ�����Դ����ֵ��
       ����ζ������data�ﲽ��Ϊ per_datalen = (2^(log2(mtu)+1)/256)���ڲ����ڵı��ļ��������Դ����ֵΪͬһ��ֵ��Ҳ����˵�����㷨�ľ����� per_datalen ������
       ��ˣ�Max(mtu)Խ���������Ӧ����Դ����ֵ���Ⱦ�Խ�͡�
       data[i]�ڴ��ֵΪ (per_datalen * (i+1)����Դ����ֵ
       ����Ϊtab_size�ı��Ķ�Ӧ�� i = tab_size / per_datalen (����ȡ��)��
       ��tab_sizeΪʵ�ʵĳ��� size - 1���� i = (size - 1) / per_datalen (����ȡ��)
       ����ʵ����Ϊ (per_datalen * i)+1 �� (per_datalen * (i+1))-1 + 1 �ı��ĵ�����ֵȫ����ȡ data[i] ���ֵ��data[i] Ϊ (per_datalen * (i+1))����ʵ��Դ����ֵ��

       ����mtu����Ϊ 65536 ��ȡ0��131072֮���256�����Ⱦ���ֵ����Դ����ֵ��
       �򳤶ȵĲ���Ϊ per_datalen = (2^(log2(mtu)+1)/256) = 512.
       ����ζ��512���ȼ����µ�ֵȫ����ӳ�䵽data[0]�ϣ�data[0]Ϊ512����ʵ��Դ����ֵ��
       data[1]��Ӧ�ĳ���Ϊ513-1024��data[1]Ϊ1024����ʵ��Դ����ֵ��
       date[2]��Ӧ1025-1536���ϣ���ֵΪ1536������ֵ��
       ����ζ����������Ͱ���������[0]��[1]��[2]�ڶ�����������Ч���Ҵ���ʧ׼��
       �������֪��������Ϊ��512��1024��1536��������overflow����Ϊ׼ȷ����513��1025������overflow���׼��
       ����ȫΪsize��С���������� (size/((size/512(����ȡ��)) * 512)) * rate ��overflow
       ������Ϊ513���������� 513/1024 rate �� overflow��
       ����Ϊ1025���������� 1025/1536 rate �� overflow��
       ����Խ����ʧ׼ԽС������Խ�ӽ�512�ı�������Խ׼ȷ��

       ��Ϊȱʡmtu����������Ͱ����mtuֵΪ2047���򲽳�Ϊ8������Ϊ8�ֽڡ�tcfp_mtu������Ͱ��mtu��ֵ���в�ͬ��ȱʡΪ2040��
       mtu���߼����������mtu���ʴ˴��жϴ��ڸ�mtu�ı��Ĳ���������Ͱ���㣬ֱ�� overlimit��
       mtu��ֵ���˻��������Ͱ��ļ����⣬��������ֵ����Ͱ�ļ��㡣
       ����Hybrid overflow����δʹ�÷�ֵ����Ͱ��mtu���漰������Ͱ��ļ��㡣
       �˴�ȥ�����жϣ���ʹ�ô���mtu�ı���Ҳ���������Ͱ�㷨�ļ���������
       ��һ�������£����ĵ���Դռ��ֱֵ����data[i]������
       �����ȴ���mtu�����ɳ��ȼ��������i�����255(ע:i = (size - 1) / per_datalen)��
       ��������£���Դռ��ֵ���㷨Ϊ:
       data[255]*(i >> 8) + data[i & 0xFF];
       ����Դ����ֵ�����Բ�����ǰ�ݽ�����׼�Բ�δ�ı䣬��ȥ�����жϴ�Ŀǰ�ķ��������������overflow����Ϊ���Ӱ�졣
    */
    /* Ӧ���ڴ˴������޸� */
    /* Ĭ������£�tcfp_mtuΪ2047����skb��qdisc_pkt_len���ڸ�ֵʱ��skb��������Ͱ���㣬ֱ��overlimit����Hybrid�У��޸��жϵı�Ҫ */
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
