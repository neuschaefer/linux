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
#include "srm_drvif.h"
#ifdef USE_FBM
#include "fbm_drvif.h"
#endif

#define VDEC_MM_WAIT_DISP_TIME 1000
#define WMV_DEFAULT_FRAME_RATE (30000)


#ifndef AVC_IRQ_DISABLE
#include "x_os.h"
HANDLE_T hVdecWMVFinSema = 0;
UCHAR ucVLDISREsId;
#endif


#ifdef VDEC_TIME_PROFILE
static HAL_TIME_T _rTimeRstS, _rTimeRstE, _rTimeRstDt;

static UINT32 u4DelayClock;
static UINT32 u4DramCycle;
#endif


#ifndef AVC_IRQ_DISABLE
#define WMVVLD_VECTOR 24

void _WMVVLDIsr(UINT16 u2Vector)
{
    LOG(6,"----------------------entering WMVVLDISR\n\n");

#ifdef VDEC_TIME_PROFILE
    _rTimeRstDt.u4Micros = 0;
#endif

    if (u2Vector == WMVVLD_VECTOR)
    {
        //BOOL fgEnable = FALSE;
        if (ReadREG(ucVLDISREsId,RO_VLD_PIC_DEC_END) & PIC_DEC_FINISH)
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
                    printf("RISC Read VLD(%d) = 0x%08x \n", u4Idx,ReadREG(ucEsId,VLD_REG_OFST + u4Idx*4));
                }
                printf("\n\ndump VLD state for timeout\n");

                printf("dump MC state for timeout\n\n");
                for (u4Idx =0 ; u4Idx<=200 ; u4Idx++)
                {
                    printf("RISC Read MC(%d) = 0x%08x \n", u4Idx,ReadREG(ucEsId,MC_REG_OFST + u4Idx*4));
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
                    printf("RISC Read MC checksum(%d) = 0x%08x \n", u4Idx,ReadREG(ucEsId,MC_REG_OFST + u4Idx*4));
                }
                printf("\n\ndump MC checksum for timeout\n");
            }
#endif

#ifdef IS_WMV_POST_MT5368
            {
#if 0
                WriteREG(MISC_REG_41, 0x1011); // clr
                WriteREG(MISC_REG_41, 0x1001); // mode
#else
                BIM_ClearIrq(WMVVLD_VECTOR);
#endif
            }
#endif

            //unlock semaphore
            VERIFY(x_sema_unlock(hVdecWMVFinSema) == OSR_OK);
            //rWMVDecInfo[ucVLDISREsId].rOther.fgRet = FALSE;
        }
        else if (ReadREG(ucVLDISREsId,RO_VLD_BP_DEC_END) & BD_DEC_FINISH)//ucEsId
        {
            // never use it. to change pool to use this.
            WriteREG(RW_VLD_DECSTART, (UINT32)(1<<8));
            WriteREG(RW_VLD_DECSTART, (UINT32)(0<<8));
        }
#ifdef WMV_VP_MODE
        else if (ReadREG(ucVLDISREsId, RO_VLD_VP_MODE) & 0x1)
        {
        	LOG(3, "VP-mode done !!!\n");
        	BIM_ClearIrq(WMVVLD_VECTOR);
        	//unlock semaphore
        	VERIFY(x_sema_unlock(hVdecWMVFinSema) == OSR_OK);
        }
#endif
        else if (ucVLDISREsId < MAX_ES_NUM)
        {
            LOG(0,"//=== Interrupt with no status? ===\n");
            ASSERT(0);
            rWMVDecInfo[ucVLDISREsId].rOther.fgRet = TRUE;  //maiyou
        }
        else
        {
            ASSERT(0);
        }
    }
}
#endif


VOID vPutDispQForB(UCHAR ucEsId, UCHAR ucFbIdX)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    VDEC_ES_INFO_T* prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    UCHAR ucFbId = (ucFbIdX == FBM_FB_ID_UNKNOWN ? prVdec->rOther.ucLastBFbId : ucFbIdX);

#ifdef WMV_PRINT_FBM_STATUS
    {
        UCHAR u1state = FBM_GetFrameBufferStatus(prVdec->rOther.u1FbgId,ucFbId);
        LOG(0,"WMV set B frame FBM status : %d %d DISPALY_Q\n",ucFbId,u1state);
    }
#endif
        
#ifdef CC_REALD_3D_SUPPORT
    _VDEC_Check2D3DSwitch(ucEsId, ucFbId);
#endif
    _VDEC_JobBeforePutDispQ(ucEsId, ucFbId);
    
    // to avoid FBM assert
    if (ucFbIdX == FBM_FB_ID_UNKNOWN)
        FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId, ucFbId, FBM_FB_STATUS_DECODE);
    
    FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId, ucFbId, FBM_FB_STATUS_DISPLAYQ);
    LOG(5, "\n\n[TRACKING] decoder set EARLY dispQ fb id = %d\n\n", ucFbId);
    prVdecEsInfo->u4DisplayQPicCnt++;
}


#ifdef USE_FBM
void SetPicHdr(UCHAR ucEsId,UCHAR u1FbId)
{
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;
    VDEC_ES_INFO_T* prVdecEsInfo;
    VDEC_WMV_INFO_T* prVdec;
#if 0
    BOOL fgNotDisplay = FALSE;
#endif
    prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rOther.u1FbgId, u1FbId);
    ASSERT(prFbmPicHdr != NULL);
    ASSERT(prVdecEsInfo != NULL);
    if (prFbmPicHdr == NULL || prVdecEsInfo==NULL)
    {
        LOG(3,"WMV SetPicHdr warning : prFbmPicHdr == NULL || prVdecEsInfo ==NULL\n");
        return;
    }
    //UINT8 u1RepeatFrameCount;  //(RPTFRM) need to add in
    if (prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE)
    {
        prFbmPicHdr->fgProgressiveFrm = TRUE;
    }
    else
    {
        prFbmPicHdr->fgProgressiveFrm = FALSE;
        //prFbmPicHdr->fgProgressiveFrm = TRUE;
    }
    prFbmPicHdr->fgProgressiveFrm = TRUE;
    //prFbmPicHdr->fgProgressiveFrm = prVdec->rPicLayer.u1FrameCodingMode;
    prFbmPicHdr->fgRepFirstFld = prVdec->rPicLayer.fgRepeatFirstField;
    prFbmPicHdr->fgTopFldFirst = prVdec->rPicLayer.fgTopFieldFirst;
    //prFbmPicHdr->ucPicStruct = prVdec->rPicLayer.

    if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_NETFLIX || prVdecEsInfo->fgSeamlessPlay)
    {
        prFbmPicHdr->fgSeamlessDisp = TRUE;
        prFbmPicHdr->u4PicWidth = prVdec->rSeqHdrR.u4PicWidthSrc;
        prFbmPicHdr->u4PicWidthPitch = ((prVdec->rSeqHdrR.u4PicWidthDec + 63) / 64 * 64);//prVdec->rSeqHdrR.u4PicWidthDec;
        prFbmPicHdr->u4PicHeight = prVdec->rSeqHdrR.u4PicHeightSrc;
    }

    prFbmPicHdr->ucTrueZeroPTS = FBM_USE_ZERO_PTS_IF_ZERO;

    //prFbmPicHdr->u8OffsetI = prVdec->rPesInfo.u8OffsetI;
    //prFbmPicHdr->u8Offset = prVdec->rPesInfo.u8Offset;

    if(prVdec->rPicLayer.u1PicType==IVOP)
    {
        if((!prVdec->rSeqLayer.fgTemporalFrmCntr)&&(prVdecEsInfo->eContainerType==SWDMX_FMT_VC1_ES
            ||prVdecEsInfo->eContainerType==SWDMX_FMT_MPEG2_TS_192
            ||prVdecEsInfo->eContainerType==SWDMX_FMT_MPEG2_TS
            ||prVdecEsInfo->eContainerType==SWDMX_FMT_MPEG2_TS_ZERO_192
            ||prVdecEsInfo->eContainerType==SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
               prVdec->rPicLayer.i4TemporalRef=0;
        }
    }

    prFbmPicHdr->i4TemporalRef = prVdec->rPicLayer.i4TemporalRef;
    prFbmPicHdr->u4TotlaTimeOffset = prVdecEsInfo->u4TotlaTimeOffset;

    prVdecEsInfo->ucFbgId = prVdec->rOther.u1FbgId;
    prVdecEsInfo->ucFbId = u1FbId;
    
    x_memcpy(&prFbmPicHdr->rExtra,
        &prVdecEsInfo->rExtra,
        sizeof(FBM_PIC_EXTRA_INFO_T));
    
    if(prVdecEsInfo->eContainerType==SWDMX_FMT_MKV)
    {
       if(prVdec->rOther.u4LastClustPos!=prVdecEsInfo->u8OffsetI)
       {
          prVdec->rOther.u4LastClustPts=prVdec->rPesInfo.u4PTS; 
          prVdec->rOther.u4LastClustPos=prVdecEsInfo->u8OffsetI;
       }
       
       if(prVdec->rPicLayer.u1PicType==IVOP)
       {
          prVdec->rOther.u4ClusterPos=prVdecEsInfo->u8OffsetI;
          prVdec->rOther.u4LastIFrameClustPts=prVdec->rOther.u4LastClustPts;
          
       }
       
       prFbmPicHdr->u8OffsetI=prVdec->rOther.u4ClusterPos;
       prFbmPicHdr->u4Timestap = prVdecEsInfo->u4Timestap;
       prFbmPicHdr->rExtra.u8PTSI=prVdec->rOther.u4LastIFrameClustPts;

       if(prVdec->rPesInfo.fgDtsValid==FALSE 
         &&(prVdec->rPicLayer.u1PicType==IVOP || prVdec->rPicLayer.u1PicType==PVOP))
        {
           prVdec->rPesInfo.u4PTS=prVdec->rOther.u4LastPTS;
        }
        
    }
    else
    {
       prFbmPicHdr->u8OffsetI = prVdecEsInfo->u8OffsetI;
       prFbmPicHdr->u4Timestap = prVdecEsInfo->u4Timestap;
    }
    
    prFbmPicHdr->u4PTS = prVdec->rPesInfo.u4PTS;
    prFbmPicHdr->u8Offset = prVdecEsInfo->u8Offset;
    prFbmPicHdr->u8OffsetDisp = prVdecEsInfo->u8OffsetLast;
    prFbmPicHdr->u2DecodingOrder = prVdecEsInfo->u2DecodingOrder;


#if 0
    fgNotDisplay = VDEC_IsNotDisplay(ucEsId, prVdec->rPesInfo.u4PTS, 0, 0);
    if (fgNotDisplay)
    {
        FBM_SetFrameBufferPicFlag(prVdec->rOther.u1FbgId,
                                  u1FbId,
                                  FBM_MM_NOT_DISPLAY_FLAG);

        LOG(6, "_PrsVOPHdr fgNotDisplay ucFbId %d\n", u1FbId);
    }
#endif

    /*if(prVdec->rPicLayer.u1PicType== IVOP)
        LOG(0,"I frame , PTS valuse : %x\n",prVdec->rPesInfo.u4PTS);
    else if(prVdec->rPicLayer.u1PicType== PVOP)
        LOG(0,"P frame , PTS valuse : %x\n",prVdec->rPesInfo.u4PTS);
    */
#ifdef CC_RESIZE_SMALL_IMAGE
    prFbmPicHdr->u4PicWidth = prVdec->rSeqHdrR.u4PicWidthSrc;
    prFbmPicHdr->u4PicHeight = prVdec->rSeqHdrR.u4PicHeightSrc;
    //prFbmPicHdr->u4PicWidthPitch = prVdec->rOther.u4LineSize;//prVdec->rSeqHdrR.u4PicWidthDec;
    // for 3d wmv patch, wmv refine pitch to 64 align with original width
    prFbmPicHdr->u4PicWidthPitch = ((prVdec->rSeqHdrR.u4PicWidthDec + 63) / 64 * 64);//prVdec->rSeqHdrR.u4PicWidthDec;
#endif
}

