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
 * $RCSfile: sbtl_utils.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 * History:
 *           
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#include <stdio.h>
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "inc/x_mm_common.h"

#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/drv/x_stc_ctrl.h"
#include "mutil/sbtl_engine/sbtl_widget.h"

#include "file_mngr/u_fm.h"
#include "file_mngr/x_fm.h"

#ifdef INET_SUPPORT
#include "inet/http/x_httpc_api.h"
#endif
#include "mutil/mm_util_input.h"
#ifdef MW_SM_DVBT_SUPPORT
#include "tbl_mngr/dvb_si_eng/u_dvb_si_eng.h"
#include "tbl_mngr/dvb_si_eng/dvb_si_txt_api.h"
#endif

#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "unicode/x_uc_str.h"

#include "strm_mngr/u_sm.h"

#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
#include "mutil/sbtl_engine/sbtl_drv.h"
#include "mutil/sbtl_engine/x_mm_sbtl_engine.h"
#include "mutil/sbtl_engine/sbtl_widget.h"

#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"

#include "../mm_util_input.h"
#include "x_uc_str.h"
#ifdef MW_SM_DVBT_SUPPORT
#include "u_dvb_si_eng.h"
#include "dvb_si_txt_api.h"
#endif

#include "sbtl_dbg.h"
#include "sbtl_struct.h"
#include "sbtl_utils.h"
#include "sbtl_register.h"
#include "sbtl_state.h"
#include "sbtl_drv.h"
#include "x_mm_sbtl_engine.h"
#include "sbtl_widget.h"

#endif /*__KERNEL__*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define SM_SBTL_PES_BUF_DEFAULT_SIZE        ((SIZE_T)(1024 * 8))
#define SM_SBTL_PES_BUF_INC_SIZE            ((SIZE_T)(1024))

#ifdef MM_SBTL_SIMULATION
#define SIMU_DATA_END           0xFFFFFFFF
#define PTS_FREQ                ((UINT64)(90000))
#define SIMU_TIMER_PERIOD       100
#endif

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
#ifdef MM_SBTL_SIMULATION
#define SIMU_DATA_FILE  "/mnt/usb/Mass-000/M10_dmf_5audio_5subtitle_3menus.divx"

static UINT32  aui4_simu_data1[SBTL_DMX_DATA_QUE_MAX_SIZE] = 
{0x1E6B62, 0x2D7C76, 0x39B150, 0x54A706, 0x6ED69A, 0x786A34, 0x7F34E2, 0x859888, SIMU_DATA_END };

static UINT32  aui4_simu_data2[SBTL_DMX_DATA_QUE_MAX_SIZE] = 
{0x07DA, 0x0930, 0x0BD1, 0x064E, 0x0FA9, 0x0568, 0x0A8E, 0x0862, SIMU_DATA_END };

static PTS_T    t_simu_pts_val = 0;
static HANDLE_T h_simu_timer = NULL_HANDLE;
#endif

static MM_SBTL_RECORD_INF*  _gpt_sbtl_rcd = NULL;

