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
 * $RCSfile: x_tv_dec.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains TV Decoder driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_TV_DEC_H_
#define _X_TV_DEC_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_rm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/* Get operations */
#define TV_DEC_GET_TYPE_CTRL                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define TV_DEC_GET_TYPE_RESOLUTION          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define TV_DEC_GET_TYPE_CC_DATA             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define TV_DEC_GET_TYPE_TV_SIGNAL           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define TV_DEC_GET_TYPE_COLOR_SYS           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define TV_DEC_GET_TYPE_CLOCK               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define TV_DEC_GET_TYPE_CLOCK_MAX           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define TV_DEC_GET_TYPE_CLOCK_MIN           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define TV_DEC_GET_TYPE_PHASE               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define TV_DEC_GET_TYPE_H_POSITION          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define TV_DEC_GET_TYPE_V_POSITION          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)10))
#define TV_DEC_GET_TYPE_CONTENT_ADV_INFO	(RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)11))
#define TV_DEC_GET_TYPE_CAPABILITY          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)12))
#define TV_DEC_GET_TYPE_COLOR_SYS_CAP       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)13))
#define TV_DEC_GET_TYPE_MIN_MAX             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)14))
#define TV_DEC_GET_TYPE_SCART_INP_MODE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)15))
#define TV_DEC_GET_TYPE_AMBIG_VGA_TIMING    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)16)) /* get information of ambiguous vga timing */
#define TV_DEC_GET_TYPE_VGA_STATUS          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)17))
#define TV_DEC_GET_TYPE_SIGNAL_STRENGTH     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)18))
#define TV_DEC_GET_TYPE_VID_FREQ            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)19))
#define TV_DEC_GET_TYPE_SRC_DETECT_STATUS            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)20))

/* Set operations */
#define TV_DEC_SET_TYPE_CTRL               ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_DEC_SET_TYPE_NFY_FCT             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define TV_DEC_SET_TYPE_PLAY_CC             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define TV_DEC_SET_TYPE_STOP_CC             (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define TV_DEC_SET_TYPE_COLOR_SYS          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_DEC_SET_TYPE_CLOCK              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_DEC_SET_TYPE_PHASE              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_DEC_SET_TYPE_H_POSITION         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_DEC_SET_TYPE_V_POSITION         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_DEC_SET_TYPE_AUTO_CLK_PHS_POS    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9))
#define TV_DEC_SET_TYPE_AUTO_COLOR          (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)10))
#define TV_DEC_SET_TYPE_SCART_INP_MODE      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)11))
#define TV_DEC_SET_TYPE_PLAY_TTX            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)12))
#define TV_DEC_SET_TYPE_STOP_TTX            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)13))
#define TV_DEC_SET_TYPE_UNLOCK_TTX_BUFF     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)14))
#define TV_DEC_SET_TYPE_FLUSH_TTX_BUFF      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)15))
#define TV_DEC_SET_TYPE_MONITOR_VPS_DATA    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)16))
#define TV_DEC_SET_TYPE_STOP_VPS_DATA       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)17))
#define TV_DEC_SET_TYPE_PLAY_EU_WSS         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)18))
#define TV_DEC_SET_TYPE_STOP_EU_WSS         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)19))
#define TV_DEC_SET_TYPE_PLAY_US_WSS         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)20))
#define TV_DEC_SET_TYPE_STOP_US_WSS         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)21))
#define TV_DEC_SET_TYPE_VGA_TIMING_OPTION   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)22)) | RM_SET_TYPE_ARG_NO_REF) /* set vga timing option */
#define TV_DEC_SET_TYPE_RESET_VGA_TIMING_OPTION (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)23)) /* reset vga timing option in all groups */
#define TV_DEC_SET_TYPE_AMBIG_VGA_TIMING_CTRL ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)24)) | RM_SET_TYPE_ARG_NO_REF) /* enable/disable ambiguous vga timing option */
#define TV_DEC_SET_TYPE_SIG_BOOSTER         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)25)) | RM_SET_TYPE_ARG_NO_REF) /*force enable or disable signal booster*/
#define TV_DEC_SET_TYPE_SWITCH_SIG_BOOSTER  ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)26)) | RM_SET_TYPE_ARG_NO_REF) /*switch enable or disable signal booster flag*/
#define TV_DEC_SET_TYPE_SRC_DETECT_NFY_FCT  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)27))
#define TV_DEC_SET_TYPE_VGA_DETECT          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)28)) | RM_SET_TYPE_ARG_NO_REF)  

