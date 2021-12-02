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
 * $RCSfile: nandhw_reg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_reg.h
 *  nand_reg.h The NAND register constant macros
 */

#ifndef NAND_REG_H
#define NAND_REG_H

#include "x_typedef.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define NAND_DRAM_BASE                              ((UINT32)0x80000000)

#define REG_ACDROP_GPIO                             0xF000D540  //GPIOIN23
    #define REG_ACDROP_GPIOMASK                     0x800000    //GPIOIN23

// RS232 12-bit/24-bit ECC controller selection
#define NAND_NFI_PERMISC                            0x004028
    #define NAND_NFI_PERMISC_NFI_SEL                    (((UINT32) 1) << 0)
    
#define NAND_NFI_CNFG                               0x00800
    #define NAND_NFI_CNFG_AHB_MODE                      (((UINT32) 1) << 0)
    #define NAND_NFI_CNFG_READ_MODE                     (((UINT32) 1) << 1)
    #define NAND_NFI_CNFG_SEL_SEC_512BYTE               (((UINT32) 1) << 5)
    #define NAND_NFI_CNFG_BYTE_RW                       (((UINT32) 1) << 6)
    #define NAND_NFI_CNFG_HW_ECC_EN                     (((UINT32) 1) << 8)
    #define NAND_NFI_CNFG_AUTO_FMT_EN                   (((UINT32) 1) << 9)
    #define NAND_NFI_CNFG_OP_MODE(x)                    ((((UINT32) x ) & 0x07) << 12)
        #define NAND_NFI_CNFG_OP_IDLE                       (((UINT32) 0) << 12)
        #define NAND_NFI_CNFG_OP_READ                       (((UINT32) 1) << 12)
        #define NAND_NFI_CNFG_OP_SINGLE                     (((UINT32) 2) << 12)
        #define NAND_NFI_CNFG_OP_PROGRAM                    (((UINT32) 3) << 12)
        #define NAND_NFI_CNFG_OP_ERASE                      (((UINT32) 4) << 12)
        #define NAND_NFI_CNFG_OP_RESET                      (((UINT32) 5) << 12)
        #define NAND_NFI_CNFG_OP_CUSTOM                     (((UINT32) 6) << 12)
        #define NAND_NFI_CNFG_OP_RESERVE                    (((UINT32) 7) << 12)

#define NAND_NFI_PAGEFMT                            0x00804
    #define NAND_NFI_PAGEFMT_PAGE(x)                    ((((UINT32) x ) & 0x03) << 0)
        #define NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k           (((UINT32) 0) << 0)
        #define NAND_NFI_PAGEFMT_PAGE_SIZE_4k               (((UINT32) 1) << 0)
        #define NAND_NFI_PAGEFMT_PAGE_SIZE_8k               (((UINT32) 2) << 0)
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #define NAND_NFI_PAGEFMT_SPARE(x)                   ((((UINT32) x ) & 0x0F) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_16                   (((UINT32) 0) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_26                   (((UINT32) 1) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_27                   (((UINT32) 2) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_28                   (((UINT32) 3) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_32                   (((UINT32) 4) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_36                   (((UINT32) 5) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_40                   (((UINT32) 6) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_44                   (((UINT32) 7) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_48                   (((UINT32) 8) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_50                   (((UINT32) 9) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_52                   (((UINT32) 10) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_54                   (((UINT32) 11) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_56                   (((UINT32) 12) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_62                   (((UINT32) 13) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_63                   (((UINT32) 14) << 2)
        #define NAND_NFI_PAGEFMT_SPARE_64                   (((UINT32) 15) << 2)
#else
    #define NAND_NFI_PAGEFMT_SPARE(x)                   ((((UINT32) x ) & 0x03) << 4)
        #define NAND_NFI_PAGEFMT_SPARE_16                   (((UINT32) 0) << 4)
        #define NAND_NFI_PAGEFMT_SPARE_26                   (((UINT32) 1) << 4)
        #define NAND_NFI_PAGEFMT_SPARE_27                   (((UINT32) 2) << 4)
        #define NAND_NFI_PAGEFMT_SPARE_28                   (((UINT32) 3) << 4)
