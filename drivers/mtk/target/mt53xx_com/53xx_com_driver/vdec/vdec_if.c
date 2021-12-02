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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_if.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_if.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "vdec_debug.h"
#include "vdec_if.h"
#include "vdec_drvif.h"
#include "vdec_flw.h"
#include "vdec_esm.h"
#include "vdec_type.h"
#if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
#ifndef CC_VDEC_H264_DISABLE
#include "vdec_h264api.h"
#endif
#endif
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_VDEC_VC1_DISABLE
#include "vdec_wmvapi.h"
#endif
#ifndef CC_VDEC_MPEG4_DISABLE
#include "vdec_mpeg4api.h"
#endif
#ifndef CC_VDEC_MJPEG_DISABLE
#include "vdec_img.h"
#endif
#ifndef CC_VDEC_RV_DISABLE
#include "vdec_rvapi.h"
#endif
#endif
#include "vdp_if.h"
#include "drv_dbase.h"
#include "fbm_drvif.h"
#include "imgrz_if.h"
#include "drvcust_if.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_hal_5381.h"
#include "x_chip_id.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#ifndef REPEAT_FRM
#define REPEAT_FRM 5
#endif
#define DROP_FRM (6)
#define CODEC_CHK_DATASZ (0x3000 * ((UINT32)VDEC_CODEC_CHK_TIMER_THRSD/1000))    // 12kbps
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BOOL _fgVDecInitiated = FALSE;
static VDEC_PES_INFO_T _rPesInfo; // this is for cli

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------

static VOID _DataTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucEsId;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    PARAM_MPV_T *prMpvCounter;
   
	ucEsId = (UCHAR)(UINT32)pv_tag;
	prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
	prMpvCounter = prVdecEsInfo->prMpvCounter;

	if (ucEsId >= MPV_MAX_ES)
	{
		return;
	}    

	if((prVdecEsInfo->u4NsAtTimeout == prMpvCounter->u4RetrieveNs) &&
		(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_NO_DATA))
	{
        _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_FLUSH);

        _rPesInfo.ucEsId = ucEsId;
        _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;
        VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
        
		//callback, no data receive
		if ((prVdecEsInfoKeep->pfDecNfyFct) &&
		    (!prVdecEsInfo->fgMMPlayback))
		{
			LOG(3, "Notify Status change, no data\n");
			prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, 
				VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_NO_DATA, 0);
		}
		prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NO_DATA;
	}

	prVdecEsInfo->u4NsAtTimeout = prMpvCounter->u4RetrieveNs;

	UNUSED(pt_tm_handle);
}

#if 0
static VOID _CodecChkTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UCHAR ucEsId;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    PARAM_MPV_T *prMpvCounter;
    BOOL fgErrNotify = TRUE;
   
    ucEsId = (UCHAR)(UINT32)pv_tag;
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prMpvCounter = prVdecEsInfo->prMpvCounter;

    if (ucEsId >= MPV_MAX_ES)
    {
        return;
    }    

    if((prMpvCounter->u4RetrieveNs > 0) && (prMpvCounter->u4DecOkNs == 0))
    {
        LOG(1, "RecvPesInfoCnt %d but can't dec any frm\n", prMpvCounter->u4RetrieveNs);
    }
    else if((prMpvCounter->u4RetrieveNs == 0) && (prVdecEsInfo->u4VFifoDataSz > CODEC_CHK_DATASZ))
    {
        LOG(1, "RecvDataSz 0x%x but can't find start code\n", prVdecEsInfo->u4VFifoDataSz);
    }
    else
    {
        fgErrNotify = FALSE;
        LOG(1, "RecvDataSz 0x%x, PesInfoCnt %d\n", 
            prVdecEsInfo->u4VFifoDataSz, prMpvCounter->u4RetrieveNs);
    }

    if(fgErrNotify)
    {
        //callback, no data receive
        if (prVdecEsInfoKeep->pfDecNfyFct)
        {
            LOG(3, "Notify Status change, data error\n");
            prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, 
                VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_ERROR, 0);
        }
    }
    prVdecEsInfoKeep->fgCodecTypeChecking = FALSE;
    VERIFY(x_timer_stop(prVdecEsInfoKeep->hCodecChkTimer) == OSR_OK);
    
    UNUSED(pt_tm_handle);
}
#endif

static VDEC_ASPECT_RATIO_T _DecideAspectRatio(UCHAR ucAspectRatio, UINT32 u4Width, UINT32 u4Height)
{
	if (ucAspectRatio != MPEG_ASPECT_RATIO_1_1)
	{
		switch (ucAspectRatio)
		{
			case MPEG_ASPECT_RATIO_4_3:
				return VDEC_ASPECT_RATIO_4_3;

			case MPEG_ASPECT_RATIO_16_9:
				return VDEC_ASPECT_RATIO_16_9;

			case MPEG_ASPECT_RATIO_221_1:
			    return VDEC_ASPECT_RATIO_221_1;

			case MPEG_ASPECT_RATIO_10_11:
			    return VDEC_ASPECT_RATIO_10_11;

			case MPEG_ASPECT_RATIO_40_33:
			    return VDEC_ASPECT_RATIO_40_33;

			case MPEG_ASPECT_RATIO_16_11:
			    return VDEC_ASPECT_RATIO_16_11;

            case MPEG_ASPECT_RATIO_12_11:
			    return VDEC_ASPECT_RATIO_12_11;
			    
            case MPEG_ASPECT_RATIO_3_2:
			    return VDEC_ASPECT_RATIO_3_2;

			default:
				return VDEC_ASPECT_RATIO_221_1;
		}
	}
	else
	{
		UINT32 u4Temp;

		// 4:3 = 1.33
		// 16:9 = 1.78
		// 2.21:1 = 2.21

		// if width / height < (1.78 + 1.33) / 2 >> 4:3
		// else if width / height < (2.21 + 1.78) / 2 >> 16:9
		// else 2.21 : 1
		// (1.78 + 1.33) / 2 = 14 / 9
		// (2.21 + 1.78) / 2 ~= 2

		u4Temp = (((u4Width * 9) / 14) / u4Height);

		if (u4Temp < 1)
		{
			return VDEC_ASPECT_RATIO_4_3;
		}
		else
		{
			u4Temp = ((u4Width / 2) / u4Height);

			if (u4Temp < 1)
			{
				return VDEC_ASPECT_RATIO_16_9;
			}
			else
			{
				return VDEC_ASPECT_RATIO_221_1;
			}
		}
	}
}

static VDEC_ASPECT_RATIO_T _AVCDecideAspectRatio(UCHAR ucAspectRatio, UINT32 u4Width, UINT32 u4Height)
{    
    switch (ucAspectRatio)
    {            
        case MPEG_ASPECT_RATIO_16_9:
            return VDEC_ASPECT_RATIO_16_9;
            
        case MPEG_ASPECT_RATIO_221_1:
            return VDEC_ASPECT_RATIO_221_1;

        default:
            break;
    }
    UNUSED(u4Width);
    UNUSED(u4Height);
    return VDEC_ASPECT_RATIO_4_3;
}

static void _VDPReapeatDropCB(UINT32 u4VdpId, UINT32 u4RepeatDrop)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_STATUS_CB_T rStatusCB;

    if(prVdecInfo->pfnStatusCb)
    {
        rStatusCB.u1EsId = prVdecInfo->rPesInfo.ucEsId;
        if(u4RepeatDrop == (UINT32)REPEAT_FRM)
        {
            rStatusCB.u4Status = (UINT32)VDEC_FRM_REPEAT;
        }
        else if(u4RepeatDrop == (UINT32)DROP_FRM)
        {
            rStatusCB.u4Status = (UINT32)VDEC_FRM_DROP;
        }
        else
        {
            return;
        }
        prVdecInfo->pfnStatusCb(&rStatusCB);
    }
    UNUSED(u4VdpId);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

