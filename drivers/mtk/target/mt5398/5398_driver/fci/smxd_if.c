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

/** @file smxd_if.c
 *  This file contains implementation of SM/XD slot interface functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "smxd_func.h"

LINT_EXT_HEADER_BEGIN

#ifdef SMXD_USING_NAND_IF
#ifdef CC_BD_FCI
#include "x_hal_1176.h"
#elif defined(CC_DTV_FCI)
#include "x_hal_926.h"
#endif
#include "nandxd_if.h"
#include "nand_if.h"
#endif

#include "fcihw_func.h"

#include "fci_type.h"
#include "fci_debug.h"
#include "fci_if.h"

#include "x_os.h"

LINT_EXT_HEADER_END

/*
    lint inhibition(s)
        Note 960: Violates MISRA Required Rule 54, null statement not in line by itself.
        Note 971: Use of 'char' without 'signed' or 'unsigned' [MTK Rule 5.2.1].
        Info 826: Suspicious pointer-to-pointer conversion (area too small)
*/

/*lint -save -e960 -e971 -e826*/

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//#define SMXD_READONLY_MODE

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// ECC1 start  index of  byte 0-255 in data area.

#define SMXD_DATA1_INDEX                      (0)
/// ECC2 start  index of  byte 256-511 in data area.
#define SMXD_DATA2_INDEX                      (256)

/// index of byte 512-527 in redundancy area.
#define SMXD_RDNT_START_INDEX               (512)
/// index of reserved bytes in redundancy area.
#define SMXD_RDNT_REV_INDEX                      (SMXD_RDNT_START_INDEX)
/// index of data status byte in redundancy area.
#define SMXD_RDNT_DATA_STATUS_INDEX        ((SMXD_RDNT_REV_INDEX)+4)
/// index of block status byte in redundancy area.
#define SMXD_RDNT_BLK_STATUS_INDEX             ((SMXD_RDNT_REV_INDEX)+5)
/// index of logical block address 1 in redundancy area.
#define SMXD_RDNT_BLK_ADDR1_INDEX           ((SMXD_RDNT_REV_INDEX)+6)
/// index of ECC2 in redundancy area.
#define SMXD_RDNT_ECC2_INDEX                    ((SMXD_RDNT_REV_INDEX)+8)
/// index of logical block address 2 in redundancy area.
#define SMXD_RDNT_BLK_ADDR2_INDEX           ((SMXD_RDNT_REV_INDEX)+11)
/// index of ECC1 in redundancy area.
#define SMXD_RDNT_ECC1_INDEX                      ((SMXD_RDNT_REV_INDEX)+13)

#define SMXD_FCI_BLOCK_SIZE      (512)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
/// calculate zone number by physical block.
#define SMXD_PHY_ZONE(phyblk)       (UINT16)((phyblk)/(SMXD_PHY_BLK_PER_ZONE))
/// calculate zone number by logical block.
#define SMXD_LOG_ZONE(logblk)       (UINT16)((logblk)/(SMXD_LOG_BLK_PER_ZONE))
/// calculate module physical block number by zone.
#define SMXD_MOD_PHY_BLK(phyblk)   (UINT16)((phyblk)%(SMXD_PHY_BLK_PER_ZONE))
/// calculate module logical block number by zone.
#define SMXD_MOD_LOG_BLK(logblk)     (UINT16)((logblk)%(SMXD_LOG_BLK_PER_ZONE))
/// calculate real physical address by zone and module physical block number.
#define SMXD_REAL_PHY_BLK(zone, modphyblk)   (UINT32)((modphyblk)+((zone)*(SMXD_PHY_BLK_PER_ZONE)))
/// calculate real logical address by zone and module logical block number.
#define SMXD_REAL_LOG_BLK(zone, modlogblk)   (UINT32)((modlogblk)+((zone)*(SMXD_LOG_BLK_PER_ZONE)))

// logical to physical block mapping table handling macro.
/*
#define SMXD_LPTBL_CHK(modlogblk) \
    ASSERT(_au2SmLpTbl[modlogblk] == SMXD_LOG_BLK_BLANK_NO)
*/

/// check the data of logical to physical mapping table is blank.

#define SMXD_LPTBL_CHK(modlogblk) \
    (_au2SmLpTbl[modlogblk] == SMXD_LOG_BLK_BLANK_NO)

/// check all data of logical to physical mapping table in one zone are blank.
#define SMXD_LPTBL_CHK_LOOP                                 \
    for (u2Blk = 0; u2Blk < SMXD_LOG_BLK_PER_ZONE; u2Blk++) \
    {                                                       \
         if (_au2SmLpTbl[u2Blk] == SMXD_LOG_BLK_BLANK_NO)   \
         {                                                  \
            return E_FAIL;                                  \
         }                                                  \
    }

/// read the data of logical to physical mapping table.
#define SMXD_LPTBL_READ(modphyblk, modlogblk) \
    (modphyblk) = _au2SmLpTbl[modlogblk];

/// write the data of logical to physical mapping table.
#define SMXD_LPTBL_WRITE(modphyblk, modlogblk) \
    _au2SmLpTbl[modlogblk] = (modphyblk);

/// reset all the data of logical to physical mapping table.
#define SMXD_LPTBL_RESET                                                                  \
        if (NULL == x_memset((VOID * )_au2SmLpTbl, SMXD_BLANK_BYTE, sizeof(_au2SmLpTbl))) \
        {                                                                                 \
        return E_FAIL;                                                                    \
        }

// Unused block table handling macro.

/// push unused block number to unused block table.
#ifndef SMXD_READONLY_MODE
#define SMXD_UNUSED_BLK_PUSH(modphyblk)                         \
        _au2SmUnusedBlkTbl[_au2SmUnusedBlkIndex] = (modphyblk); \
        _au2SmUnusedBlkIndex++;                                 \
        if (_au2SmUnusedBlkIndex > SMXD_PHY_BLK_PER_ZONE) return E_FAIL;
/* prevent buffer overflow.*/

/// pop unused block number to unused block table.
#define SMXD_UNUSED_BLK_POP(modphyblk)           \
    if (_au2SmUnusedBlkIndex < 1) return E_FAIL; \
    _au2SmUnusedBlkIndex--;                      \
    (modphyblk) = _au2SmUnusedBlkTbl[_au2SmUnusedBlkIndex];

/// reset unused block table.
#define SMXD_UNUSED_BLK_RESET                                                                       \
    _au2SmUnusedBlkIndex = 0;                                                                       \
    if (NULL == x_memset((VOID * )_au2SmUnusedBlkTbl, SMXD_BLANK_BYTE, sizeof(_au2SmUnusedBlkTbl))) \
    {                                                                                               \
    return E_FAIL;                                                                                  \
    }

/// check unused block table is empty.
#define SMXD_UNUSED_BLK_CHK_LOOP \
    if (_au2SmUnusedBlkIndex == 0) return E_FAIL;
#else
#define SMXD_UNUSED_BLK_PUSH(modphyblk)

/// pop unused block number to unused block table.
#define SMXD_UNUSED_BLK_POP(modphyblk)

/// reset unused block table.
#define SMXD_UNUSED_BLK_RESET

/// check unused block table is empty.
#define SMXD_UNUSED_BLK_CHK_LOOP
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
/// local buffer: it use to buffer one page of data and redundancy in each r/w operation.

//static UINT8 _au1SmBuf[SMXD_TOTAL_PAGE_SIZE];
static UINT8 *_pu1SmBufOrg = NULL;
static UINT8 *_pu1SmBuf = NULL;

/// logical block address mapping table in one zone.
/// value range is (physical block number)mod(SMXD_PHY_BLK_PER_ZONE)
/// between 0 ~ (SMXD_PHY_BLK_PER_ZONE-1)
static UINT16 _au2SmLpTbl[SMXD_LOG_BLK_PER_ZONE];

#ifndef SMXD_READONLY_MODE

/// Unused block table number in one zone.
/// value range is (physical block number)mod(SMXD_PHY_BLK_PER_ZONE)
/// between 0 ~ (SMXD_PHY_BLK_PER_ZONE-1)

static UINT16 _au2SmUnusedBlkTbl[SMXD_PHY_BLK_PER_ZONE];
static UINT16 _au2SmUnusedBlkIndex = 0;
#endif

/// define CIS default value. Usually in block 0, page 0.

