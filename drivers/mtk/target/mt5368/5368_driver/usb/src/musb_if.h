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

 /*---------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: musb_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_if.h
 *  Brief of file musb_if.h
 *  Details of file musb_if.h (optional).
 */

#ifndef MUSB_IF_H

#define MUSB_IF_H

#ifndef CC_USB_DISABLE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "mu_hfi_if.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef VOID  (*MUSB_CALLBACK)(UINT32 u4Tag, INT32 i4Status);

typedef INT32 (*MBLOCK_GETTOTALLUN)(VOID);
typedef INT32 (*MBLOCK_GETLUN)(VOID);
typedef INT32 (*MBLOCK_SETLUN)(UINT32 u4Lun);
typedef INT64 (*MBLOCK_GETSIZE)(VOID);
typedef INT32 (*MBLOCK_GETBLOCKSIZE)(VOID);
typedef INT32 (*MBLOCK_GETSTATUS)(VOID);
typedef CHAR* (*MBLOCK_GETMODEL)(VOID);
typedef INT32 (*MBLOCK_ERASE)(UINT64 u8Offset, UINT32 u4ByteCount);
typedef INT32 (*MBLOCK_READ)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
typedef INT32 (*MBLOCK_WRITE)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
typedef VOID  (*MBLOCK_SetHotswapCallback)(UINT32 u4Tag, MUSB_CALLBACK pfnCallback);
typedef INT32 (*MBLOCK_GETINFO)(VOID);
typedef struct _MBlockIOFunc 
{
    MBLOCK_GETTOTALLUN           pfnGetTotalLun;
    MBLOCK_GETLUN                pfnGetLun;
    MBLOCK_SETLUN                pfnSetLun;
    MBLOCK_GETSIZE               pfnGetSize;
    MBLOCK_GETBLOCKSIZE          pfnGetBlockSize;
    MBLOCK_GETSTATUS             pfnGetStatus;
    MBLOCK_GETMODEL              pfnGetModel;
    MBLOCK_ERASE                 pfnErase;
    MBLOCK_READ                  pfnRead;
    MBLOCK_WRITE                 pfnWrite;
    MBLOCK_SetHotswapCallback    pfnSetNotify;
    MBLOCK_GETINFO               pfnGetInfo;
} MBlockFunc_T;

#define MUSB_MAX_REG_NOTIFY     3

typedef enum
{
    USB_STATUS_DEV_READY = 0,                         // USB device with supported driver is connected
    USB_STATUS_DEV_ABSENT,      	                 // USB device with supported driver is removed

    USB_STATUS_UNSUPPORT,                               // Unsupported USB device is connected.
    USB_STATUS_UNSUPPORT_ABSENT,                // Unsupported usb device is removed.

    USB_STATUS_HUB_READY, 		                   // USB hub device is connected
    USB_STATUS_HUB_ABSENT, 		                   // USB hub device is connected
    
    USB_STATUS_UNSUPPORT_HUB,                       // Unsupported USB hub is connected.
    USB_STATUS_UNSUPPORT_HUB_ABSENT         // Unsupported usb hub is removed.
} MUSB_CONNECT_STATUS;

typedef struct _MUSB_HOTSWAP_DETECT_T
{
    MUSB_CONNECT_STATUS  eStatus;
    UINT16 u2VendorID;
    UINT16 u2ProductID;
    UINT8  *pManufacturer;
    UINT8  *pProduct;
    UINT8  *pSerialNum;
} MUSB_HOWSWAP_DETECT_T;

typedef VOID (*MUSB_HOTSWAP_CALLBACK)(UINT32 u4Tag,  
                                      MUSB_HOWSWAP_DETECT_T *prResult);

typedef struct _MUSB_HOTSWAP_T
{
    UINT32                u4Tag;
    MUSB_HOTSWAP_CALLBACK pfnHotSwapNotify;
} MUSB_HOTSWAP_T;

typedef struct _MUSB_HOTSWAP_NFY_T
{
    UINT32         u4Count;
    MUSB_HOTSWAP_T rHotSwap[MUSB_MAX_REG_NOTIFY];
} MUSB_HOTSWAP_NFY_T;

typedef VOID  (*MUSB_SetHotswapNotify)(MUSB_HOTSWAP_T prHotSwap);

/// init usb module to host mode.
#define MUSB_HOST_MODE          (1)         

