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
 * $RCSfile: mheg5_os.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all os related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifdef MHEG5_IC_SUPPORT
#include "dbg/x_dbg.h"
#include "dbg/u_dbg.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"
#include "dbg/def_dbg_level_mw.h"
#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "handle/u_handle.h"
#include "os/inc/x_os.h"
#include "os/inc/u_os.h"
#include "cmpb/cmpb_util.h"
#include "cmpb/IMtkPb_ErrorCode.h"
#include "cmpb/IMtkPb_Ctrl.h"
#include "cmpb/IMtkPb_Ctrl_DTV.h"
#include "svctx/mm_hdlr/u_mm_hdlr.h"

#include "mheg5/u_mheg5.h"
#include "mheg5/x_mheg5.h"
#include "mheg5/OceanBlue/glue/mheg5_OceanBlue_eng.h"
#include "mheg5/OceanBlue/glue/strm_ctrl/sct.h"
#include "mheg5/OceanBlue/glue/gui_render/grd.h"


/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_errors.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_ic.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_audio.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_video.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_graphics.h"

#include "mheg5/OceanBlue/glue/ic/mheg5_ic.h"

/*-----------------------------------------------------------------------------
                    macro, structure, enum declarations
----------------------------------------------------------------------------*/
typedef struct _MHEG5_ICS_AUDIO_INFO_T
{
    UINT8 aui1_lang[4];
    UINT8 ui1_audio_type;
} MHEG5_ICS_AUDIO_INFO_T;

typedef struct _MHEG5_ICS_SUBTITLING_INFO_T
{
    UINT8  aui1_lang[4];
    UINT8  ui1_subtitling_type;
    UINT16 ui2_composition_page_id;
    UINT16 ui2_ancillary_page_id;
    IMTK_PB_CTRL_DVB_SBTL_TYPE_T e_sys_type;
} MHEG5_ICS_SUBTITLING_INFO_T;


typedef struct _MHEG5_ICS_PMT_COMPONENT_T
{
    UINT16 ui2_pmt_idx;
    UINT8  ui1_stream_type;
    UINT16 ui2_elementary_pid;
    UINT8  ui1_component_tag;
    UINT16 ui2_num_audio;
    UINT16 ui2_num_subtilting;
    MHEG5_ICS_AUDIO_INFO_T*      pt_audio_info;
    MHEG5_ICS_SUBTITLING_INFO_T* pt_subtitling_info;
} MHEG5_ICS_PMT_COMPONENT_T;


typedef struct _MHEG5_ICS_PMT_T
{
    UINT16 ui2_program_number;
    UINT16 ui2_pcr_pid;
    UINT16 ui2_num_components;
    MHEG5_ICS_PMT_COMPONENT_T* pt_component;
} MHEG5_ICS_PMT_T;

typedef enum _MHEG5_ICS_STATE_T
{
    MHEG5_ICS_STATE_STOPPED = 0,
    MHEG5_ICS_STATE_STARTED = 1,
    MHEG5_ICS_STATE_PAUSED  = 2,

} MHEG5_ICS_STATE_T;


typedef struct _MHEG5_ICS_INFO_T
{
    MHEG5_ICS_STATE_T e_state;
    MHEG5_ICS_PMT_T*  pt_ics_pmt;
    IMTK_PB_HANDLE_T  hPB_Handle;
    tmMHEG5ICSKeys_t* pt_encrypt_keys;
    tmMHEG5ICSComponents_t t_comp;
    BOOL              b_eos;
    BOOL              b_comp_inited;
    BOOL              b_underflow;
    BOOL              b_start_notified;
    UINT32            ui4_cur_time;
    UINT64            ui8_cur_pos;

    MPEG_2_PID_T      t_a_pid;/* audio */
    MPEG_2_PID_T      t_v_pid;/* video */
    MPEG_2_PID_T      t_s_pid;/* subtitle */

    MHEG5_SBTL_STATE_T e_sbtl_state;
} MHEG5_ICS_INFO_T;


#define MHEG5_ICS_LOCK()        \
    x_sema_lock(_h_ics_sema, X_SEMA_OPTION_WAIT)

#define MHEG5_ICS_UNLOCK()      \
    x_sema_unlock(_h_ics_sema)

#define MHEG5_ICS_WAIT_EOS      1
#define MHEG5_ICS_PAUSE_CLOSE   0

#ifdef CLI_LVL_ALL
#ifdef  DBG_LEVEL_MODULE
#undef  DBG_LEVEL_MODULE
#endif

#define DBG_LEVEL_MODULE            mheg5_ic_get_dbg_level()

static UINT16    _ui2_mheg5_ic_dbg_level = DBG_INIT_LEVEL_MW_MHEG5;
static UINT32    g_ui4_data_trace = 0;
#endif
/*-----------------------------------------------------------------------------
                    data declarations
----------------------------------------------------------------------------*/
static volatile MHEG5_ICS_INFO_T _t_ics_info = 
{   MHEG5_ICS_STATE_STOPPED, 
    NULL, 
    IMTK_NULL_HANDLE, 
    NULL, 
    {0,0,0,0,MHEG5_VIDEO_TERMINATION_DISAPPEAR}, 
    FALSE, 
    FALSE, 
    FALSE,
    FALSE,
    (UINT32)0,
    (UINT64)0,
    MPEG_2_NULL_PID,
    MPEG_2_NULL_PID,
    MPEG_2_NULL_PID,
    MHEG5_SBTL_STOP
};
static HANDLE_T _h_ics_sema = NULL_HANDLE;
static tmMHEG5ICStatus_t _t_ic_status = MHEG5_IC_STATUS_DISABLED;
static x_mheg5_ics_guid_visibility_nfy _pf_prompt_guid_nfy = NULL;
static VOID* _pv_prompt_guid_nfy_tag  = NULL;
static CHAR* _ps_pfg_restriction = NULL;
static x_mheg5_pin_support_fct  _pf_pin_support_fct = NULL;
static VOID*   _pv_pin_support_fct_tag = NULL;


/*-----------------------------------------------------------------------------
                    functions prototypes
 ----------------------------------------------------------------------------*/
static INT32 _mheg5_ics_get_pmt(
    UINT8*             pui1_pmt_sec, 
    MHEG5_ICS_PMT_T**  ppt_ics_pmt);

static VOID _mheg5_ics_rel_pmt(
    MHEG5_ICS_PMT_T*   pt_ics_pmt);

static INT32 _mheg5_ics_copy_keys(
    tmMHEG5ICSKeys_t*  pt_src_encrypt_keys, 
    tmMHEG5ICSKeys_t** ppt_dst_encrypt_keys);

static VOID _mheg5_ics_free_keys(
    tmMHEG5ICSKeys_t*  pt_encrypt_keys);

static UINT16 _mheg5_ics_comp_tag2pid(
    STREAM_TYPE_T      e_strm_type,
    INT32              i4_component_tag, 
    MHEG5_ICS_PMT_T*   pt_ics_pmt);

static VOID _mheg5_ics_switch_av_path(
    STREAM_TYPE_T      e_strm_type,
    BOOL               b_main_path);

static IMTK_PB_CTRL_DVB_SBTL_TYPE_T _mheg5_ics_dvb_subtitle_type (
    UINT8              ui1_type);

static VOID _mheg5_ics_get_proper_subtitle_info(
    IMTK_PB_CTRL_SUBTITLE_INFO_T* pt_subtitling_info,
    MHEG5_ICS_PMT_T*   pt_ics_pmt);

static VOID _mheg5_ics_nfy_play_started(VOID);

static VOID _mheg5_ics_nfy_play_stopped(VOID);

static IMTK_PB_CB_ERROR_CODE_T _mheg5_ics_pb_cb(
    IMTK_PB_CTRL_EVENT_T       eEventType,
    void*                      pvTag,
    uint32_t                   u4Data);


/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/
static VOID _mheg5_ics_rel_pmt(MHEG5_ICS_PMT_T* pt_ics_pmt)
{
    if (pt_ics_pmt != NULL)
    {
        if (pt_ics_pmt->pt_component != NULL)
        {
            UINT16 ui2_i;

            for (ui2_i = 0; ui2_i < pt_ics_pmt->ui2_num_components; ui2_i++)
            {
                if (pt_ics_pmt->pt_component[ui2_i].pt_audio_info != NULL)
                {
                    x_mem_free(pt_ics_pmt->pt_component[ui2_i].pt_audio_info);
                    pt_ics_pmt->pt_component[ui2_i].pt_audio_info = NULL;
                }

                if (pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info != NULL)
                {
                    x_mem_free(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info);
                    pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info = NULL;
                }
            }

            x_mem_free(pt_ics_pmt->pt_component);
            pt_ics_pmt->pt_component = NULL;
        }

        x_mem_free(pt_ics_pmt);
        pt_ics_pmt = NULL;
    }
}