static const UINT8 _au1SmCISField[SMXD_TOTAL_PAGE_SIZE] =
{
    // CIS field 1 and IDI field 1.
    0x01, 0x03, 0xD9, 0x01, 0xFF, 0x18, 0x02, 0xDF, /* 0x00*/
    0x01, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x21, /* 0x08*/
    0x02, 0x04, 0x01, 0x22, 0x02, 0x01, 0x01, 0x22, /* 0x10*/
    0x03, 0x02, 0x04, 0x07, 0x1A, 0x05, 0x01, 0x03, /* 0x18*/
    0x00, 0x02, 0x0F, 0x1B, 0x08, 0xC0, 0xC0, 0xA1, /* 0x20*/
    0x01, 0x55, 0x08, 0x00, 0x20, 0x1B, 0x0A, 0xC1, /* 0x28*/
    0x41, 0x99, 0x01, 0x55, 0x64, 0xF0, 0xFF, 0xFF, /* 0x30*/
    0x20, 0x1B, 0x0C, 0x82, 0x41, 0x18, 0xEA, 0x61, /* 0x38*/
    0xF0, 0x01, 0x07, 0xF6, 0x03, 0x01, 0xEE, 0x1B, /* 0x40*/
    0x0C, 0x83, 0x41, 0x18, 0xEA, 0x61, 0x70, 0x01, /* 0x48*/
    0x07, 0x76, 0x03, 0x01, 0xEE, 0x15, 0x14, 0x05, /* 0x50*/
    0x00,
/* name of Manufacture: 0x59 ~ 0x60*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
/* name of Product: 0x61 ~ 0x65*/
    0x20, 0x20, 0x20, 0x20, 0x00,
/* product version: 0x66 ~ 0x6A*/
    0x30, 0x2E, 0x30, 0x00, 0xFF, 0x14, 0x00, 0xFF,
    0x00, 0x00,                                     /* 0x6B*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x70*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x78*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x80*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x88*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x90*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x98*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xA0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xA8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xB0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xB8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xC0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xC8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xD0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xD8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xE0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xE8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xF0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0xF8*/
// CIS field 2 and IDI field 2.
    0x01, 0x03, 0xD9, 0x01, 0xFF, 0x18, 0x02, 0xDF, /* 0x100*/
    0x01, 0x20, 0x04, 0x00, 0x00, 0x00, 0x00, 0x21, /* 0x108*/
    0x02, 0x04, 0x01, 0x22, 0x02, 0x01, 0x01, 0x22, /* 0x110*/
    0x03, 0x02, 0x04, 0x07, 0x1A, 0x05, 0x01, 0x03, /* 0x118*/
    0x00, 0x02, 0x0F, 0x1B, 0x08, 0xC0, 0xC0, 0xA1, /* 0x120*/
    0x01, 0x55, 0x08, 0x00, 0x20, 0x1B, 0x0A, 0xC1, /* 0x128*/
    0x41, 0x99, 0x01, 0x55, 0x64, 0xF0, 0xFF, 0xFF, /* 0x130*/
    0x20, 0x1B, 0x0C, 0x82, 0x41, 0x18, 0xEA, 0x61, /* 0x138*/
    0xF0, 0x01, 0x07, 0xF6, 0x03, 0x01, 0xEE, 0x1B, /* 0x140*/
    0x0C, 0x83, 0x41, 0x18, 0xEA, 0x61, 0x70, 0x01, /* 0x148*/
    0x07, 0x76, 0x03, 0x01, 0xEE, 0x15, 0x14, 0x05, /* 0x150*/
    0x00,
/* name of Manufacture: 0x159 ~ 0x160*/
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00,
/* name of Product: 0x161 ~ 0x165*/
    0x20, 0x20, 0x20, 0x20, 0x00,
/* product version: 0x166 ~ 0x16A*/
    0x30, 0x2E, 0x30, 0x00, 0xFF, 0x14, 0x00, 0xFF,
    0x00, 0x00,                                     /* 0x16B*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x170*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x178*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x180*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x188*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x190*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x198*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1A0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1A8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1B0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1B8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1C0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1C8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1D0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1D8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1E0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1E8*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1F0*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 0x1F8*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, /* 0x200*/
    0x0C, 0xCC, 0xC3, 0x00, 0x00, 0x0C, 0xCC, 0xC3  /* 0x208*/
};

#ifdef CC_BD_FCI

static INT32 _i4SMXDStatus = (INT32)STORG_DEV_DETACH;
static UINT8 _aszSMXDModel[STORG_DEV_MODELNAME_LEN];

static FCI_CALLBACK _pfnSMXDCallback;

#elif defined(CC_DTV_FCI)

static INT32 _i4SMXDStatus = (INT32)STORG_DEV_ABSENT;
static UINT8 _aszSMXDModel[STORG_DEV_MODELNAME_LEN];

static UINT32 _u4SMXDCallbackTag;
static BLOCK_CALLBACK _pfnSMXDCallback;

static BlockFunc_T _rSMXDBlkFuncTbl =
{
    NULL, NULL, NULL, SMXD_GetSize, SMXD_GetBlockSize, SMXD_GetStatus,
    SMXD_GetModel, NULL, SMXD_Read, SMXD_Write, SMXD_SetHotswapCallback
};
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _CntByte0
 *  Caculate the number of  0 bits in one byte.
 *  @param  u4Byte		user's one data byte.
 *  @retval  number of 0 bits in u4Byte.
 */
//-------------------------------------------------------------------------
static UINT32 _CntByte0(UINT32 u4Byte)
{
    UINT32 i;

    UINT32 u4Mask;
    UINT32 u4Cnt;

    u4Cnt = 0;

    // check number of 0 bit in one byte.
    for (i = 0; i < 8; i++)
    {
        u4Mask = 0x01 << i;
        if (!(u4Byte&u4Mask))
        {
            u4Cnt++;
        }
    }

    return u4Cnt;
}

//-------------------------------------------------------------------------
/** _ChkDataRdnt
 *  check redundancy area in normal data block.
 *  @param  pu1Rdnt		data pointer to the redundancy area (16 bytes).
 *  @retval  TRUE 	success.
 *  @retval  FALSE	fail.
 */
//-------------------------------------------------------------------------
static BOOL _ChkDataRdnt(const UINT8 *pu1Rdnt)
{
    UINT32 i;

    UINT32 u4BlkAddr = 0;
    UINT32 u4Parity = 0;

    ASSERT(pu1Rdnt != NULL);

    // 4 reserved bytes.
    for (i = 0; i < 4; i++)
    {
        if (pu1Rdnt[i] != SMXD_BLANK_BYTE)
        {
            return FALSE;
        }
    }

    // check data status flag.
    if (_CntByte0(pu1Rdnt[4]) >= 4)
    {
        return FALSE;
    }

    // check block status flag.
    if (_CntByte0(pu1Rdnt[5]) >= 2)
    {
        return FALSE;
    }

    // check block address field.
    if (!(pu1Rdnt[6] & 0x10))
    {
        return FALSE;
    }

    // check block address field.
    if ((pu1Rdnt[6] != pu1Rdnt[11]) || (pu1Rdnt[7] != pu1Rdnt[12]))
    {
        return FALSE;
    }

    // check block address field by calculating parity check.
    u4BlkAddr = (pu1Rdnt[6] << 8) | pu1Rdnt[7];

    for (i = 0; i < 11; i++)
    {
        u4Parity ^= (u4BlkAddr >> i) & 0x01;
    }

    // odd parity on BA0 - BA9.
    if (!u4Parity)
    {
        return FALSE;
    }

    return TRUE;
}

//-------------------------------------------------------------------------
/** _PlrPhyBlk
 *  Check two physical block are all valid or not. Return one block and erase the duplicated one.
 *  @param  u2NowZone		the handling zone.
 *  @param  u2ModPhyBlk1	the module physical address point to the same logical block with u2ModPhyBlk2.
 *  @param  u2ModPhyBlk2	the module physical address point to the same logical block with u2ModPhyBlk1.
 *  @retval  the selected  ModPhyBlk.
 */
//-------------------------------------------------------------------------
static UINT16 _PlrPhyBlk(UINT16 u2NowZone, UINT16 u2ModPhyBlk1, UINT16 u2ModPhyBlk2)
{
    UINT32 u4PhyBlk1;

    UINT32 u4PhyBlk2;
    INT32 i4Ret;

    if (u2ModPhyBlk1 == u2ModPhyBlk2)
    {
        return SMXD_LOG_BLK_BLANK_NO;
    }

    LOG(7, "%s: blk1=%d, blk2=%d.\n", __FUNCTION__, u2ModPhyBlk1, u2ModPhyBlk2);

    u4PhyBlk1 = SMXD_REAL_PHY_BLK(u2NowZone, u2ModPhyBlk1);

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_ReadRdnt(u4PhyBlk1, SMXD_LAST_PAGE_NO, _pu1SmBuf);
#else
    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, u4PhyBlk1);
    i4Ret = NANDXDHW_ReadRdnt(u4PhyBlk1, SMXD_LAST_PAGE_NO, _pu1SmBuf);
#endif

    // read block redundant area in the last sector.
    if (S_OK != i4Ret)
    {
        return SMXD_LOG_BLK_BLANK_NO;
    }

    if (!_ChkDataRdnt((const UINT8 *)_pu1SmBuf))
    {
        // block not valid.
        u2ModPhyBlk1 = SMXD_LOG_BLK_BLANK_NO;
    }

    u4PhyBlk2 = SMXD_REAL_PHY_BLK(u2NowZone, u2ModPhyBlk2);

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_ReadRdnt(u4PhyBlk2, SMXD_LAST_PAGE_NO, _pu1SmBuf);
#else
    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, u4PhyBlk2);
    i4Ret = NANDXDHW_ReadRdnt(u4PhyBlk2, SMXD_LAST_PAGE_NO, _pu1SmBuf);
