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
 * $RCSfile: x_vid_dec.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains video decoder driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_VID_DEC_H_
#define _X_VID_DEC_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "res_mngr/x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/* Get operations */
#define VID_DEC_GET_TYPE_CTRL               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define VID_DEC_GET_TYPE_STC                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define VID_DEC_GET_TYPE_RESOLUTION         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define VID_DEC_GET_TYPE_CAPABILITY         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define VID_DEC_GET_TYPE_PCR_ID             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define VID_DEC_GET_TYPE_CC_DATA            (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define VID_DEC_GET_TYPE_I_FRAME_SURFACE    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define VID_DEC_GET_TYPE_DEBLOCKING         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define VID_DEC_GET_TYPE_DECODE_STATUS      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define VID_DEC_GET_TYPE_SPEED              (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define VID_DEC_GET_TYPE_THUMBNAIL          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)10))
#define VID_DEC_GET_TYPE_ALLOC_I_FRAME_BUFF (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)11))
#define VID_DEC_GET_TYPE_STATUS             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)12))

/* Set operations */
#define VID_DEC_SET_TYPE_CTRL              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_NFY_FCT            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define VID_DEC_SET_TYPE_NFY_PTS            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define VID_DEC_SET_TYPE_INP_MODE           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define VID_DEC_SET_TYPE_PLAY_CC            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define VID_DEC_SET_TYPE_STOP_CC            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define VID_DEC_SET_TYPE_PCR_ID            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_CAPTURE            (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))
#define VID_DEC_SET_TYPE_ALLOC_CC_BUFF     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_FREE_CC_BUFF       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9))
#define VID_DEC_SET_TYPE_FLUSH_CC_BUFF      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)10))
#define VID_DEC_SET_TYPE_I_FRAME_BUFF       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)11))
#define VID_DEC_SET_TYPE_ALLOC_I_FRAME_BUFF (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)12))
#define VID_DEC_SET_TYPE_FREE_I_FRAME_BUFF  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)13))
#define VID_DEC_SET_TYPE_I_FRAME_DECODE     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)14))
#define VID_DEC_SET_TYPE_I_FRAME_NFY_FCT    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)15))
#define VID_DEC_SET_TYPE_DEBLOCKING         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)16))
#define VID_DEC_SET_TYPE_CODEC             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)17)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_SPEED             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)18)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_SEQUENCE_INFO      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)19))
#define VID_DEC_SET_TYPE_START_RENDER_INFO  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)20))
#define VID_DEC_SET_TYPE_FLUSH             ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)21)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_SUPER_FREEZE       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)22))
#define VID_DEC_SET_TYPE_RD3D_REG_DESC      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)23))
#define VID_DEC_SET_TYPE_MVC_EXT_DESC       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)24))
#define VID_DEC_SET_TYPE_NOT_DISPLAY        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)25)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_FLUSH_ES           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)26)) | RM_SET_TYPE_ARG_NO_REF)
#define VID_DEC_SET_TYPE_VDEC_REPLAY       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)27)) | RM_SET_TYPE_ARG_NO_REF)

/* Registration Flags */
#define VID_DEC_REG_FLAG_NONE                     ((UINT32) 0)
#define VID_DEC_REG_FLAG_USED_NORMAL_MODE         ((UINT32) 1)
#define VID_DEC_REG_FLAG_USED_IN_QUAD_MODE        ((UINT32) 2)


/* Notify conditions */
typedef enum
{
    VID_DEC_COND_ERROR = -1,
    VID_DEC_COND_CTRL_DONE,
    VID_DEC_COND_RESOLUTION_CHG,
    VID_DEC_COND_I_FRAME_SET_BUFF_DONE,
    VID_DEC_COND_I_FRAME_DECODE_DONE,
    VID_DEC_COND_DECODE_STATUS_CHG,
    VID_DEC_COND_SPEED_CHG,
    VID_DEC_COND_THUMBNAIL_DONE,
    VID_DEC_COND_BIT_RATE,
    VID_DEC_COND_PLAY_DONE,   /*notify MM/AP can trick, pause*/
    VID_DEC_COND_FRAMERATE_NOT_SUPPORT
}   VID_DEC_COND_T;