VOID VDEC_GetQueueInfo(UCHAR ucEsId, UINT16* pu2QueueSize,UINT16* pu2MaxQueueSize)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_INFO_T *prVdecInfo;
    
    if (!_fgVDecInitiated)
    {
        return;
    }
    
    if((!pu2MaxQueueSize) || (!pu2QueueSize))
    {
        LOG(3, "VDEC_GetQueueInfo Invalid Parameter \n");
        return;
    }
    
    *pu2QueueSize = 0;
    *pu2MaxQueueSize = 0;
    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    if(prVdecEsInfoKeep && prVdecInfo)
    {
        *pu2MaxQueueSize = VDEC_MSG_Q_SIZE;
        if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
        {
            *pu2MaxQueueSize = H264_MAX_PES_NS_PER_QUEUE;
        }
        else
        {
            *pu2MaxQueueSize = MPV_MAX_PES_NS_PER_QUEUE;
        }        
        
        VERIFY(x_msg_q_num_msgs(prVdecEsInfoKeep->hMsgQ, pu2QueueSize) == OSR_OK);
    }
}


//Synchronous command
void VDEC_Init(void)
{
    HANDLE_T hThread;
    VDEC_INFO_T *prVdecInfo;
    UCHAR ucMpvId, ucEsId;
    CHAR szBuf[16];
    DRV_DBASE_T* _prDbase;
    CRIT_STATE_T _rCritState;
    #if defined(CC_MT5360) || defined(CC_MT5363)
    UINT32 u4Clk_MC, u4Clk_SYS;
    #endif
    
    prVdecInfo = _VDEC_GetInfo();
    _prDbase = NULL;
    
    if (!_fgVDecInitiated)
    {
        _fgVDecInitiated = TRUE;

        //vdec clock src selection
        #if defined(CC_MT5360) || defined(CC_MT5363)

        u4Clk_MC = IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) & 0xFFFFFFF0;
        u4Clk_SYS = IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) & 0xFFFFFFF0;

        #if defined(CC_MT5360)
        if (BSP_GetIcVersion() < IC_VER_5360_AD)
        {
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, (u4Clk_MC | VDEC_MC_MC_CLK));
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, (u4Clk_SYS | VDEC_MC_SYS_CLK));
        }
        else
        #endif
        {
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, (u4Clk_MC | VDEC_MC_MC_CLK_ECO_B));
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, (u4Clk_SYS | VDEC_MC_SYS_CLK_ECO_B));
        }
            #ifdef CC_DYNAMIC_POWER_ONOFF   //power off
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) | VDEC_PWR_OFF);
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) | VDEC_PWR_OFF);
            #endif
        #elif defined(CC_MT5387)            
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
            #endif     
        #elif defined(CC_MT5365)
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, VDEC_MC_MC_CLK);
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
            #endif
        #else //if defined(CC_MT5391)
        IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, VDEC_MC_SYS_CLK);
            #ifdef CC_DYNAMIC_POWER_ONOFF  //power off
            IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
            #endif
        #endif
        
        x_memset((void *)prVdecInfo, 0, sizeof(VDEC_INFO_T));    //init state = VDEC_ST_STOP

        _VDEC_InitInfo();
    
        prVdecInfo->ePreviousState = prVdecInfo->eCurState;
        prVdecInfo->eCurState = VDEC_ST_STOP;
        prVdecInfo->fgVDecInitiated = TRUE;
        prVdecInfo->fgWaitIFrame = TRUE;

        // create thread semephore
        for (ucMpvId = 0; ucMpvId < MPV_MAX_VLD; ucMpvId++)
        {
            VERIFY (x_sema_create(&prVdecInfo->arVdecVldInfo[ucMpvId].hVldSemaphore, X_SEMA_TYPE_MUTEX,
                X_SEMA_STATE_UNLOCK) == OSR_OK);
        }
        
        for (ucEsId = 0; ucEsId < MPV_MAX_ES; ucEsId++)
        {
            SPrintf(szBuf, "VDEC%3d", ucEsId);

            prVdecInfo->arVdecEsInfo[ucEsId].eNotifyStatus
                = VDEC_DEC_DECODE_UNKNOWN;
            prVdecInfo->arVdecEsInfo[ucEsId].fgMMSeqInfoValid = FALSE;
            _prDbase = DBS_Lock(&_rCritState);
            ASSERT(_prDbase);
            VERIFY(DBS_Unlock(_prDbase, _rCritState));        
            prVdecInfo->arVdecEsInfo[ucEsId].prMpvCounter = &(_prDbase->rMpv[ucEsId]);

            // create thread
            _VDEC_CreateEsmSema(ucEsId);
            VERIFY(x_thread_create(&hThread, szBuf, VDEC_STACK_SIZE,
                VDEC_THREAD_PRIORITY,  _VDEC_MainLoop, 4, (void*)&ucEsId) == OSR_OK);
        }

        // Query Op code
        #ifndef CC_VDEC_MPEG2_DISABLE
        if(!DRVCUST_OpCodeQuery(eIsSupportMP2))
        #endif
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MPEG2;
        }
        #ifndef CC_VDEC_VC1_DISABLE
        if(!DRVCUST_OpCodeQuery(eIsSupportVC1))
        #endif
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_VC1;
        }
        
        #ifdef CC_VDEC_MPEG4_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MPEG4;
        }
        #endif
        #ifdef CC_VDEC_H264_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_H264;
        }
        #endif
        #ifdef CC_VDEC_MJPEG_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MJPEG;
        }
        #endif
        #ifdef CC_VDEC_RV_DISABLE
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_RV;
        }
        #endif
        /*
        if(!DRVCUST_OpCodeQuery(eIsSupportMPEG4))
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_MPEG4;
        }
        if(!DRVCUST_OpCodeQuery(eIsSupportH264))
        {
            prVdecInfo->u4OpCode |= VDEC_OP_DISABLE_H264;
        }*/
    }
}

//Synchronous command
void VDEC_Termint(void)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if (prVdecInfo->fgVDecInitiated)
    {
        UCHAR ucEsId;

        for (ucEsId = 0; ucEsId < VDEC_MAX_ES; ucEsId++)
        {
            prVdecInfo->arVdecEsInfoKeep[ucEsId].fgThreadDestroy = TRUE;
        }
    }
}

//Synchronous command
void VDEC_Reset(void)
{
    _VDEC_InitInfo();
}

//Asynchronous command
BOOL VDEC_Play(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    // should assign later
    //prVdecInfo->eCurFMT = eFmt;

    ASSERT(ucEsId < VDEC_MAX_ES);

    //power on VDEC module
    //move to main loop, need sync with VDEC thread
    
    prVdecEsInfo->fgPlayOneFrm = FALSE;
    /*
    if(FBM_ChkFrameBufferFlagIsr(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE))
    {
        FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
        FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    }
    */
    
    if((eFmt != prVdecInfo->eCurFMT) && (prVdecInfo->eCurState == VDEC_ST_PLAY))
    {
        return FALSE;
    }

    if((prVdecEsInfo->fgMMPlayback) &&
       (prVdecEsInfo->fgSendedEos) &&
       (prVdecInfo->eCurState == VDEC_ST_PLAY))
    {
        return FALSE;
    }
    
    prVdecInfo->eCurFMT = eFmt;
	if(!prVdecEsInfo->fgMMPlayback)
	{
		#if (defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)) && !defined(CC_VDEC_H264_DISABLE)
		if(prVdecInfo->eCurFMT == VDEC_FMT_H264) // format will be mpv by default before _VDEC_DecCfg
		{
			VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, H264_FIFO_CTRL_INIT) == STC_VALID);
		}
		else
		#endif
		{
			VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, MPV_FIFO_CTRL_INIT) == STC_VALID);
		}
	}
	
    if(eFmt == VDEC_FMT_H264)
    {
        #ifndef CC_VDEC_FMT_DETECT
            #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
            #ifndef CC_VDEC_H264_DISABLE
            if(!_VDEC_IsSupportH264())
            #endif
            {
                LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
                return FALSE;
            }
            #else
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
            #endif
        #endif
    }    
    else if(eFmt == VDEC_FMT_WMV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
        if(!_VDEC_IsSupportVc1())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_RV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE
        if(!_VDEC_IsSupportRV())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MP4)
    {
        #ifdef CC_VDEC_MPEG4_DISABLE        
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MJPEG)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE   
        if(!_VDEC_IsSupportMJPEG())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MPV)
    {
	    #if defined(CC_MT5387) && !defined(CC_MT5363)
        if((!prVdecEsInfo->fgMMPlayback) && (!IS_SupportDTV()))
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #endif
    }
    else if(eFmt >= VDEC_FMT_MAX)
    {
        LOG(1, "Video codec type %d err!!\n", (UINT32)eFmt);
        return FALSE;
    }
	
    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY);

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
    
