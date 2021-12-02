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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_autocolor.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef CC_MTK_LOADER

//define the verification functions
#define CHK_BLANK_VARIATION 0
#define CHK_PGA_LINEARITY 0 // 1
#define CHK_ADC_LINEARITY 0
#define CHK_VGA_Ref_Voltage 0
//#define CHK_GAIN_OFFSET_ITERATION 0 // 1
#define ALWAYS_BLANK 1
#define PGA_4bit_offset 0   //test for next generation IC

//debug message enable/disable  // 1 --> for debug
#define AUTO_COLOR_DEBUG 0
#define AUTO_BLANK_ADJUST_DEBUG   0 
#define PGA_LINEARITY_DEBUG 0
#define ON_CHIP_AUTO_COLOR_DEBUG 0 
#define AUTO_BLANK_MEASURE  0 
#define Debug_Monitor_Blank 0

//define constant
#define AUTO_COLOR_MAX_TIME 0x10
#define ADJUST_BLANK_RETRY_COUNT  3



#ifdef CC_MT5365_V2
#define MAX_OFFSET           0x3FF
#define MAX_GAIN               0xFFFF
#define DEFAULT_OFFSET     0x100
#define DEFAULT_GAIN         0xA000
#else
#define MAX_OFFSET           0xFF
#define MAX_GAIN               0xFF
#define DEFAULT_OFFSET     0x80
#define DEFAULT_GAIN         0x50
#endif
#define INCREASE_OFFSET     1
#define DECREASE_OFFSET    0


/******************************************************************************
 * Header Files
 *****************************************************************************/
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "vga_auto.h"
#include "drv_adcset.h"
#include "video_def.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_vga.h"
#include "drv_hdtv.h"
#include "drv_dvi.h"
#include "vga_table.h"
#include "drv_async.h"
#include "drv_autocolor.h"
#include "nptv_drvif.h"
#include "util.h"
#include "x_printf.h"
#include "vdo_misc.h"
#include "source_table.h"
#include "source_select.h"
#include "api_notify.h"
#ifndef CC_COPLAT_MT82
#include "eeprom_if.h"
#endif
#include "hw_hdtv.h"
#include "hw_pdwnc.h"
#include "drv_vdoclk.h"
#include "drv_video.h"
#include "drv_scpos.h"
#ifdef CC_UP8032_ATV
#include "drvcust_if.h"
#endif
LINT_EXT_HEADER_END
/******************************************************************************
 * Macro, Type Definition, and Variable Declaratoin
 *****************************************************************************/
//#if ON_CHIP_AUTO_COLOR
  UINT8 _OnChipAutoColorState;
  UINT8 _bAutoColorState0;
  UINT8 _bOrgType,_bCaliMode,_bOffsetGainDone;
  UINT8 _bAutoColorTimeOutCNT;	
  UINT8 _bGainOffset_channel; //_bAutoColorIsr,,_bOffset_Blank_OK;
  UINT8 _bTotalCnt ; //_bMaxValue,_bTotalCnt;
UINT8 _bType, _bOrder, _bDoubleChkFlag[3];
UINT8 _bGainIncDir[3], _bChgDirToggle[3], _bIncGainDirPre[3];
#ifdef CC_MT5365_V2
UINT16 _bGain;
UINT16 _bIndex_mim[3];
UINT16 _bCaliStep[3];
UINT16 _bAutoColorGain[4][3];
UINT16 _bAutoColorOffset[4][3];
UINT16 _bInitOffset;

#else
UINT8 _bGain;
UINT8 _bIndex_mim[3];
UINT8 _bCaliStep[3];
UINT8 _bAutoColorGain[4][3];
UINT8 _bAutoColorOffset[4][3];
UINT8 _bInitOffset;

#endif

  UINT16 _wADCTarget[3],_wData_v1v2[3],_wADC_Diff_value[3],_wDiff_mim[3];
  UINT8 _bReadMAXLevel; 
  UINT8 _bMode; 
  UINT8 bOnChipEEPROMOffsetAddr;
  UINT8 _bAutoColorGain_for_EFuse[4][3];
  UINT8 _bAutoColorHistory[4];

  UINT32 _dwBlankAvg[3];
  UINT32 _dwMaxLevelAvg[3];
  UINT32 _dwBlankAvg_1[3]; 
  UINT32 _dwMaxLevelAvg_1[3];
  UINT8 _bAdjBlkState0;
  UINT8 _bBlankIteration;
  UINT8 _bAdjGainAfterBlank;
  UINT8 _bHDTVMax[3];
  UINT8 _bVGAMax[3];
  
  UINT8 _bEEPROM_ready;
 // UINT8 _bWaitSignalStable;
  UINT8 _bDoubleCheck;
  UINT8 _bDiffOffset;

  #if COMPENSATE_GAIN
  UINT16 _wBlanklevel_before_adj[3];
  #endif
  
  UINT8  _bOffsetCaliDone[4];
  
  #if DIGITAL_NEW_OFFSET
  UINT8 _bDigSignBit;
  #endif

  #if CHK_ADC_LINEARITY
  UINT32 _dwAutoColorMax[3];
  UINT32 _dwStPoint[3];
  UINT32 _dwEndPoint[3];
  UINT32 _dwDeltaValue[3];
  UINT32 _dwMaxDiff[16];
  UINT32 _dwMaxDiffTemp;
  UINT32 _dwMaxDiffLocal;
  UINT8 _bLocation;
  UINT8 _bADCLinearity;
  UINT8 _bADC_LINEARITY_state=0;
  UINT8 TOTAL_POINT;

  //#define TOTAL_POINT 16
  enum
  {
      ADC_VERIFY_0,
      ADC_VERIFY_1,
      ADC_VERIFY_2,
      ADC_VERIFY_3,
      ADC_VERIFY_4,
      ADC_VERIFY_5,
      ADC_VERIFY_6
  };
#endif
  

  #if CHK_PGA_LINEARITY
  //UINT8 _bPGALinearity;
  UINT8 _bPGA_LINEARITY_state=0;
  enum
  {
      PGA_VERIFY_NOTHING,
      PGA_VERIFY_INIT,
      PGA_VERIFY_2,
      PGA_VERIFY_3,
      PGA_VERIFY_4,
      PGA_VERIFY_5
  };  
 #endif
UINT16  wOnChipColorGainTable_Temp[4][3];

UINT32 code wHDTV_Data_Sel[8][3] =                        //MC20080502_1 *** 
{
    {STA_HDTV_BLANK_PR, STA_HDTV_BLANK_PB, STA_HDTV_BLANK_Y},	    // 0  confirmed
    {STA_HDTV_BLANK_PR, STA_HDTV_BLANK_PB, STA_HDTV_BLANK_Y},	    // 1  T.B.F (from digital part)
    {STA_HDTV_BLANK_PB, STA_HDTV_BLANK_Y,  STA_HDTV_BLANK_PR},	    // 2  confirmed
    {STA_HDTV_BLANK_PB, STA_HDTV_BLANK_Y,  STA_HDTV_BLANK_PR},	    // 3  confirmed
    {STA_HDTV_BLANK_PR, STA_HDTV_BLANK_Y,  STA_HDTV_BLANK_PB},	    // 4  T.B.F
    {STA_HDTV_BLANK_Y,  STA_HDTV_BLANK_PB, STA_HDTV_BLANK_PR},	    // 5  T.B.F (from digital part)
    {STA_HDTV_BLANK_Y,  STA_HDTV_BLANK_PB, STA_HDTV_BLANK_PR},      // 6  confirmed
    {STA_HDTV_BLANK_PR, STA_HDTV_BLANK_Y,  STA_HDTV_BLANK_PB},      // 7  confirmed
};               

extern UINT16 code wColorBlankValueNew[4][3]; 
extern UINT8 code bColorMaxType1[4][3];
extern UINT8 code bColorMaxType2[4][3];
extern UINT8 code bColorMaxType3[4][3];
extern UINT16 code wOnChipColorMaxType[4][3];
extern UINT16 code wOnChipColorGainTable[4][3];
extern UINT16 code wYPbPrMappingVgaGainTable[4][3];
//extern UINT16 code wOnChipColorGainTable_SONY[4][3];
extern UINT8 code bOnChipCalibrateTolerance[5][4];
extern UINT8 code bOnChipCheckTolerance[5][2];
extern UINT8 code GAIN_HIGH_LIMIT_100[4][3];
extern UINT8 code GAIN_LOW_LIMIT_100[4][3];
extern UINT8 code GAIN_HIGH_LIMIT_120[4][3];
extern UINT8 code GAIN_LOW_LIMIT_120[4][3];
extern UINT8 code GAIN_HIGH_LIMIT_75_100[4][3];
extern UINT8 code GAIN_LOW_LIMIT_75_100[4][3];
extern UINT8 code bVGADefaultOffsetTABLE_100[4][3];

#ifdef CC_MT5365_V2
extern UINT16 code GAIN_HIGH_LIMIT_100_DIGITAL[4][3];
extern UINT16 code GAIN_LOW_LIMIT_100_DIGITAL[4][3];
#endif

//extern UINT8 code GAIN_HIGH_LIMIT_SONY[4][3];
//extern UINT8 code GAIN_LOW_LIMIT_SONY[4][3];
//extern UINT8 code bVGADefaultOffsetTABLE_SONY[4][3];

#ifdef CC_UP8032_ATV
extern BOOL fgApiEepromWriteByte(WORD wAddr, BYTE bData);
#endif

/******************************************************************************
 * Function vDrvGeneralAutoColorGainSetToNextType Declaration
 *****************************************************************************/
#ifdef CC_MT5365_V2
UINT16 AnalogToDigitalGain(UINT8 bGain)
{
  UINT16 DigitalGain;
  
  DigitalGain = 32767*(UINT32)bGain/255 + 32768;//notice (UINT32)bGain @ 8032

  return DigitalGain;
}
#endif
void vDrvGeneralAutoColorGainSetToNextType(UINT16* ColorGainTablePtr, UINT8 bOrgType, UINT8 bNextType)
{
   UINT8 bOrder,bGain;
   UINT16 wDen,wNum;
   UINT32 dGain;
   UINT16 ColorGainTable[4][3],i,j;

   for(i=0;i<4;i++)
   	for(j=0;j<3;j++)
   	{
   	  ColorGainTable[i][j] = *(ColorGainTablePtr+i*3+j);
   	}
   
   for(bOrder=0; bOrder<3; bOrder++)
   {
      dGain = _bAutoColorGain[bOrgType][bOrder];
      wNum  = ColorGainTable[bOrgType][bOrder];
      wDen  = ColorGainTable[bNextType][bOrder];
//      dGain = ((0xff + dGain)* wDen) - (0xff * wNum);
      dGain = ((0xff + dGain)* wDen) - (0xff * (UINT32)wNum);
      bGain = (dGain + (wNum>>1)) /wNum ;  //round off
      _bAutoColorGain[bNextType][bOrder] = bGain;
  }
}

/******************************************************************************
 * Function Forward Declaration
 *****************************************************************************/
#ifdef CC_MT5365_V2
PRIVATE UINT16 bDrvGetHDADCOffset(UINT8 bOrder)
{
UINT16 wTmpOffset=0;
	switch (bOrder)
	{
		case 0:
		       wTmpOffset = RegReadFldAlign(VFE_02, PGAOFFSET_1H) & 0x03;
		       wTmpOffset = (wTmpOffset<<8)|(UINT16)(RegReadFldAlign(VFE_02, PGAOFFSET_1L));	
		       break;
		case 1:
		       wTmpOffset = RegReadFldAlign(VFE_06, PGAOFFSET_2H) & 0x03;
		       wTmpOffset = (wTmpOffset<<8)|(UINT16)(RegReadFldAlign(VFE_06, PGAOFFSET_2L));
		       break;
		case 2:
		default:
		       wTmpOffset = RegReadFldAlign(VFE_0A, PGAOFFSET_3H) & 0x03;
		       wTmpOffset = (wTmpOffset<<8)|(UINT16)(RegReadFldAlign(VFE_0A, PGAOFFSET_3L));
		       break;
	}
	return (wTmpOffset);
}

void bDrvSetHDADCOffset(UINT8 bOrder,UINT16 value)
{
UINT8 bTmpHighByte,bTmpLowByte;

	bTmpHighByte = (value>>8)&0x03;
	bTmpLowByte = value & 0xff;
	switch (bOrder)
	{
		case 0:
			vRegWriteFldAlign(VFE_02, bTmpLowByte, PGAOFFSET_1L);
			vRegWriteFldAlign(VFE_02, bTmpHighByte, PGAOFFSET_1H);
			break;
		case 1:
			vRegWriteFldAlign(VFE_06, bTmpLowByte, PGAOFFSET_2L);
			vRegWriteFldAlign(VFE_06, bTmpHighByte, PGAOFFSET_2H);
			break;
		case 2:
		default:
			vRegWriteFldAlign(VFE_0A, bTmpLowByte, PGAOFFSET_3L);
			vRegWriteFldAlign(VFE_0A, bTmpHighByte, PGAOFFSET_3H);
			break;
	}
}
#else
PRIVATE UINT8 bDrvGetHDADCOffset(UINT8 bOrder)
{
	switch (bOrder)
	{
		case 0:
			return (RegReadFldAlign(VFE_02, PGAOFFSET_1));	//R channel maximum value in a frame
		case 1:
			return (RegReadFldAlign(VFE_06, PGAOFFSET_2));	//G channel maximum value in a frame
		case 2:
		default:
			return (RegReadFldAlign(VFE_0A, PGAOFFSET_3));	//B channel maximum value in a frame
	}
}

void bDrvSetHDADCOffset(UINT8 bOrder,UINT8 value)
{
	switch (bOrder)
	{
		case 0:
			vRegWriteFldAlign(VFE_02, value, PGAOFFSET_1);
			break;
		case 1:
			vRegWriteFldAlign(VFE_06, value, PGAOFFSET_2);
			break;
		case 2:
		default:
			vRegWriteFldAlign(VFE_0A, value, PGAOFFSET_3);
			break;
	}
}
#endif


#ifdef CC_MT5365_V2
/**
 * @brief bDrvGetHDADCGain_Digital(BYTE bOrder )
 * Get digital gain value 
 * @param  bOrder : HDADC Channel
 * @retval gain value
 * @example bDrvGetHDADCGain_Digital(bOrder)
 */
PRIVATE UINT16 bDrvGetHDADCGain_Digital(UINT8 bOrder)
{
    switch (bOrder)
    {
        case 0:
            return (RegReadFldAlign(HDFE_01, AD1_GAIN));	//R channel maximum value in a frame
        case 1:
            return (RegReadFldAlign(HDFE_01, AD2_GAIN));	//G channel maximum value in a frame
        case 2:
        default:
            return (RegReadFldAlign(HDFE_02, AD3_GAIN));	//B channel maximum value in a frame
    }
}
#else
/**
 * @brief bDrvGetHDADCGain( BYTE bOrder )
 * Get ADC gain Value of frame
 * @param  bOrder : HDADC Channel
 * @retval gain value
 * @example bDrvGetHDADCGain()
 */
PRIVATE UINT8 bDrvGetHDADCGain(UINT8 bOrder)
{
	switch (bOrder)
	{
		case 0:
			return (RegReadFldAlign(VFE_02, PGAGAIN_1));	//R channel maximum value in a frame
		case 1:
			return (RegReadFldAlign(VFE_06, PGAGAIN_2));	//G channel maximum value in a frame
		case 2:
		default:
			return (RegReadFldAlign(VFE_0A, PGAGAIN_3));	//B channel maximum value in a frame
	}
}

#endif

void bDrvSetHDADCGain(UINT8 bOrder,UINT8 value)
{
	switch (bOrder)
	{
	#ifdef CC_MT5365_V2
            case 0:
            case 1:
            case 2:
            default:
                   break;
	#else
		case 0:
			vRegWriteFldAlign(VFE_02, value, PGAGAIN_1);
			break;
		case 1:
			vRegWriteFldAlign(VFE_06, value, PGAGAIN_2);
			break;
		case 2:
		default:
			vRegWriteFldAlign(VFE_0A, value, PGAGAIN_3);
			break;
       #endif			
	}
}

#ifdef CC_MT5365_V2
PRIVATE void bDrvSetHDADCGain_Digital(UINT8 bOrder, UINT16 value)
{
    switch (bOrder)
    {
        case 0:
            vRegWriteFldAlign(HDFE_01, value, AD1_GAIN);
            break;
        case 1:
            vRegWriteFldAlign(HDFE_01, value, AD2_GAIN);
            break;
        case 2:
        default:
            vRegWriteFldAlign(HDFE_02, value, AD3_GAIN);
            break;
    }
}
#endif



PRIVATE UINT8 bDrvGetColorTargetValue(UINT8 bType, UINT8 bOrder)
{
   if((bType<4)&&(bOrder<3))
   {
       if(DRVCUST_OptGet(eVFE_HW_075ohm))
       {
             return ( bColorMaxType1[bType][bOrder]);
       }
       else if(DRVCUST_OptGet(eVFE_HW_SPECIAL_TARGET))
       {
             return ( bColorMaxType3[bType][bOrder]);
       }
       else
       {
           if(DRVCUST_OptGet(eYPBPR_ADC_SUPPORT_120))
           {
               return ( bColorMaxType2[bType][bOrder]);
           }
           else
           {
               return (bColorMaxType1[bType][bOrder]);
           }
       }
   }
   else
   {
      return 0xff;
   }
}   


PRIVATE void vDrvSetDigitalGain(UINT8 bUpDown, UINT8 bGain)
{
UINT8 bGainWeighting;
      bGainWeighting =0x40;
      if(bUpDown==0)
      {
             switch (_bOrder)
             {
                       case 0:
                vRegWriteFldAlign(HDFE_01, 0x8000 - ((UINT16)bGain*(UINT16)bGainWeighting), AD1_GAIN);  // reset the Digital gain to uint gain
                           break;
                       case 1:
                vRegWriteFldAlign(HDFE_01, 0x8000 - ((UINT16)bGain*(UINT16)bGainWeighting), AD2_GAIN);  // reset the Digital gain to uint gain
                           break;
                       case 2:
                       default:
                vRegWriteFldAlign(HDFE_02, 0x8000 - ((UINT16)bGain*(UINT16)bGainWeighting), AD3_GAIN);  // reset the Digital gain to uint gain
                           break;
             }
     }
     else
     {
             switch (_bOrder)
             {
                       case 0:
                vRegWriteFldAlign(HDFE_01, 0x8000 + ((UINT16)bGain*(UINT16)bGainWeighting), AD1_GAIN);  // reset the Digital gain to uint gain
                              break;
                       case 1:
                vRegWriteFldAlign(HDFE_01, 0x8000 + ((UINT16)bGain*(UINT16)bGainWeighting), AD2_GAIN);  // reset the Digital gain to uint gain
                              break;
                       case 2:
                       default:
                vRegWriteFldAlign(HDFE_02, 0x8000 + ((UINT16)bGain*(UINT16)bGainWeighting), AD3_GAIN);  // reset the Digital gain to uint gain
                           break;
             }
      }
}


UINT16 bDrvGetBlankVal(UINT8 bOrder)    //MC20080502_1 ***
{
UINT8 bDataOrder;
UINT16 wBlankValue;

    bDataOrder = RegReadFldAlign(HDTV_03,HDTV_DATA_SEL);
    if ((bDataOrder<8)&&(bOrder<3))
    {
    wBlankValue= RegReadFldAlign(HDTV_STA_06,wHDTV_Data_Sel[bDataOrder][bOrder]);
    return (wBlankValue);
    }
    else
    {
    return (0xFFFF);
    }
}                                         //MC20080502_1 &&&                                       //MC20080502_1 &&&

PRIVATE void vDrvPreSetToReadBlank(UINT8 bOnOff)
{
    #if ALWAYS_BLANK
      bOnOff = 0;
      vRegWriteFldAlign(HDTV_03, bOnOff, HDTV_BLAK_SET);
    #else
      vRegWriteFldAlign(HDTV_03, bOnOff, HDTV_BLAK_SET);
    #endif
    
}


#if DIGITAL_NEW_OFFSET
/**
 * @brief bDrvSetHDADCDigitalOffset( void )
 *
 * @param  bOrder,value
 *
 * @retval None
  */
