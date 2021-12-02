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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_init.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_init.c
 *  Demux driver - initialization
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_drm_drvif.h"
#include "dmx_pcr.h"
#include "x_chip_id.h"

#undef CC_DMX_USE_CT
#ifndef DMX_NT_UCODE_DISABLE
#define CC_DMX_USE_CT
#endif  // DMX_NT_UCODE_DISABLE

#ifdef CC_DMX_EMULATION
#undef CC_DMX_USE_CT
#endif

#include "dmx_imem.h"

#include "dmx_imem_test.h"
#include "dmx_debug.h"
#include "x_lint.h"

#include "fvr.h"

#include "gcpu_if.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_util.h"
#include "x_os.h"
#include "x_timer.h"
#include "x_bim.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Define EXT_TS2 to select the second external TS interface. By default the
// first external TS interface is selected
//
//#define EXT_TS2


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NUM_UP_TEST_RESULT_WORDS            16
#define DMX_MAX_DBM_WAIT_COUNT              10

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_INPUT_TYPE_T _eDmxInputType;
static UINT32 _u4uCodeVersion = 0;

static BOOL _fgMicroProcessorStopped = FALSE;  // Is micro-processor stopped?

static DMX_FRONTEND_T _aeDmxFramerInput[DMX_FRAMER_COUNT];

static DMX_EXT_FRAMER_T _eDmxExtFramer = DMX_EXT_FRAMER_NORMAL;

static UINT16 _au2DmxFramerPktSize[DMX_FRAMER_COUNT];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DdmxIsFTIRunning
 *  Query if FTI is running or not
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsFTIRunning(void)
{
    UINT32 u4State;

    u4State = (DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3;
    LOG(7, "uP state %d\n", u4State);

    return 0 == u4State;
}


//-----------------------------------------------------------------------------
/** _DmxCkgenToggle
 */
//-----------------------------------------------------------------------------
static void _DmxCkgenToggle(UINT8 u1Framer)
{
#ifndef CC_DMX_FPGA_TEST
    UINT32 i, u4Ctrl;
    UINT8 au1Toggle[4] = { 4, 20, 0, 26 };

    if(u1Framer >= DMX_FRAMER_COUNT)
    {
        return;
    }

    if(u1Framer == 2)
    {
        return;
    }

    // Force toggle clock to guarantee switch clock success
    u4Ctrl = CKGEN_READ32(0x24C) & ~(0x1 << au1Toggle[u1Framer]);

    // Toggle 6 time is enough, but 10 for safe
    for(i=0; i<10; i++)
    {
        HAL_Delay_us(1);
        CKGEN_WRITE32(0x24C, u4Ctrl | (1 << au1Toggle[u1Framer]));
        HAL_Delay_us(1);
        CKGEN_WRITE32(0x24C, u4Ctrl);
    }
#endif
}


//-----------------------------------------------------------------------------
/** _DmxCkgenInit
 *  Select clock and pin mux for FTI
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxCkgenInit(BOOL fgOn)
{
    UINT32 u4Ctrl = 0;
#ifndef CC_DMX_FPGA_TEST
    UINT32 u4Reg;

    if(fgOn)
    {
        CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) | 0x10000);

		#ifdef CC_MT5389
		CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) | 0x10);
		#endif

        CKGEN_WRITE32(0x22c, 0x6);            // 324MHz

        _DMX_Lock();

        u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
        u4Ctrl &= ~0x1;
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);// Power on to dram clock

        // Init input source
        u4Reg = CKGEN_READ32(0x24C);
        u4Reg &= (~0x3030003);
        CKGEN_WRITE32(0x24C, u4Reg);
        _aeDmxFramerInput[0] = DMX_FE_INTERNAL;
        _aeDmxFramerInput[1] = DMX_FE_INTERNAL;
        _aeDmxFramerInput[3] = DMX_FE_INTERNAL;
        _DmxCkgenToggle(0);
        _DmxCkgenToggle(1);
        _DmxCkgenToggle(3);

        // Set DMX SRAM used by DMX
        u4Ctrl = DMXCMD_READ32(DMX_REG_FTUPCR) & 0xFBFFFFFF;
        DMXCMD_WRITE32(DMX_REG_FTUPCR, u4Ctrl);

        _fgMicroProcessorStopped = FALSE;
        _DMX_Unlock();
    }
    else
    {
        // Turn ff DMX clock
        _DMX_Lock();
        _fgMicroProcessorStopped = TRUE;
        _DMX_Unlock();

        CKGEN_WRITE32(0x22c, 0x80);
        CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) & ~0x10000);

		#ifdef CC_MT5389
		CKGEN_WRITE32(0x278, CKGEN_READ32(0x278) & ~0x10);
		#endif
    }
#else
    _DMX_Lock();

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
    u4Ctrl &= ~0x1;
    DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);// Power on to dram clock

    _aeDmxFramerInput[0] = DMX_FE_INTERNAL;
    _aeDmxFramerInput[1] = DMX_FE_INTERNAL;
    _aeDmxFramerInput[3] = DMX_FE_INTERNAL;
    _DmxCkgenToggle(0);
    _DmxCkgenToggle(1);
    _DmxCkgenToggle(3);

    // Set DMX SRAM used by DMX
    u4Ctrl = DMXCMD_READ32(DMX_REG_FTUPCR) & 0xFBFFFFFF;
    DMXCMD_WRITE32(DMX_REG_FTUPCR, u4Ctrl);

    _fgMicroProcessorStopped = FALSE;
    _DMX_Unlock();

    UNUSED(_aeDmxFramerInput);
#endif // CC_DMX_FPGA_TEST
}


//-----------------------------------------------------------------------------
/** _DmxSetExtCkgen
 */
//-----------------------------------------------------------------------------
static void _DmxSetExtCkgen(UINT8 u1TsIdx, DMX_EXT_CKGEN_T eSel)
{
#ifdef CC_MT5396
    UINT32 u4Ctrl, u4Mask;
    UINT8 au1Ext[4] = { 0, 2, 3, 1};
    UINT8 au1Ext2[4] = { 2, 0, 3, 1};
    UINT8 au1Ext3[4] = { 2, 3, 0, 1};

    ASSERT(u1TsIdx < DMX_FRAMER_COUNT);
    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return;
    }

    if(eSel >= DMX_CKGEN_NUM)
    {
        return;
    }

    if(u1TsIdx == 0)
    {
        u4Mask = ~0x30000;
    }
    else if(u1TsIdx == 1)
    {
        u4Mask = ~0x300000;
    }
    else if(u1TsIdx == 3)
    {
        u4Mask = ~0x3000000;
    }
    else
    {
        return;
    }

    u4Ctrl = CKGEN_READ32(0x2B4) & u4Mask;

    if(u1TsIdx == 0)
    {
        u4Ctrl |= ((UINT32)au1Ext[eSel] << 16);
    }
    else if(u1TsIdx == 1)
    {
        u4Ctrl |= ((UINT32)au1Ext2[eSel] << 20);
    }
    else if(u1TsIdx == 3)
    {
        u4Ctrl |= ((UINT32)au1Ext3[eSel] << 24);
    }

    CKGEN_WRITE32(0x2B4, u4Ctrl);
#endif
}


//-----------------------------------------------------------------------------
/** _DmxSetExtPinmux
 */
