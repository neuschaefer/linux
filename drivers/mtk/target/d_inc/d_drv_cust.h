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
 * $RCSfile: d_drv_cust.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *---------------------------------------------------------------------------*/

#ifndef _D_DRV_CUST_H_
#define _D_DRV_CUST_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_start_common.h"
#include "x_rm_dev_types.h"
#include "x_vid_plane.h"
#include "x_vid_dec.h"
#include "x_tv_dec.h"
#include "x_aud_dec.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

typedef UINT32  CUSTOM_TYPE_T;

#define DRV_CUSTOM_VERSION                      ((UINT16) 0)
#define DRV_CUSTOM_NONE                         ((CUSTOM_TYPE_T)  0)

#define DRV_ID_END                              {DRVT_UNKNOWN, 0}
#define DRV_CUSTOM_END                          {DRV_CUSTOM_NONE, NULL, 0}
#define DRV_CUSTOM_DIR_END                      {DRVT_UNKNOWN, DRVT_UNKNOWN, NULL}

#define IS_DRV_ID_END( pt_x )                   ( (pt_x->e_type==DRVT_UNKNOWN) && (pt_x->ui2_id==0) )
#define IS_DRV_CUSTOM_END( pt_x )               ( (pt_x->e_custom_type==DRV_CUSTOM_NONE) && (pt_x->pv_arg==NULL) && (pt_x->ui2_id==0) )
#define IS_DRV_CUSTOM_DIR_END( pt_x )           ( (pt_x->e_first_type==DRVT_UNKNOWN) && (pt_x->e_last_type==DRVT_UNKNOWN) && (pt_x->pt_custom==NULL) )

#define DRV_CUSTOM_GRP( e_drv_type )            ((CUSTOM_TYPE_T) e_drv_type << 16)
#define DRV_CUSTOM_TARGET_SPEC_TYPE_BEGIN       ((CUSTOM_TYPE_T) 0x00008000)

#define AVC_GET_SCART_TYPE(_val)                (((UINT16)_val) & 0xFF00)
#define AVC_GET_SCART_ID(_val)                  (((UINT16)_val) & 0x00FF) 

/* Driver customization callback function return values */
#define DCR_OK                              ((INT32)  0)
#define DCR_INV_ARG                         ((INT32) -1)
#define DCR_NOT_SUPPORT                     ((INT32) -2)

/* Driver customization group */
#define DRV_CUSTOM_GRP_COMMON                   DRV_CUSTOM_GRP(DRVT_UNKNOWN)

/* common customization types*/
#define DRV_CUSTOM_TYPE_COMMON_REG                  (DRV_CUSTOM_GRP_COMMON + 1)
#define DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME         (DRV_CUSTOM_GRP_COMMON + 2)
#define DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL        (DRV_CUSTOM_GRP_COMMON + 3)
#ifdef MW_PIP_POP_SUPPORT
#define DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL_AP     (DRV_CUSTOM_GRP_COMMON + 4)
#endif

/* Video Plane customization types */
#define DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT        (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 0)
#define DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT   (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 1)
#define DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX     (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 2)
#define DRV_CUSTOM_TYPE_VID_PLA_GET_CURRENT_DISP_FMT (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 3)
#define DRV_CUSTOM_TYPE_VID_PLA_OVERSCAN_CB          (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 4)
#define DRV_CUSTOM_TYPE_VID_PLA_UPDATE_OVERSCAN_TBL  (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 5)
#define DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT_EX (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 6)
#define DRV_CUSTOM_TYPE_VID_PLA_VID_QA_TBL_QUERY_CB  (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 7)
#define DRV_CUSTOM_TYPE_VID_PLA_GENERAL_CB           (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 8)
#define DRV_CUSTOM_TYPE_VID_PLA_SRC_REGION_CHG_CB           (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 9)
#define DRV_CUSTOM_TYPE_VID_PLA_OUT_REGION_CHG_CB           (DRV_CUSTOM_GRP(DRVT_VID_PLANE) + 10)