#endif 
    #define NAND_NFI_PAGEFMT_FDM_NUM(x)                 ((((UINT32) x ) & 0x1F) << 6)
    #define NAND_NFI_PAGEFMT_ECC_NUM(x)                 ((((UINT32) x ) & 0x1F) << 11)
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #define NAND_NFI_PAGEFMT_SECTOR_SIZE(x)              ((((UINT32) x ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_512_16      ((((UINT32) 0x210 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32     ((((UINT32) 0x420 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_52     ((((UINT32) 0x434 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_54     ((((UINT32) 0x436 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_56     ((((UINT32) 0x438 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_64     ((((UINT32) 0x440 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_72     ((((UINT32) 0x448 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_80     ((((UINT32) 0x450 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_88     ((((UINT32) 0x458 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_96     ((((UINT32) 0x460 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_100    ((((UINT32) 0x464 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_104    ((((UINT32) 0x468 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_108    ((((UINT32) 0x46C ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_112    ((((UINT32) 0x470 ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_124    ((((UINT32) 0x47C ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_126    ((((UINT32) 0x47E ) & 0x7FF) << 16)
        #define NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_128    ((((UINT32) 0x480 ) & 0x7FF) << 16)
#endif
#define NAND_NFI_CON                                0x00808
    #define NAND_NFI_CON_FIFO_FLUSH                     (((UINT32) 1) << 0)
    #define NAND_NFI_CON_NFI_RST                        (((UINT32) 1) << 1)
    #define NAND_NFI_CON_SRD                            (((UINT32) 1) << 4)
    #define NAND_NFI_CON_NOB(x)                         ((((UINT32) x ) & 0x7) << 5)
    #define NAND_NFI_CON_BRD                            (((UINT32) 1) << 8)
    #define NAND_NFI_CON_BWR                            (((UINT32) 1) << 9)
    #define NAND_NFI_CON_SEC_NUM(x)                     ((((UINT32) x ) & 0xF) << 12)

#define NAND_NFI_ACCCON1                            0x0080C
#define NAND_NFI_ACCCON2                            0x009A4

#define NAND_NFI_INTR_EN                            0x00810
    #define NAND_NFI_INTR_EN_RD_DONE                    (((UINT32) 1) << 0)
    #define NAND_NFI_INTR_EN_WR_DONE                    (((UINT32) 1) << 1)
    #define NAND_NFI_INTR_EN_RESET_DONE                 (((UINT32) 1) << 2)
    #define NAND_NFI_INTR_EN_ERASE_DONE                 (((UINT32) 1) << 3)
    #define NAND_NFI_INTR_EN_BUSY_RETURN                (((UINT32) 1) << 4)
    #define NAND_NFI_INTR_EN_ACCESS_LOCK                (((UINT32) 1) << 5)
    #define NAND_NFI_INTR_EN_AHB_DONE                   (((UINT32) 1) << 6)
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #define NAND_NFI_INTR_EN_BUSY_RETURN_EN2            (((UINT32) 1) << 7)
#endif 
    #define NAND_NFI_INTR_EN_RS232RD_DONE               (((UINT32) 1) << 8)
    #define NAND_NFI_INTR_EN_RS232WR_DONE               (((UINT32) 1) << 9)
    #define NAND_NFI_INTR_EN_MDMA_DONE                  (((UINT32) 1) << 10)

#define NAND_NFI_INTR                               0x00814
    #define NAND_NFI_INTR_RD_DONE                       (((UINT32) 1) << 0)
    #define NAND_NFI_INTR_WR_DONE                       (((UINT32) 1) << 1)
    #define NAND_NFI_INTR_RESET_DONE                    (((UINT32) 1) << 2)
    #define NAND_NFI_INTR_ERASE_DONE                    (((UINT32) 1) << 3)
    #define NAND_NFI_INTR_BUSY_RETURN                   (((UINT32) 1) << 4)
    #define NAND_NFI_INTR_ACCESS_LOCK                   (((UINT32) 1) << 5)
    #define NAND_NFI_INTR_AHB_DONE                      (((UINT32) 1) << 6)
    #define NAND_NFI_INTR_RS232RD_DONE                  (((UINT32) 1) << 8)
    #define NAND_NFI_INTR_RS232WR_DONE                  (((UINT32) 1) << 9)
    #define NAND_NFI_INTR_MDMA_DONE                     (((UINT32) 1) << 10)
    #define NAND_NFI_INTR_MASK                          ((UINT32) 0x44B)

