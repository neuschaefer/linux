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
 * $RCSfile: drv_adcset.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
#ifndef _ADCSET_H_
#define _ADCSET_H_ 


/******************************************************************************
 * Header Files
 *****************************************************************************/
#include "typedef.h"
#include "feature.h"
#include "hw_vfe.h"

#define ADAPTIVE_SLICER_TSET 1
#define CLI_CVBS 1
#define PD_ADC_ENABLE 1

//===================================================================
//  VFE GPIO Debug Monitor
//===================================================================
#define VFE_GPIODEBUG 0
#if VFE_GPIODEBUG
// MT5388M1V1 Setting Pin setting refer driver/ckgen/Pinmux.c
#define VFE_GPIONum 4
#define VFE_GPIO_00 83 // O0N(GPIO83) NJP1 Pin01
#define VFE_GPIO_01 82 // O0P(GPIO82) NJP1 Pin02 
#define VFE_GPIO_02 85 // O1N(GPIO85) NJP1 Pin03
#define VFE_GPIO_03 84 // O1P(GPIO84) NJP1 Pin04
INT32 VFE_GPIOPINTbl[VFE_GPIONum]={VFE_GPIO_00,VFE_GPIO_01,VFE_GPIO_02,VFE_GPIO_03};
#endif

enum {
	DISABLE,
	ENABLE
};
enum {
	SPREAD_CENTER,
	SPREAD_DOWN,
	SPREAD_UP
};
enum {
	POWER_ON,
	POWER_DOWN
};
enum {
	CLK_OF_CLKIN,
	CLK_OF_XTAL
};
enum {
	ADC0_CH0,
	ADC0_CH1
};

enum {
    CLAMP_OF_ALWAYS,
    CLAMP_OF_TVD3D,
    CLAMP_OF_HDTV,
    CLAMP_OF_VGA
};
enum {
    ADAPTIVE_SLICER_INI,
    ADAPTIVE_SLICER_STATE0,
    ADAPTIVE_SLICER_STATE1,
    ADAPTIVE_SLICER_STATE2,
    ADAPTIVE_SLICER_STATE3,
    ADAPTIVE_SLICER_STATE4,
    ADAPTIVE_SLICER_STATE5,
    ADAPTIVE_SLICER_STATE6,
    ADAPTIVE_SLICER_STATE7
};
enum {
    IPS_OTHER,		//input source don't use AD
    IPS_ATV,		//input source is ATV
    IPS_ATD,		//input source is ATD
    IPS_DTV,		//input source is DTV
    IPS_DTD,		//input source is DTD
    IPS_CVBS_A,		//input source is CVBS_A
    IPS_CVBS_B,		//input source is CVBS_B
    IPS_CVBS_A_B,		//input source is CVBS_A/CVBS_B
    IPS_MAX		//end of enum
};

//************************************************************************
//TVD AGC, 0x47 set Ch0 gain control from AGC_TVD3D
#define TVD3D_AGC 0x544
#define vDrvPGA0(bVAL) vWriteVDOINMsk(TVD3D_AGC,bVAL , 0xFF, 2)
#define vDrvPGA0A(bVAL) vWriteVDOINMsk(TVD3D_AGC,bVAL , 0xFF, 2)


extern UINT8 bAdaptiveSlicerStateMachine;	//set initial value in mode detect done

//********Function define ************************************/

void vDrvSetHDTVMux(UINT8 bVDOINPUT) ;
void vDrvSVdoEnable(void) ;
void vDrvSVdoDisable(void) ;
void vDrvBypPD(void);
void vDrvAvByp(void);
void vDrvSVdoByp(void);
void vDrvSetTVADC(UINT8 bVDOINPUT) ;
void vDrvADCDefaultSetting(void) ;
#if CLI_CVBS
void vDrvCVBSPGACheck(void);
void vDrvTargetBlankCheck(void);
void vDrvIOOffsetBlankCheck(void);
void vDrvPeakLimitCheck(UINT8 PYC);
void vDrvSVFTest(void);
#endif
void vDrvCVBSOffsetTest(void);
void vDrvADCOffsetCal(void) ;
void vDrvTVADCDefaultSetting(void) ;
void vDrvTVADCOffsetCal(void) ;
void vDrvCvbsAdcPD(void) ;
void vDrvCvbsVfePD(void) ;
void vDrvCvbsAdcPWON(void) ;
void vDrvCvbsVfePWON(void) ;
void vDrvCvbsAdcCal(void) ;
void vDrvHDTVADCDefaultSetting(void) ;
void vDrvHDTVADCOffsetCalDo(UINT8 bType) ;
void vDrvVGAPD(void);
void vDrvVGAPWON(void);
UINT8 bDrvHDADCGetVal(UINT8 bADC) ;

