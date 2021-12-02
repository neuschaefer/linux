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
 * $Date: 2012/05/25 $
 * $RCSfile: nor_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file nor_if.h
 *  nor_if.h The NOR flash module export types/functions
 */


#ifndef NOR_IF_H
#define NOR_IF_H
#if 0
#ifndef CC_NOR_DISABLE
#define CC_NOR_DISABLE
#endif
#endif

#ifndef CC_NOR_DISABLE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
LINT_EXT_HEADER_END
#include "storage_if.h"

;

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#define MAX_FLASHCOUNT  2
#define SFLASHNAME_LEN  48

#define NORPART_DYNAMIC_TYPE   (0)  // with wear leveling algorithm protection.
#define NORPART_STATIC_TYPE       (1) // raw data access device without wear leveling.
#define NORPART_UNUSED_TYPE      (0xFF) // unused partition.

#ifndef NORPART_PARTIAL_PROTECTION_ID
   #define NORPART_PARTIAL_PROTECTION_ID (2)
#endif
#define NORPART_PROTECTION_ENABLE TRUE
#define NORPART_PROTECTION_DISENABLE FALSE

#if 0 // move to storage_if.h
typedef struct __rNorSegInfo
{
    UINT32 u4SectNum;
    UINT32 u4SectSize;
} NORSEGINFO_T;

typedef struct __rNorSectTbl
{
    UINT32 u4SegNum;
    NORSEGINFO_T *prSegInfo;
} SECTTBL_T;
#endif
typedef struct
{
    UINT8   u1MenuID;
    UINT8   u1DevID1;
    UINT8   u1DevID2;
    UINT32  u4ChipSize;
    UINT32  u4SecSize;
    UINT32  u4SecCount;

    CHAR    pcFlashStr[SFLASHNAME_LEN];
} SFLASH_CHIPINFO_T;

typedef struct
{
    UINT8   u1FlashCount;
    SFLASH_CHIPINFO_T arFlashInfo[MAX_FLASHCOUNT];
} SFLASH_INFO_T;




//-----------------------------------------------------------------------------
//  Public functions
//-----------------------------------------------------------------------------

/* Implement at nor_part.c */
EXTERN INT32 NORPART_IoCtl(UINT32 u4IdType, UINT64 u8Data);
EXTERN UINT32 NORPART_PartStartOffset(UINT32 u4Pid);
EXTERN INT32 NORPART_GetMWPartNum(void);
EXTERN INT32 NORPART_GetDRVPartNum(void);
EXTERN UINT32 NORPART_GetPartSectorNum(UINT32 u4Pid);
EXTERN UINT32 NORPART_GetPartSize(UINT32 u4Pid);
EXTERN INT32 NORPART_GetSectorTbl(SECTTBL_T *pt_sector_tbl, UINT32 u4Pid);

// the following functions are used in storage.c.
EXTERN INT32 NORPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NORPART_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NORPART_Erase(UINT64 u8Offset, UINT32 u4SectNum);

