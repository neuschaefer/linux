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
 * $Author: xuezhong.li $
 * $Date: 2012/06/27 $
 * $RCSfile: swdmx_drvif.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_drvif.c
 *  Software program stream demux driver - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "swdmx_if.h"
#include "swdmx_drvif.h"
#include "swdmx_avi.h"
#include "swdmx_mpg.h"
#include "swdmx_wmv.h"
#include "swdmx_ogg.h"
#include "swdmx_mp4.h"
#include "swdmx_flv.h"
#include "swdmx_mkv.h"
#include "swdmx_rm.h"
#include "swdmx_cmpb.h"
#include "swdmx_mpg_pvr.h"
#include "feeder_if.h"
#include "fbm_drvif.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "vdec_drvif.h"
#include "x_util.h"
#include "x_rand.h"
#include "x_timer.h"
#ifdef CC_FLASH_TEST
#include "swdmx_flash.h"
#endif

#include "b2r_drvif.h"
#include "b2r_if.h"

#if defined(SWDMX_DBG_SEMIHOST) || defined(SWDMX_DBG_USB)
#include "swdmx_debug_util.h"
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SWDMX_VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(0, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }      \
          } while (0)

#define SWDMX_VERIFY_NULL_ERROR(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(0, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return FALSE; }      \
          } while (0)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DATA_EXHAUSTED_THRESHOLD (16)
#define DATA_EXHAUSTED_THRESHOLD_TRICKMODE (4)
#define UNDER_FLOW_COUNT (100)

// 1 seconds
#define DATA_READY_TIME_THRESHOLD (117000)
// 100 ms
#define DATA_EXHAUSTED_TIME_THRESHOLD (45000)

#define WAIT_INPUT_DATA_THRD 3000   // 3 sec
#define WAIT_INPUT_DATA_THRD_TRICKMODE 6000//6 sec
#define AUDIO_DATA_READY_THRESHOLD 0x10000 //64k

#define IS_NETWORK_SRC(x)   ((x == MM_SRC_TYPE_NETWORK_VUDU) || (x == MM_SRC_TYPE_NETWORK_CMPB) \
                             || (x == MM_SRC_TYPE_NETWORK_DLNA) || (x == MM_SRC_TYPE_PUSH)   \
                             || (x == MM_SRC_TYPE_NETWORK_XUNLEI) || (x == MM_SRC_TYPE_NETWORK_NETFLIX))
#define USE_COMMON_NETBUFFER_FLOW(x) ((x==MM_SRC_TYPE_NETWORK_DLNA) || (x == MM_SRC_TYPE_NETWORK_CMPB))
//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgSwdmxInitialized = FALSE;
#ifdef CC_SKYPE_FINE_INSTANCE
SWDMX_INFO_T _arSwdmxInfo[SWDMX_SOURCE_MAX];
#elif defined(CC_FBM_TWO_FBP)
SWDMX_INFO_T _arSwdmxInfo[SWDMX_SOURCE_MAX];
#else
SWDMX_INFO_T _arSwdmxInfo[SWDMX_SOURCE_MAX];
#endif
static SWDMX_KEEP_INFO_T _rSwdmxKeepInfo = {0};
UINT32 _u4APreFIFO[2]; // 0: start,1:end
UINT32 _u4APreFIFOSize;

//static SWDMX_INFO_T _rSwdmxInfo = {0};

//static SWDMX_RANGE_LIST_T prSwdmxInst->rCurRangeReq;
//static SWDMX_RANGE_LIST_T prSwdmxInst->rQRangeReq;

//static HANDLE_T prSwdmxInst->hSwDmxMainThread = 0;
//static HANDLE_T prSwdmxInst->hRangeMutex = 0;
//static HANDLE_T prSwdmxInst->hDmxFeederReqDataQ = 0;
//static HANDLE_T prSwdmxInst->hDmxPlayCmdQ = 0;
//static HANDLE_T prSwdmxInst->hFlushMutex = 0;
//static HANDLE_T prSwdmxInst->hFlushSema = 0;

#ifdef CC_DLNA_SUPPORT
//static HANDLE_T _hFeederUnderFlowMutex = NULL;
//static BOOL prSwdmxInst->fgFeederUnderFlow = FALSE;
#endif
//static SWDMX_DECODER_INFO_T _rDecoderInfo;

#ifdef CC_SWDMX_RENDER_PTS
static UINT32 _u4RenderPts = 0;
#endif

#ifdef CC_SWDMX_RENDER_POSITION_TEST
static BOOL _fgSetRenderPosition = FALSE;
static START_POS_INFO_T _rRenderPos;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static VOID _SWDMX_ResetSwdmxInfo(UINT8 u1SrcId);

VOID _SWDMX_RANGE_CALLBACK (VOID*            pv_nfy_tag,
                               MM_RANGE_COND_T  e_nfy_cond,
                               UINT32           ui4_data_1,
                               UINT32           ui4_data_2,
                               UINT32           ui4_data_3)
{
    UINT8 u1SrcId = (UINT8)ui4_data_3;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);

    if (prSwdmxInst->pfnSwdmxRangeCb)
    {
        prSwdmxInst->pfnSwdmxRangeCb(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                    e_nfy_cond,
                                    prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                    ui4_data_2);
    }
    else if (prSwdmxInst->pfnSwdmxRangeCbEx)
    {
        prSwdmxInst->pfnSwdmxRangeCbEx(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                    e_nfy_cond,
                                    prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                    ui4_data_2,
                                    ui4_data_3);
    }
    else
    {
        LOG(3, "pfnSwdmxRangeCb/pfnSwdmxRangeCbEx null\n");
    }
    UNUSED(pv_nfy_tag);
    UNUSED(ui4_data_1);
    UNUSED(ui4_data_2);
    UNUSED(ui4_data_3);
}

#if 0
static VOID _SwdmxWaitDataTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    LOG(2, "swdmx data exhausted=1, timeout\n");
    //UINT32 u4SrcId = (UINT32)(*pv_tag);
    UINT32 u4SrcId = 0;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst((UINT8)u4SrcId);

    FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);
    _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                            MM_RANGE_BUFF_UNDERFLOW,
                            prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            u4SrcId);
    //prSwdmxInst->fgSendUnderflow2MW=TRUE;
    prSwdmxInst->fgWaitDataTimerStart = TRUE;
}
#endif

static VOID _CheckDataExhausted(UINT8 u1SrcId,MM_SRC_TYPE_T e_SrcType)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    UINT32 u4SrcId = (UINT32)u1SrcId;
    UINT32 u4Pts,u4STC;
    UINT32 u4Delta =0,u4UnderThreshold=0,u4ReadyThreashold=0;
    INT32 i4Delta =0 ;
    //BOOL fgDataReady = FALSE;
    SWDMX_BUFFER_STATE_T eBufferState = SWDMX_BUFFER_STATE_NORMAL;

    //UINT16 u2VdecQueueSize = 0, u2VdecMaxQueueSize = 0;
    VDEC_ES_INFO_T* prVdecEsInfo = NULL;

    SWDMX_VERIFY_NULL(prSwdmxInst);
    u4STC = STC_GetStcValue(STC_SRC_A1);
    prVdecEsInfo = _VDEC_GetEsInfo(prSwdmxInst->u1VdecId);

    if(prVdecEsInfo == NULL)
    {
        return;
    }

    u4Pts = prVdecEsInfo->u4LatestPTS;

    //the same with vdp_frc : calculate delta of pts and stc for AV sync
    if(u4Pts > u4STC)
    {
        u4Delta = (u4Pts - u4STC);
        i4Delta = -u4Delta;
    }
    else
    {
        u4Delta = u4STC - u4Pts;
        i4Delta = u4Delta;
    }
    if(prSwdmxInst->fgEnVideoDmx)
    {
        if(!MPV_IsDisplay(prSwdmxInst->u1VdecId))
        {
            return;
        }
    }
    else if(prSwdmxInst->fgEnAudioDmx)
    {
        if(!AUD_IsDecoderPlay(prSwdmxInst->u1AdecId))
        {
           return;
        }
    }

    if(prSwdmxInst->i4PlaySpeed != 1000)
    {
        if(prSwdmxInst->fgSendUnderflow2MW)
        {
           //FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);
            _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                MM_RANGE_BUFF_READY,
                                prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF,
                                u4SrcId);
            prSwdmxInst->fgSendUnderflow2MW=FALSE;
            prSwdmxInst->fgDataExhausted = FALSE;

        }
        return;
    }

    if(prSwdmxInst->fgEnVideoDmx)
    {
        u4UnderThreshold=SWDMX_BUFFER_STATE_UNDER_FLOW;
        u4ReadyThreashold=DATA_READY_TIME_THRESHOLD;
        if(USE_COMMON_NETBUFFER_FLOW(e_SrcType))
        {
           u4ReadyThreashold=DATA_READY_TIME_THRESHOLD*5;
        }
    }
    else if(prSwdmxInst->fgEnAudioDmx)
    {
        u4UnderThreshold=0x1000;
        u4ReadyThreashold=AUDIO_DATA_READY_THRESHOLD;
    }

    if(prSwdmxInst->fgEnVideoDmx)
    {
        if(i4Delta <0)
        {
            // pts - stc > 1 seconds , it means we have at least 1 seconds data in vfifo
            if(u4Delta > u4ReadyThreashold)
            {
                eBufferState = SWDMX_BUFFER_STATE_READY;
            }
            else if(u4Delta < u4UnderThreshold)
            {
                eBufferState = SWDMX_BUFFER_STATE_UNDER_FLOW;
            }
        }
        else
        {
            eBufferState = SWDMX_BUFFER_STATE_UNDER_FLOW;
        }
    }
    else if(prSwdmxInst->fgEnAudioDmx)
    {
        UINT32 u4AudAvailSize = 0;
        if (u1SrcId < SWDMX_SOURCE_MAX)
        {
            _SWDMX_GetInfo(u1SrcId, eSWDMX_GET_AUDIO_ACTIVE_PIDX, &u4AudAvailSize, 0, 0);
        }

        if(u4AudAvailSize >u4ReadyThreashold)
        {
            eBufferState = SWDMX_BUFFER_STATE_READY;
        }
        else if(u4AudAvailSize<u4UnderThreshold)
        {
            eBufferState = SWDMX_BUFFER_STATE_UNDER_FLOW;
        }
    }

    if(prSwdmxInst->fgRcvEOS || prSwdmxInst->fgFifoFull)
    {
         eBufferState = SWDMX_BUFFER_STATE_READY;
    }

    // maybe other src type is also needn't following code,not only cmpb and dlna.
    if(!USE_COMMON_NETBUFFER_FLOW(e_SrcType))
    {
        //error handling for pause/play issue.
        //disable under flow check after under flow 100 times
        if((eBufferState == SWDMX_BUFFER_STATE_READY) && (prSwdmxInst->u4DataUnderFlowCount < UNDER_FLOW_COUNT))
        {
            prSwdmxInst->u4DataUnderFlowCount = 0;
        }
        else
        {
            prSwdmxInst->u4DataUnderFlowCount++;
            if(prSwdmxInst->u4DataUnderFlowCount >= UNDER_FLOW_COUNT)
            {
                if(prSwdmxInst->fgSendUnderflow2MW)
                {
                    _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                        MM_RANGE_BUFF_READY,
                                        prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                        0xFFFFFFFF,
                                        u4SrcId);
                    prSwdmxInst->fgSendUnderflow2MW=FALSE;
                }
                prSwdmxInst->fgDataExhausted = TRUE;
                return;
            }
        }
    }

    if (eBufferState == SWDMX_BUFFER_STATE_UNDER_FLOW)
    {
        if(!prSwdmxInst->fgDataExhausted)
        {
            if(prSwdmxInst->fgEnVideoDmx && prSwdmxInst->fgSendUnderflow2MW == FALSE &&
                !(prSwdmxInst->fgEnAudioDmx && AUD_IsFifoFull(prSwdmxInst->u1AdecId)))
            {
                FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);
                _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                        MM_RANGE_BUFF_UNDERFLOW,
                                        prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                        0xFFFFFFFF,
                                        u4SrcId);

                LOG(2, "swdmx data exhausted1 = 1 (pts-stc = %d), start timer\n", u4Delta);
                prSwdmxInst->fgSendUnderflow2MW = TRUE;
                prSwdmxInst->fgDataExhausted = TRUE;
            }
        }
    }
    else if ((prSwdmxInst->fgRcvEOS || (eBufferState == SWDMX_BUFFER_STATE_READY)) && prSwdmxInst->fgDataExhausted)
    {
        LOG(2, "swdmx data exhausted3 = 0 (pts-stc = %d), stop timer\n",u4Delta);

        prSwdmxInst->fgDataExhausted = FALSE;
        if(prSwdmxInst->fgSendUnderflow2MW)
        {
            _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                MM_RANGE_BUFF_READY,
                                prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF,
                                u4SrcId);
            prSwdmxInst->fgSendUnderflow2MW=FALSE;
        }
    }
}


#if 0//def CC_DLNA_SUPPORT
VOID _SWDMX_FeederUnderFlowCb(UINT8 u1SrcId)
{
    UINT32 u4SrcId = (UINT32)u1SrcId;
    SWDMX_INFO_T *prSwdmxInst = &_arSwdmxInfo[u4SrcId];

    //VERIFY(x_sema_lock(_hFeederUnderFlowMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (prSwdmxInst->i4PlaySpeed == 1000 && !prSwdmxInst->fgFeederUnderFlow)
    {
        _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                    MM_RANGE_BUFF_UNDERFLOW,
                                    prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                    0xFFFFFFFF,
                                    u4SrcId);
        prSwdmxInst->fgFeederUnderFlow = TRUE;
        if (prSwdmxInst->fgEnVideoDmx)
        {
            VDEC_Pause(prSwdmxInst->u1VdecId);
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
        }
        if (prSwdmxInst->fgEnAudioDmx)
        {
#ifdef CC_MT5363
            AUD_DSPCmdPause(AUD_D_DSP, prSwdmxInst->u1AdecId);
            AUD_DSPCmdPause(AUD_A_DSP, prSwdmxInst->u1AdecId);
#else
            AUD_DSPCmdPause(prSwdmxInst->u1AdecId);
#endif
        }
        LOG(0,"_SWDMX_FeederUnderFlowCb : A/V pause\n");
    }
    //VERIFY(x_sema_unlock(_hFeederUnderFlowMutex) == OSR_OK);
}

VOID _SWDMX_FeederStatusNfyCb(UINT8 u1SrcId, UINT32 u4ActionId, UINT32 u4Param1, UINT32 u4Param2)
{
    UINT32 u4SrcId = (UINT32)u1SrcId;

    VDEC_HDR_INFO_T rVdecHdrInfo;
    SWDMX_INFO_T *prSwdmxInst = &_arSwdmxInfo[u4SrcId];

    if (u4ActionId == FEEDER_NFY_STATUS_READY)
    {
        //VERIFY(x_sema_lock(_hFeederUnderFlowMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
        if (((prSwdmxInst->i4PlaySpeed == 1000)|| (prSwdmxInst->i4PlaySpeed == 2000))
            && prSwdmxInst->fgFeederUnderFlow)
        {
            _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                        MM_RANGE_BUFF_READY,
                                        prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                        0xFFFFFFFF,
                                        u4SrcId);
            prSwdmxInst->fgFeederUnderFlow = FALSE;
            VDEC_QueryInfo(prSwdmxInst->u1VdecId,&rVdecHdrInfo);
            if (prSwdmxInst->fgEnVideoDmx)
            {
                if (VDP_GetStepForward(prSwdmxInst->u1B2rId) != 0)
                {
                    VDP_SetFBNoTimeout(prSwdmxInst->u1B2rId, FALSE);
                    VDP_SetStepForward(prSwdmxInst->u1B2rId, VDP_PLAY_NORMAL);
                    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
                }
                if (VDP_GetSlowForward(prSwdmxInst->u1B2rId) != 0)
                {
                    VDP_SetFBNoTimeout(prSwdmxInst->u1B2rId, FALSE);
                    VDP_SetSlowForward(prSwdmxInst->u1B2rId, VDP_PLAY_NORMAL);
                }
                VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
                VDEC_Play(prSwdmxInst->u1VdecId, rVdecHdrInfo.eCodecType);

                if(prSwdmxInst->i4PlaySpeed == 1000)
                {
                    VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);
                    VDP_SetSpeed(prSwdmxInst->u1B2rId,STC_SPEED_TYPE_FORWARD_1X);
                    VDP_SetTrickMode(prSwdmxInst->u1B2rId, VDP_PLAY_NORMAL);
                }
                else
                {
                    VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_2X, 0, 0);
                    VDP_SetSpeed(prSwdmxInst->u1B2rId,STC_SPEED_TYPE_FORWARD_2X);
                    VDP_SetTrickMode(prSwdmxInst->u1B2rId, VDP_PLAY_NORMAL);
                }
            }
            if (prSwdmxInst->fgEnAudioDmx)
            {
#ifdef CC_MT5363
                AUD_DSPCmdResume(AUD_D_DSP, prSwdmxInst->u1AdecId);
                AUD_DSPCmdResume(AUD_A_DSP, prSwdmxInst->u1AdecId);
#else
                AUD_DSPCmdResume(prSwdmxInst->u1AdecId);
#endif
            }
            LOG(0,"_SWDMX_FeederStatusNfyCb : A/V resume\n");
        }
        //VERIFY(x_sema_unlock(_hFeederUnderFlowMutex) == OSR_OK);
    }
    else if (u4ActionId == FEEDER_NFY_STATUS_IGNORE_READY)
    {
        if (prSwdmxInst->fgFeederUnderFlow)
        {
             VDEC_QueryInfo(prSwdmxInst->u1VdecId,&rVdecHdrInfo);
            if (prSwdmxInst->fgEnVideoDmx)
            {
                VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
                VDEC_Play(prSwdmxInst->u1VdecId, rVdecHdrInfo.eCodecType);
            }
            if (prSwdmxInst->fgEnAudioDmx)
            {
#ifdef CC_MT5363
                AUD_DSPCmdResume(AUD_D_DSP, prSwdmxInst->u1AdecId);
                AUD_DSPCmdResume(AUD_A_DSP, prSwdmxInst->u1AdecId);
#else
                AUD_DSPCmdResume(prSwdmxInst->u1AdecId);
#endif
            }
            LOG(0,"_SWDMX_FeederStatusNfyCb : A/V resume\n");
            prSwdmxInst->fgFeederUnderFlow = FALSE;
       }
    }
}
#endif
/**
*/
#if 0
SWDMX_HDR_PRS_STATE_T _SWDMX_HdrParseFeederTest(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL   fgRet;
    UINT16 u2Cnt;
    INT32  i4Ret;
    UINT32 u4FeederReqID;
    UINT32 u4SendSize;
    UINT32 u4SendPos;
    UINT32 u4Count;
    UINT32 u4ReqCount;
    UINT32 u4TestCount;
    FEEDER_REQ_DATA_T rFeederReqData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_BUF_INFO_T rBuffInfo;
    UINT32 u4NewPtr;

    srand(1328198);

    while (1)
    {
        x_thread_delay(100);
        VERIFY(x_msg_q_num_msgs(prSwdmxInst->hDmxFeederReqDataQ, &u2Cnt) == OSR_OK);
        if (u2Cnt == 0)
        {
            break;
        }
        _SWDMX_FlushReqMsgQ();
        LOG(0, "Wait for flush\n");
    }

    u4FeederReqID = 0;
    u4SendPos = 0;
    u4SendSize = 1024;
    u4TestCount = 0;
    FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &rBuffInfo);
    while (u4TestCount < 100000)
    {
        _SWDMX_FlushReqMsgQ();
        u4ReqCount = 0;
        u4Count = (((UINT32)rand()) % 5) + 1; // random
        u4SendPos = (((UINT32)rand()) % ((UINT32)prSwdmxInst->u8FileSize)) + 1; // random
        u4SendSize = (((UINT32)rand()) % ((235*1024) - 1)) + 1; // random

        if (u4SendSize == 0)
        {
            LOG(1, "u4SendSize = 0\n");
            continue;
        }

        while (u4Count--)
        {
            if ( u4SendPos >= (UINT32)prSwdmxInst->u8FileSize)
            {
                u4SendPos = 0;
            }
            i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                                     u4SendSize,
                                     (UINT64)(u4SendPos),
                                     &rFeederCond,
                                     u4FeederReqID++);
            if (i4Ret != FEEDER_E_OK)
            {
                LOG(0, "FeederSetRequest Fail, pos %d size %d\n", (UINT32)u4SendPos, (UINT32)u4SendSize);
                ASSERT(0);
            }
            else
            {
                //handle other case, FEEDER_E_INVALID_PARAM
            }
            u4SendPos += u4SendSize;
            u4ReqCount ++;
        }

        u4Count = u4ReqCount; // the times try to remove previouw feeder's ack
        while (u4Count--)
        {
            fgRet = _SWDMX_ReceiveFeederAck((VOID*)&rFeederReqData);
            // check rFeederReqData.u4Id
            if ((!fgRet) || (rFeederReqData.u4ReadSize == 0))
            {
                LOG(0, "size = 0 or ack timeout\n");
                ASSERT(0);
            }
            if (rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
            {
                LOG(0, "FEEDER_SOURCE_INVALID.\n");
                ASSERT(0);
            }
            u4NewPtr = (rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize > rBuffInfo.u4EndAddr)?
                       rBuffInfo.u4StartAddr + rFeederReqData.u4ReadSize - (rBuffInfo.u4EndAddr - rFeederReqData.u4WriteAddr)
                       : rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize;
            i4Ret = FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, u4NewPtr);
            if (i4Ret != FEEDER_E_OK)
            {
                LOG(0, "FeederUpdateReadPointer fail pos %d\n",
                    rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize);
                ASSERT(0);
            }
        }
        if (rFeederReqData.u4Id != (u4FeederReqID-1))
        {
            LOG(0, "return ReqID %d not equal to request one %d\n", rFeederReqData.u4Id, (u4FeederReqID-1));
            ASSERT(0);
        }


        if ((u4TestCount % 500) == 0)
        {
            LOG(0, "TestCount %d\n", u4TestCount);
        }
        u4TestCount++;
    }
    LOG(0, "Feeder Test Finish\n");

    switch (prSwdmxInst->eCurFMT)
    {
    case SWDMX_FMT_AVI:
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_AVIHdrParse;
        break;
    case SWDMX_FMT_VC1_WMV:
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_WMVHeaderParse;
        break;
    case SWDMX_FMT_MPEG1_DAT:
    case SWDMX_FMT_MPEG2_PS:
    case SWDMX_FMT_MPEG2_TS:
    case SWDMX_FMT_MPEG2_TS_192:
    case SWDMX_FMT_MPEG2_TS_ZERO_192:
    case SWDMX_FMT_MPEG2_TS_ENCRYPT_192:
    case SWDMX_FMT_MPEG2_TS_TIME_SHIFT:
    case SWDMX_FMT_MPEG2_VIDEO_ES:
    case SWDMX_FMT_H264_VIDEO_ES:
    case SWDMX_FMT_VC1_ES:
    case SWDMX_FMT_MPEG4_VIDEO_ES:
    case SWDMX_FMT_RAW_AUDIO_ES:
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        break;
    case SWDMX_FMT_MP4:
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_Mp4HdrPrs;
        break;
    case SWDMX_FMT_MKV:
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MKVHdrParse;
        break;

    default:
        LOG(0, "unknow format\n");
        break;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}