//-----------------------------------------------------------------------------
static void _DmxSetExtPinmux(DMX_PINMUX_SEL_T ePin)
{
    UINT32 u4Ctrl;

    switch(ePin)
    {
#if defined(CC_MT5396)
    case DMX_PINMUX_EXT_S:
        // pmux4[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x414) & (~0x7000);
        u4Ctrl |= 0x1000;
        CKGEN_WRITE32(0x414, u4Ctrl);
        break;

    case DMX_PINMUX_EXT_P:
        // pmux1[4:6], pmux4[6:7], pmux4[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x70);
        u4Ctrl |= 0x10;
        CKGEN_WRITE32(0x404, u4Ctrl);
        u4Ctrl = CKGEN_READ32(0x414) & (~0x70C0);
        u4Ctrl |= 0x1040;
        CKGEN_WRITE32(0x414, u4Ctrl);
        break;

    case DMX_PINMUX_EXT2_0_S:
        // pmux1[8:10], pmux1[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x7700);
        u4Ctrl |= 0x1100;
        CKGEN_WRITE32(0x404, u4Ctrl);

        // Disable EXT2_1, or all ts to ext2_1 not ext2_0
        // pmux1[8:10], pmux1[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x70);
        CKGEN_WRITE32(0x404, u4Ctrl);
        break;

    case DMX_PINMUX_EXT2_0_P:
        // pmux1[8:10], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x700);
        u4Ctrl |= 0x100;
        CKGEN_WRITE32(0x404, u4Ctrl);
        // pmux1[24:26], pmux1[28:30], function 2
        u4Ctrl = CKGEN_READ32(0x404) & (~0x77000000);
        u4Ctrl |= 0x22000000;
        CKGEN_WRITE32(0x404, u4Ctrl);
        // pmux2[19:18]
        u4Ctrl = CKGEN_READ32(0x408) & (~0xC0000);
        u4Ctrl |= 0x80000;
        CKGEN_WRITE32(0x408, u4Ctrl);

        // Disable EXT2_1, or all ts to ext2_1 not ext2_0
        // pmux1[8:10], pmux1[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x70);
        CKGEN_WRITE32(0x404, u4Ctrl);
        break;

    case DMX_PINMUX_EXT2_1_S:
        // pmux1[4:6], function 2
        u4Ctrl = CKGEN_READ32(0x404) & (~0x70);
        u4Ctrl |= 0x20;
        CKGEN_WRITE32(0x404, u4Ctrl);

        // Disable EXT2_0, or all ts to ext2_0 not ext2_1
        // pmux1[8:10], pmux1[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x7700);
        CKGEN_WRITE32(0x404, u4Ctrl);
        break;

    case DMX_PINMUX_EXT2_1_P:
        // pmux1[4:6], function 2
        u4Ctrl = CKGEN_READ32(0x404) & (~0x70);
        u4Ctrl |= 0x20;
        CKGEN_WRITE32(0x404, u4Ctrl);
        // pmux1[24:26], pmux1[28:30], function 2
        u4Ctrl = CKGEN_READ32(0x404) & (~0x77000000);
        u4Ctrl |= 0x22000000;
        CKGEN_WRITE32(0x404, u4Ctrl);
        // pmux2[19:18]
        u4Ctrl = CKGEN_READ32(0x408) & (~0xC0000);
        u4Ctrl |= 0x80000;

        // Disable EXT2_0, or all ts to ext2_0 not ext2_1
        // pmux1[8:10], pmux1[12:14], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~0x7700);
        CKGEN_WRITE32(0x404, u4Ctrl);
        break;

    case DMX_PINMUX_EXT3_S:
        break;

    case DMX_PINMUX_EXT3_P:
        break;

    case DMX_PINMUX_LEGACY_P:
        // pmux4[14:12], function1
        u4Ctrl = CKGEN_READ32(0x414) & (~0x7000);
        u4Ctrl |= 0x1000;
        CKGEN_WRITE32(0x414, u4Ctrl);
        // pmux1[18:16], function5, pmux1[14:12], function4, pmux1[10:8], function2
        u4Ctrl = CKGEN_READ32(0x404) & (~0x77700);
        u4Ctrl |= 0x54200;
        CKGEN_WRITE32(0x404, u4Ctrl);
        break;
#elif defined(CC_MT5368)
    case DMX_PINMUX_EXT_S:
    case DMX_PINMUX_EXT_1_S:
    case DMX_PINMUX_EXT_2_S:
        // pmux2[29:28], function 1, pmux2[31:30], function 1
        u4Ctrl = CKGEN_READ32(0x408) & (~(0xFU << 28));
        u4Ctrl |= (0x5 << 28);
        CKGEN_WRITE32(0x408, u4Ctrl);
        break;

    case DMX_PINMUX_EXT_P:
        // pmux2[29:28], function 1, pmux2[31:30], function 1
        u4Ctrl = CKGEN_READ32(0x408) & (~(0xFU << 28));
        u4Ctrl |= (0x5 << 28);
        CKGEN_WRITE32(0x408, u4Ctrl);
        // 418[0] = 1
        u4Ctrl = CKGEN_READ32(0x418) & (~(1 << 0));
        u4Ctrl |= (1 << 0);
        CKGEN_WRITE32(0x418, u4Ctrl);
        break;

    case DMX_PINMUX_EXT_1_P:
        // pmux2[29:28], function 1, pmux2[31:30], function 1
        // pmux2[0], should be 0, this bit setting priority is higher than AOLRCK - AOSDATA0
        u4Ctrl = CKGEN_READ32(0x408) & (~(0xFU << 28));
        u4Ctrl |= (0x5 << 28);
        u4Ctrl &= ~(0x1 << 0);
        CKGEN_WRITE32(0x408, u4Ctrl);
        // pmux0[9:7], function 3, pmux0[12:10], function 3, pmux0[15:13], function 3,
        // pmux0[18:16], function 3, pmux0[21:19], function 3
        u4Ctrl = CKGEN_READ32(0x400) & (~(0x7FFF << 7));
        u4Ctrl |= (0x36DB << 7);
        CKGEN_WRITE32(0x400, u4Ctrl);
        break;

    case DMX_PINMUX_EXT_2_P:
        // pmux2[29:28], function 1, pmux2[31:30], function 1
        u4Ctrl = CKGEN_READ32(0x408) & (~(0xFU << 28));
        u4Ctrl |= (0x5 << 28);
        CKGEN_WRITE32(0x408, u4Ctrl);
        // pmux2[1:0],pmux1[31] function 2
        u4Ctrl = CKGEN_READ32(0x404) & (~(1U << 31));
        CKGEN_WRITE32(0x404, u4Ctrl);
        u4Ctrl = CKGEN_READ32(0x408) & (~(3 << 0));
        u4Ctrl |= (1 << 0);
        CKGEN_WRITE32(0x408, u4Ctrl);
        // pmux2[4:2], function 2, pmux2[7:5], function 2
        u4Ctrl = CKGEN_READ32(0x408) & (~(0x3F << 2));
        u4Ctrl |= (0x12 << 2);
        CKGEN_WRITE32(0x408, u4Ctrl);
        break;

    case DMX_PINMUX_EXT2_S:
        // pmux3[1:0], function 1
        u4Ctrl = CKGEN_READ32(0x40C) & (~(0x3 << 0));
        u4Ctrl |= (0x1 << 0);
        CKGEN_WRITE32(0x40C, u4Ctrl);
        // 418[0] = 0
        u4Ctrl = CKGEN_READ32(0x418) & (~(1 << 0));
        CKGEN_WRITE32(0x418, u4Ctrl);
		break;
    case DMX_PINMUX_EXT2_P:
        // pmux3[1:0], function 1
        u4Ctrl = CKGEN_READ32(0x40C) & (~(0x3 << 0));
        u4Ctrl |= (0x1 << 0);
        CKGEN_WRITE32(0x40C, u4Ctrl);
        // 418[0] = 0
        u4Ctrl = CKGEN_READ32(0x418) & (~(1 << 0));
        CKGEN_WRITE32(0x418, u4Ctrl);
        // pmux3[4:2], function 3, pmux3[7:5], function 3
        u4Ctrl = CKGEN_READ32(0x40C) & (~(0x3F << 2));
        u4Ctrl |= (0x1B << 2);
        CKGEN_WRITE32(0x40C, u4Ctrl);
        break;
#elif defined(CC_MT5389)
    case DMX_PINMUX_EXT_S:
        #ifndef CC_DMX_EMULATION
        if(!IS_SupportTsIn())
        {
            return;
        }
        #endif
        // pmux1[21:19], function 1, pmux1[18:16], function 1
        u4Ctrl = CKGEN_READ32(0x404) & (~(0x3FU << 16));
        u4Ctrl |= (0x9 << 16);
        CKGEN_WRITE32(0x404, u4Ctrl);
        break;
#endif
    default:
        return;
    }
}

//-----------------------------------------------------------------------------
/** _DMX_BypassErrorHandlingTable_Enable
	TRUE: bypass error handling table
	FALSE: DO framr error handling table
 */
