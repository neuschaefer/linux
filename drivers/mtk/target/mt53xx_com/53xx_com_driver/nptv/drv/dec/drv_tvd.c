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

/*****************************************************************************
*
* Filename:
* ---------
*   $Workfile: drv_tvd.c $
*
* Project:
* --------
*   MT8205
*
* Description:
* ------------
*   TVD Driver
*
* Author:
* -------
*   Cindy Huang
*
* Last changed:
* -------------
* $Author: dtvbm11 $
*
* $Modtime: 04/06/01 6:05p $
*
* $Revision: #1 $
****************************************************************************/
/**
* @file drv_tvd.c
*/
#define _DRV_TVD_C_
/**************************************************************************
 * Header Files
 *************************************************************************/
#ifndef CC_UP8032_ATV  //For 53xx
#include "x_bim.h"
#include "x_assert.h"
#ifdef CC_COPLAT_MT82
#include "general.h"
#endif 
#else
#include "general_mt82.h" //For 8032
#include "x_hal_5381.h"
#include "general.h"
#endif

#ifndef CC_COPLAT_MT82 
#include "x_os.h"
#endif

#include "hw_vdoin.h"
#include "drv_tvd.h"
#include "video_def.h"
#include "source_select.h"
#if SUPPORT_SCART
#include "drv_scart.h"
#endif
#include "drv_nr.h"
#include "drv_di.h"
#include "drv_tdc.h"
//#include "nptv_debug.h"
#include "tvd_debug.h"
#include "util.h"
#include "hw_tdc.h"
#include "x_lint.h"
#include "api_notify.h"
#include "drv_video.h"
#include "nptv_drvif.h"
#include "eepdef.h"
#include "drvcust_if.h"
#include "x_util.h"
#include "api_eep.h"
#include "vdo_misc.h"
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif 
#include "pd_atd_if.h"

/**************************************************************************
 * Local Constant/Configure Definitions
 *************************************************************************/
#if defined(CC_MT5363) || defined(CC_MT8223) || defined(CC_MT5365) || defined(CC_MT5395)
#define CC_MT5363_TVD
#endif
#if defined(CC_MT8223) || defined(CC_MT5365) || defined(CC_MT5395)
#define CC_MT8223_TVD
#endif
#if defined(CC_MT5365) || defined(CC_MT5395)
#define CC_MT5365_TVD
#endif
 
#ifdef CC_COPLAT_MT82
#define GetTargetCountry() COUNTRY_EU
#define eTVDLLockFreeRun 0
#endif
#define TVD_USE_NEW_DAGC    0
#if TVD_USE_NEW_DAGC
#define TVD_DAGC_WA 	1
#else
#define TVD_DAGC_WA 	0
#endif
#define TVD_AntiTrs        1
#define TVD_PHALT_WA 	1
#define TVD_MODE_Deglitch 	1
#define TVD_MODESWITCH_PROTECT 0 //new add
#define TVD_MAX_CLAMP_TABLE	1
#define TVD_SERRTYPE_ONE	1
#define TVD_LLOCK_STATUS 1
#define TVD_FORCE_FAST_VALIGN 1
#define TVD_8222_NONSTANDARD 1
#define TVD_NO_BYPASS_TRICK 1
#define TVD_VCR_PALSW_FAST 0
#define TVD_VCR_SECAM_DARK 0
#define TVD_OLD_VCR 0
#define TVD_FAST_MODEGAIN_CHANNEL_CHANGE 0
#define SUPPORT_DFE_VPRES_DET 1
#define TVD_SECAM_DET 1 // 1: check burst lock
#define TVD_FORCE_UPD_MODE 1 // 1: always update mode
#define TVD_VCR_BV_STA     1
#define CAGC_PEAK_CNR_ADJ  1 //Adjust CAGC Peak paramater by CNR
#define TVD_HSHAKING_WA     1


#ifdef CC_SCPOS_VPORCH_WA
#define TVD_VPORCH_TUNE (-2)	/* fix DISP mode & MJC,send mjc one line for MT5391 */
#else
#define TVD_VPORCH_TUNE 0
#endif
#define CCVSYNC_P_DEFAULT 0x19
#define CCVSYNC_N_DEFAULT 0x15

//----------Standard / Non-Standard Line number Define --------//
#define LINE_NON_STD_MIN  1695
#define LINE_NON_STD_MAX  1761
#define LINE_STD_MIN      1701
#define LINE_STD_MAX      1749

#define NA_SIZE 16 // window size

#define DET443CAGCThres 0x18
#define TVD_SCAN_CAGCThres 0x14 //20% chroma level

#if TVD_AUTO_CALIB
//target 16~235
#define TARGET_Y_LEVEL_N 0x230//560 NTSC_M, NTSC_J
#define TARGET_Y_LEVEL_N216 0x203//515 for 16~216
#define TARGET_Y_LEVEL_P 0x224 //548 PAL
#define TARGET_Y_LEVEL_P216 0x1f4 //500 PAL for 16~216
#define TVDAutoCalibDelayTime 0x10 //16 main_loop
#endif


#if TVD_SERRTYPE_ONE
#define AVGCOIIR_CNT 0x8
#define AVGCOIIR_TH 0x20
#endif 

#define TVD_CAGC_LIM_GAIN 0x20

//For TvdCheckTVDLock
#define VLOCK_CHECK_TIME        0x20
#define TVDLOCK_RATIO           60      // 60%
#define TVDNRLevel_TH           0x13

#define MENU_AV_DELAY_H_OFFSET 0x90
#define CSCAN_DIFF_MIN_TH      0x1C0
#define CSCAN_NR_TH            0x5

/**************************************************************************
 * Local Type/Enum definitions
 *************************************************************************/
enum ehwVCRSetting
{
	VCR_NONE,
	VCR_MODE1
};

//--------- Noise Analysis -----------
enum eNASTATE
{
	NA_UNKNOWN,
	NA_LOWNOISE,
	NA_HIGHNOISE,
	NA_SUPERHIGHNOISE,
	NA_STOP
};

/**************************************************************************
 * Imported functions (optional)
 *************************************************************************/
#ifdef CC_UP8032_ATV
#define fgApiEepromWriteByte(wAddr, bData) vApiEepromWriteByte(wAddr, bData)
//#else 
//extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData); 
#endif

/**************************************************************************
 * Imported variables (optional)
 *************************************************************************/
extern UINT16 _wTargetBlk;
extern UINT8  RF_level;
extern UINT32 _u4BypassNptvMloop;
/**************************************************************************
 * Static/Local variables
 *************************************************************************/

static TVD_PFN_CLI_NFY   _pfnTVDCLINotify     = NULL;
//static UINT8 _seSourceType; //Refer to enum Signal_Type
static struct  RTvdCtrl
{
	UINT8 en;
	UINT8 val;  /* should reserve 8bit MSB from effective bArg*/
} _srTvdCtrl[TCTL_MAX];
#if !TVD_HSHAKING_WA
static UINT8 _sbTVDFreeRun = 0; //Flag for HW FreeRun.
#endif

//{PAL_N, PAL, PAL_M, NTSC358, SECAM, PAL_60, SV_CS_UNSTABLE, NTSC443};
static const UINT8 _sTvd_MenuAvDelay_Tbl[] = 
  {0xBE, 0xB2, 0xB2, 0xB2, 0x9e, 0xA6, 0xA6, 0xA6};
#if !TVD_HSHAKING_WA
static const UINT8 _sTvd_SHNA_MenuAvDelay_Tbl[] = 
	{0xC7, 0xB7, 0xB2, 0xB9, 0xA3, 0xAD, 0xB0, 0xA7};
//	{0xCB, 0xBD, 0xBF, 0xBF, 0xA9, 0xBD, 0xB0, 0xB0};
#endif
static INT8 _su1UIDelay; 

#if TVD_LLOCK_STATUS
static UINT16 _swLLStatus_avg;
#endif

//Counter for VCR mode
static UINT8   _sbVCRCnt;

#if TVD_SECAM_DET
static UINT8 _sbBurstcnt;
#endif 
#if 0//TVD_PHALT_WA
static UINT8 _sbPhalt_Vpres=0;
#endif 

//Mode Change Counter
static UINT8   _sbTvdModeCnt;
static UINT8   _sbTvd_McDone_cnt = 12;

//Co-Channel deglitch Counter
static UINT8   _sbCOCHDeCnt; 
//Co-Channel2 Counter
static UINT8   _sbCOCH2Cnt; 

static UINT8 _sbHSyncPosDetCnt=0;

#if SUPPORT_SCART
//Scart counter
static UINT8 _sbSCnt;
//Counter for Scart SFisrt
static UINT8 _sbScart_SV;
#endif

//Target sync height
static UINT16  _swAGCTargetSyncHeight = 0x380;

//NR Avg 
static UINT16 _swTVDNRavg1 = 3;

//Na detect
//static UINT8  _sna_HysterCnt; // For hystersis of NA calculation.
//static UINT8  _sna_HysterCntThres;
//static UINT8  _sna_cnt;
//static UINT16 _sna_sum;


#if TVD_SERRTYPE_ONE
static UINT16 _swCochIir_Avg; 
static UINT8 _sbAvgCOCH_detect;
#endif

static UINT8 _sbHLOCKCnt;

#if TVD_OLD_VCR
//VCR 
static UINT8 _sbOldVCRCnt;
static BOOL  _sfgOldVCR;  //maybe no useful ??
#endif

#if TVD_FRS_FOR_NONSTDSIGNAL
static UINT8 _sbNONSTDCnt;
static BOOL  _fgForceUpdNSTD;
#endif

static UINT8 _spre_na_state;

#if TVD_AUTO_CALIB
static UINT8 _sbTVDAutoCalibState1;
static UINT8 _sbTVDAutoCalibResult;
#endif

#if !TVD_NO_BYPASS_TRICK
static UINT8 _sbBypassTrick;
#endif

static UINT8 _sbDCLAMPCnt;

//for Fix AGC
static UINT8 _sbIsTVDFixAgc = 0;//default auto
static UINT8  _sbTvdMenuAcode = 0x1F;
static UINT16 _swTvdMenuDcode = 0x800;


//YCDelay control by AP
static UINT8 _sbIsSetYCDelaybyAP = 0;
//CAGC control by AP
static UINT8 _sbIsSetCAGCbyAP = 0;
//AAF control by AP
static UINT8 _sbIsSetAAFbyAP = 0;
//Offset control by AP
static UINT8 _sbIsTVDOffsetbyAP = 0;

static UINT8 _sbTVDSerrCnt;
static UINT8 _bTVDOverSampleEn = CC_SUPPORT_OVERSAMPLE_1440;

#if TVD_AUTO_CALIB
static UINT8 _sbTVDAutoCalibCount,_sbPre_Manual_ACode=0xff;
static UINT16 _swY_Avg_Lvl, _swY_Diff, _swTvd_Gain;
#endif 

#if SUPPORT_S_DETECT
static UINT8   _sbSFirstCnt;
static UINT8   _sbSVFNotify=SV_FALSE;
static UINT8   _sbSVFBypassNotify=SV_FALSE;
#endif
#if TVD_VPRES_STATE
static UINT8 _sbVpresState;
static UINT8 _sbVpresStateCnt1;
#endif
static UINT8 fgPreVPres_0;

#if TVD_VCR_BV
static UINT8 _sbVCRBVCnt;
#endif 

#if TVD_VCR_BV_STA
static UINT8 _sbVCRBVStaCnt;
static UINT8 _fgVCRBV;
#endif
static UINT8 _sbVP1Cnt;
#if 0
static const UINT8 _sbTvdModeMapTable[8]=
{
	/* Is525,Is443,PHALT*/
    /* 0,0,0*/	SV_CS_PAL, /* SV_CS_UNSTABLE*/
    /* 0,0,1*/	SV_CS_PAL_N,
    /* 0,1,0*/	SV_CS_SECAM,
    /* 0,1,1*/	SV_CS_PAL,
    /* 1,0,0*/	SV_CS_NTSC358,
    /* 1,0,1*/	SV_CS_PAL_M,
    /* 1,1,0*/	SV_CS_NTSC443,
    /* 1,1,1*/	SV_CS_PAL_60
};
#endif

#ifdef CC_MT8223
static UINT8 _sbHsliceCnt;
#endif
// For TVD Vlock used
static UINT16 _swTVDNRLevel, _swTVDNRLevel_AVG; //New Add

static UINT8 _sbPreAutoSearch = 0 ;
/**************************************************************************
 * Global variables
 *************************************************************************/
RTvdAGCInfo     _rTvd3dAGCInfo;
RTvdCAGCInfo    _rTvd3dCAGCInfo;
RTvdStatus      _rTvd3dStatus;
RTvdNSTDStatus      _rTvd3dNSTDStatus;
UINT8           _bTvdHoldMode;
UINT8  _bTvdISRCnt;
UINT8 _bAgcPedEn;       //Pedestal Mode
UINT8 _bSetChannelCnt;  //Counter for tuner, set 60 when change channel number rock 20060324
#if TVD_AntiTrs
static UINT8 _sbCAGC1 =0 ;                        //  FIRST field :the value of cagc
static UINT8 _sfgCagc=0;
#endif
#if TVD_COCH_FLICKER
UINT8 bIsBlankVarLarge;
#endif

#if TVD_MODE_Deglitch
//--------- Counter for Pre-Mode-Change -----------
static UINT8   _sbTvdPreModeCnt=0xFF;
static BOOL    _sfgPreVPres_2=0;
static BOOL    _sfgPreIs525_2=0;
static UINT8   _sbPreTvdMode_2=0xFF;

#define vDrvTvdRstPreModeCnt()        (_sbTvdPreModeCnt = 0)
#define vDrvTvdIncPreModeCnt()        (_sbTvdPreModeCnt = (_sbTvdPreModeCnt == 0xff)?0xff:(_sbTvdPreModeCnt+1))
#define fgDrvTvdIsPreModeCnt(bCnt)    (_sbTvdPreModeCnt==(bCnt))

static UINT8 TVD_PreMCDONE_CNT_Vpres=5;
static UINT8 TVD_PreMCDONE_CNT_Mode=20;
#endif

UINT8 bhwVCRSetting;

//To solve PAL_SW problem of VCR. Benson060210.
UINT16 wTVDNRavg;  //CDET NR Avg. 

BOOL _fgHLOCK;

#if TVD_FRS_FOR_NONSTDSIGNAL
UINT16 _wPreAvg_VLen; 
UINT16 _wPreLCnt; 
#endif

UINT8 _na_state; /* noise analysis state machine */

#if TVD_AUTO_CALIB
UINT8 _bTVDAutoCalibState;
#endif

#ifdef COUNTRY_FROM_EEPROM
	UINT8 u1TargetCountryInfo;
#endif

#if TVD_DAGC_WA
    UINT8 _sbTvdDCodeCnt = 0;
#endif

/**************************************************************************
 * Local Macro definitions
 *************************************************************************/
#define IS_LINE_STD(line)      ((((line) < LINE_STD_MAX)&&((line) > LINE_STD_MIN)) ? TRUE : FALSE) 
#define IS_LINE_NON_STD(line)  ((((line) > LINE_NON_STD_MAX)||((line) < LINE_NON_STD_MIN)) ? TRUE : FALSE) 
#define IS_AUTOSEARCH()        ((_fgAutoSearch) ? TRUE : FALSE) //_fgAutoSearch extern golbal variable. 
#define IS_FH_NON_STD()        (fgHwTvdFHPos()||fgHwTvdFHNeg())
#define IS_5387_NonECO()       (IS_IC_5387() && (BSP_GetIcVersion() <= IC_VER_5387_AA))   
#define IS_5387_ECO()          (IS_IC_5387() && (BSP_GetIcVersion() >= IC_VER_5387_AB))
#define IS_5363_NonECO()       (IS_IC_5363() && (BSP_GetIcVersion() <= IC_VER_5363_AA))
#define IS_5363_ECO()          (IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB))


//--------- Counter for VCR Mode ------------------------------//
#define TVD_VCRON_CNT  0xFF
#define TVD_VCROFF_CNT 0x0
#define TVD_VCRReset   0xF2
#define vDrvTvdRstVCRCnt()      (_sbVCRCnt = TVD_VCRReset)
#define vDrvTvdIncVCRCnt()      (_sbVCRCnt = (_sbVCRCnt == TVD_VCRON_CNT)?TVD_VCRON_CNT:(_sbVCRCnt+1))
#define vDrvTvdDecVCRCnt()      (_sbVCRCnt = (_sbVCRCnt == TVD_VCROFF_CNT)?TVD_VCROFF_CNT:(_sbVCRCnt-1))
#define fgDrvTvdIsVCRCnt(bCnt)  ((_sbVCRCnt==(bCnt))? TRUE : FALSE) 

//---------Counter for Burst unlock change BI_START/END -------//
#if TVD_SECAM_DET
#define TVD_BurstLock_CNT  0xA
#define TVD_BurstLock_CNT2 0x0
#define BurstcntReset    0x5

#define vDrvTvdRstBurstCnt()      (_sbBurstcnt = BurstcntReset)
#define vDrvTvdIncBurstCnt()      (_sbBurstcnt = (_sbBurstcnt == TVD_BurstLock_CNT)?TVD_BurstLock_CNT:(_sbBurstcnt+1))
#define vDrvTvdDecBurstCnt()      (_sbBurstcnt = (_sbBurstcnt == TVD_BurstLock_CNT2)?TVD_BurstLock_CNT2:(_sbBurstcnt-1))
#define fgDrvTvdIsBurstCnt(bCnt)  ((_sbBurstcnt == (bCnt))? TRUE : FALSE) 
#endif

//--------- Counter for Mode Change ---------------------------//
#define vDrvTvdRstModeCnt()       (_sbTvdModeCnt = 0)
#define vDrvTvdIncModeCnt()       (_sbTvdModeCnt = (_sbTvdModeCnt == 0xff)?0xff:(_sbTvdModeCnt+1))
#define fgDrvTvdIsModeCnt(bCnt)   ((_sbTvdModeCnt==(bCnt))? TRUE : FALSE) 
#define fgDrvTvdModeCntGt(bCnt)   ((_sbTvdModeCnt>=(bCnt))? TRUE : FALSE) 

//--------- Counter for COCH deglitch-----------
#define TVD_COCHDe_CNT  0x20
#define TVD_COCHDe_CNT2 0x0
#define COCHDeReset     0x10
#define vDrvTvdRstCOCHDeCnt()       (_sbCOCHDeCnt = COCHDeReset)
#define vDrvTvdIncCOCHDeCnt()       (_sbCOCHDeCnt = (_sbCOCHDeCnt == TVD_COCHDe_CNT)?TVD_COCHDe_CNT:(_sbCOCHDeCnt+1))
#define vDrvTvdDecCOCHDeCnt()       (_sbCOCHDeCnt = (_sbCOCHDeCnt == TVD_COCHDe_CNT2)?TVD_COCHDe_CNT2:(_sbCOCHDeCnt-1))
#define fgDrvTvdIsCOCHDeCnt(bCnt)   ((_sbCOCHDeCnt == (bCnt))? TRUE : FALSE)
//--------- Counter for COCH2 Enable-----------
#define TVD_COCH2EN_CNT  0x40
#define TVD_COCH2EN_CNT2 0x0
#define COCH2ENReset     0x20
#define COIIR_TH_HIGH    0x20
#define COIIR_TH_LOW     0x18
#define vDrvTvdRstCOCH2ENCnt()       (_sbCOCH2Cnt = COCH2ENReset)
#define vDrvTvdIncCOCH2ENCnt()       (_sbCOCH2Cnt = (_sbCOCH2Cnt == TVD_COCH2EN_CNT)?TVD_COCH2EN_CNT:(_sbCOCH2Cnt+1))
#define vDrvTvdDecCOCH2ENCnt()       (_sbCOCH2Cnt = (_sbCOCH2Cnt == TVD_COCH2EN_CNT2)?TVD_COCH2EN_CNT2:(_sbCOCH2Cnt-1))
#define fgDrvTvdIsCOCH2ENCnt(bCnt)   ((_sbCOCH2Cnt == (bCnt))? TRUE : FALSE) 

//HSync Position Detection
#define HSYNCPOS_DCNT_TH1 0x3A
#define HSYNCPOS_DCNT_TH2 0x80
#define TVD_HSYNCPOS_CNT  0x20
#define TVD_HSYNCPOS_CNT2 0x0
#define HSYNCPOSReset     0x10
#define vDrvTvdRstHsyncPosDetCnt()       (_sbHSyncPosDetCnt = HSYNCPOSReset)
#define vDrvTvdIncHsyncPosDetCnt()       (_sbHSyncPosDetCnt = (_sbHSyncPosDetCnt == TVD_HSYNCPOS_CNT)?TVD_HSYNCPOS_CNT:(_sbHSyncPosDetCnt+1))
#define vDrvTvdDecHsyncPosDetCnt()       (_sbHSyncPosDetCnt = (_sbHSyncPosDetCnt == TVD_HSYNCPOS_CNT2)?TVD_HSYNCPOS_CNT2:(_sbHSyncPosDetCnt-1))
#define fgDrvTvdIsHsyncPosDetCnt(bCnt)   ((_sbHSyncPosDetCnt == (bCnt))? TRUE : FALSE)


#if SUPPORT_SCART
//--------- Counter for Scart SFisrt -----------
#define TVD_S_CNT     9
#define TVD_S_CNT2    1

#define vDrvTvdRstSCnt()      (_sbSCnt = 5)
#define vDrvTvdIncSCnt()      (_sbSCnt = (_sbSCnt == 10) ?10:(_sbSCnt+1))
#define vDrvTvdDecSCnt()      (_sbSCnt = (_sbSCnt == 0x0)?0x0:(_sbSCnt-1))
#define fgDrvTvdIsSCnt(bCnt)  ((_sbSCnt==(bCnt))? TRUE : FALSE) 
#endif

//--------- Counter for HLOCK for Manual Line-lock-----------
#define TVD_HLOCKDONE_CNT  0x50
#define TVD_HLOCKDONE_CNT2 0x0
#define TVD_HLOCK_RSTCNT   0x4a

#define vDrvTvdRstHLOCKCnt()      (_sbHLOCKCnt = TVD_HLOCK_RSTCNT)
#define vDrvTvdIncHLOCKCnt()      (_sbHLOCKCnt = (_sbHLOCKCnt == TVD_HLOCKDONE_CNT)?TVD_HLOCKDONE_CNT:(_sbHLOCKCnt+1))
#define vDrvTvdDecHLOCKCnt()      (_sbHLOCKCnt = (_sbHLOCKCnt == TVD_HLOCKDONE_CNT2)?TVD_HLOCKDONE_CNT2:(_sbHLOCKCnt-1))
#define fgDrvTvdIsHLOCKCnt(bCnt)  ((_sbHLOCKCnt == (bCnt))? TRUE : FALSE) 

#if TVD_FRS_FOR_NONSTDSIGNAL
//--------- Counter for NonStandard  -----------
#define TVD_NONSTD_CNT      0xd0
#define TVD_NONSTD_CNT2     0x30
#define TVD_NONSTD_RSTCNT   0x80
#define vDrvTvdRstNONSTDCnt()      (_sbNONSTDCnt = TVD_NONSTD_RSTCNT)
#define vDrvTvdIncNONSTDCnt()      (_sbNONSTDCnt = (_sbNONSTDCnt == 0xFF)?0xFF:(_sbNONSTDCnt+1))
#define vDrvTvdDecNONSTDCnt()      (_sbNONSTDCnt = (_sbNONSTDCnt == 0)?0:(_sbNONSTDCnt-1))
#define fgDrvTvdIsNONSTDCnt(bCnt)  ((_sbNONSTDCnt == (bCnt))? TRUE : FALSE) 
#define vDrvTvdForceUpdNSTD(fgForceNSTD) (_fgForceUpdNSTD = (fgForceNSTD))
#define vDrvTvdIsForceUpdNSTD()          (_fgForceUpdNSTD)
#endif


#if TVD_OLD_VCR
//--------- Counter for OldVCR for Manual Line-lock-----------
#define TVD_OldVCR_CNT  0x70
#define TVD_OldVCR_CNT2 0x0
#define _bOldVCRReset   0x68

#define vDrvTvdRstOldVCRCnt()      (_sbOldVCRCnt = _bOldVCRReset)
#define vDrvTvdIncOldVCRCnt()      (_sbOldVCRCnt = (_sbOldVCRCnt == TVD_OldVCR_CNT)?TVD_OldVCR_CNT:(_sbOldVCRCnt+1))
#define vDrvTvdDecOldVCRCnt()      (_sbOldVCRCnt = (_sbOldVCRCnt == TVD_OldVCR_CNT2)?TVD_OldVCR_CNT2:(_sbOldVCRCnt-1))
#define fgDrvTvdIsOldVCRCnt(bCnt)  (_sbOldVCRCnt == (bCnt))
#endif


#if SUPPORT_S_DETECT
#define TVD_SFIRST_CNT   10//original 20
//--------- Counter for Scart SFisrt -----------
#define vDrvTvdRstSFirstCnt()       (_sbSFirstCnt = 0)
#define vDrvTvdIncSFirstCnt()       (_sbSFirstCnt = (_sbSFirstCnt == 0xff)?0xff:(_sbSFirstCnt+1))
#define fgDrvTvdIsSFirstCnt(bCnt)   ((_sbSFirstCnt==(bCnt))? TRUE : FALSE) 
#define fgDrvTvdSFirstCntGt(bCnt)   ((_sbSFirstCnt>=(bCnt))? TRUE : FALSE)  
#endif
#if TVD_VPRES_STATE
//--------- Counter for Vpres State  -----------
#define VPRES_STATE_0 0  // Vpres4 off, Vpres off
#define VPRES_STATE_1 1  // Vpres4 off, Vpres on
#define VPRES_STATE_2 2  // Vpres4 on, Vpres off
#define VPRES_STATE_3 3  // Vpres4 on, Vpres on
#define VPRES_STATE_4 4  // SVIDEO, Only Check Vpres3
#define VPRES_TH 2
#define vDrvTvdRstVpresStateCnt1()      (_sbVpresStateCnt1= 0)
#define vDrvTvdIncVpresStateCnt1()      (_sbVpresStateCnt1 = (_sbVpresStateCnt1 == 0xFF)?0xFF:(_sbVpresStateCnt1+1))
#define vDrvTvdDecVpresStateCnt1()      (_sbVpresStateCnt1 = (_sbVpresStateCnt1 == 0)?0:(_sbVpresStateCnt1-1))
#define fgDrvTvdIsVpresStateCnt1(bCnt)  ((_sbVpresStateCnt1 == (bCnt))? TRUE : FALSE)
#endif


//--------- Counter for Dclamp on/off ------------------------------//
#define TVD_DCLAMPON_CNT  0x20
#define TVD_DCLAMPOFF_CNT 0x0
#define TVD_DCLAMPReset   0x10
#define vDrvTvdRstDCLAMPCnt()      (_sbDCLAMPCnt = TVD_DCLAMPReset)
#define vDrvTvdIncDCLAMPCnt()      (_sbDCLAMPCnt = (_sbDCLAMPCnt == TVD_DCLAMPON_CNT)?TVD_DCLAMPON_CNT:(_sbDCLAMPCnt+1))
#define vDrvTvdDecDCLAMPCnt()      (_sbDCLAMPCnt = (_sbDCLAMPCnt == TVD_DCLAMPOFF_CNT)?TVD_DCLAMPOFF_CNT:(_sbDCLAMPCnt-1))
#define fgDrvTvdIsDCLAMPCnt(bCnt)  ((_sbDCLAMPCnt==(bCnt))? TRUE : FALSE) 
#define TVD_DCLAMP_Y4H 0x3
#define TVD_STD_Y4H 0x2
//-----------Counter for channel Changed 
#define CHANCHG_START  0x40
#define CHANCHG_STOP   0x0
#define CHANCHG_DONE   0x1

#define vDrvTvdSetChanchgCnt(value) (_bSetChannelCnt = (value))
#define vDrvTvdDecChanchgCnt()      (_bSetChannelCnt = (_bSetChannelCnt==0)?0:(_bSetChannelCnt-1))
#define fgDrvTvdIsChanchgCnt(bCnt)  ((_bSetChannelCnt == (bCnt))? TRUE : FALSE)

#if TVD_VCR_BV
#define TVD_VCR_BV_CNT  0x50

#define vDrvTvdRstVCRBVCnt()      (_sbVCRBVCnt = 0)
#define vDrvTvdIncVCRBVCnt()      (_sbVCRBVCnt = (_sbVCRBVCnt == TVD_VCR_BV_CNT)?TVD_VCR_BV_CNT:(_sbVCRBVCnt+1))

//#define vDrvTvdDecBurstCnt()      (_sbBurstcnt = (_sbBurstcnt == TVD_BurstLock_CNT2)?TVD_BurstLock_CNT2:(_sbBurstcnt-1))

#define fgDrvTvdIsVCRBVCnt(bCnt)  ((_sbVCRBVCnt == (bCnt))? TRUE : FALSE) 
#define fgDrvTvdVCRBVCntGt(bCnt)  ((_sbVCRBVCnt >= (bCnt))? TRUE : FALSE) 



//--------- Counter for VCR BV Status ------------------------------//
#define TVD_VCRBV_UP_GAIN   0x01
#define TVD_VCRBV_DOWN_GAIN 0x10
#define TVD_IS_VCRBVSTAON   0x50
#define TVD_IS_VCRBVSTAOFF  0x00
#define vDrvTvdRstVCRBVStaCnt()      (_sbVCRBVStaCnt = _fgVCRBV=0)
#define vDrvTvdIncVCRBVStaCnt()      (_sbVCRBVStaCnt = (_sbVCRBVStaCnt >= TVD_IS_VCRBVSTAON)?TVD_IS_VCRBVSTAON:(_sbVCRBVStaCnt+TVD_VCRBV_UP_GAIN))
#define vDrvTvdDecVCRBVStaCnt()      (_sbVCRBVStaCnt = (_sbVCRBVStaCnt <= TVD_VCRBV_DOWN_GAIN)?0x0:(_sbVCRBVStaCnt-TVD_VCRBV_DOWN_GAIN))
#define fgDrvTvdIsVCRStaCnt(bCnt)    ((_sbVCRBVStaCnt==(bCnt))? TRUE : FALSE) 
#define fgGetTvdVCRBVSta()           ((_fgVCRBV)? TRUE : FALSE)
#endif

//--------- Counter for VP1 on/off gain when vpres off->on-----------//
#define TVD_VP1Reset   0x0
#define TVD_VP1OFFON_threshold   0x80
#define vDrvTvdinitVP1Cnt()      (_sbVP1Cnt = TVD_VP1OFFON_threshold)
#define vDrvTvdRstVP1Cnt()      (_sbVP1Cnt = TVD_VP1Reset)
#define vDrvTvdIncVP1Cnt()      (_sbVP1Cnt = (_sbVP1Cnt == 0xFF)?0xFF:(_sbVP1Cnt+1))
#define fgDrvTvdVP1CntGt(bCnt)  ((_sbVP1Cnt>=(bCnt))? TRUE : FALSE) 
#define vDrvTvdGetTVDNRAvg()    (wTVDNRavg)

//--------- Counter for DCODE Status for small swing------------------------------//
#if TVD_DAGC_WA
#define TVD_IS_DCODESTAON   0x40
#define TVD_IS_DCODESTAOFF  0x00
#define TVD_DCODE_RST  0x20
#define TVD_DCODE_UPTH  0x700
#define TVD_DCODE_DNTH  0x600
#define vDrvTvdRstDCODEStaCnt()      (_sbTvdDCodeCnt = TVD_DCODE_RST)
#define vDrvTvdIncDCODEStaCnt()      (_sbTvdDCodeCnt = (_sbTvdDCodeCnt >= TVD_IS_DCODESTAON)?TVD_IS_DCODESTAON:(_sbTvdDCodeCnt+1))
#define vDrvTvdDecDCODEStaCnt()      (_sbTvdDCodeCnt = (_sbTvdDCodeCnt <= TVD_IS_DCODESTAOFF)?0x0:(_sbTvdDCodeCnt-1))
#define fgDrvTvdIsDCODEStaCnt(bCnt)    ((_sbTvdDCodeCnt==(bCnt))? TRUE : FALSE) 
#endif
#ifdef CC_MT8223
//--------- Counter for Hsync Slice Level adjust-----------//
#define TVD_HsliceReset   0x10
#define TVD_Hslice_threshold   0x20
#define vDrvTvdRstHsliceCnt()      (_sbHsliceCnt = TVD_HsliceReset)
#define vDrvTvdIncHsliceCnt()      (_sbHsliceCnt = (_sbHsliceCnt == TVD_Hslice_threshold)?TVD_Hslice_threshold:(_sbHsliceCnt+1))
#define vDrvTvdDecHsliceCnt()      (_sbHsliceCnt = (_sbHsliceCnt == 0)?0:(_sbHsliceCnt-1))
#define fgDrvTvdIsHsliceCnt(bCnt)    ((_sbHsliceCnt==(bCnt))? TRUE : FALSE) 
#endif

/**************************************************************************
 * Local/Static Functions
 *************************************************************************/
/**
 * @brief TVD SVF bypass function
 *
 * This function only be called at CLI
 *
 * @param None
 * @return None
 */