#endif

VOID _SWDMX_LockFlushMutex(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!prSwdmxInst)
    {
        return;
    }

    if (prSwdmxInst->hFlushMutex == 0)
    {
        return;
    }
    VERIFY(x_sema_lock(prSwdmxInst->hFlushMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
}

VOID _SWDMX_UnlockFlushMutex(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!prSwdmxInst)
    {
        return;
    }

    if (prSwdmxInst->hFlushMutex == 0)
    {
        return;
    }
    VERIFY(x_sema_unlock(prSwdmxInst->hFlushMutex) == OSR_OK);
}

VOID _SWDMX_LockFlushSema(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!prSwdmxInst)
    {
        return;
    }

    if (prSwdmxInst->hFlushSema == 0)
    {
        return;
    }
    VERIFY(x_sema_lock(prSwdmxInst->hFlushSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

VOID _SWDMX_UnlockFlushSema(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!prSwdmxInst)
    {
        return;
    }

    if (prSwdmxInst->hFlushSema == 0)
    {
        return;
    }
    VERIFY(x_sema_unlock(prSwdmxInst->hFlushSema) == OSR_OK);
}

static VOID _SWDMX_LockSrcMutex(VOID)
{
    if (!_rSwdmxKeepInfo.hSwdmxInfoMutex)
    {
        VERIFY(x_sema_create(&_rSwdmxKeepInfo.hSwdmxInfoMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    VERIFY(x_sema_lock(_rSwdmxKeepInfo.hSwdmxInfoMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
}

static VOID _SWDMX_UnlockSrcMutex(VOID)
{
    VERIFY(x_sema_unlock(_rSwdmxKeepInfo.hSwdmxInfoMutex) == OSR_OK);
}

static BOOL _SWDMX_ReceivePlayCmd(UINT8 u1SrcId, VOID *pvData, UINT32 u4CurState)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    ENUM_SWDMX_PLAY_STATE_T eStatus;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    zMsgSize = sizeof(UINT32);
    //LOG(0,"current status : %d\n",u4CurState);
    if (u4CurState != eSWDMX_STATE_PLAY)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(prSwdmxInst->hDmxPlayCmdQ), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(prSwdmxInst->hDmxPlayCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    ASSERT(zMsgSize == sizeof(UINT32));

    //u4Cmd = (UINT32*)(pvData);
    //LOG(0,"_SWDMX_ReceivePlayCmd : %d\n",*u4Cmd);
    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    eStatus = (ENUM_SWDMX_PLAY_STATE_T)(*(UINT32*)pvData);
    if (eStatus == eSWDMX_STATE_MAX)
    {
        _SWDMX_LockFlushMutex(u1SrcId);
        if (prSwdmxInst->fgFlush)
        {
            UNUSED(_SWDMX_DoFlush(u1SrcId));
            prSwdmxInst->fgFlush = FALSE;
            _SWDMX_UnlockFlushSema(u1SrcId);
        }
        _SWDMX_UnlockFlushMutex(u1SrcId);
        return FALSE;
    }

    return TRUE;
}


// block function from now on.
static BOOL _SWDMX_SendPlayCmd(UINT8 u1SrcId, ENUM_SWDMX_PLAY_STATE_T ePlayState)
{
    INT32 i4Ret;
    UINT32 u4PlayCmd = ePlayState;
    SIZE_T zMsgSize = sizeof(UINT32);
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    //LOG(0, "_SWDMX_SendPlayCmd %d\n", ePlayState);

    i4Ret = x_msg_q_send(prSwdmxInst->hDmxPlayCmdQ, &u4PlayCmd, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "SWDMX Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(prSwdmxInst->hDmxPlayCmdQ, &u4PlayCmd, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

#if 0
    // make sure status is changed.
    while (prSwdmxInst->eSwDmxStatus != ePlayState)
    {
        x_thread_delay(1);
    }
#endif

    return TRUE;

}


VOID _SWDMX_FlushReqMsgQ(UINT8 u1SrcId)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    //UINT32 u4DecFin;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    FEEDER_REQ_DATA_T rData;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL(prSwdmxInst);

    LOG(5, "%s(%d)\n", __FUNCTION__, u1SrcId);

    zMsgSize = sizeof(FEEDER_REQ_DATA_T);

    while (x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                           &(prSwdmxInst->hDmxFeederReqDataQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(prSwdmxInst->hDmxFeederReqDataQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
    // We have to have one dummy ack in hand.
    // So we could return to swdmx main loop after we do FlushReqMsgQ.
    rData.eDataType = FEEDER_SOURCE_INVALID;
    zMsgSize = sizeof(rData);
    i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &rData, zMsgSize, 255);
    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
}

UINT16 _SWDMX_GetFeederQueueSize(UINT8 u1SrcId)
{
    UINT16 u2QueueSize;

    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    VERIFY(x_msg_q_num_msgs(prSwdmxInst->hDmxFeederReqDataQ, &u2QueueSize) == OSR_OK);

    return u2QueueSize;
}

#ifdef SWDMX_DBG_USB
static inline VOID _SWDMX_UsbDumpFeederBlock(VOID *pvData)
{
    FEEDER_REQ_DATA_T *prData;
    static UINT32 u4Cnt = 0;
    
    if (NULL ==pvData)
    {
        if (DUMP_LOG)
        {
            LOG(1, "%s:%u Pointer PvData is NULL.\n", __FUNCTION__, __LINE__);
        }
    }
    
    prData = (FEEDER_REQ_DATA_T *)pvData;

    if (USB_DUMP_ON(DUMP_DATA_FEEDER_SWDMX))
    {
         static CHAR ucFileName[NAME_MAX_LEN] = {0};
            
        if (FEEDER_SOURCE_INVALID!=prData->eDataType && 0!=prData->u4WriteAddr )
        {
            x_sprintf(ucFileName, "%s/Feeder_%010u_%010u.raw", FILE_LOCAL, prData->u8FilePos, u4Cnt);
            Dbg_SaveBlock(ucFileName, VIRTUAL(prData->u4WriteAddr+prData->u4AlignOffset), prData->u4ReadSize);
            
            if (DUMP_LOG)
            {
                LOG(1, "Feeder KAddr:0x%08x VAddr:0x%08x Size=%u\n", prData->u4WriteAddr+prData->u4AlignOffset, 
                    VIRTUAL(prData->u4WriteAddr+prData->u4AlignOffset), prData->u4ReadSize);
            }
            
            u4Cnt++;
        }
#ifdef CC_FLASH_TEST
        if (TRUE == prData->fgFirst)
        {
            if (DUMP_LOG)
            {
                LOG(1, "%s:%u First Time.\n", __FUNCTION__, __LINE__);
            }
        }
#endif
    }

    return ;
}
#endif

INT32 _SWDMX_ReceiveFeederAck(UINT8 u1SrcId, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    FEEDER_REQ_DATA_T *prData;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!pvData)
    {
        LOG(1, "_SWDMX_ReceiveFeederAck pvData = NULL\n");
        return FALSE;
    }

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);


    zMsgSize = sizeof(FEEDER_REQ_DATA_T);
    prData = pvData;
    // swdmx is used in playmgr parsing info.
    // we can't check range info here.
    if (!prSwdmxInst->rCurRangeReq.prDmxRangeInfo)
    {
        //LOG(3, "Wrong Range in _SWDMX_ReceiveFeederAck\n");
        //return FALSE;
       i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, pvData, &zMsgSize,
                                        &(prSwdmxInst->hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT_NETWORK);
    }
    else
    {
#if 0
       if ((prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_UNKNOWN) ||
          (prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_HIGH_SPEED_STORAGE))
       {
          i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, pvData, &zMsgSize,
                                        &(prSwdmxInst->hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT_HIGH_SPEED);
       }
       else
       {
          i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, pvData, &zMsgSize,
                                        &(prSwdmxInst->hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT_NETWORK);
       }
#endif
        i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, pvData, &zMsgSize,
                                    &(prSwdmxInst->hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT_HIGH_SPEED);
    }


    ASSERT(zMsgSize == sizeof(FEEDER_REQ_DATA_T));

    if(prData->eDataType != FEEDER_SOURCE_INVALID)
    {
        if(prData->fgEof==TRUE)
        {
            SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_SET_EOS, TRUE, 0, 0);
        }
        else
        {
            SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_SET_EOS, FALSE, 0, 0);
        }
    }

    if (i4Ret == OSR_TIMEOUT)
    {
        prData->eDataType = FEEDER_SOURCE_INVALID;
        LOG(1, "_SWDMX_ReceiveFeederAck Timeout\n");
        // Restore original behavior. We would not reset request when timeout
        // Only reset request when u4WriteAddr = 0 (error case).
        //return OSR_TIMEOUT;
        return TRUE;
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(3, "_SWDMX_ReceiveFeederAck Fail %d\n", i4Ret);
        return FALSE;
    }

#ifdef SWDMX_DBG_USB
    if (USB_DUMP_ON(DUMP_DATA_FEEDER_SWDMX) &&
        FEEDER_SOURCE_INVALID!=prData->eDataType &&
        0!=prData->u4WriteAddr)
    {
        _SWDMX_UsbDumpFeederBlock(pvData);
    }
#endif

    return TRUE;
}


INT32 _SWDMX_ReceiveFeederAckNoWait(UINT8 u1SrcId, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    FEEDER_REQ_DATA_T *prData;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!pvData)
    {
        LOG(1, "_SWDMX_ReceiveFeederAck pvData = NULL\n");
        return FALSE;
    }

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    zMsgSize = sizeof(FEEDER_REQ_DATA_T);
    prData = pvData;
    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &prSwdmxInst->hDmxFeederReqDataQ, 1, X_MSGQ_OPTION_NOWAIT);

    ASSERT(zMsgSize == sizeof(FEEDER_REQ_DATA_T));

    if((prData->eDataType != FEEDER_SOURCE_INVALID) && (i4Ret==OSR_OK))
    {
        if(prData->fgEof==TRUE)
        {
             SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_SET_EOS, TRUE, 0, 0);
        }
        else
        {
             SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_SET_EOS, FALSE, 0, 0);
        }
    }

    if (i4Ret == OSR_NO_MSG)
    {
        LOG(9, "_SWDMX_ReceiveFeederAckNoWait do nothing\n");
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(3, "_SWDMX_ReceiveFeederAckNoWait Fail %d\n", i4Ret);
    }
    
#ifdef SWDMX_DBG_USB
    if (USB_DUMP_ON(DUMP_DATA_FEEDER_SWDMX) &&
        FEEDER_SOURCE_INVALID!=prData->eDataType &&
        0!=prData->u4WriteAddr)
    {
        _SWDMX_UsbDumpFeederBlock(pvData);
    }
#endif

    return i4Ret;
}

