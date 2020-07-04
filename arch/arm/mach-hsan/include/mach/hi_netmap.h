/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2009-2019, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_netmap.h
  版 本 号   : 初稿
  作    者   : 11
  生成日期   : D2012_06_14

******************************************************************************/

#ifndef __HI_NETMAP_H__
#define __HI_NETMAP_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_NETMAP_MAX_BLOCK            512
#define HI_NETMAP_MEM_LEN              (2004+256)

typedef struct {
    struct sk_buff *pst_skb;
    struct list_head    st_list_node;
}hi_netmap_node_s;

typedef struct {
    hi_uint32                ui_cnt_task_charge;  // 补充内存任务进入调度次数

    hi_uint32                ui_per_low;   // 分配阀值,低于该计数重新补充分配
    hi_uint32                ui_per_high;  // 分配阀值,高于改值可以暂时不补充    

    hi_uint32                ui_cnt_empty;    // 队列空无法获取内存次数
    hi_uint32                ui_cnt_alloc;    // 成功申请内存次数
    hi_uint32                ui_cnt_alloc_fail; // 申请内存失败计数

    hi_uint32                ui_cnt_idle;  // 当前空闲内存块数

    hi_uint32                ui_cnt_overlen; // 分配长度过大
    hi_uint32                ui_cnt_overfree; // 回收内存溢出
    hi_uint32                ui_cnt_skbfree;  // 无法回收内存次数
    hi_uint32                ui_cnt_recycle;  // 回收内存次数

    hi_uint32                ui_flag;         // netmap使能标志, 1 enable; 0 disable

    struct list_head         st_list_idle;
    
    struct tasklet_struct st_tasklet_idle;
}hi_netmap_mng_s;


extern hi_int32 hi_netmap_init( hi_void );
extern hi_uint32 hi_netmap_datadump( hi_void *pv_data, hi_uint32 ui_inlen, hi_uint32 *pui_outlen);
extern struct sk_buff *hi_netmap_alloc_skb(struct net_device *dev,
				   hi_uint32 ui_len, gfp_t gfp_mask);
extern hi_void hi_netmap_free_skb(struct sk_buff *pst_skb);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __HI_NETMAP_H__ */

