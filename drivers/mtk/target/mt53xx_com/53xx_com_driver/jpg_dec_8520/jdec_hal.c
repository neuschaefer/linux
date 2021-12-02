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
/*! \file hal_vdec_mpeg_if.h
*
*  \par LEGAL DISCLAIMER
*
* (Header of MediaTek Software/Firmware Release or Documentation)
*
* BY OPENING OR USING THIS FILE, USER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
* ARE PROVIDED TO USER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS
* ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
* NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
* RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED
* IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND USER AGREES TO LOOK ONLY TO
* SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL
* ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO USER'S
* SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* USER HEREBY ACKNOWLEDGES THE CONFIDENTIALITY OF MEDIATEK SOFTWARE AND AGREES
* NOT TO DISCLOSE OR PERMIT DISCLOSURE OF ANY MEDIATEK SOFTWARE TO ANY THIRD
* PARTY OR TO ANY OTHER PERSON, EXCEPT TO DIRECTORS, OFFICERS, EMPLOYEES OF
* USER WHO ARE REQUIRED TO HAVE THE INFORMATION TO CARRY OUT THE PURPOSE OF
* OPENING OR USING THIS FILE.
*
* \par Project
*    MT8520
*
* \par Description
*    Jpeg Decoder HAL interface definition.
*
* \par Author_Name
*    CK Hu
*
* \par Last_Changed
* $Author: dtvbm11 $
* $Modtime: $
* $Revision: #1 $
*
*/

// *********************************************************************
// Memo
// *********************************************************************
/*
*/

#include "x_hal_5381.h"
#include "jdec_hal.h"
#include "jdec_hal_errcode.h"
#include "jdec_hal_if.h"
#include "jdec_com_jfif.h"

#undef Printf
#define Printf while(0){}

#ifdef CC_MT5363
#ifdef VLD0_BASE
#undef VLD0_BASE
#define VLD0_BASE   JPG_VLD_BASE
#endif
#endif

static JDEC_HAL_DERIVED_HUFF_TABLE_T rDerivedDcHuffmanTable[4];
static JDEC_HAL_DERIVED_HUFF_TABLE_T rDerivedAcHuffmanTable[4];
#if 0 // winton
INLINE void vHwJdecWrite32(UINT32 u4HwId,UINT32 addr,UINT32 value)
{
    *(volatile UINT32 *)(IO_BASE_ADDRESS + u4HwJdecVldOffset[u4HwId] + addr ) = value;
}


INLINE UINT32 u4HwJdecRead32(UINT32 u4HwId,UINT32 addr)
{
    return *(volatile UINT32 *)(IO_BASE_ADDRESS + u4HwJdecVldOffset[u4HwId] + addr);
}


INLINE void vHwJdecMcWrite32(UINT32 u4HwId,UINT32 addr,UINT32 value)
{
    *(volatile UINT32 *)(IO_BASE_ADDRESS + u4HwJdecMcOffset[u4HwId] + addr ) = value;
}


INLINE UINT32 u4HwJdecMcRead32(UINT32 u4HwId,UINT32 addr)
{
    return *(volatile UINT32 *)(IO_BASE_ADDRESS + u4HwJdecMcOffset[u4HwId] + addr);
}
#else
	INLINE UINT32 u4HwJdecRead32(UINT32 u4HwId, UINT32 u4Offset)
	{
		UINT32 u4Reg;
		u4Reg = IO_READ32(VLD0_BASE, (u4Offset));
		//Printf("  - 0x%08x = VLD0_READ32(0x%03x)\n", u4Reg, (u4Offset));
//		Printf("r 0x%08x 1\n", (VLD0_BASE + (u4Offset - 0x400)));
		return u4Reg;
	}
	INLINE UINT32 u4HwJdecMcRead32(UINT32 u4HwId, UINT32 u4Offset)
	{
		UINT32 u4Reg;
		u4Reg = IO_READ32(MC0_BASE,	u4Offset);
		//Printf("  - 0x%08x = MC0_READ32(0x%08x)\n", u4Reg, u4Offset);
		return u4Reg;
	}
	INLINE void vHwJdecWrite32(UINT32 u4HwId, UINT32 u4Offset, UINT32 u4Value)
	{
		//Printf("  - VLD0_WRITE32(0x%03x, 0x%08x)\n", (u4Offset), u4Value);
		IO_WRITE32(VLD0_BASE, (u4Offset), u4Value);
//		Printf("w 0x%08x 0x%08x\n", (VLD0_BASE + (u4Offset - 0x400)), u4Value);
	}
	INLINE void vHwJdecMcWrite32(UINT32 u4HwId, UINT32 u4Offset, UINT32 u4Value)
	{
		//Printf("  - MC0_WRITE32(0x%8x, 0x%08x)\n", u4Offset, u4Value);
		IO_WRITE32(MC0_BASE, u4Offset, u4Value);
	}
#endif

void vHwJdecVLDPowerOn(UINT32 u4HwId,BOOL fgPowerOn)
{
    //Printf("vHwJdecVLDPowerOn()\n");
    
    if(fgPowerOn)
        vHwJdecWrite32(u4HwId,RW_VLD_PWRSAVE,0);
    else
        vHwJdecWrite32(u4HwId,RW_VLD_PWRSAVE,3);
}


void vHwJdecHwReset(UINT32 u4HwId)
{
    UINT32 u4TempVal;

    u4TempVal = u4HwJdecRead32(u4HwId,RW_VLD_RISC_WP);  //save previous risc wp
    //Printf("vHwJdecHwReset()\n");
    vHwJdecWrite32(u4HwId,WO_VLD_SW_RESET, VLD_SW_RESET_RESET);
    vHwJdecWrite32(u4HwId,WO_VLD_SW_RESET, VLD_SW_RESET_CLEAR);

//#ifdef CC_MT5387    
#if defined(CC_MT5387) && !defined(CC_MT5363) 
    vHwJdecWrite32(u4HwId, RW_VLD_RISC_WP, u4TempVal | 0x1);  //restore risc wp

    vHwJdecWrite32(u4HwId, RW_VLD_ASMR, 0x01000001);
#else
    vHwJdecWrite32(u4HwId, RW_VLD_RISC_WP, u4TempVal);  //restore risc wp

    vHwJdecWrite32(u4HwId, RW_VLD_ASMR, 0x010E0103);
#endif    
}


void vHwJdecSRAMClear(UINT32 u4HwId)
{
    //Printf("vHwJdecSRAMClear()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_SRAM_CLEAR,0);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_SRAM_CLEAR,0x100);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_SRAM_CLEAR,0);
}


void vHwJdecSetVLDWaitTimeoutValue(UINT32 u4HwId,UINT32 u4TimeoutValue)
{
    //Printf("vHwJdecSetVLDWaitTimeoutValue()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_WAITT,u4TimeoutValue);
}


void vHwJdecSetDecodeModeBaseline(UINT32 u4HwId)
{
    UINT32 u4Reg;

    //Printf("vHwJdecSetDecodeModeBaseline()\n");
    u4Reg = u4HwJdecRead32(u4HwId,RW_VLD_JPG_PRG_MODE);
    u4Reg &= ~DECODE_MODE_PROG;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_MODE,u4Reg);
}


void vHwJdecSetDecodeModeProgressive(UINT32 u4HwId)
{
    UINT32 u4Reg;

    //Printf("vHwJdecSetDecodeModeProgressive()\n");
    u4Reg = u4HwJdecRead32(u4HwId,RW_VLD_JPG_PRG_MODE);
    u4Reg |= DECODE_MODE_PROG;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_MODE,u4Reg);
}


void vHwJdecSetAhAl(UINT32 u4HwId,UINT32 Ah,UINT32 Al)
{
    //Printf("vHwJdecSetAhAl(%u, %u)\n", Ah, Al);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_AH_AL,((Ah & 0xF)<<8) | (Al & 0xF));
}


void vHwJdecSetSsSe(UINT32 u4HwId,UINT32 Ss,UINT32 Se)
{
    //Printf("vHwJdecSetSsSe(%u, %u)\n", Ss, Se);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_SS_SE,((Ss & 0x7F) << 8) | (Se & 0x3F));
}