#endif

    // read block redundant area in the last sector.
    if (S_OK != i4Ret)
    {
        return SMXD_LOG_BLK_BLANK_NO;
    }

    if (!_ChkDataRdnt((const UINT8 *)_pu1SmBuf))
    {
        // block not valid.
        u2ModPhyBlk2 = SMXD_LOG_BLK_BLANK_NO;
    }

    ASSERT((u2ModPhyBlk1 != SMXD_LOG_BLK_BLANK_NO) || (u2ModPhyBlk2 != SMXD_LOG_BLK_BLANK_NO));

    // erase the duplicate or invalid block.
    if (u2ModPhyBlk1 == SMXD_LOG_BLK_BLANK_NO)
    {
#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_EraseBlock(u4PhyBlk1);
#else
        i4Ret = NANDXDHW_EraseBlock(u4PhyBlk1);
#endif

        if (S_OK != i4Ret)
        {
            return SMXD_LOG_BLK_BLANK_NO;
        }
        return u2ModPhyBlk2;
    }
    else if (u2ModPhyBlk2 == SMXD_LOG_BLK_BLANK_NO)
    {
#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_EraseBlock(u4PhyBlk2);
#else
        i4Ret = NANDXDHW_EraseBlock(u4PhyBlk2);
#endif

        if (S_OK != i4Ret)
        {
            return SMXD_LOG_BLK_BLANK_NO;
        }
        return u2ModPhyBlk1;
    }
    else
    {
        // choose the block with the lower physical address.
        if (u2ModPhyBlk1 > u2ModPhyBlk2)
        {
#ifndef SMXD_USING_NAND_IF
            i4Ret = SMXDHW_EraseBlock(u4PhyBlk1);
#else
            i4Ret = NANDXDHW_EraseBlock(u4PhyBlk1);
#endif

            if (S_OK != i4Ret)
            {
                return SMXD_LOG_BLK_BLANK_NO;
            }
            return u2ModPhyBlk2;
        }
        else
        {
#ifndef SMXD_USING_NAND_IF
            i4Ret = SMXDHW_EraseBlock(u4PhyBlk2);
#else
            i4Ret = NANDXDHW_EraseBlock(u4PhyBlk2);
#endif

            if (S_OK != i4Ret)
            {
                return SMXD_LOG_BLK_BLANK_NO;
            }
            return u2ModPhyBlk1;
        }
    }
}

//-------------------------------------------------------------------------
/** _ChkCISRdnt
 *  Check the format of CIS block redundancy area.
 *  @param  pu1Rdnt			data pointer to the redundancy area (16 bytes).
 *  @retval  TRUE 	success.
 *  @retval  FALSE	fail.
 */
//-------------------------------------------------------------------------
static BOOL _ChkCISRdnt(const UINT8 *pu1Rdnt)
{
    UINT32 i;

    ASSERT(pu1Rdnt != NULL);

    // 4 reserved bytes.
    for (i = 0; i < 4; i++)
    {
        if (pu1Rdnt[i] != SMXD_BLANK_BYTE)
        {
            return FALSE;
        }
    }

    // check data status flag.
    if (_CntByte0(pu1Rdnt[4]) >= 4)
    {
        return FALSE;
    }

    // check block status and address fieldf.
    if ((pu1Rdnt[5] != SMXD_BLANK_BYTE) || (pu1Rdnt[6] != 0) || (pu1Rdnt[7] != 0)
            || (pu1Rdnt[11] != 0) || (pu1Rdnt[12] != 0))
    {
        return FALSE;
    }

    return TRUE;
}

#ifndef SMXD_READONLY_MODE

//-------------------------------------------------------------------------
/** _WriteCIS
 *  Write default CIS to card.
 *  @param  u1CISBlk		CIS will be written at the first page of u1CISBlk block.
 *  @retval  S_OK 	success.
 *  @retval  others	fail.
 */
//-------------------------------------------------------------------------

static INT32 _WriteCIS(UINT8 u1CISBlk)
{
    INT32 i4Ret = 0;

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_EraseBlock(u1CISBlk);
#else
    i4Ret = NANDXDHW_EraseBlock(u1CISBlk);
#endif

    if (S_OK != i4Ret)
    {
        return i4Ret;
    }
#ifdef SMXD_USING_NAND_IF
    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
    i4Ret = NANDXDHW_ReadPage(u1CISBlk, 0, _pu1SmBuf, 512);
    if (S_OK != i4Ret)
    {
        return i4Ret;
    }
#endif
#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_WritePage(u1CISBlk, 0, (UINT8 *)_au1SmCISField);
#else
    // make sure DRAM address is multiple of 16 bytes.
    VERIFY(x_memcpy((VOID *)_pu1SmBuf,
        (const VOID *)_au1SmCISField, SMXD_TOTAL_PAGE_SIZE));

    HalFlushInvalidateDCacheMultiple((UINT32)_pu1SmBuf, 512);
    i4Ret = NANDXDHW_WritePage(u1CISBlk, 0, (UINT8 *)_pu1SmBuf);

/*
    if (S_OK != i4Ret)
    {
        return i4Ret;
    }

    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
    i4Ret = NANDXDHW_ReadPage(u1CISBlk, 0, _pu1SmBuf, 512);
*/
#endif

    return i4Ret;
}
#endif

//-------------------------------------------------------------------------
/** _ChkPhyFmt
 *  Check CIS data is exist or not. Write the default CIS to card if CIS not exist.
 *  @param  VOID
 *  @retval  S_OK 	success.
 *  @retval  others	fail.
 */
//-------------------------------------------------------------------------

static INT32 _ChkPhyFmt(VOID)
{
    INT32 i4Ret = 0;

    SMXD_T *prSMXD;
    UINT32 i = 0;
    UINT32 u4Ecc1, u4Ecc2, u4Ecc3, u4Ecc4;
    UINT8 au1Ecc[3];
    UINT32 u4Data;
    UINT8 u1CISBlk = SMXD_BLANK_BYTE;

    prSMXD = FCI_GetSMXDSlot();

    while (i < SMXD_CHK_BLK_NO)
    {
#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_ReadRdnt(i, SMXD_FIRST_PAGE_NO, _pu1SmBuf);
#else
        HalFlushFlushDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
        i4Ret = NANDXDHW_ReadRdnt(i, SMXD_FIRST_PAGE_NO, _pu1SmBuf);
#endif

        if (S_OK != i4Ret)
        {
            return E_FAIL;
        }

        if (_ChkCISRdnt((const UINT8 *)_pu1SmBuf))
        {
            u1CISBlk = i;
            break;
        }
        i++;
    }

    if (u1CISBlk == SMXD_BLANK_BYTE)
    {

#ifndef SMXD_READONLY_MODE

        // write CIS field in card in block 0, page 0.

        u1CISBlk = 0;

#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_SetStartPtr();
#else
        i4Ret = NANDXDHW_SetStartPtr();
        //i4Ret = NANDXDHW_ReadPage(0, 0, (UINT8 *)_pu1SmBuf, SMXD_PAGE_SIZE);
#endif

        if (S_OK != i4Ret)
        {
            return E_FAIL;
        }

        if (S_OK != _WriteCIS(u1CISBlk))
        {
            return E_FAIL;
        }

#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_ReadRdnt(u1CISBlk, SMXD_FIRST_PAGE_NO, _pu1SmBuf);
#else
        HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
        i4Ret = NANDXDHW_ReadRdnt(u1CISBlk, SMXD_FIRST_PAGE_NO, _pu1SmBuf);
#endif

        if (S_OK != i4Ret)
        {
            return E_FAIL;
        }

        if (!_ChkCISRdnt((const UINT8 *)_pu1SmBuf))
        {
            return E_FAIL;
        }
#else

        return E_FAIL;
#endif

    }

    LOG(1, "%s: ***block = %d ***.\n", __FUNCTION__, 0);
    u4Data = (_pu1SmBuf[3] << 24) | (_pu1SmBuf[2] << 16) | (_pu1SmBuf[1] << 8) | (_pu1SmBuf[0]);
    LOG(1, "%s: u4Reserve= 0x%08X.\n", __FUNCTION__, u4Data);
    u4Data = (_pu1SmBuf[5] << 8) | (_pu1SmBuf[4]);
    LOG(1, "%s: u2InvalidData = 0x%04X.\n", __FUNCTION__, u4Data);
    u4Data = (_pu1SmBuf[7] << 8) | (_pu1SmBuf[6]);
    LOG(1, "%s: u2BlkAddr1 = 0x%04X.\n", __FUNCTION__, u4Data);
    u4Ecc2 = (_pu1SmBuf[10] << 16) | (_pu1SmBuf[9] << 8) | (_pu1SmBuf[8]);
    LOG(1, "%s: ucECC2 = 0x%X.\n", __FUNCTION__, u4Ecc2);
    u4Data = (_pu1SmBuf[12] << 8) | (_pu1SmBuf[11]);
    LOG(1, "%s: u2BlkAddr2 = 0x%04X.\n", __FUNCTION__, u4Data);
    u4Ecc1 = (_pu1SmBuf[15] << 16) | (_pu1SmBuf[14] << 8) | (_pu1SmBuf[13]);
    LOG(1, "%s: ucECC1 = 0x%X.\n", __FUNCTION__, u4Ecc1);
    UNUSED(u4Data);

    // read CIS data;
#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_ReadPage(u1CISBlk, 0, _pu1SmBuf, prSMXD->u2PageSz);
#else
    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, prSMXD->u2PageSz);
    i4Ret = NANDXDHW_ReadPage(u1CISBlk, 0, _pu1SmBuf, prSMXD->u2PageSz);
