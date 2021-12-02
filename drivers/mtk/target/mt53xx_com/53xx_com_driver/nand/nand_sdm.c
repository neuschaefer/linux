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
 * $RCSfile: nand_sdm.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_sdm.c
 *  nand_sdm.c provide the nand device information.
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
#include "nandhw_if.h"

#if !defined(CC_MTK_LOADER)
#include "drvcust_if.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define BLK_STATE_BAD           0x00
#define BLK_STATE_EMPTY         0xFFFF
#define BLK_STATE_USED          0x5A

#define BLK_ALL_FF              ((UINT32)0xFFFFFFFF)
#define BLK_00FFFFFF            ((UINT32)0x00FFFFFF)

#define SDM_SUCC                0
#define SDM_FAIL                1

#define SDM_ERASECNT_SLC_BASE   0x3FF
#define SDM_ERASECNT_MLC_BASE   0xFF

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT8   u1Bi[2];             // 0~1
    UINT8   u1Lbn[3];            // 2~4
    UINT8   u1EraseCount0;       // 5
    UINT8   u1EraseCount1;       // 6
    UINT8   u1EraseCount2;       // 7
} SDM_FDM_BBM0_T; // bad block mark is at 0th byte of spare area

typedef struct
{
    UINT8   u1Lbn[3];             // 0~2
    UINT8   u1EraseCount0;        // 3
    UINT8   u1EraseCount1;        // 4
    UINT8   u1Bi[2];              // 5~6
    UINT8   u1EraseCount2;        // 7
} SDM_FDM_BBM5_T; // bad block mark is at 5th byte of spare area

typedef struct
{
    UINT32  u4LBN;
    UINT32  u4EraseCount;
    UINT16  u2State;
} SDM_BLK_STATUS_T;

typedef struct
{
    UINT32  u4BlkBase;
    UINT32  u4BlkNum;
    UINT32  *pu4PBNMap;
    SDM_BLK_STATUS_T *prBlkStatus;
} SDM_DATA_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static SDM_DATA_T   _rSDMData[NANDPART_PARTNUM];  // sync with NAND_PART_MAX_NUM
static UINT32 u4GlobalSpare[NAND_MAX_OOB_SIZE>>2];
static UINT32 u4GlobalData[NAND_MAX_PAGE_SIZE>>2];

extern NAND_FLASH_DEV_T *_prFlashDev;

//-----------------------------------------------------------------------------
static UINT32 _NAND_SDM_MarkBadBlk(UINT8 u1PartId, UINT32 u4LBN, UINT32 u4PBN)
{
    UINT32 u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;
    
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "_NAND_SDM_MarkBadBlk: u1PartId invalid %d!\n", u1PartId);
        return SDM_FAIL;
    }
    
    _rSDMData[u1PartId].prBlkStatus[u4PBN].u2State= BLK_STATE_BAD;
    _rSDMData[u1PartId].prBlkStatus[u4PBN].u4LBN = BLK_00FFFFFF;
    _rSDMData[u1PartId].pu4PBNMap[u4LBN] = BLK_ALL_FF;
    
    if (NAND_MarkBadBlk(u4PBNFlat) == FALSE)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "_NAND_SDM_MarkBadBlk fail: partid = %d, u4PBNFlat = %d, u4PBN = %d!\n", 
            u1PartId, u4PBNFlat, u4PBN);
#else
        LOG(0, "_NAND_SDM_MarkBadBlk fail: partid = %d, u4PBNFlat = %d, u4PBN = %d!\n", 
            u1PartId, u4PBNFlat, u4PBN);
#endif
        return SDM_FAIL;
    }
    else
    {
        return SDM_SUCC;
    }
}

//-----------------------------------------------------------------------------
static UINT32 _NAND_SDM_GetBlockState(SDM_BLK_STATUS_T *prStatus)
{
    ASSERT(prStatus != NULL);
    
    if (prStatus->u2State == 0)
    {
        return BLK_STATE_BAD;
    }
    
    if (prStatus->u4LBN == BLK_00FFFFFF)
    {
        return BLK_STATE_EMPTY;
    }
    else
    {
       return BLK_STATE_USED;
    }
}

