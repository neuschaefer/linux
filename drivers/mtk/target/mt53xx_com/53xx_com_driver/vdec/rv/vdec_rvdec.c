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

#ifdef USE_FBM
#include "fbm_drvif.h"
#endif

#define VDEC_MM_WAIT_DISP_TIME 1000

#ifdef CC_MT5395
externC void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
#else
extern void HalFlushInvalidateDCache(void);
#endif

#ifndef AVC_IRQ_DISABLE
#include "x_os.h"
//static HANDLE_T rWMVDecInfo.rOther.hVdecFinSema = NULL;
#endif

#define DEC_WAIT_CNT          300000  // time to wait for decode finish

#if 1  // RV_ERROR_CONCEALMENT
static UINT32 _u4RvErrConceal = (2 << 24) | (1 << 27);  // enable error conceal, 0x0A000000
#else
static UINT32 _u4RvErrConceal = 0;
#endif
static UINT32 _u4FastParsing = 0x40000000;
static UINT32 _u4CycleCnt = 0x00800000;
static UINT32 _u4Rv8TblNum = 1;

static UINT32 _u4RefQpMb0 = 0;
UINT32 _u4VldWindow = 0;
static BOOL _fgDecTimeout = 0;

static UINT32 _u4BwdIsI;


#ifndef RV_IRQ_DISABLE
void _RVVLDIsr(UINT16 u2Vector)
{
    LOG(6,"----------------------entering RVVLDISR\n\n");
    if (u2Vector == RVVLD_VECTOR)
    {
    }
}
#endif


static void SetRVPicHdr(UCHAR u1FbId)
{
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    FBM_SEQ_HDR_T *prFbmSeqHdr = NULL;

    #if 0
    BOOL fgNotDisplay = FALSE;
    #endif

    prFbmPicHdr = FBM_GetFrameBufferPicHdr(_rRVDecInfo.rOther.u1FbgId, u1FbId);
    prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_rRVDecInfo.rOther.u1FbgId);
    if (prFbmPicHdr == NULL || prFbmSeqHdr == NULL)
    {
        ASSERT(0);
        return;
    }
    //UINT8 u1RepeatFrameCount;  //(RPTFRM) need to add in
    prFbmPicHdr->fgProgressiveFrm = TRUE;
    prFbmPicHdr->fgRepFirstFld = FALSE;  //_rRVDecInfo.rPicLayer.fgRepeatFirstField;  !!!
    prFbmPicHdr->fgTopFldFirst = TRUE;  //_rRVDecInfo.rPicLayer.fgTopFieldFirst;  !!!
    //prFbmPicHdr->ucPicStruct = _rRVDecInfo.rPicLayer.
    prFbmPicHdr->u4PTS = _rRVDecInfo.rPesInfo.u4PTS;

    prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;

    prFbmPicHdr->u8OffsetI = _rRVDecInfo.rPesInfo.u8OffsetI;
    prFbmPicHdr->u8Offset = _rRVDecInfo.rPesInfo.u8Offset;

    #if 0
    fgNotDisplay = VDEC_IsNotDisplay(0, _rRVDecInfo.rPesInfo.u4PTS, 0, 0);
    if (fgNotDisplay)
    {
        FBM_SetFrameBufferPicFlag(_rRVDecInfo.rOther.u1FbgId,
            u1FbId,
            FBM_MM_NOT_DISPLAY_FLAG);

        LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", u1FbId);
    }
    #endif

    /*if(_rRVDecInfo.rPicLayer.u1PicType== IVOP)
        LOG(0,"I frame , PTS valuse : %x\n",_rRVDecInfo.rPesInfo.u4PTS);
    else if(_rRVDecInfo.rPicLayer.u1PicType== PVOP)
        LOG(0,"P frame , PTS valuse : %x\n",_rRVDecInfo.rPesInfo.u4PTS);
    */
//#ifdef CC_RESIZE_SMALL_IMAGE
    prFbmPicHdr->u4PicWidth = _rRVDecInfo.rRvPicAttr.u4OrgWidth;
    prFbmPicHdr->u4PicHeight = _rRVDecInfo.rRvPicAttr.u4OrgHeight;
    prFbmPicHdr->u4PicWidthPitch = _rRVDecInfo.rRvPicAttr.u4LineSize;
//#endif

    if (!(prFbmSeqHdr->fgResizeSmallPic) &&
       (prFbmSeqHdr->u2OrgHSize != prFbmPicHdr->u4PicWidth ||
        prFbmSeqHdr->u2OrgVSize != prFbmPicHdr->u4PicHeight)
    )
    {
        prFbmPicHdr->fgSeamlessDisp = TRUE;
    }

}

