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

//extern TPicInf rWMVDecInfo.rOther.rPicInfo;
//extern int rWMVDecInfo.rOther.u4repeatframe;

//#define TEST
extern UCHAR _VDecReceiveEs(UCHAR);

#ifdef TEST
UINT32 _dwLookAhead;
#endif

//copy from main.c
void reset_pic_hdr_bits(void)
{
    rWMVDecInfo.rOther.i4PicHdrBits = 0;
    rWMVDecInfo.rOther.fgCounting = TRUE;
}

void before_bp(void)
{
    rWMVDecInfo.rOther.i4PicHdrBits += 5;
    rWMVDecInfo.rOther.fgCounting = FALSE;
}

void after_bp(void)
{
    rWMVDecInfo.rOther.fgCounting = TRUE;
}

inline UINT32 pic_hdr_bitcount(void)
{
    return rWMVDecInfo.rOther.i4PicHdrBits;
}

inline void set_pic_hdr_bits(UINT32 n)
{
    rWMVDecInfo.rOther.i4PicHdrBits = n;
}

//copy from vparser.c
void vInitVParser(void)
{
    rWMVDecInfo.rOther.u4Datain = 0;
    rWMVDecInfo.rOther.u4BitCount = 0;

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

 if(!fgWaitVldFetchOk())
 {
 LOG(0,"Fail to WMV waitVLDFetchOK\n");
 return FALSE;
 }
 WriteREG(RW_VLD_PROC, 0x00800000);

 u4RealAddr = dwVldRPtr(&u4Bits);
 if (dwPtr > u4RealAddr)
 {
 UINT32 i;

 u4RestData = dwPtr -u4RealAddr;
 for (i = 0; i < u4RestData; i++)
 {
 dwGetBitStream(8);
 }
 }
 return TRUE;

 }
 */

BOOL fgVldInitBarrelShifter(UINT32 dwPtr)
{
    BOOL fgFetchOK = FALSE;
    UINT32 i, j;
    UINT32 dwVLDByte, dwVLDBit;
    UINT32 dwVFifo;
    UINT32 dwVLDRemainByte;

    LOG(7, "fgVldInitBarrelShifter, position : %08x\n", dwPtr);

    dwVFifo = ((UINT32)rWMVDecInfo.rDram.pu1VFifo);
    dwVLDRemainByte = dwPtr & 0xf;

    for (i =0 ; i<5 ; i++) {
        _VDEC_UpdateVldWMVWPtr(rWMVDecInfo.rOther.u4WPtr);

        WriteREG(RW_VLD_RPTR, dwPtr);

        if (ReadREG(RO_VLD_VRPTR) != (dwPtr - (dwPtr % 16)))
        {
            LOG(1, "RPTR moved before INIFET\n");
            return FALSE;
        }
        WriteREG(RW_VLD_PROC, (UINT32)(1<<20));

        if (fgWaitVldFetchOk())
        {
            fgFetchOK = TRUE;
            break;
        }
    }
    if (!fgFetchOK)
        return FALSE;
    WriteREG(RW_VLD_PROC, (UINT32)(1<<23));

    dwVLDByte = dwVldRPtr(&dwVLDBit);

    for (j=0; j<dwVLDRemainByte; j++)
    {
        //mark by spork
        dwGetBitStream(8);
        dwVLDByte = dwVldRPtr(&dwVLDBit);
        if (dwVLDByte == (dwPtr-dwVFifo))
            break;
    }
    return (TRUE);
}

BOOL fgWaitVldFetchOk(void)
{
    UINT32 u4Tmp = 0;

    while ((ReadREG(RO_VLD_FETCHOK) & 0x1) == 0)
    {
        u4Tmp++;
        if (u4Tmp > 0x10000)
        {
            return (FALSE);
        }
    }
#ifdef WAIT_SRAM_STABLE
    u4Tmp = 0;
    while ((ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
    {
        u4Tmp++;
        if (u4Tmp >= 0x10000)
        {
            ASSERT(0);
        }
    }
#endif

    return (TRUE);
}

void vPowerOffVDec(void)
{
    while ((ReadREG(RO_VLD_FETCHOK) & VLD_BAR_FETCH_FIN) == 0)
        ;
    WriteREG(RW_VLD_PWRSAVE, 1);
}

UINT32 dwGetBitStream(UINT8 bShiftBit)
{
    UINT32 dwRegVal;

    rWMVDecInfo.rOther.u4BitCount += (UINT32)bShiftBit;
    dwRegVal = ReadREG((UINT32)(RO_VLD_BARL + (bShiftBit << 2)));
    return (dwRegVal);
}

void vVldByteAlign(void)
{
    UINT8 u1BitCount;

    u1BitCount = (UINT8)(ReadREG(RW_VLD_BITCOUNT) & 0x7);
    if (u1BitCount != 0)
    {
        dwGetBitStream((UINT8)(8-u1BitCount));
    }
    rWMVDecInfo.rOther.u4BitCount = 0;
}

/********************/
/* member functions */
/********************/

void vInitVParserWMV(void)
{
    rWMVDecInfo.rOther.u4DispBufIdx = 1;
    if (rWMVDecInfo.rOther.u4DispBufIdx == 1)
    {
        vSetFRefBuf(0);
        vSetBRefBuf(1);
    }
    else
    {
        vSetFRefBuf(1);
        vSetBRefBuf(0);
    }
    rWMVDecInfo.rOther.u4DecPicNo = 0;
    rWMVDecInfo.rOther.u4CurrPicNo = 0;

    //  rWMVDecInfo.rSeqLayer.fgVC1 = FALSE; //move to vCodecVersion(), incluing initialize rWMVDecInfo.rOther.i4CodecVersion

    //  vCodecVersion();

    //Sequence Header variables initialization
    //Advanced
    //local _iLevel = 0;
    //local _iChromaFormat = 1;
    rWMVDecInfo.rSeqLayer.fgBroadcastFlags = FALSE;
    rWMVDecInfo.rSeqLayer.fgInterlacedSource = FALSE;
    rWMVDecInfo.rSeqLayer.fgTemporalFrmCntr = FALSE;

    rWMVDecInfo.rSeqLayer.fgSeqFrameInterpolation = FALSE;
    //for display use _fgInterpolateCurrentFrame = FALSE;

    rWMVDecInfo.rSeqLayer.fgHRD_PARAM_FLAG = FALSE;
    rWMVDecInfo.rSeqLayer.i4hrd_num_leaky_buckets = 0;
    rWMVDecInfo.rPicLayer.fgTopFieldFirst = TRUE; //ming
    //Simple & Main
    //local _fgRndCtrlOn = FALSE;
    rWMVDecInfo.rSMProfile.fgXintra8Switch = FALSE;
    rWMVDecInfo.rSMProfile.fgMultiresEnabled = FALSE;
    rWMVDecInfo.rSMProfile.i4ResIndex = 0;
    //local _fg16bitXform = FALSE;
    rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED = FALSE;
    //local _fgStartCode = FALSE;
    rWMVDecInfo.rSMProfile.fgPreProcRange = FALSE;
    rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
    //local _fgBetaContent = TRUE;
    //local _fgRTMContent = FALSE;
    //no use _iBetaRTMMismatchIndex = 0;
    rWMVDecInfo.rSMProfile.fgRotatedIdct = FALSE;
    rWMVDecInfo.rPicLayer.u1FrameCodingMode = PROGRESSIVE; //ming
    // WMV7 & WMV8
    rWMVDecInfo.rSMProfile.fgMixedPel = FALSE;
    rWMVDecInfo.rSMProfile.fgFrmHybridMVOn = FALSE;
    rWMVDecInfo.rSMProfile.fgXintra8 = FALSE;

    //End of Sequence

    rWMVDecInfo.rPicLayer.fgPostRC1 = TRUE;

    //EntryPoint Header variables initialization
    rWMVDecInfo.rEntryPoint.fgBrokenLink = FALSE;
    rWMVDecInfo.rEntryPoint.fgClosedEntryPoint = FALSE;
    rWMVDecInfo.rEntryPoint.fgPanScanPresent = FALSE;
    rWMVDecInfo.rEntryPoint.fgRefDistPresent = FALSE;
    rWMVDecInfo.rEntryPoint.fgLoopFilter = FALSE;
    rWMVDecInfo.rEntryPoint.fgUVHpelBilinear = FALSE;
    rWMVDecInfo.rEntryPoint.i4RangeState = 0;
    rWMVDecInfo.rEntryPoint.i4ReconRangeState = 0;
    rWMVDecInfo.rEntryPoint.fgExtendedMvMode = FALSE;
    rWMVDecInfo.rEntryPoint.i4MVRangeIndex = 0;
    rWMVDecInfo.rEntryPoint.i4DQuantCodingOn = 0;
    rWMVDecInfo.rEntryPoint.fgXformSwitch = FALSE;
    rWMVDecInfo.rEntryPoint.fgSequenceOverlap = FALSE;
    //Quant related
    rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer = FALSE;
    rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer = FALSE;
    rWMVDecInfo.rEntryPoint.fgExplicitQuantizer = FALSE;
    rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = FALSE;
    rWMVDecInfo.rPicLayer.fgHalfStep = FALSE;

    rWMVDecInfo.rEntryPoint.fgExtendedDeltaMvMode = FALSE;
    rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex = 0;
    rWMVDecInfo.rEntryPoint.i4ExtendedDMVX = 0;
    rWMVDecInfo.rEntryPoint.i4ExtendedDMVY = 0;
    rWMVDecInfo.rEntryPoint.i4RefFrameDistance = 0;

    rWMVDecInfo.rPicLayer.i4BNumerator = 0;
    rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
    rWMVDecInfo.rPicLayer.i4DCStepSize = 0;
    rWMVDecInfo.rOther.i4X9MVMode = 0;

    rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
    rWMVDecInfo.rOther.i4FrameXformMode = 0;

    rWMVDecInfo.rEntryPoint.fgRangeRedY_Flag = FALSE;
    rWMVDecInfo.rEntryPoint.fgRangeRedUV_Flag = FALSE;
    //End of EntryPoint


    rWMVDecInfo.rDram.u4ForwardRefPicType = 0;
    rWMVDecInfo.rDram.u4BackwardRefPicType = 0;

    // Picture Header
    //WMV7 & WMV8
    rWMVDecInfo.rOther.fgDCPred_IMBInPFrame = FALSE;

    //for field pictures

    rWMVDecInfo.rPicLayer.fgTopFieldFirst = TRUE;
    rWMVDecInfo.rPicLayer.fgRepeatFirstField = FALSE;
    //FALSE for PROGRESSIVE.
    rWMVDecInfo.rPicLayer.fgInterlaceV2 = FALSE;
    rWMVDecInfo.rPicLayer.fgFieldMode = FALSE;
    rWMVDecInfo.rPicLayer.i4CurrentField = 0; // 0:TOP, 1:BOTTOM field
    rWMVDecInfo.rPicLayer.i4CurrentTemporalField = 0; // 0:1st field or frame picture, 1: 2nd field

    rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVX = 0;
    rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVY = 0;
    rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVX = 0;
    rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVY = 0;
    rWMVDecInfo.rPicLayer.i4FarFieldScale1 = 0;
    rWMVDecInfo.rPicLayer.i4FarFieldScale2 = 0;
    rWMVDecInfo.rPicLayer.i4NearFieldScale = 0;
    rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVX = 0;
    rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVY = 0;
    rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVX = 0;
    rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVY = 0;
    rWMVDecInfo.rPicLayer.i4FarFieldScaleBack1 = 0;
    rWMVDecInfo.rPicLayer.i4FarFieldScaleBack2 = 0;
    rWMVDecInfo.rPicLayer.i4NearFieldScaleBack = 0;

    rWMVDecInfo.rPicLayer.fgTwoRefPictures = TRUE;
    rWMVDecInfo.rPicLayer.fgUseOppFieldForRef = TRUE;
    rWMVDecInfo.rPicLayer.fgUseSameFieldForRef = TRUE;
    //Robert TODO: 0511
    rWMVDecInfo.rPicLayer.fgBackRefUsedHalfPel = FALSE;
    rWMVDecInfo.rPicLayer.fgBackRefTopFieldHalfPelMode = FALSE;
    rWMVDecInfo.rPicLayer.fgBackRefBottomFieldHalfPelMode = FALSE;

    rWMVDecInfo.rPicLayer.fgMvResolution = FALSE;

    rWMVDecInfo.rPicLayer.i4Overlap = 0;
    rWMVDecInfo.rOther.i4MvTable = 0;
    rWMVDecInfo.rOther.i4CBPTable = 0;
    rWMVDecInfo.rOther.i4MBModeTable = 0;
    rWMVDecInfo.rOther.i42MVBPTable = 0;
    rWMVDecInfo.rOther.i44MVBPTable = 0;

    //!WMVA profile
    rWMVDecInfo.rSMProfile.fgPreProcRange = FALSE;

    rWMVDecInfo.rEntryPoint.fgNewDCQuant = FALSE;

    rWMVDecInfo.rPicLayer.fgDCTTable_MB = FALSE;

    // WMV7 & WMV8
    //Robert TODO:
    if (rWMVDecInfo.rOther.i4CodecVersion == WMV2)
    {
        rWMVDecInfo.rSMProfile.fgSKIPBIT_CODING = TRUE;
        rWMVDecInfo.rSMProfile.fgNEW_PCBPCY_TABLE = TRUE;
    }
    else
    {
        rWMVDecInfo.rSMProfile.fgSKIPBIT_CODING = FALSE;
        rWMVDecInfo.rSMProfile.fgNEW_PCBPCY_TABLE = FALSE;
    }

    rWMVDecInfo.rSMProfile.fgCODFlagOn = TRUE;

    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        rWMVDecInfo.rEntryPoint.fgNewDCQuant = TRUE;
    }

    ComputeDQuantDecParam();

    rWMVDecInfo.rOther.u1DiffQtProfile = 0;

    //rWMVDecInfo.rOther.i4SeqHdrData1 = 0;
    //rWMVDecInfo.rOther.i4SeqHdrData2 = 0;
    rWMVDecInfo.rSMProfile.i4skip_bit_mode_v87 = 0;
    rWMVDecInfo.rSMProfile.i4wmv8_bp_mode = 0;

    //NEEDS to initialize
    rWMVDecInfo.rOther.u4new_entry_point = 0;
    rWMVDecInfo.rOther.i4pred_use_wdle = 0;
    rWMVDecInfo.rSeqLayer.fgPostProcInfoPresent = FALSE;
    rWMVDecInfo.rSMProfile.fgYUV411 = FALSE;
    rWMVDecInfo.rSMProfile.fgSpriteMode = FALSE;
    rWMVDecInfo.rEntryPoint.i4RangeRedY = 0;
    rWMVDecInfo.rEntryPoint.i4Range_MapUV = 0;
    rWMVDecInfo.rPicLayer.u1RepeatFrameCount = 0;
    rWMVDecInfo.rOther.u1DQuantBiLevelStepSize = 0;
    rWMVDecInfo.rOther.fgDQuantOn = FALSE;
    rWMVDecInfo.rOther.i4Panning = 0;
    rWMVDecInfo.rOther.fgDQuantBiLevel = FALSE;

}

void vResetConditionalVariablesForSequenceSwitch(void)
{
    rWMVDecInfo.rEntryPoint.fgExtendedDeltaMvMode = FALSE;
    rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer
    = rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer
      = rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = FALSE;
    //rWMVDecInfo.rPicLayer.fgTopFieldFirst = TRUE;
    rWMVDecInfo.rPicLayer.fgRepeatFirstField = FALSE;
    rWMVDecInfo.rPicLayer.u1RepeatFrameCount = 0;
    //local _fgUVProgressiveSubsampling = TRUE;
    //for display use _fgInterpolateCurrentFrame = FALSE;
    //local _iPpMethod = -1;
    rWMVDecInfo.rEntryPoint.i4MVRangeIndex = 0;
    rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex = 0;
    rWMVDecInfo.rEntryPoint.i4ExtendedDMVX
    = rWMVDecInfo.rEntryPoint.i4ExtendedDMVY = 0;
    //_fgCBEntropyCoding = FALSE;
    rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
    rWMVDecInfo.rEntryPoint.i4RefFrameDistance = 0;
    rWMVDecInfo.rOther.i4FrameXformMode = XFORMMODE_8x8;
    rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
}

//extern UINT32 rWMVDecInfo.rOther.u4BitCount;

UINT32 dwVParserWMVA(void)
{
    UINT32 u4StartCode;
    UINT32 u4bits;
    UINT32 u4Addr;
    UCHAR *pucBuf;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    UNUSED(u4bits);
    //BOOL fgFirstTime = TRUE;
    prVdecEsInfo = _VDEC_GetEsInfo(ES0);

    if (prVdecEsInfo == NULL)
    {
        ASSERT(0);
        return 0;
    }
    rWMVDecInfo.rOther.i4SetPos = (rWMVDecInfo.rPesInfo.u4VldReadPtr - rWMVDecInfo.rPesInfo.u4FifoStart);

#if 0
    x_memcpy(&u4StartCode, (VOID*)VIRTUAL((UINT32)(rWMVDecInfo.rDram.pu1VFifo
                                          + rWMVDecInfo.rOther.i4SetPos)), 4);
    u4StartCode = ((u4StartCode&0x000000FF)<<24)
                  | ((u4StartCode&0x0000FF00)<<8) | ((u4StartCode&0x00FF0000)>>8)
                  | ((u4StartCode&0xFF000000)>>24);
    fgVldInitBarrelShifter((UINT32)rWMVDecInfo.rDram.pu1VFifo
                           + rWMVDecInfo.rOther.i4SetPos+4);
#else
    u4Addr = rWMVDecInfo.rPesInfo.u4VldReadPtr + 3;
    if (u4Addr >= rWMVDecInfo.rPesInfo.u4FifoEnd)
    {
        u4Addr -= (rWMVDecInfo.rPesInfo.u4FifoEnd - rWMVDecInfo.rPesInfo.u4FifoStart);
    }
    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
    u4StartCode = pucBuf[0];
    u4StartCode |= 0x100;
    fgVldInitBarrelShifter(u4Addr+1);
#endif
    //ASSERT(rWMVDecInfo.rPesInfo.u4VldReadPtr+4 <= (rWMVDecInfo.rDram.pu1FifoEnd));

    LOG(6,"Rptr 0x%x, input win 0x%x\n", dwVldRPtr(&u4bits), ReadREG(RO_VLD_BARL));
    LOG(6,"start code : 0x%08x, offset : %x\n",u4StartCode,rWMVDecInfo.rOther.i4SetPos);

    switch (u4StartCode)
    {
    case WMV_SC_FRAME:
        rWMVDecInfo.rPicLayer.i4CurrentTemporalField = 0;
        break;
    case WMV_SC_FIELD:
        break;
    case WMV_SC_SEQ:
        //LOG(0,"parsing sequence header\n");
        decodeSequenceHead_Advanced();
        rWMVDecInfo.rOther.fgChangeResolution = FALSE;
        //create FBM group
#if 1
        if (rWMVDecInfo.rOther.u1FbgId != FBM_FBG_ID_UNKNOWN)
        {
            if (((rWMVDecInfo.rSeqHdrR.u4PicHeightSrc == rWMVDecInfo.rSeqHdrR.u4PrevPicHeightSrc)
                    && (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc == rWMVDecInfo.rSeqHdrR.u4PrevPicWidthSrc))
                    || (rWMVDecInfo.rSeqHdrR.u4PrevPicWidthSrc ==0 && rWMVDecInfo.rSeqHdrR.u4PrevPicHeightSrc ==0))
            {
                rWMVDecInfo.rOther.fgChangeResolution = FALSE;
                break;
            }
            else
            {
                rWMVDecInfo.rOther.fgChangeResolution = TRUE;
                if (prVdecEsInfo->eMMSrcType != SWDMX_SRC_TYPE_NETWORK_NETFLIX)
                {
                    FBM_SetFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_RELEASE_FBG);
                }
                else
                {
                    break;
                }
                //FBM_SetFrameBufferFlag(rWMVDecInfo.rOther.u1FbgId, FBM_FLAG_SEQ_CHG);
                //break;
            }
        }
        vInitVdoMem();
#else
        if (rWMVDecInfo.rOther.u1FbgId == FBM_FBG_ID_UNKNOWN)
        {
            vInitVdoMem();
        }
#endif
        break;
    case WMV_SC_ENTRY:
        //LOG(0,"paring entry point header\n");
        vResetConditionalVariablesForSequenceSwitch();
        DecodeEntryPointHeader();
        break;
    case WMV_SC_ENDOFSEQ:
        //rWMVDecInfo.rOther.u4Datain = dwGetBitStream(32); // flush the Seq End Code.
        return END_OF_FILE;
    default:
        dwGetHi(8); //flush this Start Code
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
    if (rWMVDecInfo.rOther.i4CodecVersion != WMVA) // simple or main profile
    {
        // WMVideoDecDecodeFrameHead();
    }
    else //advanced profile
    {
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0); //flush Start Code.
        reset_pic_hdr_bits();

        if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 1)
        {
            set_pic_hdr_bits(rWMVDecInfo.rPicLayer.i4SlicePicHeaderNumField);
            goto INTERLACE_FIELD;
        }

        if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
        {
            if (0 == dwGetHi(1))
            {
                rWMVDecInfo.rPicLayer.u1FrameCodingMode = PROGRESSIVE;
            }
            else
            {
                if (0 == dwGetHi(1))
                {
                    rWMVDecInfo.rPicLayer.u1FrameCodingMode = INTERLACEFRAME;
                }
                else
                {
                    rWMVDecInfo.rPicLayer.u1FrameCodingMode = INTERLACEFIELD;
                }
            }
        }
        else
        {
            rWMVDecInfo.rPicLayer.u1FrameCodingMode = PROGRESSIVE;
        }

        if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == PROGRESSIVE)
        {
            rWMVDecInfo.rSeqHdrR.u4PicWidthDec
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[PROGRESSIVE].iWidthDec;
            rWMVDecInfo.rSeqHdrR.u4PicHeightDec
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[PROGRESSIVE].iHeightDec;
            rWMVDecInfo.rSeqHdrR.u4NumMBX = rWMVDecInfo.rSeqHdrR.u4PicWidthDec
                                            >> 4;
            rWMVDecInfo.rSeqHdrR.u4NumMBY = rWMVDecInfo.rSeqHdrR.u4PicHeightDec
                                            >> 4;
            rWMVDecInfo.rSeqHdrR.u4PicWidthCmp
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[PROGRESSIVE].iFrmWidthSrc;
            rWMVDecInfo.rSeqHdrR.u4PicHeightCmp
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[PROGRESSIVE].iFrmHeightSrc;

            rWMVDecInfo.rPicLayer.fgInterlaceV2 = FALSE;
            rWMVDecInfo.rPicLayer.fgFieldMode = FALSE;
            rWMVDecInfo.rPicLayer.i4CurrentField = 0; // 0:TOP, 1:BOTTOM field
            decodeVOPHeadProgressiveWMVA(); // advanced progressive
        }
        else if (rWMVDecInfo.rPicLayer.u1FrameCodingMode == INTERLACEFRAME)
        {
            //Printf("!!!!!interlaceframe\n");
            rWMVDecInfo.rSeqHdrR.u4PicWidthDec
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFRAME].iWidthDec;
            rWMVDecInfo.rSeqHdrR.u4PicHeightDec
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFRAME].iHeightDec;
            rWMVDecInfo.rSeqHdrR.u4NumMBX = rWMVDecInfo.rSeqHdrR.u4PicWidthDec
                                            >> 4;
            rWMVDecInfo.rSeqHdrR.u4NumMBY = rWMVDecInfo.rSeqHdrR.u4PicHeightDec
                                            >> 4;
            rWMVDecInfo.rSeqHdrR.u4PicWidthCmp
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFRAME].iFrmWidthSrc;
            rWMVDecInfo.rSeqHdrR.u4PicHeightCmp
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFRAME].iFrmHeightSrc;

            rWMVDecInfo.rPicLayer.fgInterlaceV2 = TRUE;
            rWMVDecInfo.rPicLayer.fgFieldMode = FALSE;
            rWMVDecInfo.rPicLayer.i4CurrentField = 0; // 0:TOP, 1:BOTTOM field

            decodeVOPHeadInterlaceV2(); // advanced interlace-frame
        }
        else // INTERLACE_FIELD
        {
INTERLACE_FIELD:
            rWMVDecInfo.rSeqHdrR.u4PicWidthDec
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFIELD].iWidthDec;
            rWMVDecInfo.rSeqHdrR.u4PicHeightDec
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFIELD].iHeightDec;
            rWMVDecInfo.rSeqHdrR.u4NumMBX = rWMVDecInfo.rSeqHdrR.u4PicWidthDec
                                            >> 4;
            rWMVDecInfo.rSeqHdrR.u4NumMBY = rWMVDecInfo.rSeqHdrR.u4PicHeightDec
                                            >> 4;
            rWMVDecInfo.rSeqHdrR.u4PicWidthCmp
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFIELD].iFrmWidthSrc;
            rWMVDecInfo.rSeqHdrR.u4PicHeightCmp
            = rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFIELD].iFrmHeightSrc;

            rWMVDecInfo.rPicLayer.fgInterlaceV2 = TRUE;
            rWMVDecInfo.rPicLayer.fgFieldMode = TRUE;

            if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0) //1st field
            {
                decodeVOPHeadFieldPicture(); // advanced interlace-field
                rWMVDecInfo.rPicLayer.i4SlicePicHeaderNumField
                = pic_hdr_bitcount();

                rWMVDecInfo.rPicLayer.i4CurrentField
                = (rWMVDecInfo.rPicLayer.fgTopFieldFirst) ? (UINT32)0 : (UINT32)1; // 0:TOP, 1:BOTTOM field

                rWMVDecInfo.rPicLayer.u1PicType
                = rWMVDecInfo.rPicLayer.u1FirstFieldType;
                decodeFieldHeadFieldPicture();
                if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                {
                    SetupFieldPictureMVScaling(rWMVDecInfo.rEntryPoint.i4RefFrameDistance);
                    decodePFieldMode();
                }
                else if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
                {
                    INT32
                    iForwardRefDistance =(INT32)
                                         (rWMVDecInfo.rPicLayer.i4BNumerator
                                          * rWMVDecInfo.rEntryPoint.i4RefFrameDistance
                                          * rWMVDecInfo.rPicLayer.i4BFrameReciprocal)
                                         >> 8;
                    INT32 iBackwardRefDistance = (INT32)
                                                 (rWMVDecInfo.rEntryPoint.i4RefFrameDistance
                                                  - iForwardRefDistance - 1);
                    // SetupForwardBFieldPictureMVScaling (pWMVDec, iForwardRefDistance);
                    if (iBackwardRefDistance < 0)
                        iBackwardRefDistance = 0;
                    SetupFieldPictureMVScaling((UINT32)iForwardRefDistance);
                    SetupBackwardBFieldPictureMVScaling((UINT32)iBackwardRefDistance);
                    decodeBFieldMode();

                    rWMVDecInfo.rOther.i4FwdRefDist = iForwardRefDistance;
                    rWMVDecInfo.rOther.i4BwdRefDist = iBackwardRefDistance;
                }
            }
            else //2nd field
            {
                rWMVDecInfo.rPicLayer.u1PicType
                = rWMVDecInfo.rPicLayer.u1SecondFieldType;
                decodeFieldHeadFieldPicture();
                if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
                {
                    SetupFieldPictureMVScaling(rWMVDecInfo.rEntryPoint.i4RefFrameDistance);
                    decodePFieldMode();

                }
                else if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
                {
                    INT32
                    iForwardRefDistance = (INT32)
                                          (rWMVDecInfo.rPicLayer.i4BNumerator
                                           * rWMVDecInfo.rEntryPoint.i4RefFrameDistance
                                           * rWMVDecInfo.rPicLayer.i4BFrameReciprocal)
                                          >> 8;
                    INT32 iBackwardRefDistance = (INT32)
                                                 (rWMVDecInfo.rEntryPoint.i4RefFrameDistance
                                                  - iForwardRefDistance - 1);
                    //SetupForwardBFieldPictureMVScaling (pWMVDec, iForwardRefDistance);
                    if (iBackwardRefDistance < 0)
                        iBackwardRefDistance = 0;
                    SetupFieldPictureMVScaling((UINT32)iForwardRefDistance);
                    SetupBackwardBFieldPictureMVScaling((UINT32)iBackwardRefDistance);
                    decodeBFieldMode();

                    rWMVDecInfo.rOther.i4FwdRefDist = iForwardRefDistance;
                    rWMVDecInfo.rOther.i4BwdRefDist = iBackwardRefDistance;
                }
            }
        }

        rWMVDecInfo.rPicLayer.i4SlicePicHeaderNum = pic_hdr_bitcount();
    }

    return 0;
}