/*
#ifdef CC_MT5365_V2
void vDrvSetHDADCDigitalOffset_V2(UINT8 bOrder,UINT16 value_16bit)
{
UINT8 value;
       value = value_16bit & 0xff;
       switch (bOrder)
       {
           case 0:
       	vRegWriteFldAlign(HDFE_00, _bDigSignBit, SIGN_OFFSET1);
		vRegWriteFldAlign(HDFE_02, value, AD1_OFFSET);          
               break;
           case 1:
       	vRegWriteFldAlign(HDFE_00, _bDigSignBit, SIGN_OFFSET2);
		vRegWriteFldAlign(HDFE_02, value, AD2_OFFSET);          
               break;
           case 2:
       	vRegWriteFldAlign(HDFE_00, _bDigSignBit, SIGN_OFFSET3);
		vRegWriteFldAlign(HDFE_00, value, AD3_OFFSET);          
               break;
       }
}

#else
*/
void vDrvSetHDADCDigitalOffset(UINT8 bOrder,UINT8 value)
{
       switch (bOrder)
       {
           case 0:
       	vRegWriteFldAlign(HDFE_00, _bDigSignBit, SIGN_OFFSET1);
		vRegWriteFldAlign(HDFE_02, value, AD1_OFFSET);          
               break;
           case 1:
       	vRegWriteFldAlign(HDFE_00, _bDigSignBit, SIGN_OFFSET2);
		vRegWriteFldAlign(HDFE_02, value, AD2_OFFSET);          
               break;
           case 2:
       	vRegWriteFldAlign(HDFE_00, _bDigSignBit, SIGN_OFFSET3);
		vRegWriteFldAlign(HDFE_00, value, AD3_OFFSET);          
               break;
       }
}
//#endif
static UINT8 vReadSignBit(UINT8 bOrder)
{
    switch (bOrder)
    {
        case 0:
           return(RegReadFldAlign(HDFE_00, SIGN_OFFSET1));
        case 1:
           return(RegReadFldAlign(HDFE_00, SIGN_OFFSET2));
        case 2:
        default:
           return(RegReadFldAlign(HDFE_00, SIGN_OFFSET3));
   }
}

static UINT8 bReadHDTVDigitalOffset(UINT8 bOrder)
{
    switch (bOrder)
    {
        case 0:
           return(RegReadFldAlign(HDFE_02, AD1_OFFSET));
        case 1:
           return(RegReadFldAlign(HDFE_02, AD2_OFFSET));
        case 2:
        default:
           return(RegReadFldAlign(HDFE_00, AD3_OFFSET));
   }
}

#if AUTO_BLANK_ADJUST_DEBUG
static UINT16 wReadHDTVDigitalGain(UINT8 bOrder)
{
    switch (bOrder)
    {
        case 0:
           return(RegReadFldAlign(HDFE_01, AD1_GAIN));
        case 1:
           return(RegReadFldAlign(HDFE_01, AD2_GAIN));
        case 2:
        default:
           return(RegReadFldAlign(HDFE_02, AD3_GAIN));
   }
}
#endif
UINT8 vCalculateDigOffset(UINT8 bOffset,UINT8 bAdjOffset,UINT8 bCmpResult)
{
    if(bCmpResult>0)
    {
         if(_bDigSignBit>0)  // current digital offset value is positive
         {
             if(bOffset>=bAdjOffset)
             {
                  bOffset = bOffset - bAdjOffset;
             }
             else
             {
                  bOffset = bAdjOffset- bOffset;
                  _bDigSignBit = 0;
             }
         }
         else
         {
             if(bOffset <(0xff- bAdjOffset))
             {
                  bOffset = bOffset + bAdjOffset;
             } 
             else
             {
                 bOffset = 0xff;
             }                                      
        }
    }
    else
    {
        if(_bDigSignBit>0)
        {
            if(bOffset<(0xff-bAdjOffset))
            {
                 bOffset= bOffset +bAdjOffset;
            }
            else
            {
                 bOffset = 0xff;
            }
        }
        else
        {
            if(bOffset<bAdjOffset)
            {
                 bOffset = bAdjOffset - bOffset;
                 _bDigSignBit=1;
            }
            else
            {
                 bOffset= bOffset - bAdjOffset;
            }
       }
   }
   return (bOffset);
}

#endif





/**
 * @brief vDrvIntAutoColorStart( void )
 * Internal auto color start
 * @param  void
 * @retval void
 * @example vDrvIntAutoColorStart()
 */
void vDrvIntAutoColorStart(void)
{
	 _bAutoColorState0 = VDO_AUTO_COLOR_START;
}


//#if ON_CHIP_AUTO_COLOR

UINT8 bDrvOnChipGetADCMaxValueRGBYPBPR(UINT8 bOrder)
{
 return(bDrvOnChipGetADCMaxMinValue(bOrder,AS_PHASE_MAX_SEL));
}


UINT8 bDrvOnChipGetADCMaxMinValue(UINT8 bOrder,UINT8 bMode)
{
  vRegWriteFldAlign(ASYNC_0F, bMode , AS_PHASE_MAXMIN_SEL);
  switch (bOrder)
  {
    case 0:
	  return (RegReadFldAlign(STA_SYNC0_07, AS_RMAXMIND));	//R channel maximum value in a frame
    case 1:
	  return (RegReadFldAlign(STA_SYNC0_08, AS_GMAXMIND));	//G channel maximum value in a frame
    case 2:
    default:
	  return (RegReadFldAlign(STA_SYNC0_09, AS_BMAXMIND));	//B channel maximum value in a frame
  }
}



UINT8 bDrvOnChipGetVFESignalType(UINT8 bAutoInput)
{
  UINT8 bColor_Type;
  
		  switch (bAutoInput)
	   {
			  case P_VGA:
				  bColor_Type =(UINT8)INT_VGA ;
				  break;
			  case P_YP0:
			  case P_YP1:
				  bColor_Type = (UINT8)INT_HDTV ;
				  break;
			  case P_VGACOMP:
			  	bColor_Type = (UINT8)INT_VGA_COMPOENT ;
				  break;
			   case P_FB0:
			  case P_FB1:
				  bColor_Type = (UINT8)INT_SCART;
				  break;
		   default:
				  bColor_Type = (UINT8)P_FA;
				  break;
	   }
    //Printf("---bAutoInput=%x,  bColor_Type=%x   \n",bAutoInput,bColor_Type);
    return (bColor_Type);
}
 

PRIVATE void vDrvOnChipAutoColorGainSetToNextType(UINT8 bOrgType, UINT8 bNextType)
{
   UINT8 bOrder,bGain;
   UINT16 wDen,wNum;
   UINT32 dGain;

   for(bOrder=0; bOrder<3; bOrder++)
   {
        if (_bEEPROM_ready==1)
        {
          dGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_START + (bOrgType*3) + bOrder);
        }
        else
        {
          dGain = _bAutoColorGain[bOrgType][bOrder];
        }
          wNum  = wOnChipColorGainTable[bOrgType][bOrder];
          wDen  = wOnChipColorGainTable[bNextType][bOrder];
          //dGain = ((0xff + dGain)* wDen) - (0xff * wNum);
          dGain = ((0xff + dGain)* wDen) - (0xff * (UINT32)wNum);
          bGain = (dGain + (wNum>1)) /wNum ;  //round off
        if (_bEEPROM_ready==1)
        {
          UNUSED(fgApiEepromWriteByte((EEP_VIDEO_AUTO_COLOR_START + (bNextType*3) + bOrder), bGain));
        }
        else
        {
           _bAutoColorGain[bNextType][bOrder] = bGain;
        }
  }
}

//==========================================================================
//function    : vDrvOnChipGainMapping(UINT8 bType)
//description : Use the VGA gain calibration to calculate the gain for the other signal type 
//input       : bType
//return      : none
//===========================================================================
PRIVATE void vDrvOnChipGainMapping(UINT8 bColorType)
{
  UINT8 bNextType;

  switch (bColorType)
  {
   case (UINT8)INT_HDTV:    //YPbPr to VGA
      bNextType = (UINT8)INT_VGA ;
      vDrvOnChipAutoColorGainSetToNextType(bColorType,bNextType);
      bNextType = (UINT8)INT_SCART ;
      vDrvOnChipAutoColorGainSetToNextType(bColorType,bNextType);
      bNextType = (UINT8)INT_VGA_COMPOENT ;
      vDrvOnChipAutoColorGainSetToNextType(bColorType,bNextType);
      break;

  case (UINT8)INT_VGA:     //input source is VGA and map gain to YPbPr
      bNextType = (UINT8)INT_HDTV ;
      vDrvOnChipAutoColorGainSetToNextType(bColorType,bNextType);
      bNextType = (UINT8)INT_SCART ;
      vDrvOnChipAutoColorGainSetToNextType(bColorType,bNextType);
      bNextType = (UINT8)INT_VGA_COMPOENT ;
      vDrvOnChipAutoColorGainSetToNextType(bColorType,bNextType);
      break;
  }
}

#if ON_CHIP_AUTO_COLOR_DEBUG
PRIVATE void vDrvCheckGain(void)
{
UINT8 bGain,bOffset,bType,bOrder;
  for(bType=0; bType<4; bType++)
  {
    for(bOrder=0; bOrder<3; bOrder++)
    {
            if (_bEEPROM_ready==1)
            {
      bGain = bApiEepromReadByte(EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bOrder);
      bOffset = bApiEepromReadByte(EEP_ADC_OFFSET_START + (bType*3)+bOrder);
            }
            else
            {
      bGain   = _bAutoColorGain[bType][bOrder];
      bOffset = _bAutoColorOffset[bType][bOrder];
            }
    Printf("check Type=%u, bOrder=%u, bGain=%x, bOffset=%x\n",bType,bOrder,bGain,bOffset);
    }
  }
}
#endif
PRIVATE void vDrvOnChipModeOnOff(UINT8 bOnOff)
{
UINT8 bCh;
    vIO32WriteFldAlign(VGACFG0, bOnOff, FLD_RG_VMUX_PWD); //to power down VGA MUX    

    vRegWriteFldAlign(VFE_00, bOnOff, ALWAYSAG); // 1:enable "PGA Negative AG Always Connect" 
    for (bCh=0; bCh<3; bCh++)
    {
       //vRegWriteFldAlign(VFE_01+(bCh<<4), 0, CLAMP_1_PWD); //5365 mark //don't power down clamp no matter what bOnOff=1 or 0
       if(bOnOff==0)
       {
//	  vRegWriteFldAlign(VFE_01+(bCh<<4), 1, PGA_1_PWD);		//power down PGA  // [SA7_Nick] 080525 marked
//	  vRegWriteFldAlign(VFE_03+(bCh<<4), 1, ADC_1_PWD);	       //power down ADC  // [SA7_Nick] 080525 marked
       }
       else
       {
	  vRegWriteFldAlign(VFE_01+(bCh<<4), 0, PGA_1_PWD);		//power on PGA
	  vRegWriteFldAlign(VFE_03+(bCh<<4), 0, ADC_1_PWD);	       //power on ADC
       }
      vRegWriteFldAlign(VFE_01+(bCh<<4),bOnOff,CLAMP_SOURCE_OFF_1);
      vRegWriteFldAlign(VFE_01+(bCh<<4),bOnOff,CLAMP_SINK_OFF_1);
    }
 
    vRegWriteFldAlign(HDTV_03, bOnOff, HDTV_EN);    
  
    if(bOnOff==0)     //turn off
    {
         #if Phase3Channel// 3 phase
               vIO32WriteFldMulti(CKGEN_AFEPLLCFG4, P_Fld(0x1, FLD_RG_VGAPLL_CKO_SEL)|P_Fld(0x1, FLD_RG_VGAPLL_B_EN)|P_Fld(0x1,FLD_RG_VGAPLL_G_EN)|P_Fld(0x1,FLD_RG_VGAPLL_R_EN));
        #else//single phase
               vIO32WriteFldMulti(CKGEN_AFEPLLCFG4, P_Fld(0x2, FLD_RG_VGAPLL_CKO_SEL)|P_Fld(0x0, FLD_RG_VGAPLL_B_EN)|P_Fld(0x0,FLD_RG_VGAPLL_G_EN)|P_Fld(1,FLD_RG_VGAPLL_R_EN));
        #endif
          vIO32WriteFldAlign(CKGEN_AFEPLLCFG7, 0x0, FLD_RG_VGAPLL_X27IN_EN);
       vRegWriteFldAlign(VFE_04, 1, ACBUF_PWD);
       vRegWriteFldAlign(VFE_04, 0, ACCH1IN);
       vRegWriteFldAlign(VFE_08, 0, ACCH2IN );
       vRegWriteFldAlign(VFE_0C, 0, ACCH3IN );
    }
    else
    {
       vIO32WriteFldAlign(CKGEN_AFEPLLCFG4, 0x0, FLD_RG_VGAPLL_CKO_SEL);
       vIO32WriteFldAlign(CKGEN_AFEPLLCFG7, 0x1, FLD_RG_VGAPLL_X27IN_EN);
       //CKGEN_WRITE32(REG_ADCPLL_CFG0, ((CKGEN_READ32(REG_ADCPLL_CFG0) & 0xFFFFFF3F) | 0x00000000)); //Set ADC clock=27MHz
       vRegWriteFldAlign(VFE_04, 0, ACBUF_PWD );   //to power down "ON CHIP auto color buffer and disable 825/450mv,
    }

}
    
/*
void vDrvPreSetToReadBlank(UINT8 bOnOff)
{
    UINT8 bValue;
    bValue = (bOnOff==0) ? 2 : 0 ;
    vRegWriteFldAlign(HDTV_03, bValue, HDTV_BLAK_SET);
}
*/

PRIVATE void vDrvSelectAutoColorBufferOutputChannel(UINT8 bOrder)
{
  vRegWriteFldAlign(VFE_04+(4<<(bOrder)), 1, ACCH1IN); //5360 select channel buffer of ON CHIP AUTO COLOR 
}

PRIVATE void vDrvSelV1V2(UINT8 bLoopCnt_V1V2)
{
                if(bLoopCnt_V1V2 ==0)
             	  {
                   vRegWriteFldAlign(VFE_04, 1, ACHIN); //Select V2=750mv  
           	  }
                else
                {
                   vRegWriteFldAlign(VFE_04, 0, ACHIN); //Select V1=400mv 
               }
}

PRIVATE UINT16 bDrvGetADCData(UINT8 bOrder)
{

	switch (bOrder)
	{
		case 0:
			return (RegReadFldAlign(HDTV_STA_07, STA_HDTV_ADC1));	//ADC channel 1 raw data
		case 1:
			return (RegReadFldAlign(HDTV_STA_07, STA_HDTV_ADC2));	//ADC channel 2 raw data
		case 2:
		default:
			return (RegReadFldAlign(HDTV_STA_07, STA_HDTV_ADC3));	//ADC channel 3 raw data
	}
}

PRIVATE void vDrvOnChipAutoColorModeSet(UINT8 bMode, UINT8 bType)
{
if((bType<=INT_VGA_COMPOENT) &&(_bOrder<3))//for klocwork check
{     
   switch (bMode)
   {
     case ON_CHIP_GAIN_MODE:             //On chip Gain calibration mode
          _bInitOffset  = DEFAULT_OFFSET;
          _wADCTarget[_bOrder] = wOnChipColorMaxType[bType][_bOrder];      
          vDrvOnChipModeOnOff(1);
          bDrvSetHDADCGain(_bOrder,0x50); 
          //Printf("Gain order=%2u, target=0x%x\n",_bOrder,_wADCTarget[_bOrder]);
          break;
	case OFFSET_MODE:    
          _wADCTarget[_bOrder] = wColorBlankValueNew[bType][_bOrder];
            _bInitOffset= _bAutoColorOffset[bType][_bOrder];
	    if( (_bInitOffset==MAX_OFFSET)||(_bInitOffset==0))   // initial value protect
	    	{
	    	_bInitOffset=DEFAULT_OFFSET;
	    	}
          vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
          //Printf("offset order=%2u, target=0x%x\n",_bOrder,_wADCTarget[_bOrder]);
	   break;

     case GAIN_MODE:    
     	      _bGain= _bAutoColorGain[bType][_bOrder];
		if((_bGain==MAX_GAIN)||(_bGain==0))   // initial value protect
		{
		    _bGain = DEFAULT_GAIN;
		}
		_bInitOffset= _bAutoColorOffset[bType][_bOrder];
		if((_bInitOffset==MAX_OFFSET)||(_bInitOffset==0))   // initial value protect
		{
		    _bInitOffset = DEFAULT_OFFSET;
		}
		_wADCTarget[_bOrder]  = bDrvGetColorTargetValue(bType, _bOrder); //bColorMaxTypeNew[bType][_bOrder];

                #ifdef CC_MT5365_V2
                     bDrvSetHDADCGain_Digital(_bOrder, _bGain);
                #else
                     bDrvSetHDADCGain(_bOrder,_bGain);
                #endif
          vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
          //Printf("Gain order=%2u, _bGain=%x, _bInitOffset%x,target=0x%x\n",_bOrder,_bGain,_bInitOffset,_wADCTarget[_bOrder]);
          break;
   #if DIGITAL_NEW_OFFSET
     case DIGITAL_OFFSET_MODE:
          _wADCTarget[_bOrder] = wColorBlankValueNew[bType][_bOrder];
          _bInitOffset = bReadHDTVDigitalOffset(_bOrder);       
          _bDigSignBit = vReadSignBit(_bOrder); 
          vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
          break;
   #endif
     default:
          break;
   }   
 }
}


void vDrvHDTVMeasureSetting(UINT8 bField_Number)
{
    vRegWriteFldAlign(HDTV_00, Average_128_line , HDTV_BLK_CALI_LCNT);  //LCNT * PERIOD +START < HTOTAL
    vRegWriteFldAlign(HDTV_00, Per_1_line            , HDTV_BLK_CALI_PERIOD);
    vRegWriteFldAlign(HDTV_01, Start_line_32        , HDTV_BLK_CALI_START);
    if(bField_Number>3)
    {
       bField_Number = 3;  //maximum is 8 field
    }
    vRegWriteFldAlign(HDTV_02, bField_Number  , HDTV_BLK_CALI_FCNT);
    vRegWriteFldAlign(HDTV_03, Pixel_16_per_line  , HDTV_BLANK_AVG);
}

void vDrvHDTVClampMethodDefaultSetting(UINT8 bType)
{
UINT8 bTMP1[3];
    #ifdef FULLY_HW_AUTO_CALIBRATION
    if(bType<=INT_VGA_COMPOENT)   //for klocwork check
    {
            bTMP1[0]= wColorBlankValueNew[bType][0]>>6;
            bTMP1[1]= wColorBlankValueNew[bType][1]>>6;
            bTMP1[2]= wColorBlankValueNew[bType][2]>>6;
            vRegWriteFldAlign(HDTV_02, bTMP1[0] , HDTV_BLK_CALI_Y_TAR); 
            vRegWriteFldAlign(HDTV_02, bTMP1[1] , HDTV_BLK_CALI_PB_TAR); 
            vRegWriteFldAlign(HDTV_02, bTMP1[2] , HDTV_BLK_CALI_PR_TAR); 
            vRegWriteFldAlign(HDTV_00, 0x02  , HDTV_BLK_CALI_MIN);    //mimumu:2 code 10bit
            vRegWriteFldAlign(HDTV_02, 0x3f  , HDTV_BLK_CALI_THRES); //maximum:16 code 8bit
            vRegWriteFldAlign(HDTV_02, CALI_ENABLE  , HDTV_BLK_CALI_EN);          //Enable line blank average measurement function
            vRegWriteFldAlign(HDTV_02, CALI_ENABLE  , HDTV_BLK_CALI_ADJ_ON);  //Enable HW blank Adjust Function
            vRegWriteFldAlign(HDTV_00, CALI_DISABLE   , HDTV_LCLAMP_EN);          //Disable line clamp
     }       
   #else 
      #if HW_MEASURE_BLANK  //No HDTV claping function and only HDTV HW blank level measurement method
            vRegWriteFldAlign(HDTV_02, CALI_ENABLE  , HDTV_BLK_CALI_EN);           //Enable line blank average measurement function
            vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_ADJ_ON); //Disable HW blank Adjust Function
            vRegWriteFldAlign(HDTV_00, CALI_DISABLE   , HDTV_LCLAMP_EN);           //Disable line clamp
            vRegWriteFldAlign(HDTV_00, BEFORE_ADJ, HDTV_BLK_STA_SEL);
      #else                             //No HDTV clamping function and No HDTV HW blank level measurement method
            vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_EN);         //Disable line blank average measurement function
            vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_ADJ_ON); //Disable HW blank Adjust Function
            vRegWriteFldAlign(HDTV_00, CALI_DISABLE   , HDTV_LCLAMP_EN);           //Disable line clamp
            vRegWriteFldAlign(HDTV_00, AFTER_ADJ   , HDTV_BLK_STA_SEL); //blank level after calibration
      #endif
   #endif
  
// 128 *2+32 = 288 > 262 per field (480i)  --> can not use for 480i
// 128 *1+32 = 160 < 262 per field (480i)  --> can use for 480i
}