/* TV_DEC_GET_TYPE_CAPABILITY ************************************************/
#define TV_DEC_CAP_NONE                    ((UINT32) 0)
#define TV_DEC_CAP_CC                      MAKE_BIT_MASK_32(0)
#define TV_DEC_CAP_TV_SIGNAL              MAKE_BIT_MASK_32(1)
#define TV_DEC_CAP_COLOR_SYS             	MAKE_BIT_MASK_32(2)
#define TV_DEC_CAP_CLOCK                  MAKE_BIT_MASK_32(3)
#define TV_DEC_CAP_PHASE                  MAKE_BIT_MASK_32(4)
#define TV_DEC_CAP_H_POSITION            MAKE_BIT_MASK_32(5)
#define TV_DEC_CAP_V_POSITION            MAKE_BIT_MASK_32(6)
#define TV_DEC_CAP_CONTENT_ADV           MAKE_BIT_MASK_32(7)
#define TV_DEC_CAP_AUTO_CLK_PHS_POS     MAKE_BIT_MASK_32(8)
#define TV_DEC_CAP_AUTO_COLOR            MAKE_BIT_MASK_32(9)
#define TV_DEC_CAP_TTX                    MAKE_BIT_MASK_32(10)
#define TV_DEC_CAP_TTX_HW                MAKE_BIT_MASK_32(11)  /* this flag can be turned off only in the case that no physical hardware circuit to support TTX, for example, when the TVD is a pseudo component */
#define TV_DEC_CAP_VPS                    MAKE_BIT_MASK_32(12)
#define TV_DEC_CAP_EU_WSS                MAKE_BIT_MASK_32(13)
#define TV_DEC_CAP_US_WSS                MAKE_BIT_MASK_32(14)

/* TV_DEC_GET_TYPE_COLOR_SYS_CAP *********************************************/
#define TV_DEC_COLOR_SYS_CAP_NONE           ((UINT32) 0)
#define TV_DEC_COLOR_SYS_CAP_NTSC           MAKE_BIT_MASK_32(0)
#define TV_DEC_COLOR_SYS_CAP_NTSC_443      MAKE_BIT_MASK_32(1)
#define TV_DEC_COLOR_SYS_CAP_PAL            MAKE_BIT_MASK_32(2)
#define TV_DEC_COLOR_SYS_CAP_PAL_N          MAKE_BIT_MASK_32(3)
#define TV_DEC_COLOR_SYS_CAP_PAL_M          MAKE_BIT_MASK_32(4)
#define TV_DEC_COLOR_SYS_CAP_PAL_60         MAKE_BIT_MASK_32(5)
#define TV_DEC_COLOR_SYS_CAP_SECAM          MAKE_BIT_MASK_32(6)

/* TV_DEC_CC_COND_ERROR error code */
#define TV_DEC_CC_ERR_BUFF_OVER_FLOW       ((UINT32) 1)

/* CC condition */
typedef enum
{
    TV_DEC_CC_COND_ERROR = -1,
    TV_DEC_CC_COND_DATA_ARRIVAL
}   TV_DEC_CC_COND_T;

/* TTX Packet Size */
#define TV_DEC_TTX_PKT_SIZE                 42

/* TV_DEC_TTX_COND_ERROR error code */
#define TV_DEC_TTX_ERR_BUFF_OVER_FLOW       ((UINT32) 1)
#define TV_DEC_TTX_ERR_BUFF_ERROR           ((UINT32) 2)

