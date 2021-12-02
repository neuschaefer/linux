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
 * $RCSfile: srm_dram.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file srm_dram.c
 *  Brief of file srm_dram.c.
 *  Details of file srm_dram.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "srm_drvif.h"
#include "srm_debug.h"
#include "drv_common.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "x_assert.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SRM_DRAM_MONITOR_IDLE 500 // Take a rest between each monitor


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/**
 ** Brief of HAL_DRAMC_T
 */
typedef struct
{
    UINT32 u4Bmgp3ag                        : 3;
    UINT32 u4ReqCntEn                       : 1;
    UINT32 u4Bmgp2ag                        : 3;
    UINT32 u4PaCntEn                        : 1;
    UINT32 u4Bmgp1ag                        : 4;
    UINT32 fgBmbp1                          : 1;
    UINT32 fgBmbp2                          : 1;
    UINT32 fgBmbp3                          : 1;
    UINT32 fgDMBMFreeze                     : 1;    // Bus monitor freeze
    UINT32 u4Reserved                       : 16;

LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed))  HAL_DRAMC_T;


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

static UINT32 _u4DramMeasureModule = SRM_DRV_TOTAL;
static HANDLE_T _hMonitor = NULL;
static BOOL _fgSrmDramInit = FALSE;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of _SrmDramMonitor.
 */
//-----------------------------------------------------------------------------

