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
 * $RCSfile: piana_TDAU4XB02A.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file piana_TDAU4XB02A.h
 *
 * Alps TDAU4XB02A Tuner Driver Declaration.
 *  
 **/

#ifndef _PIANA_TDAU4XB02A_H_
#define _PIANA_TDAU4XB02A_H_

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_anana_tvsys_info.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define fcMODIFY_FOR_MP         0

//#define TUNER_ONE_ADDR_TUNER    0xC2

#define TDAU4XB02A_ANA_INIT_FREQ   55000       // Khz
#define TDAU4XB02A_ANA_PIC_IF      45750       // Khz
#define TDAU4XB02A_ANA_FREQ_STEP   500         // 100hz

//#if fcNEW_ANA_PIPD_ARCH
//#define cFREQ_FINETUNE_RANGE    300         // Khz
//#else
// change for one channel lost; 0919
//    #if fcMODIFY_FOR_MP
//#define cFREQ_FINETUNE_RANGE    5
//    #else
//#define cFREQ_FINETUNE_RANGE    6
//    #endif
//#endif

#define TDAU4XB02A_ANA_UNLOCK        0
#define TDAU4XB02A_ANA_LOCK          1

#define TDAU4XB02A_ANA_TUNER_VER     "ver1.0_TDAU4XB02A"

/**  Tuner I2C Address*/
#define TUNER_ONE_ADDR_TUNER       0xC2

// Every Tuner have one
//#define TV_FINE_TUNE_THRESHOLD  0x48
#define TV_FINE_TUNE_THRESHOLD_KHZ  4500    // -2.25MHz ~  +2.25MHz
#define SCAN_TINY_STEP_KHZ          125
#define SCAN_SMALL_STEP_KHZ         250
#define SCAN_MIDDLE_STEP_KHZ        500
//#define SCAN_LARGE_STEP_KHZ         1000
//#define SCAN_QUICK_STEP_KHZ         2000
//#define SCAN_NEXT_CH_STEP_KHZ       3000

//enum __TUNERPART_CTRLDATA
//{
  //  TUNER_PART_CTRL_FREQ_H = 0,
  //  TUNER_PART_CTRL_FREQ_L,
  //  TUNER_PART_CTRL_STEP,
  //  TUNER_PART_CTRL_BAND,
  //  TUNER_PART_CTRL_AUX,
  //  TUNER_PART_CTRL_DATASIZE
//};

  /**
 *  Control Byte CB
 *
 *   Bit7  Bit6  Bit5  Bit4  Bit3  Bit2 Bit1 Bit0
 *    1     CP    T2    T1    T0   RSA  RSB   OS
 *
 *  Default value: 11001110 = 0xCE
 *
 *   PLL Disabling (normal operation) : OS=0
 *  Resolution frequency (62.5KHz) :  RSA=1, RSB=1
 *   Test Mode Setting (normal operation) : T2=0, T1=0, T0=1
 *   Charge-pump current (High) :  CP=1
  */

//#define DTVS205_TUNER_PART_CTRL_STEP_DEFAULT  0x8B
//#define DTVS205_TUNER_PART_CTRL_DEFAULT       (0xC6 | BIT5)

 /**
*    Auxiliary Byte AB
*
*    Bit7  Bit6  Bit5  Bit4  Bit3  Bit2 Bit1 Bit0
*    ATC   AL2   AL1   AL0    0     0    0    0
*
*   Default value: 01000000 = 0x40
*
*   AGC time constant (2 sec) : ATC=0
*   AGC TOP Setting (106 dBuV) : AL2=1, AL1=0, AL0=0
*/
//#define TUNER_PART_CTRL_AUX_DEFAULT   0x40

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#endif /* __PIANA_DTVS205X514A_H__ */
  
