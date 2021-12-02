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
#include "drv_ycproc.h"
#include "drv_di.h"

#define NR_REGTBL_END 0xffff

#define NR_ENABLE_SYNC_REGEN 1

#define NR_DISABLE_PIXEL_REP_WHEN_BLANKING_IS_SAMLL 0

#define NR_BASIC_MODE_ENABLED 1

#define NR_ENABLE_ROUNDING_TOGGLE_ON_OFF 0

#define NR_ADAPTIVE_DEBA_ENABLED 1

//Adaptive NR on/off
#ifdef THE_3RD_PARTY_SW_SUPPORT
#define ANR_ENABLE 1
#else
#define ANR_ENABLE 0
#endif
#define ADB_ENABLE 1
#define ANM_ENABLE 1    //exclusive with ANR_ENABLE

extern UINT32 wDrvVideoGetPreScaleWidth(UINT8 bPath);


#if ANM_ENABLE

typedef struct
{
    UINT8 bDefEn;//Discard Edge Detection

    UINT8 bDefTh; //This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL0;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL1;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL2;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL3;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL4;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL5;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL6;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously
    UINT8 bTBL7;// This Offset will shift Color/Chroma/Default/StillEdge tbl simultaneously


    UINT8 bMoEdgeTh; 
    UINT8 bMoEdgeTBL0;
    UINT8 bMoEdgeTBL1;
    UINT8 bMoEdgeTBL2;
    UINT8 bMoEdgeTBL3;
    UINT8 bMoEdgeTBL4;
    UINT8 bMoEdgeTBL5;
    UINT8 bMoEdgeTBL6;
    UINT8 bMoEdgeTBL7;

    UINT8 bHSharpCoring;    
    
    UINT8 bLCSharpL;
    UINT8 bLCSharpM;
    UINT8 bLCSharpH;

    UINT8 bReducedSat;
}ANM_NOISE_TH_TBL;


#define ANM_REDUCE_SAT_STEP 8 //Saturation Reduced Step

enum{	
            NM_INIT,	
            NM_CHK_PARAMETER,	
            NM_LEVEL_DONE
         };

