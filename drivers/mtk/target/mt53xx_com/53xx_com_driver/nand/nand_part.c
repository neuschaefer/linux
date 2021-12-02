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
 * $RCSfile: nand_part.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_part.c
 *  nand_part.c provide the nand partition information.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "nand_debug.h"
#include "nand_if.h"
#include "nand_part.h"

#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_rand.h"
#include "x_bim.h"
#include "drvcust_if.h"
#include "x_timer.h"
#include "x_hal_5381.h"

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static NANDPART_STRUCT_T _arNandPart[NANDPART_PARTNUM] =
{
    { 0, "Part 0", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 1, "Part 1", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 2, "Part 2", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 3, "Part 3", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 4, "Part 4", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 5, "Part 5", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 6, "Part 6", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 7, "Part 7", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 8, "Part 8", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    { 9, "Part 9", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {10, "Part10", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {11, "Part11", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {12, "Part12", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {13, "Part13", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {14, "Part14", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {15, "Part15", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {16, "Part16", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {17, "Part17", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {18, "Part18", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {19, "Part19", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {20, "Part20", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {21, "Part21", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {22, "Part22", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {23, "Part23", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {24, "Part24", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {25, "Part25", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {26, "Part26", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {27, "Part27", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {28, "Part28", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {29, "Part29", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {30, "Part30", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
    {31, "Part31", 0x0, 0x0, NANDPART_NONE, NANDPART_ATTRB_READWRITE, NULL, NULL },
};

typedef struct _NAND_PART_T
{
    UINT32 fgInitial;
    UINT32 u4PartSize[NANDPART_PARTNUM];
    UINT32 u4PartOffset[NANDPART_PARTNUM];
} NAND_PART_T;

// block buffer
static UINT32 *_pu4BlkBuf = NULL;

// virtual eeprom
typedef enum
{
    NAND_EEPROM_NONE,
    NAND_EEPROM_PARTA,
    NAND_EEPROM_PARTB,
} NAND_EEPROM_PART_T;

typedef struct _NAND_EEPROM_T
{
    BOOL fgInitial;
    UINT32 u4SDMPartId;
    UINT32 u4EEPPartIdA;
    UINT32 u4EEPPartIdB;
    UINT32 u4EEPPartOffA;
    UINT32 u4EEPPartOffB;
    UINT32 u4EEPPageNum;
    UINT32 u4EEPSize;
    
    UINT32 u4WriteCnt;
    
    UINT32 u4EEPPageA;
    UINT32 u4EEPPageB;
    
    NAND_EEPROM_PART_T eEEPPart;

    UINT8 *pu1EEPBuf;
} NAND_EEPROM_T;

static NAND_EEPROM_T _rNandEEPROM;

#define NAND_EEPBUF_WRITECNT    (*((UINT32 *)(_rNandEEPROM.pu1EEPBuf + _rNandEEPROM.u4EEPSize - 4)))

//-----------------------------------------------------------------------------
// Static functions

#if defined(CC_MTK_LOADER)

#define NANDPART_RW_ENTRY
#define NANDPART_RW_EXIT

void NANDPART_ShowPartition()
{
    UINT32 u4PartId = 0;
    UINT32 u4StaticPartLastID = 0;
    UINT32 u4Offset = 0;
    UINT32 u4PartSize = 0;
    UINT32 u4StaticSize = 0;
    UINT32 u4DynamicSize = 0;
    UINT32 u4DynamicUsedSize = 0;
    UINT32 u4DynamicReservedSize = 0;
	UINT8  u4Encryped=0;
    NANDDEV_T rNandDev;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "Cant get nand device info!\n");
        return;
    }
    
    Printf("\nNAND info:\n");
    if (rNandDev.u4PgSz % 0x400 == 0)
    {
        if (rNandDev.u4PgSz % 0x100000 == 0)
        {
            Printf("pagesize: %dM\n", rNandDev.u4PgSz/0x100000);
        }
        else
        {
            Printf("pagesize: %dK\n", rNandDev.u4PgSz/0x400);
        }
    }
    else
    {
        Printf("pagesize: %d\n", rNandDev.u4PgSz);
    }
        
    if (rNandDev.u4BlkSz % 0x400 == 0)
    {
        if (rNandDev.u4BlkSz % 0x100000 == 0)
        {
            Printf("blksize: %dM\n", rNandDev.u4BlkSz/0x100000);
        }
        else
        {
            Printf("blksize: %dK\n", rNandDev.u4BlkSz/0x400);
        }
    }
    else
    {
        Printf("blksize: %d\n", rNandDev.u4PgSz);
    }
    
    if (rNandDev.u4TotalSize % 0x400 == 0)
    {
        if (rNandDev.u4TotalSize % 0x100000 == 0)
        {
            Printf("chipsize: %dM\n", rNandDev.u4TotalSize/0x100000);
        }
        else
        {
            Printf("chipsize: %dK\n", rNandDev.u4TotalSize/0x400);
        }
    }
    else
    {
        Printf("chipsize: %d\n", rNandDev.u4PgSz);
    }
    
    Printf("\n");
    u4StaticPartLastID = DRVCUST_InitGet(eNANDFlashStaticPartitionLastID);
    
    for (u4PartId = 0; u4PartId <= u4StaticPartLastID; u4PartId++)
    {
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
		
        if (u4PartSize == 0)
        {
            Printf("Part:#%02d, offset:0x00000000, size:0x00000000,  none none\n", u4PartId);
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
                Printf("Part:#%02d, offset:0x%08x, size:0x%08x,  BDM  ENC\n", u4PartId, u4Offset, u4PartSize);
            }
            else
            {
                Printf("Part:#%02d, offset:0x%08x, size:0x%08x,  BDM  none\n", u4PartId, u4Offset, u4PartSize);
            }
        }
    }

    for ( ; u4PartId < NANDPART_PARTNUM; u4PartId++)
    {
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
		u4DynamicUsedSize += u4PartSize;
        if (u4PartSize == 0)
        {
            Printf("Part:#%02d, offset:0x00000000, size:0x00000000,  none none\n", u4PartId);
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
                Printf("Part:#%02d, offset:0x%08x, size:0x%08x,  SDM  ENC\n", u4PartId, u4Offset, u4PartSize);
            }
            else
            {
                Printf("Part:#%02d, offset:0x%08x, size:0x%08x,  SDM  none\n", u4PartId, u4Offset, u4PartSize);
            }
        }
    }

    u4StaticSize = DRVCUST_InitGet(eNANDFlashStaticPartitionSize);
    u4DynamicSize = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);

    Printf("\nPartitionSize info:\n");
    Printf("Total Partition size: %dM\n",(u4StaticSize+u4DynamicSize)/0x100000);
    
    if (u4StaticSize % 0x400 == 0)
    {
        if (u4StaticSize % 0x100000 == 0)
        {
            Printf("Total Static Partition size: %dM\n", u4StaticSize/0x100000);
        }
        else
        {
            Printf("Total Static Partition size: %dK\n", u4StaticSize/0x400);
        }
    }
    else
    {
        Printf("Total Static Partition size: %d\n", u4StaticSize);
    }
    
    if (u4DynamicUsedSize != 0)
    {
        u4DynamicReservedSize = u4DynamicSize - u4DynamicUsedSize;
        
        if (u4DynamicSize % 0x400 == 0)
        {
            if (u4DynamicSize % 0x100000 == 0)
            {
                Printf("Total Dynamic Partition size: %dM\n", u4DynamicSize/0x100000);
            }
            else
            {
                Printf("Total Dynamic Partition size: %dK\n", u4DynamicSize/0x400);
            }
        }
        else
        {
            Printf("Total Dynamic Partition size: %d\n", u4DynamicSize);
        }
        
        if (u4DynamicReservedSize % 0x400 == 0)
        {
            if (u4DynamicReservedSize % 0x100000 == 0)
            {
                Printf("Reserved size in Dynamic Partition: %dM\n", u4DynamicReservedSize/0x100000);
            }
            else
            {
                Printf("Reserved size in Dynamic Partition: %dK\n", u4DynamicReservedSize/0x400);
            }
        }
        else
        {
            Printf("Reserved size in Dynamic Partition: %d\n", u4DynamicReservedSize);
        }
    }
}

#ifndef NDEBUG  /*only valid in debug version*/

UINT32 NANDPART_Writer_NandStructure(void)
{
    NANDDEV_T rNandDev;
    UINT32 ui4_ret = 0;
    UINT32 u4StaticSize = 0;
    UINT32 u4DynamicSize = 0;    

    u4StaticSize = DRVCUST_InitGet(eNANDFlashStaticPartitionSize);
    u4DynamicSize = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);

    Printf("0x%x  ", u4StaticSize + u4DynamicSize); /*Total Partition size*/
    
    if (NAND_GetInfo(&rNandDev) == 0)
    {
        Printf("0x%x 0x%x 0x%x ", rNandDev.u4BlkSz, rNandDev.u4PgSz, rNandDev.u4OOBSz);
    }
    else
    {
        Printf("<block_size> <page_size> <oob_size> ");
        ui4_ret++;
    }
    
    return ui4_ret;
}



UINT32 NANDPART_Writer_NandLayout(void)
{
    NANDDEV_T rNandDev;
    UINT32 ui4_ret = 0;
    UINT32 u4PartId = 0;
    UINT32 u4StaticPartLastID = 0;
    UINT32 u4Offset = 0;
    UINT32 u4PartSize = 0;
    UINT32 u4PartSize_last_empty = 0;
    UINT8  u4Encryped=0;
   
    u4StaticPartLastID = DRVCUST_InitGet(eNANDFlashStaticPartitionLastID);

    if (u4StaticPartLastID == 0)
    {
        ui4_ret++;
    }

    if (NAND_GetInfo(&rNandDev) == 0)
    {
        if ((rNandDev.u4PgSz == 512) && (rNandDev.u4BBMode == 1))
            Printf("BBM0\n");
        else
            Printf("NONE\n");
    }
    else
    {
        Printf("NONE\n");
    }
        
    for (u4PartId = 0; u4PartId <= u4StaticPartLastID; u4PartId++)
    {
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
        
        if (u4PartSize != 0)
        {
#ifdef CC_MTD_ENCRYPT_SUPPORT
            u4Encryped = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + u4PartId));
#else
            u4Encryped = 0;
#endif
            if(u4Encryped)
            {
                Printf("<file_for_part_%02d>       ENC   BDM   0x%08x   0x%08x\n", u4PartId, u4Offset, u4PartSize);
            }
            else
            {
                Printf("<file_for_part_%02d>       NOE   BDM   0x%08x   0x%08x\n", u4PartId, u4Offset, u4PartSize);
            }
        }
    }

    for ( ; u4PartId < NANDPART_PARTNUM; u4PartId++)
    {
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
        
        if (u4PartSize != 0)
        {
#ifdef CC_MTD_ENCRYPT_SUPPORT
            u4Encryped = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + u4PartId));
#else
            u4Encryped = 0;
#endif
            if(u4Encryped)
            {
                Printf("<file_for_part_%02d>       ENC   SDM   0x%08x   0x%08x\n", u4PartId, u4Offset, u4PartSize);
            }
            else
            {
                Printf("<file_for_part_%02d>       NOE   SDM   0x%08x   0x%08x\n", u4PartId, u4Offset, u4PartSize);
            }

            if (u4PartSize_last_empty == 1)
            {
                ui4_ret++;
                u4PartSize_last_empty = 0;
            }
        }
        else
        {
            u4PartSize_last_empty = 1;
        }
    }
    
    return ui4_ret;
}

#endif


#else

static NAND_PART_T _rNandPartLayout;

static HANDLE_T _hRWSema   = NULL_HANDLE;
static HANDLE_T _hInitSema = NULL_HANDLE;

#define NANDPART_RW_ENTRY       {  NAND_ApiLock(&_hRWSema);      }
#define NANDPART_RW_EXIT        {  NAND_ApiUnLock(&_hRWSema);    }
#define NANDPART_INIT_ENTRY     {  NAND_ApiLock(&_hInitSema);    }
#define NANDPART_INIT_EXIT      {  NAND_ApiUnLock(&_hInitSema);  }

//-------------------------------------------------------------------------
/** _NANDPART_ParameterInit
*  NAND parameter initialization function.
*  @param  void.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
static INT32 _NANDPART_ParameterInit(void)
{
    static BOOL _fgNandParamInit = FALSE;
    UINT32 i, u4StaticPartLastID = 0, u4DynPartID = 0;
    NANDDEV_T rNandDev;

    NANDPART_INIT_ENTRY
    if (_fgNandParamInit)
    {
        NANDPART_INIT_EXIT
        return 0;
    }
    _fgNandParamInit = TRUE;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        NANDPART_INIT_EXIT
        return -1;
    }

    u4StaticPartLastID = DRVCUST_InitGet(eNANDFlashStaticPartitionLastID);
    u4DynPartID =  u4StaticPartLastID + 1;
    
    for (i = 0; i <= u4StaticPartLastID; i++)
    {
        _rNandPartLayout.u4PartSize[i] = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
        _rNandPartLayout.u4PartOffset[i] = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + i));

#if (!defined(__KERNEL__)) || (defined(CC_FORCE_ENABLE_BDM))
        if (_rNandPartLayout.u4PartSize[i])
        {
            if (NANDPART_Mount((UINT8)i, NANDPART_BDM, 
                    _rNandPartLayout.u4PartOffset[i], _rNandPartLayout.u4PartSize[i]) != 0)
            {
                LOG(0, "BDM partition[%d] mount fail!\n", i);
                ASSERT(0);
            }
            else
            {
                LOG(1, "BDM partition[%d] locate:0x%x, size:0x%x\n",
                    i, _rNandPartLayout.u4PartOffset[i], _rNandPartLayout.u4PartSize[i]);
            }
        }
#endif
    }

    _rNandPartLayout.u4PartSize[u4DynPartID] = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);
    _rNandPartLayout.u4PartOffset[u4DynPartID] = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4DynPartID));

    if (_rNandPartLayout.u4PartSize[u4DynPartID])
    {
        if(NANDPART_Mount((UINT8)u4DynPartID, NANDPART_SDM, 
                _rNandPartLayout.u4PartOffset[u4DynPartID], _rNandPartLayout.u4PartSize[u4DynPartID]) != 0)
        {
            LOG(0, "SDM partition[%d] mount fail!\n", u4DynPartID);
            ASSERT(0);
        }
        else
        {
            LOG(1, "SDM partition[%d] locate:0x%x, size:0x%x\n",
                u4DynPartID, _rNandPartLayout.u4PartOffset[u4DynPartID], _rNandPartLayout.u4PartSize[u4DynPartID]);
        }
    }

    NANDPART_INIT_EXIT
    return 0;
}

//-------------------------------------------------------------------------
/** NANDPART_GetMWPartNum
*  Return the partition numbers of Middleware. This function is call by char_mw.c only.
*  @param  void.
*  @return  total partition number in system.
*/
//-------------------------------------------------------------------------
INT32 NANDPART_GetMWPartNum(void)
{
    // number of middleware partition.
    return (INT32)NANDPART_PARTNUM;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_Init.
 *  Details of NANDPART_Init (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_Init(void)
{
    if (!_rNandPartLayout.fgInitial)
    {
        // initial nand part parameter.
        _NANDPART_ParameterInit();
        _rNandPartLayout.fgInitial = TRUE;
    }
    
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_ShowPartition.
 *  Details of NANDPART_ShowPartition (optional).
 */
//-----------------------------------------------------------------------------
void NANDPART_ShowPartition()
{
    UINT32 i;

    for (i = 0; i < NANDPART_PARTNUM; i++)
    {
        Printf("#%02d: name:%6s, offset:0x%08x, size:0x%08x, ", 
            i, _arNandPart[i].cName, _arNandPart[i].u4Offset, _arNandPart[i].u4Size);
            
        switch (_arNandPart[i].rType)
        {
        case NANDPART_NONE:
            Printf(" none type\n");
            break;
            
        case NANDPART_BDM:
            Printf(" BDM  type\n");
            break;
            
        case NANDPART_SDM:
            Printf(" SDM  type\n");
            break;
        }
    }
}

//-------------------------------------------------------------------------
/** NANDPART_GetMaxPartId
*  Return the number total used partitions.
*  @return   the number of used partition.
*/
//-------------------------------------------------------------------------
UINT32 NANDPART_GetMaxPartId()
{
    return NANDPART_PARTNUM;
}

//-------------------------------------------------------------------------
/** NORPART_GetPageSize
*  This function returns the size of the selected partition in bytes..
*  @param  u4PartId      selected partition.
*  @return   the size of selected partition.
*/
//-------------------------------------------------------------------------
UINT32 NANDPART_GetPageSize(UINT32 u4PartId)
{
    NANDDEV_T rNandDev;
    
    if (u4PartId >= (UINT32)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u4PartId);
        ASSERT(0);
        return 0;
    }
    
    NANDPART_Init();
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return 0;
    }
    
    return rNandDev.u4PgSz; // To sync size with middleware.
}

//-------------------------------------------------------------------------
/** NORPART_GetPartSize
*  This function returns the size of the selected partition in bytes..
*  @param  u4PartId      selected partition.
*  @return   the size of selected partition.
*/
//-------------------------------------------------------------------------
UINT32 NANDPART_GetPartSize(UINT32 u4PartId)
{
    UINT32 u4Size = 0;
     
    if (u4PartId >= (UINT32)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u4PartId);
        ASSERT(0);
        return 0;
    }

    NANDPART_Init();
    u4Size = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));

    return u4Size;
}

//-------------------------------------------------------------------------
/** NANDPART_GetSectorTbl
*  Return the erase sector maps of the selected partition.
*  @param  pt_sector_tbl    pointer to sector table information.
*  @param  u4PartId      selected partition.
*  @retval 0   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 NANDPART_GetSectorTbl(SECTTBL_T *pt_sector_tbl, UINT32 u4PartId)
{
    UINT32 u4Size;
    NANDDEV_T rNandDev;
    
    ASSERT(pt_sector_tbl != NULL);
    
    if (u4PartId >= (UINT32)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u4PartId);
        ASSERT(0);
        return 1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return 1;
    }
    
    if (pt_sector_tbl->u4SegNum < 1)
    {
        LOG(3, "pt_sector_tbl->u4SegNum = 0\n");
    }
    
    pt_sector_tbl->u4SegNum = 1;
    u4Size = NANDPART_GetPartSize(u4PartId);

    pt_sector_tbl->prSegInfo[0].u4SectNum = (u4Size + (rNandDev.u4BlkSz - 1)) / rNandDev.u4BlkSz;
    pt_sector_tbl->prSegInfo[0].u4SectSize = MIN(u4Size, rNandDev.u4BlkSz);
    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** Brief of NANDPART_Erase.
 *  Details of NANDPART_Erase (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_Erase(UINT8 u1PartId)
{
    NANDDEV_T rNandDev;
    UINT32 u4PartOffset, u4PartSize, u4DynPartId;
    UINT32 u4PgStart, u4PgEnd, u4PgCount;

    // Only BDM partition need erasure before upgrade
    u4DynPartId = DRVCUST_InitGet(eNANDFlashDynamicPartitionID);
    if (u1PartId >= (UINT8)u4DynPartId)
    {
        LOG(0, "NANDPART_Erase: Error PartId %d\n", u1PartId);
        ASSERT(0);
        return -1;
    }
    
    u4PartOffset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u1PartId));
    u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u1PartId));
    if (u4PartSize == 0)
    {
        LOG(0, "Part size zero!\n");
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "Cant get nand device info!\n");
        return -1;
    }
    
    u4PgStart = u4PartOffset / rNandDev.u4PgSz;
    u4PgEnd = (u4PartOffset + u4PartSize + rNandDev.u4PgSz - 1) / rNandDev.u4PgSz;
    u4PgCount = u4PgEnd - u4PgStart;

    LOG(0, "NANDPART_Erase: PartId=%d, offset=0x%x, size=0x%x\n", u1PartId, u4PartOffset, u4PartSize);
    if (NAND_Erase(u4PgStart, u4PgCount) != 0)
    {
        LOG(0, "part erase operates failed!\n");
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_GetPartStruct.
 *  Details of NANDPART_GetPartStruct (optional).
 */
//-----------------------------------------------------------------------------
NANDPART_STRUCT_T* NANDPART_GetPartStruct()
{
    return _arNandPart;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_IORequest.
 *  Details of NANDPART_IORequest (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_IORequest(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen, BOOL fgRead)
{
    INT32 i4Ret;

    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (_arNandPart[u1PartId].rType == NANDPART_NONE)
    {
        LOG(0, "This partition is not mount!\n");
        return -1;
    }

    if (fgRead)
    {
        ASSERT(_arNandPart[u1PartId].pfnNandRead != NULL);
        i4Ret = _arNandPart[u1PartId].pfnNandRead(u1PartId, u4Offset, u4MemPtr, u4MemLen);
    }
    else
    {
        ASSERT(_arNandPart[u1PartId].pfnNandWrite != NULL);
        i4Ret = _arNandPart[u1PartId].pfnNandWrite(u1PartId, u4Offset, u4MemPtr, u4MemLen);
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_Check.
 *  Check if partition offset and size is block alignment in debug version.
 */
//-----------------------------------------------------------------------------
static void NANDPART_Check(void)
{
    NANDDEV_T rNandDev;
    UINT32 u4PartId, u4Offset, u4Size, u4TotalSize;
    static BOOL fgchecked = FALSE;

    if (fgchecked)
    {
        return;
    }
    fgchecked = TRUE;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return;
    }

    for (u4PartId = 0; u4PartId < NANDPART_PARTNUM; u4PartId++)
    {
        u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
        if (u4Offset & (rNandDev.u4BlkSz - 1))
        {
           LOG(0, "NANDPART_Check ASSERT: part %d offset 0x%x is not block size aligned!\n", u4PartId, u4Offset);
           ASSERT(0);
           return;
        }
        
        u4Size = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
        if (u4Size & (rNandDev.u4BlkSz - 1))
        {
           LOG(0, "NANDPART_Check ASSERT: part %d Size 0x%x is not block size aligned!\n", u4PartId, u4Size);
           ASSERT(0);
           return;
        }
    }
    
    // The summation size of each partition must be less than total used flash size.
    u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + NANDPART_PARTNUM - 1));
    u4Size = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + NANDPART_PARTNUM - 1));
    u4TotalSize = DRVCUST_InitGet(eNANDFlashTotalSize);

    if (u4TotalSize & (rNandDev.u4BlkSz - 1))
    {
       LOG(0, "NANDPART_Check ASSERT: NAND_FLASH_TOTAL_SIZE 0x%x is not block size aligned!\n", u4TotalSize);
       ASSERT(0);
       return;
    }
    
    if (u4Offset + u4Size > u4TotalSize)
    {
        LOG(0, "NANDPART_Check ASSERT: NAND_FLASH_TOTAL_SIZE too small!");
        ASSERT(0);
        return;
    }
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_Mount.
 *  Details of NANDPART_Mount (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_Mount(UINT8 u1PartId, NANDPART_TYPE rType, UINT32 u4Offset, UINT32 u4Size)
{
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (_arNandPart[u1PartId].rType != NANDPART_NONE)
    {
        return 0;
    }

    NANDPART_Check();

    _arNandPart[u1PartId].u4Offset = u4Offset;
    _arNandPart[u1PartId].u4Size = u4Size;
    
    switch(rType)
    {
    case NANDPART_BDM:
        _arNandPart[u1PartId].rType = NANDPART_BDM;
        if (NAND_BDM_Mount(u1PartId) != 0)
        {
            NAND_BDM_UnMount(u1PartId);
            return -1;
        }

        _arNandPart[u1PartId].pfnNandRead = NAND_BDM_ReadPages;
        _arNandPart[u1PartId].pfnNandWrite = NAND_BDM_WriteBlocks;
        break;

    case NANDPART_SDM:
        _arNandPart[u1PartId].rType = NANDPART_SDM;
        if (NAND_SDM_Mount(u1PartId) != 0)
        {
            NAND_SDM_UnMount(u1PartId);
            return -1;
        }

        _arNandPart[u1PartId].pfnNandRead = NAND_SDM_ReadPagesEx;
        _arNandPart[u1PartId].pfnNandWrite = NAND_SDM_WriteBlocks;
        break;

    case NANDPART_NONE:
    default:
        LOG(0, "Unknown partition type!\n");
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_UnMount.
 *  Details of NANDPART_UnMount (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_UnMount(UINT8 u1PartId)
{
    NANDPART_TYPE rType;
    
#ifdef __KERNEL__
    printk(KERN_CRIT "NANDPART_UnMount: Partition ID = %d\n", u1PartId);
#else
    LOG(0, "NANDPART_UnMount: Partition ID = %d\n", u1PartId);
#endif

    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }
    
    rType = _arNandPart[u1PartId].rType;
    if (rType == NANDPART_NONE)
    {
        return 0;
    }

    _arNandPart[u1PartId].rType        = NANDPART_NONE;
    _arNandPart[u1PartId].pfnNandRead  = NULL;
    _arNandPart[u1PartId].pfnNandWrite = NULL;
    _arNandPart[u1PartId].u4Offset     = 0x0;
    _arNandPart[u1PartId].u4Size       = 0x0;

    switch(rType)
    {
    case NANDPART_BDM:
        NAND_BDM_UnMount(u1PartId);
        break;
        
    case NANDPART_SDM:
        NAND_SDM_UnMount(u1PartId);
        break;
        
    default:
        LOG(0, "Unknown partition type!\n");
        return -1;
    }
	
    if(_pu4BlkBuf != NULL)
    {
        x_mem_free(_pu4BlkBuf);
        _pu4BlkBuf = NULL;
    }
    
    return 0;
}

static INT32 _NAND_Read_EEPROMData(NAND_EEPROM_PART_T eEEPPart, UINT32 u4PageIdx)
{
    NANDDEV_T rNandDev;
    UINT32 u4PartOffset, u4ReadSize;

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return -1;
    }
    
    if (eEEPPart == NAND_EEPROM_PARTA)
    {
        u4PartOffset = _rNandEEPROM.u4EEPPartOffA;
    }
    else if (eEEPPart == NAND_EEPROM_PARTB)
    {
        u4PartOffset = _rNandEEPROM.u4EEPPartOffB;
    }
    else
    {
        ASSERT(0);
        return -1;
    }

    u4ReadSize = (_rNandEEPROM.u4EEPSize < rNandDev.u4PgSz) ? rNandDev.u4PgSz : _rNandEEPROM.u4EEPSize;
    if (NAND_SDM_ReadPages(_rNandEEPROM.u4SDMPartId, u4PartOffset, u4PageIdx, 
            (UINT32)_rNandEEPROM.pu1EEPBuf, u4ReadSize) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "_NAND_Read_EEPROMData fail!\n");
#else
        LOG(0, "_NAND_Read_EEPROMData fail!\n");
#endif
        return -1;
    }
    
    return 0;
}

static INT32 _NAND_Write_EEPROMData(NAND_EEPROM_PART_T eEEPPart)
{
    NANDDEV_T rNandDev;
    UINT32 u4PartOffset, u4PageIdx, u4WriteSize;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return -1;
    }

    if (eEEPPart == NAND_EEPROM_PARTA)
    {
        u4PartOffset = _rNandEEPROM.u4EEPPartOffA;
        u4PageIdx = _rNandEEPROM.u4EEPPageA;
    }
    else if (eEEPPart == NAND_EEPROM_PARTB)
    {
        u4PartOffset = _rNandEEPROM.u4EEPPartOffB;
        u4PageIdx = _rNandEEPROM.u4EEPPageB;
    }
    else
    {
        ASSERT(0);
        return -1;
    }

    if (u4PageIdx == 0xFFFFFFFF)
    {
        u4PageIdx = 0;
    }
    else
    {
        u4PageIdx = (u4PageIdx + _rNandEEPROM.u4EEPPageNum) % rNandDev.u4PgPerBlk;
    }
    
    u4WriteSize = (_rNandEEPROM.u4EEPSize < rNandDev.u4PgSz) ? rNandDev.u4PgSz : _rNandEEPROM.u4EEPSize;
    if (NAND_SDM_WritePages(_rNandEEPROM.u4SDMPartId, u4PartOffset, &u4PageIdx, 
            (UINT32)_rNandEEPROM.pu1EEPBuf, u4WriteSize) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "_NAND_Write_EEPROMData fail!\n");
#else
        LOG(0, "_NAND_Write_EEPROMData fail!\n");
#endif
        return -1;
    }

    if (eEEPPart == NAND_EEPROM_PARTA)
    {
        _rNandEEPROM.u4EEPPageA = u4PageIdx;
        _rNandEEPROM.eEEPPart = NAND_EEPROM_PARTA;
    }
    else
    {
        _rNandEEPROM.u4EEPPageB = u4PageIdx;
        _rNandEEPROM.eEEPPart = NAND_EEPROM_PARTB;
    }
    
    return 0;
}

static INT32 _NAND_Find_EEPROMData(void)
{
    INT32 i4PageIdx;
    NANDDEV_T rNandDev;
    UINT32 u4WriteCntA = 0xFFFFFFFF, u4WriteCntB = 0xFFFFFFFF;

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return -1;
    }
    
    for (i4PageIdx = rNandDev.u4PgPerBlk - _rNandEEPROM.u4EEPPageNum; i4PageIdx >= 0; i4PageIdx -= _rNandEEPROM.u4EEPPageNum)
    {
        if (_NAND_Read_EEPROMData(NAND_EEPROM_PARTA, (UINT32)i4PageIdx) != 0)
        {
            break;
        }
        
        if (NAND_EEPBUF_WRITECNT == 0xFFFFFFFF)
        {
            continue;
        }
            
        u4WriteCntA = NAND_EEPBUF_WRITECNT;
        _rNandEEPROM.u4EEPPageA = (UINT32)i4PageIdx;
        break;
    }
    
    for (i4PageIdx = rNandDev.u4PgPerBlk - _rNandEEPROM.u4EEPPageNum; i4PageIdx >= 0; i4PageIdx -= _rNandEEPROM.u4EEPPageNum)
    {
        if (_NAND_Read_EEPROMData(NAND_EEPROM_PARTB, (UINT32)i4PageIdx) != 0)
        {
            break;
        }
        
        if (NAND_EEPBUF_WRITECNT == 0xFFFFFFFF)
        {
            continue;
        }
            
        u4WriteCntB = NAND_EEPBUF_WRITECNT;
        _rNandEEPROM.u4EEPPageB = (UINT32)i4PageIdx;
        break;
    }

    if (u4WriteCntA != 0xFFFFFFFF && u4WriteCntB != 0xFFFFFFFF)
    {
        if (u4WriteCntA > u4WriteCntB)
        {
            _rNandEEPROM.eEEPPart = NAND_EEPROM_PARTA;
            _rNandEEPROM.u4WriteCnt = u4WriteCntA;
        }
        else
        {
            _rNandEEPROM.eEEPPart = NAND_EEPROM_PARTB;
            _rNandEEPROM.u4WriteCnt = u4WriteCntB;
        }
    }
    else if (u4WriteCntA != 0xFFFFFFFF)
    {
        _rNandEEPROM.eEEPPart = NAND_EEPROM_PARTA;
        _rNandEEPROM.u4WriteCnt = u4WriteCntA;
    }
    else if (u4WriteCntB != 0xFFFFFFFF)
    {
        _rNandEEPROM.eEEPPart = NAND_EEPROM_PARTB;
        _rNandEEPROM.u4WriteCnt = u4WriteCntB;
    }
    else
    {
        _rNandEEPROM.eEEPPart = NAND_EEPROM_NONE;
        _rNandEEPROM.u4WriteCnt = 0xFFFFFFFF;
    }

    if (_rNandEEPROM.eEEPPart == NAND_EEPROM_PARTA)
    {
        if (_NAND_Read_EEPROMData(NAND_EEPROM_PARTA, _rNandEEPROM.u4EEPPageA) != 0)
        {
            return -1;
        }
        
        LOG(1, "_NAND_Find_EEPROMData: Find EEPROM data in partition A, page = 0x%x\n", _rNandEEPROM.u4EEPPageA);
    }
    else if (_rNandEEPROM.eEEPPart == NAND_EEPROM_PARTB)
    {
        if (_NAND_Read_EEPROMData(NAND_EEPROM_PARTB, _rNandEEPROM.u4EEPPageB) != 0)
        {
            return -1;
        }
        LOG(1, "_NAND_Find_EEPROMData: Find EEPROM data in partition B, page = 0x%x\n", _rNandEEPROM.u4EEPPageB);
    }
    else
    {
        x_memset((void *)_rNandEEPROM.pu1EEPBuf, 0xFF, _rNandEEPROM.u4EEPSize);
        LOG(0, "_NAND_Find_EEPROMData: Can not Find valid EEPROM data!\n");
    }
    
    return 0;
}

static INT32 _NAND_EEPROM_Init(void)
{
    UINT32 i, u4PartSizeA, u4PartSizeB;
    NANDDEV_T rNandDev;

    if (_rNandEEPROM.fgInitial)
    {
        return 0;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return -1;
    }
    
    _rNandEEPROM.u4EEPSize    = DRVCUST_InitGet(eSystemEepromSize);
    _rNandEEPROM.u4SDMPartId  = DRVCUST_InitGet(eNANDFlashDynamicPartitionID);
    _rNandEEPROM.u4EEPPartIdA = DRVCUST_InitGet(eNANDFlashPartIdEepromA);
    _rNandEEPROM.u4EEPPartIdB = DRVCUST_InitGet(eNANDFlashPartIdEepromB);
    
    _rNandEEPROM.pu1EEPBuf = (UINT8 *)x_mem_alloc_virtual(_rNandEEPROM.u4EEPSize);
    if (_rNandEEPROM.pu1EEPBuf == NULL)
    {
        LOG(0, "_NAND_EEPROM_Init Can't allocate memory!\n");
        ASSERT(0);
        return -1;
    }
    
    u4PartSizeA = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + _rNandEEPROM.u4EEPPartIdA));
    u4PartSizeB = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + _rNandEEPROM.u4EEPPartIdB));
    if ((u4PartSizeA != rNandDev.u4BlkSz) || (u4PartSizeB != rNandDev.u4BlkSz))
    {
        LOG(0, "virtual eeprom partition size must be a block of nand flash!\n");
        ASSERT(0);
        return -1;
    }
    
#if defined(CC_MTK_LOADER)
    {
        UINT32 u4PartOffset, u4PartSize;
        u4PartOffset = DRVCUST_InitGet(eNANDFlashStaticPartitionSize);
        u4PartSize = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);
        NANDPART_Mount(_rNandEEPROM.u4SDMPartId, NANDPART_SDM, u4PartOffset, u4PartSize);
    }
#else
    VERIFY(NANDPART_Init() == 0);
#endif

    _rNandEEPROM.u4WriteCnt = 0xFFFFFFFF;
    _rNandEEPROM.u4EEPPageNum = 
        (_rNandEEPROM.u4EEPSize < rNandDev.u4PgSz) ? 1 : _rNandEEPROM.u4EEPSize / rNandDev.u4PgSz;
    
    _rNandEEPROM.eEEPPart = NAND_EEPROM_NONE;
    _rNandEEPROM.u4EEPPageA = 0xFFFFFFFF;
    _rNandEEPROM.u4EEPPageB = 0xFFFFFFFF;
    
    for (i = _rNandEEPROM.u4SDMPartId; i < _rNandEEPROM.u4EEPPartIdA; i++)
    {
        _rNandEEPROM.u4EEPPartOffA += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
    }

    for (i = _rNandEEPROM.u4SDMPartId; i < _rNandEEPROM.u4EEPPartIdB; i++)
    {
        _rNandEEPROM.u4EEPPartOffB += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
    }

    x_memset((void *)_rNandEEPROM.pu1EEPBuf, 0xFF, _rNandEEPROM.u4EEPSize);
    
    _NAND_Find_EEPROMData();
    _rNandEEPROM.fgInitial = TRUE;
    return 0;
}

INT32 NAND_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    
    NANDPART_RW_ENTRY
    VERIFY(_NAND_EEPROM_Init() == 0);

    if (_arNandPart[_rNandEEPROM.u4SDMPartId].rType == NANDPART_NONE)
    {
        LOG(0, "NAND_EEPROM_Read: This partition is not mount!\n");
        
        ASSERT(0);
        NANDPART_RW_EXIT
        return -1;
    }

    if ((u4Offset >= _rNandEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rNandEEPROM.u4EEPSize))
    {
        LOG(0, "NAND_EEPROM_Read: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        NANDPART_RW_EXIT
        return -1;
    }
    
    if (_rNandEEPROM.eEEPPart == NAND_EEPROM_NONE)
    {
        x_memset((void *)u4MemPtr, 0xFF, u4MemLen);
    }
    else
    {
        if (_rNandEEPROM.eEEPPart == NAND_EEPROM_PARTA)
        {
            i4Ret = _NAND_Read_EEPROMData(NAND_EEPROM_PARTA, _rNandEEPROM.u4EEPPageA);
        }
        else
        {
            i4Ret = _NAND_Read_EEPROMData(NAND_EEPROM_PARTB, _rNandEEPROM.u4EEPPageB);
        }

        if (i4Ret == 0)
        {
            x_memcpy((void *)u4MemPtr, (void *)(_rNandEEPROM.pu1EEPBuf + u4Offset), u4MemLen);
        }
    }
    
    NANDPART_RW_EXIT
    return i4Ret;
}

INT32 NAND_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;
    
    NANDPART_RW_ENTRY

    VERIFY(_NAND_EEPROM_Init() == 0);
    
    if (_arNandPart[_rNandEEPROM.u4SDMPartId].rType == NANDPART_NONE)
    {
        LOG(0, "NAND_EEPROM_Write: This partition is not mount!\n");
        
        ASSERT(0);
        NANDPART_RW_EXIT
        return -1;
    }
    
    if ((u4Offset >= _rNandEEPROM.u4EEPSize) || ((u4Offset + u4MemLen) > _rNandEEPROM.u4EEPSize))
    {
        LOG(0, "NAND_EEPROM_Write: out of boundry! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
        
        ASSERT(0);
        NANDPART_RW_EXIT
        return -1;
    }
    
    x_memcpy((void *)(_rNandEEPROM.pu1EEPBuf + u4Offset), (void *)u4MemPtr, u4MemLen);
    
    if (_rNandEEPROM.u4WriteCnt == 0xFFFFFFFF)
    {
        _rNandEEPROM.u4WriteCnt = 1;
    }
    else
    {
        _rNandEEPROM.u4WriteCnt++;
    }
    NAND_EEPBUF_WRITECNT = _rNandEEPROM.u4WriteCnt;
    
    LOG(1, "NAND_EEPROM_Write: offset = 0x%x, memptr = 0x%x, u4MemLen = 0x%x, writecnt = 0x%x, partition %c, page = 0x%x\n", 
        u4Offset, u4MemPtr, u4MemLen, _rNandEEPROM.u4WriteCnt,
        (_rNandEEPROM.eEEPPart == NAND_EEPROM_PARTA) ? 'B' : 'A', 
        (_rNandEEPROM.eEEPPart == NAND_EEPROM_PARTA) ? _rNandEEPROM.u4EEPPageB : _rNandEEPROM.u4EEPPageA);

    // Write eeprom data to another partition.
    if (_rNandEEPROM.eEEPPart == NAND_EEPROM_PARTA)
    {
        i4Ret = _NAND_Write_EEPROMData(NAND_EEPROM_PARTB);
    }
    else
    {
        i4Ret = _NAND_Write_EEPROMData(NAND_EEPROM_PARTA);
    }
    
    NANDPART_RW_EXIT
    return i4Ret;
}

BOOL NANDPART_IsEEPROM(UINT32 u4PartId)
{
    UINT32 u4DynPartId = DRVCUST_InitGet(eNANDFlashDynamicPartitionID);
    
    if (u4PartId >= u4DynPartId)
    {
        UINT32 u4EEPPartIdA, u4EEPPartIdB;
        
        u4EEPPartIdA = DRVCUST_InitGet(eNANDFlashPartIdEepromA);
        u4EEPPartIdB = DRVCUST_InitGet(eNANDFlashPartIdEepromB);

        if ((u4PartId == u4EEPPartIdA) || (u4PartId == u4EEPPartIdB))
        {
            return TRUE;
        }
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_Read.
 *  Details of NANDPART_Read (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4PartId = (UINT32)((u8Offset >> (UINT64)32) & 0xFFFF);
    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    UINT32 u4RestRdSize = u4MemLen;
    UINT32 u4RdMemPtr = u4MemPtr;
    
    UINT32 u4DynPartId = DRVCUST_InitGet(eNANDFlashDynamicPartitionID);
    UINT32 u4RdPgOffset, u4OffsetInPg;
    UINT32 u4RdSize;
    NANDDEV_T rNandDev;
    
#if defined(CC_MTK_LOADER)
    NANDPART_TYPE rType;
    UINT32 u4PartOffset, u4PartSize;
#endif

    if (u4PartId >= NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u4PartId);
        ASSERT(0);
        return -1;
    }

    // virtual eeprom read
    if (NANDPART_IsEEPROM(u4PartId))
    {
        UINT32 u4EEPSize = DRVCUST_InitGet(eSystemEepromSize);
        
        if ((u4Offset != 0) || (u4MemLen != u4EEPSize))
        {
            LOG(0, "NANDPART_Read: Read eeprom parameter error, u4Offset = 0x%x, u4MemLen = 0x%x!\n", 
                u4Offset, u4MemLen);
            
            ASSERT(0);
            return -1;
        }
        
        return NAND_EEPROM_Read(0, u4MemPtr, u4EEPSize - 4);
    }

    NANDPART_RW_ENTRY

    if (u4PartId >= u4DynPartId)
    {
        UINT32 i;
        
        for (i = u4DynPartId; i < u4PartId; i++)
        {
            u4Offset += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
        }
        
        u4PartId = u4DynPartId;
    }
    
#if defined(CC_MTK_LOADER)
    if (u4PartId == u4DynPartId)
    {
        rType = NANDPART_SDM;
        u4PartOffset = DRVCUST_InitGet(eNANDFlashStaticPartitionSize);
        u4PartSize = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);
    }
    else
    {
        rType = NANDPART_BDM;
        u4PartOffset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
    }
    
    NANDPART_Mount(u4PartId, rType, u4PartOffset, u4PartSize);
#else
    VERIFY(NANDPART_Init() == 0);

    LOG(7, "NANDPART_Read: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
    ASSERT(_rNandPartLayout.fgInitial != 0);
#endif

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        NANDPART_RW_EXIT
        return -1;
    }
    
    if(_pu4BlkBuf == NULL)
    {
        _pu4BlkBuf = (UINT32 *)x_mem_alloc_virtual(rNandDev.u4BlkSz);
        if (_pu4BlkBuf == NULL)
        {
            LOG(0, "NANDPART_Read Can't allocate memory!\n");
            NANDPART_RW_EXIT
            return -1;
        }
    }

    u4OffsetInPg = u4Offset & (rNandDev.u4PgSz - 1);
    u4RdPgOffset = u4Offset - u4OffsetInPg;
    
    // Handle offset non-page-size-aligned case
    if (u4OffsetInPg)
    {
        if (NANDPART_IORequest((UINT8)u4PartId, u4RdPgOffset, (UINT32)_pu4BlkBuf, rNandDev.u4PgSz, TRUE))
        {
#ifdef __KERNEL__
            printk(KERN_ERR "NANDPART_Read ASSERT1: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Read ASSERT1: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }

        if (u4RestRdSize >= (rNandDev.u4PgSz - u4OffsetInPg))
        {
            u4RdSize = rNandDev.u4PgSz - u4OffsetInPg;
        }
        else
        {
            u4RdSize = u4RestRdSize;
        }
        
        x_memcpy((void *)u4RdMemPtr, (const void *)((UINT8 *)_pu4BlkBuf + u4OffsetInPg), u4RdSize);
        u4RestRdSize -= u4RdSize;
        u4RdMemPtr += u4RdSize;
        u4RdPgOffset += rNandDev.u4PgSz;
    }
    
	u4RdSize = u4RestRdSize - (u4RestRdSize & (rNandDev.u4PgSz - 1));
    
    // Handle offset & size page-size-aligned case
    if (u4RdSize)
    {
        if (NANDPART_IORequest((UINT8)u4PartId, u4RdPgOffset, u4RdMemPtr, u4RdSize, TRUE))
        {
#ifdef __KERNEL__
            printk(KERN_ERR "NANDPART_Read ASSERT2: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Read ASSERT2: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }
        
        u4RestRdSize -= u4RdSize;
        u4RdMemPtr += u4RdSize;
        u4RdPgOffset += u4RdSize;
    }
    
    // Handle size non-page-size-aligned case
    if (u4RestRdSize)
    {
        if (NANDPART_IORequest((UINT8)u4PartId, u4RdPgOffset, (UINT32)_pu4BlkBuf, rNandDev.u4PgSz, TRUE))
        {
#ifdef __KERNEL__
            printk(KERN_ERR "NANDPART_Read ASSERT3: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Read ASSERT3: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }
        
        x_memcpy((void *)u4RdMemPtr, (const void *)_pu4BlkBuf, u4RestRdSize);
    }
    
    NANDPART_RW_EXIT
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NANDPART_Write.
 *  Details of NANDPART_Write (optional).
 */
//-----------------------------------------------------------------------------
INT32 NANDPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    UINT32 u4PartId = (UINT32)((u8Offset >> (UINT64)32) & 0xFFFF);
    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    UINT32 u4RestWtSize  = u4MemLen;
    UINT32 u4WtMemPtr = u4MemPtr;
    
    UINT32 u4DynPartId = DRVCUST_InitGet(eNANDFlashDynamicPartitionID);
    UINT32 u4WtBlkOffset, u4OffsetInBlk;
    NANDDEV_T rNandDev;
    
#if defined(CC_MTK_LOADER)
    NANDPART_TYPE rType;
    UINT32 u4PartOffset, u4PartSize;
#endif

    if (u4PartId >= NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u4PartId);
        ASSERT(0);
        return -1;
    }
    
    // virtual eeprom upgrade
    if (NANDPART_IsEEPROM(u4PartId))
    {
        UINT32 u4EEPSize = DRVCUST_InitGet(eSystemEepromSize);
        
        if ((u4Offset != 0) || (u4MemLen != u4EEPSize))
        {
            LOG(0, "NANDPART_Write: Upgrade eeprom parameter error, u4Offset = 0x%x, u4MemLen = 0x%x!\n", 
                u4Offset, u4MemLen);
            
            ASSERT(0);
            return -1;
        }
        
        return NAND_EEPROM_Write(0, u4MemPtr, u4EEPSize - 4);
    }

    NANDPART_RW_ENTRY

    if (u4PartId >= u4DynPartId)
    {
        UINT32 i;
        
        for (i = u4DynPartId; i < u4PartId; i++)
        {
            u4Offset += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
        }
        
        u4PartId = u4DynPartId;
    }
    else
    {
        // Erase BDM partition before upgrade.
        if (u4Offset == 0)
        {
            NANDPART_Erase((UINT8)u4PartId);
        }
    }
    
#if defined(CC_MTK_LOADER)
    if (u4PartId == u4DynPartId) 
    {
        rType = NANDPART_SDM;
        u4PartOffset = DRVCUST_InitGet(eNANDFlashStaticPartitionSize);
        u4PartSize = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);
    }
    else
    {
        rType = NANDPART_BDM;
        u4PartOffset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
    }
    
    NANDPART_Mount(u4PartId, rType, u4PartOffset, u4PartSize);
#else
    VERIFY(NANDPART_Init() == 0);

    LOG(7, "NANDPART_Write: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
    ASSERT(_rNandPartLayout.fgInitial != 0);
#endif

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        NANDPART_RW_EXIT
        return -1;
    }
    
    if(_pu4BlkBuf == NULL)
    {
        _pu4BlkBuf = (UINT32 *)x_mem_alloc_virtual(rNandDev.u4BlkSz);
        if (_pu4BlkBuf == NULL)
        {
            LOG(0, "NANDPART_Write Can't allocate memory!\n");
            NANDPART_RW_EXIT
            return -1;
        }
    }

    u4OffsetInBlk = u4Offset & (rNandDev.u4BlkSz - 1);
    u4WtBlkOffset = u4Offset - u4OffsetInBlk;
    
    // Handle offset non-block-size-aligned case
    if (u4OffsetInBlk)
    {
        UINT32 u4WtSize;
        
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, TRUE))
        {
#ifdef __KERNEL__
           printk(KERN_ERR "NANDPART_Write ASSERT1: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Write ASSERT1: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           //NANDPART_RW_EXIT
           //return -1;
           
            x_memset((void *)_pu4BlkBuf, 0xFF, rNandDev.u4BlkSz);
        }
        
        if (u4RestWtSize >= (rNandDev.u4BlkSz - u4OffsetInBlk))
        {
            u4WtSize = rNandDev.u4BlkSz - u4OffsetInBlk;
        }
        else
        {
            u4WtSize = u4RestWtSize;
        }

        x_memcpy((void *)((UINT8 *)_pu4BlkBuf + u4OffsetInBlk), (const void *)u4WtMemPtr, u4WtSize);
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, FALSE))
        {
#ifdef __KERNEL__
           printk(KERN_ERR "NANDPART_Write ASSERT2: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Write ASSERT2: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }

        u4RestWtSize -= u4WtSize;
        u4WtMemPtr += u4WtSize;
        u4WtBlkOffset += rNandDev.u4BlkSz;
    }
    
    // Handle offset & size block-size-aligned case
    while (u4RestWtSize >= rNandDev.u4BlkSz)
    {
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, u4WtMemPtr, rNandDev.u4BlkSz, FALSE))
        {
#ifdef __KERNEL__
           printk(KERN_ERR "NANDPART_Write ASSERT3: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Write ASSERT3: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }
        
        u4RestWtSize -= rNandDev.u4BlkSz;
        u4WtMemPtr += rNandDev.u4BlkSz;
        u4WtBlkOffset += rNandDev.u4BlkSz;
    }
    
    // Handle size none-block-size-aligned case
    if (u4RestWtSize)
    {
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, TRUE))
        {
#ifdef __KERNEL__
           printk(KERN_ERR "NANDPART_Write ASSERT4: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Write ASSERT4: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           //NANDPART_RW_EXIT
           //return -1;
           
            x_memset((void *)_pu4BlkBuf, 0xFF, rNandDev.u4BlkSz);
        }
        
        x_memcpy((void *)_pu4BlkBuf, (const void *)u4WtMemPtr, u4RestWtSize);
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, FALSE))
        {
#ifdef __KERNEL__
           printk(KERN_ERR "NANDPART_Write ASSERT5: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_Write ASSERT5: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }
    }
    
    NANDPART_RW_EXIT
    return 0;
}

INT32 NANDPART_BulkWrite(UINT64 u8Offset, UINT8 u1Val, UINT32 u4MemLen)
{
    UINT32 u4PartId = (UINT32)((u8Offset >> (UINT64)32) & 0xFFFF);
    UINT32 u4Offset = (UINT32)u8Offset & 0xFFFFFFFF;
    UINT32 u4RestWtSize  = u4MemLen;
    
    UINT32 u4DynPartId = DRVCUST_InitGet(eNANDFlashDynamicPartitionID);
    UINT32 u4WtBlkOffset, u4OffsetInBlk;
    NANDDEV_T rNandDev;
    
#if defined(CC_MTK_LOADER)
    NANDPART_TYPE rType;
    UINT32 u4PartOffset, u4PartSize;
#endif

    if (u4PartId >= NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u4PartId);
        ASSERT(0);
        return -1;
    }
    
    NANDPART_RW_ENTRY

    if (u4PartId >= u4DynPartId)
    {
        UINT32 i;
        
        for (i = u4DynPartId; i < u4PartId; i++)
        {
            u4Offset += DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
        }
        
        u4PartId = u4DynPartId;
    }
    
#if defined(CC_MTK_LOADER)
    if (u4PartId == u4DynPartId) 
    {
        rType = NANDPART_SDM;
        u4PartOffset = DRVCUST_InitGet(eNANDFlashStaticPartitionSize);
        u4PartSize = DRVCUST_InitGet(eNANDFlashDynamicPartitionSize);
    }
    else
    {
        rType = NANDPART_BDM;
        u4PartOffset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + u4PartId));
        u4PartSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + u4PartId));
    }
    
    NANDPART_Mount(u4PartId, rType, u4PartOffset, u4PartSize);
#else
    VERIFY(NANDPART_Init() == 0);

    LOG(7, "NANDPART_BulkWrite: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
    ASSERT(_rNandPartLayout.fgInitial != 0);
#endif

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        NANDPART_RW_EXIT
        return -1;
    }
    
    if(_pu4BlkBuf == NULL)
    {
        _pu4BlkBuf = (UINT32 *)x_mem_alloc_virtual(rNandDev.u4BlkSz);
        if (_pu4BlkBuf == NULL)
        {
            LOG(0, "NANDPART_BulkWrite Can't allocate memory!\n");
            NANDPART_RW_EXIT
            return -1;
        }
    }

    u4OffsetInBlk = u4Offset & (rNandDev.u4BlkSz - 1);
    u4WtBlkOffset = u4Offset - u4OffsetInBlk;
    
    // Handle offset non-block-size-aligned case
    if (u4OffsetInBlk)
    {
        UINT32 u4WtSize;
        
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, TRUE))
        {
#ifdef __KERNEL__
           printk(KERN_CRIT "NANDPART_BulkWrite ASSERT1: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_BulkWrite ASSERT1: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           //NANDPART_RW_EXIT
           //return -1;
           
            x_memset((void *)_pu4BlkBuf, 0xFF, rNandDev.u4BlkSz);
        }
        
        if (u4RestWtSize >= (rNandDev.u4BlkSz - u4OffsetInBlk))
        {
            u4WtSize = rNandDev.u4BlkSz - u4OffsetInBlk;
        }
        else
        {
            u4WtSize = u4RestWtSize;
        }

        x_memset((void *)((UINT8 *)_pu4BlkBuf + u4OffsetInBlk), u1Val, u4WtSize);
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, FALSE))
        {
#ifdef __KERNEL__
           printk(KERN_CRIT "NANDPART_BulkWrite ASSERT2: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_BulkWrite ASSERT2: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }

        u4RestWtSize -= u4WtSize;
        u4WtBlkOffset += rNandDev.u4BlkSz;
    }
    
    // Handle offset block-size-aligned case
    while (u4RestWtSize >= rNandDev.u4BlkSz)
    {
        x_memset((void *)_pu4BlkBuf, u1Val, rNandDev.u4BlkSz);
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, FALSE))
        {
#ifdef __KERNEL__
           printk(KERN_CRIT "NANDPART_BulkWrite ASSERT3: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_BulkWrite ASSERT3: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }
        
        u4RestWtSize -= rNandDev.u4BlkSz;
        u4WtBlkOffset += rNandDev.u4BlkSz;
    }
    
    // Handle write size none-block-size-aligned case
    if (u4RestWtSize)
    {
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, TRUE))
        {
#ifdef __KERNEL__
           printk(KERN_CRIT "NANDPART_BulkWrite ASSERT4: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_BulkWrite ASSERT4: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
            //ASSERT(0);
            //NANDPART_RW_EXIT
            //return -1;
            
            x_memset((void *)_pu4BlkBuf, 0xFF, rNandDev.u4BlkSz);
        }
        
        x_memset((void *)_pu4BlkBuf, u1Val, u4RestWtSize);
        if (NANDPART_IORequest((UINT8)u4PartId, u4WtBlkOffset, (UINT32)_pu4BlkBuf, rNandDev.u4BlkSz, FALSE))
        {
#ifdef __KERNEL__
           printk(KERN_CRIT "NANDPART_BulkWrite ASSERT5: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#else
           LOG(0, "NANDPART_BulkWrite ASSERT5: Partition ID = %d, Offset = 0x%x\n", u4PartId, u4Offset);
#endif
           //ASSERT(0);
           NANDPART_RW_EXIT
           return -1;
        }
    }
    
    NANDPART_RW_EXIT
    return 0;
}

INT32 Loader_GetNandFlashBlockSize(void)
{
    NANDDEV_T rNandDev;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "Loader_GetNandFlashBlockSize get nand info failed!\n");
        return -1;        
    }

    return (INT32)rNandDev.u4BlkSz;
}

/* for loader to read nand flash */
INT32 Loader_ReadNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
    UINT64 u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    
    if (NANDPART_Read(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
    {
        return (INT32)u4MemLen;
    }
    else
    {
        return -1;
    }
}

INT32 Loader_WriteNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
    UINT64 u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    
    if (NANDPART_Write(u8Offset, (UINT32)pvMemPtr, u4MemLen) == 0)
    {
        return (INT32)u4MemLen;
    }
    else
    {
        return -1;
    }
}

