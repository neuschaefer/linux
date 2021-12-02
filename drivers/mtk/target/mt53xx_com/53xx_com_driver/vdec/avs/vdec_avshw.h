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
 * $RCSfile: vdec_h264hw.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_h264hw.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_AVSHW_H
#define VDEC_AVSHW_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdec_avsutil.h"
#include "x_lint.h"
#include "x_hal_5381.h"
#include "x_bim.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
//5360A, 5360A ECO, 5360B, 5391, 5393 difference
#if (!defined(CC_MT5360B)) && (!defined(CC_MT5392B)) && (!defined(CC_MT5365))
#define NON_SPEC_CTRL1_EN   //for 5360A, 5360A ECO, 5391
#endif

// *********************************************************************
// Video Decoder Registers define
// *********************************************************************

//#define MEM_PAUSE_SUPPORT

#define VLD_BASE                        (IO_VIRT + 0x02000)
#define MC_BASE                            (IO_VIRT + 0x0a000)
#define AVS_VLD_BASE                    (IO_VIRT + 0x9000)
#define AVC_MV_BASE                    (IO_VIRT + 0x31000)
//for mt5363 h.264 and RV
#define PP_BASE                     (IO_VIRT + 0x2f000)

#if 0
#define VLD_READ32(offset)                    IO_READ32    (VLD_BASE,    (offset))
#define VLD_WRITE32(offset, value)            IO_WRITE32    (VLD_BASE,    (offset), (value))

#define MC_READ32(offset)                    IO_READ32    (MC_BASE,    (offset))
#define MC_WRITE32(offset, value)            IO_WRITE32    (MC_BASE,    (offset), (value))

#define AVS_VLD_READ32(offset)            IO_READ32    (AVC_VLD_BASE,    (offset))
#define AVS_VLD_WRITE32(offset, value)        IO_WRITE32    (AVC_VLD_BASE,    (offset), (value))

#define AVS_MV_READ32(offset)                IO_READ32    (AVC_MV_BASE,    (offset))
#define AVS_MV_WRITE32(offset, value)        IO_WRITE32    (AVC_MV_BASE,    (offset), (value))
#else
#define VLD_READ32(offset)                _VDEC_AVS_IO_Read32(VLD_BASE, offset)
#define VLD_WRITE32(offset, value)        _VDEC_AVS_IO_Write32(VLD_BASE, offset, value)

#define MC_READ32(offset)                _VDEC_AVS_IO_Read32(MC_BASE, offset)
#define MC_WRITE32(offset, value)            _VDEC_AVS_IO_Write32(MC_BASE, offset, value)

#define AVS_VLD_READ32(offset)            _VDEC_AVS_IO_Read32(AVS_VLD_BASE, offset)
#define AVS_VLD_WRITE32(offset, value)    _VDEC_AVS_IO_Write32(AVS_VLD_BASE, offset, value)

#define AVS_MV_READ32(offset)            _VDEC_AVS_IO_Read32(AVC_MV_BASE, offset)
#define AVS_MV_WRITE32(offset, value)    _VDEC_AVS_IO_Write32(AVC_MV_BASE, offset, value)

//for mt5363
#define PP_READ32(offset)            _VDEC_AVS_IO_Read32(PP_BASE, offset)
#define PP_WRITE32(offset, value)    _VDEC_AVS_IO_Write32(PP_BASE, offset, value)

#endif
/*#define VLD_REG_OFFSET   0x1000
#define MC_REG_OFFSET    0x2000
#define AVC_VLD_REG_OFFSET    0x3000
#define AVC_MV_REG_OFFSET    0x4000
#define AVC_FG_REG_OFFSET    0x5000
#define AVC_DRAM_TEST_OFFSET    0x6c00
*/

// *********************************************************************
// Interrupt Related Registers define
// *********************************************************************
#define AVLD_VECTOR 24

// *********************************************************************
// Checksum Related Registers define
// *********************************************************************
#define RW_VLD_MB_STOP_POS            0x3FC
#define RO_VLD_CHKSUM_ALL                0x2F4
#define RO_VLD_CHKSUM_JPG                0x2F8
#define RO_VLD_CHKSUM_DCAC_OUT        0x3AC
#define RO_VLD_CHKSUM_DCAC_DRAMI        0x3B0
#define RO_VLD_CHKSUM_DCAC_DRAMO    0x3B4
#define RO_VLD_CHKSUM_IS_DATA            0x3B8

#define RO_AVLD_CHKSUM_DRAMI            0x294
#define RO_AVLD_CHKSUM_DRAMO            0x298
#define RO_AVLD_CHKSUM_RESIDUAL        0x29C
#define RO_AVLD_CHKSUM_BS                0x2A0
#define RO_AVLD_CHKSUM_MIX1            0x2A8
#define RO_AVLD_CHKSUM_MIX2            0x2AC
#define RO_AVLD_CHKSUM_MIX3            0x2B0
#define RO_AVLD_CHKSUM_MIX4            0x2B4
#define RO_AVLD_CHKSUM_W_ADDR        0x2BC
#define RO_AVLD_CHKSUM_R_ADDR        0x2C0
#define RO_AVLD_CHKSUM_MIX_START        0x2CC