/* TTX condition */
typedef enum
{
    TV_DEC_TTX_COND_ERROR = -1,
    TV_DEC_TTX_COND_DATA_ARRIVAL,
    TV_DEC_TTX_COND_MEM_MAP    /* This is a notify for linux adapter layer to do memory map.*/
}   TV_DEC_TTX_COND_T;

/* VPS DATA condition */
typedef enum
{
    TV_DEC_VPS_DATA_COND_ERROR = -1,
    TV_DEC_VPS_DATA_COND_DATA_CHANGED
}   TV_DEC_VPS_DATA_COND_T;

/* EU WSS condition */
typedef enum 
{
    TV_DEC_EU_WSS_COND_ERROR = -1,
    TV_DEC_EU_WSS_COND_DATA_ARRIVAL
} TV_DEC_EU_WSS_COND_T;

/* US WSS condition */
typedef enum 
{
    TV_DEC_US_WSS_COND_ERROR = -1,
    TV_DEC_US_WSS_COND_DATA_ARRIVAL
} TV_DEC_US_WSS_COND_T;

/* Source aspect ratios */
typedef enum
{
    TV_DEC_SRC_ASPECT_RATIO_UNKNOWN = 0,
    TV_DEC_SRC_ASPECT_RATIO_4_3,
    TV_DEC_SRC_ASPECT_RATIO_14_9,
    TV_DEC_SRC_ASPECT_RATIO_16_9,
    TV_DEC_SRC_ASPECT_RATIO_2_21_1
}   TV_DEC_SRC_ASPECT_RATIO_T;

/* TV Decoder conditions */
typedef enum
{
    TV_DEC_COND_ERROR = -1,
    TV_DEC_COND_CTRL_DONE,
    TV_DEC_COND_TV_SIGNAL,
    TV_DEC_COND_COLOR_SYS,
    TV_DEC_COND_RESOLUTION_CHG,
    TV_DEC_COND_CONTENT_ADV_INFO,
    TV_DEC_COND_TV_SIGNAL_DETECTING,
    TV_DEC_COND_HDCP_CONTENT,
    TV_DEC_COND_MACROVISION,
    TV_DEC_COND_AUD_PROTECT_INFO
}   TV_DEC_COND_T;

/* CC data condition notify function */
typedef VOID (*x_tv_dec_cc_nfy_fct)(VOID*               pv_tag,
                                    TV_DEC_CC_COND_T       e_cc_cond,
                                    VOID*                  pv_arg);

/* TTX data condition notify function */
typedef VOID (*x_tv_dec_ttx_nfy_fct)(VOID*                   pv_tag,
                                     TV_DEC_TTX_COND_T       e_ttx_cond,
                                     VOID*                   pv_arg);

/* VPS DATA condition notify function */
typedef VOID (*x_tv_dec_vps_data_nfy_fct)(VOID*                     pv_tag,
                                          TV_DEC_VPS_DATA_COND_T    e_vps_data_cond,
                                          VOID*                     pv_arg);

/* EU WSS condition notify function */
typedef VOID (*x_tv_dec_eu_wss_nfy_fct)(
    VOID*                   pv_tag,
    TV_DEC_EU_WSS_COND_T    e_eu_wss_cond,
    VOID*                   pv_arg);

/* US WSS condition notify function */
typedef VOID (*x_tv_dec_us_wss_nfy_fct)(
    VOID*                   pv_tag,
    TV_DEC_US_WSS_COND_T    e_us_wss_cond,
    VOID*                   pv_arg);

/* source detect condition notify function */
typedef VOID (*x_tv_dec_src_detect_nfy_fct)(
    VOID*                   pv_tag,
    TV_DEC_COND_T           e_nfy_cond,
    UINT32                  u4_data1,
    UINT32                  u4_data2);