#define NAND_NFI_CMD                                0x00820
    
#define NAND_NFI_ADDRNOB                            0x00830
#define NAND_NFI_COLADDR                            0x00834
#define NAND_NFI_ROWADDR                            0x00838
#define NAND_NFI_STRDATA                            0x00840
    #define NAND_NFI_STRDATA_STRDATA                    (((UINT32) 0x1) << 0)
    
#define NAND_NFI_DATAW                              0x00850
#define NAND_NFI_DATAWB                             0x00850
#define NAND_NFI_DATAR                              0x00854
#define NAND_NFI_DATARB                             0x00854

#define NAND_NFI_STA                                0x00860
    #define NAND_NFI_STA_NFI_FSM                        (((UINT32) 0xF) << 16)
        #define NAND_NFI_STA_NFI_FSM_READ_DATA              (((UINT32) 0x3) << 16)
        
#define NAND_NFI_FIFOSTA                            0x00864
    #define NAND_NFI_FIFOSTA_RD_REMAIN                  (((UINT32) 0x1F) << 0)
    #define NAND_NFI_FIFOSTA_RD_EMPTY                   (((UINT32) 1) << 6)
    #define NAND_NFI_FIFOSTA_WT_EMPTY                   (((UINT32) 1) << 14)

#define NAND_NFI_LOCKSTA                            0x00868
#define NAND_NFI_ADDRCNTR                           0x00870
    #define NAND_NFI_ADDRCNTR_SEC_CNTR(x)               ((((UINT32) x ) & 0xF) << 12)

#define NAND_NFI_STRADDR                            0x00880
#define NAND_NFI_BYTELEN                            0x00884

// I/O Pin Control
#define NAND_NFI_CSEL                               0x00890
#define NAND_NFI_IOCON                              0x00894

//FDM Data Content
#define NAND_NFI_FDM0L                              0x008A0
#define NAND_NFI_FDM0M                              0x008A4
#define NAND_NFI_FDM0L2                             0x008A8
#define NAND_NFI_FDM0M2                             0x008AC
#define NAND_NFI_FDM_LEN                            16
#define NAND_NFI_FDM_SECTNUM                        8

//Flash Lock
#define NAND_NFI_LOCKEN                             0x009E0
#define NAND_NFI_LOCKCON                            0x009E4
#define NAND_NFI_LOCKANOB                           0x009E8
#define NAND_NFI_LOCK00ADD                          0x009F0
#define NAND_NFI_LOCK00FMT                          0x009F4
#define NAND_NFI_LOCK01ADD                          0x009F8
#define NAND_NFI_LOCK01FMT                          0x009FC
#define NAND_NFI_LOCK02ADD                          0x00920
#define NAND_NFI_LOCK02FMT                          0x00924
#define NAND_NFI_LOCK03ADD                          0x00928
#define NAND_NFI_LOCK03FMT                          0x0092C
#define NAND_NFI_LOCK04ADD                          0x00930
#define NAND_NFI_LOCK04FMT                          0x00934
#define NAND_NFI_LOCK05ADD                          0x00938
#define NAND_NFI_LOCK05FMT                          0x0093C
#define NAND_NFI_LOCK06ADD                          0x00940
#define NAND_NFI_LOCK06FMT                          0x00944
#define NAND_NFI_LOCK07ADD                          0x00948
#define NAND_NFI_LOCK07FMT                          0x0094C
#define NAND_NFI_LOCK08ADD                          0x00950
#define NAND_NFI_LOCK08FMT                          0x00954
#define NAND_NFI_LOCK09ADD                          0x00958
#define NAND_NFI_LOCK09FMT                          0x0095C
#define NAND_NFI_LOCK10ADD                          0x00960
#define NAND_NFI_LOCK10FMT                          0x00964
#define NAND_NFI_LOCK11ADD                          0x00968
#define NAND_NFI_LOCK11FMT                          0x0096C
#define NAND_NFI_LOCK12ADD                          0x00970
#define NAND_NFI_LOCK12FMT                          0x00974
#define NAND_NFI_LOCK13ADD                          0x00978
#define NAND_NFI_LOCK13FMT                          0x0097C
#define NAND_NFI_LOCK14ADD                          0x00980
#define NAND_NFI_LOCK14FMT                          0x00984
#define NAND_NFI_LOCK15ADD                          0x00988
#define NAND_NFI_LOCK15FMT                          0x0098C

