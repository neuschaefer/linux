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
 * $RCSfile: msp_const.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file msp_const.h
 *  Macro definition for MS card.
 */

#ifndef MSP_CONST_H
#define MSP_CONST_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

/* TPC Code Specifications */
#define TPC_READ_PAGE_DATA          0x02 /* 1 Page is 512 byte. */
#define TPC_READ_SHORT_DATA       0x03   /* 32, 64, 128, or 256 Byte, in TPC parameter Reg. */
#define TPC_READ_REG                       0x04
#define TPC_GET_INT                           0x07
#define TPC_SET_RW_REG_ADRS       0x08
#define TPC_EX_SET_CMD                   0x09
#define TPC_WRITE_REG                     0x0B
#define TPC_WRITE_SHORT_DATA     0x0C  /* 32, 64, 128, or 256 Byte. in TPC parameter Reg. */
#define TPC_WRITE_PAGE_DATA       0x0D /* 1 Page is 512 byte. */
#define TPC_SET_CMD                         0x0E

/*****************************************************************************
 * MS Protocol Register Map
 ****************************************************************************/
/* #define REG_RESERVED_X       0x00 */
#define REG_INT                 0x01
#define INT_CED                 0x80  /* Command EnD */
#define INT_ERR                 0x40  /* ERRor */
#define INT_BREQ              0x20    /* Buffer REQuest */
#define INT_CMDNK           0x01      /* CoMmanD NacK */

#define REG_STATUS_0     0x02
#define ST0_MB                  0x80  /* Media Busy */
#define ST0_FB0                 0x40  /* Flash Busy 0 */
#define ST0_BE                   0x20 /* Buffer Empty */
#define ST0_BF                   0x10 /* Buffer Full */
#define ST0_SL                   0x02 /* SLeep (1: Sleep, 0: Wake up) */
#define ST0_WP                  0x01  /* Write Protect (1: on, 0: off) */

#define REG_STATUS_1     0x03
#define ST1_MB                  0x80  /* Media Busy */
#define ST1_FB1                0x40   /* Flash Busy 1 */
#define ST1_DTER              0x20    /* DaTa ERror */
#define ST1_UCDT              0x10    /* Unable to Correct DaTa */
#define ST1_EXER              0x08    /* EXtra data ERror */
#define ST1_UCEX              0x04    /* Unable to Correct EXtra data */
#define ST1_FGER              0x02    /* FlaG ERror */
#define ST1_UCFG              0x01    /* Unable to Correct FlaG */
#define REG_TYPE              0x04

/* #define REG_RESERVED_X       0x05 */
#define REG_CATEGORY             0x06
#define REG_CLASS                    0x07
#define REG_SYSTEM_PARAM   0x10
#define SYSP_ON                        0x80 /* Always 1 */
#define SYSP_PAM                      0x08  /* Parallel Access Mode */
#define REG_BLOCK_ADDR_2    0x11
#define REG_BLOCK_ADDR_1    0x12
#define REG_BLOCK_ADDR_0    0x13
#define REG_CMD_PARAM          0x14
#define CMD_OVERWRITE           0x80 /* Overwrite Access Mode */
#define CMD_EXTRA_ACC           0x40 /* Extra Data Access Mode */
#define CMD_SINGLE_ACC          0x20 /* Single Page Access Mode */
#define CMD_BLOCK_ACC           0x00 /* Block Access Mode */
#define REG_PAGE_ADDR           0x15
#define REG_OVERWRITE_FLAG      0x16
#define OWFG_BLOCK_STT              0x80 /* BlocK STatus, BKST */
#define BLKSTT_OK                          0x80
#define BLKSTT_NG                          0x00
#define OWFG_PAGE_STT                0x60 /* PaGe STatus, PGST0/1 */
#define PAGESTT_OK                       0x60
#define PAGESTT_NG                       0x20
#define PAGESTT_ERR                     0x00
#define OWFG_UPDATE_STT           0x10 /* UpDate STatus, UDST */
#define UPDTSTT_ERASED              0x10
#define UPDTSTT_USED                  0x00
#define REG_MANAGEMENT_FLAG 0x17
#define MGFG_SCMS                      0x30 /* Digital Read-Protected bit, both bits must always 1 (on) */
#define MGFG_TRANS_TBL            0x08      /* Conversion table bit (1: Invalid, 0: Table block) */
#define MGFG_SYSTEM                  0x04   /* System bit (0: Boot Block, 1: Other) */
#define MGFG_SEQUENCE              0x02     /* Sequence page bit (0: Sequence page, 1: Other) */
#define REG_LOGICAL_ADDR_1   0x18
#define REG_LOGICAL_ADDR_0   0x19
#define REG_RESERVED_4            0x1A
#define REG_RESERVED_3            0x1B
#define REG_RESERVED_2            0x1C
#define REG_RESERVED_1            0x1D
#define REG_RESERVED_0            0x1E