/* Plane Mixer customization types */
#define DRV_CUSTOM_TYPE_PLA_MXR_DISP_QA_MIN_MAX_DFT  (DRV_CUSTOM_GRP(DRVT_PLA_MXR) + 0)

/* AVC combi customization types */
#define DRV_CUSTOM_TYPE_AVC_COMBI_REG           (DRV_CUSTOM_GRP(DRVT_AVC_COMBI) + 0)

/* AVC Scart output customization types */
#define DRV_CUSTOM_TYPE_AVC_SCART_OUT_REG       (DRV_CUSTOM_GRP(DRVT_AVC_SCART_OUT) + 0)

/* AVC SCART driver type */
#define AVC_SCART_TYPE_SOURCE           (MAKE_BIT_MASK_16(0) << 8)
#define AVC_SCART_TYPE_SINK             (MAKE_BIT_MASK_16(1) << 8)
#define AVC_SCART_TYPE_MONITOR          (MAKE_BIT_MASK_16(2) << 8)

#define AVC_SCART_TYPE_SOURCE_SINK      (AVC_SCART_TYPE_SOURCE | AVC_SCART_TYPE_SINK)

typedef struct _DRV_ID_T
{
   DRV_TYPE_T     e_type;
   UINT16         ui2_id;
} DRV_ID_T;

typedef struct _DRV_CUSTOM_T
{
   CUSTOM_TYPE_T  e_custom_type;
   VOID*          pv_arg;
   UINT16         ui2_id;
} DRV_CUSTOM_T;

typedef struct _DRV_CUSTOM_DIR_T
{
   DRV_TYPE_T     e_first_type;
   DRV_TYPE_T     e_last_type;
   DRV_CUSTOM_T*  pt_custom;
} DRV_CUSTOM_DIR_T;


/* DRV_CUSTOM_TYPE_COMMON_REG */
typedef struct _DRV_REG_T
{
    CHAR           s_name[RM_NAME_LEN];
    UINT32         ui4_hw_addr;
    DRV_ID_T       t_drv_id;
    DRV_ID_T*      pt_connection_ids;
} DRV_REG_T;

/* DRV_CUSTOM_TYPE_COMMON_REG_GRP_NAME */
typedef struct _DRV_REG_GRP_NAME_T
{
    CHAR           s_grp_name[RM_NAME_LEN];
    DRV_ID_T*      pt_drv_ids;
} DRV_REG_GRP_NAME_T;

/* DRV_CUSTOM_TYPE_COMMON_REG_COMP_EXCL */
typedef struct _DRV_REG_COMP_EXCL_T
{
    DRV_ID_T*      pt_drv_ids;
} DRV_REG_COMP_EXCL_T;

/* DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT */
typedef struct _DRV_CUSTOM_RESOLUTION_INFO_T
{
    UINT16                  ui2_width;    /* pixel base */
    UINT16                  ui2_height;   /* pixel base */
}  DRV_CUSTOM_RESOLUTION_INFO_T;

typedef VOID (*d_vid_pla_xlat_disp_fmt_fct) (
    const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_src_res,
    const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_disp_res,
    UINT8                                   ui1_afd,
    VID_PLA_VID_REGION_T*                   pt_src_region,     /* OUT */
    VID_PLA_VID_REGION_T*                   pt_disp_region );  /* OUT */

typedef struct _DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_INFO_T
{
    VID_PLA_DISP_FMT_T              e_disp_fmt;
    d_vid_pla_xlat_disp_fmt_fct     pf_xlat_disp_fmt;
} DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_INFO_T;

