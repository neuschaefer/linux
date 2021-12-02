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
 * $RCSfile: vdec_wmvdec.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_rvdec.c
 *  This header file declares exported APIs of RealVideo.
 */

#include "x_assert.h"
#include "x_printf.h"
#include "dmx_if.h"
#include "vdec_rvhw.h"
#include "vdec_rvutil.h"
#include "vdec_rvdec.h"
#include "vdec_rvapi.h"
#include "vdec_rvpsr.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "srm_drvif.h"

#ifdef USE_FBM
#include "fbm_drvif.h"
#endif

#ifdef MULTIMEDIA_AUTOTEST_SUPPORT
#include "x_mm_common.h"
#endif

#define VDEC_MM_WAIT_DISP_TIME 1000

#define SUPPORT_RPR

#define DEC_WAIT_CNT          300000  // time to wait for decode finish

#if 1  // RV_ERROR_CONCEALMENT
static UINT32 _u4RvErrConceal = (2 << 24) | (1 << 27);  // enable error conceal, 0x0A000000
#else
static UINT32 _u4RvErrConceal = 0;
#endif
static UINT32 _u4FastParsing = 0x40000000;
#if defined(VDEC_IS_POST_MT5368)
static UINT32 _u4CycleCnt = 1;
#else
static UINT32 _u4CycleCnt = 0x00800000;
#endif
static UINT32 _u4Rv8TblNum = 1;

static UINT32 _u4RefQpMb0[VDEC_MAX_ES];
UINT32 _u4VldWindow = 0;

static UINT32 _u4BwdIsI[VDEC_MAX_ES];

#ifndef RV_IRQ_DISABLE
static HANDLE_T _hVdecRvFinSema;
#endif

#ifdef VDEC_TIME_PROFILE
static HAL_TIME_T _rTimeHwS, _rTimeHwDt, _rTimeHwE;
static UINT32 u4DelayClock;
static UINT32 u4DramCycle;
static UINT32 u4DecCycle = 0;
#endif

//#define RV_MESAURE_DEC_TIME

#ifdef RV_MESAURE_DEC_TIME
#include "x_timer.h"
static UINT32 _u4DecTimeI[4] = {0};     // max min total cnt
static UINT32 _u4DecTimeP[4] = {0};
static UINT32 _u4DecTimeB[4] = {0};
static HAL_TIME_T _rStartTime;
static HAL_TIME_T _rEndTime;
static UINT32 _u4DecTotal;
static void DecTimeInit(void)
{
  UINT8 i;
  for (i = 0; i < sizeof(_u4DecTimeI)/sizeof(UINT32); i++)
  {
    _u4DecTimeI[i] = 0;
    _u4DecTimeP[i] = 0;
    _u4DecTimeB[i] = 0;
  }
  _u4DecTotal = 0;
  _u4DecTimeI[1] = 0xFFFFFFFF;
  _u4DecTimeP[1] = 0xFFFFFFFF;
  _u4DecTimeB[1] = 0xFFFFFFFF;

}

static void DecTimeStat(VDEC_RV_INFO_T *prRvInfo)
{
  HAL_TIME_T _rDeltTime;
  UINT32 avg;
  if (!prRvInfo)
  {
    return;
  }
  
  _u4DecTotal++;
  HAL_GetDeltaTime(&_rDeltTime, &_rEndTime, &_rStartTime);

  switch(prRvInfo->rRvPicAttr.ePtype)
  {
  case RV_INTRAPIC:
  case RV_FORCED_INTRAPIC:
    if (_u4DecTimeI[0] < _rDeltTime.u4Micros)
    {
      _u4DecTimeI[0] = _rDeltTime.u4Micros;
    }
    if (_u4DecTimeI[1] > _rDeltTime.u4Micros)
    {
      _u4DecTimeI[1] = _rDeltTime.u4Micros;
    }
    _u4DecTimeI[2] += _rDeltTime.u4Micros;
    _u4DecTimeI[3] ++;
    break;  
  case RV_INTERPIC:
    if (_u4DecTimeP[0] < _rDeltTime.u4Micros)
    {
      _u4DecTimeP[0] = _rDeltTime.u4Micros;
    }
    if (_u4DecTimeP[1] > _rDeltTime.u4Micros)
    {
      _u4DecTimeP[1] = _rDeltTime.u4Micros;
    }
    _u4DecTimeP[2] += _rDeltTime.u4Micros;
    _u4DecTimeP[3] ++;
    break;
  case RV_TRUEBPIC:  
    if (_u4DecTimeB[0] < _rDeltTime.u4Micros)
    {
      _u4DecTimeB[0] = _rDeltTime.u4Micros;
    }
    if (_u4DecTimeB[1] > _rDeltTime.u4Micros)
    {
      _u4DecTimeB[1] = _rDeltTime.u4Micros;
    }
    _u4DecTimeB[2] += _rDeltTime.u4Micros;
    _u4DecTimeB[3] ++;
    break;
  }


  if ((_u4DecTotal % 1000) == 0)
  {
    Printf("RV Dec Time Stat\n");
    Printf("Total Frames:%d\n", _u4DecTotal);
    if (_u4DecTimeI[3] > 0)
    {
      avg = _u4DecTimeI[2]/_u4DecTimeI[3];
      Printf("Dec Time I: (max, min, avg, cnt) (%d,%d,%d,%d) \n", _u4DecTimeI[0],_u4DecTimeI[1], avg, _u4DecTimeI[3]); 
    }
    if (_u4DecTimeP[3] > 0)
    {
      avg = _u4DecTimeP[2]/_u4DecTimeP[3];
      Printf("Dec Time P: (max, min, avg, cnt) (%d,%d,%d,%d) \n", _u4DecTimeP[0],_u4DecTimeP[1], avg, _u4DecTimeP[3]); 
    }
    if (_u4DecTimeB[3] > 0)
    {
      avg = _u4DecTimeB[2]/_u4DecTimeB[3];
      Printf("Dec Time B: (max, min, avg, cnt) (%d,%d,%d,%d) \n", _u4DecTimeB[0],_u4DecTimeB[1], avg, _u4DecTimeB[3]); 
    }
  }
}

#endif

#ifndef RV_IRQ_DISABLE
void _RVVLDIsr(UINT16 u2Vector)
{
    if (u2Vector == RVVLD_VECTOR)
    {
        vWriteREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_CLR, 1);
        vWriteREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_CLR, 0);
        #ifdef VDEC_IS_POST_MT5368
        //vWriteREG(MISC_BASE + RW_VDEC_MISC_INTERRUPT_CLR, 0x1011);
        //vWriteREG(MISC_BASE + RW_VDEC_MISC_INTERRUPT_CLR, 0x1001);
        BIM_ClearIrq(RVVLD_VECTOR);
        #endif
        VERIFY(x_sema_unlock(_hVdecRvFinSema) == OSR_OK);
    }
}
#endif


static void SetRVPicHdr(UCHAR u1FbId, UCHAR ucEsId)
{
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    FBM_SEQ_HDR_T *prFbmSeqHdr = NULL;
    VDEC_RV_INFO_T* prVdec;
    RvPicAttr *prPic;

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
    prPic = &(prVdec->rRvPicAttr);

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rOther.u1FbgId, u1FbId);
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdec->rOther.u1FbgId);
    if (prFbmPicHdr == NULL || prFbmSeqHdr == NULL)
    {
        ASSERT(0);
        return;
    }
    //prFbmPicHdr->i4TemporalRef = prVdec->rOther.u4CurrPicNo;
    //UINT8 u1RepeatFrameCount;  //(RPTFRM) need to add in
    prFbmPicHdr->fgProgressiveFrm = TRUE;
    prFbmPicHdr->fgRepFirstFld = FALSE;  //prVdec->rPicLayer.fgRepeatFirstField;  !!!
    prFbmPicHdr->fgTopFldFirst = TRUE;  //prVdec->rPicLayer.fgTopFieldFirst;  !!!
    //prFbmPicHdr->ucPicStruct = prVdec->rPicLayer.
    prFbmPicHdr->u4PTS = prVdec->rPesInfo.u4PTS;

    prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;
    
    x_memcpy(&prFbmPicHdr->rExtra,
        &prVdec->rPesInfo.rExtra,
        sizeof(FBM_PIC_EXTRA_INFO_T));
    prFbmPicHdr->u8OffsetI = prVdec->rPesInfo.u8OffsetI;
    prFbmPicHdr->u4Timestap = prVdec->rPesInfo.u4Timestap;
    prFbmPicHdr->u8Offset = prVdec->rPesInfo.u8Offset;

    prFbmPicHdr->u4PicWidth = prPic->u4OrgWidth;
    prFbmPicHdr->u4PicHeight = prPic->u4OrgHeight;
    prFbmPicHdr->u4PicWidthPitch = prPic->u4LineSize;

    if (!(prFbmSeqHdr->fgResizeSmallPic) &&
       (prFbmSeqHdr->u2OrgHSize != prFbmPicHdr->u4PicWidth ||
        prFbmSeqHdr->u2OrgVSize != prFbmPicHdr->u4PicHeight)
    )
    {
        prFbmPicHdr->fgSeamlessDisp = TRUE;
    }
    else
    {
        prFbmPicHdr->fgSeamlessDisp = FALSE;
    }

