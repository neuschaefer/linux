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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mtal_sbtl_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mtal_sbtl_if.c
 *  MTAL external subtitle interface.
 */

#include <linux/string.h>
#include "mtal_sbtl_if.h"


static VOID*                pv_engine = NULL;
static HANDLE_T             h_subtitle_file = NULL_HANDLE;
static MEDIA_FEEDER_T       t_feeder;

static UINT32 _u4ExtSbtlFileSize = 0;
static UINT8 *_pu1SbtlTextBuf = NULL;
static UINT32 _u4SbtlTextPtr = 0;

static EXT_SBTL_SETTING_T _rExtSbtlSetting;


static INT32 _sbtl_util_finput1bytes_fct(VOID* pv_obj, UINT8* pui1_data)
{
    UINT8               ui1_data;
    UINT32              ui4_read;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    //i4_ret = x_fm_read(pt_obj->h_feeder, (VOID*)&ui1_data, sizeof(ui1_data), &ui4_read);
    i4_ret = FMR_OK;
    ui1_data = *(_pu1SbtlTextBuf + _u4SbtlTextPtr++);
    *pui1_data = ui1_data;
    if (i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_EOT;
    }
    
    return MMSBTLR_CORE;
}

static INT32 _sbtl_util_fsetpos_fct(VOID* pv_obj, INT64 i8_offset, UINT8 ui1_whence)
{
    INT32               i4_ret;
    UINT64              ui8_cur_pos;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    //i4_ret = x_fm_lseek(pt_obj->h_feeder, i8_offset, ui1_whence, &ui8_cur_pos);
    i4_ret = FMR_OK;
    switch (ui1_whence)
    {
    case MEDIA_SEEK_BGN:
        _u4SbtlTextPtr = i8_offset;
        break;
    case MEDIA_SEEK_CUR:
        _u4SbtlTextPtr += i8_offset;
        break;
    case MEDIA_SEEK_END:
        _u4SbtlTextPtr = _u4ExtSbtlFileSize - 1;
        break;
    }
    
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_EOT;
    }
    
    return MMSBTLR_CORE;
}

static INT32 _sbtl_util_fgetpos_fct(VOID* pv_obj, UINT64* pui8_cur_pos)
{
    UINT64              ui8_cur_pos;
    INT32               i4_ret;
    MEDIA_FEEDER_T*     pt_obj;

    pt_obj = (MEDIA_FEEDER_T*)pv_obj;
    
    //i4_ret = x_fm_lseek(pt_obj->h_feeder, 0, FM_SEEK_CUR, &ui8_cur_pos);
    i4_ret = FMR_OK;
    ui8_cur_pos = _u4SbtlTextPtr;
    *pui8_cur_pos = ui8_cur_pos;
    
    if(i4_ret == FMR_OK)
    {
        return MMSBTLR_OK;
    }
    else if (i4_ret == FMR_EOF)
    {
        return MMSBTLR_EOT;
    }
    
    return MMSBTLR_CORE;
}

