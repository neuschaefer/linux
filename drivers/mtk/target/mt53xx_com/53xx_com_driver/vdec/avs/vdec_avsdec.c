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
 * $RCSfile: vdec_avsdec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_avsdec.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "x_ckgen.h"
#include "srm_drvif.h"
#include "vdec_avshw.h"
#include "vdec_avsutil.h"
#include "vdec_avsdec.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "stc_drvif.h"
#include "x_util.h"
#ifdef FBM_DPB_CTRL
#include "fbm_drvif.h"
#endif

#include "vdec_drvif.h"
#include "drv_dbase.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define POC_abs(x)                  (((x) >= 0) ? (x) : -(x))
#define FB_ALIGN  0xF
#define FRAME_RATE_MAX 9
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static BOOL _fgVDecComplete = FALSE;
static HAL_TIME_T _rTimeHwS, _rTimeHwDt, _rTimeHwE;

#if 0
static UINT32 u4DecTimeCnt = 0;
static UINT32 u4DecCycle = 0, u4MaxDecCycle = 0;
#endif

static HANDLE_T _hVdecFinSema = (HANDLE_T)NULL;
static BOOL _bTimeOutFlag = FALSE;


#ifdef VDEC_TIME_PROFILE
static UINT32 u4DecCycle = 0;
static UINT32 u4DelayClock;
static UINT32 u4DramCycle;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _AVLD_AVSIsr(UINT16 u2Vector);

#if 0
static BOOL _bNotifyAudStartPlay(UCHAR ucStcId, UINT32 u4PTS);
#endif

static void _vAllocateFBuf(VDEC_AVS_INFO_T *prVdec);

extern void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference);

static void _VDEC_AvsFillQTable2Sram(void);

static void _VDEC_AvsSetMcReg(const VDEC_AVS_INFO_T* prVdec, INT32 *pi4Ret);


static void _VDEC_AvsSetMvReg(const VDEC_AVS_INFO_T* prVdec, INT32 *pi4Ret);

static void _VDEC_AvsSetPpReg(const VDEC_AVS_INFO_T* prVdec, INT32 *pi4Ret);

// *********************************************************************
// Function : void _AVLDIsr
// Description : 
// Parameter : 
// Return    : 
// *********************************************************************

static void _AVLD_AVSIsr(UINT16 u2Vector)
{
    #ifdef VDEC_TIME_PROFILE
    _rTimeHwDt.u4Micros = 0;
    #endif
    
    if (u2Vector == AVLD_VECTOR)
    {
        if (AVS_VLD_READ32(RO_AVS_VLD_COMPLETE))
        {
            AVS_VLD_WRITE32(RW_AVS_VLD_COMPLETE_CLR, 1);
            AVS_VLD_WRITE32(RW_AVS_VLD_COMPLETE_CLR, 0);

            //#ifdef VDEC_TIME_PROFILE
            HAL_GetTime(&_rTimeHwE);
            HAL_GetDeltaTime(&_rTimeHwDt, &_rTimeHwS, &_rTimeHwE);

#if 0
            if ((rTimeHwDt.u4Seconds/600) > u4DecTimeCnt)
            {
                LOG(3, "HwHdr %u.%06us\n", rTimeHwDt.u4Seconds, rTimeHwDt.u4Micros);
                u4DecTimeCnt = rTimeHwDt.u4Seconds/600;
            }
#endif
            
            //#endif            

            if ((AVS_VLD_READ32(RO_AVS_ERROR_TYPE) & 0x80000000))
            {
                LOG(3, "Avs hw timeout!!!\n");
                _bTimeOutFlag = TRUE;
            }
             //unlock semaphore
            VERIFY(x_sema_unlock(_hVdecFinSema) == OSR_OK);
        }
#if 0
        else if(AVS_VLD_READ32(RO_AVLD_ERR_MESSAGE) & AVLD_ERR_MESSAGE_MSK)
        {
            _bTimeOutFlag = TRUE;
            AVS_VLD_WRITE32(RW_AVLD_TIMEOUT_CLR, 1);
            AVS_VLD_WRITE32(RW_AVLD_TIMEOUT_CLR, 0);
        }
#endif        
        else
        {
            LOG(3, "Unknown state for avs isr\n");
        }
    }
}


#if 0
// *********************************************************************
// Function    : BOOL _bNotifyAudStartPlay(void)
// Description : notify audio when first video frame ready to play
// Parameter   : None
// Return      : None
// *********************************************************************
static BOOL _bNotifyAudStartPlay(UCHAR ucStcId, UINT32 u4PTS)
{
    if (STC_SetStartPts(AV_SYNC_TARGET_VDO, ucStcId, u4PTS) != STC_VALID)
    {
        LOG(3, "STC_SetStartPts Invalid\n");
        return FALSE;
    }
    LOG(5, "STC_SetStartPts 0x%x\n", u4PTS);
    
    return TRUE;
}             
#endif


// *********************************************************************
// Function    : void vAllocateFBuf()
// Description : Allocate decoding frm buff in DPB
// Parameter   : 
// Return      : None
// *********************************************************************
static void _vAllocateFBuf(VDEC_AVS_INFO_T *prVdec)
{
    UINT32 i;
    UINT32 u4FbIdx = VDEC_AVS_UNKNOWN_IDX;
    UCHAR ucFbStatus;

    FBM_PIC_HDR_T * prFbmPicHdr = NULL;

    VDEC_AVS_PIC_T *prPicPrm = &prVdec->rPicHdr;
    VDEC_AVS_DECPRM_T *prDecPrm = &prVdec->rDecParam;
    
    for(i = 0; i < prDecPrm->u4MaxFBufNum; i++)
    {
        ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId);
        if(prDecPrm->arFBufInfo[i].u1FBufStatus == NO_PIC)
        {
            LOG(6, "prDecPrm->arFBufInfo[i].u1FBufStatus == NO_PIC\n");
            u4FbIdx = i;
            break;
        }
        else if(fgIsNonRefFBuf(prDecPrm->arFBufInfo[i].u1FBufRefType, prDecPrm->arFBufInfo[i].u1TFldRefType, prDecPrm->arFBufInfo[i].u1BFldRefType))
        {
            if((ucFbStatus != FBM_FB_STATUS_DISPLAYQ) && (ucFbStatus != FBM_FB_STATUS_LOCK))
            {
                LOG(6, "(ucFbStatus != FBM_FB_STATUS_DISPLAYQ) && (ucFbStatus != FBM_FB_STATUS_LOCK)\n");
                u4FbIdx = i;
                break;
            }
        }
    }

    LOG(6, "u4FbIdx = %d\n", u4FbIdx);

    if (u4FbIdx < prDecPrm->u4MaxFBufNum)
    {
        // No empty DPB, 1 FBuf output
        if(prDecPrm->arFBufInfo[u4FbIdx].u1FBufStatus != NO_PIC)
        {
            LOG(6, "///// ClrBuf in AllocFBuf, Idx %d , Status %d(>0)\n", 
                u4FbIdx, 
                (UINT32)FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4FbIdx].ucFbId));

            LOG(6, "%s Set fb to empty %d\n", __FUNCTION__,
                prDecPrm->arFBufInfo[u4FbIdx].ucFbId);
            
            //find an empty fb entry, put non-ref buf to empty
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[u4FbIdx].ucFbId, 
                FBM_FB_STATUS_EMPTY);
            _VDEC_ClrAvsFBufInfo(prDecPrm, u4FbIdx, TRUE);
        }

        prDecPrm->u1DecFBufIdx = (UINT8)u4FbIdx;
        if (prDecPrm->u1DecFBufIdx > prVdec->rDecParam.u4MaxFBufNum)
        {
            ASSERT(prDecPrm->u1DecFBufIdx <= prVdec->rDecParam.u4MaxFBufNum);
            return;
        }
        prDecPrm->prCurrFBufInfo = &prDecPrm->arFBufInfo[prDecPrm->u1DecFBufIdx];

        prDecPrm->prCurrFBufInfo->ucFbId = FBM_GetEmptyFrameBuffer(prDecPrm->ucFbgId, prDecPrm->u4WaitFbTime);
        LOG(6, "Empty fb id = %d\n", prDecPrm->prCurrFBufInfo->ucFbId);
    }
    else
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "AllocFB Fail!!\n");
        return;
    }

    LOG(6, "Allocate FB id = %d\n", prDecPrm->prCurrFBufInfo->ucFbId);

    if(prDecPrm->prCurrFBufInfo->ucFbId == FBM_FB_ID_UNKNOWN)
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "AllocFB Fail!!\n");
        return;
    }
    else
    {
        prDecPrm->fgAllocFbFail = FALSE;

        // Record some frame information
        prDecPrm->prCurrFBufInfo->u1FBufStatus = FRAME;
        prDecPrm->prCurrFBufInfo->u4PicDistance = prPicPrm->u4PicDistance;
        prDecPrm->prCurrFBufInfo->u1PicCodingType = (UINT8)prPicPrm->u4PicCodingType;
        
        if ((prVdec->rPicHdr.u4PicCodingType == I_IMG) ||
            (prVdec->rPicHdr.u4PicCodingType == P_IMG))
        {
            prDecPrm->prCurrFBufInfo->u1FBufRefType = REF_PIC;
        }
    }

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId);
    if(prFbmPicHdr == NULL)
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "GetFbgPicHdr Fail!!\n");
        //ASSERT(0);
        return;
    }

    if (prVdec->rPesInfo.fgDtsValid)
    {
        prFbmPicHdr->u4PTS = prVdec->rPesInfo.u4PTS;
    }

    //prFbmPicHdr->u4PTS = prVdecEsInfoKeep->rPesInfo.u4PTS;
    //prDecPrm->prCurrFBufInfo->fgPTSValid = prVdecEsInfoKeep->rPesInfo.fgDtsValid;
#if 0
    FBM_UpdateReferenceList(prDecPrm->ucFbgId, 
        prDecPrm->prCurrFBufInfo->ucFbId, 
        fgIsRefPic(prVdec->rPicHdr.u4PicCodingType));
#endif

    FBM_GetFrameBufferAddr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId,
        &prDecPrm->prCurrFBufInfo->u4YAddr, &prDecPrm->prCurrFBufInfo->u4CAddr);

    if((prDecPrm->prCurrFBufInfo->u4YAddr == (UINT32)NULL) || 
    	(prDecPrm->prCurrFBufInfo->u4CAddr == (UINT32)NULL))
    {
        prDecPrm->fgAllocFbFail = TRUE;
        LOG(2, "FbAddr NULL!!\n");
        //ASSERT(0);
        return;
    }

    prDecPrm->pucDecWorkBufY = (UINT8*)(prDecPrm->prCurrFBufInfo->u4YAddr);
    prDecPrm->pucDecWorkBufC = (UINT8*)(prDecPrm->prCurrFBufInfo->u4CAddr);
    prDecPrm->u4CurDecMs = 0;
    prDecPrm->u4CurDramCycle = 0;
    prDecPrm->u4CurDecCycle = 0;
    //prDecPrm->pucDecWorkBufMV = (UINT8*)(prDecPrm->prCurrFBufInfo->u4MVBufAddr);
}