VOID _SWDMX_FeederCallBack(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param)
{
    // Send Msg to prSwdmxInst->hDmxFeederReqDataQ
    INT32 i4Ret;
    SIZE_T zMsgSize;
    FEEDER_REQ_DATA_T rFeederReqData;

    if ((!prNfyInfo) ||
        (u4Param == 0) ||
        (prNfyInfo->u1SwdmxId >= SWDMX_SOURCE_MAX))
    {
        LOG(3, "Unknown status in _SWDMX_FeederCallBack\n");
        return;
    }


    zMsgSize = sizeof(FEEDER_REQ_DATA_T);
    x_memcpy(&rFeederReqData, (FEEDER_REQ_DATA_T*)u4Param, sizeof(FEEDER_REQ_DATA_T));
    //LOG(0, "_SWDMX_FeederCallBack Id %d\n", rFeederReqData.u4Id);
    i4Ret = x_msg_q_send(_arSwdmxInfo[prNfyInfo->u1SwdmxId].hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "_SWDMX_FeederCallBack Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(_arSwdmxInfo[prNfyInfo->u1SwdmxId].hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);
}

#ifdef CC_CMPB_PUSH_MODE
static VOID _SWDMX_PushModeReset(SWDMX_INFO_T *prSwdmxInst)
{
	x_memset(&(prSwdmxInst->demuxer), 0, sizeof(sd_demuxer_t));
	prSwdmxInst->demuxer.stream = sd_stream_open(SD_STREAM_MODE_XX);
	prSwdmxInst->demuxer.stream->pv_host = (void *)prSwdmxInst;
	prSwdmxInst->pb_ctrl.pv_host = (void *)prSwdmxInst;
	if ((prSwdmxInst->demuxer.sub_demuxer = swdmx_get_sub_demuxer(prSwdmxInst->eCurFMT)) != NULL)
	{
		extern SWDMX_HDR_PRS_STATE_T _SWDMX_HdrPrsX(UINT8,SWDMX_RANGE_LIST_T*,SWDMX_CUR_CMD_T*);
		prSwdmxInst->pfnSwdmxHdrParse	 = _SWDMX_HdrPrsX;
		prSwdmxInst->pfnSwdmxEnableStrm  = _SWDMX_EnableStrmX;
		prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_DisableStrmX;
		prSwdmxInst->pfnSwdmxStop		 = _SWDMX_StopX; 
		prSwdmxInst->pfnSwdmxSetRange	 = NULL;
		prSwdmxInst->pfnSwdmxSetStrmID	 = NULL;
		prSwdmxInst->pfnSwdmxAbort		 = NULL;
	
		//prSwdmxInst->pfnSwdmxSetInfo	   = _SWDMX_SetDecoderInfoX;
		prSwdmxInst->pfnSwdmxGetInfo	 = _SWDMX_GetInfoX;
	
		prSwdmxInst->pb_ctrl.curr_s.state = INIT_S;
	}
	//memory 
	swdmx_mem_init(&(prSwdmxInst->pr_mem_list));

}
#endif
UINT32 _u4SwdmxDelay = 33/*1*/;  // megaa 20100414
static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverData(UINT8 u1SrcId)
{
    SWDMX_HDR_PRS_STATE_T ePrsState;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl;
    UINT32 u4StartPTS, u4EndPTS;
    static UINT64 u8SeekPos;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    if (!prSwdmxInst)
    {
        LOG(1, "_SWDMX_DeliverData Swdmx Instance is NULL\n");
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    if (prSwdmxInst->rCurRangeReq.fgIsFull == TRUE)
    {
        prSwdmxInst->rCurRangeReq.fgInUse = TRUE;

        ASSERT(prSwdmxInst->pfnSwdmxHdrParse);

        prSwdmxInst->rCmd.fgChange = prSwdmxInst->fgPlayChange;
        prSwdmxInst->rCmd.u8SeekPos = prSwdmxInst->u8SeekPos;
        prSwdmxInst->rCmd.i4PlaySpeed = prSwdmxInst->i4PlaySpeed;
        prSwdmxInst->rCmd.ePlayMode=prSwdmxInst->ePlayMode;
        if (prSwdmxInst->fgPlayChange)
        {
            if(!((prSwdmxInst->i4PlaySpeed == SWDMX_PLAY_SPEED_1X) ||
                (prSwdmxInst->i4PlaySpeed == SWDMX_PLAY_SPEED_2X)))
            {
                FeederIgnoreReady(prSwdmxInst->eFeederSrc);
            }

            prSwdmxInst->rCmd.fgChange = TRUE;
            prSwdmxInst->rCmd.fgSeek = FALSE; // only active with SEEK_OFFSET
            prSwdmxInst->rCmd.u8SeekPos = prSwdmxInst->u8SeekPos;
            prSwdmxInst->rCmd.i4PlaySpeed = prSwdmxInst->i4PlaySpeed;
            if(prSwdmxInst->fgPlayChangeOffset)
            {
                prSwdmxInst->rCmd.fgSeek = TRUE;
                prSwdmxInst->rCmd.u4SeekMode = SWDMX_CUR_CMD_SEEK_OFFSET;
                prSwdmxInst->rCmd.rSeekOffset.ui8_pts_info =
                    prSwdmxInst->rSeekOffset.ui8_pts_info;
                prSwdmxInst->rCmd.rSeekOffset.ui8_stc =
                    prSwdmxInst->rSeekOffset.ui8_stc;
                prSwdmxInst->rCmd.rSeekOffset.ui8_frame_position_disp =
                    prSwdmxInst->rSeekOffset.ui8_frame_position_disp;
                prSwdmxInst->rCmd.rSeekOffset.ui8_i_frame_position =
                    prSwdmxInst->rSeekOffset.ui8_i_frame_position;
                prSwdmxInst->rCmd.rSeekOffset.ui8_frame_position =
                    prSwdmxInst->rSeekOffset.ui8_frame_position;
                prSwdmxInst->rCmd.rSeekOffset.i4_temporal_reference =
                    prSwdmxInst->rSeekOffset.i4_temporal_reference;
                prSwdmxInst->rCmd.rSeekOffset.ui2_decoding_order =
                    prSwdmxInst->rSeekOffset.ui2_decoding_order;
                prSwdmxInst->rCmd.rSeekOffset.ui8_aud_pts_info =
                    prSwdmxInst->rSeekOffset.ui8_aud_pts_info;
                prSwdmxInst->rCmd.rSeekOffset.ui8_i_pts_info =
                    prSwdmxInst->rSeekOffset.ui8_i_pts_info;
            }
            else // seek time
            {
                // keep the prSwdmxInst->u8SeekPos = seek time
                // for backword compatible
            }

            ePrsState = prSwdmxInst->pfnSwdmxHdrParse(
                            u1SrcId,
                            &prSwdmxInst->rCurRangeReq,
                            &prSwdmxInst->rCmd
                        );
            u8SeekPos = prSwdmxInst->u8SeekPos;
            prSwdmxInst->u8SeekPos = 0xFFFFFFFF;
            prSwdmxInst->u8SeekPos <<= 32;
            prSwdmxInst->u8SeekPos += 0xFFFFFFFF;
            prSwdmxInst->fgPlayChange = FALSE;
            prSwdmxInst->fgPlayChangeOffset = FALSE;
            prSwdmxInst->rCmd.fgSeek = FALSE;
            prSwdmxInst->rCmd.u4SeekMode = SWDMX_CUR_CMD_SEEK_ORIGINAL;
            prSwdmxInst->rCmd.fgChange = FALSE;
            prSwdmxInst->ePrePlayMode=prSwdmxInst->ePlayMode;
        }
        else
        {
            ePrsState = prSwdmxInst->pfnSwdmxHdrParse(
                            u1SrcId,
                            &prSwdmxInst->rCurRangeReq,
                            &prSwdmxInst->rCmd
                        );

          if(prSwdmxInst->rCurRangeReq.prDmxRangeInfo &&
             IS_NETWORK_SRC(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
            {
               _CheckDataExhausted(u1SrcId,prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type);
               prSwdmxInst->fgFifoFull = FALSE;
            }
        }
        switch (ePrsState)
        {
        case eSWDMX_HDR_PRS_SUCCEED:
            if ((prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_FINISHED) ||
                    (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_SKIP) ||
                    (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_PENDING)||
                    (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_NO_INDEX))
            {
                MM_RANGE_INFO_T* pTempRangeInfo = prSwdmxInst->rCurRangeReq.prDmxRangeInfo;

                // Callback to the upper layer for notifying range complete
                if (//(prSwdmxInst->pfnSwdmxRangeCb != 0) &&
                    (pTempRangeInfo))
                {
                    //mp4 seek case
                    if (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_SKIP)
                    {
                        if (prSwdmxInst->rQRangeReq.fgIsFull == TRUE)
                        {
                            prIdxTbl = prSwdmxInst->rQRangeReq.prDmxRangeInfo->pt_idxtbl_lst;
                            //find video table
                            while (prIdxTbl != NULL)
                            {
                                if (prIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
                                {
                                    //query if Q range contain target seek PTS
                                    u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
                                    u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;

                                    LOG(1, "Instance %d, Seek: seek 0x%x, Q 0x%x--0x%x\n",
                                        u1SrcId, (UINT32)u8SeekPos, u4StartPTS, u4EndPTS);
                                    break;
                                }
                                prIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prIdxTbl->pv_next_tbl;
                            }
                            _SWDMX_RANGE_CALLBACK(pTempRangeInfo->pv_tag,
                                                        MM_RANGE_ABORT,
                                                        pTempRangeInfo->ui4_range_id,
                                                        0xFFFFFFFF,
                                                        u1SrcId);
                        }
                        else
                        {
                            LOG(2, "Instance %d Wait Q range indexing done.Request pts = 0x%08x\n",
                                u1SrcId, prSwdmxInst->rCurRangeReq.u4CurPTS);
                            _SWDMX_RANGE_CALLBACK(pTempRangeInfo->pv_tag,
                                                        MM_RANGE_DONE,
                                                        pTempRangeInfo->ui4_range_id,
                                                        prSwdmxInst->rCurRangeReq.u4CurPTS,
                                                        u1SrcId);
                        }
                    }
                    else if (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_FINISHED)
                    {
                        _SWDMX_RANGE_CALLBACK(pTempRangeInfo->pv_tag,
                                                    MM_RANGE_DONE,
                                                    prSwdmxInst->rCurRangeReq.u4RangeId,
                                                    0xFFFFFFFF,
                                                    u1SrcId);
                        LOG(5, "Instance %d, Range Finished Callback to Update Range ID = 0x%08x\n",
                            u1SrcId,
                            prSwdmxInst->rCurRangeReq.u4RangeId);
                        {
                            // delete range
                            prSwdmxInst->rCurRangeReq.fgInUse = FALSE;
                            SWDMX_DelRange(u1SrcId, NULL, (UINT32)prSwdmxInst->rCurRangeReq.u4RangeId, FALSE);
                        }
                    }
                    else if (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_NO_INDEX)
                    {
                        _SWDMX_RANGE_CALLBACK(pTempRangeInfo->pv_tag,
                                                    MM_RANGE_SEEK_ERROR,
                                                    prSwdmxInst->rCurRangeReq.u4RangeId,
                                                    0xFFFFFFFF,
                                                    u1SrcId);
                    }
                }

                if (((prSwdmxInst->eCurFMT == SWDMX_FMT_MP4)||(prSwdmxInst->eCurFMT == SWDMX_FMT_M4A)) &&
                        ((prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_FINISHED) ||
                         (prSwdmxInst->rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_SKIP)))
                {
                    // for speed change in the file end, we keep
                    prSwdmxInst->rCurRangeReq.fgInUse = FALSE;
                    SWDMX_DelRange(u1SrcId, NULL, (UINT32)prSwdmxInst->rCurRangeReq.u4RangeId, FALSE);  //set new range from Qrange
                }
                else
                {
                    // we finish play, sleep a while
                    x_thread_delay(33);
                }
            }
            break;
        case eSWDMX_HDR_PRS_FED_MOVE_FAIL:
        case eSWDMX_HDR_PRS_FAIL:
            // Fail to move data
            // we finish play, sleep a while
#if 1  // megaa 20100414
            x_thread_delay(_u4SwdmxDelay);  // megaa 20100225: tmp solution for push mode
#else
            x_thread_delay(33);
#endif
            break;
        default:
            break;
        }
    }
    else
    {
        x_thread_delay(10);
    }

    return eSWDMX_HDR_PRS_EMPTY_RANGE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_MainLoop
 *  Software demuxer main loop
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _SWDMX_MainLoop(VOID *pvArg)
{
    UINT8 u1SrcId;
    UINT32 u4PlayCmd;
    SWDMX_INFO_T *prSwdmxInst;
    //SWDMX_HDR_PRS_STATE_T eStatus;
    //UNUSED(pvArg);

    u1SrcId = (UINT8)(*((UINT8*)pvArg));
    if(u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(1, "_SWDMX_MainLoop (u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return;
    }

    prSwdmxInst = &_arSwdmxInfo[u1SrcId];
    while (1)
    {
        // block if SwDmx status is not eSWDMX_STATE_PLAY
        u4PlayCmd = (UINT32)eSWDMX_STATE_MAX;
        if (_SWDMX_ReceivePlayCmd(u1SrcId, &u4PlayCmd, prSwdmxInst->eSwDmxStatus) == TRUE)
        {
            if ((prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_STOP) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_PLAY))
            {
                ASSERT(prSwdmxInst->rCurRangeReq.prDmxRangeInfo!=NULL);
                if (prSwdmxInst->eCurFMT != SWDMX_FMT_ES_WITH_PTS)
                {
                    ASSERT(prSwdmxInst->pfnSwdmxSetRange != NULL);
                }
#ifdef CC_CMPB_PUSH_MODE
				if(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type==MM_SRC_TYPE_PUSH
					&&prSwdmxInst->eCurFMT==SWDMX_FMT_FLV)
				{
					prSwdmxInst->u1PushMode = TRUE;
					_SWDMX_PushModeReset(prSwdmxInst);
				}
				else
				{
					prSwdmxInst->u1PushMode = FALSE;

				}
#endif			
                _SWDMX_FlushReqMsgQ(u1SrcId);

                if (prSwdmxInst->pfnSwdmxSetRange != NULL)
                {
                    prSwdmxInst->pfnSwdmxSetRange(
                        u1SrcId,
                        &prSwdmxInst->rCurRangeReq,
                        prSwdmxInst->u4VidStrmID,
                        prSwdmxInst->u4AudStrmID,
                        prSwdmxInst->u4AudSubStrmID);
                }
                if (prSwdmxInst->rCurRangeReq.prDmxRangeInfo)
                {
                    if(USE_COMMON_NETBUFFER_FLOW(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
                    {
                        prSwdmxInst->fgDataExhausted = FALSE;
                    }
                    if (prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_vid_render_counts)
                    {
                        VDEC_SetRenderPicCount(prSwdmxInst->u1VdecId, TRUE,
                            prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_vid_render_counts);
                    }
                }
            }

            if ((prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PAUSE) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_PLAY))
            {
                if (prSwdmxInst->pfnSwdmxPlay)
                {
                    prSwdmxInst->pfnSwdmxPlay(u1SrcId);
                }
            }

            if ((prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PLAY) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_PAUSE))
            {
                if (prSwdmxInst->pfnSwdmxPause)
                {
                    prSwdmxInst->pfnSwdmxPause(u1SrcId);
                }
            }

            if ((prSwdmxInst->eSwDmxStatus != eSWDMX_STATE_STOP) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_STOP))
            {
                if(prSwdmxInst->rCurRangeReq.prDmxRangeInfo &&
                    IS_NETWORK_SRC(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
                {
                    prSwdmxInst->fgRcvEOS = FALSE;
                    prSwdmxInst->fgDataExhausted = TRUE;
                    prSwdmxInst->fgWaitDataTimerStart = FALSE;
                    prSwdmxInst->fgSendUnderflow2MW=FALSE;
                    prSwdmxInst->u4DataUnderFlowCount = 0;
                    VERIFY(x_timer_stop(prSwdmxInst->hWaitDataTimer) == OSR_OK);
                    LOG(2, "swdmx data exhausted, stop timer\n");
                }

                if (prSwdmxInst->pfnSwdmxStop)
                {
                    prSwdmxInst->pfnSwdmxStop(u1SrcId);
                }

#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
                prSwdmxInst->fgIsBadInterleaved = FALSE;
#endif
                prSwdmxInst->eSubVideoType = ENUM_SWDMX_VDEC_UNKNOWN;
                prSwdmxInst->u8SeekPos = 0xFFFFFFFF;
                prSwdmxInst->u8SeekPos <<= 32;
                prSwdmxInst->u8SeekPos += 0xFFFFFFFF;
                prSwdmxInst->u4SeekTime = 0;
                prSwdmxInst->fgPlayChange = FALSE;
                prSwdmxInst->fgPlayChangeOffset = FALSE;
                prSwdmxInst->i4PlaySpeed = SWDMX_PLAY_SPEED_1X;
                prSwdmxInst->ePlayMode=SWDMX_PLAY_MODE_NORMAL;
                prSwdmxInst->ePrePlayMode=SWDMX_PLAY_MODE_NORMAL;
                prSwdmxInst->ePlaybackBufType = eSWDMX_BUF_MODE_DEFAULT;
                prSwdmxInst->u4PlaybackBufThreshold = 0;
                prSwdmxInst->rCmd.i4PlaySpeed = SWDMX_PLAY_SPEED_1X;
                prSwdmxInst->rCmd.u8SeekPos = 0xFFFFFFFF;
                prSwdmxInst->rCmd.u8SeekPos <<= 32;
                prSwdmxInst->rCmd.u8SeekPos += 0xFFFFFFFF;
                prSwdmxInst->rCmd.fgSeek = FALSE;
                prSwdmxInst->rCmd.fgChange = FALSE;
                prSwdmxInst->rCmd.u4SeekMode = SWDMX_CUR_CMD_SEEK_ORIGINAL;

                if (prSwdmxInst->rQRangeReq.fgIsFull
                    // && prSwdmxInst->pfnSwdmxRangeCb
                    && prSwdmxInst->rQRangeReq.prDmxRangeInfo)
                {
                    LOG(5, "Instance %d Callback for QRange Abort id = 0x%08x\n",
                        u1SrcId,
                        prSwdmxInst->rQRangeReq.prDmxRangeInfo->ui4_range_id);

                    _SWDMX_RANGE_CALLBACK(prSwdmxInst->rQRangeReq.prDmxRangeInfo->pv_tag,
                                                MM_RANGE_ABORT,
                                                prSwdmxInst->rQRangeReq.prDmxRangeInfo->ui4_range_id,
                                                0xFFFFFFFF,
                                                u1SrcId);
                }
                x_memset((VOID*)&prSwdmxInst->rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));

                if (prSwdmxInst->rCurRangeReq.fgIsFull
                    /*&& prSwdmxInst->pfnSwdmxRangeCb*/
                && prSwdmxInst->rCurRangeReq.prDmxRangeInfo)
                {
                    LOG(5, "Instance %d Callback for Cur Range Abort id = 0x%08x\n",
                        u1SrcId,
                        prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id);

                    _SWDMX_RANGE_CALLBACK(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->pv_tag,
                                                MM_RANGE_ABORT,
                                                prSwdmxInst->rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                                0xFFFFFFFF,
                                                u1SrcId);
                }
                x_memset((VOID*)&prSwdmxInst->rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));

                // clean Vdec thumbnail count
                VDEC_SetRenderPicCount(prSwdmxInst->u1VdecId, FALSE, 0);
                AUD_MM_Set_Dec_Fmt_Scramble(prSwdmxInst->u1AdecId, FALSE);
                AUD_MM_Set_Dec_Fmt_Conflict(prSwdmxInst->u1AdecId, FALSE);
                VDP_SetABRepeat(prSwdmxInst->u1B2rId, FALSE);
                {
                    VDP_AB_INTO_T rABInfo;
                    x_memset(&rABInfo, 0, sizeof(VDP_AB_INTO_T));
                    VDP_SetABInfo(prSwdmxInst->u1B2rId, &rABInfo);
                }
                //for normal case, not audio ES case. #ifdef FEEDER_HEADER_CACHE
                FeederSetCacheSize(prSwdmxInst->eFeederSrc, FEEDER_CACHED_MAX_READ_SIZE);

                // DTV00144297, pvr file also have this problem
                // let all format reset pcr
                //if(prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    // DTV00139410, [MT5361B_EU]AV can not sync when do timeshift at HD channel
                    // dmx use the same micro code with DTV.
                    // so dmx would not call dmx_reset -> dmx_pcrinit
                    // STC might be stopped if our last control is pause.
                    // dmx_pcrinit will start stc
                    if(prSwdmxInst->u1StcId == 0)
                    {
                        DMX_PcrInit();
                    }
                }
            }

            if ((u4PlayCmd == eSWDMX_STATE_STOP) ||
                    (u4PlayCmd == eSWDMX_STATE_PAUSE))
            {
                if (prSwdmxInst->pfSwdmxNfyFct)
                {
                    prSwdmxInst->pfSwdmxNfyFct(prSwdmxInst->pvSwdmxNfyTag, SWDMX_COND_CTRL_DONE, u1SrcId, 0);
                }
            }

            //stop command will send "feeder invalid" msg each time
            //no matter what current status is, so do flush while receiving stop.
            if (u4PlayCmd == (UINT32)eSWDMX_STATE_STOP)
            {
                _SWDMX_FlushReqMsgQ(u1SrcId);
            }

            // update status if receive command successfully
            prSwdmxInst->eSwDmxPrevousStatus = prSwdmxInst->eSwDmxStatus;
            UNUSED(prSwdmxInst->eSwDmxPrevousStatus);
            prSwdmxInst->eSwDmxStatus = (ENUM_SWDMX_PLAY_STATE_T)u4PlayCmd;
            //LOG(0,"prSwdmxInst->eSwDmxStatus :%d\n",prSwdmxInst->eSwDmxStatus);
            continue;
        }

        if (prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PLAY)
        {
            _SWDMX_DeliverData(u1SrcId);
        }
    }
}


//-----------------------------------------------------------------------------
/** _SWDMX_Cfg
 *  Config some function pointers for different file format
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_Cfg(UINT8 u1SrcId)
{
    BOOL fgRet= TRUE;
    DMX_DECODER_CALLBACKS_T rDmxCallback;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    if(u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(1, "_SWDMX_Cfg (u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return FALSE;
    }

    _SWDMX_ResetSwdmxInfo(u1SrcId);

    prSwdmxInst->pfnSwdmxInit = NULL;
    prSwdmxInst->pfnSwdmxPlay = NULL;
    prSwdmxInst->pfnSwdmxPause = NULL;
    prSwdmxInst->pfnSwdmxAbort = NULL;
    prSwdmxInst->pfnSwdmxGetStreamInfo = NULL;
    prSwdmxInst->pfnSwdmxSetStrmID = NULL;
    prSwdmxInst->pfnSwdmxEnableStrm = NULL;
    prSwdmxInst->pfnSwdmxDisableStrm = NULL;
    prSwdmxInst->pfnSwdmxSetRange = NULL;
    prSwdmxInst->pfnSwdmxHdrParse = NULL;
    prSwdmxInst->pfnSwdmxStop = NULL;
    prSwdmxInst->pfnSwdmxFlush = NULL;
    prSwdmxInst->pfnSwdmxSetInfo = NULL;
    prSwdmxInst->pfnSwdmxGetInfo = NULL;
    prSwdmxInst->pfnSwdmxRangeCb = NULL;
    prSwdmxInst->pfnSwdmxRangeCbEx = NULL;
    prSwdmxInst->pfnSwdmxReadBuf = NULL;
    prSwdmxInst->pfnSwdmxReleaseBuf = NULL;
    prSwdmxInst->pfnSwdmxAudioCb = NULL;
    prSwdmxInst->pfnSwdmxSendVideoPes = NULL;
    prSwdmxInst->pfnSwdmxSendAudioPes = NULL;
    prSwdmxInst->fgFifoFull = FALSE;
    LOG(5, "_SWDMX_Cfg(%d) File format = %d\n", u1SrcId, prSwdmxInst->eCurFMT);

    switch (prSwdmxInst->eCurFMT)
    {
#ifndef CC_FLV_DISABLE
    case SWDMX_FMT_FLV:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_FlvInit;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_FlvSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_FlvEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_FlvDisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_FlvSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_FlvHdrParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_FlvStop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_FlvSetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_FlvGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_FlvAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_FlvVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = AUD_SendAudioPes;
        prSwdmxInst->pfnSwdmxAbort=_SWDMX_FlvAbort;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_FlvFlush;
        break;
#endif
#ifndef CC_AVI_DISABLE
    case SWDMX_FMT_AVI:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_AVIInit;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_AVISetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_AVIEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_AVIDisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_AVISetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_AVIHdrParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_AVIStop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_AVISetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_AVIGetInfo;
        prSwdmxInst->pfnSwdmxReadBuf = _SWDMX_AviReadBuffer;
        prSwdmxInst->pfnSwdmxReleaseBuf = _SWDMX_AviReleaseBuffer;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_AviAudioDrvCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_AviVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_AviAudioCallback;
        break;
#endif
#ifndef CC_OGG_DISABLE
    case SWDMX_FMT_OGG:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_OggInit;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_OggSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_OggEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_OggDisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_OggSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_OggHdrPrs;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_OggStop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_OggSetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_OggGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_OggAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_OggVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_OggAudioCallback;
        prSwdmxInst->pfnSwdmxAbort =_SWDMX_OggAbort;
        break;
#endif

#ifndef CC_WMV_DISABLE
    case SWDMX_FMT_VC1_WMV:
    case SWDMX_FMT_WMA_AUDIO_ES:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);

        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_WMVSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_WMVEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_WMVDisableStrm;

        prSwdmxInst->pfnSwdmxInit = _SWDMX_WMVInit;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_WMVSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_WMVHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_WMVStop;
        prSwdmxInst->pfnSwdmxAbort=_SWDMX_WMVAbort;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_WMVSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_WMVGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_WMVAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_WmvVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_WmvAudioCallback;
        break;
#endif
#ifndef CC_PS_DISABLE
    case SWDMX_FMT_MPEG1_DAT:
    case SWDMX_FMT_MPEG2_PS:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_PS);

        prSwdmxInst->pfnSwdmxInit = _SWDMX_MpgPsInit;
        prSwdmxInst->pfnSwdmxPlay = _SWDMX_MpgPlay;
        prSwdmxInst->pfnSwdmxPause = _SWDMX_MpgPause;
        prSwdmxInst->pfnSwdmxAbort = _SWDMX_MpgAbort;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        prSwdmxInst->pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_MpgStop;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_MpgFlush;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_MpgVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_MpgAudioCallback;
        _SWDMX_MpgSetSwdmxInfo(u1SrcId, prSwdmxInst);
        break;
#endif
#ifndef CC_TS_DISABLE
    case SWDMX_FMT_MPEG2_TS:
    case SWDMX_FMT_MPEG2_TS_192:
    case SWDMX_FMT_MPEG2_TS_ZERO_192:
    case SWDMX_FMT_MPEG2_TS_ENCRYPT_192:
    case SWDMX_FMT_MPEG2_TS_TIME_SHIFT:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_TS);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_MpgTsInit;
        prSwdmxInst->pfnSwdmxPlay = _SWDMX_MpgPlay;
        prSwdmxInst->pfnSwdmxPause = _SWDMX_MpgPause;
        prSwdmxInst->pfnSwdmxAbort = _SWDMX_MpgAbort;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        prSwdmxInst->pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_MpgStop;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_MpgFlush;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_MpgVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_MpgAudioCallback;
        _SWDMX_MpgSetSwdmxInfo(u1SrcId, prSwdmxInst);
        break;
#endif
#ifndef CC_ES_DISABLE
#ifndef CC_ES_MPEG2_DISABLE
    case SWDMX_FMT_MPEG2_VIDEO_ES:
#endif
#ifndef CC_ES_MPEG4_DISABLE
    case SWDMX_FMT_MPEG4_VIDEO_ES:
#endif
//#ifndef CC_ES_VC1_DISABLE              //for VC1 ES file, when mw setVideo type , we notify "codec not support " to mw DTV00148208
    case SWDMX_FMT_VC1_ES:
//#endif
#ifndef CC_ES_AVS_DISABLE
    case SWDMX_FMT_AVS_VIDEO_ES:
#endif
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_ES);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_MpgEsInit;
        prSwdmxInst->pfnSwdmxPlay = _SWDMX_MpgPlay;
        prSwdmxInst->pfnSwdmxPause = _SWDMX_MpgPause;
        prSwdmxInst->pfnSwdmxAbort = _SWDMX_MpgAbort;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        prSwdmxInst->pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_MpgStop;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_MpgFlush;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_MpgVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_MpgAudioCallback;
        _SWDMX_MpgSetSwdmxInfo(u1SrcId, prSwdmxInst);
        break;
#ifdef CC_FLASH_TEST
    case SWDMX_FMT_ES_WITH_PTS:
        prSwdmxInst->pfnSwdmxInit = _SWDMX_FlashEsInit;
        prSwdmxInst->pfnSwdmxPlay = _SWDMX_FlashPlay;
        prSwdmxInst->pfnSwdmxPause = _SWDMX_FlashPause;
        prSwdmxInst->pfnSwdmxAbort = _SWDMX_FlashAbort;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_FlashSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_FlashEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_FlashDisableStrm;
        prSwdmxInst->pfnSwdmxGetStreamInfo = _SWDMX_FlashGetStreamInfo;
        prSwdmxInst->pfnSwdmxSetRange = NULL;//_SWDMX_FlashSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_FlashHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_FlashStop;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_FlashFlush;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_FlashSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_FlashGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_FlashAudioCb;
        _SWDMX_FlashSetSwdmxInfo(u1SrcId, prSwdmxInst);
        break;
#endif
#ifndef CC_ES_H264_DISABLE
    case SWDMX_FMT_H264_VIDEO_ES:
#endif
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_ES);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_MpgEsInit;
        prSwdmxInst->pfnSwdmxPlay = _SWDMX_MpgPlay;
        prSwdmxInst->pfnSwdmxPause = _SWDMX_MpgPause;
        prSwdmxInst->pfnSwdmxAbort = _SWDMX_MpgAbort;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        prSwdmxInst->pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_MpgStop;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_MpgFlush;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_MpgVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_MpgAudioCallback;
        _SWDMX_MpgSetSwdmxInfo(u1SrcId, prSwdmxInst);
        break;
    case SWDMX_FMT_RAW_AUDIO_ES:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_MpgEsInit;
        prSwdmxInst->pfnSwdmxPlay = _SWDMX_MpgPlay;
        prSwdmxInst->pfnSwdmxPause = _SWDMX_MpgPause;
        prSwdmxInst->pfnSwdmxAbort = _SWDMX_MpgAbort;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        prSwdmxInst->pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_MpgStop;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_MpgFlush;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_MpgVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_MpgAudioCallback;
        _SWDMX_MpgSetSwdmxInfo(u1SrcId, prSwdmxInst);
        break;
#endif
#ifndef CC_MP4_DISABLE
    case SWDMX_FMT_MP4:
    case SWDMX_FMT_M4A:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_Mp4Init;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_Mp4SetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_Mp4EnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_Mp4DisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_Mp4SetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_Mp4HdrPrs;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_Mp4Stop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_Mp4SetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_Mp4GetInfo;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_Mp4AudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_Mp4VideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_Mp4AudioCallback;
        break;
#endif
#ifndef CC_MKV_DISABLE
    case SWDMX_FMT_MKV:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_MKVInit;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_MKVSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_MKVEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_MKVDisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_MKVSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_MKVHdrParse;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_MKVStop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_MKVSetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_MKVGetInfo;
        prSwdmxInst->pfnSwdmxFlush = _SWDMX_MKVFlush;
        //prSwdmxInst->pfnSwdmxAbort = _SWDMX_MKVFlush;
        prSwdmxInst->pfnSwdmxReadBuf = _SWDMX_MkvReadBuffer;
        prSwdmxInst->pfnSwdmxReleaseBuf = _SWDMX_MkvReleaseBuffer;
        prSwdmxInst->pfnSwdmxAbort = NULL;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_MkvAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_MkvVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = _SWDMX_MkvAudioCallback;
        break;
#endif
#ifndef CC_RM_DISABLE
    case SWDMX_FMT_RM:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_RmInit;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_RmSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_RmEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_RmDisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_RmSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_RmHdrPrs;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_RmStop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_RmSetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_RmGetInfo;
        //prSwdmxInst->pfnSwdmxFlush = _SWDMX_RmFlush;
        //prSwdmxInst->pfnSwdmxAbort = _SWDMX_RmFlush;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_RmAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_RmVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = AUD_SendAudioPes;//_SWDMX_RmAudioCallback;
        break;
#endif
#ifndef CC_CMPB_DISABLE
    case SWDMX_FMT_CMPB:
        fgRet = DMX_MUL_SetInstType(prSwdmxInst->u1DmxId, DMX_IN_PLAYBACK_MM);
        prSwdmxInst->pfnSwdmxInit = _SWDMX_CmpbInit;
        prSwdmxInst->pfnSwdmxSetStrmID = _SWDMX_CmpbSetStrmID;
        prSwdmxInst->pfnSwdmxEnableStrm = _SWDMX_CmpbEnableStrm;
        prSwdmxInst->pfnSwdmxDisableStrm = _SWDMX_CmpbDisableStrm;
        prSwdmxInst->pfnSwdmxSetRange = _SWDMX_CmpbSetRange;
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_CmpbHdrPrs;
        prSwdmxInst->pfnSwdmxStop = _SWDMX_CmpbStop;
        prSwdmxInst->pfnSwdmxSetInfo = _SWDMX_CmpbSetDecoderInfo;
        prSwdmxInst->pfnSwdmxGetInfo = _SWDMX_CmpbGetInfo;
        //prSwdmxInst->pfnSwdmxFlush = _SWDMX_CmpbFlush;
        //prSwdmxInst->pfnSwdmxAbort = _SWDMX_CmpbFlush;
        prSwdmxInst->pfnSwdmxAudioCb = _SWDMX_CmpbAudioCb;
        prSwdmxInst->pfnSwdmxSendVideoPes = _SWDMX_CmpbVideoCallback;
        prSwdmxInst->pfnSwdmxSendAudioPes = AUD_SendAudioPes;
        break;
#endif

    default:
        LOG(0, "_SWDMX_Cfg false\n");
        return FALSE;
    }
    if(!fgRet)
    {
        LOG(3, "DMX_MUL_SetInstType fail\n");
        return fgRet;
    }



    DMX_SetToDecoder(TRUE);
    x_memset(&rDmxCallback, 0, sizeof(rDmxCallback));
    rDmxCallback.pfnSendAudioPes = _SWDMX_AudioCallback;
    rDmxCallback.pfnSendVideoPes = _SWDMX_VideoCallback;
    rDmxCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    fgRet = DMX_MUL_SetDecoderCallbacks(prSwdmxInst->u1DmxId, &rDmxCallback);
    if(!fgRet)
    {
        LOG(3, "DMX_MUL_SetDecoderCallbacks fail\n");
        return fgRet;
    }

    prSwdmxInst->ePlayMode=SWDMX_PLAY_MODE_NORMAL;
    prSwdmxInst->ePrePlayMode=SWDMX_PLAY_MODE_NORMAL;
    VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_CONTAINER_TYPE,(UINT32)(prSwdmxInst->eCurFMT),0,0);
    VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_STREAM_TYPE,(UINT32)VDEC_STREAM_TYPE_IPB,0,0);
    prSwdmxInst->pfnSwdmxInit(u1SrcId, prSwdmxInst->eCurFMT);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SWDMX_DmxReqDataNotify
 *  Callback from the demuxer for reqesting video data
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DmxReqDataNotify(UINT8 u1SrcId, UINT8 u1Idx)
{
    INT32 i4Ret;
    UINT32 u4ReqType = u1Idx;
    SIZE_T zMsgSize = sizeof(UINT32);
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    Printf("_SWDMX_DmxReqDataNotify\n");

    i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &u4ReqType, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        //LOG(0, "SWDMX Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &u4ReqType, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_Init
 *  Initialize software program stream demux driver
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Init(VOID)
{
    UINT8 i = 0;
    INT32 i4Ret = OSR_OK;
    char acBuf[32];
    SWDMX_INFO_T *prSwdmxInfo;
    if (!_fgSwdmxInitialized)
    {
        _fgSwdmxInitialized = TRUE;
        LOG(7, "_SWDMX_Init()\n");

        ASSERT(sizeof(FBM_PIC_EXTRA_INFO_T) == sizeof(VDEC_PIC_EXTRA_INFO_T));
        ASSERT(sizeof(FBM_PIC_EXTRA_INFO_T) == sizeof(VDP_PIC_EXTRA_INTO_T));

        _u4APreFIFO[0] = 0;
        _u4APreFIFO[1] = 0;

        for (i = 0; i < SWDMX_SOURCE_MAX; i++)
        {
            prSwdmxInfo = _arSwdmxInfo;
            prSwdmxInfo += (UINT32)i;
            x_memset((void*)prSwdmxInfo, 0, sizeof(SWDMX_INFO_T));

            //if (_arSwdmxInfo[i].fgSwDmxInit == FALSE)
            {
                x_memset(acBuf, 0, 32 * sizeof(char));
                x_snprintf(acBuf, sizeof(acBuf), "SWDMX_FEDQ %d", i);
                // create data queue
                i4Ret = x_msg_q_create(&_arSwdmxInfo[i].hDmxFeederReqDataQ,
                    acBuf,
                    sizeof(FEEDER_REQ_DATA_T),
                    SWDMX_DATA_Q_SIZE);
                VERIFY(i4Ret == OSR_OK);

                x_memset(acBuf, 0, 32 * sizeof(char));
                x_snprintf(acBuf, sizeof(acBuf), "SWDMX_PLAYCMDQ %d", i);
                // create play command queue
                VERIFY(x_msg_q_create(&_arSwdmxInfo[i].hDmxPlayCmdQ,
                    acBuf,
                    sizeof(UINT32),
                    SWDMX_DATA_Q_SIZE) == OSR_OK);

                x_memset(acBuf, 0, 32 * sizeof(char));
                x_snprintf(acBuf, sizeof(acBuf), "SWDMX %d", i);
                // create software demux main loop thread for command receiving
                VERIFY(x_thread_create(&_arSwdmxInfo[i].hSwDmxMainThread,
                    acBuf,
                    SWDMX_THREAD_STACK_SIZE,
                    SWDMX_THREAD_PRIORITY,
                    (x_os_thread_main_fct)_SWDMX_MainLoop, sizeof(UINT8), (VOID*)&i) == OSR_OK);

                x_thread_set_affinity(_arSwdmxInfo[i].hSwDmxMainThread, 1);

                VERIFY(x_sema_create(&_arSwdmxInfo[i].hRangeMutex,
                    X_SEMA_TYPE_MUTEX,
                    X_SEMA_STATE_UNLOCK) == OSR_OK);

                VERIFY(x_sema_create(&_arSwdmxInfo[i].hFlushMutex,
                    X_SEMA_TYPE_MUTEX,
                    X_SEMA_STATE_UNLOCK) == OSR_OK);

                VERIFY(x_sema_create(&_arSwdmxInfo[i].hFlushSema,
                    X_SEMA_TYPE_BINARY,
                    X_SEMA_STATE_LOCK) == OSR_OK);

                VERIFY(x_sema_create(&_arSwdmxInfo[i].hRangeSema,       //for time-shift
                    X_SEMA_TYPE_BINARY,
                    X_SEMA_STATE_UNLOCK) == OSR_OK);

                VERIFY(x_timer_create(&_arSwdmxInfo[i].hWaitDataTimer) == OSR_OK);

                //#ifdef CC_DLNA_SUPPORT
                //VERIFY(x_sema_create(&_hFeederUnderFlowMutex, X_SEMA_TYPE_MUTEX,
                //    X_SEMA_STATE_UNLOCK) == OSR_OK);
                //#endif
                if (i == SWDMX_PRIMARY_SOURCE)
                {
#if 0//def CC_DLNA_SUPPORT

                    FeederSetInfo(FEEDER_PROGRAM_SOURCE,
                        FEEDER_SWDMX_UNDERFLOW_CB,
                        (UINT32)_SWDMX_FeederUnderFlowCb);

                    FeederSetInfo(FEEDER_PROGRAM_SOURCE,
                        FEEDER_SWDMX_STATUS_CB,
                        (UINT32)_SWDMX_FeederStatusNfyCb);

                    FeederSetInfo(FEEDER_PROGRAM_SOURCE,
                        FEEDER_SWDMX_INSTANCE,
                        i);
#endif
                    _arSwdmxInfo[i].eFeederSrc = FEEDER_PROGRAM_SOURCE;

                }
                else if(i == SWDMX_SECONDARY_SOURCE)
                {
#if 0//def CC_DLNA_SUPPORT
                    FeederSetInfo(FEEDER_AUDIO_SOURCE,
                        FEEDER_SWDMX_UNDERFLOW_CB,
                        (UINT32)_SWDMX_FeederUnderFlowCb);

                    FeederSetInfo(FEEDER_AUDIO_SOURCE,
                        FEEDER_SWDMX_STATUS_CB,
                        (UINT32)_SWDMX_FeederStatusNfyCb);

                    FeederSetInfo(FEEDER_AUDIO_SOURCE,
                        FEEDER_SWDMX_INSTANCE,
                        i);
#endif
                    _arSwdmxInfo[i].eFeederSrc = FEEDER_AUDIO_SOURCE;

                }

                x_memset(acBuf, 0, 32 * sizeof(char));

                _arSwdmxInfo[i].u1SwdmxId = i;
                _arSwdmxInfo[i].u1DmxId = i;     // DTV and MM use 0
                _arSwdmxInfo[i].i4PlaySpeed = SWDMX_PLAY_SPEED_1X;
                _arSwdmxInfo[i].u8SeekPos = 0xFFFFFFFF;
                _arSwdmxInfo[i].u8SeekPos <<= 32;
                _arSwdmxInfo[i].u8SeekPos += 0xFFFFFFFF;
                //_arSwdmxInfo[i].fgSwDmxInit = TRUE;
                _arSwdmxInfo[i].fgRcvEOS = FALSE;
                _arSwdmxInfo[i].fgDataExhausted = TRUE;
                _arSwdmxInfo[i].u4DataUnderFlowCount = 0;
                _arSwdmxInfo[i].fgWaitDataTimerStart = FALSE;
                _arSwdmxInfo[i].fgSendUnderflow2MW=FALSE;
                _arSwdmxInfo[i].fgValid = TRUE;
                _arSwdmxInfo[i].rCmd.i4PlaySpeed = SWDMX_PLAY_SPEED_1X;
                _arSwdmxInfo[i].pucAudBuf =
                    (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_AUDIO_ALLOC_SIZE, 16));

                _arSwdmxInfo[i].u4SeekTime = 0;

                if(!_arSwdmxInfo[i].pucAudBuf)
                {
                    LOG(3, "%s Fail to allocate swdmx audio buffer\n", __FUNCTION__);
                    ASSERT(0);
                }
                if (!_arSwdmxInfo[i].pucSwdmxInternalBuf)
                {
                    _arSwdmxInfo[i].pucSwdmxInternalBuf =
                        (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_INTERNAL_BUF_SIZE,16));
                    if (!_arSwdmxInfo[i].pucSwdmxInternalBuf)
                    {
                        LOG(3, "%s Fail to allocate swdmx internal buffer\n", __FUNCTION__);
                        ASSERT(0);
                    }
                }
            }
        }

#if defined(CC_FBM_SUPPORT_SWDMX)
        {
            FBM_POOL_T *prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_SWDMX);
            ASSERT(prFbmPool != NULL);
            ASSERT(prFbmPool->u4Addr);
            ASSERT(prFbmPool->u4Size);
            _u4APreFIFO[0] = VIRTUAL(prFbmPool->u4Addr);
            _u4APreFIFO[1]= _u4APreFIFO[0] + prFbmPool->u4Size;
            _u4APreFIFOSize = prFbmPool->u4Size;
            LOG(0,"FBM_POOL_TYPE_SWDMX: 0x%08x, %d\n", prFbmPool->u4Addr, prFbmPool->u4Size);
            LOG(0,"_u4APreFIFO: 0x%08x--0x%08x, %d\n", _u4APreFIFO[0], _u4APreFIFO[1], _u4APreFIFOSize);
        }
#else
        _u4APreFIFO[0] = VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_AUDIO_PREBUF_SIZE,16));

        if(_u4APreFIFO[0] == 0)
        {
            LOG(0,"Fail to allocate audio prebuffer\n");
            ASSERT(0);
        }
        else
        {
            _u4APreFIFO[1]= _u4APreFIFO[0] + SWDMX_AUDIO_PREBUF_SIZE;
            _u4APreFIFOSize = SWDMX_AUDIO_PREBUF_SIZE;
        }
#endif

        VERIFY(x_sema_create(&_rSwdmxKeepInfo.hSwdmxInfoMutex,
            X_SEMA_TYPE_MUTEX,
            X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY(x_sema_create(&_rSwdmxKeepInfo.hSwdmxMoveSema,
            X_SEMA_TYPE_MUTEX,
            X_SEMA_STATE_UNLOCK) == OSR_OK);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_GetStreamInfo
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_GetStreamInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
                          UINT32 u4InfoMask,
                          UINT64 u8FileSize,
                          SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet = FALSE;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->eCurFMT = eType;
    prSwdmxInst->u8FileSize = u8FileSize;

    _SWDMX_Cfg(u1SrcId);

    if (prSwdmxInst->pfnSwdmxGetStreamInfo)
    {
        fgRet = prSwdmxInst->pfnSwdmxGetStreamInfo(u1SrcId, eType, u4InfoMask, u8FileSize, prPInfo);
        if (prPInfo->rPgm[0].u4TotalTime)
        {
            prSwdmxInst->u4TotalTime = prPInfo->rPgm[0].u4TotalTime;
        }
    }
    return fgRet;
}

//-----------------------------------------------------------------------------
/** _SWDMX_SetContainerType
 *  Set the container type to SwDmx
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetContainerType(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eContainerType)
{
    BOOL fgRet;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    DMX_MUL_EnableMultipleInst(TRUE);

    prSwdmxInst->eCurFMT = eContainerType;
    fgRet = _SWDMX_Cfg(u1SrcId);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_Cfg fail\n");
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Set_Play_Speed
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetSpeed(UINT8 u1SrcId, INT32 i4Speed)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (i4Speed > 1024000 || i4Speed < -1024000)
    {
        LOG(3, "_SWDMX_SetSpeed not in 1024000 -1024000\n");
        return FALSE;
    }

    // not need new trick mode format.
    if(prSwdmxInst->eCurFMT==SWDMX_FMT_OGG
#ifndef CC_PS_DISABLE
        ||prSwdmxInst->pfnSwdmxInit==_SWDMX_MpgPsInit
#endif
#ifndef CC_TS_DISABLE
        ||prSwdmxInst->pfnSwdmxInit==_SWDMX_MpgTsInit
#endif
#ifndef CC_ES_DISABLE
        ||prSwdmxInst->pfnSwdmxInit==_SWDMX_MpgEsInit
#endif
#ifndef  CC_CMPB_DISABLE
        ||prSwdmxInst->pfnSwdmxInit==_SWDMX_CmpbInit
#endif
        )
    {
        if(prSwdmxInst->i4PlaySpeed != i4Speed || i4Speed == -1)
        {
            prSwdmxInst->i4PlaySpeed = i4Speed;
            prSwdmxInst->fgPlayChange = TRUE;
            if(i4Speed>2000 || i4Speed<0)
            {
                VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_STREAM_TYPE,(UINT32)VDEC_STREAM_TYPE_I,0,0);
            }
            else
            {
                VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_STREAM_TYPE,(UINT32)VDEC_STREAM_TYPE_IPB,0,0);
            }
        }
    }
    else if((prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL&& prSwdmxInst->fgEnVideoDmx)
        &&(prSwdmxInst->i4PlaySpeed != i4Speed)
        &&(i4Speed<=2000))
    {
        LOG(5, "Instance %d _SWDMX_SetSpeed from %d to %d,PlayMode %d\n", u1SrcId, prSwdmxInst->i4PlaySpeed, i4Speed,prSwdmxInst->ePlayMode);
        //Printf("******>>_SWDMX_SetSpeed(0):%d-->%d\n",prSwdmxInst->i4PlaySpeed,i4Speed);
        prSwdmxInst->i4PlaySpeed = i4Speed;
        prSwdmxInst->fgPlayChange = TRUE;
    }
    else if(prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME
        && (prSwdmxInst->i4PlaySpeed != i4Speed || i4Speed == -1))
    {
       // swdmx have to know -1 speed again, or we don't know it's new backstep.
        LOG(5, "Instance %d _SWDMX_SetSpeed from %d to %d,PlayMode %d\n", u1SrcId, prSwdmxInst->i4PlaySpeed, i4Speed,prSwdmxInst->ePlayMode);
        //Printf("******>>_SWDMX_SetSpeed(1):%d-->%d\n",prSwdmxInst->i4PlaySpeed,i4Speed);
        prSwdmxInst->i4PlaySpeed = i4Speed;
        prSwdmxInst->fgPlayChange = TRUE;
    }
    else
    {
        //Printf("******>>_SWDMX_SetSpeed(2):%d-->%d\n",prSwdmxInst->i4PlaySpeed,i4Speed);
        prSwdmxInst->i4PlaySpeed = i4Speed;
        //prSwdmxInst->fgPlayChange=FALSE;
        LOG(5, "Instance %d _SWDMX_SetSpeed from %d to %d,PlayMode %d\n", u1SrcId, prSwdmxInst->i4PlaySpeed, i4Speed,prSwdmxInst->ePlayMode);
    }
    return TRUE;
}

BOOL _SWDMX_SetPlayMode(UINT8 u1SrcId, SWDMX_PLAY_MODE_T ePlayMode)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "Instance %d _SWDMX_SetPlaymode from %d to %d\n", u1SrcId, prSwdmxInst->ePlayMode,ePlayMode);
    Printf("******>>_SWDMX_SetPlayMode:%d-->%d\n",prSwdmxInst->ePlayMode,ePlayMode);
    if(prSwdmxInst->ePlayMode!=ePlayMode)
    {
       prSwdmxInst->fgPlayChange = TRUE;
    }

    prSwdmxInst->ePlayMode=ePlayMode;
    if(ePlayMode==SWDMX_PLAY_MODE_NORMAL)
    {
        VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_STREAM_TYPE,(UINT32)VDEC_STREAM_TYPE_IPB,0,0);
    }
    else
    {
        VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_STREAM_TYPE,(UINT32)VDEC_STREAM_TYPE_I,0,0);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SeekTime
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime, UINT64* pu8FilePos)
{
    UINT64 u8Remainder;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->fgPlayChange = TRUE;
    prSwdmxInst->fgPlayChangeOffset = FALSE;
    prSwdmxInst->u4SeekTime = u4SeekTime;

    if(pu8FilePos == NULL)
    {
        LOG(3, "_SWDMX_SeekTime pu8FilePos NULL\n");
        prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
        return FALSE;
    }

    #if 0
    // we don't have file size for time-shift
    if ((prSwdmxInst->eCurFMT != SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
        (u4SeekTime > prSwdmxInst->u4TotalTime)&&(prSwdmxInst->eCurFMT != SWDMX_FMT_MPEG2_TS))
    {
        LOG(3, "_SWDMX_SeekTime u4SeekTime > u4TotalTime\n");
        prSwdmxInst->u8SeekPos = prSwdmxInst->u4TotalTime;
        return FALSE;
    }
    // we don't have file size for time-shift
    if ((u4SeekTime > prSwdmxInst->u4TotalTime)&&(prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS))

    {
         prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
         return TRUE;
    }
    #endif


    *pu8FilePos = 0;
    // to decide time -> pos.
    if ((prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG1_DAT) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_PS) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_192) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_H264_VIDEO_ES) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_ES_WITH_PTS) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_VC1_ES))
    {
        if (prSwdmxInst->u4TotalTime > 0)
        {
            *pu8FilePos = _SWDMX_Div6464((prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset)*(u4SeekTime),
                                    (UINT64)(prSwdmxInst->u4TotalTime), &u8Remainder);
        }
        else
        {
            *pu8FilePos = 0;
        }
        *pu8FilePos += prSwdmxInst->u8FileOffset;
        VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->u8FileSize);// PTS
        prSwdmxInst->u8SeekPos = *pu8FilePos;
        if ((prSwdmxInst->u4TotalTime > 0) && (u4SeekTime > prSwdmxInst->u4TotalTime))
        {
            LOG(3, "_SWDMX_SeekTime u4SeekTime > u4TotalTime\n");
            prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
        }

        if(!prSwdmxInst->fgEnVideoDmx) //for audio only file, when do seek after pause ,needed set STC Value to updata time code
        {
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u4SeekTime);
        }
    }
    else if (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        // u4SeekTime is tick
        //if(_SWDMX_MpgGetPosByTick(FALSE, u4SeekTime, pu8FilePos))
        if(_SWDMX_MpgGetPosByTick(u1SrcId, TRUE, u4SeekTime, pu8FilePos))
        {
            prSwdmxInst->u8SeekPos = *pu8FilePos;
            if(!_SWDMX_MpgIsPositionAvailable(u1SrcId, prSwdmxInst->u8SeekPos))
            {
                LOG(3, "_SWDMX_SeekTime _SWDMX_MpgIsPositionAvailable fail\n");
                prSwdmxInst->fgPlayChange = FALSE;
                return FALSE;
            }
        }
        else
        {
            LOG(3, "_SWDMX_SeekTime _SWDMX_MpgGetPosByTick fail\n");
            prSwdmxInst->fgPlayChange = FALSE;
            return FALSE;
        }
    }
    else if (prSwdmxInst->eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
    {
        if(_SWDMX_MpgGetPosByTime(u1SrcId, TRUE, u4SeekTime, pu8FilePos))
        {
        }
        else
        {
            if (prSwdmxInst->u4TotalTime > 0)
            {
                *pu8FilePos = _SWDMX_Div6464((prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset)*(u4SeekTime),
                                        (UINT64)(prSwdmxInst->u4TotalTime), &u8Remainder);
            }
            else
            {
                *pu8FilePos = 0;
            }
            *pu8FilePos += prSwdmxInst->u8FileOffset;
        }
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, u4SeekTime);
        prSwdmxInst->u8SeekPos = *pu8FilePos;
        if ((prSwdmxInst->u4TotalTime > 0) && (u4SeekTime > prSwdmxInst->u4TotalTime))
        {
            LOG(3, "_SWDMX_SeekTime u4SeekTime > u4TotalTime\n");
            prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
        }
    }
    else
    {
        //for audio only file, when do seek after pause ,needed set STC Value to updata time code
        if(!prSwdmxInst->fgEnVideoDmx)
        {
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, u4SeekTime);
        }
        prSwdmxInst->u8SeekPos = u4SeekTime;
        if ((prSwdmxInst->u4TotalTime > 0) && (u4SeekTime > prSwdmxInst->u4TotalTime))
        {
            LOG(3, "_SWDMX_SeekTime u4SeekTime > u4TotalTime\n");
            prSwdmxInst->u8SeekPos = prSwdmxInst->u4TotalTime;
        }
    }

    LOG(5, "_SWDMX_SeekTime(%d) u4SeekTime %d u8FilePos 0x%llx  \n", u1SrcId, u4SeekTime, (*pu8FilePos));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SeekPos
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekPos(UINT8 u1SrcId, UINT64 u8SeekPos, UINT64 u8SeekPosI)
{
    UINT32 u4SeekTime;
    UINT64 u8Remainder;

    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->fgPlayChange = TRUE;
    prSwdmxInst->fgPlayChangeOffset = FALSE;

    if((u8SeekPosI > prSwdmxInst->u8FileSize) ||
        (u8SeekPos > prSwdmxInst->u8FileSize))
    {
        LOG(3, "_SWDMX_Seek u8SeekPosI or u8SeekPos > u8FileSize\n");
        prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
        return FALSE;
    }

    if (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        prSwdmxInst->u8SeekPos = (u8SeekPos*MPG_LBA_BLOCK_SIZE);
    }
    else if (prSwdmxInst->eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
    {
        u4SeekTime = _SWDMX_Div6464(u8SeekPos*(prSwdmxInst->u4TotalTime),
                               (UINT64)(prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset), &u8Remainder);

        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, u4SeekTime);
    }
    else
    {
        VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->u8FileSize);// PTS
    }

    prSwdmxInst->u8SeekPos = u8SeekPosI;

    LOG(5, "_SWDMX_SeekPos(%d) PosI:0x%08X Pos:0x%08X\n", u1SrcId, u8SeekPosI, u8SeekPos);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SeekPos
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekOffset(UINT8 u1SrcId, VOID *prPosition)
{
    START_POS_INFO_T *prPos = (START_POS_INFO_T*)prPosition;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if(!prPos)
    {
        LOG(3, "_SWDMX_SeekOffset prPosition null\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_SeekOffset Render Pos 0x%llx\n", prPos->ui8_frame_position);
    prSwdmxInst->rSeekOffset = *prPos;
    prSwdmxInst->fgPlayChange = TRUE;
    prSwdmxInst->fgPlayChangeOffset = TRUE;
    prSwdmxInst->u8SeekPos = 0xFFFFFFFF;
    prSwdmxInst->u8SeekPos <<= 32;
    prSwdmxInst->u8SeekPos += 0xFFFFFFFF;
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SeekPercent
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent, UINT64* pu8FilePos)
{
    UINT32 u4SeekTime;
    UINT64 u8Remainder;

    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->fgPlayChange = TRUE;
    prSwdmxInst->fgPlayChangeOffset = FALSE;

    if(pu8FilePos == NULL)
    {
        LOG(3, "_SWDMX_SeekPercent pu8FilePos NULL\n");
        prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
        return FALSE;
    }

    if (u4SeekPercent > 100)
    {
        LOG(3, "_SWDMX_SeekPercent u4SeekPercent > 100, == 0\n");
        prSwdmxInst->u8SeekPos = prSwdmxInst->u8FileSize;
        return FALSE;
    }

    *pu8FilePos = 0;
    // to decide time -> pos.
    if ((prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG1_DAT) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_PS) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_192) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_H264_VIDEO_ES) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_ES_WITH_PTS) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (prSwdmxInst->eCurFMT == SWDMX_FMT_VC1_ES))
    {
        *pu8FilePos = _SWDMX_Div6464((prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset)*(u4SeekPercent),
                                (UINT64)(100), &u8Remainder);
        *pu8FilePos += prSwdmxInst->u8FileOffset;
        VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->u8FileSize);// PTS
    }
    else if(prSwdmxInst->eCurFMT == SWDMX_FMT_FLV)
    {
        *pu8FilePos = _SWDMX_Div6464((prSwdmxInst->u8FileSize)*(u4SeekPercent),
                                (UINT64)(100), &u8Remainder);
        u4SeekTime = _SWDMX_Div6464(prSwdmxInst->u4TotalTime*u4SeekPercent,100,&u8Remainder);
    }
    else if (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        // Isaac
    }
    else if (prSwdmxInst->eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
    {
        *pu8FilePos = _SWDMX_Div6464((prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset)*(u4SeekPercent),
                                (UINT64)(100), &u8Remainder);
        *pu8FilePos += prSwdmxInst->u8FileOffset;
        u4SeekTime = _SWDMX_Div6464((*pu8FilePos)*(prSwdmxInst->u4TotalTime),
                               (UINT64)(prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset), &u8Remainder);

        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, u4SeekTime);
    }
    else
    {
        // please decide your pts here.
        VDEC_SetRenderPts(prSwdmxInst->u1VdecId, 0);// PTS
    }

    LOG(5, "_SWDMX_SeekPercent(%d) %d u8FilePos -> (INT32)%d  \n", u1SrcId, u4SeekPercent, (UINT32)(*pu8FilePos));

    prSwdmxInst->u8SeekPos = *pu8FilePos;
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_RenderFromPos
 *  Render from position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_RenderFromPos(UINT8 u1SrcId, VOID *prPosition)
{
#ifdef CC_SWDMX_RENDER_POSITION
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    START_POS_INFO_T *prPos = (START_POS_INFO_T*)prPosition;
    if(!prPos)
    {
        LOG(3, "_SWDMX_RenderFromPos prPosition null\n");
        return FALSE;
    }
    LOG(5, "_SWDMX_RenderFromPos Render Pos 0x%llx\n", prPos->ui8_frame_position);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->rCurRangeReq.i4TempRef = prPos->i4_temporal_reference;
    prSwdmxInst->rCurRangeReq.u2DecodingOrder = prPos->ui2_decoding_order;
    prSwdmxInst->rCurRangeReq.u8Offset = prPos->ui8_frame_position;
    prSwdmxInst->rCurRangeReq.u8OffsetI = prPos->ui8_i_frame_position;
    prSwdmxInst->rCurRangeReq.u8StartPts = prPos->ui8_pts_info;
    prSwdmxInst->rCurRangeReq.u8StartPtsI = prPos->ui8_i_pts_info;
    prSwdmxInst->rCurRangeReq.u8StartAudPts = prPos->ui8_aud_pts_info;
    prSwdmxInst->rCurRangeReq.u8StartAudOffset = prPos->ui8_aud_frame_position;
    prSwdmxInst->rCurRangeReq.u4Timestap = prPos->ui4_timestap;
    // put fgSetRenderPosition at last one
    prSwdmxInst->rCurRangeReq.fgSetRenderPosition = TRUE;
#endif
    UNUSED(u1SrcId);
    UNUSED(prPosition);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Set_Video_StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetVideoStreamID(UINT8 u1SrcId, UINT32 u4VidStrmID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxInst->u4VidStrmID = u4VidStrmID;

    if (prSwdmxInst->pfnSwdmxSetStrmID != 0)
    {
        LOG(5, "Instance %d _SWDMX_SetVideoStreamID = %08x\n", u1SrcId, u4VidStrmID);
        return prSwdmxInst->pfnSwdmxSetStrmID(
            u1SrcId,
            eSWDMX_STRM_TYPE_VID,
            u4VidStrmID,
            0);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Set_Video2_StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetVideo2StreamID(UINT8 u1SrcId, UINT32 u4VidStrmID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxInst->u4SubVidStrmID = u4VidStrmID;

    if (prSwdmxInst->pfnSwdmxSetStrmID != 0)
    {
        LOG(5, "Instance %d _SWDMX_SetVideoStreamID = %08x\n", u1SrcId, u4VidStrmID);
        return prSwdmxInst->pfnSwdmxSetStrmID(
            u1SrcId,
            eSWDMX_STRM_TYPE_VID2,
            u4VidStrmID,
            0);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Set_Audio_StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetAudioStreamID(UINT8 u1SrcId, UINT32 u4AudStrmID, UINT32 u4AudStrmSubID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->u4AudStrmID = u4AudStrmID;
    prSwdmxInst->u4AudSubStrmID = u4AudStrmSubID;

    if (prSwdmxInst->pfnSwdmxSetStrmID != 0)
    {
        LOG(5, "Instance %d _SWDMX_SetAudioStreamID = %08x, sub = %08x\n", u1SrcId, u4AudStrmID, u4AudStrmSubID);
        return prSwdmxInst->pfnSwdmxSetStrmID(
            u1SrcId,
            eSWDMX_STRM_TYPE_AUD,
            u4AudStrmID,
            u4AudStrmSubID);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SetAudio2StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetAudio2StreamID(UINT8 u1SrcId, UINT32 u4AudStrmID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->u4Aud2StrmID = u4AudStrmID;

    if (prSwdmxInst->pfnSwdmxSetStrmID != 0)
    {
        LOG(5, "Instance %d _SWDMX_SetAudio2StreamID = %08x\n", u1SrcId, u4AudStrmID);
        return prSwdmxInst->pfnSwdmxSetStrmID(
            u1SrcId,
            eSWDMX_STRM_TYPE_AUD2,
            u4AudStrmID,
            0);
    }

    return FALSE;
}
//-----------------------------------------------------------------------------
/** _SWDMX_SetSubTitleStreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetSubTitleStreamID(UINT8 u1SrcId, UINT32 u4SubTitleStrmID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->u4SubTitleStrmID = u4SubTitleStrmID;
    if (prSwdmxInst->pfnSwdmxSetStrmID != 0)
    {
        LOG(5, "Instance %d _SWDMX_SetSubtitleStreamID = %08x\n", u1SrcId, u4SubTitleStrmID);
        return prSwdmxInst->pfnSwdmxSetStrmID(
            u1SrcId,
            eSWDMX_STRM_TYPE_SUBTITLE,
            u4SubTitleStrmID,
            0);
    }

    return TRUE;
}
//-----------------------------------------------------------------------------
/** _SWDMX_SetPcrStreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetPcrStreamID(UINT8 u1SrcId, UINT32 u4PcrStrmID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->u4PcrStrmID = u4PcrStrmID;
    if (prSwdmxInst->pfnSwdmxSetStrmID != 0)
    {
        LOG(7, "_SWDMX_SetPcrStreamID = %08x\n", u4PcrStrmID);
        return prSwdmxInst->pfnSwdmxSetStrmID(
            u1SrcId,
            eSWDMX_STRM_TYPE_PCR,
            u4PcrStrmID,
            0);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Play
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Play(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (!_fgSwdmxInitialized)
    {
        LOG(1, "_SWDMX_Play fgSwDmxInit fail\n");
    }

    LOG(5, "_SWDMX_Play(%d)\n", u1SrcId);
    _SWDMX_SendPlayCmd(u1SrcId, eSWDMX_STATE_PLAY);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Pause
 *  Pause software demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Pause(UINT8 u1SrcId)
{
    UINT32 u4Count = 0;
    INT32 i4Ret;
    UINT16 u2QueueSize;
    FEEDER_REQ_DATA_T rFeederReqData;
    SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);

    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (_fgSwdmxInitialized)
    {
        LOG(5, "_SWDMX_Pause(%d)\n", u1SrcId);

        _SWDMX_SendPlayCmd(u1SrcId, eSWDMX_STATE_PAUSE);
        prSwdmxInst->fgPreviouIsPause = TRUE;
        VERIFY(x_msg_q_num_msgs(prSwdmxInst->hDmxFeederReqDataQ, &u2QueueSize) == OSR_OK);

        if (u2QueueSize == 0)
        {
            rFeederReqData.eDataType = FEEDER_SOURCE_INVALID;
            i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
            while (i4Ret == OSR_TOO_MANY)
            {
                x_thread_delay(10);
                u4Count ++;
                if(u4Count > 20)
                {
                    break;
                }
                LOG(0, "_SWDMX_Pause Queue, OSR_TOO_MANY\n");
                i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
            }
            VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
        }

        if (prSwdmxInst->pfnSwdmxAbort)
        {
            prSwdmxInst->pfnSwdmxAbort(u1SrcId);
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_Stop
 *  Stop software PS parser
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Stop(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (_fgSwdmxInitialized)
    {
        UINT32 u4Count = 0;
        INT32 i4Ret;
        UINT16 u2MsgQIdx;
        FEEDER_REQ_DATA_T rFeederReqData;
        SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);

        LOG(5, "_SWDMX_Stop(%d)\n", u1SrcId);

        _SWDMX_SendPlayCmd(u1SrcId, eSWDMX_STATE_STOP);

        // To prevent the hang problem in wait feeder's ack,
        // so I send a dummy msg to main loop

        //if(prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PLAY)
        //{
        rFeederReqData.eDataType = FEEDER_SOURCE_INVALID;
        i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
        while (i4Ret == OSR_TOO_MANY)
        {
            x_thread_delay(10);
            u4Count ++;
            if(u4Count > 20)
            {
                break;
            }
            LOG(0, "_SWDMX_Stop Queue, OSR_TOO_MANY, eat one\n");
            i4Ret = x_msg_q_receive(&u2MsgQIdx, &rFeederReqData, &zMsgSize,
                                    &prSwdmxInst->hDmxFeederReqDataQ, 1, X_MSGQ_OPTION_NOWAIT);
            i4Ret = x_msg_q_send(prSwdmxInst->hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
        }
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
        //}

#if 0
        AUD_Mute(prSwdmxInst->u1AdecId);
        AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
        AUD_UnMute(prSwdmxInst->u1AdecId);
#endif
#if 0//def CC_DLNA_SUPPORT
        prSwdmxInst->fgFeederUnderFlow = FALSE;
#endif
        if (prSwdmxInst->pfnSwdmxAbort)
        {
            prSwdmxInst->pfnSwdmxAbort(u1SrcId);
        }

#ifdef CC_SWDMX_RENDER_PTS
        _u4RenderPts = 0;
#endif
#ifdef CC_SWDMX_RENDER_POSITION_TEST
        {
            VDP_POS_INTO_T rVdpPos;
            VDP_GetPosInfo(prSwdmxInst->u1B2rId, &rVdpPos);
            LOG(0, "Current position:\n");
            LOG(0, "( %d %d %d %d %d %d %d %d %d )\n",
                (UINT32)rVdpPos.u8AudPts,
                (UINT32)rVdpPos.u8AudOffset,
                (UINT32)rVdpPos.u8PtsI,
                (UINT32)rVdpPos.u8Pts,
                (UINT32)rVdpPos.u8OffsetI,
                (UINT32)rVdpPos.u8Offset,
                (UINT32)rVdpPos.u8OffsetDisp,
                (UINT32)rVdpPos.i4TemporalRef,
                (UINT32)rVdpPos.u2DecodingOrder);
            /*in following order:
            UINT64  ui8_aud_pts_info;
            UINT64  ui8_aud_frame_position;
            UINT64  ui8_i_pts_info;
            UINT64  ui8_pts_info;
            UINT64  ui8_i_frame_position;
            UINT64  ui8_frame_position;
            UINT64  ui8_frame_position_disp;
            INT32   i4_temporal_reference;
            UINT16  ui2_decoding_order;*/
        }

        _fgSetRenderPosition = FALSE;
        x_memset(&_rRenderPos, 0, sizeof(START_POS_INFO_T));
#endif
    }
    else
    {
        x_memset((VOID*)&prSwdmxInst->rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        x_memset((VOID*)&prSwdmxInst->rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        prSwdmxInst->u4DataUnderFlowCount = 0;
        return FALSE;
    }
    
    #ifdef SWDMX_DBG_USB
    UNUSED( Dbg_CloseUsbFile( prSwdmxInst->rDbgInfo.hFileGetCrc, &prSwdmxInst->rDbgInfo.hFileGetCrc) );
    UNUSED( Dbg_CloseUsbFile( prSwdmxInst->rDbgInfo.hFileGetCrc1, &prSwdmxInst->rDbgInfo.hFileGetCrc1) );
    UNUSED( Dbg_CloseUsbFile( prSwdmxInst->rDbgInfo.hFileSaveCrc, &prSwdmxInst->rDbgInfo.hFileSaveCrc )  );
    UNUSED( Dbg_CloseUsbFile( prSwdmxInst->rDbgInfo.hFileSaveCrc1, &prSwdmxInst->rDbgInfo.hFileSaveCrc1 ) );
    UNUSED( Dbg_CloseUsbFile( prSwdmxInst->rDbgInfo.hFileSaveData, &prSwdmxInst->rDbgInfo.hFileSaveData ) );
    UNUSED( Dbg_CloseUsbFile( prSwdmxInst->rDbgInfo.hFileSaveData1, &prSwdmxInst->rDbgInfo.hFileSaveData1 ) );
    x_memset( (VOID *)&prSwdmxInst->rDbgInfo, 0, sizeof( MM_DBG_INFO_T ) );
    #endif // SWDMX_DBG_USB
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Flush
 *  Send cmd to Flush A/V data
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Flush(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_Flush(%d)\n", u1SrcId);
    _SWDMX_LockFlushMutex(u1SrcId);
    prSwdmxInst->fgFlush = TRUE;
    _SWDMX_UnlockFlushMutex(u1SrcId);

    _SWDMX_SendPlayCmd(u1SrcId, eSWDMX_STATE_MAX);
    if (prSwdmxInst->pfnSwdmxAbort)
    {
        prSwdmxInst->pfnSwdmxAbort(u1SrcId);
    }

    _SWDMX_LockFlushSema(u1SrcId);
    LOG(5, "_SWDMX_Flush Done\n");
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_DoFlush
 *  Flush A/V data
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DoFlush(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (prSwdmxInst->fgEnVideoDmx)
    {
        // To not block decoder block by get frame buffer.
        VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

        MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);

        VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
        VDP_ResetPts();
        VDP_SetABRepeat(prSwdmxInst->u1B2rId, TRUE);
    }

#ifdef CC_3D_MM_DS_SUPPORT
    if (prSwdmxInst->fgEnVideo2Dmx)
    {
        // To not block decoder block by get frame buffer.
        VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);

        MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, FALSE);

        VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id);
        VDP_ResetPts();
        VDP_SetABRepeat(prSwdmxInst->u1B2rId, TRUE);
    }
#endif

    if (prSwdmxInst->fgEnAudioDmx)
    {
        AUD_WaitDspFlush(prSwdmxInst->u1AdecId);;
    }

    // container part flush
    if (prSwdmxInst->pfnSwdmxFlush)
    {
        prSwdmxInst->pfnSwdmxFlush(u1SrcId);
    }

    return TRUE;
}

ENUM_SWDMX_PLAY_STATE_T _SWDMX_QueryStatus(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if(!prSwdmxInst)
    {
        return eSWDMX_STATE_MAX;
    }
    return prSwdmxInst->eSwDmxStatus;
}


BOOL _SWDMX_SetRangeCb(UINT8 u1SrcId, VOID* pfnCbFunc)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxInst->pfnSwdmxRangeCb = (x_mm_range_nfy_fct)pfnCbFunc;
    return TRUE;
}

