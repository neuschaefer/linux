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
 *   $Workfile: drv_contrast.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/23 2:49p $
 *
 * $Revision: #1 $
*************************************************************************/

#ifndef _DRV_METER_H_
#define _DRV_METER_H_

#include "general.h"

#define LUMA_HIST_NUM	        32
#define LUMA_HIST_NUM_WEIGHT    (LUMA_HIST_NUM + 1)
#define SAT_HIST_NUM            8
#define HUE_HIST_NUM            8
#define RGB_MAX_HIST_NUM        32
#define SAT_HUE_2DWIN_NUM       3

typedef enum
{
    E_HUE_DIST_R,     
    E_HUE_DIST_G,     
    E_HUE_DIST_B,     
    E_HUE_DIST_C,     
    E_HUE_DIST_Y,    
    E_HUE_DIST_M,  
    E_HUE_DIST_W,
    E_HUE_DIST_SKIN
} E_HUE_DIST;


// ---- General API ------
void vDrvMeterInit(void);
void vDrvMeterProc(void);

// ---- Luma Info ------
void vDrvGetLumaInfoHW(void);
UINT8 bDrvGetAPL(void);
void vDrvGetAPLLR(UINT8* bAPLL, UINT8* bAPLLR);
UINT8 u1DrvGetLumaHist(UINT16 waHistCur[LUMA_HIST_NUM_WEIGHT], UINT16 u2NormBase);
void vDrvGetLumaHistLR(UINT16 waHistL[LUMA_HIST_NUM_WEIGHT], UINT16 waHistR[LUMA_HIST_NUM], UINT16 u2NormBase);
UINT8 bDrvGetOutputLumaMax(UINT8 bPath);
UINT8 bDrvGetOutputLumaMin(UINT8 bPath);
void vDrvGetInputLumaMaxLR(UINT8* bMaxL, UINT8* bMaxR);
void vDrvGetInputLumaMinLR(UINT8* bMinL, UINT8* bMinR);

// ---- RGB Max Meter ------
UINT8 u1DrvGetRGBHist(UINT16 u4aHist[RGB_MAX_HIST_NUM], UINT16 u2NormBase);
void vDrvSetRGBMaxBoundary(UINT8 * bBoundary);

// ---- Saturation Histogram ------
UINT8 u1DrvGetASL(void);
void vDrvSetSatHistBoundary(UINT8* bBoundary);
void vDrvSetSatHistWindow(UINT16* u2Window);
UINT8 u1DrvGetSatHist(UINT16 u2aHist[SAT_HIST_NUM], UINT16 u2NormBase);
void vDrvGetSatHistLR(UINT16 u2aHistL[SAT_HIST_NUM], UINT16 u2aHistR[SAT_HIST_NUM], UINT16 u2NormBase);

// ---- Hue Histogram ------
void vDrvSetHueHistBoundary(UINT8* bBoundary);
UINT8 u1DrvGetHueHist(UINT16 u2aHist[HUE_HIST_NUM], UINT32 *u4PixNumNorm, UINT16 u2NormBase);
void vDrvGetHueHistLR(UINT16 u2aHistL[HUE_HIST_NUM], UINT16 u2aHistR[HUE_HIST_NUM], UINT16 u2NormBase);
void vDrvSatHistWinReconfig(void);

// ---- Sat & Hue 2D Histogram ------
void vDrvSetSatHue2DWindowBound(UINT8 bWinSel, UINT8 bSatUpper, UINT8 bSatLower, UINT8 bHueUpper, UINT8 bHueLower);
UINT32 u4DrvGetSatHue2DWindowResult(UINT8 bWinSel, UINT16 u2NormBase);
void vDrvGet2DWindowLR(UINT16 u2WinL[3], UINT16 u2WinR[3], UINT16 u2NormBase);

#endif
