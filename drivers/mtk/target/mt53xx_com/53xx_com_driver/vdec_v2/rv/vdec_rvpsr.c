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

/** @file vdec_rvpsr.c
 *  This header file declares exported APIs of RV.
 */

#include "x_lint.h"
#include "vdec_rvutil.h"
#include "vdec_rvhw.h"
#include "vdec_rvapi.h"
#include "vdec_rvpsr.h"
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

#define DWRD UINT32


static const DWRD MBA_NumMBs[]     = {47, 98, 395, 1583, 6335, 9215};
static const DWRD MBA_FieldWidth[] = {6, 7, 9, 11, 13, 14 };
static const DWRD MBA_Widths = sizeof( MBA_FieldWidth ) / sizeof( MBA_FieldWidth[0] );
#define TR_WRAP  256
#define TR_WRAP_RV  8192
#define MAX_NUM_RPR_SIZES RV_MAX_NUM_RPR_SIZES 
#define RV_GetBit(n) rv_bitring_read(prRingBuf,n)

typedef enum
{
	RV_FID_REALVIDEO30,        /* 0 (00) */
	RV_FID_RV89COMBO           /* 1 (01) */
} EnumRVBitsVersion;

DWRD *_dwRPRSizes[VDEC_MAX_ES];  //DWRD _dwRPRSizes[2 * MAX_NUM_RPR_SIZES];  !!!
BOOL _fgIsRV9[VDEC_MAX_ES];
DWRD _dwPSQuant[VDEC_MAX_ES];
DWRD _dwOSVQuant[VDEC_MAX_ES]; // only RV9
BOOL _fgDeblockPassThru[VDEC_MAX_ES];
DWRD _dwTr[VDEC_MAX_ES];
INT32 _i4Trd[VDEC_MAX_ES];
INT32 _i4Trb[VDEC_MAX_ES];
#ifdef CC_FBM_TWO_FBP
DWRD _dwTrWarp[VDEC_MAX_ES] = {TR_WRAP_RV, TR_WRAP_RV}; //(TR_WRAP_RV);  // ToDo: water-proof it
#else
DWRD _dwTrWarp[VDEC_MAX_ES] = {TR_WRAP_RV}; //(TR_WRAP_RV);  // ToDo: water-proof it
#endif
DWRD _dwPicSz[VDEC_MAX_ES];  // only RV8
DWRD _dwMbaSz[VDEC_MAX_ES];
DWRD _dwMba[VDEC_MAX_ES];
DWRD _dwRType[VDEC_MAX_ES]; // only RV8
DWRD _dwIRatio[VDEC_MAX_ES];
DWRD _dwPicWidth[VDEC_MAX_ES];
DWRD _dwPicHeight[VDEC_MAX_ES];
DWRD _dwSzOfPicSz[VDEC_MAX_ES];
DWRD _dwPicType[VDEC_MAX_ES]; 
DWRD _dwHdrBitCnt[VDEC_MAX_ES];


/*extern */DWRD _dwBitCount;
/*extern */DWRD _dwHSize[VDEC_MAX_ES];
/*extern */DWRD _dwVSize[VDEC_MAX_ES];

DWRD _dwRv8TblNum;
DWRD _dwRefQpMb0;
DWRD _dwFRefTr[VDEC_MAX_ES];
DWRD _dwBRefTr[VDEC_MAX_ES];


#if 0  // !!! use the version from emulation
BOOL fgVldInitBarrelShifter(UINT32 dwPtr)
{
    BOOL fgFetchOK = FALSE;
    UINT16 i, j;
    UINT32 dwVLDByte, dwVLDBit;
    UINT32 dwVFifo;
    UINT32 dwVLDRemainByte;

    LOG(7, "fgVldInitBarrelShifter, position : %08x\n", dwPtr);

    dwVFifo = ((UINT32)rWMVDecInfo.rDram.pu1VFifo);
    dwVLDRemainByte = dwPtr & 0xf;

    for(i =0 ;i<5 ;i++) {
        _VDEC_UpdateVldRVWPtr(rWMVDecInfo.rOther.u4WPtr);

        vWriteREG(RW_VLD_RPTR, dwPtr);
      
        if (u4ReadREG(RO_VLD_VRPTR) != (dwPtr - (dwPtr % 16)))
        {
            LOG(1, "RPTR moved before INIFET\n");
            return FALSE;
        }
        vWriteREG(RW_VLD_PROC, (1<<20));

        if (fgWaitVldFetchOk())
        {
            fgFetchOK = TRUE;
            break;
        }
    }
    if(!fgFetchOK)
        return FALSE;
    vWriteREG(RW_VLD_PROC, (1<<23));

    dwVLDByte = dwRVVldRPtr(&dwVLDBit);

    for (j=0; j<dwVLDRemainByte; j++)
    {
        //mark by spork
        dwGetBitStream(8);
        dwVLDByte = dwRVVldRPtr(&dwVLDBit);
        if (dwVLDByte == (dwPtr-dwVFifo))
            break;
    }
    return (TRUE);
}