BOOL _SWDMX_SetRangeCbEx(UINT8 u1SrcId, VOID* pfnCbFunc)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxInst->pfnSwdmxRangeCbEx = (x_mm_range_nfy_fct_ex)pfnCbFunc;
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_Add_Range
 *  Add the demux range
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_AddRange(UINT8 u1SrcId, MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    LOG(11, "Enter _SWDMX_AddRange\n");

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    VERIFY(x_sema_lock(prSwdmxInst->hRangeMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (!prDmxRange)
    {
        LOG(3, "_SWDMX_AddRange Range is NULL\n");
        VERIFY(x_sema_unlock(prSwdmxInst->hRangeMutex) == OSR_OK);
        return FALSE;
    }

    //set whole file total duration
    if (prDmxRange->ui8_pb_duration > 0)
    {
        prSwdmxInst->u4TotalTime = (UINT32)prDmxRange->ui8_pb_duration;
    }

    if (prDmxRange->z_range_sz > 0)
    {
        prSwdmxInst->u8FileSize = (prDmxRange->z_range_sz + prDmxRange->ui8_fileoffset);
        prSwdmxInst->u8RangeSize = prDmxRange->z_range_sz;
    }

#ifdef CC_SWDMX_RENDER_PTS
    prDmxRange->t_vid_start_render_pts = _u4RenderPts;
#endif

    prSwdmxInst->u8FileOffset = prDmxRange->ui8_fileoffset;
    if(prSwdmxInst->u8FileOffset >= prSwdmxInst->u8FileSize)
    {
        if(prSwdmxInst->u8FileSize > 0)
        {
            prSwdmxInst->u8FileOffset = (prSwdmxInst->u8FileSize - 1);
        }
        else
        {
            prSwdmxInst->u8FileOffset = 0;
        }
        LOG(3, "ui8_fileoffset >= z_range_sz\n");
    }

    if ((prSwdmxInst->rCurRangeReq.fgIsFull) &&
            (prSwdmxInst->rQRangeReq.fgIsFull))
    {
        LOG(5, "_SWDMX_AddRange Range is full\n");
        VERIFY(x_sema_unlock(prSwdmxInst->hRangeMutex) == OSR_OK);
        return FALSE;
    }
    else if (prSwdmxInst->rCurRangeReq.fgIsFull)
    {
        LOG(5, "_SWDMX_AddRange Add to Q Range, Tag = 0x%08x\n", u4TagID);
        x_memset((VOID*)&prSwdmxInst->rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        prSwdmxInst->rQRangeReq.u4RangeId = prDmxRange->ui4_range_id;
        prSwdmxInst->rQRangeReq.prDmxRangeInfo = prDmxRange;
        prSwdmxInst->rQRangeReq.fgIsFull = TRUE;
    }
    else
    {
        LOG(5, "_SWDMX_AddRange Add to Cur Range, Tag = 0x%08x\n", u4TagID);
        x_memset((VOID*)&prSwdmxInst->rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        prSwdmxInst->rCurRangeReq.u4RangeId = prDmxRange->ui4_range_id;
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
        x_memcpy((VOID*)&prSwdmxInst->rMMRangeInfo, (VOID *)prDmxRange, sizeof(MM_RANGE_INFO_T));
        prSwdmxInst->rCurRangeReq.prDmxRangeInfo = &prSwdmxInst->rMMRangeInfo;
#else
        prSwdmxInst->rCurRangeReq.prDmxRangeInfo = prDmxRange;
#endif
        prSwdmxInst->rCurRangeReq.fgIsFull = TRUE;

#ifdef CC_SWDMX_RENDER_POSITION
        if((prDmxRange->t_start_pos.i4_temporal_reference > 0) ||
           (prDmxRange->t_start_pos.ui2_decoding_order > 0) ||
           (prDmxRange->t_start_pos.ui8_frame_position > 0) ||
           (prDmxRange->t_start_pos.ui8_i_frame_position > 0) ||
           (prDmxRange->t_start_pos.ui8_pts_info > 0) ||
           (prDmxRange->t_start_pos.ui8_i_pts_info > 0) ||
           (prDmxRange->t_start_pos.ui8_aud_pts_info > 0) ||
           (prDmxRange->t_start_pos.ui8_aud_frame_position > 0))
        {
            LOG(5, "_SWDMX_AddRange Render Pos\n", u4TagID);
            //prSwdmxInst->rCurRangeReq.fgSetRenderPosition = TRUE;
            prSwdmxInst->rCurRangeReq.i4TempRef = prDmxRange->t_start_pos.i4_temporal_reference;
            prSwdmxInst->rCurRangeReq.u2DecodingOrder = prDmxRange->t_start_pos.ui2_decoding_order;
            prSwdmxInst->rCurRangeReq.u8Offset = prDmxRange->t_start_pos.ui8_frame_position;
            prSwdmxInst->rCurRangeReq.u8OffsetI = prDmxRange->t_start_pos.ui8_i_frame_position;
            prSwdmxInst->rCurRangeReq.u8StartPts = prDmxRange->t_start_pos.ui8_pts_info;
            prSwdmxInst->rCurRangeReq.u8StartPtsI = prDmxRange->t_start_pos.ui8_i_pts_info;
            prSwdmxInst->rCurRangeReq.u8StartAudPts = prDmxRange->t_start_pos.ui8_aud_pts_info;
            prSwdmxInst->rCurRangeReq.u8StartAudOffset = prDmxRange->t_start_pos.ui8_aud_frame_position;
            prSwdmxInst->rCurRangeReq.u4Timestap = prDmxRange->t_start_pos.ui4_timestap;
            prSwdmxInst->rCurRangeReq.fgSetRenderPosition = TRUE;
        }
#endif

#ifdef CC_SWDMX_RENDER_POSITION_TEST
        if (_fgSetRenderPosition)
        {
            prSwdmxInst->rCurRangeReq.i4TempRef = _rRenderPos.i4_temporal_reference;
            prSwdmxInst->rCurRangeReq.u2DecodingOrder = _rRenderPos.ui2_decoding_order;
            prSwdmxInst->rCurRangeReq.u8Offset = _rRenderPos.ui8_frame_position;
            prSwdmxInst->rCurRangeReq.u8OffsetI = _rRenderPos.ui8_i_frame_position;
            prSwdmxInst->rCurRangeReq.u8StartPts = _rRenderPos.ui8_pts_info;
            prSwdmxInst->rCurRangeReq.u8StartPtsI = _rRenderPos.ui8_i_pts_info;
            prSwdmxInst->rCurRangeReq.u8StartAudPts = _rRenderPos.ui8_aud_pts_info;
            prSwdmxInst->rCurRangeReq.u8StartAudOffset = _rRenderPos.ui8_aud_frame_position;
            prSwdmxInst->rCurRangeReq.fgSetRenderPosition = TRUE;
        }
#endif

        VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_SRC_TYPE,(UINT32)(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type),0,0);

        if(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            VDEC_SetTPTFlag(prSwdmxInst->u1VdecId,TRUE);
        }


#ifdef SUPPORT_3D_MEDIA
        // set 3D information to VDEC
        if (prDmxRange->b_stereo_scopic)
        {
            switch (prDmxRange->e_scopic_type)
            {
            case MM_SCOPIC_RF:
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_SBS_RF, 0, 0);
                break;
            case MM_SCOPIC_LF:
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_SBS_LF, 0, 0);
                break;
            case MM_SCOPIC_RT:
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_TB_RF, 0, 0);
                break;
            case MM_SCOPIC_LT:
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_TB_LF, 0, 0);
                break;
            #ifdef CC_3D_MM_DS_SUPPORT
            case MM_SCOPIC_DS:
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_DUAL_STREAM, 0, 0);
                VDEC_SetMMParam(prSwdmxInst->u1Vdec2Id, VDEC_MM_3D_INFO, VDEC_3D_DUAL_STREAM, 0, 0);
                break;
            case MM_SCOPIC_MS:
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_3D_INFO, VDEC_3D_MULTI_STREAM, 0, 0);
                break;
            #endif
            default:
                LOG(1, "Unknown 3D type!!!\n");
                break;
            }
        }