#ifdef RV_TRACKING
    if (prPic->ePtype == RV_TRUEBPIC)
    {
        prFbmPicHdr->fgTrackingMode = TRUE;
        LOG(3, "[%d]b2r_track\n", prVdec->rOther.u4CurrPicNo);
    }
    else
    {
        prFbmPicHdr->fgTrackingMode = FALSE;
    }
#endif

    prFbmPicHdr->i4TemporalRef = prVdec->rOther.u4CurrPicNo;
}

static void SetRVSeqHdr(UCHAR ucEsId)
{
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    VDEC_ES_INFO_T* prVdecEsInfo = NULL;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;
    VDEC_RV_INFO_T* prVdec;
    RvPicAttr *prPic;

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
    if (prVdec == NULL)
    {
        LOG(0, "%s(%d): prVdec is NULL\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    if (prVdec->rOther.u1GetSeqHdr)
    {
        return;
    }

    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdec->rOther.u1FbgId);
    if (prFbmSeqHdr == NULL)
    {
        LOG(0, "%s(%d): prFbmSeqHdr is NULL\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if (prVdecEsInfo == NULL)
    {
        LOG(0, "%s(%d): prVdecEsInfo is NULL\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
    if (prVdecEsInfoKeep == NULL)
    {
        LOG(0, "%s(%d): prVdecEsInfoKeep is NULL\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    prVdec->rOther.u4DeltaPTS = 3000;
    prFbmSeqHdr->fgProgressiveSeq = TRUE;
    prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
    if (!prVdecEsInfoKeep->fgVPush)
    {
        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
    }
    else // data from ffmpeg
    {
        switch(prVdecEsInfo->rMMSeqInfo.e_frame_rate)
        {
        case VDEC_SRC_FRAME_RATE_23_976: /* 24000/1001 (23.976...) */
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24_;
            break;
        case VDEC_SRC_FRAME_RATE_24:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24;
            break;
        case VDEC_SRC_FRAME_RATE_25:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_25;
            break;
        case VDEC_SRC_FRAME_RATE_29_97: /* 30000/1001 (29.97...) */
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30_;
            break;
        case VDEC_SRC_FRAME_RATE_30:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
            break;
        case VDEC_SRC_FRAME_RATE_50:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_50;
            break;
        case VDEC_SRC_FRAME_RATE_59_94: /* 60000/1001 (59.94...) */
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60_;
            break;
        case VDEC_SRC_FRAME_RATE_60:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
            break;
        default:
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
            break;
        }
    }

    prPic = &(prVdec->rRvPicAttr);
#if !defined(CC_RESIZE_SMALL_IMAGE) || defined(CC_RV_EMULATION)
    prFbmSeqHdr->u2LineSize = prPic->u4LineSize;
    prFbmSeqHdr->u2HSize = prPic->u4OrgWidth;      // rSeqHdrR.u4PicWidthSrc;
    prFbmSeqHdr->u2VSize = prPic->u4OrgHeight;     // rSeqHdrR.u4PicHeightSrc;
    prFbmSeqHdr->u2OrgHSize = prPic->u4OrgWidth;   // rSeqHdrR.u4PicWidthSrc;
    prFbmSeqHdr->u2OrgVSize = prPic->u4OrgHeight;  // rSeqHdrR.u4PicHeightSrc;
#else
    if ((prPic->u4OrgWidth < FBM_IMG_RESIZE_LIMITATION ||
         prPic->u4OrgHeight < FBM_IMG_RESIZE_LIMITATION) &&
        (!((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))))
    {
        prFbmSeqHdr->fgResizeSmallPic = TRUE;
        
        if (prPic->u4OrgWidth < FBM_IMG_RESIZE_LIMITATION)
        {
            prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
        }
        else if (prPic->u4OrgWidth > FBM_IMG_RESIZE_LIMITATION_MAX)
        {
            prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
        }
        else
        {
            prFbmSeqHdr->u2LineSize = prPic->u4LineSize;
            prFbmSeqHdr->u2HSize = (prPic->u4OrgWidth)&(0xfff0); //ref _VDP_SeamlessJob //prPic->u4OrgWidth;
            prFbmSeqHdr->u2OrgHSize = prPic->u4OrgWidth;
        }

        if (prPic->u4OrgHeight < FBM_IMG_RESIZE_LIMITATION)
        {
            prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
        }
        else if (prPic->u4OrgHeight > FBM_IMG_RESIZE_LIMITATION_MAX)
        {
            //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
        }
        else
        {
            prFbmSeqHdr->u2VSize = prPic->u4OrgHeight;
            prFbmSeqHdr->u2OrgVSize = prPic->u4OrgHeight;
        }
    }
    else
    {
        prFbmSeqHdr->u2LineSize = prPic->u4LineSize;
        prFbmSeqHdr->u2HSize = prPic->u4OrgWidth;      // rSeqHdrR.u4PicWidthSrc;
        prFbmSeqHdr->u2VSize = prPic->u4OrgHeight;     // rSeqHdrR.u4PicHeightSrc;
        prFbmSeqHdr->u2OrgHSize = prPic->u4OrgWidth;   // rSeqHdrR.u4PicWidthSrc;
        prFbmSeqHdr->u2OrgVSize = prPic->u4OrgHeight;  // rSeqHdrR.u4PicHeightSrc;
    }
#endif

    prFbmSeqHdr->u4ResizeWidth = prFbmSeqHdr->u2OrgHSize;   
    prFbmSeqHdr->u4ResizeHeight = VDEC_ALIGN_MASK(prFbmSeqHdr->u2OrgVSize,15); 
    prFbmSeqHdr->ucAspRatInf = VDEC_ASPECT_RATIO_1_1;
    prFbmSeqHdr->fgPixelAspectRatio = TRUE;

    prVdec->rOther.u1GetSeqHdr = 1;
}


void vRVVPrsIPProc(UCHAR ucEsId, INT32* pi4RetValue, BOOL fgIsP)
{
    VDEC_RV_INFO_T* prVdec;

    *pi4RetValue = E_VDEC_OK;

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
	
    if (prVdec->rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prVdec->rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_NOT_READY);
        }
    }

#ifdef REPEAT_FRAME
    if(prVdec->rOther.u4repeatframe)
    {
        // do nothing
        return;
    }
#endif

    vSetRVFRefBuf(prVdec, prVdec->rDram.u4BRefBufIdx);
    vSetRVBRefBuf(prVdec, 1 - prVdec->rDram.u4FRefBufIdx);
    vSetRVDecBuf(prVdec, prVdec->rDram.u4BRefBufIdx);
  
    #ifdef USE_FBM
    {
        UINT32 u4Yaddr, u4Caddr;
        UCHAR ucBRefFbId;
        UCHAR ucFRefFbId;
        UCHAR ucFbId;

        SetRVSeqHdr(ucEsId);

        FBM_GetRefFrameBuffer(prVdec->rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
        if (FBM_FB_ID_UNKNOWN != ucBRefFbId)
        {
            VDEC_ES_INFO_T* prVdecEsInfo;
            FBM_PIC_HDR_T *prFbmPicHdr = NULL;
            BOOL fgNotDisplay;

            prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);                
            prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rOther.u1FbgId, ucBRefFbId);

            if(prFbmPicHdr)
            {
                if(prVdec->rOther.fgFirstDispFrame)
                {
                     prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                     prVdec->rOther.fgFirstDispFrame = FALSE;
                }
                else
                {
                    if(prVdec->rPesInfo.fgDtsValid/* && (prFbmPicHdr->u4PTS >= prVdec->rOther.u4LastPTS)*/)
                    {
                        prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                    }
                    else
                    {
                        prFbmPicHdr->u4PTS = prVdec->rOther.u4LastPTS + prVdec->rOther.u4DeltaPTS;
                        prVdec->rOther.u4LastPTS += prVdec->rOther.u4DeltaPTS;
                    }
                }

                fgNotDisplay = VDEC_IsNotDisplay(ucEsId, prFbmPicHdr->u4PTS, 0, 0);
            }
            else
            {
                fgNotDisplay = VDEC_IsNotDisplay(ucEsId, 0, 0, 0);
            }
            if (fgNotDisplay)
            {
                #ifdef RV_PRINT_FBM_STATUS
                UCHAR u1state;
                u1state = FBM_GetFrameBufferStatus(prVdec->rOther.u1FbgId,ucBRefFbId);
                LOG(1,"[%d]RV set I/P FB(%d) from %d to EMPTY\n", prVdec->rOther.u4CurrPicNo - 1,ucBRefFbId,u1state);
                #endif
                FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY); 
            }
            else
            {
                #ifdef RV_PRINT_FBM_STATUS
                UCHAR u1state;
                u1state = FBM_GetFrameBufferStatus(prVdec->rOther.u1FbgId,ucBRefFbId);
                LOG(1,"[%d]RV set I/P FB(%d) from %d to DISPALY_Q\n", prVdec->rOther.u4CurrPicNo - 1,ucBRefFbId,u1state);
                #endif
                #ifdef CC_REALD_3D_SUPPORT
                _VDEC_Check2D3DSwitch(ucEsId, ucBRefFbId);
                #endif
                FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                prVdecEsInfo->u4DisplayQPicCnt++;
                if(prFbmPicHdr)
                {
                    VDEC_CheckStopStatus(ucEsId, prFbmPicHdr->u4PTS, 0);
                }
                else
                {
                    VDEC_CheckStopStatus(ucEsId, 0, 0);
                }
            }               
#ifdef SUPPORT_RPR
            // RPR check begin in other file
            if (fgIsP && prFbmPicHdr)
            {
                //check ref resolution 
                if ((prFbmPicHdr->u4PicWidth != prVdec->rRvPicAttr.u4OrgWidth)
                ||(prFbmPicHdr->u4PicHeight != prVdec->rRvPicAttr.u4OrgHeight)
                )
                {
                    //BOOL fgCheck = FALSE;
                    UINT32 u4BRefYaddr, u4BRefCaddr;
                    UINT32 u4OriAddrY, u4OriAddrC;
                    RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
                    do 
                    {
                        //get buffer first
                        FBM_GetExtraFrameBuffer(prVdec->rOther.u1FbgId,&u4BRefYaddr,&u4BRefCaddr);  
                        if (!u4BRefYaddr || !u4BRefCaddr) 
                        {
                            LOG(0, "RPR, get buffer fail\n");
                            break;
                        }

                        FBM_GetFrameBufferAddr(prVdec->rOther.u1FbgId, ucBRefFbId,&u4OriAddrY, &u4OriAddrC);

                        //resize
                        x_memset(&rScaleParamV,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));
                        rScaleParamV.u4IsRsIn = 0;
                        rScaleParamV.u4InMode = E_RZ_VDO_OUTMD_420;
                        rScaleParamV.u4YSrcBase = u4OriAddrY;
                        rScaleParamV.u4YSrcBufLen = prFbmPicHdr->u4PicWidthPitch;
                        rScaleParamV.u4YSrcHOffset = 0;
                        rScaleParamV.u4YSrcVOffset = 0;
                        rScaleParamV.u4YSrcW = prFbmPicHdr->u4PicWidth;
                        rScaleParamV.u4YSrcH = prFbmPicHdr->u4PicHeight;
                        rScaleParamV.u4CSrcBase = u4OriAddrC;
                        rScaleParamV.u4CSrcHOffset = 0;
                        rScaleParamV.u4CSrcVOffset = 0;
                        rScaleParamV.u4CSrcW = prFbmPicHdr->u4PicWidth >> 1;
                        rScaleParamV.u4CSrcH = prFbmPicHdr->u4PicHeight >> 1;
                        rScaleParamV.u4IsRsOut = 0;
                        rScaleParamV.u4OutMode = E_RZ_VDO_OUTMD_420;
                        rScaleParamV.u4IsVdo2Osd = 0;

                        rScaleParamV.u4YTgHOffset = 0;
                        rScaleParamV.u4YTgVOffset = 0;
                        rScaleParamV.u4YTgBase = u4BRefYaddr;
                        rScaleParamV.u4YTgCM = E_RZ_VDO_OUTMD_420;
                        rScaleParamV.u4YTgW = prVdec->rRvPicAttr.u4OrgWidth;
                        rScaleParamV.u4YTgH = prVdec->rRvPicAttr.u4OrgHeight;
                        rScaleParamV.u4YTgBufLen= prFbmPicHdr->u4PicWidthPitch;//prVdec->rRvPicAttr.u4OrgWidth;
                        rScaleParamV.u4CTgBase = u4BRefCaddr;
                        rScaleParamV.u4CTgW=rScaleParamV.u4YTgW>>1;
                        rScaleParamV.u4CTgH=rScaleParamV.u4YTgH>>1;
                        rScaleParamV.u4OutSwap = 6;
                        rScaleParamV.u4SrcSwap = 6;

                        IMGRZ_Lock();
                        IMGRZ_ReInit();
                        IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
                        IMGRZ_Scale((void *)(&rScaleParamV));
                        IMGRZ_Flush();
                        IMGRZ_Wait();
                        IMGRZ_Unlock();

                        //replace bRef pic address
                        if (prVdec->rDram.u4FRefBufIdx == 0 && prVdec->rDram.u4BRefBufIdx == 1)
                        {
                            prVdec->rDram.pu1Pic0Y = (UINT8*)u4BRefYaddr;
                            prVdec->rDram.pu1Pic0C = (UINT8*)u4BRefCaddr;
                        }
                        else if (prVdec->rDram.u4FRefBufIdx == 1 && prVdec->rDram.u4BRefBufIdx == 0)
                        {
                            prVdec->rDram.pu1Pic1Y = (UINT8*)u4BRefYaddr;
                            prVdec->rDram.pu1Pic1C = (UINT8*)u4BRefCaddr;
                        }

                        //fgCheck = TRUE;
                    }
                    while (0);
#if 0 // todo    
                    if (!fgCheck)
                    {
                    LOG(3, "Wrong reference for P PIC [RPR]\n");
                    *pi4RetValue = E_VDEC_FAIL;
                    return;
                    }
#endif 
                }
            }
            //PRP check end
#endif
        }
        else if (fgIsP)
        {
            LOG(3, "Wrong reference for P PIC\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        ucFbId = FBM_GetEmptyRefFrameBuffer(prVdec->rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
        ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);
        FBM_GetFrameBufferAddr(prVdec->rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

        ASSERT(u4Yaddr !=0 && u4Caddr !=0);
        // reset mem here
        SetRVPicHdr(ucFbId, ucEsId);

        prVdec->rDram.pu1Pic2Y = (UINT8*)u4Yaddr;
        prVdec->rDram.pu1Pic2C = (UINT8*)u4Caddr;
        if (prVdec->rDram.u4DecBufIdx == 0) //pic1
        {
            prVdec->rOther.u1FbId0 = ucFbId;    
        }
        else //pic2
        {
            prVdec->rOther.u1FbId1 = ucFbId;
        }
        if (prVdec->rDram.u4FRefBufIdx == 0 && prVdec->rDram.u4BRefBufIdx == 1)
        {
            prVdec->rDram.pu1Pic1Y = (UINT8*)u4Yaddr;
            prVdec->rDram.pu1Pic1C = (UINT8*)u4Caddr;
        }
        else if (prVdec->rDram.u4FRefBufIdx == 1 && prVdec->rDram.u4BRefBufIdx == 0)
        {
            prVdec->rDram.pu1Pic0Y = (UINT8*)u4Yaddr;
            prVdec->rDram.pu1Pic0C = (UINT8*)u4Caddr;
        }
        else
        {
            ASSERT(0);
        }

        FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId,ucFbId,FBM_FB_STATUS_DECODE);
    }
    #endif
}


void vRVVPrsBProc(UCHAR ucEsId, INT32* pi4RetValue)
{
    VDEC_RV_INFO_T* prVdec;

    *pi4RetValue = E_VDEC_OK;
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);
	
    if (prVdec->rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prVdec->rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_NOT_READY);
        }
    }
    vSetRVDecBuf(prVdec, 2);
    //vSaveFrmBufPrm(prVdec->rDram.u4DecBufIdx);
  
    #ifdef USE_FBM
    {
        UINT32 u4Yaddr, u4Caddr;
        UCHAR ucBRefFbId;
        UCHAR ucFRefFbId;
        UCHAR ucFbId;

        SetRVSeqHdr(ucEsId);
        
        FBM_GetRefFrameBuffer(prVdec->rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
        if (FBM_FB_ID_UNKNOWN == ucBRefFbId || FBM_FB_ID_UNKNOWN == ucFRefFbId)
        {
            LOG(3, "Wrong reference for B PIC\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }
        
        ucFbId = FBM_GetEmptyBFrameBuffer(prVdec->rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
        ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);
        
        FBM_GetFrameBufferAddr(prVdec->rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);
        ASSERT(u4Yaddr !=0 && u4Caddr !=0);

        // reset mem here
        SetRVPicHdr(ucFbId, ucEsId);
      
        prVdec->rDram.pu1Pic2Y = (UINT8*)u4Yaddr;
        prVdec->rDram.pu1Pic2C = (UINT8*)u4Caddr;
        prVdec->rOther.u1FbId2 = ucFbId;
    }
    #endif

#ifdef RV_TRACKING
    vSetRVVSyncPrmBufPtr(ucEsId, 2);
#endif
}


#if defined(VDEC_IS_POST_MT5368)
static BOOL fgVDecIsVLDFetchOk(void)
{
    if ((u4ReadREG(RO_VLD_FETCHOK) & VLD_FETCH_OK) == 0)
    {
        return (FALSE);
    }
    return (TRUE);
}

static BOOL fgVDecWaitVldFetchOk(void)
{
    UINT32 u4Cnt;
  
    if (!fgVDecIsVLDFetchOk())
    {
        u4Cnt = 0;
        while (!fgVDecIsVLDFetchOk())
        {
            u4Cnt++;
            if (u4Cnt >= WAIT_THRD)
            {
                return (FALSE);
            }
        }
    }

    return (TRUE);
}

void vRVInitBarrelShifter(UCHAR ucEsId, UINT32 u4Addr)
{
    INT32 u4Cnt = 0;
    VDEC_RV_INFO_T *prVdec;
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    vWriteREG(RW_VLD_VSTART, (prVdec->rPesInfo.u4FifoStart >> 6));
    vWriteREG(RW_VLD_VEND, (prVdec->rPesInfo.u4FifoEnd >> 6));

    if (u4ReadREG(RO_VLD_SRAMCTRL) & PROCESS_FLAG)
    {
        while((u4ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_SCLK) == 0)
        {
            u4Cnt++;
            if(u4Cnt >= WAIT_THRD)
            {
                LOG(1, "WaitSramStable Fail\n");
                break;
            }
        }
    }

    vWriteREG(RV_VLD_REG_OFST + 0x84, 0x1);

    vWriteREG(RW_VLD_RPTR, u4Addr);
    vWriteREG(WO_VLD_WPTR, 0xffffffff);

    vWriteREG(WO_VLD_SRST, 0x100);
    vWriteREG(WO_VLD_SRST, 0x0);

    // initial fetch
    vWriteREG(RW_VLD_PROC, VLD_INIFET);

    VERIFY(fgVDecWaitVldFetchOk());

    // initial barrel shift
    vWriteREG(RW_VLD_PROC, VLD_INIBR);           
}
#else
void vRVInitBarrelShifter(UCHAR ucEsId, UINT32 u4Addr)
{
    VDEC_RV_INFO_T *prVdec;
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    vWriteREG(RV_VLD_REG_OFST + 0x84, 0x1);

    vWriteREG(VLD_REG_OFST + 0xb0, u4Addr);
    vWriteREG(VLD_REG_OFST + 0x110, 0xffffffff);
    vWriteREG(VLD_REG_OFST + 0xb4, (prVdec->rPesInfo.u4FifoStart >> 6));
    vWriteREG(VLD_REG_OFST + 0xb8, (prVdec->rPesInfo.u4FifoEnd >> 6));
    vWriteREG(VLD_REG_OFST + 0x8c, 0x00100000);

    while (!(u4ReadREG(VLD_REG_OFST + 0xe8) & 0x00000001));  // wait for FFSR bit 0 raise

    vWriteREG(VLD_REG_OFST + 0x8c, 0x00800000);

    while((((u4ReadREG(VLD_REG_OFST + 0xec) & 0xf) * 4) + 
        ((u4ReadREG(VLD_REG_OFST + 0xec) >> 24) & 0x3)) !=3);
}
#endif


#ifdef CC_RV_EMULATION
extern VOID _RVLoadBitstream(UINT32 n);
extern BOOL _VdecRvEmuIsItemEnd(VOID);
extern BOOL fgGetRvPic(RvPicAttr *prPic);
extern BOOL _fgRVEmuEnd;
extern UINT32 _u4RVEmuEndPicNum;
#endif

BOOL fgVParserProcRV(UCHAR ucEsId, INT32* pi4RetValue)
{
    VDEC_RV_INFO_T *prRvInfo;
    RvPicAttr *prPic;
    RVOther *prOther;
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    UNUSED(prVdecEsInfo);
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prRvInfo);
    prPic = &(prRvInfo->rRvPicAttr);
    prOther = &prRvInfo->rOther;

#ifndef CC_RV_EMULATION

#ifdef RV_SW_HDR_PRS // normal flow, use CPU to parse parameters
    RVParseFrameHeader(ucEsId, prPic);
#else // normal flow, use barrel shifter to parse parameters
    _VDEC_LockVld(ucEsId, prVdecEsInfo->u4VldId);
    vResetRVVDec(ucEsId);
    vSetRVVldVFifo(ucEsId);
    vRVInitBarrelShifter(ucEsId, prRvInfo->rPesInfo.u4VldReadPtr);
    RVVideoDecDecodeFrameHead(ucEsId, prPic);
    _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);
#endif

#else
    // emulation flow, read parameters directly from FRMINFO buffer
    if (_VdecRvEmuIsItemEnd()
    || prOther->u4CurrPicNo >= _u4RVEmuEndPicNum
    || !fgGetRvPic(prPic))
    {
        _fgRVEmuEnd = TRUE;
        return FALSE;
    }

    // load frame bitstream
    _RVLoadBitstream(prOther->u4CurrPicNo);
#endif

    // Maximum resolution for RV is 2048x1152
    if (prPic->u4Width > 2048 || prPic->u4Height > 1152)
    {
        *pi4RetValue = E_VDEC_RES_NOT_SUPPORT;
        return TRUE;
    }

#if defined(__MODEL_slt__) || defined(CC_RV_EMULATION)
    // change prPic->u4LineSize from 64 align to 16 align
    prPic->u4LineSize = prPic->u4Width; // TODO: consider DDR3 mode
#endif

    switch (prPic->ePtype)
    {
    case RV_INTRAPIC:
    case RV_FORCED_INTRAPIC:
    case RV_INTERPIC:
        if (prOther->u1FbgId != FBM_FBG_ID_UNKNOWN)
        {
            // Check if reset is required
            if (FBM_ChkFrameBufferFlag(prOther->u1FbgId, FBM_FLAG_RESET))
            {
                FBM_ResetGroup(prOther->u1FbgId);
                FBM_ClrFrameBufferFlag(prOther->u1FbgId, FBM_FLAG_RESET);
                FBM_ClrFrameBufferFlag(prOther->u1FbgId, FBM_FLAG_NOT_READY);  
            }
        }
        vRVVPrsIPProc(ucEsId, pi4RetValue, prPic->ePtype == RV_INTERPIC);
        break;

    case RV_TRUEBPIC:
        if (prOther->u1FbgId != FBM_FBG_ID_UNKNOWN)
        {
            // Check if reset is required
            if (FBM_ChkFrameBufferFlag(prOther->u1FbgId, FBM_FLAG_RESET))
            {
                FBM_ResetGroup(prOther->u1FbgId);
                FBM_ClrFrameBufferFlag(prOther->u1FbgId, FBM_FLAG_RESET);
                FBM_ClrFrameBufferFlag(prOther->u1FbgId, FBM_FLAG_NOT_READY);  
            }
        }
        vRVVPrsBProc(ucEsId, pi4RetValue);
        break;
    }
    
    return TRUE;
}


#if defined(VDEC_IS_POST_MT5368)
static void vVldInit(UCHAR ucEsId)
{
    VDEC_RV_INFO_T *prVdec;
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    vWriteREG(VLD_TOP_BASE + 20*4, 0x80000000);
    if (BSP_IsFPGA())
    {
        vWriteREG(VLD_TOP_BASE + 10*4, (UINT32)(prVdec->rDram.pu1VldPred)); // suppose this is only for FPGA
    }
    else
    {
        // requested by ChunChia, 20090519
        vWriteREG(VLD_TOP_BASE + 10*4, 0);
    }
}
#else
static void vVldInit(UCHAR ucEsId)
{
    VDEC_RV_INFO_T *prVdec;
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    vWriteREG(RV_VLD_REG_OFST + 38*4, 0x80000000);
    if (BSP_IsFPGA())
    {
        vWriteREG(RV_VLD_REG_OFST + 39*4, (UINT32)(prVdec->rDram.pu1VldPred));
    }
    else
    {
        // requested by ChunChia, 20090519
        vWriteREG(RV_VLD_REG_OFST + 0x9C, 0);
    }
}
#endif

static void vMvInit(UCHAR ucEsId)
{
    VDEC_RV_INFO_T *prVdec;
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    vWriteREG(AVC_MV_REG_OFST + 131*4, ((UINT32)prVdec->rDram.pu1MvDirect) >> 4);  // it is a 16-byte address!
    vWriteREG(AVC_MV_REG_OFST + 134*4, 0);
}

static void vMcInit(void)
{
  // MC global setting
  //vWriteREG(MC_REG_OFST + 39*4, (MC_REF_OFFSET/512) + 0x2FD0);
  //vWriteREG(MC_REG_OFST + 40*4, (MC_REF_OFFSET/256) + 0x7F80);

  vWriteREG(MC_REG_OFST + 28*4, 0x0);
#ifndef CC_RV_EMULATION
  vWriteREG(MC_REG_OFST + 36*4, 2); // 64x32
#else
  vWriteREG(MC_REG_OFST + 36*4, 4); // 16x32
#endif
  vWriteREG(MC_REG_OFST +  6*4, 0x0);
  vWriteREG(MC_REG_OFST + 38*4, 0x0);
  vWriteREG(MC_REG_OFST + 114*4, 0x0);
  vWriteREG(MC_REG_OFST + 9*4, 0x4);

  // requested by ChunChia, 20090519
#ifndef CC_RV_EMULATION
  vWriteREG(MC_REG_OFST + 474*4, 2);
#else
  vWriteREG(MC_REG_OFST + 474*4, u4ReadREG(MC_REG_OFST + 474*4) & 0xFFFFFEFF);
#endif
  vWriteREG(MC_REG_OFST + 0x71C, 0x88007711);
  vWriteREG(MC_REG_OFST + 0x5E4, 0x00001000);
}

#ifdef CC_RV_EMULATION
static UINT32 RV_VLD_SHIFT_BIT(UINT32 u4ShiftCnt)
{
  UINT32 u4Val;
  
  if (u4ShiftCnt == 0)
  {
    u4Val = u4ReadREG(RV_VLD_REG_OFST);
  }
  else
  {
    while (u4ShiftCnt > 0)
    {
      u4Val = u4ReadREG(RV_VLD_REG_OFST + 4);
      u4ShiftCnt--;
    }
  }
  
  return u4Val;
}
#endif


BOOL fgVDecProcRV(UCHAR ucEsId)
{
    VDEC_RV_INFO_T *prRvInfo;
    RvPicAttr *prPic;
    RVOther *prOther;
    UINT32 u4PpRvPara, u4Reg, i;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (ucEsId >= VDEC_MAX_ES)
    {
        return FALSE;
    }

    UNUSED(u4Reg);

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prRvInfo);
    prPic = &prRvInfo->rRvPicAttr;
    prOther = &prRvInfo->rOther;

    _VDEC_LockVld(ucEsId, prVdecEsInfo->u4VldId);

#ifdef RV_MESAURE_DEC_TIME
    HAL_GetTime(&_rStartTime);
#endif

    prOther->u4LastVldReadPtr = prRvInfo->rPesInfo.u4VldReadPtr;

//    Printf("fgVDecProcRV(%d, %d): 0x%08x\n", ucEsId, prOther->u4CurrPicNo, prOther->u4LastVldReadPtr);

    // VLD 
    vResetRVVDec(ucEsId);
    vSetRVVldVFifo(ucEsId);
#ifndef CC_RV_EMULATION
    vRVInitBarrelShifter(ucEsId, prRvInfo->rPesInfo.u4VldReadPtr);
    RVConsumeFrameHeader(ucEsId);
#else
    vRVInitBarrelShifter(ucEsId, (UINT32)prRvInfo->rDram.pu1VFifo);
    RV_VLD_SHIFT_BIT(prPic->u4HdrSkip);  // !!!
#endif
  
    //MV
    vMvInit(ucEsId);
    vWriteREG(AVC_MV_REG_OFST + 135 * 4, prPic->u4Iratio & 0x0000FFFF);

    //MC   
    vMcInit();
    vSetRVMcBufPtr(ucEsId);

#if MC_PP_MODE == 1
    vWriteREG(MC_REG_OFST + 136*4, 1);
#elif MC_PP_MODE == 0
    vWriteREG(MC_REG_OFST + 136*4, (prPic->u4DFP == 0 ? 1 : 0));
#endif
    vWriteREG(MC_REG_OFST + 139*4, prPic->u4LineSize / 16);
    vWriteREG(MC_REG_OFST + 142*4, (prPic->u4DFP == 0 ? ((!prPic->fgRV9 && (prPic->ePtype != RV_INTRAPIC && prPic->ePtype != RV_FORCED_INTRAPIC)) ? 2 : 3) : 0));
#if MC_PP_MODE == 1
    vWriteREG(MC_REG_OFST + 148*4, 1);
#elif MC_PP_MODE == 0
    vWriteREG(MC_REG_OFST + 148*4, (prPic->u4DFP == 0 ? 1 : 0));
#endif
    vWriteREG(MC_REG_OFST + 152*4, prPic->u4Width / 16 - 1);
    vWriteREG(MC_REG_OFST + 153*4, prPic->u4Height / 16 - 1);

    vWriteREG(MC_REG_OFST + 420*4, prPic->u4Iratio);       // B-picture I ratio
    vWriteREG(MC_REG_OFST + 130*4, prPic->u4Width);
    vWriteREG(MC_REG_OFST + 131*4, prPic->u4Height);

    if (prPic->ePtype == RV_INTRAPIC || prPic->ePtype == RV_FORCED_INTRAPIC)
    {
        VDEC_MULTI_USE(_u4BwdIsI) = 1;
    }
    else if (prPic->ePtype == RV_INTERPIC)
    {
        VDEC_MULTI_USE(_u4BwdIsI) = 0;
    }
    else if (prPic->ePtype == RV_TRUEBPIC)
    {
        vWriteREG(AVC_MV_REG_OFST + 134 * 4, VDEC_MULTI_USE(_u4BwdIsI));
    }

    vWriteREG(MC_REG_OFST + 460*4, 0x0000ffff);

    //PP
    if (prPic->ePtype == RV_TRUEBPIC)
    {
        u4PpRvPara = VDEC_MULTI_USE(_u4RefQpMb0) << 16;
        u4PpRvPara |= 3 << 10;
    }
    else
    {
        u4PpRvPara = prPic->u4Pquant << 16;
    }
    u4PpRvPara |= prPic->u4DFP << 9;
    u4PpRvPara |= (prPic->u4Width * prPic->u4Height <= 176 * 144 ? 1 : 0) << 8;
    u4PpRvPara |= _u4Rv8TblNum << 4;
    u4PpRvPara |= (prPic->fgRV9 ? 1 : 0) << 1;
    u4PpRvPara |= (prPic->fgRV9 ? 0 : 1);
    vWriteREG(RV_PP_REG_OFST + 2*4, u4PpRvPara);  // !!! 0x110011
#ifndef CC_RV_EMULATION
    vWriteREG(RV_PP_REG_OFST + 0xC, u4ReadREG(RV_PP_REG_OFST + 0xC) & 0xFFFFFFDF);
#endif

#ifdef ENABLE_RV_CRC
    vWriteREG(MISC_BASE + 1*4, 0x3);
#else
    vWriteREG(MISC_BASE + 1*4, 0);
#endif

    //RV VLD 
    vVldInit(ucEsId);
    vWriteREG(VLD_REG_OFST + 36*4, (prPic->u4Height << 16) | (prPic->u4Width / 16) | 0xFF00);  // !!! David

#if defined(VDEC_IS_POST_MT5368)
    vWriteREG(MC_BASE + 608*4, prPic->u4LineSize / 16);
#else
    vWriteREG(VLD_REG_OFST + 50*4, prPic->u4LineSize / 16);
#endif

#if defined(VDEC_IS_POST_MT5368)
    u4Reg = _u4RvErrConceal | _u4FastParsing | 0x3 | (prPic->fgRV9 ? 4 : 0);
    vWriteREG(RV_VLD_REG_OFST + 33*4, u4Reg);  // RV8 or RV9
    vWriteREG(VLD_TOP_BASE + 21*4, _u4CycleCnt);
    u4Reg = prPic->u4OrgHeight << 16 | prPic->u4OrgWidth;
    vWriteREG(VLD_TOP_BASE + 28*4, u4Reg);
    u4Reg = (prPic->u4Height / 16 - 1) << 16 | (prPic->u4Width / 16 - 1);
    vWriteREG(VLD_TOP_BASE + 26*4, u4Reg);
    u4Reg = ((prPic->u4Width / 16) * (prPic->u4Height / 16));
    vWriteREG(VLD_TOP_BASE + 27*4, u4Reg);
    vWriteREG(RV_VLD_REG_OFST + 35*4, prPic->u4MbaSize << 24 | prPic->u4PctszSize << 22);
#else
    vWriteREG(RV_VLD_REG_OFST + 33*4, _u4CycleCnt | _u4RvErrConceal | _u4FastParsing | 0x3 | (prPic->fgRV9 ? 4 : 0));  // RV8 or RV9
    vWriteREG(RV_VLD_REG_OFST + 34*4, (prPic->u4Width / 16 - 1) << 24 | prPic->u4OrgHeight << 12 | prPic->u4OrgWidth);
    vWriteREG(RV_VLD_REG_OFST + 35*4, prPic->u4MbaSize << 24 | prPic->u4PctszSize << 22 | ((prPic->u4Width / 16) * (prPic->u4Height / 16)) << 8 | (prPic->u4Height / 16 - 1));  //  0xbc39816
#endif
    vWriteREG(RV_VLD_REG_OFST + 36*4, prPic->u4Rtype << 26 | prPic->u4Pctsz << 23 | prPic->u4Tr << 10 | prPic->u4DFP << 9 | prPic->u4Oquant << 7 | prPic->u4Pquant << 2 | (UINT32)prPic->ePtype);  // 0x4000044
    vWriteREG(RV_VLD_REG_OFST + 37*4, prPic->u4Mba);
    vWriteREG(RV_VLD_REG_OFST + 61*4, prPic->u4NumSlice);
    for (i = 0; i < prPic->u4NumSlice; i++)
    {
        vWriteREG(RV_VLD_REG_OFST + (68 + i)*4, prPic->au4SliceSize[i]);
    }

    _u4VldWindow = u4ReadREG(RV_VLD_REG_OFST);

#ifdef VDEC_TIME_PROFILE
    prOther->u4CurDecMs = 0;
    prOther->u4CurDramCycle = 0;
    prOther->u4CurDecCycle = 0;
    //profile decoding time
    HAL_GetTime(&_rTimeHwS);

    u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
#if 0        
    if(prOther->u1DramMoniter == 1)  //total bandwidth
    {
    #if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368)
        u4DramCycle = SRM_DramMonitor(0, SRM_DRV_TOTAL, u4DelayClock, 0);
    #else
        u4DramCycle = SRM_DramMonitor(SRM_DRV_TOTAL, u4DelayClock, 0);
    #endif
    }
    else if(prOther->u1DramMoniter == 2)  //mpeg
#endif            
    {
    #if defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368)
        u4DramCycle = SRM_DramMonitor(0, SRM_DRV_VDEC_MC, u4DelayClock, 0);
    #else
        u4DramCycle = SRM_DramMonitor(SRM_DRV_VDEC_MC, u4DelayClock, 0);
    #endif
    }
#endif

#ifdef CC_RV_EMULATION
    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)prRvInfo->rDram.pu1VldPred),
        (UINT32)(VLD_PRED_OFFSET_SZ + MV_DIRECT_SZ));
    HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)prRvInfo->rDram.pu1VFifo),
        (UINT32)(prRvInfo->rDram.pu1FifoEnd - prRvInfo->rDram.pu1VFifo));
