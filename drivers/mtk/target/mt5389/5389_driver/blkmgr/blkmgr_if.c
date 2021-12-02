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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_os.h"
#include "x_assert.h"
#include "drv_common.h"
#include "blkmgr.h"
#include "blkmgr_debug.h"


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
// Static variables
//-----------------------------------------------------------------------------
BlockDevConnectInfo_T *pConnectDevice;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
BOOL _fgBlkMgrInit = FALSE;


//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Set the notify handler if the medium is inserted or removed
 *
 *  @param  prSetNotify     Structure address of the notify setting
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrSetHotSwapNotify(const BLOCK_NOTIFY_T *prSetNotify)
{
    return (_BlockMgrSetHotSwapNotify(prSetNotify));
}

//-----------------------------------------------------------------------------
/** Get the size of the selected device
 *
 *  @param  pDeviceList     Memory address of the size value
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrGetSize(INT64 *pi8Size)
{
    return (_BlockMgrGetDeviceSize(pi8Size));
}

//-----------------------------------------------------------------------------
/** Get the status of the selected device (connected or offline)
 *
 *  @param  pi4Status       Memory address of the device status
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrGetStatus(INT32 *pi4Status)
{
    return (_BlockMgrGetDeviceStatus(pi4Status));
}

//-----------------------------------------------------------------------------
/** Get the model name of the selected device
 *
 *  @param  pu1ModelName    Memory address of the model name buffer
 *  @param  u4BuffLen       The size of the model name buffer
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrGetModel(UINT8 *pu1ModelName, UINT32 u4BuffLen)
{
    return (_BlockMgrGetDeviceModelName(pu1ModelName, u4BuffLen));
}

//-----------------------------------------------------------------------------
/** Read the selected device
 *
 *  @param  u8Offset        The offset address of the selected device
 *  @param  u4MemLen        The lengh of the read buffer
 *  @param  u4MemPtr        The address of the read buffer
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrRead(UINT64 u8Offset, UINT32 u4MemLen, UINT32 u4MemPtr)
{
    if (u4MemPtr == NULL)
    {
        LOG(5, "BlockMgrRead : NULL memory buffer pointer\n");
        return (BLKMGR_E_INVALID_PARAM);
    }

    if (u4MemLen == 0)
    {
        LOG(7, "BlockMgrRead : zero byte \n");
        return (BLKMGR_E_OK);
    }
    
    return (_BlockMgrRead(u8Offset, u4MemPtr, u4MemLen));
}

//-----------------------------------------------------------------------------
/** Write the selected device
 *
 *  @param  u8Offset        The offset address of the selected device
 *  @param  u4MemLen        The lengh of the write buffer
 *  @param  u4MemPtr        The address of the write buffer
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrWrite(UINT64 u8Offset, UINT32 u4MemLen, UINT32 u4MemPtr)
{
    if (u4MemPtr == NULL)
    {
        LOG(5, "BlockMgrWrite : NULL memory buffer pointer\n");
        return (BLKMGR_E_INVALID_PARAM);
    }

    if (u4MemLen == 0)
    {
        LOG(7, "BlockMgrWrite : zero byte \n");
        return (BLKMGR_E_OK);
    }
    
    return (_BlockMgrWrite(u8Offset, u4MemPtr, u4MemLen));
}

//-----------------------------------------------------------------------------
/** Erase the selected device
 *
 *  @param  u8Offset        The offset address of the selected device
 *  @param  u4MemLen        The erase byte count
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrErase(UINT64 u8Offset, UINT32 u4ByteCount)
{
    if (u4ByteCount == 0)
    {
        LOG(7, "BlockMgrErase : zero byte \n");
        return (BLKMGR_E_OK);
    }
    
    return (_BlockMgrErase(u8Offset, u4ByteCount));
}

//-----------------------------------------------------------------------------
/** Connect the selected device and open it
 *
 *  @param  hDevId          Selected device handle
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrDeviceConnect(HANDLE_T hDevId)
{
    if (hDevId == NULL)
    {
        LOG(5, "BlockMgrDeviceConnect : NULL device id\n");
        return (BLKMGR_E_INVALID_PARAM);
    }
    
    return (_BlockMgrConnectDevice(hDevId));
}

//-----------------------------------------------------------------------------
/** Disconnect the selected device
 *
 *  @param  hDevId          Selected device handle
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrDeviceDisconnect(HANDLE_T hDevId)
{
    if (hDevId == NULL)
    {
        LOG(5, "BlockMgrDeviceDisconnect : NULL device id\n");
        return (BLKMGR_E_INVALID_PARAM);
    }
    
    return (_BlockMgrDisconnectDevice(hDevId));
}

//-----------------------------------------------------------------------------
/** Get the list of all connecting block devices
 *
 *  @param  pu4ConnectDevNo Total number of the connected device
 *  @param  pDeviceList     Memory address of the device list buffer
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrGetConnectDeviceList(UINT32 *pu4ConnectDevNo,
                                   BlockDevConnectInfo_T **pDeviceList)
{
    INT32 i4Result = BLKMGR_E_OK;
    UINT32 u4DeviceNo;

    if ((pDeviceList == NULL) || (pu4ConnectDevNo == NULL))
    {
        LOG(5, "BlockMgrGetConnectDeviceList : NULL paramter pointers\n");
        return (BLKMGR_E_INVALID_PARAM);
    }
    
    if (pConnectDevice)
    {
        x_mem_free(pConnectDevice);
    }

    u4DeviceNo = _BlockMgrGetConnectDeviceNo();
    pConnectDevice = x_mem_alloc(sizeof(BlockDevConnectInfo_T) * u4DeviceNo);
    if (pConnectDevice == NULL)
    {
        LOG(5, "BlockMgrGetConnectDeviceList : Memory allocation failed !\n");
        return (BLKMGR_E_MEMORY_ALLOC);
    }

    i4Result = _BlockMgrFillConnectDeviceList(pConnectDevice);
    *pu4ConnectDevNo = u4DeviceNo;
    *pDeviceList = pConnectDevice;
    
    return (i4Result);
}

//-----------------------------------------------------------------------------
/** Allocate resource, initial internal states and start the block manager
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrInit(void)
{
    INT32 i4Result = BLKMGR_E_OK;
    
    if (!_fgBlkMgrInit)
    {
        pConnectDevice = NULL;
        i4Result = _BlockMgrInit();
        
        if (i4Result == BLKMGR_E_OK)
        {
            _fgBlkMgrInit = TRUE;
        }
    }
    
    return (i4Result);
}

//-----------------------------------------------------------------------------
/** Stop the block manager and release all resource
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
INT32 BlockMgrUninit(void)
{
    if (_fgBlkMgrInit)
    {
        _fgBlkMgrInit = FALSE;
    }

    x_mem_free(pConnectDevice);
    
    return (_BlockMgrUninit());
}

