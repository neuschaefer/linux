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
 * $RCSfile: mpv_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mpv_mw.c
 *  This file defines the interfaces for middleware to access MPV driver
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drv_comp_id.h"
#include "drv_name.h"

#define DEFINE_IS_LOG    MPV_IsLog
#include "x_debug.h"
#include "x_assert.h"

#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_vid_dec.h"
#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#endif
#include "vdp_drvif.h"
#include "vdec_if.h"
#include "stc_drvif.h"
#include "mpv_drvif.h"
LINT_EXT_HEADER_END
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define COMP_ID_UNKNOWN    0xFF
#define ES_ID_UNKNOWN 0xFF
#define IFRAME_ES_ID    0x0

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//lint --e{717} do ... while (0);
#define M_GET_VALIDATE(TYPE)             \
    do                                    \
    {                                    \
        if (sizeof(TYPE) > *pz_get_info_len)    \
        {                                \
            *pz_get_info_len = sizeof(TYPE);    \
            return RMR_DRV_NOT_ENOUGH_SPACE;\
        }                                \
        *pz_get_info_len = sizeof(TYPE);        \
    } while (0)

#define M_SET_VALIDATE(TYPE)             \
    do                                    \
    {                                    \
        if (sizeof(TYPE) > z_set_info_len)    \
        {                                \
            return RMR_DRV_INV_SET_INFO;\
        }                                \
    } while (0)

typedef struct _MW_MPV_T
{
    UCHAR            ucEsId;
    UCHAR          ucMode;
    UINT16            u2PcrId;

} MW_MPV_T;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static VID_DEC_NFY_INFO_T _arDecNfyInfo[MPV_COMP_NS];
static MW_MPV_T _arMwMpv[MPV_COMP_NS];

static x_vid_dec_pts_reach_nfy_fct _arPtsCb[MPV_COMP_NS];
static UINT32 _au4PtsTag[MPV_COMP_NS];

static HANDLE_T _hMutex = NULL;			// Thread safe protection
static ENUM_VDEC_FMT_T _eFmt = VDEC_FMT_MPV;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static UCHAR _CompIdtoEsId(UINT16 u2CompId)
{
    return (_arMwMpv[u2CompId].ucEsId);
}


static UINT16 _EsIdtoCompId(UCHAR ucEsId)
{
    UINT16 u2CompId;
    
    for (u2CompId = 0 ;u2CompId < (UINT16)MPV_COMP_NS; u2CompId++)
    {
        if (_arMwMpv[u2CompId].ucEsId == ucEsId)
        {
            return u2CompId;
        }
    }
    return COMP_ID_UNKNOWN;
}

static void _PtsCbFunc(UINT32 u4VdpId, UINT32 u4Pts, UINT32 u4Arg)
{
    BOOL bOutOfSync = 0;
    UINT16 u2CompId;

    UNUSED(u4VdpId);
    
    if (u4Arg & VDP_CB_ERR)
    {
        u4Arg &= (~VDP_CB_ERR);
        bOutOfSync = 1;
    }

    u2CompId = _EsIdtoCompId(u4Arg);

    VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (_arPtsCb[u2CompId])
    {
        _arPtsCb[u2CompId]((VOID*)_au4PtsTag[u2CompId], (PTS_T)u4Pts, bOutOfSync);

        _arPtsCb[u2CompId] = NULL;
    }
    
    VERIFY(x_sema_unlock(_hMutex) == OSR_OK);    
}


static INT32 _GetCtrl(UINT16 u2CompId, VID_DEC_CTRL_T* peDecCtrl, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;
    
    ASSERT(peDecCtrl!=NULL);    
    ASSERT(pz_get_info_len!=NULL);    

    M_GET_VALIDATE(VID_DEC_CTRL_T);

    *peDecCtrl = (VID_DEC_CTRL_T)_arMwMpv[u2CompId].ucMode;

    return i4Ret;
}


static INT32 _GetStc(UCHAR ucEsId, PTS_T* ptPts, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT16 u2CompId;
    STC_CLOCK rStcClk;

    ASSERT(ptPts!=NULL);    
    ASSERT(pz_get_info_len!=NULL);
        
    M_GET_VALIDATE(PTS_T);

    u2CompId = _EsIdtoCompId(ucEsId);

    if (STC_GetSrc((UCHAR)_arMwMpv[u2CompId].u2PcrId, &rStcClk) == STC_VALID)
    {
        *ptPts = (PTS_T)(rStcClk.u4Base);
    }

    return i4Ret;
}


