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

/** @file fcihw_reg.h
 *  FCI module macro and constant declaration.
 */

#ifndef FCIHW_REG_H
#define FCIHW_REG_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

#include "fci_if.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"

LINT_EXT_HEADER_END

#if (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555) || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550) || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5363)  || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5365)  || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT5395)
#define FCIHW_INTERRUPT_WRITE_CLEAR
#endif

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define RW_MS_CTRL_REG          0x00
#define MS_DATASIZE             0x03ff
#define FCI_MS_DATA(x)          ((UINT32)x & 0x03ff)
#define MS_PID                  0xf000
#define FCI_MS_PID(x)           (((UINT32)x & 0x0f) << 12)

#define RW_SD_CTRL_REG          0x04
#define SD_DATASIZE             0x00000fff
#define SD_MULTI_BLKNO          0x0007f000
#define SD_MULTI_FLAG           0x00080000
#define SD_CMD_TYPE             0x00700000
#define SD_STOP_CMD             0x00800000
#define SD_CMD                  0x3f000000
#define SD_WRITE                0x40000000
#define SD_READ                 0x80000000

#define RW_FCI_CTRL_REG         0x08
#define CTRL_DRAM_CLKEN     ((UINT32)1 << 0)
#define CTRL_DRAM_CLKRST    ((UINT32)1 << 1)
#define CTRL_SD_CCO            ((UINT32)1 << 2)
#define CTRL_MS_CCO            ((UINT32)1 << 3)
#define CTRL_FCI_FIFORST    ((UINT32)1 << 4)
#define CTRL_FCI_INTRST     ((UINT32)1 << 5)
#define CTRL_FCI_FIFO_W_DIR ((UINT32)1 << 6)
#define CTRL_FCI_DMAEN      ((UINT32)1 << 7)
#if(CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550)  // mhzhang
#define CTRL_FCI_SRST         ((UINT32)1<<8)
#endif
#define FCI_CTRL_DATA_MASK  ((UINT32) 0xff << 0)
#define CTRL_SD_HS_ADJ        ((UINT32)1 << 12)  //  this bit is not exit in 8550, but exit in 8555
#define CTRL_SM_MLBLCNEN    ((UINT32)1 << 11)
#define CTRL_SM_CCB(x)        ((UINT32)(7 & (x)) << 8)
#define CTRL_SM_DATASWAP    ((UINT32)1 << 12)
#define CTRL_SM_SIEN        ((UINT32)1 << 13)
#define CTRL_SM_PWS         ((UINT32)1 << 14)
#define CTRL_SM_RST         ((UINT32)1 << 15)
#define FCI_CTRL_SM_MASK    ((UINT32) 0xff << 8)
#define CTRL_MS_BSYCNT      ((UINT32)7 << 16)
#define FCI_MS_BSYCNT(x)    ((UINT32)(7 & (x)) << 16)
#define CTRL_MS_WIDEBUSEN   ((UINT32)1 << 19)
#define CTRL_MS_NOCRC       ((UINT32)1 << 20)
#define CTRL_MS_SIEN        ((UINT32)1 << 21)
#define CTRL_MS_PWS         ((UINT32)1 << 22)
#define CTRL_MS_RST         ((UINT32)1 << 23)
#define FCI_CTRL_MS_MASK    ((UINT32) 0xff << 16)
#define CTRL_SD_WIDEBUSEN   ((UINT32)1 << 24)
#define CTRL_SD_RECEDGE        ((UINT32)1 << 25)
#define CTRL_SD_MLBLCNEN    ((UINT32)1 << 26)
#define CTRL_SD_NEGDRIVE    ((UINT32)1 << 27)
#define CTRL_SD_DATASWAP    ((UINT32)1 << 28)
#define CTRL_SD_SIEN        ((UINT32)1 << 29)
#define CTRL_SD_PWS         ((UINT32)1 << 30)
#define CTRL_SD_RST         ((UINT32)1 << 31)
#define FCI_CTRL_SD_MASK    ((UINT32) 0xff << 24)

#define RW_FCI_INTR_CTRL_REG        0x0C
#define INTR_MS_TOEI        ((UINT32)1 << 0) /* Tpc timeout. */
#define INTR_MS_CRCEI       ((UINT32)1 << 1) /* Tpc CRC error. */
#define INTR_MS_SITI        ((UINT32)1 << 2) /* Tpc result if there is bit 27~24 of 0x10 is available. */
#define INTR_MS_RDYI        ((UINT32)1 << 3) /* Tpc result if tpc is finished. */

#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)    // Error bit in MT8550/8555 is not interrupt source
#define INTR_MS_ALLI        (INTR_MS_SITI | INTR_MS_RDYI)
#else
#define INTR_MS_ALLI        (INTR_MS_TOEI | INTR_MS_CRCEI | INTR_MS_SITI | INTR_MS_RDYI)
#endif

