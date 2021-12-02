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
 * $RCSfile: dmx_mm_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm_mw.c
 *  Demux driver - middleware interface of Multi-Media
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx_mw.h"
#include "bagt_mw.h"
#include "dmx_if.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#endif

#ifdef ENABLE_MULTIMEDIA
#include "swdmx_if.h"
#endif
#include "x_lint.h"
#include "vdec_if.h"
#include "vdp_if.h"
#if defined(CC_MT5387) || defined(CC_MT5363)
#include "b2r_if.h"
#endif
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "srm_drvif.h"
#include "aud_if.h"
LINT_EXT_HEADER_BEGIN

#include "x_common.h"
#include "x_mm_common.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_demux.h"
#include "x_assert.h"
#include "x_aud_dec.h"
#include "dmx_mm_if.h"
#define __DMX_MW_API__  // avoid duplicate define for PSI MW and Driver
#include "x_printf.h"
#include "drv_comp_id.h"
#include "drv_name.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_param.h"
#endif

#define DEFINE_IS_LOG    SWDMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END

EXTERN BOOL _MW_DMX_MM_PES_Init(void);
EXTERN BOOL _MW_DMX_FlushAllCachePid(void);
EXTERN void _MW_DMX_SetCacheOn(BOOL fgCacheOn);

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_STREAM_TYPE 3

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/* Notify conditions */
typedef enum
{
    DMX_COND_ERROR = -1,
    DMX_COND_CTRL_DONE
}   DMX_DEC_COND_T;

typedef VOID (*x_dmx_nfy_fct) (
    VOID*               pv_nfy_tag,
    DMX_DEC_COND_T      e_nfy_cond,
    UINT32              ui4_data_1,
    UINT32              ui4_data_2
    );

typedef struct _DMX_NFY_INFO_T
{
    VOID*              pv_tag;
    x_dmx_nfy_fct      pf_vid_dec_nfy;
    VOID*              pv_previous_tag;          /* OUTPUT */
    x_dmx_nfy_fct      pf_previous_vid_dec_nfy;  /* OUTPUT */
}   DMX_NFY_INFO_T;

#ifdef TIME_SHIFT_SUPPORT
typedef struct
{
    BOOL fgValid;
    UINT16 u2StreamID;
    UINT8 u1KeyIdx;
    DMX_DESC_MODE_T eDescMode;
} DMX_MM_KEY_INFO_T;
#endif    // TIME_SHIFT_SUPPORT

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _bPreviousStateIsPause = FALSE;
static BOOL _bIsThumbnailPb=FALSE;
static UINT32 _u4PreviousSpeed = (UINT32)0xFFFFFFFF;
static BOOL _fgPreviousStateIsTrick = FALSE;
static BOOL _fgPreviousStateIsSeek = FALSE;
static UINT32 _ui4StcValue = 0;
static BOOL _bFirstRange = TRUE;

// Can't be an array
// Because the index will be hard to control
static DMX_MM_DEC_INFO_T _rDecInfo = {0};
static DMX_MM_DEC_INFO_T _rDecInfo1 = {0};
static DMX_MM_DEC_INFO_T _rDecInfo2 = {0};
static MEDIA_TYPE_T _rContainerType;
static MEDIA_CONTNR_SUBTYPE_T _rContainerSubType;
static MEDIA_TS_SUBTYPE_T _rTsSubType;
static MEDIA_AUD_SUBTYPE_T _rAudSubType;
static BOOL _fgKeyIsExist;
static BOOL _fgVideoExist = 0;
static UINT16 _u2VideoCompId = 0;
//static DMX_NFY_INFO_T _arDmxNfyInfo = {0};
static HANDLE_T _hDmxCtrlSema = 0;
static HANDLE_T _hFlushMutex = 0;
static BOOL _fgAudCtrlDone = TRUE;
static BOOL _fgAudNotSupport = FALSE;
static UINT32 _u4AudCtrl = 0;

static x_mm_range_nfy_fct _pfnSwdmxRangeCb = NULL;
static VOID* _pRangeCbTag = NULL;
static UINT32 _u4RangeID = 0;

#ifdef CC_FLASH_TEST
static BOOL _fgEsPtsConnect = FALSE;
#endif

#ifdef TIME_SHIFT_SUPPORT

/// Notification callback of scramble state
static DEMUX_NFY_INFO_T _arScrambleState[DMX_NUM_PID_INDEX];

/// State word for locking interrupt
static CRIT_STATE_T _rDmxLock;

/// Is interrupt locking? For making sure Lock()/Unlock() are not nested
static BOOL _fgDmxLocking = FALSE;

static DMX_MM_KEY_INFO_T _arDmxMMKeyInfo[DMX_NUM_PID_INDEX];
#endif  // TIME_SHIFT_SUPPORT

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static VOID _MwDmxFlushCtrl(void);
static VOID _MwDmxWaitCtrlDone(void);

#ifdef LINUX_TURNKEY_SOLUTION
static MWDMX_NFY_PARAM mwdmx_param;
static x_mm_range_nfy_fct mwdmx_nfy;
static BOOL _MwDmx_nfy_fct(VOID*                  pv_nfy_tag,
                         MM_RANGE_COND_T          e_nfy_cond,
                         UINT32                 ui4_data_1,
                         UINT32                 ui4_data_2);
#endif

VOID MW_DMX_NotifyStepDone(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret)
{
    BOOL fgRet;
    UNUSED(u4VdpId);
    UNUSED(u4PTS);
    UNUSED(u4Ret);
    LOG(5, "!!!! MW_DMX_NotifyStepDone\n");

    STC_StopStc();

    // actually this should be done in every format
    // we have to evaluate the performance then open this for all format
    if((_rContainerType == MEDIA_TYPE_TS) &&
       (_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
    {
        // we have to stop the stream,
        // or we will notfiy MW other message(like no signal).
        _MwDmxFlushCtrl();
        fgRet = SWDMX_Pause();
        if(!fgRet)
        {
            LOG(1,"MW_DMX_NotifyStepDone SWDMX_Pause fail\n");
        }
        _MwDmxWaitCtrlDone();
    }

#ifndef LINUX_TURNKEY_SOLUTION
    if(_pfnSwdmxRangeCb)
#else
    if(mwdmx_nfy)
#endif
    {
        if(u4Ret == VDP_SEEK_STEP_OK)
        {
#ifndef LINUX_TURNKEY_SOLUTION
            _pfnSwdmxRangeCb(_pRangeCbTag,
                MM_RANGE_SEEK_DONE,
                _u4RangeID,
                0xFFFFFFFF);
#else
            _MwDmx_nfy_fct(_pRangeCbTag,
                MM_RANGE_SEEK_DONE,
                _u4RangeID,
                0xFFFFFFFF);
#endif
            LOG(3, "MW_DMX_NotifyStepDone MM_RANGE_SEEK_DONE\n");
        }
        else // if(u4Ret == VDP_SEEK_STEP_NO_DATA)
        {
#ifndef LINUX_TURNKEY_SOLUTION
            _pfnSwdmxRangeCb(_pRangeCbTag,
                MM_RANGE_SEEK_NO_DATA,
                _u4RangeID,
                0xFFFFFFFF);
#else
            _MwDmx_nfy_fct(_pRangeCbTag,
                MM_RANGE_SEEK_NO_DATA,
                _u4RangeID,
                0xFFFFFFFF);
#endif
            LOG(3, "MW_DMX_NotifyStepDone MM_RANGE_SEEK_NO_DATA\n");
        }
    }
    else
    {
        LOG(1, "MW_DMX_NotifyStepDone not sent info\n");
    }
}

static VOID _MW_DMX_NotifySeekDone(UINT32 u4VdpId, UINT32 u4PTS, UINT32 u4Ret)
{
    UNUSED(u4VdpId);
    UNUSED(u4PTS);
    UNUSED(u4Ret);
    LOG(5, "!!!! _MW_DMX_NotifySeekDone\n");
#ifndef LINUX_TURNKEY_SOLUTION
    if(_pfnSwdmxRangeCb)
#else
    if(mwdmx_nfy)
#endif
    {
        if(u4Ret == VDP_SEEK_STEP_OK)
        {
#ifndef LINUX_TURNKEY_SOLUTION
            _pfnSwdmxRangeCb(_pRangeCbTag,
                MM_RANGE_SEEK_DONE,
                _u4RangeID,
                0xFFFFFFFF);
#else
            _MwDmx_nfy_fct(_pRangeCbTag,
                MM_RANGE_SEEK_DONE,
                _u4RangeID,
                0xFFFFFFFF);
#endif
            LOG(3, "_MW_DMX_NotifySeekDone MM_RANGE_SEEK_DONE\n");
        }
        else // if(u4Ret == VDP_SEEK_STEP_NO_DATA)
        {
#ifndef LINUX_TURNKEY_SOLUTION
            _pfnSwdmxRangeCb(_pRangeCbTag,
                MM_RANGE_SEEK_NO_DATA,
                _u4RangeID,
                0xFFFFFFFF);
#else
            _MwDmx_nfy_fct(_pRangeCbTag,
                MM_RANGE_SEEK_NO_DATA,
                _u4RangeID,
                0xFFFFFFFF);
#endif
            LOG(3, "_MW_DMX_NotifySeekDone MM_RANGE_SEEK_NO_DATA\n");
        }
    }
    else
    {
        LOG(1, "_MW_DMX_NotifySeekDone not sent info\n");
    }
}

static VOID _MwDmxNtyFct(VOID* pv_nfy_tag, SWDMX_COND_T e_nfy_cond,
    UINT32 ui4_data_1, UINT32 ui4_data_2)
{
    if(e_nfy_cond == SWDMX_COND_CTRL_DONE)
    {
        VERIFY (x_sema_unlock(_hDmxCtrlSema) == OSR_OK);
    }
    else
    {
        LOG(3, "_MwDmxNtyFct unknow ctrl %d\n", (UINT32)e_nfy_cond);
    }
}

void _MwDmxMMLockFlushMutex(void)
{
    if(!_hFlushMutex)
    {
        return;
    }
    VERIFY(x_sema_lock(_hFlushMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
}


void _MwDmxMMUnlockFlushMutex(void)
{
    if(!_hFlushMutex)
    {
        return;
    }
    VERIFY(x_sema_unlock(_hFlushMutex) == OSR_OK);
}

static VOID _MwDmxFlushCtrl(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock(_hDmxCtrlSema, X_SEMA_OPTION_NOWAIT);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
}

static VOID _MwDmxWaitCtrlDone(void)
{
    INT32 i4Ret;
    i4Ret = x_sema_lock_timeout(_hDmxCtrlSema, 5000);
    if (i4Ret != OSR_OK)
    {
        LOG(0, "_MwDmxWaitCtrlDone timeout\n");
    }
}

static INT32 _MwDmxOnSetNfyFct(const SWDMX_NFY_INFO_T* ptdmxNfyInfo, SIZE_T z_set_info_len)
{
	INT32 i4Ret = RMR_OK;

    ASSERT(ptdmxNfyInfo != NULL);

    //_arDmxNfyInfo.pv_tag = ptdmxNfyInfo->pv_tag;
    //_arDmxNfyInfo.pf_vid_dec_nfy = ptdmxNfyInfo->pf_vid_dec_nfy;

	SWDMX_SetDmxNfy((SWDMX_NFY_INFO_T*)ptdmxNfyInfo);

	return i4Ret;
}

static INT32 _MwDmxOnMMSetStreamID(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    STREAM_ID_T* pStrmID = (STREAM_ID_T*)pv_set_info;
    BOOL fgRet;
    UINT32 u4StrmId;
    UINT32 u4SubStrmId;

    if(pv_set_info == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }
#ifdef LINUX_TURNKEY_SOLUTION
    else
    {
        pStrmID = x_mem_alloc(sizeof(STREAM_ID_T));
        if(pStrmID == NULL)
        {
            return RMR_NOT_ENOUGH_MEM;
        }
        x_memset(pStrmID,0,sizeof(STREAM_ID_T));
        rm_copy_from_user(pStrmID, pv_set_info, sizeof(STREAM_ID_T));
    }
#endif

    fgRet = TRUE;
    u4StrmId = 0;
    u4SubStrmId = 0;

    if (pStrmID->e_type == STREAM_ID_TYPE_AVI)
    {
        u4StrmId = pStrmID->u.t_stm_avi_id;
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_ASF)
    {
        u4StrmId = pStrmID->u.t_stm_asf_id;
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_MP4)
    {
        u4StrmId = pStrmID->u.t_stm_mp4_id;
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_PS)
    {
        u4StrmId = (pStrmID->u.t_stm_ps_id.ui2_stm_id>>8);
        u4SubStrmId = (pStrmID->u.t_stm_ps_id.ui2_stm_id & 0xFF);
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_TS)
    {
        u4StrmId = pStrmID->u.t_stm_ts_id;
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_MKV)
    {
        u4StrmId = pStrmID->u.t_stm_mkv_id;
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_RM)
    {
        u4StrmId = pStrmID->u.t_stm_rm_id;
    }
    else if (pStrmID->e_type == STREAM_ID_TYPE_ES)
    {
        u4StrmId = pStrmID->u.t_stm_es_id;
    }
    else if(pStrmID->e_type==STREAM_ID_TYPE_FLV)
    {
        u4StrmId = pStrmID->u.t_stm_flv_id;
    }
    else
    {
        LOG(3, "unknown e_type %d", pStrmID->e_type);
        //fgRet = FALSE;
    }

    if(fgRet)
    {
        if (_rDecInfo.u2CompId == 0)
        {
            _rDecInfo.u2CompId = pt_comp_id->ui2_id;
            _rDecInfo.u4StrmId = u4StrmId;
            _rDecInfo.u4SubStrmId = u4SubStrmId;
        }
        else if (_rDecInfo1.u2CompId == 0)
        {
            _rDecInfo1.u2CompId = pt_comp_id->ui2_id;
            _rDecInfo1.u4StrmId = u4StrmId;
            _rDecInfo1.u4SubStrmId = u4SubStrmId;
        }
        else if (_rDecInfo2.u2CompId == 0)
        {
            _rDecInfo2.u2CompId = pt_comp_id->ui2_id;
            _rDecInfo2.u4StrmId = u4StrmId;
            _rDecInfo2.u4SubStrmId = u4SubStrmId;
        }
        else
        {
            LOG(3, "no available u2CompId");
            //fgRet = FALSE;
        }
    }

#ifdef LINUX_TURNKEY_SOLUTION
    if(pStrmID != NULL)
    {
        x_mem_free(pStrmID);
    }
#endif

    return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);
}


static INT32 _MwDmxOnMMSetMediaFormat(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    BOOL fgRet;
    MEDIA_FORMAT_T* prMediaFmt = (MEDIA_FORMAT_T*)pv_set_info;
    //SWDMX_STREAM_OPEN_INFO_T  rOpenInfo;

    _rContainerType = prMediaFmt->e_media_type;
    _u4PreviousSpeed = MM_SPEED_TYPE_FORWARD_1X;

    fgRet = FALSE;
    if (prMediaFmt->e_media_type == MEDIA_TYPE_CONTAINER)
    {
        _rContainerSubType = prMediaFmt->t_media_subtype.u.e_contnr_subtype;

        switch(prMediaFmt->t_media_subtype.u.e_contnr_subtype)
        {
            case MEDIA_CONTNR_SUBTYPE_AVI:
              fgRet = SWDMX_SetContainerType(SWDMX_FMT_AVI);
              break;
            case MEDIA_CONTNR_SUBTYPE_ASF:
              fgRet = SWDMX_SetContainerType(SWDMX_FMT_VC1_WMV);
               /*rOpenInfo.fgStreamInfo = TRUE;
               rOpenInfo.fgVideo = TRUE;
               rOpenInfo.fgAudio = TRUE;
               rOpenInfo.u2VideoId = 2;
               rOpenInfo.u2AudioId =1;
               rOpenInfo.eVideo = ENUM_SWDMX_VDEC_WMV9;
               rOpenInfo.eAudio;

              _SWDMX_Open(SWDMX_FMT_VC1_WMV, &rOpenInfo);*/
              break;
            case MEDIA_CONTNR_SUBTYPE_MP4:
              fgRet = SWDMX_SetContainerType(SWDMX_FMT_MP4);
              break;
            case MEDIA_CONTNR_SUBTYPE_MATROSKA:
              fgRet = SWDMX_SetContainerType(SWDMX_FMT_MKV);
              break;
            case MEDIA_CONTNR_SUBTYPE_RM:
                fgRet = SWDMX_SetContainerType(SWDMX_FMT_RM);
                break;
            case MEDIA_CONTNR_SUBTYPE_FLV:
                fgRet=SWDMX_SetContainerType(SWDMX_FMT_FLV);
                break;
            case MEDIA_CONTNR_SUBTYPE_CMPB:
                fgRet = SWDMX_SetContainerType(SWDMX_FMT_CMPB);
                break;
           default:
              return RMR_DRV_INV_SET_INFO;
        }
    }
    else if (prMediaFmt->e_media_type == MEDIA_TYPE_PS)
    {
        if(prMediaFmt->t_media_subtype.u.e_ps_subtype
            == MEDIA_PS_SUBTYPE_DAT)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG1_DAT);
        }
        else
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_PS);
        }
    }
    else if (prMediaFmt->e_media_type == MEDIA_TYPE_TS)
    {
        _rTsSubType = prMediaFmt->t_media_subtype.u.e_ts_subtype;
        if(prMediaFmt->t_media_subtype.u.e_ts_subtype
            == MEDIA_TS_SUBTYPE_TS_188)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS);
        }
        else if(prMediaFmt->t_media_subtype.u.e_ts_subtype
            == MEDIA_TS_SUBTYPE_TS_192)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_192);
        }
        else if(prMediaFmt->t_media_subtype.u.e_ts_subtype
            == MEDIA_TS_SUBTYPE_TS_192_ZERO)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_ZERO_192);
        }
        else if(prMediaFmt->t_media_subtype.u.e_ts_subtype
            == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_TIME_SHIFT);
        }
        else if(prMediaFmt->t_media_subtype.u.e_ts_subtype
            == MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_TS_ENCRYPT_192);
        }
        else
        {
            LOG(5,"_MwDmxOnMMSetMediaFormat MEDIA_TYPE_TS unknown\n");
        }

        if(!SWDMX_SetInfo((UINT8)eSWDMX_PID_INDEX_START,
            (UINT32)DMX_PES_COMP_ID_START, 0, 0))
        {
            LOG(3,"_MwDmxOnMMSetMediaFormat eSWDMX_PID_INDEX_START fail\n");
        }
    }
    else if (prMediaFmt->e_media_type == MEDIA_TYPE_AUDIO_ES)
    {
        _rAudSubType = prMediaFmt->t_media_subtype.u.e_aud_subtype;
        if(prMediaFmt->t_media_subtype.u.e_aud_subtype
            == MEDIA_AUD_SUBTYPE_WMA)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_WMA_AUDIO_ES);
        }
        else if((prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_MP3)
                ||
                (prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_MP2)
                ||
                (prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_LPCM)
                ||
                (prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_WAVE)
                ||
                (prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_AAC)
                ||
                (prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_AIF)
                ||
                (prMediaFmt->t_media_subtype.u.e_aud_subtype
                 == MEDIA_AUD_SUBTYPE_AC3)
               )
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_RAW_AUDIO_ES);
        }
        else
        {
            LOG(5,"_MwDmxOnMMSetMediaFormat MEDIA_TYPE_AUDIO_ES unknown\n");
        }
    }
    else if (prMediaFmt->e_media_type == MEDIA_TYPE_VIDEO_ES)
    {
        if((prMediaFmt->t_media_subtype.u.e_vid_subtype
            == MEDIA_VID_SUBTYPE_M1V) ||
            (prMediaFmt->t_media_subtype.u.e_vid_subtype
            == MEDIA_VID_SUBTYPE_M2V))
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG2_VIDEO_ES);
        }
        else if(prMediaFmt->t_media_subtype.u.e_vid_subtype
            == MEDIA_VID_SUBTYPE_H264)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_H264_VIDEO_ES);
        }
        else if(prMediaFmt->t_media_subtype.u.e_vid_subtype
            == MEDIA_VID_SUBTYPE_M4V)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_MPEG4_VIDEO_ES);
        }
        else if(prMediaFmt->t_media_subtype.u.e_vid_subtype
            == MEDIA_VID_SUBTYPE_VC1)
        {
            fgRet = SWDMX_SetContainerType(SWDMX_FMT_VC1_ES);
        }
        else
        {
            LOG(5,"_MwDmxOnMMSetMediaFormat MEDIA_TYPE_VIDEO_ES unknown\n");
            //fgRet = FALSE;
        }
    }
    else
    {
        return RMR_DRV_INV_SET_INFO;
    }

