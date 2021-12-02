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
 * $RCSfile: serialflash_func.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file serialflash_if.c
 *  Brief of file serialflash_if.c.
 *  Details of file serialflash_if.c (optional).
 */
 

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_assert.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_printf.h"
#include "x_util.h"

#include "nor_debug.h"
#include "nor_if.h"
#include "serialflash_hw.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 u4RegVal;
    CHAR   acName[16];
} SFLASH_TESTITEM_T;


static SFLASH_TESTITEM_T _arSampleEdge[] =
{
//    { 0x0, "Rising " },
    { 0x1, "Falling" },
};

static SFLASH_TESTITEM_T _arReadType[] =
{
//    { 0x0, "Normal Read" },
    { 0x1, "Fast Read  " },
};

static SFLASH_TESTITEM_T _arClk[] =
{
 //   { 0x0, " 1MHZ" },
//    { 0x1, "12MHZ" },
//    { 0x2, "15MHZ" },
    { 0x3, "20MHZ" },
    { 0x5, "30MHZ" },
    { 0x6, "40MHZ" },   // PLL
    { 0x7, "50MHZ" },   // PLL
    { 0x8, "60MHZ" },
};


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

/** The information of each erase region of all chips.
 *  Each entry contains the sector size and the number of sectors of each
 *  erase region.
 */
static NORSEGINFO_T _arSegInfo[MAX_FLASHCOUNT];   // Compatible with NOR flash
static SFLASH_INFO_T _rSFInfo;
static UINT8 _au1BackBuf[16 * 1024];


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
static INT32 _MapSectAddr(UINT32 u4Addr, UINT32 *pu4ChipIdx, UINT32 *pu4SectAddr)
{
    UINT32 i;

    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    if(u4Addr >= PBI_A_BASE)
    {
        u4Addr -= PBI_A_BASE;
    }
        
    for(i=0; i<_rSFInfo.u1FlashCount; i++)
    {
        if(u4Addr < _rSFInfo.arFlashInfo[i].u4ChipSize)
        {
            if(pu4ChipIdx != NULL)
            {
                *pu4ChipIdx = i;
            }

            if(pu4SectAddr != NULL)
            {
                *pu4SectAddr = u4Addr & ~(_rSFInfo.arFlashInfo[i].u4SecSize - 1);            
            }
            return 0;
        }
    }

    return 1;
}