BOOL fgWaitVldFetchOk(void)
{
    UINT32 u4Tmp = 0;
    while ((u4ReadREG(RO_VLD_FETCHOK) & 0x1) == 0)
    //while((u4ReadREG(RO_VLD_FETCHOK) & 0x4) == 0)
    {
        u4Tmp++;
        if (u4Tmp > 0x10000)
        {
            return (FALSE);
        }
    }
    #ifdef WAIT_SRAM_STABLE
        u4Tmp = 0;
        while((u4ReadREG(RO_VLD_SRAMCTRL) & AA_FIT_TARGET_D)==0)
        {
            u4Tmp++;
            if(u4Tmp >= 0x100000)
            {
                LOG(3, "//===SRAM !Stable 0xF4=0x%x, Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n",
                u4ReadREG(RO_VLD_SRAMCTRL), u4ReadREG(RW_VLD_RPTR), u4ReadREG(RO_VLD_VRPTR),
                u4ReadREG(RO_VLD_VBAR), u4ReadREG(RO_VLD_FETCHOK));
                return FALSE;
            }
        }
        LOG(6, "//WaitFetchOK Rptr 0xB0=0x%x, 0xFC=0x%x, 0xEC=0x%x, 0xE8=0x%x\n",
        u4ReadREG(RW_VLD_RPTR), u4ReadREG(RO_VLD_VRPTR),
        u4ReadREG(RO_VLD_VBAR), u4ReadREG(RO_VLD_FETCHOK));
    #endif
    
    return (TRUE);
}
#endif  // 0

void vInitVParserRV(UCHAR ucEsId)
{
  VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
  VDEC_RV_INFO_T *prVdec;
  _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

  prVdec->rOther.u4DispBufIdx = 1;
  if (prVdec->rOther.u4DispBufIdx == 1)
  {
    vSetRVFRefBuf(prVdec, 0);
    vSetRVBRefBuf(prVdec, 1);
  }
  else
  {
    vSetRVFRefBuf(prVdec, 1);
    vSetRVBRefBuf(prVdec, 0);
  }
  
  //prVdec->rOther.u4DecPicNo = 0;
  prVdec->rOther.u4CurrPicNo = 0;
  prVdec->rOther.u4FrminfoPtr = 0;
  prVdec->rOther.u4TimeOutCnt = 0;
  prVdec->rOther.u4DecErrCnt = 0;
  prVdec->rOther.u4LastTimeOutPicNo = 0xFFFFFFFF;
  prVdec->rOther.u4LastDecErrPicNo = 0xFFFFFFFF;

  VDEC_MULTI_USE(_fgIsRV9) = prVdecEsInfo->fgRV9;
  VDEC_MULTI_USE(_dwRPRSizes) = prVdecEsInfo->au4RPRSizes;
  VDEC_MULTI_USE(_dwSzOfPicSz) = prVdecEsInfo->u4SzOfPicSz;
}


DWRD dVLDGetRVBitS(DWRD dShiftBit)
{
  DWRD dwRegVal;
  UINT32 u4Crit;

  u4Crit = HalCriticalStart();
  
  //dShiftBit <<= 2;  // to overcome + and << operator precedence
  //dwRegVal = dReadRVVLD((dShiftBit << 2));
  dwRegVal = u4ReadREG((UINT32)(RV_VLD_REG_OFST + (dShiftBit << 2)));

  HalCriticalEnd(u4Crit);

  return(dwRegVal);
}


DWRD dwGetRVBitStream(BYTE bShiftBit)
{
  DWRD dRegVal;

  _dwBitCount += bShiftBit;
  dRegVal = dVLDGetRVBitS((DWRD)bShiftBit);
  return(dRegVal);
}


DWRD dwGetRVHi(DWRD dwBits)
{
  DWRD dwRetVal;

  
  dwRetVal = (dwGetRVBitStream(0) >> (32 - dwBits));
  
  dwGetRVBitStream(dwBits);

  return dwRetVal;
}


void vResetRVBitStream(UCHAR ucEsId)
{
  UINT32 dwReadPtrRes, i;
  VDEC_RV_INFO_T *prVdec;
  _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

  //check if _dwVldReadPtr is DWRD aligned, if no get the bits first
#if 1  // !!!
  dwReadPtrRes = prVdec->rPesInfo.u4VldReadPtr & 0x0000000f;
#else
  dwReadPtrRes = _dwVldReadPtr & 0x0000000f;
#endif
  for(i=0;i<dwReadPtrRes;i++)
  {
    dwGetRVBitStream(8);
  }
  vWriteRVVLD(RW_RV_VLD_BYTECNT_RST,1);
  vWriteRVVLD(RW_RV_VLD_BYTECNT_RST,0);
  _dwBitCount = 0;
}


