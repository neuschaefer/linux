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

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
void _DDR_DeDQSGlitch(void)
{
    /*
        Note: 5396/5368 modify rDQS/Gating delay will produce glitch.
        And this glitch will cause gating error.
        Software can workaround this problem. Use PHYRESET to clear gating error.
    */
    if ((DRAM_READ32(0x2A8) != 0) || (DRAM_READ32(0x2AC) != 0))
    {
        _DDR_ResetPhyOnly();

        // Turn off unused macro.
        if (IS_FORCE32())
        {        
            if (IS_DDR_SWAP16BIT())
            {
                // MACRO0RST
                DRAM_WRITE32(0x224, 0x14);
            }
            else
            {
                // MACRO1RST            
                DRAM_WRITE32(0x224, 0x18);
            }
        }       
    }        
}
void _DDR_SetDQS(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    UINT32 u4Val;

    u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4DQSth] : u4DQSth;
    
    // Setup 7-bit DQS offset
    u4Val = DRAM_READ32(0x3A0 + (4*(u4HwDQSth >> 2)));
    u4Val = (u4Val & (~(0xffU << (8*(u4HwDQSth & 3)))));
    u4Val |= (u4Bin << (8*(u4HwDQSth & 3)));
    DRAM_WRITE32(0x3A0 + (4*(u4HwDQSth >> 2)), u4Val);

    _DDR_DeDQSGlitch();
}

#ifdef DRAM_DEBUG_TOOL
void _DDR_SetOutDQS(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    UINT32 u4Val;

    u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4DQSth] : u4DQSth;
	
    u4Val = DRAM_DDRPHY_READ32(0x354);
    u4Val = (u4Val & (~(0xF << (4*u4HwDQSth))));
    u4Val |= (u4Bin << (4*u4HwDQSth));
    DRAM_DDRPHY_WRITE32(0x354, u4Val);
}
#endif

void _DDR_SetDQ(UINT32 u4DQSth, UINT32 u4Bin, UINT32 u4DQInputDelay[])
{
    UINT32 u4OldBin;
    UINT32 i;
    UINT32 u4Val;

    /*

        Caution: 
        5365 IS_DDR_SWAP16BIT() do not need to swap DQ input delay mapping.
        But RDQS, WDQS, WDQ need to be swapped.
        
    */
    u4OldBin = u4DQInputDelay[(2*u4DQSth)];
    for (i=0; i<4; i++)
    {
        u4Val = (u4OldBin>>(8*i)) & 0xFF;
        u4Val += u4Bin;
        u4OldBin &= ~(0xFF << (8*i));
        u4OldBin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B0 + (8*u4DQSth), u4OldBin);

    u4OldBin = u4DQInputDelay[(2*u4DQSth)+1];
    for (i=0; i<4; i++)
    {
        u4Val = (u4OldBin>>(8*i)) & 0xFF;
        u4Val += u4Bin;
        u4OldBin &= ~(0xFF << (8*i));
        u4OldBin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B4 + (8*u4DQSth), u4OldBin);
}

