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
 * $RCSfile: drv_autocolor.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_AUTOCOLOR_H_
#define _DRV_AUTOCOLOR_H_
#ifndef CC_COPLAT_MT82
#define EEPROM_READY   1
#else
#define EEPROM_READY  1//kal8225
#endif
#define EEPROM_USAGE_debug 0


//define the IC verification functions
#define CHK_BLANK_VARIATION 0
#define CHK_PGA_LINEARITY 0 // 1
#define CHK_ADC_LINEARITY 0
#define CHK_GAIN_OFFSET_ITERATION 0 // 1
#define PGA_4bit_offset 0   //test for next generation IC




#define FULLY_HW_AUTO_CALIBRATION   1  //fully total cancel blank level shift by H/W
//#define DIGITAL_CALIBRATION   1
//#define ANALOG_CALIBRATION   1
#ifdef FULLY_HW_AUTO_CALIBRATION
    #define HW_MEASURE_BLANK      0   // for read blank level during fixed period to solve VSAG problem
    #define DIGITAL_NEW_OFFSET    1   //using digital line average method must make sure the analog signal is >0
    //#define E_FUSE_DO_OFFSET_CALIBRATION 1
    #define COMPENSATE_GAIN         0
    #if COMPENSATE_GAIN
         #define COMPENSATE_BY_ANALOG_GAIN 0 //default to use digital gain to do compensation
    #endif
#elif  defined (DIGITAL_CALIBRATION)
    #define HW_MEASURE_BLANK      1   //for reading blank level during fixed period to solve VSAG problem
    #define DIGITAL_NEW_OFFSET    1   //for reducing analog offset resolution to save chip area
    //#define E_FUSE_DO_OFFSET_CALIBRATION 0  // I did not test the case of this define=1
    #define COMPENSATE_GAIN         0                   // I did not test the case of this define=1
    #if COMPENSATE_GAIN
         #define COMPENSATE_BY_ANALOG_GAIN 0 //default to use digital gain to do compensation
    #endif
#elif  defined(ANALOG_CALIBRATION   )
    #define HW_MEASURE_BLANK      0   //for reading blank level during fixed period to solve VSAG problem
    #define DIGITAL_NEW_OFFSET    0   //for reducing analog offset resolution to save chip area
    //#define E_FUSE_DO_OFFSET_CALIBRATION 0  // I did not test the case of this define=1
    #define COMPENSATE_GAIN         0                   // I did not test the case of this define=1
    #if COMPENSATE_GAIN
         #define COMPENSATE_BY_ANALOG_GAIN 0 //default to use digital gain to do compensation
    #endif
#endif

/******************************************************************************
 * Header Files
 *****************************************************************************/
#include "typedef.h"

/******************************************************************************
 * Macro, Type Definition, and Variable Declaratoin
 *****************************************************************************/
extern void vDrvIntAutoColorStart(void) ;
void bDrvSetHDADCGain(UINT8 bOrder,UINT8 value);
void bDrvSetHDADCOffset(UINT8 bOrder,UINT8 value);

extern UINT8   _OnChipAutoColorState;
extern UINT8   _bAutoColorState0;
//extern UINT8   _bAutoColorIsr;
extern UINT8   _bGainOffset_channel;
extern UINT16  _wData_v1v2[3];
extern UINT8   _bMode;  //mtest_0110
extern UINT8   _bType;
extern UINT8  _bEEPROM_ready;