void vGetMbaSz(UCHAR ucEsId)
{
  DWRD i,j;
  DWRD dwMbs, dwGOBs;
  
  dwMbs = (VDEC_MULTI_USE(_dwPicWidth) + 15) >> 4;
  dwGOBs = (VDEC_MULTI_USE(_dwPicHeight) + 15) >> 4; 

  j = dwMbs *dwGOBs - 1;

  for (i = 0; i < (MBA_Widths - 1) && MBA_NumMBs[i] <  j; i++);

  VDEC_MULTI_USE(_dwMbaSz) = MBA_FieldWidth[i];
}

DWRD dwGetMbaSz(DWRD dwPicWidth, DWRD dwPicHeight)
{
  DWRD i,j;
  DWRD dwMbs, dwGOBs;
   
  dwMbs = (dwPicWidth + 15) >> 4;
  dwGOBs = (dwPicHeight + 15) >> 4; 

  j = dwMbs *dwGOBs - 1;

  for (i = 0; i < (MBA_Widths - 1) && MBA_NumMBs[i] <  j; i++);

  return MBA_FieldWidth[i];
}



void vGetPicSize(DWRD *dwPicWidth, DWRD *dwPicHeight)
{
  DWRD dwCode;	
  DWRD dwWidth, dwHieght;
  
  const DWRD code_width[8] = {160, 176, 240, 320, 352, 640, 704, 0};
  const DWRD code_height1[8] = {120, 132, 144, 240, 288, 480, 0, 0};
  const DWRD code_height2[4] = {180, 360, 576, 0};

  /* width */	
  dwCode = dwGetRVHi(3);
  dwWidth = code_width[dwCode];
  if (dwWidth == 0)
  {		
    do		
    {			
      dwCode = dwGetRVHi(8);
      dwWidth += (dwCode << 2);
    }		
    while (dwCode == 255);
  }
  /* height */
  dwCode = dwGetRVHi(3);
  dwHieght = code_height1[dwCode];
  if (dwHieght == 0)
  {
    dwCode <<= 1;
    dwCode |= dwGetRVHi(1);
    dwCode &= 3;
    dwHieght = code_height2[dwCode];
    if (dwHieght == 0)
    {
      do
      {
        dwCode = dwGetRVHi(8);
        dwHieght += (dwCode << 2);
      }
      while (dwCode == 255);
    }
  }
  *dwPicWidth = dwWidth;
  *dwPicHeight = dwHieght;
}


void vSetRVDecParam(UCHAR ucEsId, RvPicAttr *ptRvDecPrm)
{
  //int i;
  //DWRD idx;

  if (ucEsId >= VDEC_MAX_ES)
  {
    return;
  }

  ptRvDecPrm->fgRV9 = VDEC_MULTI_USE(_fgIsRV9);
  ptRvDecPrm->ePtype = (EnumRVPicCodType)VDEC_MULTI_USE(_dwPicType);
  if((VDEC_MULTI_USE(_dwPicType) == RV_INTRAPIC) || (VDEC_MULTI_USE(_dwPicType) == RV_FORCED_INTRAPIC))  // 0528
  {
    ptRvDecPrm->fgBwdIsI = TRUE;
  }
  else if(VDEC_MULTI_USE(_dwPicType) == RV_INTERPIC)  // 0528
  {
    ptRvDecPrm->fgBwdIsI = FALSE;
  }
  ptRvDecPrm->u4OrgWidth = VDEC_MULTI_USE(_dwPicWidth);
  ptRvDecPrm->u4OrgHeight = VDEC_MULTI_USE(_dwPicHeight);
  ptRvDecPrm->u4Width = ((VDEC_MULTI_USE(_dwPicWidth) + 15) >> 4) << 4;
  ptRvDecPrm->u4Height = ((VDEC_MULTI_USE(_dwPicHeight) + 15) >> 4) << 4;
  ptRvDecPrm->u4Pquant = VDEC_MULTI_USE(_dwPSQuant);
  ptRvDecPrm->u4Oquant = VDEC_MULTI_USE(_dwOSVQuant);
  ptRvDecPrm->u4DFP = VDEC_MULTI_USE(_fgDeblockPassThru);
  ptRvDecPrm->u4Tr = VDEC_MULTI_USE(_dwTr);
  ptRvDecPrm->u4Iratio = VDEC_MULTI_USE(_dwIRatio);
  ptRvDecPrm->u4MbaSize = VDEC_MULTI_USE(_dwMbaSz);
  ptRvDecPrm->u4Mba = VDEC_MULTI_USE(_dwMba);
  ptRvDecPrm->u4Rtype = VDEC_MULTI_USE(_dwRType);
  ptRvDecPrm->u4PctszSize = VDEC_MULTI_USE(_dwSzOfPicSz);
  ptRvDecPrm->u4Pctsz = VDEC_MULTI_USE(_dwPicSz);
  //ptRvDecPrm->u4NumSlice = _ptPicIdx[_dwPicRdIdx].dwSliceNum;  !!!
  ptRvDecPrm->u4BstLength = 0;
 
#if 0  // !!!
  for( i=0; i < ptRvDecPrm->u4NumSlice; i++)
  {
    idx = _ptPicIdx[_dwPicRdIdx].dwSliceSzBufWp + i;
    if (idx >= _dwSliceSzBufNs)
    {
      idx -= _dwSliceSzBufNs;
    }
    ptRvDecPrm->au4SliceSize[i] = *(_pdwSliceSzBufSa + idx);
    ptRvDecPrm->u4BstLength += (*(_pdwSliceSzBufSa + idx));
  }
#endif
  ptRvDecPrm->u4HdrSkip = _dwBitCount;
}


