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
 * $RCSfile: vdec_wmvpsr.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvpsr.c
 *  This header file declares exported APIs of wmv.
 */

#include "x_lint.h"
#include "vdec_wmvutil.h"
#include "vdec_wmvhw.h"
#include "vdec_wmvpsrd.h"
#include "vdec_wmvpsr.h"
//#include "config.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_common.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END

//extern TPicInf prVdec->rOther.rPicInfo;
//extern int prVdec->rOther.u4repeatframe;

//#define TEST
extern UCHAR _VDecReceiveEs(UCHAR);

#ifdef TEST
UINT32 _dwLookAhead;
#endif

//copy from main.c
void reset_pic_hdr_bits(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.i4PicHdrBits = 0;
    prVdec->rOther.fgCounting = TRUE;
}

void before_bp(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.i4PicHdrBits += 5;
    prVdec->rOther.fgCounting = FALSE;
}

void after_bp(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.fgCounting = TRUE;
}

UINT32 pic_hdr_bitcount(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    return prVdec->rOther.i4PicHdrBits;
}

void set_pic_hdr_bits(UCHAR ucEsId, UINT32 n)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.i4PicHdrBits = n;
}

//copy from vparser.c
void vInitVParser(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = 0;
    prVdec->rOther.u4BitCount = 0;

    //_fgShortVideoHeader = FALSE;
    //_bVisualObjectVerid = 1;
    //_fgQuarterSample = FALSE;
    //_fgReversibleVlc = FALSE;
    //_fgReducedResolutionVopEnable = FALSE;
    //_tDirMode.dwTFrm = 0xffffffff;
    //_fgTopFldFirst = FALSE;
}
/*
 BOOL fgVldInitBarrelShifter(UINT32 dwPtr)
 {
 UINT32 u4Temp, u4RestData, u4RealAddr, u4Bits;

 WriteREG(RW_VLD_RPTR, dwPtr);
 WriteREG(RW_VLD_PROC, 0x00100000);

 if(!fgWaitVldFetchOk(ucEsId))
 {
 LOG(0,"Fail to WMV waitVLDFetchOK\n");
 return FALSE;
 }
 WriteREG(RW_VLD_PROC, 0x00800000);

 u4RealAddr = dwVldRPtr(ucEsId,&u4Bits);
 if (dwPtr > u4RealAddr)
 {
 UINT32 i;

 u4RestData = dwPtr -u4RealAddr;
 for (i = 0; i < u4RestData; i++)
 {
 dwGetBitStream(ucEsId,8);
 }
 }
 return TRUE;

 }
 */

#if 0  // megaa 20100506: debug code for checking improper lock sequence
extern VDEC_INFO_T _rVdecInfo;
void vCheckLockProper(UCHAR ucEsId)
{
    if (_rVdecInfo.arVdecVldInfo[0].ucLockEsId != ucEsId)
        printf("!!! improper lock for wmv !!!\n");
}
#endif


#if defined(IS_WMV_POST_MT5368)
static VOID _vWaitSramStable(UCHAR ucEsId)
{
#ifdef WAIT_SRAM_STABLE
    if (((ReadREG(ucEsId,RO_VLD_SRAMCTRL) >> 15) & AA_FIT_TARGET_D) == 1)
    {
        UINT32 u4Tmp = 0;
        while ((ReadREG(ucEsId,RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Tmp++;
            if (u4Tmp >= 0x10000)
            {
                //ASSERT(0);
                LOG(1, "Wait SRAM stable failed!!!\n");
                return;
            }
        }
    }
#else
    UNUSED(ucEsId);
#endif
}
#endif


BOOL fgVldInitBarrelShifter(UCHAR ucEsId, UINT32 dwPtr)
{
    BOOL fgFetchOK = FALSE;
    UINT32 i, j;
    UINT32 dwVLDByte, dwVLDBit;
    UINT32 dwVFifo;
    UINT32 dwVLDRemainByte;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    LOG(7, "fgVldInitBarrelShifter, position : %08x\n", dwPtr);

#if 0  // megaa 20100506
    vCheckLockProper(ucEsId);
#endif

#if 0  // megaa 20100321
    //vInitVParser(ucEsId);
    prVdec->rOther.u4BitCount = 0;
#endif

    dwVFifo = ((UINT32)prVdec->rDram.pu1VFifo);
    dwVLDRemainByte = dwPtr & 0xf;

    for (i =0 ; i<5 ; i++) {
        _VDEC_UpdateVldWMVWPtr(ucEsId,prVdec->rOther.u4WPtr);

#if defined(IS_WMV_POST_MT5368)
        _vWaitSramStable(ucEsId);
#endif

        WriteREG(RW_VLD_RPTR, dwPtr);  // !!!1006 do we have to write twice as MPEG4?

#if defined(IS_WMV_POST_MT5368)
        WriteREG(WO_VLD_SRST, 0x100);
        WriteREG(WO_VLD_SRST, 0);
#endif
        
        if (ReadREG(ucEsId,RO_VLD_VRPTR) != (dwPtr - (dwPtr % 16)))
        {
            LOG(1, "RPTR moved before INIFET!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            //ASSERT(0);
            //return FALSE;
        }
        WriteREG(RW_VLD_PROC, INIFET);

        if (fgWaitVldFetchOk(ucEsId))
        {
            fgFetchOK = TRUE;
            break;
        }
    }
    if (!fgFetchOK)
    {
        ASSERT(0);
        return FALSE;
    }
    WriteREG(RW_VLD_PROC, INIBR);

    dwVLDByte = dwVldRPtr(ucEsId,&dwVLDBit);

    for (j=0; j<dwVLDRemainByte; j++)
    {
        //mark by spork
        dwGetBitStream(ucEsId,8);
        dwVLDByte = dwVldRPtr(ucEsId,&dwVLDBit);
        if (dwVLDByte == (dwPtr-dwVFifo))
            break;
    }
    
#if 1  // megaa 20100321
    //vInitVParser(ucEsId);
    prVdec->rOther.u4BitCount = 0;
#endif
    prVdec->rOther.u4LastStartPos = dwPtr;  // megaa 20100603: record the last inited pointer

    return (TRUE);
}

BOOL fgWaitVldFetchOk(UCHAR ucEsId)
{
    UINT32 u4Tmp = 0;

    while ((ReadREG(ucEsId,RO_VLD_FETCHOK) & 0x1) == 0)
    {
        u4Tmp++;
        if (u4Tmp > 0x10000)
        {
            LOG(1, "Wait fetch failed!!!\n");
            return (FALSE);
        }
    }
#ifdef WAIT_SRAM_STABLE
    u4Tmp = 0;
#if defined(IS_WMV_POST_MT5368)
    if (((ReadREG(ucEsId,RO_VLD_SRAMCTRL) >> 15) & AA_FIT_TARGET_D) == 1)
#endif
    {
        while ((ReadREG(ucEsId,RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Tmp++;
            if (u4Tmp >= 0x10000)
            {
                LOG(0, "Wait SRAM stable failed!!!\n");
                ASSERT(0);
            }
        }
    }
#endif

    return (TRUE);
}

void vPowerOffVDec(UCHAR ucEsId)
{
    while ((ReadREG(ucEsId,RO_VLD_FETCHOK) & VLD_BAR_FETCH_FIN) == 0)
        ;
    WriteREG(RW_VLD_PWRSAVE, 1);
}

UINT32 dwGetBitStream(UCHAR ucEsId, UINT8 bShiftBit)
{
    UINT32 dwRegVal;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4BitCount += (UINT32)bShiftBit;
    dwRegVal = ReadREG(ucEsId,(UINT32)(RO_VLD_BARL + (bShiftBit << 2)));
    return (dwRegVal);
}

void vVldByteAlign(UCHAR ucEsId)
{
    UINT8 u1BitCount;
    //VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    u1BitCount = (UINT8)(ReadREG(ucEsId,RW_VLD_BITCOUNT) & 0x7);
    if (u1BitCount != 0)
    {
        dwGetBitStream(ucEsId,(UINT8)(8-u1BitCount));
    }
    //prVdec->rOther.u4BitCount = 0;  megaa 20100603: no need
}

/********************/
/* member functions */
/********************/

void vInitVParserWMV(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4DispBufIdx = 1;
    if (prVdec->rOther.u4DispBufIdx == 1)
    {
        vSetFRefBuf(ucEsId,0);
        vSetBRefBuf(ucEsId,1);
    }
    else
    {
        vSetFRefBuf(ucEsId,1);
        vSetBRefBuf(ucEsId,0);
    }
    prVdec->rOther.u4DecPicNo = 0;
    prVdec->rOther.u4CurrPicNo = 0;

    //  prVdec->rSeqLayer.fgVC1 = FALSE; //move to vCodecVersion(), incluing initialize prVdec->rOther.i4CodecVersion

    //  vCodecVersion();

    //Sequence Header variables initialization
    //Advanced
    //local _iLevel = 0;
    //local _iChromaFormat = 1;
    prVdec->rSeqLayer.fgBroadcastFlags = FALSE;
    prVdec->rSeqLayer.fgInterlacedSource = FALSE;
    prVdec->rSeqLayer.fgTemporalFrmCntr = FALSE;

    prVdec->rSeqLayer.fgSeqFrameInterpolation = FALSE;
    //for display use _fgInterpolateCurrentFrame = FALSE;

    prVdec->rSeqLayer.fgHRD_PARAM_FLAG = FALSE;
    prVdec->rSeqLayer.i4hrd_num_leaky_buckets = 0;
    prVdec->rPicLayer.fgTopFieldFirst = TRUE; //ming
    //Simple & Main
    //local _fgRndCtrlOn = FALSE;
    prVdec->rSMProfile.fgXintra8Switch = FALSE;
    prVdec->rSMProfile.fgMultiresEnabled = FALSE;
    prVdec->rSMProfile.i4ResIndex = 0;
    //local _fg16bitXform = FALSE;
    prVdec->rSMProfile.fgDCTTable_MB_ENABLED = FALSE;
    //local _fgStartCode = FALSE;
    prVdec->rSMProfile.fgPreProcRange = FALSE;
    prVdec->rSMProfile.i4NumBFrames = 1;
    //local _fgBetaContent = TRUE;
    //local _fgRTMContent = FALSE;
    //no use _iBetaRTMMismatchIndex = 0;
    prVdec->rSMProfile.fgRotatedIdct = FALSE;
    prVdec->rPicLayer.u1FrameCodingMode = PROGRESSIVE; //ming
    // WMV7 & WMV8
    prVdec->rSMProfile.fgMixedPel = FALSE;
    prVdec->rSMProfile.fgFrmHybridMVOn = FALSE;
    prVdec->rSMProfile.fgXintra8 = FALSE;

    //End of Sequence

    prVdec->rPicLayer.fgPostRC1 = TRUE;

    //EntryPoint Header variables initialization
    prVdec->rEntryPoint.fgBrokenLink = FALSE;
    prVdec->rEntryPoint.fgClosedEntryPoint = FALSE;
    prVdec->rEntryPoint.fgPanScanPresent = FALSE;
    prVdec->rEntryPoint.fgRefDistPresent = FALSE;
    prVdec->rEntryPoint.fgLoopFilter = FALSE;
    prVdec->rEntryPoint.fgUVHpelBilinear = FALSE;
    prVdec->rEntryPoint.i4RangeState = 0;
    prVdec->rEntryPoint.i4ReconRangeState = 0;
    prVdec->rEntryPoint.fgExtendedMvMode = FALSE;
    prVdec->rEntryPoint.i4MVRangeIndex = 0;
    prVdec->rEntryPoint.i4DQuantCodingOn = 0;
    prVdec->rEntryPoint.fgXformSwitch = FALSE;
    prVdec->rEntryPoint.fgSequenceOverlap = FALSE;
    //Quant related
    prVdec->rEntryPoint.fgExplicitSeqQuantizer = FALSE;
    prVdec->rEntryPoint.fgExplicitFrameQuantizer = FALSE;
    prVdec->rEntryPoint.fgExplicitQuantizer = FALSE;
    prVdec->rPicLayer.fgUse3QPDZQuantizer = FALSE;
    prVdec->rPicLayer.fgHalfStep = FALSE;

    prVdec->rEntryPoint.fgExtendedDeltaMvMode = FALSE;
    prVdec->rEntryPoint.i4DeltaMVRangeIndex = 0;
    prVdec->rEntryPoint.i4ExtendedDMVX = 0;
    prVdec->rEntryPoint.i4ExtendedDMVY = 0;
    prVdec->rEntryPoint.i4RefFrameDistance = 0;

    prVdec->rPicLayer.i4BNumerator = 0;
    prVdec->rSMProfile.i4NumBFrames = 1;
    prVdec->rPicLayer.i4DCStepSize = 0;
    prVdec->rOther.i4X9MVMode = 0;

    prVdec->rOther.fgMBXformSwitching = FALSE;
    prVdec->rOther.i4FrameXformMode = 0;

    prVdec->rEntryPoint.fgRangeRedY_Flag = FALSE;
    prVdec->rEntryPoint.fgRangeRedUV_Flag = FALSE;
    //End of EntryPoint


    prVdec->rDram.u4ForwardRefPicType = 0;
    prVdec->rDram.u4BackwardRefPicType = 0;

    // Picture Header
    //WMV7 & WMV8
    prVdec->rOther.fgDCPred_IMBInPFrame = FALSE;

    //for field pictures

    prVdec->rPicLayer.fgTopFieldFirst = TRUE;
    prVdec->rPicLayer.fgRepeatFirstField = FALSE;
    //FALSE for PROGRESSIVE.
    prVdec->rPicLayer.fgInterlaceV2 = FALSE;
    prVdec->rPicLayer.fgFieldMode = FALSE;
    prVdec->rPicLayer.i4CurrentField = 0; // 0:TOP, 1:BOTTOM field
    prVdec->rPicLayer.i4CurrentTemporalField = 0; // 0:1st field or frame picture, 1: 2nd field

    prVdec->rPicLayer.i4MaxZone1ScaledFarMVX = 0;
    prVdec->rPicLayer.i4MaxZone1ScaledFarMVY = 0;
    prVdec->rPicLayer.i4Zone1OffsetScaledFarMVX = 0;
    prVdec->rPicLayer.i4Zone1OffsetScaledFarMVY = 0;
    prVdec->rPicLayer.i4FarFieldScale1 = 0;
    prVdec->rPicLayer.i4FarFieldScale2 = 0;
    prVdec->rPicLayer.i4NearFieldScale = 0;
    prVdec->rPicLayer.i4MaxZone1ScaledFarBackMVX = 0;
    prVdec->rPicLayer.i4MaxZone1ScaledFarBackMVY = 0;
    prVdec->rPicLayer.i4Zone1OffsetScaledFarBackMVX = 0;
    prVdec->rPicLayer.i4Zone1OffsetScaledFarBackMVY = 0;
    prVdec->rPicLayer.i4FarFieldScaleBack1 = 0;
    prVdec->rPicLayer.i4FarFieldScaleBack2 = 0;
    prVdec->rPicLayer.i4NearFieldScaleBack = 0;

    prVdec->rPicLayer.fgTwoRefPictures = TRUE;
    prVdec->rPicLayer.fgUseOppFieldForRef = TRUE;
    prVdec->rPicLayer.fgUseSameFieldForRef = TRUE;
    //Robert TODO: 0511
    prVdec->rPicLayer.fgBackRefUsedHalfPel = FALSE;
    prVdec->rPicLayer.fgBackRefTopFieldHalfPelMode = FALSE;
    prVdec->rPicLayer.fgBackRefBottomFieldHalfPelMode = FALSE;

    prVdec->rPicLayer.fgMvResolution = FALSE;

    prVdec->rPicLayer.i4Overlap = 0;
    prVdec->rOther.i4MvTable = 0;
    prVdec->rOther.i4CBPTable = 0;
    prVdec->rOther.i4MBModeTable = 0;
    prVdec->rOther.i42MVBPTable = 0;
    prVdec->rOther.i44MVBPTable = 0;

    //!WMVA profile
    prVdec->rSMProfile.fgPreProcRange = FALSE;

    prVdec->rEntryPoint.fgNewDCQuant = FALSE;

    prVdec->rPicLayer.fgDCTTable_MB = FALSE;

    // WMV7 & WMV8
    //Robert TODO:
    if (prVdec->rOther.i4CodecVersion == WMV2)
    {
        prVdec->rSMProfile.fgSKIPBIT_CODING = TRUE;
        prVdec->rSMProfile.fgNEW_PCBPCY_TABLE = TRUE;
    }
    else
    {
        prVdec->rSMProfile.fgSKIPBIT_CODING = FALSE;
        prVdec->rSMProfile.fgNEW_PCBPCY_TABLE = FALSE;
    }

    prVdec->rSMProfile.fgCODFlagOn = TRUE;

    if (prVdec->rOther.i4CodecVersion >= WMV3)
    {
        prVdec->rEntryPoint.fgNewDCQuant = TRUE;
    }

    ComputeDQuantDecParam(ucEsId);

    prVdec->rOther.u1DiffQtProfile = 0;

    //prVdec->rOther.i4SeqHdrData1 = 0;
    //prVdec->rOther.i4SeqHdrData2 = 0;
    prVdec->rSMProfile.i4skip_bit_mode_v87 = 0;
    prVdec->rSMProfile.i4wmv8_bp_mode = 0;

    //NEEDS to initialize
    prVdec->rOther.u4new_entry_point = 0;
    prVdec->rOther.i4pred_use_wdle = 0;
    prVdec->rSeqLayer.fgPostProcInfoPresent = FALSE;
    prVdec->rSMProfile.fgYUV411 = FALSE;
    prVdec->rSMProfile.fgSpriteMode = FALSE;
    prVdec->rEntryPoint.i4RangeRedY = 0;
    prVdec->rEntryPoint.i4Range_MapUV = 0;
    prVdec->rPicLayer.u1RepeatFrameCount = 0;
    prVdec->rOther.u1DQuantBiLevelStepSize = 0;
    prVdec->rOther.fgDQuantOn = FALSE;
    prVdec->rOther.i4Panning = 0;
    prVdec->rOther.fgDQuantBiLevel = FALSE;

}

void vResetConditionalVariablesForSequenceSwitch(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rEntryPoint.fgExtendedDeltaMvMode = FALSE;
    prVdec->rEntryPoint.fgExplicitSeqQuantizer
    = prVdec->rEntryPoint.fgExplicitFrameQuantizer
      = prVdec->rPicLayer.fgUse3QPDZQuantizer = FALSE;
    //prVdec->rPicLayer.fgTopFieldFirst = TRUE;
    prVdec->rPicLayer.fgRepeatFirstField = FALSE;
    prVdec->rPicLayer.u1RepeatFrameCount = 0;
    //local _fgUVProgressiveSubsampling = TRUE;
    //for display use _fgInterpolateCurrentFrame = FALSE;
    //local _iPpMethod = -1;
    prVdec->rEntryPoint.i4MVRangeIndex = 0;
    prVdec->rEntryPoint.i4DeltaMVRangeIndex = 0;
    prVdec->rEntryPoint.i4ExtendedDMVX
    = prVdec->rEntryPoint.i4ExtendedDMVY = 0;
    //_fgCBEntropyCoding = FALSE;
    prVdec->rOther.fgLuminanceWarp = FALSE;
    prVdec->rEntryPoint.i4RefFrameDistance = 0;
    prVdec->rOther.i4FrameXformMode = XFORMMODE_8x8;
    prVdec->rOther.fgMBXformSwitching = FALSE;
}

//extern UINT32 prVdec->rOther.u4BitCount;

UINT32 dwVParserWMVA(UCHAR ucEsId)
{
    UINT32 u4StartCode;
    UINT32 u4bits;
    UINT32 u4Addr;
    UCHAR *pucBuf;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    UNUSED(u4bits);
    //BOOL fgFirstTime = TRUE;

    if (prVdecEsInfo == NULL)
    {
        ASSERT(0);
        return 0;
    }
    prVdec->rOther.i4SetPos = (prVdec->rPesInfo.u4VldReadPtr - prVdec->rPesInfo.u4FifoStart);

#if 0
    x_memcpy(&u4StartCode, (VOID*)VIRTUAL((UINT32)(prVdec->rDram.pu1VFifo
                                          + prVdec->rOther.i4SetPos)), 4);
    u4StartCode = ((u4StartCode&0x000000FF)<<24)
                  | ((u4StartCode&0x0000FF00)<<8) | ((u4StartCode&0x00FF0000)>>8)
                  | ((u4StartCode&0xFF000000)>>24);
    fgVldInitBarrelShifter(ucEsId,(UINT32)prVdec->rDram.pu1VFifo
                           + prVdec->rOther.i4SetPos+4);
#else
    u4Addr = prVdec->rPesInfo.u4VldReadPtr + 3;
    if (u4Addr >= prVdec->rPesInfo.u4FifoEnd)
    {
        u4Addr -= (prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoStart);
    }
    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
    u4StartCode = pucBuf[0];
    u4StartCode |= 0x100;
    fgVldInitBarrelShifter(ucEsId,u4Addr+1);
#endif
    //ASSERT(prVdec->rPesInfo.u4VldReadPtr+4 <= (prVdec->rDram.pu1FifoEnd));

    LOG(6,"Rptr 0x%x, input win 0x%x\n", dwVldRPtr(ucEsId,&u4bits), ReadREG(ucEsId,RO_VLD_BARL));
    LOG(6,"start code : 0x%08x, offset : %x\n",u4StartCode,prVdec->rOther.i4SetPos);

    switch (u4StartCode)
    {
    case WMV_SC_FRAME:
        prVdec->rPicLayer.i4CurrentTemporalField = 0;
        break;
    case WMV_SC_FIELD:
        break;
    case WMV_SC_SEQ:
        //LOG(0,"parsing sequence header\n");
        decodeSequenceHead_Advanced(ucEsId);
        prVdec->rOther.fgChangeResolution = FALSE;
        //create FBM group
#if 1
        if (prVdec->rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
        {
            if (((prVdec->rSeqHdrR.u4PicHeightSrc == prVdec->rSeqHdrR.u4PrevPicHeightSrc)
                    && (prVdec->rSeqHdrR.u4PicWidthSrc == prVdec->rSeqHdrR.u4PrevPicWidthSrc))
                    || (prVdec->rSeqHdrR.u4PrevPicWidthSrc ==0 && prVdec->rSeqHdrR.u4PrevPicHeightSrc ==0))
            {
                prVdec->rOther.fgChangeResolution = FALSE;
                break;
            }
            else
            {
                prVdec->rOther.fgChangeResolution = TRUE;
                if (prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_NETFLIX && !prVdecEsInfo->fgSeamlessPlay)
                {
                    FBM_SetFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_RELEASE_FBG);
                }
                else
                {
                    break;
                }
                //FBM_SetFrameBufferFlag(prVdec->rOther.u1FbgId, FBM_FLAG_SEQ_CHG);
                //break;
            }
        }
        vInitVdoMem(ucEsId);
        if (prVdec->rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)  // FBG create failed in vInitVdoMem
        {
            return E_VDEC_NOT_SUPPORT;
        }
#else
        if (prVdec->rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            vInitVdoMem(ucEsId);
        }
#endif
        break;
    case WMV_SC_ENTRY:
        //LOG(0,"paring entry point header\n");
        vResetConditionalVariablesForSequenceSwitch(ucEsId);
        DecodeEntryPointHeader(ucEsId);
        break;
    case WMV_SC_ENDOFSEQ:
        //prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,32); // flush the Seq End Code.
        return END_OF_FILE;
    default:
        dwGetHi(ucEsId,8); //flush this Start Code
        break;
    }
    if ((u4StartCode != WMV_SC_FRAME) && (u4StartCode != WMV_SC_FIELD)
            && (u4StartCode != WMV_SC_ENDOFSEQ))
    {
        return E_VDEC_PARSE_NOT_READY;
    }
    else
    {
        ;
    }
    if (prVdec->rOther.i4CodecVersion != WMVA) // simple or main profile
    {
        // WMVideoDecDecodeFrameHead(ucEsId);
    }
    else //advanced profile
    {
        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0); //flush Start Code.
        reset_pic_hdr_bits(ucEsId);

        if (prVdec->rPicLayer.i4CurrentTemporalField == 1)
        {
            set_pic_hdr_bits(ucEsId,prVdec->rPicLayer.i4SlicePicHeaderNumField);
            goto INTERLACE_FIELD;
        }

        if (prVdec->rSeqLayer.fgInterlacedSource)
        {
            if (0 == dwGetHi(ucEsId,1))
            {
                prVdec->rPicLayer.u1FrameCodingMode = PROGRESSIVE;
            }
            else
            {
                if (0 == dwGetHi(ucEsId,1))
                {
                    prVdec->rPicLayer.u1FrameCodingMode = INTERLACEFRAME;
                }
                else
                {
                    prVdec->rPicLayer.u1FrameCodingMode = INTERLACEFIELD;
                }
            }
        }
        else
        {
            prVdec->rPicLayer.u1FrameCodingMode = PROGRESSIVE;
        }

        if (prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE)
        {
            prVdec->rSeqHdrR.u4PicWidthDec
            = prVdec->rSeqHdrR.rMultiResParams[PROGRESSIVE].iWidthDec;
            prVdec->rSeqHdrR.u4PicHeightDec
            = prVdec->rSeqHdrR.rMultiResParams[PROGRESSIVE].iHeightDec;
            prVdec->rSeqHdrR.u4NumMBX = prVdec->rSeqHdrR.u4PicWidthDec
                                            >> 4;
            prVdec->rSeqHdrR.u4NumMBY = prVdec->rSeqHdrR.u4PicHeightDec
                                            >> 4;
            prVdec->rSeqHdrR.u4PicWidthCmp
            = prVdec->rSeqHdrR.rMultiResParams[PROGRESSIVE].iFrmWidthSrc;
            prVdec->rSeqHdrR.u4PicHeightCmp
            = prVdec->rSeqHdrR.rMultiResParams[PROGRESSIVE].iFrmHeightSrc;

            prVdec->rPicLayer.fgInterlaceV2 = FALSE;
            prVdec->rPicLayer.fgFieldMode = FALSE;
            prVdec->rPicLayer.i4CurrentField = 0; // 0:TOP, 1:BOTTOM field
            decodeVOPHeadProgressiveWMVA(ucEsId); // advanced progressive
        }
        else if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFRAME)
        {
            //Printf("!!!!!interlaceframe\n");
            prVdec->rSeqHdrR.u4PicWidthDec
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFRAME].iWidthDec;
            prVdec->rSeqHdrR.u4PicHeightDec
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFRAME].iHeightDec;
            prVdec->rSeqHdrR.u4NumMBX = prVdec->rSeqHdrR.u4PicWidthDec
                                            >> 4;
            prVdec->rSeqHdrR.u4NumMBY = prVdec->rSeqHdrR.u4PicHeightDec
                                            >> 4;
            prVdec->rSeqHdrR.u4PicWidthCmp
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFRAME].iFrmWidthSrc;
            prVdec->rSeqHdrR.u4PicHeightCmp
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFRAME].iFrmHeightSrc;

            prVdec->rPicLayer.fgInterlaceV2 = TRUE;
            prVdec->rPicLayer.fgFieldMode = FALSE;
            prVdec->rPicLayer.i4CurrentField = 0; // 0:TOP, 1:BOTTOM field

            decodeVOPHeadInterlaceV2(ucEsId); // advanced interlace-frame
        }
        else // INTERLACE_FIELD
        {
INTERLACE_FIELD:
            prVdec->rSeqHdrR.u4PicWidthDec
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFIELD].iWidthDec;
            prVdec->rSeqHdrR.u4PicHeightDec
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFIELD].iHeightDec;
            prVdec->rSeqHdrR.u4NumMBX = prVdec->rSeqHdrR.u4PicWidthDec
                                            >> 4;
            prVdec->rSeqHdrR.u4NumMBY = prVdec->rSeqHdrR.u4PicHeightDec
                                            >> 4;
            prVdec->rSeqHdrR.u4PicWidthCmp
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFIELD].iFrmWidthSrc;
            prVdec->rSeqHdrR.u4PicHeightCmp
            = prVdec->rSeqHdrR.rMultiResParams[INTERLACEFIELD].iFrmHeightSrc;

            prVdec->rPicLayer.fgInterlaceV2 = TRUE;
            prVdec->rPicLayer.fgFieldMode = TRUE;

            if (prVdec->rPicLayer.i4CurrentTemporalField == 0) //1st field
            {
                decodeVOPHeadFieldPicture(ucEsId); // advanced interlace-field
                prVdec->rPicLayer.i4SlicePicHeaderNumField
                = pic_hdr_bitcount(ucEsId);

                prVdec->rPicLayer.i4CurrentField
                = (prVdec->rPicLayer.fgTopFieldFirst) ? (UINT32)0 : (UINT32)1; // 0:TOP, 1:BOTTOM field

                prVdec->rPicLayer.u1PicType
                = prVdec->rPicLayer.u1FirstFieldType;
                decodeFieldHeadFieldPicture(ucEsId);
                if (prVdec->rPicLayer.u1PicType == PVOP)
                {
                    SetupFieldPictureMVScaling(ucEsId,prVdec->rEntryPoint.i4RefFrameDistance);
                    decodePFieldMode(ucEsId);
                }
                else if (prVdec->rPicLayer.u1PicType == BVOP)
                {
                    INT32
                    iForwardRefDistance =(INT32)
                                         (prVdec->rPicLayer.i4BNumerator
                                          * prVdec->rEntryPoint.i4RefFrameDistance
                                          * prVdec->rPicLayer.i4BFrameReciprocal)
                                         >> 8;
                    INT32 iBackwardRefDistance = (INT32)
                                                 (prVdec->rEntryPoint.i4RefFrameDistance
                                                  - iForwardRefDistance - 1);
                    // SetupForwardBFieldPictureMVScaling (ucEsId,pWMVDec, iForwardRefDistance);
                    if (iBackwardRefDistance < 0)
                        iBackwardRefDistance = 0;
                    SetupFieldPictureMVScaling(ucEsId,(UINT32)iForwardRefDistance);
                    SetupBackwardBFieldPictureMVScaling(ucEsId,(UINT32)iBackwardRefDistance);
                    decodeBFieldMode(ucEsId);

                    prVdec->rOther.i4FwdRefDist = iForwardRefDistance;
                    prVdec->rOther.i4BwdRefDist = iBackwardRefDistance;
                }
            }
            else //2nd field
            {
                prVdec->rPicLayer.u1PicType
                = prVdec->rPicLayer.u1SecondFieldType;
                decodeFieldHeadFieldPicture(ucEsId);
                if (prVdec->rPicLayer.u1PicType == PVOP)
                {
                    SetupFieldPictureMVScaling(ucEsId,prVdec->rEntryPoint.i4RefFrameDistance);
                    decodePFieldMode(ucEsId);

                }
                else if (prVdec->rPicLayer.u1PicType == BVOP)
                {
                    INT32
                    iForwardRefDistance = (INT32)
                                          (prVdec->rPicLayer.i4BNumerator
                                           * prVdec->rEntryPoint.i4RefFrameDistance
                                           * prVdec->rPicLayer.i4BFrameReciprocal)
                                          >> 8;
                    INT32 iBackwardRefDistance = (INT32)
                                                 (prVdec->rEntryPoint.i4RefFrameDistance
                                                  - iForwardRefDistance - 1);
                    //SetupForwardBFieldPictureMVScaling (ucEsId,pWMVDec, iForwardRefDistance);
                    if (iBackwardRefDistance < 0)
                        iBackwardRefDistance = 0;
                    SetupFieldPictureMVScaling(ucEsId,(UINT32)iForwardRefDistance);
                    SetupBackwardBFieldPictureMVScaling(ucEsId,(UINT32)iBackwardRefDistance);
                    decodeBFieldMode(ucEsId);

                    prVdec->rOther.i4FwdRefDist = iForwardRefDistance;
                    prVdec->rOther.i4BwdRefDist = iBackwardRefDistance;
                }
            }
        }

        prVdec->rPicLayer.i4SlicePicHeaderNum = pic_hdr_bitcount(ucEsId);
    }

    return 0;
}