/* TV Decoder condition notify function */
typedef VOID (*x_tv_dec_nfy_fct)(VOID*          pv_nfy_tag,
                                 TV_DEC_COND_T    e_nfy_cond,
                                 UINT32           ui4_data_1,
                                 UINT32           ui4_data_2);

/* TV_DEC_GET_TYPE_CTRL/TV_DEC_SET_TYPE_CTRL *********************************/
typedef enum
{
    TV_DEC_CTRL_RESET = 0x00,
    TV_DEC_CTRL_STOP  = 0x01,
    TV_DEC_CTRL_PLAY  = 0x02
}   TV_DEC_CTRL_T;

/* TV_DEC_GET_TYPE_TV_SIGNAL *************************************************/
typedef enum
{
    TV_DEC_TV_SIG_UNKNOWN = 0,
    TV_DEC_TV_SIG_NONE,
    TV_DEC_TV_SIG_STABLE
}   TV_DEC_TV_SIG_T;

/* TV_DEC_GET_TYPE_COLOR_SYS/TV_DEC_SET_TYPE_COLOR_SYS ***********************/
typedef enum
{
    TV_DEC_COLOR_SYS_UNKNOWN = -1,
    TV_DEC_COLOR_SYS_NTSC,
    TV_DEC_COLOR_SYS_NTSC_443,
    TV_DEC_COLOR_SYS_PAL,
    TV_DEC_COLOR_SYS_PAL_N,
    TV_DEC_COLOR_SYS_PAL_M,
    TV_DEC_COLOR_SYS_PAL_60,
    TV_DEC_COLOR_SYS_SECAM
}   TV_DEC_COLOR_SYS_T;

/* TV_DEC_SET_TYPE_PLAY_CC ***************************************************/
typedef struct _TV_DEC_CC_NFY_INFO_T
{
    VOID*                   pv_tag;
    x_tv_dec_cc_nfy_fct     pf_cc_nfy;
}   TV_DEC_CC_NFY_INFO_T;

/* TV_DEC_SET_TYPE_NFY_FCT ***************************************************/
typedef struct _TV_DEC_NFY_INFO_T
{
    VOID*             pv_tag;
    x_tv_dec_nfy_fct  pf_tv_dec_nfy;
}   TV_DEC_NFY_INFO_T;

/* TV_DEC_SET_TYPE_PLAY_TTX **************************************************/
typedef struct _TV_DEC_TTX_NFY_INFO_T
{
    VOID*                       pv_tag;
    x_tv_dec_ttx_nfy_fct        pf_ttx_nfy;
} TV_DEC_TTX_NFY_INFO_T;

/* TV_DEC_SET_TYPE_UNLOCK_TTX_BUFF *******************************************/
typedef struct _TV_DEC_TTX_PKT_T
{
    UINT32                      ui4_padding;  
    UINT8                       aui1_data[TV_DEC_TTX_PKT_SIZE];
} TV_DEC_TTX_PKT_T ;

typedef struct _TV_DEC_TTX_BUFF_INFO_T
{
    UINT16                      ui2_num_pkt;
    TV_DEC_TTX_PKT_T*           at_pkts;
} TV_DEC_TTX_BUFF_INFO_T;

/* TV_DEC_SET_TYPE_MONITOR_VPS_DATA *******************************************/
typedef struct _TV_DEC_VPS_DATA_NFY_INFO_T
{
    VOID*                       pv_tag;
    x_tv_dec_vps_data_nfy_fct   pf_vps_data_nfy;
} TV_DEC_VPS_DATA_NFY_INFO_T;

/* TV_DEC_SET_TYPE_PLAY_EU_WSS ***********************************************/
typedef struct _TV_DEC_EU_WSS_NFY_INFO_T
{
    VOID*                       pv_tag;
    x_tv_dec_eu_wss_nfy_fct     pf_eu_wss_nfy;
} TV_DEC_EU_WSS_NFY_INFO_T;