void vGetPicSizeEx(RV_RING_BUFF_T *prRingBuf, DWRD *dwPicWidth, DWRD *dwPicHeight)
{


  DWRD dwCode;	
  DWRD dwWidth, dwHieght;
  
  const DWRD code_width[8] = {160, 176, 240, 320, 352, 640, 704, 0};
  const DWRD code_height1[8] = {120, 132, 144, 240, 288, 480, 0, 0};
  const DWRD code_height2[4] = {180, 360, 576, 0};

  /* width */	
  dwCode = RV_GetBit(3);
  dwWidth = code_width[dwCode];
  if (dwWidth == 0)
  {		
    do		
    {			
      dwCode = RV_GetBit(8);
      dwWidth += (dwCode << 2);
    }		
    while (dwCode == 255);
  }
  /* height */
  dwCode = RV_GetBit(3);
  dwHieght = code_height1[dwCode];
  if (dwHieght == 0)
  {
    dwCode <<= 1;
    dwCode |= RV_GetBit(1);
    dwCode &= 3;
    dwHieght = code_height2[dwCode];
    if (dwHieght == 0)
    {
      do
      {
        dwCode = RV_GetBit(8);
        dwHieght += (dwCode << 2);
      }
      while (dwCode == 255);
    }
  }
  *dwPicWidth = dwWidth;
  *dwPicHeight = dwHieght;
}
#if 0
VOID vDumpVariable(UCHAR ucEsId)
{
    Printf("vDumpVariable\n");

    Printf("_dwPSQuant: %d\n", VDEC_MULTI_USE(_dwPSQuant));
    Printf("_dwOSVQuant: %d\n", VDEC_MULTI_USE(_dwOSVQuant));
    Printf("_fgDeblockPassThru: %d\n", VDEC_MULTI_USE(_fgDeblockPassThru));
    Printf("_dwTr: %d\n", VDEC_MULTI_USE(_dwTr));
    Printf("_i4Trd: %d\n", VDEC_MULTI_USE(_i4Trd));
    Printf("_i4Trb: %d\n", VDEC_MULTI_USE(_i4Trb));
    Printf("_dwTrWarp: %d\n", VDEC_MULTI_USE(_dwTrWarp));
    Printf("_dwPicSz: %d\n", VDEC_MULTI_USE(_dwPicSz));
    Printf("_dwMbaSz: %d\n", VDEC_MULTI_USE(_dwMbaSz));
    Printf("_dwMba: %d\n", VDEC_MULTI_USE(_dwMba));
    Printf("_dwRType: %d\n", VDEC_MULTI_USE(_dwRType));
    Printf("_dwIRatio: %d\n", VDEC_MULTI_USE(_dwIRatio));
    Printf("_dwPicWidth: %d\n", VDEC_MULTI_USE(_dwPicWidth));
    Printf("_dwPicHeight: %d\n", VDEC_MULTI_USE(_dwPicHeight));
    Printf("_dwSzOfPicSz: %d\n", VDEC_MULTI_USE(_dwSzOfPicSz));
    Printf("_dwPicType: %d\n", VDEC_MULTI_USE(_dwPicType));
    Printf("_dwHdrBitCnt: %d\n", VDEC_MULTI_USE(_dwHdrBitCnt));

}
#endif

