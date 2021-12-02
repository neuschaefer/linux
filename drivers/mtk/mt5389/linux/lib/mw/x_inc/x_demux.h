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
 * $RCSfile: x_demux.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Demux driver specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_DEMUX_H_
#define _X_DEMUX_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_mm_common.h"
#include "x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Get operations */
#define DEMUX_GET_TYPE_PID          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  0))
#define DEMUX_GET_TYPE_STREAM_ID    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  1))
#define DEMUX_GET_TYPE_CTRL         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  3))
#define DEMUX_GET_TYPE_READ_BUFFER  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  6))
#define DEMUX_GET_TYPE_FILTER_LEN   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)  9))
#define DEMUX_GET_TYPE_SCRS         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 10))
#define DEMUX_GET_TYPE_STC          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 11))
#define DEMUX_GET_TYPE_SPEED        (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 12))
#define DEMUX_GET_TYPE_MEDIA_FORMAT (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 13))
#define DEMUX_GET_TYPE_BLOCK_PSI    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 14))
#define DEMUX_GET_TYPE_CURR_LBA     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 15))
#define DEMUX_GET_TYPE_CURR_TICK_NUM (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 16)) /* for Timeshift */
#define DEMUX_GET_TYPE_UOP_CAPABILITY    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 17))
#define DEMUX_GET_TYPE_POST_INFO         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 18))
#define DEMUX_GET_TYPE_STC_OFFSET   (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 19))

#define DEMUX_GET_TYPE_READ_BUFFER_NFY  (RM_GET_TYPE_ISR_CONTEXT | DEMUX_GET_TYPE_READ_BUFFER)

/* Set operations */
#define DEMUX_SET_TYPE_PID            ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  0)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_STREAM_ID      ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  1)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_NFY_FCT         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  2))
#define DEMUX_SET_TYPE_CTRL           ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  3)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_ALLOC_BUFFER   ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  4)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_FREE_BUFFER    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  5)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_UNLOCK_BUFFER   (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  7))
#define DEMUX_SET_TYPE_SECTION_FILTER  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  8))
#define DEMUX_SET_TYPE_FILTER_LEN      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T)  9))
#define DEMUX_SET_TYPE_DEST_ID         (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 10))
#define DEMUX_SET_TYPE_FLUSH_BUFFER    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 11))
#define DEMUX_SET_TYPE_SCRS_NFY_FCT    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 12))
#define DEMUX_SET_TYPE_MEDIA_FORMAT    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 13))
#define DEMUX_SET_TYPE_ADD_RANGE       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 14))
#define DEMUX_SET_TYPE_DEL_RANGE       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 15))
#define DEMUX_SET_TYPE_SPEED          ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 16)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_SEEK_POSTION    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 17))
#define DEMUX_SET_TYPE_BLOCK_PSI      ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 18)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_DMX_MODE       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 19)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_ES_WITH_PTS_IN ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 20)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_REC_BUF_RANGE   (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 21))
#define DEMUX_SET_TYPE_MONITOR_LBA    ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 22)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_DECRYPT_INFO    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 23))
#define DEMUX_SET_TYPE_CRYPT_CMD       (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 24))
#define DEMUX_SET_TYPE_PRIORITY        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 25))
#define DEMUX_SET_TYPE_AB_POSITION     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 26))
#define DEMUX_SET_TYPE_MM_AUTOTEST     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 27))
#define DEMUX_SET_TYPE_STC_ID         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 28)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_FREE_RUN       ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 29)) | RM_SET_TYPE_ARG_NO_REF)
#define DEMUX_SET_TYPE_3D_PROPERTY     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 30))
#define DEMUX_SET_TYPE_HDMVC_INFO     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 31)) | RM_SET_TYPE_ARG_NO_REF)

/* Flags used during registration */
#define DEMUX_FLAG_INCL_SECTION_LENGTH  ((UINT32) 0x00010000)
#define DEMUX_FLAG_NEG_PATTERN_MATCH    ((UINT32) 0x00020000)
#define DEMUX_FLAG_ALLOW_FILTER_OFFSET  ((UINT32) 0x00040000)


/*STC sould be looked up in bitstream*/
#define STC_STREAM_EMBEDDED ((STC_T)0xFFFFFFFF00000000)


/* Notify conditions */
typedef enum
{
    DEMUX_COND_DATA_REC = 0,
    DEMUX_COND_OVERFLOW,
    DEMUX_COND_SCRS
}   DEMUX_COND_T;

/* Scramble status */
typedef enum
{
    DEMUX_SCRS_UNKNOWN = 0,
    DEMUX_SCRS_CLEAR,
    DEMUX_SCRS_SCRAMBLED
}   DEMUX_SCRS_T;

typedef enum
{
    DEMUX_MODE_UNKNOWN = 0,
    DEMUX_MODE_DTV,
    DEMUX_MODE_MM,
    DEMUX_MODE_TIME_SHIFT
}   DEMUX_MODE_T;

