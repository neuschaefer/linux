/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_kernel_qos_hook.h
  版 本 号   : 初稿
  生成日期   : D2012_12_10

******************************************************************************/
#ifndef __HI_KERNEL_QOS_HOOK_H__
#define __HI_KERNEL_QOS_HOOK_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum
{
    HI_KERNEL_QOS_ACCEL_FLAG_NO_E = 0,  
    HI_KERNEL_QOS_ACCEL_FLAG_SW_E = 1,
    HI_KERNEL_QOS_ACCEL_FLAG_HW_E = 2
}hi_kernel_qos_accel_flag_e; 


#define HI_KERNEL_SKB_MARK_VID_MASK             0xFFF          //vid MASK
#define HI_KERNEL_SKB_MARK_PRI_MASK             13             //pri MASK

typedef enum
{
    HI_KERNEL_MARK_HOOK_ACCEL_FLAG_E = 0,
    HI_KERNEL_MARK_HOOK_VLAN_E       = 1,
    HI_KERNEL_MARK_HOOK_QUEUE_ID_E   = 2,
    HI_KERNEL_MARK_HOOK_CAR_ID_E     = 3,
    HI_KERNEL_MARK_HOOK_QOS_TX_E     = 4,
    HI_KERNEL_MARK_HOOK_REAL_DEV_E   = 5,
    HI_KERNEL_MARK_HOOK_GRE_E        = 6,
    HI_KERNEL_MARK_HOOK_MARK_SET_E   = 7,
    HI_KERNEL_MARK_HOOK_MARK_GET_E   = 8,
    HI_KERNEL_MARK_HOOK_ACCTX_E      = 9,
    HI_KERNEL_MARK_HOOK_DSCP_E =10 
}hi_kernel_mark_hook_type_e; 

typedef unsigned int (*hi_kernel_mark_common_hook)(struct sk_buff *pst_skb); 
typedef unsigned int (*hi_kernel_mark_vlan_hook)(struct sk_buff *pst_skb, unsigned short *pus_vlan);
typedef unsigned int (*hi_kernel_mark_car_hook)(struct sk_buff *pst_skb, unsigned int *pui_car_id);
typedef unsigned int (*hi_kernel_mark_qos_txhook)(struct sk_buff *pst_skb, struct net_device *pst_dev ); 
#ifdef CONFIG_BUILD_SD5610T
extern unsigned int hi_srv_mark_hook_reg(hi_kernel_mark_hook_type_e em_type, void *pv_hook);
extern unsigned int hi_phy_mng_reg( void *pv_callback );
#else
extern unsigned int hi_kernel_srv_mark_hook_reg(hi_kernel_mark_hook_type_e em_type, void *pv_hook);
extern unsigned int hi_kernel_phy_mng_reg( void *pv_callback );
#endif
extern void eth_link_change_hooker(const char *pszChangeEthName,int portNum,int result);

extern unsigned int hi_kernel_srv_mark_acc_is_enable( struct sk_buff *pst_skb );
extern unsigned int hi_kernel_srv_mark_get_vlan(struct sk_buff *pst_skb, unsigned short *pus_vlan);
extern unsigned int hi_kernel_srv_mark_get_qid(struct sk_buff *pst_skb, unsigned int *pui_qid);
extern unsigned int hi_kernel_srv_mark_get_carid(struct sk_buff *pst_skb, unsigned int *pui_car_id);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_KERNEL_QOS_HOOK_H__ */