void vHwJdecSetDcAc(UINT32 u4HwId,UINT32 Ah,UINT32 Ss)
{
    UINT32 u4Reg;

    //Printf("vHwJdecSetDcAc(%u, %u)\n", Ah, Ss);
    u4Reg = u4HwJdecRead32(u4HwId,RW_VLD_JPG_PRG_MODE);
    u4Reg &= ~(JPG_DC_FIRST | JPG_DC_REFINE | JPG_AC_FIRST | JPG_AC_REFINE);
    if (Ah == 0)
    {
        if (Ss == 0)
            u4Reg |= JPG_DC_FIRST;
        else
            u4Reg |= JPG_AC_FIRST;
    }
    else
    {
        if (Ss == 0)
            u4Reg |= JPG_DC_REFINE;
        else
            u4Reg |= JPG_AC_REFINE;
    }
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_MODE,u4Reg);
}


void vHwJdecSetLastScan(UINT32 u4HwId,BOOL fgLastScan)
{
    UINT32 u4Reg;

    //Printf("vHwJdecSetLastScan(%d)\n", (UINT32)fgLastScan);
    u4Reg = u4HwJdecRead32(u4HwId,RW_VLD_JPG_PRG_MODE);
	if(fgLastScan)
        u4Reg |= PROG_MODE_LAST_SCAN;
    else
        u4Reg &= ~PROG_MODE_LAST_SCAN;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_MODE,u4Reg);
}


void vHwJdecSetNoneZeroHistoryBuf(UINT32 u4HwId,UINT32 u4NoneZeroHistoryBuf)
{
    //Printf("vHwJdecSetNoneZeroHistoryBuf(0x%08x)\n", u4NoneZeroHistoryBuf);
    u4NoneZeroHistoryBuf = PHYSICAL(u4NoneZeroHistoryBuf);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_NZ_HIST,u4NoneZeroHistoryBuf);
}


void vHwJdecSetOutputNoneZeroHistoryBufOnly(UINT32 u4HwId,BOOL fgNzBufOnly)
{
    //Printf("vHwJdecSetOutputNoneZeroHistoryBufOnly(%u)\n", fgNzBufOnly);
    if(fgNzBufOnly)
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_NWB,VLD_PROG_BUFF_NZ_ONLY);
    else
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_NWB,VLD_PROG_BUFF_ALL);
}


void vHwJdecSetInputBuffer(UINT32 u4HwId,UINT32 u4InBufSa,UINT32 u4InBufSz)
{
    //Printf("vHwJdecSetInputBuffer(0x%08x, 0x%08x)\n", u4InBufSa, u4InBufSa+u4InBufSz);
    u4InBufSa = PHYSICAL(u4InBufSa);
    vHwJdecWrite32(u4HwId,RW_VLD_VSTART,u4InBufSa>>6);
    vHwJdecWrite32(u4HwId,RW_VLD_VEND,(u4InBufSa+u4InBufSz)>>6);
}


INT32 i4HwJdecWaitVldFetchOk(UINT32 u4HwId)
{
	UINT32 u4TimeAccu = 0;

    //Printf("i4HwJdecWaitVldFetchOk()\n");
	while (! (u4HwJdecRead32(u4HwId,RO_VLD_FETCHOK) & VLD_FETCH_OK))
	{
		x_thread_delay(50);
		u4TimeAccu += 50;
		if (u4TimeAccu >= 300)
		{
			return E_JDEC_HAL_FAIL;
		}
	}
	return S_JDEC_HAL_OK;
}


UINT32 u4HwJdecGetBitS(UINT32 u4HwId,UINT32 u4ShiftBit)
{
  UINT32 u4RegVal;
  UINT32 u4ShiftBitL = (u4ShiftBit & 0x1F);
  UINT32 u4ShiftBitH = ((u4ShiftBit>>5) & 0x3);
  UINT32 u4I;

    //Printf("u4HwJdecGetBitS(%u)\n", u4ShiftBit);
  // Skip groups of 32 bits
  for(u4I=0; u4I<u4ShiftBitH; u4I++)
      u4HwJdecRead32(u4HwId, (UINT32)(RO_VLD_BARL + (32<< 2)));

  u4RegVal = u4HwJdecRead32(u4HwId,RO_VLD_BARL + (u4ShiftBitL<< 2));

  return(u4RegVal);
}


INT32 i4HwJdecSetBarrelShifterAddr(UINT32 u4HwId,UINT32 u4Addr,UINT32 u4Bits)
{
    //Printf("i4HwJdecSetBarrelShifterAddr(0x%08x, %u)\n", u4Addr, u4Bits);
    u4Addr = PHYSICAL(u4Addr);
    vHwJdecWrite32(u4HwId,RW_VLD_RPTR,u4Addr & (~0xF));     // Set VLD read pointer
    vHwJdecWrite32(u4HwId,RW_VLD_PROC,VLD_INIFET); // Init fetch

    {
        INT32 i4Ret;
        i4Ret = i4HwJdecWaitVldFetchOk(u4HwId);
        if(i4Ret<0)
            return i4Ret;
    }

    vHwJdecWrite32(u4HwId,RW_VLD_PROC,VLD_INIBR);  // Init barrel shifter

    u4HwJdecGetBitS(u4HwId,((u4Addr & 0xF) << 3) + u4Bits);

    return S_JDEC_HAL_OK;
}


void vHwJdecSetVLDBarrelShifterToJpegMode(UINT32 u4HwId,BOOL fgJpegMode)
{
    //Printf("vHwJdecSetVLDBarrelShifterToJpegMode(%u)\n", fgJpegMode);
    if(fgJpegMode)
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_SWITCH,1);
    else
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_SWITCH,0);
}


void vHwJdecSetMiscReg(UINT32 u4HwId)
{
    //Printf("vHwJdecSetMiscReg()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_PRG_MISC1,0xFFFFFFFF);
}


