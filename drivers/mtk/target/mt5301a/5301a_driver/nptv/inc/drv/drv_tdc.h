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
 * $RCSfile: drv_tdc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_TDC_H_
#define _DRV_TDC_H_

#define ACTIVE_WIN_NTSC_X_START		(UINT32)0x090
#define ACTIVE_WIN_NTSC_X_LENGTH	(UINT32)0x2F3
#define ACTIVE_WIN_NTSC_Y_START		(UINT32)0x014
#define ACTIVE_WIN_NTSC_Y_LENGTH	(UINT32)0x100
#define ACTIVE_WIN_NTSC_DEMO_START	(UINT32)0x1F0 // For right enable
#define ACTIVE_WIN_NTSC_DEMO_LENGTH	(UINT32)0x173 // For left enable

#define ACTIVE_WIN_PAL_X_START		(UINT32)0x098
#define ACTIVE_WIN_PAL_X_LENGTH		(UINT32)0x3A0
#define ACTIVE_WIN_PAL_Y_START		(UINT32)0x017
#define ACTIVE_WIN_PAL_Y_LENGTH		(UINT32)0x135
#define ACTIVE_WIN_PAL_DEMO_START	(UINT32)0x1F8 // For right enable
#define ACTIVE_WIN_PAL_DEMO_LENGTH	(UINT32)0x1D4 // For left enable

#define ACTIVE_WIN_PAL_M_X_START	(UINT32)0x090
#define ACTIVE_WIN_PAL_M_X_LENGTH	(UINT32)0x2F0
#define ACTIVE_WIN_PAL_M_Y_START	(UINT32)0x011
#define ACTIVE_WIN_PAL_M_Y_LENGTH	(UINT32)0x100

#define ACTIVE_WIN_PAL_N_X_START	(UINT32)0x09E
#define ACTIVE_WIN_PAL_N_X_LENGTH	(UINT32)0x2E8
#define ACTIVE_WIN_PAL_N_Y_START	(UINT32)0x018
#define ACTIVE_WIN_PAL_N_Y_LENGTH	(UINT32)0x134

#define DRAM_CMP_NTSC_HIGHBW	0x1D
#define DRAM_CMP_NTSC_LOWBW		0x17
#define DRAM_CMP_PAL_HIGHBW		0x24
#define DRAM_CMP_PAL_LOWBW		0x19

#define ADAPTIVE_2D_COMB_SHARP  1
#define ADAPTIVE_APL_DETECT	0
//#define ADAPTIVE_MB_PULL2D		1
//#define ADAPTIVE_LOWAPL_SMALLCHROMAOFF	1
#define ADAPTIVE_POST_NOTCH_FILTER	1
//#define ADAPTIVE_CZP_LEVEL		1
//#define ADAPTIVE_WEAK_SIGNAL	1
#define ADAPTIVE_GOH            0
//#define ADAPTIVE_NTSC3DYGAIN	1
#define ADAPTIVE_CCS_ON_BW      1
//#define ADAPTIVE_SLOW_MOTION    0   // To detect leopard spot pattern.
#define ADAPTIVE_MOVING_SINE 1

#if ADAPTIVE_2D_COMB_SHARP
	#define MOTION_SMALL	0x08000L
	#define MOTION_BIG		0x40000L
	#define SHARP_WEAK		0x40
	#define SHARP_STRONG	0x58
#endif // #if ADAPTIVE_2D_COMB_SHARP

#if ADAPTIVE_APL_DETECT
    #define APL_STILLFRAME	0x50 //0x48
	#define APL_PENALITY	0x30
	#define APL_MB_PULL2D	0x30
	#define COUNT_MB		(UINT32)0x10000L
#endif // #if ADAPTIVE_APL_DETECT

#if ADAPTIVE_CCS_ON_BW
    #define CCS_ENABLE_COLOR_TH 0x2500000L
#endif // #if ADAPTIVE_CCS_ON_BW

//mtk01140 20090902 3D COMB Callbak Test
//#define TDC_CALLBACK_CONTROL_BY_FBM 1


/**************************************************************************
 * Imported variables (optional)
 *************************************************************************/

/**************************************************************************
 * Global/Static variables
 *************************************************************************/
extern UINT8 _bTDCConf;
extern UINT8 _bTDCEnable;


typedef enum
{
	E_TDC_HIGH_BW,
	E_TDC_LOW_BW
} E_TDC_DAMODE;

/**************************************************************************
 * Function Members
 *************************************************************************/
void vDrvTDCInit(void);
void vDrvTDCSetDramBase(void);
void vDrvTDCSetDramMode(UINT8 b10BitMode);
void vDrvTDCSetDAMode(UINT8 bMode);
void vDrvTDCSaveBW(UINT8 bOnOff);
void vDrvTDCSet(void);
void vTdc3dProc(void);
void vDrvTDCDemo(UINT8 bOnOff, UINT8 bRegion, UINT16 wXCenter);
void vDrvTDCOnOff(UINT8 bOnOff);
void vDrvTDCActive(UINT8 bOnOff);
BOOL fgDrvTDCEnableChk(void);

void vDrvTDCSetStart(UINT16 wStart);
void vDrvTDCSetLength(UINT16 wLength);
void vDrvTDCSetTop(UINT16 wTop);
void vDrvTDCSetBottom(UINT16 wBottom);
UINT16 wDrvTDCGetStart(void);
UINT16 wDrvTDCGetLength(void);
UINT16 wDrvTDCGetTop(void);
UINT16 wDrvTDCGetBottom(void);
EXTERN void vDrvTDCSrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode);
#endif // #ifndef _DRV_TDC_H_

