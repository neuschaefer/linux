/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 华为技术有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_eth0_qos.h
  版 本 号   : 初稿
  生成日期   : D2012_02_27

******************************************************************************/
#ifndef __HI_KERNEL_SOFT_QOS_H__
#define __HI_KERNEL_SOFT_QOS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


/***************************************************************************/
/***************************************************************************/

/* 是否是没有数据的ACK报文 */
static int is_ack(struct sk_buff* skb);

static void set_highest_priority( struct sk_buff *skb);

/***************************************************************************/

/* skb->mark */
/*----------|----------|--------------|----|-----|-----|------|-----------*/
/* policy route|policy  route|default qos mark|wmm|policer|queue|802.1p|downlinkQos */

#define SKB_MARK_8021P_REMARK_ENABLE    0x00000010          //是否需要进行802.1p Remark
#define SKB_MARK_8021P_REMARK_MASK      0x000000E0          //802.1p Remark值
#define SKB_MARK_8021P_REMARK_OFFSET    5                   //802.1p Remark值的offset

#define SKB_MARK_QUEUE_MASK             0x00000F00          //queue id MASK
#define SKB_MARK_QUEUE_OFFSET           8                   //queue id值的offset

#define SKB_MARK_POLICER_MASK           0x0000F000          //policer id MASK
#define SKB_MARK_POLICER_OFFSET         12                  //policer id值的offset


/* skb->accelmark */
/*----------|----------|--------------|----|-----|-----|------|--------------*/
/*                                          reserved                                                 |acceleration tyep */

#define SKB_ACCEL_MARK_ENABLE           0x00000001          //是否需要关注该标志
#define SKB_ACCEL_MARK_MASK             0x00000006          //加速标志的MASK    
#define SKB_ACCEL_MARK_OFFSET           1                   //加速标志的OFFSET
#define SKB_ACCEL_MARK_NONE             0x00000000          //不需要加速
#define SKB_ACCEL_MARK_SW               0x00000002          //软加速
#define SKB_ACCEL_MARK_HW               0x00000004          //硬加速
#define SKB_ACCEL_MARK_CFI_ENABLE       0x00000008          //是否为控制报文

#define SKB_ACCEL_MARK_CFI_BIT          0x1000              //CFI BIT
#define SKB_ACCEL_MARK_PRIORITY_MASK    0xE000              //TCI中PRIORITY的MASK
#define SKB_ACCEL_MARK_PRIORITY_OFFSET  13                  //TCI中PRIORITY的偏移


#define SKB_ACCEL_GRE_MARK		0x40000000		// 需要GRE打上标记

/***************************************************************************/

#define SKB_UPLINK_BASE_VLAN               4000                  //用来区分上行car id添加的基vlan
#define SKB_DOWNLINK_BASE_VLAN         4017                  //用来区分下行car id添加的基vlan

/* 注： skb->accelmark 第0-2位标识加速状态、 第3位标识是否做dscp mark */
/* skb->accelmark */
/* reserved | dscp mark(1 bit) |acceleration type(3 bit) */
#define SKB_MARK_DSCP_REMARK_ENABLE    0x8          //是否需要进行dscp Remark

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_SOFT_QOS_H__ */