/* VID_DEC_GET_TYPE_DECODE_STATUS settings ***********************************/
typedef enum
{
    VID_DEC_DECODE_UNKNOWN = -1,
    VID_DEC_DECODE_NO_DATA,
    VID_DEC_DECODE_NORMAL,
    VID_DEC_DECODE_ERROR,
    VID_DEC_DECODE_HD_NOT_SUPPORT,
    VID_DEC_DECODE_CODEC_NOT_SUPPORT,
    VID_DEC_DECODE_CODEC_TYPE_CHG,
    VID_DEC_DECODE_DATA_RECEIVE, 
    VID_DEC_DECODE_RES_NOT_SUPPORT /* resolution not support */
}   VID_DEC_DECODE_STATUS_T;

/* VID_DEC_SET_TYPE_CODEC settings *******************************************/
typedef enum
{
    VID_DEC_CODEC_UNKNOWN = 0,
    VID_DEC_CODEC_MPEG_1,
    VID_DEC_CODEC_MPEG_2,
    VID_DEC_CODEC_MPEG_4,
    VID_DEC_CODEC_DIVX_311,
    VID_DEC_CODEC_DIVX_4,
    VID_DEC_CODEC_DIVX_5,
    VID_DEC_CODEC_XVID,
    VID_DEC_CODEC_WMV1,
    VID_DEC_CODEC_WMV2,
    VID_DEC_CODEC_WMV3,
    VID_DEC_CODEC_WVC1,
    VID_DEC_CODEC_H264,
    VID_DEC_CODEC_H263,
    VID_DEC_CODEC_MJPEG,
    VID_DEC_CODEC_RV8,
    VID_DEC_CODEC_RV9,
    VID_DEC_CODEC_AVS,
    VID_DEC_CODEC_NV12,
    VID_DEC_CODEC_VP8,
    VID_DEC_CODEC_VP6
}   VID_DEC_CODEC_T;

/* VID_DEC_SET_TYPE_CTRL settings ********************************************/
typedef enum
{
    VID_DEC_CTRL_RESET        = 0x00,
    VID_DEC_CTRL_STOP         = 0x01,
    VID_DEC_CTRL_FREEZE       = 0x02,
    VID_DEC_CTRL_PLAY         = 0x04,
    VID_DEC_CTRL_PLAY_I_FRAME = 0x08
}   VID_DEC_CTRL_T;

/* VID_DEC_SRC_ASPECT_RATIO settings **************************************/
typedef enum
{
    VID_DEC_SRC_ASPECT_RATIO_UNKNOWN = 0,
    VID_DEC_SRC_ASPECT_RATIO_4_3,         /* DAR 4:3 */
    VID_DEC_SRC_ASPECT_RATIO_16_9,        /* DAR 16:9 */
    VID_DEC_SRC_ASPECT_RATIO_2_21_1,      /* DAR 2.21:1 */
    VID_DEC_SRC_ASPECT_RATIO_PXL_10_11,   /* PAR 10:11 */
    VID_DEC_SRC_ASPECT_RATIO_PXL_40_33,   /* PAR 40:33 */
    VID_DEC_SRC_ASPECT_RATIO_PXL_16_11,   /* PAR 16:11 */
    VID_DEC_SRC_ASPECT_RATIO_PXL_12_11,   /* PAR 12:11 */
    VID_DEC_SRC_ASPECT_RATIO_PXL_3_2,     /* PAR 3:2 */
    VID_DEC_SRC_ASPECT_RATIO_PXL_1_1,     /* PAR 1:1 */
    VID_DEC_SRC_ASPECT_RATIO_USR          /* user */
} VID_DEC_SRC_ASPECT_RATIO_T;

/* Video source resolution descriptor */
typedef enum
{
    VID_DEC_TIMING_UNKNOWN = 0,
    VID_DEC_TIMING_VIDEO,
    VID_DEC_TIMING_GRAPHIC,
    VID_DEC_TIMING_NOT_SUPPORT
}   VID_DEC_TIMING_TYPE_T;

