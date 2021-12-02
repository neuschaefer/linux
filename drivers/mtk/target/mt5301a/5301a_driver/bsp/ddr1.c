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
/** @file ddr1.c
 *  DDR1 setting.
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

#ifdef CC_SUPPORT_DDR1
void _DDR_SetDdr1BeforeAI(void)
{
    UINT32 u4Val;
    UINT32 u4CL;
    UINT32 u4Bits = (IS_FORCE32() ? 32 : 64);
    
    u4CL = (UINT32)TCMGET_DDR_CL();
    /* 
        Note: DDR1, and SDR real CL value = u4CL/2.
        Program mapping CL= 2, 2.5, 3 to u4CL = 4, 5, 6.
    */

    // Set full frequency mode.
    if (IS_FULLFREQ())
    {
        // Set R_DM64BITEN, R_DMFREQDIV2, R_DM16BITFULL = {0, 1, 1} when 16 bits DRAM.
        // Set R_DM64BITEN, R_DMFREQDIV2, R_DM16BITFULL = {0, 0, 0} when 32 bits DRAM.
        u4Val = (IS_FORCE32()) ? 0xE0021159 : 0xE0021019;
    }
    else
    {
        // 0x7018 default: No 8BKEN, No 64BITEN, NO 16BFL, FDIV2, SIOEN, DQ4BMUX, DDR1 pinmux.
        u4Val = (IS_FORCE32()) ? 0xE0021059 : 0xE0021459;
    }
    
    // Set MATYPE: column address bits.
    u4Val &= ~0x3000;
    switch(TCMGET_COLADDR())
    {
    case COL_ADDR_BIT_9:
        u4Val |= (1 << 12);
        break;
    case COL_ADDR_BIT_10:
        u4Val |= (2 << 12);
        break;
    case COL_ADDR_BIT_11:
        u4Val |= (3 << 12);
        break;
    case COL_ADDR_BIT_8:
    default:
        break;
    }

    // Set DRAM configuration.
    DRAM_WRITE32(0x18, u4Val);

    // Set DQSISEL.
    DRAM_WRITE32(0x390, 0xF0F0);

    // Set clock duty cycle.
    DRAM_WRITE32(0x348, DDR1_OUT_CLK_DELAY_CHAIN_0);
    DRAM_WRITE32(0x34C, DDR1_OUT_CLK_DELAY_CHAIN_1);

    // Set DQ input per bit delay.
#ifndef DRAM_DQINPUTPERBITCALI    
    DRAM_WRITE32(0x3B0, DDR1_IN_DQ_DELAY_CHAIN_0);
    DRAM_WRITE32(0x3B4, DDR1_IN_DQ_DELAY_CHAIN_1);
    DRAM_WRITE32(0x3B8, DDR1_IN_DQ_DELAY_CHAIN_2);
    DRAM_WRITE32(0x3BC, DDR1_IN_DQ_DELAY_CHAIN_3);
    DRAM_WRITE32(0x3C0, DDR1_IN_DQ_DELAY_CHAIN_4);
    DRAM_WRITE32(0x3C4, DDR1_IN_DQ_DELAY_CHAIN_5);
    DRAM_WRITE32(0x3C8, DDR1_IN_DQ_DELAY_CHAIN_6);
    DRAM_WRITE32(0x3CC, DDR1_IN_DQ_DELAY_CHAIN_7);
