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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: io_ice_common.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "mttype.h"
#include "ice.h"
#include "x_timer.h"
#include "x_assert.h"
#include "util.h"
#include "drvcust_if.h"
#include "x_hal_io.h"
#include "hw_pdwnc.h"
#include "hw_vdoin.h"
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
//variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Import functions
extern UINT8 bPDWNC_InstallTouchPad(void);
extern UINT8 bPDWNC_GetOADEnableFlag(void);
extern UINT8 bPDWNC_SetOADEnableFlag(BOOL benable);
extern 	UINT32 bPDWNC_GetLoaderVersion(void);



//-----------------------------------------------------------------------------

#define COPY_FROM_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,    \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error8\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_TO_USER_ARG_K(ArgUsr, ArgKernel,size)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_ci argument error7\n");      \
        return MTR_ERR_INV;                                          \
    }  
#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_ci argument error6\n");     \
        return MTR_ERR_INV;                                          \
    } 

MT_RESULT_T _ICE_IO_COMMON_GetLoaderVersion(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    UINT8 * pLoaderVersion = (UINT8 *)arg;
  //  UINT8 bSensing[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    UINT32 dLoaderVersion;
    
    dLoaderVersion = bPDWNC_GetLoaderVersion();
    if(dLoaderVersion == 0xffffffff)
    {
        i4Ret =  MTR_NOT_OK;
    }
    printf("dLoaderVersion: %d________________________________________________\n", dLoaderVersion);
    USR_SPACE_ACCESS_VALIDATE_ARG_K(pLoaderVersion, 4);
    COPY_TO_USER_ARG_K(pLoaderVersion, dLoaderVersion, 4);
    return i4Ret;
}

 
 MT_RESULT_T _ICE_IO_COMMON_GetOADEnableFlag(unsigned long arg)
 {
     MT_RESULT_T i4Ret = MTR_OK;
     BOOL* pOADEnableFlag = (UINT8 *)arg;
     BOOL benable;
     
     benable = bPDWNC_GetOADEnableFlag();
     if(benable == 0xff)
     {
         i4Ret =  MTR_NOT_OK;
     }
     printf("_ICE_IO_COMMON_GetOADEnableFlag: %d_____________________\n", benable);
     USR_SPACE_ACCESS_VALIDATE_ARG_K(pOADEnableFlag, 1);
     COPY_TO_USER_ARG_K(pOADEnableFlag, benable, 1);
     return i4Ret;
 }


MT_RESULT_T _ICE_IO_COMMON_SetOADEnableFlag(unsigned long arg)
{
    MT_RESULT_T i4Ret = MTR_OK;
    BOOL* pOADEnableFlag = (UINT8 *)arg;
    BOOL benable;

    USR_SPACE_ACCESS_VALIDATE_ARG_K(pOADEnableFlag, 1);
    COPY_FROM_USER_ARG_K(pOADEnableFlag, benable, 1);
    printf("\nICE_IO_COMMON_SetOADEnableFlag: %d\n", benable);
    
    i4Ret = bPDWNC_SetOADEnableFlag(benable);

    return i4Ret;
}

int ice_ioctl_common(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;

    switch (cmd)
    {
        case ICE_IO_COMMON_GET_LOADER_VERSION:
            i4Ret = _ICE_IO_COMMON_GetLoaderVersion(arg);
            break;
        case ICE_IO_COMMON_GET_OAD_ENABLE_FLAG:
            i4Ret = _ICE_IO_COMMON_GetOADEnableFlag(arg);
            break;
        case ICE_IO_COMMON_SET_OAD_ENABLE_FLAG:
            i4Ret = _ICE_IO_COMMON_SetOADEnableFlag(arg);
            break;
        default:
            break;
    }
    return i4Ret;
 }