static void SetRVSeqHdr(void)
{
    if(_rRVDecInfo.rOther.u1GetSeqHdr == 0)
    {
        FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
        UINT32 u4fps;
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(_rRVDecInfo.rOther.u1FbgId);
        if (prFbmSeqHdr == NULL)
        {
            ASSERT(0);
            return;
        }

        prFbmSeqHdr->ucAspRatInf = 3;//_rRVDecInfo.rSeqLayer.i4Aspect_Ratio;//;        // 16:9
        //prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
        prFbmSeqHdr->fgProgressiveSeq = 1;//!_rRVDecInfo.rSeqLayer.fgInterlacedSource;//1;//(_rRVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE);

        //LOG(0,"frame rate : %x\n",_rRVDecInfo.rSeqLayer.i4FrameRate);

        u4fps = 30;  //_rRVDecInfo.rSeqLayer.i4FrameRate;
        _rRVDecInfo.rOther.u4FrameRate = u4fps;
        _rRVDecInfo.rOther.u4DeltaPTS = 90000 / u4fps;
        LOG(0,"u4fps : %d\n",u4fps);

        switch(u4fps)
        {
            case 22:
            case 23:
               prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24_;
            break;
            case 24:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_24;
            break;
            case 25:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_25;
            break;
            case 30:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
            break;
            case 29:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30_;
            break;
            case 50:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_50;
                break;
            case 60:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60;
                break;
            case 59:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_60_;
                break;
            default:
                prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_UNKNOWN;
            break;
        }

        //prFbmSeqHdr->ucAspRatInf = _rRVDecInfo.rSeqLayer.i4Aspect_Ratio; // 16:9
        prFbmSeqHdr->fgProgressiveSeq = TRUE;
        //LOG(0,"prFbmSeqHdr->fgProgressiveSeq : %d\n",prFbmSeqHdr->fgProgressiveSeq);

#ifdef CC_RESIZE_SMALL_IMAGE
        if (_rRVDecInfo.rRvPicAttr.u4OrgWidth < FBM_IMG_RESIZE_LIMITATION ||
            _rRVDecInfo.rRvPicAttr.u4OrgHeight < FBM_IMG_RESIZE_LIMITATION)
        {
            prFbmSeqHdr->fgResizeSmallPic = TRUE;

            if (_rRVDecInfo.rRvPicAttr.u4OrgWidth < FBM_IMG_RESIZE_LIMITATION)
            {
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            }
            else if (_rRVDecInfo.rRvPicAttr.u4OrgWidth > FBM_IMG_RESIZE_LIMITATION_MAX)
            {
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            }
            else
            {
                prFbmSeqHdr->u2LineSize = _rRVDecInfo.rRvPicAttr.u4LineSize;
                prFbmSeqHdr->u2HSize = _rRVDecInfo.rRvPicAttr.u4OrgWidth;
                prFbmSeqHdr->u2OrgHSize = _rRVDecInfo.rRvPicAttr.u4OrgWidth;
            }

            if (_rRVDecInfo.rRvPicAttr.u4OrgHeight < FBM_IMG_RESIZE_LIMITATION)
            {
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
            }
            else if (_rRVDecInfo.rRvPicAttr.u4OrgHeight > FBM_IMG_RESIZE_LIMITATION_MAX)
            {
                //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
            }
            else
            {
                prFbmSeqHdr->u2VSize = _rRVDecInfo.rRvPicAttr.u4OrgHeight;
                prFbmSeqHdr->u2OrgVSize = _rRVDecInfo.rRvPicAttr.u4OrgHeight;
            }
        }
        else
        {
            prFbmSeqHdr->u2LineSize = _rRVDecInfo.rRvPicAttr.u4LineSize;      // rSeqHdrR.u4PicWidthDec;
            prFbmSeqHdr->u2HSize = _rRVDecInfo.rRvPicAttr.u4OrgWidth;      // rSeqHdrR.u4PicWidthSrc;
            prFbmSeqHdr->u2VSize = _rRVDecInfo.rRvPicAttr.u4OrgHeight;     // rSeqHdrR.u4PicHeightSrc;
            prFbmSeqHdr->u2OrgHSize = _rRVDecInfo.rRvPicAttr.u4OrgWidth;   // rSeqHdrR.u4PicWidthSrc;
            prFbmSeqHdr->u2OrgVSize = _rRVDecInfo.rRvPicAttr.u4OrgHeight;  // rSeqHdrR.u4PicHeightSrc;
        }
#else
        prFbmSeqHdr->u2LineSize = _rRVDecInfo.rRvPicAttr.u4LineSize;      // rSeqHdrR.u4PicWidthDec;
        prFbmSeqHdr->u2HSize = _rRVDecInfo.rRvPicAttr.u4OrgWidth;      // rSeqHdrR.u4PicWidthSrc;
        prFbmSeqHdr->u2VSize = _rRVDecInfo.rRvPicAttr.u4OrgHeight;     // rSeqHdrR.u4PicHeightSrc;
        prFbmSeqHdr->u2OrgHSize = _rRVDecInfo.rRvPicAttr.u4OrgWidth;   // rSeqHdrR.u4PicWidthSrc;
        prFbmSeqHdr->u2OrgVSize = _rRVDecInfo.rRvPicAttr.u4OrgHeight;  // rSeqHdrR.u4PicHeightSrc;
#endif

        prFbmSeqHdr->u4ResizeWidth = prFbmSeqHdr->u2OrgHSize;
        prFbmSeqHdr->u4ResizeHeight = VDEC_ALIGN_MASK(prFbmSeqHdr->u2OrgVSize,15);

        _rRVDecInfo.rOther.u1GetSeqHdr = 1;
    }
}


