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
/*-----------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_sdnvram.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------*/

/** @file io_sdnvram.c
 *  SDAL ioctl interface of MT538X nvram driver.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include <linux/sdal_ioctl.h>
#include <linux/slab.h>

#include "storage_if.h"
#include "eeprom_if.h"

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
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
DECLARE_MUTEX(_hEEPSem);
static UINT32 _u4EEPWriteCount = 0; 
static UINT32 _u4EEPSize = 0;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _EEP_Read
 *  EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.		
 *  @retval   others	FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEP_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt;
    UINT32 u4Size = _u4EEPSize;
    UINT64 u8Offset = (UINT64)u4Offset;
    UINT8 *pu1Buf = NULL;    

    // driver use EEPROM_DRIVER_SIZE bytes in front of SDAL.
    u4Size -= EEPROM_DRIVER_SIZE;				            

    // check boundary.
    if ((u8Offset + u4MemLen) > u4Size)
    {
        return -1;
    }

    pu1Buf = kmalloc (u4MemLen, GFP_KERNEL);
    if (!pu1Buf) 
    {
        return -1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DRIVER_SIZE;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        i4Ret = -1;
        goto _EEP_READEXIT;
    }
    
    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_READ,
        NULL, NULL, NULL, u8Offset, u4MemLen, (void *)pu1Buf, &u4XferCnt);    
    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        i4Ret = -1;
        goto _EEP_READEXIT;
    }    
    
    if (copy_to_user((void __user *)u4MemPtr, (const void *)pu1Buf, u4MemLen))
    {
        i4Ret = -EFAULT;
    }

_EEP_READEXIT:
    kfree(pu1Buf);
    
    i4Ret1 = STORGCloseDev(prDev);
    UNUSED(i4Ret1);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _EEP_Write
 *  EEPROM write function. 
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.		
 *  @retval   others	FAIL.
 */
//-------------------------------------------------------------------------
static INT32 _EEP_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt;
    UINT32 u4Size = _u4EEPSize;
    UINT64 u8Offset = (UINT64)u4Offset;
    UINT8 *pu1Buf = NULL;    

    _u4EEPWriteCount = 0;

    // driver use EEPROM_DRIVER_SIZE bytes in front of SDAL.
    u4Size -= EEPROM_DRIVER_SIZE;				            

    // check boundary.
    if ((u8Offset + u4MemLen) > u4Size)
    {
        return -1;
    }

    pu1Buf = kmalloc (u4MemLen, GFP_KERNEL);
    if (!pu1Buf) 
    {
        return -1;
    }

    if (copy_from_user((void *)pu1Buf, (void __user *)u4MemPtr, u4MemLen))
    {
        i4Ret = -1;
        goto _EEP_WRITEEXIT;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DRIVER_SIZE;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        i4Ret = -1;
        goto _EEP_WRITEEXIT;
    }
    
    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE,
        NULL, NULL, NULL, u8Offset, u4MemLen, (void *)pu1Buf, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        i4Ret = -1;
        goto _EEP_WRITEEXIT;
    }
    else
    {
        _u4EEPWriteCount = u4MemLen;
    }

_EEP_WRITEEXIT:
    kfree(pu1Buf);

    i4Ret1 = STORGCloseDev(prDev);
    UNUSED(i4Ret1);

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int sdal_ioctl_sdnvram(struct inode *inode, struct file *file, unsigned int cmd,
                       unsigned long arg)
{
    SDAL_IOCTL_4ARG_T rArg4;
    int i4Ret = 0;

    if (down_interruptible(&_hEEPSem))
    {
        return -ERESTARTSYS;
    }
    
    // Check access of user space
    if (!access_ok(VERIFY_READ, (void __user *) arg, _IOC_SIZE(cmd)))
    {
        i4Ret = -EFAULT;
        goto _EEP_EXIT;
    }

    // Copy user space argument
    if (copy_from_user(&rArg4, (void __user *) arg, _IOC_SIZE(cmd)))
    {
        i4Ret = -EFAULT;
        goto _EEP_EXIT;
    }

    // nvram init.
    if (_u4EEPSize == 0)
    {
        if (STORGInit() != 0)
        {
            i4Ret = -1;
            goto _EEP_EXIT;           
        }
        
        _u4EEPSize = (UINT32)STORG_GetDevSize(STORG_FLAG_EEPROM, NULL);
        ASSERT (_u4EEPSize > EEPROM_DRIVER_SIZE);   
    }

    switch (cmd)
    {
        case SDAL_IO_NVRAM_INIT:            
            break;

        case SDAL_IO_NVRAM_SET:
            i4Ret = _EEP_Write(rArg4.au4Arg[0], rArg4.au4Arg[1], rArg4.au4Arg[2]);
            break;

        case SDAL_IO_NVRAM_GET:
            i4Ret = _EEP_Read(rArg4.au4Arg[0], rArg4.au4Arg[1], rArg4.au4Arg[2]);
            break;

        case SDAL_IO_NVRAM_GETSTATUS:
            // Copy data to user space.
            if (copy_to_user((void __user *) rArg4.au4Arg[1], &_u4EEPWriteCount, sizeof(UINT32)))
            {
                i4Ret = -EFAULT;
            }
            break;
            
        case SDAL_IO_NVRAM_DEBUG:
            break;
            
        default:
            i4Ret = -1;
            break;
    }

_EEP_EXIT:

    up(&_hEEPSem);
    
    return i4Ret;
}

