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
#include "x_dram.h"
#include "x_ckgen.h"
#include "x_os.h"

LINT_EXT_HEADER_END

#if defined(CC_MTK_PRELOADER) && defined(NDEBUG)
#define Printf(fmt...)
#endif /* defined(CC_MTK_PRELOADER) && defined(NDEBUG) */

#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
static DRAM_CFG_T rDDRConfig;
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

UINT16 _DDR_GrayToBinary(UINT16 num)
{
        UINT16 temp;
		
	if (num == 0)
	{
	     return 0;
	}

	temp = num ^ (num>>8);
        temp ^= (temp>>4);
        temp ^= (temp>>2);
        temp ^= (temp>>1);
        return temp;
}


void _DDR_SetDQS(UINT32 u4DQSth, UINT32 u4Bin)
{
    UINT32 u4Val, u4Gray;

    u4Gray = BIN_2_GREY(u4Bin);

    // Setup 7-bit DQS offset
    u4Val = DRAM_READ32(0x3A0 + (4*(u4DQSth >> 2)));
    u4Val = (u4Val & (~(0xffU << (8*(u4DQSth & 3)))));
    u4Val |= (u4Gray << (8*(u4DQSth & 3)));
    DRAM_WRITE32(0x3A0 + (4*(u4DQSth >> 2)), u4Val);
}

#ifdef DRAM_WRITE_DQS_CALIBRATION
void _DDR_SetOutDQS(UINT32 u4DQSth, UINT32 u4Gray)
{
    UINT32 u4Val, u4Bin;
    u4Bin = _DDR_GrayToBinary(u4Gray);
	
    u4Val = DRAM_READ32(0x354);
    u4Val = (u4Val & (~(0xF << (4*u4DQSth))));
    u4Val |= (u4Bin << (4*u4DQSth));
    DRAM_WRITE32(0x354, u4Val);
}
#endif