#define RO_AMV_CHKSUM_TYPE            0x24C
#define RO_AMV_CHKSUM_MV0            0x250
#define RO_AMV_CHKSUM_MV1            0x254
#define RO_AMV_CHKSUM_DRAMI            0x258
#define RO_AMV_CHKSUM_DRAMO            0x25C
#define RO_AMV_CHKSUM_IS_BUSY_CNT    0x3C4
#define RO_AMV_CHKSUM_IS_TOTAL        0x3C8
#define RO_AMV_CHKSUM_IS_RLVL            0x3CC
#define RO_AMV_CHKSUM_IS_NZMAP        0x3D0
#define RO_AMV_CHKSUM_IS_DATA        0x3D4
#define RO_AMV_CHKSUM_QP                0x3AC
#define RO_AMV_CHKSUM_SHIFT            0x3B0
#define RO_AMV_CHKSUM_QP_CONST        0x3B4
#define RO_AMV_CHKSUM_QMATRIX        0x3B8
#define RO_AMV_CHKSUM_SCL_DATA        0x3BC
#define RO_AMV_CHKSUM_ACC_CBP        0x350
#define RO_AMV_CHKSUM_INTRA            0x354
#define RO_AMV_CHKSUM_INTRA_TBL        0x358
#define RO_AMV_CHKSUM_MB_AVALIN        0x35C
#define RO_AMV_CHKSUM_MB_AVALY        0x360
#define RO_AMV_CHKSUM_BLK_Y_UP        0x364
#define RO_AMV_CHKSUM_BLK_Y_LT        0x368
#define RO_AMV_CHKSUM_BLK_Y_AVAL    0x36C
#define RO_AMV_CHKSUM_BLK_C_AVAL    0x370

#define RO_MC_CHKSUM_IDCT_Y0Y1        0x5E8
#define RO_MC_CHKSUM_IDCT_Y2Y3        0x5EC
#define RO_MC_CHKSUM_IDCT_CBCR        0x5F0
#define RO_MC_CHKSUM_MC_OUTY            0x5F4
#define RO_MC_CHKSUM_MC_OUTC            0x5F8
#define RO_MC_CHKSUM_MV                0x5FC
#define RO_MC_CHKSUM_MC_PARA1        0x600
#define RO_MC_CHKSUM_MC_PARA2        0x604
#define RO_MC_CHKSUM_PP_Y                0x608
#define RO_MC_CHKSUM_PP_C                0x60C
#define RO_MC_CHKSUM_HIU                0x610
#define RO_MC_CHKSUM_WP_TBL            0x614
#define RO_MC_CHKSUM_RBUS                0x618
#define RO_MC_CHKSUM_QPEL_QBI        0x61C
#define RO_MC_CHKSUM_QPEL_QBO        0x620
#define RO_MC_CHKSUM_MBI                0x624
#define RO_MC_CHKSUM_DS_MIX1            0x628
#define RO_MC_CHKSUM_DS_MIX2            0x62C
#define RO_MC_CHKSUM_DS_MIX3            0x630
#define RO_MC_CHKSUM_DS_MIX4            0x634
// *********************************************************************
// VLD Registers define
// *********************************************************************
// Barral Shifter from 0x00 - 0x80
#define RO_VLD_BARL      0x00

#define RW_VLD_VDOUFM    0x84
  #define VLD_VDOUFM       ((UINT32)0x1 << 0)
  #define VLD_MXOFF        ((UINT32)0x1 << 8)
  #define VLD_ENSTCNT      ((UINT32)0x1 << 9)
  #define VLD_AATO         ((UINT32)0x1 << 10)
  
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
  #define ERR_OUT_PIC          ((UINT32)0x1 << 14)
  #define ERR_BL_PIX           ((UINT32)0x1 << 15)
  #define ERR_CBP_NS           ((UINT32)0x1 << 16)
  #define ERR_DCT_Y_TAB        ((UINT32)0x1 << 17)
  #define ERR_DCT_C_TAB        ((UINT32)0x1 << 18)
  #define ERR_SL_SYN           ((UINT32)0x1 << 19)
  #define ERR_EOB_SYN          ((UINT32)0x1 << 20)
  #define ERR_MB_ADD_INC_TAB   ((UINT32)0x1 << 21)
  #define ERR_CBP_TAB          ((UINT32)0x1 << 22)
  #define ERR_INS_SL           ((UINT32)0x1 << 23)
  #define ERR_SL_QSCALE        ((UINT32)0x1 << 24)
  #define ERR_SL_SYN2          ((UINT32)0x1 << 25)
  #define ERR_MB_ADD           ((UINT32)0x1 << 26)
  #define ERR_MB_TBL           ((UINT32)0x1 << 27)
  #define ERR_MOT_TP           ((UINT32)0x1 << 28)
  #define ERR_MOT_CD           ((UINT32)0x1 << 29)
  #define ERR_MKB              ((UINT32)0x1 << 30)
  #define ERR_MCIDCT_BSY       ((UINT32)0x1 << 31)

#define VLD_ERR_MONITOR  (ERR_BL_PIX + ERR_CBP_NS + ERR_DCT_Y_TAB + \
                          ERR_DCT_C_TAB + ERR_SL_SYN + ERR_EOB_SYN + \
                          ERR_MB_ADD_INC_TAB + ERR_CBP_TAB + ERR_INS_SL + \
                          ERR_SL_QSCALE + ERR_SL_SYN2 + ERR_MB_ADD + \
                          ERR_MB_TBL + ERR_MOT_TP + ERR_MOT_CD + \
                          ERR_MKB + ERR_MCIDCT_BSY + ERR_OUT_PIC)

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

#define RW_VLD_PARA        0x88

#define RW_VLD_PROC        0x8C
  #define VLD_SSCBIT       ((UINT32)0x1 << 16)
  #define VLD_SSCBYTE      ((UINT32)0x1 << 17)
  #define VLD_ABRT         ((UINT32)0x1 << 18)
  #define VLD_PSUP         ((UINT32)0x1 << 19)
  #define VLD_INIFET       ((UINT32)0x1 << 20)
  #define VLD_MBDATA       ((UINT32)0x1 << 21)  // for 1389 MP ECO
  #define VLD_PDHW         ((UINT32)0x1 << 22)
  #define VLD_INIBR        ((UINT32)0x1 << 23)
  #define VLD_PERRCON      ((UINT32)0x1 << 24)
  #define VLD_RTERR        ((UINT32)0x1 << 25)
  #define VLD_EOFR         ((UINT32)0x1 << 26)
  #define VLD_IGBCL        ((UINT32)0x1 << 27)
  #define VLD_DECTOP       ((UINT32)0x1 << 29)
  #define VLD_DECBTM       ((UINT32)0x2 << 29)
  #define VLD_FPRBS        ((UINT32)0x3 << 29)
  #define VLD_B21EN        ((UINT32)0x1 << 31)

