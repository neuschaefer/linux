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

/** @file nor_part.c
 *  nor_part.c provides functions of norflash physical/virutal partitions.
 *  In nor_part.c, there are functions to calculate norflash physical/virtual
 *  partitions, such as  size, offset, sector map, and other necessary
 *  functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifndef CC_5391_LOADER

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "drvcust_if.h"
#include "nor_if.h"
#include "storage_if.h"
#include "x_pinmux.h"
#include "nor_debug.h"
LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define NORPART_DEBUG

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
// Define block header.
#define BH_SIZE  7                        // bytes.
#define BH_SECSIG        ((UINT16)0xC5A3) // security signature.

// block header query.
#define BH_GET_BEC(ptr)         ( *((UINT32*)ptr) & (UINT32)0x1FFFF )
#define BH_GET_BIU(ptr)         (( *((UINT8*)ptr + 2) & (UINT8)0x02) == 0 )
#define BH_GET_RFE(ptr)         (( *((UINT8*)ptr + 2) & (UINT8)0x10) == 0 )
#define BH_GET_GCA(ptr)         (( *((UINT8*)ptr + 2) & (UINT8)0x20) == 0 )
#define BH_GET_GCC(ptr)         (( *((UINT8*)ptr + 2) & (UINT8)0x40) == 0 )
#define BH_GET_LBN(ptr)         ( *((UINT8*)ptr + 3) & (UINT8)0xFF )
#define BH_GET_SEC(ptr)         ( *((UINT16*)ptr + 2) & (UINT16)0xFFFF )
#define BH_GET_PAN(ptr)         ( *((UINT8*)ptr + 6) & (UINT8)0xFF ) // partition number.
// block header modify.
#define BH_SET_BEC(ptr, bec)   {*((UINT16*)ptr) = ((UINT16)0xFFFF & bec ); \
                                                    if (!((UINT32)0x10000 & bec)) { \
                                                    *((UINT16*)ptr + 1) &= (UINT16)0xFFFE;}}

#define BH_SET_BIU(ptr)          (*((UINT8*)ptr + 2) &= ~((UINT8)0x02))
#define BH_SET_RFE(ptr)          (*((UINT8*)ptr + 2) &= ~((UINT8)0x10))
#define BH_SET_GCA(ptr)         (*((UINT8*)ptr + 2) &= ~((UINT8)0x20))
#define BH_SET_GCC(ptr)         (*((UINT8*)ptr + 2) &= ~((UINT8)0x40))
#define BH_SET_LBN(ptr, lbn)   (*((UINT8*)ptr + 3) = ((UINT8)0xFF & lbn))
#define BH_SET_SEC(ptr, sec)   (*((UINT16*)ptr + 2) = ((UINT16)0xFFFF & sec))
#define BH_SET_PAN(ptr, pan)  (*((UINT8*)ptr + 6) = ((UINT8)0xFF & pan))

// block header update to flash.
#define BH_UPDATE(ptr, offset)  VERIFY(0 == NOR_Write((UINT64)offset, (UINT32)ptr, BH_SIZE))

#define BH_MAX_BEC                 ((UINT32)0x20000)

/** Constant size definitions.
 *
 */
#define SZ_32MB              (32*1024*1024)
#define SZ_31MB              (31*1024*1024) 

#define SZ_30MB              (30*1024*1024)
#define SZ_29MB              (29*1024*1024) 
#define SZ_28MB              (28*1024*1024) 
#define SZ_27MB              (27*1024*1024)
#define SZ_26MB              (26*1024*1024) 
#define SZ_25MB              (25*1024*1024) 
#define SZ_24MB              (24*1024*1024)
#define SZ_23MB              (23*1024*1024)
#define SZ_22MB              (22*1024*1024) 
#define SZ_21MB              (21*1024*1024)

#define SZ_20MB              (20*1024*1024) 
#define SZ_19MB              (19*1024*1024) 
#define SZ_18MB              (18*1024*1024)
#define SZ_17MB              (17*1024*1024) 
#define SZ_16MB              (16*1024*1024) 
#define SZ_15MB              (15*1024*1024)
#define SZ_14MB              (14*1024*1024)
#define SZ_13MB              (13*1024*1024) 
#define SZ_12MB              (12*1024*1024)
#define SZ_11MB              (11*1024*1024) 

#define SZ_10MB              (10*1024*1024) 
#define SZ_9MB              (9*1024*1024)
#define SZ_8MB              (8*1024*1024)
#define SZ_7MB              (7*1024*1024)
#define SZ_6MB              (6*1024*1024)
#define SZ_5MB              (5*1024*1024)
#define SZ_4MB              (4*1024*1024)
#define SZ_3MB              (3*1024*1024)
#define SZ_2MB              (2*1024*1024)
#define SZ_1MB              (1*1024*1024)

#define SZ_512KB            (512*1024)
#define SZ_256KB            (256*1024)
#define SZ_128KB            (128*1024)
#define SZ_64KB             (64*1024)
#define SZ_40KB             (40*1024)
#define SZ_32KB             (32*1024)
#define SZ_16KB             (16*1024)
#define SZ_1KB                 (1*1024)