#endif

#if defined(VDEC_CRC_TEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT)
#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)
   if(VDEC_IsTestCRC(ucEsId))
#endif
    {
        IO_WRITE32(IO_VIRT + 0x3000, 4, 1);
    }
#endif

#ifdef RV_TRACKING
    if (prPic->ePtype != RV_TRUEBPIC)
    {
        // ref-frame, do nothing
    }
    else
    {
#ifdef CC_RV_EMULATION
        extern BOOL _fgRvTracking;
        
        if (!_fgRvTracking)
        {
            // tracking off, do nothing
        }
        else 
#endif
        if (prOther->u1TrackFbId == prOther->u1FbId2)
        {
            LOG(2, "[%d]rvtrack\n", prOther->u4CurrPicNo);
            vWriteREG(VLD_TOP_BASE + RW_VLD_TOP_TRACKING_MODE, 
                u4ReadREG(VLD_TOP_BASE + RW_VLD_TOP_TRACKING_MODE) | ENABLE_TRACKING);
        }
        else
        {
            prOther->u1TrackFbId = prOther->u1FbId2;
        }
    }
#endif

    // trigger
    vWriteREG(RV_VLD_REG_OFST + 42*4, 0x1);

    return TRUE;
}


//#define DUMP_RV_REG  // dump rv register
#ifdef DUMP_RV_REG

