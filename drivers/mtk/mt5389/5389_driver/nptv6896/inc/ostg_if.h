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
 * $Author: binghe.li $
 * $Date: 2013/01/12 $
 * $RCSfile: ostg_if.h,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file ostg_if.h
 *  NPTV Output Stage interface for LVDS and Panel Setting
 */

#ifndef _OSTG_IF_H_
#define _OSTG_IF_H_

#include "x_typedef.h"
#include "sv_const.h"
#include "api_backlt.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PANEL_OFF 0
#define PANEL_ON 1
// for backward compatible
//#define PANEL_DIMMER(bDimLevel) vApiSetPanelBright(bDimLevel)

#ifndef LOADER_MULTIPLE_LOGO_PARTITION
#define LOADER_MULTIPLE_LOGO_PARTITION	(0)
#endif

#ifndef LOADER_LED_TWINKLE
#define LOADER_LED_TWINKLE          (0)
#endif

///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------

/** Set LVDS driving current
 *  @param bDriving: 0~15
 *  @return
 */
EXTERN void vApiLVDSSetDriving(UINT8 bDriving);

/**
 * @brief Set Panel output frame rate
 * @param wFrameRate
 * @ return
 */
EXTERN void vApiLCDSetFrameRate(UINT16 wFrameRate);

/**
 * Get Panel output frame rate
 * @param void
 * @ return output frame rate
 */
EXTERN UINT8  bApiLCDGetFrameRate(void);

void vDrvGpioSet(INT32 gpio, INT32 value);
// backlight power-------------------------------------------------------------
void vDrvTurnOnBacklt(void);
void vDrvTurnOffBacklt(void);
// PDP VsON------------------------------------------------------------------
void vDrvTurnOnPDPVs(void);
void vDrvTurnOffPDPVs(void);
extern void vApiVestelPDWNSequence(void);
extern void vApiVestelPUPSequence(void);


extern void vApiPanelPowerSequence(BOOL bPMode);
extern UINT32 LDR_PmxDisplay(UINT32 u4Background);

extern UINT32 Vestel_PmxDisplay(UINT32 u4Background);

extern void BeforeOutputFrameRateChange(UINT32 u4FrameRate);
extern void AfterOutputFrameRateChange(void);

extern UINT32 BACKLT_GetCurrentPanelBright(void);
extern void   BACKLT_SetDimming(UINT32 u4Frequency, UINT32 u4Dimming);
extern UINT32 BACKLT_GetDimmingFrequency(void);

extern void vApiSetPanelBright(UINT8 bBrightLevel);
extern void vApiSetPanelBrightTemp(UINT8 bBrightLevel);//mtk70646


extern void vApiBackltONOFF(BOOL bPMode);

void vApiSetDimming(UINT16 u2Freq, UINT16 u2Duty);
void vApiSetBooster(UINT16 wLevel);


extern UINT8 bIsPanelOn(void); 

#ifdef CC_SCPOS_3DTV_SUPPORT
EXTERN void vApiSetScanningPWM(UINT16 u2HTotal, UINT16 u2VTotal, UINT32 u4Start, UINT32 u4High);
#endif

#ifdef CC_FAST_INIT
extern void ostg_set_fgPanelOn(BOOL onoff);
#endif

#endif // _OSTG_IF_H_
 