void SetSeqHdr(UCHAR ucEsId)
{

    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (prVdec->rOther.u1GetSeqHdr == 0)
    {
        FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
        VDEC_ES_INFO_T *prVdecEsInfo;
        VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
        UINT32 u4fps = 0;
        UINT32 u4DispAspectRatio = 1;
        UINT32 u4DecoderWidth;
        UINT32 u4DecoderHeight;

        prVdecEsInfo = _VDEC_GetEsInfo(ucEsId); 
        prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(ucEsId);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdec->rOther.u1FbgId);

        ASSERT(prFbmSeqHdr != NULL && prVdecEsInfo!=NULL);

        if (prFbmSeqHdr == NULL || prVdecEsInfo==NULL)
        {
            LOG(3,"WMV SetSeqHdr warning : prFbmSeqHdr == NULL || prVdecEsInfo==NULL \n");
            return;
        }

        prFbmSeqHdr->ucAspRatInf = MPEG_ASPECT_RATIO_1_1;//prVdec->rSeqLayer.i4Aspect_Ratio;//;        // 16:9
        //prFbmSeqHdr->ucFrmRatCod = 5;        // 30 fps
        prFbmSeqHdr->fgProgressiveSeq = 1;//!prVdec->rSeqLayer.fgInterlacedSource;//1;//(prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE);
        u4DecoderWidth = prVdec->rSeqHdrR.u4PicWidthSrc;
        u4DecoderHeight = prVdec->rSeqHdrR.u4PicHeightSrc;

        if (prVdec->rOther.i4CodecVersion == WMVA)
        {
            if (prVdec->rSeqLayer.i4FrameRateNr !=0 && prVdec->rSeqLayer.i4FrameRateDr !=0)
            {
                UINT64 u8FrameRate;
                UINT64 u8FrameRateScale;
                UINT64 u8Remainder;
                if (prVdec->rSeqLayer.i4FrameRateDr ==1)
                {
                    u8FrameRateScale = 1000;
                }
                else if (prVdec->rSeqLayer.i4FrameRateDr ==2)
                {
                    u8FrameRateScale = 1001;
                }
                else
                {
                    LOG(3,"WMV decoder warning : prVdec->rSeqLayer.i4FrameRateDr = %d\n",prVdec->rSeqLayer.i4FrameRateDr);
                    u8FrameRateScale = 1000;
                }
                switch (prVdec->rSeqLayer.i4FrameRateNr)
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
                UINT64 u8Remainder;
                prVdec->rSeqLayer.i4FrameRateDr = 32;
                u4fps = (UINT32)u8Div6432((UINT64)prVdec->rSeqLayer.i4FrameRateNr, (UINT64)prVdec->rSeqLayer.i4FrameRateDr, &u8Remainder);
                u4fps*=1000;
                if (u4fps == 0)
                {
                   LOG(1,"wmv decoder without frame rate syntax information");
                }
               
            }
        }
        else if (prVdec->rOther.i4CodecVersion <= WMV3)
        {
            u4fps = prVdec->rSeqLayer.i4FrameRate;
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
            prVdec->rOther.u4DeltaPTS = (90000000/u4fps);
            u4fps /= 1000;
        }
        else
        {
            prVdec->rOther.u4DeltaPTS = (90000000/WMV_DEFAULT_FRAME_RATE);
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
        case 26:
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

        //prFbmSeqHdr->ucAspRatInf = prVdec->rSeqLayer.i4Aspect_Ratio; // 16:9
        if (prVdec->rSeqLayer.fgProgressiveDisp)
        {
            prFbmSeqHdr->fgProgressiveSeq = TRUE;
        }
        else
        {
            if (!prVdec->rSeqLayer.fgInterlacedSource)
            {
                prFbmSeqHdr->fgProgressiveSeq = TRUE;
            }
            else
            {
                prFbmSeqHdr->fgProgressiveSeq = FALSE;
            }
        }
        //LOG(0,"prFbmSeqHdr->fgProgressiveSeq : %d\n",prFbmSeqHdr->fgProgressiveSeq);

        switch (prVdec->rSeqLayer.i4Aspect_Ratio)
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

	    prFbmSeqHdr->u4BitRatVal=prVdec->rSeqLayer.i4BitRate*1000;
		
        if (prVdec->rOther.i4CodecVersion <= WMV3)
        {
            if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_NETFLIX || prVdecEsInfo->fgSeamlessPlay)
            {
                UINT32 u4Tmp;
                UINT32 u4Width = FBM_IMG_RESIZE_NETFLIX_WIDTH;
                UINT32 u4Height = FBM_IMG_RESIZE_NETFLIX_HEIGHT;

                if(prVdecEsInfo->fgSeamlessPlay)
                {
                    u4Width = prVdecEsInfo->u4SeamlessWidth;
                    u4Height = prVdecEsInfo->u4SeamlessHeight;
                }
                
                prFbmSeqHdr->u2LineSize = (UINT16)u4Width;
                prFbmSeqHdr->u2HSize = (UINT16)u4Width;
                prFbmSeqHdr->u2VSize = (UINT16)u4Height;
                prFbmSeqHdr->u2OrgHSize = (UINT16)u4Width;
                prFbmSeqHdr->u2OrgVSize = (UINT16)u4Height;
                prFbmSeqHdr->u4ResizeWidth = u4Width;
                prFbmSeqHdr->u4ResizeHeight = u4Height;
                if (u4DispAspectRatio < u4Width*1000/u4Height)
                {
                    u4Tmp = (u4DispAspectRatio*u4Height)/1000;
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = u4Tmp;
                    prFbmSeqHdr->u4ResizeHeight = u4Height;
                }
                else
                {
                    u4Tmp = (u4Width*1000/u4DispAspectRatio);
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = u4Width;
                    prFbmSeqHdr->u4ResizeHeight = u4Tmp;
                }
            }
            else
            {
#ifdef CC_RESIZE_SMALL_IMAGE
                if ((prVdec->rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION ||
                        prVdec->rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION) &&
                    (!((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))))
                {
                    prFbmSeqHdr->fgResizeSmallPic = TRUE;

                    if (prVdec->rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (prVdec->rSeqHdrR.u4PicWidthSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthDec;
                        prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                    }

                    if (prVdec->rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (prVdec->rSeqHdrR.u4PicHeightSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                    }
                }
                else
                {
                    #ifndef CC_WMV_EMULATION  // 64 align
                    prFbmSeqHdr->u2LineSize = (UINT16)((prVdec->rSeqHdrR.u4PicWidthDec + 63) / 64 * 64);
                    //prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rOther.u4LineSize;
                    #else
                    prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthDec;
                    #endif
                    prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                }
#else
                prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthDec;
                prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
#endif
            }
        }
        else /* UMV Padding directly from picture boundary */
        {
            UINT32 width,height;
            FBM_GetFrameBufferSize(prVdec->rOther.u1FbgId,&width,&height);
            //LOG(0,"fbm resolution %dx%d\n",width,height);
            //LOG(0,"true resolution %dx%d\n",prVdec->rSeqHdrR.u4PicWidthSrc,prVdec->rSeqHdrR.u4PicHeightSrc);
            //LOG(0,"alignment resolution %dx%d\n",prVdec->rSeqHdrR.u4PicWidthDec,prVdec->rSeqHdrR.u4PicHeightDec);
            
            if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_NETFLIX || prVdecEsInfo->fgSeamlessPlay)
            {
                UINT32 u4Tmp;
                UINT32 u4Width = FBM_IMG_RESIZE_NETFLIX_WIDTH;
                UINT32 u4Height = FBM_IMG_RESIZE_NETFLIX_HEIGHT;

                if(prVdecEsInfo->fgSeamlessPlay)
                {
                    u4Width = prVdecEsInfo->u4SeamlessWidth;
                    u4Height = prVdecEsInfo->u4SeamlessHeight;
                }
                
                prFbmSeqHdr->u2LineSize = (UINT16)u4Width;
                prFbmSeqHdr->u2HSize = (UINT16)u4Width;
                prFbmSeqHdr->u2VSize = (UINT16)u4Height;
                prFbmSeqHdr->u2OrgHSize = (UINT16)u4Width;
                prFbmSeqHdr->u2OrgVSize = (UINT16)u4Height;
                prFbmSeqHdr->u4ResizeWidth = u4Width;
                prFbmSeqHdr->u4ResizeHeight = u4Height;
                if (u4DispAspectRatio < u4Width*1000/u4Height)
                {
                    u4Tmp = (u4DispAspectRatio*u4Height)/1000;
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = u4Tmp;
                    prFbmSeqHdr->u4ResizeHeight = u4Height;
                }
                else
                {
                    u4Tmp = (u4Width*1000/u4DispAspectRatio);
                    u4Tmp = VDEC_ALIGN_MASK(u4Tmp,15);
                    prFbmSeqHdr->u4ResizeWidth = u4Width;
                    prFbmSeqHdr->u4ResizeHeight = u4Tmp;
                }
            }

            else
            {
#ifdef CC_RESIZE_SMALL_IMAGE
                if ((prVdec->rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION ||
                        prVdec->rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION) &&
                    (!((prVdecEsInfoKeep->fgVPush) && (prVdecEsInfoKeep->fgVPushDecodeOnly))))
                {
                    prFbmSeqHdr->fgResizeSmallPic = TRUE;

                    if (prVdec->rSeqHdrR.u4PicWidthSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (prVdec->rSeqHdrR.u4PicWidthSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2HSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthDec;
                        prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                        prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                    }

                    if (prVdec->rSeqHdrR.u4PicHeightSrc < FBM_IMG_RESIZE_LIMITATION)
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION;
                    }
                    else if (prVdec->rSeqHdrR.u4PicHeightSrc > FBM_IMG_RESIZE_LIMITATION_MAX)
                    {
                        //prFbmSeqHdr->u2LineSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2VSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)FBM_IMG_RESIZE_LIMITATION_MAX;
                    }
                    else
                    {
                        prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                        prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                    }
                }
                else
                {
                    #ifndef CC_WMV_EMULATION  // 64 align
                    prFbmSeqHdr->u2LineSize = (UINT16)((prVdec->rSeqHdrR.u4PicWidthDec + 63) / 64 * 64);
                    //prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rOther.u4LineSize;
                    #else
                    prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthDec;
                    #endif
                    prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                    prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                    prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                }
#else
                prFbmSeqHdr->u2LineSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthDec;
                prFbmSeqHdr->u2HSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2VSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
                prFbmSeqHdr->u2OrgHSize = (UINT16)prVdec->rSeqHdrR.u4PicWidthSrc;
                prFbmSeqHdr->u2OrgVSize = (UINT16)prVdec->rSeqHdrR.u4PicHeightSrc;
#endif
            }
        }
        prVdec->rOther.u1GetSeqHdr = 1;
        
#if 0 /* need mentor's double check, follow Jason's mail: "About CR review on 20110316", B2R/VDEC's linesize must be 64 px. alignment */
		prFbmSeqHdr->u2LineSize = (((prFbmSeqHdr->u2LineSize + 63) >> 6) << 6);
#endif
    }
}
#endif


#ifdef WMV_VP_MODE
void _WMVDecVP(UCHAR ucEsId, UINT32 u4DstY, UINT32 u4DstC, UINT32 u4SrcY, UINT32 u4SrcC, UINT32 u4Width, UINT32 u4Height)
{
    int height;
    int width;
    int a36;
    int delay;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);

    printf("***************************************************************************\n");
    printf("[wmv]VP mode: DstY=%08X DstC=%08X SrcY=%08X SrcC=%08X\n", u4DstY, u4DstC, u4SrcY, u4SrcC);
    printf("      width = %d height = %d\n", u4Width, u4Height);
    printf("***************************************************************************\n");
    
    x_memset((void *)u4DstY, 0, u4Width * u4Height);
    x_memset((void *)u4DstC, 0, u4Width * u4Height / 2);
    
    vResetVDec(ucEsId);
    WriteREG(RW_VLD_WMV_MODE, 0x00000000); // Step 9 : change to mpeg2 mode

#if defined(IS_WMV_POST_MT5368)
    WriteREG(MISC_REG_41, ReadREG(ucEsId, MISC_REG_41) & 0xFFFFEFFF);
#endif

    WriteREG(RW_MC_R1Y, PHYSICAL(u4SrcY) >> 9); // div 512
    WriteREG(RW_MC_R1C, PHYSICAL(u4SrcC) >> 8); // div 256
    WriteREG(RW_MC_R2Y, PHYSICAL(u4DstY) >> 9); // div 512
    WriteREG(RW_MC_R2C, PHYSICAL(u4DstC) >> 8); // div 256
    WriteREG(RW_MC_OPBUF, 1);
    WriteREG(RW_MC_FWDP, 0);