static INT32 _GetResolution(UCHAR ucEsId, VID_DEC_RESOLUTION_INFO_T* peDecResolution, SIZE_T* pz_get_info_len)
{
    VDEC_HDR_INFO_T rHdrInfo;

    ASSERT(peDecResolution!=NULL);
    ASSERT(pz_get_info_len!=NULL);
    
    M_GET_VALIDATE(VID_DEC_RESOLUTION_INFO_T);

    VDEC_QueryInfo(ucEsId, &rHdrInfo);

    if(rHdrInfo.fgHdrInvalid)
    {
        return RMR_DRV_INV_GET_INFO;
    }
    else
    {
        peDecResolution->ui2_width = rHdrInfo.u2Width;
        peDecResolution->ui2_height = rHdrInfo.u2Height;
        peDecResolution->b_is_progressive = rHdrInfo.fgProgressiveSeq;
        peDecResolution->e_src_asp_ratio = (VID_DEC_SRC_ASPECT_RATIO_T)rHdrInfo.eAspectRatio;
        peDecResolution->ui2_frame_rate = rHdrInfo.u2FrmRate;
        peDecResolution->ui2_afd = (UINT16)rHdrInfo.ucAfd;
        return RMR_OK;
    }
}


static INT32 _GetCapability(UINT32 u2CompId, VID_DEC_CAPABILITY_INFO_T* pu4DecCapaInfo, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pu4DecCapaInfo!=NULL);
    ASSERT(pz_get_info_len!=NULL);
    
    M_GET_VALIDATE(VID_DEC_CAPABILITY_INFO_T);

    if (u2CompId == (UINT16)MPV_COMP_5)
    {
        *pu4DecCapaInfo = (VID_DEC_CAP_FMT_1080P | VID_DEC_CAP_FMT_1080I | VID_DEC_CAP_FMT_720P |
                                    VID_DEC_CAP_FMT_576P | VID_DEC_CAP_FMT_576I | VID_DEC_CAP_FMT_480P | VID_DEC_CAP_FMT_480I);
    }
    else
    {
        *pu4DecCapaInfo = (VID_DEC_CAP_FMT_576P | VID_DEC_CAP_FMT_576I | VID_DEC_CAP_FMT_480P | VID_DEC_CAP_FMT_480I);
    }

    if (MPV_SupportDbk())
    {
        *pu4DecCapaInfo |= (VID_DEC_CAP_DEBLOCK);
    }

    return i4Ret;
}


static INT32 _GetPcrId(UINT32 u2CompId, UINT16* pu2PcrId, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pu2PcrId!=NULL);    
    ASSERT(pz_get_info_len!=NULL);
    
    M_GET_VALIDATE(UINT16);

    *pu2PcrId = _arMwMpv[u2CompId].u2PcrId;

    return i4Ret;
}


static INT32 _GetCc(UCHAR ucEsId, VID_DEC_CC_DATA* peDecCcData, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pz_get_info_len!=NULL);
    
    M_GET_VALIDATE(VID_DEC_CC_DATA);

    i4Ret = MPV_GetCc(ucEsId, (MPV_CC_DATA*)(INT32)peDecCcData);

    return i4Ret;
}


static INT32 _GetDbkInfo(UCHAR ucEsId, VID_DEC_DEBLOCKING_INFO_T* prDbkInfo, SIZE_T* pz_get_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(pz_get_info_len!=NULL);
    
    M_GET_VALIDATE(VID_DEC_DEBLOCKING_INFO_T);

    MPV_GetDbkInfo(ucEsId, &prDbkInfo->b_is_on, &prDbkInfo->u.t_region.ui4_x, 
        &prDbkInfo->u.t_region.ui4_y, &prDbkInfo->u.t_region.ui4_w, &prDbkInfo->u.t_region.ui4_h);

    return i4Ret;
}


