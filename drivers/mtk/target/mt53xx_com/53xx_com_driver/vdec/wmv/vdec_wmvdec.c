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

/** @file vdec_wmvdec.c
 *  This header file declares exported APIs of wmv.
 */

#include "x_assert.h"
#include "x_printf.h"
#include "dmx_if.h"
#include "vdec_wmvhw.h"
#include "vdec_wmvutil.h"
#include "vdec_wmvdec.h"
#include "vdec_wmvpsr.h"
#include "x_util.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#ifdef USE_FBM
#include "fbm_drvif.h"
#endif

#define VDEC_MM_WAIT_DISP_TIME 1000
#define WMV_DEFAULT_FRAME_RATE (30000)

#ifdef CC_MT5395
externC void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);
#else
extern void HalFlushInvalidateDCache(void);
#endif

#ifndef AVC_IRQ_DISABLE
#include "x_os.h"
HANDLE_T hVdecWMVFinSema = NULL;
#endif

//#define VDEC_TIME_PROFILE

#ifdef VDEC_TIME_PROFILE
static HAL_TIME_T _rTimeRstS, _rTimeRstE, _rTimeRstDt;
#endif


#ifndef AVC_IRQ_DISABLE
#define WMVVLD_VECTOR 24
void _WMVVLDIsr(UINT16 u2Vector)
{
    LOG(6,"----------------------entering WMVVLDISR\n\n");
    if (u2Vector == WMVVLD_VECTOR)
    {
        //BOOL fgEnable = FALSE;
        if (ReadREG(RO_VLD_PIC_DEC_END) & PIC_DEC_FINISH)
        {
            WriteREG(RW_VLD_DECSTART, (UINT32)(1<<8));
            WriteREG(RW_VLD_DECSTART, (UINT32)(0<<8));
#ifdef VDEC_TIME_PROFILE
            HAL_GetTime(&_rTimeRstE);
            HAL_GetDeltaTime(&_rTimeRstDt, &_rTimeRstS, &_rTimeRstE);
            LOG(5, "VDec decoding ES Delta T = (%ld), %d.%6d \n", _rTimeRstDt.u4Micros,
                _rTimeRstE.u4Seconds, _rTimeRstE.u4Micros);
#endif
#if 0
            if (fgEnable)
            {
                UINT32 u4Idx;
                printf("dump VLD state for timeout\n\n");
                for (u4Idx =44 ; u4Idx<=250 ; u4Idx++)
                {
                    printf("RISC Read VLD(%d) = 0x%08x \n", u4Idx,ReadREG(VLD_REG_OFST + u4Idx*4));
                }
                printf("\n\ndump VLD state for timeout\n");

                printf("dump MC state for timeout\n\n");
                for (u4Idx =0 ; u4Idx<=200 ; u4Idx++)
                {
                    printf("RISC Read MC(%d) = 0x%08x \n", u4Idx,ReadREG(MC_REG_OFST + u4Idx*4));
                }
                printf("\n\ndump MC state for timeout\n");
            }
#endif
#if 0
            if (fgEnable)
            {
                UINT32 u4Idx;
                printf("dump MC checksum for timeout\n\n");
                for (u4Idx =378 ; u4Idx<=393 ; u4Idx++)
                {
                    printf("RISC Read MC checksum(%d) = 0x%08x \n", u4Idx,ReadREG(MC_REG_OFST + u4Idx*4));
                }
                printf("\n\ndump MC checksum for timeout\n");
            }
#endif

            //unlock semaphore
            VERIFY(x_sema_unlock(hVdecWMVFinSema) == OSR_OK);
            //rWMVDecInfo.rOther.fgRet = FALSE;
        }
        else if (ReadREG(RO_VLD_BP_DEC_END) & BD_DEC_FINISH)
        {
            // never use it. to change pool to use this.
            WriteREG(RW_VLD_DECSTART, (UINT32)(1<<8));
            WriteREG(RW_VLD_DECSTART, (UINT32)(0<<8));
        }
        else
        {
            LOG(0,"//=== Interrupt with no status? ===\n");
            rWMVDecInfo.rOther.fgRet = TRUE;
        }
    }
}
#endif

#ifdef USE_FBM
void SetPicHdr(UCHAR u1FbId)
{
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    VDEC_ES_INFO_T* prVdecEsInfo;

#if 0
    BOOL fgNotDisplay = FALSE;
#endif

    prVdecEsInfo = _VDEC_GetEsInfo(0);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(rWMVDecInfo.rOther.u1FbgId, u1FbId);
    ASSERT(prFbmPicHdr != NULL);
    ASSERT(prVdecEsInfo != NULL);
    if (prFbmPicHdr == NULL || prVdecEsInfo==NULL)
    {
        LOG(3,"WMV SetPicHdr warning : prFbmPicHdr == NULL || prVdecEsInfo ==NULL\n");
        return;
    }
    //UINT8 u1RepeatFrameCount;  //(RPTFRM) need to add in
    if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE)
    {
        prFbmPicHdr->fgProgressiveFrm = TRUE;
    }
    else
    {
        prFbmPicHdr->fgProgressiveFrm = FALSE;
        //prFbmPicHdr->fgProgressiveFrm = TRUE;
    }
    prFbmPicHdr->fgProgressiveFrm = TRUE;
    //prFbmPicHdr->fgProgressiveFrm = rWMVDecInfo.rPicLayer.u1FrameCodingMode;
    prFbmPicHdr->fgRepFirstFld = rWMVDecInfo.rPicLayer.fgRepeatFirstField;
    prFbmPicHdr->fgTopFldFirst = rWMVDecInfo.rPicLayer.fgTopFieldFirst;
    //prFbmPicHdr->ucPicStruct = rWMVDecInfo.rPicLayer.
    prFbmPicHdr->u4PTS = rWMVDecInfo.rPesInfo.u4PTS;

    if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_NETFLIX)
    {
        prFbmPicHdr->fgSeamlessDisp = TRUE;
        prFbmPicHdr->u4PicWidth = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        prFbmPicHdr->u4PicWidthPitch = rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
        prFbmPicHdr->u4PicHeight = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
    }

    prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;

    //prFbmPicHdr->u8OffsetI = rWMVDecInfo.rPesInfo.u8OffsetI;
    //prFbmPicHdr->u8Offset = rWMVDecInfo.rPesInfo.u8Offset;
    prFbmPicHdr->i4TemporalRef = rWMVDecInfo.rPicLayer.i4TemporalRef;
    prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;
    prFbmPicHdr->u8Offset = prVdecEsInfo->u8Offset;
    prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
    prFbmPicHdr->u8OffsetI = prVdecEsInfo->u8OffsetI;
    prFbmPicHdr->u2DecodingOrder = prVdecEsInfo->u2DecodingOrder;


#if 0
    fgNotDisplay = VDEC_IsNotDisplay(0, rWMVDecInfo.rPesInfo.u4PTS, 0, 0);
    if (fgNotDisplay)
    {
        FBM_SetFrameBufferPicFlag(rWMVDecInfo.rOther.u1FbgId,
                                  u1FbId,
                                  FBM_MM_NOT_DISPLAY_FLAG);

        LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", u1FbId);
    }
#endif

    /*if(rWMVDecInfo.rPicLayer.u1PicType== IVOP)
        LOG(0,"I frame , PTS valuse : %x\n",rWMVDecInfo.rPesInfo.u4PTS);
    else if(rWMVDecInfo.rPicLayer.u1PicType== PVOP)
        LOG(0,"P frame , PTS valuse : %x\n",rWMVDecInfo.rPesInfo.u4PTS);
    */
#ifdef CC_RESIZE_SMALL_IMAGE
    prFbmPicHdr->u4PicWidth = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
    prFbmPicHdr->u4PicHeight = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
    prFbmPicHdr->u4PicWidthPitch = rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
#endif
}

