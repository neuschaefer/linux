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
 * $RCSfile: x_mm_common.h,v $
 * $Revision: #2 $
 * $Date: 2012/06/28 $
 * $Author: wengao.han $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/8 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: 7c379a8f7b446dcae5e6d2f08e190216 $
 *
 * Description: 
 *         This header file contains type definitions common to the whole
 *         Middleware for multmedia playback.
 *---------------------------------------------------------------------------*/

#ifndef _X_MM_COMMON_H_
#define _X_MM_COMMON_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
 
#include "inc/u_mm_common.h"
#include "res_mngr/drv/x_drm_drv.h"
#include "strm_mngr/audio_hdlr/u_sm_audio_hdlr.h"
#include "handle/u_handle.h"

#else

#include "u_mm_common.h"
#include "x_drm_drv.h"
#include "u_handle.h"
#include "strm_mngr/audio_hdlr/u_sm_audio_hdlr.h"
#endif
/*
 *  lseek starting position
 
 *          ui1_whence
 *              MEDIA_SEEK_BGN tells Feeder to move current position to
 *              i8_offset from the begging of the media. if i8_offset is
 *              negative, current position is set to 0, that is the begging
 *              of media.
 *              MEDIA_SEEK_CUR tells Feeder to move current position to
 *              i8_offset from current position.
 *              MEDIA_SEEK_END tells Feeder to move current position to
 *              i8_offset from the end of the media. If i8_offset is a
 *              positive number, current position is set to the size of
 *              the media, that is the end of media.
*/
#define MEDIA_SEEK_BGN             ((UINT8) 1)
#define MEDIA_SEEK_CUR             ((UINT8) 2)
#define MEDIA_SEEK_END             ((UINT8) 3)
#define MEDIA_SEEK_CHK             ((UINT8) 0x80)

#define NW_FEEDER_CACHE_SIZE       (32*1024)
#define MULTI_CACHE_COUNT          (2)
 
/************************************************************************/
/* Feeder Error Code                                                    */
/************************************************************************/
#define MFDRR_EOF              ((INT32)  2)
#define MFDRR_NOT_ENOUGH_BUF   ((INT32)  1)
#define MFDRR_OK               ((INT32)  0)
#define MFDRR_FAIL             ((INT32) -1)


/*Multimedia Feeder Definition*/
typedef INT32 (*feeder_copybytes_fct)   (VOID*      pv_feeder_obj, 
                                         VOID*      pv_buf, 
                                         SIZE_T     z_buf_leng, 
                                         SIZE_T     z_copy_size,
                                         UINT32*    pui4_size);
typedef INT32 (*feeder_input4bytes_fct) (VOID* pv_feeder_obj, UINT32* pui4_data);
typedef INT32 (*feeder_input3bytes_fct) (VOID* pv_feeder_obj, UINT32* pui4_data);
typedef INT32 (*feeder_input2bytes_fct) (VOID* pv_feeder_obj, UINT16* pui2_data);
typedef INT32 (*feeder_input1bytes_fct) (VOID* pv_feeder_obj, UINT8*  pui1_data);
typedef INT32 (*feeder_setpos_fct)      (VOID* pv_feeder_obj, 
                                         INT64 i8_offset, 
                                         UINT8 ui1_whence);
typedef INT32 (*feeder_getpos_fct)      (VOID* pv_feeder_obj, UINT64* pui8_cur_pos);

typedef struct _FEEDER_SRC_INPUT_FCT_TBL_T
{
    feeder_copybytes_fct       pf_copybytes;
    feeder_input4bytes_fct     pf_input4;
    feeder_input3bytes_fct     pf_input3;
    feeder_input2bytes_fct     pf_input2;
    feeder_input1bytes_fct     pf_input1;
    feeder_setpos_fct          pf_set_pos;
    feeder_getpos_fct          pf_get_pos;
} FEEDER_SRC_INPUT_FCT_TBL_T;

typedef struct _MEDIA_FEEDER_T
{
  UINT32                     h_feeder;
  FEEDER_SRC_INPUT_FCT_TBL_T t_feeder_le_fct_tbl; /*in case of Little Endian*/
  FEEDER_SRC_INPUT_FCT_TBL_T t_feeder_be_fct_tbl;/* in case of Big Endian*/
  BOOL                       b_canceled;
} MEDIA_FEEDER_T;


typedef enum
{
    MM_PLAYBACK_CTRL_PULL_ABORT_FAIL            =   -2,     /* Pull Model Read Abort Fail */
    MM_PLAYBACK_CTRL_PULL_FAIL                  =   -1,     /* Pull Model Read Fail */
    MM_PLAYBACK_CTRL_PULL_READ_OK               =   0,      /* Pull Model Read OK */
    MM_PLAYBACK_CTRL_PULL_ABORT_OK              =   1,      /* Pull Model Read Abort OK */ 
    MM_PLAYBACK_CTRL_PULL_READ_EOS              =   2       /* Pull Model Read EOS */
} MM_PLAYBACK_CTRL_PULL_EVENT_T;

typedef INT32 (*x_mm_playback_pull_nfy_fct)(
                                            MM_PLAYBACK_CTRL_PULL_EVENT_T  e_event,
                                            VOID*                          pv_tag,
                                            HANDLE_T                       h_req,
                                            UINT32                         ui4_data);