#define SZ_PART_DATA              ((SZ_64KB) - (BH_SIZE))
#define GC_BLK_NUM                  (0)

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef struct _NOR_BLK_T
{
    UINT32 fgInitial;
    /* register partition type: static or dynamic type */
    UINT32 au4PartType[PART_MIDD_END]; // dynamic type or static type.

    UINT32 u4PartSize[PART_MIDD_END];

    /* static partition information.*/
    UINT32 u4StaticPartStartAddr[PART_MIDD_END];

    /* dynamic partition information.*/
    UINT32 u4BlkStartAddr;
    UINT32 u4BlkNum;
    UINT32 u4MaxBEC;
    UINT32* pu4LBA;    //< store physical block address.
    UINT8 au1BH[BH_SIZE];
    UINT8 au1PartMap[PART_MIDD_END]; //< store logical block number.
} NOR_BLK_T;

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static NOR_BLK_T _rNorBlk;

static BOOL _fgNORParamInit;

static UINT32 _u4SZ_NOR_TOTAL_SIZE;
static UINT32 _u4SZ_NOR_LOADER_SIZE;
static UINT32 _u4SZ_NOR_APPIMAGE_SIZE;
static UINT32 _u4SZ_NOR_PARTITION_NUM;
// dynamic partiiton start address and size.
static UINT32 _u4SZ_NOR_DYNAMIC_START;
static UINT32 _u4SZ_NOR_DYNAMIC_SIZE;
// static partiiton start and size.
static UINT32 _u4SZ_NOR_STATIC_START;
static UINT32 _u4SZ_NOR_STATIC_SIZE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _Norpart_EraseHWBlk
*  Erase the whole 64KB h/w physical block.
*  @param  u4Offset    block start offset.
*  @return  void.
*/
//-------------------------------------------------------------------------
static void _Norpart_EraseHWBlk(UINT32 u4Offset)
{
    UINT32 i;
    UINT32 u4Size = 0;
    SECTTBL_T rBlkMap;
    UINT32 u4SectSize;

    // make sure it is on 64 KB boundary.
    ASSERT((u4Offset % SZ_64KB) == 0);

    // Query flash map.
    VERIFY(0 == NOR_EraseMap(&rBlkMap));

    // Get the start address.
    for (i = 0; i < rBlkMap.u4SegNum; i++)
    {
        u4Size += rBlkMap.prSegInfo[i].u4SectNum * rBlkMap.prSegInfo[i].u4SectSize;
        if (u4Size > u4Offset)
        {
            break;
        }
    }
    u4SectSize = rBlkMap.prSegInfo[i].u4SectSize;

    ASSERT(u4SectSize <= SZ_64KB);
    
    if (u4SectSize == SZ_64KB)
    {
        // erase block.      
        VERIFY(0 == NOR_EraseBlk(u4Offset));        
    }
    else 
    {
        // handle sector size < 64KB case.
        for (u4Size = 0; u4Size < SZ_64KB; u4Size+= u4SectSize)
        {
            // erase block.      
            VERIFY(0 == NOR_EraseBlk(u4Offset + u4Size));                
        }
    }
}
//-------------------------------------------------------------------------
/** _Norpart_EraseBlk
*  Erase the selected logical block.
*  @param  u4Lbn    the selected logical block.
*  @return  void.
*/
//-------------------------------------------------------------------------
static void _Norpart_EraseBlk(UINT32 u4Lbn)
{
    UINT32 u4Offset;

    UINT32 u4Bec;

    ASSERT(u4Lbn < _rNorBlk.u4BlkNum);
    // get physical block.
    u4Offset = _rNorBlk.pu4LBA[u4Lbn];

    // read block header.
    VERIFY(0 == NOR_Read((UINT64)u4Offset, (UINT32)_rNorBlk.au1BH, BH_SIZE));

    // make sure block is ready for earse.
    ASSERT(BH_GET_RFE(_rNorBlk.au1BH) && BH_GET_BIU(_rNorBlk.au1BH));

    u4Bec = BH_GET_BEC(_rNorBlk.au1BH);
    u4Bec++;
    if (u4Bec >= BH_MAX_BEC)
    {
        // This block is erased more than 100 thousand times.
        // Amazing....We also don't know what will happen then.
        // God bless this super user.
        u4Bec = 0;
    }

    // erase block.      
    _Norpart_EraseHWBlk(u4Offset);

    x_memset((VOID *)_rNorBlk.au1BH, 0xFF, BH_SIZE);
    // update block header.
    BH_SET_LBN(_rNorBlk.au1BH, u4Lbn);
    BH_SET_BEC(_rNorBlk.au1BH, u4Bec);
    BH_SET_SEC(_rNorBlk.au1BH, BH_SECSIG);
    BH_UPDATE(_rNorBlk.au1BH, u4Offset);
}