void _DDR_SetDQ(UINT32 u4DQSth, UINT32 u4Bin, UINT32 u4DQInputDelay[])
{
    UINT32 u4Val; 
    UINT32 u4OldBin;
    UINT32 i;
    
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

#ifdef DRAM_WRITE_DQS_CALIBRATION
void _DDR_SetOutDQ(UINT32 u4DQSth, UINT32 u4Gray)
{
    UINT32 u4Val, u4Bin;
    u4Bin = _DDR_GrayToBinary(u4Gray);  
	
    u4Val = (u4Bin << 28) | (u4Bin << 24) |(u4Bin << 20) |(u4Bin << 16) |
        (u4Bin << 12) | (u4Bin << 8) |(u4Bin<< 4) |(u4Bin);
    DRAM_WRITE32((0x360 + 4*u4DQSth), u4Val);
}
#endif

#ifdef DRAM_DQINPUTPERBITCALI
void _DDR_SetDQPerBit(UINT32 u4DQSth, INT8 i1DQPerBitDelay[], UINT32 u4DQInputDelay[])
{
    UINT32 u4Val;
    UINT32 i;
    UINT32 u4Bin;

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

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)
BOOL DDR_SetCustCfg(DRAM_CFG_T *prDdrCfg)
{
    UINT32 u4Val;    
    UINT32 range;
    UINT32 modulation;

    if (!prDdrCfg)
    {
        return FALSE;
    }

    // ui1_ssc_range - spread spectrum clock: range 0 ~ 100: 0% ~ +-1%. 0 = disable SSC.
    // ui1_ssc_modulation - spread spectrum modulation: range 0 ~ 60: 0Khz ~ 60Khz. 0 = disable SSC.    
    if ((rDDRConfig.ui1_ssc_range != prDdrCfg->ui1_ssc_range) ||
        (rDDRConfig.ui1_ssc_modulation != prDdrCfg->ui1_ssc_modulation))
    {
        if ((prDdrCfg->ui1_ssc_range > 100) ||(prDdrCfg->ui1_ssc_modulation > 60))
        {
            return FALSE;
        }

        rDDRConfig.ui1_ssc_range = prDdrCfg->ui1_ssc_range;
        rDDRConfig.ui1_ssc_modulation = prDdrCfg->ui1_ssc_modulation;

        range = (UINT32)rDDRConfig.ui1_ssc_range;
        modulation = (UINT32)rDDRConfig.ui1_ssc_modulation;
        
        if ((range == 0) || (modulation == 0))
        {
            /* Disable Spectrum */
            CKGEN_WRITE32(0x150, CKGEN_READ32(0x150) | 0x00000004);
        
            u4Val = CKGEN_READ32(0x150);

            /* Disable Step. */	       
            u4Val &= 0x0000FFFF;
            
            /* Disable Period. */
            u4Val &= 0xFFFF000F;

            CKGEN_WRITE32(0x150, u4Val);
        }
        else
        {
            /* Disable Spectrum */
            CKGEN_WRITE32(0x150, CKGEN_READ32(0x150) | 0x00000004);

            /* Setup Step. */	       
            //an example for SYSPLL 324MHz only.
            // 4*(2^16)*(324/27)*(RANGE/10000)*modulation/(27000)
            // => ((2^18)*(12)*(modulation*RANGE))/(27000*10000)	       
            u4Val = (modulation*range * 116/10000) & 0x0000FFFF;//for 27mhz dmss output (sysplll:324) 
            CKGEN_WRITE32(0x150, (CKGEN_READ32(0x150) & 0x0000FFFF) | (u4Val<<16));

            /* Setup Period. */
            u4Val = (27000 /4) / modulation;//for 27.0mhz dmss output (sysplll:324) = 30k
            CKGEN_WRITE32(0x150, (CKGEN_READ32(0x150) & 0xFFFF000F) | (u4Val<<4));

            //enable ssc
            CKGEN_WRITE32(0x150, CKGEN_READ32(0x150) & ~0x00000004 );

            /* Set Dmpll reference clock from Xtal to DMSS */
            CKGEN_WRITE32(0x110, (CKGEN_READ32(0x110) |0x00020000));
        }            
    }  

    // ui1_clk_driving - clock driving: range 0 ~ 15.
    if (rDDRConfig.ui1_clk_driving != prDdrCfg->ui1_clk_driving)
    {
        if (prDdrCfg->ui1_clk_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_clk_driving = prDdrCfg->ui1_clk_driving;

        u4Val = DRAM_READ32(0xBC);
        u4Val &= 0x00FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_driving) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_driving) << 28;
        DRAM_WRITE32(0xBC, u4Val);
    }    

    // ui1_clk_delay - clock delay: range 0 ~ 15.
    if (rDDRConfig.ui1_clk_delay != prDdrCfg->ui1_clk_delay)
    {
        if (prDdrCfg->ui1_clk_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_clk_delay = prDdrCfg->ui1_clk_delay;

        u4Val = DRAM_READ32(0x0C);
        u4Val &= 0xF0FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_delay) << 24;
        DRAM_WRITE32(0x0C, u4Val);
        

        u4Val = DRAM_READ32(0x8C);
        u4Val &= 0xFFF0FFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_delay) << 16;
        DRAM_WRITE32(0x8C, u4Val);
    }    

    // ui1_cmd_driving - command driving: range 0 ~ 15.
    if (rDDRConfig.ui1_cmd_driving != prDdrCfg->ui1_cmd_driving)
    {
        if (prDdrCfg->ui1_cmd_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_cmd_driving = prDdrCfg->ui1_cmd_driving;

        u4Val = DRAM_READ32(0xBC);
        u4Val &= 0xFFFF00FF;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_driving) << 8;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_driving) << 12;
        DRAM_WRITE32(0xBC, u4Val);
    }    

    // ui1_cmd_delay - command delay: range 0 ~ 15.
    if (rDDRConfig.ui1_cmd_delay != prDdrCfg->ui1_cmd_delay)
    {
        if (prDdrCfg->ui1_cmd_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_cmd_delay = prDdrCfg->ui1_cmd_delay;

        u4Val = DRAM_READ32(0x1A8);
        u4Val &= ~0x1F1F1F1F;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_delay);
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_delay) << 8;        
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_delay) << 16;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_driving) << 24;
        DRAM_WRITE32(0x1A8, u4Val);
        DRAM_WRITE32(0x1AC, u4Val);
        DRAM_WRITE32(0x1B0, u4Val);
        DRAM_WRITE32(0x1B4, u4Val);
        DRAM_WRITE32(0x1B8, u4Val);
        DRAM_WRITE32(0x1BC, u4Val);        
    }    

    // ui1_wdqs_driving - write DQS driving: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs_driving != prDdrCfg->ui1_wdqs_driving)
    {
        if (prDdrCfg->ui1_wdqs_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs_driving = prDdrCfg->ui1_wdqs_driving;

        u4Val = DRAM_READ32(0xB8);
        u4Val &= 0x00FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs_driving) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs_driving) << 28;
        DRAM_WRITE32(0xB8, u4Val);
    }    

    // ui1_wdqs0_delay - write DQS0 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs0_delay != prDdrCfg->ui1_wdqs0_delay)
    {
        if (prDdrCfg->ui1_wdqs0_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs0_delay = prDdrCfg->ui1_wdqs0_delay;

        u4Val = DRAM_READ32(0x14);
        u4Val &= 0xFFFFFFF0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs0_delay) << 0;
        DRAM_WRITE32(0x14, u4Val);
    }    

    // ui1_wdqs1_delay - write DQS1 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs1_delay != prDdrCfg->ui1_wdqs1_delay)
    {
        if (prDdrCfg->ui1_wdqs1_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs1_delay = prDdrCfg->ui1_wdqs1_delay;

        u4Val = DRAM_READ32(0x14);
        u4Val &= 0xFFFFFF0F;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs1_delay) << 4;
        DRAM_WRITE32(0x14, u4Val);
    }    
    
    // ui1_wdqs2_delay - write DQS2 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs2_delay != prDdrCfg->ui1_wdqs2_delay)
    {
        if (prDdrCfg->ui1_wdqs2_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs2_delay = prDdrCfg->ui1_wdqs2_delay;

        u4Val = DRAM_READ32(0x14);
        u4Val &= 0xFFFFF0FF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs2_delay) << 8;
        DRAM_WRITE32(0x14, u4Val);
    }    

    // ui1_wdqs3_delay - write DQS3 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs3_delay != prDdrCfg->ui1_wdqs3_delay)
    {
        if (prDdrCfg->ui1_wdqs3_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs3_delay = prDdrCfg->ui1_wdqs3_delay;

        u4Val = DRAM_READ32(0x14);
        u4Val &= 0xFFFF0FFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs3_delay) << 12;
        DRAM_WRITE32(0x14, u4Val);
    }    

    // ui1_wdq_driving - write DQS driving: range 0 ~ 15.
    if (rDDRConfig.ui1_wdq_driving != prDdrCfg->ui1_wdq_driving)
    {
        if (prDdrCfg->ui1_wdq_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdq_driving = prDdrCfg->ui1_wdq_driving;

        u4Val = DRAM_READ32(0xB8);
        u4Val &= 0xFFFF00FF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq_driving) << 8;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq_driving) << 12;
        DRAM_WRITE32(0xB8, u4Val);
    }    

    // ui1_wdq0_delay - write DQ bit 0 ~ 7 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdq0_delay != prDdrCfg->ui1_wdq0_delay)
    {
        if (prDdrCfg->ui1_wdq0_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdq0_delay = prDdrCfg->ui1_wdq0_delay;

        u4Val = 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 4;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 8;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 12;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 16;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 20;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq0_delay) << 28;
        DRAM_WRITE32(0x200, u4Val);
    }    

    // ui1_wdq1_delay - write DQ bit 8 ~ 15 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdq1_delay != prDdrCfg->ui1_wdq1_delay)
    {
        if (prDdrCfg->ui1_wdq1_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdq1_delay = prDdrCfg->ui1_wdq1_delay;

        u4Val = 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 4;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 8;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 12;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 16;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 20;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq1_delay) << 28;
        DRAM_WRITE32(0x204, u4Val);
    }    
    
    // ui1_wdq2_delay - write DQ bit 16 ~ 23 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdq2_delay != prDdrCfg->ui1_wdq2_delay)
    {
        if (prDdrCfg->ui1_wdq2_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdq2_delay = prDdrCfg->ui1_wdq2_delay;

        u4Val = 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 4;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 8;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 12;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 16;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 20;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq2_delay) << 28;
        DRAM_WRITE32(0x208, u4Val);
    }

    // ui1_wdq3_delay - write DQ bit 24 ~ 31 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdq3_delay != prDdrCfg->ui1_wdq3_delay)
    {
        if (prDdrCfg->ui1_wdq3_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdq3_delay = prDdrCfg->ui1_wdq3_delay;

        u4Val = 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 4;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 8;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 12;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 16;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 20;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq3_delay) << 28;
        DRAM_WRITE32(0x20C, u4Val);
    }

    return TRUE;
}

