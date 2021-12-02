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
/** @file ddr2.c
 *  DDR2 setting.
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

#ifdef CC_SUPPORT_DDR2
void _DDR_SetDdr2BeforeAI(void)
{
    UINT32 u4Val;
    UINT32 u4MRS = 0;
    UINT32 u4ACTIM1 = 0;
    UINT32 u4CL;
       
    u4CL = (UINT32)TCMGET_DDR_CL();

    // 0x7018 default: No 8BKEN, No 64BITEN, FDIV2, SIOEN, DQ4BMUX, DDR2 pinmux.
    u4Val = 0xE0121050;
    
    // Set 8BKEN.
    if (DRVCUST_InitGet(eFlag1GBitSupport))
    {
        u4Val |= 0x4000;
    }

    // Set SIOEN and EMRS single end DQS support.
    if (IS_SINGLEDQS())
    {
        u4MRS = 0x04000000;
        u4Val |= 0x10;
    }
    else
    {
        u4Val &= ~0x10;
    }

    // Set 64BITEN.
    if (!IS_FORCE32())
    {
        u4Val |= 0x400;
    }

    // Set DRAM configuration.
    DRAM_WRITE32(0x18, u4Val);

    // Set DQSISEL.
    DRAM_WRITE32(0x390, 0);

    // Set clock duty cycle.
    DRAM_WRITE32(0x348, DDR2_OUT_CLK_DELAY_CHAIN_0);
    DRAM_WRITE32(0x34C, DDR2_OUT_CLK_DELAY_CHAIN_1);

    // Set DQ input per bit delay.
#ifndef DRAM_DQINPUTPERBITCALI    
    DRAM_WRITE32(0x3B0, DDR2_IN_DQ_DELAY_CHAIN_0);
    DRAM_WRITE32(0x3B4, DDR2_IN_DQ_DELAY_CHAIN_1);
    DRAM_WRITE32(0x3B8, DDR2_IN_DQ_DELAY_CHAIN_2);
    DRAM_WRITE32(0x3BC, DDR2_IN_DQ_DELAY_CHAIN_3);
    DRAM_WRITE32(0x3C0, DDR2_IN_DQ_DELAY_CHAIN_4);
    DRAM_WRITE32(0x3C4, DDR2_IN_DQ_DELAY_CHAIN_5);
    DRAM_WRITE32(0x3C8, DDR2_IN_DQ_DELAY_CHAIN_6);
    DRAM_WRITE32(0x3CC, DDR2_IN_DQ_DELAY_CHAIN_7);
#endif

    // Set pad driving.
    DRAM_WRITE32(0x300, ((UINT32)DDR2_CLK_DRIVING<<24) | 0x80);
    DRAM_WRITE32(0x308, ((UINT32)DDR2_CMD_DRIVING<<24) | 0x80);
    DRAM_WRITE32(0x310, ((UINT32)DDR2_DQS_DRIVING<<24) | 0x80);
    DRAM_WRITE32(0x318, ((UINT32)DDR2_DQ_DRIVING<<24) | 0x80);

    // Set RDSEL duty cycle control.
    DRAM_WRITE32(0x398, DDR2_DQS_RDSEL);
    DRAM_WRITE32(0x39C, DDR2_DQ_RDSEL);

    // Set Manual DQS mode.
    DRAM_WRITE32(0x234, 0x00010000);
    DRAM_WRITE32(0x230, 0x40000000);

    // Write ODT setting.
    switch (DRVCUST_InitGet(eFlagWriteODT))
    {
        case 0: // Disable
            u4MRS |= 0x00000000;
            break;
        case 75: // 75Ohm
            u4MRS |= 0x00040000;
            break;
        case 150: // 15Ohm
            u4MRS |= 0x00400000;
            break;
        default: 
            u4MRS |= 0x00440000; // Default as 50Ohm
            break;            
    }

    // Set burst length = 8.
    u4MRS |= 0x3;

    // Set CAS latency.    
    if (u4CL == 7)
    {
        u4MRS |= 0x70;
    }
    else //if (u4CL == 6)
    {
        u4MRS |= 0x60;
    }

    // Set reduce strength mode and WR = 6.
    u4MRS |= 0x00020A00;    
    DRAM_WRITE32(0x10, u4MRS);

    // Set CAS, RAS, WE, ADDR, CS delay: setup/hold time.
    DRAM_WRITE32(0x320, DDR2_CMD_DELAY_0);
    DRAM_WRITE32(0x324, DDR2_CMD_DELAY_1);
    DRAM_WRITE32(0x328, DDR2_CMD_DELAY_2);
    DRAM_WRITE32(0x32C, DDR2_CMD_DELAY_3);
    DRAM_WRITE32(0x330, DDR2_CMD_DELAY_4);
    DRAM_WRITE32(0x334, DDR2_CMD_DELAY_5);

    // Set clock delay.
    DRAM_WRITE32(0x304, DDR2_CLK_DELAY);

    // Set DQ output delay.
    DRAM_WRITE32(0x360, DDR2_DQ_OUTPUT_DELAY_0);
    DRAM_WRITE32(0x364, DDR2_DQ_OUTPUT_DELAY_1);
    DRAM_WRITE32(0x368, DDR2_DQ_OUTPUT_DELAY_2);
    DRAM_WRITE32(0x36C, DDR2_DQ_OUTPUT_DELAY_3);

    // Set DQS output delay.    
    DRAM_WRITE32(0x354, DDR2_DQS_DELAY);

    // Reset DDR PHY control, but do not auto init.
    DRAM_WRITE32(0x224, 0x3);
    CHIP_Delay_us(1000);
    DRAM_WRITE32(0x224, 0x0);
    CHIP_Delay_us(1000);
      
    // Set DQS gating window.
    if (u4CL == 7)
    {        
        DRAM_WRITE32(0x388, DDR2_DQSIEN);
        DRAM_WRITE32(0x380, 0x04000400);
        DRAM_WRITE32(0x384, 0x04000400);        
    }
    else //if (u4CL == 6)
    {
        DRAM_WRITE32(0x388, DDR2_DQSIEN);
        DRAM_WRITE32(0x380, 0x01000100);
        DRAM_WRITE32(0x384, 0x01000100);
    }

    if (IS_DDR_QFP())
    {
        // WODT ON when reading.
        u4Val = 0x002D00F3;
    }
    else
    {
        u4Val = 0x002D00F1;
    }

    if (u4CL == 7)
    {
        u4Val = (u4Val & 0xFFF7FFFB) | 0x4;

        u4ACTIM1= 0x0652330F;
    }
    else if (u4CL == 6)
    {
        u4ACTIM1= 0x0542330F;
    }
    DRAM_WRITE32(0x33C, u4Val);
    DRAM_WRITE32(0x04, u4ACTIM1);   

    /* 
        TFAW spec value:
        2KB 45 ns.
        
        TRFC spec value:
        256Mb, 75ns
        512Mb, 105ns
        1Gb, 127.5ns
        2Gb, 197.5ns
    */
    DRAM_WRITE32(0x00, 0x33A84487);

    // Set MACRODDROE = 1 for DDR1, DDR2, MCKMOD = 1, CLKJOPT = 1.
    DRAM_WRITE32(0x344, 0x00182150);
}

void _DDR_SetDdr2AfterAI(void)
{
    _DDR_SetRTT(DRVCUST_InitGet(eFlagReadODT));
}
#endif //CC_SUPPORT_DDR2

