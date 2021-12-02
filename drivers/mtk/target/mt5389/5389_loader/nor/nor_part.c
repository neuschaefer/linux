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
 * $Date: 2012/07/03 $
 * $RCSfile: nor_part.c,v $
 * $Revision: #3 $
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

#include "nor_if.h"
#ifndef CC_NOR_DISABLE

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

/*replace EEPORM with NOR */
#ifdef REPLACE_EEPROM_WITH_NOR
#define MAX_EEPROM_BUFFER_SIZE (8*1024)
#define DEFAULT_NOR_EEPROM_SECTOR_NUMBER 2 //Two 64k sector For one 2k or 4k Eerprom
#define NOR_EEPROM_TIMES 32 //32  times nor size, each should be 16 times nor size  
#define CURRENT_PARTITION_FLAG_VAL 0xA5 
#define NOT_CURRENT_PARTITION_FLAG_VAL 0x00 

#define CURRENT_PARTITION_FLAG_ADDR_OFF_END 4
#define OPTIONAL_RESTRICT_AREA_SIZE 1020
#define RESTRICT_AREA_SIZE (OPTIONAL_RESTRICT_AREA_SIZE + CURRENT_PARTITION_FLAG_ADDR_OFF_END)

#define NOR_EEPROM_PARTITION_CNT 2     

#define APPEND_UINT_BTTE_CNT 4
#endif

/** Constant size definitions.
 *
 */
#if 0
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
#endif

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
    BOOL au4Protection[PART_MIDD_END]; // firmware protection.
} NOR_BLK_T;

/*replace EEPORM with NOR */
//EEPROM Replacement,please do not protect eeprom area. 
#ifdef REPLACE_EEPROM_WITH_NOR
typedef struct _NOR_EEPROM_T
{
    BOOL   fgInitial;
    UINT32 u4EEPSize;	            //EEPROM Size
    UINT32 u4EEPNORPartId;          //Eeprom NOR  Partiton ID
    //UINT32 u4EEPNORPartStartOffset;   //Start offset from partition 
    UINT32 u4NORSectorSize;
    UINT32 u4EEPNORBlockNum;

	UINT32 u4EEPPartOffA;	
	UINT32 u4EEPPartOffB;
	UINT32 u4EEPNORPartCurAppendOffset;
   
    UINT8 u1EEPBuf[MAX_EEPROM_BUFFER_SIZE];
} NOR_EEPROM_T;