// the following functions send requests to storage.c.
EXTERN INT32 NORSTG_Read(UINT32 u4Pid, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NORSTG_Write(UINT32 u4Pid, UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NORSTG_Erase(UINT32 u4Pid);

// the following functions are used in nor_if.c
#ifdef REPLACE_EEPROM_WITH_NOR

EXTERN INT32 Get_NOR_EEPROM_INFO(VOID);
#endif

// the following functions are used in Eeprom_hw.c
#ifdef REPLACE_EEPROM_WITH_NOR
//NOR replace of EERPORM
EXTERN INT32 NOR_EEPROM_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

EXTERN INT32 NOR_EEPROM_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
#endif
/* Implement at nor_if.c */
EXTERN INT32 NOR_Init(void);
#ifdef REPLACE_EEPROM_WITH_NOR
EXTERN void SFLASHHW_GetFlashInfo(SFLASH_INFO_T *prInfo);

EXTERN INT32 NOR_GetInfo(SFLASH_INFO_T *prNOR);
#endif

EXTERN INT32 NOR_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NOR_Queue(void);

EXTERN INT32 NOR_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 NOR_WriteByte(UINT32 u4Offset, UINT32 u4Data);
EXTERN INT32 NOR_WrNoE(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

EXTERN INT32 NOR_EraseChip(UINT32 u4FlashIndex);
EXTERN INT32 NOR_Erase(UINT32 u4SectIdx, UINT32 u4SectNum);
EXTERN INT32 NOR_EraseAddr(UINT32 u4Offset, UINT32 u4ByteCount);
EXTERN INT32 NOR_EraseBlk(UINT32 u4Offset);

EXTERN INT32 NOR_GetSize(void);
EXTERN INT32 NOR_EraseMap(SECTTBL_T *prBlkMap); // Get erase map

EXTERN INT32 NOR_CalCRC(UINT32 u4Addr, UINT32 u4Size, UINT32* pu4Checksum);

EXTERN INT32 NOR_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size);

EXTERN INT32 NOR_Diag(void);


/* Implement at serialflash_func.c */
EXTERN INT32 SFLASH_Init(void);
EXTERN INT32 SFLASH_Diag(void);
EXTERN INT32 SFLASH_Reset(void);
EXTERN INT32 SFLASH_GetID(UINT32 u4Index, UINT8 *pu1MenuID, UINT8 *pu1DevID1, UINT8 *pu1DevID2);

EXTERN INT32 SFLASH_Read(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount);
EXTERN INT32 SFLASH_Write(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount);
EXTERN INT32 SFLASH_WriteOneByte(UINT32 u4Addr, UINT8 u1Data);
EXTERN INT32 SFLASH_WrNoE(UINT32 u4Offset, UINT32 u4MemPtr, UINT32 u4ByteCount);

EXTERN INT32 SFLASH_EraseAddr(UINT32 u4Offset, UINT32 u4ByteLen);
EXTERN INT32 SFLASH_EraseSectIdx(UINT32 u4SectIdx, UINT32 u4SectNum);
EXTERN INT32 SFLASH_EraseOneBlk(UINT32 u4Addr);
EXTERN INT32 SFLASH_EraseChip(UINT32 u4Index);

EXTERN INT32 SFLASH_CalCRC(UINT32 u4Addr, UINT32 u4Len, UINT32* pu4CheckSum);

EXTERN INT32 SFLASH_EraseBlockMap(SECTTBL_T *prBlkMap, UINT32 *pu4TotalNorSz);

EXTERN void SFLASH_Calibrate(UINT32 u4Addr, UINT32 u4ByteCount, const UINT8 *pu1Buf);

EXTERN INT32 SFLASH_ReadFlashStatus(UINT32 u4Index, UINT8* pu1Status);
EXTERN INT32 SFLASH_WriteProtect(UINT32 u4Index, BOOL fgEnable);

EXTERN CHAR* SFLASH_GetFlashName(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2);
EXTERN UINT32 SFLASH_GetFlashSize(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2);


EXTERN INT32 SFLASH_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size);

EXTERN void SFLASH_PinMux(UINT32 u4Val);


/* Implement at serialflash_hw.c */
EXTERN void SFLASHHW_SetClk(UINT32 u4Val);
EXTERN void SFLASHHW_SetEnableClkDelay(BOOL fgEnable);
EXTERN void SFLASHHW_SetEnableDataDelay(BOOL fgEnable);
EXTERN void SFLASHHW_SetClkDelay(UINT32 u4Val);
EXTERN void SFLASHHW_SetDataDelay(UINT32 u4Val);
EXTERN UINT32 SFLASHHW_GetClk(void);
EXTERN UINT32 SFLASHHW_GetDataDelay(void);
EXTERN UINT32 SFLASHHW_GetClkDelay(void);
EXTERN void SFLASHHW_SetReadType(UINT32 u4Val);
EXTERN UINT32 SFLASHHW_GetReadType(void);
EXTERN void SFLASHHW_SetSampleEdge(UINT32 u4Val);
EXTERN UINT32 SFLASHHW_GetSampleEdge(void);


#else 
#include "x_typedef.h"
#include "storage_if.h"
#define ERR_NOT_SUPPORT_NOR -1

#endif/*CC_NOR_DISABLE*/

#endif /* NOR_IF_H */