INT32 i4HwJdecCalculateOutputBufAddr(UINT32 u4HwId,UINT32 u4CurrMCUNum,
                                                   JDEC_JFIF_SOF_T *prSofInfo,
                                                   JDEC_JFIF_SOS_T *prScanInfo,
                                                   JDEC_HAL_OUTPUT_INFO_T *prOutputInfo,
                                                   UINT32 u4OutCoefYBufSa,
                                                   UINT32 u4OutCoefCbBufSa,
                                                   UINT32 u4OutCoefCrBufSa,
                                                   UINT32 *pu4YBufSa,
                                                   UINT32 *pu4CbBufSa,
                                                   UINT32 *pu4CrBufSa,
                                                   UINT32 *pu4YCoeffBufSa,
                                                   UINT32 *pu4CbCoeffBufSa,
                                                   UINT32 *pu4CrCoeffBufSa, JDEC_HAL_DEC_PROG_T eFlow)
{
    UINT32 u4CompId;
    UINT32 u4MCUx,u4MCUy;
    UINT32 u4MCUWidth;//,u4MCUHeight;
    UINT32 u4CombMCUWidth;//,u4CombMCUHeight;
    UINT32 au4MCUWidth[3];//,au4MCUHeight[3];
    UINT32 u4YBufSa,u4CbBufSa,u4CrBufSa;
    UINT32 u4YCoeffBufSa,u4CbCoeffBufSa,u4CrCoeffBufSa;

    //Printf("i4HwJdecCalculateOutputBufAddr()\n");
    for(u4CompId = 0; u4CompId<3; u4CompId++)
    {
        if (!prSofInfo->arSofComp[u4CompId].u1HSampFactor)
        {
            prSofInfo->arSofComp[u4CompId].u1HSampFactor = 1;
        }
        UINT32 u4WidthPerMCU = (prSofInfo->arSofComp[0].u1HSampFactor /
                                prSofInfo->arSofComp[u4CompId].u1HSampFactor) * 8;
        //UINT32 u4HeightPerMCU = (prSofInfo->arSofComp[0].u1VSampFactor /
        //                         prSofInfo->arSofComp[u4CompId].u1VSampFactor) * 8;

        au4MCUWidth[u4CompId] = (prSofInfo->u2ImageWidth + u4WidthPerMCU - 1) / u4WidthPerMCU;
        //au4MCUHeight[u4CompId] = (prSofInfo->u2ImageHeight + u4HeightPerMCU - 1) / u4HeightPerMCU;
    }

    u4CombMCUWidth = (prSofInfo->u2ImageWidth + prSofInfo->arSofComp[0].u1HSampFactor * 8 - 1) /
                     (prSofInfo->arSofComp[0].u1HSampFactor * 8);
    //u4CombMCUHeight = (prSofInfo->u2ImageHeight + prSofInfo->arSofComp[0].u1VSampFactor * 8 - 1) /
    //                  (prSofInfo->arSofComp[0].u1VSampFactor * 8);

    if(prScanInfo->u1CompInScan == 1)
    {
        UINT32 u4CompId = prScanInfo->au1CompId[0];
        u4CompId = (u4CompId > (UINT32)2) ? (UINT32)2 : u4CompId;
        u4MCUWidth = au4MCUWidth[u4CompId];
        //u4MCUHeight = au4MCUHeight[u4CompId];
    } else
    {
        u4MCUWidth = u4CombMCUWidth;
        //u4MCUHeight = u4CombMCUHeight;
    }

    u4MCUx = u4CurrMCUNum % u4MCUWidth;
    u4MCUy = u4CurrMCUNum / u4MCUWidth;

    // Calculate IDCT buffer address
    u4YBufSa = prOutputInfo->u4OutYBufSa;
    u4CbBufSa = prOutputInfo->u4OutCbBufSa;
    u4CrBufSa = prOutputInfo->u4OutCrBufSa;

    if(prScanInfo->u1CompInScan == 1)
    {
        if (eFlow == JDEC_HAL_DEC_PROG_VERIFY)//bc adds for FPGA full pic output buffer
        {
        u4YBufSa += u4MCUy*prOutputInfo->u4OutYBufPitch*8;
        u4CbBufSa += u4MCUy*prOutputInfo->u4OutCbBufPitch*8;
        u4CrBufSa += u4MCUy*prOutputInfo->u4OutCrBufPitch*8;
        }
        u4YBufSa += u4MCUx*8;
        u4CbBufSa += u4MCUx*8;
        u4CrBufSa += u4MCUx*8;
    } else
    {
        if (eFlow == JDEC_HAL_DEC_PROG_VERIFY)//bc adds for FPGA full pic output buffer
        {
        u4YBufSa += u4MCUy*prOutputInfo->u4OutYBufPitch*(prSofInfo->arSofComp[0].u1VSampFactor * 8);
        u4CbBufSa += u4MCUy*prOutputInfo->u4OutCbBufPitch*(prSofInfo->arSofComp[1].u1VSampFactor * 8);
        u4CrBufSa += u4MCUy*prOutputInfo->u4OutCrBufPitch*(prSofInfo->arSofComp[2].u1VSampFactor * 8);
        }
        u4YBufSa += u4MCUx*(prSofInfo->arSofComp[0].u1HSampFactor * 8);
        u4CbBufSa += u4MCUx*(prSofInfo->arSofComp[1].u1HSampFactor * 8);
        u4CrBufSa += u4MCUx*(prSofInfo->arSofComp[2].u1HSampFactor * 8);
    }

    // Calculate Coefficient buffer address
    u4YCoeffBufSa = u4OutCoefYBufSa;
    u4CbCoeffBufSa = u4OutCoefCbBufSa;
    u4CrCoeffBufSa = u4OutCoefCrBufSa;

    if (eFlow == JDEC_HAL_DEC_ENHANCE)              //reduce coef buffer using
    {
        u4MCUx = 0;
        u4MCUy = 0;
    }

    if(prScanInfo->u1CompInScan == 1)
    {
        u4YCoeffBufSa += u4MCUy * u4CombMCUWidth
                                * (prSofInfo->arSofComp[0].u1HSampFactor * 8)
                                * 8
                                * 2;

        u4CbCoeffBufSa += u4MCUy * u4CombMCUWidth
                                 * (prSofInfo->arSofComp[1].u1HSampFactor * 8)
                                 * 8
                                 * 2;

        u4CrCoeffBufSa += u4MCUy * u4CombMCUWidth
                                 * (prSofInfo->arSofComp[2].u1HSampFactor * 8)
                                 * 8
                                 * 2;

        u4YCoeffBufSa += u4MCUx * 8
                                * 8
                                * 2;

        u4CbCoeffBufSa += u4MCUx * 8
                                 * 8
                                 * 2;

        u4CrCoeffBufSa += u4MCUx * 8
                                 * 8
                                 * 2;
    } else
    {
        u4YCoeffBufSa += u4MCUy * u4MCUWidth
                                * (prSofInfo->arSofComp[0].u1HSampFactor * 8)
                                * (prSofInfo->arSofComp[0].u1VSampFactor * 8)
                                * 2;

        u4CbCoeffBufSa += u4MCUy * u4MCUWidth
                                 * (prSofInfo->arSofComp[1].u1HSampFactor * 8)
                                 * (prSofInfo->arSofComp[1].u1VSampFactor * 8)
                                 * 2;

        u4CrCoeffBufSa += u4MCUy * u4MCUWidth
                                 * (prSofInfo->arSofComp[2].u1HSampFactor * 8)
                                 * (prSofInfo->arSofComp[2].u1VSampFactor * 8)
                                 * 2;

        u4YCoeffBufSa += u4MCUx * (prSofInfo->arSofComp[0].u1HSampFactor * 8)
                                * 8
                                * 2;

        u4CbCoeffBufSa += u4MCUx * (prSofInfo->arSofComp[1].u1HSampFactor * 8)
                                 * 8
                                 * 2;

        u4CrCoeffBufSa += u4MCUx * (prSofInfo->arSofComp[2].u1HSampFactor * 8)
                                 * 8
                                 * 2;
    }

    if(pu4YBufSa != NULL)
        *pu4YBufSa = u4YBufSa;
    if(pu4CbBufSa != NULL)
        *pu4CbBufSa = u4CbBufSa;
    if(pu4CrBufSa != NULL)
        *pu4CrBufSa = u4CrBufSa;
    if(pu4YCoeffBufSa != NULL)
        *pu4YCoeffBufSa = u4YCoeffBufSa;
    if(pu4CbCoeffBufSa != NULL)
        *pu4CbCoeffBufSa = u4CbCoeffBufSa;
    if(pu4CrCoeffBufSa != NULL)
        *pu4CrCoeffBufSa = u4CrCoeffBufSa;

    return S_JDEC_HAL_OK;
}


void vHwJdecSetOutputBufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr,
                                           JDEC_JFIF_SOS_T *prScanInfo)
{
    //Printf("vHwJdecSetOutputBufAddr()\n");
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    if(prScanInfo->u1CompInScan == 1)
    {
        switch(prScanInfo->au1CompId[0])
        {
            case 0:
                vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_ADD0,u4Comp0Addr>>2);
                break;
            case 1:
                vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_ADD0,u4Comp1Addr>>2);
                break;
            case 2:
                vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_ADD0,u4Comp2Addr>>2);
                break;
        }
    } else
    {
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_ADD0,u4Comp0Addr>>2);
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_ADD1,u4Comp1Addr>>2);
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_ADD2,u4Comp2Addr>>2);
    }
}


void vHwJdecSetOutputCoefBufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr,
                                               JDEC_JFIF_SOS_T *prScanInfo)
{
    //Printf("vHwJdecSetOutputCoefBufAddr()\n");
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    if(prScanInfo->u1CompInScan == 1)
    {
        switch(prScanInfo->au1CompId[0])
        {
            case 0:
                vHwJdecWrite32(u4HwId,RW_VLD_JPG_RD_ADD0,u4Comp0Addr>>2);
                break;
            case 1:
                vHwJdecWrite32(u4HwId,RW_VLD_JPG_RD_ADD0,u4Comp1Addr>>2);
                break;
            case 2:
                vHwJdecWrite32(u4HwId,RW_VLD_JPG_RD_ADD0,u4Comp2Addr>>2);
                break;
        }
    } else
    {
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_RD_ADD0,u4Comp0Addr>>2);
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_RD_ADD1,u4Comp1Addr>>2);
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_RD_ADD2,u4Comp2Addr>>2);
    }
}


void vHwJdecSetReadWriteOutputBufMode(UINT32 u4HwId)
{
    //Printf("vHwJdecSetReadWriteOutputBufMode()\n");
    vHwJdecMcWrite32(u4HwId,RW_MC_ADDRSWAP,0);
    vHwJdecMcWrite32(u4HwId,RW_MC_PS_DRAM_MODE,3);
}


void vHwJdecInitScan(UINT32 u4HwId)
{
    //Printf("vHwJdecInitScan()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,0);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,VLD_RES_MKR_FND);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,0);
}


