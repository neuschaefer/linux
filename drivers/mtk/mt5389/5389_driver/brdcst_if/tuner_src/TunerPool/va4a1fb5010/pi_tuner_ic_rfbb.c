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
 * $RCSfile: pi_tuner_ic_rfbb.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_tuner_ic_rfbb.c
 *  Tuner RFIC&BBIC control for sharp va4a1fb5010
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "ctrl_bus.h"
#include "PI_Def.h"
#include "fe_tuner_common_if.h"
#include "sif_if.h"
#include "drvcust_if.h"   //Get Tuner/Demod I2C bus id
#include "pi_tuner_va4a1fb5010.h"

//-----------------------------------------------------------------------------
// Global variable
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define I2C_TIME 270
UINT8 aru1INIT_RF[]={0xC0,0xCD,0x03,0xCF,0x04,0x8A,0x03,0xA2,0x00,0xA3,0xCC,0xD1,0x86};
UINT8 aru1INIT_BB[]={0xC4,0x93,0x70,0xAD,0x07,0xCA,0x08,0xCC,0x7F,0xCC,0x3F,0xA8,0x00,0xA0,0x46};

// 181.25MHz_Terrestrial_LowIF_4MHz(181.25-->183MHz)
//Change the TV standard
UINT8 aru1CHANGE_RF_A[]={0xC0,0xA4,0x20,0xA6,0x8A,0xA7,0x20,0xA9,0x8A,0xAA,0x7B,0xAB,0x2D,0xAC,0x6B,0xAD,0x2C,0xAE,0x99,0xAF,0x28,0xB0,0x2C,0xB1,0x25,0xB2,0x24,0xB3,0x3C,0x8F,0x20,0x90,0x01,0x91,0x20,0x92,0x01,0xA1,0x14,0x8B,0xD8,0x9F,0x78,0x8C,0x3D,0xB4,0x0B,0x9A,0x00,0x9B,0x64,0x9C,0x5A,0x9D,0x55,0x9E,0x41,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_A[]={0xC4,0x85,0x06,0x86,0x03,0x90,0x03,0x9C,0x86,0x96,0x80,0x97,0x9F,0x9E,0x6A,0xAB,0x30,0x8C,0xAD,0x9A,0x13};
UINT8 aru1SET_ATV_A[]={0xC4,0xA1,0x00,0xA3,0x8A,0xA9,0x2C,0xA2,0x60};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_A[] ={0xC0,0x86,0x98,0xC5,0x09};
//UINT8 aru1RFF_PRECAL_A[]={0xC0,0xC6,0xAE,0xC8,0x96,0x82,0x84,0xCF,0xD4,0xD0,0x0B,0xC7,0xDD,0xC9,0x7E,0xCA,0x51,0xCE,0x90,0xCB,0x64,0xCC,0x7C};
UINT8 aru1RFF_PRECAL_A[]={0xC0,0xC6,0xAE,0xC8,0x96,0x82,0x84,0xCF,0x94,0xD0,0x0B,0xC7,0xDD,0xC9,0x7B,0xCA,0xD1,0xCE,0x8D,0xCB,0x94,0xCC,0x88};
UINT8 aru1RFF_CAL_A[]={0xC0,0xD0,0x8B};
//UINT8 aru1PLL_SETUP_A[]={0xC0,0xD2,0x85,0xD8,0x17,0xD3,0x3A,0xD4,0x66,0xD5,0x66,0xD6,0x50,0xD7,0xF4,0xD9,0x1F,0xDA,0x08,0xB3,0x3C};
UINT8 aru1PLL_SETUP_A[]={0xC0,0xD2,0x85,0xD8,0x17,0xD3,0x05,0xD4,0x99,0xD5,0x9A,0xD6,0x50,0xD7,0xF4,0xD9,0x1F,0xDA,0x08,0xB3,0x3C};
UINT8 aru1AGC_CH_A[]={0xC0,0xA5,0x1D,0xA8,0x1D};
//UINT8 aru1PRE_SETUP_BB_A[]={0xC4,0xA7,0xB6,0xCF,0x4C,0xCD,0xB2,0x87,0x1E};
UINT8 aru1PRE_SETUP_BB_A[]={0xC4,0xA7,0xB6,0xCF,0x50,0xCD,0xB2,0x87,0x0E};
//UINT8 aru1LOWIF_ATV_CH_A[]={0xC4,0x88,0x0F,0x89,0xB1,0x8A,0xDA,0xAB,0x31,0xAC,0x43};
UINT8 aru1LOWIF_ATV_CH_A[]={0xC4,0x88,0x0F,0x89,0xB5,0x8A,0xC3,0xAB,0x31,0xAC,0x33};
UINT8 aru1SFTRST_BB_ON_A[]={0xC4,0xCC,0x3E,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_A[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_A[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_A[]={0xC4,0x81,0x10,0xCC,0x3F};
UINT8 aru1DCOC_A[]={0xC0,0x86,0x85};
UINT8 aru1CVBS_A[]={0xC4,0x86,0x0B};

// 183MHz_ATSC_IF_4MHz
//Change the TV standard
UINT8 aru1CHANGE_RF_T[]={0xC0,0xA4,0x28,0xA6,0x85,0xA7,0x28,0xA9,0x85,0xAA,0x9E,0xAB,0x2D,0xAC,0x5C,0xAD,0x2C,0xAE,0x99,0xAF,0x28,0xB0,0x2C,0xB1,0x39,0xB2,0x28,0xB3,0x14,0x8F,0x08,0x90,0x01,0x91,0x08,0x92,0x01,0xA1,0x14,0x8B,0xDD,0x9F,0x7F,0x8C,0x3F,0xB4,0x09,0x9A,0x00,0x9B,0x7E,0x9C,0x4B,0x9D,0x41,0x9E,0x3C,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_T[]={0xC4,0x85,0x86,0x86,0x10,0x90,0x03,0x9C,0x06,0x96,0x90,0x97,0x90,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x13};
//UINT8 aru1CHANGE_BB_T[] ={0xC4,0x85,0x86,0x86,0x13,0x90,0x33,0x9C,0x06,0x96,0xA0,0x97,0xA5,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x93};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_T[] ={0xC0,0x86,0x98,0xC5,0x06};
UINT8 aru1RFF_PRECAL_T[]={0xC0,0xC6,0xAE,0xC8,0x96,0x82,0x84,0xCF,0x94,0xD0,0x0B,0xC7,0xDD,0xC9,0x7B,0xCA,0xD1,0xCE,0x8D,0xCB,0x94,0xCC,0x88};
UINT8 aru1RFF_CAL_T[]={0xC0,0xD0,0x8B};
UINT8 aru1PLL_SETUP_T[]={0xC0,0xD2,0x05,0xD8,0x17,0xD3,0x03,0xD4,0x33,0xD5,0x33,0xD6,0x50,0xD7,0xF4,0xD9,0x1F,0xDA,0x08,0xB3,0x14};
UINT8 aru1AGC_CH_T[]={0xC0,0xA5,0x1B,0xA8,0x1B};
UINT8 aru1PRE_SETUP_BB_T[]={0xC4,0xA7,0xB6,0xCF,0x4C,0xCD,0xB2,0x87,0x16};
UINT8 aru1DTV_MODE_CH_T[]={0xC4,0x88,0x00,0x89,0x56,0x8A,0x3B};
UINT8 aru1SFTRST_BB_ON_T[]={0xC4,0xCC,0x3E,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_T[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_T[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_T[]={0xC4,0x81,0x10,0xCC,0x3F};
UINT8 aru1DCOC_T[]={0xC0,0x86,0x89};

// 551MHz_ATSC_IF_4MHz
//Change the TV standard
UINT8 aru1CHANGE_RF_T_551MHZ[]={0xC0,0xA4,0x28,0xA6,0x85,0xA7,0x28,0xA9,0x85,0xAA,0x9E,0xAB,0x2D,0xAC,0x5C,0xAD,0x2C,0xAE,0x99,0xAF,0x28,0xB0,0x2C,0xB1,0x39,0xB2,0x28,0xB3,0x14,0x8F,0x08,0x90,0x01,0x91,0x08,0x92,0x01,0xA1,0x14,0x8B,0xDD,0x9F,0x7F,0x8C,0x3F,0xB4,0x09,0x9A,0x00,0x9B,0x7E,0x9C,0x4B,0x9D,0x41,0x9E,0x3C,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_T_551MHZ[]={0xC4,0x85,0x86,0x86,0x10,0x90,0x03,0x9C,0x06,0x96,0x90,0x97,0x90,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x13};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_T_551MHZ[] ={0xC0,0x86,0x98,0xC5,0x06};
UINT8 aru1RFF_PRECAL_T_551MHZ[]={0xC0,0xC6,0xCE,0xC8,0xF6,0x82,0x84,0xCF,0x1C,0xD0,0x09,0xC7,0xE5,0xC9,0x4B,0xCA,0x19,0xCE,0x5C,0xCB,0xC6,0xCC,0x9C};
UINT8 aru1RFF_CAL_T_551MHZ[]={0xC0,0xD0,0x89};
UINT8 aru1PLL_SETUP_T_551MHZ[]={0xC0,0xD2,0x1F,0xD8,0x16,0xD3,0x01,0xD4,0x99,0xD5,0x9A,0xD6,0x4B,0xD7,0xF6,0xD9,0xC8,0xDA,0x08,0xB3,0x14};
UINT8 aru1AGC_CH_T_551MHZ[]={0xC0,0xA5,0x1C,0xA8,0x1C};
UINT8 aru1PRE_SETUP_BB_T_551MHZ[]={0xC4,0xA7,0xB6,0xCF,0x50,0xCD,0xB2,0x87,0x06};
UINT8 aru1DTV_MODE_CH_T_551MHZ[]={0xC4,0x88,0x00,0x89,0x51,0x8A,0xEC};
UINT8 aru1SFTRST_BB_ON_T_551MHZ[]={0xC4,0xCC,0x3E,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_T_551MHZ[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_T_551MHZ[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_T_551MHZ[]={0xC4,0x81,0x10,0xCC,0x3F};
UINT8 aru1DCOC_T_551MHZ[]={0xC0,0x86,0x89};

// 111MHz_ATSC_Cable_IF_4MHz
//Change the TV standard
UINT8 aru1CHANGE_RF_C_111MHZ[]={0xC0,0xA4,0x28,0xA6,0x85,0xA7,0x28,0xA9,0x85,0xAA,0x9E,0xAB,0x2D,0xAC,0x5C,0xAD,0x2C,0xAE,0x99,0xAF,0x28,0xB0,0x2C,0xB1,0x39,0xB2,0x28,0xB3,0x14,0x8F,0x08,0x90,0x01,0x91,0x08,0x92,0x01,0xA1,0x14,0x8B,0xDD,0x9F,0x7F,0x8C,0x20,0xB4,0x19,0x9A,0x00,0x9B,0x7E,0x9C,0x4B,0x9D,0x41,0x9E,0x3C,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_C_111MHZ[]={0xC4,0x85,0x86,0x86,0x13,0x90,0x33,0x9C,0x06,0x96,0x90,0x97,0x90,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x93};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_C_111MHZ[] ={0xC0,0x86,0x98,0xC5,0x06};
UINT8 aru1RFF_PRECAL_C_111MHZ[]={0xC0,0xC6,0x8E,0xC8,0x64,0x82,0x84,0xCF,0x94,0xD0,0x0B,0xC7,0x65,0xC9,0x1E,0xCA,0x15,0xCE,0x1A,0xCB,0x05,0xCC,0x3C};
UINT8 aru1RFF_CAL_C_111MHZ[]={0xC0,0xD0,0x8B};
UINT8 aru1PLL_SETUP_C_111MHZ[]={0xC0,0xD2,0x05,0xD8,0x17,0xD3,0x06,0xD4,0x66,0xD5,0x66,0xD6,0x49,0xD7,0xF3,0xD9,0xC8,0xDA,0x08,0xB3,0x14};
UINT8 aru1AGC_CH_C_111MHZ[]={0xC0,0xA5,0x1E,0xA8,0x1E};
UINT8 aru1PRE_SETUP_BB_C_111MHZ[]={0xC4,0xA7,0xB6,0xCF,0x4C,0xCD,0xB2,0x87,0x16};
UINT8 aru1DTV_MODE_CH_C_111MHZ[]={0xC4,0x88,0x00,0x89,0x56,0x8A,0x3B};
UINT8 aru1SFTRST_BB_ON_C_111MHZ[]={0xC4,0xCC,0x3E,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_C_111MHZ[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_C_111MHZ[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_C_111MHZ[]={0xC4,0x81,0x10,0xCC,0x3F};
UINT8 aru1DCOC_C_111MHZ[]={0xC0,0x86,0x89};

// 183MHz_ATSC_Cable_IF_4MHz
//Change the TV standard
UINT8 aru1CHANGE_RF_C[]={0xC0,0xA4,0x28,0xA6,0x85,0xA7,0x28,0xA9,0x85,0xAA,0x9E,0xAB,0x2D,0xAC,0x5C,0xAD,0x2C,0xAE,0x99,0xAF,0x28,0xB0,0x2C,0xB1,0x39,0xB2,0x28,0xB3,0x14,0x8F,0x08,0x90,0x01,0x91,0x08,0x92,0x01,0xA1,0x14,0x8B,0xDD,0x9F,0x7F,0x8C,0x20,0xB4,0x19,0x9A,0x00,0x9B,0x7E,0x9C,0x4B,0x9D,0x41,0x9E,0x3C,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_C[]={0xC4,0x85,0x86,0x86,0x13,0x90,0x33,0x9C,0x06,0x96,0x90,0x97,0x90,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x93};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_C[] ={0xC0,0x86,0x98,0xC5,0x06};
UINT8 aru1RFF_PRECAL_C[]={0xC0,0xC6,0xAE,0xC8,0x96,0x82,0x84,0xCF,0x94,0xD0,0x0B,0xC7,0xDD,0xC9,0x7B,0xCA,0xD1,0xCE,0x8D,0xCB,0x94,0xCC,0x88};
UINT8 aru1RFF_CAL_C[]={0xC0,0xD0,0x8B};
UINT8 aru1PLL_SETUP_C[]={0xC0,0xD2,0x05,0xD8,0x17,0xD3,0x03,0xD4,0x33,0xD5,0x33,0xD6,0x50,0xD7,0xF4,0xD9,0x1F,0xDA,0x08,0xB3,0x14};
UINT8 aru1AGC_CH_C[]={0xC0,0xA5,0x1B,0xA8,0x1B};
UINT8 aru1PRE_SETUP_BB_C[]={0xC4,0xA7,0xB6,0xCF,0x4C,0xCD,0xB2,0x87,0x16};
UINT8 aru1DTV_MODE_CH_C[]={0xC4,0x88,0x00,0x89,0x56,0x8A,0x3B};
UINT8 aru1SFTRST_BB_ON_C[]={0xC4,0xCC,0x3E,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_C[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_C[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_C[]={0xC4,0x81,0x10,0xCC,0x3F};
UINT8 aru1DCOC_C[]={0xC0,0x86,0x89};


// 666MHz_DVBT_IF_4MHz
//Change the TV standard
UINT8 aru1CHANGE_RF_DVBT_666MHz[]={0xC0,0xA4,0x23,0xA6,0x85,0xA7,0x23,0xA9,0x85,0xAA,0x7E,0xAB,0x2D,0xAC,0x5C,0xAD,0x2C,0xAE,0x9F,0xAF,0x28,0xB0,0x2C,0xB1,0x7F,0xB2,0x28,0xB3,0x1F,0x8F,0x20,0x90,0x00,0x91,0x20,0x92,0x00,0xA1,0x14,0x8B,0xDD,0x9F,0x7F,0x8C,0x3F,0xB4,0x09,0x9A,0x00,0x9B,0x7E,0x9C,0x4B,0x9D,0x41,0x9E,0x3C,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_DVBT_666MHz[]={0xC4,0x85,0x86,0x86,0x10,0x90,0x03,0x9C,0x06,0x96,0xA0,0x97,0xA5,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x13,0xCC,0x3D};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_DVBT_666MHz[] ={0xC0,0x86,0x98,0xC5,0x00};
UINT8 aru1RFF_PRECAL_DVBT_666MHz[]={0xC0,0xC6,0xCE,0xC8,0xF6,0x82,0x84,0xCF,0x1C,0xD0,0x09,0xC7,0xBD,0xC9,0x30,0xCA,0x5E,0xCE,0x37,0xCB,0x27,0xCC,0xF0};
UINT8 aru1RFF_CAL_DVBT_666MHz[]={0xC0,0xD0,0x89};
UINT8 aru1PLL_SETUP_DVBT_666MHz[]={0xC0,0xD2,0x1F,0xD8,0x16,0xD3,0x39,0xD4,0x99,0xD5,0x9A,0xD6,0x4E,0xD7,0xF6,0xD9,0x53,0xDA,0x08,0xB3,0x1F};
UINT8 aru1AGC_CH_DVBT_666MHz[]={0xC0,0xA5,0x1B,0xA8,0x1B};
UINT8 aru1PRE_SETUP_BB_DVBT_666MHz[]={0xC4,0xA7,0xB6,0xCF,0x50,0xCD,0xB2,0x87,0x00};
UINT8 aru1DTV_MODE_CH_DVBT_666MHz[]={0xC4,0x88,0x00,0x89,0x66,0x8A,0x66};
UINT8 aru1SFTRST_BB_ON_DVBT_666MHz[]={0xC4,0xCC,0x3C,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_DVBT_666MHz[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_DVBT_666MHz[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_DVBT_666MHz[]={0xC4,0x81,0x10,0xCC,0x3D};
UINT8 aru1DCOC_DVBT_666MHz[]={0xC0,0x86,0x89};



// 666MHz_DVBC_IF_4MHz
//Change the TV standard
UINT8 aru1CHANGE_RF_DVBC_666MHz[]={0xC0,0xA4,0x2C,0xA6,0x85,0xA7,0x2C,0xA9,0x85,0xAA,0x9E,0xAB,0x2D,0xAC,0x5C,0xAD,0x2C,0xAE,0x99,0xAF,0x28,0xB0,0x2C,0xB1,0x39,0xB2,0x28,0xB3,0x14,0x8F,0x08,0x90,0x01,0x91,0x08,0x92,0x01,0xA1,0x14,0x8B,0xDD,0x9F,0x7F,0x8C,0x3F,0xB4,0x19,0x9A,0x00,0x9B,0x7E,0x9C,0x4B,0x9D,0x41,0x9E,0x3C,0x94,0x04,0xA2,0x00};
UINT8 aru1CHANGE_BB_DVBC_666MHz[]={0xC4,0x85,0x86,0x86,0x13,0x90,0x33,0x9C,0x06,0x96,0xA0,0x97,0xA5,0x9E,0xAA,0xAB,0x20,0x8C,0xA5,0x9A,0x93,0xCC,0x3D};
// Change the Received the received channel
UINT8 aru1PRE_SETUP_DVBC_666MHz[] ={0xC0,0x86,0x98,0xC5,0x00};
UINT8 aru1RFF_PRECAL_DVBC_666MHz[]={0xC0,0xC6,0xCE,0xC8,0xF6,0x82,0x84,0xCF,0x1C,0xD0,0x09,0xC7,0xBD,0xC9,0x30,0xCA,0x5E,0xCE,0x37,0xCB,0x27,0xCC,0xF0};
UINT8 aru1RFF_CAL_DVBC_666MHz[]={0xC0,0xD0,0x89};
UINT8 aru1PLL_SETUP_DVBC_666MHz[]={0xC0,0xD2,0x1F,0xD8,0x16,0xD3,0x39,0xD4,0x99,0xD5,0x9A,0xD6,0x4E,0xD7,0xF6,0xD9,0x53,0xDA,0x08,0xB3,0x14};
UINT8 aru1AGC_CH_DVBC_666MHz[]={0xC0,0xA5,0x1B,0xA8,0x1B};
UINT8 aru1PRE_SETUP_BB_DVBC_666MHz[]={0xC4,0xA7,0xB6,0xCF,0x50,0xCD,0xB2,0x87,0x00};
UINT8 aru1DTV_MODE_CH_DVBC_666MHz[]={0xC4,0x88,0x00,0x89,0x66,0x8A,0x66};
UINT8 aru1SFTRST_BB_ON_DVBC_666MHz[]={0xC4,0xCC,0x3C,0x81,0x00};
UINT8 aru1SFTRST_RF_ON_DVBC_666MHz[]={0xC0,0x81,0x00};
UINT8 aru1SFTRST_RF_OFF_DVBC_666MHz[]={0xC0,0x81,0x10};
UINT8 aru1SFTRST_BB_OFF_DVBC_666MHz[]={0xC4,0x81,0x10,0xCC,0x3D};
UINT8 aru1DCOC_DVBC_666MHz[]={0xC0,0x86,0x89};


//-------------------------------------------------------------------------
/** Tuner_I2cRead
 *  Read tuner api :Read regisiter of tuner by external I2C.
 *  @param  u1DevAddr     Serial interface device address.
 *  @param  regAddr       Demod regisiter address.
 *  @param  data          Pointer to user's data.
 *  @param  len           Number of byte to read. 
 *  @retval   1           Read fail
 *  @retval   0           Read success
 */
//-------------------------------------------------------------------------
UINT8 Tuner_I2cRead(UINT8 i2cAddr, UINT8 regAddr, UINT8 *data, UINT16 len)
{
UINT32 u4DemodBusId;

    u4DemodBusId = SIF_BUS_TUNER_I2C; //default	
    DRVCUST_OptQuery(eDemodI2cBusID,&u4DemodBusId);
/*
    if (SIF_X_Write((UINT8)(u4DemodBusId), 270, i2cAddr, 1, regAddr, NULL, 0) == 0)
    {
        mcSHOW_HW_MSG(("%s : %02X-%02X!\n", __FUNCTION__, i2cAddr, regAddr));
        return 1;
    }
    if (SIF_X_Read((UINT8)(u4DemodBusId), 270, i2cAddr, 0, 0, data, len) == 0)
    {
        mcSHOW_HW_MSG(("%s : %02X-%02X!\n", __FUNCTION__, i2cAddr, regAddr));
        return 1;
    }
*/
    if (SIF_X_Read((UINT8)(u4DemodBusId), I2C_TIME, i2cAddr, 1, regAddr, data, len) == 0)
    {
        mcSHOW_HW_MSG(("%s : %02X-%02X!\n", __FUNCTION__, i2cAddr, regAddr));
        return 1;
    }

    return 0;
}


UINT8 Tuner_I2cWrite(UINT8 i2cAddr, UINT8 regAddr, UINT8 *data, UINT16 len)
{
UINT8 u1Ret;

    u1Ret = ICtrlBus_I2cDemodWrite(I2C_TIME, i2cAddr, regAddr, data, len); 
    return  u1Ret;   
}

UINT8 Tuner_RegDump(void)
{
    UINT8 ucData, u1Ret;
    UINT8 ii, jj;
   
    mcSHOW_USER_MSG(("RFIC Register Page Dump Result:\n"));
    mcSHOW_USER_MSG(("	 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));

    for (ii=0; ii<6; ii++)
    {
        mcSHOW_USER_MSG(("0x%02X ", ii));
        for (jj=0; jj<16; jj++)
        {
            u1Ret |= Tuner_I2cRead(RFIC_I2C_ADDR, ii*16+jj, &ucData, 1);
            mcSHOW_USER_MSG(("%02x ", ucData));
        }
        mcSHOW_USER_MSG(("\n"));
    }

    mcSHOW_USER_MSG(("RFIC Register Page Dump Result:\n"));
    mcSHOW_USER_MSG(("	 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"));

    for (ii=0; ii<6; ii++)
    {
        mcSHOW_USER_MSG(("0x%02X ", ii));
        for (jj=0; jj<16; jj++)
        {
            u1Ret |= Tuner_I2cRead(BBIC_I2C_ADDR, ii*16+jj, &ucData, 1);
            mcSHOW_USER_MSG(("%02x ", ucData));
        }
        mcSHOW_USER_MSG(("\n"));
    }	
    return FALSE;
}

//----------------------------------------------------------------------------- 
/** Tuner_HW_Reset
 *  Tuner VA4A1FB5010 hardware reset.
 *  @retval  TRUE: success
 */
//-----------------------------------------------------------------------------

UINT8 Tuner_HW_Reset(void)
{
    // HW Reset, L->H
    mcSHOW_USER_MSG(("Tuner HW Reset, GPIO14: L-->H!\n"));  
    GPIO_SetOut(14, 0);      // GPIO[14]
    mcDELAY_MS(50);         // According to the Spec, the Delay min is 50msec
    GPIO_SetOut(14, 1);
    mcDELAY_MS(50);         // I2c After Tuner Reset, the Delay min is 50msec 

    return TRUE;
}

//----------------------------------------------------------------------------- 
/** VA4A1FB5010_SetTunerSysMode
 *  Change the TV standard.
 *  @param  u1Modulation       the TV standard enum value.
 *  @retval  0 success
                 1  fail 
 */
//-----------------------------------------------------------------------------
UINT8 Tuner_RegInit(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8 fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RFIC and BBIC Init\n",__FUNCTION__));

    //INIT_RF 
    u1TblLen = sizeof(aru1INIT_RF)/sizeof(aru1INIT_RF[0]);
    mcSHOW_DBG_MSG3(("INIT_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1INIT_RF[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1INIT_RF[ii];
        u1Data = aru1INIT_RF[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //INIT_BB 
    u1TblLen = sizeof(aru1INIT_BB)/sizeof(aru1INIT_BB[0]);
    mcSHOW_DBG_MSG3(("INIT_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1INIT_BB[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1INIT_BB[ii];
        u1Data = aru1INIT_BB[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}

//----------------------------------------------------------------------------- 
/** Tuner_SetSysMode
 *  Change the TV standard.
 *  @param  u1Modulation       the TV standard enum value.
 *  @retval  0 success
                 1  fail 
 */
//-----------------------------------------------------------------------------
UINT8 API_SET_NTSC_181_25MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 181.25MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_A)/sizeof(aru1CHANGE_RF_A[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_A[ii];
        u1Data = aru1CHANGE_RF_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_A)/sizeof(aru1CHANGE_BB_A[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_A[ii];
        u1Data = aru1CHANGE_BB_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SET_ATV 
    u1TblLen = sizeof(aru1SET_ATV_A)/sizeof(aru1SET_ATV_A[0]);
    mcSHOW_DBG_MSG3(("SET_ATV RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SET_ATV_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SET_ATV_A[ii];
        u1Data = aru1SET_ATV_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_A)/sizeof(aru1PRE_SETUP_A[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_A[ii];
        u1Data = aru1PRE_SETUP_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_A)/sizeof(aru1RFF_PRECAL_A[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_A[ii];
        u1Data = aru1RFF_PRECAL_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_A)/sizeof(aru1RFF_CAL_A[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_A[ii];
        u1Data = aru1RFF_CAL_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_A)/sizeof(aru1PLL_SETUP_A[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_A[ii];
        u1Data = aru1PLL_SETUP_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_A)/sizeof(aru1AGC_CH_A[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_A[ii];
        u1Data = aru1AGC_CH_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_A)/sizeof(aru1PRE_SETUP_BB_A[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_A[ii];
        u1Data = aru1PRE_SETUP_BB_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //LOWIF_ATV_CH 
    u1TblLen = sizeof(aru1LOWIF_ATV_CH_A)/sizeof(aru1LOWIF_ATV_CH_A[0]);
    mcSHOW_DBG_MSG3(("LOWIF_ATV_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1LOWIF_ATV_CH_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1LOWIF_ATV_CH_A[ii];
        u1Data = aru1LOWIF_ATV_CH_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_A)/sizeof(aru1SFTRST_BB_ON_A[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_A[ii];
        u1Data = aru1SFTRST_BB_ON_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_A)/sizeof(aru1SFTRST_RF_ON_A[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_A[ii];
        u1Data = aru1SFTRST_RF_ON_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_A)/sizeof(aru1SFTRST_RF_OFF_A[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_A[ii];
        u1Data = aru1SFTRST_RF_OFF_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_A)/sizeof(aru1SFTRST_BB_OFF_A[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_A[ii];
        u1Data = aru1SFTRST_BB_OFF_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(30);
	
    //DCOC 
    u1TblLen = sizeof(aru1DCOC_A)/sizeof(aru1DCOC_A[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_A[ii];
        u1Data = aru1DCOC_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CVBS 
    u1TblLen = sizeof(aru1CVBS_A)/sizeof(aru1CVBS_A[0]);
    mcSHOW_DBG_MSG3(("CVBS RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CVBS_A[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CVBS_A[ii];
        u1Data = aru1CVBS_A[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}

UINT8 API_SET_ATSC_183MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 183MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_T)/sizeof(aru1CHANGE_RF_T[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_T[ii];
        u1Data = aru1CHANGE_RF_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_T)/sizeof(aru1CHANGE_BB_T[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_T[ii];
        u1Data = aru1CHANGE_BB_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_T)/sizeof(aru1PRE_SETUP_T[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_T[ii];
        u1Data = aru1PRE_SETUP_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_T)/sizeof(aru1RFF_PRECAL_T[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_T[ii];
        u1Data = aru1RFF_PRECAL_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_T)/sizeof(aru1RFF_CAL_T[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_T[ii];
        u1Data = aru1RFF_CAL_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_T)/sizeof(aru1PLL_SETUP_T[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_T[ii];
        u1Data = aru1PLL_SETUP_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_T)/sizeof(aru1AGC_CH_T[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_T[ii];
        u1Data = aru1AGC_CH_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_T)/sizeof(aru1PRE_SETUP_BB_T[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_T[ii];
        u1Data = aru1PRE_SETUP_BB_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DTV_MODE 
    u1TblLen = sizeof(aru1DTV_MODE_CH_T)/sizeof(aru1DTV_MODE_CH_T[0]);
    mcSHOW_DBG_MSG3(("DTV_MODE RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DTV_MODE_CH_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DTV_MODE_CH_T[ii];
        u1Data = aru1DTV_MODE_CH_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_T)/sizeof(aru1SFTRST_BB_ON_T[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_T[ii];
        u1Data = aru1SFTRST_BB_ON_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_T)/sizeof(aru1SFTRST_RF_ON_T[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_T[ii];
        u1Data = aru1SFTRST_RF_ON_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_T)/sizeof(aru1SFTRST_RF_OFF_T[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_T[ii];
        u1Data = aru1SFTRST_RF_OFF_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_T)/sizeof(aru1SFTRST_BB_OFF_T[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_T[ii];
        u1Data = aru1SFTRST_BB_OFF_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DCOC 
    u1TblLen = sizeof(aru1DCOC_T)/sizeof(aru1DCOC_T[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_T[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_T[ii];
        u1Data = aru1DCOC_T[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}

UINT8 API_SET_ATSC_551MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 551MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_T_551MHZ)/sizeof(aru1CHANGE_RF_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_T_551MHZ[ii];
        u1Data = aru1CHANGE_RF_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_T_551MHZ)/sizeof(aru1CHANGE_BB_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_T_551MHZ[ii];
        u1Data = aru1CHANGE_BB_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_T_551MHZ)/sizeof(aru1PRE_SETUP_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_T_551MHZ[ii];
        u1Data = aru1PRE_SETUP_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_T_551MHZ)/sizeof(aru1RFF_PRECAL_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_T_551MHZ[ii];
        u1Data = aru1RFF_PRECAL_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_T_551MHZ)/sizeof(aru1RFF_CAL_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_T_551MHZ[ii];
        u1Data = aru1RFF_CAL_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_T_551MHZ)/sizeof(aru1PLL_SETUP_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_T_551MHZ[ii];
        u1Data = aru1PLL_SETUP_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_T_551MHZ)/sizeof(aru1AGC_CH_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_T_551MHZ[ii];
        u1Data = aru1AGC_CH_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_T_551MHZ)/sizeof(aru1PRE_SETUP_BB_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_T_551MHZ[ii];
        u1Data = aru1PRE_SETUP_BB_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DTV_MODE 
    u1TblLen = sizeof(aru1DTV_MODE_CH_T_551MHZ)/sizeof(aru1DTV_MODE_CH_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("DTV_MODE RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DTV_MODE_CH_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DTV_MODE_CH_T_551MHZ[ii];
        u1Data = aru1DTV_MODE_CH_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_T_551MHZ)/sizeof(aru1SFTRST_BB_ON_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_T_551MHZ[ii];
        u1Data = aru1SFTRST_BB_ON_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_T_551MHZ)/sizeof(aru1SFTRST_RF_ON_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_T_551MHZ[ii];
        u1Data = aru1SFTRST_RF_ON_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_T_551MHZ)/sizeof(aru1SFTRST_RF_OFF_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_T_551MHZ[ii];
        u1Data = aru1SFTRST_RF_OFF_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_T_551MHZ)/sizeof(aru1SFTRST_BB_OFF_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_T_551MHZ[ii];
        u1Data = aru1SFTRST_BB_OFF_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DCOC 
    u1TblLen = sizeof(aru1DCOC_T_551MHZ)/sizeof(aru1DCOC_T_551MHZ[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_T_551MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_T_551MHZ[ii];
        u1Data = aru1DCOC_T_551MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}


UINT8 API_SET_ATSC_Cable_111MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 111MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_C_111MHZ)/sizeof(aru1CHANGE_RF_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_C_111MHZ[ii];
        u1Data = aru1CHANGE_RF_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_C_111MHZ)/sizeof(aru1CHANGE_BB_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_C_111MHZ[ii];
        u1Data = aru1CHANGE_BB_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_C_111MHZ)/sizeof(aru1PRE_SETUP_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_C_111MHZ[ii];
        u1Data = aru1PRE_SETUP_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_C_111MHZ)/sizeof(aru1RFF_PRECAL_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_C_111MHZ[ii];
        u1Data = aru1RFF_PRECAL_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_C_111MHZ)/sizeof(aru1RFF_CAL_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_C_111MHZ[ii];
        u1Data = aru1RFF_CAL_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_C_111MHZ)/sizeof(aru1PLL_SETUP_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_C_111MHZ[ii];
        u1Data = aru1PLL_SETUP_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_C_111MHZ)/sizeof(aru1AGC_CH_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_C_111MHZ[ii];
        u1Data = aru1AGC_CH_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_C_111MHZ)/sizeof(aru1PRE_SETUP_BB_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_C_111MHZ[ii];
        u1Data = aru1PRE_SETUP_BB_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DTV_MODE 
    u1TblLen = sizeof(aru1DTV_MODE_CH_C_111MHZ)/sizeof(aru1DTV_MODE_CH_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("DTV_MODE RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DTV_MODE_CH_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DTV_MODE_CH_C_111MHZ[ii];
        u1Data = aru1DTV_MODE_CH_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_C_111MHZ)/sizeof(aru1SFTRST_BB_ON_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_C_111MHZ[ii];
        u1Data = aru1SFTRST_BB_ON_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_C_111MHZ)/sizeof(aru1SFTRST_RF_ON_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_C_111MHZ[ii];
        u1Data = aru1SFTRST_RF_ON_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_C_111MHZ)/sizeof(aru1SFTRST_RF_OFF_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_C_111MHZ[ii];
        u1Data = aru1SFTRST_RF_OFF_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_C_111MHZ)/sizeof(aru1SFTRST_BB_OFF_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_C_111MHZ[ii];
        u1Data = aru1SFTRST_BB_OFF_C_111MHZ[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DCOC 
    u1TblLen = sizeof(aru1DCOC_C_111MHZ)/sizeof(aru1DCOC_C_111MHZ[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_C_111MHZ[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_C_111MHZ[ii];
        u1Data = aru1DCOC_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}

UINT8 API_SET_ATSC_Cable_183MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 183MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_C)/sizeof(aru1CHANGE_RF_C[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_C[ii];
        u1Data = aru1CHANGE_RF_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_C)/sizeof(aru1CHANGE_BB_C[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_C[ii];
        u1Data = aru1CHANGE_BB_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_C)/sizeof(aru1PRE_SETUP_C[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_C[ii];
        u1Data = aru1PRE_SETUP_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_C)/sizeof(aru1RFF_PRECAL_C[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_C[ii];
        u1Data = aru1RFF_PRECAL_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_C)/sizeof(aru1RFF_CAL_C[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_C[ii];
        u1Data = aru1RFF_CAL_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_C)/sizeof(aru1PLL_SETUP_C[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_C[ii];
        u1Data = aru1PLL_SETUP_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_C)/sizeof(aru1AGC_CH_C[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_C[ii];
        u1Data = aru1AGC_CH_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_C)/sizeof(aru1PRE_SETUP_BB_C[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_C[ii];
        u1Data = aru1PRE_SETUP_BB_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DTV_MODE 
    u1TblLen = sizeof(aru1DTV_MODE_CH_C)/sizeof(aru1DTV_MODE_CH_C[0]);
    mcSHOW_DBG_MSG3(("DTV_MODE RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DTV_MODE_CH_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DTV_MODE_CH_C[ii];
        u1Data = aru1DTV_MODE_CH_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_C)/sizeof(aru1SFTRST_BB_ON_C[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_C[ii];
        u1Data = aru1SFTRST_BB_ON_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_C)/sizeof(aru1SFTRST_RF_ON_C[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_C[ii];
        u1Data = aru1SFTRST_RF_ON_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_C)/sizeof(aru1SFTRST_RF_OFF_C[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_C[ii];
        u1Data = aru1SFTRST_RF_OFF_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_C)/sizeof(aru1SFTRST_BB_OFF_C[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_C[ii];
        u1Data = aru1SFTRST_BB_OFF_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DCOC 
    u1TblLen = sizeof(aru1DCOC_C)/sizeof(aru1DCOC_C[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_C[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_C[ii];
        u1Data = aru1DCOC_C[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}

UINT8 API_SET_DVBT_666MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 666MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_DVBT_666MHz)/sizeof(aru1CHANGE_RF_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_DVBT_666MHz[ii];
        u1Data = aru1CHANGE_RF_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_DVBT_666MHz)/sizeof(aru1CHANGE_BB_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_DVBT_666MHz[ii];
        u1Data = aru1CHANGE_BB_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_DVBT_666MHz)/sizeof(aru1PRE_SETUP_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_DVBT_666MHz[ii];
        u1Data = aru1PRE_SETUP_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_DVBT_666MHz)/sizeof(aru1RFF_PRECAL_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_DVBT_666MHz[ii];
        u1Data = aru1RFF_PRECAL_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_DVBT_666MHz)/sizeof(aru1RFF_CAL_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_DVBT_666MHz[ii];
        u1Data = aru1RFF_CAL_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_DVBT_666MHz)/sizeof(aru1PLL_SETUP_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_DVBT_666MHz[ii];
        u1Data = aru1PLL_SETUP_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_DVBT_666MHz)/sizeof(aru1AGC_CH_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_DVBT_666MHz[ii];
        u1Data = aru1AGC_CH_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_DVBT_666MHz)/sizeof(aru1PRE_SETUP_BB_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_DVBT_666MHz[ii];
        u1Data = aru1PRE_SETUP_BB_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DTV_MODE 
    u1TblLen = sizeof(aru1DTV_MODE_CH_DVBT_666MHz)/sizeof(aru1DTV_MODE_CH_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("DTV_MODE RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DTV_MODE_CH_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DTV_MODE_CH_DVBT_666MHz[ii];
        u1Data = aru1DTV_MODE_CH_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_DVBT_666MHz)/sizeof(aru1SFTRST_BB_ON_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_DVBT_666MHz[ii];
        u1Data = aru1SFTRST_BB_ON_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_DVBT_666MHz)/sizeof(aru1SFTRST_RF_ON_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_DVBT_666MHz[ii];
        u1Data = aru1SFTRST_RF_ON_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_DVBT_666MHz)/sizeof(aru1SFTRST_RF_OFF_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_DVBT_666MHz[ii];
        u1Data = aru1SFTRST_RF_OFF_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_DVBT_666MHz)/sizeof(aru1SFTRST_BB_OFF_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_DVBT_666MHz[ii];
        u1Data = aru1SFTRST_BB_OFF_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DCOC 
    u1TblLen = sizeof(aru1DCOC_DVBT_666MHz)/sizeof(aru1DCOC_DVBT_666MHz[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_DVBT_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_DVBT_666MHz[ii];
        u1Data = aru1DCOC_DVBT_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}



UINT8 API_SET_DVBC_666MHz(void)
{
UINT8 u1TblLen;
UINT8 u1Reg, u1Data, u1I2cAddr;
UINT8 ii;
UINT8  fgRet;

    fgRet = 0;

    mcSHOW_DBG_MSG(("%s, RF = 666MHz\n",__FUNCTION__));

    //CHANGE_RF 
    u1TblLen = sizeof(aru1CHANGE_RF_DVBC_666MHz)/sizeof(aru1CHANGE_RF_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("CHANGE_RF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_RF_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_RF_DVBC_666MHz[ii];
        u1Data = aru1CHANGE_RF_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //CHANGE_BB 
    u1TblLen = sizeof(aru1CHANGE_BB_DVBC_666MHz)/sizeof(aru1CHANGE_BB_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("CHANGE_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1CHANGE_BB_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1CHANGE_BB_DVBC_666MHz[ii];
        u1Data = aru1CHANGE_BB_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP 
    u1TblLen = sizeof(aru1PRE_SETUP_DVBC_666MHz)/sizeof(aru1PRE_SETUP_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_DVBC_666MHz[ii];
        u1Data = aru1PRE_SETUP_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_PRECAL 
    u1TblLen = sizeof(aru1RFF_PRECAL_DVBC_666MHz)/sizeof(aru1RFF_PRECAL_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("RFF_PRECAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_PRECAL_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_PRECAL_DVBC_666MHz[ii];
        u1Data = aru1RFF_PRECAL_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //RFF_CAL 
    u1TblLen = sizeof(aru1RFF_CAL_DVBC_666MHz)/sizeof(aru1RFF_CAL_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("RFF_CAL RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1RFF_CAL_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1RFF_CAL_DVBC_666MHz[ii];
        u1Data = aru1RFF_CAL_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(20);
	
    //PLL_SETUP 
    u1TblLen = sizeof(aru1PLL_SETUP_DVBC_666MHz)/sizeof(aru1PLL_SETUP_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("PLL_SETUP RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PLL_SETUP_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PLL_SETUP_DVBC_666MHz[ii];
        u1Data = aru1PLL_SETUP_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(50);
	
    //AGC_CH 
    u1TblLen = sizeof(aru1AGC_CH_DVBC_666MHz)/sizeof(aru1AGC_CH_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("AGC_CH RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1AGC_CH_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1AGC_CH_DVBC_666MHz[ii];
        u1Data = aru1AGC_CH_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //PRE_SETUP_BB 
    u1TblLen = sizeof(aru1PRE_SETUP_BB_DVBC_666MHz)/sizeof(aru1PRE_SETUP_BB_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1PRE_SETUP_BB_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1PRE_SETUP_BB_DVBC_666MHz[ii];
        u1Data = aru1PRE_SETUP_BB_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DTV_MODE 
    u1TblLen = sizeof(aru1DTV_MODE_CH_DVBC_666MHz)/sizeof(aru1DTV_MODE_CH_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("DTV_MODE RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DTV_MODE_CH_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DTV_MODE_CH_DVBC_666MHz[ii];
        u1Data = aru1DTV_MODE_CH_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_ON 
    u1TblLen = sizeof(aru1SFTRST_BB_ON_DVBC_666MHz)/sizeof(aru1SFTRST_BB_ON_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_ON RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_ON_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_ON_DVBC_666MHz[ii];
        u1Data = aru1SFTRST_BB_ON_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_ON 
    u1TblLen = sizeof(aru1SFTRST_RF_ON_DVBC_666MHz)/sizeof(aru1SFTRST_RF_ON_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("PRE_SETUP_BB RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_ON_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_ON_DVBC_666MHz[ii];
        u1Data = aru1SFTRST_RF_ON_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_RF_OFF 
    u1TblLen = sizeof(aru1SFTRST_RF_OFF_DVBC_666MHz)/sizeof(aru1SFTRST_RF_OFF_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("SFTRST_RF_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_RF_OFF_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_RF_OFF_DVBC_666MHz[ii];
        u1Data = aru1SFTRST_RF_OFF_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //SFTRST_BB_OFF 
    u1TblLen = sizeof(aru1SFTRST_BB_OFF_DVBC_666MHz)/sizeof(aru1SFTRST_BB_OFF_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("SFTRST_BB_OFF RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1SFTRST_BB_OFF_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1SFTRST_BB_OFF_DVBC_666MHz[ii];
        u1Data = aru1SFTRST_BB_OFF_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    //DCOC 
    u1TblLen = sizeof(aru1DCOC_DVBC_666MHz)/sizeof(aru1DCOC_DVBC_666MHz[0]);
    mcSHOW_DBG_MSG3(("DCOC RegNum = %d \n", u1TblLen-1));
    u1I2cAddr = aru1DCOC_DVBC_666MHz[0];
    for(ii = 1; ii < u1TblLen;)
    {
        u1Reg = aru1DCOC_DVBC_666MHz[ii];
        u1Data = aru1DCOC_DVBC_666MHz[ii+1];
	 fgRet |= Tuner_I2cWrite( u1I2cAddr, u1Reg, &u1Data, 1);
	 mcSHOW_DBG_MSG3(("RegAddr: 0x%2x = 0x%2x\n", u1Reg, u1Data));
	 ii +=2;
    }

    mcDELAY_MS(10);
    return fgRet;
}