#endif

    // Set pad driving.
    DRAM_WRITE32(0x300, ((UINT32)DDR1_CLK_DRIVING<<24) | 0x80);
    DRAM_WRITE32(0x308, ((UINT32)DDR1_CMD_DRIVING<<24) | 0x80);
    DRAM_WRITE32(0x310, ((UINT32)DDR1_DQS_DRIVING<<24) | 0x80);
    DRAM_WRITE32(0x318, ((UINT32)DDR1_DQ_DRIVING<<24) | 0x80);

    // Set RDSEL duty cycle control.
    DRAM_WRITE32(0x398, DDR1_DQS_RDSEL);
    DRAM_WRITE32(0x39C, DDR1_DQ_RDSEL);

    // Set Manual DQS mode.
    DRAM_WRITE32(0x234, 0x00010000);
    DRAM_WRITE32(0x230, 0x40000000);

    // Set MRS: BL8. Set EMRS: A1 driving strength normal for Winbond DRAM 100%.
    //u4Val = 0x00000003;  
    // Set MRS: BL8. Set EMRS: A1 driving strength weak for Winbond DRAM 60%.
    u4Val = 0x00020003;  
    // Set MRS: BL8. Set EMRS: A1 driving strength weak for Winbond DRAM 30%.
    //u4Val = 0x00220003;  

    // Set CAS latency. Program mapping CL= 2, 2.5, 3, 4 to u4CL = 4, 5, 6, 8.
    if (u4CL == 4)
    {
        u4Val |= 0x20; // CL = 2. 
    }
    else if (u4CL == 5)
    {
        u4Val |= 0x60;  // CL = 2.5. 
    }
    else  if (u4CL == 6)
    {
        u4Val |= 0x30;     // CL = 3. 
    }
    else  //if (u4CL == 8)
    {
        u4Val |= 0x40;     // CL = 4 . 
    }

    DRAM_WRITE32(0x10, u4Val);

    // Set CAS, RAS, WE, ADDR, CS delay: setup/hold time.
    DRAM_WRITE32(0x320, DDR1_CMD_DELAY_0);
    DRAM_WRITE32(0x324, DDR1_CMD_DELAY_1);
    DRAM_WRITE32(0x328, DDR1_CMD_DELAY_2);
    DRAM_WRITE32(0x32C, DDR1_CMD_DELAY_3);
    DRAM_WRITE32(0x330, DDR1_CMD_DELAY_4);
    DRAM_WRITE32(0x334, DDR1_CMD_DELAY_5);

    // Set clock delay.
    DRAM_WRITE32(0x304, DDR1_CLK_DELAY);

    // Set DQ output delay.
    DRAM_WRITE32(0x360, DDR1_DQ_OUTPUT_DELAY_0);
    DRAM_WRITE32(0x364, DDR1_DQ_OUTPUT_DELAY_1);
    DRAM_WRITE32(0x368, DDR1_DQ_OUTPUT_DELAY_2);
    DRAM_WRITE32(0x36C, DDR1_DQ_OUTPUT_DELAY_3);

    // Set DQS output delay.    
    DRAM_WRITE32(0x354, DDR1_DQS_DELAY);

    // Reset DDR PHY control, but do not auto init.
    DRAM_WRITE32(0x224, 0x3);
    CHIP_Delay_us(1000);
    DRAM_WRITE32(0x224, 0x0);
    CHIP_Delay_us(1000);
       
    // Set DQS gating window.
    // CAS latency. Program mapping CL= 2, 2.5, 3 to u4CL = 4, 5, 6.
    if (u4CL == 4) //CL = 2.
    {        
        DRAM_WRITE32(0x388, DDR1_DQSIEN);
        DRAM_WRITE32(0x380, 0x00010001);
        DRAM_WRITE32(0x384, 0x00010001);        
    }
    else if ((u4CL > 4) && (u4CL <= 6))
    {
        DRAM_WRITE32(0x388, DDR1_DQSIEN);
        DRAM_WRITE32(0x380, 0x00020002);
        DRAM_WRITE32(0x384, 0x00020002);
    }
    else
    {
        DRAM_WRITE32(0x388, DDR1_DQSIEN);
        DRAM_WRITE32(0x380, 0x00080008);
        DRAM_WRITE32(0x384, 0x00080008);
    }

    // Enable read ODT.
    //DRAM_WRITE32(0x33C, 0x00050000);
    // Disable read ODT.
    DRAM_WRITE32(0x33C, 0x0);

    if ((TCMGET_COLADDR() == 10) || (TCMGET_COLADDR() == 11))
    {
        /* 
            TFAW: 5387 0x70F4[31:28], 5363 0x7000[23:20].    
            TRFC: 5387 0x7000[27, 23:20], 5363 0x7044[20:16].   

            TFAW spec value:
            2KB 45 ns.
            
            TRFC spec value:
            512Mb, 105ns --> (0x00040000)
            1Gb, 127.5ns --> (0x00090000)
            2Gb, 197.5ns --> (0x00180000)            
        */
        DRAM_WRITE32(0x00, 0x33A74487);  // Old setting = 0x33974465.

        // Set TRFC[4] = 1: Value = 21 + TRFC[3:0].
        // Set TRFC[4] = 0: Value = 8 + TRFC[3:0].
        DRAM_WRITE32(0x04, 0x06023009);  // TR2W = 3.
    }    
    else 
    {
        /*
            128 Mbit DRAM 500 Mhz => 8 ns.

            TCK = 12ns for CL3.
            TRCD = 16ns, TRP = 16ns, TFAW = Only DDR2 define, TWR = 15ns.
            TWTR = 1TCK = 12ns, TRC = 52ns, TRAS = 36ns.        
            Set TRFC[4] = 1: Value = 21 + TRFC[3:0].
            Set TRFC[4] = 0: Value = 8 + TRFC[3:0].
        */

        //DRAM_WRITE32(0x00, 0x22064354);  
        //DRAM_WRITE32(0x04, 0x05022005);  // TR2W = 2.

        DRAM_WRITE32(0x00, 0x22064354);  
        DRAM_WRITE32(0x04, 0x06023009);  // TR2W = 3.
    }

    // Set MACRODDROE = 1 for DDR1, DDR2, MCKMOD = 1, CLKJOPT = 1.
    if (IS_FULLFREQ())
    {
        DRAM_WRITE32(0x344, 0x001821D0);
    }
    else
    {   
        DRAM_WRITE32(0x344, 0x00182150);
    }
}

void _DDR_SetDdr1AfterAI(void)
{
    _DDR_SetRTT(DRVCUST_InitGet(eFlagReadODT));
}
#endif //CC_SUPPORT_DDR1

