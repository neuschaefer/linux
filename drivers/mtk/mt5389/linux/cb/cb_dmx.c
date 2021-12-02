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
 * $RCSfile: cb_dmx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file cb_dmx.c
 *  Demux callback function
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "cb_low.h"
#include "dmx_if.h"
#include <linux/mt53xx_cb.h>

#include "x_assert.h"

#define DEFINE_IS_LOG	DMX_IsLog
#include "x_debug.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DmxOnPesNotify
 *  PES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxOnPesNotify(UINT8 u1Pidx, UINT32 u4Data, const void* pvTag)
{
    // To be defined
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DmxOnEsNotify
 *  ES notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxOnEsNotify(UINT8 u1Pidx, UINT32 u4Data, const void* pvTag)
{
    // To be defined
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DmxOnPsiNotify
 *  PSI notification handler
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxOnPsiNotify(UINT8 u1Pidx, UINT32 u4Data, const void* pvTag)
{
    DMX_CB_T rDmxCb;
    DMX_NOTIFY_INFO_PSI_T* prNotifyInfo;

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    prNotifyInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prNotifyInfo != NULL);

    rDmxCb.u1Pidx = u1Pidx;
    rDmxCb.u4SecAddr = prNotifyInfo->u4SecAddr;
    rDmxCb.u4SecLen = prNotifyInfo->u4SecLen;
    rDmxCb.u1SerialNumber = prNotifyInfo->u1SerialNumber;

    LOG(5, "PSI NOTIFY - Pidx: %u, section: 0x%08x, size: %u, serial number: %d\n", u1Pidx,
        prNotifyInfo->u4SecAddr, prNotifyInfo->u4SecLen, prNotifyInfo->u1SerialNumber);

    UNUSED(_CB_PutEvent(CB_DMX_TRIGGER, sizeof (DMX_CB_T), &rDmxCb));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxCallbackHandler
 *  Demux callback handler
 *
 *  @param  u1Pidx          PID index
 *  @param  eCode           Notification code
 *  @param  u4Data          Handler-specific data
 *  @param  pvTag           Handler-specific tag
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxCallbackHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvTag)
{
    BOOL fgRet = FALSE;

    switch (eCode)
    {
    case DMX_NOTIFY_CODE_PES:
        fgRet = _DmxOnPesNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_ES:
        fgRet = _DmxOnEsNotify(u1Pidx, u4Data, pvTag);
        break;

    case DMX_NOTIFY_CODE_PSI:
        fgRet = _DmxOnPsiNotify(u1Pidx, u4Data, pvTag);
        break;

    default:
        // Unknown notification code
        LOG(1, "Pidx %u: Unknown notification code %u\n", u1Pidx,
            (UINT32)eCode);
        ASSERT(0);
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DMX_RegTrigger
 *  Demux call registration. Indirectly called by user program
 *
 *  @param  i4SubId         Sub-id. Not used
 *
 *  @retval CBR_OK          Succeed
 *  @retval Others          Fail
 */
//-----------------------------------------------------------------------------
INT32 _DMX_RegTrigger(INT32 i4SubId)
{
    DMX_PID_T rPid;
    UINT32 i;

    rPid.pfnNotify = _DmxCallbackHandler;
    rPid.pvNotifyTag = NULL;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (!DMX_SetPid(i, DMX_PID_FLAG_CALLBACK, &rPid))
        {
            LOG(3, "Pidx %u: Fail to set callback function\n", i);
            return -1;
        }
    }

    return CBR_OK;
}