UINT32 decodeSequenceHead_Advanced(void)
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

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0); // Flush the Seq Head Start Code
    //LOG(0,"datain : %x\n", rWMVDecInfo.rOther.u4Datain);
    //LOG(0,"Rptr 0x%x, input win 0x%x\n", ReadREG(RO_VLD_VRPTR), ReadREG(RO_VLD_BARL));

    //local _fgRndCtrlOn = TRUE;
    rWMVDecInfo.rEntryPoint.fgRangeRedY_Flag = FALSE; // Resetting range red flags at the beginning of new advance sequence header
    rWMVDecInfo.rEntryPoint.fgRangeRedUV_Flag = FALSE;

    // old/unused stuff
    rWMVDecInfo.rSMProfile.fgMixedPel = rWMVDecInfo.rSMProfile.fgFrmHybridMVOn
                                        = FALSE;
    rWMVDecInfo.rSMProfile.fgYUV411 = FALSE;
    rWMVDecInfo.rSMProfile.fgSpriteMode = FALSE;
    rWMVDecInfo.rSMProfile.fgXintra8Switch = FALSE;
    rWMVDecInfo.rSMProfile.fgXintra8 = FALSE;
    rWMVDecInfo.rSMProfile.fgMultiresEnabled = FALSE;
    //local _fg16bitXform = TRUE;
    //if(_fg16bitXform)
    {
        rWMVDecInfo.rSMProfile.fgRotatedIdct = TRUE;
    }

    //  rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED = FALSE;

    rWMVDecInfo.rOther.u4GetVal = dwGetHi(28);
    rWMVDecInfo.rSeqLayer.i4Profile = (rWMVDecInfo.rOther.u4GetVal & 0xc000000)
                                      >> 26;
    rWMVDecInfo.rSeqLayer.i4WMV3Profile = WMV3_ADVANCED_PROFILE;
    //iLevel = (rWMVDecInfo.rOther.u4GetVal & 0x3800000) >> 23;
    //iChromaFormat = (rWMVDecInfo.rOther.u4GetVal & 0x600000) >> 21;
    rWMVDecInfo.rSeqLayer.i4FrameRate
    = (rWMVDecInfo.rOther.u4GetVal & 0x1c0000) >> 18;
    rWMVDecInfo.rSeqLayer.i4BitRate = (rWMVDecInfo.rOther.u4GetVal & 0x3e000)
                                      >> 13;
    rWMVDecInfo.rSeqLayer.fgPostProcInfoPresent = (rWMVDecInfo.rOther.u4GetVal
            & 0x1000) >> 12;
    rWMVDecInfo.rSeqHdrR.u4MaxCodedWidth = rWMVDecInfo.rOther.u4GetVal & 0xfff;

    rWMVDecInfo.rOther.u4GetVal = dwGetHi(19);
    rWMVDecInfo.rSeqHdrR.u4MaxCodedHeight = (rWMVDecInfo.rOther.u4GetVal
                                            & 0x7ff80) >> 7;

    //reset seq hdr
    rWMVDecInfo.rOther.u1GetSeqHdr = 0;

    // Robert: translate to what I use
    rWMVDecInfo.rSeqHdrR.u4MaxPicWidthSrc = 2
                                            * rWMVDecInfo.rSeqHdrR.u4MaxCodedWidth + 2;
    rWMVDecInfo.rSeqHdrR.u4MaxPicHeightSrc = 2
            * rWMVDecInfo.rSeqHdrR.u4MaxCodedHeight + 2;
    rWMVDecInfo.rSeqHdrR.u4PicWidthSrc = 2
                                         * rWMVDecInfo.rSeqHdrR.u4MaxCodedWidth + 2;
    rWMVDecInfo.rSeqHdrR.u4PicHeightSrc = 2
                                          * rWMVDecInfo.rSeqHdrR.u4MaxCodedHeight + 2;

    rWMVDecInfo.rOther.rPicInfo.dwW = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
    rWMVDecInfo.rOther.rPicInfo.dwH = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;

    SetupMultiResParams();

    rWMVDecInfo.rSeqLayer.fgBroadcastFlags = (rWMVDecInfo.rOther.u4GetVal
            & 0x40) >> 6;
    rWMVDecInfo.rSeqLayer.fgInterlacedSource = (rWMVDecInfo.rOther.u4GetVal
            & 0x20) >> 5;
    rWMVDecInfo.rSeqLayer.fgTemporalFrmCntr = (rWMVDecInfo.rOther.u4GetVal
            & 0x10) >> 4;
    rWMVDecInfo.rSeqLayer.fgSeqFrameInterpolation
    = (rWMVDecInfo.rOther.u4GetVal & 0x8) >> 3;
    //bReserved = (UINT8)((rWMVDecInfo.rOther.u4GetVal & 0x6) >> 1);
    rWMVDecInfo.rSeqLayer.fgProgressiveDisp = (rWMVDecInfo.rOther.u4GetVal
            & 0x2)>>1;
    fgDisplay_Ext = rWMVDecInfo.rOther.u4GetVal & 0x1;

    if (fgDisplay_Ext)
    {
        rWMVDecInfo.rOther.u4GetVal = dwGetHi(29);
        //dwDisp_Horiz_Size = (rWMVDecInfo.rOther.u4GetVal & 0x1fff8000) >> 15;
        //dwDisp_Vert_Size = (rWMVDecInfo.rOther.u4GetVal & 0x7ffe) >> 1;
        fgAspect_Ratio_Flag = rWMVDecInfo.rOther.u4GetVal & 0x1;
        if (fgAspect_Ratio_Flag)
        {
            rWMVDecInfo.rSeqLayer.i4Aspect_Ratio = dwGetHi(4);
            if (rWMVDecInfo.rSeqLayer.i4Aspect_Ratio == 15)
            {
                rWMVDecInfo.rOther.u4GetVal = dwGetHi(16);
                rWMVDecInfo.rSeqLayer.i4Aspect_Horiz_Size
                = (rWMVDecInfo.rOther.u4GetVal & 0xff00) >> 8;
                rWMVDecInfo.rSeqLayer.i4Aspect_Vert_Size
                = rWMVDecInfo.rOther.u4GetVal & 0xff;
            }
        }
        fgFrameRate_Flag = dwGetHi(1);
        if (fgFrameRate_Flag)
        {
            fgFrameRateInd = dwGetHi(1);
            if (fgFrameRateInd == FALSE)
            {
                rWMVDecInfo.rOther.u4GetVal = dwGetHi(12);
                rWMVDecInfo.rSeqLayer.i4FrameRateNr
                = (rWMVDecInfo.rOther.u4GetVal & 0xff0) >> 4;
                rWMVDecInfo.rSeqLayer.i4FrameRateDr
                = rWMVDecInfo.rOther.u4GetVal & 0xf;
            }
            else
            {
                rWMVDecInfo.rSeqLayer.i4FrameRateExp = dwGetHi(16);
            }
        }
        fgColor_Format_Flag = dwGetHi(1);
        if (fgColor_Format_Flag)
        {
            rWMVDecInfo.rOther.u4GetVal = dwGetHi(24);
            //iColor_Prim = (rWMVDecInfo.rOther.u4GetVal & 0xff0000) >> 16;
            //iTransfer_Char = (rWMVDecInfo.rOther.u4GetVal & 0xff00) >> 8;
            //iMatrix_Coef = rWMVDecInfo.rOther.u4GetVal & 0xff;
        }
    }

    rWMVDecInfo.rSeqLayer.fgHRD_PARAM_FLAG = dwGetHi(1);
    if (rWMVDecInfo.rSeqLayer.fgHRD_PARAM_FLAG)
    {
        //HDR_PARAM()
        rWMVDecInfo.rOther.u4GetVal = dwGetHi(13);
        rWMVDecInfo.rSeqLayer.i4hrd_num_leaky_buckets
        = (rWMVDecInfo.rOther.u4GetVal & 0x1f00) >> 8;
        //iBit_Rate_Exponent = (rWMVDecInfo.rOther.u4GetVal & 0xf0) >> 4;
        //iBuffer_Size_Exponent = rWMVDecInfo.rOther.u4GetVal & 0xf;

        for (u4Idx = 1; u4Idx <= rWMVDecInfo.rSeqLayer.i4hrd_num_leaky_buckets; u4Idx++)
        {
            rWMVDecInfo.rOther.u4GetVal = dwGetHi(32);
            //wHrd_Rate[i] = (UINT16)((rWMVDecInfo.rOther.u4GetVal & 0xffff0000) >> 16);
            //wHrd_Buffer[i] = (UINT16)(rWMVDecInfo.rOther.u4GetVal & 0xffff);
        }
    }

    rWMVDecInfo.rPicLayer.i4BFrameReciprocal
    = _iBInverse[rWMVDecInfo.rSMProfile.i4NumBFrames];
    return (TRUE);
}

