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

typedef enum
{
	RV_FID_REALVIDEO30,        /* 0 (00) */
	RV_FID_RV89COMBO           /* 1 (01) */
} EnumRVBitsVersion;

DWRD *_dwRPRSizes;  //DWRD _dwRPRSizes[2 * MAX_NUM_RPR_SIZES];  !!!
BOOL _fgIsRV9;
DWRD _dwSliceNum;
DWRD _dwSliceSize;
DWRD _dwSizeOfPicSz;
DWRD _dwPSQuant;
DWRD _dwOSVQuant; // only RV9
BOOL _fgDeblockPassThru;
DWRD _dwTr;
INT32 _i4Trd;
INT32 _i4Trb;
DWRD _dwTrWarp = TR_WRAP_RV;  // ToDo: water-proof it
DWRD _dwPicSz;  // only RV8
DWRD _dwMbaSz;
DWRD _dwMba;
DWRD _dwRType; // only RV8
DWRD _dwIRatio;
DWRD _dwPicWidth;
DWRD _dwPicHeight;
DWRD _dwSzOfPicSz;
DWRD _dwNextPicWidth;
DWRD _dwNextPicHeight;
DWRD _dwNextPicType;
DWRD _dwPicType; 
DWRD _dwPrevPicWidth;
DWRD _dwPrevPicHeight;
DWRD _dwHdrBitCnt;
DWRD _dwRVFrmRateCode;


/*extern */DWRD _dwBitCount;
/*extern */DWRD _dwHSize;
/*extern */DWRD _dwVSize;

DWRD _dwRv8TblNum;
DWRD _dwRefQpMb0;
DWRD _dwFRefTr;
DWRD _dwBRefTr;


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

  _rRVDecInfo.rOther.u4DispBufIdx = 1;
  if (_rRVDecInfo.rOther.u4DispBufIdx == 1)
  {
    vSetRVFRefBuf(0);
    vSetRVBRefBuf(1);
  }
  else
  {
    vSetRVFRefBuf(1);
    vSetRVBRefBuf(0);
  }
  
  //_rRVDecInfo.rOther.u4DecPicNo = 0;
  _rRVDecInfo.rOther.u4CurrPicNo = 0;
  _rRVDecInfo.rOther.u4FrminfoPtr = 0;
  _rRVDecInfo.rOther.u4TimeOutCnt = 0;
  _rRVDecInfo.rOther.u4DecErrCnt = 0;
  _rRVDecInfo.rOther.u4LastTimeOutPicNo = 0xFFFFFFFF;
  _rRVDecInfo.rOther.u4LastDecErrPicNo = 0xFFFFFFFF;

  _fgIsRV9 = prVdecEsInfo->fgRV9;
  _dwRPRSizes = prVdecEsInfo->au4RPRSizes;
  _dwSzOfPicSz = prVdecEsInfo->u4SzOfPicSz;
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


