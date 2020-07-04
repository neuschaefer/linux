/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  版权所有  : 2007-2020，华为技术有限公司
  文 件 名  : nf_conntrack_pri.h
  版    本  : v1.0
  创建日期  : 2013-7-19
  描    述  : 连接跟踪优先级模块新增头文件
  函数列表  :

  历史记录      :
   1.日    期   : 2013-7-19
     修改内容   : 完成初稿

*********************************************************************************/


#ifndef _NF_CONNTRACK_PRI_H
#define _NF_CONNTRACK_PRI_H

struct nf_conntrack_proto_pri
{
	struct hlist_node hnode;	/* Internal use. */

	char *name;

	/* 根据tuple 判断协议类型*/
	int (*check_by_tuple)(struct net *net, const struct nf_conntrack_tuple *orig);

	/* 根据ct 判断协议类型*/
	int (*check_by_ct)(struct nf_conn *ct);
};

extern int nf_conntrack_pri_register(struct nf_conntrack_proto_pri *me);

extern int nf_conntrack_pri_unregister(struct nf_conntrack_proto_pri *me);

extern int nf_pri_is_alg_skb(struct sk_buff *skb);

extern void nf_pri_set_alg_mark(struct sk_buff *skb);

#endif /*_NF_CONNTRACK_PRI_H*/