//-------------------------------------------------------------------------
/** _Norpart_CheckBEC
*  Check and initial system block information at system boot up.
*  @param  void
*  @return  void.
*/
//-------------------------------------------------------------------------
static void _Norpart_CheckBEC(void)
{
    UINT32 i;

    UINT32 u4Offset = _rNorBlk.u4BlkStartAddr;
    UINT32 u4Bec;
    UINT32 u4Lbn;
    UINT32 u4Pan;
    UINT32 u4Nof_Bec = 0;
    UINT32 u4EraseOffset = 0xFFFFFFFF;

    // Get Max BEC.
    _rNorBlk.u4MaxBEC = 0;

    for (i = 0; i < _rNorBlk.u4BlkNum; i++)
    {
        // read block header.
        VERIFY(0 == NOR_Read((UINT64)u4Offset, (UINT32)_rNorBlk.au1BH, BH_SIZE));

        u4Bec = BH_GET_BEC(_rNorBlk.au1BH);
        #ifdef NORPART_DEBUG
        LOG(7, "[%02d] BEC=%d, PAN=%03d, REF=%d, BIU=%d.\n", i, BH_GET_BEC(_rNorBlk.au1BH),
               BH_GET_PAN(_rNorBlk.au1BH), BH_GET_RFE(_rNorBlk.au1BH), BH_GET_BIU(_rNorBlk.au1BH));
        #endif

        u4Lbn = BH_GET_LBN(_rNorBlk.au1BH);
        u4Pan = BH_GET_PAN(_rNorBlk.au1BH);
        
        // check block header.
        if ((!BH_GET_RFE(_rNorBlk.au1BH)) && 
            (u4Bec < BH_MAX_BEC) && 
            (BH_GET_SEC(_rNorBlk.au1BH) == BH_SECSIG) &&
            (u4Lbn < _rNorBlk.u4BlkNum))
        {
            if (u4Bec > _rNorBlk.u4MaxBEC)
            {
                _rNorBlk.u4MaxBEC = u4Bec;
            }

            // check partition is all dynamic. prepare to erase static type partition.
            if ((u4Pan == 0xFF) ||
               ((u4Pan < _rNorBlk.u4BlkNum) && (_rNorBlk.au4PartType[u4Pan] != NORPART_STATIC_TYPE)))
            {
                u4Nof_Bec++;
            }        
            else
            {
                u4EraseOffset = u4Offset;
            }
        }
        else
        {
            // prepare to be erase.
            u4EraseOffset = u4Offset;
        }
        u4Offset += SZ_64KB;
    }

    u4Offset = _rNorBlk.u4BlkStartAddr;

    // flash is in virgin status.
    if (u4Nof_Bec < (_rNorBlk.u4BlkNum - 1))
    {
        for (i = 0; i < _rNorBlk.u4BlkNum; i++)
        {
            _rNorBlk.u4MaxBEC = 0;
            // invalid block. reinitialize.
            _Norpart_EraseHWBlk(u4Offset);

            VERIFY(NULL != x_memset((VOID *)_rNorBlk.au1BH, 0xFF, BH_SIZE));

            BH_SET_BEC(_rNorBlk.au1BH, _rNorBlk.u4MaxBEC);
            BH_SET_SEC(_rNorBlk.au1BH, BH_SECSIG);
            // default LBN.
            BH_SET_LBN(_rNorBlk.au1BH, i);

            // update block header to flash.
            BH_UPDATE(_rNorBlk.au1BH, u4Offset);
            u4Offset += SZ_64KB;
        }
    }
    else if (u4Nof_Bec < _rNorBlk.u4BlkNum)
    {
        // erase the corrupt block but fill block header later.
        _Norpart_EraseHWBlk(u4EraseOffset);
    }

    u4Offset = _rNorBlk.u4BlkStartAddr;

    x_memset((VOID *)_rNorBlk.pu4LBA, 0xFF, _rNorBlk.u4BlkNum *sizeof(UINT32));
    u4EraseOffset = 0xFFFFFFFF;

    // construct logical to physical block mapping.
    for (i = 0; i < _rNorBlk.u4BlkNum; i++)
    {
        // read block header.
        VERIFY(0 == NOR_Read((UINT64)u4Offset, (UINT32)_rNorBlk.au1BH, BH_SIZE));

        u4Lbn = BH_GET_LBN(_rNorBlk.au1BH);

        // check block header.
        if (u4Lbn == 0xFF)
        {
            // corrupt block address.
            u4EraseOffset = u4Offset;
        }
        else
        {
            ASSERT(_rNorBlk.pu4LBA[u4Lbn] == 0xFFFFFFFF);

            // normal block.
            _rNorBlk.pu4LBA[u4Lbn] = u4Offset;
        }
        u4Offset += SZ_64KB;
    }

    // Browse all logical block, search the corrupt block and fill the mapping LBA.
    for (i = 0; i < _rNorBlk.u4BlkNum; i++)
    {
        if (_rNorBlk.pu4LBA[i] == 0xFFFFFFFF)
        {
            ASSERT(u4EraseOffset != 0xFFFFFFFF);

            // write block header.
            VERIFY(NULL != x_memset((VOID *)_rNorBlk.au1BH, 0xFF, BH_SIZE));
            // use Max BEC to be as the BEC of this corrupt block.
            BH_SET_BEC(_rNorBlk.au1BH, _rNorBlk.u4MaxBEC);
            BH_SET_SEC(_rNorBlk.au1BH, BH_SECSIG);
            BH_SET_LBN(_rNorBlk.au1BH, i);
            BH_UPDATE(_rNorBlk.au1BH, u4EraseOffset);

            _rNorBlk.pu4LBA[i] = u4EraseOffset;
        }
    }
}