#endif

    if (S_OK != i4Ret)
    {
        return i4Ret;
    }

    VERIFY(NULL != x_strcpy((CHAR *)_aszSMXDModel, (const CHAR *)&_pu1SmBuf[SMXD_MANU_NAME]));
    VERIFY(NULL != x_strcpy((CHAR *)prSMXD->au1ManuName, (const CHAR *)&_pu1SmBuf[SMXD_MANU_NAME]));
    LOG(1, "%s: SMXD_MANU_NAME = %s.\n", __FUNCTION__, &_pu1SmBuf[SMXD_MANU_NAME]);
    VERIFY(NULL != x_strcat((CHAR *)_aszSMXDModel, (const CHAR *)&_pu1SmBuf[SMXD_PROD_NAME]));
    VERIFY(NULL != x_strcpy((CHAR *)prSMXD->au1ProdName, (const CHAR *)&_pu1SmBuf[SMXD_PROD_NAME]));
    LOG(1, "%s: SMXD_PROD_NAME = %s.\n", __FUNCTION__, &_pu1SmBuf[SMXD_PROD_NAME]);
    VERIFY(NULL != x_strcat((CHAR *)_aszSMXDModel, (const CHAR *)&_pu1SmBuf[SMXD_VERS_NAME]));
    VERIFY(NULL != x_strcpy((CHAR *)prSMXD->au1VersName, (const CHAR *)&_pu1SmBuf[SMXD_VERS_NAME]));
    LOG(1, "%s: SMXD_VERS_NAME = %s.\n", __FUNCTION__, &_pu1SmBuf[SMXD_VERS_NAME]);
    LOG(1, "%s: cat string = %s.\n", __FUNCTION__, _aszSMXDModel);

    // ECC calculate and check.
    _SMXD_MakeEcc256(au1Ecc, (UINT32 *)&_pu1SmBuf[0]);
    u4Ecc3 = (au1Ecc[2] << 16) | (au1Ecc[1] << 8) | au1Ecc[0];
    LOG(1, "%s: calculate ucECC1 = 0x%X.\n", __FUNCTION__, u4Ecc3);

    // check 0 - 255 bytes
    if ((u4Ecc2 != 0xFFFFFF) && (u4Ecc3 != 0xFFFFFF) && (u4Ecc3 != u4Ecc2))
    {
        // if error, check 256-511 byts.
        _SMXD_MakeEcc256(au1Ecc, (UINT32 *)&_pu1SmBuf[256]);

        u4Ecc4 = (au1Ecc[2] << 16) | (au1Ecc[1] << 8) | au1Ecc[0];
        LOG(1, "%s: calculate ucECC2 = 0x%X.\n", __FUNCTION__, u4Ecc4);

        // check 256 - 512 bytes
        if (u4Ecc4 != u4Ecc1)
        {
            return E_FAIL;
        }
    }

    // check the first 10 bytes of CIS.
    if (x_memcmp((VOID *)_pu1SmBuf, (const VOID *)_au1SmCISField, 10) != 0)
    {
        LOG(1, "%s: the first 10 bytes of CIS Fail.\n", __FUNCTION__);
        return E_FAIL;
    }

    LOG(1, "%s: the first 10 bytes of CIS OK.\n", __FUNCTION__);
    return S_OK;
}

//-------------------------------------------------------------------------
/** _IsRdntEmpty
 *  check all datas in redundancy area are blank.
 *  @param  pu1Rdnt		data pointer to the redundancy area (16 bytes).
 *  @retval  TRUE 	success.
 *  @retval  FALSE	fail.
 */
