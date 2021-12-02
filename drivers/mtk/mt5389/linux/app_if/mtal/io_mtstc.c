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
 * $RCSfile: io_mtstc.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file io_mtstc.c
 *  This file contains implementation of exported APIs of STC module.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "mtstc.h"
#include "stc_drvif.h"
#include "cb_low.h"
#include <linux/mtal_ioctl.h>

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
// Define definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(int)))                            \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(int)))                        \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return -1;                                          \
    }

#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return -1;                                          \
    }
#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");     \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG_K(arg,size)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   size))              \
    {                                                       \
        printk("mtal_ioctl_video valid argument error\n");     \
        return MTR_ERR_INV;                                          \
    }   

#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
    
#define COPY_TO_USER_DATA(ArgUsr, ArgKernel, size)              \
    if (copy_to_user((void __user *)ArgUsr, (void *)ArgKernel,   \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }
#define COPY_FROM_USER_DATA(ArgUsr, ArgKernel, size)              \
    if (copy_from_user((void *)ArgKernel,(void __user *)ArgUsr,    \
                       size))          \
    {                                                       \
        printk("mtal_ioctl_video argument error\n");      \
        return MTR_ERR_INV;                                          \
    }


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static MT_RESULT_T _MTSTC_GetStcValue(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 ucStcId;
    UINT32* pu4StcValue;
    UINT32 u4StcValue;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    ucStcId = (UINT8)(rArg.ai4Arg[0]);
    pu4StcValue = (UINT32*)(rArg.ai4Arg[1]);
    
    u4StcValue = STC_GetStcValue(ucStcId);

    USR_SPACE_ACCESS_VALIDATE_ARG(pu4StcValue,UINT32);
    COPY_TO_USER_ARG(pu4StcValue, u4StcValue, UINT32);

    return MTR_OK;    
}

static MT_RESULT_T _MTSTC_SetStcValue(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 ucStcId;
    UINT32 u4StcValue;
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    ucStcId = (UINT8)(rArg.ai4Arg[0]);
    u4StcValue = (UINT32)(rArg.ai4Arg[1]);

    STC_SetStcValue(ucStcId,u4StcValue);
    
    return MTR_OK;    
}

static MT_RESULT_T _MTSTC_SetSyncMode(unsigned long arg)
{
    MTAL_IOCTL_4ARG_T rArg;
    UINT8 ucStcId;
    UINT8 ucAdecId;
    UINT8 ucVdecId;
    UINT32 u4SyncMode;
    
    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);
    
    ucStcId = (UINT8)(rArg.ai4Arg[0]);
    ucAdecId = (UINT8)(rArg.ai4Arg[1]);
    ucVdecId = (UINT8)(rArg.ai4Arg[2]);
    u4SyncMode = (UINT32)(rArg.ai4Arg[3]);

    STC_SetSyncMode(ucStcId,ucAdecId,ucVdecId,u4SyncMode);
    
    return MTR_OK;    
}

static MT_RESULT_T _MTSTC_StartStc(unsigned long arg)
{
    INT32 i4Arg;
    UINT8 ucStcId;
    
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,INT32);
    COPY_FROM_USER_ARG(arg, i4Arg,INT32);
    
    ucStcId = (UINT8)(i4Arg);

    STC_StartStc(ucStcId);
    
    return MTR_OK;    
}

static MT_RESULT_T _MTSTC_StopStc(unsigned long arg)
{
    INT32 i4Arg;
    UINT8 ucStcId;
    USR_SPACE_ACCESS_VALIDATE_ARG(arg,INT32);
    COPY_FROM_USER_ARG(arg, i4Arg,INT32);
    
    ucStcId = (UINT8)(i4Arg);

    STC_StopStc(ucStcId);
    
    return MTR_OK;    
}

static MT_RESULT_T _MTSTC_GetSrc(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    UINT8 ucStcId;
    MTSTC_CLOCK* prStcInfo;
    MTSTC_CLOCK  rStcInfo;

    x_memset(&rStcInfo,0,sizeof(MTSTC_CLOCK));
    
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    
    ucStcId = (UINT8)(rArg.ai4Arg[0]);
    prStcInfo = (MTSTC_CLOCK*)(rArg.ai4Arg[1]);

    STC_GetSrc(ucStcId,(STC_CLOCK*)&rStcInfo);

    USR_SPACE_ACCESS_VALIDATE_ARG(prStcInfo,MTSTC_CLOCK);
    COPY_TO_USER_ARG(prStcInfo, rStcInfo, MTSTC_CLOCK);

    return MTR_OK;    
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------



int mtal_ioctl_mtstc(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int i4Ret = 0;

	switch (cmd)
	{
		case MTAL_IO_STC_STOP_STC:
		    return _MTSTC_StopStc(arg);
        case MTAL_IO_STC_SET_SYNC_MODE:
            return _MTSTC_SetSyncMode(arg);
        case MTAL_IO_STC_SET_STC_VALUE:
            return _MTSTC_SetStcValue(arg);        
        case MTAL_IO_STC_START_STC:
            return _MTSTC_StartStc(arg);
        case MTAL_IO_STC_GET_STC_VALUE:
            return _MTSTC_GetStcValue(arg);
        case MTAL_IO_STC_GET_SRC:
            return _MTSTC_GetSrc(arg);            
		default:
		    break;
	}
	return i4Ret;
}