//-------------------------------------------------------------------------
/** _Norpart_GetFreeBlk
*  Get free block with lowest BEC.
*  @param  void
*  @return  the block with lowest BEC.
*/
//-------------------------------------------------------------------------
static UINT32 _Norpart_GetFreeBlk(void)
{
    UINT32 i;

    UINT32 u4Offset;
    UINT32 u4Bec = 0xFFFFFFFF;
    UINT32 u4Bec1 = 0xFFFFFFFF;
    UINT32 u4BlkToUse = 0xFFFFFFFF;
    UINT32 u4BlkToErase = 0xFFFFFFFF;

    // search blocks for partition.
    for (i = 0; i < _rNorBlk.u4BlkNum; i++)
    {
        // search logical block.
        u4Offset = _rNorBlk.pu4LBA[i];

        // read block header.
        VERIFY(0 == NOR_Read((UINT64)u4Offset, (UINT32)_rNorBlk.au1BH, BH_SIZE));
        #ifdef NORPART_DEBUG
        LOG(7, "[%02d] BEC=%d, PAN=%03d, REF=%d, BIU=%d.\n", i, BH_GET_BEC(_rNorBlk.au1BH),
               BH_GET_PAN(_rNorBlk.au1BH), BH_GET_RFE(_rNorBlk.au1BH), BH_GET_BIU(_rNorBlk.au1BH));
        #endif
        // Get the block is not used, not ready for erase and lowest BEC.
        if ((!BH_GET_BIU(_rNorBlk.au1BH)) && 
            (!BH_GET_RFE(_rNorBlk.au1BH)) && 
            (BH_GET_BEC(_rNorBlk.au1BH) < u4Bec))
        {
            u4BlkToUse = i;
            u4Bec = BH_GET_BEC(_rNorBlk.au1BH);
        }

        // search the REF and BIU block with lowest bec at the same time.
        if ((BH_GET_BIU(_rNorBlk.au1BH)) && 
            (BH_GET_RFE(_rNorBlk.au1BH)) && 
            (BH_GET_BEC(_rNorBlk.au1BH) < u4Bec1))
        {
            u4BlkToErase = i;
            u4Bec1 = BH_GET_BEC(_rNorBlk.au1BH);
        }
    }

    // no block is free. Erase the lowest BEC block
    if (u4BlkToUse == 0xFFFFFFFF)
    {
        ASSERT(u4BlkToErase != 0xFFFFFFFF);

        _Norpart_EraseBlk(u4BlkToErase);
        u4BlkToUse = u4BlkToErase;
    }

    ASSERT(u4BlkToUse < _rNorBlk.u4BlkNum);

    return u4BlkToUse;
}

//-------------------------------------------------------------------------
/** _Norpart_CheckDynamicPartition
*  Check the system partition at system boot up.
*  @param  void
*  @return  void.
*/
//-------------------------------------------------------------------------
static void _Norpart_CheckDynamicPartition(void)
{
    UINT32 i;

    UINT32 u4Offset;
    UINT32 u4Pan;
    UINT32 u4NofPan = 0;
    UINT32 u4FreeBlk;

    // Reset partition map to 0xFF as initial value.
    VERIFY(NULL != x_memset((VOID *)_rNorBlk.au1PartMap, 0xFF, PART_MIDD_END));

    // search blocks for partition.
    for (i = 0; i < _rNorBlk.u4BlkNum; i++)
    {
        // search logical block.
        u4Offset = _rNorBlk.pu4LBA[i];

        // read block header.
        VERIFY(0 == NOR_Read((UINT64)u4Offset, (UINT32)_rNorBlk.au1BH, BH_SIZE));
        u4Pan = BH_GET_PAN(_rNorBlk.au1BH);

        // Check not ready for erase block and with correct partition number.
        if ((!BH_GET_RFE(_rNorBlk.au1BH)) && (u4Pan < (UINT32)PART_MIDD_END))
        {
            ASSERT(_rNorBlk.au1PartMap[u4Pan] == 0xFF);

            // partition mapping to block number.
            _rNorBlk.au1PartMap[u4Pan] = i;

            // check all partition is found.
            u4NofPan++;
            if (u4NofPan == (UINT32)PART_MIDD_END)
            {
                return;
            }
        }
    }

    // assign a logical block number to empty partition.
    for (i = PART_MIDD_0; i < (UINT32)PART_MIDD_END; i++)
    {
        if ((_rNorBlk.au4PartType[i] == NORPART_DYNAMIC_TYPE) && 
            (_rNorBlk.au1PartMap[i] == 0xFF))
        {
            // Get free logical block with lowest BEC.
            u4FreeBlk = _Norpart_GetFreeBlk();
            ASSERT(u4FreeBlk < _rNorBlk.u4BlkNum);
            _rNorBlk.au1PartMap[i] = u4FreeBlk;
            u4Offset = _rNorBlk.pu4LBA[u4FreeBlk];
            // read block header.
            VERIFY(0 == NOR_Read((UINT64)u4Offset, (UINT32)_rNorBlk.au1BH, BH_SIZE));
            // write partition number to block header.
            BH_SET_PAN(_rNorBlk.au1BH, i);
            BH_SET_BIU(_rNorBlk.au1BH);
            // update block header to flash.
            BH_UPDATE(_rNorBlk.au1BH, u4Offset);
        }
    }
}