#ifndef CC_EMULATION
    if(prVdecEsInfoKeep->hDataTimer && !prVdecEsInfoKeep->fgTimerStarted
        && !prVdecEsInfo->fgMMPlayback)
    {
        VERIFY(x_timer_start(prVdecEsInfoKeep->hDataTimer, VDEC_DATA_TIMER_THRSD, X_TIMER_FLAG_REPEAT, 
            _DataTimeout, (void*)(UINT32)0) == OSR_OK);
        prVdecEsInfoKeep->fgTimerStarted = TRUE;
    }
#endif
    #if 0
    //check if codec type setting error
    if((!prVdecEsInfoKeep->fgCodecTypeChecking) && (!prVdecEsInfo->fgMMPlayback))
    {
        VERIFY(x_timer_start(prVdecEsInfoKeep->hCodecChkTimer, VDEC_CODEC_CHK_TIMER_THRSD, 
            X_TIMER_FLAG_ONCE, _CodecChkTimeout, (void*)(UINT32)0) == OSR_OK);
        prVdecEsInfoKeep->fgCodecTypeChecking = TRUE;
        prVdecEsInfo->u4VFifoDataSz = 0;
    }
    #endif
    return TRUE;
}


void VDEC_Play_I_Frame(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecInfo->eCurFMT = eFmt; 

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo->fgPlayOneFrm = FALSE;

    if(eFmt == VDEC_FMT_H264)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(!_VDEC_IsSupportH264())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return;
        #endif
    }
    else if(eFmt >= VDEC_FMT_MAX)
    {
        LOG(1, "Video codec type %d err!!\n", (UINT32)eFmt);
        return;
    }
    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY_I_FRAME);

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
}

void VDEC_PlayVideoClip(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    prVdecInfo->eCurFMT = eFmt; 

    ASSERT(ucEsId < VDEC_MAX_ES);

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY_VIDEO_CLIP);

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
}


//Asynchronous command
void VDEC_Stop(UCHAR ucEsId)
{
#if 0
    VDEC_INFO_T* prVdecInfo;
#endif
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        ASSERT(ucEsId < VDEC_MAX_ES);
        ucEsId = (VDEC_MAX_ES-1);
    }

#if 0
    prVdecInfo = _VDEC_GetInfo();
#endif
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    //power off VDEC module
    //move to main loop, need sync with VDEC thread

    VERIFY(STC_SetPtsDrift(prVdecEsInfo->ucStcId, MPV_FIFO_CTRL_INIT) == STC_VALID);

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_STOP);
    
    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    
#if 0
    // release 1 frame to empty Q to avoid waiting too long at get_frame_buffer
    if(prVdecInfo->eCurFMT == VDEC_FMT_H264)
    {
        if(prVdecInfo->eDecStatus == VDEC_DEC_HEADER_PARSE)
        {
            FBM_ReleaseOneFB(prVdecEsInfo->ucFbgId, prVdecEsInfo->ucFbId);
            prVdecEsInfo->ucFbId = FBM_FB_ID_UNKNOWN;
        }
    }
    else
    {
        FBM_ReleaseOneFB(prVdecEsInfo->ucFbgId, FBM_FB_ID_UNKNOWN);
    }
#endif    
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);

    MPV_StopCc(ucEsId);

     // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
    
    if(prVdecEsInfoKeep->hDataTimer && prVdecEsInfoKeep->fgTimerStarted
        && !prVdecEsInfo->fgMMPlayback)
    {
        VERIFY(x_timer_stop(prVdecEsInfoKeep->hDataTimer) == OSR_OK);
        prVdecEsInfoKeep->fgTimerStarted = FALSE;
    }		
}

//Asynchronous command
void VDEC_Pause(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PAUSE);
    if(!prVdecEsInfo->fgMMPlayback)
    {
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    }

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);

    // for time-shift, change from DTV to MM-time-shfit
    if(prVdecEsInfoKeep->hDataTimer && prVdecEsInfoKeep->fgTimerStarted
        && prVdecEsInfo->fgMMPlayback)
    {
        VERIFY(x_timer_stop(prVdecEsInfoKeep->hDataTimer) == OSR_OK);
        prVdecEsInfoKeep->fgTimerStarted = FALSE;
    }
}

//Asynchronous command
void VDEC_Flush_DPB(UCHAR ucEsId)
{
    ASSERT(ucEsId < VDEC_MAX_ES);

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_FLUSH_DPB);

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
}

void VDEC_SyncStc(UCHAR ucEsId, UCHAR ucMode, UCHAR ucStcId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    ASSERT(ucEsId < MPV_MAX_ES);
    ASSERT(prVdecEsInfo!=NULL);

    prVdecEsInfo->ucSyncMode = ucMode;
    prVdecEsInfo->ucStcId = ucStcId;

    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_AVSYNC);

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
}


//Synchronous command
void VDEC_RegEventNotifyFunc(PFN_VDEC_EVENT_NOTIFY pfFunc)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    prVdecInfo->pfErrNotify = pfFunc;
}
 
INT32 VDEC_SetDecNfy(UCHAR ucEsId, const VDEC_DEC_NFY_INFO_T* prDecNfyInfo)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
	
    if (prDecNfyInfo == NULL)
    {
        return (MPV_DRV_INV_SET_INFO);
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    prVdecEsInfoKeep->pvDecNfyTag = prDecNfyInfo->pvTag;
    prVdecEsInfoKeep->pfDecNfyFct = prDecNfyInfo->pfDecNfy;

    return (VDEC_DRV_OK);
}

BOOL VDEC_GetGnlInfo(UCHAR ucEsId, VDEC_GNL_INFO_T* prGnlInfo)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    if(prGnlInfo == NULL)
    {
        return FALSE;
    }
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(1, "ucEsId >= VDEC_MAX_ES\n");
        ASSERT(ucEsId < VDEC_MAX_ES);
        ucEsId = (VDEC_MAX_ES-1);
    }
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(prVdecEsInfo == NULL)
    {
        LOG(1, "VDEC_GetGnlInfo prVdecEsInfo null\n");
        return FALSE;
    }
    
    prGnlInfo->u2UdfNs = prVdecEsInfo->u2UdfNs;
    return TRUE;
}
    