//-----------------------------------------------------------------------------
void _DMX_BypassErrorHandlingTable_Enable(UINT8 u1Framer,BOOL fgEnable)
{
    UINT32 u4Reg;


    ASSERT(u1Framer < 4);

    if(u1Framer == 2)
    {
        // Framer 2 only use internal
        return;
    }
	else if (u1Framer == 0)
	{
		if (fgEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
			u4Reg |= (0x1 << 4);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
			u4Reg &= ~(0x1 << 4);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
		}
	}
	else if (u1Framer == 1)
	{
		if (fgEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
			u4Reg |= (0x1 << 5);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
			u4Reg &= ~(0x1 << 5);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
		}

	}
	else if (u1Framer == 3)
	{
		if (fgEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
			u4Reg |= (0x1 << 6);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
			u4Reg &= ~(0x1 << 6);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
		}
	}
	else
	{
		Printf("Input the wrong Framer number!\n");
	}
}

//-----------------------------------------------------------------------------
/** _DMX_SetFramer

 */
//-----------------------------------------------------------------------------
void _DMX_SetFramer(UINT8 u1FramerIdx, BOOL fgEnable, BOOL fgPareallel, BOOL fgExtSync, BOOL fgPosEdge)
{
	UINT32 u4Reg, u4Val = 0x0;

	ASSERT(u1FramerIdx <= 3);

	if (u1FramerIdx == 0x2)
	{
		return;
	}

	if (u1FramerIdx == 0x3)
	{
		u1FramerIdx -= 0x1;
	}

	//disable framer first, before set framer again.
	u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
	u4Reg &= ~(0x1 << (4 * u1FramerIdx));
	DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);

	//set value
	u4Val |= (fgEnable ? 0x1 : 0x0); //framer enable
	u4Val |= ((fgPareallel ? 0x1 : 0x0) << 1); //parallel or serial
	u4Val |= ((fgExtSync ? 0x1 : 0x0) << 2); //external or internal
	u4Val |= ((fgPosEdge ? 0x1 : 0x0) << 3); //external or internal

	//set framer
	u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
	u4Reg &= ~(0xF << (4 * u1FramerIdx));
	u4Reg |= (u4Val << (4 * u1FramerIdx));
	DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);

}
//-----------------------------------------------------------------------------
/** _DMX_Framer_130byteEnable

 */
//-----------------------------------------------------------------------------
BOOL _DMX_Framer_130byteEnable(UINT8 u1Framer, BOOL fgDemod130byteTs, BOOL fgInputEnable, UINT8 u1InputPktSize ,BOOL fgOutputEnable, UINT8 u1OutputPktSize)
{
	UINT32 u4Reg;

	ASSERT(u1Framer < 4);

    if(u1Framer == 2)
    {
        Printf("Framer 2 no need set 130 byte enable!\n");
        return TRUE;
    }
	else if (u1Framer == 0)
	{
		//set input packet size
		u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
		u4Reg &= ~(0xFF);
		u4Reg |= u1InputPktSize;
    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);

		//set input enable
		if (fgInputEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
			u4Reg |= (0x1 << 8);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
			u4Reg &= ~(0x1 << 8);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
		}

		//set output packet size
		u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
		u4Reg &= ~(0xFF);
		u4Reg |= (u1OutputPktSize);
		DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

		//set output enable  and dbm auto switch
		if (fgOutputEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg |= (0x1 << 24); //output enable
			u4Reg |= (0x1 << 27); //dbm auto switch
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg &= ~(0x1 << 24);
			u4Reg &= ~(0x1 << 27);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}

		//set Demod 130byte ts enable or disable 17920[28]
		if (fgDemod130byteTs)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg |= (0x1 << 28);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg &= ~(0x1 << 28);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}

	}
	else if (u1Framer == 1)
	{
		//set input packet size
		u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
		u4Reg &= ~(0xFF << 16);
		u4Reg |= (u1InputPktSize << 16);
    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);

		//set input enable
		if (fgInputEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
			u4Reg |= (0x1 << 24);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
			u4Reg &= ~(0x1 << 24);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
		}

		//set output packet size
		u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
		u4Reg &= ~(0xFF << 8);
		u4Reg |= (u1OutputPktSize << 8);
		DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

		//set output enable  and dbm auto switch
		if (fgOutputEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg |= (0x1 << 25); //output enable
			u4Reg |= (0x1 << 27); //dbm auto switch
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg &= ~(0x1 << 25);
			u4Reg &= ~(0x1 << 27);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}

		//set Demod 130byte ts enable or disable 17920[29]
		if (fgDemod130byteTs)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg |= (0x1 << 29);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg &= ~(0x1 << 29);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}

	}
	else if (u1Framer == 3)
	{
		//set input packet size
		u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2);
		u4Reg &= ~(0xFF);
		u4Reg |= (u1InputPktSize);
    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);

		//set input enable
		if (fgInputEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2);
			u4Reg |= (0x1 << 8);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2);
			u4Reg &= ~(0x1 << 8);
	    	DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);
		}

		//set output packet size
		u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
		u4Reg &= ~(0xFF << 16);
		u4Reg |= (u1OutputPktSize << 16);
		DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

		//set output enable  and dbm auto switch
		if (fgOutputEnable)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg |= (0x1 << 26); //output enable
			u4Reg |= (0x1 << 27); //dbm auto switch
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg &= ~(0x1 << 26);
			u4Reg &= ~(0x1 << 27);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}

		//set Demod 130byte ts enable or disable 17920[30]
		if (fgDemod130byteTs)
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg |= (0x1 << 30); //output enable
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}
		else
		{
			u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
			u4Reg &= ~(0x1 << 30);
			DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
		}

	}
	else
	{
		Printf("Input the wrong Framer number!\n");
        return FALSE;
	}

	return TRUE;

}


//-----------------------------------------------------------------------------
/** _DMX_SetFramerMode
 */
//-----------------------------------------------------------------------------
void _DMX_SetFramerMode(UINT8 u1Framer, DMX_FRAMER_MODE_T eMode,
                        BOOL fgExtSync, BOOL fgPosEdge)
{
    UINT32 u4Ctrl, u4RegOffset;
    UINT8 u1FramerControl;
    UINT32 u4Ctrl1;

    ASSERT(u1Framer < 4);

    if(u1Framer == 2)
    {
        // Framer 2 only use internal
        return;
    }

    if(eMode == DMX_FRAMER_TWOBIT)
    {
        u4Ctrl1 = DMXCMD_READ32(DMX_REG_CONFIG1);
        u4Ctrl1 &= ~(3<<26);

        if(u1Framer == 0)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
            u4Ctrl &= ~(3 << 24);                   //clear framer 0 2-bit mode format
            u4Ctrl |= (2 << 24);                    //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
            u4Ctrl1 |= (1<<26);
        }
        else if(u1Framer == 1)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
            u4Ctrl &= ~(3 << 26);                   //clear framer 1 2-bit mode format
            u4Ctrl |= (2 << 26);                    //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
            u4Ctrl1 |= (1<<27);
        }
        else if(u1Framer == 3)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Ctrl &= ~(3);                         //clear framer 1 2-bit mode format
            u4Ctrl |= (2);                          //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Ctrl);
            // ?
        }
        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl1);
    }
	else
	{
		u4Ctrl1 = DMXCMD_READ32(DMX_REG_CONFIG1);
		u4Ctrl1 &= ~(3<<26);
		DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl1);
		u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
		u4Ctrl &= ~(0xF << 24);
		DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
	}


    u1FramerControl = 0x1;
    if((eMode == DMX_FRAMER_PARALLEL) || (eMode == DMX_FRAMER_TWOBIT))
    {
        u1FramerControl |= 0x2;
    }

    if(fgExtSync)
    {
        u1FramerControl |= 0x4;
    }

    if(fgPosEdge)
    {
        u1FramerControl |= 0x8;
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4RegOffset = (u1Framer == 3) ? 2 : (UINT32)u1Framer;
    if(((u4Ctrl >> (u4RegOffset * 4)) & 0xF) != u1FramerControl)
    {
    	//disable framer before set framer again
    	u4Ctrl = (u4Ctrl & ~((0xf) << (4*u4RegOffset)));
    	DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
		//set framer again, Becarefull
        u4Ctrl = (u4Ctrl & ~((0xf) << (4*u4RegOffset))) | ((u1FramerControl) << (4*u4RegOffset));
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        _DMX_ResetFramer(u1Framer);
    }
}


