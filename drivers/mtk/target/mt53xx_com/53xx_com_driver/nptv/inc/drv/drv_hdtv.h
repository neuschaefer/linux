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
 * $RCSfile: drv_hdtv.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_HDTV_H_
#define _DRV_HDTV_H_

#include "general.h"
#include "hw_vdoin.h"
#include "hw_hdtv.h"

#define HDTV_VOut_Offset		10

extern UINT8   _bHdtvTiming;
extern UINT8   _IsHdtvDetectDone;
extern UINT8   _bUnLockCnt;
//for not support blue mute 
extern UINT8   _bHdtvTimerCnt ;
extern UINT8   _bHdtvNotSupCnt;
extern UINT8   _bHdtvBlueMuteFlg;
extern UINT8 _bHsyncWidth;

extern void vHdtvConnect(UINT8 bchannel, UINT8 fgIsOn) ;
extern UINT16 wHdtvInputWidth(void) ;
extern UINT8 bHdtvInputWidthOverSample(UINT8 bMode);
extern UINT16 wHdtvInputHeight(void) ;
extern UINT8 bHdtvRefreshRate(void) ;
extern UINT8 bHdtvInterlace(void) ;
//extern UINT16 wHdtvMDDIVPorch(UINT8 bPath);
extern UINT16 wHdtvVPorch(UINT8 bPath) ;
extern UINT16 wHdtvHPorch(UINT8 bPath) ;
extern UINT8 bHdtvSigStatus(void) ;
extern void vHdtvISR(void) ;
extern void vHdtvHwInit(void);

extern void vHdtvModeDetect(void) ;
extern void vHdtvChkModeChange(void) ;
extern void vHdtvSetInput(UINT16 wstart, UINT16 wwidth) ;
extern void vHdtvClampWin(UINT16 wclamp_start, UINT16 wclamp_end) ;
extern void vHdtvBlankStart(UINT16 wblank_start) ;
extern void vHdtvReset(void) ;
extern void vHdtvSetInputCapature(UINT8 bmode, UINT8 bIsHdtv) ;
extern UINT16 wHdtvDEInputWidth(void) ;

typedef struct RHDTVNSTDStatus
{
	UINT8 fgIsNSTD;
	UINT8 bRefreshRate;
	UINT16 wVTotal;
	UINT16 wHTotal;
	INT16 wVTotalDiff;
} RHDTVNSTDStatus;

extern void vHDTVGetNSTDStatus(RHDTVNSTDStatus* pHDTVNSTDStatus);
extern RHDTVNSTDStatus _rHDTV_NSTDStatus;


#if 0
/* for cust */
extern UINT16 vHdtv_CustPos_GetHPos(void);
extern UINT16 vHdtv_CustPos_GetVPos(void);
extern UINT8 vHdtv_CustPos_SetHPos(UINT8 path, UINT16 wValue);
extern UINT8 vHdtv_CustPos_SetVPos(UINT8 path, UINT16 wValue);
extern UINT16 vHdtv_CustPos_HPosMax(void);
extern UINT16 vHdtv_CustPos_HPosMin(void);
extern UINT16 vHdtv_CustPos_VPosMax(void);
extern UINT16 vHdtv_CustPos_VPosMin(void);
extern void vHdtv_CustPos_Execute(void);
extern void vHdtv_CustPos_Update(void);
extern void vHdtv_CustPos_EEPReset(void);
extern void vHdtv_CustPos_Init(void);
extern void vHdtv_CustPos_Exit(void);
#endif


/* for CLI */
void vHdtvSwReset(void);
void vHdtvStatus(void);

#define vHdtvCenSel(fgswitch) vRegWriteFldAlign(HDTV_00,fgswitch,HDTV_CEN_SEL)
#define vHdtvRGBFormatOn(fgswitch) vRegWriteFldAlign(HDTV_00,fgswitch,HDTV_RGB)
#define vHdtvAGCSet(blevel)	vRegWriteFldAlign(HDTV_04,blevel,HDTV_AGC)
#define bHdtvCheckCenEnable()	RegReadFldAlign(HDTV_00,HDTV_CEN_SEL)

void vHdtvSetOversampleForSD(UINT8 bmode);
UINT8 vHdtvGetOversampleForSD(void);


/* HDTV options ------------------------------*/
extern UINT8 bHdtvOpt01_MDmute0; //for MD , waiting lock mute
extern UINT8 bHdtvOpt01_MDmute1; //for MD , after lock stable
/* Opt03 leave for sync slicer*/
extern UINT8 bHdtvOpt04_AutoPhase;//for auto phase
//extern UINT8 bHdtvOpt05_CustPos;//for save each timing custom position in EEPROM
extern UINT8 bHdtvOpt05_AdaptiveSlicer; //Enable adaptive slicer

#endif
