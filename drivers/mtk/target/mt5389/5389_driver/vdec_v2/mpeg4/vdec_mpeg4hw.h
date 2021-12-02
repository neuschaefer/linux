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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_mpeg4hw.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_mpeg4hw.h
 *  This header file declares exported APIs of mpge4 decoder.
 */

#ifndef VDEC_MPEG4HW_H
#define VDEC_MPEG4HW_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "vdec_mpeg4util.h"
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
// Type definitions
//-----------------------------------------------------------------------------
#define MEM_PAUSE_SUPPORT

#define VLD_BASE                (IO_VIRT + 0x02000)
#define VLD_TOP_BASE        (IO_VIRT + 0x02800)
#define MC_BASE                 (IO_VIRT + 0x0a000)
#define PP_BASE                     (IO_VIRT + 0x2f000)
#define MISC_BASE              (IO_VIRT + 0x3000)

#if 1
#define VLD_READ32(offset)               VDEC_IO_Read32 (VLD_BASE,  (offset))
#define VLD_WRITE32(offset, value)  VDEC_IO_Write32  (VLD_BASE,  (offset), (value))

#define VLD_TOP_READ32(offset)              VDEC_IO_Read32 (VLD_TOP_BASE,  (offset))
#define VLD_TOP_WRITE32(offset, value) VDEC_IO_Write32  (VLD_TOP_BASE,  (offset), (value))

#define MC_READ32(offset)                VDEC_IO_Read32 (MC_BASE, (offset))
#define MC_WRITE32(offset, value)   VDEC_IO_Write32  (MC_BASE, (offset), (value))

#define PP_READ32(offset)                VDEC_IO_Read32(PP_BASE, offset)
#define PP_WRITE32(offset, value)   VDEC_IO_Write32  (PP_BASE, (offset), (value))

#define MISC_READ32(offset)                VDEC_IO_Read32(MISC_BASE, offset)
#define MISC_WRITE32(offset, value)   VDEC_IO_Write32  (MISC_BASE, (offset), (value))

#else
#define VLD_READ32(offset)          IO_READ32 (VLD_BASE,  (offset))
#define VLD_WRITE32(offset, value)  IO_WRITE32  (VLD_BASE,  (offset), (value))

#define VLD_TOP_READ32(offset) IO_READ32 (VLD_TOP_BASE,  (offset))
#define VLD_TOP_WRITE32(offset, value) IO_WRITE32  (VLD_TOP_BASE,  (offset), (value))

#define MC_READ32(offset)           IO_READ32 (MC_BASE, (offset))
#define MC_WRITE32(offset, value)   IO_WRITE32  (MC_BASE, (offset), (value))

#define PP_READ32(offset)            IO_READ32(PP_BASE, offset)
#define PP_WRITE32(offset, value)   IO_WRITE32  (PP_BASE, (offset), (value))

#define MISC_READ32(offset)            IO_READ32(MISC_BASE, offset)
#define MISC_WRITE32(offset, value)   IO_WRITE32  (MISC_BASE, (offset), (value))

#define VLD_READ32(offset)            LOG(0, "r 0x%x\n", VLD_BASE + offset)
#define VLD_WRITE32(offset, value)    LOG(0, "w 0x%x 0x%x\n", (VLD_BASE + offset), (value))

#define MC_READ32(offset)             LOG(0, "r 0x%x\n", MC_BASE + offset)
#define MC_WRITE32(offset, value)     LOG(0, "w 0x%x 0x%x\n", (MC_BASE + offset), (value))

#endif

// *********************************************************************
// Video Decoder Registers define
// *********************************************************************
// *********************************************************************
// VLD Registers definitions
// *********************************************************************
// Barral Shifter from 0x00 - 0x80
#define RO_VLD_BARL             0x00

