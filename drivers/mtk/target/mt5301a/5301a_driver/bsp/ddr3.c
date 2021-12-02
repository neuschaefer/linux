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
/** @file ddr3.c
 *  DDR3 setting.
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

#ifdef CC_SUPPORT_DDR3
void _DDR_SetDdr3BeforeAI(void)
{
    UINT32 u4Val, u4CL;

    u4CL = (UINT32)TCMGET_DDR_CL();

    u4Val = BIN_2_GREY(DDR3_DQSIEN) & 0xFF;
    u4Val = (u4Val << 24) | (u4Val << 16) | (u4Val << 8) | u4Val;
    DRAM_WRITE32(0x94, DDR_DQSIEN_DMCKSEL | u4Val); // Need to do early

    DRAM_WRITE32(0x80, DDR3_EMRS_BM);
    DRAM_WRITE32(0x90, DDR3_IN_DELAY_CHAIN_1);

    DRAM_WRITE32(0x98, DDR3_OUT_CLK_DELAY_CHAIN_0);
    DRAM_WRITE32(0x9C, DDR3_OUT_CLK_DELAY_CHAIN_1);
    DRAM_WRITE32(0xA0, DDR3_OUT_CLK_DELAY_CHAIN_2);
    DRAM_WRITE32(0xA4, DDR3_OUT_CLK_DELAY_CHAIN_3);

    DRAM_WRITE32(0xB8, DDR3_PAD_DRIVING_0);
    DRAM_WRITE32(0xBC, DDR3_PAD_DRIVING_1);
    DRAM_WRITE32(0xD4, DDR3_IO_CLK_DELAY_CHAIN);

    DRAM_WRITE32(0xE8, DDR3_DQS_RDSEL);
    DRAM_WRITE32(0xEC, DDR3_DQ_RDSEL);
    
    DRAM_WRITE32(0x28, 0xF1A00F0D);//0xF1A50F0D

    u4Val = 0x00002200 | DDR3_WRITE_ODT;
    DRAM_WRITE32(0x08, u4Val);

    DRAM_WRITE32(0x0C, DDR3_CMD_CLK_DELAY);
    DRAM_WRITE32(0x10, DDR3_DQ_DELAY);
    DRAM_WRITE32(0x14, DDR3_DQS_DELAY);
    //DRAM_WRITE32(0x8C, fg64En ? 0x00000001 : 0x00000000);
    DRAM_WRITE32(0x8C, 0x00000000);

    _DDR_ResetPhy();
    DRAM_WRITE32(0xF0, DDR3_PHY_CTRL_1);
    DRAM_WRITE32(0xF4, DDR3_CTRL_1);
    DRAM_WRITE32(0x100, DDR3_OCD_CALIB_CTRL);

    DRAM_WRITE32(0xE4, 0x00006611);//0x00006601
    DRAM_WRITE32(0xE4, 0x00006613);//0x00006603

    // 070D8H MCKDLY

#ifdef CC_MT5392B
    if (BSP_GetIcVersion() < IC_VER_5392B_E1)
    {
        // before ECO code.
        u4Val = 0x0020F600; // B ver
    }
    else
    {
        // after ECO code.
        u4Val = 0x0020FD00; // C ver
    }
#else
    u4Val = 0x0020F600;
#endif
    DRAM_WRITE32(0xD8, u4Val);

    if (u4CL == 10)
    {
        DRAM_WRITE32(0xDC, 0x43200200);
        DRAM_WRITE32(0xE0, 0x32200200);
    }
    else
    {
        ASSERT(0);  // fix me
    }

    DRAM_WRITE32(0x7C, 0xD8832005);	//RL 0xD8832075

    if ((TCMGET_COLADDR() == 10) || (TCMGET_COLADDR() == 11))
    {
        DRAM_WRITE32(0x00, 0x64F8C5C7); // fix me
    }
    else
    {
        //DRAM_WRITE32(0x00, 0x64F8C5C7);
        DRAM_WRITE32(0x00, 0x4498C496);//0x4498C496 0x7798C49A
    }

    if (TCMGET_DRAMTYPE() == DDR_III_x2)
    {
        DRAM_WRITE32(0x110, 0x00012276);
    }
}

void _DDR_SetDdr3AfterAI(void)
{

}

#endif //CC_SUPPORT_DDR3

#if defined(CC_MTK_PRELOADER) && defined(__KERNEL__)
/* Replacement (=dummy) for GNU/Linux undefined reference on gcc preloader */
static void raise(void)
{
    return;
}

static void __aeabi_unwind_cpp_pr0(void)
{
    return;
}
#endif

