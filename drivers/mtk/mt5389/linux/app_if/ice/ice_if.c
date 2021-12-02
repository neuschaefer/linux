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
 * $RCSfile: ice_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ice_if.c
 *  ICE ioctl interface.
 */

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// IOCTL debug flag
//#define IOCTL_DEBUG

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtal.h"
#include "ice.h"
#include <linux/version.h>
#include <linux/ice_ioctl.h>
#include <linux/mm.h>
#include <asm/pgtable.h>

#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_linux.h"
#include "x_typedef.h"

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

static int ice_open(struct inode * inode, struct file * filp)
{

    return 0;
}

static int ice_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg)
{
    int retval = 0;
    switch (_IOC_TYPE(cmd))
    {
        case ICE_IOCTYPE_AUDIO:
                retval = ice_ioctl_aud(inode, file, cmd, arg);
             break;   
        case ICE_IOCTYPE_VGA:
                retval = ice_ioctl_vga(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_HDMI:
                retval = ice_ioctl_hdmi(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_VIDEO:
                retval = ice_ioctl_video(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_PMX:
                retval = ice_ioctl_pmx(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_OSD:
                retval = ice_ioctl_osd(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_HARDWARE:
                retval = ice_ioctl_hardware(inode, file, cmd, arg);
            break;
        case ICE_IOMTPERIPHERAL_IR:
                retval = ice_ioctl_ir(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_FILESYSTEM:
                retval = ice_ioctl_filesystem(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_TOUCHPAD:
                retval = ice_ioctl_touchpad(inode, file, cmd, arg);
            break;
        case ICE_IOCTYPE_COMMON:
                retval = ice_ioctl_common(inode, file, cmd, arg);
            break;
        default:
            break;
    }

#ifdef IOCTL_DEBUG
    printk("[ICE]IOCTL case: 0x%08x, cmd: 0x%08x, arg: 0x%08x, ret: %d\n",_IOC_TYPE(cmd), cmd, arg, retval);
#endif

    return retval;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

struct file_operations ice_fops = {
    .open       = ice_open,
    .ioctl      = ice_ioctl,
};