#ifdef FULLY_HW_AUTO_CALIBRATION
void vDrvHDTV_HW_AUTO_ONOFF(UINT8 bMode, UINT8 bType)
{
UINT8 bTMP1[3];
        if((bType !=P_FA) && (bType<=INT_VGA_COMPOENT))   //for klocwork check
        {
            bTMP1[0]= wColorBlankValueNew[bType][0]>>6;
            bTMP1[1]= wColorBlankValueNew[bType][1]>>6;
            bTMP1[2]= wColorBlankValueNew[bType][2]>>6;
            vRegWriteFldAlign(HDTV_02, bTMP1[0] , HDTV_BLK_CALI_Y_TAR); 
            vRegWriteFldAlign(HDTV_02, bTMP1[1] , HDTV_BLK_CALI_PB_TAR); 
            vRegWriteFldAlign(HDTV_02, bTMP1[2] , HDTV_BLK_CALI_PR_TAR); 
        }
       vRegWriteFldAlign(HDTV_02, bMode  , HDTV_BLK_CALI_ADJ_ON);                      //Enable or disable  HW blank Adjust Function
       vRegWriteFldAlign(HDTV_02, bMode, HDTV_BLK_CALI_EN);                             //Enable line blank average measurement function
       vRegWriteFldAlign(HDTV_00, AFTER_ADJ, HDTV_BLK_STA_SEL);   //blank level after calibration
}
#endif


void vDrvHDTVSelBlankDataSource(void)
{
#ifdef FULLY_HW_AUTO_CALIBRATION  // The blank level is compsesated by H/W auto. However, we have to disable this function when we want to execute manual measurement.
       vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_ADJ_ON);                      //Disable HW blank Adjust Function
       #if HW_MEASURE_BLANK
       vRegWriteFldAlign(HDTV_00, BEFORE_ADJ   , HDTV_BLK_STA_SEL);                                 //blank level after calibration
       #else
       vRegWriteFldAlign(HDTV_00, AFTER_ADJ   , HDTV_BLK_STA_SEL);                                 //blank level after calibration
       #endif
       //vRegWriteFldAlign(HDTV_02, CALI_ENABLE  , HDTV_BLK_CALI_ADJ_ON);                      //Disable HW blank Adjust Function
       //vRegWriteFldAlign(HDTV_00, BEFORE_ADJ   , HDTV_BLK_STA_SEL);                                 //blank level after calibration
       vRegWriteFldAlign(HDTV_02, CALI_ENABLE  , HDTV_BLK_CALI_EN);                             //Enable line blank average measurement function
       
#else
          #if HW_MEASURE_BLANK
                vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_ADJ_ON);                          //disable HW blank Adjust Function
                vRegWriteFldAlign(HDTV_00, BEFORE_ADJ   , HDTV_BLK_STA_SEL); //output line average value
                vRegWriteFldAlign(HDTV_02, CALI_ENABLE  , HDTV_BLK_CALI_EN);                             //Enable line blank average measurement function
          #else
                vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_ADJ_ON);                          //disable HW blank Adjust Function
                vRegWriteFldAlign(HDTV_00, AFTER_ADJ   , HDTV_BLK_STA_SEL);
                vRegWriteFldAlign(HDTV_02, CALI_DISABLE  , HDTV_BLK_CALI_EN);                           //disable line blank average measurement function
          #endif
#endif          
}

UINT8 bDvMeasureDataSel(void)
{
     if ((RegReadFldAlign(HDTV_00,HDTV_BLK_STA_SEL)==BEFORE_ADJ)  && (RegReadFldAlign(HDTV_02,HDTV_BLK_CALI_EN)==CALI_ENABLE))
     {
       return 1;
     }
     else
     {
       return 0;
     }
}

PRIVATE void vDrvOnChipAutoColorGetADCData(UINT8 bMode)
{
 UINT32 dwSumADCRawData;//,dwSumADCRawData2;
 UINT16 wSumADC[3][2];
 //UINT8  bSampleV1V2,bSampleRGBLevel;
 UINT8  bLoopCnt,bSelV1V2;//,bGainOffsetMode;  //dwSumADCRawData2 = u4RegRead4B(VFE_00+(4*bIndexCnt));
 if(_bOrder<3)  //for klocwork check
 {
    switch (bMode)
    {
	case ON_CHIP_GAIN_MODE:
	        //Printf("-----ON CHIP GAIN MODE----------\n");
            vDrvSelectAutoColorBufferOutputChannel(_bOrder);  //select channel buffer of ON CHIP AUTO COLOR 
               //dwSumADCRawData2 = u4RegRead4B(VFE_03);
	        //Printf("Select buffer=%x, register=0x%x, data=0x%x\n",_bOrder,VFE_03,dwSumADCRawData2);
            for (bSelV1V2=0; bSelV1V2<2; bSelV1V2++)
            {
               vDrvSelV1V2(bSelV1V2);             //select V1 or V2 for the input of PGA+ 
               vUtDelay1ms(10);
               wSumADC[_bOrder][bSelV1V2]=0;
               dwSumADCRawData = 0;
               for (bLoopCnt = 0; bLoopCnt<16; bLoopCnt++)
               {
                  dwSumADCRawData += bDrvGetADCData(_bOrder); 
                  vUtDelay2us(100);
               }
               dwSumADCRawData = ((dwSumADCRawData+2)/4); // +8)/16);  //round off // average : gain calibration
               wSumADC[_bOrder][bSelV1V2] = dwSumADCRawData;               
            }
            //Printf("Channel=%x, SUM1=%x, SUM2=%x\n",_bOrder,wSumADC[_bOrder][0],wSumADC[_bOrder][1]);
            _wData_v1v2 [_bOrder]= (wSumADC[_bOrder][0]>wSumADC[_bOrder][1]) ? (wSumADC[_bOrder][0]-wSumADC[_bOrder][1]) : (wSumADC[_bOrder][1]-wSumADC[_bOrder][0]);    
	  break;
          
	case OFFSET_MODE:
            dwSumADCRawData = 0;
            if(bDvMeasureDataSel()==1)
            {
                  dwSumADCRawData = bDrvGetBlankVal(_bOrder);   //to read the blank level of Y, Pb, Pr respective
                  _wData_v1v2[_bOrder] = (dwSumADCRawData)<<3; // 10 bits to 13 bits = 8bit sum 32 times               
            }
            else
            {
                 for(bLoopCnt = 0; bLoopCnt<32; bLoopCnt++)
                 {
                     dwSumADCRawData += bDrvGetBlankVal(_bOrder);   //to read the blank level of Y, Pb, Pr respective
	              vUtDelay2us(50);
                }
               _wData_v1v2[_bOrder] = (dwSumADCRawData+2)>>2; // 10 bits to 8 bits // +8) >> 4; // 12 bits to 8 bits
           }
	    break;

	case GAIN_MODE:
            _wData_v1v2[_bOrder] = bDrvOnChipGetADCMaxMinValue(_bOrder,AS_PHASE_MAX_SEL);
            break;

	default:
	    break;
    }
    //Printf("-- channel %u sample data=%x\n",_bOrder,_wData_v1v2[_bOrder]);
  }
}

#ifdef CC_MT5365_V2
void bDrvSetSmallStep(UINT8 bMode)
{
	if (bMode == GAIN_MODE)
	{
		if (_bCaliStep[_bOrder] > 0x200)
		{
		    _bCaliStep[_bOrder] = 0x200;
		}
	}
	else
	{
		if (_bCaliStep[_bOrder] > 0x04)
		{  
		    _bCaliStep[_bOrder] = 0x04;
		}
	}
}

PRIVATE void vDrvOnChipAutoColorSearchTarget_V2(UINT8 bMode)
{
UINT8 bGainOffsetMode,bTolenence;
#define MAX_TOGGLE_TIME         16
if((_bOrder<3) && (bMode<=DIGITAL_OFFSET_MODE))  //for Kclockwork check
{
       if(bMode==OFFSET_MODE)                               //MC20080417_1 ***
       {
          bTolenence = (_bAutoColorTimeOutCNT>6) ? (0X10) : (0X00);
       }
       else
       {
          bTolenence = (_bAutoColorTimeOutCNT>6) ? (1) : (0);
       }
      	
       _bIncGainDirPre[_bOrder]=_bGainIncDir[_bOrder];
       if((_bChgDirToggle[_bOrder]> MAX_TOGGLE_TIME) || (_bTotalCnt>100)) // if timeout
       {
          _bGain = _bIndex_mim[_bOrder];
          if(bMode==OFFSET_MODE)
          {  
               bDrvSetHDADCOffset(_bOrder,_bGain); 
          }
          else
          {
               bDrvSetHDADCGain_Digital(_bOrder, _bGain);
          }
	    _bGainOffset_channel |= (0x01 << _bOrder);
       }
       else if((_wData_v1v2[_bOrder] <= (_wADCTarget[_bOrder] + bOnChipCalibrateTolerance[bMode][0]+bTolenence)) && (_wData_v1v2[_bOrder] >= ( _wADCTarget[_bOrder] - (bOnChipCalibrateTolerance[bMode][1]+bTolenence))))
       {
          if((_bDoubleChkFlag[_bOrder]==1) || (_bChgDirToggle[_bOrder]>5))
          {
              _bGainOffset_channel |= (0x01 << _bOrder);
          } 
          else
          {
              bDrvSetSmallStep(bMode);
              _bDoubleChkFlag[_bOrder] = 1;           //no change Gain/Offset
          }
       }
       else                                //change Gain/Offset step size
       {
          if((_wData_v1v2[_bOrder] <= (_wADCTarget[_bOrder] + bOnChipCalibrateTolerance[bMode][2])) && (_wData_v1v2[_bOrder] >= (_wADCTarget[_bOrder] - bOnChipCalibrateTolerance[bMode][3])))  //0x0087~0x008B          
          {
              bDrvSetSmallStep(bMode);
          }
          _bDoubleChkFlag[_bOrder] = 0;

          if(_wData_v1v2[_bOrder] < _wADCTarget[_bOrder])
          {
		bGainOffsetMode = 0;  //to increase offset/gain
             if (bMode==OFFSET_MODE)
             {  
			if(_bGain>=0x3f0)         
			{   
			   _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME +1;  //set quit condition
			} 
             }
             else 
		{
		     if (_bGain >= 0xfC00)
		     {
		         _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME + 1;  //set quit condition
		     }
		}
		_wADC_Diff_value[_bOrder] = (_wADCTarget[_bOrder] - _wData_v1v2[_bOrder]);
          }  
          else
          {
             bGainOffsetMode = 1;      //to decrease offset/gain
             if(bMode==OFFSET_MODE)
             {  
	            if(_bGain<=0x10)       
	            {   
	               _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME +1;   //set quit condition
	            }  
             }
             else
		{
			if (_bGain <= 0x400)
			{
			    _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME + 1;  //set quit condition
			}
             }
             _wADC_Diff_value[_bOrder] = (_wData_v1v2[_bOrder] - _wADCTarget[_bOrder] );
          }

          if(_wADC_Diff_value[_bOrder] < _wDiff_mim[_bOrder])
          {
              _wDiff_mim[_bOrder] = _wADC_Diff_value[_bOrder] ;
              _bIndex_mim[_bOrder]= _bGain;
          }

          switch (bGainOffsetMode)
          {
             case 0:     //to increase offset/gain
			if(_bGainIncDir[_bOrder]==0)
			{
				if (bMode == GAIN_MODE)
				{
					if (_bCaliStep[_bOrder] > 0x80)
					{
					    _bCaliStep[_bOrder] = _bCaliStep[_bOrder] >> 1;// calful °£¥H2 
					}
				}
				else
				{
					if (_bCaliStep[_bOrder] > 1)
					{
					    _bCaliStep[_bOrder] = _bCaliStep[_bOrder] >> 1;
					}
				}
			      _bChgDirToggle[_bOrder]++;
			}
			_bGainIncDir[_bOrder] = 1;

			if (bMode == GAIN_MODE)
			{
				if (_bGain < (0xffff - _bCaliStep[_bOrder]))
				{
				    _bGain += _bCaliStep[_bOrder];
				}
				else
				{
				    _bGain +=0x80;
				}
			}
			else
			{
				if (_bGain<(0x3ff-_bCaliStep[_bOrder]))      
				{    
				    _bGain += _bCaliStep[_bOrder];      
				}
				else                            
				{
				    _bGain ++;  
				}
			}
			break;

             case 1:
             /*
             if(bMode == GAIN_MODE)
             {
                 Printf("CH%x need to decrease gain,  _bGainIncDir=%x\n",_bOrder,_bGainIncDir[_bOrder]);
             }
             else
             {
                 Printf("CH%x need to decrease offset, _bGainIncDir=%x\n",_bOrder,_bGainIncDir[_bOrder]);
             }
             */
			if(_bGainIncDir[_bOrder]==1)
			{
				if (bMode == GAIN_MODE)
				{
					if (_bCaliStep[_bOrder] > 0x80)
					{
					    _bCaliStep[_bOrder] = _bCaliStep[_bOrder] >> 1;
					}
				}
				else
				{
					if(_bCaliStep[_bOrder]>1)               
					{  
					    _bCaliStep[_bOrder] = _bCaliStep[_bOrder]>>1;   
					}
				}
				_bChgDirToggle[_bOrder]++;
			}
		      _bGainIncDir[_bOrder] = 0;
                    if (bMode == GAIN_MODE)
                    {
				if (_bGain > _bCaliStep[_bOrder])
				{
				  _bGain -= _bCaliStep[_bOrder];
				}
				else
				{
				  _bGain -=0x80;
				}
                    }
                    else
                    { 
				if(_bGain>_bCaliStep[_bOrder])             
				{  
				    _bGain -= _bCaliStep[_bOrder];         
				}
				else                            
				{
				    _bGain --; 
				}
                    }
                    break;
            }
           /*
		if(bMode == GAIN_MODE)
		{
		    Printf("ch%x new gain=%x\n",_bOrder,_bGain);
		}
		else
		{
		    Printf("ch%x new offset=%x\n",_bOrder,_bGain);
		}
	   */
       }
   }       
}
#else
PRIVATE void vDrvOnChipAutoColorSearchTarget(UINT8 bMode)
{
UINT8 bGainOffsetMode,bTolenence;
#define MAX_TOGGLE_TIME         16
if((_bOrder<3) && (bMode<=DIGITAL_OFFSET_MODE))  //for Kclockwork check
{
       if(bMode==OFFSET_MODE)                               //MC20080417_1 ***
       {
          bTolenence = (_bAutoColorTimeOutCNT>6) ? (0X10) : (0X00);
       }
       else
       {
          bTolenence = (_bAutoColorTimeOutCNT>6) ? (1) : (0);
       }
      	
       _bIncGainDirPre[_bOrder]=_bGainIncDir[_bOrder];
       if((_bChgDirToggle[_bOrder]> MAX_TOGGLE_TIME) || (_bTotalCnt>64)) 
       {
          _bGain = _bIndex_mim[_bOrder];
          if(bMode==OFFSET_MODE)
          {  
               bDrvSetHDADCOffset(_bOrder,_bGain); 
          }
          else
          {
	           bDrvSetHDADCGain(_bOrder,_bGain);  
          }
	    _bGainOffset_channel |= (0x01 << _bOrder);
       }
       else if((_wData_v1v2[_bOrder] <= (_wADCTarget[_bOrder] + bOnChipCalibrateTolerance[bMode][0]+bTolenence)) && (_wData_v1v2[_bOrder] >= ( _wADCTarget[_bOrder] - (bOnChipCalibrateTolerance[bMode][1]+bTolenence))))
       {
          if((_bDoubleChkFlag[_bOrder]==1) || (_bChgDirToggle[_bOrder]>5))
          {
             _bGainOffset_channel |= (0x01 << _bOrder);
          }
          else
          {
			if(_bCaliStep[_bOrder]>0x04)
			{
			     _bCaliStep[_bOrder]=0x04;
			}
			_bDoubleChkFlag[_bOrder] = 1;           //no change Gain/Offset
          }
       }
       else                                //change Gain/Offset step size
       {
          if((_wData_v1v2[_bOrder] <= (_wADCTarget[_bOrder] + bOnChipCalibrateTolerance[bMode][2])) && (_wData_v1v2[_bOrder] >= (_wADCTarget[_bOrder] - bOnChipCalibrateTolerance[bMode][3])))  //0x0087~0x008B          
          {
			if (_bCaliStep[_bOrder] > 0x04)
			{
			    _bCaliStep[_bOrder] = 0x04;
			}    
          }
          _bDoubleChkFlag[_bOrder] = 0;

          if(_wData_v1v2[_bOrder] < _wADCTarget[_bOrder])
          {
             if((bMode==OFFSET_MODE)||(bMode==ON_CHIP_OFFSET_MODE))
             {  
                bGainOffsetMode = 1;
                if(_bGain==0)         
                {   
                   _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME +1;  //set quit condition
                } 
             }
             else
             {  
			bGainOffsetMode = 0;
			if (_bGain == 0xff)
			{
			    _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME + 1;  //set quit condition
			}                    
             }
             _wADC_Diff_value[_bOrder] = (_wADCTarget[_bOrder] - _wData_v1v2[_bOrder]);
          }  
          else
          {
             if((bMode==OFFSET_MODE)||(bMode==ON_CHIP_OFFSET_MODE))
             {  
                bGainOffsetMode = 0; 
                if(_bGain==0xff)       
                {   
                   _bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME +1;   //set quit condition
                }  
             }
             else
             {  
			bGainOffsetMode = 1;
			if(_bGain==0)          
			{   
				_bChgDirToggle[_bOrder] = MAX_TOGGLE_TIME +1;   //set quit condition
			}  
             }
             _wADC_Diff_value[_bOrder] = (_wData_v1v2[_bOrder] - _wADCTarget[_bOrder] );
          }

          if(_wADC_Diff_value[_bOrder] < _wDiff_mim[_bOrder])
          {
              _wDiff_mim[_bOrder] = _wADC_Diff_value[_bOrder] ;
              _bIndex_mim[_bOrder]= _bGain;
          }

          switch (bGainOffsetMode)
          {
             case 0:
			if(_bGainIncDir[_bOrder]==0)
			{
				if(_bCaliStep[_bOrder]>1)               
				{  
				    _bCaliStep[_bOrder] = _bCaliStep[_bOrder]>>1;   
				}
				_bChgDirToggle[_bOrder]++;
			}
			_bGainIncDir[_bOrder] = 1;
			if (_bGain < (0xff - _bCaliStep[_bOrder]))
			{
      			    _bGain += _bCaliStep[_bOrder];
			}
			else
			{
			    _bGain ++;
			} 
                break;

             case 1:
			if(_bGainIncDir[_bOrder]==1)
			{
				if (_bCaliStep[_bOrder] > 1)
				{
					_bCaliStep[_bOrder] = _bCaliStep[_bOrder] >> 1;
				}
				_bChgDirToggle[_bOrder]++;
			}
		       _bGainIncDir[_bOrder] = 0;
                    if (_bGain > _bCaliStep[_bOrder])
                    {
                        _bGain -= _bCaliStep[_bOrder];
                    }
                    else
                    {
                        _bGain --;
                    }
               break;
            }
       }
   }       
}
#endif
UINT8 bDrvOnChipAutoColorCheckSignalReady(void)
{
UINT8 bIntputType,bType;
    bType = (UINT8)P_FA;
   //1. to check input type
    bIntputType   = bGetICInputType(SV_VP_MAIN);        //MC20080417_1 ***
    bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
    if(bType==P_FA)
    {
       bIntputType   = bGetICInputType(SV_VP_PIP);        //MC20080417_1 ***
       bType  = bDrvOnChipGetVFESignalType(bIntputType);  //to get Signal Type
       if(bType==P_FA)
       {
          return (bType);   //no any input
       }
    }
    //2. to check signal ready ?
    if((bType==INT_SCART) && (fgIsCLKLock()))
    {
    	  return (bType);
    }
    else                                                                             //MC20080417_1 &&&
    {
    if (((_IsVgaDetectDone == (BOOL)TRUE) || (_IsHdtvDetectDone == (BOOL)TRUE)) && (fgIsCLKLock()))
    {
          bType  = bDrvOnChipGetVFESignalType(bIntputType);  //Signal is stable and ready
      }
      else
       {
         bType = P_FA;
       }
      return (bType);
     }
}


PRIVATE void vDrvOnChipGetADCMaxValue(void)
{
   _wData_v1v2[0] = bDrvOnChipGetADCMaxMinValue(0,AS_PHASE_MAX_SEL);
   _wData_v1v2[1] = bDrvOnChipGetADCMaxMinValue(1,AS_PHASE_MAX_SEL);
   _wData_v1v2[2] = bDrvOnChipGetADCMaxMinValue(2,AS_PHASE_MAX_SEL);
}




