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
 * $RCSfile: drv_auto.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


#ifndef _DRV_AUTO_H_
#define _DRV_AUTO_H_

/******************************************************************************
 * Header Files
 *****************************************************************************/
#include "typedef.h"

/******************************************************************************
 * Macro, Type Definition, and Variable Declaratoin
 *****************************************************************************/
#define wDrvVGAGetHStart()	RegReadFldAlign(HDTV_01, HDTV_AV_START)
#define vDrvVGASetHStart(wHStart) vRegWriteFldAlign(HDTV_01, wHStart, HDTV_AV_START)
#define vDrvVGASetPhsMix(bMethod)	vRegWriteFldAlign(ASYNC_17, bMethod, AS_C_PSNE_SRC_SEL)	
// hw auto macro define
#define bDrvHwAutoClkRdy()	RegReadFldAlign(STA_SYNC0_0E, AS_AUTO_CLK_RDY)
#define dDrvAutoGetPhsMaxMinDiff()	RegReadFldAlign(STA_SYNC0_0E, AS_PHS_MAXMIN_DIFF_S)
#define bDrvHwAutoPhsRdy() 	RegReadFldAlign(STA_SYNC0_0C, AS_AUTO_PHASE_RDY)
#define bDrvAutoGetPhsGood()	RegReadFldAlign(STA_SYNC0_0C, AS_PHASE_GOOD)

enum
{
	BD_RED,
	BD_GREEN,		
	BD_BLUE,
	BD_MAX
};

enum
{
	RED_BD_SEL = 0x1,
	GREEN_BD_SEL = 0x2,		
	BLUE_BD_SEL = 0x4,
	ALL_BD_SEL = 0x7
};


enum
{
	Amode,
	Bmode
};

#define TOP_THRE 0x50 // orig. 0x20 ,gbsh 060901 
#define PSNE_THRE1	0x70 //orig. 0x20,gbsh 060901
#define PSNE_THRE2	0x20 //orig. 5
#define SUPPORT_MIX_PHASE_STA	1

/** 
 * @brief exprot for vga auto 
 */
extern UINT8 bDrvVGAGetPhase(void);	
extern void vDrvVGASetPhase(UINT8 bVal)  ;
extern void vDrvVGASetBDDataTh(UINT8 bTh) ;
extern void vDrvVGASetBDCha(UINT8 bCha);
extern UINT16 wDrvVGAGetLeftBound(void);
extern UINT16 wDrvVGAGetTopBound(void);
extern UINT16 wDrvVGAGetRightBound(void);
extern UINT16 wDrvVGAGetBottomBound(void);
extern UINT32 dVGAGetAllDiffValue(void);
extern UINT32 dHDTVGetAllDiffValue(void); //YPbPr Auto Phase 2006/11/07
extern void vVGAPhaseModeSet(UINT8 bVal);
extern void vDrvVGASetTopThr(UINT8 bVal);
extern void vDrvVGASetPsneThr(UINT8 bThr1, UINT8 bThr2);
extern void vVgaHwAutoPhaseEnable(UINT8 bEnable);
extern void vVgaHwAutoPhaseReset(void);
extern void vVgaHwAutoClkEnable(UINT8 bEnable);
extern void vDrvVGASetPhase_Simple(UINT8 bVal);


/* for CLI or MW*/
UINT8 u1DrvVGAGetPhase(void);
UINT16 wDrvVGAGetClock(void); //127 offset
UINT16 wDrvVGAHPositionMax(void);
UINT16 wDrvVGAHPositionMin(void);
UINT16 wDrvVGAVPositionMax(void);
UINT16 wDrvVGAVPositionMin(void);
UINT16 wDrvVGAGetHPosition(void);
UINT16 wDrvVGAGetVPosition(void);
UINT8 wDrvVgaGetClockMax(void);

//---------------------------------------

/** 
 * @brief exprot for dvi auto 
 */
extern UINT32 dDVIGetAllDiffValue(void);
extern void vDVIPhaseModeSet(UINT8 bMode);
extern void vDVISetTopThr(UINT8 bVal);
extern void vDrvDVISetBDDataTh(UINT8 bTh);
extern void vDrvDVISetBDCha(UINT8 bCha);
extern UINT16 wDrvDVIGetLeftBound(void);
extern UINT16 wDrvDVIGetRightBound(void);
extern UINT16 wDrvDVIGetTopBound(void);
extern UINT16 wDrvDVIGetBottomBound(void);
//---------------------------------------

/** 
 * @brief exprot for drv_dvi.c 
 */
extern void vDrvDVIMeasureDELen(void) ;
extern UINT16 wDrvDVIGetWidth(void) ;
extern UINT16 wDrvDVIGetHeight(void) ;
//---------------------------------------

#endif
