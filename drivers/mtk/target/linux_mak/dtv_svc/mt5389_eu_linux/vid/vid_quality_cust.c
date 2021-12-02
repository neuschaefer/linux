/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $RCSfile: vid_quality_cust.c,v $
 * $Revision: #2 $
 * $Date: 2012/05/23 $
 * $Author: hongjun.chu $
 *
 * Description:
 *         This file contains application initialization.
 *---------------------------------------------------------------------------*/
#include "d_drv_cust.h"
#include "app_util/a_cfg.h"

/* Extern function prototypes. */
INT32 asp_ratio_get_disp_fmt_by_inp_src (const DRV_CUST_INPUT_SRC_TYPE_T e_inp_src_type,
                                         const DRV_CUSTOM_TIMING_INFO_T* pt_src_timing,
                                         VID_PLA_DISP_FMT_T* pe_disp_fmt);
/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
typedef struct _VID_QA_TBL_CUST_T
{
    DRV_TYPE_T              e_src_type;
    VID_PLA_PIC_MODE_T      e_pic_mode;
    VID_QA_MIN_MAX_DFT_T*   pt_min_max_dft;
} VID_QA_TBL_CUST_T;

/******************************************************************************
 * Video related customization 
 *****************************************************************************/
#define VID_QA_ELEM_NUM     (16)