UINT32 decodeSequenceHead_Advanced(UCHAR ucEsId)
{
    //UINT8 bReserved;
    BOOL fgDisplay_Ext;
    BOOL fgAspect_Ratio_Flag;
    BOOL fgFrameRate_Flag;
    BOOL fgFrameRateInd;
    BOOL fgColor_Format_Flag;
    UINT32 u4Idx;
    //UCHAR ucFbgType;
    //int iLevel = 0;
    //int iChromaFormat = 1;
    //UINT32 dwDisp_Horiz_Size;
    //UINT32 dwDisp_Vert_Size;
    //int iColor_Prim;
    //int iTransfer_Char;
    //int iMatrix_Coef;
    //int iBit_Rate_Exponent;
    //int iBuffer_Size_Exponent;
    //UINT16 wHrd_Rate[32];
    //UINT16 wHrd_Buffer[32];
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0); // Flush the Seq Head Start Code
    //LOG(0,"datain : %x\n", prVdec->rOther.u4Datain);
    //LOG(0,"Rptr 0x%x, input win 0x%x\n", ReadREG(ucEsId,RO_VLD_VRPTR), ReadREG(RO_VLD_BARL));

    //local _fgRndCtrlOn = TRUE;
    prVdec->rEntryPoint.fgRangeRedY_Flag = FALSE; // Resetting range red flags at the beginning of new advance sequence header
    prVdec->rEntryPoint.fgRangeRedUV_Flag = FALSE;

    // old/unused stuff
    prVdec->rSMProfile.fgMixedPel = prVdec->rSMProfile.fgFrmHybridMVOn
                                        = FALSE;
    prVdec->rSMProfile.fgYUV411 = FALSE;
    prVdec->rSMProfile.fgSpriteMode = FALSE;
    prVdec->rSMProfile.fgXintra8Switch = FALSE;
    prVdec->rSMProfile.fgXintra8 = FALSE;
    prVdec->rSMProfile.fgMultiresEnabled = FALSE;
    //local _fg16bitXform = TRUE;
    //if(_fg16bitXform)
    {
        prVdec->rSMProfile.fgRotatedIdct = TRUE;
    }

    //  prVdec->rSMProfile.fgDCTTable_MB_ENABLED = FALSE;

    prVdec->rOther.u4GetVal = dwGetHi(ucEsId,28);
    prVdec->rSeqLayer.i4Profile = (prVdec->rOther.u4GetVal & 0xc000000)
                                      >> 26;
    prVdec->rSeqLayer.i4WMV3Profile = WMV3_ADVANCED_PROFILE;
    //iLevel = (prVdec->rOther.u4GetVal & 0x3800000) >> 23;
    //iChromaFormat = (prVdec->rOther.u4GetVal & 0x600000) >> 21;
    prVdec->rSeqLayer.i4FrameRate
    = (prVdec->rOther.u4GetVal & 0x1c0000) >> 18;
    prVdec->rSeqLayer.i4BitRate = (prVdec->rOther.u4GetVal & 0x3e000)
                                      >> 13;
    prVdec->rSeqLayer.fgPostProcInfoPresent = (prVdec->rOther.u4GetVal
            & 0x1000) >> 12;
    prVdec->rSeqHdrR.u4MaxCodedWidth = prVdec->rOther.u4GetVal & 0xfff;

    prVdec->rOther.u4GetVal = dwGetHi(ucEsId,19);
    prVdec->rSeqHdrR.u4MaxCodedHeight = (prVdec->rOther.u4GetVal
                                            & 0x7ff80) >> 7;

    //reset seq hdr
    prVdec->rOther.u1GetSeqHdr = 0;

    // Robert: translate to what I use
    prVdec->rSeqHdrR.u4MaxPicWidthSrc = 2
                                            * prVdec->rSeqHdrR.u4MaxCodedWidth + 2;
    prVdec->rSeqHdrR.u4MaxPicHeightSrc = 2
            * prVdec->rSeqHdrR.u4MaxCodedHeight + 2;
    prVdec->rSeqHdrR.u4PicWidthSrc = 2
                                         * prVdec->rSeqHdrR.u4MaxCodedWidth + 2;
    prVdec->rSeqHdrR.u4PicHeightSrc = 2
                                          * prVdec->rSeqHdrR.u4MaxCodedHeight + 2;

    prVdec->rOther.rPicInfo.dwW = prVdec->rSeqHdrR.u4PicWidthSrc;
    prVdec->rOther.rPicInfo.dwH = prVdec->rSeqHdrR.u4PicHeightSrc;

    SetupMultiResParams(ucEsId);

    prVdec->rSeqLayer.fgBroadcastFlags = (prVdec->rOther.u4GetVal
            & 0x40) >> 6;
    prVdec->rSeqLayer.fgInterlacedSource = (prVdec->rOther.u4GetVal
            & 0x20) >> 5;
    prVdec->rSeqLayer.fgTemporalFrmCntr = (prVdec->rOther.u4GetVal
            & 0x10) >> 4;
    prVdec->rSeqLayer.fgSeqFrameInterpolation
    = (prVdec->rOther.u4GetVal & 0x8) >> 3;
    //bReserved = (UINT8)((prVdec->rOther.u4GetVal & 0x6) >> 1);
    prVdec->rSeqLayer.fgProgressiveDisp = (prVdec->rOther.u4GetVal
            & 0x2)>>1;
    fgDisplay_Ext = prVdec->rOther.u4GetVal & 0x1;

    if (fgDisplay_Ext)
    {
        prVdec->rOther.u4GetVal = dwGetHi(ucEsId,29);
        //dwDisp_Horiz_Size = (prVdec->rOther.u4GetVal & 0x1fff8000) >> 15;
        //dwDisp_Vert_Size = (prVdec->rOther.u4GetVal & 0x7ffe) >> 1;
        fgAspect_Ratio_Flag = prVdec->rOther.u4GetVal & 0x1;
        if (fgAspect_Ratio_Flag)
        {
            prVdec->rSeqLayer.i4Aspect_Ratio = dwGetHi(ucEsId,4);
            if (prVdec->rSeqLayer.i4Aspect_Ratio == 15)
            {
                prVdec->rOther.u4GetVal = dwGetHi(ucEsId,16);
                prVdec->rSeqLayer.i4Aspect_Horiz_Size
                = (prVdec->rOther.u4GetVal & 0xff00) >> 8;
                prVdec->rSeqLayer.i4Aspect_Vert_Size
                = prVdec->rOther.u4GetVal & 0xff;
            }
        }
        fgFrameRate_Flag = dwGetHi(ucEsId,1);
        if (fgFrameRate_Flag)
        {
            fgFrameRateInd = dwGetHi(ucEsId,1);
            if (fgFrameRateInd == FALSE)
            {
                prVdec->rOther.u4GetVal = dwGetHi(ucEsId,12);
                prVdec->rSeqLayer.i4FrameRateNr
                = (prVdec->rOther.u4GetVal & 0xff0) >> 4;
                prVdec->rSeqLayer.i4FrameRateDr
                = prVdec->rOther.u4GetVal & 0xf;
            }
            else
            {
                 prVdec->rSeqLayer.i4FrameRateNr = dwGetHi(ucEsId,16) + 1;
            }
        }
        fgColor_Format_Flag = dwGetHi(ucEsId,1);
        if (fgColor_Format_Flag)
        {
            prVdec->rOther.u4GetVal = dwGetHi(ucEsId,24);
            //iColor_Prim = (prVdec->rOther.u4GetVal & 0xff0000) >> 16;
            //iTransfer_Char = (prVdec->rOther.u4GetVal & 0xff00) >> 8;
            //iMatrix_Coef = prVdec->rOther.u4GetVal & 0xff;
        }
    }

    prVdec->rSeqLayer.fgHRD_PARAM_FLAG = dwGetHi(ucEsId,1);
    if (prVdec->rSeqLayer.fgHRD_PARAM_FLAG)
    {
        //HDR_PARAM()
        prVdec->rOther.u4GetVal = dwGetHi(ucEsId,13);
        prVdec->rSeqLayer.i4hrd_num_leaky_buckets
        = (prVdec->rOther.u4GetVal & 0x1f00) >> 8;
        //iBit_Rate_Exponent = (prVdec->rOther.u4GetVal & 0xf0) >> 4;
        //iBuffer_Size_Exponent = prVdec->rOther.u4GetVal & 0xf;

        for (u4Idx = 1; u4Idx <= prVdec->rSeqLayer.i4hrd_num_leaky_buckets; u4Idx++)
        {
            prVdec->rOther.u4GetVal = dwGetHi(ucEsId,32);
            //wHrd_Rate[i] = (UINT16)((prVdec->rOther.u4GetVal & 0xffff0000) >> 16);
            //wHrd_Buffer[i] = (UINT16)(prVdec->rOther.u4GetVal & 0xffff);
        }
    }

    prVdec->rPicLayer.i4BFrameReciprocal
    = _iBInverse[prVdec->rSMProfile.i4NumBFrames];
    return (TRUE);
}