#define INTR_DRAM_DRFI      ((UINT32)1 << 5)
#define INTR_FCI_CDI         ((UINT32)1 << 6)
#define INTR_FCI_HIGH_SPEED_MODE         ((UINT32)1 << 6) // for DTV
// Max Liao, remove DRFI to prevent the interrupt source of
// "write card finish" and "DRAM read finish" getting confused.
//#define INTR_FCI_ALLI       (INTR_FCI_CDI)
#define INTR_FCI_ALLI       (0)

#define INTR_SD_RDTOI       ((UINT32)1 << 7)
#define INTR_SD_WDTOI       ((UINT32)1 << 8)
#define INTR_SD_CMDTOI      ((UINT32)1 << 9)
#define INTR_SD_RDCRCI      ((UINT32)1 << 10)
#define INTR_SD_WRCRCI      ((UINT32)1 << 11)
#define INTR_SD_FPEI        ((UINT32)1 << 12)
#define INTR_SD_CMDCRCI     ((UINT32)1 << 13)
#define INTR_SD_DTRDYI      ((UINT32)1 << 14)
#define INTR_SD_CMDRDYI     ((UINT32)1 << 15)
#define INTR_SD_DMD_MODE    ((UINT32)1 << 16)
#define INTR_SD_DMT_MODE    ((UINT32)1 << 17)
#define INTR_SD_DMR_MODE    ((UINT32)1 << 18)
#define INTR_SD_RMS_MODE    ((UINT32)1 << 19)

#define INTR_SRAMSELI       ((UINT32)3 << 20)
#define FCI_32B_SRAM        3
#define FCI_64B_SRAM        2
#define FCI_128B_SRAM       1
#define FCI_256B_SRAM       0
#define FCI_SRAMSEL(x)      ((((UINT32)x) << 20) & INTR_SRAMSELI)

#define INTR_SDMS_SRST_INT        ((UINT32)1 << 22)
#define INTR_SM_TOEI        ((UINT32)1 << 22)
#define INTR_SM_RDYI        ((UINT32)1 << 23)
#define INTR_SM_ALLI        (INTR_SM_TOEI | INTR_SM_RDYI)

#define INTR_DTPOP_MODE     ((UINT32)1 << 24)

#define INTR_SDDI_DIRECT_IN     ((UINT32)1 << 26)
#define INTR_SDIO_INT       ((UINT32)1 << 27)
#define INTR_SD_PFI         ((UINT32)1 << 28)
#define INTR_SD_RBI         ((UINT32)1 << 29)

// do not include SDIO INT.
#if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
#ifdef FCI_SDMS_S_RST_INT
#define INTR_SD_ALLI        (INTR_SD_DTRDYI | INTR_SD_CMDRDYI|INTR_SDMS_SRST_INT)
#else
#define INTR_SD_ALLI        (INTR_SD_DTRDYI | INTR_SD_CMDRDYI)
#endif
#define INTR_SD_ERRI        (INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI |        \
                                INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_FPEI |INTR_SD_CMDCRCI)
#else // #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)
#ifdef FCI_SDMS_S_RST_INT
#define INTR_SD_ALLI        (INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI |        \
                                INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_FPEI |     \
                                INTR_SD_CMDCRCI | INTR_SD_DTRDYI | INTR_SD_CMDRDYI | \
                                INTR_SD_PFI | INTR_SD_RBI | INTR_SDMS_SRST_INT)
#else
#define INTR_SD_ALLI        (INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI |        \
                                INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_FPEI |     \
                                INTR_SD_CMDCRCI | INTR_SD_DTRDYI | INTR_SD_CMDRDYI | \
                                INTR_SD_PFI | INTR_SD_RBI)
#endif
#define INTR_SD_ERRI        (INTR_SD_RDTOI | INTR_SD_WDTOI | INTR_SD_CMDTOI |        \
                                INTR_SD_RDCRCI | INTR_SD_WRCRCI | INTR_SD_FPEI |INTR_SD_CMDCRCI)
#endif  // #if (FCI_INTERRUPT_ARCHITECTURE == FCI_NEW_INTERRUPT_ARCHITECTURE)

#define INTR_FCISELI        ((UINT32)3 << 30)
#define FCI_SEL(x)          ((((UINT32)x) << 30) & INTR_FCISELI)
#define FCI_SEL_SD          FCI_SEL(0)
#define FCI_SEL_MS          FCI_SEL(1)
#define FCI_SEL_SM          FCI_SEL(2)

#define FCI_TYPES             (3)
#define FCI_GET_TYPE(x)         (((x)>>30) & FCI_TYPES)

#define RW_FCI_INTR_STAT_REG        0x10

