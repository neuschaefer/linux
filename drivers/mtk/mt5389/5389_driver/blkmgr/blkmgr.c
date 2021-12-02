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

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "blkmgr.h"
#include "blkmgr_debug.h"
#ifdef CC_FCI_ENABLE
#include "fci_if.h"
#endif
#ifndef CC_USB_DISABLE
#include "musb_if.h"
#endif
LINT_EXT_HEADER_END
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
BLOCK_GETFUNC rBlockFuncMap[BLOCK_DEV_TOTAL] =
{
#ifndef CC_USB_DISABLE
    (BLOCK_GETFUNC)MUSB_GetBlkFuncTbl, // USB
#else
    NULL,
#endif

#ifdef CC_FCI_ENABLE
    MS_GetBlkFuncTbl,   // MS
    SD_GetBlkFuncTbl,   // SD

#ifdef CC_FCI_SMXD_SUPPORT
    SMXD_GetBlkFuncTbl, // SMXD
#endif
#else
    NULL, NULL, NULL,
#endif

    NULL,               // NAND
    NULL                // NOR
};

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
BlockMgrInfo_T *pgrBlockMgr;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define INCREASE(COUNT, MAX_LIMIT, CRITT)   if (COUNT < MAX_LIMIT)      \
                                            {                           \
                                                CRITT = x_crit_start(); \
                                                COUNT++;                \
                                                x_crit_end(CRITT);      \
                                            }

#define DECREASE(COUNT, MIN_LIMIT, CRITT)   if (COUNT > MIN_LIMIT)      \
                                            {                           \
                                                CRITT = x_crit_start(); \
                                                COUNT--;                \
                                                x_crit_end(CRITT);      \
                                            }

//-----------------------------------------------------------------------------
// Inter-file functions prototype definition
//-----------------------------------------------------------------------------
BOOL _IsDeviceSelected(VOID);
VOID _BlockMgrNotify(UINT32 u4Tag, INT32 i4Status);
VOID _UpdateDeviceProperty(UINT32 u4Index, UINT32 uLun, BLOCK_DEV_STATE rState);


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Query if the selected device is connected
 *
 *
 *  @retval TRUE            Connected
 *  @retval FALSE           Disconnected
 */
//-----------------------------------------------------------------------------
BOOL _IsDeviceSelected(VOID)
{
    BlockLunProperty_T *pLunProperty = pgrBlockMgr->prSelectDev;

    if (pLunProperty == NULL)
    {
        LOG(5, "No connected device \n");
        return (FALSE);
    }

    if (pLunProperty->rState == BLOCK_DEV_CONNECT)
    {
        LOG(7, "Device %d (lun : %d) is active now\n",
               (INT32)pLunProperty->rDevType, pLunProperty->u4Lun);
        return (TRUE);
    }

    LOG(5, "Device %d (lun : %d) is not active now\n",
           (INT32)pLunProperty->rDevType, pLunProperty->u4Lun);
    return (FALSE);
}

//-----------------------------------------------------------------------------
/** Update the device property
 *
 *  @param  u4Index         Device type
 *  @param  u4Lun           Lun no.
 *  @param  rState          Device status (connected or disconnected)
 *
 */