//Synchronous command
BOOL VDEC_QueryInfo(UCHAR ucEsId, VDEC_HDR_INFO_T* prHdrInfo)
{
    FBM_SEQ_HDR_T *prFbmSeqHdr;
    FBM_PIC_HDR_T *prFbmPicHdr;
    VDEC_ES_INFO_T *prVdecEsInfo;
    UINT32 u4FbgId, u4FbId, u4VideoFmt = 5; //default value
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    
    if(prHdrInfo == NULL)
    {
        return FALSE;
    }
  	
    prHdrInfo->fgHdrInvalid = TRUE;
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    prHdrInfo->eNotifyStatus = prVdecEsInfo->eNotifyStatus;
    prHdrInfo->u4FailCnt = prVdecEsInfo->u4FailCnt;

    if (!VDEC_IsLock(ucEsId))
    {
    	return FALSE;
    }
    
    if(prVdecInfo->pfnVdecGetParam)
    {
        prVdecInfo->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4VideoFmt);
        prVdecEsInfo->ucVideoFmt = (UCHAR)u4VideoFmt;
    }
    else
    {
        u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        u4FbId = (UINT32)prVdecEsInfo->ucFbId;
    }
    
    if (u4FbgId == FBM_FBG_ID_UNKNOWN)
    {
        return FALSE;
    }
    
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr((UCHAR)u4FbgId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr((UCHAR)u4FbgId, (UCHAR)u4FbId);
    if (prFbmSeqHdr == NULL)
    {
        x_memset(prHdrInfo, 0, sizeof(VDEC_HDR_INFO_T));
        prHdrInfo->fgHdrInvalid = TRUE;
    	return FALSE;
    }
    else
    {
        prHdrInfo->fgHdrInvalid = FALSE;
        prHdrInfo->u2Width = prFbmSeqHdr->u2HSize;
        prHdrInfo->u2Height = prFbmSeqHdr->u2VSize;
        prHdrInfo->u2DHS = prFbmSeqHdr->u2DHS;
        prHdrInfo->u2DVS = prFbmSeqHdr->u2DVS;        
        prHdrInfo->u2OrgWidth = prFbmSeqHdr->u2OrgHSize;
        prHdrInfo->u2OrgHeight = prFbmSeqHdr->u2OrgVSize;
        prHdrInfo->ucVideoFmt = prVdecEsInfo->ucVideoFmt;
        if(prHdrInfo->eCodecType == VDEC_FMT_H264)
        {
            prHdrInfo->eAspectRatio = _AVCDecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        }
        else
        {
            prHdrInfo->eAspectRatio = _DecideAspectRatio(prFbmSeqHdr->ucAspRatInf,
                                                prFbmSeqHdr->u2OrgHSize, prFbmSeqHdr->u2OrgVSize);
        }
        prHdrInfo->u4BitRate = prFbmSeqHdr->u4BitRatVal;
        prHdrInfo->fgProgressiveSeq = prFbmSeqHdr->fgProgressiveSeq;
        prHdrInfo->ucAfd = prFbmSeqHdr->ucActFmt;
        prHdrInfo->fgNoOverScan = prFbmSeqHdr->fgNoOverScan;
        
        prHdrInfo->fgContrained = prVdecEsInfo->fgContrained;
        prHdrInfo->fgMPEG2 = prVdecEsInfo->fgMPEG2;
        prHdrInfo->fgSeqDispExt = prVdecEsInfo->fgSeqDispExt;
        prHdrInfo->fgLowDelay = prVdecEsInfo->fgLowDelay;
        prHdrInfo->ucProfileId = prVdecEsInfo->ucProfileId;
        prHdrInfo->u4VbvSizeExt = prVdecEsInfo->u4VbvSizeExt;
        
        prHdrInfo->ucPicType = prVdecEsInfo->ucPicType;

        if(prFbmPicHdr != NULL)
        {
            prHdrInfo->u2TempRef = (UINT16)prFbmPicHdr->i4TemporalRef;
            prHdrInfo->ucPicStruct = prFbmPicHdr->ucPicStruct;
            prHdrInfo->fgTFF = prFbmPicHdr->fgTopFldFirst;
            prHdrInfo->fgRFF = prFbmPicHdr->fgRepFirstFld;
            prHdrInfo->fgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
        }
        
        prHdrInfo->fgColorDescrip = prFbmSeqHdr->fgColorDescrip;
        prHdrInfo->ucMatrixCoefficients = prFbmSeqHdr->ucMatrixCoeff;
        prHdrInfo->ucTransferCharacteristics = prFbmSeqHdr->ucTransCharacter;
        prHdrInfo->ucColorPrimaries = prFbmSeqHdr->ucColourPrimaries;
        prHdrInfo->u4ChromaFmt = prFbmSeqHdr->ucCrmaFmt;

        switch (prFbmSeqHdr->ucFrmRatCod)
        {
            case MPEG_FRAME_RATE_24_:
                prHdrInfo->u2FrmRate = 2397;
                break;
            case MPEG_FRAME_RATE_24:
                prHdrInfo->u2FrmRate = 2400;            
                break;
            case MPEG_FRAME_RATE_25:
                prHdrInfo->u2FrmRate = 2500;            
                break;
            case MPEG_FRAME_RATE_30_:
                prHdrInfo->u2FrmRate = 2997;
                break;
            case MPEG_FRAME_RATE_30:
                prHdrInfo->u2FrmRate = 3000;
                break;
            case MPEG_FRAME_RATE_50:
                prHdrInfo->u2FrmRate = 5000;
                break;
            case MPEG_FRAME_RATE_60_:
                prHdrInfo->u2FrmRate = 5994;
                break;
            case MPEG_FRAME_RATE_60:
                prHdrInfo->u2FrmRate = 6000;
                break;
            default:
                prHdrInfo->u2FrmRate = 0;
                break;
        }
        //frame buffer number
        prHdrInfo->eCodecType = prVdecInfo->eCurFMT;
        if(prHdrInfo->eCodecType == VDEC_FMT_H264)
        {
            if(prVdecInfo->pfnVdecGetParam)
            {
                prVdecInfo->pfnVdecGetParam(ucEsId, 1, 
                    &prHdrInfo->u4Profile, &prHdrInfo->u4Level, &prHdrInfo->u4IsCABAC);
            }
        }
        prHdrInfo->u1CurState = (UINT8)prVdecInfo->eCurState;
        prHdrInfo->ucFbNum = FBM_GetFrameBufferNs((UCHAR)u4FbgId);
        prHdrInfo->u4MaxEsCnt = prVdecEsInfo->u4MaxEsCnt;
        prHdrInfo->u4CurEsCnt = (UINT32)prVdecEsInfo->u2Count;
        prHdrInfo->u4DisplayQPicCnt = prVdecEsInfo->u4DisplayQPicCnt;
        prHdrInfo->u4FirstDispPTS = prVdecEsInfo->u4FirstDispPTS;
        x_memcpy((void*)&prHdrInfo->au4DecTime[0], (void*)&prVdecEsInfo->au4DecTime[0], sizeof(prVdecEsInfo->au4DecTime));
    }
    return TRUE;
}

//Set parameter
void VDEC_SetParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    if(u4Type == 11)    //vdec_h264api.h
    {
        x_memset(prVdecEsInfo->au4DecTime, 0, sizeof(prVdecEsInfo->au4DecTime));    
    }
    else if(prVdecInfo->pfnVdecSetParam)
    {
        prVdecInfo->pfnVdecSetParam(ucEsId , u4Type, u4Param1, u4Param2, u4Param3);
    }
}

//Get parameter
void VDEC_GetParam(UCHAR ucEsId, UINT32 u4Type, UINT32* pu4Param1, UINT32* pu4Param2, UINT32* pu4Param3)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    
    if(prVdecInfo->pfnVdecGetParam)
    {
        prVdecInfo->pfnVdecGetParam(ucEsId, u4Type, pu4Param1, pu4Param2, pu4Param3);
    }
}