void SetSeqHdr()
{
    if (rWMVDecInfo.rOther.u1GetSeqHdr == 0)
    {
        FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
        VDEC_ES_INFO_T *prVdecEsInfo;
        UINT32 u4fps = 23976;
        UINT32 u4DispAspectRatio = 1;
        UINT32 u4DecoderWidth;
        UINT32 u4DecoderHeight;
        prVdecEsInfo = _VDEC_GetEsInfo(ES0);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(rWMVDecInfo.rOther.u1FbgId);

        ASSERT(prFbmSeqHdr != NULL && prVdecEsInfo!=NULL);

        if (prFbmSeqHdr == NULL || prVdecEsInfo==NULL)
        {
            LOG(3,"WMV SetSeqHdr warning : prFbmSeqHdr == NULL || prVdecEsInfo==NULL \n");
            return;
        }

        prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;//rWMVDecInfo.rSeqLayer.i4Aspect_Ratio;//;        // 16:9
        //prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
        prFbmSeqHdr->fgProgressiveSeq = 1;//!rWMVDecInfo.rSeqLayer.fgInterlacedSource;//1;//(rWMVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE);
        u4DecoderWidth = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        u4DecoderHeight = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;

        if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
        {
            if (rWMVDecInfo.rSeqLayer.i4FrameRateNr !=0 && rWMVDecInfo.rSeqLayer.i4FrameRateDr !=0)
            {
                UINT64 u8FrameRate;
                UINT64 u8FrameRateScale;
                UINT64 u8Remainder;
                if (rWMVDecInfo.rSeqLayer.i4FrameRateDr ==1)
                {
                    u8FrameRateScale = 1000;
                }
                else if (rWMVDecInfo.rSeqLayer.i4FrameRateDr ==2)
                {
                    u8FrameRateScale = 1001;
                }
                else
                {
                    LOG(3,"WMV decoder warning : rWMVDecInfo.rSeqLayer.i4FrameRateDr = %d\n",rWMVDecInfo.rSeqLayer.i4FrameRateDr);
                    u8FrameRateScale = 1000;
                }
                switch (rWMVDecInfo.rSeqLayer.i4FrameRateNr)
                {
                case 1:
                    u8FrameRate = 24000000;
                    break;
                case 2:
                    u8FrameRate = 25000000;
                    break;
                case 3:
                    u8FrameRate = 30000000;
                    break;
                case 4:
                    u8FrameRate = 50000000;
                    break;
                case 5:
                    u8FrameRate = 60000000;
                    break;
                default:
                    u8FrameRate = 0;
                    break;
                }

                u4fps = (UINT32)u8Div6432(u8FrameRate, u8FrameRateScale, &u8Remainder);
            }
            else
            {
                //decoder without frame rate syntax information
                LOG(3,"wmv decoder without frame rate syntax information , set fps = %d\n",u4fps);
            }
        }
        else if (rWMVDecInfo.rOther.i4CodecVersion <= WMV3)
        {
            u4fps = rWMVDecInfo.rSeqLayer.i4FrameRate;
            u4fps *= 1000;
        }

        if (u4fps ==0)
        {
            switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
            {
            case VDEC_SRC_FRAME_RATE_23_976:
                u4fps = 23976;
                break;
            case VDEC_SRC_FRAME_RATE_24:
                u4fps = 24000;
                break;
            case VDEC_SRC_FRAME_RATE_25:
                u4fps = 25000;
                break;
            case VDEC_SRC_FRAME_RATE_29_97:
                u4fps = 29970;
                break;
            case VDEC_SRC_FRAME_RATE_30:
                u4fps = 30000;
                break;
            case VDEC_SRC_FRAME_RATE_50:
                u4fps = 50000;
                break;
            case VDEC_SRC_FRAME_RATE_59_94:
                u4fps = 59940;
                break;
            case VDEC_SRC_FRAME_RATE_60:
                u4fps = 60000;
                break;
            default:
                u4fps = 0;
                break;
            }
        }

        if (u4fps != 0)
        {
            rWMVDecInfo.rOther.u4DeltaPTS = (90000000/u4fps);
            u4fps /= 1000;
        }
        else
        {
            rWMVDecInfo.rOther.u4DeltaPTS = (90000000/WMV_DEFAULT_FRAME_RATE);
        }

        switch (u4fps)
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

        //prFbmSeqHdr->ucAspRatInf = rWMVDecInfo.rSeqLayer.i4Aspect_Ratio; // 16:9
        if (rWMVDecInfo.rSeqLayer.fgProgressiveDisp)
        {
            prFbmSeqHdr->fgProgressiveSeq = TRUE;
        }
        else
        {
            if (!rWMVDecInfo.rSeqLayer.fgInterlacedSource) {
                prFbmSeqHdr->fgProgressiveSeq = TRUE;
            }
            else {
                prFbmSeqHdr->fgProgressiveSeq = FALSE;
            }
        }
        //LOG(0,"prFbmSeqHdr->fgProgressiveSeq : %d\n",prFbmSeqHdr->fgProgressiveSeq);

        switch (rWMVDecInfo.rSeqLayer.i4Aspect_Ratio)
        {
        case 0x1: // 1:1 Square
            u4DispAspectRatio = (1000* u4DecoderWidth) / u4DecoderHeight;
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;
            break;
        case 0x2: // 12:11 625-type for 4:3 picture
            u4DispAspectRatio = (1000* u4DecoderWidth*12) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_12_11;
            break;
        case 0x3: // 10:11 525-type for 4:3 picture
            u4DispAspectRatio = (1000* u4DecoderWidth*10) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_10_11;
            break;
        case 0x4: // 16:11 625-type stretched for 16:9 picture
            u4DispAspectRatio = (1000* u4DecoderWidth*16) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_16_11;
            break;
        case 0x5: // 40:33 525-type stretched for 16:9 picture
            u4DispAspectRatio = (1000* u4DecoderWidth*40) / (u4DecoderHeight*33);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_40_33;
            break;
        case 0x6: //24:11
            u4DispAspectRatio = (1000* u4DecoderWidth*24) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_24_11;
            break;
        case 0x7: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*20) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_20_11;
            break;
        case 0x8: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*32) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_32_11;
            break;
        case 0x9: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*80) / (u4DecoderHeight*33);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_80_33;
            break;
        case 0xA: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*18) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_18_11;
            break;
        case 0xB: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*15) / (u4DecoderHeight*11);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_15_11;
            break;
        case 0xC: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*64) / (u4DecoderHeight*33);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_64_33;
            break;
        case 0xD: // Reserved
            u4DispAspectRatio = (1000* u4DecoderWidth*160) / (u4DecoderHeight*99);
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_24_11;
            break;
        case 0xF: // extended PAR
        case 0xE: // Reserved
        case 0x0: // forbidden
        default:
            u4DispAspectRatio = (1000* u4DecoderWidth) / u4DecoderHeight;
            prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;
            break;
        }

        if (rWMVDecInfo.rOther.i4CodecVersion <= WMV3)
        {
            if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_NETFLIX)
            {
                UINT32 u4Tmp;
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                prFbmSeqHdr->u4ResizeWidth = FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u4ResizeHeight = FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                if (u4DispAspectRatio < FBM_IMG_RESIZE_NETFLIX_WIDTH*1000/FBM_IMG_RESIZE_NETFLIX_HEIGHT)
                {
                    u4Tmp = (u4DispAspectRatio*FBM_IMG_RESIZE_NETFLIX_HEIGHT)/1000;
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = u4Tmp;
                    prFbmSeqHdr->u4ResizeHeight = FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                }
                else
                {
                    u4Tmp = (FBM_IMG_RESIZE_NETFLIX_WIDTH*1000/u4DispAspectRatio);
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = FBM_IMG_RESIZE_NETFLIX_WIDTH;
                    prFbmSeqHdr->u4ResizeHeight = u4Tmp;
                }
            }
            else
            {
#ifdef CC_RESIZE_SMALL_IMAGE
                if (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION ||
                        rWMVDecInfo.rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->fgResizeSmallPic = TRUE;

                    if (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
                        prFbmSeqHdr->u2HSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                    }

                    if (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                    }
                }
                else
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
                    prFbmSeqHdr->u2HSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2VSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                }
#else
                prFbmSeqHdr->u2LineSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
                prFbmSeqHdr->u2HSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2VSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                prFbmSeqHdr->u2OrgHSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2OrgVSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