//-------------------------------------------------------------------------
static BOOL _IsRdntEmpty(const UINT8 *pu1Rdnt)
{
    UINT32 i;

    ASSERT(pu1Rdnt != NULL);

    for (i = 0; i < SMXD_RDNT_SIZE; i++)
    {
        if (pu1Rdnt[i] != SMXD_BLANK_BYTE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

#ifndef SMXD_READONLY_MODE

//-------------------------------------------------------------------------
/** _MakeBlkAddr
 *  Transfer logical address into the format of SM/XD block address field.
 *  NOTICE: the logical address must between 0 - 999. If not follow this rule, it may cause write data fail.
 *  @param  pu1BlkAddr		pointer to block address buffer.
 *  @param  u4LogBlk		logical block address.
 *  @retval  S_OK 	success.
 *  @retval  others	fail.
 */
//-------------------------------------------------------------------------

static INT32 _MakeBlkAddr(UINT8 *pu1BlkAddr, UINT32 u4LogBlk)
{
    UINT16 u2BlkAddr = 0x0010;

    UINT32 i;
    UINT16 u2Parity = 0;
    UINT16 u2LogBlk = SMXD_MOD_LOG_BLK(u4LogBlk);

    ASSERT(pu1BlkAddr != NULL);

    // Parity check P.
    for (i = 0; i < 10; i++)
    {
        u2Parity ^= (u2LogBlk >> i) & 0x01;
    }

    // odd parity.
    u2Parity ^= 0x1;

    // BA9, BA8, BA7.
    u2BlkAddr |= (u2LogBlk >> 7);
    // BA0-BA6.
    u2BlkAddr |= (((u2LogBlk << 1) | (u2Parity)) & 0x00FF) << 8;

    pu1BlkAddr[0] = u2BlkAddr & 0xFF;
    pu1BlkAddr[1] = (u2BlkAddr >> 8) & 0xFF;

    return S_OK;
}
#endif

//-------------------------------------------------------------------------
/** _GetBlkAddr
 *  Get logical block address from redundancy area.
 *  @param  pu1Rdnt		data pointer to the redundancy area (16 bytes).
 *  @retval   modulo logical block number.
 */
//-------------------------------------------------------------------------

static UINT16 _GetBlkAddr(const UINT8 *pu1Rdnt)
{
    ASSERT(pu1Rdnt != NULL);

    return (UINT16)(((pu1Rdnt[6] & 0x07) << 7) | (pu1Rdnt[7] >> 1));
}

#ifndef SMXD_READONLY_MODE

//-------------------------------------------------------------------------
/** _GetNewPhyBlk
 *  Get new block numberf  in the same zone from unused block table.
 *  @param  u4BlkAddr		pointer to the new block number buffer.
 *  @retval  S_OK 	success.
 *  @retval  others	fail.
 */
//-------------------------------------------------------------------------

static INT32 _GetNewPhyBlk(UINT32 *u4BlkAddr)
{
    SMXD_T *prSMXD;

    UINT16 u2ModPhyBlk = 0;

    ASSERT(u4BlkAddr != NULL);

    prSMXD = FCI_GetSMXDSlot();

    // allocate a new physical block for storage.
    SMXD_UNUSED_BLK_POP(u2ModPhyBlk);

    // add the zone offset to real physical address.
    *u4BlkAddr = SMXD_REAL_PHY_BLK(prSMXD->u2NowZone, u2ModPhyBlk);

    return S_OK;
}
#endif

#ifndef SMXD_READONLY_MODE

//-------------------------------------------------------------------------
/** _UpdateBlkAddr
 *    After logical block is updated, update physical block number in the logical to physical mapping table.
 *    And erase the old physical block.
 *  @param  u4LogBlk		logical block number.
 *  @param  u4NewPhyBlk		new physical block number.
 *  @param  u4OldPhyBlk		old physical block number.
 *  @retval  S_OK 	success.
 *  @retval  others	fail.
 */
//-------------------------------------------------------------------------

static INT32 _UpdateBlkAddr(UINT32 u4LogBlk, UINT32 u4NewPhyBlk, UINT32 u4OldPhyBlk)
{
    SMXD_T *prSMXD;

    INT32 i4Ret;

    prSMXD = FCI_GetSMXDSlot();

    // check variable.
    if (prSMXD->u2NowZone != SMXD_LOG_ZONE(u4LogBlk))
    {
        return E_FAIL;
    }

    //old physical block is not empty, erase it and push back to the unused block table.
    if (u4OldPhyBlk != SMXD_PHY_BLK_BLANK_NO)
    {

#ifndef SMXD_USING_NAND_IF

        i4Ret = SMXDHW_EraseBlock(u4OldPhyBlk);
#else

        i4Ret = NANDXDHW_EraseBlock(u4OldPhyBlk);
#endif

        if (S_OK != i4Ret)
        {
            return E_FAIL;
        }

        // push the erased block to table.
        SMXD_UNUSED_BLK_PUSH(SMXD_MOD_PHY_BLK(u4OldPhyBlk));
    }

    SMXD_LPTBL_WRITE(SMXD_MOD_PHY_BLK(u4NewPhyBlk), SMXD_MOD_LOG_BLK(u4LogBlk));

    return S_OK;
}

#endif

//-------------------------------------------------------------------------
/** _BuildTable
 *    Build logical to physical mapping table in one zone.
 *  @param  prSMXD			pointer to SM/xD card information structure.
 *  @param  u2NowZone		the handling zone number.
 *  @retval  S_OK 	success.
 *  @retval  others	fail.
 */
//-------------------------------------------------------------------------

/** Build logical to physical mapping table in one zone.
 */

static INT32 _BuildTable(SMXD_T *prSMXD, UINT16 u2NowZone)
{
    UINT16 u2Blk;

    UINT16 u2PlrBlk;
    UINT16 u2ModPhyBlk;
    UINT16 u2LogBlk;
    UINT32 u4PhyBlk;
    INT32 i4Ret;

    ASSERT(prSMXD != NULL);

    if (u2NowZone > prSMXD->u2TotalZone)
    {
        LOG(1, "%s: BuildTable e1.\n", __FUNCTION__);
        return E_FAIL;
    }

    if (prSMXD->u2NowZone == u2NowZone)
    {
        LOG(7, "%s: BuildTable exist OK.\n", __FUNCTION__);
        return S_OK;
    }

    // initialize all variable.
    SMXD_LPTBL_RESET

    SMXD_UNUSED_BLK_RESET

    for (u2Blk = 0; u2Blk < SMXD_PHY_BLK_PER_ZONE; u2Blk++)
    {
        u4PhyBlk = SMXD_REAL_PHY_BLK(u2NowZone, u2Blk);

#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_ReadRdnt(u4PhyBlk, SMXD_FIRST_PAGE_NO, _pu1SmBuf);
#else
        HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
        i4Ret = NANDXDHW_ReadRdnt(u4PhyBlk, SMXD_FIRST_PAGE_NO, _pu1SmBuf);
#endif

        // read block redundant area.
        if (S_OK != i4Ret)
        {
            return E_FAIL;
        }

        // check redundancy is empty or not and build logical to physical table.
        if (!_IsRdntEmpty((const UINT8 *)_pu1SmBuf))
        {
            if (_ChkDataRdnt((const UINT8 *)_pu1SmBuf))
            {
                u2LogBlk = _GetBlkAddr((const UINT8 *)_pu1SmBuf);

                if (u2LogBlk >= SMXD_LOG_BLK_PER_ZONE)
                {
                    return E_FAIL;
                }
                if (SMXD_LPTBL_CHK(u2LogBlk))
                {
                    SMXD_LPTBL_WRITE(u2Blk, u2LogBlk);
                }
                else
                {
                    SMXD_LPTBL_READ(u2ModPhyBlk, u2LogBlk);

                    // power loss recovery.
                    u2PlrBlk = _PlrPhyBlk(u2NowZone, u2ModPhyBlk, u2Blk);

                    if (u2PlrBlk == SMXD_LOG_BLK_BLANK_NO)
                    {
                        return E_FAIL;
                    }
                    SMXD_LPTBL_WRITE(u2PlrBlk, u2LogBlk);
                }
            }
        }

#ifndef SMXD_READONLY_MODE

        else
        {
#ifndef SMXD_USING_NAND_IF
            i4Ret = SMXDHW_EraseBlock(u4PhyBlk);
#else
            i4Ret = NANDXDHW_EraseBlock(u4PhyBlk);
#endif

            if (S_OK != i4Ret)
            {
                return E_FAIL;
            }

            // push the empty block to table.

            SMXD_UNUSED_BLK_PUSH(u2Blk);
        }
#endif

    }

    //SMXD_LPTBL_CHK_LOOP
    SMXD_UNUSED_BLK_CHK_LOOP;
    prSMXD->u2NowZone = u2NowZone;

#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_SetStartPtr();
#else
        HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
        //i4Ret = NANDXDHW_SetStartPtr();
        i4Ret = NANDXDHW_ReadPage(0, 0, (UINT8 *)_pu1SmBuf, SMXD_PAGE_SIZE);
#endif

    if (S_OK != i4Ret)
    {
        return E_FAIL;
    }

    LOG(7, "%s: BuildTable OK.\n", __FUNCTION__);

    return S_OK;
}

//-------------------------------------------------------------------------
/** _TransBlkAddr
 *    translate logical block number to physical block number.
 *  @param  u4LogBlk		logical block number.
 *  @retval  physcal block number.
 *  @retval  SMXD_PHY_BLK_BLANK_NO fail.
 */
//-------------------------------------------------------------------------
static UINT32 _TransBlkAddr(UINT32 u4LogBlk)
{
    SMXD_T *prSMXD;

    UINT16 u2NowZone;
    UINT16 u2ModPhyBlk;

    prSMXD = FCI_GetSMXDSlot();

    u2NowZone = SMXD_LOG_ZONE(u4LogBlk);

    // logical mapping table is not built in this zone.
    if (prSMXD->u2NowZone != u2NowZone)
    {
        // build up logical to physical block map in this zone.
        if (S_OK != _BuildTable(prSMXD, u2NowZone))
        {
            return SMXD_PHY_BLK_BLANK_NO;
        }
    }

    SMXD_LPTBL_READ(u2ModPhyBlk, SMXD_MOD_LOG_BLK(u4LogBlk));

    if (u2ModPhyBlk != SMXD_LOG_BLK_BLANK_NO)
    {
        // add the zone offset to real physical address.
        return SMXD_REAL_PHY_BLK(u2NowZone, u2ModPhyBlk);
    }

    return SMXD_PHY_BLK_BLANK_NO;
}

//---------------------------------------------------------------------------
// Inter-file functions (use in emulation chip set.)
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _SMXD_ReadBlock
*  Read data block in CLI test.
*  @param  u4StBlock		write block number.
*  @param  u4MemPtr		destination memory address.
*  @param  u4NoPage		number page to be read.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_ReadBlock(UINT32 u4StBlock, UINT32 u4MemPtr, UINT32 u4NoPage)
{
    INT32 i4Ret = 0;

    UINT64 u8Offset;
    UINT32 u4MemLen;
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();

    u8Offset = (UINT64)u4StBlock * (UINT64)prSMXD->u2BlkSz * (UINT64)prSMXD->u2PageSz;

    u4MemLen = u4NoPage * prSMXD->u2PageSz;

    i4Ret = SMXD_Read(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _SMXD_WriteBlock
*  Write data block in CLI test.
*  @param  u4StBlock		write block number.
*  @param  u4MemPtr		source memory address.
*  @param  u4NoPage		number page to be written.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_WriteBlock(UINT32 u4StBlock, UINT32 u4MemPtr, UINT32 u4NoPage)
{
    INT32 i4Ret = 0;

    UINT64 u8Offset;
    UINT32 u4MemLen;
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();

    u8Offset = (UINT64)u4StBlock * (UINT64)prSMXD->u2BlkSz * (UINT64)prSMXD->u2PageSz;

    u4MemLen = u4NoPage * prSMXD->u2PageSz;

    i4Ret = SMXD_Write(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** _SMXD_Reset
*  Reset the card  in CLI test.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_Reset(VOID)
{
#ifndef SMXD_USING_NAND_IF
    return SMXDHW_Reset();
#else
    return NANDXDHW_Reset();
#endif
}

//-------------------------------------------------------------------------
/** _SMXD_StressTest
*  Read, Erase, Write, and data compare in whole card  in CLI test.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_StressTest(VOID)
{
    INT32 i4Ret = 0;

    UINT32 i = 0;
    UINT32 j = 0;
    SMXD_T *prSMXD;
    UINT32 u4MemPtr = 0x3000000;
    UINT32 u4SrcPtr = 0x1000;
    UINT32 u4Data;
    UINT32 u4Data1;

    prSMXD = FCI_GetSMXDSlot();

    for (i = 0; i < prSMXD->u4LogBlkNs; i++)
    {
        LOG(1, "ST: SMXD_Write block = %d.\n", i);

        i4Ret = SMXD_Write((UINT64)i *prSMXD->u2BlkSz *prSMXD->u2PageSz, u4SrcPtr,
                           prSMXD->u2BlkSz *prSMXD->u2PageSz);

        if (S_OK != i4Ret)
        {
            LOG(1, "ST: SMXD_Write fail.\n");

            LOG(1, "ST: ERROR block number = %d.\n", i);
            return E_FAIL;
        }

        LOG(1, "ST: SMXD_Read block = %d.\n", i);

        i4Ret = SMXD_Read((UINT64)i * (UINT64)prSMXD->u2BlkSz * (UINT64)prSMXD->u2PageSz,
        u4MemPtr,
        prSMXD->u2BlkSz * prSMXD->u2PageSz);

        if (S_OK != i4Ret)
        {
            LOG(1, "ST: SMXD_Read fail.\n");

            LOG(1, "ST: ERROR block number = %d.\n", i);
            return E_FAIL;
        }

        // compare src data with dest data.
        for (j = 0; j < (prSMXD->u2BlkSz *prSMXD->u2PageSz); j += 4)
        {
            u4Data = *((UINT32 *)(u4MemPtr + j));

            u4Data1 = *((UINT32 *)(u4SrcPtr + j));
            if (u4Data != u4Data1)
            {
                LOG(1, "ST: 0x%08X not the same.\n", u4MemPtr + j);

                LOG(1, "ST: ERROR block number = %d.\n", i);
                return E_FAIL;
            }
        }
        LOG(1, "ST: test pass blk = %d.\n", i);
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _SMXD_PageUnitTest
*  Read, Erase, Write, and data compare in one page of card  in CLI test.
*  @param  u4Blk 	test block number.
*  @param  u4Page 	test page number.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_PageUnitTest(UINT32 u4Blk, UINT32 u4Page)
{
#if 0
    INT32 i4Ret = 0;

    UINT32 j = 0;
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();
    FCIHW_ResetSlot(SMM_SLOT);

#ifndef SMXD_USING_NAND_IF
    SMXDHW_Init();

    i4Ret = SMXDHW_CheckCard();
#else
    i4Ret = NANDXDHW_CheckCard();
#endif
    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDIdentify fail.\n");
        return i4Ret;
    }

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_Reset();
#else
    i4Ret = NANDXDHW_Reset();
#endif

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDReset fail.\n");
        return i4Ret;
    }

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_EraseBlock(u4Blk);
#else
    i4Ret = NANDXDHW_EraseBlock(u4Blk);
#endif

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDEraseBlock fail.\n");
        return i4Ret;
    }
#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_ReadPage(u4Blk, u4Page, _pu1SmBuf, prSMXD->u2PageSz);
#else
    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, prSMXD->u2PageSz);
    i4Ret = NANDXDHW_ReadPage(u4Blk, u4Page, _pu1SmBuf, prSMXD->u2PageSz);
#endif

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDReadPage fail.\n");
        return i4Ret;
    }

    // check data.
    for (j = 0; j < prSMXD->u2PageSz; j++)
    {
        if (_pu1SmBuf[j] != SMXD_BLANK_BYTE)
        {
            // erase block error.
            return E_FAIL;
        }
    }

    // initialize data.
    for (j = 0; j < prSMXD->u2PageSz; j++)
    {
        _pu1SmBuf[j] = (UINT8)(j & 0xFF);
    }

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_WritePage(u4Blk, u4Page, _pu1SmBuf);
#else
    HalFlushInvalidateDCacheMultipleLine((UINT32)_pu1SmBuf, 512);
    i4Ret = NANDXDHW_WritePage(u4Blk, u4Page, _pu1SmBuf);
#endif

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDWritePage fail.\n");
        return i4Ret;
    }

    // initialize data.
    for (j = 0; j < prSMXD->u2PageSz; j++)
    {
        _pu1SmBuf[j] = 0;
    }

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_ReadPage(u4Blk, u4Page, _pu1SmBuf, prSMXD->u2PageSz);
#else
    HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, prSMXC->u2PageSz);
    i4Ret = NANDXDHW_ReadPage(u4Blk, u4Page, _pu1SmBuf, prSMXD->u2PageSz);
#endif


    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDReadPage fail.\n");
        return i4Ret;
    }

    // check data.
    for (j = 0; j < prSMXD->u2PageSz; j++)
    {
        if (_pu1SmBuf[j] != (UINT8)(j & 0xFF))
        {
            // write data error.
            return E_FAIL;
        }
    }
#endif
    return S_OK;
}

//-------------------------------------------------------------------------
/** _SMXD_EraseAll
*  Erase whole card  in CLI test.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_EraseAll(VOID)
{
    INT32 i4Ret = 0;

    UINT32 i = 0;
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();

    LOG(7, "EA: SMXDEraseBlock Begin.\n");

    // erase all the data of blocks.
    // xd is from 1 block. 0 block is CIS block.
    for (i = 1; i < prSMXD->u4PhyBlkNs; i++)
    {
#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_EraseBlock(i);
#else
        i4Ret = NANDXDHW_EraseBlock(i);
#endif

        // erase all the block.
        if (S_OK != i4Ret)
        {
            return i4Ret;
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _SMXD_ReadAll
*  Read whole card  in CLI test.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_ReadAll(VOID)
{
    INT32 i4Ret = 0;

    UINT32 i = 0;
    UINT32 j = 0;
    SMXD_T *prSMXD;
    UINT32 *pbDst = (UINT32 *)0x1000000;
    UINT32 u4Data;

    prSMXD = FCI_GetSMXDSlot();

    LOG(7, "RA: SMXD_Read Begin.\n");

    // read all the data of blocks.
    for (i = 0; i < prSMXD->u4PhyBlkNs; i++)
    {
        i4Ret = SMXD_Read((UINT64)i * (UINT64)prSMXD->u2BlkSz *(UINT64)prSMXD->u2PageSz,
        (UINT32)pbDst,
        prSMXD->u2BlkSz * prSMXD->u2PageSz);

        if (S_OK != i4Ret)
        {
            LOG(1, "RA: SMXD_Read block = %d ERROR.\n", i);
            return i4Ret;
        }

        // check card data is cleaned ?
        for (j = 0; j < ((prSMXD->u2PageSz *prSMXD->u2BlkSz) /4); j++)
        {
            u4Data = *((UINT32 *)(pbDst + j));
            if (SMXD_BLANK_DWORD != u4Data)
            {
                LOG(1, "RA: (%d): (%d) u4Data  = 0x%08X.\n", i, j, u4Data);
            }
        }
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** _SMXD_WriteMultiBlk
*  Multiple block write (X4) test of card  in CLI test
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 _SMXD_WriteMultiBlk(VOID)
{
    // under development.

#if 0

    INT32 i4Ret = 0;

    UINT32 i = 0;
    UINT32 j = 0;
    SMXD_T *prSMXD;
    UINT8 *pbDst = (UINT8 *)0x1000000;

    prSMXD = FCI_GetSMXDSlot();

    // xd is from 1 block. 0 block is
    //for (i=0; i < prSMXD->u4PhyBlkNs; i++)
    for (i = 0; i < 4; i++)
    {
#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_EraseBlock(i);
#else
        i4Ret = NANDXDHW_EraseBlock(i);
#endif

        // erase all the block.
        if (S_OK != i4Ret)
        {
            return i4Ret;
        }
    }

    // initialize data buffer.
    VERIFY(NULL != x_memset((VOID *)pbDst, 0x55, 0x10800));

    // write 4 blocks == 128 pages.
    i4Ret = SMXDHW_WriteMultiBlkTest(pbDst);

    if (S_OK != i4Ret)
    {
        return i4Ret;
    }

    // initialize data buffer.
    VERIFY(NULL != x_memset((VOID *)pbDst, 0x0, 0x10800));

    // check data.
    for (i = 0; i < 4; i++)      // block.
    {
        for (j = 0; j < 32; j++) // page
        {
#ifndef SMXD_USING_NAND_IF
            i4Ret = SMXDHW_ReadPage(i, j, pbDst);
#else
            i4Ret = NANDXDHW_ReadPage(i, j, pbDst);
#endif

            if (S_OK != i4Ret)
            {
                return i4Ret;
            }
            pbDst += prSMXD->u2PageSz;
        }
    }
#endif

    return S_OK;
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** SMXD_Read
*  SM/xD card read function.
*  @param  u8Offset     read offset from card, u8Offset should be multiple of page size (512 bytes).
*  @param  u4MemPtr      user's read buffer address.
*  @param  u4MemLen      read length, u4MemLen should be multiple of page size (512 bytes).
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SMXD_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = 0;

    UINT32 u4ReadPtr;
    UINT32 i;
    UINT16 u2Page;
    UINT32 u4PageNs;
    UINT32 u4LogBlk;
    UINT32 u4PhyBlk;
    SMXD_T *prSMXD;
    UINT32 u4Offset = (UINT32)u8Offset;

    // make sure buffer address = multiple of 16 bytes.
    // NAND dma limitation.
    ASSERT((u4MemPtr & 0xF) == 0);

    prSMXD = FCI_GetSMXDSlot();

    FCIHW_ResetSlot(SMM_SLOT);

    u4ReadPtr = u4MemPtr;

    // read by page basis.
    if (((u4Offset %prSMXD->u2PageSz) != 0) && ((u4MemLen %prSMXD->u2PageSz) != 0))
    {
        // parameter error.
        return E_FAIL;
    }

    u4LogBlk = (UINT32)(u4Offset /SMXD_BLK_SIZE);
    u2Page = (u4Offset /SMXD_PAGE_SIZE) % SMXD_PAGE_PER_BLK;
    u4PageNs = u4MemLen / SMXD_PAGE_SIZE;

    u4PhyBlk = _TransBlkAddr(u4LogBlk);

    if (u4PhyBlk == SMXD_PHY_BLK_BLANK_NO)
    {
        return E_FAIL;
    }

    for (i = 0; i < u4PageNs; i++)
    {
        if (u2Page >= SMXD_PAGE_PER_BLK)
        {
            u2Page = 0;

            u4LogBlk++;

            u4PhyBlk = _TransBlkAddr(u4LogBlk);
            if (u4PhyBlk == SMXD_PHY_BLK_BLANK_NO)
            {
                return E_FAIL;
            }
        }

        //initialize buffer.
        //VERIFY(NULL != x_memset((VOID * )_pu1SmBuf, 0, SMXD_TOTAL_PAGE_SIZE));

#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_ReadPage(u4PhyBlk, u2Page, (UINT8 *)u4ReadPtr, SMXD_PAGE_SIZE);
#else
        HalFlushDCacheMultipleLine((UINT32)u4ReadPtr, SMXD_PAGE_SIZE);
        i4Ret = NANDXDHW_ReadPage(u4PhyBlk, u2Page, (UINT8 *)u4ReadPtr, SMXD_PAGE_SIZE);
#endif

        if (S_OK != i4Ret)
        {
            return i4Ret;
        }

        // copy data to buffer.
/*
        if (NULL == x_memcpy((VOID *)u4ReadPtr, (const VOID *)_pu1SmBuf, SMXD_PAGE_SIZE))
        {
            return E_FAIL;
        }
*/
        u4ReadPtr += SMXD_PAGE_SIZE;
        u2Page++;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** SMXD_Write
*  SM/xD card write function.
*  @param  u8Offset     write offset from card, u8Offset should be multiple of page size (512 bytes).
*  @param  u4MemPtr      user's write buffer address.
*  @param  u4MemLen      write length, u4MemLen should be multiple of page size (512 bytes).
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SMXD_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{

#ifdef SMXD_READONLY_MODE

    UNUSED(u8Offset);

    UNUSED(u4MemPtr);
    UNUSED(u4MemLen);

    return E_FAIL;
#else

    INT32 i4Ret = 0;

    UINT32 i;
    UINT16 u2StartPage;
    UINT16 u2WritePage;
    UINT32 u4PageNs;
    UINT32 u4LogBlk;
    UINT32 u4NewPhyBlk = 0;
    UINT32 u4OldPhyBlk;
    SMXD_T *prSMXD;
    UINT32 u4Offset = (UINT32)u8Offset;

    // make sure buffer address = multiple of 16 bytes.
    // NAND dma limitation.
    ASSERT((u4MemPtr & 0xF) == 0);

    prSMXD = FCI_GetSMXDSlot();

    FCIHW_ResetSlot(SMM_SLOT);

    // write by page basis.
    if (((u4Offset %prSMXD->u2PageSz) != 0) && ((u4MemLen %prSMXD->u2PageSz) != 0))
    {
        // parameter error.
        return E_FAIL;
    }

    u4LogBlk = (UINT32)(u4Offset /SMXD_BLK_SIZE);
    u2StartPage = (u4Offset /SMXD_PAGE_SIZE) % SMXD_PAGE_PER_BLK;
    u4PageNs = u4MemLen / SMXD_PAGE_SIZE;

    while (u4PageNs)
    {
        // get old physical block.
        //RE_WRITE:
        u4OldPhyBlk = _TransBlkAddr(u4LogBlk);

        ASSERT((u2StartPage %SMXD_PAGE_PER_BLK) == 0);
        // old block not empty, get new physical block.
        VERIFY(S_OK == _GetNewPhyBlk(&u4NewPhyBlk));

        // check if the updated pages is across two blocks.

        u2WritePage = ((u2StartPage + u4PageNs) > SMXD_PAGE_PER_BLK)
                          ? (SMXD_PAGE_PER_BLK - u2StartPage) : u4PageNs;

        for (i = 0; i < SMXD_PAGE_PER_BLK; i++)
        {
            //LOG(7, "SMXD_Write: u4PhyBlk=%d, u4LogBlk=%d, u2Page=%d.\n", u4PhyBlk, u4LogBlk, u2Page);

            // copy new page data from user's buffer to local page buffer.
            if ((i >= u2StartPage) && (i < (u2StartPage + u2WritePage)))
            {
                // copy data to buffer.
                VERIFY(NULL != x_memcpy((VOID *)_pu1SmBuf, (const VOID *)u4MemPtr, SMXD_PAGE_SIZE));

                u4MemPtr += SMXD_PAGE_SIZE;
                //calculate 0 -255 bytes ecc.
                _SMXD_MakeEcc256(&_pu1SmBuf[SMXD_RDNT_ECC1_INDEX],
                                 (UINT32 *)&_pu1SmBuf[SMXD_DATA1_INDEX]);
                //calculate 256 -511 bytes ecc.
                _SMXD_MakeEcc256(&_pu1SmBuf[SMXD_RDNT_ECC2_INDEX],
                                 (UINT32 *)&_pu1SmBuf[SMXD_DATA2_INDEX]);
                // set reserved area to 0xFF.
                _pu1SmBuf[SMXD_RDNT_REV_INDEX] = SMXD_BLANK_BYTE;
                _pu1SmBuf[SMXD_RDNT_REV_INDEX + 1] = SMXD_BLANK_BYTE;
                _pu1SmBuf[SMXD_RDNT_REV_INDEX + 2] = SMXD_BLANK_BYTE;
                _pu1SmBuf[SMXD_RDNT_REV_INDEX + 3] = SMXD_BLANK_BYTE;
                // data status bytes.
                _pu1SmBuf[SMXD_RDNT_DATA_STATUS_INDEX] = SMXD_BLANK_BYTE;
                // block status bytes.
                _pu1SmBuf[SMXD_RDNT_BLK_STATUS_INDEX] = SMXD_BLANK_BYTE;
                // make block address at redundancy.
                VERIFY(S_OK == _MakeBlkAddr(&_pu1SmBuf[SMXD_RDNT_BLK_ADDR1_INDEX], u4LogBlk));

                _pu1SmBuf[SMXD_RDNT_BLK_ADDR2_INDEX] = _pu1SmBuf[SMXD_RDNT_BLK_ADDR1_INDEX];
                _pu1SmBuf[SMXD_RDNT_BLK_ADDR2_INDEX + 1] = _pu1SmBuf[SMXD_RDNT_BLK_ADDR1_INDEX + 1];
            }
            else // copy original page data in card to local page buffer.
            {
                //initialize buffer.
                //VERIFY(NULL != x_memset((VOID * )_pu1SmBuf, 0, SMXD_TOTAL_PAGE_SIZE));

#ifndef SMXD_USING_NAND_IF
                i4Ret = SMXDHW_ReadPage(u4OldPhyBlk, i, _pu1SmBuf, SMXD_TOTAL_PAGE_SIZE);
#else
                HalFlushDCacheMultipleLine((UINT32)_pu1SmBuf, SMXD_TOTAL_PAGE_SIZE);
                i4Ret = NANDXDHW_ReadPage(u4OldPhyBlk, i, _pu1SmBuf, SMXD_TOTAL_PAGE_SIZE);
#endif
                if (S_OK != i4Ret)
                {
                    return i4Ret;
                }
            }

            // write local page buffer data to new block.
#ifndef SMXD_USING_NAND_IF
            i4Ret = SMXDHW_WritePage(u4NewPhyBlk, i, _pu1SmBuf);
/*
            if (S_OK != i4Ret)
            {
                return i4Ret;
            }

            x_memset((VOID *)0x3000000, 0x55, SMXD_TOTAL_PAGE_SIZE);
            i4Ret = SMXDHW_ReadPage(u4NewPhyBlk, i, (UINT8 *)0x3000000, SMXD_TOTAL_PAGE_SIZE);
*/
#else
            HalFlushInvalidateDCacheMultipleLine(_pu1SmBuf, 512);
            i4Ret = NANDXDHW_WritePage(u4NewPhyBlk, i, _pu1SmBuf);
/*
            if (S_OK != i4Ret)
            {
                return i4Ret;
            }
            x_memset((VOID *)0x3000000, 0x55, SMXD_TOTAL_PAGE_SIZE);
            HalFlushDCacheMultipleLine(0, 512);
            i4Ret = NANDXDHW_ReadPage(u4NewPhyBlk, i, (UINT8 *)0x3000000, SMXD_TOTAL_PAGE_SIZE);
*/
#endif

            if (S_OK != i4Ret)
            {
                //goto RE_WRITE;
                return i4Ret;
            }
        }

        // update logical block address table in this zone.
        VERIFY(S_OK == _UpdateBlkAddr(u4LogBlk, u4NewPhyBlk, u4OldPhyBlk));
        // prepare for new block update.

        u4PageNs -= u2WritePage;
        u2StartPage = 0;
        u4LogBlk++;
    }

    return S_OK;
#endif

}

//-------------------------------------------------------------------------
/** SMXD_Erase
*  SM/xD card erase function.
*  @param  u4BlkIdx     physical block index.
*  @param  u4BlkNum   total number of erased block.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SMXD_Erase(UINT32 u4BlkIdx, UINT32 u4BlkNum)
{

#ifdef SMXD_READONLY_MODE

    UNUSED(u4BlkIdx);

    UNUSED(u4BlkNum);

    return E_FAIL;
#else

    INT32 i4Ret = 0;

    UINT32 i;

    for (i = u4BlkIdx; i < (u4BlkIdx + u4BlkNum); i++)
    {
#ifndef SMXD_USING_NAND_IF
        i4Ret = SMXDHW_EraseBlock(i);
#else
        i4Ret = NANDXDHW_EraseBlock(i);
#endif

        if (S_OK != i4Ret)
        {
            return i4Ret;
        }
    }

    return S_OK;
#endif

}

//-------------------------------------------------------------------------
/** SMXD_Insert
*  SM/xD card insert function.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SMXD_Insert(VOID)
{
    INT32 i4Ret = 0;

#ifdef SMXD_USING_NAND_IF
    BOOL fgVal;
#endif

#ifdef CC_BD_FCI
    _i4SMXDStatus = (INT32)STORG_DEV_DETACH;
#elif defined(CC_DTV_FCI)
    _i4SMXDStatus = (INT32)STORG_DEV_ABSENT;
#endif

    if (SMXDHW_IsEmpty())
    {
        return E_NO_CARD;
    }

#ifndef SMXD_USING_NAND_IF
    FCIHW_ResetSlot(SMM_SLOT);

    _i4SMXDStatus = (INT32)STORG_DEV_INIT;

    SMXDHW_Init();

    i4Ret = SMXDHW_Reset();
#else
    NAND_Init();

    // set software ECC mode.
    fgVal = 1;
    // Jethro Mark - fgVal = NAND_QuerySetSwEcc(&fgVal);
    UNUSED(fgVal);

    i4Ret = NANDXDHW_Reset();
#endif

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDReset fail.\n");

        _i4SMXDStatus = (INT32)STORG_DEV_FAIL;

        return E_NO_CARD;
    }

#ifndef SMXD_USING_NAND_IF
    i4Ret = SMXDHW_CheckCard();
#else
    i4Ret = NANDXDHW_CheckCard();
#endif

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: SMXDIdentify fail.\n");

        _i4SMXDStatus = (INT32)STORG_DEV_FAIL;

        return E_NO_CARD;
    }

    // Initial pointer and make address = multiple of 16 bytes.
    if (!_pu1SmBuf)
    {
        _pu1SmBufOrg = x_mem_alloc(SMXD_TOTAL_PAGE_SIZE + 16);
        if (!_pu1SmBufOrg)
        {
            return -1;
        }
        _pu1SmBuf = (UINT8 *)((((UINT32)_pu1SmBufOrg) + 16) & (~((UINT32)0xF)));
    }

    i4Ret = _ChkPhyFmt();

    if (S_OK != i4Ret)
    {
        LOG(1, "XD_Insert: _ChkPhyFmt fail.\n");

        _i4SMXDStatus = (INT32)STORG_DEV_FAIL;

        if (_pu1SmBufOrg)
        {
            x_mem_free(_pu1SmBufOrg);
        }
        return i4Ret;
    }

    // notify block device manager.
#ifdef CC_BD_FCI
    if ((_pfnSMXDCallback) && (_i4SMXDStatus == (INT32)STORG_DEV_DETACH))
    {
        _pfnSMXDCallback((INT32)STORG_DEV_ATTACH);
    }

    _i4SMXDStatus = (INT32)STORG_DEV_ATTACH;
#elif defined(CC_DTV_FCI)
    if ((_pfnSMXDCallback) && (_i4SMXDStatus == (INT32)STORG_DEV_ABSENT))
    {
        _pfnSMXDCallback((INT32)_u4SMXDCallbackTag, (INT32)STORG_DEV_READY);
    }

    _i4SMXDStatus = (INT32)STORG_DEV_READY;
#endif
    return S_OK;
}

//-------------------------------------------------------------------------
/** SMXD_Remove
*  SM/xD card remove function.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SMXD_Remove(VOID)
{
    // notify block device manager.
#ifdef CC_BD_FCI
    if ((_pfnSMXDCallback) && (_i4SMXDStatus == (INT32)STORG_DEV_ATTACH))
    {
        _pfnSMXDCallback((INT32)STORG_DEV_DETACH);
    }

    _i4SMXDStatus = (INT32)STORG_DEV_DETACH;
#elif defined(CC_DTV_FCI)
    if ((_pfnSMXDCallback) && (_i4SMXDStatus == (INT32)STORG_DEV_READY))
    {
        _pfnSMXDCallback((INT32)_u4SMXDCallbackTag, (INT32)STORG_DEV_ABSENT);
    }

    _i4SMXDStatus = (INT32)STORG_DEV_ABSENT;
#endif
    return S_OK;
}

/*****************************************************************************
 *  SMXD Slot Query functions.
 */

//-------------------------------------------------------------------------
/** SMXD_GetModel
*  SM/xD card get model name function.
*  @param  VOID.
*  @return   SM/xD model name.
*/
//-------------------------------------------------------------------------
CHAR *SMXD_GetModel(VOID)
{
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();

    if (prSMXD->fgDetect)
    {
        return (CHAR *)_aszSMXDModel;
    }
    else
    {
        return NULL;
    }
}

//-------------------------------------------------------------------------
/** SMXD_GetCard
*  SM/xD card get card type function.
*  @param  VOID.
*  @return  the card is SM card or xD card.
*/
//-------------------------------------------------------------------------
INT32 SMXD_GetCard(VOID)
{
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();

    return prSMXD->u2CardType;
}
//-------------------------------------------------------------------------
/** SMXD_GetBlockSize
*  SMXD card get card block size function.
*  @param  VOID.
*  @return  the card block size in bytes.
*/
//-------------------------------------------------------------------------
INT32 SMXD_GetBlockSize(VOID)
{
    return (INT32)(SMXD_FCI_BLOCK_SIZE);
}


//-------------------------------------------------------------------------
/** SMXD_GetSize
*  SM/xD card get card size function.
*  @param  VOID.
*  @return  the card size.
*/
//-------------------------------------------------------------------------
INT64 SMXD_GetSize(VOID)
{
    SMXD_T *prSMXD;

    prSMXD = FCI_GetSMXDSlot();

    if (prSMXD->fgDetect)
    {
        return ((INT64)prSMXD->u4LogBlkNs * (INT64)SMXD_PAGE_PER_BLK);
    }
    else
    {
        return 0;
    }
}

#ifdef CC_DTV_FCI

//-------------------------------------------------------------------------
/** SMXD_GetStatus
*  SM/xD card get card status.
*  @param  VOID.
*  @return  the card status in STORG_DEV_STATUS_T.
*/
//-------------------------------------------------------------------------
INT32 SMXD_GetStatus(VOID)
{
    return _i4SMXDStatus;
}

//-------------------------------------------------------------------------
/** SMXD_SetHotswapCallback
* SM/xD card register card call back function when card is removed, inserted.
*  @param  u4Tag             user's tag in call back function.
*  @param  pfnCallback     user's call back function, it will be called at card inserted or removed.
*  @retval VOID.
*/
//-------------------------------------------------------------------------
VOID SMXD_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback)
{
    _u4SMXDCallbackTag = u4Tag;
    _pfnSMXDCallback = pfnCallback;
}

//-------------------------------------------------------------------------
/** SMXD_GetBlkFuncTbl
* Get SMXD block function table pointer.
*  @param  pFuncTable      define the query function that the block manager
*                                       can get a table of pointers to all IO functions.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SMXD_GetBlkFuncTbl(BlockFunc_T *pFuncTable)
{
    x_memcpy(pFuncTable, &_rSMXDBlkFuncTbl, sizeof(BlockFunc_T));
    return 0;
}

#endif // #ifdef CC_DTV_FCI
//lint -restore
#endif  // #ifdef CC_FCI_ENABLE
