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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_papi_papionly_lkb.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_src_src.c
 *  PAPI kernel driver implementation: Src selection
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "nptv_if.h"
#include "source_table.h"
#include "vdp_if.h"
#include "mpv_if.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "aud_dsp_cfg.h"
#include "fbm_drvif.h"
#include "x_assert.h"
#include "papi.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define PAPI_PAPIONLY_LKB_FAIL                     (-0x800)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return PAPI_PAPIONLY_LKB_FAIL  ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return PAPI_PAPIONLY_LKB_FAIL  ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return PAPI_PAPIONLY_LKB_FAIL  ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return PAPI_PAPIONLY_LKB_FAIL  ; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _ioctl_Enable(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;

    return 0;
}

static INT32 _ioctl_Disable(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;

    return 0;
} 

static INT32 _ioctl_isEnabled(unsigned long u4Arg)
{
    INT32 i4Ret;
    return i4Ret;
}


static INT32 _ioctl_Init(unsigned long u4Arg)
{
    INT32 i4Ret;
    MTK_CUST_KEYBOARD_BUTTON_T *prButtons;
    UINT32 u4NumButtons;
    UINT32 u4Servo;
    void* pvUser = NULL;
    IOCTL_3ARG_T rArg;

    COPY_FROM_USER(&rArg, u4Arg, sizeof(IOCTL_3ARG_T));   
    
    if(_MTK_CUST_GetKeyboard(&u4Servo, &u4NumButtons, &prButtons))
    {
        return -1;
    }
    
    pvUser = (void*)rArg.au4Arg[2];        

    rArg.au4Arg[0] = u4Servo;
    rArg.au4Arg[1] = u4NumButtons;    
    COPY_TO_USER((void*)pvUser, (void*)prButtons, MTK_CUST_KEYBOARD_BUTTON_MAX_COUNT * sizeof(MTK_CUST_KEYBOARD_BUTTON_T));    
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_3ARG_T));
        
    i4Ret = 0;
        
    return i4Ret;
}

static INT32 _ioctl_Poll(unsigned long u4Arg)
{
    INT32 i4Ret = 0;
    IOCTL_2ARG_T rArg;

    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_2ARG_T));

    rArg.au4Arg[1] = IRRX_ReadSvadc(rArg.au4Arg[0]);

    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));
   
    return i4Ret;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_papionly_lkb(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_PAPIONLY_LKB_ENABLE:
        i4Ret = _ioctl_Enable(arg);
        break;
    case PAPI_IO_PAPIONLY_LKB_DISABLE:
        i4Ret = _ioctl_Disable(arg);
        break;
    case PAPI_IO_PAPIONLY_LKB_ISENABLED:
        i4Ret = _ioctl_isEnabled(arg);
        break;
    case PAPI_IO_PAPIONLY_LKB_INIT:
        i4Ret = _ioctl_Init(arg);
        break;         
    case PAPI_IO_PAPIONLY_LKB_POLL:
        i4Ret = _ioctl_Poll(arg);
        break;             
    default:
        printk("papi_ioctl_papiony_lkb: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}


