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

/** @file sdc_const.h
 *  Macro definition for SD card.
 */

#ifndef SDC_CONST_H
#define SDC_CONST_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

// clock control.
#define SDC_HIGH_CLOCK  2
#define SDC_NORM_CLOCK  1
#define SDC_LOW_CLOCK   0

// SD command and argument.
#define SD_CMD_NO_ARG       0
#define SD_GENERAL_CMD(cmd, type)   (((cmd) << 24) | ((type) << 20))
#define SD_GET_TYPE(cmd)   (UINT32)(((cmd) >> 20) & 0x07)
#define SD_CMD_READ_BIT     0x80000000
#define SD_CMD_WRITE_BIT    0x40000000
#define SD_CMD_STOP_BIT     0x00800000
#define SD_CMD_RW_MULTI_BIT 0x00080000
#define SD_CMD_IDX(x)       (((x) >> 24) & 0x7)
#define SD_RESP_IDX(x)       (((x) >> 20) & 0x7)
#define TYPE_NORES          0
#define TYPE_R1             1
#define TYPE_R6             1
#define TYPE_R1b            2
#define TYPE_R2             3
#define TYPE_R3             4
#define TYPE_SDIO_R4        TYPE_R3                                                            /* SDIO without CRC */
#define TYPE_MMC_R4         TYPE_R1                                                            /* MMC with CRC */
#define TYPE_R5             5
#define TYPE_R7                   TYPE_R6

