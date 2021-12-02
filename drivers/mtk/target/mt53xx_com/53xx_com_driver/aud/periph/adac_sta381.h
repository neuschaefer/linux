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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: adac_apogee.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_sta381.h
 *  Brief of file adac_sta381.h. \n
 */
#ifndef ADAC_STA381_H
#define ADAC_STA381_H

//#define CC_AUD_AMP_SUPPORT_5_1_CH
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define REG_MODE_CTL     0x0A
  #define MASTER_MODE    0x00
  #define SLAVE_MODE     0x20

#define GPIO_PAD_ML  42
#define GPIO_PAD_MC  43
#define GPIO_PAD_MD  44  

#ifdef CC_AUD_AMP_IN_I2C_BUS_2
#define AUD_AMP_BUS_ID          2
#elif defined (CC_AUD_AMP_IN_I2C_BUS_1)
#define AUD_AMP_BUS_ID          1
#else
#define AUD_AMP_BUS_ID          0
#endif

#ifndef CC_AUD_AMP_STA333BW_SA_1
#define STA381BW_ADDR             0x38
#else
#define STA381BW_ADDR             0x3a
#endif

#define SIF_CLK_DIV                                0x100

#define STA381BW_CONFA 0x00
#define STA381BW_CONFB 0x01
#define STA381BW_CONFC 0x02
#define STA381BW_CONFD 0x03
#define STA232W_CONFE 0x04
#define STA381BW_CONFF 0x05
#define STA381BW_MASMUTE 0x06
#define STA381BW_MASTER_VOL_CRL   0x07
   #define MASTER_VOL_0DB           (0x0)
   #define MASTER_VOL_NEG_38DB      (0x4c)
   #define MASTER_VOL_MUTE          (0xff)   
#define STA381BW_CH1_VOL_CRL    0x08
#define STA381BW_CH2_VOL_CRL    0x09
#define STA381BW_CH3_VOL_CRL    0x0a
#define STA381BW_AUTOMODE       0x0c

#define STA232W_AUTOMODE 0x0B
#define STA381BW_CH1CONFIG	0x0E
#define STA381BW_CH2CONFIG	0x0F
#define STA381BW_CH3CONFIG	0x10
#define STA232W_LIMITER_1   0x12
#define STA381BW_LIMITER	0x13
#define STA381BW_LIMITER_THRESHOLD	0x15

#define STA339_ATTACK1_THRESHOLD 0x32
#define STA339_ATTACK2_THRESHOLD 0x34
#define STA339_RELEASE1_THRESHOLD 0x33
#define STA339_RELEASE2_THRESHOLD 0x35
#define STA339_ATTACK_RATE1 0x12
#define STA339_ATTACK_RATE2 0x14

#define STA339_EQDRC_CFG 0x31
#define STA339_CFADDR    0x16
#define STA339_CFUD      0x26

#define STA339_B1CF1 0x17
#define STA339_B1CF2 0x18
#define STA339_B1CF3 0x19

#define STA339_B2CF1 0x1A
#define STA339_B2CF2 0x1B
#define STA339_B2CF3 0x1C

#define STA339_A1CF1 0x1D
#define STA339_A1CF2 0x1E
#define STA339_A1CF3 0x1F

#define STA339_A2CF1 0x20
#define STA339_A2CF2 0x21
#define STA339_A2CF3 0x22

#define STA339_B0CF1 0x23
#define STA339_B0CF2 0x24
#define STA339_B0CF3 0x25

#define STA559BW_CFADDR         0x16
#define STA559BW_BIQUADST       0x17
#define STA559BW_CFUD           0x26
    #define CFUD_WA             (0x01 << 1)

#define MUTE_ALL_CHL  0x01
#define UNMUTE_ALL_CHL 0

#define STA381BW_HPCFG      0x55
#define STA381BW_F3XCFG2    0x59
#define STA381BW_MISC4      0x7E
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern AMP_T* _AMP_GetAmpSTA381FunTbl(void);

#endif /* ADAC_STA381_H */