void vDrvBlankAdjParaReset(void)
{
UINT8 bCnt;
        _bGainOffset_channel = 0x00;
        _bAutoColorTimeOutCNT =0;
        for(bCnt=0; bCnt<3; bCnt++)
        {
             _bDoubleChkFlag[bCnt]=0;
             _dwBlankAvg[bCnt] =0;
            _dwMaxLevelAvg[bCnt]=0;
        }
}

void vDrvBlankMeasureParaReset(void)
{
UINT8 bCnt;
      _bAutoColorTimeOutCNT = 0;
      for(bCnt=0; bCnt<3; bCnt++)
      {
           _dwBlankAvg[bCnt]=0;
           _dwMaxLevelAvg[bCnt]=0;
      }
}

void vDrvEnableBlankLevelAdjust(void)
{
    _bBlankIteration = 0;
    _bAdjGainAfterBlank = 0;
//    _bWaitSignalStable = 0;
    _bDoubleCheck = ADJUST_BLANK_RETRY_COUNT;
    vDrvBlankAdjParaReset(); 
    _bAdjBlkState0 = BLK_START;
}

void vDrvEnableBlankLevelMeasure(void)
{
       _bAdjBlkState0 = BLK_MEASURE;
	_bDoubleCheck = 0;   //this flag is used to distinguish the blank level adjust and blank level measurement
       vDrvBlankMeasureParaReset();       
}

void vDrvMeasureMaxBlankLevel(void)
{
    for(_bOrder=0;  _bOrder<3; _bOrder++)
    {	              
       vDrvOnChipAutoColorGetADCData(OFFSET_MODE);
        if(bDvMeasureDataSel()==1)
        {
          _wData_v1v2[_bOrder] = _wData_v1v2[_bOrder]>>3; //10 BIT resoultion
        }
       else
       {
          _wData_v1v2[_bOrder] = (_wData_v1v2[_bOrder]+4)/8; //10 BIT resoultion
       }
       _dwBlankAvg[_bOrder] += _wData_v1v2[_bOrder] ;
   }
   #if AUTO_BLANK_MEASURE
      Printf("CH1=%u,  CH2=%u, CH3=%u\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
   #endif
   vDrvOnChipGetADCMaxValue();
   _dwMaxLevelAvg[0] += _wData_v1v2[0] ;    //8 bit resolution
   _dwMaxLevelAvg[1] += _wData_v1v2[1] ;
   _dwMaxLevelAvg[2] += _wData_v1v2[2] ;
   
    #if AUTO_BLANK_MEASURE
      Printf("MAX 1=%3u, Max2=%3u, Max3=%3u\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
   #endif

}
#if PGA_4bit_offset
         #define ADJUST_TOLRENCE 0X38   // 0x38=10bit 7 code.  can not bigger than 0X40,
#else
         #ifdef FULLY_HW_AUTO_CALIBRATION
         #define ADJUST_TOLRENCE 0X10   //0x10= 10bit 2 code
         #else
         #define ADJUST_TOLRENCE 0X08   //0x08= 10bit 1 code
         #endif
#endif
#define ADJUST_MAX_TIME 40
#define GAIN_DOWN   0
#define GAIN_UP         1
//#define WAIT_STABLE_FIELD_NUMBER   4

#if AUTO_BLANK_ADJUST_DEBUG
void vDrvShowDigGainOffset(UINT8 bOrder)
   {
   #if DIGITAL_NEW_OFFSET    //reset the digital offset
         UINT16 wDgain;
         UINT8 bDOffset;
         wDgain = wReadHDTVDigitalGain(bOrder);
         bDOffset = bReadHDTVDigitalOffset(bOrder);
         _bDigSignBit = vReadSignBit(_bOrder); 
         if(_bDigSignBit==0)
         {
             if(_bAdjBlkState0 == BLK_MEASURE)
             {
                 Printf("After BlkAdj: Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",bOrder,wDgain,(INT8)(0-bDOffset));
             }
             else if(_bAdjBlkState0 == BLK_START)
             {
                 Printf("Before BlkAdj: Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",bOrder,wDgain,(INT8)(0-bDOffset));
             }
             else
            {
                 Printf("Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",bOrder,wDgain,(INT8)(0-bDOffset));
            }
         }
         else
        {
             if(_bAdjBlkState0 == BLK_MEASURE)
          {
                 Printf("After BlkAdj: Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",bOrder,wDgain,bDOffset);
          }
             else if (_bAdjBlkState0 == BLK_START)
          {
                 Printf("Before BlkAdj: Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",bOrder,wDgain,bDOffset);
          }
          else
          {
                 Printf("Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",bOrder,wDgain,bDOffset);
           }                 
        }
            #endif
}                  
            
void vDrvShowAnalogGainOffset(UINT8 bOrder)
{
     #ifdef CC_MT5365_V2
         UINT16 bAOffset;
         //bAgain = bDrvGetHDADCGain_Digital(bOrder);
         bAOffset= bDrvGetHDADCOffset(bOrder);  //V2 OK
         if (_bAdjBlkState0 == BLK_MEASURE)
         {
             Printf("After BlkAdj: analog offset : CH=%u, Aoffset=0x%x\n",bOrder,bAOffset);
         }
         else if (_bAdjBlkState0 == BLK_START)
         {
             Printf("Before BlkAdj: analog offset : CH=%u, Aoffset=0x%x\n",bOrder,bAOffset);
          }
         else
         {
             Printf("analog offset : CH=%u, Aoffset=0x%x\n",bOrder,bAOffset);
        }
         #else
         UINT8 bAgain;
         UINT8 bAOffset;
         bAgain = bDrvGetHDADCGain(bOrder);
         bAOffset= bDrvGetHDADCOffset(bOrder);  //V2 OK

         if (_bAdjBlkState0 == BLK_MEASURE)
         {
             Printf("After BlkAdj: Analog gain & offset : CH=%u, AGain=0x%x, Aoffset=0x%x\n",bOrder,bAgain,bAOffset);
         }
         else if (_bAdjBlkState0 == BLK_START)
         {
             Printf("Before BlkAdj: Analog gain & offset : CH=%u, AGain=0x%x, Aoffset=0x%x\n",bOrder,bAgain,bAOffset);
          }
         else
         {
             Printf("Analog gain & offset : CH=%u, AGain=0x%x, Aoffset=0x%x\n",bOrder,bAgain,bAOffset);
        }
         #endif
        
}                  
#endif


#ifdef FULLY_HW_AUTO_CALIBRATION
UINT8 bDrvEFuseOffsetREADY(void)
{
UINT8 bResult=0;
           if((_bEFUSE_AUTOCOLOR_READY==1) && (_bType<INT_VGA_COMPOENT))
           {
                bResult = (_bOffsetCaliDone[_bType]==0) ? 0 : 1 ;
           }
           else
           {
                bResult = 1;  
           }

       return (bResult);
}
#endif




#if defined (FULLY_HW_AUTO_CALIBRATION) || defined (DIGITAL_NEW_OFFSET)
#define NO_NEED_TO_DO_ANALOG_CALIBRATION  1
#define NEED_TO_DO_ANALOG_CALIBRATION  0
#define BLANKUNIT_10BIT_1CODE     8   //DON'T CHANGE
#if DIGITAL_NEW_OFFSET
     #define BLANK_ADJ_LOW_TOLERENCE          48  //6 code 10 bit
     #define BLANK_ADJ_UP_TOLERENCE              80 //10 code  10 bit   
#else
     #define BLANK_ADJ_LOW_TOLERENCE          16  // 2 code 10 bit, because no digital offset compensation 
     #define BLANK_ADJ_UP_TOLERENCE             16   // 2 code 10 bit, because no digital offset compensation
#endif
UINT8 bDrvCheckAnalogBlankLevel(void)
{
  if(_bOrder<3)//for Kclockwork check
  {
       #ifdef FULLY_HW_AUTO_CALIBRATION
              if(bDrvEFuseOffsetREADY()==SV_FALSE)  // This  is used for  E-FUSE  autocolor function. It will only execute one time for the VGA/YPbPr source is selected
              {
                     if ((_wData_v1v2[_bOrder]<(_wADCTarget[_bOrder]-BLANKUNIT_10BIT_1CODE)) || (_wData_v1v2[_bOrder]> (_wADCTarget[_bOrder]+BLANKUNIT_10BIT_1CODE))) // target +/- 1 code (10bit)
                     { 
                           return (NEED_TO_DO_ANALOG_CALIBRATION); // less than low boundary or bigger than high boundary --> need to do calibration
                     }
                     else
                     {
                           return (NO_NEED_TO_DO_ANALOG_CALIBRATION);
                     }
              }
              else
       #endif
              {
                     if ((_wData_v1v2[_bOrder]<(_wADCTarget[_bOrder]-BLANK_ADJ_LOW_TOLERENCE)) || (_wData_v1v2[_bOrder]> (_wADCTarget[_bOrder]+BLANK_ADJ_UP_TOLERENCE))) // less than 10 bit 4 code or bigger than (target+ 10 bit 4 code)
                     { 
                            return (NEED_TO_DO_ANALOG_CALIBRATION); // less than low boundary or bigger than high boundary --> need to do calibration
                     }
                     else
                     {
                            return (NO_NEED_TO_DO_ANALOG_CALIBRATION);
                     }
              }
  }
  return (NO_NEED_TO_DO_ANALOG_CALIBRATION);
}
#endif

#define INCREASE_OFFSET     1
#define DECREASE_OFFSET    0
void vDrvAnalogOffsetAdjBlk(void)
{
#ifdef CC_MT5365_V2
   UINT16 bTempOffset[3];
   UINT8 bAdjOffset;
#else
   UINT8 bTempOffset[3];
   UINT8 bAdjOffset;
#endif   
#if AUTO_BLANK_ADJUST_DEBUG
UINT8 bDigital_gain;
#endif

UINT8 bCalibration_direction;

 if(_bOrder<3)  //for klocwork check
 {
    bTempOffset[_bOrder]= bDrvGetHDADCOffset(_bOrder);
    vDrvOnChipAutoColorGetADCData(OFFSET_MODE);
    vDrvOnChipAutoColorModeSet(OFFSET_MODE,_bType);
    #if AUTO_BLANK_ADJUST_DEBUG
        switch (bDigital_gain)
        {
           case 0:
               bDigital_gain = (RegReadFldAlign(HDFE_02, AD1_OFFSET));
               break;
           case 1:
               bDigital_gain = (RegReadFldAlign(HDFE_02, AD2_OFFSET));
               break;
           case 2:
           default:
               bDigital_gain = (RegReadFldAlign(HDFE_00, AD3_OFFSET));
               break;
        }
      Printf("AnalogAdjBlk : CH %u, target=0x%x, current=0x%x, Digital offset=0x%x, Analog offset=0x%x\n",_bOrder, _wADCTarget[_bOrder],_wData_v1v2[_bOrder],bDigital_gain,bTempOffset[_bOrder]);
    #endif
    
      if(_wData_v1v2[_bOrder]>(_wADCTarget[_bOrder]+_bDiffOffset)) // 0X08=10 bit 1 code
      {
            bAdjOffset = (UINT8)((_wData_v1v2[_bOrder]-_wADCTarget[_bOrder])>>3);  //change to 10bit base
	      #ifdef CC_MT5365_V2
	      bCalibration_direction = DECREASE_OFFSET;
            //Printf("=== Current BLANK LEVEL is higher than Target level : difference is 0x%x===\n",bAdjOffset);
	      #else
	      bCalibration_direction = INCREASE_OFFSET;
	      #endif
      }
      else
      {
            bAdjOffset = (UINT8)((_wADCTarget[_bOrder]-_wData_v1v2[_bOrder])>>3);
            //Printf("=== Current BLANK LEVEL is smaller than Target level : difference is 0x%x===\n",bAdjOffset);
	      #ifdef CC_MT5365_V2
	      bCalibration_direction = INCREASE_OFFSET;
	      #else
	      bCalibration_direction = DECREASE_OFFSET;
	      #endif
      }
    
      if(bCalibration_direction == INCREASE_OFFSET) // 0X08=10 bit 1 code
      {
         _bDoubleChkFlag[_bOrder]=0;
         if(bTempOffset[_bOrder]< MAX_OFFSET)    //(_bInitOffset+MAX_ALLOWED_OFFSET_RANGE))
         {
            //bAdjOffset = (UINT8)((_wData_v1v2[_bOrder]-_wADCTarget[_bOrder])>>3);  //change to 10bit base
            //Printf("+ _bDiffOffset=%x\n",bAdjOffset);
                 if(bTempOffset[_bOrder] < (MAX_OFFSET-bAdjOffset))
                 {         
                     if(bAdjOffset<3)
                     {
                        bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]+1);
                     }
                     else
                     {
                        bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]+(bAdjOffset/2));
                     }
                 }
                 else
                 {
                        bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]+1);
                 }
         }
         else
         {
            _bGainOffset_channel |= (0x01 << _bOrder);  //compensate limit
         }
      }
      else if(bCalibration_direction == DECREASE_OFFSET)
      {
         _bDoubleChkFlag[_bOrder]=0;
         if(bTempOffset[_bOrder]> 0) //(_bInitOffset-MAX_ALLOWED_OFFSET_RANGE))
         {
            //bAdjOffset = (UINT8)((_wADCTarget[_bOrder]-_wData_v1v2[_bOrder])>>3);
            //Printf("- _bDiffOffset=%x\n",bAdjOffset);
                if(bTempOffset[_bOrder]>(1+bAdjOffset/2))
                {
                      if(bAdjOffset<3)
                      {
                           bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]-1);
                      }
                      else 
                      {
                           bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]-(1+bAdjOffset/2));
                      }
                }
                else
                {
                    bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]-1);
                }
         }
         else
         {
            _bGainOffset_channel |= (0x01 << _bOrder);  //compensate limit
         }
      }
      else
      {
         if(_bDoubleChkFlag[_bOrder]==1)
         {
            if(_wData_v1v2[_bOrder]>(_wADCTarget[_bOrder]+8))
            {
               bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]+1);
            }
            else if(_wData_v1v2[_bOrder]<(_wADCTarget[_bOrder]-8))
            {
               bDrvSetHDADCOffset(_bOrder,bTempOffset[_bOrder]-1);
            }
            _bGainOffset_channel |= (0x01 << _bOrder);
            #ifdef FULLY_HW_AUTO_CALIBRATION
                 Printf("After vDrvAnalogOffsetAdjBlk, Blank leve is : CH=%u,  blank level=%u\n", _bOrder,(_wData_v1v2[_bOrder]>>3));
            #endif
         }
         else
         {
            _bDoubleChkFlag[_bOrder]=1;
         }
      }
   }
} 

void vDrvDigitalOffsetAdjBlk(void)
{
#if DIGITAL_NEW_OFFSET
UINT8 bCmpResult,bAdjOffset;

#ifdef CC_MT5365_V2
UINT8 bTmpOffset;
#endif
      vDrvOnChipAutoColorGetADCData(OFFSET_MODE);
      vDrvOnChipAutoColorModeSet(DIGITAL_OFFSET_MODE,_bType);     //_bInitOffset is 10 bit base
      bTmpOffset = _bInitOffset&0xff;
      //Printf("  vDrvDigitalOffsetAdjBlk, CH=%u, Current blank level=0x%x, Current digital offset=0x%x\n",_bOrder,(_wData_v1v2[_bOrder]>>3),bTmpOffset);
if(_bOrder<3)  //only for Klocwork check
{
    if (bDrvCheckAnalogBlankLevel() == NEED_TO_DO_ANALOG_CALIBRATION)   //((_wData_v1v2[_bOrder]>>3)<4) || ((_wData_v1v2[_bOrder]>>3)> ((_wADCTarget[_bOrder]>>3)+16)))
    {
        Printf("== NEED_TO_DO_ANALOG_CALIBRATION===\n");
        vDrvAnalogOffsetAdjBlk();
    }
    else
    {     
        Printf("== NO_NEED_TO_DO_ANALOG_CALIBRATION, Use digital offset to adjust blank level===\n");
      #if AUTO_BLANK_ADJUST_DEBUG
      Printf("vDrvDigitalOffsetAdjBlk : CH %u, target=0x%x, current=0x%x, Digital offset=0x%x, Analog offset=0x%x\n",_bOrder,_wADCTarget[_bOrder],_wData_v1v2[_bOrder],bReadHDTVDigitalOffset(_bOrder),bDrvGetHDADCOffset(_bOrder));
      #endif       
       if(_wData_v1v2[_bOrder]>(_wADCTarget[_bOrder]+0x10)) // 0X10=10 bit 2 code
       {
           _bDoubleChkFlag[_bOrder]=0;
           bCmpResult =1;
           bAdjOffset = (_wData_v1v2[_bOrder]-_wADCTarget[_bOrder])>>3; //transfer to 10 bit base
           bTmpOffset =vCalculateDigOffset(bTmpOffset,bAdjOffset,bCmpResult);
           if(bTmpOffset==0xff)
           {
               _bGainOffset_channel |= (0x01 << _bOrder);  //compensate limit
           }
           else
           {
               vDrvSetHDADCDigitalOffset(_bOrder,bTmpOffset);                                  
           }
       }
       else if(_wData_v1v2[_bOrder]<(_wADCTarget[_bOrder]-0x10))
       {
            _bDoubleChkFlag[_bOrder]=0; //_wADCTarget
            bCmpResult =0;
            bAdjOffset = (_wADCTarget[_bOrder]-_wData_v1v2[_bOrder])>>3; //transfer to 10 bit base
            bTmpOffset =vCalculateDigOffset(bTmpOffset,bAdjOffset,bCmpResult);
            if(bTmpOffset==0xff)
	    {
                _bGainOffset_channel |= (0x01 << _bOrder);  //compensate limit
            }
            else
            {
                vDrvSetHDADCDigitalOffset(_bOrder,bTmpOffset);  
            }
       }
       else
       {
            if(_bDoubleChkFlag[_bOrder]==1)
            {
                if(_wData_v1v2[_bOrder]>=(_wADCTarget[_bOrder]+6))
                {
                    bCmpResult =1;
                    bAdjOffset = 1;
                    bTmpOffset =vCalculateDigOffset(bTmpOffset,bAdjOffset,bCmpResult);
                }
                else if(_wData_v1v2[_bOrder]<=(_wADCTarget[_bOrder]-6))
                {
                    bCmpResult =0;
                    bAdjOffset = 1;
                    bTmpOffset =vCalculateDigOffset(bTmpOffset,bAdjOffset,bCmpResult);
                }
                else
                {
                    bAdjOffset = 0;
                }
                vDrvSetHDADCDigitalOffset(_bOrder,bTmpOffset);  
                _bGainOffset_channel |= (0x01 << _bOrder);
            }
            else
            {
                _bDoubleChkFlag[_bOrder]=1;
            }
       }
   }
 }
#endif       
}


void vDrvResetAllGainOffset(void)
{
     if(_bType<=INT_VGA_COMPOENT)   //for klocwork check
     {
              for(_bOrder=0;  _bOrder<3; _bOrder++)
              {
                    vDrvOnChipAutoColorModeSet(GAIN_MODE,_bType);
                    vDrvOnChipAutoColorModeSet(OFFSET_MODE,_bType);               
                    bDrvSetHDADCOffset(_bOrder, _bInitOffset);   //reset the offset value to the value stored in EEPROM
                 #if DIGITAL_NEW_OFFSET    //reset the digital offset
                    _bDigSignBit = 1;
                    vDrvSetHDADCDigitalOffset(_bOrder,0);
                 #endif 
              
                 #if AUTO_BLANK_ADJUST_DEBUG
                    vDrvShowDigGainOffset(_bOrder);
                    vDrvShowAnalogGainOffset(_bOrder);
                 #endif
              }
       #ifdef CC_MT5365_V2
       #else
              vRegWriteFldAlign(HDFE_01, 0x8000, AD1_GAIN);  // reset the Digital gain to uint gain
              vRegWriteFldAlign(HDFE_01, 0x8000, AD2_GAIN);	
              vRegWriteFldAlign(HDFE_02, 0x8000, AD3_GAIN);
       #endif 
       }
}       

void vPrint_Input_Type(void)
{
            switch (_bType)
            {
                case INT_HDTV:
                    Printf("Input sourec is YPbPr\n");
                    break;
                case INT_VGA:
                    Printf("Input sourec is VGA\n");
                    break;
                case INT_SCART:
                    Printf("Input sourec is SCART\n");
                    break;
                 default:
                    Printf("Input sourec is Other\n");
                    break;
            }
}