typedef INT32 (*x_mm_playback_pull_open_fct)(
                                            HANDLE_T    h_pb,
                                            HANDLE_T*   ph_pull,
                                            VOID*       pv_app_tag);

typedef INT32 (*x_mm_playback_pull_associate_fct)(
                                            HANDLE_T    h_pull,
                                            VOID*       pv_app_tag,
                                            UINT32      ui4_phy_addr,
                                            UINT32      ui4_size);

typedef INT32 (*x_mm_playback_pull_dissociate_fct)(
                                            HANDLE_T    h_pull,
                                            VOID*       pv_app_tag);

typedef INT32 (*x_mm_playback_pull_close_fct)(HANDLE_T  h_pull,
                                              VOID*     pv_app_tag);


typedef INT32 (*x_mm_playback_pull_read_fct)(
                                            HANDLE_T  h_pull,
                                            VOID*     pv_app_tag,
                                            UINT8*    pu1_dst,
                                            UINT32    ui4_count,                                           
                                            UINT32*   pui4_read);

typedef INT32 (*x_mm_playback_pull_read_async_fct)(
                                            HANDLE_T                    h_pull,
                                            VOID*                       pv_app_tag,
                                            UINT8*                      pu1_dst,
                                            UINT32                      ui4_count,
                                            x_mm_playback_pull_nfy_fct  pf_nfy,
                                            VOID*                       pv_tag,
                                            HANDLE_T*                   ph_req);

typedef INT32 (*x_mm_playback_pull_abort_read_async_fct)(
                                            HANDLE_T            h_pull,
                                            VOID*               pv_app_tag,
                                            HANDLE_T            h_req);

typedef INT32 (*x_mm_playback_pull_byteseek_fct)(
                                            HANDLE_T  h_pull,
                                            VOID*     pv_app_tag,
                                            UINT64    u8SeekPos,
                                            UINT8     u1Whence,
                                            UINT64*   pu8CurPos);

typedef INT32 (*x_mm_playback_pull_get_input_lengh_fct)(
                                            HANDLE_T  h_pull,
                                            VOID*     pv_app_tag,
                                            UINT64*   pui8_len);


typedef struct _MM_PB_CTRL_PULL_INFO_T
{
    HANDLE_T                                    h_pb;
    UINT32                                         ui4_svc_id;   /*reserved for client process id assignment*/
    VOID*                                       pv_app_tag;
    x_mm_playback_pull_open_fct                 pf_open;
    x_mm_playback_pull_associate_fct            pf_associate;
    x_mm_playback_pull_dissociate_fct           pf_dissociate;
    x_mm_playback_pull_close_fct                pf_close;
    x_mm_playback_pull_read_fct                 pf_read;
    x_mm_playback_pull_read_async_fct           pf_read_async;
    x_mm_playback_pull_abort_read_async_fct     pf_abort_read_async;
    x_mm_playback_pull_byteseek_fct             pf_byteseek;
    x_mm_playback_pull_get_input_lengh_fct      pf_get_input_len;     

    /*add for range info*/
    UINT64                                      ui8_preroll;
    UINT32                                      ui4_max_acces_unt;
    UINT32                                      ui4_min_acces_unt;
    UINT64                                      ui8_media_data_ofst;
    UINT64                                      ui8_media_data_size;
    UINT64                                      ui8_media_duration;
} MM_PB_CTRL_PULL_INFO_T;

typedef enum
{
    MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_CONSUMED    =   1,
    MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_OVERFLOW    =   2,
    MM_PLAYBACK_CTRL_PUSH_EVENT_BUF_UNDRFLOW    =   3
} MM_PLAYBACK_CTRL_PUSH_EVENT_T;

typedef VOID (*x_mm_playback_push_nfy_fct)(
                                            MM_PLAYBACK_CTRL_PUSH_EVENT_T  e_event,
                                            VOID*                                                pv_tag,
                                            UINT32                                               ui4_data);


typedef struct 
{
    HANDLE_T                    h_mem_part;
    VOID*                       pv_buffer;
    VOID*                       pv_private;
    VOID*                       pv_private2;
    SIZE_T                      z_buffer_size;
} MM_PB_CTRL_PUSH_BUF_INFO_T;


typedef struct 
{
    HANDLE_T                    hHandle;
    UINT32                      ui4_svc_id;   /*NO USE NOW. reserved for client process id assignment*/
    UINT64                      ui8_preroll;
    x_mm_playback_push_nfy_fct  pf_nfy;
} MM_PB_CTRL_PUSH_MODE_INFO_T;

typedef enum
{
    MEDIA_FEEDER_NW_TYPE_URI    =   1,
    MEDIA_FEEDER_NW_TYPE_PULL
} MEDIA_FEEDER_NW_TYPE_T;

typedef struct _MEDIA_MULTI_CACHE_T MEDIA_MULTI_CACHE_T;
struct _MEDIA_MULTI_CACHE_T
{
  UINT64                        ui8_cache_pos;
  UINT32                        ui4_read_count;
  UINT8                         aui1_cache_buf[NW_FEEDER_CACHE_SIZE+1024];
  MEDIA_MULTI_CACHE_T           *pt_next;
};