UINT32 DecodeEntryPointHeader(void)
{
    UINT32 u4Idx;
    BOOL fgCoded_Size_Flag;
    //UINT8 bHrd_Fullness[32];


    rWMVDecInfo.rOther.u4new_entry_point = 1;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0); // Flush the Entry Point Start Code
    //LOG(0,"Rptr 0x%x, input win 0x%x\n", ReadREG(RO_VLD_VRPTR), ReadREG(RO_VLD_BARL));
    rWMVDecInfo.rOther.u4GetVal = dwGetHi(13);
    rWMVDecInfo.rEntryPoint.fgBrokenLink = (rWMVDecInfo.rOther.u4GetVal
                                            & 0x1000) >> 12;
    rWMVDecInfo.rEntryPoint.fgClosedEntryPoint = (rWMVDecInfo.rOther.u4GetVal
            & 0x800) >> 11;
    rWMVDecInfo.rEntryPoint.fgPanScanPresent = (rWMVDecInfo.rOther.u4GetVal
            & 0x400) >> 10;
    rWMVDecInfo.rEntryPoint.fgRefDistPresent = (rWMVDecInfo.rOther.u4GetVal
            & 0x200) >> 9;
    rWMVDecInfo.rEntryPoint.fgLoopFilter
    = (rWMVDecInfo.rOther.u4GetVal & 0x100) >> 8;
    rWMVDecInfo.rEntryPoint.fgUVHpelBilinear = (rWMVDecInfo.rOther.u4GetVal
            & 0x80) >> 7;
    rWMVDecInfo.rEntryPoint.fgExtendedMvMode = (rWMVDecInfo.rOther.u4GetVal
            & 0x40) >> 6;
    rWMVDecInfo.rEntryPoint.i4DQuantCodingOn = (rWMVDecInfo.rOther.u4GetVal
            & 0x30) >> 4;
    rWMVDecInfo.rEntryPoint.fgXformSwitch = (rWMVDecInfo.rOther.u4GetVal & 0x8)
                                            >> 3;
    rWMVDecInfo.rEntryPoint.fgSequenceOverlap = (rWMVDecInfo.rOther.u4GetVal
            & 0x4) >> 2;
    rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer
    = (rWMVDecInfo.rOther.u4GetVal & 0x2) >> 1;
    if (rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer)
        rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = rWMVDecInfo.rOther.u4GetVal
                & 0x1;
    else
        rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer
        = rWMVDecInfo.rOther.u4GetVal & 0x1;

    rWMVDecInfo.rEntryPoint.fgExplicitQuantizer
    = rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer
      || rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer;
    rWMVDecInfo.rPicLayer.i4BFrameReciprocal
    = _iBInverse[rWMVDecInfo.rSMProfile.i4NumBFrames];
    rWMVDecInfo.rPicLayer.u1PicType = IVOP;

    if (rWMVDecInfo.rSeqLayer.fgHRD_PARAM_FLAG)
    {
        //hrd_fullness()
        for (u4Idx = 1; u4Idx <= rWMVDecInfo.rSeqLayer.i4hrd_num_leaky_buckets; u4Idx++)
        {
            /*bHrd_Fullness[i] =*/
            (UINT8)dwGetHi(8);
        }
    }

    fgCoded_Size_Flag = dwGetHi(1);
    if (fgCoded_Size_Flag)
    {
        rWMVDecInfo.rOther.u4GetVal = dwGetHi(24);
        rWMVDecInfo.rEntryPoint.u2Coded_Width
        = (UINT16)((rWMVDecInfo.rOther.u4GetVal & 0xfff000) >> 12);
        rWMVDecInfo.rEntryPoint.u2Coded_Height
        = (UINT16)(rWMVDecInfo.rOther.u4GetVal & 0xfff);

        //reset seq hdr
        rWMVDecInfo.rOther.u1GetSeqHdr = 0;

        //ASSERT(rWMVDecInfo.rEntryPoint.u2Coded_Width <= rWMVDecInfo.rSeqHdrR.u4MaxCodedWidth);
        //ASSERT(rWMVDecInfo.rEntryPoint.u2Coded_Height <= rWMVDecInfo.rSeqHdrR.u4MaxCodedHeight);
        if ((rWMVDecInfo.rEntryPoint.u2Coded_Width
                <= rWMVDecInfo.rSeqHdrR.u4MaxCodedWidth)
                && (rWMVDecInfo.rEntryPoint.u2Coded_Height
                    <= rWMVDecInfo.rSeqHdrR.u4MaxCodedHeight))
        {
            // Robert: translate to what I use
            rWMVDecInfo.rSeqHdrR.u4PicWidthSrc = 2
                                                 * (UINT32)rWMVDecInfo.rEntryPoint.u2Coded_Width + 2;
            rWMVDecInfo.rSeqHdrR.u4PicHeightSrc = 2
                                                  * (UINT32)rWMVDecInfo.rEntryPoint.u2Coded_Height + 2;

            rWMVDecInfo.rOther.rPicInfo.dwW
            = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
            rWMVDecInfo.rOther.rPicInfo.dwH
            = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
            SetupMultiResParams();
        }
    }
    if (rWMVDecInfo.rEntryPoint.fgExtendedMvMode)
    {
        rWMVDecInfo.rEntryPoint.fgExtendedDeltaMvMode = dwGetHi(1);
    }

    rWMVDecInfo.rEntryPoint.fgRangeRedY_Flag = dwGetHi(1);
    if (rWMVDecInfo.rEntryPoint.fgRangeRedY_Flag)
        rWMVDecInfo.rEntryPoint.i4RangeRedY = dwGetHi(3) + 1;
    else
        rWMVDecInfo.rEntryPoint.i4RangeRedY = 0;

    rWMVDecInfo.rEntryPoint.fgRangeRedUV_Flag = dwGetHi(1);
    if (rWMVDecInfo.rEntryPoint.fgRangeRedUV_Flag)
        rWMVDecInfo.rEntryPoint.i4Range_MapUV = dwGetHi(3) + 1;
    else
        rWMVDecInfo.rEntryPoint.i4Range_MapUV = 0;

    return (TRUE);
}

void UpdateDCStepSize(UINT32 iStepSize)
{

    rWMVDecInfo.rPicLayer.i4StepSize = iStepSize;
    //Printf("codec version wmva 44: %d\n",rWMVDecInfo.rOther.i4CodecVersion);

    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
    {
        DQuantDecParam *pDQ;
        iStepSize = (UINT8)((2 * iStepSize - 1) + (UINT8)rWMVDecInfo.rPicLayer.fgHalfStep);
        pDQ = &rWMVDecInfo.rPicLayer.prDQuantParam[iStepSize];
        rWMVDecInfo.rPicLayer.i4DCStepSize = pDQ->iDCStepSize;
    }
    else
    {
        rWMVDecInfo.rPicLayer.i4DCStepSize = 8;

        if ((rWMVDecInfo.rOther.i4CodecVersion >= MMIDRATE43)
                || (rWMVDecInfo.rOther.i4CodecVersion == MMIDRATE4S))
        {
            if (rWMVDecInfo.rPicLayer.i4StepSize <= 4)
            {
                rWMVDecInfo.rPicLayer.i4DCStepSize = 8;
                if (rWMVDecInfo.rEntryPoint.fgNewDCQuant
                        && (rWMVDecInfo.rPicLayer.i4StepSize <=2))
                    rWMVDecInfo.rPicLayer.i4DCStepSize
                    = rWMVDecInfo.rPicLayer.i4StepSize * 2;
            }
            else if (rWMVDecInfo.rOther.i4CodecVersion >= WMV1)
            {
                rWMVDecInfo.rPicLayer.i4DCStepSize
                = rWMVDecInfo.rPicLayer.i4StepSize / 2 + 6;
            }
            else if (rWMVDecInfo.rPicLayer.i4StepSize <= 8)
            {
                rWMVDecInfo.rPicLayer.i4DCStepSize = 2
                                                     * rWMVDecInfo.rPicLayer.i4StepSize;
            }
            else if (rWMVDecInfo.rPicLayer.i4StepSize <= 24)
            {
                rWMVDecInfo.rPicLayer.i4DCStepSize
                = rWMVDecInfo.rPicLayer.i4StepSize + 8;
            }
            else
            {
                rWMVDecInfo.rPicLayer.i4DCStepSize = 2
                                                     * rWMVDecInfo.rPicLayer.i4StepSize - 16;
            }
        }
    }
}

UINT32 decodeVOPHeadProgressiveWMVA(void)
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

    rWMVDecInfo.rPicLayer.u1PrevPicType = rWMVDecInfo.rPicLayer.u1PicType;
    // PTYPE
    //  dwTmp = (rWMVDecInfo.rOther.u4Datain & 0xf0000000) >> 28;
    //  rWMVDecInfo.rPicLayer.u1PicType = WmvAdvPicType[dwTmp].bPicType;
    //  rWMVDecInfo.rOther.u4Datain = dwGetBitStream(WmvAdvPicType[dwTmp].dwBits);
    if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = PVOP;
    else if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = BVOP;
    else if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = IVOP;
    else if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = BIVOP;
    else
        rWMVDecInfo.rPicLayer.u1PicType = SKIPFRAME;

    if (rWMVDecInfo.rPicLayer.u1PicType != SKIPFRAME) // SKIPFRAME does not contain temporal reference
    {
        if (rWMVDecInfo.rSeqLayer.fgTemporalFrmCntr)
        {
            rWMVDecInfo.rPicLayer.i4TemporalRef = dwGetHi(8);
        }
    }

    if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
    {
        if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
        {
            rWMVDecInfo.rOther.u4GetVal = dwGetHi(2);
            rWMVDecInfo.rPicLayer.fgTopFieldFirst
            = (rWMVDecInfo.rOther.u4GetVal & 0x2) >> 31;
            rWMVDecInfo.rPicLayer.fgRepeatFirstField
            = rWMVDecInfo.rOther.u4GetVal & 0x1;
        }
        else
        {
            rWMVDecInfo.rPicLayer.u1RepeatFrameCount = (UINT8)dwGetHi(2);
        }
    }

    if (rWMVDecInfo.rEntryPoint.fgPanScanPresent)
    {
        fgPanScanPresent = dwGetHi(1);
        if (fgPanScanPresent)
        {
            if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
            {
                if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(2 + rWMVDecInfo.rPicLayer.fgRepeatFirstField);
                else
                    dwNumberOfPanScanWindows = 2;
            }
            else
            {
                if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(1 + rWMVDecInfo.rPicLayer.u1RepeatFrameCount);
                else
                    dwNumberOfPanScanWindows = 1;
            }
            for (i = 0; i < dwNumberOfPanScanWindows; i++)
            {
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanHorizOffset
                = dwGetHi(18);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanVertOffset
                = dwGetHi(18);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanWidth
                = dwGetHi(14);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanHeight
                = dwGetHi(14);
            }
        } // fgPanScanPresent
    }

    if (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)
    {
        return (TRUE);
    }

    rWMVDecInfo.rPicLayer.i4RndCtrl = dwGetHi(1);

    if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
    {
        /*fgUVProgressiveSubsampling =*/
        dwGetHi(1);
    }
    if (rWMVDecInfo.rSeqLayer.fgSeqFrameInterpolation)
    {
        /*fgInterpolateCurrentFrame =*/
        dwGetHi(1);
    }

    // Decode B FRACION
    if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
    {
        dwIdx = dwGetHi(3);
        if (dwIdx == 0x7)
        {
            dwIdx = dwGetHi(4);
            if (dwIdx == 0xe) // "Invalid" in VLC
                return INVALID_32;
            rWMVDecInfo.rPicLayer.i4BNumerator = _iNumLongVLC[dwIdx];
            rWMVDecInfo.rPicLayer.i4BDenominator = _iDenLongVLC[dwIdx];
        }
        else
        {
            rWMVDecInfo.rPicLayer.i4BNumerator = _iNumShortVLC[dwIdx];
            rWMVDecInfo.rPicLayer.i4BDenominator = _iDenShortVLC[dwIdx];
        }
        rWMVDecInfo.rPicLayer.i4BFrameReciprocal
        = _iBInverse[rWMVDecInfo.rPicLayer.i4BDenominator - 1];
        if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
            rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
    }

    rWMVDecInfo.rPicLayer.i4PicQtIdx = dwGetHi(5);

    if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAXHALFQP)
    {
        rWMVDecInfo.rPicLayer.fgHalfStep = dwGetHi(1);
    }
    else
        rWMVDecInfo.rPicLayer.fgHalfStep = FALSE;

    if (rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer)
    {
        rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = dwGetHi(1);
    }

    if (!rWMVDecInfo.rEntryPoint.fgExplicitQuantizer)
    {
        if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = TRUE;
            rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;
        }
        else
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = FALSE;
            rWMVDecInfo.rPicLayer.i4StepSize
            = _iStepRemap[rWMVDecInfo.rPicLayer.i4PicQtIdx];
        }
    }
    else
        rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;

    rWMVDecInfo.rPicLayer.prDQuantParam
    = rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer ? rWMVDecInfo.rPicLayer.rDQuantParam3QPDeadzone
      : rWMVDecInfo.rPicLayer.rDQuantParam5QPDeadzone;

    rWMVDecInfo.rPicLayer.i4Overlap = 0;
    if (rWMVDecInfo.rEntryPoint.fgSequenceOverlap
            && (rWMVDecInfo.rPicLayer.u1PicType != BVOP))
    {
        if (rWMVDecInfo.rPicLayer.i4StepSize >= 9)
            rWMVDecInfo.rPicLayer.i4Overlap = 1;
        else if (rWMVDecInfo.rPicLayer.u1PicType == IVOP
                 || rWMVDecInfo.rPicLayer.u1PicType == BIVOP)
            rWMVDecInfo.rPicLayer.i4Overlap = 7; // last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (rWMVDecInfo.rSeqLayer.fgPostProcInfoPresent)
    {
        /*iPpMethod =*/
        dwGetHi(2);
    }

    vSetNewMode();

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        //ACPRED, Bitplane

        //dwTmp = dwGetBitStream(0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iACPRED =*/
            iDecodeBitPlane(5); // "ACPRED", bp_num = 5
        }
        else
        {
            fgHWDecBP(5);
        }


        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        if (rWMVDecInfo.rPicLayer.i4Overlap & 2)
        {
            if (0 == dwGetHi(1))
                rWMVDecInfo.rPicLayer.i4Overlap = 0;
            else if (0 == dwGetHi(1))
                rWMVDecInfo.rPicLayer.i4Overlap = 1;
            else
            {

                //dwTmp = dwGetBitStream(0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iOVERFLAGS = */
                    iDecodeBitPlane(6); // "OVERFLAGS", bp_num = 6
                }
                else
                {
                    fgHWDecBP(6);
                }
                rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
            }
        }

        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);

        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn != 0)
            vDecodeVOPDQuant();
        else
            vSetDefaultDQuantSetting();

        if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
        {
            rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
            rWMVDecInfo.rOther.fgLuminanceWarpTop
            = rWMVDecInfo.rOther.fgLuminanceWarpBottom = FALSE;
        }

    }// End of IVOP, BIVOP
    else if ((rWMVDecInfo.rPicLayer.u1PicType == PVOP)
             || (rWMVDecInfo.rPicLayer.u1PicType == BVOP))
    {
        if (rWMVDecInfo.rEntryPoint.fgExtendedMvMode)
        {
            // MVRANGE0 = 0, MVRANGE1 = 1,  MVRANGE2 = 2, MVRANGE3 = 3
            rWMVDecInfo.rEntryPoint.i4MVRangeIndex = dwGetHi(1); // 0: MVRANGE0;
            if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex)
            {
                rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1); // 1: MVRANGE1
                if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex == 2)
                {
                    rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1); // 2: MVRANGE2,  3: MVRANGE3
                }
            }
        }

        //Start==== decodeVOPHead_WMV3() ===================================

        rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
        rWMVDecInfo.rOther.fgLuminanceWarpTop
        = rWMVDecInfo.rOther.fgLuminanceWarpBottom = FALSE;

        if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
                && (!rWMVDecInfo.rPicLayer.fgFieldMode))
        {
            if (dwGetHi(1))
            {
                rWMVDecInfo.rOther.i4X9MVMode = ALL_1MV;
            }
            else
            {
                rWMVDecInfo.rOther.i4X9MVMode = ALL_1MV_HALFPEL_BILINEAR;
            }

            //dwTmp = dwGetBitStream(0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                /*iDIRECTMB =*/
                iDecodeBitPlane(3); // "DirectMB", bp_num = 3
            }
            else
            {
                fgHWDecBP(3);
            }


            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        }
        else
        {
            rWMVDecInfo.rOther.i4X9MVMode = iGetPMvMode(
                                                rWMVDecInfo.rPicLayer.i4StepSize, FALSE);
            if (rWMVDecInfo.rOther.i4X9MVMode == INTENSITY_COMPENSATION)
            {
                rWMVDecInfo.rOther.fgLuminanceWarp = TRUE;
                rWMVDecInfo.rOther.i4X9MVMode = iGetPMvMode(
                                                    rWMVDecInfo.rPicLayer.i4StepSize, TRUE);
                rWMVDecInfo.rOther.i4LumScale = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumShift = dwGetHi(6);
            }

            if (rWMVDecInfo.rOther.i4X9MVMode == MIXED_MV)
            {
                //dwTmp = dwGetBitStream(0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iMVTYPEMB =*/
                    iDecodeBitPlane(1); //"4mv", bp_num = 1
                }
                else
                {
                    fgHWDecBP(1);
                }

                rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
            }
        }


        //dwTmp = dwGetBitStream(0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iSKIPMB =*/
            iDecodeBitPlane(0); // "SkipMB", bp_num = 0
        }
        else
        {
            fgHWDecBP(0);
        }

        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        rWMVDecInfo.rSMProfile.fgCODFlagOn = TRUE;

        rWMVDecInfo.rOther.i4MvTable = dwGetHi(2);
        rWMVDecInfo.rOther.i4CBPTable = dwGetHi(2);
        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant();
        }

        if (rWMVDecInfo.rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(1);
            if (fgTransTypeMB)
            {
                rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(2);
                rWMVDecInfo.rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                rWMVDecInfo.rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
        }

        //End==== decodeVOPHead_WMV3() ===================================


        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1); // Coding set index = 0, 1, or 2.
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx == 1)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx
        = rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx;

        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);
        //=================================================================
    } // PVOP, BVOP

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        UpdateDCStepSize(rWMVDecInfo.rPicLayer.i4StepSize);
    }

    return (TRUE);
}

UINT32 decodeVOPHeadInterlaceV2(void)
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
#ifdef TEST
    _dwLookAhead = dwGetBitStream(0);