VID_QA_MIN_MAX_DFT_T at_drv_vdp_vid_qa_min_max_dft[][VID_QA_ELEM_NUM] =
{
    /* User */
    {
        /*type                          MIN,  MAX,  DFT */
        { VID_QA_TYPE_BRIGHTNESS,         0,  100,   50 },
        { VID_QA_TYPE_CONTRAST,           0,  100,   80 },
        { VID_QA_TYPE_HUE,              -50,   50,    0 },
        { VID_QA_TYPE_SATURATION,         0,  100,   60 },
        { VID_QA_TYPE_CTI,                0,    3,    2 },
        { VID_QA_TYPE_SHARPNESS,          0,   20,   12 },
        { VID_QA_TYPE_COLOR_GAIN,        -20,  20,    0 },
        { VID_QA_TYPE_COLOR_OFFSET,      -20,  20,    0 },
        { VID_QA_TYPE_NR,                 0,    4,    2 },
        { VID_QA_TYPE_WHITE_PEAK_LMT,     0,    1,    1 },
        { VID_QA_TYPE_FLESH_TONE,         0,    3,    2 },
        { VID_QA_TYPE_LUMA,               0,    3,    2 },
        { VID_QA_TYPE_BLACK_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_WHITE_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_3D_NR,              0,    4,    2 },
        
        /* DO NOT REMOVE THIS LINE */
        VID_QA_MIN_MAX_DFT_END
    },
    
    /* Cinema */
    {
        /*type                          MIN,  MAX,  DFT */
        { VID_QA_TYPE_BRIGHTNESS,         0,  100,   40 },
        { VID_QA_TYPE_CONTRAST,           0,  100,   60 },
        { VID_QA_TYPE_HUE,              -50,   50,    0 },
        { VID_QA_TYPE_SATURATION,         0,  100,   50 },
        { VID_QA_TYPE_CTI,                0,    3,    2 },
        { VID_QA_TYPE_SHARPNESS,          0,   20,    6 },
        { VID_QA_TYPE_COLOR_GAIN,        -20,  20,    0 },
        { VID_QA_TYPE_COLOR_OFFSET,      -20,  20,    0 },
        { VID_QA_TYPE_NR,                 0,    4,    2 },
        { VID_QA_TYPE_WHITE_PEAK_LMT,     0,    1,    1 },
        { VID_QA_TYPE_FLESH_TONE,         0,    3,    2 },
        { VID_QA_TYPE_LUMA,               0,    3,    2 },
        { VID_QA_TYPE_BLACK_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_WHITE_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_3D_NR,              0,    4,    2 },
        
        /* DO NOT REMOVE THIS LINE */
        VID_QA_MIN_MAX_DFT_END
    },
    
    /* Sport */
    {
        /*type                          MIN,  MAX,  DFT */
        { VID_QA_TYPE_BRIGHTNESS,         0,  100,   50 },
        { VID_QA_TYPE_CONTRAST,           0,  100,   70 },
        { VID_QA_TYPE_HUE,              -50,   50,    0 },
        { VID_QA_TYPE_SATURATION,         0,  100,   55 },
        { VID_QA_TYPE_CTI,                0,    3,    2 },
        { VID_QA_TYPE_SHARPNESS,          0,   20,   14 },
        { VID_QA_TYPE_COLOR_GAIN,        -20,  20,    0 },
        { VID_QA_TYPE_COLOR_OFFSET,      -20,  20,    0 },
        { VID_QA_TYPE_NR,                 0,    4,    2 },
        { VID_QA_TYPE_WHITE_PEAK_LMT,     0,    1,    1 },
        { VID_QA_TYPE_FLESH_TONE,         0,    3,    2 },
        { VID_QA_TYPE_LUMA,               0,    3,    2 },
        { VID_QA_TYPE_BLACK_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_WHITE_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_3D_NR,              0,    4,    2 },
        
        /* DO NOT REMOVE THIS LINE */
        VID_QA_MIN_MAX_DFT_END
    },
    
    /* Vivid */
    {
        /*type                          MIN,  MAX,  DFT */
        { VID_QA_TYPE_BRIGHTNESS,         0,  100,   50 },
        { VID_QA_TYPE_CONTRAST,           0,  100,   80 },
        { VID_QA_TYPE_HUE,              -50,   50,    0 },
        { VID_QA_TYPE_SATURATION,         0,  100,   60 },
        { VID_QA_TYPE_CTI,                0,    3,    2 },
        { VID_QA_TYPE_SHARPNESS,          0,   20,   12 },
        { VID_QA_TYPE_COLOR_GAIN,        -20,  20,    0 },
        { VID_QA_TYPE_COLOR_OFFSET,      -20,  20,    0 },
        { VID_QA_TYPE_NR,                 0,    4,    2 },
        { VID_QA_TYPE_WHITE_PEAK_LMT,     0,    1,    1 },
        { VID_QA_TYPE_FLESH_TONE,         0,    3,    2 },
        { VID_QA_TYPE_LUMA,               0,    3,    2 },
        { VID_QA_TYPE_BLACK_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_WHITE_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_3D_NR,              0,    4,    2 },
        
        /* DO NOT REMOVE THIS LINE */
        VID_QA_MIN_MAX_DFT_END
    },
    
    /* Hi-bright */
    {
        /*type                          MIN,  MAX,  DFT */
        { VID_QA_TYPE_BRIGHTNESS,         0,  100,   60 },
        { VID_QA_TYPE_CONTRAST,           0,  100,   75 },
        { VID_QA_TYPE_HUE,              -50,   50,    0 },
        { VID_QA_TYPE_SATURATION,         0,  100,   50 },
        { VID_QA_TYPE_CTI,                0,    3,    2 },
        { VID_QA_TYPE_SHARPNESS,          0,   20,   12 },
        { VID_QA_TYPE_COLOR_GAIN,        -20,  20,    0 },
        { VID_QA_TYPE_COLOR_OFFSET,      -20,  20,    0 },
        { VID_QA_TYPE_NR,                 0,    4,    2 },
        { VID_QA_TYPE_WHITE_PEAK_LMT,     0,    1,    1 },
        { VID_QA_TYPE_FLESH_TONE,         0,    3,    2 },
        { VID_QA_TYPE_LUMA,               0,    3,    2 },
        { VID_QA_TYPE_BLACK_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_WHITE_STRETCH,      0,    3,    2 },
        { VID_QA_TYPE_3D_NR,              0,    4,    2 },
        
        /* DO NOT REMOVE THIS LINE */
        VID_QA_MIN_MAX_DFT_END
    }
};

#define VID_PLA_PIC_MODE_UNKNOWN   VID_PLA_PIC_MODE_MAX

#define at_vid_qa_tbl_default      at_drv_vdp_vid_qa_min_max_dft[0] /* User      */
#define at_vid_qa_tbl_custom_1     at_drv_vdp_vid_qa_min_max_dft[1] /* Cinema    */
#define at_vid_qa_tbl_custom_2     at_drv_vdp_vid_qa_min_max_dft[2] /* Sport     */
#define at_vid_qa_tbl_custom_3     at_drv_vdp_vid_qa_min_max_dft[3] /* Vivid     */
#define at_vid_qa_tbl_custom_4     at_drv_vdp_vid_qa_min_max_dft[4] /* Hi-bright */