typedef struct _MEDIA_FEEDER_NW_T
{
  MEDIA_FEEDER_NW_TYPE_T        e_type;
  UINT32                        h_feeder;
  FEEDER_SRC_INPUT_FCT_TBL_T    t_feeder_le_fct_tbl; /*in case of Little Endian*/
  FEEDER_SRC_INPUT_FCT_TBL_T    t_feeder_be_fct_tbl;/* in case of Big Endian*/
  MM_PB_CTRL_PULL_INFO_T        t_pull_read_fct_tbl;
  
  UINT64                        ui8_file_size;
  UINT64                        ui8_pos;
  UINT64                        ui8_cache_pos;
  UINT32                        ui4_read_count;
  UINT8                         aui1_cache_buf[NW_FEEDER_CACHE_SIZE+1024];
  MEDIA_MULTI_CACHE_T           at_multi_cache[MULTI_CACHE_COUNT];
  MEDIA_MULTI_CACHE_T           *pt_free_cache;
  MEDIA_MULTI_CACHE_T           *pt_used_cache;
  UINT8*                        aui1_phy_buf;
  UINT8*                        aui1_share_buf;
  HANDLE_T                      h_pull; /*used for pull mode only*/
  VOID*                         pv_app_tag; /*used for pull mode only*/
  BOOL                          b_seekable_checked;
  BOOL                          b_canceled;
} MEDIA_FEEDER_NW_T;


/***********  Media Rabge Control Condition ***********************/
typedef UINT32 MM_EVENT_TYPE_T;
#define MM_MINFO_EVENT            ((MM_EVENT_TYPE_T)  0)
#define MM_MIDXBULD_EVENT         ((MM_EVENT_TYPE_T)  1)
#define MM_MRANGE_CTRL_EVENT      ((MM_EVENT_TYPE_T)  2)


typedef UINT32 MM_RANGE_CTRL_EVENT_T;
#define MM_RANGE_CTRL_ADD      ((MM_RANGE_CTRL_EVENT_T)  0)
#define MM_RANGE_CTRL_DEL      ((MM_RANGE_CTRL_EVENT_T)  1)
#define MM_RANGE_CTRL_FLUSH    ((MM_RANGE_CTRL_EVENT_T)  2)
#define MM_RANGE_CTRL_BUILD    ((MM_RANGE_CTRL_EVENT_T)  3)
/***********  Mutimedia Stream ID Definition ***********************/
#define NULL_STREAM_ID              0

/*Stream ID Type*/
typedef enum
{
    STREAM_ID_TYPE_UNKNOWN = 0,
    STREAM_ID_TYPE_AVI,
    STREAM_ID_TYPE_ASF,
    STREAM_ID_TYPE_MP4,
    STREAM_ID_TYPE_MKV,
    STREAM_ID_TYPE_RM,
    STREAM_ID_TYPE_AIF,
    STREAM_ID_TYPE_PS,
    STREAM_ID_TYPE_TS,
    STREAM_ID_TYPE_ES,
    STREAM_ID_TYPE_FLV, /*add for flv*/
    STREAM_ID_TYPE_OGG
} STREAM_ID_TYPE_T;

typedef UINT32 STREAM_ID_AVI_T;
typedef UINT32 STREAM_ID_ASF_T;
typedef UINT32 STREAM_ID_MP4_T;
typedef UINT32 STREAM_ID_MKV_T;
typedef UINT32 STREAM_ID_ES_T;
typedef UINT32 STREAM_ID_RM_T;
typedef UINT32 STREAM_ID_AIF_T;
typedef UINT32 STREAM_ID_FLV_T;
typedef UINT32 STREAM_ID_OGG_T;

typedef struct _STREAM_ID_PS_T
{
    UINT16 ui2_stm_id;
    UINT16 ui2_sub_stm_id;
} STREAM_ID_PS_T;

typedef MPEG_2_PID_T STREAM_ID_TS_T;

typedef struct _STREAM_ID_T
{
  STREAM_ID_TYPE_T  e_type;
  union
  {
    STREAM_ID_AVI_T  t_stm_avi_id;
    STREAM_ID_ASF_T  t_stm_asf_id;
    STREAM_ID_MP4_T  t_stm_mp4_id;
    STREAM_ID_MKV_T  t_stm_mkv_id;
    STREAM_ID_PS_T   t_stm_ps_id;
    STREAM_ID_TS_T   t_stm_ts_id;
    STREAM_ID_ES_T   t_stm_es_id;
    STREAM_ID_RM_T   t_stm_rm_id;
    STREAM_ID_AIF_T   t_stm_aif_id;
    STREAM_ID_FLV_T  t_stm_flv_id;
    STREAM_ID_OGG_T  t_stm_ogg_id;
  }u;
} STREAM_ID_T;