#define RW_VLD_VDOUFM           0x84
#define VLD_VDOUFM              ((UINT32)0x1 << 0)
#define VLD_MXOFF               ((UINT32)0x1 << 8)
#define VLD_ENSTCNT             ((UINT32)0x1 << 9)
#define VLD_AATO                ((UINT32)0x1 << 10)
  
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
#define ERR_OUT_PIC             ((UINT32)0x1 << 14)
#define ERR_BL_PIX              ((UINT32)0x1 << 15)
#define ERR_CBP_NS              ((UINT32)0x1 << 16)
#define ERR_DCT_Y_TAB           ((UINT32)0x1 << 17)
#define ERR_DCT_C_TAB           ((UINT32)0x1 << 18)
#define ERR_SL_SYN              ((UINT32)0x1 << 19)
#define ERR_EOB_SYN             ((UINT32)0x1 << 20)
#define ERR_MB_ADD_INC_TAB      ((UINT32)0x1 << 21)
#define ERR_CBP_TAB             ((UINT32)0x1 << 22)
#define ERR_INS_SL              ((UINT32)0x1 << 23)
#define ERR_SL_QSCALE           ((UINT32)0x1 << 24)
#define ERR_SL_SYN2             ((UINT32)0x1 << 25)
#define ERR_MB_ADD              ((UINT32)0x1 << 26)
#define ERR_MB_TBL              ((UINT32)0x1 << 27)
#define ERR_MOT_TP              ((UINT32)0x1 << 28)
#define ERR_MOT_CD              ((UINT32)0x1 << 29)
#define ERR_MKB                 ((UINT32)0x1 << 30)
#define ERR_MCIDCT_BSY          ((UINT32)0x1 << 31)

#define VLD_ERR_MONITOR         (ERR_BL_PIX + ERR_CBP_NS + ERR_DCT_Y_TAB + \
                                ERR_DCT_C_TAB + ERR_SL_SYN + ERR_EOB_SYN + \
                                ERR_MB_ADD_INC_TAB + ERR_CBP_TAB + ERR_INS_SL + \
                                ERR_SL_QSCALE + ERR_SL_SYN2 + ERR_MB_ADD + \
                                ERR_MB_TBL + ERR_MOT_TP + ERR_MOT_CD + \
                                ERR_MKB + ERR_MCIDCT_BSY + ERR_OUT_PIC)

// Error Type Define
#define ETP_OUT_PIC             18
#define ETP_BL_PIX              1
#define ETP_CBP_NS              2
#define ETP_DCT_Y_TAB           3
#define ETP_DCT_C_TAB           4
#define ETP_SL_SYN              5
#define ETP_EOB_SYN             6
#define ETP_MB_ADD_INC_TAB      7
#define ETP_CBP_TAB             8
#define ETP_INS_SL              9
#define ETP_SL_QSCALE           10
#define ETP_SL_SYN2             11
#define ETP_MB_ADD              12
#define ETP_MB_TBL              13
#define ETP_MOT_TP              14
#define ETP_MOT_CD              15
#define ETP_MKB                 16
#define ETP_MCIDCT_BSY          17

#define RW_VLD_PARA             0x88

#define RW_VLD_PROC             0x8C
#define VLD_SSCBIT              ((UINT32)0x1 << 16)
#define VLD_SSCBYTE             ((UINT32)0x1 << 17)
#define VLD_ABRT                ((UINT32)0x1 << 18)
#define VLD_PSUP                ((UINT32)0x1 << 19)
#define VLD_INIFET              ((UINT32)0x1 << 20)
#define VLD_MBDATA              ((UINT32)0x1 << 21)  // for 1389 MP ECO
#define VLD_PDHW                ((UINT32)0x1 << 22)
#define VLD_INIBR               ((UINT32)0x1 << 23)
#define VLD_PERRCON             ((UINT32)0x1 << 24)
#define VLD_RTERR               ((UINT32)0x1 << 25)
#define VLD_EOFR                ((UINT32)0x1 << 26)
#define VLD_IGBCL               ((UINT32)0x1 << 27)
#define VLD_DECTOP              ((UINT32)0x1 << 29)
#define VLD_DECBTM              ((UINT32)0x2 << 29)
#define VLD_FPRBS               ((UINT32)0x3 << 29)
#define VLD_B21EN               ((UINT32)0x1 << 31)