enum{
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

void vDrvANRUpdateStatus(UINT8 bLevel);

UINT32 fgDrvGetAutoNROnOff(void);
void vDrvSetAutoNROnOff(UINT8 bOnOff);
void vDrvANRSetSWRegister(void);

EXTERN void vDrvDynamicNR(UINT32 NRLevel);
EXTERN UINT8 vDrvNMLevel(void);
EXTERN UINT8 DynamicNRCurLevel, DynamicNRPreLevel, _bNMChkState;
EXTERN BOOL NMEnable, fgIsAutoNREnabled, fgPreAutoNREnabled;
EXTERN UINT8 bReducedSaturation;

#endif  //ANM_ENABLE

/* for 5387 */

typedef struct
{
	UINT16 u2Addr;
	UINT32 u4Value;
	UINT32 u4Mask;
} NR_REGTBL_T;

typedef struct
{
    //to keep PSCAN PIP base address in 3d ink debug mode
    UINT32 u4AddrYMsb;
    UINT32 u4AddrYLsb;
    UINT32 u4AddrUMsb;
    UINT32 u4AddrULsb;
    UINT32 u4AddrVMsb;
    UINT32 u4AddrVLsb;
    UINT32 u4AddrLoLimit;
    UINT32 u4AddrHiLimit;    
}NR_3DINK_DEBUG_T;

typedef struct
{
    UINT8 bNRBasicModeEnabled;
    UINT8 bSmoothAreaStrength;
    UINT8 bMessAreaStrength;
    UINT8 bEdgeAreaStrength;
    UINT8 bMosAreaStrength;
    UINT8 bBlendLevel;
}NR_2D_BASIC_MODE_INFO_T;

typedef struct
{
	UINT8 bNRPath;          //NR Path Main/PIP
	UINT8 bNRBypass;	    //NR Bypass
	UINT8 bForceNRBypass;   //Force NR bypass
	UINT8 b2DNROnOff;       //2D NR On/ Off	
	E_NR_DMODE bSrm3DNRMode;//3D NR mode by SRM/ DI
    UINT8 b3DNROnOff;       //3D NR On/ Off
	UINT8 b3DNRUIOnOff;     //3D NR UI Setting On/ Off    
	UINT8 bDeblockingOnOff; //Deblocking On/Off
	UINT8 bMosquitoOnOff;   //Mosquito On/Off	
	UINT8 b2DNRUI;
	UINT8 bMPEGNRUI;   
	UINT16 u2NRWidth;       //NR input width
	UINT16 u2NRHeight;      //NR input height
	UINT16 u2XOffset;
	UINT16 u2YOffset;
	//for cli debug
	UINT8 bNRDebugPath;
	NR_3DINK_DEBUG_T NR3DInkInfo;
	NR_2D_BASIC_MODE_INFO_T NR2DBasicModeInfo;
} NR_PRM_T;

typedef enum 
{
	NR_CHROMA_LINE_AVERAGE_MODE_OFF,
	NR_CHROMA_LINE_AVERAGE_MODE_UP,
	NR_CHROMA_LINE_AVERAGE_MODE_DOWN,
	NR_CHROMA_LINE_AVERAGE_MODE_121,
}NR_CHROMA_LINE_AVERAGE_MODE;

typedef enum
{
    NR_SCAN_MODE_PROGRASSIVE,
    NR_SCAN_MODE_INTERLACE
}NR_SCAN_MODE;

typedef enum
{
    NR_YUV_MODE_Y_ONLY,
    NR_YUV_MODE_420,
    NR_YUV_MODE_422,
    NR_YUV_MODE_444
}NR_YUV_MODE;

typedef enum
{
    NR_BIT_SEL_8BIT,
    NR_BIT_SEL_10BIT,
}NR_BIT_SEL;

#if NR_ADAPTIVE_DEBA_ENABLED
typedef enum
{
    NR_ADAPTIVE_DEBA_STATE_INIT,
    NR_ADAPTIVE_DEBA_STATE_RESET,
    NR_ADAPTIVE_DEBA_STATE_CONFIG0,
    NR_ADAPTIVE_DEBA_STATE_CONFIG1,
    NR_ADAPTIVE_DEBA_STATE_COMPARE,
    NR_ADAPTIVE_DEBA_STATE_MONITOR
}NR_ADAPTIVE_DEBA_STATE;
#endif

// functions for DI/NR interface
void vDrvNRSetIIRPath(UINT8 u1VdpId);
void vDrvNRSetReadEnable(UINT8 u1VdpId, UINT8 bOnOff);
void vDrvNRSetScanMode(UINT8 u1VdpId, NR_SCAN_MODE scanMode);
void vDrvNRSetYUVMode(UINT8 u1VdpId, NR_YUV_MODE nrYuvMode);
void vDrvNRSetBitSel(UINT8 u1VdpId, NR_BIT_SEL nrBitSel);
// functions for setting Chroma line avg mode for TVD
void vDrvNRSetChromaLineAvgMode(UINT8 bPath, NR_CHROMA_LINE_AVERAGE_MODE bMode);
//function to load table
void vDrvNRLoadRegTbl(const NR_REGTBL_T* prRegTbl);
// functions for Get/Set NR path
UINT8 bDrvNRGetCurrentPath(void);
// functions for NR flow
void vDrvNRInit(void);
void vDrvNRQtyInit(void);
void vDrvNRLoadQualityTable(void);
void vDrvNRSetSystem(void);
void vDrvNRProc(void);
UINT8 bDrvNRDeterminePath(void);
void vDrvNRBypass(UINT8 bOnOff);
UINT8 bDrvNRIsNRBypassed(void);
void vDrvNRSet3DNRUIOnOff(UINT8 bOnOff);
void vDrvNRSet2DNROnOff(UINT8 bOnOff);
void vDrvNRSet3DNROnOff(UINT8 bOnOff);
void vDrvNRSetDeblockingOnOff(UINT8 bOnOff);
void vDrvNRSetMosquitoOnOff(UINT8 bOnOff);
void vDrv3DNRCliOnOff(UINT8 bMode);
void vDrv2DNRCliOnOff(UINT8 bMode);
void vDrvNRSetNRDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode);
//function for setting NR quality table
void vDrv2DNRParam(UINT8 bCurUI);
void vDrvMPEGNRParam(UINT8 bCurUI);
void vDrv3DNRParam(void);
//function for NR demo mode (slice mode)
void vDrv2DNRDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
void vDrv3DNRDemo(UINT8 bOnOff, Region bRegion);
void vDrvNRSrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode);

//for cli
void DrvNRSetManualWeight(UINT8 bOnOff,UINT8 bWeight);
void vDrvNRSetDebugPath(UINT8 u1VdpId);
void vDrvNRSetFreeze(UINT8 bOnOff);
void vDrvNRSet3DINKDebugMode(UINT8 bOnOff);
void vDrvNRGetNRStatus(void);
void vDrvNRSet2DLevel(UINT8 smoothLevel,UINT8 messLevel,UINT8 edgeLevel,UINT8 mosquitoLevel,UINT8 blendLevel);
void vDrvNRSetForceBypass(UINT8 bOnOff);

//Auto DeBlocking
void vDrvAutoDB(void);

// when frame still condition is true, disable 2DNR color condition detect.
void vDrvColorFrmStChg(void);

//NR basic mode
#if NR_BASIC_MODE_ENABLED
void vUpdateNRBasicModeRegister(void);
#endif

#if NR_ENABLE_ROUNDING_TOGGLE_ON_OFF
void vDrvNRToggleRoundingOnOff()
#endif

#if NR_ADAPTIVE_DEBA_ENABLED
void vDrvAdaptiveDebaSetState(NR_ADAPTIVE_DEBA_STATE state);
NR_ADAPTIVE_DEBA_STATE vDrvAdaptiveDebaGetState(void);
UINT8 bDrvAdaptiveDebaGetVLineCount(void);
void vDrvAdaptiveDebaSetHDelay(UINT8 delay);
UINT8 bDrvAdaptiveDebaGetHDelay(void);
void vDrvAdaptiveDebaCheckState(void);
#endif

#endif //_DRV_NR_H_