void _VDEC_AvsPrepareFBufInfo(VDEC_AVS_INFO_T *prVdec)
{
    VDEC_AVS_DECPRM_T *prDecPrm = &prVdec->rDecParam;

    if ((prDecPrm->u4LastHSize != prVdec->rSeqHdr.u4HSize) ||
        (prDecPrm->u4LastVSize != prVdec->rSeqHdr.u4VSize) ||
        (prDecPrm->ucFbgId == FBM_FBG_ID_UNKNOWN))
    {
        UINT8 u1FbgType;
        
        _VDEC_FlushAvsDPB(prDecPrm);
        FBM_ReleaseGroup(prDecPrm->ucFbgId);

        u1FbgType = FBM_SelectGroupType(prVdec->rSeqHdr.u4HSize , prVdec->rSeqHdr.u4VSize);
        
        UINT32 u4FbWidth = 0, u4FbHeight = 0;
        FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;    

        VDEC_ES_INFO_T *prVdecEsInfo;

        prVdecEsInfo = _VDEC_GetEsInfo(ES0);

        prDecPrm->ucFbgType = u1FbgType;
        
        // Should add FBM_VDEC_AVS
        prDecPrm->ucFbgId = FBM_CreateGroup(
                                            u1FbgType, 
                                            FBM_VDEC_AVS, 
                                            prVdec->rSeqHdr.u4HSize, 
                                            prVdec->rSeqHdr.u4VSize);
        if (prDecPrm->ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            LOG(3, "Fail to create FBM group type\n");
            return;
        }

        prVdecEsInfo->ucFbgId = prDecPrm->ucFbgId;

        FBM_GetFrameBufferSize(prDecPrm->ucFbgId, &u4FbWidth, &u4FbHeight);
        FBM_FbgChgNotify(prDecPrm->ucFbgId, 0);

        prVdec->rDecParam.u4LineSize = u4FbWidth;

        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prDecPrm->ucFbgId);
            FBM_ClrFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_NOT_READY);
        }

        prVdec->rDecParam.u4MaxFBufNum = FBM_GetFrameBufferNs(prDecPrm->ucFbgId);
        LOG(6, "u4MaxFBufNum = %d\n", prVdec->rDecParam.u4MaxFBufNum);
        
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->ucFbgId);
        if (!prFbmSeqHdr)
        {
            LOG(3, "Fail to get FBM sequence header\n");
            return;
        }

        prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdr.u4HSize;
        prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdr.u4VSize;

        prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdr.u4HSize;
        prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdr.u4VSize;

        prFbmSeqHdr->u2LineSize = (UINT16)u4FbWidth;

        prFbmSeqHdr->fgProgressiveSeq = (BOOL)prVdec->rSeqHdr.u4IsProgSeq;

        prDecPrm->u4LastHSize = prVdec->rSeqHdr.u4HSize;
        prDecPrm->u4LastVSize = prVdec->rSeqHdr.u4VSize;

        // ToDo (aspect ratio)
        switch (prVdec->rSeqHdr.u4AspRatio)
        {
            case 1:
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;
                LOG(5, "AspRatio -> 1:1\n");
                break;
            case 2:
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;
                LOG(5, "AspRatio -> 4:3\n");
                break;
            case 3:
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_16_9;
                LOG(5, "AspRatio -> 16:9\n");
                break;
            case 4:
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_221_1;
                LOG(5, "AspRatio -> 2.21:1\n");
                break;
            default:
                prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_4_3;
                LOG(5, "Unknown aspect ratio\n");
                break;
        }

        switch (prVdec->rSeqHdr.u4FrmRate)
        {
            case 1:
                LOG(5, "AVS Frame rate is 23.976\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24_;
                break;
            case 2:
                LOG(5, "AVS Frame rate is 24\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24;
                break;
            case 3:
                LOG(5, "AVS Frame rate is 25\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_25;
                break;
            case 4:
                LOG(5, "AVS Frame rate is 29.97\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30_;
                break;
            case 5:
                LOG(5, "AVS Frame rate is 30\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
                break;
            case 6:
                LOG(5, "AVS Frame rate is 50\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_50;
                break;
            case 7:
                LOG(5, "AVS Frame rate is 59.94\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60_;
                break;
            case 8:
                LOG(5, "AVS Frame rate is 60\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
                break;
            default:
                LOG(3, "AVS Unknown Frame rate\n");
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
                break;
        }
    }

    _vAllocateFBuf(prVdec);
    
}



// *********************************************************************
// Function    : void vDumpReg()
// Description : Dump decoder registers when decoding error
// Parameter   None
// Return      : None
// *********************************************************************
void _VDEC_DumpAvsReg(void)
{
    UINT32 u4Cnt, u4Temp;
    
    for (u4Cnt = 0x84; u4Cnt <= 0x9c; u4Cnt += 4)
    {
        u4Temp = VLD_READ32(u4Cnt);
        LOG(3, "0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
        x_thread_delay(1);
    }

    for (u4Cnt = 0xa8; u4Cnt <= 0x3ec; u4Cnt += 4)
    {
        u4Temp = VLD_READ32(u4Cnt);
        LOG(3, "0x%8x (vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
        x_thread_delay(1);
    }

    for (u4Cnt = 0x0; u4Cnt <= 0x934; u4Cnt += 4)
    {
        u4Temp = MC_READ32(u4Cnt);
        LOG(3, "0x%8x (mc Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
        x_thread_delay(1);
    }

     for (u4Cnt = 0x84; u4Cnt <= 0x13c; u4Cnt += 4)
    {
        u4Temp = AVS_VLD_READ32(u4Cnt);
        LOG(3, "0x%8x (AVS vld Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
        x_thread_delay(1);
    }

    for (u4Cnt = 0x0; u4Cnt <= 0xC0; u4Cnt += 4)
    {
        VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, (0x100 | u4Cnt));
        u4Temp = VLD_READ32(RW_AVS_VLD_QUANT_TABLE);
        LOG(3, "(AVS Qunat Pos %ld) =  0x%8x\n", u4Cnt, u4Temp);
        x_thread_delay(1);
    }

    u4Temp = AVS_MV_READ32(RW_AVS_BLK_FW_DISTANCE_1_0);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_BLK_FW_DISTANCE_1_0, (RW_AVS_BLK_FW_DISTANCE_1_0>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_BLK_FW_DISTANCE_3_2);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_BLK_FW_DISTANCE_3_2, (RW_AVS_BLK_FW_DISTANCE_3_2>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_BLK_BW_DISTANCE_1_0);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_BLK_BW_DISTANCE_1_0, (RW_AVS_BLK_BW_DISTANCE_1_0>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_BLK_FW_MED_RATIO_1_0);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_BLK_FW_MED_RATIO_1_0, (RW_AVS_BLK_FW_MED_RATIO_1_0>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_MV_FW_MED_RATIO_3_2);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_MV_FW_MED_RATIO_3_2, (RW_AVS_MV_FW_MED_RATIO_3_2>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_MV_BW_MED_RATIO_1_0);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_MV_BW_MED_RATIO_1_0, (RW_AVS_MV_BW_MED_RATIO_1_0>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_MV_DIRECT_RATIO_1_0);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_MV_DIRECT_RATIO_1_0, (RW_AVS_MV_DIRECT_RATIO_1_0>>2), u4Temp);
    x_thread_delay(1);
    u4Temp = AVS_MV_READ32(RW_AVS_MV_DIRECT_RATIO_3_2);
    LOG(3, "0x%8x (AVS MV Register %ld) =  0x%8x\n", RW_AVS_MV_DIRECT_RATIO_3_2, (RW_AVS_MV_DIRECT_RATIO_3_2>>2), u4Temp);
    x_thread_delay(1);


    for (u4Cnt = 0x0; u4Cnt <= 0x900; u4Cnt += 4)
    {
        u4Temp = PP_READ32(u4Cnt);
        LOG(3, "0x%8x (AVS PP Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
        x_thread_delay(10);
    }

    UNUSED(u4Temp);
}



//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

// **************************************************************************
// Function : void vInitVDecHW(DWRD dwRDPtrAddr);
// Description : Init Video Decoder
// Parameter : None
// Return      : TRUE: init OK.
//                   FALSE: init failed
// **************************************************************************
void _VDEC_InitAVSVDecHW(BOOL bIsInit)
{
    x_os_isr_fct pfnOldIsr;        
    if(_hVdecFinSema == (HANDLE_T)NULL)
    {
        VERIFY(x_sema_create(&_hVdecFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }
    // reg ISR        
    if (x_reg_isr(AVLD_VECTOR, _AVLD_AVSIsr, &pfnOldIsr) != OSR_OK)
    {
        ASSERT(0);
    }

    if(bIsInit)
    {
        _VDEC_AvsVLDHWReset(bIsInit);        
        #ifdef BARREL2_SUPPORT
        _VDEC_AvsVLDHWReset2(bIsInit);
        #endif
    }
}

// *********************************************************************
// Function : void _VDEC_SetAvsBsInfo
// Description : Set Bs fifo start addr, end addr, and current read ptr
// Parameter : u4RDPtrAddr, u4BsFifoS, u4BsFifoE
// Return    : None
// *********************************************************************
void _VDEC_SetAvsBsInfo(UINT8 u1Num, VDEC_AVS_DECPRM_T *prDecParam, UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr,
    UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd, UINT8 u1ECCMode, UINT32 u4ECType)
{    
    #ifdef WAIT_SRAM_STABLE
    UINT32 u4Cnt = 0;
    #endif

    AVS_VLD_WRITE32(RW_AVS_VLD_CTRL, 0x8007); 
    AVS_VLD_WRITE32(RW_AVS_VLD_MODE_SWITCH, 7);
    
    // Set for VLD boundary check 0x5000000
    #ifdef BARREL2_SUPPORT
    if(u1Num == BARREL2)
    {
        _VDEC_AvsVLDHWReset2(FALSE);
        while(!_VDEC_AvsResetVLD2(PHYSICAL(u4RDPtrAddr), PHYSICAL(u4WPtrAddr), 
            PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd)))
        {
            _VDEC_AvsVLDHWReset2(TRUE);
        }
    }
    else
    #else
    UNUSED(u1Num);
    #endif
    {
        #ifdef WAIT_SRAM_STABLE
#ifdef CC_AVS_EMULATION
#if 0
        while(!_VDEC_AvsResetVLD1(prDecParam, PHYSICAL(u4RDPtrAddr), PHYSICAL(0xffffffff),
            PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd), PHYSICAL(0), u1ECCMode, u4ECType))
#endif            
        while(!_VDEC_AvsResetVLD1(prDecParam, PHYSICAL(u4RDPtrAddr), PHYSICAL(u4WPtrAddr),
            PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd), PHYSICAL(0), u1ECCMode, u4ECType))
#else            
        while(!_VDEC_AvsResetVLD1(prDecParam, PHYSICAL(u4RDPtrAddr), PHYSICAL(u4WPtrAddr),
            PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd), PHYSICAL(0), u1ECCMode, u4ECType))
#endif            
        {
            LOG(2, "SwRst %d\n", u4Cnt++);
            _VDEC_AvsVLDHWReset(TRUE);
        }
        UNUSED(u4Cnt);
        #else
        _VDEC_AvsVLDSetVFifo(PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd));
    
        if(!_VDEC_AvsVLDInitBarrelShifter(prDecParam, u4RDPtrAddr, PHYSICAL(u4BsFifoStart), PHYSICAL(u4BsFifoEnd)))
        {
            LOG(2, "Barrel Shifter1 Init failed");
        }
        #endif
    }
}


// **************************************************************************
// Function : UINT8 _VDEC_WaitAvsDecDone(VDEC_AVS_INFO_T rDecPrm);
// Description : WaitDecDone
// Parameter : rDecPrm
// Return      : BOOL, if decode successfully
// **************************************************************************
UINT8 _VDEC_WaitAvsDecDone(VDEC_AVS_INFO_T *prVdec, 
    UINT8 u1PicStruct, 
    UINT32 u4W, 
    UINT32 u4H, 
    BOOL bWaitDecLock)
{
    UINT8 u1Ret = 0;
    //UINT32 u4CntTimeChk = 0;
    //UINT32 u4MbX, u4MbY;
    //UINT32 u4Tmp;
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;
    VDEC_INFO_T *prVdecInfo;   
    VDEC_STATUS_CB_T rStatusCB;
    //INT32 i;
    VDEC_AVS_DECPRM_T* prDecPrm;
    
    prVdecInfo = _VDEC_GetInfo();
    
    prDecPrm = (VDEC_AVS_DECPRM_T*)&prVdec->rDecParam;

    #ifndef AVS_IRQ_DISABLE
    if(bWaitDecLock)
    {
        if (x_sema_lock_timeout(_hVdecFinSema, AVS_WAIT_DEC_TIME) != OSR_OK)
        {
            u1Ret = 0;
#if 0
            LOG(3, "AVS decoder timeout!!!\n");
            LOG(3, "VLD 44 = 0x%x\n", VLD_READ32(0xB0));
            LOG(3, "VLD 45 = 0x%x\n", VLD_READ32(0xB4));
            LOG(3, "VLD 46 = 0x%x\n", VLD_READ32(0xB8));
            LOG(3, "VLD 62 = 0x%x\n", VLD_READ32(0xF8));
            LOG(3, "VLD 63 = 0x%x\n", VLD_READ32(0xFC));
            Printf("VLD 44 = 0x%x\n", VLD_READ32(0xB0));
            Printf("VLD 45 = 0x%x\n", VLD_READ32(0xB4));
            Printf("VLD 46 = 0x%x\n", VLD_READ32(0xB8));
            Printf("VLD 62 = 0x%x\n", VLD_READ32(0xF8));
            Printf("VLD 63 = 0x%x\n", VLD_READ32(0xFC));
            if (VLD_READ32(0xF8) != VLD_READ32(0xFC))
            {
                Printf("AVS decoder timeout!!!\n");
            }

            if (prVdec->rSeqHdr.u4IsProgSeq == prVdec->rPicHdr.u4ProgFrm)
            {
                while (1)
                {}
            }
            else
            {
                Printf("Progressive flag is not the same\n");
            }
#endif
            
            prDecPrm->u4TimeOutNum++;
            _bTimeOutFlag = TRUE;
            //_VDEC_DumpAvsReg();
        }
    }

#if 0
    {
        UINT32 u4Cnt = 0, u4Temp = 0;
        for (u4Cnt = 0x5E8; u4Cnt <= 0x634; u4Cnt += 4)
        {
            u4Temp = MC_READ32(u4Cnt);
            LOG(3, "0x%8x (mc Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
            x_thread_delay(1);
        }
        for (u4Cnt = 0x77c; u4Cnt <= 0x798; u4Cnt += 4)
        {
            u4Temp = MC_READ32(u4Cnt);
            LOG(3, "0x%8x (mc Register %ld) =  0x%8x\n", u4Cnt, (u4Cnt>>2), u4Temp);
            x_thread_delay(1);
        }
    }
    UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%sTest_%d.avs\" 0x%08x--0x%08x", 
        "c:\\", prVdec->rDecParam.u4PicCnt, 
        (UINT32)_pu1GoldenData, (UINT32)(_pu1GoldenData + prDispInfo->u4YSize)));
#endif

    if (AVS_VLD_READ32(RO_AVS_ERROR_TYPE))
    {
        LOG(3, "//Error TYPE (69) =0x%x !!!\n", AVS_VLD_READ32(RO_AVS_ERROR_TYPE));
        LOG(3, "//AVS_VLD ACC (70) Error acc 0x%x\n", AVS_VLD_READ32(RO_AVS_ERROR_ACC));
        LOG(3, "Bandwidth = %d\n", MC_READ32(0x770) * 480);
    }
#if 0
    LOG(3, "AVS VLD %d checksum = 0x%08x\n", RW_AVS_VLD_PIC_SIZE >> 2, AVS_VLD_READ32(RW_AVS_VLD_PIC_SIZE));
    LOG(3, "AVS VLD %d checksum = 0x%08x\n", RW_AVS_VLD_PHP >> 2, AVS_VLD_READ32(RW_AVS_VLD_PHP));
    LOG(3, "//Error TYPE (69) =0x%x !!!\n", AVS_VLD_READ32(RO_AVS_ERROR_TYPE));
    LOG(3, "//AVS_VLD ACC (70) Error acc 0x%x\n", AVS_VLD_READ32(RO_AVS_ERROR_ACC));
    for (i = 378; i < 397; i++)
    {
        LOG(6, "MC %d checksum = 0x%08x\n", i, MC_READ32(i *4));
    }

    LOG(6, "MBX = %d, MBY = %d\n", MC_READ32(0x28), MC_READ32(0x2c));
    LOG(6, "PP 129 checksum = 0x%08x\n", PP_READ32(129 *4));
    LOG(6, "PP 131 checksum = 0x%08x\n", PP_READ32(131 *4));
    LOG(6, "PP 323 checksum = 0x%08x\n", PP_READ32(323 *4));
    LOG(6, "PP 325 checksum = 0x%08x\n", PP_READ32(325 *4));
    LOG(6, "PP 576 checksum = 0x%08x\n", PP_READ32(576 *4));
    LOG(6, "PP 577 checksum = 0x%08x\n", PP_READ32(577 *4));
#endif

    u1Ret = (UINT8)(_bTimeOutFlag? 0: 1);
    if(u1PicStruct == FRAME)
    {
        _prDbase = DBS_Lock(&_rCritState);
        ASSERT(_prDbase);
        VERIFY(DBS_Unlock(_prDbase, _rCritState));        
        prInfo = &(_prDbase->rMpv[0]);
    
        if(_bTimeOutFlag == 1)
        {
            prInfo->u4TimeOutNs++;
            if(prVdecInfo->pfnStatusCb)
            {
                rStatusCB.u1EsId = prVdecInfo->rPesInfo.ucEsId;
                rStatusCB.u4Status = (UINT32)VDEC_PIC_DECODING_ERROR;
                prVdecInfo->pfnStatusCb(&rStatusCB);
            }            
        }
        else
        {
            prInfo->u4DecOkNs++;
        }
    }
    if(_bTimeOutFlag)
    {
        UINT32 u4Temp;
        
        LOG(2, "//Error! TimeOut, Error TYPE (69) =0x%x !!!\n", AVS_VLD_READ32(RO_AVS_ERROR_TYPE));
        LOG(2, "//AVS_VLD ACC (70) Error acc 0x%x\n", AVS_VLD_READ32(RO_AVS_ERROR_ACC));
        LOG(2, "MBX = %d, MBY = %d\n", MC_READ32(0x28), MC_READ32(0x2C));
        //LOG(2, "//AVS_VLD (71)  0x%x\n", AVS_VLD_READ32(RO_AVLD_DEC_CYCLE));    
        //u4CntTimeChk = DEC_RETRY_NUM;
        _bTimeOutFlag = FALSE;

#if 0
#ifdef CC_AVS_EMULATION
        Printf("Avs VLD 0x88 = 0x%x\n", AVS_VLD_READ32(0x88));
        Printf("Avs VLD 0x104 = 0x%x\n", AVS_VLD_READ32(0x104));
        Printf("Read ptr = 0x%x\n", prVdec->rPesInfo.u4VldReadPtr);
        Printf("Write ptr = 0x%x\n", prVdec->rDecParam.u4WPtr);
        
        u4Temp = MC_READ32(0x6C);
        if (u4Temp != 0)
        {
            LOG(2, "Block in MC\n");
            INT32 i4FwBlkDist1_0 = 0;
            INT32 i4FwBlkDist3_2 = 0;
            INT32 i4BwBlkDist1_0 = 0;
            INT32 i4FwMedRatio1_0 = 0;
            INT32 i4FwMedRatio3_2 = 0;
            INT32 i4BwMedRatio1_0 = 0;
            INT32 i4DirectRatio1_0 = 0;
            INT32 i4DirectRatio3_2 = 0;
            i4FwBlkDist1_0 = AVS_MV_READ32(RW_AVS_BLK_FW_DISTANCE_1_0);
            i4FwBlkDist3_2 = AVS_MV_READ32(RW_AVS_BLK_FW_DISTANCE_3_2);
            i4BwBlkDist1_0 = AVS_MV_READ32(RW_AVS_BLK_BW_DISTANCE_1_0);
            i4FwMedRatio1_0 = AVS_MV_READ32(RW_AVS_BLK_FW_MED_RATIO_1_0);
            i4FwMedRatio3_2 = AVS_MV_READ32(RW_AVS_MV_FW_MED_RATIO_3_2);
            i4BwMedRatio1_0 = AVS_MV_READ32(RW_AVS_MV_BW_MED_RATIO_1_0);
            i4DirectRatio1_0 = AVS_MV_READ32(RW_AVS_MV_DIRECT_RATIO_1_0);
            i4DirectRatio3_2 = AVS_MV_READ32(RW_AVS_MV_DIRECT_RATIO_3_2);
            Printf("RISCWrite_MV(170, 32'h%x);\n", i4FwBlkDist1_0);
            Printf("RISCWrite_MV(171, 32'h%x);\n", i4FwBlkDist3_2);
            Printf("RISCWrite_MV(172, 32'h%x);\n", i4BwBlkDist1_0);
            Printf("RISCWrite_MV(173, 32'h%x);\n", i4FwMedRatio1_0);
            Printf("RISCWrite_MV(174, 32'h%x);\n", i4FwMedRatio3_2);
            Printf("RISCWrite_MV(175, 32'h%x);\n", i4BwMedRatio1_0);
            Printf("RISCWrite_MV(176, 32'h%x);\n", i4DirectRatio1_0);
            Printf("RISCWrite_MV(177, 32'h%x);\n", i4DirectRatio3_2);
        }
#endif        
#endif
    }
    else    //wait picture finish
    {
#if 0
        while(((AVS_VLD_READ32(RO_AVLD_PIC_FINISH) & AVLD_FINISH_MASK) == 0) &&
            (u4CntTimeChk < DEC_RETRY_NUM))
        {
            u4CntTimeChk++;
        }
#endif
        #ifdef VDEC_TIME_PROFILE
        u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
        u4DecCycle = MC_READ32(0x778);
        //LOG(2, "dec cycle = %d, dram cycle=%d\n", u4DecCycle, u4DramCycle);
        #endif        
    }

    
    #ifdef VDP_RACE_LOG
    VDP_FrcRacingLog('F');
    #endif
        
    #else
    UINT32 u4MbX_last, u4MbY_last; 
    UINT32 u4Cnt = 0;

    _VDEC_AvsMCGetMbXY(&u4MbX, &u4MbY);
    while(u4CntTimeChk < DEC_RETRY_NUM) //polling retry times
    {
        u4Cnt++;
        if((u4Cnt & 0x3f)== 0x3f)    //delay
        {
            //vIrqProc();
            if(AVS_VLD_READ32(RO_AVLD_COMPLETE))
            {
                //_fgVDecComplete = TRUE;
                u1Ret = 1;
                break;
            }
            /*if(_fgIsVDecComplete(u1PicStruct, u4W, u4H))
            {
                u4CntTimeChk = 0;
                u1Ret = 1;
                break;
            }*/
            else
            {
                u4MbX_last = u4MbX;
                u4MbY_last = u4MbY;
                _VDEC_AvsMCGetMbXY(&u4MbX, &u4MbY);
                if((u4MbX == u4MbX_last) && (u4MbY == u4MbY_last))
                {
                    u4CntTimeChk++;
                }
                else
                {
                    u4CntTimeChk = 0;
                }
            }
            u4Cnt = 0;
        }
    }
    #endif

    //_vSaveChkSum();
    //_VDEC_DumpReg();

    UNUSED(u4W);
    UNUSED(u4H);
    UNUSED(u1PicStruct);
    return u1Ret;
}

// **************************************************************************
// Function : void _VDEC_AvsDecProc(void);
// Description : Init Video Decoder
// Parameter : None
// Return      : 
// **************************************************************************
void _VDEC_AvsDecProc(VDEC_AVS_INFO_T* prVdec, INT32* pi4RetValue)
{
    UINT32 u4Temp;
    UINT32 u4PicStruct;
    UINT32 u4Php = 0;
    UINT32 u4MbWidthMinusOne;
    UINT32 u4MbHeightMinusOne;
    UINT32 u4WorkingSize, u4WorkingAddr;
    UINT32 u4PicCodingType; 

    VDEC_DISP_PIC_INFO_T rDispInfo = {0};

    VDEC_AVS_PIC_T *prPicInfo = &prVdec->rPicHdr;
    VDEC_AVS_DECPRM_T *prDecPrm = &prVdec->rDecParam;

    // Error handling for re-trigger case
    if (prDecPrm->fgSecField)
    {
        UINT32 u4Rptr, u4Wptr;

        LOG(6, "prepare for decoding Sec Field\n");
        AVS_VLD_WRITE32(RW_AVS_FIND_STARTCODE, 1);
        while (1)
        {
            u4Rptr = VLD_READ32(RO_VLD_VRPTR);
            u4Wptr = VLD_READ32(RO_VLD_VWPTR);
            while (AVAILABLE_SZ(u4Rptr, u4Wptr) <= AVS_MIN_LEN)
            {
                LOG(3, "There is not enough data for start code search\n");
                *pi4RetValue = (INT32)E_VDEC_FAIL;
                break;
            }

            while(AVS_VLD_READ32(RW_AVS_FIND_STARTCODE))
            {
            }
#ifdef CC_DUMP_AVS_RISC_SETTING
            if (prDecPrm->fgDumpSetting)
            {
                Printf("RISCRead_VDEC(`AVS_VLD_BASE + 52*4);\n");
                Printf("while (data_out != 32'h0)\n");
                Printf("begin\n");
                Printf("@(posedge bclk);\n");
                Printf("RISCRead_VDEC(`AVS_VLD_BASE + 52*4);\n");
            }
#endif
            // Shift 0x000001 for read pointer
            u4Temp = _VDEC_AVSGetShtBitStream(&prDecPrm->u4BitCnt, 0);
            if (((u4Temp >> 8) == 0x000001) &&
                ((u4Temp & 0x000000FF) <= 0xAF))
            {
                break;
            }
            else if (((u4Temp >> 8) == 0x000001) &&
                (((u4Temp & 0x000000FF) == I_PICTURE_SC) ||
                ((u4Temp & 0x000000FF) == PB_PICTURE_SC)))
            {
                LOG(3, "There is no data for second field decoding\n");
                _VDEC_AvsUpdateFBList(prVdec);
                *pi4RetValue = (INT32)E_VDEC_FAIL;
                return;
            }
            AVS_VLD_WRITE32(RW_AVS_FIND_NEXT_STARTCODE, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
            if (prDecPrm->fgDumpSetting)
            {
                Printf("RISCWrite(`AVS_VLD_BASE + 53*4, 32'h1)\n");
            }
#endif
        }
#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("end\n");
    }
#endif
    }

    u4PicCodingType = prPicInfo->u4PicCodingType; 
    
    u4MbWidthMinusOne = ((prDecPrm->u4LastHSize + 15) >> 4) - 1;

    if (prPicInfo->u4PicStruct != FRM_ENCODE)
    {
        u4MbHeightMinusOne = ((prDecPrm->u4LastVSize + 15) >> 5) - 1;
    }
    else
    {
        u4MbHeightMinusOne = ((prDecPrm->u4LastVSize + 15) >> 4) - 1;
    }

    // Write Addressing Mode
    MC_WRITE32(RW_MC_ADDRSWAP, prDecPrm->u4AddrMode);
    MC_WRITE32(RW_MC_ADDRSWAP_NBM, prDecPrm->u4AddrMode);
    u4Temp = MC_READ32(0x5E4);
    u4Temp |= (0x1000);
    MC_WRITE32(0x5E4, u4Temp);
#if 0
#ifdef CC_MT5365
#ifdef CC_AVS_EMULATION
#if 1
    MC_WRITE32(RW_MC_ADDRSWAP_NBM, (prDecPrm->u4AddrMode | 0x100)); //with b2r --> NBM removed 

    if (prDecPrm->u4AddrMode == ADDR_MODE_8520) //with b2r --> NBM removed 
    {
        MC_WRITE32(0x714, (1)); 
    }
#endif    
    //MC_WRITE32(RW_MC_ADDRSWAP_NBM, prDecPrm->u4AddrMode);
#else
    MC_WRITE32(RW_MC_ADDRSWAP_NBM, prDecPrm->u4AddrMode);
#endif
#endif

    u4Temp = MC_READ32(0x5E4);
    u4Temp |= (0x1000);
    MC_WRITE32(0x5E4, u4Temp);
#endif

    // Set some VLD registers
    AVS_VLD_WRITE32(RW_AVS_VLD_CTRL, 0x8007); 
    AVS_VLD_WRITE32(RW_AVS_VLD_PIC_SIZE, ((u4MbHeightMinusOne << 8) | u4MbWidthMinusOne)); 

    // Set VLD Timeout register
    //AVS_VLD_WRITE32(RW_AVS_VLD_HW_TIMEOUT, AVS_HW_TIMEOUT);
    
    // pic type/pic struct/pic reference/skip mode flag/fix pic qp
    if (prPicInfo->u4PicStruct == FRM_ENCODE)
    {
        u4PicStruct = 3;
    }
    else 
    {
        if (prPicInfo->u4TFT)
        {
            if (!prDecPrm->fgSecField)
            {
                u4PicStruct = 1; // top field
            }
            else
            {
                u4PicStruct = 2; //bottom field
            }
        }
        else
        {
            if (!prDecPrm->fgSecField)
            {
                u4PicStruct = 2; // bottom field
            }
            else
            {
                u4PicStruct = 1; //top field

                // bottom field reconstructs from top field
                if (prPicInfo->u4PicCodingType == I_IMG)
                {
                    u4PicCodingType = P_IMG;
                }
            }
        }
    }

    u4Php = ((u4PicCodingType & 3) |
                 ((u4PicStruct & 3) << 2) |
                 ((prPicInfo->u4PicRefFg & 1) << 4) |
                 ((prPicInfo->u4SkipModeFg & 1) << 5) |
                 ((prPicInfo->u4FixedPicQP & 1) << 6) |
                 ((prPicInfo->u4PicQP & 0x3f) << 8) |
                 ((prPicInfo->i4AlphaCOffset & 0x1f) << 16) |
                 ((prPicInfo->i4BetaOffset & 0x1f) << 24));
    AVS_VLD_WRITE32(RW_AVS_VLD_PHP, u4Php); 

    AVS_VLD_WRITE32(RW_AVS_VLD_ERR_CONCEALMENT, AVS_ERR_CONCEALMENT_ENABLE);

#if 0
    LOG(6, "AVS VLD %d checksum = 0x%08x\n", RW_AVS_VLD_PIC_SIZE >> 2, AVS_VLD_READ32(RW_AVS_VLD_PIC_SIZE));
    LOG(6, "AVS VLD %d checksum = 0x%08x\n", RW_AVS_VLD_PHP >> 2, AVS_VLD_READ32(RW_AVS_VLD_PHP));
#endif

#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prVdec->rDecParam.fgDumpSetting)
    {
        Printf("RISCWrite(`AVS_VLD_BASE + 33*4, 32'h8007);\n");
        Printf("RISCWrite(`AVS_VLD_BASE + 47*4, 32'hffffffff);\n");
        Printf("RISCWrite(`AVS_VLD_BASE + 34*4, 32'h%x);\n", ((u4MbHeightMinusOne << 8) | u4MbWidthMinusOne));
        Printf("RISCWrite(`AVS_VLD_BASE + 35*4, 32'h%x);\n", u4Php);
        Printf("RISCWrite(`AVS_VLD_BASE + 49*4, 32'h1);\n");
    }
#endif

    FBM_GetWorkingBuffer(prDecPrm->ucFbgId, &u4WorkingAddr, &u4WorkingSize);    

    // Get/Set working buffer for VLD prediction module
    prDecPrm->u4VldPredBufAddr = u4WorkingAddr;
    //AVS_VLD_WRITE32(RW_AVS_VLD_PRED_BASE_ADDR, (prDecPrm->u4VldPredBufAddr | 0x40000000));
    AVS_VLD_WRITE32(RW_AVS_VLD_PRED_BASE_ADDR, 0);    

    // Set MV Related Settings
    prDecPrm->u4TopMvBufAddr = prDecPrm->u4VldPredBufAddr + AVS_VLD_PRED_SZ;
    prDecPrm->u4BottomMvBufAddr = prDecPrm->u4TopMvBufAddr + AVS_VLD_MV_SZ;
    _VDEC_AvsSetMvReg(prVdec, pi4RetValue);
    
    // Fill Quantization Table
    _VDEC_AvsFillQTable2Sram();

    // Set MC Related Settings
    _VDEC_AvsSetMcReg(prVdec, pi4RetValue);

    // Set PP Related Settings
    _VDEC_AvsSetPpReg(prVdec, pi4RetValue);

    // Mid protection for avs
    rDispInfo.pvYAddr = (void*)((UINT32)prDecPrm->pucDecWorkBufY);    
    rDispInfo.pvCAddr = (void*)((UINT32)prDecPrm->pucDecWorkBufC);
    rDispInfo.u4YSize = (UINT32)prDecPrm->pucDecWorkBufC - (UINT32)prDecPrm->pucDecWorkBufY;
    if(!_VDEC_MidSetting(ES0, &rDispInfo))
    {
        LOG(3, "VdecMid Fail~\n");
    }
    
    HalFlushInvalidateDCache();
    
    //profile decoding time
    HAL_GetTime(&_rTimeHwS);
#ifdef VDEC_TIME_PROFILE

    u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
    if(prDecPrm->u1DramMoniter == 1)  //total bandwidth
    {
        u4DramCycle = SRM_DramMonitor(SRM_DRV_TOTAL, u4DelayClock, 0);
    }
    else if(prDecPrm->u1DramMoniter == 2)  //mpeg
    {
        #if defined(IS_POST_MT5363)
        #ifdef VDEC_TIME_PROFILE
        u4DramCycle = SRM_DramMonitor(SRM_DRV_VDEC_MC, u4DelayClock, 0);
        #endif
        #endif
    }
#endif

    // Trigger Decoder
    AVS_VLD_WRITE32(RW_AVS_VLD_DECSTART, 1);
    
    *pi4RetValue = 0;
}


//old output display and buffer clean at the same time
void _VDEC_OutputAvsDispPic(VDEC_AVS_DECPRM_T* prDecPrm, INT32* pi4RetValue)
{
    *pi4RetValue = E_VDEC_OK;
}

// *********************************************************************
// Function    : void vClrPicRefInfo(BYTE bPicType, BYTE bFBufIdx)
// Description : Clear picture info in frame buffer
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vClrAvsPicRefInfo(VDEC_AVS_DECPRM_T* prDecPrm, UINT8 u1PicType, UINT8 u1FBufIdx)
{
    if (!prDecPrm->u4MaxFBufNum)
    {
        LOG(5, "%s Max frame buf num = 0\n", __FUNCTION__);
        return;
    }

    ASSERT(prDecPrm!=NULL);
    ASSERT(u1FBufIdx < prDecPrm->u4MaxFBufNum);

    if(u1FBufIdx >= prDecPrm->u4MaxFBufNum)
    {
        return;
    }
    if(u1PicType & TOP_FIELD)
    {
        prDecPrm->arFBufInfo[u1FBufIdx].u1TFldRefType = NREF_PIC;
    }
    if(u1PicType & BOTTOM_FIELD)
    {
        prDecPrm->arFBufInfo[u1FBufIdx].u1BFldRefType = NREF_PIC;
    }
    prDecPrm->arFBufInfo[u1FBufIdx].u1FBufRefType = NREF_PIC;
}

void _VDEC_ClrAvsFBufInfo(VDEC_AVS_DECPRM_T *prDecPrm, UINT32 u4FBufIdx, BOOL fgInit)
{
    VDEC_AVS_FBUF_INFO_T* prFBufInfo;
    
    if (!fgInit)
    {
        ASSERT(prDecPrm!=NULL);
        ASSERT(u4FBufIdx < prDecPrm->u4MaxFBufNum);

        if(u4FBufIdx >= prDecPrm->u4MaxFBufNum)
        {
            return;
        }
    }
    
    prFBufInfo = &prDecPrm->arFBufInfo[u4FBufIdx];
        
    prFBufInfo->fgNonExisting = FALSE;    
    prFBufInfo->fgIsBufRef = FALSE;
    prFBufInfo->fgIsErr = FALSE;
    prFBufInfo->fgIsNotDisp = FALSE;
    
    prFBufInfo->u1FBufStatus = NO_PIC;    

    prFBufInfo->ucFbId = FBM_FB_ID_UNKNOWN;
    
    _vClrAvsPicRefInfo(prDecPrm, FRAME, (UINT8)u4FBufIdx);
  
    prFBufInfo->u4FrameNum = (UINT32)0xffffffff;
    prFBufInfo->i4FrameNumWrap = (INT32)0xefffffff;

    LOG(6, "_VDEC_ClrAvsFBufInfo fb id =%d\n", prFBufInfo->ucFbId);
}

void _VDEC_GetAvsRptr(UINT32* pu4Rptr)
{
    ASSERT(pu4Rptr!=NULL);
    *pu4Rptr = _VDEC_AvsVldRPtr();
}

void _VDEC_SetAvsWptr(UINT32 u4Wptr)
{
    _VDEC_SetAvsVldWPtr(u4Wptr);
}

BOOL _VDEC_AvsSkipFrm(const VDEC_AVS_INFO_T* prDecPrm)
{
    DRV_DBASE_T* _prDbase = NULL;
    CRIT_STATE_T _rCritState;
    PARAM_MPV_T* prInfo;

    ASSERT(prDecPrm != NULL);

    _prDbase = DBS_Lock(&_rCritState);
    ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));        
    prInfo = &(_prDbase->rMpv[0]);
        
    prInfo->u4EsmSkipNs++;
    #ifdef FBM_DPB_CTRL
    VDEC_INFO_T *prVdecInfo = _VDEC_GetInfo();
    if(((prDecPrm->prCurrFBufInfo == NULL) ||
        (prDecPrm->prSliceHdr == NULL)) ||
        (prVdecInfo && (prVdecInfo->eCurState == VDEC_ST_PLAY_I_FRAME)))
    {
        return FALSE;
    }

    if((prDecPrm->prCurrFBufInfo->u1FBufStatus == TOP_FIELD) ||
        (prDecPrm->prCurrFBufInfo->u1FBufStatus == BOTTOM_FIELD))
    {
        if(fgIsNonRefFBuf(prDecPrm->prCurrFBufInfo->u1FBufRefType, 
        prDecPrm->prCurrFBufInfo->u1TFldRefType, 
        prDecPrm->prCurrFBufInfo->u1BFldRefType))
        {
            LOG(3, "Put Non-Ref Frm to Empty in SkipFrm\n");
            prDecPrm->prCurrFBufInfo->fgIsNotDisp = TRUE;
            prDecPrm->prCurrFBufInfo->u1FBufStatus = FRAME;
            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
                prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_READY);
            LOG(3, "Skip B\n");
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else if(prDecPrm->prSliceHdr->fgFieldPicFlag)   //field encoded
    {
        return FALSE;
    }
    else
    {
        prDecPrm->prCurrFBufInfo->u4SkipFrmCnt++;
        LOG(3, "Skip B\n");
        return TRUE;
    }
        
    #endif

    return FALSE;
}

void _VDEC_AvsFrmFin(VDEC_AVS_DECPRM_T* prDecPrm)
{
    //UINT32 u4ErrAcc = 0, u4PrevAddr = 0, u4PList0 = 0;
    
    ASSERT(prDecPrm!=NULL);
    
    //save decode time
    prDecPrm->u4CurDecMs += (_rTimeHwDt.u4Micros / 1000);
    LOG(6, "dec time = %d\n", _rTimeHwDt.u4Micros);
    #ifdef VDEC_TIME_PROFILE
    prDecPrm->u4CurDecCycle += u4DecCycle;
    prDecPrm->u4CurDramCycle += u4DramCycle;
    #endif

}


void _VDEC_FlushAvsDPB(VDEC_AVS_DECPRM_T *prDecPrm)
{
    UINT32 i;
    FBM_PIC_HDR_T * prFbmPicHdr;

    LOG(5, "Flush Avs DPB\n");
    for(i = 0; i < prDecPrm->u4MaxFBufNum; i++)
    {
        if (prDecPrm->arFBufInfo[i].u1FBufStatus != NO_PIC)
        {
            if (prDecPrm->arFBufInfo[i].u1FBufRefType == REF_PIC)
            {
                LOG(5, "%dth frame is Ref Pic\n", i);
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId);
                if(prFbmPicHdr == NULL)
                {
                    LOG(2, "prFbmPicHdr NULL in FlushDpb\n");
                }
                else
                {
                    if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId) == FBM_FB_STATUS_READY)
                    {
                        LOG(5, "%dth frame is Ref Pic and it's in ready state\n", i);
                        FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId, FALSE);
                        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arFBufInfo[i].ucFbId, 
                                                                    FBM_FB_STATUS_EMPTY);
                    }
                }
            }
        }
        _VDEC_ClrAvsFBufInfo(prDecPrm, i, TRUE);
    }

    for (i = 0; i < MAX_REF_PICNUM; i++)
    {
        prDecPrm->arRefFBufInfo[i].ucFbId = FBM_FB_ID_UNKNOWN;
        prDecPrm->arRefFBufInfo[i].fgValid = TRUE;
        prDecPrm->arRefFBufInfo[i].u1DecFBufIdx = VDEC_AVS_UNKNOWN_IDX;
    }
}