#if SUPPORT_S_DETECT
void vDrvSetSVFBypass(UINT8 bOnOff)
{
	_sbSVFBypassNotify=bOnOff;
}
#endif
/**
 * @brief TVD SVF counter function
 *
 * This function only be called at isr
 *
 * @param None
 * @return None
 */
#if SUPPORT_S_DETECT
void vSVPresDetCount(void)
{
	UINT8 fgSVPres;
	if ((_bMainMonICIn == P_FA) && (_bSubMonICIn == P_FA))
	{
		return;
	}
	fgSVPres = fgHwTvdVPresSVF();

	if (fgSVPres == _rTvd3dStatus.fgSVPres)
	{
		vDrvTvdIncSFirstCnt();

		if (fgDrvTvdIsSFirstCnt(TVD_SFIRST_CNT)&&(_sbSVFBypassNotify==SV_FALSE))
		{
			_sbSVFNotify=SV_TRUE;
		}

	}
	else
	{
		_rTvd3dStatus.fgSVPres = fgSVPres;
		vDrvTvdRstSFirstCnt();
	}
}
#endif

#if TVD_DAGC_WA
/**
 * @brief Fine Tune for TVD FixGain Mode 
 * @param bMcDone_Cnt IN: Mode Change Counter
 * @param bIsTvdFixAgc IN: TVD FixAgc Flag
 * @param bNaState IN: Noise State
 * @retval : NOON
 */
static void _svTvdFixedGainFineTune(UINT8 bMcDone_Cnt, UINT8 bIsTvdFixAgc, UINT8 bNaState)
{
    if(fgDrvTvdModeCntGt(bMcDone_Cnt) && (bIsTvdFixAgc))
    {
        if(bNaState!=NA_SUPERHIGHNOISE)
        {
            if(RegReadFldAlign(DFE_STA_04, DGAIN_CODE)>(RegReadFldAlign(DFE_07, AGC2_MANUAL_DCODE)+TVD_DCODE_UPTH))
            {
                vDrvTvdIncDCODEStaCnt();
                if(fgDrvTvdIsDCODEStaCnt(TVD_IS_DCODESTAON))
                {
                    vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_LowNA, FIXAGC_DAGCSEL);
                }
            }
            else if(RegReadFldAlign(DFE_STA_04, DGAIN_CODE)<(RegReadFldAlign(DFE_07, AGC2_MANUAL_DCODE)+TVD_DCODE_DNTH))
            {
                vDrvTvdDecDCODEStaCnt();
                if(fgDrvTvdIsDCODEStaCnt(TVD_IS_DCODESTAOFF))
                {
                    vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_HighNA, FIXAGC_DAGCSEL);
                }
            }
        }
        else
        {
            vDrvTvdRstDCODEStaCnt();
            vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_HighNA, FIXAGC_DAGCSEL);
        }
    }
}
#endif

static void _svTvdQtestStaUpdate(void)
{
    if(_pfnTVDCLINotify!=NULL)
    {
        _rTvd3dStatus.fgHSYNC_LOCK = fgHwTvdHsyncLock();
        _rTvd3dStatus.wLine_err = wHwTvdLineErr();
        _rTvd3dStatus.fgVLOCK = fgHwTvdVLock();
        _rTvd3dStatus.fgHLOCK = fgHwTvdHLock();
        _rTvd3dStatus.LineInfo.wERR_VLEN=RegReadFldAlign(TG_STA_01, ERR_VLEN);
        _rTvd3dStatus.LineInfo.wLOCK_VLEN=RegReadFldAlign(TG_STA_01, LOCK_VLEN);
        _rTvd3dStatus.u4LLockStatus = RegReadFldAlign(TG_STA_03, LLOCK_STATUS);

        
        vRegWriteFldAlign(SECAM_07, 8, YUV_CATCH_SEL);
        //vRegWriteFldAlign(SECAM_07, u2X, YUV_CATCH_HPT);//four pixels
        vRegWriteFldAlign(SECAM_07, 0x1B8, Fld(10, 10, AC_MSKW21));//one pixel
        vRegWriteFldAlign(SECAM_07, 0x40, YUV_CATCH_VPT);
        _rTvd3dStatus.u4Y= RegReadFldAlign(STA_CTG_04, Fld(10, 0, AC_MSKW10));
        _rTvd3dStatus.u4U = RegReadFldAlign(STA_CTG_04, Fld(10, 12, AC_MSKW21));
        _rTvd3dStatus.u4V = RegReadFldAlign(STA_CTG_04, Fld(10, 22, AC_MSKW32));
    }
}

#if TVD_MODE_Deglitch

/**
 * @brief Mode Deglish Initial Function  
 * @retval : NOON
 */
static void _svTvdInitModeDeglish(void)
{
	if (bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
	{
		_sfgPreIs525_2= 0x1;
		_sbPreTvdMode_2= 0x3;
	}
	else
	{
		if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
		{
			_sfgPreIs525_2= 0x00;
		}
		else
		{
			_sfgPreIs525_2= 0x01;
		}
		_sbPreTvdMode_2= 0xff;
	}
	_sfgPreVPres_2= FALSE;
}

/**
 * @brief Mode Deglish Function  
 * @param OUT:pointer to VPres status variable
 * @param OUT:pointer to pbTvdMode status variable
 * @param OUT:pointer to pfgIs525 status variable
 * @retval : NOON
 */
static UINT8 _sbTvdModeDeglish(BOOL *pfgVPres , UINT8 *pbTvdMode, BOOL *pfgIs525 )
{
    static  BOOL fgPreVPres_1=0;
    static  BOOL fgPreIs525_1=0;
    static  UINT8 bPreTvdMode_1=0xFF;

	if((fgPreVPres_1 != fgPreVPres_0)||
	    (bPreTvdMode_1 != bHwTvdMode())||
		(fgPreIs525_1 != fgHwTvd525()))
	{
		vDrvTvdRstPreModeCnt();
		fgPreVPres_1 = fgPreVPres_0;
		bPreTvdMode_1 = bHwTvdMode();
		fgPreIs525_1 = fgHwTvd525();
		LOG(6,"[TVD_DBG_MSG] _sbTvdModeDeglish fgPreVPres_1=%d, bPreTvdMode_1=%d, fgPreIs525_1=%d\n",
			fgPreVPres_1,bPreTvdMode_1,fgPreIs525_1);
	}
	vDrvTvdIncPreModeCnt();
	if((_swLLStatus_avg>0x10)&&(_rTvd3dStatus.eSourceType!=SV_ST_TV)
		#if TVD_VCR_BV_STA
		&&fgGetTvdVCRBVSta()
		#endif
		)
	{
		if(fgDrvTvdIsPreModeCnt(TVD_PreMCDONE_CNT_Vpres))
		{
			if((_sfgPreVPres_2 != fgPreVPres_1)||(_sfgPreIs525_2 != fgPreIs525_1))
			{
				_sfgPreVPres_2  = fgPreVPres_1;
				_sbPreTvdMode_2 = bPreTvdMode_1;
				_sfgPreIs525_2 = fgPreIs525_1;
				LOG(6,"[TVD_DBG_MSG] _sbTvdModeDeglish Vpres modechange\n");
			}
		}
		if(fgDrvTvdIsPreModeCnt(TVD_PreMCDONE_CNT_Mode))
		{
			_sfgPreVPres_2  = fgPreVPres_1;
			_sbPreTvdMode_2 = bPreTvdMode_1;
			_sfgPreIs525_2 = fgPreIs525_1;
			LOG(6,"[TVD_DBG_MSG] _sbTvdModeDeglish Mode modechange\n");
		}
	}
	else
	{
		_sfgPreVPres_2  = fgPreVPres_1;
		_sbPreTvdMode_2 = bPreTvdMode_1;
		_sfgPreIs525_2 = fgPreIs525_1;
	}
	*pfgVPres  = _sfgPreVPres_2;
	*pbTvdMode = _sbPreTvdMode_2;
	*pfgIs525 = _sfgPreIs525_2;

    return 0;
}
#endif


/**
 * @brief TVD ISR VCR Setting.  
 * @param fgIsVCR IN: VCR Status. 
 * @param bhwVCRSetting OUT: pointer to bhwVCRSetting variable. 
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svTvdVcrSet(BOOL fgIsVCR, UINT8 *pbhwVCRSetting, RTvdStatus *pTvd3dStatus)
{
    if (pTvd3dStatus->fgVPres && (fgDrvTvdModeCntGt(8)) && (pTvd3dStatus->bSigStatus != (UINT8)SV_VDO_NOSIGNAL))
	{
		// If VCR mode status is changed, reset VCR Counter
		if (fgIsVCR != pTvd3dStatus->fgIsVCR)
		{
			pTvd3dStatus->fgIsVCR = fgIsVCR;
			vDrvTvdRstVCRCnt();
		}
		if (IS_AUTOSEARCH())
		{
			// Reset VCR Counter after auto searching.  Benson 06.02.08
			vDrvTvdRstVCRCnt();
		}
    //************************************NonVCR Setting of TVD************************************
		else if (!pTvd3dStatus->fgIsVCR)
		{
			vDrvTvdDecVCRCnt();
			if (fgDrvTvdIsVCRCnt(TVD_VCROFF_CNT+1)) //check counter value is 1
			{
				vRegWriteFldAlign(DFE_02, SV_ON, POS_DIFF_ONLY);
                #if !TVD_VPRES_IMPROVE
				vDrvTvd3dVPresOnOff(0x12, DFE_NORMAL_ONOFF_THRD);
                #endif
                
                #if TVD_VCR_SECAM_DARK
    			vRegWriteFldMulti(CTG_07,
    					  P_Fld(1, FIXBLV)|
    					  P_Fld(1, SLOWBLC));
                #endif
				*pbhwVCRSetting = (UINT8)VCR_NONE;
			}
			/*****************************TDC On/Off under NonVCR**************************/
            #if TVD_VCR_PALSW_FAST
			if (fgDrvTvdIsVCRCnt(TVD_VCROFF_CNT+0xe3)) //check counter value is 0xe3
			{
				vRegWriteFldAlign(CDET_05,  SV_OFF, PALSW_FAST);
				vRegWriteFldAlign(CDET_05,  SV_OFF, PALSW_STEP_F);
			}
            #endif
		}
    //**************************************VCR Setting of TVD************************************
		else if (pTvd3dStatus->fgIsVCR)
		{
			vDrvTvdIncVCRCnt();
			if (fgDrvTvdIsVCRCnt(TVD_VCRON_CNT-1)) //check VCR counter is 0xFE
			{
				vRegWriteFldAlign(DFE_02, ((_na_state == NA_SUPERHIGHNOISE)?1:0), POS_DIFF_ONLY);
                #if !TVD_VPRES_IMPROVE
				vDrvTvd3dVPresOnOff(0x21, DFE_NORMAL_ONOFF_THRD);
                #endif

				*pbhwVCRSetting = (UINT8)VCR_MODE1;

                #if TVD_VCR_SECAM_DARK
				if ( pTvd3dStatus->bTvdMode==SV_CS_SECAM)
				{
        			vRegWriteFldMulti(CTG_07,
        					  P_Fld(0, FIXBLV)|
        					  P_Fld(0, SLOWBLC));
				}
				else
				{
        			vRegWriteFldMulti(CTG_07,
        					  P_Fld(1, FIXBLV)|
        					  P_Fld(1, SLOWBLC));
				}
                #endif

    			/*****************************PAL Switch Setting under VCR**************************/
    			// Use a large counter(0x40)  for PALSW_FAST to on.
    			//( To avoid fast Vdoswitch causing trick mode false alarm.)
                #if TVD_VCR_PALSW_FAST
				if (vDrvTvdGetTVDNRAvg() < 0x15)
				{
					vRegWriteFldAlign(CDET_05,  SV_ON, PALSW_FAST);
					vRegWriteFldAlign(CDET_05,  SV_ON, PALSW_STEP_F);
				}
				else
				{	// Avoid false alarm for trick mode (pTvd3dStatus->fgIsVCR will on).
					vRegWriteFldAlign(CDET_05,  SV_OFF, PALSW_FAST);
					vRegWriteFldAlign(CDET_05,  SV_OFF, PALSW_STEP_F);
				}
                #endif
			}
		}
	}

        //Improve VBI CC Performance
        if( (*pbhwVCRSetting==(UINT8)VCR_MODE1) && (fgHwTvdVCRBV()) && (!fgHwTvdIsMacroVision()) && (fgHwTvdVLock()) && fgDrvTvdModeCntGt(250))
            vRegWriteFldAlign(TG_04, SV_OFF, FAST_OFF);
        else
            vRegWriteFldAlign(TG_04, SV_ON, FAST_OFF);
}
/**
 * @brief TVD ISR adjust line threshold 
 * @param IN:NOON
 * @retval : NOON
 */
 static void _svAdjLineThr(void)
{
    if(RegReadFldAlign(TG_STA_05, LINE_STDFH_FLAG)) //AVG LINE In STD
    {
    	vRegWriteFldMulti(TG_17,
        	  P_Fld(TG_LINE_STDFH_UP_STD, LINE_STDFH_UP)|
        	  P_Fld(TG_LINE_STDFH_LO_STD, LINE_STDFH_LO));
    }
    else 
    {
    	vRegWriteFldMulti(TG_17,
        	  P_Fld(TG_LINE_STDFH_UP_NSTD, LINE_STDFH_UP)|
        	  P_Fld(TG_LINE_STDFH_LO_NSTD, LINE_STDFH_LO));
    }	
}
/**
 * @brief TVD ISR adjust Again up/dn threshold
 * @param bAgainCode  IN:AgainCode value 
 * @retval : NOON
 */
static void  _svAgainUpDnThr(UINT8 bAgainCode)
{
    if (bAgainCode < DFE_AGAIN_TH_SML)
    {
    	vRegWriteFldAlign(DFE_06,DFE_AGAIN_DN_RATIO_SML, AGC2_ACODE_DN_RATIO);
    	vRegWriteFldAlign(DFE_06,DFE_AGAIN_UP_RATIO_SML, AGC2_ACODE_UP_RATIO);
    }
    else
    {
    	vRegWriteFldAlign(DFE_06,DFE_AGAIN_DN_RATIO_NML, AGC2_ACODE_DN_RATIO);
    	vRegWriteFldAlign(DFE_06,DFE_AGAIN_UP_RATIO_NML, AGC2_ACODE_UP_RATIO);
    }

}

/**
 * @brief TVD ISR VCR_BV Setting
 * @param bNaState IN: Na State.
 * @param bMcDoneCnt IN:_sbTvd_McDone_cnt Value
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
#if TVD_VCR_BV
static void _svVcrBVSet(UINT16 wLLStatusAvg, UINT8 bNaState,  UINT8 bMcDoneCnt,RTvdStatus *pTvd3dStatus)
{
    UINT8 fgVCRBV;
	if(pTvd3dStatus->fgVPres&&(bNaState==NA_LOWNOISE))
	{
		fgVCRBV=fgHwTvdVCRBV();
		if(fgVCRBV!=pTvd3dStatus->fgIsVCRBV)
		{
			if((fgDrvTvdModeCntGt(bMcDoneCnt+10))&&fgVCRBV&&(!fgHwTvdVLock())&&(wLLStatusAvg>0x20))
			{
				vDrvTvdRstVCRBVCnt();
				vDrvTvdIncVCRBVCnt();
				vRegWriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
				vRegWriteFldAlign(TG_17, SV_OFF, MHSS_EN);
				vRegWriteFldAlign(TG_17, SV_OFF, MHSS);
				vRegWriteFldAlign(TG_04, SV_OFF, LF_AUTO);
				vRegWriteFldAlign(TG_04, TG_LF_KP0, LF_KP0);
				vRegWriteFldAlign(TG_04, TG_LF_KI1, LF_KI1);
			    //printf("\n bHwTvdABSDiffFv()>0x12 .............. \n");
				LOG(6,"[TVD_DBG_MSG] _svVcrBVSet LLOCK small gain !\n");
			}
			pTvd3dStatus->fgIsVCRBV=fgVCRBV;
		}
	}
}
#endif

#ifdef CC_MT8223
/**
 * @brief TVD ISR Hsync slice leve adjust Setting
 * @param bNaState IN: Na State.
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svHSlicer(UINT8 bNaState, RTvdStatus *pTvd3dStatus)
{
	if(pTvd3dStatus->bSigStatus == (UINT8)SV_VDO_STABLE)
	{
		if(bNaState!=NA_SUPERHIGHNOISE)
		{
			if(bHwTvdSYNCLV()< 0xA0)
			{
				vDrvTvdIncHsliceCnt();
				if(fgDrvTvdIsHsliceCnt(TVD_Hslice_threshold-1))
				{
					vRegWriteFldAlign(TG_0C, TG_HSLICE_NORMAL, HSLICE_ADJ);
					LOG(6,"[TVD_DBG_MSG] _svHSlicer TG_HSLICE_NORMAL !\n");
				}
			}
			else if(bHwTvdSYNCLV()> 0xA8)
			{
				vDrvTvdDecHsliceCnt();
				if(fgDrvTvdIsHsliceCnt(1))
				{
					vRegWriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
					LOG(6,"[TVD_DBG_MSG] _svHSlicer TG_HSLICE_DEFAULT !\n");
				}
			}
		}
		else
		{
			vDrvTvdRstHsliceCnt();
			vRegWriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
		}
	}
	else
	{
		vDrvTvdRstHsliceCnt();
		vRegWriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
	}
}
#endif

/**
 * @brief TVD ISR Channel Change 
 * @param IsTvdFixAgc IN: tvd fix AGC status
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svChanChg(RTvdStatus *pTvd3dStatus,UINT8 bIsTVDFixAgc)

{
    if (pTvd3dStatus->eSourceType==SV_ST_TV)
    {
        if (fgDrvTvdIsChanchgCnt(CHANCHG_START))
        {
			LOG(6,"[TVD_DBG_MSG] _svChanChg CHANCHG_START\n");
            // Fast Chroma Lock.
            #if TVD_FRS_FOR_NONSTDSIGNAL
            vDrvTvdRstNONSTDCnt();
            #endif
            // Reset HLock Cnt, OldVCR Cnt and Apply Auto Line Lock.
            vDrvTvdRstHLOCKCnt();
            //Enable LF_AUTO
            vRegWriteFldAlign(TG_04, SV_ON, LF_AUTO); 
            // Fast VLock.
            if (!IS_AUTOSEARCH())
            {
            	vRegWriteFldAlign(TG_08, SV_ON, FAST_VALIGN);
            }
            else
            {
            	vRegWriteFldAlign(TG_08, SV_OFF, FAST_VALIGN);
            }
            // To avoid channel change AGC speed too slow, so set to defaule gain during mode change.   Mark 070711
            if(bIsTVDFixAgc != SV_ON)
            {
                vTvdSetAGCMode(E_HOLD_GAIN);			
            }
        }
        else if (fgDrvTvdIsChanchgCnt(CHANCHG_DONE))
        {
			LOG(6,"[TVD_DBG_MSG] _svChanChg CHANCHG_DONE\n");
            vRegWriteFldAlign(TG_08, SV_OFF, FAST_VALIGN);
            // To avoid channel change AGC speed too slow, so set to defaule gain during mode change.   Mark 070711
            if(bIsTVDFixAgc != SV_ON)
            {
                vTvdSetAGCMode(E_AUTO_GAIN);			
            }
        }
    }
    vDrvTvdDecChanchgCnt();

}
/**
 * @brief TVD ISR STD/NSTD Line Setting
 * @param fgvpres IN: vpres Status.  
 * @param bNaState IN: Na State. 
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svStdNStdLineSet(UINT8 bNaState,RTvdStatus *pTvd3dStatus)

{
    (pTvd3dStatus->LineInfo).wLinecnt=RegReadFldAlign(TG_STA_05, STA_LCNT);
	if(!pTvd3dStatus->fgVPres)
    {		
      (pTvd3dStatus->LineInfo).wLinecnt=1716;
    }
	if (IS_LINE_NON_STD((pTvd3dStatus->LineInfo).wLinecnt))
	{
		vRegWriteFldAlign(DFE_01, DFE_BLANK_WIN_START_NSTD_L, BLANK_WIN_START);
		vRegWriteFldAlign(TG_0D, SV_ON, HLEN_FHPOS_EN);
        vRegWriteFldAlign(CDET_00, SV_ON, MDET_V525_SEL);
        vRegWriteFldAlign(TG_04, SV_ON, LF_OFFSET_EN); 
	}
	else if(IS_LINE_STD((pTvd3dStatus->LineInfo).wLinecnt))
	{
		vRegWriteFldAlign(DFE_01, DFE_BLANK_WIN_START_STD_L, BLANK_WIN_START);
		vRegWriteFldAlign(TG_0D, SV_OFF, HLEN_FHPOS_EN);
        vRegWriteFldAlign(CDET_00, SV_OFF, MDET_V525_SEL);
        vRegWriteFldAlign(TG_04, SV_OFF, LF_OFFSET_EN); 
	}
	if (bNaState == (UINT8)NA_LOWNOISE)
	{
		if (IS_LINE_NON_STD((pTvd3dStatus->LineInfo).wLinecnt))
		{
			vRegWriteFldAlign(CTG_05, SV_ON,BST_START_SEL);//?SHI
			vRegWriteFldAlign(CTG_00, SV_ON, SOBVLD_MASK_EN); //auto  //?SHI
		}
		else if(IS_LINE_STD((pTvd3dStatus->LineInfo).wLinecnt))
		{
			vRegWriteFldAlign(CTG_05, SV_OFF,BST_START_SEL);
			vRegWriteFldAlign(CTG_00, SV_OFF, SOBVLD_MASK_EN); //force valid
		}
    	if (fgHwTvdSVID())//?SHI
    	{
			vRegWriteFldAlign(CTG_00, SV_ON, SOBVLD_MASK_EN); //auto
		}
	}
	else
	{
		vRegWriteFldAlign(CTG_05, SV_OFF, BST_START_SEL);
		vRegWriteFldAlign(CTG_00, SV_OFF, SOBVLD_MASK_EN); //force valid
		if (fgHwTvdSVID())
		{
			vRegWriteFldAlign(CTG_00, SV_ON, SOBVLD_MASK_EN); //auto
		}
	}

}
/**
 * @brief TVD ISR Turn on/off Fast vlock
 * @param  IN:NOON
 * @retval : NOON
 */
#if TVD_FORCE_FAST_VALIGN
static void _svFastValign()
{    
    UINT16 weer_vlen;
    UINT16 wlock_vlen;	  
    weer_vlen=RegReadFldAlign(TG_STA_01, ERR_VLEN);
    wlock_vlen=RegReadFldAlign(TG_STA_01, LOCK_VLEN);

    if (weer_vlen >= wlock_vlen)
    {
        weer_vlen= weer_vlen-wlock_vlen;
    }
    if (fgDrvTvdIsChanchgCnt(CHANCHG_STOP))
    {
        if (weer_vlen > 20)
        {
        	vRegWriteFldAlign(TG_08,SV_ON,FAST_VALIGN);
//			LOG(6,"[TVD_DBG_MSG] _svFastValign FAST_VALIGN on \n");
        }
        else
        {
        	vRegWriteFldAlign(TG_08,SV_OFF,FAST_VALIGN);
        }
    }

}

#endif
/**
 * @brief TVD ISR CO IIR Check
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svCOIIRCHK(RTvdStatus *pTvd3dStatus, UINT8 bAvgCOCH_detect)
{
    if (!IS_AUTOSEARCH())
    {
    	if ((RegReadFldAlign(DFE_STA_08, COCH_IIR)>COIIR_TH_HIGH)&&(bHwTvdNRLevel()<0x40)) //Co-channel level
    	{
    		vDrvTvdIncCOCH2ENCnt();
    		if (fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT-1))
    		{
                #if TVD_VPRES_IMPROVE
    				vDrvTvd3dVPresOnOff(0x31, DFE_NORMAL_ONOFF_THRD);
                #endif

                #if TVD_SERRTYPE_ONE
                    vRegWriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
                #endif 
    			vRegWriteFldAlign(DFE_18,SV_ON,PVSYNC_COCH_EN);//pseudo-Vsync minimum point generate Vsync
    			vRegWriteFldAlign(VSRC_08, SV_ON, COCH2_EN);//line avg compensate CCI(CoChannel Interference)
				LOG(6,"[TVD_DBG_MSG] _svCOIIRCHK COCH2 Enable !\n");
    		}
    	}
        else if((RegReadFldAlign(DFE_STA_08, COCH_IIR)<COIIR_TH_LOW)||(bHwTvdNRLevel()>0x40))
    	{
    		vDrvTvdDecCOCH2ENCnt();
    		if (fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT2+1))
    		{
                #if TVD_SERRTYPE_ONE
                //Keep Original COCH setting. 
    			if((bAvgCOCH_detect)||(pTvd3dStatus->bTvdMode == SV_CS_PAL_M))
				{
				    vRegWriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
				}
    			else
				{
				    vRegWriteFldAlign(TG_07, SV_ON, SERR_TYPE);
				}
                #endif
    			vRegWriteFldAlign(DFE_18,SV_OFF,PVSYNC_COCH_EN);
    			vRegWriteFldAlign(VSRC_08, SV_OFF, COCH2_EN);
				LOG(6,"[TVD_DBG_MSG] _svCOIIRCHK COCH2 Disable !\n");
    		}
            #if TVD_VPRES_IMPROVE
    		else if(fgDrvTvdIsCOCH2ENCnt(TVD_COCH2EN_CNT2))
    		{
				if (pTvd3dStatus->fgVPres)
				{
					vDrvTvdIncVP1Cnt();
					if(fgDrvTvdVP1CntGt(TVD_VP1OFFON_threshold))
					{
						if(RegReadFldAlign(DFE_STA_07, VPRES_VFLAG))
							vDrvTvd3dVPresOnOff(0x21, DFE_NORMAL_ONOFF_THRD);
						else
							vDrvTvd3dVPresOnOff(0x35, DFE_NORMAL_ONOFF_THRD);
					}
					else
					{
						vDrvTvd3dVPresOnOff(0x21, DFE_NORMAL_ONOFF_THRD);
					}
				}
				else
				{
					if(bHwTvdNRLevel()>0x10)
					{
						vDrvTvdRstVP1Cnt();
						vDrvTvd3dVPresOnOff(0x35, DFE_NORMAL_ONOFF_THRD);
					}
					else
					{
						vDrvTvd3dVPresOnOff(0x12, DFE_NORMAL_ONOFF_THRD);
					}
				}
    		}
            #endif
    	}

    }
    else //Auto Search : SERR_TYPE = 0, Disable COCH2 and PVSYNC_COCH 
    {
		//vRegWriteFldAlign(TG_07, SV_OFF, SERR_TYPE);  // mtk70117 marked for finetune V-shifting
		vRegWriteFldAlign(DFE_18,SV_OFF,PVSYNC_COCH_EN);
		vRegWriteFldAlign(VSRC_08, SV_OFF, COCH2_EN);
    }  


}
/**
 * @brief TVD ISR NA State Check
 * @param bNaState IN: Na State. 
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svNAStateCHK(UINT8 bNaState, RTvdStatus *pTvd3dStatus)
{
    if (bNaState == (UINT8)NA_LOWNOISE)
	{
		if (pTvd3dStatus->bTvdMode==SV_CS_SECAM)
		{
			vRegWriteFldAlign(CTG_03,SV_OFF, CTG_VUP_EN);
		}
		else
		{
			if (pTvd3dStatus->bCAGC < 0x08)
			{
				vRegWriteFldAlign(CTG_03,SV_ON, CTG_VUP_EN);
			}
			else
			{
				vRegWriteFldAlign(CTG_03,SV_OFF, CTG_VUP_EN);
			}
		}
		vRegWriteFldAlign(CDET_00, SV_OFF, DET443_SEL);//DET443_SEL=1:det by f otherwise by energy
	}
	else // High and Super Hight NA
	{
	    vRegWriteFldAlign(CDET_00, SV_ON, DET443_SEL);
		vRegWriteFldAlign(CTG_03,SV_OFF, CTG_VUP_EN);
	}
}

#if TVD_NR_AVG
static UINT8 _svTvdGetNRLevel(void)
{
	return RF_level;
}
#endif

/**
 * @brief TVD ISR Mode switch protection
 * @param fgvpres IN: vpres Status.  
 * @param bTvdMode IN: bTvdMode
  * @param bMcDoneCnt IN:_sbTvd_McDone_cnt Value
 * @retval : NOON
 */
 #if TVD_MODESWITCH_PROTECT
static void _svModeSwPrtec(BOOL fgVPres,UINT8 bTvdMode, UINT8 bMcDoneCnt)
{
    if(fgDrvTvdIsModeCnt(bMcDoneCnt-9))
    {
        if(fgVPres)
        {
            if(RegReadFldAlign(STA_CDET_00, PHALT_TVD3D))
            {
                switch (bTvdMode)
                {
                    case SV_CS_NTSC358:
                    case SV_CS_NTSC443:
                    case SV_CS_SECAM:
                        //printf("TVD_MODESWITCH_PROTECT: Set PAL MODE=0 Cnt=%d\n", _bTvdModeCnt);
                        vRegWriteFldAlign(CDET_08, 0x0, CDET_PAL_MODE);
                        break;
                    default:
                        break;
                };
            }
        }
    }
    else if(fgDrvTvdIsModeCnt(bMcDoneCnt))
    {
        //printf("TVD_MODESWITCH_PROTECT: Set PAL MODE=3 Cnt=%d\n", _bTvdModeCnt);
        vRegWriteFldAlign(CDET_08, 0x3, CDET_PAL_MODE);
    }

}
 #endif
 /**
 * @brief TVD ISR Non STD FH Setting
 * @param  IN :NOON
 * @retval : NOON
 */
static void  _svNonStdFhSet(void)
{
    if (IS_FH_NON_STD()) //Non STD FH  include FH_POS and FH_NEG
    {
    	vRegWriteFldMulti(CDET_04,
                        P_Fld(SV_OFF, CTG_NA_SEL)|
                        P_Fld(SV_OFF, CDET_NA_SEL));
    	vRegWriteFldAlign(CTG_00, 0x0, CTG_SWLBF); //free run for chroma
    }
    else
    {
    	vRegWriteFldMulti(CDET_04,
                        P_Fld(SV_ON, CTG_NA_SEL)|
                        P_Fld(SV_ON, CDET_NA_SEL));
    	if (!fgHwTvdSVID())
    		vRegWriteFldAlign(CTG_00, 0x1, CTG_SWLBF);
    }
}
 /**
 * @brief TVD ISR HLOCK detection
 * @param  PfgHLOCK OUT:pointer to fghlock status
 * @retval : NOON
 */
static void  _svHLockDET(BOOL *PfgHLOCK )
{
    if (fgHwTvdHLock())
    {
    	vDrvTvdIncHLOCKCnt();
    	if (fgDrvTvdIsHLOCKCnt(TVD_HLOCKDONE_CNT-1))
    	{
    		*PfgHLOCK = 1;
    	}
    }
    else
    {
    	vDrvTvdDecHLOCKCnt();
    	if (fgDrvTvdIsHLOCKCnt(TVD_HLOCKDONE_CNT2+1))
    	{
    		*PfgHLOCK = 0;
    	}
    }

}
 
 /**
 * @brief TVD ISR Dclamp on/off condition check
 * @param bHwVCRSetting IN: HW VCR Setting.  
 * @param PTvd3dNSTDStatus INOUT:pointer to RTvdNSTDStatus status
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svDclampCHK(UINT8 bHwVCRSetting,RTvdNSTDStatus*PTvd3dNSTDStatus,RTvdStatus *pTvd3dStatus)
{
	if(((TVDAbsDiff(PTvd3dNSTDStatus->wHTotal,1716)<3)||(TVDAbsDiff(PTvd3dNSTDStatus->wHTotal,1728)<3))
		&&((TVDAbsDiff(PTvd3dNSTDStatus->wVTotal,525)<10) || (TVDAbsDiff(PTvd3dNSTDStatus->wVTotal,625)<10))
		&&(!pTvd3dStatus->fgIsCoChannel)
		&&((pTvd3dStatus->fgTrick == 0)&&(bHwVCRSetting==(UINT8)VCR_NONE))
	   )
	{
		if(_svTvdGetNRLevel()>75)
		{
			vDrvTvdIncDCLAMPCnt();
			if(fgDrvTvdIsDCLAMPCnt(TVD_DCLAMPON_CNT-1))
			{
				if(RegReadFldAlign(DFE_STA_05, CP_CUR)> 0x14)
				{
					vRegWriteFldAlign(DFE_08, SV_ON, DCLAMP_Y_EN);
					#ifdef CC_MT8223
					vRegWriteFldAlign(DFE_0E, SV_ON, DCLAMP_CHECK_LIM);	//new reg for 8223
					#endif
					vRegWriteFldAlign(DFE_02, TVD_DCLAMP_Y4H, Y4H_BW); 
					LOG(6,"[TVD_DBG_MSG] _svDclampCHK Dclamp Enable !\n");
				}
				else
				{
					vRegWriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN);
					#ifdef CC_MT8223
					vRegWriteFldAlign(DFE_0E, SV_OFF, DCLAMP_CHECK_LIM);	//new reg for 8223
					#endif
					vRegWriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW); 
					LOG(6,"[TVD_DBG_MSG] _svDclampCHK Dclamp CP_CUR Disable !\n");
				}
				vRegWriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VSAG, CLAMP_TABLE_05);
			}
		}
		else if(_svTvdGetNRLevel()<65)
		{
			vDrvTvdDecDCLAMPCnt();
			if(fgDrvTvdIsDCLAMPCnt(TVD_DCLAMPOFF_CNT+1))
			{
				vRegWriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW); 
				vRegWriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN); 
				vRegWriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
				LOG(6,"[TVD_DBG_MSG] _svDclampCHK Dclamp NRLevel Disable !\n");
			}
		}
	}
	else
	{
		vDrvTvdDecDCLAMPCnt();
		if(fgDrvTvdIsDCLAMPCnt(TVD_DCLAMPOFF_CNT+1))
		{
			vRegWriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW); 
			vRegWriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN); 
			vRegWriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
		}
	}

}
 /* @brief Set Secam Burst start/end window.
 * @param  bStart : start point 
 * @param  bEnd   : end point
 * @retval void
 */