//-----------------------------------------------------------------------------
static void _NAND_SDM_SetSpare(UINT32 *pu4Spare, SDM_BLK_STATUS_T *prStatus)
{
    NANDDEV_T rNandDev;
    SDM_FDM_BBM0_T *prFDMBBM0;
    SDM_FDM_BBM5_T *prFDMBBM5;
    
    ASSERT(prStatus != NULL);
    ASSERT(pu4Spare != NULL);
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return;
    }
	
    if ((_prFlashDev->u4BBMode & NAND_BBMODE_BBMARK) == 0x00000500)
    {
        prFDMBBM5 = (SDM_FDM_BBM5_T *)pu4Spare;
        
        prFDMBBM5->u1Lbn[0]      = (BYTE)(prStatus->u4LBN);
        prFDMBBM5->u1Lbn[1]      = (BYTE)(prStatus->u4LBN>>8);
        prFDMBBM5->u1Lbn[2]      = (BYTE)(prStatus->u4LBN>>16);
        prFDMBBM5->u1EraseCount0 = (BYTE)(prStatus->u4EraseCount);
        prFDMBBM5->u1EraseCount1 = (BYTE)(prStatus->u4EraseCount>>8);
        prFDMBBM5->u1Bi[0]       = (BYTE)(prStatus->u2State);
        prFDMBBM5->u1Bi[1]       = (BYTE)(prStatus->u2State>>8);
        prFDMBBM5->u1EraseCount2 = (BYTE)(prStatus->u4EraseCount>>16);
    }
    else
    {
        prFDMBBM0 = (SDM_FDM_BBM0_T *)pu4Spare;
        
        prFDMBBM0->u1Bi[0]       = (BYTE)(prStatus->u2State);
        prFDMBBM0->u1Bi[1]       = (BYTE)(prStatus->u2State>>8);
        prFDMBBM0->u1Lbn[0]      = (BYTE)prStatus->u4LBN;
        prFDMBBM0->u1Lbn[1]      = (BYTE)(prStatus->u4LBN>>8);
        prFDMBBM0->u1Lbn[2]      = (BYTE)(prStatus->u4LBN>>16);
        prFDMBBM0->u1EraseCount0 = (BYTE)prStatus->u4EraseCount;
        prFDMBBM0->u1EraseCount1 = (BYTE)(prStatus->u4EraseCount>>8);
        prFDMBBM0->u1EraseCount2 = (BYTE)(prStatus->u4EraseCount>>16);
    }
}

//-----------------------------------------------------------------------------
static void _NAND_SDM_GetSpare(UINT32 *pu4Spare, SDM_BLK_STATUS_T *prStatus)
{
    NANDDEV_T rNandDev;
    SDM_FDM_BBM0_T *prFDMBBM0;
    SDM_FDM_BBM5_T *prFDMBBM5;
    UINT32 u4EraseCntNew;
    
    ASSERT(prStatus != NULL);
    ASSERT(pu4Spare != NULL);
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return;
    }
	
    if ((_prFlashDev->u4BBMode & NAND_BBMODE_BBMARK) == 0x00000500)
    {
        prFDMBBM5 = (SDM_FDM_BBM5_T *)pu4Spare;
        
        prStatus->u4LBN   = (((UINT32)prFDMBBM5->u1Lbn[2])<<16) |
                            (((UINT32)prFDMBBM5->u1Lbn[1])<<8 ) |
                            (((UINT32)prFDMBBM5->u1Lbn[0]));
        u4EraseCntNew     = (((UINT32)prFDMBBM5->u1EraseCount2)<<16) |
                            (((UINT32)prFDMBBM5->u1EraseCount1)<<8 ) |
                            (((UINT32)prFDMBBM5->u1EraseCount0));
        prStatus->u2State = (((UINT16)prFDMBBM5->u1Bi[1])<<8) |
                            (((UINT16)prFDMBBM5->u1Bi[0]));
    }
    else
    {
        prFDMBBM0 = (SDM_FDM_BBM0_T *)pu4Spare;
        
        prStatus->u4LBN   = (((UINT32)prFDMBBM0->u1Lbn[2])<<16) |
                            (((UINT32)prFDMBBM0->u1Lbn[1])<<8 ) |
                            (((UINT32)prFDMBBM0->u1Lbn[0]));
        u4EraseCntNew     = (((UINT32)prFDMBBM0->u1EraseCount2)<<16) |
                            (((UINT32)prFDMBBM0->u1EraseCount1)<<8 ) |
                            (((UINT32)prFDMBBM0->u1EraseCount0));
        prStatus->u2State = (((UINT16)prFDMBBM0->u1Bi[1])<<8) |
                            (((UINT16)prFDMBBM0->u1Bi[0]));
    }
    
    if (u4EraseCntNew != ((UINT32)0x00FFFFFF))
    {
        prStatus->u4EraseCount = u4EraseCntNew;
    }
}

