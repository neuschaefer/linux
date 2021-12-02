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
 * $RCSfile: cmpb_ctrl.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor:  $
 * $MD5HEX: d908c026a37c5be69fa4ecb1520bc593 $
 *
 * Description:
 *         This is the source file for common playback adapter
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "svctx/u_svctx.h"
#include "svctx/x_svctx.h"
#include "svctx/c_svctx.h"
#include "mutil/minfo/u_minfo.h"
#include "svctx/mm_hdlr/u_mm_hdlr.h"

#include "cmpb_util.h"
#include "IMtkPb_ErrorCode.h"
#include "IMtkPb_Ctrl.h"
#include "IMtkPb_Ctrl_DTV.h"
#include "IMtkPb_Snd.h"

#include "handle/c_handle.h"
#include "handle/x_handle.h"
#include "mutil/minfo/c_minfo.h"
#include "os/inc/os.h"
#include "os/inc/c_os.h"
#include "os/inc/x_os.h"
#include "unicode/c_uc_str.h"
#include "strm_mngr/scc/c_scc.h"
#include "strm_mngr/scc/x_scc.h"
#include "conn_mngr/cm_playback/u_playback_handler.h"


/*-----------------------------------------------------------------------------
 * macros, typedefs, enums
 *---------------------------------------------------------------------------*/
#define OUTPUT_LOG                  1
#if OUTPUT_LOG
#define SND_LOG                     printf
#else
#define SND_LOG(string,...)                   
#endif
#define BUF_THRESHOLD               5
#define MAX_CMPB_INST_NUM           4
#define CMPB_HANDLE_TAG             0x65430000
#define INVALIDE_CMPB_HANDLE        (IMTK_PB_HANDLE_T)0xFFFFFFFF

#define CMPB_SND_COUNT              1
#define DRIVER_BUFF_COUNT           8
#define DRIVER_BUFF_SIZE            (1024*16)

#define MAX_SET_AUD_NUM             1
#define MAX_GET_AUD_NUM             8

#define MAX_SET_VID_NUM             1
#define MAX_GET_VID_NUM             1

#define MAX_GET_SUB_NUM             8

#define CMPB_SVCTX_NAME_MAIN                ("main_svctx")
#define CMPB_SVCTX_NAME_SUB                 ("sub_svctx")
#define CMPB_SVCTX_NAME_THIRD               ("thrd_svctx")
#define CMPB_SVCTX_NAME_TV_BYPASS           ("tv_bypass_svctx")
#define CMPB_SVCTX_NAME_MONITOR_BYPASS      ("mn_bypass_svctx")

#ifdef LINUX_TURNKEY_SOLUTION

#define CMPB_HANDLE_VALID           c_handle_valid
#define CMPB_SEMA_CREATE            c_sema_create
#define CMPB_SEMA_DELETE            c_sema_delete
#define CMPB_SEMA_LOCK              c_sema_lock
#define CMPB_SEMA_UNLOCK            c_sema_unlock
#define CMPB_UC_W2S_STRNCPY         c_uc_w2s_strncpy
#define CMPB_MEM_FREE               c_mem_free
#define CMPB_MEM_CALLOC             c_mem_calloc
#define CMPB_MEM_PART_ALLOC         c_mem_part_alloc
#define CMPB_MEM_PART_CREATE        c_mem_part_create
#define CMPB_MEM_PART_DELETE        c_mem_part_delete
#define CMPB_MEMSET                 c_memset
#define CMPB_MEMCPY                 c_memcpy
#define CMPB_STRNCPY                c_strncpy
#define CMPB_STRLEN                 c_strlen
#define CMPB_STRCPY                 c_strcpy



#define CMPB_MSG_Q_CREATE           x_msg_q_create
#define CMPB_MSG_Q_DELETE           x_msg_q_delete
#define CMPB_MSG_Q_SEND             x_msg_q_send
#define CMPB_MSG_Q_RECEIVE          x_msg_q_receive
#define CMPB_THREAD_CREATE          x_thread_create
#define CMPB_THREAD_EXIT            x_thread_exit
#define CMPB_THREAD_DELAY           c_thread_delay
#define CMPB_TIMER_CREATE           x_timer_create
#define CMPB_TIMER_DELETE           x_timer_delete
#define CMPB_TIMER_START            x_timer_start

#define CMPB_SVCTX_OPEN             c_svctx_open
#define CMPB_SVCTX_CLOSE            c_svctx_close
#define CMPB_SVCTX_SET              c_svctx_set
#define CMPB_SVCTX_GET              c_svctx_get
#define CMPB_SVCTX_SELECT_SVC       c_svctx_select_svc
#define CMPB_SVCTX_SYNC_STOP_SVC    c_svctx_sync_stop_svc
#define CMPB_SVCTX_SELECT_STREAM    c_svctx_select_stream
#define CMPB_SVCTX_STOP_STREAM      c_svctx_stop_stream
#define CMPB_SVCTX_SYNC_STOP_STREAM         c_svctx_sync_stop_stream
#define CMPB_SVCTX_GET_GET_STREAM_DATA      c_svctx_get_using_strm_data
#define CMPB_SVCTX_SET_STREAM_ATTR  c_svctx_set_stream_attr
#define CMPB_SVCTX_SET_SESSION_ATTR c_svctx_set_session_attr
#define CMPB_SVCTX_MEDIA_OPEN       c_svctx_media_open
#define CMPB_SVCTX_MEDIA_CLOSE      c_svctx_media_close
#define CMPB_SVCTX_MEDIA_SEEK       c_svctx_media_seek
#define CMPB_SVCTX_MEDIA_SET_INFO   c_svctx_media_set_info
#define CMPB_SVCTX_MEDIA_GET_INFO   c_svctx_media_get_info
#define CMPB_SVCTX_GET_STREAM_COMP_ID   c_svctx_get_stream_comp_id
#define CMPB_SVCTX_GENERIC_SET_INFO     c_svctx_generic_set_info
#define CMPB_SVCTX_GENERIC_GET_INFO     c_svctx_generic_get_info

