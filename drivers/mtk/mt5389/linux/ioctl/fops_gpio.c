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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: fops_gpio.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/


/** @file gpio_mod.c
 *  Kernel mode GPIO module driver to handle ioctl from user sapce.
 */

 
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
/*lint -save -e* */
#include "ionodes.h"
#include "x_linux.h"
#include "x_assert.h"
#include <linux/version.h>
#include <linux/mt53xx_gpio.h>
#include <linux/semaphore.h>
/*lint -restore */

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

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37)
#define DECLARE_MUTEX(mutex)  DEFINE_SEMAPHORE(mutex)
#endif

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
/*lint -save -e* */
static DECLARE_MUTEX(_rMutex);
/*lint -restore */


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** gpio_ioctl() GPIO driver ioctl function.
 */
//-----------------------------------------------------------------------------
/*lint -save -e* */
long gpio_ioctl(struct file *file, unsigned int cmd,
                unsigned long arg)
/*lint -restore */
{
    GPIO_IOCTL_2ARG_T rArg;
    INT32 i4Ret;

    if (down_interruptible(&_rMutex))
    {
        return (-ERESTARTSYS);
    }
    
    switch (cmd) {
    case GPIO_IOCTL_INIT:
        i4Ret = GPIO_Init();
        break;
    case GPIO_IOCTL_STOP:
        i4Ret = GPIO_Stop();
        break;
    case GPIO_IOCTL_SETOUT:
        if (!access_ok(VERIFY_READ, (void __user *)arg,
                       sizeof(GPIO_IOCTL_2ARG_T)))
        {
            i4Ret = -EFAULT;
            goto _GPIO_IOCTL_EXIT;
        }
        if (copy_from_user(&rArg, (void __user *)arg,
                           sizeof(GPIO_IOCTL_2ARG_T)))
        {
            i4Ret = -EFAULT;
            goto _GPIO_IOCTL_EXIT;
        }
        i4Ret = GPIO_SetOut(rArg.au4Arg[0], rArg.au4Arg[1]);
        break;
    case GPIO_IOCTL_GETIN:
        if (!access_ok(VERIFY_READ, (void __user *)arg,
                       sizeof(INT32)))
        {
            i4Ret = -EFAULT;
            goto _GPIO_IOCTL_EXIT;
        }
        if (copy_from_user(&rArg.au4Arg[0], (void __user *)arg,
                           sizeof(INT32)))
        {
            i4Ret = -EFAULT;
            goto _GPIO_IOCTL_EXIT;
        }
        i4Ret = GPIO_GetIn(rArg.au4Arg[0]);
        break;
    default:
        i4Ret = -EINVAL;
        ASSERT(0);
    }

_GPIO_IOCTL_EXIT:

    up(&_rMutex);
    
    return i4Ret;
}

struct file_operations gpio_fops = {
    .unlocked_ioctl      = gpio_ioctl,
};
