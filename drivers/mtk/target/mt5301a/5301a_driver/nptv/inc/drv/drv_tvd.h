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
 /*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_tvd.h $
 *
 * Project: MT5381
 * --------
 *
 *
 * Description:
 * ------------
 *    TVD status macro
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/31 9:52p $
 *
 * $Revision: #1 $
*************************************************************************/
/**
* @file drv_tvd.h
*/
#ifndef _DRV_TVD_H_
#define _DRV_TVD_H_


/**************************************************************************
 * Header Files
 *************************************************************************/
#include "hw_tvd.h"
#include "typedef.h"
/**************************************************************************
 * Configure Definitions
 *************************************************************************/

#define TVD_NR_AVG                1
#define TVD_CLI_SUPPORT           1
#define TVD_AUTO_CALIB            1
#define TVD_FRS_FOR_NONSTDSIGNAL  1
#define TVD_CHECK_BURST  1
#define TVD_VPRES_STATE     1
#define TVD_NEW_VPORCH 1
#define TVD_VCR_BV 1
/**************************************************************************
 * Type definitions & Enum
 *************************************************************************/
/** Function prototype for CLI cmd. */
typedef void (*TVD_PFN_CLI_NFY) (void *pInput);

typedef struct _RTvdLineInfo
{
	UINT16 wLinecnt;
	UINT16 wERR_VLEN;
	UINT16 wLOCK_VLEN;
    #if TVD_FRS_FOR_NONSTDSIGNAL
	UINT16 wAvg_VLen;
    #endif
}RTvdLineInfo;
typedef struct _RTvdNAInfo
{
 UINT8  na_HysterCnt; // For hystersis of NA calculation.
 UINT8  na_HysterCntThres;
 UINT8  na_cnt;
UINT16 na_sum;
}RTvdNAInfo;


typedef struct RTvdStatus
{
	UINT8 eSourceType;
	UINT8 fgIs525;
	UINT8 fgIsPhaseAlt;
	UINT8 fgVPres;
	UINT8 fgIsVCR;
	UINT8 bTvdMode;
	UINT8 bSigStatus;
	UINT8 fgTrick;
	UINT8 bColSys;
	UINT8 fgSVPres;
	UINT8 fgIsMV;
    UINT8 bCAGC;
    UINT8 bAgainCode;
	#if TVD_VCR_BV
	UINT8 fgIsVCRBV;
	#endif
    BOOL  fgIsBurstLock; 
	BOOL  fgIsCkill;
	BOOL  fgIsPSync;
	BOOL  fgIsCoChannel;
	BOOL  fgCNRLow;
	BOOL  fgIs443nPHALT;
	BOOL  fgHLOCK;
	BOOL  fgVLOCK;
	BOOL  fgHSYNC_LOCK;
	INT16 wLine_err;
    RTvdLineInfo LineInfo;
    RTvdNAInfo  NAInfo;
	
} RTvdStatus;

typedef struct RTvdAGCInfo
{
    #ifndef CC_UP8032_ATV
    BOOL bmanualAGC;
    #else
    UINT8 bmanualAGC;
    #endif
    UINT8 minHWidth;
    UINT8 maxHWidth;
    UINT8 thHigh;
    UINT8 thSuper;
} RTvdAGCInfo;

typedef struct 
{
    UINT8 MCAGC_EN;
    UINT8 MCAGC;
    UINT32 Bound;
    UINT8 L_Step;
    UINT8 S_Step;
} RTvdCAGCInfo;

typedef enum _TVD_AGC_T
{
    E_HOLD_GAIN     = 0x1,  /// Hold Current AGain & DGain 
    E_MENU_GAIN     = 0x6,  /// Menu DGain & AGain
    E_AUTO_GAIN     = 0x8,  /// Auto DGain & AGain 
    E_MENU_DGAIN    = 0xA,  /// Menu DGain    
    E_MENU_AGAIN    = 0xC,  /// Menu AGain
    E_DFT_MENU_GAIN = 0xE   /// Menu DGain & AGain with default gain code when VPRES OFF
} TVD_AGC_T;

typedef struct RTvdNSTDStatus
{
	UINT8 fgIsNSTD;
	UINT8 bRefreshRate;
	UINT16 wVTotal;
	UINT16 wHTotal;
	INT16 wVTotalDiff;
} RTvdNSTDStatus;