/*VID_DEC_TAG3D_TYPE settings **********************************************/
typedef enum
{
    VID_DEC_TAG3D_2D = 0,
    VID_DEC_TAG3D_MVC,          /* MVC = Multi-View Codec */
    VID_DEC_TAG3D_FP,           /* FP = Frame Packing */
    VID_DEC_TAG3D_FS,           /* FS = Frame Sequential */
    VID_DEC_TAG3D_TB,           /* TB = Top-and-Bottom */
    VID_DEC_TAG3D_SBS,          /* SBS = Side-by-Side */
    VID_DEC_TAG3D_REALD,
    VID_DEC_TAG3D_SENSIO,
    VID_DEC_TAG3D_LA,           /* LA = Line Alternative */
    VID_DEC_TAG3D_TTDO,         /* TTD only */
    VID_DEC_TAG3D_NOT_SUPPORT
}   VID_DEC_TAG3D_TYPE_T;

/* VID_DEC_RESOLUTION_INFO settings ****************************************/
typedef struct _VID_DEC_RESOLUTION_INFO_T
{
    UINT16                     ui2_width;
    UINT16                     ui2_height;
    UINT16                     ui2_frame_rate;
    UINT16                     ui2_afd;
    BOOL                       b_is_progressive;
    BOOL                       b_overscan_not_applied;
    VID_DEC_SRC_ASPECT_RATIO_T e_src_asp_ratio;
    VID_DEC_TIMING_TYPE_T      e_timing_type;
    VID_DEC_TAG3D_TYPE_T       e_tag3d_type;
    UINT32                     u4_aspect_ratio_w; 
    UINT32                     u4_aspect_ratio_h;
    BOOL                       b_is_display_aspect_ratio;
} VID_DEC_RESOLUTION_INFO_T;


typedef struct _VID_DEC_START_RENDER_INFO_T
{
    PTS_T                     t_pts;
    BOOL                      b_auto_pause;
} VID_DEC_START_RENDER_INFO_T;


/* VID_DEC_GET_TYPE_CAPABILITY settings **************************************/
typedef enum
{
    VID_DEC_FMT_UNKNOWN = 0,
    VID_DEC_FMT_480I,
    VID_DEC_FMT_480P,
    VID_DEC_FMT_576I,
    VID_DEC_FMT_576P,
    VID_DEC_FMT_720P,
    VID_DEC_FMT_1080I,
    VID_DEC_FMT_1080P
}   VID_DEC_FMT_T;

#define VID_DEC_CAP_NONE            ((UINT32) 0)
#define VID_DEC_CAP_FMT_480I        MAKE_BIT_MASK_32(1)
#define VID_DEC_CAP_FMT_480P        MAKE_BIT_MASK_32(2)
#define VID_DEC_CAP_FMT_576I        MAKE_BIT_MASK_32(3)
#define VID_DEC_CAP_FMT_576P        MAKE_BIT_MASK_32(4)
#define VID_DEC_CAP_FMT_720P        MAKE_BIT_MASK_32(5)
#define VID_DEC_CAP_FMT_1080I       MAKE_BIT_MASK_32(6)
#define VID_DEC_CAP_FMT_1080P       MAKE_BIT_MASK_32(7)
#define VID_DEC_CAP_DEBLOCK         MAKE_BIT_MASK_32(8)

#define VID_DEC_CAP_CODEC_UNKNOWN   ((UINT32) 0)
#define VID_DEC_CAP_CODEC_MPEG_1    MAKE_BIT_MASK_32(1)
#define VID_DEC_CAP_CODEC_MPEG_2    MAKE_BIT_MASK_32(2)
#define VID_DEC_CAP_CODEC_MPEG_4    MAKE_BIT_MASK_32(3)
#define VID_DEC_CAP_CODEC_DIVX_311  MAKE_BIT_MASK_32(4)
#define VID_DEC_CAP_CODEC_DIVX_4    MAKE_BIT_MASK_32(5)
#define VID_DEC_CAP_CODEC_DIVX_5    MAKE_BIT_MASK_32(6)
#define VID_DEC_CAP_CODEC_XVID      MAKE_BIT_MASK_32(7)
#define VID_DEC_CAP_CODEC_WMV1      MAKE_BIT_MASK_32(8)
#define VID_DEC_CAP_CODEC_WMV2      MAKE_BIT_MASK_32(9)
#define VID_DEC_CAP_CODEC_WMV3      MAKE_BIT_MASK_32(10)
#define VID_DEC_CAP_CODEC_WVC1      MAKE_BIT_MASK_32(11)
#define VID_DEC_CAP_CODEC_H264      MAKE_BIT_MASK_32(12)
#define VID_DEC_CAP_CODEC_H263      MAKE_BIT_MASK_32(13)
#define VID_DEC_CAP_CODEC_MJPEG     MAKE_BIT_MASK_32(14)
#define VID_DEC_CAP_CODEC_VP6       MAKE_BIT_MASK_32(15)
#define VID_DEC_CAP_CODEC_VP8       MAKE_BIT_MASK_32(16)