//-----------------------------------------------------------------------------
static INT32 _NAND_SDM_Scan(UINT8 u1PartId)
{
    NANDDEV_T rNandDev;
    UINT32 u4BlkNum, u4PBN, u4PBNFlat, u4LBN;
    UINT32 u4BadBlkCnt = 0;

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }

    u4BlkNum = _rSDMData[u1PartId].u4BlkNum;
    for (u4PBN = 0; u4PBN < u4BlkNum; u4PBN++)
    {
        u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;
        
        if (NAND_ReadAttr((UINT64)(u4PBNFlat * rNandDev.u4BlkSz), 
			(UINT32)u4GlobalData, (UINT32)u4GlobalSpare, rNandDev.u4PgSz) != 0)
        {
            if (NAND_IsBadBlk(u4PBNFlat))
            {
                u4BadBlkCnt++;
                _rSDMData[u1PartId].prBlkStatus[u4PBN].u2State = BLK_STATE_BAD;
                
                LOG(0, "_NAND_SDM_Scan: Read Spare Failed! %d\n", u4PBN);
                continue;
            }
            
            x_memset(u4GlobalSpare, 0xFF, sizeof(u4GlobalSpare));
        }
        
        _NAND_SDM_GetSpare(u4GlobalSpare, &_rSDMData[u1PartId].prBlkStatus[u4PBN]);
        
        u4LBN = _rSDMData[u1PartId].prBlkStatus[u4PBN].u4LBN;
        if (u4LBN < u4BlkNum)
        {
            if(_rSDMData[u1PartId].prBlkStatus[u4PBN].u2State != BLK_STATE_BAD)
            {
                _rSDMData[u1PartId].pu4PBNMap[u4LBN] = u4PBN;
            }
        }
        
        if (_rSDMData[u1PartId].prBlkStatus[u4PBN].u2State == BLK_STATE_BAD)
        {
            u4BadBlkCnt++;
        }
    }
    
    LOG(0, "NAND_SDM_Mount: Partid: %d, Total Block Count: %d, Bad Block Count: %d\n", 
        u1PartId, u4BlkNum, u4BadBlkCnt);
    return 0;
}

//-----------------------------------------------------------------------------
static INT32 _NAND_SDM_LocateBlk(UINT8 u1PartId, UINT32 u4LBN, UINT32 *pu4PBN)
{
    NANDDEV_T rNandDev;
    UINT32 u4PBN, u4PBNFlat, u4MinPBN;
    UINT32 u4State, u4MinEraseCnt, u4EraseCnt;
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "_NAND_SDM_LocateBlk: Get nand info failed!\n");
        return -1;
    }

    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "_NAND_SDM_LocateBlk: u1PartId invalid! %d\n", u1PartId);
        return -1;
    }

    // Mark old block as dirty
    u4PBN = _rSDMData[u1PartId].pu4PBNMap[u4LBN];
    if (u4PBN != BLK_ALL_FF)  //There is an existing mapping for this PBN & LBN.
    {
        if (u4PBN >= _rSDMData[u1PartId].u4BlkNum)
        {
            LOG(0, "_NAND_SDM_LocateBlk: Out of range!\n");
            return -1;
        }
        
        // Wear level u4EraseCnt
        u4EraseCnt = _rSDMData[u1PartId].prBlkStatus[u4PBN].u4EraseCount;
        if (rNandDev.fgMLC)
        {
            u4EraseCnt =  (u4EraseCnt + 1) & SDM_ERASECNT_MLC_BASE;
        }
        else
        {
            u4EraseCnt =  (u4EraseCnt + 1) & SDM_ERASECNT_SLC_BASE;
        }
        
        if (u4EraseCnt != 0x00)
        {
            *pu4PBN = u4PBN;
            return SDM_SUCC;
        }

        u4State = _NAND_SDM_GetBlockState(&_rSDMData[u1PartId].prBlkStatus[u4PBN]);
        if (u4State != BLK_STATE_USED)
        {
            LOG(0, "_NAND_SDM_LocateBlk: Block State Error!\n");
            ASSERT(0);
            return -1;
        }
        
        _rSDMData[u1PartId].pu4PBNMap[u4LBN] = BLK_ALL_FF;
        _rSDMData[u1PartId].prBlkStatus[u4PBN].u4LBN = BLK_00FFFFFF;
        _rSDMData[u1PartId].prBlkStatus[u4PBN].u4EraseCount++;
        
        u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;
        if (NAND_Erase(u4PBNFlat * rNandDev.u4PgPerBlk, rNandDev.u4PgPerBlk) != 0)
        {
            _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);
        }
        else
        {
            x_memset(u4GlobalData, 0xFF, sizeof(u4GlobalData));
            _NAND_SDM_SetSpare(u4GlobalSpare, &_rSDMData[u1PartId].prBlkStatus[u4PBN]);
            
            if (NAND_WriteAttr((UINT64)(u4PBNFlat * rNandDev.u4BlkSz), 
                               (UINT32)u4GlobalData, (UINT32)u4GlobalSpare, rNandDev.u4PgSz) != 0)
            {
                _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);
            }
        }
    }

    //-------------------------------------
    // Find new block
    //-------------------------------------
    u4MinPBN = 0;
    u4MinEraseCnt = BLK_ALL_FF;
    for (u4PBN = 0; u4PBN < _rSDMData[u1PartId].u4BlkNum; u4PBN++)
    {
        u4State = _NAND_SDM_GetBlockState(&_rSDMData[u1PartId].prBlkStatus[u4PBN]);
        if (u4State == BLK_STATE_EMPTY)
        {
            if (u4MinEraseCnt > _rSDMData[u1PartId].prBlkStatus[u4PBN].u4EraseCount)
            {
                u4MinEraseCnt = _rSDMData[u1PartId].prBlkStatus[u4PBN].u4EraseCount;
                u4MinPBN = u4PBN;
            }
        }
    }

    if (u4MinEraseCnt != BLK_ALL_FF)
    {
        *pu4PBN = u4MinPBN;
        return SDM_SUCC;
    }
    else
    {
        LOG(0, "_NAND_SDM_LocateBlk: No free block!\n");
        return -1;
    }
}

