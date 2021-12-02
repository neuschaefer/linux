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
 * $RCSfile: fci_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fci_if.h
 *  This header file declares the function prototype of all cards and fci module.
 */

#ifndef FCI_IF_H
#define FCI_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "storage_if.h"
#include "blkmgr_if.h"
LINT_EXT_HEADER_END

#define FCI_INT_VECTOR_ID              ((UINT32)VECTOR_FCI)
#define FCI_IO_BASE_ADDR            ((UINT32)FLASH_CARD_BASE)

#define SD_MLT_FNC_CFG 1 
#define MS_MLT_FNC_CFG  1

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#define FCI_TYPE_NONE        ((UINT32)0)
#define FCI_TYPE_MS           ((UINT32) 1 << 2)
#define FCI_TYPE_SD           ((UINT32) 1 << 3)
#define FCI_TYPE_SMXD         ((UINT32) 1 << 4)
#define FCI_TYPE_SDIO          ((UINT32) 1 << 5)

typedef struct _SDIO_DRV_IF_S SDIO_DRV_IF_T;

typedef INT32 (*sdio_init_fnct)   	(SDIO_DRV_IF_T *pt_wlan_sdio_if);
typedef INT32 (*sdio_deinit_fnct) 	(VOID);
typedef INT32 (*sdio_rb_fnct)      	(UINT16 u2Offset, UINT32 *pu4Data);
typedef INT32 (*sdio_wb_fnct)   	(UINT16 u2Offset, UINT32 u4Data);
typedef INT32 (*sdio_rp_fnct)   	(UINT8 *pucBuf, UINT16 u2Len, UINT16 u2Port);
typedef INT32 (*sdio_wp_fnct)   	(UINT8 *pucBuf, UINT16 u2Len, UINT16 u2Port);
typedef INT32 (*irq_callback_fnct)	(UINT32 u4IntStatus);

struct _SDIO_DRV_IF_S
{
    /* sdio driver assigned */
    sdio_init_fnct      pf_sdio_init;
    sdio_deinit_fnct    pf_sdio_deinit;
    sdio_rb_fnct        pf_sdio_r_bytes;
    sdio_wb_fnct        pf_sdio_w_bytes;
    sdio_rp_fnct        pf_sdio_r_port;
    sdio_wp_fnct        pf_sdio_w_port;

    /* wlan driver assigned */
    irq_callback_fnct   pf_irq_callback;
};

typedef VOID (* FCI_CALLBACK)(INT32 fgStatus);

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
/* FCI export primative APIs */
EXTERN BOOL  FCI_PowerOn(VOID);
EXTERN BOOL  FCI_PowerOff(VOID);
EXTERN INT32 FCI_RegInit(VOID);
EXTERN INT32 FCI_RegDeInit(VOID);
EXTERN INT32 FCI_Init(VOID);
EXTERN INT32 FCI_Stop(VOID);
EXTERN INT32 FCI_Reset(VOID);
#ifdef CC_CLI
EXTERN INT32 FCI_Diag(VOID);
#endif

/* SD Slot APIs. */
EXTERN INT32 SD_SwitchHighSpeed(BOOL bHighSpeed);
EXTERN INT32 SD_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 SD_Write(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 SD_Erase(UINT32 u4BlkIdx, UINT32 u4BlkNum);
EXTERN INT32 SD_DetectCardRemove(VOID);
EXTERN INT32 SD_Insert(VOID);
EXTERN INT32 SD_Remove(VOID);
EXTERN CHAR *SD_GetModel(VOID);
EXTERN INT32 SD_GetCard(VOID);
EXTERN INT32 SD_GetBlockSize(VOID);
EXTERN INT64 SD_GetSize(VOID);
EXTERN INT32 SD_GetStatus(VOID);
EXTERN VOID SD_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback);
EXTERN INT32 SD_GetBlkFuncTbl(BlockFunc_T *pFuncTable);
EXTERN INT32 SDC_GetStatus(UINT16 u2RCA, UINT32 *pu4Tmp);

/* SDIO APIs. */
EXTERN INT32 i4SdioDrvInit(SDIO_DRV_IF_T *prSdioDrvfnc);
EXTERN INT32 i4SdioDrvDeInit(VOID);

/* MS Slot APIs. */
EXTERN INT32 MS_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MS_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 MS_Erase(UINT32 u4BlkIdx, UINT32 u4BlkNum);
EXTERN INT32 MS_DetectCardRemove(VOID);
EXTERN INT32 MS_Insert(VOID);
EXTERN INT32 MS_Remove(VOID);
EXTERN CHAR *MS_GetModel(VOID);
EXTERN INT32 MS_GetCard(VOID);
EXTERN INT32 MS_GetBlockSize(VOID);
EXTERN INT64 MS_GetSize(VOID);
EXTERN INT32 MS_GetStatus(VOID);
EXTERN VOID MS_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback);
EXTERN INT32 MS_GetBlkFuncTbl(BlockFunc_T *pFuncTable);

#ifdef CC_FCI_SMXD_SUPPORT
/* SMXD Slot APIs. */
EXTERN INT32 SMXD_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 SMXD_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 SMXD_Erase(UINT32 u4BlkIdx, UINT32 u4BlkNum);
EXTERN INT32 SMXD_Insert(VOID);
EXTERN INT32 SMXD_Remove(VOID);
EXTERN CHAR *SMXD_GetModel(VOID);
EXTERN INT32 SMXD_GetCard(VOID);
EXTERN INT32 SMXD_GetBlockSize(VOID);
EXTERN INT64 SMXD_GetSize(VOID);
EXTERN INT32 SMXD_GetStatus(VOID);
EXTERN VOID SMXD_SetHotswapCallback(FCI_CALLBACK pfnCallback);
EXTERN INT32 SMXD_GetBlkFuncTbl(BlockFunc_T *pFuncTable);
#endif

#endif // FCI_IF_H
