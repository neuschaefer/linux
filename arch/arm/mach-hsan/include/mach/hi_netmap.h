/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  ��Ȩ���� (C), 2009-2019, ��˼�뵼�����޹�˾

 ******************************************************************************
  �� �� ��   : hi_netmap.h
  �� �� ��   : ����
  ��    ��   : 11
  ��������   : D2012_06_14

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
    hi_uint32                ui_cnt_task_charge;  // �����ڴ����������ȴ���

    hi_uint32                ui_per_low;   // ���䷧ֵ,���ڸü������²������
    hi_uint32                ui_per_high;  // ���䷧ֵ,���ڸ�ֵ������ʱ������    

    hi_uint32                ui_cnt_empty;    // ���п��޷���ȡ�ڴ����
    hi_uint32                ui_cnt_alloc;    // �ɹ������ڴ����
    hi_uint32                ui_cnt_alloc_fail; // �����ڴ�ʧ�ܼ���

    hi_uint32                ui_cnt_idle;  // ��ǰ�����ڴ����

    hi_uint32                ui_cnt_overlen; // ���䳤�ȹ���
    hi_uint32                ui_cnt_overfree; // �����ڴ����
    hi_uint32                ui_cnt_skbfree;  // �޷������ڴ����
    hi_uint32                ui_cnt_recycle;  // �����ڴ����

    hi_uint32                ui_flag;         // netmapʹ�ܱ�־, 1 enable; 0 disable

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