static INT32 _mheg5_ics_get_pmt(UINT8* pui1_pmt_sec, MHEG5_ICS_PMT_T** ppt_ics_pmt)
{
    MHEG5_ICS_PMT_T* pt_ics_pmt = NULL;
    UINT16          ui2_tmp;
    UINT16          ui2_sec_len;
    UINT16          ui2_i1, ui2_i2, ui2_N1, ui2_N2;
    BOOL            b_error_exit = FALSE;

    if (ppt_ics_pmt == NULL || pui1_pmt_sec == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    *ppt_ics_pmt = NULL;

    /* sanity check */
    if (pui1_pmt_sec[0] != 0x02)
    {
        return MHEG5R_INV_ARG;
    }

    pt_ics_pmt = x_mem_calloc(1, sizeof(MHEG5_ICS_PMT_T));
    if (pt_ics_pmt == NULL)
    {
        return MHEG5R_OUT_OF_MEMORY;
    }

    ui2_sec_len = pui1_pmt_sec[1];
    ui2_sec_len = ((ui2_sec_len << 8) + pui1_pmt_sec[2]) & 0x0FFF;

    pt_ics_pmt->ui2_program_number = pui1_pmt_sec[3];
    pt_ics_pmt->ui2_program_number = (pt_ics_pmt->ui2_program_number << 8) + pui1_pmt_sec[4];

    pt_ics_pmt->ui2_pcr_pid = pui1_pmt_sec[8];
    pt_ics_pmt->ui2_pcr_pid = ((pt_ics_pmt->ui2_pcr_pid << 8) + pui1_pmt_sec[9]) & 0x1FFF;

    ui2_tmp = pui1_pmt_sec[10];
    ui2_tmp = ((ui2_tmp << 8) + pui1_pmt_sec[11]) & 0x0FFF;

    ui2_N1 = ui2_sec_len - 9 - ui2_tmp - 4;

    ui2_tmp += 12;

    pui1_pmt_sec += ui2_tmp;

    for (ui2_i1 = 0; ui2_i1 < ui2_N1; )
    {
        MHEG5_ICS_PMT_COMPONENT_T* pt_pmt_comp = NULL;
        
        pt_ics_pmt->ui2_num_components ++;
        pt_ics_pmt->pt_component = x_mem_realloc(pt_ics_pmt->pt_component, (pt_ics_pmt->ui2_num_components)*(sizeof(MHEG5_ICS_PMT_COMPONENT_T)));
        if (pt_ics_pmt->pt_component != NULL)
        {
            pt_pmt_comp = &(pt_ics_pmt->pt_component[pt_ics_pmt->ui2_num_components - 1]);
            x_memset(pt_pmt_comp, 0, sizeof(MHEG5_ICS_PMT_COMPONENT_T));

            pt_pmt_comp->ui2_pmt_idx = pt_ics_pmt->ui2_num_components - 1;/* Fixme: start from 0 or 1? */
            pt_pmt_comp->ui1_stream_type = pui1_pmt_sec[ui2_i1++];
            pt_pmt_comp->ui2_elementary_pid = pui1_pmt_sec[ui2_i1++];
            pt_pmt_comp->ui2_elementary_pid = ((pt_pmt_comp->ui2_elementary_pid << 8) + pui1_pmt_sec[ui2_i1++]) & 0x1FFF;

            ui2_N2 = pui1_pmt_sec[ui2_i1++];
            ui2_N2 = ((ui2_N2 << 8) + pui1_pmt_sec[ui2_i1++]) & 0x0FFF;

            for (ui2_i2 = 0; ui2_i2 < ui2_N2; )
            {
                if (pui1_pmt_sec[ui2_i1] == 0x0A)
                {
                    /* ISO_639_language_descriptor */
                    UINT8 ui1_i, ui1_N;
                    ui1_N = pui1_pmt_sec[ui2_i1 + 1];
                    ui2_i1 += 2;
                    ui2_i2 += 2;
                    
                    for (ui1_i = 0; ui1_i < ui1_N; )
                    {
                        MHEG5_ICS_AUDIO_INFO_T* pt_audio_info = NULL;
                        
                        pt_pmt_comp->ui2_num_audio ++;
                        pt_pmt_comp->pt_audio_info = x_mem_realloc(pt_pmt_comp->pt_audio_info, (pt_pmt_comp->ui2_num_audio)*(sizeof(MHEG5_ICS_AUDIO_INFO_T)));
                        if (pt_pmt_comp->pt_audio_info != NULL)
                        {
                            pt_audio_info = &(pt_pmt_comp->pt_audio_info[pt_pmt_comp->ui2_num_audio - 1]);

                            pt_audio_info->aui1_lang[0] = pui1_pmt_sec[ui2_i1++];
                            pt_audio_info->aui1_lang[1] = pui1_pmt_sec[ui2_i1++];
                            pt_audio_info->aui1_lang[2] = pui1_pmt_sec[ui2_i1++];
                            pt_audio_info->aui1_lang[3] = '\0';

                            pt_audio_info->ui1_audio_type = pui1_pmt_sec[ui2_i1++];
                        }
                        else
                        {
                            b_error_exit = TRUE;
                            break;
                        }
                        ui1_i += 4;
                        ui2_i2 += 4;
                    }

                    if (b_error_exit == TRUE)
                    {
                        break;
                    }
                }
                else if (pui1_pmt_sec[ui2_i1] == 0x52)
                {
                    /* stream_identifier_descriptor */
                    pt_pmt_comp->ui1_component_tag = pui1_pmt_sec[ui2_i1 + 2];
                    ui2_i1 += 3;
                    ui2_i2 += 3;
                }
                else if (pui1_pmt_sec[ui2_i1] == 0x59)
                {
                    /* subtitling descriptor */
                    UINT8 ui1_i, ui1_N;
                    ui1_N = pui1_pmt_sec[ui2_i1 + 1];
                    ui2_i1 += 2;
                    ui2_i2 += 2;

                    for (ui1_i = 0; ui1_i < ui1_N; )
                    {
                        MHEG5_ICS_SUBTITLING_INFO_T* pt_subtitling_info = NULL;
                        
                        pt_pmt_comp->ui2_num_subtilting ++;
                        pt_pmt_comp->pt_subtitling_info = x_mem_realloc(pt_pmt_comp->pt_subtitling_info, (pt_pmt_comp->ui2_num_subtilting)*(sizeof(MHEG5_ICS_SUBTITLING_INFO_T)));
                        if (pt_pmt_comp->pt_subtitling_info != NULL)
                        {
                            pt_subtitling_info = &(pt_pmt_comp->pt_subtitling_info[pt_pmt_comp->ui2_num_subtilting - 1]);

                            pt_subtitling_info->aui1_lang[0] = pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->aui1_lang[1] = pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->aui1_lang[2] = pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->aui1_lang[3] = '\0';

                            pt_subtitling_info->ui1_subtitling_type = pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->ui2_composition_page_id = pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->ui2_composition_page_id = (pt_subtitling_info->ui2_composition_page_id << 8) + pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->ui2_ancillary_page_id = pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->ui2_ancillary_page_id = (pt_subtitling_info->ui2_ancillary_page_id << 8) + pui1_pmt_sec[ui2_i1++];
                            pt_subtitling_info->e_sys_type = _mheg5_ics_dvb_subtitle_type(pt_subtitling_info->ui1_subtitling_type);
                        }
                        else
                        {
                            b_error_exit = TRUE;
                            break;
                        }
                        ui1_i += 8;
                        ui2_i2 += 8;
                    }
                    
                    if (b_error_exit == TRUE)
                    {
                        break;
                    }
                }
                else
                {
                    ui2_tmp = pui1_pmt_sec[ui2_i1 + 1] + 2;
                    ui2_i1 += ui2_tmp;
                    ui2_i2 += ui2_tmp;
                }
            }
            if (b_error_exit == TRUE)
            {
                break;
            }
        }
        else
        {
            b_error_exit = TRUE;
            break;
        }
    }

    if (b_error_exit == TRUE)
    {
        _mheg5_ics_rel_pmt(pt_ics_pmt);
        return MHEG5R_OUT_OF_MEMORY;
    }
    else
    {
        *ppt_ics_pmt = pt_ics_pmt;
        return MHEG5R_OK;
    }    
}

static INT32 _mheg5_ics_copy_keys(tmMHEG5ICSKeys_t* pt_src_encrypt_keys, tmMHEG5ICSKeys_t** ppt_dst_encrypt_keys)
{
    tmMHEG5ICSKeys_t* pt_keys = NULL;
    
    if (ppt_dst_encrypt_keys == NULL)
    {
        return MHEG5R_INV_ARG;
    }

    if ((pt_src_encrypt_keys != NULL) && 
        (pt_src_encrypt_keys->num_pids != ((U16BIT)0)) &&
        (pt_src_encrypt_keys->pid_info != NULL))
    {
        pt_keys = x_mem_alloc(sizeof(tmMHEG5ICSKeys_t));
        if (pt_keys != NULL)
        {
            pt_keys->num_pids = pt_src_encrypt_keys->num_pids;

            pt_keys->pid_info = x_mem_alloc((pt_keys->num_pids) * sizeof(tmMHEG5ICSPidInfo_t));

            if (pt_keys->pid_info != NULL)
            {
                x_memcpy(pt_keys->pid_info, pt_src_encrypt_keys->pid_info, ((pt_keys->num_pids) * sizeof(tmMHEG5ICSPidInfo_t)));

                *ppt_dst_encrypt_keys = pt_keys;
                return MHEG5R_OK;
            }
            else
            {
                x_mem_free(pt_keys);
                pt_keys = NULL;
                
                *ppt_dst_encrypt_keys = NULL;
                return MHEG5R_OUT_OF_MEMORY;
            }
        }
        else
        {
            *ppt_dst_encrypt_keys = NULL;
            return MHEG5R_OUT_OF_MEMORY;
        }
    }
    else
    {
        *ppt_dst_encrypt_keys = NULL;
        return MHEG5R_OK;
    }
}

static VOID _mheg5_ics_free_keys(tmMHEG5ICSKeys_t* pt_encrypt_keys)
{
    if (pt_encrypt_keys != NULL)
    {
        
        if (pt_encrypt_keys->pid_info != NULL)
        {
            x_mem_free(pt_encrypt_keys->pid_info);
            pt_encrypt_keys->pid_info = NULL;
        }
        
        x_mem_free(pt_encrypt_keys);
        pt_encrypt_keys = NULL;
    }
}

static IMTK_PB_CTRL_DVB_SBTL_TYPE_T _mheg5_ics_dvb_subtitle_type (UINT8  ui1_type)
{
    IMTK_PB_CTRL_DVB_SBTL_TYPE_T  e_type;
    
    switch (ui1_type)
    {
        case 0x10:
            e_type = IMTK_PB_CTRL_DVB_SBTL_NO_ASP_RATIO;
            break;
            
        case 0x11:
            e_type = IMTK_PB_CTRL_DVB_SBTL_4_3;
            break;
            
        case 0x12:
            e_type = IMTK_PB_CTRL_DVB_SBTL_16_9;
            break;
            
        case 0x13:
            e_type = IMTK_PB_CTRL_DVB_SBTL_221_1;
            break;
            
        case 0x14:
            e_type = IMTK_PB_CTRL_DVB_SBTL_HD;
            break;
            
        case 0x20:
            e_type = IMTK_PB_CTRL_DVB_SBTL_NO_ASP_RATIO_HOH;
            break;
            
        case 0x21:
            e_type = IMTK_PB_CTRL_DVB_SBTL_4_3_HOH;
            break;
            
        case 0x22:
            e_type = IMTK_PB_CTRL_DVB_SBTL_16_9_HOH;
            break;
            
        case 0x23:
            e_type = IMTK_PB_CTRL_DVB_SBTL_221_1_HOH;
            break;
            
        case 0x24:
            e_type = IMTK_PB_CTRL_DVB_SBTL_HD_HOH;
            break;
            
        default:
            e_type = IMTK_PB_CTRL_DVB_SBTL_UNKNOWN;
            break;
    }
    
    return e_type;
}

static VOID _mheg5_ics_get_proper_subtitle_info(
    IMTK_PB_CTRL_SUBTITLE_INFO_T* pt_subtitling_info,
    MHEG5_ICS_PMT_T* pt_ics_pmt)
{
    if (pt_subtitling_info != NULL &&
        pt_ics_pmt != NULL)
    {
        MHEG5_SUBTITLE_CFG_T t_cfg;
        UINT16 ui2_i, ui2_j;
        BOOL   b_found = FALSE;
    
        if ((MHEG5R_OK == sct_get_sbtl_cfg_info(&t_cfg)) &&
            (TRUE == sct_get_subtitle_visibility()))
        {
            for (ui2_i = 0; (ui2_i < pt_ics_pmt->ui2_num_components)&&(b_found == FALSE); ui2_i++)
            {
                if (pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info != NULL)
                {
                    /* First language + Type*/
                    for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_subtilting; ui2_j++)
                    {
                        if ((x_strcmp(t_cfg.s639_lang_1st, (CHAR*)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang)) == 0) &&
                            (t_cfg.e_type == (DVB_SBTL_TYPE_T)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type)))
                        {
                            b_found = TRUE;
                            pt_subtitling_info->e_sbtl_type = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type;
                            pt_subtitling_info->s_lang[0] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[0];
                            pt_subtitling_info->s_lang[1] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[1];
                            pt_subtitling_info->s_lang[2] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[2];
                            pt_subtitling_info->s_lang[3] = '\0';
                            pt_subtitling_info->ui2_anci_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_ancillary_page_id;
                            pt_subtitling_info->ui2_comp_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_composition_page_id;
                            pt_subtitling_info->ui2_pid        = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                            pt_subtitling_info->ui2_pmt_index  = pt_ics_pmt->pt_component[ui2_i].ui2_pmt_idx;
                            pt_subtitling_info->h_gl_plane     = grd_get_sbtl_canvas();
                            break;
                        }
                    }
                    if (b_found == TRUE)
                    {
                        break;
                    }
                }
            }
            
            for (ui2_i = 0; (ui2_i < pt_ics_pmt->ui2_num_components)&&(b_found == FALSE); ui2_i++)
            {
                if (pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info != NULL)
                {
                    /* Second language + Type */
                    for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_subtilting; ui2_j++)
                    {
                        if ((x_strcmp(t_cfg.s639_lang_2nd, (CHAR*)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang)) == 0) &&
                            (t_cfg.e_type == (DVB_SBTL_TYPE_T)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type)))
                        {
                            b_found = TRUE;
                            pt_subtitling_info->e_sbtl_type = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type;
                            pt_subtitling_info->s_lang[0] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[0];
                            pt_subtitling_info->s_lang[1] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[1];
                            pt_subtitling_info->s_lang[2] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[2];
                            pt_subtitling_info->s_lang[3] = '\0';
                            pt_subtitling_info->ui2_anci_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_ancillary_page_id;
                            pt_subtitling_info->ui2_comp_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_composition_page_id;
                            pt_subtitling_info->ui2_pid        = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                            pt_subtitling_info->ui2_pmt_index  = pt_ics_pmt->pt_component[ui2_i].ui2_pmt_idx;
                            pt_subtitling_info->h_gl_plane     = grd_get_sbtl_canvas();
                            break;
                        }
                    }
                    if (b_found == TRUE)
                    {
                        break;
                    }
                }
            }
            
            for (ui2_i = 0; (ui2_i < pt_ics_pmt->ui2_num_components)&&(b_found == FALSE); ui2_i++)
            {
                if (pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info != NULL)
                {
                    /* Type */
                    for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_subtilting; ui2_j++)
                    {
                        if (t_cfg.e_type == (DVB_SBTL_TYPE_T)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type))
                        {
                            b_found = TRUE;
                            pt_subtitling_info->e_sbtl_type = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type;
                            pt_subtitling_info->s_lang[0] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[0];
                            pt_subtitling_info->s_lang[1] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[1];
                            pt_subtitling_info->s_lang[2] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[2];
                            pt_subtitling_info->s_lang[3] = '\0';
                            pt_subtitling_info->ui2_anci_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_ancillary_page_id;
                            pt_subtitling_info->ui2_comp_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_composition_page_id;
                            pt_subtitling_info->ui2_pid        = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                            pt_subtitling_info->ui2_pmt_index  = pt_ics_pmt->pt_component[ui2_i].ui2_pmt_idx;
                            pt_subtitling_info->h_gl_plane     = grd_get_sbtl_canvas();
                            break;
                        }
                    }
                    if (b_found == TRUE)
                    {
                        break;
                    }
                }
            }
            
            for (ui2_i = 0; (ui2_i < pt_ics_pmt->ui2_num_components)&&(b_found == FALSE); ui2_i++)
            {
                if (pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info != NULL)
                {
                    /* Nothing, try audio & GUI language */
                    {
                        /* receiver default */
                        ISO_639_LANG_T s639_lang_def;
                        ISO_639_LANG_T s639_lang_gui;
                        
                        sct_get_default_lang(s639_lang_def);
                        sct_get_gui_lang(s639_lang_gui);

                        
                        /* Default audio language */
                        for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_subtilting; ui2_j++)
                        {
                            if ((x_strcmp(s639_lang_def, (CHAR*)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang)) == 0))
                            {
                                b_found = TRUE;
                                pt_subtitling_info->e_sbtl_type = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type;
                                pt_subtitling_info->s_lang[0] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[0];
                                pt_subtitling_info->s_lang[1] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[1];
                                pt_subtitling_info->s_lang[2] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[2];
                                pt_subtitling_info->s_lang[3] = '\0';
                                pt_subtitling_info->ui2_anci_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_ancillary_page_id;
                                pt_subtitling_info->ui2_comp_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_composition_page_id;
                                pt_subtitling_info->ui2_pid        = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                                pt_subtitling_info->ui2_pmt_index  = pt_ics_pmt->pt_component[ui2_i].ui2_pmt_idx;
                                pt_subtitling_info->h_gl_plane     = grd_get_sbtl_canvas();
                                break;
                            }
                        }
                        if (b_found == TRUE)
                        {
                            break;
                        }
                        /* GUI language */
                        for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_subtilting; ui2_j++)
                        {
                            if ((x_strcmp(s639_lang_gui, (CHAR*)(pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang)) == 0))
                            {
                                b_found = TRUE;
                                pt_subtitling_info->e_sbtl_type = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type;
                                pt_subtitling_info->s_lang[0] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[0];
                                pt_subtitling_info->s_lang[1] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[1];
                                pt_subtitling_info->s_lang[2] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[2];
                                pt_subtitling_info->s_lang[3] = '\0';
                                pt_subtitling_info->ui2_anci_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_ancillary_page_id;
                                pt_subtitling_info->ui2_comp_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_composition_page_id;
                                pt_subtitling_info->ui2_pid        = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                                pt_subtitling_info->ui2_pmt_index  = pt_ics_pmt->pt_component[ui2_i].ui2_pmt_idx;
                                pt_subtitling_info->h_gl_plane     = grd_get_sbtl_canvas();
                                break;
                            }
                        }
                        if (b_found == TRUE)
                        {
                            break;
                        }
                        /* Just choose the first subtitle */
                        for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_subtilting; ui2_j++)
                        {
                            b_found = TRUE;
                            pt_subtitling_info->e_sbtl_type = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].e_sys_type;
                            pt_subtitling_info->s_lang[0] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[0];
                            pt_subtitling_info->s_lang[1] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[1];
                            pt_subtitling_info->s_lang[2] = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].aui1_lang[2];
                            pt_subtitling_info->s_lang[3] = '\0';
                            pt_subtitling_info->ui2_anci_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_ancillary_page_id;
                            pt_subtitling_info->ui2_comp_pg_id = pt_ics_pmt->pt_component[ui2_i].pt_subtitling_info[ui2_j].ui2_composition_page_id;
                            pt_subtitling_info->ui2_pid        = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                            pt_subtitling_info->ui2_pmt_index  = pt_ics_pmt->pt_component[ui2_i].ui2_pmt_idx;
                            pt_subtitling_info->h_gl_plane     = grd_get_sbtl_canvas();
                            break;
                        }
                        if (b_found == TRUE)
                        {
                            break;
                        }
                    }
                }
            }
        }
        if (b_found == FALSE)
        {
            x_memset(pt_subtitling_info, 0, sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));
            pt_subtitling_info->ui2_pid = MPEG_2_NULL_PID;
            pt_subtitling_info->e_sbtl_type = IMTK_PB_CTRL_DVB_SBTL_UNKNOWN;
            pt_subtitling_info->h_gl_plane = NULL_HANDLE;
        }
    }
}