#if defined(CC_MT5387) || defined(CC_MT5363)
    // switch to small buffer for audio only file or audio only mode
    if ((prMediaFmt->e_mm_svc_type == MEDIA_SVC_TYPE_AUDIO_ONLY) ||
    	(prMediaFmt->e_mm_svc_type == MEDIA_SVC_TYPE_UNKNOWN && prMediaFmt->e_media_type == MEDIA_TYPE_AUDIO_ES))
    {
        if (FeederSetMMP(FEEDER_PROGRAM_SOURCE) != (INT32)FEEDER_E_OK)
        {
            LOG(0,"_MwDmxOnMMSetMediaFormat MEDIA_TYPE_AUDIO_ES set MMP buffer failed\n");
        }
    }
#endif

    // Set Container Type to SWDMX
    return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);
}


// Convert the speed setting from the upper layer
static INT32 _Speed_convert(MM_SPEED_TYPE_T e_mm_spd)
{
    switch (e_mm_spd)
    {
        case MM_SPEED_TYPE_STEP:          /* Forward 1X */
            return 1;
        case MM_SPEED_TYPE_FORWARD_1X:          /* Forward 1X */
            return 1000;
        case MM_SPEED_TYPE_FORWARD_2X:          /* Fast Forward 2X */
            return 2000;
        case MM_SPEED_TYPE_FORWARD_4X:          /* Fast Forward 4X */
            return 4000;
        case MM_SPEED_TYPE_FORWARD_8X:          /* Fast Forward 8X */
            return 8000;
        case MM_SPEED_TYPE_FORWARD_16X:         /* Fast Forward 16X */
            return 16000;
        case MM_SPEED_TYPE_FORWARD_32X:        /* Fast Forward 32X */
            return 32000;
        case MM_SPEED_TYPE_FORWARD_64X:        /* Fast Forward 64X */
            return 64000;
        case MM_SPEED_TYPE_FORWARD_128X:        /* Fast Forward 128X */
            return 128000;
        case MM_SPEED_TYPE_FORWARD_256X:        /* Fast Forward 256 */
            return 256000;
        case MM_SPEED_TYPE_FORWARD_512X:        /* Fast Forward 512X */
            return 512000;
        case MM_SPEED_TYPE_FORWARD_1024X:        /* Fast Forward 1024X */
            return 1024000;
        case MM_SPEED_TYPE_REWIND_1X:          /* Rewind 1X */
            return (-1000);
        case MM_SPEED_TYPE_REWIND_2X:          /* Fast Rewind 2X */
            return (-2000);
        case MM_SPEED_TYPE_REWIND_4X:          /* Fast Rewind 4X */
            return (-4000);
        case MM_SPEED_TYPE_REWIND_8X:          /* Fast Rewind 8X */
            return (-8000);
        case MM_SPEED_TYPE_REWIND_16X:         /* Fast Rewind 16X */
            return (-16000);
        case MM_SPEED_TYPE_REWIND_32X:        /* Fast Rewind 32X */
            return (-32000);
        case MM_SPEED_TYPE_REWIND_64X:        /* Fast Rewind 64X */
            return -64000;
        case MM_SPEED_TYPE_REWIND_128X:        /* Fast Rewind 128X */
            return -128000;
        case MM_SPEED_TYPE_REWIND_256X:        /* Fast Rewind 256 */
            return -256000;
        case MM_SPEED_TYPE_REWIND_512X:        /* Fast Rewind 512X */
            return -512000;
        case MM_SPEED_TYPE_REWIND_1024X:        /* Fast Rewind 1024X */
            return -1024000;
        case MM_SPEED_TYPE_FORWARD_1_DIV_2X:     /* Slow Rewind 1/2X */
            return (1000/2);
        case MM_SPEED_TYPE_FORWARD_1_DIV_4X:     /* Slow Rewind 1/4X */
            return (1000/4);
        case MM_SPEED_TYPE_FORWARD_1_DIV_8X:     /* Slow Rewind 1/8X */
            return (1000/8);
        case MM_SPEED_TYPE_FORWARD_1_DIV_16X:    /* Slow Rewind 1/16X */
            return (1000/16);
        case MM_SPEED_TYPE_FORWARD_1_DIV_32X:    /* Slow Rewind 1/32X */
            return (1000/32);
    }

    return  1000;
}

static INT32 _MwDmxOnMMSetPriority(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    return RMR_OK;
}