void vHwJdecRestart(UINT32 u4HwId)
{
    //Printf("vHwJdecRestart()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,VLD_MCU_RESTART);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,0);
}


void vHwJdecCleanInterBuf(UINT32 u4HwId,BOOL fgClean)
{
    //Printf("vHwJdecCleanInterBuf(%u)\n", fgClean);
    if(fgClean)
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_FAST_DEC,VLD_CLEAN_INTERNAL_BUFFER);
    else
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_FAST_DEC,VLD_KEEP_INTERNAL_BUFFER);
}


void vHwJdecSetCurrMCUNum(UINT32 u4HwId,UINT32 u4CurrMCUNum)
{
    //Printf("vHwJdecSetCurrMCUNum(%u)\n", u4CurrMCUNum);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_MCU_COUNT,u4CurrMCUNum);
}


void vHwJdecDecodeMCU(UINT32 u4HwId)
{
    //Printf("vHwJdecDecodeMCU()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_START,VLD_DECODE_MCU);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_START,VLD_DECODE_MCU_CLEAR);
}


BOOL fgHwJdecIsDecodeFinish(UINT32 u4HwId)
{
    //Printf("fgHwJdecIsDecodeFinish()\n");
    if(u4HwJdecRead32(u4HwId,RW_VLD_JPG_RMK_AND_FINISH) & VLD_MCU_DECODE_FINISH)
        return TRUE;

    return FALSE;
}


BOOL fgHwJdecIsFoundMarker(UINT32 u4HwId)
{
    //Printf("fgHwJdecIsFoundMarker()\n");
    if(u4HwJdecRead32(u4HwId,RW_VLD_JPG_RMK_AND_FINISH) & VLD_RECEIVE_MARKER)
        return TRUE;

    return FALSE;
}


UINT32 u4HwJdecGetFoundMarker(UINT32 u4HwId)
{
    //Printf("u4HwJdecGetFoundMarker()\n");
    return (u4HwJdecRead32(u4HwId,RW_VLD_JPG_RMK_AND_FINISH) >> 8) & 0xFF;
}


INT32 i4HwJdecSetBlockPostition(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo,JDEC_JFIF_SOS_T *prScanInfo)
{
    UINT32 u4Block;
    UINT32 u4CompNum;
    UINT32 u4X,u4Y;
	UINT32 u4BlockXPosList = 0, u4BlockYPosList = 0;

    //Printf("i4HwJdecSetBlockPostition()\n");
    if(prPicInfo == NULL)
        return E_JDEC_HAL_FAIL;

    u4Block = 0;
    if(prScanInfo->u1CompInScan == 1)
    {
    } else
    {
        if (prPicInfo->u1NumComponents > (UINT8)MAX_JPEG_COMPONENT)
        {
            return E_JDEC_HAL_FAIL;
        }
        for(u4CompNum=0; u4CompNum<prPicInfo->u1NumComponents; u4CompNum++)
            for(u4Y=0; u4Y<prPicInfo->arSofComp[u4CompNum].u1VSampFactor; u4Y++)
                for(u4X=0; u4X<prPicInfo->arSofComp[u4CompNum].u1HSampFactor; u4X++)
                {
                    u4BlockXPosList |= (u4X & 0x7) << (u4Block * 3);
                    u4BlockYPosList |= (u4Y & 0x7) << (u4Block * 3);
                    u4Block++;
                }
    }

    vHwJdecWrite32(u4HwId,RW_VLD_JPG_X_IN_MCU,u4BlockXPosList);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_Y_IN_MCU,u4BlockYPosList);

    return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetBlkAndDcAcTblList(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo)
{
    UINT32 u4Block;
    UINT32 u4Count;
    UINT32 u4CompNum;
    UINT32 u4CompId;
    UINT32 au4CompBlkNum;
    UINT32 u4DcHuffTblList = 0,u4AcHuffTblList = 0;

    //Printf("i4HwJdecSetBlkAndDcAcTblList()\n");
    if((prSofComp == NULL) || (prScanInfo == NULL))
		return E_JDEC_HAL_FAIL;

    u4Block = 0;
    for(u4CompNum = 0; (u4CompNum<prScanInfo->u1CompInScan) && (u4CompNum<3); u4CompNum++)
    {
        u4CompId = prScanInfo->au1CompId[u4CompNum];

        if(prScanInfo->u1CompInScan == 1)
            au4CompBlkNum = 1;
        else
            au4CompBlkNum = prSofComp[u4CompId].u1HSampFactor * prSofComp[u4CompId].u1VSampFactor;

        if (u4CompId > (UINT32)3)
        {
            return E_JDEC_HAL_FAIL;
        }
        for (u4Count = 0; u4Count < au4CompBlkNum; u4Count++)
        {
            u4DcHuffTblList |= (prScanInfo->au1DcId[u4CompId]) << (u4Block * 2);
            u4AcHuffTblList |= (prScanInfo->au1AcId[u4CompId]) << (u4Block * 2);
	        u4Block++;
        }
	}

    vHwJdecWrite32(u4HwId,RW_VLD_JPG_BLK_DC_TBL,u4DcHuffTblList);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_BLK_AC_TBL,u4AcHuffTblList);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetBlkAndCompMembership(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo)
{
    UINT32 u4Block;
    UINT32 u4Count;
    UINT32 u4CompNum;
    UINT32 u4CompId;
    UINT32 au4CompBlkNum;
    UINT32 u4MembershipList = 0;

    //Printf("i4HwJdecSetBlkAndCompMembership()\n");
    if((prSofComp == NULL) || (prScanInfo == NULL))
		return E_JDEC_HAL_FAIL;

    u4Block = 0;
    for(u4CompNum = 0; (u4CompNum<prScanInfo->u1CompInScan) && (u4CompNum<3); u4CompNum++)
    {
        if(prScanInfo->u1CompInScan == 1)
            au4CompBlkNum = 1;
        else
        {
            u4CompId = prScanInfo->au1CompId[u4CompNum];
            au4CompBlkNum = prSofComp[u4CompId].u1HSampFactor * prSofComp[u4CompId].u1VSampFactor;
        }

	    for (u4Count = 0; u4Count < au4CompBlkNum; u4Count++)
	    {
            u4MembershipList |= u4CompNum << (u4Block * 2);
	        u4Block++;
	    }
	}

    vHwJdecWrite32(u4HwId,RW_VLD_JPG_MEMBERSHIP,u4MembershipList);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetQuantTable(UINT32 u4HwId,JDEC_JFIF_DQT_T *prQTblInfo)
{
    INT32 i4Ret;
    UINT32 u4Dummy;

    //Printf("i4HwJdecSetQuantTable()\n");
    if(prQTblInfo == NULL)
		return E_JDEC_HAL_FAIL;

    if(prQTblInfo->u1NumQ > 2)
		return E_JDEC_HAL_FAIL;

    // Set V FIFO start/end address to full range.
    vHwJdecWrite32(u4HwId,RW_VLD_VSTART,0);
    vHwJdecWrite32(u4HwId,RW_VLD_VEND,~0);

    i4Ret = i4HwJdecSetBarrelShifterAddr(u4HwId,(UINT32)(&(prQTblInfo->aau1Qtbl[0][0])),0);
    if(i4Ret<0)
        return i4Ret;

    u4Dummy = u4HwJdecRead32(u4HwId,RW_VLD_TABLIM); // Load intra Quantization Matrix

    i4Ret = i4HwJdecSetBarrelShifterAddr(u4HwId,(UINT32)(&(prQTblInfo->aau1Qtbl[1][0])),0);
    if(i4Ret<0)
        return i4Ret;

    u4Dummy = u4HwJdecRead32(u4HwId,RW_VLD_TABLNIM); // Load non-intra Quantization Matrix

    (void)u4Dummy;

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetCurrUsedQuantTable(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo,JDEC_JFIF_SOS_T *prScanInfo)
{
    UINT32 u4QtblUsed = 0;
    UINT32 u4CompNum;

    //Printf("i4HwJdecSetCurrUsedQuantTable()\n");
    if((prPicInfo == NULL) || (prScanInfo == NULL))
		return E_JDEC_HAL_FAIL;

    if (prScanInfo->u1CompInScan > (UINT8)MAX_JPEG_COMPONENT)
    {
        return E_JDEC_HAL_FAIL;
    }
    for(u4CompNum=0; u4CompNum<prScanInfo->u1CompInScan; u4CompNum++)
        u4QtblUsed |= (prPicInfo->arSofComp[prScanInfo->au1CompId[u4CompNum]].u1QuantTblNo) <<
                      (u4CompNum<<1);

    vHwJdecWrite32(u4HwId,RW_VLD_JPG_Q_TBL,u4QtblUsed);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetPitchPerRow(UINT32 u4HwId,UINT32 u4Comp0PitchPerRow,UINT32 u4Comp1PitchPerRow,UINT32 u4Comp2PitchPerRow,BOOL fgCoeffPitch, BOOL fgLastScan)
{
    UINT32 u4Value;
    
    //Printf("i4HwJdecSetPitchPerRow()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_IDCT_WIDTH01,
                          ((u4Comp0PitchPerRow>>2) & 0x3FFF)
                          | (((u4Comp1PitchPerRow>>2) & 0x3FFF) << 14));

    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0);
    u4Value = (u4Value & (~0x3FFFF)) | ((u4Comp2PitchPerRow>>2) & 0x3FFF);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0,u4Value);

    if(fgCoeffPitch)
    {
        if (!fgLastScan)
        {
            u4Comp0PitchPerRow = (UINT32)(u4Comp0PitchPerRow * 2);
            u4Comp1PitchPerRow = (UINT32)(u4Comp1PitchPerRow * 2);
            u4Comp2PitchPerRow = (UINT32)(u4Comp2PitchPerRow * 2);
        }
        
        u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0);
        u4Value = (u4Value & (~(0x3FFFFUL<<14))) | (((u4Comp0PitchPerRow>>2) & 0x3FFFUL)<<14);
        vHwJdecWrite32(u4HwId,RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0,u4Value);


        vHwJdecWrite32(u4HwId,RW_VLD_JPG_COEFF_WIDTH12,
                              ((u4Comp1PitchPerRow>>2) & 0x3FFF)
                              | (((u4Comp2PitchPerRow>>2) & 0x3FFF) <<14));
    }

	return S_JDEC_HAL_OK;
}