/***********  PVR Metatdata Definition ***********************/
#define PVR_PIC_INFO_SET_LBA_OFFSET(_pt_pic_info, _i4_lba_ofs)          \
        {                                                               \
            UINT32  ui4_abs_lba_ofs = (_i4_lba_ofs) >= 0 ? (UINT32) (_i4_lba_ofs) : (UINT32) (-(_i4_lba_ofs)); \
                                                                        \
            (_pt_pic_info)->aui1_data[0] = ((UINT8) ui4_abs_lba_ofs);   \
            (_pt_pic_info)->aui1_data[1] = ((UINT8) ((((UINT32) ui4_abs_lba_ofs) & 0x0000FF00) >>  8));  \
            (_pt_pic_info)->aui1_data[2] = ((UINT8) ((((UINT32) ui4_abs_lba_ofs) & 0x00FF0000) >> 16));  \
            (_pt_pic_info)->aui1_data[3] = ((UINT8) (((_pt_pic_info)->aui1_data[3] & 0xFE) |             \
                                                    ((UINT8) (((UINT32) ((_i4_lba_ofs) & 0x80000000)) >> 31)))); \
        }

#define PVR_PIC_INFO_GET_LBA_OFFSET(_pt_pic_info, _pi4_lba_ofs)         \
        {                                                               \
            UINT32  ui4_abs_lba_ofs = (((UINT32) (_pt_pic_info)->aui1_data[0])        |     \
                                      (((UINT32) (_pt_pic_info)->aui1_data[1]) << 8)  |     \
                                      (((UINT32) (_pt_pic_info)->aui1_data[2]) << 16) );    \
            *(_pi4_lba_ofs) = ((_pt_pic_info)->aui1_data[3] & 0x01) ? (-((INT32) ui4_abs_lba_ofs)) : ((INT32) ui4_abs_lba_ofs); \
        }

#define PVR_PIC_INFO_SET_PIC_TYPE(_pt_pic_info, _e_pic_type)            \
            (_pt_pic_info)->aui1_data[3] = (((((UINT8) (_e_pic_type)) & ((UINT8) 0x03)) << 6) | \
                                           ((_pt_pic_info)->aui1_data[3] & ((UINT8) 0x3F)))

#define PVR_PIC_INFO_GET_PIC_TYPE(_pt_pic_info)                         \
            ((UINT8) (((_pt_pic_info)->aui1_data[3] & ((UINT8) 0xC0)) >> 6))
            
#define PVR_PIC_INFO_SET_PIC_BLKS(_pt_pic_info, _ui4_pic_blks)          \
        {                                                               \
            (_pt_pic_info)->aui1_data[4] = ((UINT8) (_ui4_pic_blks));   \
            (_pt_pic_info)->aui1_data[5] = (UINT8) ((((UINT32) (_ui4_pic_blks)) & 0x0000FF00) >>  8);   \
            (_pt_pic_info)->aui1_data[6] = (UINT8) ((((UINT32) (_ui4_pic_blks)) & 0x00FF0000) >> 16);   \
        }

#define PVR_PIC_INFO_GET_PIC_BLKS(_pt_pic_info)                         \
            ((UINT32) ( ((UINT32) (_pt_pic_info)->aui1_data[4])       | \
                       (((UINT32) (_pt_pic_info)->aui1_data[5]) << 8) | \
                       (((UINT32) (_pt_pic_info)->aui1_data[6]) << 16)) )

/* picture info entry */
typedef struct _PVR_PIC_INFO_T
{
    UINT8       aui1_data[7];  /* bit  0-24: LBA offset (-2^24 ~ (2^24-1))
                                  bit 25-29: reserved,
                                  bit 30-31: picture type
                                  bit 32-55: picture blocks (0 ~ (2^24)) */
    
} PVR_PIC_INFO_T;


#define PVR_TICK_DETAIL_SET_BLKS(_pt_tk_detail, _ui2_tk_blks)           \
    (_pt_tk_detail)->ui4_blks_entries = (((_pt_tk_detail)->ui4_blks_entries & ((UINT32) 0xFFFF0000)) | \
                                         (((UINT32) (_ui2_tk_blks)) & ((UINT32) 0x0000FFFF)))
    
#define PVR_TICK_DETAIL_GET_BLKS(_pt_tk_detail)                         \
    ((UINT16) ((_pt_tk_detail)->ui4_blks_entries & ((UINT32) 0x0000FFFF)))
    
#define PVR_TICK_DETAIL_SET_ENTRIES(_pt_tk_detail, _ui1_entries)        \
    (_pt_tk_detail)->ui4_blks_entries = (((_pt_tk_detail)->ui4_blks_entries & ((UINT32) 0xFF00FFFF)) | \
                                         ((((UINT32) (_ui1_entries)) & ((UINT32) 0x000000FF)) << 16))

#define PVR_TICK_DETAIL_GET_ENTRIES(_pt_tk_detail)                      \
    ((UINT8) (((_pt_tk_detail)->ui4_blks_entries & ((UINT32) 0x00FF0000)) >> 16))

/* tick detail info */
typedef struct _PVR_TICK_DETAIL_T
{
    UINT32              ui4_tick_num;
    UINT32              ui4_blks_entries; /* bit  0-15: tick blocks (0 ~ 65535),  
                                             bit 16-23: entries (0 ~ 255) 
                                             bit 24-31: reserved */
    
    /* PVR_PIC_INFO_T*  pt_pics; picture info array */
    
    /* UINT64           ui8_system_time;  check flags in PVR_TICK_INDEX_T*/
    
} PVR_TICK_DETAIL_T;