//Set parameter
void VDEC_SetWptr(UINT8 u1VdecId, UINT32 u4Wptr)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo((UCHAR)u1VdecId);
    FBM_POOL_T* prFbmPool;

    // get video fifo size from FBM
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != 0);

    if((prVdecEsInfo->u4VFifoPrevWptr > 0) && (prVdecEsInfo->u4VFifoDataSz <= CODEC_CHK_DATASZ))
    {
        if(u4Wptr > prVdecEsInfo->u4VFifoPrevWptr)
        {
            prVdecEsInfo->u4VFifoDataSz += (u4Wptr - prVdecEsInfo->u4VFifoPrevWptr);
        }
        else
        {
            prVdecEsInfo->u4VFifoDataSz += 
                ((u4Wptr + prFbmPool->u4Size) - prVdecEsInfo->u4VFifoPrevWptr);
        }
    }
    prVdecEsInfo->u4VFifoPrevWptr = u4Wptr;

    if(prVdecInfo->pfnVdecSetWptr)
    {
        prVdecInfo->pfnVdecSetWptr(u1VdecId, u4Wptr);
    }
}

BOOL VDEC_IsLock(UCHAR ucEsId)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    UINT32 u4FbgId, u4FbId;
    FBM_SEQ_HDR_T *prFbmSeqHdr;

    if(ucEsId >= VDEC_MAX_ES)
    {
        return FALSE;
    }
    
    if((prVdecInfo == NULL) || (prVdecEsInfo == NULL))
    {
        return FALSE;
    }

    /* MPV_IsLock is designed to check if mpv driver receive any video frame
       in a period. Original design is used for no signal check.
       Now we have decode status notify. If customer need that, we could give a
       notification to them.
    if(prVdecInfo->eCurFMT == VDEC_FMT_MPV)
    {
        return MPV_IsLock(ucEsId);
    }
    else
    */
    {
        if(prVdecInfo->pfnVdecGetParam)
        {
            prVdecInfo->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4FbId);
        }
        else
        {
            u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        }
    
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr((UCHAR)u4FbgId);
        if(!prFbmSeqHdr)
        {
            return FALSE;
        }
        if((prFbmSeqHdr->u2HSize > 0) &&
            (prFbmSeqHdr->u2VSize > 0) &&
            (prFbmSeqHdr->u2OrgHSize > 0) &&
            (prFbmSeqHdr->u2OrgVSize > 0))
        {
            return TRUE;
        }
    }
    
	// ToDo: add other format
    return FALSE;
}


//Set parameter
void VDEC_PlayMM(UCHAR ucEsId, BOOL fgPlayMM)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo)
    {
        prVdecEsInfo->fgMMPlayback = fgPlayMM;
    }
}


//Get parameter
BOOL VDEC_IsPlayMM(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(ucEsId >= VDEC_MAX_ES)
    {
        ucEsId = ES0;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo)
    {
        return prVdecEsInfo->fgMMPlayback;
    }
    return FALSE;
}


void VDEC_SetMMParam(UCHAR ucEsId, UINT32 u4Type, UINT32 u4Param1, UINT32 u4Param2, UINT32 u4Param3)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(u4Type == (UINT32)VDEC_MM_PLAY_SPEED)
    {
        prVdecEsInfo->u4MMSpeed = u4Param1;
    }
    else if(u4Type == (UINT32)VDEC_MM_SEQ_INFO)
    {
        x_memcpy((void*)&prVdecEsInfo->rMMSeqInfo, (void*)u4Param1, sizeof(VDEC_SEQUENCE_DATA_T));
        prVdecEsInfo->fgMMSeqInfoValid = TRUE;
    }
    else if(u4Type == (UINT32)VDEC_MM_TPT_SEQ_INFO)
    {
        x_memcpy((void*)&prVdecEsInfo->rMMTPTSeqInfo, (void*)u4Param1, sizeof(VDEC_SEQUENCE_DATA_T));
        prVdecEsInfo->fgMMTPTSeqInfoValid = TRUE;
    }    
    else if(u4Type == (UINT32)VDEC_MM_FRAME_RATE)
    {
        prVdecEsInfo->rMMSeqInfo.e_frame_rate = (ENUM_VDEC_FRAME_RATE_T)(u4Param1);
    }
#ifdef ENABLE_MULTIMEDIA
    else if(u4Type == (UINT32)VDEC_MM_CONTAINER_TYPE)
    {
        prVdecEsInfo->eContainerType = (ENUM_SWDMX_FMT_T)(u4Param1);
    }
    else if(u4Type == (UINT32)VDEC_MM_SRC_TYPE)
    {
        prVdecEsInfo->eMMSrcType = (SWDMX_SOURCE_TYPE_T)(u4Param1);
    }
#ifndef CC_VDEC_RV_DISABLE  
	else if (u4Type == (UINT32)VDEC_MM_RV_RPR_INFO)
	{
		UINT32 au4Size[9] = {0,1,1,2,2,3,3,3,3};
		INT32 i;
		ASSERT(u4Param1 < 9);

		prVdecEsInfo->u4SzOfPicSz = au4Size[u4Param1];
		for (i = 0; i < 2 * u4Param1; i++)
		{
			prVdecEsInfo->au4RPRSizes[i] = *(UINT32 *)u4Param2;
			u4Param2 += 4;
		}
		prVdecEsInfo->fgRV9 = (BOOL)u4Param3;
	}
#endif
#endif
    UNUSED(u4Param2);
    UNUSED(u4Param3);
}

//Set parameter
void VDEC_SetBigBParameter(UCHAR ucEsId, BOOL fgForceEnable, UINT32 u4BigBLevel1,
    UINT32 u4BigBLevel2, UINT32 u4BigBLevel3, UINT32 u4BigBBaseCnt)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    prVdecEsInfo->fgBigBForceEnable = fgForceEnable;
    prVdecEsInfo->u4BigBLevel1 = u4BigBLevel1;
    prVdecEsInfo->u4BigBLevel2 = u4BigBLevel2;
    prVdecEsInfo->u4BigBLevel3 = u4BigBLevel3;
    prVdecEsInfo->u4BigBBaseCnt = u4BigBBaseCnt;
}

//Set parameter
void VDEC_WaitSyncPoint(UCHAR ucEsId, BOOL fgTricking)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    #if 0
    PARAM_MPV_T *prMpvCounter;
    #endif
    
    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    #if 0
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;
    #endif
    
    MPV_FlushEsmQ(ucEsId, TRUE); // abort decoder? FixMe

    #if 0
    prVdecEsInfo->u4PicCntJump = prMpvCounter->u4DecOkNs;
    #endif
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    prVdecEsInfo->ucSequenceHdrFound = FALSE;
    prVdecEsInfo->fgKeepPes = FALSE;
    prVdecEsInfo->fgSendedEos = FALSE;
    prVdecInfo->rPesInfo.fgEos = FALSE;
    prVdecEsInfo->fgTricking = fgTricking;
}

//Get parameter
#if 0
UINT32 VDEC_GetDecPicCount(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    PARAM_MPV_T *prMpvCounter;
    
    prMpvCounter = (PARAM_MPV_T *)prVdecEsInfo->prMpvCounter;
    
    return prMpvCounter->u4DecOkNs - prVdecEsInfo->u4PicCntJump;
}
#endif
//clear the seqinfo flags
void VDEC_ClrMMParam(UCHAR ucEsId, UINT32 u4Type)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	prVdecEsInfo->fgMMSeqInfoValid = FALSE;

}

//clear the seqinfo flags
void VDEC_SetTPTFlag(UCHAR ucEsId, BOOL fgIsTPT)
{
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
	prVdecEsInfo->fgIsTPT = fgIsTPT;
}


//Get parameter
UINT32 VDEC_GetAvgPicSize(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    if(prVdecEsInfo == NULL)
    {
        LOG(3, "VDEC_GetAvgPicSize prVdecEsInfo null");
        return 0;
    }
    
    return prVdecEsInfo->u4AvgPicSize;
}