void RVParseFrameHeader(UCHAR ucEsId, RvPicAttr *prPic)
{
    DWRD dwBitVersion, dwTrb, dwTrd, dwRatio0, dwRatio1;
    //DWRD dwNextRdIdx, dwNextPicSz;
    BOOL fgECC;
    BOOL fgInterlaced;
    VDEC_RV_INFO_T *prVdec;
    RV_RING_BUFF_T rRingBuff;
    RV_RING_BUFF_T *prRingBuf;

    if (ucEsId >= VDEC_MAX_ES)
    {
      return;
    }
    _VDEC_RVGetWholePesStruct(ucEsId, (void **)&prVdec);

    prRingBuf = &rRingBuff;
    rRingBuff.ui4Start  = VIRTUAL(prVdec->rPesInfo.u4FifoStart);
    rRingBuff.ui4End    = VIRTUAL(prVdec->rPesInfo.u4FifoEnd);
    rRingBuff.ui4Rp     = VIRTUAL(prVdec->rPesInfo.u4VldReadPtr);
    rRingBuff.ui1Left   = 8;

    if(VDEC_MULTI_USE(_fgIsRV9)) //codec is RV9 or RV10
    {
        //_bMpegVer = V_RV9;  !!!
        fgECC = RV_GetBit(1);
        VDEC_MULTI_USE(_dwPicType)  = RV_GetBit(2);
        VDEC_MULTI_USE(_dwPSQuant) = RV_GetBit(5);
        dwBitVersion = RV_GetBit(1);
        fgInterlaced = RV_GetBit(1);
        if((dwBitVersion != 0) || (fgInterlaced))
        {
          //return RV_SYNTAX_ERROR1;  megaa 20080723: BMW-2.rmvb #0 has this error
        }
        VDEC_MULTI_USE(_dwOSVQuant) = RV_GetBit(2);
        VDEC_MULTI_USE(_fgDeblockPassThru) = RV_GetBit(1);
        VDEC_MULTI_USE(_dwTr) = RV_GetBit(13);
        if(VDEC_MULTI_USE(_dwPicType) != RV_TRUEBPIC)
        {
            VDEC_MULTI_USE(_dwFRefTr) = VDEC_MULTI_USE(_dwBRefTr);
            VDEC_MULTI_USE(_dwBRefTr) = VDEC_MULTI_USE(_dwTr);
            // calculate IRatio
            VDEC_MULTI_USE(_dwIRatio) = 0;
        }
        else
        {
            VDEC_MULTI_USE(_i4Trd) = VDEC_MULTI_USE(_dwBRefTr) - VDEC_MULTI_USE(_dwFRefTr);
            VDEC_MULTI_USE(_i4Trb) =VDEC_MULTI_USE(_dwTr) -  VDEC_MULTI_USE(_dwFRefTr);
            // calculate IRatio
            dwTrd = (VDEC_MULTI_USE(_i4Trd) < 0) ? (VDEC_MULTI_USE(_i4Trd) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trd);
            dwTrb = (VDEC_MULTI_USE(_i4Trb) < 0) ? (VDEC_MULTI_USE(_i4Trb) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trb);
            /* just in case TR's are incorrect */
            if(dwTrb > dwTrd)					
            {
                dwTrb=0;
            }
            if(dwTrd>0) 
            {			
                dwRatio0 = (dwTrb << 14) / dwTrd;
                dwRatio1 = ((dwTrd - dwTrb) << 14) / dwTrd;
                VDEC_MULTI_USE(_dwIRatio) = (dwRatio1 << 16 | dwRatio0);
            }		
            else 		
            {			
                VDEC_MULTI_USE(_dwIRatio) = 0;	
            }
        }
        // get dimension
        if((VDEC_MULTI_USE(_dwPicType) == RV_INTERPIC) || (VDEC_MULTI_USE(_dwPicType) == RV_TRUEBPIC))
        {
            if(!RV_GetBit(1))
            {
                vGetPicSizeEx(&rRingBuff, &VDEC_MULTI_USE(_dwPicWidth), &VDEC_MULTI_USE(_dwPicHeight));
            }
        }
        else
        {
            vGetPicSizeEx(&rRingBuff, &VDEC_MULTI_USE(_dwPicWidth), &VDEC_MULTI_USE(_dwPicHeight));
        }
        // ~get dimension
        vGetMbaSz(ucEsId);
        VDEC_MULTI_USE(_dwMba) = RV_GetBit(VDEC_MULTI_USE(_dwMbaSz));
    }
    else //codec is RV8
    {
        //_bMpegVer = V_RV8;  !!!
        dwBitVersion = RV_GetBit(3);
        VDEC_MULTI_USE(_dwPicType)  = RV_GetBit(2);
        fgECC = RV_GetBit(1);
        VDEC_MULTI_USE(_dwPSQuant) = RV_GetBit(5);
        VDEC_MULTI_USE(_fgDeblockPassThru) = RV_GetBit(1);
        VDEC_MULTI_USE(_dwTr) = RV_GetBit(13);

        if(VDEC_MULTI_USE(_dwPicType) != RV_TRUEBPIC)
        {
            VDEC_MULTI_USE(_dwFRefTr) = VDEC_MULTI_USE(_dwBRefTr);
            VDEC_MULTI_USE(_dwBRefTr) = VDEC_MULTI_USE(_dwTr);
            // calculate IRatio
            VDEC_MULTI_USE(_dwIRatio) = 0;
        }
        else
        {
            VDEC_MULTI_USE(_i4Trd) = VDEC_MULTI_USE(_dwBRefTr) - VDEC_MULTI_USE(_dwFRefTr);
            VDEC_MULTI_USE(_i4Trb) =VDEC_MULTI_USE(_dwTr) -  VDEC_MULTI_USE(_dwFRefTr);
            // calculate IRatio
            dwTrd = (VDEC_MULTI_USE(_i4Trd) < 0) ? (VDEC_MULTI_USE(_i4Trd) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trd);
            dwTrb = (VDEC_MULTI_USE(_i4Trb) < 0) ? (VDEC_MULTI_USE(_i4Trb) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trb);
            /* just in case TR's are incorrect */
            if(dwTrb > dwTrd)					
            {
                dwTrb=0;
            }
            if(dwTrd>0) 
            {			
                dwRatio0 = (dwTrb << 14) / dwTrd;
                dwRatio1 = ((dwTrd - dwTrb) << 14) / dwTrd;
                VDEC_MULTI_USE(_dwIRatio) = (dwRatio1 << 16 | dwRatio0);
            }		
            else 		
            {			
                VDEC_MULTI_USE(_dwIRatio) = 0;	
            }
        }

        if(VDEC_MULTI_USE(_dwSzOfPicSz) > 0)
        {
            VDEC_MULTI_USE(_dwPicSz) = RV_GetBit(VDEC_MULTI_USE(_dwSzOfPicSz));
            VDEC_MULTI_USE(_dwPicWidth) = VDEC_MULTI_USE(_dwRPRSizes)[2*VDEC_MULTI_USE(_dwPicSz)];
            VDEC_MULTI_USE(_dwPicHeight) = VDEC_MULTI_USE(_dwRPRSizes)[2*VDEC_MULTI_USE(_dwPicSz) + 1]; 
        }
        vGetMbaSz(ucEsId);
        VDEC_MULTI_USE(_dwMba) = RV_GetBit(VDEC_MULTI_USE(_dwMbaSz));
        VDEC_MULTI_USE(_dwRType) = RV_GetBit(1);
    }
    VDEC_MULTI_USE(_dwHdrBitCnt) = _dwBitCount;

    vSetRVDecParam(ucEsId, prPic);

    if(((VDEC_MULTI_USE(_dwHSize) != VDEC_MULTI_USE(_dwPicWidth))||(VDEC_MULTI_USE(_dwVSize) != VDEC_MULTI_USE(_dwPicHeight))))
    {
        VDEC_MULTI_USE(_dwHSize) = VDEC_MULTI_USE(_dwPicWidth);
        VDEC_MULTI_USE(_dwVSize) =  VDEC_MULTI_USE(_dwPicHeight);
    }
    else
    {
        VDEC_MULTI_USE(_dwHSize) = VDEC_MULTI_USE(_dwPicWidth);
        VDEC_MULTI_USE(_dwVSize) =  VDEC_MULTI_USE(_dwPicHeight);
    }

    UNUSED(fgECC);

}