#if defined(IS_WMV_POST_MT5368)
    WriteREG(RW_MC_PITCH, (u4Width + 15) >> 4);
#endif

    WriteREG(RW_MC_UMV_PIC_WIDTH, u4Width);    // 0x208
    WriteREG(RW_MC_UMV_PIC_HEIGHT, u4Height);  // 0x20C

    height = (u4Height + 15) >> 4;
    width = (u4Width + 15) >> 4;

#if defined(IS_WMV_POST_MT5368)

#if 1 /* sy's modify */
	WriteREG(RW_VLD_PSUPCTR, (height * width)+1);   // 0xAC //sy's modify
#else
    WriteREG(RW_VLD_PSUPCTR, (height << 16) | width);   // 0xAC
#endif

    WriteREG(RW_VLD_MBROWPRM, height - 1);  // 0x94
#else
    WriteREG(RW_VLD_PSUPCTR, ((prVdec->rSeqHdrR.u4PicWidthDec * height/*prVdec->rSeqHdrR.u4PicHeightDec*/) >> 8) + 1);
    WriteREG(RW_VLD_MBROWPRM, (((height/*prVdec->rSeqHdrR.u4PicHeightDec*/ + 15) >> 4) - 1) << 16);
#endif
    WriteREG(RW_VLD_PARA, 0xC0500000);  // 0x88, Frame Picture + VP ???

#if defined(IS_WMV_POST_MT5368)
    height = height << 4;
    width = width << 4;
#else
    a36 = (height << 16) + (prVdec->rSeqHdrR.u4NumMBY);
#endif
    WriteREG(RW_VLD_PICSZ, height << 16);  // 0x90
#if defined(IS_WMV_POST_MT5368)
    WriteREG(RW_VLD_TOP_PIC_PIX_SIZE, (height << 16) | width);    // 0x70 (28*4)
#endif

    // addr swap mode
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(IS_WMV_POST_MT5368)
#ifdef CC_WMV_EMULATION
    //WriteREG(RW_MC_ADDRSWAP, 0x3);
    //WriteREG(RW_MC_NBM_ADDRSWAP, 0x3);
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x2);
#endif
#else
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x4); //8520,1389 mode
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2); //5351 mode
#endif
#endif
  
#if defined (IS_WMV_POST_MT5368)
    WriteREG(RW_VLD_DIGMBSA, (width >> 4) - 1);  // 0x98
#else
    WriteREG(RW_VLD_DIGMBSA, 0);
#endif
    
#if defined (IS_WMV_POST_MT5368)
    WriteREG(RW_VLD_PROC, PSUP);         // 0x8C
    WriteREG(RW_VLD_PROC, PDHW | PSUP);  // 0x8C
#else
    WriteREG(RW_VLD_PROC, RTERR + PDHW + PSUP + (prVdec->rSeqHdrR.u4PicWidthDec >> 4));    
#endif

#ifdef AVC_IRQ_DISABLE /* sy's modify */
#if 1 /* using polling method */
	{
		int vp_fin;
		printf("<SY> polling start\n");
		do
		{
			vp_fin = ReadREG(ucEsId, RO_VLD_BLKFIN);
		} while((vp_fin & 0x1) != 1);
		printf("<SY> polling end\n");
	}
#else /* for loop delay */
    for (delay = 0; delay < 1000000; delay++)
        ;
#endif
#else
	/* sy's modify: using interrupt with vp_mode */
	if (x_sema_lock_timeout(hVdecWMVFinSema, 500) != OSR_OK)
	{
#if 0  // debug code
		if ((UINT32)prVdec == (UINT32)&rWMVDecInfo[0])
		{
			printf("#%d decode timeout!!!!!!!!\n", prVdec->rOther.u4CurrPicNo);
			vDumpWmvReg();
			vPrintSavedWmvReg();
		}
#endif
		/* should do error handling here */
		// ...
	}
	//-
#endif  // AVC_IRQ_DISABLE

#if defined(IS_WMV_POST_MT5368)
    WriteREG(MISC_REG_41, ReadREG(ucEsId, MISC_REG_41) | 0x00001000);
#endif

    LOG(3, "VP OK!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}
#endif  // WMV_VP_MODE


void vVPrsIPProc(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (prVdec->rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
    {
        // Check if reset is required
        if (FBM_ChkFrameBufferFlag(prVdec->rOther.u1FbgId, (UINT32)FBM_FLAG_RESET))
        {
            FBM_ResetGroup(prVdec->rOther.u1FbgId);
            FBM_ClrFrameBufferFlag(prVdec->rOther.u1FbgId, (UINT32)FBM_FLAG_RESET);
            FBM_ClrFrameBufferFlag(prVdec->rOther.u1FbgId, (UINT32)FBM_FLAG_NOT_READY);
        }
    }

#ifdef REPEAT_FRAME
    if (prVdec->rOther.u4repeatframe)
    {
        // do nothing
        return;
    }
#endif

    {
        vSetFRefBuf(ucEsId,prVdec->rDram.u4BRefBufIdx);
        vSetBRefBuf(ucEsId,1 - prVdec->rDram.u4FRefBufIdx);
        vSetDecBuf(ucEsId,prVdec->rDram.u4BRefBufIdx);
    }

#ifdef USE_FBM  // megaa 20100113 ToDo: check the matching with #endif
    {
        UINT32 u4Yaddr, u4Caddr;
        UINT32 u4Yaddr2 = 0;
        UINT32 u4Caddr2 = 0;
        UCHAR ucBRefFbId;
        UCHAR ucFRefFbId;

        SetSeqHdr(ucEsId);

        if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (prVdec->rPicLayer.i4CurrentTemporalField == 0))
        {
            UCHAR ucFbId;
            
            FBM_GetRefFrameBuffer(prVdec->rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
            if (FBM_FB_ID_UNKNOWN != ucBRefFbId)
            {
                VDEC_ES_INFO_T* prVdecEsInfo;
                FBM_PIC_HDR_T *prFbmPicHdr = NULL;
                BOOL fgNotDisplay;

                prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);//maiyou

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

                prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rOther.u1FbgId, ucBRefFbId);
                FBM_GetFrameBufferAddr(prVdec->rOther.u1FbgId, ucBRefFbId,&u4Yaddr2, &u4Caddr2);

                if (prFbmPicHdr)
                {
                    if (prVdecEsInfo->u4MMSpeed == (UINT32)STC_SPEED_TYPE_FORWARD_1X)
                    {
                        if (prVdec->rOther.fgFirstDispFrame)
                        {
                            prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                            prVdec->rOther.fgFirstDispFrame = FALSE;
                        }
                        else
                        {
                            if (prVdec->rOther.i4CodecVersion != WMVA)
                            {
                                prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
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
                                     if ((prVdec->rOther.fgBindingByte == TRUE) && 
                                        (prVdec->rOther.u1BindingByte & 0x02))
                                    {
                                        // Only I&P frames, no need to re-calculate PTS.
                                        prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                    }
                                    else
                                    {
                                        UINT32 u4HalfDeltaPTS = prVdec->rOther.u4DeltaPTS>>1;
                                        if ((prFbmPicHdr->u4PTS <= (prVdec->rOther.u4LastPTS+prVdec->rOther.u4DeltaPTS+u4HalfDeltaPTS))
                                            && prFbmPicHdr->u4PTS >= (prVdec->rOther.u4LastPTS + u4HalfDeltaPTS))
                                        {
                                            prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                        }
                                        else
                                        {
                                            prFbmPicHdr->u4PTS = prVdec->rOther.u4LastPTS + prVdec->rOther.u4DeltaPTS;
                                            prVdec->rOther.u4LastPTS += prVdec->rOther.u4DeltaPTS;
                                        }
                                    }
                                }
                                else
                                {
                                    if ((!prVdec->rPesInfo.fgDtsValid) || prVdecEsInfo->eContainerType == SWDMX_FMT_MKV)
                                    {
                                        prFbmPicHdr->u4PTS = prVdec->rOther.u4LastPTS + prVdec->rOther.u4DeltaPTS;
                                        prVdec->rOther.u4LastPTS += prVdec->rOther.u4DeltaPTS;
                                    }
                                    else
                                    {
                                        prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                                    }
                                }
                            }
                        }
                        prFbmPicHdr->i4TemporalRef = prVdec->rPicLayer.i4TemporalRef;
                        LOG(5,"EsId[%d] Vdec WMV I/P/Skip Frame PTS : %8x, temporal ref : %d\n",ucEsId,prFbmPicHdr->u4PTS,prFbmPicHdr->i4TemporalRef);
                        prVdec->rPicLayer.i4TemporalRef++;
                    }
                   else if(prVdecEsInfo->eContainerType == SWDMX_FMT_VC1_ES
                          ||prVdecEsInfo->eContainerType==SWDMX_FMT_MKV)
                   {
                       if ((!prVdec->rPesInfo.fgDtsValid)|| prVdecEsInfo->eContainerType == SWDMX_FMT_VC1_ES)
                       {
                           prFbmPicHdr->u4PTS = prVdec->rOther.u4LastPTS + prVdec->rOther.u4DeltaPTS;
                           prVdec->rOther.u4LastPTS += prVdec->rOther.u4DeltaPTS;
                       }
                       else
                       {
                           prVdec->rOther.u4LastPTS = prFbmPicHdr->u4PTS;
                       }
                    }
//#endif  // megaa 20100113 ToDo: check the matching with #ifdef USE_FBM

                    fgNotDisplay = VDEC_IsNotDisplay(ucEsId, prFbmPicHdr->u4PTS,
                                                     prFbmPicHdr->i4TemporalRef, prFbmPicHdr->u8Offset);
                }
                else
                {
                    fgNotDisplay = VDEC_IsNotDisplay(ucEsId, 0, 0, 0);
                }
                if (fgNotDisplay)
                {
#ifdef WMV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(prVdec->rOther.u1FbgId,ucBRefFbId);
                    LOG(0,"WMV set I/P frame FBM status : %d %d EMPTY\n",ucBRefFbId,u1state);
#endif
                    FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_EMPTY);
                }
                else
                {
#ifdef WMV_PRINT_FBM_STATUS
                    UCHAR u1state;
                    u1state = FBM_GetFrameBufferStatus(prVdec->rOther.u1FbgId,ucBRefFbId);
                    LOG(0,"WMV set I/P frame FBM status : %d %d DISPALY_Q\n",ucBRefFbId,u1state);
#endif
#ifdef CC_REALD_3D_SUPPORT
                    _VDEC_Check2D3DSwitch(ucEsId, ucBRefFbId);
#endif
                    _VDEC_JobBeforePutDispQ(ucEsId, ucBRefFbId);

                    FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId, ucBRefFbId, FBM_FB_STATUS_DISPLAYQ);
                    LOG(5, "\n\n[TRACKING] decoder set dispQ fb id = %d\n\n", ucBRefFbId);
                    prVdecEsInfo->u4DisplayQPicCnt++;

                }
                if (prFbmPicHdr)
                {
                    VDEC_CheckStopStatus(ucEsId, prFbmPicHdr->u4PTS, prFbmPicHdr->i4TemporalRef );
                }
                else
                {
                    VDEC_CheckStopStatus(ucEsId, 0, 0);
                }
            }
            else
            {
                if(prVdec->rPesInfo.fgDtsValid)
                {
                   prVdec->rOther.u4LastPTS = prVdec->rPesInfo.u4PTS;
                }
            }
#ifdef WMV_MINST
            vSaveVDec(ucEsId);
#endif
            _VDEC_UnlockVld(ucEsId,0);//maiyou1
            
            ucFbId = FBM_GetEmptyRefFrameBuffer(prVdec->rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
            LOG(5, "\n\n[TRACKING] decoder got ref fb id = %d\n\n", ucFbId);
            ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);

            LOG(7,"6\n");
            _VDEC_LockVld(ucEsId,0);//maiyou1

#ifdef WMV_MINST
            //_VDEC_WMVSWRst(0,ucEsId);
            //fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.i4SetPos);
            vRestoreVDec(ucEsId, TRUE);