static UINT16 _mheg5_ics_comp_tag2pid(
    STREAM_TYPE_T    e_strm_type,
    INT32            i4_component_tag, 
    MHEG5_ICS_PMT_T* pt_ics_pmt)
{
    UINT16 ui2_pid = MPEG_2_NULL_PID;

    if (pt_ics_pmt != NULL)
    {
        if (pt_ics_pmt->pt_component != NULL)
        {
            UINT16 ui2_i;

            if (i4_component_tag == ((INT32)-1))
            {
                if (e_strm_type == ST_AUDIO)
                {
                    UINT16 ui2_j;
                    /* receiver default */
                    ISO_639_LANG_T s639_lang_def;
                    ISO_639_LANG_T s639_lang_gui;
                    
                    sct_get_default_lang(s639_lang_def);
                    sct_get_gui_lang(s639_lang_gui);

                    for (ui2_i = 0; ui2_i < pt_ics_pmt->ui2_num_components; ui2_i++)
                    {
                        if (pt_ics_pmt->pt_component[ui2_i].ui1_stream_type == 0x11)
                        {
                            if (pt_ics_pmt->pt_component[ui2_i].pt_audio_info != NULL)
                            {
                                for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_audio; ui2_j++)
                                {
                                    if (x_strcmp(s639_lang_def, (CHAR*)(pt_ics_pmt->pt_component[ui2_i].pt_audio_info[ui2_j].aui1_lang)) == 0)
                                    {
                                        ui2_pid = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                                        break;
                                    }
                                }
                                if (ui2_pid != MPEG_2_NULL_PID)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    /* Didn't found default, search by GUI language */
                    if (ui2_pid == MPEG_2_NULL_PID)
                    {
                        for (ui2_i = 0; ui2_i < pt_ics_pmt->ui2_num_components; ui2_i++)
                        {
                            if (pt_ics_pmt->pt_component[ui2_i].ui1_stream_type == 0x11)
                            {
                                if (pt_ics_pmt->pt_component[ui2_i].pt_audio_info != NULL)
                                {
                                    for (ui2_j = 0; ui2_j < pt_ics_pmt->pt_component[ui2_i].ui2_num_audio; ui2_j++)
                                    {
                                        if (x_strcmp(s639_lang_gui, (CHAR*)(pt_ics_pmt->pt_component[ui2_i].pt_audio_info[ui2_j].aui1_lang)) == 0)
                                        {
                                            ui2_pid = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                                            break;
                                        }
                                    }
                                    if (ui2_pid != MPEG_2_NULL_PID)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    /* Shit, still not found, just search with stream type:17, MPEG4 HE-AAC */
                    if (ui2_pid == MPEG_2_NULL_PID)
                    {
                        for (ui2_i = 0; ui2_i < pt_ics_pmt->ui2_num_components; ui2_i++)
                        {
                            if (pt_ics_pmt->pt_component[ui2_i].ui1_stream_type == 0x11)
                            {
                                ui2_pid = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                                break;
                            }
                        }
                    }                    
                }
                else if (e_strm_type == ST_VIDEO)
                {
                    for (ui2_i = 0; ui2_i < pt_ics_pmt->ui2_num_components; ui2_i++)
                    {
                        /* H264 SD AVC */
                        if (pt_ics_pmt->pt_component[ui2_i].ui1_stream_type == 0x1B)
                        {
                            ui2_pid = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                            break;
                        }
                    }
                }
                else
                {
                    
                }
            }
            else
            {
                for (ui2_i = 0; ui2_i < pt_ics_pmt->ui2_num_components; ui2_i++)
                {
                    if (i4_component_tag == (INT32)(pt_ics_pmt->pt_component[ui2_i].ui1_component_tag))
                    {
                        ui2_pid = pt_ics_pmt->pt_component[ui2_i].ui2_elementary_pid;
                        break;
                    }
                }
            }
        }
    }

    return ui2_pid;
}

static VOID _mheg5_ics_switch_av_path(
    STREAM_TYPE_T      e_strm_type,
    BOOL               b_main_path)
{
    #if 0
    if (e_strm_type == ST_AUDIO)
    {
        /* Switch audio path  */
        MHEG5_MSG_T t_msg;
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = MHEG5_MSG_SWITCH_AUDIO_PATH;
        t_msg.pv_nfy_tag   = (VOID*)((UINT32)b_main_path);
        
        x_mheg5_send_msg(MHEG5_OBS_ENG_CODE, &t_msg, 10);
    }
    else if (e_strm_type == ST_VIDEO)
    {
        /* Switch audio path  */
        MHEG5_MSG_T t_msg;
        t_msg.ui1_module   = MHEG5_MOD_SCT;
        t_msg.ui2_msg_type = MHEG5_MSG_SWITCH_VIDEO_PATH;
        t_msg.pv_nfy_tag   = (VOID*)((UINT32)b_main_path);
        
        x_mheg5_send_msg(MHEG5_OBS_ENG_CODE, &t_msg, 10);
    }
    else
    {
        return ;
    }
    #endif
}

static VOID _mheg5_ics_nfy_play_started()
{
    MHEG5_ICS_LOCK();

    if (_t_ics_info.e_state == MHEG5_ICS_STATE_STARTED &&
        _t_ics_info.b_start_notified == FALSE)
    {
        if (_t_ics_info.t_comp.audio_active)
        {
            tmMHEG5NotifyAudioStarted(FALSE);
            x_dbg_stmt(">>> tmMHEG5NotifyAudioStarted >>>\n");
            _t_ics_info.b_start_notified = TRUE;
        }

        if (_t_ics_info.t_comp.video_active)
        {
            tmMHEG5NotifyVideoStarted();
            x_dbg_stmt(">>> tmMHEG5NotifyVideoStarted >>>\n");
            _t_ics_info.b_start_notified = TRUE;
        }
    }

    MHEG5_ICS_UNLOCK();
}

static VOID _mheg5_ics_nfy_play_stopped()
{
    MHEG5_ICS_LOCK();

    if (_t_ics_info.e_state != MHEG5_ICS_STATE_STOPPED &&
        _t_ics_info.b_start_notified == TRUE)
    {
        if (_t_ics_info.t_comp.audio_active)
        {
            tmMHEG5NotifyAudioStopped(FALSE);
            x_dbg_stmt(">>> tmMHEG5NotifyAudioStopped >>>\n");
            _t_ics_info.b_start_notified = FALSE;
        }

        if (_t_ics_info.t_comp.video_active)
        {
            tmMHEG5NotifyVideoStopped();
            x_dbg_stmt(">>> tmMHEG5NotifyVideoStopped >>>\n");
             _t_ics_info.b_start_notified = FALSE;
        }
    }

    MHEG5_ICS_UNLOCK();
}

static IMTK_PB_CB_ERROR_CODE_T _mheg5_ics_pb_cb(IMTK_PB_CTRL_EVENT_T       eEventType,
                                                       void*                      pvTag,
                                                       uint32_t                   u4Data)
{

    switch (eEventType)
    {
        case IMTK_PB_CTRL_EVENT_UNKNOWN:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_UNKNOWN >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_TOTAL_TIME_UPDATE >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE:
            /*x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_CUR_TIME_UPDATE >>>\n");*/
            break;
        case IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_BUFFER_UNDERFLOW >>>\n");
            _t_ics_info.b_underflow = TRUE;
            /*MHEG5_ICS_UNLOCK();*/
            break;
        case IMTK_PB_CTRL_EVENT_BUFFER_HIGH:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_BUFFER_HIGH >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_BUFFER_LOW:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_BUFFER_LOW >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_PLAYBACK_ERROR >>>\n");
            MHEG5_ICS_UNLOCK();
            break;
        case IMTK_PB_CTRL_EVENT_EOS:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_EOS >>>\n");
            _t_ics_info.b_eos = TRUE;
            #ifdef MHEG5_ICS_WAIT_EOS
            MHEG5_ICS_UNLOCK();
            #endif
            break;
        case IMTK_PB_CTRL_EVENT_EOF:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_EOF >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_PLAY_DONE:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_PLAY_DONE >>>\n");
            _mheg5_ics_nfy_play_started();
            break;
        case IMTK_PB_CTRL_EVENT_STEP_DONE:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_STEP_DONE >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_TIMESEEK_DONE:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_TIMESEEK_DONE >>>\n");
            break;
        case IMTK_PB_CTRL_EVENT_GET_BUF_READY:
            x_dbg_stmt("<<< IMTK_PB_CTRL_EVENT_GET_BUF_READY >>>\n");
            break;

        default :
            break;
    }
    
    return IMTK_PB_CB_ERROR_CODE_OK;
}


INT32 mheg5_ic_reg_pin_support_cb_fct(
    x_mheg5_pin_support_fct pf_fct, 
    VOID*                   pv_tag)
{
    _pf_pin_support_fct     = pf_fct;
    _pv_pin_support_fct_tag = pv_tag;
    
    return MHEG5R_OK;
}

INT32 mheg5_ic_nfy_state_changed(MHEG5_IC_STATE_T e_ic_state)
{
    tmMHEG5Err_t t_err;

    if (e_ic_state == MHEG5_IC_STATE_ACTIVE)
    {
        _t_ic_status = MHEG5_IC_STATUS_ACTIVE;
    }
    else if (e_ic_state == MHEG5_IC_STATE_INACTIVE)
    {
        _t_ic_status = MHEG5_IC_STATUS_INACTIVE;
    }
    else if (e_ic_state == MHEG5_IC_STATE_DISABLED)
    {
        _t_ic_status = MHEG5_IC_STATUS_DISABLED;
    }
    else
    {
        return MHEG5R_INV_ARG;
    }
    
    t_err = tmMHEG5NotifyICStatusChanged();
    
    return mheg5_obs_convert_err_code(t_err);
}

tmMHEG5Err_t tmMHEG5GetICStatus(tmMHEG5ICStatus_t *pIcStatus)
{
    if (pIcStatus != NULL)
    {
        *pIcStatus = _t_ic_status;

        return MHEG5_OK;
    }
    else
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
}

tmMHEG5Err_t tmMHEG5SetupICS(
    tmMHEG5ICSPmt_t pmt,
    tmMHEG5ICSKeys_t *keys,
    tmMHEG5ICSStatus_t *status)
{
    INT32                i4_ret;    
    IMTK_PB_ERROR_CODE_T e_return;
    MHEG5_ICS_PMT_T*     pt_ics_pmt = NULL;
    tmMHEG5ICSKeys_t*    pt_encrypt_keys = NULL;
    IMTK_PB_HANDLE_T     hPB_Handle = IMTK_NULL_HANDLE;

    DBG_INFO(("{MHEG5 ICS}DBG_INFO %s @ %d ,pmt:%p, keys:%p\n", __FUNCTION__, __LINE__, pmt, keys));
    DBG_ERROR(("{MHEG5 ICS}DBG_ERROR %s @ %d ,pmt:%p, keys:%p\n", __FUNCTION__, __LINE__, pmt, keys));
    i4_ret = MHEG5R_INTERNAL_ERROR;

    if (_h_ics_sema == NULL_HANDLE)
    {
        i4_ret = x_sema_create( &_h_ics_sema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_OK)
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed to create semaphore(%d)\n", __FUNCTION__, __LINE__, i4_ret));
            return MHEG5_ERR_OTHER;
        }
    }

    if (_t_ics_info.e_state != MHEG5_ICS_STATE_STOPPED)
    {
        DBG_INFO(("{MHEG5 ICS} %s @ %d ,Must be stopped before starting a new ICS playback\n", __FUNCTION__, __LINE__));
        return MHEG5_ERR_OTHER;
    }

    do
    {
        i4_ret = _mheg5_ics_get_pmt((UINT8*)pmt, &pt_ics_pmt);
        if (i4_ret != MHEG5R_OK || pt_ics_pmt == NULL)
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed to retrieve PMT or engine didn't provide PMT\n", __FUNCTION__, __LINE__));
            break;
        }

        i4_ret = _mheg5_ics_copy_keys(keys, &pt_encrypt_keys);
        if (i4_ret != MHEG5R_OK)
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed to copy entryption keys\n", __FUNCTION__, __LINE__));
            break;
        }

		if (sct_check_if_main_path_have_video())
		{
			x_dbg_stmt("MHEG5SetupICS open CMPB with sub_svctx, ICS can only play audio and non full screen video\n!");
	        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                            IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,
                            IMTK_PB_CTRL_APP_MASTER, 
                            (uint8_t*)"sub_svctx");
		}
		else
		{
			x_dbg_stmt("MHEG5SetupICS open CMPB with MAIN_PUSH_ON_SUB\n!");
	        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
	                                    IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,
	                                    IMTK_PB_CTRL_APP_MASTER, 
	                                    (uint8_t*)"MAIN_PUSH_ON_SUB");
		}
                                    
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Open() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            break;       
        }

        
        e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                                           NULL,
                                           _mheg5_ics_pb_cb);
                                
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_RegCallback() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            break;        
        }

        i4_ret = MHEG5R_OK;
    } while(0);

    if (i4_ret == MHEG5R_OK)
    {
        MHEG5_ICS_LOCK();

        x_memset((void*)&_t_ics_info, 0, sizeof(MHEG5_ICS_INFO_T));

        _t_ics_info.e_state = MHEG5_ICS_STATE_STARTED;
        _t_ics_info.pt_ics_pmt = pt_ics_pmt;
        _t_ics_info.hPB_Handle = hPB_Handle;
        _t_ics_info.pt_encrypt_keys = pt_encrypt_keys;
        _t_ics_info.b_eos = FALSE;
        _t_ics_info.b_comp_inited = FALSE;
        _t_ics_info.e_sbtl_state = MHEG5_SBTL_STOP;
        
        MHEG5_ICS_UNLOCK();
        
        *status = MHEG5_ICS_STATUS_READY;
        
        return MHEG5_OK;
    }
    else
    {
        _mheg5_ics_rel_pmt(pt_ics_pmt);
        _mheg5_ics_free_keys(pt_encrypt_keys);

        if (hPB_Handle != IMTK_NULL_HANDLE)
        {
            IMtkPb_Ctrl_Close(hPB_Handle);
        }
        
        MHEG5_ICS_LOCK();
        x_memset((void*)&_t_ics_info, 0, sizeof(MHEG5_ICS_INFO_T));
        MHEG5_ICS_UNLOCK();

        return MHEG5_ERR_OTHER;
    }

}