//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_Mount.
 *  Details of NAND_SDM_Mount (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_Mount(UINT8 u1PartId)
{
    NANDPART_STRUCT_T *prNandPart;
    NANDDEV_T rNandDev;
    
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        ASSERT(0);
        return -1;
    }
    
    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }
    
    prNandPart = NANDPART_GetPartStruct();

#ifdef __KERNEL__
    printk(KERN_CRIT "NAND_SDM_Mount: Partid=%d, offset=0x%x, size=0x%x\n", 
        u1PartId, prNandPart[u1PartId].u4Offset, prNandPart[u1PartId].u4Size);
#else
    LOG(0, "NAND_SDM_Mount: Partid=%d, offset=0x%x, size=0x%x\n", 
        u1PartId, prNandPart[u1PartId].u4Offset, prNandPart[u1PartId].u4Size);
#endif
    
    _rSDMData[u1PartId].u4BlkNum = prNandPart[u1PartId].u4Size / rNandDev.u4BlkSz;
    _rSDMData[u1PartId].u4BlkBase = prNandPart[u1PartId].u4Offset / rNandDev.u4BlkSz;

    // Block status
    _rSDMData[u1PartId].prBlkStatus =
         (SDM_BLK_STATUS_T *)x_mem_alloc(_rSDMData[u1PartId].u4BlkNum * sizeof(SDM_BLK_STATUS_T));
    if (_rSDMData[u1PartId].prBlkStatus == NULL)
    {
        return -1;
    }
    x_memset((void *)_rSDMData[u1PartId].prBlkStatus, 0, _rSDMData[u1PartId].u4BlkNum * sizeof(SDM_BLK_STATUS_T));

    // Logical blk mapping
    _rSDMData[u1PartId].pu4PBNMap = (UINT32 *)x_mem_alloc(_rSDMData[u1PartId].u4BlkNum * sizeof(UINT32));
    if (_rSDMData[u1PartId].pu4PBNMap == NULL)
    {
        return -1;
    }
    x_memset((void *)_rSDMData[u1PartId].pu4PBNMap, 0xFF, _rSDMData[u1PartId].u4BlkNum * sizeof(UINT32));

    // Scan block
    if (_NAND_SDM_Scan(u1PartId) != 0)
    {
        return -1;
    }
    
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_UnMount.
 *  Details of NAND_SDM_UnMount (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_UnMount(UINT8 u1PartId)
{
    x_mem_free((void *)_rSDMData[u1PartId].prBlkStatus);
    x_mem_free((void *)_rSDMData[u1PartId].pu4PBNMap);

    _rSDMData[u1PartId].prBlkStatus = NULL;
    _rSDMData[u1PartId].pu4PBNMap = NULL;

    return 0;
}

INT32 NAND_SDM_ReadPagesEx(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    UINT32 u4LBN, u4PBN, u4PBNFlat,u4OffsetInBlk,u4Size,u4PgIdx;
    INT32 i4BlkNum;
    UINT32 u4BlkState,u4RestPgCnt,u4RdPgCnt;
    
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }
    
    if ((u4Offset & (rNandDev.u4PgSz - 1)) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPagesEx: Read offset not page alignment!\n");
#else
        LOG(0, "NAND_SDM_ReadPagesEx: Read offset not page alignment!\n");
#endif
        return -1;
    }
    
    if ((u4MemLen & (rNandDev.u4PgSz - 1)) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPagesEx: Read size not page alignment!\n");
#else
        LOG(0, "NAND_SDM_ReadPagesEx: Read size not page alignment!\n");
#endif
        return -1;
    }
    
    u4LBN = u4Offset / rNandDev.u4BlkSz;
    i4BlkNum = u4MemLen / rNandDev.u4BlkSz;
    u4Size = u4MemLen;
	
    if (u4LBN+i4BlkNum > _rSDMData[u1PartId].u4BlkNum)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPagesEx: Read Offset too large!\n");
#else
        LOG(0, "NAND_SDM_ReadPagesEx: Read Offset too large!\n");
#endif
        return -1;
    }

    
    while (u4Size > 0)
    {
       u4OffsetInBlk = u4Offset &(rNandDev.u4BlkSz - 1);
       u4PgIdx = u4OffsetInBlk/rNandDev.u4PgSz;
       u4PBN = _rSDMData[u1PartId].pu4PBNMap[u4LBN];
       u4BlkState = _NAND_SDM_GetBlockState(&_rSDMData[u1PartId].prBlkStatus[u4PBN]);
       u4RestPgCnt = u4Size /rNandDev.u4PgSz;

       if (u4RestPgCnt > rNandDev.u4PgPerBlk - u4PgIdx)
       {
	   u4RdPgCnt = rNandDev.u4PgPerBlk - u4PgIdx;
       }
       else
       {
	   u4RdPgCnt = u4RestPgCnt;
       }
	   if (u4PBN == BLK_ALL_FF)
       {
          //LOG(5, "NAND_SDM_ReadPages: Un-mapped block 0x%x!\n", u4LBN);
          x_memset((void *)u4MemPtr, 0xFF, rNandDev.u4PgSz*u4RdPgCnt);
       }
       else if (u4BlkState != BLK_STATE_USED)
       {
          LOG(5, "NAND_SDM_ReadPagesEx: Not used block, 0x%x!\n", u4LBN);
          x_memset((void *)u4MemPtr, 0xFF, rNandDev.u4PgSz*u4RdPgCnt);
       }
       else
       {
           u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;
        
           if (NAND_Read((UINT64)(u4PBNFlat * rNandDev.u4BlkSz + u4OffsetInBlk), 
                    u4MemPtr, rNandDev.u4PgSz*u4RdPgCnt) != 0)
           {
#ifdef __KERNEL__
              printk(KERN_ERR "NAND_SDM_ReadPagesEx fail!\n");
#else
              LOG(0, "NAND_SDM_ReadPagesEx fail!\n");
#endif
              return -1;
           }
        }
        u4MemPtr += rNandDev.u4PgSz*u4RdPgCnt;
        u4Size -= rNandDev.u4PgSz*u4RdPgCnt;
        u4Offset += rNandDev.u4PgSz*u4RdPgCnt;
	    u4LBN ++;  
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_ReadBlocks.
 *  Details of NAND_SDM_ReadBlocks (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_ReadBlocks(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    UINT32 u4LBN, u4PBN, u4PBNFlat;
    INT32 i4BlkNum;
    
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        LOG(0, "NAND flash info get error!\n");
        return -1;
    }

    if (((u4Offset % rNandDev.u4BlkSz) != 0) || ((u4MemLen % rNandDev.u4BlkSz) != 0))
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadBlocks: NOT block alignment! u4Offset = 0x%x, u4MemLen = 0x%x\n", u4Offset, u4MemLen);
#else
        LOG(0, "NAND_SDM_ReadBlocks: Read offset or length not block alignment!\n");
#endif
        return -1;
    }

    u4LBN = u4Offset / rNandDev.u4BlkSz;
    i4BlkNum = u4MemLen / rNandDev.u4BlkSz;

    if ((u4LBN + i4BlkNum) > _rSDMData[u1PartId].u4BlkNum)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadBlocks: Read Offset too large! 0x%x, 0x%x, 0x%x\n", 
            u4LBN, i4BlkNum, _rSDMData[u1PartId].u4BlkNum);
