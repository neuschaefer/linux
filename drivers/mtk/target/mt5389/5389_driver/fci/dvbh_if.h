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

/** @file dvbh_if.h
 *  This header file declares the DVBH SDIO host functions.
 */

#ifndef DVBH_IF_H
#define DVBH_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_typedef.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

// for IC verification and fpga emulation.

#define DVBH_UNIT_TEST
#define SDIO_INTERRUPT_CHECK_TEST

// for MPFEC loop test.
#define DVBH_MPFEC_LOOP_TEST
//#define DVBH_MPFEC_LOOP_TEST_WRITEBACK     // must enable MT5151 site #define DVBH_MPFEC_LOOP_TEST_WRITE_BACK_VERIFY

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define DVBH_IO_SIZE             ((UINT32)16384)

#define DVBH_RAM_BASE              ((UINT32)0x0)

#define DVBH_FUN1_BASE              ((UINT32)0x100)
#define DVBH_REG_SIZE              ((UINT32)0x4)

#define DVBH_CHECK                   (TRUE)
#define DVBH_NOCHECK              (FALSE)

#define DVBH_BLOCK_SIZE         ((UINT32)512)

/// DVBH wait interrupt time. Unit = milli-second.
#define DVBH_WAIT_INT_TIME             ((UINT32)1000)

// DMA register.
#define DVBH_ADDR                         (DVBH_FUN1_BASE + ((UINT32)0x0))
#define DVBH_SIZE                           (DVBH_FUN1_BASE + ((UINT32)0x4))
#define DVBH_DATA                          (DVBH_FUN1_BASE + ((UINT32)0x8))
#define DVBH_CMD                            (DVBH_FUN1_BASE + ((UINT32)0xC))

// interrupt host register.
#define DVBH_INT_HOST                   (DVBH_FUN1_BASE + ((UINT32)0x10))
#define DVBH_INT_HOST_MASK       (DVBH_FUN1_BASE + ((UINT32)0x14))

#define DVBH_INT_ERR_CRC                  ((UINT32)0x1)
#define DVBH_INT_ERR_IO                     ((UINT32)(0x1 << 1))
#define DVBH_INT_ERR_DMAR                ((UINT32)(0x1 << 2))
#define DVBH_INT_ERR_DMAW                ((UINT32)(0x1 << 3))
#define DVBH_INT_ERR_INACTIVE         ((UINT32)(0x1 << 4))
#define DVBH_INT_ERR_ALL                     \
    ((UINT32)(DVBH_INT_ERR_CRC |DVBH_INT_ERR_IO |DVBH_INT_ERR_DMAR |DVBH_INT_ERR_DMAW |DVBH_INT_ERR_INACTIVE))

// Interrupt code define by DVBH device.
// DVBH device interrupt host.
#define DVBH_INT_DEV_INIT                  ((UINT32)(0x1 << 5))
#define DVBH_INT_DEV_RESP                 ((UINT32)(0x1 << 6))
#define DVBH_INT_DEV_CMD                  ((UINT32)(0x1 << 7))
#define DVBH_INT_DEV_DRV                  ((UINT32)(0x1 << 10))

// interrupt link ARM register.
#define DVBH_INT_DEVICE                   (DVBH_FUN1_BASE + ((UINT32)0x18))
#define DVBH_INT_DEVICE_MASK       (DVBH_FUN1_BASE + ((UINT32)0x1c))

// Interrupt code define by DVBH device.
// Host interrupt DVBH device.
#define DVBH_INT_HST_INIT                  ((UINT32)(0x1 << 0))
#define DVBH_INT_HST_CMD                  ((UINT32)(0x1 << 1))
#define DVBH_INT_HST_RESP                 ((UINT32)(0x1 << 2))
#define DVBH_INT_HST_DONE                ((UINT32)(0x1 << 3))
#define DVBH_INT_HST_DRV_GET          ((UINT32)(0x1 << 4))
#define DVBH_INT_HST_DRV_SET          ((UINT32)(0x1 << 5))

