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
 * $RCSfile: board.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file board.c
 *  Board-related rountines.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "board.h"
#include "hal.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_hal_926.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
#include "c_model.h"
#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/semaphore.h>


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define FPGA_SYSTEM_CLOCK			20000000	// 20M Hz system clock

#define REG_PSR_CTRL                0x200001f0
#define PSR_CTRL_PCKE               (1 << 1)

#define REG_IC_VERSION              0x20000000

#define SIGNAL_MSG_NUM              15

#ifndef CC_DRIVER_PROGRAM
#define CC_DRIVER_PROGRAM           0
#endif /* CC_DRIVER_PROGRAM */

#ifndef NOR_FLASH_LOADER_SIZE
#define NOR_FLASH_LOADER_SIZE             (128*1024)
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/*----------------------------------------------------------------------------
 * Function: BSP_IsFPGA
 *
 * Description:
 *      Check if current platform if FPGA or not
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE: FPGA
 *      FALSE: IC
 *---------------------------------------------------------------------------*/
BOOL BSP_IsFPGA(void)
{
    UINT32 u4Val;

    /* If there is FPGA ID, it must be FPGA, too. */
    u4Val = BIM_READ32(REG_RO_FPGA_ID);
    if (u4Val != 0) { return 1; }

    /* otherwise, it's not FPGA. */
    return 0;
}

/*----------------------------------------------------------------------------
 * Function: BSP_GetIcVersion
 *
 * Description:
 *      Get IC version
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      The IC version, or IC_VER_FPGA if running in a FPGA platform, or
 *      IC_VER_UNKOWN for unknown IC version
 *---------------------------------------------------------------------------*/
IC_VERSION_T BSP_GetIcVersion(void)
{

#if !defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)
    IC_VERSION_T eVer = IC_VER_FPGA;

    if (!BSP_IsFPGA())
    {
#endif /* !CC_NAND_BOOT && !CC_ROM_BOOT */
        UINT32 u4Version;

#if 1
        u4Version = BIM_READ32(REG_RO_SW_ID);
#else /* CC_MT5392B */
        // u4Version = *(UINT32*)REG_IC_VERSION;
        u4Version = BIM_READ32(REG_RO_CHIP_ID);
        u4Version = ((u4Version & 0x0fffff00U) << 4) |
                    ((u4Version & 0x000000ffU) << 0) |
                    ((u4Version & 0xf0000000U) >> 20);
#endif /* CC_MT5392B */

        return (IC_VERSION_T)u4Version;
#if !defined(CC_NAND_BOOT) && !defined(CC_ROM_BOOT)
    }

    return eVer;
#endif /* !CC_NAND_BOOT && !CC_ROM_BOOT */
}

void BSP_HaltSystem(void)
{
    panic("BSP_HaltSystem\n");
}

//-----------------------------------------------------------------------------
/** BSP_GetSystemClock() Get system clock
 *  @return The system clock
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetSystemClock(void)
{
    if (BSP_IsFPGA())
    {
        return FPGA_SYSTEM_CLOCK;
    }
    else
    {
        return GET_XTAL_CLK();
    }
}

//-----------------------------------------------------------------------------
/** BSP_GetFbmMemAddr()
 *  @return the frame buffer start address
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetFbmMemAddr()
{
    return FBM_START_PHY_ADDR;
}

//-----------------------------------------------------------------------------
/** BSP_GetPQDataOffset()
 *  No one should use it in Linux.
 */
//-----------------------------------------------------------------------------
UINT32 BSP_GetPQDataOffset()
{
    ASSERT(0);
    return 0;
}

//-----------------------------------------------------------------------------
/* _BoardInit Board-related initializations
 */
//-----------------------------------------------------------------------------

void __init BSP_InitBoard(void)
{
    BSP_CkgenInit();
}