void RVVideoDecDecodeFrameHead(UCHAR ucEsId, RvPicAttr *prPic)
{
  //int i;
  DWRD dwBitVersion, dwTrb, dwTrd, dwRatio0, dwRatio1;
  //DWRD dwNextRdIdx, dwNextPicSz;
  BOOL fgECC;
  BOOL fgInterlaced;
 
  if (ucEsId >= VDEC_MAX_ES)
  {
    return;
  }
  
  vResetRVBitStream(ucEsId);
  //_fgVopCoded0 = FALSE;  !!!

  if(VDEC_MULTI_USE(_fgIsRV9)) //codec is RV9 or RV10
  {
    //_bMpegVer = V_RV9;  !!!
    fgECC = dwGetRVHi(1);
    VDEC_MULTI_USE(_dwPicType)  = dwGetRVHi(2);
    VDEC_MULTI_USE(_dwPSQuant) = dwGetRVHi(5);
    dwBitVersion = dwGetRVHi(1);
    fgInterlaced = dwGetRVHi(1);
    if((dwBitVersion != 0) || (fgInterlaced))
    {
      //return RV_SYNTAX_ERROR1;  megaa 20080723: BMW-2.rmvb #0 has this error
    }
    VDEC_MULTI_USE(_dwOSVQuant) = dwGetRVHi(2);
    VDEC_MULTI_USE(_fgDeblockPassThru) = dwGetRVHi(1);
    VDEC_MULTI_USE(_dwTr) = dwGetRVHi(13);
    if(VDEC_MULTI_USE(_dwPicType) != RV_TRUEBPIC)
    {
      VDEC_MULTI_USE(_dwFRefTr) = VDEC_MULTI_USE(_dwBRefTr);
      VDEC_MULTI_USE(_dwBRefTr) = VDEC_MULTI_USE(_dwTr);
      // calculate IRatio
      VDEC_MULTI_USE(_dwIRatio) = 0;
    }
    else
    {
      VDEC_MULTI_USE(_i4Trd) = VDEC_MULTI_USE(_dwBRefTr) - VDEC_MULTI_USE(_dwFRefTr);
      VDEC_MULTI_USE(_i4Trb) =VDEC_MULTI_USE(_dwTr) -  VDEC_MULTI_USE(_dwFRefTr);
      // calculate IRatio
      dwTrd = (VDEC_MULTI_USE(_i4Trd) < 0) ? (VDEC_MULTI_USE(_i4Trd) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trd);
      dwTrb = (VDEC_MULTI_USE(_i4Trb) < 0) ? (VDEC_MULTI_USE(_i4Trb) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trb);
      /* just in case TR's are incorrect */
      if(dwTrb > dwTrd)					
      {
        dwTrb=0;
      }
      if(dwTrd>0) 
      {			
        dwRatio0 = (dwTrb << 14) / dwTrd;
        dwRatio1 = ((dwTrd - dwTrb) << 14) / dwTrd;
        VDEC_MULTI_USE(_dwIRatio) = (dwRatio1 << 16 | dwRatio0);
      }		
      else 		
      {			
        VDEC_MULTI_USE(_dwIRatio) = 0;	
      }
    }
    // get dimension
    if((VDEC_MULTI_USE(_dwPicType) == RV_INTERPIC) || (VDEC_MULTI_USE(_dwPicType) == RV_TRUEBPIC))
    {
      if(!dwGetRVHi(1))
      {
        vGetPicSize(&VDEC_MULTI_USE(_dwPicWidth), &VDEC_MULTI_USE(_dwPicHeight));
      }
    }
    else
    {
      vGetPicSize(&VDEC_MULTI_USE(_dwPicWidth), &VDEC_MULTI_USE(_dwPicHeight));
    }
    // ~get dimension
    vGetMbaSz(ucEsId);
    VDEC_MULTI_USE(_dwMba) = dwGetRVHi(VDEC_MULTI_USE(_dwMbaSz));
  }
  else //codec is RV8
  {
    //_bMpegVer = V_RV8;  !!!
    dwBitVersion = dwGetRVHi(3);
    VDEC_MULTI_USE(_dwPicType)  = dwGetRVHi(2);
    fgECC = dwGetRVHi(1);
    VDEC_MULTI_USE(_dwPSQuant) = dwGetRVHi(5);
    VDEC_MULTI_USE(_fgDeblockPassThru) = dwGetRVHi(1);
    VDEC_MULTI_USE(_dwTr) = dwGetRVHi(13);

    if(VDEC_MULTI_USE(_dwPicType) != RV_TRUEBPIC)
    {
      VDEC_MULTI_USE(_dwFRefTr) = VDEC_MULTI_USE(_dwBRefTr);
      VDEC_MULTI_USE(_dwBRefTr) = VDEC_MULTI_USE(_dwTr);
      // calculate IRatio
      VDEC_MULTI_USE(_dwIRatio) = 0;
    }
    else
    {
      VDEC_MULTI_USE(_i4Trd) = VDEC_MULTI_USE(_dwBRefTr) - VDEC_MULTI_USE(_dwFRefTr);
      VDEC_MULTI_USE(_i4Trb) =VDEC_MULTI_USE(_dwTr) -  VDEC_MULTI_USE(_dwFRefTr);
      // calculate IRatio
      dwTrd = (VDEC_MULTI_USE(_i4Trd) < 0) ? (VDEC_MULTI_USE(_i4Trd) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trd);
      dwTrb = (VDEC_MULTI_USE(_i4Trb) < 0) ? (VDEC_MULTI_USE(_i4Trb) + VDEC_MULTI_USE(_dwTrWarp)) : VDEC_MULTI_USE(_i4Trb);
      /* just in case TR's are incorrect */
      if(dwTrb > dwTrd)					
      {
        dwTrb=0;
      }
      if(dwTrd>0) 
      {			
        dwRatio0 = (dwTrb << 14) / dwTrd;
        dwRatio1 = ((dwTrd - dwTrb) << 14) / dwTrd;
        VDEC_MULTI_USE(_dwIRatio) = (dwRatio1 << 16 | dwRatio0);
      }		
      else 		
      {			
        VDEC_MULTI_USE(_dwIRatio) = 0;	
      }
    }

    if(VDEC_MULTI_USE(_dwSzOfPicSz) > 0)
    {
      VDEC_MULTI_USE(_dwPicSz) = dwGetRVHi(VDEC_MULTI_USE(_dwSzOfPicSz));
      VDEC_MULTI_USE(_dwPicWidth) = VDEC_MULTI_USE(_dwRPRSizes)[2*VDEC_MULTI_USE(_dwPicSz)];
      VDEC_MULTI_USE(_dwPicHeight) = VDEC_MULTI_USE(_dwRPRSizes)[2*VDEC_MULTI_USE(_dwPicSz) + 1]; 
    }
    vGetMbaSz(ucEsId);
    
    VDEC_MULTI_USE(_dwMba) = dwGetRVHi(VDEC_MULTI_USE(_dwMbaSz));
    VDEC_MULTI_USE(_dwRType) = dwGetRVHi(1);
  }
  VDEC_MULTI_USE(_dwHdrBitCnt) = _dwBitCount;

  vSetRVDecParam(ucEsId, prPic);

  if(((VDEC_MULTI_USE(_dwHSize) != VDEC_MULTI_USE(_dwPicWidth))||(VDEC_MULTI_USE(_dwVSize) != VDEC_MULTI_USE(_dwPicHeight))))
  {
    VDEC_MULTI_USE(_dwHSize) = VDEC_MULTI_USE(_dwPicWidth);
    VDEC_MULTI_USE(_dwVSize) =  VDEC_MULTI_USE(_dwPicHeight);
  }
  else
  {
    VDEC_MULTI_USE(_dwHSize) = VDEC_MULTI_USE(_dwPicWidth);
    VDEC_MULTI_USE(_dwVSize) =  VDEC_MULTI_USE(_dwPicHeight);
  }

  UNUSED(fgECC);

  return;
}