#endif
            }
        }
        else /* UMV Padding directly from picture boundary */
        {
            UINT32 width,height;
            FBM_GetFrameBufferSize(rWMVDecInfo.rOther.u1FbgId,&width,&height);
            //LOG(0,"fbm resolution %dx%d\n",width,height);
            //LOG(0,"true resolution %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthSrc,rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
            //LOG(0,"alignment resolution %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthDec,rWMVDecInfo.rSeqHdrR.u4PicHeightDec);
            if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_NETFLIX)
            {
                UINT32 u4Tmp;
                prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                prFbmSeqHdr->u4ResizeWidth = FBM_IMG_RESIZE_NETFLIX_WIDTH;
                prFbmSeqHdr->u4ResizeHeight = FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                if (u4DispAspectRatio < FBM_IMG_RESIZE_NETFLIX_WIDTH*1000/FBM_IMG_RESIZE_NETFLIX_HEIGHT)
                {
                    u4Tmp = (u4DispAspectRatio*FBM_IMG_RESIZE_NETFLIX_HEIGHT)/1000;
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = u4Tmp;
                    prFbmSeqHdr->u4ResizeHeight = FBM_IMG_RESIZE_NETFLIX_HEIGHT;
                }
                else
                {
                    u4Tmp = (FBM_IMG_RESIZE_NETFLIX_WIDTH*1000/u4DispAspectRatio);
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = FBM_IMG_RESIZE_NETFLIX_WIDTH;
                    prFbmSeqHdr->u4ResizeHeight = u4Tmp;
                }

            }
            else
            {
#ifdef CC_RESIZE_SMALL_IMAGE
                if (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION ||
                        rWMVDecInfo.rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION)
                {
                    prFbmSeqHdr->fgResizeSmallPic = TRUE;

                    if (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
                        prFbmSeqHdr->u2HSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                    }

                    if (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                    }
                }
                else
                {
                    prFbmSeqHdr->u2LineSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
                    prFbmSeqHdr->u2HSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2VSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                }
#else
                prFbmSeqHdr->u2LineSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthDec;
                prFbmSeqHdr->u2HSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2VSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
                prFbmSeqHdr->u2OrgHSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2OrgVSize = (UINT16)rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
#endif
            }
        }
        rWMVDecInfo.rOther.u1GetSeqHdr = 1;
    }
}
#endif

void vVPrsIPProc(void)
{

    if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(rWMVDecInfo.rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, (UINT32)FBM_FLAG_NOT_READY);
        }
    }

#ifdef REPEAT_FRAME
    if (rWMVDecInfo.rOther.u4repeatframe)
    {
        // do nothing
        return;
    }
#endif

    {
        vSetFRefBuf(rWMVDecInfo.rDram.u4BRefBufIdx);
        vSetBRefBuf(1 - rWMVDecInfo.rDram.u4FRefBufIdx);
        vSetDecBuf(rWMVDecInfo.rDram.u4BRefBufIdx);
    }

#ifdef USE_FBM  // megaa 20100113 ToDo: check the matching with #endif
    {
        UINT32 u4Yaddr, u4Caddr;
        UINT32 u4Yaddr2 = NULL;
        UINT32 u4Caddr2 = NULL;

        SetSeqHdr();

        UCHAR ucBRefFbId;
        UCHAR ucFRefFbId;
        if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0))
        {
            FBM_GetRefFrameBuffer(rWMVDecInfo.rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
            if (FBM_FB_ID_UNKNOWN != ucBRefFbId)
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

                prFbmPicHdr = FBM_GetFrameBufferPicHdr(rWMVDecInfo.rOther.u1FbgId, ucBRefFbId);
                FBM_GetFrameBufferAddr(rWMVDecInfo.rOther.u1FbgId, ucBRefFbId,&u4Yaddr2, &u4Caddr2);

                if (prFbmPicHdr)
                {
                    if (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_1X)
                    {
                        if (rWMVDecInfo.rOther.fgFirstDispFrame)
                        {
                            rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                            rWMVDecInfo.rOther.fgFirstDispFrame = FALSE;
                        }
                        else
                        {
                            if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
                            {
                                rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                            }
                            else
                            {
                                if (prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS
                                        && prVdecEsInfo->eContainerType != SWDMX_FMT_MKV
                                        && prVdecEsInfo->eContainerType != SWDMX_FMT_VC1_ES
                                        && prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_192
                                        && prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_ZERO_192
                                        && prVdecEsInfo->eContainerType != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                                {
                                    UINT32 u4HalfDeltaPTS = rWMVDecInfo.rOther.u4DeltaPTS>>1;
                                    if ((prFbmPicHdr->u4PTS <= (rWMVDecInfo.rOther.u4LastPTS+rWMVDecInfo.rOther.u4DeltaPTS+u4HalfDeltaPTS))
                                            && prFbmPicHdr->u4PTS >= (rWMVDecInfo.rOther.u4LastPTS + u4HalfDeltaPTS))
                                    {
                                        rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                    }
                                    else
                                    {
                                        prFbmPicHdr->u4PTS = rWMVDecInfo.rOther.u4LastPTS + rWMVDecInfo.rOther.u4DeltaPTS;
                                        rWMVDecInfo.rOther.u4LastPTS += rWMVDecInfo.rOther.u4DeltaPTS;
                                    }
                                }
                                else
                                {
                                    if (!rWMVDecInfo.rPesInfo.fgDtsValid)
                                    {
                                        prFbmPicHdr->u4PTS = rWMVDecInfo.rOther.u4LastPTS + rWMVDecInfo.rOther.u4DeltaPTS;
                                        rWMVDecInfo.rOther.u4LastPTS += rWMVDecInfo.rOther.u4DeltaPTS;
                                    }
                                    else
                                    {
                                        rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                    }
                                }
                            }
                        }
                        prFbmPicHdr->i4TemporalRef = rWMVDecInfo.rPicLayer.i4TemporalRef;
                        LOG(5,"Vdec WMV I/P/Skip Frame PTS : %8x, temporal ref : %d\n",prFbmPicHdr->u4PTS,prFbmPicHdr->i4TemporalRef);
                        rWMVDecInfo.rPicLayer.i4TemporalRef++;
                    }
                    else if((prVdecEsInfo->u4MMSpeed >= (UINT32)STC_SPEED_TYPE_FORWARD_1_DIV_2X)
                        &&(prVdecEsInfo->u4MMSpeed <= (UINT32)STC_SPEED_TYPE_FORWARD_1_DIV_32X)
                        && (prVdecEsInfo->eContainerType == SWDMX_FMT_VC1_ES))
                    {
                        if (!rWMVDecInfo.rPesInfo.fgDtsValid)
                        {
                            prFbmPicHdr->u4PTS = rWMVDecInfo.rOther.u4LastPTS + rWMVDecInfo.rOther.u4DeltaPTS;
                            rWMVDecInfo.rOther.u4LastPTS += rWMVDecInfo.rOther.u4DeltaPTS;
                        }
                        else
                        {
                            rWMVDecInfo.rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                        }
                    }
//#endif  // megaa 20100113 ToDo: check the matching with #ifdef USE_FBM

                    fgNotDisplay = VDEC_IsNotDisplay(0, prFbmPicHdr->u4PTS,
                                                     prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset);
                }
                else
                {
                    fgNotDisplay = VDEC_IsNotDisplay(0, 0, 0, 0);
                }
                if (fgNotDisplay)
                {
#ifdef WMV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId,ucBRefFbId);
                    LOG(0,"WMV set I/P frame FBM status : %d %d EMPTY\n",ucBRefFbId,u1state);
#endif
                    FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {
#ifdef WMV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId,ucBRefFbId);
                    LOG(0,"WMV set I/P frame FBM status : %d %d DISPALY_Q\n",ucBRefFbId,u1state);
#endif
                    FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                    prVdecEsInfo->u4DisplayQPicCnt++;

                }
                if (prFbmPicHdr)
                {
                    VDEC_CheckStopStatus(0, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef );
                }
                else
                {
                    VDEC_CheckStopStatus(0, 0, 0);
                }
            }


            UCHAR ucFbId = FBM_GetEmptyRefFrameBuffer(rWMVDecInfo.rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
            ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);
            FBM_GetFrameBufferAddr(rWMVDecInfo.rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

            ASSERT(u4Yaddr !=0 || u4Caddr !=0);
            // reset mem here
            SetPicHdr(ucFbId);

            if (rWMVDecInfo.rDram.u4DecBufIdx == 0) //pic1
            {
                rWMVDecInfo.rDram.pu1Pic0Y = (UINT8*)u4Yaddr;
                rWMVDecInfo.rDram.pu1Pic0C = (UINT8*)u4Caddr;
                rWMVDecInfo.rOther.u1FbId0 = ucFbId;
            }
            else //pic2
            {
                rWMVDecInfo.rDram.pu1Pic1Y = (UINT8*)u4Yaddr;
                rWMVDecInfo.rDram.pu1Pic1C = (UINT8*)u4Caddr;
                rWMVDecInfo.rOther.u1FbId1 = ucFbId;
            }
            if (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME && u4Yaddr2!= NULL && u4Caddr2 !=NULL)
            {
                UINT32 u4Width,u4Height;
                UINT32 u4Size;
                FBM_GetFrameBufferSize(rWMVDecInfo.rOther.u1FbgId,&u4Width,&u4Height);
                u4Size = u4Width*u4Height;
                x_memcpy((VOID*)(VIRTUAL(u4Yaddr)),(VOID*)(VIRTUAL(u4Yaddr2)),u4Size);
                x_memcpy((VOID*)(VIRTUAL(u4Caddr)),(VOID*)(VIRTUAL(u4Caddr2)),(u4Size>>1));
            }
            FBM_SetFrameBufferStatus(rWMVDecInfo.rOther.u1FbgId,ucFbId,FBM_FB_STATUS_DECODE);
            //spork
            vSetMcBufPtr();
        }
    }
#endif  // megaa 20100113 ToDo: check the matching with #ifdef USE_FBM
}

void vVPrsBProc(void)
{
    if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
        {
            FBM_ResetGroup(rWMVDecInfo.rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
        }
    }
    vSetDecBuf(2);
    //vSaveFrmBufPrm(rWMVDecInfo.rDram.u4DecBufIdx);

#ifdef USE_FBM
    {
        UINT32 u4Yaddr, u4Caddr;
        SetSeqHdr();
        if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0))
        {
            UCHAR ucFbId = FBM_GetEmptyBFrameBuffer(rWMVDecInfo.rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
            ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);

            FBM_GetFrameBufferAddr(rWMVDecInfo.rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

            ASSERT(u4Yaddr !=0 || u4Caddr !=0);
            // ilii
            // reset mem here
            SetPicHdr(ucFbId);
            //FBM_PIC_HDR_T* prFbmPicHdr;
            //prFbmPicHdr = FBM_GetFrameBufferPicHdr(rWMVDecInfo.rOther.u1FbgId, ucFbId);
            //prFbmPicHdr->fgProgressiveFrm = 1;//(rWMVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE);

            //if((rWMVDecInfo.rPicLayer.u1PicType == BVOP) || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP)) //prog_B
            {
                rWMVDecInfo.rDram.pu1Pic2Y = (UINT8*)u4Yaddr;
                rWMVDecInfo.rDram.pu1Pic2C = (UINT8*)u4Caddr;
                rWMVDecInfo.rOther.u1FbId2 = ucFbId;
            }
            //spork
            vSetMcBufPtr();
        }
    }
#endif
}