void vDrvCalculateMaxBLKAvg(void)
{
                for(_bOrder=0; _bOrder<3; _bOrder++)
               {
                    _dwMaxLevelAvg[_bOrder] = (_dwMaxLevelAvg[_bOrder]+(_bAutoColorTimeOutCNT/2) )/_bAutoColorTimeOutCNT;
                    _dwBlankAvg[_bOrder] = (_dwBlankAvg[_bOrder]+(_bAutoColorTimeOutCNT/2) )/_bAutoColorTimeOutCNT;
               }
}

void vDrvAdjustBlankLevel(void)
{
static UINT8 _bAdjBlkState0_flow=0;
#if DIGITAL_NEW_OFFSET
#else
UINT8 bTempOffset[3];
#endif
UINT8 bDeltaGain=0;
UINT8 bGainUpDown=0;
//UINT8 bAdjOffset;

//#ifdef FULLY_HW_AUTO_CALIBRATION
    //#if E_FUSE_DO_OFFSET_CALIBRATION
          //UINT16 wTmpwVgaClock,wTmpwPixelClk;
    //#endif
//#endif

   if((! fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG)) || (!(_bAutoColorState0==VDO_AUTO_COLOR_NOT_BEGIN)))
   {
          return;
   }

   if(_bAdjBlkState0==BLK_NOTHING)
   {
            return;
   }

   _bType = bDrvOnChipAutoColorCheckSignalReady();
   if((_bType>INT_VGA_COMPOENT)  || (_bType ==P_FA))   //for klocwork check
   {
       return;
   }
   else
   {
       if (_bAdjBlkState0==BLK_START)
       {
            _bOrgType = _bType;
            vPrint_Input_Type();
       }
       else
       {
           if (_bType!=_bOrgType) //type is changed and to restart the flow
           {
               Printf("---Input type is changed. to restart the adjustment flow ----\n");
               vDrvEnableBlankLevelAdjust();
               return;
           }
       }  
   }

#ifdef CC_MT5365_V2
    _bAdjBlkState0 = BLK_NOTHING;
#endif


    if(_bAdjBlkState0!=_bAdjBlkState0_flow)
    {
        Printf("=== state of adjust blank =%x\n",_bAdjBlkState0);
        _bAdjBlkState0_flow = _bAdjBlkState0;
    }
    switch (_bAdjBlkState0)
    {  
        case BLK_NOTHING:
           vClrSP0Flg(SP0_AUTOCOLOR_FLG);
           break;
    
        case BLK_START:
           //_bType = bDrvOnChipAutoColorCheckSignalReady();
           if((_bType<=INT_VGA_COMPOENT) &&(_bType !=P_FA))   //for klocwork check
           {
           #ifdef FULLY_HW_AUTO_CALIBRATION
              vDrvResetAllGainOffset();
              vDrvHDTVSelBlankDataSource();
              _bAdjGainAfterBlank=0;
              vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
              if(_bEFUSE_AUTOCOLOR_READY==1)
              {
                 _bAdjBlkState0 = BLK_LEVEL_STABLE;  //always do blank level adjust check
              }
              else
              {
                 if(_bAutoColorHistory[_bType] !=SV_TRUE)
                 {
                      vDrvHDTVMeasureSetting(UPDATE_PER_1_FIELD);
                      vDrvHDTV_HW_AUTO_ONOFF(CALI_ENABLE,_bType);
                      _bAdjBlkState0= BLK_NOTHING;    //no need to jump to _bAdjBlkState0= BLK_CLAMP_DELAY;
                      vDrvBlankAdjParaReset();
                 }
                 else
                 {
                      _bAdjBlkState0 = BLK_LEVEL_STABLE;
                 }
              }
         #else
              if(_bAutoColorHistory[_bType] !=SV_TRUE) // if auto color NOT done then no need to do calibration
              {
                      _bAdjBlkState0 = BLK_NOTHING;
              }
              else
              {
                      vDrvResetAllGainOffset();
                      vDrvHDTVSelBlankDataSource();
                      _bAdjBlkState0= BLK_LEVEL_STABLE;         
                      _bAdjGainAfterBlank=0;
                      vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
              }
          #endif          
           }
           vClrSP0Flg(SP0_AUTOCOLOR_FLG);
           break;

        case BLK_LEVEL_STABLE:
           vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
           if(_bAutoColorTimeOutCNT<4)
           {
             _bAutoColorTimeOutCNT++;
               vDrvMeasureMaxBlankLevel();
           }
           else
           {
               vDrvCalculateMaxBLKAvg();   
               if ((_bType == INT_VGA) && ((_dwBlankAvg[0]>128) || (_dwBlankAvg[1]>128) || (_dwBlankAvg[2]>128))) //8 bit 32 code
               {
                    _bAdjBlkState0= BLK_NOTHING;    
               }
               else
               {
                      #ifdef FULLY_HW_AUTO_CALIBRATION
                          if(bDrvEFuseOffsetREADY()==SV_FALSE)  // if never do offset then do offset adjust
                          {
                             Printf("Pre-check, Blank level: CH1=%3u,  CH2=%3u, CH3=%3u\n",_dwBlankAvg[0],_dwBlankAvg[1],_dwBlankAvg[2]);
                             _bAdjBlkState0 =BLK_OFFSET_ADJ;
                          }
                          else
                      #endif
                          {
                             _bAdjBlkState0 =BLK_OFFSET_CHECK;
                          }
                      vDrvBlankAdjParaReset();                                                
               }
           }
           vClrSP0Flg(SP0_AUTOCOLOR_FLG);
           break;
        
        case BLK_OFFSET_ADJ:
              if((_bType<=INT_VGA_COMPOENT) &&(_bType !=P_FA))   //for klocwork check
              {
                    if( (_bAutoColorTimeOutCNT<ADJUST_MAX_TIME)&& ((_bGainOffset_channel&0x07) != 0x07))
	             {
                        _bAutoColorTimeOutCNT ++;
                        _bDiffOffset = (_bAutoColorTimeOutCNT>(ADJUST_MAX_TIME/2)) ? (ADJUST_TOLRENCE+0x08) : (ADJUST_TOLRENCE) ; //0x08 is 10 bit 1 code
                        
                        for(_bOrder=0;  _bOrder<3; _bOrder++)
                        {                        
                           if(!(_bGainOffset_channel & (0x01<<_bOrder)))
                           {
                           #if DIGITAL_NEW_OFFSET
                               vDrvDigitalOffsetAdjBlk();
                           #else
                               vDrvAnalogOffsetAdjBlk();
                           #endif
                           }
                        }
                        _bAdjBlkState0= BLK_OFFSET_DELAY;  // [SA7_Michael] 080828 for adjusting blanking level stable
                     }
                     else  
                     {
                        #ifdef FULLY_HW_AUTO_CALIBRATION
                            if(bDrvEFuseOffsetREADY()==SV_FALSE)  // if never do offset then set the flag and store the offset
                            {
                                 _bOffsetCaliDone[_bType]= 1;
                                 for (_bOrder=0; _bOrder<3; _bOrder++)
                                 {
                                     _bAutoColorOffset[_bType][_bOrder] = bDrvGetHDADCOffset(_bOrder);
                                 }
                                 //vVgaSetInputCapature(_bVgaTiming);
                                 vDrvEnableBlankLevelAdjust();
                            }
                            else
                            {
                                 Printf(" Offset_calibration spends %u times\n",(_bAutoColorTimeOutCNT-1));
                                 vDrvHDTVMeasureSetting(UPDATE_PER_1_FIELD);
                                 vDrvHDTV_HW_AUTO_ONOFF(CALI_ENABLE,_bType);
                                 _bAdjBlkState0= BLK_NOTHING;    //no need to jump to _bAdjBlkState0= BLK_CLAMP_DELAY;
                                 //_bAdjBlkState0= BLK_MEASURE ; //for test
                                 vDrvBlankAdjParaReset();
                            }
                        #else
                            _bAdjBlkState0= BLK_OFFSET_CHECK;
                            vDrvBlankAdjParaReset();
                        #endif
                        if(_bAdjBlkState0==BLK_NOTHING)
                        {
                           _bAdjBlkState0 = BLK_APICMD;
                        }
                     }
             }                     
             vClrSP0Flg(SP0_AUTOCOLOR_FLG);
             break;

	    case BLK_APICMD:
             _dwBlankAvg_1[0]=bDrvGetBlankVal(0); //kal for sony
             _dwBlankAvg_1[1]=bDrvGetBlankVal(1);
             _dwBlankAvg_1[2]=bDrvGetBlankVal(2);
             _dwMaxLevelAvg_1[0]=RegReadFldAlign(STA_SYNC0_07, AS_RMAXMIND);
             _dwMaxLevelAvg_1[1]=RegReadFldAlign(STA_SYNC0_08, AS_GMAXMIND);
             _dwMaxLevelAvg_1[2]=RegReadFldAlign(STA_SYNC0_09, AS_BMAXMIND);
             //Printf("API read blank %4d %4d %4d\n", _dwBlankAvg_1[0], _dwBlankAvg_1[1], _dwBlankAvg_1[2]);			 vDrvBlankAdjParaReset();
		_bAdjBlkState0= BLK_NOTHING; 
		break;

        case BLK_OFFSET_DELAY:  // [SA7_Michael] 080828 for adjusting blanking level stable
             vClrSP0Flg(SP0_AUTOCOLOR_FLG);
             _bAdjBlkState0 = (bDvMeasureDataSel()==1) ?  BLK_OFFSET_DELAY2 :  BLK_OFFSET_ADJ ;
             break;

        case BLK_OFFSET_DELAY2:  
             vClrSP0Flg(SP0_AUTOCOLOR_FLG);
             _bAdjBlkState0= BLK_OFFSET_ADJ;
             break;
             
        case BLK_OFFSET_CHECK:
           vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
           if(_bAutoColorTimeOutCNT<4)
           {
                _bAutoColorTimeOutCNT++;
                vDrvMeasureMaxBlankLevel();
           }
           else
           {   
               vDrvCalculateMaxBLKAvg();
               //Printf(" Before check the _bGainOffset_channel=%u\n",_bGainOffset_channel);
               for(_bOrder=0; _bOrder<3; _bOrder++)
               {
                   #if COMPENSATE_GAIN
                    if ((_bBlankIteration==0) && (_bType==INT_VGA))
                    {
                        if((_dwBlankAvg[_bOrder]>((wColorBlankValueNew[_bType][_bOrder]>>3)-3)) && (_dwBlankAvg[_bOrder]<((wColorBlankValueNew[_bType][_bOrder]>>3)+3)))
                        {
                             _wBlanklevel_before_adj[_bOrder] = (wColorBlankValueNew[_bType][_bOrder]>>3); //means no modify gain
                        }
                        else
                        {
                             if (_dwBlankAvg[_bOrder]> (wColorBlankValueNew[_bType][_bOrder]>>3))
                             {
                                 _wBlanklevel_before_adj[_bOrder] = (_dwBlankAvg[_bOrder]<=((wColorBlankValueNew[_bType][_bOrder]>>3)+8)) ? (_dwBlankAvg[_bOrder] -2) : ((wColorBlankValueNew[_bType][_bOrder]>>3)+6);
                             }
                             else
                             {
                                  _wBlanklevel_before_adj[_bOrder] = _dwBlankAvg[_bOrder] +2;
                             }
                        }
                    }
                    #endif
                    
                    if(!(_bGainOffset_channel & (0x01<<_bOrder)))
                    {
                    #ifdef FULLY_HW_AUTO_CALIBRATION
                        _wData_v1v2[_bOrder]= _dwBlankAvg[_bOrder] <<3; //change to 13 bit base for bDrvCheckAnalogBlankLevel()
                        _wADCTarget[_bOrder] = wColorBlankValueNew[_bType][_bOrder];
                        if(bDrvCheckAnalogBlankLevel()== NO_NEED_TO_DO_ANALOG_CALIBRATION)
                    #else
                         if((_dwBlankAvg[_bOrder] <= ((wColorBlankValueNew[_bType][_bOrder]+ADJUST_TOLRENCE)>>3)) && (_dwBlankAvg[_bOrder] >= ((wColorBlankValueNew[_bType][_bOrder]-ADJUST_TOLRENCE)>>3)))  // 10 bit +/- 4 code               
                    #endif
                         {
                             _bGainOffset_channel |= (0x01 << _bOrder);
                         }
                    }
               }
               if (_bBlankIteration==0)
               {
                  Printf("     \n");
                  Printf("before Blank adjust, Blank level: CH1=%3u,  CH2=%3u, CH3=%3u\n",_dwBlankAvg[0],_dwBlankAvg[1],_dwBlankAvg[2]);
                  Printf("before Blank adjust, Maximum level: CH1=%3u,  CH2=%3u, CH3=%3u\n",_dwMaxLevelAvg[0],_dwMaxLevelAvg[1],_dwMaxLevelAvg[2]);
               }
               else
               {
               #if  AUTO_BLANK_ADJUST_DEBUG
                  Printf("After Blank adjust, Blank leve is : CH1=%u,  CH2=%u, CH3=%u\n",_dwBlankAvg[0],_dwBlankAvg[1],_dwBlankAvg[2]);
                  Printf("_bBlankIteration=%x,Blank adjust tolerance is %x, 0x08 means 1 code(10bit)\n",_bBlankIteration,_bDiffOffset);
               #endif
               }
              _bBlankIteration++;
              
              if(((_bGainOffset_channel & 0x07) != 0x07) && (_bBlankIteration<3))
              {
                   _bAdjBlkState0 = BLK_OFFSET_ADJ;
                   Printf("--- NEED_TO_DO_ANALOG_CALIBRATION ---\n");                  
              }
              else
              {
              #ifdef FULLY_HW_AUTO_CALIBRATION
                   Printf("--- NO_NEED_TO_DO_ANALOG_CALIBRATION ---\n");
                   vDrvHDTVMeasureSetting(UPDATE_PER_1_FIELD);
                   vDrvHDTV_HW_AUTO_ONOFF(CALI_ENABLE,_bType);
	             _bAdjBlkState0= BLK_NOTHING;    //no need to jump to _bAdjBlkState0= BLK_CLAMP_DELAY;
		      //_bAdjBlkState0= BLK_MEASURE ; //for test
                   _bAdjBlkState0= BLK_APICMD;
                   vDrvBlankAdjParaReset();
              #else
                    #if  DIGITAL_NEW_OFFSET
                          _bAdjBlkState0= BLK_DELAY;
                          vDrvBlankAdjParaReset();
                          #if AUTO_BLANK_ADJUST_DEBUG
                              UINT16 wDgain;
                              UINT8 bDOffset;
                              for(_bOrder=0; _bOrder<3; _bOrder++)
                              {
                                   wDgain = wReadHDTVDigitalGain(_bOrder);
                                   bDOffset = bReadHDTVDigitalOffset(_bOrder);
                                   _bDigSignBit = vReadSignBit(_bOrder); 
                                   if(_bDigSignBit==0)
                                   {
                                       Printf("Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",_bOrder,wDgain,(INT8)(0-bDOffset));
                                   }
                                   else
                                   {
                                       Printf("Digital gain & offset: CH=%u, DGain=0x%x, Doffset=%d\n",_bOrder,wDgain,bDOffset);
                                   }                   
                               }
                           #endif
                    #else
                          _bAdjBlkState0= BLK_DELAY;
                          vDrvBlankAdjParaReset();
                   #endif
              #endif
              }
            }
             vClrSP0Flg(SP0_AUTOCOLOR_FLG);
             break;


        case BLK_GAIN_ADJ:
           if(_bAutoColorTimeOutCNT<4)
           {
                _bAutoColorTimeOutCNT++;
                vDrvMeasureMaxBlankLevel();
           }
           else
           {
                vDrvCalculateMaxBLKAvg();
                #if AUTO_BLANK_ADJUST_DEBUG
                Printf("before digital gain ajust Blank level (10 bit) :CH1=%3u, CH2=%3u, CH3=%3u\n",_dwBlankAvg[0],_dwBlankAvg[1],_dwBlankAvg[2]);
                Printf("before digital gain adjust, Maximum level : Max1=%3u, Max2=%3u, Max3=%3u\n",_dwMaxLevelAvg[0],_dwMaxLevelAvg[1],_dwMaxLevelAvg[2]);
                #endif
                  for(_bOrder=0;  _bOrder<3; _bOrder++)
                  {                  
                      if((_dwMaxLevelAvg[_bOrder] <= ((UINT32)bDrvGetColorTargetValue(_bType,_bOrder)))   //+1)) 
                      && (_dwMaxLevelAvg[_bOrder] >= ((UINT32)bDrvGetColorTargetValue(_bType,_bOrder))))  //-1)))
                    {
                           bDeltaGain = 0;
                           bGainUpDown = 0;
                    }
                    else
                    {
                      #if DIGITAL_NEW_OFFSET
                           vDrvOnChipAutoColorModeSet(DIGITAL_OFFSET_MODE,_bType); 
                           bGainUpDown = (_bDigSignBit>0) ? GAIN_DOWN  : GAIN_UP ;
                           bDeltaGain = _bInitOffset;
                      #else
                      vDrvOnChipAutoColorModeSet(OFFSET_MODE,_bType);       
                    if(_bOrder<3)  //for klocwork check
                    {
                      bTempOffset [_bOrder]= bDrvGetHDADCOffset(_bOrder);
                      bDeltaGain = 0;
                      if(bTempOffset [_bOrder]> _bInitOffset)    
                      {
                          bDeltaGain= bTempOffset [_bOrder] - _bInitOffset;
                          bGainUpDown=GAIN_UP;                                       // to enlarge level    
                      }
                      else
                      {
                          bDeltaGain = _bInitOffset -bTempOffset [_bOrder] ;
                          bGainUpDown=GAIN_DOWN;                                       // to reduce level
                      }
                    }
                      #endif                      
                           if (bDeltaGain==0)
                           {
                               if(_dwMaxLevelAvg[_bOrder] > ((UINT32)bDrvGetColorTargetValue(_bType,_bOrder))) //+1))
                               {
                                   bGainUpDown = GAIN_DOWN;
                                   bDeltaGain = 5;
                               }
                           }
                           else 
                           {
                               if (bGainUpDown==GAIN_UP) //to do gain up
                               {
                                     if(_dwMaxLevelAvg[_bOrder] >= ((UINT32)bDrvGetColorTargetValue(_bType,_bOrder)))  //+1))
                                     {
                                         bDeltaGain = 0;
                                         bGainUpDown = 0;
                                     }
                               }
                               else
                               {
                                     if(_dwMaxLevelAvg[_bOrder] <= ((UINT32)bDrvGetColorTargetValue(_bType,_bOrder)))  //-1))
                                     {
                                          bDeltaGain = 0;
                                          bGainUpDown = 0;
                                     }
                               }
                           }    
                           if (bDeltaGain>0)
                           {
                              vDrvSetDigitalGain(bGainUpDown, bDeltaGain);
                           }
                       }                      
                  #if AUTO_BLANK_ADJUST_DEBUG
                      Printf("--bGainUpDown =%u,  Gain delta=%u \n",bGainUpDown, bDeltaGain);
                  #endif
                  }
                  _bAdjBlkState0= BLK_MEASURE;
                  vDrvBlankMeasureParaReset();
            }
            vClrSP0Flg(SP0_AUTOCOLOR_FLG);
            break;

        case BLK_DELAY:
             vClrSP0Flg(SP0_AUTOCOLOR_FLG);
             _bAdjBlkState0= BLK_GAIN_ADJ;
             break;
   
        case BLK_MEASURE:
               vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
               if(_bAutoColorTimeOutCNT<8)
               {
                    _bAutoColorTimeOutCNT++;
                    vDrvMeasureMaxBlankLevel();
               }
               else
               {
                    _bGainOffset_channel = 0;
                    for(_bOrder=0; _bOrder<3; _bOrder++)
                    {
                       _dwBlankAvg[_bOrder] = (_dwBlankAvg[_bOrder]+(_bAutoColorTimeOutCNT/2) )/_bAutoColorTimeOutCNT;
                       _dwMaxLevelAvg[_bOrder] = (_dwMaxLevelAvg[_bOrder]+(_bAutoColorTimeOutCNT/2) )/_bAutoColorTimeOutCNT;
                       //#if AUTO_BLANK_ADJUST_DEBUG
                       //Printf("CH=%u,Offset=%x\n",_bOrder,bDrvGetHDADCOffset(_bOrder));
                       //#endif
                       if (_bType<=INT_VGA_COMPOENT) //for klocwork check  
                       {
                           if(_bDoubleCheck > 0)
                           {
                               if((_dwBlankAvg[_bOrder] <= ((wColorBlankValueNew[_bType][_bOrder]>>3)+1)) && (_dwBlankAvg[_bOrder] >= ((wColorBlankValueNew[_bType][_bOrder]>>3)-1)))  // 10 Bit resolution
                               {
                                  _bGainOffset_channel |= (0x01 << _bOrder);
                               }
                           }
                       }
                    }

               #if AUTO_BLANK_ADJUST_DEBUG
               Printf("After digital gain ajust Blank level (10 bit) :CH1=%3u, CH2=%3u, CH3=%3u\n",_dwBlankAvg[0],_dwBlankAvg[1],_dwBlankAvg[2]);
               Printf("After digital gain adjust, Maximum level : Max1=%3u, Max2=%3u, Max3=%3u\n",_dwMaxLevelAvg[0],_dwMaxLevelAvg[1],_dwMaxLevelAvg[2]);
               #endif
                   if(((_bGainOffset_channel & 0x07) != 0x07)  && (_bDoubleCheck>0))
                   {
                      _bAdjBlkState0= BLK_OFFSET_ADJ;
                      #if AUTO_BLANK_ADJUST_DEBUG
                      Printf("go back to OFFSET adjust\n");
                      #endif
                      vDrvBlankAdjParaReset();
                      _bBlankIteration = 1;
                      _bDoubleCheck = _bDoubleCheck-1;
                   }
                   else
                   {
                     Printf("Blank level adjust %u times\n",(ADJUST_BLANK_RETRY_COUNT-_bDoubleCheck+1));
                     Printf("After vDrvAdjustBlankLevel, Blank level (10 bit) :CH1=%3u, CH2=%3u, CH3=%3u\n",_dwBlankAvg[0],_dwBlankAvg[1],_dwBlankAvg[2]);
                     Printf("After vDrvAdjustBlankLevel, Maximum level (8 bit) :CH1=%u, CH2=%u, CH3=%u\n",_dwMaxLevelAvg[0],_dwMaxLevelAvg[1],_dwMaxLevelAvg[2]);
                    #if  AUTO_BLANK_ADJUST_DEBUG
                          _bType = bDrvOnChipAutoColorCheckSignalReady();
                          if(_bType<=INT_VGA_COMPOENT)
                          {
                               Printf("Target blank level (10 bit):CH1=%3u, CH2=%3u, CH3=%3u\n",(wColorBlankValueNew[_bType][0]>>3),(wColorBlankValueNew[_bType][1]>>3),(wColorBlankValueNew[_bType][2]>>3));
                               Printf("Target Maximum (8 bit):CH1=%4u, CH2=%4u, CH3=%4u\n",bDrvGetColorTargetValue(_bType,0),bDrvGetColorTargetValue(_bType,1),bDrvGetColorTargetValue(_bType,2));
                          }
                          for (_bOrder=0; _bOrder<3; _bOrder++)
                          {                    
                               vDrvShowDigGainOffset(_bOrder);
                               vDrvShowAnalogGainOffset(_bOrder);
                          }
                    #endif
                        _bAdjBlkState0= BLK_NOTHING;
                  }
               }
               vClrSP0Flg(SP0_AUTOCOLOR_FLG);
               break;

        default :
            break;
    }
}