#define RW_VLD_PICSZ            0x90
#define RW_VLD_TOP_PIC_WIDTH 0x68
#define RW_VLD_TOP_PIC_HEIGHT 0x70
#define RW_VLD_MBROWPRM         0x94
#define RW_VLD_DIGMBSA          0x98
#define RW_VLD_SCALE            0x9C
#define H_SCALE_1_1             0
#define H_SCALE_1_2             1
#define H_SCALE_1_4             2
#define H_SCALE_3_4             3
#define H_SCALE_3_8             4
#define V_SCALE_1_1             0
#define V_SCALE_1_2             1
#define V_SCALE_1_4             2

#define RW_VLD_TABLIM           0xA0
#define RW_VLD_TABLNIM          0xA4
#define RW_VLD_DIGMBYOFF        0xA8
#define RW_VLD_PSUPCTR          0xAC
#define RW_VLD_RPTR             0xB0
#define RW_VLD_VSTART           0xB4
#define RW_VLD_VEND             0xB8
#define RO_VLD_BLKFIN           0xBC

#define RW_VLD_RDY_SWTICH       0xC0
#define READY_TO_RISC           (0x1 << 17)
#define RW_VLD_PWRSAVE          0xC4
#define RW_VLD_MCPICSZ    0xC8
#define RW_VLD_ASYNC            0xD0
#define RW_VLD_INTP          0xD4

#define VLD_INTP               (0x1 <<0)
#define MC_INTP                (0x1 << 1)
#define MDEC_TIMEOUT_INTP  (0x1 << 2)
#define START_CODE_INT         (0x1 << 3)
#define VLD_CLR                (0x1 << 8)
#define MDEC_TIMEOUT_CLR      (0x1 << 10)
#define START_CODE_CLR         (0x1 << 11)  

#define RW_VLD_TIMEOUT          0xDC
#define RO_VLD_FSBI             0xE0
#define RO_VLD_ERRFG            0xE4
#define RO_VLD_FETCHOK          0xE8
#define VLD_FETCH_OK            ((UINT32)0x1 << 0)
#define VLD_DRAM_REQ_FIN        ((UINT32)0x1 << 2)

#define RO_VLD_VBAR             0xEC
#define RO_VLD_SRAMCTRL         0xF4
#ifdef CC_IS_MPEG4_POST_MT5363
#define AA_FIT_TARGET_D (1<<0)
#else
#define AA_FIT_TARGET_D (1<<17)
#endif
#define RO_VLD_VWPTR            0xF8
#define RO_VLD_VRPTR            0xFC
    #define RPTR_ALIGN    16
    #define WPTR_OFFSET 512//256        //64x4    //mpeg 2 use 144=16x9


#define RW_VLD_WAITT            0x100
#define WO_VLD_FDEC             0x104
#define VLD_PIC_COMPLETE        ((UINT32)0x1 << 0)
#define VLD_RELOAD_INTRA        ((UINT32)0x1 << 8)
#define VLD_RELOAD_NONINTRA     ((UINT32)0x1 << 9)
#define VLD_RST_QMATRIX         ((UINT32)0x1 << 10)

#define WO_VLD_SRST             0x108
#define WO_VLD_WPTR             0x110
#define VLD_RISC_WR_EN          ((UINT32)0x1 << 1)
#define WO_VLD_BBUFRST          0x10C

#define RO_VLD_SUM              0x114

#define RO_VLD_DEC_OK             0x1E0

#define RW_VLD_DIGEST_BOUNDARY  0x1A4
#define RW_VLD_MP4_ABS         0x23C


// *********************************************************************
// Registers definitions
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
#define RW_VLD_NON_P            0x1E8
#define RO_VLD_MP4DECERR        0x1F4

#define RW_VLD_RISC_QMATRIX    0x260
#define VLD_NONINTRA            (0x1 << 6)
#define VLD_READ_QMATRIX    (0x1 << 8)      
#define VLD_WRITE_QMATRIX  (0x1 << 9)