static void _VDEC_AvsFillQTable2Sram(void)
{
    // Because I don't want to waste memory, I use hard codes for Q table 
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 0);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xdd38ff);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 4);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x5678d08);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 8);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xa49638ff);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 12);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x6167451d);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 16);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff963851);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 20);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x5b9f3c1);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 24);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x299638d6);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 28);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x2e904598);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 32);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff9638ff);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 36);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xf05345ac);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 40);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xffa038ff);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 44);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x5673bb7);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 48);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff963d04);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 52);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x56745c1);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 56);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x3963501);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 60);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x56745c3);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 64);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x808c98a5);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 68);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb5c5d8ec);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 72);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x808b98a5);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 76);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb4c5d7ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 80);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff8b98a6);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 84);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb5c5d6ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 88);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x808b98a5);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 92);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb5c5d7ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 96);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff8b98a5);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 100);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb4c5d7ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 104);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff8b98a5);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 108);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb5c5d7ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 112);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xff8b98a6);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 116);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb5c5d7ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 120);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x808b98a6);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 124);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb5c5d7ea);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 128);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xe0e0e0e);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 132);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xe0e0e0e);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 136);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xd0d0d0d);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 140);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xd0d0d0d);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 144);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xd0c0c0c);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 148);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xc0c0c0c);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 152);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb0b0b0b);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 156);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb0b0b0b);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 160);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xb0a0a0a);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 164);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xa0a0a0a);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 168);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0xa090909);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 172);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x9090909);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 176);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x9080808);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 180);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x8080808);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 184);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x7070707);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE_POS, 188);
    VLD_WRITE32(RW_AVS_VLD_QUANT_TABLE, 0x7070707);
}