//UINT8 bGainBackup[3];
//UINT8 bOffsetBackup[3];
void vDrvSetPGAGainOffsetFromEEPROM(void)
{
    if(_bType<=INT_VGA_COMPOENT)    //for klocwork check
    {
        for(_bOrder=0; _bOrder<3; _bOrder++)
        {
          vDrvOnChipAutoColorModeSet(GAIN_MODE,_bType);
          vDrvOnChipAutoColorModeSet(OFFSET_MODE,_bType);
          bDrvSetHDADCOffset(_bOrder,_bInitOffset);                  //Set offset=0 mv
//          bGainBackup[_bOrder]= _bAutoColorGain[_bType][_bOrder];
//          bOffsetBackup[_bOrder]= _bAutoColorOffset[_bType][_bOrder];
        }
    }
}

PRIVATE void vDrvOnChipAutoColor_CheckGainOffset(UINT8 bMode)
{
UINT8 bTolenence;
  if((_bType <=INT_VGA_COMPOENT) && (bMode<=DIGITAL_OFFSET_MODE))   //for klocwork check
  {
       if(bMode==OFFSET_MODE)                          //MC20080417_1 ***
       {
          bTolenence = (_bAutoColorTimeOutCNT>6) ?  0x10 : 0 ;
       }
       else
       {
          bTolenence = (_bAutoColorTimeOutCNT>6) ?  1 : 0 ;
       }
      	
       for(_bOrder=0;  _bOrder<3; _bOrder++)
       {
          vDrvOnChipAutoColorModeSet(bMode,_bType);  // target, gain, offset
          bDrvSetHDADCOffset(_bOrder,_bInitOffset);   
          vUtDelay1ms(2);
          vDrvOnChipAutoColorGetADCData(bMode);
       }
     #if (AUTO_COLOR_DEBUG||ON_CHIP_AUTO_COLOR_DEBUG)
        Printf("--- Check Gain/ Offset --\n");
     #endif
     for (_bOrder=0;  _bOrder<3; _bOrder++)
    {
	 //Printf("ch1=%x, ch2=%x, ch3=%x\n",_wData_v1v2[0] ,_wData_v1v2[1], _wData_v1v2[2] );
        if ((_wData_v1v2[_bOrder] >= (_wADCTarget[_bOrder] - (bOnChipCheckTolerance[bMode][1]+bTolenence)))&&(_wData_v1v2[_bOrder] <= (_wADCTarget[_bOrder] + (bOnChipCheckTolerance[bMode][0]+bTolenence)))) 
        {
           _bGainOffset_channel |= (0x01 << _bOrder);
           if(bMode==GAIN_MODE)
           {
                  if(_bType==INT_HDTV)
                  {
                     _bHDTVMax[_bOrder]=_wData_v1v2[_bOrder];
                  }
                  else if(_bType==INT_VGA)
                  {
                     _bVGAMax[_bOrder]=_wData_v1v2[_bOrder];
                  }
           }
             #if (AUTO_COLOR_DEBUG||ON_CHIP_AUTO_COLOR_DEBUG)
                 if(bMode==OFFSET_MODE)  {  Printf("channel %u is OK, Blank=%u\n",(_bOrder),(_wData_v1v2[_bOrder])>>5);    }
                 else {    Printf("channel %u is OK, MaxLevel=%u\n",(_bOrder),(_wData_v1v2[_bOrder]));    }
	      #endif
	    }
	    else
	    {
		  _bGainOffset_channel &= (~(0x01 << _bOrder));
	       #if (AUTO_COLOR_DEBUG|| ON_CHIP_AUTO_COLOR_DEBUG)
		    if(bMode==OFFSET_MODE)   {  Printf("channel %u need cali, Blank=%u\n",(_bOrder),(_wData_v1v2[_bOrder])>>5); }
                    else                     {  Printf("channel %u need cali, MaxLevel=%u\n",(_bOrder),(_wData_v1v2[_bOrder]));  }
              #endif
            }
	   }
        }
     }

#if CHK_ADC_LINEARITY
void vDrvSetMeasureWindow(UINT8 location)
{
UINT8 bInput_timing;
UINT16 wHor_total;
UINT16 wBack_porch;
UINT16 wActive_area;
UINT16 wMeasure_Start_point;
UINT16 wMeasure_End_point;

        bInput_timing = bDrvVideoGetTiming(SV_VP_MAIN);
        wHor_total= Get_VGAMODE_IHTOTAL(bInput_timing);
        wBack_porch = Get_VGAMODE_IPH_BP(bInput_timing);
        wActive_area = Get_VGAMODE_IPH_WID(bInput_timing);
        //Printf("H total=%u,  BP=%u,  ACT=%u\n",wHor_total,wBack_porch,wActive_area);  
        wMeasure_Start_point = wBack_porch ; // wBack_porch + ((wActive_area>>3)*(7-location));   // 7/8, 6/8, 5/8, 4/8
        if(location > (TOTAL_POINT-1))
        {
           location = TOTAL_POINT-1;
        }
        wMeasure_End_point = wHor_total - wBack_porch -((wActive_area/TOTAL_POINT)*((TOTAL_POINT-1)-location)) ;//         wHor_total - wMeasure_Start_point -0x40;
        vDrvAsyncPreDataActive(wMeasure_Start_point,wMeasure_End_point);
        //Printf("pre-margin=%u,  post-margin=%u\n",wMeasure_Start_point,wMeasure_End_point);
}
#endif

void vCheckAutoColorGainRange(void)
{
UINT8 bCaliResult;
    
UINT8 i,j;
#ifdef CC_MT5365_V2
    UINT16 wGain_High_limit, wGain_LOW_limit;
    UINT16 wTmpOffset, wTmpGain;
    UINT8 bTmpHighByte,bTmpLowByte;
#else
    UINT8 bGain_High_limit, bGain_LOW_limit;
    UINT8 bTmpOffset, bTmpGain;
#endif
   
    
    if(_bType <=INT_VGA_COMPOENT)    //for klocwork check
    {
              bCaliResult = 0;
              for(_bOrder =0; _bOrder<3; _bOrder++)
              {
              #ifdef CC_MT5365_V2
			wTmpGain = bDrvGetHDADCGain_Digital(_bOrder);
			wTmpOffset = bDrvGetHDADCOffset(_bOrder);
			
			wGain_High_limit = GAIN_HIGH_LIMIT_100_DIGITAL[_bType][_bOrder];
			wGain_LOW_limit = GAIN_LOW_LIMIT_100_DIGITAL[_bType][_bOrder];

                    if((wTmpGain<wGain_High_limit) && (wTmpGain > wGain_LOW_limit))
                    {
                        bCaliResult= bCaliResult |(0x01<<_bOrder);
                        if (_bEEPROM_ready==1)
                        {
        			  //Printf("CH %x, Final GAIN value=0x%x, Hi_lim=0x%x, Low_lim=0x%x\n",_bOrder,wTmpGain,wGain_High_limit,wGain_LOW_limit);
                            bTmpHighByte = (wTmpGain>>8)&0xff;
                            bTmpLowByte = wTmpGain & 0xff;
                            UNUSED(fgApiEepromWriteByte(EEP_ADC_DIG_GAIN_HDTV0_LOW_BYTE + (_bType*6)+(2*_bOrder), bTmpLowByte));
                            UNUSED(fgApiEepromWriteByte(EEP_ADC_DIG_GAIN_HDTV0_HIGH_BYTE + (_bType*6)+(2*_bOrder), bTmpHighByte));
                            
                            bTmpHighByte = (wTmpOffset>>8)&0x03;
                            bTmpLowByte = wTmpOffset & 0xff;
        			  //Printf("CH %x, Final Offset value=0x%x, bTmpHighByte=0x%x, bTmpLowByte=0x%x\n",_bOrder,wTmpOffset,bTmpHighByte,bTmpLowByte);
                            UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_START + (_bType*3)+_bOrder, bTmpLowByte)); 
                            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_START + (_bType*3)+_bOrder, bTmpHighByte)); 
                        }
                        _bAutoColorGain[_bType][_bOrder] = wTmpGain;
                        _bAutoColorOffset[_bType][_bOrder] = wTmpOffset;      
                    }
                //Printf("CH%x, Offset value=%x, Gain value=%x\n",_bOrder,_bAutoColorOffset[_bType][_bOrder],_bAutoColorGain[_bType][_bOrder]);

	       #else
	             bTmpGain = bDrvGetHDADCGain(_bOrder);
	             bTmpOffset = bDrvGetHDADCOffset(_bOrder);
	             if(DRVCUST_OptGet(eVFE_HW_075ohm))
                    {
                          bGain_High_limit = GAIN_HIGH_LIMIT_75_100[_bType][_bOrder];
                          bGain_LOW_limit = GAIN_LOW_LIMIT_75_100[_bType][_bOrder];
                    }
                    else
                    {
                          if(DRVCUST_OptGet(eYPBPR_ADC_SUPPORT_120))
                          {
                                bGain_High_limit = GAIN_HIGH_LIMIT_120[_bType][_bOrder];
                                bGain_LOW_limit = GAIN_LOW_LIMIT_120[_bType][_bOrder];
                          }
                          else
                          {
                                bGain_High_limit = GAIN_HIGH_LIMIT_100[_bType][_bOrder];
                                bGain_LOW_limit = GAIN_LOW_LIMIT_100[_bType][_bOrder];
                          }
                    }

	             #if  AUTO_COLOR_DEBUG
	                  Printf("Gain HIGH limit=0x%x ,LOW limit=0x%x\n", bGain_High_limit,bGain_LOW_limit);
	             #endif
                    if((bTmpGain<bGain_High_limit) && (bTmpGain > bGain_LOW_limit))
                    {
                        bCaliResult= bCaliResult |(0x01<<_bOrder);
                        if (_bEEPROM_ready==1)
                        {
                            UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_START + (_bType*3)+_bOrder, bTmpOffset)); 
                            UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_START + (_bType*3)+_bOrder, bTmpGain));
                        }
                        _bAutoColorGain[_bType][_bOrder] = bTmpGain;
                        _bAutoColorOffset[_bType][_bOrder] = bTmpOffset;                   
                    }
              #endif
              }
		 
		#if  AUTO_COLOR_DEBUG 
			 Printf("The auto color result=%x\n",bCaliResult);
		#endif
		      Printf("==== Iteration count=%u  ===\n",_bAutoColorTimeOutCNT);
		      
              if((bCaliResult==0x07)&&(_bAutoColorTimeOutCNT<AUTO_COLOR_MAX_TIME))
              {
                    if (_bEEPROM_ready==1)
                    {
                        UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE+_bType,SV_TRUE));
                    }
                    _bAutoColorHistory[_bType] = SV_TRUE;
                    if(_bType==_bOrgType)
                    {
                         if((_bType==INT_SCART) &&(DRVCUST_OptGet(eSCART_DO_AUTOCOLOR)==1)) ////MC20081115 add for SCART //if SCART auto color function is not enable then do auto color by VGA signal
                         {
                             vApiNotifyAutoColorDone(0,TRUE);
                         }             
                         else
                         {
                              vApiNotifyAutoColorDone(((fgIsMainVga() || fgIsMainYPbPr())?0:1),TRUE);
                         }
                    }
                
                    if((DRVCUST_OptGet(eVFE_HW_SPECIAL_TARGET)) && (DRVCUST_OptGet(eVFE_HW_YPbPrMapToVGA)) && (_bType==0) )//for SONY YPbPr(_bType==0) mappint to VGA,SCART
                    {
                       for(i=0;i<4;i++)
                       {
   	                   for(j=0;j<3;j++)
                          {
                            wOnChipColorGainTable_Temp[i][j] = wYPbPrMappingVgaGainTable[i][j];
   	                   }
   	                }
   	                //Mapping to VGA, VGA's _bType = 1
                       vDrvGeneralAutoColorGainSetToNextType((UINT16 *)wOnChipColorGainTable_Temp,0,1);//_bType 0 maps to 1
                       if (_bEEPROM_ready==1)
                       {
					for(_bOrder=0;_bOrder<3;_bOrder++)
					{
					 UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_START + (1*3)+_bOrder, _bAutoColorGain[1][_bOrder]));
					 UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_START + (1*3)+_bOrder, bVGADefaultOffsetTABLE_100[1][_bOrder])); 
					}
					UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE+1,SV_TRUE));
                       }
                       //Mapping to SCART, SCART's _bType = 2
                       vDrvGeneralAutoColorGainSetToNextType((UINT16 *)wOnChipColorGainTable_Temp,0,2);//_bType 0 maps to 2
                       if (_bEEPROM_ready==1)
                       {
					for(_bOrder=0;_bOrder<3;_bOrder++)
					{
					 UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_START + (2*3)+_bOrder, _bAutoColorGain[2][_bOrder]));
					 UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_START + (2*3)+_bOrder, bVGADefaultOffsetTABLE_100[2][_bOrder])); 
					}
					UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE+2,SV_TRUE));
                       }
                       //Mapping to VGA_YPBPR, VGA_YPBPR's _bType = 3
                       vDrvGeneralAutoColorGainSetToNextType((UINT16 *)wOnChipColorGainTable_Temp,0,3);//_bType 0 maps to 3
                       if (_bEEPROM_ready==1)
                       {
					for(_bOrder=0;_bOrder<3;_bOrder++)
					{
					 UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_START + (3*3)+_bOrder, _bAutoColorGain[3][_bOrder]));
					 UNUSED(fgApiEepromWriteByte(EEP_ADC_OFFSET_START + (3*3)+_bOrder, bVGADefaultOffsetTABLE_100[3][_bOrder])); 
					}
					UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE+3,SV_TRUE));
                       }         	
                       Printf("==== Auto color mapping finished  VGA Gain1=%2x, Gain2=%2x, Gain3=%x ====\n", _bAutoColorGain[1][0],_bAutoColorGain[1][1],_bAutoColorGain[1][2]);
                    }//
                    
                    Printf("==== Auto color success  ===\n");
               }
               else
               {
                    if (_bEEPROM_ready==1)
                    {
                         UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE+_bType,SV_FALSE));
                    }
                    _bAutoColorHistory[_bType] = SV_FALSE;
                    if(_bType==_bOrgType)
                    {
                         if((_bType==INT_SCART) &&(DRVCUST_OptGet(eSCART_DO_AUTOCOLOR)==1)) ////MC20081115 add for SCART //if SCART auto color function is not enable then do auto color by VGA signal
                         {
                             vApiNotifyAutoColorDone(0,FALSE);
                         }             
                         else
                         {
                             vApiNotifyAutoColorDone(((fgIsMainVga() || fgIsMainYPbPr())?0:1),FALSE);
                         }
                    }
                    #ifdef __MODEL_slt__
                     vDrvSetOSTGOutPattern(1);
                    #endif

                    if(DRVCUST_OptGet(eAUTOCOLOR_FAIL_SHOW_PATTERN))
                    {
                      vDrvSetOSTGOutPattern(1);
                    }
                    
                    Printf("==== Auto color fail !!!!  ===\n");
                }
          //Printf("auto color history: HDTV=%x, VGA=%x, SCART=%x, VGACOMP=%x\n",_bAutoColorHistory[0],_bAutoColorHistory[1],_bAutoColorHistory[2],_bAutoColorHistory[3]);
          #if EEPROM_READY             
                #if  EEPROM_USAGE_debug
                #ifdef CC_MT5365_V2
                UINT8 bTmpHigh[3];
                UINT8 bTmpLow[3];
                #else
                UINT8 bTmpEEPROMdata[3];                
                #endif
                UINT8 bCh, bType;

                bType = _bType;
                          for (bType=0; bType<4; bType++)
                          {
                        #ifdef CC_MT5365_V2
                                for(bCh=0; bCh<3; bCh++)
                                {
		                           bTmpHigh[bCh] =  bApiEepromReadByte((EEP_ADC_DIG_GAIN_HDTV0_HIGH_BYTE + (bType*6)+(2*bCh)));
		                           bTmpLow[bCh] =  bApiEepromReadByte((EEP_ADC_DIG_GAIN_HDTV0_LOW_BYTE + (bType*6)+(2*bCh)));
		                   }
                                Printf("after Type=%u  ,Gain1  =0x%x%x ,Gain2  =0x%x%x ,Gain3  =0x%x%x \n",bType,bTmpHigh[0],bTmpLow[0],bTmpHigh[1],bTmpLow[1],bTmpHigh[2],bTmpLow[2]);
                        #else                          
                                for(bCh=0; bCh<3; bCh++)
                                {
                                     bTmpEEPROMdata[bCh] = bApiEepromReadByte((EEP_VIDEO_AUTO_COLOR_START + (bType*3) + bCh));
                                }
                                Printf("after Type=%u  ,Gain1  =0x%x ,Gain2  =0x%x ,Gain3  =0x%x \n",bType,bTmpEEPROMdata[0],bTmpEEPROMdata[1],bTmpEEPROMdata[2]);
                        #endif                                
                           }
                 #endif
           #endif
    }           
}

#if (CHK_ADC_LINEARITY )
PRIVATE void vDrvGetMaxValue16Times(void)
{
    for(_bOrder=0; _bOrder<3; _bOrder++)
    {
        vDrvOnChipAutoColorGetADCData(GAIN_MODE);
        _dwMaxLevelAvg[_bOrder] += _wData_v1v2[_bOrder] ;    //8 bit resolution
    }
    //Printf("_bAutoColorTimeOutCNT=%u, _wData_v1v2=0x%x, _dwMaxLevelAvg=0x%x\n",_bAutoColorTimeOutCNT  ,_wData_v1v2[0], _dwMaxLevelAvg[0] );
}
#endif

#if (CHK_ADC_LINEARITY || CHK_PGA_LINEARITY)
PRIVATE void vDrvClearMAXvariable(void)
{
UINT8 bLoop;
    for (bLoop=0; bLoop<3; bLoop++)
    {
           _dwMaxLevelAvg[bLoop]=0;
           _wData_v1v2[bLoop] = 0;
    }
}
#endif

