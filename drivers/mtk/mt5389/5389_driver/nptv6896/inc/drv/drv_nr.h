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
 * $RCSfile: drv_nr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file NR header file
 *
 * @author mf_tien@mtk.com.tw
 */

#ifndef _DRV_NR_H_
#define _DRV_NR_H_

#include "typedef.h"

#define NR_R(u2Addr, u4Fld) RegReadFldAlign((u2Addr), (u4Fld))

#define NR_R4B(u2Addr) u4RegRead4B((u2Addr))

#define NR_W(u2Addr, u4Value, u4Fld) \
    vRegWriteFldAlign((u2Addr), (u4Value), (u4Fld))

#define NR_WM(u2Addr, list) \
    vRegWriteFldMulti((u2Addr), (list))

#define NR_WMSK(u2Addr, u4Value, u4Msk) \
    vRegWrite4BMsk((u2Addr), (u4Value), (u4Msk))

#define _SWR_4B(addr) u4IO32Read4B(addr)
#define _SWR(addr, fld) IO32ReadFldAlign((addr), (fld))
#define _SWW_4B(addr, val) vIO32Write4B((addr), (val))
#define _SWW(addr,val,fld) vIO32WriteFldAlign((addr), (val), (fld))
#define _SWWM(addr,list) vIO32WriteFldMulti((addr),(list))

#define NR_VER_ECO          (1<<0)
#define NR_SUPPORT_6BIT_NL  (1<<0)
#define NR_SUPPORT_STRENTH  (1<<1)
#define NR_SUPPORT_BKMTR    (1<<1)

#define NR_NM_SAD_HIST      (1)
#define NR_NM_TDF_BASE      (2)

enum
{
    NM_LEVEL1_0,
    NM_LEVEL1_1,
    NM_LEVEL1_2,
    NM_LEVEL1_3,
    NM_LEVEL2,
    NM_LEVEL3,
    NM_LEVEL4,
    NM_LEVEL5,
    NM_LEVEL6,
    NM_LEVEL7,
    NM_LEVEL8,
    NM_INVALIDLEVEL
};

typedef enum
{
    E_NR_FB_MANR = 0,
    E_NR_FB_MCNR = 1
} E_NR_FB_TYPE;

typedef enum
{
    MCNR_MODE_OFF = 0, 
    MCNR_MODE_MC  = 1,
    MCNR_MODE_MV  = 2,
    MCNR_MODE_NM  = 3,
    MCNR_MODE_MA  = 4
} E_3DNR_MODE;

typedef enum
{
    NXNR_MODE_OFF = 0,
    NXNR_MODE_ON = 1
} E_2DNR_MODE;

typedef struct
{
	UINT16 u2Addr;
	UINT32 u4Value;
	UINT32 u4Mask;
} NR_REGTBL_T;

typedef struct
{    
    E_3DNR_MODE eMCNRMode;
    E_2DNR_MODE eNXNRMode;
    UINT8 u1Ver;
    UINT8 u1Timing;
    UINT8 u1ParamUpdate;
    UINT8 u1NoiseMeterEn;
    UINT8 u1Interlace;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT32 u4ConflictCnt;
    UINT32 u4TolBlks;
} NR_PARAM_T;

typedef struct
{
    UINT8 u1ExtNLMode;
    UINT8 u1SlopLmtL;
    UINT8 u1SlopLmtM;
    UINT8 u1SlopLmtH;
    UINT8 u1AsymmetryTolrnc;
    UINT8 u1FatTailRto;
    UINT8 u1FatTailRto2;
    UINT8 u1RprsntPeakRto;
    UINT8 u1RprsntTolRto;
    UINT8 u1Fb0Thd;
    UINT8 u1CleanRto;
    UINT8 u1Lvl1Rto;
    UINT8 u1LowLvlThd;
    UINT8 u1FltCoefCur;
    UINT8 u1FbDeclineIntrvl;
    UINT8 u1DefaultNoiseLvl;
    UINT8 u1SADHistIIR;
    UINT8 u1SADHistSubSample;
} NR_FLFB_PARAM_T;