VID_QA_TBL_T at_vid_qa_tbl[] =
{
    { DRVT_AVC_COMP_VIDEO, at_vid_qa_tbl_default },
    { DRVT_AVC_S_VIDEO,    at_vid_qa_tbl_default },
    { DRVT_AVC_Y_PB_PR,    at_vid_qa_tbl_default },
    { DRVT_AVC_VGA,        at_vid_qa_tbl_default },
    { DRVT_AVC_SCART,      at_vid_qa_tbl_default },
    { DRVT_AVC_DVI,        at_vid_qa_tbl_default },
    { DRVT_AVC_HDMI,       at_vid_qa_tbl_default },
    
    { DRVT_TUNER_CAB_DIG,  at_vid_qa_tbl_default },
    { DRVT_TUNER_TER_DIG,  at_vid_qa_tbl_default },
    { DRVT_TUNER_CAB_ANA,  at_vid_qa_tbl_default },
    { DRVT_TUNER_TER_ANA,  at_vid_qa_tbl_default },
    
    /* DO NOT REMOVE THIS LINE */
    VID_QA_TBL_END
};

VID_QA_TBL_CUST_T at_vid_qa_tbl_cust[] =
{
    /* Source type         Picture mode                     QA table */
    { DRVT_UNKNOWN,        VID_PLA_PIC_MODE_DEFAULT,        at_vid_qa_tbl_default  },
#if 1 /* Only declare one picture mode table if the flash PQ is not enabled */
    { DRVT_UNKNOWN,        VID_PLA_PIC_MODE_CUSTOM_1,       at_vid_qa_tbl_custom_1 },
    { DRVT_UNKNOWN,        VID_PLA_PIC_MODE_CUSTOM_2,       at_vid_qa_tbl_custom_2 },
    { DRVT_UNKNOWN,        VID_PLA_PIC_MODE_CUSTOM_3,       at_vid_qa_tbl_custom_3 },
    { DRVT_UNKNOWN,        VID_PLA_PIC_MODE_CUSTOM_4,       at_vid_qa_tbl_custom_4 },
#endif
                                                            
    /* Don't remove this line */                            
    { DRVT_UNKNOWN,        VID_PLA_PIC_MODE_UNKNOWN,        at_vid_qa_tbl_default }
};

/******************************************************************************
 * Display related customization 
 *****************************************************************************/
DISP_QA_MIN_MAX_DFT_T at_drv_pmx_disp_qa_min_max_dft[] =
{
    /*type                          MIN,  MAX,  DFT */
    { DISP_QA_TYPE_BACK_LIGHT,        0,  100,   50 },
    { DISP_QA_TYPE_ADP_BACK_LIGHT,    0,    2,    0 },

    /* DO NOT REMOVE THIS LINE */
    DISP_QA_MIN_MAX_DFT_END
};

DISP_QA_TBL_T at_disp_qa_tbl[] =
{
    { DRVT_AVC_COMP_VIDEO, at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_AVC_S_VIDEO,    at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_AVC_Y_PB_PR,    at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_AVC_VGA,        at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_AVC_SCART,      at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_AVC_DVI,        at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_AVC_HDMI,       at_drv_pmx_disp_qa_min_max_dft },
    
    { DRVT_TUNER_CAB_DIG,  at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_TUNER_TER_DIG,  at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_TUNER_CAB_ANA,  at_drv_pmx_disp_qa_min_max_dft },
    { DRVT_TUNER_TER_ANA,  at_drv_pmx_disp_qa_min_max_dft },
    
    /* DO NOT REMOVE THIS LINE */
    DISP_QA_TBL_END
};