#define RW_VLD_QMATRIX_RW     0x264


#define RW_VLD_DCACSA           0x300
#define RW_VLD_DCACPITCH        0x304
#define RW_VLD_MVSA             0x308
#define RW_VLD_DIRE_MD          0x30C
#define RW_VLD_BMB1             0x310
#define RW_VLD_BMB2             0x314
#define RW_VLD_BCODE_SA         0x318
#define RW_VLD_DIRE_MD_IL       0x31C
#define RW_VLD_DCMVSEL          0x320
#define RW_VLD_DRAMMBSEL        0x324
#define RW_VLD_VOP_TYPE         0x350

#define RW_VLD_NEW_MV_PITCH    0x308
#define RW_VLD_NEW_MVSA    0x36C
#define RW_VLD_NEW_BMB1_PITCH 0x310
#define RW_VLD_NEW_BMB2_PITCH 0x314
#define RW_VLD_NEW_BMB1    0x370
#define RW_VLD_NEW_BMB2    0x374
#define RW_VLD_NEW_BCODE    0x378
#define RW_VLD_NEW_DCACPITCH    0x328
#define RW_VLD_NEW_DCAC_SIZE    0x304
#define RW_VLD_NEW_BCODE_SIZE    0x318

#define RO_VLD_DCACBSY          0x380
#define RO_VLD_DCDRAM           0x384
#define RO_VLD_DRAMWR           0x394

// *********************************************************************
// MC Registers definitions
// *********************************************************************
#define RW_MC_R1Y               0x00
#define RW_MC_R1C               0x04
#define RW_MC_R2Y               0x08
#define RW_MC_R2C               0x0C
#define RW_MC_BY                0x10
#define RW_MC_BC                0x14
#define RW_MC_FWDP              0x18
#define RW_MC_DECFLD            0x1C

#define RW_MC_2FLDMD            0x20
#define RW_MC_OPBUF             0x24
#define RO_MC_MBX               0x28
#define RO_MC_MBY               0x2C
#define RO_MC_BMBX              0x30
#define RO_MC_BMBY              0x34
#define RO_MC_MOTION            0x38
#define RO_MC_MVVALID           0x3C

#define RO_MC_MV1X              0x40
#define RO_MC_MV1Y              0x44
#define RO_MC_MV2X              0x48
#define RO_MC_MV2Y              0x4C
#define RO_MC_MV3X              0x50
#define RO_MC_MV3Y              0x54
#define RO_MC_MV4X              0x58
#define RO_MC_MV4Y              0x5C

#define RO_MC_REFFLD            0x60
#define RO_MC_MBINTRA           0x64
#define RO_MC_FRMDCT            0x68
#define RO_MC_CBP               0x6C
#define RW_MC_HREFP             0x70
#define RW_MC_WMBDIG            0x74
#define RW_MC_DIGMBX            0x78
#define RW_MC_DIGMBY            0x7C

#define RW_MC_DIGY              0x80
#define RW_MC_DIGC              0x84
//#define RW_MC_DIGWD             0x88
#define RW_MC_YDBC			0x88
#define RW_MC_YDCS              0x8C

#define RW_MC_ADDRSWAP          0x90
#define RW_MC_NBM_ADDRSWAP 0x768
#define RW_MC_MCPICSZ    0x980

#define MPEG4_ADDR_MODE_8520_NORMAL    0x3
#define MPEG4_ADDR_MODE_5351_NORMAL    0x2

#define RO_MC_STATUS            0x94
#define RW_MC_DISSOLVE          0x98
#define RW_MC_BY1               0x9C   //first B pic buffer ptr Y
#define RW_MC_BC1               0xA0   //first B pic C
#define RW_MC_BY2               0xA4   //second B pic buffer ptr Y
#define RW_MC_BC2               0xA8   //second B pic C