static void _svSetSCMBurstWin(UINT8 bStart, UINT8 bEnd) reentrant
{
	vRegWriteFldMulti(CDET_02,P_Fld(bStart,BI_START)|P_Fld(bEnd,BI_END));
}

/**
 * @brief TVD ISR SECAM Detection 
 * @param bMcDoneCnt IN:_sbTvd_McDone_cnt Value
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svSecamDET(RTvdStatus *pTvd3dStatus, UINT8 bMcDoneCnt)
{
    #if TVD_SECAM_DET //when !BLOCK move windows to SECAM
	if (fgDrvTvdModeCntGt(bMcDoneCnt-2) && pTvd3dStatus->fgVPres)
	{
		if (pTvd3dStatus->fgIsBurstLock&&((pTvd3dStatus->bTvdMode==SV_CS_SECAM)||(pTvd3dStatus->bTvdMode==SV_CS_PAL)))
		{
			vDrvTvdIncBurstCnt();
			if (fgDrvTvdIsBurstCnt(TVD_BurstLock_CNT-1))
			{
                _svSetSCMBurstWin(0x40, 0x80);
				LOG(6,"[TVD_DBG_MSG] _svSecamDET BurstWin 0x4080\n");
			}
		}
		else
		{
			vDrvTvdDecBurstCnt();
			if (fgDrvTvdIsBurstCnt(TVD_BurstLock_CNT2+1))
			{
                _svSetSCMBurstWin(0x30, 0xb0);
				LOG(6,"[TVD_DBG_MSG] _svSecamDET BurstWin 0x30b0\n");
			}
		}
	}
    #else
	if ((pTvd3dStatus->bCAGC<DET443CAGCThres) && (!pTvd3dStatus->fgIsCoChannel) &&
			((pTvd3dStatus->bTvdMode!=SV_CS_SECAM)||(pTvd3dStatus->bSigStatus != (UINT8)SV_VDO_STABLE))
	   )
	{
		if (vDrvTvdGetTVDNRAvg() <= 0x09)
		{
            _svSetSCMBurstWin(0x40, 0x80);
		}
		else if (vDrvTvdGetTVDNRAvg() >= 0x0C)
		{
            _svSetSCMBurstWin(0x30, 0xa0);

		}
	}
	else
	{
		// Fix high noise SECAM sensitivity.
        _svSetSCMBurstWin(0x30, 0xb0);
	}
    #endif		
}
/**
 * @brief Set TVD Target Sync Height Value 
 * @param :Noon
 */
static void _svDrvSetTVDAGCTarget(UINT16 value)
{
	vRegWriteFldAlign(DFE_03, value, AGC2_TARGET_SYNC_HEIGHT);
}

/**
 * @brief TVD ISR COCH Adjust
 * @param fgCoChannel IN: fgIsCoChannel Status. 
 * @param wAGCTargetSyncHeight IN: _swAGCTargetSyncHeight value
 * @retval : NOON
 */
static void _svCochAdj(BOOL fgCoChannel,UINT16 wAGCTargetSyncHeight)
{
    if (fgCoChannel)
    {
    	vDrvTvdIncCOCHDeCnt();
    	if(fgDrvTvdIsCOCHDeCnt(TVD_COCHDe_CNT-1))
    	{
    		vRegWriteFldAlign(DFE_02, SV_OFF, BLV_LIM_AUTO); //Avoid cochannel wrong measure.
    		_svDrvSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_COCHANNEL);
    		vRegWriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_CCI, BLANK_BW);//test impluse noise
			LOG(6,"[TVD_DBG_MSG] _svCochAdj CoChannel detected!\n");
    	}
    }
    else
    {
    	vDrvTvdDecCOCHDeCnt();
    	if(fgDrvTvdIsCOCHDeCnt(TVD_COCHDe_CNT2+1))
    	{
    		vRegWriteFldAlign(DFE_02,SV_ON, BLV_LIM_AUTO);
    		_svDrvSetTVDAGCTarget(wAGCTargetSyncHeight);
    		vRegWriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_NML, BLANK_BW);//test impluse noise
			LOG(6,"[TVD_DBG_MSG] _svCochAdj CoChannel non-detected!\n");
    	}
        if(RegReadFldAlign(STA_CDET_01, DCNT) < HSYNCPOS_DCNT_TH1 || RegReadFldAlign(STA_CDET_01, DCNT) > HSYNCPOS_DCNT_TH2)
        {
            vDrvTvdIncHsyncPosDetCnt();
        }
        else
        {
            vDrvTvdDecHsyncPosDetCnt();
        }
        if(_na_state==NA_LOWNOISE && (!fgHwTvdCNRLow()) && (!fgHwTvdVCRBV()))
        {
            if(fgDrvTvdIsHsyncPosDetCnt(TVD_HSYNCPOS_CNT-1))
            {
                vRegWriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_CCI, BLANK_BW);//NSTD: hsync position
//				LOG(6,"[TVD_DBG_MSG] _svCochAdj NSTD hsync position detected!\n");
            }
			else if(fgDrvTvdIsHsyncPosDetCnt(TVD_HSYNCPOS_CNT2+1))
			{
                vRegWriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_NML, BLANK_BW);//test impluse noise
//				LOG(6,"[TVD_DBG_MSG] _svCochAdj NSTD hsync position non-detected!\n");
			}
        }
    }
}
/* @brief TVD ISR get NR average under non auto search
 * @param fgTrick IN: Trick Status. 
 * @param bHwVCRSetting IN: HW VCR Setting. 
 * @param bNaState IN: Na State. 
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svGetNRAvg(UINT16*PwTVDNRavg,UINT16 *PwTVDNRavg1,RTvdStatus *pTvd3dStatus)
{
    if (IS_AUTOSEARCH())
	{
		//Use old vpres method when searching channel.
		vRegWriteFldMulti(DFE_0E,P_Fld(0, VPRES_SEL)|P_Fld(0x01, VPRES_MASK)); //Check VPRES 1

		if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_EU)
		{
			vDrvTvd3dVPresOnOff(0x14, DFE_AUTOSRCH_ONOFF_THRD);
		}
		else
		{
			vDrvTvd3dVPresOnOff(0x3c, DFE_AUTOSRCH_ONOFF_THRD);
		}
	}
//-----------------Caulcate wTVDNRavg & _swTVDNRavg1 in non auto search mode------// 
	else if (!fgHwTvdVLock() || !fgHwTvdVPres())
	{
		pTvd3dStatus->NAInfo.na_HysterCnt=0;
			
	}
	else if ((pTvd3dStatus->NAInfo).na_HysterCnt>(pTvd3dStatus->NAInfo).na_HysterCntThres) //Caculate the statistics only when vpres and vlock on.
	{
		(pTvd3dStatus->NAInfo).na_cnt++;
		(pTvd3dStatus->NAInfo).na_sum+= bHwTvdNRLevel();
		if ((pTvd3dStatus->NAInfo).na_cnt == (UINT8)NA_SIZE)
		{
			*PwTVDNRavg =(pTvd3dStatus->NAInfo).na_sum/ NA_SIZE;
			(pTvd3dStatus->NAInfo).na_cnt = 0;
			(pTvd3dStatus->NAInfo).na_sum = 0;
			/* move to _svTvd3dBHNAStateChg() */
			*PwTVDNRavg1 = ((*PwTVDNRavg1* 3) +*PwTVDNRavg) >> 2;  //SW IIR
			if(*PwTVDNRavg1 > 0x80) *PwTVDNRavg1 = 0x80;
		}
	}


}
/**
 * @brief TVD ISR Trick Mode detection 
 * @param fgTrick IN: Trick Status. 
 * @param bHwVCRSetting IN: HW VCR Setting. 
 * @param bNaState IN: Na State. 
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svTrickModeDET(BOOL fgTrick, UINT8 bHwVCRSetting, UINT8 bNaState, RTvdStatus *pTvd3dStatus)
{	 

  if ((fgTrick && (!(pTvd3dStatus->eSourceType==SV_ST_TV))) ||
			(IS_AUTOSEARCH()&&(pTvd3dStatus->eSourceType==SV_ST_TV)))
	{
		//To fix trick mode brightness increasing.
		vRegWriteFldMulti(DFE_0E,P_Fld(0, VPRES_SEL)|P_Fld(0x01, VPRES_MASK)); //Check VPRES 1
	}
	else
	{
		//To fix PICO Vpres toggle.//VPRES1 && VPRES2 for Special tuner
		vRegWriteFldMulti(DFE_0E,P_Fld(1, VPRES_SEL)|P_Fld(0x04, VPRES_MASK)); //Check VPRES 1 & VPRES 2
	}

	if (
        #if !TVD_NO_BYPASS_TRICK
		(_sbBypassTrick == 0x00) &&
        #endif
		(pTvd3dStatus->bSigStatus == (UINT8)SV_VDO_STABLE)) // CYJ 2006.01.07
	{
		if (pTvd3dStatus->fgTrick!=fgTrick)
		{
			pTvd3dStatus->fgTrick=fgTrick;
			if (fgIsMainTvd3d())
			{
				vDrvDITrickModeOnOff(SV_VP_MAIN,  (pTvd3dStatus->fgTrick)?SV_ON:SV_OFF);
			}
			else
			{
				vDrvDITrickModeOnOff(SV_VP_PIP,  (pTvd3dStatus->fgTrick)?SV_ON:SV_OFF);
			}
		}
		// CYJ 2006.01.07 Trick mode and VCR mode
		if ((pTvd3dStatus->fgTrick > 0)&&(bHwVCRSetting==(UINT8)VCR_MODE1))
		{
			//e77f4->e7ff4. Fix trick mode color flickering.  Benson.06.01.25
            #if !TVD_8222_NONSTANDARD
			// shorten CAGC large step
			vRegWriteFldAlign(CDET_08, 0x03, CAGC_L_STEP);
            #endif

			switch (pTvd3dStatus->bTvdMode)
			{
    			case SV_CS_PAL_N:
    			case SV_CS_PAL:
    			case SV_CS_PAL_M:
    			case SV_CS_PAL_60:
    				//To avoid Trick mode no color.
    				vRegWriteFldAlign(CTG_01, SV_ON, BST_DET);
    				break;
    			default:
    				vRegWriteFldAlign(CTG_01, SV_OFF, BST_DET);
    				break;
			}
		}
		else //non trick mode 
		{
			if (bNaState==NA_LOWNOISE)  // Low noise
			{
                #ifndef CC_COPLAT_MT82 
				UNUSED(pTvd3dStatus->fgIsCkill);
                #endif 

				//Upper scean magenta flamesh. Benson. 060314
				if (pTvd3dStatus->bTvdMode!=SV_CS_SECAM)
				{
					if (fgHwTvdIsMacroVision())
					{
						vRegWriteFldAlign(CTG_01, SV_OFF, BST_DET);
					}
					else
					{
						vRegWriteFldAlign(CTG_01, SV_ON, BST_DET);
					}
				}
				else
				{
					vRegWriteFldAlign(CTG_01, SV_OFF, BST_DET);
				}
			}
			else
			{
				//Color Sensitivity. Benson.060213.
				if (pTvd3dStatus->bTvdMode==SV_CS_PAL)
				{
					vRegWriteFldAlign(CTG_01, SV_ON, BST_DET);
				}
				else
				{
					vRegWriteFldAlign(CTG_01, SV_OFF, BST_DET);
				}
			}
            #if !TVD_8222_NONSTANDARD
			// CAGC large step
			vRegWriteFldAlign(CDET_08, 0x05, CAGC_L_STEP);
            #endif
		}
	}
	
	
}

 /* @brief Adjust CAGC loop gain to shorten the transient time  
 * @param IN:bCAGC
 * @retval : NOON
 */
 #if   TVD_AntiTrs
static void _svTvdAntiTrs(UINT8 bcagc,UINT8 *fgcagc,UINT8 *_sbCAGC1)
{ 
 
   if((*_sbCAGC1>bcagc+15)&&(*fgcagc==0))	 	
	 {
	    *fgcagc=1; 
	_bTvdISRCnt=0;
	 vRegWriteFldAlign(CDET_0B, 0x05, LARGE_LOOP_GAIN);
//        printf("\n bcagc1=%d,bcagc=%d \n",*_sbCAGC1,bcagc); 		
		LOG(6,"[TVD_DBG_MSG] _svTvdAntiTrs bcagc1=%d, bcagc=%d \n",_sbCAGC1,bcagc);
         
	 }
	
  if((_bTvdISRCnt>=0x20)&&(*fgcagc==1))
	     {
	         
	        * fgcagc=0;
	       // _bTvdISRCnt=0;	       
//		   printf("\n the changed bcagc:bCAGC=%d \n",bHwTvdCAGC());
		   LOG(6,"[TVD_DBG_MSG] _svTvdAntiTrs the changed bcagc:bCAGC=%d \n",bHwTvdCAGC());
		  vRegWriteFldAlign(CDET_0B, 0x02,  LARGE_LOOP_GAIN);	
	      }  
	
	 * _sbCAGC1=bcagc;  
}
 #endif


static void _svTvd3dSetCAgcUVPeak(UINT8 bMode);


#if CAGC_PEAK_CNR_ADJ
/**
 * @brief Adjust CAGC Peak paramater based on CNR. 
 * @param IN:fgCNRLow : HW CNR value
 * @param IN:fgTrick  : HwTrick Mode Flag. 
 * @retval : NOON
 */
static void _svTvdAdjustCAGCPeak( BOOL fgCNRLow , BOOL fgTrick, UINT8 bMode)
{
    if (fgCNRLow && fgTrick)    
    {
	    vRegWriteFldAlign(CDET_08, CDET_CAGC_PEAK_TC_CNR, CAGC_PEAK_TC);
		vRegWriteFldAlign(CDET_09, CAGC_PEAK_U_CNR, CAGC_PEAK_U);
		vRegWriteFldAlign(CDET_09, CAGC_PEAK_V_CNR, CAGC_PEAK_V);
    }
    else
    {
	    vRegWriteFldAlign(CDET_08, CDET_CAGC_PEAK_TC, CAGC_PEAK_TC);
        _svTvd3dSetCAgcUVPeak(bMode);
    }
}
#endif 

/**
 * @brief Get TVD VCR BV status after On/Off Gain. 
 * @param IN:fgCurVCRBV : Current HW status
 * @param INOUT:pVCRBVSta : pointer VCR BV status for FW referenced. 
 * @retval : NOON
 */
 #if TVD_VCR_BV_STA
static void _sbTvd3dSetVCRBVSta(UINT8 fgCurVCRBV, UINT8 *pVCRBVSta)
{

    if (fgCurVCRBV)
    {
        vDrvTvdIncVCRBVStaCnt();
    }
    else
    {
        vDrvTvdDecVCRBVStaCnt();
    }
    
    if (fgDrvTvdIsVCRStaCnt(TVD_IS_VCRBVSTAON))
    {   
            *pVCRBVSta = 1;
    }
    else if (fgDrvTvdIsVCRStaCnt(TVD_IS_VCRBVSTAOFF))
    {
            *pVCRBVSta = 0;

    }
}
#endif

/**
 * @brief Reset TVD Non Standard status. 
 * @param IN:fgIs525 : TRUE: 525 / FALSE: 625
 * @param OUT:pRTvdNSTDStatus : pointer to RTvdNSTDStatus structure variables
 * @retval : NOON.
 */
 #if TVD_FRS_FOR_NONSTDSIGNAL
static void _svTvd3dRstNSTStatus(BOOL fgIs525, RTvdNSTDStatus* pRTvdNSTDStatus)
{
	LOG(6,"[TVD_DBG_MSG] _svTvd3dRstNSTStatus \n");
	if(fgIs525)
	{
        pRTvdNSTDStatus->wVTotal = 525;
        pRTvdNSTDStatus->wHTotal = 1716;
        pRTvdNSTDStatus->bRefreshRate = 60;
	}
	else
	{
        pRTvdNSTDStatus->wVTotal = 625;
        pRTvdNSTDStatus->wHTotal = 1728;
        pRTvdNSTDStatus->bRefreshRate = 50;	
	}
	pRTvdNSTDStatus->fgIsNSTD = 0;
}
#endif

/**
 * @brief Vpres Status state machine. 
 * @param :pVpresState IN/OUT: Pointer to Vpres State
 * @retval : VPRES status.
 */
#if TVD_VPRES_STATE
static UINT8 _sbTvdVpresStateMachine(UINT8 *pVpresState)
{
    UINT8 bCurVpresState = *pVpresState;
    UINT8 fgRetVpres;

	switch(bCurVpresState)
	{
		case VPRES_STATE_0:  // VPRES4 OFF,VPRES3 OFF
			fgRetVpres = 0;
			vRegWriteFldAlign(DFE_0E, SV_OFF, VPRES4TVD_MODE); 
			if(RegReadFldAlign(STA_CDET_00, VPRES_TVD3D))
			{
				vDrvTvdIncVpresStateCnt1();
				if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
				{
					*pVpresState = VPRES_STATE_1; //Next State
					vDrvTvdRstVpresStateCnt1();
					fgRetVpres=1;
					LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_1\n");
				}
			}
			else
			{
				vDrvTvdRstVpresStateCnt1();
			}
			break;
		case VPRES_STATE_1:  // Vpres4 Off,Vpres3 On
			fgRetVpres = 1;
			if(RegReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))
			{
				*pVpresState = VPRES_STATE_3; //Next State
				vDrvTvdRstVpresStateCnt1();
				LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_3\n");
			}
			else
			{
				if(!RegReadFldAlign(STA_CDET_00, VPRES_TVD3D))
				{
					vDrvTvdIncVpresStateCnt1();
					if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
					{
						*pVpresState=VPRES_STATE_0; //Next State
						vDrvTvdRstVpresStateCnt1();
						fgRetVpres=0;
						LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_0\n");
					}
				}
				else
				{
					vDrvTvdRstVpresStateCnt1();
				}
			}
			break;
		case VPRES_STATE_2:  // Vpres4 on,Vpres3 off
			fgRetVpres = 1;
			if(!RegReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))
			{
				*pVpresState = VPRES_STATE_0; //Next State
				vDrvTvdRstVpresStateCnt1();
				fgRetVpres=0;
				LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_0\n");
			}
			else
			{
				if(!RegReadFldAlign(STA_CDET_00, VPRES_TVD3D))
				{
					vDrvTvdRstVpresStateCnt1();
				}
				else
				{
					vDrvTvdIncVpresStateCnt1();
					if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
					{
						
						*pVpresState = VPRES_STATE_3; //Next State
						vDrvTvdRstVpresStateCnt1();
						LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_3\n");
					}
				}
			}
			break;
		case VPRES_STATE_3:  // Vpres4 on,Vpres3 on
			fgRetVpres = 1;
			vRegWriteFldAlign(DFE_0E, SV_ON, VPRES4TVD_MODE); 	
			if(!RegReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))
			{
				*pVpresState = VPRES_STATE_1; //Next State
				vDrvTvdRstVpresStateCnt1();	
				LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_1\n");
			}
			else
			{
				if(RegReadFldAlign(STA_CDET_00, VPRES_TVD3D))
				{
					vDrvTvdRstVpresStateCnt1();
				}
				else
				{
					vDrvTvdIncVpresStateCnt1();
					if(fgDrvTvdIsVpresStateCnt1(VPRES_TH))
					{
						*pVpresState = VPRES_STATE_2; //Next State
						vDrvTvdRstVpresStateCnt1();
						LOG(6,"[TVD_DBG_MSG] _sbTvdVpresStateMachine change to VPRES_STATE_2\n");
					}
					
				}
				
			}
			break;
		case VPRES_STATE_4:  // Only Check Vpres3
			fgRetVpres = RegReadFldAlign(STA_CDET_00, VPRES_TVD3D);
			vRegWriteFldAlign(DFE_0E, SV_OFF, VPRES4TVD_MODE); 
			break;
		default:
		    fgRetVpres = 0;
			break;
	}
    return fgRetVpres;
		
}
#endif

/**
 * @brief Reset VPRES State machine.
 * @param :Noon
 * @retval : Nood
 */
static void _svTvdRstVpresState()
{
#if TVD_VPRES_STATE
    #ifdef CC_MT5363_TVD
    if(_rTvd3dStatus.eSourceType!=SV_ST_SV)
    #else
    if((_rTvd3dStatus.eSourceType!=SV_ST_SV)&&(IS_5363_NonECO()||IS_5363_ECO()||IS_5387_ECO()))
    #endif
    {
        _sbVpresState=VPRES_STATE_0;
        vDrvTvdRstVpresStateCnt1();
    }
    else
    {
        _sbVpresState=VPRES_STATE_4;
    }
    fgPreVPres_0=0;
#endif
}


/**
 * @brief Set Default TVD Menu Av Delay.
 * @param :bTvdMode: TVD Mode 
 * @retval Default AvDelay Value
 */
static UINT16 _swTvdGetDftMenuAvDelay(UINT8 bTvdMode)
{
    UINT16 wTvd_DftAvdelay = 0;
	if (bTvdMode >= sizeof(_sTvd_MenuAvDelay_Tbl)) 
	    goto EXIT;
#if TVD_HSHAKING_WA
        wTvd_DftAvdelay = _sTvd_MenuAvDelay_Tbl[bTvdMode];
#else
	if ((_na_state != (UINT8)NA_SUPERHIGHNOISE) || (_sbTVDFreeRun == 1))//super high noise
    {
        wTvd_DftAvdelay = _sTvd_MenuAvDelay_Tbl[bTvdMode];
    }
	else
    {
        wTvd_DftAvdelay = _sTvd_SHNA_MenuAvDelay_Tbl[bTvdMode];
    }
#endif    
EXIT:
    return wTvd_DftAvdelay;

}


/**
 * @brief Set TVD Menu Av Delay.
 * @param :bTvdMode: TVD Mode 
 * @retval AvDelay Value
 */
static UINT16 _swTvdGetMenuAvDelay(UINT8 bTvdMode)
{
    UINT16 wRetAvDelay = 0;

	switch (bTvdMode)
	{
    	case SV_CS_PAL_60:
    	case SV_CS_PAL_M:
        case SV_CS_NTSC358:
        case SV_CS_NTSC443:
        	wRetAvDelay = RegReadFldAlign(TG_07, AV_START_N);    
            break;

    	case SV_CS_SECAM:
    	case SV_CS_PAL:
        case SV_CS_PAL_N:
            wRetAvDelay = RegReadFldAlign(TG_07, AV_START_P);    
            break;

		default:
			assert(0);
			break;
	}
    return wRetAvDelay;
}

/**
 * @brief Set TVD Menu Av Delay.
 * @param :bTvdMode: TVD Mode.
 * @param :wAvDelay: Av Delay Value. 
 * @retval VOID
 */
static void _svTvdSetMenuAvDelay(UINT8 bTvdMode, UINT16 wAvDelay)
{
	switch (bTvdMode)
	{
    	case SV_CS_PAL_60:
    	case SV_CS_PAL_M:
        case SV_CS_NTSC358:
        case SV_CS_NTSC443:
        	vRegWriteFldAlign(TG_07, wAvDelay, AV_START_N);    
            break;

    	case SV_CS_SECAM:
    	case SV_CS_PAL:
        case SV_CS_PAL_N:
            vRegWriteFldAlign(TG_07, wAvDelay, AV_START_P);
            break;

		default:
			assert(0);
			break;
	}
}
/**
 * @brief  Set Tvd3D AV Delay by Mode
 * @param  bTvdMode: TVD Modes 
 * @retval void
 */
static void _svTvd3dSetAVDelay(UINT8 bTvdMode)
{
	UINT16 wTVD_AVDelay;
    wTVD_AVDelay = _swTvdGetDftMenuAvDelay(bTvdMode);
    _svTvdSetMenuAvDelay(bTvdMode, _su1UIDelay + wTVD_AVDelay);
	
}


/**
 * @brief TVD ISR H Lock FineTune (KI/KP Gain, Menu/Auto Linelock, HSS Fine Tune). 
 * @param bNaState IN: Na State. 
 * @param pTvd3dStatus IN :pointer to RTvdStatus status
 * @retval : NOON
 */
static void  _svTvdHFineTune(UINT8 bNaState,  RTvdStatus *pTvd3dStatus)
{
    #if TVD_HSHAKING_WA
    static UINT8 _sbHShakingCnt = 0;
    #endif

    if (( bNaState==NA_SUPERHIGHNOISE)&& (pTvd3dStatus->fgVPres) && (!IS_AUTOSEARCH()))  
	{
	    #if !TVD_HSHAKING_WA
		if(_svTvdGetNRLevel() > 35)
		{
			vRegWriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
			vRegWriteFldAlign(TG_09, SV_ON, IIR_LLOCK_EN);
		}
		else if(_svTvdGetNRLevel() > 32)
		{
			vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
		}
		else
		{
            #ifdef CC_COPLAT_MT82		
			if(eTVDLLockFreeRun )
            #else
			if(DRVCUST_OptGet(eTVDLLockFreeRun)  )
            #endif
			{
				vRegWriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
			}
			else
			{
				vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
			}
		}
	    #else
		vRegWriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
		vRegWriteFldAlign(TG_17, SV_ON, MHSS_EN);
		vRegWriteFldAlign(TG_17, SV_ON, MHSS);
		vRegWriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);
		vRegWriteFldAlign(TG_04, SV_ON, LF_AUTO);
		if(_svTvdGetNRLevel() > 35)
		{
			vRegWrite4B(TG_13, TG_KIKP_GAIN);
			vRegWriteFldAlign(TG_04, TG_FAST_KP, FAST_KP);
			_sbHShakingCnt = 0;
		}
		else
		{
			_sbHShakingCnt ++;
			if(_sbHShakingCnt == 0x4)
			{
				vRegWriteFldAlign(TG_04, TG_FAST_KP_TMP, FAST_KP);
				vRegWrite4B(TG_13, TG_KIKP_GAIN_TMP);
			}
			else if(_sbHShakingCnt == 0x8)
			{
				vRegWriteFldAlign(TG_04, TG_FAST_KP_SHNA, FAST_KP);
				vRegWrite4B(TG_13, TG_KIKP_GAIN_SHNA);
			}
		}
	    #endif
	}
	else
	{
		#if TVD_HSHAKING_WA
		vRegWrite4B(TG_13, TG_KIKP_GAIN);
		vRegWriteFldAlign(TG_04, TG_FAST_KP, FAST_KP);
		#endif
		#if TVD_VCR_BV
		//if(fgDrvTvdVCRBVCntGt(1))
		{
			vDrvTvdIncVCRBVCnt();
			if(fgDrvTvdVCRBVCntGt(TVD_VCR_BV_CNT))
			{
				vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
				vRegWriteFldAlign(TG_04, SV_ON, LF_AUTO);
				#if TVD_HSHAKING_WA
				vRegWriteFldAlign(TG_17, SV_OFF, MHSS_EN);
				vRegWriteFldAlign(TG_17, SV_OFF, MHSS);				
				_sbHShakingCnt = 0;
				#endif
			}
		}
		#else
		vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
		#endif
	}
    #ifdef CC_COPLAT_MT82		
	if((eTVDLLockFreeRun) && (pTvd3dStatus->fgVPres) && (!IS_AUTOSEARCH()) )
    #else
	if(DRVCUST_OptGet(eTVDLLockFreeRun) && (pTvd3dStatus->fgVPres) && (!IS_AUTOSEARCH()) )
    #endif
	{

		vRegWriteFldAlign(TG_16, 0, LLOCK_ERRWIN_SEL);//Disable Free Run function

		if(_svTvdGetNRLevel() < 32) //Low Signal 
		{
			//vRegWriteFldAlign(TG_0B, SV_OFF, AUTO_MLLOCK);
			vRegWriteFldAlign(TG_04, SV_OFF, LF_AUTO);
            #if !TVD_HSHAKING_WA
			_sbTVDFreeRun = 1;
            #endif
			if(fgHwTvdHsyncLock())
			{
				vRegWriteFldAlign(TG_16, 1, LLOCK_ERRWIN_SEL);//Enable Free Run function	
				vRegWriteFldAlign(TG_00, TG_MAX_HWIDTH_FRUN, MAX_HWIDTH);//Let HW free run
			}
			else
			{
				vRegWriteFldAlign(TG_00, TG_MAX_HWIDTH_NML, MAX_HWIDTH);
			}
		}
		else
		{
			//vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
			vRegWriteFldAlign(TG_04, SV_ON, LF_AUTO);
			vRegWriteFldAlign(TG_00, TG_MAX_HWIDTH_NML, MAX_HWIDTH);
			#if !TVD_HSHAKING_WA
			_sbTVDFreeRun = 0;
			#endif
		}
		_svTvd3dSetAVDelay(pTvd3dStatus->bTvdMode);
	}

}
/**
 * @brief Set TVD CAGC Bound for Data Patch.
 * @param :bMidB: Lower Bound of target burst tolerance
 * @param :bMidA: Upper Bound of target burst tolerance
 * @param :bTgtB: Lower Bound of target burst level
 * @param :bTgtA: Upper Bound of target burst level
 */
static void _svTvdSetCAGCBound4Data(UINT8 bMidB, UINT8 bMidA, UINT8 bTgtB, UINT8 bTgtA) reentrant
{

	if(_sbIsSetCAGCbyAP == SV_ON) 
    {
        return;
    }
    vRegWriteFldMulti(SECAM_03,P_Fld(bMidB,CAGC_IP_MID_B)|P_Fld(bMidA,CAGC_IP_MID_A));
    #ifdef CC_MT8223
    vRegWriteFldMulti(SECAM_05, P_Fld(bTgtB, CAGC_IP_TARGET_B) | P_Fld(bTgtA, CAGC_IP_TARGET_A));
    #else
    vRegWriteFldMulti(SECAM_04,P_Fld(bTgtB,CAGC_IP_TARGET_B)|P_Fld(bTgtA,CAGC_IP_TARGET_A));
    #endif
}


/**
 * @brief Set TVD CAGC Bound for Ctrl Patch.
 * @param :dwBound: MID_B[31:25]MID_A[23:16]TARGET_B[15:8]TARGET[7:0]
 */
static void _svTvd3dSetCAGCBound4Ctrl(UINT32 dwBound)
{
	vRegWrite4B(CDET_07, dwBound);// CAGCBound
}


/**
 * @brief Set TVD Burst lock threshold
 * @param :wThr1: burst lock --> unlock threshold
 * @param :wThr2: burst unlock --> lock threshold
 */
static void _svTvdSetBstLockTh (UINT16 wThr1, UINT16 wThr2) reentrant
{
    vRegWriteFldMulti(CTG_01,P_Fld(wThr2,BSTLOCK_THR2)
                        |P_Fld(wThr1,BSTLOCK_THR1));
}



/**
 * @brief Get TVD Target Sync Height Value 
 * @param :Noon
 */
static UINT16 _swDrvGetTVDAGCTarget(void)
{
	return RegReadFldAlign(DFE_03, AGC2_TARGET_SYNC_HEIGHT);
}

/**
 * @brief Set TVD Blank Level for YUV-->YCBCR 
 * @param :bTVDMode: TVD Mode 
 * @param :u2BlkLevel: YUV Blank Level 
 */
static void _svDrvSetTVDBlk(UINT8 bTVDMode, UINT16 u2BlkLevel)
{
	switch (bTVDMode)//(_rTvd3dStatus.bTvdMode)
	{
		case SV_CS_SECAM:
		case SV_CS_PAL:
		case SV_CS_PAL_60:
		case SV_CS_PAL_N:
		    _svDrvSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_625); //5391 Calib by TG35
			vRegWriteFldAlign(DFE_04, ((_rTvd3dStatus.eSourceType==SV_ST_TV)?DFE_PEAKY_TV_625 : DFE_PEAKY_OTHERS_625)+(u2BlkLevel-DFE_BLANK_TARGET), AGC2_PY_LIM);
				vRegWriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_PAL, BLKLVL);//support ultra black
			break;
		case SV_CS_PAL_M:
		case SV_CS_NTSC358:
		case SV_CS_NTSC443:
		    _svDrvSetTVDAGCTarget(DFE_TGT_SYNC_HEIGHT_525); //5391 Calib by TG35
			vRegWriteFldAlign(DFE_04, ((_rTvd3dStatus.eSourceType==SV_ST_TV)?DFE_PEAKY_TV_525 : DFE_PEAKY_OTHERS_525)+(u2BlkLevel-DFE_BLANK_TARGET), AGC2_PY_LIM);
			if (_bAgcPedEn)
			{
				vRegWriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_NTSCJ, BLKLVL);
			}
			else
			{
				vRegWriteFldAlign(CTG_07, u2BlkLevel+DFE_BLANK_OFFSET_NTSC, BLKLVL);// MT5381TVDPort CTG Port BLKLVL
			}
			break;
		default:
			assert(0);
			break;
	}
}