/******************************************************************************
 * Overscan Customization
 *
 *   [Input Source]
 *   Input Source Type  INP_SRC_TYPE_TV
 *                      INP_SRC_TYPE_AV
 *                      INP_SRC_TYPE_UNKNOWN = don't care
 *
 *   Device Type        DEV_TUNER_TER_ANA (ATV)
 *                      DEV_TUNER_TER_DIG (DTV)
 *                      DEV_AVC_COMP_VIDEO
 *                      DEV_AVC_S_VIDEO
 *                      DEV_AVC_Y_PB_PR
 *                      DEV_AVC_VGA
 *                      DEV_AVC_DVI
 *                      DEV_AVC_HDMI
 *                      DEV_UNKNOWN = don't care
 *   [Timing]
 *   Note: Graphics timing does not support overscan.
 *
 *   Width              0 = don't care 
 *   Height             0 = don't care 
 *   Frame Rate         0 = don't care 
 *   Progressive        'i' = interlace
 *                      'p' = progressive
 *                      '*' = don't care    
 *                                              
 *   [Overscan]
 *   Top
 *   Bottom
 *   Left
 *   Right
 * 
 *****************************************************************************/
APP_CFG_CUST_OVERSCAN_TBL_T  at_sys_overscan_table_main[] = 
{ 

 /* [           Input Source Type           ]    [      Timing    ] [  Overscan   ] */
 /* Input Type,            Device Type,           W,   H,Rate,Prog,  T,  B,  L,  R  */

    /* TV */
    {INP_SRC_TYPE_TV,      DEV_TUNER_TER_ANA,     0,   0,  0,  '*', 350, 250, 350, 310},
    {INP_SRC_TYPE_TV,      DEV_TUNER_TER_DIG,     0,   0,  0,  '*', 350, 250, 350, 310},
    {INP_SRC_TYPE_TV,      DEV_UNKNOWN,           0,   0,  0,  '*', 350, 250, 350, 310},

    /* Compositve */
    {INP_SRC_TYPE_AV,      DEV_AVC_COMP_VIDEO,    0,   0,  0,  '*', 350, 250, 350, 310},
    /* S Video */
    {INP_SRC_TYPE_AV,      DEV_AVC_S_VIDEO,       0,   0,  0,  '*', 350, 250, 350, 310},
    /* Component */
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0, 480,  0,  'i', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0, 480,  0,  'p', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0, 720,  0,  'i', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0, 720,  0,  'p', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0,1080,  0,  'i', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0,1080,  0,  'p', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_Y_PB_PR,       0,   0,  0,  '*', 350, 250, 350, 310},
    /* HDMI */
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0, 480,  0,  'i', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0, 480,  0,  'p', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0, 720,  0,  'i', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0, 720,  0,  'p', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0,1080,  0,  'i', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0,1080,  0,  'p', 350, 250, 350, 310},
    {INP_SRC_TYPE_AV,      DEV_AVC_HDMI,          0,   0,  0,  '*', 350, 250, 350, 310},
    /* VGA */
    {INP_SRC_TYPE_AV,      DEV_AVC_DVI,           0,   0,  0,  '*',  0,  0,  0,  0},
    {INP_SRC_TYPE_AV,      DEV_AVC_VGA,           0,   0,  0,  '*',  0,  0,  0,  0},
    /* MULTIMEDIA */
    {INP_SRC_TYPE_MM,      DEV_UNKNOWN,           0,   0,  0,  '*',  0,  0,  0,  0},                                                                               
    /* Don't remove this line. Set the default overscan */                          
    {INP_SRC_TYPE_UNKNOWN, DEV_UNKNOWN,           0,   0,  0,  '*', 350, 250, 350, 310}
};

APP_CFG_CUST_OVERSCAN_TBL_T  at_sys_overscan_table_sub[] = 
{ 
    /* Don't remove this line. Set the default overscan */                          
    {INP_SRC_TYPE_UNKNOWN, DEV_UNKNOWN,           0,   0,  0,  '*', 370, 240, 250, 300}
};

