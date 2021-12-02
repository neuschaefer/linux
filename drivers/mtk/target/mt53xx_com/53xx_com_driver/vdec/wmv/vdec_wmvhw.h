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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_wmvhw.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvhw.c
 *  This header file declares exported APIs of wmv.
 */

#ifndef _VDEC_WMVHW_H
#define _VDEC_WMVHW_H

#include "vdec_wmvutil.h"
#include "x_lint.h"
#include "x_hal_5381.h"
#include "x_bim.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


// **********************************************************************
// VLD Register
// **********************************************************************
//#define VLD_REG_OFST     (0x400 )
#define VLD_REG_OFST     (IO_VIRT+0x2000 )

// Barral Shifter from 0x00 - 0x80
#define RO_VLD_BARL      (VLD_REG_OFST + (0x00 ))
#define RW_VLD_VDOUFM    (VLD_REG_OFST + (0x84 ))
#define VDOUFM           0x1
#define MXOFF            (0x1 << 8)
#define ENSTCNT          (0x1 << 9)
//#define FINPHDR          (0x1 << 16)

// Type 18 [BIT14] out_of_picture error
// Type 1  [BIT15] block pixel overflow
// Type 2  [BIT16] cbp number mismatch
// Type 3  [BIT17] dct_dc_size_lum table error
// Type 4  [BIT18] dct_dc_size_chrom table error
// Type 5  [BIT19] slice-layer syntax error
// Type 6  [BIT20] end_of_block syntax rooer
// Type 7  [BIT21] macroblock address increment table error
// Type 8  [BIT22] coded_block_pattern table error
// Type 9  [BIT23] insufficient slice error
// Type 10 [BIT24] slice-level quantization scale error
// Type 11 [BIT25] other slice-level syntax error
// Type 12 [BIT26] macroblock address overflow error
// Type 13 [BIT27] mb_type table error
// Type 14 [BIT28] motion type error
// Type 15 [BIT29] motion code error
// Type 16 [BIT30] marker syntax error
// Type 17 [BIT31] MC-IDCT busy overflow
#define ERR_OUT_PIC          ((unsigned)0x1 << 14)
#define ERR_BL_PIX           ((unsigned)0x1 << 15)
#define ERR_CBP_NS           ((unsigned)0x1 << 16)
#define ERR_DCT_Y_TAB        ((unsigned)0x1 << 17)
#define ERR_DCT_C_TAB        ((unsigned)0x1 << 18)
#define ERR_SL_SYN           ((unsigned)0x1 << 19)
#define ERR_EOB_SYN          ((unsigned)0x1 << 20)
#define ERR_MB_ADD_INC_TAB   ((unsigned)0x1 << 21)
#define ERR_CBP_TAB          ((unsigned)0x1 << 22)
#define ERR_INS_SL           ((unsigned)0x1 << 23)
#define ERR_SL_QSCALE        ((unsigned)0x1 << 24)
#define ERR_SL_SYN2          ((unsigned)0x1 << 25)
#define ERR_MB_ADD           ((unsigned)0x1 << 26)
#define ERR_MB_TBL           ((unsigned)0x1 << 27)
#define ERR_MOT_TP           ((unsigned)0x1 << 28)
#define ERR_MOT_CD           ((unsigned)0x1 << 29)
#define ERR_MKB              ((unsigned)0x1 << 30)
#define ERR_MCIDCT_BSY       ((unsigned)0x1 << 31)

// Error Type Define
#define ETP_OUT_PIC          18
#define ETP_BL_PIX           1
#define ETP_CBP_NS           2
#define ETP_DCT_Y_TAB        3
#define ETP_DCT_C_TAB        4
#define ETP_SL_SYN           5
#define ETP_EOB_SYN          6
#define ETP_MB_ADD_INC_TAB   7
#define ETP_CBP_TAB          8
#define ETP_INS_SL           9
#define ETP_SL_QSCALE        10
#define ETP_SL_SYN2          11
#define ETP_MB_ADD           12
#define ETP_MB_TBL           13
#define ETP_MOT_TP           14
#define ETP_MOT_CD           15
#define ETP_MKB              16
#define ETP_MCIDCT_BSY       17


#define RW_VLD_PARA      (VLD_REG_OFST + (0x88 ))
#define RW_VLD_PROC      (VLD_REG_OFST + (0x8C ))
#define SSCBIT           (0x1 << 16)
#define SSCUINT8          (0x1 << 17)
#define VLDABRT          (0x1 << 18)
#define PSUP             (0x1 << 19)
#define INIFET           (0x1 << 20)
#define PDHW             (0x1 << 22)
#define INIBR            (0x1 << 23)
#define PERRCON          (0x1 << 24)
#define RTERR            (0x1 << 25)
#define EOFR             (0x1 << 26)
#define IGBCL            (0x1 << 27)
#define DECTOP           (0x1 << 29)
#define DECBTM           (0x2 << 29)
#define FPRBS            (0x3 << 29)
#define B21EN            ((unsigned)0x1 << 31)

#define RW_VLD_PICSZ     (VLD_REG_OFST + (0x90 ))
#define RW_VLD_MBROWPRM  (VLD_REG_OFST + (0x94 ))
#define RW_VLD_DIGMBSA   (VLD_REG_OFST + (0x98 ))
#define RW_VLD_SCALE     (VLD_REG_OFST + (0x9C ))
#define H_SCALE_1_1          0
#define H_SCALE_1_2          1
#define H_SCALE_1_4          2
#define H_SCALE_3_4          3
#define H_SCALE_3_8          4
#define V_SCALE_1_1          0
#define V_SCALE_1_2          1
#define V_SCALE_1_4          2
#define RW_VLD_TABLIM    (VLD_REG_OFST + (0xA0 ))
#define RW_VLD_TABLNIM   (VLD_REG_OFST + (0xA4 ))
#define RW_VLD_DIGMBYOFF (VLD_REG_OFST + (0xA8 ))
#define RW_VLD_PSUPCTR   (VLD_REG_OFST + (0xAC ))
#define RW_VLD_RPTR      (VLD_REG_OFST + (0xB0 ))

#define RW_VLD_VSTART    (VLD_REG_OFST + (0xB4 ))
#define RW_VLD_VEND      (VLD_REG_OFST + (0xB8 ))
#define RO_VLD_BLKFIN    (VLD_REG_OFST + (0xBC ))
#define RW_VLD_PWRSAVE   (VLD_REG_OFST + (0xC4 ))
#define RW_VLD_MCPIC_BLKW    (VLD_REG_OFST + (0xC8))
#define RW_VLD_ASYNC            (VLD_REG_OFST +(0xD0))


#define RO_VLD_FSBI      (VLD_REG_OFST + (0xE0 ))
#define RO_VLD_ERRFG     (VLD_REG_OFST + (0xE4 ))
#define RO_VLD_FETCHOK   (VLD_REG_OFST + (0xE8 ))
#define RO_VLD_VBAR       (VLD_REG_OFST + (0xEC ))
#define VLD_BAR_FETCH_FIN  (0x1 << 2)

#define RO_VLD_INPUTWND  (VLD_REG_OFST + (0xF0 ))
#define RO_VLD_SRAMCTRL  (VLD_REG_OFST + (0xF4 ))
#if defined(CC_MT5365) || defined(CC_MT5395)
#define AA_FIT_TARGET_D (1<<0)
#else
#define AA_FIT_TARGET_D (1<<17)
#endif
#define RO_VLD_VWPTR     (VLD_REG_OFST + (0xF8 ))
#define RO_VLD_VRPTR     (VLD_REG_OFST + (0xFC ))
#define RPTR_ALIGN    16
#define WPTR_OFFSET 512//256        //64x4    //mpeg 2 use 144=16x9

#define RW_VLD_WAITT     (VLD_REG_OFST + (0x100 ))
#define WO_VLD_FDEC      (VLD_REG_OFST + (0x104 ))
#define VLD_RELOAD_INTRA_MATRIX     (0x1 << 8)
#define VLD_RELOAD_NONINTRA_MATRIX  (0x1 << 9)
#define VLD_RST_QMATRIX             (0x1 << 10)

#define WO_VLD_SRST      (VLD_REG_OFST + (0x108 ))
#define WO_VLD_BBUFRST   (VLD_REG_OFST + (0x10C ))
#define WO_VLD_VWPTR  (VLD_REG_OFST + (0x110 ))

#define RW_VLD_BITCOUNT  (VLD_REG_OFST + (0x114 ))  // 1389
//#define RW_VLD_BITCOUNT  (VLD_REG_OFST + (0x84 ))  // just test

/* JPEG decoding */
#define WO_VLD_JPG_BITS_TBL   (VLD_REG_OFST + (0x118 ))
#define RW_VLD_JPG_HUFF_TBL   (VLD_REG_OFST + (0x11C ))
#define RW_VLD_JPG_Q_TBL   (VLD_REG_OFST + (0x120 ))
#define RW_VLD_JPG_LAHUFF_IDX   (VLD_REG_OFST + (0x124 ))
#define RW_VLD_JPG_LAHUFF_DATA  (VLD_REG_OFST + (0x128 ))
#define RW_VLD_JPG_BLK_DC_TBL   (VLD_REG_OFST + (0x130 ))
#define RW_VLD_JPG_BLK_AC_TBL   (VLD_REG_OFST + (0x134 ))
#define RW_VLD_JPG_DC_AC_NEEDED   (VLD_REG_OFST + (0x138 ))
#define RW_VLD_JPG_MEMBERSHIP   (VLD_REG_OFST + (0x13C ))
#define RW_VLD_JPG_START   (VLD_REG_OFST + (0x140 ))
#define RW_VLD_JPG_SWITCH   (VLD_REG_OFST + (0x144 ))
#define RW_VLD_JPG_FAST_DEC   (VLD_REG_OFST + (0x148 ))
#define RW_VLD_JPG_RESTART   (VLD_REG_OFST + (0x14C ))
#define RW_VLD_JPG_PRG_MODE   (VLD_REG_OFST + (0x150 ))
#define JPG_BASELINE 0
#define PROG_MODE 0x100
#define JPG_DC_REFINE 1
#define JPG_AC_FIRST 2
#define JPG_AC_REFINE 3
#define JPG_DC_FIRST 4
#define WR_IDCT_DATA 8
#define WR_VLD_COEFF 0
#define RW_VLD_JPG_PRG_NWB   (VLD_REG_OFST + (0x154 ))
#define RW_VLD_JPG_AH_AL   (VLD_REG_OFST + (0x158 ))
#define RW_VLD_JPG_SS_SE   (VLD_REG_OFST + (0x15C ))
#define RW_VLD_JPG_NZ_HIST   (VLD_REG_OFST + (0x160 ))
#define RW_VLD_JPG_MCU_COUNT   (VLD_REG_OFST + (0x164 ))
#define RW_VLD_JPG_WR_EOB_RUN   (VLD_REG_OFST + (0x168 ))
#define RW_VLD_JPG_RD_ADD0   (VLD_REG_OFST + (0x16C ))
#define RW_VLD_JPG_RD_ADD1   (VLD_REG_OFST + (0x170 ))
#define RW_VLD_JPG_RD_ADD2   (VLD_REG_OFST + (0x174 ))
#define RW_VLD_JPG_WR_ADD0   (VLD_REG_OFST + (0x178 ))
#define RW_VLD_JPG_WR_ADD1   (VLD_REG_OFST + (0x17C ))
#define RW_VLD_JPG_WR_ADD2   (VLD_REG_OFST + (0x180 ))
#define RW_VLD_JPG_X_IN_MCU   (VLD_REG_OFST + (0x184 ))
#define RW_VLD_JPG_Y_IN_MCU   (VLD_REG_OFST + (0x188 ))
#define RW_VLD_JPG_IDCT_WIDTH01   (VLD_REG_OFST + (0x18C ))
#define RW_VLD_JPG_IDCT_WIDTH2_COEFF_WIDTH0   (VLD_REG_OFST + (0x190 ))
#define RW_VLD_JPG_COEFF_WIDTH12   (VLD_REG_OFST + (0x194 ))
#define RW_VLD_JPG_RMK_AND_FINISH   (VLD_REG_OFST + (0x198 ))
#define RW_VLD_JPG_CLR_ST_MRK   (VLD_REG_OFST + (0x19C ))
#define RO_VLD_JPG_RD_EOB_RUN   (VLD_REG_OFST + (0x1A0 ))
#define RW_VLD_DIGEST_BOUNDARY  (VLD_REG_OFST + (0x1A4 ))