#ifdef FCIHW_INTERRUPT_WRITE_CLEAR
#define INTR_FCI_SDCD    ((UINT32)1 << 4)
#define INTR_FCI_SDRB    ((UINT32)1 << 3)
#endif

#define INTR_FCI_WRFIFO_FULL    ((UINT32)1 << 16)
#define INTR_FCI_WRFIFO_EMPTY   ((UINT32)1 << 17)
#define INTR_FCI_RDFIFO_FULL    ((UINT32)1 << 18)
#define INTR_FCI_RDFIFO_EMPTY   ((UINT32)1 << 19)
#define INTR_FCI_DMAREQ         ((UINT32)1 << 20)
#define INTR_SM_NONBUSY         ((UINT32)1 << 21)
#define INTR_SM_TOERR             ((UINT32)1 << 22)
#define INTR_SM_TRRDY         ((UINT32)1 << 23)
#define INTR_SD_CMD_IDX         ((UINT32)0x7 << 24)
#define INTR_SDIO_INT            ((UINT32)1 << 27)
#define INTR_MS_CED             ((UINT32)1 << 24)
#define INTR_MS_ERR             ((UINT32)1 << 25)
#define INTR_MS_BREQ            ((UINT32)1 << 26)
#define INTR_MS_CMDNK           ((UINT32)1 << 27)
#define INTR_SD_NWRITEBUSY      ((UINT32)1 << 28)
#define INTR_SD_RESPBUSY        ((UINT32)1 << 29)
#define INTR_SD_DATAIDLE        ((UINT32)1 << 30)
#define INTR_MMC_IRQSTATE       ((UINT32)1 << 31)

#define INTR_FIFO_MASK    (INTR_FCI_WRFIFO_FULL |INTR_FCI_WRFIFO_EMPTY \
                                                |INTR_FCI_RDFIFO_FULL |INTR_FCI_RDFIFO_EMPTY)


#define RW_SD_BUSYCNT_REG       0x14
#define MAX_NAC_BSYCNT          ((UINT32) 0x0fffff << 0)
#define MAX_NWP_BSYCNT          ((UINT32) 0x0f << 20)
#define MAX_NCR_BSYCNT          ((UINT32) 0x3f << 24)

#define CTRL_FCI_SDWC             ((UINT32) 0x01 << 31)
#define RW_SD_ARG_REG           0x18
#define RW_MS_AUTO_CTRL_REG     0x1c
#define CTRL_MS_ND              ((UINT32)1 << 0)
#define CTRL_MS_D5              ((UINT32)1 << 1)
#define CTRL_SIF_TOE            ((UINT32)1 << 2)
#define CTRL_MS_CLK_PS       ((UINT32)0x7 << 3) // mhzhang

// ??
//Shiuan, JY suggests remove this bit at all settings. 
#define CTRL_MS_RED             ((UINT32)1 << 14)
#define CTRL_MS_ACE             ((UINT32)1 << 15)
#define MAX_MS_ADS              ((UINT32)0x03ff << 16)
#define MS_ADS(x)               ((((UINT32)x) & 0x03ff) << 16)
#define MAX_MS_APID             ((UINT32)0x0f << 28)
#define MS_APID(x)              ((((UINT32)x) & 0x0f) << 28)

#define RO_SD_RESP1_REG         0x20
#define WO_FCI_DATA_REG         0x20
#define RO_SD_RESP2_REG         0x24
#define RO_SD_RESP3_REG         0x28
#define RO_SD_RESP4_REG         0x2c
#define RW_DMA_UP_BOUNDARY_REG  0x30
#define RW_DMA_LOW_BOUNDARY_REG 0x34
#define RW_DMA_START_ADDR_REG   0x38
#define RW_DMA_DS_REG           0x3c
#define RW_RD_DATA_REG          0x40
#define RW_WR_POINTER_REG       0x44
#define RW_RD_POINTER_REG       0x48
#define RW_SM_CTRL_REG          0x4C
// Operating Command Table
#define SM_CMD_SERIAL_DATA_INPUT      ((UINT32)(0x80) << 24)
#define SM_CMD_READ                   0x00080000
#define SM_CMD_READ_O                 ((UINT32)(0x00) << 24)
#define SM_CMD_READ_E                 ((UINT32)(0x01) << 24)
#define SM_CMD_READ_R                 0x50080000
#define SM_CMD_RESET                  0xFF100000
#define SM_CMD_PAGE_PROGRAM           ((UINT32)(0x10) << 24)
#define SM_CMD_DUMMY_PAGE_PROGRAM           ((UINT32)(0x11) << 24)
#define SM_CMD_MULTI_BLOCK_PROGRAM           ((UINT32)(0x15) << 24)
#define SM_CMD_BLOCK_ERASE_1          ((UINT32)(0x60) << 24)
#define SM_CMD_BLOCK_ERASE_2          ((UINT32)(0xD0) << 24)
#define SM_CMD_STATUS_READ            0x700C0000
#define SM_CMD_STATUS_MULBLK_READ            0x710C0000
#define SM_ADDR_CYCLE(x)              ((UINT32)(x) << 21)
#define SM_CMD_AC_SERIAL_DATA_INPUT   ((UINT32)(0x80) << 21)
#define SM_CMD_AC_READ                ((UINT32)(0x00) << 21)
#define SM_CMD_AC_READ_2              ((UINT32)(0x01) << 21)
#define SM_CMD_AC_READ_3              ((UINT32)(0x50) << 21)
#define SM_CMD_AC_RESET               ((UINT32)(0xFF) << 21)
#define SM_CMD_AC_PAGE_PROGRAM        ((UINT32)(0x10) << 21)
#define SM_CMD_AC_BLOCK_REASE_1       ((UINT32)(0x60) << 21)
#define SM_CMD_AC_BLOCK_REASE_2       ((UINT32)(0xD0) << 21)
#define SM_CMD_AC_STATUS_READ         ((UINT32)(0x70) << 21)
#define SM_CMD_AC_ID_READ_90          ((UINT32)(0x90) << 21)
#define SM_CMD_AC_ID_READ_91          ((UINT32)(0x91) << 21)