#define CMPB_SVCTX_GET_VIDEO_SRC_RESOLUTION     c_svctx_get_video_src_resolution

#define CMPB_C_SCC_COMP_OPEN                c_scc_comp_open
#define CMPB_C_SCC_COMP_CLOSE               c_scc_comp_close
#define CMPB_C_SCC_COMP_GRP_SET             c_scc_comp_grp_set
#define CMPB_C_SCC_COMP_AUD_SET_CLIP        c_scc_aud_set_clip
#define CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL   c_scc_aud_set_clip_ctrl

#else

#define CMPB_HANDLE_VALID           x_handle_valid
#define CMPB_SEMA_CREATE            x_sema_create
#define CMPB_SEMA_DELETE            x_sema_delete
#define CMPB_SEMA_LOCK              x_sema_lock
#define CMPB_SEMA_UNLOCK            x_sema_unlock
#define CMPB_UC_W2S_STRNCPY         x_uc_w2s_strncpy
#define CMPB_MEM_FREE               x_mem_free
#define CMPB_MEM_CALLOC             x_mem_calloc
#define CMPB_MEM_PART_ALLOC         x_mem_part_alloc
#define CMPB_MEM_PART_CREATE        x_mem_part_create
#define CMPB_MEM_PART_DELETE        x_mem_part_delete
#define CMPB_MEMSET                 x_memset
#define CMPB_MEMCPY                 x_memcpy
#define CMPB_STRNCPY                x_strncpy
#define CMPB_STRLEN                 x_strlen
#define CMPB_STRCPY                 x_strcpy


#define CMPB_MSG_Q_CREATE           x_msg_q_create
#define CMPB_MSG_Q_DELETE           x_msg_q_delete
#define CMPB_MSG_Q_SEND             x_msg_q_send
#define CMPB_MSG_Q_RECEIVE          x_msg_q_receive
#define CMPB_THREAD_CREATE          x_thread_create
#define CMPB_THREAD_EXIT            x_thread_exit
#define CMPB_THREAD_DELAY           x_thread_delay
#define CMPB_TIMER_CREATE           x_timer_create
#define CMPB_TIMER_DELETE           x_timer_delete
#define CMPB_TIMER_START            x_timer_start

#define CMPB_SVCTX_OPEN             x_svctx_open
#define CMPB_SVCTX_CLOSE            x_svctx_close
#define CMPB_SVCTX_SET              x_svctx_set
#define CMPB_SVCTX_GET              x_svctx_get
#define CMPB_SVCTX_SELECT_SVC       x_svctx_select_svc
#define CMPB_SVCTX_SYNC_STOP_SVC    x_svctx_sync_stop_svc
#define CMPB_SVCTX_SELECT_STREAM    x_svctx_select_stream
#define CMPB_SVCTX_STOP_STREAM      x_svctx_stop_stream
#define CMPB_SVCTX_SYNC_STOP_STREAM         x_svctx_sync_stop_stream
#define CMPB_SVCTX_GET_GET_STREAM_DATA      x_svctx_get_using_strm_data
#define CMPB_SVCTX_SET_STREAM_ATTR  x_svctx_set_stream_attr
#define CMPB_SVCTX_SET_SESSION_ATTR x_svctx_set_session_attr
#define CMPB_SVCTX_MEDIA_OPEN       x_svctx_media_open
#define CMPB_SVCTX_MEDIA_CLOSE      x_svctx_media_close
#define CMPB_SVCTX_MEDIA_SEEK       x_svctx_media_seek
#define CMPB_SVCTX_MEDIA_SET_INFO   x_svctx_media_set_info
#define CMPB_SVCTX_MEDIA_GET_INFO   x_svctx_media_get_info
#define CMPB_SVCTX_GET_STREAM_COMP_ID   x_svctx_get_stream_comp_id
#define CMPB_SVCTX_GENERIC_SET_INFO     x_svctx_generic_set_info
#define CMPB_SVCTX_GENERIC_GET_INFO     x_svctx_generic_get_info

#define CMPB_SVCTX_GET_VIDEO_SRC_RESOLUTION     x_svctx_get_video_src_resolution

#define CMPB_C_SCC_COMP_OPEN                x_scc_comp_open
#define CMPB_C_SCC_COMP_CLOSE               x_scc_comp_close
#define CMPB_C_SCC_COMP_GRP_SET             x_scc_comp_grp_set
#define CMPB_C_SCC_COMP_AUD_SET_CLIP        x_scc_aud_set_clip
#define CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL   x_scc_aud_set_clip_ctrl

#endif

typedef struct
{
    void* pv_phy_addr;
    void* pv_vir_addr;
}SND_BUF_T;

typedef struct
{
    UINT8       ui1_free_buf_cnt;
    UINT8       ui1_cur_buf_idx;
    SND_BUF_T   at_snd_buf[DRIVER_BUFF_COUNT];
}SND_CLIP_BUF_INFO_T;

typedef struct
{   
    HANDLE_T                        h_sema;
    HANDLE_T                        h_scc;
    BOOL                            b_play;
    BOOL                            b_active;
    BOOL                            b_ctrl_error;
    BOOL                            b_ctrl_done;
    IMTK_PB_CTRL_AUD_PCM_INFO_T     tPcmInfo;
    IMtkSnd_Ctrl_Nfy_Fct            pf_callback;
    void*                           pv_tag;
    SND_CLIP_BUF_INFO_T             t_aud_clip_buf;
} CMPB_SND_INST_T;

/*-----------------------------------------------------------------------------
 *                   data declarations
 ----------------------------------------------------------------------------*/
static CMPB_SND_INST_T at_snd_inst[CMPB_SND_COUNT];
/***************add for send file from URL*********************/
static struct 
{
    BOOL                   b_play_file;
    BOOL                   b_play_finish;
    UINT32                 ui4_rpt_cnt;
    UINT32                 ui4_file_split_cnt;
    UINT32                 ui4_send_cnt;
    CHAR                   sz_pcm_file_path[256];
    IMtkSnd_Ctrl_Nfy_Fct   pf_send_file_callback;
    IMTK_PB_HANDLE_T       hpb_handle;
} rSndFilePCM = {0, 1};
/***************end add for send file from URL*********************/