void vRVVPrsIPProc(INT32* pi4RetValue, BOOL fgIsP)
{
    *pi4RetValue = E_VDEC_OK;

    if (_rRVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(_rRVDecInfo.rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
        }
    }

#ifdef REPEAT_FRAME
    if(_rRVDecInfo.rOther.u4repeatframe)
    {
        // do nothing
        return;
    }
#endif

    {
        vSetRVFRefBuf(_rRVDecInfo.rDram.u4BRefBufIdx);
        vSetRVBRefBuf(1 - _rRVDecInfo.rDram.u4FRefBufIdx);
        vSetRVDecBuf(_rRVDecInfo.rDram.u4BRefBufIdx);
    }

    #ifdef USE_FBM
    {
        UINT32 u4Yaddr, u4Caddr;
        SetRVSeqHdr();

        UCHAR ucBRefFbId;
        UCHAR ucFRefFbId;

            FBM_GetRefFrameBuffer(_rRVDecInfo.rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
            if(FBM_FB_ID_UNKNOWN != ucBRefFbId)
            {
                VDEC_ES_INFO_T* prVdecEsInfo;
                FBM_PIC_HDR_T *prFbmPicHdr = NULL;
                BOOL fgNotDisplay;

                prVdecEsInfo = _VDEC_GetEsInfo(0);

                //set first frame pts to audio
                #if 0
                if ((!prVdecEsInfo->fgSetStartPts) && (prVdecEsInfo->ucSyncMode != VID_SYNC_MODE_NONE))
                {
                    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, ucBRefFbId);
                        if (STC_SetStartPts(AV_SYNC_TARGET_VDO, prVdecEsInfo->ucStcId, prFbmPicHdr->u4PTS) != STC_VALID)
                        {
                            LOG(3, "STC_SetStartPts Invalid\n");
                        }
                        LOG(5, "STC_SetStartPts 0x%x\n", prFbmPicHdr->u4PTS);

                    prVdecEsInfo->fgSetStartPts = TRUE;
                }
                #endif

                prFbmPicHdr = FBM_GetFrameBufferPicHdr(_rRVDecInfo.rOther.u1FbgId, ucBRefFbId);

                if(prFbmPicHdr)
                {
                    if(_rRVDecInfo.rOther.fgFirstDispFrame)
                    {
                         _rRVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                         _rRVDecInfo.rOther.fgFirstDispFrame = FALSE;
                    }
                    else
                    {
                        if(_rRVDecInfo.rPesInfo.fgDtsValid/* && (prFbmPicHdr->u4PTS >= _rRVDecInfo.rOther.u4LastPTS)*/)
                        {
                            _rRVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                        }
                        else
                        {
                            prFbmPicHdr->u4PTS = _rRVDecInfo.rOther.u4LastPTS + _rRVDecInfo.rOther.u4DeltaPTS;
                            _rRVDecInfo.rOther.u4LastPTS += _rRVDecInfo.rOther.u4DeltaPTS;
                        }
                    }

                    fgNotDisplay = VDEC_IsNotDisplay(0, prFbmPicHdr->u4PTS, 0, 0);
                }
                else
                {
                    fgNotDisplay = VDEC_IsNotDisplay(0, 0, 0, 0);
                }
                if (fgNotDisplay)
                {
                    #ifdef RV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId,ucBRefFbId);
                    LOG(0,"WMV set I/P frame FBM status : %d %d EMPTY\n",ucBRefFbId,u1state);
                    #endif
                    FBM_SetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {
                    #ifdef RV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId,ucBRefFbId);
                    LOG(0,"WMV set I/P frame FBM status : %d %d DISPALY_Q\n",ucBRefFbId,u1state);
                    #endif
                    FBM_SetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;
                    if(prFbmPicHdr)
                    {
                        VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, 0);
                    }
                    else
                    {
                        VDEC_CheckStopStatus(0, 0, 0);
                    }
                }

                // RPR check begin in other file
                if (fgIsP && prFbmPicHdr)
                {
                    //check ref resolution
                    if ((prFbmPicHdr->u4PicWidth != _rRVDecInfo.rRvPicAttr.u4OrgWidth)
                        ||(prFbmPicHdr->u4PicHeight != _rRVDecInfo.rRvPicAttr.u4OrgHeight)
                    )
                    {
                        //BOOL fgCheck = FALSE;
                        UINT32 u4BRefYaddr, u4BRefCaddr;
                        UINT32 u4OriAddrY, u4OriAddrC;
                        RZ_VDO_SCL_PARAM_SET_T rScaleParamV;
                        do
                        {
                            //get buffer first
                            FBM_GetExtraFrameBuffer(_rRVDecInfo.rOther.u1FbgId,&u4BRefYaddr,&u4BRefCaddr);
                            if (!u4BRefYaddr || !u4BRefCaddr)
                            {
                                LOG(0, "RPR, get buffer fail\n");
                                break;
                            }

                            FBM_GetFrameBufferAddr(_rRVDecInfo.rOther.u1FbgId, ucBRefFbId,&u4OriAddrY, &u4OriAddrC);

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
                            rScaleParamV.u4YTgW = _rRVDecInfo.rRvPicAttr.u4OrgWidth;
                            rScaleParamV.u4YTgH = _rRVDecInfo.rRvPicAttr.u4OrgHeight;
                            rScaleParamV.u4YTgBufLen= _rRVDecInfo.rRvPicAttr.u4OrgWidth;
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
                            if (_rRVDecInfo.rDram.u4FRefBufIdx == 0 && _rRVDecInfo.rDram.u4BRefBufIdx == 1)
                            {
                                _rRVDecInfo.rDram.pu1Pic0Y = (UINT8*)u4BRefYaddr;
                                _rRVDecInfo.rDram.pu1Pic0C = (UINT8*)u4BRefCaddr;
                            }
                            else if (_rRVDecInfo.rDram.u4FRefBufIdx == 1 && _rRVDecInfo.rDram.u4BRefBufIdx == 0)
                            {
                                _rRVDecInfo.rDram.pu1Pic1Y = (UINT8*)u4BRefYaddr;
                                _rRVDecInfo.rDram.pu1Pic1C = (UINT8*)u4BRefCaddr;
                            }

                            //fgCheck = TRUE;
                        }while (0);
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

            }
            else if (fgIsP)
            {
                LOG(3, "Wrong reference for P PIC\n");
                *pi4RetValue = E_VDEC_FAIL;
                return;
            }

            UCHAR ucFbId = FBM_GetEmptyRefFrameBuffer(_rRVDecInfo.rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
            ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);
            FBM_GetFrameBufferAddr(_rRVDecInfo.rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

            ASSERT(u4Yaddr !=0 && u4Caddr !=0);
            // reset mem here
            SetRVPicHdr(ucFbId);

#if 1
            _rRVDecInfo.rDram.pu1Pic2Y = (UINT8*)u4Yaddr;
            _rRVDecInfo.rDram.pu1Pic2C = (UINT8*)u4Caddr;
            if (_rRVDecInfo.rDram.u4DecBufIdx == 0) //pic1
            {
                _rRVDecInfo.rOther.u1FbId0 = ucFbId;
            }
            else //pic2
              {
                _rRVDecInfo.rOther.u1FbId1 = ucFbId;
              }
            if (_rRVDecInfo.rDram.u4FRefBufIdx == 0 && _rRVDecInfo.rDram.u4BRefBufIdx == 1)
            {
                _rRVDecInfo.rDram.pu1Pic1Y = (UINT8*)u4Yaddr;
                _rRVDecInfo.rDram.pu1Pic1C = (UINT8*)u4Caddr;
            }
            else if (_rRVDecInfo.rDram.u4FRefBufIdx == 1 && _rRVDecInfo.rDram.u4BRefBufIdx == 0)
              {
                _rRVDecInfo.rDram.pu1Pic0Y = (UINT8*)u4Yaddr;
                _rRVDecInfo.rDram.pu1Pic0C = (UINT8*)u4Caddr;
              }
            else
            {
                ASSERT(0);
            }
#else
            if(_rRVDecInfo.rDram.u4DecBufIdx == 0) //pic1
            {
                _rRVDecInfo.rDram.pu1Pic0Y = (UINT8*)u4Yaddr;
                _rRVDecInfo.rDram.pu1Pic0C = (UINT8*)u4Caddr;
                _rRVDecInfo.rDram.pu1Pic2Y = (UINT8*)u4Yaddr;  // !!!
                _rRVDecInfo.rDram.pu1Pic2C = (UINT8*)u4Caddr;  // !!!
                _rRVDecInfo.rOther.u1FbId0 = ucFbId;
            }
            else //pic2
              {
                _rRVDecInfo.rDram.pu1Pic1Y = (UINT8*)u4Yaddr;
                _rRVDecInfo.rDram.pu1Pic1C = (UINT8*)u4Caddr;
                _rRVDecInfo.rDram.pu1Pic2Y = (UINT8*)u4Yaddr;  // !!!
                _rRVDecInfo.rDram.pu1Pic2C = (UINT8*)u4Caddr;  // !!!
                _rRVDecInfo.rOther.u1FbId1 = ucFbId;
              }
#endif
              FBM_SetFrameBufferStatus(_rRVDecInfo.rOther.u1FbgId,ucFbId,FBM_FB_STATUS_DECODE);

            vSetRVMcBufPtr();
    }
    #endif
}


void vRVVPrsBProc(INT32* pi4RetValue)
{
    *pi4RetValue = E_VDEC_OK;

    if (_rRVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(_rRVDecInfo.rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
        }
    }
    vSetRVDecBuf(2);
    //vSaveFrmBufPrm(_rRVDecInfo.rDram.u4DecBufIdx);

    #ifdef USE_FBM
    {
        UINT32 u4Yaddr, u4Caddr;
        UCHAR ucBRefFbId;
        UCHAR ucFRefFbId;

        SetRVSeqHdr();

        FBM_GetRefFrameBuffer(_rRVDecInfo.rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
        if(FBM_FB_ID_UNKNOWN == ucBRefFbId || FBM_FB_ID_UNKNOWN == ucFRefFbId)
        {
            LOG(3, "Wrong reference for B PIC\n");
            *pi4RetValue = E_VDEC_FAIL;
            return;
        }

        UCHAR ucFbId = FBM_GetEmptyBFrameBuffer(_rRVDecInfo.rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
        ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);

        FBM_GetFrameBufferAddr(_rRVDecInfo.rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

        ASSERT(u4Yaddr !=0 && u4Caddr !=0);
        // ilii
        // reset mem here
        SetRVPicHdr(ucFbId);
        //FBM_PIC_HDR_T* prFbmPicHdr;
        //prFbmPicHdr = FBM_GetFrameBufferPicHdr(_rRVDecInfo.rOther.u1FbgId, ucFbId);
        //prFbmPicHdr->fgProgressiveFrm = 1;//(_rRVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE);

      //if((_rRVDecInfo.rPicLayer.u1PicType == BVOP) || (_rRVDecInfo.rPicLayer.u1PicType == BIVOP)) //prog_B
      {
          _rRVDecInfo.rDram.pu1Pic2Y = (UINT8*)u4Yaddr;
          _rRVDecInfo.rDram.pu1Pic2C = (UINT8*)u4Caddr;
          _rRVDecInfo.rOther.u1FbId2 = ucFbId;
      }
      vSetRVMcBufPtr();
    }
    #endif
}


#ifdef CC_RV_EMULATION
static UINT32 u4GetField(UINT32 u4Val, int b0, int b1)
{
    int i;
    UINT32 u4Mask = 0;

    for (i = b0; i <= b1; i++)
      u4Mask |= (1 << i);

    u4Val &= u4Mask;
    u4Val >>= b0;

    return u4Val;
}
#endif

#ifdef CC_RV_EMULATION
extern BOOL _fgRVEmuEnd;
extern UINT32 _u4RVEmuEndPicNum;
#endif

static void vRVInitBarrelShifter(UINT32 u4Addr)
{
  vWriteREG(RV_VLD_REG_OFST + 0x84, 0x1);

 #if 0
  // RPAR: read pointer
  *(volatile UINT32 *)0x200020b0 = u4Addr;  //VFIFO_ADDR;

  *(volatile UINT32 *)0x20002110 = 0xffffffff;               // RCRW: ???
  *(volatile UINT32 *)0x200020b4 = _rRVDecInfo.rPesInfo.u4FifoStart >> 6;               // DSTA: V FIFO start addr
  *(volatile UINT32 *)0x200020b8 = _rRVDecInfo.rPesInfo.u4FifoEnd >> 6;    // DETA: V FIFO end addr
  *(volatile UINT32 *)0x2000208c = 0x00100000;               // RCPR: set initial fetch flag

  while (!((*(volatile UINT32 *)0x200020e8) & 0x00000001));  // wait for FFSR bit 0 raise

  *(volatile UINT32 *)0x2000208c = 0x00800000;  // RCPR: set initial filling of barrel shifter flag

    while((((*(volatile UINT32 *)0x200020ec & 0xf) * 4) +
        (((*(volatile UINT32 *)0x200020ec) >> 24) & 0x3)) !=3);
#else

    vWriteREG(VLD_REG_OFST + 0xb0, u4Addr);
    vWriteREG(VLD_REG_OFST + 0x110, 0xffffffff);
    vWriteREG(VLD_REG_OFST + 0xb4, (_rRVDecInfo.rPesInfo.u4FifoStart >> 6));
    vWriteREG(VLD_REG_OFST + 0xb8, (_rRVDecInfo.rPesInfo.u4FifoEnd >> 6));
    vWriteREG(VLD_REG_OFST + 0x8c, 0x00100000);

    while (!(u4ReadREG(VLD_REG_OFST + 0xe8) & 0x00000001));  // wait for FFSR bit 0 raise

    vWriteREG(VLD_REG_OFST + 0x8c, 0x00800000);

    while((((u4ReadREG(VLD_REG_OFST + 0xec) & 0xf) * 4) +
        ((u4ReadREG(VLD_REG_OFST + 0xec) >> 24) & 0x3)) !=3);


#endif
}

BOOL fgVParserProcRV(INT32* pi4RetValue)
{
    VDEC_RV_INFO_T *prRvInfo;
    RvPicAttr *prPic;
#ifdef CC_RV_EMULATION
    UINT32 *pu4FrminfoPtr;
    UINT32 u4Parm;
    UINT32 i;
#endif

    _VDEC_RVGetWholePesStruct(0, (void **)&prRvInfo);
#ifndef CC_RV_EMULATION
    // normal flow, use barrel shifter to parse parameters
    prPic = &(prRvInfo->rRvPicAttr);

#if 1
    vRVInitBarrelShifter((UINT32)_rRVDecInfo.rDram.pu1VFifo + _rRVDecInfo.rOther.i4SetPos);
#else
    //fgVldInitBarrelShifter((UINT32)_rRVDecInfo.rDram.pu1VFifo + _rRVDecInfo.rOther.i4SetPos);
#endif
    //x_memcpy(&u4Tmp,(void*)(rWMVDecInfo.rDram.pu1VFifo + rWMVDecInfo.rOther.i4SetPos),4);
    LOG(6,"init barrel shifter1, set position : %x\n",_rRVDecInfo.rOther.i4SetPos);
    //LOG(0,"fetch 32bit data : %x\n",u4Tmp);
    /*_rRVDecInfo.rOther.i4VprErr = */RVVideoDecDecodeFrameHead(prPic);
#else
    // emulation flow, read parameters directly from FRMINFO buffer

    pu4FrminfoPtr = (UINT32 *)(prRvInfo->rPesInfo.u4RenderPts + prRvInfo->rOther.u4FrminfoPtr);
    u4Parm = *pu4FrminfoPtr++;
    prRvInfo->rOther.u4FrminfoPtr += 4;

    if (u4Parm == 0xFFFFFFFF)  // hit end
    {
        _fgRVEmuEnd = TRUE;
        return FALSE;
    }

    if (prRvInfo->rOther.u4CurrPicNo >= _u4RVEmuEndPicNum)
    {
        _fgRVEmuEnd = TRUE;
        return FALSE;
    }

    prPic = &(prRvInfo->rRvPicAttr);

    prPic->ePtype = (EnumRVPicCodType)u4GetField(u4Parm, 0, 1);
    prPic->u4OrgWidth = u4GetField(u4Parm, 2, 12);
    prPic->u4OrgHeight = u4GetField(u4Parm, 13, 23);
    prPic->u4Width = (prPic->u4OrgWidth + 15) / 16 * 16;
    prPic->u4Height = (prPic->u4OrgHeight + 15) / 16 * 16;
    prPic->u4PctszSize = u4GetField(u4Parm, 24, 25);
    prPic->u4Pctsz = u4GetField(u4Parm, 26, 28);
    prPic->u4Oquant = u4GetField(u4Parm, 29, 30);
    prPic->u4DFP = u4GetField(u4Parm, 31, 31);

    u4Parm = *pu4FrminfoPtr++;
    prRvInfo->rOther.u4FrminfoPtr += 4;

    prPic->u4Tr = u4GetField(u4Parm, 0, 12);
    prPic->u4Mba = u4GetField(u4Parm, 13, 26);
    prPic->u4Rtype = u4GetField(u4Parm, 27, 27);
    prPic->fgRV9 = u4GetField(u4Parm, 28, 28);

    u4Parm = *pu4FrminfoPtr++;
    prRvInfo->rOther.u4FrminfoPtr += 4;

    prPic->u4NumSlice = u4GetField(u4Parm, 0, 7);
    prPic->u4Pquant = u4GetField(u4Parm, 8, 12);
    prPic->u4MbaSize = u4GetField(u4Parm, 13, 16);
    prPic->u4HdrSkip = u4GetField(u4Parm, 17, 24);

    u4Parm = *pu4FrminfoPtr++;
    prRvInfo->rOther.u4FrminfoPtr += 4;

    prPic->u4Iratio = u4Parm;

    prPic->u4BstLength = 0;
    for (i = 0; i < prPic->u4NumSlice; i++)
    {
        u4Parm = *pu4FrminfoPtr++;
        prRvInfo->rOther.u4FrminfoPtr += 4;
        prPic->au4SliceSize[i] = u4Parm;
        prPic->u4BstLength += u4Parm;
    }
#endif

    // Maximum resolution for RV is 2048x1152
    if (prPic->u4Width > 2048 || prPic->u4Height > 1152)
    {
        *pi4RetValue = E_VDEC_NOT_SUPPORT;
        return TRUE;
    }

    // load frame bitstream
#ifdef CC_RV_EMULATION
    _RVLoadBitstream(prRvInfo->rOther.u4CurrPicNo, prRvInfo->rPesInfo.u4FifoStart);
#endif
    //fgVldInitBarrelShifter(prRvInfo->rPesInfo.u4VldReadPtr);

    switch (prPic->ePtype)
    {
        case RV_INTRAPIC:
        case RV_FORCED_INTRAPIC:
        case RV_INTERPIC:
            if (_rRVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
            {
                // Check if reset is required
                if (FBM_ChkFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
                {
                    FBM_ResetGroup(_rRVDecInfo.rOther.u1FbgId);
                    FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
                    FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
                }
            }
            vRVVPrsIPProc(pi4RetValue, prPic->ePtype == RV_INTERPIC);
            break;
        case RV_TRUEBPIC:
            if (_rRVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
            {
                // Check if reset is required
                if (FBM_ChkFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
                {
                    FBM_ResetGroup(_rRVDecInfo.rOther.u1FbgId);
                    FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
                    FBM_ClrFrameBufferFlag(_rRVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
                }
            }
            vRVVPrsBProc(pi4RetValue);
            break;
    }

    return TRUE;
}


#ifdef CC_RV_EMULATION
void vGetRVInitialParm(void)
{
    VDEC_RV_INFO_T *prRvInfo;
    RvPicAttr *prPic;
    UINT32 *pu4FrminfoPtr;
    UINT32 u4Parm;

    _VDEC_RVGetWholePesStruct(0, (void **)&prRvInfo);

    pu4FrminfoPtr = (UINT32 *)(prRvInfo->rPesInfo.u4RenderPts + prRvInfo->rOther.u4FrminfoPtr);
    u4Parm = *pu4FrminfoPtr++;

    prPic = &(prRvInfo->rRvPicAttr);

    prPic->u4OrgWidth = u4GetField(u4Parm, 2, 12);
    prPic->u4OrgHeight = u4GetField(u4Parm, 13, 23);
}
#endif


// !!!
#define VFIFO_S 0x5134000
#define VFIFO_E (0x7FF0000 - 1048576 * 2 - MV_DIRECT_SZ - VLD_PRED_OFFSET_SZ)
#define VFIFO_ADDR VFIFO_S
#define VFIFO_SZ   (VFIFO_E - VFIFO_S)
#define MV_DIRECT_ADDR      (VFIFO_ADDR + VFIFO_SZ)
#define VLD_PRED_OFFSET     (MV_DIRECT_ADDR + MV_DIRECT_SZ)

static void vVldInit(void)
{
    vWriteREG(RV_VLD_REG_OFST + 38*4, 0x80000000);
#ifndef CC_RV_EMULATION
    vWriteREG(RV_VLD_REG_OFST + 39*4, (UINT32)_rRVDecInfo.rDram.pu1VldPred);
#else
    vWriteREG(RV_VLD_REG_OFST + 39*4, VLD_PRED_OFFSET);
#endif

    // requested by ChunChia, 20090519
    vWriteREG(RV_VLD_REG_OFST + 0x9C, 0);
}

static void vMvInit(void)
{
#ifndef CC_RV_EMULATION
    vWriteREG(AVC_MV_REG_OFST + 131*4, ((UINT32)_rRVDecInfo.rDram.pu1MvDirect) >> 4);  // it is a 16-byte address!
#else
    vWriteREG(AVC_MV_REG_OFST + 131*4, MV_DIRECT_ADDR >> 4);  // it is a 16-byte address!
#endif
    vWriteREG(AVC_MV_REG_OFST + 134*4, 0);
}

static void vMcInit(void)
{
  // MC global setting
  //vWriteREG(MC_REG_OFST + 39*4, (MC_REF_OFFSET/512) + 0x2FD0);
  //vWriteREG(MC_REG_OFST + 40*4, (MC_REF_OFFSET/256) + 0x7F80);

  vWriteREG(MC_REG_OFST + 28*4, 0x0);
#ifndef CC_RV_EMULATION
  vWriteREG(MC_REG_OFST + 36*4, 2);
#else
  vWriteREG(MC_REG_OFST + 36*4, 0x4);
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
static void INIT_BARREL_SHIFTER(void)
{
  vWriteREG(RV_VLD_REG_OFST + 0x84, 0x1);

  // RPAR: read pointer
  *(UINT32 *)0x200020b0 = VFIFO_ADDR;

  *(UINT32 *)0x20002110 = 0xffffffff;               // RCRW: ???
  *(UINT32 *)0x200020b4 = (VFIFO_ADDR >> 6);               // DSTA: V FIFO start addr
  *(UINT32 *)0x200020b8 = (VFIFO_ADDR + VFIFO_SZ) >> 6;    // DETA: V FIFO end addr
  *(UINT32 *)0x2000208c = 0x00100000;               // RCPR: set initial fetch flag

  while (!((*(UINT32 *)0x200020e8) & 0x00000001));  // wait for FFSR bit 0 raise

  *(UINT32 *)0x2000208c = 0x00800000;  // RCPR: set initial filling of barrel shifter flag

    while((((*(UINT32 *)0x200020ec & 0xf) * 4) +
        (((*(UINT32 *)0x200020ec) >> 24) & 0x3)) !=3);
}

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

BOOL fgVDecProcRV(void)
{
  VDEC_RV_INFO_T *prRvInfo;
  RvPicAttr *prPic;
  UINT32 u4PpRvPara;
  UINT32 i;

  _VDEC_RVGetWholePesStruct(0, (void **)&prRvInfo);
  prPic = &prRvInfo->rRvPicAttr;

  //vResetRVVDec();  // vVldSoftReset();

  vMvInit();
  vWriteREG(AVC_MV_REG_OFST + 135 * 4, prPic->u4Iratio & 0x0000FFFF);

  vMcInit();

  //vPpInit();  because it does nothing

  //vWriteREG(MC_REG_OFST + 39*4, _u4OutBufY >> 9);
  //vWriteREG(MC_REG_OFST + 40*4, _u4OutBufC >> 8);
  //vWriteREG(MC_REG_OFST + 137*4, _u4OutBufY >> 9);
  //vWriteREG(MC_REG_OFST + 138*4, _u4OutBufC >> 8);
#if MC_PP_MODE == 1
  vWriteREG(MC_REG_OFST + 136*4, 1);
#elif MC_PP_MODE == 0
  vWriteREG(MC_REG_OFST + 136*4, (prPic->u4DFP == 0 ? 1 : 0));
#endif
#ifdef __MODEL_slt__
  vWriteREG(MC_REG_OFST + 139*4, prPic->u4Width / 16);
#else
  vWriteREG(MC_REG_OFST + 139*4, prPic->u4LineSize / 16);
#endif
  vWriteREG(MC_REG_OFST + 142*4, (prPic->u4DFP == 0 ? ((!prPic->fgRV9 && (prPic->ePtype != RV_INTRAPIC && prPic->ePtype != RV_FORCED_INTRAPIC)) ? 2 : 3) : 0));
#if MC_PP_MODE == 1
  vWriteREG(MC_REG_OFST + 148*4, 1);
#elif MC_PP_MODE == 0
  vWriteREG(MC_REG_OFST + 148*4, (prPic->u4DFP == 0 ? 1 : 0));
#endif
  vWriteREG(MC_REG_OFST + 152*4, prPic->u4Width / 16 - 1);
  vWriteREG(MC_REG_OFST + 153*4, prPic->u4Height / 16 - 1);
  if (prPic->ePtype == RV_TRUEBPIC)
    u4PpRvPara = _u4RefQpMb0 << 16;
  else
    u4PpRvPara = prPic->u4Pquant << 16;
  u4PpRvPara |= (prPic->ePtype == RV_TRUEBPIC ? 1 : 0) << 11;
  u4PpRvPara |= (prPic->ePtype == RV_TRUEBPIC ? 1 : 0) << 10;
  u4PpRvPara |= prPic->u4DFP << 9;
  u4PpRvPara |= (prPic->u4Width * prPic->u4Height <= 176 * 144 ? 1 : 0) << 8;
  u4PpRvPara |= _u4Rv8TblNum << 4;
  u4PpRvPara |= (prPic->fgRV9 ? 1 : 0) << 1;
  u4PpRvPara |= (prPic->fgRV9 ? 0 : 1);
  vWriteREG(RV_PP_REG_OFST + 2*4, u4PpRvPara);  // !!! 0x110011
#ifndef CC_RV_EMULATION
  vWriteREG(RV_PP_REG_OFST + 0xC, u4ReadREG(RV_PP_REG_OFST + 0xC) & 0xFFFFFFDF);
#endif

#if 0
  // MC setting
  if (prPic->ePtype == RV_INTERPIC || prPic->ePtype == RV_TRUEBPIC)
  {
    vWriteREG(MC_REG_OFST +  0*4, _u4FwdBufY >> 9);
    vWriteREG(MC_REG_OFST +  1*4, _u4FwdBufC >> 8);
  }
  if (prPic->ePtype == RV_TRUEBPIC)
  {
    vWriteREG(MC_REG_OFST +  2*4, _u4BwdBufY >> 9);
    vWriteREG(MC_REG_OFST +  3*4, _u4BwdBufC >> 8);
  }
#endif
  vWriteREG(MC_REG_OFST + 420*4, prPic->u4Iratio);       // B-picture I ratio
  vWriteREG(MC_REG_OFST + 130*4, prPic->u4Width);
  vWriteREG(MC_REG_OFST + 131*4, prPic->u4Height);

  if (prPic->ePtype == RV_INTRAPIC || prPic->ePtype == RV_FORCED_INTRAPIC)
      _u4BwdIsI = 1;
  else if (prPic->ePtype == RV_INTERPIC)
      _u4BwdIsI = 0;
  if (prPic->ePtype == RV_TRUEBPIC)
  {
      vWriteREG(AVC_MV_REG_OFST + 134 * 4, _u4BwdIsI);
  }

#if 1 // !!! Dvid: turn on mc_new_bandwidth_mode
  //if (_fgSetMc474)
  {
    //vWriteREG(MC_REG_OFST + 474*4, u4ReadREG(MC_REG_OFST + 474*4) | 0x100);  !!!
  }
  vWriteREG(MC_REG_OFST + 460*4, 0x0000ffff);
#endif  // VLD setting

  vVldInit();
  vWriteREG(VLD_REG_OFST + 36*4, (prPic->u4Height << 16) | (prPic->u4Width / 16) | 0xFF00);  // !!! David
#ifdef __MODEL_slt__
  vWriteREG(VLD_REG_OFST + 50*4, prPic->u4Width / 16);
#else
  vWriteREG(VLD_REG_OFST + 50*4, prPic->u4LineSize / 16);
#endif
#ifndef CC_RV_EMULATION
#else
  INIT_BARREL_SHIFTER();  // !!!
  RV_VLD_SHIFT_BIT(prPic->u4HdrSkip);  // !!!
#endif
  vWriteREG(RV_VLD_REG_OFST + 33*4, _u4CycleCnt | _u4RvErrConceal | _u4FastParsing | 0x3 | (prPic->fgRV9 ? 4 : 0));  // RV8 or RV9
  vWriteREG(RV_VLD_REG_OFST + 34*4, (prPic->u4Width / 16 - 1) << 24 | prPic->u4OrgHeight << 12 | prPic->u4OrgWidth);
  vWriteREG(RV_VLD_REG_OFST + 35*4, prPic->u4MbaSize << 24 | prPic->u4PctszSize << 22 | ((prPic->u4Width / 16) * (prPic->u4Height / 16)) << 8 | (prPic->u4Height / 16 - 1));  //  0xbc39816
  vWriteREG(RV_VLD_REG_OFST + 36*4, prPic->u4Rtype << 26 | prPic->u4Pctsz << 23 | prPic->u4Tr << 10 | prPic->u4DFP << 9 | prPic->u4Oquant << 7 | prPic->u4Pquant << 2 | (UINT32)prPic->ePtype);  // 0x4000044
  vWriteREG(RV_VLD_REG_OFST + 37*4, prPic->u4Mba);
  vWriteREG(RV_VLD_REG_OFST + 61*4, prPic->u4NumSlice);
  for (i = 0; i < prPic->u4NumSlice; i++)
  {
    vWriteREG(RV_VLD_REG_OFST + (68 + i)*4, prPic->au4SliceSize[i]);
  }

  _u4VldWindow = u4ReadREG(RV_VLD_REG_OFST);

#ifdef CC_MT5395
  HalFlushDCacheMultipleLine(prRvInfo->rPesInfo.u4FifoStart, prRvInfo->rPesInfo.u4FifoEnd - prRvInfo->rPesInfo.u4FifoStart);
#else
  HalFlushInvalidateDCache();
#endif

  // trigger
  vWriteREG(RV_VLD_REG_OFST + 42*4, 0x1);

  return TRUE;
}

//extern UINT32 _rRVDecInfo.rOther.u4BitCount;
BOOL fgVDecodeEndRV(void)
{
  VDEC_RV_INFO_T *prRvInfo;
  RvPicAttr *prPic;
  UINT32 u4Wait;

  _VDEC_RVGetWholePesStruct(0, (void **)&prRvInfo);
  prPic = &prRvInfo->rRvPicAttr;

  // wait for complete
  _fgDecTimeout = FALSE;
  u4Wait = 0;
  while (u4ReadREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_FLAG) == 0)
  {
    if (++u4Wait > DEC_WAIT_CNT)
    {
      _fgDecTimeout = TRUE;
      break;
    }
  }
  vWriteREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_CLR, 1);
  vWriteREG(RV_VLD_REG_OFST + RW_RV_VLD_OK_CLR, 0);
  u4Wait = 0;
  while (u4ReadREG(RV_VLD_REG_OFST + RW_RV_VLD_PIC_FINISH) == 0)
  {
    if (++u4Wait > DEC_WAIT_CNT)
    {
      _fgDecTimeout = TRUE;
      break;
    }
  }

#if 0
  if (!_fgDecTimeout)
  {
    UINT32 u4CycleCnt;

    u4CycleCnt = u4ReadREG(RV_VLD_REG_OFST + 50 * 4);
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
        _fgDecTimeout = TRUE;
        break;
      }
    }
    if (!_fgDecTimeout)
      _eRprRacStt = RPR_RAC_SCP1_DONE;
  }
#endif

  // save ref_qp_mv0 for the next picture
  if (prPic->ePtype != RV_TRUEBPIC && !_fgDecTimeout)
    _u4RefQpMb0 = u4ReadREG(RV_PP_REG_OFST + 7 * 4);

  if (_fgDecTimeout)
  {
      prRvInfo->rOther.u4TimeOutCnt++;
      prRvInfo->rOther.u4LastTimeOutPicNo = prRvInfo->rOther.u4CurrPicNo;
      LOG(0, "[VDEC] RV Decode timeout!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  }
  else
  {
#ifdef CC_RV_EMULATION
      printf("Decode complete!\n");
#endif
      u4Wait = u4ReadREG(RV_VLD_REG_OFST + 0xC0);
      if (u4Wait)
      {
          prRvInfo->rOther.u4DecErrCnt++;
          prRvInfo->rOther.u4LastDecErrPicNo = prRvInfo->rOther.u4CurrPicNo;
          LOG(0, "[VDEC] RV Decode error: 0x%08X!!!!!!!!!!!!!!!!!!!!!!!!!!\n", u4Wait);
      }
      /*
      if (prRvInfo->rRvPicAttr.ePtype == RV_INTRAPIC || prRvInfo->rRvPicAttr.ePtype == RV_FORCED_INTRAPIC)
          printf("[RVDEC] I pic pts = %d\n", prRvInfo->rPesInfo.u4PTS);
      */
  }

#ifdef __MODEL_slt__
  if (prRvInfo->rPesInfo.u4PTS == 96030)
  {
      UINT32 *pu4Ptr;
      UINT32 u4CksumY, u4CksumC, i;

      pu4Ptr = VIRTUAL(prRvInfo->rDram.pu1Pic2Y);
      for (i = 0, u4CksumY = 0; i < 640 * 480 / 4; i++)
          u4CksumY ^= *pu4Ptr++;
      Printf("RM SLT Y cksum: 0x%08X\n", u4CksumY);

      pu4Ptr = VIRTUAL(prRvInfo->rDram.pu1Pic2C);
      for (i = 0, u4CksumC = 0; i < 640 * 480 / 8; i++)
          u4CksumC ^= *pu4Ptr++;
      Printf("RM SLT C cksum: 0x%08X\n", u4CksumC);

      if (u4CksumY == 0x86395C45 && u4CksumC == 0x2E2FCAEF)
          Printf("MMOK!!\n");
      else
          Printf("MMNG!!\n");
  }
#endif

  return (!_fgDecTimeout);
}


/** Initialize module for wmv
 */
void _RVInit()
{
    vInitRVVDec();
    vInitRVVld();
    vInitRVPred();
    //vWriteREG(RW_MC_ADDRSWAP, 0x4); //8520,1389 mode
    vWriteREG(RW_MC_ADDRSWAP, 0x2); //5351 mode
}

void _RVHdrPrs(INT32* pi4RetValue)
{
    if (fgVParserProcRV(pi4RetValue))
    {
        //*pi4RetValue = E_VDEC_OK;
    }
    else
    {
        #ifdef USE_FBM
        FBM_ReleaseGroup(_rRVDecInfo.rOther.u1FbgId);
        #endif
        *pi4RetValue = E_VDEC_BS_END;
    }

    return;
}

/** wmv decoder hardware config and decoding trigger
*@param hVdec handler
 */
void _RVDecStart(INT32* pi4RetValue)
{
    //LOG(6,"Wait to Dec, no: %d, type: %d\n", _rRVDecInfo.rOther.u4CurrPicNo, _rRVDecInfo.rPicLayer.u1PicType);
    if(fgVDecProcRV())
    {
        //_rRVDecInfo.rOther.u1DecState = VDEC_NORM_WAIT_DECODE;
        //_rRVDecInfo.rOther.u4Test01 = 0;
        *pi4RetValue = E_VDEC_OK;
    }
    else
    {
        *pi4RetValue = E_VDEC_FAIL;
    }
}


/** wmv wait decoding finish, display module setting / decoded data checking
*@param hVdec handler
 */
void _RVDecFin(INT32* pi4RetValue)
{
  if (fgVDecodeEndRV())
  {
    vSetRVVSyncPrmBufPtr(_rRVDecInfo.rDram.u4DecBufIdx);

    //Set VLD RPtr to the position of the next picture.
    //fgVldInitBarrelShifter(((UINT32)_rRVDecInfo.rDram.pu1VFifo) + _rRVDecInfo.rOther.i4SetPos);  !!!

    //_rRVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
    _rRVDecInfo.rOther.u4CurrPicNo++;

    *pi4RetValue = E_VDEC_OK;
  }
  else
  {
#if 1
    *pi4RetValue = E_VDEC_OK;
    vSetRVVSyncPrmBufPtr(_rRVDecInfo.rDram.u4DecBufIdx);
#else
    *pi4RetValue = E_VDEC_FAIL;
#endif
  }
}

/** wmv SW reset
*@param hVdec handler
 */
void _RVSWRst(INT32* pi4RetValue)
{
    //UINT32 dwByte, dwBit;
    //dwByte = dwRVVldRPtr(&dwBit);
    vResetRVVDec();
    //vSetRVMcBufPtr();
    vSetRVVldVFifo();
    vInitRVPred();
    //vWriteREG(RW_MC_ADDRSWAP, 0x4); //8520,1389 mode
    vWriteREG(RW_MC_ADDRSWAP, 0x2);//5351 mode

    *pi4RetValue = E_VDEC_OK;
}

/** wmv decode stop / freeze
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