#if defined(CC_MT5363) && !defined(CC_MT5365) && !defined(CC_MT5395) && !defined(CC_MT5368)
#define RW_MC_DBK_CTRL     0xC0
    #define DBK_EN      (0x1 << 0)
    #define DBK_2MBROW (0x1 << 1)    
    #define DBK_BW_SAVING (0x1 << 5)            
    #define DBK_DISABLE_ECO (0x1 << 6)       
    #define DBK_DISABLE_DELAY (0x1 << 7)          
    #define DBK_GROUP1 (0x1 << 9)                
#define RW_MC_DBK_PICSZ      0xC4
#define RW_MC_DBK_Y             0xC8
#define RW_MC_DBK_C             0xCC
#define RW_MC_DBK_THRSD  0xD0
#define RW_MC_DBK_QP_Y 0xD4
#define RW_MC_DBK_ADV_THRSD_Y 0xD8
#define WO_MC_DBK_DEC 0xDC
    #define DBK_START    (0x1 << 0)
#define RO_MC_DBK_STATUS 0xE0
#define RW_MC_DBK_DEBUG   0xE4
    #define OFST_EN (0x1 << 0)
    #define DEF_EN (0x1 << 1)
    #define FPF_EN (0x1 << 2)    
#define RO_MC_DBK_OFST_CNT 0xE8
#define RO_MC_DBK_DEFT_CNT 0xEC
#define RO_MC_DBK_4P_CNT 0xF0
#define RW_MC_DBK_QP_C 0xF4
#define RW_MC_DBK_ADV_THRSD_C 0xF8
#define RW_MC_DBK_INTEREST 0xFC
#else
#define RW_MC_POSTEN 0x220
    #define POSTEN    (0x1 << 0)
    
#define RW_MC_DBK_Y 0x224
#define RW_MC_DBK_C 0x228

#define RW_MC_POST_WD 0x22C

#define RW_MC_DBK_CTRL 0x238
    #define DBKC      (0x1 << 0)
    #define DBKY    (0x1 << 1)
    
#define RW_MC_DBLK_THR    0x23C

#define RW_MC_WPOST   0x250
    #define DISABLE_MC_WRITE    (0x1 << 0)
    
#define RW_MC_HMBRNG   0x260
#define RW_MC_VMBRNG   0x264

#define RW_ECO_CTRL   0x268
	#define DBK_FLR_CNT  (0x1 << 8)
	#define DBK_FLR_NEW_MODE   (0x1 << 4)
	
#define RO_MC_DBK_4P     0x5C4

#define RO_MC_DBK_DEFT     0x608
#define RO_MC_DBK_OFST     0x60C

#define RW_MC_DBK_FLR   0x658
    #define PARTIAL     (0x1 << 0)
    #define INTERLACE    (0x1 << 1)
    #define ENGR        (0x1 << 2)    
    #define SKIPMB      (0x1 << 3)    
    #define BYPASSWRAP    (0x1 << 4)    
    #define WRAPIDX    (0x1 << 5)    
    
#define RW_MC_DBK_PARTIAL 0x65C

#define RW_MC_QPBPT  0x700
#define RW_MC_DBKATHR   0x704

#define MC_REG_525   0x834


#define RW_PP_CTRL      0x30
	#define DBK_STR_FLITER  (0x1 << 8)
#define RO_PP_DBK_4P    0x50
#define RO_PP_DBK_DEFT  0x54
#define RO_PP_DBK_OFST  0x58
#endif