#define VID_DEC_STATUS_NORMAL       ((UINT32) 0)
#define VID_DEC_STATUS_DROP         MAKE_BIT_MASK_32(1)
#define VID_DEC_STATUS_SKIP         MAKE_BIT_MASK_32(2)
#define VID_DEC_STATUS_TIMEOUT      MAKE_BIT_MASK_32(3)

/* VID_DEC_CAPABILITY_INFO *****************************************************/
typedef struct _VID_DEC_CAPABILITY_INFO_T
{
    UINT32      ui4_fmt_timing;
    UINT32      ui4_codec_type;
    BOOL        b_3d_multi_track_available;
} VID_DEC_CAPABILITY_INFO_T;

/* VID_DEC_GET_TYPE_CC_DATA **************************************************/
typedef struct _VID_DEC_CC_DATA
{
    UCHAR*     puc_cc_data;    /* IN/OUT */
    SIZE_T     z_size;         /* IN/OUT */
    PTS_T      t_pts;          /* OUT */
    BOOL       b_is_scte_cc;   /* OUT */
    BOOL       b_even_first;   /* OUT */
} VID_DEC_CC_DATA;

/* VID_DEC_SET_TYPE_NFY_FCT settings *****************************************/
typedef VOID (*x_vid_dec_nfy_fct) (
    VOID*               pv_nfy_tag,
    VID_DEC_COND_T      e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2
    );

typedef struct _VID_DEC_NFY_INFO_T
{
    VOID*              pv_tag;
    x_vid_dec_nfy_fct  pf_vid_dec_nfy;
    VOID*              pv_previous_tag;          /* OUTPUT */
    x_vid_dec_nfy_fct  pf_previous_vid_dec_nfy;  /* OUTPUT */
}   VID_DEC_NFY_INFO_T;


/* VID_DEC_SET_TYPE_PLAY_CC settings *****************************************/
/* CC notify conditions */
typedef enum
{
    VID_DEC_CC_COND_ERROR = -1,
    VID_DEC_CC_COND_DATA_ARRIVAL
}   VID_DEC_CC_COND_T;

/* error codes used in VID_DEC_CC_COND_ERROR */
#define VID_DEC_CC_ERR_BUFF_OVER_FLOW       ((UINT32) 1)

typedef VOID (*x_vid_dec_cc_nfy_fct) (
    VOID*                   pv_tag,
    VID_DEC_CC_COND_T       e_cc_cond,
    VOID*                   pv_arg
    );

typedef struct _VID_DEC_CC_NFY_INFO_T
{
    VOID*                       pv_tag;
    x_vid_dec_cc_nfy_fct        pf_cc_nfy;
}   VID_DEC_CC_NFY_INFO_T;


/* VID_DEC_SET_TYPE_NFY_PTS settings *****************************************/
typedef VOID (*x_vid_dec_pts_reach_nfy_fct) (
    VOID*               pv_nfy_tag,
    PTS_T               t_pts,
    BOOL                b_out_of_sync
    );

typedef struct _VID_DEC_NFY_PTS_INFO_T
{
    PTS_T                         t_pts;
    VOID*                         pv_tag;
    x_vid_dec_pts_reach_nfy_fct   pf_vid_dec_pts_reach_nfy;
}   VID_DEC_NFY_PTS_INFO_T;


/* VID_DEC_SET_TYPE_INP_MODE settings ****************************************/
typedef enum
{
    VID_DEC_INP_MODE_NORMAL,
    VID_DEC_INP_MODE_PULL
}   VID_DEC_INP_MODE_T;

typedef INT32 (*x_vid_dec_data_acquire_fct)(
    VOID*            pv_tag,
    UCHAR**          ppc_buff,
    SIZE_T*          pz_buff_size
    );

