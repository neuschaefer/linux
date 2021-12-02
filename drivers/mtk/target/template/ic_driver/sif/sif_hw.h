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
 * $RCSfile: sif_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sif_hw.h
 *  Define SIF hardware information.
 */

#ifndef SIF_HW_H

#define SIF_HW_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
/// SM0CTL0_REG: Serial interface master 0 control 0 register. 
#define RW_SM0_CTRL0_REG             ((UINT32)0x0240)

/// SM0EN: Serial interface master 0 enable. 
#define SM0EN                          ((UINT32)1 << 1)
/// BIDA:	Serial interface uni-direction pin selection.
#define BIDA                              ((UINT32)1 << 2)
/// SIN: Serial interface data in option in uni-direction mode.
#define SIN                              ((UINT32)1 << 3)
/// HD1T: tDH = 1T @ ACK, STOP1, CHANGE state.
#define HD1T                            ((UINT32)1 << 4)
/// PAD selection from OSDA0 or TUNER_DATA
#define PSEL                            ((UINT32)1 << 5)
/// #bytes of word address
#define WORD_LEN(x)            ((((UINT32)(x)) & 0x3) << 6)
/// HDMIA: Internal HDMI Address Select.
#define HDMIA                       ((UINT32)1 << 8)
/// AS_HDMI: Auto Select internal HDMI if Device address is equal to internal HDMI.
#define AS_HDMI                       ((UINT32)1 << 9)
/// FS_HDMI: Force SIF master to communicate internal HDMI only.
#define FS_HDMI                        ((UINT32)1 << 10)
/// CLK_DIV: Parameter of divider to divide 27M for SCL.
#define SIFM_OP                        ((UINT32)1 << 31)
#define CLK_DIV(x)                    ((((UINT32)(x)) & 0x0FFF) << 16)

/// SM0CTL1_REG: Serial interface master 0 control 1 register. 
#define RW_SM0_CTRL1_REG           ((UINT32)0x0244)

/// TRI: Trigger serial interface. Read back as serial interface busy.
#define TRI                           ((UINT32)1 << 0)
/// WR: Write cycle.
#define WR                           ((UINT32)1 << 1)
/// CURAR: Current address read or random read.
#define CURAR                         ((UINT32)1 << 2)
/// PGLEN: Page length of sequential read/write. The maximum is 8 bytes. Set 0 as 1 bytes.
#define PGLEN(x)                      ((((UINT32)(x)) & 0x0007) << 8)
#ifdef CC_MT5391
/// SIF_MODE: start ==>data==>stop, start==>data, data==>stop, only data
#define SIF_MODE(x)                      ((((UINT32)(x)) & 0x0003) << 4)
#define SIF_MODE_MASK                ((UINT32)0x30)
#endif
/// ACK: 	Acknowledge bits. 
#define ACK(x)                    ((((UINT32)(x)) & 0x03FF0000) >> 16)

/// SM0DEV_REG: Serial interface master 0 device address register. 
#define RW_SM0_DEV_REG               ((UINT32)0x0248)

/// SM0WD_REG: Serial interface master 0 word address register. 
#define RW_SM0_WD_REG                  ((UINT32)0x024C)

/// SM0D0_REG: Serial interface master 0 data port 0 register. 
#define RW_SM0_D0_REG               ((UINT32)0x0250)

/// SM0D1_REG: Serial interface master 0 data port 1 register. 
#define RW_SM0_D1_REG                ((UINT32)0x0254)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
/*
typedef struct _SIFDEVPARAM_T
{
    UINT16 u2ClkDiv;  ///< parameter of divider to divide 27 Mhz for SCL. 12 bits width.
    UINT8 u1DevAddr;  ///< device address: 7 bits width.
    UINT32 u4WordAddrNum; ///< number of word address:  2 bits width, 0, 1, 2, 3.
    UINT32 u4WordAddr; ///< word address:  24 bits width.
    UINT8 *pu1Buf;    ///< point to user's buffer.
    UINT16 u2ByteCnt; ///< the data length that user want to read/write.
    UINT16 u2ProcCnt; ///< the data length that SIF read/write completely.
    BOOL fgTuner;       ///< output pins are TUNER_DATA & TUNER_CLK.
    BOOL fgNonStart;   //<send start bit or not> only in 5391
    BOOL fgNonStop;     //<send stop bit or not> only in 5391
} SIFDEVPARAM_T;
*/
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif // SIF_HW_H