//-----------------------------------------------------------------------------
/** _DmxLoadIMem
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxLoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len)
{
    UINT32 u4Cmd, u4Data, i;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    ASSERT(pu4IData != NULL);

    // Check uP status
    if (_DmxIsFTIRunning())
    {
        if (!fgForceWrite)
        {
            LOG(7, "FTI uP is running, can't load to I-mem!\n");
            return FALSE;
        }
        else
        {
            LOG(7, "FTI uP is running, now will halt it\n");
            _DMX_EnableFTI(FALSE);
            while (_DmxIsFTIRunning())
            {
                LOG(5, "wait uP stop...\n");
            }
        }
    }

    // Clear the "FTuP instruction mode" flag.
    DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= ~(1 << 25);

    // Load to I-mem
    for (i = 0; i < u4Len; i++)
    {

        _DMX_Lock();

        // Issue a write command
        DMXCMD_WRITE32(DMX_REG_MEM_DATA, pu4IData[i]);
        u4Cmd = (UINT32)((1 << 0) | (2 << 8) | (i << 16));
        DMXCMD_WRITE32(DMX_REG_MEM_CMD,  u4Cmd);

        // Wait for ready
        while (1)
        {
            u4Cmd = DMXCMD_READ32(DMX_REG_MEM_CMD);
            if (((u4Cmd >> 8) & 0xf) == 0)
            {
                break;
            }
        }

        _DMX_Unlock();
    }

    // Verify
    for (i = 0; i < u4Len; i++)
    {
        _DMX_Lock();

        // Issue a read command
        u4Cmd = (UINT32)((1 << 0) | (1 << 8) | (i << 16));
        DMXCMD_WRITE32(DMX_REG_MEM_CMD, u4Cmd);

        // Wait for ready
        while (1)
        {
            u4Cmd = DMXCMD_READ32(DMX_REG_MEM_CMD);
            if (((u4Cmd >> 8) & 0xf) == 0)
            {
                break;
            }
        }
        // Verify
        u4Data = DMXCMD_READ32(DMX_REG_MEM_DATA);

        _DMX_Unlock();

        if (u4Data != pu4IData[i])
        {
            LOG(1, "Load IMem error, at word %u, write 0x%08x, read 0x%08x\n",
                i, pu4IData[i], u4Data);
            return FALSE;
        }
    }

    _u4uCodeVersion = pu4IData[0];
    u2CodeVersion = (UINT16)(pu4IData[0] & 0xffff);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0xf);
    u1Minor = (UINT8)(u2CodeVersion & 0xff);
    LOG(5, "Demux uCode version: %u.%u\n", u1Major, u1Minor);

    // Reset demux (uP), so uP can run into the entry pointer of uCode
    VERIFY(_DMX_Reset());

    // Suppress warnings in lint and release build
    UNUSED(u1Major);
    UNUSED(u1Minor);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_LoadIMem
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_LoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len)
{
    UINT8 i = 0;
    BOOL fgRet;
    for(i = 0; i < 10; i++)
    {
        fgRet = _DmxLoadIMem(fgForceWrite, pu4IData, u4Len);
        if(fgRet)
        {
            return TRUE;
        }
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DmxInitFVR
 */
//-----------------------------------------------------------------------------
static void _DmxInitFVR(void)
{
    UINT32 i, u4Ctrl;

    //-----------------------------------------------------
    // Reset DMX Record hardware
    //-----------------------------------------------------
    x_memset((void*)&(FVR_GBL_ARY_W(0, 0)), 0, FVR_GBL_SIZE * 4);
    x_memset((void*)&(FVR_GBL_ARY_W(1, 0)), 0, FVR_GBL_SIZE * 4);
    for (i = 0; i < DMX_NUM_FVR_HW_PID_COUNT; i++)
    {
        FVR_PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(FVR_PER_PID_S(i)), 0, FVR_PID_SIZE * 4);
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF;
    u4Ctrl |= (FVR_PER_PID_OFFSET) << 16;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFD00FEFF;
    u4Ctrl |= (1 << 25);                    // Disable record path
    u4Ctrl |= ((FVR_PID_SIZE * 4) << 16); // Set key pid size
    u4Ctrl |= (1 << 8);                     // Direct map
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Disable insert timestamp
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
}


//-----------------------------------------------------------------------------
/** _DmxTSSoftInit
 *  Soft-initialize demux
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTSSoftInit(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    //---------------------------------------------------------
    // Clear FVR SRAM
    //---------------------------------------------------------

    //---------------------------------------------------------
    // Clear DMX SRAM and struct
    //---------------------------------------------------------
    // Clear HW PID index table and PID DMEM
    for (i = 0; i < DMX_NUM_HW_PID_COUNT; i++)
    {
        PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(PID_S(i)), 0, DMX_DMEM_ENTRY_LEN * 4);
        // Debug, reset continuity counter to 0xff
        PID_S_W(i, 3) = 0xffff0000;
    }

    // PID structures
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_STRUCT_T* prPidStruct;
        UINT8 u1SerialNum;

        prPidStruct = _DMX_GetPidStruct(i);
        u1SerialNum = prPidStruct->u1SerialNumber;
        x_memset((void*)prPidStruct, 0, sizeof(PID_STRUCT_T));
        prPidStruct->u1SerialNumber = u1SerialNum;
    }

    // Clear Pattern match DMEM
    x_memset((void*)DMX_PATTERN_MATCH_BASE, 0, DMX_DMEM_PATTERN_MATCH_LEN * 4);

    // Clear section filter
    x_memset((void*)DMX_SECTION_POSNEG, 0, 8 * DMX_NUM_HW_FILTER_COUNT);
    x_memset((void*)DMX_SECTION_CONTROL, 0, 4 * DMX_NUM_HW_FILTER_COUNT);
    x_memset((void*)DMX_SECTION_PATTERN, 0, 8 * DMX_NUM_HW_FILTER_COUNT);
    x_memset((void*)DMX_SECTION_MASK, 0, 8 * DMX_NUM_HW_FILTER_COUNT);

    x_memset((void*)DMX_ONEBYTE_FILTER_BASE, 0, 2 * DMX_NUM_FVR_HW_FILTER_COUNT);


    if(GetTargetCountry() != COUNTRY_US)
    {
        #if defined(CC_DMX_USE_CT) && !defined(CC_DRIVER_BUILD)
        LOG(0, "You should define DMX_NT_UCODE_DISABLE in .cfg file for Non-US model!\n");
        ASSERT(0);
        #endif
		CT_TSIDX_SETTING = 0xFFFFFFFF;
		CT_SETTING = 0x0;
    }
    else
    {
        // Set the byte 3 of CT_SETTING to 0xFF which represents an invalid
        // PID Index.  Other values greater than 31 will do, but I simply use 0xFF.
        if((BIM_READ32(0x664) & 0x4) == 0x4)
        {
            // Demod does not clear VCT.  DMX has the choice to clear VCT or not.
            #ifdef CC_DMX_USE_CT
            CT_SETTING = 0xFF000002;  // DMX clears VCT.
            #else
            CT_SETTING = 0xFF000000;  // DMX does not clear VCT.
            #endif  // CC_DMX_USE_CT
            CT_TSIDX_SETTING = 0xFFFFFFFF;
        }
        else
        {
            // Demod clears VCT.
            CT_SETTING = 0xFF000001;   // DMX must use the CRC result from Demod.
            #ifndef CC_DMX_USE_CT
            ASSERT(0);  // There shall be no such usage scenario.
            #endif  // CC_DMX_USE_CT
        }
    }


    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF0000;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    // Clear section filters
    for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
    {
        FILTER_STRUCT_T *prFilterStruct;

        prFilterStruct = _DMX_GetFilterStruct(i);
        x_memset((void*)prFilterStruct, 0, sizeof(FILTER_STRUCT_T));
    }
    _DMX_Unlock();

    // Initialize interrupt handler
    _DMX_InitISR();

    // Initialize common routines
    VERIFY(_DMX_InitAPI());

    _DMX_Lock();

    DMXCMD_WRITE32(DMX_REG_FRAMER_ERROR_HANDLE, 0xf0c0f0c0);

    //-----------------------------------------------------
    // Record default setting
    //-----------------------------------------------------
    // Disable record
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) | (1 << 25);
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Set packet length to 188 bytes
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0x08;          // maximum burst size
    u4Ctrl = (u4Ctrl & 0xFFFF00FF) | (188 << 8);    // packet len = 188 bytes
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x47 << 16);  // TS packet sync byte
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);

    // Setup framer control
    _DMX_SetFramerMode(0, DMX_FRAMER_PARALLEL, TRUE, TRUE);
    _DMX_SetFramerMode(1, DMX_FRAMER_PARALLEL, TRUE, TRUE);

    // Set DBM to normal mode and enable DBM
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl = (u4Ctrl & 0xcfffffff) | (1 << 30);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);

    // Set PID structure size
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1);
    u4Ctrl &= 0xffffEf00;                       // Select Group 1 DRAM agent
    u4Ctrl |= (DMX_DMEM_ENTRY_LEN * 4);         // Size of each PID data
    u4Ctrl |= (1 << 9);                         // DMA delay ack, debug
    u4Ctrl |= (1 << 10);                        // Enable multi-PID channel
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    _DMX_Unlock();

    _DMX_ResetFramer(0);
    _DMX_ResetFramer(1);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxInitBroadcastTS
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitBroadcastTS(void)
{
    UINT32 u4Ctrl;

#ifdef DMX_MEASURE_PSI_TIME
    _DMX_PSI_ResetMaxTime();
#endif  // DMX_MEASURE_PSI_TIME
    // Load default uCode (for normal transport stream)
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM, FTI_IMEM_LEN))
    {
        return FALSE;
    }

    // Soft init
    if (!_DmxTSSoftInit())
    {
        return FALSE;
    }

    _DmxInitFVR();

    _DMX_Lock();
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    // Set DBM output spacing
    u4Ctrl = (u4Ctrl & 0xff00ffff) | (8 << 16);
    // Set DBM max playback TS packets
    u4Ctrl = (u4Ctrl & 0xffff00ff) | (1 << 8);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);
    _DMX_Unlock();

    // Important: Set framer after calling _DmxTSSoftInit().
    // Be sure to set the register while the FTI is disabled.
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x03 << 16);  // Sync Lock
    u4Ctrl = (u4Ctrl & 0x00FFFFFF) | (0x01 << 24);  // Sync Loss
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

	//enable framer pre-full to control DDI and play back speed
	u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG4);
	u4Ctrl |= 0x80000000;
	DMXCMD_WRITE32(DMX_REG_CONFIG4,u4Ctrl);

    _DMX_SetFramerMode(0, DMX_FRAMER_SERIAL, FALSE, TRUE);
    _DMX_SetFramerMode(1, DMX_FRAMER_SERIAL, FALSE, TRUE);

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    return TRUE;
}

//-----------------------------------------------------------------------------
static BOOL _DmxInitDiag(void)
{
    // Load test uCode
    if (!_DMX_LoadIMem(TRUE, FTI_IMEM_TEST_LATCH, FTI_IMEM_LEN_TEST_LATCH))
    {
        return FALSE;
    }

    // Reset FTI
    if (!_DmxTSSoftInit())
    {
        return FALSE;
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxFrontEndCust
 */