static void _VDEC_AvsSetMcReg(const VDEC_AVS_INFO_T* prVdec, INT32 *pi4Ret)
{
    UINT32 u4WidthMb;
    UINT32 u4FbWidth, u4FbHeight;

    VDEC_AVS_PIC_T* prPicHdr;
    VDEC_AVS_DECPRM_T* prDecPrm;

    prPicHdr = (VDEC_AVS_PIC_T*)&prVdec->rPicHdr;
    prDecPrm = (VDEC_AVS_DECPRM_T*)&prVdec->rDecParam;

    if (prDecPrm->u4AddrMode == ADDR_MODE_5351_NORMAL)
    {
        u4WidthMb = (UINT32)((prDecPrm->u4LineSize + 15) >> 4);
        u4WidthMb = ((u4WidthMb + 3) / 4) * 4;
        VLD_WRITE32(RW_VLD_MCPICSZ, u4WidthMb);
        LOG(6, "VLD_WRITE32(%d, %d)\n", RW_VLD_MCPICSZ >> 2, u4WidthMb);
    }
    else
    {
        u4WidthMb = (UINT32)((prDecPrm->u4LastHSize + 15) >> 4);
        VLD_WRITE32(RW_VLD_MCPICSZ, u4WidthMb);
    }

    // Set frame width and height
    MC_WRITE32(RW_MC_UMV_PIC_WIDTH, prDecPrm->u4LastHSize);
    MC_WRITE32(RW_MC_UMV_PIC_HEIGHT, prDecPrm->u4LastVSize);
    LOG(6, "MC_WRITE32(%d, %d)\n", RW_MC_UMV_PIC_WIDTH >> 2, prDecPrm->u4LastHSize);
    LOG(6, "MC_WRITE32(%d, %d)\n", RW_MC_UMV_PIC_WIDTH >> 2, prDecPrm->u4LastVSize);

    // For debug usage
    MC_WRITE32(0x788, 3);
    MC_WRITE32(0x790, 3);

    // Enable PP
    MC_WRITE32(RW_AVS_MC_PP_ENABLE, prDecPrm->fgEnPP);

    FBM_GetFrameBufferSize(prDecPrm->ucFbgId, &u4FbWidth, &u4FbHeight);

    // Set the size of luma
    MC_WRITE32(RW_AVS_MC_LUMA_SIZE, (u4FbWidth * (u4FbHeight)));
    LOG(6, "MC_WRITE32(%d, %d)\n", RW_AVS_MC_LUMA_SIZE >> 2, (u4FbWidth * (u4FbHeight)));

    // Disable MC Write
    MC_WRITE32(RW_AVS_MC_DISABLE_WRITE, 1);

    // Set Reference Buffer
    if (prPicHdr->u4PicStruct == FRM_ENCODE)
    {
        if (prPicHdr->u4PicCodingType == I_IMG)
        {
            LOG(6, "I frame RW_AVS_MC_P_LIST0_PIC0_Y_ADDR fb id = 0x%08x\n", 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].ucFbId);
            MC_WRITE32(RW_AVS_MC_P_LIST0_PIC0_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
        }
        else if (prPicHdr->u4PicCodingType == P_IMG)
        {
            LOG(6, "P frame RW_AVS_MC_P_LIST0_PIC0_Y_ADDR fb id = 0x%08x\n", 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].ucFbId);
            MC_WRITE32(RW_AVS_MC_P_LIST0_PIC0_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
            LOG(6, "P frame RW_AVS_MC_P_LIST0_PIC1_Y_ADDR fb id = 0x%08x\n", 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].ucFbId);
            MC_WRITE32(RW_AVS_MC_P_LIST0_PIC1_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
        }
        else if (prPicHdr->u4PicCodingType == B_IMG)
        {
            LOG(6, "B frame RW_AVS_MC_B_LIST0_PIC0_Y_ADDR fb id = 0x%08x\n", 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].ucFbId);
            MC_WRITE32(RW_AVS_MC_B_LIST0_PIC0_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
            LOG(6, "B frame RW_AVS_MC_B_LIST1_PIC0_Y_ADDR fb id = 0x%08x\n", 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].ucFbId);
            MC_WRITE32(RW_AVS_MC_B_LIST1_PIC0_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
        }
    }
    else
    {
        if ((prPicHdr->u4PicCodingType == I_IMG) &&
            (prDecPrm->fgSecField))
        {
            MC_WRITE32(RW_AVS_MC_P_LIST0_PIC0_Y_ADDR, (UINT32)prDecPrm->pucDecWorkBufY);
            MC_WRITE32(RW_AVS_MC_P_LIST0_PIC1_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
            MC_WRITE32(RW_AVS_MC_P_LIST0_PIC2_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);

            if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
            {
                MC_WRITE32(RW_AVS_MC_P_LIST0_PIC3_Y_ADDR, 
                    prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
            }
            // bottom Field
            MC_WRITE32(RW_AVS_MC_P_FIELD_DESCRIPTION, 0xa);
#ifdef CC_DUMP_AVS_RISC_SETTING
            if (prDecPrm->fgDumpSetting)
            {
                Printf("RISCWrite_MC(%d, 10);\n", RW_AVS_MC_P_FIELD_DESCRIPTION>>2);
            }
#endif            
        }
        else if (prPicHdr->u4PicCodingType == P_IMG)
        {
            if (prPicHdr->u4TFT)
            {
                if (!prDecPrm->fgSecField)
                {
                    MC_WRITE32(RW_AVS_MC_P_LIST0_PIC0_Y_ADDR, 
                        prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
                    MC_WRITE32(RW_AVS_MC_P_LIST0_PIC1_Y_ADDR, 
                        prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);

                    if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
                    {
                        MC_WRITE32(RW_AVS_MC_P_LIST0_PIC2_Y_ADDR, 
                            prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
                        MC_WRITE32(RW_AVS_MC_P_LIST0_PIC3_Y_ADDR, 
                            prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
                    }
                    // Top Field
                    MC_WRITE32(RW_AVS_MC_P_FIELD_DESCRIPTION, 0x5);
                }
                else
                {
                    MC_WRITE32(RW_AVS_MC_P_LIST0_PIC0_Y_ADDR, (UINT32)prDecPrm->pucDecWorkBufY);
                    MC_WRITE32(RW_AVS_MC_P_LIST0_PIC1_Y_ADDR, 
                        prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
                    MC_WRITE32(RW_AVS_MC_P_LIST0_PIC2_Y_ADDR, 
                        prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);

                    if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
                    {
                        MC_WRITE32(RW_AVS_MC_P_LIST0_PIC3_Y_ADDR, 
                            prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
                    }
                    // bottom Field
                    MC_WRITE32(RW_AVS_MC_P_FIELD_DESCRIPTION, 0xa);
#ifdef CC_DUMP_AVS_RISC_SETTING
                    if (prDecPrm->fgDumpSetting)
                    {
                        Printf("RISCWrite_MC(%d, 10);\n", RW_AVS_MC_P_FIELD_DESCRIPTION>>2);
                    }
#endif            
                }
            }
            else
            {
            }
        }
        else if (prPicHdr->u4PicCodingType == B_IMG)
        {
            MC_WRITE32(RW_AVS_MC_B_LIST0_PIC0_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);
            MC_WRITE32(RW_AVS_MC_B_LIST0_PIC1_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx].u4YAddr);

            MC_WRITE32(RW_AVS_MC_B_LIST1_PIC0_Y_ADDR, 
                prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);
            MC_WRITE32(RW_AVS_MC_B_LIST1_PIC1_Y_ADDR, 
                    prDecPrm->arFBufInfo[prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx].u4YAddr);

            MC_WRITE32(0x568, 1);
            MC_WRITE32(0x56C, 2);
#ifdef CC_DUMP_AVS_RISC_SETTING
            if (prDecPrm->fgDumpSetting)
            {
                Printf("RISCWrite_MC(%d, 1);\n", 0x568>>2);
                Printf("RISCWrite_MC(%d, 2);\n", 0x56C>>2);
            }
#endif            
        }
    }

    if (prPicHdr->u4PicCodingType == B_IMG)
    {
        MC_WRITE32(RW_AVS_MC_B_REF, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCWrite_MC(%d, 1);\n", RW_AVS_MC_B_REF>>2);
        }
#endif
    }
#if 0    
    // Set Decoding Buffer
    MC_WRITE32(RW_MC_DEC_Y, prDecPrm->pucDecWorkBufY);
    MC_WRITE32(RW_MC_DEC_C, prDecPrm->pucDecWorkBufC);    
#endif

#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("RISCWrite_VLD(%d, %d);\n", RW_VLD_MCPICSZ>>2, u4WidthMb);
        Printf("RISCWrite_MC(%d, %d);\n", RW_MC_UMV_PIC_WIDTH>>2, prDecPrm->u4LastHSize);
        Printf("RISCWrite_MC(%d, %d);\n", RW_MC_UMV_PIC_HEIGHT>>2, prDecPrm->u4LastVSize);
        Printf("RISCWrite_MC(%d, %d);\n", RW_AVS_MC_LUMA_SIZE>>2, (prDecPrm->u4LastHSize * prDecPrm->u4LastVSize));
        Printf("RISCWrite_MC(%d, 1);\n", RW_AVS_MC_DISABLE_WRITE>>2);
        Printf("RISCWrite_MC(%d, %d);\n", RW_AVS_MC_PP_ENABLE>>2, prDecPrm->fgEnPP);
    }
#endif

}


static void _VDEC_AvsSetMvReg(const VDEC_AVS_INFO_T* prVdec, INT32 *pi4Ret)
{
    UINT8 u1BufIdx = 0;

    INT32 ai4BlkDistFw[4];
    INT32 ai4BlkDistBw[2];
    INT32 ai4DirectRatio[4];
    INT32 ai4MedRatioFw[4];
    INT32 ai4MedRatioBw[4];

    INT32 i4FwBlkDist1_0 = 0;
    INT32 i4FwBlkDist3_2 = 0;
    INT32 i4BwBlkDist1_0 = 0;
    INT32 i4FwMedRatio1_0 = 0;
    INT32 i4FwMedRatio3_2 = 0;
    INT32 i4BwMedRatio1_0 = 0;
    INT32 i4DirectRatio1_0 = 0;
    INT32 i4DirectRatio3_2 = 0;

    INT32 i = 0;
    
    VDEC_AVS_PIC_T* prPicHdr;
    VDEC_AVS_DECPRM_T* prDecPrm;

    prPicHdr = (VDEC_AVS_PIC_T*)&prVdec->rPicHdr;
    prDecPrm = (VDEC_AVS_DECPRM_T*)&prVdec->rDecParam;

    if ((!prDecPrm->u4TopMvBufAddr) ||
        (!prDecPrm->u4BottomMvBufAddr))
    {
        *pi4Ret = E_VDEC_FAIL;
        LOG(3, "_VDEC_AvsSetMvReg NULL motion vector address\n");
        return;
    }

    if (prPicHdr->u4PicStruct == FLD_ENCODE)
    {
        if (prPicHdr->u4TFT)
        {
            if (!prDecPrm->fgSecField)
            {
                AVS_MV_WRITE32(RW_AVS_MV_ADDR, prDecPrm->u4TopMvBufAddr >> 4);
#ifdef CC_DUMP_AVS_RISC_SETTING
                if (prDecPrm->fgDumpSetting)
                {
                    Printf("RISCWrite_MV(131, 32'h%x);\n", (prDecPrm->u4TopMvBufAddr >> 4));
                }
#endif
            }
            else
            {
                AVS_MV_WRITE32(RW_AVS_MV_ADDR, prDecPrm->u4BottomMvBufAddr >> 4);
#ifdef CC_DUMP_AVS_RISC_SETTING
                if (prDecPrm->fgDumpSetting)
                {
                    Printf("RISCWrite_MV(131, 32'h%x);\n", (prDecPrm->u4BottomMvBufAddr >> 4));
                }
#endif
            }
        }
        else
        {
            if (!prDecPrm->fgSecField)
            {
                AVS_MV_WRITE32(RW_AVS_MV_ADDR, prDecPrm->u4BottomMvBufAddr >> 4);
#ifdef CC_DUMP_AVS_RISC_SETTING
                if (prDecPrm->fgDumpSetting)
                {
                    Printf("RISCWrite_MV(131, 32'h%x);\n", (prDecPrm->u4BottomMvBufAddr >> 4));
                }
#endif
            }
            else
            {
                AVS_MV_WRITE32(RW_AVS_MV_ADDR, prDecPrm->u4TopMvBufAddr >> 4);
#ifdef CC_DUMP_AVS_RISC_SETTING
                if (prDecPrm->fgDumpSetting)
                {
                    Printf("RISCWrite_MV(131, 32'h%x);\n", (prDecPrm->u4TopMvBufAddr >> 4));
                }
#endif
            }
        }
    }
    else
    {
        AVS_MV_WRITE32(RW_AVS_MV_ADDR, prDecPrm->u4TopMvBufAddr >> 4);
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCWrite_MV(131, 32'h%x);\n", (prDecPrm->u4TopMvBufAddr >> 4));
        }
#endif
    }

    x_memset(ai4BlkDistFw, 0, sizeof(INT32) * 4);
    x_memset(ai4BlkDistBw, 0, sizeof(INT32) * 2);
    x_memset(ai4DirectRatio, 0, sizeof(INT32) * 4);
    x_memset(ai4MedRatioFw, 0, sizeof(INT32) * 4);
    x_memset(ai4MedRatioBw, 0, sizeof(INT32) * 4);
    
    // If the decoding source is the bottom field or the second field, 
    // the index distance should be picture distance * 2 + 1.
    // Otherwise, the index distance should be picture distance * 2
    if (prPicHdr->u4PicCodingType == I_IMG)
    {
        if ((prPicHdr->u4PicStruct == FRM_ENCODE) ||
            ((prPicHdr->u4PicStruct == FLD_ENCODE) && (!prDecPrm->fgSecField)))
        {
            LOG(6, "I frame/I top field\n");
        }
        else
        {
            u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;

            ai4BlkDistFw[0] = 1 ;
            ai4BlkDistFw[1] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
            ai4BlkDistFw[2] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2) + 1;
            if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
            {
                u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
                ai4BlkDistFw[3] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
            }
            LOG(6, "I bottom field\n");
        }
    }
    else if(prPicHdr->u4PicCodingType == P_IMG)
    {
        if (prPicHdr->u4PicStruct == FRM_ENCODE)
        {
            u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
            
            ai4BlkDistFw[0] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
            if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
            {
                u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;

                ai4BlkDistFw[1] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
            }
            LOG(6, "P Frame\n");
        }
        else
        {
            if (prPicHdr->u4TFT)
            {
                if (!prDecPrm->fgSecField)
                {
                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                    
                    ai4BlkDistFw[0] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2) - 1;
                    ai4BlkDistFw[1] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);

                    if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
                    {
                        u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;

                        ai4BlkDistFw[2] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2) - 1;
                        ai4BlkDistFw[3] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
                    }
                    LOG(6, "P top field\n");
                }
                else
                {
                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                    
                    ai4BlkDistFw[0] = 1 ;
                    ai4BlkDistFw[1] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
                    ai4BlkDistFw[2] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2) + 1;
                    if (prDecPrm->u1LastRefIdx >= REF_LIST_1)
                    {
                        u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
                        ai4BlkDistFw[3] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
                    }
                    LOG(6, "P bottom field\n");
                }
            }
            else
            {
            }
        }
    }
    else
    {
        UINT8 u1BufIdx2;
        
        if (prPicHdr->u4PicStruct == FRM_ENCODE)
        {
            ASSERT(prDecPrm->u1LastRefIdx >= 1);
            u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
            ai4BlkDistFw[0] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
            u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
            ai4BlkDistBw[0] = (INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2- (INT32)(prPicHdr->u4PicDistance) * 2);

            u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
            u1BufIdx2 = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
            ai4DirectRatio[0] =	(INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2- 
                (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2);

            if (prDecPrm->u1LastRefIdx > REF_LIST_1)
            {
                u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                u1BufIdx2 = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-2].u1DecFBufIdx;
                ai4DirectRatio[1] = (INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2- 
                    (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2);
            }

            u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
            if (prDecPrm->arFBufInfo[u1BufIdx].u1PicCodingType == I_IMG)
            {
                AVS_MV_WRITE32(RW_AVS_MV_BW_I_REF, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
                if (prDecPrm->fgDumpSetting)
                {
                    Printf("RISCWrite_MV(134, 32'h1);\n");
                }
#endif
            }
            else
            {
                AVS_MV_WRITE32(RW_AVS_MV_BW_I_REF, 0);
#ifdef CC_DUMP_AVS_RISC_SETTING
                if (prDecPrm->fgDumpSetting)
                {
                    Printf("RISCWrite_MV(134, 32'h0);\n");
                }
#endif
            }
            LOG(6, "B Frame\n");
        }
        else
        {
            if (prPicHdr->u4TFT)
            {
                // Top field
                if (!prDecPrm->fgSecField)
                { 
                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;

                    ai4BlkDistFw[0] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2) - 1;
                    ai4BlkDistFw[1] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);

                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                    ai4BlkDistBw[0] = (INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prPicHdr->u4PicDistance) * 2);
                    ai4BlkDistBw[1] = (INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prPicHdr->u4PicDistance) * 2) + 1;

                    u1BufIdx2 = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
                    ai4DirectRatio[0] =	(INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2) - 1;
                    ai4DirectRatio[1] =	(INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2);

                    if (prDecPrm->u1LastRefIdx > REF_LIST_1)
                    {
                        u1BufIdx2 = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-2].u1DecFBufIdx;
                        ai4DirectRatio[2] =	(INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance * 2) - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2) - 1;
                        ai4DirectRatio[3] =	(INT32)((INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance * 2) - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2);
                    }

                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                    if (prDecPrm->arFBufInfo[u1BufIdx].u1PicCodingType == I_IMG)
                    {
                        AVS_MV_WRITE32(RW_AVS_MV_BW_I_REF, 1);
#ifdef CC_DUMP_AVS_RISC_SETTING
                        if (prDecPrm->fgDumpSetting)
                        {
                            Printf("RISCWrite_MV(134, 32'h1);\n");
                        }
#endif
                    }
                    else
                    {
                        AVS_MV_WRITE32(RW_AVS_MV_BW_I_REF, 0);
#ifdef CC_DUMP_AVS_RISC_SETTING
                        if (prDecPrm->fgDumpSetting)
                        {
                            Printf("RISCWrite_MV(134, 32'h0);\n");
                        }
#endif
                    }

                    LOG(6, "B Top Field\n");
                }
                else
                {
                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
                    ai4BlkDistFw[0] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2);
                    ai4BlkDistFw[1] = (INT32)((INT32)(prPicHdr->u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2) + 1;

                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                    ai4BlkDistBw[0] = (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prPicHdr->u4PicDistance) * 2 -1;
                    ai4BlkDistBw[1] = (INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prPicHdr->u4PicDistance) * 2;

                    ai4DirectRatio[0] =	1;

                    u1BufIdx = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx].u1DecFBufIdx;
                    u1BufIdx2 = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-1].u1DecFBufIdx;
                    ai4DirectRatio[1] =	(INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2);
                    ai4DirectRatio[2] =	(INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2) + 1;

                    if (prDecPrm->u1LastRefIdx > REF_LIST_1)
                    {
                        u1BufIdx2 = prDecPrm->arRefFBufInfo[prDecPrm->u1LastRefIdx-2].u1DecFBufIdx;
                        ai4DirectRatio[3] =	(INT32)(prDecPrm->arFBufInfo[u1BufIdx].u4PicDistance) * 2 - (INT32)(prDecPrm->arFBufInfo[u1BufIdx2].u4PicDistance) * 2;
                    }

                    AVS_MV_WRITE32(RW_AVS_MV_BW_I_REF, 0);