/* TV_DEC_SET_TYPE_PLAY_US_WSS ***********************************************/
typedef struct _TV_DEC_US_WSS_NFY_INFO_T
{
    VOID*                       pv_tag;
    x_tv_dec_us_wss_nfy_fct     pf_us_wss_nfy;
} TV_DEC_US_WSS_NFY_INFO_T;

/* TV_DEC_SET_TYPE_SRC_DETECT_NFY_FCT****************************************/
typedef struct _TV_DEC_SRC_DETECT_NFY_INFO_T
{
    VOID*                           pv_tag;
    x_tv_dec_src_detect_nfy_fct     pf_src_detect_nfy;
} TV_DEC_SRC_DETECT_NFY_INFO_T;

/* TV_DEC_GET_TYPE_CONTENT_ADV_INFO *****************************************/
typedef struct _TV_DEC_CNT_ADV_INFO_T
{
    UINT8       ui1_char_1;
    UINT8       ui1_char_2;
}   TV_DEC_CNT_ADV_INFO_T;

/* TV_DEC_GET_TYPE_MIN_MAX **************************************************/
typedef struct _TV_DEC_MIN_MAX_INFO_T
{
    UINT32     e_get_type;
    UINT32     ui4_min_value;
    UINT32     ui4_max_value;
}   TV_DEC_MIN_MAX_INFO_T;

/* CC data descriptor */
typedef struct _TV_DEC_CC_DATA_T
{
    BOOL       b_even_field;
    UINT8      ui1_byte1;
    UINT8      ui1_byte2;
} 	TV_DEC_CC_DATA_T;

/* TTX data descriptor */
typedef struct _TV_DEC_TTX_DATA_T
{
    TV_DEC_TTX_BUFF_INFO_T   t_data;
} TV_DEC_TTX_DATA_T;


/* VPS data data descriptor */
#define TV_DEC_VPS_DATA_SIZE    13

typedef struct _TV_DEC_VPS_DATA_T
{
    const UINT8*            aui1_data;
    
} TV_DEC_VPS_DATA_T;


/* EU WSS data descriptor */
typedef struct _TV_DEC_EU_WSS_DATA_T
{
    UINT16                  ui2_data;
} TV_DEC_EU_WSS_DATA_T;

/* US WSS data descriptor */
typedef struct _TV_DEC_US_WSS_DATA_T
{
    UINT16                  ui2_data;
} TV_DEC_US_WSS_DATA_T;

/* Video source resolution descriptor */
typedef enum
{
    TV_DEC_TIMING_UNKNOWN = 0,
    TV_DEC_TIMING_VIDEO,
    TV_DEC_TIMING_GRAPHIC,
    TV_DEC_TIMING_NOT_SUPPORT
}   TV_DEC_TIMING_TYPE_T;

#define TVD_DEC_EXT_HDMI_SCAN_INFO_MASK           ((UINT32)0x3)

#define TV_DEC_RES_EXT_GET_HDMI_SCAN_INFO( ui4_ext )                          \
    ((ui4_ext) & TVD_DEC_EXT_HDMI_SCAN_INFO_MASK)

#define TV_DEC_RES_EXT_SET_HDMI_SCAN_INFO( ui4_ext, scan_info )               \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFFFFC) |                         \
                      ((UINT32)scan_info & TVD_DEC_EXT_HDMI_SCAN_INFO_MASK)) )

#define TVD_DEC_EXT_HDMI_SCAN_INFO_NONE           ((UINT32)0x0)
#define TVD_DEC_EXT_HDMI_SCAN_INFO_OVERSCAN       ((UINT32)0x1)
#define TVD_DEC_EXT_HDMI_SCAN_INFO_UNDERSCAN      ((UINT32)0x2)
#define TVD_DEC_EXT_HDMI_SCAN_INFO_FUTURE         ((UINT32)0x3)

#define TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_MASK       ((UINT32)0xc)

#define TV_DEC_RES_EXT_GET_HDMI_PIC_ASP_RATIO( ui4_ext )                      \
    (((ui4_ext) & TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_MASK) >> 2)

