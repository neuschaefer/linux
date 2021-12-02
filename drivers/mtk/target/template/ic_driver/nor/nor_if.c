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
 * $RCSfile: nor_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file serialflash_if.c
 *  Brief of file nor_serflash.c.
 *  Details of file nor_serflash.c (optional).
 */
 

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_assert.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"


#include "nor_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NOR_FLASH_BASE_ADDR     (0x28000000)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** NOR_Init() to read data from nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Init()
{
    return SFLASH_Init();
}


//-----------------------------------------------------------------------------
/** NOR_Read() to read data from nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4Offset = (UINT32) u8Offset;
    x_memcpy((void *)(u4MemPtr),
                    (void *)(NOR_FLASH_BASE_ADDR + u4Offset), u4MemLen);

    return 0;
}


//-----------------------------------------------------------------------------
/** NOR_EraseBlk() to erase the block which the offset is at.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseBlk(UINT32 u4Offset)
{
    return SFLASH_EraseOneBlk(u4Offset);
}


//-----------------------------------------------------------------------------
/** NOR_Erase() to erase data to 0xff on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Erase(UINT32 u4SectIdx, UINT32 u4SectNum)
{
    return SFLASH_EraseSectIdx(u4SectIdx, u4SectNum);
}


//-----------------------------------------------------------------------------
/** NOR_EraseAddr() to erase data to 0xff on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseAddr(UINT32 u4Offset, UINT32 u4ByteCount)
{
    return SFLASH_EraseAddr(u4Offset, u4ByteCount);
}


//-----------------------------------------------------------------------------
/** NOR_EraseChip() to erase data to 0xff on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseChip(UINT32 u4FlashIndex)
{
    return SFLASH_EraseChip(u4FlashIndex);
}


//-----------------------------------------------------------------------------
/** NOR_Write to write data to nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{   
    return SFLASH_Write((UINT32)u8Offset, u4MemPtr, u4ByteCount);
}


//-----------------------------------------------------------------------------
/** NOR_WriteByte() to write single byte on nor flash.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_WriteByte(UINT32 u4Offset, UINT32 u4Data)
{
    return SFLASH_WriteOneByte(u4Offset, (UINT8)(u4Data&0xFF));
}


//-----------------------------------------------------------------------------
/** NOR_WrNoE() to write data on nor flash and erase automaticallly if need.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_WrNoE(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    return SFLASH_WrNoE(u4Offset, u4MemPtr, u4MemLen);
}


//-----------------------------------------------------------------------------
/** NOR_GetSize() to get the size of norflash by bytes.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_GetSize()
{
    SECTTBL_T rBlkMap;
    UINT32 u4Size;

    VERIFY(SFLASH_EraseBlockMap(&rBlkMap, &u4Size) == 0);

    return (INT32)u4Size;
}


//-----------------------------------------------------------------------------
/** NOR_EraseMap() to get the norflash sector map.
 *  @return 0 successful, otherwise failed.
 */
//-----------------------------------------------------------------------------
INT32 NOR_EraseMap(SECTTBL_T *prBlkMap)
{
    return SFLASH_EraseBlockMap(prBlkMap, NULL);
}


//-----------------------------------------------------------------------------
/** NOR_Diag() diagnostic function to check norflash status.
 */
//-----------------------------------------------------------------------------
INT32 NOR_CalCRC(UINT32 u4Addr, UINT32 u4Size, UINT32* pu4Checksum)
{
    return SFLASH_CalCRC(u4Addr, u4Size, pu4Checksum);
}


//-----------------------------------------------------------------------------
/** NOR_Diag() diagnostic function to check norflash status.
 */
//-----------------------------------------------------------------------------
INT32 NOR_Diag()
{
    return SFLASH_Diag();
}


#ifdef CC_MT5382P
//-----------------------------------------------------------------------------
/** Brief of NOR_EnableDMA.
 *  Details of NOR_EnableDMA (optional).
 */
//-----------------------------------------------------------------------------
INT32 NOR_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size)
{
    return SFLASH_EnableDMA(u4SrcAddr, u4DestAddr, u4Size);
}
#endif