#endif
            FBM_GetFrameBufferAddr(prVdec->rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

            ASSERT(u4Yaddr !=0 || u4Caddr !=0);
            // reset mem here
            SetPicHdr(ucEsId, ucFbId);

            if (prVdec->rDram.u4DecBufIdx == 0) //pic1
            {
                prVdec->rDram.pu1Pic0Y = (UINT8*)u4Yaddr;
                prVdec->rDram.pu1Pic0C = (UINT8*)u4Caddr;
                prVdec->rOther.u1FbId0 = ucFbId;
            }
            else //pic2
            {
                prVdec->rDram.pu1Pic1Y = (UINT8*)u4Yaddr;
                prVdec->rDram.pu1Pic1C = (UINT8*)u4Caddr;
                prVdec->rOther.u1FbId1 = ucFbId;
            }
            if (prVdec->rPicLayer.u1PicType == SKIPFRAME && u4Yaddr2 != 0 && u4Caddr2 != 0)
            {
                UINT32 u4Width,u4Height;
                UINT32 u4Size;
                FBM_GetFrameBufferSize(prVdec->rOther.u1FbgId,&u4Width,&u4Height);
                u4Size = u4Width*u4Height;
#ifdef WMV_VP_MODE  // use VP mode to copy skipframe
                UNUSED(u4Size);
                _WMVDecVP(ucEsId, u4Yaddr, u4Caddr, u4Yaddr2, u4Caddr2, u4Width, u4Height);
#else
#if defined(CC_MT5395) || defined(IS_WMV_POST_MT5368)
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4Yaddr2), u4Size);
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4Caddr2), u4Size>>1);
#else
                HalFlushInvalidateDCache();
#endif
                x_memcpy((VOID*)(VIRTUAL(u4Yaddr)),(VOID*)(VIRTUAL(u4Yaddr2)),u4Size);
                x_memcpy((VOID*)(VIRTUAL(u4Caddr)),(VOID*)(VIRTUAL(u4Caddr2)),u4Size>>1);
#if defined(CC_MT5395)
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4Yaddr), u4Size);  // HalFlushDCacheMultipleLine should be also OK
                HalFlushInvalidateDCacheMultipleLine(VIRTUAL(u4Caddr), u4Size>>1);  // HalFlushDCacheMultipleLine should be also OK
#elif defined(IS_WMV_POST_MT5368)
                HalFlushDCacheMultipleLine(VIRTUAL(u4Yaddr), u4Size);
                HalFlushDCacheMultipleLine(VIRTUAL(u4Caddr), u4Size>>1);
#else
                HalFlushInvalidateDCache();  // HalFlushDCache should be also OK
#endif
#endif  // WMV_VP_MODE
            }
            FBM_SetFrameBufferStatus(prVdec->rOther.u1FbgId,ucFbId,FBM_FB_STATUS_DECODE);
            //spork
            vSetMcBufPtr(ucEsId);
        }
    }
#endif  // megaa 20100113 ToDo: check the matching with #ifdef USE_FBM
}

void vVPrsBProc(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
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
    vSetDecBuf(ucEsId,2);
    //vSaveFrmBufPrm(prVdec->rDram.u4DecBufIdx);

#ifdef USE_FBM
    {
        UINT32 u4Yaddr, u4Caddr;
        SetSeqHdr(ucEsId);
        if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (prVdec->rPicLayer.i4CurrentTemporalField == 0))
        {
            UCHAR ucFbId;
#ifdef WMV_MINST
            vSaveVDec(ucEsId);
#endif
            _VDEC_UnlockVld(ucEsId,0);//maiyou1

            ucFbId = FBM_GetEmptyBFrameBuffer(prVdec->rOther.u1FbgId, VDEC_MM_WAIT_DISP_TIME);
            LOG(5, "\n\n[TRACKING] decoder got B fb id = %d\n\n", ucFbId);
            ASSERT(ucFbId != FBM_FB_ID_UNKNOWN);
            
            LOG(7,"7\n");
            _VDEC_LockVld(ucEsId,0);//maiyou1

#ifdef WMV_MINST
            //_VDEC_WMVSWRst(0,ucEsId);
            //fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.i4SetPos);
            vRestoreVDec(ucEsId, TRUE);
#endif
            FBM_GetFrameBufferAddr(prVdec->rOther.u1FbgId, ucFbId,&u4Yaddr, &u4Caddr);

            ASSERT(u4Yaddr !=0 || u4Caddr !=0);
            // ilii
            // reset mem here
            SetPicHdr(ucEsId,ucFbId);
            //FBM_PIC_HDR_T* prFbmPicHdr;
            //prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rOther.u1FbgId, ucFbId);
            //prFbmPicHdr->fgProgressiveFrm = 1;//(prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE);

            //if((prVdec->rPicLayer.u1PicType == BVOP) || (prVdec->rPicLayer.u1PicType == BIVOP)) //prog_B
            {
                prVdec->rDram.pu1Pic2Y = (UINT8*)u4Yaddr;
                prVdec->rDram.pu1Pic2C = (UINT8*)u4Caddr;
                prVdec->rOther.u1FbId2 = ucFbId;
            }
            //spork
            vSetMcBufPtr(ucEsId);
        }
    }
#endif
}

BOOL fgVParserProcWMV(UCHAR ucEsId)
{
    //UINT32 u4Tmp;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);

#ifdef CC_WMV_EMULATION
    if (prVdec->rPesInfo.fgSkipFrame)  // ??? !!!1123
    {
        return TRUE;
    }
#endif

    // ilii, for more than 1 bitplane, should keep this variable
    //prVdec->rOther.u4BPRawFlag = 0;
    if (prVdec->rOther.i4CodecVersion != WMVA)
    {
        fgVldInitBarrelShifter(ucEsId,(UINT32)prVdec->rDram.pu1VFifo + prVdec->rOther.i4SetPos);
        //x_memcpy(&u4Tmp,(void*)(prVdec->rDram.pu1VFifo + prVdec->rOther.i4SetPos),4);
        LOG(6,"init barrel shifter1, set position : %x\n",prVdec->rOther.i4SetPos);
        //LOG(0,"fetch 32bit data : %x\n",u4Tmp);
        //spork
        //fgSkipFrame = prVdec->rPesInfo.fgSkipFrame;
        
		if(prVdec->rPicLayer.u1PicType==SKIPFRAME&&prVdec->rPesInfo.u2SkipVidCount>0)
        {
           prVdec->rOther.u4Datain=getbits(ucEsId,8);
           LOG(3,"Skip frame:%d\n",prVdec->rOther.u4Datain);
        }
		else if (prVdec->rOther.i4CodecVersion == WMV3) //VC-1 Simple & Main
        {
            prVdec->rOther.i4VprErr = WMVideoDecDecodeFrameHead(ucEsId);
        }
        else if ((prVdec->rOther.i4CodecVersion == WMV1) || (prVdec->rOther.i4CodecVersion == WMV2)) //WMV7, 8
        {
            prVdec->rOther.i4VprErr = WMV78DecodePicture(ucEsId);
            if (prVdec->rSMProfile.fgXintra8)
            {
                return TRUE;
            }
        }

        if (prVdec->rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            return FALSE;
        }
        
        switch (prVdec->rPicLayer.u1PicType)
        {
        case IVOP:
        case PVOP:
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
            if(ucEsId<MAX_ES_NUM)
            {
                vVPrsIPProc(ucEsId);
            }
            break;
        case BIVOP:
        case BVOP:
            {
               UINT8 ucFRefFbId=0,ucBRefFbId=0;
               FBM_PIC_HDR_T *prFbmPicHdr = NULL;
               UINT32 u4TmpPts=0;
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
               
               FBM_GetRefFrameBuffer(prVdec->rOther.u1FbgId, &ucFRefFbId, &ucBRefFbId);
               if(FBM_FB_ID_UNKNOWN != ucBRefFbId && prVdec->rPesInfo.fgDtsValid)
               {
                   prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdec->rOther.u1FbgId, ucBRefFbId);
                   if(prFbmPicHdr)
                   {
                       if(prFbmPicHdr->u4PTS<prVdec->rPesInfo.u4PTS)
                       {
                         u4TmpPts=prFbmPicHdr->u4PTS;
                         prFbmPicHdr->u4PTS=prVdec->rPesInfo.u4PTS;
                         prVdec->rPesInfo.u4PTS=u4TmpPts;
                       }
                   }
               }
               
               if(ucEsId<MAX_ES_NUM)
               {
                  vVPrsBProc(ucEsId);
               }
            }
            break;
        case SKIPFRAME:
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
            if(ucEsId<MAX_ES_NUM)
            {
                vVPrsIPProc(ucEsId);
            }
            break;
        default:
            break;
        } //switch(prVdec->rPicLayer.u1PicType)

        if (prVdec->rPicLayer.u1PicType == SKIPFRAME)
        {
            prVdec->rOther.u4DecPicNo++;
        }
        if (cbWMVHeaderOnly)
        {
            print_vop_header_parameter(ucEsId,prVdec->rOther.u4DecPicNo, prVdec->rDram.u4FRefBufIdx, prVdec->rDram.u4DecBufIdx); //umv_from_mb = 1 for !WMVA
            prVdec->rOther.u4DecPicNo++;
        }
    }
    else //Advanced Profile
    {
        if ((prVdec->rOther.i4VprErr = dwVParserWMVA(ucEsId)) != 0) //paring advanced profile, including sequence header
        {
            if (prVdec->rOther.i4VprErr == END_OF_FILE)
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
        if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (prVdec->rPicLayer.i4CurrentTemporalField == 0))
        {
            switch (prVdec->rPicLayer.u1PicType)
            {
            case IVOP:
            case PVOP:
                if(ucEsId<MAX_ES_NUM)
                {
                    vVPrsIPProc(ucEsId);
                }
                ///WriteREG(RW_MC_SAME_REF_PIC, 0);
                break;
            case BVOP:
            case BIVOP:
                if(ucEsId<MAX_ES_NUM)
                {
                    vVPrsBProc(ucEsId);
                }
                break;
            case SKIPFRAME:
                if(ucEsId<MAX_ES_NUM)
                {
                    vVPrsIPProc(ucEsId);
                }
                //WriteREG(RW_MC_FWDP, prVdec->rDram.u4FRefBufIdx);
                //vSetFRefBuf(ucEsId,prVdec->rDram.u4DecBufIdx); // F_Ref = Dec. TEST for Progressive and Interlaced Frame.
                //WriteREG(RW_MC_SAME_REF_PIC, 1);
                break;
            default:
                break;
            } //switch(prVdec->rPicLayer.u1PicType)
        }

        if (cbWMVHeaderOnly)
        {
            print_vop_header_parameter(ucEsId,/*0,*/ prVdec->rOther.u4DecPicNo, prVdec->rDram.u4FRefBufIdx, prVdec->rDram.u4DecBufIdx); //umv_from_mb = 0 for WMVA
            if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (prVdec->rPicLayer.i4CurrentTemporalField == 1))
            {
                prVdec->rOther.u4DecPicNo++;
            }
            if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
            {
                prVdec->rPicLayer.i4CurrentTemporalField ^= 1; //toggle field
                prVdec->rPicLayer.i4CurrentField ^= 1;
            }
        }
        //End of Switch Frame Buffer
    }

    return(TRUE);
}


BOOL fgVDecProcWMV(UCHAR ucEsId)
{
    UINT32 u4RegVal;
    UINT32 u4MC_a377;
    VDEC_WMV_INFO_T* prVdec;

    prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);

#ifdef WMV_MINST
    vCheckBP(ucEsId);
#endif
    
    if (prVdec->rOther.i4CodecVersion == WMVA)
    {
        print_vop_header_parameter(ucEsId,prVdec->rOther.u4DecPicNo, prVdec->rDram.u4FRefBufIdx, prVdec->rDram.u4DecBufIdx); //umv_from_mb = 0 for WMVA
        if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD) || (prVdec->rPicLayer.i4CurrentTemporalField == 1))
        {
            prVdec->rOther.u4DecPicNo++;
        }
    }
    else
    {
        print_vop_header_parameter(ucEsId,prVdec->rOther.u4DecPicNo, prVdec->rDram.u4FRefBufIdx, prVdec->rDram.u4DecBufIdx); //umv_from_mb = 1 for !WMVA
        prVdec->rOther.u4DecPicNo++;
    }
    //for vld slice header parsing
    u4RegVal = 0x00080000;
#ifdef USING_WMV_FIND_HDR_FIX
    u4RegVal |= (ReadREG(ucEsId, RW_VLD_BP_MODE_FIRE) & BP_MODE_WMV_FIND_HDR);
#endif
    WriteREG(RW_VLD_BP_MODE_FIRE, u4RegVal);
    //WriteREG(MC_REG_OFST+0x660,0x03120456);//change local arbiter priority
    