#define TV_DEC_RES_EXT_SET_HDMI_PIC_ASP_RATIO( ui4_ext, asp_info )            \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFFFF3) |                         \
                      (((UINT32)(asp_info<<2)) & TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_MASK)) )

#define TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_NO_DATA    ((UINT32)0x0)
#define TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_4_3        ((UINT32)0x1)
#define TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_16_9       ((UINT32)0x2)
#define TVD_DEC_EXT_HDMI_PIC_ASP_RATIO_FUTURE     ((UINT32)0x3)

#define TVD_DEC_EXT_HDMI_AFAR_MASK                ((UINT32)0xf0)

#define TV_DEC_RES_EXT_GET_HDMI_AFAR( ui4_ext )                               \
    (((ui4_ext) & TVD_DEC_EXT_HDMI_AFAR_MASK) >> 4)

#define TV_DEC_RES_EXT_SET_HDMI_AFAR( ui4_ext, afar_info )                    \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFFF0F) |                         \
                      (((UINT32)(afar_info<<4)) & TVD_DEC_EXT_HDMI_AFAR_MASK)) )

#define TVD_DEC_EXT_HDMI_AFAR_SAME_AS_PIC_ASP_RATIO ((UINT32)0x8)
#define TVD_DEC_EXT_HDMI_AFAR_4_3_CENTER            ((UINT32)0x9)
#define TVD_DEC_EXT_HDMI_AFAR_16_9_CENTER           ((UINT32)0xa)
#define TVD_DEC_EXT_HDMI_AFAR_14_9_CENTER           ((UINT32)0xb)

#define TVD_DEC_EXT_NTSC_WSS_ASP_RATIO_MASK         ((UINT32)0x7000)

#define TV_DEC_RES_EXT_GET_NTSC_WSS_ASP_RATIO( ui4_ext )                      \
    (((ui4_ext) & TVD_DEC_EXT_NTSC_WSS_ASP_RATIO_MASK) >> 12)

#define TV_DEC_RES_EXT_SET_NTSC_WSS_ASP_RATIO( ui4_ext, asp_ratio )           \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFF8FFF) |                         \
                      (((UINT32)(asp_ratio<<12)) & TVD_DEC_EXT_NTSC_WSS_ASP_RATIO_MASK)) )

#define TV_DEC_RES_EXT_NTSC_WSS_4_3                         ((UINT32)0x0)
#define TV_DEC_RES_EXT_NTSC_WSS_16_9                        ((UINT32)0x1)
#define TV_DEC_RES_EXT_NTSC_WSS_4_3_LB                      ((UINT32)0x2)
#define TV_DEC_RES_EXT_NTSC_WSS_16_9_LB                     ((UINT32)0x3)
#define TV_DEC_RES_EXT_NTSC_WSS_UNKNOWN                     ((UINT32)0x4)

#define TVD_DEC_EXT_PAL_SECAM_WSS_ASP_RATIO_MASK    ((UINT32)0xf00)

#define TV_DEC_RES_EXT_GET_PAL_SECAM_WSS_ASP_RATIO( ui4_ext )                 \
    (((ui4_ext) & TVD_DEC_EXT_PAL_SECAM_WSS_ASP_RATIO_MASK) >> 8)

#define TV_DEC_RES_EXT_SET_PAL_SECAM_WSS_ASP_RATIO( ui4_ext, asp_ratio )      \
    ( (ui4_ext) = (((UINT32)(ui4_ext) & 0xFFFFF0FF) |                         \
                      (((UINT32)(asp_ratio<<8)) & TVD_DEC_EXT_PAL_SECAM_WSS_ASP_RATIO_MASK)) )