#define SD_CMD_0        SD_GENERAL_CMD(0, TYPE_NORES)                                          // -
#define SD_CMD_1        SD_GENERAL_CMD(1, TYPE_R3)                                             // R3
#define SD_CMD_2        SD_GENERAL_CMD(2, TYPE_R2)                                             // R2
#define SD_CMD_3_SD     SD_GENERAL_CMD(3, TYPE_R6)                                             // R6
#define SD_CMD_3_MMC    SD_GENERAL_CMD(3, TYPE_R1)                                             // R1
#define SD_CMD_4        SD_GENERAL_CMD(4, TYPE_NORES)                                          // -
#define SD_CMD_5        SD_GENERAL_CMD(5, TYPE_SDIO_R4)                                        // R4
#define SD_CMD_6        (SD_GENERAL_CMD(6, TYPE_R1) | SD_CMD_READ_BIT)                // R1
#define SD_CMD_6_MMC     SD_GENERAL_CMD(6, TYPE_R1b)                                        // R1b
#define SD_CMD_7        SD_GENERAL_CMD(7, TYPE_R1b)                                            // R1b
#define SD_CMD_8        SD_GENERAL_CMD(8, TYPE_R7)                                             // R7
#define SD_CMD_8_MMC     (SD_GENERAL_CMD(8, TYPE_R1) | SD_CMD_READ_BIT)         // R1
#define SD_CMD_9        SD_GENERAL_CMD(9, TYPE_R2)                                             // R2
#define SD_CMD_10       SD_GENERAL_CMD(10, TYPE_R2)                                            // R2
#define SD_CMD_11_MMC   (SD_GENERAL_CMD(11, TYPE_R1) | SD_CMD_READ_BIT)                        // R1
#define SD_CMD_12       (SD_GENERAL_CMD(12, TYPE_R1b) | SD_CMD_STOP_BIT)                       // R1b
#define SD_CMD_13       SD_GENERAL_CMD(13, TYPE_R1)                                            // R1
#define SD_CMD_15       SD_GENERAL_CMD(15, TYPE_NORES)                                         // -
#define SD_CMD_16       SD_GENERAL_CMD(16, TYPE_R1)                                            // R1
#define SD_CMD_17       (SD_GENERAL_CMD(17, TYPE_R1) | SD_CMD_READ_BIT)                        // R1
#define SD_CMD_18       (SD_GENERAL_CMD(18, TYPE_R1) | SD_CMD_READ_BIT |SD_CMD_RW_MULTI_BIT)  // R1
#define SD_CMD_20       (SD_GENERAL_CMD(20, TYPE_R1) | SD_CMD_WRITE_BIT)                       // R1
#define SD_CMD_24       (SD_GENERAL_CMD(24, TYPE_R1) | SD_CMD_WRITE_BIT)                       // R1
#define SD_CMD_25       (SD_GENERAL_CMD(25, TYPE_R1) | SD_CMD_WRITE_BIT |SD_CMD_RW_MULTI_BIT) // R1
#define SD_CMD_26       (SD_GENERAL_CMD(26, TYPE_R1) | SD_CMD_WRITE_BIT)                       // R1
#define SD_CMD_27       (SD_GENERAL_CMD(27, TYPE_R1) | SD_CMD_WRITE_BIT)                       // R1
#define SD_CMD_28       SD_GENERAL_CMD(28, TYPE_R1b)                                           // R1b
#define SD_CMD_29       SD_GENERAL_CMD(29, TYPE_R1b)                                           // R1b
#define SD_CMD_30       (SD_GENERAL_CMD(30, TYPE_R1) | SD_CMD_READ_BIT)                        // R1
#define SD_CMD_32       SD_GENERAL_CMD(32, TYPE_R1)                                            // R1
#define SD_CMD_33       SD_GENERAL_CMD(33, TYPE_R1)                                            // R1
#define SD_CMD_34_MMC   SD_GENERAL_CMD(34, TYPE_R1)                                            // R1
#define SD_CMD_35_MMC   SD_GENERAL_CMD(35, TYPE_R1)                                            // R1
#define SD_CMD_36_MMC   SD_GENERAL_CMD(36, TYPE_R1)                                            // R1
#define SD_CMD_37_MMC   SD_GENERAL_CMD(37, TYPE_R1)                                            // R1
#define SD_CMD_38       SD_GENERAL_CMD(38, TYPE_R1b)                                           // R1b
#define SD_CMD_39_MMC   SD_GENERAL_CMD(39, TYPE_R4)                                            // R4 XXX
#define SD_CMD_40_MMC   SD_GENERAL_CMD(40, TYPE_R5)                                            // R5
#define SD_CMD_41_SD    SD_GENERAL_CMD(41, TYPE_R3)                                            // ACMD R3
#define SD_CMD_42       (SD_GENERAL_CMD(42, TYPE_R1b) | SD_CMD_WRITE_BIT)                      // R1
#define SD_CMD_52       SD_GENERAL_CMD(52, TYPE_R5)                                            // R5
#define SD_CMD_53_R     (SD_GENERAL_CMD(53, TYPE_R5) | SD_CMD_READ_BIT)                        // R5
#define SD_CMD_53_MULT_R     (SD_GENERAL_CMD(53, TYPE_R5) | SD_CMD_READ_BIT |SD_CMD_RW_MULTI_BIT)                        // R5
#define SD_CMD_53_W     (SD_GENERAL_CMD(53, TYPE_R5) | SD_CMD_WRITE_BIT)                       // R5
#define SD_CMD_53_MULT_W     (SD_GENERAL_CMD(53, TYPE_R5) | SD_CMD_WRITE_BIT |SD_CMD_RW_MULTI_BIT)                       // R5

#define SD_CMD_55       SD_GENERAL_CMD(55, TYPE_R1)                                            // R1
#define SD_CMD_56_MMC_R (SD_GENERAL_CMD(56, TYPE_R1b) | SD_CMD_READ_BIT)                       // R1b argument must be 1
#define SD_CMD_56_MMC_W (SD_GENERAL_CMD(56, TYPE_R1b) | SD_CMD_WRITE_BIT)                      // R1b argument must be 0
#define SD_CMD_56_SD_R  (SD_GENERAL_CMD(56, TYPE_R1) | SD_CMD_READ_BIT)                        // R1 argument must be 1
#define SD_CMD_56_SD_W  (SD_GENERAL_CMD(56, TYPE_R1) | SD_CMD_WRITE_BIT)                       // R1 argument must be 0