#define DVBH_INT_MASK                         ((UINT32)(0xFFFF))

#define DVBH_MEM_MAP_BASE             ((UINT32)0x001F0000)

// SDIO host set command and response to SDIO device.
#define DVBH_CMD_SIZE             ((UINT32)(DVBH_MEM_MAP_BASE + 0x0000))
#define DVBH_CMD_ADDR             ((UINT32)(DVBH_MEM_MAP_BASE + 0x0004))
#define DVBH_CMD_LEN                ((UINT32)0xFC)
#define DVBH_RESP_SIZE           ((UINT32)(DVBH_MEM_MAP_BASE + 0x0100))
#define DVBH_RESP_ADDR           ((UINT32)(DVBH_MEM_MAP_BASE + 0x0104))
#define DVBH_RESP_LEN              ((UINT32)0xFC)

// SDIO host get command and response from SDIO device.
#define DVBH_DEV_CMD_ADDR          ((UINT32)(DVBH_MEM_MAP_BASE + 0x0200))
#define DVBH_DEV_CMD_SIZE           ((UINT32)(DVBH_MEM_MAP_BASE + 0x0204))
#define DVBH_DEV_RESP_ADDR        ((UINT32)(DVBH_MEM_MAP_BASE + 0x0208))
#define DVBH_DEV_RESP_SIZE          ((UINT32)(DVBH_MEM_MAP_BASE + 0x020C))

// SDIO host get driver data from SDIO device.
#define DVBH_DEV_DRV_ADDR         ((UINT32)(DVBH_MEM_MAP_BASE + 0x0210))
#define DVBH_DEV_DRV_SIZE           ((UINT32)(DVBH_MEM_MAP_BASE + 0x0214))

// SDIO host set driver data to SDIO device.
#define DVBH_HOST_DRV_SIZE           ((UINT32)(DVBH_MEM_MAP_BASE + 0x0218))
#define DVBH_HOST_DRV_ADDR         ((UINT32)(0x001D0000))

// SDIO dma buffer for device command. Length = 0x100
#define DVBH_DEV_CMD_DMA_ADDR        ((UINT32)(DVBH_MEM_MAP_BASE + 0x0220))
// SDIO dma buffer for device response. Length = 0x100
#define DVBH_DEV_RESP_DMA_ADDR        ((UINT32)(DVBH_MEM_MAP_BASE + 0x0320))

// CBus register.
#define DVBH_CBUS_DATA                  (DVBH_FUN1_BASE + ((UINT32)0x20))
#define DVBH_CBUS_ADDR                  (DVBH_FUN1_BASE + ((UINT32)0x24))
#define DVBH_CBUS_STATUS              (DVBH_FUN1_BASE + ((UINT32)0x28))
#define DVBH_CBUS_WRITE                ((UINT32)0x02)
#define DVBH_CBUS_READ                  ((UINT32)0x00)
#define DVBH_CBUS_DONE                  ((UINT32)0x01)

// DMA debug register.
#define DVBH_DMA_RCOUNT             (DVBH_FUN1_BASE + ((UINT32)0x2C))
#define DVBH_DMA_RADDR               (DVBH_FUN1_BASE + ((UINT32)0x30))
#define DVBH_DMA_WCOUNT             (DVBH_FUN1_BASE + ((UINT32)0x34))
#define DVBH_DMA_WADDR               (DVBH_FUN1_BASE + ((UINT32)0x38))

// note: MT5151 DMA DRAM address must <= 2 Mbytes
#define DVBH_DMA_MAX_ADDR    ((UINT32)0x1FFFFF)
#define DVBH_DMA_ADDR              ((UINT32)0x1F0000)
#define DVBH_DMA_SIZE              (DVBH_IO_SIZE * (sizeof(UINT32)))
#define DVBH_DMA_SET                ((UINT32)0x0)
#define DVBH_DMA_GET                ((UINT32)0x1)
#define DVBH_DMA_READY           ((UINT32)0x2)