#define RW_VLD_MP4_SIG1         (VLD_REG_OFST + (0x1C4 ))
#define RW_VLD_114              (VLD_REG_OFST + (0x1C8 ))
#define RW_VLD_115              (VLD_REG_OFST + (0x1CC ))
#define RW_VLD_116              (VLD_REG_OFST + (0x1D0 ))
#define RW_VLD_117              (VLD_REG_OFST + (0x1D4 ))
#define RW_VLD_118              (VLD_REG_OFST + (0x1D8 ))
#define RW_VLD_DX3              (VLD_REG_OFST + (0x1DC ))
#define RW_VLD_DECEND           (VLD_REG_OFST + (0x1E0 ))

// temporary for emulation ?
#define RW_VLD_DECSTART         (VLD_REG_OFST + (0x1E4 ))
// in real HW
#define RW_VLD_NONE_P           (VLD_REG_OFST + (0x1E8 ))


// HHKuo's
#define RW_VLD_DCACSA           (VLD_REG_OFST + (0x300 ))
#define RW_VLD_DCACPITCH        (VLD_REG_OFST + (0x304 ))
#define RW_VLD_MVSA             (VLD_REG_OFST + (0x308 ))
#define RW_VLD_DIRE_MD          (VLD_REG_OFST + (0x30C ))
#define RW_VLD_BMB1             (VLD_REG_OFST + (0x310 ))
#define RW_VLD_BMB2             (VLD_REG_OFST + (0x314 ))
#define RW_VLD_BCODE_SA         (VLD_REG_OFST + (0x318 ))
#define RW_VLD_DIRE_MD_IL       (VLD_REG_OFST + (0x31C ))

#define VDEC_ERR_MONITOR  (ERR_BL_PIX + ERR_CBP_NS + ERR_DCT_Y_TAB + \
                           ERR_DCT_C_TAB + ERR_SL_SYN + ERR_EOB_SYN + \
                           ERR_MB_ADD_INC_TAB + ERR_CBP_TAB + ERR_INS_SL + \
                           ERR_SL_QSCALE + ERR_SL_SYN2 + ERR_MB_ADD + \
                           ERR_MB_TBL + ERR_MOT_TP + ERR_MOT_CD + \
                           ERR_MKB + ERR_MCIDCT_BSY + ERR_OUT_PIC)

#define RW_VLD_SRAM_TEST_WRITE_ADDR  (VLD_REG_OFST + (0xCC))
#define RW_VLD_SRAM_TEST_WRITE_DATA  (VLD_REG_OFST + (0xD0))
#define RW_VLD_WMVDEC           (VLD_REG_OFST + (0x208))
#define WMV_DEC_START         (0x1)
#define WMV_BP_DEC_START      (0x1 << 8)
#define RW_VLD_WMV_MODE         (VLD_REG_OFST + (0x20C))
#define RW_VLD_WMV_PARA1        (VLD_REG_OFST + (0x210))
#define RW_VLD_WMV_PARA2        (VLD_REG_OFST + (0x214))
#define RW_VLD_WMV_OVC          (VLD_REG_OFST + (0x218))
#define RW_VLD_WMV_LOAD_SUM     (VLD_REG_OFST + (0x21C))
#define RW_VLD_WMV_PARA3        (VLD_REG_OFST + (0x220))
#define RW_VLD_WMV_BP           (VLD_REG_OFST + (0x224))
#define RW_VLD_BP_MODE_FIRE     (VLD_REG_OFST + (0x228))
#define RW_VLD_WMV_QM           (VLD_REG_OFST + (0x230))
#define RO_VLD_BP_DEC_END       (VLD_REG_OFST + (0x234))
#define BD_DEC_FINISH         (0x1 << 8)
#define RW_VLD_WMV_TAB          (VLD_REG_OFST + (0x238))
#define RW_VLD_WMV_ABS          (VLD_REG_OFST + (0x23C))
#define RO_VLD_PIC_DEC_END      (VLD_REG_OFST + (0x240))
#define PIC_DEC_FINISH        (0x1)
#define RW_VLD_WMV_PUD          (VLD_REG_OFST + (0x244))
#define RW_VLD_WMV_PBN          (VLD_REG_OFST + (0x248))

// 5365/5395 add start
#define RW_VLD_WMV_NEW_CTRL         (VLD_REG_OFST + (0x28C))
#define WMV_NEW_FLG_EN                                   (1 << 0)
#define WMV_PRED_MOD_EN                                  (1 << 1)
#define WMV_PRED_RD_SYNC_DISABLE                         (1 << 3)
#define WMV_DRAM_BURST_MODE_EN                           (1 << 4)
#define WMV_BP_USE_PRED_RD_EN                            (1 << 5)

#define RW_VLD_WMV_DCAC_NEW_ADDR    (VLD_REG_OFST + (0x290))
#define RW_VLD_WMV_BP0_NEW_ADDR     (VLD_REG_OFST + (0x29C))
#define RW_VLD_WMV_BP1_NEW_ADDR     (VLD_REG_OFST + (0x2A0))
#define RW_VLD_WMV_BP2_NEW_ADDR     (VLD_REG_OFST + (0x2A4))
#define RW_VLD_WMV_OW_PRED          (VLD_REG_OFST + (0x2A8))

#define RW_VLD_WMV_OW_BP            (VLD_REG_OFST + (0x2AC))
#define WMV_NUM_OW_BP_0                                  0
#define WMV_NUM_OW_BP_1                                  8
#define WMV_NUM_OW_BP_2                                 16

#define RW_VLD_SLICE_MARK_REG0      (VLD_REG_OFST + (0x274))
#define RW_VLD_SLICE_MARK_REG1      (VLD_REG_OFST + (0x278))
#define RW_VLD_SLICE_MARK_REG2      (VLD_REG_OFST + (0x27C))
// 5365/5395 add end

#define RW_VLD_BP1_ADDR         (VLD_REG_OFST + (0x318)) // the same as RW_VLD_BCODE_SA
#define RW_VLD_DCMVSEL          (VLD_REG_OFST + (0x320))
#define RW_VLD_MBDRAM_SEL       (VLD_REG_OFST + (0x324))

#define RW_VLD_BP2_ADDR         (VLD_REG_OFST + (0x32C))
#define RW_VLD_BP3_ADDR         (VLD_REG_OFST + (0x330))
#define RW_VLD_BP4_ADDR         (VLD_REG_OFST + (0x334))
#define RW_VLD_MVVOP_SEL        (VLD_REG_OFST + (0x338))
#define RW_VLD_MVF1             (VLD_REG_OFST + (0x33C))
#define RW_VLD_MVF2             (VLD_REG_OFST + (0x340))
#define RW_VLD_MVF3             (VLD_REG_OFST + (0x344))
#define RW_VLD_MVF4             (VLD_REG_OFST + (0x348))
#define RW_VLD_MV2_ADDR         (VLD_REG_OFST + (0x34C))
#define RW_VLD_VOP_TYPE         (VLD_REG_OFST + (0x350))
#define RW_VLD_DCAC2_ADDR       (VLD_REG_OFST + (0x354))
#define RW_VLD_MV3_ADDR         (VLD_REG_OFST + (0x358))


//#define VLD_REG_051    (VLD_REG_OFST + (0x00cc ))
//#define VLD_REG_052    (VLD_REG_OFST + (0x00d0 ))
//#define VLD_REG_130    (VLD_REG_OFST + (0x0208 ))
//#define VLD_REG_131    (VLD_REG_OFST + (0x020c ))
//#define VLD_REG_132    (VLD_REG_OFST + (0x0210 ))
//#define VLD_REG_133    (VLD_REG_OFST + (0x0214 ))
#define VLD_REG_134    (VLD_REG_OFST + (0x0218 ))
//#define VLD_REG_135    (VLD_REG_OFST + (0x021c ))
//#define VLD_REG_136    (VLD_REG_OFST + (0x0220 ))
//#define VLD_REG_137    (VLD_REG_OFST + (0x0224 ))
//#define VLD_REG_140    (VLD_REG_OFST + (0x0230 ))
//#define VLD_REG_141    (VLD_REG_OFST + (0x0234 ))
//#define VLD_REG_142    (VLD_REG_OFST + (0x0238 ))
//#define VLD_REG_143    (VLD_REG_OFST + (0x023c ))
//#define VLD_REG_144    (VLD_REG_OFST + (0x0240 ))
#define VLD_REG_145    (VLD_REG_OFST + (0x0244 ))
//#define VLD_REG_146    (VLD_REG_OFST + (0x0248 ))
//#define VLD_REG_200    (VLD_REG_OFST + (0x0320 ))
//#define VLD_REG_201    (VLD_REG_OFST + (0x0324 ))
#define VLD_REG_202    (VLD_REG_OFST + (0x0328 ))
//#define VLD_REG_203    (VLD_REG_OFST + (0x032c ))
//#define VLD_REG_204    (VLD_REG_OFST + (0x0330 ))
//#define VLD_REG_205    (VLD_REG_OFST + (0x0334 ))
//#define VLD_REG_206    (VLD_REG_OFST + (0x0338 ))
//#define VLD_REG_207    (VLD_REG_OFST + (0x033c ))
//#define VLD_REG_208    (VLD_REG_OFST + (0x0340 ))
//#define VLD_REG_209    (VLD_REG_OFST + (0x0344 ))
//#define VLD_REG_210    (VLD_REG_OFST + (0x0348 ))
//#define VLD_REG_211    (VLD_REG_OFST + (0x034c ))
//#define VLD_REG_212    (VLD_REG_OFST + (0x0350 ))
//#define VLD_REG_213    (VLD_REG_OFST + (0x0354 ))
//#define VLD_REG_214    (VLD_REG_OFST + (0x0358 ))

// **********************************************************************
// MC/IDCT Register
// **********************************************************************
#define MC_REG_OFST      (IO_VIRT+0xa000 )

#define RW_MC_R1Y        (MC_REG_OFST + (0x00 ))
#define RW_MC_R1C        (MC_REG_OFST + (0x04 ))
#define RW_MC_R2Y        (MC_REG_OFST + (0x08 ))
#define RW_MC_R2C        (MC_REG_OFST + (0x0C ))
#define RW_MC_BY         (MC_REG_OFST + (0x10 ))
#define RW_MC_BC         (MC_REG_OFST + (0x14 ))
#define RW_MC_FWDP       (MC_REG_OFST + (0x18 ))
#define RW_MC_DECFLD     (MC_REG_OFST + (0x1C ))
#define RW_MC_2FLDMD     (MC_REG_OFST + (0x20 ))
#define RW_MC_OPBUF  (MC_REG_OFST + (0x24 ))
#define RO_MC_MBX        (MC_REG_OFST + (0x28 ))
#define RO_MC_MBY        (MC_REG_OFST + (0x2C ))
#define RO_MC_BMBX       (MC_REG_OFST + (0x30 ))
#define RO_MC_BMBY       (MC_REG_OFST + (0x34 ))
#define RO_MC_MOTION   (MC_REG_OFST + (0x38 ))
#define RO_MC_MVVALID  (MC_REG_OFST + (0x3C ))
#define RO_MC_MV1X     (MC_REG_OFST + (0x40 ))
#define RO_MC_MV1Y     (MC_REG_OFST + (0x44 ))
#define RO_MC_MV2X     (MC_REG_OFST + (0x48 ))
#define RO_MC_MV2Y     (MC_REG_OFST + (0x4C ))
#define RO_MC_MV3X     (MC_REG_OFST + (0x50 ))
#define RO_MC_MV3Y     (MC_REG_OFST + (0x54 ))
#define RO_MC_MV4X     (MC_REG_OFST + (0x58 ))
#define RO_MC_MV4Y     (MC_REG_OFST + (0x5C ))
#define RO_MC_REFFLD     (MC_REG_OFST + (0x60 ))
#define RO_MC_MBINTRA    (MC_REG_OFST + (0x64 ))
#define RO_MC_FRMDCT     (MC_REG_OFST + (0x68 ))
#define RO_MC_CBP        (MC_REG_OFST + (0x6C ))
#define RW_MC_HREFP      (MC_REG_OFST + (0x70 ))
#define RO_MC_WMBDIG     (MC_REG_OFST + (0x74 ))
//#define RW_MC_DIGMBX     (MC_REG_OFST + (0x78 ))
//#define RW_MC_DIGMBY     (MC_REG_OFST + (0x7C ))
#define RW_MC_DIGY       (MC_REG_OFST + (0x80 ))
#define RW_MC_DIGC       (MC_REG_OFST + (0x84 ))
#define RW_MC_DIGWD      (MC_REG_OFST + (0x88 ))
#define RW_MC_YDCS       (MC_REG_OFST + (0x8C ))
#define RW_MC_ADDRSWAP   (MC_REG_OFST + (0x90 ))
#define RO_MC_STATUS     (MC_REG_OFST + (0x94 ))
#define RW_MC_DISSOLVE   (MC_REG_OFST + (0x98 ))