#ifdef CC_DUMP_AVS_RISC_SETTING
                    if (prDecPrm->fgDumpSetting)
                    {
                        Printf("RISCWrite_MV(134, 32'h0);\n");
                    }
#endif
                    
                    LOG(6, "B bottom Field\n");
                }					
            }
            else
            {
            }
        }
    }        

    for(i = 0; i < 4; i++)
    {
        ai4BlkDistFw[i] = (ai4BlkDistFw[i] + 512)%512;
        ai4DirectRatio[i] = (ai4DirectRatio[i] + 512)%512;
        
        if(ai4BlkDistFw[i] > 0)
        {
            ai4MedRatioFw[i] = 512/ai4BlkDistFw[i];
        }

        if(ai4DirectRatio[i] > 0)
        {
            ai4DirectRatio[i] = 16384/ai4DirectRatio[i];
        }
    }
    
    for(i = 0; i < 2; i++)
    {
        ai4BlkDistBw[i] = (ai4BlkDistBw[i] + 512)%512;
        
        if(ai4BlkDistBw[i] > 0)
        {
            ai4MedRatioBw[i] = 512 /ai4BlkDistBw[i];
        }
    }

    i4FwBlkDist1_0 = ai4BlkDistFw[0] | (ai4BlkDistFw[1] << 16);
    i4FwBlkDist3_2 = ai4BlkDistFw[2] | (ai4BlkDistFw[3] << 16);
    i4BwBlkDist1_0 = ai4BlkDistBw[0] | (ai4BlkDistBw[1] << 16);
    i4FwMedRatio1_0 = ai4MedRatioFw[0] | (ai4MedRatioFw[1] << 16);
    i4FwMedRatio3_2 = ai4MedRatioFw[2] | (ai4MedRatioFw[3] << 16);
    i4BwMedRatio1_0 = ai4MedRatioBw[0] | (ai4MedRatioBw[1] << 16);
    i4DirectRatio1_0 = ai4DirectRatio[0] | (ai4DirectRatio[1] << 16);
    i4DirectRatio3_2 = ai4DirectRatio[2] | (ai4DirectRatio[3] << 16);
   
    LOG(6, "RW_AVS_MV_BW_I_REF = 0x%08x\n", AVS_MV_READ32(RW_AVS_MV_BW_I_REF));
    AVS_MV_WRITE32(RW_AVS_BLK_FW_DISTANCE_1_0, i4FwBlkDist1_0);
    LOG(6, "RW_AVS_BLK_FW_DISTANCE_1_0 = 0x%08x\n", i4FwBlkDist1_0);
    AVS_MV_WRITE32(RW_AVS_BLK_FW_DISTANCE_3_2, i4FwBlkDist3_2);
    LOG(6, "RW_AVS_BLK_FW_DISTANCE_3_2 = 0x%08x\n", i4FwBlkDist3_2);
    AVS_MV_WRITE32(RW_AVS_BLK_BW_DISTANCE_1_0, i4BwBlkDist1_0);
    LOG(6, "RW_AVS_BLK_BW_DISTANCE_1_0 = 0x%08x\n", i4BwBlkDist1_0);
    AVS_MV_WRITE32(RW_AVS_BLK_FW_MED_RATIO_1_0, i4FwMedRatio1_0);
    LOG(6, "RW_AVS_BLK_FW_MED_RATIO_1_0 = 0x%08x\n", i4FwMedRatio1_0);
    AVS_MV_WRITE32(RW_AVS_MV_FW_MED_RATIO_3_2, i4FwMedRatio3_2);
    LOG(6, "RW_AVS_MV_FW_MED_RATIO_3_2 = 0x%08x\n", i4FwMedRatio3_2);
    AVS_MV_WRITE32(RW_AVS_MV_BW_MED_RATIO_1_0, i4BwMedRatio1_0);
    LOG(6, "RW_AVS_MV_BW_MED_RATIO_1_0 = 0x%08x\n", i4BwMedRatio1_0);
    AVS_MV_WRITE32(RW_AVS_MV_DIRECT_RATIO_1_0, i4DirectRatio1_0);
    LOG(6, "RW_AVS_MV_DIRECT_RATIO_1_0 = 0x%08x\n", i4DirectRatio1_0);
    AVS_MV_WRITE32(RW_AVS_MV_DIRECT_RATIO_3_2, i4DirectRatio3_2);
    LOG(6, "RW_AVS_MV_DIRECT_RATIO_3_2 = 0x%08x\n", i4DirectRatio3_2);