#else
        LOG(0, "NAND_SDM_ReadBlocks: Read Offset too large!\n");
#endif
        return -1;
    }

    while (i4BlkNum > 0)
    {
        u4PBN = _rSDMData[u1PartId].pu4PBNMap[u4LBN];

        if (u4PBN == BLK_ALL_FF)
        {
            LOG(5, "NAND_SDM_ReadBlocks: Un-mapped block 0x%x!\n", u4LBN);
            x_memset((void *)u4MemPtr, 0xFF, rNandDev.u4BlkSz);
        }
        else if (_NAND_SDM_GetBlockState(&_rSDMData[u1PartId].prBlkStatus[u4PBN]) != BLK_STATE_USED)
        {
            LOG(5, "NAND_SDM_ReadBlocks: Not used block, 0x%x!\n", u4LBN);
            x_memset((void *)u4MemPtr, 0xFF, rNandDev.u4BlkSz);
        }
        else
        {
            u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;
            
            if (NAND_Read((UINT64)(u4PBNFlat * rNandDev.u4BlkSz), u4MemPtr, rNandDev.u4BlkSz) != 0)
            {
#ifdef __KERNEL__
                printk(KERN_ERR "NAND_SDM_ReadBlocks fail!\n");
#else
                LOG(0, "NAND_SDM_ReadBlocks fail!\n");
#endif
                return -1;
            }
        }

        i4BlkNum--;
        u4LBN++;
        u4MemPtr += rNandDev.u4BlkSz;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_WriteBlocks.
 *  Details of NAND_SDM_WriteBlocks (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_WriteBlocks(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    UINT32 u4LBN, u4PBN, u4PBNFlat;
    INT32  i4BlkNum;
    
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }

    if (((u4Offset % rNandDev.u4BlkSz) != 0) || ((u4MemLen % rNandDev.u4BlkSz) != 0))
    {
        LOG(0, "NAND_SDM_WriteBlocks: Write offset or length not block alignment!\n");
        return -1;
    }

    u4LBN = u4Offset / rNandDev.u4BlkSz;
    i4BlkNum = u4MemLen / rNandDev.u4BlkSz;

    if ((u4LBN + i4BlkNum) > _rSDMData[u1PartId].u4BlkNum)
    {
        LOG(0, "NAND_SDM_WriteBlocks: Write block Offset too large!\n");
        return -1;
    }

    while (i4BlkNum > 0)
    {
        // Locate block
        if (_NAND_SDM_LocateBlk(u1PartId, u4LBN, &u4PBN) != 0)
        {
            return -1;
        }

        u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;

        // Erase
        if (NAND_Erase(u4PBNFlat * rNandDev.u4PgPerBlk, rNandDev.u4PgPerBlk) != 0)
        {
            _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);
            continue;
        }

        _rSDMData[u1PartId].prBlkStatus[u4PBN].u2State = BLK_STATE_EMPTY;
        _rSDMData[u1PartId].prBlkStatus[u4PBN].u4EraseCount++;
        _rSDMData[u1PartId].prBlkStatus[u4PBN].u4LBN = u4LBN;
        _rSDMData[u1PartId].pu4PBNMap[u4LBN] = u4PBN;
        _NAND_SDM_SetSpare(u4GlobalSpare, &_rSDMData[u1PartId].prBlkStatus[u4PBN]);
        
        if (NAND_WriteAttr((UINT64)(u4PBNFlat * rNandDev.u4BlkSz), 
                            u4MemPtr, (UINT32)u4GlobalSpare, rNandDev.u4BlkSz) != 0)
        {
            _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);
            continue;
        }
        
#if defined(CC_NAND_WRITE_VERIFY)
        if (NAND_VerifyAttr((UINT64)(u4PBNFlat * rNandDev.u4BlkSz), rNandDev.u4BlkSz) != 0)
        {
            _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);
            continue;
        }
