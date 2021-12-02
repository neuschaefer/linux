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
 * $RCSfile: fops_cli.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file cli_mt538x.c
 *  Command Line Interface of MT538X.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_serial.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_os.h"
#include "dmx_if.h"
#include "ionodes.h"
#include <linux/version.h>
#include <linux/mt53xx_cli.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
#define USE_UNLOCK_IOCTL
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return -EFAULT; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return -EFAULT; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return -EFAULT; \
    }


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

EXTERN INT32 CLI_Parser(const CHAR *szCmdBuf);

EXTERN void Serial_PutKey(char cKey);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// Capture semaphore
static HANDLE_T _hSemCapture = (HANDLE_T)NULL;


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static BOOL _OnReceiveData(const DMX_CAPTURE_INFO_T* prInfo)
{
    INT32 i4Ret;

    i4Ret = x_sema_unlock(_hSemCapture);
    if (i4Ret != OSR_OK)
    {
        printk("x_sema_unlock failed, err = %d\n", (int) i4Ret);
    }

    UNUSED(prInfo);

#if 0
    // Debug
    {
        static UINT32 _u4Count = 0;

        // Debug, drop captured data directly
//        VERIFY(DMX_UpdateCaptureReadPointer(prInfo->u4Wp));

        printk("[%u] Captured WP: 0x%08x, RP: 0x%08x, size: %u\n",
            _u4Count++, VIRTUAL(prInfo->u4Wp), VIRTUAL(prInfo->u4Rp),
            prInfo->u4DataSize);
    }
#endif

    return TRUE;
}


static BOOL _OnCaptureBufferFull(const DMX_CAPTURE_INFO_T* prInfo)
{
    printk("Capture buffer full!\n");

    UNUSED(prInfo);

    return TRUE;
}


static BOOL _CaptureHandler(DMX_CAPTURE_NOTIFY_CODE_T eCode,
    const DMX_CAPTURE_INFO_T* prInfo, void* pvTag)
{
    BOOL fgRet = FALSE;

    switch (eCode)
    {
    case DMX_CAPTURE_NOTIFY_CODE_RECEIVE_DATA:
        fgRet = _OnReceiveData(prInfo);
        break;

    case DMX_CAPTURE_NOTIFY_CODE_BUFFER_FULL:
        fgRet = _OnCaptureBufferFull(prInfo);
        break;

    default:
        ASSERT(FALSE);
        break;
    }

    return fgRet;
}