extern UINT8 bDrvOnChipGetVFESignalType(UINT8 bAutoInput);
extern UINT8 bDrvOnChipAutoColorCheckSignalReady(void);
extern UINT16 bDrvGetBlankVal(UINT8 bOrder);
extern UINT8 bDvMeasureDataSel(void);
extern UINT8 bDrvOnChipGetADCMaxMinValue(UINT8 bOrder, UINT8 bMode);
extern void vDrvOnChipAutoColor_GainOffset(UINT8 bMode); //for ON_CHIP_AUTO_COLOR
//extern void vChkOnChipOffsetEEPROM(void);
extern void vDrvOnChipAutoColorIteration(void);
extern void vDrvAdjustBlankLevel(void);
extern void vDrvEnableBlankLevelAdjust(void);
extern void vDrvEnableBlankLevelMeasure(void);
extern void vDrvEnableADCLinearityVerify(UINT8 bType);
extern void vDrvEnablePGALinearityVerify(void);
extern void vDrvPGALinearityVerify(void);
extern void vDrvADCLinearityVerify(void);
extern void vDrvHDTVMeasureSetting(UINT8 bField_Number);
extern void  vDrvHDTVClampMethodDefaultSetting(UINT8 bType);
extern void vDrvEnableVGARefVoltageVerify(UINT8 bType);
extern void vDrvEnableSyncSlicerRefVoltageVerify(UINT8 bType);
#ifdef FULLY_HW_AUTO_CALIBRATION
extern void vDrvHDTV_HW_AUTO_ONOFF(UINT8 bMode, UINT8 bType);
#endif
//extern void vDrvPreSetToReadBlank(UINT8 bOnOff);

#define BLANK_RESET 0x08 //rock20060220
#define BLANK_HOLD 0x04
#define BLANK_ALWAYS 0x00
#define vDrvSetBlankStatus(bSta)	vWriteVDOINMsk(HDTV04, bSta, 0x0c, 1)
//#define vDrvSetBlankHold(bEnable)	vWriteVDOINMsk(HDTV04, (bEnable <<2), 0x04, 1)
#define vDrvClrSP1DE() 		vWriteVDOINMsk(VSRC00, 0x00, 0x10, 3)
#define vSetAutoClrState(bState)	(_bAutoColorState0 = bState)
#define bGetAutoClrState()	(_bAutoColorState0)
extern UINT8   _bAutoColorState0;
extern UINT8  _bAutoColorGain[4][3];
extern UINT8  _bAutoColorGain_for_EFuse[4][3];
extern UINT8  _bAutoColorOffset[4][3];
extern UINT8  _bAutoColorHistory[4];
extern UINT32 _dwBlankAvg_1[3]; //blank level for ch1, ch2, ch3
extern UINT32 _dwMaxLevelAvg_1[3];//maximum level for ch1, ch2, ch3

//#ifdef FULLY_HW_AUTO_CALIBRATION
//    #if E_FUSE_DO_OFFSET_CALIBRATION
         extern UINT8 _bOffsetCaliDone[4];
//    #endif
//#endif

#if (CHK_PGA_LINEARITY ||CHK_ADC_LINEARITY||CHK_GAIN_OFFSET_ITERATION||CHK_BLANK_VARIATION)
extern  UINT8 _bVFE_VERIFY_CASE;
#endif

extern void vDrvIntAutoColorState(void) ;

extern void vDrvExtAutoColorStart(void) ;
extern void vDrvExtAutoColorLoad(void) ;
extern void vDrvExtAutoColorState(void) ;

enum
{
Pixel_16_per_line,
Pixel_32_per_line,
Pixel_64_per_line,
Pixel_128_per_line
};

enum
{
Start_line_0,
Start_Lline_8,
Start_line_16,
Start_line_24,
Start_line_32,
Start_line_40,
Start_line_48,
Start_line_56,
Start_line_64,
Start_line_72,
Start_line_80,
Start_line_88,
Start_line_96,
Start_line_104,
Start_line_112,
Start_line_120,
Start_line_128,
};

enum
{
Per_1_line,
Per_2_line,
Per_4_line,
Per_8_line,
Per_16_line,
Per_32_line,
Per_64_line,
Per_128_line
};
enum
{
Average_128_line,
Average_256_line,
Average_512_line,
Average_1024_line
};

enum
{
UPDATE_PER_1_FIELD,
UPDATE_PER_2_FIELD,
UPDATE_PER_4_FIELD,
UPDATE_PER_8_FIELD
};

enum
{
AFTER_ADJ,
BEFORE_ADJ
};

enum
{
CALI_DISABLE,
CALI_ENABLE
};
#endif