//////////////////////////////////////////////////////////////
//
// Progressive Related Function
//
//////////////////////////////////////////////////////////////
INT32 i4HwJdecSetCoefPitchPerRow(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo)
{
    UINT32 u4CompId;
    UINT32 au4CompPitchPerRow[3] = { 0, 0, 0};
    UINT32 u4Value;
    UINT32 u4MaxHSampleFactor = 0;
    UINT32 u4McuPerRow = 0;

    if(prPicInfo == NULL)
		return E_JDEC_HAL_FAIL;

    for(u4CompId=0; u4CompId<3; u4CompId++)
    {
        if (prPicInfo->arSofComp[u4CompId].u1HSampFactor >= u4MaxHSampleFactor)
        {
            u4MaxHSampleFactor = prPicInfo->arSofComp[u4CompId].u1HSampFactor;
        }
    }
    if (!u4MaxHSampleFactor)
    {
        u4MaxHSampleFactor = (UINT32)1;
    }
    u4McuPerRow = (UINT32)((prPicInfo->u2ImageWidth + u4MaxHSampleFactor * 8 - 1)/(u4MaxHSampleFactor * 8));
    for(u4CompId=0; u4CompId<3; u4CompId++)
    {
        au4CompPitchPerRow[u4CompId] = u4McuPerRow * prPicInfo->arSofComp[u4CompId].u1HSampFactor * 8 * 2;
    }

    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0);
    u4Value = (u4Value & (~(0x3FFFFUL<<14))) | (((au4CompPitchPerRow[0]>>2) & 0x3FFFUL)<<14);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0,u4Value);


    vHwJdecWrite32(u4HwId,RW_VLD_JPG_COEFF_WIDTH12,
                          ((au4CompPitchPerRow[1]>>2) & 0x3FFF)
                          | (((au4CompPitchPerRow[2]>>2) & 0x3FFF) <<14));

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetDcAcNeededList(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo)
{
    UINT32 u4Block;
    UINT32 u4Count;
    UINT32 u4CompNum;
    UINT32 u4CompId;
    UINT32 au4CompBlkNum;
    UINT32 u4DcNeededList = 0;
    UINT32 u4AcNeededList = 0;
    UINT32 u4Value;

   // Printf("i4HwJdecSetDcAcNeededList()\n");
    if((prSofComp == NULL) || (prScanInfo == NULL))
		return E_JDEC_HAL_FAIL;

    u4Block = 0;
    for(u4CompNum = 0; (u4CompNum<prScanInfo->u1CompInScan) && (u4CompNum<3); u4CompNum++)
    {
        if(prScanInfo->u1CompInScan == 1)
            au4CompBlkNum = 1;
        else
        {
            u4CompId = prScanInfo->au1CompId[u4CompNum];
            if (u4CompId >= MAX_JPEG_COMPONENT)
            {
                return E_JDEC_HAL_FAIL;
            }
            au4CompBlkNum = prSofComp[u4CompId].u1HSampFactor * prSofComp[u4CompId].u1VSampFactor;
        }

	    for (u4Count = 0; u4Count < au4CompBlkNum; u4Count++)
	    {
	        u4DcNeededList |= (1<<u4Block);
	        u4AcNeededList |= (1<<u4Block);
	        u4Block++;
	    }
	}

    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_DC_AC_NEEDED);
    u4Value = (u4Value & (~0x3FF)) | (u4DcNeededList & 0x3FF);
    u4Value = (u4Value & (~(0x3FF<<10))) | ((u4AcNeededList & 0x3FF)<<10);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_DC_AC_NEEDED,u4Value);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetBlockCount(UINT32 u4HwId,JDEC_SOF_COMP_T *prSofComp,JDEC_JFIF_SOS_T *prScanInfo)
{
    UINT32 u4CompNum;
    UINT32 u4CompId;
    UINT32 u4BlockCount = 0;
    UINT32 u4Value;

    //Printf("i4HwJdecSetBlockCount()\n");
    if((prSofComp == NULL) || (prScanInfo == NULL))
		return E_JDEC_HAL_FAIL;

    for(u4CompNum = 0; (u4CompNum<prScanInfo->u1CompInScan) && (u4CompNum<3); u4CompNum++)
    {
        if(prScanInfo->u1CompInScan == 1)
            u4BlockCount = 1;
        else
        {
            u4CompId = prScanInfo->au1CompId[u4CompNum];
            if (u4CompId >= MAX_JPEG_COMPONENT)
            {
                return E_JDEC_HAL_FAIL;
            }
            u4BlockCount += prSofComp[u4CompId].u1HSampFactor * prSofComp[u4CompId].u1VSampFactor;
        }
    }

    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_DC_AC_NEEDED);
    u4Value = (u4Value & (~(0xF<<24))) | ((u4BlockCount & 0xF)<<24);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_DC_AC_NEEDED,u4Value);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetEOBRUN(UINT32 u4HwId,UINT32 u4EOBRUN)
{
    //Printf("i4HwJdecSetEOBRUN(%u)\n", u4EOBRUN);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_WR_EOB_RUN,u4EOBRUN);

	return S_JDEC_HAL_OK;
}


UINT32 u4HwJdecGetEOBRUN(UINT32 u4HwId)
{
    //Printf("u4HwJdecGetEOBRUN()\n");
    return u4HwJdecRead32(u4HwId,RO_VLD_JPG_RD_EOB_RUN);
}


INT32 i4HwJdecProcRestart(UINT32 u4HwId)
{
    UINT32 u4Value;

    //Printf("i4HwJdecProcRestart()\n");
    vHwJdecRestart(u4HwId);
    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_RMK_AND_FINISH);
    if(!(u4Value & VLD_RECEIVE_MARKER)) // Not found any marker
    {
        // TODO:
    } else if(((u4Value>>8) & 0xFF) != 0xD7) // Found marker, but not RST marker.
    {
        // TODO:
    }

	return S_JDEC_HAL_OK;
}


//////////////////////////////////////////////////////////////
//
// Huffman Table Related Function
//
//////////////////////////////////////////////////////////////
void vHwJdecResetDcPrediction(UINT32 u4HwId)
{
    //Printf("vHwJdecResetDcPrediction()\n");
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,0);
	vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,VLD_RES_DC_PRED);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART,0);
}


