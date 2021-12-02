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
 * $RCSfile: srm_sram.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file srm_sram.c
 *  Brief of file srm_sram.c
 *  Details of file srm_sram.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "srm_debug.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_ckgen.h"
#include "drv_scpos.h"
#include "mute_if.h"
#include "x_os.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SRAM_MASK_HD        (1 << 0)
#define SRAM_MASK_MAIN_UP   (1 << 1)
#define SRAM_MASK_SUB_UP    (1 << 2)
#define SRAM_MASK_VGA       (1 << 3)
#define SRAM_MASK_8TAP      (1 << 4)
#define SRAM_MASK_6TAP      (1 << 5)
#define SRAM_MODE_NS        12
#define SRAM_REG_CLK_1      0x2c8
#define SRAM_REG_CLK_2      0x2cc
#define SRAM_REG_POOL       0x110
#define SRM_SET_POOL_TIME (3)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _arClkMap[SRAM_MODE_NS][3] =
{
    {0xb, 0x228A0A20, 0x00824924},
    {0xf, 0x228A0A24, 0x00824924},
    {0x6, 0x228A2A28, 0x00A24920},
    {0x2, 0x228A2A28, 0x00A24820},
    {0x4, 0x228A2A28, 0x00A20920},
    {0x0, 0x228A2A28 , 0x00A20820},
    {0x7, 0x228A2A24, 0x00824924},
    {0x3, 0x228A2A20, 0x00824924},
    {0x5, 0x228A2A24, 0x00820820},
    {0x1, 0x228A2A20, 0x00820820},
    {0x16, 0x248A2A28, 0x00A24920},
    {0x12, 0x248A2A28, 0x00A24820}
};

static UINT32 _u4MainSrc = 0;
static UINT32 _u4POPMode = 0;
static UINT32 _u4MainScaleUp = 0;
static UINT32 _u4SubScaleUp = 0;
static UINT32 _u4PoolReg = 0;
static UINT32 _u4ClkReg1 = 0;
static UINT32 _u4ClkReg2 = 0;
static UINT32 _u4SramMode = 0;
#ifndef CC_MTK_LOADER // defined (CC_MT5360) && !defined(CC_5360_LOADER)
extern UINT32 _u4SrmPoolSetMute;
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static void _SrmSramWriteReg(UINT32 u4ClkReg1, UINT32 u4ClkReg2, UINT32 u4PoolReg)
{
    _u4ClkReg1 = u4ClkReg1;
    _u4ClkReg2 = u4ClkReg2;
    _u4PoolReg = u4PoolReg;
    CKGEN_WRITE32(SRAM_REG_CLK_1, _u4ClkReg1);
    CKGEN_WRITE32(SRAM_REG_CLK_2, _u4ClkReg2);
    IO_WRITE32(SCPOS_BASE, SRAM_REG_POOL, _u4PoolReg);

    LOG(5, "[SRM] sram mode=%d, clk1=0x%08x, clk2=0x%08x, pool=0x%08x\n",
        _u4SramMode, u4ClkReg1, u4ClkReg2, u4PoolReg);
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

UINT32 SRM_SramQueryMode(UINT32 u4MainSrc, UINT32 u4POPMode,
                         UINT32 u4MainScaleUp, UINT32 u4SubScaleUp)
{
    UINT32 u4PoolReg = 0;
    UINT32 u4Mode;

    if ((u4POPMode == 0) && (u4MainSrc > SRM_SRAM_SRC_SD))
    {
        u4PoolReg |= SRAM_MASK_HD;
        if (u4MainSrc > SRM_SRAM_SRC_HD)
        {
            u4PoolReg |= SRAM_MASK_VGA;
        }
    }

    if ((u4MainScaleUp == 1) && (u4POPMode == 0) && (u4MainSrc == SRM_SRAM_SRC_SD))
    {
        u4PoolReg |= SRAM_MASK_8TAP;
    }

    if ((u4MainScaleUp == 1) || ((u4POPMode == 0) && (u4MainSrc > SRM_SRAM_SRC_HD)))
    {
        u4PoolReg |= SRAM_MASK_MAIN_UP;
    }

    if (u4SubScaleUp == 1)
    {
        u4PoolReg |= SRAM_MASK_SUB_UP;
    }

    for (u4Mode = 0; u4Mode < SRAM_MODE_NS; u4Mode++)
    {
        if (_arClkMap[u4Mode][0] == u4PoolReg)
        {
            return u4Mode;
        }
    }

    return SRAM_MODE_NS;
}

UINT32 SRM_SramSetMode(UINT32 u4Mode)
{
    if (u4Mode >= SRAM_MODE_NS)
    {
        Printf("SRAM mode must < %d\n", SRAM_MODE_NS);
        return 1;
    }

    _u4SramMode = u4Mode;
    _u4ClkReg1 = _arClkMap[u4Mode][1];
    _u4ClkReg2 = _arClkMap[u4Mode][2];
    _SrmSramWriteReg(_u4ClkReg1, _u4ClkReg2, _arClkMap[u4Mode][0]);

    return 0;
}

UINT32 SRM_SramSetPool(UINT32 u4MainSrc, UINT32 u4POPMode,
                       UINT32 u4MainScaleUp, UINT32 u4SubScaleUp)
{
    UINT32 u4Mode;
    
#ifndef CC_MTK_LOADER // defined (CC_MT5360) && !defined(CC_5360_LOADER)
    UINT32 u4PreMode;
#endif
    u4Mode = SRM_SramQueryMode(u4MainSrc, u4POPMode, u4MainScaleUp, u4SubScaleUp);

#ifndef CC_MTK_LOADER
    u4PreMode = SRM_SramQueryMode(_u4MainSrc, _u4POPMode, _u4MainScaleUp, _u4SubScaleUp);
    if (u4PreMode != u4Mode) // black mute for transient garbage when sram configuration updated.
    {
        _vDrvVideoSetMute(MUTE_MODULE_SRM, 0, SRM_SET_POOL_TIME, FALSE);
    }
#endif

    if (u4Mode < SRAM_MODE_NS)
    {
        SRM_SramSetMode(u4Mode);
        _u4MainScaleUp = u4MainScaleUp;
        _u4SubScaleUp = u4SubScaleUp;
        _u4POPMode = u4POPMode;
        _u4MainSrc = u4MainSrc;
        return 0;
    }
    else
    {
        return 1;
    }
}

VOID SRM_SramQuery(VOID)
{
    Printf("SramMode=%d, MainSrc=%d, POPMode=%d, MainScaleUp=%d, SubScaleUp=%d\n",
           _u4SramMode, _u4MainSrc, _u4POPMode, _u4MainScaleUp, _u4SubScaleUp);
    Printf("[FW]ClkReg1=0x%08x, ClkReg2=0x%08x, PoolReg=0x%08x\n",
           _u4ClkReg1, _u4ClkReg2, _u4PoolReg);
    Printf("[HW]ClkReg1=0x%08x, ClkReg2=0x%08x, PoolReg=0x%08x\n",
           CKGEN_READ32(SRAM_REG_CLK_1), CKGEN_READ32(SRAM_REG_CLK_2),
           IO_READ32(SCPOS_BASE, SRAM_REG_POOL));
}