//-----------------------------------------------------------------------------
VOID _UpdateDeviceProperty(UINT32 u4Index, UINT32 u4Lun, BLOCK_DEV_STATE rState)
{
    BlockFunc_T *pFunc;
    BlockDevProperty_T *pDevProperty;
    BlockLunProperty_T *pLunProperty;

    pDevProperty = &pgrBlockMgr->arProperty[u4Index];
    pFunc = &pDevProperty->rFuncTable;
    pLunProperty = pDevProperty->prLunProperty + u4Lun;

    pgrBlockMgr->rCritState = x_crit_start();
    pLunProperty->rState = rState;
    x_crit_end(pgrBlockMgr->rCritState);

    if (rState == BLOCK_DEV_CONNECT)
    {
        UINT32 u4OldLun = 0;

        if (pFunc->pfnGetLun != NULL)
        {
            u4OldLun = (UINT32)pFunc->pfnGetLun();

            // Set the new lun
            if ((pFunc->pfnSetLun != NULL) && (u4OldLun != u4Lun))
            {
                UNUSED(pFunc->pfnSetLun(u4Lun));
            }
        }

        // Get the information of the connected lun device
        if (pFunc->pfnGetBlockSize != NULL)
        {
            INT32 i4BlockSize = pFunc->pfnGetBlockSize();

            pgrBlockMgr->rCritState = x_crit_start();
            pLunProperty->i4BlockSize = i4BlockSize;
            x_crit_end(pgrBlockMgr->rCritState);
        }

        if (pFunc->pfnGetSize != NULL)
        {
            INT64 i8DevSize = pFunc->pfnGetSize();

            pgrBlockMgr->rCritState = x_crit_start();
            pLunProperty->i8Size = i8DevSize * pLunProperty->i4BlockSize;
            x_crit_end(pgrBlockMgr->rCritState);
        }

        if (pFunc->pfnGetModel != NULL)
        {
            UINT8 *pModel = (UINT8 *)pFunc->pfnGetModel();

            pgrBlockMgr->rCritState = x_crit_start();
            pLunProperty->pModelName = pModel;
            x_crit_end(pgrBlockMgr->rCritState);
        }

        // Set to be the original lun
        if (pFunc->pfnSetLun != NULL)
        {
            UNUSED(pFunc->pfnSetLun(u4OldLun));
        }

        LOG(7, "Device(type:%d, lun:%d) is connected, size = %d, model = %s\n",
               u4Index, u4Lun, pLunProperty->i8Size, pLunProperty->pModelName);
    }
    else
    {
        pgrBlockMgr->rCritState = x_crit_start();
        pLunProperty->i8Size = 0;
        pLunProperty->i4BlockSize = 512;
        pLunProperty->pModelName = NULL;
        x_crit_end(pgrBlockMgr->rCritState);

        LOG(7, "Device (type: %d, lun: %d) is removed \n", u4Index, u4Lun);
    }
}

//-----------------------------------------------------------------------------
/** Notify function
 *
 *  @param  i4Tag           Device type & lun no
 *  @param  i4Status        Device status (connected or disconnected)
 *
 */
//-----------------------------------------------------------------------------
VOID _BlockMgrNotify(UINT32 u4Tag, INT32 i4Status)
{
    UINT32 u4Lun = GET_DEV_LUN(u4Tag);
    UINT32 u4DevType = GET_DEV_TYPE(u4Tag);
    BLOCK_DEV_STATE rStatus = (BLOCK_DEV_STATE)i4Status;

    if ((rStatus == BLOCK_DEV_CONNECT) || (rStatus == BLOCK_DEV_FAIL))
    {
        INCREASE(pgrBlockMgr->u4ConnetDevCnt, BLOCK_MAX_CONNECT_LUN,
                 pgrBlockMgr->rCritState)

        LOG(5, "Block Device (type : %d, lun : %d) is connected now \n",
               u4DevType, u4Lun);
    }
    else
    {
        DECREASE(pgrBlockMgr->u4ConnetDevCnt, 0, pgrBlockMgr->rCritState)
        LOG(5, "Block Device (type : %d, lun : %d) is removed now \n",
               u4DevType, u4Lun);

        if (pgrBlockMgr->i4SelectIndex == (INT32)u4DevType)
        {
            // If the selected device is removed, reset the select index
            pgrBlockMgr->rCritState = x_crit_start();
            pgrBlockMgr->i4SelectIndex = BLOCK_NO_DEV_CONNECT;
            x_crit_end(pgrBlockMgr->rCritState);

            LOG(7, "Selected index = %d \n", pgrBlockMgr->i4SelectIndex);
        }
    }

    _UpdateDeviceProperty(u4DevType, u4Lun, (BLOCK_DEV_STATE)i4Status);
    LOG(7, "%d block devices is connected now \n", pgrBlockMgr->u4ConnetDevCnt);

    if (pgrBlockMgr->rNotifyTbl.fEnableNotify)
    {
        if (pgrBlockMgr->rNotifyTbl.rHotSwap.pfnHandler != NULL)
        {
            pgrBlockMgr->rNotifyTbl.rHotSwap.pfnHandler(u4Tag, i4Status);
        }
    }
}