#define RW_MC_BY1              (MC_REG_OFST + (0x9C )) //first B pic buffer ptr Y
#define RW_MC_BC1              (MC_REG_OFST + (0xA0 )) //first B pic C
#define RW_MC_BY2              (MC_REG_OFST + (0xA4 )) //second B pic buffer ptr Y
#define RW_MC_BC2              (MC_REG_OFST + (0xA8 )) //second B pic C
#define RW_MC_PS_CMB11         (MC_REG_OFST + (0xAC ))//cmb1 intra 1xy 2xy
#define RW_MC_PS_CMB12         (MC_REG_OFST + (0xB0 ))//cmb1 intra 3xy 4xy
#define RW_MC_PS_CMB13         (MC_REG_OFST + (0xB4 ))//cmb1 intra 5xy 6xy
#define RW_MC_PS_CMB14         (MC_REG_OFST + (0xB8 ))//cmb1 intra 7xy 8xy
#define RW_MC_PS_CMB15         (MC_REG_OFST + (0xBC ))//cmb1 intra 9xy 10xy
#define RW_MC_PS_CMB16         (MC_REG_OFST + (0xC0 ))//cmb1 MVxy , IDCTxy
#define RW_MC_PS_CMB21         (MC_REG_OFST + (0xC4 ))//cmb2 intra 1xy 2xy
#define RW_MC_PS_CMB22         (MC_REG_OFST + (0xC8 ))//cmb2 intra 3xy 4xy
#define RW_MC_PS_CMB23         (MC_REG_OFST + (0xCC ))//cmb2 intra 5xy 6xy
#define RW_MC_PS_CMB24         (MC_REG_OFST + (0xD0 ))//cmb2 intra 7xy 8xy
#define RW_MC_PS_CMB25         (MC_REG_OFST + (0xD4 ))//cmb2 intra 9xy 10xy
#define RW_MC_PS_CMB26         (MC_REG_OFST + (0xD8 ))//cmb2 MVxy , IDCTxy
#define RW_MC_PS_CMB_INTRA_THD (MC_REG_OFST + (0xDC ))//CMB intra dc threshold
#define RW_MC_PS_CMB_MV_MAX    (MC_REG_OFST + (0xE0 ))//CMB maximun MV
#define  PS_CMB_MV1_VALID        ((unsigned)(0x1) << 30)
#define  PS_CMB_MV2_VALID        ((unsigned)(0x1) << 29)
#define RW_MC_PS_MP_BOUND      (MC_REG_OFST + (0xE4 ))//motion plane IDCT diff curve upper bound
#define RW_MC_PS_MP_DIFF       (MC_REG_OFST + (0xE8 ))//motion plane IDCT diffrence curve level
#define RW_MC_PS_MP_DIFF1      (MC_REG_OFST + (0xEC ))//motion plane IDCT diffrence curve level
#define RW_MC_PS_MP_IDCT_CNT   (MC_REG_OFST + (0xF0 ))//motion plane IDCT count threshold
#define RW_MC_PS_CMB_IDCT_MAX  (MC_REG_OFST + (0xF4 ))//CMB maximun IDCT abs sum
#define  PS_CMB_IDCT1_VALID      ((unsigned)(0x1) << 18)
#define  PS_CMB_IDCT2_VALID      ((unsigned)(0x1) << 17)
#define RW_MC_PS_CMB_CTRL      (MC_REG_OFST + (0xF8 ))//CMB control register
#define  PS_CMB1_CLR             ((unsigned)(0x1) << 7)//clear cmb group1
#define  PS_CMB2_CLR             ((unsigned)(0x1) << 6)//clear cmb group2
#define  PS_CMB_SEL2             ((unsigned)(0x1) << 5)// 1:select 2 ,0:select cmb group 1
#define  PS_SRC_DETECT           ((unsigned)(0x1) << 4)//film detect on
#define  PS_MAD_ON               ((unsigned)(0x1) << 3)//motion plane on
#define  PS_CMB_INTRA            ((unsigned)(0x1) << 2)//cmb select intra on
#define  PS_CMB_IDCT             ((unsigned)(0x1) << 1)//cmb select idct on
#define  PS_CMB_MV               ((unsigned)(0x1) << 0)//cmb select mv on

#define RW_MC_PS_MP_READ       (MC_REG_OFST + (0xFC ))//motion plane read ptr
#define RW_MC_PS_MP_WRITE      (MC_REG_OFST + (0x100 ))//motion plane write ptr
#define RW_MC_PS_DINTRA1       (MC_REG_OFST + (0x104 ))//Dinra 1-12
#define RW_MC_PS_DINTRA2       (MC_REG_OFST + (0x108 ))
#define RW_MC_PS_DINTRA3       (MC_REG_OFST + (0x10C ))
#define RW_MC_PS_DINTRA4       (MC_REG_OFST + (0x110 ))
#define RW_MC_PS_DINTRA5       (MC_REG_OFST + (0x114 ))
#define RW_MC_PS_DINTRA6       (MC_REG_OFST + (0x118 ))
#define RW_MC_PS_DINTRA7       (MC_REG_OFST + (0x11C ))
#define RW_MC_PS_DINTRA8       (MC_REG_OFST + (0x120 ))
#define RW_MC_PS_DINTRA9       (MC_REG_OFST + (0x124 ))
#define RW_MC_PS_DINTRA10      (MC_REG_OFST + (0x128 ))
#define RW_MC_PS_DINTRA11      (MC_REG_OFST + (0x12C ))
#define RW_MC_PS_DINTRA12      (MC_REG_OFST + (0x130 ))
#define RW_MC_PS_DINTER1       (MC_REG_OFST + (0x134 ))//Dinter 1-12
#define RW_MC_PS_DINTER2       (MC_REG_OFST + (0x138 ))
#define RW_MC_PS_DINTER3       (MC_REG_OFST + (0x13C ))
#define RW_MC_PS_DINTER4       (MC_REG_OFST + (0x140 ))
#define RW_MC_PS_DINTER5       (MC_REG_OFST + (0x144 ))
#define RW_MC_PS_DINTER6       (MC_REG_OFST + (0x148 ))
#define RW_MC_PS_DINTER7       (MC_REG_OFST + (0x14C ))
#define RW_MC_PS_DINTER8       (MC_REG_OFST + (0x150 ))
#define RW_MC_PS_DINTER9       (MC_REG_OFST + (0x154 ))
#define RW_MC_PS_DINTER10      (MC_REG_OFST + (0x158 ))
#define RW_MC_PS_DINTER11      (MC_REG_OFST + (0x15C ))
#define RW_MC_PS_DINTER12      (MC_REG_OFST + (0x160 ))
#define RW_MC_PS_DT1           (MC_REG_OFST + (0x164 ))//Dt 1-12
#define RW_MC_PS_DT2           (MC_REG_OFST + (0x168 ))
#define RW_MC_PS_DT3           (MC_REG_OFST + (0x16C ))
#define RW_MC_PS_DT4           (MC_REG_OFST + (0x170 ))
#define RW_MC_PS_DT5           (MC_REG_OFST + (0x174 ))
#define RW_MC_PS_DT6           (MC_REG_OFST + (0x178 ))
#define RW_MC_PS_DT7           (MC_REG_OFST + (0x17C ))
#define RW_MC_PS_DT8           (MC_REG_OFST + (0x180 ))
#define RW_MC_PS_DT9           (MC_REG_OFST + (0x184 ))
#define RW_MC_PS_DT10          (MC_REG_OFST + (0x188 ))
#define RW_MC_PS_DT11          (MC_REG_OFST + (0x18C ))
#define RW_MC_PS_DT12          (MC_REG_OFST + (0x190 ))
#define RW_MC_PS_DB1           (MC_REG_OFST + (0x194 ))//Db 1-12
#define RW_MC_PS_DB2           (MC_REG_OFST + (0x198 ))
#define RW_MC_PS_DB3           (MC_REG_OFST + (0x19C ))
#define RW_MC_PS_DB4           (MC_REG_OFST + (0x1A0 ))
#define RW_MC_PS_DB5           (MC_REG_OFST + (0x1A4 ))
#define RW_MC_PS_DB6           (MC_REG_OFST + (0x1A8 ))
#define RW_MC_PS_DB7           (MC_REG_OFST + (0x1AC ))
#define RW_MC_PS_DB8           (MC_REG_OFST + (0x1B0 ))
#define RW_MC_PS_DB9           (MC_REG_OFST + (0x1B4 ))
#define RW_MC_PS_DB10          (MC_REG_OFST + (0x1B8 ))
#define RW_MC_PS_DB11          (MC_REG_OFST + (0x1BC ))
#define RW_MC_PS_DB12          (MC_REG_OFST + (0x1C0 ))
#define RW_MC_PS_DIFF_CTRL     (MC_REG_OFST + (0x1C4 ))//D calculation control
#define  PS_BUF_ID               ((unsigned)(0x1) << 27)//current pic buffer id
#define  PS_LAST_BUF_ID          ((unsigned)(0x1) << 25)//last pic buffer id
#define  PS_MP_CNT_CLR           ((unsigned)(0x1) << 3)//clear motion table count
#define  PS_DINTERA_ON           ((unsigned)(0x1) << 2)//enable Dinter, Dintra
#define  PS_DTB_ON               ((unsigned)(0x1) << 1)//enable Dt, Db
#define  PS_D_START              ((unsigned)(0x1) << 0)//start to calculate D values
#define RW_MC_PS_DRAM_MODE     (MC_REG_OFST + (0x1C8 ))//read/write Dram address mode


#if 0
//OLD definitions for MPEG-4 emulation code (0x220 ~ 0x240)
#define RW_MC_QPEL_TYPE        (MC_REG_OFST + (0x220 ))
#define TYPE_14496            0
#define TYPE_MOMUSYS          1
#define RW_MC_QUARTER_SAMPLE   (MC_REG_OFST + (0x224 ))
#define RW_MC_ROUNDING_CTRL    (MC_REG_OFST + (0x228 ))
#define RW_MC_UMV_PIC_WIDTH    (MC_REG_OFST + (0x22C ))
#define RW_MC_UMV_PIC_HEIGHT   (MC_REG_OFST + (0x230 ))
#define RW_MC_WARP_POINT_NUM   (MC_REG_OFST + (0x234 ))
#define RW_MC_GMC_Y_MV         (MC_REG_OFST + (0x238 ))
#define RW_MC_GMC_C_MV         (MC_REG_OFST + (0x23C ))
#define RW_MC_CBCR_MV_TYPE     (MC_REG_OFST + (0x240 ))
#define QPEL_4MV_CMV_14496     (0x0)
#define QPEL_4MV_CMV_DIVX503   (0x1)
#define QPEL_FIELD_CMV_14496   (0x0 << 8)
#define QPEL_FIELD_CMV_DIVX503 (0x1 << 8)
#define QPEL_FRAME_CMV_14496   (0x0 << 16)
#define QPEL_FRAME_CMV_DIVX503 (0x1 << 16)
#define QPEL_FRAME_CMV_DIVXOLD (0x2 << 16)
#else //porting from real firmware 
#define RW_MC_QPEL_TYPE        (MC_REG_OFST + (0x1FC ))
#define TYPE_14496            0
#define TYPE_MOMUSYS          1
#define RW_MC_QUARTER_SAMPLE   (MC_REG_OFST + (0x200 ))
#define RW_MC_ROUNDING_CTRL    (MC_REG_OFST + (0x204 ))
#define RW_MC_UMV_PIC_WIDTH    (MC_REG_OFST + (0x208 ))
#define RW_MC_UMV_PIC_HEIGHT   (MC_REG_OFST + (0x20C ))
#define RW_MC_WARP_POINT_NUM   (MC_REG_OFST + (0x210 ))
#define RW_MC_GMC_Y_MV         (MC_REG_OFST + (0x214 ))
#define RW_MC_GMC_C_MV         (MC_REG_OFST + (0x218 ))
#define RW_MC_CBCR_MV_TYPE     (MC_REG_OFST + (0x21C ))
#define QPEL_4MV_CMV_14496     (0x0)
#define QPEL_4MV_CMV_DIVX503   (0x1)
#define QPEL_FIELD_CMV_14496   (0x0 << 8)
#define QPEL_FIELD_CMV_DIVX503 (0x1 << 8)
#define QPEL_FRAME_CMV_14496   (0x0 << 16)
#define QPEL_FRAME_CMV_DIVX503 (0x1 << 16)
#define QPEL_FRAME_CMV_DIVXOLD (0x2 << 16)