const static UINT16 aui2_cp1252[256] = 
{
    0x0000,
    0x0001,
    0x0002,
    0x0003,
    0x0004,
    0x0005,
    0x0006,
    0x0007,
    0x0008,
    0x0009,
    0x000A,
    0x000B,
    0x000C,
    0x000D,
    0x000E,
    0x000F,
    0x0010,
    0x0011,
    0x0012,
    0x0013,
    0x0014,
    0x0015,
    0x0016,
    0x0017,
    0x0018,
    0x0019,
    0x001A,
    0x001B,
    0x001C,
    0x001D,
    0x001E,
    0x001F,
    0x0020,
    0x0021,
    0x0022,
    0x0023,
    0x0024,
    0x0025,
    0x0026,
    0x0027,
    0x0028,
    0x0029,
    0x002A,
    0x002B,
    0x002C,
    0x002D,
    0x002E,
    0x002F,
    0x0030,
    0x0031,
    0x0032,
    0x0033,
    0x0034,
    0x0035,
    0x0036,
    0x0037,
    0x0038,
    0x0039,
    0x003A,
    0x003B,
    0x003C,
    0x003D,
    0x003E,
    0x003F,
    0x0040,
    0x0041,
    0x0042,
    0x0043,
    0x0044,
    0x0045,
    0x0046,
    0x0047,
    0x0048,
    0x0049,
    0x004A,
    0x004B,
    0x004C,
    0x004D,
    0x004E,
    0x004F,
    0x0050,
    0x0051,
    0x0052,
    0x0053,
    0x0054,
    0x0055,
    0x0056,
    0x0057,
    0x0058,
    0x0059,
    0x005A,
    0x005B,
    0x005C,
    0x005D,
    0x005E,
    0x005F,
    0x0060,
    0x0061,
    0x0062,
    0x0063,
    0x0064,
    0x0065,
    0x0066,
    0x0067,
    0x0068,
    0x0069,
    0x006A,
    0x006B,
    0x006C,
    0x006D,
    0x006E,
    0x006F,
    0x0070,
    0x0071,
    0x0072,
    0x0073,
    0x0074,
    0x0075,
    0x0076,
    0x0077,
    0x0078,
    0x0079,
    0x007A,
    0x007B,
    0x007C,
    0x007D,
    0x007E,
    0x007F,
    0x20AC,
    0x0000,
    0x201A,
    0x0192,
    0x201E,
    0x2026,
    0x2020,
    0x2021,
    0x02C6,
    0x2030,
    0x0160,
    0x2039,
    0x0152,
    0x0000,
    0x017D,
    0x0000,
    0x0000,
    0x2018,
    0x2019,
    0x201C,
    0x201D,
    0x2022,
    0x2013,
    0x2014,
    0x02DC,
    0x2122,
    0x0161,
    0x203A,
    0x0153,
    0x0000,
    0x017E,
    0x0178,
    0x00A0,
    0x00A1,
    0x00A2,
    0x00A3,
    0x00A4,
    0x00A5,
    0x00A6,
    0x00A7,
    0x00A8,
    0x00A9,
    0x00AA,
    0x00AB,
    0x00AC,
    0x00AD,
    0x00AE,
    0x00AF,
    0x00B0,
    0x00B1,
    0x00B2,
    0x00B3,
    0x00B4,
    0x00B5,
    0x00B6,
    0x00B7,
    0x00B8,
    0x00B9,
    0x00BA,
    0x00BB,
    0x00BC,
    0x00BD,
    0x00BE,
    0x00BF,
    0x00C0,
    0x00C1,
    0x00C2,
    0x00C3,
    0x00C4,
    0x00C5,
    0x00C6,
    0x00C7,
    0x00C8,
    0x00C9,
    0x00CA,
    0x00CB,
    0x00CC,
    0x00CD,
    0x00CE,
    0x00CF,
    0x00D0,
    0x00D1,
    0x00D2,
    0x00D3,
    0x00D4,
    0x00D5,
    0x00D6,
    0x00D7,
    0x00D8,
    0x00D9,
    0x00DA,
    0x00DB,
    0x00DC,
    0x00DD,
    0x00DE,
    0x00DF,
    0x00E0,
    0x00E1,
    0x00E2,
    0x00E3,
    0x00E4,
    0x00E5,
    0x00E6,
    0x00E7,
    0x00E8,
    0x00E9,
    0x00EA,
    0x00EB,
    0x00EC,
    0x00ED,
    0x00EE,
    0x00EF,
    0x00F0,
    0x00F1,
    0x00F2,
    0x00F3,
    0x00F4,
    0x00F5,
    0x00F6,
    0x00F7,
    0x00F8,
    0x00F9,
    0x00FA,
    0x00FB,
    0x00FC,
    0x00FD,
    0x00FE,
    0x00FF
};
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
#ifndef MM_SBTL_SIMULATION
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static BOOL _sbtl_dmx_pes_nfy_fct (VOID*            pv_nfy_tag,
                                   DEMUX_COND_T     e_nfy_cond,
                                   UINT32           ui4_data_1,
                                   UINT32           ui4_data_2
                            #ifndef MW_CLASSIC_DMX_NFY
                                   ,
                                   UINT64           ui8_ext_data_1,
                                   UINT64           ui8_ext_data_2,
                                   UINT32           ui4_ext_data_3
                            #endif
                                   )
{
    MM_SBTL_HDLR_MSG_T  t_msg;
    SBTL_ENGINE_T*      pt_engine;
    
    pt_engine = (SBTL_ENGINE_T*) pv_nfy_tag;
    if (e_nfy_cond == DEMUX_COND_OVERFLOW)
    {
        return TRUE;
    }
    
    if (e_nfy_cond == DEMUX_COND_DATA_REC)
    {
        if (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_DIVX)
        {
            mm_sbtl_hdlr_send_dmx_msg(t_msg, pt_engine, e_nfy_cond, ui4_data_1, ui4_data_2, 0, 0, 0);
        }
        else if (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MKV_VOBSUB ||
                 pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MKV_SRT ||
                 pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MKV_SSA ||
                 pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_PS_VOBSUB ||
                 pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MP4_BLURAY ||
                 pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MP4_VOBSUB)
        {
            MKV_PACK_T* pt_mkv_pack;

            if (ui4_data_1 == 0)
            {
                return FALSE;
            }
            
            pt_mkv_pack = (MKV_PACK_T*)ui4_data_1;
            mm_sbtl_hdlr_send_dmx_msg(t_msg, pt_engine, e_nfy_cond, pt_mkv_pack->ui4_data_1, pt_mkv_pack->ui4_data_2, pt_mkv_pack->ui4_pts, pt_mkv_pack->ui4_duration, 0);
        }
        else
        {
            return FALSE;
        }
        return TRUE;
    }
    
    return FALSE;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_open(SBTL_ENGINE_T*      pt_engine,
                    HANDLE_T            h_source)
{
#ifdef MM_SBTL_SIMULATION
    return MMSBTLR_OK;
#else
    SBTL_DMX_PES_FMT_T   e_pes_fmt;

    switch (pt_engine->t_cfg.e_sbtl_type)
    {
    case MM_SBTL_TYPE_DIVX:
        e_pes_fmt = SBTL_DMX_PES_DIVX;
        break;
        
    case MM_SBTL_TYPE_PS_VOBSUB:
        e_pes_fmt = SBTL_DMX_PES_PS;
        break;
        
    case MM_SBTL_TYPE_MKV_VOBSUB:
    case MM_SBTL_TYPE_MKV_SRT:
    case MM_SBTL_TYPE_MKV_SSA:
    case MM_SBTL_TYPE_MP4_BLURAY:
    case MM_SBTL_TYPE_MP4_VOBSUB:
        e_pes_fmt = SBTL_DMX_PES_MKV;
        break;

    default:
        return MMSBTLR_NOT_SUPPORT;
    }
    
    return sbtl_dmx_open_drv(h_source, &pt_engine->h_dmx, e_pes_fmt);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_close(SBTL_ENGINE_T*    pt_engine)
{
#ifdef MM_SBTL_SIMULATION
    return MMSBTLR_OK;
#else
    sbtl_dmx_close_drv(pt_engine->h_dmx);
    pt_engine->h_dmx = NULL_HANDLE;
    return MMSBTLR_OK;
#endif
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

#ifdef MM_SBTL_SIMULATION
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _simu_timer_cb_fct(HANDLE_T     pt_tm_handle,
                               VOID*        pv_tag)
{
    t_simu_pts_val += ((SIMU_TIMER_PERIOD * PTS_FREQ)/1000);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _simu_timer_start(VOID)
{
    INT32   i4_ret;

    if (h_simu_timer)
    {
        x_timer_delete(h_simu_timer);
        h_simu_timer = NULL_HANDLE;
    }

    i4_ret = x_timer_create(&h_simu_timer);
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }

    i4_ret = x_timer_start(h_simu_timer,
                           SIMU_TIMER_PERIOD,
                           X_TIMER_FLAG_REPEAT,
                           _simu_timer_cb_fct,
                           (VOID*)h_simu_timer);
                    
    if (i4_ret != OSR_OK)
    {
        return MMSBTLR_CORE;
    }
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _simu_timer_stop(VOID)
{
    if (h_simu_timer)
    {
        x_timer_delete(h_simu_timer);
        h_simu_timer = NULL_HANDLE;
        t_simu_pts_val = 0;
    }
}
#endif

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_enable(SBTL_ENGINE_T*   pt_engine)
{
#ifdef MM_SBTL_SIMULATION
    MM_SBTL_HDLR_MSG_T  t_msg;
    UINT32              ui4_i;

    for (ui4_i=0; aui4_simu_data1[ui4_i]!=SIMU_DATA_END; ui4_i++)
    {
        mm_sbtl_hdlr_send_dmx_msg(t_msg, pt_engine, (DEMUX_COND_T)0, aui4_simu_data1[ui4_i], aui4_simu_data2[ui4_i], 0);
    }

    return MMSBTLR_OK;
#else
    if(pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_PS_VOBSUB)
    {
        return sbtl_dmx_enable_drv(pt_engine->h_dmx,
                                   pt_engine->t_cfg.t_subtitle_ps.ui2_strm_id,
                                   _sbtl_dmx_pes_nfy_fct,
                                   (VOID*)pt_engine);

    }else if(pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MP4_BLURAY ||
             pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_MP4_VOBSUB)
    {
        return sbtl_dmx_enable_drv(pt_engine->h_dmx,
                                   pt_engine->t_cfg.t_subtitle_mp4.ui4_stream_id,
                                   _sbtl_dmx_pes_nfy_fct,
                                   (VOID*)pt_engine);
    }
    else
    {
        return sbtl_dmx_enable_drv(pt_engine->h_dmx,
                                   pt_engine->t_cfg.t_subtitle_avi.ui4_stream_id,
                                   _sbtl_dmx_pes_nfy_fct,
                                   (VOID*)pt_engine);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_disable(SBTL_ENGINE_T*   pt_engine)
{
#ifdef MM_SBTL_SIMULATION
    return MMSBTLR_OK;
#else

    return sbtl_dmx_disable_drv(pt_engine->h_dmx);
#endif
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_stc_open(SBTL_ENGINE_T*     pt_engine)
{
#ifdef MM_SBTL_SIMULATION
    _simu_timer_start();
    return MMSBTLR_OK;
#else
    return sbtl_stc_open_drv(&(pt_engine->h_stc), 
                             pt_engine->t_cfg.s_dest_grp_name);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_stc_close(SBTL_ENGINE_T*    pt_engine)
{
#ifdef MM_SBTL_SIMULATION
    _simu_timer_stop();
    return MMSBTLR_OK;
#else
    sbtl_stc_close_drv(pt_engine->h_stc);
    pt_engine->h_stc = NULL_HANDLE;
    return MMSBTLR_OK;
#endif
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_stc_pts(SBTL_ENGINE_T*      pt_engine,
                   PTS_T*              pt_pts)
{
#ifdef MM_SBTL_SIMULATION
    *pt_pts = t_simu_pts_val;
    return MMSBTLR_OK;
#else
    if(pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_LRC ||
       pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_KRK_KING ||
       pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_MP3_KRK)
    {
        if(pt_engine->t_cfg.pf_pts_cb != NULL && pt_engine->t_cfg.pv_pts_cb_tag != NULL)
        {
            if(pt_engine->t_cfg.pf_pts_cb(pt_engine->t_cfg.pv_pts_cb_tag, pt_pts))
            {
                *pt_pts =  (*pt_pts)*90000;
                return MMSBTLR_OK;
            }
            else
            {
                return sbtl_stc_pts_drv(pt_engine->h_stc, pt_pts);
            }
        }
        else
        {
            return sbtl_stc_pts_drv(pt_engine->h_stc, pt_pts);
        }
    }
    else
    {
        return sbtl_stc_pts_drv(pt_engine->h_stc, pt_pts);
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_stc_type(SBTL_ENGINE_T*      pt_engine,
                      STC_CTRL_TYPE_T*    pt_type)
{
#ifdef MM_SBTL_SIMULATION
    *pt_pts = t_simu_pts_val;
    return MMSBTLR_OK;
#else
    return sbtl_stc_type_drv(pt_engine->h_stc, pt_type);
#endif
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_retrive_data(SBTL_ENGINE_T*     pt_engine,
                            UINT32             ui4_tag,
                            UINT32             ui4_length,
                            UINT8*             aui1_buf)
{
#ifdef MM_SBTL_SIMULATION
    HANDLE_T    h_file = NULL_HANDLE;
    UINT64      ui8_pos;
    UINT32      ui4_read_cnt;
    
    x_fm_open(FM_ROOT_HANDLE, 
              SIMU_DATA_FILE,
              FM_READ_ONLY, 
              0666, 
              FALSE, 
              &h_file);

    x_fm_lseek(h_file,
               ui4_tag,
               FM_SEEK_BGN,
               &ui8_pos);
    
    x_fm_read(h_file, aui1_buf, ui4_length, &ui4_read_cnt);
    
    x_fm_close(h_file);

    return MMSBTLR_OK;
#else
    return sbtl_dmx_retrive_data_drv(pt_engine->h_dmx,
                                     ui4_tag,
                                     ui4_length,
                                     aui1_buf);
#endif
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_release_data(SBTL_ENGINE_T*     pt_engine,
                            UINT32             ui4_tag,
                            UINT32             ui4_length)
{
#ifdef MM_SBTL_SIMULATION
    return MMSBTLR_OK;
#else /*MM_SBTL_SIMULATION*/
    return sbtl_dmx_release_data_drv(pt_engine->h_dmx,
                                     ui4_tag,
                                     ui4_length);
#endif /*MM_SBTL_SIMULATION*/
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_data_que_open(SBTL_ENGINE_T*  pt_engine)
{
    pt_engine->t_data_que.ui2_first_entry_idx = 0;
    pt_engine->t_data_que.ui2_last_entry_idx = 0;
    pt_engine->t_data_que.ui2_used_entry_cnt = 0;
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_data_que_close(SBTL_ENGINE_T*   pt_engine)
{
    BOOL                            b_found;
    UINT32                          ui4_data_1;
    UINT32                          ui4_data_2;
    UINT32                          ui4_pts;
    UINT32                          ui4_duration;
    PTS_T                           t_stc;
    
    b_found = FALSE;
    ui4_data_1 = 0;
    ui4_data_2 = 0;
    
    while(1)
    {
        b_found = sbtl_dmx_data_que_retrieve(pt_engine,
                                             &ui4_data_1,
                                             &ui4_data_2,
                                             &ui4_pts,
                                             &ui4_duration,
                                             &t_stc);
        if (b_found)
        {
#ifndef MM_SBTL_SIMULATION
            sbtl_dmx_release_data(pt_engine, ui4_data_1, ui4_data_2);
#endif
        }
        else
        {
            break;
        }
    }
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_dmx_data_que_append(SBTL_ENGINE_T*       pt_engine,
                               UINT32               ui4_data_1,
                               UINT32               ui4_data_2,
                               UINT32               ui4_pts,
                               UINT32               ui4_duration,
                               PTS_T                t_stc)
{
    SBTL_DMX_DATA_QUE_T*    pt_data_que;
    
    pt_data_que = &(pt_engine->t_data_que);

#ifndef MM_SBTL_SIMULATION
    if (SBTL_DMX_DATA_QUE_MAX_SIZE == pt_data_que->ui2_used_entry_cnt)
    {
        if (SBTL_COND_STARTED == pt_engine->t_state.ui4_curr_state)
        {
            UINT32  ui4_data_que_1;
            UINT32  ui4_data_que_2;
            UINT32  ui4_pts;
            UINT32  ui4_duration;
            PTS_T   t_stc_que;
            BOOL    b_get;
            
            /* something is wrong, drop frame ??? */
            /*mm_sbtl_hdlr_send_state_msg(t_msg, pt_engine, EVN_MM_SBTL_DRV_ERR);*/
            b_get = sbtl_dmx_data_que_query(pt_engine,
                                            &ui4_data_que_1,
                                            &ui4_data_que_2,
                                            &ui4_pts,
                                            &ui4_duration);
            if (b_get)
            {
                sbtl_dmx_data_que_retrieve(pt_engine,
                                           &ui4_data_que_1,
                                           &ui4_data_que_2,
                                           &ui4_pts,
                                           &ui4_duration, 
                                           &t_stc_que);
                sbtl_dmx_release_data(pt_engine, ui4_data_que_1, ui4_data_que_2);
            }
            else
            {
                sbtl_dmx_release_data(pt_engine, ui4_data_1, ui4_data_2);
                return (MMSBTLR_OK);
            }
        }
        else
        {
            sbtl_dmx_release_data(pt_engine, ui4_data_1, ui4_data_2);
            return (MMSBTLR_OK);
        }
    }
#endif

    if (pt_data_que->ui2_last_entry_idx >= SBTL_DMX_DATA_QUE_MAX_SIZE)
    {
        ASSERT(0, ("sbtl_dmx_data_que_append out of boundary\n"));
        pt_data_que->ui2_first_entry_idx = 0;
        pt_data_que->ui2_last_entry_idx = 0;
        pt_data_que->ui2_used_entry_cnt = 0;

        return MMSBTLR_OK;
    }

    /* append data */
    pt_data_que->aui4_data1[pt_data_que->ui2_last_entry_idx] = ui4_data_1;
    pt_data_que->aui4_data2[pt_data_que->ui2_last_entry_idx] = ui4_data_2;
    pt_data_que->aui4_pts[pt_data_que->ui2_last_entry_idx] = ui4_pts;
    pt_data_que->aui4_duration[pt_data_que->ui2_last_entry_idx] = ui4_duration;

    pt_data_que->at_stc[pt_data_que->ui2_last_entry_idx] = t_stc;
    
    pt_data_que->ui2_last_entry_idx = (UINT16)((pt_data_que->ui2_last_entry_idx + 1) % 
        SBTL_DMX_DATA_QUE_MAX_SIZE);

    pt_data_que->ui2_used_entry_cnt++;


    return MMSBTLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
BOOL sbtl_dmx_data_que_retrieve(SBTL_ENGINE_T*      pt_engine,
                                UINT32*             pui4_data_1,
                                UINT32*             pui4_data_2,
                                UINT32*             pui4_pts,
                                UINT32*             pui4_duration,
                                PTS_T*              pt_stc)
{
    SBTL_DMX_DATA_QUE_T*            pt_data_que;

    pt_data_que = &(pt_engine->t_data_que);

    /* No entry */
    if (0 == pt_data_que->ui2_used_entry_cnt)
    {
        return FALSE;
    }

    if (pt_data_que->ui2_first_entry_idx >= SBTL_DMX_DATA_QUE_MAX_SIZE)
    {
        ASSERT(0, ("sbtl_dmx_data_que_retrieve out of boundary\n"));
        pt_data_que->ui2_first_entry_idx = 0;
        pt_data_que->ui2_last_entry_idx = 0;
        pt_data_que->ui2_used_entry_cnt = 0;

        return FALSE;
    }

    *pui4_data_1 = pt_data_que->aui4_data1[pt_data_que->ui2_first_entry_idx];
    *pui4_data_2 = pt_data_que->aui4_data2[pt_data_que->ui2_first_entry_idx];
    *pui4_pts = pt_data_que->aui4_pts[pt_data_que->ui2_first_entry_idx];
    *pui4_duration = pt_data_que->aui4_duration[pt_data_que->ui2_first_entry_idx];

    *pt_stc = pt_data_que->at_stc[pt_data_que->ui2_first_entry_idx];
    
    pt_data_que->ui2_first_entry_idx = 
        (UINT16)((pt_data_que->ui2_first_entry_idx + 1) % SBTL_DMX_DATA_QUE_MAX_SIZE);
    
    pt_data_que->ui2_used_entry_cnt --;

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
BOOL sbtl_dmx_data_que_query(SBTL_ENGINE_T*     pt_engine,
                             UINT32*            pui4_data_1,
                             UINT32*            pui4_data_2,
                             UINT32*            pui4_pts,
                             UINT32*            pui4_duration)

{
    SBTL_DMX_DATA_QUE_T*            pt_data_que;

    pt_data_que = &(pt_engine->t_data_que);

    /* No entry */
    if (0 == pt_data_que->ui2_used_entry_cnt)
    {
        return FALSE;
    }

    if (pt_data_que->ui2_first_entry_idx >= SBTL_DMX_DATA_QUE_MAX_SIZE)
    {
        ASSERT(0, ("sbtl_dmx_data_que_query out of boundary\n"));
        pt_data_que->ui2_first_entry_idx = 0;
        pt_data_que->ui2_last_entry_idx = 0;
        pt_data_que->ui2_used_entry_cnt = 0;

        return FALSE;
    }
    
    *pui4_data_1 = pt_data_que->aui4_data1[pt_data_que->ui2_first_entry_idx];
    *pui4_data_2 = pt_data_que->aui4_data2[pt_data_que->ui2_first_entry_idx];
    *pui4_pts = pt_data_que->aui4_pts[pt_data_que->ui2_first_entry_idx];
    *pui4_duration = pt_data_que->aui4_duration[pt_data_que->ui2_first_entry_idx];

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_init(SBTL_ENGINE_T*          pt_engine,
                        MM_SBTL_FEEDER_TYPE_T   e_type)
{
    MEDIA_FEEDER_T* pt_feeder;
    setpos_fct      pf_set_pos;
    getpos_fct      pf_get_pos;
    UINT64          ui8_pos_bgn;
    UINT64          ui8_pos_end;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }
    
    pt_feeder = pt_engine->t_cfg.pt_feeder[e_type];
        
    ASSERT(pt_feeder, ("pt_feeder=NULL\n"));

    pf_get_pos = pt_feeder->t_feeder_be_fct_tbl.pf_get_pos;
    pf_set_pos = pt_feeder->t_feeder_be_fct_tbl.pf_set_pos;
    pf_set_pos(pt_feeder, 0, MEDIA_SEEK_END);
    pf_get_pos(pt_feeder, &ui8_pos_end);
    pf_set_pos(pt_feeder, 0, MEDIA_SEEK_BGN);
    pf_get_pos(pt_feeder, &ui8_pos_bgn);

    pt_engine->aui8_file_size[e_type] = ui8_pos_end-ui8_pos_bgn;
    pt_engine->aui4_read_count[e_type] = 0;
    pt_engine->aui8_pos[e_type] = 0;
    pt_engine->aui8_cache_pos[e_type] = 0;

    sbtl_feeder_setpos(pt_engine, 0, MEDIA_SEEK_BGN, e_type);
    
    return MMSBTLR_OK;
}

UINT8* sbtl_feeder_get_data_addr(
    SBTL_ENGINE_T*                  pt_engine,
    MM_SBTL_FEEDER_TYPE_T           e_type)
{
    UINT64          ui8_offset;

    ui8_offset = pt_engine->aui8_pos[e_type] - pt_engine->aui8_cache_pos[e_type];
    return &(pt_engine->aui1_cache_buf[e_type][ui8_offset]); 
}

INT32 sbtl_feeder_get_file_size(
    SBTL_ENGINE_T*            pt_engine,
    UINT64*                   pui8_offset,
    MM_SBTL_FEEDER_TYPE_T     e_type)
{
    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }

    *pui8_offset = pt_engine->aui8_file_size[e_type];
    return MMSBTLR_OK;
}

INT32 sbtl_feeder_get_cache_endpos(
    SBTL_ENGINE_T*            pt_engine,
    UINT64*                   pui8_offset,
    MM_SBTL_FEEDER_TYPE_T     e_type)
{
    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }

    *pui8_offset = pt_engine->aui8_cache_pos[e_type] + pt_engine->aui4_read_count[e_type];
    return MMSBTLR_OK;
}

INT32 sbtl_feeder_render_new_cache(
    SBTL_ENGINE_T*                  pt_engine,
    MM_SBTL_FEEDER_TYPE_T           e_type)
{
    UINT64          ui8_pos;
    copybytes_fct   pf_copybytes;
    setpos_fct      pf_set_pos;
    MEDIA_FEEDER_T* pt_feeder;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }

    pt_feeder = pt_engine->t_cfg.pt_feeder[e_type];
    ASSERT(pt_feeder, ("pt_feeder=NULL\n"));
    
    pf_copybytes = pt_feeder->t_feeder_be_fct_tbl.pf_copybytes;
    pf_set_pos = pt_feeder->t_feeder_be_fct_tbl.pf_set_pos;
    
    sbtl_feeder_getpos(pt_engine, &ui8_pos, e_type);
    
    if (ui8_pos+1 > pt_engine->aui8_file_size[e_type])
    {
        return MMSBTLR_EOT;
    }

    pf_set_pos(pt_feeder, (INT64)pt_engine->aui8_pos[e_type], MEDIA_SEEK_BGN);
    pf_copybytes(pt_feeder, 
                 pt_engine->aui1_cache_buf[e_type], 
                 FEEDER_CACHE_SIZE, 
                 FEEDER_CACHE_SIZE,
                 &pt_engine->aui4_read_count[e_type]);

    pt_engine->aui8_cache_pos[e_type] = pt_engine->aui8_pos[e_type];
    
    return MMSBTLR_OK;
}

INT32 sbtl_feeder_inpnbyte_p(SBTL_ENGINE_T*          pt_engine,
                                     UINT32                  ui4_len,
                                     UINT8**                 ppui1_buf,
                                     UINT32*                 pui4_done_bytes,
                                     MM_SBTL_FEEDER_TYPE_T   e_type)
{
    UINT64          ui8_pos;
    UINT64          ui8_read_end,ui8_cache_end;
    UINT64          ui8_offset = 0;
    UINT32          ui4_read_bytes = 0;
    INT32           i4_ret = MMSBTLR_OK;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }

    if(pt_engine->aui4_read_count[e_type] == 0)
    {
       i4_ret = sbtl_feeder_render_new_cache(pt_engine,e_type);
       if(i4_ret != MMSBTLR_OK)
       {
            if(pui4_done_bytes != NULL)
            {
                *pui4_done_bytes = 0;
            }
            return MMSBTLR_EOT;
       }
    }
    
    ui8_pos = pt_engine->aui8_pos[e_type];
    ui8_read_end = ui8_pos + (UINT64)ui4_len;
    ui8_cache_end = pt_engine->aui8_cache_pos[e_type] + pt_engine->aui4_read_count[e_type];

    if(ui8_read_end <= ui8_cache_end)
    { /*The read data is in the cache buffer.*/
        ui8_offset = pt_engine->aui8_pos[e_type] - pt_engine->aui8_cache_pos[e_type];
       (*ppui1_buf) = &(pt_engine->aui1_cache_buf[e_type][ui8_offset]); 
        ui4_read_bytes = ui4_len;
    }
    else if(ui8_cache_end <= pt_engine->aui8_file_size[e_type])
    {  
        /*The part of read data range is out of cache.Get new cache page from current position.*/
       i4_ret = sbtl_feeder_render_new_cache(pt_engine,e_type);
       if(i4_ret != MMSBTLR_OK)
       {
            ui4_read_bytes = 0;
            i4_ret = MMSBTLR_EOT;
       }
       else if(ui4_len <= pt_engine->aui4_read_count[e_type])
       {
            (*ppui1_buf) = &(pt_engine->aui1_cache_buf[e_type][0]); 
            ui4_read_bytes = ui4_len;
       }         
       else
       {
            (*ppui1_buf) = &(pt_engine->aui1_cache_buf[e_type][0]); 
            ui4_read_bytes = pt_engine->aui4_read_count[e_type];
            i4_ret = MMSBTLR_EOT;
       }
    }
    
    pt_engine->aui8_pos[e_type] += ui4_read_bytes;
    if(pui4_done_bytes != NULL)
    {
        *pui4_done_bytes = ui4_read_bytes;
    }

    return i4_ret;
}

INT32 sbtl_feeder_inpnbyte_addr_p(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    UINT32                          ui4_len,
    UINT8**                         ppui1_buf,
    UINT32*                         pui4_done_bytes,
    MM_SBTL_FEEDER_TYPE_T           e_type)
{
    INT32                           i4_ret = MMSBTLR_OK;

    ASSERT(pt_engine, ("pt_feeder=NULL\n"));
    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }
    
    if (ui8_start_pos > pt_engine->aui8_file_size[e_type])
    {
        if(pui4_done_bytes != NULL)
        {
            *pui4_done_bytes = 0;
        }
        return MMSBTLR_EOT;
    }

    sbtl_feeder_setpos(pt_engine, (INT64)ui8_start_pos, MEDIA_SEEK_BGN, e_type);

    i4_ret = sbtl_feeder_inpnbyte_p(pt_engine,ui4_len,ppui1_buf,pui4_done_bytes,e_type);
    
    return i4_ret;
}

UINT8 sbtl_tag_fmt_check(
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf,
    UINT8*                          pui1_buf,
    UINT8*                          pui1_fmt_len,
    UINT8*                          pui1_fmt_idx,
    UINT32                          ui4_read_len,
    BOOL                            b_UTF16)
{
    UINT8                           ui1_fmt;
    UINT8                           ui1_i = 0;
    UINT8                           ui1_j = 0;
    UINT8                           ui1_pos = 0;
    UINT8                           ui1_count;
    UINT16                          ui2_tmp1,ui2_tmp2;
    UINT16*                         pui2_buf;
    INT32                           i4_ret = SBTL_TIME_TAG_NOT_FOUND;

    if((pt_tag_inf == NULL) || (pui1_buf == NULL))
    {
        return SBTL_TIME_TAG_NOT_FOUND;
    }

    pui2_buf = (UINT16*)pui1_buf;
    
    for(ui1_fmt = 0; ui1_fmt < ui1_fmt_cnt; ui1_fmt ++)
    {
        if(ui4_read_len < pt_tag_inf[ui1_fmt].ui1_fmt_len)
        {
            i4_ret = SBTL_TIME_TAG_NOT_FOUND;
            continue;
        }
        ui1_count = 0;
        ui1_j = 0;
        for(ui1_i = 0; ui1_i < pt_tag_inf[ui1_fmt].ui1_fmt_len; ui1_i ++)
        {
            if(b_UTF16 == TRUE)
            {
                ui2_tmp2 = (UINT16)pui2_buf[ui1_i];
            }
            else
            {
                ui2_tmp2 = (UINT16)pui1_buf[ui1_i];
            }
            if(ui1_j >= pt_tag_inf[ui1_fmt].ui1_len)
            {
                ui1_j = pt_tag_inf[ui1_fmt].ui1_len - 1;
            }
            if(ui1_j >= SBTL_TIME_FORMAT_LEN)
            {
                break;
            }
            ui1_pos = pt_tag_inf[ui1_fmt].ui1_content_pos[ui1_j];
            if(ui1_i == ui1_pos)
            {
                ui2_tmp1 = (UINT16)pt_tag_inf[ui1_fmt].s_content[ui1_j];
                ui1_j ++;
                
                if(ui2_tmp1 == ui2_tmp2)
                {
                    if(b_UTF16 == TRUE)
                    {
                        ui1_count += 2;
                    }
                    else
                    {
                        ui1_count ++;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(pt_tag_inf[ui1_fmt].b_check_num)
                {
                    if(ui2_tmp2 >= '0' && ui2_tmp2 <='9')
                    {
                        if(b_UTF16 == TRUE)
                        {
                            ui1_count += 2;
                        }
                        else
                        {
                            ui1_count ++;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                     if(b_UTF16 == TRUE)
                    {
                        ui1_count += 2;
                    }
                    else
                    {
                        ui1_count ++;
                    }
                }
            }
        }

        if(ui1_count >= pt_tag_inf[ui1_fmt].ui1_fmt_len)
        {
            if(pui1_fmt_idx != NULL)
            {
                *pui1_fmt_idx = ui1_fmt;
            }

            if (pui1_fmt_len != NULL)
            {
                *pui1_fmt_len = pt_tag_inf[ui1_fmt].ui1_fmt_len;
            }
            return SBTL_TIME_TAG_FOUND;
        }
    }

    return i4_ret;
}

UINT32 sbtl_get_tag_fmt_max_len(
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf)
{
    UINT8                           ui1_max_len = 0;
    UINT8                           ui1_i;
    
    if(pt_tag_inf == NULL)
    {
        return 0;
    }   
    
    for(ui1_i = 0; ui1_i < ui1_fmt_cnt; ui1_i ++)
    {
        if(pt_tag_inf[ui1_i].ui1_fmt_len > ui1_max_len)
        {
            ui1_max_len = pt_tag_inf[ui1_i].ui1_fmt_len;
        }
    }
    
    return (UINT32)ui1_max_len;
}

UINT32 sbtl_get_tag_fmt_min_len(
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf)
{
    UINT8                           ui1_min_len = 0xff;
    UINT8                           ui1_i;
    
    if(pt_tag_inf == NULL)
    {
        return 0;
    }   
    
    for(ui1_i = 0; ui1_i < ui1_fmt_cnt; ui1_i ++)
    {
        if(pt_tag_inf[ui1_i].ui1_fmt_len < ui1_min_len)
        {
            ui1_min_len = pt_tag_inf[ui1_i].ui1_fmt_len;
        }
    }
    
    return (UINT32)ui1_min_len;
}

INT32 sbtl_tag_inf_seek(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    BOOL                            b_found_total,
    UINT8**                         ppui1_buf,
    UINT64*                         pui8_seek_len,
    UINT16*                         pui2_count,
    UINT8*                          pui1_fmt_idx,
    BOOL                            b_UTF16)
{
    INT32                           i4_ret = MMSBTLR_OK;
    UINT32                          ui4_read_len;
    UINT32                          ui4_leave_len;
    UINT8*                          pui1_buf_s = 0;
    UINT64                          ui8_pos = 0;
    UINT8                           ui1_ret;
    UINT8                           ui1_fmt_len;
    //UINT32                          ui4_fmt_max;
    UINT32                          ui4_fmt_min;
    #if 0
    UINT8                           at_name[20];
    #endif

    ASSERT(pt_engine, ("pt_feeder=NULL\n"));
    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        return MMSBTLR_INV_ARG;
    }

    if ((ui1_fmt_cnt == 0) || (pt_tag_inf == NULL) || (pui2_count == NULL) ||
        ((b_found_total == FALSE) && ((ppui1_buf == NULL) || (pui8_seek_len == NULL))) )
    {
        return MMSBTLR_INV_ARG;
    }

    //ui4_fmt_max = sbtl_get_tag_fmt_max_len(ui1_fmt_cnt, pt_tag_inf);
    ui4_fmt_min = sbtl_get_tag_fmt_min_len(ui1_fmt_cnt, pt_tag_inf);
    *pui2_count = 0;
    ui8_pos = ui8_start_pos;
    
    while(1)
    {
        if(b_UTF16 == TRUE)
        {
            ui8_pos = (ui8_pos >> 1) << 1;
        }
        
        sbtl_feeder_inpnbyte_addr_p(pt_engine,ui8_pos,FEEDER_CACHE_SIZE,&pui1_buf_s,&ui4_read_len,e_type);

        if ((ui4_read_len < ui4_fmt_min) || (pui1_buf_s == 0))
        {
            if(pui8_seek_len != NULL)
            {
            *pui8_seek_len = ui4_read_len;
            }

            i4_ret = MMSBTLR_EOT;
            break;
        }
    
        /*Seek time tag in one cache range buffer.*/
        ui4_leave_len = ui4_read_len;
        while(ui4_leave_len > 0)
        {
            if(ui4_leave_len < ui4_fmt_min)
            {
                break;
            }
            
            /*Check the time format.*/
            ui1_ret = sbtl_tag_fmt_check(ui1_fmt_cnt,
                                            pt_tag_inf,
                                            pui1_buf_s,
                                            &ui1_fmt_len,
                                            pui1_fmt_idx,
                                            ui4_leave_len,
                                            b_UTF16);

            if (ui1_ret == SBTL_TIME_TAG_OUT)
            {
                /*This cache buffer is seek in the end.*/
                break;
            }
            else if (ui1_ret == SBTL_TIME_TAG_FOUND)
            {
            #if 0
                x_memcpy(at_name,pui1_buf_s,ui1_fmt_len);
                at_name[ui1_fmt_len+1] = '\0';
                x_dbg_stmt("->found time:%s\n",at_name);
                if((*pui2_count) == 52)
                {
                    x_dbg_stmt("->found time:%s\n",at_name);
                }
            #endif                
                (*pui2_count) ++;
                if((b_found_total == FALSE) && ((*pui2_count) > 0))
                {
                    break;
                }
                pui1_buf_s += ui1_fmt_len;
                ui4_leave_len -= ui1_fmt_len;
            }
            else if(b_UTF16 == TRUE) 
            {
                pui1_buf_s += 2;
                ui4_leave_len -= 2;
            }                
            else
            {
                pui1_buf_s ++;
                ui4_leave_len --;
            }
        }

        /*if (((*pui2_count) > 0) && (b_found_total == FALSE))
        {
            ui8_pos = ui8_pos + ui4_read_len - ui4_leave_len;
            sbtl_feeder_inpnbyte_addr_p(pt_engine,ui8_pos,ui1_fmt_len,ppui1_buf,&ui4_read_len,e_type);
            *pui8_seek_len = ui8_pos + ui1_fmt_len - ui8_start_pos;
            break;
        }
        else
        {
            if((*pui2_count) == 0 && ui4_read_len < FEEDER_CACHE_SIZE)
            {
                *pui8_seek_len = ui4_read_len;
                i4_ret = MMSBTLR_EOT;
                break;
            }
        }*/
        if(b_found_total == FALSE)
        {
            if((*pui2_count) > 0)
            {
                ui8_pos = ui8_pos + ui4_read_len - ui4_leave_len;
                sbtl_feeder_inpnbyte_addr_p(pt_engine,ui8_pos,ui1_fmt_len,ppui1_buf,&ui4_read_len,e_type);
                *pui8_seek_len = ui8_pos + ui1_fmt_len - ui8_start_pos;
                break;
            }
            else
            {
                if(ui4_read_len < FEEDER_CACHE_SIZE)
                {
                    *pui8_seek_len = ui4_read_len;
                    i4_ret = MMSBTLR_EOT;
                    break;
                }
            }
        }
        else
        {
            if(ui4_read_len < FEEDER_CACHE_SIZE)
            {
                break;
            }
        }

        ui8_pos = ui8_pos + ui4_read_len - ui4_leave_len;
    }

    return i4_ret;
}

INT32 sbtl_seek_tag_inf(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    SBTL_TAG_FORMAT_T*              pt_start_tag,
    SBTL_TAG_FORMAT_T*              pt_end_tag,
    UINT8**                         ppui1_buf,
    UINT64*                         pui8_offset,
    UINT64*                         pui8_len,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    BOOL                            b_UTF16)
{
    UINT8*                              pui1_buf = NULL; 
    UINT64                              ui8_seek_len;
    UINT64                              ui8_pos = 0;
    UINT16                              ui2_found_cnt;
    UINT8                               ui1_fmt; 
    UINT32                              ui4_done;

    ASSERT(pt_engine, ("pt_feeder=NULL\n"));
    if ((e_type >= MM_SBTL_FEEDER_MAX) || (pt_start_tag == NULL) || (pt_end_tag == NULL) ||
        (pt_start_tag->ui1_fmt_len == 0) || (pt_end_tag->ui1_fmt_len == 0))
    {
        return MMSBTLR_INV_ARG;
    }

    ui2_found_cnt = 0;
    ui8_seek_len = 0;
    ui1_fmt = 0;
    sbtl_tag_inf_seek(pt_engine,
           ui8_start_pos,
           1,
           pt_start_tag,
           e_type,
           FALSE,
           &pui1_buf,
           &ui8_seek_len,
           &ui2_found_cnt,
           &ui1_fmt,
           b_UTF16); 
    
    if(ui2_found_cnt == 0)
    {
        return MMSBTLR_NOT_FOUND;
    }
    
    ui8_pos = ui8_start_pos + ui8_seek_len;
    sbtl_tag_inf_seek(pt_engine,
           ui8_pos,
           1,
           pt_end_tag,
           e_type,
           FALSE,
           &pui1_buf,
           &ui8_seek_len,
           &ui2_found_cnt,
           &ui1_fmt,
           b_UTF16); 
    
    if(ui2_found_cnt == 0)
    {
        return MMSBTLR_NOT_FOUND;
    }

    ui4_done = 0;
    sbtl_feeder_inpnbyte_addr_p(pt_engine,ui8_pos,(UINT32)ui8_seek_len,&pui1_buf,&ui4_done,e_type);  
    if(ui4_done == 0)
    {
        return MMSBTLR_NOT_FOUND;
    }

    if(pui8_offset != NULL)
    {
        *pui8_offset = ui8_pos;
    }
    
    if(pui8_len != NULL)
    {
        *pui8_len = ui4_done - pt_end_tag->ui1_fmt_len;
    }

    if(ppui1_buf != NULL)
    {
        *ppui1_buf = pui1_buf;
    }
    
    return MMSBTLR_OK;
}

BOOL sbtl_check_tag_inf_by_addr(
    SBTL_ENGINE_T*                  pt_engine,
    UINT64                          ui8_start_pos,
    UINT8                           ui1_fmt_cnt,
    SBTL_TAG_FORMAT_T*              pt_tag_inf,
    MM_SBTL_FEEDER_TYPE_T           e_type,
    UINT8**                         ppui1_buf,
    UINT8*                          pui1_fmt_idx,
    BOOL                            b_UTF16)
{
    UINT8                           ui1_fmt;
    UINT8                           ui1_count;
    UINT8                           ui1_i;
    UINT8                           ui1_j = 0;
    UINT8                           ui1_pos;
    UINT8*                          pui1_buf;
    UINT16*                         pui2_buf;
    UINT32                          ui4_read_len;
    UINT16                          ui2_tmp1, ui2_tmp2;
    
    ASSERT(pt_engine, ("pt_feeder=NULL\n"));
    ASSERT((e_type < MM_SBTL_FEEDER_MAX), ("e_type is invalid\n"));

    if ((ui1_fmt_cnt == 0) || (pt_tag_inf == NULL))
    {
        return FALSE;
    }

    for(ui1_fmt = 0; ui1_fmt < ui1_fmt_cnt; ui1_fmt ++)
    {
        ui1_count = 0;
        ui1_j = 0;
        sbtl_feeder_inpnbyte_addr_p(pt_engine,
                                    ui8_start_pos,
                                    (UINT32)pt_tag_inf[ui1_fmt].ui1_fmt_len,
                                    &pui1_buf,
                                    &ui4_read_len,
                                    e_type);
        pui2_buf = (UINT16*)pui1_buf;
        if(ui4_read_len >= (UINT32)pt_tag_inf[ui1_fmt].ui1_fmt_len)
        {
            for(ui1_i = 0; ui1_i < pt_tag_inf[ui1_fmt].ui1_fmt_len; ui1_i ++)
            {              
                if(b_UTF16)
                {
                    ui2_tmp2 = pui2_buf[ui1_i];
                }
                else
                {
                    ui2_tmp2 = (UINT16)pui1_buf[ui1_i];
                }
                if(ui1_j >= pt_tag_inf[ui1_fmt].ui1_len)
                {
                    ui1_j = pt_tag_inf[ui1_fmt].ui1_len - 1;
                }
                if(ui1_j >= SBTL_TIME_FORMAT_LEN)
                {
                    break;
                }
                ui1_pos = pt_tag_inf[ui1_fmt].ui1_content_pos[ui1_j];
                if(ui1_i == ui1_pos)
                {
                    ui2_tmp1 = (UINT16)pt_tag_inf[ui1_fmt].s_content[ui1_j];
                    ui1_j ++;
                    
                    if(ui2_tmp1 == ui2_tmp2)
                    {
                        if(b_UTF16 == TRUE)
                        {
                            ui1_count += 2;
                        }
                        else
                        {
                            ui1_count ++;
                        }

                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if(pt_tag_inf[ui1_fmt].b_check_num)
                    {
                        if(ui2_tmp2 >= '0' && ui2_tmp2 <='9')
                        {
                            if(b_UTF16 == TRUE)
                            {
                                ui1_count += 2;
                            }
                            else
                            {
                                ui1_count ++;
                            }

                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if(b_UTF16 == TRUE)
                        {
                            ui1_count += 2;
                        }
                        else
                        {
                            ui1_count ++;
                        }

                    }
                }
                if(ui1_count >= pt_tag_inf[ui1_fmt].ui1_fmt_len)
                {
                    if(pui1_fmt_idx != NULL)
                    {
                        *pui1_fmt_idx = ui1_fmt;
                    }

                    if(ppui1_buf != NULL)
                    {
                        *ppui1_buf = pui1_buf;
                    }
                    return TRUE;
                }
            }
            
        }

    }
    return FALSE;
}
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_inp1byte(SBTL_ENGINE_T*          pt_engine,
                           UINT8*                  pui1_data,
                           MM_SBTL_FEEDER_TYPE_T   e_type)
{
    UINT64          ui8_pos;
    copybytes_fct   pf_copybytes;
    setpos_fct      pf_set_pos;
    MEDIA_FEEDER_T* pt_feeder;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }
    
    pt_feeder = pt_engine->t_cfg.pt_feeder[e_type];
    ASSERT(pt_feeder, ("pt_feeder=NULL\n"));
    
    pf_copybytes = pt_feeder->t_feeder_be_fct_tbl.pf_copybytes;
    pf_set_pos = pt_feeder->t_feeder_be_fct_tbl.pf_set_pos;
    
    sbtl_feeder_getpos(pt_engine, &ui8_pos, e_type);
    
    if (ui8_pos+1 > pt_engine->aui8_file_size[e_type])
    {
        return MMSBTLR_EOT;
    }

    if (0 == pt_engine->aui4_read_count[e_type]
        || pt_engine->aui8_pos[e_type] < pt_engine->aui8_cache_pos[e_type]
        || pt_engine->aui8_pos[e_type]+1 > pt_engine->aui8_cache_pos[e_type]+pt_engine->aui4_read_count[e_type])
    {
        pf_set_pos(pt_feeder, (INT64)pt_engine->aui8_pos[e_type], MEDIA_SEEK_BGN);
        pf_copybytes(pt_feeder, 
                     pt_engine->aui1_cache_buf[e_type], 
                     FEEDER_CACHE_SIZE, 
                     FEEDER_CACHE_SIZE,
                     &pt_engine->aui4_read_count[e_type]);
        
        pt_engine->aui8_cache_pos[e_type] = pt_engine->aui8_pos[e_type];
    }

    *pui1_data = pt_engine->aui1_cache_buf[e_type][pt_engine->aui8_pos[e_type]-pt_engine->aui8_cache_pos[e_type]];
    pt_engine->aui8_pos[e_type]++;
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_inp2byte(SBTL_ENGINE_T*          pt_engine,
                           UINT16*                 pui2_data,
                           MM_SBTL_FEEDER_TYPE_T   e_type)
{
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        ASSERT(0, ("wrong feeder type\n"));
    }

    *pui2_data = 0;
    for (ui1_idx=0; ui1_idx<2; ui1_idx++)
    {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, e_type);
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }
        
        *pui2_data = (((*pui2_data)<<8)|ui1_tmp);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 _sbtl_feeder_inp3byte(SBTL_ENGINE_T*          pt_engine,
                            UINT32*                 pui4_data,
                            MM_SBTL_FEEDER_TYPE_T   e_type)
{
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        ASSERT(0, ("wrong feeder type\n"));
    }

    *pui4_data = 0;
    
    for (ui1_idx=0; ui1_idx<3; ui1_idx++)
    {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, e_type);
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }
        
        *pui4_data = (((*pui4_data)<<8)|ui1_tmp);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_inp4byte(SBTL_ENGINE_T*          pt_engine,
                           UINT32*                 pui4_data,
                           MM_SBTL_FEEDER_TYPE_T   e_type)
{
    UINT8           ui1_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        ASSERT(0, ("wrong feeder type\n"));
    }

    for (ui1_idx=0; ui1_idx<4; ui1_idx++)
    {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, e_type);
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }
        
        *pui4_data = (((*pui4_data)<<8)|ui1_tmp);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_inpnbyte(SBTL_ENGINE_T*          pt_engine,
                           UINT32                  ui4_len,
                           UINT8*                  aui_buf,
                           UINT32*                 pui4_done_bytes,
                           MM_SBTL_FEEDER_TYPE_T   e_type)
{
    UINT32          ui4_idx;
    UINT8           ui1_tmp;
    INT32           i4_ret;
    
    i4_ret = MMSBTLR_OK;
    
    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        ASSERT(0, ("wrong feeder type\n"));
    }

    if (pui4_done_bytes)
    {
        *pui4_done_bytes = 0;
    }
    
    for (ui4_idx=0; ui4_idx<ui4_len; ui4_idx++)
    {
        i4_ret = sbtl_feeder_inp1byte(pt_engine, &ui1_tmp, e_type);
        if (i4_ret != MMSBTLR_OK)
        {
            return i4_ret;
        }

        if (pui4_done_bytes)
        {
            *pui4_done_bytes = *pui4_done_bytes + 1;
        }
        
        aui_buf[ui4_idx] = ui1_tmp;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_setpos(SBTL_ENGINE_T*            pt_engine,
                         INT64                     i8_offset, 
                         UINT8                     ui1_whence,
                         MM_SBTL_FEEDER_TYPE_T     e_type)
{ 
    INT64           i8_pos;
    setpos_fct      pf_set_pos;
    MEDIA_FEEDER_T* pt_feeder;

    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }

    pt_feeder = pt_engine->t_cfg.pt_feeder[e_type];
    ASSERT(pt_feeder, ("pt_feeder=NULL\n"));
        
    pf_set_pos = pt_feeder->t_feeder_be_fct_tbl.pf_set_pos;

    switch (ui1_whence)
    {
        case MEDIA_SEEK_BGN:
            i8_pos = i8_offset;
            break;
            
        case MEDIA_SEEK_CUR: 
            i8_pos = (INT64)(pt_engine->aui8_pos[e_type]);
            i8_pos = i8_pos + i8_offset;
            break;
            
        case MEDIA_SEEK_END:
            i8_pos = (INT64)(pt_engine->aui8_file_size[e_type]);
            i8_pos = i8_pos + i8_offset;
            break;
            
        default:
            return MMSBTLR_INV_ARG;
    }
    
    if (i8_pos > (INT64)(pt_engine->aui8_file_size[e_type]))
    {
        return MMSBTLR_EOT;
    }
    if (i8_pos < 0)
    {
        return MMSBTLR_EOT;
    }

    pt_engine->aui8_pos[e_type] = (UINT64)i8_pos;
    
    if (pt_engine->aui8_pos[e_type] < pt_engine->aui8_cache_pos[e_type]
        || pt_engine->aui8_pos[e_type]+1 > pt_engine->aui8_cache_pos[e_type]+pt_engine->aui4_read_count[e_type])
    {
        pt_engine->aui4_read_count[e_type] = 0;
        pf_set_pos(pt_feeder, i8_pos, MEDIA_SEEK_BGN);
    }
    
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 sbtl_feeder_getpos(SBTL_ENGINE_T*            pt_engine,
                         UINT64*                   pui8_offset,
                         MM_SBTL_FEEDER_TYPE_T     e_type)
{
    if (e_type >= MM_SBTL_FEEDER_MAX)
    {
        //ASSERT(0, ("wrong feeder type\n"));
        return MMSBTLR_INV_ARG;
    }

    *pui8_offset = pt_engine->aui8_pos[e_type];
    return MMSBTLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
UINT32 sbtl_asci2decimal(UINT8*    aui1_ary,
                         UINT8     ui1_round)
{
    UINT32  ui4_result = 0;
    UINT32  ui4_multiply = 1;
    while(ui1_round)
    {
        ui1_round -= 1;
        ui4_result += (aui1_ary[ui1_round]-'0')*ui4_multiply;
        ui4_multiply *= 10;
    }

    return ui4_result;
}

UINT32 sbtl_wasci2decimal(UINT16*     aui2_ary,
                          UINT8       ui1_round)
{
    UINT32  ui4_result = 0;
    UINT32  ui4_multiply = 1;
    while(ui1_round)
    {
        ui1_round -= 1;
        ui4_result += (aui2_ary[ui1_round]-'0')*ui4_multiply;
        ui4_multiply *= 10;
    }

    return ui4_result;
}

UINT16 sbtl_asci2cp1252(UINT8    ui1_asci)
{
    return aui2_cp1252[ui1_asci];
}

UINT32 sbtl_get_utf16_string_len(UINT16* pui2_buf, UINT32 ui4_max_len)
{
    UINT32              ui4_len;

    ui4_len = 0;
    
    while(1)
    {
        if(ui4_len >= ui4_max_len)
        {
            break;
        }

        if(pui2_buf[ui4_len] == 0)
        {
            break;
        }
        else
        {
            ui4_len ++;
        }
    }
    
    return ui4_len;
}
#ifndef __KERNEL__
#ifdef MW_SM_DVBT_SUPPORT
INT32 sbtl_convert_encoding(MM_SBTL_ENCODING_T e_sbtl_encoding, 
                                    E_DVB_TEXT_CHAR_TBL* pe_dvb_encoding)
{
    if (pe_dvb_encoding == NULL || e_sbtl_encoding >= MM_SBTL_ENCODE_END)
    {
        return MMSBTLR_INV_ARG;
    }
    switch(e_sbtl_encoding)
    {
        case MM_SBTL_ENCODE_ISO8859_2:
            *pe_dvb_encoding = DVB_TEXT_CHAR_CODE_ISO_8859_2;
            break;
            
        case MM_SBTL_ENCODE_ISO8859_5:
            *pe_dvb_encoding = DVB_TEXT_CHAR_CODE_ISO_8859_5;
            break;
            
        case MM_SBTL_ENCODE_ISO8859_7:
            *pe_dvb_encoding = DVB_TEXT_CHAR_CODE_ISO_8859_7;
            break;
            
        case MM_SBTL_ENCODE_ISO8859_8:
            *pe_dvb_encoding = DVB_TEXT_CHAR_CODE_ISO_8859_8;
            break;
            
        case MM_SBTL_ENCODE_ISO8859_9:
            *pe_dvb_encoding = DVB_TEXT_CHAR_CODE_ISO_8859_9;
            break;
            
        case MM_SBTL_ENCODE_ISO8859_16:
            *pe_dvb_encoding = DVB_TEXT_CHAR_CODE_ISO_8859_16;
            break;

        default:
            return DVB_TEXT_CHAR_CODE_ISO_8859_1;
    }

    return MMSBTLR_OK;
}
#endif
#endif

INT32 sbtl_trans_to_w2s(MM_SBTL_LANG_T          e_language,
                            UINT16                  ui2_data_in,
                            UINT16*                 pui2_data_out)
{
    UINT8           ui1_tmp;
    UINT32          ui4_offset = 0;
    UINT32          ui4_count = 2;

    if(pui2_data_out == NULL || e_language >= MM_SBTL_LANG_END)
    {
        return MMSBTLR_INV_ARG;
    }

    ui1_tmp = (UINT8)ui2_data_in;
    
    switch(e_language)
    {
        case MM_SBTL_LANG_ASCI:
            *pui2_data_out = sbtl_asci2cp1252(ui1_tmp);
            break;

        case MM_SBTL_LANG_UNICODE:
            *pui2_data_out = ui2_data_in;
            break;

        case MM_SBTL_LANG_BIG5:
            sbtl_multi_byte_to_unicode(&ui2_data_in, 2, &ui4_offset, MUL_BYTE_BIG5, pui2_data_out, &ui4_count);
            break;

        case MM_SBTL_LANG_BG2312:
            sbtl_multi_byte_to_unicode(&ui2_data_in, 2, &ui4_offset, MUL_BYTE_GB2312, pui2_data_out, &ui4_count);
            break;

        default:
            break;
    }

    return MMSBTLR_OK;
}


INT32 sbtl_trans_to_w2s_ex(MM_SBTL_ENCODING_T          e_encoding,
                            UINT8*                  pui1_src,
                            UINT32                  ui4_len,
                            UINT16*                 pui2_data_out)
{
    UINT8           ui1_tmp;
    UINT16*         pui2_buf;
    UINT32          ui4_i;
    UINT32          ui4_offset = 0;
    UINT32          ui4_count = ui4_len;
#ifndef __KERNEL__
#ifdef MW_SM_DVBT_SUPPORT
    UINT16          ui2_count = (UINT16)ui4_len;
    E_DVB_TEXT_CHAR_TBL   e_dvb_encoding;
#endif
#endif

    if(pui1_src == NULL || pui2_data_out == NULL || e_encoding >= MM_SBTL_ENCODE_END)
    {
        return MMSBTLR_INV_ARG;
    }

    pui2_buf = (UINT16*)pui1_src;
    if(e_encoding == MM_SBTL_ENCODE_UTF16)
    {
        ui4_count = ui4_count >> 1;
    }
    
    switch(e_encoding)
    {
        case MM_SBTL_ENCODE_ASCI:
            for(ui4_i = 0; ui4_i < ui4_len;ui4_i ++)
            {
                ui1_tmp = pui1_src[ui4_i];
                pui2_data_out[ui4_i] = sbtl_asci2cp1252(ui1_tmp);
            }
            
            break;

         case MM_SBTL_ENCODE_UTF8:
            x_uc_ps_to_w2s((CHAR*)pui1_src,(UTF16_T*)pui2_data_out,ui4_len);
            
            break;

        case MM_SBTL_ENCODE_UTF16:
            for(ui4_i = 0; ui4_i < ui4_count;ui4_i ++)
            {
                *pui2_data_out = *pui2_buf;
                pui2_buf ++;
                pui2_data_out ++;
            }            
            break;

        case MM_SBTL_ENCODE_BIG5:
            sbtl_multi_byte_to_unicode(pui2_buf, ui4_len, &ui4_offset, MUL_BYTE_BIG5, pui2_data_out, &ui4_count);
            break;

        case MM_SBTL_ENCODE_GB2312:
            sbtl_multi_byte_to_unicode(pui2_buf, ui4_len, &ui4_offset, MUL_BYTE_GB2312, pui2_data_out, &ui4_count);
            break;
#ifndef __KERNEL__
#ifdef MW_SM_DVBT_SUPPORT
        case MM_SBTL_ENCODE_ISO8859_2:
        case MM_SBTL_ENCODE_ISO8859_5:
        case MM_SBTL_ENCODE_ISO8859_7:
        case MM_SBTL_ENCODE_ISO8859_8:
        case MM_SBTL_ENCODE_ISO8859_9:
        case MM_SBTL_ENCODE_ISO8859_16:
            sbtl_convert_encoding(e_encoding, &e_dvb_encoding);
            dvb_si_txt_cnv_8859_ps_to_w2s(e_dvb_encoding,
                                            (CHAR*)pui1_src,
                                            ui4_len,
                                            pui2_data_out,
                                            &ui2_count);
            break;
#endif
#endif
        default:
            break;
    }

    return MMSBTLR_OK;
}

INT32 sbtl_record_init(VOID)
{
    _gpt_sbtl_rcd = (MM_SBTL_RECORD_INF*)x_mem_alloc(sizeof(MM_SBTL_RECORD_INF));
    if(_gpt_sbtl_rcd == NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    else
    {
        x_memset(_gpt_sbtl_rcd, 0, sizeof(MM_SBTL_RECORD_INF));
    }

    _gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_VIDEO] = MM_SBTL_INF_MSK_NONE;
    _gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_AUDIO] = MM_SBTL_INF_MSK_NONE;
    return MMSBTLR_OK;
}

INT32 sbtl_set_encode_record(
    SBTL_ENGINE_T*                   pt_engine,
    MM_SBTL_ENCODING_T               e_encoding)
{
    if(pt_engine == NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    
    if((pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_LRC) ||
       (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_KRK_KING) ||
       (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_MP3_KRK))
    {
        _gpt_sbtl_rcd->e_coding[MM_SBTL_INF_RCD_AUDIO] = e_encoding;
        _gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_AUDIO] = MM_SBTL_INF_MSK_ENCODING;
    }
    else
    {
        _gpt_sbtl_rcd->e_coding[MM_SBTL_INF_RCD_VIDEO] = e_encoding;
        _gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_VIDEO] = MM_SBTL_INF_MSK_ENCODING;
    }

    return MMSBTLR_OK;
}


INT32 sbtl_set_font_inf_record(
    SBTL_ENGINE_T*                   pt_engine,
    WGL_FONT_INFO_T*                 pt_font_inf)
{
    if(pt_engine == NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    
    if((pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_LRC) ||
       (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_KRK_KING) ||
       (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_MP3_KRK))
    {
        x_memcpy(&_gpt_sbtl_rcd->at_font_info[MM_SBTL_INF_RCD_AUDIO],
                 pt_font_inf,
                 sizeof(WGL_FONT_INFO_T));
        _gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_AUDIO] = MM_SBTL_INF_MSK_FONT_INF;
    }
    else
    {
        x_memcpy(&_gpt_sbtl_rcd->at_font_info[MM_SBTL_INF_RCD_VIDEO],
                 pt_font_inf,
                 sizeof(WGL_FONT_INFO_T));
        _gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_VIDEO] = MM_SBTL_INF_MSK_FONT_INF;
    }

    return MMSBTLR_OK;
}

INT32 sbtl_check_record_inf(
    SBTL_ENGINE_T*                   pt_engine)
{
    
    if(pt_engine == NULL)
    {
        return MMSBTLR_INV_ARG;
    }

    if((pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_LRC) ||
       (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_KRK_KING) ||
       (pt_engine->t_cfg.e_sbtl_type == MM_SBTL_TYPE_LYRIC_MP3_KRK))
    {
        if(_gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_AUDIO] & MM_SBTL_INF_MSK_FONT_INF)
        {
            x_memcpy(&pt_engine->t_cfg.t_font_info,
                     &_gpt_sbtl_rcd->at_font_info[MM_SBTL_INF_RCD_AUDIO],
                     sizeof(WGL_FONT_INFO_T));
        }

        if(_gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_AUDIO] & MM_SBTL_INF_MSK_ENCODING)
        {
            pt_engine->t_cfg.e_encoding = _gpt_sbtl_rcd->e_coding[MM_SBTL_INF_RCD_AUDIO];
        }        
    }
    else
    {
        if(_gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_VIDEO] & MM_SBTL_INF_MSK_FONT_INF)
        {
            x_memcpy(&pt_engine->t_cfg.t_font_info,
                     &_gpt_sbtl_rcd->at_font_info[MM_SBTL_INF_RCD_VIDEO],
                     sizeof(WGL_FONT_INFO_T));
        }

        if(_gpt_sbtl_rcd->aui1_rcd_mask[MM_SBTL_INF_RCD_VIDEO] & MM_SBTL_INF_MSK_ENCODING)
        {
            pt_engine->t_cfg.e_encoding = _gpt_sbtl_rcd->e_coding[MM_SBTL_INF_RCD_VIDEO];
        }        
    }
    
    return MMSBTLR_OK;
}

INT32 sbtl_check_encoding_info(
    SBTL_ENGINE_T*                   pt_engine,
    UINT8                            ui1_data_1,
    UINT8                            ui1_data_2,
    BOOL*                            pb_UTF16)
{
    BOOL                             b_UTF16 = FALSE;
    SBTL_CFG_T*                      pt_cfg;
    
    if(pt_engine == NULL)
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_cfg = &pt_engine->t_cfg;
    if(pt_cfg->e_encoding == MM_SBTL_ENCODE_AUTO)
    {
        if ((ui1_data_1 == 0xFF) && ui1_data_2 == 0xFE)
        {
            b_UTF16 = TRUE;
            pt_cfg->e_encoding = MM_SBTL_ENCODE_UTF16;
        }
        else if(pt_cfg->t_font_info.e_font_cmap == FE_CMAP_ENC_BIG5) 
        {
            pt_cfg->e_encoding = MM_SBTL_ENCODE_BIG5;
            pt_cfg->t_font_info.e_font_cmap = FE_CMAP_ENC_UNICODE;
        }
        else if(pt_cfg->t_font_info.e_font_cmap == FE_CMAP_ENC_GB2312) 
        {
            pt_cfg->e_encoding = MM_SBTL_ENCODE_GB2312;
            pt_cfg->t_font_info.e_font_cmap = FE_CMAP_ENC_UNICODE;
        }
        else
        {
            pt_cfg->e_encoding = MM_SBTL_ENCODE_ASCI;
        }
    }

    if(pb_UTF16 != NULL)
    {
        *pb_UTF16 = b_UTF16;
    }
    return MMSBTLR_OK;
}

BOOL sbtl_cmp_string(
    UINT16*             pui2_buf,
    UINT16              ui2_len,
    CHAR*               s_string)
{
    UINT16              ui2_str_len;
    UINT16              ui2_i;

    if(pui2_buf == NULL || ui2_len == 0)
    {
        return FALSE;
    }
    
    ui2_str_len = (UINT16)x_strlen((const CHAR *)s_string);
    if(ui2_str_len != ui2_len)
    {
        return FALSE;
    }

    for(ui2_i = 0; ui2_i < ui2_len; ui2_i++)
    {
        if(pui2_buf[ui2_i] != (UINT16)s_string[ui2_i])
        {
            return FALSE;
        }
    }

    return TRUE;
}

INT32 sbtl_util_fcopybytes_fct(VOID*       pv_obj, 
                                       VOID*       pv_buf, 
                                       SIZE_T      z_buf_leng, 
                                       SIZE_T      z_copy_size,
                                       UINT32*     pui4_size)
{
#ifndef __KERNEL__
    UINT32              ui4_read;
    INT32               i4_ret;
    BOOL                b_buf_not_enough;
    SIZE_T              z_copy;
    MEDIA_FEEDER_T*     pt_obj;

    ASSERT(pv_obj, ("_sbtl_util_fcopybytes_fct pv_obj=NULL\n"));
    ASSERT(pv_buf, ("_sbtl_util_fcopybytes_fct pv_buf=NULL\n"));
    ASSERT(pui4_size, ("_sbtl_util_fcopybytes_fct pui4_size=NULL\n"));
    
    if ((NULL==pv_obj) || (NULL==pv_buf) || (NULL==pui4_size))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
        b_buf_not_enough = TRUE;
    }
    else
    {
        z_copy = z_copy_size;
        b_buf_not_enough = FALSE;
    }
#ifndef ANDROID
    i4_ret = x_fm_read(pt_obj->h_feeder, pv_buf, z_copy, &ui4_read);
    *pui4_size = ui4_read;
    if (i4_ret == FMR_OK)
    {
        if(ui4_read < z_copy)
        {
            return MMSBTLR_INSUFFICIENT_SIZE;
        }
        else
        {
            if(b_buf_not_enough)
            {
                return MMSBTLR_INSUFFICIENT_MEMORY;
            }
            else
            {
                return MMSBTLR_OK;
            }
        }
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
    i4_ret = fread(pv_buf, 1, z_copy, (FILE*)pt_obj->h_feeder);
    if (i4_ret < 0)
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
    else if(i4_ret > 0)
    {
        ui4_read = i4_ret;
        if(ui4_read < z_copy)
        {
            return MMSBTLR_INSUFFICIENT_SIZE;
        }
        return MMSBTLR_OK;
    }
    else
    {
        ui4_read = 0;
        return IMTK_PB_CB_ERROR_CODE_EOF;
    }
    
#endif
#else
    return MMSBTLR_OK;
#endif
}


INT32 sbtl_util_finput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
#ifndef __KERNEL__


    UINT8               ui1_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    
    ASSERT(pv_obj, ("_sbtl_util_finput1bytes_fct pv_obj=NULL\n"));
    ASSERT(pui1_data, ("_sbtl_util_finput1bytes_fct pui1_data=NULL\n"));

    if ((NULL==pv_obj) || (NULL==pui1_data))
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
#ifndef ANDROID
    i4_ret = x_fm_read(pt_obj->h_feeder, (VOID*)&ui1_data, sizeof(ui1_data), &ui4_read);
    *pui1_data = ui1_data;
    if (i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
    i4_ret = fread((VOID*)&ui1_data, 1, sizeof(ui1_data), (FILE*)pt_obj->h_feeder);
    if (i4_ret < 0)
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
    else if(i4_ret > 0)
    {
        ui4_read = i4_ret;
        if(ui4_read < sizeof(ui1_data))
        {
            return MMSBTLR_INSUFFICIENT_SIZE;
        }
        return MMSBTLR_OK;
    }
    else
    {
        ui4_read = 0;
        return IMTK_PB_CB_ERROR_CODE_EOF;
    }
    
#endif
#else
        return MMSBTLR_OK;
#endif

}

INT32 sbtl_util_fsetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#ifndef __KERNEL__
    INT32               i4_ret;
    UINT64              ui8_cur_pos;
    MEDIA_FEEDER_T*     pt_obj;

    ASSERT(pv_obj, ("_sbtl_util_fsetpos_fct pv_obj=NULL\n"));

    if (NULL==pv_obj)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
#ifndef ANDROID
    i4_ret = x_fm_lseek(pt_obj->h_feeder, i8_offset, ui1_whence, &ui8_cur_pos);
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
    switch (ui1_whence)
    {
    case MEDIA_SEEK_BGN:
        ui1_whence = SEEK_SET;
        break;
    case MEDIA_SEEK_CUR:
        ui1_whence = SEEK_CUR;
        break;
    case MEDIA_SEEK_END:
        ui1_whence = SEEK_END;
        break;
    default:
        return MMSBTLR_INTERNAL_ERROR;
    }
    i4_ret = fseek((FILE*)pt_obj->h_feeder, i8_offset, ui1_whence);
    ui8_cur_pos = ftell((FILE*)pt_obj->h_feeder);
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    /*else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }*/
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#endif

#else
        return MMSBTLR_OK;
#endif

}

INT32 sbtl_util_fgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
#ifndef __KERNEL__
    UINT64              ui8_cur_pos = 0;
    INT32               i4_ret      = 0;
    MEDIA_FEEDER_T*     pt_obj;

    ASSERT(pv_obj, ("_sbtl_util_fgetpos_fct pv_obj=NULL\n"));
    ASSERT(pui8_cur_pos, ("_sbtl_util_fgetpos_fct pui8_cur_pos=NULL\n"));

    if ((NULL==pv_obj) || (NULL==pui8_cur_pos))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
#ifndef ANDROID
    i4_ret = x_fm_lseek(pt_obj->h_feeder, 0, FM_SEEK_CUR, &ui8_cur_pos);
#else
    ui8_cur_pos = ftell((FILE*)pt_obj->h_feeder);
#endif
    *pui8_cur_pos = ui8_cur_pos;
    
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
    return MMSBTLR_OK;
#endif
}


#ifdef INET_SUPPORT
INT32 sbtl_util_http_fcopybytes_fct(VOID*       pv_obj, 
                                    VOID*       pv_buf, 
                                    SIZE_T      z_buf_leng, 
                                    SIZE_T      z_copy_size,
                                    UINT32*     pui4_size)
{
#ifndef __KERNEL__


    UINT32              ui4_read;
    INT32               i4_ret;
    BOOL                b_buf_not_enough;
    SIZE_T              z_copy;
    MEDIA_FEEDER_T*     pt_obj;

    ASSERT(pv_obj, ("_sbtl_util_fcopybytes_fct pv_obj=NULL\n"));
    ASSERT(pv_buf, ("_sbtl_util_fcopybytes_fct pv_buf=NULL\n"));
    ASSERT(pui4_size, ("_sbtl_util_fcopybytes_fct pui4_size=NULL\n"));
    
    if ((NULL==pv_obj) || (NULL==pv_buf) || (NULL==pui4_size))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    if (z_buf_leng < z_copy_size)
    {
        z_copy = z_buf_leng;
        b_buf_not_enough = TRUE;
    }
    else
    {
        z_copy = z_copy_size;
        b_buf_not_enough = FALSE;
    }
    
    i4_ret = x_httpc_fm_read(pt_obj->h_feeder, pv_buf, z_copy, &ui4_read);
    *pui4_size = ui4_read;
    if (i4_ret == FMR_OK)
    {
        if(ui4_read < z_copy)
        {
            return MMSBTLR_INSUFFICIENT_SIZE;
        }
        else
        {
            if(b_buf_not_enough)
            {
                return MMSBTLR_INSUFFICIENT_MEMORY;
            }
            else
            {
                return MMSBTLR_OK;
            }
        }
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
    return MMSBTLR_OK;
#endif
}


INT32 sbtl_util_http_finput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
#ifndef __KERNEL__


    UINT8               ui1_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    
    ASSERT(pv_obj, ("_sbtl_util_finput1bytes_fct pv_obj=NULL\n"));
    ASSERT(pui1_data, ("_sbtl_util_finput1bytes_fct pui1_data=NULL\n"));

    if ((NULL==pv_obj) || (NULL==pui1_data))
    {
        return MMSBTLR_INV_ARG;
    }
    
    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    i4_ret = x_httpc_fm_read(pt_obj->h_feeder, (VOID*)&ui1_data, sizeof(ui1_data), &ui4_read);
    *pui1_data = ui1_data;
    if (i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
        return MMSBTLR_OK;
#endif

}

INT32 sbtl_util_http_fsetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
#ifndef __KERNEL__


    INT32               i4_ret;
    UINT64              ui8_cur_pos;
    MEDIA_FEEDER_T*     pt_obj;

    ASSERT(pv_obj, ("_sbtl_util_fsetpos_fct pv_obj=NULL\n"));

    if (NULL==pv_obj)
    {
        return MMSBTLR_INV_ARG;
    }

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    i4_ret = x_httpc_fm_lseek(pt_obj->h_feeder, i8_offset, ui1_whence, &ui8_cur_pos);
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
        return MMSBTLR_OK;
#endif

}

INT32 sbtl_util_http_fgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
#ifndef __KERNEL__


    UINT64              ui8_cur_pos;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    ASSERT(pv_obj, ("_sbtl_util_fgetpos_fct pv_obj=NULL\n"));
    ASSERT(pui8_cur_pos, ("_sbtl_util_fgetpos_fct pui8_cur_pos=NULL\n"));

    if ((NULL==pv_obj) || (NULL==pui8_cur_pos))
    {
        return MMSBTLR_INV_ARG;
    }

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    i4_ret = x_httpc_fm_lseek(pt_obj->h_feeder, 0, FM_SEEK_CUR, &ui8_cur_pos);
    *pui8_cur_pos = ui8_cur_pos;
    
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_INSUFFICIENT_SIZE;
    }
    else
    {
        return MMSBTLR_INTERNAL_ERROR;
    }
#else
    return MMSBTLR_OK;
#endif
}
#endif

INT32 x_sbtl_util_fcopybytes_fct(VOID*       pv_obj, 
                                    VOID*       pv_buf, 
                                    SIZE_T      z_buf_leng, 
                                    SIZE_T      z_copy_size,
                                    UINT32*     pui4_size)
{
    return sbtl_util_fcopybytes_fct(pv_obj,pv_buf,z_buf_leng,z_copy_size,pui4_size);
}


INT32 x_sbtl_util_finput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
    return sbtl_util_finput1bytes_fct(pv_obj, pui1_data);
}

INT32 x_sbtl_util_fsetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
    return sbtl_util_fsetpos_fct(pv_obj, i8_offset, ui1_whence);
}

INT32 x_sbtl_util_fgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
    return sbtl_util_fgetpos_fct(pv_obj, pui8_cur_pos);
}