/* DRV_CUSTOM_TYPE_VID_PLA_XLAT_DISP_FMT_EX */
typedef enum
{
    DRV_CUSTOM_VID_SRC_INFO_TYPE_UNKNOWN = 0,
    DRV_CUSTOM_VID_SRC_INFO_TYPE_MPEG,
    DRV_CUSTOM_VID_SRC_INFO_TYPE_NTSC_WSS,
    DRV_CUSTOM_VID_SRC_INFO_TYPE_PAL_SECAM_WSS_SCART,
    DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_ASP_RATIO, /* will be replaced by DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AVI_INFOFRAME */
    DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AFD, /* will be replaced by DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AVI_INFOFRAME */
    DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AVI_INFOFRAME,
    DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC
} DRV_CUSTOM_VID_SRC_INFO_TYPE_T;

/* replace DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_ASP_RATIO */
#define DRV_CUSTOM_HDMI_PIC_ASP_RATIO_MASK       ((UINT8)0x30)
#define DRV_CUSTOM_GET_HDMI_PIC_ASP_RATIO( ui1_hdmi_avi_infoframe_byte_2 )                  \
    (((ui1_hdmi_avi_infoframe_byte_2) & DRV_CUSTOM_HDMI_PIC_ASP_RATIO_MASK)>>4)

#define DRV_CUSTOM_HDMI_PIC_ASP_RATIO_NO_DATA          ((UINT8)0x0)
#define DRV_CUSTOM_HDMI_PIC_ASP_RATIO_4_3              ((UINT8)0x1)
#define DRV_CUSTOM_HDMI_PIC_ASP_RATIO_16_9             ((UINT8)0x2)
#define DRV_CUSTOM_HDMI_PIC_ASP_RATIO_FUTURE           ((UINT8)0x3)

/* replace DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AFD */
#define DRV_CUSTOM_HDMI_AFAR_MASK                ((UINT8)0x0f)
#define DRV_CUSTOM_GET_HDMI_AFAR( ui1_hdmi_avi_infoframe_byte_2 )                           \
    ((ui1_hdmi_avi_infoframe_byte_2) & DRV_CUSTOM_HDMI_AFAR_MASK)

#define DRV_CUSTOM_HDMI_AFAR_SAME_AS_PIC_ASP_RATIO     ((UINT8)0x8)
#define DRV_CUSTOM_HDMI_AFAR_4_3_CENTER                ((UINT8)0x9)
#define DRV_CUSTOM_HDMI_AFAR_16_9_CENTER               ((UINT8)0xa)
#define DRV_CUSTOM_HDMI_AFAR_14_9_CENTER               ((UINT8)0xb)

typedef enum
{
    DRV_CUSTOM_SCART_PIN_8_UNKNOWN = 0,
    DRV_CUSTOM_SCART_PIN_8_NO_SIGNAL,    
    DRV_CUSTOM_SCART_PIN_8_ASPECT_16_9,
    DRV_CUSTOM_SCART_PIN_8_ASPECT_4_3
}   DRV_CUSTOM_SCART_PIN_8_T;

typedef enum
{
    DRV_CUSTOM_HDMI_ASP_RATIO_UNKNOWN          = 0,
    DRV_CUSTOM_HDMI_ASP_RATIO_ASPECT_4_3       = 1,
    DRV_CUSTOM_HDMI_ASP_RATIO_ASPECT_16_9      = 2,
    DRV_CUSTOM_HDMI_ASP_RATIO_ASPECT_FUTURE    = 3
}   DRV_CUSTOM_HDMI_ASP_RATIO_T;

typedef struct _DRV_CUSTOM_VID_SRC_WSS_SCART_INFO_T
{
    UINT16                     ui2_pal_secam_wss; 
    DRV_CUSTOM_SCART_PIN_8_T   e_scart_pin_8; /* It will be handled only when ui2_pal_secam_wss=0 */
} DRV_CUSTOM_VID_SRC_WSS_SCART_INFO_T;