BOOL fgVParserProcWMV(void)
{
    //UINT32 u4Tmp;

#ifdef CC_WMV_EMULATION
    if (rWMVDecInfo.rPesInfo.fgSkipFrame)
    {
        return TRUE;
    }
#endif

    // ilii, for more than 1 bitplane, should keep this variable
    //rWMVDecInfo.rOther.u4BPRawFlag = 0;
    if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
    {
        fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo + rWMVDecInfo.rOther.i4SetPos);
        //x_memcpy(&u4Tmp,(void*)(rWMVDecInfo.rDram.pu1VFifo + rWMVDecInfo.rOther.i4SetPos),4);
        LOG(6,"init barrel shifter1, set position : %x\n",rWMVDecInfo.rOther.i4SetPos);
        //LOG(0,"fetch 32bit data : %x\n",u4Tmp);
        //spork
        //fgSkipFrame = rWMVDecInfo.rPesInfo.fgSkipFrame;

        if (rWMVDecInfo.rOther.i4CodecVersion == WMV3) //VC-1 Simple & Main
        {
            rWMVDecInfo.rOther.i4VprErr = WMVideoDecDecodeFrameHead();
        }
        else if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1) || (rWMVDecInfo.rOther.i4CodecVersion == WMV2)) //WMV7, 8
        {
            rWMVDecInfo.rOther.i4VprErr = WMV78DecodePicture();
            if (rWMVDecInfo.rSMProfile.fgXintra8)
            {
                return TRUE;
            }
        }

        switch (rWMVDecInfo.rPicLayer.u1PicType)
        {
        case IVOP:
        case PVOP:
            if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
            {
                // Check if reset is required
                if (FBM_ChkFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
                {
                    FBM_ResetGroup(rWMVDecInfo.rOther.u1FbgId);
                    FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
                    FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
                }
            }
            vVPrsIPProc();
            break;
        case BIVOP:
        case BVOP:
            if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
            {
                // Check if reset is required
                if (FBM_ChkFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
                {
                    FBM_ResetGroup(rWMVDecInfo.rOther.u1FbgId);
                    FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
                    FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
                }
            }

            vVPrsBProc();
            break;
        case SKIPFRAME:
            if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
            {
                // Check if reset is required
                if (FBM_ChkFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET))
                {
                    FBM_ResetGroup(rWMVDecInfo.rOther.u1FbgId);
                    FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RESET);
                    FBM_ClrFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_NOT_READY);
                }
            }
            vVPrsIPProc();
            break;
        default:
            break;
        } //switch(rWMVDecInfo.rPicLayer.u1PicType)

        if (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)
        {
            rWMVDecInfo.rOther.u4DecPicNo++;
        }
        if (cbWMVHeaderOnly)
        {
            print_vop_header_parameter(rWMVDecInfo.rOther.u4DecPicNo, rWMVDecInfo.rDram.u4FRefBufIdx, rWMVDecInfo.rDram.u4DecBufIdx); //umv_from_mb = 1 for !WMVA
            rWMVDecInfo.rOther.u4DecPicNo++;
        }
    }
    else //Advanced Profile
    {
        if ((rWMVDecInfo.rOther.i4VprErr = dwVParserWMVA()) != 0) //paring advanced profile, including sequence header
        {
            if (rWMVDecInfo.rOther.i4VprErr == END_OF_FILE)
            {
                //sprintf(strMsg,"End of file 1");
            }
            else
            {
                //Error Case
            }
            return(FALSE);
        }

        //Switch Frame Buffer
        if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0))
        {
            switch (rWMVDecInfo.rPicLayer.u1PicType)
            {
            case IVOP:
            case PVOP:
                vVPrsIPProc();
                ///WriteREG(RW_MC_SAME_REF_PIC, 0);
                break;
            case BVOP:
            case BIVOP:
                vVPrsBProc();
                break;
            case SKIPFRAME:
                vVPrsIPProc();
                //WriteREG(RW_MC_FWDP, rWMVDecInfo.rDram.u4FRefBufIdx);
                //vSetFRefBuf(rWMVDecInfo.rDram.u4DecBufIdx); // F_Ref = Dec. TEST for Progressive and Interlaced Frame.
                //WriteREG(RW_MC_SAME_REF_PIC, 1);
                break;
            default:
                break;
            } //switch(rWMVDecInfo.rPicLayer.u1PicType)
        }

        if (cbWMVHeaderOnly)
        {
            print_vop_header_parameter(/*0,*/ rWMVDecInfo.rOther.u4DecPicNo, rWMVDecInfo.rDram.u4FRefBufIdx, rWMVDecInfo.rDram.u4DecBufIdx); //umv_from_mb = 0 for WMVA
            if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1))
            {
                rWMVDecInfo.rOther.u4DecPicNo++;
            }
            if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
            {
                rWMVDecInfo.rPicLayer.i4CurrentTemporalField ^= 1; //toggle field
                rWMVDecInfo.rPicLayer.i4CurrentField ^= 1;
            }
        }
        //End of Switch Frame Buffer
    }
    return(TRUE);
}


BOOL fgVDecProcWMV(void)
{
    //UINT32 u4Tmp;
    UINT32 u4MC_DBK;

#if defined(CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395)
    UINT32 u4MC_a377;
#endif

    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        print_vop_header_parameter(rWMVDecInfo.rOther.u4DecPicNo, rWMVDecInfo.rDram.u4FRefBufIdx, rWMVDecInfo.rDram.u4DecBufIdx); //umv_from_mb = 0 for WMVA
        if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1))
        {
            rWMVDecInfo.rOther.u4DecPicNo++;
        }
    }
    else
    {
        print_vop_header_parameter(rWMVDecInfo.rOther.u4DecPicNo, rWMVDecInfo.rDram.u4FRefBufIdx, rWMVDecInfo.rDram.u4DecBufIdx); //umv_from_mb = 1 for !WMVA
        rWMVDecInfo.rOther.u4DecPicNo++;
    }
    //for vld slice header parsing
    WriteREG(VLD_REG_OFST + 0x0228, 0x00080000);
    //WriteREG(MC_REG_OFST+0x660,0x03120456);//change local arbiter priority
    WriteREG(RW_VLD_MCPIC_BLKW, (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15) >> 4);
    //WriteREG(RW_VLD_MCPIC_BLKW, 1920>>4 );

