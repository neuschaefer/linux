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
/** @file sdr.c
 *  SDR setting.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"

#include "ddr.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

#ifdef CC_SUPPORT_SDR
void _DDR_SetSdrBeforeAI(void)
{
    UINT32 u4Val;
    UINT32 u4CL;
    UINT32 u4Bits = (IS_FORCE32() ? 32 : 64);
    
    u4CL = (UINT32)TCMGET_DDR_CL();
    /* 
        Note: DDR1, and SDR real CL value = u4CL/2.
        Program mapping CL= 2, 2.5, 3 to u4CL = 4, 5, 6.
    */
    ASSERT(u4CL == 6);

    // Set DRAM configuration.
    // 0x7018 default: No 8BKEN, No 64BITEN, SIOEN, DQ4BMUX, SDR pinmux.
    // Set MATYPE: column address bits = 8.
    DRAM_WRITE32(0x18, 0xE002001A);

    // Set SDR configuration.
    DRAM_WRITE32(0x28, 0xA0540000);

    // Set DQSISEL.
    DRAM_WRITE32(0x390, 0xF0F0);

    // Set clock duty cycle.
    DRAM_WRITE32(0x348, SDR_OUT_CLK_DELAY_CHAIN_0);
    DRAM_WRITE32(0x34C, SDR_OUT_CLK_DELAY_CHAIN_1);

    // Set DQ input per bit delay.
#ifndef DRAM_DQINPUTPERBITCALI    
    DRAM_WRITE32(0x3B0, SDR_IN_DQ_DELAY_CHAIN_0);
    DRAM_WRITE32(0x3B4, SDR_IN_DQ_DELAY_CHAIN_1);
    DRAM_WRITE32(0x3B8, SDR_IN_DQ_DELAY_CHAIN_2);
    DRAM_WRITE32(0x3BC, SDR_IN_DQ_DELAY_CHAIN_3);
    DRAM_WRITE32(0x3C0, SDR_IN_DQ_DELAY_CHAIN_4);
    DRAM_WRITE32(0x3C4, SDR_IN_DQ_DELAY_CHAIN_5);
    DRAM_WRITE32(0x3C8, SDR_IN_DQ_DELAY_CHAIN_6);
    DRAM_WRITE32(0x3CC, SDR_IN_DQ_DELAY_CHAIN_7);
#endif

    // Set pad driving.
    DRAM_WRITE32(0x300, ((UINT32)SDR_CLK_DRIVING<<24));
    DRAM_WRITE32(0x308, ((UINT32)SDR_CMD_DRIVING<<24));
    DRAM_WRITE32(0x310, 0);
    DRAM_WRITE32(0x318, ((UINT32)SDR_DQ_DRIVING<<24));

    // Set power saving
    DRAM_WRITE32(0x318, (DRAM_READ32(0x318) | 0x40));    

    // Set RDSEL duty cycle control.
    DRAM_WRITE32(0x39C, SDR_DQ_RDSEL);

    // Set Manual DQS mode.
    DRAM_WRITE32(0x234, 0x00010000);
    DRAM_WRITE32(0x230, 0x40000000);

    // Set MRS: BL8.   
    u4Val = 0x00000003;  

    // Set CAS latency. Program mapping CL= 2, 2.5, 3 to u4CL = 4, 5, 6.
    if (u4CL == 4)
    {
        u4Val |= 0x20; // CL = 2. 
    }
    else if (u4CL == 5)
    {
        u4Val |= 0x60;  // CL = 2.5. 
    }
    else // if (u4CL == 6)
    {
        u4Val |= 0x30;     // CL = 3. 
    }

    DRAM_WRITE32(0x10, u4Val);

    // Set CAS, RAS, WE, ADDR, CS delay: setup/hold time.
    DRAM_WRITE32(0x320, SDR_CMD_DELAY_0);
    DRAM_WRITE32(0x324, SDR_CMD_DELAY_1);
    DRAM_WRITE32(0x328, SDR_CMD_DELAY_2);
    DRAM_WRITE32(0x32C, SDR_CMD_DELAY_3);
    DRAM_WRITE32(0x330, SDR_CMD_DELAY_4);
    DRAM_WRITE32(0x334, SDR_CMD_DELAY_5);

    // Set clock delay.
    DRAM_WRITE32(0x304, SDR_CLK_DELAY);

    // Set DQ output delay.
    DRAM_WRITE32(0x360, SDR_DQ_OUTPUT_DELAY_0);
    DRAM_WRITE32(0x364, SDR_DQ_OUTPUT_DELAY_1);
    DRAM_WRITE32(0x368, SDR_DQ_OUTPUT_DELAY_2);
    DRAM_WRITE32(0x36C, SDR_DQ_OUTPUT_DELAY_3);
 
    // Reset DDR PHY control, but do not auto init.
    DRAM_WRITE32(0x224, 0x3);
    CHIP_Delay_us(1000);
    DRAM_WRITE32(0x224, 0x0);
    CHIP_Delay_us(1000);
    
    // Set clock gating window.
    // CAS latency. Program mapping CL= 2, 2.5, 3 to u4CL = 4, 5, 6.
    if (u4CL == 4) //CL = 2.
    {        
        DRAM_WRITE32(0x388, SDR_DQSIEN);
        DRAM_WRITE32(0x380, 0x00000000);
        DRAM_WRITE32(0x384, 0x00000000);        
    }
    else if (u4CL == 5) //CL = 2.5.
    {
        DRAM_WRITE32(0x388, SDR_DQSIEN);
        DRAM_WRITE32(0x380, 0x00010001);
        DRAM_WRITE32(0x384, 0x00010001);
    }
    else // if (u4CL == 6) //CL = 3.
    {
        DRAM_WRITE32(0x388, SDR_DQSIEN);
        DRAM_WRITE32(0x380, 0x00020002);
        DRAM_WRITE32(0x384, 0x00020002);
    }

    // Enable read ODT.
    //DRAM_WRITE32(0x33C, 0x00070000);
    // Disable read ODT.
    DRAM_WRITE32(0x33C, 0x00000000);

    /*
        64Mbit DRAM 166 Mhz => 6 ns.

        TCK = 10ns for CL2.
        TRCD = 24 ns, TRP = 18ns, TFAW = Only DDR2 define, TWR = 3*TCK = 30ns.
        TWTR = 1TCK = 10ns, TRC = 66ns, TRAS = 48ns.        
    */
    DRAM_WRITE32(0x00, 0x43064365);  

    // TRRD = 12ns, TRFC = 66ns. 
    // Set TRFC[4] = 1: Value = 21 + TRFC[3:0].
    // Set TRFC[4] = 0: Value = 8 + TRFC[3:0].
    DRAM_WRITE32(0x04, 0x05029A04);

    // Set MACROSDR enable, MEMPLL power down.
    DRAM_WRITE32(0x344, 0x0000D520);
    // Set MEMPLL full frequency mode, MEMPLL power on.
   // DRAM_WRITE32(0x344, 0x00005590);    
}

void _DDR_SetSdrAfterAI(void)
{
    _DDR_SetRTT(DRVCUST_InitGet(eFlagReadODT));
}
#endif //CC_SUPPORT_SDR