void VDEC_GetDecStatus(UCHAR ucEsId, BOOL* pfgLock
    , BOOL* pfgTopFldFirst, BOOL* pfgRepFirstFld, BOOL* pfgProgressiveFrm
    , UCHAR* pucPicType, UCHAR* pucPicStruture, UCHAR* pucTemporalRef
    , UINT32* pu4ReceiveNs, UINT32* pu4DecOkNs)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_INFO_T *prVdecInfo;
    PARAM_MPV_T* prMpvCounter;
    FBM_PIC_HDR_T *prFbmPicHdr = NULL;
    UINT32 u4FbgId;
    UINT32 u4FbId;
    UINT32 u4VideoFmt;

    if((pfgLock == NULL) ||
       (pfgTopFldFirst == NULL) ||
       (pfgRepFirstFld == NULL) ||
       (pfgProgressiveFrm == NULL) ||
       (pucPicType == NULL) ||
       (pucPicStruture == NULL) ||
       (pucTemporalRef == NULL) ||
       (pu4ReceiveNs == NULL) ||
       (pu4DecOkNs == NULL))
    {
        LOG(1, "VDEC_GetDecStatus pointer null");
        return;
    }
    
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecInfo = _VDEC_GetInfo();
    prMpvCounter = prVdecEsInfo->prMpvCounter;
    
    if (prVdecEsInfo->eNotifyStatus == VDEC_DEC_DECODE_NORMAL)
    {
        *pfgLock = TRUE;
    }
    else
    {
        *pfgLock = FALSE;
    }
    
    *pu4ReceiveNs = prMpvCounter->u4RetrieveNs;
    *pu4DecOkNs = prMpvCounter->u4DecOkNs;
    *pucPicType = prVdecEsInfo->ucPicType;
    
    if(prVdecInfo->pfnVdecGetParam)
    {
        prVdecInfo->pfnVdecGetParam(ucEsId, 0, &u4FbgId, &u4FbId, &u4VideoFmt);
    }
    else
    {
        u4FbgId = (UINT32)prVdecEsInfo->ucFbgId;
        u4FbId = (UINT32)prVdecEsInfo->ucFbId;
    }
    
    if (u4FbgId != FBM_FBG_ID_UNKNOWN)
    {
        prFbmPicHdr = FBM_GetFrameBufferPicHdr((UCHAR)u4FbgId, (UCHAR)u4FbId);
    }

    if(prFbmPicHdr)
    {
        *pucPicStruture = prFbmPicHdr->ucPicStruct;
        *pfgTopFldFirst = prFbmPicHdr->fgTopFldFirst;
        *pfgRepFirstFld = prFbmPicHdr->fgRepFirstFld;
        *pfgProgressiveFrm = prFbmPicHdr->fgProgressiveFrm;
        // ToDo: pucTemporalRef -> pi4TemporalRef
        *pucTemporalRef = (UCHAR)prFbmPicHdr->i4TemporalRef;
        *pucPicStruture = prFbmPicHdr->ucPicStruct;
    }
    else
    {    
        *pucPicStruture = 0;
        *pfgTopFldFirst = 0;
        *pfgRepFirstFld = 0;
        *pfgProgressiveFrm = 0;
        *pucTemporalRef = 0;
        *pucPicStruture = 0;
    }
    return;
}


BOOL  VDEC_RegDecErrCb(PFN_VDEC_DECERR_CB pfnDecErrCb, UINT32 u4ErrDuration)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (pfnDecErrCb == NULL)
    {
        return FALSE;
    }

    prVdecInfo->pfnDecErrCb = pfnDecErrCb;
    prVdecInfo->u4ErrDuration= u4ErrDuration;

    return TRUE;
}


BOOL VDEC_RegPicTypeCb(PFN_VDEC_PIC_TYPE_CB pfnPicTypeCb)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (!prVdecInfo)
    {
        return FALSE;
    }

    prVdecInfo->pfPicTypeCb = pfnPicTypeCb;

    return TRUE;
}


BOOL VDEC_RegStatusCb(PFN_VDEC_STATUS_CB pfnStatusCb)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (!prVdecInfo)
    {
        return FALSE;
    }

    prVdecInfo->pfnStatusCb = pfnStatusCb;
    VDP_RegCbFunc(VDP_CB_FUNC_REPEAT_DROP_IND, (UINT32)_VDPReapeatDropCB, 0, 0);

    return TRUE;
}


//Asynchronous command
BOOL VDEC_PlayOneFrm(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    UCHAR ucFRefFbId, ucBRefFbId;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    if((eFmt != prVdecInfo->eCurFMT) || 
        ((prVdecInfo->eCurState != VDEC_ST_STOP) &&
        (prVdecInfo->eCurState != VDEC_ST_IDLE) && 
        (prVdecInfo->eCurState != VDEC_ST_PLAY)) ) // trick mode in play state
    {
        return FALSE;
    }

    if(eFmt == VDEC_FMT_H264)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(!_VDEC_IsSupportH264())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }    
    else if(eFmt == VDEC_FMT_WMV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
        if(!_VDEC_IsSupportVc1())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MP4)
    {
        #ifdef CC_VDEC_MPEG4_DISABLE        
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MJPEG)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE   
        if(!_VDEC_IsSupportMJPEG())
        #endif
        {
            LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_RV)
    {
#ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE   
        if(!_VDEC_IsSupportRV())
        #endif
    {
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
    }
        #else
        LOG(1, "Video codec format %d not support!!", (UINT32)eFmt);
        return FALSE;
#endif
    }
    
    {
        LOG(5, "VDEC_PlayOneFrm\n");
        prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
        prVdecEsInfo->fgPlayOneFrm = TRUE;

        //Put Backward Reference pic to EMPTY
        FBM_GetRefFrameBuffer(prVdecEsInfo->ucFbgId, &ucFRefFbId, &ucBRefFbId);
        //prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
        //if(prFbmPicHdr)
        {
            if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_READY))
            {
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                LOG(3, "VDEC_PlayOneFrm ucBRefFbId %d\n", ucBRefFbId);
            }
            if(FBM_CheckFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_READY))
            {
                FBM_SetFrameBufferStatus(prVdecEsInfo->ucFbgId, ucFRefFbId, FBM_FB_STATUS_EMPTY);
                LOG(3, "VDEC_PlayOneFrm ucFRefFbId %d\n", ucFRefFbId);            
            }
        }

        /*
        FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
        FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
        */
        
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;

        _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_PLAY);
    }
    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);
    
    return TRUE;
}


BOOL  VDEC_RegRenderPtsCb(PFN_VDEC_RENDER_PTS_CB pfnRenderPtsCb)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (!prVdecInfo)
    {
        return FALSE;
    }

    prVdecInfo->pfnRenderPtsCb = pfnRenderPtsCb;

    return TRUE;
}


BOOL  VDEC_RegTrickPtsCb(PFN_VDEC_TRICK_PTS_CB pfnTrickPtsCb)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();

    if (!prVdecInfo)
    {
        return FALSE;
    }

    // trick -> normal, will register VDEC_RegTrickPtsCb = NULL;
    prVdecInfo->pfnTrickPtsCb = pfnTrickPtsCb;

    return TRUE;
}


BOOL  VDEC_SetRenderPts(UCHAR ucEsId, UINT32 u4Pts)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderPts = TRUE;
    prVdecEsInfo->u4RenderPts = u4Pts;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = FALSE;
    
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;
    prVdecEsInfo->fgKeepPes = FALSE;
    prVdecEsInfo->fgSendedEos = FALSE;
    prVdecInfo->rPesInfo.fgEos = FALSE;

    return TRUE;
}