#define dReadRVVLD(add) u4ReadREG(RV_VLD_REG_OFST + (add))
#define dReadAVCMV(add) u4ReadREG(AVC_MV_REG_OFST + (add))
#define dReadMC(add) u4ReadREG(MC_REG_OFST + (add))
#define dReadRVPP(add) u4ReadREG(RV_PP_REG_OFST + (add))
#define dReadVLD(add) u4ReadREG(VLD_REG_OFST + (add))

void vDumpVldCkSum(void)
{
    Printf("RV VLD 33: 0x%08x\n", dReadRVVLD(33 * 4));
    Printf("RV VLD 34: 0x%08x\n", dReadRVVLD(34 * 4));
    Printf("RV VLD 35: 0x%08x\n", dReadRVVLD(35 * 4));
    Printf("RV VLD 36: 0x%08x\n", dReadRVVLD(36 * 4));
    Printf("RV VLD 37: 0x%08x\n", dReadRVVLD(37 * 4));
    Printf("RV VLD 43: 0x%08x\n", dReadRVVLD(43 * 4));
    Printf("RV VLD 44: 0x%08x\n", dReadRVVLD(44 * 4));
    Printf("RV VLD 48: 0x%08x\n", dReadRVVLD(48 * 4));
    Printf("RV VLD 59: 0x%08x\n", dReadRVVLD(59 * 4));
    Printf("RV VLD 60: 0x%08x\n", dReadRVVLD(60 * 4));
    Printf("RV VLD 67: 0x%08x\n", dReadRVVLD(67 * 4));
    Printf("RV VLD 63: 0x%08x\n", dReadRVVLD(63 * 4));
    Printf("RV VLD 64: 0x%08x\n", dReadRVVLD(64 * 4));
    Printf("RV VLD 65: 0x%08x\n", dReadRVVLD(65 * 4));
    Printf("RV VLD 196: 0x%08x\n", dReadRVVLD(196 * 4));
}