typedef VOID (*x_vid_dec_data_release_fct)(
    VOID*            pv_tag,
    UCHAR*           pc_buff,
    SIZE_T           z_buff_size
    );

typedef struct _VID_DEC_INP_MODE_INFO_T
{
    VID_DEC_INP_MODE_T           e_inp_mode;
    VOID*                        pv_tag;                      /* pull mode only */
    x_vid_dec_data_acquire_fct   pf_vid_dec_data_acquire_nfy; /* pull mode only */
    x_vid_dec_data_release_fct   pf_vid_dec_data_release_nfy; /* pull mode only */
}   VID_DEC_INP_MODE_INFO_T;


/* VID_DEC_SET_TYPE_CAPTURE settings ****************************************/
typedef struct _VID_DEC_CAPTURE_INFO_T
{
    UCHAR*                    pc_frame_buff;
    SIZE_T                    z_buff_size;
    BOOL                      b_is_progressive;
}   VID_DEC_CAPTURE_INFO_T;


/* VID_DEC_SET_TYPE_I_FRAME_BUFF settings ***********************************/
/* VID_DEC_SET_TYPE_ALLOC_I_FRAME_BUFF **************************************/
/* VID_DEC_SET_TYPE_FREE_I_FRAME_BUFF ***************************************/
typedef struct _VID_DEC_I_FRAME_BUFF_T
{
    UCHAR*                    pc_frame_buff_sp;
    SIZE_T                    z_frame_size;
}   VID_DEC_I_FRAME_BUFF_T;

/* VID_DEC_GET_TYPE_I_FRAME_SURFACE *****************************************/
typedef struct _VID_DEC_GET_I_FRAME_SURFACE_T
{
    UCHAR*                     pc_y_buf;
    UINT32                     ui4_y_pitch;
    UCHAR*                     pc_c_buf;
    UINT32                     ui4_c_pitch;
}   VID_DEC_GET_I_FRAME_SURFACE_T;

/* VID_DEC_GET_TYPE_DEBLOCKING/VID_DEC_SET_TYPE_DEBLOCKING ******************/
typedef struct _VID_DEC_REGION_T
{
    UINT16        ui4_x;
    UINT16        ui4_y;
    UINT16        ui4_h;
    UINT16        ui4_w;    
} VID_DEC_REGION_T;

typedef struct _VID_DEC_DEBLOCKING_INFO_T
{
    BOOL                       b_is_on;
    
    union
    {
        VID_DEC_REGION_T       t_region;
    } u;
}   VID_DEC_DEBLOCKING_INFO_T;

/* VID_DEC_SET_TYPE_SEQUENCE_INFO *******************************************/
#define VID_DEC_SRC_WIDTH_UNKNOWN                 ((UINT16) 0xFFFF)
#define VID_DEC_SRC_HEIGHT_UNKNOWN                ((UINT16) 0xFFFF)

typedef enum
{
    VID_DEC_SRC_FRAME_RATE_UNKNOWN = 0,
    VID_DEC_SRC_FRAME_RATE_23_976, /* 24000/1001 (23.976...) */
    VID_DEC_SRC_FRAME_RATE_24,
    VID_DEC_SRC_FRAME_RATE_25,
    VID_DEC_SRC_FRAME_RATE_29_97, /* 30000/1001 (29.97...) */
    VID_DEC_SRC_FRAME_RATE_30,
    VID_DEC_SRC_FRAME_RATE_50,
    VID_DEC_SRC_FRAME_RATE_59_94, /* 60000/1001 (59.94...) */
    VID_DEC_SRC_FRAME_RATE_60,
    VID_DEC_SRC_FRAME_RATE_120,
    VID_DEC_SRC_FRAME_RATE_1,
    VID_DEC_SRC_FRAME_RATE_5,
    VID_DEC_SRC_FRAME_RATE_8,
    VID_DEC_SRC_FRAME_RATE_10,
    VID_DEC_SRC_FRAME_RATE_12,
    VID_DEC_SRC_FRAME_RATE_15,
    VID_DEC_SRC_FRAME_RATE_2,
    VID_DEC_SRC_FRAME_RATE_6,
    VID_DEC_SRC_FRAME_RATE_48,
    VID_DEC_SRC_FRAME_RATE_70,
    VID_DEC_SRC_FRAME_RATE_20,
    VID_DEC_SRC_FRAME_RATE_VARIABLE
} VID_DEC_SRC_FRAME_RATE_T;