//-----------------------------------------------------------------------------
BOOL _DmxCustFrontEnd(void)
{
    BOOL fgRet = TRUE;
    UINT8 i;
    DMX_FRONTEND_CONFIG_T* prDmxFeCfgTable;

    UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));

    for(i = 0; i < DMX_FRAMER_COUNT; i++)
    {
        if(prDmxFeCfgTable[i].fgEnable)
        {
            DMX_FRONTEND_T eFrontEnd;
            if(prDmxFeCfgTable[i].ePinSet == DMX_PINSET_INTERNAL)
            {
                eFrontEnd = DMX_FE_INTERNAL;
            }
            else
            {
                eFrontEnd = (DMX_FRONTEND_T)((UINT32)prDmxFeCfgTable[i].ePinSet | (UINT32)prDmxFeCfgTable[i].eMode);
            }

            if(!_DMX_SetFrontEndEx(i, eFrontEnd))
            {
                fgRet = FALSE;
            }
        }
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DmxInitHWPath
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitHWPath(void)
{
#if 0   // Enable framer2
    UINT32 u4Reg;

    // Framer2 size
    u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
    u4Reg |= 0x1;   // Enable framer2
    DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);
    _DMX_ResetFramer(2);
#endif

    if(!_DmxCustFrontEnd())
    {
        return FALSE;
    }

#ifdef ENABLE_MULTIMEDIA
    //----------------------------------
    // DDI path
    //----------------------------------
    //_DMX_DDI_SetPort(DMX_DDI_PORT_NO_FRAMER, TRUE);       // DDI chunk data: avi, mkv, mp4
    _DMX_DDI_SetPort(DMX_DDI_PORT_FRAMER1, TRUE);

    //----------------------------------
    // PVR path
    //----------------------------------
    _DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_DBM); // PVR for TS file
    UNUSED(_DMX_PVRPlay_SetMoveMode(DMX_TSFMT_188, TRUE));
#endif  // ENABLE_MULTIMEDIA

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxSetTSInputCkgen
 */
