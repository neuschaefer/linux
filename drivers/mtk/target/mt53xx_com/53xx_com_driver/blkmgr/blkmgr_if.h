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
#ifndef BLKMGR_IF_H
#define BLKMGR_IF_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Define Error code
#define BLKMGR_E_OK             (INT32)(0)          ///< Success
#define BLKMGR_E_FAIL           (INT32)(-1)         ///< IO error
#define BLKMGR_E_INVALID_PARAM  (INT32)(-2)         ///< Get the invalid param
#define BLKMGR_E_MEMORY_ALLOC   (INT32)(-3)         ///< Memory allocation fail


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// define is base on BLKDEV_STATUS_T in x_blk_dev.h.
typedef enum
{
    BLOCK_DEV_DISCONNECT = 0,       ///< Device is offline
    BLOCK_DEV_FAIL,                 ///< Device is online but can not work
    BLOCK_DEV_CONNECT               ///< Device is online and work fine
} BLOCK_DEV_STATE;

/// Callbalck function when the device/medium is inserted or removed
typedef VOID  (*BLOCK_CALLBACK)(UINT32 u4Tag, INT32 i4Status);

/// Definition of functions in the IO function table
typedef INT32 (*BLOCK_GETTOTALLUN)(VOID);
typedef INT32 (*BLOCK_GETLUN)(VOID);
typedef INT32 (*BLOCK_SETLUN)(UINT32 u4Lun);
typedef INT64 (*BLOCK_GETSIZE)(VOID);
typedef INT32 (*BLOCK_GETBLOCKSIZE)(VOID);
typedef INT32 (*BLOCK_GETSTATUS)(VOID);
typedef CHAR* (*BLOCK_GETMODEL)(VOID);
typedef INT32 (*BLOCK_ERASE)(UINT64 u8Offset, UINT32 u4ByteCount);
typedef INT32 (*BLOCK_READ)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
typedef INT32 (*BLOCK_WRITE)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
typedef VOID  (*BLOCK_SetHotswapCallback)(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback);

/// IO function table exported by each block device
typedef struct _BlockIOFunc 
{
    BLOCK_GETTOTALLUN           pfnGetTotalLun;     ///< Get total lun
    BLOCK_GETLUN                pfnGetLun;          ///< Get the current lun
    BLOCK_SETLUN                pfnSetLun;          ///< Set the specific lun
    BLOCK_GETSIZE               pfnGetSize;         ///< Get the device size
    BLOCK_GETBLOCKSIZE          pfnGetBlockSize;    ///< Get the block size
    BLOCK_GETSTATUS             pfnGetStatus;       ///< Get the device status
    BLOCK_GETMODEL              pfnGetModel;        ///< Get the model name
    BLOCK_ERASE                 pfnErase;           ///< Erase device
    BLOCK_READ                  pfnRead;            ///< Read device
    BLOCK_WRITE                 pfnWrite;           ///< Write deivce
    BLOCK_SetHotswapCallback    pfnSetNotify;       ///< Set the notify function
} BlockFunc_T;

/// Query the IO function table of the block device
typedef INT32 (*BLOCK_GETFUNC)(BlockFunc_T *pFuncTable);

/// The type of the block devices
typedef enum
{
    BLOCK_DEV_USB = 0,              ///< USB device
    BLOCK_DEV_MS,                   ///< MS card
    BLOCK_DEV_SD,                   ///< SD card
    BLOCK_DEV_SMXD,                 ///< SM or xD card
    BLOCK_DEV_NAND,                 ///< Nand
    BLOCK_DEV_NOR,                  ///< Nor
    BLOCK_DEV_TOTAL                 ///< Type count of supported devices
} BLOCK_DEV_TYPE;

/// Notify settings
typedef struct _BLOCK_NOTIFY_T
{
    BLOCK_CALLBACK  pfnHandler;     ///< Notify handler
    UINT32          u4Tag;          ///< Tag of notify handler
} BLOCK_NOTIFY_T;

/// The information of all connected devices
typedef struct _BlockDevConnectInfo
{
    HANDLE_T        hDevId;         ///< Handle of the device id
    BLOCK_DEV_TYPE  rType;          ///< Type of the block device (usb, ms...)
    BOOL            fActive;        ///< Device is active now
    UINT32          u4Lun;          ///< Lun No
    INT64           i8Size;         ///< Device size
    UINT8           *pModelName;    ///< Model name
} BlockDevConnectInfo_T;


//-----------------------------------------------------------------------------
// Prototype  of public functions
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
extern INT32 BlockMgrSetHotSwapNotify(const BLOCK_NOTIFY_T *prSetNotify);

//-----------------------------------------------------------------------------
/** Get the size of the selected device
 *
 *  @param  pDeviceList     Memory address of the size value
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrGetSize(INT64 *pi8Size);

//-----------------------------------------------------------------------------
/** Get the status of the selected device (connected or offline)
 *
 *  @param  pi4Status       Memory address of the device status
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrGetStatus(INT32 *pi4Status);

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
extern INT32 BlockMgrGetModel(UINT8 *pu1ModelName, UINT32 u4BuffLen);

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
extern INT32 BlockMgrErase(UINT64 u8Offset, UINT32 u4ByteCount);

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
extern INT32 BlockMgrRead(UINT64 u8Offset, UINT32 u4MemLen, UINT32 u4MemPtr);

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
extern INT32 BlockMgrWrite(UINT64 u8Offset, UINT32 u4MemLen, UINT32 u4MemPtr);

//-----------------------------------------------------------------------------
/** Allocate resource, initial internal states and start the block manager
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrInit(void);

//-----------------------------------------------------------------------------
/** Stop the block manager and release all resource
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrUninit(void);

//-----------------------------------------------------------------------------
/** Connect the selected device and open it
 *
 *  @param  hDevId          Selected device handle
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrDeviceConnect(HANDLE_T hDevId);

//-----------------------------------------------------------------------------
/** Disconnect the selected device
 *
 *  @param  hDevId          Selected device handle
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrDeviceDisconnect(HANDLE_T hDevId);

//-----------------------------------------------------------------------------
/** Get the list of all connecting block devices
 *
 *  @param  pu4ConnectDevNo Total number of the connected device
 *  @param  pDeviceList     Memory address of the device list buffer
 *
 *  @retval BLKMGR_E_OK     Succeed
 *  @retval BLKMGR_E_FAIL   Fail
 */
//-----------------------------------------------------------------------------
extern INT32 BlockMgrGetConnectDeviceList(UINT32 *pu4ConnectDevNo,
                                          BlockDevConnectInfo_T **pDeviceList);

#endif  // BLKMGR_IF_H