#if 0
    _VDEC_AVS_IO_Write32(0x3000, 0x3C, prDecPrm->u4TopMvBufAddr);
    _VDEC_AVS_IO_Write32(0x3000, 0x40, prDecPrm->u4TopMvBufAddr + AVS_VLD_MV_SZ * 2);
#endif

#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("RISCWrite_MV(170, 32'h%x);\n", i4FwBlkDist1_0);
        Printf("RISCWrite_MV(171, 32'h%x);\n", i4FwBlkDist3_2);
        Printf("RISCWrite_MV(172, 32'h%x);\n", i4BwBlkDist1_0);
        Printf("RISCWrite_MV(173, 32'h%x);\n", i4FwMedRatio1_0);
        Printf("RISCWrite_MV(174, 32'h%x);\n", i4FwMedRatio3_2);
        Printf("RISCWrite_MV(175, 32'h%x);\n", i4BwMedRatio1_0);
        Printf("RISCWrite_MV(176, 32'h%x);\n", i4DirectRatio1_0);
        Printf("RISCWrite_MV(177, 32'h%x);\n", i4DirectRatio3_2);
    }
#endif

}

static void _VDEC_AvsSetPpReg(const VDEC_AVS_INFO_T* prVdec, INT32 *pi4Ret)
{
    UINT32 u4Height;
    UINT32 u4WidthMB, u4HeightMB;
    
    VDEC_AVS_PIC_T* prPicHdr;
    VDEC_AVS_DECPRM_T* prDecPrm;

    prPicHdr = (VDEC_AVS_PIC_T*)&prVdec->rPicHdr;
    prDecPrm = (VDEC_AVS_DECPRM_T*)&prVdec->rDecParam;

    // Set AVS mode for PP
    PP_WRITE32(RW_AVS_PP_AVS_MODE, 0x100);

    // Set PP buffer
    MC_WRITE32(RW_AVS_PP_LUMA_ADDR, (UINT32)(prDecPrm->pucDecWorkBufY) >> 9);
    MC_WRITE32(RW_AVS_PP_CHROM_ADDR, (UINT32)(prDecPrm->pucDecWorkBufC) >> 8);    

#if 0
    _VDEC_AVS_IO_Write32(0x3000, 0x44, (UINT32)(prDecPrm->pucDecWorkBufY) >> 5);
    _VDEC_AVS_IO_Write32(0x3000, 0x48, (UINT32)(prDecPrm->pucDecWorkBufC) >> 4);
    _VDEC_AVS_IO_Write32(0x3000, 0x28, 3);
#endif

    MC_WRITE32(0x250, 1);
    // PP Write Enable.
    MC_WRITE32(0x220, 1);

    if (!prPicHdr->u4LoopFilterDisable)
    {
        MC_WRITE32(0x238, 3);
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCWrite_MC(%d, 3);\n", 0x238>>2);
        }
#endif
    }
    else
    {
        MC_WRITE32(0x238, 0);
#ifdef CC_DUMP_AVS_RISC_SETTING
        if (prDecPrm->fgDumpSetting)
        {
            Printf("RISCWrite_MC(%d, 0);\n", 0x238>>2);
        }
#endif
    }

    if (!prPicHdr->u4ProgFrm)
    {
        u4Height = prDecPrm->u4LastVSize/2;
    }
    else
    {
        u4Height = prDecPrm->u4LastVSize;
    }

    u4WidthMB = ((prDecPrm->u4LastHSize + 15)/16) - 1;
    u4HeightMB = ((u4Height + 15)/16) - 1;
        
    MC_WRITE32(0x260, u4WidthMB);
    MC_WRITE32(0x264, u4HeightMB);