/// init usb module to device mode.
#define MUSB_DEVICE_MODE       (0)

/// Generic Capture function.
#define MUSB_DEVICE_MODE_CAP       (0)
/// CDC USB to Uart function.
#define MUSB_DEVICE_MODE_CDC       (1)
/// MSD USB function.
#define MUSB_DEVICE_MODE_MSD      (2)

/// host waring error code.
#define MUSB_WARNING                          (MGC_M_STATUS_WARNING)
#define MUSB_CONNECT                           (MUSB_STATUS_CHILD_CONNECT)
#define MUSB_DISCONNECT                     (MUSB_STATUS_CHILD_DISCONNECT)
#define MUSB_UNSUPPORTED_DEVICE   (MUSB_STATUS_UNSUPPORTED_DEVICE)
#define MUSB_UNSUPPORTED_HUB        (MUSB_STATUS_UNSUPPORTED_HUB)
#define MUSB_UNSUPPORTED_REMOVE (MUSB_STATUS_UNSUPPORTED_REMOVE)


/** Function-only port */
#define USB_PORT_TYPE_FUNCTION   (1)
/** Host-only port */
#define USB_PORT_TYPE_HOST     (2)

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** MUSB_Read
*  MUSB read function.
*  @param  u8Offset     read offset from card, u8Offset should be multiple of block size.
*  @param  u4MemPtr      user's read buffer address.
*  @param  u4MemLen      read length, u4MemLen should be multiple of block size.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

//-------------------------------------------------------------------------
/** MUSB_Write
*  MUSB write function.
*  @param  u8Offset     write offset from card, u8Offset should be multiple of block size.
*  @param  u4MemPtr      user's write buffer address.
*  @param  u4MemLen      write length, u4MemLen should be multiple of block size.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_Write(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

//-------------------------------------------------------------------------
/** MUSB_GetModel
*  MUSB get model name function.
*  @param  VOID.
*  @return   USB model name.
*/
//-------------------------------------------------------------------------
EXTERN CHAR *MUSB_GetModel(VOID);

//-------------------------------------------------------------------------
/** MUSB_GetBlockSize
*  MUSB get block size function.
*  @param  VOID.
*  @return  the card block size in bytes.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_GetBlockSize(VOID);

//-------------------------------------------------------------------------
/** MUSB_GetSize
*  MUSB get card size function.
*  @param  VOID.
*  @return  the card size. Unit in sector:
*                512 bytes or 2048 bytes depend on pMediumInfo->dwBlockSize.
*/
//-------------------------------------------------------------------------
EXTERN INT64 MUSB_GetSize(VOID);

//-------------------------------------------------------------------------
/** MUSB_GetInsert
*  MUSB get usb insert status.
*  @param  VOID.
*  @retval 1   Device is inserted in USB socket.
*  @retval 0   Device absent.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_GetInsert(VOID);

//-------------------------------------------------------------------------
/** MUSB_GetStatus
*  MUSB get usb status.
*  @param  VOID.
*  @return  the card status in STORG_DEV_STATUS_T.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_GetStatus(VOID);

//-------------------------------------------------------------------------
/** MUSB_SetHotswapCallback
* MUSB register device call back function when usb is removed, inserted.
*  @param  u4Tag             user's tag in call back function.
*  @param  pfnCallback     user's call back function, it will be called at card inserted or removed.
*  @retval VOID.
*/
//-------------------------------------------------------------------------
EXTERN VOID MUSB_SetHotswapCallback(UINT32 u4Tag, MUSB_CALLBACK pfnCallback);

//-------------------------------------------------------------------------
/** MUSB_GetInfo
* Get target protected information.
*  @retval 0         Success.
*  @retval 2  Device read-only.
*  @retval 3  Device abstent.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_GetInfo(void);

//-------------------------------------------------------------------------
/** MUSB_GetBlkFuncTbl
* Get MUSB block function table pointer.
*  @param  pFuncTable      define the query function that the block manager 
*                                       can get a table of pointers to all IO functions.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_GetBlkFuncTbl(MBlockFunc_T *pFuncTable);

//-------------------------------------------------------------------------
/** MUSB_GetAccessType
*  MUSB get access type.
*  @retval device accesss type.
*/
//-------------------------------------------------------------------------
EXTERN MUSB_HfiAccessType MUSB_GetAccessType(void);