//Debug Register
#define NAND_NFI_FIFODATA0                          0x00990
#define NAND_NFI_FIFODATA1                          0x00994
#define NAND_NFI_FIFODATA2                          0x00998
#define NAND_NFI_FIFODATA3                          0x0099C

#define NAND_NFI_MISC                               0x009A0
    #define NAND_NFI_MISC_FLASH_PMUX                    (((UINT32) 1) << 2)
#define NAND_NFI_LCD2NAND                           0x009A4
// clock division
#define NAND_NFI_CLKDIV                             0x009AC
    #define NAND_NFI_CLKDIV_EN                          (((UINT32) 1) << 0)

// multi-page dma
#define NAND_NFI_MDMACON                            0x009B0
    #define NAND_NFI_MDMA_TRIG                          (((UINT32) 1) << 0)
    #define NAND_NFI_MDMA_EN                            (((UINT32) 1) << 4)
    #define NAND_NFI_MDMA_MODE(x)                       ((((UINT32) x ) & 0x3) << 8)
        #define NAND_NFI_MDMA_READ                          (((UINT32) 0) << 8)
        #define NAND_NFI_MDMA_WRITE                         (((UINT32) 1) << 8)
        #define NAND_NFI_MDMA_ERASE                         (((UINT32) 2) << 8)
    #define NAND_NFI_MDMA_LEN(x)                        ((((UINT32) x ) & 0x7) << 12)

#define NAND_NFI_MDMA_PAGENUM                       8
#define NAND_NFI_MDMAADDR                           0x009B4
    #define NAND_NFI_MDMAADDR_DRAMADDR                  (NAND_NFI_MDMA_PAGENUM * 0)
    #define NAND_NFI_MDMAADDR_ROWADDR                   (NAND_NFI_MDMA_PAGENUM * 1)
    #define NAND_NFI_MDMAADDR_FDM                       (NAND_NFI_MDMA_PAGENUM * 2)
    #define NAND_NFI_MDMAADDR_DECDONE                   (NAND_NFI_MDMA_PAGENUM * 34)
    #define NAND_NFI_MDMAADDR_DECFER                    (NAND_NFI_MDMA_PAGENUM * 35)
    #define NAND_NFI_MDMAADDR_DECENUM0                  (NAND_NFI_MDMA_PAGENUM * 36)
    #define NAND_NFI_MDMAADDR_DECENUM1                  (NAND_NFI_MDMA_PAGENUM * 37)
    #define NAND_NFI_MDMAADDR_DECEL0                    (NAND_NFI_MDMA_PAGENUM * 38)
#define NAND_NFI_MDMADATA                           0x009B8

#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
// random mizer
#define NAND_NFI_RANDOM_CFG                         0x009BC
    #define NAND_NFI_ENCODING_RANDON_EN                 (((UINT32) 1) << 0)
    #define NAND_NFI_ENCODING_RANDON_SEED(x)            ((((UINT32) x ) & 0x7FFF) << 1)
    #define NAND_NFI_DECODING_RANDON_EN                 (((UINT32) 1) << 16)
    #define NAND_NFI_DECODING_RANDON_SEED(x)            ((((UINT32) x ) & 0x7FFF) << 17)
#else
// scramble
#define NAND_NFI_SCRAMBLE                           0x009BC
    #define NAND_NFI_INTERLEAVE_EN                      (((UINT32) 1) << 0)
    #define NAND_NFI_SCRAMBLE_EN                        (((UINT32) 1) << 4)
    #define NAND_NFI_SCRAMBLE_PAT(x)                    ((((UINT32) x ) & 0xFF) << 8)
#endif 