//-------------------------------------------------------------------------
/** _Norpart_CheckStaticPartition
*  Check the system static partition at system boot up.
*  @param  void
*  @return  void.
*/
//-------------------------------------------------------------------------
static void _Norpart_CheckStaticPartition(void)
{
    UINT32 i;
    UINT32 u4Addr = _u4SZ_NOR_STATIC_START;

    // assign a logical block number to empty partition.
    for (i = PART_MIDD_2; i < (UINT32)PART_MIDD_END; i++)
    {
        if (_rNorBlk.au4PartType[i] == NORPART_STATIC_TYPE)
        {
            _rNorBlk.u4StaticPartStartAddr[i] = u4Addr;
            u4Addr += _rNorBlk.u4PartSize[i];
            if (u4Addr > (_u4SZ_NOR_STATIC_START + _u4SZ_NOR_STATIC_SIZE))
            {
#ifdef NORPART_DEBUG
                LOG(1, "static partition size config error !\n");
#endif
                ASSERT(0);
            }
        }
    }
}

//-------------------------------------------------------------------------
/** _Norpart_ParameterInit
*  NOR parameter initialization function.
*  @param  void.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
static INT32 _Norpart_ParameterInit(void)
{
    UINT32 u4Spaceleft;
    UINT32 i;
    UINT32 u4QryType;
    UINT32 u4StaticSize = 0;
    UINT32 u4DynamicSize = 0;

    if (_fgNORParamInit)
    {
        return 0;
    }
    _fgNORParamInit = TRUE;
    
    _u4SZ_NOR_TOTAL_SIZE = DRVCUST_OptGet(eNorFlashTotalSize);     
    _u4SZ_NOR_LOADER_SIZE = DRVCUST_OptGet(eNorFlashLoaderSize);         
    _u4SZ_NOR_APPIMAGE_SIZE= DRVCUST_OptGet(eNorFlashAppImageSize);     
    _u4SZ_NOR_DYNAMIC_START = _u4SZ_NOR_LOADER_SIZE + _u4SZ_NOR_APPIMAGE_SIZE;
    ASSERT(_u4SZ_NOR_TOTAL_SIZE > _u4SZ_NOR_DYNAMIC_START);

    // set user partition space.
    u4Spaceleft = _u4SZ_NOR_TOTAL_SIZE - _u4SZ_NOR_DYNAMIC_START; 
    
    _u4SZ_NOR_DYNAMIC_SIZE = DRVCUST_OptGet(eNorFlashDynamicPartitionSize);
    _u4SZ_NOR_DYNAMIC_SIZE = MIN(u4Spaceleft, _u4SZ_NOR_DYNAMIC_SIZE);
    
    ASSERT(_u4SZ_NOR_TOTAL_SIZE >= 
        (_u4SZ_NOR_DYNAMIC_START + _u4SZ_NOR_DYNAMIC_SIZE));

    // set static partition space.
    _u4SZ_NOR_STATIC_START = 
        _u4SZ_NOR_DYNAMIC_START + _u4SZ_NOR_DYNAMIC_SIZE;
    _u4SZ_NOR_STATIC_SIZE = DRVCUST_OptGet(eNorFlashStaticPartitionSize);

    if (_u4SZ_NOR_TOTAL_SIZE <
        (_u4SZ_NOR_STATIC_START + _u4SZ_NOR_STATIC_SIZE))
    {
#ifdef NORPART_DEBUG
                LOG(1, "static partition size config over flash size !\n");
#endif    
        ASSERT(0);
    }

    // set part 0 as loader 
    _rNorBlk.u4PartSize[0] = _u4SZ_NOR_LOADER_SIZE;
    _rNorBlk.au4PartType[0] = NORPART_STATIC_TYPE; 
    _rNorBlk.u4StaticPartStartAddr[0] = 0;

    // set part 1 as image app 
    _rNorBlk.u4PartSize[1] = _u4SZ_NOR_APPIMAGE_SIZE;
    _rNorBlk.au4PartType[1] = NORPART_STATIC_TYPE;    
    _rNorBlk.u4StaticPartStartAddr[1] = _rNorBlk.u4StaticPartStartAddr[0] + _rNorBlk.u4PartSize[0];

    // get partition size and determine partition type.
    _u4SZ_NOR_PARTITION_NUM = 0;
    u4QryType = (UINT32)eNorFlashPartSize2;
    for (i = (UINT32)PART_MIDD_2; i < (UINT32)PART_MIDD_END; i++)
    {
        _rNorBlk.u4PartSize[i] = DRVCUST_OptGet((QUERY_TYPE_T)u4QryType++);
        if (_rNorBlk.u4PartSize[i] > SZ_64KB)
        {
            u4StaticSize += _rNorBlk.u4PartSize[i];
            _rNorBlk.au4PartType[i] = NORPART_STATIC_TYPE;    
            _u4SZ_NOR_PARTITION_NUM ++;
        }
        else if (_rNorBlk.u4PartSize[i] == SZ_64KB)
        {
            // NOTICE: when define dynamic size == 0.
            // all partitions are static partition type.
            if (_u4SZ_NOR_DYNAMIC_SIZE > 0)
            {
                _rNorBlk.u4PartSize[i] -= BH_SIZE;
                u4DynamicSize += _rNorBlk.u4PartSize[i];
                _rNorBlk.au4PartType[i] = NORPART_DYNAMIC_TYPE;        
            }
            else
            {
                u4StaticSize += _rNorBlk.u4PartSize[i];
                _rNorBlk.au4PartType[i] = NORPART_STATIC_TYPE;    
            }
            _u4SZ_NOR_PARTITION_NUM ++;
        }
        else
        {
            _rNorBlk.u4PartSize[i] = 0;
            _rNorBlk.au4PartType[i] = NORPART_UNUSED_TYPE;        
        }
    }   

    ASSERT(u4StaticSize <= _u4SZ_NOR_STATIC_SIZE);
    ASSERT(u4DynamicSize <= _u4SZ_NOR_DYNAMIC_SIZE);
    UNUSED(u4StaticSize);
    UNUSED(u4DynamicSize);    
    return 0;
}
//-------------------------------------------------------------------------
/** _Norpart_PartInit
*  NOR partition initialization function.
*  @param  void.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
static INT32 _Norpart_PartInit(void)
{
    UINT32 i;

    UINT32 u4Size = 0;
    UINT32 u4TotalSize = 0;
    SECTTBL_T rBlkMap;

    if (!_rNorBlk.fgInitial)
    {
        UNUSED(_Norpart_ParameterInit());

        // Query flash map.
        VERIFY(0 == NOR_EraseMap(&rBlkMap));

        // check flash total size.
        for (i = 0; i < rBlkMap.u4SegNum; i++)
        {
            u4TotalSize += rBlkMap.prSegInfo[i].u4SectNum * rBlkMap.prSegInfo[i].u4SectSize;
        }

        ASSERT(u4TotalSize >= _u4SZ_NOR_TOTAL_SIZE);
        UNUSED(u4TotalSize);
        
        if (_u4SZ_NOR_DYNAMIC_SIZE > 0)
        {
            // Get the start address.
            for (i = 0; i < rBlkMap.u4SegNum; i++)
            {
                u4Size += rBlkMap.prSegInfo[i].u4SectNum * rBlkMap.prSegInfo[i].u4SectSize;
                if (u4Size > _u4SZ_NOR_DYNAMIC_START)
                {
                    break;
                }
            }

            // nor partition must use 64KB block.
            ASSERT(rBlkMap.prSegInfo[i].u4SectSize == SZ_64KB);

            // initial block information.
            _rNorBlk.u4BlkNum = ((u4Size - _u4SZ_NOR_DYNAMIC_START) >= _u4SZ_NOR_DYNAMIC_SIZE)
                                    ? (_u4SZ_NOR_DYNAMIC_SIZE /SZ_64KB)
                                    : ((u4Size - _u4SZ_NOR_DYNAMIC_START) /(SZ_64KB));

            // s/w partition number <= h/w partition number.
            if (_u4SZ_NOR_PARTITION_NUM > _rNorBlk.u4BlkNum)
            {
#ifdef NORPART_DEBUG
                LOG(1, "config error !.\n");
#endif
                ASSERT(0);
            }

            _rNorBlk.u4BlkStartAddr = _u4SZ_NOR_DYNAMIC_START;

            _rNorBlk.pu4LBA = (UINT32 *)x_mem_alloc(_rNorBlk.u4BlkNum *sizeof(UINT32 *));
            ASSERT(_rNorBlk.pu4LBA);

            // check blocks and handle incomplete block initialize.    
            _Norpart_CheckBEC();

            // check dynamic partition and initialize partition mapping table.
            _Norpart_CheckDynamicPartition();
        }

        // check static partition and initialize partition mapping table.
        _Norpart_CheckStaticPartition();

        _rNorBlk.fgInitial = TRUE;
    }

    return 0;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** NORPART_PartStartOffset
*  Return the physical address offset of the selected partition.
*  @param  u4Pid      selected partition.
*  @return  the physical address offset of the selected partition.
*/
//-------------------------------------------------------------------------
UINT32 NORPART_PartStartOffset(UINT32 u4Pid)
{
    UINT32 u4LBN;

    ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    // initial nor part 
    VERIFY(_Norpart_PartInit() == 0);
    ASSERT(_rNorBlk.au4PartType[u4Pid] <= NORPART_STATIC_TYPE);
    
    if (_rNorBlk.au4PartType[u4Pid] == NORPART_STATIC_TYPE)
    {
        return _rNorBlk.u4StaticPartStartAddr[u4Pid];
    }
    else
    {
        u4LBN = _rNorBlk.au1PartMap[u4Pid];

        ASSERT(u4LBN < _rNorBlk.u4BlkNum);

        // begin at (block base address + block header size).
        return (_rNorBlk.pu4LBA[u4LBN] + BH_SIZE);
    }        
}

