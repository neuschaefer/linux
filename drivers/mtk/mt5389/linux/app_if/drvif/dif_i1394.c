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

/** @file dif_i1394.c
 *  I1394 high-level api.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dif_debug.h"
#include "drv_stc.h"
#include "i1394_if.h"
#include "x_linux.h"
#include <linux/mt53xx_i1394.h>

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
/** _I1394OnInit
 *  Handler of 1394 init command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnInit(unsigned long arg)
{
    if (!DVMgr_Init())
    {
        LOG(3, "I1394_IOCTL: DVMgr_Init() failed\n");
        return FALSE;
    }
    
    if (I1394_Init() != I1394RET_OK)
    {
        LOG(3, "I1394_IOCTL: I1394_Init() failed\n");
        return FALSE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnGetAllDevice
 *  Handler of get all device command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnGetAllDevice(unsigned long arg)
{
    I1394_IOCTL_ALL_DEVICE_GET_T rDeviceGetAll;
    I1394_DEVICE_LIST_INFO_T *prDeviceInfo;
    PREMOTE_DEV pDeviceList = NULL;
    void   *pDrvBuff;
    INT32  i4TotalDevice;
    UINT32 u4BufferSize, u4CopySize;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof (I1394_IOCTL_ALL_DEVICE_GET_T)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get device info buffer
    if (copy_from_user(&rDeviceGetAll, (void __user *)arg,
                       sizeof(rDeviceGetAll)))
    {
        LOG(3, "I1394_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }

    u4BufferSize = rDeviceGetAll.u4BufferSize;

    if (!access_ok(VERIFY_WRITE, (void __user *)rDeviceGetAll.pTransferBuf,
                   u4BufferSize))
    {
        return FALSE;
    }
    
    pDrvBuff = x_mem_alloc(u4BufferSize);
    if (pDrvBuff == NULL)
    {
        LOG(3, "I1394_IOCTL: memory allocation failed\n");
        return FALSE;
    }
    
    if (I1394_GetAllDevices(&pDeviceList, &i4TotalDevice) != I1394RET_OK)
    {
        LOG(3, "I1394_IOCTL: I1394_GetAllDevices() failed\n");
        return FALSE;
    }

    u4CopySize = i4TotalDevice * sizeof(REMOTE_DEV);
    prDeviceInfo = (I1394_DEVICE_LIST_INFO_T *)pDrvBuff;
    prDeviceInfo->rInfoHeader.i4TotalDevice = i4TotalDevice;
    if (u4CopySize > (u4BufferSize - sizeof(prDeviceInfo->rInfoHeader)))
    {
        LOG(3, "I1394_IOCTL: insufficient user space for device list\n");
        prDeviceInfo->rInfoHeader.i4RetValue = STATUS_INSUFFICIENT_RESOURCES;
    }
    else
    {
        prDeviceInfo->rInfoHeader.i4RetValue = STATUS_SUCCESS;
        memcpy((UINT8 *)&(prDeviceInfo->prDeviceList), 
               (UINT8 *)pDeviceList, u4CopySize);
    }
    
    if (copy_to_user((void __user *)rDeviceGetAll.pTransferBuf, pDrvBuff,
                     u4BufferSize))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnGetDeviceCnt
 *  Handler of get device counter command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnGetDeviceCnt(unsigned long arg)
{
    I1394_IOCTL_DEVICE_CNT_GET_T rDeviceCnt;
    INT32  i4TotalDevice;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
        sizeof (I1394_IOCTL_DEVICE_CNT_GET_T)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get Device No. info
    if (copy_from_user(&rDeviceCnt, (void __user *)arg, 
                       sizeof(I1394_IOCTL_DEVICE_CNT_GET_T)))
    {
        LOG(3, "I1394_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }

    if (!access_ok(VERIFY_WRITE, (void __user *)rDeviceCnt.pu4DeviceNo,
                   sizeof(UINT32)))
    {
        return FALSE;
    }
    
    if (I1394_GetAllDevices(NULL, &i4TotalDevice) != I1394RET_OK)
    {
        LOG(3, "I1394_IOCTL: I1394_GetAllDevices() failed\n");
        return FALSE;
    }

    if (copy_to_user((void __user *)rDeviceCnt.pu4DeviceNo, &i4TotalDevice,
                     sizeof(INT32)))
    {
        return FALSE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnSelectDevice
 *  Handler of select device command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnSelectDevice(unsigned long arg)
{
    UINT32 u4NodeId;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg, sizeof(UINT32)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get device node id
    u4NodeId = arg;
    
    if (I1394_SelectDevice(u4NodeId) != I1394RET_OK)
    {
        LOG(3, "I1394_IOCTL: I1394_SelectDevice() failed\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnSetDeviceControl
 *  Handler of set device control command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnSetDeviceControl(unsigned long arg)
{
    I1394_IOCTRL_DEVICE_CONTROL_T rDeviceCtrl;
    UINT32 u4NodeId;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
        sizeof (I1394_IOCTRL_DEVICE_CONTROL_T)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get Device No. info
    if (copy_from_user(&rDeviceCtrl, (void __user *)arg, 
                       sizeof(I1394_IOCTRL_DEVICE_CONTROL_T)))
    {
        LOG(3, "I1394_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }

    u4NodeId = rDeviceCtrl.u4NodeId;
    
    switch (rDeviceCtrl.u4CtrlCode)
    {
    case I1394_IOCTL_CTRL_GENERAL:
        {
            UCHAR ucCtrlCode;
            
            ucCtrlCode = (UCHAR)rDeviceCtrl.u4Param1;
            if (I1394_DeviceControl(u4NodeId, ucCtrlCode) != I1394RET_OK)
            {
                LOG(3, "I1394_IOCTL: I1394_DeviceControl() failed\n");
                return FALSE;
            }
        }
        break;
        
    case I1394_IOCTL_CTRL_PLAY:
        {
            BOOL bForward = (BOOL)rDeviceCtrl.u4Param1;
            UINT32 u4Speed = rDeviceCtrl.u4Param1;
            if (I1394_SetDevicePlay(u4NodeId, bForward, u4Speed) != I1394RET_OK)
            {
                LOG(3, "I1394_IOCTL: I1394_SetDevicePlay() failed\n");
                return FALSE;
            }
        }
        break;
        
    case I1394_IOCTL_CTRL_STOP:
        if (I1394_SetDeviceStop(u4NodeId) != I1394RET_OK)
        {
            LOG(3, "I1394_IOCTL: I1394_SetDeviceStop() failed\n");
            return FALSE;
        }
        break;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnSetDeviceFormat
 *  Handler of set device format command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnSetDeviceFormat(unsigned long arg)
{
    I1394_IOCTRL_DEVICE_FMT_SET_T rDeviceFmt;
    BOOL bIsDVStream;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(I1394_IOCTRL_DEVICE_FMT_SET_T)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get Device No. info
    if (copy_from_user(&rDeviceFmt, (void __user *)arg, 
                       sizeof(I1394_IOCTRL_DEVICE_FMT_SET_T)))
    {
        LOG(3, "I1394_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }
    
    if (!access_ok(VERIFY_WRITE, (void __user *)rDeviceFmt.pbIsDVMode,
                   sizeof(BOOL)))
    {
        return FALSE;
    }
    
    if (I1394_SetSignalFormat(rDeviceFmt.u4NodeId, &bIsDVStream) != I1394RET_OK)
    {
        LOG(3, "I1394_IOCTL: I1394_SetSignalFormat() failed\n");
        return FALSE;
    }

    if (copy_to_user((void __user *)rDeviceFmt.pbIsDVMode, &bIsDVStream,
                     sizeof(BOOL)))
    {
        return FALSE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnSetIsoCtrl
 *  Handler of set Iso. channel control command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnSetIsoCtrl(unsigned long arg)
{
    I1394_IOCTRL_ISO_CONTROL_T rIsoCtrl;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(I1394_IOCTRL_ISO_CONTROL_T)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get iso. channel control info
    if (copy_from_user(&rIsoCtrl, (void __user *)arg, 
                       sizeof(I1394_IOCTRL_ISO_CONTROL_T)))
    {
        LOG(3, "I1394_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }
    
    if (I1394_ControlIsoTransaction(rIsoCtrl.bIsRxTransaction,
                                    rIsoCtrl.bIsTransactionEnable) != I1394RET_OK)
    {
        LOG(3, "I1394_IOCTL: I1394_ControlIsoTransaction() failed\n");
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _I1394OnGetDeviceState
 *  Handler of get device state command
 *
 *  @param  arg             Command-specific argument
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL _I1394OnGetDeviceState(unsigned long arg)
{
    I1394_IOCTRL_DEVICE_STATE_T rDeviceState;
    UINT8  u1Pidx = 0;
    INT32  i4DeviceState;

    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *)arg,
                   sizeof(I1394_IOCTRL_DEVICE_STATE_T)))
    {
        LOG(3, "I1394_IOCTL: access_ok() failed\n");
        return FALSE;
    }

    // Get Device No. info
    if (copy_from_user(&rDeviceState, (void __user *)arg, 
                       sizeof(I1394_IOCTRL_DEVICE_STATE_T)))
    {
        LOG(3, "I1394_IOCTL: copy_from_user() failed\n");
        return FALSE;
    }
    
    if (!access_ok(VERIFY_WRITE, (void __user *)rDeviceState.pi4DeviceState,
                   sizeof(INT32)))
    {
        return FALSE;
    }
    
    i4DeviceState = I1394_CheckDevicePlayState(rDeviceState.u4NodeId,
                                               rDeviceState.bIsDVMode, u1Pidx);

    if (copy_to_user((void __user *)rDeviceState.pi4DeviceState, &i4DeviceState,
                     sizeof(INT32)))
    {
        return FALSE;
    }
    
    return TRUE;
}