/* DEMUX_GET_TYPE_UOP_CAPABILITY */
typedef enum
{
    DEMUX_UOP_UNKNOWN = 0,
    DEMUX_UOP_SEEK,
    DEMUX_UOP_TRICK
}   DEMUX_UOP_T;

typedef struct _DEMUX_UOP_CAP_INFO_T
{
    DEMUX_UOP_T  e_uop;
    BOOL         b_is_allowed;
    union
    {
        MM_SPEED_TYPE_T t_speed;
        /*to be extend*/
    } u;
}   DEMUX_UOP_CAP_INFO_T;

typedef enum
{
    DEMUX_POST_INFO_UNKNOWN = 0,
    DEMUX_POST_INFO_MEDIA_BTE_OFST,
    DEMUX_POST_INFO_MEDIA_TM_OFST,
    DEMUX_POST_INFO_MEDIA_RENDER_OFST,
    DEMUX_POST_INFO_RANGE_BTE_OFST,
    DEMUX_POST_INFO_RANGE_TM_OFST
} DEMUX_POST_INFO_TYPE_T;

typedef struct _RENDER_INFO_T
{
    UINT64  ui8_stc;
    UINT64  ui8_aud_pts_info;
    UINT64  ui8_aud_frame_position;
    UINT64  ui8_i_pts_info;
    UINT64  ui8_pts_info;
    UINT64  ui8_i_frame_position;
    UINT64  ui8_frame_position;
    UINT64  ui8_frame_position_disp;
    INT32   i4_temporal_reference;
    UINT16  ui2_decoding_order;
    UINT32  ui4_timestap;
}RENDER_INFO_T;

typedef struct _DEMUX_POST_INFO_T
{
    DEMUX_POST_INFO_TYPE_T  e_type;
    UINT64 ui8_pos_info;
    union
    {
        RENDER_INFO_T render_info;
    } u;
} DEMUX_POST_INFO_T;

/* Notify function */
typedef BOOL (*x_demux_nfy_fct) (VOID*         pv_nfy_tag,
                                 DEMUX_COND_T  e_nfy_cond,
                                 UINT32        ui4_data_1,
                                 UINT32        ui4_data_2
                        #ifndef MW_CLASSIC_DMX_NFY
                                 ,
                                 UINT64        ui8_ext_data_1,
                                 UINT64        ui8_ext_data_2,
                                 UINT32        ui4_ext_data_3
                        #endif
                                 );

/* Destination type and id info */
typedef struct _DEMUX_DEST_INFO_T
{
    DRV_TYPE_T  e_type;

    UINT16  ui2_id;
}   DEMUX_DEST_INFO_T;

/* Notify info setting */
typedef struct _DEMUX_NFY_INFO_T
{
    VOID*  pv_tag;

    x_demux_nfy_fct  pf_demux_nfy;
}   DEMUX_NFY_INFO_T;

/* Read demux buffer */
typedef struct _DEMUX_READ_BUFFER_INFO_T
{
    UINT8*  pui1_dest;

    VOID*  pv_src_tag;

    SIZE_T  z_skip_len;
    SIZE_T  z_copy_len;
}   DEMUX_READ_BUFFER_INFO_T;

/* Set section filter */
typedef struct _DEMUX_SECTION_FILTER_INFO_T
{
    UINT8  ui1_tbl_id_pattern;
    UINT8  ui1_tbl_id_mask;

    UINT8  ui1_pattern_offset;
    UINT8  ui1_pattern_len;
    UINT8*  pui1_pos_pattern;
    UINT8*  pui1_pos_mask;

    UINT8*  pui1_neg_pattern;
    UINT8*  pui1_neg_mask;

    BOOL  b_check_crc;
}   DEMUX_SECTION_FILTER_INFO_T;

/* Unlock buffer */
typedef struct _DEMUX_UNLOCK_BUFFER_INFO_T
{
    VOID*  pv_src_tag;

    SIZE_T  z_src_len;
}   DEMUX_UNLOCK_BUFFER_INFO_T;

/* STC value */
#define DEMUX_STC_FLAG_STABLE      ((UINT8) 0x01)
#define DEMUX_STC_FLAG_33_BIT_STC  ((UINT8) 0x02)

typedef struct _DEMUX_STC_INFO_T
{
    STC_T  t_stc;

    UINT8  ui1_flags;
}   DEMUX_STC_INFO_T;

/* DMX PES for multi-media */
typedef enum
{
    DMX_MM_PES_DIVX,
    DMX_MM_PES_MKV,
    DMX_MM_PES_PS,
    DMX_MM_PES_MAX
} DEMUX_MM_PES_FMT_T;

typedef struct
{
    UINT32 ui4_data_1;
    UINT32 ui4_data_2;
    UINT32 ui4_pts;
    UINT32 ui4_duration;
} DEMUX_MKV_PACK_T;

#endif /* _X_DEMUX_H_ */