//-----------------------------------------------------------------------------
static INT32 _MapSectIndex(UINT32 u4SectIndex, UINT32 *pu4ChipIdx, UINT32 *pu4SectAddr)
{
    UINT32 i, u4SectCount;

    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    for(i=0; i<_rSFInfo.u1FlashCount; i++)
    {
        u4SectCount = _rSFInfo.arFlashInfo[i].u4SecCount;
        
        if(u4SectIndex < u4SectCount)
        {
            if(pu4ChipIdx != NULL)
            {
                *pu4ChipIdx = i;
            }

            // return sector addres in flash offset
            if(pu4SectAddr != NULL)
            {
                *pu4SectAddr = u4SectIndex * _rSFInfo.arFlashInfo[i].u4SecSize;
            }
            return 0;
        }
        else
        {
            u4SectIndex -= u4SectCount;
        }
    }

    return 1;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of SFLASH_Init.
 *  Details of SFLASH_Init (optional).
 *  @retval 0    Success
 *  @retval 1   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_Init()
{
    static BOOL fgInit = FALSE;
    UINT32 i;

    if(fgInit)
    {
        return 0;
    }

    x_memset((VOID*)&_rSFInfo, 0x0, sizeof(SFLASH_INFO_T));
    if(SFLASHHW_Init() != 0)
    {
        return 1;
    }

    SFLASHHW_GetFlashInfo(&_rSFInfo);

    for(i=0;i<_rSFInfo.u1FlashCount; i++)
    {
        _arSegInfo[i].u4SectNum = _rSFInfo.arFlashInfo[i].u4SecCount;
        _arSegInfo[i].u4SectSize = _rSFInfo.arFlashInfo[i].u4SecSize;
    }
        
    fgInit = TRUE;
    
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_Reset.
 *  Details of SFLASH_Reset (optional).
 *  @retval 0   Success
 *  @retval 1   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_Reset()
{
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_GetID.
 *  Details of SFLASH_GetID (optional).
 *  @retval 0    Success
 *  @retval 1   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_GetID(UINT32 u4Index, UINT8 *pu1MenuID, UINT8 *pu1DevID1, UINT8 *pu1DevID2)
{
    INT32 i4Ret;

    if((pu1MenuID == NULL) || (pu1DevID1 == NULL) || (pu1DevID2 == NULL))
    {
        return 1;
    }

    i4Ret = SFLASHHW_GetID(u4Index, pu1MenuID, pu1DevID1, pu1DevID2);

    return i4Ret;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_Read.
 *  Details of SFLASH_Read (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
#if 1
    x_memcpy((void *)(u4MemPtr),
                    (void *)(SFLASH_MEM_BASE + u4Offset), u4ByteCount);
#else
    SFLASHHW_Read(u4Offset, u4ByteCount, (UINT8*)u4MemPtr);
#endif

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_Write.
 *  Details of SFLASH_Write (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
    UINT8 *pu1Buf;
    UINT32 i, j, u4WriteAddr, u4Len, u4WriteByte, u4ChipOffset, u4SectStart, u4SectEnd;

    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    ASSERT(_rSFInfo.u1FlashCount > 0);

    pu1Buf = (UINT8*)u4MemPtr;

    u4WriteAddr = u4Offset;
    u4WriteByte = u4ByteCount;
    u4ChipOffset = 0;
    u4SectStart = 0;
    u4SectEnd = 0;
    for(i=0; i<_rSFInfo.u1FlashCount; i++)
    {
        if(SFLASH_WriteProtect(i, FALSE) != 0)
        {
            LOG(3, "Disable Flash write protect fail!\n");
            return 1;
        }
        for(j=0; j<_rSFInfo.arFlashInfo[i].u4SecCount; j++)
        {
            u4SectEnd = u4SectStart + _rSFInfo.arFlashInfo[i].u4SecSize;
            if((u4SectStart<=u4WriteAddr)&&(u4WriteAddr<u4SectEnd))
            {
                u4Len = _rSFInfo.arFlashInfo[i].u4SecSize - (u4WriteAddr - u4SectStart);
                if(u4Len >= u4WriteByte)
                {
                    u4Len = u4WriteByte;
                }
                
                if(SFLASHHW_WriteSector(i, u4WriteAddr - u4ChipOffset, u4Len, pu1Buf) != 0)
                {
                    LOG(3, "Write flash error !\n");
                    if(SFLASH_WriteProtect(i, TRUE) != 0)
                    {
                        LOG(3, "Enable flash write protect fail!\n");
                        return 1;
                    }
                    return 1;
                }

                u4WriteAddr += u4Len;                
                u4WriteByte -= u4Len;
                pu1Buf += u4Len;

                if(u4WriteByte == 0)
                {
                    if(SFLASH_WriteProtect(i, TRUE) != 0)
                    {
                        LOG(3, "Enable flash write protect fail!\n");
                        return 1;
                    }                        
                    return 0;
                }
            }
            u4SectStart += _rSFInfo.arFlashInfo[i].u4SecSize;
        }
        u4ChipOffset += _rSFInfo.arFlashInfo[i].u4ChipSize;

        if(SFLASH_WriteProtect(i, TRUE) != 0)
        {
            LOG(3, "Enable flash write protect fail!\n");
            return 1;
        }
    }

    return 1;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_WriteOneByte.
 *  Details of SFLASH_WriteOneByte (optional).
 *  @retval 0   Success
 *  @retval 1   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_WriteOneByte(UINT32 u4Addr, UINT8 u1Data)
{
    UINT8 u1Val;

    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    ASSERT(_rSFInfo.u1FlashCount > 0);

    // Read
    if(SFLASH_Read(u4Addr, (UINT32)&u1Val, 1) != 0)
    {
        LOG(3, "Read flash fail!\n");
        return 1;
    }

    if(u1Val == u1Data)
    {
        return 0;
    }

    if(((~u1Val) & u1Data) != 0)
    {
        LOG(3, "Erase first !\n");
        return 1;
    }
    
    if(SFLASH_Write(u4Addr, (UINT32)&u1Data, 1) != 0)
    {
        return 1;
    }
        
    return 0;
}



//-----------------------------------------------------------------------------
/** Brief of SFLASH_EraseAddr.
 *  Details of SFLASH_EraseAddr (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_EraseAddr(UINT32 u4Offset, UINT32 u4ByteLen)
{
    UINT32 i, j, u4EraseAddr, u4ChipOffset, u4SectStart, u4SectEnd;

    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    ASSERT(_rSFInfo.u1FlashCount > 0);    

    u4EraseAddr = u4Offset;
    u4ChipOffset = 0;
    u4SectStart = 0;
    u4SectEnd = 0;
    for(i=0; i<_rSFInfo.u1FlashCount; i++)
    {
        if(SFLASH_WriteProtect(i, FALSE) != 0)
        {
            LOG(3, "Disable Flash write protect fail!\n");
            return 1;
        }
    
        for(j=0; j<_rSFInfo.arFlashInfo[i].u4SecCount; j++)
        {
            u4SectEnd = u4SectStart + _rSFInfo.arFlashInfo[i].u4SecSize;
            if((u4SectStart<=u4EraseAddr)&&(u4EraseAddr<u4SectEnd))
            {
                if(SFLASHHW_EraseSector(i, u4SectStart - u4ChipOffset) != 0)
                {
                    LOG(3, "Erase chip #%d, sector 0x%08X ~ 0x%08X FAIL\n", i,
                        u4SectStart - u4ChipOffset,
                        (u4SectStart - u4ChipOffset) + (_rSFInfo.arFlashInfo[i].u4SecSize - 1));

                    if(SFLASH_WriteProtect(i, TRUE) != 0)
                    {
                        LOG(3, "Enable Flash write protect fail!\n");
                        return 1;
                    }
                    return 1;
                }
                else
                {
                    LOG(5, "Erase chip #%d, sector 0x%08X ~ 0x%08X OK\n", i,
                        u4SectStart - u4ChipOffset,
                        (u4SectStart - u4ChipOffset) + (_rSFInfo.arFlashInfo[i].u4SecSize - 1));
                }

                u4EraseAddr = u4SectEnd;
            }
            
            if(u4EraseAddr >= (u4Offset + u4ByteLen))
            {
                if(SFLASH_WriteProtect(i, TRUE) != 0)
                {
                    LOG(3, "Enable Flash write protect fail!\n");
                    return 1;
                }            
                return 0;
            }

            u4SectStart += _rSFInfo.arFlashInfo[i].u4SecSize;            
        }
        u4ChipOffset += _rSFInfo.arFlashInfo[i].u4ChipSize;

        if(SFLASH_WriteProtect(i, TRUE) != 0)
        {
            LOG(3, "Enable flash write protect fail!\n");
            return 1;
        }
    }
    
    return 1;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_EraseOneBlk.
 *  Details of SFLASH_EraseOneBlk (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_EraseOneBlk(UINT32 u4Offset)
{
    UINT32 u4FlashIdx, u4SectAddr; 
    INT32 i4Ret;

    if(_MapSectAddr(u4Offset, &u4FlashIdx, &u4SectAddr) == 1)
    {
        return 1;
    }

    if(SFLASH_WriteProtect(u4FlashIdx, FALSE) != 0)
    {
        LOG(3, "Disable flash write protect fail!\n");
        return 1;
    }

    i4Ret = SFLASHHW_EraseSector(u4FlashIdx, u4SectAddr);

    if(SFLASH_WriteProtect(u4FlashIdx, TRUE) != 0)
    {
        LOG(3, "Enable flash write protect fail!\n");
        return 1;
    }
    
    return i4Ret;    
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_EraseSectIdx.
 *  Details of SFLASH_EraseSectIdx (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_EraseSectIdx(UINT32 u4SectIdx, UINT32 u4SectNum)
{
    UINT32 i, u4FlashIdx, u4SectAddr;

    for(i=0; i<u4SectNum; i++)
    {
        if(_MapSectIndex(u4SectIdx + i, &u4FlashIdx, &u4SectAddr) != 0)
        {
            return 1;
        }

        if(SFLASH_WriteProtect(u4FlashIdx, FALSE) != 0)
        {
            LOG(3, "Disable flash write protect fail!\n");
            return 1;
        }

        if(SFLASHHW_EraseSector(u4FlashIdx, u4SectAddr) != 0)
        {
            return 1;
        }

        if(SFLASH_WriteProtect(u4FlashIdx, TRUE) != 0)
        {
            LOG(3, "Enable flash write protect fail!\n");
            return 1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_GetInfo.
 *  Details of SFLASH_GetInfo (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_EraseChip(UINT32 u4Index)
{
    INT32 i4Ret;
    
    ASSERT(u4Index < MAX_FLASHCOUNT);
    
    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    if(SFLASH_WriteProtect(u4Index, FALSE) != 0)
    {
        LOG(3, "Disable flash write protect fail!\n");
        return 1;
    }

    i4Ret = SFLASHHW_EraseChip(u4Index); 

    if(SFLASH_WriteProtect(u4Index, TRUE) != 0)
    {
        LOG(3, "Enable flash write protect fail!\n");
        return 1;
    }
    
    return i4Ret;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_WrNoE.
 *  Write data on nor flash and erase automaticallly if need
 *  @retval 0   Success
 *  @retval 1   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_WrNoE(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount)
{
    UINT8 *pu1Buf;
    UINT32 i, j, u4WriteAddr, u4Len, u4WriteByte, u4ChipOffset, u4SectStart, u4SectEnd;
    
    if(SFLASH_Init() != 0)
    {
        return 1;
    }

    ASSERT(_rSFInfo.u1FlashCount > 0);

    pu1Buf = (UINT8*)u4MemPtr;

    u4WriteAddr = u4Offset;
    u4WriteByte = u4ByteCount;
    u4ChipOffset = 0;
    u4SectStart = 0;
    u4SectEnd = 0;
    for(i=0; i<_rSFInfo.u1FlashCount; i++)
    {
        if(SFLASH_WriteProtect(i, FALSE) != 0)
        {
            LOG(3, "Disable flash write protect fail!\n");
            return 1;
        }
    
        for(j=0; j<_rSFInfo.arFlashInfo[i].u4SecCount; j++)
        {
            u4SectEnd = u4SectStart + _rSFInfo.arFlashInfo[i].u4SecSize;
            if((u4SectStart<=u4WriteAddr)&&(u4WriteAddr<u4SectEnd))
            {
                u4Len = _rSFInfo.arFlashInfo[i].u4SecSize - (u4WriteAddr - u4SectStart);
                if(u4Len >= u4WriteByte)
                {
                    u4Len = u4WriteByte;
                }

                if(u4Len < _rSFInfo.arFlashInfo[i].u4SecSize)
                {
                    if(SFLASH_Read(u4SectStart, (UINT32)_au1BackBuf, 
                                        _rSFInfo.arFlashInfo[i].u4SecSize) != 0)
                    {
                        LOG(3, "Read flash fail!\n");
                        return 1;
                    }
                    
                    x_memcpy((UINT8*)(_au1BackBuf + (u4WriteAddr - u4SectStart)), 
                             pu1Buf, u4Len);

                    if(SFLASHHW_EraseSector(i, u4SectStart - u4ChipOffset) != 0)
                    {
                        LOG(3, "Erase flash fail !\n");
                        if(SFLASH_WriteProtect(i, TRUE) != 0)
                        {
                            LOG(3, "Enable flash write protect fail!\n");
                            return 1;
                        }                        
                        return 1;
                    }

                    if(SFLASHHW_WriteSector(i, u4SectStart - u4ChipOffset, u4Len, _au1BackBuf) != 0)
                    {
                        LOG(3, "Write flash error !\n");
                        if(SFLASH_WriteProtect(i, TRUE) != 0)
                        {
                            LOG(3, "Enable flash write protect fail!\n");
                            return 1;
                        }                                                
                        return 1;
                    }                                 
                }
                else
                {
                    if(SFLASHHW_EraseSector(i, u4SectStart - u4ChipOffset) != 0)
                    {
                        LOG(3, "Erase flash fail !\n");
                        if(SFLASH_WriteProtect(i, TRUE) != 0)
                        {
                            LOG(3, "Enable flash write protect fail!\n");
                            return 1;
                        }                                                
                        return 1;
                    }

                    if(SFLASHHW_WriteSector(i, u4SectStart - u4ChipOffset, u4Len, pu1Buf) != 0)
                    {
                        LOG(3, "Write flash error !\n");
                        if(SFLASH_WriteProtect(i, TRUE) != 0)
                        {
                            LOG(3, "Enable flash write protect fail!\n");
                            return 1;
                        }                                                
                        return 1;
                    }
                }

                u4WriteAddr += u4Len;                
                u4WriteByte -= u4Len;
                pu1Buf += u4Len;

                if(u4WriteByte == 0)
                {
                    if(SFLASH_WriteProtect(i, TRUE) != 0)
                    {
                        LOG(3, "Enable flash write protect fail!\n");
                        return 1;
                    }                                        
                    return 0;
                }
            }
            u4SectStart += _rSFInfo.arFlashInfo[i].u4SecSize;
        }
        u4ChipOffset += _rSFInfo.arFlashInfo[i].u4ChipSize;
        if(SFLASH_WriteProtect(i, TRUE) != 0)
        {
            LOG(3, "Enable flash write protect fail!\n");
            return 1;
        }                                
    }

    return 1;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_CalCRC.
 *  Details of SFLASH_CalCRC (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_CalCRC(UINT32 u4Addr, UINT32 u4Len, UINT32* pu4CheckSum)
{
    if(u4Addr >= PBI_A_BASE)
    {
        u4Addr -= PBI_A_BASE;
    }

    return SFLASHHW_CalCRC(u4Addr, u4Len, pu4CheckSum);
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_EraseBlockMap.
 *  Details of SFLASH_EraseBlockMap (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_EraseBlockMap(SECTTBL_T *prBlkMap, UINT32 *pu4TotalNorSz)
{
    UINT32 i;

    if(SFLASH_Init() != 0)
    {
        return 1;
    }
    
    if(prBlkMap)
    {
        prBlkMap->u4SegNum = _rSFInfo.u1FlashCount;
        prBlkMap->prSegInfo = _arSegInfo;
    }

    if(pu4TotalNorSz)
    {
        *pu4TotalNorSz = 0;
        for(i=0; i<_rSFInfo.u1FlashCount; i++)
        {
            *pu4TotalNorSz += _rSFInfo.arFlashInfo[i].u4ChipSize;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_Calibrate.
 *  Details of SFLASH_Calibrate (optional).
 */
//-----------------------------------------------------------------------------
void SFLASH_Calibrate(UINT32 u4Addr, UINT32 u4ByteCount, const UINT8 *pu1Buf)
{
    UINT32 clk_num, readtype_num, sample_num;
    UINT32 i, j, k;
    UINT32 u4OrgClk, u4OrgSampleEdge, u4OrgReadType;

    if(pu1Buf == NULL)
    {
        LOG(1, "NULL buffer!\n");
        ASSERT(0);
    }

    clk_num = sizeof(_arClk) / sizeof(SFLASH_TESTITEM_T);
    readtype_num = sizeof(_arReadType) / sizeof(SFLASH_TESTITEM_T);
    sample_num = sizeof(_arSampleEdge) / sizeof(SFLASH_TESTITEM_T);

    u4OrgClk = SFLASHHW_GetClk();
    u4OrgSampleEdge = SFLASHHW_GetSampleEdge();
    u4OrgReadType = SFLASHHW_GetReadType();

    for(i=0; i<clk_num; i++)
    {
        for(j=0; j<readtype_num; j++)
        {
            for(k=0; k<sample_num; k++)
            {
                SFLASHHW_SetSampleEdge(_arSampleEdge[k].u4RegVal);
                SFLASHHW_SetReadType(_arReadType[j].u4RegVal);
                SFLASHHW_SetClk(_arClk[i].u4RegVal);

                x_thread_delay(500);

                if(x_memcmp(pu1Buf, (void *)(SFLASH_MEM_BASE + u4Addr), u4ByteCount) == 0)
                {
                    Printf("%s, %s, %s OK\n", 
                            _arClk[i].acName, 
                            _arReadType[j].acName, 
                            _arSampleEdge[k].acName);
                }
                else
                {
                    Printf("%s, %s, %s Fail\n", 
                            _arClk[i].acName, 
                            _arReadType[j].acName, 
                            _arSampleEdge[k].acName);
                }
            }
        }
    }

    SFLASHHW_SetSampleEdge(u4OrgSampleEdge);
    SFLASHHW_SetReadType(u4OrgReadType);
    SFLASHHW_SetClk(u4OrgClk);

    x_thread_delay(500);
}


#ifdef CC_MT5382P
//-----------------------------------------------------------------------------
/** Brief of SFLASH_EnableDMA.
 *  Details of SFLASH_EnableDMA (optional).
 *  @retval 1    Success
 *  @retval 0   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size)
{
    return SFLASHHW_EnableDMA(u4SrcAddr, u4DestAddr, u4Size);
}
#endif

//-----------------------------------------------------------------------------
/** Brief of SFLASH_WriteProtect.
 *  Details of SFLASH_WriteProtect (optional).
 *  @retval 1    Success
 *  @retval 0   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_WriteProtect(UINT32 u4Index, BOOL fgEnable)
{
    return SFLASHHW_WriteProtect(u4Index, fgEnable);
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_GetFlashName.
 *  Details of SFLASH_GetFlashName (optional).
 */
//-----------------------------------------------------------------------------
CHAR* SFLASH_GetFlashName(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2)
{
    return SFLASHHW_GetFlashName(u1MenuID, u1DevID1, u1DevID2);
}


//-----------------------------------------------------------------------------
/** Brief of SFLASH_WriteProtect.
 *  Details of SFLASH_WriteProtect (optional).
 *  @retval 1    Success
 *  @retval 0   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_ReadFlashStatus(UINT32 u4Index, UINT8* pu1Status)
{
    return SFLASHHW_ReadFlashStatus(u4Index, pu1Status);
}



/*****************************************************************************/
/*****************************************************************************/
#if 0
static REG_TEST_T _arSFLASHRgtList[] = {
    // bsp will set it as 0x00c3c3c3 from 0x00000001
    { 0x0028, eRD_WR, 4, 0x00cfcfdf, 1, 0x00c3c3c3 },
    // End.
    { 0xffff, eNO_TYPE, -1, 0, 0, 0 },
};
#endif

//-----------------------------------------------------------------------------
/** Brief of SFLASH_Diag.
 *  Details of SFLASHHW_Diag (optional).
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
INT32 SFLASH_Diag()
{
#if 0
    UINT32 u4ChipNum, u4TotalBytes;
    INT32 i4Ret, i4Val;

    i4Val = 0;
    i4Ret = 0;
    // i4Ret = UTIL_RegDefChk(NOR_BASE, _arNORRgtList);
    // i4Val |= i4Ret;
    Printf("Register default value check .............. %s\n",
                    (i4Ret ? "FAIL" : "PASS"));
    i4Ret = UTIL_RegRWTest(BIM_BASE, _arSFLASHRgtList);
    i4Val |= i4Ret;
    Printf("Register read/write test .................. %s\n",
                    (i4Ret ? "FAIL" : "PASS"));
    // i4Ret = UTIL_AllSpaceRWTest(BIM_BASE, NOR_REG_LENGTH);
    // i4Val |= i4Ret;
    // Printf("Memory space read/write test .............. %s\n",
    //              (i4Ret ? "FAIL" : "PASS"));
    Printf("Memory space read/write test .............. %s\n", "SKIP");
#if 0    
    u4ChipNum = 0;
    i4Ret = NORHW_Identify(&u4ChipNum, &u4TotalBytes);
    if (u4ChipNum==0)
    {
        i4Ret = 1;
    }
    i4Val |= i4Ret;
    Printf("NOR Flash self checking ................... %s\n",
                    (i4Ret ? "FAIL" : "PASS"));
#endif
    return i4Val;
#endif
    return 0;
}


