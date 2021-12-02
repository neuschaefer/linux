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
 * $RCSfile: nand_bdm.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_bdm.c
 *  nand_bdm.c provide the nand device information.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"

#include "nand_debug.h"
#include "nand_part.h"
#include "nand_if.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT8*  pu1BadBlkMap;
    UINT32* pu4LPMap;
} BDM_DATA_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static BDM_DATA_T _rBDMData[NANDPART_PARTNUM];

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void _NAND_BDM_BuildLPMap(UINT8 u1PartId, UINT32 u4BlkNum)
{
    UINT32 i, u4PBlkIdx = 0;

    if (u1PartId >= NANDPART_PARTNUM)
    {
        LOG(0, "_NAND_BDM_BuildLPMap: Invalid partition id!\n");
    }

    for (i = 0; i < u4BlkNum; i++)
    {
        while(_rBDMData[u1PartId].pu1BadBlkMap[u4PBlkIdx] == 0x0)
        {
            u4PBlkIdx++;
        }
        
        if (u4PBlkIdx >= u4BlkNum)
        {
            break;
        }
        
        _rBDMData[u1PartId].pu4LPMap[i] = u4PBlkIdx;
        u4PBlkIdx++;
    }
}

//-----------------------------------------------------------------------------
/** Brief of NAND_BDM_Mount.
 *  Details of NAND_BDM_Mount (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_BDM_Mount(UINT8 u1PartId)
{
    NANDDEV_T rNandDev;
    NANDPART_STRUCT_T *prNandPart;
    UINT32 u4BlkNum, i, u4BadBlkCnt = 0, u4BlkIdx;

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }

    if (u1PartId >= NANDPART_PARTNUM)
    {
        LOG(0, "NAND_BDM_Mount: Invalid partition id!\n");
        return -1;
    }
    
    prNandPart = NANDPART_GetPartStruct();

    LOG(0, "NAND_BDM_Mount: Partid=%d, offset=0x%x, size=0x%x\n", 
        u1PartId, prNandPart[u1PartId].u4Offset, prNandPart[u1PartId].u4Size);
    
    u4BlkNum = prNandPart[u1PartId].u4Size / rNandDev.u4BlkSz;    
    _rBDMData[u1PartId].pu1BadBlkMap = (UINT8 *)x_mem_alloc(u4BlkNum);
    if (_rBDMData[u1PartId].pu1BadBlkMap == 0)
    {
        LOG(0, "NAND_BDM_Mount: malloc pu1BadBlkMap failed!\n");
        return -1;
    }
    
    _rBDMData[u1PartId].pu4LPMap = (UINT32 *)x_mem_alloc(u4BlkNum * sizeof(UINT32));
    if (_rBDMData[u1PartId].pu4LPMap == 0)
    {
        LOG(0, "NAND_BDM_Mount: malloc pu4LPMap failed!\n");
        return -1;
    }

    x_memset(_rBDMData[u1PartId].pu1BadBlkMap, 0xFF, u4BlkNum);
    x_memset((UINT8 *)_rBDMData[u1PartId].pu4LPMap, 0xFF, (u4BlkNum * sizeof(UINT32)));

    // Scan bad blocks
    for (i = 0 ; i < u4BlkNum ; i++)
    {
        u4BlkIdx = (prNandPart[u1PartId].u4Offset / rNandDev.u4BlkSz) + i;
        if (NAND_IsBadBlk(u4BlkIdx))
        {
            if (rNandDev.u4PgSz == 512)  // small page
            {
                _rBDMData[u1PartId].pu1BadBlkMap[i] = 0x0;
                u4BadBlkCnt++;
            }
            else    // large page
            {
                _rBDMData[u1PartId].pu1BadBlkMap[i] = 0x0;
                u4BadBlkCnt++;
            }
        }
    }    

    // Build LP Map (Logical/Physical)
    _NAND_BDM_BuildLPMap(u1PartId, u4BlkNum);

    LOG(0, "NAND_BDM_Mount: Partid: %d, Total Block Count: %d, Bad Block Count: %d\n", 
        u1PartId, u4BlkNum, u4BadBlkCnt);
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of NAND_BDM_UnMount.
 *  Details of NAND_BDM_UnMount (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_BDM_UnMount(UINT8 u1PartId)
{
    if (u1PartId >= NANDPART_PARTNUM)
    {
        LOG(0, "NAND_BDM_UnMount: Invalid partition id!\n");
        return -1;
    }
    
    x_mem_free((void *)_rBDMData[u1PartId].pu1BadBlkMap);
    _rBDMData[u1PartId].pu1BadBlkMap = NULL;
    
    x_mem_free((void *)_rBDMData[u1PartId].pu4LPMap);
    _rBDMData[u1PartId].pu4LPMap = NULL;
    
    return 0;
}

INT32 NAND_BDM_ReadPages(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    NANDPART_STRUCT_T *prNandPart;
    UINT32 u4Size, u4Addr, u4BasePBN, u4LBN, u4PBN, u4BaseOffset, u4BlkNum, u4OffsetInBlk, u4PageIdx;
    UINT32 u4RestPgCnt,u4RdPgCnt;

    prNandPart = NANDPART_GetPartStruct();
    
    if (u1Part >= NANDPART_PARTNUM)
    {
        LOG(0, "NAND_BDM_ReadPages: Invalid partition id!\n");
        return -1;
    }
    
    if (u4Offset >= prNandPart[u1Part].u4Size)
    {
        LOG(0, "NAND_BDM_ReadPages: Offset too large %d!\n",u4Offset);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND_BDM_ReadPages: Read before mount!\n");
        return -1;
    }

    if ((u4Offset & (rNandDev.u4PgSz - 1)) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_BDM_ReadPages: Read offset not page alignment! Offset = 0x%x, page size = 0x%x\n", u4Offset, rNandDev.u4PgSz);
#else
        LOG(0, "NAND_BDM_ReadPages: Read offset not page alignment!\n");
#endif
        return -1;
    }
    
    if ((u4MemLen & (rNandDev.u4PgSz - 1)) != 0)
    {
        LOG(0, "NAND_BDM_ReadPages: Read length not page alignment!\n");
        return -1;
    }
	 
    if (_rBDMData[u1Part].pu1BadBlkMap == 0)
    {
        LOG(0, "NAND_BDM_ReadPages: Read before mount!\n");
        return -1;
    }

    u4LBN = u4Offset / rNandDev.u4BlkSz;
    u4Size = u4MemLen;
    u4BasePBN = prNandPart[u1Part].u4Offset / rNandDev.u4BlkSz;
    u4BaseOffset = prNandPart[u1Part].u4Offset;
    u4BlkNum = prNandPart[u1Part].u4Size / rNandDev.u4BlkSz;
    
    while (u4Size > 0)
    {
        u4OffsetInBlk = u4Offset & (rNandDev.u4BlkSz - 1);
        u4PageIdx = u4OffsetInBlk/rNandDev.u4PgSz;
        u4PBN = _rBDMData[u1Part].pu4LPMap[u4LBN] + u4BasePBN;
        u4Addr = u4BaseOffset + _rBDMData[u1Part].pu4LPMap[u4LBN] * rNandDev.u4BlkSz + u4OffsetInBlk;
	u4RestPgCnt = u4Size / rNandDev.u4PgSz;
	    
	if (u4RestPgCnt > rNandDev.u4PgPerBlk - u4PageIdx)
	{
	    u4RdPgCnt = rNandDev.u4PgPerBlk - u4PageIdx;
	}
	else
	{
	    u4RdPgCnt = u4RestPgCnt;
	}
        
        if (_rBDMData[u1Part].pu4LPMap[u4LBN] == 0xFFFFFFFF)
        {
            LOG(0, "NAND_BDM_ReadPages: BDM too many bad block in partition %d\n", u1Part);
            return -1;
        }
        
        if (u4Addr >= rNandDev.u4TotalSize)
        {
            LOG(0, "NAND_BDM_ReadPages: nand read Offset exceeds total size!\n");
            return -1;
        }
        
        if (u4Addr >= (prNandPart[u1Part].u4Offset + prNandPart[u1Part].u4Size))
        {
            LOG(0, "NAND_BDM_ReadPages: nand read Offset exceeds partition size!\n");
            return -1;
        }

        if (NAND_IsBadBlk(u4PBN))
        {
            LOG(0, "NAND_BDM_ReadPages: Bad block %d, addr 0x%x\n", u4PBN, u4PBN * rNandDev.u4BlkSz);
            _rBDMData[u1Part].pu1BadBlkMap[u4PBN - u4BasePBN] = 0x0;
            _NAND_BDM_BuildLPMap(u1Part, u4BlkNum);
            continue;
        }

        if (NAND_Read(u4Addr, u4MemPtr, rNandDev.u4PgSz * u4RdPgCnt) != 0)
        {
            LOG(0, "NAND_BDM_ReadBlocks: fail at offset 0x%x, size 0x%x\n", u4Addr, rNandDev.u4PgSz*u4RdPgCnt);
            return -1;
        }

        u4MemPtr += rNandDev.u4PgSz * u4RdPgCnt;
        u4Size -= rNandDev.u4PgSz * u4RdPgCnt;
        u4Offset += rNandDev.u4PgSz * u4RdPgCnt;
	u4LBN++;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of NAND_BDM_ReadBlocks.
 *  Details of NAND_BDM_ReadBlocks (optional).
 *  The min access unit is page.
 */