#define RW_MC_PS_CMB11          0xAC //cmb1 intra 1xy 2xy
#define RW_MC_PS_CMB12          0xB0 //cmb1 intra 3xy 4xy
#define RW_MC_PS_CMB13          0xB4 //cmb1 intra 5xy 6xy
#define RW_MC_PS_CMB14          0xB8 //cmb1 intra 7xy 8xy
#define RW_MC_PS_CMB15          0xBC //cmb1 intra 9xy 10xy
#define RW_MC_PS_CMB16          0xC0 //cmb1 MVxy , IDCTxy
#define RW_MC_PS_CMB21          0xC4 //cmb2 intra 1xy 2xy
#define RW_MC_PS_CMB22          0xC8 //cmb2 intra 3xy 4xy
#define RW_MC_PS_CMB23          0xCC //cmb2 intra 5xy 6xy
#define RW_MC_PS_CMB24          0xD0 //cmb2 intra 7xy 8xy
#define RW_MC_PS_CMB25          0xD4 //cmb2 intra 9xy 10xy
#define RW_MC_PS_CMB26          0xD8 //cmb2 MVxy , IDCTxy
#define RW_MC_PS_CMB_INTRA_THD  0xDC //CMB intra dc threshold
#define RW_MC_PS_CMB_MV_MAX     0xE0 //CMB maximun MV
#define PS_CMB_MV1_VALID        ((unsigned)(0x1) << 30)
#define PS_CMB_MV2_VALID        ((unsigned)(0x1) << 29)
#define RW_MC_PS_MP_BOUND       0xE4  //motion plane IDCT diff curve upper bound
#define RW_MC_PS_MP_DIFF        0xE8  //motion plane IDCT diffrence curve level
#define RW_MC_PS_MP_DIFF1       0xEC  //motion plane IDCT diffrence curve level
#define RW_MC_PS_MP_IDCT_CNT    0xF0  //motion plane IDCT count threshold
#define RW_MC_PS_CMB_IDCT_MAX   0xF4  //CMB maximun IDCT abs sum
#define PS_CMB_IDCT1_VALID      ((unsigned)(0x1) << 18)
#define PS_CMB_IDCT2_VALID      ((unsigned)(0x1) << 17)
#define RW_MC_PS_CMB_CTRL       0xF8//CMB control register
#define PS_CMB1_CLR             ((unsigned)(0x1) << 7)//clear cmb group1
#define PS_CMB2_CLR             ((unsigned)(0x1) << 6)//clear cmb group2
#define PS_CMB_SEL2             ((unsigned)(0x1) << 5)// 1:select 2 ,0:select cmb group 1
#define PS_SRC_DETECT           ((unsigned)(0x1) << 4)//film detect on
#define PS_MAD_ON               ((unsigned)(0x1) << 3)//motion plane on
#define PS_CMB_INTRA            ((unsigned)(0x1) << 2)//cmb select intra on
#define PS_CMB_IDCT             ((unsigned)(0x1) << 1)//cmb select idct on
#define PS_CMB_MV               ((unsigned)(0x1) << 0)//cmb select mv on