#ifndef CC_WMV_EMULATION  // 64 align
    u4RegVal = (prVdec->rSeqHdrR.u4PicWidthSrc + 63) / 64 * 64 / 16;
    //u4RegVal = prVdec->rOther.u4LineSize >> 4;
#else
    u4RegVal = (prVdec->rSeqHdrR.u4PicWidthSrc + 15) >> 4;
#endif

#if defined(IS_WMV_POST_MT5368)
    WriteREG(RW_MC_PITCH, u4RegVal);  // !!!1006 take care of 64x32 mode as change note described
#else
    WriteREG(RW_VLD_MCPIC_BLKW, u4RegVal);
#endif

    //for new MC. merge motion vector
    u4MC_a377 = ReadREG(ucEsId, MC_REG_377);
    u4MC_a377 |=  (1<<12);
    WriteREG(MC_REG_377, u4MC_a377);
    //for new MC. merge motion vector

#ifdef CC_WMV_EMULATION
#if defined(CC_MT5395) || defined(IS_WMV_POST_MT5368)
    HalFlushInvalidateDCacheMultipleLine(prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoStart);
#else
    HalFlushInvalidateDCache();
#endif
#endif
    //u4Tmp = ((prVdec->rSeqHdrR.u4PicWidthSrc + 15) >> 4);
    //if(u4Tmp&0x1 ==1)
    //  u4Tmp +=1;


    //WriteREG((VLD_REG_OFST + (0x1FC)), 0x1000);

    //import for interrupt , spork
    prVdec->rOther.fgRet = FALSE;

    //LOG(5,"Rptr 0x%x, input win 0x%x\n", ReadREG(ucEsId,RO_VLD_VRPTR), ReadREG(ucEsId,RO_VLD_BARL));
    //LOG(6,"PP Y 0x%x, PP C 0x%x\n", ReadREG(ucEsId,RW_MC_PP_Y_ADDR), ReadREG(ucEsId,RW_MC_PP_C_ADDR));
    //update write pointer

    //_VDEC_UpdateVldWMVWPtr(ucEsId,prVdec->rOther.u4WPtr);

    //ilii, in this case, f/w decode bp, tell h/w that we have decoded it.
    if (cbWMVFWBP) // trigger bp if  f/w decodes it
    {
        u4RegVal = ReadREG(ucEsId, RW_VLD_BP_MODE_FIRE);
        
        WriteREG(RW_VLD_BP_MODE_FIRE, 1<<26);
        WriteREG(RW_VLD_BP_MODE_FIRE, 0);
        
#ifdef USING_WMV_FIND_HDR_FIX
        WriteREG(RW_VLD_BP_MODE_FIRE, (u4RegVal & BP_MODE_WMV_FIND_HDR));
#endif
    }

#ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&_rTimeRstS);
#endif

    if (prVdec->rOther.fgNewMode)
    {
        INT32 i4Predictor = 0;

        if (prVdec->rOther.i4CodecVersion == WMVA || prVdec->rOther.i4CodecVersion == WMV3)  // VC-1 or WMV9
        {
            UINT32 a131, a134, a135, a137, a138, a139;
            int TRB, TRD, FWD, BWD;
            UINT32 amv_max_v, amv_max_h;

            // MV part
            // 131
            if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
            {
                if (prVdec->rPicLayer.i4CurrentField == 0) // top
                    a131 = PHYSICAL((UINT32)prVdec->rDram.pu1Mv_1) >> 4;
                else
                    a131 = PHYSICAL((UINT32)prVdec->rDram.pu1Mv_1 + (1920/16)*(1088/16)*16/2) >> 4;
            }
            else
            {
                a131 = PHYSICAL((UINT32)prVdec->rDram.pu1Mv_1) >> 4;  // ???
            }
            WriteREG(RW_WMV_MV_NEW_ADDR, a131);

            // 134
            if (prVdec->rPicLayer.u1PicType == BVOP)
            {
                if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    if (prVdec->rPicLayer.i4CurrentField == 0) // top
                    {
                        a134 = ((prVdec->rOther.u1FrameTypeLastTop != PVOP) ? 1: 0);
                    }
                    else
                    {
                        a134 = ((prVdec->rOther.u1FrameTypeLastBot != PVOP) ? 1: 0);
                    }
                }
                else
                {
                    a134 = ((prVdec->rOther.u1FrameTypeLast != PVOP) ? 1: 0);
                }

                WriteREG(RW_WMV_BWDREF_PIC_TYPE, a134);
            }

            // 135
            if (prVdec->rPicLayer.u1PicType == BVOP)
            {
                TRB = prVdec->rPicLayer.i4BNumerator * prVdec->rPicLayer.i4BFrameReciprocal;
                TRD = TRB - 256;
                a135 = (TRB << 0) + (TRD << 16);

                WriteREG(RW_WMV_MV_DIRECT_SCL_FAC, a135);
            }

            // 137
            if (prVdec->rPicLayer.u1PicType == PVOP)
            {
                FWD = prVdec->rEntryPoint.i4RefFrameDistance;
                BWD = 0;
            }
            else if (prVdec->rPicLayer.u1PicType == BVOP)
            {
                FWD = prVdec->rOther.i4FwdRefDist;
                BWD = prVdec->rOther.i4BwdRefDist;
            }
            else
            {
                FWD = 0;
                BWD = 0;
            }
            a137 =
                (((prVdec->rPicLayer.i4CurrentTemporalField == 0) ? 1: 0) << WMV_FIRST_FIELD)
                + (BWD                                                       << WMV_BWDREF_DIST)
                + (FWD                                                       << WMV_FWDREF_DIST)
                + (prVdec->rPicLayer.fgUseOppFieldForRef                 << WMV_USE_OPP_REF_FIELD)
                + (prVdec->rPicLayer.fgTwoRefPictures                    << WMV_TWO_REF_PIC)
                + ((prVdec->rOther.i4CodecVersion == WMVA ? 1 : 0)       << WMV_POST_RESCENT)
                + (prVdec->rEntryPoint.fgUVHpelBilinear                  << WMV_FAST_UV)
                + (prVdec->rEntryPoint.i4MVRangeIndex                    << WMV_MV_RNG_IDX)
                + (prVdec->rOther.use_quarter_pel_mv                     << WMV_QPIXEL);
            //+ (prWmvDecPrm->prPPS->fgHalfStep                          << WMV_QPIXEL);

            WriteREG(RW_WMV_MV_CFG, a137);

            // 138
            amv_max_v = ((prVdec->rSeqHdrR.u4PicWidthDec + 15)>> 4) * 2 * 8 * 4 - 4;
            amv_max_h = (((prVdec->rSeqHdrR.u4PicHeightDec << (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD ? 1 : 0)) + 15) >> 4) * 2 * 8 * 4 - 4;
            a138 = ((amv_max_h << 16) | amv_max_v);
            WriteREG(RW_WMV_MV_MAX_1, a138);

            // 139
            amv_max_v = ((prVdec->rSeqHdrR.u4PicWidthDec + 15)>> 4) * 8 * 4 - 4;
            amv_max_h = (((prVdec->rSeqHdrR.u4PicHeightDec << (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD ? 1 : 0)) + 15) >> 4) * 8 * 4 - 4;
            a139 = ((amv_max_h << 16) | amv_max_v);
            WriteREG(RW_WMV_MV_MAX_2, a139);
        }
        else if (prVdec->rOther.i4CodecVersion == WMV1 || prVdec->rOther.i4CodecVersion == WMV2)  // WMV 7/8
        {
            UINT32 u4Reg;
            int i, wmv78_slice_num;

            vSetNewMode(ucEsId);  // tmp solution

            // MV part
            // 131
            if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
            {
                if (prVdec->rPicLayer.i4CurrentField == 0) // top
                    u4Reg = PHYSICAL((UINT32)prVdec->rDram.pu1Mv_1) >> 4;
                else
                    u4Reg = PHYSICAL((UINT32)prVdec->rDram.pu1Mv_1 + (1920/16)*(1088/16)*16/2) >> 4;
            }
            else
            {
                u4Reg = PHYSICAL((UINT32)prVdec->rDram.pu1Mv_1) >> 4;  // ???
            }
            WriteREG(RW_WMV_MV_NEW_ADDR, u4Reg);

            // 180
            WriteREG(RW_WMV_MV_180, (prVdec->rSMProfile.fgFrmHybridMVOn && !prVdec->rPicLayer.fgMvResolution) ? 1 : 0);

            // VLD part
            // 134
            u4Reg = ReadREG(ucEsId,RW_VLD_WMV_OVC);
            u4Reg |= (1 << 15);
            WriteREG(RW_VLD_WMV_OVC, u4Reg);

            // 157, 158, 159
            if (prVdec->rOther.i4CodecVersion == WMV2)
                wmv78_slice_num = prVdec->rSeqHdrR.u4NumMBY / prVdec->rSMProfile.i4SliceCode;
            else
                wmv78_slice_num = prVdec->rSeqHdrR.u4NumMBY / (prVdec->rSMProfile.i4SliceCode - 22);
            for (i = 1; i < prVdec->rSeqHdrR.u4NumMBY; i++)
            {
                if (i < 31)
                {
                    u4Reg = ReadREG(ucEsId,RW_VLD_SLICE_MARK_REG0);
                    u4Reg += ((i % wmv78_slice_num) == 0) << (i + 1);
                    WriteREG(RW_VLD_SLICE_MARK_REG0, u4Reg);
                }
                else if (i < 63)
                {
                    u4Reg = ReadREG(ucEsId,RW_VLD_SLICE_MARK_REG1);
                    u4Reg += ((i% wmv78_slice_num) == 0) << (i - 31);
                    WriteREG(RW_VLD_SLICE_MARK_REG1, u4Reg);
                }
                else
                {
                    u4Reg = ReadREG(ucEsId,RW_VLD_SLICE_MARK_REG2);
                    u4Reg += ((i% wmv78_slice_num) == 0) << (i - 63);
                    WriteREG(RW_VLD_SLICE_MARK_REG2, u4Reg);
                }
            }
        }

        // DCAC part
        // 0
        if (prVdec->rOther.i4CodecVersion == WMVA || prVdec->rOther.i4CodecVersion == WMV3)  // VC-1 or WMV9
        {
            WriteREG(RW_WMV_DCAC_RULEB, prVdec->rOther.use_rule_b);
        }
        else if (prVdec->rOther.i4CodecVersion == WMV1 || prVdec->rOther.i4CodecVersion == WMV2)  // WMV 7/8
        {
            UINT32 u4Reg;

#if 1
            u4Reg = 1 |
                    (prVdec->rPicLayer.u1PicType == PVOP ? ((prVdec->rOther.fgDCPred_IMBInPFrame ? 1 : 0) << 2) : 0) |
                    (prVdec->rPicLayer.i4StepSize << 8);
#else
            u4Reg = prVdec->rOther.use_rule_b;
            u4Reg |= ((prVdec->rOther.fgDCPred_IMBInPFrame ? 1 : 0) << 2);
            u4Reg |= (prVdec->rPicLayer.i4DCStepSize << 8);
#endif

            WriteREG(RW_WMV_DCAC_RULEB, u4Reg);
        }

        // 1
        if (prVdec->rPicLayer.i4DCStepSize > 0)
        {
            i4Predictor = (1024 + (prVdec->rPicLayer.i4DCStepSize >> 1)) / (prVdec->rPicLayer.i4DCStepSize);
        }
        WriteREG(RW_WMV_DCAC_PRED, i4Predictor);  // ToDo: check the case that i4DCStepSize == 0
    }

    prVdec->rOther.u4CurDecMs = 0;
#ifdef VDEC_TIME_PROFILE
    prVdec->rOther.u4CurDramCycle = 0;
    u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ; //dram cycle /sec
    #if defined(CC_MT5395) || defined(IS_WMV_POST_MT5368)
    u4DramCycle = SRM_DramMonitor(0, SRM_DRV_VDEC_MC, u4DelayClock, 0);
    #else
    u4DramCycle = SRM_DramMonitor(SRM_DRV_VDEC_MC, u4DelayClock, 0);
    #endif
#endif

#if defined(CC_WMV_EMULATION) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT) && !defined(CC_WMV_EMULATION)
    if(VDEC_IsTestCRC(ucEsId))
#endif
    {
       UINT32 u4CrcSource,u4CrcCtrl;
       u4CrcCtrl=ReadREG(ucEsId,RW_CRC_CTRL);
       ASSERT(u4CrcCtrl&0x1);
       u4CrcSource=ReadREG(ucEsId,RW_MC_PP_ENABLE);
       u4CrcCtrl=1;
       u4CrcCtrl|=((u4CrcSource&0x1)<<1);
       WriteREG(RW_CRC_CTRL, u4CrcCtrl);
    }