typedef enum
{
    VID_DEC_ASP_USR_TYPE_SRC = 1, /* source aspect ratio */
    VID_DEC_ASP_USR_TYPE_DISP     /* display aspect ratio */
} VID_DEC_ASP_USR_TYPE_T;

typedef struct _VID_DEC_SEQUENCE_DATA_T
{
    UINT16                     ui2_width;      /* horizontal_size_value */
    UINT16                     ui2_height;     /* vertical_size_value */
    VID_DEC_SRC_FRAME_RATE_T   e_frame_rate;   /* Frame Rate */
    VID_DEC_SRC_ASPECT_RATIO_T e_src_asp;      /* Display Aspect Ratio (DAR) */
    VID_DEC_ASP_USR_TYPE_T     e_asp_usr_type; /* source or display asp */
    UINT16                     ui2_asp_usr_h;
    UINT16                     ui2_asp_usr_w;    
    UINT32                     ui4_seq_hdr;
    UINT16                     ui2_seq_hdr_sz;     
    BOOL                       b_chk_framerate;
} VID_DEC_SEQUENCE_DATA_T;

typedef struct _VID_DEC_SEQUENCE_RAW_T
{
    UCHAR*                     pc_buff;
    SIZE_T                     z_buff_size;
    VID_DEC_SRC_FRAME_RATE_T   e_frame_rate;   /* Frame Rate */
} VID_DEC_SEQUENCE_RAW_T;

typedef struct _VID_DEC_SEQUENCE_INFO_T
{
    BOOL                       b_is_raw;
    BOOL                       b_is_second_raw;

    union
    {
        VID_DEC_SEQUENCE_RAW_T     t_seq_raw;
        VID_DEC_SEQUENCE_DATA_T    t_seq_data;
    } u;
#if SUPPORT_MULTI_INFO
    struct _VID_DEC_SEQUENCE_INFO_T*  pt_next_scale;
#endif
} VID_DEC_SEQUENCE_INFO_T;

/* VID_DEC_SET_TYPE_SUPER_FREEZE **********************************************/
typedef enum
{
    VID_DEC_SUPER_FREEZE_TYPE_MEMO = 0,
    VID_DEC_SUPER_FREEZE_TYPE_FORCE    /* for super freeze */
}   VID_DEC_SUPER_FREEZE_TYPE_T;

typedef struct _VID_DEC_SUPER_FREEZE_T
{
    VID_DEC_SUPER_FREEZE_TYPE_T e_type;
    BOOL                        b_enable;
}   VID_DEC_SUPER_FREEZE_T;


/* VID_DEC_GET_TYPE_THUMBNAIL ***********************************************/
/* color mode */
typedef enum
{
    VID_DEC_COLORMODE_AYUV_CLUT2     = 0,
    VID_DEC_COLORMODE_AYUV_CLUT4     = 1,
    VID_DEC_COLORMODE_AYUV_CLUT8     = 2,
    VID_DEC_COLORMODE_UYVY_16        = 3,
    VID_DEC_COLORMODE_YUYV_16        = 4,
    VID_DEC_COLORMODE_AYUV_D8888     = 5,
    VID_DEC_COLORMODE_ARGB_CLUT2     = 6,
    VID_DEC_COLORMODE_ARGB_CLUT4     = 7,
    VID_DEC_COLORMODE_ARGB_CLUT8     = 8,
    VID_DEC_COLORMODE_RGB_D565       = 9,
    VID_DEC_COLORMODE_ARGB_D1555     = 10,
    VID_DEC_COLORMODE_ARGB_D4444     = 11,
    VID_DEC_COLORMODE_ARGB_D8888     = 12,
    VID_DEC_COLORMODE_YUV_420_BLK    = 13,
    VID_DEC_COLORMODE_YUV_420_RS     = 14,
    VID_DEC_COLORMODE_YUV_422_BLK    = 15,
    VID_DEC_COLORMODE_YUV_422_RS     = 16,
    VID_DEC_COLORMODE_YUV_444_BLK    = 17,
    VID_DEC_COLORMODE_YUV_444_RS     = 18
} VID_DEC_COLORMODE_T;