BOOL  VDEC_SetTotalTime(UCHAR ucEsId, UINT32 u4TotalTime)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u4TotalTime = u4TotalTime;

    return TRUE;
}

BOOL  VDEC_SetFileSize(UCHAR ucEsId, UINT64 u8FileSize)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u8FileSize = u8FileSize;

    return TRUE;
}

BOOL  VDEC_SetLastTime(UCHAR ucEsId, UINT32 u4LastTime)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u4PTSLast = u4LastTime;

    return TRUE;
}

BOOL  VDEC_SetWaitPvrSps(UCHAR ucEsId, UINT8 u1PVRWaitSps)
{    
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->u1PVRWaitSps = u1PVRWaitSps;

    return TRUE;
}

BOOL  VDEC_SetRenderBackStep(UCHAR ucEsId, BOOL fgRenderBackStep, UINT64 u8Pos,
    INT32 i4TempRef, UINT16 u2DecodingOrder)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderBackStep = fgRenderBackStep;
    
    if(fgRenderBackStep)
    {
        prVdecEsInfo->u8RenderFromPos = u8Pos;
        prVdecEsInfo->i4RenderTempRef = i4TempRef;
        prVdecEsInfo->u2RenderDecodingOrder = u2DecodingOrder;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;
        
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL  VDEC_SetRenderFromPos(UCHAR ucEsId, BOOL fgRenderFromPos,
    UINT64 u8Pos, INT32 i4TempRef, UINT16 u2DecodingOrder)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderFromPos = fgRenderFromPos;
    
    if(prVdecEsInfo->fgRenderFromPos)
    {
        prVdecEsInfo->u8RenderFromPos = u8Pos;
        prVdecEsInfo->i4RenderTempRef = i4TempRef;
        prVdecEsInfo->u2RenderDecodingOrder = u2DecodingOrder;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;
        
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}


BOOL  VDEC_SetRenderFromPts(UCHAR ucEsId, UINT32 u4Pts)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderFromPts = TRUE;
    prVdecEsInfo->u4RenderFromPts = u4Pts;
    prVdecEsInfo->u2Disp2EmptyCnt = 0;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = FALSE;
    prVdecEsInfo->fgAutoPause = FALSE;
    prVdecInfo->rPesInfo.fgEos = FALSE;
    
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;

    return TRUE;
}


BOOL  VDEC_SetRenderFromPtsAutoPause(UCHAR ucEsId, UINT32 u4Pts, BOOL fgAutoPause)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderFromPts = TRUE;
    prVdecEsInfo->u4RenderFromPts = u4Pts;
    prVdecEsInfo->u2Disp2EmptyCnt = 0;
    prVdecEsInfo->u4PTSLast = 0;
    prVdecEsInfo->fgSetStartPts = FALSE;
    prVdecEsInfo->fgAutoPause = fgAutoPause;
    prVdecInfo->rPesInfo.fgEos = FALSE;
    
    SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
    //prVdecEsInfo->ucSequenceHdrFound = FALSE;

    return TRUE;
}


BOOL  VDEC_SetRenderLastFrame(UCHAR ucEsId, BOOL fgRenderLastFrame)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderLastFrame = fgRenderLastFrame;
    
    if(prVdecEsInfo->fgRenderLastFrame)
    {
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;
        
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL VDEC_SetRenderFromFirstPic(UCHAR ucEsId,
    BOOL fgRenderFromFirstPic, UINT64 u8LastRenderPos)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderFromFirstPic = fgRenderFromFirstPic;

    if(fgRenderFromFirstPic)
    {
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;
        prVdecEsInfo->u8RenderFromPos = u8LastRenderPos;
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;
    }

    return TRUE;
}

BOOL VDEC_SetRenderPicCount(UCHAR ucEsId, BOOL fgRenderPicCnt, UINT32 u4Count)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgRenderPicCnt = fgRenderPicCnt;
    prVdecEsInfo->u4RenderPicCnt = u4Count;
    if(fgRenderPicCnt)
    {
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;

        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;
    }
    
    return TRUE;
}

BOOL VDEC_SetGetSeqHdrOnlyBegin(UCHAR ucEsId,
    ENUM_VDEC_FMT_T eFmt, BOOL *pfgPowerOff)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prVdecEsInfo->fgOnlyGotSeqHeader = TRUE;
    
    *pfgPowerOff = (!prVdecInfo->arVdecVldInfo[VLD0].fgPowerOn);
    if(!prVdecInfo->arVdecVldInfo[VLD0].fgPowerOn)
    {
        UNUSED(VDEC_PowerOn(VLD0));
    }

    if((eFmt != prVdecInfo->eCurFMT) ||
       (prVdecInfo->eCurState == VDEC_ST_STOP))
    {
        prVdecInfo->eCurFMT = eFmt;
        UNUSED(_VDEC_DecCfg(ES0));
    }
    
    return TRUE;
}

BOOL VDEC_SetGetSeqHdrOnlyEnd(UCHAR ucEsId, BOOL fgPowerOff)
{
    VDEC_INFO_T *prVdecInfo;
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecInfo = _VDEC_GetInfo();
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(prVdecEsInfo->fgOnlyGotSeqHeader)
    {
        if((prVdecInfo->pfnVdecStop) && (prVdecInfo->eCurState == VDEC_ST_STOP))
        {
            prVdecInfo->pfnVdecStop(ES0);
        }
        // I am not sure if we need this.
        //MPV_FlushEsmQ(ES0, FALSE);

        // for resume stop, we should assign this variable
        if(!prVdecEsInfo->ucSequenceHdrFound)
        {
            // copy from mpv_if.c _MPV_WaitRandomAP(...)
            prVdecEsInfo->ucSequenceHdrFound = 1;
            prVdecEsInfo->fgChgThrsd = TRUE;
            prVdecEsInfo->u4TotalPicSize = 0;
            prVdecEsInfo->u4PicCnt = 0;
            prVdecEsInfo->u4LastVldRptr = 0;
        }
    
        if(fgPowerOff)
        {
            UNUSED(VDEC_PowerOff(VLD0));
        }
    }
    prVdecEsInfo->fgOnlyGotSeqHeader = FALSE;
    
    return TRUE;
}

BOOL  VDEC_SetSuperFreeze(UCHAR ucEsId, BOOL fgSuperFreeze)
{    
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    if(fgSuperFreeze)
    {
        LOG(5, "VDEC_SetSuperFreeze!!\n");
        
        MPV_FlushEsmQ(ucEsId, FALSE);
        
        prVdecEsInfo->u2Disp2EmptyCnt = 0;
        prVdecEsInfo->u4PTSLast = 0;
        prVdecEsInfo->fgSetStartPts = FALSE;
        
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
        SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_AVC_RAP);
        //prVdecEsInfo->ucSequenceHdrFound = FALSE;
        prVdecEsInfo->fgKeepPes = FALSE;
        prVdecEsInfo->fgSendedEos = FALSE;
        prVdecInfo->rPesInfo.fgEos = FALSE;

        FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SUPER_FREEZE);
        prVdecEsInfo->fgTricking = FALSE;        
        prVdecEsInfo->fgMMPlayback = TRUE;
        VDEC_Pause(ucEsId);
        LOG(5, "VDEC_SetSuperFreeze exit!!\n");
    }

    return TRUE;
}

