/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef __BR_IGMP_PARSE_H__
#define __BR_IGMP_PARSE_H__

#include <linux/skbuff.h>
#include "br_private.h"

extern int is_igmp_need_process(const struct sk_buff *skb);

void br_igmp_process(struct net_bridge *br, struct sk_buff *skb);

#endif