#endif

    rWMVDecInfo.rPicLayer.u1PrevPicType = rWMVDecInfo.rPicLayer.u1PicType;
    // PTYPE
    if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = PVOP;
    else if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = BVOP;
    else if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = IVOP;
    else if (0 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = BIVOP;
    else
        rWMVDecInfo.rPicLayer.u1PicType = SKIPFRAME;

    if (rWMVDecInfo.rPicLayer.u1PicType != SKIPFRAME) // SKIPFRAME does not contain temporal reference
    {
        if (rWMVDecInfo.rSeqLayer.fgTemporalFrmCntr)
            rWMVDecInfo.rPicLayer.i4TemporalRef = dwGetHi(8);
    }

    if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
    {
        if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
        {
            rWMVDecInfo.rOther.u4GetVal = dwGetHi(2);
            rWMVDecInfo.rPicLayer.fgTopFieldFirst
            = (rWMVDecInfo.rOther.u4GetVal & 0x2) >> 1;
            rWMVDecInfo.rPicLayer.fgRepeatFirstField
            = rWMVDecInfo.rOther.u4GetVal & 0x1;
        }
        else
        {
            rWMVDecInfo.rPicLayer.u1RepeatFrameCount = dwGetHi(2);
        }
    }

    if (rWMVDecInfo.rEntryPoint.fgPanScanPresent)
    {
        fgPSPresent = dwGetHi(1);
        if (fgPSPresent)
        {
            if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
            {
                if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(2 + rWMVDecInfo.rPicLayer.fgRepeatFirstField);
                else
                    dwNumberOfPanScanWindows = 2;
            }
            else
            {
                if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(1 + rWMVDecInfo.rPicLayer.u1RepeatFrameCount);
                else
                    dwNumberOfPanScanWindows = 1;
            }
            for (i = 0; i < dwNumberOfPanScanWindows; i++)
            {
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanHorizOffset
                = dwGetHi(18);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanVertOffset
                = dwGetHi(18);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanWidth
                = dwGetHi(14);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanHeight
                = dwGetHi(14);
            }
        } // fgPSPresent
    }

    if (rWMVDecInfo.rPicLayer.u1PicType == SKIPFRAME)
    {
        return (TRUE);
    }

    rWMVDecInfo.rPicLayer.i4RndCtrl = dwGetHi(1);

    if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
    {
        /*fgUVProgressiveSubsampling =*/
        dwGetHi(1);
    }

    rWMVDecInfo.rPicLayer.i4PicQtIdx = dwGetHi(5);

    if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAXHALFQP)
        rWMVDecInfo.rPicLayer.fgHalfStep = dwGetHi(1);
    else
        rWMVDecInfo.rPicLayer.fgHalfStep = FALSE;

    if (rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer)
        rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = dwGetHi(1);

    if (!rWMVDecInfo.rEntryPoint.fgExplicitQuantizer)
    {
        if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = TRUE;
            rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;
        }
        else
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = FALSE;
            rWMVDecInfo.rPicLayer.i4StepSize
            = _iStepRemap[rWMVDecInfo.rPicLayer.i4PicQtIdx];
        }
    }
    else
        rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;

    rWMVDecInfo.rPicLayer.prDQuantParam
    = rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer ? rWMVDecInfo.rPicLayer.rDQuantParam3QPDeadzone
      : rWMVDecInfo.rPicLayer.rDQuantParam5QPDeadzone;

    rWMVDecInfo.rPicLayer.i4Overlap = 0;
    if (rWMVDecInfo.rEntryPoint.fgSequenceOverlap
            && (rWMVDecInfo.rPicLayer.u1PicType != BVOP))
    {
        if (rWMVDecInfo.rPicLayer.i4StepSize >= 9)
            rWMVDecInfo.rPicLayer.i4Overlap = 1;
        else if (rWMVDecInfo.rPicLayer.u1PicType == IVOP
                 || rWMVDecInfo.rPicLayer.u1PicType == BIVOP)
            rWMVDecInfo.rPicLayer.i4Overlap = 7; // last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (rWMVDecInfo.rSeqLayer.fgPostProcInfoPresent)
    {
        /*iPpMethod =*/
        dwGetHi(2);
    }

    // Decode B FRACION
    if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
    {
        dwIdx = dwGetHi(3);
        if (dwIdx == 0x7)
        {
            dwIdx = dwGetHi(4);
            if (dwIdx == 0xe) // "Invalid" in VLC
                return INVALID_32;
            rWMVDecInfo.rPicLayer.i4BNumerator = _iNumLongVLC[dwIdx];
            rWMVDecInfo.rPicLayer.i4BDenominator = _iDenLongVLC[dwIdx];
        }
        else
        {
            rWMVDecInfo.rPicLayer.i4BNumerator = _iNumShortVLC[dwIdx];
            rWMVDecInfo.rPicLayer.i4BDenominator = _iDenShortVLC[dwIdx];
        }
        rWMVDecInfo.rPicLayer.i4BFrameReciprocal
        = _iBInverse[rWMVDecInfo.rPicLayer.i4BDenominator - 1];
        if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
            rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
    }

    vSetNewMode();

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        //FIELDTX, Bitplane
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        //dwTmp = dwGetBitStream(0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iFIELDTX =*/
            iDecodeBitPlane(4); // "FIELDTX", bp_num = 4
        }
        else
        {
            fgHWDecBP(4);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        //ACPRED, Bitplane
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        //dwTmp = dwGetBitStream(0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iACPRED =*/
            iDecodeBitPlane(5); // "ACPRED", bp_num = 5
        }
        else
        {
            fgHWDecBP(5);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        if (rWMVDecInfo.rPicLayer.i4Overlap & 2)
        {
            if (0 == dwGetHi(1))
                rWMVDecInfo.rPicLayer.i4Overlap = 0;
            else if (0 == dwGetHi(1))
                rWMVDecInfo.rPicLayer.i4Overlap = 1;
            else
            {
                //OVERFLAGS, Bitplane
#ifdef TEST
                _dwLookAhead = dwGetBitStream(0);
#endif
                //dwTmp = dwGetBitStream(0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iOVERFLAGS =*/
                    iDecodeBitPlane(6); // "OVERFLAGS", bp_num = 6
                }
                else
                {
                    fgHWDecBP(6);
                }
#ifdef TEST
                _dwLookAhead = dwGetBitStream(0);
#endif
                rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
            }
        }

        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx += dwGetHi(1);
        }

        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);

        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn != 0)
            vDecodeVOPDQuant();
        else
            vSetDefaultDQuantSetting();

    }// End of IVOP, BIVOP
    else if ((rWMVDecInfo.rPicLayer.u1PicType == PVOP)
             || (rWMVDecInfo.rPicLayer.u1PicType == BVOP))
    {
        UINT32 iMBModeIndex;

        if (rWMVDecInfo.rEntryPoint.fgExtendedMvMode)
        {
            // MVRANGE0 = 0, MVRANGE1 = 1,  MVRANGE2 = 2, MVRANGE3 = 3
            rWMVDecInfo.rEntryPoint.i4MVRangeIndex = dwGetHi(1); // 0: MVRANGE0;
            if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex)
            {
                rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1); // 1: MVRANGE1
                if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex == 2)
                {
                    rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1); // 2: MVRANGE2,  3: MVRANGE3
                }
            }
        }

        if (rWMVDecInfo.rEntryPoint.fgExtendedDeltaMvMode)
        {
            rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex = dwGetHi(1);
            if (rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex)
            {
                rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(1);
                if (rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex == 2)
                {
                    rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(1);
                }
            }
            rWMVDecInfo.rEntryPoint.i4ExtendedDMVX
            = rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex & 1;
            rWMVDecInfo.rEntryPoint.i4ExtendedDMVY
            = (rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex & 2) >> 1;
        }

        //    rWMVDecInfo.rOther.fgLuminanceWarpTop = rWMVDecInfo.rOther.fgLuminanceWarpBottom = FALSE;
        //4MVSWITCH
        if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            rWMVDecInfo.rOther.i4X9MVMode = ALL_1MV; //Interlaced Frame B picture has only ALL_1MV mode.
        else
        {
            if (dwGetHi(1))
                rWMVDecInfo.rOther.i4X9MVMode = MIXED_MV;
            else
                rWMVDecInfo.rOther.i4X9MVMode = ALL_1MV;
        }

        //INTCOMP
        rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
        if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
        {
            dwGetHi(1); //No matter what value this bit is,
            rWMVDecInfo.rOther.fgLuminanceWarp = FALSE; //Shall always be FALSE for Interlaced Frame B picture.
        }
        else //PVOP
        {
            if (dwGetHi(1))
            {
                rWMVDecInfo.rOther.fgLuminanceWarp = TRUE;
            }
        }
        if (rWMVDecInfo.rOther.fgLuminanceWarp)
        {
            rWMVDecInfo.rOther.i4LumScale = dwGetHi(6);
            rWMVDecInfo.rOther.i4LumShift = dwGetHi(6);
        }

        if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
        {
            //DIRECTMB, Bitplane
#ifdef TEST
            _dwLookAhead = dwGetBitStream(0);
#endif
            //dwTmp = dwGetBitStream(0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                /*iDIRECTMB =*/
                iDecodeBitPlane(3); //"DIRECTMB", bp_num = 3
            }
            else
            {
                fgHWDecBP(3);
            }
#ifdef TEST
            _dwLookAhead = dwGetBitStream(0);
#endif
            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        }

        //SKIPMB, Bitplane
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        //dwTmp = dwGetBitStream(0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iSKIPMB =*/
            iDecodeBitPlane(0); // "SKIPMB", bp_num = 0
        }
        else
        {
            fgHWDecBP(0);
        }

        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        rWMVDecInfo.rSMProfile.fgCODFlagOn = TRUE;

        iMBModeIndex = dwGetHi(2);

        if (rWMVDecInfo.rOther.i4X9MVMode == MIXED_MV)
        {
            rWMVDecInfo.rOther.i4MBModeTable = iMBModeIndex; //ming add
        }
        else
        {
            rWMVDecInfo.rOther.i4MBModeTable = 4 + (iMBModeIndex & 0x3); //ming add
        }
        rWMVDecInfo.rOther.i4MvTable = dwGetHi(2);
        rWMVDecInfo.rOther.i4CBPTable = dwGetHi(3);
        rWMVDecInfo.rOther.i42MVBPTable = dwGetHi(2);

        if ((rWMVDecInfo.rOther.i4X9MVMode == MIXED_MV)
                || (rWMVDecInfo.rPicLayer.u1PicType == BVOP))
        {
            rWMVDecInfo.rOther.i44MVBPTable = dwGetHi(2);
        }

        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant();
        }

        if (rWMVDecInfo.rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(1);
            if (fgTransTypeMB)
            {
                rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(2);
                rWMVDecInfo.rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                rWMVDecInfo.rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
        }

        //End==== decodeVOPHead_WMV3() ===================================

        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1); // Coding set index = 0, 1, or 2.
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx == 1)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx
        = rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx;

        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);
        //=================================================================
    } // PVOP, BVOP

    //  if((rWMVDecInfo.rPicLayer.u1PicType == IVOP) || (rWMVDecInfo.rPicLayer.u1PicType == PVOP))
    //  {
    //    UpdateDCStepSize(rWMVDecInfo.rPicLayer.i4StepSize);
    //  }
    return (TRUE);
}

UINT32 decodeVOPHeadFieldPicture(void)
{
    UINT32 iCode;
    BOOL fgPSPresent;
    UINT32 i, dwNumberOfPanScanWindows;
    //BOOL fgUVProgressiveSubsampling;

    iCode = dwGetHi(3);

    rWMVDecInfo.rPicLayer.u1FirstFieldType = s_vopFirstFieldType[iCode];
    rWMVDecInfo.rPicLayer.u1SecondFieldType = s_vopSecondFieldType[iCode];

    if (rWMVDecInfo.rPicLayer.u1PicType != SKIPFRAME) // SKIPFRAME does not contain temporal reference
    {
        if (rWMVDecInfo.rSeqLayer.fgTemporalFrmCntr)
            rWMVDecInfo.rPicLayer.i4TemporalRef = dwGetHi(8);
    }

    if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
    {
        if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
        {
            rWMVDecInfo.rOther.u4GetVal = dwGetHi(2);
            rWMVDecInfo.rPicLayer.fgTopFieldFirst = (rWMVDecInfo.rOther.u4GetVal & 0x2) >> 1;
            rWMVDecInfo.rPicLayer.fgRepeatFirstField = rWMVDecInfo.rOther.u4GetVal & 0x1;
        }
        else
        {
            rWMVDecInfo.rPicLayer.u1RepeatFrameCount = dwGetHi(2);
        }
    }

    if (rWMVDecInfo.rEntryPoint.fgPanScanPresent)
    {
        fgPSPresent = dwGetHi(1);
        if (fgPSPresent)
        {
            if (rWMVDecInfo.rSeqLayer.fgInterlacedSource)
            {
                if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(2 + rWMVDecInfo.rPicLayer.fgRepeatFirstField);
                else
                    dwNumberOfPanScanWindows = 2;
            }
            else
            {
                if (rWMVDecInfo.rSeqLayer.fgBroadcastFlags)
                    dwNumberOfPanScanWindows = (UINT32)(1 + rWMVDecInfo.rPicLayer.u1RepeatFrameCount);
                else
                    dwNumberOfPanScanWindows = 1;
            }
            for (i = 0; i < dwNumberOfPanScanWindows; i++)
            {
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanHorizOffset
                = dwGetHi(18);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanVertOffset
                = dwGetHi(18);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanWidth
                = dwGetHi(14);
                rWMVDecInfo.rPicLayer.sWindowInfo[i].dwPanScanHeight
                = dwGetHi(14);
            }
        } // fgPSPresent
    }

    rWMVDecInfo.rPicLayer.i4RndCtrl = dwGetHi(1);
    /*fgUVProgressiveSubsampling =*/
    dwGetHi(1);

    if (rWMVDecInfo.rEntryPoint.fgRefDistPresent
            && ((rWMVDecInfo.rPicLayer.u1FirstFieldType == IVOP)
                || (rWMVDecInfo.rPicLayer.u1FirstFieldType == PVOP)))
    {
        rWMVDecInfo.rEntryPoint.i4RefFrameDistance = dwGetHi(2);
        if (rWMVDecInfo.rEntryPoint.i4RefFrameDistance == 3)
        {
            while (dwGetHi(1))
            {
                rWMVDecInfo.rEntryPoint.i4RefFrameDistance++;
            }
        }
    }

    // Decode B frac
    if ((rWMVDecInfo.rPicLayer.u1FirstFieldType == BVOP)
            || (rWMVDecInfo.rPicLayer.u1FirstFieldType == BIVOP))
    {
        UINT32 iShort = 0;
        UINT32 iLong = 0;
        iShort = dwGetHi(3);
        if (iShort == 0x7)
        {
            iLong = dwGetHi(4);
            if (iLong == 0xe) // "Invalid" in VLC
                return INVALID_32;
            if (iLong == 0xf)
                rWMVDecInfo.rPicLayer.u1PicType = BIVOP;
            else
            {
                rWMVDecInfo.rPicLayer.i4BNumerator = _iNumLongVLC[iLong];
                rWMVDecInfo.rPicLayer.i4BDenominator = _iDenLongVLC[iLong];
                rWMVDecInfo.rPicLayer.i4BFrameReciprocal
                = _iBInverse[rWMVDecInfo.rPicLayer.i4BDenominator - 1];
                if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
                    rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
            }
        }
        else
        {
            rWMVDecInfo.rPicLayer.i4BNumerator = _iNumShortVLC[iShort];
            rWMVDecInfo.rPicLayer.i4BDenominator = _iDenShortVLC[iShort];
            rWMVDecInfo.rPicLayer.i4BFrameReciprocal
            = _iBInverse[rWMVDecInfo.rPicLayer.i4BDenominator - 1];
            if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
                rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
        }
    }
    return TRUE;
}