//-----------------------------------------------------------------------------
static void _DmxSetTSInputCkgen(UINT8 u1Idx, DMX_FRONTEND_T eFrontEnd)
{
#ifndef CC_DMX_FPGA_TEST
    BOOL fgToggle;
    DMX_FRONTEND_T ePreType;
    UINT32 u4Reg;
    UINT8 au1Offset[4] = { 0, 16, 0, 24 };
    UINT8 i;

    ASSERT(u1Idx < DMX_FRAMER_COUNT);
    if(u1Idx >= DMX_FRAMER_COUNT)
    {
        return;
    }

    //--------------------------
    // Set Framer Ckgen
    //--------------------------
    ePreType = _aeDmxFramerInput[u1Idx];
    fgToggle = FALSE;
    if((ePreType != DMX_FE_DDI) && (ePreType != DMX_FE_TSOUT))
    {
        fgToggle = TRUE;
    }

    u4Reg = CKGEN_READ32(0x24C);

    //// Double confirm
    //ASSERT((u4Reg&0x3) == _aeDmxFramerInput[0]);
    //ASSERT(((u4Reg>>au1Offset[1])&0x3) == _aeDmxFramerInput[1]);
    //ASSERT(((u4Reg>>au1Offset[3])&0x3) == _aeDmxFramerInput[3]);

    u4Reg &= ~(0x3 << au1Offset[u1Idx]);

    // DDI can only input one framer
    if(eFrontEnd == DMX_FE_DDI)
    {
        for(i=0; i<4; i++)
        {
            if(i == u1Idx)
            {
                continue;
            }

            if(_aeDmxFramerInput[i] == DMX_FE_DDI)
            {
                u4Reg &= ~(0x3 << au1Offset[i]);
                u4Reg |= ((UINT32)DMX_FE_TSOUT << au1Offset[i]);
                _aeDmxFramerInput[i] = DMX_FE_TSOUT;
            }
        }
    }

    if((eFrontEnd == DMX_FE_INTERNAL) || (eFrontEnd == DMX_FE_DDI) ||
       (eFrontEnd == DMX_FE_TSOUT))
    {
        u4Reg |= (((UINT32)eFrontEnd & 0x3) << au1Offset[u1Idx]);
    }
    else
    {
        u4Reg |= (1 << au1Offset[u1Idx]);
    }

    _aeDmxFramerInput[u1Idx] = eFrontEnd;

    CKGEN_WRITE32(0x24C, u4Reg);

    if(fgToggle)
    {
        _DmxCkgenToggle(u1Idx);
    }
#endif // CC_DMX_FPGA_TEST
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_ResetDbmSafely
 *
 *  Reset DBM safely (at packet boundary) in three steps.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetDbmSafely(VOID)
{
    BOOL fgRet = TRUE;
    UINT32 u4Reg, u4Counter;

    // 1/3. Disable DBM
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg &= ~(1 << 30);                            // Clear "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

    // 2/3. Polling the value of 0x20017410
    //      if bit[16..12] != 0x10
    //          then reset DBM
    u4Counter = 0;
    while (u4Counter < DMX_MAX_DBM_WAIT_COUNT)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
        if (((u4Reg >> 12) & 0x1F) == 0x10)
        {
            x_thread_delay(1);
            u4Counter++;
        }
        else
        {
            break;
        }
    }

    // 3/3. Reset DBM which also resets Framer and PID index table. Enable DBM.
    if (u4Counter == DMX_MAX_DBM_WAIT_COUNT)
    {
        LOG(3, "Failed to reset DBM safely (still outputing TS packets)!\n");
        fgRet = FALSE;
    }
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg |= 0x80000000;                            // Set "Reset" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    u4Reg &= 0x7FFFFFFF;                            // Clear "Reset" bit
    u4Reg |= (1 << 30);                             // Set "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_ClearInterruptQueue
 */
//-----------------------------------------------------------------------------
VOID _DMX_ClearInterruptQueue(VOID)
{
    UINT32 i;

    _DMX_Lock();

    DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
    }

    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_ResetFTuP
 *
 *  This function resets the uP, and sets its "boot address" to 0.
 */
//-----------------------------------------------------------------------------
VOID _DMX_ResetFTuP(VOID)
{
    // Reset uP and its booting address.
    UINT32 u4Reg;
    _DMX_Lock();
    _fgMicroProcessorStopped = TRUE;
    _DMX_Unlock();
    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg |= 0x81000000;                        // Disable "FTuP Stalling"
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);
    while (1)
    {
        if ((DMXCMD_READ32(DMX_REG_FTuP_CONTROL) & (1 << 30)) != 0)
        {
            break;
        }
    }

    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg &= 0xFeFF0000;        // Enable "FTuP Stalling"; Clear "Boot address"
    u4Reg |= 0x1;               // Change "Boot address" to 1.
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);        // Write it twice!
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);        // Write it twice!

    _DMX_ClearInterruptQueue();

    // Enable uP
    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg &= 0x7FFF0000;        // Change "Boot address" to 0.
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);
    _DMX_Lock();
    _fgMicroProcessorStopped = FALSE;
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_EnableFTI
 *  Enable or disable FTI
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_EnableFTI(BOOL fgEnable)
{
    UINT32 u4Ctrl, i;

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
    if (fgEnable)
    {
        // Enable FTI
        u4Ctrl &= 0x3fffffff;
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);

        // Enable uP
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= 0x7fffffff;
        _DMX_Lock();
        _fgMicroProcessorStopped = FALSE;
        _DMX_Unlock();
    }
    else
    {
        // Halt uP
        //DMXCMD_REG32(DMX_REG_FTuP_CONTROL) |= 0x80000000;
        // MT5391, turn on "bit[24]: disble uP stall"
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) |= 0x81000000;   // MT5391
        while (1)
        {
            if ((DMXCMD_READ32(DMX_REG_FTuP_CONTROL) & (1 << 30)) != 0)
            {
                break;
            }
        }

        // MT5391, turn off "bit[24]: disble uP stall"
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= 0xfeffffff;

        // Clear all interrupts
        for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
        {
            DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
            DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
        }

        // Halt FTI
        _DMX_Lock();
        _fgMicroProcessorStopped = TRUE;
        _DMX_Unlock();
        u4Ctrl = ((u4Ctrl & 0x3fffffff) | 0x40000000);
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);

        while(1)
        {
            // Check if FTI is halted
            if (0x1 != ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
            {
                x_thread_delay(1);
            }
            else
            {
                break;
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** _DMX_Reset
 *  Reset FTI
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Reset(void)
{
    UINT32 i;

    _DMX_Lock();

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    }

    // Reset FTI
    _fgMicroProcessorStopped = TRUE;
    DMXCMD_WRITE32(DMX_REG_CONTROL, 0x8f000000);

    _DMX_Unlock();

    // Wait for ready
    while(1)
    {
        // Check if FTI is halted
        if (0x1 != ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
        {
            x_thread_delay(1);
        }
        else
        {
            break;
        }
    }

#ifndef CC_DMX_FPGA_TEST
#ifdef __PCR_RECOVERY__
    // Initialize PCR whenever demux is reset
    if(!_DMX_GetIsPanicReset())
    {
        _DMX_PcrInit();
    }
    else
    {
        _DMX_PcrReInitHW();
    }
#endif  // __PCR_RECOVERY__
#endif

    // Set to previous value before reset
    _DMX_SetExternalFramer(_eDmxExtFramer);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFrontEnd
 */
//-----------------------------------------------------------------------------
DMX_FRONTEND_T _DMX_GetFrontEnd(UINT8 u1TsIdx)
{
    DMX_FRONTEND_T eFE;

    if(u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return DMX_FE_NULL;
    }

    _DMX_Lock();
    eFE = _aeDmxFramerInput[u1TsIdx];
    _DMX_Unlock();

    return eFE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFrontEndEx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFrontEndEx(UINT8 u1Idx, DMX_FRONTEND_T eFrontEnd)
{
    DMX_PINMUX_SEL_T ePinmux;
    DMX_EXT_CKGEN_T eCkgen;
    DMX_FRONTEND_CONFIG_T* prDmxFeCfgTable;
    DMX_EDGE_T eEdge;
    DMX_SYNC_T eSync;

    if(u1Idx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    #ifdef CC_MT5368
    if(((u1Idx == 0) && ((eFrontEnd >= DMX_FE_EXT2_0_S) && (eFrontEnd <= DMX_FE_EXT2_0_2BIT))) ||
        ((u1Idx == 1) && ((eFrontEnd >= DMX_FE_EXT_S) && (eFrontEnd <= DMX_FE_EXT_2_2BIT))))
    {
        LOG(1, "This is a wrong front end setup TsIdx: %d, type: 0x%x.\n", u1Idx, eFrontEnd);
        return FALSE;
    }
    #endif // CC_MT5368

	#ifdef CC_MT5389 //mt5389 only have sirial external pin
    if(((eFrontEnd >= DMX_FE_EXT_1_S) && (eFrontEnd <= DMX_FE_EXT2_0_2BIT)) || (eFrontEnd == DMX_FE_EXT_P))
    {
        LOG(1, "This is a wrong front end setup TsIdx: %d, type: 0x%x.\n", u1Idx, eFrontEnd);
        return FALSE;
    }

    // 5389 bonding check
    #ifndef CC_DMX_EMULATION
    if((eFrontEnd == DMX_FE_EXT_S) || (eFrontEnd == DMX_FE_EXT_2BIT))
    {
        if(!IS_SupportTsIn())
        {
            return FALSE;
        }
    }
    #endif

    #endif // CC_MT5389

    //--------------------
    // Set pinmux
    //--------------------
    if(eFrontEnd >= 0x10)    // External
    {
        ePinmux = (DMX_PINMUX_SEL_T)eFrontEnd;
        if(((UINT32)eFrontEnd & 0xF) == 2)
        {
            ePinmux = (DMX_PINMUX_SEL_T)((UINT32)eFrontEnd - 2);
        }
        _DmxSetExtPinmux(ePinmux);
    }

    //--------------------
    // Set Ext ckgen
    //--------------------
    // only 96 has internal CI
    #ifdef CC_MT5396
    if(eFrontEnd == DMX_FE_CI)
    {
        _DmxSetExtCkgen(u1Idx, DMX_CKGEN_CI);
    }
    #endif
    if(eFrontEnd >= 0x10)    // External
    {
        switch((UINT32)(eFrontEnd >> 4) & 0xF)
        {
#if defined(CC_MT5396)
        case 1:
        case 5:
        default:
            eCkgen = DMX_CKGEN_EXT_0;
            break;
        case 2:
        case 3:
            eCkgen = DMX_CKGEN_EXT_2;
            break;
        case 4:
            eCkgen = DMX_CKGEN_EXT_3;
            break;
#elif defined(CC_MT5368)
        case 1:
        case 2:
        case 3:
        default:
            eCkgen = DMX_CKGEN_EXT_0;
            break;
        case 4:
            eCkgen = DMX_CKGEN_EXT_2;
            break;
#endif
        }
        _DmxSetExtCkgen(u1Idx, eCkgen);
    }

    //--------------------
    // Set framer mode
    //--------------------
    UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));
    eEdge = prDmxFeCfgTable[u1Idx].eEdge;
    eSync = prDmxFeCfgTable[u1Idx].eSync;

    if(eFrontEnd == DMX_FE_INTERNAL)
    {
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL,
            (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
    }
    else if(eFrontEnd == DMX_FE_DDI)
    {
        #ifdef CC_DMX_EMULATION
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, TRUE, TRUE);
        #else
		_DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, FALSE, TRUE);
        #endif // CC_DMX_EMULATION
    }
    else if(eFrontEnd == DMX_FE_TSOUT)
    {
        #ifdef CC_DMX_EMULATION
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, TRUE, TRUE);
        #else
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, FALSE, TRUE);
        #endif // CC_DMX_EMULATION
    }
    else
    {
        switch((UINT32)eFrontEnd & 0xF)
        {
        case 0:
            _DMX_SetFramerMode(u1Idx, DMX_FRAMER_SERIAL,
                (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
            break;
        case 1:
        default:
            _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL,
                (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
            break;
        case 2:
            _DMX_SetFramerMode(u1Idx, DMX_FRAMER_TWOBIT,
                (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
            break;
        }
    }

    //--------------------
    // Set framer input
    //--------------------
    _DmxSetTSInputCkgen(u1Idx, eFrontEnd);

    //--------------------
    // Update TS index
    //--------------------

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ResetFramer
 */
//-----------------------------------------------------------------------------
void _DMX_ResetFramer(UINT8 u1Framer)
{
#if !defined(CC_DMX_EMULATION) && !defined(__MODEL_slt__)
    UINT32 u4Ctrl;

	//enable bit to reset the DBM some framer releated register
	u4Ctrl = DMXCMD_READ32(DMX_REG_RECOVERTY_SET);
	u4Ctrl |= (0x1 << 5);
	DMXCMD_WRITE32(DMX_REG_RECOVERTY_SET, u4Ctrl);

    // Reset framer - bit12 for framer0 reset, bi13 for framer 1 reset
    if(u1Framer == 0)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x1000);
        u4Ctrl |= 0x1000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x1000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    }
    else if(u1Framer == 1)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x2000);
        u4Ctrl |= 0x2000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x2000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    }
    else if(u1Framer == 2)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
        u4Ctrl |= 0x10;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Ctrl);
        HAL_Delay_us(1);
        u4Ctrl &= ~0x10;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Ctrl);
    }
    else if(u1Framer == 3)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x8000);
        u4Ctrl |= 0x8000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x8000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    }
    else
    {
        ASSERT(0);
    }

    LOG(9, "Reset framer\n");