UINT32 DecodeEntryPointHeader(UCHAR ucEsId)
{
    UINT32 u4Idx;
    BOOL fgCoded_Size_Flag;
    //UINT8 bHrd_Fullness[32];
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);

    prVdec->rOther.u4new_entry_point = 1;

    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0); // Flush the Entry Point Start Code
    //LOG(0,"Rptr 0x%x, input win 0x%x\n", ReadREG(ucEsId,RO_VLD_VRPTR), ReadREG(ucEsId,RO_VLD_BARL));
    prVdec->rOther.u4GetVal = dwGetHi(ucEsId,13);
    prVdec->rEntryPoint.fgBrokenLink = (prVdec->rOther.u4GetVal
                                            & 0x1000) >> 12;
    prVdec->rEntryPoint.fgClosedEntryPoint = (prVdec->rOther.u4GetVal
            & 0x800) >> 11;
    prVdec->rEntryPoint.fgPanScanPresent = (prVdec->rOther.u4GetVal
            & 0x400) >> 10;
    prVdec->rEntryPoint.fgRefDistPresent = (prVdec->rOther.u4GetVal
            & 0x200) >> 9;
    prVdec->rEntryPoint.fgLoopFilter
    = (prVdec->rOther.u4GetVal & 0x100) >> 8;
    prVdec->rEntryPoint.fgUVHpelBilinear = (prVdec->rOther.u4GetVal
            & 0x80) >> 7;
    prVdec->rEntryPoint.fgExtendedMvMode = (prVdec->rOther.u4GetVal
            & 0x40) >> 6;
    prVdec->rEntryPoint.i4DQuantCodingOn = (prVdec->rOther.u4GetVal
            & 0x30) >> 4;
    prVdec->rEntryPoint.fgXformSwitch = (prVdec->rOther.u4GetVal & 0x8)
                                            >> 3;
    prVdec->rEntryPoint.fgSequenceOverlap = (prVdec->rOther.u4GetVal
            & 0x4) >> 2;
    prVdec->rEntryPoint.fgExplicitSeqQuantizer
    = (prVdec->rOther.u4GetVal & 0x2) >> 1;
    if (prVdec->rEntryPoint.fgExplicitSeqQuantizer)
        prVdec->rPicLayer.fgUse3QPDZQuantizer = prVdec->rOther.u4GetVal
                & 0x1;
    else
        prVdec->rEntryPoint.fgExplicitFrameQuantizer
        = prVdec->rOther.u4GetVal & 0x1;

    prVdec->rEntryPoint.fgExplicitQuantizer
    = prVdec->rEntryPoint.fgExplicitSeqQuantizer
      || prVdec->rEntryPoint.fgExplicitFrameQuantizer;
    prVdec->rPicLayer.i4BFrameReciprocal
    = _iBInverse[prVdec->rSMProfile.i4NumBFrames];
    prVdec->rPicLayer.u1PicType = IVOP;

    if (prVdec->rSeqLayer.fgHRD_PARAM_FLAG)
    {
        //hrd_fullness()
        for (u4Idx = 1; u4Idx <= prVdec->rSeqLayer.i4hrd_num_leaky_buckets; u4Idx++)
        {
            /*bHrd_Fullness[i] = (UINT8)*/
            dwGetHi(ucEsId,8);
        }
    }

    fgCoded_Size_Flag = dwGetHi(ucEsId,1);
    if (fgCoded_Size_Flag)
    {
        prVdec->rOther.u4GetVal = dwGetHi(ucEsId,24);
        prVdec->rEntryPoint.u2Coded_Width
        = (UINT16)((prVdec->rOther.u4GetVal & 0xfff000) >> 12);
        prVdec->rEntryPoint.u2Coded_Height
        = (UINT16)(prVdec->rOther.u4GetVal & 0xfff);

        //reset seq hdr
        prVdec->rOther.u1GetSeqHdr = 0;

        //ASSERT(prVdec->rEntryPoint.u2Coded_Width <= prVdec->rSeqHdrR.u4MaxCodedWidth);
        //ASSERT(prVdec->rEntryPoint.u2Coded_Height <= prVdec->rSeqHdrR.u4MaxCodedHeight);
        if ((prVdec->rEntryPoint.u2Coded_Width
                <= prVdec->rSeqHdrR.u4MaxCodedWidth)
                && (prVdec->rEntryPoint.u2Coded_Height
                    <= prVdec->rSeqHdrR.u4MaxCodedHeight))
        {
            // Robert: translate to what I use
            prVdec->rSeqHdrR.u4PicWidthSrc = 2
                                                 * (UINT32)prVdec->rEntryPoint.u2Coded_Width + 2;
            prVdec->rSeqHdrR.u4PicHeightSrc = 2
                                                  * (UINT32)prVdec->rEntryPoint.u2Coded_Height + 2;

            prVdec->rOther.rPicInfo.dwW
            = prVdec->rSeqHdrR.u4PicWidthSrc;
            prVdec->rOther.rPicInfo.dwH
            = prVdec->rSeqHdrR.u4PicHeightSrc;
            SetupMultiResParams(ucEsId);
        }
    }
    if (prVdec->rEntryPoint.fgExtendedMvMode)
    {
        prVdec->rEntryPoint.fgExtendedDeltaMvMode = dwGetHi(ucEsId,1);
    }

    prVdec->rEntryPoint.fgRangeRedY_Flag = dwGetHi(ucEsId,1);
    if (prVdec->rEntryPoint.fgRangeRedY_Flag)
        prVdec->rEntryPoint.i4RangeRedY = dwGetHi(ucEsId,3) + 1;
    else
        prVdec->rEntryPoint.i4RangeRedY = 0;

    prVdec->rEntryPoint.fgRangeRedUV_Flag = dwGetHi(ucEsId,1);
    if (prVdec->rEntryPoint.fgRangeRedUV_Flag)
        prVdec->rEntryPoint.i4Range_MapUV = dwGetHi(ucEsId,3) + 1;
    else
        prVdec->rEntryPoint.i4Range_MapUV = 0;

    return (TRUE);
}

void UpdateDCStepSize(UCHAR ucEsId, UINT32 iStepSize)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rPicLayer.i4StepSize = iStepSize;
    //Printf("codec version wmva 44: %d\n",prVdec->rOther.i4CodecVersion);

    if (prVdec->rOther.i4CodecVersion >= WMV3)
    {
        DQuantDecParam *pDQ;
        iStepSize = (UINT8)((2 * iStepSize - 1) + (UINT8)prVdec->rPicLayer.fgHalfStep);
        pDQ = &prVdec->rPicLayer.prDQuantParam[iStepSize];
        prVdec->rPicLayer.i4DCStepSize = pDQ->iDCStepSize;
    }
    else
    {
        prVdec->rPicLayer.i4DCStepSize = 8;

        if ((prVdec->rOther.i4CodecVersion >= MMIDRATE43)
                || (prVdec->rOther.i4CodecVersion == MMIDRATE4S))
        {
            if (prVdec->rPicLayer.i4StepSize <= 4)
            {
                prVdec->rPicLayer.i4DCStepSize = 8;
                if (prVdec->rEntryPoint.fgNewDCQuant
                        && (prVdec->rPicLayer.i4StepSize <=2))
                    prVdec->rPicLayer.i4DCStepSize
                    = prVdec->rPicLayer.i4StepSize * 2;
            }
            else if (prVdec->rOther.i4CodecVersion >= WMV1)
            {
                prVdec->rPicLayer.i4DCStepSize
                = prVdec->rPicLayer.i4StepSize / 2 + 6;
            }
            else if (prVdec->rPicLayer.i4StepSize <= 8)
            {
                prVdec->rPicLayer.i4DCStepSize = 2
                                                     * prVdec->rPicLayer.i4StepSize;
            }
            else if (prVdec->rPicLayer.i4StepSize <= 24)
            {
                prVdec->rPicLayer.i4DCStepSize
                = prVdec->rPicLayer.i4StepSize + 8;
            }
            else
            {
                prVdec->rPicLayer.i4DCStepSize = 2
                                                     * prVdec->rPicLayer.i4StepSize - 16;
            }
        }
    }
}

UINT32 decodeVOPHeadProgressiveWMVA(UCHAR ucEsId)
{
    BOOL fgPanScanPresent;
    //BOOL fgInterpolateCurrentFrame;
    UINT32 dwNumberOfPanScanWindows;
    //BOOL fgUVProgressiveSubsampling;
    //int iPpMethod;
    //int iACPRED;
    //int iOVERFLAGS;
    //int iMVTYPEMB;
    //int iDIRECTMB;
    //int iSKIPMB;
    BOOL fgTransTypeMB;
    UINT32 iTransTypeFrame;

    UINT32 i, dwIdx;
    //UINT32 dwTmp;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rPicLayer.u1PrevPicType = prVdec->rPicLayer.u1PicType;
    // PTYPE
    //  dwTmp = (prVdec->rOther.u4Datain & 0xf0000000) >> 28;
    //  prVdec->rPicLayer.u1PicType = WmvAdvPicType[dwTmp].bPicType;
    //  prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,WmvAdvPicType[dwTmp].dwBits);
    if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = PVOP;
    else if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = BVOP;
    else if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = IVOP;
    else if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = BIVOP;
    else
        prVdec->rPicLayer.u1PicType = SKIPFRAME;

    if (prVdec->rPicLayer.u1PicType != SKIPFRAME) // SKIPFRAME does not contain temporal reference
    {
        if (prVdec->rSeqLayer.fgTemporalFrmCntr)
        {
            prVdec->rPicLayer.i4TemporalRef = dwGetHi(ucEsId,8);
        }
    }

    if (prVdec->rSeqLayer.fgBroadcastFlags)
    {
        if (prVdec->rSeqLayer.fgInterlacedSource)
        {
            prVdec->rOther.u4GetVal = dwGetHi(ucEsId,2);
            prVdec->rPicLayer.fgTopFieldFirst
            = (prVdec->rOther.u4GetVal & 0x2) >> 31;
            prVdec->rPicLayer.fgRepeatFirstField
            = prVdec->rOther.u4GetVal & 0x1;
        }
        else
        {
            prVdec->rPicLayer.u1RepeatFrameCount = (UINT8)dwGetHi(ucEsId,2);
        }
    }

    if (prVdec->rEntryPoint.fgPanScanPresent)
    {
        fgPanScanPresent = dwGetHi(ucEsId,1);
        if (fgPanScanPresent)
        {
            if (prVdec->rSeqLayer.fgInterlacedSource)
            {
                if (prVdec->rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(2 + prVdec->rPicLayer.fgRepeatFirstField);
                else
                    dwNumberOfPanScanWindows = 2;
            }
            else
            {
                if (prVdec->rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(1 + prVdec->rPicLayer.u1RepeatFrameCount);
                else
                    dwNumberOfPanScanWindows = 1;
            }
            for (i = 0; i < dwNumberOfPanScanWindows; i++)
            {
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanHorizOffset
                = dwGetHi(ucEsId,18);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanVertOffset
                = dwGetHi(ucEsId,18);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanWidth
                = dwGetHi(ucEsId,14);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanHeight
                = dwGetHi(ucEsId,14);
            }
        } // fgPanScanPresent
    }

    if (prVdec->rPicLayer.u1PicType == SKIPFRAME)  // ??? !!!1123
    {
        return (TRUE);
    }

    prVdec->rPicLayer.i4RndCtrl = dwGetHi(ucEsId,1);

    if (prVdec->rSeqLayer.fgInterlacedSource)
    {
        /*fgUVProgressiveSubsampling =*/
        dwGetHi(ucEsId,1);
    }
    if (prVdec->rSeqLayer.fgSeqFrameInterpolation)
    {
        /*fgInterpolateCurrentFrame =*/
        dwGetHi(ucEsId,1);
    }

    // Decode B FRACION
    if (prVdec->rPicLayer.u1PicType == BVOP)
    {
        dwIdx = dwGetHi(ucEsId,3);
        if (dwIdx == 0x7)
        {
            dwIdx = dwGetHi(ucEsId,4);
            if (dwIdx == 0xe) // "Invalid" in VLC
                return INVALID_32;
            prVdec->rPicLayer.i4BNumerator = _iNumLongVLC[dwIdx];
            prVdec->rPicLayer.i4BDenominator = _iDenLongVLC[dwIdx];
        }
        else
        {
            prVdec->rPicLayer.i4BNumerator = _iNumShortVLC[dwIdx];
            prVdec->rPicLayer.i4BDenominator = _iDenShortVLC[dwIdx];
        }
        prVdec->rPicLayer.i4BFrameReciprocal
        = _iBInverse[prVdec->rPicLayer.i4BDenominator - 1];
        if (prVdec->rSMProfile.i4NumBFrames == 0)
            prVdec->rSMProfile.i4NumBFrames = 1;
    }

    prVdec->rPicLayer.i4PicQtIdx = dwGetHi(ucEsId,5);

    if (prVdec->rPicLayer.i4PicQtIdx <= MAXHALFQP)
    {
        prVdec->rPicLayer.fgHalfStep = dwGetHi(ucEsId,1);
    }
    else
        prVdec->rPicLayer.fgHalfStep = FALSE;

    if (prVdec->rEntryPoint.fgExplicitFrameQuantizer)
    {
        prVdec->rPicLayer.fgUse3QPDZQuantizer = dwGetHi(ucEsId,1);
    }

    if (!prVdec->rEntryPoint.fgExplicitQuantizer)
    {
        if (prVdec->rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = TRUE;
            prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;
        }
        else
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = FALSE;
            prVdec->rPicLayer.i4StepSize
            = _iStepRemap[prVdec->rPicLayer.i4PicQtIdx];
        }
    }
    else
        prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;

    prVdec->rPicLayer.prDQuantParam
    = prVdec->rPicLayer.fgUse3QPDZQuantizer ? prVdec->rPicLayer.rDQuantParam3QPDeadzone
      : prVdec->rPicLayer.rDQuantParam5QPDeadzone;

    prVdec->rPicLayer.i4Overlap = 0;
    if (prVdec->rEntryPoint.fgSequenceOverlap
            && (prVdec->rPicLayer.u1PicType != BVOP))
    {
        if (prVdec->rPicLayer.i4StepSize >= 9)
            prVdec->rPicLayer.i4Overlap = 1;
        else if (prVdec->rPicLayer.u1PicType == IVOP
                 || prVdec->rPicLayer.u1PicType == BIVOP)
            prVdec->rPicLayer.i4Overlap = 7; // last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (prVdec->rSeqLayer.fgPostProcInfoPresent)
    {
        /*iPpMethod =*/
        dwGetHi(ucEsId,2);
    }

    vSetNewMode(ucEsId);

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        //ACPRED, Bitplane

        //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iACPRED =*/
            iDecodeBitPlane(ucEsId,5); // "ACPRED", bp_num = 5
        }
        else
        {
            fgHWDecBP(ucEsId,5,TRUE);
        }


        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        if (prVdec->rPicLayer.i4Overlap & 2)
        {
            if (0 == dwGetHi(ucEsId,1))
                prVdec->rPicLayer.i4Overlap = 0;
            else if (0 == dwGetHi(ucEsId,1))
                prVdec->rPicLayer.i4Overlap = 1;
            else
            {

                //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iOVERFLAGS = */
                    iDecodeBitPlane(ucEsId,6); // "OVERFLAGS", bp_num = 6
                }
                else
                {
                    fgHWDecBP(ucEsId,6,TRUE);
                }
                prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
            }
        }

        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACInterTableIndx)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACIntraTableIndx)
        {
            prVdec->rPicLayer.u4DCTACIntraTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);

        if (prVdec->rEntryPoint.i4DQuantCodingOn != 0)
            vDecodeVOPDQuant(ucEsId);
        else
            vSetDefaultDQuantSetting(ucEsId);

        if (prVdec->rPicLayer.u1PicType == IVOP)
        {
            prVdec->rOther.fgLuminanceWarp = FALSE;
            prVdec->rOther.fgLuminanceWarpTop
            = prVdec->rOther.fgLuminanceWarpBottom = FALSE;
        }

    }// End of IVOP, BIVOP
    else if ((prVdec->rPicLayer.u1PicType == PVOP)
             || (prVdec->rPicLayer.u1PicType == BVOP))
    {
        if (prVdec->rEntryPoint.fgExtendedMvMode)
        {
            // MVRANGE0 = 0, MVRANGE1 = 1,  MVRANGE2 = 2, MVRANGE3 = 3
            prVdec->rEntryPoint.i4MVRangeIndex = dwGetHi(ucEsId,1); // 0: MVRANGE0;
            if (prVdec->rEntryPoint.i4MVRangeIndex)
            {
                prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1); // 1: MVRANGE1
                if (prVdec->rEntryPoint.i4MVRangeIndex == 2)
                {
                    prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1); // 2: MVRANGE2,  3: MVRANGE3
                }
            }
        }

        //Start==== decodeVOPHead_WMV3() ===================================

        prVdec->rOther.fgLuminanceWarp = FALSE;
        prVdec->rOther.fgLuminanceWarpTop
        = prVdec->rOther.fgLuminanceWarpBottom = FALSE;

        if ((prVdec->rPicLayer.u1PicType == BVOP)
                && (!prVdec->rPicLayer.fgFieldMode))
        {
            if (dwGetHi(ucEsId,1))
            {
                prVdec->rOther.i4X9MVMode = ALL_1MV;
            }
            else
            {
                prVdec->rOther.i4X9MVMode = ALL_1MV_HALFPEL_BILINEAR;
            }

            //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                /*iDIRECTMB =*/
                iDecodeBitPlane(ucEsId,3); // "DirectMB", bp_num = 3
            }
            else
            {
                fgHWDecBP(ucEsId,3,TRUE);
            }


            prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
        }
        else
        {
            prVdec->rOther.i4X9MVMode = iGetPMvMode(ucEsId,
                                                prVdec->rPicLayer.i4StepSize, FALSE);
            if (prVdec->rOther.i4X9MVMode == INTENSITY_COMPENSATION)
            {
                prVdec->rOther.fgLuminanceWarp = TRUE;
                prVdec->rOther.i4X9MVMode = iGetPMvMode(ucEsId,
                                                    prVdec->rPicLayer.i4StepSize, TRUE);
                prVdec->rOther.i4LumScale = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumShift = dwGetHi(ucEsId,6);
            }

            if (prVdec->rOther.i4X9MVMode == MIXED_MV)
            {
                //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iMVTYPEMB =*/
                    iDecodeBitPlane(ucEsId,1); //"4mv", bp_num = 1
                }
                else
                {
                    fgHWDecBP(ucEsId,1,TRUE);
                }

                prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
            }
        }


        //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iSKIPMB =*/
            iDecodeBitPlane(ucEsId,0); // "SkipMB", bp_num = 0
        }
        else
        {
            fgHWDecBP(ucEsId,0,TRUE);
        }

        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        prVdec->rSMProfile.fgCODFlagOn = TRUE;

        prVdec->rOther.i4MvTable = dwGetHi(ucEsId,2);
        prVdec->rOther.i4CBPTable = dwGetHi(ucEsId,2);
        if (prVdec->rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant(ucEsId);
        }

        if (prVdec->rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(ucEsId,1);
            if (fgTransTypeMB)
            {
                prVdec->rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(ucEsId,2);
                prVdec->rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                prVdec->rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            prVdec->rOther.fgMBXformSwitching = FALSE;
        }

        //End==== decodeVOPHead_WMV3() ===================================


        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1); // Coding set index = 0, 1, or 2.
        if (prVdec->rPicLayer.u4DCTACInterTableIndx == 1)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx
        = prVdec->rPicLayer.u4DCTACInterTableIndx;

        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);
        //=================================================================
    } // PVOP, BVOP

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == PVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        UpdateDCStepSize(ucEsId,prVdec->rPicLayer.i4StepSize);
    }

    return (TRUE);
}