tmMHEG5Err_t tmMHEG5SetICSComponents(
    tmMHEG5ICSComponents_t *components)
{
    DBG_INFO(("{MHEG5 ICS} %s @ %d ,components:%p\n", __FUNCTION__, __LINE__, components));

    if (components == NULL)
    {
        DBG_ERROR(("{MHEG5 ICS} %s @ %d ,components is a NULL pointer\n", __FUNCTION__, __LINE__));
        return MHEG5_ERR_OTHER;
    }
    
    MHEG5_ICS_LOCK();

    if (_t_ics_info.e_state == MHEG5_ICS_STATE_STOPPED)
    {
        DBG_INFO(("{MHEG5 ICS} %s @ %d ,ICS state is MHEG5_ICS_STATE_STOPPED\n", __FUNCTION__, __LINE__));
        
        MHEG5_ICS_UNLOCK();
        return MHEG5_ERR_OTHER;        
    }

    if (_t_ics_info.b_comp_inited == FALSE)
    {
        IMTK_PB_ERROR_CODE_T          e_return;
        IMTK_PB_CTRL_ENGINE_PARAM_T   t_parm;
        IMTK_PB_CTRL_SET_MEDIA_INFO_T t_media_infor;
        IMTK_PB_CTRL_SUBTITLE_INFO_T  t_sbtl_infor;

        x_memcpy((void*)&_t_ics_info.t_comp, components, sizeof(tmMHEG5ICSComponents_t));
        x_memset((void*)&t_parm, 0, sizeof(IMTK_PB_CTRL_ENGINE_PARAM_T));
        
        t_parm.u4PlayFlag |= IMTK_PB_CTRL_PLAY_FLAG_AUDIO;
        t_parm.u4PlayFlag |= IMTK_PB_CTRL_PLAY_FLAG_VIDEO;
        
        e_return = IMtkPb_Ctrl_SetEngineParam(_t_ics_info.hPB_Handle,
                                              &t_parm);
            
        if (e_return != IMTK_PB_ERROR_CODE_OK)
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SetEngineParam() Failed as %d !\n", __FUNCTION__, __LINE__, e_return));
            
            MHEG5_ICS_UNLOCK();
            return MHEG5_ERR_OTHER;        
        }

        
        t_media_infor.eMediaType = IMTK_PB_CTRL_MEDIA_TYPE_MPEG2_TS;
        t_media_infor.u4TotalDuration = 0xFFFFFFFF;
        t_media_infor.u8Size = (UINT64)-1;

        t_media_infor.uFormatInfo.tTsInfo.ePacketType = IMTK_PB_CTRL_MPEG2_TS_PACKET_TYPE_188BYTE;
        
        if (components->audio_active)
        {
            t_media_infor.uFormatInfo.tTsInfo.eAudInfo.u2Pid =_mheg5_ics_comp_tag2pid(ST_AUDIO, components->audio_component_tag, _t_ics_info.pt_ics_pmt);
            /*t_media_infor.uFormatInfo.tTsInfo.eAudInfo.uAudCodecInfo. = ;*/
            if (t_media_infor.uFormatInfo.tTsInfo.eAudInfo.u2Pid != MPEG_2_NULL_PID)
            {
                t_media_infor.uFormatInfo.tTsInfo.eAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_AAC;
            }
            else
            {
                t_media_infor.uFormatInfo.tTsInfo.eAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
            }

            /* Switch audio path to SUB */
            _mheg5_ics_switch_av_path(ST_AUDIO, FALSE);

        }
        else
        {            
            t_media_infor.uFormatInfo.tTsInfo.eAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
            t_media_infor.uFormatInfo.tTsInfo.eAudInfo.u2Pid = MPEG_2_NULL_PID;
            /*t_media_infor.uFormatInfo.tTsInfo.eAudInfo.uAudCodecInfo. = ;*/
            
            /* Switch audio path to MAIN */
            _mheg5_ics_switch_av_path(ST_AUDIO, TRUE);
        }
        _t_ics_info.t_a_pid = t_media_infor.uFormatInfo.tTsInfo.eAudInfo.u2Pid;

        if (components->video_active)
        {            
            t_media_infor.uFormatInfo.tTsInfo.eVidInfo.u2Pid = _mheg5_ics_comp_tag2pid(ST_VIDEO, components->video_component_tag, _t_ics_info.pt_ics_pmt);
            /*t_media_infor.uFormatInfo.tTsInfo.eVidInfo.uVidCodecInfo. = ;*/
            if (t_media_infor.uFormatInfo.tTsInfo.eVidInfo.u2Pid != MPEG_2_NULL_PID)
            {
                t_media_infor.uFormatInfo.tTsInfo.eVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_H264;
            }
            else
            {
                t_media_infor.uFormatInfo.tTsInfo.eVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;
            }

            _mheg5_ics_get_proper_subtitle_info(&t_sbtl_infor, _t_ics_info.pt_ics_pmt);

            /* Switch video path to SUB */
            _mheg5_ics_switch_av_path(ST_VIDEO, FALSE);
        }
        else
        {            
            t_media_infor.uFormatInfo.tTsInfo.eVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;
            t_media_infor.uFormatInfo.tTsInfo.eVidInfo.u2Pid = MPEG_2_NULL_PID;
            /*t_media_infor.uFormatInfo.tTsInfo.eVidInfo.uVidCodecInfo. = ;*/
            
            x_memset(&t_sbtl_infor, 0, sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));
            t_sbtl_infor.e_sbtl_type = IMTK_PB_CTRL_DVB_SBTL_UNKNOWN;
            t_sbtl_infor.ui2_pid     = MPEG_2_NULL_PID;
            t_sbtl_infor.h_gl_plane  = NULL_HANDLE;

            /* Switch video path to MAIN */
            _mheg5_ics_switch_av_path(ST_VIDEO, TRUE);
        }
        _t_ics_info.t_v_pid = t_media_infor.uFormatInfo.tTsInfo.eVidInfo.u2Pid;
        _t_ics_info.t_s_pid = t_sbtl_infor.ui2_pid;

        e_return = IMtkPb_Ctrl_SetSubtitleInfo(_t_ics_info.hPB_Handle, &t_sbtl_infor);
        if (e_return != IMTK_PB_ERROR_CODE_OK)
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SetMediaInfo() Failed as %d !\n", __FUNCTION__, __LINE__, e_return));
        }

        e_return = IMtkPb_Ctrl_SetMediaInfo(_t_ics_info.hPB_Handle, &t_media_infor);
        if (e_return != IMTK_PB_ERROR_CODE_OK)
        {            
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SetMediaInfo() Failed as %d !\n", __FUNCTION__, __LINE__, e_return));
            
            MHEG5_ICS_UNLOCK();
            return MHEG5_ERR_OTHER;        
        }

        e_return = IMtkPb_Ctrl_Play(_t_ics_info.hPB_Handle, 0);
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {            
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Play() Failed as %d !\n", __FUNCTION__, __LINE__, e_return));
            
            MHEG5_ICS_UNLOCK();
            return MHEG5_ERR_OTHER;        
        }

        if (_t_ics_info.t_s_pid != MPEG_2_NULL_PID)
        {
            x_thread_delay(1000);
            IMtkPb_Ctrl_SetSubtitleTrack(_t_ics_info.hPB_Handle, 0);
        }
        x_thread_delay(1000);
        mheg5_ics_handle_sbtl_visibility();
        
        if (_t_ics_info.pt_encrypt_keys != NULL)
        {
            IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO tEncryptInfo;

            x_memset(&tEncryptInfo, 0, sizeof(IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO));

            tEncryptInfo.eEncryptMode = IMTK_PB_CTRL_IBC_ENCRYPT_MODE_AES_CBC;
            tEncryptInfo.uEncryptInfo.tAes.u1Mask = 
                (IMTK_PB_CTRL_AES_ENCRYPT_IV | IMTK_PB_CTRL_AES_ENCRYPT_EVEN);
            tEncryptInfo.uEncryptInfo.tAes.u1KeyLen = 16;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpKey = FALSE;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpIV  = FALSE;
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1IV, _t_ics_info.pt_encrypt_keys->pid_info[0].iv, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1Odd_key, _t_ics_info.pt_encrypt_keys->pid_info[0].odd_key, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1EvenKey, _t_ics_info.pt_encrypt_keys->pid_info[0].even_key, 16);

            tEncryptInfo.eRtbMode = IMTK_PB_CTRL_IBCC_RTB_MODE_SCTE52;
                
            e_return = IMtkPb_Ctrl_SetEncryptInfo(_t_ics_info.hPB_Handle, &tEncryptInfo);
            if (e_return != IMTK_PB_ERROR_CODE_OK)                                
            {
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SendCmd() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            }
            
            x_memset(&tEncryptInfo, 0, sizeof(IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO));

            tEncryptInfo.eEncryptMode = IMTK_PB_CTRL_IBC_ENCRYPT_MODE_AES_CBC;
            tEncryptInfo.uEncryptInfo.tAes.u1Mask = 
                (IMTK_PB_CTRL_AES_ENCRYPT_IV | IMTK_PB_CTRL_AES_ENCRYPT_ODD);
            tEncryptInfo.uEncryptInfo.tAes.u1KeyLen = 16;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpKey = FALSE;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpIV  = FALSE;
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1IV, _t_ics_info.pt_encrypt_keys->pid_info[0].iv, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1Odd_key, _t_ics_info.pt_encrypt_keys->pid_info[0].odd_key, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1EvenKey, _t_ics_info.pt_encrypt_keys->pid_info[0].even_key, 16);

            tEncryptInfo.eRtbMode = IMTK_PB_CTRL_IBCC_RTB_MODE_SCTE52;
                
            e_return = IMtkPb_Ctrl_SetEncryptInfo(_t_ics_info.hPB_Handle, &tEncryptInfo);
            if (e_return != IMTK_PB_ERROR_CODE_OK)                                
            {
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SendCmd() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            }
        }
        _t_ics_info.b_comp_inited = TRUE;

        
        MHEG5_ICS_UNLOCK();

        return MHEG5_OK;
    }
    else
    {
        IMTK_PB_ERROR_CODE_T e_return1, e_return2;
        IMTK_PB_CTRL_AUD_INFO_T t_audio_info;
        IMTK_PB_CTRL_VID_INFO_T t_video_info;
        IMTK_PB_CTRL_SUBTITLE_INFO_T t_sbtl_info;
        
        x_memcpy((void*)&_t_ics_info.t_comp, components, sizeof(tmMHEG5ICSComponents_t));

        if (components->audio_active)
        {            
            t_audio_info.tMpgTsAudInfo.u2Pid   = _mheg5_ics_comp_tag2pid(ST_AUDIO, components->audio_component_tag, _t_ics_info.pt_ics_pmt);
            /*t_audio_info.tMpgTsAudInfo.uAudCodecInfo. = ;*/
            
            if (t_audio_info.tMpgTsAudInfo.u2Pid != MPEG_2_NULL_PID)
            {
                t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_AAC;
            }
            else
            {
                t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
            }

            /* Switch audio path to SUB */
            _mheg5_ics_switch_av_path(ST_AUDIO, FALSE);
        }
        else
        {
            t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
            t_audio_info.tMpgTsAudInfo.u2Pid   = MPEG_2_NULL_PID;
            /*t_audio_info.tMpgTsAudInfo.uAudCodecInfo. = ;*/
            
            /* Switch audio path to MAIN */
            _mheg5_ics_switch_av_path(ST_AUDIO, TRUE);
        }
        
        if (_t_ics_info.t_a_pid != t_audio_info.tMpgTsAudInfo.u2Pid)
        {
            _t_ics_info.t_a_pid = t_audio_info.tMpgTsAudInfo.u2Pid;
            
            e_return1 = IMtkPb_Ctrl_ChangeAudio(_t_ics_info.hPB_Handle, &t_audio_info);
            if (e_return1 != IMTK_PB_ERROR_CODE_OK)                                
            {
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_ChangeAudio() Failed as %d !\n", __FUNCTION__, __LINE__, e_return1));
            }
        }

        if (components->video_active)
        {            
            t_video_info.tMpgTsVidInfo.u2Pid = _mheg5_ics_comp_tag2pid(ST_VIDEO, components->video_component_tag, _t_ics_info.pt_ics_pmt);
            /*t_video_info.tMpgTsVidInfo.uVidCodecInfo. = ;*/
            if (t_video_info.tMpgTsVidInfo.u2Pid != MPEG_2_NULL_PID)
            {
                t_video_info.tMpgTsVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_H264;
            }
            else
            {
                t_video_info.tMpgTsVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;
            }
            
            _mheg5_ics_get_proper_subtitle_info(&t_sbtl_info, _t_ics_info.pt_ics_pmt);

            /* Switch video path to SUB */
            _mheg5_ics_switch_av_path(ST_VIDEO, FALSE);
        }
        else
        {            
            t_video_info.tMpgTsVidInfo.eVidEnc = IMTK_PB_CTRL_VID_ENC_UNKNOWN;
            t_video_info.tMpgTsVidInfo.u2Pid = MPEG_2_NULL_PID;
            /*t_video_info.tMpgTsVidInfo.uVidCodecInfo. = ;*/

            x_memset(&t_sbtl_info, 0, sizeof(IMTK_PB_CTRL_SUBTITLE_INFO_T));
            t_sbtl_info.e_sbtl_type = IMTK_PB_CTRL_DVB_SBTL_UNKNOWN;
            t_sbtl_info.ui2_pid = MPEG_2_NULL_PID;
            t_sbtl_info.h_gl_plane = NULL_HANDLE;
            
            /* Switch video path to MAIN */
            _mheg5_ics_switch_av_path(ST_VIDEO, TRUE);
        }
        
        if (_t_ics_info.t_v_pid != t_video_info.tMpgTsVidInfo.u2Pid)
        {
            _t_ics_info.t_v_pid = t_video_info.tMpgTsVidInfo.u2Pid;

            e_return2 = IMtkPb_Ctrl_ChangeVideo(_t_ics_info.hPB_Handle, &t_video_info);
            if (e_return2 != IMTK_PB_ERROR_CODE_OK)                                
            {            
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_ChangeVideo() Failed as %d !\n", __FUNCTION__, __LINE__, e_return2));
            }
        }
        
        if (_t_ics_info.t_s_pid != t_sbtl_info.ui2_pid)
        {
            _t_ics_info.t_s_pid = t_sbtl_info.ui2_pid;

            e_return2 = IMtkPb_Ctrl_ChangeSubtitle(_t_ics_info.hPB_Handle, &t_sbtl_info);
            if (e_return2 != IMTK_PB_ERROR_CODE_OK)                                
            {            
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_ChangeSubtitle() Failed as %d !\n", __FUNCTION__, __LINE__, e_return2));
            }
            
            if (_t_ics_info.t_a_pid != MPEG_2_NULL_PID)
            {
                IMtkPb_Ctrl_SetSubtitleTrack(_t_ics_info.hPB_Handle, 0);
            }

            mheg5_ics_handle_sbtl_visibility();
        }
        
        MHEG5_ICS_UNLOCK();

        return MHEG5_OK;
    }    
}