void vDumpAvcMvCkSum(void)
{
    Printf("AVC MV 148: 0x%08x\n", dReadAVCMV(148 * 4));
    Printf("AVC MV 149: 0x%08x\n", dReadAVCMV(149 * 4));
    Printf("AVC MV 150: 0x%08x\n", dReadAVCMV(150 * 4));
    Printf("AVC MV 151: 0x%08x\n", dReadAVCMV(151 * 4));
}

void vDumpMcCkSum(void)
{
    Printf("MC 378: 0x%08x\n", dReadMC(378 * 4));
    Printf("MC 379: 0x%08x\n", dReadMC(379 * 4));
    Printf("MC 380: 0x%08x\n", dReadMC(380 * 4));
    Printf("MC 381: 0x%08x\n", dReadMC(381 * 4));
    Printf("MC 382: 0x%08x\n", dReadMC(382 * 4));
    Printf("MC 388: 0x%08x\n", dReadMC(388 * 4));
    Printf("MC 390: 0x%08x\n", dReadMC(390 * 4));
    Printf("MC 391: 0x%08x\n", dReadMC(391 * 4));
    Printf("MC 392: 0x%08x\n", dReadMC(392 * 4));
    Printf("MC 393: 0x%08x\n", dReadMC(393 * 4));
    Printf("MC 446: 0x%08x\n", dReadMC(446 * 4));
    Printf("MC 447: 0x%08x\n", dReadMC(447 * 4));
}