#define RW_MC_PS_MP_READ        0xFC  //motion plane read ptr
#define RW_MC_PS_MP_WRITE       0x100 //motion plane write ptr
#define RW_MC_PS_DINTRA1        0x104 //Dinra 1-12
#define RW_MC_PS_DINTRA2        0x108
#define RW_MC_PS_DINTRA3        0x10C
#define RW_MC_PS_DINTRA4        0x110
#define RW_MC_PS_DINTRA5        0x114
#define RW_MC_PS_DINTRA6        0x118
#define RW_MC_PS_DINTRA7        0x11C
#define RW_MC_PS_DINTRA8        0x120
#define RW_MC_PS_DINTRA9        0x124
#define RW_MC_PS_DINTRA10       0x128
#define RW_MC_PS_DINTRA11       0x12C
#define RW_MC_PS_DINTRA12       0x130
#define RW_MC_PS_DINTER1        0x134 //Dinter 1-12
#define RW_MC_PS_DINTER2        0x138
#define RW_MC_PS_DINTER3        0x13C
#define RW_MC_PS_DINTER4        0x140
#define RW_MC_PS_DINTER5        0x144
#define RW_MC_PS_DINTER6        0x148
#define RW_MC_PS_DINTER7        0x14C
#define RW_MC_PS_DINTER8        0x150
#define RW_MC_PS_DINTER9        0x154
#define RW_MC_PS_DINTER10       0x158
#define RW_MC_PS_DINTER11       0x15C
#define RW_MC_PS_DINTER12       0x160
#define RW_MC_PS_DT1            0x164 //Dt 1-12
#define RW_MC_PS_DT2            0x168
#define RW_MC_PS_DT3            0x16C
#define RW_MC_PS_DT4            0x170
#define RW_MC_PS_DT5            0x174
#define RW_MC_PS_DT6            0x178
#define RW_MC_PS_DT7            0x17C
#define RW_MC_PS_DT8            0x180
#define RW_MC_PS_DT9            0x184
#define RW_MC_PS_DT10           0x188
#define RW_MC_PS_DT11           0x18C
#define RW_MC_PS_DT12           0x190
#define RW_MC_PS_DB1            0x194 //Db 1-12
#define RW_MC_PS_DB2            0x198
#define RW_MC_PS_DB3            0x19C
#define RW_MC_PS_DB4            0x1A0
#define RW_MC_PS_DB5            0x1A4
#define RW_MC_PS_DB6            0x1A8
#define RW_MC_PS_DB7            0x1AC
#define RW_MC_PS_DB8            0x1B0
#define RW_MC_PS_DB9            0x1B4
#define RW_MC_PS_DB10           0x1B8
#define RW_MC_PS_DB11           0x1BC
#define RW_MC_PS_DB12           0x1C0
#define RW_MC_PS_DIFF_CTRL      0x1C4 //D calculation control
#define PS_BUF_ID               ((unsigned)(0x1) << 27) //current pic buffer id
#define PS_LAST_BUF_ID          ((unsigned)(0x1) << 25) //last pic buffer id
#define PS_MP_CNT_CLR           ((unsigned)(0x1) << 3)  //clear motion table count
#define PS_DINTERA_ON           ((unsigned)(0x1) << 2)  //enable Dinter, Dintra
#define PS_DTB_ON               ((unsigned)(0x1) << 1)  //enable Dt, Db
#define PS_D_START              ((unsigned)(0x1) << 0)  //start to calculate D values
#define RW_MC_PS_DRAM_MODE      0x1C8 //read/write Dram address mode

#define RW_MC_QPEL_TYPE         0x1FC
#define TYPE_14496              0
#define TYPE_MOMUSYS            1
#define RW_MC_QUARTER_SAMPLE    0x200
#define RW_MC_ROUNDING_CTRL     0x204
#define RW_MC_UMV_PIC_WIDTH     0x208
#define RW_MC_UMV_PIC_HEIGHT    0x20C
#define RW_MC_WARP_POINT_NUM    0x210
#define RW_MC_GMC_Y_MV          0x214
#define RW_MC_GMC_C_MV          0x218
#define RW_MC_CBCR_MV_TYPE      0x21C
#define QPEL_4MV_CMV_14496      (0x0)
#define QPEL_4MV_CMV_DIVX503    (0x1)
#define QPEL_FIELD_CMV_14496    (0x0 << 8)
#define QPEL_FIELD_CMV_DIVX503  (0x1 << 8)
#define QPEL_FRAME_CMV_14496    (0x0 << 16)
#define QPEL_FRAME_CMV_DIVX503  (0x1 << 16)
#define QPEL_FRAME_CMV_DIVXOLD  (0x2 << 16)


#define RW_PP_ADDRSWAP_SEL 0xC
#define PP_ADDR_MODE_8520   0x21
#define PP_ADDR_MODE_5351_NORMAL    0x1