UINT32 decodeVOPHeadInterlaceV2(UCHAR ucEsId)
{
    BOOL fgPSPresent;
    UINT32 dwNumberOfPanScanWindows;
    //BOOL fgUVProgressiveSubsampling;
    //int iPpMethod;
    //int iACPRED;
    //int iOVERFLAGS;
    //int iDIRECTMB;
    //int iSKIPMB;
    //int iFIELDTX;
    BOOL fgTransTypeMB;
    UINT32 iTransTypeFrame;

    UINT32 i, dwIdx;
    //UINT32 dwTmp;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
#ifdef TEST
    _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif

    prVdec->rPicLayer.u1PrevPicType = prVdec->rPicLayer.u1PicType;
    // PTYPE
    if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = PVOP;
    else if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = BVOP;
    else if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = IVOP;
    else if (0 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = BIVOP;
    else
        prVdec->rPicLayer.u1PicType = SKIPFRAME;

    if (prVdec->rPicLayer.u1PicType != SKIPFRAME) // SKIPFRAME does not contain temporal reference
    {
        if (prVdec->rSeqLayer.fgTemporalFrmCntr)
            prVdec->rPicLayer.i4TemporalRef = dwGetHi(ucEsId,8);
    }

    if (prVdec->rSeqLayer.fgBroadcastFlags)
    {
        if (prVdec->rSeqLayer.fgInterlacedSource)
        {
            prVdec->rOther.u4GetVal = dwGetHi(ucEsId,2);
            prVdec->rPicLayer.fgTopFieldFirst
            = (prVdec->rOther.u4GetVal & 0x2) >> 1;
            prVdec->rPicLayer.fgRepeatFirstField
            = prVdec->rOther.u4GetVal & 0x1;
        }
        else
        {
            prVdec->rPicLayer.u1RepeatFrameCount = dwGetHi(ucEsId,2);
        }
    }

    if (prVdec->rEntryPoint.fgPanScanPresent)
    {
        fgPSPresent = dwGetHi(ucEsId,1);
        if (fgPSPresent)
        {
            if (prVdec->rSeqLayer.fgInterlacedSource)
            {
                if (prVdec->rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(2 + prVdec->rPicLayer.fgRepeatFirstField);
                else
                    dwNumberOfPanScanWindows = 2;
            }
            else
            {
                if (prVdec->rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(1 + prVdec->rPicLayer.u1RepeatFrameCount);
                else
                    dwNumberOfPanScanWindows = 1;
            }
            for (i = 0; i < dwNumberOfPanScanWindows; i++)
            {
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanHorizOffset
                = dwGetHi(ucEsId,18);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanVertOffset
                = dwGetHi(ucEsId,18);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanWidth
                = dwGetHi(ucEsId,14);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanHeight
                = dwGetHi(ucEsId,14);
            }
        } // fgPSPresent
    }

    if (prVdec->rPicLayer.u1PicType == SKIPFRAME)  // ??? !!!1123
    {
        return (TRUE);
    }

    prVdec->rPicLayer.i4RndCtrl = dwGetHi(ucEsId,1);

    if (prVdec->rSeqLayer.fgInterlacedSource)
    {
        /*fgUVProgressiveSubsampling =*/
        dwGetHi(ucEsId,1);
    }

    prVdec->rPicLayer.i4PicQtIdx = dwGetHi(ucEsId,5);

    if (prVdec->rPicLayer.i4PicQtIdx <= MAXHALFQP)
        prVdec->rPicLayer.fgHalfStep = dwGetHi(ucEsId,1);
    else
        prVdec->rPicLayer.fgHalfStep = FALSE;

    if (prVdec->rEntryPoint.fgExplicitFrameQuantizer)
        prVdec->rPicLayer.fgUse3QPDZQuantizer = dwGetHi(ucEsId,1);

    if (!prVdec->rEntryPoint.fgExplicitQuantizer)
    {
        if (prVdec->rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = TRUE;
            prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;
        }
        else
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = FALSE;
            prVdec->rPicLayer.i4StepSize
            = _iStepRemap[prVdec->rPicLayer.i4PicQtIdx];
        }
    }
    else
        prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;

    prVdec->rPicLayer.prDQuantParam
    = prVdec->rPicLayer.fgUse3QPDZQuantizer ? prVdec->rPicLayer.rDQuantParam3QPDeadzone
      : prVdec->rPicLayer.rDQuantParam5QPDeadzone;

    prVdec->rPicLayer.i4Overlap = 0;
    if (prVdec->rEntryPoint.fgSequenceOverlap
            && (prVdec->rPicLayer.u1PicType != BVOP))
    {
        if (prVdec->rPicLayer.i4StepSize >= 9)
            prVdec->rPicLayer.i4Overlap = 1;
        else if (prVdec->rPicLayer.u1PicType == IVOP
                 || prVdec->rPicLayer.u1PicType == BIVOP)
            prVdec->rPicLayer.i4Overlap = 7; // last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (prVdec->rSeqLayer.fgPostProcInfoPresent)
    {
        /*iPpMethod =*/
        dwGetHi(ucEsId,2);
    }

    // Decode B FRACION
    if (prVdec->rPicLayer.u1PicType == BVOP)
    {
        dwIdx = dwGetHi(ucEsId,3);
        if (dwIdx == 0x7)
        {
            dwIdx = dwGetHi(ucEsId,4);
            if (dwIdx == 0xe) // "Invalid" in VLC
                return INVALID_32;
            prVdec->rPicLayer.i4BNumerator = _iNumLongVLC[dwIdx];
            prVdec->rPicLayer.i4BDenominator = _iDenLongVLC[dwIdx];
        }
        else
        {
            prVdec->rPicLayer.i4BNumerator = _iNumShortVLC[dwIdx];
            prVdec->rPicLayer.i4BDenominator = _iDenShortVLC[dwIdx];
        }
        prVdec->rPicLayer.i4BFrameReciprocal
        = _iBInverse[prVdec->rPicLayer.i4BDenominator - 1];
        if (prVdec->rSMProfile.i4NumBFrames == 0)
            prVdec->rSMProfile.i4NumBFrames = 1;
    }

    vSetNewMode(ucEsId);

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        //FIELDTX, Bitplane
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iFIELDTX =*/
            iDecodeBitPlane(ucEsId, 4); // "FIELDTX", bp_num = 4
        }
        else
        {
            fgHWDecBP(ucEsId, 4, TRUE);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        //ACPRED, Bitplane
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iACPRED =*/
            iDecodeBitPlane(ucEsId, 5); // "ACPRED", bp_num = 5
        }
        else
        {
            fgHWDecBP(ucEsId, 5, TRUE);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        if (prVdec->rPicLayer.i4Overlap & 2)
        {
            if (0 == dwGetHi(ucEsId,1))
                prVdec->rPicLayer.i4Overlap = 0;
            else if (0 == dwGetHi(ucEsId,1))
                prVdec->rPicLayer.i4Overlap = 1;
            else
            {
                //OVERFLAGS, Bitplane
#ifdef TEST
                _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
                //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iOVERFLAGS =*/
                    iDecodeBitPlane(ucEsId, 6); // "OVERFLAGS", bp_num = 6
                }
                else
                {
                    fgHWDecBP(ucEsId, 6, TRUE);
                }
#ifdef TEST
                _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
                prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
            }
        }

        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACInterTableIndx)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACIntraTableIndx)
        {
            prVdec->rPicLayer.u4DCTACIntraTableIndx += dwGetHi(ucEsId,1);
        }

        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);

        if (prVdec->rEntryPoint.i4DQuantCodingOn != 0)
            vDecodeVOPDQuant(ucEsId);
        else
            vSetDefaultDQuantSetting(ucEsId);

    }// End of IVOP, BIVOP
    else if ((prVdec->rPicLayer.u1PicType == PVOP)
             || (prVdec->rPicLayer.u1PicType == BVOP))
    {
        UINT32 iMBModeIndex;

        if (prVdec->rEntryPoint.fgExtendedMvMode)
        {
            // MVRANGE0 = 0, MVRANGE1 = 1,  MVRANGE2 = 2, MVRANGE3 = 3
            prVdec->rEntryPoint.i4MVRangeIndex = dwGetHi(ucEsId,1); // 0: MVRANGE0;
            if (prVdec->rEntryPoint.i4MVRangeIndex)
            {
                prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1); // 1: MVRANGE1
                if (prVdec->rEntryPoint.i4MVRangeIndex == 2)
                {
                    prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1); // 2: MVRANGE2,  3: MVRANGE3
                }
            }
        }

        if (prVdec->rEntryPoint.fgExtendedDeltaMvMode)
        {
            prVdec->rEntryPoint.i4DeltaMVRangeIndex = dwGetHi(ucEsId,1);
            if (prVdec->rEntryPoint.i4DeltaMVRangeIndex)
            {
                prVdec->rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(ucEsId,1);
                if (prVdec->rEntryPoint.i4DeltaMVRangeIndex == 2)
                {
                    prVdec->rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(ucEsId,1);
                }
            }
            prVdec->rEntryPoint.i4ExtendedDMVX
            = prVdec->rEntryPoint.i4DeltaMVRangeIndex & 1;
            prVdec->rEntryPoint.i4ExtendedDMVY
            = (prVdec->rEntryPoint.i4DeltaMVRangeIndex & 2) >> 1;
        }

        //    prVdec->rOther.fgLuminanceWarpTop = prVdec->rOther.fgLuminanceWarpBottom = FALSE;
        //4MVSWITCH
        if (prVdec->rPicLayer.u1PicType == BVOP)
            prVdec->rOther.i4X9MVMode = ALL_1MV; //Interlaced Frame B picture has only ALL_1MV mode.
        else
        {
            if (dwGetHi(ucEsId,1))
                prVdec->rOther.i4X9MVMode = MIXED_MV;
            else
                prVdec->rOther.i4X9MVMode = ALL_1MV;
        }

        //INTCOMP
        prVdec->rOther.fgLuminanceWarp = FALSE;
        if (prVdec->rPicLayer.u1PicType == BVOP)
        {
            dwGetHi(ucEsId,1); //No matter what value this bit is,
            prVdec->rOther.fgLuminanceWarp = FALSE; //Shall always be FALSE for Interlaced Frame B picture.
        }
        else //PVOP
        {
            if (dwGetHi(ucEsId,1))
            {
                prVdec->rOther.fgLuminanceWarp = TRUE;
            }
        }
        if (prVdec->rOther.fgLuminanceWarp)
        {
            prVdec->rOther.i4LumScale = dwGetHi(ucEsId,6);
            prVdec->rOther.i4LumShift = dwGetHi(ucEsId,6);
        }

        if (prVdec->rPicLayer.u1PicType == BVOP)
        {
            //DIRECTMB, Bitplane
#ifdef TEST
            _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
            //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                /*iDIRECTMB =*/
                iDecodeBitPlane(ucEsId, 3); //"DIRECTMB", bp_num = 3
            }
            else
            {
                fgHWDecBP(ucEsId, 3, TRUE);
            }
#ifdef TEST
            _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
            prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
        }

        //SKIPMB, Bitplane
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iSKIPMB =*/
            iDecodeBitPlane(ucEsId, 0); // "SKIPMB", bp_num = 0
        }
        else
        {
            fgHWDecBP(ucEsId, 0, TRUE);
        }

        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        prVdec->rSMProfile.fgCODFlagOn = TRUE;

        iMBModeIndex = dwGetHi(ucEsId,2);

        if (prVdec->rOther.i4X9MVMode == MIXED_MV)
        {
            prVdec->rOther.i4MBModeTable = iMBModeIndex; //ming add
        }
        else
        {
            prVdec->rOther.i4MBModeTable = 4 + (iMBModeIndex & 0x3); //ming add
        }
        prVdec->rOther.i4MvTable = dwGetHi(ucEsId,2);
        prVdec->rOther.i4CBPTable = dwGetHi(ucEsId,3);
        prVdec->rOther.i42MVBPTable = dwGetHi(ucEsId,2);

        if ((prVdec->rOther.i4X9MVMode == MIXED_MV)
                || (prVdec->rPicLayer.u1PicType == BVOP))
        {
            prVdec->rOther.i44MVBPTable = dwGetHi(ucEsId,2);
        }

        if (prVdec->rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant(ucEsId);
        }

        if (prVdec->rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(ucEsId,1);
            if (fgTransTypeMB)
            {
                prVdec->rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(ucEsId,2);
                prVdec->rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                prVdec->rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            prVdec->rOther.fgMBXformSwitching = FALSE;
        }

        //End==== decodeVOPHead_WMV3() ===================================

        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1); // Coding set index = 0, 1, or 2.
        if (prVdec->rPicLayer.u4DCTACInterTableIndx == 1)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx
        = prVdec->rPicLayer.u4DCTACInterTableIndx;

        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);
        //=================================================================
    } // PVOP, BVOP

    //  if((prVdec->rPicLayer.u1PicType == IVOP) || (prVdec->rPicLayer.u1PicType == PVOP))
    //  {
    //    UpdateDCStepSize(ucEsId,prVdec->rPicLayer.i4StepSize);
    //  }
    return (TRUE);
}

UINT32 decodeVOPHeadFieldPicture(UCHAR ucEsId)
{
    UINT32 iCode;
    BOOL fgPSPresent;
    UINT32 i, dwNumberOfPanScanWindows;
    //BOOL fgUVProgressiveSubsampling;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    iCode = dwGetHi(ucEsId,3);

    prVdec->rPicLayer.u1FirstFieldType = s_vopFirstFieldType[iCode];
    prVdec->rPicLayer.u1SecondFieldType = s_vopSecondFieldType[iCode];

    if (prVdec->rPicLayer.u1PicType != SKIPFRAME) // SKIPFRAME does not contain temporal reference
    {
        if (prVdec->rSeqLayer.fgTemporalFrmCntr)
            prVdec->rPicLayer.i4TemporalRef = dwGetHi(ucEsId,8);
    }

    if (prVdec->rSeqLayer.fgBroadcastFlags)
    {
        if (prVdec->rSeqLayer.fgInterlacedSource)
        {
            prVdec->rOther.u4GetVal = dwGetHi(ucEsId,2);
            prVdec->rPicLayer.fgTopFieldFirst = (prVdec->rOther.u4GetVal & 0x2) >> 1;
            prVdec->rPicLayer.fgRepeatFirstField = prVdec->rOther.u4GetVal & 0x1;
        }
        else
        {
            prVdec->rPicLayer.u1RepeatFrameCount = dwGetHi(ucEsId,2);
        }
    }

    if (prVdec->rEntryPoint.fgPanScanPresent)
    {
        fgPSPresent = dwGetHi(ucEsId,1);
        if (fgPSPresent)
        {
            if (prVdec->rSeqLayer.fgInterlacedSource)
            {
                if (prVdec->rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(2 + prVdec->rPicLayer.fgRepeatFirstField);
                else
                    dwNumberOfPanScanWindows = 2;
            }
            else
            {
                if (prVdec->rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(1 + prVdec->rPicLayer.u1RepeatFrameCount);
                else
                    dwNumberOfPanScanWindows = 1;
            }
            for (i = 0; i < dwNumberOfPanScanWindows; i++)
            {
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanHorizOffset
                = dwGetHi(ucEsId,18);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanVertOffset
                = dwGetHi(ucEsId,18);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanWidth
                = dwGetHi(ucEsId,14);
                prVdec->rPicLayer.sWindowInfo[i].dwPanScanHeight
                = dwGetHi(ucEsId,14);
            }
        } // fgPSPresent
    }

    prVdec->rPicLayer.i4RndCtrl = dwGetHi(ucEsId,1);
    /*fgUVProgressiveSubsampling =*/
    dwGetHi(ucEsId,1);

    if (prVdec->rEntryPoint.fgRefDistPresent
            && ((prVdec->rPicLayer.u1FirstFieldType == IVOP)
                || (prVdec->rPicLayer.u1FirstFieldType == PVOP)))
    {
        prVdec->rEntryPoint.i4RefFrameDistance = dwGetHi(ucEsId,2);
        if (prVdec->rEntryPoint.i4RefFrameDistance == 3)
        {
            while (dwGetHi(ucEsId,1))
            {
                prVdec->rEntryPoint.i4RefFrameDistance++;
            }
        }
    }

    // Decode B frac
    if ((prVdec->rPicLayer.u1FirstFieldType == BVOP)
            || (prVdec->rPicLayer.u1FirstFieldType == BIVOP))
    {
        UINT32 iShort = 0;
        UINT32 iLong = 0;
        iShort = dwGetHi(ucEsId,3);
        if (iShort == 0x7)
        {
            iLong = dwGetHi(ucEsId,4);
            if (iLong == 0xe) // "Invalid" in VLC
                return INVALID_32;
            if (iLong == 0xf)
                prVdec->rPicLayer.u1PicType = BIVOP;
            else
            {
                prVdec->rPicLayer.i4BNumerator = _iNumLongVLC[iLong];
                prVdec->rPicLayer.i4BDenominator = _iDenLongVLC[iLong];
                prVdec->rPicLayer.i4BFrameReciprocal
                = _iBInverse[prVdec->rPicLayer.i4BDenominator - 1];
                if (prVdec->rSMProfile.i4NumBFrames == 0)
                    prVdec->rSMProfile.i4NumBFrames = 1;
            }
        }
        else
        {
            prVdec->rPicLayer.i4BNumerator = _iNumShortVLC[iShort];
            prVdec->rPicLayer.i4BDenominator = _iDenShortVLC[iShort];
            prVdec->rPicLayer.i4BFrameReciprocal
            = _iBInverse[prVdec->rPicLayer.i4BDenominator - 1];
            if (prVdec->rSMProfile.i4NumBFrames == 0)
                prVdec->rSMProfile.i4NumBFrames = 1;
        }
    }
    return TRUE;
}

UINT32 decodeFieldHeadFieldPicture(UCHAR ucEsId)
{
    //int iPpMethod;
    //int iACPRED;
    //int iOVERFLAGS;
    //int iFORWARDMB;
    BOOL fgTransTypeMB;
    UINT32 iTransTypeFrame;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rPicLayer.i4PicQtIdx = dwGetHi(ucEsId,5);

    if (prVdec->rPicLayer.i4PicQtIdx <= MAXHALFQP)
        prVdec->rPicLayer.fgHalfStep = dwGetHi(ucEsId,1);
    else
        prVdec->rPicLayer.fgHalfStep = FALSE;

    if (prVdec->rEntryPoint.fgExplicitFrameQuantizer)
        prVdec->rPicLayer.fgUse3QPDZQuantizer = dwGetHi(ucEsId,1);

    if (!prVdec->rEntryPoint.fgExplicitQuantizer)
    {
        if (prVdec->rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = TRUE;
            prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;
        }
        else
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = FALSE;
            prVdec->rPicLayer.i4StepSize
            = _iStepRemap[prVdec->rPicLayer.i4PicQtIdx];
        }
    }
    else
        // Explicit quantizer
        prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;

    prVdec->rPicLayer.prDQuantParam
    = prVdec->rPicLayer.fgUse3QPDZQuantizer ? prVdec->rPicLayer.rDQuantParam3QPDeadzone
      : prVdec->rPicLayer.rDQuantParam5QPDeadzone;

    prVdec->rPicLayer.i4Overlap = 0;
    if (prVdec->rEntryPoint.fgSequenceOverlap
            && (prVdec->rPicLayer.u1PicType != BVOP))
    {
        if (prVdec->rPicLayer.i4StepSize >= 9)
            prVdec->rPicLayer.i4Overlap = 1;
        else if (prVdec->rPicLayer.u1PicType == IVOP
                 || prVdec->rPicLayer.u1PicType == BIVOP)
            prVdec->rPicLayer.i4Overlap = 7; // last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (prVdec->rSeqLayer.fgPostProcInfoPresent)
    {
        //spork
        /*iPpMethod =*/
        dwGetHi(ucEsId,2);
    }

    vSetNewMode(ucEsId);

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        //ming modify@2006/4/12
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        //ACPRED, Bitplane
        if (cbWMVFWBP)
        {
            //spork
            /*iACPRED =*/
            iDecodeBitPlane(ucEsId,5); // "ACPRED", bp_num = 5
        }
        else
        {
            fgHWDecBP(ucEsId,5,TRUE);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif

        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        if (prVdec->rPicLayer.i4Overlap & 2)
        {
            if (0 == dwGetHi(ucEsId,1))
                prVdec->rPicLayer.i4Overlap = 0;
            else if (0 == dwGetHi(ucEsId,1))
                prVdec->rPicLayer.i4Overlap = 1;
            else
            {
#ifdef TEST
                _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
                //ming modify@2006/4/12
                //OVERFLAGS, Bitplane
                if (cbWMVFWBP)
                {
                    //spork
                    /*iOVERFLAGS =*/
                    iDecodeBitPlane(ucEsId,6); // "OVERFLAGS", bp_num = 6
                }
                else
                {
                    fgHWDecBP(ucEsId,6,TRUE);
                }
#ifdef TEST
                _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
                prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
            }
        }

        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACInterTableIndx)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACIntraTableIndx)
        {
            prVdec->rPicLayer.u4DCTACIntraTableIndx += dwGetHi(ucEsId,1);
        }

        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);

        if (prVdec->rEntryPoint.i4DQuantCodingOn != 0)
            vDecodeVOPDQuant(ucEsId);
        else
            vSetDefaultDQuantSetting(ucEsId);

        if (prVdec->rPicLayer.u1PicType == IVOP)
        {
            prVdec->rOther.fgLuminanceWarp = FALSE;
            prVdec->rOther.fgLuminanceWarpTop
            = prVdec->rOther.fgLuminanceWarpBottom = FALSE;
        }

    }// End of IVOP, BIVOP
    else if ((prVdec->rPicLayer.u1PicType == PVOP)
             || (prVdec->rPicLayer.u1PicType == BVOP))
    {
        BOOL fgUseMostRecentFieldForRef;

        //NUMREF
        if (prVdec->rPicLayer.u1PicType != BVOP)
            prVdec->rPicLayer.fgTwoRefPictures = dwGetHi(ucEsId,1);
        else
            prVdec->rPicLayer.fgTwoRefPictures = TRUE; // For BVOP, prVdec->rPicLayer.fgTwoRefPictures is always TRUE;

        if (prVdec->rPicLayer.fgTwoRefPictures)
        {
            prVdec->rPicLayer.fgUseSameFieldForRef = TRUE;
            prVdec->rPicLayer.fgUseOppFieldForRef = TRUE;
        }
        else
        {
            fgUseMostRecentFieldForRef = !dwGetHi(ucEsId,1);
            if (fgUseMostRecentFieldForRef)
            {
                prVdec->rPicLayer.fgUseSameFieldForRef = FALSE;
                prVdec->rPicLayer.fgUseOppFieldForRef = TRUE;
            }
            else
            {
                prVdec->rPicLayer.fgUseSameFieldForRef = TRUE;
                prVdec->rPicLayer.fgUseOppFieldForRef = FALSE;
            }
        }

        if (prVdec->rEntryPoint.fgExtendedMvMode)
        {
            // MVRANGE0 = 0, MVRANGE1 = 1,  MVRANGE2 = 2, MVRANGE3 = 3
            prVdec->rEntryPoint.i4MVRangeIndex = dwGetHi(ucEsId,1); // 0: MVRANGE0;
            if (prVdec->rEntryPoint.i4MVRangeIndex)
            {
                prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1); // 1: MVRANGE1
                if (prVdec->rEntryPoint.i4MVRangeIndex == 2)
                {
                    prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1); // 2: MVRANGE2,  3: MVRANGE3
                }
            }
        }

        if (prVdec->rEntryPoint.fgExtendedDeltaMvMode)
        {
            prVdec->rEntryPoint.i4DeltaMVRangeIndex = dwGetHi(ucEsId,1);
            if (prVdec->rEntryPoint.i4DeltaMVRangeIndex)
            {
                prVdec->rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(ucEsId,1);
                if (prVdec->rEntryPoint.i4DeltaMVRangeIndex == 2)
                {
                    prVdec->rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(ucEsId,1);
                }
            }
            prVdec->rEntryPoint.i4ExtendedDMVX
            = prVdec->rEntryPoint.i4DeltaMVRangeIndex & 1;
            prVdec->rEntryPoint.i4ExtendedDMVY
            = (prVdec->rEntryPoint.i4DeltaMVRangeIndex & 2) >> 1;
        }

        prVdec->rOther.fgLuminanceWarp = FALSE;
        prVdec->rOther.fgLuminanceWarpTop
        = prVdec->rOther.fgLuminanceWarpBottom = FALSE;

        //MVMODE
        prVdec->rOther.i4X9MVMode = iGetIFMvMode(ucEsId,
                                            prVdec->rPicLayer.i4StepSize, FALSE);
        if (prVdec->rOther.i4X9MVMode == INTENSITY_COMPENSATION) //PVOP only.
        {
            prVdec->rOther.fgLuminanceWarp = TRUE;
            prVdec->rOther.i4X9MVMode = iGetIFMvMode(ucEsId,
                                                prVdec->rPicLayer.i4StepSize, TRUE);
            if (dwGetHi(ucEsId,1))
            {
                // Both reference field remapped
                prVdec->rOther.fgLuminanceWarpTop
                = prVdec->rOther.fgLuminanceWarpBottom = TRUE;
                prVdec->rOther.i4LumScaleTop = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumShiftTop = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumScaleBottom = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumShiftBottom = dwGetHi(ucEsId,6);
            }
            else if (dwGetHi(ucEsId,1))
            {
                // Bottom reference field remapped
                prVdec->rOther.fgLuminanceWarpBottom = TRUE;
                prVdec->rOther.i4LumScaleBottom = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumShiftBottom = dwGetHi(ucEsId,6);
            }
            else
            {
                // Top reference field remapped
                prVdec->rOther.fgLuminanceWarpTop = TRUE;
                prVdec->rOther.i4LumScaleTop = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumShiftTop = dwGetHi(ucEsId,6);
            }
        }

        if (prVdec->rPicLayer.u1PicType == BVOP)
        {
            //ming modify@2006/4/12
            //FORWARDMB
            if (cbWMVFWBP)
            {
                /*iFORWARDMB =*/
                iDecodeBitPlane(ucEsId,7); //"FORWARDMB", bp_num = 7
            }
            else
            {
                fgHWDecBP(ucEsId,7,TRUE);
            }
            prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
        }

        prVdec->rSMProfile.fgCODFlagOn = TRUE;
        // read MV and CBP codetable indices
        prVdec->rOther.i4MBModeTable = dwGetHi(ucEsId,3);
        prVdec->rOther.i4MvTable
        = (prVdec->rPicLayer.fgTwoRefPictures) ? dwGetHi(ucEsId,3)
          : dwGetHi(ucEsId,2);
        prVdec->rOther.i4CBPTable = dwGetHi(ucEsId,3);
        if (prVdec->rOther.i4X9MVMode == MIXED_MV)
        {
            prVdec->rOther.i44MVBPTable = dwGetHi(ucEsId,2);
        }

        if (prVdec->rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant(ucEsId);
        }

        if (prVdec->rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(ucEsId,1);
            if (fgTransTypeMB)
            {
                prVdec->rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(ucEsId,2);
                prVdec->rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                prVdec->rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            prVdec->rOther.fgMBXformSwitching = FALSE;
        }

        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1); // Coding set index = 0, 1, or 2.
        if (prVdec->rPicLayer.u4DCTACInterTableIndx == 1)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx
        = prVdec->rPicLayer.u4DCTACInterTableIndx;

        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);
    } // PVOP, BVOP

    //  if((prVdec->rPicLayer.u1PicType == PVOP) || (prVdec->rPicLayer.u1PicType == BVOP))
    {
        UpdateDCStepSize(ucEsId,prVdec->rPicLayer.i4StepSize);
    }

    return (TRUE);
}