/**
 * @brief Set TVD Anti-Aliasing Filter
 * call by 
 * @param  bValue: 0:7.5HZ, 1:4.2MHZ, 2:5.0MHZ, 3:5.7MHZ, 4:6.0MHZ, 5:6.5MHZ
 * @retval void
 */
static void _svTvd3dSetAAF(UINT8 bValue)
{
	if(_sbIsSetAAFbyAP == SV_ON) return;
	vRegWriteFldAlign(VSRC_07, bValue, AAF_SEL);
}

#if TVD_LLOCK_STATUS
/**
 * @brief  Set Tvd3D AV Delay by Mode
 * @param  bTvdMode: TVD Modes 
 * @retval Avg Line Error 
 */
static UINT16 _svTvd3dChkLineStatus (void)
{
    static UINT16 _wLLStatus_tmp;
    static UINT8  _bLLcnt;

//	if (fgHwTvdHLock())
	{
		if (_bLLcnt < 15)
		{
			_wLLStatus_tmp+=RegReadFldAlign(TG_STA_03, LLOCK_STATUS);
			_bLLcnt++;
		}
		else
		{
			_swLLStatus_avg=_wLLStatus_tmp/16;
			_wLLStatus_tmp=RegReadFldAlign(TG_STA_03, LLOCK_STATUS);
			_bLLcnt=0;
		}
	}
	return _swLLStatus_avg;
}
#endif


/**
 * @brief  Noise Status Reset Function
 * @param  void
 * @retval void 
 */
static void _svTvdNAReset(void)
{
	vRegWriteFldAlign(TG_09, SV_OFF, IIR_LLOCK_EN);

    #if !TVD_NO_BYPASS_TRICK
	_sbBypassTrick  = 0x00;
    #endif

    vRegWriteFldAlign(DFE_18, SV_ON, VMAXMIN_EN);
	//Set CAGC bound & Burst unlock threshold according to TvdMode in Low Noise Sate.
	switch (_rTvd3dStatus.bTvdMode)
	{
    	case SV_CS_SECAM:
    		//Set normal burst unlock threshold under low noise.
    	case SV_CS_PAL_N:
    	case SV_CS_PAL:
    	case SV_CS_PAL_M:
    	case SV_CS_PAL_60:
    		//Set normal burst unlock threshold under low noise.
            _svTvdSetBstLockTh(CTG_BSTLOCK_THR1_LN_625, CTG_BSTLOCK_THR2_LN_625);    		
    		break;
    	case SV_CS_NTSC358:
    	case SV_CS_NTSC443:
    		//Set normal burst unlock threshold under low noise.
            _svTvdSetBstLockTh(CTG_BSTLOCK_THR1_LN_525, CTG_BSTLOCK_THR2_LN_525);    		
    		break;
    	default:
    		assert(0);
    		break;
	}

	switch (_rTvd3dStatus.bTvdMode)
	{
    	case SV_CS_SECAM:
    		_svTvdSetCAGCBound4Data(SCM_MID_B, SCM_MID_A, SCM_TGT_B, SCM_TGT_A);
    		break;
    	case SV_CS_PAL:
    	case SV_CS_PAL_60:
    	case SV_CS_PAL_N:
    		_svTvdSetCAGCBound4Data(PAL_MID_B, PAL_MID_A, PAL_TGT_B, PAL_TGT_A);
    		break;
    	case SV_CS_PAL_M:
    	case SV_CS_NTSC358:
    	case SV_CS_NTSC443:
    		if (_bAgcPedEn)
    		{
        		_svTvdSetCAGCBound4Data(NTSC_MID_B, NTSC_MID_A, NTSC_TGT_B, NTSC_TGT_A);		
    		}
    		else
    		{
        		_svTvdSetCAGCBound4Data(NTSC_J_MID_B, NTSC_J_MID_A, NTSC_J_TGT_B, NTSC_J_TGT_A);		
    		}
    		break;
    	default:
    		assert(0);
    		break;
	}
}

#if TVD_AUTO_DFT

/**
 * @brief Set TVD Default Mode
 * @param  bMode: TV mode enmu
 * @retval void
 */
static void _svDrvTvdSetDftMode(UINT8 bMode) reentrant
{
	switch(bMode)
	{
    	case SV_CS_PAL_N:
    		vRegWriteFldMulti(CDET_00,  P_Fld(0,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(0,INI_V525)|P_Fld(0,L525));
    		break;
    	case SV_CS_PAL:
    		vRegWriteFldMulti(CDET_00, P_Fld(1,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(0,INI_V525)|P_Fld(0,L525));
    		break;
    	case SV_CS_PAL_M:
    		vRegWriteFldMulti(CDET_00, P_Fld(0,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
    		break;	
    	case SV_CS_NTSC358:
    		vRegWriteFldMulti(CDET_00,  P_Fld(0,INI_IS443)|P_Fld(0,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
    		break;
    	case SV_CS_SECAM:
    		vRegWriteFldMulti(CDET_00,  P_Fld(1,INI_IS443)|P_Fld(0,INI_PHALT)|P_Fld(0,INI_V525)|P_Fld(0,L525));
    		break;
    	case SV_CS_PAL_60:
    		vRegWriteFldMulti(CDET_00,  P_Fld(1,INI_IS443)|P_Fld(1,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
    		break;
    	case SV_CS_NTSC443:
    		vRegWriteFldMulti(CDET_00,  P_Fld(1,INI_IS443)|P_Fld(0,INI_PHALT)|P_Fld(1,INI_V525)|P_Fld(1,L525));
    		break;	
    	default:
    		break;
	}

}
#endif

/**
 * @brief Set TVD CAGC UV peak value
 * @param  bMode : TVD color system
 * @retval void
 */
static void _svTvd3dSetCAgcUVPeak(UINT8 bMode)
{
	switch(bMode)
	{
    	case SV_CS_PAL_N:
    	case SV_CS_PAL:
    	case SV_CS_SECAM:
		vRegWriteFldAlign(CDET_09, CAGC_PEAK_U_PAL, CAGC_PEAK_U);
		vRegWriteFldAlign(CDET_09, CAGC_PEAK_V_PAL, CAGC_PEAK_V);
    		break;	
    	case SV_CS_PAL_M:
    	case SV_CS_PAL_60:
    	case SV_CS_NTSC358:
    	case SV_CS_NTSC443:
		if (_bAgcPedEn)
		{
			vRegWriteFldAlign(CDET_09, CAGC_PEAK_U_NTSC, CAGC_PEAK_U);
			vRegWriteFldAlign(CDET_09, CAGC_PEAK_V_NTSC, CAGC_PEAK_V);
		}
		else
		{
			vRegWriteFldAlign(CDET_09, CAGC_PEAK_U_NTSCJ, CAGC_PEAK_U);
			vRegWriteFldAlign(CDET_09, CAGC_PEAK_V_NTSCJ, CAGC_PEAK_V);
		}
    		break;	
    	default:
    		break;
	}
}

/**
 * @brief TVD Mode Change Done Handle Function
 *
 * When Mode Change Done, signal status must be properly set.
 * TDC, NR and other mode related setting must be set according
 * to 525/625 of input signal and then turn on.
 *
 * @bug
 */
static void _svDrvTvdModeChgDone(void)
{
	LOG(3,"[TVD_DBG_MSG] _svDrvTvdModeChgDone fgVPres=%d \n",_rTvd3dStatus.fgVPres);
	_sbTVDSerrCnt = TG_SERR_CNT; //Initialize serr _cnt
    #if TVD_FRS_FOR_NONSTDSIGNAL
	_wPreAvg_VLen = 0xff;
    #endif

	vDrvTvdRstDCLAMPCnt();
	vRegWriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW); 
	vRegWriteFldAlign(DFE_08, SV_OFF, DCLAMP_Y_EN); 
    #ifdef CC_MT8223
    vRegWriteFldAlign(DFE_0E, SV_OFF, DCLAMP_CHECK_LIM);
    #endif
    if (fgHwTvd525())
    {
	    vRegWriteFldAlign(CDET_00, 0, MODE000); //Setup Mode000 for NTSC
    }
    else
    {
        #ifdef FORCE_MODE000_NTSC
        vRegWriteFldAlign(CDET_00, 0, MODE000); //Setup Mode000 for PAL
        #else
        vRegWriteFldAlign(CDET_00, 1, MODE000); //Setup Mode000 for PAL
        #endif
    }
    
	//Restore 443 and PAL_UP PAL_DN gain. (Fast chennel is On in SetChNo() in srvext.c). Benson.06.02.27.
	if (_rTvd3dStatus.eSourceType==SV_ST_TV)
	{
		vTvd3dFastChannelChange(SV_OFF);
	}

	if (_rTvd3dStatus.fgVPres == 0)
	{
		_rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_NOSIGNAL;
        #if TVD_MAX_CLAMP_TABLE
		vRegWriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_OFF, CLAMP_TABLE_05);
        #endif
		//no snowscreen when ATV without signal
		if (_rTvd3dStatus.fgIs525 == 0)
		{
			vRegWriteFldAlign(TG_05, SV_ON, INV_FIELD);
		}
	}
	else
	{
		_rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_STABLE;

		// To avoid channel change AGC speed too slow, so set to defaule gain during mode change. Mark070711
		if(_sbIsTVDFixAgc != SV_ON)
		{
		    vTvdSetAGCMode(E_AUTO_GAIN);
		}

        #if TVD_MAX_CLAMP_TABLE
		vRegWriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
        #endif
		vRegWriteFldAlign(TG_05, SV_OFF, INV_FIELD);
		vRegWriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_NML, BLANK_BW);
		vRegWriteFldAlign(TG_17, TG_FRAME_STDFV_TH_STD, FRAME_STDFV_TH);
	}
	#if TVD_VCR_BV
	vDrvTvdRstVCRBVCnt();
	vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
	vRegWriteFldAlign(TG_04, SV_ON, LF_AUTO);
	#endif

    #if TVD_FRS_FOR_NONSTDSIGNAL
    vDrvTvdForceUpdNSTD(TRUE);
    #endif
    #if TVD_AUTO_DFT
        //if (!IS_AUTOSEARCH())    //MARKED BY xiaokuan
        {
            _svDrvTvdSetDftMode(((_rTvd3dStatus.fgIs525)?SV_CS_NTSC358:SV_CS_PAL));
        }
    #endif

    #if TVD_VCR_BV_STA
    vDrvTvdRstVCRBVStaCnt();
    #endif


#ifdef CC_TVE_POWERDOWN_HALF
	if(_rTvd3dStatus.fgIs525)
	{
		vRegWriteFldAlign(SECAM_07, 0x1B0, YUV_CATCH_VPT);
	}
	else
	{
		vRegWriteFldAlign(SECAM_07, 0x201, YUV_CATCH_VPT);
	}
	vRegWriteFldAlign(SECAM_07, 0x1a0, Fld(10, 10, AC_MSKW21));//one pixel
	vRegWriteFldAlign(SECAM_07, 6, YUV_CATCH_SEL);
#endif
	if(bTvdCtrl(TCTL_BP_MODECHG,TC_GETEN,0))
		return;

	if (fgIsMainTvd3d())
	{
		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
	}
	if (fgIsPipTvd3d())
	{
		vSetPipFlg(PIP_FLG_MODE_DET_DONE);
	}
}

/**
 * @brief Set TVD Old Burst frequency detection Gain
 * @param  bUpGain: Up Gain
 * @param  bDnGain: Dn Gain
 * @retval void
 */
static void _svDrvTvdSetDET443Gain(UINT8 bUpGain, UINT8 bDnGain)
{
    vRegWriteFldAlign(CDET_03, bUpGain, DET_443_GAIN_UP);// DET_443_GAIN_UP
    vRegWriteFldAlign(CDET_03, bDnGain, DET_443_GAIN_DN);// DET_443_GAIN_DN
}

/**
 * @brief TVD NA state change bottom half
 * @param  void
 * @retval void
 */
static void _svTvd3dBHNAStateChg(void)
{
	if(_na_state != _spre_na_state)
	{
		_svTvd3dSetAVDelay(_rTvd3dStatus.bTvdMode);
	}
	//Caculate the statistics only when vpres and vlock on.
	if ((_rTvd3dStatus.NAInfo.na_HysterCnt >_rTvd3dStatus.NAInfo.na_HysterCntThres)&&(_na_state != _spre_na_state))
    {
        _spre_na_state = _na_state;
		LOG(3,"[TVD_DBG_MSG] _svTvd3dBHNAStateChg _na_state=%d \n",_na_state);
        
        switch (_na_state)
        {
            case NA_LOWNOISE:
    			_svTvdNAReset();
    			vRegWriteFldAlign(VSRC_08, SV_ON, VPRES_COCH_EN);

                #if !TVD_NO_BYPASS_TRICK
    			_sbBypassTrick = 0x0;
                #endif

    			_svTvd3dSetAVDelay(_rTvd3dStatus.bTvdMode);
    			vRegWriteFldAlign(CDET_00, SV_ON, CTG_ADAP); //Low noise adap use high gain
                vRegWriteFldAlign(CTG_06, SV_OFF, BLOCK4DETSEL);//MT5387 new TH 
                vRegWriteFldAlign(TG_17, TG_FRAME_STDFV_TH_LHNA, FRAME_STDFV_TH); 
                if (IS_5363_NonECO() || IS_5387_NonECO())
                {
                    vRegWriteFldAlign(TG_08, SV_ON, VFIR_EN);
                }
                #if TVD_USE_NEW_DAGC
                    vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_AGAIN:E_AUTO_GAIN);
                    #if !TVD_DAGC_WA
                    if(_sbIsTVDFixAgc)
                        vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_LowNA, FIXAGC_DAGCSEL);
                    #endif
                #else
                    vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_GAIN:E_AUTO_GAIN);
                #endif
                break;


            case NA_HIGHNOISE:
                #if !TVD_NO_BYPASS_TRICK
				_sbBypassTrick = 0x0;
                #endif
                vRegWriteFldAlign(DFE_18, SV_OFF, VMAXMIN_EN);
    			vRegWriteFldAlign(CDET_00, SV_OFF, CTG_ADAP); //hi_nosise adap use high gain,hard to relock back
    			vRegWriteFldAlign(VSRC_08, SV_OFF, VPRES_COCH_EN);
                vRegWriteFldAlign(TG_17, TG_FRAME_STDFV_TH_LHNA, FRAME_STDFV_TH);
                vRegWriteFldAlign(CTG_06, SV_OFF, BLOCK4DETSEL);//MT5387 new TH 
                if (IS_5363_NonECO() || IS_5387_NonECO())
                {
                    vRegWriteFldAlign(TG_08, SV_ON, VFIR_EN);
                }
    			//Set burst unlock threshold under PAL.
    			switch (_rTvd3dStatus.bTvdMode)
    			{
        			case SV_CS_SECAM:
                		_svTvdSetCAGCBound4Data(SCM_HN_MID_B, SCM_HN_MID_A, SCM_HN_TGT_B, SCM_HN_TGT_A);					    
        				break;
        			case SV_CS_PAL_N:
        			case SV_CS_PAL:
        			case SV_CS_PAL_M:
        			case SV_CS_PAL_60:
                        _svTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_625, CTG_BSTLOCK_THR2_HN_625);    		
                		_svTvdSetCAGCBound4Data(PAL_HN_MID_B, PAL_HN_MID_A, PAL_HN_TGT_B, PAL_HN_TGT_A);					    
        				break;
        			case SV_CS_NTSC358:
        			case SV_CS_NTSC443:
                        _svTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_525, CTG_BSTLOCK_THR2_HN_525);    		
                		_svTvdSetCAGCBound4Data(NTSC_HN_MID_B, NTSC_HN_MID_A, NTSC_HN_TGT_B, NTSC_HN_TGT_A);					    
        				break;
        			default:
        				assert(0);
        				break;
    			}
                #if TVD_USE_NEW_DAGC
                    vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_AGAIN:E_AUTO_GAIN);
                    #if !TVD_DAGC_WA
                    if(_sbIsTVDFixAgc)
                        vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_HighNA, FIXAGC_DAGCSEL);
                    #endif
                #else
                    vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_GAIN:E_AUTO_GAIN);
                #endif
                break;



            case NA_SUPERHIGHNOISE:
                #if !TVD_NO_BYPASS_TRICK
				_sbBypassTrick = 0x01;
                #endif
                vRegWriteFldAlign(DFE_18, SV_OFF, VMAXMIN_EN);
    			vRegWriteFldAlign(CDET_00, SV_OFF, CTG_ADAP); //hi_nosise adap use high gain,hard to relock back
    			vRegWriteFldAlign(VSRC_08, SV_OFF, VPRES_COCH_EN);
                vRegWriteFldAlign(TG_17, TG_FRAME_STDFV_TH_STD, FRAME_STDFV_TH);                 
                vRegWriteFldAlign(CTG_06, SV_ON, BLOCK4DETSEL);//MT5387 new TH 
                if (IS_5363_NonECO() || IS_5387_NonECO())
                {
                    vRegWriteFldAlign(TG_08, SV_OFF, VFIR_EN);
                }
    			//Set burst unlock threshold under PAL.
    			switch (_rTvd3dStatus.bTvdMode)
    			{
        			case SV_CS_SECAM:
                		_svTvdSetCAGCBound4Data(SCM_HN_MID_B, SCM_HN_MID_A, SCM_HN_TGT_B, SCM_HN_TGT_A);					    
        				break;
        			case SV_CS_PAL_N:
        			case SV_CS_PAL:
        			case SV_CS_PAL_M:
        			case SV_CS_PAL_60:
                        _svTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_625, CTG_BSTLOCK_THR2_HN_625);    		
                		_svTvdSetCAGCBound4Data(PAL_HN_MID_B, PAL_HN_MID_A, PAL_HN_TGT_B, PAL_HN_TGT_A);					    
        				break;
        			case SV_CS_NTSC358:
        			case SV_CS_NTSC443:
                        _svTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_525, CTG_BSTLOCK_THR2_HN_525);    		
                		_svTvdSetCAGCBound4Data(NTSC_HN_MID_B, NTSC_HN_MID_A, NTSC_HN_TGT_B, NTSC_HN_TGT_A);					    
        				break;
        			default:
        				assert(0);
        				break;
    			}
                vTvdSetAGCMode(_sbIsTVDFixAgc?E_MENU_GAIN:E_AUTO_GAIN);
                #if TVD_USE_NEW_DAGC
                    #if !TVD_DAGC_WA
                    if(_sbIsTVDFixAgc)
                        vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_HighNA, FIXAGC_DAGCSEL);
                    #endif
                #endif
                break;
		}

	}


}

/**
 * @brief Set TVD 625/525 line system detection gain
 * @param  bV525Gain : Range 0~3
 * @param  bV625Gain : Range 0~3
 * @retval void
 */
static void _svTvd3dSetDetV525V625Gain(UINT8 bV525Gain, UINT8 bV625Gain) reentrant
{
	vRegWriteFldMulti(CDET_0D,
        			  P_Fld(bV625Gain,DET_V625_GAIN)|
        			  P_Fld(bV525Gain,DET_V525_GAIN));
}

/**
 * @brief Set TVD Phalt up/dn gain
 * @param  bUpGain : Range 0~7
 * @param  bDnGain : Range 0~7
 * @retval void
 */
static void _svTvd3dSetPALTGain(UINT8 bUpGain, UINT8 bDnGain)
{
	UINT8 bGain;

	bGain = ((bUpGain << 3) | bDnGain);
	vRegWriteFldAlign(CDET_03, bGain,DET_PHALT_GAIN);
}

/**
 * @brief  Start Noise State 
 * @param  void
 * @retval void
 */
static void _svTvdNAStart(void)
{
	_na_state = (UINT8)NA_LOWNOISE;
	_spre_na_state = (UINT8)NA_UNKNOWN;
	_rTvd3dStatus.NAInfo.na_HysterCnt=0;	
	_svTvdNAReset();
}

/**
 * @brief  Stop Noise State 
 * @param  void
 * @retval void
 */
static void _svTvdNAStop(void)
{
	_na_state = (UINT8)NA_STOP;
	_svTvdNAReset();
}


/**
 * @brief TVD Mode Change Handle Function
 *
 * When TVD mode change, we need to turn off NR, TDC and other HW that would write Dram.
 * Main/Pip MODE_CHG flags are issued accordingly.
 *
 * @bug
 *
 */
static void _svDrvTvdModeChg(void)
{
	LOG(3,"[TVD_DBG_MSG] _svDrvTvdModeChg\n");
        if (_rTvd3dStatus.fgVPres != 0)
        {
            vRegWriteFldAlign(TG_05, SV_OFF, INV_FIELD);
        }
	_svTvdNAStop();
#if 0//TVD_PHALT_WA
	if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
	{
		if(_sbPhalt_Vpres==0)
		{
			vRegWriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); 
		}
		else
		{
//		printf("_svDrvTvdModeChg\n");
//				printf("NPTV TVD BLOCK = %d ,TVD_MMODE=%d ,_sbTvdModeCnt=%d\n",fgHwTvdBLock(),fgHwTvdIsMMode(),_sbTvdModeCnt);
//				printf("mode status=%d ,NR_level=%d \n",(RegReadFldAlign(STA_CDET_00, Fld(3, 24, AC_MSKB3))),bHwTvdNRLevel());
	_svTvdSetBstLockTh(CTG_BSTLOCK_THR1_HN_625, CTG_BSTLOCK_THR2_HN_625);			
		}
	}
#endif
	if(bTvdCtrl(TCTL_BP_MODECHG,TC_GETEN,0))
		return;
	if (fgIsMainTvd3d())
	{
		vSetMainFlg(MAIN_FLG_MODE_CHG);
		vClrMainFlg(MAIN_FLG_MODE_DET_DONE);	// Clear this flag!
	}

	if (fgIsPipTvd3d())
	{
		vSetPipFlg(PIP_FLG_MODE_CHG);
		vClrPipFlg(PIP_FLG_MODE_DET_DONE);	// Clear this flag!
	}
    #if TVD_SERRTYPE_ONE
	_swCochIir_Avg=0;
    #endif

    #if SUPPORT_SCART
	vDrvTvdRstSCnt();
    #endif
	_rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_UNKNOWN;
	vTvd3dTrigModeDet(); // Reset Mode Change Counter

	bhwVCRSetting = 0x0; // Initialize it to Prefer nonVCR.
	vDrvTvdRstVCRCnt();
	_rTvd3dStatus.fgIsVCR = 0;
	vRegWriteFldAlign(DFE_02, 1, POS_DIFF_ONLY); // also check hi-low pattern
	vDrvTvdRstHLOCKCnt();
    #if TVD_OLD_VCR
	vDrvTvdRstOldVCRCnt();
    #endif
    #if TVD_SECAM_DET
	vDrvTvdRstBurstCnt();
    #endif
	// Turn off TDC
	vDrvTDCActive(SV_OFF);

	if(bTvdCtrl(TCTL_MDSTBPROT,TC_GETEN,0))
	{
		vRegWriteFldAlign(CDET_05, SV_ON, FORCE_UPD_MODE);
	}

	vRegWriteFldAlign(DFE_02, DFE_BLANK_LPF_BW_CCI, BLANK_BW);
}

/**
 * @brief TVD ISR SCART Setting
 * @param bUIScartMode IN: SCART mode 
 * @param PbSFisSV INOUT:pointer to SRisSV
 * @param PbTrigScartAuto INOUT:pointer to bTrigScartAuto
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
 #if SUPPORT_SCART  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
static void _svScartSet(UINT8 bUIScartMode,UINT8 *PbSFisSV,UINT8 *PbTrigScartAuto,RTvdStatus *pTvd3dStatus)
{
    if (_bMainICIn == P_FB0 || _bMainICIn == P_FB1
        #if SUPPORT_POP
    		|| _bSubICIn == P_FB0 || _bSubICIn == P_FB1
        #endif
       )
    {
    	if (bUIScartMode == SV_SCART_AUTO)
    	{
    		if (fgHwTvdSVFBstDet())
    		{
    			vDrvTvdIncSCnt();
    			if ((fgDrvTvdIsSCnt(TVD_S_CNT) && (*PbSFisSV == SV_FALSE))||(*PbTrigScartAuto==SV_TRUE))
    			{
    				*PbSFisSV = SV_TRUE;
    				*PbTrigScartAuto = SV_FALSE;
    				vApiVideoSetEvtFlg(VDO_FLG_SCART_SF_CHG);
				_svDrvTvdModeChg();
				LOG(6,"[TVD_DBG_MSG] _svScartSet fgHwTvdSVFBstDet True\n");
    			}
    		}
    		else
    		{
    			vDrvTvdDecSCnt();
    			if ((fgDrvTvdIsSCnt(TVD_S_CNT2) && (*PbSFisSV == SV_TRUE))||(*PbTrigScartAuto==SV_TRUE))
    			{
    				*PbSFisSV = SV_FALSE;
    				*PbTrigScartAuto = SV_FALSE;
    				vApiVideoSetEvtFlg(VDO_FLG_SCART_SF_CHG);
				_svDrvTvdModeChg();
				LOG(6,"[TVD_DBG_MSG] _svScartSet fgHwTvdSVFBstDet False\n");
    			}
    		}
    	
    	#if TVD_NEW_BST_DET_TH
    	vRegWriteFldAlign(SVF_03, 0x8+(pTvd3dStatus->bCAGC>>3), SVF_BSTDET_TH);
    	#endif
    	}
    	else
    	{
    		vDrvTvdRstSCnt();  //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
    	}
    }
   

}
  #endif


#if TVD_AUTO_CALIB
#ifdef CC_TVE_POWERDOWN_HALF
/**
 * @brief bDrvGetTVDADCOffset(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT16 manual offset code
 */
UINT16 _sbDrvGetTVDADCOffset(void)
{
	return (UINT16)RegReadFldAlign(DFE_0A, CLAMP_TARGET_BLANK_LV);
}

UINT8 _sbGetTvdCatch(UINT8 u1Stage, UINT32 u2X, UINT32 u2Y, UINT32 *pu2Y, UINT32 *pu2U, UINT32 *pu2V)
{
	vRegWriteFldAlign(SECAM_07, u1Stage, YUV_CATCH_SEL);
	//vRegWriteFldAlign(SECAM_07, u2X, YUV_CATCH_HPT);//four pixels
	vRegWriteFldAlign(SECAM_07, u2X, Fld(10, 10, AC_MSKW21));//one pixel

	vRegWriteFldAlign(SECAM_07, u2Y, YUV_CATCH_VPT);

	*pu2Y = RegReadFldAlign(STA_CTG_04, Fld(10, 0, AC_MSKW10));
	*pu2U = RegReadFldAlign(STA_CTG_04, Fld(10, 12, AC_MSKW21));
	*pu2V = RegReadFldAlign(STA_CTG_04, Fld(10, 22, AC_MSKW32));
	return SV_TRUE;
}
/**
 * _sbGetTvdCatchAverage()
 */
UINT8 _sbGetTvdCatchAverage(UINT8 u1Stage, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 *pu4Y, UINT32 *pu4U, UINT32 *pu4V)
{
	UINT32 u4TotalPixel;
	UINT32 u4XIndex, u4YIndex;
	UINT32 u4DataY, u4DataU, u4DataV;
	UINT32 u4TotalY=0, u4TotalU=0, u4TotalV=0;

	if((u4Width ==0) || (u4Height==0))
		return SV_FALSE;
	
	for(u4XIndex=0; u4XIndex<u4Width; u4XIndex++)
	{
		for(u4YIndex=0; u4YIndex<u4Height; u4YIndex++)
		{
			if(_sbGetTvdCatch(u1Stage, u4X+(u4XIndex*2), u4Y+(u4YIndex*2), &u4DataY, &u4DataU, &u4DataV)== SV_FALSE)
			{
				return SV_FALSE;
			}
			else
			{
				u4TotalY +=u4DataY;
				u4TotalU +=u4DataU;
				u4TotalV +=u4DataV;
			}
		}
	}

	u4TotalPixel= (UINT32)(u4Width * u4Height);
	*pu4Y = (UINT32)(u4TotalY/ u4TotalPixel);
	*pu4U = (UINT32)(u4TotalU/ u4TotalPixel);
	*pu4V = (UINT32)(u4TotalV/ u4TotalPixel);
    	return SV_TRUE;
}
#endif //end of CC_TVE_POWERDOWN_HALF

UINT16 wDrvGetTVDYLvl(void)
{
#ifdef CC_TVE_POWERDOWN_HALF
	UINT32 u4Y, u4U, u4V, u4Y_sum=0;
	UINT8 i;

		for (i = 0;i<32;i++)
		{
			if(fgHwTvd525())
			{
				_sbGetTvdCatchAverage(6, 0x1a0, 0x1B0, 4, 4, &u4Y, &u4U, &u4V); 
			}
			else
			{
				_sbGetTvdCatchAverage(6, 0x1a0, 0x201, 4, 4, &u4Y, &u4U, &u4V); 
			}
			u4Y_sum += u4Y;
		}
		return (UINT16)(u4Y_sum >> 5) - _sbDrvGetTVDADCOffset();
#else
	UINT16 YSum = 0;
	UINT8 i;
	for(i = 0; i<16; i++)
	{
		YSum += RegReadFldAlign(DFE_STA_03, PY_LV);
		vUtDelay2us(20);
	}
	return YSum>>4;
#endif
}
#endif //end of TVD_AUTO_CALIB


static BOOL _sfgDrvTvdCheckVLock( UINT8 bTryLoopCnt )
{
    UINT8 bLockedTime = 0;
    UINT16 wLockTarget = 0;
    UINT16 wMinDiff=0, wMinDiffAvg=0;
    UINT32 dwTunerNR = 0, dwTunerNRAvg = 0;
    
    if (bTryLoopCnt)
    {
        wLockTarget = bTryLoopCnt * TVDLOCK_RATIO / 100;
    }
    _swTVDNRLevel = 0;
    _swTVDNRLevel_AVG = 0xffff;

    do
    {
        if (TRUE == fgHwTvdVLock())
        {
            bLockedTime++;
            vRegWriteFldAlign(DFE_18, 5, VMAXMIN_STASEL);
            wMinDiff = (RegReadFldAlign(DFE_STA_09, VMAXMIN_STATUS));
            #ifdef USE_ATD_IF_DEMOD
            dwTunerNR = (ATD_GetNoiseLevel() >> 11) &0xFF;
            #else
            dwTunerNR = 0;
            #endif
            
//            LOG(3,"!!![DIFF_MIX,NR] = [%x][%x] \n", wMinDiff,dwTunerNR);
            wMinDiffAvg += wMinDiff;
            dwTunerNRAvg += dwTunerNR;
            _swTVDNRLevel += bHwTvdNRLevel();
            if(bLockedTime >= wLockTarget)
            {
                if (bLockedTime)
                {
                    _swTVDNRLevel_AVG = _swTVDNRLevel/bLockedTime;
                    wMinDiffAvg = wMinDiffAvg/bLockedTime;
                    dwTunerNRAvg = dwTunerNRAvg/bLockedTime;
//                    LOG(3,"!!![DIFF_MIX,AVGNR] = [%x][%x]\n", wMinDiffAvg,dwTunerNRAvg );
					LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock [DIFF_MIX,AVGNR] = [%x][%x]\n", wMinDiffAvg,dwTunerNRAvg );
                }
                if ((wMinDiffAvg > CSCAN_DIFF_MIN_TH) && (dwTunerNRAvg < CSCAN_NR_TH))
                    return FALSE;
                else
                    return (TRUE);
            }
        }
        vUtDelay1ms(15);
        bTryLoopCnt--;
    }
    while (bTryLoopCnt != 0);
//    LOG(3, "TVD VLock fail (%d/%d)\n", bLockedTime,wLockTarget);
	LOG(3,"[TVD_DBG_MSG] _sfgDrvTvdCheckVLock VLock fail (%d/%d)\n", bLockedTime,wLockTarget);
    return (FALSE);
}

/**
 * @brief TVD ISR Mode Change  detection 
 * @param fgvpres IN: vpres Status. 
 * @param fgIs525 IN: 525 Status. 
 * @param bTvdMode IN: bTvdMode
 * @param preTvd443nPHALT INOUT: preTvd443nPHALT Status
 * @param pTvd3dStatus INOUT:pointer to RTvdStatus status
 * @retval : NOON
 */