void vDumpPpCkSum(void)
{
    int i;

    Printf("PP   5: 0x%08x\n", dReadRVPP(  5 * 4));
    Printf("PP   6: 0x%08x\n", dReadRVPP(  6 * 4));
    Printf("PP  67: 0x%08x\n", dReadRVPP( 67 * 4));

    Printf("PP 69 - 76");
    for (i = 69; i <= 76; i++)
        Printf("0x%08x\n", dReadRVPP(i * 4));

    Printf("PP 161 - 178");
    for (i = 161; i <= 178; i++)
        Printf("0x%08x\n", dReadRVPP(i * 4));

    Printf("PP  67: 0x%08x\n", dReadRVPP( 67 * 4));
    Printf("PP 199: 0x%08x\n", dReadRVPP(199 * 4));
    Printf("PP 200: 0x%08x\n", dReadRVPP(200 * 4));
    Printf("PP 201: 0x%08x\n", dReadRVPP(201 * 4));
    Printf("PP 227: 0x%08x\n", dReadRVPP(227 * 4));

}

void vDumpRegForRm(void)
{
    Printf("AVC_MV 131: 0x%08x\n", dReadAVCMV(131 * 4));
    Printf("AVC_MV 134: 0x%08x\n", dReadAVCMV(134 * 4));
    Printf("AVC_MV 135: 0x%08x\n", dReadAVCMV(135 * 4));

    Printf("MC 136: 0x%08x\n", dReadMC(136 * 4));
    Printf("MC 137: 0x%08x\n", dReadMC(137 * 4));
    Printf("MC 138: 0x%08x\n", dReadMC(138 * 4));
    Printf("MC 139: 0x%08x\n", dReadMC(139 * 4));
    Printf("MC 142: 0x%08x\n", dReadMC(142 * 4));
    Printf("MC 148: 0x%08x\n", dReadMC(148 * 4));
    Printf("MC 152: 0x%08x\n", dReadMC(152 * 4));
    Printf("MC 153: 0x%08x\n", dReadMC(153 * 4));
    Printf("PP 2: 0x%08x\n", dReadRVPP(2 * 4));

    Printf("MC 420: 0x%08x\n", dReadMC(420 * 4));
    Printf("MC 130: 0x%08x\n", dReadMC(130 * 4));
    Printf("MC 131: 0x%08x\n", dReadMC(131 * 4));
    Printf("VLD 36: 0x%08x\n", dReadVLD(36 * 4));
    Printf("VLD 50: 0x%08x\n", dReadVLD(50 * 4));
    Printf("MC 0: 0x%08x\n", dReadMC(0 * 4));
    Printf("MC 1: 0x%08x\n", dReadMC(1 * 4));
    Printf("MC 2: 0x%08x\n", dReadMC(2 * 4));
    Printf("MC 3: 0x%08x\n", dReadMC(3 * 4));
    Printf("MC 39: 0x%08x\n", dReadMC(39 * 4));
    Printf("MC 40: 0x%08x\n", dReadMC(40 * 4));
    Printf("MC 9: 0x%08x\n", dReadMC(9 * 4));
    Printf("MC 28: 0x%08x\n", dReadMC(28 * 4));
    Printf("MC 36: 0x%08x\n", dReadMC(36 * 4));
    Printf("MC 6: 0x%08x\n", dReadMC(6 * 4));
    Printf("MC 38: 0x%08x\n", dReadMC(38 * 4));
    Printf("MC 114: 0x%08x\n", dReadMC(114 * 4));

    Printf("RV VLD 39: 0x%08x\n", dReadRVVLD(39 * 4));
    Printf("RV VLD 66: 0x%08x\n", dReadRVVLD(66 * 4));
    Printf("RV VLD 33: 0x%08x\n", dReadRVVLD(33 * 4));
    Printf("RV VLD 34: 0x%08x\n", dReadRVVLD(34 * 4));
    Printf("RV VLD 35: 0x%08x\n", dReadRVVLD(35 * 4));
    Printf("RV VLD 36: 0x%08x\n", dReadRVVLD(36 * 4));
    Printf("RV VLD 37: 0x%08x\n", dReadRVVLD(37 * 4));

    Printf("RV VLD 42: 0x%08x\n", dReadRVVLD(42 * 4));
    Printf("RV VLD window: 0x%08x\n", _u4VldWindow);
}