void SetupFieldPictureMVScaling(UCHAR ucEsId, UINT32 iRefFrameDistance)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (iRefFrameDistance > 3)
        iRefFrameDistance = 3;

    if (prVdec->rPicLayer.i4CurrentTemporalField == 0)
    {
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        prVdec->rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iFarFieldScale1;
        prVdec->rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iFarFieldScale2;
        prVdec->rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iNearFieldScale;
    }
    else
    {
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        prVdec->rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iFarFieldScale1;
        prVdec->rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iFarFieldScale2;
        prVdec->rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iNearFieldScale;
    }
}

void SetupForwardBFieldPictureMVScaling(UCHAR ucEsId,UINT32 iRefFrameDistance)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (iRefFrameDistance > 3)
        iRefFrameDistance = 3;
    if (prVdec->rPicLayer.i4CurrentTemporalField == 0)
    {
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        prVdec->rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale1;
        prVdec->rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale2;
        prVdec->rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iNearFieldScale;
    }
    else
    {
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        prVdec->rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        prVdec->rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale1;
        prVdec->rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale2;
        prVdec->rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iNearFieldScale;
    }
}

void SetupBackwardBFieldPictureMVScaling(UCHAR ucEsId, UINT32 iRefFrameDistance)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (iRefFrameDistance > 3)
        iRefFrameDistance = 3;
    if (prVdec->rPicLayer.i4CurrentTemporalField == 0)
    {
        prVdec->rPicLayer.i4MaxZone1ScaledFarBackMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        prVdec->rPicLayer.i4MaxZone1ScaledFarBackMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarBackMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarBackMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        prVdec->rPicLayer.i4FarFieldScaleBack1
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale1;
        prVdec->rPicLayer.i4FarFieldScaleBack2
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale2;
        prVdec->rPicLayer.i4NearFieldScaleBack
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iNearFieldScale;
    }
    else
    {
        prVdec->rPicLayer.i4MaxZone1ScaledFarBackMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        prVdec->rPicLayer.i4MaxZone1ScaledFarBackMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarBackMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        prVdec->rPicLayer.i4Zone1OffsetScaledFarBackMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        prVdec->rPicLayer.i4FarFieldScaleBack1
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale1;
        prVdec->rPicLayer.i4FarFieldScaleBack2
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale2;
        prVdec->rPicLayer.i4NearFieldScaleBack
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iNearFieldScale;
    }
}

void decodePFieldMode(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    BOOL fgHalfPelMode = ((prVdec->rOther.i4X9MVMode == ALL_1MV_HALFPEL)
                          || (prVdec->rOther.i4X9MVMode == ALL_1MV_HALFPEL_BILINEAR));

    // Set MV state for B field direct mode
    if (prVdec->rPicLayer.i4CurrentField == 0)
        prVdec->rPicLayer.fgBackRefTopFieldHalfPelMode = fgHalfPelMode;
    else
        prVdec->rPicLayer.fgBackRefBottomFieldHalfPelMode = fgHalfPelMode;
}

void decodeBFieldMode(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (prVdec->rPicLayer.i4CurrentField == 0)
        prVdec->rPicLayer.fgBackRefUsedHalfPel
        = prVdec->rPicLayer.fgBackRefTopFieldHalfPelMode;
    else
        prVdec->rPicLayer.fgBackRefUsedHalfPel
        = prVdec->rPicLayer.fgBackRefBottomFieldHalfPelMode;
}


static UINT32 _u4WMVGetRealBitStream(UCHAR ucEsId, UCHAR ucShiftBit)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    UINT32 u4RetVal;

    u4RetVal = dwGetBitStream(ucEsId, 0);
    
    //prVdec->rOther.u4BitCount += ucShiftBit;  will be done in dwGetBitStream
    if (prVdec->rOther.fgCounting)
    {
        prVdec->rOther.i4PicHdrBits += ucShiftBit;
    }
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId, ucShiftBit);

    return(u4RetVal >> (32 - ucShiftBit));
}

#define ucBPMask  prVdec->rOther.u1BPMask
int iDecodeBitPlane(UCHAR ucEsId, int i4BpNum)
{
    BOOL fgInvert, fgDiff = FALSE;
    UCHAR ucBPCodingMode, ucSymbol;  //, ucMpvId, ucBsId;
    INT32 i, j;
    UINT32 u4Tmp, u4Datain, u4Count = 0;
    UINT64 u8HyTmp;
    UINT32 *u4WrAddr;
    UINT64 *u8HyBp;  //u8HyBp[2*MAX_MBY];    
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
#if 0
    UCHAR ucBPMask[MAX_MBY][MAX_MBX];
#else
    //UINT8 **ucBPMask;

    //ucBPMask = prVdec->rOther.u1BPMask;
    u8HyBp = prVdec->rOther.hyBP;
#endif
        
    //ucMpvId = prVDecEsInfo->rVDecNormInfo.ucVldId;
    //ucBsId = prVDecEsInfo->rVDecNormInfo.ucBsId;
  
    u4Datain = dwGetBitStream(ucEsId, 0);
    u4Tmp = (u4Datain >> 27) & 0xF;
  
    if (u4Tmp == 0)
    {
        switch (i4BpNum)
        {
        case 0: //SKIP
            prVdec->rOther.u4BPRawFlag |= 0x1;
            break;
        case 1: //4MV
            prVdec->rOther.u4BPRawFlag |= 0x2;
            break;
        case 3: //DIRECT
            prVdec->rOther.u4BPRawFlag |= 0x4;
            break;
        case 4: //FIELD
            prVdec->rOther.u4BPRawFlag |= 0x8;
            break;
        case 5: //ACPRED
            prVdec->rOther.u4BPRawFlag |= 0x10;
            break;
        case 6: //OVERLAP
            prVdec->rOther.u4BPRawFlag |= 0x20;
            break;
        case 7: //FORWARD
            prVdec->rOther.u4BPRawFlag |= 0x40;
            break;
        default:
            prVdec->rOther.u4BPRawFlag = 0;
            break;
        }
    }
  
    // before_bp
    prVdec->rOther.i4PicHdrBits += 5;
    prVdec->rOther.fgCounting = FALSE;
  
    fgInvert = ((u4Datain >> (32 - (u4Count + 1))) & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
    u4Count++;
    ucBPCodingMode = ((u4Datain >> (32 - (u4Count + 2))) & 0x03);//((dwGetHi(0) >> (dwCount)) & (0x03));// VCL
    u4Count += 2;
    // reset bitplane mask table.
    for (i = 0; i < MAX_MBY; i++)
    {
        for (j = 0; j < MAX_MBX; j++)
        {
            ucBPMask[i][j] = 0;
        }
    }

    for (i = 0; i < 2*MAX_MBY; i++)
    {
        u8HyBp[i] = 0;
    }
  
    switch (ucBPCodingMode)
    {
    case 2: // Norm-2
        ucBPCodingMode = NORM2;
        u4Datain = dwGetBitStream(ucEsId, u4Count);
        vDecBPNorm2Diff2(ucEsId);
        break;
    case 3: // Norm-6
        ucBPCodingMode = NORM6;
        u4Datain = dwGetBitStream(ucEsId, u4Count);
        if (!fgDecodeNorm6Diff6(ucEsId))
        {
            prVdec->rOther.fgCounting = TRUE;
            return INVALID_32;
        }    
        break;
    case 1:
        ucBPCodingMode = ((u4Datain >> (32-(u4Count +1))) & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
        u4Count++;
        u4Datain = dwGetBitStream(ucEsId, u4Count);
        // reset bitplane mask table.
        if (ucBPCodingMode)
        {   // Colskip
            ucBPCodingMode = COLSKIP;
            for (j = 0; j < prVdec->rSeqHdrR.u4NumMBX; j++)
            {
                if (_u4WMVGetRealBitStream(ucEsId, 1)) // this column has nonzero values
                {
                    for (i = 0; i < prVdec->rSeqHdrR.u4NumMBY; i++)
                    {
                        ucSymbol = _u4WMVGetRealBitStream(ucEsId, 1);
                        ucBPMask[i][j] = ucSymbol;
                    }
                }
                else // this column are all zeros.
                {
                    for (i = 0; i < prVdec->rSeqHdrR.u4NumMBY; i++)
                    {
                        ucSymbol = 0;
                        ucBPMask[i][j] = 0;
                    }
                }
            }
        }
        else
        {   // Rowskip
            ucBPCodingMode = ROWSKIP;
            for (i = 0; i < prVdec->rSeqHdrR.u4NumMBY; i++)
            {
                if (_u4WMVGetRealBitStream(ucEsId, 1)) // this row has nonzero values
                {
                    for (j = 0; j < prVdec->rSeqHdrR.u4NumMBX; j++)
                    {
                        ucSymbol = _u4WMVGetRealBitStream(ucEsId, 1);
                        ucBPMask[i][j] = ucSymbol;
                    }
                }
                else
                {
                    for (j = 0; j < prVdec->rSeqHdrR.u4NumMBX; j++)
                    {
                        ucSymbol = 0;
                        ucBPMask[i][j] = 0;
                    }
                }
            }
        }
        break;
    default:
        ucBPCodingMode = ((u4Datain >> (32-(u4Count +1))) & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
        u4Count++;
        if (ucBPCodingMode)
        {   // Diff-2
            ucBPCodingMode = DIFF2;
            u4Datain = dwGetBitStream(ucEsId, u4Count);
            fgDiff = TRUE;
            vDecBPNorm2Diff2(ucEsId);
        }
        else
        {
            ucBPCodingMode = ((u4Datain >> (32-(u4Count +1))) & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
            u4Count++;
            if (ucBPCodingMode)
            {   // Diff-6
                ucBPCodingMode = DIFF6;
                u4Datain = dwGetBitStream(ucEsId, u4Count);
                fgDiff = TRUE;
                if (!fgDecodeNorm6Diff6(ucEsId))
                {
                    prVdec->rOther.fgCounting = TRUE;
                    return INVALID_32;
                }
            } 
            else
            {   // Raw
                u4Datain = dwGetBitStream(ucEsId, u4Count);
                ucBPCodingMode = RAW;
                prVdec->rOther.fgCounting = TRUE;
                return(ucBPCodingMode);
            }
        }
        break;
    }
  
    if (fgDiff)
    {
        for (i = 0; i < prVdec->rSeqHdrR.u4NumMBY; i++) 
        {
            for (j = 0; j < prVdec->rSeqHdrR.u4NumMBX; j++) 
            {
                INT32  i4Pred = 0;
                if (i + j == 0)
                {
                     i4Pred = fgInvert;
                }
                else if (i == 0)
                {
                    if (j >= 1)  // to shut up stupid Klocwork
                    {
                        i4Pred = ucBPMask[i][j-1];//(pmb1 - 1)->m_bSkip;
                    }
                }
                else if (j == 0)
                {
                    if (i >= 1)  // to shut up stupid Klocwork
                    {
                        i4Pred = ucBPMask[i-1][j];//(pmb1 - u4NumMBX)->m_bSkip;
                    }
                }
                else
                {
                    if (i >= 1 && j >= 1)  // to shut up stupid Klocwork
                    {
                        if (ucBPMask[i][j-1] == ucBPMask[i-1][j])
                        {
                            i4Pred = ucBPMask[i][j-1];
                        }
                        else
                        {
                            i4Pred = fgInvert;
                        }
                    }
                }
                ucBPMask[i][j] ^= i4Pred;
            }
        }
    }
    else
    {
        if (fgInvert)
        {
            for (i = 0; i < prVdec->rSeqHdrR.u4NumMBY; i++)
            {
                 for (j = 0; j < prVdec->rSeqHdrR.u4NumMBX; j++) 
                 {
                     ucBPMask[i][j] ^= 1;
                 }
            }
        }
    }   
    
    if (ucBPCodingMode != RAW)
    {
        //Select Bp buffer
        if (prVdec->rOther.i4CodecVersion != WMVA)
        {
            UINT8 *pu1Addr;
            switch (prVdec->rPicLayer.u1PicType)
            {
            case IVOP:
            case BIVOP:
                ASSERT(0);
                return ucBPCodingMode;
                
            case PVOP:
                ASSERT(0 <= i4BpNum && i4BpNum <= 1);
                pu1Addr = i4BpNum == 0 ? prVdec->rDram.pu1Bp_1 : prVdec->rDram.pu1Bp_2;
                break;
                
            case BVOP:
                ASSERT(i4BpNum == 0 || i4BpNum == 3);
                pu1Addr = i4BpNum == 0 ? prVdec->rDram.pu1Bp_1 : prVdec->rDram.pu1Bp_3;
                break;
            
            default:
                ASSERT(0);
                return ucBPCodingMode;
            }
            u4WrAddr = (UINT32 *)VIRTUAL((UINT32)pu1Addr);
        }
        else
        {
            UINT8 *pu1Addr;
            switch (prVdec->rPicLayer.u1PicType)
            {
            case IVOP:
            case BIVOP:
                if (prVdec->rPicLayer.u1FrameCodingMode != INTERLACEFRAME)
                {
                    ASSERT(5 <= i4BpNum && i4BpNum <= 6);
                    pu1Addr = i4BpNum == 5 ? prVdec->rDram.pu1Bp_2 : prVdec->rDram.pu1Bp_3;
                }
                else
                {
                    if (i4BpNum == 4)
                        pu1Addr = prVdec->rDram.pu1Bp_1;
                    else if (i4BpNum == 5)
                        pu1Addr = prVdec->rDram.pu1Bp_2;
                    else if (i4BpNum == 6)
                        pu1Addr = prVdec->rDram.pu1Bp_3;
                    else
                    {
                        ASSERT(0);
                        return ucBPCodingMode;
                    }
                }
                break;
            
            case PVOP:
                if (prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE)
                {
                    ASSERT(0 <= i4BpNum && i4BpNum <= 1);
                    pu1Addr = i4BpNum == 0 ? prVdec->rDram.pu1Bp_1 : prVdec->rDram.pu1Bp_2;
                }
                else if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFRAME)
                {
                    ASSERT(0 == i4BpNum);
                    pu1Addr = prVdec->rDram.pu1Bp_1;
                }
                else if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    // impossible
                    ASSERT(0);
                    return ucBPCodingMode;
                }
                else
                {
                    ASSERT(0);
                    return ucBPCodingMode;
                }
                break;
            
            case BVOP:
                if (prVdec->rPicLayer.u1FrameCodingMode == PROGRESSIVE)
                {
                    ASSERT(i4BpNum == 0 || i4BpNum == 3);
                    pu1Addr = i4BpNum == 0 ? prVdec->rDram.pu1Bp_1 : prVdec->rDram.pu1Bp_3;
                }
                else if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFRAME)
                {
                    ASSERT(i4BpNum == 0 || i4BpNum == 3);
                    pu1Addr = i4BpNum == 0 ? prVdec->rDram.pu1Bp_1 : prVdec->rDram.pu1Bp_3;
                }
                else if (prVdec->rPicLayer.u1FrameCodingMode == INTERLACEFIELD)
                {
                    ASSERT(i4BpNum == 7);
                    pu1Addr = prVdec->rDram.pu1Bp_3;
                }
                else
                {
                    ASSERT(0);
                    return ucBPCodingMode;
                }
                break;
            
            default:
                ASSERT(0);
                return ucBPCodingMode;
            }
            u4WrAddr = (UINT32 *)VIRTUAL((UINT32)pu1Addr); 
        }
        // reconstruct bitplane to u8HyBp[]
        for (i = 0; i < prVdec->rSeqHdrR.u4NumMBY; i++) // for each MB-row
        {
            for (j = 0; j < prVdec->rSeqHdrR.u4NumMBX; j++)
            {
                if (j < 64)
                {
                    if (ucBPMask[i][j] != 0)
                    {
                        u8HyTmp = (UINT64)ucBPMask[i][j];
                        u8HyBp[2*i] += u8HyTmp << ((BP_MB_BITS-1) - j);
                    }
                }
                else
                {
                    if (ucBPMask[i][j] != 0)
                    {
                        u8HyTmp = (UINT64)ucBPMask[i][j];
                        u8HyBp[2*i+1] += u8HyTmp << ((BP_MB_BITS-1) - (j-64));
                    }
                }
            }
            *u4WrAddr = (u8HyBp[2*i+1] & 0xFFFFFFFF);
            u4WrAddr ++;
            *u4WrAddr = ((u8HyBp[2*i+1] >> 32) & 0xFFFFFFFF);
            u4WrAddr ++;
            *u4WrAddr = (u8HyBp[2*i] & 0xFFFFFFFF);
            u4WrAddr ++;
            *u4WrAddr = ((u8HyBp[2*i] >> 32) & 0xFFFFFFFF);
            u4WrAddr ++;
        }
        #if defined(CC_MT5395)
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)prVdec->rDram.pu1Bp_1), (UINT32)prVdec->rDram.pu1Bp_4 - (UINT32)prVdec->rDram.pu1Bp_1);
        #elif defined(IS_WMV_POST_MT5368)
        HalFlushDCacheMultipleLine(VIRTUAL((UINT32)prVdec->rDram.pu1Bp_1), (UINT32)prVdec->rDram.pu1Bp_4 - (UINT32)prVdec->rDram.pu1Bp_1);
        #else
        HalFlushInvalidateDCache();
        #endif
    }
  
    prVdec->rOther.fgCounting = TRUE;   

    return (ucBPCodingMode);
}