//WMV mapping 0x220 ~ 0x240
// post-processing registers
#define RW_MC_PP_ENABLE        (MC_REG_OFST + (0x220 ))
#define RW_MC_PP_Y_ADDR        (MC_REG_OFST + (0x224 ))
#define RW_MC_PP_C_ADDR        (MC_REG_OFST + (0x228 ))
#define RW_MC_PP_MB_WIDTH      (MC_REG_OFST + (0x22C ))
#define RW_MC_BREF             (MC_REG_OFST + (0x230 ))
#define NML_BREF_I_VOP        (0x1 << 5)
#define RW_MC_PP_QP_TYPE       (MC_REG_OFST + (0x234 ))
#define SEL_GIVEN_QP           (0x0 << 8)
#define SEL_USER_QP            (0x1 << 8)
#define SEL_LUT_QP             (0x2 << 8)
#define RW_MC_PP_DBLK_MODE     (MC_REG_OFST + (0x238 ))
#define DBLK_14496             (0x0 << 16)
#define DBLK_DIVX              (0x1 << 16)
#define DBLK_V_THEN_H          (0x0 << 8)
#define DBLK_H_THEN_V          (0x1 << 8)
#define DBLK_Y                 (0x1 << 1)
#define DBLK_C                 (0x1)
#define RW_MC_PP_DBLK_THD      (MC_REG_OFST + (0x23C ))
#define RW_MC_PP_DBLK_DIR      (MC_REG_OFST + (0x240 ))
#define DBLK_H_ONLY            (0x1 << 8)
#define DBLK_V_ONLY            (0x1)
#define RW_MC_PP_DRING_MODE    (MC_REG_OFST + (0x244 ))
#define COMB_WITH_DBLK         (0x1 << 16)
#define MAX_DIFF_MOMUSYS       (0x0 << 8)
#define MAX_DIFF_14496         (0x1 << 8)
#define DRING_Y                (0x1 << 1)
#define DRING_C                (0x1)
#define RW_MC_PP_DRING_THD     (MC_REG_OFST + (0x248 ))
#define RW_MC_PP_INTERLACE     (MC_REG_OFST + (0x24C ))
#define INTERLACE_DRING        (0x1 << 8)
#define INTERLACE_DBLK         (0x1)
#define RW_MC_PP_WB_BY_POST    (MC_REG_OFST + (0x250 ))
#define RW_MC_PP_LOW_BW        (MC_REG_OFST + (0x254 ))
#define RW_MC_PP_SEQ           (MC_REG_OFST + (0x258 ))
#define DBLK_THEN_DRING        (0x0)
#define DRING_THEN_DBLK        (0x1)
#define RW_MC_PP_X_RANGE       (MC_REG_OFST + (0x260 ))
#define RW_MC_PP_Y_RANGE       (MC_REG_OFST + (0x264 ))
#endif


#define RW_MC_WMV9_PRE_PROC    (MC_REG_OFST + (0x270))
#define NO_SCALE             (0x0)
#define SCALE_DOWN           (0x1)
#define SCALE_UP             (0x1 << 1)
#define RW_MC_WMV9_ICOMP_EN    (MC_REG_OFST + (0x274))
#define ICOMP_C_OFF          (0x0)
#define ICOMP_C_EN           (0x1)
#define ICOMP_Y_OFF          (0x0 << 1)
#define ICOMP_Y_EN           (0x1 << 1)
#define RW_MC_ISCALE1_X1       (MC_REG_OFST + (0x278))
#define RW_MC_ISCALE1_X3       (MC_REG_OFST + (0x27C))
#define RW_MC_ISCALE1_X5       (MC_REG_OFST + (0x280))
#define RW_MC_ISCALE1_X7       (MC_REG_OFST + (0x284))
#define RW_MC_ISCALE1_X9       (MC_REG_OFST + (0x288))
#define RW_MC_ISCALE1_X11      (MC_REG_OFST + (0x28C))
#define RW_MC_ISCALE1_X13      (MC_REG_OFST + (0x290))
#define RW_MC_ISCALE1_X15      (MC_REG_OFST + (0x294))
#define RW_MC_YSHIFT_OFF1      (MC_REG_OFST + (0x298))
#define RW_MC_CSHIFT_OFF1      (MC_REG_OFST + (0x29C))
#define RW_MC_FILTER_TYPE      (MC_REG_OFST + (0x2A0))
#define C_BILINEAR           (0x0)
#define C_BICUBIC            (0x1)
#define Y_BILINEAR           (0x0 << 1)
#define Y_BICUBIC            (0x1 << 1)
#define RW_MC_WRITE_BUS_TYPE   (MC_REG_OFST + (0x2A4))
#define CLIP_0_255_TYPE      0
#define UNCLIP_TYPE          1
#define RW_MC_INTRA_BLK_ADD128 (MC_REG_OFST + (0x2a8))
#define ADD128_OFF           0
#define ADD128_ON            1
#define RW_MC_FAST_UVMC        (MC_REG_OFST + (0x2B4))
#define FAST_UVMC_EN         (0x1)
#define RW_MC_OVL_SMTH_FILTER  (MC_REG_OFST + (0x2BC))
#define OVL_OFF              (0x0)
#define OVL_EN               (0x1)
#define RW_MC_COND_OVL_FILTER  (MC_REG_OFST + (0x2C0))
#define COND_OVL_OFF         (0x0)
#define COND_OVL_EN          (0x1)
#define RW_MC_PP_DBLK_OPT      (MC_REG_OFST + (0x2C4))
#define CHK_INTRA0           (0x1)
#define CHK_BLK3             (0x1 << 8)
#define DBLK_FLG_4x4         (0x1 << 16)
#define VC1_ALL_SET_ONE      (CHK_INTRA0 + CHK_BLK3 + DBLK_FLG_4x4)
#define NOT_VC1_ALL_SET_ZERO (0)
#define RW_MC_PP_DBLK_Y_ADDR   (MC_REG_OFST + (0x2C8))
#define RW_MC_PP_DBLK_C_ADDR   (MC_REG_OFST + (0x2CC))
#define RW_MC_WMV8_MIX_PEL     (MC_REG_OFST + (0x2D0))
#define MIX_PEL_MC_EN        (0x0)
#define RW_MC_RNG_PARA         (MC_REG_OFST + (0x2E4)) //This register is not used in current hardware any more
#define RW_MC_ISCALE2_X1       (MC_REG_OFST + (0x2EC))
#define RW_MC_ISCALE2_X3       (MC_REG_OFST + (0x2F0))
#define RW_MC_ISCALE2_X5       (MC_REG_OFST + (0x2F4))
#define RW_MC_ISCALE2_X7       (MC_REG_OFST + (0x2F8))
#define RW_MC_ISCALE2_X9       (MC_REG_OFST + (0x2FC))
#define RW_MC_ISCALE2_X11      (MC_REG_OFST + (0x300))
#define RW_MC_ISCALE2_X13      (MC_REG_OFST + (0x304))
#define RW_MC_ISCALE2_X15      (MC_REG_OFST + (0x308))
#define RW_MC_YSHIFT_OFF2      (MC_REG_OFST + (0x30C))
#define RW_MC_CSHIFT_OFF2      (MC_REG_OFST + (0x310))
#define RW_MC_ICOMP_TYPE       (MC_REG_OFST + (0x314))
#define NO_ICOMP             0
#define FRAME_ICOMP          1
#define TOP_FLD_ICOMP        2
#define BTM_FLD_ICOMP        3
#define BOTH_FLD_ICOMP       4
#define RW_MC_ISCALE3_X1       (MC_REG_OFST + (0x318))
#define RW_MC_ISCALE3_X3       (MC_REG_OFST + (0x31C))
#define RW_MC_ISCALE3_X5       (MC_REG_OFST + (0x320))
#define RW_MC_ISCALE3_X7       (MC_REG_OFST + (0x324))
#define RW_MC_ISCALE3_X9       (MC_REG_OFST + (0x328))
#define RW_MC_ISCALE3_X11      (MC_REG_OFST + (0x32C))
#define RW_MC_ISCALE3_X13      (MC_REG_OFST + (0x330))
#define RW_MC_ISCALE3_X15      (MC_REG_OFST + (0x334))
#define RW_MC_YSHIFT_OFF3      (MC_REG_OFST + (0x338))
#define RW_MC_CSHIFT_OFF3      (MC_REG_OFST + (0x33C))
#define RW_MC_ICOMP2_EN        (MC_REG_OFST + (0x340))
#define RW_MC_ISCALE4_X1       (MC_REG_OFST + (0x344))
#define RW_MC_ISCALE4_X3       (MC_REG_OFST + (0x348))
#define RW_MC_ISCALE4_X5       (MC_REG_OFST + (0x34C))
#define RW_MC_ISCALE4_X7       (MC_REG_OFST + (0x350))
#define RW_MC_ISCALE4_X9       (MC_REG_OFST + (0x354))
#define RW_MC_ISCALE4_X11      (MC_REG_OFST + (0x358))
#define RW_MC_ISCALE4_X13      (MC_REG_OFST + (0x35C))
#define RW_MC_ISCALE4_X15      (MC_REG_OFST + (0x360))
#define RW_MC_YSHIFT_OFF4      (MC_REG_OFST + (0x364))
#define RW_MC_CSHIFT_OFF4      (MC_REG_OFST + (0x368))
#define RW_MC_ISCALE5_X1       (MC_REG_OFST + (0x36C))
#define RW_MC_ISCALE5_X3       (MC_REG_OFST + (0x370))
#define RW_MC_ISCALE5_X5       (MC_REG_OFST + (0x374))
#define RW_MC_ISCALE5_X7       (MC_REG_OFST + (0x378))
#define RW_MC_ISCALE5_X9       (MC_REG_OFST + (0x37C))
#define RW_MC_ISCALE5_X11      (MC_REG_OFST + (0x380))
#define RW_MC_ISCALE5_X13      (MC_REG_OFST + (0x384))
#define RW_MC_ISCALE5_X15      (MC_REG_OFST + (0x388))
#define RW_MC_YSHIFT_OFF5      (MC_REG_OFST + (0x38C))
#define RW_MC_CSHIFT_OFF5      (MC_REG_OFST + (0x390))
#define RW_MC_BWD_ICOMP_FLD    (MC_REG_OFST + (0x394))
#define BWD_TOP_FLD          (0x0)
#define BWD_BTM_FLD          (0x1)
#define RW_MC_SAME_ICOMP       (MC_REG_OFST + (0x398))
#define NO_USE_SAME_ICOPM1_FOR_FRAME    (0x0)
#define USE_SAME_ICOMP1_FOR_FRAME       (0x1)
#define RW_MC_REF_PIC_TYPE     (MC_REG_OFST + (0x39C))
#define BWD_PROG_REF_PIC     (0x0)
#define BWD_INTLCE_REF_PIC   (0x1)
#define FWD_PROG_REF_PIC     (0x0 << 16)
#define FWD_INTLCE_REF_PIC   (0x1 << 16)
#define RW_MC_ISCALE6_X1       (MC_REG_OFST + (0x3A0))
#define RW_MC_ISCALE6_X3       (MC_REG_OFST + (0x3A4))
#define RW_MC_ISCALE6_X5       (MC_REG_OFST + (0x3A8))
#define RW_MC_ISCALE6_X7       (MC_REG_OFST + (0x3AC))
#define RW_MC_ISCALE6_X9       (MC_REG_OFST + (0x3B0))
#define RW_MC_ISCALE6_X11      (MC_REG_OFST + (0x3B4))
#define RW_MC_ISCALE6_X13      (MC_REG_OFST + (0x3B8))
#define RW_MC_ISCALE6_X15      (MC_REG_OFST + (0x3BC))
#define RW_MC_YSHIFT_OFF6      (MC_REG_OFST + (0x3C0))
#define RW_MC_CSHIFT_OFF6      (MC_REG_OFST + (0x3C4))
#define RW_MC_SAME_REF_PIC     (MC_REG_OFST + (0x3CC))
#define RO_MC_QIU_STATUS       (MC_REG_OFST + (0x3D4))
#define RO_MC_HIU_STATUS       (MC_REG_OFST + (0x3D8))