// post-processing registers
#define RW_MC_PP_ENABLE         0x220
#define RW_MC_PP_Y_ADDR         0x224
#define RW_MC_PP_C_ADDR         0x228
#define RW_MC_PP_MB_WIDTH       0x22C
#define RW_MC_BREF              0x230
#define RW_MC_PP_QP_TYPE        0x234
#define SEL_GIVEN_QP            (0x0 << 8)
#define SEL_USER_QP             (0x1 << 8)
#define SEL_LUT_QP              (0x2 << 8)
#define RW_MC_PP_DBLK_MODE      0x238
#define DBLK_14496              (0x0 << 16)
#define DBLK_DIVX               (0x1 << 16)
#define DBLK_V_THEN_H           (0x0 << 8)
#define DBLK_H_THEN_V           (0x1 << 8)
#define DBLK_Y                  (0x1 << 1)
#define DBLK_C                  (0x1)
#define RW_MC_PP_DBLK_THD       0x23C
#define RW_MC_PP_DBLK_DIR       0x240
#define DBLK_H_ONLY             (0x1 << 8)
#define DBLK_V_ONLY             (0x1)
#define RW_MC_PP_DRING_MODE     0x244
#define COMB_WITH_DBLK          (0x1 << 16)
#define MAX_DIFF_MOMUSYS        (0x0 << 8)
#define MAX_DIFF_14496          (0x1 << 8)
#define DRING_Y                 (0x1 << 1)
#define DRING_C                 (0x1)
#define RW_MC_PP_DRING_THD      0x248
#define RW_MC_PP_INTERLACE      0x24C
#define INTERLACE_DRING         (0x1 << 8)
#define INTERLACE_DBLK          (0x1)
#define RW_MC_PP_WB_BY_POST     0x250
#define RW_MC_PP_LOW_BW         0x254
#define RW_MC_PP_SEQ            0x258
#define DBLK_THEN_DRING         (0x0)
#define DRING_THEN_DBLK         (0x1)
#define RW_MC_PP_X_RANGE        0x260
#define RW_MC_PP_Y_RANGE        0x264
#define RW_MC_DDRCTL            0x26C

#define RW_MC_32B_MODE_EN1  0x7FC
#define RW_MC_32B_MODE_EN2  0x800

// VLD Wrapper Setting
#define RW_VLD_TOP_OUTRANGE_MODE 0x2C

// tracking mode
#define RW_VLD_TOP_TRACKING_MODE 0x48


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//module initilization
VOID _VDEC_VLDMpeg4WaitForSramStable(VOID);

void _VDEC_VLDMpeg4HWReset(void);

void _VDEC_ResetMpeg4VLD1(UINT32 u4RDPtrAddr, UINT32 u4BsFifoStart, 
	                                                    UINT32 u4BsFifoEnd, VDEC_HDLR hVDec);

void _VDEC_VLDMpeg4SetVFifo(UINT32 u4Sa, UINT32 u4Ea);

BOOL _VDEC_VLDInitMpeg4BarrelShifter(UINT32 u4Ptr, VDEC_HDLR hVdec);

BOOL _VDEC_GetVldRPtr(UINT32* pu4Addr, UINT32* pu4Bits, UINT32 u4FifoStart, UINT32 u4FifoEnd);

void _VDEC_MCSetFrmBufPtr(UINT32 u4BufIdx, UINT32 u4PtrY, UINT32 u4PtrC);

UINT32 _VDEC_VldMpeg4RPtr(UINT32* pu4Bits);

UINT32 _VDEC_UpdateVldMpeg4WPtr(UINT32 u4WPtr);


void _VDEC_Mpeg4SetFrmBufPtr(UINT32 u4BufIdx, UINT32 u4PtrY, UINT32 u4PtrC);

void _VDEC_Mpeg4McSetDecFld(UCHAR ucDecFld, UCHAR uc2ndFldSw);

//vld parsing
UINT32 _VDEC_Mpeg4GetBitStream(UINT32* pu4BitCount, UINT32 u4ShiftBit);

void _VDEC_VLDBarl2Vdec(void);

void _VDEC_VLDVdec2Barl(void);

#ifdef CC_MPEG4_DEBLOCKING
void _SetMpeg4DbkPrmProc(VDEC_MPEG4_DEC_PARAM_INFO_T *prDecPrm);
#endif

#define PROCESS_FLAG (1<<15)    
#define S_BITS_PROC_NOP 1
#endif //VDEC_MPEG4HW_H

UINT32 VDEC_IO_Read32(UINT32 u4Base, UINT32 u4Offset);

void VDEC_IO_Write32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Value);