#define TV_DEC_RES_EXT_PAL_SECAM_WSS_4_3_FF                 ((UINT32)0x8)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_14_9_LB_CENTER         ((UINT32)0x1)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_14_9_LB_TOP            ((UINT32)0x2)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_16_9_LB_CENTER         ((UINT32)0xb)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_16_9_LB_TOP            ((UINT32)0x4)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_BIG_16_9_LB_CENTER     ((UINT32)0xd)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_4_3_FF_SP_14_9         ((UINT32)0xe)
#define TV_DEC_RES_EXT_PAL_SECAM_WSS_16_9_FF                ((UINT32)0x7)

typedef enum
{
    TV_DEC_COLOR_FMT_UNKNOWN = 0,
    TV_DEC_COLOR_FMT_VGA_4_4_4,
    TV_DEC_COLOR_FMT_YUV_4_2_2,
    TV_DEC_COLOR_FMT_YUV_4_4_4
}   TV_DEC_COLOR_FMT_T;

typedef enum
{
    TV_DEC_SCART_PIN_8_UNKNOWN = 0,
    TV_DEC_SCART_PIN_8_NO_SIGNAL,    
    TV_DEC_SCART_PIN_8_ASPECT_16_9,
    TV_DEC_SCART_PIN_8_ASPECT_4_3
}   TV_DEC_SCART_PIN_8_T;

/* HDMI AVI Info Frame Data Length */
#define TVD_HDMI_INFO_FRAME_LEN     (6)

typedef struct _TV_DEC_HDMI_AVI_INFOFRAME_INFO_T
{
    BOOL    b_valid; /* TRUE: The following is AVI info frame data. FALSE: no info frame data */
    UINT8   aui1_info_frame_data [TVD_HDMI_INFO_FRAME_LEN];
} TV_DEC_HDMI_AVI_INFOFRAME_INFO_T;

typedef enum
{
    TV_DEC_TAG3D_2D = 0,
    TV_DEC_TAG3D_MVC,          /* MVC = Multi-View Codec */
    TV_DEC_TAG3D_FP,           /* FP = Frame Packing */
    TV_DEC_TAG3D_FS,           /* FS = Frame Sequential */
    TV_DEC_TAG3D_TB,           /* TB = Top-and-Bottom */
    TV_DEC_TAG3D_SBS,          /* SBS = Side-by-Side */
    TV_DEC_TAG3D_REALD,
    TV_DEC_TAG3D_SENSIO,
    TV_DEC_TAG3D_LA,           /* LA = Line Alternative */
    TV_DEC_TAG3D_TTDO,         /* TTD only */
    TV_DEC_TAG3D_NOT_SUPPORT
}   TV_DEC_TAG3D_TYPE_T;

typedef struct _TV_DEC_RESOLUTION_INFO_T
{
    UINT16                     ui2_frame_rate;
    UINT16                     ui2_width;
    UINT16                     ui2_height;
    BOOL                       b_is_progressive;
    TV_DEC_SRC_ASPECT_RATIO_T  e_src_asp_ratio;
    TV_DEC_TIMING_TYPE_T       e_timing_type;
    TV_DEC_COLOR_FMT_T         e_color_fmt;
    TV_DEC_COLOR_FMT_T         e_src_color_space;
    TV_DEC_SCART_PIN_8_T       e_scart_pin_8;
    UINT32                     ui4_res_idx;
    UINT32                     ui4_ext;
    TV_DEC_HDMI_AVI_INFOFRAME_INFO_T t_hdmi_infoframe;
    TV_DEC_TAG3D_TYPE_T        e_tag3d_type;
}   TV_DEC_RESOLUTION_INFO_T;

/* SCART operation mode */
typedef enum
{
    TV_DEC_SCART_MODE_UNKNOWN = 0,
    TV_DEC_SCART_MODE_AUTO,
    TV_DEC_SCART_MODE_MIX,
    TV_DEC_SCART_MODE_COMPOSITE,
    TV_DEC_SCART_MODE_RGB,    
    TV_DEC_SCART_MODE_S_VIDEO,   
    TV_DEC_SCART_MODE_COMPONENT          
}   TV_DEC_SCART_MODE_T;

