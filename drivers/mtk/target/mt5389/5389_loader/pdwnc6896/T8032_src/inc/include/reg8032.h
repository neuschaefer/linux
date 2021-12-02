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
/**
 * @file reg8032.h
 *
 * Extended register setting for 8032
 *
 * @author Mtk00469
 */

#ifndef _REG52_EXT_H_
#define _REG52_EXT_H_

#include <reg52.h>

sbit P10 = P1 ^ 0;
sbit P11 = P1 ^ 1;
sbit P12 = P1 ^ 2;
sbit P13 = P1 ^ 3;
sbit P14 = P1 ^ 4;
sbit P15 = P1 ^ 5;
sbit P16 = P1 ^ 6;
sbit P17 = P1 ^ 7;

sbit P30 = P3 ^ 0;
sbit P31 = P3 ^ 1;
sbit P32 = P3 ^ 2;

sbit P34 = P3 ^ 4;
sbit P35 = P3 ^ 5;

sfr TA = 0xc7;
sfr CKCON = 0x8E;
sfr WDCON  = 0xd8;
sfr EIE  = 0xe8;

/*Bit register*/
sbit RWT = WDCON ^ 0;
sbit EWT = WDCON ^ 1;
sbit WTRF = WDCON ^ 2;
sbit WDIF = WDCON ^ 3;
sbit PFI = WDCON ^ 4;
sbit EPFI = WDCON ^ 5;
sbit POR = WDCON ^ 6;

sbit EWDI = EIE ^ 4;

/*=====================================================================
  Define Constants about 8032 TMOD
=====================================================================*/
#define TM0MODE0           0x00
#define TM0MODE1           0x01
#define TM0MODE2           0x02
#define TM0MODE3           0x03
#define TM0COUNTER         0x04
#define TM0TIMER           0x00
#define TM0GATE_ENABLE     0x00
#define TM0GATE_DISABLE    0x08

#define TM1MODE0           0x00
#define TM1MODE1           0x10
#define TM1MODE2           0x20
#define TM1MODE3           0x30
#define TM1COUNTER         0x40
#define TM1TIMER           0x00
#define TM1GATE_ENABLE     0x00
#define TM1GATE_DISABLE    0x80

#define TM2COUNTER         0x02
#define TM2TIMER           0x00
#define TM2MODE0           0x00	// Auto load
#define TM2MODE1           0x01	// Capture

#define INT0_LEVEL         0x00
#define INT0_EDGE          0x01
#define INT1_LEVEL         0x00
#define INT1_EDGE          0x04

#endif
