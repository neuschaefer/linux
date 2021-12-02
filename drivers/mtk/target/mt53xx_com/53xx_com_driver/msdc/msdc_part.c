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
 * $RCSfile: nor_part.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file msdc_part.c
 *  msdc_part.c provides functions of norflash physical/virutal partitions.
 *  In msdc_part.c, there are functions to calculate norflash physical/virtual
 *  partitions, such as  size, offset, sector map, and other necessary
 *  functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "msdc_debug.h"
#include "msdc_if.h"
#include "msdc_drv.h"
#include "x_lint.h"
#include "drvcust_if.h"
#include "x_pinmux.h"
#include "x_debug.h"


typedef struct _MSDC_PART_T
{
    UINT32 fgInitial;
    UINT32 u4PartSize[MSDCPART_PARTNUM];
    UINT32 u4PartOffset[MSDCPART_PARTNUM];
} MSDC_PART_T;


void MSDCPART_ShowPartition()
{
    UINT32 u4PartId;
    UINT32 u4Offset, u4PartSize, u4Encryped;
    
    for (u4PartId = 0; u4PartId < MSDCPART_PARTNUM; u4PartId++)
    {
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u4Offset   = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
		
        if (u4PartSize == 0)
        {
            LOG(0, "Part:#%02d, offset:0x%08x, size:0x%08x,  none\n", u4PartId, 0, 0);
        }
        else
        {
#ifdef CC_MTD_ENCRYPT_SUPPORT
            u4Encryped = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + u4PartId));
#else
            u4Encryped = 0;
#endif
            if(u4Encryped)
            {
                LOG(0, "Part:#%02d, offset:0x%08x, size:0x%08x,  ENC\n", u4PartId, u4Offset, u4PartSize);
            }
            else
            {
                LOG(0, "Part:#%02d, offset:0x%08x, size:0x%08x,  none\n", u4PartId, u4Offset, u4PartSize);
            }
        }
    }
}

#if defined(__KERNEL__)
#include <linux/mmc/mtk_msdcpart.h>
#endif

//-------------------------------------------------------------------------
/** MSDCPART_Read
*  MSDCPART_Read() to read data from emmc flash.
*  @param  u8Offset        Lower 32 bits: offset to read data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's read buffer address.
*  @param  u4MemLen     read length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MSDCPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT16 u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;

    // the high 32 bits as partition id.
    if(u2PartId >= (UINT32)MSDCPART_PARTNUM)
    {
        return 1;
    }
    
#if defined(__KERNEL__)
    // The partno in kernel is start from 1, while mtkdriver is start from 0.
    return msdc_partread(u2PartId+1, u4Offset, u4MemLen, (void *)u4MemPtr);
#else

    // get physical address by partition id.
    u4Offset += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u2PartId));

    return eMMC2DRAM(u4Offset, (UINT32 *)u4MemPtr, u4MemLen, 0);
#endif
}

//-------------------------------------------------------------------------
/** MSDCPART_Write
*  MSDCPART_Write() to write data to emmc flash.
*  @param  u8Offset        Lower 32 bits: offset to write data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's write buffer address.
*  @param  u4MemLen     Write length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MSDCPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT16 u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    
    // the high 32 bits as partition id.
    if(u2PartId >= (UINT32)MSDCPART_PARTNUM)
    {
        return 1;
    }
    
#if defined(__KERNEL__)
    // The partno in kernel is start from 1, while mtkdriver is start from 0.
    return msdc_partwrite(u2PartId+1, u4Offset, u4MemLen, (void *)u4MemPtr);
#else
    // get physical address by partition id.
    u4Offset += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u2PartId));

    return eMMC2DRAM(u4Offset, (UINT32 *)u4MemPtr, u4MemLen, 1);
#endif
}

//-------------------------------------------------------------------------
/** MSDCPART_Erase
 *  Erase selected partition, and allocate new physical block to this partition.
 *  @param  u8Offset        Selected partition and block start index to be erased.
 *  @param  u4SectNum    sector number.
 *  @retval 0	        Success
 *  @retval Others	Fail
 */
//-------------------------------------------------------------------------
INT32 MSDCPART_Erase(UINT64 u8Offset, UINT32 u4SectNum)
{
    INT32  i4Ret = MSDC_SUCCESS;
    UINT16 u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;

    // the high 32 bits as partition id.
    if(u2PartId >= (UINT32)MSDCPART_PARTNUM)
    {
        return 1;
    }

    // get physical address by partition id.
    u4Offset += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u2PartId));
    
    return i4Ret;
}

INT32 Loader_GetMsdcEraseSize(void)
{
    UINT32 u4EraseSize = 128*1024;
    
    return (INT32)u4EraseSize;
}

INT32 Loader_ReadMsdc(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
    UINT64 u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    
    if (MSDCPART_Read(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
    {
        return (INT32)u4MemLen;
    }
    else
    {
        return -1;
    }
}

INT32 Loader_WriteMsdc(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
    UINT64 u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    
    if (MSDCPART_Write(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
    {
        return (INT32)u4MemLen;
    }
    else
    {
        return -1;
    }
}