UINT32 decodeFieldHeadFieldPicture(void)
{
    //int iPpMethod;
    //int iACPRED;
    //int iOVERFLAGS;
    //int iFORWARDMB;
    BOOL fgTransTypeMB;
    UINT32 iTransTypeFrame;

    rWMVDecInfo.rPicLayer.i4PicQtIdx = dwGetHi(5);

    if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAXHALFQP)
        rWMVDecInfo.rPicLayer.fgHalfStep = dwGetHi(1);
    else
        rWMVDecInfo.rPicLayer.fgHalfStep = FALSE;

    if (rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer)
        rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = dwGetHi(1);

    if (!rWMVDecInfo.rEntryPoint.fgExplicitQuantizer)
    {
        if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = TRUE;
            rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;
        }
        else
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = FALSE;
            rWMVDecInfo.rPicLayer.i4StepSize
            = _iStepRemap[rWMVDecInfo.rPicLayer.i4PicQtIdx];
        }
    }
    else
        // Explicit quantizer
        rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;

    rWMVDecInfo.rPicLayer.prDQuantParam
    = rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer ? rWMVDecInfo.rPicLayer.rDQuantParam3QPDeadzone
      : rWMVDecInfo.rPicLayer.rDQuantParam5QPDeadzone;

    rWMVDecInfo.rPicLayer.i4Overlap = 0;
    if (rWMVDecInfo.rEntryPoint.fgSequenceOverlap
            && (rWMVDecInfo.rPicLayer.u1PicType != BVOP))
    {
        if (rWMVDecInfo.rPicLayer.i4StepSize >= 9)
            rWMVDecInfo.rPicLayer.i4Overlap = 1;
        else if (rWMVDecInfo.rPicLayer.u1PicType == IVOP
                 || rWMVDecInfo.rPicLayer.u1PicType == BIVOP)
            rWMVDecInfo.rPicLayer.i4Overlap = 7; // last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (rWMVDecInfo.rSeqLayer.fgPostProcInfoPresent)
    {
        //spork
        /*iPpMethod =*/
        dwGetHi(2);
    }

    vSetNewMode();

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        //ming modify@2006/4/12
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        //ACPRED, Bitplane
        if (cbWMVFWBP)
        {
            //spork
            /*iACPRED =*/
            iDecodeBitPlane(5); // "ACPRED", bp_num = 5
        }
        else
        {
            fgHWDecBP(5);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif

        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        if (rWMVDecInfo.rPicLayer.i4Overlap & 2)
        {
            if (0 == dwGetHi(1))
                rWMVDecInfo.rPicLayer.i4Overlap = 0;
            else if (0 == dwGetHi(1))
                rWMVDecInfo.rPicLayer.i4Overlap = 1;
            else
            {
#ifdef TEST
                _dwLookAhead = dwGetBitStream(0);
#endif
                //ming modify@2006/4/12
                //OVERFLAGS, Bitplane
                if (cbWMVFWBP)
                {
                    //spork
                    /*iOVERFLAGS =*/
                    iDecodeBitPlane(6); // "OVERFLAGS", bp_num = 6
                }
                else
                {
                    fgHWDecBP(6);
                }
#ifdef TEST
                _dwLookAhead = dwGetBitStream(0);
#endif
                rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
            }
        }

        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx += dwGetHi(1);
        }

        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);

        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn != 0)
            vDecodeVOPDQuant();
        else
            vSetDefaultDQuantSetting();

        if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
        {
            rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
            rWMVDecInfo.rOther.fgLuminanceWarpTop
            = rWMVDecInfo.rOther.fgLuminanceWarpBottom = FALSE;
        }

    }// End of IVOP, BIVOP
    else if ((rWMVDecInfo.rPicLayer.u1PicType == PVOP)
             || (rWMVDecInfo.rPicLayer.u1PicType == BVOP))
    {
        BOOL fgUseMostRecentFieldForRef;

        //NUMREF
        if (rWMVDecInfo.rPicLayer.u1PicType != BVOP)
            rWMVDecInfo.rPicLayer.fgTwoRefPictures = dwGetHi(1);
        else
            rWMVDecInfo.rPicLayer.fgTwoRefPictures = TRUE; // For BVOP, rWMVDecInfo.rPicLayer.fgTwoRefPictures is always TRUE;

        if (rWMVDecInfo.rPicLayer.fgTwoRefPictures)
        {
            rWMVDecInfo.rPicLayer.fgUseSameFieldForRef = TRUE;
            rWMVDecInfo.rPicLayer.fgUseOppFieldForRef = TRUE;
        }
        else
        {
            fgUseMostRecentFieldForRef = !dwGetHi(1);
            if (fgUseMostRecentFieldForRef)
            {
                rWMVDecInfo.rPicLayer.fgUseSameFieldForRef = FALSE;
                rWMVDecInfo.rPicLayer.fgUseOppFieldForRef = TRUE;
            }
            else
            {
                rWMVDecInfo.rPicLayer.fgUseSameFieldForRef = TRUE;
                rWMVDecInfo.rPicLayer.fgUseOppFieldForRef = FALSE;
            }
        }

        if (rWMVDecInfo.rEntryPoint.fgExtendedMvMode)
        {
            // MVRANGE0 = 0, MVRANGE1 = 1,  MVRANGE2 = 2, MVRANGE3 = 3
            rWMVDecInfo.rEntryPoint.i4MVRangeIndex = dwGetHi(1); // 0: MVRANGE0;
            if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex)
            {
                rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1); // 1: MVRANGE1
                if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex == 2)
                {
                    rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1); // 2: MVRANGE2,  3: MVRANGE3
                }
            }
        }

        if (rWMVDecInfo.rEntryPoint.fgExtendedDeltaMvMode)
        {
            rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex = dwGetHi(1);
            if (rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex)
            {
                rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(1);
                if (rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex == 2)
                {
                    rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex += dwGetHi(1);
                }
            }
            rWMVDecInfo.rEntryPoint.i4ExtendedDMVX
            = rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex & 1;
            rWMVDecInfo.rEntryPoint.i4ExtendedDMVY
            = (rWMVDecInfo.rEntryPoint.i4DeltaMVRangeIndex & 2) >> 1;
        }

        rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
        rWMVDecInfo.rOther.fgLuminanceWarpTop
        = rWMVDecInfo.rOther.fgLuminanceWarpBottom = FALSE;

        //MVMODE
        rWMVDecInfo.rOther.i4X9MVMode = iGetIFMvMode(
                                            rWMVDecInfo.rPicLayer.i4StepSize, FALSE);
        if (rWMVDecInfo.rOther.i4X9MVMode == INTENSITY_COMPENSATION) //PVOP only.
        {
            rWMVDecInfo.rOther.fgLuminanceWarp = TRUE;
            rWMVDecInfo.rOther.i4X9MVMode = iGetIFMvMode(
                                                rWMVDecInfo.rPicLayer.i4StepSize, TRUE);
            if (dwGetHi(1))
            {
                // Both reference field remapped
                rWMVDecInfo.rOther.fgLuminanceWarpTop
                = rWMVDecInfo.rOther.fgLuminanceWarpBottom = TRUE;
                rWMVDecInfo.rOther.i4LumScaleTop = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumShiftTop = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumScaleBottom = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumShiftBottom = dwGetHi(6);
            }
            else if (dwGetHi(1))
            {
                // Bottom reference field remapped
                rWMVDecInfo.rOther.fgLuminanceWarpBottom = TRUE;
                rWMVDecInfo.rOther.i4LumScaleBottom = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumShiftBottom = dwGetHi(6);
            }
            else
            {
                // Top reference field remapped
                rWMVDecInfo.rOther.fgLuminanceWarpTop = TRUE;
                rWMVDecInfo.rOther.i4LumScaleTop = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumShiftTop = dwGetHi(6);
            }
        }

        if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
        {
            //ming modify@2006/4/12
            //FORWARDMB
            if (cbWMVFWBP)
            {
                /*iFORWARDMB =*/
                iDecodeBitPlane(7); //"FORWARDMB", bp_num = 7
            }
            else
            {
                fgHWDecBP(7);
            }
            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        }

        rWMVDecInfo.rSMProfile.fgCODFlagOn = TRUE;
        // read MV and CBP codetable indices
        rWMVDecInfo.rOther.i4MBModeTable = dwGetHi(3);
        rWMVDecInfo.rOther.i4MvTable
        = (rWMVDecInfo.rPicLayer.fgTwoRefPictures) ? dwGetHi(3)
          : dwGetHi(2);
        rWMVDecInfo.rOther.i4CBPTable = dwGetHi(3);
        if (rWMVDecInfo.rOther.i4X9MVMode == MIXED_MV)
        {
            rWMVDecInfo.rOther.i44MVBPTable = dwGetHi(2);
        }

        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant();
        }

        if (rWMVDecInfo.rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(1);
            if (fgTransTypeMB)
            {
                rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(2);
                rWMVDecInfo.rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                rWMVDecInfo.rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
        }

        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1); // Coding set index = 0, 1, or 2.
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx == 1)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx
        = rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx;

        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);
    } // PVOP, BVOP

    //  if((rWMVDecInfo.rPicLayer.u1PicType == PVOP) || (rWMVDecInfo.rPicLayer.u1PicType == BVOP))
    {
        UpdateDCStepSize(rWMVDecInfo.rPicLayer.i4StepSize);
    }

    return (TRUE);
}

void SetupFieldPictureMVScaling(UINT32 iRefFrameDistance)
{

    if (iRefFrameDistance > 3)
        iRefFrameDistance = 3;

    if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0)
    {
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iFarFieldScale1;
        rWMVDecInfo.rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iFarFieldScale2;
        rWMVDecInfo.rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesFirstField[iRefFrameDistance].iNearFieldScale;
    }
    else
    {
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iFarFieldScale1;
        rWMVDecInfo.rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iFarFieldScale2;
        rWMVDecInfo.rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesSecondField[iRefFrameDistance].iNearFieldScale;
    }
}

void SetupForwardBFieldPictureMVScaling(UINT32 iRefFrameDistance)
{
    if (iRefFrameDistance > 3)
        iRefFrameDistance = 3;
    if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0)
    {
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale1;
        rWMVDecInfo.rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale2;
        rWMVDecInfo.rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iNearFieldScale;
    }
    else
    {
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4FarFieldScale1
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale1;
        rWMVDecInfo.rPicLayer.i4FarFieldScale2
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale2;
        rWMVDecInfo.rPicLayer.i4NearFieldScale
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iNearFieldScale;
    }
}

void SetupBackwardBFieldPictureMVScaling(UINT32 iRefFrameDistance)
{
    if (iRefFrameDistance > 3)
        iRefFrameDistance = 3;
    if (rWMVDecInfo.rPicLayer.i4CurrentTemporalField == 0)
    {
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVX
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVY
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4FarFieldScaleBack1
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale1;
        rWMVDecInfo.rPicLayer.i4FarFieldScaleBack2
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iFarFieldScale2;
        rWMVDecInfo.rPicLayer.i4NearFieldScaleBack
        = s_sMVScaleValuesFirstFieldB[iRefFrameDistance].iNearFieldScale;
    }
    else
    {
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4MaxZone1ScaledFarBackMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iMaxZone1ScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVX
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVX;
        rWMVDecInfo.rPicLayer.i4Zone1OffsetScaledFarBackMVY
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iZone1OffsetScaledFarMVY;
        rWMVDecInfo.rPicLayer.i4FarFieldScaleBack1
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale1;
        rWMVDecInfo.rPicLayer.i4FarFieldScaleBack2
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iFarFieldScale2;
        rWMVDecInfo.rPicLayer.i4NearFieldScaleBack
        = s_sMVScaleValuesSecondFieldB[iRefFrameDistance].iNearFieldScale;
    }
}

void decodePFieldMode(void)
{
    BOOL fgHalfPelMode = ((rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL)
                          || (rWMVDecInfo.rOther.i4X9MVMode == ALL_1MV_HALFPEL_BILINEAR));

    // Set MV state for B field direct mode
    if (rWMVDecInfo.rPicLayer.i4CurrentField == 0)
        rWMVDecInfo.rPicLayer.fgBackRefTopFieldHalfPelMode = fgHalfPelMode;
    else
        rWMVDecInfo.rPicLayer.fgBackRefBottomFieldHalfPelMode = fgHalfPelMode;
}

void decodeBFieldMode(void)
{
    if (rWMVDecInfo.rPicLayer.i4CurrentField == 0)
        rWMVDecInfo.rPicLayer.fgBackRefUsedHalfPel
        = rWMVDecInfo.rPicLayer.fgBackRefTopFieldHalfPelMode;
    else
        rWMVDecInfo.rPicLayer.fgBackRefUsedHalfPel
        = rWMVDecInfo.rPicLayer.fgBackRefBottomFieldHalfPelMode;
}

//extern unsigned long rWMVDecInfo.rOther.u4BPRawFlag;
int iDecodeBitPlane(int iBpNum)
{
    BOOL fgInvert;
    UINT8 bBPCodingMode;
    UINT8 bSymbol;
    int i, j;
    BOOL fgDiff=FALSE;
    UINT64 hyTmp;
    UINT32 dwWrSa = 0 ;
    //UINT32 *ptHpAddr;
    //UINT8 *pbPtr;
    //spork
    UINT32 dwCount;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
    unsigned long dwTmp = (dwGetBitStream(0) >> 27) & 0xF;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
    rWMVDecInfo.rOther.u4VldPosByte = dwVldRPtr(&rWMVDecInfo.rOther.u4VldPosBit);

    if (dwTmp == 0)
    {
        switch (iBpNum)
        {
        case 0: //SKIP
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x1;
            break;
        case 1: //4MV
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x2;
            break;
        case 3: //DIRECT
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x4;
            break;
        case 4: //FIELD
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x8;
            break;
        case 5: //ACPRED
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x10;
            break;
        case 6: //OVERLAP
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x20;
            break;
        case 7: //FORWARD
            rWMVDecInfo.rOther.u4BPRawFlag |= 0x40;
            break;
        default:
            rWMVDecInfo.rOther.u4BPRawFlag = 0;
            break;
        }
    }
    dwCount = 0;

    before_bp();

    fgInvert = ((rWMVDecInfo.rOther.u4Datain >> (32-(dwCount +1))) & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
    dwCount++;
    bBPCodingMode = ((rWMVDecInfo.rOther.u4Datain >> (32-(dwCount +2))) & 0x03);//((dwGetHi(0) >> (dwCount)) & (0x03));// VCL
    dwCount = dwCount + 2;
    // reset bitplane mask table.
    for (i = 0; i < MAX_MBY; i++)
    {
        for (j = 0; j < MAX_MBX; j++)
        {
            rWMVDecInfo.rOther.u1BPMask[i][j] = 0;
        }
        rWMVDecInfo.rOther.hyBP[i] = 0;
    }

    vSetNewMode();

    switch (bBPCodingMode)
    {
    case 2: // Norm-2
        bBPCodingMode = NORM2;
        //Printf("NORM2, bp %d\n", iBpNum&0x3);
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream((UINT8)dwCount);
        vDecBPNorm2Diff2();
        break;
    case 3: // Norm-6
        bBPCodingMode = NORM6;
        //Printf("NORM6, bp %d\n", iBpNum&0x3);
        // ilii test
        //jump_bp();
        //ilii undo it
        if (1)
        {
            {
                rWMVDecInfo.rOther.i4PicHdrBits -= 5;
                rWMVDecInfo.rOther.fgCounting = TRUE;
            }
            fgHWDecBP(iBpNum);
            return 0;
        }
        //spork
        /*
         rWMVDecInfo.rOther.u4Datain = dwGetBitStream((UINT8)dwCount);
         if(!fgDecodeNorm6Diff6())
         {
         //Printf("===Error  Norm-6\n");
         return FALSE;
         } */
        //break;
    case 1:
        bBPCodingMode = ((rWMVDecInfo.rOther.u4Datain >> (32-(dwCount +1)))
                         & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
        dwCount++;
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(dwCount);
        // reset bitplane mask table.
        if (bBPCodingMode)
        {   // Colskip
            bBPCodingMode = COLSKIP;
            //Printf("COLSKIP, bp %d\n", iBpNum&0x3);
            for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
            {
                if (dwGetHi(1)) // this column has nonzero values
                {
                    for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
                    {
                        bSymbol = dwGetHi(1);
                        rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
                    }
                }
                else // this column are all zeros.
                {
                    for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
                    {
                        bSymbol = 0;
                        rWMVDecInfo.rOther.u1BPMask[i][j] = 0;
                    }
                }
            }
        }
        else
        {   // Rowskip
            bBPCodingMode = ROWSKIP;
            LOG(7, "ROWSKIP, bp %d\n", iBpNum&0x3);

            for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
            {
                if (dwGetHi(1)) // this row has nonzero values
                {
                    for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
                    {
                        bSymbol = dwGetHi(1);
                        rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
                    }
                }
                else
                {
                    for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
                    {
                        bSymbol = 0;
                        rWMVDecInfo.rOther.u1BPMask[i][j] = 0;
                    }
                }
            }
        }
        break;
    default:
        bBPCodingMode = ((rWMVDecInfo.rOther.u4Datain >> (32-(dwCount +1)))
                         & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
        dwCount++;
        if (bBPCodingMode)
        {   // Diff-2
            bBPCodingMode = DIFF2;
            LOG(7,"DIFF2, bp %d\n", iBpNum&0x3);
            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(dwCount);
            fgDiff = TRUE;
            vDecBPNorm2Diff2();
        }
        else
        {
            bBPCodingMode = ((rWMVDecInfo.rOther.u4Datain >> (32-(dwCount +1)))
                             & 0x01);//((dwGetHi(0) >> (dwCount)) & (0x01));
            dwCount++;
            if (bBPCodingMode)
            {   // Diff-6
                bBPCodingMode = DIFF6;
                LOG(7, "DIFF6, bp %d\n", iBpNum&0x3);
                // ilii
                bBPCodingMode = RAW;
                //jump_bp();
                //ilii undo it
                {
                    rWMVDecInfo.rOther.i4PicHdrBits -= 5;
                    rWMVDecInfo.rOther.fgCounting = TRUE;
                }
                fgHWDecBP(iBpNum);
                return 0;
                //spork
                /*
                 rWMVDecInfo.rOther.u4Datain = dwGetBitStream((UINT8)dwCount);
                 fgDiff = TRUE;
                 if(!fgDecodeNorm6Diff6())
                 {
                 return FALSE;
                 }
                 #ifdef TEST
                 _dwLookAhead = dwGetBitStream(0);
                 #endif*/
            }
            else
            {   // Raw
                //ilii rWMVDecInfo.rOther.u4Datain = dwGetBitStream(dwCount);
                LOG(6, "=== Error? Raw?, bp %d\n", iBpNum&0x3);
                bBPCodingMode = RAW;
                //jump_bp();
                //ilii undo it
                {
                    rWMVDecInfo.rOther.i4PicHdrBits -= 5;
                    rWMVDecInfo.rOther.fgCounting = TRUE;
                }

                fgHWDecBP(iBpNum);
                return (bBPCodingMode);
            }
        }
        break;
    }

    if (fgDiff)
    {   // undo differential coding
        // Hardware should take care of this.
        //int iPred;
        /*
         for (i = 0; i < rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
         {
         for (j = 0; j < rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
         {
         if((0 == i) && (0 == j))
         {
         iPred = fgInvert;
         }
         else if(0 == j)
         {
         iPred = rWMVDecInfo.rOther.u1BPMask[i-1][j];
         }
         else if((0 < i) && (rWMVDecInfo.rOther.u1BPMask[i][j-1] != rWMVDecInfo.rOther.u1BPMask[i-1][j]))
         {
         iPred = fgInvert;
         }
         else
         {
         iPred = rWMVDecInfo.rOther.u1BPMask[i][j-1];
         }

         rWMVDecInfo.rOther.u1BPMask[i][j] = (rWMVDecInfo.rOther.u1BPMask[i][j] ^ iPred);
         }
         }
         */
#if 0
        for (i = 0; i < rWMVDecInfo.rSeqHdrR.u4NumMBY && i<MAX_MBY; i++)
        {
            for (j = 0; j < rWMVDecInfo.rSeqHdrR.u4NumMBX && j<MAX_MBX; j++)
            {
                if ((i >= MAX_MBY) || (j >= MAX_MBX))
                {
                    LOG(3,"iDecodeBitPlane waring , (i >= MAX_MBY) || (j >= MAX_MBX) \n");
                    ASSERT(!((i >= MAX_MBY) || (j >= MAX_MBX)));
                    return 0;
                }
                if ((i + j) == 0)
                    iPred = fgInvert;
                else if (i == 0)
                    iPred = rWMVDecInfo.rOther.u1BPMask[i][j-1];//(pmb1 - 1)->m_bSkip;
                else if (j == 0)
                    iPred = rWMVDecInfo.rOther.u1BPMask[i-1][j];//(pmb1 - rWMVDecInfo.rSeqHdrR.u4NumMBX)->m_bSkip;
                else
                {
                    if (rWMVDecInfo.rOther.u1BPMask[i][j-1]
                            == rWMVDecInfo.rOther.u1BPMask[i-1][j])
                        iPred = rWMVDecInfo.rOther.u1BPMask[i][j-1];
                    else
                        iPred = fgInvert;
                }
                rWMVDecInfo.rOther.u1BPMask[i][j] ^= iPred;
            }
        }
#endif
    }
    else
    {
        if (fgInvert)
        {
            for (i = 0; i < rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
            {
                for (j = 0; j < rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
                {
                    rWMVDecInfo.rOther.u1BPMask[i][j] ^= 1;
                }
            }
        }
    }
    if (bBPCodingMode != RAW)
    {
        //Select Bp buffer
        switch (iBpNum & 0x3)
        {
        case 0:
            dwWrSa = (UINT32)rWMVDecInfo.rDram.pu1Bp_1;
            break;
        case 1:
            dwWrSa = (UINT32)rWMVDecInfo.rDram.pu1Bp_2;
            break;
        case 2:
            dwWrSa = (UINT32)rWMVDecInfo.rDram.pu1Bp_3;
            break;
        case 3:
            dwWrSa = (UINT32)rWMVDecInfo.rDram.pu1Bp_4;
            break;
        }
        //dwWrAddr = dwWrSa >> 4;
        // reconstruct bitplane to rWMVDecInfo.rOther.hyBP[]

        for (i = 0; i < (int)MAX_MBY; i++) // for each MB-row
        {

            rWMVDecInfo.rOther.hyBP[2*i+1] = 0;
            for (j = (int)MAX_MBX/2; j < (int)MAX_MBX; j++)
            {
                if (rWMVDecInfo.rOther.u1BPMask[i][j] != 0)
                {
                    hyTmp = (UINT64)rWMVDecInfo.rOther.u1BPMask[i][j];
                    rWMVDecInfo.rOther.hyBP[2*i+1] += hyTmp << ((BP_MB_BITS-1)
                                                      - (j - (int)MAX_MBX/2));
                }
            }
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 0)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 8)&0xFF));
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 16)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 24)&0xFF) );
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 32)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 40)&0xFF));
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 48)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1]
                                             >> 56)&0xFF) );
            dwWrSa++;

            rWMVDecInfo.rOther.hyBP[2*i] = 0;
            for (j = 0; j < (int)MAX_MBX/2; j++)
            {
                if (rWMVDecInfo.rOther.u1BPMask[i][j] != 0)
                {
                    hyTmp = (UINT64)rWMVDecInfo.rOther.u1BPMask[i][j];
                    rWMVDecInfo.rOther.hyBP[2*i] += hyTmp << ((BP_MB_BITS-1)
                                                    - j);
                }
            }
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 0)
                                            &0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 8)
                                            &0xFF));
            dwWrSa++;
            WriteDram8b(dwWrSa, 0,
                        (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 16)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0,
                        (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 24)&0xFF) );
            dwWrSa++;
            WriteDram8b(dwWrSa, 0,
                        (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 32)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0,
                        (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 40)&0xFF));
            dwWrSa++;
            WriteDram8b(dwWrSa, 0,
                        (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 48)&0xFF)); //Low UINT32
            dwWrSa++;
            WriteDram8b(dwWrSa, 0,
                        (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 56)&0xFF) );
            dwWrSa++;

        }