//-------------------------------------------------------------------------
/** NORPART_GetMWPartNum
*  Return the partition numbers of Middleware. This function is call by char_mw.c only.
*  @param  void.
*  @return  total partition number in system.
*/
//-------------------------------------------------------------------------
INT32 NORPART_GetMWPartNum(void)
{
    // number of middleware partition.
    return (INT32)PART_MIDD_END;
}

//-------------------------------------------------------------------------
/** NORPART_GetDRVPartNum
*  Return the partition numbers of Driver. This function is call by driver only.
*  @param  void.
*  @return  total partition number in system.
*/
//-------------------------------------------------------------------------
INT32 NORPART_GetDRVPartNum(void)
{
    // number of driver partition.
    return (INT32)(PART_DRV_END - PART_MIDD_END);
}

//-------------------------------------------------------------------------
/** NORPART_GetPartSectorNum
*  Return the erase sector maps of the selected partition.
*  @param  pt_sector_tbl    pointer to sector table information.
*  @param  u4Pid      selected partition.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NORPART_GetSectorTbl(SECTTBL_T *pt_sector_tbl, UINT32 u4Pid)
{
    UINT32 u4Size;
    
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    ASSERT(pt_sector_tbl != NULL);

    if (pt_sector_tbl->u4SegNum < 1)
    {
        return (INT32)1;
    }
    
    pt_sector_tbl->u4SegNum = 1;
    u4Size = NORPART_GetPartSize(u4Pid);
    pt_sector_tbl->prSegInfo[0].u4SectNum = (u4Size + (SZ_64KB -1))/SZ_64KB;
    pt_sector_tbl->prSegInfo[0].u4SectSize = MIN(u4Size, SZ_64KB);

    return 0;
}

//-------------------------------------------------------------------------
/** NORPART_GetPartSectorNum
*  Return the number of sectors in selected partition.
*  @param  u4Pid      selected partition.
*  @return   the number of sectors in selected partition.
*/
//-------------------------------------------------------------------------
UINT32 NORPART_GetPartSectorNum(UINT32 u4Pid)
{
    UINT32 u4Size;

    ASSERT(u4Pid < (UINT32)PART_MIDD_END);

    u4Size = NORPART_GetPartSize(u4Pid);

    return ((u4Size + (SZ_64KB -1))/SZ_64KB);
}