void RVConsumeFrameHeader(UCHAR ucEsId)
{
  //int i;
  DWRD dwPicType = 0, dwMbaSz;
  DWRD dwPicWidth, dwPicHeight, dwPicSz = 0;

  if (ucEsId >= VDEC_MAX_ES)
  {
    return;
  }

  vResetRVBitStream(ucEsId);
  //_fgVopCoded0 = FALSE;  !!!

  if(VDEC_MULTI_USE(_fgIsRV9)) //codec is RV9 or RV10
  {
    //_bMpegVer = V_RV9;  !!!
    dwGetRVHi(1);
    dwPicType = dwGetRVHi(2);
    #if 0
    dwGetRVHi(5);
    dwGetRVHi(1);
    dwGetRVHi(1);
    dwGetRVHi(2);
    dwGetRVHi(1);
    dwGetRVHi(13);
    #else
    dwGetRVHi(23);
    #endif
    // get dimension
    dwPicWidth = VDEC_MULTI_USE(_dwPicWidth);
    dwPicHeight = VDEC_MULTI_USE(_dwPicHeight);

    if((dwPicType == RV_INTERPIC) || (dwPicType == RV_TRUEBPIC))
    {
      if(!dwGetRVHi(1))
      {
        //vGetPicSize(&VDEC_MULTI_USE(_dwPicWidth), &VDEC_MULTI_USE(_dwPicHeight));
        vGetPicSize(&dwPicWidth, &dwPicHeight);
      }
    }
    else
    {
      //vGetPicSize(&VDEC_MULTI_USE(_dwPicWidth), &VDEC_MULTI_USE(_dwPicHeight));
      vGetPicSize(&dwPicWidth, &dwPicHeight);
    }

    // ~get dimension
    dwMbaSz = dwGetMbaSz(dwPicWidth, dwPicHeight);
    dwGetRVHi(dwMbaSz);
  }
  else //codec is RV8
  {
    //_bMpegVer = V_RV8;  !!!
    #if 0
    dwGetRVHi(3);
    dwGetRVHi(2);
    dwGetRVHi(1);
    dwGetRVHi(5);
    dwGetRVHi(1);
    dwGetRVHi(13);
    #else
    dwGetRVHi(25);
    #endif

    if(VDEC_MULTI_USE(_dwSzOfPicSz) > 0)
    {
      dwPicSz = dwGetRVHi(VDEC_MULTI_USE(_dwSzOfPicSz));
      dwPicWidth = VDEC_MULTI_USE(_dwRPRSizes)[2*dwPicSz];
      dwPicHeight = VDEC_MULTI_USE(_dwRPRSizes)[2*dwPicSz + 1]; 
    }
    dwMbaSz = dwGetMbaSz(dwPicWidth, dwPicHeight);
   
    dwGetRVHi(dwMbaSz);
    dwGetRVHi(1);
  }

  return;
}