void vDecBPNorm2Diff2(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    //UCHAR **ucBPMask = (UCHAR **)prVdec->rOther.u1BPMask;
    INT32 i;
    UINT32 u4NumMB = prVdec->rSeqHdrR.u4NumMBX * prVdec->rSeqHdrR.u4NumMBY;
    UCHAR ucSymbol2n, ucSymbol2n_1;
    UINT32 u4CurrMBX,u4CurrMBY;
  
    if (u4NumMB & 0x1)
    {
        ucBPMask[0][0] = _u4WMVGetRealBitStream(ucEsId, 1); // If rowMB กั colMB is odd, the first symbol is encoded raw.
    }

    u4CurrMBX =  (u4NumMB & 0x1);
    u4CurrMBY = 0;
    for (i = (u4NumMB & 0x1); i < u4NumMB; i += 2)
    {
        if (_u4WMVGetRealBitStream(ucEsId, 1))
        {
            if (_u4WMVGetRealBitStream(ucEsId, 1))
            {   // Codeword = 11b
                ucSymbol2n = 1;
                ucSymbol2n_1 = 1;
            }
            else
            {
                if (_u4WMVGetRealBitStream(ucEsId, 1))
                {   // Codeword = 101b
                    ucSymbol2n = 0;
                    ucSymbol2n_1 = 1;
                }
                else
                {   // Codeword = 100b
                    ucSymbol2n = 1;
                    ucSymbol2n_1 = 0;
                }
            }
        }
        else
        {   // Codeword = 0b
            ucSymbol2n = 0;
            ucSymbol2n_1 = 0;
        }
        
        //u4CurrMBX = i % prWmvDrvInfo->rSPS.u4NumMBX;
        //u4CurrMBY = i / prWmvDrvInfo->rSPS.u4NumMBX;
        ucBPMask[u4CurrMBY][u4CurrMBX] = ucSymbol2n;
        u4CurrMBX++;
        if (u4CurrMBX == prVdec->rSeqHdrR.u4NumMBX)
        {
           u4CurrMBX = 0;
           u4CurrMBY++;
        }
        //u4CurrMBX = (i+1) % prWmvDrvInfo->rSPS.u4NumMBX;
        //u4CurrMBY = (i+1) / prWmvDrvInfo->rSPS.u4NumMBX;
        ucBPMask[u4CurrMBY][u4CurrMBX] = ucSymbol2n_1;

        u4CurrMBX++;
        if (u4CurrMBX == prVdec->rSeqHdrR.u4NumMBX)
        {
           u4CurrMBX = 0;
           u4CurrMBY++;
        }
    }
}

BOOL fgDecodeNorm6Diff6(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    //UCHAR **ucBPMask = (UCHAR **)prVdec->rOther.u1BPMask;
    INT32  i, j;
    UCHAR ucSymbol[6];
    // u42On4Off are consecutive indices which have 2 1s and 4 0s in binary rep
    INT32  u42On4Off[15] = {  3,  5,  6,  9, 10, 12, 17, 18,
                          20, 24, 33, 34, 36, 40, 48     };
    // u4Count is the number of 1s
    INT32   u4Count[32] = { 0, 1, 1, 2, 1, 2, 2, 3,
                         1, 2, 2, 3, 2, 3, 3, 4,
                         1, 2, 2, 3, 2, 3, 3, 4,
                         2, 3, 3, 4, 3, 4, 4, 5 };
    INT32  i4Start = 0, j4Start = 0;
    INT32 i6;
    UINT32 u4NumMBX = prVdec->rSeqHdrR.u4NumMBX;
    UINT32 u4NumMBY = prVdec->rSeqHdrR.u4NumMBY;
  
    if (((u4NumMBY % 3) == 0) && (u4NumMBX % 3))
    {   // vertically oriented 2 x 3 tiles
        j4Start = u4NumMBX & 1;
        for (i = 0; i < (int)u4NumMBY; i += 3)
        {
            for (j = j4Start; j < (int)u4NumMBX; j += 2)
            {
                if (fgDecNorm6Diff6Tile(ucEsId, u42On4Off, u4Count, &i6) == FALSE)
                {
                    return FALSE;
                }
                ucSymbol[0] = i6 & 1;
                ucBPMask[i][j] = ucSymbol[0];
                i6 >>= 1;
                ucSymbol[1] = i6 & 1;
                ucBPMask[i][j+1] = ucSymbol[1];
                i6 >>= 1;
                ucSymbol[2] = i6 & 1;
                ucBPMask[i+1][j] = ucSymbol[2];
                i6 >>= 1;
                ucSymbol[3] = i6 & 1;
                ucBPMask[i+1][j+1] = ucSymbol[3];
                i6 >>= 1;
                ucSymbol[4] = i6 & 1;
                ucBPMask[i+2][j] = ucSymbol[4];
                i6 >>= 1;
                ucSymbol[5] = i6 & 1;
                ucBPMask[i+2][j+1] = ucSymbol[5];
            }
        }
    }
    else
    {   // horizontally oriented 3 x 2 tiles
        i4Start = u4NumMBY & 1;
        j4Start = u4NumMBX % 3;
        for (i = i4Start; i < u4NumMBY; i += 2)
        {
            for (j = j4Start; j < u4NumMBX; j += 3)
            {
                if (fgDecNorm6Diff6Tile(ucEsId, u42On4Off, u4Count, &i6) == FALSE)
                {
                    return FALSE;
                }
                ucSymbol[0] = i6 & 1;
                ucBPMask[i][j] = ucSymbol[0];
                i6 >>= 1;
                ucSymbol[1] = i6 & 1;
                ucBPMask[i][j+1] = ucSymbol[1];
                i6 >>= 1;
                ucSymbol[2] = i6 & 1;
                ucBPMask[i][j+2] = ucSymbol[2];
                i6 >>= 1;
                ucSymbol[3] = i6 & 1;
                ucBPMask[i+1][j] = ucSymbol[3];
                i6 >>= 1;
                ucSymbol[4] = i6 & 1;
                ucBPMask[i+1][j+1] = ucSymbol[4];
                i6 >>= 1;
                ucSymbol[5] = i6 & 1;
                ucBPMask[i+1][j+2] = ucSymbol[5];
            }
        }
    }
  
    // left column(s)
    for (j = 0; j < j4Start; j++)
    {
        if (_u4WMVGetRealBitStream(ucEsId, 1))
        {
            for (i = 0; i < u4NumMBY; i++)
            {
                ucBPMask[i][j] = _u4WMVGetRealBitStream(ucEsId, 1);
            }
        }
        else
        {
            for (i = 0; i < u4NumMBY; i++)
            {
                ucSymbol[0] = 0;
                ucBPMask[i][j] = 0;
            }
        }
    }
  
    // top row
    if (i4Start)
    {
        if (_u4WMVGetRealBitStream(ucEsId, 1))
        {
            for (j = j4Start; j < u4NumMBX; j++)
            {
                ucBPMask[i][j] = _u4WMVGetRealBitStream(ucEsId, 1);
            }
        }
        else
        {
            for (j = j4Start; j < u4NumMBX; j++)
            {
                ucSymbol[0] = 0;
                ucBPMask[i][j] = 0;
            }
        }
    }

    return TRUE;
}

BOOL fgDecNorm6Diff6Tile(UCHAR ucEsId, INT32 *u42On4Off, INT32 *u4Count, INT32 *p4i6)
{
    BOOL fgInvert = FALSE;
    INT32  i4Code;
    INT32 i6 = *p4i6;

    // 4 stage decoding - read 1, 3, 1, 3 bits respectively
StartOfLocalFn6:
    // length 1 code
    if (_u4WMVGetRealBitStream(ucEsId, 1)) 
    {
        i6 = 0;
        goto EndOfLocalFn6;
    }
    i4Code = _u4WMVGetRealBitStream(ucEsId, 3);
    if (i4Code > 1) 
    {
        // length 4 codes
        i6 = 1 << (i4Code - 2);
        goto EndOfLocalFn6;
    }
    if (i4Code == 1) 
    {
        // length 5 codes
        if (_u4WMVGetRealBitStream(ucEsId, 1)) 
        {
            // complementary codes - send to start
            if (fgInvert)  // only one inversion possible
            {
                *p4i6 = i6 ;
                return FALSE;
            }
            fgInvert = TRUE;
            goto StartOfLocalFn6;
        }
        else 
        {
            // 3 on 3 off
            i6 = _u4WMVGetRealBitStream(ucEsId, 5);
            i4Code = u4Count[i6];
            if (i4Code == 2) 
            {
                i6 |= 32;
            }
            else if (i4Code == 3);  // do nothing
            else 
            {
                *p4i6 = i6 ;
                return FALSE; // invalid symbol
            }
            goto EndOfLocalFn6;
        }
    }
    // length 8 codes
    i4Code = _u4WMVGetRealBitStream(ucEsId, 4);
    if (i4Code == 0xff)  // non-existent codeword
    {
        *p4i6 = i6 ;
        return FALSE;
    }
    i6 = u42On4Off[i4Code];

EndOfLocalFn6:
    if (fgInvert)
    {
        i6 ^= 0x3f;
    }
    *p4i6 = i6 ;
    
    return(TRUE);
}

void vDecodeVOPDQuant(UCHAR ucEsId)
{
    UINT8 bPanningCode;
    UINT8 bPicQtDiff;
    UINT8 bAltPicQt;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    if (prVdec->rEntryPoint.i4DQuantCodingOn == 2)
    {
        prVdec->rOther.fgDQuantOn = TRUE;
        prVdec->rOther.i4Panning = 0xf; // four edges
        bPicQtDiff = dwGetHi(ucEsId,3);
        if (bPicQtDiff == 7)
        {
            bAltPicQt = dwGetHi(ucEsId,5);
        }
        else
        {
            bAltPicQt = prVdec->rPicLayer.i4StepSize + bPicQtDiff
                        + (UINT8)1;
        }
        prVdec->rOther.u1DQuantBiLevelStepSize = bAltPicQt;
    }
    else
    {
        prVdec->rOther.fgDQuantBiLevel = FALSE;
        prVdec->rOther.i4Panning = 0;
        prVdec->rOther.fgDQuantOn = dwGetHi(ucEsId,1);
        prVdec->rOther.u1DiffQtProfile = 0;
        if (prVdec->rOther.fgDQuantOn)
        {
            prVdec->rOther.u1DiffQtProfile = dwGetHi(ucEsId,2);
            if (prVdec->rOther.u1DiffQtProfile == ALL_4EDGES)
            {
                prVdec->rOther.i4Panning = 0xf;
            }
            else if (prVdec->rOther.u1DiffQtProfile == SINGLE_EDGES)
            {
                bPanningCode = dwGetHi(ucEsId,2);
                prVdec->rOther.i4Panning = 0x1 << bPanningCode;
            }
            else if (prVdec->rOther.u1DiffQtProfile == DOUBLE_EDGES)
            {
                bPanningCode = dwGetHi(ucEsId,2);
                if (bPanningCode != 3)
                    prVdec->rOther.i4Panning = 0x3 << bPanningCode;
                else
                    prVdec->rOther.i4Panning = 0x9;
            }
            else if (prVdec->rOther.u1DiffQtProfile == ALL_MBS)
            {
                prVdec->rOther.fgDQuantBiLevel = dwGetHi(ucEsId,1);
            }

            if (prVdec->rOther.i4Panning
                    || prVdec->rOther.fgDQuantBiLevel) //i.e. if((prVdec->rOther.u1DiffQtProfile != ALL_MBS) || (prVdec->rOther.fgDQuantBiLevel))
            {
                bPicQtDiff = dwGetHi(ucEsId,3);
                if (bPicQtDiff == 7)
                {
                    bAltPicQt = dwGetHi(ucEsId,5);
                }
                else
                {
                    bAltPicQt = prVdec->rPicLayer.i4StepSize + bPicQtDiff
                                + 1;
                }
                prVdec->rOther.u1DQuantBiLevelStepSize = bAltPicQt;
            }
        } // prVdec->rOther.fgDQuantOn
    }
}

void vSetDefaultDQuantSetting(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.fgDQuantOn = FALSE;
    prVdec->rOther.fgDQuantBiLevel = FALSE;
    prVdec->rOther.i4Panning = 0;
    prVdec->rOther.u1DQuantBiLevelStepSize
    = prVdec->rPicLayer.i4StepSize;
    //Printf("vSetDefaultDQuantSetting, prVdec->rOther.u1DQuantBiLevelStepSize : %d\n",prVdec->rOther.u1DQuantBiLevelStepSize);
    //  prVdec->rOther.u1DiffQtProfile = 0;
    return;
}

UINT32 iGetPMvMode(UCHAR ucEsId, UINT32 iPQuant, BOOL fgRepeat)
{
    UINT32 bMvMode;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    if (iPQuant > 12)
    { // P Picture Low Rate
        if (dwGetHi(ucEsId,1))
            bMvMode = ALL_1MV_HALFPEL_BILINEAR; // 1b
        else if (dwGetHi(ucEsId,1))
            bMvMode = ALL_1MV; // 01b
        else if (dwGetHi(ucEsId,1))
            bMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (fgRepeat || (!dwGetHi(ucEsId,1)))
                bMvMode = MIXED_MV; // 0000b
            else
                bMvMode = INTENSITY_COMPENSATION; // 0001b
        }
    }
    else
    { // P Picture High rate
        if (dwGetHi(ucEsId,1))
            bMvMode = ALL_1MV; // 1b
        else if (dwGetHi(ucEsId,1))
            bMvMode = MIXED_MV; // 01b
        else if (dwGetHi(ucEsId,1))
            bMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (fgRepeat || (!dwGetHi(ucEsId,1)))
                bMvMode = ALL_1MV_HALFPEL_BILINEAR; // 0000b
            else
                bMvMode = INTENSITY_COMPENSATION; // 0001b
        }
    }
    return bMvMode;
}

UINT32 iGetIFMvMode(UCHAR ucEsId, UINT32 iPQuant, BOOL fgRepeat) //for Interlace Field
{
    int iMvMode;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    if (iPQuant > 12)
    { // P/B Field Picture Low Rate
        if (dwGetHi(ucEsId,1))
            iMvMode = ALL_1MV_HALFPEL_BILINEAR; // 1b
        else if (dwGetHi(ucEsId,1))
            iMvMode = ALL_1MV; // 01b
        else if (dwGetHi(ucEsId,1))
            iMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (prVdec->rPicLayer.u1PicType == BVOP)
            {
                iMvMode = MIXED_MV;
            }
            else
            {
                if (fgRepeat || (!dwGetHi(ucEsId,1)))
                    iMvMode = MIXED_MV; // 0000b
                else
                    iMvMode = INTENSITY_COMPENSATION; // 0001b
            }
        }
    }
    else
    { // P/B Field Picture High rate
        if (dwGetHi(ucEsId,1))
            iMvMode = ALL_1MV; // 1b
        else if (dwGetHi(ucEsId,1))
            iMvMode = MIXED_MV; // 01b
        else if (dwGetHi(ucEsId,1))
            iMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (prVdec->rPicLayer.u1PicType == BVOP)
            {
                iMvMode = ALL_1MV_HALFPEL_BILINEAR;
            }
            else
            {
                if (fgRepeat || (!dwGetHi(ucEsId,1)))
                    iMvMode = ALL_1MV_HALFPEL_BILINEAR; // 0000b
                else
                    iMvMode = INTENSITY_COMPENSATION; // 0001b
            }
        }
    }
    return iMvMode;
}

