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
 * $RCSfile: dif_dmx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dif_dmx.c
 *  Demux high-level api.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dif_debug.h"
#include "drv_stc.h"
#include "dmx_if.h"
#include "x_linux.h"
#include <linux/mt53xx_dif.h>

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

static UINT8 _au4PsiBuf[8192];


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DmxOnTest
 *  Handler of test command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DmxOnTest(unsigned long arg)
{
    DMX_IOCTL_TEST_T rTest;

    if (copy_from_user(&rTest, (void __user *)arg, sizeof(rTest)))
    {
        LOG(3, "DMX_CTRL: copy_from_user() failed\n");
        return FALSE;
    }

    LOG(5, "DMX_IOCTL: TEST\n");
    LOG(5, "Receive data: %u\n", rTest.u4Data);

    rTest.u4Data = 5678;
    if (copy_to_user((void __user *)arg, &rTest, sizeof (rTest)))
    {
        LOG(3, "DMX_IOCTL: copy_to_user() failed\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxOnPsiGetBuffer
 *  Handler of PSI get buffer command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DmxOnPsiGetBuffer(unsigned long arg)
{
    DMX_IOCTL_PSI_GET_T rPsiGet;
    UINT32 u4CopySize;
    UINT8 u1Pidx;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
        sizeof (DMX_IOCTL_PSI_GET_T)))
    {
        LOG(3, "DMX_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get PSI info
    if (copy_from_user(&rPsiGet, (void __user *)arg, sizeof (rPsiGet)))
    {
        LOG(3, "DMX_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }

    u4CopySize = rPsiGet.u4CopySize;
    u1Pidx = rPsiGet.u1Pidx;

    if (u4CopySize > DMX_MAX_PSI_COPY_SIZE)
    {
        return FALSE;
    }

    if (!access_ok(VERIFY_WRITE, (void __user *)rPsiGet.pu1Buf, u4CopySize))
    {
        return FALSE;

    }

    if (!DMX_PSI_GetBuffer(u1Pidx, rPsiGet.u1SerialNumber, rPsiGet.u4FrameAddr,
        rPsiGet.u4SkipSize, u4CopySize, _au4PsiBuf))
    {
        return FALSE;
    }

    if (copy_to_user((void __user *)rPsiGet.pu1Buf, _au4PsiBuf,
        u4CopySize))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxOnPsiUnlockBuffer
 *  Handler of PSI unlock buffer command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _DmxOnPsiUnlockBuffer(unsigned long arg)
{
    DMX_IOCTL_PSI_UNLOCK_T rPsiUnlock;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
        sizeof (DMX_IOCTL_PSI_UNLOCK_T)))
    {
        LOG(3, "DMX_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get PSI info
    if (copy_from_user(&rPsiUnlock, (void __user *)arg, sizeof (rPsiUnlock)))
    {
        LOG(3, "DMX_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }

    if (!DMX_PSI_UnlockBuffer(rPsiUnlock.u1Pidx, rPsiUnlock.u1SerialNumber,
        rPsiUnlock.u4Size, rPsiUnlock.u4FrameAddr))
    {
        LOG(3, "DMX_IOCTL: PSI_UnlockBuffer() failed\n");
        return FALSE;
    }

    return TRUE;
}