#endif

#ifdef MC_LOG_DUMP
    //WriteREG(RW_MC_PARA_BEHAVIOR, PARA4_BEHAVIOR);
    WriteREG(RW_MC_PARA_BEHAVIOR, 0);
#endif

    prVdec->rOther.fgEarlyDisp = FALSE;
#ifdef WMV_TRACKING_MODE
    {
        extern BOOL _fgEnableTracking;  // tmp solution
        VDEC_ES_INFO_T* prVdecEsInfoX;
        FBM_PIC_HDR_T* prFbmPicHdrX = NULL;

        prVdecEsInfoX = _VDEC_GetEsInfo(ucEsId);
        prFbmPicHdrX = FBM_GetFrameBufferPicHdr(prVdecEsInfoX->ucFbgId, prVdecEsInfoX->ucFbId);
		
        FBM_SetFrameBufferFlag(prVdecEsInfoX->ucFbgId, FBM_FLAG_TRACKING_MODE_ENABLE);
		
        if (prVdec->rPicLayer.u1PicType == BVOP)
        {
            if (_fgEnableTracking)
            {
                if (prVdecEsInfoX->ucFbId == prVdec->rOther.ucLastBFbId)
                {
#if 1
                    UINT32 value;
                    LOG(1, "<SY>YES-RACE[%d:%d]\n", prVdecEsInfoX->ucFbgId, prVdecEsInfoX->ucFbId);
                    value = ReadREG(ucEsId, RW_VLD_TOP_TRACKING_MODE);
                    LOG(1, "<SY> Orig: %08x\n", value);
#endif
                    if (prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD)
                    {
                        WriteREG(RW_VLD_TOP_TRACKING_MODE, ReadREG(ucEsId, RW_VLD_TOP_TRACKING_MODE) | ENABLE_TRACKING | BUF_MBL_OFFSET);
                    }
                    else
                    {
                        WriteREG(RW_VLD_TOP_TRACKING_MODE, ReadREG(ucEsId, RW_VLD_TOP_TRACKING_MODE) | ENABLE_TRACKING | FIELD_PICTURE | BUF_MBL_OFFSET);
                    }
                    prVdec->rOther.fgEarlyDisp = TRUE;  // set it for early display flow
#if 1
                    value = ReadREG(ucEsId, RW_VLD_TOP_TRACKING_MODE);
                    LOG(1, "<SY> Aftr: %08x\n", value);
#endif
                }

                LOG(1, "Enable tracking for FBM!!!\n");
                prFbmPicHdrX->fgTrackingMode = 1;  // enable it for all B
            }
		    
            prVdec->rOther.ucLastBFbId = prVdecEsInfoX->ucFbId;  // record the FB ID for this B
        }
#if 1
        LOG(1, "<MEGAA> Tracking = %08X\n", ReadREG(ucEsId, RW_VLD_TOP_TRACKING_MODE));
        LOG(1, "<MEGAA> FB ID = %d, Type = %d\n", prVdecEsInfoX->ucFbId, prVdec->rPicLayer.u1PicType);
#endif
    }
#endif  // WMV_TRACKING_MODE

#if defined(CC_MT5389)

    if(prVdec->rSeqHdrR.u4PicWidthDec > 1920)
    {
        UINT32 u4TmpVal = 0;
		Printf("*****prVdec->rSeqHdrR.u4PicWidthDec %x",prVdec->rSeqHdrR.u4PicWidthDec);
		WriteREG(MC_REG_OFST+0x57c, 0x00000001); 
		
		u4TmpVal=ReadREG(ucEsId,(MC_REG_OFST+0x80C));
		u4TmpVal = u4TmpVal&0xFFFFFFEF;
		WriteREG((MC_REG_OFST+0x80C),u4TmpVal); 

		u4TmpVal=ReadREG(ucEsId,(MC_REG_OFST+0x834));
		u4TmpVal = u4TmpVal|0x00000001;
		WriteREG((MC_REG_OFST+0x834),u4TmpVal);
    }
#endif    

    WriteREG(RW_VLD_WMVDEC, 0x00000001);
    WriteREG(RW_VLD_WMVDEC, 0x00000000);

#ifdef WMV_TRACKING_MODE
    if (prVdec->rOther.fgEarlyDisp)
    {
        vPutDispQForB(ucEsId, FBM_FB_ID_UNKNOWN);
    }
#endif
    
    prVdec->rOther.fgBPDirty = FALSE;

    return (TRUE);
}

//extern UINT32 prVdec->rOther.u4BitCount;
BOOL fgVDecodeEndWMV(UCHAR ucEsId)
{
    BOOL fgDecEnd = FALSE;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    //UINT32 dwMp2Mode;
    //UINT32 dwInputWnd1, dwInputWnd2;
    //UINT32 dMbX, dMbY;
    //UINT32 dwVldByte, dwVldBit;

#ifndef AVC_IRQ_DISABLE
    if (!prVdec->rOther.fgRet)
    {
        if (x_sema_lock_timeout(hVdecWMVFinSema, 500) != OSR_OK)
        {
            LOG(1,"VC-1 decoder timeout\n");
#if 0  // debug code
            if ((UINT32)prVdec == (UINT32)&rWMVDecInfo[0])
            {
                printf("#%d decode timeout!!!!!!!!\n", prVdec->rOther.u4CurrPicNo);
                vDumpWmvReg();
                vPrintSavedWmvReg();
            }
#endif
            return FALSE;
        }
    }
    if (prVdec->rOther.fgRet == TRUE)
    {
        return FALSE;
    }
    fgDecEnd = TRUE;
#else
#if 1
    fgDecEnd = ReadREG(ucEsId,RO_VLD_PIC_DEC_END) & 0x1;
    LOG(3,"fgDecEnd : %d\n",fgDecEnd);
#else  // 5365/5395 emulation test code, for reference only
    UINT32 u4Reg;
    UINT32 u4Cnt = 0;
    UINT32 u4Delay;

    do
    {
        u4Reg = ReadREG(ucEsId,RO_VLD_PIC_DEC_END);
        fgDecEnd = u4Reg/*ReadREG(ucEsId,RO_VLD_PIC_DEC_END)*/ & 0x1;
        //LOG(3,"fgDecEnd : %d\n",fgDecEnd);
        //printf("vdec_wmvdec 1239 RO_VLD_PIC_DEC_END = %d\n", u4Reg);
        if (++u4Cnt >= 10)
        {
            printf("vdec_wmvdec 1410 timeout!!!!!!!!!!!!\n");
            u4DumpWmvReg(NULL);
            ASSERT(0);
            fgDecEnd = TRUE;
        }
        for (u4Delay = 0; u4Delay < 100000; u4Delay++);
    }
    while (!fgDecEnd);
#endif
#endif


#ifdef __MODEL_slt__
    //printf("#%02d: %08X %08X %08X %08X\n", prVdec->rOther.u4CurrPicNo, ReadREG(ucEsId,RW_CRC_Y_0), ReadREG(ucEsId,RW_CRC_Y_1), ReadREG(ucEsId,RW_CRC_Y_2), ReadREG(ucEsId,RW_CRC_Y_3));
    //printf("#%02d: %08X %08X %08X %08X\n", prVdec->rOther.u4CurrPicNo, ReadREG(ucEsId,RW_CRC_C_0), ReadREG(ucEsId,RW_CRC_C_1), ReadREG(ucEsId,RW_CRC_C_2), ReadREG(ucEsId,RW_CRC_C_3));
    if (prVdec->rPesInfo.u4PTS == 33030)
    {
        if (fgDecEnd)
        {
            if (ReadREG(ucEsId,RW_CRC_Y_0) == 0x54A134C6 &&
                    ReadREG(ucEsId,RW_CRC_Y_1) == 0x6EBF46DC &&
                    ReadREG(ucEsId,RW_CRC_Y_2) == 0x796DE869 &&
                    ReadREG(ucEsId,RW_CRC_Y_3) == 0x1749F9EE &&
                    ReadREG(ucEsId,RW_CRC_C_0) == 0x1F08BB5F &&
                    ReadREG(ucEsId,RW_CRC_C_1) == 0x48C411AF &&
                    ReadREG(ucEsId,RW_CRC_C_2) == 0xEF862173 &&
                    ReadREG(ucEsId,RW_CRC_C_3) == 0x79D1637C)
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
    //wmv crc

#if 0
        if (!fgWaitVldFetchOk(ucEsId))
        {
            LOG(3,"fetch not ok\n");
            return(FALSE);
        }
        // load wmv-sum to mpeg2-sum
        //WriteREG(RW_VLD_116, 0x1);
        //WriteREG(RW_VLD_116, 0x0);
        // switch to mpeg2 mode
        //WriteREG(RW_VLD_WMV_MODE, 0x0);
        //fgDecEnd = ReadREG(ucEsId,RO_VLD_PIC_DEC_END);
#endif

#ifdef VDEC_TIME_PROFILE
        prVdec->rOther.u4CurDecMs += (_rTimeRstDt.u4Micros / 1000);
        u4DramCycle = IO_REG32(DRAM_BASE, 0x190);
        //LOG(2, "dram cycle=%d\n", u4DramCycle);
        prVdec->rOther.u4CurDramCycle += u4DramCycle;
#endif

        vVldByteAlign(ucEsId);
        prVdec->rOther.u4BPRawFlag = 0;
        return(TRUE);
    }

    return(FALSE);
}


/** Initialize module for wmv
 */
void _WMVInit(UCHAR ucEsId)
{
    UINT32 u4Temp;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    
    vInitVDec(ucEsId);
    vInitVld(ucEsId);
    //vInitPred(ucEsId);
#ifdef WMV_TRACKING_MODE
    //WriteREG(MC_REG_525, 1);  // take care of FPGA-only setting!!!
#endif
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x3);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x3);
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x2);
#endif
    u4Temp = ReadREG(ucEsId,MC_REG_325);
    u4Temp |= (0x1000);
    WriteREG(MC_REG_325, u4Temp);
    
    u4Temp = ReadREG(ucEsId, MC_REG_515);
    u4Temp &= 0xFFEFFFFF;  // clear bit 20 to enhance DRAM performance
#if defined(IS_WMV_POST_MT5368)
    u4Temp &= 0xFBFFFFFF;  // bit 26: PRED to DRAM
    if (cbWMVFWBP)
    {
        u4Temp |= 0x08000000;  // bit 27: BP to DRAM
    }
#else
    if (cbWMVFWBP)
    {
        u4Temp |= 0x01000000;  // bit 24: BP to DRAM
    }
#endif
    WriteREG(MC_REG_515, u4Temp);
//wmv crc
#if defined(__MODEL_slt__) || defined(CC_WMV_EMULATION) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT) && !defined(CC_WMV_EMULATION) && !defined(__MODEL_slt__)
    if(VDEC_IsTestCRC(ucEsId))
#endif
    {
        WriteREG(RW_CRC_CTRL, 0x1);
    }
#endif

    prVdec->rOther.ucLastBFbId = 0xFF;  // needed for tracking mode
}

/*
** wmv header parsing function
 */
void _getESInfo(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    //spork
    //prVdec->rSeqHdrR.u4PicWidthSrc = prVdec->rPesInfo.u4Width;
    //prVdec->rSeqHdrR.u4PicHeightSrc = prVdec->rPesInfo.u4Height;
    LOG(6,"my resolution = %d x %d\n",prVdec->rSeqHdrR.u4PicWidthSrc,prVdec->rSeqHdrR.u4PicHeightSrc);
    prVdec->rOther.rPicInfo.dwW = prVdec->rSeqHdrR.u4PicWidthSrc;
    prVdec->rOther.rPicInfo.dwH = prVdec->rSeqHdrR.u4PicHeightSrc;
    prVdec->rSeqHdrR.u4PicWidthDec = (prVdec->rSeqHdrR.u4PicWidthSrc + 15) & ~15;
    prVdec->rSeqHdrR.u4PicHeightDec = (prVdec->rSeqHdrR.u4PicHeightSrc + 15) & ~15;
    prVdec->rSeqHdrR.u4NumMBX = prVdec->rSeqHdrR.u4PicWidthDec >> 4;
    prVdec->rSeqHdrR.u4NumMBY = prVdec->rSeqHdrR.u4PicHeightDec >> 4;
    prVdec->rSeqHdrR.u4PicWidthCmp = prVdec->rSeqHdrR.u4PicWidthSrc;
    prVdec->rSeqHdrR.u4PicHeightCmp = prVdec->rSeqHdrR.u4PicHeightSrc;
#if 0  // megaa 20100321
    prVdec->rOther.u4BitCount = prVdec->rPesInfo.u4VldReadPtr<<3; // # of parsing bits
#endif
    //prVdec->rOther.i4SeqHdrDataLen = prVdec->rPesInfo.u4SeqHdrDataLen;
    //prVdec->rOther.i4SeqHdrData1 = prVdec->rPesInfo.u4SeqHdrData1;
}
void _WMVFreeMiscMem(UCHAR ucEsId)
{
#ifndef USE_FBM
#ifndef CC_WMV_EMULATION
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Dcac);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Mv_1);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Mv_2);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Bp_1);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Bp_2);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Bp_3);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Bp_4);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Mv_3);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Mv_1_2);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Dcac_2);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Pp_1);
    BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Pp_2);