//-------------------------------------------------------------------------
/** NORPART_GetPartSize
*  This function returns the size of the selected partition in bytes..
*  @param  u4Pid      selected partition.
*  @return   the size of selected partition.
*/
//-------------------------------------------------------------------------
UINT32 NORPART_GetPartSize(UINT32 u4Pid)
{
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);

    // initial nor part parameter.
    UNUSED(_Norpart_ParameterInit());

    if (_rNorBlk.au4PartType[u4Pid] <= NORPART_STATIC_TYPE)
    {
        return _rNorBlk.u4PartSize[u4Pid];
    }
    else
    {
        return 0;
    }
}

//-------------------------------------------------------------------------
/** NORPART_Read
*  NORPART_Read() to read data to nor flash.
*  @param  u8Offset         Lower 32 bits: offset to read data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's read buffer address.
*  @param  u4MemLen      read length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NORPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT16 u2PartId;

    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    INT32 i4Ret;

    // initial nor part 
    VERIFY(_Norpart_PartInit() == 0);

    // the high 32 bits as partition id.
    u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    ASSERT(u2PartId < (UINT16)PART_MIDD_END);
    ASSERT((u4Offset+u4MemLen) <= NORPART_GetPartSize(u2PartId));
    #ifdef NORPART_DEBUG
    LOG(7, "NOR read PID=%d, offset=%d.\n", u2PartId, u4Offset);
    #endif

    ASSERT(_rNorBlk.fgInitial != 0);

    // get physical address by partition id.
    u4Offset += NORPART_PartStartOffset(u2PartId);

    i4Ret = NOR_Read((UINT64)u4Offset, u4MemPtr, u4MemLen);
    ASSERT(i4Ret == 0);
    return i4Ret;
}

//-------------------------------------------------------------------------
/** NORPART_Write
*  NORPART_Write() to write data to nor flash.
*  @param  u8Offset         Lower 32 bits: offset to write data. Higher 32 bits: partition ID.
*  @param  u4MemPtr      User's write buffer address.
*  @param  u4MemLen      Write length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NORPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT16 u2PartId;

    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    INT32 i4Ret;

    // initial nor part 
    VERIFY(_Norpart_PartInit() == 0);
    
    // the high 32 bits as partition id.
    u2PartId = (u8Offset >> (UINT64)32) & 0xFFFF;
    ASSERT(u2PartId < (UINT16)PART_MIDD_END);
    ASSERT((u4Offset+u4MemLen) <= NORPART_GetPartSize(u2PartId));
    #ifdef NORPART_DEBUG
    LOG(7, "NOR write PID=%d, offset=%d.\n", u2PartId, u4Offset);
    #endif

    ASSERT(_rNorBlk.fgInitial != 0);

    // get physical address by partition id.
    u4Offset += NORPART_PartStartOffset(u2PartId);
    i4Ret = NOR_Write((UINT64)u4Offset, u4MemPtr, u4MemLen);
    ASSERT(i4Ret == 0);
    return i4Ret;
}
//-------------------------------------------------------------------------
/** NORPART_Erase
 *  Erase selected partition, and allocate new physical block to this partition.
 *  @param  u8Offset        Selected partition and block start index to be erased.
 *  @param  u4SectNum    sector number. 
 *  @retval 0	        Success
 *  @retval Others	Fail
 */