#define RW_VLD_PICSZ     0x90
#define RW_VLD_MBROWPRM  0x94

#define RW_VLD_DIGMBSA   0x98
#define RW_VLD_SCALE     0x9C
  #define H_SCALE_1_1    0
  #define H_SCALE_1_2    1
  #define H_SCALE_1_4    2
  #define H_SCALE_3_4    3
  #define H_SCALE_3_8    4
  #define V_SCALE_1_1    0
  #define V_SCALE_1_2    1
  #define V_SCALE_1_4    2

#define RW_VLD_TABLIM    0xA0
#define RW_VLD_TABLNIM   0xA4
#define RW_VLD_DIGMBYOFF 0xA8
#define RW_VLD_PSUPCTR   0xAC
#define RW_VLD_RPTR      0xB0
    #define RPTR_ALIGN    16
    #define WPTR_OFFSET 512//256        //64x4    //mpeg 2 use 144=16x9
#define RW_VLD_VSTART    0xB4
#define RW_VLD_VEND      0xB8
#define RO_VLD_BLKFIN    0xBC

#define RW_VLD_RDY_SWTICH   0xC0
 #define READY_TO_RISC (0x1 << 18)//(0x1 << 17)
#define RW_VLD_PWRSAVE   0xC4
#define RW_VLD_ASYNC   0xD0
    #define WPTR_SETBY_RISC_EN ((UINT32)0x1 << 18)
#define RW_VLD_TIMEOUT   0xDC
#define RO_VLD_FSBI      0xE0
#define RO_VLD_ERRFG     0xE4
#define RO_VLD_FETCHOK   0xE8
    #define VLD_FETCH_OK        ((UINT32)0x1 << 0)
    #define VLD_DRAM_REQ_FIN    ((UINT32)0x1 << 2)

#define RO_VLD_VBAR      0xEC
#define RO_VLD_SRAMCTRL  0xF4
    #if defined(CC_MT5365)
    #define AA_FIT_TARGET_D (1<<1)
    #else
    #define AA_FIT_TARGET_D (1<<17)
    #endif
    #define AA_FIT_TARGET_SCLK (1<<16)
#define RO_VLD_VWPTR     0xF8
#define RO_VLD_VRPTR     0xFC

#define RW_VLD_WAITT     0x100
#define WO_VLD_FDEC      0x104
  #define VLD_PIC_COMPLETE     ((UINT32)0x1 << 0)
  #define VLD_RELOAD_INTRA     ((UINT32)0x1 << 8)
  #define VLD_RELOAD_NONINTRA  ((UINT32)0x1 << 9)
  #define VLD_RST_QMATRIX      ((UINT32)0x1 << 10)

#define WO_VLD_SRST      0x108
#define WO_VLD_WPTR      0x110
  #define WPTR_LSH_BITS  3
  #define VLD_EWPL        ((UINT32)0x1 << 2)
  #define VLD_RISC_WR_EN      ((UINT32)0x1 << 1)
#define WO_VLD_BBUFRST   0x10C

#define RO_VLD_SUM       0x114

#define RW_VLD_DIGEST_BOUNDARY  0x1A4

// *********************************************************************
// MPEG4 relate Registers define
// *********************************************************************
#define RW_VLD_MP4_HDR          0x1C4
#define RW_VLD_MP4_FLG          0x1C8
#define RW_VLD_LDSR             0x1CC
#define RW_VLD_LDSH             0x1D0
#define RW_VLD_QUANT            0x1D4
#define RW_VLD_M4SH             0x1D8
#define RW_VLD_DX3              0x1DC
#define RO_VLD_MP4DECBSY        0x1E0
#define RW_VLD_DECSTART         0x1E4
#define RW_VLD_BREF             0x1E8
#define RO_VLD_MP4DECERR        0x1F4

// HHKuo's
#define RW_VLD_DCACSA           0x300
#define RW_VLD_DCACPITCH        0x304
#define RW_VLD_MVSA             0x308
#define RW_VLD_DIRE_MD          0x30C
#define RW_VLD_BMB1             0x310
#define RW_VLD_BMB2             0x314
#define RW_VLD_BCODE_SA         0x318
#define RW_VLD_DIRE_MD_IL       0x31C
#define RW_VLD_DCMVSEL          0x320
 #define LITE_BREF_I_VOP        (0x1 << 18)
#define RW_VLD_DRAMMBSEL        0x324
#define RO_VLD_DCACBSY          0x380
#define RO_VLD_DCDRAM           0x384
#define RO_VLD_DRAMWR           0x394

#define RW_VLD_2ND_PROC        0x48C
#define RW_VLD_2ND_RPTR      0x4B0
#define RW_VLD_2ND_VSTART    0x4B4
#define RW_VLD_2ND_VEND      0x4B8
#define RW_VLD_2ND_RDY_SWTICH   0x4C0
#define RW_VLD_2ND_ASYNC   0x4D0
#define RO_VLD_2ND_FETCHOK   0x4E8
#define RO_VLD_2ND_VBAR      0x4EC
#define RO_VLD_2ND_VRPTR     0x4FC
#define RO_VLD_2ND_SRAMCTRL  0x4F4
#define RW_VLD_2ND_WPTR      0x510

// *********************************************************************
// AVS relate Registers definitions
// *********************************************************************
#define RW_AVS_VLD_COMPLETE_CLR 0xac

#define RW_AVS_VLD_HW_TIMEOUT 0xB4
#ifdef CC_AVS_EMULATION
#define AVS_HW_TIMEOUT 0x66684B
#else
#define AVS_HW_TIMEOUT 0xCCD097
#endif

#define RO_AVS_VLD_COMPLETE        0x120    
#define RO_AVS_VLD_PIC_FINISH     0x124