//-----------------------------------------------------------------------------
// Export functions
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
INT32 _BlockMgrSetHotSwapNotify(const BLOCK_NOTIFY_T *prSetNotify)
{
    if (prSetNotify == NULL)
    {
        LOG(5, "Null pointer of hot-swap notify setting \n");
        return (BLKMGR_E_INVALID_PARAM);
    }

    pgrBlockMgr->rNotifyTbl.fEnableNotify = TRUE;
    x_memcpy((void *)&pgrBlockMgr->rNotifyTbl.rHotSwap,
             (void *)prSetNotify, sizeof(BLOCK_NOTIFY_T));
    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Get the size of the selected device
 *
 *  @param  pDeviceList     Memory address of the size value
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrGetDeviceSize(INT64 *i8Size)
{
    INT32 i4DevIndex;

    if (i8Size == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    if (!_IsDeviceSelected())
    {
        *i8Size = 0;
        return (BLKMGR_E_INVALID_PARAM);
    }

    i4DevIndex = pgrBlockMgr->i4SelectIndex;
    *i8Size = pgrBlockMgr->arProperty[i4DevIndex].rFuncTable.pfnGetSize();

    pgrBlockMgr->rCritState = x_crit_start();
    pgrBlockMgr->prSelectDev->i8Size = *i8Size;
    x_crit_end(pgrBlockMgr->rCritState);

    LOG(7, "Get Device(%x) size = %x bytes \n", i4DevIndex, *i8Size);

    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Get the status of the selected device (connected or offline)
 *
 *  @param  pi4Status       Memory address of the device status
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrGetDeviceStatus(INT32 *i4Status)
{
    INT32 i4DevIndex;

    if (i4Status == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    if (!_IsDeviceSelected())
    {
        *i4Status = 0;
        return (BLKMGR_E_INVALID_PARAM);
    }

    i4DevIndex = pgrBlockMgr->i4SelectIndex;
    *i4Status = pgrBlockMgr->arProperty[i4DevIndex].rFuncTable.pfnGetStatus();

    pgrBlockMgr->rCritState = x_crit_start();
    pgrBlockMgr->prSelectDev->rState = (BLOCK_DEV_STATE)*i4Status;
    x_crit_end(pgrBlockMgr->rCritState);

    if (*i4Status == (INT32)BLOCK_DEV_CONNECT)
    {
        LOG(7, "Device(%x) is connected \n", i4DevIndex);
    }
    else
    {
        LOG(7, "Device(%x) is disconnected \n", i4DevIndex);
    }

    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Get the model name of the selected device
 *
 *  @param  pu1ModelName    Memory address of the model name buffer
 *  @param  u4BuffLen       The size of the model name buffer
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrGetDeviceModelName(UINT8 *pModelName, UINT32 u4Len)
{
    INT32 i4DevIndex;
    CHAR  *pGetModel;

    if (!_IsDeviceSelected())
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    i4DevIndex = pgrBlockMgr->i4SelectIndex;
    pGetModel = pgrBlockMgr->arProperty[i4DevIndex].rFuncTable.pfnGetModel();

    if (pGetModel == NULL)
    {
        LOG(7, "Device(%x) has no model name \n", i4DevIndex);
        x_memset(pModelName, 0, u4Len);
    }
    else
    {
        LOG(7, "Device(%x) is %s \n", i4DevIndex, pGetModel);
        x_memcpy((void *)pModelName, (void *)pGetModel, u4Len);
    }

    pgrBlockMgr->rCritState = x_crit_start();
    pgrBlockMgr->prSelectDev->pModelName = (UINT8 *)pGetModel;
    x_crit_end(pgrBlockMgr->rCritState);

    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Read the selected device
 *
 *  @param  u8Offset        The offset address of the selected device
 *  @param  u4MemLen        The lengh of the read buffer
 *  @param  u4MemPtr        The address of the read buffer
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrRead(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    BLOCK_READ         pfnRead;
    INT32              i4Ret;
    INT32              i4Select;
    UINT32             u4BlockSize;
    UINT32             u4ReadSize;
    UINT64             u8ReadOffset;
    UINT8              *pBuff;

    if (!_IsDeviceSelected())
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    i4Select = pgrBlockMgr->i4SelectIndex;
    pfnRead = pgrBlockMgr->arProperty[i4Select].rFuncTable.pfnRead;
    if (pfnRead == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    LOG(7, "Device(%x) request %x bytes (offset addr = %x, buffer addr = %x)\n",
           i4Select, u4MemLen, u8Offset, u4MemPtr);

    u4BlockSize = (UINT32)pgrBlockMgr->prSelectDev->i4BlockSize;

    u8ReadOffset = u8Offset - (u8Offset % u4BlockSize);
    u4ReadSize = u4MemLen + (u8Offset % u4BlockSize);

    if (u4ReadSize % u4BlockSize)
    {
        UINT32 u4PadSize;

        u4PadSize = u4BlockSize - (u4ReadSize % u4BlockSize);
        u4ReadSize += u4PadSize;
    }

    pBuff = (UINT8 *)x_mem_alloc(u4ReadSize);
    if (pBuff == NULL)
    {
        LOG(5, "Memory Allocation Failed !\n");
        return BLKMGR_E_MEMORY_ALLOC;
    }

    LOG(7, "Device(%x) read %x bytes (offset addr = %x, buffer addr = %x)\n",
           i4Select, u4ReadSize, u8ReadOffset, (UINT32)pBuff);

    i4Ret = pfnRead(u8ReadOffset, (UINT32)pBuff, u4ReadSize);
    if (i4Ret == BLKMGR_E_OK)
    {
        x_memcpy((void *)u4MemPtr,
                 (void *)(pBuff + (u8Offset - u8ReadOffset)), u4MemLen);
    }

    x_mem_free(pBuff);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** Write the selected device
 *
 *  @param  u8Offset        The offset address of the selected device
 *  @param  u4MemLen        The lengh of the write buffer
 *  @param  u4MemPtr        The address of the write buffer
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    BLOCK_WRITE pfnWrite;

    if (!_IsDeviceSelected())
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    pfnWrite =
        pgrBlockMgr->arProperty[pgrBlockMgr->i4SelectIndex].rFuncTable.pfnWrite;
    if (pfnWrite == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    LOG(7, "Device(%x) write %x bytes (offset addr = %x, buffer addr = %x)\n",
           pgrBlockMgr->i4SelectIndex, u4MemLen, u8Offset, u4MemPtr);
    return (pfnWrite(u8Offset, u4MemPtr, u4MemLen));
}

//-----------------------------------------------------------------------------
/** Erase the selected device
 *
 *  @param  u8Offset        The offset address of the selected device
 *  @param  u4MemLen        The erase byte count
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrErase(UINT64 u8Offset, UINT32 u4Bytes)
{
    BLOCK_ERASE pfnErase;

    if (!_IsDeviceSelected())
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    pfnErase =
        pgrBlockMgr->arProperty[pgrBlockMgr->i4SelectIndex].rFuncTable.pfnErase;
    if (pfnErase == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    LOG(7, "Device(%x) erase %x bytes fromt addr = %x)\n",
           pgrBlockMgr->i4SelectIndex, u4Bytes, u8Offset);
    return (pfnErase(u8Offset, u4Bytes));
}

//-----------------------------------------------------------------------------
/** Connect the selected device and open it
 *
 *  @param  hDeviceId       Selected device handle
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrConnectDevice(HANDLE_T hDeviceId)
{
    UINT32  u4DevType;
    UINT32  u4Lun;
    BlockLunProperty_T *pLunProperty;
    BlockLunProperty_T *pSelectDev;

    if (hDeviceId == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    pSelectDev = (BlockLunProperty_T *)hDeviceId;
    u4DevType = (UINT32)pSelectDev->rDevType;
    u4Lun = pSelectDev->u4Lun;

    if (u4DevType >= BLOCK_DEV_TOTAL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    pLunProperty = pgrBlockMgr->arProperty[u4DevType].prLunProperty + u4Lun;
    if ((pLunProperty == pSelectDev) &&
        (pLunProperty->rState == BLOCK_DEV_CONNECT))
    {
        BlockFunc_T *pfnFunc = &pgrBlockMgr->arProperty[u4DevType].rFuncTable;

        // Set the selected lun
        if (pfnFunc->pfnSetLun != NULL)
        {
            UNUSED(pfnFunc->pfnSetLun(u4Lun));
        }

        pgrBlockMgr->rCritState = x_crit_start();
        pgrBlockMgr->i4SelectIndex = (INT32)u4DevType;
        pgrBlockMgr->prSelectDev = pSelectDev;
        x_crit_end(pgrBlockMgr->rCritState);

        LOG(7, "Connect the block device -- handle : %x, type : %d, lun : %d\n",
               hDeviceId, u4DevType, u4Lun);
        return (BLKMGR_E_OK);
    }

    LOG(7, "The selected block device %x is not active \n", hDeviceId);
    return (BLKMGR_E_FAIL);
}

//-----------------------------------------------------------------------------
/** Disconnect the selected device
 *
 *  @param  hDeviceId       Selected device handle
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrDisconnectDevice(HANDLE_T hDeviceId)
{
    if (pgrBlockMgr->prSelectDev == (BlockLunProperty_T *)hDeviceId)
    {
        pgrBlockMgr->rCritState = x_crit_start();
        pgrBlockMgr->i4SelectIndex = BLOCK_NO_DEV_CONNECT;
        pgrBlockMgr->prSelectDev = NULL;
        x_crit_end(pgrBlockMgr->rCritState);

        LOG(7, "Disconnect the block device -- handle : %x, type : %d\n",
               hDeviceId, pgrBlockMgr->i4SelectIndex);
        return (BLKMGR_E_OK);
    }

    LOG(7, "The selected block device %x is not active \n", hDeviceId);
    return (BLKMGR_E_FAIL);
}

//-----------------------------------------------------------------------------
/** Get the count of all connecting block devices
 *
 *
 *  @retval the connected device no.
 */
//-----------------------------------------------------------------------------
UINT32 _BlockMgrGetConnectDeviceNo(void)
{
    LOG(7, "%d block devices is connected now \n", pgrBlockMgr->u4ConnetDevCnt);

    return (pgrBlockMgr->u4ConnetDevCnt);
}

//-----------------------------------------------------------------------------
/** Fill the list of all connecting block devices
 *
 *  @param  pDeviceList     Memory address of the device list buffer
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrFillConnectDeviceList(BlockDevConnectInfo_T *pDeviceList)
{
    UINT32                  u4DevType;
    UINT32                  u4Lun;
    BlockDevProperty_T      *pDevProperty;
    BlockLunProperty_T      *pLunProperty;
    BlockDevConnectInfo_T   *pList;

    if (pDeviceList == NULL)
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    pList = pDeviceList;
    for (u4DevType = 0; u4DevType < (UINT32)BLOCK_DEV_TOTAL; u4DevType++)
    {
        pDevProperty = &pgrBlockMgr->arProperty[u4DevType];

        for (u4Lun = 0; u4Lun < pDevProperty->u4TotalLun; u4Lun++)
        {
            pLunProperty = pDevProperty->prLunProperty + u4Lun;
            if (pLunProperty->rState != BLOCK_DEV_DISCONNECT)
            {
                pList->fActive = (pLunProperty->rState == BLOCK_DEV_CONNECT);
                pList->rType  = (BLOCK_DEV_TYPE)u4DevType;
                pList->u4Lun  = u4Lun;
                pList->hDevId = pLunProperty->hDevId;
                pList->i8Size = pLunProperty->i8Size;
                pList->pModelName = pLunProperty->pModelName;
                LOG(9, "Connected device %x -> type: %d, lun: %d, size: %d\n",
                       pList->hDevId, pList->rType, pList->u4Lun, pList->i8Size);
                LOG(9, "\t model name : %s : \n", pList->pModelName);

                pList++;
            }
        }
    }

    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Query the IO function table of each block device driver
 *
 *  @param  pFuncTable      Memory address of the Function table
 *  @param  u4DevType       Device type
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrGetDevFuncTable(BlockFunc_T *pFuncTable, UINT32 u4DevType)
{
    BLOCK_GETFUNC pfnGetFunc = rBlockFuncMap[u4DevType];

    // Get the function tables of all block devices
    if ((pFuncTable == NULL) || (pfnGetFunc == NULL))
    {
        return (BLKMGR_E_INVALID_PARAM);
    }

    pgrBlockMgr->rCritState = x_crit_start();
    VERIFY(pfnGetFunc(pFuncTable) == BLKMGR_E_OK);
    x_crit_end(pgrBlockMgr->rCritState);

    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Allocate resource, initial internal states and start the block manager
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrInit(void)
{
    UINT32 u4DevType;
    UINT32 u4Tag;
    UINT32 u4Lun;
    BLOCK_DEV_STATE rState;
    BlockLunProperty_T *pLunProperty;
    BlockDevProperty_T *pDevProperty;

    pgrBlockMgr = x_mem_alloc(sizeof(BlockMgrInfo_T));
    if (pgrBlockMgr == NULL)
    {
        return (BLKMGR_E_MEMORY_ALLOC);
    }

    // Get the IO functions of all block devices
    x_memset(pgrBlockMgr, 0, sizeof(BlockMgrInfo_T));

    // Initial the notify function setting
    pgrBlockMgr->rNotifyTbl.fEnableNotify = FALSE;
    x_memset(&pgrBlockMgr->rNotifyTbl.rHotSwap, 0, sizeof(BLOCK_NOTIFY_T));

    pgrBlockMgr->i4SelectIndex = BLOCK_NO_DEV_CONNECT;

    // Initial the block device state and register the notify function
    for (u4DevType = 0; u4DevType < (UINT32)BLOCK_DEV_TOTAL; u4DevType++)
    {
        pDevProperty = &pgrBlockMgr->arProperty[u4DevType];

        pDevProperty->hDevId = (HANDLE_T)((UINT32)pDevProperty);
        pDevProperty->rDevType = (BLOCK_DEV_TYPE)u4DevType;
        pDevProperty->u4TotalLun = 1;
        pDevProperty->prLunProperty = NULL;

        // Get IO function pointer
        UNUSED(_BlockMgrGetDevFuncTable(&pDevProperty->rFuncTable, u4DevType));

        // Get the total lun
        if (pDevProperty->rFuncTable.pfnGetTotalLun != NULL)
        {
            pDevProperty->u4TotalLun =
                (UINT32)pDevProperty->rFuncTable.pfnGetTotalLun();
        }

        pLunProperty =
            x_mem_alloc(pDevProperty->u4TotalLun * sizeof(BlockLunProperty_T));
        if (pLunProperty == NULL)
        {
            UNUSED(_BlockMgrUninit());
            return (BLKMGR_E_MEMORY_ALLOC);
        }

        pDevProperty->prLunProperty = pLunProperty;
        for (u4Lun = 0; u4Lun < pDevProperty->u4TotalLun; u4Lun++)
        {
            u4Tag = (u4DevType << 16) | u4Lun;

            pLunProperty->hDevId = (HANDLE_T)pLunProperty;
            pLunProperty->rState = BLOCK_DEV_DISCONNECT;
            pLunProperty->rDevType = (BLOCK_DEV_TYPE)u4DevType;
            pLunProperty->u4Lun = u4Lun;
            pLunProperty->i8Size = 0;
            pLunProperty->i4BlockSize = 512;
            pLunProperty->pModelName = NULL;

            // Set the Lun number
            if (pDevProperty->rFuncTable.pfnSetLun != NULL)
            {
                UNUSED(pDevProperty->rFuncTable.pfnSetLun(u4Lun));
            }

            // Get Device connection status
            if (pDevProperty->rFuncTable.pfnGetStatus != NULL)
            {
                rState =
                    (BLOCK_DEV_STATE)pDevProperty->rFuncTable.pfnGetStatus();
                if ((rState == BLOCK_DEV_CONNECT) || (rState == BLOCK_DEV_FAIL))
                {
                    pgrBlockMgr->u4ConnetDevCnt++;

                    // Update the device current status
                    _UpdateDeviceProperty(u4DevType, u4Lun, rState);
                }
            }

            // Set the callback function for card insert/remove
            if (pDevProperty->rFuncTable.pfnSetNotify != NULL)
            {
                pDevProperty->rFuncTable.pfnSetNotify(u4Tag, _BlockMgrNotify);
            }

            pLunProperty++;
        }

        // Set the default lun number to be zero
        if (pDevProperty->rFuncTable.pfnSetLun != NULL)
        {
            UNUSED(pDevProperty->rFuncTable.pfnSetLun(0));
        }
    }

    LOG(7, "%d block devices is connected now \n", pgrBlockMgr->u4ConnetDevCnt);

    return (BLKMGR_E_OK);
}

//-----------------------------------------------------------------------------
/** Stop the block manager and release all resource
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
INT32 _BlockMgrUninit(void)
{
    UINT32 i;

    for (i = 0; i < (UINT32)BLOCK_DEV_TOTAL; i++)
    {
        if (pgrBlockMgr->arProperty[i].prLunProperty != NULL)
        {
            x_mem_free(pgrBlockMgr->arProperty[i].prLunProperty);
            pgrBlockMgr->arProperty[i].prLunProperty = NULL;
        }
    }

    x_mem_free(pgrBlockMgr);
    pgrBlockMgr = NULL;
    return (BLKMGR_E_OK);
}


