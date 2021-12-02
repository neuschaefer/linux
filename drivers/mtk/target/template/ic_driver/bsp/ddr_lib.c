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
/** @file ddr_lib.c
 *  Dram related routines.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "ddr.h"
#include "x_assert.h"
#include "x_bim.h"

LINT_EXT_HEADER_END

EXTERN void CHIP_DisplayString(const INT8 *szString);
EXTERN void CHIP_DisplayInteger(INT32 i);

void print_dqs(INT32 i4Line, INT32 i4Min, INT32 i4Max, INT32 i4Avg);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static UINT32 _DdrIsTestDdrOk()
{
    // wait dram stable
    WAIT_DRAM_STABLE();

    MEM_WRITE32(0x00, DRAM_TEST_DATA1);
    MEM_WRITE32(0x04, DRAM_TEST_DATA2);
    MEM_WRITE32(0x08, DRAM_TEST_DATA1);
    MEM_WRITE32(0x0c, DRAM_TEST_DATA2);
    MEM_WRITE32(0x10, DRAM_TEST_DATA1);
    MEM_WRITE32(0x14, DRAM_TEST_DATA2);
    MEM_WRITE32(0x18, DRAM_TEST_DATA1);
    MEM_WRITE32(0x1c, DRAM_TEST_DATA2);

    if ((MEM_READ32(0x00)!=DRAM_TEST_DATA1) ||
        (MEM_READ32(0x04)!=DRAM_TEST_DATA2) ||
        (MEM_READ32(0x08)!=DRAM_TEST_DATA1) ||
        (MEM_READ32(0x0c)!=DRAM_TEST_DATA2) ||
        (MEM_READ32(0x10)!=DRAM_TEST_DATA1) ||
        (MEM_READ32(0x14)!=DRAM_TEST_DATA2) ||
        (MEM_READ32(0x18)!=DRAM_TEST_DATA1) ||
        (MEM_READ32(0x1c)!=DRAM_TEST_DATA2))
    {
        return FALSE;
    }

    return TRUE;
}

static void _DdrSetTurbo(UINT32 u4Bits)
{
    ASSERT((u4Bits == 32) || (u4Bits == 64));

    UINT32 u4Val = (u4Bits == 64) ? DDR_64_TURBO : DDR_32_TURBO;
    u4Val = (u4Val & ~(0x00000300U)) | (DRAM_READ32(0x04) & (0x00000300U));
    DRAM_WRITE32(0x04, u4Val);
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _DDR_SetDQS(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Val, u4Gray;

    u4Gray = BIN_2_GREY(u4Bin);

    // Setup 8-bit DQS offset
    u4Val = DRAM_READ32(0x18 + (4*(u4DQSth >> 2)));
    u4Val = (u4Val & (~(0xffU << (8*(u4DQSth & 3)))));
    u4Val |= (u4Gray << (8*(u4DQSth & 3)));
    DRAM_WRITE32(0x18 + (4*(u4DQSth >> 2)), u4Val);
}

void _DDR_SetDQ(UINT32 u4DQth, UINT32 u4Bin)
{
    UINT32 u4Val, u4HighBit, u4Gray;

    u4Bin &= 0x7f;
    u4Gray = BIN_2_GREY(u4Bin);

    u4Val = DRAM_READ32(0x90 + (4*(u4DQth >> 2)));
    u4HighBit = u4Val & 0x80000000;
    u4Val = (u4Val & (~(0xffU << (8*(u4DQth & 3)))));
    u4Val |= (u4Gray << (8*(u4DQth & 3)));
    u4Val |= u4HighBit;
    DRAM_WRITE32(0x90 + (4*(u4DQth >> 2)), u4Val);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** DDR_ResetBits() follow user requests to detect/set DDR.
 *  @param u4Clk        Dram controller clock, DMPLL clock / 2.
 *  @param fgDDR        (fgDDR & 0x0f) is the number of DRAM,
 *                      (fgDDR & 0xf0 >> 4) is the DDR-I or DDR-II.
 *  @param fgFullFreq   0 for half frequence, otherwise full frequence setting
 *  @param fgForce32    0 for 64 bits enable, otherwise 32 bits only.
 *  @retval only 16/32/64 bits, otherwise failure.
 */
//-----------------------------------------------------------------------------
UINT32 DDR_ResetBits(UINT32 u4Clk, UINT32 fgDDR, UINT32 fgFullFreq,
                     UINT32 fgForce32)
{
    UNUSED(u4Clk);

    _DdrSetTurbo(fgForce32 ? 32 : 64);

    if (!_DdrIsTestDdrOk())
    {
        return 0;
    }

    return (fgForce32 ? (fgFullFreq ? 32 : 16) : (fgFullFreq ? 64 : 32));
}

void print_dqs(INT32 i4Line, INT32 i4Min, INT32 i4Max, INT32 i4Avg)
{
    CHIP_DisplayString("Byte ");
    CHIP_DisplayInteger(i4Line);
    CHIP_DisplayString(" : (");
    CHIP_DisplayInteger(i4Min);
    CHIP_DisplayString(" ~ ");
    CHIP_DisplayInteger(i4Max);
    CHIP_DisplayString("), Size ");
    CHIP_DisplayInteger(i4Max - i4Min);
    CHIP_DisplayString(", Set ");
    CHIP_DisplayInteger(i4Avg);
    CHIP_DisplayString("\n");
}

#if defined(CC_5391_PRELOADER) && defined(__KERNEL__)
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