#define SD_ACMD_6       SD_GENERAL_CMD(6, TYPE_R1)                                             // R1
#define SD_ACMD_13      (SD_GENERAL_CMD(13, TYPE_R1) | SD_CMD_READ_BIT)                        // R1
#define SD_ACMD_22      (SD_GENERAL_CMD(22, TYPE_R1) | SD_CMD_READ_BIT)                        // R1
#define SD_ACMD_23      SD_GENERAL_CMD(23, TYPE_R1)                                            // R1
#define SD_ACMD_41      SD_GENERAL_CMD(41, TYPE_R3)                                            // R3
#define SD_ACMD_42      SD_GENERAL_CMD(42, TYPE_R1)                                            // R1
#define SD_ACMD_51      (SD_GENERAL_CMD(51, TYPE_R1) | SD_CMD_READ_BIT)                        // R1

#define MMC_CMD_1       SD_CMD_1
#define MMC_CMD_3       SD_CMD_3_MMC
#define MMC_CMD_7       SD_CMD_7//0x47000000
#define MMC_CMD_9       SD_CMD_9//0x22400000
#define MMC_CMD_10      SD_CMD_10//0x28400000
#define MMC_CMD_16      SD_CMD_16//0x10080000

#define MMC_DEF_RCA     0x0001

#define SD_DEF_DSR      0x0404

#define SDC_OCR_SD_DEFAULT     0x00FC0000   // for SD 1.X
#define SDC_OCR_HD_DEFAULT     0x40FC0000   // for SD 2.0

#define SDC_HVS_DEFAULT     0x000001AA

#define SDC_CMD6_DEFAULT_SPEED    ((UINT32)0x00FFFF00)
#define SDC_CMD6_HIGH_SPEED       ((UINT32)0x00FFFF01)
#define SDC_CMD6_CHECK_FUNC 		((UINT32)0x00 << 31)
#define SDC_CMD6_SWITCH_FUNC 	   ((UINT32)0x01 << 31)

// MMC CMD6 Argument :
// (1) Bus Width Selection :
// Access bits = 0x03 (Write byte), Index = 0xB7 = 183, Value = 0(1bit), 1(4bits), 2(8bits)
#define MMC_CMD6_ARG_1BIT_BUS  0x03B70000
#define MMC_CMD6_ARG_4BIT_BUS 0x03B70100
#define MMC_CMD6_ARG_8BIT_BUS 0x03B70200

// (2) High SPeed Mode Selection :
// Access bits = 0x03 (Write byte), Index = 0xB9 = 185, Value = 0(26 Mhz max), 1(52 Mhz max)
#define MMC_CMD6_ARG_NORM_SPEED  0x03B90000
#define MMC_CMD6_ARG_HIGH_SPEED   0x03B90100

// #define SDC_OCR_DEFAULT      0x00020000
#define TRY_OCR             10
#define SDC_OCR_BUSY        0x80000000
#define SDC_CCS                  0x40000000
#define SDC_RCA_DEFAULT     0x0000
#define ARG_RCA_MASK        0xffff0000
#define SDC_RCA_MMC         1
//#define SDC_DSR_DEFAULT       0x0404
#define SDC_NO_ARG          0
#define SDC_MAX_BKLENGTH    2048
#define SDC_MAX_LOCKBK      34

//masks for command error response(32) of SDC_CSTA
#define SDC_OUT_OF_RANGE        0x80000000
#define SDC_ADDRESS_ERROR       0x40000000
#define SDC_BLOCK_LEN_ERROR     0X20000000
#define SDC_ERASE_SEQ_ERROR     0X10000000
#define SDC_ERASE_PARAM         0X08000000
#define SDC_WP_VIOLATION        0X04000000
#define SDC_CARD_IS_LOCKED      0x02000000
#define SDC_LOCK_UNLOCK_FAILED  0X01000000
#define SDC_COM_CRC_ERROR       0X00800000
#define SDC_ILLEGAL_COMMAND     0X00400000
#define SDC_CARD_ECC_FAILED     0X00200000
#define SDC_CC_ERROR            0X00100000
#define SDC_ERROR               0X00080000
#define SDC_UNDERRUN            0X00040000
#define SDC_OVERRUN             0X00020000
#define SDC_CIDCSD_OVERWRITE    0X00010000
#define SDC_AKE_SEQ_ERROR       0X00000008