static int _OnCaptureInit(UINT32 u4Arg)
{
    // Create capture semaphore
    if (x_sema_create(&_hSemCapture, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK)
        != OSR_OK)
    {
        return -1;
    }

    if (x_sema_lock(_hSemCapture, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        return -1;
    }

    return 0;
}


static int _OnCaptureExit(UINT32 u4Arg)
{
    // Delete capture semaphore
    if (x_sema_delete(_hSemCapture) != OSR_OK)
    {
        return -1;
    }

    return 0;
}


static int _OnCaptureSet(UINT32 u4Arg)
{
    CLI_IOCTL_8ARG_T rArg;
    DMX_CAPTURE_T rCapture;
    UINT32 u4BufSize, u4Threshold;
    BOOL fgEnable;

    COPY_FROM_USER(&rArg, u4Arg, sizeof (CLI_IOCTL_8ARG_T));

    // Get parameters
    fgEnable = (BOOL)rArg.au4Arg[0];
    u4BufSize = (UINT32)rArg.au4Arg[1];
    u4Threshold = (UINT32)rArg.au4Arg[2];

    rCapture.u4BytePassDev = DMX_CAPTURE_BYPASS_FRAMER0;
    rCapture.u4BufSize = u4BufSize;
    rCapture.u4Threshold = u4Threshold;
    rCapture.pfnCaptureHandler = _CaptureHandler;
    rCapture.pvCaptureTag = NULL;

    if (!DMX_SetCapture(fgEnable, &rCapture))
    {
        return -1;
    }

    return 0;
}


static int _OnCaptureGet(UINT32 u4Arg)
{
    CLI_IOCTL_8ARG_T rArg;
    DMX_CAPTURE_INFO_T rInfo;
    UINT32 u4UserBufSize, u4MinSize, u4BufStart, u4BufEnd, u4Rp, u4Wp;
    UINT32 u4DataSize, u4RealSize, u4Size, u4BufSize;
    UINT32 *pu4RealSize;
    UINT8 *pu1UserBuf;

    COPY_FROM_USER(&rArg, u4Arg, sizeof (CLI_IOCTL_8ARG_T));

    // Get parameters
    u4UserBufSize = (UINT32)rArg.au4Arg[0];
    u4MinSize = (UINT32)rArg.au4Arg[1];
    pu1UserBuf = (UINT8*)rArg.au4Arg[2];
    pu4RealSize = (UINT32*)rArg.au4Arg[3];

    // Check parameters
    if ((u4MinSize > u4UserBufSize) || (pu1UserBuf == NULL) ||
        (pu4RealSize == NULL))
    {
        return -1;
    }

    while (1)
    {
        // Get capture info
        if (!DMX_GetCaptureInfo(&rInfo))
        {
            return -1;
        }

        if (rInfo.u4DataSize >= u4MinSize)
        {
            break;
        }

        // Not enough data, sleep
        VERIFY(x_sema_lock(_hSemCapture, X_SEMA_OPTION_WAIT) == OSR_OK);
    }

    // Check if user buffer is valid
    if (!access_ok(VERIFY_WRITE, (void __user *)pu1UserBuf, u4UserBufSize))
    {
        return -1;
    }

    u4BufStart = VIRTUAL(rInfo.u4BufStart);
    u4BufEnd = VIRTUAL(rInfo.u4BufEnd);
    u4BufSize = u4BufEnd - u4BufStart;
    u4Rp = VIRTUAL(rInfo.u4Rp);
    u4Wp = VIRTUAL(rInfo.u4Wp);
    u4DataSize = MIN(rInfo.u4DataSize, u4UserBufSize);
    u4RealSize = u4DataSize;

    // Copy data
    if ((u4Rp > u4Wp) && (u4DataSize > 0))
    {
        u4Size = MIN((u4BufEnd - u4Rp), u4DataSize);
        if (copy_to_user((void __user *)pu1UserBuf, (void*)u4Rp, u4Size))
        {
            return -1;
        }

        u4Rp += u4Size;
        if (u4Rp >= u4BufEnd)
        {
            u4Rp -= u4BufSize;
        }
        pu1UserBuf += u4Size;
        u4DataSize -= u4Size;
    }

    if ((u4Rp < u4Wp) && (u4DataSize > 0))
    {
        u4Size = MIN(u4Wp - u4Rp, u4DataSize);
        if (copy_to_user((void __user *)pu1UserBuf, (void*)u4Rp, u4Size))
        {
            return -1;
        }

        u4Rp += u4Size;
        if (u4Rp >= u4BufEnd)
        {
            u4Rp -= u4BufSize;
        }
    }

    // Update read pointer of capture buffer
    VERIFY(DMX_UpdateCaptureReadPointer(PHYSICAL(u4Rp)));

    // Update real size
    if (!access_ok(VERIFY_WRITE, (void __user *)pu4RealSize, sizeof (UINT32)))
    {
        return -1;
    }
    if (copy_to_user((void __user *)pu4RealSize, &u4RealSize, sizeof (UINT32)))
    {
        return -1;
    }

    return 0;
}


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** cli_ioctl.
 */
//-----------------------------------------------------------------------------
#ifndef USE_UNLOCK_IOCTL
int cli_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
#else
long cli_ioctl(struct file *file, unsigned int cmd,
               unsigned long arg)
#endif
{
    char cKey;
    char szCmd[MAX_CMD_LEN];
    int retval = 0;

	switch (cmd) {
    case CMD_PASSSTR:
        if (copy_from_user(&cKey, (void __user *)arg, sizeof(char)))
        {
            return -EFAULT;
        }

        Serial_PutKey(cKey);
        break;

    case CMD_PASSCMD:
        if (copy_from_user(szCmd, (void __user *)arg, sizeof(char) * MAX_CMD_LEN))
        {
            return -EFAULT;
        }

        retval = CLI_Parser(szCmd);
        break;

    case CMD_CAP_INIT:
        retval = _OnCaptureInit(arg);
        break;

    case CMD_CAP_EXIT:
        retval = _OnCaptureExit(arg);
        break;

    case CMD_CAP_SET:
        retval = _OnCaptureSet(arg);
        break;

    case CMD_CAP_GET:
        retval = _OnCaptureGet(arg);
        break;

    default:
        // Unknown command
        break;
	}
	return retval;
}

struct file_operations cli_fops = {
#ifndef USE_UNLOCK_IOCTL
	.ioctl		= cli_ioctl,
#else
	.unlocked_ioctl = cli_ioctl,
#endif
};