static INT32 _SetCtrl(UINT16 u2CompId, VID_DEC_CTRL_T eDecCtrl, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UCHAR ucEsId;
    #ifdef CHANNEL_CHANGE_LOG
    HAL_TIME_T dt;
    #endif


    M_SET_VALIDATE(VID_DEC_CTRL_T);
            
    ucEsId = _CompIdtoEsId(u2CompId);
    
    if (ucEsId == ES_ID_UNKNOWN)
    {
        ucEsId = 0;
    }

    switch (eDecCtrl)
    {
        case VID_DEC_CTRL_RESET:
            VDEC_Stop(ucEsId);
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_STOP;
            break;
            
        case VID_DEC_CTRL_STOP:
            if (_arMwMpv[u2CompId].ucMode == (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                ucEsId = IFRAME_ES_ID;
            }
            if (_arMwMpv[u2CompId].ucMode != (UCHAR)VID_DEC_CTRL_STOP)
            {
                VDEC_Stop(ucEsId);
                _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_STOP;
            }
            break;
            
        case VID_DEC_CTRL_FREEZE:
            VDEC_Pause(ucEsId);
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_FREEZE;
            break;
            
        case VID_DEC_CTRL_PLAY:
            #ifdef CHANNEL_CHANGE_LOG
            HAL_GetTime(&dt);
            LOG(0, " %u.%06u s [AV SYNC] 5 MW mpv play\n", dt.u4Seconds, dt.u4Micros);            
            #endif
            
            VDEC_Play(ucEsId, _eFmt);
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_PLAY;
            break;

        case VID_DEC_CTRL_PLAY_I_FRAME:
            if (_arMwMpv[u2CompId].ucMode != (UCHAR)VID_DEC_CTRL_STOP)
            {
                return RMR_DRV_SET_FAILED;
            }

            MW_VDP_Comp2EsInd(u2CompId, IFRAME_ES_ID);            
            VDEC_Play_I_Frame(IFRAME_ES_ID, _eFmt);
            _arMwMpv[u2CompId].ucMode = (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME;        
            break;            
        	
        default:
            LOG(1, "_SetCtrl(): Invalid DecMode\n");
            return RMR_DRV_INV_SET_INFO;
    }

    if (_arDecNfyInfo[u2CompId].pf_vid_dec_nfy)
    {
        _arDecNfyInfo[u2CompId].pf_vid_dec_nfy(_arDecNfyInfo[u2CompId].pv_tag,
                                    VID_DEC_COND_CTRL_DONE,
                                    (UINT32)eDecCtrl, 0);
    }

    return i4Ret;
}


static INT32 _SetNfyFct(UINT16 u2CompId, const VID_DEC_NFY_INFO_T* ptDecNfyInfo, SIZE_T z_set_info_len)
{
	INT32 i4Ret = RMR_OK;
//	UCHAR ucEsId;
    
    ASSERT(ptDecNfyInfo != NULL);

    M_SET_VALIDATE(VID_DEC_NFY_INFO_T);

    _arDecNfyInfo[u2CompId].pv_tag = ptDecNfyInfo->pv_tag;
    _arDecNfyInfo[u2CompId].pf_vid_dec_nfy = ptDecNfyInfo->pf_vid_dec_nfy;

#if 0
	ucEsId = _CompIdtoEsId(u2CompId);

	MPV_SetDecNfy(ucEsId, (MPV_DEC_NFY_INFO_T*)ptDecNfyInfo);

#endif

	return i4Ret;
}


static INT32 _SetNfyFctPts(UINT16 u2CompId, const VID_DEC_NFY_PTS_INFO_T* ptPtsNfyInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UCHAR ucEsId;

    if ((ptPtsNfyInfo == NULL) ||
        (z_set_info_len != sizeof (VID_DEC_NFY_PTS_INFO_T)))
    {
        i4Ret = RMR_DRV_INV_SET_INFO;
    }
    else
    {
        VERIFY(x_sema_lock(_hMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    
         _arPtsCb[u2CompId] = ptPtsNfyInfo->pf_vid_dec_pts_reach_nfy;
        _au4PtsTag[u2CompId] = (UINT32)(ptPtsNfyInfo->pv_tag);

        ucEsId = _CompIdtoEsId(u2CompId);

        if (ucEsId == ES_ID_UNKNOWN)
        {
            ucEsId = 0;
        }

        VDP_RegCbFunc(VDP_CB_FUNC_PTS_IND, (UINT32)_PtsCbFunc, (UINT32)ucEsId, (UINT32)ptPtsNfyInfo->t_pts);
        i4Ret = RMR_OK;
        
        VERIFY(x_sema_unlock(_hMutex) == OSR_OK);        
    }

    return i4Ret;
}


static INT32 _SetInpMode(UCHAR ucEsId, const VID_DEC_INP_MODE_INFO_T* ptInpInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    
    M_SET_VALIDATE(VID_DEC_INP_MODE_INFO_T);

    i4Ret = MPV_SetInpNfyFct(ucEsId, (MPV_INP_NFY_INFO_T*)(void*)ptInpInfo);

    return i4Ret;
}


static INT32 _SetPlayCc(UCHAR ucEsId, const VID_DEC_CC_NFY_INFO_T* ptCcNfyInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_CC_NFY_INFO_T);

    i4Ret = MPV_SetCcNfyFct(ucEsId, (MPV_CC_NFY_INFO_T*)(void*)ptCcNfyInfo);

    MPV_PlayCc(ucEsId);

    return i4Ret;
}


static INT32 _SetStopCc(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    MPV_StopCc(ucEsId);

    return i4Ret;
}


static INT32 _SetPcrId(UCHAR ucEsId, UINT16 u2PcrId, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;
    UINT16 u2CompId;
    
    M_SET_VALIDATE(UINT16);

    u2CompId = _EsIdtoCompId(ucEsId);

    _arMwMpv[u2CompId].u2PcrId = u2PcrId;

    VDEC_SyncStc(ucEsId, AV_SYNC_MODE_SLAVE, u2PcrId);

    return i4Ret;
}


static INT32 _SetCapture(UCHAR ucEsId, const VID_DEC_CAPTURE_INFO_T* ptCaptuerInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

   ASSERT(ptCaptuerInfo != NULL);
    M_SET_VALIDATE(VID_DEC_CAPTURE_INFO_T);
    UNUSED(ucEsId);

    return i4Ret;
}


static INT32 _SetAllocCcBuf(UCHAR ucEsId, SIZE_T zCcBufSize, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(SIZE_T);

    i4Ret = MPV_AllocCcBuf(ucEsId, (UINT32)zCcBufSize);

    return i4Ret;
}


static INT32 _SetFreeCcBuf(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    i4Ret = MPV_FreeCcBuf(ucEsId);

    return i4Ret;
}


static INT32 _SetFlushCcBuf(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    i4Ret = MPV_FlushCcBuf(ucEsId);

    return i4Ret;
}


static INT32 _SetIFrameNfyFct(UINT16 u2CompId, const VID_DEC_NFY_INFO_T* ptDecNfyInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    ASSERT(ptDecNfyInfo != NULL);
    M_SET_VALIDATE(VID_DEC_NFY_INFO_T);

    _arDecNfyInfo[u2CompId].pv_tag = ptDecNfyInfo->pv_tag;
    _arDecNfyInfo[u2CompId].pf_vid_dec_nfy = ptDecNfyInfo->pf_vid_dec_nfy;

    i4Ret = MPV_SetDecNfy(IFRAME_ES_ID, (MPV_DEC_NFY_INFO_T*)(void*)ptDecNfyInfo);

    return i4Ret;
}


static INT32 _SetAllocIFrameBuf(UCHAR ucEsId, const VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_I_FRAME_BUFF_T);

    i4Ret = MPV_AllocIFrameBuf(ucEsId, (MPV_DEC_I_FRAME_BUFF_T*)(void*)ptIFrameBuff);

    return i4Ret;
}


static INT32 _SetFreeIFrameBuf(UCHAR ucEsId)
{
    INT32 i4Ret = RMR_OK;

    i4Ret = MPV_FreeIFrameBuf(ucEsId);

    return i4Ret;
}


static INT32 _SetIFrameBuf(UCHAR ucEsId, const VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_I_FRAME_BUFF_T);

    i4Ret = MPV_SetIFrameBuf(ucEsId, (MPV_DEC_I_FRAME_BUFF_T*)(void*)ptIFrameBuff);

    return i4Ret;
}


static INT32 _SetIFrameDec(UCHAR ucEsId, const VID_DEC_I_FRAME_BUFF_T* ptIFrameBuff, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_I_FRAME_BUFF_T);

    i4Ret = MPV_SetIFrameDec(ucEsId, (MPV_DEC_I_FRAME_BUFF_T*)(void*)ptIFrameBuff);

    return i4Ret;
}