#endif

        i4BlkNum--;
        u4LBN++;
        u4MemPtr += rNandDev.u4BlkSz;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_ReadPages.
 *  Details of NAND_SDM_ReadPages (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_ReadPages(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4PageIdx, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    UINT32 u4LBN, u4PBN, u4PBNFlat;
    
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }
    
    if ((u4Offset % rNandDev.u4BlkSz) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPages: Read offset not block alignment!\n");
#else
        LOG(0, "NAND_SDM_ReadPages: Read offset not block alignment!\n");
#endif
        return -1;
    }
    
    if ((u4MemLen % rNandDev.u4PgSz) != 0)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPages: Read size not page alignment!\n");
#else
        LOG(0, "NAND_SDM_ReadPages: Read size not page alignment!\n");
#endif
        return -1;
    }

    if ((u4PageIdx + u4MemLen / rNandDev.u4PgSz) > rNandDev.u4PgPerBlk)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPages: Read page index too large!\n");
#else
        LOG(0, "NAND_SDM_ReadPages: Read page index too large!\n");
#endif
        return -1;
    }
    
    u4LBN = u4Offset / rNandDev.u4BlkSz;
    if (u4LBN > _rSDMData[u1PartId].u4BlkNum)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "NAND_SDM_ReadPages: Read Offset too large!\n");
