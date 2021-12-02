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

/** @file mt53xx_i1394.h
 *  Define I1394 interface.
 */

#ifndef MT53XX_I1394_H
#define MT53XX_I1394_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <x_typedef.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define I1394_DEVICE                            "/dev/dif"

#define STATUS_SUCCESS                          0
#define STATUS_INSUFFICIENT_RESOURCES           -1

#define I1394_IOCTL_CTRL_GENERAL                0
#define I1394_IOCTL_CTRL_PLAY                   1
#define I1394_IOCTL_CTRL_STOP                   2

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _I1394_REMOTE_DEV
{
    INT32 i4NodeID;
    INT32 i4Type;
    UCHAR uDevName[64];
} I1394_REMOTE_DEV, *PI1394_REMOTE_DEV;

// Structure of 1394 device list
typedef struct
{
    INT32  i4TotalDevice;
    INT32  i4RetValue;
} DEVICE_INFO_HEADER;

typedef struct
{
    DEVICE_INFO_HEADER rInfoHeader;
    PI1394_REMOTE_DEV  prDeviceList;
} I1394_DEVICE_LIST_INFO_T;

// Structure of get all device command
typedef struct
{
    UINT32  u4BufferSize;
    void*   pTransferBuf;
} I1394_IOCTL_ALL_DEVICE_GET_T;

// Structure of get device number command
typedef struct
{
    UINT32 *pu4DeviceNo;
} I1394_IOCTL_DEVICE_CNT_GET_T;

// Structure of set device format command
typedef struct
{
    UINT32 u4NodeId;
    BOOL*  pbIsDVMode;
} I1394_IOCTRL_DEVICE_FMT_SET_T;

// Structure of set device control command
typedef struct
{
    UINT32 u4NodeId;
    UINT32 u4CtrlCode;
    UINT32 u4Param1;
    UINT32 u4Param2;
} I1394_IOCTRL_DEVICE_CONTROL_T;

// Structure of set iso. channel control command
typedef struct
{
    BOOL bIsRxTransaction;
    BOOL bIsTransactionEnable;
} I1394_IOCTRL_ISO_CONTROL_T;

// Structure of get device play state command
typedef struct
{
    UINT32 u4NodeId;
    BOOL   bIsDVMode;
    INT32  *pi4DeviceState;
} I1394_IOCTRL_DEVICE_STATE_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------


#endif  // MT53XX_I1394_H