static INT32 _SetDbkInfo(UCHAR ucEsId, const VID_DEC_DEBLOCKING_INFO_T* ptDbkInfo, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_OK;

    M_SET_VALIDATE(VID_DEC_DEBLOCKING_INFO_T);

    MPV_SetDbkInfo(ucEsId, ptDbkInfo->b_is_on, ptDbkInfo->u.t_region.ui4_x, 
        ptDbkInfo->u.t_region.ui4_y, ptDbkInfo->u.t_region.ui4_w, ptDbkInfo->u.t_region.ui4_h);

    return i4Ret;    
}


static INT32 _MpvMwConnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    INT32 i4RetCode = RMR_OK;
    DRV_COMP_ID_T* prDmxCompReg;

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }
    
    if (pv_conn_info == NULL)
    {
        return (RMR_DRV_INV_CONN_INFO);
    }
    

    prDmxCompReg = (DRV_COMP_ID_T*)pv_conn_info;

    if (prDmxCompReg->e_type != DRVT_DEMUX_TS_PES_PACKET)
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    if ((prDmxCompReg->ui2_id) > (UINT16)(DMX_PES_COMP_ID_END))
    {
       return (RMR_DRV_INV_CONN_INFO);
    }

    UNUSED(pt_comp_id);    

    return i4RetCode;
}