static void _svTvdModeChgDet(BOOL fgIs525,UINT8 bTvdMode,RTvdStatus *pTvd3dStatus)
{
    static UINT8 sbPreTvd443nPHALT = 0;

    if (bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0) == 0)
    {
    	if (pTvd3dStatus->fgVPres) //Issue Mode change only when VPres is on.
    	{
    		if (pTvd3dStatus->bTvdMode != bTvdMode)
    		{
			LOG(6,"[TVD_DBG_MSG] tvd mode change %d->%d\n",pTvd3dStatus->bTvdMode,bTvdMode);
    			pTvd3dStatus->bTvdMode = bTvdMode;
    			switch (bTvdMode)
    			{
    			case SV_CS_NTSC358:
    			case SV_CS_NTSC443:
    			case SV_CS_PAL_M:
    			case SV_CS_PAL_60:
    				pTvd3dStatus->fgIs525 = 1;
    				break;
    			default:
    				pTvd3dStatus->fgIs525 = 0;
    				break;
    			};
    			_svDrvTvdModeChg();

    		}
    		if((fgHwTvdIsMMode())&&(pTvd3dStatus->fgIs525 != fgIs525))
    		{
 			LOG(6,"[TVD_DBG_MSG] tvd 525 mode change %d->%d\n",pTvd3dStatus->fgIs525,fgIs525);
   			pTvd3dStatus->fgIs525 = fgIs525;
    			_svDrvTvdModeChg();
      		}
    	}
    }
    else
    {
    	if (pTvd3dStatus->fgVPres) //Issue Mode change only when VPres is on.
    	{
    		if (sbPreTvd443nPHALT != pTvd3dStatus->fgIs443nPHALT)
    		{
    			sbPreTvd443nPHALT = pTvd3dStatus->fgIs443nPHALT;
    			vTvdResetChromaLock();
    		}
    		if(pTvd3dStatus->fgIs525 != fgIs525)
    		{
  			LOG(6,"[TVD_DBG_MSG] tvd 525 mode change %d->%d\n",pTvd3dStatus->fgIs525,fgIs525);
   			pTvd3dStatus->fgIs525 = fgIs525;
    			_svDrvTvdModeChg();
      		}
    	}
    }



}
/*****************  ********************************************************
 * Global Functions
 *************************************************************************/
UINT16 wTvdCCI(void)
{
    return RegReadFldAlign(DFE_STA_08, COCH_IIR);
}
BOOL fgHwTvdVPres(void)
{
    if(IS_AUTOSEARCH())
    {
        return (RegReadFldAlign(STA_CDET_00, VPRES_TVD3D));
    }
    else
    {
#ifdef CC_MT5363_TVD 
        return ((RegReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))||(RegReadFldAlign(STA_CDET_00, VPRES_TVD3D)));
#else
        if (IS_5387_ECO())
            return ((RegReadFldAlign(DFE_STA_00, VPRES4_ON_FLAG))||(RegReadFldAlign(STA_CDET_00, VPRES_TVD3D)));
        else
            return (RegReadFldAlign(STA_CDET_00, VPRES_TVD3D));
#endif
    }
}


/**
 * @brief Enable/Disable CAGC adaptive control
 * @param bOnOff SV_ON: Enable / SV_OFF: Disable  CAGC ADAP
 * @return None
 */
void vTvd3dSetCAGCADAP(UINT8 bOnOff)
{
	LOG(6,"[TVD_DBG_MSG] vTvd3dSetCAGCADAP bOnOff=%d \n", bOnOff);
	if(_sbIsSetCAGCbyAP == SV_ON) return;
	vRegWriteFldAlign(CDET_00, bOnOff, CAGC_ADAP);
}

/**
 * @brief SVideo Init funciton
 * @param bOnOff SV_ON: Enable / SV_OFF: Disable  SV Init
 * @return None
 */
#if SUPPORT_SCART
void vTvd3dSVInit(UINT8 bSVOnOff)
{
	LOG(6,"[TVD_DBG_MSG] vTvd3dSVInit bSVOnOff=%d \n", bSVOnOff);
	if (bSVOnOff==SV_ON)
	{
		_sbScart_SV = 1;
		// Set Y/C Delay.
		vTvd3dSetYCDelay(0);
//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM
		vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM
	}
	else // SV Off
	{
		_sbScart_SV = 0;
		vTvd3dSetYCDelay(0);
		vDrvTDCSet();
		vDrvTDCActive(SV_ON);
	}
}
#endif


/**
 * @brief TVD Set VPres1 On/Off Gain and Threshold
 * TVD Set On/Off Gain and Threshold
 * @param bPath bPath indicates Main or PIP path currently TVD is connect to or disconnect from
 * @param bOnOff bOnOff indicates Connect or Disconnect
 * @return None
 */
void vDrvTvd3dVPresOnOff(UINT8 bGain, UINT8 bThrs) reentrant
{
    #if SUPPORT_DFE_VPRES_DET	//default
	// Set OnOff Gain to 1:1
	vRegWriteFldMulti(DFE_0F, P_Fld((bGain >> 4),VP1_ON_GAIN)|P_Fld((bGain & 0xF),VP1_OFF_GAIN));
	// 2005/07/26 Set Threshold of OnOff Gain
	vRegWriteFldAlign(DFE_0F, bThrs, VP1_THR);

    #endif
}


void vTvdHoldMode(UINT8 bHoldOnOff)
{
	_bTvdHoldMode = bHoldOnOff;
//	LOG(6,"[TVD_DBG_MSG] vTvdHoldMode bHoldOnOff=%d \n",bHoldOnOff);
}


/**
 * @brief The HTotal of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the HTotal of current input signal of TVDecoder.
 *
 * @param None
 * @return the HTotal of current video signal
 *
 */
UINT16 wTvdHTotal(void)
{
	if (_rTvd3dStatus.fgIs525)
	{
		return RegReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1716:858;
	}
	else
	{
		return RegReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1728:864;
	}
}

/**
 * @brief The VTotal of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the VTotal of current input signal of TVDecoder.
 *
 * @param None
 * @return the VTotal of current video signal
 *
 */
UINT16 wTvdVTotal(void)
{
	if (_rTvd3dStatus.fgIs525)
	{
		return 525;
	}
	else
	{
		return 625;
	}
}

/**
 * @brief measure if current input signal is Non-Standard Signal Status or not in V-sync ISR
 *
 * (Common Function of Video Decoders)
 * return none.
 *
 * @param VTotal and HTotal
 * @return none
 *
 */
#if TVD_FRS_FOR_NONSTDSIGNAL
void vTvd3dChkNSTD(UINT16 VTotal, UINT16 HTotal, RTvdNSTDStatus* pRTvdNSTDStatus)
{
	if (VTotal!=_wPreAvg_VLen)
	{
		_wPreAvg_VLen = VTotal;
		vDrvTvdRstNONSTDCnt();
	}

	if (TVDAbsDiff( HTotal, _wPreLCnt) > 3)	
	{
		_wPreLCnt = HTotal;
		vDrvTvdRstNONSTDCnt();
	}

	if ((((VTotal!=524)&&(VTotal!=624))|| 
		((TVDAbsDiff(HTotal, 1715) > 3) && (TVDAbsDiff(HTotal, 1727) > 3)))&& 
		(_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE))
	{
		vDrvTvdIncNONSTDCnt();
		if (fgDrvTvdIsNONSTDCnt(TVD_NONSTD_CNT) || vDrvTvdIsForceUpdNSTD())
		{
			pRTvdNSTDStatus->fgIsNSTD = 1;
			pRTvdNSTDStatus->wVTotal = VTotal+1;
			pRTvdNSTDStatus->wHTotal = HTotal+1;
			pRTvdNSTDStatus->bRefreshRate = (UINT8)((27000000 + (((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal))>>1))/((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal)));
			if(_rTvd3dStatus.fgIs525)
				pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 525;
			else
				pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 625;
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD Non STD Detected !\n");
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD fgIsNSTD=%x\n",pRTvdNSTDStatus->fgIsNSTD);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotal=%d\n",pRTvdNSTDStatus->wVTotal);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wHTotal=%d\n",pRTvdNSTDStatus->wHTotal);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD bRefreshRate=%d\n",pRTvdNSTDStatus->bRefreshRate);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotalDiff=%d\n",pRTvdNSTDStatus->wVTotalDiff);
		}
	}
	else
	{
		vDrvTvdDecNONSTDCnt();
		if (fgDrvTvdIsNONSTDCnt(TVD_NONSTD_CNT2) || vDrvTvdIsForceUpdNSTD())
		{
			pRTvdNSTDStatus->fgIsNSTD = 0;
			pRTvdNSTDStatus->wVTotal = VTotal+1;
			pRTvdNSTDStatus->wHTotal = HTotal+1;
			pRTvdNSTDStatus->bRefreshRate = (UINT8)((27000000 + (((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal))>>1))/((UINT32)(pRTvdNSTDStatus->wHTotal)/2 * (UINT32)(pRTvdNSTDStatus->wVTotal)));
			if(_rTvd3dStatus.fgIs525)
				pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 525;
			else
				pRTvdNSTDStatus->wVTotalDiff = pRTvdNSTDStatus->wVTotal + 0x800 - 625;
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD STD Detected !\n");
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD fgIsNSTD=%x\n",pRTvdNSTDStatus->fgIsNSTD);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotal=%d\n",pRTvdNSTDStatus->wVTotal);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wHTotal=%d\n",pRTvdNSTDStatus->wHTotal);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD bRefreshRate=%d\n",pRTvdNSTDStatus->bRefreshRate);
			LOG(6,"[TVD_DBG_MSG] vTvd3dChkNSTD wVTotalDiff=%d\n",pRTvdNSTDStatus->wVTotalDiff);
		}
	}
    vDrvTvdForceUpdNSTD(FALSE);

}
#endif 
/**
 * @brief The Non-Standard Signal Status of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the NSTD Signal Status of current input signal of TVDecoder.
 *
 * @param pTvdNSTDStatus  - A pointer to RTvdNSTDStatus to receive tvd status
 * @return the Signal Status of current video signal, return values include
 * fgIsNSTD, bRefreshRate, wVTotal and wVTotalDiff.
 *
 */
void vTvd3dGetNSTDStatus(RTvdNSTDStatus* pTvdNSTDStatus)
{
	assert(pTvdNSTDStatus != NULL);

	pTvdNSTDStatus->fgIsNSTD = _rTvd3dNSTDStatus.fgIsNSTD;
	pTvdNSTDStatus->bRefreshRate = _rTvd3dNSTDStatus.bRefreshRate;
	pTvdNSTDStatus->wVTotal = _rTvd3dNSTDStatus.wVTotal;
	pTvdNSTDStatus->wHTotal = _rTvd3dNSTDStatus.wHTotal;
	pTvdNSTDStatus->wVTotalDiff = _rTvd3dNSTDStatus.wVTotalDiff;
}


/**
 * @brief enable/disable Force TVD Manual VPRES
 * @param bMVPresOnOff: SV_ON / SV_OFF
 * @note: No used currently
 */
void vTvdForceTvdManualVPres(UINT8 bMVPresOnOff)
{
	if (bMVPresOnOff==SV_ON)
	{
		vRegWriteFldAlign(DFE_0E, SV_ON, MVPRES_TVD_EN);
		vRegWriteFldAlign(DFE_0E, SV_TRUE, MVPRES_TVD);
	}
	else if (bMVPresOnOff==SV_OFF)
	{
		vRegWriteFldAlign(DFE_0E, SV_OFF, MVPRES_TVD_EN);
		vRegWriteFldAlign(DFE_0E, SV_FALSE, MVPRES_TVD);
	}
}


/**
 * @brief enable/disable white peak limitator
 * @param eAgcType: reference to TVD_AGC_T Type
 */
void vTvdSetAGCMode(TVD_AGC_T eAgcType) reentrant
{
	if (RegReadFldAlign(VSRC_07,ATD_ENABLE) && (eAgcType == E_AUTO_GAIN))
		eAgcType = E_MENU_AGAIN;

	vRegWriteFldAlign(DFE_03, eAgcType, AGC2_MODE);
}
/**
 * @brief enable/disable white peak limitator
 * @param bOnOff: SV_ON/SV_OFF
 */
void vDrvWhitePeakLimit(UINT8 bOnOff)
{
	if(!_sbIsTVDFixAgc)
	vRegWriteFldAlign(DFE_03, ((bOnOff == SV_ON) ? 0xf : 0x7), AGC2_PK_MODE);// AGC_LIMIT_EN
	else
		vRegWriteFldAlign(DFE_03, 0x7, AGC2_PK_MODE);// AGC_LIMIT_EN
}
/**
 * @brief bDrvGetTVDADCGain(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT8 manual analog gain code
 */
UINT8 bDrvGetTVDADCGain(void)
{
	return (UINT8)RegReadFldAlign(DFE_07, AGC2_MANUAL_ACODE);
}
/**
 * @brief bDrvSetTVDADCAGAIN(UINT8 value)
 * call by d_custom_vid_set
 * @param  value manual analog gain code
 * @retval void
 */
void vDrvSetTVDADCGain(UINT8 value)
{
    _sbTvdMenuAcode = value;
	vRegWriteFldAlign(DFE_07, value, AGC2_MANUAL_ACODE);
}
/**
 * @brief bDrvGetTVDADCDGain(void)
 * call by 
 * @param  void
 * @retval UINT8 manual analog gain code
 */
UINT16 bDrvGetTVDADCDGain(void)
{
	return (UINT16)RegReadFldAlign(DFE_07, AGC2_MANUAL_DCODE);
}
/**
 * @brief bDrvSetTVDADCDGAIN(UINT8 value)
 * call by 
 * @param  value manual analog gain code
 * @retval void
 */
void vDrvSetTVDADCDGain(UINT16 value)
{
    _swTvdMenuDcode = value;
	vRegWriteFldAlign(DFE_07, value, AGC2_MANUAL_DCODE);
}


/**
 * @brief Set TVD Anti-Aliasing Filter
 * call by 
 * @param  bOnOff: SV_ON: Enable AAF / SV_OFF: Bypass AAF
 * @param  bValue 0:7.5HZ, 1:4.2MHZ, 2:5.0MHZ, 3:5.7MHZ, 4:6.0MHZ, 5:6.5MHZ
 * @retval void
 * @Note: If call this function, TVD internal driver won't set AAF again. 
 */
void vTvd3dSetAAFbyAP(UINT8 bOnOff, UINT8 bValue)
{
	_sbIsSetAAFbyAP = SV_ON;
	if(bOnOff)
	{
		vRegWriteFldAlign(VSRC_07, SV_OFF, BYPASS_AAF);
	}
	else
	{
		vRegWriteFldAlign(VSRC_07, SV_ON, BYPASS_AAF);
	}
	vRegWriteFldAlign(VSRC_07, bValue, AAF_SEL);
}


/**
 * @brief Get UI Delay
 * @param  void
 * @retval void
 */
void vTvd3dGetUIDelay(void)
{
	UINT16 wTVD_AVDelay;

	wTVD_AVDelay = _swTvdGetDftMenuAvDelay(_rTvd3dStatus.bTvdMode);
	_su1UIDelay = _swTvdGetMenuAvDelay(_rTvd3dStatus.bTvdMode)- wTVD_AVDelay;

}



/**
 * @brief TVD Mode Change Done bottom half
 *
 * @bug
 */

void vTvd3dBHModeDone(void)
{
	/* call by vVdoMainState(), vVdoPipState() */
	
	if (_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
	{
		_svTvdNAStart();

		if(bTvdCtrl(TCTL_SERRTYPE,TC_GETEN,0))
		{
			vRegWriteFldAlign(TG_07,bTvdCtrl(TCTL_SERRTYPE,TC_GETVAL,0), SERR_TYPE);
		}
		else
		{
            #if TVD_SERRTYPE_ONE
			if (((_rTvd3dStatus.bTvdMode == SV_CS_PAL_M)||(_sbAvgCOCH_detect))&&(!IS_AUTOSEARCH()))
			{
				vRegWriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
			}
			else
			{
				vRegWriteFldAlign(TG_07, SV_ON, SERR_TYPE);
			}
            #else
			vRegWriteFldAlign(TG_07, SV_OFF, SERR_TYPE);
            #endif
		}
		vRegWriteFldAlign(TG_06, 0x1, FDLY_SEL);//for VBI (1 line delay)
		vRegWriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);

		switch (_rTvd3dStatus.bTvdMode)
		{
    		case SV_CS_PAL_N:
    		case SV_CS_PAL:
    		case SV_CS_PAL_M:
    		case SV_CS_PAL_60:
                     _svTvd3dSetAAF(0x04);
    			vDrvNRSetChromaLineAvgMode((fgIsMainTvd3d()?SV_VP_MAIN:SV_VP_PIP), NR_CHROMA_LINE_AVERAGE_MODE_121);
    			//vRegWriteFldAlign(CDET_06, CDET_KBURST_PAL, KBURST);
    			vRegWriteFldAlign(CDET_06, CDRT_KHYST_625, KHYST);
    			vRegWriteFldAlign(CDET_01, CDET_NA_TH_HIGH_PAL, TH_HIGH);
        			if (RegReadFldAlign(VSRC_07,ATD_ENABLE))
				    vRegWriteFldAlign(CDET_01, CDET_NA_TH_SUPER_PAL_ATD, TH_SUPER);
				else
				    vRegWriteFldAlign(CDET_01, CDET_NA_TH_SUPER_PAL, TH_SUPER);
    			_svTvd3dSetPALTGain(5, 3);
    			vRegWriteFldAlign(CDET_06, SV_OFF, SCF_GAIN_SEL);//MT5387 YJ
    			break;
    		case SV_CS_SECAM:
                     _svTvd3dSetAAF(0x00);
    			vDrvNRSetChromaLineAvgMode((fgIsMainTvd3d()?SV_VP_MAIN:SV_VP_PIP), NR_CHROMA_LINE_AVERAGE_MODE_121);
    			//vRegWriteFldAlign(CDET_06, CDET_KBURST_SECAM, KBURST);
    			vRegWriteFldAlign(CDET_06, CDRT_KHYST_625, KHYST);
    			vRegWriteFldAlign(CDET_01, CDET_NA_TH_HIGH_SECAM, TH_HIGH);
    			vRegWriteFldAlign(CDET_01, CDET_NA_TH_SUPER_SECAM, TH_SUPER);
    			_svTvd3dSetPALTGain(3, 3);
    			vRegWriteFldAlign(CDET_06, SV_ON, SCF_GAIN_SEL);//MT5387 YJ
    			break;
    		default://NTSC358, NTSC443
                     _svTvd3dSetAAF(0x03);
    			vDrvNRSetChromaLineAvgMode((fgIsMainTvd3d()?SV_VP_MAIN:SV_VP_PIP), NR_CHROMA_LINE_AVERAGE_MODE_OFF);
    			//vRegWriteFldAlign(CDET_06, CDET_KBURST_NTSC, KBURST);
    			vRegWriteFldAlign(CDET_06, CDRT_KHYST_525, KHYST);
    			vRegWriteFldAlign(CDET_01, CDET_NA_TH_HIGH_NTSC, TH_HIGH);
    			vRegWriteFldAlign(CDET_01, CDET_NA_TH_SUPER_NTSC, TH_SUPER);
    			vRegWriteFldAlign(CDET_06, SV_OFF, SCF_GAIN_SEL);//MT5387 YJ
    			break;
		}
        
		//Setting High/Super High NA TH for Non Std FH
		if (IS_FH_NON_STD())
		{
			vRegWriteFldAlign(CDET_01, CDET_NA_TH_HIGH_NSTD_FH, TH_HIGH);//for NTSC V Line fix FH -1500Hz H-unlock
			vRegWriteFldAlign(CDET_01, CDET_NA_TH_SUPER_NSTD_FH, TH_SUPER);
		}

		//AAF Setting.
            if (_rTvd3dStatus.eSourceType!=SV_ST_TV) // max AAF bandwidth for better video quality		
            {			
                //vRegWriteFldAlign(VSRC_07, 0, AAF_SEL);			
                _svTvd3dSetAAF(0x00);		
            }        

		//Only use IF compensation under NTSC TV.
		if ((_rTvd3dStatus.eSourceType==SV_ST_TV)&&(_rTvd3dStatus.bTvdMode==SV_CS_NTSC358))
		{
			vRegWriteFldAlign(CTG_05, CTG_NTSC_IFCOMP_TV, IFCOMP_SEL);
		}
		else
		{
			vRegWriteFldAlign(CTG_05, CTG_NTSC_IFCOMP_DFT, IFCOMP_SEL);
		}

		if(_sbIsTVDOffsetbyAP == SV_ON)//for TVD offset
		{
			_wTargetBlk = DFE_BLANK_TARGET;
		}

		//------Set Blank Level & Related PeakY Limit
        _svDrvSetTVDBlk(_rTvd3dStatus.bTvdMode, _wTargetBlk);
		_swAGCTargetSyncHeight = _swDrvGetTVDAGCTarget();
		vDrvTDCSet();
		// Turn on TDC
		if ((_rTvd3dStatus.eSourceType != SV_ST_SV)
        #if SUPPORT_SCART
			&&(_sbScart_SV!=1)
        #endif
		   )
		{
			// Before Turn on TDC, Set the parameter of TDC
			vDrvTDCActive(SV_ON);
		}

		if (_rTvd3dStatus.fgIs525)
		{
			// Set DEF VSYNC MASK to avoid BAD VBI affecting Peak white limit.
			vRegWriteFldMulti(TG_01, P_Fld(SV_ON,FRONT_VMASK_SEL)|
                P_Fld(SV_ON,FRONT_VMASK_NEW)|
                P_Fld(TG_FRONT_VMASK_START,FRONT_VMASK_START)|
                P_Fld(TG_FRONT_VMASK_STOP_525,FRONT_VMASK_END));
			//vRegWrite1BMsk(TG_01_0, 0xc2, 0xf7);//	FRONT_VMASK_SEL|FRONT_VMASK_NEW|FRONT_VMASK_START|FRONT_VMASK_END
			vRegWriteFldAlign(TG_02, TG_MASK_STOP_525, MASK_STOP);
                #ifdef CC_MT8223
                vRegWriteFldAlign(TG_16, ATD_VMASK_START_NTSC, ATD_VMASK_START);
                //vRegWriteFldAlign(PQCRC_01, ATD_VMASK_START_NTSC, ATD_VMASK_START_I2C);
                #endif
		}
		else
		{
			// Set DEF VSYNC MASK to avoid BAD VBI affecting Peak white limit.
			vRegWriteFldMulti(TG_01, P_Fld(SV_ON,FRONT_VMASK_SEL)|
                P_Fld(SV_ON,FRONT_VMASK_NEW)|
                P_Fld(TG_FRONT_VMASK_START,FRONT_VMASK_START)|
                P_Fld(TG_FRONT_VMASK_STOP_625,FRONT_VMASK_END));
			//vRegWrite1BMsk(TG_01_0, 0xc0, 0xf7);// FRONT_VMASK_SEL|FRONT_VMASK_NEW|FRONT_VMASK_START|FRONT_VMASK_END
			vRegWriteFldAlign(TG_02, TG_MASK_STOP_625, MASK_STOP);//for PAL macrovision 
                #ifdef CC_MT8223        
                vRegWriteFldAlign(TG_16, ATD_VMASK_START_PAL, ATD_VMASK_START);
                //vRegWriteFldAlign(PQCRC_01, ATD_VMASK_START_PAL, ATD_VMASK_START_I2C);
                #endif
		}
	}
}

/**
* @brief TVD Mode Change bottom half
*
* @bug
*/

void vTvd3dBHModeChg(void)
{
	// Wrtie AV_Delay at once when TVDMode changes. Benson.060213
	_svTvd3dSetAVDelay(_rTvd3dStatus.bTvdMode);
	_svTvd3dSetCAgcUVPeak(_rTvd3dStatus.bTvdMode);
#if TVD_DAGC_WA
    if(_sbIsTVDFixAgc)
    {
        vDrvTvdRstDCODEStaCnt();
        vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_LowNA, FIXAGC_DAGCSEL);
    }
#endif	
}


/**
* @brief TVD MainLoop routine for reduce ISR usage
*  in 539x improve TVD ISR max exec time from 57us to 34 us
* @bug
*/
void vTvd3dMainLoop(void)
{
	UINT8 fgIsMV;
	if (_rTvd3dStat.bIsMain||_rTvd3dStat.bIsPip)
	{
        _svTvdQtestStaUpdate();
		
        #if SUPPORT_VBI
            #if TVD_SERRTYPE_ONE		
                VBI_SetAVFieldType(_rTvd3dStatus.fgIs525, 
                                    !(RegReadFldAlign(TG_07, SERR_TYPE)), //Serrtype 0: Field Inversed.
                                    _rTvd3dStatus.bTvdMode);
            #endif		
        #endif

		if (IS_AUTOSEARCH())
		{
			if(DRVCUST_OptGet(eTVDPalNtscMixAutosearch))
			{
				if(_sbPreAutoSearch != _fgAutoSearch)
				{
					if(!_rTvd3dStatus.fgVPres)
					{
//						LOG(3,"auto search mode change\n");
						LOG(3,"[TVD_DBG_MSG] vTvd3dMainLoop auto search mode change\n");
						_svDrvTvdModeChg();
					}
					_sbPreAutoSearch = _fgAutoSearch ;
				}
				if(_rTvd3dStatus.bColSys != SV_CS_AUTO)
				{
					_svDrvTvdSetDftMode(((_rTvd3dStatus.fgIs525)?SV_CS_NTSC358:SV_CS_PAL));
				}
				else
				{
					_svDrvTvdSetDftMode(SV_CS_NTSC358);
				}
				if(!_rTvd3dStatus.fgVPres)
				{
					_rTvd3dStatus.fgIs525 = 0x01;
					_rTvd3dStatus.bTvdMode = 0xff;
				}
			}
			vTvd3dFastChannelChange(SV_ON);			
			_svTvdNAStart();
		}
		else 
		{
			if(DRVCUST_OptGet(eTVDPalNtscMixAutosearch))
			{
				_sbPreAutoSearch = 0;
			}
			if (_rTvd3dStatus.bSigStatus != (UINT8)SV_VDO_NOSIGNAL)
			{
				_svTvd3dBHNAStateChg();
			}
		}
		
		if (_rTvd3dStatus.bSigStatus == (UINT8)SV_VDO_STABLE)
		{
			fgIsMV = fgHwTvdIsPSync();//fgHwTvdIsMacroVision
			if(_rTvd3dStatus.fgIsMV != fgIsMV)
			{
				vApiNotifyMVChg(_rTvd3dStat.bIsMain ? SV_VP_MAIN : SV_VP_PIP, fgIsMV);
				_rTvd3dStatus.fgIsMV = fgIsMV;
			}
		}
	}

}


/**
 * @brief TVD Interrupt Service Routine
 *
 * handles VDO On/Off, Mode Change, MacroVision On/Off and VSync Interrupt
 *
 * @param wStatus wStatus contains Interrupt Status Information
 * @return None
 * @warning Don't place too many code in ISR function to avoid blocking the execution of main flow
 *
 * @pre TVD is connected, and some interrupt happened
 * @post Video interrupt is properly handled
 */