static INT32 _sbtl_util_fcopybytes_fct(VOID*       pv_obj, 
                                       VOID*       pv_buf, 
                                       SIZE_T      z_buf_leng, 
                                       SIZE_T      z_copy_size,
                                       UINT32*     pui4_size)
{
    UINT32              ui4_read;
    INT32               i4_ret;
    BOOL                b_buf_not_enough;
    SIZE_T              z_copy;
    MEDIA_FEEDER_T*     pt_obj;

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
    
    //i4_ret = x_fm_read(pt_obj->h_feeder, pv_buf, z_copy, &ui4_read);
    i4_ret = FMR_OK;
    x_memcpy(pv_buf, _pu1SbtlTextBuf + _u4SbtlTextPtr, z_copy);
    ui4_read = z_copy;
    *pui4_size = ui4_read;
    if (i4_ret == FMR_OK)
    {
        if(ui4_read < z_copy)
        {
            return MMSBTLR_EOT;
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
        return -1;//INPUTR_FAIL;
    }
}

VOID _mm_sbtl_state_notify(VOID*        pv_tag,
                           SBTL_COND_T  e_cond,
                           UINT32       e_event,
                           UINT32       ui4_data)
{
    //x_dbg_stmt("_mm_sbtl_state_notify cond:%d event:%d\n", e_cond, e_event);
}


BOOL fgMtalSbtlTextOpen(void)
{
    SBTL_CFG_T              t_cfg;
    INT32                   i4_ret;
    MM_SBTL_TYPE_T          eSbtlType = _rExtSbtlSetting.eExtSbtlType;
    UINT32                  u4Size = _rExtSbtlSetting.u4ExtSbtlSize;
    SAMI_STREAM_INFO_T      t_stream_info;
    //UINT32                  ui4_idx;

#if 0  // mark off WGL related stuff
    /*Open the screen */
    if (NULL_HANDLE == h_screen)
    {
        i4_ret = x_gl_screen_open (SN_PRES_MAIN_DISPLAY,
                                   NULL,
                                   NULL,
                                   & h_screen);
        if (GL_FAILED(i4_ret))
        {
            //x_dbg_stmt("Can't open screen\n");
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_plane)
    {
        /*Open the plane */
        i4_ret = x_gl_plane_open (h_screen,
                                  1,
                                  &h_plane);
        if (GL_FAILED(i4_ret))
        {
            //x_dbg_stmt("Can't open plane\n");
            x_handle_free(h_screen);
            return CLIR_OK;
        }
        
        /*Link the plane*/
        i4_ret = x_gl_plane_link (h_plane, h_screen, TRUE);
        if (GL_FAILED(i4_ret))
        {
            //x_dbg_stmt("Can't link plane\n");
            x_handle_free(h_screen);
            x_handle_free(h_plane);
            return CLIR_OK;
        }
    }

    if (NULL_HANDLE == h_canvas)
    {
        x_wgl_get_canvas(h_plane, &h_canvas);
    }
#endif

    _u4ExtSbtlFileSize = u4Size;

    t_cfg.e_sbtl_type = eSbtlType;      
    t_cfg.i4_speed = 100;
#if 0  // mark off WGL related stuff
    t_cfg.h_gl_plane = h_canvas;
#endif
    t_cfg.ui4_stream_id = 0;

    if (NULL_HANDLE == h_subtitle_file)
    {
        /*
        x_fm_open(FM_ROOT_HANDLE, 
                  SRT_TEST_FILE,
                  FM_READ_ONLY, 
                  0666, 
                  FALSE, 
                  &h_subtitle_file);
        */
        
        t_feeder.h_feeder = h_subtitle_file;
        t_feeder.t_feeder_be_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_be_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_be_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;

        t_feeder.t_feeder_le_fct_tbl.pf_copybytes = _sbtl_util_fcopybytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_input4 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input3 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input2 = NULL;
        t_feeder.t_feeder_le_fct_tbl.pf_input1 = _sbtl_util_finput1bytes_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_set_pos = _sbtl_util_fsetpos_fct;
        t_feeder.t_feeder_le_fct_tbl.pf_get_pos = _sbtl_util_fgetpos_fct;
    }
    t_cfg.pt_feeder[MM_SBTL_FEEDER_SUBTITLE] = &t_feeder;

#if 0  // mark off WGL related stuff
    x_memset(&t_cfg.t_font_info, 0, sizeof(WGL_FONT_INFO_T));
    x_strcpy(t_cfg.t_font_info.a_c_font_name, SN_FONT_DEFAULT);
    t_cfg.t_font_info.e_font_size  = FE_FNT_SIZE_MEDIUM;
    t_cfg.t_font_info.e_font_style = FE_FNT_STYLE_REGULAR;
    t_cfg.t_font_info.e_font_cmap  = FE_CMAP_ENC_UNICODE;

    t_cfg.t_bg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_bg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_white, 
              sizeof(GL_COLOR_T));

    t_cfg.t_fg_color_info.e_type = WGL_COLOR_SET_STANDARD;
    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_enable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_disable, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    x_memcpy ((VOID *)&t_cfg.t_fg_color_info.u_color_data.t_standard.t_highlight, 
              &t_color_black, 
              sizeof(GL_COLOR_T));

    t_cfg.ui1_text_alignment = 0;//WGL_AS_CENTER_BOTTOM;
#endif

    x_sbtl_engine_open(&t_cfg,
                     TRUE,
                     _mm_sbtl_state_notify,
                     NULL,
                     &pv_engine);

    x_sbtl_engine_get(pv_engine,
                      EVN_MM_SBTL_GET_STREAM_INFO,
                      &t_stream_info,
                      sizeof(SAMI_STREAM_INFO_T));
    _rExtSbtlSetting.u2TrackNs = t_stream_info.ui1_total_stream_count;
    //printk("SMI total track: %d, current track: %d\n", t_stream_info.ui1_total_stream_count, t_stream_info.ui1_stream_idx);

#if 0
    ui4_idx = 1;
    x_sbtl_engine_set(pv_engine,
                      EVN_MM_SBTL_SET_STREAM_INDEX,
                      &ui4_idx,
                      4);
#endif

    _rExtSbtlSetting.fgActive = TRUE;
    
    return TRUE;
}

BOOL fgMtalSbtlTextClose(void)
{
    if (pv_engine == NULL)
        return FALSE;
        
    x_sbtl_engine_close(pv_engine);
    pv_engine = NULL;
    
    _rExtSbtlSetting.fgActive = FALSE;
    _rExtSbtlSetting.fgToChgTrack = FALSE;
    
    return TRUE;
}

BOOL fgMtalSbtlTextStart(void)
{
    if (pv_engine == NULL)
        return FALSE;
        
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_APP_PLAY_REQ,
                    NULL,
                    0);

    _rExtSbtlSetting.fgActive = TRUE;
                    
    return TRUE;
}

