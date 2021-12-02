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
 * $RCSfile: vdec_h264psr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_h264psr.c
 *  This file contains implementation of exported APIs of VDEC.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
#include "vdec_h264dec.h"
#include "vdec_h264util.h"
#include "vdec_h264hw.h"
#include "fbm_drvif.h"
#include "x_timer.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
const UINT8 NumClockTS[9] =
{
   1,  1,  1,  2,  2,  3,  3,  2,  3
};

const CHAR ZZ_SCAN[16]  =
{  0,  1,  4,  8,  5,  2,  3,  6,  9, 12, 13, 10,  7, 11, 14, 15
};

const CHAR ZZ_SCAN8[64] =
{  0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
   12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
   35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
   58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63
};

//Error handling
#ifdef FBM_DPB_CTRL
#define CHKPARAM(u4Param, u4HBound)\
    if(u4Param > u4HBound)\
    {\
        if(prDecPrm->prCurrFBufInfo)\
        {\
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) == FBM_FB_STATUS_DECODE)\
            {\
                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_EMPTY);\
                _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx);\
            }\
        }\
        LOG(2, "Hdr err\n");\
        *pi4RetValue = 1;\
        return;\
    }

#define CHKPARAM_R(i4Param, i4LBound, i4HBound)\
    if((i4Param < i4LBound) || (i4Param > i4HBound))\
    {\
        if(prDecPrm->prCurrFBufInfo)\
        {\
            if(FBM_GetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId) == FBM_FB_STATUS_DECODE)\
            {\
                FBM_SetFrameBufferStatus(prDecPrm->ucFbgId, prDecPrm->prCurrFBufInfo->ucFbId, FBM_FB_STATUS_EMPTY);\
                _VDEC_ClrFBufInfo(prDecPrm, prDecPrm->u1DecFBufIdx);\
            }\
        }\
        LOG(2, "Hdr err\n");\
        *pi4RetValue = 1;\
        return;\
    }

#else
#define CHKPARAM(u4Param, u4HBound)\
    if(u4Param > u4HBound)\
    {\
        LOG(2, "Hdr err\n");\
        *pi4RetValue = 1;\
        return;\
    }

#define CHKPARAM_R(i4Param, i4LBound, i4HBound)\
    if((i4Param < i4LBound) || (i4Param > i4HBound))\
    {\
        LOG(2, "Hdr err\n");\
        *pi4RetValue = 1;\
        return;\
    }
    
#endif

#define AFD_IDENTIFIER_DTG1 0x44544731
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _fgIsMoreRbspData(void);

static UINT32 _InitSPS(VDEC_AVC_SPS_T* prSPS);

static void _vInitSliceHdr(VDEC_AVC_SLICE_HDR_T* prSliceHdr, UINT32 u4NumRefIdxL0, UINT32 u4NumRefIdxL1);

static void _vHrdParameters(VDEC_AVC_HRD_PARAM_T* prHrdPara);

static void _vRefPicMarking(BOOL bIsIDR, VDEC_AVC_SLICE_HDR_T* prSliceHdr);

static void _vScalingList(CHAR* pcScalingList, UINT32 u4SizeOfScalingList, BOOL *fgUseDefaultScalingMatrixFlag);

static BOOL _vInterpretSEI_PicTiming(VDEC_AVC_DecPrm* prDecPrm);

//static void _vInterpretSEI_BufferingPeriodInfo(VDEC_H264_INFO_T* prVdec);

#ifdef FBM_DPB_CTRL
extern void FBM_UpdateReferenceList(UCHAR ucFbgId, UCHAR ucFbId, BOOL fgReference);
#endif


// *********************************************************************
// Function    : BOOL fgIsMoreRbspData()
// Description : 
// Parameter   : None
// Return      : TRUE: (i)the next bit is 0
//                            (ii)the next bit is 1, but the remaining bits != 0
//                   FALSE: otherse
// *********************************************************************
static BOOL _fgIsMoreRbspData(void)
{
    UINT32 u4RemainedBits;
    UINT32 u4Temp;
    UINT32 i;
  
    u4RemainedBits = (_VDEC_AVCVLDShiftBits() % 8); //0~7
    //u4RemainedBits = (8 - (((dReadAVCVLD(RW_AVLD_CTRL) >> 16) & 0x3F) % 8));  
    u4Temp = 0xffffffff;
    for(i=0; i<=u4RemainedBits; i++)
    {
        u4Temp &= (~((UINT32)1<<i));
    }
  
    if((_VDEC_AVCGetBitStream(0) & u4Temp) == (UINT32)(0x80000000))
    {
        // no more
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

// *********************************************************************
// Function    : void vInitSPS(TSeqParameterSetRbsp *ptSPS)
// Description : Init SPS related fields
// Parameter   : None
// Return      : None
// *********************************************************************
static UINT32 _InitSPS(VDEC_AVC_SPS_T* prSPS)
{
    INT32 i;
    UINT32 u4DpbSize;

    ASSERT(prSPS!=NULL);
      
    prSPS->u4ChromaFormatIdc = 1;
    prSPS->u4BitDepthLumaMinus8 = 0;
    prSPS->u4BitDepthChromaMinus8 = 0;
    prSPS->fgQpprimeYZeroTransformBypassFlag = FALSE;  
    prSPS->fgSeqScalingMatrixPresentFlag = FALSE;
    for(i=0; i<8; i++)
    {
        prSPS->fgSeqScalingListPresentFlag[i] = FALSE;  
    }

    switch (prSPS->u4LevelIdc)
    {
        case 10:
            u4DpbSize = 152064;
            break;
        case 11:
            u4DpbSize = 345600;
            break;
        case 12:
            u4DpbSize = 912384;
            break;
        case 13:
            u4DpbSize = 912384;
            break;
        case 20:
            u4DpbSize = 912384;
            break;
        case 21:
            u4DpbSize = 1824768;
            break;
        case 22:
            u4DpbSize = 3110400;
            break;
        case 30:
            u4DpbSize = 3110400;
            break;
        case 31:
            u4DpbSize = 6912000;
            break;
        case 32:
            u4DpbSize = 7864320;
            break;
        default:    //40 or upper
            u4DpbSize = 12582912;
            break;
    }
    return u4DpbSize;
  
}

static void _vInitSliceHdr(VDEC_AVC_SLICE_HDR_T* prSliceHdr, UINT32 u4NumRefIdxL0, UINT32 u4NumRefIdxL1)
{
    ASSERT(prSliceHdr!=NULL);
    
    prSliceHdr->ai4DeltaPicOrderCnt[0] = 0;
    prSliceHdr->ai4DeltaPicOrderCnt[1] = 0;
    prSliceHdr->u4RedundantPicCnt = 0;  
    prSliceHdr->u4DisableDeblockingFilterIdc = 0;  
    prSliceHdr->i4SliceAlphaC0OffsetDiv2 = 0;    
    prSliceHdr->i4SliceBetaOffsetDiv2 = 0;    
    prSliceHdr->u4NumRefIdxL0ActiveMinus1 = u4NumRefIdxL0;
    prSliceHdr->u4NumRefIdxL1ActiveMinus1 = u4NumRefIdxL1;
    prSliceHdr->u4LongTermFrameIdx = 0xffffffff; 
    prSliceHdr->u4LongTermPicNum = 0;  
    prSliceHdr->fgDirectSpatialMvPredFlag = FALSE;
    prSliceHdr->u4IdrPicId = 0;
}

// *********************************************************************
// Function    : void vInitSliceHdr()
// Description : 
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vHrdParameters(VDEC_AVC_HRD_PARAM_T* prHrdPara)
{
    UINT32 u4SchedSelIdx;

      ASSERT(prHrdPara!=NULL);
    prHrdPara->u4CpbCntMinus1 = _VDEC_UeCodeNum();
    prHrdPara->u4BitRateScale = _VDEC_AVCGetRealShtBitStream(4);
    prHrdPara->u4CpbSizeScale = _VDEC_AVCGetRealShtBitStream(4);
    if(prHrdPara->u4CpbCntMinus1 >= MAXIMUMVALUEOFcpb_cnt)
    {
        LOG(2, "HrdPrs Err\n");
        return;
    }
    for(u4SchedSelIdx=0; u4SchedSelIdx<=  prHrdPara->u4CpbCntMinus1; u4SchedSelIdx++)
    {
        prHrdPara->u4BitRateValueMinus1[u4SchedSelIdx] = _VDEC_UeCodeNum();
        prHrdPara->u4CpbSizeValueMinus1[u4SchedSelIdx] = _VDEC_UeCodeNum();
        prHrdPara->fgCbrFlag[u4SchedSelIdx] = _VDEC_AVCGetBitStreamFlg();
    }
    prHrdPara->u4InitialCpbRemovalDelayLengthMinus1 = _VDEC_AVCGetRealShtBitStream(5);  
    prHrdPara->u4CpbRemovalDelayLengthMinus1 = _VDEC_AVCGetRealShtBitStream(5);  
    prHrdPara->u4DpbOutputDelayLengthMinus1 = _VDEC_AVCGetRealShtBitStream(5);  
    prHrdPara->u4TimeOffsetLength = _VDEC_AVCGetRealShtBitStream(5);  
}

// *********************************************************************
// Function    : void vDec_Ref_Pic_Marking(void)
// Description : mark ref pic
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vRefPicMarking(BOOL bIsIDR, VDEC_AVC_SLICE_HDR_T* prSliceHdr)
{    
    UINT32 u4Cnt;

    ASSERT(prSliceHdr!=NULL);

    if(bIsIDR)
    {
        prSliceHdr->fgNoOutputOfPriorPicsFlag = _VDEC_AVCGetBitStreamFlg();
        prSliceHdr->fgLongTermReferenceFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSliceHdr->fgLongTermReferenceFlag)
        {
        }
        else
        {
        }
    }
    else
    {
        prSliceHdr->fgAdaptiveRefPicMarkingModeFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSliceHdr->fgAdaptiveRefPicMarkingModeFlag)
        {
            u4Cnt = 0;
            do
            {
                if(u4Cnt >= 17)
                {
                    LOG(2, "Error! MMCO OperationNum >= 17\n");
                    break;
                }            
                prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] = _VDEC_UeCodeNum();
                if((prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] == 1))
                {
                    prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] |= (_VDEC_UeCodeNum() << 8);
                }
                else if(prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] == 2)
                {
                    prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] |= (_VDEC_UeCodeNum() << 8);
                }
                else if(prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] == 3)
                {
                    prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] |= (_VDEC_UeCodeNum() << 8);
                    prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] |= (_VDEC_UeCodeNum() << 16);          
                }
                else if(prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] == 4)
                {
                    prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] |= (_VDEC_UeCodeNum() << 8);                    
                }
                else if(prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] == 5)
                {
                    prSliceHdr->fgMmco5 = TRUE;
                }
                else if(prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] == 6)
                {
                    prSliceHdr->au4MemoryManagementControlOperation[u4Cnt] |= (_VDEC_UeCodeNum() << 8);          
                }        
                u4Cnt ++;
            }while(prSliceHdr->au4MemoryManagementControlOperation[u4Cnt-1] != 0);
        }
    }
}