void vTvd3dVSyncISR(void)
{
	BOOL fgVPres, fgIs525, fgIsVCR, fgTrick;	
	UINT8 bTvdMode;
//********************************** VSync Interrupt***************************************
	if (!((_rTvd3dStat.bIsMain && fgIsVdoIntMainDet()) || (_rTvd3dStat.bIsPip && fgIsVdoIntPipDet())))
		return;
	if(bTvdCtrl(TCTL_BP_ISR,TC_GETEN,0))
		return;
	_bTvdISRCnt++;

    #if TVD_VCR_BV_STA
    _sbTvd3dSetVCRBVSta(fgHwTvdVCRBV(), &_fgVCRBV);
    #endif

	//-------------------------TVD Vpres State-------------------------------------------//
    #if TVD_VPRES_STATE
	fgPreVPres_0 = _sbTvdVpresStateMachine(&_sbVpresState);
    #else
    fgPreVPres_0 = fgHwTvdVPres();
    #endif

	fgVPres=fgPreVPres_0;
	bTvdMode = bHwTvdMode();
	fgIs525 = fgHwTvd525();

    #if TVD_MODE_Deglitch
    _sbTvdModeDeglish(&fgVPres , &bTvdMode, &fgIs525); 
    #else
	if(!fgHwTvdFHNeg())
	{
		fgIs525 = fgHwTvd525();
	}
	else
	{
		switch(bTvdMode)
		{
			case SV_CS_NTSC358:
			case SV_CS_NTSC443:
			case SV_CS_PAL_M:
			case SV_CS_PAL_60: 
				fgIs525 = 1;
				break;
			default:
				fgIs525 = 0;
				break;
		};
	}
    #endif 
    
	_rTvd3dStatus.fgIsBurstLock = fgHwTvdBLock();
	_rTvd3dStatus.fgIsCkill = fgHwTvdCKill();
	_rTvd3dStatus.bCAGC = bHwTvdCAGC();
	fgTrick = fgHwTvdTrick() && fgVPres && (_na_state != (UINT8)NA_SUPERHIGHNOISE);
	fgIsVCR = bTvdCtrl(TCTL_VCR,TC_GETEN,0)?bTvdCtrl(TCTL_VCR,TC_GETVAL,0):(fgHwTvdHeadSwitch()||fgTrick); //Use head switch status to detect VCR. Benson.06.01.09
	_rTvd3dStatus.fgIsPSync = fgHwTvdIsPSync();
	_rTvd3dStatus.fgIsCoChannel =  bTvdCtrl(TCTL_COCH,TC_GETEN,0)?bTvdCtrl(TCTL_COCH,TC_GETVAL,0):fgHwTvdCoChannel()&&(_na_state != (UINT8)NA_SUPERHIGHNOISE);
	_rTvd3dStatus.fgIs443nPHALT = (fgHwTvd443()<<1)|fgHwTvdPHALT();
    #if TVD_FRS_FOR_NONSTDSIGNAL
	_rTvd3dStatus.LineInfo.wAvg_VLen = wHwTvdAvgVlen();
    #endif
    #if CAGC_PEAK_CNR_ADJ
    _rTvd3dStatus.fgCNRLow = fgHwTvdCNRLow();
    #endif

	//big UP/DN_RATIO for small Againcode
	_rTvd3dStatus.bAgainCode = RegReadFldAlign(DFE_STA_04, AGAIN_CODE);
    #if TVD_LLOCK_STATUS
    _svTvd3dChkLineStatus();
    #endif

    #if TVD_NR_AVG
	vTvdMeasureNR(SV_ON);
    #endif
//-------------------------over charoma adjust  when nocolor->color-----start------------------------//
#if   TVD_AntiTrs
	  _svTvdAntiTrs(_rTvd3dStatus.bCAGC,&_sfgCagc,&_sbCAGC1)  ; //Adjust CAGC loop gain to shorten the transient time
	  #endif
	 
/*	 if((bCAGC1>bCAGC+15)&&(fgcagc==0))	 	
	 {
	    fgcagc=1; 
	_bTvdISRCnt=0;
	 vRegWriteFldAlign(CDET_0B, 0x05, LARGE_LOOP_GAIN);
        printf("\n bcagc1=%d,bcagc=%d \n",bCAGC1,bCAGC)	; 		
         
	 }
	
	
	
	if((_bTvdISRCnt>=0x20)&&(fgcagc==1))
	      {
	         
	         fgcagc=0;
	        _bTvdISRCnt=0;	       
		   printf("\n the changed bcagc:bCAGC=%d \n",bHwTvdCAGC());
		  vRegWriteFldAlign(CDET_0B, 0x02,  LARGE_LOOP_GAIN);	
	       }  
	  bCAGC1=bCAGC;  */
      //-------------------------over charoma adjust when nocolor->color----end-------------------------//    

	
	

//-------------------------SFirst detect counter ----------------------------//
#if SUPPORT_S_DETECT
     vSVPresDetCount();
#endif
//-------------------------Adj 2d/3d line avg threshold ----------------------------//
     _svAdjLineThr();	

	_na_state =bTvdCtrl(TCTL_NOISELVL,TC_GETEN,0)?bTvdCtrl(TCTL_NOISELVL,TC_GETVAL,0):bHwTvdNAState();

//-------------------Check Vpress Changed status----------------------
	if (_rTvd3dStatus.fgVPres != fgVPres)
	{
 		LOG(6,"[TVD_DBG_MSG] tvd vpres mode change %d->%d\n",_rTvd3dStatus.fgVPres,fgVPres);
		_rTvd3dStatus.fgVPres = fgVPres;

#if 0//TVD_PHALT_WA 
		if((_rTvd3dStatus.bColSys == SV_CS_AUTO)&&(_rTvd3dStatus.fgVPres))
		{
			_sbPhalt_Vpres=1;
//			printf("NPTV TVD vpres _sbPhalt_Vpres=1\n");
		}
		else
		{
			_sbPhalt_Vpres=0;
//			printf("NPTV TVD vpres _sbPhalt_Vpres=0\n");
		}
#endif
		_svDrvTvdModeChg();

		// mtk70117 marked begin for secam detection in super high noise.
		/*if (_rTvd3dStatus.fgVPres)
		{
			vRegWriteFldMulti(CDET_03,\
					  P_Fld(CDET_SCF_NR_UP_VPON, SCF_NR_UP)|\
					  P_Fld(CDET_SCF_NR_DN_VPON, SCF_NR_DN));
            #if !TVD_VPRES_IMPROVE
			vRegWriteFldAlign(DFE_11, DFE_VPRES2_H_OFF_GAIN, VP2_H_OFF_OFFSET);
            #endif
		}
		else
		{
			vRegWriteFldMulti(CDET_03,\
					  P_Fld(CDET_SCF_NR_UP_VPOFF, SCF_NR_UP)|\
					  P_Fld(CDET_SCF_NR_DN_VPOFF, SCF_NR_DN));
            #if !TVD_VPRES_IMPROVE
			vRegWriteFldAlign(DFE_11, DFE_VPRES2_H_OFF_GAIN_IN_VOFF, VP2_H_OFF_OFFSET);	// To add hsytersis of VPRES.
            #endif
		}*/ // mtk70117 marked end for secam detection in super high noise
	}

#if 0//TVD_PHALT_WA 
	if((fgIs525 == 0)&&(fgHwTvd443() == 0)&&(fgHwTvdPHOLD() == 0)
		&&(_sbPhalt_Vpres==1)&&(bHwTvdNRLevel()>0x1E)&&(!fgHwTvdBLock()))
	{
		vRegWriteFldAlign(CDET_00, SV_ON, TVD_MMODE); 
		vRegWriteFldAlign(CDET_00, SV_CS_PAL_N, TVD_MODE); 
		_sbPhalt_Vpres=2;
//		printf("NPTV TVD vpres mode000\n");
//				printf("NPTV TVD BLOCK = %d ,TVD_MMODE=%d ,_sbTvdModeCnt=%d\n",fgHwTvdBLock(),fgHwTvdIsMMode(),_sbTvdModeCnt);
//				printf("mode status=%d ,NR_level=%d \n",(RegReadFldAlign(STA_CDET_00, Fld(3, 24, AC_MSKB3))),bHwTvdNRLevel());
	}
#endif
//-------------------Adjust Again Up/Dn Th------------------------
    _svAgainUpDnThr(_rTvd3dStatus.bAgainCode);	
//-------------------Adjust Non Std FH ------------------------
    #if TVD_8222_NONSTANDARD
	if (IS_FH_NON_STD())
    {
	    vRegWriteFldAlign(DFE_0F, SV_ON, VP2_IGN_HSYNC_P);//ignore VP2 hsync period check 
    }
	else
    {
	    vRegWriteFldAlign(DFE_0F, SV_OFF, VP2_IGN_HSYNC_P);
    }
    #endif
//------------------- Macro Vision --------------------------------
	vHwTvdSetMAV(_rTvd3dStatus.fgIsPSync);

//------------------- SCart----------------------------------------
    #if SUPPORT_SCART 
    _svScartSet(_bUIScartMode,&_bSFisSV,&_bTrigScartAuto,&_rTvd3dStatus);
     #endif   

//----Setting VCR Mode when VPres On-----
    _svTvdVcrSet(fgIsVCR, &bhwVCRSetting, &_rTvd3dStatus);

/*************MT8202B Change Channel************/
    _svChanChg(&_rTvd3dStatus,_sbIsTVDFixAgc );	

/*******************************Noise Analasis*****************************/

	// Measure noise only when vlock and vpres is on continuously after _sna_HysterCnt Vsync.
    _rTvd3dStatus.NAInfo.na_HysterCnt = (_rTvd3dStatus.NAInfo.na_HysterCnt>_rTvd3dStatus.NAInfo.na_HysterCntThres)?_rTvd3dStatus.NAInfo.na_HysterCnt : (_rTvd3dStatus.NAInfo.na_HysterCnt+1) ;

//-------------------Check Auto Search state---------------------------//
    _svGetNRAvg(&wTVDNRavg,&_swTVDNRavg1,&_rTvd3dStatus);	


//-----------------Trick Mode Detection----------------------// 	
    _svTrickModeDET(fgTrick, bhwVCRSetting, _na_state, &_rTvd3dStatus);			
				
//-----------------Secam Detect-------------------------/
    _svSecamDET(&_rTvd3dStatus, _sbTvd_McDone_cnt);

//-----------------Co Channel Adjustment-------------------/
    _svCochAdj(_rTvd3dStatus.fgIsCoChannel,_swAGCTargetSyncHeight);
	
//-----------------STD/NONSTD Line Related Seeting-----------/
    _svStdNStdLineSet(_na_state,&_rTvd3dStatus);
	//Check Line Count 
	
//-----------------Non STD FH related Setting--------------/
     _svNonStdFhSet();
	
//-----------------HLOCK Detection-----------------/
    _svHLockDET(&_fgHLOCK );	
	
//-----------------Old VCR Detection*(check STA_COMB_0C)---------/
    #if TVD_OLD_VCR
	if ((fgHwTvdVCR()) &&
		(_rTvd3dStatus.bTvdMode <= SV_CS_NTSC358) &&
		(fgHwTvdNProg()))
	{
		vDrvTvdIncOldVCRCnt();
		if (fgDrvTvdIsOldVCRCnt(TVD_OldVCR_CNT-1))
		{
			_sfgOldVCR = 1;
		}
	}
	else
	{
		vDrvTvdDecOldVCRCnt();
		if (fgDrvTvdIsOldVCRCnt(TVD_OldVCR_CNT2+1))
		{
			_sfgOldVCR = 0;
		}
	}
    #endif
//-----------------Check STD/NONSTD VLEN-----------------/
#if TVD_FRS_FOR_NONSTDSIGNAL
	if((_rTvd3dStatus.LineInfo.wAvg_VLen != 0) && (_rTvd3dStatus.LineInfo.wLinecnt !=0) &&(_rTvd3dStatus.bSigStatus == SV_VDO_STABLE))
	{
		vTvd3dChkNSTD(_rTvd3dStatus.LineInfo.wAvg_VLen, _rTvd3dStatus.LineInfo.wLinecnt, &_rTvd3dNSTDStatus);
	}
#endif
//-----------------DClamp on/off condition check-----------------/
    _svDclampCHK(bhwVCRSetting,&_rTvd3dNSTDStatus,&_rTvd3dStatus);	

//-----------------Check Blank variation large-----------------/
#if TVD_COCH_FLICKER
	bIsBlankVarLarge = ((fgHwTvdBlkLargeVar()&&_rTvd3dStatus.fgIsCoChannel)?SV_TRUE:SV_FALSE); //?? no used curretly
#endif

//-----------------Force Fast VALIGN-----------------/
    #if TVD_FORCE_FAST_VALIGN
    _svFastValign(); 
    #endif
//-----------------CO IIR Check-----------------/
    _svCOIIRCHK(&_rTvd3dStatus, _sbAvgCOCH_detect);	
   
//-----------------NA State Check----------------------/
    _svNAStateCHK(_na_state,&_rTvd3dStatus);
	
//-----------------Serr Cnt Check----------------------/
	if (fgHwTvdFieldDamp()) //Signal Is no good and field detect is toggling
	{
		_sbTVDSerrCnt = (_sbTVDSerrCnt == 0x6B) ? TG_SERR_CNT: _sbTVDSerrCnt + 1;
		vRegWriteFldAlign(TG_06, _sbTVDSerrCnt , SERR_CNT);
	}
//-------------------VCR_BV setting------------------------------------------------------
    #if TVD_VCR_BV
    _svVcrBVSet(_swLLStatus_avg,_na_state,_sbTvd_McDone_cnt,&_rTvd3dStatus);		
	#endif
    #ifdef CC_MT8223
    _svHSlicer(_na_state,&_rTvd3dStatus);	
    #endif
//-----------------TVD H Fine Tune----------------------/
    _svTvdHFineTune(_na_state, &_rTvd3dStatus);

//-----------------Mode Change Counter--------------------/
	//Start mode detection only when Tvd doesn't hold the mode.
	if ((_bTvdHoldMode==SV_TRUE) && (_rTvd3dStatus.eSourceType==SV_ST_TV))
	{
		if (!fgDrvTvdModeCntGt(_sbTvd_McDone_cnt))
		{
			vTvd3dTrigModeDet();
		}
	}
	vDrvTvdIncModeCnt();
//-----------------AVG CoChannel Counter for Serrtype Switch--------------------/
    #if TVD_SERRTYPE_ONE
	if (fgVPres)
	{
		if ((!fgDrvTvdModeCntGt(_sbTvd_McDone_cnt))&&(fgDrvTvdModeCntGt(_sbTvd_McDone_cnt-AVGCOIIR_CNT)))
		{
			_swCochIir_Avg+=RegReadFldAlign(DFE_STA_08, COCH_IIR);
			//printf("\n COCH iir=0x%x\n",RegReadFldAlign(DFE_STA_08, COCH_IIR));
		}
	}
    #endif
//-----------------Mode Switch Portection--------------------/
    #if TVD_MODESWITCH_PROTECT
    _svModeSwPrtec(fgVPres,bTvdMode,_sbTvd_McDone_cnt);  	
    #endif

//-----------------Fixed gain issue--------------------/
    #if TVD_DAGC_WA
        _svTvdFixedGainFineTune(_sbTvd_McDone_cnt, _sbIsTVDFixAgc, _na_state);
    #endif
   
//-----------------Mode Change Detection--------------------/
//-----------------phalt wa in PAL-N----------------------/
#if TVD_PHALT_WA 
		if(fgDrvTvdModeCntGt(150))
		{
			if ((_na_state == (UINT8)NA_SUPERHIGHNOISE)||(_na_state == (UINT8)NA_HIGHNOISE))
			{
				if((_rTvd3dStatus.bColSys == SV_CS_AUTO)&&(!fgHwTvdBLock4DET()))
				{
					if(((fgIs525 == 0)&&(fgHwTvd443() == 0)&&(fgHwTvdPHOLD() == 0))
						&&(_rTvd3dStatus.bTvdMode == SV_CS_PAL_N)
						)
					{
						if(_rTvd3dStatus.bTvdMode != bTvdMode)
						{
							vRegWriteFldAlign(CDET_00, SV_ON, TVD_MMODE); 
							//printf("\n do not run here ................\n");
							//printf("\n do not run here ................%d \n",_na_state);
							vRegWriteFldAlign(CDET_00, SV_CS_PAL_N, TVD_MODE); 
							bTvdMode=SV_CS_PAL_N;
							LOG(6,"[TVD_DBG_MSG] TVD_PHALT_WA TVD_MMODE PAL_N \n");
						}
					}
					else
					{
						vRegWriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); 
					}
				}
			}
			else 
			{
				if(_rTvd3dStatus.bColSys == SV_CS_AUTO)
				{
					vRegWriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); 
				}
			}
		}
#endif
    _svTvdModeChgDet(fgIs525, bTvdMode, &_rTvd3dStatus);
	
//-----------------Force Update Mode-----------------/
	if(bTvdCtrl(TCTL_MDSTBPROT,TC_GETEN,0)&&fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt-2))
	{
		vRegWriteFldAlign(CDET_05, TVD_FORCE_UPD_MODE?SV_ON:SV_OFF, FORCE_UPD_MODE);
	}

//-----------------Mode Change Done-----------------/
#if 0//TVD_PHALT_WA 
	if(_sbPhalt_Vpres==2)
	{
		if(fgDrvTvdModeCntGt(_sbTvd_McDone_cnt+1))
		{
			if(!fgDrvTvdModeCntGt(100))
			{
				if(fgHwTvdBLock())
				{
					_sbPhalt_Vpres=0;
					_svDrvTvdModeChgDone();
				}
			}
			else if(fgDrvTvdIsModeCnt(100))
			{
				if(fgHwTvdBLock())
				{
					_sbPhalt_Vpres=0;
					_svDrvTvdModeChgDone();
				}
				else
				{
					_sbPhalt_Vpres=0;
					vRegWriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); 
					_rTvd3dStatus.bTvdMode=bTvdMode;
					_svDrvTvdModeChgDone();
//					printf("\n do not run here4 ................\n");
//							printf("NPTV TVD BLOCK = %d ,TVD_MMODE=%d ,_sbTvdModeCnt=%d\n",fgHwTvdBLock(),fgHwTvdIsMMode(),_sbTvdModeCnt);
//							printf("mode status=%d ,NR_level=%d \n",(RegReadFldAlign(STA_CDET_00, Fld(3, 24, AC_MSKB3))),bHwTvdNRLevel());
				}
			}
		}
		else
		{
			if(fgHwTvdBLock())
			{
				_sbPhalt_Vpres=0;
			}
		}
	}
#endif
	if (fgDrvTvdIsModeCnt(_sbTvd_McDone_cnt))
	{
        #if TVD_SERRTYPE_ONE
		if (fgVPres)
		{
			_swCochIir_Avg=_swCochIir_Avg/AVGCOIIR_CNT;
			_sbAvgCOCH_detect=(_swCochIir_Avg>0x20)?1:0;
		}
        #endif
	
        //if(bTvdCtrl(TCTL_MDSTBPROT,TC_GETEN,0)&&!fgHwTvdIsMMode()&&_rTvd3dStatus.fgIsBurstLock
        //&&(_rTvd3dStatus.bTvdMode!=_sbTvdModeMapTable[(UINT8)_rTvd3dStatus.fgIs443nPHALT |(UINT8)(fgIs525) << 2]))
        //{
        //      _svDrvTvdModeChg();
        //}
        //else	
        //{
#if 0//TVD_PHALT_WA 
		if(_sbPhalt_Vpres!=2)
		{
			_sbPhalt_Vpres=0;
			_svDrvTvdModeChgDone();
		}
#else
        _svDrvTvdModeChgDone();
#endif
        //}
	}

    #if CAGC_PEAK_CNR_ADJ
    _svTvdAdjustCAGCPeak(_rTvd3dStatus.fgCNRLow ,_rTvd3dStatus.fgTrick, _rTvd3dStatus.bTvdMode);
    #endif

    //TVD CLI Nfy CallBack
    if (_pfnTVDCLINotify)
        _pfnTVDCLINotify(&_rTvd3dStatus);

}


/**
 * @brief TVD Connect/Dis-connect function
 *
 * To connect/dis-connect TVDecoder when input source is changed. Reset necessary registers
 * and firmware variables.
 *
 * @param bPath bPath indicates Main or PIP path currently TVD is connect to or disconnect from
 * @param bOnOff bOnOff indicates Connect or Disconnect
 * @return None
 */
void vTvd3dConnect(UINT8 bPath, UINT8 bOnOff)
{
	LOG(3,"[TVD_DBG_MSG] vTvd3dConnect bOnOff=%d \n",bOnOff);
	wTVDNRavg = 0;
	_rTvd3dStatus.fgIsVCR = 0;
	bhwVCRSetting = 0x0;
    #if TVD_FRS_FOR_NONSTDSIGNAL
	_wPreAvg_VLen = 0xff;
    #endif

    #if TVD_MAX_CLAMP_TABLE
	vRegWriteFldAlign(DFE_1F, DFE_CLAMP_TBL_05_IN_VP_ON, CLAMP_TABLE_05);
    #endif

	vDrvTvdinitVP1Cnt();
	bTvdCtrl(TCTL_NTSCONLY,TC_INIT|(DRVCUST_OptGet(eTVDSupportNTSCOnly)?TC_ENABLE:TC_DISABLE),1);
	bTvdCtrl(TCTL_MDSTBPROT,TC_INIT|TC_ENABLE,1); //Enable Force Updated 
	bTvdCtrl(TCTL_AGCPEDEN,TC_INIT|(DRVCUST_OptGet(eTVDSupportPedEn)?TC_ENABLE:TC_DISABLE),1);
    //Init Mode Deglish status. 
    #if TVD_MODE_Deglitch
    _svTvdInitModeDeglish();
    #endif 
    
	//Init _rTvd3dStatus variable
	if (bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
	{
		_rTvd3dStatus.bTvdMode = 0x3;
	}
	else
	{
		_rTvd3dStatus.bTvdMode = 0xff;
	}
	_rTvd3dStatus.fgVPres = FALSE;
	_rTvd3dStatus.fgTrick = FALSE;
	_rTvd3dStatus.fgSVPres = 0xFF; // To force notify mw SV no signal. Benson.
	_rTvd3dStatus.fgIsMV = FALSE;
	if (bTvdCtrl(TCTL_AGCPEDEN,TC_GETVAL,0)==1)
	{
		_bAgcPedEn = 1;
	}
	else
	{
		_bAgcPedEn = 0;
	}
	_rTvd3dStatus.eSourceType = bTvdCtrl(TCTL_INPUTTYPE,TC_GETEN,0)?
		bTvdCtrl(TCTL_INPUTTYPE,TC_GETVAL,0):bGetSignalType(bPath);

    #if TVD_FRS_FOR_NONSTDSIGNAL
    _svTvd3dRstNSTStatus(_rTvd3dStatus.fgIs525, &_rTvd3dNSTDStatus);     
    #endif

    //DFE initial
    vRegWriteFldAlign(DFE_00, DFE_CLAMP_START, CLAMP_START);
    #ifdef CC_MT5363_TVD 
    vRegWriteFldAlign(DFE_0E, 0x1, NR_DET_VPRES_SEL);
    //vRegWriteFldAlign(DFE_0E, 0x1, VPRES4TVD_MODE); //Enable VPRES4  // mtk70117 marked 	
    #else //5387  //Disable VPRES 4
    if (IS_5387_NonECO())
        vRegWriteFldAlign(DFE_0E, 0x0, VPRES4TVD_MODE); //Disable VPRES4
    else if (IS_5387_ECO())
    {
        vRegWriteFldAlign(DFE_0E, 0x1, NR_DET_VPRES_SEL);
        vRegWriteFldAlign(DFE_0E, 0x1, VPRES4TVD_MODE); //Enable VPRES4 
    }
    #endif 
    vRegWriteFldAlign(DFE_0E, 0x0, VPRES4PIC_MODE); //DFE referenced VPRES3

	vRegWriteFldAlign(DFE_02, 0x1, BLV_LIM_SEL);// Original Back-porch blank level

    #if TVD_COCH_FLICKER
	vRegWriteFldAlign(DFE_13, DFE_CLAMP_ERROR_SUM_TH, CLAMP_CURRENT_SUM_THR);
    #endif

	vRegWriteFldMulti(DFE_02,
        			  P_Fld(SV_ON, BLV_LIM_AUTO)|
        			  P_Fld(SV_OFF, BLV_LIM_EN));
	vRegWriteFldAlign(DFE_02, TVD_STD_Y4H, Y4H_BW); 
	vRegWriteFldMulti(DFE_08,
				P_Fld(SV_OFF, DCLAMP_Y_EN)|
				P_Fld(SV_ON, DCLAMP_2LINE_AVG)|
				P_Fld(DFE_DCLAMP_LOW_COR, DCLAMP_COR));
    #ifdef CC_MT8223
    vRegWriteFldAlign(DFE_0E, SV_OFF, DCLAMP_CHECK_LIM);//new reg for 8223
    vRegWriteFldAlign(DFE_0E, 0x30,DCLAMP_UP_LIM);//new reg for 8223	
    #endif
    vDrvSetTVDADCGain(DFE_AGAIN_DFT);
	vDrvTvd3dVPresOnOff(0x12, DFE_NORMAL_ONOFF_THRD);
	vRegWriteFldAlign(DFE_02, 1, POS_DIFF_ONLY);    //Consider invalid blank if blank region is MacroVision Like.
	vRegWriteFldMulti(DFE_09,
        			  P_Fld(SV_ON, CLAMP_HEADSWITCH_ADAP)|
        			  P_Fld(SV_OFF, CLAMP_VBI_FORBID)|
        			  P_Fld(SV_ON, CLAMP_VBI_NOISE_EN));

    #if TVD_VPRES_IMPROVE
     vRegWriteFldMulti(DFE_11,\
                      P_Fld(DFE_VPRES2_H_ON_GAIN, VP2_H_ON_OFFSET)|\
                      P_Fld(DFE_VPRES2_H_OFF_GAIN,VP2_H_OFF_OFFSET)|\
                      P_Fld(DFE_VPRES2_V_ON_GAIN, VP2_V_ON_OFFSET)|\
                      P_Fld(DFE_VPRES2_V_OFF_GAIN,VP2_V_OFF_OFFSET));			  
    #else
      vRegWriteFldMulti(DFE_11,\
                      P_Fld(DFE_VPRES2_H_ON_GAIN, VP2_H_ON_OFFSET)|\
                      P_Fld(DFE_VPRES2_H_OFF_GAIN,VP2_H_OFF_OFFSET)|\
                      P_Fld(DFE_VPRES2_V_ON_GAIN, VP2_V_ON_OFFSET)|\
                      P_Fld(DFE_VPRES2_V_OFF_GAIN,VP2_V_OFF_OFFSET));			  
    #endif

    #if TVD_8222_NONSTANDARD
	vRegWriteFldAlign(DFE_0F, SV_OFF, VP2_IGN_HSYNC_P);
    #endif
	vRegWriteFldAlign(DFE_10, DFE_VPRES2_HSYNC_W_LLIM, VP2_HSYNC_W_LLIM);// VP2_VSYNC_W_ULIM,VP2_VSYNC_W_LLIM,VP2_HSYNC_W_ULIM,VP2_HSYNC_W_LLIM
	vRegWriteFldAlign(DFE_21, SV_OFF, AGC_PED_EN);
	vRegWriteFldAlign(DFE_04, DFE_PEAKYC+(_wTargetBlk-DFE_BLANK_TARGET), AGC2_PYC_LIM);
	vRegWriteFldAlign(DFE_04, DFE_BYC_LIM, AGC2_BYC_LIM);
	vRegWriteFldAlign(DFE_03, DFE_NORMAL_LOOP_GAIN, AGC2_NORMAL_LOOP_GAIN);// Apply normal loop gain to avoid flag flickering.
	vRegWriteFldAlign(DFE_05, DFE_PEAK_STEP, AGC2_PEAK_STEP);
	vRegWriteFldAlign(DFE_05, DFE_AGAIN_STEP, AGC2_ACODE_STEP);//AGC2 gain steps
	vRegWriteFldAlign(DFE_18, SV_ON, VMAXMIN_EN);
	vRegWriteFldAlign(DFE_18, 5, VMAXMIN_STASEL); // Use Diffof Min level as status output.
	vRegWriteFldAlign(DFE_16, DFE_ABV_UP_TH, ABV_UP_TH);
    //5387 Add
	vRegWriteFldAlign(DFE_19, DFE_VDET_MBLK, VDET_MBLK);//MT5387 TODO:IC setting is different from FPGA 0xE0
	vRegWriteFldAlign(DFE_18, SV_ON, MULTIV_DET_EN);//MT5387
	vRegWriteFldAlign(DFE_23, DFE_MULTIV_DET_CNTH, MULTIV_DET_CNTH);//MT5387
	vRegWriteFldAlign(DFE_23, DFE_RAWV_WDTH, RAWV_WDTH);//MT5387 TODO 0x20 or 0x16
	vRegWriteFldAlign(DFE_12, DFE_VCNT_TH, VCNT_TH);//MT5387
	vRegWriteFldAlign(DFE_17, DFE_MAXMINMIN_TH, MAXMINMIN_TH);//MT5387: Default:0x40(Qlab airplane flutter 0dB will v_unlock)

	if(((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_DONE)==0xaa))
	||(!(_rTvd3dStatus.eSourceType==SV_ST_TV) && (bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_DONE)==0xaa)))
	{
		_sbIsTVDFixAgc = SV_ON;
		//TVD_AUTO_CALIB_DONE = 1
		if(_rTvd3dStatus.eSourceType==SV_ST_TV)
		{
			_sbTvdMenuAcode = bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0);
			_swTvdMenuDcode = (UINT16)(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN1)) |(((UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN2))<<8);
			vRegWriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
			vRegWriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
		}
		else
		{
			_sbTvdMenuAcode = bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0);
			_swTvdMenuDcode = (UINT16)(bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN1)) |(((UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN2))<<8);
			vRegWriteFldAlign(DFE_07, _sbTvdMenuAcode, AGC2_MANUAL_ACODE);
			vRegWriteFldAlign(DFE_07, _swTvdMenuDcode, AGC2_MANUAL_DCODE);
		}
	}
	else
	{
		_sbIsTVDFixAgc = SV_OFF;
	}

	if(_sbIsTVDFixAgc == SV_ON)
	{
            #if TVD_USE_NEW_DAGC
                vTvdSetAGCMode(E_MENU_AGAIN);
                vRegWriteFldAlign(DFE_03, SV_ON, MANUAL_DAGC_EN);
                vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_LowNA, FIXAGC_DAGCSEL);
            #else
                vTvdSetAGCMode(E_MENU_GAIN);
            #endif
	}
	else
	{
		vTvdSetAGCMode(E_AUTO_GAIN);			
		vRegWriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);
		vRegWriteFldAlign(DFE_18, DFE_AUTOAGC_DAGCSEL, FIXAGC_DAGCSEL);
            #ifdef CC_MT8223
            if(_rTvd3dStatus.eSourceType==SV_ST_TV)
            {
                vRegWriteFldAlign(DFE_07, DFE_ATD_MANUAL_DCODE, AGC2_MANUAL_DCODE);//for ATD snow noise is too dark
            }
            else
            {
                vRegWriteFldAlign(DFE_07, DFE_DFT_MANUAL_DCODE, AGC2_MANUAL_DCODE);
            }
            #endif
	}
    #if TVD_8222_NONSTANDARD
	vRegWriteFldAlign(TG_06, SV_ON, FDLY_SEL); //for MDET_V525_SEL Fh+-2000Hz
    #else
	vRegWriteFldAlign(TG_06, 0x0, FDLY_SEL);
    #endif
    vRegWriteFldAlign(TG_06, TG_SERR_CNT, SERR_CNT);
    _sbTVDSerrCnt = TG_SERR_CNT;
    //CDET initial
	vRegWriteFldAlign(CDET_09, TVD_CAGC_LIM_GAIN, CAGC_LIM_GAIN);
    #if TVD_8222_NONSTANDARD
	//MT5387 FPGA
	//MY & YJ recommend default off, after finefine, then implement new threshold into hw
	vTvd3dSetCAGCADAP(SV_OFF);
	vRegWriteFldAlign(CDET_00, SV_ON, PALSW_FAST_ADAP);
	vRegWriteFldAlign(CDET_05, SV_OFF, PALSW_STEP_F);
    #endif
    
	vRegWriteFldAlign(CDET_00, SV_OFF, DET443_SEL);
	vRegWriteFldAlign(CDET_00, SV_ON, CTG_ADAP);

    #if TVD_8222_NONSTANDARD
	vRegWriteFldAlign(CDET_00, SV_OFF, MODE_CKILL_EN);
	vRegWriteFldMulti(CDET_0C,
        			  P_Fld(SV_OFF,BSWTGT_IGNR)| //for chroma flicker
        			  P_Fld(SV_ON,DET443_NB_EN)|
        			  P_Fld(SV_ON,PHALT_NB_EN));
    #else
	vRegWriteFldAlign(CDET_00, SV_ON, MODE_CKILL_EN);
	vRegWriteFldMulti(CDET_0C,
        			  P_Fld(SV_ON,DET443_NB_EN)|
        			  P_Fld(SV_ON,PHALT_NB_EN));
    #endif

	vRegWriteFldAlign(CDET_01, CDET_LOW_NA_TH, LOW_NA_TH);
	vRegWriteFldMulti(CDET_03,
        			  P_Fld(CDET_DET443_UP,DET443_UP)|
        			  P_Fld(CDET_DET443_DN,DET443_DN));
	vRegWriteFldMulti(CDET_03,
        			  P_Fld(CDET_SCF_STD_UP,SCF_STD_UP)|
        			  P_Fld(CDET_SCF_STD_DOWN,SCF_STD_DN));
	vRegWriteFldAlign(CDET_04, CDET_SCF_TH, SCF_TH);
	vRegWriteFldAlign(CDET_04, CDET_DET443_TH, DET443_TH);
	vRegWriteFldAlign(CDET_05, CDET_BURST_THRESHOLD_SEL, BURST_THRESHOLD_SEL);
	vRegWriteFldMulti(CDET_06,
        			  P_Fld(CDET_SCKILL_UP,SCKILL_UP)|
        			  P_Fld(CDET_SCKILL_DN,SCKILL_DN));
	vRegWriteFldAlign(CDET_08, CDET_PAL_SW_GAIN, PAL_SW_GAIN);
    #if !TVD_8222_NONSTANDARD
	vRegWriteFldAlign(CDET_08, CAGC_LARGE_STEP, CAGC_L_STEP); 
    #endif
    vRegWriteFldAlign(CDET_08, SV_OFF, PHALT_SEL);//MT5387 4lines
	vRegWriteFldAlign(CDET_08, 0x00, CDET_PAL_MODE); //roll back to old PAL SECAM mode detect state machin
	vRegWriteFldAlign(CDET_05, SV_ON, CKILL_PHALT); //reset 443/358 up/down counter
	vRegWriteFldAlign(CDET_0F, CDET_SB4CTG_TH, SB4CTG_TH);//for VCR issue
	vDrvTvdRstCOCH2ENCnt();
	vDrvTvdRstCOCHDeCnt();
    vDrvTvdRstHsyncPosDetCnt();

	/*FPGA script*/
	//TG Init 
	vRegWriteFldAlign(TG_0B, SV_ON, AUTO_MLLOCK);
	vRegWriteFldAlign(TG_0C, SV_OFF, LLOCK_FINE_LIM_EN);
	vRegWriteFldAlign(TG_12, SV_ON, TOPFIELD_POLARITY); //same as MT5360A
	vRegWrite4B(TG_13, TG_KIKP_GAIN);
	//Disable LF_OFFSET 
    vRegWriteFldAlign(TG_04, SV_OFF, LF_OFFSET_EN);	
    #ifdef CC_MT8223
    vRegWriteFldAlign(TG_0C, TG_HSLICE_DEFAULT, HSLICE_ADJ);
    #endif

	//CTG Init 
	//MT5387 FPGA
	vRegWriteFldAlign(CTG_03, 1,CNRDET_EN);
	vRegWriteFldAlign(CTG_05, 1,BUNLOCK_CKILL); //??
//	vRegWriteFldAlign(TG_08, 0x0, TGEN_DEBUG);//MT5387 FPGA
    vRegWriteFldAlign(CDET_08, 0x1, MDET_ALL_RDY);//MT5387 FPGA

	vRegWriteFldMulti(CTG_00,\
        		  P_Fld(SV_OFF,FRUN_MSEED_EN)|\
        		  P_Fld(1,CTG_SWLBF)|\
        		  P_Fld(1,CTG_DEMOD_LPFSEL)|\
        		  P_Fld(SV_ON,CTG_PHASE_ERR_SEL)|\
        		  P_Fld(SV_OFF,BSYNC_AUTO)|\
        		  P_Fld(CTG_CHROMA_PG,CHROMA_PG));

    //MT5387 FPGA 0xbf  for bpres off->on,bsync auto counter 
    vRegWrite4B(TG_18, 0xbf488488);   //SCART_03, for KIP_FORCE Gain. china#35 ???
	
	vRegWriteFldAlign(CTG_03, SV_ON, CTG_BSTART_FORCE); //??

	vRegWriteFldAlign(VSRC_08, VSRC_COCH_SLV, COCH_SLV); //for COCH field signal upper screen Hjitter

	vRegWriteFldAlign(CDET_05, SV_ON, CAGC_LIM_EN);
	vRegWriteFldAlign(CDET_08, CDET_CAGC_PEAK_TC, CAGC_PEAK_TC);
	vRegWriteFldAlign(CDET_0B, CDET_CAGC_PEAK_GAIN, CAGC_PEAK_GAIN);
//	vRegWriteFldAlign(TG_08, 0x0, TGEN_DEBUG);//MT5387 FPGA
	vRegWriteFldAlign(CDET_0C, CDET_CAGC_MAX, CAGC_MAX);
	vRegWriteFldAlign(CDET_0D, CDET_SMALL_SYNC_TH, SMALL_SYNC_TH);
	vRegWriteFldAlign(CDET_0E, CDET_V625_LIMIT, V625_LIMIT); // set same value as 8226.
    _svTvd3dSetDetV525V625Gain(0x3, 0x3);
    _svDrvTvdSetDET443Gain(0, 0);	
	_svTvd3dSetPALTGain(5, 3);

	if ((_rTvd3dStatus.eSourceType==SV_ST_TV) && (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_US))
	{
		vRegWriteFldMulti(CDET_00,
        				P_Fld(SV_OFF,NTSC443_EN)|
        				P_Fld(SV_OFF,PAL60_EN));
	}
	else
	{
		vRegWriteFldMulti(CDET_00,
        				P_Fld(SV_ON,NTSC443_EN)|
        				P_Fld(SV_ON,PAL60_EN));
	}
    #if !TVD_SECAM_DET
	if (!(_rTvd3dStatus.eSourceType==SV_ST_TV))
	{
        _svSetSCMBurstWin(0x30, 0xb0);

	}
	else
    #endif
	{
        _svSetSCMBurstWin(0x40, 0x80);
	}

    //CTG initial
    #if TVD_8222_NONSTANDARD
	vRegWriteFldAlign(CTG_00, SV_OFF, BUKRSTSD_EN); /// for FSC burst Ckill improve
    #endif
	vRegWriteFldAlign(CTG_00, CTG_BSYNC_TYPE, BSYNC_TYPE);

	vRegWriteFldMulti(CTG_05,
        P_Fld(SV_ON, BUNLOCK_CKILL)|
        P_Fld(0,DEMOD_LPFSEL));// 0: 1.44 Mhz /1: 0.97Mhz  Bandwidth 

	vRegWriteFldMulti(CTG_05,
        P_Fld(SV_OFF,DETEN_SEL)|//DETEN_FORCE = 1; set 8 pixels cause of MV
        P_Fld(CTG_BP_OFFSET,BP_OFFSET));
	vRegWriteFldMulti(CTG_05,
			  P_Fld(SV_OFF,CTG_SV_SECAM_EN)|
			  P_Fld(SV_ON,DETEN_FORCE));//MT5387 MY
	vRegWriteFldAlign(CTG_07, 0x1, CTG_SECAM_PHASE_SEL);//MT5387

    #if TVD_CLAMP_ON_BLANK
	vRegWriteFldAlign(CTG_07, SV_ON, FIXBLV);    // Set Fix Blv. Target blank level will be set at adcset_int.c
    #endif