#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
#define NAND_SELECT_REGISTER                        0x00BF0
#define NAND_NFI_MLC_CFG                            0x00BFC
    #define NAND_CHK_DRAM_WDLE_EN                       (((UINT32) 1) << 30)
    #define NAND_CHK_DRAM_WDLE_EN2                      (((UINT32) 1) << 31)
#endif

// ECC
#define NAND_NFIECC_1BIT                         	  14	// 1 bit ECC need 14 bit ecc code
#define NAND_NFIECC_ENCON                           0x00A00
#define NAND_NFIECC_ENCCNFG                         0x00A04
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
// 5398 60bit ECC ability
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_4              (((UINT32) 0) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_8              (((UINT32) 2) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_10             (((UINT32) 3) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_12             (((UINT32) 4) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_20             (((UINT32) 8) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_22             (((UINT32) 9) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_24             (((UINT32) 10) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_28             (((UINT32) 11) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_32             (((UINT32) 12) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_36             (((UINT32) 13) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_40             (((UINT32) 14) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_44             (((UINT32) 15) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_48             (((UINT32) 16) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_52             (((UINT32) 17) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_56             (((UINT32) 18) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_60             (((UINT32) 19) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_NFI_MODE            (((UINT32) 1) << 5)
    #define NAND_NFIECC_ENCCNFG_ENC_MS_520              (((UINT32) 0x1040) << 16)
    #define NAND_NFIECC_ENCCNFG_ENC_MS_1032             (((UINT32) 0x2040) << 16)
    #define NAND_NFIECC_ENCCNFG_ENC_MS_1040             (((UINT32) 0x2080) << 16)
#else
// 24bit ECC ability
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_4              (((UINT32) 0) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_8              (((UINT32) 2) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_10             (((UINT32) 3) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_12             (((UINT32) 4) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_20             (((UINT32) 8) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_TNUM_24             (((UINT32) 10) << 0)
    #define NAND_NFIECC_ENCCNFG_ENC_NFI_MODE            (((UINT32) 1) << 4)
    #define NAND_NFIECC_ENCCNFG_ENC_MS_520              (((UINT32) 0x1040) << 16)
    #define NAND_NFIECC_ENCCNFG_ENC_MS_1032             (((UINT32) 0x2040) << 16)
    #define NAND_NFIECC_ENCCNFG_ENC_MS_1040             (((UINT32) 0x2080) << 16)
#endif 

#define NAND_NFIECC_ENCDIADDR                       0x00A08
#define NAND_NFIECC_ENCIDLE                         0x00A0C
#define NAND_NFIECC_ENCSTA                          0x00A24
    #define NAND_NFIECC_ENCSTA_FSM_MASK                 (((UINT32) 0x3F) << 0)
    #define NAND_NFIECC_ENCSTA_FSM_IDLE                 (((UINT32) 0) << 0)
    #define NAND_NFIECC_ENCSTA_FSM_WAITIN               (((UINT32) 1) << 0)
    #define NAND_NFIECC_ENCSTA_FSM_BUSY                 (((UINT32) 2) << 0)
    #define NAND_NFIECC_ENCSTA_FSM_PAROUT               (((UINT32) 4) << 0)
    #define NAND_NFIECC_ENCSTA_COUNT_PS                 (((UINT32) 0x1FF) << 7)
    #define NAND_NFIECC_ENCSTA_COUNT_MS                 (((UINT32) 0x3FFF) << 16)
#define NAND_NFIECC_ENCIRQEN                        0x00A28
#define NAND_NFIECC_ENCIRQSTA                       0x00A2C

#define NAND_NFIECC_ENCPAR0                         0x00A30
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #define NAND_NFI_ENCPAR_NUM                         27
#else
    #define NAND_NFI_ENCPAR_NUM                         11
#endif
#define NAND_NFIECC_DECCON                          0x00B00
#define NAND_NFIECC_DECCNFG                         0x00B04
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_4              (((UINT32) 0) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_8              (((UINT32) 2) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_10             (((UINT32) 3) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_12             (((UINT32) 4) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_20             (((UINT32) 8) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_24             (((UINT32) 10) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_28             (((UINT32) 11) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_32             (((UINT32) 12) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_36             (((UINT32) 13) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_40             (((UINT32) 14) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_44             (((UINT32) 15) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_48             (((UINT32) 16) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_52             (((UINT32) 17) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_56             (((UINT32) 18) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_60             (((UINT32) 19) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_NFI_MODE            (((UINT32) 1) << 5)
    #define NAND_NFIECC_DECCNFG_SEL_CHIEN_SEARCH        (((UINT32) 1) << 7)
#else
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_4              (((UINT32) 0) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_8              (((UINT32) 2) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_10             (((UINT32) 3) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_12             (((UINT32) 4) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_20             (((UINT32) 8) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_TNUM_24             (((UINT32) 10) << 0)
    #define NAND_NFIECC_DECCNFG_DEC_NFI_MODE            (((UINT32) 1) << 4)
#endif 
    #define NAND_NFIECC_DECCNFG_DEC_CON_NONE            (((UINT32) 1) << 12)
    #define NAND_NFIECC_DECCNFG_DEC_CON_SOFT            (((UINT32) 2) << 12)
    #define NAND_NFIECC_DECCNFG_DEC_CON_AUTO            (((UINT32) 3) << 12)
    #define NAND_NFIECC_DECCNFG_DEC_CS_520_4            (((UINT32) 0x1078) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_520_10           (((UINT32) 0x10CC) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_8           (((UINT32) 0x20B0) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_12          (((UINT32) 0x20E8) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1040_8           (((UINT32) 0x20F0) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1040_20          (((UINT32) 0x2198) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_24          (((UINT32) 0x2190) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_28          (((UINT32) 0x21C8) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_32          (((UINT32) 0x2200) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_36          (((UINT32) 0x2238) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_40          (((UINT32) 0x2270) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_44          (((UINT32) 0x22A8) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_48          (((UINT32) 0x22E0) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_52          (((UINT32) 0x2318) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_56          (((UINT32) 0x2350) << 16)
    #define NAND_NFIECC_DECCNFG_DEC_CS_1032_60          (((UINT32) 0x2388) << 16)
    
    #define NAND_NFIECC_DECCNFG_DEC_CS_EMPTY_EN         (((UINT32) 1) << 31)

#define NAND_NFIECC_DECDIADDR                       0x00B08
#define NAND_NFIECC_DECIDLE                         0x00B0C
#define NAND_NFIECC_DECFER                          0x00B10
#define NAND_NFIECC_DECDONE                         0x00B18
#define NAND_NFIECC_DECIRQEN                        0x00B34
#define NAND_NFIECC_DECIRQSTA                       0x00B38
#define NAND_NFIECC_FDMADDR                         0x00B3C
#define NAND_NFIECC_DECFSM                          0x00B40
#define NAND_NFIECC_SYNSTA                          0x00B44
#define NAND_NFIECC_DECNFIDI                        0x00B48
#define NAND_NFIECC_SYN0                            0x00B4C
#define NAND_NFIECC_DECENUM0                        0x00B50
#define NAND_NFIECC_DECENUM1                        0x00B54
#define NAND_NFIECC_DECEL0                          0x00B60
#if defined(CC_MT5880)
#define NAND_DMMERGE_CFG                            0x00FF8
#endif
//-----------------------------------------------------------------------------
// Common
//-----------------------------------------------------------------------------
#define NAND_CMD_READ1_00                           0x00
#define NAND_CMD_READ1_01                           0x01
#define NAND_CMD_PROG_PAGE                          0x10 /* WRITE 2 */
#define NAND_CMD_READ_30                            0x30
#define NAND_CMD_READ2                              0x50
#define NAND_CMD_ERASE1_BLK                         0x60
#define NAND_CMD_STATUS                             0x70
#define NAND_CMD_INPUT_PAGE                         0x80    /* WRITE 1 */
#define NAND_CMD_READ_ID                            0x90
#define NAND_CMD_ERASE2_BLK                         0xD0
#define NAND_CMD_RESET                              0xFF

#define STATUS_WRITE_PROTECT                        0x0
#define STATUS_READY_BUSY                           0x40
#define STATUS_ERASE_SUSPEND                        0x20
#define STATUS_PASS_FAIL                            0x01

#endif /* NAND_REG_H */