#define RW_AVS_VLD_QUANT_TABLE_POS      0x260 
#define RW_AVS_VLD_QUANT_TABLE              0x264

#define RW_AVS_VLD_CTRL 0x84
#define RW_AVS_VLD_PIC_SIZE 0x88
#define RW_AVS_VLD_PHP 0x8C
#define RW_AVS_VLD_ERR_CONCEALMENT 0x90
    #define AVS_ERR_CONCEALMENT_ENABLE 0x201
    #define AVS_ERR_CONCEALMENT_DISABLE 0
    
#define RW_AVS_VLD_PRED_BASE_ADDR 0x98

#define RW_AVS_VLD_DECSTART 0x9C
#define RW_AVS_VLD_CLR_BYTE_COUNT 0xC0
#define RW_VLD_MCPICSZ    0xC8

#define RW_AVS_VLD_BITSTUFFING_SWITCH 0xC4
#define RW_AVS_VLD_MODE_SWITCH 0xF4

#define RW_AVS_VLD_BYTE_COUNT 0x138

#define RW_MC_UMV_PIC_WIDTH     0x208
#define RW_MC_UMV_PIC_HEIGHT    0x20C

#define RW_MC_DEC_Y               0x9C   // output decoder buffer for y
#define RW_MC_DEC_C               0xA0   // output decoder buffer for cbcr

#define RW_AVS_MV_ADDR 0x20C

#define RW_AVS_MC_PP_ENABLE 0x220

#define RW_AVS_MC_DISABLE_WRITE 0x250

#define RW_AVS_MC_P_LIST0_PIC0_Y_ADDR 0x3DC
#define RW_AVS_MC_P_LIST0_PIC1_Y_ADDR 0x3E0
#define RW_AVS_MC_P_LIST0_PIC2_Y_ADDR 0x3E4
#define RW_AVS_MC_P_LIST0_PIC3_Y_ADDR 0x3E8

#define RW_AVS_MC_B_LIST0_PIC0_Y_ADDR 0x45C
#define RW_AVS_MC_B_LIST0_PIC1_Y_ADDR 0x460
#define RW_AVS_MC_B_LIST1_PIC0_Y_ADDR 0x4DC
#define RW_AVS_MC_B_LIST1_PIC1_Y_ADDR 0x4E0

#define RW_AVS_MC_P_FIELD_DESCRIPTION 0x564

#define RW_AVS_MC_B_REF 0x930

#define RW_AVS_MV_BW_I_REF 0x218

#define RW_AVS_BLK_FW_DISTANCE_1_0 0x2A8
#define RW_AVS_BLK_FW_DISTANCE_3_2 0x2AC
#define RW_AVS_BLK_BW_DISTANCE_1_0 0x2B0
#define RW_AVS_BLK_FW_MED_RATIO_1_0 0x2B4
#define RW_AVS_MV_FW_MED_RATIO_3_2 0x2B8
#define RW_AVS_MV_BW_MED_RATIO_1_0 0x2BC
#define RW_AVS_MV_DIRECT_RATIO_1_0 0x2C0
#define RW_AVS_MV_DIRECT_RATIO_3_2 0x2C4

#define RW_AVS_MC_LUMA_SIZE 0x934

#define RW_AVS_PP_AVS_MODE 0x0
#define RW_AVS_PP_LUMA_ADDR 0x224
#define RW_AVS_PP_CHROM_ADDR 0x228

#define RO_AVS_VLD_UE    0xC8 
#define RO_AVS_VLD_SE     0xCC         // 2's complement

#define RW_AVS_FIND_STARTCODE   0xD0
#define RW_AVS_FIND_NEXT_STARTCODE 0xD4

#define RO_AVS_ERROR_TYPE 0x114 
#define RO_AVS_ERROR_ACC 0x118 
#define RO_AVS_TIMEOUT_CNT 0x11C 
// *********************************************************************
// MC Registers define
// *********************************************************************
#define RW_MC_R1Y        0x00
#define RW_MC_R1C        0x04
#define RW_MC_R2Y        0x08
#define RW_MC_R2C        0x0C
#define RW_MC_BY         0x10
#define RW_MC_BC         0x14
#define RW_MC_FWDP       0x18
#define RW_MC_DECFLD     0x1C

#define RW_MC_2FLDMD     0x20
#define RW_MC_OPBUF         0x24
#define RO_MC_MBX        0x28
#define RO_MC_MBY        0x2C
#define RO_MC_BMBX       0x30
#define RO_MC_BMBY       0x34
#define RO_MC_MOTION     0x38
#define RO_MC_MVVALID     0x3C

#define RO_MC_MV1X         0x40
#define RO_MC_MV1Y         0x44
#define RO_MC_MV2X         0x48
#define RO_MC_MV2Y         0x4C
#define RO_MC_MV3X         0x50
#define RO_MC_MV3Y         0x54
#define RO_MC_MV4X         0x58
#define RO_MC_MV4Y         0x5C

#define RO_MC_REFFLD     0x60
#define RO_MC_MBINTRA    0x64
#define RO_MC_FRMDCT     0x68
#define RO_MC_CBP        0x6C
#define RW_MC_HREFP      0x70
#define RW_MC_WMBDIG     0x74
#define RW_MC_DIGMBX     0x78
#define RW_MC_DIGMBY     0x7C

#define RW_MC_DIGY       0x80
#define RW_MC_DIGC       0x84
#define RW_MC_DIGWD      0x88
#define RW_MC_YDCS       0x8C

#define RW_MC_ADDRSWAP   0x90
#define RW_MC_ADDRSWAP_NBM   0x768
#define ADDR_MODE_8520        0x4    //1389
#define ADDR_MODE_5351_NORMAL    0x2

#define RW_PP_ADDRSWAP_SEL 0xC
#define PP_ADDR_MODE_8520   0x21
#define PP_ADDR_MODE_5351_NORMAL    0x1