#define SM_CMD_ID_READ_90             0x900C0000
#define SM_CMD_ID_READ_91             0x910C0000
#define SM_CMD_ID_READ_9A             0x9A0C0000

#define SM_WD          ((UINT32)1 << 20)
#define SM_DE          ((UINT32)1 << 19)
#define SM_NB          ((UINT32)1 << 18)
#define SM_WPE          ((UINT32)1 << 17)
#define SM_WPD        ((UINT32)1 << 16)
// SM multiple block transfer counter.
#define SM_MBC_2        ((UINT32)1 << 13)
#define SM_MBC_4        ((UINT32)2 << 13)
#define SM_MBC_6        ((UINT32)3 << 13)
#define SM_MBC_8        ((UINT32)4 << 13)
#define SM_MBC_10        ((UINT32)5 << 13)
#define SM_MBC_12        ((UINT32)6 << 13)
#define SM_MBC_14        ((UINT32)7 << 13)

// Status Output Table
#define SM_STATUS_PASS                0x00
#define SM_STATUS_FAIL                0x01
#define SM_STATUS_BUSY                (0x00 << 6)
#define SM_STATUS_READY               (0x01 << 6)
#define SM_STATUS_WRITE_PROTECT       (0x00 << 7)
#define SM_STATUS_NOT_WRITE_PROTECT   (0x01 << 7)
#define SM_STATUS_MASK                0xC1
#define SM_MULTI_BLK_STATUS_MASK                0xDF

#define RW_WINDOW_REG           0x50                            // 0xB050 not exsit in MT8550
#define SET_MS_WINDOW(secs)  ((secs) & 0x0F)            
#define SET_SD_WINDOW(secs)  (((secs) & 0x0F) << 4) 




// // Cmd 90 - ID Read Code
#define XD_WITH_EXT_BLK         0xA5
#define XD_WITH_91_ID_CMD       0xC0

// Cmd 90 BYTE4
#define XD_ID_WITH_91_CODE              0xC0

// Cmd 91 X4 multiple block flag.
#define XD_ID_X4MULBLK_CODE              0x20

// Cmd 9A
#define XD_ID_CODE              0xB5

// Byte operation.
#define BYTE1(x)          ((UINT32)x & 0xff)
#define BYTE2(x)          ((((UINT32)x)>>8) & 0xff)
#define BYTE3(x)          ((((UINT32)x)>>16) & 0xff)
#define BYTE4(x)          ((((UINT32)x)>>24) & 0xff)

#define RO_FSM_REG              0x54
#define SD_DATA_FSM             ((UINT32)0xf << 0)
#define SD_CMD_FSM              ((UINT32)0xf << 4)
//mhzhang 
#if((CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550) || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555) )
#define SD_CMD_DAT_IDLE        ((UINT32)1 << 8)
#define SD_CMD_ID                    ((UINT32)1 << 9)
#define MS_STATE                      ((UINT32)0xf << 16)
#define MS_IDLE                         ((UINT32)1 << 20)
#endif


//mhzhang
#if((CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8550) || (CONFIG_CHIP_VER_CURR == CONFIG_CHIP_VER_MT8555) )
// (0xB058)
#define FCI_DUMMY_REG           0x58
#define FCI_DUMMY_80             ((UINT32)0xff << 0)
#define FCI_INT_SEL                 ((UINT32)1 << 9)
#define FCI_DUMMY_h16           ((UINT32)0xfffff<<16)
#endif
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif // FCIHW_REG_H