typedef struct _VID_DEC_THUMBNAIL_INFO_T
{
    UCHAR*                    pc_canvas_buffer;
    VID_DEC_COLORMODE_T       e_canvas_colormode;
    UINT32                    ui4_canvas_width;     /* in pixels */
    UINT32                    ui4_canvas_height;    /* in pixels */
    UINT32                    ui4_canvas_pitch;     /* in bytes */

    UINT32                    ui4_thumbnail_x;      /* in pixels */
    UINT32                    ui4_thumbnail_y;      /* in pixels */
    UINT32                    ui4_thumbnail_width;  /* in pixels */
    UINT32                    ui4_thumbnail_height; /* in pixels */
}   VID_DEC_THUMBNAIL_INFO_T;

typedef struct _VID_DEC_BOOT_LOGO_SET_T
{
    BOOL    b_default;      /* enable default boot logo */
    BOOL    b_onoff;        /* on/off the vdec boot logo */
    UINT8   c_data_select;  /* select the number for boot */
    UINT32  ui4_valid;      /* valid for each vdec boot logo address */
} VID_DEC_BOOT_LOGO_SET_T;

typedef struct _VID_DEC_BOOT_LOGO_GET_T
{
    BOOL     b_default;      /* enable default boot logo */
    BOOL     b_default_exist;/* is default boot logo exist?*/
    BOOL     b_onoff;        /* on/off the vdec boot logo */
    UINT8    c_data_select;  /* select the number for boot */
    UINT8    c_slot_num;     /* slot number */
    UINT32   ui4_valid;      /* valid for each vdec boot logo address */
    UINT32   ui4_flash_addr; /* flash start address */
    UINT32   ui4_flash_size; /* flash size */
} VID_DEC_BOOT_LOGO_GET_T;

typedef struct _VID_DEC_BOOT_LOGO_ADDR_SET_T
{
    UINT8   c_data_type;       /* data type, one of VID_DEC_CODEC_T */
    UINT32  ui4_data_size;     /* data size */
    UINT32  ui4_addr;          /* the address of this vdec boot logo */
} VID_DEC_BOOT_LOGO_ADDR_SET_T;

typedef struct _VID_DEC_BOOT_LOGO_ADDR_GET_T
{
    UINT8    c_data_type;        /* data type, one of VID_DEC_CODEC_T */
    UINT32   ui4_data_size;      /* data size */
    UINT32   ui4_addr;           /* the address of this vdec boot logo */
} VID_DEC_BOOT_LOGO_ADDR_GET_T;

/* for 3D video */
typedef UINT8 VD_REALD_LAYER_TYPE_T;

#define VD_REALD_BASE_LAYER        ((VD_REALD_LAYER_TYPE_T)0x1)
#define VD_REALD_ENHANCEMENT_LAYER ((VD_REALD_LAYER_TYPE_T)0x2)


typedef UINT8 VD_REALD_CODING_METHOD_T;

#define VD_CRITICALLY_SAMPLED_METHOD        ((VD_REALD_CODING_METHOD_T)0x00)
#define VD_NON_CRITICALLY_SAMPLED_METHOD    ((VD_REALD_CODING_METHOD_T)0x80)

/* RD3D_registration descriptor */
typedef struct _VID_DEC_RD3D_REG_DESC_T
{
    VD_REALD_LAYER_TYPE_T      e_layer_type;
    VD_REALD_CODING_METHOD_T   e_coding_method;    
} VID_DEC_RD3D_REG_DESC_T;

/* MVC extension descriptor */
typedef struct _VID_DEC_MVC_EXT_DESC_T
{
    UINT16  ui2_avg_bit_rate;
    UINT16  ui2_max_bit_rate;
    UINT16  ui2_view_ord_idx_min;
    UINT16  ui2_view_ord_idx_max;
    UINT8   ui1_temporal_id_start;
    UINT8   ui1_temporal_id_end;
    BOOL    b_no_sei_nal_present;
    BOOL    b_no_prefix_nal_present;
} VID_DEC_MVC_EXT_DESC_T;


#endif /* _X_VID_DEC_H_ */
