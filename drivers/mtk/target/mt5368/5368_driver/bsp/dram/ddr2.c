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
    UINT32 u4CL;
   
    u4CL = (UINT32)TCMGET_DDR_CL();

    // 0x7018 default: 
    // No 8BKEN, 9 bits column address.
    // No 64BITEN, 95ECO1, FDIV2, No SIOEN, DQ4BMUX, DDR2 pinmux.
    u4Val = 0xE2121041;

    // Set 16BITSWAP.
    if (IS_DDR_SWAP16BIT())
    {
        u4Val |= 0x01000000;
    }
    
    // Set 64BITEN.
    if (!IS_FORCE32())
    {
        u4Val |= 0x400;
    }

    // Anti-glitch at DRAM command/address. Suggestion by MS Liou.
    DRAM_DDRPHY_WRITE32(0x320, 0x01010101);
    DRAM_DDRPHY_WRITE32(0x324, 0x01010101);
    DRAM_DDRPHY_WRITE32(0x328, 0x01010101);
    DRAM_DDRPHY_WRITE32(0x32C, 0x01010101);
    DRAM_DDRPHY_WRITE32(0x330, 0x01010101); // CS is here.
    DRAM_DDRPHY_WRITE32(0x334, 0x01010101);
    DRAM_DDRPHY_WRITE32(0x338, 0x01010101);

    // Set CAS, RAS, WE, ADDR, CS delay: setup/hold time.
    if (IS_DRAM_CHANNELB_ACTIVE())
    {

        DRAM_DDRPHY_WRITE32(0x320, DDR2_CHB_CMD_DELAY_0);
        DRAM_DDRPHY_WRITE32(0x324, DDR2_CHB_CMD_DELAY_1);
        DRAM_DDRPHY_WRITE32(0x328, DDR2_CHB_CMD_DELAY_2);
        DRAM_DDRPHY_WRITE32(0x32C, DDR2_CHB_CMD_DELAY_3);
        DRAM_DDRPHY_WRITE32(0x330, DDR2_CHB_CMD_DELAY_4); // CS is here.
        DRAM_DDRPHY_WRITE32(0x334, DDR2_CHB_CMD_DELAY_5);
	DRAM_DDRPHY_WRITE32(0x338, DDR2_CHB_CMD_DELAY_6);
    }
    else
    {
        DRAM_DDRPHY_WRITE32(0x320, DDR2_CHA_CMD_DELAY_0);
        DRAM_DDRPHY_WRITE32(0x324, DDR2_CHA_CMD_DELAY_1);
        DRAM_DDRPHY_WRITE32(0x328, DDR2_CHA_CMD_DELAY_2);
        DRAM_DDRPHY_WRITE32(0x32C, DDR2_CHA_CMD_DELAY_3);
        DRAM_DDRPHY_WRITE32(0x330, DDR2_CHA_CMD_DELAY_4); // CS is here.
        DRAM_DDRPHY_WRITE32(0x334, DDR2_CHA_CMD_DELAY_5);
    	DRAM_DDRPHY_WRITE32(0x338, DDR2_CHA_CMD_DELAY_6);
    }

    // Set DRAM configuration.
    DRAM_WRITE32(0x18, u4Val);

    //Set DDR2 pinmux
    u4Val = DRAM_DDRPHY_READ32(0x394);
    u4Val &= ~0x7;
    /*
        PINMUX define.
        0: LQFP DDR3 X16 DRAM
        1: LQFP DDR2 X16 DRAM
        2: BGA DDR3 X16 DRAM
        3: BGA DDR2 X16 DRAM
        4: LQFP DDR3 X8 DRAM
        5: LQFP DDR2 X8 DRAM
        6: BGA DDR3 X8 DRAM
        7: BGA DDR2 X8 DRAM
    */
    u4Val |= (IS_DDR_QFP()) ? 1 : 3;
    u4Val |= (IS_DDR_BUSX8()) ? 4: 0;        
    DRAM_DDRPHY_WRITE32(0x394, u4Val);

    // Disable GDDR3 reset.
    DRAM_WRITE32(0x0C, 0x100);

    // Set DQSINCTL
    DRAM_WRITE32(0x38C, 0x20);

    // Set DQSISEL.
    DRAM_DDRPHY_WRITE32(0x390, 0);

    // Set DQ input per bit delay.