static VOID _SrmDramMonitor(VOID* pvArg)
{
    UNUSED(pvArg);

    while (1)
    {
#if 0
        UINT32 u4Result;
        UINT32 u4DelayClock;
        UINT64 u8Result;

        u4DelayClock = (BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) * 2;

        u4Result = SRM_DramMonitor(_u4DramMeasureModule, u4DelayClock, 2000);

        u8Result = (UINT64)u4Result;
        u8Result *= 1000;
        u8Result /= u4DelayClock;

        u4Result = (UINT32) u8Result;

        SRM_SendEvent(SRM_DRV_DRAM, SRM_DRV_TOTAL, u4Result, 0);
#else
        UNUSED(_u4DramMeasureModule);
#endif

        // Take a rest between each monitor
        x_thread_delay(SRM_DRAM_MONITOR_IDLE);
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of SRM_Init.
 */
//-----------------------------------------------------------------------------

VOID SRM_DramInit(VOID)
{
    if (_fgSrmDramInit == FALSE)
    {
        HANDLE_T hThread;
        // create semaphore
        VERIFY(x_sema_create(&_hMonitor, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);

        // create thread
        if (x_thread_create(&hThread, "DRAM_M", 1024, (LOG_PRINT_PRIORITY - 1),
            _SrmDramMonitor, 0, NULL) != OSR_OK)
        {
            ASSERT(0);
        }

        _fgSrmDramInit = TRUE;
    }
}


//-----------------------------------------------------------------------------
/** Brief of SRM_DramMonitor.
 */
//-----------------------------------------------------------------------------

UINT32 SRM_DramMonitor(UINT32 u4Mod, UINT32 u4DelayClock, UINT32 u4MonitorDelay)
{
    UINT32 u4AgendId;
    UINT32 u4GroupId;
    UINT32 u4Result;
    UINT32 u4Total;

    volatile HAL_DRAMC_T* _prDramcReg = (HAL_DRAMC_T*) (DRAM_BASE + 0x080);
    HAL_DRAMC_T rDramcReg;

    if (_fgSrmDramInit == FALSE)
    {
        LOG(1, "SRM not init\n");
        return 0;
    }

    VERIFY(x_sema_lock(_hMonitor, X_SEMA_OPTION_WAIT) == OSR_OK);

    u4Total = 0;

    switch (u4Mod)
    {
        case SRM_DRV_TOTAL:
        u4Total = 1;
        u4GroupId = 1;
        u4AgendId = 1;
        break;

        // group1 --------------------------------------------------------------

        case SRM_DRV_AUD:
        u4AgendId = 0;
        u4GroupId = 1;
        break;

        case SRM_DRV_DMX:   // demux, pvr
        u4AgendId = 1;
        u4GroupId = 1;
        break;

        case SRM_DRV_VDOIN:    // 3d-comb, vbi, teltext
        u4AgendId = 2;
        u4GroupId = 1;
        break;

        case SRM_DRV_OSD:
        u4AgendId = 3;
        u4GroupId = 1;
        break;

        case SRM_DRV_MDDI:  // nr, pscan, pscan_pip
        u4AgendId = 4;
        u4GroupId = 1;
        break;

        case SRM_DRV_B2R:
        u4AgendId = 5;
        u4GroupId = 1;
        break;

        case SRM_DRV_CPU:
        u4AgendId = 6;
        u4GroupId = 1;
        break;

        case SRM_DRV_SCPOS:
        u4AgendId = 7;
        u4GroupId = 1;
        break;

        case SRM_DRV_MPEG:  // vdec_mc
        u4AgendId = 8;
        u4GroupId = 1;
        break;

        case SRM_DRV_JPEG:  // cabac,avc_mv,vld1,vld2,mpv_pred,jpeg
        u4AgendId = 9;
        u4GroupId = 1;
        break;

        case SRM_DRV_GFX_HI:
        u4AgendId = 11;
        u4GroupId = 1;
        break;
        
        // group2 --------------------------------------------------------------

        case SRM_DRV_USB:
        u4AgendId = 0;
        u4GroupId = 2;
        break;

        case SRM_DRV_GDMA:  // sdio, img-resizer
        u4AgendId = 1;
        u4GroupId = 2;
        break;

        case SRM_DRV_DDI:   // Demux_pull, ddi
        u4AgendId = 3;
        u4GroupId = 2;
        break;
        
        case SRM_DRV_DSP:
        u4AgendId = 6;
        u4GroupId = 2;
        break;

        case SRM_DRV_FCI:
        u4AgendId = 7;
        u4GroupId = 2;
        break;
        
        // group3 --------------------------------------------------------------

        case SRM_DRV_GFX_LO:
        u4AgendId = 0;
        u4GroupId = 3;
        break;
        
        case SRM_DRV_TCM:
        u4AgendId = 2;
        u4GroupId = 3;
        break;
        
        case SRM_DRV_IDETEST:
        u4AgendId = 6;
        u4GroupId = 3;
        break;
        
        case SRM_DRV_UART:
        u4AgendId = 7;
        u4GroupId = 3;
        break;

        default:
        u4Total = 1;
        u4GroupId = 1;
        u4AgendId = 1;
        break;
    }


    IO_REG32(DRAM_BASE, 0x084) = u4DelayClock;

    rDramcReg = *_prDramcReg;
    rDramcReg.fgBmbp1 = 0;
    rDramcReg.fgBmbp2 = 0;
    rDramcReg.fgBmbp3 = 0;
    if (u4Total != 0)
    {
        // All agent, including group 1, 2, and 3
        rDramcReg.u4Bmgp1ag = 0xF;
    }
    else
    if (u4GroupId == 1)
    {
        rDramcReg.u4Bmgp1ag = u4AgendId;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.u4Bmgp2ag = u4AgendId;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.u4Bmgp3ag = u4AgendId;
    }
    *_prDramcReg = rDramcReg;
    if (u4GroupId == 1)
    {
        rDramcReg.fgBmbp1 = 1;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.fgBmbp2 = 1;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.fgBmbp3 = 1;
    }

    *_prDramcReg = rDramcReg;

    x_thread_delay(u4MonitorDelay);

    u4Result = 0;
    if (u4GroupId == 1)
    {
        u4Result = IO_REG32(DRAM_BASE, 0x3C8);
    }
    else
    if (u4GroupId == 2)
    {
        u4Result = IO_REG32(DRAM_BASE, 0x3CC);
    }
    else
    if (u4GroupId == 3)
    {
        u4Result = IO_REG32(DRAM_BASE, 0x3D0);
    }

    VERIFY(x_sema_unlock(_hMonitor) == OSR_OK);

    return u4Result;
}

