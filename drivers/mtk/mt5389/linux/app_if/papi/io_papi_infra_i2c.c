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

/** @file io_papi_infra_i2c.c
 *  PAPI kernel driver implementation: i2c
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "x_gpio.h"
#include <linux/papi_ioctl.h>	
#include "papi.h"
#include "papi_types.h"
//#include "papi_infra.h"
#include "sif_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define MAX_I2C_BUS 2

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return -EACCES; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return -EACCES; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return -EACCES; \
    } \
    if (copy_to_user((void __user *)(dst), (void *)(src), (size))) \
    { \
        return -EACCES; \
    }
        

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static Bool _fgI2CBusEnabled[MAX_I2C_BUS] = {FALSE, FALSE};
static Bool _fgI2CBusIdle[MAX_I2C_BUS] = {FALSE, FALSE};
static UINT32 _fgI2CBusClkDiv[MAX_I2C_BUS] = {0x200, 0x200};
static UINT32 _fgI2CBusTimeOut[MAX_I2C_BUS] = {0, 0};

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------
static int _io_papi_infra_i2cc_MasterEnable(unsigned long u2Arg)
{
    IOCTL_2ARG_T rArg;
    Bool bretval = TRUE;

    INT32 bus;
    Bool *pbretval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u2Arg, sizeof (IOCTL_2ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    pbretval = (Bool *)rArg.au4Arg[1];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        bretval = FALSE;
        COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
        return papi_infra_i2cc_IllegalUse;
    }

    _fgI2CBusEnabled[bus] = TRUE;

    COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cc_MasterDisable(unsigned long u2Arg)
{
    IOCTL_2ARG_T rArg;
    Bool bretval = TRUE;

    INT32 bus;
    Bool *pbretval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u2Arg, sizeof (IOCTL_2ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    pbretval = (Bool *)rArg.au4Arg[1];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        bretval = FALSE;
        COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
        return papi_infra_i2cc_IllegalUse;
    }

    _fgI2CBusEnabled[bus] = FALSE;

    COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cc_EnterIdleMode(unsigned long u2Arg)
{
    IOCTL_2ARG_T rArg;
    Bool bretval = TRUE;

    INT32 bus;
    Bool *pbretval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u2Arg, sizeof (IOCTL_2ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    pbretval = (Bool *)rArg.au4Arg[1];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        bretval = FALSE;
        COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
        return papi_infra_i2cc_IllegalUse;
    }

    _fgI2CBusIdle[bus] = TRUE;

    COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cc_LeaveIdleMode(unsigned long u2Arg)
{
    IOCTL_2ARG_T rArg;
    Bool bretval = TRUE;

    INT32 bus;
    Bool *pbretval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u2Arg, sizeof (IOCTL_2ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    pbretval = (Bool *)rArg.au4Arg[1];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        bretval = FALSE;
        COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
        return papi_infra_i2cc_IllegalUse;
    }

    _fgI2CBusIdle[bus] = FALSE;

    COPY_TO_USER(pbretval, &bretval, sizeof (Bool));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cc_GetControlWord(unsigned long u2Arg)
{
    IOCTL_2ARG_T rArg;
    UINT32 u4retval = 0;

    INT32 bus;
    UINT32 *pu4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u2Arg, sizeof (IOCTL_2ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    pu4retval = (UINT32 *)rArg.au4Arg[1];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cc_IllegalUse;
    }

    if (_fgI2CBusEnabled[bus] == TRUE)
    {
        u4retval |= MasterEnabled;
    }

    if (_fgI2CBusIdle[bus] == TRUE)
    {
        u4retval |= Idle;
    }

    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cc_SetBitRate(unsigned long u4Arg)
{
    IOCTL_4ARG_T rArg;
    UINT32 u4retval = papi_infra_i2cc_Ok;

    INT32 bus, bitrate;
    FResult *pu4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_4ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    bitrate= (INT32)rArg.au4Arg[1];
    pu4retval = (FResult *)rArg.au4Arg[2];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) || (bitrate <= 0) || (bitrate > 1000) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        u4retval = papi_infra_i2cc_IllegalUse;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cc_IllegalUse;
    }

    _fgI2CBusClkDiv[bus] = 27000/bitrate;

    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cc_SetMasterTimeout(unsigned long u4Arg)
{
    IOCTL_4ARG_T rArg;
    UINT32 u4retval = papi_infra_i2cc_Ok;

    INT32 bus, timeout;
    FResult *pu4retval;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_4ARG_T));

    bus = (INT32)rArg.au4Arg[0];
    timeout= (INT32)rArg.au4Arg[1];
    pu4retval = (FResult *)rArg.au4Arg[2];

    if ( (bus < 0) || (bus > MAX_I2C_BUS) || (timeout < 0) )
    {
        printk("%s() error: invalid parameter\n", __FUNCTION__);
        u4retval = papi_infra_i2cc_IllegalUse;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cc_IllegalUse;
    }

    _fgI2CBusTimeOut[bus] = timeout;

    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return papi_infra_i2cc_Ok;
}

static int _io_papi_infra_i2cm_Read(unsigned long u4Arg)
{
    IOCTL_4ARG_T rArg;    
    UINT32 u4retval = papi_infra_i2cm_Ok;

    INT32 i4Address, i4Len;
    Nat8 *pu1Msg;
    FResult *pu4retval;

    UINT8 u1CHannelID, u1DevAddr, *pu1KernelBuf = NULL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_4ARG_T));

    i4Address = (INT32)rArg.au4Arg[0];
    pu1Msg = (Nat8 *)rArg.au4Arg[1];
    i4Len = (INT32)rArg.au4Arg[2];
    pu4retval = (FResult *)rArg.au4Arg[3];

    u1CHannelID = (i4Address & 0xf000) >> 12;
    u1DevAddr = (UINT8)(i4Address & 0xff);

    if (_fgI2CBusEnabled[u1CHannelID] == FALSE)
    {
        u4retval = papi_infra_i2cm_BusError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_BusError;
    }
    else if (_fgI2CBusIdle[u1CHannelID] == TRUE)
    {
        u4retval = papi_infra_i2cm_Ok;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_Ok;
    }
    
    pu1KernelBuf = kmalloc(i4Len, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        u4retval = papi_infra_i2cm_OtherError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_OtherError;
    }
//    printk("\nSIF_X_Read(%x, %x, %x, 0, 0, %x, %x)\n", u1CHannelID, _fgI2CBusClkDiv[u1CHannelID], u1DevAddr, pu1KernelBuf, i4Len);
    if (SIF_X_Read(u1CHannelID, _fgI2CBusClkDiv[u1CHannelID], u1DevAddr, 0, 0, pu1KernelBuf, (UINT16)i4Len) == 0)
    {
        u4retval = papi_infra_i2cm_ReadError;        
    }
    COPY_TO_USER(pu1Msg, pu1KernelBuf, i4Len);

    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return papi_infra_i2cm_Ok;
}

static int _io_papi_infra_i2cm_Write(unsigned long u4Arg)
{
    IOCTL_4ARG_T rArg;    
    UINT32 u4retval = papi_infra_i2cm_Ok;

    INT32 i4Address, i4Len;
    Nat8 *pu1Msg;
    FResult *pu4retval;

    UINT8 u1CHannelID, u1DevAddr, *pu1KernelBuf = NULL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_4ARG_T));

    i4Address = (INT32)rArg.au4Arg[0];
    pu1Msg = (Nat8 *)rArg.au4Arg[1];
    i4Len = (INT32)rArg.au4Arg[2];
    pu4retval = (FResult *)rArg.au4Arg[3];

    u1CHannelID = (i4Address & 0xf000) >> 12;
    u1DevAddr = (UINT8)(i4Address & 0xff);

    if (_fgI2CBusEnabled[u1CHannelID] == FALSE)
    {
        u4retval = papi_infra_i2cm_BusError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_BusError;
    }
    else if (_fgI2CBusIdle[u1CHannelID] == TRUE)
    {
        u4retval = papi_infra_i2cm_Ok;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_Ok;
    }

    pu1KernelBuf = kmalloc(i4Len, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        u4retval = papi_infra_i2cm_OtherError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_OtherError;
    }
    COPY_FROM_USER(pu1KernelBuf, pu1Msg, i4Len);
    x_thread_delay(5);

//    printk("\SIF_X_Write(%x, %x, %x, 0, 0, %x, %x)\n", u1CHannelID, _fgI2CBusClkDiv[u1CHannelID], u1DevAddr, pu1KernelBuf, i4Len);        
    if (SIF_X_Write(u1CHannelID, _fgI2CBusClkDiv[u1CHannelID], u1DevAddr, 0, 0, pu1KernelBuf, (UINT16)i4Len) == 0)
    {
        u4retval = papi_infra_i2cm_WriteError;        
    }
    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return papi_infra_i2cm_Ok;
}

static int _io_papi_infra_i2cm_WriteRead(unsigned long u4Arg)
{
    IOCTL_6ARG_T rArg;    
    UINT32 u4retval = papi_infra_i2cm_Ok;

    INT32 i4Address, i4LenWrite, i4LenRead;
    Nat8 *pu1MsgWrite, *pu1MsgRead;
    FResult *pu4retval;

    INT32 i;
    UINT8 u1CHannelID, u1DevAddr, *pu1KernelBuf = NULL, *pu1AddrKernel = NULL;
    UINT32 u4WordAddr = 0;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, u4Arg, sizeof (IOCTL_6ARG_T));

    i4Address = (INT32)rArg.au4Arg[0];
    pu1MsgWrite = (Nat8 *)rArg.au4Arg[1];
    i4LenWrite = (INT32)rArg.au4Arg[2];
    pu1MsgRead = (Nat8 *)rArg.au4Arg[3];
    i4LenRead = (INT32)rArg.au4Arg[4];
    pu4retval = (FResult *)rArg.au4Arg[5];

    u1CHannelID = (i4Address & 0xf000) >> 12;
    u1DevAddr = (UINT8)(i4Address & 0xff);

    if (_fgI2CBusEnabled[u1CHannelID] == FALSE)
    {
        u4retval = papi_infra_i2cm_BusError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_BusError;
    }
    else if (_fgI2CBusIdle[u1CHannelID] == TRUE)
    {
        u4retval = papi_infra_i2cm_Ok;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_Ok;
    }

    pu1KernelBuf = kmalloc(i4LenRead, GFP_KERNEL);
    if (!pu1KernelBuf)
    {
        u4retval = papi_infra_i2cm_OtherError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_OtherError;
    }

    pu1AddrKernel = kmalloc(i4LenWrite, GFP_KERNEL);
    if (!pu1AddrKernel)
    {
        if (pu1KernelBuf) 
        {
            kfree(pu1KernelBuf);
        }
        u4retval = papi_infra_i2cm_OtherError;
        COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
        return papi_infra_i2cm_OtherError;
    }
    COPY_FROM_USER(pu1AddrKernel, pu1MsgWrite, i4LenWrite);

    for (i = 0; i < i4LenWrite; i++)
    {
        u4WordAddr = u4WordAddr << 8;
        u4WordAddr |= *(pu1AddrKernel + i);
    }
//    printk("\nSIF_X_Read(%x, %x, %x, %x, %x, %x , %x)\n", u1CHannelID, _fgI2CBusClkDiv[u1CHannelID], u1DevAddr, i4LenWrite, u4WordAddr, pu1KernelBuf, i4LenRead);
    if (SIF_X_Read(u1CHannelID, _fgI2CBusClkDiv[u1CHannelID], u1DevAddr, i4LenWrite, u4WordAddr, pu1KernelBuf, (UINT16)i4LenRead) == 0)
    {
        u4retval = papi_infra_i2cm_ReadError;        
    }

    COPY_TO_USER(pu1MsgRead, pu1KernelBuf, i4LenRead);
    if (pu1KernelBuf) 
    {
        kfree(pu1KernelBuf);
    }
    if (pu1AddrKernel) 
    {
        kfree(pu1AddrKernel);
    }
    COPY_TO_USER(pu4retval, &u4retval, sizeof (UINT32));
    return papi_infra_i2cm_Ok;        
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_infra_i2c(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_INFRA_I2CC_MASTERENABLE:
        return _io_papi_infra_i2cc_MasterEnable(arg);
    case PAPI_IO_INFRA_I2CC_MASTERDISABLE:
        return _io_papi_infra_i2cc_MasterDisable(arg);
    case PAPI_IO_INFRA_I2CC_ENTERIDLEMODE:
        return _io_papi_infra_i2cc_EnterIdleMode(arg);
    case PAPI_IO_INFRA_I2CC_LEAVEIDLEMODE:
        return _io_papi_infra_i2cc_LeaveIdleMode(arg);
    case PAPI_IO_INFRA_I2CC_GETCONTROLWORD:
        return _io_papi_infra_i2cc_GetControlWord(arg);
    case PAPI_IO_INFRA_I2CC_SETBITRATE:
        return _io_papi_infra_i2cc_SetBitRate(arg);
    case PAPI_IO_INFRA_I2CC_SETMASTERTIMEOUT:
        return _io_papi_infra_i2cc_SetMasterTimeout(arg);
    case PAPI_IO_INFRA_I2CM_READ:
        return _io_papi_infra_i2cm_Read(arg);
    case PAPI_IO_INFRA_I2CM_WRITE:
        return _io_papi_infra_i2cm_Write(arg);
    case PAPI_IO_INFRA_I2CM_WRITEREAD:
        return _io_papi_infra_i2cm_WriteRead(arg);
    default:
        printk("\npapi_ioctl_infra_i2c: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