// MT5151 SRAM register.
#define DVBH_SDRAM_BASE           ((UINT32)0x02000000)
#define DVBH_SDRAM_INIT           (DVBH_SDRAM_BASE + ((UINT32)0x0))
#define DVBH_SDRAM_PARM           (DVBH_SDRAM_BASE + ((UINT32)0x4))
#define DVBH_SDRAM_REFRESH        (DVBH_SDRAM_BASE + ((UINT32)0x8))
#define DVBH_SDRAM_DLADJ          (DVBH_SDRAM_BASE + ((UINT32)0xC))

#define DEFAULT_INIT              ((UINT32)0x01)
#define DEFAULT_PARM              ((UINT32)0x00002107)
#define DEFAULT_REFRESH           ((UINT32)0x00010190)
#define DEFAULT_DLADJ             ((UINT32)0x00000030)

// MT5151 BIM register.
#define DVBH_BIM_BASE                       ((UINT32)0x04000000)
#define DVBH_BIM_CACHE_CONFIG     (DVBH_BIM_BASE + ((UINT32)0x0))
#define DVBH_BIM_CPU_ENABLE         (DVBH_BIM_BASE + ((UINT32)0x4))
#define DEFAULT_CPUEN                      ((UINT32)0x01)

// MT5151 SDIO register.
#define DVBH_SDIO_BASE                       ((UINT32)0x1E000000)
// Interrupt link ARM register.
#define DVBH_SDIO_INT_DEVICE                   (DVBH_SDIO_BASE + ((UINT32)0x18))
#define DVBH_SDIO_INT_DEVICE_MASK         (DVBH_SDIO_BASE + ((UINT32)0x1C))
//#define DVBH_SDIO_INT_DEVICE                   (DVBH_SDIO_BASE + ((UINT32)0x10))
//#define DVBH_SDIO_INT_DEVICE_MASK       (DVBH_SDIO_BASE + ((UINT32)0x14))
// Interrupt host register.
#define DVBH_SDIO_INT_HOST                      (DVBH_SDIO_BASE + ((UINT32)0x10))
#define DVBH_SDIO_INT_HOST_MASK            (DVBH_SDIO_BASE + ((UINT32)0x14))
//#define DVBH_SDIO_INT_HOST                      (DVBH_SDIO_BASE + ((UINT32)0x18))
//#define DVBH_SDIO_INT_HOST_MASK          (DVBH_SDIO_BASE + ((UINT32)0x1C))

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef VOID (*DVBH_CALLBACK)(UINT32 u4Status, VOID *pvTag);

typedef struct
{
    VOID *pvTag;
    DVBH_CALLBACK pfnNfy; /* NULL - sync, otherwise - async. */
} DVBH_NOTIFY_T;

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
#ifdef DVBH_UNIT_TEST
EXTERN INT32 DVBH_SetCmdTest(BOOL fgEnable);
EXTERN INT32 DVBH_DataTest(VOID);
EXTERN INT32 DVBH_InterruptTest(VOID);
#endif
//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
EXTERN void DVBH_MwSetNotify(DVBH_CALLBACK pfnNfy, void *pvTag);
EXTERN void DVBH_DrvSetNotify(DVBH_CALLBACK pfnNfy, void *pvTag);

EXTERN INT32 DVBH_MwGetCmd(UINT32 *pu4Cmd, UINT32 *pu4CmdSize);
EXTERN INT32 DVBH_MwSetResp(UINT32 *pu4Resp, UINT32 u4RespSize);
EXTERN INT32 DVBH_MwGetResp(UINT32 *pu4Resp, UINT32 *pu4RespSize);
EXTERN INT32 DVBH_MwSetCmd(UINT32 *pu4Cmd, UINT32 u4CmdSize);
EXTERN INT32 DVBH_DrvGet(UINT32 *pu4Buf, UINT32 *pu4Size);
EXTERN INT32 DVBH_DrvSet(UINT32 *pu4Buf, UINT32 u4Size);

EXTERN INT32 DVBH_Init(VOID);

#endif // DVBH_IF_H
