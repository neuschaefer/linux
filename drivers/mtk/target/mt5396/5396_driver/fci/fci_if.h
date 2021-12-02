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

/** @file fci_if.h
 *  This header file declares the function prototype of all cards and fci module.
 */

#ifndef FCI_IF_H
#define FCI_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#ifdef CC_BD_FCI
#include "x_debug.h"
#else
#define UTIL_Printf Printf
#endif

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"

#ifdef CC_BD_FCI
#include "drv_fci.h"
#include "drv_config.h"
#else // if defined(CC_DTV_FCI)
#include "storage_if.h"
#include "blkmgr_if.h"
#endif

#ifdef CC_BD_FCI
#include "chip_ver.h"
#endif

#define FCI_NEW_INTERRUPT_ARCHITECTURE    1      
#define FCI_OLD_INTERRUPT_ARCHITECTURE     0

// Currently only 8555 & 8550 use new architecture
#define FCI_INTERRUPT_ARCHITECTURE      FCI_OLD_INTERRUPT_ARCHITECTURE

#ifndef CONFIG_CHIP_VER_MT8520
#define CONFIG_CHIP_VER_MT8520 8520
#endif // #ifndef CONFIG_CHIP_VER_MT8520

#ifndef CONFIG_CHIP_VER_MT8530
#define CONFIG_CHIP_VER_MT8530 8530
#endif // #ifndef CONFIG_CHIP_VER_MT8530

#ifndef CONFIG_CHIP_VER_MT8550
#define CONFIG_CHIP_VER_MT8550 8550
#endif // #ifndef CONFIG_CHIP_VER_MT8550

#ifndef CONFIG_CHIP_VER_MT8555
#define CONFIG_CHIP_VER_MT8555 8550
#endif // #ifndef CONFIG_CHIP_VER_MT8555

#ifndef CONFIG_CHIP_VER_MT5363
#define CONFIG_CHIP_VER_MT5363 5363
#endif //#ifndef CONFIG_CHIP_VER_MT5363

#ifndef CONFIG_CHIP_VER_MT5387
#define CONFIG_CHIP_VER_MT5387 5387
#endif // #ifndef CONFIG_CHIP_VER_MT5387

#ifndef CONFIG_CHIP_VER_MT5365
#define CONFIG_CHIP_VER_MT5365 5365
#endif //#ifndef CONFIG_CHIP_VER_MT5365

#ifndef CONFIG_CHIP_VER_MT5395
#define CONFIG_CHIP_VER_MT5395 5395
#endif //#ifndef CONFIG_CHIP_VER_MT5395

#ifndef CONFIG_CHIP_VER_CURR
#ifdef CC_MT5395
#define CONFIG_CHIP_VER_CURR CONFIG_CHIP_VER_MT5395
#elif defined(CC_MT5365)
#define CONFIG_CHIP_VER_CURR CONFIG_CHIP_VER_MT5365
#elif defined(CC_MT5363)
#define CONFIG_CHIP_VER_CURR CONFIG_CHIP_VER_MT5363
#elif defined(CC_MT5387) && !defined(CC_MT5363)
#define CONFIG_CHIP_VER_CURR CONFIG_CHIP_VER_MT5387
#else
#define CONFIG_CHIP_VER_CURR CONFIG_CHIP_VER_MT8530
#endif
#endif // #ifndef CONFIG_CHIP_VER_CURR

#ifndef CC_BD_FCI   // for DTV FPGA configuration
#ifdef CC_FPGA
#define CONFIG_DRV_FPGA_BOARD 1
#else
#define CONFIG_DRV_FPGA_BOARD 0
#endif
#endif

LINT_EXT_HEADER_END

#ifdef CC_BD_FCI
/// FCI interrupt vector id.
#ifdef MT8520_USE_SD2
    #define FCI_INT_VECTOR_ID              ((UINT32)VECTOR_FCI2)
#else
    #define FCI_INT_VECTOR_ID              ((UINT32)VECTOR_FLASHCARD)
#endif
#else // if defined(CC_DTV_FCI)
#define FCI_INT_VECTOR_ID              ((UINT32)VECTOR_FCI)
#endif

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8520)
/// FCI I/O base address.
#ifdef MT8520_USE_SD2
    #define FCI_IO_BASE_ADDR            ((UINT32)FCI2_BASE)
