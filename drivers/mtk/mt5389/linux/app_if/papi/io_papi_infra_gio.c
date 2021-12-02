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
 * $RCSfile: io_papi_infra_gio.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_setup.c
 *  PAPI kernel driver implementation: setup
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_gpio.h"
#include <linux/papi_ioctl.h>	
#include "papi.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PAPI_OK         (0)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return -EACCES; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return -EACCES; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return -EACCES; \
    } \
    if (copy_to_user((void __user *)(dst), (void *)(src), (size))) \
    { \
        return -EACCES; \
    }
        

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------
static int _papi_infra_gio_PinGetLatch(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    int i4GpioNum, i4Polarity;
    int *pi4Polarity;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    i4GpioNum = rArg.au4Arg[0];
    pi4Polarity = (int *)rArg.au4Arg[1];
    i4Polarity = GPIO_Output(i4GpioNum, NULL);
    COPY_TO_USER(pi4Polarity, &i4Polarity, sizeof(int));

    return PAPI_OK;
}

static int _papi_infra_gio_PinGetLine(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    int i4GpioNum, i4Polarity;
    int *pi4Polarity;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    i4GpioNum = rArg.au4Arg[0];
    pi4Polarity = (int *)rArg.au4Arg[1];
    i4Polarity = GPIO_Input(i4GpioNum);
    COPY_TO_USER(pi4Polarity, &i4Polarity, sizeof(int));

    return PAPI_OK;
}

static int _papi_infra_gio_PinGetMode(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    int i4GpioNum, i4Mode;
    int *pi4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    i4GpioNum = rArg.au4Arg[0];
    pi4Mode = (int *)rArg.au4Arg[1];
    i4Mode = GPIO_Enable(i4GpioNum, NULL);
    COPY_TO_USER(pi4Mode, &i4Mode, sizeof(int));

    return PAPI_OK;
}

static int _papi_infra_gio_PinSetLatch(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    int i4GpioNum, i4Polarity;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    i4GpioNum = rArg.au4Arg[0];
    i4Polarity = rArg.au4Arg[1];
    GPIO_Output(i4GpioNum, (const int *)&i4Polarity);

    return PAPI_OK;
}

static int _papi_infra_gio_PinSetMode(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    int i4GpioNum, i4Mode;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    i4GpioNum = rArg.au4Arg[0];
    i4Mode = rArg.au4Arg[1];
    GPIO_Enable(i4GpioNum, (const int *)&i4Mode);

    return PAPI_OK;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_infra_gio(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_INFRA_GIO_PINGETLATCH:
        return _papi_infra_gio_PinGetLatch(arg);
    case PAPI_IO_INFRA_GIO_PINGETLINE:
        return _papi_infra_gio_PinGetLine(arg);
    case PAPI_IO_INFRA_GIO_PINGETMODE:
        return _papi_infra_gio_PinGetMode(arg);
    case PAPI_IO_INFRA_GIO_PINSETLATCH:
        return _papi_infra_gio_PinSetLatch(arg);
    case PAPI_IO_INFRA_GIO_PINSETMODE:
        return _papi_infra_gio_PinSetMode(arg);
    default:
        printk("papi_ioctl_infra: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