#define PVR_TICK_INDEX_FLAG_EMPTY           MAKE_BIT_MASK_32(0)
#define PVR_TICK_INDEX_FLAG_BAD             MAKE_BIT_MASK_32(1)
#define PVR_TICK_INDEX_FLAG_HAS_I_FRAME     MAKE_BIT_MASK_32(2)
#define PVR_TICK_INDEX_FLAG_HAS_SYS_TIME    MAKE_BIT_MASK_32(3)

/* tick index info */
typedef struct _PVR_TICK_INDEX_T
{
    UINT32              ui4_tick_num;
    UINT32              ui4_lba;   
    PVR_TICK_DETAIL_T*  pt_tick_detail;
    UINT32              ui4_flags; 
    
} PVR_TICK_INDEX_T;

/* tick metadata control block */
typedef struct _PVR_TICK_CTRL_BLK_T
{
    VOID*       pv_start_address;
    VOID*       pv_end_address;
    UINT32      ui4_entry_num;
    UINT32      ui4_entry_size;
    UINT32      ui4_max_vld_entry_num;
    UINT32      ui4_tick_period;
    UINT32      ui4_lba_init;
    UINT64      ui8_fifo_offset;   
    UINT32      ui4_fifo_pkt_num;
    
} PVR_TICK_CTRL_BLK_T;

/* tick number and tick index should follow the below rules */
#define PVR_GET_TICK_IDX_BY_TICK_NUM(_pt_tk_ctrl_blk, _ui4_tick_num)    \
    ((UINT32) (_ui4_tick_num % (_pt_tk_ctrl_blk)->ui4_entry_num))


/***********  Mutimedia Playback Range Definition ***********************/

#define MM_RANGE_DIR_FORWARD               0
#define MM_RANGE_DIR_BACKWARD              1   
#define MM_RANGE_SEAMLESS                  0xFFFFFFFF

//#define VDEC_TIME_PROFILE

#ifdef VDEC_TIME_PROFILE
//#define VDEC_TIME_PROFILE_SAVE_TO_CURRET
//#define VDEC_TIME_PROFILE_PLAY_FREE_RUN
#endif

#ifndef MULTIMEDIA_AUTOTEST_SUPPORT
//#define MULTIMEDIA_AUTOTEST_SUPPORT
#endif

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT

typedef enum
{
 MM_AUTOTEST_COND_CASE_DONE,
 MM_AUTOTEST_COND_GOLDEN_DONE,
 MM_AUTOTEST_COND_INVALID,   
}MM_AUTOTEST_COND_T;

typedef VOID (*x_mm_autotest_nfy_fct) (
    VOID*               pv_nfy_tag,
    MM_AUTOTEST_COND_T  e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2
    );

typedef enum
{
  MM_AUTOTEST_INIT,
  MM_AUTOTEST_RESET,
  MM_AUTOTEST_TIMECASE,
  MM_AUTOTEST_EOSCASE,
  MM_AUTOTEST_MAKEGOLDEN,
  MM_AUTOTEST_GETEOSTIME,
  MM_AUTOTEST_PLAYSTOP,
  MM_AUTOTEST_CRCCHECK,  
  MM_AUTOTEST_GETCRCGOLDEN,
}MM_AUTOTEST_TYPE_T;

typedef struct _MM_AUTOTEST_INFO_T
{
  MM_AUTOTEST_TYPE_T e_cmd_type;
  UINT32 u4rParam[4];
#ifdef VDEC_TIME_PROFILE  
  CHAR   ucAutoPlayCurFileDir[512];
  CHAR   ucAutoPlayCurFileName[256];
#endif
} MM_AUTOTEST_INFO_T;
#endif


typedef enum
{
    MM_RANGE_DONE = 1,
    MM_RANGE_ABORT,
#ifdef TIME_SHIFT_SUPPORT
    MM_MONITOR_LBA_ARRIVAL,     /* pass the monitor LBA by ui4_data_1 */
    MM_OUT_OF_SAFE_RANGE_START, /* pass the current LBA by ui4_data_1 */
    MM_OUT_OF_SAFE_RANGE_END,   /* pass the current LBA by ui4_data_1 */
    MM_REACH_VALID_RANGE_START, /* pass the current LBA by ui4_data_1 */
    MM_REACH_VALID_RANGE_END,   /* pass the current LBA by ui4_data_1 */
    MM_SIGNAL_Y_TO_N,           /* pass the 1st tick number of section w/o signal */
    MM_SIGNAL_N_TO_Y,           /* pass the 1st tick number of section with signal */
    MM_BAD_TICK_Y_TO_N,
    MM_BAD_TICK_N_TO_Y,
    MM_DATA_READ_ERROR,
    MM_INTERNAL_ERROR,
#endif /* TIME_SHIFT_SUPPORT */
    MM_RANGE_SEEK_DONE,
    MM_RANGE_SEEK_ERROR,
    MM_RANGE_SEEK_NO_DATA,
    MM_RANGE_BUFF_READY,
    MM_RANGE_BUFF_UNDERFLOW,
#ifndef DRV_NO_PRE_PARSING
    MM_RANGE_PARSING_MODE,
    MM_RANGE_PARSING_MODE_DONE,
#endif
    MM_RANGE_MAX /* last one */
} MM_RANGE_COND_T;