void vDrvSetHDTVADC(UINT8 bVDOINPUT) ;
void vDrvSetHDTVMux(UINT8 bVDOINPUT) ;
void vDrvHDTVADCOffsetCal(void) ;
void vDrvSetHDADCGainSrcDef(void) ;
void vDrvAllHDADCPow(UINT8 bPow) ;
UINT16 vDrvReadADC0(UINT8 bCH) ;

void vDrvReadADC0Register(void) ;

void vDrvTVADCOffsetCal(void) ;
void vDrvTVADC0CH0ClampOnBlankOffsetCal(void) ;
void vDrvTVADC0CH1ClampOnBottomOffsetCal(void) ;
#ifdef CC_SUPPORT_SIF_BYPASS
BOOL vDrvSetSIFADC(void);
#endif
#ifdef CC_MT5365_V2
#else
void vDrvInputOffsetCal(void) ;
#endif
UINT16 wDrvOutputOffsetCal(UINT8 bCH,UINT16 wCLAMP) ;
void vDrvDFE(UINT16 wTARGET_BLANK) ;
#ifdef CC_CLI
void vDrvTVDBlankLevel(UINT16 wTARGET_BLANK) ;
#endif
void vDrvCalibrationDisable(UINT8 bCH, UINT8 bCLAMP) ;
void vDrvCalibrationEnable(UINT8 bCH, UINT8 bCLAMP) ;
void vDrvAAFCalibration(void) ;


void vCust_R_Offset_Update(UINT8 bOffset) ;
void vCust_G_Offset_Update(UINT8 bOffset) ;
void vCust_B_Offset_Update(UINT8 bOffset) ;
void vCust_R_Gain_Update(UINT8 bGain) ;
void vCust_G_Gain_Update(UINT8 bGain) ;
void vCust_B_Gain_Update(UINT8 bGain) ;
void vCust_EBGain_OverRule_Update(UINT8 bNum) ; //kaloverrule
void vCust_Embedded_Gain(UINT8 bType); 
void vCust_Gain_OFFSet_Update(void);

UINT8 vCust_Current_Use_ADC_Read(UINT8 bType);
UINT8 vCust_R_Offset_Read(void) ;
UINT8 vCust_G_Offset_Read(void) ;
UINT8 vCust_B_Offset_Read(void) ;
UINT8 vCust_R_Gain_Read(void) ;
UINT8 vCust_G_Gain_Read(void) ;
UINT8 vCust_B_Gain_Read(void) ;
UINT8 vCust_R_MAX_Read(void);
UINT8 vCust_G_MAX_Read(void);
UINT8 vCust_B_MAX_Read(void);
UINT16 vCust_B_Blank_Read(void);
UINT16 vCust_G_Blank_Read(void);
UINT16 vCust_R_Blank_Read(void);
UINT16 vCust_B_Max_Read(void);
UINT16 vCust_G_Max_Read(void);
UINT16 vCust_R_Max_Read(void);
UINT8 vCust_EBGain_OverRule_Read(void) ; //kaloverrule

void vCust_R_MAX_Update(UINT8 value);
void vCust_G_MAX_Update(UINT8 value);
void vCust_B_MAX_Update(UINT8 value);
void vCust_clear_eeprom(UINT8 bValue);
void vCust_SOGY_VTH_Set(UINT32 dwLevel);
void vCust_SOGY_VTL_Set(UINT32 dwLevel);
void MTAL_SOGY_VTH_Set(UINT8 bLevel);
void MTAL_SOGY_VTL_Set(UINT8 bLevel);
BOOL MTAL_SetPGAGainOffsetInitVal(void);