//NEW registers for WMV
//#define MC_REG_051    (MC_REG_OFST + (0x00cc ))
//#if0 #define MC_REG_127    (MC_REG_OFST + (0x01fc ))
//#if0 #define MC_REG_128    (MC_REG_OFST + (0x0200 ))
//#if0 #define MC_REG_129    (MC_REG_OFST + (0x0204 ))
//#if0 #define MC_REG_130    (MC_REG_OFST + (0x0208 ))
//#if0 #define MC_REG_131    (MC_REG_OFST + (0x020c ))
//#if0 #define MC_REG_143    (MC_REG_OFST + (0x023c ))
#define MC_REG_145    (MC_REG_OFST + (0x0244 )) //differ from RW_MC__PP_DRING_MODE ???
//#if0 #define MC_REG_148    (MC_REG_OFST + (0x0250 ))
//#if0 #define MC_REG_152    (MC_REG_OFST + (0x0260 ))
//#if0 #define MC_REG_153    (MC_REG_OFST + (0x0264 ))
//#define MC_REG_156    (MC_REG_OFST + (0x0270 ))
//#define MC_REG_157    (MC_REG_OFST + (0x0274 ))
//#define MC_REG_158    (MC_REG_OFST + (0x0278 ))
//#define MC_REG_159    (MC_REG_OFST + (0x027c ))
//#define MC_REG_160    (MC_REG_OFST + (0x0280 ))
//#define MC_REG_161    (MC_REG_OFST + (0x0284 ))
//#define MC_REG_162    (MC_REG_OFST + (0x0288 ))
//#define MC_REG_163    (MC_REG_OFST + (0x028c ))
//#define MC_REG_164    (MC_REG_OFST + (0x0290 ))
//#define MC_REG_165    (MC_REG_OFST + (0x0294 ))
//#define MC_REG_166    (MC_REG_OFST + (0x0298 ))
//#define MC_REG_167    (MC_REG_OFST + (0x029c ))
//#define MC_REG_168    (MC_REG_OFST + (0x02a0 ))
//#define MC_REG_169    (MC_REG_OFST + (0x02a4 ))
//#define MC_REG_170    (MC_REG_OFST + (0x02a8 ))
//#define MC_REG_173    (MC_REG_OFST + (0x02b4 ))
//#define MC_REG_175    (MC_REG_OFST + (0x02bc ))
//#define MC_REG_176    (MC_REG_OFST + (0x02c0 ))
//#define MC_REG_177    (MC_REG_OFST + (0x02c4 ))
//#define MC_REG_178    (MC_REG_OFST + (0x02c8 ))
//#define MC_REG_179    (MC_REG_OFST + (0x02cc ))
//#define MC_REG_180    (MC_REG_OFST + (0x02d0 ))
//#define MC_REG_185    (MC_REG_OFST + (0x02e4 ))
//#define MC_REG_187    (MC_REG_OFST + (0x02ec ))
//#define MC_REG_188    (MC_REG_OFST + (0x02f0 ))
//#define MC_REG_189    (MC_REG_OFST + (0x02f4 ))
//#define MC_REG_190    (MC_REG_OFST + (0x02f8 ))
//#define MC_REG_191    (MC_REG_OFST + (0x02fc ))
//#define MC_REG_192    (MC_REG_OFST + (0x0300 ))
//#define MC_REG_193    (MC_REG_OFST + (0x0304 ))
//#define MC_REG_194    (MC_REG_OFST + (0x0308 ))
//#define MC_REG_195    (MC_REG_OFST + (0x030c ))
//#define MC_REG_196    (MC_REG_OFST + (0x0310 ))
//#define MC_REG_197    (MC_REG_OFST + (0x0314 ))
//#define MC_REG_198    (MC_REG_OFST + (0x0318 ))
//#define MC_REG_199    (MC_REG_OFST + (0x031c ))
//#define MC_REG_200    (MC_REG_OFST + (0x0320 ))
//#define MC_REG_201    (MC_REG_OFST + (0x0324 ))
//#define MC_REG_202    (MC_REG_OFST + (0x0328 ))
//#define MC_REG_203    (MC_REG_OFST + (0x032c ))
//#define MC_REG_204    (MC_REG_OFST + (0x0330 ))
//#define MC_REG_205    (MC_REG_OFST + (0x0334 ))
//#define MC_REG_206    (MC_REG_OFST + (0x0338 ))
//#define MC_REG_207    (MC_REG_OFST + (0x033c ))
//#define MC_REG_208    (MC_REG_OFST + (0x0340 ))
//#define MC_REG_209    (MC_REG_OFST + (0x0344 ))
//#define MC_REG_210    (MC_REG_OFST + (0x0348 ))
//#define MC_REG_211    (MC_REG_OFST + (0x034c ))
//#define MC_REG_212    (MC_REG_OFST + (0x0350 ))
//#define MC_REG_213    (MC_REG_OFST + (0x0354 ))
//#define MC_REG_214    (MC_REG_OFST + (0x0358 ))
//#define MC_REG_215    (MC_REG_OFST + (0x035c ))
//#define MC_REG_216    (MC_REG_OFST + (0x0360 ))
//#define MC_REG_217    (MC_REG_OFST + (0x0364 ))
//#define MC_REG_218    (MC_REG_OFST + (0x0368 ))
//#define MC_REG_219    (MC_REG_OFST + (0x036c ))
//#define MC_REG_220    (MC_REG_OFST + (0x0370 ))
//#define MC_REG_221    (MC_REG_OFST + (0x0374 ))
//#define MC_REG_222    (MC_REG_OFST + (0x0378 ))
//#define MC_REG_223    (MC_REG_OFST + (0x037c ))
//#define MC_REG_224    (MC_REG_OFST + (0x0380 ))
//#define MC_REG_225    (MC_REG_OFST + (0x0384 ))
//#define MC_REG_226    (MC_REG_OFST + (0x0388 ))
//#define MC_REG_227    (MC_REG_OFST + (0x038c ))
//#define MC_REG_228    (MC_REG_OFST + (0x0390 ))
//#define MC_REG_229    (MC_REG_OFST + (0x0394 ))
//#define MC_REG_230    (MC_REG_OFST + (0x0398 ))
//#define MC_REG_231    (MC_REG_OFST + (0x039C ))
//#define MC_REG_232    (MC_REG_OFST + (0x03a0 ))
//#define MC_REG_233    (MC_REG_OFST + (0x03a4 ))
//#define MC_REG_234    (MC_REG_OFST + (0x03a8 ))
//#define MC_REG_235    (MC_REG_OFST + (0x03ac ))
//#define MC_REG_236    (MC_REG_OFST + (0x03b0 ))
//#define MC_REG_237    (MC_REG_OFST + (0x03b4 ))
//#define MC_REG_238    (MC_REG_OFST + (0x03b8 ))
//#define MC_REG_239    (MC_REG_OFST + (0x03bc ))
//#define MC_REG_240    (MC_REG_OFST + (0x03c0 ))
//#define MC_REG_241    (MC_REG_OFST + (0x03c4 ))
//#define MC_REG_243    (MC_REG_OFST + (0x03cc ))

#define MC_REG_325             (MC_REG_OFST + 0x514)

// 5365/5395 add start
#define RW_MC_WRAPPER_SW       (MC_REG_OFST + 0x57C)
#define RW_MC_NBM_ADDRSWAP     (MC_REG_OFST + 0x768)
// 5365/5395 add end

// 5365/5395 add start
// **********************************************************************
// MV Register
// **********************************************************************
#define WMV_MV_REG_OFST        (IO_VIRT + 0x31000)

#define RW_WMV_MV_NEW_ADDR        (WMV_MV_REG_OFST + 0x20C)
#define RW_WMV_BWDREF_PIC_TYPE    (WMV_MV_REG_OFST + 0x218)

#define RW_WMV_MV_DIRECT_SCL_FAC  (WMV_MV_REG_OFST + 0x21C)
#define WMV_IRAT_0                                        0
#define WMV_IRAT_1                                        16

#define RW_WMV_MV_CFG             (WMV_MV_REG_OFST + 0x224)
#define WMV_FIRST_FIELD                                   28
#define WMV_BWDREF_DIST                                   24
#define WMV_FWDREF_DIST                                   20
#define WMV_USE_OPP_REF_FIELD                             16
#define WMV_TWO_REF_PIC                                   12
#define WMV_POST_RESCENT                                   9
#define WMV_FAST_UV                                        8
#define WMV_MV_RNG_IDX                                     4
#define WMV_QPIXEL                                         0

#define RW_WMV_MV_MAX_1           (WMV_MV_REG_OFST + 0x228)
#define RW_WMV_MV_MAX_2           (WMV_MV_REG_OFST + 0x22C)

#define RW_WMV_MV_180             (WMV_MV_REG_OFST + 0x2D0)

// **********************************************************************
// DCAC Register
// **********************************************************************
#define WMV_DCAC_REG_OFST      (IO_VIRT + 0x2f000 + 0x700)

#define RW_WMV_DCAC_RULEB      (WMV_DCAC_REG_OFST + 0x0)
#define RW_WMV_DCAC_PRED       (WMV_DCAC_REG_OFST + 0x4)

#define CRC_REG_OFST           (IO_VIRT + 0x03000)

#define RW_CRC_CTRL            (CRC_REG_OFST + 0x04)
#define RW_CRC_Y_0             (CRC_REG_OFST + 0x08)
#define RW_CRC_Y_1             (CRC_REG_OFST + 0x0C)
#define RW_CRC_Y_2             (CRC_REG_OFST + 0x10)
#define RW_CRC_Y_3             (CRC_REG_OFST + 0x14)
#define RW_CRC_C_0             (CRC_REG_OFST + 0x18)
#define RW_CRC_C_1             (CRC_REG_OFST + 0x1C)
#define RW_CRC_C_2             (CRC_REG_OFST + 0x20)
#define RW_CRC_C_3             (CRC_REG_OFST + 0x24)
// 5365/5395 add end