#endif
}


//-----------------------------------------------------------------------------
/** _DMX_GetFramerIndex
 *  Get DTV framer
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_GetFramerIndex(void)
{
    // Fixme, 0 use as default first
    return 0;
}


//-----------------------------------------------------------------------------
/** _DMX_Start
 *  Start demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Start(void)
{
    UINT32 u4Ctrl;

    _DMX_Lock();

    // Enable framers
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl |= (1 << 0) | (1 << 4);
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Stop
 *  Stop demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Stop(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    // Disable framers
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl &= ~((1 << 0) | (1 << 4));
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxTestFTuPLatch
 *  Test uP latch-up
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_TestFTuPLatch(void)
{
    UINT32 i;
    BOOL fgRet;

    if(!_DMX_SelectInputType(DMX_IN_DIAG))
    {
        return FALSE;
    }

    // Wait for a while
    x_thread_delay(100);

    // Check result
    fgRet = TRUE;
    for (i = 0; i < NUM_UP_TEST_RESULT_WORDS; i++)
    {
        if (PID_W(i) != _au4TestResult[i])
        {
            LOG(5, "uP latch test failed at word %u: expect = 0x%08x,"
                " read = 0x%08x\n", i, _au4TestResult[i], PID_W(i));
            fgRet = FALSE;
            break;
        }
    }

    // Restore normal uCode
    if(!_DMX_Init())
    {
        return FALSE;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DmxOneuCodeInit
 */