typedef struct _DRV_CUSTOM_VID_SRC_MPEG_INFO_T
{
    UINT8                        ui1_afd;
    BOOL  					     b_is_display_aspect_ratio;
    UINT32 						 u4_aspect_ratio_w;
    UINT32 						 u4_aspect_ratio_h;
    VID_DEC_SRC_ASPECT_RATIO_T   e_asp_ratio;
    BOOL								b_is_cropping;
    UINT32							u4_cropping_x;
    UINT32							u4_cropping_y;
    UINT32							u4_cropping_width;
    UINT32							u4_cropping_height;
} DRV_CUSTOM_VID_SRC_MPEG_INFO_T;

typedef struct _DRV_CUSTOM_VID_SRC_HDMI_AVI_INFOFRAME_INFO_T
{
    UINT8                        ui1_byte_1;
    UINT8                        ui1_byte_2;
    UINT8                        ui1_byte_3;    
    UINT8                        ui1_byte_4;
    UINT8                        ui1_byte_5;
    UINT8                        ui1_byte_6;
} DRV_CUSTOM_VID_SRC_HDMI_AVI_INFOFRAME_INFO_T;

typedef enum
{
    DRV_CUST_INPUT_SRC_TYPE_UNKNOWN = 0,
    DRV_CUST_INPUT_SRC_TYPE_DTV,
    DRV_CUST_INPUT_SRC_TYPE_ATV,
    DRV_CUST_INPUT_SRC_TYPE_CVBS,
    DRV_CUST_INPUT_SRC_TYPE_YPBPR,
    DRV_CUST_INPUT_SRC_TYPE_VGA,
    DRV_CUST_INPUT_SRC_TYPE_HDMI,
    DRV_CUST_INPUT_SRC_TYPE_DVI,
    DRV_CUST_INPUT_SRC_TYPE_SCART,
    DRV_CUST_INPUT_SRC_TYPE_MULTIMEDIA,
    DRV_CUST_INPUT_SRC_TYPE_TIMESHIFT,
    DRV_CUST_INPUT_SRC_TYPE_WILDCARD
} DRV_CUST_INPUT_SRC_TYPE_T;

typedef struct _DRV_CUSTOM_VID_SRC_INPUT_SOURCE_INFO_T
{
    DRV_CUST_INPUT_SRC_TYPE_T    e_input_source;
    UINT16                       ui2_id;
    TV_DEC_TIMING_TYPE_T         e_timing_type;
} DRV_CUSTOM_VID_SRC_INPUT_SOURCE_INFO_T;

typedef struct _DRV_CUSTOM_VID_SRC_INFO_T
{
    UINT16                           ui2_vdp_id;
    DRV_CUSTOM_VID_SRC_INFO_TYPE_T   e_type;
    UINT32 ui4_panel_width;
    UINT32 ui4_panel_height;
    union
    {
        VOID*                               pv_unknown;
        DRV_CUSTOM_VID_SRC_MPEG_INFO_T      t_mpeg;
        UINT32                              ui4_ntsc_wss;
        DRV_CUSTOM_VID_SRC_WSS_SCART_INFO_T t_wss_scart;
        DRV_CUSTOM_HDMI_ASP_RATIO_T         e_hdmi_asp; /* will be replaced by t_hdmi_avi_infoframe */
        UINT8                               ui1_hdmi_afd; /* will be replaced by t_hdmi_avi_infoframe */
        DRV_CUSTOM_VID_SRC_HDMI_AVI_INFOFRAME_INFO_T t_hdmi_avi_infoframe; /* DRV_CUSTOM_VID_SRC_INFO_TYPE_HDMI_AVI_INFOFRAME */
        DRV_CUSTOM_VID_SRC_INPUT_SOURCE_INFO_T t_input_src; /* DRV_CUSTOM_VID_SRC_INFO_TYPE_INPUT_SRC */
    } u;
} DRV_CUSTOM_VID_SRC_INFO_T;