#ifndef DRAM_DQINPUTPERBITCALI    
    if(IS_DRAM_CHANNELB_ACTIVE()) //Channel B Dq input delay
    {
        DRAM_WRITE32(0x3B0, DDR2_CHB_IN_DQ_DELAY_CHAIN_0);
        DRAM_WRITE32(0x3B4, DDR2_CHB_IN_DQ_DELAY_CHAIN_1);
        DRAM_WRITE32(0x3B8, DDR2_CHB_IN_DQ_DELAY_CHAIN_2);
        DRAM_WRITE32(0x3BC, DDR2_CHB_IN_DQ_DELAY_CHAIN_3);
	DRAM_WRITE32(0x3C0, DDR2_CHB_IN_DQ_DELAY_CHAIN_4);
        DRAM_WRITE32(0x3C4, DDR2_CHB_IN_DQ_DELAY_CHAIN_5);
        DRAM_WRITE32(0x3C8, DDR2_CHB_IN_DQ_DELAY_CHAIN_6);
        DRAM_WRITE32(0x3CC, DDR2_CHB_IN_DQ_DELAY_CHAIN_7);
    }
    else
    {
        DRAM_WRITE32(0x3B0, DDR2_CHA_IN_DQ_DELAY_CHAIN_0);
        DRAM_WRITE32(0x3B4, DDR2_CHA_IN_DQ_DELAY_CHAIN_1);
        DRAM_WRITE32(0x3B8, DDR2_CHA_IN_DQ_DELAY_CHAIN_2);
        DRAM_WRITE32(0x3BC, DDR2_CHA_IN_DQ_DELAY_CHAIN_3);
        DRAM_WRITE32(0x3C0, DDR2_CHA_IN_DQ_DELAY_CHAIN_4);
        DRAM_WRITE32(0x3C4, DDR2_CHA_IN_DQ_DELAY_CHAIN_5);
        DRAM_WRITE32(0x3C8, DDR2_CHA_IN_DQ_DELAY_CHAIN_6);
        DRAM_WRITE32(0x3CC, DDR2_CHA_IN_DQ_DELAY_CHAIN_7);
    }
#endif

    // Set pad driving.
    DRAM_DDRPHY_WRITE32(0x300, ((UINT32)DDR2_CLK_DRIVING<<24));
    DRAM_DDRPHY_WRITE32(0x308, ((UINT32)DDR2_CMD_DRIVING<<24));
    DRAM_DDRPHY_WRITE32(0x30C, ((UINT32)DDR2_CS_DRIVING<<24));
    DRAM_WRITE32(0x310, ((UINT32)DDR2_DQS_DRIVING<<24));
    DRAM_WRITE32(0x318, ((UINT32)DDR2_DQ_DRIVING<<24));

    //Set clk duty-cycle
    u4Val = IS_DRAM_CHANNELB_ACTIVE() ? DDR2_CHB_CLK_DUTY : DDR2_CHA_CLK_DUTY;
    DRAM_DDRPHY_WRITE32(0x35C, u4Val);

    // Set RDSEL duty cycle control.
    u4Val = IS_DRAM_CHANNELB_ACTIVE() ? DDR2_CHB_DQS_RDSEL : DDR2_CHA_DQS_RDSEL;
    DRAM_DDRPHY_WRITE32(0x398, u4Val);
    u4Val = IS_DRAM_CHANNELB_ACTIVE() ? DDR2_CHB_DQ_RDSEL : DDR2_CHA_DQ_RDSEL;
    DRAM_DDRPHY_WRITE32(0x39C, u4Val);

    // Set Manual DQS mode.
    DRAM_WRITE32(0x234, 0x00010000);

    // Set DLLFZ.
    DRAM_WRITE32(0x230, 0x40000000);

    // Write ODT setting. Default 150 Ohm
    u4MRS |= 0x00400000; 

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

    // Set WR by DRAM clock.
    u4MRS |= (TCMGET_DDR_CLK() <= 800000000) ? 0x00020A00 : 0x00020C00;    
    DRAM_WRITE32(0x10, u4MRS);

    // Set clock delay.
    DRAM_DDRPHY_WRITE32(0x304, DDR2_CLK_DELAY);

