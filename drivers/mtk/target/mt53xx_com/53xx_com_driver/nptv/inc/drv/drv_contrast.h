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

#ifndef _DRV_CONTRAST_H_
#define _DRV_CONTRAST_H_


#include "general.h"
#include "hw_ycproc.h"
#include "hw_scpos.h"
#include "vdo_misc.h"
#include "video_def.h"

#define Y_FTN_START     (Y_FTN_1_0_MAIN+6)

UINT16 wReadHist32Cur(UINT8 wAddr);
void   vWriteHist32Cur(UINT8 wAddr, UINT16 wData);
UINT16 wReadHist32Rec(UINT8 wAddr);
void   vWriteHist32Rec(UINT8 wAddr, UINT16 wData);


#define LUMA_BACKLIGHT_CTRL		SV_FALSE	// backlight control initialize: 0: OFF, 1:ON
#define LUMA_MODIFY_COUNT		0x0060	    // 0x1000 = 4096
#define LUMA_LONG_TIME			0x0020	    // 0x0040 = 64,  statistic 64 frame APL
#define LUMA_GAIN_ON			0x00
#define LUMA_GAIN_VALUE			0xd0
#define LUMA_GAIN_START_POINT	0x00
#define LUMA_GAIN_MAX_POINT		0x28
#define LUMA_GAIN_ZERO_POINT	0x50

#define CONFIG_BACKLIGHT_AL     SV_TRUE
#define BLACK_STRETCH_ENABLE    SV_TRUE
#define WHITE_STRETCH_ENABLE    SV_TRUE

#define BLACK_STRETCH_LEVEL 	5
#define BLACK_STRETCH_GAIN	    0x40
#define BLACK_STRETCH_OFFSET	0x20
#define BLACK_STRETCH_RATIO	    0x90
#define BLACK_STRETCH_LIMIT	    0x10

#define WHITE_STRETCH_LEVEL 	5
#define WHITE_STRETCH_GAIN	    0x40
#define WHITE_STRETCH_OFFSET	0x10
#define WHITE_STRETCH_RATIO	    0x68
#define WHITE_STRETCH_LIMIT	    0x00

#define AL_GAIN			        0x40
#define AL_OFFSET		        0
#define AL_LIMIT				0xFF
#define MAX_DIFF_THRESHOLD	    0x0A
#define TOTAL_DIFF_THRESHOLD  	0x30
#define MAX_NOISE_THRESHOLD     5

#define LUMA_HIST_LEVEL		    16
#define LUMA_HIST_NUM	        32
#define LUMA_CURVE_NUM	        (LUMA_HIST_NUM+1)

#define DYNAMIC_BACKLIGHT_ON        SV_TRUE
#define DBL_MAX_DIFF_THRESHOLD	    0x0A  
#define DBL_TOTAL_DIFF_THRESHOLD  	0x30 
#define DBL_MAX_NOISE_THRESHOLD     0x01
#define DBL_NORMALIZED_HIST_PREC    12
#define ALTERNATE_DBL_AND_AL_ON     SV_TRUE

typedef struct structBacklightCurve
{
	UINT8 x1;
	UINT8 x2;
	UINT8 x3;
	UINT8 x4;
	UINT8 y0;
	UINT8 y1;
	UINT8 y2;
	UINT8 y3;
	UINT8 y4;
	INT16 m0;
	INT16 m1;
	INT16 m2;
	INT16 m3;
	INT16 m4;
} structBacklightCurve;

enum
{
	DYNAMIC_CONTRAST_RATIO_100PERCENT = 0,
	DYNAMIC_CONTRAST_RATIO_150PERCENT,
	DYNAMIC_CONTRAST_RATIO_200PERCENT,
	DYNAMIC_CONTRAST_END
};


// ---- Histogram ------
void vDrvHistInit(void);
void vDrvHistTrigger(void);
UINT8 bDrvIsHistReady(void);
void vDrvGetHistInfo(void);
UINT8 bDrvGetHist(UINT16 *wHistgram);
UINT8 bDrvGetHist32(UINT16 *wHistgram);
UINT8 bDrvGetNormHist(UINT16 * wHist);
UINT8 bDrvGetNormHist32(UINT16 * wHist);
void vDrvGetNormHist32(UINT32 * wHist);
void vDrvGetHistogramInfoInVsync(void);  
void vDrvTriggerHistogramInVsync(void);
// ---- Luma Info ------
UINT8 bDrvGetAPL(void);
UINT8 bDrvGetLumaMax(void);
UINT8 bDrvGetLumaMin(void);
UINT8 bDrvGetOutputAPL(UINT8 bPath);
UINT8 bDrvGetOutputLumaMax(UINT8 bPath);
UINT8 bDrvGetOutputLumaMin(UINT8 bPath);

// ---- Adaptive luma -----
void vDrvAutoContrastInit(void);
void vDrvAutoContrastOn(UINT8 bPath);
void vDrvAutoContrastOff(UINT8 bPath);
void vDrvContrastUpdate(void);
void vDrvAutoContrast(void);
void vDrvSetYGammaCoeff(UINT8 bmode);
void vDrvAutoContrastFreeze(UINT8 bFreeze);
void vDrvContrastDefault(void);
void vDrvSetAutoContrastWindow(void);
void vDrvSetLumaCurve(const UINT16 *waLArray);
void vDrvReadLumaCurve(UINT16 * waLArray);
void vDrvAutoContrastParam(void);
void vDrvAutoContrastSetDefault(void);
void vDrvSetYGammaCoeff(UINT8 bmode);


void vDrvAutoContrastSetALOff(UINT8 bPath);


// ---- BWS -----
void vDrvVideoSoftBWSOnOff(UINT8 bOnOff);

void vDrvAutoContrastSetBSOff(UINT8 bPath);

// ---- Internal -----
void vDrvContrastProc_int(void);
void vDrvAutoContrastInit_int(void);

// ---- Dynamic Backlight -----
//void vDrvBacklightPreSet(UINT8 y0, UINT8 x1, UINT8 y1, UINT8 x2, UINT8 y2, UINT8 x3, UINT8 y3, UINT8 x4, UINT8 y4, UINT8 y5);
//void vDrvBacklightCtrl(void);
void vDrvDBLInit(void);
void vDrvBacklightCtrlInit(void);

// ---- OPC ----
#define RGB_HIST_NUM 16

UINT32 dwReadRGBHist16(UINT8 wAddr);
void vWriteRGBHist16(UINT8 wAddr, UINT32 wData);

void vDrvOPCProc(void);

#endif