/*Range Notify function*/
typedef BOOL (*x_mm_range_nfy_fct) (VOID*            pv_nfy_tag,
                                    MM_RANGE_COND_T  e_nfy_cond,
                                    UINT32           ui4_data_1,
                                    UINT32           ui4_data_2);
typedef BOOL (*x_mm_range_nfy_fct_ex) (VOID*            pv_nfy_tag,
                                       MM_RANGE_COND_T  e_nfy_cond,
                                       UINT32           ui4_data_1,
                                       UINT32           ui4_data_2,
                                       UINT32           ui4_data_3);
/* POSTION TYPE */
typedef enum
{
    MM_RANGE_POS_TYPE_UNKNOWN =0,
    MM_RANGE_POS_TYPE_TIME_RANGE_CURR,
    MM_RANGE_POS_TYPE_TIME_RANGE_ID,
    MM_RANGE_POS_TYPE_TIME_MEDIA,        /* Timeshift use tick index */
    MM_RANGE_POS_TYPE_OFFSET_RANGE_CURR,
    MM_RANGE_POS_TYPE_OFFSET_RANGE_ID,
    MM_RANGE_POS_TYPE_OFFSET_MEDIA,       /* Timeshift use LBA */
    MM_RANGE_POS_TYPE_OFFSET_RANGE_START,
    MM_RANGE_POS_TYPE_RANGE_AB_START        
} MM_RANGE_POS_TYPE_T ;

typedef struct _START_POS_INFO_T
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
}START_POS_INFO_T;

typedef struct _MM_RANGE_POS_INFO_T
{
    MM_RANGE_POS_TYPE_T   e_pos_type;      /*(I)*/
    MM_SPEED_TYPE_T       e_auto_speed;    /*(I)*/
    UINT32                ui4_idx;         /*(I)*/  
    UINT64                ui8_pos_val;     /*(I)*/
    START_POS_INFO_T      t_start_pos;
} MM_RANGE_POS_INFO_T;

/*ab_repeat_driver*/
typedef struct _MM_POS_T
{
    /*timecode for the most file*/
    UINT64  ui8_stc;
    /*position for ts and  video es*/
    UINT64  ui8_aud_pts_info;
    UINT64  ui8_aud_frame_position;
    UINT64  ui8_i_pts_info;
    UINT64  ui8_pts_info;
    UINT64  ui8_i_frame_position;
    UINT64  ui8_frame_position;
    UINT64  ui8_frame_position_disp;
    INT32   i4_temporal_reference;
    UINT16  ui2_decoding_order;
}MM_POS_T;

typedef struct _MM_AB_POS_T
{
    BOOL b_valid;          /* Enable or Cancel */
    MM_POS_T t_pos_a;
    MM_POS_T t_pos_b;
}MM_AB_POS_T;


typedef enum
{
    MM_3D_PROPERTY_UNKOWN = 0,
    MM_3D_PROPERTY_DAUL
}MM_3D_PROPERTY_T;


typedef struct _MM_BUF_RANGE_INFO_T
{
    UINT32      ui4_start; /* index of range start */
    UINT32      ui4_end;   /* index of range end, ui4_end could be smaller than ui4_start when tick index wrap over */
}   MM_BUF_RANGE_INFO_T;

/* SM_SESS_GNRC_SET_TYPE_REC_BUFFER_RANGE */
/* PM_SET_REC_BUF_RANGE */
/* DEMUX_SET_TYPE_REC_BUF_RANGE */
typedef struct _MM_REC_BUF_RANGE_INFO_T
{
    MM_BUF_RANGE_INFO_T     t_valid; /* valid range for playback */
    MM_BUF_RANGE_INFO_T     t_safe;  /* safe range for playback, safe range should be a subset of valid range */
}   MM_REC_BUF_RANGE_INFO_T;

#define MM_REC_BUF_SAFE_RANGE_START_THRESHOLD   (UINT32)4
#define MM_REC_BUF_SAFE_RANGE_END_THRESHOLD     (UINT32)2

/* INDEX TABLE TYPE*/
typedef enum
{
    MM_RANGE_ELEMT_IDXTBL_TYPE_UNKNOWN=0,
    MM_RANGE_ELEMT_IDXTBL_TYPE_PLAYBACK,
    MM_RANGE_ELEMT_IDXTBL_TYPE_TIMESHIFT,
    MM_RANGE_ELEMT_IDXTBL_TYPE_NONEXIST
} MM_RANGE_ELEMT_IDXTBL_TYPE_T;

/*ELEMENT TYPE*/
typedef enum
{
    MM_ELEMT_TYPE_UNKNOWN=0,
    MM_ELEMT_TYPE_VIDEO,
    MM_ELEMT_TYPE_AUDIO,
    MM_ELEMT_TYPE_SP,
    MM_ELEMT_TYPE_KEY
} MM_ELEMT_TYPE_T;


/*Table entry for multimedia playback*/
typedef struct _MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T
{
    UINT32 ui4_pts;                /* might be invalid */
    UINT32 ui4_relative_offset;    /* physcical file offset = ui8_base_offset + ui4_relative_offset */
    UINT32 ui4_size;
} MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T;

