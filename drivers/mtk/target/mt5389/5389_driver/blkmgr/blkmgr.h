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
#ifndef DRV_BLOCK_MANAGER_H
#define DRV_BLOCK_MANAGER_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "blkmgr_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define BLOCK_DEV_TYPE_MASK     0xFFFF0000
#define BLOCK_DEV_LUN_MASK      0x0000FFFF

#define BLOCK_NO_DEV_CONNECT    ((INT32)-1)

#define BLOCK_DEV_MAX_LUN       6
#define BLOCK_MAX_CONNECT_LUN   ((UINT32)BLOCK_DEV_TOTAL * BLOCK_DEV_MAX_LUN)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define GET_DEV_TYPE(u4Tag)     ((u4Tag & BLOCK_DEV_TYPE_MASK) >> 16)
#define GET_DEV_LUN(u4Tag)      (u4Tag & BLOCK_DEV_LUN_MASK)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _BLOCK_LUN_PROPERTY_T
{
    HANDLE_T            hDevId;
    BLOCK_DEV_STATE     rState;
    BLOCK_DEV_TYPE      rDevType;
    UINT32              u4Lun;
    INT64               i8Size;
    INT32               i4BlockSize;
    UINT8               *pModelName;
} BlockLunProperty_T;

typedef struct _BLOCK_DEV_PROPERTY_T
{
    HANDLE_T            hDevId;
    BLOCK_DEV_TYPE      rDevType;           
    UINT32              u4TotalLun;
    BlockLunProperty_T  *prLunProperty;
    BlockFunc_T         rFuncTable;
} BlockDevProperty_T;

typedef struct _BLOCK_NOTIFY_TABLE
{
    BOOL                fEnableNotify;
    BLOCK_NOTIFY_T      rHotSwap;
} BLOCK_NOTIFY_TABLE;

typedef struct _BLOCK_MGR_INFO_T
{
    CRIT_STATE_T        rCritState;
    BLOCK_NOTIFY_TABLE  rNotifyTbl;
    UINT32              u4ConnetDevCnt;
    INT32               i4SelectIndex;
    BlockLunProperty_T  *prSelectDev;
    BlockDevProperty_T  arProperty[BLOCK_DEV_TOTAL];
} BlockMgrInfo_T;


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
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
extern INT32 _BlockMgrSetHotSwapNotify(const BLOCK_NOTIFY_T *prSetNotify);

//-----------------------------------------------------------------------------
/** Get the size of the selected device
 *
 *  @param  pDeviceList     Memory address of the size value
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrGetDeviceSize(INT64 *i8Size);

//-----------------------------------------------------------------------------
/** Get the status of the selected device (connected or offline)
 *
 *  @param  pi4Status       Memory address of the device status
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrGetDeviceStatus(INT32 *i4Status);

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
extern INT32 _BlockMgrGetDeviceModelName(UINT8 *pModelName, UINT32 u4Len);

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
extern INT32 _BlockMgrRead(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

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
extern INT32 _BlockMgrWrite(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

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
extern INT32 _BlockMgrErase(UINT64 u8Offset, UINT32 u4Bytes);

//-----------------------------------------------------------------------------
/** Connect the selected device and open it
 *
 *  @param  hDeviceId       Selected device handle
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrConnectDevice(HANDLE_T hDeviceId);

//-----------------------------------------------------------------------------
/** Disconnect the selected device
 *
 *  @param  hDeviceId       Selected device handle
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrDisconnectDevice(HANDLE_T hDeviceId);

//-----------------------------------------------------------------------------
/** Fill the list of all connecting block devices
 *
 *  @param  pDeviceList     Memory address of the device list buffer
 *
 *  @retval FAT_E_OK        Succeed
 *  @retval FAT_E_FAIL      Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrFillConnectDeviceList(BlockDevConnectInfo_T *pDeviceList);

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
extern INT32 _BlockMgrGetDevFuncTable(BlockFunc_T *pFuncTable, UINT32 u4DevType);

//-----------------------------------------------------------------------------
/** Allocate resource, initial internal states and start the block manager
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrInit(void);

//-----------------------------------------------------------------------------
/** Stop the block manager and release all resource
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 _BlockMgrUninit(void);

//-----------------------------------------------------------------------------
/** Get the count of all connecting block devices
 *
 *
 *  @retval the connected device no.
 */
//-----------------------------------------------------------------------------
extern UINT32 _BlockMgrGetConnectDeviceNo(void);

#endif // DRV_BLOCK_MANAGER_H