#else
        LOG(0, "NAND_SDM_ReadPages: Read Offset too large!\n");
#endif
        return -1;
    }

    u4PBN = _rSDMData[u1PartId].pu4PBNMap[u4LBN];

    if (u4PBN == BLK_ALL_FF)
    {
        //LOG(5, "NAND_SDM_ReadPages: Un-mapped block 0x%x!\n", u4LBN);
        x_memset((void *)u4MemPtr, 0xFF, u4MemLen);
    }
    else if (_NAND_SDM_GetBlockState(&_rSDMData[u1PartId].prBlkStatus[u4PBN]) != BLK_STATE_USED)
    {
        LOG(5, "NAND_SDM_ReadPages: Not used block, 0x%x!\n", u4LBN);
        x_memset((void *)u4MemPtr, 0xFF, u4MemLen);
    }
    else
    {
        u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;
        
        if (NAND_Read((UINT64)(u4PBNFlat * rNandDev.u4BlkSz + u4PageIdx * rNandDev.u4PgSz), 
                    u4MemPtr, u4MemLen) != 0)
        {
#ifdef __KERNEL__
            printk(KERN_ERR "NAND_SDM_ReadPages fail!\n");
#else
            LOG(0, "NAND_SDM_ReadPages fail!\n");
#endif
            return -1;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_WritePages.
 *  Details of NAND_SDM_WritePages (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_WritePages(UINT8 u1PartId, UINT32 u4Offset, UINT32 *pu4PageIdx, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    NANDDEV_T rNandDev;
    UINT32 u4PageIdx = *pu4PageIdx;
    UINT32 u4LBN, u4PBN, u4PBNFlat;
   
    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        LOG(0, "Partition number %d is not match!\n", u1PartId);
        ASSERT(0);
        return -1;
    }

    if (NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }
    
    if ((u4Offset % rNandDev.u4BlkSz) != 0)
    {
        LOG(0, "NAND_SDM_WritePages: Write offset not block alignment!\n");
        return -1;
    }
    
    if ((u4MemLen % rNandDev.u4PgSz) != 0)
    {
        LOG(0, "NAND_SDM_WritePages: Write size not page alignment!\n");
        return -1;
    }
    
    if ((u4PageIdx + u4MemLen / rNandDev.u4PgSz) > rNandDev.u4PgPerBlk)
    {
        LOG(0, "NAND_SDM_WritePages: Read page index too large!\n");
        return -1;
    }

    u4LBN = u4Offset / rNandDev.u4BlkSz;
    if (u4LBN > _rSDMData[u1PartId].u4BlkNum)
    {
        LOG(0, "NAND_SDM_WritePages: Write block Offset too large!\n");
        return -1;
    }

    while (1)
    {
        // Locate new block
        if (u4PageIdx == 0)
        {
            if (_NAND_SDM_LocateBlk(u1PartId, u4LBN, &u4PBN) != 0)
            {
                return -1;
            }
            
            u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;

            // Erase
            if (NAND_Erase(u4PBNFlat * rNandDev.u4PgPerBlk, rNandDev.u4PgPerBlk) != 0)
            {
                _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);

                u4PageIdx = 0;
                continue;
            }

            _rSDMData[u1PartId].prBlkStatus[u4PBN].u2State = BLK_STATE_EMPTY;
            _rSDMData[u1PartId].prBlkStatus[u4PBN].u4EraseCount++;
            _rSDMData[u1PartId].prBlkStatus[u4PBN].u4LBN = u4LBN;
            _rSDMData[u1PartId].pu4PBNMap[u4LBN] = u4PBN;
            _NAND_SDM_SetSpare(u4GlobalSpare, &_rSDMData[u1PartId].prBlkStatus[u4PBN]);
        }
        else
        {
            u4PBN = _rSDMData[u1PartId].pu4PBNMap[u4LBN];
            ASSERT(u4PBN != 0xFFFFFFFF);
            u4PBNFlat = u4PBN + _rSDMData[u1PartId].u4BlkBase;

            x_memset(u4GlobalSpare, 0xFF, sizeof(u4GlobalSpare));
        }
        
        if (NAND_WriteAttr((UINT64)(u4PBNFlat * rNandDev.u4BlkSz + u4PageIdx * rNandDev.u4PgSz), 
                            u4MemPtr, (UINT32)u4GlobalSpare, u4MemLen) != 0)
        {
            _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);

            u4PageIdx = 0;
            continue;
        }

#if defined(CC_NAND_WRITE_VERIFY)
        if (NAND_VerifyAttr((UINT64)(u4PBNFlat * rNandDev.u4BlkSz + u4PageIdx * rNandDev.u4PgSz), u4MemLen) != 0)
        {
            _NAND_SDM_MarkBadBlk(u1PartId, u4LBN, u4PBN);
            
            u4PageIdx = 0;
            continue;
        }
#endif
        break;
    }

    *pu4PageIdx = u4PageIdx;
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_DumpStatus.
 *  Details of NAND_SDM_DumpStatus (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_DumpStatus(UINT8 u1PartId, UINT32 u4Flags)
{
    UINT32 i;
    NANDDEV_T rNandDev;

    if (u1PartId >= (UINT8)NANDPART_PARTNUM)
    {
        return -1;
    }

    if(NAND_GetInfo(&rNandDev) != 0)
    {
        return -1;
    }

    Printf("Physical order:\n");
    for (i = 0; i < _rSDMData[u1PartId].u4BlkNum; i++)
    {
        if ((u4Flags & NAND_SDMDUMP_BAD) && (_rSDMData[u1PartId].prBlkStatus[i].u2State == BLK_STATE_BAD))
        {
            Printf("0x%06x: lbn: 0x%08x, state: 0x%08x, ec: %08x\n", i,
                        _rSDMData[u1PartId].prBlkStatus[i].u4LBN,
                        _rSDMData[u1PartId].prBlkStatus[i].u2State,
                        _rSDMData[u1PartId].prBlkStatus[i].u4EraseCount);
        }

        if ((u4Flags & NAND_SDMDUMP_EMPTY) && (_rSDMData[u1PartId].prBlkStatus[i].u2State == BLK_STATE_EMPTY))
        {
            Printf("0x%06x: lbn: 0x%08x, state: 0x%08x, ec: %08x\n", i,
                        _rSDMData[u1PartId].prBlkStatus[i].u4LBN,
                        _rSDMData[u1PartId].prBlkStatus[i].u2State,
                        _rSDMData[u1PartId].prBlkStatus[i].u4EraseCount);
        }

        if ((u4Flags & NAND_SDMDUMP_USED) && (_rSDMData[u1PartId].prBlkStatus[i].u2State == BLK_STATE_USED))
        {
            Printf("0x%06x: lbn: 0x%08x, state: 0x%08x, ec: %08x\n", i,
                        _rSDMData[u1PartId].prBlkStatus[i].u4LBN,
                        _rSDMData[u1PartId].prBlkStatus[i].u2State,
                        _rSDMData[u1PartId].prBlkStatus[i].u4EraseCount);
        }
    }

    if (u4Flags & NAND_SDMDUMP_LOGIC)
    {
        Printf("Logic order:\n");
        for (i = 0; i < _rSDMData[u1PartId].u4BlkNum; i++)
        {
            if (_rSDMData[u1PartId].pu4PBNMap[i] != BLK_ALL_FF)
            {
                Printf("logic:0x%06x, physical: [0x%08x: 0x%08x]\n", i,
                              (_rSDMData[u1PartId].u4BlkBase + _rSDMData[u1PartId].pu4PBNMap[i]) * rNandDev.u4BlkSz,
                              _rSDMData[u1PartId].pu4PBNMap[i]);
            }
            else
            {
                Printf("logic:0x%06x, physical: [0x%08x: 0x%08x]\n", i,
                              _rSDMData[u1PartId].u4BlkBase * rNandDev.u4BlkSz,
                              _rSDMData[u1PartId].pu4PBNMap[i]);
            }
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_SyncCheck.
 *  Details of NAND_SDM_SyncCheck (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_SyncCheck(UINT8 u1PartId)
{
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of NAND_SDM_SyncStatus.
 *  Details of NAND_SDM_SyncStatus (optional).
 */
//-----------------------------------------------------------------------------
INT32 NAND_SDM_SyncStatus(UINT8 u1PartId)
{
    return 0;
}