INT32 iHwJdecCalcDerivedHuffTable(UINT8 *au1Bits,UINT8 *au1HuffVal,JDEC_HAL_DERIVED_HUFF_TABLE_T *prDerivedHuffTbl)
{
    //Printf("iHwJdecCalcDerivedHuffTable(bits(0x%08x),val(0x%08x),dhufftbl(0x%08x))\n",au1Bits,au1HuffVal,prDerivedHuffTbl);

	//int numsymbols;
	INT32	i4BitLen, i4Symbol, i4LookBits, i4Ctr, i4Size, i4Loop1, i4Loop2;
	INT16	i2Bits;

	INT8		ai1HuffSize[257];
	UINT16	au2HuffCode[257];
	UINT16	u2Code;

    if((au1Bits == NULL) || (au1HuffVal == NULL) || (prDerivedHuffTbl == NULL))
        return E_JDEC_HAL_FAIL;

	/* Note that huffsize[] and huffcode[] are filled in code-length order,
	 * paralleling the order of the symbols themselves in htbl->huffval[].
	 */

	//x_memset(ai1HuffSize, 0, sizeof(ai1HuffSize));
	//x_memset(au2HuffCode, 0, sizeof(au2HuffCode));

	for (i4Loop1 = 0; i4Loop1 < 257; i4Loop1++)
	{
		ai1HuffSize[i4Loop1]=0;
		au2HuffCode[i4Loop1]=0;
	}
	
	/* Figure C.1: make table of Huffman code length for each symbol */

	i4Symbol = 0;
	for (i4BitLen = 1; i4BitLen <= 16; i4BitLen++)
	{
		i2Bits = (INT16)au1Bits[i4BitLen];

		if ((i4Symbol + i2Bits) > 256) /* protect against table overrun */
		{
			// Error handler:
			// set to nearest value
			i2Bits = 255 - i4Symbol;	/* p + i = 255 */
		}

		while (i2Bits--)  // safe while
		{
			ai1HuffSize[i4Symbol++] = (INT8) i4BitLen;
		}
	}

	ai1HuffSize[i4Symbol] = 0;
	//numsymbols = p;

	/* Figure C.2: generate the codes themselves */
	/* We also validate that the counts represent a legal Huffman code tree. */

	i4Symbol	= 0;
	u2Code		= 0;
	i4Size		= ai1HuffSize[0];

#define BIG_LOOP 5000

	for (i4Loop1 = 0; i4Loop1 < BIG_LOOP; i4Loop1++)
	{
	    if (i4Symbol >= 256)
	    {
	        i4Symbol = 256;
	    }
		if (ai1HuffSize[i4Symbol])
		{
			for (i4Loop2 = 0; i4Loop2 < BIG_LOOP; i4Loop2++)
			{
				if (((INT32) ai1HuffSize[i4Symbol]) == i4Size)
				{
					au2HuffCode[i4Symbol++] = u2Code;
					u2Code++;
				}
				else
				{
					break;
				}
			}
			/* code is now 1 more than the last code used for codelength si; but
			* it must still fit in si bits, since no code is allowed to be all ones.
			*/

			if (((INT32) u2Code) >= (((INT32) 1) << i4Size))
			{
				// Error handler:
				// set to 0
				u2Code = 0;
			}

			u2Code <<= 1;
			i4Size++;
		}// huffsize[9]
		else
		{
			break;
		}
	}// BIG_LOOP

	/* Figure F.15: generate decoding tables for bit-sequential decoding */

	i4Symbol = 0;
	for (i4BitLen = 1; i4BitLen <= 16; i4BitLen++)
	{
		if (au1Bits[i4BitLen])
		{
			/* valoffset[l] = huffval[] index of 1st symbol of code length l,
			 * minus the minimum code of length l
			 */
			prDerivedHuffTbl->ai4Valoffset[i4BitLen] =
				(INT32)i4Symbol - (INT32)au2HuffCode[i4Symbol];
			i4Symbol += au1Bits[i4BitLen];
			if (!i4Symbol)
			{
			    i4Symbol = 1;
			}
			prDerivedHuffTbl->ai4Maxcode[i4BitLen] =
				(INT32)au2HuffCode[i4Symbol - 1];		/* maximum code of length l */
		}
		else
		{
			prDerivedHuffTbl->ai4Valoffset[i4BitLen] = 0;
				/* clear to zero if no codes of this length */
			prDerivedHuffTbl->ai4Maxcode[i4BitLen] = -1;
				/* -1 if no codes of this length */
		}
	}
	prDerivedHuffTbl->ai4Maxcode[17] = (INT32)0xFFFFF;
		/* ensures jpeg_huff_decode terminates */

	/* Compute lookahead tables to speed up decoding.
	 * First we set all the table entries to 0, indicating "too long";
	 * then we iterate through the Huffman codes that are short enough and
	 * fill in all the entries that correspond to bit sequences starting
	 * with that code.
	 */

	for (i4Loop1 = 0; i4Loop1 < (1 << D_HUFF_LOOKAHEAD_BITS); i4Loop1++)
	{
		prDerivedHuffTbl->ai4LookNbits[i4Loop1] = 0;
	}

	i4Symbol = 0;
	for (i4BitLen = 1; i4BitLen <= D_HUFF_LOOKAHEAD_BITS; i4BitLen++)
	{
		for (i2Bits = 1; i2Bits <= (INT16) au1Bits[i4BitLen]; i2Bits++, i4Symbol++)
		{
			/* i4BitLen = current code's length, i4Symbol = its index in huffcode[] & huffval[]. */
			/* Generate left-justified code followed by all possible bit sequences */
			i4LookBits = (INT32)au2HuffCode[i4Symbol] << (D_HUFF_LOOKAHEAD_BITS - i4BitLen);
			for (i4Ctr = 1 << (D_HUFF_LOOKAHEAD_BITS - i4BitLen); i4Ctr > 0; i4Ctr--)
			{
				prDerivedHuffTbl->ai4LookNbits[i4LookBits] = i4BitLen;
				prDerivedHuffTbl->au1LookSym[i4LookBits] = au1HuffVal[i4Symbol];
				i4LookBits++;
			}
		}
	}

    return S_JDEC_HAL_OK;
}


// load ac look ahead table
INT32 iHwJdecLoadAcLookAheadTable(UINT32 u4HwId,UINT8 u1HuffTblNo,JDEC_HAL_DERIVED_HUFF_TABLE_T *prDerivedHuffTbl)
{
    //Printf("iHwJdecLoadAcLookAheadTable(%u, 0x%08x)\n", u1HuffTblNo, prDerivedHuffTbl);
	INT32 i4LookAhead;

    if(prDerivedHuffTbl == NULL)
        return E_JDEC_HAL_FAIL;

    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LAHUFF_IDX,(UINT32)u1HuffTblNo);

	for (i4LookAhead = 0; i4LookAhead < 64; i4LookAhead++)
	{
		vHwJdecWrite32(u4HwId,RW_VLD_JPG_LAHUFF_DATA,
			((prDerivedHuffTbl->ai4LookNbits[i4LookAhead] << 8) |
			(UINT32)prDerivedHuffTbl->au1LookSym[i4LookAhead]));
	}

	vHwJdecResetDcPrediction(u4HwId);

	return S_JDEC_HAL_OK;
}


// maxcode and valoffset
INT32 i4HwJdecSetBitsTable(UINT32 u4HwId,INT32 *ai4Value)
{
    //Printf("i4HwJdecSetBitsTable()\n");
	INT32 i4Index;

	if(ai4Value == NULL)
        return E_JDEC_HAL_FAIL;

	for (i4Index = 1; i4Index <= 16; i4Index++)
        vHwJdecWrite32(u4HwId,WO_VLD_JPG_BITS_TBL,(UINT32)(ai4Value[i4Index]));

	return S_JDEC_HAL_OK;
}