#if SUPPORT_SCART
	if ( !(_bMainICIn==(UINT8)P_FB0 || _bMainICIn==(UINT8)P_FB1
#if SUPPORT_POP
			|| _bSubICIn==(UINT8)P_FB0 || _bSubICIn==(UINT8)P_FB1
#endif
	      )
	   )
#endif
	{
		vTvd3dSetYCDelay(0);
#if SUPPORT_SCART
		_sbScart_SV = 0;
#endif
	}


    //TG initial
	vRegWriteFldAlign(TG_00, TG_MIN_HWIDTH, MIN_HWIDTH);    //Set min H sync width to 0x7 for Braizal Field signal and HWIDTH Relock.
	vRegWriteFldAlign(TG_02, TG_MASK_STOP_525, MASK_STOP);
	vRegWriteFldAlign(TG_05, TG_VHYST_EN, VHYST_EN);
	vRegWriteFldAlign(TG_0B, SV_OFF, AUTO_AVDELAY);
    #if TVD_FORCE_FAST_VALIGN
	vRegWriteFldMulti(TG_08,P_Fld(SV_OFF,FAST_VALIGN)|P_Fld(0x1, VALIGN_SPEED));
    #else
	vRegWriteFldAlign(TG_08, 0x2, VALIGN_SPEED);
    #endif
    if (IS_5363_NonECO() || IS_5387_NonECO())
    {
        vRegWriteFldAlign(TG_08, SV_ON, VFIR_EN);
    }
    else
    {
        vRegWriteFldAlign(TG_08, SV_OFF, VFIR_EN);
    }
    vRegWriteFldAlign(TG_08, 0, VFIR_SEL);       // 2 fields
	vRegWriteFldAlign(TG_14, 0x0, LLOCK_S0_THR);//cntseng ask MY lin 0x34
    vRegWriteFldAlign(TG_17, TG_FRAME_STDFV_TH_STD, FRAME_STDFV_TH);
    vRegWriteFldAlign(TG_17, TG_FRAME_STDFH_TH_STD, FRAME_STDFH_TH);
	vRegWriteFldAlign(TG_0F, 1, TKMODE_SW);
	#if TVD_NEW_VPORCH
	#if defined(CC_MT5387) || defined(CC_MT5363)
	if(IS_5387_ECO()||IS_5363_ECO())
	#endif
	{
		vRegWriteFldAlign(TG_0A,SV_ON,TG_CCVADJ_EN);
		vRegWriteFldAlign(TG_0A,0,TG_SAF_ADJ);
		vRegWriteFldAlign(TG_0A,SV_ON,CCV_F_LATCH_AUTO);
	}
	#endif
	vRegWriteFldMulti(TG_11,\
			  P_Fld(CCVSYNC_P_DEFAULT+1+TVD_VPORCH_TUNE,CCVSYNC1_P)|P_Pad0(2, 30)|\
			  P_Fld(CCVSYNC_P_DEFAULT+TVD_VPORCH_TUNE,CCVSYNC0_P)|P_Pad0(2, 22)|\
			  P_Fld(CCVSYNC_N_DEFAULT+TVD_VPORCH_TUNE,CCVSYNC1_N)|P_Pad0(2, 14)|\
			  P_Fld(CCVSYNC_N_DEFAULT+TVD_VPORCH_TUNE,CCVSYNC0_N)|P_Pad0(2, 6));
	vRegWriteFldAlign(TG_15, SV_ON, NHLOCK_EN);

	if (_rTvd3dStatus.eSourceType==SV_ST_TV)   // hsync de-glitch
	{
		vRegWriteFldAlign(TG_0D, TG_UNVLD_HSYNC_WIDTH_TV, UNVLD_HSYNC_WIDTH);
		vRegWriteFldAlign(DFE_0F, SV_ON, VP2_CHK_VSYNC);
		_rTvd3dStatus.NAInfo.na_HysterCntThres= 0x0A;
		
	}
	else
	{
		vRegWriteFldAlign(TG_0D, TG_UNVLD_HSYNC_WIDTH_SAV, UNVLD_HSYNC_WIDTH);
		vRegWriteFldAlign(DFE_0F, SV_OFF, VP2_CHK_VSYNC);
		_rTvd3dStatus.NAInfo.na_HysterCntThres = 0x40;
	}

    //other
	vRegWriteFldAlign(VSRC_08, SV_ON, VPRES_COCH_EN);
	/*if (bTvdCtrl(TCTL_MCNT,TC_GETEN,0) == 0)
	{	
	if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
	{
    	if (_rTvd3dStatus.eSourceType==SV_ST_TV)
    	{
    		_sbTvd_McDone_cnt=15;
    	}
    	else
    	{
    		_sbTvd_McDone_cnt=25;
    	}
	}
	else
	{
	    _sbTvd_McDone_cnt=12; //increase change channel time
	}
	}
	else
	{
		_sbTvd_McDone_cnt = bTvdCtrl(TCTL_MCNT, TC_GETVAL, 0);
	}*/
	vTvdSetOverSampleEnable(_bTVDOverSampleEn);
	//SECAM Setting
	vRegWriteFldAlign(SECAM_02, SCM_SEED_SEL, SEED_SEL);// MT5381TVDPort SECAM Port SEED_SEL
	vRegWriteFldMulti(SECAM_01,
        			  P_Fld(SV_ON,BPRES_CKILL_EN)|
        			  P_Fld(SV_ON,VMASK_CKILL_EN)|
        			  P_Fld(SV_ON,TVDCKILL_CKILL_EN)|
        			  P_Fld(SV_ON,BLENDING_SEL)|
        			  P_Fld(SCM_PDIFF_THR,PDIFF_THR)|
        			  P_Fld(SCM_MSTART_SEL,MSTART_SEL)|
        			  P_Fld(SCM_CSEL,CSEL)|
        			  P_Fld(SCM_YSEL,YSEL)
        			 );

	vRegWriteFldAlign(SECAM_02, SCM_MATCH_PAL_DELAY_FOR_C, MATCH_PAL_DELAY_FOR_C);
	vRegWriteFldAlign(SECAM_02, SCM_MATCH_PAL_DELAY_FOR_Y, MATCH_PAL_DELAY_FOR_Y);
	//SECAM Correct U/V Scale.
	vRegWriteFldMulti(SECAM_00,
        			  P_Fld(SCM_VSCALE,VSCALE)|
        			  P_Fld(SCM_USCALE,USCALE));

	//change SVF VPRES2 to VPRES1
	vRegWriteFldAlign(SVF_00, 0x0, SVF_VPRES_SEL);

	if(bTvdCtrl(TCTL_MDSTBPROT,TC_APPLY,0))//MT5387 FPGA
	{
		vRegWriteFldAlign(CDET_05, SV_ON, FORCE_UPD_MODE);
	}
	vRegWriteFldMulti(SECAM_06,
		P_Fld(SCM_STDVSTB_TH, STDVSTB_TH)|
		P_Fld(SCM_HLOCKSTB_TH, HLOCKSTB_TH)|
		P_Fld(SCM_VLOCKSTB_TH, VLOCKSTB_TH)|
		P_Fld(SCM_SCFSTB_TH, SCFSTB_TH));	  

	// Reset Related Counters
	vTvd3dTrigModeDet();

#if TVD_SECAM_DET
	vDrvTvdRstBurstCnt();
#endif
	// Connect
	if (bOnOff == SV_ON)
	{
		if (bPath == SV_VP_MAIN)
		{
			_rTvd3dStat.bIsMain = TRUE;
			// Trun on Interrupt
			vVDOINIrqOn(MSK_MAIN_DET);
		}
		else
		{
			_rTvd3dStat.bIsPip = TRUE;
			// Trun on Interrupt
			vVDOINIrqOn(MSK_PIP_DET);
		}
		if (bTvdCtrl(TCTL_MCNT,TC_GETEN,0) == 0)
		{	
			
    		_sbTvd_McDone_cnt=25;
		}
		else
		{
			_sbTvd_McDone_cnt = bTvdCtrl(TCTL_MCNT, TC_GETVAL, 0);
		}
		
		#if	TVD_AntiTrs
		_sbCAGC1=0;                                             //INIT THE VALUE
		_sfgCagc=0  ;   		             //INIT THE VALUE
		#endif
		
		
        vDrvTvdSetChanchgCnt(CHANCHG_START);
		vTvd3dReset();
		
	}
	else
	{
		if (bPath == SV_VP_MAIN)
		{
			// Reset VCR mode for DI
			vDrvDITrickModeOnOff(SV_VP_MAIN, SV_OFF);
			_rTvd3dStat.bIsMain = FALSE;
			vVDOINIrqOff(MSK_MAIN_DET);
		}
		else
		{
			// Reset VCR mode for DI
			vDrvDITrickModeOnOff(SV_VP_PIP, SV_OFF);
			_rTvd3dStat.bIsPip = FALSE;
			vVDOINIrqOff(MSK_PIP_DET);
		}
		//turn off Chroma Line Average to avoid DTV chroma flicker
		vDrvNRSetChromaLineAvgMode(bPath, NR_CHROMA_LINE_AVERAGE_MODE_OFF); //???
		/* 3D Comb */
		vDrvTDCActive(SV_OFF);
//mtk01140 20090902 3D COMB Callbak Test
#ifndef TDC_CALLBACK_CONTROL_BY_FBM		
            vDrvTDCOnOff(SV_OFF);
#endif //TDC_CALLBACK_CONTROL_BY_FBM            
		_rTvd3dStatus.bSigStatus = (UINT8)SV_VDO_NOSIGNAL;
		_rTvd3dStatus.bTvdMode = SV_CS_NONE;
        vDrvTvdSetChanchgCnt(CHANCHG_STOP);
	}

	//CAGC status change to old cagc, YJ will change hw to old cagc CAGC_STA_SEL
	vRegWriteFldAlign(CDET_08, 1, CAGC_STA_SEL); //CAGC_STA_SEL

	//YJ for PAL-N signal lock in PAL mode, will set by HW by def.
	vRegWriteFldAlign(CTG_07, 1, CTG_4DET_SUM_SEL); //SUM8SEL

    // Set CAGC bound for ctrl path. 
    _svTvd3dSetCAGCBound4Ctrl(PAL_CAGC_BOUND);
    vRegWriteFldAlign(CDET_0B, 0, CAGC_DATA_PATH_SEL); 
	vRegWriteFldAlign(CDET_0B, 0, CAGC_CTL_PATH_SEL); 
    vRegWriteFldAlign(CDET_06, CDET_KBURST_PAL, KBURST);

    #if TVD_VCR_BV_STA
    vDrvTvdRstVCRBVStaCnt();
    #endif

    #if CAGC_PEAK_CNR_ADJ
    vRegWriteFldAlign(CTG_05, SV_OFF, CNR_BSTADJ_EN);
    vRegWriteFldAlign(CTG_03, 1, SUM_DIV_SEL);
    #endif 

	
}


/**
 * @brief The refresh rate of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the refresh rate of current input signal of TVDecoder.
 * The refresh rate of possible TVDecoder input are 50 or 60, depend on
 * the mode of input signal.
 *
 * @param None
 * @return the refresh rate of current video signal
 *
 */
UINT8 bTvd3dRefreshRate(void)
{

	return ((_rTvd3dStatus.fgIs525)?60:50);
}

/**
 * @brief The Signal Status of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the Signal Status of current input signal of TVDecoder.
 *
 * @param None
 * @return the Signal Status of current video signal, possible return values include
 * SV_VDO_NOSIGNAL, SV_VDO_NOSUPPORT, SV_VDO_UNKNOWN and SV_VDO_NOSTABLE.
 *
 */
UINT8 bTvd3dSignalStatus(void)
{
#if 0//SUPPORT_ATV_SNOWSCREEN
        if(fgIsSrcAtv(SV_VP_MAIN)|| fgIsSrcAtv(SV_VP_PIP))
        {
	        return SV_VDO_STABLE;
	 }
	 else
	 {
            return _rTvd3dStatus.bSigStatus;
	 }
#else

	//if ((bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_EU) && IS_AUTOSEARCH())
	if (IS_AUTOSEARCH())
	{
			return SV_VDO_STABLE; // For EU system channel scan screen crash
	}
	return _rTvd3dStatus.bSigStatus;
#endif
}

/**
 * @brief Set 525, 443 gain to fasten the speed of changing channel.
 * @param bOnOff Set / Restore the register setting for fast changing channel ( Fast setting would cause false channel if
 *                       it is applied under autosearching).
 * @return None
 */

void vTvd3dFastChannelChange(UINT8 bOnOff)
{
	if (bOnOff==SV_ON)
	{
		//_sbFastChannelChange = SV_TRUE;
		_svTvd3dSetDetV525V625Gain(0x3, 0x3);
        #if TVD_FAST_MODEGAIN_CHANNEL_CHANGE
        _svDrvTvdSetDET443Gain(3,3);
        #endif
	}
	else
	{
		//_sbFastChannelChange = SV_FALSE;
        _svTvd3dSetDetV525V625Gain(0,0);
        #if TVD_FAST_MODEGAIN_CHANNEL_CHANGE
        _svDrvTvdSetDET443Gain(0,0);
        #endif
	}
}

void vTvd3dChannelChange(void)
{
	// Prevent blackmute when block TV in AV.
	if(_rTvd3dStatus.eSourceType==SV_ST_TV) 
	{
		vTvd3dReset();
        vDrvTvdSetChanchgCnt(CHANCHG_START);
	}
}
/**
 * @brief Triger TVDecoder firmware to re-confirm the format of input signal
 *
 * (Common Function of Video Decoders)
 * Triger TVDecoder firmware to re-confirm the format of input signal, current no hardware
 * action is taken.
 *
 * @param None
 * @return None
 *
 */
void vTvd3dTrigModeDet(void)
{
	vDrvTvdRstModeCnt();
}

/**
 * @brief Set the color system of TVDecoder
 *
 * return the width of current input signal of TVDecoder
 *
 * @param bPath bPath specifies whether SV_VP_MAIN or SV_VP_PIP is the target.
 * @param bColSys bColSys specifies the desired color system, it can be SV_CS_PAL_N,
 * SV_CS_PAL, SV_CS_PAL_M, SV_CS_NTSC358, SV_CS_SECAM, SV_CS_PAL_60, SV_CS_NTSC443, and
 * SV_CS_AUTO (for auto mode detection).
 * @return None
 */
void vDrvTvd3dSetColorSystem(UINT8 bColSys)
{
	LOG(3,"[TVD_DBG_MSG] vDrvTvd3dSetColorSystem bColSys=%d \n",bColSys);
	if (bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)== 1)
	{
		vRegWriteFldMulti(CDET_00, P_Fld(SV_ON, TVD_MMODE)|P_Fld(SV_CS_NTSC358, TVD_MODE));//Menu NTSC Mode
		return;
	}
	if (bColSys != _rTvd3dStatus.bColSys)
	{
		vTvd3dReset();
		if (bColSys == SV_CS_AUTO)	// Switch to Hardware Auto Detect mode
		{
			vRegWriteFldAlign(CDET_00, SV_OFF, TVD_MMODE); //Auto Mode
		}
		else
		{
			vRegWriteFldAlign(CDET_00, bColSys, TVD_MODE); 
			vRegWriteFldAlign(CDET_00, SV_ON, TVD_MMODE); //Enable Menu Mode
		}

		_rTvd3dStatus.bColSys = bColSys;
	}
	else
	{
		return;
	}

	vTvd3dTrigModeDet();

	if (fgIsMainTvd3d())
	{
		vSetMainFlg(MAIN_FLG_MODE_CHG);
	}

	if (fgIsPipTvd3d())
	{
		vSetPipFlg(PIP_FLG_MODE_CHG);
	}
}

/**
 * @brief Set the color system of TVDecoder
 *
 * return the width of current input signal of TVDecoder
 *
 * @param bColSys bColSys specifies the desired color system, it can be SV_CS_PAL_N,
 * SV_CS_PAL, SV_CS_PAL_M, SV_CS_NTSC358, SV_CS_SECAM, SV_CS_PAL_60, SV_CS_NTSC443, and
 * SV_CS_AUTO (for auto mode detection).
 * @return None
 */
UINT8 bTvd3dGetColorSystem(void)
{
	if (fgIsMainTvd3d()||fgIsPipTvd3d())
	{
		if (fgHwTvdIsMMode())
		{
			return (bHwTvdMMode());
		}
		else
		{
			return (bHwTvdMode());
		}
	}
	else
	{
		return (SV_CS_UNSTABLE);
	}
}


void vTvd3dReset(void)
{
	CRIT_STATE_T csState;
	csState = x_crit_start();

	vDrvTDCActive(SV_OFF);

	vTvd3dResetOn();
	vUtDelay2us(2);
	vTvd3dResetOff();
    _svTvdRstVpresState();
	x_crit_end(csState);
	vUtDelay10ms(10);
}


UINT8 bTvdNRLevel(void)
{
	return (bHwTvdNRLevel());
}

#if TVD_CHECK_BURST
/**
 * @brief   For tuner, check TVDLock Burst frequency.
 *
 * @param   none
 *
 * @retval  IS443/IS358/unknow
 */

UINT8 bDrvTvdCheckBurstFreq(void)
{

UINT32 dwTVDBurstSeed;

	vRegWriteFldAlign(CTG_00, SV_OFF, BSYNCSTA_SEL); 
	vRegWriteFldAlign(CTG_04, SV_OFF, STACTG00_SEL); 
	dwTVDBurstSeed=RegReadFldAlign(STA_CTG_00,Fld(20, 12, AC_FULLDW));

	if((fgHwTvdVPres())&&fgHwTvdBLock4DET()&&(bHwTvdCAGC()<TVD_SCAN_CAGCThres))
	{
		if (fgHwTvd443() && (dwTVDBurstSeed < TVD_BURSTSEED_THR))
		{
			return TVD_BLOCK_IS443;
		}
		else if((!fgHwTvd443()) && (dwTVDBurstSeed > TVD_BURSTSEED_THR))
		{
			return TVD_BLOCK_IS358;
		}
		else
		{
			return TVD_BLOCK_UNKNOW;
		}
	}
    return TVD_BLOCK_UNKNOW;
}
#endif

/**
 * @brief   For tuner, check TVDLock.
 *
 * @param   bTryLoopCnt: loop count
 *
 * @retval  TRUE/FALSE
 */
BOOL fgDrvTvdCheckTVDLock( UINT16 bTryLoopCnt )
	{
    // update interval * max value = 2048*128 = 2^18
    // 2^18 / 27 MHz = 0.0097 sec  = about 10 ms
    // tatal: 10Times x 2ms =20ms to do safe check
    if (_rTvd3dStatus.eSourceType!=SV_ST_TV) return TRUE;//for TV SCART bypass, it should not check TVD lock
    
	if(IS_AUTOSEARCH())
	{
    vTvd3dReset(); 
    //wTryLoopCnt = wVpresCheckTime;

    // CN suggest to add 100ms after reset for scan loss, 081114
    	//vUtDelay10ms(10);
	}

    while(bTryLoopCnt>0)
    {
        if (RegReadFldAlign(STA_CDET_00, VPRES_TVD3D)) //avoid vpres4 lock the sound carrier
        {
            if(bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0) == COUNTRY_US)
            {            
				LOG(3,"[TVD_DBG_MSG] fgDrvTvdCheckTVDLock Vpres lcoked\n");
                return TRUE;
            }
            else
            {
                //don't check Vlock when setting channel, Yulia,2005.10.24
                if(IS_AUTOSEARCH())
                {
                    if(_sfgDrvTvdCheckVLock(VLOCK_CHECK_TIME) != TRUE)
                    {//check again when Vpres on but vunlock
                        return _sfgDrvTvdCheckVLock(VLOCK_CHECK_TIME);
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                else
                {
                    return TRUE;
                }
            }
        }
        vUtDelay1ms(2);
        bTryLoopCnt --;
    }
	LOG(3,"[TVD_DBG_MSG] fgDrvTvdCheckTVDLock Vpres unlcoked\n");
    return FALSE;
}


/**
 * @brief   Get NA state.
 *
 * @param   None
 *
 * @retval  2:High Noise, 1: Low Noise
 */
UINT8 bTvdNAState4Step(void)
{
    if(_swTVDNRLevel_AVG > TVDNRLevel_TH)
    {
         return 2;//high noise
    }
    else
    {
        return 1;//low noise
    }
}

/**
 * @brief signal level for tuner
 * @param bMode 
 *	0: 100~0 
 *	1: dB value
 * @return the signal strength level 
 */
UINT8 bTvdSignalStrengthLevel(UINT8 bMode)
{
	UINT8 bValue;

	bValue = 100 -((_swTVDNRavg1*100)>>7);
	if(fgHwTvdVPres())
	{
		if(bMode == 0)
		{
			return bValue;
		}
		//get signal level interface in mw_if
		else if(bMode == 2)
		{
		    bValue = ((_svTvdGetNRLevel()-20)*100)/60;
		    return bValue;
		}
		else
		{
			return _svTvdGetNRLevel();
		}
	}
	else
	{
		return 0;
	}
}


/**
 * @brief The width of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the width of current input signal of TVDecoder.
 * The width of possible TVDecoder input is only 720.
 *
 * @param None
 * @return the width of current video signal
 *
 */
UINT16 wTvdWidth(void)
{
	return RegReadFldAlign(SYS_00,DATA_SYNC_DUAL)?1440:720;
}

/**
 * @brief The height of current input signal of TVDecoder
 *
 * (Common Function of Video Decoders)
 * return the height of current input signal of TVDecoder.
 * The height of possible TVDecoder input are 480 or 576, depend on
 * the mode of input signal.
 *
 * @warning When tuner is performing auto-search, we force the return height to be 480,
 * to prevent scale factor unstable during the process.
 * @param None
 * @return the height of current video signal
 *
 */
UINT16 wTvd3dHeight(void)
{
	return _rTvd3dStatus.fgIs525?480:576;
}



#if TVD_AUTO_CALIB
/**
 * @brief vDrvIntAutoColorStart( void )
 * Internal auto color start
 * @param  void
 * @retval void
 * @example vDrvIntAutoColorStart()
 */
void vDrvTVDAutoCalibStart(void)
{
	if (!fgIsMainTvd3d() && !fgIsPipTvd3d())
	{
		LOG(1,"Calibration Fail !  Please use correct source to do calibration\n");
		return;
	}

	_bTVDAutoCalibState = VDO_AUTO_COLOR_START;
	_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_1_START;
	_sbIsTVDFixAgc = SV_ON;
}

void vDrvTVDAutoCalibState(void)
{
	switch (_bTVDAutoCalibState)
	{
		case VDO_AUTO_COLOR_NOT_BEGIN:
			break;

		case VDO_AUTO_COLOR_START:		
			if(bDrvTVDAdcCalib())
			{
				vApiVideoClrEvtFlg(VDO_FLG_TVDAUTO_DOING);
				_bTVDAutoCalibState = VDO_AUTO_COLOR_END;
				if(_sbTVDAutoCalibResult == SV_SUCCESS)
				{	
					vApiNotifyAutoColorDone((fgIsMainTvd3d()?0:1),TRUE);			
				}	
				else
				{
					vApiNotifyAutoColorDone((fgIsMainTvd3d()?0:1),FALSE);			
				}
			}	
			break;

		case VDO_AUTO_COLOR_END:
				_bTVDAutoCalibState = VDO_AUTO_COLOR_NOT_BEGIN;
                            LOG(1,"======> Auto Color Finish !!! <====== \n");
				break;
		default:
				break;
	}
}

void vDrvSetTVDOffset(UINT16 value)
{
	vRegWriteFldAlign(CTG_07, value, BLKLVL);
}


/**
 * @brief bDrvGetTVDADCGain(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT8 manual analog gain code
 */
UINT8 wDrvGetTVDADCMAX(void)
{
    	if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
	{
		return bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_MAX);
	}
	else
	{
		return bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_MAX);
	}
}


//Mike modify ADC setting 20080704
/**
 * @brief vCust_TVD_ADC_MAX_Update(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT8 manual analog gain code
 */
void  vCust_TVD_ADC_MAX_Update(UINT8 value)
{
    	if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
	{
		UNUSED( fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_MAX,value));
	}
	else
	{
		UNUSED( fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_MAX,value));
	}
}

void vCust_TVD_Gain_Update(UINT8 value)
{
    	if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0, value));
		//UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_DONE, 0xAA));
	}
	else
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0, value));
		//UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_DONE, 0xAA));
	}
}

/**
 * @brief bDrvGetTVDADCOffset(void)
 * call by d_custom_vid_get
 * @param  void
 * @retval UINT16 manual offset code
 */
UINT16 bDrvGetTVDADCOffset(void)
{
	return (UINT16)RegReadFldAlign(DFE_0A, CLAMP_TARGET_BLANK_LV);
}
/**
 * @brief bDrvSetTVDADCOffset(UINT16 value)
 * call by d_custom_vid_set
 * @param  value manual offset code
 * @retval void
 */
void vDrvSetTVDADCOffset(UINT16 value)
{
	_sbIsTVDOffsetbyAP = 1;
	vRegWriteFldAlign(DFE_0A, value, CLAMP_TARGET_BLANK_LV);
}

void vCust_TVD_Offset_Update(UINT16 value)
{
	value = value - 200;//only store the value - 200
    	if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_OFFSET, (UINT8)value));
	}
	else
	{
		UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_OFFSET, (UINT8)value));
	}
}

/**
 * @brief    AV and TV ADC Calibration target 16~235
 * @remarks
 * @return   TRUE/FALSE
 */
BOOL bDrvTVDAdcCalib(void)
{
	UINT8 MANUAL_ACODE, TUNE_STEP;
	UINT16 MANUAL_DCODE;
	UINT8 Tolerance = 0x2;
	UINT16 TARGET_Y_LEVEL;
#ifdef CC_TVE_POWERDOWN_HALF
	UINT16 TARGET_SYNC_ORI;
#endif
	switch (_rTvd3dStatus.bTvdMode)
	{
		case SV_CS_SECAM:
		case SV_CS_PAL:
		case SV_CS_PAL_N:
			if(DRVCUST_OptGet(eTVDSupportOutput216))
			{
				TARGET_Y_LEVEL = TARGET_Y_LEVEL_P216;
			}
			else
			{
				TARGET_Y_LEVEL = TARGET_Y_LEVEL_P;
			}
			break;
		default:
			if(DRVCUST_OptGet(eTVDSupportOutput216))
			{
				TARGET_Y_LEVEL = TARGET_Y_LEVEL_N216;
			}
			else
			{
				TARGET_Y_LEVEL = TARGET_Y_LEVEL_N;
			}
			break;
	}

#ifdef CC_TVE_POWERDOWN_HALF
			//amplifier gain=2.83, 1/(2.83/2)=0.707
				switch (_rTvd3dStatus.bTvdMode)
				{
					case SV_CS_SECAM:
					case SV_CS_PAL:
					case SV_CS_PAL_N:
						TARGET_SYNC_ORI = DFE_TGT_SYNC_HEIGHT_625;
						break;
					default:
						TARGET_SYNC_ORI = DFE_TGT_SYNC_HEIGHT_525;
						break;
				}
				if(RegReadFldAlign(DFE_03, AGC2_MODE) == E_AUTO_GAIN)
				{
					TARGET_Y_LEVEL = TARGET_Y_LEVEL*0.76;
				}
				else
				{
					TARGET_Y_LEVEL = TARGET_Y_LEVEL;
				}
#endif
	//Enable AGC
	if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_1_START)
	{
#ifdef CC_TVE_POWERDOWN_HALF
			if(DRVCUST_OptGet(eTVDSupportOutput216))
			{
				_swTvd_Gain = TARGET_SYNC_ORI*0.69;//0.76*0.91;
			}
			else
			{
				_swTvd_Gain = TARGET_SYNC_ORI*0.76;
			}
			_svDrvSetTVDAGCTarget(_swTvd_Gain);
#endif
		vDrvWhitePeakLimit(SV_OFF);
		vRegWriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);
		vRegWriteFldAlign(DFE_18, DFE_AUTOAGC_DAGCSEL, FIXAGC_DAGCSEL);
		vTvdSetAGCMode(E_AUTO_GAIN);			
		_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_2_START;
		_sbTVDAutoCalibCount = 0;
		_swY_Avg_Lvl = wDrvGetTVDYLvl();		//get Avg. PY_LV dummy read
		return SV_FAIL;
	}

	if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_2_START)
	{
		_sbTVDAutoCalibCount++;
		if(_sbTVDAutoCalibCount == TVDAutoCalibDelayTime)
		{
			_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_3_START;
			_sbTVDAutoCalibCount = 0;
		}
		return SV_FAIL;
	}

	if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_3_START)
	{	
		_swTvd_Gain = _swDrvGetTVDAGCTarget() & 0x0fff;		//gain calibration
		_swY_Avg_Lvl = wDrvGetTVDYLvl();		//get Avg. PY_LV
		_swY_Diff = (_swY_Avg_Lvl>TARGET_Y_LEVEL)?(_swY_Avg_Lvl-TARGET_Y_LEVEL):(TARGET_Y_LEVEL-_swY_Avg_Lvl);

		if(_swY_Diff > 0xA0)
		{
			printf("_swY_Avg_Lvl= %d,TARGET_Y_LEVEL=%d,_swY_Diff=%d\n",_swY_Avg_Lvl,TARGET_Y_LEVEL,_swY_Diff);
			printf("Calibration Fail !  Please use correct pattern to do calibration\n");
			_bTVDAutoCalibState = VDO_AUTO_COLOR_END;
			_sbTVDAutoCalibResult = SV_FAIL;
			return SV_TRUE;
		}
		_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_4_START;
		return SV_FAIL;
	}

	if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_4_START)
	{
		if(_swY_Diff>0x40)
		{
			TUNE_STEP = 0x2;
		}
		else
		{
			TUNE_STEP = 0x1;
		}
		if(_swY_Avg_Lvl > TARGET_Y_LEVEL)
		{
			_swTvd_Gain -= TUNE_STEP;
		}
		else
		{
			_swTvd_Gain += TUNE_STEP;
		}			
		_svDrvSetTVDAGCTarget(_swTvd_Gain);
		printf("_swTvd_Gain(Target_Sync_High)=%x\n",_swTvd_Gain);

		if((_swTvd_Gain > 0x400) || (_swTvd_Gain < 0x200))
		{
			_bTVDAutoCalibState = VDO_AUTO_COLOR_END;
			_sbTVDAutoCalibResult = SV_FAIL;
			return SV_TRUE;
		}
		_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_5_START;
		return SV_FAIL;
	}
	if(_sbTVDAutoCalibState1 == VDO_AUTO_COLOR_5_START)
	{	
		_swY_Avg_Lvl = wDrvGetTVDYLvl();
		_swY_Diff = (_swY_Avg_Lvl>TARGET_Y_LEVEL)?(_swY_Avg_Lvl-TARGET_Y_LEVEL):(TARGET_Y_LEVEL-_swY_Avg_Lvl);
		printf("_swY_Avg_Lvl=%x , _swY_Diff=%x\n",_swY_Avg_Lvl, _swY_Diff);
		
		if(_swY_Diff > Tolerance)
		{
			_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_4_START;
			return SV_FAIL;
		}

		//Fix AGC
		MANUAL_ACODE = RegReadFldAlign(DFE_STA_04, AGAIN_CODE);
		if(_sbPre_Manual_ACode != MANUAL_ACODE)
		{
			_sbPre_Manual_ACode = MANUAL_ACODE;
			_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_5_START;
			return SV_FAIL;
		}
#ifdef CC_TVE_POWERDOWN_HALF
		if(RegReadFldAlign(DFE_03, AGC2_MODE) == E_AUTO_GAIN)
		{
			printf("Calibration Again ok, start calibrate Dgain\n");
			if(DRVCUST_OptGet(eTVDSupportOutput216))
			{
				_swTvd_Gain = TARGET_SYNC_ORI*0.91;
			}
			else
			{
				_swTvd_Gain = TARGET_SYNC_ORI;
			}
			_svDrvSetTVDAGCTarget(_swTvd_Gain);
			vDrvSetTVDADCGain(MANUAL_ACODE);
			vTvdSetAGCMode(E_MENU_AGAIN);
			_sbTVDAutoCalibState1 = VDO_AUTO_COLOR_5_START;
			return SV_FAIL;
		}
//UINT8 i;
//LOG(1,"DCODE start\n");
//for(i = 0; i<50; i++)
//{
//	MANUAL_DCODE = RegReadFldAlign(DFE_STA_04, DGAIN_CODE);
//	LOG(1,"DCODE=%x\n",MANUAL_DCODE);
//	vUtDelay1ms(1);
//}
//LOG(1,"DCODE END\n");
#endif

		vTvdSetAGCMode(E_HOLD_GAIN);	
		MANUAL_DCODE = RegReadFldAlign(DFE_STA_04, DGAIN_CODE);
		printf("MANUAL_ACODE=%x\n",MANUAL_ACODE);
		printf("MANUAL_DCODE=%x\n",MANUAL_DCODE);

        vDrvSetTVDADCGain(MANUAL_ACODE);
        vDrvSetTVDADCDGain(MANUAL_DCODE);
            #if TVD_USE_NEW_DAGC
                vTvdSetAGCMode(E_MENU_AGAIN);
                vRegWriteFldAlign(DFE_03, SV_ON, MANUAL_DAGC_EN);
                vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_LowNA, FIXAGC_DAGCSEL);
            #else
                vTvdSetAGCMode(E_MENU_GAIN);
            #endif
		vDrvWhitePeakLimit(SV_ON);
		//save data in EEPROM after calibartion
		switch (_rTvd3dStatus.bTvdMode)//YUV2YCbCr
		{
			case SV_CS_SECAM:
			case SV_CS_PAL:
			case SV_CS_PAL_60:
			case SV_CS_PAL_N:
				//wYlevel = ((wYlevel+40)*1.6)/4;
				_swY_Avg_Lvl = ((_swY_Avg_Lvl+40)*2)/5;
				break;
			default:
				//wYlevel = ((wYlevel+41)*1.56)/4;
				_swY_Avg_Lvl = ((_swY_Avg_Lvl+41)*39)/100;
				break;
		}

			printf("TVD MAX=%d\n",_swY_Avg_Lvl);
		
	    	if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
		{
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0, MANUAL_ACODE));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN1, (UINT8)MANUAL_DCODE));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN2, (UINT8)(MANUAL_DCODE>>8)));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_MAX, _swY_Avg_Lvl));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_TV_DONE, 0xAA));
		}
		else
		{
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0, MANUAL_ACODE));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN1, (UINT8)MANUAL_DCODE));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN2, (UINT8)(MANUAL_DCODE>>8)));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_MAX, _swY_Avg_Lvl));
			UNUSED(fgApiEepromWriteByte(EEP_VIDEO_AUTO_CALIB_AV_DONE, 0xAA));
		}
		_sbTVDAutoCalibResult = SV_SUCCESS;
		printf("Calibration SUCCESS!\n");
		return SV_SUCCESS;
	}
	return SV_FAIL;
}