/*******************************************/
// *** DISPLAY REGISTERS START ***
/*******************************************/
#define VDO_REG_OFST      (0x0C00 )
#define RW_VDO_PTRY       (VDO_REG_OFST + 0x00)
#define RW_VDO_PTRC       (VDO_REG_OFST + (0x04 ))
#define RW_VDO_PTRY2      (VDO_REG_OFST + (0x08 ))//new
#define RW_VDO_PTRC2      (VDO_REG_OFST + (0x0c ))//new
//#define RW_VDO_PFLINE   (VDO_REG_OFST + (0x08 ))
#define RW_VDO_PICSIZE    (VDO_REG_OFST + (0x10 ))
//#define RW_VDO_SKIP     (VDO_REG_OFST + (0x10 ))
//***notice*****
// in 1379 B version chip, bit 19 can not be read, so don't use
//   "ReadReg() then Write Back" in pxlskip
//**************
#define RW_VDO_SCALE      (VDO_REG_OFST + (0x14 ))
#define RW_VDO_STAMBL     (VDO_REG_OFST + (0x18 ))
#define RW_VDO_FILTER     (VDO_REG_OFST + (0x1C ))//VDO_MODE
#define VDO_ENABLE      (0x1 << 0)
#define RACE          (0x1 << 1) //new
#define Y_AVG             (0x1 << 2) //new
#define C_AVG             (0x1 << 3) //new
#define HALF_Y            (0x1 << 4)
#define  HALF_C           (0x1 << 5)
#define YLR               (0x1 << 6)
#define CLR               (0x1 << 7)
#define VDO_MPEG1         (0x1 << 8)
#define VDO_CDG           (0x1 << 9)//new
#define VDO_ADDR_SWAP     (0x1 << 10)
#define X_HALF            (0x1 << 11)//new
//#define  PRGS_OUT     0 //no flag in 1379
//#define  GAMMA_ENABLE   0 //no flag in 1379
//#define  EDOFF          (0)//no flag
//#define  CEDOFF         (0)//no flag
#define TWO_FLD_FILTER    (0x1 << 16)//new
#define MAD               (0x1 << 17)//new
#define STILL             (0x1 << 18)
#define MOTION            (0x1 << 19)//new

#define DOWN_FLT_EN       (0x1 << 21)
#define CUBIC_EN          (0x1 << 22)
#define DELAY_BACKUP      (0x1 << 23)

#ifdef MT1379_E3
#define FORCE_COMB        (0x1 << 30)//1379 E3
#else
#define FORCE_COMB        (0)
#endif
#define RW_VDO_SL       (VDO_REG_OFST + (0x20 ))//Y
#define RW_VDO_SLC      (VDO_REG_OFST + (0x24 ))//STALINEC
#define RW_VDO_SSL      (VDO_REG_OFST + (0x28 ))//Y
#define RW_VDO_SSLC     (VDO_REG_OFST + (0x2C ))//C

#define RW_VDO_CTRL     (VDO_REG_OFST + (0x30 ))
#define  FRM_Y            0x1
#define  FRM_C            (0x1 << 1)
#define  PIC_FIELD        (0x1 << 2)
#define  B_PIC            (0x1 << 3)
#define  FLD_B_PIC        (0x1 << 4)
#define  ALT_TV_FLD       (0x1 << 5)
#define  USE_ALT_TV_FLD   (0x1 << 6)
#define  B_PIC2           (0x1 << 7)


#define RW_VDO_VACANCY  (VDO_REG_OFST + (0x34 ))
#define RW_VDO_MOTION   (VDO_REG_OFST + (0x38 ))
#define RW_VDO_RST      (VDO_REG_OFST + (0x3C ))//write only new
#define RW_VDO_SMBR     (VDO_REG_OFST + (0x3C ))//RW_VDO_DISP read only new
#define VDOV             ((unsigned)(0x1) << 7)
#define TVFLD            ((unsigned)(0x1) << 15)
#define PUR              ((unsigned)(0x1) << 23)
#define RW_VDO_PTRSVDOY (VDO_REG_OFST + (0x40 ))
#define SVEN             ((unsigned)(0x1) << 28)

#define RW_VDO_PTRSVDOC (VDO_REG_OFST + (0x44 ))
#define RW_VDO_SVDOLINE (VDO_REG_OFST + (0x48 ))
#define RW_VDO_SVDOPXL  (VDO_REG_OFST + (0x4C ))

#define RW_VDO_SL2      (VDO_REG_OFST + (0x50 ))//Y2 new
#define RW_VDO_SLC2     (VDO_REG_OFST + (0x54 ))//STALINEC2 new
#define RW_VDO_MBAVG1 (VDO_REG_OFST + (0x58 ))
#define RW_VDO_MBAVG2 (VDO_REG_OFST + (0x5c ))
#define RW_VDO_COMB_CNT (VDO_REG_OFST + (0x60 ))
#define RW_VDO_PSMODE (VDO_REG_OFST + (0x64 ))
#define GET_3FLD      (0x1 << 0)
#define WEB_MASK_ON       (0x1 << 1)
#define B_PIC_3FLD        (0x1 << 2)
#define FLD3_AFTERMP      (0x1 << 3)
#define GET_MORE_LINE     (0x1 << 4)
#define USE_MORE_LINE     (0x1 << 5)
#define NEW_3FMAD         (0x1 << 6)
#define COMB_SAVE_LOWER   (0x1 << 7)
#define NOT_LOADS         (0x1 << 8)
#define NOT_LOADS2        (0x1 << 9)
//#define SHIFT_1_LINE      (0x1 << 10)
#define USE_OLD_CFLD_LINE (0x1 << 11)
#define Y_MAD             (0x1 << 12)
#define C_MAD             (0x1 << 13)
#define FIX_SCALE         (0x1 << 14)
#define SPLIT_LOAD_S      (0x1 << 15)
#define USE_MBAVG         (0x1 << 16)
#define MP_OR_MODE        (0x1 << 17)
#define MOTION_AVG        (0x1 << 18)
#define FILT_STILL        (0x1 << 19)
#define ONE_FLD_MOTION    (0x1 << 20)
#define OR_MODE           (0x1 << 21)
#define IGNORE_PEAK       (0x1 << 22)
#define COMB_SEL          (0x1 << 23)
#define SAVE_COMB         (0x1 << 24)
#define SAVE_MODE         (0x1 << 25)
#define COMB_CNT_SEL      (0x1 << 26)
#define COMB3_ONE_FLD     (0x1 << 27)
#define STILL_COMB_ON     (0x1 << 28)
#define C_USE_COMB        (0x1 << 29)
#define INCLUDE_COMB4     (0x1 << 30)
//#define N_STATE_SEL       (0x1 << 31)
#define RW_VDO_3F_THRD  (VDO_REG_OFST + (0x68 ))
#define RW_VDO_STAPOS (VDO_REG_OFST + (0x6c ))

#define RW_VDO_PSFILTER (VDO_REG_OFST + (0x70 ))
#define FILT_TWICE      (0x1 << 0)
#define CFILT_TWICE       (0x1 << 1)
#define MOTION_EXP        (0x1 << 2)
#define STILL_EXP         (0x1 << 3)
#define NEW_4FMAD         (0x1 << 4)
#define RW_MOTION         (0x1 << 5)
#define YS23NOT_RSTYNEED  (0x1 << 6)
#define SUBLINE_ACT_SEL   (0x1 << 7)
#define VDO_TEST0         (0x1 << 8)
#define VDO_TEST1         (0x1 << 9)
#define RST_FIND_YS       (0x1 << 10)
#define MR_SEL            (0x1 << 11)
#define NOT_LOADCS        (0x1 << 12)
#define C_LOAD_OFF        (0x1 << 13)
#define PREFETCH_STEP2    (0x1 << 14)
#define CPREFETCH_STEP2   (0x1 << 15)
#define NOT_S_EXMP        (0x1 << 16)
#define SPEEL_SEL         (0x1 << 17)
#define C_LOW_BW          (0x1 << 18)
#define MB_3F             (0x1 << 19)
#define MOTION_EXP_S      (0x1 << 20)
#define STILL_EXP_S       (0x1 << 21)
#define CSTILL_COMB_ON    (0x1 << 22)
#define CSELF_COMB_EN     (0x1 << 23)
#define MOTION_EXP_4F     (0x1 << 24)
#define STILL_EXP_4F      (0x1 << 25)
#define DELAY_YL          (0x1 << 26)
#define DELAY_CL          (0x1 << 27)
#define WEB_MASK_SEL      (0x1 << 28)
#define NOT_S_EX_MP2      (0x1 << 29)
#define C_BEFORE_Y        (0x1 << 30)

#define RW_VDO_M_CNT  (VDO_REG_OFST + (0x74 ))

#define RW_VDO_RW_CTRL  (VDO_REG_OFST + (0x78 ))
#define M_R_SIMPLE        (0x1 << 18)
#define M_W_SIMPLE        (0x1 << 19)
#define M_R_NO_PF         (0x1 << 20)
#define M_W_AVOID_FILL    (0x1 << 21)
#define NO_M_W            (0x1 << 22)
#define DISABLE_VDO_WRITE (0x1 << 23)
#define M_R_OTHER         (0x1 << 24)
#define M_W_OTHER         (0x1 << 25)
#define SPLIT_CNT_ENABLE  (0x1 << 26)
#define M_W_LAST_SEL      (0x1 << 27)






#define RW_VDO_ENCSYNC  (VDO_REG_OFST + (0x80 ))
#ifdef MT1379_E6
#define PAL60          (0x1)
#define SYNPXL         (0x0)
#else
#define SYNPXL         (0x1)
#endif
#ifdef MT1379_E3
#define Y_G_SYNC       (0x1 << 10)
#endif
#define RW_VDO_TVE_MD   (VDO_REG_OFST + (0x84 ))
#define ENCEN           0
#define CBON            (0x1 << 1)
#define SETUP           (0x1 << 2)
#define CLPF            (0x2 << 4)
#define YLPF            (0x1 << 7)
#define YDEL_MASK       (0x3f << 8)
#define YDEL_0          (0x0 << 8)
#define YDEL_1          (0x5 << 8)
#define DA0_S_CVBS      (0x1 << 16)
#define DA0_YUV         (0x2 << 16)
#define DA0_RGB         (0x3 << 16)
#define DA1_CVBS        (0x1 << 18)
#define DA1_YUV         (0x2 << 18)
#define DA1_RGB         (0x3 << 18)
#ifdef MT1379_E6
#define RB_NO_SYN       (0x1 << 20)
#endif
#define VGA             (0x1 << 21)
#define C54SEL          (0x2 << 22)
#define C54OFF          (0x3 << 22)
#define RGBMVOFF        (0x2 << 28)
#define MVOFF           (0x3 << 28)
#define RW_VDO_CC       (VDO_REG_OFST + (0x88 ))
#define RW_VDO_YUVSCALE (VDO_REG_OFST + (0x8C ))
#define RW_VDO_CGAIN1   (VDO_REG_OFST + (0x90 ))
#define RW_VDO_CGAIN2   (VDO_REG_OFST + (0x94 ))
#define RW_VDO_CGAIN3   (VDO_REG_OFST + (0x98 ))
#define RW_VDO_CGAIN4   (VDO_REG_OFST + (0x9C ))

#define RW_VDO_GAMMA0   (VDO_REG_OFST + (0xA0 ))
#define RW_VDO_GAMMA1   (VDO_REG_OFST + (0xA4 ))
#define RW_VDO_GAMMA2   (VDO_REG_OFST + (0xA8 ))
#define RW_VDO_GAMMA3   (VDO_REG_OFST + (0xAC ))
#define RW_VDO_GAMMA4   (VDO_REG_OFST + (0xB0 ))

#define RW_VDO_DACTRL   (VDO_REG_OFST + (0xB4 ))
#define DARB            0x02
#define DAG             0x08
#define DAX             0x20
#define DAS             0x80
#define PDET            (0x1 << 12)

#define RW_VDO_BURST    (VDO_REG_OFST + (0xB8 ))
#define PAL_60         (0x1 << 15)
#define VGA_SYNC       (0x1 << 17)
#define NTSC_SCH       (0x34 << 16)
#define PAL_SCH        (0x2c << 16)

#define RW_VDO_MV1      (VDO_REG_OFST + (0xC0 ))
#define RW_VDO_MV2      (VDO_REG_OFST + (0xC4 ))
#define RW_VDO_MV3      (VDO_REG_OFST + (0xC8 ))
#define RW_VDO_MV4      (VDO_REG_OFST + (0xCC ))
#define RW_VDO_MV5      (VDO_REG_OFST + (0xD0 ))
#define RW_VDO_MV6      (VDO_REG_OFST + (0xD4 ))

#define RW_VDO_YLF_COE0 (VDO_REG_OFST + (0xE0 ))
#define RW_VDO_YLF_COE1 (VDO_REG_OFST + (0xE4 ))
#define RW_VDO_YLF_COE2 (VDO_REG_OFST + (0xE8 ))
#define RW_VDO_YLF_COE3 (VDO_REG_OFST + (0xEC ))
#define RW_VDO_YLF_COE4 (VDO_REG_OFST + (0xF0 ))
// *** DISPLAY REGISTERS END ***

