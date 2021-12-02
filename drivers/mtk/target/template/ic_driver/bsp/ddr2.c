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

#include "ddr.h"
#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"

LINT_EXT_HEADER_END

//lint --e{717} do ... while (0);

#if CC_SUPPORT_DDR2

void _DDR_SetDdr2BeforeAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fg64En,
    UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum)
{
    UINT32 u4Val, i, u4CL;

    UNUSED(fgDQS);
    UNUSED(fgGDDR);

    u4CL = (UINT32)DRVCUST_InitGet(eDdrCL);
    ASSERT((u4CL == 5) || (u4CL == 6) || (u4CL == 7));
 	//ASSERT((u4CL == 5) || (u4CL == 6));
    ASSERT((u4ChipNum==1) || (u4ChipNum==2));
    ASSERT(!fgFull);

    u4Val = BIN_2_GREY(DDR_DQSIEN) & 0xFF;
    u4Val = (u4Val << 24) | (u4Val << 16) | (u4Val << 8) | u4Val;
    DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | u4Val); // Need to do early

    DRAM_WRITE32(0x80, DDR2_EMRS_BM);
    DRAM_WRITE32(0x90, DDR2_IN_DELAY_CHAIN_1);

    //for (i = 0x98; i <= 0xB4; i += 4)
    //{
    //    DRAM_WRITE32(i, DDR2_OUT_CLK_DELAY_CHAIN_0);
    //}
	DRAM_WRITE32(0x98, DDR2_OUT_CLK_DELAY_CHAIN_0);
	DRAM_WRITE32(0x9C, DDR2_OUT_CLK_DELAY_CHAIN_1);
	DRAM_WRITE32(0xA0, DDR2_OUT_CLK_DELAY_CHAIN_2);
	DRAM_WRITE32(0xA4, DDR2_OUT_CLK_DELAY_CHAIN_3);

    DRAM_WRITE32(0xB8, DDR2_PAD_DRIVING_0);
    DRAM_WRITE32(0xBC, DDR2_PAD_DRIVING_1);

    DRAM_WRITE32(0xE8, DDR2_DQS_RDSEL);
    DRAM_WRITE32(0xEC, DDR2_DQ_RDSEL);

    if (!fg64En)    // 16 bits [22:21]=00, [15:8]=0x03
    {
        DRAM_WRITE32(0x28, 0xE185030D);
    }
    else            // 32 bits [22:21]=01, [15:8]=0x0f
    {
        DRAM_WRITE32(0x28, 0xF1250F0D);
    }

    u4Val = 0x00000300 | DDR2_WRITE_ODT;
    if (u4CL == 7)
    {
        u4Val |= 0x00007000;
    }
    else if (u4CL == 6)
    {
        u4Val |= 0x00006000;
    }
    else // if (u4CL == 5)
    {
        u4Val |= 0x00005000;
    }

    u4Val |= 0x00020000;
    DRAM_WRITE32(0x08, u4Val);

    DRAM_WRITE32(0x0C, DDR2_CMD_CLK_DELAY);
    DRAM_WRITE32(0x10, DDR2_DQ_DELAY);
    DRAM_WRITE32(0x14, DDR2_DQS_DELAY);
    DRAM_WRITE32(0x8C, fg64En ? 0x00000001 : 0x00000000);

    _DDR_ResetPhy();
    DRAM_WRITE32(0xF0, DDR2_PHY_CTRL_1);
    DRAM_WRITE32(0xF4, DDR2_CTRL_1);

    // 070D8H MCKDLY
    u4Val = DDR_II;
#ifdef CC_MT5392B
	if (BSP_GetIcVersion() < IC_VER_5392B_E1)
	{
		// before ECO code.
		u4Val = (u4Val << 30) | 0x0020F002; // B ver
	}
	else
	{
		// after ECO code.
		u4Val = (u4Val << 30) | 0x0020F300; // C ver
	}
#else
	u4Val = (u4Val << 30) | 0x0020F002;
#endif

    DRAM_WRITE32(0xD8, u4Val);

	if (u4CL == 7)
    {
        DRAM_WRITE32(0xDC, 0x42400400);
        DRAM_WRITE32(0xE0, 0x00400400);
    }
    else if (u4CL == 6)
    {
        DRAM_WRITE32(0xDC, 0x42100100);
        DRAM_WRITE32(0xE0, 0x00100100);
    }
    else // if (u4CL == 5)
    {
        DRAM_WRITE32(0xDC, 0x00040040);
        DRAM_WRITE32(0xE0, 0x00040040);
    }

    u4Val = 0xBAFF23DF;
    if (u4CL == 7)
    {
        u4Val = (u4Val & 0x0FFFFF0F) | 0xD00000D0;
    }
    else if (u4CL == 6)
    {
        u4Val = (u4Val & 0x0FFFFF0F) | 0xC00000D0;
    }
    DRAM_WRITE32(0x7C, u4Val);


    if ((TCMGET_COLADDR() == 10) || (TCMGET_COLADDR() == 11))
    {
        DRAM_WRITE32(0x00, 0x33974465);
    }
    else
    {
        DRAM_WRITE32(0x00, 0x33774455);
    }
    
#ifdef DDR_AC_TIMING
	DRAM_WRITE32(0x00, DDR_AC_TIMING);
#endif

}

void _DDR_SetDdr2AfterAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fg64En,
    UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum)
{
    DRAM_WRITE32(0x04, fg64En ? DDR_64_TURBO : DDR_32_NORMAL); // Test
    //DRAM_WRITE32(0x08, 0x00000020);
    _DDR_SetRtt((DDR_RTT_T)CC_DDR2_READ_ODT);
}

#else //CC_SUPPORT_DDR2

void _DDR_SetDdr2BeforeAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fg64En,
    UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum)
{
    LOG(1, "Compiled with DDR2 not support!");
    ASSERT(0);
}

void _DDR_SetDdr2AfterAI(UINT32 u4Clk, UINT32 fgFull, UINT32 fg64En,
    UINT32 fgDQS, UINT32 fgGDDR, UINT32 u4ChipNum)
{
    LOG(1, "Compiled with DDR2 not support!");
    ASSERT(0);
}

#endif //CC_SUPPORT_DDR2