#define ADDRSWAP_ON      (0x1 << 0)
#define RO_MC_STATUS     0x94
#define RW_MC_DISSOLVE   0x98
#define RW_MC_BY1        0x9C   //first B pic buffer ptr Y
#define RW_MC_BC1        0xA0   //first B pic C
#define RW_MC_BY2        0xA4   //second B pic buffer ptr Y
#define RW_MC_BC2        0xA8   //second B pic C
#define RW_MC_BLK_MODE_W 0xC8

#define RW_MC_PS_CMB11         0xAC //cmb1 intra 1xy 2xy
#define RW_MC_PS_CMB12         0xB0 //cmb1 intra 3xy 4xy
#define RW_MC_PS_CMB13         0xB4 //cmb1 intra 5xy 6xy
#define RW_MC_PS_CMB14         0xB8 //cmb1 intra 7xy 8xy
#define RW_MC_PS_CMB15         0xBC //cmb1 intra 9xy 10xy
#define RW_MC_PS_CMB16         0xC0 //cmb1 MVxy , IDCTxy
#define RW_MC_PS_CMB21         0xC4 //cmb2 intra 1xy 2xy
//#define RW_MC_PS_CMB22         0xC8 //cmb2 intra 3xy 4xy
#define RW_MC_PS_CMB23         0xCC //cmb2 intra 5xy 6xy
#define RW_MC_PS_CMB24         0xD0 //cmb2 intra 7xy 8xy
#define RW_MC_PS_CMB25         0xD4 //cmb2 intra 9xy 10xy
#define RW_MC_PS_CMB26         0xD8 //cmb2 MVxy , IDCTxy
#define RW_MC_PS_CMB_INTRA_THD 0xDC //CMB intra dc threshold
#define RW_MC_PS_CMB_MV_MAX    0xE0 //CMB maximun MV
#define  PS_CMB_MV1_VALID        ((UINT32)(0x1) << 30)
#define  PS_CMB_MV2_VALID        ((UINT32)(0x1) << 29)
#define RW_MC_PS_MP_BOUND      0xE4//motion plane IDCT diff curve upper bound
#define RW_MC_PS_MP_DIFF       0xE8//motion plane IDCT diffrence curve level
#define RW_MC_PS_MP_DIFF1      0xEC//motion plane IDCT diffrence curve level
#define RW_MC_PS_MP_IDCT_CNT   0xF0//motion plane IDCT count threshold
#define RW_MC_PS_CMB_IDCT_MAX  0xF4//CMB maximun IDCT abs sum
#define  PS_CMB_IDCT1_VALID      ((UINT32)(0x1) << 18)
#define  PS_CMB_IDCT2_VALID      ((UINT32)(0x1) << 17)
#define RW_MC_PS_CMB_CTRL      0xF8//CMB control register
#define  PS_CMB1_CLR             ((UINT32)(0x1) << 7)//clear cmb group1
#define  PS_CMB2_CLR             ((UINT32)(0x1) << 6)//clear cmb group2
#define  PS_CMB_SEL2             ((UINT32)(0x1) << 5)// 1:select 2 ,0:select cmb group 1
#define  PS_SRC_DETECT           ((UINT32)(0x1) << 4)//film detect on
#define  PS_MAD_ON               ((UINT32)(0x1) << 3)//motion plane on
#define  PS_CMB_INTRA            ((UINT32)(0x1) << 2)//cmb select intra on
#define  PS_CMB_IDCT             ((UINT32)(0x1) << 1)//cmb select idct on
#define  PS_CMB_MV               ((UINT32)(0x1) << 0)//cmb select mv on

#define RW_MC_PS_MP_READ       0xFC//motion plane read ptr
#define RW_MC_PS_MP_WRITE      0x100//motion plane write ptr
#define RW_MC_PS_DINTRA1       0x104//Dinra 1-12
#define RW_MC_PS_DINTRA2       0x108
#define RW_MC_PS_DINTRA3       0x10C
#define RW_MC_PS_DINTRA4       0x110
#define RW_MC_PS_DINTRA5       0x114
#define RW_MC_PS_DINTRA6       0x118
#define RW_MC_PS_DINTRA7       0x11C
#define RW_MC_PS_DINTRA8       0x120
#define RW_MC_PS_DINTRA9       0x124
#define RW_MC_PS_DINTRA10      0x128
#define RW_MC_PS_DINTRA11      0x12C
#define RW_MC_PS_DINTRA12      0x130
#define RW_MC_PS_DINTER1       0x134//Dinter 1-12
#define RW_MC_PS_DINTER2       0x138
#define RW_MC_PS_DINTER3       0x13C
#define RW_MC_PS_DINTER4       0x140
#define RW_MC_PS_DINTER5       0x144
#define RW_MC_PS_DINTER6       0x148
#define RW_MC_PS_DINTER7       0x14C
#define RW_MC_PS_DINTER8       0x150
#define RW_MC_PS_DINTER9       0x154
#define RW_MC_PS_DINTER10      0x158
#define RW_MC_PS_DINTER11      0x15C
#define RW_MC_PS_DINTER12      0x160
#define RW_MC_PS_DT1           0x164//Dt 1-12
#define RW_MC_PS_DT2           0x168
#define RW_MC_PS_DT3           0x16C
#define RW_MC_PS_DT4           0x170
#define RW_MC_PS_DT5           0x174
#define RW_MC_PS_DT6           0x178
#define RW_MC_PS_DT7           0x17C
#define RW_MC_PS_DT8           0x180
#define RW_MC_PS_DT9           0x184
#define RW_MC_PS_DT10          0x188
#define RW_MC_PS_DT11          0x18C
#define RW_MC_PS_DT12          0x190
#define RW_MC_PS_DB1           0x194//Db 1-12
#define RW_MC_PS_DB2           0x198
#define RW_MC_PS_DB3           0x19C
#define RW_MC_PS_DB4           0x1A0
#define RW_MC_PS_DB5           0x1A4
#define RW_MC_PS_DB6           0x1A8
#define RW_MC_PS_DB7           0x1AC
#define RW_MC_PS_DB8           0x1B0
#define RW_MC_PS_DB9           0x1B4
#define RW_MC_PS_DB10          0x1B8
#define RW_MC_PS_DB11          0x1BC
#define RW_MC_PS_DB12          0x1C0
#define RW_MC_PS_DIFF_CTRL     0x1C4//D calculation control
#define RW_MC_ADDR_MODE         0x1C8
#define WADDM_RASTER    0x1
#define RADDM_RASTER    0x2