void vResetRVBitStream(void)
{
  UINT32 dwReadPtrRes, i;

  //check if _dwVldReadPtr is DWRD aligned, if no get the bits first
#if 1  // !!!
  dwReadPtrRes = _rRVDecInfo.rPesInfo.u4VldReadPtr & 0x0000000f;
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


void vGetMbaSz(void)
{
  DWRD i,j;
  DWRD dwMbs, dwGOBs;
  
   
  dwMbs = (_dwPicWidth + 15) >> 4;
  dwGOBs = (_dwPicHeight + 15) >> 4; 

  j = dwMbs *dwGOBs - 1;

  for (i = 0; i < (MBA_Widths - 1) && MBA_NumMBs[i] <  j; i++);

  _dwMbaSz = MBA_FieldWidth[i];
  
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


void vSetRVDecParam(RvPicAttr *ptRvDecPrm)
{
  //int i;
  //DWRD idx;

  ptRvDecPrm->fgRV9 = _fgIsRV9;
  ptRvDecPrm->ePtype = (EnumRVPicCodType)_dwPicType;
  if((_dwPicType == RV_INTRAPIC) || (_dwPicType == RV_FORCED_INTRAPIC))  // 0528
  {
    ptRvDecPrm->fgBwdIsI = TRUE;
  }
  else if(_dwPicType == RV_INTERPIC)  // 0528
  {
    ptRvDecPrm->fgBwdIsI = FALSE;
  }
  ptRvDecPrm->u4OrgWidth = _dwPicWidth;
  ptRvDecPrm->u4OrgHeight = _dwPicHeight;
  ptRvDecPrm->u4Width = ((_dwPicWidth + 15) >> 4) << 4;
  ptRvDecPrm->u4Height = ((_dwPicHeight + 15) >> 4) << 4;
  ptRvDecPrm->u4Pquant = _dwPSQuant;
  ptRvDecPrm->u4Oquant = _dwOSVQuant;
  ptRvDecPrm->u4DFP = _fgDeblockPassThru;
  ptRvDecPrm->u4Tr = _dwTr;
  ptRvDecPrm->u4Iratio = _dwIRatio;
  ptRvDecPrm->u4MbaSize = _dwMbaSz;
  ptRvDecPrm->u4Mba = _dwMba;
  ptRvDecPrm->u4Rtype = _dwRType;
  ptRvDecPrm->u4PctszSize = _dwSzOfPicSz;
  ptRvDecPrm->u4Pctsz = _dwPicSz;
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


void RVVideoDecDecodeFrameHead(RvPicAttr *prPic)
{
  //int i;
  DWRD dwBitVersion, dwTrb, dwTrd, dwRatio0, dwRatio1;
  //DWRD dwNextRdIdx, dwNextPicSz;
  BOOL fgECC;
  BOOL fgInterlaced;
  
  vResetRVBitStream();
  //_fgVopCoded0 = FALSE;  !!!

  //_dwPrevPicWidth = _dwPicWidth;  !!!
  //_dwPrevPicHeight = _dwPicHeight;  !!!
  if(_fgIsRV9) //codec is RV9 or RV10
  {
    //_bMpegVer = V_RV9;  !!!
    fgECC = dwGetRVHi(1);
    _dwPicType  = dwGetRVHi(2);
    _dwPSQuant = dwGetRVHi(5);
    dwBitVersion = dwGetRVHi(1);
    fgInterlaced = dwGetRVHi(1);
    if((dwBitVersion != 0) || (fgInterlaced))
    {
      //return RV_SYNTAX_ERROR1;  megaa 20080723: BMW-2.rmvb #0 has this error
    }
    _dwOSVQuant = dwGetRVHi(2);
    _fgDeblockPassThru = dwGetRVHi(1);
    _dwTr = dwGetRVHi(13);
    if(_dwPicType != RV_TRUEBPIC)
    {
      _dwFRefTr = _dwBRefTr;
      _dwBRefTr = _dwTr;
      // calculate IRatio
      _dwIRatio = 0;
    }
    else
    {
      _i4Trd = _dwBRefTr - _dwFRefTr;
      _i4Trb =_dwTr -  _dwFRefTr;
      // calculate IRatio
      dwTrd = (_i4Trd < 0) ? (_i4Trd + _dwTrWarp) : _i4Trd;
      dwTrb = (_i4Trb < 0) ? (_i4Trb + _dwTrWarp) : _i4Trb;
      /* just in case TR's are incorrect */
      if(dwTrb > dwTrd)					
      {
        dwTrb=0;
      }
      if(dwTrd>0) 
      {			
        dwRatio0 = (dwTrb << 14) / dwTrd;
        dwRatio1 = ((dwTrd - dwTrb) << 14) / dwTrd;
        _dwIRatio = (dwRatio1 << 16 | dwRatio0);
      }		
      else 		
      {			
        _dwIRatio = 0;	
      }
    }
    // get dimension
    if((_dwPicType == RV_INTERPIC) || (_dwPicType == RV_TRUEBPIC))
    {
      if(!dwGetRVHi(1))
      {
        vGetPicSize(&_dwPicWidth, &_dwPicHeight);
      }
    }
    else
    {
      vGetPicSize(&_dwPicWidth, &_dwPicHeight);
    }
    // ~get dimension
    vGetMbaSz();
    _dwMba = dwGetRVHi(_dwMbaSz);
  }
  else //codec is RV8
  {
    //_bMpegVer = V_RV8;  !!!
    dwBitVersion = dwGetRVHi(3);
    _dwPicType  = dwGetRVHi(2);
    fgECC = dwGetRVHi(1);
    _dwPSQuant = dwGetRVHi(5);
    _fgDeblockPassThru = dwGetRVHi(1);
    _dwTr = dwGetRVHi(13);

    if(_dwPicType != RV_TRUEBPIC)
    {
      _dwFRefTr = _dwBRefTr;
      _dwBRefTr = _dwTr;
      // calculate IRatio
      _dwIRatio = 0;
    }
    else
    {
      _i4Trd = _dwBRefTr - _dwFRefTr;
      _i4Trb =_dwTr -  _dwFRefTr;
      // calculate IRatio
      dwTrd = (_i4Trd < 0) ? (_i4Trd + _dwTrWarp) : _i4Trd;
      dwTrb = (_i4Trb < 0) ? (_i4Trb + _dwTrWarp) : _i4Trb;
      /* just in case TR's are incorrect */
      if(dwTrb > dwTrd)					
      {
        dwTrb=0;
      }
      if(dwTrd>0) 
      {			
        dwRatio0 = (dwTrb << 14) / dwTrd;
        dwRatio1 = ((dwTrd - dwTrb) << 14) / dwTrd;
        _dwIRatio = (dwRatio1 << 16 | dwRatio0);
      }		
      else 		
      {			
        _dwIRatio = 0;	
      }
    }

    if(_dwSzOfPicSz > 0)
    {
      _dwPicSz = dwGetRVHi(_dwSzOfPicSz);
      _dwPicWidth = _dwRPRSizes[2*_dwPicSz];
      _dwPicHeight = _dwRPRSizes[2*_dwPicSz + 1]; 
    }
    vGetMbaSz();
    _dwMba = dwGetRVHi(_dwMbaSz);
    _dwRType = dwGetRVHi(1);
  }
  _dwHdrBitCnt = _dwBitCount;

  vSetRVDecParam(prPic);

#if 0  // !!!
  if(!fgInquiry)
  {
    // get next decode frame picture width and height
#if 1  // Ginny 0612
    dwNextRdIdx = _dwPicRdIdx;
#endif
    do
    {
#if 1  // Ginny 0612
      dwNextRdIdx = dwGetNextPicIdx(dwNextRdIdx);
#else
      dwNextRdIdx = dwGetNextPicIdx(_dwPicRdIdx);
#endif
      if(_ptPicIdx[dwNextRdIdx].dwVtype == SEQ_END)
      {
        goto PARSE_END;
      }
      if(dwNextRdIdx == _dwPicWrIdx)
      {
        return(PIC_INDEX_EMPTY);
      }
    }while((!fgIsRealPic(_ptPicIdx[dwNextRdIdx].dwVtype)) ||
    ((_dwIPBMode == IP_MODE)&&(fgIsBType(_ptPicIdx[dwNextRdIdx].dwVtype))));

#if 1  // Ginny 0612
    _dwVldReadPtr = _ptPicIdx[dwNextRdIdx].dwVaddr;
    vVLDSetRp(_dwVldReadPtr);
#else
    vVldSetReadPtr(_ptPicIdx[dwNextRdIdx].dwVaddr); 
#endif
    if(fgInitBarrelShifter(_ptPicIdx[dwNextRdIdx].dwVaddr, TRUE) == FALSE)
    {
      return(BAR_SHIFTER_ERR);
    }
    vResetRVBitStream();
    if(_fgIsRV9)
    {
      dwGetRVHi(1);
      _dwNextPicType = dwGetRVHi(2);
      dwGetRVHi(23);
      // get dimension
      if((_dwNextPicType == RV_INTERPIC) || (_dwNextPicType == RV_TRUEBPIC))
      {
        if(!dwGetRVHi(1))
        {
          vGetPicSize(&_dwNextPicWidth, &_dwNextPicHeight);
        }
        else
        {
          _dwNextPicWidth = _dwPicWidth;
          _dwNextPicHeight = _dwPicHeight;
        }
      }
      else
      {
        vGetPicSize(&_dwNextPicWidth, &_dwNextPicHeight);
      }
    }
    else
    {
#if 1  // Ginny 0616
      dwGetRVHi(3);
      _dwNextPicType = dwGetRVHi(2);
      dwGetRVHi(20);
#else
      dwGetRVHi(25);
#endif
      if(_dwSzOfPicSz > 0)
      {
        dwNextPicSz = dwGetRVHi(_dwSzOfPicSz);
        _dwNextPicWidth = _dwRPRSizes[2*dwNextPicSz];
        _dwNextPicHeight = _dwRPRSizes[2*dwNextPicSz + 1]; 
      }
      else
      {
        _dwNextPicWidth = _dwPicWidth;
        _dwNextPicHeight = _dwPicHeight;
      }
    }
    // ~get next decode frame picture width and height
    // initial vld to the start of current decoded frame
#if 1  // Ginny 0612
    _dwVldReadPtr = _ptPicIdx[_dwPicRdIdx].dwVaddr;
    vVLDSetRp(_dwVldReadPtr);
#else
    vVldSetReadPtr(_ptPicIdx[_dwPicRdIdx].dwVaddr); 
#endif
    if(fgInitBarrelShifter(_ptPicIdx[_dwPicRdIdx].dwVaddr, TRUE) == FALSE)
    {
      return(BAR_SHIFTER_ERR);
    }
    vResetRVBitStream();
    for(i=0; i < (_dwHdrBitCnt >> 3); i++)
    {
      dwGetRVHi(8); 
    }
    dwGetRVHi(_dwHdrBitCnt & 0x7); 
    // ~initial vld to the start of current decoded frame
  }

PARSE_END:  
#endif  // 0
  if(((_dwHSize != _dwPicWidth)||(_dwVSize != _dwPicHeight)))
  {
    _dwHSize = _dwPicWidth;
    _dwVSize =  _dwPicHeight;
  }
  else
  {
    _dwHSize = _dwPicWidth;
    _dwVSize =  _dwPicHeight;
  }

#if 0  // !!!
  if(!fgInquiry)
  {
    vRvSetMpeg2Var();
  }
  else
  {
    if(!fgMpvPicDimChk(_dwHSize, _dwVSize, TRUE))
    {
      return(PIC_DIM_ERR);
    }
  }

  vMpvSetSeqLayerDecPrm();
  _tMpvDecPrm.dBBufStart = 0;
#endif  // 0

  UNUSED(fgECC);

  return;
}