#if defined(CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395)
    //for new MC. merge motion vector
    u4MC_a377 = ReadREG(MC_REG_OFST + 0x5E4);
    u4MC_a377 |=  (1<<12);
    WriteREG(MC_REG_OFST + 0x5E4,u4MC_a377);
    //for new MC. merge motion vector
#endif

#ifdef CC_MT5395
    HalFlushDCacheMultipleLine(rWMVDecInfo.rPesInfo.u4FifoStart, rWMVDecInfo.rPesInfo.u4FifoEnd - rWMVDecInfo.rPesInfo.u4FifoStart);
#else
    HalFlushInvalidateDCache();
#endif
    //u4Tmp = ((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15) >> 4);
    //if(u4Tmp&0x1 ==1)
    //  u4Tmp +=1;


    //WriteREG(RW_VLD_MCPIC_BLKW, ((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 63) >> 6));
    //WriteREG((VLD_REG_OFST + (0x1FC)), 0x1000);

    //import for interrupt , spork
    rWMVDecInfo.rOther.fgRet = FALSE;
#if !defined(CC_MT5365) && !defined(CC_MT5395)
    //LOG(0,"DBK3 : %x\n",ReadREG(MC_REG_OFST + 0x658));
    u4MC_DBK = ReadREG(MC_REG_OFST + 0x658);
    //LOG(0,"DBK1 : %x\n",u4MC_DBK);
    u4MC_DBK &= u4MC_DBK & 0xFFFFFFDF;
    //LOG(0,"DBK2 : %x\n",u4MC_DBK);
    WriteREG(MC_REG_OFST + 0x658,u4MC_DBK);
#endif

    //LOG(5,"Rptr 0x%x, input win 0x%x\n", ReadREG(RO_VLD_VRPTR), ReadREG(RO_VLD_BARL));
    //LOG(6,"PP Y 0x%x, PP C 0x%x\n", ReadREG(RW_MC_PP_Y_ADDR), ReadREG(RW_MC_PP_C_ADDR));
    //update write pointer

    //_VDEC_UpdateVldWMVWPtr(rWMVDecInfo.rOther.u4WPtr);

    //ilii
    /*
    LOG(6,"before trigger, 60 = 0x%x, 62 = 0x%x, 63 = 0x%x, \n56 = 0x%x, 131 = 0x%x, 200 = 0x%x, \n35 = 0x%x, 137 = 0x%x, \n",
    ReadREG(VLD_REG_OFST + (60*4)),
    ReadREG(VLD_REG_OFST + (62*4)),
    ReadREG(VLD_REG_OFST + (63*4)),
    ReadREG(VLD_REG_OFST + (56*4)),
    ReadREG(VLD_REG_OFST + (131*4)),
    ReadREG(VLD_REG_OFST + (200*4)),
    ReadREG(VLD_REG_OFST + (35*4)),
    ReadREG(VLD_REG_OFST + (137*4)));
    */

    //ilii, in this case, f/w decode bp, tell h/w that we have decoded it.
    if (cbWMVFWBP) // trigger bp if  f/w decodes it
    {
        WriteREG(RW_VLD_BP_MODE_FIRE, 1<<26);
        WriteREG(RW_VLD_BP_MODE_FIRE, 0);
    }

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&_rTimeRstS);
#endif

#if defined(CC_MT5365) || defined(CC_MT5395)
    if (rWMVDecInfo.rOther.fgNewMode)
    {
        if (rWMVDecInfo.rOther.i4CodecVersion == WMVA || rWMVDecInfo.rOther.i4CodecVersion == WMV3)  // VC-1 or WMV9
        {
            UINT32 a131, a134, a135, a137, a138, a139;
            int TRB, TRD, FWD, BWD;
            UINT32 amv_max_v, amv_max_h;

            // MV part
            // 131
            if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
            {
                if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) // top
                    a131 = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_1) >> 4;
                else
                    a131 = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_1 + (1920/16)*(1088/16)*16/2) >> 4;
            }
            else
            {
                a131 = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_1) >> 4;  // ???
            }
            WriteREG(RW_WMV_MV_NEW_ADDR, a131);

            // 134
            if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            {
                if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) // top
                    {
                        a134 = ((rWMVDecInfo.rOther.u1FrameTypeLastTop != PVOP) ? 1: 0);
                    }
                    else
                    {
                        a134 = ((rWMVDecInfo.rOther.u1FrameTypeLastBot != PVOP) ? 1: 0);
                    }
                }
                else
                {
                    a134 = ((rWMVDecInfo.rOther.u1FrameTypeLast != PVOP) ? 1: 0);
                }

                WriteREG(RW_WMV_BWDREF_PIC_TYPE, a134);
            }

            // 135
            if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            {
                TRB = rWMVDecInfo.rPicLayer.i4BNumerator * rWMVDecInfo.rPicLayer.i4BFrameReciprocal;
                TRD = TRB - 256;
                a135 = (TRB << 0) + (TRD << 16);

                WriteREG(RW_WMV_MV_DIRECT_SCL_FAC, a135);
            }

            // 137
            if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
            {
                FWD = rWMVDecInfo.rEntryPoint.i4RefFrameDistance;
                BWD = 0;
            }
            else if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            {
                FWD = rWMVDecInfo.rOther.i4FwdRefDist;
                BWD = rWMVDecInfo.rOther.i4BwdRefDist;
            }
            else
            {
                FWD = 0;
                BWD = 0;
            }
            a137 =
                (((rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0) ? 1: 0) << WMV_FIRST_FIELD)
                + (BWD                                                       << WMV_BWDREF_DIST)
                + (FWD                                                       << WMV_FWDREF_DIST)
                + (rWMVDecInfo.rPicLayer.fgUseOppFieldForRef                 << WMV_USE_OPP_REF_FIELD)
                + (rWMVDecInfo.rPicLayer.fgTwoRefPictures                    << WMV_TWO_REF_PIC)
                + ((rWMVDecInfo.rOther.i4CodecVersion == WMVA ? 1 : 0)       << WMV_POST_RESCENT)
                + (rWMVDecInfo.rEntryPoint.fgUVHpelBilinear                  << WMV_FAST_UV)
                + (rWMVDecInfo.rEntryPoint.i4MVRangeIndex                    << WMV_MV_RNG_IDX)
                + (rWMVDecInfo.rOther.use_quarter_pel_mv                     << WMV_QPIXEL);
            //+ (prWmvDecPrm->prPPS->fgHalfStep                          << WMV_QPIXEL);

            WriteREG(RW_WMV_MV_CFG, a137);

            // 138
            amv_max_v = ((rWMVDecInfo.rSeqHdrR.u4PicWidthDec + 15)>> 4) * 2 * 8 * 4 - 4;
            amv_max_h = (((rWMVDecInfo.rSeqHdrR.u4PicHeightDec << (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD ? 1 : 0)) + 15) >> 4) * 2 * 8 * 4 - 4;
            a138 = ((amv_max_h << 16) | amv_max_v);
            WriteREG(RW_WMV_MV_MAX_1, a138);

            // 139
            amv_max_v = ((rWMVDecInfo.rSeqHdrR.u4PicWidthDec + 15)>> 4) * 8 * 4 - 4;
            amv_max_h = (((rWMVDecInfo.rSeqHdrR.u4PicHeightDec << (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD ? 1 : 0)) + 15) >> 4) * 8 * 4 - 4;
            a139 = ((amv_max_h << 16) | amv_max_v);
            WriteREG(RW_WMV_MV_MAX_2, a139);
        }
        else if (rWMVDecInfo.rOther.i4CodecVersion == WMV1 || rWMVDecInfo.rOther.i4CodecVersion == WMV2)  // WMV 7/8
        {
            UINT32 u4Reg;

            vSetNewMode();  // tmp solution

            // MV part
            // 131
            if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
            {
                if (rWMVDecInfo.rPicLayer.i4CurrentField == 0) // top
                    u4Reg = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_1) >> 4;
                else
                    u4Reg = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_1 + (1920/16)*(1088/16)*16/2) >> 4;
            }
            else
            {
                u4Reg = PHYSICAL((UINT32)rWMVDecInfo.rDram.pu1Mv_1) >> 4;  // ???
            }
            WriteREG(RW_WMV_MV_NEW_ADDR, u4Reg);

            // 180
            WriteREG(RW_WMV_MV_180, (rWMVDecInfo.rSMProfile.fgFrmHybridMVOn && !rWMVDecInfo.rPicLayer.fgMvResolution) ? 1 : 0);

            // VLD part
            // 134
            u4Reg = ReadREG(RW_VLD_WMV_OVC);
            u4Reg |= (1 << 15);
            WriteREG(RW_VLD_WMV_OVC, u4Reg);

            // 157, 158, 159
            int i, wmv78_slice_num;
            if (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
                wmv78_slice_num = rWMVDecInfo.rSeqHdrR.u4NumMBY / rWMVDecInfo.rSMProfile.i4SliceCode;
            else
                wmv78_slice_num = rWMVDecInfo.rSeqHdrR.u4NumMBY / (rWMVDecInfo.rSMProfile.i4SliceCode - 22);
            for (i = 1; i < rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
            {
                if (i < 31)
                {
                    u4Reg = ReadREG(RW_VLD_SLICE_MARK_REG0);
                    u4Reg += ((i % wmv78_slice_num) == 0) << (i + 1);
                    WriteREG(RW_VLD_SLICE_MARK_REG0, u4Reg);
                }
                else if (i < 63)
                {
                    u4Reg = ReadREG(RW_VLD_SLICE_MARK_REG1);
                    u4Reg += ((i% wmv78_slice_num) == 0) << (i - 31);
                    WriteREG(RW_VLD_SLICE_MARK_REG1, u4Reg);
                }
                else
                {
                    u4Reg = ReadREG(RW_VLD_SLICE_MARK_REG2);
                    u4Reg += ((i% wmv78_slice_num) == 0) << (i - 63);
                    WriteREG(RW_VLD_SLICE_MARK_REG2, u4Reg);
                }
            }
        }

        // DCAC part
        // 0
        if (rWMVDecInfo.rOther.i4CodecVersion == WMVA || rWMVDecInfo.rOther.i4CodecVersion == WMV3)  // VC-1 or WMV9
        {
            WriteREG(RW_WMV_DCAC_RULEB, rWMVDecInfo.rOther.use_rule_b);
        }
        else if (rWMVDecInfo.rOther.i4CodecVersion == WMV1 || rWMVDecInfo.rOther.i4CodecVersion == WMV2)  // WMV 7/8
        {
            UINT32 u4Reg;

#if 1
            u4Reg = 1 |
                    (rWMVDecInfo.rPicLayer.u1PicType == PVOP ? ((rWMVDecInfo.rOther.fgDCPred_IMBInPFrame ? 1 : 0) << 2) : 0) |
                    (rWMVDecInfo.rPicLayer.i4StepSize << 8);
#else
            u4Reg = rWMVDecInfo.rOther.use_rule_b;
            u4Reg |= ((rWMVDecInfo.rOther.fgDCPred_IMBInPFrame ? 1 : 0) << 2);
            u4Reg |= (rWMVDecInfo.rPicLayer.i4DCStepSize << 8);
#endif

            WriteREG(RW_WMV_DCAC_RULEB, u4Reg);
        }

        // 1
        INT32 i4Predictor;
        if (rWMVDecInfo.rPicLayer.i4DCStepSize > 0)
        {
            i4Predictor = (1024 + (rWMVDecInfo.rPicLayer.i4DCStepSize >> 1)) / (rWMVDecInfo.rPicLayer.i4DCStepSize);
        }
        WriteREG(RW_WMV_DCAC_PRED, i4Predictor);
    }