void vDumpRmChkSum(void)
{
    Printf("vDumpRmChkSum\n");
    vDumpVldCkSum();
    vDumpAvcMvCkSum();
    vDumpMcCkSum();
    vDumpPpCkSum();
    vDumpRegForRm();
    ASSERT(0);
}

UINT32 _u4DumpNo = (UINT32) -1;
#endif


BOOL fgVDecodeEndRV(UCHAR ucEsId)
{
    VDEC_RV_INFO_T *prRvInfo;
    RvPicAttr *prPic;
    RVOther *prOther;
    UINT32 u4Wait;
    BOOL fgDecTimeout = FALSE;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (ucEsId >= VDEC_MAX_ES)
    {
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);

    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prRvInfo);
    prPic = &prRvInfo->rRvPicAttr;
    prOther = &prRvInfo->rOther;

// wait for complete
#ifndef RV_IRQ_DISABLE
    VERIFY(x_sema_lock(_hVdecRvFinSema, X_SEMA_OPTION_WAIT) == OSR_OK);
#else
    u4Wait = 0;
    while (u4ReadREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_FLAG) == 0)
    {
        if (++u4Wait > DEC_WAIT_CNT)
        {
            fgDecTimeout = TRUE;
            break;
        }
    }
    vWriteREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_CLR, 1);
    vWriteREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_CLR, 0);
#endif

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&_rTimeHwE);

    _rTimeHwDt.u4Micros = 0;
    HAL_GetDeltaTime(&_rTimeHwDt, &_rTimeHwS, &_rTimeHwE);
#endif

#if 1 // TODO: no need to check for new IC, confirm with designer later
    u4Wait = 0;
    while (u4ReadREG(RV_VLD_REG_OFST + RW_RV_VLD_PIC_FINISH) == 0)
    {
        if (++u4Wait > DEC_WAIT_CNT)
        {
            fgDecTimeout = TRUE;
            break;
        }
    }
#endif

#ifdef VDEC_TIME_PROFILE
    //save decode time
    prOther->u4CurDecMs += (_rTimeHwDt.u4Micros / 1000);
//    LOG(5, "dec time = %d\n", _rTimeHwDt.u4Micros);
    u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
    u4DecCycle = IO_REG32(MC0_BASE, 0x778);
    prOther->u4CurDecCycle += u4DecCycle;
    prOther->u4CurDramCycle +=  u4DramCycle;
#endif

#if 0
  if (!fgDecTimeout)
  {
    UINT32 u4CycleCnt;

#if defined(VDEC_IS_POST_MT5368)
    u4CycleCnt = u4ReadREG(VLD_TOP_BASE + 40 * 4);
#else
    u4CycleCnt = u4ReadREG(RV_VLD_REG_OFST + 50 * 4);
#endif
    u4CycleCnt /= ((prPic->u4Width / 16) * (prPic->u4Height / 16));
    if (u4CycleCnt > _u4MaxCycleCnt)
    {
      _u4MaxCycleCnt = u4CycleCnt;
      _u4MaxCycleCntPicNum = n;
    }
  }