#ifdef DRAM_DEBUG_TOOL
void _DDR_SetOutDQ(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Swap16Bit[4] = {2, 3, 0, 1}; // Mapping when 16 bits DQ swap.
    UINT32 u4HwDQSth;
    UINT32 u4Val;

    if ((!IS_DDR_QFP()) ||(!DRVCUST_InitGet(eDdrBusX8)))
    {
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4DQSth] : u4DQSth;
    	
        u4Val = (u4Bin << 28) | (u4Bin << 24) |(u4Bin << 20) |(u4Bin << 16) |
            (u4Bin << 12) | (u4Bin << 8) |(u4Bin<< 4) |(u4Bin);
        
        DRAM_DDRPHY_WRITE32((0x360 + 4*u4HwDQSth), u4Val);
    }
    else
    {
        /*

        Only 5365 QFP and data bus x8 mode need to remap DQ output delay control.
        
        byte0 output delay register => 0xf0007360[15:0], 0xf0007364[15:0]
        byte2 output delay register => 0xf0007368[15:0], 0xf000736C[15:0]

        byte1 output delay register => 0xf0007360[31:16], 0xf0007364[31:16]
        byte3 output delay register => 0xf0007368[31:16], 0xf000736C[31:16]

        */
        u4HwDQSth = (IS_DDR_SWAP16BIT()) ? u4Swap16Bit[u4DQSth] : u4DQSth;

        if (u4HwDQSth == 2)
        {
            u4Val = DRAM_DDRPHY_READ32(0x368);
            u4Val &= 0xFFFF0000;
            u4Val |= (u4Bin << 12) | (u4Bin << 8) |(u4Bin<< 4) |(u4Bin);
            DRAM_DDRPHY_WRITE32(0x368, u4Val);

            u4Val = DRAM_DDRPHY_READ32(0x36C);
            u4Val &= 0xFFFF0000;
            u4Val |= (u4Bin << 12) | (u4Bin << 8) |(u4Bin<< 4) |(u4Bin);
            DRAM_DDRPHY_WRITE32(0x36C, u4Val);                        
        }
        else if (u4HwDQSth == 3)
        {
            u4Val = DRAM_DDRPHY_READ32(0x368);
            u4Val &= 0x0000FFFF;
            u4Val |= (u4Bin << 28) | (u4Bin << 24) |(u4Bin << 20) |(u4Bin << 16);
            DRAM_DDRPHY_WRITE32(0x368, u4Val);

            u4Val = DRAM_DDRPHY_READ32(0x36C);
            u4Val &= 0x0000FFFF;
            u4Val |= (u4Bin << 28) | (u4Bin << 24) |(u4Bin << 20) |(u4Bin << 16);
            DRAM_DDRPHY_WRITE32(0x36C, u4Val);                        
        }
        else if (u4HwDQSth == 0)
        {
            u4Val = DRAM_DDRPHY_READ32(0x360);
            u4Val &= 0x0000FFFF;
            u4Val |= (u4Bin << 28) | (u4Bin << 24) |(u4Bin << 20) |(u4Bin << 16);
            DRAM_DDRPHY_WRITE32(0x360, u4Val);

            u4Val = DRAM_DDRPHY_READ32(0x364);
            u4Val &= 0x0000FFFF;
            u4Val |= (u4Bin << 28) | (u4Bin << 24) |(u4Bin << 20) |(u4Bin << 16);
            DRAM_DDRPHY_WRITE32(0x364, u4Val);                        
        }
        else if (u4HwDQSth == 1)
        {
            u4Val = DRAM_DDRPHY_READ32(0x360);
            u4Val &= 0xFFFF0000;
            u4Val |= (u4Bin << 12) | (u4Bin << 8) |(u4Bin<< 4) |(u4Bin);
            DRAM_DDRPHY_WRITE32(0x360, u4Val);

            u4Val = DRAM_DDRPHY_READ32(0x364);
            u4Val &= 0xFFFF0000;
            u4Val |= (u4Bin << 12) | (u4Bin << 8) |(u4Bin<< 4) |(u4Bin);
            DRAM_DDRPHY_WRITE32(0x364, u4Val);                        
        }
    }
}
#endif

#ifdef DRAM_DQINPUTPERBITCALI
void _DDR_SetDQPerBit(UINT32 u4DQSth, INT8 i1DQPerBitDelay[], UINT32 u4DQInputDelay[])
{
    UINT32 u4Val;
    UINT32 i;
    UINT32 u4Bin;

    /*

        Caution: 
        5365 IS_DDR_SWAP16BIT() do not need to swap DQ input delay mapping.
        But RDQS, WDQS, WDQ need to be swapped.
        
    */
    
    u4Bin = u4DQInputDelay[(2*u4DQSth)];
    for (i=0; i<4; i++)
    {
        u4Val = (u4Bin>>(8*i)) & 0xFF;
        u4Val += i1DQPerBitDelay[i];
        u4Bin &= ~(0xFF << (8*i));
        u4Bin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B0 + 8*u4DQSth, u4Bin);

    Printf("DQ perbit set 0x%04X=0x%08X.\n", (0x73B0 + 8*u4DQSth), u4Bin);

    u4Bin = u4DQInputDelay[(2*u4DQSth)+1];
    for (i=0; i<4; i++)
    {
        u4Val = (u4Bin>>(8*i)) & 0xFF;
        u4Val += i1DQPerBitDelay[i+4];
        u4Bin &= ~(0xFF << (8*i));
        u4Bin |= u4Val << (8*i);
    }
    DRAM_WRITE32(0x3B4 + 8*u4DQSth, u4Bin);
    
    Printf("DQ perbit set 0x%04X=0x%08X.\n", (0x73B4 + 8*u4DQSth), u4Bin);
}
#endif /* #ifdef DRAM_DQINPUTPERBITCALI */