/*
void vDrvResetDigitalGainOffset(void)
{
#if DIGITAL_NEW_OFFSET
UINT8 bCH;
          for(bCH=0; bCH<3; bCH++)
          {
              vDrvSetHDADCDigitalOffset(bCH,0);
          }
#endif          
          vRegWriteFldAlign(HDFE_01, 0x8000, AD1_GAIN);  // reset the Digital gain to uint gain
          vRegWriteFldAlign(HDFE_01, 0x8000, AD2_GAIN);	
          vRegWriteFldAlign(HDFE_02, 0x8000, AD3_GAIN);
}
*/
void vDrvEnablePGALinearityVerify(void)
{
#if CHK_PGA_LINEARITY    
     vDrvAsyncPreDataActive(0x20, 0x20);  //vDrvSetMeasureWindow(0xff);
     vDrvClearMAXvariable();  // to clear _dwMaxLevelAvg[];
     //_bPGALinearity = 0;
     _bPGA_LINEARITY_state = PGA_VERIFY_INIT;
#endif     
}

void vDrvPGALinearityVerify(void)
{
#if CHK_PGA_LINEARITY
          if( bDrvOnChipAutoColorCheckSignalReady() ==P_FA)
          {
               return;
          }

          switch (_bPGA_LINEARITY_state)
          {
          case PGA_VERIFY_INIT: 
                  vDrvHDTVSelBlankDataSource();              //to stop HW auto blank adjust function
                  vDrvResetAllGainOffset();                         // to reset all of the gain and offset
                  _bGain = 0;
                  _bPGA_LINEARITY_state = PGA_VERIFY_2;
                   break;
                   
          case PGA_VERIFY_2:                                                                    
                  for(_bOrder=0; _bOrder<3; _bOrder++)  // to set minimum gain
                  {
                       bDrvSetHDADCGain(_bOrder,_bGain);  
                       _dwMaxLevelAvg[_bOrder] = 0;
                  }
                  _bAutoColorTimeOutCNT = 4;
                  _bPGA_LINEARITY_state = PGA_VERIFY_3;
                  break;
                  
          case PGA_VERIFY_3:                                                                    
                  _bAutoColorTimeOutCNT=_bAutoColorTimeOutCNT-1;
                  if(_bAutoColorTimeOutCNT==0)
                  {
                       _bPGA_LINEARITY_state = PGA_VERIFY_4;
                  }
                  break;
                   
          case PGA_VERIFY_4: 
                  if(_bAutoColorTimeOutCNT<8)
                  {
                       _bAutoColorTimeOutCNT++;
                       vDrvOnChipGetADCMaxValue();
                       _dwMaxLevelAvg[0] += _wData_v1v2[0] ;    //8 bit resolution
                       _dwMaxLevelAvg[1] += _wData_v1v2[1] ;
                       _dwMaxLevelAvg[2] += _wData_v1v2[2] ;
                  }
                  else
                  {
                       for(_bOrder=0; _bOrder<3; _bOrder++)
                       {
                           _dwMaxLevelAvg[_bOrder] = (_dwMaxLevelAvg[_bOrder]+(_bAutoColorTimeOutCNT/2) )/_bAutoColorTimeOutCNT;
                           //_dwBlankAvg[_bOrder] = (_dwBlankAvg[_bOrder]+(_bAutoColorTimeOutCNT/2) )/_bAutoColorTimeOutCNT;
                      }
                      Printf("Maximum level @ Gain= %3u :PGA_Max1= %3u ,PGA_Max2= %3u ,PGA_Max3= %3u\n",_bGain,_dwMaxLevelAvg[0],_dwMaxLevelAvg[1],_dwMaxLevelAvg[2]);
                      if ( _bGain<0xff)
                      {
                          _bGain ++;
                          _bPGA_LINEARITY_state = PGA_VERIFY_2;
                      }
                      else
                      {
                            Printf("---To restore the gain and offset ---\n");
                            vDrvSetPGAGainOffsetFromEEPROM();
                            for(_bOrder=0; _bOrder<3; _bOrder++)
                            {
                                 Printf("_bOrder=%x, Initial GAIN=0x%x, Initial OFFSET=0x%x\n",_bOrder,bDrvGetHDADCGain(_bOrder),bDrvGetHDADCOffset(_bOrder));
                            }         
                            #ifdef FULLY_HW_AUTO_CALIBRATION
                                 vDrvHDTV_HW_AUTO_ONOFF(CALI_ENABLE,_bType);
                            #else
                                 vDrvEnableBlankLevelAdjust();
                            #endif
                             _bPGA_LINEARITY_state = PGA_VERIFY_NOTHING;
                      }
                   }
                   break;
          }         
#endif
}

void vDrvEnableVGARefVoltageVerify(UINT8 bType)
{
#if CHK_VGA_Ref_Voltage  
#endif     
}

void vDrvEnableSyncSlicerRefVoltageVerify(UINT8 bType)
{
#if CHK_VGA_Ref_Voltage  
#endif     
}