#endif

    WriteREG(RW_VLD_WMVDEC, 0x00000001);
    WriteREG(RW_VLD_WMVDEC, 0x00000000);

    return (TRUE);
}

//extern UINT32 rWMVDecInfo.rOther.u4BitCount;
BOOL fgVDecodeEndWMV(void)
{
    BOOL fgDecEnd = FALSE;
    //UINT32 dwMp2Mode;
    //UINT32 dwInputWnd1, dwInputWnd2;
    //UINT32 dMbX, dMbY;
    //UINT32 dwVldByte, dwVldBit;

#ifndef AVC_IRQ_DISABLE
    if (!rWMVDecInfo.rOther.fgRet)
    {
        if (x_sema_lock_timeout(hVdecWMVFinSema, 500) != OSR_OK) {
            LOG(1,"VC-1 decoder timeout\n");
            return FALSE;
        }
    }
    if (rWMVDecInfo.rOther.fgRet == TRUE)
    {
        return FALSE;
    }
    fgDecEnd = TRUE;
#else
#if 1
    fgDecEnd = ReadREG(RO_VLD_PIC_DEC_END) & 0x1;
    LOG(3,"fgDecEnd : %d\n",fgDecEnd);
#else  // 5365/5395 emulation test code, for reference only
    UINT32 u4Reg;
    UINT32 u4Cnt = 0;
    UINT32 u4Delay;

    do {
        u4Reg = ReadREG(RO_VLD_PIC_DEC_END);
        fgDecEnd = u4Reg/*ReadREG(RO_VLD_PIC_DEC_END)*/ & 0x1;
        //LOG(3,"fgDecEnd : %d\n",fgDecEnd);
        //printf("vdec_wmvdec 1239 RO_VLD_PIC_DEC_END = %d\n", u4Reg);
        if (++u4Cnt >= 10)
        {
            printf("vdec_wmvdec 1247 timeout!!!!!!!!!!!!\n");
            ASSERT(0);
            fgDecEnd = TRUE;
        }
        for (u4Delay = 0; u4Delay < 100000; u4Delay++);
    } while (!fgDecEnd);
#endif
#endif

#ifdef __MODEL_slt__
    //printf("#%02d: %08X %08X %08X %08X\n", rWMVDecInfo.rOther.u4CurrPicNo, ReadREG(RW_CRC_Y_0), ReadREG(RW_CRC_Y_1), ReadREG(RW_CRC_Y_2), ReadREG(RW_CRC_Y_3));
    //printf("#%02d: %08X %08X %08X %08X\n", rWMVDecInfo.rOther.u4CurrPicNo, ReadREG(RW_CRC_C_0), ReadREG(RW_CRC_C_1), ReadREG(RW_CRC_C_2), ReadREG(RW_CRC_C_3));
    if (rWMVDecInfo.rPesInfo.u4PTS == 33030)
    {
        if (fgDecEnd)
        {
            if (ReadREG(RW_CRC_Y_0) == 0x54A134C6 &&
                    ReadREG(RW_CRC_Y_1) == 0x6EBF46DC &&
                    ReadREG(RW_CRC_Y_2) == 0x796DE869 &&
                    ReadREG(RW_CRC_Y_3) == 0x1749F9EE &&
                    ReadREG(RW_CRC_C_0) == 0x1F08BB5F &&
                    ReadREG(RW_CRC_C_1) == 0x48C411AF &&
                    ReadREG(RW_CRC_C_2) == 0xEF862173 &&
                    ReadREG(RW_CRC_C_3) == 0x79D1637C)
            {
                Printf("\n\nMMOK!!\n\n\n");
                //vDumpWmvReg();
            }
            else
            {
                Printf("\n\nMMNG!!\n\n\n");
                //vDumpWmvReg();
            }
        }
        else
        {
            Printf("\n\nMMNG!!\n\n\n");
        }
    }
#endif

    if (fgDecEnd)
    {
#if 0
        if (!fgWaitVldFetchOk())
        {
            LOG(3,"fetch not ok\n");
            return(FALSE);
        }
        // load wmv-sum to mpeg2-sum
        //WriteREG(RW_VLD_116, 0x1);
        //WriteREG(RW_VLD_116, 0x0);
        // switch to mpeg2 mode
        //WriteREG(RW_VLD_WMV_MODE, 0x0);
        //fgDecEnd = ReadREG(RO_VLD_PIC_DEC_END);
#endif
        vVldByteAlign();
        rWMVDecInfo.rOther.u4BPRawFlag = 0;
        return(TRUE);
    }
    return(FALSE);

}


/** Initialize module for wmv
 */
void _WMVInit()
{
    vInitVDec();
    vInitVld();
    //vInitPred();
#if defined(CC_MT5365) || defined(CC_MT5395)
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x3);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x3);
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x2);
#endif
    UINT32 u4Temp = ReadREG(MC_REG_325);
    u4Temp |= (0x1000);
    WriteREG(MC_REG_325, u4Temp);
#ifdef __MODEL_slt__
    WriteREG(RW_CRC_CTRL, 0x1);
