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
 * $RCSfile: drv_vdoclk.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


#ifndef _VDO_CLK_H_
#define _VDO_CLK_H_

#include "typedef.h"
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "hw_vfe.h"

// PLL related define macros.
#ifdef CC_UP8032_ATV
//#define EnableSDDSCalibration  0
#define EnableDDDSCalibration  0
#else
//#define EnableSDDSCalibration  1
#define EnableDDDSCalibration  1
#endif

#define ENABLE_IC_TEST_MODULE 0

#if ENABLE_IC_TEST_MODULE //slt_yl
#define EnableSDDSCalibration  1
#else
#define EnableSDDSCalibration  0
#endif
#define USE_MT8202_PLL_SETTING              0
#define THERMAL_COMPENSATION_ENABLE 0
#define CHANGE_SDDS_KPI 1            //Michael 20090123 ***

#define NEW_PHASE_ALGORITHM    1
#if NEW_PHASE_ALGORITHM         // Phase3Channel can be 1 or 0 only when NEW_PHASE_ALGORITHM=1
#define VGA_HW_AUTO	  0 // please always fix to 0
#define Phase3Channel       1 //0

   #if Phase3Channel
        #define CHANNEL_NUM  3
   #else
        #define CHANNEL_NUM  1
   #endif
   #define MAX_PHASE_ARRAY  33  
    extern void vYPbPrPhaseIsr_New(void) ; //YPbPr Auto Phase 2006/11/07
    extern void vVgaPhaseIsr_New(void) ;
    extern void vDrvInitPhaseVar(void);
    extern UINT32	 _dBestSumALL[CHANNEL_NUM];
    //extern UINT8   _bMaxSlope[CHANNEL_NUM];
    extern UINT8   _bChBestPhase[CHANNEL_NUM];
    extern UINT32	 _dSumTmpPhase[CHANNEL_NUM][MAX_PHASE_ARRAY];
    extern UINT32 _dwPhase3CH[CHANNEL_NUM];
#else
#define VGA_HW_AUTO	1 // 1
#define Phase3Channel     0 // please always fix to 0
#endif
 extern UINT32	_dSumTmp[3];


void vDrvSetDviPll(UINT8 u1Mode) ;
void vDrvClkInit(void);
void vDrvSetVoSpreSTEP(UINT32 bVal);
void vDrvSlicerCheck(void); //for slicer CLI
void vDrvSDDSCWCheck(INT32 i4Argc, UINT16 KP0);
UINT32 vDrvReadVoSpreSTEP(void);
void vDrvSetVOSpread(UINT32 u4Permillage);
UINT32 DDDS_GetSpreadSpectrum(void);
UINT32 DDDS_GetSpreadSpectrumFreq(void);
void DDDS_SetSpreadSpectrum(UINT32 u4Freq, UINT32 u4Range);
void SetSpreadSpectrumOnOff(UINT8 Onoff);
void vDrvVOFreqSet(UINT32 u4DCLK);
void vDrvSetSSMDRAsInputHsync(void);//for dynamic MDR 
void vDrvVGASetClock(UINT16 wHtotal);
void vDrvTVADCPLLIni(void);
void vDrvVOPLLPWUPIni(void);
void vDrvVOPLLSet(UINT32 u4CLK);
void vDrvVOPLLAutoKVCOBand(void);
void vThermalCompInit(void);
void vDrvThermalCompensate(void);
UINT8 vAdcpllCurPhaseDelay(void);
void vDrvAdcpllSetPhaseDelay(UINT8 bVal);
UINT8 vCurTemperature(void);
void vDrvCLKINDefaultSet(void) ;
void vDrvCLKIN2DefaultSet(void) ;
extern void vDrvADCPLLSetFreq(UINT8 bADCCLK) ;
extern void vDrvCLKINSetHtotal(UINT16 wHtotal) ;
extern void vDrvCLKINSetHsyncWid(UINT8 wWidth) ;
extern void vDrvADCPLLSet(UINT16 wPixClk, UINT16 wHtotal) ;
extern void vDrvADCPLLMinPower(UINT8 bPow) ;
extern void vDrvADCPLLPower(UINT8 bPow) ;
//extern void vDrvEnable_SDDS_Calibration(UINT8 bKP0_START,UINT8 bKP0_END, UINT8 bKI0_START, UINT8 bKI0_END);//lili
extern void vDrvSDDS_Response(void);
extern void vDrvDDDS_Response(void);
extern void SDDS_wait_message(void);
extern void DDDS_wait_message(void);
#if CHANGE_SDDS_KPI
extern void vDrvEnableChang_SDDS_BW(void);
extern void Set_SDDS_KPI(UINT8 bValue);
extern void vDrvClkSetLockBandwidth(UINT8 bIsLock);
#endif
extern void vDrvAllADCPLLPow(UINT8 bPow);

#if EnableSDDSCalibration
extern UINT8 _bSDDSCaliState;
#endif

extern UINT8   _bCLKByte3;
extern UINT8   _bCLKByte2;
extern UINT8   _bCLKByte1;
extern UINT8   _bCLKSetFlag;

extern UINT8   _bGetCLKByte3;
extern UINT8   _bGetCLKByte2;
extern UINT8   _bGetCLKByte1;
extern UINT8   _bGetCLKByte0;

enum
{
  IntClk,
  ExtClk
};

enum
{
  DCLK_IN_SOG,
  DCLK_IN_HSYNC
};

// define the Thermal compensate state machine
enum 
{
	VDO_THERMALCOM_INI,
	VDO_THERMALCOM_STATE1,
	VDO_THERMALCOM_STATE2,
	VDO_THERMALCOM_STATE3
};