void SetupMultiResParams(UCHAR ucEsId)
{
    // Calculate half res params
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    int iHalfWidthSrc = prVdec->rSeqHdrR.u4PicWidthSrc >> 1;
    int iHalfHeightSrc = prVdec->rSeqHdrR.u4PicHeightSrc >> 1;
    int iHalfWidthDec = ((prVdec->rSeqHdrR.u4PicWidthSrc >> 1) + 15) & ~15; //Half & MB aligned
    int iHalfHeightDec = ((prVdec->rSeqHdrR.u4PicHeightSrc >> 1) + 15)
                         & ~15; //Half & MB aligned
    MULTIRES_PARAMS* pMultiResParams;

    if (prVdec->rOther.i4CodecVersion == WMVA)
    {
        //For PROGRESSIVE
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[PROGRESSIVE];

        pMultiResParams->iFrmWidthSrc = prVdec->rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = prVdec->rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (prVdec->rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = (prVdec->rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        //For INTERLACEFIELD
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[INTERLACEFIELD];

        pMultiResParams->iFrmWidthSrc = prVdec->rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = prVdec->rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (prVdec->rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = ((prVdec->rSeqHdrR.u4PicHeightSrc >>1) + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        //For INTERLACEFRAME
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[INTERLACEFRAME];

        pMultiResParams->iFrmWidthSrc = prVdec->rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = prVdec->rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (prVdec->rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = (prVdec->rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;
    }
    else //For WMV3
    {
        // Save parameters for full res (index = 0)
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[0];

        pMultiResParams->iFrmWidthSrc = prVdec->rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = prVdec->rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (prVdec->rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = (prVdec->rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        // Save parameters for half-horizontal, full-vertical res (index = 1)
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[1];

        pMultiResParams->iFrmWidthSrc = iHalfWidthSrc;
        pMultiResParams->iFrmHeightSrc = prVdec->rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = iHalfWidthDec;
        pMultiResParams->iHeightDec
        = (prVdec->rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        // Save parameters for full-horizontal, half-vertical res (index = 2)
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[2];

        pMultiResParams->iFrmWidthSrc = prVdec->rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = iHalfHeightSrc;
        pMultiResParams->iWidthDec = (prVdec->rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec = iHalfHeightDec;
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        // Save parameters for half-horizontal, half-vertical res (index = 3)
        pMultiResParams = &prVdec->rSeqHdrR.rMultiResParams[3];

        pMultiResParams->iFrmWidthSrc = iHalfWidthSrc;
        pMultiResParams->iFrmHeightSrc = iHalfHeightSrc;
        pMultiResParams->iWidthDec = iHalfWidthDec;
        pMultiResParams->iHeightDec = iHalfHeightDec;
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;
    }
}

void ComputeDQuantDecParam(UCHAR ucEsId)
{
    int iStepSize;
    int iDCStepSize;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    for (iStepSize = 1; iStepSize < 63; iStepSize++)
    {
        DQuantDecParam *pDQ =
            &prVdec->rPicLayer.rDQuantParam3QPDeadzone[iStepSize];

        int iDoubleStepSize = (iStepSize + 1);

        pDQ->iDoubleStepSize = iDoubleStepSize;
        pDQ->iStepMinusStepIsEven = 0;
        pDQ->i2DoublePlusStepSize = iDoubleStepSize;
        //pDQ->i2DoublePlusStepSizeNeg = -1 * pDQ->i2DoublePlusStepSize;

        iDCStepSize = (iStepSize + 1) >> 1;
        if (iDCStepSize <= 4)
        {
            pDQ->iDCStepSize = 8;
            if (prVdec->rEntryPoint.fgNewDCQuant && iDCStepSize <= 2)
            {
                pDQ->iDCStepSize = 2 * iDCStepSize;
            }
        }
        else
        {
            pDQ->iDCStepSize = iDCStepSize / 2 + 6;
        }
    }

    for (iStepSize = 1; iStepSize < 63; iStepSize++)
    {
        DQuantDecParam *pDQ =
            &prVdec->rPicLayer.rDQuantParam5QPDeadzone [iStepSize];
        int iDoubleStepSize;

        iDoubleStepSize = (iStepSize + 1);

        pDQ->iDoubleStepSize = iDoubleStepSize;

        if (prVdec->rOther.i4CodecVersion >= WMV3)
        {
            pDQ->iStepMinusStepIsEven = (iStepSize + 1) >> 1;

            pDQ->i2DoublePlusStepSize = iDoubleStepSize
                                        + pDQ->iStepMinusStepIsEven;
        }
        else
        {
            int iStepSize2 = (iStepSize + 1) >> 1;
            pDQ->iStepMinusStepIsEven = iStepSize2 - ((iStepSize2 & 1) == 0);
            pDQ->i2DoublePlusStepSize = iDoubleStepSize
                                        + pDQ->iStepMinusStepIsEven;
        }

        //pDQ->i2DoublePlusStepSizeNeg = -1 * pDQ->i2DoublePlusStepSize;

        iDCStepSize = (iStepSize + 1) >> 1;
        if (iDCStepSize <= 4)
        {
            pDQ->iDCStepSize = 8;
            if (prVdec->rEntryPoint.fgNewDCQuant && iDCStepSize <= 2)
                pDQ->iDCStepSize = 2 * iDCStepSize;

        }
        else
        {
            pDQ->iDCStepSize = iDCStepSize / 2 + 6;
        }
    }
}

UINT32 getbits(UCHAR ucEsId, UINT32 dwNum) // the range of dwNum is 1 to 32.
{
#ifdef MAX_GET_32_BITS
    UINT32 dwValue;

    dwValue = dwGetBitStream(ucEsId,0); // Just get 32 bits from shifter
    if (dwNum> 0)
    {
        dwValue >>= (32 - dwNum); // Get highest dwNum bits
    }
    dwGetBitStream(ucEsId,dwNum); // Flush dwNum bits

    return dwValue;

#else

    UINT32 dwValue1, dwValue2;
    UINT32 dwRemainBits;

    if (dwNum <= 16)
    {
        dwValue1 = get16(ucEsId,dwNum);
    }
    else
    {
        dwValue1 = get16(ucEsId,16);
        dwRemainBits = dwNum - 16;
        dwValue2 = get16(ucEsId,dwRemainBits);
        dwValue1 = (dwValue1 << dwRemainBits) + dwValue2;
    }

    return dwValue1;
#endif
}

UINT32 get16(UCHAR ucEsId, UINT32 dwNum) // dwNum must be greater than 0
{
    UINT32 dwValue;

    dwValue = dwGetBitStream(ucEsId,0); // Just get 32 bits from shifter
    if (dwNum > 0)
    {
        dwValue >>= (32 - dwNum); // Get highest dwNum bits
    }
    dwGetBitStream(ucEsId,dwNum); // Flush dwNum bits

    return dwValue;
}

UINT32 dwGetHi(UCHAR ucEsId, UINT32 dwN)
{
    UINT32 dwRetVal;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    dwRetVal = prVdec->rOther.u4Datain >> (32 - dwN);
    //Printf("GetHi prVdec->rOther.u4Datain : %d %08x\n",dwN,prVdec->rOther.u4Datain);
    if (prVdec->rOther.fgCounting)
        prVdec->rOther.i4PicHdrBits += (int)dwN;

    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,dwN);

    return dwRetVal;
}

//WMV3 : Simple & Main Profile, decode sequence header
UINT32 decodeVOLHead_WMV3(UCHAR ucEsId)
{
    //BOOL fgRndCtrlOn;
    BOOL fg16bitXform;
    //BOOL fgStartCode;
    //BOOL fgRTMContent;
    //BOOL fgBetaContent;
    //int iBetaRTMMismatchIndex;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rSeqLayer.i4Profile = (prVdec->rOther.i4SeqHdrData1
                                       & 0xc0000000) >> 30;
    if (prVdec->rSeqLayer.i4Profile == 0)
        prVdec->rSeqLayer.i4WMV3Profile = WMV3_SIMPLE_PROFILE;
    else if (prVdec->rSeqLayer.i4Profile == 1)
        prVdec->rSeqLayer.i4WMV3Profile = WMV3_MAIN_PROFILE;
    else if (prVdec->rSeqLayer.i4Profile == 2)
        prVdec->rSeqLayer.i4WMV3Profile = WMV3_PC_PROFILE;

    prVdec->rSMProfile.fgYUV411 = (prVdec->rOther.i4SeqHdrData1
                                       & 0x20000000) >> 29;
    prVdec->rSMProfile.fgSpriteMode = (prVdec->rOther.i4SeqHdrData1
                                           & 0x10000000) >> 28;
    prVdec->rSeqLayer.i4FrameRate = (prVdec->rOther.i4SeqHdrData1
                                         & 0x0e000000) >> 25;
    prVdec->rSeqLayer.i4FrameRate = 4 * prVdec->rSeqLayer.i4FrameRate
                                        + 2;
    prVdec->rSeqLayer.i4BitRate = (prVdec->rOther.i4SeqHdrData1
                                       & 0x01f00000) >> 20;
    prVdec->rSeqLayer.i4BitRate = 64 * prVdec->rSeqLayer.i4BitRate + 32;

    //fgRndCtrlOn = TRUE;
    prVdec->rEntryPoint.fgLoopFilter = (prVdec->rOther.i4SeqHdrData1
                                            & 0x00080000) >> 19;
    prVdec->rSMProfile.fgXintra8Switch = (prVdec->rOther.i4SeqHdrData1
            & 0x00040000) >> 18;
    prVdec->rSMProfile.fgMultiresEnabled
    = (prVdec->rOther.i4SeqHdrData1 & 0x00020000) >> 17;
    fg16bitXform = (prVdec->rOther.i4SeqHdrData1 & 0x00010000) >> 16;
    prVdec->rEntryPoint.fgUVHpelBilinear
    = (prVdec->rOther.i4SeqHdrData1 & 0x00008000) >> 15;
    prVdec->rEntryPoint.fgExtendedMvMode
    = (prVdec->rOther.i4SeqHdrData1 & 0x00004000) >> 14;
    prVdec->rEntryPoint.i4DQuantCodingOn
    = (prVdec->rOther.i4SeqHdrData1 & 0x00003000) >> 12;
    prVdec->rEntryPoint.fgXformSwitch = (prVdec->rOther.i4SeqHdrData1
                                            & 0x00000800) >> 11;
    prVdec->rSMProfile.fgDCTTable_MB_ENABLED
    = (prVdec->rOther.i4SeqHdrData1 & 0x00000400) >> 10;
    prVdec->rEntryPoint.fgSequenceOverlap
    = (prVdec->rOther.i4SeqHdrData1 & 0x00000200) >> 9;
    //fgStartCode = (prVdec->rOther.i4SeqHdrData1 & 0x00000100) >> 8;
    prVdec->rSMProfile.fgPreProcRange = (prVdec->rOther.i4SeqHdrData1
                                            & 0x00000080) >> 7;
    prVdec->rSMProfile.i4NumBFrames = (prVdec->rOther.i4SeqHdrData1
                                           & 0x00000070) >> 4;
    prVdec->rEntryPoint.fgExplicitSeqQuantizer
    = (prVdec->rOther.i4SeqHdrData1 & 0x00000008) >> 3;
    if (prVdec->rEntryPoint.fgExplicitSeqQuantizer)
        prVdec->rPicLayer.fgUse3QPDZQuantizer
        = (prVdec->rOther.i4SeqHdrData1 & 0x00000004) >> 2;
    else
        prVdec->rEntryPoint.fgExplicitFrameQuantizer
        = (prVdec->rOther.i4SeqHdrData1 & 0x00000004) >> 2;
    prVdec->rEntryPoint.fgExplicitQuantizer
    = prVdec->rEntryPoint.fgExplicitSeqQuantizer
      || prVdec->rEntryPoint.fgExplicitFrameQuantizer;

    prVdec->rSeqLayer.fgSeqFrameInterpolation
    = (prVdec->rOther.i4SeqHdrData1 & 0x00000002) >> 1;
    prVdec->rPicLayer.i4BFrameReciprocal
    = _iBInverse[prVdec->rSMProfile.i4NumBFrames];

    if ((prVdec->rSMProfile.fgYUV411)
            || (prVdec->rSMProfile.fgSpriteMode))
    {
        // 411 is depreciated and untested.  Use WMV9 SP and MP only porting kit for 411 reference if you need it.
        return WMV_UnSupportedCompressedFormat;
    }

    if ((prVdec->rOther.i4SeqHdrData1 & 0x00000001) == 1)
    {
        // RTM content
        //fgRTMContent = TRUE;
        //fgBetaContent = FALSE;
        //iBetaRTMMismatchIndex = 0;
    }

    if (fg16bitXform)
    {
        prVdec->rSMProfile.fgRotatedIdct = TRUE;
    }

    // If prVdec->rOther.u4HeaderLen > 4, it goes into this condition.
    if (prVdec->rOther.i4SeqHdrDataLen > 4)
    {
        //BOOL bVCMInfoPresent;

        prVdec->rSeqLayer.fgPostProcInfoPresent
        = (prVdec->rOther.i4SeqHdrData2 & 0x80) >> 7;
        // VCM bit (34th bit) is reserved for VCM contents if VOL size > 4 bytes.
        //bVCMInfoPresent = (prVdec->rOther.i4SeqHdrData2 & 0x40) >> 6;
    }

    return WMV_Succeeded;
}

//WMV8 only: decode sequence header
UINT32 decodeVOLHead_WMV12(UCHAR ucEsId)
{
    //BOOL fgRndCtrlOn;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    ASSERT((prVdec->rOther.i4CodecVersion == WMV2)
           || (prVdec->rOther.i4CodecVersion == WMV1));

    ASSERT(prVdec->rOther.i4SeqHdrData2 == 0);

    prVdec->rSeqLayer.i4FrameRate = (prVdec->rOther.i4SeqHdrData1
                                         & 0xf8000000) >> 27;
    prVdec->rSeqLayer.i4BitRate = (prVdec->rOther.i4SeqHdrData1
                                       & 0x07ff0000) >> 16;
    //fgRndCtrlOn = TRUE;
    prVdec->rSMProfile.fgMixedPel = (prVdec->rOther.i4SeqHdrData1
                                         & 0x00008000) >> 15;
    prVdec->rEntryPoint.fgLoopFilter = (prVdec->rOther.i4SeqHdrData1
                                            & 0x00004000) >> 14;
    prVdec->rEntryPoint.fgXformSwitch = (prVdec->rOther.i4SeqHdrData1
                                            & 0x00002000) >> 13;
    prVdec->rSMProfile.fgXintra8Switch = (prVdec->rOther.i4SeqHdrData1
            & 0x00001000) >> 12;

    prVdec->rSMProfile.fgFrmHybridMVOn = (prVdec->rOther.i4SeqHdrData1
            & 0x00000800) >> 11;

    // DCTTABLE S/W at MB level for WMV2.
    prVdec->rSMProfile.fgDCTTable_MB_ENABLED
    = (prVdec->rOther.i4SeqHdrData1 & 0x00000400) >> 10;
    prVdec->rSMProfile.i4SliceCode = (prVdec->rOther.i4SeqHdrData1
                                          & 0x00000380) >> 7;

    return WMV_Succeeded;
}

int ReconRangeState_new = 0;
//WMV3 : Simple & Main Profile, decode picture header
UINT32 WMVideoDecDecodeFrameHead(UCHAR ucEsId)
{
    //BOOL fgInterpolateCurrentFrame;
    //int iFrmCntMod4 = 0;
    //int iMVTYPEMB;
    //int iDIRECTMB;
    //int iSKIPMB;
    BOOL fgTransTypeMB;
    UINT32 iTransTypeFrame;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    //UINT32 dwTmp;
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
    LOG(6, "WMVideoDecDecodeFrameHead datain : %x\n",prVdec->rOther.u4Datain);
    if (prVdec->rSeqLayer.fgSeqFrameInterpolation)
    {
        /*fgInterpolateCurrentFrame =*/
        dwGetHi(ucEsId,1);
    }
    /*iFrmCntMod4 =*/dwGetHi(ucEsId,2);

    if (prVdec->rSMProfile.fgPreProcRange)
    {
        prVdec->rEntryPoint.i4RangeState = dwGetHi(ucEsId,1);
    }

    prVdec->rPicLayer.u1PrevPicType = prVdec->rPicLayer.u1PicType;
    // Picture coding type
    if (1 == dwGetHi(ucEsId,1))
        prVdec->rPicLayer.u1PicType = PVOP;
    else
    {
        if (prVdec->rSMProfile.i4NumBFrames == 0)
        {
            prVdec->rPicLayer.u1PicType = IVOP;
        }
        else
        {
            if (dwGetHi(ucEsId,1) == 1)
                prVdec->rPicLayer.u1PicType = IVOP;
            else
                prVdec->rPicLayer.u1PicType = BVOP;
        }
    }

    // Decode B frac
    if ((prVdec->rPicLayer.u1PicType == BVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        int iShort = 0, iLong = 0;
        iShort = dwGetHi(ucEsId,3);
        if (iShort == 0x7)
        {
            iLong = dwGetHi(ucEsId,4);
            if (iLong == 0xe) // "Invalid" in VLC
                return INVALID_32;

            if (iLong == 0xf)
                prVdec->rPicLayer.u1PicType = BIVOP;
            else
            {
                prVdec->rPicLayer.i4BNumerator = _iNumLongVLC[iLong];
                prVdec->rPicLayer.i4BDenominator = _iDenLongVLC[iLong];
                prVdec->rPicLayer.i4BFrameReciprocal
                = _iBInverse[prVdec->rPicLayer.i4BDenominator - 1];
                if (prVdec->rSMProfile.i4NumBFrames == 0)
                    prVdec->rSMProfile.i4NumBFrames = 1;
            }
        }
        else
        {
            prVdec->rPicLayer.i4BNumerator = _iNumShortVLC[iShort];
            prVdec->rPicLayer.i4BDenominator = _iDenShortVLC[iShort];
            prVdec->rPicLayer.i4BFrameReciprocal
            = _iBInverse[prVdec->rPicLayer.i4BDenominator - 1];
            if (prVdec->rSMProfile.i4NumBFrames == 0)
                prVdec->rSMProfile.i4NumBFrames = 1;
        }
    }

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        int iBufferFullPercent = dwGetHi(ucEsId,7);
        UNUSED(iBufferFullPercent);
    }

    prVdec->rPicLayer.i4PicQtIdx = dwGetHi(ucEsId,5);

    if (prVdec->rPicLayer.i4PicQtIdx <= MAXHALFQP)
        prVdec->rPicLayer.fgHalfStep = dwGetHi(ucEsId,1);
    else
        prVdec->rPicLayer.fgHalfStep = FALSE;

    if (prVdec->rEntryPoint.fgExplicitFrameQuantizer)
        prVdec->rPicLayer.fgUse3QPDZQuantizer = dwGetHi(ucEsId,1);

    if (!prVdec->rEntryPoint.fgExplicitQuantizer)
    {
        if (prVdec->rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = TRUE;
            prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;
        }
        else
        {
            prVdec->rPicLayer.fgUse3QPDZQuantizer = FALSE;
            prVdec->rPicLayer.i4StepSize
            = _iStepRemap[prVdec->rPicLayer.i4PicQtIdx];
        }
    }
    else
        prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;

    prVdec->rPicLayer.prDQuantParam
    = prVdec->rPicLayer.fgUse3QPDZQuantizer ? prVdec->rPicLayer.rDQuantParam3QPDeadzone
      : prVdec->rPicLayer.rDQuantParam5QPDeadzone;

    prVdec->rPicLayer.i4Overlap = 0;
    if (prVdec->rEntryPoint.fgSequenceOverlap
            && (prVdec->rPicLayer.u1PicType != BVOP))
    {
        if (prVdec->rPicLayer.i4StepSize >= 9)
            prVdec->rPicLayer.i4Overlap = 1;
        else if ((prVdec->rSeqLayer.i4WMV3Profile == WMV3_ADVANCED_PROFILE)
                 && ((prVdec->rPicLayer.u1PicType == IVOP)
                     || (prVdec->rPicLayer.u1PicType == BIVOP)))
            prVdec->rPicLayer.i4Overlap = 7;
        //prVdec->rPicLayer.i4Overlap last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (prVdec->rEntryPoint.fgExtendedMvMode)
    {
        prVdec->rEntryPoint.i4MVRangeIndex = dwGetHi(ucEsId,1);
        if (prVdec->rEntryPoint.i4MVRangeIndex)
            prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1);
        if (prVdec->rEntryPoint.i4MVRangeIndex == 2)
            prVdec->rEntryPoint.i4MVRangeIndex += dwGetHi(ucEsId,1);
    }

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == PVOP))
    {
        if (prVdec->rSMProfile.fgMultiresEnabled)
            prVdec->rSMProfile.i4ResIndex = dwGetHi(ucEsId,2);
    }

    //  if(prVdec->rSMProfile.i4ResIndex != 0)
    {
        prVdec->rSeqHdrR.u4PicWidthDec
        = prVdec->rSeqHdrR.rMultiResParams[prVdec->rSMProfile.i4ResIndex].iWidthDec;
        prVdec->rSeqHdrR.u4PicHeightDec
        = prVdec->rSeqHdrR.rMultiResParams[prVdec->rSMProfile.i4ResIndex].iHeightDec;
        //Printf("src decode resolution : %dx%d\n",prVdec->rSeqHdrR.u4PicWidthDec,prVdec->rSeqHdrR.u4PicHeightDec);
        //Printf("index : %d",prVdec->rSMProfile.i4ResIndex);
        prVdec->rSeqHdrR.u4NumMBX
        = prVdec->rSeqHdrR.rMultiResParams[prVdec->rSMProfile.i4ResIndex].dwNumMBX;
        prVdec->rSeqHdrR.u4NumMBY
        = prVdec->rSeqHdrR.rMultiResParams[prVdec->rSMProfile.i4ResIndex].dwNumMBY;
        prVdec->rSeqHdrR.u4PicWidthCmp
        = prVdec->rSeqHdrR.rMultiResParams[prVdec->rSMProfile.i4ResIndex].iFrmWidthSrc;
        prVdec->rSeqHdrR.u4PicHeightCmp
        = prVdec->rSeqHdrR.rMultiResParams[prVdec->rSMProfile.i4ResIndex].iFrmHeightSrc;
    }

    vSetNewMode(ucEsId);

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACInterTableIndx)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx = dwGetHi(ucEsId,1);
        if (prVdec->rPicLayer.u4DCTACIntraTableIndx)
        {
            prVdec->rPicLayer.u4DCTACIntraTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);

        vSetDefaultDQuantSetting(ucEsId);

        prVdec->rPicLayer.i4RndCtrl = 1;

    } //End of IVOP, BIVOP
    else // PVOP, BVOP
    {
        //Start==== decodeVOPHead_WMV3() ===================================
        prVdec->rOther.fgLuminanceWarp = FALSE;
        prVdec->rOther.fgLuminanceWarpTop
        = prVdec->rOther.fgLuminanceWarpBottom = FALSE;

        if ((prVdec->rPicLayer.u1PicType == BVOP)
                && (!prVdec->rPicLayer.fgFieldMode))
        {
            if (dwGetHi(ucEsId,1))
            {
                prVdec->rOther.i4X9MVMode = ALL_1MV;
            }
            else
            {
                prVdec->rOther.i4X9MVMode = ALL_1MV_HALFPEL_BILINEAR;
            }

#ifdef TEST
            _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif

            //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                //spork
                /*iDIRECTMB = */
                iDecodeBitPlane(ucEsId,3); // "DirectMB", bp_num = 3
            }
            else
            {
                fgHWDecBP(ucEsId,3,TRUE);
            }

#ifdef TEST
            _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif

            prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
        }
        else
        {
            prVdec->rOther.i4X9MVMode = iGetPMvMode(ucEsId,
                                                prVdec->rPicLayer.i4StepSize, FALSE);
            if (prVdec->rOther.i4X9MVMode == INTENSITY_COMPENSATION)
            {
                prVdec->rOther.fgLuminanceWarp = TRUE;
                prVdec->rOther.i4X9MVMode = iGetPMvMode(ucEsId,
                                                    prVdec->rPicLayer.i4StepSize, TRUE);
                prVdec->rOther.i4LumScale = dwGetHi(ucEsId,6);
                prVdec->rOther.i4LumShift = dwGetHi(ucEsId,6);
            }

            if (prVdec->rOther.i4X9MVMode == MIXED_MV)
            {
#ifdef TEST
                _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
                //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iMVTYPEMB =*/
                    iDecodeBitPlane(ucEsId,1); //"4mv", bp_num = 1
                }
                else
                {
                    fgHWDecBP(ucEsId,1,TRUE);
                }
#ifdef TEST
                _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
                prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
            }
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iSKIPMB =*/
            iDecodeBitPlane(ucEsId,0); // "SkipMB", bp_num = 0
        }
        else
        {
            fgHWDecBP(ucEsId,0,TRUE);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
        prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

        prVdec->rSMProfile.fgCODFlagOn = TRUE;

        prVdec->rOther.i4MvTable = dwGetHi(ucEsId,2);
        prVdec->rOther.i4CBPTable = dwGetHi(ucEsId,2); // Only in P & B pictures.
        //Printf("-------------mv table : %x , cbp table : %x\nprVdec->rEntryPoint.i4DQuantCodingOn : %x\n",prVdec->rOther.i4MvTable,prVdec->rOther.i4CBPTable,prVdec->rEntryPoint.i4DQuantCodingOn);
        if (prVdec->rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant(ucEsId);
        }

        if (prVdec->rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(ucEsId,1);
            if (fgTransTypeMB)
            {
                prVdec->rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(ucEsId,2);
                prVdec->rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                prVdec->rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            prVdec->rOther.fgMBXformSwitching = FALSE;
        }
        //End==== decodeVOPHead_WMV3() ===================================

        prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1); // Coding set index = 0, 1, or 2.
        if (prVdec->rPicLayer.u4DCTACInterTableIndx == 1)
        {
            prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
        }
        prVdec->rPicLayer.u4DCTACIntraTableIndx
        = prVdec->rPicLayer.u4DCTACInterTableIndx;

        prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);
        //=================================================================
        if (prVdec->rPicLayer.u1PicType == PVOP)
        {
            if (!prVdec->rOther.u4repeatframe)
                prVdec->rPicLayer.i4RndCtrl ^= 1;
        }
    }

    if ((prVdec->rPicLayer.u1PicType == IVOP)
            || (prVdec->rPicLayer.u1PicType == PVOP)
            || (prVdec->rPicLayer.u1PicType == BIVOP))
    {
        UpdateDCStepSize(ucEsId,prVdec->rPicLayer.i4StepSize);
    }

    if (prVdec->rPicLayer.u1PicType == PVOP)
    {
        AdjustReconRange(ucEsId);
    }
    else if (prVdec->rPicLayer.u1PicType == IVOP)
    {
        ReconRangeState_new = prVdec->rEntryPoint.i4RangeState;
    }

    return WMV_Succeeded;
}

//WMV7 & WMV8: decode picture header
UINT32 WMV78DecodePicture(UCHAR ucEsId)
{
    int iNumBitsFrameType;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    iNumBitsFrameType = (prVdec->rOther.i4CodecVersion == WMV2) ? 1 : 2;
    prVdec->rPicLayer.u1PrevPicType = prVdec->rPicLayer.u1PicType;
    prVdec->rPicLayer.u1PicType = dwGetHi(ucEsId,iNumBitsFrameType);

    if ((prVdec->rOther.i4CodecVersion >= WMV2)
            && (prVdec->rPicLayer.u1PicType == IVOP))
    {
        int iBufferFullPercent = dwGetHi(ucEsId,7);
        UNUSED(iBufferFullPercent);
    }

    prVdec->rPicLayer.i4PicQtIdx = dwGetHi(ucEsId,5);
    prVdec->rPicLayer.prDQuantParam
    = prVdec->rPicLayer.rDQuantParam5QPDeadzone;
    prVdec->rPicLayer.i4StepSize = prVdec->rPicLayer.i4PicQtIdx;

    if (prVdec->rPicLayer.u1PicType == IVOP)
    {
        if (prVdec->rOther.i4CodecVersion < WMV2) // WMV1 only
        {
            prVdec->rSMProfile.i4SliceCode = dwGetHi(ucEsId,NUMBITS_SLICE_SIZE);
            ASSERT(prVdec->rSMProfile.i4SliceCode > 22);
        }

        WMVideoDecDecodeClipInfo(ucEsId);
        if (prVdec->rSMProfile.fgXintra8)
        {
            return WMV_Succeeded;
        }

        if (prVdec->rOther.i4CodecVersion >= WMV2)
        {
            prVdec->rOther.fgDCPred_IMBInPFrame = FALSE;
        }
        else // WMV1
        {
            prVdec->rSMProfile.fgDCTTable_MB_ENABLED
            = (prVdec->rSeqLayer.i4BitRate > MIN_BITRATE_MB_TABLE);
            prVdec->rOther.fgDCPred_IMBInPFrame
            = ((prVdec->rSeqLayer.i4BitRate
                <= MAX_BITRATE_DCPred_IMBInPFrame)
               && (prVdec->rSeqHdrR.u4PicWidthSrc
                   * prVdec->rSeqHdrR.u4PicHeightSrc < 320
                   * 240));
        }

        if ((!prVdec->rSMProfile.fgXintra8)
                && (prVdec->rOther.i4CodecVersion >= MMIDRATE43))
        {
            if (prVdec->rSMProfile.fgDCTTable_MB_ENABLED)
            {
                prVdec->rPicLayer.fgDCTTable_MB = dwGetHi(ucEsId,1);
            }
            if (!prVdec->rPicLayer.fgDCTTable_MB)
            {
                // DCT Table swtiching, I and P index are coded separately.
                // Can be jointly coded using the following table.
                // IP Index : Code
                // 00       : 00,
                // 11       : 01,
                // 01       : 100,
                // 10       : 101,
                // 02       : 1100,
                // 12       : 1101,
                // 20       : 1110,
                // 21       : 11110
                // 22       : 11111
                prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1);
                if (prVdec->rPicLayer.u4DCTACInterTableIndx)
                {
                    prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
                }
                prVdec->rPicLayer.u4DCTACIntraTableIndx = dwGetHi(ucEsId,1);
                if (prVdec->rPicLayer.u4DCTACIntraTableIndx)
                {
                    prVdec->rPicLayer.u4DCTACIntraTableIndx += dwGetHi(ucEsId,1);
                }
            }
            prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);
        }
        vSetDefaultDQuantSetting(ucEsId);
        prVdec->rPicLayer.i4RndCtrl = 1;
    }
    else //PVOP
    {
        decodeVOPHead_WMV2(ucEsId);

        if (prVdec->rOther.i4CodecVersion >= MMIDRATE43)
        {
            // MMIDRATE43 || WMV1 (2)
            if (prVdec->rSMProfile.fgDCTTable_MB_ENABLED)
            {
                prVdec->rPicLayer.fgDCTTable_MB = dwGetHi(ucEsId,1);
            }

            if (!prVdec->rPicLayer.fgDCTTable_MB)
            {
                prVdec->rPicLayer.u4DCTACInterTableIndx = dwGetHi(ucEsId,1);
                if (prVdec->rPicLayer.u4DCTACInterTableIndx)
                {
                    prVdec->rPicLayer.u4DCTACInterTableIndx += dwGetHi(ucEsId,1);
                }
            }
            prVdec->rPicLayer.u4DCTACIntraTableIndx
            = prVdec->rPicLayer.u4DCTACInterTableIndx;

            prVdec->rPicLayer.fgIntraDCTDCTable = dwGetHi(ucEsId,1);

            if (prVdec->rOther.i4CodecVersion != WMV3)
                prVdec->rOther.i4MvTable = dwGetHi(ucEsId,1);
        }
        if (prVdec->rPicLayer.u1PicType == PVOP)
        {
            if (!prVdec->rOther.u4repeatframe)
                prVdec->rPicLayer.i4RndCtrl ^= 1;
        }
    }

    UpdateDCStepSize(ucEsId,prVdec->rPicLayer.i4StepSize);

    return WMV_Succeeded;
}