#endif
#else
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x4); //8520,1389 mode
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2); //5351 mode
#endif
#endif
}

/*
** wmv header parsing function
 */
void _getESInfo(void)
{
    //spork
    //rWMVDecInfo.rSeqHdrR.u4PicWidthSrc = rWMVDecInfo.rPesInfo.u4Width;
    //rWMVDecInfo.rSeqHdrR.u4PicHeightSrc = rWMVDecInfo.rPesInfo.u4Height;
    LOG(6,"my resolution = %d x %d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthSrc,rWMVDecInfo.rSeqHdrR.u4PicHeightSrc);
    rWMVDecInfo.rOther.rPicInfo.dwW = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
    rWMVDecInfo.rOther.rPicInfo.dwH = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
    rWMVDecInfo.rSeqHdrR.u4PicWidthDec = (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15) & ~15;
    rWMVDecInfo.rSeqHdrR.u4PicHeightDec = (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc + 15) & ~15;
    rWMVDecInfo.rSeqHdrR.u4NumMBX = rWMVDecInfo.rSeqHdrR.u4PicWidthDec >> 4;
    rWMVDecInfo.rSeqHdrR.u4NumMBY = rWMVDecInfo.rSeqHdrR.u4PicHeightDec >> 4;
    rWMVDecInfo.rSeqHdrR.u4PicWidthCmp = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
    rWMVDecInfo.rSeqHdrR.u4PicHeightCmp = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
    rWMVDecInfo.rOther.u4BitCount = rWMVDecInfo.rPesInfo.u4VldReadPtr<<3; // # of parsing bits
    //rWMVDecInfo.rOther.i4SeqHdrDataLen = rWMVDecInfo.rPesInfo.u4SeqHdrDataLen;
    //rWMVDecInfo.rOther.i4SeqHdrData1 = rWMVDecInfo.rPesInfo.u4SeqHdrData1;
}
void _WMVFreeMiscMem()
{
#ifndef USE_FBM
#ifndef CC_WMV_EMULATION
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Dcac);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Mv_1);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Mv_2);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Bp_1);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Bp_2);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Bp_3);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Bp_4);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Mv_3);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Mv_1_2);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Dcac_2);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Pp_1);
    BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Pp_2);
#endif
#endif

    //BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Pp_1);
    //BSP_FreeAlignedDmaMemory(rWMVDecInfo.rDram.pu1Pp_2);
}

void _WMVHdrPrs(INT32* pi4RetValue)
{
#ifdef CC_WMV_EMULATION
    extern BOOL _fgAbtCurItem;
#else
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);
#endif

#ifdef CC_WMV_EMULATION
    if (rWMVDecInfo.rPesInfo.ucPicType == 0xff)
    {
        _WMVFreeMiscMem();
#ifdef USE_FBM
        FBM_ReleaseGroup(rWMVDecInfo.rOther.u1FbgId);
#endif
        *pi4RetValue = E_VDEC_BS_END;
        return;
    }

    if (_fgAbtCurItem)
    {
        *pi4RetValue = E_VDEC_SKIPFRAME;
        return;
    }
#endif

#if 1
    //init dmx pes information and MM_set_parameter information
    rWMVDecInfo.rOther.i4SetPos = (rWMVDecInfo.rPesInfo.u4VldReadPtr - rWMVDecInfo.rPesInfo.u4FifoStart);
    rWMVDecInfo.rDram.pu1VFifo = (void*)rWMVDecInfo.rPesInfo.u4FifoStart;
    rWMVDecInfo.rDram.pu1FifoEnd = (void*)rWMVDecInfo.rPesInfo.u4FifoEnd;

#ifndef CC_WMV_EMULATION
    if (prVdecEsInfo != NULL && rWMVDecInfo.rPesInfo.u4VideoType != DMX_VIDEO_VC1)
        //if(prVdecEsInfo != NULL)
    {
        rWMVDecInfo.rSeqHdrR.u4PicWidthSrc =  (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
        rWMVDecInfo.rSeqHdrR.u4PicHeightSrc = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
        rWMVDecInfo.rOther.i4SeqHdrDataLen = (UINT32)prVdecEsInfo->rMMSeqInfo.u2SequenceHeaderSize;
        rWMVDecInfo.rOther.i4SeqHdrData1 = (UINT32)prVdecEsInfo->rMMSeqInfo.u4SequenceHeader;
    }
#else
    // vparser use _VdecVParserWMV to set decoder parameters
    if (rWMVDecInfo.rPesInfo.ucPicType == 0xff &&rWMVDecInfo.rPesInfo.fgEos)
    {
        return;
    }
#endif

    if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_VC1)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = TRUE;
        rWMVDecInfo.rOther.i4CodecVersion = WMVA;
    }
    else if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_WMV7)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = WMV1;
    }
    else if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_WMV8)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = WMV2;
    }
    else if (rWMVDecInfo.rPesInfo.u4VideoType == DMX_VIDEO_WMV9)
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = WMV3;
    }
    else
    {
        rWMVDecInfo.rSeqLayer.fgVC1 = FALSE;
        rWMVDecInfo.rOther.i4CodecVersion = UNKNOW_TYPE;
    }

    //LOG(0,"video type : %d\n",rWMVDecInfo.rOther.i4CodecVersion);
#endif

#if 0
    if (rWMVDecInfo.rOther.fgFirstFrame) //init state
    {
        LOG(6,"hello, it's first frame\n");
        _WMVInit();
        vInitVParser();
        vInitVParserWMV();
        vPowerOnVDec();

        rWMVDecInfo.rOther.fgDumpSimPat = FALSE;
        rWMVDecInfo.rOther.u4BPRawFlag = 0;
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
        {
            _getESInfo();
            if (rWMVDecInfo.rOther.i4CodecVersion == WMV3)
            {
                SetupMultiResParams();
                rWMVDecInfo.rOther.i4VprErr = decodeVOLHead_WMV3();
            }
            else if ((rWMVDecInfo.rOther.i4CodecVersion == WMV1) || (rWMVDecInfo.rOther.i4CodecVersion == WMV2))
            {
                rWMVDecInfo.rOther.i4VprErr = decodeVOLHead_WMV12();
            }
        }
        rWMVDecInfo.rOther.fgFirstFrame = FALSE;
    }
#endif

    if (fgVParserProcWMV())
    {
        if (rWMVDecInfo.rSMProfile.fgXintra8)
        {
            rWMVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
            //*pi4RetValue = VDEC_PLAY_HEADER_PARSE;
            *pi4RetValue = E_VDEC_NOT_SUPPORT;
            rWMVDecInfo.rSMProfile.fgXintra8 = FALSE;
            //_WMVSWRst(NULL);
            return;
        }
#ifdef CC_WMV_EMULATION
        else if (rWMVDecInfo.rPesInfo.fgSkipFrame)
        {
            rWMVDecInfo.rOther.u4DecPicNo++; //Robert 060608
            print_vop_header_parameter(/*0,*/ rWMVDecInfo.rOther.u4DecPicNo, rWMVDecInfo.rDram.u4FRefBufIdx, rWMVDecInfo.rDram.u4DecBufIdx);
            //vSetVSyncPrmBufPtr(rWMVDecInfo.rDram.u4DecBufIdx);
            rWMVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
            *pi4RetValue = E_VDEC_SKIPFRAME;
            return;
        }
#endif

        if (cbWMVHeaderOnly)
        {
            if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
            {
                vVldByteAlign(); //in order to use fgNextStartCode().
            }
            else
            {
                fgVldInitBarrelShifter(((UINT32)rWMVDecInfo.rDram.pu1VFifo) + rWMVDecInfo.rOther.i4SetPos);
            }
            rWMVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
        }
        else
        {
            rWMVDecInfo.rOther.u4VldPosByte = dwVldRPtr(&rWMVDecInfo.rOther.u4VldPosBit);
            if (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)
            {
                rWMVDecInfo.rOther.u4DecPicNo++; //Robert 060608
                print_vop_header_parameter(/*0,*/ rWMVDecInfo.rOther.u4DecPicNo, rWMVDecInfo.rDram.u4FRefBufIdx, rWMVDecInfo.rDram.u4DecBufIdx);
                rWMVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
                *pi4RetValue = E_VDEC_SKIPFRAME;
                //_WMVSWRst(NULL,NULL);
                //Printf("SKIPFRAME, %d\n", rWMVDecInfo.rPicLayer.u1PicType);
                vSetVSyncPrmBufPtr(rWMVDecInfo.rDram.u4DecBufIdx);
                if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
                {
                    if (cbResetVDec)
                    {
                        //Software reset
                        vResetVDec();
                        //If you perform vResetVDec(), you need to do the following things.
                        vSetMcBufPtr();
                        vSetVldVFifo();
                        vInitPred();
                    }
                    fgVldInitBarrelShifter(((UINT32)rWMVDecInfo.rDram.pu1VFifo) + rWMVDecInfo.rOther.i4SetPos);
                    //fgVldInitBarrelShifter(rWMVDecInfo.rOther.i4SetPos);
                }
                else
                {
                    vVldByteAlign(); //in order to use fgNextStartCode().
                    if (cbResetVDec)
                    {
                        //Software reset
                        vResetVDec();
                        //If you perform vResetVDec(), you need to do the following things.
                        vSetMcBufPtr();
                        vSetVldVFifo();
                        vInitPred();
                    }
                    fgVldInitBarrelShifter(((UINT32)rWMVDecInfo.rDram.pu1VFifo) + rWMVDecInfo.rOther.u4VldPosByte);
                }
            }
            else
            {
                rWMVDecInfo.rOther.u1DecState = VDEC_NORM_WAIT_TO_DEC;
                *pi4RetValue = E_VDEC_OK;
            }
        }
    }
    else
    {
        if (rWMVDecInfo.rOther.i4VprErr == END_OF_FILE)
        {
            rWMVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
            *pi4RetValue = E_VDEC_FAIL;
            //_WMVSWRst(NULL,NULL);
        }
        else if (rWMVDecInfo.rOther.i4VprErr == E_VDEC_PARSE_NOT_READY)
        {
            *pi4RetValue = E_VDEC_PARSE_NOT_READY;
        }
        else
        {
            rWMVDecInfo.rOther.u1DecState = VDEC_NORM_ERROR;
            *pi4RetValue = E_VDEC_FAIL;
            //_WMVSWRst(NULL,NULL);
        }
    }
}