BOOL DDR_GetCustCfg(DRAM_CFG_T *prDdrCfg)
{
    UINT32 u4Val;    
    UINT32 range;
    UINT32 modulation;

    // Clock driving is never be zero.
    if (rDDRConfig.ui1_clk_driving == 0)
    {
        u4Val = CKGEN_READ32(0x150);
        u4Val = (u4Val >> 4) & 0xFFF;
        modulation = (u4Val > 0) ? ((27000 /4)/u4Val) : 0;

        u4Val = CKGEN_READ32(0x150);
        u4Val = u4Val >> 16;
        range = (u4Val > 0) ? ((u4Val * 10000)/(modulation * 116)) : 0;

        rDDRConfig.ui1_ssc_range = (UINT8)range;
        rDDRConfig.ui1_ssc_modulation = (UINT8)modulation;

        u4Val = DRAM_READ32(0xBC);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_clk_driving = u4Val;

        u4Val = DRAM_READ32(0x0C);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_clk_delay = u4Val;

        u4Val = DRAM_READ32(0xBC);
        u4Val = (u4Val >> 8) & 0xF;
        rDDRConfig.ui1_cmd_driving = u4Val;

        u4Val = DRAM_READ32(0x1A8);
        u4Val &= 0xF;
        rDDRConfig.ui1_cmd_delay = u4Val;

        u4Val = DRAM_READ32(0xB8);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_wdqs_driving = u4Val;

        u4Val = DRAM_READ32(0x14);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdqs0_delay = u4Val;

        u4Val = DRAM_READ32(0x14);
        u4Val = (u4Val >> 4) & 0xF;
        rDDRConfig.ui1_wdqs1_delay = u4Val;

        u4Val = DRAM_READ32(0x14);
        u4Val = (u4Val >> 8) & 0xF;
        rDDRConfig.ui1_wdqs2_delay = u4Val;

        u4Val = DRAM_READ32(0x14);
        u4Val = (u4Val >> 12) & 0xF;
        rDDRConfig.ui1_wdqs3_delay = u4Val;
        
        u4Val = DRAM_READ32(0xB8);
        u4Val = (u4Val >> 8) & 0xF;
        rDDRConfig.ui1_wdq_driving = u4Val;
        
        u4Val = DRAM_READ32(0x200);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq0_delay = u4Val;

        u4Val = DRAM_READ32(0x204);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq1_delay = u4Val;

        u4Val = DRAM_READ32(0x208);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq2_delay = u4Val;

        u4Val = DRAM_READ32(0x20C);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq3_delay = u4Val;        
    }

    if (!prDdrCfg)
    {
        return FALSE;
    }
   
    (void *)x_memcpy((void *)prDdrCfg, (const void *)&rDDRConfig, sizeof(DRAM_CFG_T));
    
    return TRUE;    
}
#endif /* !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */

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

