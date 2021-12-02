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
 * $Date  $
 * $RCSfile: drv_pwm.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef DRV_PWM_H
#define DRV_PWM_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_bim.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SrcPWM0 0
#define SrcPWM1 1
#define SrcPWM2 2
#define SrcPWM3 3
#define SrcPWM4 4
#define SrcPWM5 5

#define MINILVDS_IS_HSYNC 0
#define MINILVDS_IS_VSYNC 1

#if defined(CC_MT5396)
//OPWM0
#define PAD_PWM0_PWM0 	0	//FUNC2 
//OPWM1
#define PAD_PWM1_PWM1 	0	//FUNC2 (FUNC3 SCANPWM)
#define PAD_PWM1_GPIO48 1	//FUNC1
//OPWM2
#define PAD_PWM2_PWM2 	0	//FUNC2 

#elif defined(CC_MT5389)
//OPWM0
#define PAD_PWM0_GPIO3  0	//FUNC1	//opwm0
#define PAD_PWM0_OSDA0  1	//FUNC3
//OPWM1
#define PAD_PWM1_IF_AGC 0	//FUNC2	//opwm1
#define PAD_PWM1_OSCL0  1	//FUNC3
//OPWM2
#define PAD_PWM2_PWM0 	0	// FUNC1	//opwm2

#else	//defined(CC_MT5368)
//OPWM0
#define PAD_PWM0_PWM0 	0	// FUNC1
#define PAD_PWM0_GPIO4  1	//FUNC1
#define PAD_PWM0_OSDA0  2	//FUNC3
//OPWM1
#define PAD_PWM1_PWM1 	0	// FUNC1 ( FUNC2 SCANPWM)
#define PAD_PWM1_GPIO5  1	//FUNC1
#define PAD_PWM1_GPIO8  2	//FUNC1
#define PAD_PWM1_IF_AGC 3	//FUNC2
#define PAD_PWM1_OSCL0  4	//FUNC3
//OPWM2
#define PAD_PWM2_PWM2 	0	//FUNC1
#endif

#define vDrvSetScan1Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_00, u4Val, REG_PWM_SCAN1_START)
#define vDrvSetScan1High(u4Val) vIO32WriteFldAlign(PWM_SCAN_01, u4Val, REG_PWM_SCAN1_HIGH)
#define vDrvSetScan1Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_02, u4Val, REG_PWM_SCAN1_LOW)
#define vDrvSetScan2Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_03, u4Val, REG_PWM_SCAN2_START)
#define vDrvSetScan2High(u4Val) vIO32WriteFldAlign(PWM_SCAN_04, u4Val, REG_PWM_SCAN2_HIGH)
#define vDrvSetScan2Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_05, u4Val, REG_PWM_SCAN2_LOW)
// MT5396/68 New feature for ECO
#define vDrvSetScan3Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_07, u4Val, REG_PWM_SCAN3_START)
#define vDrvSetScan3High(u4Val) vIO32WriteFldAlign(PWM_SCAN_08, u4Val, REG_PWM_SCAN3_HIGH)
#define vDrvSetScan3Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_09, u4Val, REG_PWM_SCAN3_LOW)
#define vDrvSetScan4Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_0A, u4Val, REG_PWM_SCAN4_START)
#define vDrvSetScan4High(u4Val) vIO32WriteFldAlign(PWM_SCAN_0B, u4Val, REG_PWM_SCAN4_HIGH)
#define vDrvSetScan4Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_0C, u4Val, REG_PWM_SCAN4_LOW)
#define vDrvSetScan5Start(u4Val) vIO32WriteFldAlign(PWM_SCAN_0D, u4Val, REG_PWM_SCAN5_START)
#define vDrvSetScan5High(u4Val) vIO32WriteFldAlign(PWM_SCAN_0E, u4Val, REG_PWM_SCAN5_HIGH)
#define vDrvSetScan5Low(u4Val) vIO32WriteFldAlign(PWM_SCAN_0F, u4Val, REG_PWM_SCAN5_LOW)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

extern void vDrvSetPWM(UINT32 u4Src, UINT32 u4TimeBase, UINT32 u4DutyON,
                       UINT32 u4DutyAll);
void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON);
UINT8 vDrvGetPWMDutyCycleRatio(void);
void vDrvInitSCANPWM(void);
void vDrvSetScanPWM(UINT32 u4Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low);
void vDrvSetScanPWMPolarity(UINT32 u4Src, UINT8 u1blPol);
void vDrvSetScanPWMOnOff(UINT8 u1OnOff);
void vDrvSetScanPWMLatchMode(UINT8 u1Mode);
void vDrvSetScanPWMOutBL(UINT32 u4Src, UINT8 u1OutBL);
void vDrvSetScanBLPolarity(UINT8 u1blPol);
void vDrvSetScanBLOnOff(UINT8 u1OnOff);
void vDrvSetScanBLLatchMode(UINT8 u1Mode);

void vDrvSetISOnOff(UINT8 u1Src, UINT8 u1OnOff);
void vDrvSetISMaskNum(UINT8 u1Src, UINT8 u1MaskNum);
void vDrvSetISInputInverse(UINT8 u1Src, UINT8 u1Enable);
void vDrvSetISDelayOnOff(UINT8 u1Src, UINT8 u1OnOff);
void vDrvSetISDelayNum(UINT8 u1Src, UINT16 u2DelayNum);
void vDrvSetISHoldOnOff(UINT8 u1Src, UINT8 u1OnOff);
void vDrvSetISRelation(UINT8 u1Src, UINT8 u1SyncCnt, UINT8 u1ISSyncCnt);


#endif