// **********************************************************************
// BIM/VFMT Register
// **********************************************************************
#define VFMT_REG_OFST     (0x1000 )

#ifdef OLD_BIM
// define IRQ Shift because FPGA EMULATION and Real Chip have difference
// define in IRQ register
#define IRQ_SHIFT         0
#else
#define IRQ_SHIFT         1
#endif

#define RO_IRQST          (VFMT_REG_OFST + 0x00)
#define RW_IRQEN          (VFMT_REG_OFST + (0x04 ))
#define WO_IRQCR          (VFMT_REG_OFST + (0x08 ))
#define IRQ_IRCI          0x1
#define IRQ_SBI           (0x1 << 1)
#define IRQ_STAI          (0x1 << (1 + IRQ_SHIFT))
#define IRQ_OSDI          (0x1 << (2 + IRQ_SHIFT))
#define IRQ_VURI          (0x1 << (3 + IRQ_SHIFT))
#define IRQ_SURI          (0x1 << (4 + IRQ_SHIFT))
#define IRQ_OURI          (0x1 << (5 + IRQ_SHIFT))
#define IRQ_GI            (0x1 << (6 + IRQ_SHIFT))
#define IRQ_PI            (0x1 << (7 + IRQ_SHIFT))
#define IRQ_MCFI          (0x1 << (8 + IRQ_SHIFT))
#define IRQ_VLDI          (0x1 << (9 + IRQ_SHIFT))
#define IRQ_T0I           (0x1 << (10 + IRQ_SHIFT))
#define IRQ_T1I           (0x1 << (11 + IRQ_SHIFT))
// RISC null space
#define IRQ_NSPAI         (0x1 << 13)
// RISC write to Flash
#define IRQ_WFLAI         (0x1 << 14)
#define IRQ_DSPI          (0x1 << 15)
#define IRQ_RS232I        (0x1 << 16)
#define IRQ_VLDECI        (0x1 << 17)

#ifdef MT1379
#define IRQ_FEDIE         (0x1 << 20)
#else
// DRAM fill
#define IRQ_DF            (0x1 << 20)
#endif

//#ifdef MT1379
#define IRQ_GRAPH         (0x1 << 22)
//#endif
#define IRQ_AINI          (0x1 << 24) // IRQ of Audio input


#define RO_FIQST          (VFMT_REG_OFST + (0x0C ))
#define RW_FIQEN          (VFMT_REG_OFST + (0x10 ))
#define WO_FIQCR          (VFMT_REG_OFST + (0x14 ))
#define FIQ_VSYNC         (0x1 << (17 + IRQ_SHIFT))

#ifdef FIQ_ONLY
// If define FIQ_ONLY, We map IRQ register to FIQ
#undef  RO_IRQST
#undef  RW_IRQEN
#undef  WO_IRQCR
#define RO_IRQST          RO_FIQST
#define RW_IRQEN          RW_FIQEN
#define WO_IRQCR          WO_FIQCR
#endif

// ATAPI Command register to 8032
#define RW_TRBM0          (VFMT_REG_OFST + (0x18 ))
#define RW_TRBM4          (VFMT_REG_OFST + (0x1C ))
#define RW_TRBM8          (VFMT_REG_OFST + (0x20 ))
#define IDE_BSY           (0x1 << 24)
#define IDE_SKEY          0xFF
#define IDE_SCODE         (0xFF << 8)
#define IDE_SQ            (0xFF << 16)
// Share-Memory register to 8032
#define RW_TRBMC          (VFMT_REG_OFST + (0x24 ))
#define RW_SHAINFO        RW_TRBMC
#define ATAPI_COMPLETE    0x1
// ATAPI Status from 8032
#define RO_TRAS0          (VFMT_REG_OFST + (0x28 ))
// IR Command from 8032
#define RO_TRAS4          (VFMT_REG_OFST + (0x2C ))
// OSD Command from 8032
#define RO_TRAS8          (VFMT_REG_OFST + (0x30 ))

#define WC_AINT           (VFMT_REG_OFST + (0x34 ))
#define ACMDI             0x1
#define ASHAI             (0x1 << 1)
#define APOKI             (0x1 << 2)
#define RO_AINT           (VFMT_REG_OFST + (0x34 ))
#define CMDBSY            0x1
#define SHAIBSY           (0x1 << 1)

#ifdef OLD_BIM
#define PSTART            (0x1 << 1)
#else
#define PSTART            (0x1 << 2)
#endif

// DMA for Flash
#define RW_DMARA          (VFMT_REG_OFST + (0x38 ))
#define RW_DMAWA          (VFMT_REG_OFST + (0x3C ))
#define RW_DMALEN         (VFMT_REG_OFST + (0x40 ))
#define RW_DMACTL         (VFMT_REG_OFST + (0x44 ))
#define DMABSY            0x1
#define DMATRI            0x1
#define IMODE            (0x1 << 1)

// Timer
#define RW_T0LMT          (VFMT_REG_OFST + (0x48 ))
#define RW_T0             (VFMT_REG_OFST + (0x4c ))
#define RW_T1LMT          (VFMT_REG_OFST + (0x50 ))
#define RW_T1             (VFMT_REG_OFST + (0x54 ))
#define RW_TCTL           (VFMT_REG_OFST + (0x58 ))
#define T0EN              0x1
#define T0ARST            (0x1 << 1)
#define T1EN              (0x1 << 8)
#define T1ARST            (0x1 << 9)

#define RW_DP             (VFMT_REG_OFST + (0x5c ))
// LED
#define RW_LED            (VFMT_REG_OFST + (0x60 ))
#define LED0              0x1
#define LED1              (0x1 << 1)
#define LED2              (0x1 << 2)
#define LED3              (0x1 << 3)
#define LED4              (0x1 << 4)
#define LED5              (0x1 << 5)
#define LED6              (0x1 << 6)
//#define LED7              (0x1 << 7)
#define VDO_LOBW          (0x1 << 7)

#ifdef RISC_SQUARE_WAVE
// LED0 use to generate 1K hz square wave
#undef LED0
#define LED0            (0x1 << 9)
#define LED_SQUARE_WAVE 0x1
#endif


//HTL
#define RO_HTL            (VFMT_REG_OFST + (0x64 ))
#define HTLZ              (0x1 << 23)     // when HTLZ = 1, HTL = 0
//TBLK
#define RO_TBLK           (VFMT_REG_OFST + (0x68 ))
//BC
#define RO_BC             (VFMT_REG_OFST + (0x6C ))

#define RO_2CH_PROT       (VFMT_REG_OFST + (0x70 ))
#define RO_BOND           (VFMT_REG_OFST + (0x70 ))


// VFMT
#define RW_CFFSEL         (VFMT_REG_OFST + (0x80 ))
#define RW_CFFA           (VFMT_REG_OFST + (0x84 ))
#define RW_CFFB           (VFMT_REG_OFST + (0x88 ))
#define RW_CFFC           (VFMT_REG_OFST + (0x8C ))

#define RW_HCTL           (VFMT_REG_OFST + (0x90 ))
#define RW_VCTL           (VFMT_REG_OFST + (0x94 ))
#define PRGS_OUT          ((unsigned)0x1 << 15)
#ifdef MT1379
#define VFMT_NTSC         ((unsigned)0x0 << 30)
#define VFMT_PAL_M        ((unsigned)0x1 << 30)
#define VFMT_PAL_N        ((unsigned)0x2 << 30)
#define VFMT_PAL_B        ((unsigned)0x3 << 30)
#define VFMT_TV_MASK      ((unsigned)0x3 << 30)
#else
#define VFMT_PAL_B        ((unsigned)0x1 << 31)
#endif

#define RW_VOFF           (VFMT_REG_OFST + (0x98 ))
#define RW_HLEN           (VFMT_REG_OFST + (0x9C ))
#define RW_HACT           (VFMT_REG_OFST + (0xA0 ))
#define RW_VACTO          (VFMT_REG_OFST + (0xA4 ))
#define RW_VACTE          (VFMT_REG_OFST + (0xA8 ))
#define RW_FMTCTL         (VFMT_REG_OFST + (0xAC ))
#define FMTEN             0x1
#define FMTMAS            (0x1 << 1)
#define VSEL              (0x1 << 2)
#define HPOR              (0x1 << 3)
#define VPOR              (0x1 << 4)
#define FPOR              (0x1 << 5)
#define SYNCEN            (0x1 << 6)
#define FMTRST            (0x1 << 10)
#define SP_UV_SWAP        (0x1 << 18)
#define UV_SWAP           (0x1 << 19)
#ifdef MT1379
#define MIXEN             (0x1 << 24)
//max
#define NO_PF             (0x1 << 27)
//max
#endif

#define RW_SCACTL         (VFMT_REG_OFST + (0xB0 ))
#define SCAON             0x1
#define LNR               (0x1 << 1)

#define RW_BICOLOR        (VFMT_REG_OFST + (0xB4 ))
#define BIEN              (0x1 << 24)

#define RW_BKCOLOR        (VFMT_REG_OFST + (0xB8 ))

#define RW_EDGE           (VFMT_REG_OFST + (0xBC ))
#ifdef MT1379
#define CORE_8            (0x8 << 8)   // set core threshold to 8
#else
#define CORE_8            (0x8 << 6)    // set core threshold to 8
#define COREON            (0x1 << 11)
#endif

// note: the following are general purpose register
#define RW_RGPR0          (VFMT_REG_OFST + (0x100 ))
#define RW_RGPR1          (VFMT_REG_OFST + (0x104 ))
#define RW_RGPR2          (VFMT_REG_OFST + (0x108 ))
#define RW_RGPR3          (VFMT_REG_OFST + (0x10C ))
#define RW_RGPR4          (VFMT_REG_OFST + (0x110 ))
#define RW_RGPR5          (VFMT_REG_OFST + (0x114 ))
#define RW_RGPR6          (VFMT_REG_OFST + (0x118 ))
#define RW_RGPR7          (VFMT_REG_OFST + (0x11C ))
#define RW_RGPR8          (VFMT_REG_OFST + (0x120 ))
#define RW_RGPR9          (VFMT_REG_OFST + (0x124 ))
#define RW_RGPRA          (VFMT_REG_OFST + (0x128 ))
#define RW_RGPRB          (VFMT_REG_OFST + (0x12C ))
#define RW_RGPRC          (VFMT_REG_OFST + (0x130 ))
#define RW_RGPRD          (VFMT_REG_OFST + (0x134 ))
#define RW_RGPRE          (VFMT_REG_OFST + (0x138 ))
#define RW_RGPRF          (VFMT_REG_OFST + (0x13C ))

// Register to handle address swap
#define RW_SWAP0_BEGIN    (VFMT_REG_OFST + (0x140 ))
#define RW_SWAP0_END      (VFMT_REG_OFST + (0x144 ))
#define RW_SWAP1_BEGIN    (VFMT_REG_OFST + (0x148 ))
#define RW_SWAP1_END      (VFMT_REG_OFST + (0x14C ))
#define RW_SWAP_CTRL      (VFMT_REG_OFST + (0x150 ))
#define SWAP_SR0RD        (0x1)
#define SWAP_SR0WR        (0x1 << 1)
#define SWAP_SR1RD        (0x1 << 2)
#define SWAP_SR1WR        (0x1 << 3)

#define DRAMANC_SA 0
#define dwAbsDramANc(dwPtr)    (((UINT32)(dwPtr)) - DRAMANC_SA)
#define vPowerOnVDec()          WriteREG(RW_VLD_PWRSAVE, 0)

#define WMV_DEFINE
#define WMV_LARGE_FRAME
//#define WMV_DOWN_SCALER

#ifdef WMV_DEFINE

//ilii , spork
#define USE_FBM


//#define V_FIFO_SA         0x03000000//0x0179c000
#define V_FIFO_SA         0x0aa00000  //0x0179c000

#ifdef USE_FBM
#undef V_FIFO_SA

#define V_FIFO_SA         0x0aa00000  //0x03000000//0x02C96000
//#define V_FIFO_SA       0x02C96000