#endif
#endif

    //BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Pp_1);
    //BSP_FreeAlignedDmaMemory(prVdec->rDram.pu1Pp_2);
}

void _WMVHdrPrs(UCHAR ucEsId,INT32* pi4RetValue)
{
#ifdef CC_WMV_EMULATION
    extern BOOL _fgAbtCurItem;
#else
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
#endif
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);

#ifdef CC_WMV_EMULATION
    if (prVdec->rPesInfo.ucPicType == 0xff)
    {
        _WMVFreeMiscMem(ucEsId);
#ifdef USE_FBM
        FBM_ReleaseGroup(prVdec->rOther.u1FbgId);
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

    prVdec->rOther.u4DecBPCnt = 0;
    
#if 1
    //init dmx pes information and MM_set_parameter information
    prVdec->rOther.i4SetPos = (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart);
    prVdec->rDram.pu1VFifo = (void*)prVdec->rPesInfo.u4FifoStart;
    prVdec->rDram.pu1FifoEnd = (void*)prVdec->rPesInfo.u4FifoEnd;

#ifndef CC_WMV_EMULATION
    if (prVdecEsInfo != NULL && prVdec->rPesInfo.u4VideoType != DMX_VIDEO_VC1)
        //if(prVdecEsInfo != NULL)
    {
        prVdec->rSeqHdrR.u4PicWidthSrc =  (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
        prVdec->rSeqHdrR.u4PicHeightSrc = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
        prVdec->rOther.i4SeqHdrDataLen = (UINT32)prVdecEsInfo->rMMSeqInfo.u2SequenceHeaderSize;
        prVdec->rOther.i4SeqHdrData1 = (UINT32)prVdecEsInfo->rMMSeqInfo.u4SequenceHeader;
		if(prVdec->rSeqHdrR.u4PicWidthSrc>VDEC_WMV_MAX_WIDTH 
			|| prVdec->rSeqHdrR.u4PicHeightSrc>VDEC_WMV_MAX_HEIGHT)
		{
            *pi4RetValue = E_VDEC_NOT_SUPPORT;
			return;
		}
    }
#else
    // vparser use _VdecVParserWMV to set decoder parameters
    if (prVdec->rPesInfo.ucPicType == 0xff &&prVdec->rPesInfo.fgEos)
    {
        return;
    }
#endif

    if (prVdec->rPesInfo.u4VideoType == DMX_VIDEO_VC1)
    {
        prVdec->rSeqLayer.fgVC1 = TRUE;
        prVdec->rOther.i4CodecVersion = WMVA;
    }
    else if (prVdec->rPesInfo.u4VideoType == DMX_VIDEO_WMV7)
    {
        prVdec->rSeqLayer.fgVC1 = FALSE;
        prVdec->rOther.i4CodecVersion = WMV1;
    }
    else if (prVdec->rPesInfo.u4VideoType == DMX_VIDEO_WMV8)
    {
        prVdec->rSeqLayer.fgVC1 = FALSE;
        prVdec->rOther.i4CodecVersion = WMV2;
    }
    else if (prVdec->rPesInfo.u4VideoType == DMX_VIDEO_WMV9)
    {
        prVdec->rSeqLayer.fgVC1 = FALSE;
        prVdec->rOther.i4CodecVersion = WMV3;
    }
    else
    {
        prVdec->rSeqLayer.fgVC1 = FALSE;
        prVdec->rOther.i4CodecVersion = UNKNOW_TYPE;
    }

    //LOG(0,"video type : %d\n",prVdec->rOther.i4CodecVersion);
#endif

#if 0
    if (prVdec->rOther.fgFirstFrame) //init state
    {
        LOG(6,"hello, it's first frame\n");
        _WMVInit(ucEsId);
        vInitVParser(ucEsId);
        vInitVParserWMV(ucEsId);
        vPowerOnVDec();

        prVdec->rOther.fgDumpSimPat = FALSE;
        prVdec->rOther.u4BPRawFlag = 0;
        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
        if (prVdec->rOther.i4CodecVersion != WMVA)
        {
            _getESInfo(ucEsId);
            if (prVdec->rOther.i4CodecVersion == WMV3)
            {
                SetupMultiResParams(ucEsId);
                prVdec->rOther.i4VprErr = decodeVOLHead_WMV3(ucEsId);
            }
            else if ((prVdec->rOther.i4CodecVersion == WMV1) || (prVdec->rOther.i4CodecVersion == WMV2))
            {
                prVdec->rOther.i4VprErr = decodeVOLHead_WMV12(ucEsId);
            }
        }
        prVdec->rOther.fgFirstFrame = FALSE;
    }
#endif
    if (fgVParserProcWMV(ucEsId))
    {
        if (prVdec->rSMProfile.fgXintra8)
        {
            prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
            //*pi4RetValue = VDEC_PLAY_HEADER_PARSE;
            *pi4RetValue = E_VDEC_NOT_SUPPORT;
            prVdec->rSMProfile.fgXintra8 = FALSE;
            //_WMVSWRst(NULL);
            return;
        }
#ifdef CC_WMV_EMULATION
        else if (prVdec->rPesInfo.fgSkipFrame)  // ??? !!!1123
        {
            prVdec->rOther.u4DecPicNo++; //Robert 060608
            print_vop_header_parameter(ucEsId,/*0,*/ prVdec->rOther.u4DecPicNo, prVdec->rDram.u4FRefBufIdx, prVdec->rDram.u4DecBufIdx);
            //vSetVSyncPrmBufPtr(ucEsId, prVdec->rDram.u4DecBufIdx);
            prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
            *pi4RetValue = E_VDEC_SKIPFRAME;
            return;
        }
#endif

        if (cbWMVHeaderOnly)
        {
            if (prVdec->rOther.i4CodecVersion == WMVA)
            {
                vVldByteAlign(ucEsId); //in order to use fgNextStartCode().
            }
            else
            {
                fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.i4SetPos);
            }
            prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
        }
        else
        {
            prVdec->rOther.u4VldPosByte = dwVldRPtr(ucEsId,&prVdec->rOther.u4VldPosBit);
            if (prVdec->rPicLayer.u1PicType == SKIPFRAME && prVdec->rPesInfo.u2SkipVidCount==0)
            {
                prVdec->rOther.u4DecPicNo++; //Robert 060608
                print_vop_header_parameter(ucEsId,/*0,*/ prVdec->rOther.u4DecPicNo, prVdec->rDram.u4FRefBufIdx, prVdec->rDram.u4DecBufIdx);
                prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
                *pi4RetValue = E_VDEC_SKIPFRAME;
                //_WMVSWRst(NULL,NULL);
                //Printf("SKIPFRAME, %d\n", prVdec->rPicLayer.u1PicType);
                vSetVSyncPrmBufPtr(ucEsId,prVdec->rDram.u4DecBufIdx);
                if (prVdec->rOther.i4CodecVersion != WMVA)
                {
                    if (cbResetVDec)
                    {
                        //Software reset
                        vResetVDec(ucEsId);
                        //If you perform vResetVDec(), you need to do the following things.
                        vSetMcBufPtr(ucEsId);
                        vSetVldVFifo(ucEsId);
                        vInitPred(ucEsId);
                    }
                    fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.i4SetPos);
                    //fgVldInitBarrelShifter(ucEsId,prVdec->rOther.i4SetPos);
                }
                else
                {
                    vVldByteAlign(ucEsId); //in order to use fgNextStartCode().
                    if (cbResetVDec)
                    {
                        //Software reset
                        vResetVDec(ucEsId);
                        //If you perform vResetVDec(ucEsId), you need to do the following things.
                        vSetMcBufPtr(ucEsId);
                        vSetVldVFifo(ucEsId);
                        vInitPred(ucEsId);
                    }
#if !defined(IS_WMV_POST_MT5368)  // !!!1008 (for MT5368+, this will cause VLD dead)
                    fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.u4VldPosByte);
#endif
                }
            }
            else
            {
                prVdec->rOther.u1DecState = VDEC_NORM_WAIT_TO_DEC;
                *pi4RetValue = E_VDEC_OK;
            }
        }
		
		if(prVdec->rSeqHdrR.u4PicWidthSrc>VDEC_WMV_MAX_WIDTH 
		|| prVdec->rSeqHdrR.u4PicHeightSrc>VDEC_WMV_MAX_HEIGHT)
	    {
		  *pi4RetValue = E_VDEC_NOT_SUPPORT;
		  return;
	    }
    }
    else
    {
        if (prVdec->rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
            *pi4RetValue = E_VDEC_NOT_SUPPORT;
        }
        else if (prVdec->rOther.i4VprErr == END_OF_FILE)
        {
            prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
            *pi4RetValue = E_VDEC_FAIL;
            //_WMVSWRst(NULL,NULL);
        }
        else if (prVdec->rOther.i4VprErr == E_VDEC_PARSE_NOT_READY)
        {
            *pi4RetValue = E_VDEC_PARSE_NOT_READY;
        }
        else
        {
            prVdec->rOther.u1DecState = VDEC_NORM_ERROR;
            *pi4RetValue = E_VDEC_FAIL;
            //_WMVSWRst(NULL,NULL);
        }
    }
	
	if(prVdec->rSeqHdrR.u4PicWidthSrc>VDEC_WMV_MAX_WIDTH 
		|| prVdec->rSeqHdrR.u4PicHeightSrc>VDEC_WMV_MAX_HEIGHT)
	{
		*pi4RetValue = E_VDEC_NOT_SUPPORT;
		return;
	}
	
}

/** wmv decoder hardware config and decoding trigger
*@param hVdec handler
 */
