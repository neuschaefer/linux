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

#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include "linux/file_ops.h"

//-------------------------------------------------------------------------
/** FILE_Open
 *  Open a file for read or write
 *
 *  @param  path         File path to open
 *  @param  flags        Access flag see open(2)
 *  @param  mode         The mode/permission for created file, see open(2)
 *  @retval              File handle
 */
//-------------------------------------------------------------------------
struct file* FILE_Open(const char* path, int flags, int mode)
{
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, mode);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
    	err = PTR_ERR(filp);
    	return NULL;
    }
    return filp;
}

//-------------------------------------------------------------------------
/** FILE_Read
 *  Read data from a file.
 *
 *  @param  file         Opened file handle
 *  @param  offset       Pointer to begin offset to read.
 *  @param  data         user's buffer address.
 *  @param  size         readdata length in bytes.
 *  @retval   >0         SUCCESS, return actual bytes read.
 *  @retval   <0         Fail, errno
 */
//-------------------------------------------------------------------------
int FILE_Read(struct file* file, unsigned char* data, unsigned int size, unsigned long long *offset)
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, offset);

    set_fs(oldfs);
    return ret;
}

//-------------------------------------------------------------------------
/** FILE_Write
 *  Write data from a file.
 *
 *  @param  file         Opened file handle
 *  @param  offset       Pointer to begin offset to Write.
 *  @param  data         user's buffer address.
 *  @param  size         Writedata length in bytes.
 *  @retval   >0         SUCCESS, return actual bytes Write.
 *  @retval   <0         Fail, errno
 */
//-------------------------------------------------------------------------
int FILE_Write(struct file* file, unsigned char* data, unsigned int size, unsigned long long *offset)
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, offset);

    set_fs(oldfs);
    return ret;
}

//-------------------------------------------------------------------------
/** FILE_ReadData
 *  Read data from a file.
 *
 *  @param  path         File path name to read.
 *  @param  u4Offset     begin offset to read.
 *  @param  pData        user's buffer address.
 *  @param  i4Length     readdata length in bytes.
 *  @retval   >0         SUCCESS, return actual bytes read.
 *  @retval   <0         Fail, errno
 */
//-------------------------------------------------------------------------
INT32 FILE_ReadData(const char* path, UINT32 u4Offset, UINT8 *pData, INT32 i4Length)
{
    struct file *file = NULL;
    UINT64 u8Offset = (UINT64)u4Offset;

    file = FILE_Open(path, O_RDONLY, 0);
    if (!file)
    {
        return -EFAULT;
    }

    i4Length = FILE_Read(file, (void*)pData, i4Length, &u8Offset);
    FILE_Close(file);
    return i4Length;
}


//-------------------------------------------------------------------------
/** FILE_WriteData
 *  Write data to a file.
 *
 *  @param  path         File path name to write.
 *  @param  u4Offset     begin offset to write.
 *  @param  pData        user's buffer address.
 *  @param  i4Length     writedata length in bytes.
 *  @retval   >0         SUCCESS, return actual bytes writen.
 *  @retval   <0         Fail, errno
 */
//-------------------------------------------------------------------------
INT32 FILE_WriteData(const char* path, UINT32 u4Offset, UINT8 *pData, INT32 i4Length)
{
    struct file *file = NULL;
    UINT64 u8Offset = (UINT64)u4Offset;

    file = FILE_Open(path, O_WRONLY|O_CREAT, 0644);
    if (!file)
    {
        return -EFAULT;
    }

    i4Length = FILE_Write(file, (void*)pData, i4Length, &u8Offset);
    FILE_Close(file);
    return i4Length;
}