static INT32 _MwDmxOnMMSetSpeed(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    BOOL fgRet;
    BOOL fgAudPlay = FALSE;
    UINT32 u4Speed = (UINT32)pv_set_info;
    BOOL fgResetSwDmx = TRUE;

    if(u4Speed == MM_SPEED_TYPE_STEP || u4Speed == MM_SPEED_TYPE_STEP_REVERSE)
    {
        if(u4Speed == MM_SPEED_TYPE_STEP_REVERSE)
        {
            return RMR_DRV_INV_SET_INFO;
        }

        if(_rContainerType == MEDIA_TYPE_CONTAINER)
        {
            if((_rContainerSubType == MEDIA_CONTNR_SUBTYPE_ASF) ||
                (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_AVI) ||
                (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_RM))
            {
                if(!_fgKeyIsExist)
                {
                    return RMR_DRV_INV_SET_INFO;
                }
            }
        }
        _MwDmxFlushCtrl();
        fgRet = SWDMX_Pause();
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_Pause fail\n");
        }
        _MwDmxWaitCtrlDone();
        fgRet = SWDMX_SetSpeed(_Speed_convert((MM_SPEED_TYPE_T)u4Speed));
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_SetSpeed fail\n");
        }

        fgRet = SWDMX_Play();
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_Play fail\n");
        }

        if(_fgVideoExist)
        {
            STC_SetSyncMode(AV_SYNC_MODE_NONE);
        }
    }
    else if(u4Speed == MM_SPEED_TYPE_PAUSE)
    {
        LOG(5,"_MwDmxOnMMSetSpeed MM_SPEED_TYPE_PAUSE\n");
        _MwDmxFlushCtrl();
        fgRet = SWDMX_Pause();
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_Pause fail\n");
        }
        _MwDmxWaitCtrlDone();

		// DTV00147394,  
		// reason: After mw change the flow of change speed as this: 4x-->pause(set speed=pause)-->
		// enalbe abudio stream ---> set speed 1x,  So when change speed " set speed pause "will update 
		//  _u4PreviousSpeed=MM_SPEED_TYPE_PAUSE, this will ignore do "SWDMX_Flush" when doing "set speed=1x"
		// So, if _u4PreviousSpeed != MM_SPEED_TYPE_FORWARD_1X,  We should not update _u4PreviousSpeed=MM_SPEED_TYPE_PAUSE
        if(_u4PreviousSpeed !=MM_SPEED_TYPE_FORWARD_1X)
        {
			LOG(5,"_MwDmxOnMMSetSpeed just pause swdmx\n");
			return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);
        }
        else
        {
           _bPreviousStateIsPause = TRUE;
           _ui4StcValue = STC_GetStcValue();
           //for mkv subtitle, subtitle should display at pause, so subtitle engine can't be stop
           //in this case, stc should be stop to avoid subtitle change
           if(_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_1X)
           {
             STC_StopStc();
           }
        }
    }
    else if(u4Speed == MM_SPEED_TYPE_FORWARD_1X)
    {
        LOG(5,"_MwDmxOnMMSetSpeed MM_SPEED_TYPE_FORWARD_1X\n");
        _MwDmxFlushCtrl();
        fgRet = SWDMX_Pause();
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_Pause fail\n");
        }
        _MwDmxWaitCtrlDone();
        while(1)
        {
            if(_fgAudNotSupport)
            {
                break;
            }
            else if((_fgAudCtrlDone) &&
               ((_u4AudCtrl == AUD_DEC_CTRL_PLAY) ||
                (_u4AudCtrl == AUD_DEC_CTRL_PLAY_SYNC) ||
                ((_rContainerType == MEDIA_TYPE_AUDIO_ES)&&
                 (_u4AudCtrl == AUD_DEC_CTRL_RESUME) &&
                 (_u4PreviousSpeed !=MM_SPEED_TYPE_PAUSE))))     //for change audio trick to normal mode. play audio
            {
                fgAudPlay = TRUE;
                break;
            }			
            else if((_fgAudCtrlDone) &&
               ((_u4AudCtrl != AUD_DEC_CTRL_PLAY) &&
                (_u4AudCtrl != AUD_DEC_CTRL_PLAY_SYNC)))
            {
                break;
            }
            LOG(5,"_MwDmxOnMMSetSpeed 1X wait aud ctrl done\n");
            x_thread_delay(10);
        }
        //_MwDmxMMLockFlushMutex();

        if ((_rContainerType == MEDIA_TYPE_CONTAINER)&&
			(_rContainerSubType == MEDIA_CONTNR_SUBTYPE_AVI ||
            _rContainerSubType == MEDIA_CONTNR_SUBTYPE_MP4 ||
            _rContainerSubType == MEDIA_CONTNR_SUBTYPE_MATROSKA||
            _rContainerSubType == MEDIA_CONTNR_SUBTYPE_RM||
            _rContainerSubType==MEDIA_CONTNR_SUBTYPE_FLV))
        {
            if (!((_u4PreviousSpeed >= MM_SPEED_TYPE_PAUSE)&&(_u4PreviousSpeed <= MM_SPEED_TYPE_FORWARD_2X)))
            {
                UNUSED(SWDMX_Flush());
            }
        }
        else if (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_RM)
        {
            if (_u4PreviousSpeed != MM_SPEED_TYPE_PAUSE &&
                _u4PreviousSpeed != MM_SPEED_TYPE_FORWARD_2X &&
                !(_u4PreviousSpeed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X && _u4PreviousSpeed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X) &&
                _u4PreviousSpeed != MM_SPEED_TYPE_STEP)
            {
                UNUSED(SWDMX_Flush());
            }
        }
		else if(_rContainerType == MEDIA_TYPE_VIDEO_ES)    // DTV00078801 for video ES when <pause-2X>  to 1x  no need to do swdmx flush   
		{
		    if (!((_u4PreviousSpeed >= MM_SPEED_TYPE_PAUSE)&&(_u4PreviousSpeed <= MM_SPEED_TYPE_FORWARD_2X)))
		   {
			  UNUSED(SWDMX_Flush());
		   }
		}		
        else
        {
            if (_u4PreviousSpeed != MM_SPEED_TYPE_PAUSE)
            {
                UNUSED(SWDMX_Flush());
            }
        }

        if((_rContainerType == MEDIA_TYPE_TS) &&
           (_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
        {
            STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
        }
        else if((_rContainerType == MEDIA_TYPE_TS) && (_bIsThumbnailPb))
        {
            STC_SetSyncMode(AV_SYNC_MODE_NONE);
        }
        else if(_rContainerType == MEDIA_TYPE_VIDEO_ES)
        {
            STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
			STC_StartStc();
			
        }		
        else
        {
            // always audio master
            STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
            /*
            if (_fgVideoExist)
            {
                STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
            }
            else
            {
                STC_SetSyncMode(AV_SYNC_MODE_NONE);
            }
            */
        }

        //_MwDmxMMUnlockFlushMutex();
        fgRet = SWDMX_SetSpeed(1000);
        if(fgAudPlay)
        {
#ifdef CC_MT5363
            AUD_DSPCmdPlay(1, 0);
            AUD_DSPCmdPlay(0, 0);
#else
            AUD_DSPCmdPlay(0);
#endif
            LOG(5,"_MwDmxOnMMSetSpeed 1X AUD_DSPCmdPlay\n");
        }
        SWDMX_Play();
        if (_bPreviousStateIsPause &&
            !_fgPreviousStateIsSeek &&
            (_u4PreviousSpeed != MM_SPEED_TYPE_STEP))
        {
            STC_SetStcValue(_ui4StcValue);
            //LOG(0,"_MwDmxOnMMSetSpeed SetStcValue %d\n",_ui4StcValue);
            _bPreviousStateIsPause = FALSE;
            //for mkv subtitle, subtitle should display at pause, so subtitle engine can't be stop
            //in this case, stc should be stop to avoid subtitle change
            //restart stc if pause->1x, will be start also if pause-> seek at VDP
            STC_StartStc();
        }

        if(_fgPreviousStateIsTrick)
        {
            #if 0
            UINT32 u4CurPts = 0;
            UINT64 u8FilePos = 0;

            STC_StopStc();
            VDP_SetPauseMm(0, TRUE);
            u4CurPts = VDP_GetPts(0);

            SWDMX_SetSpeed(1000);
            SWDMX_SeekTime(u4CurPts, &u8FilePos);

            VDP_SetPauseMm(0, FALSE);
            VDP_SetSpeed(0, STC_SPEED_TYPE_FORWARD_1X);

            STC_SetStcValue(u4CurPts);
            STC_StartStc();
            #endif
        }
        _fgPreviousStateIsSeek = FALSE;
        _fgPreviousStateIsTrick = FALSE;
    }
    else
    {
        if(_rContainerType == MEDIA_TYPE_CONTAINER)
        {
            //if(u4Speed != MM_SPEED_TYPE_FORWARD_2X)
            //{
                if( (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_AVI) ||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MP4) ||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_RM)||
                     (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_FLV))
                {
                    if(!_fgKeyIsExist)
                    {
                        //STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
                        //STC_StartStc();
                        return RMR_DRV_INV_SET_INFO;
                    }
                }
                else if((_rContainerSubType == MEDIA_CONTNR_SUBTYPE_ASF) ||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MATROSKA))
                {
                    UINT32 fgTrick;
                    SWDMX_GetInfo(eSWDMX_GET_IS_TRICK,&fgTrick,0,0);
                    _fgKeyIsExist = (BOOL)fgTrick;
                    if(!_fgKeyIsExist)
                    {
                        //STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
                        //STC_StartStc();
                        return RMR_DRV_INV_SET_INFO;
                    }
                }
            //}
        }
        _fgPreviousStateIsTrick = TRUE;

        if(_bPreviousStateIsPause)
        {
            STC_StartStc();
            _bPreviousStateIsPause = FALSE;
        }

        _MwDmxFlushCtrl();
        fgRet = SWDMX_Pause();
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_Pause fail\n");
        }
        _MwDmxWaitCtrlDone();
        /*
        while(1)
        {
            if((_fgAudCtrlDone) &&
               ((_u4AudCtrl != AUD_DEC_CTRL_PLAY) &&
                (_u4AudCtrl != AUD_DEC_CTRL_PLAY_SYNC)))
            {
                break;
            }
            LOG(5,"_MwDmxOnMMSetSpeed !1X wait aud ctrl done\n");
            x_thread_delay(10);
        }
        */
        //_MwDmxMMLockFlushMutex();
        #if 1
        if (((_u4PreviousSpeed >= MM_SPEED_TYPE_PAUSE)&&
             (_u4PreviousSpeed <= MM_SPEED_TYPE_FORWARD_2X)) &&
            ((u4Speed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X) &&
             (u4Speed <= MM_SPEED_TYPE_FORWARD_2X)))
        {            
            /* Slow Forward, 1x, 2x -> Slow Forward, 1x, 2x */
            fgResetSwDmx = FALSE;
        }
        #else
        if (((_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_1X) ||
            (_u4PreviousSpeed == MM_SPEED_TYPE_PAUSE) ||
            (_u4PreviousSpeed == MM_SPEED_TYPE_STEP)) &&
            (u4Speed == MM_SPEED_TYPE_FORWARD_2X))
        {
            /* 1x -> 2x */
            fgResetSwDmx = FALSE;
        }
        else if (((_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_1X) ||
            (_u4PreviousSpeed == MM_SPEED_TYPE_PAUSE) ||
            (_u4PreviousSpeed == MM_SPEED_TYPE_STEP)) &&
            ((u4Speed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X) && (u4Speed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X)))
        {
            /* 1x -> Slow Forward */
            fgResetSwDmx = FALSE;
        }
        else if (((_u4PreviousSpeed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X) && (_u4PreviousSpeed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X)) &&
            ((u4Speed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X) && (u4Speed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X)))
        {
            /* Slow Forward -> Slow Forward */
            fgResetSwDmx = FALSE;
        }
        #endif
        if((fgResetSwDmx)&&(_rContainerType != MEDIA_TYPE_AUDIO_ES))     //for audio trick mode, no need to flush audio FIFO
        {
            /* 1x -> 2x or Slow motion not to SWDMX ?! */
            UNUSED(SWDMX_Flush());
        }
        /*else
              {
                  UNUSED(SWDMX_Flush());
              }*/
		if (_rContainerType == MEDIA_TYPE_AUDIO_ES)      //for audio trick mode A/V sync mode is audio master
		{
			STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);

			STC_StartStc(); //DTV00083152 for some specail mp3 file , when trick mode because decoder error, audio need to wait STC reach PTS, so we should start stc in audio trick mode.	
		}
        else if(((u4Speed == MM_SPEED_TYPE_FORWARD_2X)
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
           &&
           ((_rContainerType != MEDIA_TYPE_TS) ||
            (_rTsSubType != MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
#endif
           )
           || (u4Speed == MM_SPEED_TYPE_FORWARD_1_DIV_2X)
           || (u4Speed == MM_SPEED_TYPE_FORWARD_1_DIV_4X)
           || (u4Speed == MM_SPEED_TYPE_FORWARD_1_DIV_8X)
           || (u4Speed == MM_SPEED_TYPE_FORWARD_1_DIV_16X)
           || (u4Speed == MM_SPEED_TYPE_FORWARD_1_DIV_32X))
        {
            STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
        }
        else
        {
#if 0  // megaa tmp rollback
            STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
            // if we set none, vdec will to underflow check
            // prVdecEsInfo->u2UdfNs = MPV_UDF_PES_NS
            // so use audio master
#else
            STC_SetSyncMode(AV_SYNC_MODE_NONE);
#endif
        }


        //_MwDmxMMUnlockFlushMutex();
        fgRet = SWDMX_SetSpeed(_Speed_convert((MM_SPEED_TYPE_T)u4Speed));
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_SetSpeed fail\n");
        }
        fgRet = SWDMX_Play();
        if(!fgRet)
        {
            LOG(1,"_MwDmxOnMMSetSpeed SWDMX_Play fail\n");
        }
    }
    _u4PreviousSpeed = u4Speed;
    return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);
}


static INT32 _MwDmxOnMMSetSeekPos(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    BOOL fgRet = FALSE;
    MM_RANGE_POS_INFO_T* prSeekInfo;
    SWDMX_RANGE_INFO_T rRangeInfo;
    UINT64 u8FilePos;
    UINT32 u4SeekTime = 0;
    BOOL fgAudPlay = FALSE;

    prSeekInfo = (MM_RANGE_POS_INFO_T*)pv_set_info;

    if(prSeekInfo == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    LOG(5,"_MwDmxOnMMSetSeekPos\n");

    _MwDmxFlushCtrl();
    fgRet = SWDMX_Pause();
    if(!fgRet)
    {
        LOG(1,"_MwDmxOnMMSetSeekPos SWDMX_Pause fail\n");
    }
    _MwDmxWaitCtrlDone();
    while(1)
    {
        if(_fgAudNotSupport)
        {
            break;
        }
        if((_fgAudCtrlDone) &&
           ((_u4AudCtrl != AUD_DEC_CTRL_PLAY) &&
            (_u4AudCtrl != AUD_DEC_CTRL_PLAY_SYNC)))
        {
            break;
        }
        else if((_fgAudCtrlDone) &&
           ((_u4AudCtrl == AUD_DEC_CTRL_PLAY) ||
            (_u4AudCtrl == AUD_DEC_CTRL_PLAY_SYNC)))
        {
            fgAudPlay = TRUE;
            break;
        }
        LOG(5,"_MwDmxOnMMSetSeekPos wait aud ctrl done\n");
        x_thread_delay(10);
    }
    //_MwDmxMMLockFlushMutex();
    UNUSED(SWDMX_Flush());
    //_MwDmxMMUnlockFlushMutex();
    if(fgAudPlay)
    {
#ifdef CC_MT5363
        AUD_DSPCmdPlay(1, 0);
        AUD_DSPCmdPlay(0, 0);
#else
        AUD_DSPCmdPlay(0);
#endif
        LOG(5,"_MwDmxOnMMSetSeekPos AUD_DSPCmdPlay\n");
    }

    //seek
    SWDMX_QueryRangeInfo(&rRangeInfo);
    if(prSeekInfo->e_pos_type == MM_RANGE_POS_TYPE_TIME_RANGE_CURR)
    {
        u4SeekTime = (UINT32)rRangeInfo.u8CurRangePTS + (UINT32)prSeekInfo->ui8_pos_val;
        fgRet = SWDMX_SeekTime(u4SeekTime, &u8FilePos);
    }
    else if(prSeekInfo->e_pos_type == MM_RANGE_POS_TYPE_TIME_RANGE_ID)
    {
        if(rRangeInfo.u4CurRangeID == prSeekInfo->ui4_idx)
        {
            u4SeekTime = (UINT32)rRangeInfo.u8CurRangePTS + (UINT32)prSeekInfo->ui8_pos_val;
        }
        else if(rRangeInfo.u4QRangeID == prSeekInfo->ui4_idx)
        {
            u4SeekTime = (UINT32)rRangeInfo.u8QRangePTS + (UINT32)prSeekInfo->ui8_pos_val;
        }
        else
        {
            return RMR_DRV_INV_SET_INFO;
        }
        fgRet = SWDMX_SeekTime(u4SeekTime, &u8FilePos);
    }
    else if (prSeekInfo->e_pos_type == MM_RANGE_POS_TYPE_TIME_MEDIA)
    {
        u4SeekTime = (UINT32)prSeekInfo->ui8_pos_val;
        fgRet = SWDMX_SeekTime(u4SeekTime, &u8FilePos);
    }
    else if (prSeekInfo->e_pos_type == MM_RANGE_POS_TYPE_OFFSET_RANGE_CURR)
    {
        u8FilePos = prSeekInfo->ui8_pos_val;
        fgRet = SWDMX_SeekPos(u8FilePos, u8FilePos);
    }
    else if(prSeekInfo->e_pos_type == MM_RANGE_POS_TYPE_OFFSET_RANGE_ID)
    {
        if(rRangeInfo.u4CurRangeID == prSeekInfo->ui4_idx)
        {
            u8FilePos = prSeekInfo->ui8_pos_val;
        }
        else
        {
            return RMR_DRV_INV_SET_INFO;
        }
        fgRet = SWDMX_SeekPos(u8FilePos, u8FilePos);
    }
    else if(prSeekInfo->e_pos_type == MM_RANGE_POS_TYPE_OFFSET_RANGE_START)
    {
        fgRet = SWDMX_RenderFromPos((VOID*)&prSeekInfo->t_start_pos);
    }
    if(!_fgVideoExist) // audio only
    {
        MW_DMX_NotifyStepDone(0, 0, 0);
    }
    else
    {
        if(prSeekInfo->e_auto_speed != MM_SPEED_TYPE_PAUSE)
        {
            VDP_SetSeek(VDP_1, 1, 0);
            VDP_RegCbFunc(VDP_CB_FUNC_MM_SEEK_FIN_IND, (UINT32)_MW_DMX_NotifySeekDone, 0, 0);
        }
    }

    _fgPreviousStateIsSeek = TRUE;
    return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);

}

#ifdef LINUX_TURNKEY_SOLUTION
typedef struct _RANGE_LIST_T
{
    MM_RANGE_INFO_T *pt_range;
    struct _RANGE_LIST_T * pt_next;
    struct _RANGE_LIST_T * pt_prev;
}   RANGE_LIST_T;

static RANGE_LIST_T* g_pt_range_list = NULL;