#endif

//#ifdef CC_DLNA_SUPPORT
        if(prDmxRange->e_mm_src_type == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
        {
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_SRC_TYPE,FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE);
        }
        else if(prDmxRange->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_SRC_TYPE, FEEDER_SRC_TYPE_NETWORK_NETFLIX);
        }
        else if(prDmxRange->e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA)
        {
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_SRC_TYPE,FEEDER_SRC_TYPE_NETWORK_DLNA);
        }
        else if(prDmxRange->e_mm_src_type == MM_SRC_TYPE_NETWORK_CMPB)
        {
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_SRC_TYPE,FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE);
        }
        else
        {
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_SRC_TYPE,FEEDER_SRC_TYPE_UNKNOWN);
        }

//#endif

        //if(FeederCLIGetCacheSize(prSwdmxInst->eFeederSrc)==0)
        {
            //for audio ES init quickly. #ifdef FEEDER_HEADER_CACHE
            if(prSwdmxInst->eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
            {
                FeederSetCacheSize(prSwdmxInst->eFeederSrc, (64*1024));
            }
            else
            {
                FeederSetCacheSize(prSwdmxInst->eFeederSrc, FEEDER_CACHED_MAX_READ_SIZE);
            }
        }
        //else
        //{
            //if CLI value was set, we adopt cli setting value.
            //FeederSetCacheSize(prSwdmxInst->eFeederSrc, FeederCLIGetCacheSize(prSwdmxInst->eFeederSrc));
        //}

        /* move to main loop, u1VdecId is not assign in this moment.
        if (prDmxRange->ui4_vid_render_counts)
        {
            VDEC_SetRenderPicCount(prSwdmxInst->u1VdecId, TRUE, prDmxRange->ui4_vid_render_counts);
        }
        */

        if (prSwdmxInst->pfnSwdmxSetRange &&
             ((prSwdmxInst->eCurFMT==SWDMX_FMT_FLV)||
             (prSwdmxInst->eCurFMT == SWDMX_FMT_MP4) ||
             (prSwdmxInst->eCurFMT == SWDMX_FMT_M4A) ||
             (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)))
        {
            prSwdmxInst->pfnSwdmxSetRange(u1SrcId, &prSwdmxInst->rCurRangeReq, prSwdmxInst->u4VidStrmID,
                                         prSwdmxInst->u4AudStrmID, prSwdmxInst->u4SubTitleStrmID);
        }
    }

    VERIFY(x_sema_unlock(prSwdmxInst->hRangeMutex) == OSR_OK);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Add_Range
 *  Add the demux range
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DelRange(UINT8 u1SrcId, MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID, BOOL fgDelAllRange)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    VERIFY(x_sema_lock(prSwdmxInst->hRangeMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(fgDelAllRange)        //DTV00148327 before thumb nail stop, we should delete all the range info
    {

       LOG(5,"_SWDMX_DelRange delete all range\n");
       x_memset((VOID*)&prSwdmxInst->rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
       x_memset((VOID*)&prSwdmxInst->rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
       VDEC_SetRenderPicCount(prSwdmxInst->u1VdecId, FALSE, 0);

       VERIFY(x_sema_unlock(prSwdmxInst->hRangeMutex) == OSR_OK);

       return TRUE;
    }

    if ((prSwdmxInst->rQRangeReq.fgIsFull) &&
            (((UINT32)prSwdmxInst->rQRangeReq.u4RangeId) == u4TagID))
    {
        x_memset((VOID*)&prSwdmxInst->rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
    }
    else if ((prSwdmxInst->rCurRangeReq.fgIsFull) &&
             (((UINT32)prSwdmxInst->rCurRangeReq.u4RangeId) == u4TagID))
    {
        if ((prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PLAY) &&
                (prSwdmxInst->rCurRangeReq.fgInUse))
        {
            LOG(5, "_SWDMX_DelRange Cur Range is in use, id = 0x%08x\n", u4TagID);
            VERIFY(x_sema_unlock(prSwdmxInst->hRangeMutex) == OSR_OK);
            return FALSE;
        }
        else
        {
            LOG(5, "_SWDMX_DelRange Cur Range, id = 0x%08x\n", u4TagID);
            x_memset((VOID*)&prSwdmxInst->rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
            // Swap Range
            if (prSwdmxInst->rQRangeReq.fgIsFull)
            {
                x_memcpy((VOID*)&prSwdmxInst->rCurRangeReq, (VOID*)&prSwdmxInst->rQRangeReq, sizeof(SWDMX_RANGE_LIST_T));
                x_memset((VOID*)&prSwdmxInst->rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
            }
        }
    }
    if ((prSwdmxInst->pfnSwdmxSetRange) && (prSwdmxInst->rCurRangeReq.fgIsFull))
    {
        LOG(5, "Set Range tagID=0x%08x\n", prSwdmxInst->rCurRangeReq.u4RangeId);
        prSwdmxInst->pfnSwdmxSetRange(u1SrcId, &prSwdmxInst->rCurRangeReq, prSwdmxInst->u4VidStrmID,
                                     prSwdmxInst->u4AudStrmID, prSwdmxInst->u4SubTitleStrmID);
    }
    else
    {
        LOG(3, "Set Range err full %d\n", prSwdmxInst->rCurRangeReq.fgIsFull);
    }

    VERIFY(x_sema_unlock(prSwdmxInst->hRangeMutex) == OSR_OK);
    return TRUE;
}

BOOL _SWDMX_GetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    switch (u1Type)
    {
    case eSWDMX_TOTAL_DURATION:
        if (pu4Para1 == NULL)
        {
            LOG(3, "_SWDMX_GetInfo pu4Para1 null\n");
            return FALSE;
        }
        *pu4Para1 = prSwdmxInst->u4TotalTime;
        return TRUE;

    case eSWDMX_GET_CURRENT_POSITION:
    case eSWDMX_GET_CURRENT_TIME:
    case eSWDMX_SET_CHK_PSI_BUF_SIZE:
    case eSWDMX_GET_IS_TRICK:
    case eSWDMX_GET_CURRENT_TICK:
    case eSWDMX_GET_CURRENT_LBA:
    case eSWDMX_GET_STC_OFFSET:
    case eSWDMX_GET_AUDIO_ACTIVE_PIDX:
    case eSWDMX_GET_VIDEO_PIDX:
    case eSWDMX_GET_AUDIO_PIDX:
        if (prSwdmxInst->pfnSwdmxGetInfo) {
            return prSwdmxInst->pfnSwdmxGetInfo(u1SrcId, u1Type,pu4Para1,pu4Para2,pu4Para3);
        }
        break;
    case eSWDMX_CONTAINER_TYPE:
        if (pu4Para1 == NULL)
        {
            LOG(3, "_SWDMX_GetInfo pu4Para1 null\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)prSwdmxInst->eCurFMT;
        return TRUE;
    case eSWDMX_GET_INTERNAL_INFO:
        *pu4Para1 = (UINT32)&_arSwdmxInfo[u1SrcId];
        return TRUE;

    case eSWDMX_GET_FILE_OFFSET:
        {
            UINT64 *pu8Tmp;
            if (pu4Para1 == NULL)
            {
                LOG(3, "_SWDMX_GetInfo pu4Para1 null\n");
                return FALSE;
            }
            pu8Tmp = (UINT64*)(VOID*)pu4Para1;
            *pu8Tmp = 0;
            if(prSwdmxInst->eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
            {
                *pu8Tmp = prSwdmxInst->u8FileOffset;
            }
        }
        return TRUE;

    case eSWDMX_GET_INTERNAL_BUF:
        if (prSwdmxInst->pucSwdmxInternalBuf)
        {
            UINT32 u4TempPtr;

            if (!pu4Para1 || !pu4Para2)
            {
                LOG(3, "Fail to eSWDMX_GET_INTERNAL_BUF Wrong Parameter\n");
                return FALSE;
            }

            u4TempPtr = (UINT32)prSwdmxInst->pucSwdmxInternalBuf + prSwdmxInst->u4UsedInternalBufPtr;

            // parameter 3 is for alignment
            if (pu4Para3 && (*pu4Para3))
            {
                UINT32 u4Align = *pu4Para3;
                u4TempPtr = ((u4TempPtr + (u4Align - 1)) / u4Align) * u4Align;
            }

            // parameter 1 is for the return address
            // parameter 2 is for the request size
            if ((u4TempPtr + *pu4Para2) <
                (UINT32)(prSwdmxInst->pucSwdmxInternalBuf + SWDMX_INTERNAL_BUF_SIZE))
            {
                *pu4Para1 = (UINT32)u4TempPtr;
                prSwdmxInst->u4UsedInternalBufPtr = (*pu4Para2 + (u4TempPtr - (UINT32)(prSwdmxInst->pucSwdmxInternalBuf)));
                return TRUE;
            }
            else
            {
                *pu4Para1 = 0;
                *pu4Para2 = 0;

                return FALSE;
            }
        }
        break;
    #if 0
    case eSWDMX_GET_CURRENT_TIME:
        {
            UINT64 u8Tmp;
            UINT64 u8CurPos;
            UINT64 u8Remainder;

            u8CurPos = 0;
            *pu4Para1 = 0;
            if (prSwdmxInst->pfnSwdmxGetInfo)
            {
                if(!prSwdmxInst->pfnSwdmxGetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0))
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
            if ((prSwdmxInst->u4TotalTime > 0) &&
                    (prSwdmxInst->u8FileSize > 0))
            {
                // 90000->90000
                u8Tmp = _SWDMX_Div6464((prSwdmxInst->u8FileSize - prSwdmxInst->u8FileOffset)*90000,
                                  prSwdmxInst->u4TotalTime,
                                  &u8Remainder);
                if(u8Tmp > 0)
                {
                    *pu4Para1 = (UINT32)_SWDMX_Div6464(u8CurPos*90000,
                                               (UINT32)u8Tmp, &u8Remainder);
                }
                UNUSED(u8Remainder);
            }
            return TRUE;
        }
    #endif
    case eSWDMX_GET_TIME_TO_BYTES_INFO:
        if(!pu4Para1 || !pu4Para2)
        {
            LOG(3, "Fail to eSWDMX_GET_TIME_TO_BYTES_INFO Wrong Parameter\n");
            return FALSE;
        }
        else
        {
             *pu4Para2 = (VDEC_GetAvgPicSize(0)*30*(*pu4Para1))/1000;
        }
        return TRUE;
    case eSWDMX_GET_PLAYBACK_THRESHOLD:
        if(!pu4Para1 || !pu4Para2)
        {
            LOG(3, "Fail to eSWDMX_GET_PLAYBACK_THRESHOLD Wrong Parameter\n");
            return FALSE;
        }
        else
        {
            *pu4Para1 = (ENUM_SWDMX_BUFFER_TYPE_T)(prSwdmxInst->ePlaybackBufType);
            *pu4Para2 = prSwdmxInst->u4PlaybackBufThreshold;
        }
        return TRUE;
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    case eSWDMX_GET_BAD_INTERLEAVED_INFO:
        if (!pu4Para1)
        {
            LOG(3, "Fail to eSWDMX_GET_BAD_INTERLEAVED_INFO Wrong Parameter\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)prSwdmxInst->fgIsBadInterleaved;
        return TRUE;
#endif
    default:
        break;
    }

    return FALSE;
}

BOOL _SWDMX_SetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
    BOOL fgRet;

    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    switch (u1Type)
    {
    case eSWDMX_SET_VIDEOTYPE:
        prSwdmxInst->eVideoType =  (ENUM_SWDMX_VDEC_T)u4Para1;
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_SET_SUBVIDEOTYPE:
        prSwdmxInst->eSubVideoType =  (ENUM_SWDMX_VDEC_T)u4Para1;
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_SET_VIDEO2TYPE:
        prSwdmxInst->eVideo2Type =  (ENUM_SWDMX_VDEC_T)u4Para1;
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_SET_AUDIOTYPE:
        prSwdmxInst->eAudioType = (ENUM_SWDMX_ADEC_T)u4Para1;
        //prSwdmxInst->eAudio2Type = (ENUM_SWDMX_ADEC_T)u4Para2;
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_SET_VIDSEQHEADER:
    case eSWDMX_SET_TPT_VIDSEQHEADER:
    case eSWDMX_SET_CHK_PSI_BUF_SIZE:
    case eSWDMX_SET_REC_BUF_RANGE:
    case eSWDMX_PRINT_REC_BUF_STATUS:
    case eSWDMX_PRINT_INTERNAL_STATUS:
    case eSWDMX_SET_MONITOF_LBA:
    case eSWDMX_SET_NETFLIX_STREAM_ID:
    case eSWDMX_SET_SYSTEMINFO:
    case eSWDMX_PID_INDEX_START:
    case eSWDMX_SET_VIDEO_DECRYPT:
    case eSWDMX_SET_AUDIO_DECRYPT:
    case eSWDMX_SET_VID_PID_IDX:
    case eSWDMX_SET_AUD_PID_IDX:
    case eSWDMX_SET_PCR_PID_IDX:
    case eSWDMX_SET_NETFLIX_EOS:
    case eSWDMX_SET_VID_SCRAMBLE_NFY:
    case eSWDMX_SET_AUD_SCRAMBLE_NFY:
    case eSWDMX_SET_SYSTEM_B:
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_ENABLE_STREAM:
        if (prSwdmxInst->pfnSwdmxEnableStrm != NULL)
        {
            if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID)
            {
                VDEC_SetMMParam(prSwdmxInst->u1VdecId, VDEC_MM_CONTAINER_TYPE,(UINT32)(prSwdmxInst->eCurFMT),0,0);
            }
            if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID2)
            {
                VDEC_SetMMParam(prSwdmxInst->u1Vdec2Id, VDEC_MM_CONTAINER_TYPE,(UINT32)(prSwdmxInst->eCurFMT),0,0);
            }
            fgRet = prSwdmxInst->pfnSwdmxEnableStrm(u1SrcId, (UINT8)u4Para1, (VOID*)u4Para2);
            if (fgRet)
            {
                if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID)
                {
                    LOG(5, "%s Enable Video Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnVideoDmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID2)
                {
                    LOG(5, "%s Enable Video2 Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnVideo2Dmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_AUD)
                {
                    LOG(5, "%s Enable Audio Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnAudioDmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_SUBTITLE)
                {
                    LOG(5, "%s Enable Subtitle Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnSubTitleDmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_PCR)
                {
                    LOG(5, "%s Enable PCR Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnPcrDmx = TRUE;
                }
            }
            return fgRet;
        }
        break;
    case eSWDMX_DISABLE_STREAM:
        if (prSwdmxInst->pfnSwdmxDisableStrm != NULL)
        {
            fgRet =  prSwdmxInst->pfnSwdmxDisableStrm(u1SrcId, (UINT8)u4Para1);
            if (fgRet)
            {
                if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID)
                {
                    LOG(5, "%s Disable Video Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnVideoDmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID2)
                {
                    LOG(5, "%s Disable Video2 Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnVideo2Dmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_AUD)
                {
                    LOG(5, "%s Disable Audio Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnAudioDmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_SUBTITLE)
                {
                    LOG(5, "%s Disable Subtitle Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnSubTitleDmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_PCR)
                {
                    LOG(5, "%s Disable Pcr Stream Src id is %d\n", __FUNCTION__, u1SrcId);
                    prSwdmxInst->fgEnPcrDmx = FALSE;
                }
            }
            return fgRet;
        }
        break;
    case eSWDMX_SET_RANGECALLBACK:
        _SWDMX_SetRangeCb(u1SrcId, (VOID*)u4Para1);
        break;
    case eSWDMX_SET_RANGECALLBACKEX:
        _SWDMX_SetRangeCbEx(u1SrcId, (VOID*)u4Para1);
        break;
    case eSWDMX_TOTAL_DURATION:
        prSwdmxInst->u4TotalTime = u4Para1;
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_SET_SRC_TYPE:
        prSwdmxInst->eFeederSrc = (FeederSourceType)u4Para1;
        LOG(5, "%s(%d) eSWDMX_SET_SRC_TYPE is %d\n",
            __FUNCTION__, u1SrcId, prSwdmxInst->eFeederSrc);
        FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE,
            _SWDMX_FeederCallBack, (UINT32)&prSwdmxInst->rFeederInfo);
#if 0//def CC_DLNA_SUPPORT
        FeederSetInfo(prSwdmxInst->eFeederSrc,
            FEEDER_SWDMX_STATUS_CB,
            (UINT32)_SWDMX_FeederStatusNfyCb);
#endif
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_INSTANCE, u1SrcId);

        return TRUE;
#if 0
    case eSWDMX_SET_FEEDER_TEST_FUNCTION:
        prSwdmxInst->pfnSwdmxHdrParse = _SWDMX_HdrParseFeederTest;
        break;
#endif
    case eSWDMX_SET_AOUT_ENABLE:
        AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
        if (prSwdmxInst->pfnSwdmxSetInfo) {
            return prSwdmxInst->pfnSwdmxSetInfo(u1SrcId, u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;

    case eSWDMX_SET_EOS:
        prSwdmxInst->fgRcvEOS = (BOOL)u4Para1;
        break;

    case eSWDMX_SET_VDEC_ID:
        prSwdmxInst->u1VdecId = (UINT8)u4Para1;
        //for skype, vdec_id is ready after swdmx add_range(src type setting original position)
        if(prSwdmxInst->rCurRangeReq.prDmxRangeInfo)
        {
           VDEC_SetMMParam(prSwdmxInst->u1VdecId,VDEC_MM_SRC_TYPE,(UINT32)(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type),0,0);
        }
        return TRUE;

    case eSWDMX_SET_B2R_ID:
        prSwdmxInst->u1B2rId = (UINT8)u4Para1;
        return TRUE;

    case eSWDMX_SET_VDEC2_ID:
        prSwdmxInst->u1Vdec2Id = (UINT8)u4Para1;
        return TRUE;

    case eSWDMX_SET_ADEC_ID:
        prSwdmxInst->u1AdecId = (UINT8)u4Para1;
        return TRUE;

    case eSWDMX_SET_STC_ID:
        prSwdmxInst->u1StcId = (UINT8)u4Para1;
        return TRUE;

    case eSWDMX_SET_DMX_ID:
        prSwdmxInst->u1DmxId = (UINT8)u4Para1;
        return TRUE;   
#ifdef SWDMX_DBG_USB
        case eSWDMX_SET_DBGINFO:
            if ( 0 != u4Para1 )
            {
               *((UINT32 *)u4Para1) = (UINT32)&prSwdmxInst->rDbgInfo;
            }
            return TRUE;
#endif // SWDMX_DBG_USB
    case eSWDMX_SET_PLAYBACK_THRESHOLD:
        prSwdmxInst->u4PlaybackBufThreshold = (UINT32)u4Para2;
        if(u4Para2 == 0)
        {
            prSwdmxInst->ePlaybackBufType = eSWDMX_BUF_MODE_DEFAULT;
        }
        else
        {
            prSwdmxInst->ePlaybackBufType = (ENUM_SWDMX_BUFFER_TYPE_T)u4Para1;
        }
        return TRUE;
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    case eSWDMX_SET_BAD_INTERLEAVED_INFO:
        prSwdmxInst->fgIsBadInterleaved = (BOOL)u4Para1;
        return TRUE;
#endif
    default:
        LOG(5, "unknow type(%d) %d\n", u1SrcId, (UINT32)u1Type);
        break;
    }

    return FALSE;
}

VOID _SWDMX_QueryRangeInfo(UINT8 u1SrcId, SWDMX_RANGE_INFO_T* prRangeInfo)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);

    if (prRangeInfo != NULL)
    {
        x_memset(prRangeInfo, 0, sizeof(SWDMX_RANGE_INFO_T));
        if (prSwdmxInst->rCurRangeReq.fgIsFull)
        {
            prRangeInfo->u4CurRangeID = prSwdmxInst->rCurRangeReq.u4RangeId;
            prRangeInfo->u8CurRangePTS =
                (UINT64)prSwdmxInst->rCurRangeReq.prDmxRangeInfo->t_vid_start_pts;
        }
        if (prSwdmxInst->rQRangeReq.fgIsFull)
        {
            prRangeInfo->u4QRangeID = prSwdmxInst->rQRangeReq.u4RangeId;
            prRangeInfo->u8QRangePTS =
                (UINT64)prSwdmxInst->rQRangeReq.prDmxRangeInfo->t_vid_start_pts;
        }
    }
}

INT32 _SWDMX_SetDmxNfy(UINT8 u1SrcId, const SWDMX_NFY_INFO_T* prSwdmxNfyInfo)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (prSwdmxNfyInfo == NULL)
    {
        return (SWDMX_DRV_INV_SET_INFO);
    }

    prSwdmxInst->pvSwdmxNfyTag = prSwdmxNfyInfo->pvTag;
    prSwdmxInst->pfSwdmxNfyFct = prSwdmxNfyInfo->pfSwdmxNfy;

    return (SWDMX_DRV_OK);
}


BOOL _SWDMX_ReadBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (prSwdmxInst->pfnSwdmxReadBuf)
    {
        return prSwdmxInst->pfnSwdmxReadBuf(u1SrcId, eStrmType, prReadInfo);
    }

    return FALSE;
}

BOOL _SWDMX_ReleaseBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (prSwdmxInst->pfnSwdmxReleaseBuf)
    {
        return prSwdmxInst->pfnSwdmxReleaseBuf(u1SrcId, eStrmType, prReadInfo);
    }

    return FALSE;
}


BOOL _SWDMX_AudioNotify(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if (prSwdmxInst->pfnSwdmxAudioCb != NULL)
    {
        return prSwdmxInst->pfnSwdmxAudioCb(u1SrcId, eAudioNotifyType);
    }
    return TRUE;
}

BOOL _SWDMX_PcrCallBack(UINT32 u4PcrBase)
{
    UINT8 u1SrcId = 0;
    UINT32 i;

    for(i=0;i<SWDMX_SOURCE_MAX;i++)
    {
        if(_arSwdmxInfo[i].eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            u1SrcId = i;
            break;
        }
    }
    return _SWDMX_MpgPcrCallBack(u1SrcId, u4PcrBase);
}

UINT16 _SWDMX_GetNumOfCmd(UINT8 u1SrcId)
{
    UINT16 u2Count = 0;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    VERIFY(x_msg_q_num_msgs(prSwdmxInst->hDmxPlayCmdQ, &u2Count) == OSR_OK);

    return u2Count;
}

SWDMX_INFO_T* _SWDMX_GetInst(UINT8 u1SrcId)
{
    // don't need this
    //_SWDMX_LockSrcMutex();

    if (u1SrcId >= SWDMX_SOURCE_MAX)
    {
        //_SWDMX_UnlockSrcMutex();
        return NULL;
    }

    //_SWDMX_UnlockSrcMutex();
    return &_arSwdmxInfo[u1SrcId];
}

BOOL _SWDMX_LockValidSrc(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    _SWDMX_LockSrcMutex();

    if (_arSwdmxInfo[u1SrcId].fgValid)
    {
        _arSwdmxInfo[u1SrcId].fgValid = FALSE;
        _SWDMX_UnlockSrcMutex();
        return TRUE;
    }

    _SWDMX_UnlockSrcMutex();
    return FALSE;
}

VOID _SWDMX_UnlockSrc(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);

    _SWDMX_LockSrcMutex();

    _arSwdmxInfo[u1SrcId].fgValid = TRUE;

    _SWDMX_UnlockSrcMutex();
}

static VOID _SWDMX_ResetSwdmxInfo(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);
    if(u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(1, "_SWDMX_ResetSwdmxInfo (u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return;
    }

    _SWDMX_LockSrcMutex();

    prSwdmxInst->u1StcId = 0;
    prSwdmxInst->u1VdecId = ES0;
    prSwdmxInst->u1AdecId = AUD_DEC_MAIN;
    prSwdmxInst->u1B2rId = B2R_1;
    prSwdmxInst->u4UsedInternalBufPtr = 0;
    prSwdmxInst->eAudioType = ENUM_SWDMX_ADEC_UNKNOWN;
    prSwdmxInst->fgFifoFull = FALSE;

    x_memset(&_arSwdmxInfo[u1SrcId].rFmtInfo, 0, sizeof(_arSwdmxInfo[u1SrcId].rFmtInfo));

    _SWDMX_UnlockSrcMutex();
}

VOID _SWDMX_LockRangeSema(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInst->hRangeSema)
    {
        VERIFY(x_sema_lock(prSwdmxInst->hRangeSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
}

VOID _SWDMX_UnlockRangeSema(UINT8 u1SrcId)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInst->hRangeSema)
    {
        VERIFY(x_sema_unlock(prSwdmxInst->hRangeSema) == OSR_OK);
    }
}


BOOL _SWDMX_MoveData(UINT8 u1SrcId, DMX_MM_DATA_T *prDmxMMData)
{
    SWDMX_INFO_T *prSwdmxInst;
    BOOL fgRet;
    if(!prDmxMMData)
    {
        LOG(3, "_SWDMX_MoveData prDmxMMData null\n");
        return FALSE;
    }
    prSwdmxInst= _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst&&prSwdmxInst->rCurRangeReq.prDmxRangeInfo &&
    USE_COMMON_NETBUFFER_FLOW(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
    {
        prSwdmxInst->fgRcvEOS=prDmxMMData->fgEOS;
    }
    
#if defined (SWDMX_DBG_USB) || defined (SWDMX_DBG_SEMIHOST)
    do
    {
        UINT64  u8FileOffset = 0;
        
        if ( TRUE == IS_MPEG_MODULE_EXCEPT( prSwdmxInst->eCurFMT, SWDMX_FMT_MPEG2_TS_TIME_SHIFT) )
        {
            u8FileOffset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        }
        else  // if ()
        {
            // Please add your offset here
        }

#ifdef SWDMX_DBG_SEMIHOST
        if ( TRUE == IS_MPEG_MODULE_EXCEPT( prSwdmxInst->eCurFMT, SWDMX_FMT_MPEG2_TS_TIME_SHIFT ) /*  Please add your condition here */ )
        {
            UINT32  u4_size1 = 0;
            UINT32  u4_size2 = 0;
            CHAR    ai1FileName[50] = { 0 };

            if ( prDmxMMData->u4FrameSize + prDmxMMData->u4StartAddr
                 < prDmxMMData->u4BufEnd )
            {
                u4_size1 = prDmxMMData->u4FrameSize;
            }
            else
            {
                u4_size1 = prDmxMMData->u4BufEnd - prDmxMMData->u4StartAddr;
                u4_size2 = prDmxMMData->u4FrameSize - u4_size1;
            }

            x_sprintf( ai1FileName, "D:\\drvif_%u_%u.dat", u1SrcId, (UINT32)u8FileOffset );
            T32_HostSaveData( ai1FileName,
                                          (void *)prDmxMMData->u4StartAddr,
                                          u4_size1 );
            if ( 0 != u4_size2 )
            {
                x_sprintf( ai1FileName, "D:\\drvif_%u_%u_ex.dat", u1SrcId, (UINT32)u8FileOffset );
                T32_HostSaveData( ai1FileName,
                                               (void *)prDmxMMData->u4BufStart,
                                               u4_size2 );
            }
            
        }
#endif  // SWDMX_DBG_SEMIHOST
                    
#ifdef SWDMX_DBG_USB
        if ( TRUE == IS_MPEG_MODULE_EXCEPT( prSwdmxInst->eCurFMT, SWDMX_FMT_MPEG2_TS_TIME_SHIFT ) /*  Please add 
    your 
    condition here */ )
        {
            if ( TRUE == MM_DBG_SOURCE1_ON( prSwdmxInst->rDbgInfo.u4Mode, MM_DBG_MODE_LOGCRC ) )
            {
                    Dbg_LogEx( prDmxMMData->u4BufStart,
                                      prDmxMMData->u4BufEnd,
                                      prDmxMMData->u4StartAddr, 
                                      prDmxMMData->u4FrameSize,
                                      u8FileOffset );
            }

            if ( TRUE == MM_DBG_SOURCE1_ON( prSwdmxInst->rDbgInfo.u4Mode, MM_DBG_MODE_SAVECRC )  &&
                  0 != prSwdmxInst->rDbgInfo.hFileSaveCrc )
            {
                if ( FALSE == Dbg_SaveCrcEx( prSwdmxInst->rDbgInfo.hFileSaveCrc,
                                                            u8FileOffset,
                                                            prDmxMMData->u4BufStart,
                                                            prDmxMMData->u4BufEnd,
                                                            prDmxMMData->u4StartAddr,
                                                            prDmxMMData->u4FrameSize) )
                {
                    LOG( 0, "%s:%u Save CRC to usb failed!\n", __FUNCTION__, __LINE__ );
                }
            }

            if ( TRUE == MM_DBG_SOURCE1_ON( prSwdmxInst->rDbgInfo.u4Mode, MM_DBG_MODE_AUTOCHECK )   &&
                  0 != prSwdmxInst->rDbgInfo.hFileGetCrc )
            {
                UINT32 u4_crc = 0;
                Dbg_GetCrcEx(prDmxMMData->u4BufStart,
                                      prDmxMMData->u4BufEnd,
                                      prDmxMMData->u4StartAddr,
                                      prDmxMMData->u4FrameSize,
                                      &u4_crc );
                if ( 2 == Dbg_GetOrignalCrc( prSwdmxInst->rDbgInfo.hFileGetCrc,
                                                               u8FileOffset,
                                                               &u4_crc) )           
                {
                    LOG( 1, "%s:%d Crc miss match\n", __FUNCTION__, __LINE__ );
                }
            }
            
            if ( TRUE == MM_DBG_SOURCE1_ON( prSwdmxInst->rDbgInfo.u4Mode, MM_DBG_MODE_SAVEDATA )  &&
                  0 != prSwdmxInst->rDbgInfo.hFileSaveData )
            {
                if ( FALSE == Dbg_SaveUsbFileEx( prSwdmxInst->rDbgInfo.hFileSaveData,
                                                                  prDmxMMData->u4BufStart,
                                                                  prDmxMMData->u4BufEnd,
                                                                  prDmxMMData->u4StartAddr,
                                                                  prDmxMMData->u4FrameSize ) )
                {
                    LOG( 0, "%s:%u Save Data to usb failed!\n", __FUNCTION__, __LINE__ );
                }
            }

            if ( TRUE == MM_DBG_SOURCE1_ON( prSwdmxInst->rDbgInfo.u4Mode, MM_DBG_MODE_SAVEBLOCK ) )
            {
                CHAR    ai1FileName[NAME_MAX_LEN] = { 0 };
                x_sprintf(ai1FileName, "%s/block_%u", FILE_LOCAL, (UINT32)u8FileOffset);
                
                if ( FALSE == Dbg_SaveBlock(ai1FileName,
                                                          prDmxMMData->u4StartAddr,
                                                          prDmxMMData->u4FrameSize) )
                {
                    LOG( 7, "%s:%u Save block to usb failed!\n", __FUNCTION__, __LINE__ );
                }
            }
        }
#endif  // defined SWDMX_DBG_USB
        
    }while( FALSE );
#endif // defined (SWDMX_DBG_USB) || defined (SWDMX_DBG_SEMIHOST)

#ifdef SWDMX_DBG_USB
    do
    {
        if (USB_DUMP_ON(DUMP_DATA_SWDMX_DMX_VIDEO))
        {
            static UINT32 u4EsCount = 0;
            static CHAR ucFileName[NAME_MAX_LEN] = {0};
            
            if (0!=prDmxMMData->u4StartAddr &&
                prDmxMMData->u4StartAddr+prDmxMMData->u4FrameSize <= prDmxMMData->u4BufEnd)
            {
                if (TRUE == IS_MPEG_MODULE_EXCEPT( prSwdmxInst->eCurFMT, SWDMX_FMT_MPEG2_TS_TIME_SHIFT ))
                {
                    x_sprintf(ucFileName, "%s/SWDMX_%010u.raw", FILE_LOCAL, u4EsCount);
                }
                else
                {
                    if (DUMP_LOG)
                    {
                        LOG(1, "%s:%u Not deal yet.\n", __FUNCTION__, __LINE__);
                    }
                    break;
                }

                Dbg_SaveBlock(ucFileName, VIRTUAL(prDmxMMData->u4StartAddr), prDmxMMData->u4FrameSize);
                if (DUMP_LOG)
                {
                    LOG(1, "SWDMX   KAddr:0x%08x VAddr:0x%08x Size=%u\n", prDmxMMData->u4StartAddr, VIRTUAL(prDmxMMData->u4StartAddr), 
                        prDmxMMData->u4FrameSize);
                }
            }
            else if  (0 != prDmxMMData->u4StartAddr)
            {
                if (TRUE == IS_MPEG_MODULE_EXCEPT( prSwdmxInst->eCurFMT, SWDMX_FMT_MPEG2_TS_TIME_SHIFT ))
                {
                    x_sprintf(ucFileName, "%s/SWDMX_%010u_1.raw", FILE_LOCAL, u4EsCount);
                }
                else
                {
                    if (DUMP_LOG)
                    {
                        LOG(1, "%s:%u Not deal yet.\n", __FUNCTION__, __LINE__);
                    }
                    break;
                }
                Dbg_SaveBlock(ucFileName, VIRTUAL(prDmxMMData->u4StartAddr), prDmxMMData->u4BufEnd-prDmxMMData->u4StartAddr);
                LOG(9, "SWDMX_1 KAddr:0x%08x VAddr:0x%08x Size=%u\n", prDmxMMData->u4StartAddr, VIRTUAL(prDmxMMData->u4StartAddr), 
                    prDmxMMData->u4BufEnd-prDmxMMData->u4StartAddr);

                if (TRUE == IS_MPEG_MODULE_EXCEPT( prSwdmxInst->eCurFMT, SWDMX_FMT_MPEG2_TS_TIME_SHIFT ))
                {
                    x_sprintf(ucFileName, "%s/SWDMX_%010u_2.raw", FILE_LOCAL, u4EsCount);
                }
                else
                {
                    if (DUMP_LOG)
                    {
                        LOG(1, "%s:%u Not deal yet.\n", __FUNCTION__, __LINE__);
                    }
                    break;
                }
                Dbg_SaveBlock(ucFileName, VIRTUAL(prDmxMMData->u4BufStart), 
                     prDmxMMData->u4FrameSize-(prDmxMMData->u4BufEnd-prDmxMMData->u4StartAddr));
                if (DUMP_LOG)
                {
                    LOG(1, "SWDMX_2 KAddr:0x%08x VAddr:0x%08x Size=%u\n", prDmxMMData->u4BufStart, VIRTUAL(prDmxMMData->u4BufStart), 
                        prDmxMMData->u4FrameSize-(prDmxMMData->u4BufEnd-prDmxMMData->u4StartAddr)); 
                }
            }

            u4EsCount ++;
        }
    }while(FALSE);
#endif
    _SWDMX_LockMoveMutex(u1SrcId);
    fgRet = DMX_MUL_MoveData(prSwdmxInst->u1DmxId, prDmxMMData);
    _SWDMX_UnlockMoveMutex(u1SrcId);

    LOG(9, "_SWDMX_MoveData DMX_MUL_MoveData 0x%x, S=%d.\n",
        prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
    if(!fgRet)
    {
        LOG(5, "_SWDMX_MoveData DMX_MUL_MoveData Fail.\n");
        return FALSE;
    }

    return TRUE;
}

VOID _SWDMX_LockMoveMutex(UINT8 u1SrcId)
{
    VERIFY(x_sema_lock(_rSwdmxKeepInfo.hSwdmxMoveSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    _rSwdmxKeepInfo.ucLockSwdmxId = u1SrcId;

    _SWDMX_SetVideoFormat(u1SrcId);
}


VOID _SWDMX_UnlockMoveMutex(UINT8 u1SrcId)
{
    VERIFY(x_sema_unlock(_rSwdmxKeepInfo.hSwdmxMoveSema) == OSR_OK);
}


VOID _SWDMX_SetVideoFormat(UINT8 u1SrcId)
{
    DMX_VIDEO_TYPE_T eVDec;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_VERIFY_NULL(prSwdmxInst);

    // to support multiple video format, demux need change format before
    // we call move data.
    if(prSwdmxInst->fgEnVideoDmx)
    {
        /*
        if((prSwdmxInst->eCurFMT == SWDMX_FMT_VC1_ES) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_VIDEO_ES) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG4_VIDEO_ES) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_H264_VIDEO_ES))
        {
            DMX_ES_SetMoveIndex(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
        }
        */

        switch (prSwdmxInst->eVideoType)
        {
        case ENUM_SWDMX_VDEC_VC1:
            eVDec = DMX_VIDEO_VC1;
            break;
        case ENUM_SWDMX_VDEC_WMV7:
            eVDec = DMX_VIDEO_WMV7;
            break;
        case ENUM_SWDMX_VDEC_WMV8:
            eVDec = DMX_VIDEO_WMV8;
            break;
        case ENUM_SWDMX_VDEC_WMV9:
            eVDec = DMX_VIDEO_WMV9;
            break;
        case ENUM_SWDMX_VDEC_MPEG4:
        case ENUM_SWDMX_VDEC_MJPEG:
            eVDec = DMX_VIDEO_MPEG4;
            break;
        case ENUM_SWDMX_VDEC_DX311:
            if(prSwdmxInst->eCurFMT == SWDMX_FMT_VC1_WMV)
            {
                eVDec = DMX_VIDEO_MP4_IN_WMV;
            }
            else
            {
                eVDec = DMX_VIDEO_MPEG4;
            }
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            eVDec = DMX_VIDEO_MPEG;
            break;
        case ENUM_SWDMX_VDEC_H264:
            eVDec = DMX_VIDEO_H264;
            break;
        case ENUM_SWDMX_VDEC_AVS:
            eVDec = DMX_VIDEO_AVS;
            break;
        case ENUM_SWDMX_VDEC_VP6:
            eVDec = DMX_VIDEO_VP6;
            break;
        case ENUM_SWDMX_VDEC_VP8:
            eVDec = DMX_VIDEO_VP8;
            break;
        default:
            eVDec = DMX_VIDEO_MPEG;
            break;
        }

        #ifdef CC_DMX_CONN_TYPE_SUPPORT
#if defined(SWDMX_IS_POST_MT5368) // || defined(CC_MT5365) || defined(CC_MT5395)
        DMX_MUL_SetVideoType(prSwdmxInst->u1DmxId, eVDec);
#else
        DMX_SOURCE_T eSource;

        if((prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_192) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
           (prSwdmxInst->eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            eSource = DMX_SOURCE_TSFILE0;
        }
        else
        {
            eSource = DMX_SOURCE_DRAM0;
        }
        DMX_SetSourceVideoType(eSource, eVDec);
#endif
        #else
        DMX_SetVideoType(eVDec);
        #endif

    }
}

BOOL _SWDMX_GetVideoFifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    FBM_POOL_T* prFbmPool;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if((!pu4Addr) || (!pu4Size))
    {
        LOG(1, "_SWDMX_GetVideoBuffer pu4Addr or pu4Size null\n");
        return FALSE;
    }

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    if(prFbmPool == NULL)
    {
        LOG(1, "_SWDMX_GetVideoFifo FBM_GetPoolInfo Fail\n");
        return FALSE;
    }
    if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
    {
        LOG(1, "_SWDMX_GetVideoFifo prFbmPool->u4Addr/u4Size 0\n");
        return FALSE;
    }

#if defined(CC_VOMX_TV_COEXIST)
    // VDEC 0 is used in DTV. FBM DMX is used in DTV
    if(prSwdmxInst->u1VdecId == ES1)
    {
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else if(prSwdmxInst->u1VdecId == ES0)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
        if(prFbmPool == NULL)
        {
            LOG(1, "_SWDMX_GetVideoFifo FBM_GetPoolInfo Fail\n");
            return FALSE;
        }
        if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
        {
            LOG(1, "_SWDMX_GetVideoFifo prFbmPool->u4Addr/u4Size 0\n");
            return FALSE;
        }
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else
    {
        LOG(1, "_SWDMX_GetVideoFifo u1VdecId(%d) unknown\n", prSwdmxInst->u1VdecId);
        return FALSE;
    }
#elif defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
    if(prSwdmxInst->u1VdecId == ES0)
    {
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else if(prSwdmxInst->u1VdecId == ES1)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
        if(prFbmPool == NULL)
        {
            LOG(1, "_SWDMX_GetVideoFifo FBM_GetPoolInfo Fail\n");
            return FALSE;
        }
        if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
        {
            LOG(1, "_SWDMX_GetVideoFifo prFbmPool->u4Addr/u4Size 0\n");
            return FALSE;
        }
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else
    {
        LOG(1, "_SWDMX_GetVideoFifo u1VdecId(%d) unknown\n", prSwdmxInst->u1VdecId);
        return FALSE;
    }
#elif defined(CC_FBM_TWO_FBP)
    *pu4Addr          = prFbmPool->u4Addr +
        (prSwdmxInst->u1VdecId * (prFbmPool->u4Size / 2));
    *pu4Size          = (prFbmPool->u4Size / 2);
#else
    *pu4Addr          = prFbmPool->u4Addr;
    *pu4Size          = prFbmPool->u4Size;
#endif

    _rSwdmxKeepInfo.u4SwdmxVFifoStart[u1SrcId] = *pu4Addr;
    _rSwdmxKeepInfo.u4SwdmxVFifoEnd[u1SrcId] = (*pu4Addr) + (*pu4Size) + 1;

    return TRUE;
}


BOOL _SWDMX_GetVideo2Fifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    FBM_POOL_T* prFbmPool;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if((!pu4Addr) || (!pu4Size))
    {
        LOG(1, "_SWDMX_GetVideo2Fifo pu4Addr or pu4Size null\n");
        return FALSE;
    }

    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    if(prFbmPool == NULL)
    {
        LOG(1, "_SWDMX_GetVideo2Fifo FBM_GetPoolInfo Fail\n");
        return FALSE;
    }
    if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
    {
        LOG(1, "_SWDMX_GetVideo2Fifo prFbmPool->u4Addr/u4Size 0\n");
        return FALSE;
    }

#if defined(CC_VOMX_TV_COEXIST)
    // VDEC 0 is used in DTV. FBM DMX is used in DTV
    if(prSwdmxInst->u1Vdec2Id == ES1)
    {
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else if(prSwdmxInst->u1Vdec2Id == ES0)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
        if(prFbmPool == NULL)
        {
            LOG(1, "_SWDMX_GetVideoFifo FBM_GetPoolInfo Fail\n");
            return FALSE;
        }
        if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
        {
            LOG(1, "_SWDMX_GetVideoFifo prFbmPool->u4Addr/u4Size 0\n");
            return FALSE;
        }
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else
    {
        LOG(1, "_SWDMX_GetVideoFifo u1VdecId(%d) unknown\n", prSwdmxInst->u1VdecId);
        return FALSE;
    }
#elif defined(CC_FBM_TWO_FBP_SHARED_WITH_DFB)
    if(prSwdmxInst->u1Vdec2Id == ES0)
    {
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else if(prSwdmxInst->u1Vdec2Id == ES1)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX2);
        if(prFbmPool == NULL)
        {
            LOG(1, "_SWDMX_GetVideo2Fifo FBM_GetPoolInfo Fail\n");
            return FALSE;
        }
        if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
        {
            LOG(1, "_SWDMX_GetVideo2Fifo prFbmPool->u4Addr/u4Size 0\n");
            return FALSE;
        }
        *pu4Addr          = prFbmPool->u4Addr;
        *pu4Size          = prFbmPool->u4Size;
    }
    else
    {
        LOG(1, "_SWDMX_GetVideo2Fifo u1VdecId(%d) unknown\n", prSwdmxInst->u1VdecId);
        return FALSE;
    }
#elif defined(CC_FBM_TWO_FBP)
    *pu4Addr          = prFbmPool->u4Addr +
        (prSwdmxInst->u1Vdec2Id * (prFbmPool->u4Size / 2));
    *pu4Size          = (prFbmPool->u4Size / 2);
#else
    *pu4Addr          = prFbmPool->u4Addr;
    *pu4Size          = prFbmPool->u4Size;
#endif

    //_rSwdmxKeepInfo.u4SwdmxVFifoStart[u1SrcId] = *pu4Addr;
    //_rSwdmxKeepInfo.u4SwdmxVFifoEnd[u1SrcId] = (*pu4Addr) + (*pu4Size) + 1;

    return TRUE;
}


BOOL _SWDMX_GetAudioPreFifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    if((!pu4Addr) || (!pu4Size))
    {
        LOG(1, "_SWDMX_GetVideoBuffer pu4Addr or pu4Size null\n");
        return FALSE;
    }

    if(_u4APreFIFO[0]!= 0)
    {
        *pu4Addr = _u4APreFIFO[0];
        *pu4Size = _u4APreFIFOSize;
    }
    return TRUE;
}


BOOL _SWDMX_GetAudioFifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    UINT32 au4BufStart[2], au4BufEnd[2];
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

    if((!pu4Addr) || (!pu4Size))
    {
        LOG(1, "_SWDMX_GetVideoBuffer pu4Addr or pu4Size null\n");
        return FALSE;
    }

#ifdef AUD_OLDFIFO_INTF
    if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
         &au4BufEnd[1]) != AUD_OK)
#else
    if (AUD_GetAudFifo(prSwdmxInst->u1AdecId, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
    {
        LOG(1, "_SWDMX_GetAudioFifo Can't get audio buffers!\n");
        return FALSE;
    }

    *pu4Addr          = au4BufStart[0];
    *pu4Size          = au4BufEnd[0] - au4BufStart[0];

    return TRUE;
}

#ifdef CC_SWDMX_RENDER_PTS
EXTERN VOID _SWDMX_SetRenderPts(UINT32 u4RenderPts)
{
    _u4RenderPts = u4RenderPts;
}
#endif

#ifdef CC_SWDMX_RENDER_POSITION_TEST
EXTERN VOID _SWDMX_GetRenderPosition(VOID *prRenderPos)
{
    x_memcpy(prRenderPos, &_rRenderPos, sizeof(START_POS_INFO_T));
}


EXTERN VOID _SWDMX_SetRenderPosition(VOID *prRenderPos)
{
    _fgSetRenderPosition = TRUE;
    x_memcpy(&_rRenderPos, prRenderPos, sizeof(START_POS_INFO_T));
}
#endif


UINT64 _SWDMX_Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder)
{
    if(u8Dividend == 0)
    {
        *pu8Remainder = 0;
        return 0;
    }
    if (u8Divider >> 32)
    {
        return u8Div6464(u8Dividend, u8Divider, pu8Remainder);
    }
    return u8Div6432(u8Dividend, u8Divider, pu8Remainder);
}

BOOL _SWDMX_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    /* dmx thread is not realtime callback
    if((prPes->u4FrameAddr <
        _rSwdmxKeepInfo.u4SwdmxVFifoStart[_rSwdmxKeepInfo.ucLockSwdmxId]) ||
       (prPes->u4FrameAddr >
        _rSwdmxKeepInfo.u4SwdmxVFifoEnd[_rSwdmxKeepInfo.ucLockSwdmxId]))
    {
        LOG(3, "Please check video fifo address setting\n");
        ASSERT(0);
    }
    */
    UNUSED(_rSwdmxKeepInfo.ucLockSwdmxId);
    UNUSED(_rSwdmxKeepInfo.u4SwdmxVFifoStart);
    UNUSED(_rSwdmxKeepInfo.u4SwdmxVFifoEnd);

    if(prSwdmxInst->pfnSwdmxSendVideoPes)
    {
        return prSwdmxInst->pfnSwdmxSendVideoPes(prPes);
    }
    LOG(3, "pfnSwdmxSendVideoPes is null\n");
    return FALSE;
}

BOOL _SWDMX_AudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    //DMX_AUDIO_PES_T rPes;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
    SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);
    if(!prSwdmxInst->pfnSwdmxSendAudioPes)
    {
        LOG(3, "pfnSwdmxSendAudioPes is null\n");
        return FALSE;
    }
    // some api of dmx set pid still not have audio device id
    // modify audio device id here temporally.
    /*
    if((prPes->u1DeviceId == 0) &&
       (prSwdmxInst->u1AdecId != 0))
    {
        x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));
        rPes.u1DeviceId = prSwdmxInst->u1AdecId;
        return prSwdmxInst->pfnSwdmxSendAudioPes(&rPes);
    }
    */
    return prSwdmxInst->pfnSwdmxSendAudioPes(prPes);
}

BOOL SWDMX_PVR_Init(UINT8 u1SrcId)
{
    return _SWDMX_PVR_Init(u1SrcId);
}
BOOL SWDMX_PVR_Pause(UINT8 u1SrcId)
{
    return _SWDMX_PVR_Pause(u1SrcId);
}
BOOL SWDMX_PVR_Resume(UINT8 u1SrcId)
{
    return _SWDMX_PVR_Resume(u1SrcId);
}
BOOL SWDMX_PVR_FlushBuf(UINT8 u1SrcId)
{
    return _SWDMX_PVR_FlushBuf(u1SrcId);
}
BOOL SWDMX_PVR_Stop(UINT8 u1SrcId)
{
    return _SWDMX_PVR_Stop(u1SrcId);
}
BOOL SWDMX_PVR_SetPid(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T ePidType,
                              BOOL fgEnable)
{
    return _SWDMX_PVR_SetPid(u1SrcId, u1Pidx, u2PidNum, ePidType, fgEnable);
}
void SWDMX_PVR_SetTime(UINT8 u1SrcId, UINT32 u4TimSec)
{
    return _SWDMX_PVR_SetTime(u1SrcId, u4TimSec);
}
BOOL SWDMX_PVR_GetInfo(UINT8 u1SrcId, SWDMX_PROGRAM_INFO_T* prOpenInfo)
{
    return _SWDMX_PVR_GetInfo(u1SrcId, prOpenInfo);
}
BOOL SWDMX_PVR_OPEN(UINT8 u1SrcId)
{
    return _SWDMX_PVR_OPEN(u1SrcId);
}
BOOL SWDMX_PVR_OPEN2(UINT8 u1SrcId, UINT32 u4BufStart, UINT32 u4BufSize)
{
    return _SWDMX_PVR_OPEN2(u1SrcId, u4BufStart, u4BufSize);
}
BOOL SWDMX_PVR_Close(UINT8 u1SrcId)
{
    return _SWDMX_PVR_Close(u1SrcId);
}
BOOL SWDMX_PVR_SetDataMode(UINT8 u1SrcId, INT32 i4PlaySpeed)
{
    return _SWDMX_PVR_SetDataMode(u1SrcId, i4PlaySpeed);
}
SWDMX_PVR_SENDDATA_RET_T SWDMX_PVR_SendData(UINT8 u1SrcId,
    const DMX_MM_DATA_T *prDmxMMData,
    UINT32 u4MinBuf,
    BOOL fgFastMode)
{
    return _SWDMX_PVR_SendData(u1SrcId, prDmxMMData, u4MinBuf, fgFastMode);
}

#if defined(SWDMX_DBG_USB) && defined(__KERNEL__)
BOOL SWDMX_Release_source()
{
    UINT8 u1SrcId = 0;
    SWDMX_INFO_T *prSwdmxInst = NULL;

    for (u1SrcId=0; SWDMX_SOURCE_MAX>u1SrcId; u1SrcId++)
    {
            prSwdmxInst = _SWDMX_GetInst(u1SrcId);
            SWDMX_VERIFY_NULL_ERROR(prSwdmxInst);

            x_memset(&prSwdmxInst->rDbgInfo, 0, sizeof(MM_DBG_INFO_T));
    }

    return TRUE;
}
#endif