#endif

#ifdef RPR_RACING
  if (_eRprRacStt == RPR_RAC_SCP1_PENDING)
  {
    u4Wait = 0;
    while (u4ReadREG(IMG_RESZ_BASE + IMG_RESZ_DONE * 4) == 0)
    {
      if (++u4Wait > RPR_RAC_WAIT_CNT)
      {
        fgDecTimeout = TRUE;
        break;
      }
    }
    if (!fgDecTimeout)
      _eRprRacStt = RPR_RAC_SCP1_DONE;
  }
#endif

#ifdef RV_MESAURE_DEC_TIME
  HAL_GetTime(&_rEndTime);
#endif

  // save ref_qp_mv0 for the next picture
  if (prPic->ePtype != RV_TRUEBPIC && !fgDecTimeout)
    VDEC_MULTI_USE(_u4RefQpMb0) = u4ReadREG(RV_PP_REG_OFST + 7 * 4);

    if (fgDecTimeout)
    {
        vResetRVVDec(ucEsId); // reset hw when error occur
        prOther->u4TimeOutCnt++;
        prOther->u4LastTimeOutPicNo = prOther->u4CurrPicNo;
        LOG(0, "RV(%d) Decode %d timeout!!!!!!!!!!!!!!!!!!!!!!!!!!\n", ucEsId, prOther->u4CurrPicNo);
#ifdef DUMP_RV_REG
        Printf("RV(%d) Decode %d timeout!!!!!!!!!!!!!!!!!!!!!!!!!!\n", ucEsId, prOther->u4CurrPicNo);      
        vDumpRmChkSum();
#endif
    }
    else
    {
#ifdef DUMP_RV_REG
        if (prOther->u4CurrPicNo == _u4DumpNo)
        {
            vDumpRmChkSum();
        }
#endif
        u4Wait = u4ReadREG(RV_VLD_REG_OFST + 0xC0);
        if (u4Wait)
        {
            vResetRVVDec(ucEsId); // reset hw when error occur
            prOther->u4DecErrCnt++;
            prOther->u4LastDecErrPicNo = prOther->u4CurrPicNo;
            LOG(0, "RV(%d) Decode %d error: 0x%08X!!!!!!!!!!!!!!!!!!!!!!!!!!\n", ucEsId, prOther->u4CurrPicNo, u4Wait);
#ifdef DUMP_RV_REG
            Printf("RV(%d) Decode %d error: 0x%08X!!!!!!!!!!!!!!!!!!!!!!!!!!\n", ucEsId, prOther->u4CurrPicNo, u4Wait);
            vDumpRmChkSum();
#endif
        }
        else
        {
#ifdef RV_MESAURE_DEC_TIME
            DecTimeStat(prRvInfo);
#endif
        }
    }

#if defined(VDEC_CRC_TEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT)
#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT)
    if(VDEC_IsTestCRC(ucEsId))
#endif
    {
        UINT32 u4Ycrc[4] = {0};
        UINT32 u4Ccrc[4] = {0};
        UINT32 i;
           
        for(i=0;i<4;i++)
        {
           u4Ycrc[i] = IO_READ32(IO_VIRT + 0x3000,(2+i)*4);
           u4Ccrc[i] = IO_READ32(IO_VIRT + 0x3018,i*4);
        }
        for(i=0; i<4;i++)
        {
            prVdecEsInfo->u4CrcResult ^= u4Ycrc[i];
            prVdecEsInfo->u4CrcResult ^= u4Ccrc[i];
        }
    }
#endif
  
#ifdef __MODEL_slt__
  if (prRvInfo->rPesInfo.u4PTS == 96030)
  {
      UINT32 *pu4Ptr;
      UINT32 u4CksumY, u4CksumC, i;
      
      pu4Ptr = (UINT32 *)VIRTUAL(prRvInfo->rDram.pu1Pic2Y);
      for (i = 0, u4CksumY = 0; i < 640 * 480 / 4; i++)
          u4CksumY ^= *pu4Ptr++;
      Printf("RM SLT Y cksum: 0x%08X\n", u4CksumY);
      
      pu4Ptr = (UINT32 *)VIRTUAL(prRvInfo->rDram.pu1Pic2C);
      for (i = 0, u4CksumC = 0; i < 640 * 480 / 8; i++)
          u4CksumC ^= *pu4Ptr++;
      Printf("RM SLT C cksum: 0x%08X\n", u4CksumC);

      if (u4CksumY == 0x86395C45 && u4CksumC == 0x2E2FCAEF)
          Printf("MMOK!!\n");
      else
          Printf("MMNG!!\n");
  }
#endif

//    Printf("fgVDecodeEndRV(%d, %d): 0x%08x\n", ucEsId, prOther->u4CurrPicNo, prOther->u4LastVldReadPtr);

  _VDEC_UnlockVld(ucEsId, prVdecEsInfo->u4VldId);

  return (!fgDecTimeout);
}


/** Initialize module for rv
 */
void _RVInit(UCHAR ucEsId)
{      
    vInitRVVDec(ucEsId);
    vInitRVVld(ucEsId);
    vInitRVPred(ucEsId);
    #ifdef RV_MESAURE_DEC_TIME
    DecTimeInit();
    #endif

#ifndef RV_IRQ_DISABLE
    if (!_hVdecRvFinSema)
    {
        VERIFY(x_sema_create(&_hVdecRvFinSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    }
#endif
}

void _RVHdrPrs(UCHAR ucEsId, INT32* pi4RetValue)
{
    if (fgVParserProcRV(ucEsId, pi4RetValue))
    {
        //*pi4RetValue = E_VDEC_OK;
    }
    else
    {
        #ifdef USE_FBM
        VDEC_RV_INFO_T *prRvInfo;
        _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prRvInfo);
        FBM_ReleaseGroup(prRvInfo->rOther.u1FbgId);
        #endif
        *pi4RetValue = E_VDEC_BS_END;
    }
}

/** rv decoder hardware config and decoding trigger
*@param hVdec handler
 */
void _RVDecStart(UCHAR ucEsId, INT32* pi4RetValue)
{
    //LOG(6,"Wait to Dec, no: %d, type: %d\n", prVdec->rOther.u4CurrPicNo, prVdec->rPicLayer.u1PicType);
    if(fgVDecProcRV(ucEsId))
    {
        *pi4RetValue = E_VDEC_OK;
    }
    else 
    {
        *pi4RetValue = E_VDEC_FAIL;
    }
}


/** rv wait decoding finish, display module setting / decoded data checking
*@param hVdec handler
 */
void _RVDecFin(UCHAR ucEsId, INT32* pi4RetValue)
{
  VDEC_RV_INFO_T *prRvInfo;
  _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prRvInfo);

  if (fgVDecodeEndRV(ucEsId))
  {
#ifdef RV_TRACKING
        if (prRvInfo->rRvPicAttr.ePtype != RV_TRUEBPIC)
#endif
        {
            vSetRVVSyncPrmBufPtr(ucEsId, prRvInfo->rDram.u4DecBufIdx);
        }
        prRvInfo->rOther.u4CurrPicNo++;
        *pi4RetValue = E_VDEC_OK;
  }
  else
  {
#if 1
    *pi4RetValue = E_VDEC_OK;
    vSetRVVSyncPrmBufPtr(ucEsId, prRvInfo->rDram.u4DecBufIdx);
#else
    *pi4RetValue = E_VDEC_FAIL;
#endif
  }
}

/** rv SW reset
*@param hVdec handler
 */
void _RVSWRst(UCHAR ucEsId, INT32* pi4RetValue)
{
    vResetRVVDec(ucEsId);
    *pi4RetValue = E_VDEC_OK;
}

/** rv decode stop / freeze
*@param hVdec handler
 */
void _RVStop(VDEC_HDLR hVdec)
{
}

/** get pes struct pointer
*@param .
 */
void _RVGetPesStruct(VDEC_HDLR hVdec, void **ppvPesInfo)
{
}

/** get display picture addr
*@param .
 */
void _RVGetDispPic(VDEC_HDLR hVdec, void* pvInfo)
{
}


// *** RV End ***