static VOID _MwDmx_add_range_list(MM_RANGE_INFO_T *pt_range)
{
    RANGE_LIST_T* pt_temp = NULL;

    if(NULL == pt_range)
    {
        return;
    }

    pt_temp = (RANGE_LIST_T*)x_mem_alloc(sizeof(RANGE_LIST_T));
    x_memset(pt_temp,0,sizeof(RANGE_LIST_T));

    pt_temp->pt_range = pt_range;

    pt_temp->pt_next = g_pt_range_list;

    if(g_pt_range_list != NULL)
    {
        g_pt_range_list->pt_prev = pt_temp;
    }

    g_pt_range_list = pt_temp;

    return;
}

static VOID _MwDmx_recursion_free_data(MM_RANGE_ELMT_IDXTBL_T *pt_data)
{
    VOID * p_temp = NULL;

    if(NULL != pt_data)
    {

        /* Deal pv_next_tbl data. */
        _MwDmx_recursion_free_data(pt_data->pv_next_tbl);

        /* Deal pt_next_partial data. */
        _MwDmx_recursion_free_data(pt_data->pt_next_partial);

        /* Deal pt_pb_idx_tbl_entry data. */
        if(pt_data->u.pt_pb_idx_tbl_entry != NULL)
        {
            x_mem_free(pt_data->u.pt_pb_idx_tbl_entry);
        }

        x_mem_free(pt_data);
    }

    return;

}

static VOID _MwDmx_free_range_node(RANGE_LIST_T* pt_node)
{
    if(NULL == pt_node)
    {
        return;
    }

    if(NULL == pt_node->pt_range)
    {
        x_mem_free(pt_node);
        return;
    }

    if(pt_node->pt_range->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info != NULL)
    {
        x_mem_free(pt_node->pt_range->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info);
    }

    if(pt_node->pt_range->pt_track_tm_scale_lst != NULL)
    {
        x_mem_free(pt_node->pt_range->pt_track_tm_scale_lst);
    }

    _MwDmx_recursion_free_data(pt_node->pt_range->pt_idxtbl_lst);

    x_mem_free(pt_node->pt_range);

    x_mem_free(pt_node);

    return;
}

static VOID _MwDmx_remove_range_list(UINT32 ui4_range_id)
{
    RANGE_LIST_T* pt_temp = g_pt_range_list;

    while(pt_temp != NULL)
    {
        if(pt_temp->pt_range->ui4_range_id == ui4_range_id)
        {
            if(pt_temp->pt_prev != NULL)
            {
                pt_temp->pt_prev->pt_next = pt_temp->pt_next;
            }
            else
            {
                g_pt_range_list = pt_temp->pt_next;
            }

            if(pt_temp->pt_next != NULL)
            {
                pt_temp->pt_next->pt_prev = pt_temp->pt_prev;
            }

            _MwDmx_free_range_node(pt_temp);

            break;
        }

        pt_temp = pt_temp->pt_next;
    }

    return;
}

static BOOL _MwDmx_nfy_fct(VOID*                  pv_nfy_tag,
                         MM_RANGE_COND_T          e_nfy_cond,
                         UINT32                 ui4_data_1,
                         UINT32                 ui4_data_2)
{
    mwdmx_param.pv_nfy_tag = pv_nfy_tag;
    mwdmx_param.e_nfy_cond = e_nfy_cond;
    mwdmx_param.ui4_data_1 = ui4_data_1;
    mwdmx_param.ui4_data_2 = ui4_data_2;
    mwdmx_param.pf_nfy = mwdmx_nfy;
    _CB_PutEvent_Adpt(CB_DRV_MWDMX_NFY, sizeof(MWDMX_NFY_PARAM), &mwdmx_param);

    /* Free memory for range. */
    if(e_nfy_cond == MM_RANGE_DONE)
    {
        _MwDmx_remove_range_list(ui4_data_1);
    }

    return TRUE;
}

#ifdef TIME_SHIFT_SUPPORT
static BOOL _MwDmx_set_dmx_nfy(UINT32          ui4_id,
                        VOID*           pv_nfy_tag,
                        DEMUX_COND_T    e_nfy_cond,
                        UINT32          ui4_data_1,
                        UINT32          ui4_data_2
            #ifndef MW_CLASSIC_DMX_NFY
                        ,
                        UINT64          ui8_ext_data_1,
                        UINT64          ui8_ext_data_2,
                        UINT32          ui4_ext_data_3
            #endif
                        )
{
    DMX_NFY_PARAM dmx_param;
    dmx_param.pf_nfy = _arScrambleState[ui4_id].pf_demux_nfy;
    dmx_param.pv_nfy_tag = pv_nfy_tag;
    dmx_param.e_nfy_cond = e_nfy_cond;
    dmx_param.ui4_data_1 = ui4_data_1;
    dmx_param.ui4_data_2 = ui4_data_2;
#ifndef MW_CLASSIC_DMX_NFY
    dmx_param.ui8_ext_data_1 = ui8_ext_data_1;
    dmx_param.ui8_ext_data_2 = ui8_ext_data_2;
    dmx_param.ui4_ext_data_3 = ui4_ext_data_3;
#endif
    _CB_PutEvent_Adpt(CB_DRV_DMX_NFY, sizeof(DMX_NFY_PARAM), &dmx_param);
    return FALSE;
}
#endif

static INT32 _MwDmx_recursion_data(MM_RANGE_ELMT_IDXTBL_T **ppt_data)
{
    VOID * p_temp = NULL;
    INT32 i4_ret = RMR_OK;

    if(NULL != *ppt_data)
    {
        p_temp = *ppt_data;

        *ppt_data = x_mem_alloc(sizeof(MM_RANGE_ELMT_IDXTBL_T));
        if (*ppt_data == NULL)
        {
            return RMR_NOT_ENOUGH_MEM;
        }

        rm_copy_from_user(*ppt_data,p_temp,sizeof(MM_RANGE_ELMT_IDXTBL_T));

        /* Deal pt_pb_idx_tbl_entry/pt_ts_idx_tbl_entry data. */
        if((*ppt_data)->e_tbl_type == MM_RANGE_ELEMT_IDXTBL_TYPE_TIMESHIFT)
        {
            if((*ppt_data)->u.pt_ts_idx_tbl_entry != 0)
            {
                p_temp = (*ppt_data)->u.pt_ts_idx_tbl_entry;
                (*ppt_data)->u.pt_ts_idx_tbl_entry = x_mem_calloc(1,sizeof(MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T));

                if ((*ppt_data)->u.pt_ts_idx_tbl_entry)
                {
                    rm_copy_from_user((*ppt_data)->u.pt_ts_idx_tbl_entry, p_temp,
                        sizeof(MM_RANGE_ELEMT_IDXTBL_TS_ENTRY_T));
                }
                else /* Error handling and error logs here */
                {
                    _MwDmx_recursion_free_data(*ppt_data);
                    *ppt_data = NULL;
                    return RMR_NOT_ENOUGH_MEM;
                }
            }
        }
        else
        {
            if((*ppt_data)->ui4_number_of_entry > 0 && (*ppt_data)->u.pt_pb_idx_tbl_entry != 0)
            {
                p_temp = (*ppt_data)->u.pt_pb_idx_tbl_entry;
                (*ppt_data)->u.pt_pb_idx_tbl_entry = x_mem_calloc((*ppt_data)->ui4_number_of_entry,sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

                if ((*ppt_data)->u.pt_pb_idx_tbl_entry)
                {
                    rm_copy_from_user((*ppt_data)->u.pt_pb_idx_tbl_entry, p_temp,
                        sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T) * (*ppt_data)->ui4_number_of_entry);
                }
                else /* Error handling and error logs here */
                {
                    _MwDmx_recursion_free_data(*ppt_data);
                    *ppt_data = NULL;
                    return RMR_NOT_ENOUGH_MEM;
                }
            }
        }

        /* Deal pt_next_partial data. */
        if(RMR_OK != (i4_ret = _MwDmx_recursion_data(&((*ppt_data)->pt_next_partial))))
        {
            _MwDmx_recursion_free_data(*ppt_data);
            *ppt_data = NULL;
            return RMR_NOT_ENOUGH_MEM;
        }

        /* Deal pv_next_tbl data. */
        if(RMR_OK != (i4_ret = _MwDmx_recursion_data(&((*ppt_data)->pv_next_tbl))))
        {
            _MwDmx_recursion_free_data(*ppt_data);
            *ppt_data = NULL;
            return RMR_NOT_ENOUGH_MEM;
        }
    }

    return RMR_OK;

}

static INT32 _MwDmx_pad_data_from_usr_struct(MM_RANGE_INFO_T** pprDmxRange,const VOID* pv_set_info)
{
    VOID *ptemp = NULL;
    INT32 i4_ret = RMR_OK;

    if(pprDmxRange == NULL || pv_set_info == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    *pprDmxRange = (MM_RANGE_INFO_T*)x_mem_alloc(sizeof(MM_RANGE_INFO_T));

    if (*pprDmxRange == NULL)
    {
        return RMR_NOT_ENOUGH_MEM;
    }

    /* rm_copy_from_user(*pprDmxRange,pv_set_info,sizeof(MM_RANGE_INFO_T));*/
    x_memcpy(*pprDmxRange,pv_set_info,sizeof(MM_RANGE_INFO_T));

    if(RMR_OK != (i4_ret = _MwDmx_recursion_data(&((*pprDmxRange)->pt_idxtbl_lst))))
    {
        _MwDmx_free_range_node(*pprDmxRange);
        *pprDmxRange = NULL;
        return i4_ret;
    }

    if((*pprDmxRange)->ui4_track_tm_scale_num > 0)
    {
        ptemp = (*pprDmxRange)->pt_track_tm_scale_lst;

        (*pprDmxRange)->pt_track_tm_scale_lst = x_mem_calloc((*pprDmxRange)->ui4_track_tm_scale_num,
            sizeof(MM_RANGE_TRACK_TM_SCALE_T));
        if ((*pprDmxRange)->pt_track_tm_scale_lst)
        {
            rm_copy_from_user((*pprDmxRange)->pt_track_tm_scale_lst, ptemp,
                               sizeof(MM_RANGE_TRACK_TM_SCALE_T) * ((*pprDmxRange)->ui4_track_tm_scale_num));
        }
        else /* Error handling and error logs here */
        {
            _MwDmx_free_range_node(*pprDmxRange);
            *pprDmxRange = NULL;
            return RMR_NOT_ENOUGH_MEM;
        }

    }

    if((*pprDmxRange)->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info != 0)
    {
        ptemp = (*pprDmxRange)->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info;
        (*pprDmxRange)->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info = x_mem_alloc(sizeof(DIVX_DRM_CRYPTO_INFO_T));

        if ((*pprDmxRange)->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info)
        {
            rm_copy_from_user((*pprDmxRange)->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info,
                               ptemp, sizeof(DIVX_DRM_CRYPTO_INFO_T));
        }
        else /* Error handling and error logs here */
        {
            _MwDmx_free_range_node(*pprDmxRange);
            *pprDmxRange = NULL;
            return RMR_NOT_ENOUGH_MEM;
        }

    }

    return RMR_OK;
}
#endif

static INT32 _MwDmxOnMMAddRange(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    BOOL fgRet;
    MM_RANGE_INFO_T* prDmxRange;

#ifndef LINUX_TURNKEY_SOLUTION
    prDmxRange = (MM_RANGE_INFO_T*)pv_set_info;
#else
    if(_MwDmx_pad_data_from_usr_struct(&prDmxRange,pv_set_info) != 0)
    {
        return RMR_DRV_ERROR;
    }

    _MwDmx_add_range_list(prDmxRange);
#endif

    fgRet = SWDMX_AddRange((VOID*)prDmxRange, prDmxRange->ui4_range_id);

    if (_bFirstRange)
    {
        STC_SetStcValue((UINT32)prDmxRange->t_init_stc);
        _bFirstRange = FALSE;
    }

#ifndef LINUX_TURNKEY_SOLUTION
    prDmxRange = (MM_RANGE_INFO_T*)pv_set_info;
#endif

    _fgKeyIsExist = prDmxRange->b_key_tbl_exist;
    _bIsThumbnailPb=prDmxRange->b_thumbnail_pb;
    _pfnSwdmxRangeCb = prDmxRange->pt_range_nfy;
    _pRangeCbTag = prDmxRange->pv_tag;
    _u4RangeID = prDmxRange->ui4_range_id;
#ifndef LINUX_TURNKEY_SOLUTION
    SWDMX_SetInfo(eSWDMX_SET_RANGECALLBACK, (UINT32)prDmxRange->pt_range_nfy, 0, 0);
#else
    mwdmx_nfy = prDmxRange->pt_range_nfy;
    SWDMX_SetInfo(eSWDMX_SET_RANGECALLBACK, (UINT32)_MwDmx_nfy_fct, 0, 0);
#endif

    return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);
}


static INT32 _MwDmxOnMMDelRange(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    BOOL fgRet;
    MM_RANGE_INFO_T* prDmxRange;

    prDmxRange = (MM_RANGE_INFO_T*)pv_set_info;
    fgRet = SWDMX_DelRange((VOID*)prDmxRange, prDmxRange->ui4_range_id, FALSE);

    return ((fgRet == TRUE) ? RMR_OK : RMR_DRV_INV_SET_INFO);
}

/*
#ifdef TIME_SHIFT_SUPPORT
static BOOL _MwDmxSetDecrypt(void)
{
    DMX_PID_T rPid;
    UINT8 i, j;

    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(!DMX_GetPid(i, DMX_PID_FLAG_VALID | DMX_PID_FLAG_PID | DMX_PID_FLAG_TS_INDEX, &rPid))
        {
            return FALSE;
        }

        //For safety, comment by DMX PIC, to prevent race condition between swdmx thread and this thread
        //if(rPid.fgEnable)
        {
            for(j=0; j<DMX_NUM_PID_INDEX; j++)
            {
                if(_arDmxMMKeyInfo[j].fgValid && (rPid.u2Pid == _arDmxMMKeyInfo[j].u2StreamID))
                {
                    //rPid.eDescMode = DMX_DESC_MODE_NONE;//_arDmxMMKeyInfo[j].eDescMode;
                    rPid.eDescMode = DMX_DESC_MODE_AES_ECB;
                    rPid.u1KeyIndex = _arDmxMMKeyInfo[j].u1KeyIdx;
                    if(!DMX_SetPid(i, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid))
                    {
                        return FALSE;
                    }
                }
            }
        }
    }

    return TRUE;
}
#endif  // TIME_SHIFT_SUPPORT
*/