void _WMVDecStart(UCHAR ucEsId,INT32* pi4RetValue)
{
    VDEC_WMV_INFO_T* prVdec;

    prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    
    if(prVdec->rPicLayer.u1PicType==SKIPFRAME && prVdec->rPesInfo.u2SkipVidCount>0)
    {
        *pi4RetValue = E_VDEC_OK;
    }
	else if (fgVDecProcWMV(ucEsId))
    {
        prVdec->rOther.u1DecState = VDEC_NORM_WAIT_DECODE;
        prVdec->rOther.u4Test01 = 0;
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
void _WMVDecFin(UCHAR ucEsId,INT32* pi4RetValue)
{
    UINT32 dwVldByte, dwVldBit;
    VDEC_WMV_INFO_T* prVdec;
    
    prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    //UINT32 u4Tmp =0;
    //BOOL static fgTimeout = FALSE;
    //UINT8 static u1Times =0;

    do
    {
        if ( (prVdec->rPicLayer.u1PicType==SKIPFRAME && prVdec->rPesInfo.u2SkipVidCount>0) || fgVDecodeEndWMV(ucEsId))
        {
#ifdef CC_WMV_EMULATION
  #ifdef MC_LOG_DUMP
            extern char *get_wmv_cur_filename(void);
            CHAR ucPicType;
            BOOL fgIsFrame;
            
            switch (prVdec->rPicLayer.u1PicType)
            {
            case IVOP:
                ucPicType = 'I';
                break;
            case PVOP:
                ucPicType = 'P';
                break;
            case BVOP:
            case BIVOP:
                ucPicType = 'B';
                break;
            default:
                ASSERT(0);
                ucPicType = '?';
                break;
            }
            
            if (prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE || prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFRAME)
            {
                fgIsFrame = 1;
            }
            else
            {
                fgIsFrame = 0;
            }

            Printf("VDEC_perf_measure:seq_name=%s,pic_idx=%d,pic_width=%d,pic_height=%d\n", get_wmv_cur_filename(), prVdec->rOther.u4CurrPicNo, prVdec->rSeqHdrR.u4PicWidthDec, prVdec->rSeqHdrR.u4PicHeightDec);
            //Printf("VDEC_perf_measure2:pic_type=%c,framepic=%d,CYCLE_DRAM=%ld\n", ucPicType, ((fgIsFrame == TRUE) ? 1:0), ReadREG(ucEsId, (RO_MC_DECODING_CYCLE));
            Printf("VDEC_perf_measure2:framepic=%d,CYCLE_DRAM=%ld\n", (int)fgIsFrame, ReadREG(ucEsId, RO_MC_DECODING_CYCLE));
            Printf("VDEC_perf_measure3:MC_DLE_NUM=%ld,MC_MBX =%ld,MC_MBY =%ld\n", ReadREG(ucEsId, RO_MC_DLE_NUM), ReadREG(ucEsId, RO_MC_MBX), ReadREG(ucEsId, RO_MC_MBY));
            Printf("VDEC_perf_measure4:CYC_SYS=%ld,INTRA_CNT=%ld,Y_BLK_CNT=%ld\n", ReadREG(ucEsId, RO_MC_CYC_SYS), ReadREG(ucEsId, RO_MC_INTRA_CNT), ReadREG(ucEsId, RO_MC_Y_BLK_CNT));
            Printf("VDEC_perf_measure5:C_BLK_CNT=%ld,WAIT_CNT=%ld,REQ_CNT=%ld\n", ReadREG(ucEsId, RO_MC_C_BLK_CNT), ReadREG(ucEsId, RO_MC_WAIT_CNT), ReadREG(ucEsId, RO_MC_REQ_CNT));
            Printf("VDEC_perf_measure6:PARA4_BEHAVIOR=%d,PARA8_BEHAVIOR=%d\n", ((ReadREG(ucEsId, RW_MC_PARA_BEHAVIOR)&PARA4_BEHAVIOR)?1:0), ((ReadREG(ucEsId, RW_MC_PARA_BEHAVIOR)&PARA8_BEHAVIOR)?1:0));
            UNUSED(ucPicType);
  #endif  // MC_LOG_DUMP
#endif  // CC_WMV_EMULATION
#if defined(CC_WMV_EMULATION) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
#if defined(VDEC_CRC_TEST) && !defined(MULTIMEDIA_AUTOTEST_SUPPORT) && !defined(CC_WMV_EMULATION)
            if(VDEC_IsTestCRC(ucEsId))
#endif
            {
                if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD)||(prVdec->rPicLayer.i4CurrentTemporalField == 1))
                {
                    if(prVdec->rPicLayer.u1FrameCodingMode==INTERLACEFIELD)
                    {
                		prVdec->rOther.rCrcValue[1][0][0]=ReadREG(ucEsId,RW_CRC_Y_0);
                		prVdec->rOther.rCrcValue[1][0][1]=ReadREG(ucEsId,RW_CRC_Y_1);
                		prVdec->rOther.rCrcValue[1][0][2]=ReadREG(ucEsId,RW_CRC_Y_2);
                		prVdec->rOther.rCrcValue[1][0][3]=ReadREG(ucEsId,RW_CRC_Y_3);
                		prVdec->rOther.rCrcValue[1][1][0]=ReadREG(ucEsId,RW_CRC_C_0);
                		prVdec->rOther.rCrcValue[1][1][1]=ReadREG(ucEsId,RW_CRC_C_1);
                		prVdec->rOther.rCrcValue[1][1][2]=ReadREG(ucEsId,RW_CRC_C_2);
                		prVdec->rOther.rCrcValue[1][1][3]=ReadREG(ucEsId,RW_CRC_C_3);
                    }
                	else
                	{
                		prVdec->rOther.rCrcValue[0][0][0]=ReadREG(ucEsId,RW_CRC_Y_0);
                		prVdec->rOther.rCrcValue[0][0][1]=ReadREG(ucEsId,RW_CRC_Y_1);
                		prVdec->rOther.rCrcValue[0][0][2]=ReadREG(ucEsId,RW_CRC_Y_2);
                		prVdec->rOther.rCrcValue[0][0][3]=ReadREG(ucEsId,RW_CRC_Y_3);
                		prVdec->rOther.rCrcValue[0][1][0]=ReadREG(ucEsId,RW_CRC_C_0);
                		prVdec->rOther.rCrcValue[0][1][1]=ReadREG(ucEsId,RW_CRC_C_1);
                		prVdec->rOther.rCrcValue[0][1][2]=ReadREG(ucEsId,RW_CRC_C_2);
                		prVdec->rOther.rCrcValue[0][1][3]=ReadREG(ucEsId,RW_CRC_C_3);
                	}
                }
                else
                {
                	ASSERT(prVdec->rPicLayer.u1FrameCodingMode==INTERLACEFIELD);
                		prVdec->rOther.rCrcValue[0][0][0]=ReadREG(ucEsId,RW_CRC_Y_0);
                	prVdec->rOther.rCrcValue[0][0][1]=ReadREG(ucEsId,RW_CRC_Y_1);
                	prVdec->rOther.rCrcValue[0][0][2]=ReadREG(ucEsId,RW_CRC_Y_2);
                	prVdec->rOther.rCrcValue[0][0][3]=ReadREG(ucEsId,RW_CRC_Y_3);
                	prVdec->rOther.rCrcValue[0][1][0]=ReadREG(ucEsId,RW_CRC_C_0);
                	prVdec->rOther.rCrcValue[0][1][1]=ReadREG(ucEsId,RW_CRC_C_1);
                	prVdec->rOther.rCrcValue[0][1][2]=ReadREG(ucEsId,RW_CRC_C_2);
                	prVdec->rOther.rCrcValue[0][1][3]=ReadREG(ucEsId,RW_CRC_C_3);            
                }
            }
#endif  // CC_WMV_EMULATION || MULTIMEDIA_AUTOTEST_SUPPORT
            PostAdjustReconRange(ucEsId);
            vSetVSyncPrmBufPtr(ucEsId,prVdec->rDram.u4DecBufIdx);
            //u1Times =0;
            //vVSyncWriteVdoReg(ucEsId);
            if (prVdec->rOther.i4CodecVersion != WMVA)
            {
                if (cbResetVDec) //TODO: add for Advanced Profile.
                {
                    vResetVDec(ucEsId);
                    //If you perform vResetVDec(), you need to do the following things.
                    vSetMcBufPtr(ucEsId);
                    vSetVldVFifo(ucEsId);
                    vInitPred(ucEsId);
                    prVdec->rOther.u4Datain=dwGetBitStream(ucEsId,0);
                    fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.i4SetPos);
                    prVdec->rOther.u4Datain=dwGetBitStream(ucEsId,0);
                }
                else
                {
                    //Set VLD RPtr to the position of the next picture.
#if defined(IS_WMV_POST_MT5368)
                    vResetVDec(ucEsId);  // !!!1008: this will clear CRC values and some register setting!
#endif
                    fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + prVdec->rOther.i4SetPos);
                    //fgVldInitBarrelShifter(prVdec->rOther.i4SetPos);
                    LOG(6,"init barrel shifter2 %08x\n",prVdec->rOther.i4SetPos);
                }
            }
            else // WMVA
            {
                if (cbResetVDec)
                {
                    dwVldByte = dwVldRPtr(ucEsId,&dwVldBit);
                    vResetVDec(ucEsId);
                    //If you perform vResetVDec(ucEsId), you need to do the following things.
                    vSetMcBufPtr(ucEsId);
                    vSetVldVFifo(ucEsId);
                    vInitPred(ucEsId);
                    fgVldInitBarrelShifter(ucEsId,((UINT32)prVdec->rDram.pu1VFifo) + dwVldByte);
                }
            }
            prVdec->rOther.u1DecState = VDEC_NORM_DEC_END;
            if ((prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFIELD)||(prVdec->rPicLayer.i4CurrentTemporalField == 1))
            {
                if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    prVdec->rPicLayer.i4CurrentTemporalField ^= 1;  //toggle field
                    prVdec->rPicLayer.i4CurrentField ^= 1;
                }
                prVdec->rOther.u4CurrPicNo++;
                *pi4RetValue = E_VDEC_OK;
            }
            else
            {
                *pi4RetValue = E_VDEC_DISP_NOT_READY;
                if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    prVdec->rPicLayer.i4CurrentTemporalField ^= 1;  //toggle field
                    prVdec->rPicLayer.i4CurrentField ^= 1;
                }
            }
            //LOG(6,"decode finish RPTR : 0x%08x,WPTR : 0x%08x\n",(prVdec->rPesInfo.u4FifoStart+dwVldRPtr(ucEsId,&dwVldBit)),ReadREG(ucEsId,RO_VLD_VWPTR));
            //LOG(6,"decode finish Rptr 0x%08x, input win 0x%08x\n", dwVldRPtr(ucEsId,&dwVldBit), ReadREG(ucEsId,RO_VLD_BARL));  !!!1006 why this causes VLD dead on 5368?
            return;
        }
        else
        {
            //UINT32 u4Idx =0;
            //*pi4RetValue = E_VDEC_FAIL;
            *pi4RetValue = E_VDEC_OK;
            vSetVSyncPrmBufPtr(ucEsId,prVdec->rDram.u4DecBufIdx);
            if (prVdec->rPicLayer.u1PicType == IVOP)
            {
                LOG(6,"frame type = IVOP\n");
            }
            else if (prVdec->rPicLayer.u1PicType == BVOP)
            {
                LOG(6,"frame type = BVOP\n");
            }
            else if (prVdec->rPicLayer.u1PicType == PVOP)
            {
                LOG(6,"frame type = PVOP\n");
            }
            else if (prVdec->rPicLayer.u1PicType == BIVOP)
            {
                LOG(6,"frame type = BIVOP\n");
            }
            else
            {
                LOG(6,"frame type = skip frame\n");
            }

            LOG(1,"MBX : %d \nMBY : %d \n",ReadREG(ucEsId,RO_MC_MBX),ReadREG(ucEsId,RO_MC_MBY));
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
            LOG(1,"//===========Decode Timeout=========\n");
#ifdef CC_WMV_EMULATION
            ASSERT(0);
#endif
#if 0
            printf("dump VLD state for timeout\n\n");
            for (u4Idx =100 ; u4Idx<=250 ; u4Idx++)
            {
                printf("RISC Read VLD(%d) = 0x%08x \n", u4Idx,ReadREG(ucEsId,VLD_REG_OFST + u4Idx*4));
            }
            printf("\n\ndump VLD state for timeout\n");
#endif
            //fgTimeout = FALSE;
            return;
        }
    }
    while (1);
}

/** wmv SW reset
*@param hVdec handler
 */
void _WMVSWRst(UCHAR ucEsId,INT32* pi4RetValue)
{
    UINT32 u4Temp;
    
    vResetVDec(ucEsId);
    vSetMcBufPtr(ucEsId);
    vSetVldVFifo(ucEsId);
    vInitPred(ucEsId);
#ifdef CC_WMV_EMULATION
    WriteREG(RW_MC_ADDRSWAP, 0x3);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x3);
#else
    WriteREG(RW_MC_ADDRSWAP, 0x2);
    WriteREG(RW_MC_NBM_ADDRSWAP, 0x2);
#endif
    u4Temp = ReadREG(ucEsId,MC_REG_325);
    u4Temp |= (0x1000);
    WriteREG(MC_REG_325, u4Temp);

    u4Temp = ReadREG(ucEsId, MC_REG_515);
    u4Temp &= 0xFFEFFFFF;  // clear bit 20 to enhance DRAM performance
#if defined(IS_WMV_POST_MT5368)
    u4Temp &= 0xFBFFFFFF;  // bit 26: PRED to DRAM
    if (cbWMVFWBP)
    {
        u4Temp |= 0x08000000;  // bit 27: BP to DRAM
    }
#else
    if (cbWMVFWBP)
    {
        u4Temp |= 0x01000000;  // bit 24: BP to DRAM
    }
#endif
    WriteREG(MC_REG_515, u4Temp);

    //WriteREG(RW_CRC_CTRL, 0x1);

#if !defined(IS_WMV_POST_MT5368)
    u4Temp = ReadREG(ucEsId, RW_VLD_ASYNC);
    u4Temp = u4Temp | (1<<17);
    WriteREG(RW_VLD_ASYNC, u4Temp);
#endif

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