/*****************************************************************************
 * MSPro Protocol Register Map, comment is shared with MS register
 ****************************************************************************/
/*************** Below are readonly register. ***************/
/* #define REG_RESERVED_X       0x00 */
/* #define REG_INT              0x01 */
#define REG_MSPRO_STATUS        0x02
#define ST_MSPRO_ES         0x80 /* Erase Status. (1: erased, 0: not erased) */
#define ST_MSPRO_SL         0x02 /* SLeep. (1: sleep, 0: wake up) */
#define ST_MSPRO_WP         0x01 /* Write Protect. (1: on, 0: off) */
/* #define REG_RESERVED_XX      0x03 */
/* #define REG_TYPE             0x04 */ /* 1 only. Support both serial and parallel mode */
/* #define REG_RESERVED_X       0x05 */
/* #define REG_CATEGORY         0x06 */ /* 0x00 only. Memory Category */
/* #define REG_CLASS            0x07 */ /* 0x00 only. RAM */
/*************** Below are writeonly register. ***************/
/* #define REG_SYSTEM_PARAM     0x10 */
#define SYS_MSPRO_SRAC      0x80 /* SeRialACcess. (1:Serial I/F, 0:Parallel I/F) */
#define REG_MSPRO_DATACNT1      0x11
#define REG_MSPRO_DATACNT0      0x12
#define REG_MSPRO_DATAADR3      0x13
#define REG_MSPRO_DATAADR2      0x14
#define REG_MSPRO_DATAADR1      0x15
#define REG_MSPRO_DATAADR0      0x16

/* Set the data size of a READ_SHORT_DATA/WRITE_SHORT_DATA Tpc */
#define REG_MSPRO_TPC_PAR       0x17

#define TPC032_MSPRO_DTSZ   0x00 /* 32 Bytes */
#define TPC064_MSPRO_DTSZ   0x01 /* 64 Bytes */
#define TPC128_MSPRO_DTSZ   0x02 /* 128 Bytes */
#define TPC256_MSPRO_DTSZ   0x03 /* 256 Bytes */
#define REG_MSPRO_CMD_PAR       0x18

#define EXTRA_OVERWRITE_FLAG    0x00
#define EXTRA_MANAGEMENT_FLAG   0x01
#define EXTRA_LOGIC_ADDR_1      0x02
#define EXTRA_LOGIC_ADDR_0      0x03
#define EXTRA_RESERVED_4        0x04
#define EXTRA_RESERVED_3        0x05
#define EXTRA_RESERVED_2        0x06
#define EXTRA_RESERVED_1        0x07
#define EXTRA_RESERVED_0        0x08

/* Memory Stick Command */
/* Memory Access Command */
#define CMD_BLOCK_READ          0xAA
#define CMD_BLOCK_WRITE         0x55
#define CMD_BLOCK_END           0x33
#define CMD_BLOCK_ERASE         0x99
#define CMD_FLASH_STOP          0xCC
/* Function Command */
#define CMD_SLEEP               0x5A
#define CMD_CLEAR_BUF           0xC3
#define CMD_RESET               0x3C

/* Memory Stick Pro Command */
/* Memory Access Command */
#define CMD_MSPRO_READ_DATA         0x20
#define CMD_MSPRO_WRITE_DATA        0x21
#define CMD_MSPRO_READ_ATRB         0x24
#define CMD_MSPRO_STOP              0x25
#define CMD_MSPRO_ERASE             0x26
#define CMD_MSPRO_SET_IBD           0x46
#define CMD_MSPRO_GET_IBD           0x47
/* Function Command */
#define CMD_MSPRO_FORMAT            0x10
#define CMD_MSPRO_SLEEP             0x11

#define MS_EXTRA_DATA_LEN      10
#define MS_PAGE_SIZE          512

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif // MSP_CONST_H