//-------------------------------------------------------------------------
INT32 NORPART_Erase(UINT64 u8Offset, UINT32 u4SectNum)
{
    UINT32 u4OldOffset;

    UINT32 u4NewOffset;
    UINT32 u4Pan;
    UINT32 u4OldLbn;
    UINT32 u4FreeBlk;
    UINT32 u4Pid;
    UINT32 u4StartBlkIdx;
    UINT32 i;

    // initial nor part 
    VERIFY(_Norpart_PartInit() == 0);
    
    // the high 32 bits as partition id.
    u4StartBlkIdx = (UINT32)(u8Offset & 0xFFFFFFFF);
    u4Pid = (u8Offset >> (UINT64)32) & 0xFFFF;
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);

    if (_rNorBlk.au4PartType[u4Pid] == NORPART_UNUSED_TYPE)
    {
        return 0;
    }

    // erase static partition.
    if (_rNorBlk.au4PartType[u4Pid] == NORPART_STATIC_TYPE)
    {
        i = 0;
        while (i < u4SectNum)
        {
            _Norpart_EraseHWBlk((u4StartBlkIdx*SZ_64KB) + _rNorBlk.u4StaticPartStartAddr[u4Pid]);
            u4StartBlkIdx ++;  
            i ++;
        }
        return 0;
    }
    
    // read old block attribute.        
    u4OldLbn = _rNorBlk.au1PartMap[u4Pid];
    u4OldOffset = _rNorBlk.pu4LBA[u4OldLbn];
    VERIFY(0 == NOR_Read((UINT64)u4OldOffset, (UINT32)_rNorBlk.au1BH, BH_SIZE));
    u4Pan = BH_GET_PAN(_rNorBlk.au1BH);
    ASSERT(u4OldLbn == BH_GET_LBN(_rNorBlk.au1BH));
    // mark old block is ready to erase. 
    BH_SET_RFE(_rNorBlk.au1BH);
    BH_UPDATE(_rNorBlk.au1BH, u4OldOffset);

    // Get new block and copy old block attribute to new block.
    u4FreeBlk = _Norpart_GetFreeBlk();
    u4NewOffset = _rNorBlk.pu4LBA[u4FreeBlk];
    _rNorBlk.au1PartMap[u4Pid] = u4FreeBlk;
    VERIFY(0 == NOR_Read((UINT64)u4NewOffset, (UINT32)_rNorBlk.au1BH, BH_SIZE));
    BH_SET_PAN(_rNorBlk.au1BH, u4Pan);
    BH_SET_BIU(_rNorBlk.au1BH);
    BH_UPDATE(_rNorBlk.au1BH, u4NewOffset);

    // erase old block.
    _Norpart_EraseBlk(u4OldLbn);

    return 0;
}

#ifdef CC_CLI
//-------------------------------------------------------------------------
/** NORSTG_Read
*  NORSTG_Read() send read request to nor handling task in storage.c.
*   This is a synchronous call api.
*  @param  u4Pid              Partition ID.
*  @param  u4Offset         Offset to read data.
*  @param  u4MemPtr      User's read buffer address.
*  @param  u4MemLen     Read length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NORSTG_Read(UINT32 u4Pid, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    UINT64 u8Offset;
    STORG_DEV_T *prDev;
    
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    ASSERT((u4Offset+u4MemLen) <= NORPART_GetPartSize(u4Pid));
    ASSERT(u4MemPtr != 0);    

    // NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
    u8Offset = (UINT64)u4Offset;
    u8Offset |= ((UINT64)u4Pid) << 32; 

    prDev = STORGOpenDev(STORG_FLAG_NOR);
    
    i4Ret = STORGSendReq(prDev, STORG_OP_READ, NULL, NULL, NULL,
                            u8Offset, u4MemLen, (void *)u4MemPtr, NULL);

    VERIFY(0 == STORGCloseDev(prDev));
    return i4Ret;
}

//-------------------------------------------------------------------------
/** NORSTG_Write
*  NORSTG_Write() send write request to nor handling task in storage.c.
*   This is a synchronous call api.
*  @param  u4Pid              Partition ID.
*  @param  u4Offset         Offset to write data.
*  @param  u4MemPtr      User's write buffer address.
*  @param  u4MemLen     Write length (bytes).
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NORSTG_Write(UINT32 u4Pid, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    UINT64 u8Offset;
    STORG_DEV_T *prDev;
    
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    ASSERT((u4Offset+u4MemLen) <= NORPART_GetPartSize(u4Pid));
    ASSERT(u4MemPtr != 0);    

    // NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
    u8Offset = (UINT64)u4Offset;
    u8Offset |= ((UINT64)u4Pid) << 32; 

    prDev = STORGOpenDev(STORG_FLAG_NOR);
    
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE, NULL, NULL, NULL,
                            u8Offset, u4MemLen, (void *)u4MemPtr, NULL);

    VERIFY(0 == STORGCloseDev(prDev));
    return i4Ret;
}
#endif

//-------------------------------------------------------------------------
/** NORSTG_Erase
*  NORSTG_Erase() send erase partition request to nor handling task in storage.c.
*   This is a synchronous call api.
*  @param  u4Pid              Partition ID.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NORSTG_Erase(UINT32 u4Pid)
{
    INT32 i4Ret;
    STORG_DEV_T *prDev;
    UINT64 u8Offset;
    
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);

    prDev = STORGOpenDev(STORG_FLAG_NOR);

    u8Offset = ((UINT64)u4Pid) << 32;            
    i4Ret = STORGSendReq(prDev, STORG_OP_ERASE, NULL, NULL, NULL,
                            u8Offset, NORPART_GetPartSectorNum(u4Pid), NULL, NULL);

    VERIFY(0 == STORGCloseDev(prDev));
    return i4Ret;
}
#endif /* CC_5391_LOADER */