BOOL  VDEC_GetVideoThumbnail(UCHAR ucEsId, UINT16 u2CompId, BOOL bIsOnePass, 
    VDP_THUMBNAIL_INFO_T* prDstInfo, VDP_CAPTURE_INTO_T* prSrcInfo)
{
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    
    if(ucEsId >= VDEC_MAX_ES)
    {
        LOG(0, "VDEC_GetVideoThumbnail ucEsId %d!!", (UINT32)ucEsId);
        return FALSE;
    }
    if((prDstInfo == NULL) || (prSrcInfo == NULL))
    {
        LOG(0, "VDEC_GetVideoThumbnail pointer null!!");
        return FALSE;
    }
    
    prVdecEsInfo->u2CompId = u2CompId;
    x_memcpy((void*)&prVdecEsInfo->rCapDstInfo, (void*)prDstInfo, sizeof(VDP_THUMBNAIL_INFO_T));
    x_memcpy((void*)&prVdecEsInfo->rCapSrcInfo, (void*)prSrcInfo, sizeof(VDP_CAPTURE_INTO_T));
    prVdecEsInfo->bIsOnePass = bIsOnePass;
    
    _VDEC_SendCmd(ucEsId, (UINT32)VDEC_CMD_CAPTURE);

    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    _rPesInfo.ucMpvId = (UCHAR)VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);

    UNUSED(prSrcInfo);
    UNUSED(prDstInfo);
    return TRUE;
}

//Synchronous command, only called when pause/idle/stop
BOOL VDEC_ReleaseDispQ(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    
    ASSERT(ucEsId < VDEC_MAX_ES);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
        
    FBM_SetFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    FBM_ReleaseDispQ(prVdecEsInfo->ucFbgId);
    FBM_ClrFrameBufferFlag(prVdecEsInfo->ucFbgId, FBM_FLAG_SEEK_MODE);
    
    prVdecEsInfo->fgFlushFB = TRUE; //for h.264
    return TRUE;
}

BOOL VDEC_PowerOn(UCHAR ucVldId)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if(ucVldId >= VDEC_MAX_VLD)
    {
        ASSERT(ucVldId < VDEC_MAX_VLD);
        return FALSE;
    }
    
    //power on VDEC module
    #ifdef CC_DYNAMIC_POWER_ONOFF
    #if defined(CC_MT5360) || defined(CC_MT5363)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) & VDEC_PWR_ON);
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) & VDEC_PWR_ON);
    #elif defined(CC_MT5387)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) & VDEC_PWR_ON);
    #elif defined(CC_MT5365)
    IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_ON);
    #else //if defined(CC_MT5391)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) & VDEC_PWR_ON);
    #endif
    #endif
    prVdecInfo->arVdecVldInfo[ucVldId].fgPowerOn = TRUE;
    return TRUE;
}

BOOL VDEC_PowerOff(UCHAR ucVldId)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if(ucVldId >= VDEC_MAX_VLD)
    {
        ASSERT(ucVldId < VDEC_MAX_VLD);
        return FALSE;
    }
    
    //power off VDEC module
    #ifdef CC_DYNAMIC_POWER_ONOFF
    #if defined(CC_MT5360) || defined(CC_MT5363)
    IMGRZ_Lock();
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_MC, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_MC) | VDEC_PWR_OFF);
    IMGRZ_Unlock();
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL_SYS, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL_SYS) | VDEC_PWR_OFF);
    #elif defined(CC_MT5387)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
    #elif defined(CC_MT5365)
    IO_WRITE32(VDEC_CTRL_BASE, VDEC_PWR_CTRL, VDEC_PWR_OFF);
    #else //if defined(CC_MT5391)
    IO_WRITE32(CKGEN_BASE, VDEC_CLK_SEL, IO_READ32(CKGEN_BASE, VDEC_CLK_SEL) | VDEC_PWR_OFF);
    #endif
    #endif
    
    prVdecInfo->arVdecVldInfo[ucVldId].fgPowerOn = FALSE;
    return TRUE;
}

void VDEC_StartPTS(UCHAR ucEsId, BOOL fgEnCalPTS, UINT32 u4Pts, VDEC_PTS_INFO_T* prPTSInfo)
{
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    prVdecEsInfo->fgEnCalPTS = fgEnCalPTS;
    prVdecEsInfo->u4StartPTS = u4Pts;
    if(prPTSInfo)
    {
        x_memcpy((void*)&prVdecEsInfo->rPTSInfo, (void*)prPTSInfo, sizeof(VDEC_PTS_INFO_T));
    }
    if(prVdecInfo->pfnVdecStartPts)
    {
        _VDEC_FlushEs(ucEsId);
        UNUSED(VDEC_ReleaseDispQ(ucEsId));
    
        prVdecInfo->pfnVdecStartPts(ucEsId , fgEnCalPTS, u4Pts, (UINT32)prPTSInfo);     
    }
}

BOOL VDEC_ChkCodecCap(UCHAR ucEsId, ENUM_VDEC_FMT_T eFmt)
{
    UNUSED(ucEsId);

    if(eFmt == VDEC_FMT_H264)
    {
        #if defined(ENABLE_MULTIMEDIA) || defined(CC_H264_SUPPORT)
        #ifndef CC_VDEC_H264_DISABLE
        if(!_VDEC_IsSupportH264())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_WMV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_VC1_DISABLE
        if(!_VDEC_IsSupportVc1())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MP4)
    {
        #ifdef CC_VDEC_MPEG4_DISABLE        
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_MJPEG)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_MJPEG_DISABLE   
        if(!_VDEC_IsSupportMJPEG())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }
    else if(eFmt == VDEC_FMT_RV)
    {
        #ifdef ENABLE_MULTIMEDIA
        #ifndef CC_VDEC_RV_DISABLE   
        if(!_VDEC_IsSupportRV())
        #endif
        {
            LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
            return FALSE;
        }
        #else
        LOG(1, "Video codec format %d not support!!\n", (UINT32)eFmt);
        return FALSE;
        #endif
    }

    return TRUE;

}

BOOL VDEC_ChkDivXBonding(VOID)
{
    #if defined(CC_MT5391)
    return IS_DivxSupport();
    #elif defined(CC_MT5360) || defined(CC_MT5363)
    return IS_SupportDivX();
    #else
    return FALSE;
    #endif
}

BOOL VDEC_SetMid(UCHAR ucEsId, BOOL fgVdecMidEnable)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);

    if(prVdecEsInfoKeep)
    {
        prVdecEsInfoKeep->fgVdecMidEnable = fgVdecMidEnable;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL VDEC_Notify(UCHAR ucEsId,VDEC_DEC_DECODE_STATUS_T notifyStatus)
{
 VDEC_ES_INFO_T *prVdecEsInfo;
 VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep; 
 
 prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
 prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
 if(notifyStatus==VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
 {
 	if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_CODEC_NOT_SUPPORT)
 	{
	 	//callback, codec not support mpv data
	 	if (prVdecEsInfoKeep->pfDecNfyFct)
		{
		 	LOG(3, "Notify Status change, mpeg2 codec not support\n");
		 	prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, 
		 	VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
	 	}
	 	prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_CODEC_NOT_SUPPORT;
 	}
	return TRUE;
 }
  if(notifyStatus==VDEC_DEC_DECODE_NO_DATA)
 {
 	if(prVdecEsInfo->eNotifyStatus != VDEC_DEC_DECODE_NO_DATA)
 	{
	 	//callback, codec not support mpv data
	 	if (prVdecEsInfoKeep->pfDecNfyFct)
		{
		 	LOG(3, "Notify Status change, mpeg2 codec no data \n");
		 	prVdecEsInfoKeep->pfDecNfyFct(prVdecEsInfoKeep->pvDecNfyTag, 
		 	VDEC_COND_DECODE_STATUS_CHG, (UINT32)VDEC_DEC_DECODE_NO_DATA, 0);   
	 	}
	 	prVdecEsInfo->eNotifyStatus = VDEC_DEC_DECODE_NO_DATA;
 	}
	return TRUE;
 } 
 else
 {
    LOG(3, "undefine notify status--%dt\n",notifyStatus);
 	return FALSE;
 }
}