#else
    #define FCI_IO_BASE_ADDR            ((UINT32)FCI_BASE)
#endif

// For MT8170 - SD : 3, MS : 1
// For MT8520 Public Board - SD : 6, MS : 6
// Setting for Full Layer ECO MT8520 (PCB D2-MT8520PM1-V2 MD0463)
#define SD1_MLT_FNC_CFG 3   // 6, SD1 multi-function setting (Verification board default => 3)
                                            // SD1_MLT_FNC_CFG : 1 ~ 8
#define MS_MLT_FNC_CFG  1   // 6, MS multi-function setting (Verification board default => 4)
                                            // MS_MLT_FNC_CFG : 1 ~ 6, 8, 9

#define FCI_SD_CLOCK_SELECTION_STRING    50, 27, 25, 13, 4, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [50, 27, 25, 13, 4, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 0
#ifdef MT8520_USE_SD2
#define SD_DEFAULT_NORM_CLOCK_INDEX 1
#else
#define SD_DEFAULT_NORM_CLOCK_INDEX 2
#endif

#define FCI_MSP_CLOCK_SELECTION_STRING    50, 27, 25, 20, 13, 4, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [50, 27, 25, 20, 13, 4, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 2
#define MSP_DEFAULT_NORM_CLOCK_INDEX 3

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)
/// FCI I/O base address.
#define FCI_IO_BASE_ADDR            ((UINT32)FCIF_BASE)

// Setting for MT8530M1-V1 MD0478
#define SD_MLT_FNC_CFG 3   // SD multi-function setting (Verification board default => 3)
                                            // SD_MLT_FNC_CFG : 1 ~ 5
#define MS_MLT_FNC_CFG  3   // MS multi-function setting (Verification board default => 3)
                                            // MS_MLT_FNC_CFG : 1 ~ 5

#undef FCI_INTERRUPT_ARCHITECTURE
#define FCI_INTERRUPT_ARCHITECTURE      FCI_NEW_INTERRUPT_ARCHITECTURE

#if (CONFIG_DRV_FPGA_BOARD == 1)
#define FCI_SD_CLOCK_SELECTION_STRING    27, 13, 6, 3, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [27, 13, 6, 3, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 0
#define SD_DEFAULT_NORM_CLOCK_INDEX 0
#else
#define FCI_SD_CLOCK_SELECTION_STRING    54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 2
#define SD_DEFAULT_NORM_CLOCK_INDEX 6
#endif

#if (CONFIG_DRV_FPGA_BOARD == 1)
#define FCI_MSP_CLOCK_SELECTION_STRING    27, 13, 6, 3, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [27, 13, 6, 3, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 0
#define MSP_DEFAULT_NORM_CLOCK_INDEX 1
#else
#define FCI_MSP_CLOCK_SELECTION_STRING    54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 3
#define MSP_DEFAULT_NORM_CLOCK_INDEX 8
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555)
/// FCI I/O base address.
#define FCI_IO_BASE_ADDR            ((UINT32)FCIF_BASE)

// Setting for MT8555
#define SD_MLT_FNC_CFG 1   // SD1 multi-function setting (Verification board default => 3)
                                            // SD_MLT_FNC_CFG : 1 ~ 5
#define MS_MLT_FNC_CFG  1   // MS multi-function setting (Verification board default => 4)
                                            // MS_MLT_FNC_CFG : 1 ~ 5

#undef FCI_INTERRUPT_ARCHITECTURE
#define FCI_INTERRUPT_ARCHITECTURE      FCI_NEW_INTERRUPT_ARCHITECTURE

#if (CONFIG_DRV_FPGA_BOARD == 1)
#define FCI_SD_CLOCK_SELECTION_STRING    27, 13, 6, 3, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [27, 13, 6, 3, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 0
#define SD_DEFAULT_NORM_CLOCK_INDEX 0
#else
#define FCI_SD_CLOCK_SELECTION_STRING    54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 2
#define SD_DEFAULT_NORM_CLOCK_INDEX 6
#endif