#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("RISCWrite_MC(%d, %d);\n", 0x260>>2, u4WidthMB);
    }
#endif

    u4WidthMB += 1;

    if (prDecPrm->u4AddrMode == ADDR_MODE_5351_NORMAL)
    {
        u4WidthMB = ((prDecPrm->u4LineSize + 15)/16) - 1;
        u4WidthMB = ((u4WidthMB + 3) / 4) * 4;
        MC_WRITE32(0x22C, u4WidthMB);
    }
    else
    {
        MC_WRITE32(0x22C, u4WidthMB);
    }

#ifdef CC_DUMP_AVS_RISC_SETTING
    if (prDecPrm->fgDumpSetting)
    {
        Printf("RISCWrite_PP(%d, 256);\n", RW_AVS_PP_AVS_MODE>>4);
        Printf("RISCWrite_MC(%d, 1);\n", 0x250>>2);
        Printf("RISCWrite_MC(%d, 1);\n", 0x220>>2);
        Printf("RISCWrite_MC(%d, %d);\n", 0x264>>2, u4HeightMB);
        Printf("RISCWrite_MC(%d, %d);\n", 0x22C>>2, u4WidthMB);
    }
#endif
    LOG(6, "PP %d checksum = 0x%08x\n", 0x238 >> 2, MC_READ32(0x238));
    LOG(6, "PP %d checksum = 0x%08x\n", 0x260 >> 2, MC_READ32(0x260));
    LOG(6, "PP %d checksum = 0x%08x\n", 0x264 >> 2, MC_READ32(0x264));
    LOG(6, "PP %d checksum = 0x%08x\n", 0x22C >> 2, MC_READ32(0x22C));
}