static INT32 _MpvMwDisconnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_DISC_TYPE_T  e_disc_type,
                            const VOID*      pv_disc_info,
                            SIZE_T           z_disc_info_len)
{
    INT32 i4RetCode = RMR_OK;
    DRV_COMP_ID_T* prDmxCompReg;

    UNUSED(e_disc_type);
    UNUSED(pv_disc_info);
    UNUSED(z_disc_info_len);

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->b_sel_out_port)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (e_disc_type != RM_CONN_TYPE_COMP_ID)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (pv_disc_info == NULL)
    {
        return (RMR_DRV_INV_DISC_INFO);    
    }

    prDmxCompReg = (DRV_COMP_ID_T*)pv_disc_info;

    if (prDmxCompReg->e_type != DRVT_DEMUX_TS_PES_PACKET)
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    if (prDmxCompReg->ui2_id > ((UINT16)(DMX_PES_COMP_ID_END)))
    {
       return (RMR_DRV_INV_DISC_INFO);
    }

    // handle disconnect
    _arMwMpv[pt_comp_id->ui2_id].ucEsId = 0xFF;

    UNUSED(pt_comp_id);    

    return i4RetCode;
}


static INT32 _MpvMwGet(
                        DRV_COMP_ID_T*        pt_comp_id,
                        DRV_GET_TYPE_T        e_get_type,
                        VOID*                pv_get_info,
                        SIZE_T*                pz_get_info_len)
{
    INT32 i4RetCode = RMR_DRV_INV_GET_INFO;
    UCHAR ucEsId;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
           LOG(1, "MW_MPV_Get(): error component type or id type\n");
           return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
        LOG(1, "MW_MPV_Get(): error component id [%d]\n", pt_comp_id->ui2_id);
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pz_get_info_len == NULL)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);

    if (ucEsId == ES_ID_UNKNOWN)
    {
        ucEsId = 0;
    }    

    switch (e_get_type)
    {
        case VID_DEC_GET_TYPE_CTRL:
            i4RetCode = _GetCtrl(pt_comp_id->ui2_id, (VID_DEC_CTRL_T*)pv_get_info, pz_get_info_len);
            break;
            
        case VID_DEC_GET_TYPE_STC:
            i4RetCode = _GetStc(ucEsId, (PTS_T*)pv_get_info, pz_get_info_len);
            break;
            
        case VID_DEC_GET_TYPE_RESOLUTION:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode == (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                ucEsId = IFRAME_ES_ID;
            }
            i4RetCode = _GetResolution(ucEsId, (VID_DEC_RESOLUTION_INFO_T*)pv_get_info, pz_get_info_len);
            break;
            
        case VID_DEC_GET_TYPE_CAPABILITY:
            i4RetCode = _GetCapability(pt_comp_id->ui2_id, (VID_DEC_CAPABILITY_INFO_T*)pv_get_info, pz_get_info_len);
            break;
            
        case VID_DEC_GET_TYPE_PCR_ID:
            i4RetCode = _GetPcrId(pt_comp_id->ui2_id, (UINT16*)pv_get_info, pz_get_info_len);
            break;
            
        case VID_DEC_GET_TYPE_CC_DATA:
            i4RetCode = _GetCc(ucEsId, (VID_DEC_CC_DATA*)pv_get_info, pz_get_info_len);
            break;

        case VID_DEC_GET_TYPE_DEBLOCKING:
            i4RetCode = _GetDbkInfo(ucEsId, (VID_DEC_DEBLOCKING_INFO_T*)pv_get_info, pz_get_info_len);
            break;
        default:
            LOG(1, "MPV_MwGet(): Invalid get_type\n");
            return (RMR_DRV_INV_GET_INFO);

    }

    UNUSED(pt_comp_id);    

    return i4RetCode;
}