/*Table entry for multimedai timeshift*/
typedef struct _MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T
{
    PVR_TICK_CTRL_BLK_T       t_tick_ctrl_blk;
  /*detail for Time Shift usage*/
} MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T;

/*Element index table for one range*/
typedef struct _MM_RANGE_ELMT_IDXTBL_T
{
    MM_RANGE_ELEMT_IDXTBL_TYPE_T    e_tbl_type;   
    MM_ELEMT_TYPE_T                 e_elem_type;
    STREAM_ID_T                     t_strm_id;                       
    UINT32                          ui4_scale;                      
    UINT32                          ui4_rate;                       
    UINT64                          ui8_base_offset;    /* in case of file size > 4G */
    UINT32                          ui4_number_of_entry;
    UINT32                          ui4_start_frame_num;
    UINT32                          ui4_end_frame_num;
    UINT32                          ui4_avg_bytes_per_sec;
    UINT32                          ui4_align; 
    BOOL                            b_is_VBR;
    UINT64                          ui8_timeunit;
    UINT32                          ui4_avg_packet_size;  /*ogg avg page length*/
    UINT32                          ui4_avg_frame_num;   /*ogg avg granule pos*/
    union
    {
      VOID*                             pv_idx_tbl_entry; 
      MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* pt_pb_idx_tbl_entry;
      MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T* pt_ts_idx_tbl_entry;
    }u;
    ASH_AUDIO_CODEC_SPEC_INFO_T         t_aud_spec_info;
    struct _MM_RANGE_ELMT_IDXTBL_T*     pt_next_partial;
    void*                           pv_next_tbl; 
} MM_RANGE_ELMT_IDXTBL_T;

/*track timecode scale structure, for mkv file*/
typedef struct _MM_RANGE_TRACK_TM_SCALE_T
{
    UINT32                      ui4_strm_id;
    UINT32                      ui4_tm_scale_numerator;
    UINT32                      ui4_tm_scale_denominator;
}MM_RANGE_TRACK_TM_SCALE_T;

/*source type*/
typedef enum
{
    MM_SRC_TYPE_UNKNOWN=0,
    MM_SRC_TYPE_HIGH_SPEED_STORAGE,
    MM_SRC_TYPE_NETWORK_DLNA,
    MM_SRC_TYPE_NETWORK_NETFLIX,
    MM_SRC_TYPE_NETWORK_VUDU,
    MM_SRC_TYPE_NETWORK_RHAPSODY,
    MM_SRC_TYPE_PUSH,
    MM_SRC_TYPE_NETWORK_CMPB,
    MM_SRC_TYPE_NETWORK_XUNLEI,
    MM_SRC_TYPE_NETWORK_MHP,
    MM_SRC_TYPE_NETWORK_SKYPE
} MM_SRC_TYPE_T;

typedef struct _MM_AUDIO_VORBIS_OFFSET_T
{
    UINT32      ui4_vorbis_header_offset;
    UINT32      ui4_setup_offset;
    UINT32      ui4_comment_offset;
}MM_AUDIO_VORBIS_OFFSET_T;

typedef struct _MM_RANGE_DECODER_T
{
    UINT32                          ui4_strm_id;
    BOOL                            b_has_encoding;
    UINT32                          ui4_encoding_num;
    VOID*                           pv_encoding_string;
     /*add for codec private*/
    BOOL                            b_has_codec_private;
    UINT64                          ui8_codec_private_len;
    UINT8*                          pui1_codec_private;

    BOOL                            b_has_vorbis_info;
    MM_AUDIO_VORBIS_OFFSET_T        t_audio_vorbis_offset;        
    struct _MM_RANGE_DECODER_T*     pt_next; 
}MM_RANGE_DECODER_T;

typedef struct _MM_RANGE_INFO_T
{
    UINT32  ui4_range_id;
    UINT64  ui8_fileoffset;
    UINT64  ui8_timecode_scale; /*for mkv segment timecode scale*/
    UINT64  z_range_sz;
#ifdef RM_LOGICAL_STREAM_SUPPORT
    UINT64  ui8_fileoffset2[2];
    UINT64  z_range_sz2[2];
#endif
    UINT32  ui4_sz_max_acces_unt; /*for asf playback*/
    UINT32  ui4_sz_min_acces_unt; /*for asf playback*/ 
    STC_T   t_init_stc;
    PTS_T   t_vid_start_pts;
    PTS_T   t_vid_start_render_pts;
    UINT32  ui4_vid_render_counts;
    PTS_T   t_aud_start_pts;
    PTS_T   t_aud_start_render_pts;
    UINT32  ui4_aud_render_counts;
    UINT64  ui8_start_pcr;/*for ts last memory*/
    PTS_T   t_sp_start_pts;
    PTS_T   t_sp_start_render_pts;
    UINT32  ui4_sp_render_counts;
    UINT32  ui4_track_tm_scale_num; /*for mkv track tm scale*/
    PTS_T   t_pts_offset;  
    BOOL    b_direction;
    BOOL    b_endofstream;
    BOOL    b_key_tbl_exist;
    BOOL    b_thumbnail_pb;
    BOOL    b_non_seekable;        /*tell swdmx if the DLNA server support the seek*/
    MM_RANGE_ELMT_IDXTBL_T*    pt_idxtbl_lst;
    MM_RANGE_TRACK_TM_SCALE_T* pt_track_tm_scale_lst; /*for mkv track tm scale*/
    x_mm_range_nfy_fct         pt_range_nfy;
    VOID*                      pv_tag;
    BOOL b_drm;
    START_POS_INFO_T t_start_pos;
    DRM_INFO_DRV_T   t_drm_info_drv;
    UINT64  ui8_vid_duration;   /* video stream duration */
    UINT64  ui8_pb_duration;    /* file playback duration */
    MM_SRC_TYPE_T e_mm_src_type;
#if SUPPORT_MULTI_INFO
    struct _MM_RANGE_INFO_T* pt_next_scale;
#endif
    /*add for 3d media*/
    BOOL b_stereo_scopic;
    BOOL b_half_width;
    BOOL b_half_height;
    BOOL b_support_3d_trick;
    MM_SCOPIC_TYPE_T e_scopic_type;
    UINT64  ui8_skip_dur;   
    
    MM_RANGE_DECODER_T*     pt_vid_decoder_info;
    MM_RANGE_DECODER_T*     pt_aud_decoder_info; 

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    BOOL    b_is_bad_intlevd;
    UINT32  ui4_1st_pkgnum;
#endif

} MM_RANGE_INFO_T;
    