enum TVDCTL_ITEM{
     TCTL_RESET_DEFAULT=0,
     TCTL_RECONNECT,
     TCTL_INPUTTYPE,
     TCTL_COUNTRY,
     TCTL_DFTMODE,
     TCTL_MDSTBPROT,
     TCTL_NTSCONLY,
     TCTL_COLORSYSTEM,
     TCTL_NOISELVL,
     TCTL_VCR,
     TCTL_COCH,
     TCTL_CKILL,
     TCTL_SERRTYPE,
     TCTL_SERRCNT,
     TCTL_ACODE,
     TCTL_DCODE,
     TCTL_CAGC,
     TCTL_BP_ISR,
     TCTL_BP_MODECHG,
     TCTL_BP_TC_INIT,
     TCTL_REG_SAVE_RESTORE,
     TCTL_AGCPEDEN,
     TCTL_MCNT,
     TCTL_MAX
};

enum TVDCTL_CMD{
	TC_DISABLE=0,	/* ctrl disable, reset val to 0 */
	TC_ENABLE,	    /* ctrl enable, set val by arg */
	TC_APPLY,	    /* TC_GETEN? call TC_ENABLE : call TC_DISABLE  */
	TC_GETEN,	    /* check ctrl enable */
	TC_GETVAL,	    /* get ctrl value */
	/* speical flags*/
	TC_INIT=0x80	/* for initial*/
};

enum TVDFT_ITEM{
	TVDFT_ALL=0,	/* apply to all finetune items*/
	TVDFT_CHROMA_SAT,
};
enum TVDFT_CMD{
	/* general finetune command*/
	FTCMD_RESET=0,
	FTCMD_GO,
	FTCMD_HOLD,
	/* item related command */
	FTCMD_00=16,
	FTCMD_01,
	FTCMD_02,
	FTCMD_03,
	FTCMD_04,
	FTCMD_05,
	FTCMD_06,
	FTCMD_07,
};

#if TVD_CHECK_BURST
enum TVD_BLOCK{
    TVD_BLOCK_IS443=0,
    TVD_BLOCK_IS358,
    TVD_BLOCK_UNKNOW,
};
#endif
/**************************************************************************
 * Export variables
 *************************************************************************/
// TVD Status
extern RTvdAGCInfo    _rTvd3dAGCInfo;
extern RTvdCAGCInfo   _rTvd3dCAGCInfo;

extern RTvdStatus   _rTvd3dStatus;
extern UINT8        _bTvdHoldMode;
extern UINT8        _na_state;
extern UINT8        _bTvdISRCnt;
extern BOOL         _fgHLOCK;
extern UINT8        _bSetChannelCnt; //Counter for tuner, set 60 when change channel number rock 20060324
extern UINT8        bhwVCRSetting;

#if TVD_COCH_FLICKER
extern UINT8 bIsBlankVarLarge;
#endif

#if TVD_AUTO_CALIB
extern UINT8 _bTVDAutoCalibState;
#endif

extern BOOL    _fgAutoSearch;
extern UINT16  wTVDNRavg;


/**************************************************************************
 * Export Functions
 *************************************************************************/
extern BOOL fgHwTvdVPres(void);
extern UINT8 bTvdCtrl(UINT8 bItem,UINT8 bCmd,UINT32 bArg) reentrant; 
extern void vTvdSetAGCMode(TVD_AGC_T eAgcType) reentrant;
extern void vDrvColorSystem(UINT8 bTVD, UINT8 bColSys) ;
extern void vTvd3dReset(void) ;
extern BOOL fgIs525Mode(UINT8 bMode) ;
extern void vTvdResetChromaLock(void);
extern void vTvdHoldMode(UINT8 bHoldOnOff);
extern void vTvd3dChannelChange(void);