static INT32 _MpvMwSet(
                    DRV_COMP_ID_T*        pt_comp_id,
                    DRV_SET_TYPE_T         e_set_type,
                    const VOID*            pv_set_info,
                    SIZE_T                z_set_info_len)
{
    INT32 i4RetCode = RMR_DRV_INV_SET_INFO;
    UCHAR ucEsId;


    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_SET_INFO);
    }  

    if (pt_comp_id->e_type != DRVT_VID_DEC)
    {
        LOG(1, "MW_MPV_Set(): error component type or id type\n");
        return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)MPV_COMP_NS)
    {
        LOG(1, "MW_MPV_Set(): error component id [%d]\n", pt_comp_id->ui2_id);
        return (RMR_DRV_INV_SET_INFO);
    }

    #if 0
    // Check set_info
    if (pv_set_info == NULL)
    {
        LOG(1, "MW_MPV_Set(): invalid set_info\n");
        return (RMR_DRV_INV_SET_INFO);
    }
    #endif

    ucEsId = _CompIdtoEsId(pt_comp_id->ui2_id);

    if (ucEsId == ES_ID_UNKNOWN)
    {
        ucEsId = 0;
    }
    
    switch (e_set_type)
    {
        case VID_DEC_SET_TYPE_CTRL:
            i4RetCode = _SetCtrl(pt_comp_id->ui2_id, (VID_DEC_CTRL_T)(UINT32)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_NFY_FCT:
            i4RetCode = _SetNfyFct(pt_comp_id->ui2_id, (VID_DEC_NFY_INFO_T*)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_NFY_PTS:
            i4RetCode = _SetNfyFctPts(pt_comp_id->ui2_id, (VID_DEC_NFY_PTS_INFO_T*)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_INP_MODE:
            i4RetCode = _SetInpMode(ucEsId, (VID_DEC_INP_MODE_INFO_T*)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_PLAY_CC:
            i4RetCode = _SetPlayCc(ucEsId, (VID_DEC_CC_NFY_INFO_T*)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_STOP_CC:
            i4RetCode = _SetStopCc(ucEsId);
            break;
            
        case VID_DEC_SET_TYPE_PCR_ID:
            i4RetCode = _SetPcrId(ucEsId, (UINT16)(UINT32)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_CAPTURE:
            i4RetCode = _SetCapture(ucEsId, (VID_DEC_CAPTURE_INFO_T*)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_ALLOC_CC_BUFF:
            i4RetCode = _SetAllocCcBuf(ucEsId, (SIZE_T)pv_set_info, z_set_info_len);
            break;
            
        case VID_DEC_SET_TYPE_FREE_CC_BUFF:
            i4RetCode = _SetFreeCcBuf(ucEsId);
            break;
            
        case VID_DEC_SET_TYPE_FLUSH_CC_BUFF:
            i4RetCode = _SetFlushCcBuf(ucEsId);
            break;

        case VID_DEC_SET_TYPE_I_FRAME_BUFF:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_SET_FAILED);
            }                    
            i4RetCode = _SetIFrameBuf(IFRAME_ES_ID, (VID_DEC_I_FRAME_BUFF_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_ALLOC_I_FRAME_BUFF:
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_SET_FAILED);
            }             
            i4RetCode = _SetAllocIFrameBuf(IFRAME_ES_ID, (VID_DEC_I_FRAME_BUFF_T*)pv_set_info, z_set_info_len);        
            break;

        case VID_DEC_SET_TYPE_FREE_I_FRAME_BUFF:          
            i4RetCode = _SetFreeIFrameBuf(IFRAME_ES_ID);        
            break;

        case VID_DEC_SET_TYPE_I_FRAME_DECODE:        
            if (_arMwMpv[pt_comp_id->ui2_id].ucMode != (UCHAR)VID_DEC_CTRL_PLAY_I_FRAME)
            {
                return (RMR_DRV_SET_FAILED);
            }             
            i4RetCode = _SetIFrameDec(IFRAME_ES_ID, (VID_DEC_I_FRAME_BUFF_T*)pv_set_info, z_set_info_len);
            break;

        case VID_DEC_SET_TYPE_I_FRAME_NFY_FCT:
            i4RetCode = _SetIFrameNfyFct(pt_comp_id->ui2_id, (VID_DEC_NFY_INFO_T*)pv_set_info, z_set_info_len);        
            break;

        case VID_DEC_SET_TYPE_DEBLOCKING:
            i4RetCode = _SetDbkInfo(ucEsId, (VID_DEC_DEBLOCKING_INFO_T*)pv_set_info, z_set_info_len);        
            break;
            
        default:
            LOG(1, "MPV_MwSet(): Invalid set_type\n");
            return (RMR_DRV_INV_SET_INFO);

    }

    UNUSED(pt_comp_id);    

    return i4RetCode;
}


//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

INT32 MW_MPV_Init(void)
{
    INT32 i4Return;
    DRV_COMP_REG_T tMpvCompReg;
    DRV_COMP_REG_T tMpvCompExclReg;
    //DRV_COMP_REG_T tDmxCompReg;
    DRV_COMP_FCT_TBL_T tMpvFctTbl;
    UINT32 u4Idx;
    UINT16 u2CompId;

    if (_hMutex == NULL)
    {
        // create semaphore
        VERIFY(x_sema_create(&_hMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }    

    /* Init MPV */
    VDEC_Init();

    for (u4Idx = 0; u4Idx < (UINT32)MPV_COMP_NS; u4Idx++)
    {
        _arPtsCb[u4Idx] = NULL;
    }

    /* register mpv component */
    for (u4Idx = 0; u4Idx < (UINT32)MPV_COMP_NS; u4Idx++)
    {
        UINT32 u4Flags;
        
        /* Setup component ID */
        tMpvCompReg.e_type = DRVT_VID_DEC;
        tMpvCompReg.e_id_type = ID_TYPE_IND;
        tMpvCompReg.u.t_ind.ui2_id = u4Idx;
        tMpvCompReg.u.t_ind.ui1_port = ALL_PORTS;
        tMpvCompReg.u.t_ind.pv_tag = NULL;

        /* Registration Flags */
        if ((u4Idx != (UINT32)MPV_COMP_5))
        {
            u4Flags = VID_DEC_REG_FLAG_USED_NORMAL_MODE|
                        VID_DEC_REG_FLAG_USED_IN_QUAD_MODE|DRV_FLAG_SINGLE_CONN_ON_OUTPUT;
        }
        else
        {
            u4Flags = VID_DEC_REG_FLAG_USED_NORMAL_MODE|DRV_FLAG_SINGLE_CONN_ON_OUTPUT;
        }

        /* Setup component function table */
        tMpvFctTbl.pf_rm_connect = _MpvMwConnect;
        tMpvFctTbl.pf_rm_disconnect = _MpvMwDisconnect;
        tMpvFctTbl.pf_rm_get = _MpvMwGet;
        tMpvFctTbl.pf_rm_set = _MpvMwSet;

        /* Register component with the Resource Manager database */
        i4Return = x_rm_reg_comp(&tMpvCompReg,
                                1,
                                1,
                                NULL,
                                u4Flags,
                                &tMpvFctTbl,
                                NULL,
                                0);        
       
        if (i4Return != RMR_OK)
        {
            LOG(1, "MW_MPV_Init(): x_rm_reg_comp failed [%d]\n", (INT32)i4Return);
            return i4Return;
        }

    }

    /* register connection list */
#if 0
    for (u4Idx = MPV_COMP_1; u4Idx < MPV_COMP_NS; u4Idx++ )
    {

        tMpvCompReg.e_type = DRVT_VID_DEC;
        tMpvCompReg.e_id_type = ID_TYPE_IND;
        tMpvCompReg.u.t_ind.ui2_id = u4Idx;
        tMpvCompReg.u.t_ind.ui1_port = ALL_PORTS;
        tMpvCompReg.u.t_ind.pv_tag = NULL;


        /* Setup Dmx component ID */
        tDmxCompReg.e_type = DRVT_DEMUX_TS_PES_PACKET;
        tDmxCompReg.e_id_type = ID_TYPE_RANGE;
        tDmxCompReg.u.t_range.ui2_first_id = DMX_PES_COMP_ID_START;
        tDmxCompReg.u.t_range.ui2_delta_id = 1;
        tDmxCompReg.u.t_range.ui2_num_of_ids = DMX_PES_COMP_NUM;
        tDmxCompReg.u.t_range.pv_first_tag = NULL;
        tDmxCompReg.u.t_range.pv_delta_tag = NULL;
        tDmxCompReg.u.t_range.ui1_port = ALL_PORTS;

        /* register connection list */
        i4Return = x_rm_reg_conn_list( &tMpvCompReg,
                                    &tDmxCompReg,
                                    1,
                                    CONN_DIR_INP_TO_OUT,
                                    FALSE );

        if ( i4Return != RMR_OK )
        {
            LOG(1, "MW_MPV_Init(): x_rm_reg_conn_list failed [%d]\n", (unsigned int)i4Return);
            return i4Return;
        }

    }
#endif


    /* Register component exclusive list */
    tMpvCompReg.e_type = DRVT_VID_DEC;
    tMpvCompReg.e_id_type = ID_TYPE_IND;
    tMpvCompReg.u.t_ind.ui2_id = (UINT16)MPV_COMP_5;
    tMpvCompReg.u.t_ind.ui1_port = ALL_PORTS;
    tMpvCompReg.u.t_ind.pv_tag = NULL;

    tMpvCompExclReg.e_type = DRVT_VID_DEC;
    tMpvCompExclReg.e_id_type = ID_TYPE_RANGE;
    tMpvCompExclReg.u.t_range.ui2_first_id = (UINT16)MPV_COMP_1;
    tMpvCompExclReg.u.t_range.ui2_delta_id = 1;
    tMpvCompExclReg.u.t_range.ui2_num_of_ids = (UINT16)MPV_COMP_SD_NS;
    tMpvCompExclReg.u.t_range.pv_first_tag = NULL;
    tMpvCompExclReg.u.t_range.pv_delta_tag = NULL;
    tMpvCompExclReg.u.t_range.ui1_port = ALL_PORTS;

    i4Return = x_rm_reg_comp_excl_list (&tMpvCompReg,
                                        &tMpvCompExclReg,
                                        1);

    if (i4Return != RMR_OK)
    {
        LOG(1, "MW_MPV_Init(): x_rm_reg_comp_excl_list failed [%d]\n", (INT32)i4Return);
        return i4Return;
    }

    for (u2CompId = 0 ;u2CompId < (UINT16)MPV_COMP_NS; u2CompId++)
    {
        _arMwMpv[u2CompId].ucEsId = 0xFF;  
        _arMwMpv[u2CompId].ucMode= (UCHAR)VID_DEC_CTRL_STOP;          
    }
    
    return (RMR_OK);
}


UCHAR MW_MPV_CompIdtoVldId(UINT16 u2CompId, UINT32 u4ChannelId)
{
    UCHAR ucVldId = 0xf;

    switch (u2CompId)
    {
        case MPV_COMP_1:
        case MPV_COMP_2:
        case MPV_COMP_3:
        case MPV_COMP_4:
        case MPV_COMP_5:
            ucVldId = VLD0;
            break;

        default:
            LOG(1, "Invalid CompId\n");
            ASSERT(0);
            break;
    }

    _arMwMpv[u2CompId].ucEsId= u4ChannelId;

    MW_VDP_Comp2EsInd(u2CompId, u4ChannelId);

    return (ucVldId);
}

void MW_MPV_NOTIFY(UINT16 u2CompId)
{
    if (_arDecNfyInfo[u2CompId].pf_vid_dec_nfy)
    {
        _arDecNfyInfo[u2CompId].pf_vid_dec_nfy(_arDecNfyInfo[u2CompId].pv_tag, VID_DEC_COND_RESOLUTION_CHG, 0, 0);
    }
}