typedef VOID (*d_vid_pla_xlat_disp_fmt_ex_fct) (
    const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_src_res,
    const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_disp_res,
    const DRV_CUSTOM_VID_SRC_INFO_T*        pt_vid_src_info,
    VID_PLA_VID_REGION_T*                   pt_src_region,     /* OUT */
    VID_PLA_VID_REGION_T*                   pt_disp_region     /* OUT */
    #ifdef SYS_OVERRIDE_DISP_REGION_SUPPORT
    ,
    VID_PLA_VID_REGION_T*                   pt_disp_reg_override  /* OUT */
    #endif
    );

typedef struct _DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T
{
    VID_PLA_DISP_FMT_T              e_disp_fmt;
    d_vid_pla_xlat_disp_fmt_ex_fct  pf_xlat_disp_fmt;
} DRV_CUSTOM_VID_PLA_XLAT_DISP_FMT_EX_INFO_T;

/* DRV_CUSTOM_TYPE_VID_PLA_GET_CURRENT_DISP_FMT */
typedef VOID (*d_vid_pla_get_current_disp_fmt_fct) (
    const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_src_res,
    const DRV_CUSTOM_RESOLUTION_INFO_T*     pt_disp_res,
    const DRV_CUSTOM_VID_SRC_INFO_T*        pt_vid_src_info,
    VID_PLA_DISP_FMT_T*                     pe_disp_fmt );     /* OUT */

typedef VOID (*d_vid_pla_source_region_fct) (
    const UINT16                            ui2_vdp_id,
    const UINT32                                  u4X,      
    const UINT32                                  u4Y,    
    const UINT32                                  u4Width,    
    const UINT32                                  u4Height    
);
typedef VOID (*d_vid_pla_Output_region_fct) (
    const UINT16                            ui2_vdp_id,
    const UINT32                                  u4X,      
    const UINT32                                  u4Y,    
    const UINT32                                  u4Width,    
    const UINT32                                  u4Height    
);
/* DRV_CUSTOM_TYPE_VID_PLA_OVERSCAN_CB */
typedef enum
{
    DRV_CUSTOM_TIMING_SCAN_MODE_INTERLACE,
    DRV_CUSTOM_TIMING_SCAN_MODE_PROGRESSIVE,
    DRV_CUSTOM_TIMING_SCAN_MODE_WILDCARD
} DRV_CUSTOM_TIMING_TYPE_T;

typedef enum
{
    DRV_CUSTOM_COLOR_SYS_UNKNOWN = -1,
    DRV_CUSTOM_COLOR_SYS_NTSC,
    DRV_CUSTOM_COLOR_SYS_NTSC_443,
    DRV_CUSTOM_COLOR_SYS_PAL,
    DRV_CUSTOM_COLOR_SYS_PAL_N,
    DRV_CUSTOM_COLOR_SYS_PAL_M,
    DRV_CUSTOM_COLOR_SYS_PAL_60,
    DRV_CUSTOM_COLOR_SYS_SECAM,
    DRV_CUSTOM_COLOR_SYS_WILDCARD
} DRV_CUSTOM_COLOR_SYS_TYPE_T;

#define DRV_CUSTOM_TIMING_WIDTH_WILDCARD        ((UINT16)0xFFFF)
#define DRV_CUSTOM_TIMING_HEIGHT_WILDCARD       ((UINT16)0xFFFF)
#define DRV_CUSTOM_TIMING_FRAME_RATE_WILDCARD   ((UINT16)0xFFFFFFFF)

#define DRV_CUSTOM_TIMING_WILDCARD   {                                        \
    DRV_CUSTOM_TIMING_WIDTH_WILDCARD,                                         \
    DRV_CUSTOM_TIMING_HEIGHT_WILDCARD,                                        \
    DRV_CUSTOM_TIMING_FRAME_RATE_WILDCARD,                                    \
    DRV_CUSTOM_TIMING_SCAN_MODE_WILDCARD,                                     \
    DRV_CUSTOM_COLOR_SYS_WILDCARD }