extern void vTvd3dFastChannelChange(UINT8 bOnOff);
extern void vTvd3dSVInit(UINT8 bSVOnOff);
extern void vTvd3dSetYCDelay(UINT8 bYCDelay);
extern void vTvd3dSetYCDelaybyAP(UINT8 bYDelay, UINT8 bCDelay);
extern void vTVDCAGCTARGET(INT8 bOffset);
#if TVD_AUTO_CALIB
extern BOOL bDrvTVDAdcCalib(void);
extern void vDrvTVDAutoCalibStart(void);
extern BOOL bDrvTVDAdcCalib4Auto(void);
extern void vDrvTVDAutoCalibState(void);
extern UINT8 wDrvGetTVDADCMAX(void);
extern void  vCust_TVD_ADC_MAX_Update(UINT8 value);
extern void vCust_TVD_Gain_Update(UINT8 value);
extern UINT16 bDrvGetTVDADCOffset(void);
extern void vDrvSetTVDADCOffset(UINT16 value);
#endif
extern void vDrvSetTVDADCGain(UINT8 value);
extern void vDrvSetTVDADCDGain(UINT16 value);
extern void vTvd3dSetCAGCbyAP(void);
extern void vTVDAVDelay(UINT8 bDelay);        // For CLI
//extern void vTvd3dSetCAGCBound(UINT32 dwBound);
extern void vTvd3dSetCAGCADAP(UINT8 bOnOff);
extern void vDrvTvd3dVPresOnOff(UINT8 bGain, UINT8 bThrs) reentrant;
extern void vMainCKill(UINT8 bOnOff);
extern void vSubCKill(UINT8 bOnOff);
extern void vDrvWhitePeakInit(void);
extern void vDrvTvd3dSetChromaPGain(UINT8 bGain);
extern void vDrvTvd3dMCKill(UINT8 bOnOff);
extern void vTvd3dSetBUNLOCKThres(UINT16 wThres2, UINT16 wThres1);
extern void vSVPresDet(void);
extern void vTvd3dATDInit(UINT8 bATDOnOff);
extern void vDrvSetTVDGain(UINT16 value);
extern void vDrvSetTVDOffset(UINT16 value);
extern void vTvdForceTvdManualVPres(UINT8 bMVPresOnOff);
// Function Pointer
extern void vTvd3dConnect(UINT8 bPath, UINT8 bOnOff) ;
extern void vTvd3dTrigModeDet(void) ;
extern UINT16 wTvdWidth(void) ;
extern UINT16 wTvd3dHeight(void) ;
extern UINT8 bTvd3dRefreshRate(void) ;
extern UINT8 bTvd3dSignalStatus(void) ;
extern void vTvd3dVSyncISR(void) ;
extern UINT8 bTvdLumaSignalRefreshRate(void) ;
extern void vDrvTvd3dDClmpOn(void) ;
extern void vDrvTvdSetDftType(UINT8 fgIsPAL); 
extern void vDrvWhitePeakLimit(UINT8 bOnOff);
extern UINT8 bTvdNRLevel(void);
extern UINT8 bTvdSignalStrengthLevel(UINT8 bMode);
extern void vTvd3dSetAAFbyAP(UINT8 bOnOff, UINT8 bValue);
extern void vTvd3dMainLoop(void);
extern void vTvd3dBHModeChg(void);
extern void vTvd3dBHModeDone(void);
extern void vTvd3dGetUIDelay(void);
extern UINT8 bTvd3dGetColorSystem(void);
extern void vDrvTvd3dSetColorSystem(UINT8 bColSys);
extern void vDrvTvdQueryStatus(void);
extern UINT8 bDrvGetTVDADCGain(void);
extern void vDrvSetTVDADCGain(UINT8 value);
extern void vTvdSetOverSampleEnable(UINT8 bmode);
extern UINT8 vTvdGetOverSampleEnable(void);
extern void vCust_TVD_Offset_Update(UINT16 value);
extern void vTvdFineTune(UINT8  bItem, UINT8 bCmd, UINT32 dArg);
extern BOOL fgDrvTvdCheckTVDLock( UINT16 bTryLoopCnt );
extern UINT8 bTvdNAState4Step(void);
extern UINT16 wTvdHTotal(void);
extern UINT16 wTvdVTotal(void);
extern void vTvd3dGetNSTDStatus(RTvdNSTDStatus* pTvdNSTDStatus);
extern void vTvdSetModeDetTime(UINT8 bMDCnt);
extern UINT8 bTvdGetModeDetTime(void);
extern void vTvdSetPorch(UINT8 bPorchType, UINT16 wValue);
extern void vTvd3dInit(void);

#if TVD_NR_AVG
extern void vTvdMeasureNR(BOOL  fgEnable );
extern UINT8 vTvdGetNRLevel(void);
#endif

#if TVD_CLI_SUPPORT
extern void vTvdSetCLINfy(TVD_PFN_CLI_NFY pfnCliNfy);
extern void vTvd3dSetFixAgc(UINT8 bOnOff);
extern void vTVD3dAGCPedEn(UINT8 bOnOff);       
extern void vTvd3dSetPatGen(UINT8 bOnOff);
#endif
#if TVD_CHECK_BURST
extern UINT8 bDrvTvdCheckBurstFreq(void);
#endif
extern UINT16 wDrvGetTVDYLvl(void);

#endif