#define  PS_BUF_ID               ((UINT32)(0x1) << 27)//current pic buffer id
#define  PS_LAST_BUF_ID          ((UINT32)(0x1) << 25)//last pic buffer id
#define  PS_MP_CNT_CLR           ((UINT32)(0x1) << 3)//clear motion table count
#define  PS_DINTERA_ON           ((UINT32)(0x1) << 2)//enable Dinter, Dintra
#define  PS_DTB_ON               ((UINT32)(0x1) << 1)//enable Dt, Db
#define  PS_D_START              ((UINT32)(0x1) << 0)//start to calculate D values
#define RW_MC_PS_DRAM_MODE     0x1C8//read/write Dram address mode

#define RW_MC_QPEL_TYPE        0x1FC
 #define TYPE_14496            0
 #define TYPE_MOMUSYS          1
#define RW_MC_QUARTER_SAMPLE   0x200
#define RW_MC_ROUNDING_CTRL    0x204
#define RW_MC_UMV_PIC_WIDTH    0x208
#define RW_MC_UMV_PIC_HEIGHT   0x20C
#define RW_MC_WARP_POINT_NUM   0x210
#define RW_MC_GMC_Y_MV         0x214
#define RW_MC_GMC_C_MV         0x218
#define RW_MC_CBCR_MV_TYPE     0x21C
 #define QPEL_4MV_CMV_14496     (0x0)
 #define QPEL_4MV_CMV_DIVX503   (0x1)
 #define QPEL_FIELD_CMV_14496   (0x0 << 8)
 #define QPEL_FIELD_CMV_DIVX503 (0x1 << 8)
 #define QPEL_FRAME_CMV_14496   (0x0 << 16)
 #define QPEL_FRAME_CMV_DIVX503 (0x1 << 16)
 #define QPEL_FRAME_CMV_DIVXOLD (0x2 << 16)

// post-processing registers
#define RW_MC_PP_ENABLE        0x220
#define RW_MC_PP_Y_ADDR        0x224
#define RW_MC_PP_C_ADDR        0x228
#define RW_MC_PP_MB_WIDTH      0x22C
#define RW_MC_BREF             0x230
 #define NML_BREF_I_VOP        (0x1 << 5)
#define RW_MC_PP_QP_TYPE       0x234
 #define SEL_GIVEN_QP           (0x0 << 8)
 #define SEL_USER_QP            (0x1 << 8)
 #define SEL_LUT_QP             (0x2 << 8)
#define RW_MC_PP_DBLK_MODE     0x238
 #define DBLK_14496             (0x0 << 16)
 #define DBLK_DIVX              (0x1 << 16)
 #define DBLK_V_THEN_H          (0x0 << 8)
 #define DBLK_H_THEN_V          (0x1 << 8)
 #define DBLK_Y                 (0x1 << 1)
 #define DBLK_C                 (0x1)
#define RW_MC_PP_DBLK_THD      0x23C
#define RW_MC_PP_DBLK_DIR      0x240
 #define DBLK_H_ONLY            (0x1 << 8)
 #define DBLK_V_ONLY            (0x1)
#define RW_MC_PP_DRING_MODE    0x244
 #define COMB_WITH_DBLK         (0x1 << 16)
 #define MAX_DIFF_MOMUSYS       (0x0 << 8)
 #define MAX_DIFF_14496         (0x1 << 8)
 #define DRING_Y                (0x1 << 1)
 #define DRING_C                (0x1)
#define RW_MC_PP_DRING_THD     0x248
#define RW_MC_PP_INTERLACE     0x24C
 #define INTERLACE_DRING        (0x1 << 8)
 #define INTERLACE_DBLK         (0x1)
#define RW_MC_PP_WB_BY_POST    0x250
#define RW_MC_PP_LOW_BW        0x254
#define RW_MC_PP_SEQ           0x258
 #define DBLK_THEN_DRING        (0x0)
 #define DRING_THEN_DBLK        (0x1)
#define RW_MC_PP_X_RANGE       0x260
#define RW_MC_PP_Y_RANGE       0x264
#define RW_PP_ECO_SWITCH  0x268
  #define PP_ECO_DISABLE  0x2
#define RW_MC_P_LIST0       0x3DC // ~0x458
#define RW_MC_B_LIST0       0x45C // ~0x4d8
#define RW_MC_B_LIST1       0x4DC // ~0x558

#define RW_MC_CBCR_OFFSET        0x55C
#define RW_MC_Y_OUT_ADDR        0x560
#define RW_MC_P_LIST0_FLD        0x564
#define RW_MC_B_LIST0_FLD       0x568
#define RW_MC_B_LIST1_FLD       0x56C

#define RW_MC_BYPASS_DBLK_WRAP    0x57C
#define RO_MC_PP_STATUS        0x5c4
#define RW_MC_REQ_CONNECT   0x5e4
    #define REQ_CON_DISABLE     0x1000
    #define FIFO_4BANK               0x40000
#define RW_MC_HALT_MB            0x664
    #define MC_HALT_ENABLE        0x1
#define RW_MC_RR_CTRL            0x668
    #define BY_PASS 1
    #define RR_COMPACT 0x10
    #define EN_Y_BIT 8
    #define EN_C_BIT 16
    #define Y_RATIO_BIT 12
    #define C_RATIO_BIT 20
    #define HBAND_BIT 30
#define RW_MC_PP_RR_CTRL        0x66C
#define RW_MC_WRAPPER   0x71C
    #define PP_WRAPPER_Y_LINES ((UINT32)0x1 << 27)
    #define PP_WRAPPER_C_LINES ((UINT32)0x1 << 31)