//-------------------------------------------------------------------------
/** MUSB_GetTotalLun
* Get total number of logical unit number in MSD device.
*  @param  void.
*  @retval   total number of logical unit number in MSD device.
*/
//-------------------------------------------------------------------------
EXTERN UINT32 MUSB_GetTotalLun(void);

//-------------------------------------------------------------------------
/** MUSB_GetLun
* Get active target logical unit number of MSD device.
*  @retval active target logical unit number of MSD device.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_GetLun(void);

//-------------------------------------------------------------------------
/** MUSB_SetLun
* Set target logical unit number of MSD device.
*  @param  u4Lun   target logical unit number of MSD device.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_SetLun(UINT32 u4Lun);

//-------------------------------------------------------------------------
/** MUSB_Suspend
* Set suspend usb bus.
*  @param  void.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_Suspend(void);

//-------------------------------------------------------------------------
/** MUSB_Resume
* Set resume usb bus.
*  @param  void.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_Resume(void);

//-------------------------------------------------------------------------
/** MUSB_SetHostErrorCallback
* Notify user usb host detect error.
*  @param  u4Tag             user's tag in call back function.
*  @param  pfnCallback     user's call back function, it will be called at host detect error.
*                                       (MUSB_WARNING, MUSB_UNSUPPORTED_DEVICE, MUSB_UNSUPPORTED_HUB, 
*                                       MUSB_UNSUPPORTED_REMOVE)
*  @retval VOID.
*/
//-------------------------------------------------------------------------
EXTERN void MUSB_SetHostErrorCallback(UINT32 u4Tag, MUSB_CALLBACK pfnCallback);

//-------------------------------------------------------------------------
/** MUSB_SetPortMode
 *  Set USB port to host or device mode function.
 *  @param  u4PortIndex   port index 0 ~ (MUSB_CountPorts() - 1).
 *  @param  u4Type	    
            1: USB_PORT_TYPE_FUNCTION
            2: USB_PORT_TYPE_HOST            
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_SetPortMode(UINT32 u4PortIndex, UINT32 u4Type);

//-------------------------------------------------------------------------
/** MUSB_Init
 *  Set USB host or device mode function.
 *  @param  u4Mode		1: host, 0: device.
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_Init(UINT32 u4Mode);

//-------------------------------------------------------------------------
/** MUSB_InitDeviceMode
 *  Set USB device mode function.
 *  @param  u4Mode 
 *      MUSB_DEVICE_MODE_CAP         (0)
 *      MUSB_DEVICE_MODE_CDC        (1)
 *      MUSB_DEVICE_MODE_MSD        (2)
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_InitDeviceMode(UINT32 u4Mode);

//-------------------------------------------------------------------------
/** MUSB_CheckSLT
 *  USB SLT check function.
 *  @param  void.
 *  @retval  0 	SUCCESS.
 *  @retval  -1	FAIL. 
 */
//-------------------------------------------------------------------------
EXTERN INT32 MUSB_CheckSLT(void);

//-------------------------------------------------------------------------
/** MUSB_InitTimeMonitor
* Initial variable of data rate monitor time.
*  @param void.
*  @retval void.
*/
//-------------------------------------------------------------------------
EXTERN void MUSB_InitTimeMonitor(void);

//-------------------------------------------------------------------------
/** MUSB_InitRateMonitor
* Get total data rate monitor operation time.
*  @param void.
*  @retval  total operation time in micro second.
*/
//-------------------------------------------------------------------------
EXTERN UINT64 MUSB_GetTimeMonitor(void);

//#ifdef CC_USBIF_TEST
//-------------------------------------------------------------------------
/** MUSB_SetHotSwap
 *  Set hot-swap notify for USB-IF test function.
 *  @param  void.
 *  @retval TRUE 	SUCCESS.
 *  @retval FALSE   FAIL. 
 */
//-------------------------------------------------------------------------
extern BOOL MUSB_SetHotSwap(const MUSB_HOTSWAP_T *prHotSwap);
//#endif

//-------------------------------------------------------------------------
/** MUSB_TurnOffVBus
 *  Turn off USB 5V.
 *  @param  void.
 *  @retval void.
 */
//-------------------------------------------------------------------------
extern void MUSB_TurnOffVBus(void);



#endif // CC_USB_DISABLE
#endif // MUSB_IF_H