#ifdef CC_MT5395
        HalFlushDCacheMultipleLine(rWMVDecInfo.rPesInfo.u4FifoStart, rWMVDecInfo.rPesInfo.u4FifoEnd - rWMVDecInfo.rPesInfo.u4FifoStart);
#else
        HalFlushInvalidateDCache();
#endif
    }

    after_bp();

    return (bBPCodingMode);
}

void vDecBPNorm2Diff2(void)
{
    UINT32 i;
    UINT32 dwNumMB = rWMVDecInfo.rSeqHdrR.u4NumMBX
                     * rWMVDecInfo.rSeqHdrR.u4NumMBY;
    UINT8 bSymbol2n, bSymbol2n_1;
    UINT32 dwCurrMBX, dwCurrMBY;
    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (dwNumMB & 0x1)
    {
        rWMVDecInfo.rOther.u1BPMask[0][0] = dwGetHi(1); // If rowMB กั colMB is odd, the first symbol is encoded raw.
    }

    for (i = (dwNumMB & 0x1); i < dwNumMB; i += 2)
    {
        if (dwGetHi(1))
        {
            if (dwGetHi(1))
            {   // Codeword = 11b
                bSymbol2n = 1;
                bSymbol2n_1 = 1;
            }
            else
            {
                if (dwGetHi(1))
                {   // Codeword = 101b
                    bSymbol2n = 0;
                    bSymbol2n_1 = 1;
                }
                else
                {   // Codeword = 100b
                    bSymbol2n = 1;
                    bSymbol2n_1 = 0;
                }
            }
        }
        else
        {   // Codeword = 0b
            bSymbol2n = 0;
            bSymbol2n_1 = 0;
        }
        dwCurrMBX = i%rWMVDecInfo.rSeqHdrR.u4NumMBX;
        dwCurrMBY = i/rWMVDecInfo.rSeqHdrR.u4NumMBX;
        rWMVDecInfo.rOther.u1BPMask[dwCurrMBY][dwCurrMBX] = bSymbol2n;
        dwCurrMBX = (i+1)%rWMVDecInfo.rSeqHdrR.u4NumMBX;
        dwCurrMBY = (i+1)/rWMVDecInfo.rSeqHdrR.u4NumMBX;
        rWMVDecInfo.rOther.u1BPMask[dwCurrMBY][dwCurrMBX] = bSymbol2n_1;
    }
}

BOOL fgDecodeNorm6Diff6(void)
{
    int i, j;
    UINT8 bSymbol[6];
    // a2On4Off are consecutive indices which have 2 1s and 4 0s in binary rep
    int a2On4Off[15] =
    { 3, 5, 6, 9, 10, 12, 17, 18, 20, 24, 33, 34, 36, 40, 48 };
    // aCount is the number of 1s
    int aCount[32] =
    {   0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4,
        2, 3, 3, 4, 3, 4, 4, 5
    };
    int iStart = 0, jStart = 0;
    int i6;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (((rWMVDecInfo.rSeqHdrR.u4NumMBY % 3) == 0)
            && (rWMVDecInfo.rSeqHdrR.u4NumMBX % 3))
    {   // vertically oriented 2 x 3 tiles
        jStart = rWMVDecInfo.rSeqHdrR.u4NumMBX & 1;
        for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i += 3)
        {
            for (j = jStart; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j += 2)
            {
                if (fgDecNorm6Diff6Tile(a2On4Off, aCount, &i6) == FALSE)
                {
                    return FALSE;
                }
                bSymbol[0] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol[0];
                i6 >>= 1;
                bSymbol[1] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i][j+1] = bSymbol[1];
                i6 >>= 1;
                bSymbol[2] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+1][j] = bSymbol[2];
                i6 >>= 1;
                bSymbol[3] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+1][j+1] = bSymbol[3];
                i6 >>= 1;
                bSymbol[4] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+2][j] = bSymbol[4];
                i6 >>= 1;
                bSymbol[5] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+2][j+1] = bSymbol[5];
            }
        }
    }
    else
    {   // horizontally oriented 3 x 2 tiles
        iStart = rWMVDecInfo.rSeqHdrR.u4NumMBY & 1;
        jStart = rWMVDecInfo.rSeqHdrR.u4NumMBX % 3;
        for (i = iStart; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i += 2)
        {
            for (j = jStart; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j += 3)
            {
                if (fgDecNorm6Diff6Tile(a2On4Off, aCount, &i6) == FALSE)
                {
                    return FALSE;
                }
                bSymbol[0] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol[0];
                i6 >>= 1;
                bSymbol[1] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i][j+1] = bSymbol[1];
                i6 >>= 1;
                bSymbol[2] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i][j+2] = bSymbol[2];
                i6 >>= 1;
                bSymbol[3] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+1][j] = bSymbol[3];
                i6 >>= 1;
                bSymbol[4] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+1][j+1] = bSymbol[4];
                i6 >>= 1;
                bSymbol[5] = i6 & 1;
                rWMVDecInfo.rOther.u1BPMask[i+1][j+2] = bSymbol[5];
            }
        }
    }

    // left column(s)
    for (j = 0; j < jStart; j++)
    {
        if (dwGetHi(1))
        {
            for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
            {
                rWMVDecInfo.rOther.u1BPMask[i][j] = dwGetHi(1);
            }
        }
        else
        {
            for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
            {
                bSymbol[0] = 0;
                rWMVDecInfo.rOther.u1BPMask[i][j] = 0;
            }
        }
    }

    // top row
    if (iStart)
    {
        if (dwGetHi(1))
        {
            for (j = jStart; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
            {
                rWMVDecInfo.rOther.u1BPMask[i][j] = dwGetHi(1);
            }
        }
        else
        {
            for (j = jStart; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
            {
                bSymbol[0] = 0;
                rWMVDecInfo.rOther.u1BPMask[i][j] = 0;
            }
        }
    }
    return (TRUE);
}

BOOL fgDecNorm6Diff6Tile(int *a2On4Off, int *aCount, int *pi6)
{
    BOOL bInvert = FALSE;
    int iCode;
    int i6 = *pi6;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    // 4 stage decoding - read 1, 3, 1, 3 bits respectively
StartOfLocalFn6:
    // length 1 code
    if (dwGetHi(1))
    {
        i6 = 0;
        goto EndOfLocalFn6;
    }
    iCode = dwGetHi(3);
    if (iCode > 1)
    {
        // length 4 codes
        i6 = 1 << (iCode - 2);
        goto EndOfLocalFn6;
    }
    if (iCode == 1)
    {
        // length 5 codes
        if (dwGetHi(1))
        {
            // complementary codes - send to start
            if (bInvert) // only one inversion possible
            {
                *pi6 = i6;
                return FALSE;
            }
            bInvert = TRUE;
            goto StartOfLocalFn6;
        }
        else
        {
            // 3 on 3 off
            i6 = dwGetHi(5);
            iCode = aCount[i6];
            if (iCode == 2)
            {
                i6 |= 32;
            }
            else if (iCode == 3)
                ; // do nothing
            else
            {
                *pi6 = i6;
                return FALSE; // invalid symbol
            }
            goto EndOfLocalFn6;
        }
    }
    // length 8 codes
    iCode = dwGetHi(4);
    if (iCode == 0xff) // non-existent codeword
    {
        *pi6 = i6;
        return FALSE;
    }
    i6 = a2On4Off[iCode];

EndOfLocalFn6:
    if (bInvert)
        i6 ^= 0x3f;
    *pi6 = i6;
    return (TRUE);
}

void vDecodeVOPDQuant(void)
{
    UINT8 bPanningCode;
    UINT8 bPicQtDiff;
    UINT8 bAltPicQt;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn == 2)
    {
        rWMVDecInfo.rOther.fgDQuantOn = TRUE;
        rWMVDecInfo.rOther.i4Panning = 0xf; // four edges
        bPicQtDiff = dwGetHi(3);
        if (bPicQtDiff == 7)
        {
            bAltPicQt = dwGetHi(5);
        }
        else
        {
            bAltPicQt = rWMVDecInfo.rPicLayer.i4StepSize + bPicQtDiff
                        + (UINT8)1;
        }
        rWMVDecInfo.rOther.u1DQuantBiLevelStepSize = bAltPicQt;
    }
    else
    {
        rWMVDecInfo.rOther.fgDQuantBiLevel = FALSE;
        rWMVDecInfo.rOther.i4Panning = 0;
        rWMVDecInfo.rOther.fgDQuantOn = dwGetHi(1);
        rWMVDecInfo.rOther.u1DiffQtProfile = 0;
        if (rWMVDecInfo.rOther.fgDQuantOn)
        {
            rWMVDecInfo.rOther.u1DiffQtProfile = dwGetHi(2);
            if (rWMVDecInfo.rOther.u1DiffQtProfile == ALL_4EDGES)
            {
                rWMVDecInfo.rOther.i4Panning = 0xf;
            }
            else if (rWMVDecInfo.rOther.u1DiffQtProfile == SINGLE_EDGES)
            {
                bPanningCode = dwGetHi(2);
                rWMVDecInfo.rOther.i4Panning = 0x1 << bPanningCode;
            }
            else if (rWMVDecInfo.rOther.u1DiffQtProfile == DOUBLE_EDGES)
            {
                bPanningCode = dwGetHi(2);
                if (bPanningCode != 3)
                    rWMVDecInfo.rOther.i4Panning = 0x3 << bPanningCode;
                else
                    rWMVDecInfo.rOther.i4Panning = 0x9;
            }
            else if (rWMVDecInfo.rOther.u1DiffQtProfile == ALL_MBS)
            {
                rWMVDecInfo.rOther.fgDQuantBiLevel = dwGetHi(1);
            }

            if (rWMVDecInfo.rOther.i4Panning
                    || rWMVDecInfo.rOther.fgDQuantBiLevel) //i.e. if((rWMVDecInfo.rOther.u1DiffQtProfile != ALL_MBS) || (rWMVDecInfo.rOther.fgDQuantBiLevel))
            {
                bPicQtDiff = dwGetHi(3);
                if (bPicQtDiff == 7)
                {
                    bAltPicQt = dwGetHi(5);
                }
                else
                {
                    bAltPicQt = rWMVDecInfo.rPicLayer.i4StepSize + bPicQtDiff
                                + 1;
                }
                rWMVDecInfo.rOther.u1DQuantBiLevelStepSize = bAltPicQt;
            }
        } // rWMVDecInfo.rOther.fgDQuantOn
    }
}

void vSetDefaultDQuantSetting(void)
{
    rWMVDecInfo.rOther.fgDQuantOn = FALSE;
    rWMVDecInfo.rOther.fgDQuantBiLevel = FALSE;
    rWMVDecInfo.rOther.i4Panning = 0;
    rWMVDecInfo.rOther.u1DQuantBiLevelStepSize
    = rWMVDecInfo.rPicLayer.i4StepSize;
    //Printf("vSetDefaultDQuantSetting, rWMVDecInfo.rOther.u1DQuantBiLevelStepSize : %d\n",rWMVDecInfo.rOther.u1DQuantBiLevelStepSize);
    //  rWMVDecInfo.rOther.u1DiffQtProfile = 0;
    return;
}

UINT32 iGetPMvMode(UINT32 iPQuant, BOOL fgRepeat)
{
    UINT32 bMvMode;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (iPQuant > 12)
    {   // P Picture Low Rate
        if (dwGetHi(1))
            bMvMode = ALL_1MV_HALFPEL_BILINEAR; // 1b
        else if (dwGetHi(1))
            bMvMode = ALL_1MV; // 01b
        else if (dwGetHi(1))
            bMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (fgRepeat || (!dwGetHi(1)))
                bMvMode = MIXED_MV; // 0000b
            else
                bMvMode = INTENSITY_COMPENSATION; // 0001b
        }
    }
    else
    {   // P Picture High rate
        if (dwGetHi(1))
            bMvMode = ALL_1MV; // 1b
        else if (dwGetHi(1))
            bMvMode = MIXED_MV; // 01b
        else if (dwGetHi(1))
            bMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (fgRepeat || (!dwGetHi(1)))
                bMvMode = ALL_1MV_HALFPEL_BILINEAR; // 0000b
            else
                bMvMode = INTENSITY_COMPENSATION; // 0001b
        }
    }
    return bMvMode;
}

UINT32 iGetIFMvMode(UINT32 iPQuant, BOOL fgRepeat) //for Interlace Field
{
    int iMvMode;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (iPQuant > 12)
    {   // P/B Field Picture Low Rate
        if (dwGetHi(1))
            iMvMode = ALL_1MV_HALFPEL_BILINEAR; // 1b
        else if (dwGetHi(1))
            iMvMode = ALL_1MV; // 01b
        else if (dwGetHi(1))
            iMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            {
                iMvMode = MIXED_MV;
            }
            else
            {
                if (fgRepeat || (!dwGetHi(1)))
                    iMvMode = MIXED_MV; // 0000b
                else
                    iMvMode = INTENSITY_COMPENSATION; // 0001b
            }
        }
    }
    else
    {   // P/B Field Picture High rate
        if (dwGetHi(1))
            iMvMode = ALL_1MV; // 1b
        else if (dwGetHi(1))
            iMvMode = MIXED_MV; // 01b
        else if (dwGetHi(1))
            iMvMode = ALL_1MV_HALFPEL; // 001b
        else
        {
            if (rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            {
                iMvMode = ALL_1MV_HALFPEL_BILINEAR;
            }
            else
            {
                if (fgRepeat || (!dwGetHi(1)))
                    iMvMode = ALL_1MV_HALFPEL_BILINEAR; // 0000b
                else
                    iMvMode = INTENSITY_COMPENSATION; // 0001b
            }
        }
    }
    return iMvMode;
}