void vDrvOnChipAutoColorIteration(void)
{
#ifdef CC_MT5365_V2
UINT16 wTmp;
UINT16 wTmp2;
#else
     #if (AUTO_COLOR_DEBUG || CHK_BLANK_VARIATION) 
          UINT8 bTmp;
          UINT8 bTmp2;
     #endif
#endif



   switch (_bAutoColorState0)
   {
     case VDO_AUTO_COLOR_NOT_BEGIN:
           #if CHK_PGA_LINEARITY
              if(_bPGA_LINEARITY_state !=PGA_VERIFY_NOTHING)
	      {
                    vDrvPGALinearityVerify();
     	      }
	   #endif

	   #if CHK_ADC_LINEARITY
	      if(_bADC_LINEARITY_state!=ADC_VERIFY_0)
	      {
                   vDrvADCLinearityVerify();
	      }
	   #endif
	   break;

	 case VDO_AUTO_COLOR_START:
	    _bType = bDrvOnChipAutoColorCheckSignalReady();
	    if((_bType<=INT_VGA_COMPOENT) && (_bType != P_FA) )   //for klocwork check
          {
              _bOrgType = _bType;
              #ifndef __MODEL_slt__              
              if(_bType ==INT_VGA)
              {
                  if(DRVCUST_OptGet(eSCART_DO_AUTOCOLOR)==0) ////MC20081115 add for SCART //if SCART auto color function is not enable then do auto color by VGA signal
                  {
                      _bType = INT_SCART;
                  }             
              }
              #endif
              vDrvHDTVSelBlankDataSource();
              _bAutoColorState0 = VDO_AUTO_COLOR_1P0_START;  //
          }
          else
          {
		   _bAutoColorState0 = VDO_AUTO_COLOR_END;
          }
          break;
        
     case VDO_AUTO_COLOR_1P0_START:  //to set gain and offset for the corresponding signal type
          vDrvSetPGAGainOffsetFromEEPROM();
          vDrvAsyncPreDataActive(0x20, 0x20);  //vDrvSetMeasureWindow(0xff);
          _bOffsetGainDone = 0;
          _bAutoColorTimeOutCNT = 0;
          _bAutoColorState0 = VDO_AUTO_COLOR_1_START;
       #ifdef CC_MT5365_V2
            //vRegWriteFldAlign(VFE_02, 0, PGAGAIN_1);  // reset the Analog gain to unit gain
            //vRegWriteFldAlign(VFE_06, 0, PGAGAIN_2);
            //vRegWriteFldAlign(VFE_0A, 0, PGAGAIN_3);
            vRegWriteFldAlign(HDFE_00, 0, SIGN_OFFSET1);
            vRegWriteFldAlign(HDFE_02, 0, AD1_OFFSET);
            vRegWriteFldAlign(HDFE_00, 0, SIGN_OFFSET2);
            vRegWriteFldAlign(HDFE_02, 0, AD2_OFFSET);
            vRegWriteFldAlign(HDFE_00, 0, SIGN_OFFSET3);
            vRegWriteFldAlign(HDFE_00, 0, AD3_OFFSET);
               for(_bOrder=0; _bOrder<3; _bOrder++)
               {
                    wTmp = bDrvGetHDADCOffset(_bOrder);
                    wTmp2 = bDrvGetHDADCGain_Digital(_bOrder);
                    Printf("== _bOrder=%x, Initial GAIN=0x%x, OFFSET=0x%x ====\n",_bOrder,wTmp2,wTmp);
               }         

       #else
          vRegWriteFldAlign(HDFE_01, 0x8000, AD1_GAIN);  // reset the Digital gain to uint gain
          vRegWriteFldAlign(HDFE_01, 0x8000, AD2_GAIN);	
          vRegWriteFldAlign(HDFE_02, 0x8000, AD3_GAIN);
          vRegWriteFldAlign(HDFE_00, 0, SIGN_OFFSET1);
          vRegWriteFldAlign(HDFE_02, 0, AD1_OFFSET);          
          vRegWriteFldAlign(HDFE_00, 0, SIGN_OFFSET2);
          vRegWriteFldAlign(HDFE_02, 0, AD2_OFFSET);          
          vRegWriteFldAlign(HDFE_00, 0, SIGN_OFFSET3);
          vRegWriteFldAlign(HDFE_00, 0, AD3_OFFSET);          
          #if AUTO_COLOR_DEBUG
               for(_bOrder=0; _bOrder<3; _bOrder++)
               {
                    bTmp = bDrvGetHDADCOffset(_bOrder);
                    bTmp2 = bDrvGetHDADCGain(_bOrder);
                    Printf("== _bOrder=%x, Initial GAIN=0x%x, OFFSET=0x%x ====\n",_bOrder,bTmp2,bTmp);
               }         
          #endif
       #endif
        
        break;

     case VDO_AUTO_COLOR_1_START:  //to wait for gain and offset stable
        if (fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG))
	{
           _bCaliMode=OFFSET_MODE;
           vClrSP0Flg(SP0_AUTOCOLOR_FLG);
           _bAutoColorState0 = VDO_AUTO_COLOR_2_START;
	}
        break;

     case VDO_AUTO_COLOR_2_START:
        if(((_bOffsetGainDone & 0x03) == 0x03) || (_bAutoColorTimeOutCNT>AUTO_COLOR_MAX_TIME))
	{
	   //Printf("--- _bOffsetGainDone=%x, _bAutoColorTimeOutCNT=%x\n",_bOffsetGainDone,_bAutoColorTimeOutCNT);
           _bAutoColorState0 = VDO_AUTO_COLOR_4_START;
	}
        else
        {
          if (fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG))
          {
             vClrSP0Flg(SP0_AUTOCOLOR_FLG);
	     _bGainOffset_channel =0x07;
             //vRegWriteFldAlign(HDTV_03, AS_BLANK_ALWAYS, HDTV_BLAK_SET);
             vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
             vDrvOnChipAutoColor_CheckGainOffset(_bCaliMode);
             if((_bGainOffset_channel&0x07)!=0x07)
	     {
                _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_INITIAL_STATE;
                _bAutoColorTimeOutCNT ++;
                _bOffsetGainDone= 0;
	     }  
	     else
	     {
                _OnChipAutoColorState= ON_CHIP_AUTO_COLOR_END;
                _bOffsetGainDone = (_bCaliMode == OFFSET_MODE) ? (_bOffsetGainDone | 0x01) : (_bOffsetGainDone | 0x02) ;
             }
             _bAutoColorState0 = VDO_AUTO_COLOR_3_START;
	  }
        }
        break;

      case VDO_AUTO_COLOR_3_START:
        if(_OnChipAutoColorState != ON_CHIP_AUTO_COLOR_END)
        {
           vDrvOnChipAutoColor_GainOffset(_bCaliMode);
           if (bDvMeasureDataSel()==1)
           {
                _bAutoColorState0 = VDO_AUTO_COLOR_3_START_Delay1;
           }
        }
        else
        {
           _bCaliMode = (_bCaliMode == GAIN_MODE)  ? OFFSET_MODE : GAIN_MODE ;
           _bAutoColorState0 = VDO_AUTO_COLOR_2_START;
        }
        break;

     case VDO_AUTO_COLOR_3_START_Delay1:
        if (fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG))
        {
           vClrSP0Flg(SP0_AUTOCOLOR_FLG);
           _bAutoColorState0 = VDO_AUTO_COLOR_3_START;
        }
        break;

     case VDO_AUTO_COLOR_4_START:
        if (fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG))
        {
           //--------- to show the PGA offset --------------
           for(_bOrder=0;  _bOrder<3; _bOrder++)
           {
             _wData_v1v2[_bOrder]=bDrvGetHDADCOffset(_bOrder);
           }
           if(_bType==INT_HDTV)
           {
               Printf("HDTV: Offset_1=0x%x, Offset_2=0x%x, Offset_3=0x%x\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           else if(_bType==INT_VGA)
           {
               Printf("VGA: Offset_1=0x%x, Offset_2=0x%x, Offset_3=0x%x\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           else if(_bType==INT_SCART)
           {
               Printf("SCART: Offset_1=0x%x, Offset_2=0x%x, Offset_3=0x%x\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           
           //--------- to show the PGA gain --------------
           for(_bOrder=0;  _bOrder<3; _bOrder++)
           {
           #ifdef CC_MT5365_V2
             _wData_v1v2[_bOrder]=bDrvGetHDADCGain_Digital(_bOrder);
           #else
             _wData_v1v2[_bOrder]=bDrvGetHDADCGain(_bOrder);
           #endif
           }
           if(_bType==INT_HDTV)
           {
               Printf("HDTV: Gain1=0x%x, Gain2=0x%x, Gain3=0x%x\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           else if(_bType==INT_VGA)
           {
               Printf("VGA: Gain1=0x%x, Gain2=0x%x, Gain3=0x%x\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           else if(_bType==INT_SCART)
           {
               Printf("SCART: Gain1=0x%x, Gain2=0x%x, Gain3=0x%x\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           
           //--------- to show the blank level --------------
          vDrvPreSetToReadBlank(AS_BLANK_ALWAYS);
           for(_bOrder=0;  _bOrder<3; _bOrder++)
           {
             vDrvOnChipAutoColorGetADCData(OFFSET_MODE);
           }
           if(_bType==INT_HDTV)
           {
               Printf("HDTV: 12 bits base: Blank1=0x%3x, Blank2=0x%3x, Blank3=0x%3x\n",(_wData_v1v2[0]>>1),(_wData_v1v2[1]>>1),(_wData_v1v2[2]>>1));
           }
           else if(_bType==INT_VGA)
           {
               Printf("VGA: 12 bits base: Blank1=0x%3x, Blank2=0x%3x, Blank3=0x%3x\n",(_wData_v1v2[0]>>1),(_wData_v1v2[1]>>1),(_wData_v1v2[2]>>1));
           }
           else if(_bType==INT_SCART)
           {
               Printf("SCART: 12 bits base: Blank1=0x%3x, Blank2=0x%3x, Blank3=0x%3x\n",(_wData_v1v2[0]>>1),(_wData_v1v2[1]>>1),(_wData_v1v2[2]>>1));
           }
           
           //--------- to show the maximum level --------------
           vDrvOnChipGetADCMaxValue();
           if(_bType==INT_HDTV)
           {
               Printf("HDTV: Max1=%3u, Max2=%3u, Max3=%3u\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           else if(_bType==INT_VGA)
	            {
               Printf("VGA: Max1=%3u, Max2=%3u, Max3=%3u\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
             }
           else if(_bType==INT_SCART)
             {
               Printf("SCART: Max1=%3u, Max2=%3u, Max3=%3u\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
           }
           
           vClrSP0Flg(SP0_AUTOCOLOR_FLG);

           if(_bType == _bOrgType)
	    {
            #if (CHK_ADC_LINEARITY)
                 _bAutoColorTimeOutCNT = 0;
                 vDrvEnableADCLinearityVerify(16);
                 _bAutoColorState0 = VDO_AUTO_COLOR_END;
            #elif (CHK_BLANK_VARIATION )
                 _bAutoColorTimeOutCNT = 0;
                 Printf("--- To check Gain/offset iteration ---\n");
                 vDrvSetPGAGainOffsetFromEEPROM();
                 Printf("----Sum of 32 times for Blank=\n");
                 _bAutoColorState0 = VDO_AUTO_COLOR_5_START;
           #else
                #ifdef CC_MT5365_V2
                #else
                if (_bEEPROM_ready==1)
                {
                    if(_bType==INT_VGA)
                    {
			     UNUSED(fgApiEepromWriteByte(EEP_ADC_VGA_MAX_CH0,_bVGAMax[0]));
			     UNUSED(fgApiEepromWriteByte(EEP_ADC_VGA_MAX_CH1,_bVGAMax[1]));
			     UNUSED(fgApiEepromWriteByte(EEP_ADC_VGA_MAX_CH2,_bVGAMax[2]));
                    }
		       else if (_bType==INT_HDTV)
		       {
			     UNUSED(fgApiEepromWriteByte(EEP_ADC_HDTV_MAX_CH0,_bHDTVMax[0]));
			     UNUSED(fgApiEepromWriteByte(EEP_ADC_HDTV_MAX_CH1,_bHDTVMax[1]));
			     UNUSED(fgApiEepromWriteByte(EEP_ADC_HDTV_MAX_CH2,_bHDTVMax[2]));						
		       }
		   }
		   #endif
                _bAutoColorState0 = VDO_AUTO_COLOR_END;
           #endif
	    }
	    else
	    {
	          vCheckAutoColorGainRange();
                 _bType = _bOrgType;
                 _bAutoColorState0 = VDO_AUTO_COLOR_1P0_START;
           } 
        }
        break;
        
    #if ( CHK_BLANK_VARIATION)
     case VDO_AUTO_COLOR_5_START:
        if (fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG))
        {
		vClrSP0Flg(SP0_AUTOCOLOR_FLG);
		_bAutoColorTimeOutCNT ++;
	          if(RegReadFldAlign(HDTV_00,HDTV_BLK_STA_SEL)==BEFORE_ADJ)
	          {
	            Printf("--- Blank level is before adjust ----\n");
	          }
	          else
	          {
	            Printf("--- Blank level is after adjust ----\n");
	          }
	          if(RegReadFldAlign(HDTV_02,HDTV_BLK_CALI_EN)==CALI_ENABLE)
                {
                   Printf("--- HDTV_BLK_CALI_EN is enable ----\n");
                }
                else
                {
                   Printf("--- HDTV_BLK_CALI_EN is disable ----\n");
                }
		if(_bAutoColorTimeOutCNT<0x20)
		{
			for(_bOrder=0;  _bOrder<3; _bOrder++)
			{
				vDrvOnChipAutoColorGetADCData(OFFSET_MODE);
				Printf("_bOrder=%x, blank level=%d\n",_bOrder,  (_wData_v1v2[_bOrder]>>2));
			}
		}
		else
		{
			 _bAutoColorState0 = VDO_AUTO_COLOR_END;
			 _bAutoColorTimeOutCNT = 0;
		}
	  }
        break;
  #endif  

     case VDO_AUTO_COLOR_END:
             //_bAutoColorBlankMeasure = 0;
             _bAutoColorState0 = VDO_AUTO_COLOR_NOT_BEGIN;
             _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_DO_NOTHING;

             vDrvPreSetToReadBlank(AS_BLANK_RESET);
             vCheckAutoColorGainRange();
             vApiVideoClrEvtFlg(VDO_FLG_VGAAUTO_DOING);
             vDrvAsyncPreDataActive(0x20, 0x20);  //vDrvSetMeasureWindow(0xff);
             #ifdef FULLY_HW_AUTO_CALIBRATION
                     vDrvHDTV_HW_AUTO_ONOFF(CALI_ENABLE,_bType);
             #endif
             
             if(DRVCUST_OptGet(eDoAutoPhaseAfterAutoColor))
             {
                 vDrvYPbPrAutoStart();
             }



     default:
        break;
   }
}



//=============================================================================================================
//Description : This subroutine can support 3 kinds calibration mode by setting the parameter bMode. 
//
//Parameter   : bMode =0 ; do gain calibration. This mode uses on chip voltage to do gain calibration.
//            : bMode =1 ; do offset calibration. This mode will calibrate the offset base on the input signal.
//            : bMode =2 ; do gain calibration. This mode uses on chip voltage to do gain calibration.
//
//return      : none.
//==============================================================================================================

void vDrvOnChipAutoColor_GainOffset(UINT8 bMode) 
{
UINT8 bIndexCnt,bCaliNG; //, bTmpCnt, bTmpCNT2;
#ifdef CC_MT5365_V2
  #define CALISTEP                0x400 //0x08*0x80
#else
  #define CALISTEP                0x08
#endif
#define INITGAIN                0x50
#define INITOFFSET              0x80

 if( (bMode == ON_CHIP_GAIN_MODE ) || (bMode == ON_CHIP_OFFSET_MODE))
 {
 }
 else
 {
    if (!fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG))
   {
	      return;
    }
    vClrSP0Flg(SP0_AUTOCOLOR_FLG);
 }

if((_bType<=INT_VGA_COMPOENT) &&(bMode<=DIGITAL_OFFSET_MODE))//for klocwork check
{
 switch (_OnChipAutoColorState) 
 {
    case ON_CHIP_AUTO_COLOR_DO_NOTHING:
       break;
       
    case ON_CHIP_AUTO_COLOR_INITIAL_STATE:
    //Printf("--------- ON_CHIP_AUTO_COLOR_INITIAL_STATE, mode=%x----\n",bMode);
       if((bMode== ON_CHIP_GAIN_MODE)||(bMode== ON_CHIP_OFFSET_MODE))
       {
         _bType= INT_VGA;
       }
       else if((_bAutoColorState0 == VDO_AUTO_COLOR_NOT_BEGIN) && (bMode== OFFSET_MODE))
       {
         _bType = bDrvOnChipAutoColorCheckSignalReady();
         if(_bType==P_FA)
         {
           break; 
         }
       }

       if((_bGainOffset_channel&0x07)==0x07)
       {
          _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_FINISH; 
       }
       else
       {
          if(_bType<=INT_VGA_COMPOENT)   //for klocwork check
          {
              for(bIndexCnt=0; bIndexCnt<3; bIndexCnt++)
              {
                 if(!(_bGainOffset_channel & (0x01<<bIndexCnt)))
	          {
                     _bOrder = bIndexCnt;
                     vDrvOnChipAutoColorModeSet(bMode,_bType);
                     bDrvSetHDADCOffset(_bOrder,_bInitOffset);                  //Set offset=0 mv
                     vUtDelay1ms(5);
                 #ifdef CC_MT5365_V2
                     if(_bCaliMode == OFFSET_MODE)
                     {
                        _bCaliStep[_bOrder]       = 0x08;//notice
                     }  
                     else
                     {
                        _bCaliStep[_bOrder]       = CALISTEP;
                     }
                 #else
                        _bCaliStep[_bOrder]       = CALISTEP;
                 #endif

                     _bDoubleChkFlag[_bOrder] = 0;
                     _bGainIncDir[_bOrder]     = 0xff;
                     _bChgDirToggle[_bOrder]  = 0;
                     _wDiff_mim[_bOrder]      = 0xffff;
                  }
               }
          }
      #ifdef CC_MT5365_V2
      #else
          vRegWriteFldAlign(HDFE_01, 0x8000, AD1_GAIN);  // reset the Digital gain to uint gain
          vRegWriteFldAlign(HDFE_01, 0x8000, AD2_GAIN);	
          vRegWriteFldAlign(HDFE_02, 0x8000, AD3_GAIN);         
      #endif
          
          _bTotalCnt       = 0;
          _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_SEARCH_TARGET; 
          /*
          Printf("Print the register value");
          UINT32 dWData;
          for(bIndexCnt=0;bIndexCnt<14;bIndexCnt++)
          {
            dWData = u4RegRead4B(VFE_00+(4*bIndexCnt));
            Printf("Reg address=0x%x value=0x%x\n",VFE_00+(4*bIndexCnt),dWData);
          }
          */
       }
	break;

     case ON_CHIP_AUTO_COLOR_SEARCH_TARGET:  //get ADC raw data 
       //Printf("--------- ON_CHIP_AUTO_COLOR_SEARCH_TARGET ----\n");
       _bTotalCnt ++;
       
       for(bIndexCnt=0; bIndexCnt<3; bIndexCnt++)
       {
          if(!(_bGainOffset_channel & (0x01<<bIndexCnt)))
          {
             _bOrder = bIndexCnt;
             vDrvOnChipAutoColorGetADCData(bMode);
             if((bMode==OFFSET_MODE)||(bMode==ON_CHIP_OFFSET_MODE))    
	      {
                _bGain = bDrvGetHDADCOffset(_bOrder); 	    
                #if (AUTO_COLOR_DEBUG || ON_CHIP_AUTO_COLOR_DEBUG)
                Printf("order=%2u, offset=%x, step=%x, toggle=%u, Data=0x%x\n",_bOrder,_bGain,_bCaliStep[_bOrder],_bChgDirToggle[_bOrder],_wData_v1v2[_bOrder]);  
                #endif

            #ifdef CC_MT5365_V2
                vDrvOnChipAutoColorSearchTarget_V2(bMode);
            #else
                vDrvOnChipAutoColorSearchTarget(bMode);
            #endif
                bDrvSetHDADCOffset(_bOrder,_bGain); 	       
	      }
	      else
	      {
            #ifdef CC_MT5365_V2
    	          _bGain = bDrvGetHDADCGain_Digital(_bOrder);
                #if (AUTO_COLOR_DEBUG || ON_CHIP_AUTO_COLOR_DEBUG)
                Printf("order=%2u, Gain=%x, step=%x, toggle=%u, Data=0x%x\n",_bOrder,_bGain,_bCaliStep[_bOrder],_bChgDirToggle[_bOrder],_wData_v1v2[_bOrder]);    
                #endif
                vDrvOnChipAutoColorSearchTarget_V2(bMode);
  	          bDrvSetHDADCGain_Digital(_bOrder, _bGain);
            #else
                _bGain = bDrvGetHDADCGain(_bOrder);            
                #if (AUTO_COLOR_DEBUG || ON_CHIP_AUTO_COLOR_DEBUG)
                Printf("order=%2u, Gain=%x, step=%x, toggle=%u, Data=0x%x\n",_bOrder,_bGain,_bCaliStep[_bOrder],_bChgDirToggle[_bOrder],_wData_v1v2[_bOrder]);    
                #endif
                vDrvOnChipAutoColorSearchTarget(bMode);
                bDrvSetHDADCGain(_bOrder,_bGain);          
            #endif 
            }
          }
       }

       if((_bGainOffset_channel&0x07)==0x07)
       {
          _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_CHECK_VALUE; 
	}
       else if ((bMode==GAIN_MODE)||(bMode==OFFSET_MODE))
       {
         _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_WAIT_VALUE; 
       }
       break;

    case ON_CHIP_AUTO_COLOR_WAIT_VALUE:
       _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_SEARCH_TARGET; 
	break;

    case ON_CHIP_AUTO_COLOR_CHECK_VALUE:
       bCaliNG = 0;
       for(_bOrder=0; _bOrder<3; _bOrder++)
       {
          if ((_wData_v1v2[_bOrder] >= (_wADCTarget[_bOrder] - bOnChipCalibrateTolerance[bMode][3]))&&(_wData_v1v2[_bOrder] <= (_wADCTarget[_bOrder] + bOnChipCalibrateTolerance[bMode][2]))) //0x87~0x89
          {
             if((bMode==OFFSET_MODE)||(bMode==ON_CHIP_OFFSET_MODE))
             {
                _bGain = bDrvGetHDADCOffset(_bOrder);
                if(bMode==OFFSET_MODE)
                {
                    if (_wData_v1v2[_bOrder] > (_wADCTarget[_bOrder] + 6)) 
                    {
                        _bGain=_bGain+1;
                    }
                    else if(_wData_v1v2[_bOrder] < (_wADCTarget[_bOrder] - 6)) 
                    {
                        _bGain = _bGain-1;
                    }
                    bDrvSetHDADCOffset(_bOrder,_bGain); 	       
                }
                _bAutoColorOffset[_bType][_bOrder] = _bGain;
                
                #if (AUTO_COLOR_DEBUG||ON_CHIP_AUTO_COLOR_DEBUG)
                Printf("Channel %u Offset success. offset=%x, ADC=%u, target=%u\n", _bOrder,_bGain,_wData_v1v2[_bOrder],_wADCTarget[_bOrder]);
                #endif
             }
             else
             {

             #ifdef CC_MT5365_V2
                _bGain = bDrvGetHDADCGain_Digital(_bOrder);
             #else
                _bGain = bDrvGetHDADCGain(_bOrder);
             #endif
                if(bMode==ON_CHIP_GAIN_MODE)
                {
                   _bReadMAXLevel=1;
                }
                   _bAutoColorGain[_bType][_bOrder] = _bGain;

                #if (AUTO_COLOR_DEBUG ||ON_CHIP_AUTO_COLOR_DEBUG )
                Printf("Channel %u Auto Gain success. Gain=%x, ADC=%u, target=%u\n", _bOrder,_bGain,_wData_v1v2[_bOrder],_wADCTarget[_bOrder]);  
                #endif
             }
          }
          else
          {
             bCaliNG =1;
             #if 1  //AUTO_COLOR_DEBUG
             Printf("ON_CHIP_AUTO_COLOR fail at mode=%u\n",bMode);
             #endif
          }
       }
       if(bCaliNG==0)
	{
         _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_FINISH;
	}
	else
	{
         if(bMode==ON_CHIP_GAIN_MODE)
         {
            vDrvOnChipModeOnOff(0); //turn off "on chip auto color" mode
         }
         //vRegWriteFldAlign(HDTV_03, AS_BLANK_RESET, HDTV_BLAK_SET);
         vDrvPreSetToReadBlank(AS_BLANK_RESET);
         _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_END; 
	}
	break;

    case ON_CHIP_AUTO_COLOR_FINISH: 
      #if ON_CHIP_AUTO_COLOR_DEBUG
        if(bMode==OFFSET_MODE)
        {
          if(_bReadMAXLevel==1)
	  {
             _bReadMAXLevel = 0;
             vDrvOnChipGetADCMaxValue();
	     Printf("Before auto color Max1=%3u, Max2=%3u, Max3=%3u\n",_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
	  }
        }
        else
      #endif
        {
          if(bMode== ON_CHIP_GAIN_MODE)
	  {
          vDrvOnChipModeOnOff(0); //turn off "on chip auto color" mode
           if (_bEEPROM_ready==1)
	  {
	    UNUSED(fgApiEepromWriteByte(EEP_ON_CHIP_AUTO_COLOR_GAIN_DONE,0xAA));
	    //UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_DONE,SV_TRUE));
	    //UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_HDTV_DONE,SV_TRUE));
	    //UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_VGA_COMP_DONE,SV_TRUE));
	    //UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_COLOR_SCART_DONE,SV_TRUE));
	    }
	    vDrvOnChipGainMapping(_bType);
	    #if ON_CHIP_AUTO_COLOR_DEBUG
	    vDrvCheckGain();
	    #endif
	  }
       }
       //vRegWriteFldAlign(HDTV_03, AS_BLANK_RESET, HDTV_BLAK_SET);
        vDrvPreSetToReadBlank(AS_BLANK_RESET);
       _OnChipAutoColorState = ON_CHIP_AUTO_COLOR_END; 
	break;

    case ON_CHIP_AUTO_COLOR_END:
      default:
      break;
    }
  }
}



void vDrvEnableADCLinearityVerify(UINT8 bType)
{
#if CHK_ADC_LINEARITY
     vDrvAsyncPreDataActive(0x20, 0x20);  //vDrvSetMeasureWindow(0xff);
     vDrvClearMAXvariable();  // to clear _dwMaxLevelAvg[];
     _bLocation = 0;
     _bADCLinearity = 0;
     _bADC_LINEARITY_state = ADC_VERIFY_3;
     TOTAL_POINT = bType;
#endif     
}

void vDrvADCLinearityVerify(void)
{
#if CHK_ADC_LINEARITY
   if((! fgIsSP0FlgSet(SP0_AUTOCOLOR_FLG)) || (!(_bAutoColorState0==VDO_AUTO_COLOR_NOT_BEGIN)))
   {
      return;
   }

   _bType = bDrvOnChipAutoColorCheckSignalReady();
   if(_bType==INT_VGA)
   {
        switch (_bADC_LINEARITY_state)
        {
             case ADC_VERIFY_0:
                 break;

             case ADC_VERIFY_1:
               vClrSP0Flg(SP0_AUTOCOLOR_FLG);
               if(_bAutoColorTimeOutCNT<0x10)
               {
                     vDrvGetMaxValue16Times();
                     //Printf("TimeOutCNT=%u, _wData1=0x%x, _wData2=0x%x, _wData3=0x%x\n",_bAutoColorTimeOutCNT  ,_wData_v1v2[0],_wData_v1v2[1],_wData_v1v2[2]);
                     _bAutoColorTimeOutCNT ++;
               }
               else
               {
                     _bLocation = 0;
                     _dwAutoColorMax[0] = _dwMaxLevelAvg[0];
                     _dwAutoColorMax[1] = _dwMaxLevelAvg[1];
                     _dwAutoColorMax[2] = _dwMaxLevelAvg[2];
                     _bADC_LINEARITY_state = ADC_VERIFY_2;  
                     _bADCLinearity =1;
                     Printf("---- The top level of 3 CH after auto color is: %d %d %d\n",_dwAutoColorMax[0] ,_dwAutoColorMax[1] ,_dwAutoColorMax[2] );
               }
             break;

             case ADC_VERIFY_2:
                 vClrSP0Flg(SP0_AUTOCOLOR_FLG);
                 vDrvClearMAXvariable();  // to clear _dwMaxLevelAvg[];
                 vDrvSetMeasureWindow(_bLocation);  // to set measure window
                _bADC_LINEARITY_state = ADC_VERIFY_3;
                break;
        
             case ADC_VERIFY_3:
                 vClrSP0Flg(SP0_AUTOCOLOR_FLG);
                 vDrvGetMaxValue16Times();  // to do dumy read
                 vDrvClearMAXvariable();  // to clear _dwMaxLevelAvg[];
                 _bAutoColorTimeOutCNT= 0;

                 if (_bADCLinearity==0)
                 {
                      _bADC_LINEARITY_state = ADC_VERIFY_1;
                 }
                 else if(_bADCLinearity==1)
                 {
                      _bADC_LINEARITY_state = ADC_VERIFY_4;
                 }
                 else if((_bADCLinearity==2))
                 {
                      _bADC_LINEARITY_state = ADC_VERIFY_5;
                 }
                 else
                 {
                      _bADC_LINEARITY_state = ADC_VERIFY_6;
                 }
                 break;
        
             case ADC_VERIFY_4:
                 vClrSP0Flg(SP0_AUTOCOLOR_FLG);
                 if(_bAutoColorTimeOutCNT<0x10)
                 {
                     vDrvGetMaxValue16Times();
                     _bAutoColorTimeOutCNT ++;
                 }
                 else
                 {
                     Printf("The level of 3 CH at location %2d are CH1: %4d ,CH2: %4d ,CH3: %4d\n",_bLocation,_dwMaxLevelAvg[0] ,_dwMaxLevelAvg[1] ,_dwMaxLevelAvg[2] );
                     _dwMaxDiffTemp =0;
                     for (_bOrder=0; _bOrder<3; _bOrder++)   
                     {
                           _dwMaxLevelAvg[_bOrder] = _dwAutoColorMax[_bOrder] - _dwMaxLevelAvg[_bOrder];  // to get the difference between MAX and local max
                           
                           if(_dwMaxDiffTemp < _dwMaxLevelAvg[_bOrder])     
                           {
                                  _dwMaxDiffTemp = _dwMaxLevelAvg[_bOrder];  // to get the max difference between MAX and local max
                           }
                     }
                     
                     _dwMaxDiffLocal = 0;
                     for (_bOrder=0; _bOrder<3; _bOrder++)
                     {
                           _dwMaxLevelAvg[_bOrder] = _dwMaxDiffTemp - _dwMaxLevelAvg[_bOrder];  //to calculate the difference between channel and channel

                           if (_dwMaxDiffLocal < _dwMaxLevelAvg[_bOrder] )
                           {
                               _dwMaxDiffLocal = _dwMaxLevelAvg[_bOrder] ;    // to get the max difference between channel and channel
                           }
                     }
                     
                     _dwMaxDiff[_bLocation] = _dwMaxDiffLocal;
                     _bLocation = _bLocation++;
                     if(_bLocation<TOTAL_POINT)
                     {
                           _bADC_LINEARITY_state = ADC_VERIFY_2;
                     }
                     else
                     {
                          //vDrvAsyncPreDataActive(10,10);
                          _dwMaxDiffLocal = 0;
                          Printf("The channel to channel different at position1,2,3,4... are \n");
                          for (_bOrder=0; _bOrder<TOTAL_POINT; _bOrder++)
                          {
                                if(_dwMaxDiffLocal < _dwMaxDiff[_bOrder])
                                {
                                      _dwMaxDiffLocal = _dwMaxDiff[_bOrder];
                                }
                           Printf("Position %2d = %3d\n",_bOrder,_dwMaxDiff[_bOrder]);
                           }
                           Printf("The maximum code difference (12 bit) between channel to channel is %d\n", _dwMaxDiffLocal);
                           _bLocation = 0;
                           _bADCLinearity =2;
                           _bADC_LINEARITY_state = ADC_VERIFY_2;
                      }
                  }

             break;
        
             case ADC_VERIFY_5:
                 vClrSP0Flg(SP0_AUTOCOLOR_FLG);
                 if(_bAutoColorTimeOutCNT<0x10)
                 {
                     vDrvGetMaxValue16Times();
                     _bAutoColorTimeOutCNT ++;
                 }
                 else
                 {
                     if(_bLocation==0)
                     {
                          _dwStPoint[0] = _dwMaxLevelAvg[0];
                          _dwStPoint[1] = _dwMaxLevelAvg[1];
                          _dwStPoint[2] = _dwMaxLevelAvg[2];
                          #if PGA_LINEARITY_DEBUG
                          Printf("Start Point= %u , %u , %u\n",_dwStPoint[0],_dwStPoint[1],_dwStPoint[2]);
                          #endif
                          _bLocation = TOTAL_POINT-2;
                          _bADC_LINEARITY_state = ADC_VERIFY_2;  
                     }
                     else if(_bLocation==(TOTAL_POINT-2))
                     {
                          for(_bOrder = 0; _bOrder <3; _bOrder++)
                          {
                               _dwEndPoint[_bOrder] = _dwMaxLevelAvg[_bOrder];
                               _dwDeltaValue[_bOrder] = ((_dwStPoint[_bOrder] - _dwEndPoint[_bOrder])+(TOTAL_POINT/2-1))/(TOTAL_POINT-2);
                          }
                          #if PGA_LINEARITY_DEBUG
                          Printf("End Point= %u , %u , %u\n",_dwEndPoint[0],_dwEndPoint[1],_dwEndPoint[2]);
                          #endif
                          _bLocation = 0;  //_bLocation -1;
                          _bADCLinearity =3;
                          _bADC_LINEARITY_state = ADC_VERIFY_2; 
                     }
                 }
                 break;
             case ADC_VERIFY_6:
                 vClrSP0Flg(SP0_AUTOCOLOR_FLG);
                 if(_bAutoColorTimeOutCNT<0x10)
                 {
                     vDrvGetMaxValue16Times();
                     _bAutoColorTimeOutCNT ++;
                 }
                 else
                 {
                     for (_bOrder=0; _bOrder<3; _bOrder++)   
                     {
                           _dwMaxDiffTemp = _dwDeltaValue[_bOrder]*(TOTAL_POINT-2-_bLocation)+_dwEndPoint[_bOrder];
                           #if PGA_LINEARITY_DEBUG
                           Printf("Channel= %u ,point %u  target level= %u ,Current level= %u\n",_bOrder,_bLocation,_dwMaxDiffTemp,_dwMaxLevelAvg[_bOrder]);
                           #endif
                           if(_dwMaxDiffTemp> _dwMaxLevelAvg[_bOrder])
                           {
                              _dwMaxDiff[_bOrder] = _dwMaxDiffTemp - _dwMaxLevelAvg[_bOrder];
                           }
                           else
                           {
                              _dwMaxDiff[_bOrder] = _dwMaxLevelAvg[_bOrder] -_dwMaxDiffTemp;
                           }
                      }
                      Printf("The channel different at position %2u is %4u , %4u , %4u\n",_bLocation, _dwMaxDiff[0],_dwMaxDiff[1],_dwMaxDiff[2]);

                      _bLocation = _bLocation+1;
                      if(_bLocation < (TOTAL_POINT-1))
                      {
                           _bADC_LINEARITY_state = ADC_VERIFY_2; 
                      }
                      else
                      {
                           vDrvAsyncPreDataActive(0x20, 0x20);
                           _bADC_LINEARITY_state = ADC_VERIFY_0;
                      }
                }
                break;
        }
    }
    else
    {
         _bADC_LINEARITY_state = ADC_VERIFY_0;
    }
#endif
}

#endif