U32BIT DVP_HandleStreamData(
    U8BIT *data, 
    U32BIT len, 
    BOOLEAN last)
{
    /* We have to try to let the CMPB always be hungry!!!  */
    INT32  i4_retry_times = 100;
    UINT8* pui1_buf = NULL;
    IMTK_PB_CTRL_BUFFER_STATUS_EXT_T t_buf_status;
    IMTK_PB_ERROR_CODE_T e_return;
    
#ifdef CLI_LVL_ALL
    if ((g_ui4_data_trace % 10 == 0) || ((BOOL)last == TRUE))
    {
        DBG_INFO(("{MHEG5 ICS} %s @ %d ,data:%p, len:%d, last:%d, times:%lu\n", __FUNCTION__, __LINE__, data, len, last, g_ui4_data_trace));
    }
    g_ui4_data_trace ++;
#endif

    if (data == NULL)
    {
        DBG_ERROR(("{MHEG5 ICS} %s @ %d ,data is a NULL pointer\n", __FUNCTION__, __LINE__));
        return ((U32BIT)0);
    }
    /* TODO: Need to handle engine pausing and resuming */
    if (_t_ics_info.e_state != MHEG5_ICS_STATE_STARTED)
    {
        DBG_INFO(("{MHEG5 ICS} %s @ %d ,ICS state is not STARTED\n", __FUNCTION__, __LINE__));
        return ((U32BIT)0);
    }

    /* Need to query buffer status first */
    while (i4_retry_times-- > 0)
    {
        e_return = IMtkPb_Ctrl_GetBufferStatus_Ext(_t_ics_info.hPB_Handle, &t_buf_status);
        if (IMTK_PB_ERROR_CODE_OK != e_return)
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_GetBufferStatus() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            return ((U32BIT)0);
        }

        if (t_buf_status.e_buf_status == IMTK_PB_CTRL_PUSH_BUF_HIGH ||
            t_buf_status.e_buf_status == IMTK_PB_CTRL_PUSH_BUF_OVERFLOW)
        {
            /* Buffer status is high, wait a while */
            DBG_INFO(("{MHEG5 ICS} %s @ %d ,Before sending new data, Buffer status is high, wait 10 ms\n", __FUNCTION__, __LINE__));
            x_thread_delay(10);
        }
        else
        {
            break;
        }
    }
    /* Now feed the CMPB */
    i4_retry_times = 100;
    while (i4_retry_times-- > 0)
    {
        e_return = IMtkPb_Ctrl_GetBuffer(_t_ics_info.hPB_Handle, 
                                         len, 
                                         &pui1_buf);
        
        if (IMTK_PB_ERROR_CODE_OK == e_return)
        {
            x_memcpy(pui1_buf, data, len);

            IMtkPb_Ctrl_SendData(_t_ics_info.hPB_Handle, 
                                 len, 
                                 pui1_buf);

            if ((BOOL)last == TRUE) 
            {
                IMtkPb_Ctrl_SetPushModelEOS(_t_ics_info.hPB_Handle);

                #ifndef MHEG5_ICS_WAIT_EOS
                {
                    x_thread_delay(400);
                    /* Now last frame may be displayed */
                    _mheg5_ics_nfy_play_stopped();
                }
                #else
                MHEG5_ICS_LOCK();
                #endif
                
                MHEG5_ICS_LOCK();
                _t_ics_info.b_eos = FALSE;
                if (_t_ics_info.t_comp.video_termination == MHEG5_VIDEO_TERMINATION_DISAPPEAR)
                {
                    IMtkPb_Ctrl_Stop(_t_ics_info.hPB_Handle);
                    /*IMtkPb_Ctrl_ClearVideo(_t_ics_info.hPB_Handle);*/
                }
                MHEG5_ICS_UNLOCK();

                _mheg5_ics_nfy_play_stopped();
            }
            
            break;
        }
        else if (IMTK_PB_ERROR_CODE_GET_BUF_PENDING == e_return)
        {
            x_thread_delay(10);
        }
        else
        {
            x_thread_delay(10);
        }
    }

    if (e_return != IMTK_PB_ERROR_CODE_OK)
    {
        return ((U32BIT)0);
    }
    else
    {
        i4_retry_times = 100;
        /* Need to query buffer status again */
        while (i4_retry_times-- > 0)
        {
            e_return = IMtkPb_Ctrl_GetBufferStatus_Ext(_t_ics_info.hPB_Handle, &t_buf_status);
            if (IMTK_PB_ERROR_CODE_OK != e_return)
            {
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_GetBufferStatus() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
                break;
            }
            else
            {
                if (t_buf_status.e_buf_status == IMTK_PB_CTRL_PUSH_BUF_HIGH ||
                    t_buf_status.e_buf_status == IMTK_PB_CTRL_PUSH_BUF_OVERFLOW)
                {
                    /* Buffer status is high, wait a while */
                    DBG_INFO(("{MHEG5 ICS} %s @ %d ,After sending new data, Buffer status is high, wait 10 ms\n", __FUNCTION__, __LINE__));
                    x_thread_delay(10);
                }
                else
                {
                    break;
                }
            }
        }

        return len;
    }
}