void SetupMultiResParams(void)
{
    // Calculate half res params
    int iHalfWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc >> 1;
    int iHalfHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc >> 1;
    int iHalfWidthDec = ((rWMVDecInfo.rSeqHdrR.u4PicWidthSrc >> 1) + 15) & ~15; //Half & MB aligned
    int iHalfHeightDec = ((rWMVDecInfo.rSeqHdrR.u4PicHeightSrc >> 1) + 15)
                         & ~15; //Half & MB aligned
    MULTIRES_PARAMS* pMultiResParams;

    if (rWMVDecInfo.rOther.i4CodecVersion == WMVA)
    {
        //For PROGRESSIVE
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[PROGRESSIVE];

        pMultiResParams->iFrmWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        //For INTERLACEFIELD
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFIELD];

        pMultiResParams->iFrmWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = ((rWMVDecInfo.rSeqHdrR.u4PicHeightSrc >>1) + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        //For INTERLACEFRAME
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[INTERLACEFRAME];

        pMultiResParams->iFrmWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;
    }
    else //For WMV3
    {
        // Save parameters for full res (index = 0)
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[0];

        pMultiResParams->iFrmWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec
        = (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        // Save parameters for half-horizontal, full-vertical res (index = 1)
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[1];

        pMultiResParams->iFrmWidthSrc = iHalfWidthSrc;
        pMultiResParams->iFrmHeightSrc = rWMVDecInfo.rSeqHdrR.u4PicHeightSrc;
        pMultiResParams->iWidthDec = iHalfWidthDec;
        pMultiResParams->iHeightDec
        = (rWMVDecInfo.rSeqHdrR.u4PicHeightSrc + 15) & ~15; //MB-aligned
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        // Save parameters for full-horizontal, half-vertical res (index = 2)
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[2];

        pMultiResParams->iFrmWidthSrc = rWMVDecInfo.rSeqHdrR.u4PicWidthSrc;
        pMultiResParams->iFrmHeightSrc = iHalfHeightSrc;
        pMultiResParams->iWidthDec = (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc + 15)
                                     & ~15; //MB-aligned
        pMultiResParams->iHeightDec = iHalfHeightDec;
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;

        // Save parameters for half-horizontal, half-vertical res (index = 3)
        pMultiResParams = &rWMVDecInfo.rSeqHdrR.rMultiResParams[3];

        pMultiResParams->iFrmWidthSrc = iHalfWidthSrc;
        pMultiResParams->iFrmHeightSrc = iHalfHeightSrc;
        pMultiResParams->iWidthDec = iHalfWidthDec;
        pMultiResParams->iHeightDec = iHalfHeightDec;
        pMultiResParams->dwNumMBX = pMultiResParams->iWidthDec >> 4;
        pMultiResParams->dwNumMBY = pMultiResParams->iHeightDec >> 4;
    }
}

void ComputeDQuantDecParam(void)
{
    int iStepSize;
    int iDCStepSize;

    for (iStepSize = 1; iStepSize < 63; iStepSize++)
    {
        DQuantDecParam *pDQ =
            &rWMVDecInfo.rPicLayer.rDQuantParam3QPDeadzone[iStepSize];

        int iDoubleStepSize = (iStepSize + 1);

        pDQ->iDoubleStepSize = iDoubleStepSize;
        pDQ->iStepMinusStepIsEven = 0;
        pDQ->i2DoublePlusStepSize = iDoubleStepSize;
        //pDQ->i2DoublePlusStepSizeNeg = -1 * pDQ->i2DoublePlusStepSize;

        iDCStepSize = (iStepSize + 1) >> 1;
        if (iDCStepSize <= 4)
        {
            pDQ->iDCStepSize = 8;
            if (rWMVDecInfo.rEntryPoint.fgNewDCQuant && iDCStepSize <= 2)
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
            &rWMVDecInfo.rPicLayer.rDQuantParam5QPDeadzone [iStepSize];
        int iDoubleStepSize;

        iDoubleStepSize = (iStepSize + 1);

        pDQ->iDoubleStepSize = iDoubleStepSize;

        if (rWMVDecInfo.rOther.i4CodecVersion >= WMV3)
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
            if (rWMVDecInfo.rEntryPoint.fgNewDCQuant && iDCStepSize <= 2)
                pDQ->iDCStepSize = 2 * iDCStepSize;

        }
        else
        {
            pDQ->iDCStepSize = iDCStepSize / 2 + 6;
        }
    }
}

UINT32 getbits(UINT32 dwNum) // the range of dwNum is 1 to 32.
{
#ifdef MAX_GET_32_BITS
    UINT32 dwValue;

    dwValue = dwGetBitStream(0); // Just get 32 bits from shifter
    if (dwNum> 0)
    {
        dwValue >>= (32 - dwNum); // Get highest dwNum bits
    }
    dwGetBitStream(dwNum); // Flush dwNum bits

    return dwValue;

#else

    UINT32 dwValue1, dwValue2;
    UINT32 dwRemainBits;

    if (dwNum <= 16)
    {
        dwValue1 = get16(dwNum);
    }
    else
    {
        dwValue1 = get16(16);
        dwRemainBits = dwNum - 16;
        dwValue2 = get16(dwRemainBits);
        dwValue1 = (dwValue1 << dwRemainBits) + dwValue2;
    }

    return dwValue1;
#endif
}

UINT32 get16(UINT32 dwNum) // dwNum must be greater than 0
{
    UINT32 dwValue;

    dwValue = dwGetBitStream(0); // Just get 32 bits from shifter
    if (dwNum > 0)
    {
        dwValue >>= (32 - dwNum); // Get highest dwNum bits
    }
    dwGetBitStream(dwNum); // Flush dwNum bits

    return dwValue;
}

inline UINT32 dwGetHi(UINT32 dwN)
{
    UINT32 dwRetVal;

    dwRetVal = rWMVDecInfo.rOther.u4Datain >> (32 - dwN);
    //Printf("GetHi rWMVDecInfo.rOther.u4Datain : %d %08x\n",dwN,rWMVDecInfo.rOther.u4Datain);
    if (rWMVDecInfo.rOther.fgCounting)
        rWMVDecInfo.rOther.i4PicHdrBits += (int)dwN;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(dwN);

    return dwRetVal;
}

//WMV3 : Simple & Main Profile, decode sequence header
UINT32 decodeVOLHead_WMV3(void)
{
    //BOOL fgRndCtrlOn;
    BOOL fg16bitXform;
    //BOOL fgStartCode;
    //BOOL fgRTMContent;
    //BOOL fgBetaContent;
    //int iBetaRTMMismatchIndex;

    rWMVDecInfo.rSeqLayer.i4Profile = (rWMVDecInfo.rOther.i4SeqHdrData1
                                       & 0xc0000000) >> 30;
    if (rWMVDecInfo.rSeqLayer.i4Profile == 0)
        rWMVDecInfo.rSeqLayer.i4WMV3Profile = WMV3_SIMPLE_PROFILE;
    else if (rWMVDecInfo.rSeqLayer.i4Profile == 1)
        rWMVDecInfo.rSeqLayer.i4WMV3Profile = WMV3_MAIN_PROFILE;
    else if (rWMVDecInfo.rSeqLayer.i4Profile == 2)
        rWMVDecInfo.rSeqLayer.i4WMV3Profile = WMV3_PC_PROFILE;

    rWMVDecInfo.rSMProfile.fgYUV411 = (rWMVDecInfo.rOther.i4SeqHdrData1
                                       & 0x20000000) >> 29;
    rWMVDecInfo.rSMProfile.fgSpriteMode = (rWMVDecInfo.rOther.i4SeqHdrData1
                                           & 0x10000000) >> 28;
    rWMVDecInfo.rSeqLayer.i4FrameRate = (rWMVDecInfo.rOther.i4SeqHdrData1
                                         & 0x0e000000) >> 25;
    rWMVDecInfo.rSeqLayer.i4FrameRate = 4 * rWMVDecInfo.rSeqLayer.i4FrameRate
                                        + 2;
    rWMVDecInfo.rSeqLayer.i4BitRate = (rWMVDecInfo.rOther.i4SeqHdrData1
                                       & 0x01f00000) >> 20;
    rWMVDecInfo.rSeqLayer.i4BitRate = 64 * rWMVDecInfo.rSeqLayer.i4BitRate + 32;

    //fgRndCtrlOn = TRUE;
    rWMVDecInfo.rEntryPoint.fgLoopFilter = (rWMVDecInfo.rOther.i4SeqHdrData1
                                            & 0x00080000) >> 19;
    rWMVDecInfo.rSMProfile.fgXintra8Switch = (rWMVDecInfo.rOther.i4SeqHdrData1
            & 0x00040000) >> 18;
    rWMVDecInfo.rSMProfile.fgMultiresEnabled
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00020000) >> 17;
    fg16bitXform = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00010000) >> 16;
    rWMVDecInfo.rEntryPoint.fgUVHpelBilinear
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00008000) >> 15;
    rWMVDecInfo.rEntryPoint.fgExtendedMvMode
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00004000) >> 14;
    rWMVDecInfo.rEntryPoint.i4DQuantCodingOn
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00003000) >> 12;
    rWMVDecInfo.rEntryPoint.fgXformSwitch = (rWMVDecInfo.rOther.i4SeqHdrData1
                                            & 0x00000800) >> 11;
    rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000400) >> 10;
    rWMVDecInfo.rEntryPoint.fgSequenceOverlap
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000200) >> 9;
    //fgStartCode = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000100) >> 8;
    rWMVDecInfo.rSMProfile.fgPreProcRange = (rWMVDecInfo.rOther.i4SeqHdrData1
                                            & 0x00000080) >> 7;
    rWMVDecInfo.rSMProfile.i4NumBFrames = (rWMVDecInfo.rOther.i4SeqHdrData1
                                           & 0x00000070) >> 4;
    rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000008) >> 3;
    if (rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer)
        rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer
        = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000004) >> 2;
    else
        rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer
        = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000004) >> 2;
    rWMVDecInfo.rEntryPoint.fgExplicitQuantizer
    = rWMVDecInfo.rEntryPoint.fgExplicitSeqQuantizer
      || rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer;

    rWMVDecInfo.rSeqLayer.fgSeqFrameInterpolation
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000002) >> 1;
    rWMVDecInfo.rPicLayer.i4BFrameReciprocal
    = _iBInverse[rWMVDecInfo.rSMProfile.i4NumBFrames];

    if ((rWMVDecInfo.rSMProfile.fgYUV411)
            || (rWMVDecInfo.rSMProfile.fgSpriteMode))
    {
        // 411 is depreciated and untested.  Use WMV9 SP and MP only porting kit for 411 reference if you need it.
        return WMV_UnSupportedCompressedFormat;
    }

    if ((rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000001) == 1)
    {
        // RTM content
        //fgRTMContent = TRUE;
        //fgBetaContent = FALSE;
        //iBetaRTMMismatchIndex = 0;
    }

    if (fg16bitXform)
    {
        rWMVDecInfo.rSMProfile.fgRotatedIdct = TRUE;
    }

    // If rWMVDecInfo.rOther.u4HeaderLen > 4, it goes into this condition.
    if (rWMVDecInfo.rOther.i4SeqHdrDataLen > 4)
    {
        //BOOL bVCMInfoPresent;

        rWMVDecInfo.rSeqLayer.fgPostProcInfoPresent
        = (rWMVDecInfo.rOther.i4SeqHdrData2 & 0x80) >> 7;
        // VCM bit (34th bit) is reserved for VCM contents if VOL size > 4 bytes.
        //bVCMInfoPresent = (rWMVDecInfo.rOther.i4SeqHdrData2 & 0x40) >> 6;
    }

    return WMV_Succeeded;
}

//WMV8 only: decode sequence header
UINT32 decodeVOLHead_WMV12(void)
{
    //BOOL fgRndCtrlOn;

    ASSERT((rWMVDecInfo.rOther.i4CodecVersion == WMV2)
           || (rWMVDecInfo.rOther.i4CodecVersion == WMV1));

    ASSERT(rWMVDecInfo.rOther.i4SeqHdrData2 == 0);

    rWMVDecInfo.rSeqLayer.i4FrameRate = (rWMVDecInfo.rOther.i4SeqHdrData1
                                         & 0xf8000000) >> 27;
    rWMVDecInfo.rSeqLayer.i4BitRate = (rWMVDecInfo.rOther.i4SeqHdrData1
                                       & 0x07ff0000) >> 16;
    //fgRndCtrlOn = TRUE;
    rWMVDecInfo.rSMProfile.fgMixedPel = (rWMVDecInfo.rOther.i4SeqHdrData1
                                         & 0x00008000) >> 15;
    rWMVDecInfo.rEntryPoint.fgLoopFilter = (rWMVDecInfo.rOther.i4SeqHdrData1
                                            & 0x00004000) >> 14;
    rWMVDecInfo.rEntryPoint.fgXformSwitch = (rWMVDecInfo.rOther.i4SeqHdrData1
                                            & 0x00002000) >> 13;
    rWMVDecInfo.rSMProfile.fgXintra8Switch = (rWMVDecInfo.rOther.i4SeqHdrData1
            & 0x00001000) >> 12;

    rWMVDecInfo.rSMProfile.fgFrmHybridMVOn = (rWMVDecInfo.rOther.i4SeqHdrData1
            & 0x00000800) >> 11;

    // DCTTABLE S/W at MB level for WMV2.
    rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED
    = (rWMVDecInfo.rOther.i4SeqHdrData1 & 0x00000400) >> 10;
    rWMVDecInfo.rSMProfile.i4SliceCode = (rWMVDecInfo.rOther.i4SeqHdrData1
                                          & 0x00000380) >> 7;

    return WMV_Succeeded;
}

int ReconRangeState_new = 0;
//WMV3 : Simple & Main Profile, decode picture header
UINT32 WMVideoDecDecodeFrameHead(void)
{
    //BOOL fgInterpolateCurrentFrame;
    //int iFrmCntMod4 = 0;
    //int iMVTYPEMB;
    //int iDIRECTMB;
    //int iSKIPMB;
    BOOL fgTransTypeMB;
    UINT32 iTransTypeFrame;

    //UINT32 dwTmp;
    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
    LOG(6, "WMVideoDecDecodeFrameHead datain : %x\n",rWMVDecInfo.rOther.u4Datain);
    if (rWMVDecInfo.rSeqLayer.fgSeqFrameInterpolation)
    {
        /*fgInterpolateCurrentFrame =*/
        dwGetHi(1);
    }
    /*iFrmCntMod4 =*/dwGetHi(2);

    if (rWMVDecInfo.rSMProfile.fgPreProcRange)
    {
        rWMVDecInfo.rEntryPoint.i4RangeState = dwGetHi(1);
    }

    rWMVDecInfo.rPicLayer.u1PrevPicType = rWMVDecInfo.rPicLayer.u1PicType;
    // Picture coding type
    if (1 == dwGetHi(1))
        rWMVDecInfo.rPicLayer.u1PicType = PVOP;
    else
    {
        if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
        {
            rWMVDecInfo.rPicLayer.u1PicType = IVOP;
        }
        else
        {
            if (dwGetHi(1) == 1)
                rWMVDecInfo.rPicLayer.u1PicType = IVOP;
            else
                rWMVDecInfo.rPicLayer.u1PicType = BVOP;
        }
    }

    // Decode B frac
    if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        int iShort = 0, iLong = 0;
        iShort = dwGetHi(3);
        if (iShort == 0x7)
        {
            iLong = dwGetHi(4);
            if (iLong == 0xe) // "Invalid" in VLC
                return INVALID_32;

            if (iLong == 0xf)
                rWMVDecInfo.rPicLayer.u1PicType = BIVOP;
            else
            {
                rWMVDecInfo.rPicLayer.i4BNumerator = _iNumLongVLC[iLong];
                rWMVDecInfo.rPicLayer.i4BDenominator = _iDenLongVLC[iLong];
                rWMVDecInfo.rPicLayer.i4BFrameReciprocal
                = _iBInverse[rWMVDecInfo.rPicLayer.i4BDenominator - 1];
                if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
                    rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
            }
        }
        else
        {
            rWMVDecInfo.rPicLayer.i4BNumerator = _iNumShortVLC[iShort];
            rWMVDecInfo.rPicLayer.i4BDenominator = _iDenShortVLC[iShort];
            rWMVDecInfo.rPicLayer.i4BFrameReciprocal
            = _iBInverse[rWMVDecInfo.rPicLayer.i4BDenominator - 1];
            if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
                rWMVDecInfo.rSMProfile.i4NumBFrames = 1;
        }
    }

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        int iBufferFullPercent = dwGetHi(7);
        UNUSED(iBufferFullPercent);
    }

    rWMVDecInfo.rPicLayer.i4PicQtIdx = dwGetHi(5);

    if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAXHALFQP)
        rWMVDecInfo.rPicLayer.fgHalfStep = dwGetHi(1);
    else
        rWMVDecInfo.rPicLayer.fgHalfStep = FALSE;

    if (rWMVDecInfo.rEntryPoint.fgExplicitFrameQuantizer)
        rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = dwGetHi(1);

    if (!rWMVDecInfo.rEntryPoint.fgExplicitQuantizer)
    {
        if (rWMVDecInfo.rPicLayer.i4PicQtIdx <= MAX3QP)
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = TRUE;
            rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;
        }
        else
        {
            rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer = FALSE;
            rWMVDecInfo.rPicLayer.i4StepSize
            = _iStepRemap[rWMVDecInfo.rPicLayer.i4PicQtIdx];
        }
    }
    else
        rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;

    rWMVDecInfo.rPicLayer.prDQuantParam
    = rWMVDecInfo.rPicLayer.fgUse3QPDZQuantizer ? rWMVDecInfo.rPicLayer.rDQuantParam3QPDeadzone
      : rWMVDecInfo.rPicLayer.rDQuantParam5QPDeadzone;

    rWMVDecInfo.rPicLayer.i4Overlap = 0;
    if (rWMVDecInfo.rEntryPoint.fgSequenceOverlap
            && (rWMVDecInfo.rPicLayer.u1PicType != BVOP))
    {
        if (rWMVDecInfo.rPicLayer.i4StepSize >= 9)
            rWMVDecInfo.rPicLayer.i4Overlap = 1;
        else if ((rWMVDecInfo.rSeqLayer.i4WMV3Profile == WMV3_ADVANCED_PROFILE)
                 && ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
                     || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP)))
            rWMVDecInfo.rPicLayer.i4Overlap = 7;
        //rWMVDecInfo.rPicLayer.i4Overlap last 3 bits: [MB switch=1/frame switch=0][sent=1/implied=0][on=1/off=0]
    }

    if (rWMVDecInfo.rEntryPoint.fgExtendedMvMode)
    {
        rWMVDecInfo.rEntryPoint.i4MVRangeIndex = dwGetHi(1);
        if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex)
            rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1);
        if (rWMVDecInfo.rEntryPoint.i4MVRangeIndex == 2)
            rWMVDecInfo.rEntryPoint.i4MVRangeIndex += dwGetHi(1);
    }

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == PVOP))
    {
        if (rWMVDecInfo.rSMProfile.fgMultiresEnabled)
            rWMVDecInfo.rSMProfile.i4ResIndex = dwGetHi(2);
    }

    //  if(rWMVDecInfo.rSMProfile.i4ResIndex != 0)
    {
        rWMVDecInfo.rSeqHdrR.u4PicWidthDec
        = rWMVDecInfo.rSeqHdrR.rMultiResParams[rWMVDecInfo.rSMProfile.i4ResIndex].iWidthDec;
        rWMVDecInfo.rSeqHdrR.u4PicHeightDec
        = rWMVDecInfo.rSeqHdrR.rMultiResParams[rWMVDecInfo.rSMProfile.i4ResIndex].iHeightDec;
        //Printf("src decode resolution : %dx%d\n",rWMVDecInfo.rSeqHdrR.u4PicWidthDec,rWMVDecInfo.rSeqHdrR.u4PicHeightDec);
        //Printf("index : %d",rWMVDecInfo.rSMProfile.i4ResIndex);
        rWMVDecInfo.rSeqHdrR.u4NumMBX
        = rWMVDecInfo.rSeqHdrR.rMultiResParams[rWMVDecInfo.rSMProfile.i4ResIndex].dwNumMBX;
        rWMVDecInfo.rSeqHdrR.u4NumMBY
        = rWMVDecInfo.rSeqHdrR.rMultiResParams[rWMVDecInfo.rSMProfile.i4ResIndex].dwNumMBY;
        rWMVDecInfo.rSeqHdrR.u4PicWidthCmp
        = rWMVDecInfo.rSeqHdrR.rMultiResParams[rWMVDecInfo.rSMProfile.i4ResIndex].iFrmWidthSrc;
        rWMVDecInfo.rSeqHdrR.u4PicHeightCmp
        = rWMVDecInfo.rSeqHdrR.rMultiResParams[rWMVDecInfo.rSMProfile.i4ResIndex].iFrmHeightSrc;
    }

    vSetNewMode();

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx = dwGetHi(1);
        if (rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx)
        {
            rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);

        vSetDefaultDQuantSetting();

        rWMVDecInfo.rPicLayer.i4RndCtrl = 1;

    } //End of IVOP, BIVOP
    else // PVOP, BVOP
    {
        //Start==== decodeVOPHead_WMV3() ===================================
        rWMVDecInfo.rOther.fgLuminanceWarp = FALSE;
        rWMVDecInfo.rOther.fgLuminanceWarpTop
        = rWMVDecInfo.rOther.fgLuminanceWarpBottom = FALSE;

        if ((rWMVDecInfo.rPicLayer.u1PicType == BVOP)
                && (!rWMVDecInfo.rPicLayer.fgFieldMode))
        {
            if (dwGetHi(1))
            {
                rWMVDecInfo.rOther.i4X9MVMode = ALL_1MV;
            }
            else
            {
                rWMVDecInfo.rOther.i4X9MVMode = ALL_1MV_HALFPEL_BILINEAR;
            }

#ifdef TEST
            _dwLookAhead = dwGetBitStream(0);
#endif

            //dwTmp = dwGetBitStream(0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                //spork
                /*iDIRECTMB = */
                iDecodeBitPlane(3); // "DirectMB", bp_num = 3
            }
            else
            {
                fgHWDecBP(3);
            }

#ifdef TEST
            _dwLookAhead = dwGetBitStream(0);
#endif

            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        }
        else
        {
            rWMVDecInfo.rOther.i4X9MVMode = iGetPMvMode(
                                                rWMVDecInfo.rPicLayer.i4StepSize, FALSE);
            if (rWMVDecInfo.rOther.i4X9MVMode == INTENSITY_COMPENSATION)
            {
                rWMVDecInfo.rOther.fgLuminanceWarp = TRUE;
                rWMVDecInfo.rOther.i4X9MVMode = iGetPMvMode(
                                                    rWMVDecInfo.rPicLayer.i4StepSize, TRUE);
                rWMVDecInfo.rOther.i4LumScale = dwGetHi(6);
                rWMVDecInfo.rOther.i4LumShift = dwGetHi(6);
            }

            if (rWMVDecInfo.rOther.i4X9MVMode == MIXED_MV)
            {
#ifdef TEST
                _dwLookAhead = dwGetBitStream(0);
#endif
                //dwTmp = dwGetBitStream(0) >> 28;
                //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
                if (cbWMVFWBP)
                {
                    /*iMVTYPEMB =*/
                    iDecodeBitPlane(1); //"4mv", bp_num = 1
                }
                else
                {
                    fgHWDecBP(1);
                }
#ifdef TEST
                _dwLookAhead = dwGetBitStream(0);
#endif
                rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
            }
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        //dwTmp = dwGetBitStream(0) >> 28;
        //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
        if (cbWMVFWBP)
        {
            /*iSKIPMB =*/
            iDecodeBitPlane(0); // "SkipMB", bp_num = 0
        }
        else
        {
            fgHWDecBP(0);
        }
#ifdef TEST
        _dwLookAhead = dwGetBitStream(0);
#endif
        rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

        rWMVDecInfo.rSMProfile.fgCODFlagOn = TRUE;

        rWMVDecInfo.rOther.i4MvTable = dwGetHi(2);
        rWMVDecInfo.rOther.i4CBPTable = dwGetHi(2); // Only in P & B pictures.
        //Printf("-------------mv table : %x , cbp table : %x\nrWMVDecInfo.rEntryPoint.i4DQuantCodingOn : %x\n",rWMVDecInfo.rOther.i4MvTable,rWMVDecInfo.rOther.i4CBPTable,rWMVDecInfo.rEntryPoint.i4DQuantCodingOn);
        if (rWMVDecInfo.rEntryPoint.i4DQuantCodingOn)
        {
            vDecodeVOPDQuant();
        }

        if (rWMVDecInfo.rEntryPoint.fgXformSwitch)
        {
            fgTransTypeMB = dwGetHi(1);
            if (fgTransTypeMB)
            {
                rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
                iTransTypeFrame = dwGetHi(2);
                rWMVDecInfo.rOther.i4FrameXformMode
                = s_pXformLUT[iTransTypeFrame];
            }
            else
                rWMVDecInfo.rOther.fgMBXformSwitching = TRUE;
        }
        else
        {
            rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
        }
        //End==== decodeVOPHead_WMV3() ===================================

        rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1); // Coding set index = 0, 1, or 2.
        if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx == 1)
        {
            rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
        }
        rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx
        = rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx;

        rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);
        //=================================================================
        if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
        {
            if (!rWMVDecInfo.rOther.u4repeatframe)
                rWMVDecInfo.rPicLayer.i4RndCtrl ^= 1;
        }
    }

    if ((rWMVDecInfo.rPicLayer.u1PicType == IVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
            || (rWMVDecInfo.rPicLayer.u1PicType == BIVOP))
    {
        UpdateDCStepSize(rWMVDecInfo.rPicLayer.i4StepSize);
    }

    if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
    {
        AdjustReconRange();
    }
    else if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
    {
        ReconRangeState_new = rWMVDecInfo.rEntryPoint.i4RangeState;
    }

    return WMV_Succeeded;
}