#if (CONFIG_DRV_FPGA_BOARD == 1)
#define FCI_MSP_CLOCK_SELECTION_STRING    27, 13, 6, 3, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [27, 13, 6, 3, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 0
#define MSP_DEFAULT_NORM_CLOCK_INDEX 1
#else
#define FCI_MSP_CLOCK_SELECTION_STRING    54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [54, 50, 43, 37, 36, 33, 27, 23, 18, 13, 6, 3, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 3
#define MSP_DEFAULT_NORM_CLOCK_INDEX 8
#endif

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8530)
/// FCI I/O base address.
#define FCI_IO_BASE_ADDR            ((UINT32)FCIF_BASE)

// Setting for MT8530M1-V1 MD0478
#define SD_MLT_FNC_CFG 1   // SD1 multi-function setting (Verification board default => 3)
                                            // SD_MLT_FNC_CFG : 1 ~ 5
#define MS_MLT_FNC_CFG  4   // MS multi-function setting (Verification board default => 4)
                                            // MS_MLT_FNC_CFG : 1 ~ 5

#define FCI_SD_CLOCK_SELECTION_STRING    54, 50, 43, 36, 34, 33, 27, 13, 6, 3, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [54, 50, 43, 36, 34, 33, 27, 13, 6, 3, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 2
#define SD_DEFAULT_NORM_CLOCK_INDEX 6

#define FCI_MSP_CLOCK_SELECTION_STRING    54, 50, 43, 36, 34, 33, 27, 13, 6, 3, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [54, 50, 43, 36, 34, 33, 27, 13, 6, 3, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 3
#define MSP_DEFAULT_NORM_CLOCK_INDEX 7

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)
#define FCI_IO_BASE_ADDR            ((UINT32)FLASH_CARD_BASE)

//#define SD_MLT_FNC_CFG 1  // Main board
#define SD_MLT_FNC_CFG 2  // Product Board
#define MS_MLT_FNC_CFG  1

#define FCI_SD_CLOCK_SELECTION_STRING    54, 45, 33, 27, 20, 13, 9, 6, 3, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [54, 45, 33, 27, 20, 13, 9, 6, 3, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 1
#define SD_DEFAULT_NORM_CLOCK_INDEX 3

#define FCI_MSP_CLOCK_SELECTION_STRING    60, 54, 45, 33, 27, 13, 9, 6, 1, 0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [60, 54, 45, 33, 27, 13, 9, 6, 1, 0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 3
#define MSP_DEFAULT_NORM_CLOCK_INDEX 5

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5365)
#define FCI_IO_BASE_ADDR            ((UINT32)FLASH_CARD_BASE)

#define SD_MLT_FNC_CFG 1
#define MS_MLT_FNC_CFG  1

#define FCI_SD_CLOCK_SELECTION_STRING    42, 21, 10, 5, 2, 1, 0
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [42, 21, 10, 5, 2, 1, 0]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 0
#define SD_DEFAULT_NORM_CLOCK_INDEX 1

#define FCI_MSP_CLOCK_SELECTION_STRING    0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 0
#define MSP_DEFAULT_NORM_CLOCK_INDEX 0

#elif (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5395)
#define FCI_IO_BASE_ADDR            ((UINT32)FLASH_CARD_BASE)

//#define FCI_SDMS_S_RST_INT

#define SD_MLT_FNC_CFG 1
#define MS_MLT_FNC_CFG  1

#define FCI_SD_CLOCK_SELECTION_STRING    54, 48, 27, 13, 9, 5, 2, 1
#define FCI_SD_Clock_String  UTIL_Printf("Clock Selection : [54, 48, 27, 13, 9, 5, 2, 1]\n" )
#define SD_DEFAULT_HIGH_CLOCK_INDEX 0
#define SD_DEFAULT_NORM_CLOCK_INDEX 1

#define FCI_MSP_CLOCK_SELECTION_STRING    0
#define FCI_MSP_Clock_String  UTIL_Printf("Clock Selection : [0]\n" )
#define MSP_DEFAULT_HIGH_CLOCK_INDEX 0
#define MSP_DEFAULT_NORM_CLOCK_INDEX 0
#endif

// Read / Write retry counts for SD / MS card
#define  SD_READ_RETRY  6
#define  SD_WRITE_RETRY  6

#define  MS_READ_RETRY  6
#define  MS_WRITE_RETRY  6

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef void (*FCI_CALLBACK)(INT32 u4IntStatus);