// *********************************************************************
// Function    : void vScalingList(void)
// Description : 
// Parameter   : None
// Return      : None
// *********************************************************************
static void _vScalingList(CHAR* pcScalingList, UINT32 u4SizeOfScalingList, BOOL *fgUseDefaultScalingMatrixFlag)
{
    UINT32 i;
    UINT8 u1Scanj;
    INT32 i4LastScale;
    INT32 i4NextScale;
    INT32 i4DeltaScale;

     ASSERT(pcScalingList!=NULL);
     ASSERT(fgUseDefaultScalingMatrixFlag!=NULL);
     
    i4LastScale = 8;
    i4NextScale = 8;

    for(i=0; i<u4SizeOfScalingList; i++)
    {
        u1Scanj = (UINT8)((u4SizeOfScalingList==16) ? ZZ_SCAN[i]:ZZ_SCAN8[i]);
    
        if(i4NextScale != 0)
        {
            i4DeltaScale = _VDEC_SeCodeNum();
            i4NextScale = (i4LastScale + i4DeltaScale + 256) % 256;
            *fgUseDefaultScalingMatrixFlag = ((u1Scanj == 0) && (i4NextScale == 0))? TRUE : FALSE;
        }
        pcScalingList[u1Scanj] = (CHAR)((i4NextScale == 0)? i4LastScale : i4NextScale);
        i4LastScale = pcScalingList[u1Scanj];
    }
}


static BOOL _vInterpretSEI_PicTiming(VDEC_AVC_DecPrm* prDecPrm)
{
    VDEC_AVC_SEI_T* prSEI;
    VDEC_AVC_VUI_T* prVUI;
    UINT8 u1Idx;
    
    ASSERT(prDecPrm != NULL);
    prSEI = &prDecPrm->rSEI;
    if((prDecPrm->prSPS != NULL) && prDecPrm->prSPS->fgSPSValid)
    {
        prVUI = &prDecPrm->prSPS->rVUI;
    }
    else
    {
        return FALSE;
    }
    
    if(prVUI->fgNalHrdParametersPresentFlag)
    {
        prSEI->u4CpbRemovalDelay = _VDEC_AVCGetRealShtBitStream
            (prVUI->rNalHrdParameters.u4CpbRemovalDelayLengthMinus1 + 1);
        prSEI->u4DpbOutputDelay = _VDEC_AVCGetRealShtBitStream
            (prVUI->rNalHrdParameters.u4DpbOutputDelayLengthMinus1 + 1);        
    }
    else if(prVUI->fgVclHrdParametersPresentFlag)
    {
        prSEI->u4CpbRemovalDelay = _VDEC_AVCGetRealShtBitStream
            (prVUI->rVclHrdParameters.u4CpbRemovalDelayLengthMinus1 + 1);
        prSEI->u4DpbOutputDelay = _VDEC_AVCGetRealShtBitStream
            (prVUI->rVclHrdParameters.u4DpbOutputDelayLengthMinus1 + 1);
    }

    if(prVUI->fgPicStructPresentFlag)
    {
        prSEI->u1PicStruct = (UINT8)_VDEC_AVCGetRealShtBitStream(4);
        if(prSEI->u1PicStruct > 8)
        {
            LOG(2, "PicStructPrs Err\n");
            return TRUE;
        }
        ASSERT(NumClockTS[prSEI->u1PicStruct] < 5);
        for(u1Idx = 0; (u1Idx < NumClockTS[prSEI->u1PicStruct]) && (u1Idx < 5); u1Idx++)
        {
            prSEI->u1ClkTimeStampFlag[u1Idx] = (UINT8)_VDEC_AVCGetRealShtBitStream(1);
            if(prSEI->u1ClkTimeStampFlag[u1Idx] > 0)
            {
                prSEI->u1CtType = (UINT8)_VDEC_AVCGetRealShtBitStream(2);
                prSEI->u1NuitFieldBasedFlag = (UINT8)_VDEC_AVCGetRealShtBitStream(1);
                prSEI->u1CountingType = (UINT8)_VDEC_AVCGetRealShtBitStream(5);
                prSEI->u1FullTimeStampFlag = (UINT8)_VDEC_AVCGetRealShtBitStream(1);
                prSEI->u1DiscontinuityFlag = (UINT8)_VDEC_AVCGetRealShtBitStream(1);
                prSEI->u1CntDroppedFlag = (UINT8)_VDEC_AVCGetRealShtBitStream(1);
                prSEI->u1NFrames = (UINT8)_VDEC_AVCGetRealShtBitStream(8);
                if(prSEI->u1FullTimeStampFlag > 0)
                {
                    prSEI->u1SecondsValue = (UINT8)_VDEC_AVCGetRealShtBitStream(6);
                    prSEI->u1MinutesValue = (UINT8)_VDEC_AVCGetRealShtBitStream(6);
                    prSEI->u1HoursValue = (UINT8)_VDEC_AVCGetRealShtBitStream(5);
                }
                else
                {
                    if(_VDEC_AVCGetRealShtBitStream(1) > 0)    //seconds_flag
                    {
                        prSEI->u1SecondsValue = (UINT8)_VDEC_AVCGetRealShtBitStream(6);
                        if(_VDEC_AVCGetRealShtBitStream(1) > 0)    //minutes_flag
                        {
                            prSEI->u1MinutesValue = (UINT8)_VDEC_AVCGetRealShtBitStream(6);
                            if(_VDEC_AVCGetRealShtBitStream(1) > 0)    //hours_flag
                            {
                                prSEI->u1HoursValue = (UINT8)_VDEC_AVCGetRealShtBitStream(5);
                            }
                        }
                    }
                }
                if(prVUI->rNalHrdParameters.u4TimeOffsetLength > 0)
                {
                    prSEI->i1TimeOffset = (INT8)_VDEC_AVCGetRealShtBitStream(
                        prVUI->rNalHrdParameters.u4TimeOffsetLength);
                }
                else
                {
                    prSEI->i1TimeOffset = 0;
                }
            }
        }
    }

    return TRUE;
}