//WMV7 & WMV8: decode picture header
UINT32 WMV78DecodePicture(void)
{
    int iNumBitsFrameType;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    iNumBitsFrameType = (rWMVDecInfo.rOther.i4CodecVersion == WMV2) ? 1 : 2;
    rWMVDecInfo.rPicLayer.u1PrevPicType = rWMVDecInfo.rPicLayer.u1PicType;
    rWMVDecInfo.rPicLayer.u1PicType = dwGetHi(iNumBitsFrameType);

    if ((rWMVDecInfo.rOther.i4CodecVersion >= WMV2)
            && (rWMVDecInfo.rPicLayer.u1PicType == IVOP))
    {
        int iBufferFullPercent = dwGetHi(7);
        UNUSED(iBufferFullPercent);
    }

    rWMVDecInfo.rPicLayer.i4PicQtIdx = dwGetHi(5);
    rWMVDecInfo.rPicLayer.prDQuantParam
    = rWMVDecInfo.rPicLayer.rDQuantParam5QPDeadzone;
    rWMVDecInfo.rPicLayer.i4StepSize = rWMVDecInfo.rPicLayer.i4PicQtIdx;

    if (rWMVDecInfo.rPicLayer.u1PicType == IVOP)
    {
        if (rWMVDecInfo.rOther.i4CodecVersion < WMV2) // WMV1 only
        {
            rWMVDecInfo.rSMProfile.i4SliceCode = dwGetHi(NUMBITS_SLICE_SIZE);
            ASSERT(rWMVDecInfo.rSMProfile.i4SliceCode > 22);
        }

        WMVideoDecDecodeClipInfo();
        if (rWMVDecInfo.rSMProfile.fgXintra8)
        {
            return WMV_Succeeded;
        }

        if (rWMVDecInfo.rOther.i4CodecVersion >= WMV2)
        {
            rWMVDecInfo.rOther.fgDCPred_IMBInPFrame = FALSE;
        }
        else // WMV1
        {
            rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED
            = (rWMVDecInfo.rSeqLayer.i4BitRate > MIN_BITRATE_MB_TABLE);
            rWMVDecInfo.rOther.fgDCPred_IMBInPFrame
            = ((rWMVDecInfo.rSeqLayer.i4BitRate
                <= MAX_BITRATE_DCPred_IMBInPFrame)
               && (rWMVDecInfo.rSeqHdrR.u4PicWidthSrc
                   * rWMVDecInfo.rSeqHdrR.u4PicHeightSrc < 320
                   * 240));
        }

        if ((!rWMVDecInfo.rSMProfile.fgXintra8)
                && (rWMVDecInfo.rOther.i4CodecVersion >= MMIDRATE43))
        {
            if (rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED)
            {
                rWMVDecInfo.rPicLayer.fgDCTTable_MB = dwGetHi(1);
            }
            if (!rWMVDecInfo.rPicLayer.fgDCTTable_MB)
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
                rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1);
                if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx)
                {
                    rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
                }
                rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx = dwGetHi(1);
                if (rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx)
                {
                    rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx += dwGetHi(1);
                }
            }
            rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);
        }
        vSetDefaultDQuantSetting();
        rWMVDecInfo.rPicLayer.i4RndCtrl = 1;
    }
    else //PVOP
    {
        decodeVOPHead_WMV2();

        if (rWMVDecInfo.rOther.i4CodecVersion >= MMIDRATE43)
        {
            // MMIDRATE43 || WMV1 (2)
            if (rWMVDecInfo.rSMProfile.fgDCTTable_MB_ENABLED)
            {
                rWMVDecInfo.rPicLayer.fgDCTTable_MB = dwGetHi(1);
            }

            if (!rWMVDecInfo.rPicLayer.fgDCTTable_MB)
            {
                rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx = dwGetHi(1);
                if (rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx)
                {
                    rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx += dwGetHi(1);
                }
            }
            rWMVDecInfo.rPicLayer.u4DCTACIntraTableIndx
            = rWMVDecInfo.rPicLayer.u4DCTACInterTableIndx;

            rWMVDecInfo.rPicLayer.fgIntraDCTDCTable = dwGetHi(1);

            if (rWMVDecInfo.rOther.i4CodecVersion != WMV3)
                rWMVDecInfo.rOther.i4MvTable = dwGetHi(1);
        }
        if (rWMVDecInfo.rPicLayer.u1PicType == PVOP)
        {
            if (!rWMVDecInfo.rOther.u4repeatframe)
                rWMVDecInfo.rPicLayer.i4RndCtrl ^= 1;
        }
    }

    UpdateDCStepSize(rWMVDecInfo.rPicLayer.i4StepSize);

    return WMV_Succeeded;
}

UINT32 WMVideoDecDecodeClipInfo(void)
{
    //BOOL fgRndCtrlOn;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (rWMVDecInfo.rOther.i4CodecVersion >= WMV2)
    {
        if (rWMVDecInfo.rSMProfile.fgXintra8Switch)
            rWMVDecInfo.rSMProfile.fgXintra8 = dwGetHi(1);
        if (rWMVDecInfo.rSMProfile.fgXintra8)
        {
#ifdef CC_WMV_EMULATION
            extern BOOL _fgAbtCurItem;
            _fgAbtCurItem = TRUE;
#endif
            LOG(0, "===x8_intra clip @ #%d\n", rWMVDecInfo.rOther.u4CurrPicNo);
        }
    }
    else // WMV1
    {
        int iFrameRate = dwGetHi(5);
        if (rWMVDecInfo.rSeqLayer.i4FrameRate == 0) // if the info is available from system (app), use it.
        {
            rWMVDecInfo.rSeqLayer.i4FrameRate = iFrameRate;
        }
        rWMVDecInfo.rSeqLayer.i4BitRate = dwGetHi(11);
        if (rWMVDecInfo.rOther.i4CodecVersion != MMIDRATE42)
        {
            /*fgRndCtrlOn =*/
            dwGetHi(1);
        }
    }

    return WMV_Succeeded;
}

void decodeVOPHead_WMV2(void)
{
    //UINT32 dwTmp;

    vSetNewMode();

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    if (!rWMVDecInfo.rSMProfile.fgSKIPBIT_CODING)
    {
        rWMVDecInfo.rSMProfile.fgCODFlagOn = dwGetHi(1);
        rWMVDecInfo.rSMProfile.i4skip_bit_mode_v87
        = rWMVDecInfo.rSMProfile.fgCODFlagOn; //0:no_skip_bit, 1:raw_mode
    }
    else
    {
        int iSkipBitCode = dwGetHi(2);
        rWMVDecInfo.rSMProfile.fgCODFlagOn = TRUE;
        if (iSkipBitCode == 0)
        {
            rWMVDecInfo.rSMProfile.i4skip_bit_mode_v87 = 0;
            rWMVDecInfo.rSMProfile.fgCODFlagOn = FALSE;
        }
        else
        {
            rWMVDecInfo.rSMProfile.i4skip_bit_mode_v87 = 2; //2: read dram
            if (iSkipBitCode == 1)
            {
                rWMVDecInfo.rSMProfile.i4SkipBitCodingMode = Normal;
                rWMVDecInfo.rSMProfile.i4wmv8_bp_mode = 1;
            }
            else if (iSkipBitCode == 2)
            {
                rWMVDecInfo.rSMProfile.i4SkipBitCodingMode = RowPredict;
                rWMVDecInfo.rSMProfile.i4wmv8_bp_mode = 2;
            }
            else
            {
                rWMVDecInfo.rSMProfile.i4SkipBitCodingMode = ColPredict;
                rWMVDecInfo.rSMProfile.i4wmv8_bp_mode = 3;
            }
#ifdef TEST
            _dwLookAhead = dwGetBitStream(0);
#endif
            //dwTmp = dwGetBitStream(0) >> 28;
            //if(((dwTmp & 0x7) == 0x3) || (cbWMVFWBP->Checked))
            if (cbWMVFWBP)
            {
                DecodeSkipBit();; //SKIPMB, wmv78 bitplane decode
            }
            else
            {
                fgHWDecBP(0); //SKIPMB
            }
#ifdef TEST
            _dwLookAhead = dwGetBitStream(0);
#endif
            rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);
        }
    }

    // NEW_PCBPCY_TABLE
    if (rWMVDecInfo.rSMProfile.fgNEW_PCBPCY_TABLE)
    {
        if (rWMVDecInfo.rPicLayer.i4StepSize <= 10)
        {
            if (!dwGetHi(1))
            {   //0 High
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = HighRate;
            }
            else if (!dwGetHi(1))
            {   //10 Low
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = LowRate;
            }
            else
            {   //11 Mid
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = MidRate;
            }
        }
        else if (rWMVDecInfo.rPicLayer.i4StepSize <= 20)
        {
            if (!dwGetHi(1))
            {   //0 Mid
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = MidRate;
            }
            else if (!dwGetHi(1))
            {   //10 High
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = HighRate;
            }
            else
            {   //11 Low
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = LowRate;
            }
        }
        else
        {
            if (!dwGetHi(1))
            {   //0 Low
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = LowRate;
            }
            else if (!dwGetHi(1))
            {   //10 Mid
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = MidRate;
            }
            else
            {   //11 High
                rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec = HighRate;
            }
        }
        rWMVDecInfo.rOther.i4CBPTable
        = rWMVDecInfo.rSMProfile.i4HufNewPCBPCYDec;
    }
    //Robert TODO: rWMVDecInfo.rOther.i4CBPTable = ???. It should be assigned here !!!

    //_MIXEDPEL_
    if (rWMVDecInfo.rSMProfile.fgMixedPel)
    {
        rWMVDecInfo.rPicLayer.fgMvResolution = dwGetHi(1);
    }

    if (rWMVDecInfo.rEntryPoint.fgXformSwitch)
    {
        if (dwGetHi(1) == 1)
        {
            rWMVDecInfo.rOther.fgMBXformSwitching = FALSE;
            if (dwGetHi(1) == 0)
                rWMVDecInfo.rOther.i4FrameXformMode = XFORMMODE_8x8;
            else if (dwGetHi(1) == 0)
                rWMVDecInfo.rOther.i4FrameXformMode = XFORMMODE_8x4;
            else
                rWMVDecInfo.rOther.i4FrameXformMode = XFORMMODE_4x8;
        }
        else
        {
            rWMVDecInfo.rOther.fgMBXformSwitching = TRUE;
        }
    }
}

void DecodeSkipBit(void)
{
    UINT8 bSymbol;
    int i, j;
    UINT64 hyTmp;
    UINT32 dwWrSa;

    rWMVDecInfo.rOther.u4Datain = dwGetBitStream(0);

    // reset bitplane mask table.
    for (i = 0; i < MAX_MBY; i++)
    {
        for (j = 0; j < MAX_MBX; j++)
        {
            rWMVDecInfo.rOther.u1BPMask[i][j] = 0;
        }
        rWMVDecInfo.rOther.hyBP[i] = 0;
    }

    switch (rWMVDecInfo.rSMProfile.i4SkipBitCodingMode)
    {
    case Normal:
        for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
        {
            for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
            {
                bSymbol = dwGetHi(1);
                rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
            }
        }
        break;
    case RowPredict:
        for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
        {
            if (dwGetHi(1))
            {   //skip row
                for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
                {
                    bSymbol = 1;
                    rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
                }
            }
            else
            {
                for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
                {
                    bSymbol = dwGetHi(1);
                    rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
                }
            }
        }
        break;
    case ColPredict:
        for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
        {
            if (dwGetHi(1))
            {   // skip column
                for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
                {
                    bSymbol = 1;
                    rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
                }
            }
            else
            {
                for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++)
                {
                    bSymbol = dwGetHi(1);
                    rWMVDecInfo.rOther.u1BPMask[i][j] = bSymbol;
                }
            }
        }
        break;
    default:
        ASSERT(FALSE);
        break;
    }

    // reconstruct bitplane to rWMVDecInfo.rOther.hyBP[]
    for (i = 0; i < (int)rWMVDecInfo.rSeqHdrR.u4NumMBY; i++) // for each MB-row
    {
        for (j = 0; j < (int)rWMVDecInfo.rSeqHdrR.u4NumMBX; j++)
        {
            if (rWMVDecInfo.rOther.u1BPMask[i][j] != 0)
            {
                hyTmp = (UINT64)rWMVDecInfo.rOther.u1BPMask[i][j];
                rWMVDecInfo.rOther.hyBP[i] += hyTmp << ((BP_MB_BITS-1) - j);
            }
        }
    }

    dwWrSa = (UINT32)rWMVDecInfo.rDram.pu1Bp_1;

    for (i = 0; i < (int)MAX_MBY; i++) // for each MB-row
    {

        rWMVDecInfo.rOther.hyBP[2*i+1] = 0;
        for (j = (int)MAX_MBX/2; j < (int)MAX_MBX; j++)
        {
            if (rWMVDecInfo.rOther.u1BPMask[i][j] != 0)
            {
                hyTmp = (UINT64)rWMVDecInfo.rOther.u1BPMask[i][j];
                rWMVDecInfo.rOther.hyBP[2*i+1] += hyTmp << ((BP_MB_BITS-1) - (j
                                                  - (int)MAX_MBX/2));
            }
        }
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 0)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 8)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 16)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 24)
                                        &0xFF) );
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 32)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 40)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 48)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i+1] >> 56)
                                        &0xFF) );
        dwWrSa++;

        rWMVDecInfo.rOther.hyBP[2*i] = 0;
        for (j = 0; j < (int)MAX_MBX/2; j++)
        {
            if (rWMVDecInfo.rOther.u1BPMask[i][j] != 0)
            {
                hyTmp = (UINT64)rWMVDecInfo.rOther.u1BPMask[i][j];
                rWMVDecInfo.rOther.hyBP[2*i] += hyTmp << ((BP_MB_BITS-1) - j);
            }
        }
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 0)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 8)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 16)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 24)
                                        &0xFF) );
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 32)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 40)
                                        &0xFF));
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 48)
                                        &0xFF)); //Low UINT32
        dwWrSa++;
        WriteDram8b(dwWrSa, 0, (UINT32)((rWMVDecInfo.rOther.hyBP[2*i] >> 56)
                                        &0xFF) );
        dwWrSa++;
    }

#ifdef CC_MT5395
    HalFlushDCacheMultipleLine(rWMVDecInfo.rPesInfo.u4FifoStart, rWMVDecInfo.rPesInfo.u4FifoEnd - rWMVDecInfo.rPesInfo.u4FifoStart);
#else
    HalFlushInvalidateDCache();
#endif
}

// WMV7 & WMV8 End
//int ReconRangeState_new = 0;
void AdjustReconRange(void)
{
    rWMVDecInfo.rEntryPoint.i4ReconRangeState = ReconRangeState_new;
    if (ReconRangeState_new == 0)
    {
        if (rWMVDecInfo.rEntryPoint.i4RangeState == 1)
        {
            ReconRangeState_new = rWMVDecInfo.rEntryPoint.i4RangeState;
        }
    }
    else if (ReconRangeState_new == 1)
    {
        if (rWMVDecInfo.rEntryPoint.i4RangeState == 0)
        {
            ReconRangeState_new = rWMVDecInfo.rEntryPoint.i4RangeState;
        }
    }
}

void PostAdjustReconRange(void)
{
    if (rWMVDecInfo.rSMProfile.fgPreProcRange)
    {
        if (rWMVDecInfo.rPicLayer.u1PicType == IVOP
                || rWMVDecInfo.rPicLayer.u1PicType == BIVOP)
        {
            if (rWMVDecInfo.rSMProfile.i4NumBFrames == 0)
                rWMVDecInfo.rEntryPoint.i4ReconRangeState
                = rWMVDecInfo.rEntryPoint.i4RangeState;
            else if (rWMVDecInfo.rPicLayer.u1PicType != BVOP)
            {
                AdjustReconRange();
            }
        }
        //    if(rWMVDecInfo.rEntryPoint.i4RangeState != 0)
        //    {
        //      AdjustDecRange();
        //    }
    }
}