#endif
//#define V_FIFO_SZ         (0x04A9C000 - V_FIFO_SA) //0x3ffff0 - V_FIFO_SA // 10,240 Kbytes
//#define V_FIFO_ED         0x04A9C000

//#define V_FIFO_SZ         (50*1024*1024)
//#define V_FIFO_ED         (0x03000000 + V_FIFO_SZ)
//V_FIFO_ED - V_FIFO_SA can't be near (read pointer - write pointer) by designer said
// make it different
#define V_FIFO_SZ         (50*1024*1024)
#define V_FIFO_ED         (V_FIFO_SA + V_FIFO_SZ)

//#define V_FIFO_ED        (V_FIFO_SA + V_FIFO_SZ) //(0x03000000 + V_FIFO_SZ)

//Define DCAC, MV, Bitplane for WMV

#define PP_Size       (((1920/16)*9*8)*2)
#define DCAC_Size     (7*16*(1920/16))  //(1920*4);  new mode uses larger!!!
#define MV1_Size      ((1920/16)*(1088/16)*16)
#define MV23_Size     1920
#define Bitplane_Size 1088

#define addr_added 0  //0x100
#define DCAC_SA       (V_FIFO_ED + addr_added)
#define Mv_1              (DCAC_SA + DCAC_Size + addr_added)
#define Mv_2              (Mv_1 + MV1_Size + addr_added)
#define Bp_1              (Mv_2 + MV23_Size + addr_added)
#define Bp_2              (Bp_1 + Bitplane_Size + addr_added)
#define Bp_3              (Bp_2 + Bitplane_Size + addr_added)
#define Bp_4              (Bp_3 + Bitplane_Size + addr_added)
#define Mv_3              (Bp_4 + Bitplane_Size + addr_added)
#define Mv_1_2            (Mv_3 + MV23_Size + addr_added)
#define DCAC_2            (Mv_1_2 + MV1_Size + addr_added)
#define Pp_1              (DCAC_2 + DCAC_Size + addr_added)
#define Pp_2              (Pp_1 + PP_Size + addr_added)
#define WBUF_END          (Pp_2 + PP_Size + addr_added)
/*
#define DCAC_SA       0x04A9C004//0x06A9C004//0x00000000
#define Mv_1              0x04A9D198//0x10000//0x27100
#define Mv_2              0x04ABF478//0x4fc00//0x4e200
#define Bp_1              0x04ABFE30//0x8f800//0x75300
#define Bp_2              0x04AC02F0//0x04AC02EC//0x9f800//0x9c400
#define Bp_3              0x04AC0790//0xaf800//0xc3500
#define Bp_4              0x04AC0C40//0xbf800//0xea600
#define Mv_3              0x04AC10FC//0xcf800//0x111700
#define Mv_1_2            0x04AC1AC0//0xdf800//0x138800
#define DCAC_2            0x04AE3DA0//0xef800//0x15f900
#define DCAC_2_END        0x04AE4F34
*/

#ifdef WMV_LARGE_FRAME

#define PIC_Y_SZ          (2048*1088) *2
#define PIC_C_SZ          (2048*1088 / 2) *2
#define PIC_BASE_SA       WBUF_END//0x04AE5000//0x6acfc00
#define VDO_REF_AREA_SA   PIC_BASE_SA
#define VDO_REF_AREA_SZ   ((PIC_Y_SZ + PIC_C_SZ) * 2)
#define PIC0Y_SA          VDO_REF_AREA_SA
#define PIC0C_SA          (PIC0Y_SA + PIC_Y_SZ)
#define PIC1Y_SA          (PIC0C_SA + PIC_C_SZ)
#define PIC1C_SA          (PIC1Y_SA + PIC_Y_SZ)
#define VDO_B_AREA_SA     (VDO_REF_AREA_SA + VDO_REF_AREA_SZ)
#define VDO_B_AREA_SZ     (PIC_Y_SZ + PIC_C_SZ)
#define PIC2Y_SA          VDO_B_AREA_SA
#define PIC2C_SA          (PIC2Y_SA + PIC_Y_SZ)

#define DCAC_TEST_SA      (PIC2C_SA + PIC_C_SZ)
/*
#define DEC_PP_SZ         ((2048/16)*9*8)*2
#define DEC_PP_SA         PIC2C_SA + PIC_C_SZ
#define DEC_PP_1          DEC_PP_SA
#define DEC_PP_2          DEC_PP_SA + DEC_PP_SZ
*/

#else  // !WMV_LARGE_FRAME

#define PIC_Y_SZ          (1920*1088) *2
#define PIC_C_SZ          (1920*1088 / 2) *2
#define PIC_BASE_SA       0x2acfc00
#define VDO_REF_AREA_SA   PIC_BASE_SA
#define VDO_REF_AREA_SZ   ((PIC_Y_SZ + PIC_C_SZ) * 2)
#define PIC0Y_SA          VDO_REF_AREA_SA
#define PIC0C_SA          (PIC0Y_SA + PIC_Y_SZ)
#define PIC1Y_SA          (PIC0C_SA + PIC_C_SZ)
#define PIC1C_SA          (PIC1Y_SA + PIC_Y_SZ)
#define VDO_B_AREA_SA     (VDO_REF_AREA_SA + VDO_REF_AREA_SZ)
#define VDO_B_AREA_SZ     (PIC_Y_SZ + PIC_C_SZ)
#define PIC2Y_SA          VDO_B_AREA_SA
#define PIC2C_SA          (PIC2Y_SA + PIC_Y_SZ)

/*
#define DEC_PP_SZ         ((1920/16)*9*8)*2
#define DEC_PP_SA         PIC2C_SA + PIC_C_SZ
#define DEC_PP_1          DEC_PP_SA
#define DEC_PP_2          DEC_PP_SA + DEC_PP_SZ
*/
#endif
#else //!WMV_DEFINE

#define PIC_Y_SZ          (720*576)
#define PIC_C_SZ          (720*576 / 2)
#define VDO_REF_AREA_SA   (V_FIFO_SA + V_FIFO_SZ)
#define VDO_REF_AREA_SZ   ((PIC_Y_SZ + PIC_C_SZ) * 2)
#define PIC0Y_SA          VDO_REF_AREA_SA
#define PIC0C_SA          (PIC0Y_SA + PIC_Y_SZ)
#define PIC1Y_SA          (PIC0C_SA + PIC_C_SZ)
#define PIC1C_SA          (PIC1Y_SA + PIC_Y_SZ)
#define VDO_B_AREA_SA     (VDO_REF_AREA_SA + VDO_REF_AREA_SZ)
#define VDO_B_AREA_SZ     (PIC_Y_SZ + PIC_C_SZ)
#define PIC2Y_SA          VDO_B_AREA_SA
#define PIC2C_SA          (PIC2Y_SA + PIC_Y_SZ)
#endif //WMV_DEFINE

//Robert: MP4 uses.
// HHKuo's DRAM size total: 13770 bytes
#define MP4_DCAC_SA           (VDO_B_AREA_SA + VDO_B_AREA_SZ)
//Ginny for HD emulation 061122
#ifdef WMV_DEFINE
#ifdef WMV_LARGE_FRAME
#ifdef WRITE_FULL_DCAC_DATA
#define DCAC_SZ           ((((2048 / 16) * 4) * ((1088 / 16) * 4)) * 4) // (MBx * 4) * (MBy * 4) = 25920
#else
#define DCAC_SZ           ((((2048 / 16) * 4) * (4)) * 4) // (MBx * 4) * (4) = 7680
#endif
#define MVEC_SA           (MP4_DCAC_SA + DCAC_SZ)
#define MVEC_SZ           (((2048 / 16) * 4 * (1088 / 16)) * 4) // (MBx * 4) * (MBy) = 6480
#define BMB1_SA           (MVEC_SA + MVEC_SZ)
#define BMB1_SZ           (((2048 / 16) * 4 * (1088 / 16)) * 4) // (MBx * 4) * (MBy) = 6480
#define BCODE_SA          (BMB1_SA + BMB1_SZ)
#define BCODE_SZ          (((2048 / 16) * 2) * 4) // (MBx * 2) = 90
#else   // !WMV_LARGE_FRAME
#ifdef WRITE_FULL_DCAC_DATA
#define DCAC_SZ           ((((1920 / 16) * 4) * ((1088 / 16) * 4)) * 4) // (MBx * 4) * (MBy * 4) = 25920
#else
#define DCAC_SZ           ((((1920 / 16) * 4) * (4)) * 4) // (MBx * 4) * (4) = 7680
#endif
#define MVEC_SA           (MP4_DCAC_SA + DCAC_SZ)
#define MVEC_SZ           (((1920 / 16) * 4 * (1088 / 16)) * 4) // (MBx * 4) * (MBy) = 6480
#define BMB1_SA           (MVEC_SA + MVEC_SZ)
#define BMB1_SZ           (((1920 / 16) * 4 * (1088 / 16)) * 4) // (MBx * 4) * (MBy) = 6480
#define BCODE_SA          (BMB1_SA + BMB1_SZ)
#define BCODE_SZ          (((1920 / 16) * 2) * 4) // (MBx * 2) = 90
#endif
#else  //not define WMV_DEFINE
#ifdef WRITE_FULL_DCAC_DATA
#define DCAC_SZ           ((((720 / 16) * 4) * ((576 / 16) * 4)) * 4) // (MBx * 4) * (MBy * 4) = 25920
#else
#define DCAC_SZ           ((((720 / 16) * 4) * (4)) * 4) // (MBx * 4) * (4) = 720
#endif
#define MVEC_SA           (MP4_DCAC_SA + DCAC_SZ)
#define MVEC_SZ           (((720 / 16) * 4 * (576 / 16)) * 4) // (MBx * 4) * (MBy) = 6480
#define BMB1_SA           (MVEC_SA + MVEC_SZ)
#define BMB1_SZ           (((720 / 16) * 4 * (576 / 16)) * 4) // (MBx * 4) * (MBy) = 6480
#define BCODE_SA          (BMB1_SA + BMB1_SZ)
#define BCODE_SZ          (((720 / 16) * 2) * 4) // (MBx * 2) = 90
#endif

#ifdef WMV_DOWN_SCALER
#ifdef WMV_LARGE_FRAME
#define  VDSCL_Y_BUF_SA    (BCODE_SA + BCODE_SZ)
#define  VDSCL_Y_BUF_SZ     0x250000// 2048*1088
#define  VDSCL_C_BUF_SA  (VDSCL_Y_BUF_SA + VDSCL_Y_BUF_SZ)
#define  VDSCL_C_BUF_SZ   0x250000
#define  VDSCL_WORK_SA   (VDSCL_C_BUF_SA + VDSCL_C_BUF_SZ)
#define  VDSCL_WORK_SZ    0x15000
#else
#define  VDSCL_Y_BUF_SA    (BCODE_SA + BCODE_SZ)
#define  VDSCL_Y_BUF_SZ     0x200000// 1920*1080
#define  VDSCL_C_BUF_SA  (VDSCL_Y_BUF_SA + VDSCL_Y_BUF_SZ)
#define  VDSCL_C_BUF_SZ   0x200000
#define  VDSCL_WORK_SA   (VDSCL_C_BUF_SA + VDSCL_C_BUF_SZ)
#define  VDSCL_WORK_SZ    0x10000
#endif
#endif
VOID vInitVld(void);
VOID vInitPred(void);
VOID vInitVdoMem(void);
VOID vInitWMVMiscMem(void);
VOID vSetVldVFifo(void);
VOID vInitVDec(void);
VOID vResetVDec(void);
VOID vSetMcBufPtr(void);
BOOL fgHWDecBP(UINT32);
VOID WriteREG(UINT32 , UINT32 );
UINT32 ReadREG(UINT32);
UINT32 dwVldRPtr(UINT32*);
VOID _VDEC_UpdateVldWMVWPtr(UINT32 u4WPtr);
VOID vSetVSyncPrmBufPtr(UINT32);
VOID vVSyncWriteVdoReg(void);
VOID WriteDram8b(UINT32 dwAddr, UINT32 dw64HiDW, UINT32 dw64LoDW);
void print_vop_header_parameter(int vopnum, int fref_ctrl, int out_ctrl);
void vSetNewMode(void);

#endif