#define SDC_CSTA_MASK 0xffff0000

//masks for card status contained in R1
#define R1_AKE_SEQ_ERROR_3          0x00000004
#define R1_APP_CMD_5                0x00000020
#define R1_READY_FOR_DATA_8         0x00000100
#define R1_CURRENT_STATE_9_12       0x00001e00
#define R1_ERASE_RESET_13           0x00002000
#define R1_CARD_ECC_DISABLE_14      0x00004000
#define R1_WP_ERASE_SKIP_15         0x00008000
#define R1_CID_CSD_OVERWRITE_16     0x00010000
#define R1_OVERRUN_17               0x00020000
#define R1_UNDERRUN_18              0x00040000
#define R1_ERROR_19                 0x00080000
#define R1_CC_ERROR_20              0x00100000
#define R1_CARD_ECC_FAILED_21       0x00200000
#define R1_ILLEGAL_COMMAND_22       0x00400000
#define R1_COM_CRC_ERROR_23         0x00800000
#define R1_LOCK_UNLOCK_FAILED_24    0x01000000
#define R1_CARD_IS_LOCKED_25        0x02000000
#define R1_WP_VIOLATION_26          0x04000000
#define R1_ERASE_PARAM_27           0x08000000
#define R1_ERASE_SEQ_ERROR_28       0x10000000
#define R1_BLOCK_LEN_ERROR_29       0x20000000
#define R1_ADDRESS_ERROR_30         0x40000000
#define R1_OUT_OF_RANGE_31          0x80000000

#define SD_RW_ERRORS    (R1_OUT_OF_RANGE_31|R1_ADDRESS_ERROR_30|R1_ERASE_PARAM_27|R1_WP_VIOLATION_26|R1_CARD_IS_LOCKED_25|R1_LOCK_UNLOCK_FAILED_24 \
                        |R1_COM_CRC_ERROR_23|R1_ILLEGAL_COMMAND_22|R1_CARD_ECC_FAILED_21|R1_CC_ERROR_20|R1_ERROR_19                                \
                        |R1_UNDERRUN_18|R1_OVERRUN_17|R1_CID_CSD_OVERWRITE_16|R1_WP_ERASE_SKIP_15|R1_CARD_ECC_DISABLE_14                           \
                        |R1_ERASE_RESET_13)
#define SD_COM_ERRORS   (R1_OUT_OF_RANGE_31|R1_ERASE_PARAM_27|R1_WP_VIOLATION_26|R1_CARD_IS_LOCKED_25|R1_LOCK_UNLOCK_FAILED_24                     \
                        |R1_COM_CRC_ERROR_23|R1_ILLEGAL_COMMAND_22|R1_CARD_ECC_FAILED_21|R1_CC_ERROR_20|R1_ERROR_19                                \
                        |R1_UNDERRUN_18|R1_OVERRUN_17|R1_CID_CSD_OVERWRITE_16|R1_WP_ERASE_SKIP_15|R1_CARD_ECC_DISABLE_14                           \
                        |R1_ERASE_RESET_13)

// SDIO macro.
//masks for card status contained in R4.
#define SDIO_OCR(R4)                            (((UINT32)(R4)) & 0xFFFFFF)    
#define SDIO_MEMPRESENT(R4)            ((((UINT32)(R4)) >> 27) & 0x1)    
#define SDIO_NUMIO(R4)                       ((((UINT32)(R4)) >> 28) & 0x3)    
#define SDIO_IORDY(R4)                       ((((UINT32)(R4)) >> 31) & 0x1)    