/***********  MM Encryption/Decryption Definition ***********************/
/* Encryption/Descryption algorithm */
typedef enum
{
    MM_CRYPT_MODE_INVALID = -1,
    MM_CRYPT_MODE_DES_EBC = 0,
    MM_CRYPT_MODE_DES_CBC,
    MM_CRYPT_MODE_3_DES_EBC,
    MM_CRYPT_MODE_3_DES_CBC,    
    MM_CRYPT_MODE_AES_EBC
}   MM_CRYPT_MODE_T;

/* Encryption/Descryption key index */
typedef enum
{
    MM_CRYPT_KEY_ODD = 0,
    MM_CRYPT_KEY_EVEN,
    MM_CRYPT_KEY_ALTERNATE
}   MM_CRYPT_KEY_INDEX_T;

/* Encryption/Descryption info */
typedef struct 
{
    MM_CRYPT_MODE_T       e_mode;
    UINT32                   aui4_odd_key_1[2];
    UINT32                   aui4_odd_key_2[2];
    UINT32                   aui4_even_key_1[2];
    UINT32                   aui4_even_key_2[2];
    MM_CRYPT_KEY_INDEX_T    e_key_idx;    
}MM_PVR_CRYPT_INFO_T;

typedef struct 
{
    STREAM_TYPE_T       e_strm_type;
    UINT32              ui4_track_id;
    UINT16              ui2_key_idx;
}MM_TRACK_CRYPT_INFO_T;

#define MAX_MARLIN_TRACK_NUM        7/* marlin max number of track : 1(video) + 2(audio) + 4(subtitle) */
typedef struct 
{
    UINT8                   ui1_valid_cnt;
    MM_TRACK_CRYPT_INFO_T   t_track_drm[MAX_MARLIN_TRACK_NUM];
}MM_ML_CRYPT_INFO_T;

typedef enum
{
    MM_CRYPT_TYPE_UNKNOWN = 0,
    MM_CRYPT_TYPE_PVR,
    MM_CRYPT_TYPE_MLN
} MM_CRYPT_TYPE_T;
    
typedef struct 
{ 
    MM_CRYPT_TYPE_T         e_mm_crypt_type;
    union
    {
        MM_PVR_CRYPT_INFO_T     t_pvr_crypt;
        MM_ML_CRYPT_INFO_T      t_ml_crypt;
    }u;
}   MM_CRYPT_INFO_T;


/* Encryption/Descryption initial vector info */
typedef struct _MM_CRYPT_IV_T
{
    UINT32                   aui4_init_vector[2];
    
} MM_CRYPT_IV_T;


/* Encryption/Descryption info with stream id */
typedef struct _MM_DESC_CRYPT_INFO_T
{
    STREAM_ID_T t_stream_setting; 
    MM_CRYPT_INFO_T  t_crypto_info;    
}   MM_DESC_CRYPT_INFO_T;


/* Encryption/Descryption command type */
typedef enum
{
    MM_CRYPT_CTRL_CMD_UNKNOWN = 0,
    MM_CRYPT_CTRL_CMD_RESET_ALL
}   MM_CRYPT_CTRL_CMD_T;

typedef struct
{
    MM_CRYPT_CTRL_CMD_T t_crypt_ctrl_cmd;
    VOID*                 pt_info;
}   MM_CRYPT_CMD_T;

/* cli dump tick control block */
typedef struct
{
    UINT32              ui4_magic_num;
    UINT32              ui4_tick_period;
    UINT32              ui4_tick_num;
    UINT32              ui4_valid_range_start;
    UINT32              ui4_valid_range_end;
    UINT64              ui8_pvr_pre_ofst;
    MM_CRYPT_INFO_T     t_sess_key;
} REC_UTIL_CLI_DUMP_TICK_CB_T;

#define REC_UTIL_CLI_DUMP_TICK_CB_MAGIC_NUM     (UINT32)0xAAAAAAAA

#endif /* _X_MM_COMMON_H_ */

