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
 * $Author: jiangbo.wang $
 * $Date: 2012/11/30 $
 * $RCSfile: io_ice_vga.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_vga.c
 *  ICE VGA driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "ice_vga.h"
#include "drv_vga.h"
#include "drv_async.h"

#ifndef COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)
#define COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,    \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error8\n");      \
        return MTR_ERR_INV;                                          \
    }
#endif
#ifndef COPY_TO_USER_ARG_K(ArgUsr, ArgKernel,size)
#define COPY_TO_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error7\n");      \
        return MTR_ERR_INV;                                          \
    }  
#endif
#ifndef USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size) 
#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_ci argument error6\n");     \
        return MTR_ERR_INV;                                          \
    } 
 #endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
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
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static MT_RESULT_T _ICE_VGA_SetSyncMethod(unsigned long arg)
{
    UINT8 u1ForceSyncMode;
    UINT32 u4VgaSyncMethod = (UINT32)arg;
    
    ICE_VGA_PRINT("Set SyncMethod %d\n", (int)u4VgaSyncMethod);

    if(u4VgaSyncMethod == ice_vga_sync_horizontal_and_vertical)
    {
        u1ForceSyncMode = SEPERATESYNC;
    }
    else if(u4VgaSyncMethod == ice_vga_sync_on_green)
    {
        u1ForceSyncMode = SYNCONGREEN;
    }
    else
    {
        return MTR_NOT_OK;
    }

    bVgaForceSyncMode(u1ForceSyncMode);
    ICE_VGA_PRINT("done\n", (int)arg);

    return MTR_OK;
}

static MT_RESULT_T _ICE_VGA_GetSyncMethod(unsigned long arg)
{
    ICE_VgaSyncMethod *peVgaSyncMethod;
    ICE_VgaSyncMethod eVgaSyncMethod;
    UINT8 u1SyncMode;
    MT_RESULT_T eRet = MTR_OK;

    peVgaSyncMethod = (UINT32*)arg;

    u1SyncMode = bVgaGetCurrSyncMode();
    ICE_VGA_PRINT("Get SyncMode (drv %d)\n", u1SyncMode);

    switch(u1SyncMode)
    {
        case SYNCONGREEN:
        case COMPOSITESYNC:
            eVgaSyncMethod = ice_vga_sync_on_green;
        break;
        case SEPERATESYNC:
            eVgaSyncMethod = ice_vga_sync_horizontal_and_vertical;
        break;
        default:
            eRet = MTR_NOT_OK;
        break;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(peVgaSyncMethod, ICE_VgaSyncMethod);
    COPY_TO_USER_ARG(peVgaSyncMethod, eVgaSyncMethod,ICE_VgaSyncMethod);

    ICE_VGA_PRINT("done  SyncMethod %d\n", eVgaSyncMethod);

    return eRet;
}
extern UINT8 bVgaOpt03_SoGen;
static MT_RESULT_T _ICE_VGA_EnableDisableSOG(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pEnableDisable = (UINT8 *)arg;
    UINT8 bEnableDisable = 0;
    
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pEnableDisable, 1);
    COPY_FROM_USER_ARG_K(pEnableDisable, bEnableDisable, 1);
    printf("_ICE_VGA_EnableDisableSOG: %d", bEnableDisable);

    bVgaOpt03_SoGen = bEnableDisable;
    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_vga(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
        case ICE_IO_VGA_SETSYNCMETHOD:
            i4Ret = _ICE_VGA_SetSyncMethod(arg);  
            break;
        case ICE_IO_VGA_GETSYNCMETHOD:
            i4Ret = _ICE_VGA_GetSyncMethod(arg);  
            break;
        case ICE_IO_VGA_ENABLEDISABLESOG:
            i4Ret = _ICE_VGA_EnableDisableSOG(arg); 
            break;
        default:
            break;
    }
    return i4Ret;
}