// SDIO argument.
#define SDIO_READ                     ((UINT32)0)
#define SDIO_WRITE                   ((UINT32)1)
#define SDIO_FUNC(X)                (((UINT32)(X)) & 0x7)
#define SDIO_BLOCK_MODE       ((UINT32)1)
#define SDIO_BYTE_MODE          ((UINT32)0)
#define SDIO_OP_FIXADR          ((UINT32)0)
#define SDIO_OP_INCADR          ((UINT32)1)
#define SDIO_RAW_NO               ((UINT32)0)
#define SDIO_RAW_YES              ((UINT32)1)
#define SDIO_REGADR(X)            (((UINT32)(X)) & 0x1FFFF)
#define SDIO_WDATA(X)             (((UINT32)(X)) & 0xFF)
#define SDIO_COUNT(X)              (((UINT32)(X)) & 0x1FF)
#define SDIO_BYTE_COUNT(X)    ((((UINT32)(X)) == 512) ? (0):(((UINT32)(X)) & 0x1FF))


// SDIO CMD52 argument.
#define SDIO52_RWFLAG(u4Arg, rwflag)                 (u4Arg |= ((rwflag) << 31))
#define SDIO52_FUNCNUM(u4Arg, func)                   (u4Arg |= ((func) << 28))
#define SDIO52_RAWFLAG(u4Arg, rawflag)             (u4Arg |= ((rawflag) << 27))
#define SDIO52_REGADDR(u4Arg, regaddr)             (u4Arg |= ((regaddr) << 9))
#define SDIO52_DATA(u4Arg, data)                         (u4Arg |= (data))

#define SDIO52_CMD(u4Arg, rwflag, func, rawflag, regaddr, data) \
    u4Arg = 0; \
    SDIO52_RWFLAG(u4Arg, rwflag); \
    SDIO52_FUNCNUM(u4Arg, func); \
    SDIO52_RAWFLAG(u4Arg, rawflag); \
    SDIO52_REGADDR(u4Arg, regaddr); \
    SDIO52_DATA(u4Arg, data)
                                                                                    
// SDIO CMD53 argument.
#define SDIO53_BLKMOD(u4Arg, blkmode)             (u4Arg |= ((blkmode) << 27))
#define SDIO53_OPCODE(u4Arg, inc)                       (u4Arg |= ((inc) << 26))
#define SDIO53_COUNT(u4Arg, count)                     (u4Arg |= ((count) & 0x1FF))

#define SDIO53_CMD(u4Arg, rwflag, func, blkmode, inc, regaddr, count) \
    u4Arg = 0; \
    SDIO52_RWFLAG(u4Arg, rwflag); \
    SDIO52_FUNCNUM(u4Arg, func); \
    SDIO53_BLKMOD(u4Arg, blkmode); \
    SDIO53_OPCODE(u4Arg, inc); \
    SDIO52_REGADDR(u4Arg, regaddr); \
    SDIO53_COUNT(u4Arg, count)

// SDIO state.
#define SDIO_STATE_DIS          0x00
#define SDIO_STATE_CMD         0x01
#define SDIO_STATE_TRN          0x02

// SDIO R5 response mask.
#define SDIOR5_RESP_CRCERR          0x80
#define SDIOR5_RESP_ILLCMD          0x40
#define SDIOR5_RESP_CURSTATE      0x30
#define SDIOR5_RESP_GENERR          0x08
#define SDIOR5_RESP_RFU                 0x04
#define SDIOR5_RESP_FUNNUM         0x02
#define SDIOR5_RESP_OUTRANGE     0x01

// SDIO CIS tuple code.
#define SDIO_CISTPL_NULL                0x00
#define SDIO_CISTPL_CHECKSUM      0x10
#define SDIO_CISTPL_VER_1              0x15
#define SDIO_CISTPL_ALTSTR            0x16
#define SDIO_CISTPL_MANFID           0x20
#define SDIO_CISTPL_FUNCID            0x21
#define SDIO_CISTPL_FUNCE              0x22
#define SDIO_CISTPL_VENDUNQSTART        0x80
#define SDIO_CISTPL_VENDUNQEND            0x8F
#define SDIO_CISTPL_SDIOSTD          0x91
#define SDIO_CISTPL_SDIOEXT          0x92
#define SDIO_CISTPL_END                  0xFF