#define vDrvCLKINSyncSel(bSrc) 	vIO32WriteFldAlign(CKGEN_AFECFG7, bSrc, FLD_RG_ADCPLL_SDDS_HSYNC_SEL)
#define vDrvCLKINHsyncPol(bPol) vIO32WriteFldAlign(CKGEN_AFECFG7, bPol, FLD_RG_ADCPLL_SDDS_HSYNC_POL)
#define vDrvADCPLLSetRelatch(bPol) vIO32WriteFldAlign(CKGEN_AFECFG7, bPol, FLD_RG_ADCPLL_SDDS_RSYNC_POL)

//VFE_14
#define vDrvCLKINFreeRun(bEna) vRegWriteFldAlign(VFE_17, bEna, DCLK_INIT)
#define vDrvCLKINDclkRST(bEna) vRegWriteFldAlign(VFE_17, bEna, DCLK_RESETB)
//#define vDrvCLKINPsncoRST(bEna) vRegWriteFldAlign(VFE_14, bEna, PSNCO_RST) //MT5387 don't have this bit
#define vDrvCLKINMaxPeriod(bVal) vRegWriteFldAlign(VFE_14, bVal, MAX_PERIOD)
#define vDrvCLKINFMPeriod(bVal) vRegWriteFldAlign(VFE_14, bVal, FM_PERIOD)
#define vDrvCLKINFreeHsyncPol(bPol) vRegWriteFldAlign(VFE_17, bPol, HSYNC_POL_VFE)
#define vDrvCLKINSetLockThr(bVal) vRegWriteFldAlign(VFE_14, bVal, LOCK_THR)
#define vDrvCLKINSetKI0(bVal) vRegWriteFldAlign(VFE_14, bVal, DCLK1_KI_0)
#define vDrvCLKINSetKI1(bVal) vRegWriteFldAlign(VFE_14, bVal, DCLK1_KI_1)
#define vDrvCLKINSetLockCnt(bVal) vRegWriteFldAlign(VFE_14, bVal, LOCK_CNT)
#define vDrvCLKINSetKP0(bVal) vRegWriteFldAlign(VFE_14, bVal, DCLK1_KP_0)
#define vDrvCLKINSetKP1(bVal) vRegWriteFldAlign(VFE_14, bVal, DCLK1_KP_1)
#define vDrvCLKINSetErrLim(bVal) vRegWriteFldAlign(VFE_14, bVal, ERR_LIM)

//VFE_15
#define vDrvCLKINDCLKFast(bEna) vRegWriteFldAlign(VFE_15, bEna, DCLK_FAST_LF)
#define vDrvCLKINSetCW(bVal) vRegWriteFldAlign(VFE_15, bVal, DCLK_FREQ_CW)

//VFE_16
#define vDrvCLKINDCLKPFDSEL(bEna) vRegWriteFldAlign(VFE_14, bEna, DCLK_PFD_SEL)

//VFE_17
//#define vDMSS_TEST_DDS_CW(bVal) vRegWriteFldAlign(VFE_17, bVal, DMSS_TEST)
#define vDrvVoDclkSpreSpecDir(bVal) vRegWriteFldAlign(OUTSTG_OS_E0, bVal, OS_E0_SPREAD_DIR)
#define vDrvVoDclkSpreSpecSTEP(bVal) vRegWriteFldAlign(OUTSTG_OS_E1, bVal, OS_E1_SPREAD_STEP)

//VFE_18
#define vDrvVoDclkSpreadInit(bVal) vRegWriteFldAlign(OUTSTG_OS_E0, bVal, OS_E0_SPREAD_INIT)
//#define vDrvDMSSSpreadReset(bVal) vRegWriteFldAlign(VFE_18, bVal, RESET_DMSPREAD)
#define vDrvCLKINSetSecKI0(bVal) vRegWriteFldAlign(VFE_17, bVal, DCLK2_KI_0)
#define vDrvCLKINSetSecKI1(bVal) vRegWriteFldAlign(VFE_17, bVal, DCLK2_KI_1)
#define vDrvCLKINSetSecKP0(bVal) vRegWriteFldAlign(VFE_17, bVal, DCLK2_KP_0)
#define vDrvCLKINSetSecKP1(bVal) vRegWriteFldAlign(VFE_17, bVal, DCLK2_KP_1)
#define vDrvVoDclkSpreSpecPeriod(bVal) vRegWriteFldAlign(OUTSTG_OS_E0, bVal, OS_E0_SPREAD_PERIOD) 

//VFE_19
//#define vDrvDMSSSpreadInit(bVal) vRegWriteFldAlign(VFE_19, bVal, DMSPREAD_INIT)
//#define vDrvDMSSSpreSpecDir(bVal) vRegWriteFldAlign(VFE_19, bVal, DMSPREAD_DIR)
#define vDrvVoDDDSRST(bVal) vRegWriteFldAlign(OUTSTG_OS_E0, bVal, OS_E0_DDDS_RESETB)
#define vDrvVoDDDSCW(bVal) vRegWriteFldAlign(OUTSTG_OS_E2, bVal, OS_E2_DDDS_FREQ_CW)

//VFE_1A
//#define vDrvDMSSCW(bVal) vRegWriteFldAlign(VFE_1A, bVal, C_DMSPREAD_CW)
//#define vDrvDMSSSpreadSpecPeriod(bVal) vRegWriteFldAlign(VFE_1A, bVal, C_DMSPREAD_PERIOD)
//#define vDrvDMSSSpreadSpecStep(bVal) vRegWriteFldAlign(VFE_1A, bVal, C_DMSPREAD_STEP)

#endif
