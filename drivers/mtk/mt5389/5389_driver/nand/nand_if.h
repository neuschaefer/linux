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
 * $RCSfile: nand_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_if.h
 *  nand_if.h The NAND driver export function declaration.
 */

#ifndef NAND_IF_H
#define NAND_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "storage_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if defined(CC_MTK_LOADER) || defined(__cpluscplus)
    #define NAND_ENABLE_ISR             (0)
#else
    #define NAND_ENABLE_ISR             (1)
#endif

#define NAND_MULTIPAGE_ACCESS           (0)

#define NAND_CTRL_FLAGS_ECC             ((UINT32)0x1)
#define NAND_CTRL_FLAGS_DMA             ((UINT32)0x2)
#define NAND_CTRL_FLAGS_ISR             ((UINT32)0x4)
#define NAND_CTRL_FLAGS_ALL             ((UINT32)0xFFFFFFFF)

//-----------------------------------------------------------------------------
// IOCTL flags
//-----------------------------------------------------------------------------
#define NAND_IOCTL_GET_NANDSIZE         (0x00000001)
#define NAND_IOCTL_GET_BLOCKSIZE        (0x00000002)
#define NAND_IOCTL_GET_PARTITION_NUM    (0x00000003)
#define NAND_IOCTL_GET_PARTITION_SIZE   (0x00000004)
#define NAND_IOCTL_LOWLEVEL_FORMAT      (0x00000110)

#define NAND_MAX_PAGE_SIZE              (1024*8)
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #define NAND_MAX_OOB_SIZE               (80*8)
#else
    #define NAND_MAX_OOB_SIZE               (64*8)
#endif
#define NAND_NAME_MAX_SIZE              (32)

#define NAND_SDMDUMP_BAD                (0x01)
#define NAND_SDMDUMP_EMPTY              (0x02)
#define NAND_SDMDUMP_USED               (0x04)
#define NAND_SDMDUMP_DIRTY              (0x08)
#define NAND_SDMDUMP_LOGIC              (0x10)
#define NAND_SDMDUMP_ALL                (0x0F)

#define NANDPART_PARTNUM                (32)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Device
typedef struct strucNandDev
{
    UINT32 u4PgSz;
    UINT32 u4BlkSz;
    UINT32 u4OOBSz;
    UINT32 u4TotalBlk;
    UINT32 u4TotalSize;
    UINT32 u4PgPerBlk;
    BOOL   fgMLC;
    UINT32 u4BBMode;
} NANDDEV_T;

typedef enum
{
    NAND_ECC_NONE,
    NAND_ECC_SOFT,
    NAND_ECC_HARD,
} NAND_ECC_TYPE_T;

typedef struct
{
    NAND_ECC_TYPE_T rEccType;
    BOOL fgDMA;
    BOOL fgIsr;
} NAND_CONTROL_T;

//-----------------------------------------------------------------------------
// NAND partition struct
//-----------------------------------------------------------------------------
typedef enum
{
    NANDPART_NONE,
    NANDPART_BDM,
    NANDPART_SDM,
} NANDPART_TYPE;

typedef enum
{
    NANDPART_ATTRB_READONLY,
    NANDPART_ATTRB_READWRITE,
} NANDPART_ATTRB;

typedef INT32 (*PFN_NANDREAD)(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Memlen);
typedef INT32 (*PFN_NANDWRITE)(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4Memlen);

typedef struct
{
    UINT8           u1PartId;
    CHAR            cName[NAND_NAME_MAX_SIZE];
    UINT32          u4Offset;
    UINT32          u4Size;
    NANDPART_TYPE   rType;
    NANDPART_ATTRB  rAttrb;

    PFN_NANDREAD    pfnNandRead;
    PFN_NANDWRITE   pfnNandWrite;
} NANDPART_STRUCT_T;

//-----------------------------------------------------------------------------
// mtk partition information 
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32  u4PgStart;
    UINT32  u4PgEnd;
    UINT32  u4Encryped;
} mtk_part_info_t;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
extern void NAND_SetControl(UINT32 u4Flag, NAND_CONTROL_T *prCtrl);
EXTERN void NAND_GetControl(UINT32 u4Flag, NAND_CONTROL_T *prCtrl);

#ifdef CC_FCI_ENABLE
// fix me
EXTERN BOOL NAND_QuerySetSwEcc(const BOOL *pfgSet);     // Compatible to XD
#endif

EXTERN VOID NAND_ApiLock(HANDLE_T *hhSema);
EXTERN VOID NAND_ApiUnLock(HANDLE_T *hhSema);

EXTERN void NAND_Init(void);
EXTERN void NAND_Stop(void);
EXTERN void NAND_Reset(void);
EXTERN INT32 NAND_Diag(void);
EXTERN INT32 NAND_GetInfo(NANDDEV_T *prNand);
EXTERN INT32 NAND_ReadID(UINT32 *pID);