/*static void _vInterpretSEI_BufferingPeriodInfo(VDEC_H264_INFO_T* prVdec)
{
    UINT32 u4SPSID;
    UINT32 u4InitCpbRemovalDelay = 0;
    UINT32 u4InitCpbRemovalDelayOffset = 0;
    INT32 i4Idx;
    VDEC_AVC_DecPrm* prDecPrm;

    ASSERT(prVdec != NULL);
    
    prDecPrm = &prVdec->rDecParam;  
    u4SPSID = _VDEC_UeCodeNum();
    if(prVdec->arSPS[u4SPSID].fgSPSValid)
    {
        if((!prDecPrm->prSliceHdr->fgNoOutputOfPriorPicsFlag) &&
            (prDecPrm->prSPS != (&prVdec->arSPS[u4SPSID])))
        {
            _VDEC_FlushBufRefInfo(prDecPrm);
        }
        prDecPrm->prSPS = &prVdec->arSPS[u4SPSID];
        if(prDecPrm->prSPS->fgVuiParametersPresentFlag)
        {
            if(prDecPrm->prSPS->rVUI.fgNalHrdParametersPresentFlag)
            {
                for (i4Idx=0; i4Idx < (INT32)(prDecPrm->prSPS->rVUI.rNalHrdParameters.u4CpbCntMinus1+1); i4Idx++)
                {
                    u4InitCpbRemovalDelay = _VDEC_AVCGetRealShtBitStream
                        (prDecPrm->prSPS->rVUI.rNalHrdParameters.u4InitialCpbRemovalDelayLengthMinus1 + 1);
                    u4InitCpbRemovalDelayOffset = _VDEC_AVCGetRealShtBitStream
                        (prDecPrm->prSPS->rVUI.rNalHrdParameters.u4InitialCpbRemovalDelayLengthMinus1 + 1);
                }
            }
        
            if(prDecPrm->prSPS->rVUI.fgVclHrdParametersPresentFlag)
            {
                for (i4Idx=0; i4Idx < (INT32)(prDecPrm->prSPS->rVUI.rVclHrdParameters.u4CpbCntMinus1+1); i4Idx++)
                {
                    u4InitCpbRemovalDelay = _VDEC_AVCGetRealShtBitStream
                        (prDecPrm->prSPS->rVUI.rVclHrdParameters.u4InitialCpbRemovalDelayLengthMinus1 + 1);
                    u4InitCpbRemovalDelayOffset = _VDEC_AVCGetRealShtBitStream
                        (prDecPrm->prSPS->rVUI.rVclHrdParameters.u4InitialCpbRemovalDelayLengthMinus1 + 1);
                }
            }
        }
    }
    UNUSED(u4InitCpbRemovalDelay);
    UNUSED(u4InitCpbRemovalDelayOffset);
}*/

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

/** .
*@param .
 */
UINT8 _VDEC_AVCGetStartCode(UINT8 u1Num)
{
    UINT32 u4Temp;
    BOOL fgVLDRem03;

    u4Temp = _VDEC_AVCGetBitStream(0);
    fgVLDRem03 = (u1Num == BARREL1)? (AVC_VLD_READ32(RW_AVLD_CTRL) >> 31) : (AVC_VLD_READ32(RO_AVLD_2ND_CTRL) & 0x40);

    while(((u4Temp >> 8) != START_CODE) || fgVLDRem03)
    {
        u4Temp = _VDEC_AVCGetBitStream(8);
        fgVLDRem03 = (u1Num == BARREL1)? (AVC_VLD_READ32(RW_AVLD_CTRL) >> 31) : (AVC_VLD_READ32(RO_AVLD_2ND_CTRL) & 0x40);
    }
    u4Temp = _VDEC_AVCGetShtBitStream(32);
    return (UINT8)(u4Temp & 0xff);
}