#define FCI_TYPE_NONE        ((UINT32)0)
#define FCI_TYPE_MS           ((UINT32) 1 << 2)
#define FCI_TYPE_SD           ((UINT32) 1 << 3)
#define FCI_TYPE_SMXD         ((UINT32) 1 << 4)
#define FCI_TYPE_SDIO          ((UINT32) 1 << 5)

typedef struct SDIO_DRV_IF_T SDIO_DRV_IF_T;

typedef INT32 (*sdio_init_fnct)   	(SDIO_DRV_IF_T *pt_wlan_sdio_if);
typedef INT32 (*sdio_deinit_fnct) 	(VOID);
typedef INT32 (*sdio_rb_fnct)      	(UINT16 u2Offset, UINT32 *pu4Data);
typedef INT32 (*sdio_wb_fnct)   	(UINT16 u2Offset, UINT32 u4Data);
typedef INT32 (*sdio_rp_fnct)   	(UINT8 *pucBuf, UINT16 u2Len, UINT16 u2Port);
typedef INT32 (*sdio_wp_fnct)   	(UINT8 *pucBuf, UINT16 u2Len, UINT16 u2Port);
typedef INT32 (*irq_callback_fnct)	(UINT32 u4IntStatus);

struct SDIO_DRV_IF_T
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

#if 0 // unused
EXTERN INT32 FCI_Reset(VOID);
#endif

#ifdef CC_CLI
EXTERN INT32 FCI_Diag(VOID);
#endif

/* SD Slot APIs. */
EXTERN INT32 SD_FifoFullTest(BOOL fgFifoFullTest, BOOL fgHighSpeedTestBit);
EXTERN INT32 SD_CmdTimeOutTest(BOOL fgCmdTimeOutTest);
EXTERN INT32 SD_DataTimeOutTest(BOOL fgDataTimeOutTest);
EXTERN INT32 SD_SwitchHighSpeed(BOOL bHighSpeed);
EXTERN INT32 SDC_HiSpeedModeSet(BOOL fgHiSpeedModeSet);
EXTERN INT32 SDC_SelectCard(BOOL fgSelect, UINT32 u2NewRCA);
EXTERN INT32 SD_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 SD_Write(UINT64 u4Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
EXTERN INT32 SD_Erase(UINT32 u4BlkIdx, UINT32 u4BlkNum);
EXTERN INT32 SD_DetectCardRemove(VOID);
EXTERN INT32 SD_Insert(VOID);
EXTERN INT32 SD_Remove(VOID);
EXTERN CHAR *SD_GetModel(VOID);
EXTERN INT32 SD_GetCard(VOID);
EXTERN INT32 SD_GetBusWidth(VOID);
EXTERN INT32 SD_GetBlockSize(VOID);
EXTERN INT64 SD_GetSize(VOID);
#ifdef CC_BD_FCI
//EXTERN VOID SD_SetHotswapCallback(FCI_CALLBACK pfnCallback);
#else // if defined(CC_DTV_FCI)
EXTERN INT32 SD_GetStatus(VOID);
EXTERN VOID SD_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback);
EXTERN INT32 SD_GetBlkFuncTbl(BlockFunc_T *pFuncTable);
#endif

EXTERN BOOL SD_IsPIOMode(VOID);
EXTERN BOOL SD_IsHighSpeedMode(VOID);
EXTERN BOOL SD_IsFifoTestMode(VOID);
EXTERN BOOL SD_IsCmdTimeOutTestMode(VOID);
EXTERN BOOL SD_IsDataTimeOutTestMode(VOID);
  
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
EXTERN INT32 MS_GetBusWidth(VOID);
EXTERN INT32 MS_GetBlockSize(VOID);
EXTERN INT64 MS_GetSize(VOID);
#ifdef CC_BD_FCI
//EXTERN VOID MS_SetHotswapCallback(FCI_CALLBACK pfnCallback);
#else // if defined(CC_DTV_FCI)
EXTERN INT32 MS_GetStatus(VOID);
EXTERN VOID MS_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback);
EXTERN INT32 MS_GetBlkFuncTbl(BlockFunc_T *pFuncTable);
#endif

EXTERN BOOL MS_IsPIOMode(VOID);

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
#ifndef CC_BD_FCI
EXTERN INT32 SMXD_GetBlkFuncTbl(BlockFunc_T *pFuncTable);
#endif
#endif

#endif // FCI_IF_H