#define RW_MC_REQ_MAX_LEN 0x748
    #define MAX_LEN_EN  ((UINT32)0x1 << 20)

#define RW_MC_RR_FMT    0x74C   //field mode
    #define RR_FIELD_DISP_MODE 0xF

// *********************************************************************
// AVC VLD Registers define
// *********************************************************************
// Barral Shifter from 0x00 - 0x80
#define RO_AVLD_BARL      0x00

#define RW_AVLD_CTRL    0x84
    #define AVC_DEC_EN    (0x1)
    #define AVC_RDY_WITH_CNT    (0x0 << 1)
    #define AVC_RDY_CNT_THD    (0x0 << 3)     
    #define AVC_DEC_CYCLE_EN    (0x1<<13)
    #define AVC_DEC_CYCLE_DISABLE    (0xFFFFDFFF)
    #define AVC_RBSP_SWITCH    (0x1<<14)
    #ifdef NON_SPEC_CTRL1_EN
    #define AVC_NON_SPEC_SWITCH    (0x1<<15)    
    #else
    #define AVC_NON_SPEC_SWITCH    (0x0<<15)
    #endif
    #define AVC_DEC_ERROR_SWITCH_ON    (0x2<<22)
    #define AVC_DEC_ERROR_SWITCH_OFF    (0x0<<22)
    #define AVC_DEC_ERROR_ONLY1MC_START    (0x1<<25)
    #define AVC_RBSP_CHK_INV             ((UINT32)(0x0) << 28)
    #define AVC_SUM6_APPEND_INV       ((UINT32)(0x1) << 29)
#ifdef MEM_PAUSE_SUPPORT
    #define AVLD_MEM_PAUSE_MOD_EN  ((UINT32)0x1<<27)
    #define AVC_READ_FLAG_CHK_INV    ((UINT32)(0x0) << 30)
    #define AVC_NOT_CHK_DATA_VALID  ((UINT32)(0x1) << 31)
#else
     #define AVLD_MEM_PAUSE_MOD_EN  ((UINT32)0x0<<27)
    #define AVC_READ_FLAG_CHK_INV    ((UINT32)(0x1) << 30)
    #define AVC_NOT_CHK_DATA_VALID  ((UINT32)(0x0) << 31)
#endif

#define RW_AVLD_SPS        0x88
#define RW_AVLD_PIC_SIZE    0x8C
#define RW_AVLD_PPS_1        0x90
#define RW_AVLD_PPS_2        0x94
#define RW_AVLD_SHDR_1        0x98
#define RW_AVLD_SHDR_2        0x9C

#define RW_AVLD_MAX_PIC_NUM    0xA0
#define RW_AVLD_CUR_PIC_NUM    0xA4
#define RW_AVLD_REORD_P0_RPL    0xA8    //~0x124
#define RW_AVLD_REORD_B0_RPL    0x128    //~0x1A4
#define RW_AVLD_REORD_B1_RPL    0x1A8    //~0x224

#define RW_AVLD_RESET_PIC_NUM    0x228
    #define RESET_PIC_NUM        0x1
#define RW_AVLD_RPL_REORD         0x22C
#define RW_AVLD_WEIGHT_PRED_TBL    0x230
#define RW_AVLD_INIT_CTX_SRAM    0x234
#define RW_AVLD_PROC            0x238 
 
#define RO_AVLD_UE    0x23C 
#define RO_AVLD_SE     0x240         // 2's complement
#define RW_AVLD_PRED_ADDR    0x244
    #define PRED_SRAM_USED        0
    #define PRED_SRAM_BYPASS    0x40000000
#define RO_AVLD_SLICE_HDR_STATE    0x248
#define RO_AVLD_WEIGHTED_PRED_STATE    0x24C
#define RO_AVLD_SMKR            0x250
#define RO_AVLD_SLICE_STATE    0x254
#define RO_AVLD_MB_STATE        0x258
#define RO_AVLD_MB_PRED_STATE    0x25C
#define RO_AVLD_RESIDUAL_STATE    0x260
#define RO_AVLD_RESIDUAL_BLK_STATE    0x264

#define RW_AVLD_TIMEOUT_THD    0x268
    #if defined(CC_MT5360) || defined(CC_MT5365)
    #define AVLD_TIMEOUT_THD 0xCCD097
    #else
    #define AVLD_TIMEOUT_THD 0x1148000//0x3e00000 // 80 ms at 216MHz,  0xffffffff 
    #endif
#define RW_AVLD_TIMEOUT_CLR    0x26C
#define RO_AVLD_ERR_MESSAGE    0x270
    #define AVLD_ERR_MESSAGE_MSK    0x80000000
#define RO_AVLD_COMPLETE        0x274
#define RO_AVLD_PIC_FINISH     0x278
    #define AVLD_FINISH_MASK    0x80000000

#define RW_AVLD_2ND_BARL_CTRL    0x27C
    #define AVLD_2ND_BARL_EN    0x3 
#define RW_AVLD_ERR_MASK        0x280
    #ifdef NON_SPEC_CTRL1_EN
    #define ERR_MASK    0x7fdfbeff    //disable bit21, ignore cbp error case, disable bit14, cabac_zero_word_err
    #else
    #define ERR_MASK    0x7fdffeff
    #endif
    #define ERR_MASK_OBSV        0x7fdfffff
    #define FBUF_ERR_MASK        0x30000000
    #define ERR_REF_MASK          0//0x200//0x4210  //lg bitstream
    #define ERR_DISP_MASK        0x4210
#define RO_AVLD_ERR_ACC        0x284
#define RO_AVLD_DEC_CYCLE        0x288
#define RW_AVLD_RESET_SUM        0x28C
    #define AVLD_RESET_SUM_OFF 0x0
    #define AVLD_RESET_SUM_ON 0x1