#ifndef DRAM_WDQS_CALIBRATION
    // Set DQ output delay.
    DRAM_DDRPHY_WRITE32(0x360, DDR2_DQ_OUTPUT_DELAY_0);
    DRAM_DDRPHY_WRITE32(0x364, DDR2_DQ_OUTPUT_DELAY_1);
    DRAM_DDRPHY_WRITE32(0x368, DDR2_DQ_OUTPUT_DELAY_2);
    DRAM_DDRPHY_WRITE32(0x36C, DDR2_DQ_OUTPUT_DELAY_3);

    //Set DQM output delay
    DRAM_DDRPHY_WRITE32(0x350, DDR2_DQM_DELAY);
    // Set DQS output delay.    
    DRAM_DDRPHY_WRITE32(0x354, DDR2_DQS_DELAY);
#endif

    // Reset DDR PHY control, but do not auto init.
    DRAM_WRITE32(0x224, 0x13);
    CHIP_Delay_us(1000);
    DRAM_WRITE32(0x224, 0x10);
    CHIP_Delay_us(1000);

    // Set read/write ODT.
    DRAM_WRITE32(0x33C, 0x004D0075);
    //Set ERODT/EODT
    DRAM_DDRPHY_WRITE32(0x33C, 0x00080004);
      
    if (u4CL == 7)
    {        
        // Set DQS gating window.
        DRAM_WRITE32(0x388, DDR2_DQSIEN);
        u4Val = (!IS_DDR_BUSX8()) ? 0x00040004: 0x00080008;
        DRAM_DDRPHY_WRITE32(0x380, u4Val);
        DRAM_DDRPHY_WRITE32(0x384, u4Val);

        //set AC timing
        if (!IS_DDR_ASYNCON())
        {
            DRAM_WRITE32(0x04, 0x0452230F);
        }
        else
        {
            DRAM_WRITE32(0x04, 0x0652230F);
        }

        u4Val = 0x002500F1;
        // Set EODT for CL7, RODT = 3.
        u4Val &= 0xFF0FFFFF;
        u4Val |= 0x00380004;
        DRAM_WRITE32(0x33C, u4Val);
        DRAM_DDRPHY_WRITE32(0x33C, u4Val);
    }
    else // u4CL == 6.
    {
        // Set DQS gating window.
        DRAM_WRITE32(0x388, DDR2_DQSIEN);
        u4Val = (!IS_DDR_BUSX8()) ? 0x00010001: 0x00020002;
        DRAM_DDRPHY_WRITE32(0x380, u4Val);
        DRAM_DDRPHY_WRITE32(0x384, u4Val);

        //set AC timing
        if (!IS_DDR_ASYNCON())
        {
            DRAM_WRITE32(0x04, 0x0542230F);
        }
        else
        {
            DRAM_WRITE32(0x04, 0x0542230F);
        }

        u4Val = 0x002500F1;
        DRAM_WRITE32(0x33C, u4Val);
        DRAM_DDRPHY_WRITE32(0x33C, u4Val);
    }

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

    // Set PHYPHDLY, CLKJOPT.
    DRAM_WRITE32(0x344, 0x00180320);

	//set SYNCRST, NOSYNC
	u4Val = DRAM_DDRPHY_READ32(0x344);
	if (!IS_DDR_ASYNCON())
    {
        DRAM_DDRPHY_WRITE32(0x344, u4Val | 0x360);
    }
    else
    {
        DRAM_DDRPHY_WRITE32(0x344, u4Val | (0x3 << 8));
    }        

    // Set CMPPD: REXTDN power down. Read only. Not write.
    DRAM_WRITE32(0xBBC, 0x8);
}

void _DDR_SetDdr2AfterAI(void)
{
    _DDR_SetRTT(DRVCUST_InitGet(eFlagReadODT));
}
#endif //CC_SUPPORT_DDR2