// *********************************************************************
// Function    : void vSeq_Par_Set_Rbsp(void)
// Description : Handle picture parameter set header
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_SPSrbsp(VDEC_H264_INFO_T* prVdec, INT32* pi4RetValue)
{
    UINT32 u4Temp;
    UINT32 i;
    UINT32 u4SeqParameterSetId;
    VDEC_AVC_SPS_T* prSPS;
    #ifdef FBM_DPB_CTRL
    VDEC_AVC_DecPrm* prDecPrm;
    #endif
    
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);

    prDecPrm = &prVdec->rDecParam;
    
    u4Temp = (_VDEC_AVCGetShtBitStream(24) >> 8);
    if((u4Temp & 0xf00) > 0)  // [11:8]
    {
        *pi4RetValue = 1;
        LOG(2, "reserved_zero_4bits error in Seq_Par_Set_Rbsp\n");
        return;
    }

    // 1st
    u4SeqParameterSetId = _VDEC_UeCodeNum();
    if(u4SeqParameterSetId < 32)
    {      
        prSPS = &prVdec->arSPS[u4SeqParameterSetId];
        x_memset((void*)prSPS, 0, sizeof(VDEC_AVC_SPS_T));
        prVdec->rDecParam.prSPS = prSPS;
        prSPS->fgSPSValid = FALSE;    //turn on the flag after all parameters are set

        prSPS->u4ProfileIdc = (u4Temp >> 16);                              // [23:16]
        prSPS->fgConstrainedSet0Flag = (u4Temp >> 15) & 0x1;      // [15]
        prSPS->fgConstrainedSet1Flag = (u4Temp >> 14) & 0x1;      // [14]
        prSPS->fgConstrainedSet2Flag = (u4Temp >> 13) & 0x1;      // [13]
        prSPS->fgConstrainedSet3Flag = (u4Temp >> 12) & 0x1;      // [12]
        prSPS->u4LevelIdc = (u4Temp & 0xff);                              // [7:0]
        if((!fgChkProfile((UINT8)prSPS->u4ProfileIdc)) || (!fgChkLevel((UINT8)prSPS->u4LevelIdc)))
        {
            *pi4RetValue = 1;
            LOG(2, "err at SPS Profile/Level\n");
            return;
        }
        prSPS->u4SeqParameterSetId = u4SeqParameterSetId;
    }
    else
    {
        *pi4RetValue = 1;
        LOG(2, "err in SPS Num\n");
        return;
    }
    prDecPrm->u4DPBSize = _InitSPS(prSPS);

    // 2nd
    if((prSPS->u4ProfileIdc == FREXT_HP) 
        || (prSPS->u4ProfileIdc == FREXT_Hi10P) 
        || (prSPS->u4ProfileIdc == FREXT_Hi422)
        || (prSPS->u4ProfileIdc == FREXT_Hi444))
    {
        prSPS->u4ChromaFormatIdc = _VDEC_UeCodeNum();
        CHKPARAM(prSPS->u4ChromaFormatIdc, 3);
        if(prSPS->u4ChromaFormatIdc == 3)
        {
            prSPS->fgResidualColorTransformFlag = _VDEC_AVCGetBitStreamFlg();
        }
        prSPS->u4BitDepthLumaMinus8 = _VDEC_UeCodeNum();
        CHKPARAM(prSPS->u4BitDepthLumaMinus8, 4);
        prSPS->u4BitDepthChromaMinus8 = _VDEC_UeCodeNum();
        CHKPARAM(prSPS->u4BitDepthChromaMinus8, 4);
        prSPS->fgQpprimeYZeroTransformBypassFlag = _VDEC_AVCGetBitStreamFlg();

        prSPS->fgSeqScalingMatrixPresentFlag = _VDEC_AVCGetBitStreamFlg();

        if(prSPS->fgSeqScalingMatrixPresentFlag)
        {
            for(i = 0; i < 8; i ++)
            {
                prSPS->fgSeqScalingListPresentFlag[i] = _VDEC_AVCGetBitStreamFlg();
                if(prSPS->fgSeqScalingListPresentFlag[i])
                {
                    if(i < 6)
                    {
                        _vScalingList(prSPS->ppcScalingList4x4[i],16, &prSPS->fgUseDefaultScalingMatrix4x4Flag[i]);
                    }
                    else
                    {
                        _vScalingList(prSPS->ppcScalingList8x8[i-6],64, &prSPS->fgUseDefaultScalingMatrix8x8Flag[i-6]);
                    }
                }
            }
        }
    }

    prSPS->u4Log2MaxFrameNumMinus4 = _VDEC_UeCodeNum();
    CHKPARAM(prSPS->u4Log2MaxFrameNumMinus4, 12);
    prSPS->u4MaxFrameNum = 1 << (prSPS->u4Log2MaxFrameNumMinus4 + 4);
    prSPS->u4PicOrderCntType = _VDEC_UeCodeNum();
    CHKPARAM(prSPS->u4PicOrderCntType, 2);
    if(prSPS->u4PicOrderCntType == 0)
    {
        prSPS->u4Log2MaxPicOrderCntLsbMinus4 = _VDEC_UeCodeNum();
        CHKPARAM(prSPS->u4Log2MaxPicOrderCntLsbMinus4, 12);
    }
    else if(prSPS->u4PicOrderCntType == 1)
    {
        prSPS->fgDeltaPicOrderAlwaysZeroFlag = _VDEC_AVCGetBitStreamFlg();

        prSPS->i4OffsetForNonRefPic = _VDEC_SeCodeNum();
        prSPS->i4OffsetForTopToBottomField = _VDEC_SeCodeNum();
        prSPS->u4NumRefFramesInPicOrderCntCycle = _VDEC_UeCodeNum();
        CHKPARAM(prSPS->u4NumRefFramesInPicOrderCntCycle, 255);
        for(i=0 ; i<prSPS->u4NumRefFramesInPicOrderCntCycle; i++)
        {
            prSPS->ai4OffsetForRefFrame[i] = _VDEC_SeCodeNum();
        }
    }
    prSPS->u4NumRefFrames = _VDEC_UeCodeNum();
    CHKPARAM(prSPS->u4NumRefFrames, 16);

    prSPS->fgGapsInFrameNumValueAllowedFlag = _VDEC_AVCGetBitStreamFlg();
    prSPS->u4PicWidthInMbsMinus1 = _VDEC_UeCodeNum();

    prSPS->u4PicHeightInMapUnitsMinus1 = _VDEC_UeCodeNum();

    prSPS->fgFrameMbsOnlyFlag = _VDEC_AVCGetBitStreamFlg();
    if((prSPS->u4ProfileIdc >= 77) // upper than Main Profile
        && ((prSPS->u4LevelIdc <= 20) || (prSPS->u4LevelIdc >= 42)))
    {
        prSPS->fgFrameMbsOnlyFlag = 1;
    }

    prSPS->fgMbAdaptiveFrameFieldFlag = FALSE;
    if(!prSPS->fgFrameMbsOnlyFlag)
    {
        prSPS->fgMbAdaptiveFrameFieldFlag = _VDEC_AVCGetBitStreamFlg();
    }
    prSPS->fgDirect8x8InferenceFlag = _VDEC_AVCGetBitStreamFlg();
    /*if((prSPS->u4ProfileIdc >= 77) // upper than Main Profile
        && (prSPS->u4LevelIdc >= 30))
    {
        prSPS->fgDirect8x8InferenceFlag = 1;
    }*/  

    prSPS->fgFrameCroppingFlag = _VDEC_AVCGetBitStreamFlg();

    if(prSPS->fgFrameCroppingFlag)
    {
        prSPS->u4FrameCropLeftOffset = _VDEC_UeCodeNum();
        prSPS->u4FrameCropRightOffset = _VDEC_UeCodeNum();

        prSPS->u4FrameCropTopOffset = _VDEC_UeCodeNum();
        prSPS->u4FrameCropBottomOffset = _VDEC_UeCodeNum();
        if(prSPS->u4ChromaFormatIdc == 1)   // 4:2:0
        {
            prSPS->u4FrameCropLeftOffset <<= 1;
            prSPS->u4FrameCropRightOffset <<= 1;

            prSPS->u4FrameCropTopOffset <<= 1;
            prSPS->u4FrameCropBottomOffset <<= 1;
        }
        else if(prSPS->u4ChromaFormatIdc == 2)   // 4:2:2
        {
            prSPS->u4FrameCropLeftOffset <<= 1;
            prSPS->u4FrameCropRightOffset <<= 1;
        }
    }
    prSPS->fgVuiParametersPresentFlag = _VDEC_AVCGetBitStreamFlg();
    
    #if 1
    if(prSPS->fgVuiParametersPresentFlag)
    {
        prSPS->rVUI.fgAspectRatioInfoPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgAspectRatioInfoPresentFlag)
        {
            prSPS->rVUI.u4AspectRatioIdc = _VDEC_AVCGetRealShtBitStream(8);
            if(prSPS->rVUI.u4AspectRatioIdc == 255) //Extended_SAR
            {
                prSPS->rVUI.u4SarWidth = _VDEC_AVCGetRealShtBitStream(16);
                prSPS->rVUI.u4SarHeight = _VDEC_AVCGetRealShtBitStream(16);
            }
        }
        prSPS->rVUI.fgOverscanInfoPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgOverscanInfoPresentFlag)
        {
            prSPS->rVUI.fgOverscanAppropriateFlag = _VDEC_AVCGetBitStreamFlg();
        }
        prSPS->rVUI.fgVideoSignalTypePresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgVideoSignalTypePresentFlag)
        {
            prSPS->rVUI.u4VideoFormat = _VDEC_AVCGetRealShtBitStream(3); 
            prSPS->rVUI.fgVideoFullRangeFlag = _VDEC_AVCGetBitStreamFlg();
            prSPS->rVUI.fgColourDescriptionPresentFlag = _VDEC_AVCGetBitStreamFlg();
            if(prSPS->rVUI.fgColourDescriptionPresentFlag)
            {
                prSPS->rVUI.u4ColourPrimaries = _VDEC_AVCGetRealShtBitStream(8); 
                prSPS->rVUI.u4TransferCharacteristics = _VDEC_AVCGetRealShtBitStream(8); 
                prSPS->rVUI.u4MatrixCoefficients = _VDEC_AVCGetRealShtBitStream(8); 
            }
        }
        prSPS->rVUI.fgChromaLocationInfoPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgChromaLocationInfoPresentFlag)
        {
            prSPS->rVUI.u4ChromaSampleLocTypeTopField = _VDEC_UeCodeNum(); 
            prSPS->rVUI.u4ChromaSampleLocTypeBottomField = _VDEC_UeCodeNum(); 
        }
        prSPS->rVUI.fgTimingInfoPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgTimingInfoPresentFlag)
        {
            prSPS->rVUI.u4NumUnitsInTick = _VDEC_AVCGetRealShtBitStream(32); 
            prSPS->rVUI.u4TimeScale = _VDEC_AVCGetRealShtBitStream(32); 
            prSPS->rVUI.fgFixedFrameRateFlag = _VDEC_AVCGetBitStreamFlg();
        }
        prSPS->rVUI.fgNalHrdParametersPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgNalHrdParametersPresentFlag)
        {
            _vHrdParameters(&prSPS->rVUI.rNalHrdParameters);
        }
        prSPS->rVUI.fgVclHrdParametersPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgVclHrdParametersPresentFlag)
        {
            _vHrdParameters(&prSPS->rVUI.rVclHrdParameters);
        }
        if(prSPS->rVUI.fgNalHrdParametersPresentFlag || prSPS->rVUI.fgVclHrdParametersPresentFlag)
        {
            prSPS->rVUI.fgLowDelayHrdFlag = _VDEC_AVCGetBitStreamFlg();
        }
        prSPS->rVUI.fgPicStructPresentFlag = _VDEC_AVCGetBitStreamFlg();
        prSPS->rVUI.fgBitstreamRestrictionFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSPS->rVUI.fgBitstreamRestrictionFlag)
        {
            prSPS->rVUI.fgMotionVectorsOverPicBoundariesFlag = _VDEC_AVCGetBitStreamFlg();
            prSPS->rVUI.u4MaxBytesPerPicDenom = _VDEC_UeCodeNum();
            prSPS->rVUI.u4MaxBitsPerMbDenom = _VDEC_UeCodeNum();
            prSPS->rVUI.u4Log2MaxMvLengthHorizontal = _VDEC_UeCodeNum();
            prSPS->rVUI.u4Log2MaxMvLengthVertical = _VDEC_UeCodeNum();
            prSPS->rVUI.u4NumReorderFrames = _VDEC_UeCodeNum();
            prSPS->rVUI.u4MaxDecFrameBuffering = _VDEC_UeCodeNum();
        }
    }
    #endif
    _VDEC_TrailingBits();

    #ifdef FBM_DPB_CTRL
    if (FBM_ChkFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_RELEASE_FBG) || prDecPrm->fgChgRRMode)        
    {
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);    
        FBM_WaitUnlockFrameBuffer(prDecPrm->ucFbgId, WAIT_DISP_UNLOCK_TIME);    // block until display unlock
        _VDEC_FlushDPB(prDecPrm);
        FBM_ReleaseGroup(prDecPrm->ucFbgId);
        prDecPrm->ucFbgId = FBM_FBG_ID_UNKNOWN;
        prDecPrm->fgChgRRMode = FALSE;
    }
    #endif
    prSPS->fgSPSValid = TRUE;
    *pi4RetValue = 0;
}


