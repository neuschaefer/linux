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
 * Description:
 *      DDR factory commands
 *
 *---------------------------------------------------------------------------*/
#if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER)

#include "x_printf.h"
#ifdef __MW_CLI_DEF__

    #include "x_mid_cli.h"
#else

    #include "x_drv_cli.h"
#endif

#define DEFINE_IS_LOG       MEMTEST_IsLog
#include "x_debug.h"

#include "x_stl_lib.h"
#include "x_assert.h"

#include "x_timer.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "c_model.h"
#include "x_dram.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_hal_arm.h"
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
//#define BINARY2GRAY(u4Bin)   ((u4Bin) ^ ((u4Bin) >> 1))

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static DRAM_CFG_T rDDRConfig;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
BOOL DDR_SetCustCfg(DRAM_CFG_T *prDdrCfg)
{
    UINT32 u4Val;    
    UINT32 range;
    UINT32 modulation;
    UINT32 u4SysPLL = 459; // Unit in Mhz.

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
            CKGEN_WRITE32(0x190, CKGEN_READ32(0x190) | 0x40);

            /* Disable Step. */
            u4Val = CKGEN_READ32(0x194);
            u4Val &= 0xFFFF0000;
            CKGEN_WRITE32(0x194, u4Val);
            
            /* Disable Period. */
            u4Val = CKGEN_READ32(0x190);
            u4Val &= 0x0000FFFF;
            CKGEN_WRITE32(0x190, u4Val);
        }
        else
        {
            /* Disable Spectrum */
            CKGEN_WRITE32(0x190, CKGEN_READ32(0x190) | 0x40 );

            /* Setup Step. */	       
            u4Val = (u4SysPLL*360)/1000;
            u4Val = ((modulation*range*u4Val)/10000) & 0xFFFF;
            CKGEN_WRITE32(0x194, (CKGEN_READ32(0x194) & ~0xFFFF) | u4Val);

            /* Setup Period. */
            u4Val = (27000 /4) / modulation;
            CKGEN_WRITE32(0x190, (CKGEN_READ32(0x190) & 0xFFFF) | (u4Val<<16));

            //enable ssc
            CKGEN_WRITE32(0x190, CKGEN_READ32(0x190) & ~0x40 );

            /* Set Direct Mempll reference clock(Bypass Dmpll) from Xtal to DMSS */
            CKGEN_WRITE32(0x124, CKGEN_READ32(0x124) | 0x0800);
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

        u4Val = DRAM_READ32(0x300);
        u4Val &= 0x00FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_driving) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_driving) << 28;
        DRAM_WRITE32(0x300, u4Val);
    }    

    // ui1_clk_delay - clock delay: range 0 ~ 15.
    if (rDDRConfig.ui1_clk_delay != prDdrCfg->ui1_clk_delay)
    {
        if (prDdrCfg->ui1_clk_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_clk_delay = prDdrCfg->ui1_clk_delay;

        u4Val = DRAM_READ32(0x304);
        u4Val &= 0xFFFFFF00;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_delay) << 0;
        u4Val |= ((UINT32)prDdrCfg->ui1_clk_delay) << 4;
        DRAM_WRITE32(0x304, u4Val);
    }    

    // ui1_cmd_driving - command driving: range 0 ~ 15.
    if (rDDRConfig.ui1_cmd_driving != prDdrCfg->ui1_cmd_driving)
    {
        if (prDdrCfg->ui1_cmd_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_cmd_driving = prDdrCfg->ui1_cmd_driving;

        u4Val = DRAM_READ32(0x308);
        u4Val &= 0x00FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_driving) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_driving) << 28;
        DRAM_WRITE32(0x308, u4Val);
    }    

    // ui1_cmd_delay - command delay: range 0 ~ 15.
    if (rDDRConfig.ui1_cmd_delay != prDdrCfg->ui1_cmd_delay)
    {
        if (prDdrCfg->ui1_cmd_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_cmd_delay = prDdrCfg->ui1_cmd_delay;

        u4Val = DRAM_READ32(0x320);
        u4Val &= ~0x1F1F1F1F;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_delay);
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_delay) << 8;        
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_delay) << 16;
        u4Val |= ((UINT32)prDdrCfg->ui1_cmd_driving) << 24;
        DRAM_WRITE32(0x320, u4Val);
        DRAM_WRITE32(0x324, u4Val);
        DRAM_WRITE32(0x328, u4Val);
        DRAM_WRITE32(0x32C, u4Val);
        DRAM_WRITE32(0x330, u4Val);
        DRAM_WRITE32(0x334, u4Val);        
    }    

    // ui1_wdqs_driving - write DQS driving: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs_driving != prDdrCfg->ui1_wdqs_driving)
    {
        if (prDdrCfg->ui1_wdqs_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs_driving = prDdrCfg->ui1_wdqs_driving;

        u4Val = DRAM_READ32(0x310);
        u4Val &= 0x00FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs_driving) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs_driving) << 28;
        DRAM_WRITE32(0x310, u4Val);
    }    

    // ui1_wdqs0_delay - write DQS0 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs0_delay != prDdrCfg->ui1_wdqs0_delay)
    {
        if (prDdrCfg->ui1_wdqs0_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs0_delay = prDdrCfg->ui1_wdqs0_delay;

        u4Val = DRAM_READ32(0x354);
        u4Val &= 0xFFFFFFF0;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs0_delay) << 0;
        DRAM_WRITE32(0x354, u4Val);
    }    

    // ui1_wdqs1_delay - write DQS1 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs1_delay != prDdrCfg->ui1_wdqs1_delay)
    {
        if (prDdrCfg->ui1_wdqs1_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs1_delay = prDdrCfg->ui1_wdqs1_delay;

        u4Val = DRAM_READ32(0x354);
        u4Val &= 0xFFFFFF0F;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs1_delay) << 4;
        DRAM_WRITE32(0x354, u4Val);
    }    
    
    // ui1_wdqs2_delay - write DQS2 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs2_delay != prDdrCfg->ui1_wdqs2_delay)
    {
        if (prDdrCfg->ui1_wdqs2_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs2_delay = prDdrCfg->ui1_wdqs2_delay;

        u4Val = DRAM_READ32(0x354);
        u4Val &= 0xFFFFF0FF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs2_delay) << 8;
        DRAM_WRITE32(0x354, u4Val);
    }    

    // ui1_wdqs3_delay - write DQS3 delay: range 0 ~ 15.
    if (rDDRConfig.ui1_wdqs3_delay != prDdrCfg->ui1_wdqs3_delay)
    {
        if (prDdrCfg->ui1_wdqs3_delay > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdqs3_delay = prDdrCfg->ui1_wdqs3_delay;

        u4Val = DRAM_READ32(0x354);
        u4Val &= 0xFFFF0FFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdqs3_delay) << 12;
        DRAM_WRITE32(0x354, u4Val);
    }    

    // ui1_wdq_driving - write DQS driving: range 0 ~ 15.
    if (rDDRConfig.ui1_wdq_driving != prDdrCfg->ui1_wdq_driving)
    {
        if (prDdrCfg->ui1_wdq_driving > 15)
        {
            return FALSE;
        }

        rDDRConfig.ui1_wdq_driving = prDdrCfg->ui1_wdq_driving;

        u4Val = DRAM_READ32(0x318);
        u4Val &= 0x00FFFFFF;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq_driving) << 24;
        u4Val |= ((UINT32)prDdrCfg->ui1_wdq_driving) << 28;
        DRAM_WRITE32(0x318, u4Val);
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
        DRAM_WRITE32(0x360, u4Val);
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
        DRAM_WRITE32(0x364, u4Val);
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
        DRAM_WRITE32(0x368, u4Val);
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
        DRAM_WRITE32(0x36C, u4Val);
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

        u4Val = DRAM_READ32(0x300);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_clk_driving = u4Val;

        u4Val = DRAM_READ32(0x304);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_clk_delay = u4Val;

        u4Val = DRAM_READ32(0x308);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_cmd_driving = u4Val;

        u4Val = DRAM_READ32(0x320);
        u4Val &= 0xF;
        rDDRConfig.ui1_cmd_delay = u4Val;

        u4Val = DRAM_READ32(0x310);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_wdqs_driving = u4Val;

        u4Val = DRAM_READ32(0x354);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdqs0_delay = u4Val;

        u4Val = DRAM_READ32(0x354);
        u4Val = (u4Val >> 4) & 0xF;
        rDDRConfig.ui1_wdqs1_delay = u4Val;

        u4Val = DRAM_READ32(0x354);
        u4Val = (u4Val >> 8) & 0xF;
        rDDRConfig.ui1_wdqs2_delay = u4Val;

        u4Val = DRAM_READ32(0x354);
        u4Val = (u4Val >> 12) & 0xF;
        rDDRConfig.ui1_wdqs3_delay = u4Val;
        
        u4Val = DRAM_READ32(0x318);
        u4Val = (u4Val >> 24) & 0xF;
        rDDRConfig.ui1_wdq_driving = u4Val;
        
        u4Val = DRAM_READ32(0x360);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq0_delay = u4Val;

        u4Val = DRAM_READ32(0x364);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq1_delay = u4Val;

        u4Val = DRAM_READ32(0x368);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq2_delay = u4Val;

        u4Val = DRAM_READ32(0x36C);
        u4Val = (u4Val >> 0) & 0xF;
        rDDRConfig.ui1_wdq3_delay = u4Val;        
    }

    if (!prDdrCfg)
    {
        return FALSE;
    }
   
    (void)x_memcpy((void *)prDdrCfg, (const void *)&rDDRConfig, sizeof(DRAM_CFG_T));
    
    return TRUE;    
}

#endif /* #if !defined(CC_MTK_PRELOADER) && !defined(CC_MTK_LOADER) */