//-----------------------------------------------------------------------------
INT32 NAND_BDM_ReadBlocks(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    NANDPART_STRUCT_T *prNandPart;
    UINT32 u4Size, u4Addr, u4BasePBN, u4LBN, u4PBN, u4BaseOffset, u4BlkNum;

    prNandPart = NANDPART_GetPartStruct();
    
    if (u1Part >= NANDPART_PARTNUM)
    {
        LOG(0, "NAND_BDM_ReadBlocks: Invalid partition id!\n");
        return -1;
    }
    
    if (u4Offset >= prNandPart[u1Part].u4Size)
    {
        LOG(0, "NAND_BDM_ReadBlocks: Offset too large %d!\n",u4Offset);
        return -1;
    }
    
    if (_rBDMData[u1Part].pu1BadBlkMap == 0)
    {
        LOG(0, "NAND_BDM_ReadBlocks: Read before mount!\n");
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }

    u4LBN = u4Offset / rNandDev.u4BlkSz;
    u4Size = u4MemLen;
    u4BasePBN = prNandPart[u1Part].u4Offset / rNandDev.u4BlkSz;
    u4BaseOffset = prNandPart[u1Part].u4Offset;
    u4BlkNum = prNandPart[u1Part].u4Size / rNandDev.u4BlkSz;
    
    if ((u4MemLen & (rNandDev.u4BlkSz - 1)) != 0)
    {
        LOG(0, "NAND_BDM_ReadBlocks: Read length not block alignment!\n");
        return -1;
    }

    while (u4Size > 0)
    {
        u4PBN = _rBDMData[u1Part].pu4LPMap[u4LBN] + u4BasePBN;
        u4Addr = u4BaseOffset + _rBDMData[u1Part].pu4LPMap[u4LBN] * rNandDev.u4BlkSz;
        
        if (_rBDMData[u1Part].pu4LPMap[u4LBN] == 0xFFFFFFFF)
        {
            LOG(0, "NAND_BDM_ReadBlocks: BDM too many bad block in partition %d\n", u1Part);
            return -1;
        }
        
        if (u4Addr >= rNandDev.u4TotalSize)
        {
            LOG(0, "NAND_BDM_ReadBlocks: nand read Offset exceeds total size!\n");
            return -1;
        }
        
        if (u4Addr >= (prNandPart[u1Part].u4Offset + prNandPart[u1Part].u4Size))
        {
            LOG(0, "NAND_BDM_ReadBlocks: nand read Offset exceeds partition size!\n");
            return -1;
        }

        if (NAND_IsBadBlk(u4PBN))
        {
            LOG(0, "NAND_BDM_ReadBlocks: Bad block %d, addr 0x%x\n", u4PBN, u4PBN * rNandDev.u4BlkSz);
            _rBDMData[u1Part].pu1BadBlkMap[u4PBN - u4BasePBN] = 0x0;
            _NAND_BDM_BuildLPMap(u1Part, u4BlkNum);
            continue;
        }

        if (NAND_Read(u4Addr, u4MemPtr, rNandDev.u4BlkSz) != 0)
        {
            LOG(0, "NAND_BDM_ReadBlocks: fail at offset 0x%x, size 0x%x\n", u4Addr, rNandDev.u4BlkSz);
            return -1;
        }

        u4MemPtr += rNandDev.u4BlkSz;
        u4Size -= rNandDev.u4BlkSz;
        u4LBN++;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of NAND_BDM_WriteBlocks.
 *  Details of NAND_BDM_WriteBlocks (optional).
 *  The min access unit is block.
 */
//-----------------------------------------------------------------------------
INT32 NAND_BDM_WriteBlocks(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    NANDPART_STRUCT_T *prNandPart;
    UINT32 u4Size, u4Addr, u4BasePBN, u4LBN, u4PBN, u4BaseOffset, u4BlkNum;

    prNandPart = NANDPART_GetPartStruct();

    if (u1Part >= NANDPART_PARTNUM)
    {
        LOG(0, "NAND_BDM_WriteBlocks: Invalid partition id!\n");
        return -1;
    }
    
    if (u4Offset >= prNandPart[u1Part].u4Size)
    {
        LOG(0, "NAND_BDM_WriteBlocks: Offset too large %d!\n", u4Offset);
        return -1;
    }
    
    if (_rBDMData[u1Part].pu1BadBlkMap == 0)
    {
        LOG(0, "NAND_BDM_WriteBlocks: Write before mount !\n");
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }

    u4LBN = u4Offset / rNandDev.u4BlkSz;
    u4Size = u4MemLen;
    u4BasePBN = prNandPart[u1Part].u4Offset / rNandDev.u4BlkSz;
    u4BaseOffset = prNandPart[u1Part].u4Offset;
    u4BlkNum = prNandPart[u1Part].u4Size / rNandDev.u4BlkSz;

    if ((u4MemLen & (rNandDev.u4BlkSz - 1)) != 0)
    {
        LOG(0, "NAND_BDM_WriteBlocks: Write length not block alignment!\n");
        return -1;
    }

    while (u4Size > 0)
    {
        u4PBN = _rBDMData[u1Part].pu4LPMap[u4LBN] + u4BasePBN;
        u4Addr = u4BaseOffset + _rBDMData[u1Part].pu4LPMap[u4LBN] * rNandDev.u4BlkSz;
        
        if (_rBDMData[u1Part].pu4LPMap[u4LBN] == 0xFFFFFFFF)
        {
            LOG(0, "NAND_BDM_WriteBlocks: BDM too many bad block in partition %d\n", u1Part);
            return -1;
        }
        
        if (u4Addr >= rNandDev.u4TotalSize)
        {
            LOG(0, "NAND_BDM_WriteBlocks: nand write Offset exceeds total size!\n");
            return -1;
        }
        
        if (u4Addr >= (prNandPart[u1Part].u4Offset + prNandPart[u1Part].u4Size))
        {
            LOG(0, "NAND_BDM_WriteBlocks: nand write Offset exceeds partition size!\n");
            return -1;
        }

        if (NAND_IsBadBlk(u4PBN))
        {
            LOG(3, "NAND_BDM_WriteBlocks: Bad block %d, addr 0x%x\n", u4PBN, u4PBN * rNandDev.u4BlkSz);
            _rBDMData[u1Part].pu1BadBlkMap[u4PBN - u4BasePBN] = 0x0;
            _NAND_BDM_BuildLPMap(u1Part, u4BlkNum);
            continue;
        }

        if (NAND_Erase(u4Addr/rNandDev.u4PgSz, rNandDev.u4PgPerBlk) != 0)
        {
            LOG(0, "NAND_BDM_WriteBlocks: nand erase fail page %d\n", u4Addr/rNandDev.u4PgSz);
            NAND_MarkBadBlk(u4PBN);
            _rBDMData[u1Part].pu1BadBlkMap[u4PBN - u4BasePBN] = 0x0;
            _NAND_BDM_BuildLPMap(u1Part, u4BlkNum);
            continue;
        }

        if (NAND_Write((UINT64)u4Addr, u4MemPtr, rNandDev.u4BlkSz) != 0)
        {
            LOG(0, "NAND_BDM_WriteBlocks: fail at offset 0x%x, size 0x%x\n", u4Addr, rNandDev.u4BlkSz);
            NAND_MarkBadBlk(u4PBN);
            _rBDMData[u1Part].pu1BadBlkMap[u4PBN - u4BasePBN] = 0x0;
            _NAND_BDM_BuildLPMap(u1Part, u4BlkNum);
            continue;
        }
#if defined(CC_NAND_WRITE_VERIFY)
	if (NAND_VerifyAttr((UINT64)u4Addr, u4BlkSize) != 0)
	{
           LOG(0, "NAND_BDM_WriteBlocks verify fail offset 0x%x, size 0x%x\n", u4Addr, u4BlkSize);
           NAND_MarkBadBlk(u4PBN);
           _rBDMData[u1Part].u1BadBlockMap[u4PBN - u4BasePBN] = 0x0;
           _NAND_BDM_BuildLPMap(u1Part, u4BlkNum);
           continue;
	}
#endif

        u4MemPtr += rNandDev.u4BlkSz;
        u4Size -= rNandDev.u4BlkSz;
        u4LBN++;
    }

    return 0;
}