//-----------------------------------------------------------------------------
static BOOL _DmxOneuCodeInit(void)
{
#ifdef ENABLE_MULTIMEDIA
    DMX_DDI_T rDDI;
#endif // ENABLE_MULTIMEDIA

    if(!_DmxInitBroadcastTS())
    {
        return FALSE;
    }
    _DMX_SetInputMode(DMX_HALF_PUSH);

#ifdef ENABLE_MULTIMEDIA
    if(!_DMX_DDI_Init())
    {
        return FALSE;
    }

    rDDI.eMode = DMX_DDI_MODE_SINGLE;
    rDDI.u4RateN = 0;
    rDDI.u4RateM = 0;
    if(!DMX_DDI_Set((UINT32)(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE), &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return FALSE;
    }

    if(!_DMX_PS_CPU_Init())
    {
        return FALSE;
    }

    if(!_DMX_MM_Init())
    {
        return FALSE;
    }
/*
    // Set DBM to bypass mode and keep all TS packets
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl |= (1 << 30);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);
*/
#endif  // ENABLE_MULTIMEDIA

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Init
 *  Initialize demux - can run multiple times
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Init(void)
{
    UINT8 i;
    _DMX_Lock();
    _eDmxInputType = DMX_IN_NONE;   // Legacy
    for(i=0; i<DMX_FRAMER_COUNT; i++)
    {
        _au2DmxFramerPktSize[i] = 188;
    }
    _DMX_Unlock();

    // Select clock and pin mux
    _DmxCkgenInit(TRUE);

    _DMX_SetErrLevel(DMX_ERR_RESET);

    if(!_DMX_SelectInputType(DMX_IN_BROADCAST_TS))
    {
        return FALSE;
    }

    if(!_DmxOneuCodeInit())
    {
        return FALSE;
    }

    if(!_DMX_VCodeInit())
    {
        return FALSE;
    }

    // Initialize handler if integrated with VLD
    if (!_DMX_InitHandler())
    {
        return FALSE;
    }

    if(!_DMX_InitPSIHandler())
    {
        return FALSE;
    }

    _DMX_DMEM_CA_Init();

#ifdef ENABLE_MULTIMEDIA
#ifdef CC_DMX_AUDIO_PREBUF
    if(!_DMX_InitAudHandler())
    {
        return FALSE;
    }
#endif  // CC_DMX_AUDIO_PREBUF

    if(!GCPU_Init(0))
    {
        return FALSE;
    }

    if(!_DMX_PVRPlay_Init())
    {
        return FALSE;
    }

    if(!_DMX_GetIsPanicReset())
    {
    	if(!_DMX_MUL_Init())
    	{
        	return FALSE;
    	}
    }
#endif  // ENABLE_MULTIMEDIA

#ifdef CC_DMX_TS_LOOPBACK
    if(!_DMX_Loopback_Init())
    {
        return FALSE;
    }
#endif  // CC_DMX_TS_LOOPBACK

    if(!_DmxInitHWPath())
    {
        return FALSE;
    }

    if(!_DMX_InitRawTsHandler())
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SelectInputType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SelectInputType(DMX_INPUT_TYPE_T eInputType)
{
    LOG(3, "%s, 0x%x\n", __FUNCTION__, (INT32)eInputType);

    if(eInputType == DMX_IN_DIAG)
    {
        if(!_DmxInitDiag())
        {
            return FALSE;
        }
    }

    _DMX_Lock();
    _eDmxInputType = eInputType;
    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetInputType
 */
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T _DMX_GetInputType(VOID)
{
    DMX_INPUT_TYPE_T eInputType;

    _DMX_Lock();
    eInputType = _eDmxInputType;
    _DMX_Unlock();

    return eInputType;
}


//-----------------------------------------------------------------------------
/** _DMX_SetInputMode
 */
//-----------------------------------------------------------------------------
void _DMX_SetInputMode(DMX_INPUT_MODE_T rMode)
{
    UINT32 u4Reg;

    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    _DMX_Unlock();

    u4Reg &= 0x3fffffff;

    switch(rMode)
    {
    case DMX_PUSH:
        break;

    case DMX_HALF_PUSH:
        u4Reg |= (1U << 30);
        break;

    case DMX_PULL:
        u4Reg |= (3U << 30);
        break;

    default:
        ASSERT(0);
        break;
    }

    _DMX_Lock();
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    // Enable "DBM input full" signal to TS input device
    //bit31~24 -FMR3-EHT2   bit23~16 -FMR3-EHT1
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL);
	u4Reg &= ~(0x7);
	u4Reg |= 0x1;
    DMXCMD_WRITE32(DMX_REG_DBM_BUF_CTRL, u4Reg);
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_SetIgnorePESLen
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetIgnorePESLen(BOOL fgEnable)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFF00);
    if(fgEnable)
    {
        CT_SETTING = u4Ctrl | 0x100;
    }
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_EnablePower
 */
//-----------------------------------------------------------------------------
VOID _DMX_EnablePower(BOOL fgEnable)
{
    _DmxCkgenInit(fgEnable);
}


//-----------------------------------------------------------------------------
/** _DMX_SetScrambleScheme
 *  fgOrg - false means use PES header to decide scramble state
 *          true means use scramble flag to decide scramble state
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetScrambleScheme(BOOL fgOrg)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFF0000);
    if(fgOrg)
    {
        u4Ctrl |= 0x10000;
    }
    CT_SETTING = u4Ctrl;
    _DMX_Unlock();

#ifdef TIME_SHIFT_SUPPORT
    FVR_SetScrambleScheme(fgOrg);
#endif // TIME_SHIFT_SUPPORT
}


//-----------------------------------------------------------------------------
/** _DMX_GetScrambleScheme
 *  return TRUE: new, FALSE: org
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetScrambleScheme(void)
{
    UINT32 u4Ctrl;

    u4Ctrl = (CT_SETTING) & 0x10000;
    if(u4Ctrl != 0) // original
    {
        return FALSE;
    }

    return TRUE;    // new
}



//-----------------------------------------------------------------------------
/** _DMX_Version
 *  Show DMX version information
 */
//-----------------------------------------------------------------------------
VOID _DMX_Version(void)
{
    UINT32 u4Ctrl;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    LOG(1, "----------------------------------------\n");
    LOG(1, "DMX information\n");
    LOG(1, "----------------------------------------\n");

    u2CodeVersion = (UINT16)(_u4uCodeVersion & 0xffff);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0xf);
    u1Minor = (UINT8)(u2CodeVersion & 0xff);
    LOG(1, "Demux uCode version: %u.%u\n", u1Major, u1Minor);

    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    LOG(1, "Framer0: %s, %s, %s, %s\n",
            (u4Ctrl & 0x1) ? "Enable" : "Disable",
            (u4Ctrl & 0x2) ? "Parallel" : "Serial",
            (u4Ctrl & 0x4) ? "External sync" : "Internal sync",
            (u4Ctrl & 0x8) ? "Positive" : "Negative");

    LOG(1, "Framer1: %s, %s, %s, %s\n",
            (u4Ctrl & 0x10) ? "Enable" : "Disable",
            (u4Ctrl & 0x20) ? "Parallel" : "Serial",
            (u4Ctrl & 0x40) ? "External sync" : "Internal sync",
            (u4Ctrl & 0x80) ? "Positive" : "Negative");

    LOG(1, "----------------------------------------\n");
}


//-----------------------------------------------------------------------------
/** _DMX_uCodeHasAW
 *  Return TRUE if the TS uCode supports AW.  Otherwise, return FALSE.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_uCodeHasAW(void)
{
#ifdef CC_DMX_USE_CT
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    u2CodeVersion = (UINT16)(FTI_IMEM[0] & 0xFFFF);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0x0F);
    u1Minor = (UINT8)(u2CodeVersion & 0xFF);

    // On 5387/5388/5363, CT can be configured for TS uCode 3.24 and above.
    // Although 3.21 does support CT configuration, it does not support the
    // case where (CT_SETTING == 2).  Thus, 3.24 is a better option.
    // The 5388 uCode has been renamed to 4.25 since 3.25.
    if (((u1Major == 3) && (u1Minor == 24)) ||
        ((u1Major == 4) && (u1Minor >= 25)) ||
        ((u1Major == 5) && (u1Minor >= 40)))
    {
        return TRUE;
    }
#endif  // CC_DMX_USE_CT

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DMX_IsMicroProcessorStopped
 *  Return TRUE if the micro-processor is stopped according to the global
 *  variable _fgMicroProcessorStopped.  Otherwise, return FALSE.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_IsMicroProcessorStopped(void)
{
    BOOL fgMicroProcessorStopped;

    _DMX_Lock();
    fgMicroProcessorStopped = _fgMicroProcessorStopped;
    _DMX_Unlock();

    return fgMicroProcessorStopped;
}


//-----------------------------------------------------------------------------
/** _DMX_SetExternalFramer
 */
//-----------------------------------------------------------------------------
void _DMX_SetExternalFramer(DMX_EXT_FRAMER_T eFramer)
{
    UINT32 u4Reg;

    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg &= ~0x03000000;

    switch(eFramer)
    {
    case DMX_EXT_FRAMER_2_PVR_3_EXT:
        u4Reg |= 0x02000000;
        break;

    case DMX_EXT_FRAMER_2_DDI_3_EXT:
        u4Reg |= 0x03000000;
        break;

    default:
        break;
    }

    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

    _eDmxExtFramer = eFramer;
}


//-----------------------------------------------------------------------------
/** _DMX_GetExternalFramer
 */
//-----------------------------------------------------------------------------
DMX_EXT_FRAMER_T _DMX_GetExternalFramer(void)
{
    UINT32 u4Val;

    u4Val = (DMXCMD_READ32(DMX_REG_DBM_CONTROL) >> 24) & 0xF;

    switch(u4Val)
    {
    case 2:
        return DMX_EXT_FRAMER_2_PVR_3_EXT;
    case 3:
        return DMX_EXT_FRAMER_2_DDI_3_EXT;
    default:
        return DMX_EXT_FRAMER_NORMAL;
    }
}


//-----------------------------------------------------------------------------
/** _DMX_SetFramerPktSize
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFramerPktSize(UINT8 u1TsIndex, UINT16 u2PktSize)
{
    UINT32 u4Reg;

    if(u1TsIndex >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    if(_au2DmxFramerPktSize[u1TsIndex] == u2PktSize)
    {
        return TRUE;
    }

    if(u1TsIndex == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL) & 0xFFFF0000;
        u4Reg |= 0x0100;
        u4Reg |= (UINT32)(u2PktSize & 0xFF);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
    }
    else if(u1TsIndex == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL) & 0xFFFF;
        u4Reg |= 0x01000000;
        u4Reg |= ((UINT32)(u2PktSize & 0xFF) << 16);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
    }
    else if(u1TsIndex == 2)
    {
        // Framer2 size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL2) & ~0xFF000000;
        u4Reg |= ((UINT32)u2PktSize << 24);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL2, u4Reg);

        u4Reg = 0x5;        // Prebyte and Enable
        DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);
    }
    else if(u1TsIndex == 3)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2) & 0xFFFF0000;
        u4Reg |= 0x0100;
        u4Reg |= (UINT32)(u2PktSize & 0xFF);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);
    }
    else
    {
        ASSERT(0);
    }

    _DMX_ResetFramer(u1TsIndex);

    _au2DmxFramerPktSize[u1TsIndex] = u2PktSize;

    return TRUE;
}


#ifdef CC_FAST_INIT
//-----------------------------------------------------------------------------
/** DMX_pm_resuem
 */
//-----------------------------------------------------------------------------
void DMX_pm_resume(void)
{
    // 1. Do init
    if(!_DMX_Init())
    {
        LOG(1, "Resume fail\n");
    }

    // 2. Enable interrupt controller
    if(!BIM_EnableIrq(VECTOR_DEMUX))
    {
        LOG(1, "Eanble DMX isr fail\n");
    }
    if(!BIM_EnableIrq(VECTOR_DDI))
    {
        LOG(1, "Eanble DDI isr fail\n");
    }
    if(!BIM_EnableIrq(VECTOR_UP0))
    {
        LOG(1, "Eanble GCPU isr fail\n");
    }
    if(!BIM_EnableIrq(VECTOR_MMU_GCPU))
    {
        LOG(1, "Eanble GCPU IOMMU isr fail\n");
    }
}
#endif  // CC_FAST_INIT