// *********************************************************************
// Function    : void vPic_Par_Set_Rbsp(void)
// Description : Handle picture parameter set header
// Parameter   : None
// Return      : None
// *********************************************************************
void _VDEC_PPSrbsp(VDEC_H264_INFO_T* prVdec, INT32* pi4RetValue)
{
    UINT32 i;
    UINT32 u4PPSID;
    VDEC_AVC_SPS_T* prSPS;
    VDEC_AVC_PPS_T* prPPS;
    #ifdef FBM_DPB_CTRL
    VDEC_AVC_DecPrm* prDecPrm;
    #endif
    
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);

    #ifdef FBM_DPB_CTRL
    prDecPrm = &prVdec->rDecParam;
    #endif
    
    u4PPSID = _VDEC_UeCodeNum();
    if(u4PPSID < 256)
    {
        prPPS = &prVdec->arPPS[u4PPSID];
        x_memset((void*)prPPS, 0, sizeof(VDEC_AVC_PPS_T));
        prPPS->fgPPSValid = FALSE; // FALSE until set completely
    }
    else
    {
        *pi4RetValue = 1;
        LOG(2, "err in PPS Num %d, InWin 0x%x\n", u4PPSID, VLD_READ32(RO_VLD_BARL));
        return;
    }

    prPPS->u4SeqParameterSetId = _VDEC_UeCodeNum();
    CHKPARAM(prPPS->u4SeqParameterSetId, 31);
    prSPS = &prVdec->arSPS[prPPS->u4SeqParameterSetId];
    if(!prSPS->fgSPSValid)
    {    
        *pi4RetValue = 1;
        LOG(2, "SPS Num in PPS err\n");
        return;
    }

    prPPS->fgEntropyCodingModeFlag = _VDEC_AVCGetBitStreamFlg();
    prPPS->fgPicOrderPresentFlag = _VDEC_AVCGetBitStreamFlg();

    prPPS->u4NumSliceGroupsMinus1 = _VDEC_UeCodeNum();

    if(prPPS->u4NumSliceGroupsMinus1 > 255)
    {
        LOG(2, "Warning!!! num_slice_groups_minus1 size isn't enough\n");
    }

    if(prPPS->u4NumSliceGroupsMinus1 > 0)
    {
        prPPS->u4SliceGroupMapType = _VDEC_UeCodeNum();
        CHKPARAM(prPPS->u4SliceGroupMapType, 6);

        if(prPPS->u4SliceGroupMapType == 0)
        {
            for(i = 0; i <= prPPS->u4NumSliceGroupsMinus1; i++)
            {
                prPPS->u4RunLengthMinus1[i] = _VDEC_UeCodeNum();
            }
        }
        else if(prPPS->u4SliceGroupMapType == 2)
        {
            for(i = 0; i < prPPS->u4NumSliceGroupsMinus1; i++)
            {
                prPPS->u4TopLeft[i] = _VDEC_UeCodeNum();
                prPPS->u4BottomRight[i] = _VDEC_UeCodeNum();
            }
        }
        else if((prPPS->u4SliceGroupMapType == 3) ||
            (prPPS->u4SliceGroupMapType == 4) ||
            (prPPS->u4SliceGroupMapType == 5))
        {
            prPPS->fgSliceGroupChangeDirectionFlag = _VDEC_AVCGetBitStreamFlg();
            prPPS->u4SliceGroupChangeRateMinus1 = _VDEC_UeCodeNum();
        }
        else if(prPPS->u4SliceGroupMapType == 6)
        {
            prPPS->u4PicSizeInMapUnitsMinus1 = _VDEC_UeCodeNum();
            for(i = 0; i <= prPPS->u4PicSizeInMapUnitsMinus1; i++)
            {
                prPPS->au4SliceGroupId[i] = _VDEC_UeCodeNum();
            }
        }
    }

    prPPS->u4NumRefIdxL0ActiveMinus1 = _VDEC_UeCodeNum();
    CHKPARAM(prPPS->u4NumRefIdxL0ActiveMinus1, 31);
    prPPS->u4NumRefIdxL1ActiveMinus1 = _VDEC_UeCodeNum();
    CHKPARAM(prPPS->u4NumRefIdxL1ActiveMinus1, 31);
    
    prPPS->fgWeightedPredFlag = _VDEC_AVCGetBitStreamFlg();
    prPPS->u4WeightedBipredIdc = _VDEC_AVCGetRealShtBitStream(2);
    CHKPARAM(prPPS->u4WeightedBipredIdc, 2);
    prPPS->i4PicInitQpMinus26 = _VDEC_SeCodeNum();
    CHKPARAM_R(prPPS->i4PicInitQpMinus26, -(26 + (6*(INT32)prSPS->u4BitDepthLumaMinus8)), 25);
    prPPS->i4PicInitQsMinus26 = _VDEC_SeCodeNum();
    CHKPARAM_R(prPPS->i4PicInitQsMinus26, -26, 25);

    prPPS->i4ChromaQpIndexOffset = _VDEC_SeCodeNum();
    CHKPARAM_R(prPPS->i4ChromaQpIndexOffset, -12, 12);
    prPPS->fgDeblockingFilterControlPresentFlag = _VDEC_AVCGetBitStreamFlg();
    prPPS->fgConstrainedIntraPredFlag = _VDEC_AVCGetBitStreamFlg();
    prPPS->fgRedundantPicCntPresentFlag = _VDEC_AVCGetBitStreamFlg();

    if(_fgIsMoreRbspData())
    {
        prPPS->fgTransform8x8ModeFlag = _VDEC_AVCGetBitStreamFlg();
        prPPS->fgPicScalingMatrixPresentFlag = _VDEC_AVCGetBitStreamFlg();
        if(prPPS->fgPicScalingMatrixPresentFlag)
        {
            for(i=0; i<(((UINT32)prPPS->fgTransform8x8ModeFlag << 1) + 6); i++)
            {
                prPPS->fgPicScalingListPresentFlag[i] = _VDEC_AVCGetBitStreamFlg();
                if(prPPS->fgPicScalingListPresentFlag[i])
                {
                    if(i < 6)
                    {
                        _vScalingList(prPPS->ppcScalingList4x4[i], 16, &prPPS->fgUseDefaultScalingMatrix4x4Flag[i]);
                    }
                    else
                    {
                        _vScalingList(prPPS->ppcScalingList8x8[i-6], 64, &prPPS->fgUseDefaultScalingMatrix8x8Flag[i-6]);
                    }
                }
            }
        }
        prPPS->i4SecondChromaQpIndexOffset = _VDEC_SeCodeNum();
        CHKPARAM_R(prPPS->i4SecondChromaQpIndexOffset, -12, 12);
    }
    else
    {
        prPPS->fgTransform8x8ModeFlag = 0;
        prPPS->fgPicScalingMatrixPresentFlag = FALSE;
        prPPS->i4SecondChromaQpIndexOffset = prPPS->i4ChromaQpIndexOffset;
    }
  
    _VDEC_TrailingBits();
    prPPS->fgPPSValid = TRUE;
    prDecPrm->fgWaitPPS = FALSE;
    *pi4RetValue = 0;
}