typedef struct _DRV_CUSTOM_TIMING_INFO_T
{
    UINT16                         ui2_width;
    UINT16                         ui2_height;
    UINT32                         ui4_fram_rate; /* Hz unit */
    DRV_CUSTOM_TIMING_TYPE_T       e_scan_mode;
    DRV_CUSTOM_COLOR_SYS_TYPE_T    e_color_sys;
    TV_DEC_TIMING_TYPE_T           e_timing_type;
    VID_DEC_SRC_FRAME_RATE_T       e_vid_frame_rate;
    AUD_DEC_FMT_T                  e_aud_fmt;
} DRV_CUSTOM_TIMING_INFO_T;

typedef VOID (*d_vid_pla_overscan_fct) (
    const UINT16                            ui2_vdp_id,
    const DRV_CUST_INPUT_SRC_TYPE_T         e_src_type,
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing,
    UINT16                                  *pui2_up,      /* OUT */
    UINT16                                  *pui2_down,    /* OUT */
    UINT16                                  *pui2_left,    /* OUT */
    UINT16                                  *pui2_right    /* OUT */
);

/* DRV_CUSTOM_TYPE_VID_PLA_UPDATE_OVERSCAN_TBL */
typedef VOID (*d_vid_pla_update_overscan_tbl_fct) (
    const UINT16                            ui2_vdp_id,
    const DRV_CUST_INPUT_SRC_TYPE_T         e_src_type,
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing,
    const UINT16                            ui2_up,
    const UINT16                            ui2_down,
    const UINT16                            ui2_left,
    const UINT16                            ui2_right
);

/* DRV_CUSTOM_TYPE_VID_PLA_GENERAL_CB */
typedef enum
{
    DRV_CUSTOM_GET_TYPE_UNKNOWN = 0,
    DRV_CUSTOM_GET_TYPE_AUDIO_DELAY,
    DRV_CUSTOM_GET_TYPE_NON_LINEAR_FACTOR
} DRV_CUSTOM_GET_TYPE_T;

/* DRV_CUSTOM_GET_TYPE_AUDIO_DELAY */
#define VID_AUD_DELAY_TBL_END   { VID_DEC_SRC_FRAME_RATE_UNKNOWN, '*', 0 }
typedef struct _VID_AUD_DELAY_TBL_T
{
    VID_DEC_SRC_FRAME_RATE_T  e_vid_frame_rate;
    CHAR                      c_progressive;    /* 'i' = interlace,
                                                   'p' = progressive,
                                                   '*' = don't care  */
    UINT32                    ui4_total_delay;  /* in mini second */
} VID_AUD_DELAY_TBL_T;

typedef struct _DRV_CUSTOM_AUDIO_DELAY_INFO_T
{
    UINT32 ui4_total_delay; /* in mini second */
} DRV_CUSTOM_AUDIO_DELAY_INFO_T;

/* DRV_CUSTOM_GET_TYPE_NON_LINEAR_FACTOR */
typedef struct _DRV_CUSTOM_NON_LINEAR_FACTOR_INFO_T
{
    BOOL   b_auto;
    UINT32 ui4_start;
    UINT32 ui4_slope;
    UINT32 ui4_middle;
    UINT32 ui4_stop;
} DRV_CUSTOM_NON_LINEAR_FACTOR_INFO_T;

typedef struct _DRV_CUSTOM_GET_INFO_T
{
    DRV_CUSTOM_GET_TYPE_T e_type;
    
    union
    {
        DRV_CUSTOM_AUDIO_DELAY_INFO_T       t_audio_delay;
        DRV_CUSTOM_NON_LINEAR_FACTOR_INFO_T t_non_linear_factor;
    } u;
} DRV_CUSTOM_GET_INFO_T;

/* general call back function prototype **************************************/ 
typedef INT32 (*d_vid_pla_cust_cb_fct) (
    UINT16                                  ui2_vdp_id,    /* Main or Sub */
    DRV_CUST_INPUT_SRC_TYPE_T               e_src_type,    /* Source type */
    const DRV_CUSTOM_TIMING_INFO_T*         pt_src_timing, /* Timing info */
    DRV_CUSTOM_GET_INFO_T*                  pt_get_info
);

/* DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT */
/* Video Quality types */
typedef enum
{
    VID_QA_TYPE_UNKNOWN = -1,
    VID_QA_TYPE_BRIGHTNESS,
    VID_QA_TYPE_CONTRAST,
    VID_QA_TYPE_HUE,
    VID_QA_TYPE_SATURATION,
    VID_QA_TYPE_CTI,
    VID_QA_TYPE_SHARPNESS,
    VID_QA_TYPE_LTI,
    VID_QA_TYPE_COLOR_GAIN,
    VID_QA_TYPE_COLOR_OFFSET,
    VID_QA_TYPE_COLOR_SUPPRESS,
    VID_QA_TYPE_NR,
    VID_QA_TYPE_GAMMA,
    VID_QA_TYPE_WHITE_PEAK_LMT,
    VID_QA_TYPE_FLESH_TONE,
  
    VID_QA_TYPE_LUMA,
    VID_QA_TYPE_3D_NR,
    VID_QA_TYPE_DEMO,
    VID_QA_TYPE_DI_FILM_MODE,
    VID_QA_TYPE_DI_MA,
    VID_QA_TYPE_DI_EDGE_PRESERVE,
    VID_QA_TYPE_MJC_MODE,
    VID_QA_TYPE_MJC_EFFECT,
    VID_QA_TYPE_MJC_DEMO,
    VID_QA_TYPE_XVYCC,
    VID_QA_TYPE_WCG,
    VID_QA_TYPE_GAME_MODE,
    VID_QA_TYPE_BLUE_STRETCH, 
    VID_QA_TYPE_MPEG_NR,           
    VID_QA_TYPE_BLACK_STRETCH,       
    VID_QA_TYPE_WHITE_STRETCH,

    VID_QA_TYPE_ETI,
    VID_QA_TYPE_BLACK_LVL_EXT,

    /* do not remove this */
    VID_QA_TYPE_MAX
} VID_QA_TYPE_T;

#define VID_QA_MIN_MAX_DFT_END   { VID_QA_TYPE_UNKNOWN, 0, 0, 0 }
typedef struct _VID_QA_MIN_MAX_DFT_T
{
    VID_QA_TYPE_T e_vid_qa_type;
    INT32         i4_min;
    INT32         i4_max;
    INT32         i4_default;
} VID_QA_MIN_MAX_DFT_T;

typedef struct _VID_QA_MIN_MAX_DFT_D_T	
{
	UINT8	ui1_dimension_i;
	UINT8	ui1_dimension_j;
	VID_QA_MIN_MAX_DFT_T* pt_min_max_dft_lst;
} VID_QA_MIN_MAX_DFT_D_T;

#define VID_QA_TBL_END           { DRVT_UNKNOWN, NULL }
typedef struct _VID_QA_TBL_T
{
    DRV_TYPE_T             e_src_type;
    VID_QA_MIN_MAX_DFT_T*  pt_min_max_dft_lst;
} VID_QA_TBL_T;

/* DRV_CUSTOM_TYPE_VID_PLA_VID_QA_MIN_MAX_DFT_EX */
/* Video Quality types */
#define VID_QA_TBL_EX_END           { DRVT_UNKNOWN, VID_PLA_PIC_MODE_DEFAULT, NULL }
typedef struct _VID_QA_TBL_EX_T
{
    DRV_TYPE_T             e_src_type;
    VID_PLA_PIC_MODE_T     e_pic_mode;
    VID_QA_MIN_MAX_DFT_T*  pt_min_max_dft_lst;
} VID_QA_TBL_EX_T;

/* DRV_CUSTOM_TYPE_VID_PLA_VID_QA_TBL_QUERY_CB */
typedef INT32 (*d_vid_pla_vid_qa_tbl_query_fct) (
    const UINT16                            ui2_vdp_id,
    const DRV_TYPE_T                        e_src_type,
    const VID_PLA_PIC_MODE_T                e_pic_mode,
    VID_QA_MIN_MAX_DFT_T**                  ppt_min_max_dft_lst /* OUT */
);