tmMHEG5Err_t tmMHEG5PauseICS()
{    
    IMTK_PB_ERROR_CODE_T e_return;
    MHEG5_ICS_LOCK();

    DBG_INFO(("{MHEG5 ICS} %s @ %d \n", __FUNCTION__, __LINE__));
    
    mheg5_ics_handle_sbtl_visibility();

    e_return = IMtkPb_Ctrl_Pause(_t_ics_info.hPB_Handle);
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
        MHEG5_ICS_UNLOCK();
        return MHEG5_ERR_OTHER;        
    }
    
    _t_ics_info.e_state = MHEG5_ICS_STATE_PAUSED;
    MHEG5_ICS_UNLOCK();

    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5ResumeICS()
{
    IMTK_PB_ERROR_CODE_T e_return;
    
    MHEG5_ICS_LOCK();
    
    DBG_INFO(("{MHEG5 ICS} %s @ %d \n", __FUNCTION__, __LINE__));

    mheg5_ics_handle_sbtl_visibility();
    e_return = IMtkPb_Ctrl_Play(_t_ics_info.hPB_Handle, 0);
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
        MHEG5_ICS_UNLOCK();
        return MHEG5_ERR_OTHER;        
    }

    _t_ics_info.e_state = MHEG5_ICS_STATE_STARTED;
    MHEG5_ICS_UNLOCK();
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5SetICSVideoProperties(S_VideoPosition *position)
{
    DBG_INFO(("{MHEG5 ICS} %s @ %d ,position:%p\n", __FUNCTION__, __LINE__, position));

    DVP_VideoPosition(position);
    
    return MHEG5_OK;
}

