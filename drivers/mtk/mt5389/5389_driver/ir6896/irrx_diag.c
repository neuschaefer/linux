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
 * $RCSfile: irrx_diag.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file irrx_diag.c
 *  irrx_diag.c provides IRRX/IRTX register access functions.
 */

#ifdef CC_CLI

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

// local header files
#include "ir_debug.h"
#include "ir_if.h"
#include "drvcust_if.h"

#include "x_bim.h"
#include "x_os.h"
#include "x_util.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_lint.h"
#include "x_printf.h"
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define IR_BASE         BIM_BASE
#define IR_REG_LENGTH   0

// The maximum of diagnostic function retry count
#define DIAG_RETRY  10



//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

REG_TEST_T arIRRgtList[] = {
    // IR RX
    { 0x0200, eRD_ONLY, 4, 0xffffff3f, 0, 0 },
    { 0x0204, eRD_ONLY, 4, 0xffffffff, 0, 0 },
    { 0x0208, eRD_ONLY, 4, 0x00ffffff, 0, 0 },
    { 0x020c, eRD_WR,   2, 0x0ff7, 1, 0 },
    { 0x0210, eRD_WR,   2, 0x00ff, 1, 0xff },
    // { 0x0214, eRD_WR,    2, 0x00ff, 1, 0 },
    { 0x0214, eRD_WR,   2, 0x037f, 1, 0 },

    // IR TX
    { 0x0224, eRD_WR,   4, 0xffffffff, 1, 0 },
    { 0x0228, eRD_WR,   4, 0xffffffff, 1, 0 },
    { 0x022c, eRD_WR,   4, 0xffffffff, 1, 0 },
    { 0x0230, eRD_WR,   4, 0xffffffff, 1, 0x10083b4c },
    { 0x0234, eRD_WR,   4, 0xffffffff, 1, 0x00e602b2 },
    /* IR TX have STRT at bit 0 of 0x8220, it will make data register
     * halting for a moment. */
    { 0x0220, eRD_WR,   2, 0x7ffe, 1, 0 }, /* we don't have to test bit 0,
                                              it's trigger bit. */

    // End.
    { 0xffff, eNO_TYPE, -1, 0, 0, 0 },
};


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 IR_Diag()
{
    INT32 i4Ret, i4Val, i4Try;
    UINT32 u4Key, u4TxData;
    INT32 i4TxConfig, i4TxModulation;
    IRTXWAVE_T rWave;
    IRRX_RAW_DATA_T rRaw;

    i4Val = 0;
    i4Ret = 0;
#if 0
    // i4Ret = UTIL_RegDefChk(IR_BASE, arIRRgtList);
    // i4Val |= i4Ret;
    Printf("Register default value check .............. %s\n",
                    (i4Ret ? "FAIL" : "PASS"));
    i4Ret = UTIL_RegRWTest(IR_BASE, arIRRgtList);
    i4Val |= i4Ret;
    Printf("Register read/write test .................. %s\n",
                    (i4Ret ? "FAIL" : "PASS"));
    i4Ret = UTIL_AllSpaceRWTest(IR_BASE, IR_REG_LENGTH);
    i4Val |= i4Ret;
    Printf("Memory space read/write test .............. %s\n",
                    (i4Ret ? "FAIL" : "PASS"));
#endif
    VERIFY(IRRX_InitMtkIr(IRRX_NEC_TYPE)==IR_SUCC);
    Printf("\n\nPlease use MTK TV remote controller, and ");
    Printf("follow the required key to press button\n");

    IRHW_TxRdConf(&i4TxConfig, &rWave, &i4TxModulation);
    i4TxConfig = 0x90;
    rWave.u4SyncHigh = 0x3b4c0;
    rWave.u4SyncLow = 0x1da60;
    rWave.u4Data0High = 0x3b10;
    rWave.u4Data0Low = 0x3b10;
    rWave.u4Data1High = 0x3b10;
    rWave.u4Data1Low = 0xb23e;
    // i4TxModulation is not changed.
    IRHW_TxWrConf(i4TxConfig, &rWave, i4TxModulation);

    /**************************** Power Key **********************************/
    Printf("[ Power Key ]\n");
    i4Try = 0;

    do
    {
        u4TxData = 0xf10e9f20;
        IRHW_TxSendData((const INT32 *)&u4TxData, 33);
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
        if (u4Key!=BTN_NONE) { i4Try++; }
    } while ((i4Ret==IR_SUCC) &&
                    (u4Key != BTN_POWER) &&
                    (i4Try < DIAG_RETRY));

    Printf("[ Power Key ] ............................. %s\n",
                (((i4Ret==IR_SUCC) && (u4Key==BTN_POWER)) ? "PASS" : "FAIL"));

    i4Val |= (INT32)!(u4Key==BTN_POWER);

    do
    {
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
    } while ((i4Ret==IR_SUCC) && (u4Key != BTN_NONE));

    /**************************** Dig 1 Key **********************************/
    Printf("[ Dig 1 Key ]\n");
    i4Try = 0;
    do
    {
        u4TxData = 0xec139f20;
        IRHW_TxSendData((const INT32 *)&u4TxData, 33);
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
        if (u4Key!=BTN_NONE) { i4Try++; }
    } while ((i4Ret==IR_SUCC) &&
                    (u4Key != BTN_DIGIT_1) &&
                    (i4Try < DIAG_RETRY));

    Printf("[ Dig 1 Key ] ............................. %s\n",
            (((i4Ret==IR_SUCC) && (u4Key==BTN_DIGIT_1)) ? "PASS" : "FAIL"));

    i4Val |= (INT32)!(u4Key==BTN_DIGIT_1);
    do
    {
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
    } while ((i4Ret==IR_SUCC) && (u4Key != BTN_NONE));

    /**************************** Dig 2 Key **********************************/
    Printf("[ Dig 2 Key ]\n");
    i4Try = 0;
    do {
        u4TxData = 0xef109f20;
        IRHW_TxSendData((const INT32 *)&u4TxData, 33);
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
        if (u4Key!=BTN_NONE) { i4Try++; }
    } while ((i4Ret==IR_SUCC) &&
                    (u4Key != BTN_DIGIT_2) &&
                    (i4Try < DIAG_RETRY));

    Printf("[ Dig 2 Key ] ............................. %s\n",
            (((i4Ret==IR_SUCC) && (u4Key==BTN_DIGIT_2)) ? "PASS" : "FAIL"));

    i4Val |= (INT32)!(u4Key==BTN_DIGIT_2);

    do
    {
        i4Ret = IRRX_PollMtkIr(&u4Key, &rRaw);
    } while ((i4Ret==IR_SUCC) && (u4Key != BTN_NONE));

    return i4Val;
}

#endif /* CC_CLI */
