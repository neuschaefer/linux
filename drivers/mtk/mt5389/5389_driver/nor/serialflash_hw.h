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
 * $Author: muran.xiang $
 * $Date: 2013/01/25 $
 * $RCSfile: serialflash_hw.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

#ifndef SERIAL_FLASH_HW_H
#define SERIAL_FLASH_HW_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_hal_5381.h"
#include "x_typedef.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define SFLASH_MEM_BASE         (IO_VIRT + 0x8000000)
#define SFLASH_REG_BASE         (IO_VIRT + 0x08700)

#define SFLASH_CMD_REG          ((UINT32)0x00)
#define SFLASH_CNT_REG          ((UINT32)0x04)
#define SFLASH_RDSR_REG         ((UINT32)0x08)
#define SFLASH_RDATA_REG        ((UINT32)0x0C)
#define SFLASH_RADR0_REG        ((UINT32)0x20)
#define SFLASH_RADR1_REG        ((UINT32)0x24)
#define SFLASH_RADR2_REG        ((UINT32)0x28)
#define SFLASH_WDATA_REG        ((UINT32)0x2C)
#define SFLASH_PRGDATA0_REG     ((UINT32)0x30)
#define SFLASH_PRGDATA1_REG     ((UINT32)0x34)
#define SFLASH_PRGDATA2_REG     ((UINT32)0x38)
#define SFLASH_PRGDATA3_REG     ((UINT32)0x3C)
#define SFLASH_PRGDATA4_REG     ((UINT32)0x40)
#define SFLASH_PRGDATA5_REG     ((UINT32)0x44)
#define SFLASH_SHREG0_REG       ((UINT32)0x48)
#define SFLASH_SHREG1_REG       ((UINT32)0x4C)
#define SFLASH_SHREG2_REG       ((UINT32)0x50)
#define SFLASH_SHREG3_REG       ((UINT32)0x54)
#define SFLASH_SHREG4_REG       ((UINT32)0x58)
#define SFLASH_SHREG5_REG       ((UINT32)0x5C)
#define SFLASH_CFG1_REG         ((UINT32)0x60)
#define SFLASH_CFG2_REG         ((UINT32)0x64)
#define SFLASH_CFG3_REG         ((UINT32)0x68)
#define SFLASH_STATUS0_REG      ((UINT32)0x70)
#define SFLASH_STATUS1_REG      ((UINT32)0x74)
#define SFLASH_STATUS2_REG      ((UINT32)0x78)
#define SFLASH_STATUS3_REG      ((UINT32)0x7C)
#define SFLASH_PP_DATA_REG      ((UINT32)0x80)
#define SFLASH_PREBUF_STUS_REG  ((UINT32)0x84)
#define SFLASH_SF_INTRSTUS_REG  ((UINT32)0x88)
#define SFLASH_SF_INTREN_REG    ((UINT32)0x8C)
#define SFLASH_SF_TIME_REG      ((UINT32)0xA4)
#define SFLASH_CHKSUM_CTL_REG   ((UINT32)0xA8)
#define SFLASH_CHECKSUM_REG     ((UINT32)0xAC)
#define SFLASH_CKGEN_REG        ((UINT32)0xB0)
#define SFLASH_SAMPLE_EDGE_REG  ((UINT32)0xB4)
#define SFLASH_DELAY_SELECTION  ((UINT32)0xB8)
#define SFLASH_DUAL_REG         ((UINT32)0xC0)

//-----------------------------------------------------------------------------
#define SFLASH_WRBUF_SIZE       128


//-----------------------------------------------------------------------------
#define SFLASHHWNAME_LEN    48

typedef struct
{
    UINT8   u1MenuID;
    UINT8   u1DevID1;
    UINT8   u1DevID2;
    UINT8   u1PPType;
    UINT32  u4ChipSize;
    UINT32  u4SecSize;
    UINT32  u4CEraseTimeoutMSec;

    UINT8   u1WRENCmd;
    UINT8   u1WRDICmd;
    UINT8   u1RDSRCmd;
    UINT8   u1WRSRCmd;
    UINT8   u1READCmd;
    UINT8   u1FASTREADCmd;
    UINT8   u1READIDCmd;
    UINT8   u1SECERASECmd;
    UINT8   u1CHIPERASECmd;
    UINT8   u1PAGEPROGRAMCmd;
    UINT8   u1AAIPROGRAMCmd;
    UINT8   u1DualREADCmd;
    UINT8   u1Protection;
    CHAR    pcFlashStr[SFLASHHWNAME_LEN];
} SFLASHHW_VENDOR_T;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
EXTERN INT32 SFLASHHW_Init(void);

EXTERN void SFLASHHW_SetIsr(BOOL fgSet);

EXTERN INT32 SFLASHHW_GetID(UINT32 u4Index, UINT8 *pu1MenuID,
                            UINT8 *pu1DevID1, UINT8 *pu1DevID2);
//EXTERN void SFLASHHW_GetFlashInfo(SFLASH_INFO_T *prInfo);

EXTERN INT32 SFLASHHW_Read(UINT32 u4Addr, UINT32 u4len, UINT8* pu1buf);
EXTERN INT32 SFLASHHW_WriteSector(UINT32 u4Index, UINT32 u4Addr,
                                  UINT32 u4Len, const UINT8* pu1Buf);
EXTERN INT32 SFLASHHW_EraseSector(UINT32 u1Index, UINT32 u4Addr);
EXTERN INT32 SFLASHHW_EraseChip(UINT32 u4Index);
EXTERN INT32 SFLASHHW_CalCRC(UINT32 u4Addr, UINT32 u4Len, UINT32* pu4CheckSum);

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

EXTERN INT32 SFLASHHW_ReadFlashStatus(UINT32 u4Index, UINT8 *pu1Val);
EXTERN INT32 SFLASHHW_WriteProtect(UINT32 u4Index, BOOL fgEnable);

EXTERN CHAR* SFLASHHW_GetFlashName(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2);
EXTERN UINT32 SFLASHHW_GetFlashSize(UINT8 u1MenuID, UINT8 u1DevID1, UINT8 u1DevID2);

EXTERN INT32 SFLASHHW_EnableDMA(UINT32 u4SrcAddr, UINT32 u4DestAddr, UINT32 u4Size);

EXTERN UINT32 SFLASHHW_PinMux(UINT32 u4Val);
#endif

