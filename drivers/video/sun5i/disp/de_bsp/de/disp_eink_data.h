#ifndef __DISP_EINK_DATA_H__
#define __DISP_EINK_DATA_H__

#include "disp_display_i.h"
#include "disp_eink_config.h"

typedef struct __EINK_INIT_WF_S
{
    __u16 total_frame;                  //帧数
    __u8 wf_data[COL_MAX];
}eink_init_wf_t;                        //初始化波形数据文件

typedef struct __EINK_DU_WF_S
{
    __u16 total_frame;                  //帧数
    __u8 wf_data[DU_ROW_MAX][COL_MAX];
}eink_du_wf_t;                          //DU 模式下波形数据文件

typedef struct __EINK_WF_DATA_S
{
    __u16 total_frame;            //帧数
    __u8 wf_data[ROW_MAX][COL_MAX];    
}eink_wf_data_t;

extern  eink_wf_data_t eink_gc16_mode_wf[14];

extern const __u32 eink_ctrl_line_index_tbl[EINK_LCD_H];
extern const __u32 eink_ctrl_tbl_GC16_COMMON[8][258];
extern const __u32 eink_ctrl_tbl_GC16_TIANZHI[8][258];

extern const __u8 eink_init_T_tbl_OED[T_MAX];
extern const eink_init_wf_t eink_init_mode_wf_OED[15];
extern const __u8 eink_A2_mode_wf_OED[3][10];

extern const __u8 eink_init_T_tbl_PVI[T_MAX];
extern const eink_init_wf_t eink_init_mode_wf_PVI[15];
extern const __u8 eink_A2_mode_wf_PVI[3][10];


#endif