// Storage device interface function
EXTERN INT32 NAND_Read(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_Write(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_ReadAttr(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4SparePtr, UINT32 u4MemLen);
#if defined(CC_NAND_WRITE_VERIFY)
EXTERN INT32 NAND_VerifyAttr(UINT64 u8Offset, UINT32 u4MemLen);
#endif
EXTERN INT32 NAND_WriteAttr(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4SparePtr, UINT32 u4MemLen);
EXTERN INT32 NAND_Erase(UINT32 u4PageIdx, UINT32 u4PageNum);
EXTERN INT32 NAND_EraseForce(UINT32 u4PageIdx, UINT32 u4PageNum);
EXTERN INT32 NAND_RdAttr(UINT64 u8Offset, UINT32 u4DataMemPtr, UINT32 u4ByteCount);
EXTERN INT32 NAND_WrAttr(UINT64 u8Offset, UINT32 u4DataMemPtr, UINT32 u4ByteCount);
EXTERN INT64 NAND_GetSize(void);

EXTERN INT32 NAND_ReadBlock(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_BlankCheck(UINT64 u8Offset, UINT32 u4PageCount);
EXTERN INT32 NAND_BlankCheckAttr(UINT64 u8Offset, UINT32 u4PageCount);

EXTERN BOOL NAND_IsBadBlk(UINT32 u4BlkNum);
EXTERN BOOL NAND_MarkBadBlk(UINT32 u4BlkNum);

EXTERN void NAND_ShowDevList(void);

// nand_part.c
EXTERN INT32 NANDPART_Init(void);
EXTERN NANDPART_STRUCT_T* NANDPART_GetPartStruct(void);
EXTERN void NANDPART_ShowPartition(void);

#ifndef NDEBUG  /*only valid in debug version*/
EXTERN UINT32 NANDPART_Writer_NandStructure(void);
EXTERN UINT32 NANDPART_Writer_NandLayout(void);
#endif

EXTERN INT32 NANDPART_IORequest(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen, BOOL fgRead);
EXTERN INT32 NANDPART_Mount(UINT8 u1PartId, NANDPART_TYPE rType, UINT32 u4Offset, UINT32 u4Size);
EXTERN INT32 NANDPART_UnMount(UINT8 u1PartId);
EXTERN INT32 NANDPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NANDPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NANDPART_BulkWrite(UINT64 u8Offset, UINT8 u1Val, UINT32 u4MemLen);
EXTERN INT32 NANDPART_GetSectorTbl(SECTTBL_T *pt_sector_tbl, UINT32 u4Pid);
EXTERN INT32 NANDPART_GetMWPartNum(void);
EXTERN UINT32 NANDPART_GetMaxPartId(void);
EXTERN UINT32 NANDPART_GetPartSize(UINT32 u4Pid);
EXTERN INT32 NANDPART_Erase(UINT8 u1PartId);
EXTERN UINT32 NANDPART_GetPageSize(UINT32 u4Pid);

// nand_bdm.c
EXTERN INT32 NAND_BDM_Mount(UINT8 u1PartId);
EXTERN INT32 NAND_BDM_UnMount(UINT8 u1PartId);
EXTERN INT32 NAND_BDM_Format(UINT8 u1PartId);

EXTERN INT32 NAND_BDM_ReadBlocks(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_BDM_WriteBlocks(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_BDM_ReadSectors(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_BDM_WriteSectors(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_BDM_ReadPages(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

// nand_sdm.c
EXTERN INT32 NAND_SDM_Mount(UINT8 u1PartId);
EXTERN INT32 NAND_SDM_UnMount(UINT8 u1PartId);

EXTERN INT32 NAND_SDM_ReadBlocks(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_SDM_WriteBlocks(UINT8 u1Part, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_SDM_ReadPagesEx(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

EXTERN INT32 NAND_SDM_ReadPages(UINT8 u1PartId, UINT32 u4Offset, UINT32 u4PageIdx, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_SDM_WritePages(UINT8 u1PartId, UINT32 u4Offset, UINT32 *pu4PageIdx, UINT32 u4MemPtr, UINT32 u4MemLen);

EXTERN INT32 NAND_SDM_DumpStatus(UINT8 u1PartId, UINT32 u4Flags);
EXTERN INT32 NAND_SDM_SyncCheck(UINT8 u1PartId);
EXTERN INT32 NAND_SDM_SyncStatus(UINT8 u1PartId);
EXTERN INT32 NAND_SDM_RWTest(UINT32 u4Count);
EXTERN INT32 NAND_SDM_MountTest(UINT32 u4Count);

// for loader use
EXTERN INT32 Loader_GetNandFlashBlockSize(void);
EXTERN INT32 Loader_ReadNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);
EXTERN INT32 Loader_WriteNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);

// for eeprom replacement
EXTERN INT32 NAND_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NAND_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

#endif /* NAND_IF_H */