// SDIO CIS address macro.
#define SDIO_CIS_ADDR(X0B, X0A, X09)    (((X0B)<<16) |((X0A)<<8)|(X09))

// SDIO CCCR Register name.
#define CCCR_SDIOREV            0x00
#define CCCR_SDSPECREV        0x01
#define CCCR_IOENABLE           0x02
#define CCCR_IOREADY            0x03
#define CCCR_INTENABLE         0x04
#define CCCR_INTPENDING       0x05
#define CCCR_IOABORT             0x06
#define CCCR_BUSCONFIG         0x07
#define CCCR_CAPABILITY        0x08
#define CCCR_CISPTR_1           0x09
#define CCCR_CISPTR_2           0x0A
#define CCCR_CISPTR_3           0x0B
#define CCCR_BUSSUSPEND     0x0C
#define CCCR_FUNCSELECT      0x0D
#define CCCR_EXECFLAG          0x0E
#define CCCR_READYFLAG       0x0F
#define CCCR_BLKSIZE_1         0x10
#define CCCR_BLKSIZE_2         0x11
#define CCCR_POWER               0x12
#define CCCR_HIGHSPEED         0x13

// SDIO FBR Register name.
#define FBR_FUNCCODE_0            0x00
#define FBR_FUNCCODE_1            0x01
#define FBR_POWESELECT            0x02
#define FBR_CISPTR_1                 0x09
#define FBR_CISPTR_2                 0x0A
#define FBR_CISPTR_3                 0x0B
#define FBR_CSAPTR_1                0x0C
#define FBR_CSAPTR_2                0x0D
#define FBR_CSAPTR_3                0x0E
#define FBR_CSADATA                  0x0F
#define FBR_BLKSIZE_1               0x10
#define FBR_BLKSIZE_2               0x11


// SDIO CCCR macro in card capability register.
#define SDIO_CAPABLE_SDC(cccrptr)    (cccrptr[CCCR_CAPABILITY]  & 0x1)
#define SDIO_CAPABLE_SMB(cccrptr)    ((cccrptr[CCCR_CAPABILITY] >> 1)& 0x1)
#define SDIO_CAPABLE_SRW(cccrptr)   ((cccrptr[CCCR_CAPABILITY] >> 2)& 0x1)
#define SDIO_CAPABLE_SBS(cccrptr)    ((cccrptr[CCCR_CAPABILITY] >> 3)& 0x1)
#define SDIO_CAPABLE_S4MI(cccrptr)    ((cccrptr[CCCR_CAPABILITY] >> 4)& 0x1)
#define SDIO_CAPABLE_E4MI(cccrptr)    ((cccrptr[CCCR_CAPABILITY] >> 5)& 0x1)
#define SDIO_CAPABLE_LSC(cccrptr)    ((cccrptr[CCCR_CAPABILITY] >> 6)& 0x1)
#define SDIO_CAPABLE_4BLS(cccrptr)   ((cccrptr[CCCR_CAPABILITY] >> 7)& 0x1)

// SDIO CCCR macro in power control register.
#define SDIO_POWER_SMPC(cccrptr)    (cccrptr[CCCR_POWER]  & 0x1)
#define SDIO_POWER_EMPC(cccrptr)    ((cccrptr[CCCR_POWER] >> 1)& 0x1)

// SDIO CCCR macro in High-Speed control register.
#define SDIO_HIGHSPEED_SHS(cccrptr)      (cccrptr[CCCR_HIGHSPEED]& 0x1)
#define SDIO_HIGHSPEED_EHS(cccrptr)      ((cccrptr[CCCR_HIGHSPEED] >> 1)& 0x1)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

#endif // SDC_CONST_H
