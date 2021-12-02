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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "x_typedef.h"
#include "drvcust_if.h"
#include "linux/file_ops.h"

/* File base EEPROM emulation. */


#define EEPROM_FILE_NAME "/3rd/eeprom_1"

static UINT32 _u4EEPROMSize, _u4EEPROMSizeInit;

INT16 EEPROM_Init(VOID)
{
    return 0;
}

INT32 EEPROM_GetSize(VOID)
{
    if (!_u4EEPROMSizeInit)
    {
        _u4EEPROMSize = DRVCUST_InitGet(eSystemEepromSize);
        _u4EEPROMSizeInit = 1;
    }

    return (INT32)_u4EEPROMSize;
}

//-------------------------------------------------------------------------
/** EEPROM_Read
 *  SYSTEM EEPROM read function.
 *  @param  u8Offset     begin address to read.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     read data length in bytes.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    struct file *file = file_open(EEPROM_FILE_NAME, O_RDONLY, 0);

    if (!file)
    {
        memset((void*)u4MemPtr, 0, u4MemLen);
        return 0;
    }

    u4MemLen = file_read(file, (void*)u4MemPtr, u4MemLen, &u8Offset);

    file_close(file);

    return 0;
}

//-------------------------------------------------------------------------
/** EEPROM_Write
 *  SYSTEM EEPROM write function.
 *  @param  u8Offset     begin address to write.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     writedata length in bytes.
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    struct file *file = file_open(EEPROM_FILE_NAME, O_WRONLY|O_CREAT, 0644);
    UINT32 size = EEPROM_GetSize();

    if (!file)
    {
        return 1;
    }

    if (u8Offset > size)
        return 1;

    if (u8Offset + u4MemLen > size)
        u4MemLen = size - u8Offset;

    u4MemLen = file_write(file, (void*)u4MemPtr, u4MemLen, &u8Offset);

    file_close(file);

    return 0;
}

//-------------------------------------------------------------------------
/** EEPROM_X_Read
 *  Generic EEPROM read function.
 *  @param  u8Offset     begin address to read.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     read data length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_X_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                    UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size)
{
    return EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);
}

//-------------------------------------------------------------------------
/** EEPROM_X_Write
 *  Generic EEPROM write function.
 *  @param  u8Offset     begin address to write.
 *  @param  u4MemPtr     user's buffer address.
 *  @param  u4MemLen     writedata length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @param  i4SysWP     EEPROM write protect GPIO pin number
 *  @param  i4SysWPEnable EEPROM write protect enable polarity
 *  @retval   0        SUCCESS.
 *  @retval   1        FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPROM_X_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                     UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size, INT32 i4SysWP, INT32 i4SysWPEnable)
{
    return EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);
}