/** wmv decoder hardware config and decoding trigger
*@param hVdec handler
 */
void _WMVDecStart(INT32* pi4RetValue)
{
    LOG(6,"Wait to Dec, no: %d, type: %d\n", rWMVDecInfo.rOther.u4CurrPicNo, rWMVDecInfo.rPicLayer.u1PicType);
    if (fgVDecProcWMV())
    {
        rWMVDecInfo.rOther.u1DecState = VDEC_NORM_WAIT_DECODE;
        rWMVDecInfo.rOther.u4Test01 = 0;
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
void _WMVDecFin(INT32* pi4RetValue)
{
    UINT32 dwVldByte, dwVldBit;
    //UINT32 u4Tmp =0;
    //BOOL static fgTimeout = FALSE;
    //UINT8 static u1Times =0;
    do {
        if (fgVDecodeEndWMV())
        {
            PostAdjustReconRange();
            vSetVSyncPrmBufPtr(rWMVDecInfo.rDram.u4DecBufIdx);
            //u1Times =0;
            //vVSyncWriteVdoReg();
            if (rWMVDecInfo.rOther.i4CodecVersion != WMVA)
            {
                if (cbResetVDec) //TODO: add for Advanced Profile.
                {
                    vResetVDec();
                    //If you perform vResetVDec(), you need to do the following things.
                    vSetMcBufPtr();
                    vSetVldVFifo();
                    vInitPred();
                    rWMVDecInfo.rOther.u4Datain=dwGetBitStream(0);
                    fgVldInitBarrelShifter(((UINT32)rWMVDecInfo.rDram.pu1VFifo) + rWMVDecInfo.rOther.i4SetPos);
                    rWMVDecInfo.rOther.u4Datain=dwGetBitStream(0);
                }
                else
                {
                    //Set VLD RPtr to the position of the next picture.
                    fgVldInitBarrelShifter(((UINT32)rWMVDecInfo.rDram.pu1VFifo) + rWMVDecInfo.rOther.i4SetPos);
                    //fgVldInitBarrelShifter(rWMVDecInfo.rOther.i4SetPos);
                    LOG(6,"init barrel shifter2 %08x\n",rWMVDecInfo.rOther.i4SetPos);
                }
            }
            else // WMVA
            {
                if (cbResetVDec)
                {
                    dwVldByte = dwVldRPtr(&dwVldBit);
                    vResetVDec();
                    //If you perform vResetVDec(), you need to do the following things.
                    vSetMcBufPtr();
                    vSetVldVFifo();
                    vInitPred();
                    fgVldInitBarrelShifter(((UINT32)rWMVDecInfo.rDram.pu1VFifo) + dwVldByte);
                }
            }
            rWMVDecInfo.rOther.u1DecState = VDEC_NORM_DEC_END;
            if ((rWMVDecInfo.rPicLayer.u1FrameCodingMode != INTERLACEFIELD)||(rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1))
            {
                if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    rWMVDecInfo.rPicLayer.i4CurrentTemporalField ^= 1;  //toggle field
                    rWMVDecInfo.rPicLayer.i4CurrentField ^= 1;
                }
                rWMVDecInfo.rOther.u4CurrPicNo++;
                *pi4RetValue = E_VDEC_OK;
            }
            else
            {
                *pi4RetValue = E_VDEC_DISP_NOT_READY;
                if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    rWMVDecInfo.rPicLayer.i4CurrentTemporalField ^= 1;  //toggle field
                    rWMVDecInfo.rPicLayer.i4CurrentField ^= 1;
                }
            }
            LOG(6,"decode finish RPTR : 0x%08x,WPTR : 0x%08x\n",(rWMVDecInfo.rPesInfo.u4FifoStart+dwVldRPtr(&dwVldBit)),ReadREG(RO_VLD_VWPTR));
            LOG(6,"decode finish Rptr 0x%08x, input win 0x%08x\n", dwVldRPtr(&dwVldBit), ReadREG(RO_VLD_BARL));
            return;
        }
        else
        {
            //UINT32 u4Idx =0;
            //*pi4RetValue = E_VDEC_FAIL;
            *pi4RetValue = E_VDEC_OK;
            vSetVSyncPrmBufPtr(rWMVDecInfo.rDram.u4DecBufIdx);
            if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            {
                LOG(1,"frame type = IVOP\n");
            }
            else if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            {
                LOG(1,"frame type = BVOP\n");
            }
            else if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
            {
                LOG(1,"frame type = PVOP\n");
            }
            else if (rWMVDecInfo.rPicLayer.u1PicType == BIVOP)
            {
                LOG(1,"frame type = BIVOP\n");
            }
            else
            {
                LOG(1,"frame type = skip frame\n");
            }

            LOG(1,"MBX : %d \nMBY : %d \n",ReadREG(RO_MC_MBX),ReadREG(RO_MC_MBY));
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
#if 0
            printf("dump VLD state for timeout\n\n");
            for (u4Idx =100 ; u4Idx<=250 ; u4Idx++)
            {
                printf("RISC Read VLD(%d) = 0x%08x \n", u4Idx,ReadREG(VLD_REG_OFST + u4Idx*4));
            }
            printf("\n\ndump VLD state for timeout\n");
#endif
            //fgTimeout = FALSE;
            return;
        }
    } while (1);
}

/** wmv SW reset
*@param hVdec handler
 */
void _WMVSWRst(INT32* pi4RetValue)
{
    UINT32 r52;
    vResetVDec();
    vSetMcBufPtr();
    vSetVldVFifo();
    vInitPred();
#if defined(CC_MT5365) || defined(CC_MT5395)
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x3);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x3);
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x2);
#endif
    UINT32 u4Temp = ReadREG(MC_REG_325);
    u4Temp |= (0x1000);
    WriteREG(MC_REG_325, u4Temp);
#else
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x4); //8520,1389 mode
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2);//5351 mode
#endif
#endif
    r52 = ReadREG(VLD_REG_OFST + 0xd0);
    r52 = r52 | (1<<17);
    WriteREG(VLD_REG_OFST + 0xd0,r52);

    *pi4RetValue = E_VDEC_OK;
}

/** wmv decode stop / freeze
*@param hVdec handler
 */
void _WMVStop(VDEC_HDLR hVdec)
{
}

/** get pes struct pointer
*@param .
 */
void _WMVGetPesStruct(VDEC_HDLR hVdec, void **ppvPesInfo)
{
}

/** get display picture addr
*@param .
 */
void _WMVGetDispPic(VDEC_HDLR hVdec, void* pvInfo)
{
}


// *** WMV End ***