/*-----------------------------------------------------------------------------
                    functions implementation
 ----------------------------------------------------------------------------*/
 VOID vid_pla_overscan_fct (
    const UINT16                            ui2_vdp_id,
    const DRV_CUST_INPUT_SRC_TYPE_T         e_src_type,
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing,
    UINT16                                  *pui2_up,      /* OUT */
    UINT16                                  *pui2_down,    /* OUT */
    UINT16                                  *pui2_left,    /* OUT */
    UINT16                                  *pui2_right )  /* OUT */
{
    INP_SRC_TYPE_T      e_inp_src_type;
    DEVICE_TYPE_T       e_video_type;
    UINT32              ui4_width;
    UINT32              ui4_height;
    UINT32              ui4_frame_rate;
    CHAR                c_progressive;
    UINT8               ui1_i;
    VID_PLA_DISP_FMT_T  e_disp_fmt;
    INT32               i4_ret;
    APP_CFG_CUST_OVERSCAN_TBL_T*    pt_overscan_tbl = NULL;
    
    if ( !pt_src_timing || !pui2_up || !pui2_down || !pui2_left || !pui2_right )
    {
        return;
    }

    switch (ui2_vdp_id)
    {
    case 0:
    default:
        /* Main. */
        pt_overscan_tbl = at_sys_overscan_table_main;
        break;
    case 1:
        /* Sub. */
        pt_overscan_tbl = at_sys_overscan_table_sub;
        break;
    }

    i4_ret = asp_ratio_get_disp_fmt_by_inp_src (e_src_type, pt_src_timing, &e_disp_fmt);
    if (i4_ret == 0)
    {
        if (e_disp_fmt == VID_PLA_DISP_FMT_DOT_BY_DOT)
        {
            /* Set overscan to all 0 if current screen mode is dot by dot. */
            *pui2_up = 0;
            *pui2_down = 0;
            *pui2_left = 0;
            *pui2_right = 0;

            return;
        }
    }
    
    /* Get required information. */
    switch (e_src_type)
    {
    case DRV_CUST_INPUT_SRC_TYPE_DTV:
    case DRV_CUST_INPUT_SRC_TYPE_ATV:
    case DRV_CUST_INPUT_SRC_TYPE_TIMESHIFT:
        e_inp_src_type = INP_SRC_TYPE_TV; 
        break;
    case DRV_CUST_INPUT_SRC_TYPE_CVBS:
    case DRV_CUST_INPUT_SRC_TYPE_YPBPR:
    case DRV_CUST_INPUT_SRC_TYPE_VGA:
    case DRV_CUST_INPUT_SRC_TYPE_HDMI:
    case DRV_CUST_INPUT_SRC_TYPE_DVI:
    case DRV_CUST_INPUT_SRC_TYPE_SCART:
        e_inp_src_type = INP_SRC_TYPE_AV;      
        break;
    case DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA:
        e_inp_src_type = INP_SRC_TYPE_MM;      
        break;
    case DRV_CUST_INPUT_SRC_TYPE_WILDCARD:
    case DRV_CUST_INPUT_SRC_TYPE_UNKNOWN:
    default:
        e_inp_src_type = INP_SRC_TYPE_UNKNOWN;
        break;
    }
    
    switch (e_src_type)
    {
    case DRV_CUST_INPUT_SRC_TYPE_DTV:
    case DRV_CUST_INPUT_SRC_TYPE_TIMESHIFT:
        e_video_type = DEV_TUNER_TER_DIG;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_ATV:
        e_video_type = DEV_TUNER_TER_ANA;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_CVBS:
        e_video_type = DEV_AVC_COMP_VIDEO;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_YPBPR:
        e_video_type = DEV_AVC_Y_PB_PR;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_VGA:
        e_video_type = DEV_AVC_VGA;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_HDMI:
        e_video_type = DEV_AVC_HDMI;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_DVI:
        e_video_type = DEV_AVC_DVI;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_SCART:
        e_video_type = DEV_AVC_SCART;
        break;
    case DRV_CUST_INPUT_SRC_TYPE_WILDCARD:
    case DRV_CUST_INPUT_SRC_TYPE_UNKNOWN:
    default:
        e_video_type = DEV_UNKNOWN;
        break;
    }
    /* Search table at_overscan_tbl[] to find a matched entry. */
    ui4_width      = (UINT32) pt_src_timing->ui2_width;
    ui4_height     = (UINT32) pt_src_timing->ui2_height;
    ui4_frame_rate = (UINT32) pt_src_timing->ui4_fram_rate;
    
    switch (pt_src_timing->e_scan_mode)
    {
    case DRV_CUSTOM_TIMING_SCAN_MODE_INTERLACE:
        c_progressive = 'i';
        break;
    case DRV_CUSTOM_TIMING_SCAN_MODE_PROGRESSIVE:
        c_progressive = 'p';
        break;
    case DRV_CUSTOM_TIMING_SCAN_MODE_WILDCARD:
    default:
        c_progressive = '*';
        break;
    }
    
    /* Search table at_overscan_tbl[] to find a matched entry. */
    ui1_i = 0;
    
    while(TRUE)
    {
        if(ui1_i > 128)
        {
            return;
        }

        /* Input type */
        if(pt_overscan_tbl[ui1_i].e_src_type != INP_SRC_TYPE_UNKNOWN &&
           pt_overscan_tbl[ui1_i].e_src_type != e_inp_src_type)
        {
            ui1_i++; continue;
        }

        /* Video type */
        if(pt_overscan_tbl[ui1_i].e_video_type != DEV_UNKNOWN &&
           pt_overscan_tbl[ui1_i].e_video_type != e_video_type)
        {
            ui1_i++; continue;
        }

        /* Width */
        if(pt_overscan_tbl[ui1_i].ui4_width != 0 &&
           pt_overscan_tbl[ui1_i].ui4_width != ui4_width)
        {
            ui1_i++; continue;
        }

        /* Height */
        if(pt_overscan_tbl[ui1_i].ui4_height != 0 &&
           pt_overscan_tbl[ui1_i].ui4_height != ui4_height)
        {
            ui1_i++; continue;
        }

        /* Frame rate */
        if(pt_overscan_tbl[ui1_i].ui4_frame_rate != 0 &&
           pt_overscan_tbl[ui1_i].ui4_frame_rate != ui4_frame_rate)
        {
            ui1_i++; continue;
        }

        /* Progressive */
        if(pt_overscan_tbl[ui1_i].c_progressive != '*' &&
           pt_overscan_tbl[ui1_i].c_progressive != c_progressive)
        {
            ui1_i++; continue;
        }

        /* Match!! Break the loop */
        *pui2_up    = (UINT16) pt_overscan_tbl[ui1_i].ui4_top;
        *pui2_down  = (UINT16) pt_overscan_tbl[ui1_i].ui4_bottom;
        *pui2_left  = (UINT16) pt_overscan_tbl[ui1_i].ui4_left;
        *pui2_right = (UINT16) pt_overscan_tbl[ui1_i].ui4_right;
        break;
    }    
}

 VOID vid_pla_update_overscan_tbl (
    const UINT16                            ui2_vdp_id,
    const DRV_CUST_INPUT_SRC_TYPE_T         e_src_type,
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing,
    const UINT16                            ui2_up,
    const UINT16                            ui2_down,
    const UINT16                            ui2_left,
    const UINT16                            ui2_right )
{
    if ( !pt_src_timing || !ui2_up || !ui2_down || !ui2_left || !ui2_right )
    {
        return;
    }
    
    /* Search table at_overscan_tbl[] to update a matched entry. */
}