static NOR_EEPROM_T _rNorEEPROM;
#endif
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
EXTERN UINT32 _fgNorPartialProtection;
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
    
    x_memset(&rBlkMap, 0, sizeof(SECTTBL_T));

    // Query flash map.
    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    // make sure it is on 64 KB boundary.
    ASSERT((u4Offset % rBlkMap.prSegInfo->u4SectSize) == 0);

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

    ASSERT(u4SectSize <= rBlkMap.prSegInfo->u4SectSize);

    if (u4SectSize == rBlkMap.prSegInfo->u4SectSize)
    {
        // erase block.
        VERIFY(0 == NOR_EraseBlk(u4Offset));
    }
    else
    {
        // handle sector size < 64KB case.
        for (u4Size = 0; u4Size < rBlkMap.prSegInfo->u4SectSize; u4Size+= u4SectSize)
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
    SECTTBL_T rBlkMap;
    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    
    x_memset(&rBlkMap, 0, sizeof(SECTTBL_T));

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
        u4Offset += rBlkMap.prSegInfo->u4SectSize;
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
            u4Offset += rBlkMap.prSegInfo->u4SectSize;
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
        u4Offset += rBlkMap.prSegInfo->u4SectSize;
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
#if (defined(CC_NAND_BOOT) || defined(CC_NAND_LOADER))
UINT32 u4Temp = 0;
   UNUSED(_fgNORParamInit);
   UNUSED(_u4SZ_NOR_TOTAL_SIZE);
   UNUSED(_u4SZ_NOR_LOADER_SIZE);
   UNUSED(_u4SZ_NOR_APPIMAGE_SIZE);
   UNUSED(_u4SZ_NOR_PARTITION_NUM);
   UNUSED(_u4SZ_NOR_DYNAMIC_START);
   UNUSED(_u4SZ_NOR_DYNAMIC_SIZE);
   u4Temp++;
   u4Temp = 0;
   if(u4Temp)
   {
      UNUSED(_Norpart_CheckBEC());
      UNUSED(_Norpart_CheckDynamicPartition());
      UNUSED(_Norpart_CheckStaticPartition());
   }
#else
    UINT32 u4Spaceleft;
    UINT32 i,u4Temp;
    UINT32 u4QryType;
    UINT32 u4StaticSize = 0;
    UINT32 u4DynamicSize = 0;
    SECTTBL_T rBlkMap;
    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    if (_fgNORParamInit)
    {
        return 0;
    }
    _fgNORParamInit = TRUE;

    _u4SZ_NOR_TOTAL_SIZE = DRVCUST_InitGet(eNorFlashTotalSize);
    _u4SZ_NOR_LOADER_SIZE = DRVCUST_InitGet(eNorFlashLoaderSize);
    _u4SZ_NOR_APPIMAGE_SIZE= DRVCUST_InitGet(eNorFlashAppImageSize);
    _u4SZ_NOR_DYNAMIC_START = _u4SZ_NOR_LOADER_SIZE + _u4SZ_NOR_APPIMAGE_SIZE;
    ASSERT(_u4SZ_NOR_TOTAL_SIZE > _u4SZ_NOR_DYNAMIC_START);

    // set user partition space.
    u4Spaceleft = _u4SZ_NOR_TOTAL_SIZE - _u4SZ_NOR_DYNAMIC_START;

    _u4SZ_NOR_DYNAMIC_SIZE = DRVCUST_InitGet(eNorFlashDynamicPartitionSize);
    _u4SZ_NOR_DYNAMIC_SIZE = MIN(u4Spaceleft, _u4SZ_NOR_DYNAMIC_SIZE);

    ASSERT(_u4SZ_NOR_TOTAL_SIZE >=
        (_u4SZ_NOR_DYNAMIC_START + _u4SZ_NOR_DYNAMIC_SIZE));

    // set static partition space.
    _u4SZ_NOR_STATIC_START =
        _u4SZ_NOR_DYNAMIC_START + _u4SZ_NOR_DYNAMIC_SIZE;
    _u4SZ_NOR_STATIC_SIZE = DRVCUST_InitGet(eNorFlashStaticPartitionSize);

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

    for (i = (UINT32)PART_MIDD_0; i < (UINT32)PART_MIDD_END; i++)
    {
       u4Temp = NORPART_PARTIAL_PROTECTION_ID;
       if(i < u4Temp)
       {
          _rNorBlk.au4Protection[i] = NORPART_PROTECTION_ENABLE;
       }
       else
       {
          _rNorBlk.au4Protection[i] = NORPART_PROTECTION_DISENABLE;
       }
    }

    // get partition size and determine partition type.
    _u4SZ_NOR_PARTITION_NUM = 0;
    u4QryType = (UINT32)eNorFlashPartSize2;
    for (i = (UINT32)PART_MIDD_2; i < (UINT32)PART_MIDD_END; i++)
    {
        _rNorBlk.u4PartSize[i] = DRVCUST_InitGet((QUERY_TYPE_T)u4QryType++);
        if (_rNorBlk.u4PartSize[i] > rBlkMap.prSegInfo->u4SectSize)
        {
            u4StaticSize += _rNorBlk.u4PartSize[i];
            _rNorBlk.au4PartType[i] = NORPART_STATIC_TYPE;
            _u4SZ_NOR_PARTITION_NUM ++;
        }
        else if (_rNorBlk.u4PartSize[i] == rBlkMap.prSegInfo->u4SectSize)
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

//    ASSERT(u4StaticSize <= _u4SZ_NOR_STATIC_SIZE);
    ASSERT(u4DynamicSize <= _u4SZ_NOR_DYNAMIC_SIZE);
    UNUSED(u4StaticSize);
    UNUSED(u4DynamicSize);
#endif
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
#if (defined(CC_NAND_BOOT) || defined(CC_NAND_LOADER))

#else
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

//        ASSERT(u4TotalSize >= _u4SZ_NOR_TOTAL_SIZE);
        if (u4TotalSize < _u4SZ_NOR_TOTAL_SIZE)
        {
            LOG(0, "***nor config may have problem,u4TotalSize:%d,  _u4SZ_NOR_TOTAL_SIZE:%x***\n", u4TotalSize, _u4SZ_NOR_TOTAL_SIZE);
        }
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
//            ASSERT(rBlkMap.prSegInfo[i].u4SectSize == SZ_64KB);

            // initial block information.
            _rNorBlk.u4BlkNum = ((u4Size - _u4SZ_NOR_DYNAMIC_START) >= _u4SZ_NOR_DYNAMIC_SIZE)
                                    ? (_u4SZ_NOR_DYNAMIC_SIZE /rBlkMap.prSegInfo->u4SectSize)
                                    : ((u4Size - _u4SZ_NOR_DYNAMIC_START) /(rBlkMap.prSegInfo->u4SectSize));

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
#endif
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
    SECTTBL_T rBlkMap;
    
    x_memset(&rBlkMap, 0, sizeof(SECTTBL_T));

    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    //ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    ASSERT(pt_sector_tbl != NULL);
    
    if(u4Pid >= (UINT32)PART_MIDD_END)
    {
        LOG(1, "NOR Part ID exceeds!\n");
        return (INT32)1;   	
    }

    if (pt_sector_tbl->u4SegNum < 1)
    {
        return (INT32)1;
    }

    pt_sector_tbl->u4SegNum = 1;
    u4Size = NORPART_GetPartSize(u4Pid);
    pt_sector_tbl->prSegInfo[0].u4SectNum = (u4Size + (rBlkMap.prSegInfo->u4SectSize  -1))/rBlkMap.prSegInfo->u4SectSize;
    pt_sector_tbl->prSegInfo[0].u4SectSize = MIN(u4Size, rBlkMap.prSegInfo->u4SectSize);

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
    SECTTBL_T rBlkMap;
    
    x_memset(&rBlkMap, 0, sizeof(SECTTBL_T));

    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    //ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    if(u4Pid >= (UINT32)PART_MIDD_END)
    {
        LOG(1, "NOR Part ID exceeds!\n");
        return 0;
    }

    u4Size = NORPART_GetPartSize(u4Pid);

    return ((u4Size + (rBlkMap.prSegInfo->u4SectSize -1))/rBlkMap.prSegInfo->u4SectSize);
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
    if(u2PartId >= (UINT32)PART_MIDD_END){return 1;}
//	printf("u2PartId = %d \n",u2PartId);
//	printf("u4Offset+u4MemLen = %d , NORPART_GetPartSize(u2PartId) = %d \n",(u4Offset+u4MemLen),NORPART_GetPartSize(u2PartId));
  //  ASSERT((u4Offset+u4MemLen) <= NORPART_GetPartSize(u2PartId));
    #ifdef NORPART_DEBUG
    LOG(7, "NOR read PID=%d, offset=%d.\n", u2PartId, u4Offset);
    #endif

 //   ASSERT(_rNorBlk.fgInitial != 0);

    // get physical address by partition id.
    u4Offset += DRVCUST_InitGet(eNorFlashPartOffset0+u2PartId);
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
    if(u2PartId >= (UINT32)PART_MIDD_END){return 1;}
    ASSERT((u4Offset+u4MemLen) <= NORPART_GetPartSize(u2PartId));
    #ifdef NORPART_DEBUG
    LOG(7, "NOR write PID=%d, offset=%d.\n", u2PartId, u4Offset);
    #endif

    ASSERT(_rNorBlk.fgInitial != 0);
    ASSERT(u2PartId < (UINT16)PART_MIDD_END);
    if(u2PartId < (UINT16)PART_MIDD_END)//Klocwork 20090703
    {
      if(_rNorBlk.au4Protection[u2PartId] == NORPART_PROTECTION_ENABLE)
      {
         LOG(0, "NORPART Id[%d] Write Protection Enable[%d]\n", u2PartId, _rNorBlk.au4Protection[u2PartId]);
         ASSERT(_rNorBlk.au4Protection[u2PartId] != NORPART_PROTECTION_ENABLE);
         return 1;
      }
    }
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
    SECTTBL_T rBlkMap;
    
    x_memset(&rBlkMap, 0, sizeof(SECTTBL_T));

    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    // initial nor part
    VERIFY(_Norpart_PartInit() == 0);

    // the high 32 bits as partition id.
    u4StartBlkIdx = (UINT32)(u8Offset & 0xFFFFFFFF);
    u4Pid = (u8Offset >> (UINT64)32) & 0xFFFF;
    ASSERT(u4Pid < (UINT32)PART_MIDD_END);
    if(u4Pid >= (UINT32)PART_MIDD_END){return 1;}
    if (_rNorBlk.au4PartType[u4Pid] == NORPART_UNUSED_TYPE)
    {
        return 0;
    }
    if(_rNorBlk.au4Protection[u4Pid] == NORPART_PROTECTION_ENABLE)
    {
       LOG(0, "NORPART Id[%d] Erase Protection Enable[%d]\n", u4Pid, _rNorBlk.au4Protection[u4Pid]);
       ASSERT(_rNorBlk.au4Protection[u4Pid] != NORPART_PROTECTION_ENABLE);
       return 1;
    }

    // erase static partition.
    if (_rNorBlk.au4PartType[u4Pid] == NORPART_STATIC_TYPE)
    {
        i = 0;
        while (i < u4SectNum)
        {
            _Norpart_EraseHWBlk((u4StartBlkIdx*rBlkMap.prSegInfo->u4SectSize) + _rNorBlk.u4StaticPartStartAddr[u4Pid]);
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
#ifdef REPLACE_EEPROM_WITH_NOR

INT32 Get_NOR_EEPROM_INFO(VOID )
{
	//Print  _rNorEEPROM Information
	LOG(1, "\n_rNorEEPROM Info:\n");
	LOG(1, "-----> u4EEPSize: 0X%x\n", _rNorEEPROM.u4EEPSize);
	LOG(1, "-----> u4EEPNORPartId: %d\n", _rNorEEPROM.u4EEPNORPartId);
	LOG(1, "-----> u4NORSectorSize: 0X%x\n", _rNorEEPROM.u4NORSectorSize);
	LOG(1, "-----> u4EEPNORBlockNum: %d\n", _rNorEEPROM.u4EEPNORBlockNum);
	LOG(1, "-----> u4EEPPartOffA: 0X%x\n", _rNorEEPROM.u4EEPPartOffA);
	LOG(1, "-----> u4EEPPartOffB: 0X%x\n", _rNorEEPROM.u4EEPPartOffB);
	LOG(1, "-----> u4EEPNORPartCurAppendOffset: 0X%x\n", _rNorEEPROM.u4EEPNORPartCurAppendOffset);
	LOG(1, "-----> u1EEPBuf: 0X%x\n", (UINT32)(_rNorEEPROM.u1EEPBuf));
		
	LOG(1, "\nGeneral Macro Info:\n");
	LOG(1, "-----> MAX_EEPROM_BUFFER_SIZE: 0X%x\n", MAX_EEPROM_BUFFER_SIZE);
	LOG(1, "-----> NOR_EEPROM_TIMES: 0X%x\n", NOR_EEPROM_TIMES);
	LOG(1, "-----> DEFAULT_NOR_EEPROM_SECTOR_NUMBER: 0X%x\n", DEFAULT_NOR_EEPROM_SECTOR_NUMBER);
	LOG(1, "-----> CURRENT_PARTITION_FLAG_ADDR_OFF_END: 0X%x\n", CURRENT_PARTITION_FLAG_ADDR_OFF_END);
	LOG(1, "-----> OPTIONAL_RESTRICT_AREA_SIZE: 0X%x\n", OPTIONAL_RESTRICT_AREA_SIZE);
	LOG(1, "-----> RESTRICT_AREA_SIZE: 0X%x\n", RESTRICT_AREA_SIZE);
	LOG(1, "-----> NOR_EEPROM_PARTITION_CNT: 0X%x\n", NOR_EEPROM_PARTITION_CNT);
	LOG(1, "-----> APPEND_UINT_BTTE_CNT: 0X%x\n", APPEND_UINT_BTTE_CNT);	

	LOG(1, "\nEEPROM Partition Info:\n");
	LOG(1, "-----> Partition ID: 0X%x\n", _rNorEEPROM.u4EEPNORPartId);
	LOG(1, "-----> Partition start offset: 0X%x\n", NORPART_PartStartOffset(_rNorEEPROM.u4EEPNORPartId));
	LOG(1, "-----> Partition size: 0X%x\n", NORPART_GetPartSize(_rNorEEPROM.u4EEPNORPartId));	
	return 0;
}

//-------------------------------------------------------------------------
/** CalculateCRC
 *  @param  u4Addr          The start address.
 *  @param  u4ByteNum    Byte number.
 *  @retval   CRC value
 *  
 */
//-------------------------------------------------------------------------
static UINT8 CalculateCRC(UINT32 u4Addr, UINT32 u4ByteNum)
{
	UINT32 i,j,k;
	UINT8 u1Ret = 0;
	UINT8 *u1Addr = (UINT8 *)u4Addr; 
	UINT8 u1TempBuf[8];
	UINT8 u1Tempvalue;
	
	ASSERT(u1Addr != NULL);
	ASSERT(u4ByteNum > 0);
	
	u1Ret = 0xff;
	for (i = 0; i < u4ByteNum; i++)
	{
	   u1Tempvalue = u1Addr[i];
	   for (j = 0 ; j < 8 ; j++)
	   {
			u1TempBuf[j] = u1Tempvalue & 0x01;
			u1Tempvalue >>= 1;
	   }
	   
	   for (k = 0; k < 8; k++)
	   {
			u1TempBuf[7-k] <<=7;
			if ((u1Ret ^ u1TempBuf[7-k]) & 0x80)
			{
				u1Ret = (u1Ret << 1)^0x1d ;
			}
			else
			{
				u1Ret <<= 1;
			}
	   }
	}
	
	u1Ret ^= 0xff;
	return u1Ret;
}

//-------------------------------------------------------------------------
/** _NOR_Find_EEPROMData 
 *   _NOR_Find_EEPROMData find the right EEPROM data from NOR flash when reset the PCB .
 *  @retval 0	        Success
 *  @retval Others	Fail
 *  
 */
//-------------------------------------------------------------------------
static INT32 _NOR_Find_EEPROMData(void)
{
	UINT64 u8CurOff = 0;
	UINT32 u4PartitionSize;
	UINT32 u4PartAFlag;
	UINT32 u4PartBFlag;
	UINT32 u4PartAFlagOffSet;
	UINT32 u4PartBFlagOffSet;
	UINT32 u4CurPartEndoffset;
	UINT32 u4CurPartition;
	UINT32 u4ScanOffset;

	UINT32 u4MemPtr = 0x00;
	UINT32 u4TmpPtr = 0x00;
	UINT32 u4EndPtr = 0x00;
	
	
	UINT32 *u4WordPtr = 0x00;
	UINT16 *u2EPPROMAddr = 0x00;
	UINT8  *u1BytePtr = 0x00;
	INT32   i4Ret = 0;
	
	//Find the current NORpartition for EEPROM
	u4PartAFlagOffSet = _rNorEEPROM.u4EEPPartOffA + (_rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT)*(_rNorEEPROM.u4NORSectorSize) - CURRENT_PARTITION_FLAG_ADDR_OFF_END;
	u4PartBFlagOffSet = _rNorEEPROM.u4EEPPartOffB + (_rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT)*(_rNorEEPROM.u4NORSectorSize) - CURRENT_PARTITION_FLAG_ADDR_OFF_END;

	u8CurOff = (UINT64)(u4PartAFlagOffSet);
	u8CurOff |= (((UINT64)(_rNorEEPROM.u4EEPNORPartId)) << 32);

	LOG(1, "Read partA current flag: 0X%x,0X%x,0X%x\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, u4PartAFlagOffSet);
	i4Ret = NORPART_Read(u8CurOff, (UINT32)(&u4PartAFlag), 4);
	if(0 != i4Ret)
	{
		LOG(0, "ERROR: Read partA current flag error!(_NOR_Find_EEPROMData)\n");
		LOG(0, "-----> Partition Id: 0X%x\n", _rNorEEPROM.u4EEPNORPartId);
		LOG(0, "-----> Flag offset: 0X%x\n", u4PartAFlagOffSet);		
		return -1;	
	}

	u8CurOff = (UINT64)(u4PartBFlagOffSet);
	u8CurOff |= (((UINT64)(_rNorEEPROM.u4EEPNORPartId)) << 32);	
	
	LOG(1, "Read partB current flag: 0X%x,0X%x,0X%x\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, u4PartBFlagOffSet);
	i4Ret = NORPART_Read(u8CurOff, (UINT32)(&u4PartBFlag), 4);
	if(0 != i4Ret)
	{
		LOG(0, "ERROR: Read partB current flag error!(_NOR_Find_EEPROMData)\n");
		LOG(0, "-----> Partition Id: 0X%x\n", _rNorEEPROM.u4EEPNORPartId);
		LOG(0, "-----> Flag offset: 0X%x\n", u4PartBFlagOffSet);		
		return -1;	
	}

	LOG(1, "PartAFlag: 0X%x, PartBFlag: 0X%x\n", u4PartAFlag, u4PartBFlag);

	if(u4PartAFlag == u4PartBFlag)
	{
		u8CurOff = (UINT64)(u4PartAFlagOffSet);
		u8CurOff |= (((UINT64)(_rNorEEPROM.u4EEPNORPartId)) << 32);
		u4PartAFlag--;
		LOG(1, "Write partA current flag: 0X%x,0X%x,0X%x,0X%x\n,", 
			u8CurOff, 
			_rNorEEPROM.u4EEPNORPartId, 
			_rNorEEPROM.u4EEPPartOffA,
			u4PartAFlag
			);
		i4Ret = NORPART_Write(u8CurOff, (UINT32)(&u4PartAFlag), 4);				
		if(0 != i4Ret)
		{
			LOG(0, "ERROR: Write partA current flag error!(_NOR_Find_EEPROMData)\n");
			LOG(0, "-----> Partition Id: 0X%x\n", _rNorEEPROM.u4EEPNORPartId);
			LOG(0, "-----> Flag offset: 0X%x\n", u4PartAFlagOffSet);		
			return -1;	
		}


		if(u4PartBFlag == 0xffffffff)
		{
			_rNorEEPROM.u4EEPNORPartCurAppendOffset = _rNorEEPROM.u4EEPPartOffA + _rNorEEPROM.u4EEPSize;
			LOG(1, "LOG: EEPROM have not been written yet!(_NOR_Find_EEPROMData)\n");
			LOG(1, "LOG: u4EEPNORPartCurAppendOffset: 0X%x(_NOR_Find_EEPROMData)\n", _rNorEEPROM.u4EEPNORPartCurAppendOffset);
			return 0;				
		}
		else
		{
			u4CurPartition = _rNorEEPROM.u4EEPPartOffA;
			LOG(0, "WARN: Flag error!Assume current is Part(_NOR_Find_EEPROMData)\n");
		}

	}
	else if(u4PartAFlag > u4PartBFlag)
	{
		if(u4PartAFlag == 0xffffffff && u4PartBFlag == 0x00)
		{
			u4CurPartition = _rNorEEPROM.u4EEPPartOffA;
		}
		else
		{
			u4CurPartition = _rNorEEPROM.u4EEPPartOffB;
		}
	}
	else
	{
		if(u4PartAFlag == 0x00 && u4PartBFlag == 0xffffffff)
		{
			u4CurPartition = _rNorEEPROM.u4EEPPartOffB;
		}
		else
		{
			u4CurPartition = _rNorEEPROM.u4EEPPartOffA;
		}
	}
	
	//LOG(1, "u4CurPartition: 0X%x\n", u4CurPartition);
	u8CurOff = (UINT64)u4CurPartition & 0xffffffff; 
	u8CurOff |= (((UINT64)(_rNorEEPROM.u4EEPNORPartId)) << 32);
	u4PartitionSize = _rNorEEPROM.u4NORSectorSize * _rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT;
	u4CurPartEndoffset = u4CurPartition + u4PartitionSize - RESTRICT_AREA_SIZE;
	_rNorEEPROM.u4EEPNORPartCurAppendOffset = u4CurPartition + _rNorEEPROM.u4EEPSize;
	
	u4MemPtr = (UINT32)x_mem_alloc(_rNorEEPROM.u4EEPSize);
	if(u4MemPtr == 0)
    {
		LOG(0, "ERROR: Malloc u4MemPtr failed!(_NOR_Find_EEPROMData)\n");	
        return -1;
    }	
	
	LOG(1, "Read current partition: 0X%x,0X%x,0X%x\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, u4CurPartition);
	i4Ret = NORPART_Read(u8CurOff, (UINT32)(_rNorEEPROM.u1EEPBuf), _rNorEEPROM.u4EEPSize);	
	if(0 != i4Ret)
	{
		LOG(0, "ERROR: Read current partition failed: 0X%x,0X%x,0X%x!\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, u4CurPartition);	
		return -1;	
	}	
	
	//Scan from the start of partition for filling the buffer of NOR_EEPROM and finding the current read/write pointer
	for(u4ScanOffset = u4CurPartition + _rNorEEPROM.u4EEPSize; u4ScanOffset < u4CurPartEndoffset; u4ScanOffset += _rNorEEPROM.u4EEPSize)
	{
		u4EndPtr = u4MemPtr + _rNorEEPROM.u4EEPSize;
		if(u4ScanOffset + _rNorEEPROM.u4EEPSize >= u4CurPartEndoffset)
		{
			u4EndPtr -= (u4ScanOffset + _rNorEEPROM.u4EEPSize - u4CurPartEndoffset);
		}
		
		u8CurOff = (UINT64)u4ScanOffset & 0xffffffff; 
		u8CurOff |= (((UINT64)(_rNorEEPROM.u4EEPNORPartId)) << 32);
		
		LOG(1, "Read current partition: 0X%x,0X%x,0X%x\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, u4ScanOffset);	
		i4Ret = NORPART_Read(u8CurOff, u4MemPtr, _rNorEEPROM.u4EEPSize);	
		if(0 != i4Ret)
		{
			LOG(0, "ERROR: Read current partition failed: 0X%x,0X%x,0X%x!\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, u4ScanOffset);	
			return -1;
		}

		for(u4TmpPtr = u4MemPtr; u4TmpPtr < u4EndPtr; u4TmpPtr +=  APPEND_UINT_BTTE_CNT)
		{
			u4WordPtr = (UINT32 *)u4TmpPtr;
			if(u4WordPtr[0] == 0xFFFFFFFF)
			{
					//_rNorEEPROM.u4EEPNORPartCurAppendOffset = (UINT32)(u8CurOff & 0xFFFFFFFF) + (UINT32)(u4WordPtr + 1) - u4MemPtr;
					x_mem_free((VOID *)u4MemPtr);
					u4MemPtr = 0x00;
					//LOG(1, "LOG: EEPROM have not been written yet!(_NOR_Find_EEPROMData)\n");
					LOG(1, "LOG: Scan finished!u4EEPNORPartCurAppendOffset: 0X%x(_NOR_Find_EEPROMData)\n", _rNorEEPROM.u4EEPNORPartCurAppendOffset);
					return 0;		

			}
			else
			{
				u1BytePtr = (UINT8	*)u4WordPtr;
				u2EPPROMAddr = (UINT16	*)u4WordPtr;
				_rNorEEPROM.u4EEPNORPartCurAppendOffset += APPEND_UINT_BTTE_CNT;
				//check address
				if((UINT32)(u2EPPROMAddr[0] & 0xFFFF) > _rNorEEPROM.u4EEPSize)
				{
					//Error but continue
					LOG(0, "WARN: EEPROM Address error 0X%x!(_NOR_Find_EEPROMData)\n", *u4WordPtr);
					continue;
				}

				//check CRC
				if(CalculateCRC(u4TmpPtr, 3) != u1BytePtr[3])
				{
					//Printf("CRC error!");
					LOG(0, "WARN: CRC error 0X%x!(_NOR_Find_EEPROMData)\n", *u4WordPtr);
					continue;
				}
				_rNorEEPROM.u1EEPBuf[(UINT32)(u2EPPROMAddr[0])] = u1BytePtr[2];							
			}
		}
	}
	
	x_mem_free((VOID *)u4MemPtr);
	u4MemPtr = 0x00;
	return 0;
}

static INT32 _NOR_EEPROM_Init(void)
{
    //UINT32 i;
	UINT32 u4SectorNum;
	SFLASH_INFO_T rNORInfo;
    SECTTBL_T rBlkMap;

    if (_rNorEEPROM.fgInitial == TRUE)
    {
        return 0;
    }

	//GetNORInfo & initial NOR Flash
    if (NOR_GetInfo(&rNORInfo) != 0)
    {
        LOG(0, "ERROR: NOR_GetInfo error![_NOR_EEPROM_Init]\n");
        return -1;
    }
	
    x_memset((void *)(&rBlkMap), 0, sizeof(SECTTBL_T));

    VERIFY(0 == NOR_EraseMap(&rBlkMap));
    // initial nor part
    VERIFY(_Norpart_PartInit() == 0);	

	_rNorEEPROM.u4NORSectorSize = rBlkMap.prSegInfo->u4SectSize;
    _rNorEEPROM.u4EEPSize = DRVCUST_InitGet(eSystemEepromSize);
    _rNorEEPROM.u4EEPNORPartId = DRVCUST_InitGet(eEepromNorPartId);


	if(_rNorEEPROM.u4EEPNORPartId > 1)
	{
		_rNorEEPROM.u4EEPPartOffA = (UINT32) DRVCUST_InitGet(eEepromNorPartoffset);
		if((_rNorEEPROM.u4EEPPartOffA % _rNorEEPROM.u4NORSectorSize) != 0)
		{
			LOG(0, "ERROR: EEPPartOffA: 0X%x should be N*SectorSize!(_NOR_EEPROM_Init)\n", _rNorEEPROM.u4EEPPartOffA);
			return -1;			
		}
	}
	else
	{
		LOG(0, "ERROR:Partition ID: %d error!(_NOR_EEPROM_Init)\n", _rNorEEPROM.u4EEPNORPartId);
		return -1;
	}

	if(MAX_EEPROM_BUFFER_SIZE < _rNorEEPROM.u4EEPSize)
	{
		LOG(0, "ERROR: EEPROM size is larger than MAX_EEPROM_BUFFER_SIZE!(_NOR_EEPROM_Init)\n");
		LOG(0, "-----> MAX_EEPROM_BUFFER_SIZE: 0X%x\n", MAX_EEPROM_BUFFER_SIZE);
		LOG(0, "-----> EEPROM size: 0X%x\n", _rNorEEPROM.u4EEPSize);
		return -1;
	}

	//caculate the size of NOR Flash for Eeprom
	u4SectorNum = (_rNorEEPROM.u4EEPSize * NOR_EEPROM_TIMES) / _rNorEEPROM.u4NORSectorSize;
	u4SectorNum = (u4SectorNum <=  DEFAULT_NOR_EEPROM_SECTOR_NUMBER) ? DEFAULT_NOR_EEPROM_SECTOR_NUMBER : u4SectorNum;
	if((_rNorEEPROM.u4EEPPartOffA + u4SectorNum * _rNorEEPROM.u4NORSectorSize) <= NORPART_GetPartSize(_rNorEEPROM.u4EEPNORPartId))
	{
		_rNorEEPROM.u4EEPNORBlockNum = u4SectorNum;
		_rNorEEPROM.u4EEPPartOffB = _rNorEEPROM.u4EEPPartOffA + (_rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT) * _rNorEEPROM.u4NORSectorSize;
	}
	else
	{
		LOG(0, "ERROR: Partition size is too small!(_NOR_EEPROM_Init)\n");
		LOG(0, "-----> Partition size: 0X%x\n", NORPART_GetPartSize(_rNorEEPROM.u4EEPNORPartId));
		LOG(0, "-----> EEPPartOffA: 0X%x\n", _rNorEEPROM.u4EEPPartOffA);
		LOG(0, "-----> EEPROM size: 0X%x\n", _rNorEEPROM.u4EEPSize);
		LOG(0, "-----> NOR_EEPROM_TIMES :0X%x\n", NOR_EEPROM_TIMES);
		return -1;
	}
    
    x_memset((void *)(_rNorEEPROM.u1EEPBuf), 0xFF, MAX_EEPROM_BUFFER_SIZE);
    
	_rNorEEPROM.u4EEPNORPartCurAppendOffset = 0xFFFFFFFF;
	
	//Find the current offset and read from nor to fill _rNorEEPROM.u1EEPBuf
	_NOR_Find_EEPROMData();
	
	_rNorEEPROM.fgInitial = TRUE;
	Get_NOR_EEPROM_INFO();

	return 0;
}

INT32 NOR_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    LOG(1, "read offset = 0x%x, memptr = 0x%x, u4MemLen = 0x%x(NOR_EEPROM_Read)\n", u4Offset, u4MemPtr, u4MemLen);
    VERIFY(_NOR_EEPROM_Init() == 0);
	//UINT32 i;
	
	if(u4Offset +  u4MemLen > _rNorEEPROM.u4EEPSize)
	{
		//Printf("Read EEPRMOM offset is larger than EEPROM size!");
		LOG(0, "ERROR:offset add Len is larger than EEPROM size!(NOR_EEPROM_Read)\n");
		LOG(0, "---->:offset 0X%x,len 0X%x,EEPROM size 0X%x\n", u4Offset, u4MemLen, _rNorEEPROM.u4EEPSize);
		return -1;
	}

	x_memcpy((VOID*)(u4MemPtr), (VOID*)(_rNorEEPROM.u1EEPBuf + u4Offset), u4MemLen);
	return 0;
}

INT32 NOR_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
//	UINT32 u4CurPartition = 0;
	UINT64 u8NORPartEraseoffset = 0;
	UINT64 u8NORPartWriteoffset; 


	UINT32 u4AppendEndNORAddr = 0;
	BOOL   bIsSwithPartition = FALSE;

	UINT32 u4PriorPartition = 0;
	UINT32 i = 0;
	UINT32 u4AppendData = 0;


	UINT8 *u1UnitStartAddr = 0x00;
	UINT16 *u2Addr = 0x00;

	UINT64 u8MarkFlagAddr = 0;
	UINT64 u8UnMarkFlagAddr = 0;
	INT32 u4FlagValue;

	VERIFY(_NOR_EEPROM_Init() == 0);

    LOG(1, "write offset = 0x%x, memptr = 0x%x, u4MemLen = 0x%x(NOR_EEPROM_Write)\n", u4Offset, u4MemPtr, u4MemLen);

	if(u4Offset +  u4MemLen > _rNorEEPROM.u4EEPSize)
	{
		//Printf("Read EEPRMOM offset is larger than EEPROM size!");
		LOG(0, "ERROR:offset add Len is larger than EEPROM size!\n(NOR_EEPROM_Read)");
		LOG(0, "---->:offset 0X%x,len 0X%x,EEPROM size 0X%x\n", u4Offset, u4MemLen, _rNorEEPROM.u4EEPSize);
		return -1;

	}

	//Partition A append area(start addr and end addr)
	UINT32 u4PartA_AppenAreaStarAddr = _rNorEEPROM.u4EEPPartOffA + _rNorEEPROM.u4EEPSize;
	UINT32 u4PartA_AppenAreaEndAddr  = _rNorEEPROM.u4EEPPartOffA + _rNorEEPROM.u4NORSectorSize * _rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT - RESTRICT_AREA_SIZE;

	//Partition B append area(start addr and end addr)
	UINT32 u4PartB_AppenAreaStarAddr = _rNorEEPROM.u4EEPPartOffB + _rNorEEPROM.u4EEPSize;
	UINT32 u4PartB_AppenAreaEndAddr  = _rNorEEPROM.u4EEPPartOffB + _rNorEEPROM.u4NORSectorSize * _rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT - RESTRICT_AREA_SIZE;

	u4AppendEndNORAddr = _rNorEEPROM.u4EEPNORPartCurAppendOffset + u4MemLen*APPEND_UINT_BTTE_CNT;
	


	// whether u4EEPNORPartCurAppendOffset is valid
	if(_rNorEEPROM.u4EEPNORPartCurAppendOffset < u4PartA_AppenAreaStarAddr
		|| (_rNorEEPROM.u4EEPNORPartCurAppendOffset > u4PartA_AppenAreaEndAddr && _rNorEEPROM.u4EEPNORPartCurAppendOffset < u4PartB_AppenAreaStarAddr)
		|| _rNorEEPROM.u4EEPNORPartCurAppendOffset >  u4PartB_AppenAreaEndAddr)
	{
		//Printf("NOR_EEPROM_Write: EEPROM current offset is invalid!");
		LOG(0, "ERROR: current appendoffset 0X%x invalid !(NOR_EEPROM_Write)\n", _rNorEEPROM.u4EEPNORPartCurAppendOffset);
		return -1;
	}

	//Is Swith Partition
	if((_rNorEEPROM.u4EEPNORPartCurAppendOffset >= u4PartA_AppenAreaStarAddr && u4AppendEndNORAddr <= u4PartA_AppenAreaEndAddr)//If current partition is partion A and not need switch partition
    || (_rNorEEPROM.u4EEPNORPartCurAppendOffset >= u4PartB_AppenAreaStarAddr && u4AppendEndNORAddr <= u4PartB_AppenAreaEndAddr)
	  )
	{
		bIsSwithPartition = FALSE;
	}
	else
	{
		bIsSwithPartition = TRUE;
	}

	if(bIsSwithPartition == FALSE)
	{
		u4AppendData = (UINT32)x_mem_alloc(APPEND_UINT_BTTE_CNT*u4MemLen);
		for(i = 0; i < u4MemLen; i++)
		{
			u1UnitStartAddr = (UINT8 *)(u4AppendData + APPEND_UINT_BTTE_CNT*i);
			u2Addr = (UINT16 *)u1UnitStartAddr;
			u2Addr[0] = u4Offset + i;
			u1UnitStartAddr[2] = ((UINT8 *)(u4MemPtr))[i];
			u1UnitStartAddr[3] = CalculateCRC((UINT32)u1UnitStartAddr, 3);
		}	
		
		u8NORPartWriteoffset = (UINT64)(_rNorEEPROM.u4EEPNORPartCurAppendOffset) & 0xFFFFFFFF;
		u8NORPartWriteoffset |= (UINT64)(_rNorEEPROM.u4EEPNORPartId) << 32;

		LOG(1, "Write NOR EEPROM: 0X%x,0X%x,0X%x\n", u8NORPartWriteoffset, _rNorEEPROM.u4EEPNORPartId, _rNorEEPROM.u4EEPNORPartCurAppendOffset);	
		if(0 != NORPART_Write(u8NORPartWriteoffset, u4AppendData, APPEND_UINT_BTTE_CNT*u4MemLen))
		{
			return -1;
		}
		_rNorEEPROM.u4EEPNORPartCurAppendOffset += APPEND_UINT_BTTE_CNT*u4MemLen;
		x_memcpy((VOID*)(_rNorEEPROM.u1EEPBuf + u4Offset), (VOID*)(u4MemPtr), u4MemLen);	
		x_mem_free((VOID *)u4AppendData);
	}
	else
	{
		if(u4AppendEndNORAddr  > u4PartB_AppenAreaEndAddr) //If current partition is partion B and need switch partition
		{
			//Erase partition A
			u8NORPartEraseoffset = (UINT64)(_rNorEEPROM.u4EEPPartOffA / _rNorEEPROM.u4NORSectorSize) & 0xffffffff;
			u8NORPartWriteoffset = (UINT64)(_rNorEEPROM.u4EEPPartOffA) & 0xFFFFFFFF;
			u4PriorPartition    = _rNorEEPROM.u4EEPPartOffB;
		}
		else
		{
			u8NORPartEraseoffset =  (UINT64)(_rNorEEPROM.u4EEPPartOffB / _rNorEEPROM.u4NORSectorSize) & 0xffffffff;
			u8NORPartWriteoffset = (UINT64)(_rNorEEPROM.u4EEPPartOffB) & 0xFFFFFFFF;
			u4PriorPartition = _rNorEEPROM.u4EEPPartOffA;
		}

		u8NORPartEraseoffset |= (UINT64)(_rNorEEPROM.u4EEPNORPartId) << 32;
		LOG(1, "Erase NOR EEPROM: 0X%x,0X%x\n", u8NORPartEraseoffset, (UINT32)u8NORPartEraseoffset);	
		if(NORPART_Erase(u8NORPartEraseoffset, _rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT) != 0)
		{
			LOG(0, "Error: NORPART_Erase error,offset:%x,sector num:%d!(NOR_EEPROM_Write)\n", u8NORPartEraseoffset,_rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT);
			return -1;
		}

		x_memcpy((VOID *)(_rNorEEPROM.u1EEPBuf + u4Offset) , (VOID *)u4MemPtr, u4MemLen);
		u8NORPartWriteoffset |= (UINT64)(_rNorEEPROM.u4EEPNORPartId) << 32;
		
		LOG(1, "Write NOR EEPROM: 0X%x,0X%x,0X%x\n", u8NORPartWriteoffset, _rNorEEPROM.u4EEPNORPartId, _rNorEEPROM.u4EEPNORPartCurAppendOffset);	
		if(0 != NORPART_Write(u8NORPartWriteoffset, (UINT32)(_rNorEEPROM.u1EEPBuf), _rNorEEPROM.u4EEPSize))
		{
			return -1;
		}
		//Mark current partition and unmark prior partition
		u8UnMarkFlagAddr = (UINT64)(u4PriorPartition + _rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT * _rNorEEPROM.u4NORSectorSize - CURRENT_PARTITION_FLAG_ADDR_OFF_END);
		u8UnMarkFlagAddr |= (UINT64)(_rNorEEPROM.u4EEPNORPartId) << 32;
		//LOG(1, "_NOR_Find_EEPROMData: Read from nor %x %x %x\n", u8CurOff, _rNorEEPROM.u4EEPNORPartId, _rNorEEPROM.u4EEPPartOffA);

		if(NORPART_Read(u8UnMarkFlagAddr, (UINT32)(&u4FlagValue), CURRENT_PARTITION_FLAG_ADDR_OFF_END) != 0)
		{
			return -1;
		}

		LOG(1, "Read prior partition flag: 0X%x,0X%x,0X%x\n", _rNorEEPROM.u4EEPNORPartId, (UINT32)(u8UnMarkFlagAddr & 0xFFFFFFFF), u4FlagValue);	

		u8MarkFlagAddr = (UINT64)((UINT32)(u8NORPartWriteoffset & 0xFFFFFFFF) + _rNorEEPROM.u4EEPNORBlockNum / NOR_EEPROM_PARTITION_CNT * _rNorEEPROM.u4NORSectorSize - CURRENT_PARTITION_FLAG_ADDR_OFF_END);
		u8MarkFlagAddr |= (UINT64)(_rNorEEPROM.u4EEPNORPartId) << 32;

//		if((--u4FlagValue) == 0xffffffff)
		{
			--u4FlagValue;	
		}

		LOG(1, "Write cur partition flag: 0X%x,0X%x,0X%x\n", _rNorEEPROM.u4EEPNORPartId, (UINT32)(u8MarkFlagAddr & 0xFFFFFFFF), u4FlagValue);	

		NORPART_Write(u8MarkFlagAddr, (UINT32)(&u4FlagValue), CURRENT_PARTITION_FLAG_ADDR_OFF_END);		
		_rNorEEPROM.u4EEPNORPartCurAppendOffset = (UINT32)(u8NORPartWriteoffset & 0xFFFFFFFF) + _rNorEEPROM.u4EEPSize;

	}

	
	return 0;
}
#endif/*REPLACE_EEPROM_WITH_NOR*/

#ifndef CC_MTK_LOADER
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
//-------------------------------------------------------------------------
/** NORPART_IoCtl
 *  Erase selected partition, and allocate new physical block to this partition.
 *  @param  u8Offset        Selected partition and block start index to be erased.
 *  @param  u4SectNum    sector number.
 *  @retval 0	        Success
 *  @retval Others	Fail
 */
//-------------------------------------------------------------------------
#define BLK_DEV_IO_CTRL_TYPE_ENABLE_WP ((UINT32) 0x100)
#define CHR_DEV_IO_CTRL_TYPE_ENABLE_WP ((UINT32) 0x200)
INT32 NORPART_IoCtl(UINT32 u4IdType, UINT64 u8Data)
{
UINT16 u2Type,u2Id,u2DefId;
BOOL *bEnable;
    u2Type = (UINT16)u4IdType;
    u2Id = (UINT16)(u4IdType>>16);
    u2DefId = NORPART_PARTIAL_PROTECTION_ID;
    switch(u2Type)
    {
    case CHR_DEV_IO_CTRL_TYPE_ENABLE_WP:
        bEnable = (BOOL *)&u8Data;
        LOG(0, "NORPART IOCTL Id[%d] = %d,%d.\n", u2Id, *bEnable, _fgNorPartialProtection);
        ASSERT(u2Id < (UINT32)PART_MIDD_END);
        _rNorBlk.au4Protection[u2Id] = *bEnable;
        if(u2Id < u2DefId)
        {
            _fgNorPartialProtection = *bEnable;
        }
        break;
    default :
        break;
    }
    return 0;
}
#endif /* !CC_MTK_LOADER */

#else 
INT32 NORSTG_Read(UINT32 u4Pid, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    return ERR_NOT_SUPPORT_NOR;
}

#ifndef CONFIG_MTD_MT53XX_NOR
INT32 NORPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    return ERR_NOT_SUPPORT_NOR;	
}
#endif

INT32 NORPART_Erase(UINT64 u8Offset, UINT32 u4SectNum)
{
    return ERR_NOT_SUPPORT_NOR;	
}

INT32 NORPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    return ERR_NOT_SUPPORT_NOR;	
}

INT32 NORPART_GetMWPartNum(void)
{
    return ERR_NOT_SUPPORT_NOR;
}

INT32 NORPART_GetSectorTbl(SECTTBL_T *pt_sector_tbl, UINT32 u4Pid)
{
    return ERR_NOT_SUPPORT_NOR;	
}

UINT32 NORPART_GetPartSectorNum(UINT32 u4Pid)
{
    return ERR_NOT_SUPPORT_NOR;		
}
#ifdef REPLACE_EEPROM_WITH_NOR
INT32 NOR_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
	return ERR_NOT_SUPPORT_NOR;
}

INT32 NOR_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
	return ERR_NOT_SUPPORT_NOR;
}
#endif/*REPLACE_EEPROM_WITH_NOR*/
#endif/* CC_NOR_DISABLE */