typedef struct
{
    UINT8 u1PeakBin;
    UINT8 u1PeakBinMo;
    UINT8 u1Centroid;
    UINT8 u1RTrough;
    UINT8 u1Terminate;
    UINT8 u1Mono;
    UINT8 u1SteepSlope;
    UINT8 u1SlowSlope;
    UINT8 u1Symmetry;
    UINT8 u1FatTail;
    UINT8 u1Rprsnt;
    UINT8 u1StillTex;
    UINT8 u1StillScene;
    UINT8 u1StillNoisyScene;
    UINT8 u1ZoomTex;
    UINT8 u1HighZero;
    UINT8 u1NotLowZero;
    UINT8 u1Lvl0;
    UINT8 u1Lvl1;
    UINT8 u1MvApprove;
    UINT8 u1LowSec;
    UINT8 u1LowOne;
    UINT8 u1FbkSta;
    UINT8 u1FbkStep;
    UINT8 u1NoiseLevel;
    UINT8 u1ValidNoiseLevel;
    UINT8 u1AntiNLGain;
    UINT8 u1FbCnt;
    UINT8 u1GmvX;
    UINT8 u1GmvY;
    UINT8 u1GmvValid;
    UINT8 u1GmvRatio;
    UINT8 u1CleanDiffLvl;
    UINT8 u1FlfbStatus;
    UINT8 u1CompositeStatus;
    UINT8 u1ScnChg;
    UINT8 u1ScnChgCnt;
    UINT8 u1NLScnChgCnt;
    UINT8 u1FavorCSState;
    UINT8 u1ZoomTexCondInk;
    UINT8 u1HistInvalidRto;
    UINT8 u1TdfNL;
    UINT8 u1Pause;
    UINT16 u2PeakBinRawVal;
    UINT16 u2Nzc;
    UINT16 u2SnrThm;
    UINT16 u2HistMaxCnt; 
    UINT32 u4HistDiffAcc;
    UINT32 u4HomoSum;
    UINT32 u4PreHomoSum;
    UINT32 u4HomoSumDiffTh;
    UINT32 u4StaDetail;
    UINT32 u4EdgeCnt;
    UINT32 u4HistMoment;
    UINT32 u4TotalHistCnt;
    UINT32 u4TmprDiffSum;
    UINT32 u4TmprDiffCnt;
    UINT32 u4MoPixCnt;
    UINT32 u4PreMoPixCnt;
} NR_FLFB_STA_T;

EXTERN void vDrvMCNRResetNoiseLevel(void);
EXTERN void vDrvNRPQInit(void);
EXTERN void vDrvNRISR(void);
EXTERN void vDrvNRVBlankProc(void);

E_3DNR_MODE eDrvNRGet3DNRMode(void);
void vDrvNRSet3DNRMode(E_3DNR_MODE e3DMode);
void vDrvNRSet2DNRMode(E_2DNR_MODE e2DMode);
void vDrvNRSetChromaUpSample(BOOL fgOnOff);
void vDrvNRSetQuality(UINT8 u1Interlace, UINT16 u2Width, UINT16 u2Height);

void vDrvNRSet3DNRParam(void);
void vDrvNRSetSNRParam(void);
void vDrvNRSetMpegNRParam(void);
void vDrvNRSet2DNRBNROnOff(UINT8 u1OnOff);
void vDrvNRSet2DNRMNROnOff(UINT8 u1OnOff);
void vDrvNRSet2DNRSNROnOff(UINT8 u1OnOff);
void vDrvNRSet2DNRAuto(UINT8 u1OnOff);
void vDrvNRSet3DNROnOff(UINT8 u1OnOff);
void vDrvNRSet3DNRStrength(UINT8 u1Strength);
void vDrvNRSet3DNRAuto(UINT8 u1OnOff);
void vDrvNRSet3DNRChromaPath(UINT8 u1OnOff);
void vDrvNRSetBitRes(UINT8 u18bit);

void vDrvNRSet2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
void vDrvNRSet3DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);

void vDrvNRSetForceBypass(UINT8 u1OnOff);
void vDrvNRGetNRStatus(void);
UINT8 u1DrvNRGetBlockMeterSupport(void);


void vDrvNRInit(void);
void vDrvNRProc(void);

//=================== To be removed===============================

typedef enum 
{
	NR_CHROMA_LINE_AVERAGE_MODE_OFF,
	NR_CHROMA_LINE_AVERAGE_MODE_DYN,
	NR_CHROMA_LINE_AVERAGE_MODE_121,
}NR_CHROMA_LINE_AVERAGE_MODE;

void vDrvNRSetChromaLineAvgMode(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);
void vDrvNRSetInk(UINT8 u1Mode, UINT8 u1Type);
void vDrvNRSetChromaLineAvgModeReg(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);

#endif //_DRV_NR_H_