BOOL fgMtalSbtlTextStop(void)
{
    if (pv_engine == NULL)
        return FALSE;
        
    x_sbtl_engine_set(pv_engine,
                    EVN_MM_SBTL_APP_STOP_REQ,
                    NULL,
                    0);

    _rExtSbtlSetting.fgActive = FALSE;
    
    return TRUE;
}

BOOL fgMtalSbtlTextChgTrack(void)
{
    UINT32 ui4_idx;
    
    if (pv_engine == NULL)
        return FALSE;
        
    ui4_idx = _rExtSbtlSetting.u2TrackNo;
    x_sbtl_engine_set(pv_engine,
                      EVN_MM_SBTL_SET_STREAM_INDEX,
                      &ui4_idx,
                      4);

    _rExtSbtlSetting.fgToChgTrack = FALSE;
    
    return TRUE;
}


void vMtalSbtlTextBufInit(UINT8 *pu1SubtitleBuffer)
{
    _pu1SbtlTextBuf = pu1SubtitleBuffer + EXT_SBTL_STR_BUF_SIZE;
}

void vCopyExtSbtlStr(UINT8 *pu1Str)
{
    strcpy(_pu1SbtlTextBuf - EXT_SBTL_STR_BUF_SIZE + 2, pu1Str);
    *(UINT16 *)(_pu1SbtlTextBuf - EXT_SBTL_STR_BUF_SIZE) = strlen(pu1Str);
}

void vMtalSbtlTextAssign(BOOL fgExist, MM_SBTL_TYPE_T eType, UINT32 u4Size)
{
    _rExtSbtlSetting.fgExist = fgExist;
    _rExtSbtlSetting.eExtSbtlType = eType;
    _rExtSbtlSetting.u4ExtSbtlSize = u4Size;
    _rExtSbtlSetting.fgEnable = FALSE;
    _rExtSbtlSetting.fgToChgTrack = FALSE;
    
    if (fgExist)
        _rExtSbtlSetting.u2TrackNs = 1;  // ToDo: multi track SMI
}

void vMtalSbtlTextSet(UINT32 u4Mask, BOOL fgEnable, UINT16 u2TrackNo, INT8 i1Sync)
{
    if (!_rExtSbtlSetting.fgExist)
        return;

    if (u4Mask & MST_MASK_ENABLE)
        _rExtSbtlSetting.fgEnable = fgEnable;
    if ((u4Mask & MST_MASK_TRK_NO) && u2TrackNo != _rExtSbtlSetting.u2TrackNo)
    {
        _rExtSbtlSetting.u2TrackNo = u2TrackNo;
        _rExtSbtlSetting.fgToChgTrack = TRUE;
    }
    if (u4Mask & MST_MASK_SYNC)
        _rExtSbtlSetting.i1Sync = i1Sync;
}

BOOL fgIsMtalSbtlExisted(void)
{
    return _rExtSbtlSetting.fgExist;
}

BOOL fgIsMtalSbtlEnabled(void)
{
    return _rExtSbtlSetting.fgEnable;
}

UINT16 u2MtalSbtlGetTrackNs(void)
{
    if (_rExtSbtlSetting.fgExist)
        return _rExtSbtlSetting.u2TrackNs;
    else
        return 0;
}

void vMtalSbtlTextCheck(void)
{
    if (!_rExtSbtlSetting.fgExist)
        return;

    if (_rExtSbtlSetting.fgEnable && !_rExtSbtlSetting.fgActive)
    {
        //fgMtalSbtlTextStart();
        fgMtalSbtlTextOpen();
        return;
    }

    if (!_rExtSbtlSetting.fgEnable && _rExtSbtlSetting.fgActive)
    {
        fgMtalSbtlTextStop();
        fgMtalSbtlTextClose();
        return;
    }
    
    if (_rExtSbtlSetting.fgToChgTrack)
    {
        fgMtalSbtlTextChgTrack();
    }
}

INT32 i4MtalSbtlGetSync(void)
{
    INT32 i4Sync = _rExtSbtlSetting.i1Sync;  // in step

    i4Sync *= 45000;  // 1 step = 0.5 sec = 45000 STC unit
    return i4Sync;
}