tmMHEG5Err_t tmMHEG5SetICSAudioProperties(S32BIT volumeAdjust)
{
    DBG_INFO(("{MHEG5 ICS} %s @ %d ,volumeAdjust:%d\n", __FUNCTION__, __LINE__, volumeAdjust));
    
    return tmMHEG5SetAudioVolumeAdjustment(volumeAdjust);
}

tmMHEG5Err_t tmMHEG5StopICS()
{
    IMTK_PB_ERROR_CODE_T e_return;
    
    MHEG5_ICS_LOCK();
    
    DBG_INFO(("{MHEG5 ICS} %s @ %d ,\n", __FUNCTION__, __LINE__));

    e_return = IMtkPb_Ctrl_Stop(_t_ics_info.hPB_Handle);
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
    }
    
    /*IMtkPb_Ctrl_ClearVideo(_t_ics_info.hPB_Handle);*/

    e_return = IMtkPb_Ctrl_Close(_t_ics_info.hPB_Handle);
    if (e_return != IMTK_PB_ERROR_CODE_OK)                                
    {
        DBG_ERROR(("{MHEG5 ICS} %s @ %d ,Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
    }
    
    MHEG5_ICS_UNLOCK();

    _mheg5_ics_nfy_play_stopped();
    
    /* Switch video path to MAIN */
    _mheg5_ics_switch_av_path(ST_VIDEO, TRUE);
    /* Switch audio path to MAIN */
    _mheg5_ics_switch_av_path(ST_AUDIO, TRUE);

    MHEG5_ICS_LOCK();

    _mheg5_ics_rel_pmt(_t_ics_info.pt_ics_pmt);
    _mheg5_ics_free_keys(_t_ics_info.pt_encrypt_keys);
    
    
    x_memset((void*)&_t_ics_info, 0, sizeof(MHEG5_ICS_INFO_T));

    _t_ics_info.e_state = MHEG5_ICS_STATE_STOPPED;
    _t_ics_info.e_sbtl_state = MHEG5_SBTL_STOP;
    MHEG5_ICS_UNLOCK();

#ifdef CLI_LVL_ALL
    g_ui4_data_trace = 0;
#endif

    return MHEG5_OK;
}

INT32 mheg5_ics_handle_engine_pause(VOID)
{
    IMTK_PB_ERROR_CODE_T e_return;
    uint32_t u4CurTime;
    uint64_t u8CurPos;
    
    DBG_INFO(("{MHEG5 ICS} %s @ %d \n", __FUNCTION__, __LINE__));

    if (_t_ics_info.e_state != MHEG5_ICS_STATE_STOPPED)
    {        
        IMtkPb_Ctrl_SetSubtitleShowHide(_t_ics_info.hPB_Handle, (bool)TRUE);
        
        e_return = IMtkPb_Ctrl_Pause(_t_ics_info.hPB_Handle);
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Pause() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            return MHEG5R_INTERNAL_ERROR;        
        }

        e_return = IMtkPb_Ctrl_GetCurrentPos(_t_ics_info.hPB_Handle, &u4CurTime, &u8CurPos);
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_GetCurrentPos() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            return MHEG5R_INTERNAL_ERROR;        
        }
        
        e_return = IMtkPb_Ctrl_Stop(_t_ics_info.hPB_Handle);
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Stop() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            return MHEG5R_INTERNAL_ERROR;        
        }
    #if MHEG5_ICS_PAUSE_CLOSE       
        e_return = IMtkPb_Ctrl_Close(_t_ics_info.hPB_Handle);
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Close() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
        }
    #endif
        /* Switch video path to MAIN */
        _mheg5_ics_switch_av_path(ST_VIDEO, TRUE);
        /* Switch audio path to MAIN */
        _mheg5_ics_switch_av_path(ST_AUDIO, TRUE);

        _t_ics_info.e_state = MHEG5_ICS_STATE_PAUSED;
        _t_ics_info.ui4_cur_time = (UINT32)u4CurTime;
        _t_ics_info.ui8_cur_pos  = (UINT64)u8CurPos;
        _t_ics_info.b_comp_inited = FALSE;
        
        return MHEG5R_OK;
    }
    else
    {
        return MHEG5R_INV_STATE;
    }
}

INT32 mheg5_ics_handle_engine_resume(VOID)
{
    DBG_INFO(("{MHEG5 ICS} %s @ %d \n", __FUNCTION__, __LINE__));

    if (_t_ics_info.e_state == MHEG5_ICS_STATE_PAUSED)
    {
    #if MHEG5_ICS_PAUSE_CLOSE       
        IMTK_PB_ERROR_CODE_T e_return;
        IMTK_PB_HANDLE_T     hPB_Handle = IMTK_NULL_HANDLE;
        tmMHEG5ICSComponents_t t_icscomp;

		if (sct_check_if_main_path_have_video())
		{
			x_dbg_stmt("MHEG5SetupICS open CMPB with sub_svctx, ICS can only play audio and non full screen video\n!");
	        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
                            IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,
                            IMTK_PB_CTRL_APP_MASTER, 
                            (uint8_t*)"sub_svctx");
		}
		else
		{
			x_dbg_stmt("MHEG5SetupICS open CMPB with MAIN_PUSH_ON_SUB\n!");
	        e_return = IMtkPb_Ctrl_Open(&hPB_Handle,
	                                    IMTK_PB_CTRL_BUFFERING_MODEL_PUSH,
	                                    IMTK_PB_CTRL_APP_MASTER, 
	                                    (uint8_t*)"MAIN_PUSH_ON_SUB");
		}

                                    
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Open() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            return MHEG5R_INTERNAL_ERROR;        
        }
        _t_ics_info.hPB_Handle = hPB_Handle;

        
        e_return = IMtkPb_Ctrl_RegCallback(hPB_Handle,
                                           NULL,
                                           _mheg5_ics_pb_cb);
                                
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_RegCallback() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            
            return MHEG5R_INTERNAL_ERROR;        
        }
        x_memcpy((void*)&t_icscomp, (void*)&_t_ics_info.t_comp, sizeof(tmMHEG5ICSComponents_t));

        tmMHEG5SetICSComponents(&t_icscomp);
        _t_ics_info.e_state = MHEG5_ICS_STATE_STARTED;
        return MHEG5R_OK;
    #else
        IMTK_PB_ERROR_CODE_T e_return;
        
        if (_t_ics_info.t_comp.audio_active)
        {
            /* Handle if audio lang is changed */
            IMTK_PB_CTRL_AUD_INFO_T t_audio_info;
        
            t_audio_info.tMpgTsAudInfo.u2Pid   = _mheg5_ics_comp_tag2pid(ST_AUDIO, _t_ics_info.t_comp.audio_component_tag, _t_ics_info.pt_ics_pmt);
            /*t_audio_info.tMpgTsAudInfo.uAudCodecInfo. = ;*/
            
            if (t_audio_info.tMpgTsAudInfo.u2Pid != MPEG_2_NULL_PID)
            {
                t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_AAC;
            }
            else
            {
                t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
            }
                
            if (_t_ics_info.t_a_pid != t_audio_info.tMpgTsAudInfo.u2Pid)
            {
                _t_ics_info.t_a_pid = t_audio_info.tMpgTsAudInfo.u2Pid;
                
                e_return = IMtkPb_Ctrl_ChangeAudio(_t_ics_info.hPB_Handle, &t_audio_info);
                if (e_return != IMTK_PB_ERROR_CODE_OK)                                
                {
                    DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_ChangeAudio() Failed as %d !\n", __FUNCTION__, __LINE__, e_return));
                }
            }
            /* Switch audio path to SUB */
            _mheg5_ics_switch_av_path(ST_AUDIO, FALSE);
        }
        else
        {            
            /* Switch audio path to MAIN */
            _mheg5_ics_switch_av_path(ST_AUDIO, TRUE);
        }
        
        if (_t_ics_info.t_comp.video_active)
        {
            IMTK_PB_CTRL_SUBTITLE_INFO_T t_sbtl_info;

            _mheg5_ics_get_proper_subtitle_info(&t_sbtl_info, _t_ics_info.pt_ics_pmt);
            
            if (_t_ics_info.t_s_pid != t_sbtl_info.ui2_pid)
            {
                _t_ics_info.t_s_pid = t_sbtl_info.ui2_pid;
            
                e_return = IMtkPb_Ctrl_ChangeSubtitle(_t_ics_info.hPB_Handle, &t_sbtl_info);
                if (e_return != IMTK_PB_ERROR_CODE_OK)                                
                {            
                    DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_ChangeSubtitle() Failed as %d !\n", __FUNCTION__, __LINE__, e_return));
                }                
            }

            /* Switch video path to SUB */
            _mheg5_ics_switch_av_path(ST_VIDEO, FALSE);
        }
        else
        {    
            /* Switch video path to MAIN */
            _mheg5_ics_switch_av_path(ST_VIDEO, TRUE);
        }
        
        _t_ics_info.e_state = MHEG5_ICS_STATE_STARTED;
        e_return = IMtkPb_Ctrl_Play(_t_ics_info.hPB_Handle, 0);
        if (e_return != IMTK_PB_ERROR_CODE_OK)                                
        {
            DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_Play() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            _t_ics_info.e_state = MHEG5_ICS_STATE_PAUSED;
            return MHEG5R_INTERNAL_ERROR;        
        }
        
        if (((BOOL)_t_ics_info.t_comp.video_active == TRUE) && (_t_ics_info.t_s_pid != MPEG_2_NULL_PID))
        {
            IMtkPb_Ctrl_SetSubtitleTrack(_t_ics_info.hPB_Handle, 0);
            {
                BOOL b_sbtl_hide = FALSE;
                MHEG5_SBTL_STATE_T e_state = sct_get_sbtl_visibility();

                if (e_state == MHEG5_SBTL_HIDE ||
                    e_state == MHEG5_SBTL_STOP)
                {
                    b_sbtl_hide = TRUE;
                }
                else
                {
                    b_sbtl_hide = FALSE;
                }
                
                IMtkPb_Ctrl_SetSubtitleShowHide(_t_ics_info.hPB_Handle, (bool)b_sbtl_hide);
            }
        }

        if (_t_ics_info.pt_encrypt_keys != NULL)
        {
            IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO tEncryptInfo;

            x_memset(&tEncryptInfo, 0, sizeof(IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO));

            tEncryptInfo.eEncryptMode = IMTK_PB_CTRL_IBC_ENCRYPT_MODE_AES_CBC;
            tEncryptInfo.uEncryptInfo.tAes.u1Mask = 
                (IMTK_PB_CTRL_AES_ENCRYPT_IV | IMTK_PB_CTRL_AES_ENCRYPT_EVEN);
            tEncryptInfo.uEncryptInfo.tAes.u1KeyLen = 16;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpKey = FALSE;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpIV  = FALSE;
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1IV, _t_ics_info.pt_encrypt_keys->pid_info[0].iv, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1Odd_key, _t_ics_info.pt_encrypt_keys->pid_info[0].odd_key, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1EvenKey, _t_ics_info.pt_encrypt_keys->pid_info[0].even_key, 16);

            tEncryptInfo.eRtbMode = IMTK_PB_CTRL_IBCC_RTB_MODE_SCTE52;
                
            e_return = IMtkPb_Ctrl_SetEncryptInfo(_t_ics_info.hPB_Handle, &tEncryptInfo);
            if (e_return != IMTK_PB_ERROR_CODE_OK)                                
            {
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SendCmd() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            }
            
            x_memset(&tEncryptInfo, 0, sizeof(IMTK_PB_CTRL_IBC_PARAM_SET_ENCRYPT_INFO));

            tEncryptInfo.eEncryptMode = IMTK_PB_CTRL_IBC_ENCRYPT_MODE_AES_CBC;
            tEncryptInfo.uEncryptInfo.tAes.u1Mask = 
                (IMTK_PB_CTRL_AES_ENCRYPT_IV | IMTK_PB_CTRL_AES_ENCRYPT_ODD);
            tEncryptInfo.uEncryptInfo.tAes.u1KeyLen = 16;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpKey = FALSE;
            tEncryptInfo.uEncryptInfo.tAes.fgWarpIV  = FALSE;
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1IV, _t_ics_info.pt_encrypt_keys->pid_info[0].iv, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1Odd_key, _t_ics_info.pt_encrypt_keys->pid_info[0].odd_key, 16);
            x_memcpy(tEncryptInfo.uEncryptInfo.tAes.au1EvenKey, _t_ics_info.pt_encrypt_keys->pid_info[0].even_key, 16);

            tEncryptInfo.eRtbMode = IMTK_PB_CTRL_IBCC_RTB_MODE_SCTE52;
                
            e_return = IMtkPb_Ctrl_SetEncryptInfo(_t_ics_info.hPB_Handle, &tEncryptInfo);
            if (e_return != IMTK_PB_ERROR_CODE_OK)                                
            {
                DBG_ERROR(("{MHEG5 ICS} %s @ %d ,IMtkPb_Ctrl_SendCmd() Failed as %d!\n", __FUNCTION__, __LINE__, e_return));
            }
        }
        
        _t_ics_info.e_state = MHEG5_ICS_STATE_STARTED;
        
        return MHEG5R_OK;
    #endif
    }
    else
    {
        return MHEG5R_INV_STATE;
    }
}