static INT32 _MwDmxOnMMSetControl(DRV_COMP_ID_T* pt_comp_id,
  const VOID* pv_set_info)
{
    UINT32 u4Enable;
    u4Enable = (UINT32)pv_set_info;

    if(u4Enable)
    {
        if ((_rDecInfo.u2CompId > 0) &&
            (_rDecInfo.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
        {
            _fgVideoExist = TRUE;
            _u2VideoCompId = _rDecInfo.u2CompId;
        }
        else if ((_rDecInfo1.u2CompId > 0) &&
            (_rDecInfo1.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
        {
            _fgVideoExist = TRUE;
            _u2VideoCompId = _rDecInfo1.u2CompId;
        }
        else if ((_rDecInfo2.u2CompId > 0) &&
            (_rDecInfo2.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
        {
            _fgVideoExist = TRUE;
            _u2VideoCompId = _rDecInfo2.u2CompId;
        }
        else
        {
            LOG(5, "no video\n");
        }

        if (pt_comp_id->e_type == DRVT_DEMUX_CONTAINER_ES)
        {
            UINT32 j;
            BOOL fgVideo = FALSE;
            BOOL fgDecrypt = FALSE;
            UINT8 u1KeyIdx = 0;
            DMX_DESC_MODE_T eDescMode = DMX_DESC_MODE_NONE;
            UNUSED(fgDecrypt);
            UNUSED(u1KeyIdx);
            UNUSED(eDescMode);
            UNUSED(j);
            if ((!_rDecInfo.fgEnable) &&
                (_rDecInfo.u2CompId == pt_comp_id->ui2_id))
            {
                SWDMX_SetStrmID(_rDecInfo.u4StrmId, _rDecInfo.u4SubStrmId, (ENUM_SWDMX_STRM_TYPE_T)_rDecInfo.u2DecType);

#ifdef TIME_SHIFT_SUPPORT
                if((_rContainerType == MEDIA_TYPE_TS) &&
                   ((_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT) ||
                    (_rTsSubType == MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION)))
                {
                    for(j=0; j<DMX_NUM_PID_INDEX; j++)
                    {
                        if(_arDmxMMKeyInfo[j].fgValid && (_rDecInfo.u4StrmId == _arDmxMMKeyInfo[j].u2StreamID))
                        {
                            fgDecrypt = TRUE;
#ifdef CC_MT5387
                            eDescMode = DMX_DESC_MODE_AES_ECB;
#else
                            eDescMode = _arDmxMMKeyInfo[j].eDescMode;
#endif
                            u1KeyIdx = _arDmxMMKeyInfo[j].u1KeyIdx;
                        }
                    }

                    if(fgDecrypt)
                    {
                        if((ENUM_SWDMX_STRM_TYPE_T)_rDecInfo.u2DecType == eSWDMX_STRM_TYPE_VID)
                        {
                            SWDMX_SetInfo((UINT8)eSWDMX_SET_VIDEO_DECRYPT, (UINT32)u1KeyIdx,
                                (UINT32)eDescMode, 0);
                        }
                        else if((ENUM_SWDMX_STRM_TYPE_T)_rDecInfo.u2DecType == eSWDMX_STRM_TYPE_AUD)
                        {
                            SWDMX_SetInfo((UINT8)eSWDMX_SET_AUDIO_DECRYPT, (UINT32)u1KeyIdx,
                                (UINT32)eDescMode, 0);
                        }
                    }
                }
#endif

                SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM,
                    (UINT32)_rDecInfo.u2DecType, _rDecInfo.u2CompId, 0);
                _rDecInfo.fgEnable = TRUE;

                if ((_rDecInfo.u2CompId > 0) &&
                    (_rDecInfo.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
                {
                    _fgVideoExist = TRUE;
                    _u2VideoCompId = _rDecInfo.u2CompId;
                    fgVideo = TRUE;
                }
            }
            else if ((!_rDecInfo1.fgEnable) &&
                (_rDecInfo1.u2CompId == pt_comp_id->ui2_id))
            {
                SWDMX_SetStrmID(_rDecInfo1.u4StrmId, _rDecInfo1.u4SubStrmId, (ENUM_SWDMX_STRM_TYPE_T)_rDecInfo1.u2DecType);

#ifdef TIME_SHIFT_SUPPORT
                if((_rContainerType == MEDIA_TYPE_TS) &&
                   ((_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT) ||
                    (_rTsSubType == MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION)))
                {
                    for(j=0; j<DMX_NUM_PID_INDEX; j++)
                    {
                        if(_arDmxMMKeyInfo[j].fgValid && (_rDecInfo1.u4StrmId == _arDmxMMKeyInfo[j].u2StreamID))
                        {
                            fgDecrypt = TRUE;
#ifdef CC_MT5387
                            eDescMode = DMX_DESC_MODE_AES_ECB;
#else
                            eDescMode = _arDmxMMKeyInfo[j].eDescMode;
#endif
                            u1KeyIdx = _arDmxMMKeyInfo[j].u1KeyIdx;
                        }
                    }

                    if(fgDecrypt)
                    {
                        if((ENUM_SWDMX_STRM_TYPE_T)_rDecInfo1.u2DecType == eSWDMX_STRM_TYPE_VID)
                        {
                            SWDMX_SetInfo((UINT8)eSWDMX_SET_VIDEO_DECRYPT, (UINT32)u1KeyIdx,
                                (UINT32)eDescMode, 0);
                        }
                        else if((ENUM_SWDMX_STRM_TYPE_T)_rDecInfo1.u2DecType == eSWDMX_STRM_TYPE_AUD)
                        {
                            SWDMX_SetInfo((UINT8)eSWDMX_SET_AUDIO_DECRYPT, (UINT32)u1KeyIdx,
                                (UINT32)eDescMode, 0);
                        }
                    }
                }
#endif

                SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM,
                    (UINT32)_rDecInfo1.u2DecType, _rDecInfo.u2CompId, 0);
                _rDecInfo1.fgEnable = TRUE;

                if ((_rDecInfo1.u2CompId > 0) &&
                    (_rDecInfo1.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
                {
                    _fgVideoExist = TRUE;
                    _u2VideoCompId = _rDecInfo1.u2CompId;
                    fgVideo = TRUE;
                }
            }
            else if ((!_rDecInfo2.fgEnable) &&
                (_rDecInfo2.u2CompId == pt_comp_id->ui2_id))
            {
                SWDMX_SetStrmID(_rDecInfo2.u4StrmId,
                    _rDecInfo2.u4SubStrmId,
                    (ENUM_SWDMX_STRM_TYPE_T)_rDecInfo2.u2DecType);

#ifdef TIME_SHIFT_SUPPORT
                if((_rContainerType == MEDIA_TYPE_TS) &&
                   ((_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT) ||
                    (_rTsSubType == MEDIA_TS_SUBTYPE_TS_192_ENCRYPTION)))
                {
                    for(j=0; j<DMX_NUM_PID_INDEX; j++)
                    {
                        if(_arDmxMMKeyInfo[j].fgValid && (_rDecInfo2.u4StrmId == _arDmxMMKeyInfo[j].u2StreamID))
                        {
                            fgDecrypt = TRUE;
#ifdef CC_MT5387
                            eDescMode = DMX_DESC_MODE_AES_ECB;
#else
                            eDescMode = _arDmxMMKeyInfo[j].eDescMode;
#endif
                            u1KeyIdx = _arDmxMMKeyInfo[j].u1KeyIdx;
                        }
                    }

                    if(fgDecrypt)
                    {
                        if((ENUM_SWDMX_STRM_TYPE_T)_rDecInfo2.u2DecType == eSWDMX_STRM_TYPE_VID)
                        {
                            SWDMX_SetInfo((UINT8)eSWDMX_SET_VIDEO_DECRYPT, (UINT32)u1KeyIdx,
                                (UINT32)eDescMode, 0);
                        }
                        else if((ENUM_SWDMX_STRM_TYPE_T)_rDecInfo2.u2DecType == eSWDMX_STRM_TYPE_AUD)
                        {
                            SWDMX_SetInfo((UINT8)eSWDMX_SET_AUDIO_DECRYPT, (UINT32)u1KeyIdx,
                                (UINT32)eDescMode, 0);
                        }
                    }
                }
#endif

                SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM,
                    (UINT32)_rDecInfo2.u2DecType, _rDecInfo.u2CompId, 0);
                _rDecInfo2.fgEnable = TRUE;

                if ((_rDecInfo2.u2CompId > 0) &&
                    (_rDecInfo2.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
                {
                    _fgVideoExist = TRUE;
                    _u2VideoCompId = _rDecInfo2.u2CompId;
                    fgVideo = TRUE;
                }
            }
            else
            {
                LOG(3, "all enable or unknown u2CompId %d\n", pt_comp_id->ui2_id);
            }
            if(fgVideo)
            {
                // there is a case video stream will change
                // but we want to keep the speed
                // So av sync mode would not be set again.
                // we have to set sync mode to video
                // or video drive would not know the current sync mode is.
                // refer to dmx_pcr.c
                if(_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_1X)
                {
                    if((_rContainerType == MEDIA_TYPE_TS) &&
                       (_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
                    {
                        // system master
                        VDEC_SyncStc(0, VID_SYNC_MODE_SLAVE_MM, 0);
                    }
                    else if(_bIsThumbnailPb)
                    {
                        VDEC_SyncStc(0, VID_SYNC_MODE_NONE, 0);
                    }
                    else
                    {
                        // audio master
                        VDEC_SyncStc(0, VID_SYNC_MODE_SLAVE_MM, 0);
                    }
                }
                else if(_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_2X)
                {
                    // video master
                    VDEC_SyncStc(0, VID_SYNC_MODE_MASTER, 0);
                }
                else
                {
                    // audio master
                    VDEC_SyncStc(0, VID_SYNC_MODE_SLAVE_MM, 0);
                }
            }
            /*
#ifdef TIME_SHIFT_SUPPORT
            if(!_MwDmxSetDecrypt())
            {
                LOG(3, "%s _MwDmxSetDecrypt fail\n", __FUNCTION__);
            }
#endif  // TIME_SHIFT_SUPPORT
            */
        }
        else
        {
            if(_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_1X)
            {
                if((_rContainerType == MEDIA_TYPE_TS) &&
                   (_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
                {
                    STC_SetSyncMode(AV_SYNC_MODE_SYSTEM_MASTER);
                }
                else if(_bIsThumbnailPb)
                {
                    STC_SetSyncMode(AV_SYNC_MODE_NONE);
                }
                else
                {
                    STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
                }
            }
            else if(_u4PreviousSpeed == MM_SPEED_TYPE_FORWARD_2X)
            {
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
                if((_rContainerType == MEDIA_TYPE_TS) &&
                   (_rTsSubType == MEDIA_TS_SUBTYPE_TS_TIME_SHIFT))
                {
                    STC_SetSyncMode(AV_SYNC_MODE_NONE);
                }
                else
#endif
                {
                    STC_SetSyncMode(AV_SYNC_MODE_VIDEO_MASTER);
                }
            }
            else
            {
            #if 0
                STC_SetSyncMode(AV_SYNC_MODE_AUDIO_MASTER);
                // if we set none, vdec will to underflow check
                // prVdecEsInfo->u2UdfNs = MPV_UDF_PES_NS
                // so use audio master
            #endif
                STC_SetSyncMode(AV_SYNC_MODE_NONE);
            }

            _u4PreviousSpeed = MM_SPEED_TYPE_FORWARD_1X;
            //SRM_SetMmMode(SRM_MM_MODE_TYPE_VIDEO);
            if(_rContainerType == MEDIA_TYPE_TS)
            {
                _MW_DMX_SetCacheOn(FALSE);
                if(!_MW_DMX_FlushAllCachePid())
                {
                    LOG(3, "_MwDmxOnMMSetControl _MW_DMX_FlushAllCachePid fail\n");
                }
            }
            SWDMX_Play();
        }
     }
   else
   {
       if(pt_comp_id->e_type == DRVT_DEMUX_CONTAINER_ES)
       {
            BOOL fgVideo = FALSE;
            // TS file with program change
            // We have to stop vdp to let MPV_FlushEsm finish
            if ((_rDecInfo.u2CompId > 0) &&
                (_rDecInfo.u2CompId == pt_comp_id->ui2_id) &&
                (_rDecInfo.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
            {
                fgVideo = TRUE;
            }
            else if ((_rDecInfo1.u2CompId > 0) &&
                (_rDecInfo1.u2CompId == pt_comp_id->ui2_id) &&
                (_rDecInfo1.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
            {
                fgVideo = TRUE;
            }
            else if ((_rDecInfo2.u2CompId > 0) &&
                (_rDecInfo2.u2CompId == pt_comp_id->ui2_id) &&
                (_rDecInfo2.u2DecType == (UINT16)eSWDMX_STRM_TYPE_VID))
            {
                fgVideo = TRUE;
            }

            if((_fgVideoExist)
               &&
               (fgVideo)
               &&
               ((_u4PreviousSpeed == MM_SPEED_TYPE_STEP)
                ||
                ((_u4PreviousSpeed >= MM_SPEED_TYPE_FORWARD_1_DIV_32X)
                 &&
                 (_u4PreviousSpeed <= MM_SPEED_TYPE_FORWARD_1_DIV_2X))
               )
              )
            {
                VDP_SetPauseMm(B2R_1, FALSE);
                // Reset Step Forward status
                if(VDP_GetStepForward(VDP_1) != 0)
                {
                    VDP_SetFBNoTimeout(VDP_1, FALSE);
                    VDP_SetStepForward(VDP_1, VDP_PLAY_NORMAL);
                }
                // Reset Slow Forward status
                if(VDP_GetSlowForward(VDP_1) != 0)
                {
                    VDP_SetFBNoTimeout(VDP_1, FALSE);
                    VDP_SetSlowForward(VDP_1, VDP_PLAY_NORMAL);
                }
                VDP_SetSpeed(0, STC_SPEED_TYPE_FORWARD_1X);

                // Reset trick mode status
                VDP_SetTrickMode(VDP_PLAY_NORMAL);
            }

            if (_rDecInfo.fgEnable &&
                (_rDecInfo.u2CompId == pt_comp_id->ui2_id))
            {
                SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)_rDecInfo.u2DecType, 0, 0);
                _rDecInfo.fgEnable = FALSE;
            }
            else if (_rDecInfo1.fgEnable &&
                (_rDecInfo1.u2CompId == pt_comp_id->ui2_id))
            {
                SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)_rDecInfo1.u2DecType, 0, 0);
                _rDecInfo1.fgEnable = FALSE;
            }
            else if (_rDecInfo2.fgEnable &&
                (_rDecInfo2.u2CompId == pt_comp_id->ui2_id))
            {
                SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)_rDecInfo2.u2DecType, 0, 0);
                _rDecInfo2.fgEnable = FALSE;
            }
            else
            {
                LOG(3, "disable or unknown u2CompId %d\n", pt_comp_id->ui2_id);
            }
       }
       else
       {
            _MwDmxFlushCtrl();
            SWDMX_Stop();
            _MwDmxWaitCtrlDone();
            SWDMX_Flush();

            if(_rContainerType == MEDIA_TYPE_TS)
            {
                _MW_DMX_SetCacheOn(TRUE);
            }

            //if(!_fgThumbnailMode)
            {
            //    SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);
            }

            _bFirstRange = TRUE;
            _fgAudNotSupport = FALSE;
            _u4PreviousSpeed = (UINT32)0xFFFFFFFF;

            _bPreviousStateIsPause = FALSE;
            _fgPreviousStateIsTrick = FALSE;
            _fgPreviousStateIsSeek = FALSE;
            /*
            _rDecInfo.fgEnable = FALSE;
            _rDecInfo1.fgEnable = FALSE;
            _rDecInfo2.fgEnable = FALSE;
            */
       }
   }

    return RMR_OK;
}

//-----------------------------------------------------------------------------
/** _MwDmxOnSetDestination
 *  Event handler of setting destination ID
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnMMSetDestination(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    DEMUX_DEST_INFO_T* ptDestInfo;
    INT32 i4Ret = RMR_OK;
    UINT32 eTypeAud;

    ptDestInfo = (DEMUX_DEST_INFO_T*)pv_set_info;

    if (ptDestInfo == NULL)
    {
        return i4Ret;
    }

    switch (ptDestInfo->e_type)
    {
    case DRVT_VID_DEC:
        if (_rDecInfo.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo.u2DecType = (UINT16)eSWDMX_STRM_TYPE_VID;
        }
        else if (_rDecInfo1.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo1.u2DecType = (UINT16)eSWDMX_STRM_TYPE_VID;
        }
        else if (_rDecInfo2.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo2.u2DecType = (UINT16)eSWDMX_STRM_TYPE_VID;
        }
        else
        {
            LOG(3, "unknown u2CompId %d\n", pt_comp_id->ui2_id);
        }
        break;
    case DRVT_AUD_DEC:
        if(ptDestInfo->ui2_id == AUD_DEC_MAIN)
        {
            eTypeAud = eSWDMX_STRM_TYPE_AUD;
        }
        else
        {
            eTypeAud = eSWDMX_STRM_TYPE_AUD2;
        }
        if (_rDecInfo.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo.u2DecType = (UINT16)eTypeAud;
        }
        else if (_rDecInfo1.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo1.u2DecType = (UINT16)eTypeAud;
        }
        else if (_rDecInfo2.u2CompId == pt_comp_id->ui2_id)
        {
           _rDecInfo2.u2DecType = (UINT16)eTypeAud;
        }
        else
        {
            LOG(3, "unknown u2CompId %d\n", pt_comp_id->ui2_id);
        }
        _fgAudNotSupport = FALSE;//DTV00216086
        break;
    default:
        LOG(3, "unknown e_type %d", ptDestInfo->e_type);
        i4Ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    UNUSED(pt_comp_id);

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _MwDmxOnMMSetPSIBlockMode
 *  Event handler of setting TS blocking mode with PSI buffer check
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnMMSetPSIBlockMode(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    UINT32 u4Enable;
    u4Enable = (UINT32)pv_set_info;

    if(u4Enable)
    {
        // to check PSI buffer size, only send data if PSI buffer is not full
        SWDMX_SetInfo((UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE, (UINT32)TRUE, 0, 0);
    }
    else
    {
        // not to check PSI buffer size, send data only blocked by A/V buffer
        SWDMX_SetInfo((UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE, (UINT32)FALSE, 0, 0);
    }
    return RMR_OK;
}

#ifdef TIME_SHIFT_SUPPORT
//-----------------------------------------------------------------------------
/** _MwDmxOnMMSetRecBufRange
 *  Event handler of setting TS time shift record buffer range
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnMMSetRecBufRange(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    SWDMX_SetInfo((UINT8)eSWDMX_SET_REC_BUF_RANGE, (UINT32)pv_set_info, 0, 0);
    UNUSED(pt_comp_id);
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnMMSetMonitorLBA
 *  Event handler of setting TS time shift monitor LBA
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *  @param  u1Pidx          PID index
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnMMSetMonitorLBA(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    SWDMX_SetInfo((UINT8)eSWDMX_SET_MONITOF_LBA, (UINT32)pv_set_info, 0, 0);
    UNUSED(pt_comp_id);
    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxMMLock
 *  Enter demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MwDmxMMLock(void)
{
    _rDmxLock = x_crit_start();

    ASSERT(!_fgDmxLocking);
    _fgDmxLocking = TRUE;

    UNUSED(_fgDmxLocking);          // Make Lint happy
}


//-----------------------------------------------------------------------------
/** _MwDmxMMUnlock
 *  Leave demux critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _MwDmxMMUnlock(void)
{
    ASSERT(_fgDmxLocking);
    _fgDmxLocking = FALSE;

    x_crit_end(_rDmxLock);

    UNUSED(_fgDmxLocking);          // Make Lint happy
}

//-----------------------------------------------------------------------------
/** _MwDmxOnMMScrambleStateNotify
 *  Scramble state notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnMMScrambleStateNotify(UINT8 u1Pidx, UINT32 u4Data,
    const void* pvTag)
{
    DEMUX_NFY_INFO_T rInfo;
    DMX_SCRAMBLE_STATE_T eState;
    DEMUX_SCRS_T eMwState;
    BOOL fgRet;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _MwDmxMMLock();
    rInfo = _arScrambleState[u1Pidx];
    _MwDmxMMUnlock();

    if (rInfo.pf_demux_nfy == NULL)
    {
        LOG(3, "Pidx %u: No scrabmle state handler registered\n", u1Pidx);
        return FALSE;
    }

    eState = (DMX_SCRAMBLE_STATE_T)u4Data;
    switch (eState)
    {
    case DMX_SCRAMBLE_STATE_CLEAR:
        eMwState = DEMUX_SCRS_CLEAR;
        break;

    case DMX_SCRAMBLE_STATE_SCRAMBLED:
        eMwState = DEMUX_SCRS_SCRAMBLED;
        break;

    default:
        eMwState = DEMUX_SCRS_UNKNOWN;
        break;
    }

#ifndef LINUX_TURNKEY_SOLUTION
    fgRet = rInfo.pf_demux_nfy(
        rInfo.pv_tag, DEMUX_COND_SCRS, (UINT32)eMwState, 0
    #ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
    #endif
        );
#else
    fgRet = _MwDmx_set_dmx_nfy(u1Pidx,
        rInfo.pv_tag, DEMUX_COND_SCRS, (UINT32)eMwState, 0
    #ifndef MW_CLASSIC_DMX_NFY
        , 0, 0, 0
    #endif
        );
#endif

    // The middleware guarantees that it will always return FALSE
    VERIFY(!fgRet);

    UNUSED(pvTag);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _MwDmxOnMMScrambleStateHandler
 *  Scramble-state handler
 *
 *  @param  u1Pidx          PID index
 *  @param  eCode           Notification code
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxOnMMScrambleStateHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = FALSE;

    if(u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    switch (eCode)
    {
    case DMX_NOTIFY_CODE_SCRAMBLE_STATE:
        fgRet = _MwDmxOnMMScrambleStateNotify(u1Pidx, u4Data, pvTag);
        break;

    default:
        // Unknown notification code
        LOG(1, "Pidx %u: Unknown notification code %u\n", u1Pidx,
            (UINT32)eCode);
        ASSERT(0);
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _MwDmxOnMMSetScrambleCallback
 *  Set scramble notify callback
 *
 *  @param  pt_comp_id      Component ID
 *  @param  pv_set_info     Set info
 *
 *  @retval RMR_OK if succeeded, failed otherwise
 */
//-----------------------------------------------------------------------------
static INT32 _MwDmxOnMMSetScrambleCallback(DRV_COMP_ID_T* pt_comp_id,
    const VOID* pv_set_info)
{
    INT8 u1Pidx;
    UINT16 u2DecType;
    DMX_PID_T rPid;
    DEMUX_NFY_INFO_T* prNfyInfo;

    if (_rDecInfo.u2CompId == pt_comp_id->ui2_id)
    {
        u2DecType = _rDecInfo.u2DecType;
    }
    else if (_rDecInfo1.u2CompId == pt_comp_id->ui2_id)
    {
        u2DecType = _rDecInfo1.u2DecType;
    }
    else if (_rDecInfo2.u2CompId == pt_comp_id->ui2_id)
    {
        u2DecType = _rDecInfo2.u2DecType;
    }
    else
    {
        LOG(3, "unknown u2CompId %d\n", pt_comp_id->ui2_id);
        return RMR_DRV_INV_SET_INFO;
    }

    if ((UINT16)eSWDMX_STRM_TYPE_VID == u2DecType)
    {
        u1Pidx = (UINT8)(DMX_PES_COMP_ID_START);
    }
    else if ((UINT16)eSWDMX_STRM_TYPE_AUD == u2DecType)
    {
        u1Pidx = (UINT8)(DMX_PES_COMP_ID_START + 1);
    }
    else if ((UINT16)eSWDMX_STRM_TYPE_AUD2 == u2DecType)
    {
        u1Pidx = (UINT8)(DMX_PES_COMP_ID_START + 2);
    }
    else
    {
        LOG(3, "unknown u2DecType %d\n", (UINT32)u2DecType);
        return RMR_DRV_INV_SET_INFO;
    }

    // Set scramble state callback function

    if ((u1Pidx >= DMX_NUM_PID_INDEX) || (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    prNfyInfo = (DEMUX_NFY_INFO_T*)pv_set_info;

    // Set PSI callback function to our handler
    x_memset(&rPid, 0, sizeof(rPid));
    rPid.pfnScramble = _MwDmxOnMMScrambleStateHandler;
    rPid.pvScrambleTag = NULL;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_SCRAMBLE_STATE, &rPid))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    _MwDmxMMLock();
    _arScrambleState[u1Pidx].pf_demux_nfy = prNfyInfo->pf_demux_nfy;
    _arScrambleState[u1Pidx].pv_tag = prNfyInfo->pv_tag;
    _MwDmxMMUnlock();

    return RMR_OK;
}


//-----------------------------------------------------------------------------
/** _MwDmxMMResetScrambleCallback
 *  Reset scramble callback handler
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _MwDmxMMResetScrambleCallback(UINT8 u1Pidx)
{
    DMX_PID_T rPid;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    rPid.pfnScramble = NULL;
    rPid.pvScrambleTag = NULL;
    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_SCRAMBLE_STATE, &rPid))
    {
        return FALSE;
    }

    _MwDmxMMLock();
    _arScrambleState[u1Pidx].pf_demux_nfy = NULL;
    _arScrambleState[u1Pidx].pv_tag = NULL;
    _MwDmxMMUnlock();

    return TRUE;
}

#endif

 /*-----------------------------------------------------------------------------
 * Name:
 *
 * Description: Registration
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
// For subtitle usage
BOOL _MwDmxMM_SetSubtitleStrmID(UINT32 u4StrmID)
{
    return SWDMX_SetStrmID(u4StrmID, 0, eSWDMX_STRM_TYPE_SUBTITLE);
}


 /*-----------------------------------------------------------------------------
 * Name:
 *
 * Description: Registration
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
BOOL _MwDmxMM_EnableSubtitle(void)
{

    return SWDMX_SetInfo(eSWDMX_ENABLE_STREAM, eSWDMX_STRM_TYPE_SUBTITLE, 0, 0);
}


 /*-----------------------------------------------------------------------------
 * Name:
 *
 * Description: Registration
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
BOOL _MwDmxMM_DisableSubtitle(void)
{

    return SWDMX_SetInfo(eSWDMX_DISABLE_STREAM, eSWDMX_STRM_TYPE_SUBTITLE, 0, 0);
}

#ifdef TIME_SHIFT_SUPPORT
/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description: Set description infomation
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxOnMMSetDescryptInfo(const VOID* pv_set_info)
{
    MM_DESC_CRYPT_INFO_T *prDesInfo;
    MM_CRYPT_INFO_T *ptEncryptInfo;
    DMX_DESC_MODE_T eMode;
    UINT32 au4EvenKey[6], au4OddKey[6];
    UINT8 i, u1KeyIdx;
    BOOL fgMatch;
    UINT8 u1KeyOffset = 0;

    prDesInfo = (MM_DESC_CRYPT_INFO_T*)pv_set_info;
    if(prDesInfo == NULL)
    {
        return RMR_INV_ARG;
    }

    ptEncryptInfo = (MM_CRYPT_INFO_T*)&(prDesInfo->t_crypto_info);
    if(ptEncryptInfo == NULL)
    {
        return RMR_INV_ARG;
    }

    if((ptEncryptInfo->e_mode == MM_CRYPT_MODE_DES_CBC) ||
       (ptEncryptInfo->e_mode == MM_CRYPT_MODE_3_DES_CBC))
    {
        LOG(3, "%s,%d, not support CBC yet\n", __FUNCTION__, __LINE__);
        return RMR_INV_ARG;
    }

    //--------------------------------------------------
    // Search available key index
    //--------------------------------------------------
    if(!_MwPVRAllocatePlayKeyIdx(ptEncryptInfo, &fgMatch, &u1KeyIdx))
    {
        return RMR_INV_ARG;
    }

    u1KeyOffset = FVR_SCRAMBLE_PID_INDEX;

    x_memset((void*)au4OddKey, 0, sizeof(au4OddKey));
    x_memset((void*)au4EvenKey, 0, sizeof(au4EvenKey));

    au4OddKey[0] = prDesInfo->t_crypto_info.aui4_odd_key_1[0];
    au4OddKey[1] = prDesInfo->t_crypto_info.aui4_odd_key_1[1];
    au4EvenKey[0] = prDesInfo->t_crypto_info.aui4_even_key_1[0];
    au4EvenKey[1] = prDesInfo->t_crypto_info.aui4_even_key_1[1];

    if(ptEncryptInfo->e_mode == MM_CRYPT_MODE_3_DES_EBC)
    {
        au4OddKey[2] = prDesInfo->t_crypto_info.aui4_odd_key_2[0];
        au4OddKey[3] = prDesInfo->t_crypto_info.aui4_odd_key_2[1];
        au4EvenKey[2] = prDesInfo->t_crypto_info.aui4_even_key_2[0];
        au4EvenKey[3] = prDesInfo->t_crypto_info.aui4_even_key_2[1];
    }

    if(!DMX_SetAesKey(u1KeyIdx + u1KeyOffset, FALSE, au4OddKey))
    {
        return RMR_INV_ARG;
    }

    if(!DMX_SetAesKey(u1KeyIdx + u1KeyOffset, TRUE, au4EvenKey))
    {
        return RMR_INV_ARG;
    }

    // Backup key
    switch(prDesInfo->t_crypto_info.e_mode)
    {
    case MM_CRYPT_MODE_DES_EBC:
        eMode = DMX_DESC_MODE_DES_ECB;
        break;

    case MM_CRYPT_MODE_DES_CBC:
        eMode = DMX_DESC_MODE_DES_CBC;
        break;

    case MM_CRYPT_MODE_3_DES_EBC:
        eMode = DMX_DESC_MODE_3DES_ECB;
        break;

    case MM_CRYPT_MODE_3_DES_CBC:
        eMode = DMX_DESC_MODE_3DES_CBC;
        break;

    default:
        return RMR_INV_ARG;
    }

    // Backward compatible for key storage
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(_arDmxMMKeyInfo[i].fgValid &&
          (prDesInfo->t_stream_setting.u.t_stm_ts_id == _arDmxMMKeyInfo[i].u2StreamID))
        {
            if((_arDmxMMKeyInfo[i].u1KeyIdx == u1KeyIdx) && (_arDmxMMKeyInfo[i].eDescMode == eMode))
            {
                return RMR_OK;
            }
            else
            {
                LOG(1, "%s, Key index not match, %d, %%d\n" __FUNCTION__,
                                               _arDmxMMKeyInfo[i].u1KeyIdx, u1KeyIdx);
                return RMR_INV_ARG;
            }
        }
    }

    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(!_arDmxMMKeyInfo[i].fgValid)
        {
            _arDmxMMKeyInfo[i].fgValid = TRUE;
            _arDmxMMKeyInfo[i].u1KeyIdx = u1KeyIdx;
            _arDmxMMKeyInfo[i].u2StreamID = prDesInfo->t_stream_setting.u.t_stm_ts_id;
            _arDmxMMKeyInfo[i].eDescMode = eMode;
            break;
        }
    }

    if(i == DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s, No valid info array\n", __FUNCTION__);
        return RMR_INV_ARG;
    }

    if(!fgMatch)
    {
        if(!_MwPVRSavePlayKey(ptEncryptInfo, u1KeyIdx))
        {
            return RMR_INV_ARG;
        }
    }

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 * Description: Run crypt command
 * Inputs:
 * Outputs:
 * Returns:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxOnMMSetCryptCmd(const VOID* pv_set_info)
{
    MM_CRYPT_CMD_T *prCryptCmd;
    INT32 i4Ret = RMR_OK;
    UINT8 i;

    prCryptCmd = (MM_CRYPT_CMD_T*)pv_set_info;
    if(prCryptCmd == NULL)
    {
        return RMR_INV_ARG;
    }

    switch(prCryptCmd->t_crypt_ctrl_cmd)
    {
    case MM_CRYPT_CTRL_CMD_RESET_ALL:
        if(!_MwPVRResetPlayKeyInfo())
        {
            i4Ret = RMR_INV_ARG;
        }

        for(i=0; i<DMX_NUM_PID_INDEX; i++)
        {
            _arDmxMMKeyInfo[i].fgValid = FALSE;
            _arDmxMMKeyInfo[i].u1KeyIdx = 0xFF;
        }
        break;
    }

    return i4Ret;
}
#endif  // TIME_SHIFT_SUPPORT

/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Notes: record and set something on hardware(src, dest, type...)
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxMM_Connect(
              const DRV_COMP_ID_T*    pt_comp_id,
              DRV_CONN_TYPE_T         e_conn_type,
              const VOID*             pv_conn_info,
              SIZE_T                  z_conn_info_len,
              VOID*                   pv_tag,
              x_rm_nfy_fct            pf_nfy)
{
    DRV_COMP_ID_T* prCompReg;
    SWDMX_NFY_INFO_T rDmxNfyInfo;

    if ((NULL == pt_comp_id) || (NULL == pv_conn_info) )
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    // 1. Check para valid
    if(pt_comp_id->e_type == DRVT_DEMUX_CONTAINER_ES)
    {
        if (_rDecInfo.u2CompId == pt_comp_id->ui2_id)
        {
            LOG(0, "pt_comp_id %d already connected 0\n", (UINT32)pt_comp_id->ui2_id);
            return (RMR_DRV_INV_CONN_INFO);
        }
        else if (_rDecInfo1.u2CompId == pt_comp_id->ui2_id)
        {
            LOG(0, "pt_comp_id %d already connected 1\n", (UINT32)pt_comp_id->ui2_id);
            return (RMR_DRV_INV_CONN_INFO);
        }
        else if (_rDecInfo2.u2CompId == pt_comp_id->ui2_id)
        {
            LOG(0, "pt_comp_id %d already connected 2\n", (UINT32)pt_comp_id->ui2_id);
            return (RMR_DRV_INV_CONN_INFO);
        }
        else
        {
            return RMR_OK;
        }
    }

    // 1.2 check id range
    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    prCompReg = (DRV_COMP_ID_T*)pv_conn_info;
    buf_agt_dmx_connect((BAGT_COMP_ID_T)prCompReg->ui2_id);

    x_memset(&rDmxNfyInfo, 0, sizeof(rDmxNfyInfo));
    rDmxNfyInfo.pfSwdmxNfy = _MwDmxNtyFct;
    rDmxNfyInfo.pvTag = 0;
    _MwDmxOnSetNfyFct(&rDmxNfyInfo, sizeof(rDmxNfyInfo));

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  just record, since it's hardwire
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxMM_Disconnect(
              const DRV_COMP_ID_T*    pt_comp_id,
              DRV_DISC_TYPE_T         e_disc_type,
              const VOID*             pv_disc_info,
              SIZE_T                  z_disc_info_len)
{
    INT8 u1Pidx;
    UINT16 u2DecType;
    DRV_COMP_ID_T* prCompReg;
    SWDMX_NFY_INFO_T rDmxNfyInfo;

    // 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_disc_info) )
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if(pt_comp_id->e_type == DRVT_DEMUX_CONTAINER_ES)
    {
        if(_u2VideoCompId == pt_comp_id->ui2_id)
        {
            _fgVideoExist = FALSE;
            _u2VideoCompId = 0;
        }
        if (_rDecInfo.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo.u2CompId = 0;
            u2DecType = _rDecInfo.u2DecType;
        }
        else if (_rDecInfo1.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo1.u2CompId = 0;
            u2DecType = _rDecInfo1.u2DecType;
        }
        else if (_rDecInfo2.u2CompId == pt_comp_id->ui2_id)
        {
            _rDecInfo2.u2CompId = 0;
            u2DecType = _rDecInfo2.u2DecType;
        }
        else
        {
            LOG(0, "pt_comp_id %d doesn't connected\n", (UINT32)pt_comp_id->ui2_id);
            return (RMR_DRV_INV_CONN_INFO);
        }

#ifdef TIME_SHIFT_SUPPORT
        u1Pidx = 0xFF;
        if ((UINT16)eSWDMX_STRM_TYPE_VID == u2DecType)
        {
            u1Pidx = (UINT8)(DMX_PES_COMP_ID_START);
        }
        else if ((UINT16)eSWDMX_STRM_TYPE_AUD == u2DecType)
        {
            u1Pidx = (UINT8)(DMX_PES_COMP_ID_START + 1);
        }
        else if ((UINT16)eSWDMX_STRM_TYPE_AUD2 == u2DecType)
        {
            u1Pidx = (UINT8)(DMX_PES_COMP_ID_START + 2);
        }

        if(u1Pidx != 0xFF)
        {
            // Reset scramble state callback
            if (!_MwDmxMMResetScrambleCallback(u1Pidx))
            {
                LOG(0, "_MwDmxMMResetScrambleCallback fail u1Pidx(%d)\n", (UINT32)u1Pidx);
                return RMR_DRV_DISC_FAILED;
            }
        }
#endif

        UNUSED(u1Pidx);
        UNUSED(u2DecType);
        return RMR_OK;
    }
	else                  
	{

	    SWDMX_DelRange(NULL, 0, TRUE);	//DTV00148327 before thumb nail stop, we should delete all the range info

	}

    prCompReg = (DRV_COMP_ID_T*)pv_disc_info;
	buf_agt_dmx_disconnect((BAGT_COMP_ID_T)prCompReg->ui2_id);
#ifdef CC_FLASH_TEST
    _fgEsPtsConnect = FALSE;
#endif

    x_memset(&rDmxNfyInfo, 0, sizeof(rDmxNfyInfo));
    _MwDmxOnSetNfyFct(&rDmxNfyInfo, sizeof(rDmxNfyInfo));

    UNUSED(e_disc_type);
    UNUSED(z_disc_info_len);

#ifdef LINUX_TURNKEY_SOLUTION
    while (g_pt_range_list)
    {
        if (g_pt_range_list->pt_range)
        {
            _MwDmx_remove_range_list(g_pt_range_list->pt_range->ui4_range_id);
        }
        else
        {
            break;
        }
    }
#endif

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxMM_Set(
          const DRV_COMP_ID_T*    pt_comp_id,
          DRV_SET_TYPE_T        e_set_type,
          const VOID*         pv_set_info,
          SIZE_T            z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    // Check parameters
    if ((NULL == pt_comp_id))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    // Check component type
    // ...


    switch (e_set_type)
    {
        case DEMUX_SET_TYPE_DEST_ID:
          i4Ret = _MwDmxOnMMSetDestination((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_CTRL:
          i4Ret = _MwDmxOnMMSetControl((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_STREAM_ID:
          i4Ret = _MwDmxOnMMSetStreamID((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_MEDIA_FORMAT:
          i4Ret = _MwDmxOnMMSetMediaFormat((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_ADD_RANGE:
          i4Ret = _MwDmxOnMMAddRange((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_DEL_RANGE:
          i4Ret = _MwDmxOnMMDelRange((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_SPEED:
          i4Ret = _MwDmxOnMMSetSpeed((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_SEEK_POSTION:
          i4Ret = _MwDmxOnMMSetSeekPos((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_BLOCK_PSI:
          i4Ret = _MwDmxOnMMSetPSIBlockMode((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_PRIORITY:
          i4Ret = _MwDmxOnMMSetPriority((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
#ifdef TIME_SHIFT_SUPPORT
        case DEMUX_SET_TYPE_DECRYPT_INFO:
          i4Ret = _MwDmxOnMMSetDescryptInfo(pv_set_info);
          break;
        case DEMUX_SET_TYPE_CRYPT_CMD:
          i4Ret = _MwDmxOnMMSetCryptCmd(pv_set_info);
          break;
        case DEMUX_SET_TYPE_MONITOR_LBA:
          i4Ret = _MwDmxOnMMSetMonitorLBA((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_REC_BUF_RANGE:
          i4Ret = _MwDmxOnMMSetRecBufRange((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
        case DEMUX_SET_TYPE_SCRS_NFY_FCT:
          i4Ret = _MwDmxOnMMSetScrambleCallback((DRV_COMP_ID_T*)pt_comp_id, pv_set_info);
          break;
#endif
        default:
          break;
    }

    return i4Ret;
}


/*-----------------------------------------------------------------------------
 * Name:
 *
 * Description:
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:
 ----------------------------------------------------------------------------*/
static INT32 _MwDmxMM_Get(
            const DRV_COMP_ID_T*    pt_comp_id,
            DRV_GET_TYPE_T        e_get_type,
            VOID*           pv_get_info,
            SIZE_T*           pz_get_info_len)
{
    BOOL fgRet;
    UINT32 u4Tmp;
    UINT32* pu4Info;
    UINT64 u8Tmp;
    DEMUX_POST_INFO_T* prDmxPosInfo;
    DEMUX_UOP_CAP_INFO_T* prDemuxUopCapInfo;

    // 1. Check para valid
    if ((NULL == pt_comp_id) || (NULL == pv_get_info) ||
        (NULL == pz_get_info_len))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    prDmxPosInfo = (DEMUX_POST_INFO_T*)pv_get_info;

    //get
    switch (e_get_type)
    {
        case DEMUX_GET_TYPE_CTRL:
          break;

        case DEMUX_GET_TYPE_BLOCK_PSI:
          fgRet = SWDMX_GetInfo((UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE, &u4Tmp, 0, 0);
          if(fgRet)
          {
              pu4Info = pv_get_info;
              *pu4Info = u4Tmp;
              return RMR_OK;
          }
          break;

        case DEMUX_GET_TYPE_CURR_LBA:
          fgRet = SWDMX_GetInfo((UINT8)eSWDMX_GET_CURRENT_LBA, &u4Tmp, 0, 0);
          if(fgRet)
          {
              pu4Info = pv_get_info;
              *pu4Info = u4Tmp;
              return RMR_OK;
          }
          break;

        case DEMUX_GET_TYPE_CURR_TICK_NUM:
          fgRet = SWDMX_GetInfo((UINT8)eSWDMX_GET_CURRENT_TICK, &u4Tmp, 0, 0);
          if(fgRet)
          {
              pu4Info = pv_get_info;
              *pu4Info = u4Tmp;
              return RMR_OK;
          }
          break;

        case DEMUX_GET_TYPE_UOP_CAPABILITY:

            prDemuxUopCapInfo = (DEMUX_UOP_CAP_INFO_T*)(pv_get_info);
            prDemuxUopCapInfo->b_is_allowed = TRUE;

            if(_rContainerType == MEDIA_TYPE_CONTAINER)
            {
                if( (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_AVI)||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MP4)||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_RM) ||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MATROSKA)||
                    (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_FLV))
                {
                    if(prDemuxUopCapInfo->e_uop == DEMUX_UOP_SEEK)
                    {
                        if((_rContainerSubType == MEDIA_CONTNR_SUBTYPE_ASF) ||
                            (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MATROSKA)||
                            (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_AVI)||
                            (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MP4)||
                            (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_RM)||
                            (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_FLV))
                        {
                           UINT32 fgSeek;
                           SWDMX_GetInfo(eSWDMX_GET_IS_TRICK,&fgSeek,0,0);
                           _fgKeyIsExist = (BOOL)fgSeek;
                          prDemuxUopCapInfo->b_is_allowed =_fgKeyIsExist;
                        }
                        else
                        {
                           prDemuxUopCapInfo->b_is_allowed =_fgKeyIsExist;
                        }

                        if((!_fgVideoExist) && 
                           (_rContainerSubType != MEDIA_CONTNR_SUBTYPE_FLV) &&
                           (_rContainerSubType != MEDIA_CONTNR_SUBTYPE_RM) )
                        {
                           prDemuxUopCapInfo->b_is_allowed = TRUE;
                        }
                    }
                    else if(prDemuxUopCapInfo->e_uop == DEMUX_UOP_TRICK)
                    {
                        if(prDemuxUopCapInfo->u.t_speed == MM_SPEED_TYPE_PAUSE ||
                            prDemuxUopCapInfo->u.t_speed == MM_SPEED_TYPE_FORWARD_1X )
                        {
                            prDemuxUopCapInfo->b_is_allowed = TRUE;
                        }
                        else
                        {
                            if((_rContainerSubType == MEDIA_CONTNR_SUBTYPE_ASF)
								||(_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MATROSKA)
								||(_rContainerSubType==MEDIA_CONTNR_SUBTYPE_FLV))
                            {
                               UINT32 fgTrick;
                               SWDMX_GetInfo(eSWDMX_GET_IS_TRICK,&fgTrick,0,0);
                               _fgKeyIsExist = (BOOL)fgTrick;
                               prDemuxUopCapInfo->b_is_allowed =_fgKeyIsExist;
                            }
                            else
							{
                               prDemuxUopCapInfo->b_is_allowed =_fgKeyIsExist;
                            }
                        }
                    }
                    else
                    {
                        return RMR_DRV_INV_GET_INFO;
                    }
                }
                else if((_rContainerSubType == MEDIA_CONTNR_SUBTYPE_ASF)||
                          (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_AVI)||
                          (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_MP4)||
                          (_rContainerSubType == MEDIA_CONTNR_SUBTYPE_FLV))
                {
                    UINT32 u4Trick =0;
                    UINT32 u4Seek =0;
                    SWDMX_GetInfo(eSWDMX_GET_IS_TRICK,&u4Trick,&u4Seek,0);
                    if(prDemuxUopCapInfo->e_uop == DEMUX_UOP_TRICK)
                    {
                        if(prDemuxUopCapInfo->u.t_speed == MM_SPEED_TYPE_PAUSE ||
                            prDemuxUopCapInfo->u.t_speed == MM_SPEED_TYPE_FORWARD_1X )
                        {
                            prDemuxUopCapInfo->b_is_allowed = TRUE;
                        }
                        else
                        {
                            prDemuxUopCapInfo->b_is_allowed = (BOOL)u4Trick;
                        }
                    }
                    else if(prDemuxUopCapInfo->e_uop == DEMUX_UOP_SEEK)
                    {
                        prDemuxUopCapInfo->b_is_allowed = (BOOL)u4Seek;
                    }
                    else
                    {
                        return RMR_DRV_INV_GET_INFO;
                    }
                }
                else
                {
                    prDemuxUopCapInfo->b_is_allowed = TRUE;
                }
            }
            else if((_rContainerType == MEDIA_TYPE_PS) ||
                    (_rContainerType == MEDIA_TYPE_TS) ||
                    (_rContainerType == MEDIA_TYPE_VIDEO_ES) ||
                    ((_rContainerType == MEDIA_TYPE_AUDIO_ES) &&
                     (_rAudSubType != MEDIA_AUD_SUBTYPE_WMA)))
            {
                if(prDemuxUopCapInfo->e_uop == DEMUX_UOP_TRICK)
                {
                    UINT32 u4Trick;
                    UINT32 u4Forward = 0;
                    if(prDemuxUopCapInfo->u.t_speed >= MM_SPEED_TYPE_FORWARD_1X)
                    {
                        u4Forward = 1;
                    }
                    SWDMX_GetInfo(eSWDMX_GET_IS_TRICK,&u4Trick,&u4Forward,0);
                    prDemuxUopCapInfo->b_is_allowed = (BOOL)u4Trick;
                }
                else
                {
                    prDemuxUopCapInfo->b_is_allowed = TRUE;
                }
            }
            return RMR_OK;

        case DEMUX_GET_TYPE_POST_INFO:
            u8Tmp = 0;
            ((DEMUX_POST_INFO_T*)pv_get_info)->ui8_pos_info = 0;
            fgRet = SWDMX_GetInfo((UINT8)eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8Tmp, 0, 0);
            if(fgRet)
            {
                ((DEMUX_POST_INFO_T*)pv_get_info)->ui8_pos_info = u8Tmp;
            }
            if (((DEMUX_POST_INFO_T*)pv_get_info)->e_type == DEMUX_POST_INFO_MEDIA_RENDER_OFST)
            {
                if (_fgVideoExist)
                {
                    prDmxPosInfo->u.render_info.ui8_pts_info = VDP_GetPts(B2R_1);

                    if ((_rContainerType == MEDIA_TYPE_PS) ||
                        (_rContainerType == MEDIA_TYPE_TS) ||
                        (_rContainerType == MEDIA_TYPE_VIDEO_ES))
                    {
                        UINT64 u8Offset;
                        UINT64 u8OffsetI;
                        UINT16 u2DecodingOrder;
                        INT32 i4TempRef;

                        u8Offset = 0;
                        u8OffsetI = 0;
                        u2DecodingOrder = 0;
                        i4TempRef = 0;

                        if(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
                        {
                            LOG(5, "Get DEMUX_POST_INFO_MEDIA_RENDER_OFST Fileoffset = %lld, IFileoffset = %lld\n",
                                u8Offset,
                                u8OffsetI);
                            LOG(5, "Get DEMUX_POST_INFO_MEDIA_RENDER_OFST u2DecodingOrder = %d, i4TempRef = %d\n",
                                u2DecodingOrder,
                                i4TempRef);

                            prDmxPosInfo->u.render_info.ui2_decoding_order = u2DecodingOrder;
                            prDmxPosInfo->u.render_info.i4_temporal_reference = i4TempRef;
                            prDmxPosInfo->u.render_info.ui8_i_frame_position = u8OffsetI;
                            prDmxPosInfo->u.render_info.ui8_frame_position = u8Offset;
                        }
                        else
                        {
                            LOG(3, "Fail to Get DEMUX_POST_INFO_MEDIA_RENDER_OFST\n");
                            return RMR_DRV_INV_GET_INFO;
                        }
                    }
                }
                else // audio only
                {
                    prDmxPosInfo->u.render_info.ui8_pts_info = (UINT64)STC_GetStcValue();

                    if ((_rContainerType == MEDIA_TYPE_PS) ||
                        (_rContainerType == MEDIA_TYPE_TS) ||
                        ((_rContainerType == MEDIA_TYPE_AUDIO_ES) &&
                         ((_rAudSubType == MEDIA_AUD_SUBTYPE_MP3) ||
                          (_rAudSubType == MEDIA_AUD_SUBTYPE_MP2) ||
                          (_rAudSubType == MEDIA_AUD_SUBTYPE_LPCM) ||
                          (_rAudSubType == MEDIA_AUD_SUBTYPE_WAVE) ||
                          (_rAudSubType == MEDIA_AUD_SUBTYPE_AAC) ||
                          (_rAudSubType == MEDIA_AUD_SUBTYPE_AIF) ||
                          (_rAudSubType == MEDIA_AUD_SUBTYPE_AC3)
                         )
                        )
                       )
                    {
                        prDmxPosInfo->u.render_info.ui2_decoding_order = 0;
                        prDmxPosInfo->u.render_info.i4_temporal_reference = 0;
                        prDmxPosInfo->u.render_info.ui8_i_frame_position = ((DEMUX_POST_INFO_T*)pv_get_info)->ui8_pos_info;
                        prDmxPosInfo->u.render_info.ui8_frame_position = ((DEMUX_POST_INFO_T*)pv_get_info)->ui8_pos_info;
                        LOG(5, "Get DEMUX_POST_INFO_MEDIA_RENDER_OFST Fileoffset = %lld, IFileoffset = %lld\n",
                            ((DEMUX_POST_INFO_T*)pv_get_info)->ui8_pos_info);
                    }
                }
                LOG(5, "Get DEMUX_POST_INFO_MEDIA_RENDER_OFST Render Pts = %lld\n", prDmxPosInfo->ui8_pos_info);
            }
            return RMR_OK;
        case DEMUX_GET_TYPE_STC_OFFSET:
            fgRet = SWDMX_GetInfo((UINT8)eSWDMX_GET_STC_OFFSET, &u4Tmp, 0, 0);
            if(fgRet)
            {
                pu4Info = pv_get_info;
                *pu4Info = u4Tmp;
                return RMR_OK;
            }
        default:
            break;
    }

    return RMR_DRV_INV_GET_INFO;
}


 /*-----------------------------------------------------------------------------
 * Name: _MW_DMX_MM_AudSetCtrl
 *
 * Description: Demux Multimedia part, hook _AudSetCtrl
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  #define DMX_SECTION_MEM_COMP_NUM, DMX_SECTION_MEM_COMP_ID_START
 ----------------------------------------------------------------------------*/
VOID _MW_DMX_MM_AudSetCtrl(UINT8 u1DecId, UINT32 u4AudCtrl)
{
    if ((u1DecId == AUD_DEC_MAIN) &&
        //(u4AudCtrl >= (UINT32)AUD_DEC_CTRL_RESET) &&
        (u4AudCtrl <= (UINT32)AUD_DEC_CTRL_DETECT_TV_AUD_SYS))
    {
        _fgAudCtrlDone = FALSE;
        _u4AudCtrl = u4AudCtrl;
        LOG(5, "_MW_DMX_MM_AudSetCtrl u4AudCtrl %d\n", u4AudCtrl);
    }
    else
    {
        LOG(3, "_MW_DMX_MM_AudSetCtrl unknown %d\n", u4AudCtrl);
    }
}

  /*-----------------------------------------------------------------------------
 * Name: _MW_DMX_MM_AudMwNfyFunc
 *
 * Description: Demux Multimedia part, hook _AudMwNfyFunc
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  #define DMX_SECTION_MEM_COMP_NUM, DMX_SECTION_MEM_COMP_ID_START
 ----------------------------------------------------------------------------*/
VOID _MW_DMX_MM_AudMwNfyFunc(void *pvNfyTag, UINT8 u1DecId,
                UINT32 u4NfyCond, UINT32 u4Data1, UINT32 u4Data2)
{
    UINT32 u4NfyTag = 0;

    u4NfyTag = (((UINT32)pvNfyTag) & 0xF);

    if((u4NfyTag == (UINT32)AUD_NFY_STREAM)
        && (u4NfyCond== (UINT32)AUD_COND_CTRL_DONE)
        && (!_fgAudCtrlDone))
    {
        _fgAudCtrlDone = TRUE;
        LOG(5, "_MW_DMX_MM_AudMwNfyFunc u4NfyCond done\n");
    }
    else if((u4NfyCond== (UINT32)AUD_COND_AUD_DECODE_STATUS_CHG) && u4Data1 == AUD_DECODE_NOT_SUPPORT)
    {
        _fgAudCtrlDone = TRUE;
        _fgAudNotSupport = TRUE;
        LOG(5, "_MW_DMX_MM_AudMwNfyFunc AUD_COND_AUD_DECODE_STATUS_CHG AUD_DECODE_NOT_SUPPORT\n");
    }
    else if((u4NfyCond== (UINT32)AUD_COND_AUD_DECODE_STATUS_CHG) && u4Data1 == AUD_DECODE_NO_DATA)
    {
        //SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
        LOG(5, "_MW_DMX_MM_AudMwNfyFunc AUD_COND_AUD_DECODE_STATUS_CHG AUD_DECODE_NO_DATA\n");
    }
    else
    {
        LOG(3, "_MW_DMX_MM_AudMwNfyFunc unknown u4NfyCond %d %d\n", u4NfyCond, u4Data1);
    }
}

 /*-----------------------------------------------------------------------------
 * Name:
 *
 * Description: Registration
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 * Note:  #define DMX_SECTION_MEM_COMP_NUM, DMX_SECTION_MEM_COMP_ID_START
 ----------------------------------------------------------------------------*/
BOOL _MW_DMX_MM_Init(void)
{
    UINT32 u4i = 0, u4Flags;
    INT32                ret;
    DRV_COMP_REG_T       t_comp_id;
    DRV_COMP_FCT_TBL_T   t_comp_fct_tbl;
    ID_IND_T             at_id_ind_sec[DMX_MM_CONTAINER_ES_COMP_NUM];
    //ID_IND_T             at_id_ind_fil[DMX_MM_CONTAINER_ES_COMP_NUM];

    /* register Demux Container ES component */
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmxMM_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxMM_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmxMM_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmxMM_Set;

    t_comp_id.e_type = DRVT_DEMUX_CONTAINER_ES;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = DMX_MM_CONTAINER_ES_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind_sec;

    for ( u4i = 0; u4i < DMX_MM_CONTAINER_ES_COMP_NUM; u4i++ )
    {
        at_id_ind_sec[u4i].ui2_id = (UINT16)(DMX_MM_CONTAINER_ES_COMP_ID_START + u4i);
        at_id_ind_sec[u4i].pv_tag = NULL;
        at_id_ind_sec[u4i].ui1_port = ALL_PORTS;
    }

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    ret = x_rm_reg_comp(&t_comp_id,
                        1,                              /* num in port */
                        1,                              /* num out port */
                        NULL, /* name */
                        u4Flags,                        /* flag */
                        &t_comp_fct_tbl,
                        NULL,
                        0);
    if ( ret != RMR_OK )
    {
      return FALSE;
    }

    /* register Demux Control component */
    t_comp_fct_tbl.pf_rm_connect = (x_rm_connect_fct)_MwDmxMM_Connect;
    t_comp_fct_tbl.pf_rm_disconnect = (x_rm_disconnect_fct)_MwDmxMM_Disconnect;
    t_comp_fct_tbl.pf_rm_get = (x_rm_get_fct)_MwDmxMM_Get;
    t_comp_fct_tbl.pf_rm_set = (x_rm_set_fct)_MwDmxMM_Set;

    t_comp_id.e_type = DRVT_DEMUX_CONTROL;
    t_comp_id.e_id_type = ID_TYPE_LIST;
    t_comp_id.u.t_list.ui2_num_of_ids = DMX_MM_CONTROL_COMP_NUM;
    t_comp_id.u.t_list.pt_list = at_id_ind_sec;

    for ( u4i = 0; u4i < DMX_MM_CONTROL_COMP_NUM; u4i++ )
    {
        at_id_ind_sec[u4i].ui2_id = (UINT16)(DMX_MM_CONTROL_COMP_ID + u4i);
        at_id_ind_sec[u4i].pv_tag = NULL;
        at_id_ind_sec[u4i].ui1_port = ALL_PORTS;
    }

    u4Flags = DRV_FLAG_SINGLE_CONN_ON_OUTPUT;

    ret = x_rm_reg_comp(&t_comp_id,
                        1,                              /* num in port */
                        1,                              /* num out port */
                        NULL, /* name */
                        u4Flags,                        /* flag */
                        &t_comp_fct_tbl,
                        NULL,
                        0);
    if ( ret != RMR_OK )
    {
      return FALSE;
    }

    if(!_MW_DMX_MM_PES_Init())
    {
        return FALSE;
    }

    SWDMX_Init();

    VERIFY(x_sema_create(&_hDmxCtrlSema,
        X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    VERIFY(x_sema_create(&_hFlushMutex,
        X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

#ifdef TIME_SHIFT_SUPPORT
    for(u4i=0; u4i<DMX_NUM_PID_INDEX; u4i++)
    {
        _arDmxMMKeyInfo[u4i].fgValid = 0;
        _arDmxMMKeyInfo[u4i].u1KeyIdx = 0xFF;
        _arDmxMMKeyInfo[u4i].u2StreamID = 0xFFFF;
        _arDmxMMKeyInfo[u4i].eDescMode = DMX_DESC_MODE_NONE;
    }
#endif  // TIME_SHIFT_SUPPORT

    return TRUE;
}