/* DRV_CUSTOM_TYPE_PLA_MXR_DISP_QA_MIN_MAX_DFT */
/* Display Quality types */
typedef enum
{
    DISP_QA_TYPE_UNKNOWN = -1,
    DISP_QA_TYPE_BACK_LIGHT,
    DISP_QA_TYPE_ADP_BACK_LIGHT,
    
    /* do not remove this */
    DISP_QA_TYPE_MAX
} DISP_QA_TYPE_T;

#define DISP_QA_MIN_MAX_DFT_END   { DISP_QA_TYPE_UNKNOWN, 0, 0, 0 }
typedef struct _DISP_QA_MIN_MAX_DFT_T
{
    DISP_QA_TYPE_T e_disp_qa_type;
    INT32          i4_min;
    INT32          i4_max;
    INT32          i4_default;
} DISP_QA_MIN_MAX_DFT_T;

#define DISP_QA_TBL_END           { DRVT_UNKNOWN, NULL }
typedef struct _DISP_QA_TBL_T
{
    DRV_TYPE_T              e_src_type;
    DISP_QA_MIN_MAX_DFT_T*  pt_min_max_dft_lst;
} DISP_QA_TBL_T;

/* DRV_CUSTOM_TYPE_AVC_COMBI_REG */
typedef struct _DRV_CUSTOM_TYPE_AVC_COMBI_REG
{
    CHAR           s_name[RM_NAME_LEN];
    DRV_ID_T       t_drv_id;
    DRV_ID_T*      pt_combi_drv_ids;
    DRV_ID_T*      pt_connection_ids;
} DRV_CUSTOM_TYPE_AVC_COMBI_REG_T;


/* AVC audio input ID definition *********************************************/
typedef enum
{
    AUD_INP_COMP_ID_FOR_COMP_VID_0 = 0,
    AUD_INP_COMP_ID_FOR_COMP_VID_1,
    AUD_INP_COMP_ID_FOR_COMP_VID_2,
    AUD_INP_COMP_ID_FOR_COMP_VID_3,
    AUD_INP_COMP_ID_FOR_S_VID_0,
    AUD_INP_COMP_ID_FOR_S_VID_1,
    AUD_INP_COMP_ID_FOR_S_VID_2,
    AUD_INP_COMP_ID_FOR_YPBPR_0,
    AUD_INP_COMP_ID_FOR_YPBPR_1,
    AUD_INP_COMP_ID_FOR_YPBPR_2,
    AUD_INP_COMP_ID_FOR_VGA_0,
    AUD_INP_COMP_ID_FOR_VGA_1,
    AUD_INP_COMP_ID_FOR_HDMI_0,
    AUD_INP_COMP_ID_FOR_HDMI_1,
    AUD_INP_COMP_ID_FOR_HDMI_2,
    AUD_INP_COMP_ID_FOR_HDMI_3,
    AUD_INP_COMP_ID_FOR_HDMI_4,
    AUD_INP_COMP_ID_FOR_DVI_0,
    AUD_INP_COMP_ID_FOR_DVI_1,
    AUD_INP_COMP_ID_FOR_DVI_2,
    AUD_INP_COMP_ID_FOR_DVI_3,
    AUD_INP_COMP_ID_FOR_DVI_4,
    AUD_INP_COMP_ID_FOR_SCART_0,
    AUD_INP_COMP_ID_FOR_SCART_1,
    AUD_INP_COMP_ID_FOR_SCART_2,
    AUD_INP_COMP_ID_FOR_SCART_3, 
    AUD_INP_COMP_ID_FOR_AUX_0,
    AUD_INP_COMP_ID_FOR_AUX_1,
    AUD_INP_COMP_ID_MAX
} AUD_INP_COMP_ID_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _D_DRV_CUST_H_ */