VOID mheg5_ics_handle_audio_lang_chg()
{
#if 0    
    MHEG5_ICS_LOCK();

    if (((BOOL)(_t_ics_info.t_comp.audio_active) == TRUE) &&
         (_t_ics_info.b_comp_inited == TRUE) &&
         (_t_ics_info.e_state != MHEG5_ICS_STATE_STOPPED))
    {
        IMTK_PB_ERROR_CODE_T e_return;
        IMTK_PB_CTRL_AUD_INFO_T t_audio_info;
        
        t_audio_info.tMpgTsAudInfo.u2Pid   = _mheg5_ics_comp_tag2pid(ST_AUDIO, _t_ics_info.t_comp.audio_component_tag, _t_ics_info.pt_ics_pmt);
        /*t_audio_info.tMpgTsAudInfo.uAudCodecInfo. = ;*/
        
        if (t_audio_info.tMpgTsAudInfo.u2Pid != MPEG_2_NULL_PID)
        {
            t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_AAC;
        }
        else
        {
            t_audio_info.tMpgTsAudInfo.eAudEnc = IMTK_PB_CTRL_AUD_ENC_UNKNOWN;
        }
        
        /* Switch audio path to SUB */
        _mheg5_ics_switch_av_path(ST_AUDIO, FALSE);

        if (_t_ics_info.t_a_pid != t_audio_info.tMpgTsAudInfo.u2Pid)
        {
            _t_ics_info.t_a_pid = t_audio_info.tMpgTsAudInfo.u2Pid;
            
            e_return = IMtkPb_Ctrl_ChangeAudio(_t_ics_info.hPB_Handle, &t_audio_info);
            if (e_return != IMTK_PB_ERROR_CODE_OK)                                
            {
                MHEG5_DBG_ICS(("IMtkPb_Ctrl_ChangeAudio() Failed as %d !\n", e_return));
            }
        }
    }
    
    MHEG5_ICS_UNLOCK();
#endif
}

VOID mheg5_ics_handle_sbtl_visibility(VOID)
{
    if (_t_ics_info.e_state == MHEG5_ICS_STATE_STARTED)
    {
        MHEG5_SBTL_STATE_T e_new_state = sct_get_sbtl_visibility();
        BOOL b_sbtl_hide = TRUE;

        if (_t_ics_info.e_sbtl_state != e_new_state)
        {
            _t_ics_info.e_sbtl_state = e_new_state;
        }

        if (e_new_state == MHEG5_SBTL_HIDE)
        {
            b_sbtl_hide = TRUE;
        }
        else if (e_new_state == MHEG5_SBTL_SHOW)
        {
            b_sbtl_hide = FALSE;
        }
        else if (e_new_state == MHEG5_SBTL_START)
        {
            b_sbtl_hide = FALSE;
        }
        else/* if (e_state == MHEG5_SBTL_STOP) */
        {
            b_sbtl_hide = TRUE;
        }

        IMtkPb_Ctrl_SetSubtitleShowHide(_t_ics_info.hPB_Handle, (bool)b_sbtl_hide);
    }
}


tmMHEG5Err_t tmMHEG5PromptForGuidance(
    U8BIT *restriction, 
    BOOLEAN show)
{
#if 1
    SIZE_T z_len = 0;

    if (NULL == restriction)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    z_len = x_strlen((const CHAR*)restriction);
    /* Don't free this memory, it's not a leakage */
    _ps_pfg_restriction = x_mem_realloc(_ps_pfg_restriction, z_len + 1);
    if (NULL == _ps_pfg_restriction)
    {
        return MHEG5_ERR_OTHER;
    }
    else
    {
        x_memcpy(_ps_pfg_restriction, restriction, z_len);
        _ps_pfg_restriction[z_len] = '\0';
    }

    if (_pf_prompt_guid_nfy)
    {
         _pf_prompt_guid_nfy((UINT8*)_ps_pfg_restriction, show, _pv_prompt_guid_nfy_tag);
    }

    return MHEG5_OK;
    
#else

    return MHEG5_ERR_OTHER;

#endif
}

tmMHEG5Err_t tmMHEG5GetPINSupport(tmMHEG5ICSPinSupport_t *support)
{
    if(NULL == support)
    {
        return MHEG5_ERR_BAD_PARAMETER;
    }
    
    if (_pf_pin_support_fct != NULL)
    {
        if (MHEG5R_OK == _pf_pin_support_fct(_pv_pin_support_fct_tag, (MHEG5_ICS_PIN_SUPPORT_T*)support))
        {
            return MHEG5_OK;
        }
    } 

    return MHEG5_ERR_INTERNAL;
}

#if 0
tmMHEG5Err_t tmMHEG5GetPINSupport(tmMHEG5ICSPinSupport_t *support)
{
    return MHEG5_ERR_OTHER;
}
#endif

INT32 mheg5_ics_set_eng_guid_result(BOOL b_result)
{
#if 1

    tmMHEG5Err_t      t_err;

    t_err = MHEG5_ERR_OTHER;

    t_err = tmMHEG5NotifyGuidanceResponse((BOOLEAN)b_result);

    if (MHEG5_OK != t_err)
    {
        return MHEG5R_ICS_ENG_NFY_FAIL;
    }

    return MHEG5R_OK;
    
#else

    return MHEG5R_OK;

#endif
}

INT32 mheg5_ics_reg_guid_visibility_nfy(
    x_mheg5_ics_guid_visibility_nfy      pf_nfy, 
    VOID*                                pv_tag)
{
#if 1
    if (NULL == pf_nfy)
    {
        return MHEG5R_ICS_REG_NFY_FAIL;
    }
    _pf_prompt_guid_nfy = pf_nfy;
    _pv_prompt_guid_nfy_tag = pv_tag;

    return MHEG5R_OK;
    
#else
    return MHEG5R_OK;
#endif
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: _mheg5_ic_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/
UINT16 mheg5_ic_get_dbg_level(VOID)
{
    return _ui2_mheg5_ic_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: _mheg5_ic_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 ----------------------------------------------------------------------------*/
BOOL mheg5_ic_set_dbg_level(UINT16 ui2_level)
{
    _ui2_mheg5_ic_dbg_level = ui2_level;
    return TRUE;
}

#endif

#endif

