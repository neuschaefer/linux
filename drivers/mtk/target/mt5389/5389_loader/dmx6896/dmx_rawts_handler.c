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
 * $RCSfile: dmx_mm_api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm_api.c
 *  Demux program stream driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "dmx_drm_drvif.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "mpv_drvif.h"
#include "aud_drvif.h"
LINT_EXT_HEADER_END

#include "x_assert.h"
#include "x_os.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_timer.h"

#include "drv_common.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DMX_RAW_TS_THREAD_NAME              "DMXRAWTS"
#define DMX_RAW_TS_THREAD_STACK_SIZE        DMX_PSI_THREAD_STACK_SIZE
#define DMX_RAW_TS_THREAD_PRIORITY          DMX_PSI_THREAD_PRIORITY

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hDmxRawTsThread = NULL_HANDLE;

static BOOL _afgRawTsEnable[DMX_NUM_PID_INDEX];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** _DmxPSIThread
 *  The PSI thread
 *
 *  @param  prArg           The thread argument
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _DmxRawTsThread(VOID* pvArg)
{
    UINT8 u1Pidx;
    PID_STRUCT_T* prPidStruct;
    UINT32 u4Wp, u4BufStart, u4BufEnd, u4PesRp, u4DataSize;
    BOOL fgProcessed;

    UNUSED(pvArg);

    while (1)
    {
        for(u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
        {
            if(!_afgRawTsEnable[u1Pidx])
            {
                continue;
            }

            prPidStruct = _DMX_GetPidStruct(u1Pidx);

            if(!prPidStruct->fgEnable)
            {
                continue;
            }

            fgProcessed = FALSE;

            while (1)
            {
                UINT32 u4Wp2;

                u4Wp = PID_S_W(u1Pidx, 8);
                u4Wp2 = PID_S_W(u1Pidx, 8);
                if (u4Wp == u4Wp2)
                {
                    break;
                }
            }

            prPidStruct->u4Wp = u4Wp;
            u4Wp = VIRTUAL(prPidStruct->u4Wp);

            if (prPidStruct->pfnNotify != NULL)
            {
                DMX_NOTIFY_INFO_RAW_TS_T rRawTs;

                u4BufStart = VIRTUAL(prPidStruct->u4BufStart);
                u4BufEnd = VIRTUAL(prPidStruct->u4BufEnd + 1);
                u4PesRp = VIRTUAL(prPidStruct->u4PesRp);

                DMX_PANIC((u4Wp >= u4BufStart) && (u4Wp < u4BufEnd));
                DMX_PANIC_RET_VOID();
                DMX_PANIC((u4PesRp >= u4BufStart) && (u4PesRp < u4BufEnd));
                DMX_PANIC_RET_VOID();
                u4DataSize = DATASIZE(u4PesRp, u4Wp, u4BufEnd - u4BufStart);

                HalFlushInvalidateDCacheMultipleLine(u4BufStart, u4BufEnd - u4BufStart);

                rRawTs.u1SerialNumber = prPidStruct->u1SerialNumber;
                rRawTs.u4DataAddr = u4PesRp;
                rRawTs.u4DataSize = u4DataSize;

                fgProcessed = prPidStruct->pfnNotify(u1Pidx, DMX_NOTIFY_CODE_RAW_TS,
                    (UINT32)&rRawTs, prPidStruct->pvNotifyTag);
            }

            if (!fgProcessed)
            {
                // Drop data by set RP to WP directly
                PID_S_W(u1Pidx, 9) = PHYSICAL(prPidStruct->u4Wp);
                prPidStruct->u4Rp = prPidStruct->u4Wp;
            }

            prPidStruct->u4PesRp = prPidStruct->u4Wp;
        }
        x_thread_delay(500);
    }   // while (1)
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _DMX_RawTsEnable(UINT8 u1Pidx, BOOL fgEnable)
{
    _afgRawTsEnable[u1Pidx] = fgEnable;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_InitPSIHandler
 *  Initialize PSI interfaces
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_InitRawTsHandler(void)
{
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        // Create PSI thread
        if (x_thread_create(&_hDmxRawTsThread, DMX_RAW_TS_THREAD_NAME,
            DMX_RAW_TS_THREAD_STACK_SIZE, DMX_RAW_TS_THREAD_PRIORITY, _DmxRawTsThread, 0,
            NULL) != OSR_OK)
        {
            LOG(1, "Fail to create DMX raw ts thread!\n");
            return FALSE;
        }

        _fgInit = TRUE;
    }

    return TRUE;
}