// Huffman Value address
INT32 i4HwJdecSetHuffAddr(UINT32 u4HwId,UINT32 u4DcHuff, UINT32 u4AcHuff)
{
    //Printf("i4HwJdecSetHuffAddr(dc(0x%08x),ac(0x%08x))\n", u4DcHuff, u4AcHuff);
    if((u4DcHuff == NULL) || (u4AcHuff == NULL))
        return E_JDEC_HAL_FAIL;

	if ((u4DcHuff & 15) || (u4AcHuff & 15))
		return E_JDEC_HAL_FAIL;

    u4DcHuff = PHYSICAL(u4DcHuff);
    u4AcHuff = PHYSICAL(u4AcHuff);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_HUFF_TBL,u4DcHuff);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_HUFF_TBL,u4AcHuff | VLD_AC_HUFF_FLAG);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecTransHuffTblToHwFormat(UINT8 *pu1DstAddr, UINT8 *au1Huffval)
{
	UINT8 *pu1Huff;
	INT32 i4Index;

    //Printf("i4HwJdecTransHuffTblToHwFormat(Dst:0x%08x, Huff:0x%08x)\n", pu1DstAddr, au1Huffval);
    if((pu1DstAddr == NULL) || (au1Huffval == NULL))
        return E_JDEC_HAL_FAIL;

	pu1Huff = pu1DstAddr;

	for (i4Index = 0; i4Index < 256; i4Index += 4, pu1Huff += 4)
	{
		// every 128 bits, write only 32 bits data, rest 96 bits fill zero
		//x_memcpy(pu1Huff, &au1Huffval[i4Index], 4);
		pu1Huff[0] = au1Huffval[i4Index + 0];
		pu1Huff[1] = au1Huffval[i4Index + 1];
		pu1Huff[2] = au1Huffval[i4Index + 2];
		pu1Huff[3] = au1Huffval[i4Index + 3];
		//x_memset(pu1Huff + 4, 0, 12);
	}

	return S_JDEC_HAL_OK;
}


#ifdef CC_MT5363
INT32 i4HwJdecSetHuffTblVal(UINT32 u4HwId, UINT32 u4TblNum, UINT8 *au1Huffval, BOOL fgDCTbl)
{
    UINT32 u4Val, u4I;

    u4Val = u4TblNum << JPG_HUFF_TABLE_IDENTIFIER;
    if (!fgDCTbl)
    {
        u4Val |= (1 << JPG_HUFF_DC_AC_SELECTION);
    }
    
    vHwJdecWrite32(u4HwId, RW_VLD_JPG_JFILLH, u4Val);

    for (u4I = 0; u4I < 256; u4I += 4)
    {
        u4Val = ((au1Huffval[u4I+3]<<24)|(au1Huffval[u4I+2]<<16)|(au1Huffval[u4I+1]<<8)|au1Huffval[u4I]);
        vHwJdecWrite32(u4HwId, RW_VLD_JPG_JHVAL, u4Val);
    }
    
    return S_JDEC_HAL_OK;
}
#endif


INT32 i4HwJdecSetHuffmanTable(UINT32 u4HwId,JDEC_JFIF_DHT_T *prHuffmanTable,
                              UINT8 *pu1HwDcHuffTbl,UINT8 *pu1HwAcHuffTbl)
{
    UINT32 u4DcHuffTblNum = prHuffmanTable->u4NumDcTbl;
    UINT32 u4AcHuffTblNum = prHuffmanTable->u4NumAcTbl;
    UINT32 u4TblNum;
    //JDEC_HAL_DERIVED_HUFF_TABLE_T rDerivedDcHuffmanTable[4];
    //JDEC_HAL_DERIVED_HUFF_TABLE_T rDerivedAcHuffmanTable[4];
    INT32 i4Ret;

    //Printf("i4HwJdecSetHuffmanTable()\n");
    if((pu1HwDcHuffTbl == NULL) || (pu1HwAcHuffTbl == NULL))
		return E_JDEC_HAL_FAIL;

    // Derive DC Huffman Table
    for(u4TblNum=0; u4TblNum<u4DcHuffTblNum; u4TblNum++)
    {
       // Printf("Derive DC Huff Tbl %d\n", u4TblNum);
        iHwJdecCalcDerivedHuffTable(
            prHuffmanTable->arDcTbl[u4TblNum].au1Bits,
            prHuffmanTable->arDcTbl[u4TblNum].au1HuffVal,
            &(rDerivedDcHuffmanTable[u4TblNum]));
    }

    // Derive AC Huffman Table
    for(u4TblNum=0; u4TblNum<u4AcHuffTblNum; u4TblNum++)
    {
        //Printf("Derive AC Huff Tbl %d\n", u4TblNum);
        iHwJdecCalcDerivedHuffTable(
            prHuffmanTable->arAcTbl[u4TblNum].au1Bits,
            prHuffmanTable->arAcTbl[u4TblNum].au1HuffVal,
            &(rDerivedAcHuffmanTable[u4TblNum]));
    }

    // Set AC Look Ahead Table to HW.
    for(u4TblNum=0; u4TblNum<u4AcHuffTblNum; u4TblNum++)
    {
        iHwJdecLoadAcLookAheadTable(u4HwId,u4TblNum,&(rDerivedAcHuffmanTable[u4TblNum]));
    }

    // Set DC Maxcode Table
    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Set DC maxcode table %u\n", u4TblNum);
        i4HwJdecSetBitsTable(u4HwId,rDerivedDcHuffmanTable[u4TblNum].ai4Maxcode);
    }
    
    // Set DC Valoffset Table
    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Set DC value offset table %u\n", u4TblNum);
        i4HwJdecSetBitsTable(u4HwId,rDerivedDcHuffmanTable[u4TblNum].ai4Valoffset);
    }
    
    // Set AC Maxcode Table
    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Set AC maxcode table %u\n", u4TblNum);
        i4HwJdecSetBitsTable(u4HwId,rDerivedAcHuffmanTable[u4TblNum].ai4Maxcode);
    }
    
    // Set AC Valoffset Table
    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Set AC value offset table %u\n", u4TblNum);
        i4HwJdecSetBitsTable(u4HwId,rDerivedAcHuffmanTable[u4TblNum].ai4Valoffset);
    }

#ifdef CC_MT5363
    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Set DC/AC huffman table value %u\n", u4TblNum);
        i4HwJdecSetHuffTblVal(u4HwId, u4TblNum, prHuffmanTable->arDcTbl[u4TblNum].au1HuffVal, TRUE);
        i4HwJdecSetHuffTblVal(u4HwId, u4TblNum, prHuffmanTable->arAcTbl[u4TblNum].au1HuffVal, FALSE);
    }
    UNUSED(i4Ret);
#else
    // Transfer Huffman Table to HW format Huffman Table
    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Transfer DC huffman table to HW format %u\n", u4TblNum);
        i4HwJdecTransHuffTblToHwFormat(pu1HwDcHuffTbl+(u4TblNum<<8),
                                       prHuffmanTable->arDcTbl[u4TblNum].au1HuffVal);
    }

    for(u4TblNum=0; u4TblNum<4; u4TblNum++)
    {
        //Printf("Transfer AC huffman table to HW format %u\n", u4TblNum);
        i4HwJdecTransHuffTblToHwFormat(pu1HwAcHuffTbl+(u4TblNum<<8),
                                       prHuffmanTable->arAcTbl[u4TblNum].au1HuffVal);
    }
    
    // Set HW format Huffman Table Address
    if((i4Ret = i4HwJdecSetHuffAddr(u4HwId,(UINT32)pu1HwDcHuffTbl,(UINT32)pu1HwAcHuffTbl)) < 0)
    {
        return i4Ret;
    }
#endif

	return S_JDEC_HAL_OK;
}


//////////////////////////////////////////////////////////////
//
// Picture Mode (MCU Row Mode) Related Function
//
//////////////////////////////////////////////////////////////
void vHwJdecSetOutputBank0BufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr)
{
    UINT32 u4Value;

    //Printf("vHwJdecSetOutputBank0BufAddr()\n");
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH);
    u4Value |= JPG_ROW_DEC_WR_ADDR;
    u4Value &= ~(JPG_ROW_DEC_WR_BANK1_ADDR);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH,u4Value);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP0_ADDR,u4Comp0Addr/4);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP1_ADDR,u4Comp1Addr/4);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP2_ADDR,u4Comp2Addr/4);
}


void vHwJdecSetOutputBank1BufAddr(UINT32 u4HwId,UINT32 u4Comp0Addr,UINT32 u4Comp1Addr,UINT32 u4Comp2Addr)
{
    UINT32 u4Value;
    
    //Printf("vHwJdecSetOutputBank1BufAddr()\n");
    u4Comp0Addr = PHYSICAL(u4Comp0Addr);
    u4Comp1Addr = PHYSICAL(u4Comp1Addr);
    u4Comp2Addr = PHYSICAL(u4Comp2Addr);

    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH);
    u4Value |= JPG_ROW_DEC_WR_ADDR;
    u4Value |= JPG_ROW_DEC_WR_BANK1_ADDR;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH,u4Value);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP0_ADDR,u4Comp0Addr/4);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP1_ADDR,u4Comp1Addr/4);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP2_ADDR,u4Comp2Addr/4);
}