/*-----------------------------------------------------------------------------
 *                   functions declaraions
 ----------------------------------------------------------------------------*/

extern void x_share_mem_free(void* pv_phy_addr, unsigned int z_size);
extern void x_munmap_share_mem(void* pv_virt_addr, unsigned int z_size);
extern void* x_share_mem_alloc(unsigned int z_size);
extern void* x_mmap_share_mem(void* pv_phy_addr, unsigned int z_size);

/*-----------------------------------------------------------------------------
 *                   Internal functions implementations
 -----------------------------------------------------------------------------*/

static UINT16 _cmpb_map_bit_depth(IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_T e_depth)
{
    switch (e_depth)
    {
    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_8:
        return 8;

    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_16:
        return 16;

    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_20:
        return 20;

    case IMTK_PB_CTRL_AUD_PCM_BIT_DEPTH_24:
        return 24;

    default:
        return 16;
    }
}

static UINT8 _cmpb_map_scc_channels(IMTK_PB_CTRL_AUD_CH_NUM_T e_channel)
{
    switch (e_channel)
    {
    case IMTK_PB_CTRL_AUD_CH_MONO:
        return 1;

    case IMTK_PB_CTRL_AUD_CH_STEREO:
        return 2;

    default:
        return 2;
    }
}

static UINT32 _cmpb_map_sample_rate(IMTK_PB_CTRL_AUD_SAMPLE_RATE_T eSampleRate)
{
    switch (eSampleRate)
    {
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_8K:
        return 8000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_16K:
        return 16000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_32K:
        return 32000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_11K:
        return 11025; /*return 11024*/
    
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_22K:
        return 22050; /*return 22255*/

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_44K:
        return 44100;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_12K:
        return 12000;
        
    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_24K:
        return 24000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_48K:
        return 48000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_96K:
        return 96000;

    case IMTK_PB_CTRL_AUD_SAMPLE_RATE_192K:
        return 192000;
        
    default:
        return 48000;
    }
}