INT32 vid_pla_vid_qa_tbl_query_fct (
    const UINT16                            ui2_vdp_id,
    const DRV_TYPE_T                        e_src_type,
    const VID_PLA_PIC_MODE_T                e_pic_mode,
    VID_QA_MIN_MAX_DFT_T**                  ppt_min_max_dft_lst ) /* OUT */
{
    UINT8               ui1_i;
    
    ui1_i = 0;
    
    while(TRUE)
    {
        if(ui1_i >= (sizeof(at_vid_qa_tbl_cust) / sizeof(VID_QA_TBL_CUST_T)))
        {
            return DCR_INV_ARG;
        }

        /* Source type */
        if(at_vid_qa_tbl_cust[ui1_i].e_src_type != DRVT_UNKNOWN &&
           at_vid_qa_tbl_cust[ui1_i].e_src_type != e_src_type)
        {
            ui1_i++; continue;
        }

        /* Picture mode */
        if(at_vid_qa_tbl_cust[ui1_i].e_pic_mode != VID_PLA_PIC_MODE_UNKNOWN &&
           at_vid_qa_tbl_cust[ui1_i].e_pic_mode != e_pic_mode)
        {
            ui1_i++; continue;
        }

        /* Match!! Break the loop */
        *ppt_min_max_dft_lst = at_vid_qa_tbl_cust[ui1_i].pt_min_max_dft;
        
        break;
    }
    
    return DCR_OK;
}