void vHwJdecSetMCUWidth(UINT32 u4HwId,UINT32 u4Comp0MCUWidth,UINT32 u4Comp1MCUWidth,UINT32 u4Comp2MCUWidth)
{
    UINT32 u4Value;

    //Printf("vHwJdecSetMCUWidth()\n");
    //set each component MCU width
    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH);
    u4Value |= JPG_ROW_DEC_WR_ADDR;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH,u4Value);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP0_ADDR_OFST,u4Comp0MCUWidth/4);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP1_ADDR_OFST,u4Comp1MCUWidth/4);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_ROW_DEC_COMP2_ADDR_OFST,u4Comp2MCUWidth/4);
}


void vHwJdecSetPictureSize(UINT32 u4HwId,JDEC_JFIF_SOF_T *prPicInfo)
{
    UINT32 u4MCUsPerRow;
    UINT32 u4TotalMCURows;
    UINT32 u4WidthPerMCU = prPicInfo->arSofComp[0].u1HSampFactor * 8;
    UINT32 u4HeightPerMCU = prPicInfo->arSofComp[0].u1VSampFactor * 8;

    //Printf("vHwJdecSetPictureSize()\n");
    u4MCUsPerRow = (prPicInfo->u2ImageWidth + u4WidthPerMCU - 1) / u4WidthPerMCU;
    u4TotalMCURows = (prPicInfo->u2ImageHeight + u4HeightPerMCU - 1) / u4HeightPerMCU;

    //set picture width and height
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_PIC_SIZE,(u4MCUsPerRow<<16)+u4TotalMCURows);
}


void vHwJdecSetRestartInterval(UINT32 u4HwId,UINT32 u4RestartInterval)
{
    //Printf("vHwJdecSetRestartInterval()\n");
#ifdef CC_MT5363
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART_INTERVAL,((u4RestartInterval-1) & 0xFFFF) | JPG_RST_ENABLE);
#else
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_RESTART_INTERVAL,((u4RestartInterval-1) & 0xFFFF));
#endif
}


void vHwJdecSetDecodeMode(UINT32 u4HwId,JDEC_HAL_DEC_MODE_T eDecodeMode)
{
    UINT32 u4Value;

    //Printf("vHwJdecSetDecodeMode()\n");
    u4Value = u4HwJdecRead32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH);
    u4Value &= ~(JPG_ROW_DEC_MCU_LEVEL | JPG_ROW_DEC_MCU_ROW_LEVEL | JPG_ROW_DEC_PIC_LEVEL);

    switch(eDecodeMode)
    {
        case JDEC_HAL_DEC_MODE_BASELINE_MCU:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_MULTI_COLLECT:
        case JDEC_HAL_DEC_MODE_PROGRESSIVE_MCU_ENHANCE:
            u4Value |= JPG_ROW_DEC_MCU_LEVEL;
            vHwJdecWrite32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH,u4Value);
            break;
        case JDEC_HAL_DEC_MODE_BASELINE_MCU_ROW:
            u4Value |= JPG_ROW_DEC_MCU_ROW_LEVEL;
            vHwJdecWrite32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH,u4Value);
            break;
        case JDEC_HAL_DEC_MODE_BASELINE_PIC:
            u4Value |= JPG_ROW_DEC_PIC_LEVEL;
            vHwJdecWrite32(u4HwId,RW_VLD_JPG_MB_ROW_DEC_SWITCH,u4Value);
            break;
    }
}


void vHwJdecDecodePicture(UINT32 u4HwId)
{
    //Printf("vHwJdecDecodePicture()\n");
    //trigger jpeg picture level decode
    vHwJdecWrite32(u4HwId,RW_VLD_STA_JPG_DEC_MB_ROW,JPG_ROW_DEC_FIRST_ROW);
    vHwJdecWrite32(u4HwId,RW_VLD_STA_JPG_DEC_MB_ROW,0);
}


BOOL fgHwJdecIsDecodePictureFinish(UINT32 u4HwId)
{
    BOOL fgRet;

    //Printf("fgHwJdecIsDecodePictureFinish()\n");
    fgRet = ((u4HwJdecRead32(u4HwId,RW_VLD_JPG_ROW_PIC_DEC_FINISH) & JPG_ROW_DEC_PIC_FINISH) == JPG_ROW_DEC_PIC_FINISH);
    return fgRet;
}


void vHwJdecDecodeMCURow(UINT32 u4HwId,BOOL fgFirstRow)
{
    //Printf("vHwJdecDecodeMCURow()\n");
    //trigger jpeg picture level decode
    if(fgFirstRow)
        vHwJdecWrite32(u4HwId,RW_VLD_STA_JPG_DEC_MB_ROW,JPG_ROW_DEC_FIRST_ROW);
    else
        vHwJdecWrite32(u4HwId,RW_VLD_STA_JPG_DEC_MB_ROW,JPG_ROW_DEC_NEXT_ROW);

    vHwJdecWrite32(u4HwId,RW_VLD_STA_JPG_DEC_MB_ROW,0);
}


BOOL fgHwJdecIsDecodeMCURowFinish(UINT32 u4HwId)
{
    BOOL fgRet;
    
    //Printf("fgHwJdecIsDecodeMCURowFinish()\n");
    fgRet = ((u4HwJdecRead32(u4HwId,RW_VLD_JPG_ROW_PIC_DEC_FINISH) & JPG_ROW_DEC_MCU_ROW_FINISH) == JPG_ROW_DEC_MCU_ROW_FINISH);
    return fgRet;
}


//////////////////////////////////////////////////////////////
//
// Re-initialization
//
//////////////////////////////////////////////////////////////
INT32 i4HwJdecGetHwStatus(UINT32 u4HwId,JDEC_HAL_HW_STATUS_T *prHwStatus)
{
    UINT32 u4Reg;

    //Printf("i4HwJdecGetHwStatus()\n");
    u4Reg = u4HwJdecRead32(u4HwId,RO_VLD_JPG_BITS_LEFT);
    prHwStatus->u4BitsLeftRead = u4Reg & 0xFF;
    prHwStatus->u4BufPtrRead = (u4Reg >> 8) & 0xFF;
    prHwStatus->u4ShiftMag = (u4Reg >> 16) & 0xFF;
    prHwStatus->u4MkrRm2 = (u4Reg >> 24) & 0x1;
    prHwStatus->u4JpegBufRead = u4HwJdecRead32(u4HwId,RO_VLD_JPG_INTERNAL_BUF);
    prHwStatus->u4RestartMkr = u4HwJdecRead32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA3) & 0xFFFF;
    u4Reg = u4HwJdecRead32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA4);
    prHwStatus->u4LastDcValue0 = u4Reg & 0xFFFF;
    prHwStatus->u4LastDcValue1 = (u4Reg >> 16) & 0xFFFF;
    prHwStatus->u4LastDcValue2 = u4HwJdecRead32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA5);

	return S_JDEC_HAL_OK;
}


INT32 i4HwJdecSetHwStatus(UINT32 u4HwId,JDEC_HAL_HW_STATUS_T *prHwStatus)
{
    UINT32 u4Reg;

   // Printf("i4HwJdecSetHwStatus()\n");
    u4Reg = prHwStatus->u4BitsLeftRead & 0xFF;
    u4Reg |= (prHwStatus->u4BufPtrRead & 0xFF) << 8;
    u4Reg |= (prHwStatus->u4ShiftMag & 0xFF) << 16;
    u4Reg |= (prHwStatus->u4MkrRm2 & 0x1) << 24;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA1,u4Reg);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA2,prHwStatus->u4JpegBufRead);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA3,(prHwStatus->u4RestartMkr & 0xFFFF) << 16);
    u4Reg = prHwStatus->u4LastDcValue0 & 0xFFFF;
    u4Reg |= (prHwStatus->u4LastDcValue1 & 0xFFFF) << 16;
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA4,u4Reg);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA5,prHwStatus->u4LastDcValue2);

    // Trigger load inter data
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA_TRIGGER,1);
    vHwJdecWrite32(u4HwId,RW_VLD_JPG_LOAD_INTER_DATA_TRIGGER,0);

	return S_JDEC_HAL_OK;
}