BOOL _VDEC_AvsUpdateFBList(VDEC_AVS_INFO_T *prVdec)
{
    VDEC_AVS_PIC_T* prPicHdr;
    VDEC_AVS_DECPRM_T* prDecPrm;

    prPicHdr = (VDEC_AVS_PIC_T*)&prVdec->rPicHdr;
    prDecPrm = (VDEC_AVS_DECPRM_T*)&prVdec->rDecParam;

    if (fgIsRefPic(prPicHdr->u4PicCodingType))
    {
        BOOL fgFindRef = FALSE;
        INT32 i;

        //UINT8 u1TmpFbIdx;
        //UCHAR ucTmpFbId = FBM_FB_ID_UNKNOWN;

        for (i = 0; i < (MAX_REF_PICNUM - 1); i++)
        {
            if (prDecPrm->arRefFBufInfo[i].fgValid)
            {
                prDecPrm->arRefFBufInfo[i].ucFbId = prDecPrm->prCurrFBufInfo->ucFbId;
                prDecPrm->arRefFBufInfo[i].fgValid = FALSE;
                prDecPrm->arRefFBufInfo[i].u1DecFBufIdx = prDecPrm->u1DecFBufIdx;
                fgFindRef = TRUE;

                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
                    prDecPrm->prCurrFBufInfo->ucFbId, 
                    FBM_FB_STATUS_READY);

                LOG(6, "%s Set the first frame as ready state fb id =%d\n", __FUNCTION__, 
                    prDecPrm->prCurrFBufInfo->ucFbId);

                if (i > 0)
                {
                    FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
                        prDecPrm->arRefFBufInfo[i-1].ucFbId, 
                        FBM_FB_STATUS_DISPLAYQ);

                    LOG(6, "%s Set the first frame as displayQ state fb id =%d\n", __FUNCTION__, 
                        prDecPrm->arRefFBufInfo[i-1].ucFbId);
                }
                FBM_UpdateReferenceList(prDecPrm->ucFbgId, 
                    prDecPrm->arRefFBufInfo[i].ucFbId, 
                    TRUE);
                prVdec->rDecParam.u1LastRefIdx = i;
                break;
            }
        }

        if (!fgFindRef)
        {
            UCHAR ucFbStatus;

            if (!prDecPrm->arRefFBufInfo[REF_LIST_2].fgValid)
            {
                // Update reference information
                FBM_UpdateReferenceList(prDecPrm->ucFbgId, prDecPrm->arRefFBufInfo[REF_LIST_0].ucFbId, FALSE);
                _VDEC_ClrAvsFBufInfo(prDecPrm, prDecPrm->arRefFBufInfo[REF_LIST_0].u1DecFBufIdx, TRUE);
            }
            
#if 0
            LOG(0, "%s FBM_UpdateReferenceList to rel ref fb id =%d\n", __FUNCTION__, 
                prDecPrm->arRefFBufInfo[REF_LIST_0].ucFbId);
#endif

            //ucTmpFbId = prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId;
            //u1TmpFbIdx = prDecPrm->arRefFBufInfo[REF_LIST_1].u1DecFBufIdx;

            if (!prDecPrm->arRefFBufInfo[REF_LIST_2].fgValid)
            {
                prDecPrm->arRefFBufInfo[REF_LIST_0].ucFbId = prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId;
                prDecPrm->arRefFBufInfo[REF_LIST_0].u1DecFBufIdx =prDecPrm->arRefFBufInfo[REF_LIST_1].u1DecFBufIdx;
                
                prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId = prDecPrm->arRefFBufInfo[REF_LIST_2].ucFbId;
                prDecPrm->arRefFBufInfo[REF_LIST_1].u1DecFBufIdx =prDecPrm->arRefFBufInfo[REF_LIST_2].u1DecFBufIdx;
            }

            prDecPrm->arRefFBufInfo[REF_LIST_2].ucFbId = prDecPrm->prCurrFBufInfo->ucFbId;
            prDecPrm->arRefFBufInfo[REF_LIST_2].u1DecFBufIdx = prDecPrm->u1DecFBufIdx;
            prDecPrm->arRefFBufInfo[REF_LIST_2].fgValid = FALSE;
           
            ucFbStatus = FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId);
            if ((ucFbStatus == FBM_FB_STATUS_DECODE) || (ucFbStatus == FBM_FB_STATUS_FIELD_READY) ||
                (ucFbStatus == FBM_FB_STATUS_READY))
            {
#if 0
                FBM_PIC_HDR_T * prFbmPicHdr = NULL;
                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId);
                LOG(5, "I/P pts = %d\n", prFbmPicHdr->u4PTS);
#endif

                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
                    prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId, 
                    FBM_FB_STATUS_DISPLAYQ);
                LOG(6, "%s Set ref fb id %d to display Q\n", __FUNCTION__, 
                    prDecPrm->arRefFBufInfo[REF_LIST_1].ucFbId);
            }

            FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
                prDecPrm->prCurrFBufInfo->ucFbId, 
                FBM_FB_STATUS_READY);

            LOG(6, "%s Set ref fb id %d to Ready\n", __FUNCTION__, 
                prDecPrm->arRefFBufInfo[REF_LIST_2].ucFbId);

            FBM_UpdateReferenceList(prDecPrm->ucFbgId, 
                prDecPrm->arRefFBufInfo[REF_LIST_2].ucFbId, 
                TRUE);
            prVdec->rDecParam.u1LastRefIdx = REF_LIST_2;
        }
    }
    else
    {
#if 0
        FBM_PIC_HDR_T * prFbmPicHdr = NULL;
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId);
        LOG(5, "B frame pts = %d\n", prFbmPicHdr->u4PTS);
#endif

#if 1
        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
            prDecPrm->prCurrFBufInfo->ucFbId, 
            FBM_FB_STATUS_DISPLAYQ);
#endif

        LOG(6, "Set B fb id %d to displayQ\n", prDecPrm->prCurrFBufInfo->ucFbId);

        _VDEC_ClrAvsFBufInfo(prDecPrm, 
            prDecPrm->u1DecFBufIdx,
            TRUE);

#if 0
        LOG(5, "%s FBM_SetFrameBufferStatus to ready fb id =%d\n", __FUNCTION__, 
            prDecPrm->prCurrFBufInfo->ucFbId);
#endif

#if 0
        FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, 
            prDecPrm->prCurrFBufInfo->ucFbId, 
            FBM_FB_STATUS_READY);
#endif
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