static IMTK_PB_ERROR_CODE_T _cmpb_aud_clip_rel_buf(IMTK_PB_HANDLE_T  hHandle)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;

    if ((hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if(pt_inst != NULL)
    {
        pt_inst->t_aud_clip_buf.ui1_free_buf_cnt++;
    }
    return IMTK_PB_ERROR_CODE_OK;
}

static IMTK_PB_ERROR_CODE_T _cmpb_aud_clip_init_buf(IMTK_PB_HANDLE_T  hHandle)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;

    if ((hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if(pt_inst != NULL)
    {
        UINT8 ui1_idx = 0;
        
        for(ui1_idx = 0; ui1_idx < DRIVER_BUFF_COUNT; ui1_idx++)
        {
            pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr
                = x_share_mem_alloc(DRIVER_BUFF_SIZE);
            if(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr == NULL)
            {
                return IMTK_PB_ERROR_CODE_INSUFFICIENT_MEMORY;
            }
            
            pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_vir_addr
                = x_mmap_share_mem(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr, DRIVER_BUFF_SIZE);
        }
        pt_inst->t_aud_clip_buf.ui1_free_buf_cnt = DRIVER_BUFF_COUNT;
        pt_inst->t_aud_clip_buf.ui1_cur_buf_idx = 0;
        return IMTK_PB_ERROR_CODE_OK;
    }
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

static IMTK_PB_ERROR_CODE_T _cmpb_aud_clip_free_buf(IMTK_PB_HANDLE_T  hHandle)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;

    if ((hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if(pt_inst != NULL)
    {
        UINT8 ui1_idx = 0;
        
        for(ui1_idx = 0; ui1_idx < DRIVER_BUFF_COUNT; ui1_idx++)
        {
            if(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr != NULL)
            {
                x_munmap_share_mem(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr, DRIVER_BUFF_SIZE);
                x_share_mem_free(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr, DRIVER_BUFF_SIZE);
                pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr = NULL;
                pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr = NULL;
            }
        }
        pt_inst->t_aud_clip_buf.ui1_free_buf_cnt = 0;
        pt_inst->t_aud_clip_buf.ui1_cur_buf_idx = 0;
        return IMTK_PB_ERROR_CODE_OK;
    }
    return IMTK_PB_ERROR_CODE_NOT_OK;
}

static INT32 _snd_nfy_fct ( HANDLE_T            h_aud,
                            SCC_AUD_CLIP_COND_T e_nfy_cond,
                            VOID*               pv_nfy_tag,
                            UINT32              ui4_data_1,
                            UINT32              ui4_data_2 )
{
    IMTK_PB_HANDLE_T            hHandle;
    UINT32                      ui4_i;
    CMPB_SND_INST_T*            pt_inst;
        
    hHandle = (IMTK_PB_HANDLE_T)pv_nfy_tag;
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
            if (pt_inst->h_scc != h_aud)
            {
                return SMR_INV_ARG;
            }

            switch (e_nfy_cond)
            {
            case SCC_AUD_CLIP_COND_ERROR:
                pt_inst->b_ctrl_done = TRUE;
                pt_inst->b_ctrl_error = TRUE;

                if (pt_inst->pf_callback)
                {
                    pt_inst->pf_callback(hHandle, IMTKPB_SND_CLIP_COND_ERROR, pt_inst->pv_tag, 0, 0);
                }
                break;

            case SCC_AUD_CLIP_COND_CTRL_DONE:
                pt_inst->b_ctrl_done = TRUE;
                pt_inst->b_ctrl_error = FALSE;
                break;

            case SCC_AUD_CLIP_COND_AUD_CLIP_DONE:
                _cmpb_aud_clip_rel_buf(hHandle);
                if (pt_inst->pf_callback)
                {
                    pt_inst->pf_callback(hHandle, IMTKPB_SND_CLIP_COND_CLIP_DONE, pt_inst->pv_tag, 0, 0);
                }
                break;
            default:
                break;
            }

            return SMR_OK;
        }
        else
        {
            return SMR_INV_ARG;
        }
    }
    else
    {
        return SMR_INV_ARG;
    }
}

/*-----------------------------------------------------------------------------
 *                   External functions implementations
 ----------------------------------------------------------------------------*/
IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Init(void)
{
    INT32 i4_ret;
    UINT8 ui1_idx;
    SND_LOG("enter IMtkPb_Snd_Init() \n");
    CMPB_MEMSET(&at_snd_inst, 0, CMPB_SND_COUNT*sizeof(CMPB_SND_INST_T));

    for (ui1_idx=0; ui1_idx<CMPB_SND_COUNT; ui1_idx++)
    {
        i4_ret = CMPB_SEMA_CREATE(
                                &(at_snd_inst[ui1_idx].h_sema),
                                X_SEMA_TYPE_BINARY,
                                X_SEMA_STATE_UNLOCK
                                );
        if(OSR_OK != i4_ret)
        {
            break;
        }
    }

    if (ui1_idx<CMPB_SND_COUNT)
    {
        for (ui1_idx=0; ui1_idx<CMPB_SND_COUNT; ui1_idx++)
        {
            if (at_snd_inst[ui1_idx].h_sema != NULL_HANDLE)
            {
                CMPB_SEMA_DELETE(at_snd_inst[ui1_idx].h_sema);
                at_snd_inst[ui1_idx].h_sema = NULL_HANDLE;
            }
        }

        return IMTK_PB_ERROR_CODE_NOT_OK;
    }
    SND_LOG("exit IMtkPb_Snd_Init() ok \n");
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Terminate(void)
{
    UINT8 ui1_idx;

    for (ui1_idx=0; ui1_idx<CMPB_SND_COUNT; ui1_idx++)
    {
        if (at_snd_inst[ui1_idx].h_sema != NULL_HANDLE)
        {
            CMPB_SEMA_DELETE(at_snd_inst[ui1_idx].h_sema);
            at_snd_inst[ui1_idx].h_sema = NULL_HANDLE;
        }
    }
    
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Count (uint8_t*     pu1Count)
{
    if (pu1Count)
    {
        *pu1Count = CMPB_SND_COUNT;
        return IMTK_PB_ERROR_CODE_OK;
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Query_Capability (uint8_t       u1SndIdx, 
                                                  uint32_t*     au4SampleRate,
                                                  uint8_t*      pu1Count)
{
    if ((pu1Count==NULL)||(au4SampleRate==NULL))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if (u1SndIdx >= CMPB_SND_COUNT)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    else
    {
        *pu1Count = 1;
        *au4SampleRate = 44100;
        return IMTK_PB_ERROR_CODE_OK;

    }
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Open (IMTK_PB_HANDLE_T*     phHandle,
                                      uint8_t               u1SndIdx)
{
    INT32           i4_ret;
    SM_COMMAND_T    at_sm_cmds[3];

    SND_LOG("enter IMtkPb_Snd_Open(), u1SndIdx = %d, phHandle = %x \n", (int)u1SndIdx, (unsigned int)phHandle);
        
    if ((u1SndIdx >= CMPB_SND_COUNT) || (phHandle==NULL))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (at_snd_inst[u1SndIdx].h_sema != NULL_HANDLE)
    {
        i4_ret = CMPB_SEMA_LOCK(at_snd_inst[u1SndIdx].h_sema, X_SEMA_OPTION_WAIT);
        if (i4_ret != OSR_OK)
        {
            SND_LOG("enter IMtkPb_Snd_Open() fail, lock sema fail, i4_ret = %d \n", (int)i4_ret);    
            return IMTK_PB_ERROR_CODE_NOT_OK;
        }
    }
    else
    {
        SND_LOG("enter IMtkPb_Snd_Open() fail, IMTK_PB_ERROR_CODE_NOT_INIT \n"); 
        return IMTK_PB_ERROR_CODE_NOT_INIT;
    }

    if (at_snd_inst[u1SndIdx].b_active)
    {
        SND_LOG("enter IMtkPb_Snd_Open() fail, at_snd_inst[u1SndIdx].b_active = TRUE \n"); 
        CMPB_SEMA_UNLOCK(at_snd_inst[u1SndIdx].h_sema);
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    at_sm_cmds[0].e_code     = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[0].u.ui4_data = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[1].e_code     = SCC_CMD_CODE_NAME;
    at_sm_cmds[1].u.ps_text  = SN_PRES_AUX_SOUND;//SN_PRES_MAIN_DISPLAY;
    at_sm_cmds[2].e_code = SM_CMD_CODE_END;
    at_sm_cmds[2].u.ui4_data = 0;
    i4_ret = CMPB_C_SCC_COMP_OPEN(at_sm_cmds, &at_snd_inst[u1SndIdx].h_scc);
    if(i4_ret != SMR_OK)
    {
        SND_LOG("enter IMtkPb_Snd_Open() fail, CMPB_C_SCC_COMP_OPEN() = %d \n", (int)i4_ret); 
        CMPB_SEMA_UNLOCK(at_snd_inst[u1SndIdx].h_sema);
        return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    *phHandle = CMPB_HANDLE_TAG|u1SndIdx;
    at_snd_inst[u1SndIdx].b_active = TRUE;
    _cmpb_aud_clip_init_buf(*phHandle);
    CMPB_SEMA_UNLOCK(at_snd_inst[u1SndIdx].h_sema);

    SND_LOG("enter IMtkPb_Snd_Open() ok, handle = %x \n", (unsigned int)(*phHandle)); 
    
    return IMTK_PB_ERROR_CODE_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_SetParameter (IMTK_PB_HANDLE_T               hHandle,
                                              IMTK_PB_CTRL_AUD_PCM_INFO_T*   ptPcmInfo)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;
    
    if (NULL == ptPcmInfo)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    SND_LOG("enter IMtkPb_Snd_SetParameter() ok\n"); 
    SND_LOG("IMtkPb_Snd_SetParameter() type:%d\n", (int)ptPcmInfo->e_pcm_type);
    SND_LOG("IMtkPb_Snd_SetParameter() ch_num:%d\n", (int)ptPcmInfo->eNumCh); 
    SND_LOG("IMtkPb_Snd_SetParameter() sample:%d\n", (int)ptPcmInfo->eSampleRate); 
    SND_LOG("IMtkPb_Snd_SetParameter() bit:%d\n", (int)ptPcmInfo->eBitsPerSample);  
    
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                if (at_snd_inst[ui4_i].b_play)
                {
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_DRV_ERROR;
                }
                else
                {
                    at_snd_inst[ui4_i].tPcmInfo = *ptPcmInfo;
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    SND_LOG("exit IMtkPb_Snd_SetParameter() ok\n");
                    return IMTK_PB_ERROR_CODE_OK;
                }
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_RegCallback (IMTK_PB_HANDLE_T       hHandle,
                                             IMtkSnd_Ctrl_Nfy_Fct   pfnCallback,
                                             void*                  pvTag)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;
    SND_LOG("enter IMtkPb_Snd_RegCallback() ok\n");
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                if (at_snd_inst[ui4_i].b_play)
                {
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_DRV_ERROR;
                }
                else
                {
                    at_snd_inst[ui4_i].pf_callback= pfnCallback;
                    at_snd_inst[ui4_i].pv_tag = pvTag;
                    SND_LOG("IMtkPb_Snd_RegCallback  pvTag = %x .... \n", 
                         (unsigned int)pvTag);
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    SND_LOG("exit IMtkPb_Snd_RegCallback() ok\n");
                    return IMTK_PB_ERROR_CODE_OK;
                }
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
}

/***************add for send file from URL*********************/
static void _snd_send_pcm_thread(void* pv_data)
{
    UINT32                ui4_read_cnt;
    IMTK_PB_ERROR_CODE_T    e_ret;                
    FILE*                   fp = 0;
    UINT32                ui4_strm_size = 0;
    uint8_t*                pt_buf;

    UINT32                  ui4_i;
    CMPB_SND_INST_T*        pt_inst;
    
    if ((rSndFilePCM.hpb_handle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = rSndFilePCM.hpb_handle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            SND_LOG("_snd_send_pcm_thread Snd hasn't been opened, i >= CMPB_SND_COUNT .... \n");
            return;
        }
    }
    else
    {
        SND_LOG("_snd_send_pcm_thread Snd hasn't been opened .... \n");
        return;
    }
        
    if(pt_inst == NULL)
    {
        SND_LOG("_snd_send_pcm_thread Snd hasn't been opened , pt_inst == NULL.... \n");
        return;
    }        

    fp = fopen(rSndFilePCM.sz_pcm_file_path, "rb");
    if(!fp)
    {
        rSndFilePCM.b_play_file = 0;
        rSndFilePCM.b_play_finish = TRUE;
        printf("_snd_send_pcm_thread open file failed .... \n");
        return;
    }

    fseek(fp, 0, SEEK_END);
    UINT32 uiSize = (UINT32)ftell(fp);
    rSndFilePCM.ui4_file_split_cnt = (UINT32)(uiSize / DRIVER_BUFF_SIZE);
    if (uiSize % DRIVER_BUFF_SIZE != 0)
    {
        rSndFilePCM.ui4_file_split_cnt++;
    }
    rSndFilePCM.ui4_send_cnt = 0; 
    fseek(fp, 0, SEEK_SET);

    while (rSndFilePCM.b_play_file)
    {
        SND_LOG("_snd_send_pcm_thread begine while(rSndFilePCM.b_play_file = %d) .... \n", (int)rSndFilePCM.b_play_file);
        e_ret = IMtkPb_Snd_GetClipBuf(rSndFilePCM.hpb_handle,
                                       &pt_buf,
                                       (uint32_t*)&ui4_strm_size);
        if(e_ret != IMTK_PB_ERROR_CODE_OK)
        {
            SND_LOG("_snd_send_pcm_thread get buffer failed ret = %d, wait for trying again! \n", (int)e_ret);
            CMPB_THREAD_DELAY(100);
            continue;
        }
        SND_LOG("_snd_send_pcm_thread begine get clip buffer ok .... \n");

        if (pt_buf)
        {
            ui4_read_cnt = fread( pt_buf,
                                  1,
                                  ui4_strm_size,
                                  fp);
            SND_LOG("_snd_send_pcm_thread  data size need read is %d, ui4_read_cnt = %d \n", 
                (int32_t)ui4_strm_size, (int32_t)ui4_read_cnt);            
            
            if (ui4_read_cnt < ui4_strm_size)
            {
                if(rSndFilePCM.ui4_rpt_cnt > 0)
                {
                    SND_LOG("_snd_send_pcm_thread play end do repeate now.... \n");
                    fseek(fp, 0, SEEK_SET);
                    if(rSndFilePCM.ui4_rpt_cnt < 0xFF) /*if >= 0xFF, loop forever, till user stop*/
                    {
                        rSndFilePCM.ui4_rpt_cnt --;
                    }
                }
                else
                {                    
                    rSndFilePCM.b_play_file = 0;
                    //rSndFilePCM.b_play_finish = TRUE;
                    //break;
                }
            }

            if(ui4_read_cnt != 0)
            {
                SND_LOG("_snd_send_pcm_thread send clip .... \n");
                IMtkPb_Snd_SendClip (rSndFilePCM.hpb_handle,
                                     pt_buf,
                                     ui4_read_cnt,
                                     0);
            }

            SND_LOG("_snd_send_pcm_thread send buffer ok, read_cnt = %d .... \n", (int)ui4_read_cnt);

            if(!rSndFilePCM.b_play_file)
            {
                SND_LOG("_snd_send_pcm_thread play finished, exit now .... \n");
                break;
            }
            
        }
        else
        {
            rSndFilePCM.b_play_file = 0;
            break;
        }
    }

    while(pt_inst->t_aud_clip_buf.ui1_free_buf_cnt < DRIVER_BUFF_COUNT) //wait for all clip done event received
    {
        SND_LOG("_snd_send_pcm_thread  wait for all clip done event received, _free_buf_cnt = %d \n",
                  pt_inst->t_aud_clip_buf.ui1_free_buf_cnt);
        CMPB_THREAD_DELAY(100);
    }
    
    rSndFilePCM.b_play_finish = TRUE;

    if(rSndFilePCM.pf_send_file_callback && (rSndFilePCM.ui4_rpt_cnt < 0xFF)/*if loop forever, do not send clip done to app*/)
    {
        SND_LOG("_snd_file_callback send clip done event to AP, rSndFilePCM.ui4_rpt_cnt = %d .... \n", (int)rSndFilePCM.ui4_rpt_cnt);
        rSndFilePCM.pf_send_file_callback(rSndFilePCM.hpb_handle, IMTKPB_SND_CLIP_COND_CLIP_DONE, pt_inst->pv_tag, 0, 0);
    }

    SND_LOG("_snd_send_pcm_thread  exit .... \n");
    
    fclose (fp);
}

static IMTK_PB_CB_ERROR_CODE_T _snd_file_callback(IMTK_PB_HANDLE_T         hHandle,
                                             IMTKPB_SND_CLIP_COND_T      eSndCond,
                                             void*                       pvTag,
                                             uint32_t                    u4Data,
                                             uint32_t                    u4ClipTag)
{
    SND_LOG("_snd_file_callback received event eSndCond = %d .... \n",eSndCond);
        
    if (IMTKPB_SND_CLIP_COND_CLIP_DONE == eSndCond)
    {
        UINT32              ui4_i;
        CMPB_SND_INST_T*    pt_inst;
        
        SND_LOG("_snd_file_callback IMTKPB_SND_CLIP_COND_CLIP_DONE.... \n");
        if ((hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
        {
            ui4_i = hHandle&0x0000FFFF;
            if (ui4_i < CMPB_SND_COUNT)
            {
                pt_inst = &at_snd_inst[ui4_i];
            }
            else
            {
                SND_LOG("_snd_file_callback IMTK_PB_CB_ERROR_CODE_NOT_OK.... \n");
                return IMTK_PB_CB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            SND_LOG("_snd_file_callback IMTK_PB_CB_ERROR_CODE_NOT_OK  1.... \n");
            return IMTK_PB_CB_ERROR_CODE_NOT_OK;
        }
        
        if(pt_inst != NULL)
        {
            SND_LOG("_snd_file_callback received clip done event, free count is %d, b_paly_file = %d, play_finish = %d .... \n",
                (int)pt_inst->t_aud_clip_buf.ui1_free_buf_cnt, (int)rSndFilePCM.b_play_file, (int)rSndFilePCM.b_play_finish);
            //if(!rSndFilePCM.b_play_file && (pt_inst->t_aud_clip_buf.ui1_free_buf_cnt == DRIVER_BUFF_COUNT)) /*play completed*/
            //if(!rSndFilePCM.b_play_file && rSndFilePCM.b_play_finish) /*play completed*/
            rSndFilePCM.ui4_send_cnt ++;
            SND_LOG("_snd_file_callback, rSndFilePCM.ui4_rpt_cnt = %d,  rSndFilePCM.ui4_file_split_cnt = %d .... \n", 
                        (int)rSndFilePCM.ui4_rpt_cnt,  (int)rSndFilePCM.ui4_file_split_cnt);
            if(rSndFilePCM.pf_send_file_callback && 
               ((rSndFilePCM.ui4_send_cnt % rSndFilePCM.ui4_file_split_cnt) == 0))
            {
                SND_LOG("_snd_file_callback send clip done event to AP, ui4_rpt_cnt = %d, pvTag = %x .... \n", 
                    (int)rSndFilePCM.ui4_rpt_cnt, (unsigned int)pvTag);
                rSndFilePCM.pf_send_file_callback(hHandle, IMTKPB_SND_CLIP_COND_LOOP_DONE, pvTag, u4Data, u4ClipTag);
                //rSndFilePCM.b_play_finish = TRUE;
            }
        }
    }

    return IMTK_PB_CB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Snd_SendPCM_From_URL (IMTK_PB_HANDLE_T   hHandle,
                                                               CHAR*           szFilePath,
                                                               IMTK_SND_FILE_PARM_T*    prParm)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;
    UINT32              ui4_file_path_len = 0;


    SND_LOG("IMtkPb_Snd_SendPCM_From_URL(%s), handle = %x .... \n", szFilePath, hHandle);

    if(!szFilePath)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    ui4_file_path_len = CMPB_STRLEN(szFilePath);
    if(ui4_file_path_len == 0 || ui4_file_path_len > 256)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if(rSndFilePCM.b_play_file || !rSndFilePCM.b_play_finish) //already played
    {
        SND_LOG("Error!!! to call IMtkPb_Snd_SendPCM_From_URL(), already played! \n");
        //return IMTK_PB_ERROR_CODE_NOT_OK;
    }

    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                HANDLE_T h_thread;

                CMPB_MEMSET(&rSndFilePCM, 0, sizeof(rSndFilePCM));
                CMPB_STRCPY(rSndFilePCM.sz_pcm_file_path, szFilePath);
                
                rSndFilePCM.b_play_file   = TRUE;
                rSndFilePCM.b_play_finish = FALSE;
                rSndFilePCM.hpb_handle    = hHandle; 
                
                rSndFilePCM.pf_send_file_callback = at_snd_inst[ui4_i].pf_callback;
                at_snd_inst[ui4_i].pf_callback = _snd_file_callback;
                
                if(prParm)
                {
                    rSndFilePCM.ui4_rpt_cnt = prParm->u1RptCnt;
                }
                else
                {
                    rSndFilePCM.ui4_rpt_cnt = 0;
                }                

                CMPB_THREAD_CREATE(&h_thread,
                                "_cmpb_push",
                                8192,
                                128,
                                _snd_send_pcm_thread,
                                sizeof(UINT32 *),
                                (void*)0);
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
    SND_LOG("enter IMtkPb_Snd_SendPCM_From_URL(%s) ok\n", szFilePath);
    return IMTK_PB_ERROR_CODE_OK;
}
/***************end add for send file from URL*********************/

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_GetClipBuf(IMTK_PB_HANDLE_T  hHandle,
                                           uint8_t**         ppau1SndBuf,
                                           uint32_t*         pu4BufSize)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;

    if((ppau1SndBuf == NULL) || (pu4BufSize == NULL))
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if((hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (pt_inst != NULL)
    {
        if(pt_inst->t_aud_clip_buf.ui1_free_buf_cnt == 0)
        {
            *ppau1SndBuf = NULL;
            *pu4BufSize  = 0;
            return IMTK_PB_ERROR_CODE_GET_BUF_PENDING;
        }
        else
        {
            pt_inst->t_aud_clip_buf.ui1_free_buf_cnt--;
            *ppau1SndBuf = pt_inst->t_aud_clip_buf.at_snd_buf[(pt_inst->t_aud_clip_buf.ui1_cur_buf_idx++) % DRIVER_BUFF_COUNT].pv_vir_addr;
            *pu4BufSize  = DRIVER_BUFF_SIZE;
            return IMTK_PB_ERROR_CODE_OK;
        }
    }

    return IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_SendClip (IMTK_PB_HANDLE_T  hHandle,
                                          uint8_t*          au1SndBuf,
                                          uint32_t          u4SndSize,
                                          uint32_t          u4ClipTag)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;

    if(au1SndBuf == NULL)
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                UINT8           ui1_idx = 0;
                SCC_AUD_CLIP_T t_aud_clip;

                if(rSndFilePCM.b_play_file)
                {
                    for(ui1_idx = 0; ui1_idx < DRIVER_BUFF_COUNT; ui1_idx++)
                    {
                        if(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_vir_addr == (void*)au1SndBuf)
                        {
                            au1SndBuf = (uint8_t*)(pt_inst->t_aud_clip_buf.at_snd_buf[ui1_idx].pv_phy_addr);
                            break;
                        }
                    }
                    if(ui1_idx == DRIVER_BUFF_COUNT)
                    {
                        CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                        return IMTK_PB_ERROR_CODE_INV_ARG;
                    }
                }
                else if(NULL == au1SndBuf) //use 3rd buffer directly
                {
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_INV_ARG;
                }

                t_aud_clip.e_type = SCC_AUD_CLIP_TYPE_CLIP_MIXSOUND;
                t_aud_clip.ui4_out_port_mask = SCC_AUD_OUT_PORT_MASK_SPEAKER;    /* Speaker only */
                t_aud_clip.ui4_ch_mask = ~0;
                t_aud_clip.ui4_repeat_cnt = 1;

                t_aud_clip.u.t_clip_buff.e_fmt = ASH_AUDIO_FMT_PCM;

                t_aud_clip.u.t_clip_buff.e_PCM_channel_type = (SCC_AUD_CHANNEL_TYPE_T)_cmpb_map_scc_channels(at_snd_inst[ui4_i].tPcmInfo.eNumCh);
                t_aud_clip.u.t_clip_buff.pui1_clip_buff = au1SndBuf;
                t_aud_clip.u.t_clip_buff.z_clip_buff_size = u4SndSize;
                t_aud_clip.u.t_clip_buff.ui1_PCM_BitDepth = _cmpb_map_bit_depth(at_snd_inst[ui4_i].tPcmInfo.eBitsPerSample);
                t_aud_clip.u.t_clip_buff.ui4_PCM_SampleRate = _cmpb_map_sample_rate(at_snd_inst[ui4_i].tPcmInfo.eSampleRate);
                
                i4_ret = CMPB_C_SCC_COMP_AUD_SET_CLIP( at_snd_inst[ui4_i].h_scc,
                                             &t_aud_clip,
                                             _snd_nfy_fct,
                                             (VOID*)hHandle);
                if (0 != i4_ret)
                {
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_NOT_OK;
                }
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
    return IMTK_PB_ERROR_CODE_OK;
}


IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Play (IMTK_PB_HANDLE_T  hHandle)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;

    SND_LOG("enter IMtkPb_Snd_Play() handle = %x ...\n", hHandle);
    
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                if (at_snd_inst[ui4_i].b_play)
                {
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_DRV_ERROR;
                }
                else
                {
                    at_snd_inst[ui4_i].b_ctrl_done = FALSE;
                    i4_ret = CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL(at_snd_inst[ui4_i].h_scc, SCC_AUD_CTRL_MODE_PLAY_MIXSND);
                    if (i4_ret != SMR_OK)
                    {
                        //return IMTK_PB_ERROR_CODE_NOT_OK;
                    }

                    at_snd_inst[ui4_i].b_play = TRUE;
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    SND_LOG("enter IMtkPb_Snd_Play() ok\n");
                    return IMTK_PB_ERROR_CODE_OK;
                }
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Stop (IMTK_PB_HANDLE_T  hHandle)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;

    SND_LOG("IMtkPb_Snd_Stop() hHandle = %x ....\n", hHandle);
    
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            SND_LOG("IMtkPb_Snd_Stop() fail ui4_i >= CMPB_SND_COUNT....\n");
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        SND_LOG("IMtkPb_Snd_Stop() fail invalied handle = %x ...\n", hHandle);
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
    
    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                SND_LOG("IMtkPb_Snd_Stop() lock fail...\n");
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                if (!at_snd_inst[ui4_i].b_play)
                {
                    SND_LOG("IMtkPb_Snd_Stop() fail IMTK_PB_ERROR_CODE_DRV_ERROR ...\n");
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_DRV_ERROR;
                }
                else
                {
                    if(rSndFilePCM.b_play_file)  /*add for play pcm file*/
                    {
                        rSndFilePCM.b_play_file = FALSE;
                        CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                        while(!rSndFilePCM.b_play_finish)
                        {
                            SND_LOG("wait for snd file play finished....\n");
                            CMPB_THREAD_DELAY(100);
                        }
                        i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
                        if (i4_ret != OSR_OK)
                        {
                            SND_LOG("snd stop() CMPB_SEMA_LOCK() fail i4_ret = %d ...\n", (int)i4_ret);
                            return IMTK_PB_ERROR_CODE_NOT_OK;
                        }
                    }
                    
#if 1   /*use sync stop instead*/
                    at_snd_inst[ui4_i].b_ctrl_done = TRUE;
                    i4_ret = CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL(at_snd_inst[ui4_i].h_scc, SCC_AUD_CTRL_MODE_STOP_SYNC);
                    if (0 != i4_ret)
                    {
                        SND_LOG("IMtkPb_Snd_Stop() fail set clip control fail = %d ...\n", (int)i4_ret);
                        CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                        return IMTK_PB_ERROR_CODE_DRV_ERROR;
                    }
                    else
                    {
                        at_snd_inst[ui4_i].b_play = FALSE;
                        CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                        return IMTK_PB_ERROR_CODE_OK;
                    }
                    
#else
                    at_snd_inst[ui4_i].b_ctrl_done = FALSE;
                    CMPB_C_SCC_COMP_AUD_SET_CLIP_CTRL(at_snd_inst[ui4_i].h_scc, SCC_AUD_CTRL_MODE_STOP);

                    while (!at_snd_inst[ui4_i].b_ctrl_done)
                    {
                        CMPB_THREAD_DELAY(10);
                    }

                    if (at_snd_inst[ui4_i].b_ctrl_error)
                    {
                        CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                        return IMTK_PB_ERROR_CODE_DRV_ERROR;
                    }
                    else
                    {
                        at_snd_inst[ui4_i].b_play = FALSE;
                        CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                        return IMTK_PB_ERROR_CODE_OK;
                    }
#endif
                }
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                SND_LOG("IMtkPb_Snd_Stop() fail playing not active ...\n");
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            SND_LOG("IMtkPb_Snd_Stop() fail not init ...\n");
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        SND_LOG("IMtkPb_Snd_Stop() fail pt_inst == NULL ...\n");
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_SetVolume (uint8_t   ui1_volume_value)
{
    INT32                 i4_ret  = 0;
    UINT8                 ui1_i   = 0;
    SM_COMMAND_T          at_sm_cmds[5];
    SCC_AUD_VOLUME_INFO_T t_volume_info;

    c_memset(&t_volume_info,0,sizeof(SCC_AUD_VOLUME_INFO_T));

    t_volume_info.e_ch       = SCC_AUD_CHANNEL_ALL   ;
    t_volume_info.e_out_port = SCC_AUD_OUT_PORT_SPEAKER;
    
    t_volume_info.ui1_volumn = ui1_volume_value > 100 ? 100 : ui1_volume_value;
    
    
    at_sm_cmds[ui1_i].e_code        = SCC_CMD_CODE_GRP_TYPE;
    at_sm_cmds[ui1_i++].u.ui4_data  = SCC_CMD_TYPE_AUDIO;
    at_sm_cmds[ui1_i].e_code        = SCC_CMD_CODE_NAME;
    at_sm_cmds[ui1_i++].u.ps_text   = SN_PRES_AUX_SOUND;
    at_sm_cmds[ui1_i].e_code        = SCC_CMD_CODE_AUDIO_CLIP_VOLUME;
    at_sm_cmds[ui1_i++].u.pv_data   = (VOID*)&t_volume_info;
    at_sm_cmds[ui1_i].e_code        = SM_CMD_CODE_END;
    at_sm_cmds[ui1_i++].u.ui4_data  = 0;
        
    i4_ret  = CMPB_C_SCC_COMP_GRP_SET(at_sm_cmds);  
    SND_LOG("IMtkPb_Snd_SetVolume() fail %d,value %d\n",(int)i4_ret,ui1_volume_value);
    return (i4_ret == SMR_OK) ? IMTK_PB_ERROR_CODE_OK : IMTK_PB_ERROR_CODE_NOT_OK;
}

IMTK_PB_ERROR_CODE_T IMtkPb_Snd_Close (IMTK_PB_HANDLE_T     hHandle)
{
    UINT32              ui4_i;
    CMPB_SND_INST_T*    pt_inst;
    INT32               i4_ret;

    SND_LOG("enter IMtkPb_Snd_Close() handle = %x ...\n", (unsigned int)hHandle);
    if ( (hHandle&0xFFFF0000) == CMPB_HANDLE_TAG )
    {
        ui4_i = hHandle&0x0000FFFF;
        if (ui4_i < CMPB_SND_COUNT)
        {
            pt_inst = &at_snd_inst[ui4_i];
        }
        else
        {
            return IMTK_PB_ERROR_CODE_INV_ARG;
        }
    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }

    CMPB_MEMSET(&rSndFilePCM, 0, sizeof(rSndFilePCM)); /*clear parameter for snd file*/
    rSndFilePCM.b_play_finish = 1;

    if (pt_inst)
    {
        if (at_snd_inst[ui4_i].h_sema != NULL_HANDLE)
        {
            i4_ret = CMPB_SEMA_LOCK(at_snd_inst[ui4_i].h_sema, X_SEMA_OPTION_WAIT);
            if (i4_ret != OSR_OK)
            {
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }

            if (at_snd_inst[ui4_i].b_active)
            {
                _cmpb_aud_clip_free_buf(hHandle);
                if (at_snd_inst[ui4_i].b_play)
                {
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_DRV_ERROR;
                }
                else
                {
                    CMPB_C_SCC_COMP_CLOSE(at_snd_inst[ui4_i].h_scc);
                    
                    at_snd_inst[ui4_i].h_scc = NULL_HANDLE;
                    at_snd_inst[ui4_i].b_play = FALSE;
                    at_snd_inst[ui4_i].pf_callback = NULL;
                    at_snd_inst[ui4_i].pv_tag = NULL;
                    at_snd_inst[ui4_i].b_active = FALSE;
                    
                    CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                    return IMTK_PB_ERROR_CODE_OK;
                }
            }
            else
            {
                CMPB_SEMA_UNLOCK(at_snd_inst[ui4_i].h_sema);
                return IMTK_PB_ERROR_CODE_NOT_OK;
            }
        }
        else
        {
            return IMTK_PB_ERROR_CODE_NOT_INIT;
        }

    }
    else
    {
        return IMTK_PB_ERROR_CODE_INV_ARG;
    }
}