/** Get decoding handler
*@param .
 */
 void _VDEC_SliceNonIDR(VDEC_H264_INFO_T* prVdec, INT32* pi4RetValue)
{
    VDEC_AVC_SLICE_HDR_T* prSliceHdr;// = &_tSliceHdr;
    VDEC_AVC_DecPrm* prDecPrm;
    UINT32 u4PPSID;
    #ifdef VDEC_TIME_PROFILE
    HAL_TIME_T rTimeHwS, rTimeHwE, rTimeHwDt;
    HAL_TIME_T rTimeSwS, rTimeSwDt,rTimeSwE;
    static UINT32 u4SWTimeMicro = 0;
    #endif
    
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);
    
    prDecPrm = &prVdec->rDecParam;
    prSliceHdr = &prVdec->rSliceHdr;  
    x_memset((void*)prSliceHdr, 0, sizeof(VDEC_AVC_SLICE_HDR_T));
    prDecPrm->prSliceHdr = prSliceHdr;
    //reset for multi-frame in one PES, only the first frame in PES has non-zero PTS value
    //change the handling to dmx, reset pts in queue after each picture deliver
    /*if((!prVdec->rDecParam.fgIsMMPlay) || (!prVdec->rDecParam.fgEnCalPTS))
    {
        prDecPrm->u4DispPts = prVdec->rPesInfo.u4PTS;
        //reset for multi-frame in one PES, only the first frame in PES has non-zero PTS value
        prVdec->rPesInfo.u4PTS = 0; 
    }
    else */if(prVdec->rDecParam.fgIsMMPlay && prDecPrm->prCurrFBufInfo)
    {
        prDecPrm->prCurrFBufInfo->u4SkipFrmCnt += prVdec->rPesInfo.u2SkipVidCount;
    }

    if(prDecPrm->fgNotSupport)  // check if !support but still get data
    {
        *pi4RetValue = (INT32)E_VDEC_NOT_SUPPORT;
        return;
    }    
    prDecPrm->fgEos = prVdec->rPesInfo.fgEos;
    prDecPrm->u8Offset = prVdec->rPesInfo.u8Offset;
    prDecPrm->u8OffsetI = prVdec->rPesInfo.u8OffsetI;    
    prDecPrm->u2DecodingOrder = prVdec->rPesInfo.u2DecodingOrder;
    prDecPrm->u4TickNum = prVdec->rPesInfo.u4TickNum;    

    prSliceHdr->u4FirstMbInSlice = _VDEC_UeCodeNum();
    prSliceHdr->u4SliceType = _VDEC_UeCodeNum();
    CHKPARAM(prSliceHdr->u4SliceType, 9);
    if(prSliceHdr->u4SliceType >= 5)
    {
        //prSliceHdr->u4SliceType -= 5;
    }

    u4PPSID = _VDEC_UeCodeNum();
    if((u4PPSID < 256)
        && (prVdec->arPPS[u4PPSID].fgPPSValid)
        && (prVdec->arSPS[prVdec->arPPS[u4PPSID].u4SeqParameterSetId].fgSPSValid))
    {
        prDecPrm->prPPS = &prVdec->arPPS[u4PPSID];
        prDecPrm->prSPS = &prVdec->arSPS[prVdec->arPPS[u4PPSID].u4SeqParameterSetId];
    }
    else
    {
        *pi4RetValue = (INT32)E_VDEC_FAIL;
        LOG(6, "err in Slice Hdr PPS Num %d err, inWin 0x%x\n", 
            u4PPSID, VLD_READ32(RO_VLD_BARL));
        return;
    }

    prDecPrm->fgFirstFrmTag = FALSE;
    if(!prDecPrm->fgIsFirstFrm) //first display
    {
        //wait RAP
        if(fgIsIDRPic(prVdec->u1NalUnitType)/* ||
            prDecPrm->rSEI.u1ExtMatchFlg*/)     //DTV00139283, can't use as RAP
        {
            if((!prVdec->rDecParam.fgIsMMPlay) || (!prDecPrm->fgWaitPPS))
            {
                LOG(2, "1st Frm Start Prs\n");
                prDecPrm->fgIsFirstFrm = TRUE;
                prDecPrm->fgFirstFrmTag = TRUE;
                #ifdef TIME_MEASUREMENT
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "REC_1st_I");
                #endif
            }
        }
        else 
        {
            if((!prVdec->rDecParam.fgIsMMPlay) 
                || (prDecPrm->rSEI.u1WaitRAPICnt > 0))  // start counting from I frame
            {
                prDecPrm->rSEI.u1WaitRAPCnt++;
            }
            if((prDecPrm->rSEI.u1WaitRAPCnt > WAIT_RAP_FRM_CNT) ||
                (prDecPrm->rSEI.u1WaitRAPRefCnt > (UINT8)(prDecPrm->prSPS->u4NumRefFrames << 1)) ||
                (prDecPrm->rSEI.u1WaitRAPICnt > (UINT8)prDecPrm->prSPS->u4NumRefFrames))
            {  
                LOG(2, "1st Frm Start Disp(WaitRAP Cnt %d, Ref I%d/Ref%d/Seq%d, Type %d)\n", 
                    prDecPrm->rSEI.u1WaitRAPCnt,
                    prDecPrm->rSEI.u1WaitRAPICnt, prDecPrm->rSEI.u1WaitRAPRefCnt,
                    prDecPrm->prSPS->u4NumRefFrames,
                    prSliceHdr->u4SliceType);
                prDecPrm->fgIsFirstFrm = TRUE;
                prDecPrm->fgFirstFrmTag = TRUE;
                prDecPrm->fgWaitPPS = FALSE;
                #ifdef TIME_MEASUREMENT
                TMS_DIFF_EX(TMS_FLAG_CHG_CHL, TMS_CHL_CHE_TIME, "REC_1st_I");
                #endif
            }
            else
            {
                if(fgIsRefPic(prDecPrm->u1NalRefIdc))
                {   //decode but not display
                    if(fgIsISlice(prSliceHdr->u4SliceType))
                    {
                        prDecPrm->rSEI.u1WaitRAPICnt++;
                    }
                    if(prDecPrm->rSEI.u1WaitRAPICnt > 0)    // start counting from I frame
                    {
                        prDecPrm->rSEI.u1WaitRAPRefCnt++;
                    }                    
                }
                /*else
                {
                    *pi4RetValue = (INT32)E_VDEC_FAIL;
                    return;
                }*/
            }
        }
    }
    
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeSwS);
    #endif
    
    _vInitSliceHdr(prDecPrm->prSliceHdr, prDecPrm->prPPS->u4NumRefIdxL0ActiveMinus1, 
        prDecPrm->prPPS->u4NumRefIdxL1ActiveMinus1);
    prSliceHdr->u4FrameNum = _VDEC_AVCGetRealShtBitStream(prDecPrm->prSPS->u4Log2MaxFrameNumMinus4 + 4);

    #ifdef VDEC_TIME_PROFILE
    //LOG(2, "Prs FrmNum %d, Type %d\n", prSliceHdr->u4FrameNum, prSliceHdr->u4SliceType);
    #endif

    prSliceHdr->fgFieldPicFlag = FALSE;
    prSliceHdr->fgBottomFieldFlag = FALSE;
    if(prDecPrm->prSPS->fgFrameMbsOnlyFlag)
    {
        prDecPrm->u1PicStruct = FRAME;
    }
    else
    {
        prSliceHdr->fgFieldPicFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSliceHdr->fgFieldPicFlag)
        {
            prSliceHdr->fgBottomFieldFlag = _VDEC_AVCGetBitStreamFlg();
            prDecPrm->u1PicStruct = (UINT8)((prSliceHdr->fgBottomFieldFlag) ? BOTTOM_FIELD : TOP_FIELD);            
        }
        else
        {
            prDecPrm->u1PicStruct = FRAME;           
        }
    }

    //error handling for MM I mode
    if(prDecPrm->fgIsMMPlay && (prDecPrm->ucSkipMode == IMode))
    {
        if(!fgIsISlice(prSliceHdr->u4SliceType))
        {
            if(prDecPrm->prCurrFBufInfo && 
                (prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME))
            {
                if(prDecPrm->u1PicStruct == FRAME)
                {
                    LOG(3, "SliceType %d Frm in IMode\n", prSliceHdr->u4SliceType);
                }
                else
                {
                    LOG(3, "SliceType %d Fld in IMode\n", prSliceHdr->u4SliceType);
                }                    
                *pi4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
                return;
            }
        }
    }
    
    if(fgIsIDRPic(prVdec->u1NalUnitType)) //IDR picture
    {
        prSliceHdr->u4IdrPicId = _VDEC_UeCodeNum();
        CHKPARAM(prSliceHdr->u4IdrPicId, 65535);
    }
    if(prDecPrm->prSPS->u4PicOrderCntType == 0)
    {
        prSliceHdr->i4PicOrderCntLsb = (INT32)_VDEC_AVCGetRealShtBitStream(prDecPrm->prSPS->u4Log2MaxPicOrderCntLsbMinus4 + 4);
        if(prDecPrm->prPPS->fgPicOrderPresentFlag && (!prSliceHdr->fgFieldPicFlag))
        {
            prSliceHdr->i4DeltaPicOrderCntBottom = _VDEC_SeCodeNum();
        }
        else
        {
            prSliceHdr->i4DeltaPicOrderCntBottom = 0;
        }
    }
  
    if((prDecPrm->prSPS->u4PicOrderCntType == 1) && (!prDecPrm->prSPS->fgDeltaPicOrderAlwaysZeroFlag))
    {
        prSliceHdr->ai4DeltaPicOrderCnt[0] = _VDEC_SeCodeNum();
        if(prDecPrm->prPPS->fgPicOrderPresentFlag && (!prSliceHdr->fgFieldPicFlag))
        {
            prSliceHdr->ai4DeltaPicOrderCnt[1] = _VDEC_SeCodeNum();
        }
    }
    else
    {
        if(prDecPrm->prSPS->u4PicOrderCntType == 1)
        {
            prSliceHdr->ai4DeltaPicOrderCnt[0] = 0;
            prSliceHdr->ai4DeltaPicOrderCnt[1] = 0;
        }
    }
  
    if(prDecPrm->prPPS->fgRedundantPicCntPresentFlag)
    {
        prSliceHdr->u4RedundantPicCnt = _VDEC_UeCodeNum();
    }
    if(fgIsBSlice(prSliceHdr->u4SliceType))
    {
        prSliceHdr->fgDirectSpatialMvPredFlag = _VDEC_AVCGetBitStreamFlg();
    }
    if(fgIsPSlice(prSliceHdr->u4SliceType) || fgIsBSlice(prSliceHdr->u4SliceType))
    {
        prSliceHdr->fgNumRefIdxActiveOverrideFlag = _VDEC_AVCGetBitStreamFlg();
        if(prSliceHdr->fgNumRefIdxActiveOverrideFlag)
        {
            prSliceHdr->u4NumRefIdxL0ActiveMinus1 = _VDEC_UeCodeNum();
            if(prSliceHdr->fgFieldPicFlag)
            {
                CHKPARAM(prSliceHdr->u4NumRefIdxL0ActiveMinus1, 31);
            }
            else
            {
                CHKPARAM(prSliceHdr->u4NumRefIdxL0ActiveMinus1, 15);
            }
            if(fgIsBSlice(prSliceHdr->u4SliceType))
            {
                prSliceHdr->u4NumRefIdxL1ActiveMinus1 = _VDEC_UeCodeNum();
                if(prSliceHdr->fgFieldPicFlag)
                {
                    CHKPARAM(prSliceHdr->u4NumRefIdxL1ActiveMinus1, 31);
                }
                else
                {
                    CHKPARAM(prSliceHdr->u4NumRefIdxL1ActiveMinus1, 15);
                }                
            }
        }
    }
    if(!fgIsBSlice(prSliceHdr->u4SliceType))
    {
        prSliceHdr->u4NumRefIdxL1ActiveMinus1 = 0;
    }
    
    _VDEC_SetPicInfo(prDecPrm);
    if(prDecPrm->fgAllocFbFail)
    {
        *pi4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
        prDecPrm->fgAllocFbFail = FALSE;
        #ifdef VDEC_TIME_PROFILE
        u4SWTimeMicro = 0;
        #endif
        return;
    }
    else if(prDecPrm->fgNotSupport)
    {
        *pi4RetValue = (INT32)E_VDEC_NOT_SUPPORT;
        return;
    }

    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeHwS);
    #endif
    
    if(!fgIsISlice(prSliceHdr->u4SliceType))
    {
        _VDEC_RefPicListReordering();//hw function
    }
      
    if((prDecPrm->prPPS->fgWeightedPredFlag && fgIsPSlice(prSliceHdr->u4SliceType))
        || ((prDecPrm->prPPS->u4WeightedBipredIdc == 1) && fgIsBSlice(prSliceHdr->u4SliceType)))
    {
        _VDEC_PredWeightTable();    //hw function
    }
    
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeHwE);
    HAL_GetDeltaTime(&rTimeHwDt, &rTimeHwS, &rTimeHwE);
    //LOG(2, "HwHdr %u.%06us\n", rTimeHwDt.u4Seconds, rTimeHwDt.u4Micros);
    #endif

    if(prDecPrm->u1NalRefIdc!= 0)
    {
        prSliceHdr->fgMmco5 = FALSE;
        _vRefPicMarking(fgIsIDRPic(prDecPrm->u1NalUnitType), prSliceHdr);    //save marking info into data structure
    }

    if(prDecPrm->prPPS->fgEntropyCodingModeFlag && (!fgIsISlice(prSliceHdr->u4SliceType)))
    {
        prSliceHdr->u4CabacInitIdc = _VDEC_UeCodeNum();
        CHKPARAM(prSliceHdr->u4CabacInitIdc, 2);
    }
    else
    {
        prSliceHdr->u4CabacInitIdc = 0;
    }

    prSliceHdr->i4SliceQpDelta = _VDEC_SeCodeNum();
    CHKPARAM_R(prSliceHdr->i4SliceQpDelta,
    -(((6*(INT32)prDecPrm->prSPS->u4BitDepthLumaMinus8) + 26) + prDecPrm->prPPS->i4PicInitQpMinus26), 
    (51 - (26 + prDecPrm->prPPS->i4PicInitQpMinus26)));

    if((prSliceHdr->u4SliceType == SI_Slice) || (prSliceHdr->u4SliceType == SP_Slice))
    {
        if(prSliceHdr->u4SliceType == SP_Slice)
        {
            prSliceHdr->fgSpForSwitchFlag = _VDEC_AVCGetBitStreamFlg();
        }
        prSliceHdr->i4SliceQsDelta = _VDEC_SeCodeNum();
    }

    if(prDecPrm->prPPS->fgDeblockingFilterControlPresentFlag)
    {
        prSliceHdr->u4DisableDeblockingFilterIdc = _VDEC_UeCodeNum();
        CHKPARAM(prSliceHdr->u4DisableDeblockingFilterIdc, 2);
        if(prSliceHdr->u4DisableDeblockingFilterIdc != 1)
        {
            prSliceHdr->i4SliceAlphaC0OffsetDiv2 = _VDEC_SeCodeNum();
            CHKPARAM_R(prSliceHdr->i4SliceAlphaC0OffsetDiv2, -6, 6);
            prSliceHdr->i4SliceBetaOffsetDiv2 = _VDEC_SeCodeNum();
            CHKPARAM_R(prSliceHdr->i4SliceBetaOffsetDiv2, -6, 6);
        }
        else
        {
            prSliceHdr->i4SliceAlphaC0OffsetDiv2 = 0;
            prSliceHdr->i4SliceBetaOffsetDiv2 = 0;
        }
    }

    if((prDecPrm->prPPS->u4NumSliceGroupsMinus1 > 0) && 
        (prDecPrm->prPPS->u4SliceGroupMapType >= 3) &&
        (prDecPrm->prPPS->u4SliceGroupMapType <= 5))
    {
        prSliceHdr->u4SliceGroupChangeCycle = _VDEC_AVCGetBitStreamFlg();
    }
    /*LOG(6, "FrmNum %d, sliceType %d, POCtp %d, poc %d, pts %d, qp %d, w %d, h %d, ", 
        prSliceHdr->u4FrameNum,
        prSliceHdr->u4SliceType,
        prDecPrm->prSPS->u4PicOrderCntType,
        prDecPrm->prCurrFBufInfo->i4POC,
        prVdec->rPesInfo.u4PTS,
        ((prDecPrm->prPPS->i4PicInitQpMinus26 & 0x3f) + 26) + (prSliceHdr->i4SliceQpDelta & 0x7f),
        prDecPrm->prCurrFBufInfo->u4W,
        prDecPrm->prCurrFBufInfo->u4H
        );*/
    #ifdef VDEC_TIME_PROFILE
    HAL_GetTime(&rTimeSwE);
    HAL_GetDeltaTime(&rTimeSwDt, &rTimeSwS, &rTimeSwE);
    u4SWTimeMicro += rTimeSwDt.u4Micros;
    if(prDecPrm->prCurrFBufInfo->u1FBufStatus == FRAME)
    {
        prDecPrm->u4CurSwMicro = u4SWTimeMicro - prDecPrm->u4CurWaitFbMicro;
        u4SWTimeMicro = 0;
    }
    #endif

    if(prDecPrm->fgIsResChg)
    {
        *pi4RetValue = (INT32)E_VDEC_OK_RES_CHG;
    }
    else
    {
        *pi4RetValue = (INT32)E_VDEC_OK;
    }
}


