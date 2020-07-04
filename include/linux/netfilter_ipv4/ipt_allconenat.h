/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef __IP_ALL_CONE_NAT_H__
#define __IP_ALL_CONE_NAT_H__

struct nf_nat_ipv4_multi_range_compat;

struct ip_cone_nat_data
{
    int cone_nat_type;
    struct nf_nat_ipv4_multi_range_compat data;
};

// CONE NAT 三种类型，full cone/addr restricted cone/port restricted cone
enum cone_nat_type
{
    FULL_CONE_TYPE = 0,
    ADDR_CONE_TYPE = 1,
    PORT_CONE_TYPE = 2,    
    CONE_NAT_TYPE_MAX,
};

#endif