UINT32 WMVideoDecDecodeClipInfo(UCHAR ucEsId)
{
    //BOOL fgRndCtrlOn;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    if (prVdec->rOther.i4CodecVersion >= WMV2)
    {
        if (prVdec->rSMProfile.fgXintra8Switch)
            prVdec->rSMProfile.fgXintra8 = dwGetHi(ucEsId,1);
        if (prVdec->rSMProfile.fgXintra8)
        {
#ifdef CC_WMV_EMULATION
            extern BOOL _fgAbtCurItem;
            _fgAbtCurItem = TRUE;
#endif
            LOG(0, "===x8_intra clip @ #%d\n", prVdec->rOther.u4CurrPicNo);
        }
    }
    else // WMV1
    {
        int iFrameRate = dwGetHi(ucEsId,5);
        if (prVdec->rSeqLayer.i4FrameRate == 0) // if the info is available from system (app), use it.
        {
            prVdec->rSeqLayer.i4FrameRate = iFrameRate;
        }
        prVdec->rSeqLayer.i4BitRate = dwGetHi(ucEsId,11);
        if (prVdec->rOther.i4CodecVersion != MMIDRATE42)
        {
            /*fgRndCtrlOn =*/
            dwGetHi(ucEsId,1);
        }
    }

    return WMV_Succeeded;
}

void decodeVOPHead_WMV2(UCHAR ucEsId)
{
    //UINT32 dwTmp;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);

    vSetNewMode(ucEsId);

    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    if (!prVdec->rSMProfile.fgSKIPBIT_CODING)
    {
        prVdec->rSMProfile.fgCODFlagOn = dwGetHi(ucEsId,1);
        prVdec->rSMProfile.i4skip_bit_mode_v87
        = prVdec->rSMProfile.fgCODFlagOn; //0:no_skip_bit, 1:raw_mode
    }
    else
    {
        int iSkipBitCode = dwGetHi(ucEsId,2);
        prVdec->rSMProfile.fgCODFlagOn = TRUE;
        if (iSkipBitCode == 0)
        {
            prVdec->rSMProfile.i4skip_bit_mode_v87 = 0;
            prVdec->rSMProfile.fgCODFlagOn = FALSE;
        }
        else
        {
            prVdec->rSMProfile.i4skip_bit_mode_v87 = 2; //2: read dram
            if (iSkipBitCode == 1)
            {
                prVdec->rSMProfile.i4SkipBitCodingMode = Normal;
                prVdec->rSMProfile.i4wmv8_bp_mode = 1;
            }
            else if (iSkipBitCode == 2)
            {
                prVdec->rSMProfile.i4SkipBitCodingMode = RowPredict;
                prVdec->rSMProfile.i4wmv8_bp_mode = 2;
            }
            else
            {
                prVdec->rSMProfile.i4SkipBitCodingMode = ColPredict;
                prVdec->rSMProfile.i4wmv8_bp_mode = 3;
            }
#ifdef TEST
            _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
            //dwTmp = dwGetBitStream(ucEsId,0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                DecodeSkipBit(ucEsId); //SKIPMB, wmv78 bitplane decode
            }
            else
            {
                fgHWDecBP(ucEsId,0,TRUE); //SKIPMB
            }
#ifdef TEST
            _dwLookAhead = dwGetBitStream(ucEsId,0);
#endif
            prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);
        }
    }

    // NEW_PCBPCY_TABLE
    if (prVdec->rSMProfile.fgNEW_PCBPCY_TABLE)
    {
        if (prVdec->rPicLayer.i4StepSize <= 10)
        {
            if (!dwGetHi(ucEsId,1))
            { //0 High
                prVdec->rSMProfile.i4HufNewPCBPCYDec = HighRate;
            }
            else if (!dwGetHi(ucEsId,1))
            { //10 Low
                prVdec->rSMProfile.i4HufNewPCBPCYDec = LowRate;
            }
            else
            { //11 Mid
                prVdec->rSMProfile.i4HufNewPCBPCYDec = MidRate;
            }
        }
        else if (prVdec->rPicLayer.i4StepSize <= 20)
        {
            if (!dwGetHi(ucEsId,1))
            { //0 Mid
                prVdec->rSMProfile.i4HufNewPCBPCYDec = MidRate;
            }
            else if (!dwGetHi(ucEsId,1))
            { //10 High
                prVdec->rSMProfile.i4HufNewPCBPCYDec = HighRate;
            }
            else
            { //11 Low
                prVdec->rSMProfile.i4HufNewPCBPCYDec = LowRate;
            }
        }
        else
        {
            if (!dwGetHi(ucEsId,1))
            { //0 Low
                prVdec->rSMProfile.i4HufNewPCBPCYDec = LowRate;
            }
            else if (!dwGetHi(ucEsId,1))
            { //10 Mid
                prVdec->rSMProfile.i4HufNewPCBPCYDec = MidRate;
            }
            else
            { //11 High
                prVdec->rSMProfile.i4HufNewPCBPCYDec = HighRate;
            }
        }
        prVdec->rOther.i4CBPTable
        = prVdec->rSMProfile.i4HufNewPCBPCYDec;
    }
    //Robert TODO: prVdec->rOther.i4CBPTable = ???. It should be assigned here !!!

    //_MIXEDPEL_
    if (prVdec->rSMProfile.fgMixedPel)
    {
        prVdec->rPicLayer.fgMvResolution = dwGetHi(ucEsId,1);
    }

    if (prVdec->rEntryPoint.fgXformSwitch)
    {
        if (dwGetHi(ucEsId,1) == 1)
        {
            prVdec->rOther.fgMBXformSwitching = FALSE;
            if (dwGetHi(ucEsId,1) == 0)
                prVdec->rOther.i4FrameXformMode = XFORMMODE_8x8;
            else if (dwGetHi(ucEsId,1) == 0)
                prVdec->rOther.i4FrameXformMode = XFORMMODE_8x4;
            else
                prVdec->rOther.i4FrameXformMode = XFORMMODE_4x8;
        }
        else
        {
            prVdec->rOther.fgMBXformSwitching = TRUE;
        }
    }
}

void DecodeSkipBit(UCHAR ucEsId)
{
    UINT8 bSymbol;
    int i, j;
    UINT64 hyTmp;
    UINT32 dwWrSa;
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rOther.u4Datain = dwGetBitStream(ucEsId,0);

    // reset bitplane mask table.
    for (i = 0; i < MAX_MBY; i++)
    {
        for (j = 0; j < MAX_MBX; j++)
        {
            prVdec->rOther.u1BPMask[i][j] = 0;
        }
        prVdec->rOther.hyBP[i] = 0;
    }

    switch (prVdec->rSMProfile.i4SkipBitCodingMode)
    {
    case Normal:
        for (i = 0; i < (int)prVdec->rSeqHdrR.u4NumMBY; i++)
        {
            for (j = 0; j < (int)prVdec->rSeqHdrR.u4NumMBX; j++)
            {
                bSymbol = dwGetHi(ucEsId,1);
                prVdec->rOther.u1BPMask[i][j] = bSymbol;
            }
        }
        break;
    case RowPredict:
        for (i = 0; i < (int)prVdec->rSeqHdrR.u4NumMBY; i++)
        {
            if (dwGetHi(ucEsId,1))
            { //skip row
                for (j = 0; j < (int)prVdec->rSeqHdrR.u4NumMBX; j++)
                {
                    bSymbol = 1;
                    prVdec->rOther.u1BPMask[i][j] = bSymbol;
                }
            }
            else
            {
                for (j = 0; j < (int)prVdec->rSeqHdrR.u4NumMBX; j++)
                {
                    bSymbol = dwGetHi(ucEsId,1);
                    prVdec->rOther.u1BPMask[i][j] = bSymbol;
                }
            }
        }
        break;
    case ColPredict:
        for (j = 0; j < (int)prVdec->rSeqHdrR.u4NumMBX; j++)
        {
            if (dwGetHi(ucEsId,1))
            { // skip column
                for (i = 0; i < (int)prVdec->rSeqHdrR.u4NumMBY; i++)
                {
                    bSymbol = 1;
                    prVdec->rOther.u1BPMask[i][j] = bSymbol;
                }
            }
            else
            {
                for (i = 0; i < (int)prVdec->rSeqHdrR.u4NumMBY; i++)
                {
                    bSymbol = dwGetHi(ucEsId,1);
                    prVdec->rOther.u1BPMask[i][j] = bSymbol;
                }
            }
        }
        break;
    default:
        ASSERT(FALSE);
        break;
    }

    // reconstruct bitplane to prVdec->rOther.hyBP[]
    for (i = 0; i < (int)prVdec->rSeqHdrR.u4NumMBY; i++) // for each MB-row
    {
        for (j = 0; j < (int)prVdec->rSeqHdrR.u4NumMBX; j++)
        {
            if (prVdec->rOther.u1BPMask[i][j] != 0)
            {
                hyTmp = (UINT64)prVdec->rOther.u1BPMask[i][j];
                prVdec->rOther.hyBP[i] += hyTmp << ((BP_MB_BITS-1) - j);
            }
        }
    }

    dwWrSa = (UINT32)prVdec->rDram.pu1Bp_1;

    for (i = 0; i < (int)MAX_MBY; i++) // for each MB-row
    {

        prVdec->rOther.hyBP[2*i+1] = 0;
        for (j = (int)MAX_MBX/2; j < (int)MAX_MBX; j++)
        {
            if (prVdec->rOther.u1BPMask[i][j] != 0)
            {
                hyTmp = (UINT64)prVdec->rOther.u1BPMask[i][j];
                prVdec->rOther.hyBP[2*i+1] += hyTmp << ((BP_MB_BITS-1) - (j
                                                  - (int)MAX_MBX/2));
            }
        }
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 0)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 8)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 16)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 24)
                                        &0xFF) );
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 32)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 40)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 48)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i+1] >> 56)
                                        &0xFF) );
        dwWrSa++;

        prVdec->rOther.hyBP[2*i] = 0;
        for (j = 0; j < (int)MAX_MBX/2; j++)
        {
            if (prVdec->rOther.u1BPMask[i][j] != 0)
            {
                hyTmp = (UINT64)prVdec->rOther.u1BPMask[i][j];
                prVdec->rOther.hyBP[2*i] += hyTmp << ((BP_MB_BITS-1) - j);
            }
        }
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 0)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 8)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 16)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 24)
                                        &0xFF) );
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 32)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 40)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 48)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((prVdec->rOther.hyBP[2*i] >> 56)
                                        &0xFF) );
        dwWrSa++;
    }
#ifdef CC_WMV_EMULATION
#if defined(CC_MT5395) || defined(IS_WMV_POST_MT5368)
    HalFlushInvalidateDCacheMultipleLine(prVdec->rPesInfo.u4FifoStart, prVdec->rPesInfo.u4FifoEnd - prVdec->rPesInfo.u4FifoStart);
#else
    HalFlushInvalidateDCache();
#endif
#endif
}

// WMV7 & WMV8 End
//int ReconRangeState_new = 0;
void AdjustReconRange(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    prVdec->rEntryPoint.i4ReconRangeState = ReconRangeState_new;
    if (ReconRangeState_new == 0)
    {
        if (prVdec->rEntryPoint.i4RangeState == 1)
        {
            ReconRangeState_new = prVdec->rEntryPoint.i4RangeState;
        }
    }
    else if (ReconRangeState_new == 1)
    {
        if (prVdec->rEntryPoint.i4RangeState == 0)
        {
            ReconRangeState_new = prVdec->rEntryPoint.i4RangeState;
        }
    }
}

void PostAdjustReconRange(UCHAR ucEsId)
{
    VDEC_WMV_INFO_T* prVdec = (VDEC_WMV_INFO_T*)vWMVGetHdlr(ucEsId);
    if (prVdec->rSMProfile.fgPreProcRange)
    {
        if (prVdec->rPicLayer.u1PicType == IVOP
                || prVdec->rPicLayer.u1PicType == BIVOP)
        {
            if (prVdec->rSMProfile.i4NumBFrames == 0)
                prVdec->rEntryPoint.i4ReconRangeState
                = prVdec->rEntryPoint.i4RangeState;
            else if (prVdec->rPicLayer.u1PicType != BVOP)
            {
                AdjustReconRange(ucEsId);
            }
        }
        //    if(prVdec->rEntryPoint.i4RangeState != 0)
        //    {
        //      AdjustDecRange();
        //    }
    }
}