/** Get SEI information
*@param .
 */
 void _VDEC_SEIrbsp(VDEC_H264_INFO_T* prVdec, INT32* pi4RetValue)
{
    VDEC_AVC_DecPrm* prDecPrm;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    UINT32 u4PayloadType = 0, u4PayloadSize, u4Byte, u4DataStart = 0, u4DataLen = 0, u4Sz = 0;
    UINT32 u4AFDId;
	#ifdef CRCFLOWTEST
    UCHAR i=0;
	#endif
    ASSERT(prVdec!=NULL);
    ASSERT(pi4RetValue!=NULL);
    
    prDecPrm = &prVdec->rDecParam;

    //skip SEI data if in the half of field-encoded bitstream, save parsing time
    if(prDecPrm->prCurrFBufInfo)
    {
        if((prDecPrm->prCurrFBufInfo->u1FBufStatus == TOP_FIELD) ||
            (prDecPrm->prCurrFBufInfo->u1FBufStatus == BOTTOM_FIELD))
        {
            *pi4RetValue = (INT32)E_VDEC_OK;
            return;
        }
    }

    do
    {
        u4PayloadType = 0;
        while ((u4Byte = (_VDEC_AVCGetShtBitStream(8) >> 24)) == 0xFF)
        {
            u4PayloadType += 255;
        }
        u4PayloadType += u4Byte;   // this is the last byte

        u4PayloadSize = 0;
        while ((u4Byte = (_VDEC_AVCGetShtBitStream(8) >> 24)) == 0xFF)
        {
            u4PayloadSize += 255;
        }
        u4PayloadSize += u4Byte;   // this is the last byte

        if(u4PayloadSize == 0)
        {
            //LOG(2, "SEI PayloadSz=0, rptr0x%x\n", _VDEC_VldRPtr());
            *pi4RetValue = (INT32)E_VDEC_OK;
            return;
        }
        //LOG(2, "Type %d, Sz %d, 0x%x\n", u4PayloadType, u4PayloadSize, VLD_READ32(0xF0));
        switch ( u4PayloadType )
        {
            //case SEI_BUFFERING_PERIOD:
            //    _vInterpretSEI_BufferingPeriodInfo(prVdec);
            //    break;
            case  SEI_PIC_TIMING:
                if(!_vInterpretSEI_PicTiming(prDecPrm))
                {
                    while(u4PayloadSize > 0)
                    {
                        u4Byte = _VDEC_AVCGetBitStream(8);
                        u4PayloadSize --;
                    }
                }
                break;
            case SEI_USER_DATA_REGISTERED_ITU_T_T35:
                u4DataStart = _VDEC_VldRPtr();
                u4DataLen = u4PayloadSize;                
                if(u4PayloadSize > 8)
                {
                    UNUSED(_VDEC_AVCGetRealShtBitStream(8));    //coountry_code
                    UNUSED(_VDEC_AVCGetRealShtBitStream(16));   //provider_code
                    u4AFDId = _VDEC_AVCGetRealShtBitStream(32);   //afd_identifier
                    u4PayloadSize -= 7;
                    if(u4AFDId == AFD_IDENTIFIER_DTG1)
                    {
                        UNUSED(_VDEC_AVCGetRealShtBitStream(1));    //zero_bit
                        u4Byte = _VDEC_AVCGetRealShtBitStream(1);   //active_format_flag
                        UNUSED(_VDEC_AVCGetRealShtBitStream(6));   //alignment_bits
                        u4PayloadSize --;
                        if(u4Byte == 1)
                        {
                            UNUSED(_VDEC_AVCGetRealShtBitStream(4));//reserved
                            u4Byte = _VDEC_AVCGetRealShtBitStream(4);//active_format
                            u4PayloadSize--;
                            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prDecPrm->ucFbgId);
                            if(prFbmSeqHdr != NULL)
                            {
                                if(prFbmSeqHdr->ucActFmt != (UCHAR)u4Byte)
                                {
                                    LOG(2, "AFD Chg %d, AspRatio %d\n", u4Byte, prFbmSeqHdr->ucAspRatInf);
                                    prFbmSeqHdr->ucActFmt = (UCHAR)u4Byte;
                                    FBM_SetFrameBufferFlag(prDecPrm->ucFbgId, FBM_FLAG_SEQ_CHG);
                                }
                            }
                        }
                    }
                }
                while(u4PayloadSize > 0)
                {
                    u4Byte = _VDEC_AVCGetBitStream(8);
                    u4PayloadSize --;
                }
                #ifdef CC_SEI_USER_DATA_ENABLE
                //save user data to data structure
                if((prDecPrm->rSEI.u4UserDataWptr + u4DataLen) >= MAX_USERDATA_SIZE)
                {
                    LOG(1, "UsrData in SEI len %d > max %d\n", prDecPrm->rSEI.u4UserDataWptr, MAX_USERDATA_SIZE);
                    prDecPrm->rSEI.u4UserDataWptr = 0;
                }
                else
                {
                    u4Sz = prVdec->rPesInfo.u4FifoEnd - u4DataStart;
                    if(u4DataLen <= u4Sz)  // input ring buffer
                    {
                        x_memcpy((void*)&prDecPrm->rSEI.au1UsrData[prDecPrm->rSEI.u4UserDataWptr], 
                        (void*)VIRTUAL(u4DataStart), u4DataLen);
                    }
                    else
                    {
                        x_memcpy((void*)&prDecPrm->rSEI.au1UsrData[prDecPrm->rSEI.u4UserDataWptr], 
                            (void*)VIRTUAL(u4DataStart), u4Sz);
                        x_memcpy((void*)&prDecPrm->rSEI.au1UsrData[prDecPrm->rSEI.u4UserDataWptr + u4Sz], 
                            (void*)VIRTUAL(prVdec->rPesInfo.u4FifoStart), u4DataLen - u4Sz);
                    }
                    prDecPrm->rSEI.u4UserDataWptr += u4DataLen;
                }
                #else
                UNUSED(u4Sz);
                UNUSED(u4DataLen);
                UNUSED(u4DataStart);
                #endif
                break;
            case SEI_RECOVERY_POINT:
                prDecPrm->rSEI.u4RecoveryFrmCnt = _VDEC_UeCodeNum();
                prDecPrm->rSEI.u1ExtMatchFlg = (UINT8)_VDEC_AVCGetRealShtBitStream(1);
                UNUSED(_VDEC_AVCGetRealShtBitStream(3));
                break;
			#ifdef CRCFLOWTEST
			case SEI_USER_DATA_UNREGISTERED:
				for(i=0;i<4;i++)
				{
					prVdecEsInfoKeep->u4Ycrc[i] = _VDEC_AVCGetRealShtBitStream(32);
				}

				for(i=0;i<4;i++)
				{
					prVdecEsInfoKeep->u4Ccrc[i] = _VDEC_AVCGetRealShtBitStream(32);
				}
				break;
			#endif
            default:
                if(u4PayloadType >= (UINT32)SEI_MAX_ELEMENTS)
                {
                    //for error bitstream, but dmx keep updating wptr, 
                    //vdec may spend too much time in this function                
                    x_thread_delay(1);
                    *pi4RetValue = (INT32)E_VDEC_PARSE_NOT_READY;
                    return;
                }
                //reserved_sei_message
                while(u4PayloadSize > 0)
                {
                    u4Byte = _VDEC_AVCGetBitStream(8);
                    u4PayloadSize --;
                }
                break;
        }
        _VDEC_TrailingBits();
    } while(_fgIsMoreRbspData());    // more_rbsp_data()  msg[offset] != 0x80
    
    *pi4RetValue = (INT32)E_VDEC_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