BOOL bDrvTVDAdcCalib4Auto(void)
{
	UINT16 wY_Avg_Lvl;
	UINT16 wTvd_Gain;
	UINT16 TARGET_Y_LEVEL;

	if (!fgIsMainTvd3d() && !fgIsPipTvd3d())
	{
		return SV_FAIL;
	}

	_sbIsTVDFixAgc = SV_OFF;

	//Disable white peak limiter
    vDrvWhitePeakLimit(SV_OFF);
	//Enable AGC
    vTvdSetAGCMode(E_AUTO_GAIN);			
	vUtDelay10ms(10);

	wTvd_Gain = _swDrvGetTVDAGCTarget();
	wY_Avg_Lvl = wDrvGetTVDYLvl();

	switch (_rTvd3dStatus.bTvdMode)
	{
		case SV_CS_SECAM:
		case SV_CS_PAL:
		case SV_CS_PAL_60:
		case SV_CS_PAL_N:
			TARGET_Y_LEVEL = TARGET_Y_LEVEL_P;
			break;
		default:
			TARGET_Y_LEVEL = TARGET_Y_LEVEL_N;
			break;
	}

	while (wY_Avg_Lvl != TARGET_Y_LEVEL)
	{
        if ((wY_Avg_Lvl > TARGET_Y_LEVEL) && wTvd_Gain >0)
        {
            wTvd_Gain--;
        }
        else if ((wY_Avg_Lvl < TARGET_Y_LEVEL) && wTvd_Gain < 0xFFF)
        {
            wTvd_Gain++;
        }
		_svDrvSetTVDAGCTarget(wTvd_Gain);
		LOG(5, "wTvd_Gain=%x\n",wTvd_Gain);
		vUtDelay10ms(10);
		wY_Avg_Lvl = wDrvGetTVDYLvl();
		LOG(5, "wY_Avg_Lvl=%x\n",wY_Avg_Lvl);
	}

    //Enable white peak limiter
    vDrvWhitePeakLimit(SV_ON);
	return SV_SUCCESS;
}
#endif

void vTvd3dSetYCDelay(UINT8 bYCDelay)
{
	if(_sbIsSetYCDelaybyAP) return;
	vRegWriteFldAlign(CTG_07, (bYCDelay&0x3) ,Y_DELAY); //Y_DELAY
	vRegWriteFldAlign(CTG_07, (bYCDelay>>2) ,UV_DELAY); //UV_DELAY
}

void vTvd3dSetYCDelaybyAP(UINT8 bYDelay, UINT8 bCDelay)
{
	_sbIsSetYCDelaybyAP = SV_ON;
	vRegWriteFldAlign(CTG_07, bYDelay ,Y_DELAY); //Y_DELAY
	vRegWriteFldAlign(CTG_07, bCDelay ,UV_DELAY); //UV_DELAY
}

//void vTvd3dSetCAGCBound(UINT32 dwBound)
//{
//	if(_sbIsSetCAGCbyAP == SV_ON) return;
//	vRegWrite4B(CDET_07, dwBound);// CAGCBound ??
//}


void vTvd3dSetCAGCbyAP(void)
{
	_sbIsSetCAGCbyAP = SV_ON;
	vRegWriteFldAlign(CDET_05, _rTvd3dCAGCInfo.MCAGC_EN, MACC_EN);
	vRegWriteFldAlign(CDET_05, _rTvd3dCAGCInfo.MCAGC, MACC);
	_svTvdSetCAGCBound4Data((_rTvd3dCAGCInfo.Bound>>24), 
                            (_rTvd3dCAGCInfo.Bound>>16)&0xFF,
                            (_rTvd3dCAGCInfo.Bound>>8)&0xFF,
                            _rTvd3dCAGCInfo.Bound&0xFF);
	//vRegWrite4B(CDET_07, _rTvd3dCAGCInfo.Bound);// CAGCBound
	vRegWriteFldAlign(CDET_08, _rTvd3dCAGCInfo.L_Step, CAGC_L_STEP);
	vRegWriteFldAlign(CDET_08, _rTvd3dCAGCInfo.S_Step, CAGC_S_STEP);
}

void vTvd3dATDInit(UINT8 bATDOnOff)
{
	if (bATDOnOff==SV_ON)
	{
		// Set Noise Measure Source.
		vRegWriteFldAlign(CDET_0C, SV_ON, NR_VSRC_SEL);
		// ATD Flag
		//_sbIsATDOnOff = SV_ON;
	}
	else // ATD Off
	{
		// Set Noise Measure Source.
		vRegWriteFldAlign(CDET_0C, SV_OFF, NR_VSRC_SEL);
		// ATD Flag
		//_sbIsATDOnOff = SV_OFF;
	}
	vRegWriteFldAlign(DFE_0D, DFE_CLAMP_ATD_GAIN, CLAMP_ATD_GAIN);
}

UINT8 bTvdLumaSignalRefreshRate(void)
{
	return fgHwTvd525()?60:50;
}

/**
 * @brief TDC Set Dram Base Routine
 *
 * Set TDC Dram Base
 *
 * @param None
 * @return None
 * @warning
 * @todo
 *
 * @pre System Initial
 * @post TDC can start
 */
#if SUPPORT_S_DETECT
void vSVPresDet(void)
{

	if (_sbSVFNotify==SV_TRUE)
	{
		LOG(3,"[TVD_DBG_MSG] vSVPresDet fgSVPres=%d \n",_rTvd3dStatus.fgSVPres);
		UINT8 path=(fgIsMainTvd3d()?0:1);
		if (_rTvd3dStatus.fgSVPres)
		{
			vApiNotifySFirst(path,SV_VDO_STABLE);				
		}
		else
		{
			vApiNotifySFirst(path,SV_VDO_NOSIGNAL);
		}
		_sbSVFNotify=SV_FALSE;
	}

}
#endif


void vTvdResetChromaLock(void)
{
	vRegWriteFldMulti(CDET_05,
        			  P_Fld(SV_ON,MCKILL_EN)|
        			  P_Fld(SV_ON,MCKILL));
	vUtDelay2us(2);
	vRegWriteFldMulti(CDET_05,
        			  P_Fld(0x0,MCKILL_EN)|
        			  P_Fld(0x0,MCKILL));
}

UINT16 wTvdGetDefaultPorch(UINT8 bPath,UINT8 bPorchType)
{
    switch(bPorchType)
    {
        case SV_HPORCH_DEFAULT:
            switch (_rTvd3dStatus.bTvdMode)
            {
                case SV_CS_PAL_N:
                case SV_CS_PAL:
                case SV_CS_SECAM:
                    return 68;
                case SV_CS_PAL_M:
                case SV_CS_PAL_60:
                case SV_CS_NTSC358:
                case SV_CS_NTSC443:
                    return 58;
            }
            break;

        case SV_VPORCH_DEFAULT:
            switch(_rTvd3dStatus.bTvdMode)
            {
                case SV_CS_PAL_N:
                case SV_CS_PAL:
                case SV_CS_SECAM:
                    return 40;
                case SV_CS_PAL_M:
                case SV_CS_PAL_60:
                case SV_CS_NTSC358:
                case SV_CS_NTSC443:
                    return 32;
            }
            break;

        default:
            return 0;
    }
    return 0;
}

UINT16 wTvdGetPorch(UINT8 bPath,UINT8 bPorchType)
{
	switch (bPorchType)
	{
    	case 	SV_HPORCH_CURRENT:
            return _swTvdGetMenuAvDelay(_rTvd3dStatus.bTvdMode);
    		
    	case 	SV_HPORCH_DEFAULT:
    		return _swTvdGetDftMenuAvDelay(_rTvd3dStatus.bTvdMode);
    	case	SV_VPORCH_CURRENT:
    		switch (_rTvd3dStatus.bTvdMode)
    		{
    		case SV_CS_PAL_N:
    		case SV_CS_PAL:
    		case SV_CS_SECAM:
                    #if TVD_NEW_VPORCH
                        #if defined(CC_MT5387) || defined(CC_MT5363)
                        if(IS_5387_ECO()||IS_5363_ECO())
                        #else
                        if(1)
                        #endif
                        {
                            if(RegReadFldAlign(TG_0A, TG_SAF_ADJ)>0x40)
                                return (CCVSYNC_P_DEFAULT+0x40-RegReadFldAlign(TG_0A, TG_SAF_ADJ));
                            else
                                return (CCVSYNC_P_DEFAULT+RegReadFldAlign(TG_0A, TG_SAF_ADJ));
                        }
                        else
                    #endif
                        {
                            return RegReadFldAlign(TG_11, CCVSYNC0_P);
                        }
    		case SV_CS_PAL_M:
    		case SV_CS_PAL_60:
    		case SV_CS_NTSC358:
    		case SV_CS_NTSC443:
                    #if TVD_NEW_VPORCH
                        #if defined(CC_MT5387) || defined(CC_MT5363)
                        if(IS_5387_ECO()||IS_5363_ECO())
                        #else
                        if(1)
                        #endif
                        {
                            if(RegReadFldAlign(TG_0A, TG_SAF_ADJ)>0x40)
                                return (CCVSYNC_N_DEFAULT+0x40-RegReadFldAlign(TG_0A, TG_SAF_ADJ));
                            else
                                return (CCVSYNC_N_DEFAULT+RegReadFldAlign(TG_0A, TG_SAF_ADJ));
                        }
                        else
                    #endif
                        {
                            return RegReadFldAlign(TG_11, CCVSYNC0_N);
                        }
    		default:
    			return 0;
    		}
    	case	SV_VPORCH_DEFAULT:
    		switch (_rTvd3dStatus.bTvdMode)
    		{
    		case SV_CS_PAL_N:
    		case SV_CS_PAL:
    		case SV_CS_SECAM:
    			return CCVSYNC_P_DEFAULT+TVD_VPORCH_TUNE;
    		case SV_CS_PAL_M:
    		case SV_CS_PAL_60:
    		case SV_CS_NTSC358:
    		case SV_CS_NTSC443:
    			return CCVSYNC_N_DEFAULT+TVD_VPORCH_TUNE;
    		default:
    			return CCVSYNC_P_DEFAULT+TVD_VPORCH_TUNE;
    		}
    	case SV_VPORCH_MIN:
            //return  6;
            #if TVD_NEW_VPORCH
            if(IS_5387_ECO()||IS_5363_ECO()||1)
            {
                return 0;
            }
            #else
            {
                return 10 ;
            }
            #endif 
    	case SV_VPORCH_MAX:
            #if TVD_NEW_VPORCH
            if(IS_5387_ECO()||IS_5363_ECO()||1)
            {
                return 0x7f;
            }
            #else
            {
                return  63;
            }
            #endif 
        case SV_HPORCH_MAX:
        case SV_HPORCH_MIN:
    		return wDrvVideoPorchStd(bPath,bPorchType) +  MENU_AV_DELAY_H_OFFSET;
         
        default:    
    		return wDrvVideoPorchStd(bPath,bPorchType);
	}
}


void vTvdSetPorch(UINT8 bPorchType, UINT16 wValue)
{
	if (bPorchType==SV_HPORCH_CURRENT)
	{
		//vRegWriteFldAlign(TG_0C, (UINT8)wValue, AVDELAY_PT);
        _svTvdSetMenuAvDelay(_rTvd3dStatus.bTvdMode, wValue);

	}
	else
	{
		switch (_rTvd3dStatus.bTvdMode)
		{
		case SV_CS_PAL_N:
		case SV_CS_PAL:
		case SV_CS_SECAM:
			#if TVD_NEW_VPORCH
                     #if defined(CC_MT5387) || defined(CC_MT5363)
                     if(IS_5387_ECO()||IS_5363_ECO())
                     #else
                     if(1)
			#endif
		 	{
			 	if(wValue>=wTvdGetPorch(0,SV_VPORCH_DEFAULT))
			 	{
			 		wValue-=wTvdGetPorch(0,SV_VPORCH_DEFAULT);
			 	}
				else
				{
					wValue=wTvdGetPorch(0,SV_VPORCH_DEFAULT)-wValue;
					wValue+=0x40;
				}
				//printf("\n...............wValue is ..........%x \n",wValue);
			 	vRegWriteFldAlign(TG_0A,(UINT8)wValue,TG_SAF_ADJ);
		 	}
			else
			#endif
			{
				vRegWriteFldAlign(TG_11,wValue,CCVSYNC0_P);
				vRegWriteFldAlign(TG_11,wValue+1,CCVSYNC1_P);
			}
			break;
		case SV_CS_PAL_M:
		case SV_CS_PAL_60:
		case SV_CS_NTSC358:
		case SV_CS_NTSC443:
                    #if TVD_NEW_VPORCH
                     #if defined(CC_MT5387) || defined(CC_MT5363)
			if(IS_5387_ECO()||IS_5363_ECO())
                     #else
                     if(1)
			#endif
		 	{
			 	if(wValue>=wTvdGetPorch(0,SV_VPORCH_DEFAULT))
			 	{
			 		wValue-=wTvdGetPorch(0,SV_VPORCH_DEFAULT);
			 	}
				else
				{
					wValue=wTvdGetPorch(0,SV_VPORCH_DEFAULT)-wValue;
					wValue+=0x40;
				}
				//printf("\n...............wValue is ..........%x \n",wValue);
			 	vRegWriteFldAlign(TG_0A,(UINT8)wValue,TG_SAF_ADJ);
		 	}
			else
			#endif
			{
                            vRegWriteFldAlign(TG_11,wValue,CCVSYNC0_N);
                            vRegWriteFldAlign(TG_11,wValue,CCVSYNC1_N);
			}
			break;
		default:
			break;
		}


	}
}

/**
 * TVD query function, show status in CLI with debug level 3.
 *
 * @param void
 * @return void
 */
void vDrvTvdQueryStatus(void)
{
	Printf("VPRES(%d) Mode(%d) 525(%d) 443(%d) PHA(%d) NA(%d) NA_Lvl(%d) RF_Lvl(%d) \n",
	    fgHwTvdVPres(),
	    bHwTvdMode(),
	    fgHwTvd525(),
	    fgHwTvd443(),
	    fgHwTvdPHALT(),
	    bHwTvdNAState(),
	    bHwTvdNRLevel(),
	    _svTvdGetNRLevel()
	   );
	Printf("MV(%d,%d,%d) CoCh(%d) Scan(%d) VLock(%d) VLen(%d)\n",
	    fgHwTvdIsMacroVision(),fgHwTvdIsPSync(),fgHwTvdIsCSTRIPE(),
	    fgHwTvdCoChannel(),
	    IS_AUTOSEARCH(),
	    fgHwTvdVLock(),
	    wHwTvdAvgVlen()+1
	   );
	Printf("HS(%d) Trick(%d) NONVCR(%d) VCR(%d) CAGC(%d) CKill(%d) BLock(%d) \n",
	    fgHwTvdHeadSwitch(),
	    fgHwTvdTrick(),
	    fgHwTvdVCR(),
	    bhwVCRSetting,
	    bHwTvdCAGC(),
	    fgHwTvdCKill(),
	    fgHwTvdBLock()
	   );
}

//?? didn't used functions?
void SetTVDAGCstatus(void)
{
	//vRegWriteFldAlign(DFE_03, _rTvd3dAGCInfo.bmanualAGC, AGC2_MAGC);
	vRegWriteFldAlign(TG_00, _rTvd3dAGCInfo.minHWidth, MIN_HWIDTH);
	vRegWriteFldAlign(TG_00, _rTvd3dAGCInfo.maxHWidth, MAX_HWIDTH);
	vRegWriteFldAlign(CDET_01, _rTvd3dAGCInfo.thHigh, TH_HIGH);
	vRegWriteFldAlign(CDET_01, _rTvd3dAGCInfo.thSuper, TH_SUPER);
}

//?? didn't used functions?
void GetTVDAGCstatus(void)
{

	_rTvd3dAGCInfo.minHWidth=RegReadFldAlign(TG_00, MIN_HWIDTH);
	_rTvd3dAGCInfo.maxHWidth=RegReadFldAlign(TG_00, MAX_HWIDTH);
	_rTvd3dAGCInfo.thHigh=RegReadFldAlign(CDET_01, TH_HIGH);
	_rTvd3dAGCInfo.thSuper=RegReadFldAlign(CDET_01, TH_SUPER);
}
void vTvdSetOverSampleEnable(UINT8 bmode)
{
	LOG(3,"[TVD_DBG_MSG] vTvdSetOverSampleEnable bonoff=%d \n",bmode);
	if (bmode)
	{
		vRegWriteFldAlign(VSRC_07, 0, TVD_27M_SEL);
		vRegWriteFldAlign(SYS_00, 1, DATA_SYNC_DUAL);
	}
	else
	{
		vRegWriteFldAlign(VSRC_07, 1, TVD_27M_SEL);
		vRegWriteFldAlign(SYS_00, 0, DATA_SYNC_DUAL);
	}
}

UINT8 vTvdGetOverSampleEnable(void)
{
	return RegReadFldAlign(SYS_00,DATA_SYNC_DUAL);
}

/**
 * @brief Set Tvd3D mode detect counter
 * @param  bMDCnt: TVD Modes 
 * @retval void
 */
void vTvdSetModeDetTime(UINT8 bMDCnt)
{
	bTvdCtrl(TCTL_MCNT, TC_ENABLE, bMDCnt);
}

/**
 * @brief Get Tvd3D mode detect counter
 * @param  void
 * @retval UINT8 TVD mode detect counter
 */
UINT8 bTvdGetModeDetTime(void)
{
	return bTvdCtrl(TCTL_MCNT, TC_GETVAL, 0);
}

UINT8 bTvdCtrl(UINT8 bItem,UINT8 bCmd,UINT32 bArg) reentrant
{
	UINT8 ret= SV_SUCCESS;

	if((bItem>=TCTL_MAX)||((bCmd&TC_INIT) && _srTvdCtrl[TCTL_BP_TC_INIT].en))
		return SV_FAIL; //bypass TC_INIT

	bCmd&=(UINT8)~TC_INIT;
	
	switch(bCmd)
	{
    	case TC_DISABLE:
    		_srTvdCtrl[bItem].en=0;
    		_srTvdCtrl[bItem].val=0;
    		break;
    	case TC_ENABLE:
    		_srTvdCtrl[bItem].en=1;
    		_srTvdCtrl[bItem].val=(UINT8) bArg;
    		break;
    	case TC_APPLY:
    		bCmd=(_srTvdCtrl[bItem].en)?TC_ENABLE:TC_DISABLE;
    		ret=(_srTvdCtrl[bItem].en)?SV_SUCCESS:SV_FAIL;
    		break;
    	case TC_GETEN:
    		return _srTvdCtrl[bItem].en;
    	case TC_GETVAL:
    		return _srTvdCtrl[bItem].en?_srTvdCtrl[bItem].val:0;
    	default:	
    		return SV_FAIL;
	};

	/* dedicate control for ENABLE/DISABLE command */
	switch(bItem)
	{
    	case TCTL_TVD_RESET:
    		_srTvdCtrl[bItem].en=0;
		vTvd3dReset();
		break;
	#if 0
    	case TCTL_RESET_DEFAULT:
    		_srTvdCtrl[bItem].en=0;
    		/* TODO */
    		break;
    	case TCTL_RECONNECT:
    		_srTvdCtrl[bItem].en=0;
    		#if 0 //Need to do ??
    		if(_rTvd3dStat.bIsMain)
    			vTvd3dConnect(SV_VP_MAIN,SV_ON);
    		else if(_rTvd3dStat.bIsPip)
    			vTvd3dConnect(SV_VP_PIP,SV_ON);
    		break;
    		#endif
	#endif
    	case TCTL_INPUTTYPE:
    		if(bCmd==TC_ENABLE)
    			_srTvdCtrl[bItem].val=_rTvd3dStatus.eSourceType=(bArg<=SV_ST_SV)?bArg:bGetSignalType(_rTvd3dStat.bIsPip);
    		else
    			_rTvd3dStatus.eSourceType=bGetSignalType(_rTvd3dStat.bIsPip);
    		break;
    	case TCTL_COUNTRY:
    		if(bCmd==TC_ENABLE)
    			_srTvdCtrl[bItem].val=(bArg==COUNTRY_EU)?COUNTRY_EU:COUNTRY_US;
    		else
#ifdef COUNTRY_FROM_EEPROM
		u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;
		if ((u1TargetCountryInfo == COUNTRY_US)||
			(u1TargetCountryInfo == COUNTRY_JP)||
			(u1TargetCountryInfo == COUNTRY_TW))
		{
			_srTvdCtrl[bItem].val=COUNTRY_US;
		}
		else if(u1TargetCountryInfo == COUNTRY_EU)
		{
			_srTvdCtrl[bItem].val=COUNTRY_EU;
		}
#else
		_srTvdCtrl[bItem].val=GetTargetCountry();
#endif
    		_srTvdCtrl[bItem].en=1; //always need enable country info
    		break;
    	case TCTL_DFTMODE:
    		if(bCmd==TC_ENABLE)
    		{
    			if((_srTvdCtrl[bItem].val>=SV_CS_AUTO)||(_srTvdCtrl[bItem].val==SV_CS_UNSTABLE))
    			{	//turn off
    				_srTvdCtrl[bItem].en=0;
    				_srTvdCtrl[bItem].val=0;
    			}
    			else _svDrvTvdSetDftMode(_srTvdCtrl[bItem].val);
    		}
    		else	//reference country
    			_svDrvTvdSetDftMode((_srTvdCtrl[TCTL_COUNTRY].val==COUNTRY_EU)?SV_CS_PAL:SV_CS_NTSC358);
    		break;
    	case TCTL_MDSTBPROT:
    		if(bCmd!=TC_ENABLE)
    		{
    			vRegWriteFldAlign(CDET_05, TVD_FORCE_UPD_MODE?SV_ON:SV_OFF, FORCE_UPD_MODE);
    		}
    		break;
    	case TCTL_AGCPEDEN:
    		if(bCmd==TC_ENABLE)
    		{
			_bAgcPedEn = 1;
    		}
    		else
    		{
			_bAgcPedEn = 0;
    		}
    		break;	
    	case TCTL_BP_TC_INIT:
    	case TCTL_NTSCONLY:
    	case TCTL_BP_ISR:
    	case TCTL_BP_MODECHG:
    		break;
    	case TCTL_BP_MLOOP:
    		if(bCmd==TC_ENABLE)
    		{
			_u4BypassNptvMloop |= BYPASS_NPTV_TVD;		
    		}
    		else
    		{
			_u4BypassNptvMloop &= ~BYPASS_NPTV_TVD;		
    		}
    		break;
    	case TCTL_CKILL:
    		if(bCmd==TC_ENABLE)
    		{
    			vRegWriteFldAlign(CDET_05,1,MCKILL_EN);
    			vRegWriteFldAlign(CDET_05,_srTvdCtrl[bItem].val,MCKILL);
    		}
    		else
    		{
    			vRegWriteFldMulti(CDET_05,P_Fld(0,MCKILL_EN)|P_Fld(0,MCKILL));
    		}
    		break;
    	case TCTL_COLORSYSTEM:
    		#if 0 //Need to do ??
    		if(bCmd==TC_ENABLE)
    		{
    			fgApiVideoColorSystem(_srTvdCtrl[bItem].val);
    		}
    		else
    		{
    			fgApiVideoColorSystem(SV_CS_AUTO);
    		}
    		#endif
		    break;
    	case TCTL_SERRTYPE:
    		if(bCmd==TC_ENABLE)
    			_srTvdCtrl[bItem].val=(bArg==SV_ON)?SV_ON:SV_OFF;
    		break;
    	case TCTL_SERRCNT:
    		break;
    	case TCTL_ACODE:
    		break;
    	case TCTL_DCODE:
    		break;
    	case TCTL_CAGC:
    		break;
    	case TCTL_REG_SAVE_RESTORE:
    		break;
    	case TCTL_MCNT:
    		break;
    	default:
    	case TCTL_NOISELVL:
    	case TCTL_VCR:
    	case TCTL_COCH:
    		break;
	}
	return ret; 
}

/**
 * @brief TVD Init function
 *
 * This function only be called once when system start up
 *
 * @param None
 * @return None
 */
void vTvd3dInit(void)
{

    //KI gain
    vRegWriteFldMulti(CTG_03,
                      P_Fld(CTG_KI_GAIN0, CTG_KI0) |
                      P_Fld(CTG_KI_GAIN1, CTG_KI1) |
                      P_Fld(CTG_KI_GAIN2, CTG_KI2));

    vRegWriteFldMulti(CTG_03,
                      P_Fld(CTG_KI_GAIN3, CTG_KI3) |
                      P_Fld(CTG_KI_GAIN4, CTG_KI4) |
                      P_Fld(CTG_KI_GAIN5, CTG_KI5));

	//KP gain
	vRegWriteFldMulti(CTG_04,
        P_Fld(CTG_KP_GAIN0,CTG_KP0)|
        P_Fld(CTG_KP_GAIN1,CTG_KP1)|
        P_Fld(CTG_KP_GAIN2,CTG_KP2));

	vRegWriteFldMulti(CTG_04,
        P_Fld(CTG_KP_GAIN3,CTG_KP3)|
        P_Fld(CTG_KP_GAIN4,CTG_KP4)|
        P_Fld(CTG_KP_GAIN5,CTG_KP5));
#ifdef COUNTRY_FROM_EEPROM
	u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;
	if ((u1TargetCountryInfo == COUNTRY_US)||
		(u1TargetCountryInfo == COUNTRY_JP)||
		(u1TargetCountryInfo == COUNTRY_TW))
	{
		bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,COUNTRY_US);/* inital country*/
	}
	else if(u1TargetCountryInfo == COUNTRY_EU)
	{
		bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,COUNTRY_EU);/* inital country*/
	}
#else
	bTvdCtrl(TCTL_COUNTRY,TC_INIT|TC_ENABLE,GetTargetCountry());/* inital country*/
#endif

	if (bTvdCtrl(TCTL_NTSCONLY,TC_GETVAL,0)==1)
	{
		_rTvd3dStatus.fgIs525 = 0x1;
    }
	else
	{
		if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
		{
		    _rTvd3dStatus.fgIs525 = 0x00;
		}
		else
		{
		    _rTvd3dStatus.fgIs525 = 0x01;
		}
    }
	
    #if TVD_FRS_FOR_NONSTDSIGNAL
    _svTvd3dRstNSTStatus(_rTvd3dStatus.fgIs525, &_rTvd3dNSTDStatus);     
    #endif

	if(bTvdCtrl(TCTL_DFTMODE,TC_GETEN,0))
	{
		_svDrvTvdSetDftMode(bTvdCtrl(TCTL_DFTMODE,TC_GETVAL,0));
	}
	else
	{
		if (bTvdCtrl(TCTL_COUNTRY,TC_GETVAL,0)== COUNTRY_EU)
		{
			_svDrvTvdSetDftMode(SV_CS_PAL);
		}
		else
		{
			_svDrvTvdSetDftMode(SV_CS_NTSC358);
		}
	}
}


#if TVD_CLI_SUPPORT
void vTvdSetCLINfy(TVD_PFN_CLI_NFY pfnCliNfy)
{
    _pfnTVDCLINotify = pfnCliNfy;
}

void vTVDAVDelay(UINT8 bDelay)        // For CLI
{
    _svTvdSetMenuAvDelay(_rTvd3dStatus.bTvdMode, bDelay);
}

void vTvd3dSetFixAgc(UINT8 bOnOff)
{
	_sbIsTVDFixAgc = bOnOff;
	if(bOnOff)
	{            
            //MT5387 FPGA
            //TODO
            /*
            9 bit
            [31]INTERP_DAGC_SEL
            [30]TGYI_DAGC_SEL
            [29]TDC_DAGC_SEL
            [28]SVF_DAGC_SEL
            [27]CDET_DAGC_SEL
            [26]MDET_DAGC_SEL
            [25]VBI_DAGC_SEL
            [24]TG_DAGC_SEL 0: Fix-gained;1: Digital-gained 
            [23]CTG_DAGC_SEL 0: Fix-gained;1: Digital-gained 
            */
            #if TVD_USE_NEW_DAGC
                vTvdSetAGCMode(E_MENU_AGAIN);
                vRegWriteFldAlign(DFE_03, SV_ON, MANUAL_DAGC_EN);//MT5387 FPGA
                vRegWriteFldAlign(DFE_18, DFE_FIXAGC_DAGCSEL_LowNA, FIXAGC_DAGCSEL);
            #else
                vTvdSetAGCMode(E_MENU_GAIN);
            #endif
	}
	else
	{
		vTvdSetAGCMode(E_AUTO_GAIN);			
		vRegWriteFldAlign(DFE_03, SV_OFF, MANUAL_DAGC_EN);//MT5387 FPGA
		vRegWriteFldAlign(DFE_18, DFE_AUTOAGC_DAGCSEL, FIXAGC_DAGCSEL);
	}
}
void vTVD3dAGCPedEn(UINT8 bOnOff)
{
	if (bOnOff)
	{
		_bAgcPedEn = 1;
		switch (_rTvd3dStatus.bTvdMode)
		{
		case SV_CS_PAL_M:
		case SV_CS_PAL_N:
		case SV_CS_NTSC358:
			vRegWriteFldAlign(CTG_07, _wTargetBlk+DFE_BLANK_OFFSET_NTSCJ, BLKLVL);
            _svTvdSetCAGCBound4Data(NTSC_MID_B, NTSC_MID_A, NTSC_TGT_B, NTSC_TGT_A);		
			break;
		default:
			break;
		}
	}
	else
	{
		_bAgcPedEn = 0;
		switch (_rTvd3dStatus.bTvdMode)
		{
		case SV_CS_PAL_M:
		case SV_CS_PAL_N:
		case SV_CS_NTSC358:
			vRegWriteFldAlign(CTG_07, _wTargetBlk+DFE_BLANK_OFFSET_NTSC, BLKLVL);
        	_svTvdSetCAGCBound4Data(NTSC_J_MID_B, NTSC_J_MID_A, NTSC_J_TGT_B, NTSC_J_TGT_A);		
			break;
		default:
			break;
		}
	}
}

/**
 * @brief TVDecoder pattern generator 
 *
 * @param bOnOff  0: disable; 1: enable
 * @return void.
 *
 */
void vTvd3dSetPatGen(UINT8 bOnOff)
{
	if(bOnOff)
	{
		vRegWriteFldAlign(TG_0B, SV_ON, TVD_PATGEN_EN);
	}
	else
	{
		vRegWriteFldAlign(TG_0B, SV_OFF, TVD_PATGEN_EN);
	}
}
#endif