/* TV_DEC_SET_TYPE_VGA_TIMING_OPTION *****************************************/
typedef struct _TV_DEC_VGA_TIMING_OPTION_T
{
    UINT16                    ui2_opt_index;      /* [IN] option index in group */
    BOOL                      b_is_default;       /* [IN] is default? */
}   TV_DEC_VGA_TIMING_OPTION_T;

/* TV_DEC_GET_TYPE_AMBIG_VGA_TIMING ******************************************/
typedef enum 
{
    TV_DEC_AMBIG_VGA_TIMING_TYPE_NUM_IN_GRP,
    TV_DEC_AMBIG_VGA_TIMING_TYPE_OPTION_IN_GRP,
    TV_DEC_AMBIG_VGA_TIMING_TYPE_CURR_OPT_IN_GRP
} TV_DEC_AMBIG_VGA_TIMING_TYPE_T;

#define TV_DEC_VGA_TIMING_FREQUENCY_UNKNOWN       ((UINT16)0)

typedef struct _TV_DEC_AMBIG_VGA_TIMING_OPTION_T
{
    UINT16                    ui2_opt_index;      /* [IN/OUT] option index in group (1 ~ num_in_grp) */
    UINT16                    ui2_ver_frequncy;   /* [OUT] vertical frequency */
    UINT32                    ui4_hor_resolution; /* [OUT] horizontal resolution */
    UINT32                    ui4_ver_resolution; /* [OUT] vertical resolution */
}   TV_DEC_AMBIG_VGA_TIMING_OPTION_T;

typedef struct _TV_DEC_AMBIG_VGA_TIMING_INFO_T
{
    TV_DEC_AMBIG_VGA_TIMING_TYPE_T                e_type;  /* [IN] */

    union {
        UINT16                                    ui2_num; /* [OUT] */
        TV_DEC_AMBIG_VGA_TIMING_OPTION_T          t_opt;   /* [OUT] */
    } u;
}   TV_DEC_AMBIG_VGA_TIMING_INFO_T;

/* VGA detect setting*/
typedef enum
{
  TV_DEC_VGA_ENABLE_DETECTION,
  TV_DEC_VGA_DISABLE_DETECTION
} TV_DEC_VGA_DETECT_T;

/* VGA status */
typedef enum
{
  TV_DEC_VGA_SIGNAL_STABLE,
  TV_DEC_VGA_SIGNAL_NO_SIGNAL,
  TV_DEC_VGA_SIGNAL_NOT_SUPPORT
} TV_DEC_VGA_STATUS_T;

/* audio protect infomation */
typedef enum
{
    TV_DEC_AUD_PROTECT_INFO_DMUTE_AMUTE,         /* Digtal mute and analog mute */
    TV_DEC_AUD_PROTECT_INFO_DUNMUTE_AMUTE,       /* Digtal unmute and analog mute */
    TV_DEC_AUD_PROTECT_INFO_DUNMUTE_AUNMUTE,     /* Digtal unmute and analog unmute */
    TV_DEC_AUD_PROTECT_INFO_DMUTE_AUNMUTE        /* Digtal mute and analog unmute */
}   TV_DEC_AUD_PROTECT_INFO_T;

/* TV_DEC_GET_TYPE_VID_FREQ ********************************************************/
typedef struct _TV_DEC_VID_FREQ_T
{
    BOOL          b_valid;
    UINT16        ui2_v_freq;
    UINT16        ui2_h_freq;
}   TV_DEC_VID_FREQ_T;

typedef struct _TV_DEC_SOURCE_STATUS_T
{
    UINT8 ui2_av1_status;
    UINT8 ui2_av2_status;
    UINT8 ui2_ypbpr1_status;	
    UINT8 ui2_hdmi1_status;
    UINT8 ui2_hdmi2_status;
    UINT8 ui2_hdmi3_status;
    UINT8 ui2_hdmi4_status;
    UINT8 ui2_vga_status;
} _TV_DEC_SOURCE_STATUS_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _X_TV_DEC_H_ */