#define RW_AVLD_MC_BUSY_THD    0x290
    #define MC_BUSY_THD        0xffff
#define RO_AVLD_RESISUAL_SUM    0x29C
#define RO_AVLD_BS_SUM            0x2A0
#define RW_AVLD_COMPLETE_CLR    0x2C4
#define RW_AVLD_EMU_PREV        0x2C8
    #define NON_SPEC_SHIFT_EN    0x8    //5393 new feature
    #define MEM_PAUSE_EN 0x3
    #define MEM_PAUSE_DISABLE 0x1
    #define NEIGHBOR_WRITE_DISABLE  0x40000
#define RW_AVLD_ERC_CTRL        0x2D0
    #define ERC_DISABLE 0
    #define ERC_ENABLE 0x1
    #define ERC_MODE0 0
    #define ERC_MODE1 1
    #define ERC_MODE2 2
    #define ERC_DED_ERR 0x2
    #define SLICE_CNT_THRD 3
#define RW_AVLD_ERC_DED_ERR_TYPE    0x2D4    //default 0x204008
    #define DISABLE_FIND_STARTCODE_ERR    0xFFFFFFF7

#define RO_AVLD_2ND_BARL        0x800
#define RO_AVLD_2ND_CTRL        0x884
#define RO_AVLD_2ND_UE            0x888
#define RO_AVLD_2ND_SE            0x88c         // 2's complement


// *********************************************************************
// AVC MV Registers define
// *********************************************************************
#define RW_AMV_P_REF_PARA 0             // ~0x7C
#define RW_AMV_B0_REF_PARA 0x80      // ~0xFC
#define RW_AMV_B1_REF_PARA 0x100    // ~0x17C

#define RW_AMV_MV_BUF_ADDR 0x180    // ~0x1FC

#define RW_AMV_CURR_POC 0x200
#define RW_AMV_CURR_TFLD_POC 0x204
#define RW_AMV_CURR_BFLD_POC 0x208

#define RW_AMV_WR_ADDR 0x20C
#define RW_AMV_EC_SETTING 0x210
    #define SKIP_MODE3    0x3
    #define AVOID_HANG  0x4

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//module initilization
void _VDEC_AvsVLDHWReset(BOOL bIsInit);

void _VDEC_AvsVLDHWReset2(BOOL bIsInit);

BOOL _VDEC_AvsResetVLD1(VDEC_AVS_DECPRM_T *prDecParam, UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr, UINT32 u4BsFifoStart, 
    UINT32 u4BsFifoEnd, UINT32 u4PredSA, UINT8 u1ECCMode, UINT32 u4ECType);

BOOL _VDEC_AvsResetVLD2(UINT32 u4RDPtrAddr, UINT32 u4WPtrAddr, UINT32 u4BsFifoStart, UINT32 u4BsFifoEnd);

void _VDEC_AvsVLDSetVFifo(UINT32 u4Sa, UINT32 u4Ea);

void _VDEC_AvsVLDSetVFifo2(UINT32 u4Sa, UINT32 u4Ea);

BOOL _VDEC_AvsVLDInitBarrelShifter(VDEC_AVS_DECPRM_T *prDecParam, UINT32 u4Ptr, UINT32 u4Start, UINT32 u4End);

BOOL _VDEC_AvsVLDInitBarrelShifter2(UINT32 u4Ptr);


//vld parsing
UINT32 _VDEC_AVSGetBitStream(UINT32 u4ShiftBit);

UINT32 _VDEC_AVSGetShtBitStream(UINT32 *pu4BitCount, UINT32 u4ShiftBit);

UINT32 _VDEC_AVSGetRealShtBitStream(UINT32 *pu4BitCount, UINT32 u4ShiftBit);

void _VDEC_AVSAlign2Byte(UINT32* pu4BitCount);

BOOL _VDEC_AVSGetBitStreamFlg(void);

UINT32 _VDEC_AvsUeCodeNum(void);

UINT32 _VDEC_AvsSeCodeNum(void);

//header parameter setting
void _VDEC_SetSPSAVLD(UINT32 u4SPSInfo, UINT32 u4Width, UINT32 u4Height);

void _VDEC_SetPPSAVLD(UINT32 u4PPSInfo1, UINT32 u4PPSInfo2);

void _VDEC_SetSHDRAVLD1(UINT32 u4SHDRInfo);

void _VDEC_SetSHDRAVLD2(UINT32 u4SHDRInfo);

//
void _VDEC_AvsMCGetMbXY(UINT32 *pdX, UINT32 *pdY);

void _VDEC_WriteScalingList(UINT32 u4Idx, const CHAR* pcSlicePtr);

void _VDEC_AVCReordering(void);

void _VDEC_PredWeightTable(void);

UINT32 _VDEC_AVSVLDShiftBits(void);

UINT32 _VDEC_AvsVldRPtr(void);

void _VDEC_SetAvsVldWPtr(UINT32 u4WPtr);

void _VDEC_SetAvsPRefPicListReg(VDEC_AVS_REF_PIC_LIST_T rRefPicList, const VDEC_AVS_FBUF_INFO_T* prFBufInfo, 
    UINT32 u4FBufInfo, UINT32 u4ListIdx);

void _VDEC_SetAvsBRefPicListReg(VDEC_AVS_REF_PIC_LIST_T rRefPicList, VDEC_AVS_REF_PIC_LIST_T rRefPicList1, 
    const VDEC_AVS_REF_PIC_LIST_T* prFBufInfo, UINT32 u4FBufInfo, UINT32 u4ListIdx);

UINT32 _VDEC_AvsGetByteCnt(void);

VOID _VDEC_VLDAvsWaitForSramStable(void);

void _VDEC_VLDAvsHWReset(void);

UINT32 _VDEC_AVS_IO_Read32(UINT32 u4Base, UINT32 u4Offset);

void _VDEC_AVS_IO_Write32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Value);

#if 0
void _VDEC_LogSimPat(BOOL bLogEn);
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //VDEC_H264HW_H