void vCust_AutoColor_EEPROM_Enable(UINT8 bEnable);
UINT8 vCust_EEPROM_VGA_B_Gain_Read(void);
UINT8 vCust_EEPROM_VGA_G_Gain_Read(void);
UINT8 vCust_EEPROM_VGA_R_Gain_Read(void);
UINT8 vCust_EEPROM_VGA_B_Offset_Read(void);
UINT8 vCust_EEPROM_VGA_G_Offset_Read(void);
UINT8 vCust_EEPROM_VGA_R_Offset_Read(void);
UINT8 vCust_EEPROM_CMP_Y_Gain_Read(void);
UINT8 vCust_EEPROM_CMP_PB_Gain_Read(void);
UINT8 vCust_EEPROM_CMP_PR_Gain_Read(void);
UINT8 vCust_EEPROM_CMP_Y_Offset_Read(void);
UINT8 vCust_EEPROM_CMP_PB_Offset_Read(void);
UINT8 vCust_EEPROM_CMP_PR_Offset_Read(void);
UINT8 vCust_EEPROM_SCART_B_Gain_Read(void);
UINT8 vCust_EEPROM_SCART_G_Gain_Read(void);
UINT8 vCust_EEPROM_SCART_R_Gain_Read(void);
UINT8 vCust_EEPROM_SCART_B_Offset_Read(void);
UINT8 vCust_EEPROM_SCART_G_Offset_Read(void);
UINT8 vCust_EEPROM_SCART_R_Offset_Read(void);
UINT8 vCust_EEPROM_CMP_STATUS(void);
UINT8 vCust_EEPROM_VGA_STATUS(void);
UINT8 vCust_EEPROM_SCART_STATUS(void);

extern void vTvd3dATDInit(UINT8 bATDOnOff);

void vDrvHDTVADCDefaultSetting(void);


/*    AD control safe  routine */
INT32 AD_AcquireControl(void);
INT32 AD_ReleaseControl(void);


/*	Scart Setting for drv_scart.c		*/
void vScartVfeConnect(void) ;
void vScartVfeDisconnect(void) ;
void vScartMix(void) ;
void vScartSvConnect(void) ;


void vDrvAdativeSlicerState(void);
void vVFESLT_slicer_test_verC(void);

UINT16 AD_QueryInputSource(void);

extern UINT8 _bPGAGainOffsetHistory[4];
extern UINT8 _bEFUSE_AUTOCOLOR_READY;


#if SUPPORT_SET_SLICER
extern void vSetSOGSlicer(UINT8 pair) ;//wu add
#endif


#if ADAPTIVE_SLICER_ENABLE
extern UINT8 check_quaity_state;
extern void  vSliceQuality536x(void);
extern void vSetDefaultSlicer(void) ;
extern void vNextSlicer(UINT16 wTimeout) ;
extern void vResetSliceTimer(void) ;
#if (defined(ADAPTIVE_SLICER_DEFAULT_SETTING) || defined(ADAPTIVE_MONITOR_SLICER_MEASURE))
extern UINT8 _bCurSlicerIdx;
extern UINT8 _bCurSlicerIdx_best;
#endif
#if ADAPTIVE_MONITOR_SLICER_MEASURE
extern void vSetMONSlicer(UINT8 pair);
extern void vResetVLenSP0(void);
extern void vResetVLenSP2(void);
extern void vSetMONSlicer_Matrix(void);
#if ADAPTIVE_MONITOR_SLICER_MEASURE_DEBUG
extern UINT8 bReadMONSlicer(void);
extern UINT8 bReadNewSlicer(void);
#endif
#endif

#endif

extern void vDrvSYNCSlicerWakeup(UINT8 bPow);


void vDrvSOY0EN(UINT8 bEn);
void vDrvSOY1EN(UINT8 bEn);
void vDrvSOGEN(UINT8 bEn);

#if VFE_GPIODEBUG
extern void GPIO_H(UINT8 u1GpioNumIdx);
extern void GPIO_L(UINT8 u1GpioNumIdx);
#endif

//ChB AP control API
void vDrvSetTVDCHBSrcMux(UINT8 value);
void vDrvSetTVDCHBGain(UINT8 value);

#endif


