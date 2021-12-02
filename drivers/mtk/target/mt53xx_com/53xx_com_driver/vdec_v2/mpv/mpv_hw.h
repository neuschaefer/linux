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
 * $RCSfile: mpv_hw.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_hw.h
 *  This header file contains hardware register definitions and internal functions of MPV.
 */


#ifndef MPV_HW_H
#define MPV_HW_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mpv_type.h"
#include "vdec_if.h"
#include "x_mm_common.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if defined(MPV_SEMI_HOST) && !defined(MPV_CRCTEST)
#define MPV_CRCTEST
#endif
//
// VLD Registers define
//

// Barral Shifter from 0x00 - 0x80
#define RO_VLD_BARL           0x00 

#define RW_VLD_VDOUFM     0x84
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

  #define ERR_INSUFT_MBX          (0x1 << 12)
  #define ERR_MV_OVERFLOW       (0x1 << 13)
  #define ERR_OUT_PIC                 (0x1 << 14)
  #define ERR_BL_PIX                    (0x1 << 15)
  #define ERR_CBP_NS                   (0x1 << 16)
  #define ERR_DCT_Y_TAB             (0x1 << 17)
  #define ERR_DCT_C_TAB             (0x1 << 18)
  #define ERR_SL_SYN                    (0x1 << 19)
  #define ERR_EOB_SYN                  (0x1 << 20)
  #define ERR_MB_ADD_INC_TAB   (0x1 << 21)
  #define ERR_CBP_TAB                  (0x1 << 22)
  #define ERR_INS_SL                     (0x1 << 23)
  #define ERR_SL_QSCALE              (0x1 << 24)
  #define ERR_SL_SYN2                  (0x1 << 25)
  #define ERR_MB_ADD                   (0x1 << 26)
  #define ERR_MB_TBL                    (0x1 << 27)
  #define ERR_MOT_TP                   (0x1 << 28)
  #define ERR_MOT_CD                   (0x1 << 29)
  #define ERR_MKB                          (0x1 << 30)
  #define ERR_MCIDCT_BSY            (0x1 << 31)

  #define VLD_ERR_MONITOR  (ERR_BL_PIX + ERR_CBP_NS + ERR_DCT_Y_TAB + \
                         ERR_DCT_C_TAB + ERR_SL_SYN + ERR_EOB_SYN + \
                         ERR_MB_ADD_INC_TAB + ERR_CBP_TAB + ERR_INS_SL + \
                         ERR_SL_QSCALE + ERR_SL_SYN2 + ERR_MB_ADD + ERR_MB_TBL + \
                         ERR_MOT_TP + ERR_MOT_CD + ERR_MKB + ERR_MCIDCT_BSY + \
                         ERR_OUT_PIC+ERR_MV_OVERFLOW+ERR_INSUFT_MBX)

#define RW_VLD_PARA        0x88

#define RW_VLD_PROC        0x8C
    #define VLD_SSCBIT       (0x1 << 16)
    #define VLD_SSCBYTE    (0x1 << 17)
    #define VLD_ABRT          (0x1 << 18)
    #define VLD_PSUP          (0x1 << 19)
    #define VLD_INIFET       (0x1 << 20)
    #define VLD_PDHW         (0x1 << 22)
    #define VLD_INIBR         (0x1 << 23)
    #define VLD_PERRCON    (0x1 << 24) // ilii. turn off all error detect/concealment
    #define VLD_RTERR        (0x1 << 25)
    #define VLD_EOFR          (0x1 << 26)
    #define VLD_IGBCL         (0x1 << 27)
    #define VLD_DECTOP      (0x1 << 29)
    #define VLD_DECBTM     (0x2 << 29)
    #define VLD_FPRBS        (0x3 << 29)
    #define VLD_B21EN        (0x1U << 31)

#define RW_VLD_PICSZ           0x90
#define RW_VLD_MBROWPRM  0x94
    #define RACING_EN               (0x1 << 2)
    #define WAIT_VDO_PULSE    (0x1 << 3)
    #define SYNC_MC_MBY    (0x1 << 4)    

#if defined(CC_MT5387) && !defined(CC_MT5363)
#define RW_VLD_DIGMBSA     0x98
#define RW_VLD_SCALE          0x9C
    #define H_SCALE_1_1    0
    #define H_SCALE_1_2    1
    #define H_SCALE_1_4    2
    #define V_SCALE_1_1    0
    #define V_SCALE_1_2    1
    #define V_SCALE_1_4    2
#elif defined(VDEC_IS_POST_MT5368)
#define RW_VLD_MBHP      0x98
#ifdef VDEC_IS_POST_MT5398 //20110812
#define RW_VLD_NEW_MP2_VLD_MODE   0x9C
#endif
#endif

#define RW_VLD_TABLIM        0xA0
#define RW_VLD_TABLNIM      0xA4
#if defined(CC_MT5387) && !defined(CC_MT5363)
#define RW_VLD_DIGMBYOFF 0xA8
#define RW_VLD_PSUPCTR      0xAC
#endif
#define RW_VLD_RPTR            0xB0
#define RW_VLD_VSTART       0xB4
#define RW_VLD_VEND           0xB8
#define RO_VLD_BLKFIN        0xBC

#if defined(CC_MT5387) && !defined(CC_MT5363)
#define R_VLD_SSCS              0xC0
#else
#define RW_VLD_SSCS              0xC0
  #define VLD_CLR                (0x1 << 8)
#endif
#define RW_VLD_PWRSAVE   0xC4

#define RW_VLD_MCPICSZ    0xC8
#define RW_VLD_VER            0xCC
#if defined(CC_MT5387) && !defined(CC_MT5363)
#else
#define RW_VLD_STWD            0xD0
    #define VLD_STWD_SW_WP_EN            (0x1 << 18)
    #define VLD_STWD_LATCH_WP_EN         (0x1 << 19)
    #define VLD_FIRST_PIC           (0x1 << 16)
    #define AA_SCLK_LATCH           (0x1 << 0)  
#endif  

#define RW_VLD_INTP          0xD4
  #define VLD_INTP                  (0x1 <<0)
  #define MC_INTP                   (0x1 << 1)
  #define MDEC_TIMEOUT_INTP         (0x1 << 2)
  #define START_CODE_INT            (0x1 << 3) 
#if defined(CC_MT5387) && !defined(CC_MT5363)
  #define VLD_CLR                (0x1 << 8)
#else
  #define VLD_OK_CLR                (0x1 << 8)
#endif
  #define MDEC_TIMEOUT_CLR          (0x1 << 10)
  #define START_CODE_CLR            (0x1 << 11)  

#define RO_VLD_FSBI           0xE0
#define RO_VLD_ERRFG        0xE4
#define RO_VLD_FETCHOK    0xE8
    #define VLD_FETCH_OK             (0x1 << 0)
    #define VLD_DRAM_REQ_FIN    (0x1 << 2)

#define RO_VLD_VBAR          0xEC
#ifdef VDEC_IS_POST_MT5398  // 0812
#define RWO_VLD_VBAR          0xEC
#endif 
#define RO_VLD_SRAMCTRL  0xF4
    #if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
	#define AA_FIT_TARGET_SCLK (0x1 << 0)
    #else
	#define AA_FIT_TARGET_SCLK (0x1 << 16)
    #endif
	#define PROCESS_FLAG (0x1 << 15)
#define RO_VLD_VWPTR       0xF8
#define RO_VLD_VRPTR        0xFC
#define RW_VLD_WAITT      0x100
#define RW_VLD_FDEC        0x104
    #define VLD_PIC_COMPLETE         (0x1 << 0)
    #define VLD_RELOAD_INTRA         (0x1 << 8)
    #define VLD_RELOAD_NONINTRA  (0x1 << 9)
    #define VLD_RST_QMATRIX           (0x1 << 10)

#define WO_VLD_SRST         0x108
#define RW_VLD_ERRST       0x10C
#define RW_VLD_SW_WP     0x110
    #define VLD_SW_WP_EN         (0x1 << 1)
    #define VLD_LATCH_WP_EN    (0x1 << 2)
    
#define RW_VLD_ERR_STATUS    0x114
#if defined(CC_MT5387) && !defined(CC_MT5363)
#define RO_VLD_ERR_MB            0x1DC

#define RW_VLD_RISC_QMATRIX    0x1E0
    #define VLD_NONINTRA            (0x1 << 6)
    #define VLD_READ_QMATRIX    (0x1 << 8)      
    #define VLD_WRITE_QMATRIX  (0x1 << 9)
#define RW_VLD_QMATRIX_RW     0x1E4
#define RW_VLD_DEC_TIMEOUT    0x1E8

#define RW_VLD_EC_SETTING    0x1F0
    #define OFF_MB_ADDR_INC         (0x1 << 20)
    #define OFF_HEADER2_ERR         (0x1 << 18)
    #define OFF_SLICE_REDECODE      (0x1 << 6)
    #define MC_VLD_MBX_EQ_CHECK     (0x1 << 5)
    #define OFF_INSUFICIENT_MBX     (0x1 << 4)  
#define RW_VLD_INTRAS1    0x1F4
#define RW_VLD_INTRAS2    0x1F8

#define RW_VLD_SRHSTC    0x1FC
    #define PIC_START_CODE    (0x1 << 8)
    #define SLICE_START_CODE    (0x1 << 9)
    #define USRDATA_START_CODE    (0x1 << 10)
    #define SEQ_START_CODE    (0x1 << 11)
    #define EXT_START_CODE    (0x1 << 12)
    #define GOP_START_CODE    (0x1 << 13)
    #define ALL_START_CODE    (0x1 << 14)
    
    #define START_SEARCH    (0x1 << 16)
    #define SEARCH_MODE     (0x1 << 24)

#define RO_VLD_MV1X    0x300    
#define RO_VLD_MV2X    0x304
#define RO_VLD_MV3X    0x308    
#define RO_VLD_MV4X    0x30c    
#define RO_VLD_MV1Y    0x310    
#define RO_VLD_MV2Y    0x314    
#define RO_VLD_MV3Y    0x318    
#define RO_VLD_MV4Y    0x31c
#define RO_VLD_MV12    0x320
#define RO_VLD_MV34    0x324

#define RW_VLD_SRHSTC_END    0x328
#define RO_VLD_QP_SUM    0x32c
#else
#define RO_VLD_ERR_MB            0x25C
#define RW_VLD_RISC_QMATRIX    0x260

    #define VLD_NONINTRA            (0x1 << 6)
    #define VLD_READ_QMATRIX    (0x1 << 8)      
    #define VLD_WRITE_QMATRIX  (0x1 << 9)

#define RW_VLD_QMATRIX_RW     0x264

#define RW_VLD_DEC_TIMEOUT    0x268

#define RW_VLD_SRHSTC    0x26C
    #define PIC_START_CODE    (0x1 << 8)
    #define SLICE_START_CODE    (0x1 << 9)
    #define USRDATA_START_CODE    (0x1 << 10)
    #define SEQ_START_CODE    (0x1 << 11)
    #define EXT_START_CODE    (0x1 << 12)
    #define GOP_START_CODE    (0x1 << 13)
    #define ALL_START_CODE    (0x1 << 14)
    
    #define START_SEARCH    (0x1 << 16)
    #define SEARCH_MODE     (0x1 << 24)

#define RW_VLD_EC_SETTING    0x270
    #define OFF_MB_ADDR_INC         (0x1 << 20)
    #define OFF_HEADER2_ERR         (0x1 << 18)
    #define OFF_SLICE_REDECODE      (0x1 << 6)
    #define MC_VLD_MBX_EQ_CHECK     (0x1 << 5)
    #define OFF_INSUFICIENT_MBX     (0x1 << 4)  

#define RW_VLD_INTRAS1    0x274
#define RW_VLD_INTRAS2    0x278

#define RO_VLD_MV1X    0x280    
#define RO_VLD_MV2X    0x284
#define RO_VLD_MV3X    0x288    
#define RO_VLD_MV4X    0x28c    
#define RO_VLD_MV1Y    0x290    
#define RO_VLD_MV2Y    0x294    
#define RO_VLD_MV3Y    0x298    
#define RO_VLD_MV4Y    0x29c
#define RO_VLD_MV12    0x2A0
#define RO_VLD_MV34    0x2A4

#define RW_VLD_SRHSTC_END    0x3F0
#define RO_VLD_QP_SUM    0x3F4
#endif


// 
// VLD TOP Registers define
// 
#define RW_VLD_TOP_TRACKING_MODE 0x48
    #define ENABLE_TRACKING 0x1000
    #define FIELD_PICTURE 0x10000
    #define BUF_MBL_OFFSET (0<<20)  //[23:20]
#define RW_VLD_TOP_PIC_PIX_SIZE 0x70

// 
// MC Registers define
// 

#define RW_MC_R1Y         0x00
#define RW_MC_R1C         0x04
#define RW_MC_R2Y         0x08
#define RW_MC_R2C         0x0C
#define RW_MC_BY            0x10
#define RW_MC_BC            0x14
#define RW_MC_FWDP       0x18
#define RW_MC_DECFLD     0x1C

#define RW_MC_2FLDMD     0x20
#define RW_MC_OPBUF       0x24
#define RO_MC_MBX            0x28
#define RO_MC_MBY            0x2C
#define RO_MC_BMBX          0x30
#define RO_MC_BMBY          0x34
#define RO_MC_MOTION     0x38
#define RO_MC_MVVALID    0x3C

#define RO_MC_MV1X         0x40
#define RO_MC_MV1Y         0x44
#define RO_MC_MV2X         0x48
#define RO_MC_MV2Y         0x4C
#define RO_MC_MV3X         0x50
#define RO_MC_MV3Y         0x54
#define RO_MC_MV4X         0x58
#define RO_MC_MV4Y         0x5C

#define RO_MC_REFFLD       0x60
#define RO_MC_MBINTRA    0x64
#define RO_MC_FRMDCT     0x68
#define RO_MC_CBP            0x6C
#define RW_MC_HREFP       0x70
    #define HREF_EN  (0x1 << 0)
    #define FLT_Y_EN   (0x1 << 1)    
    #define FLT_C_EN   (0x1 << 2)        
    #define HR_BIT3   (0x1 << 3)        
    #define HR_BIT4   (0x1 << 4)        
    #define HR_BIT5   (0x1 << 5)            
    
#define RW_MC_WMBDIG    0x74
#define RW_MC_DIGMBX     0x78
#define RW_MC_DIGMBY     0x7C

#define RW_MC_DIGY       0x80
#define RW_MC_DIGC       0x84
#define RW_MC_DIGWD    0x88
#define RW_MC_YDCS       0x8C

#define RW_MC_ADDRSWAP   0x90
#define RO_MC_STATUS       0x94
#define RW_MC_DISSOLVE   0x98
#define RW_MC_BY1        0x9C   
#define RW_MC_BC1        0xA0   
#define RW_MC_BY2        0xA4   
#define RW_MC_BC2        0xA8   
#if defined(CC_MT5387) && !defined(CC_MT5363)
#define RW_MC_DRAMMSB  0xAC                //DRAM addr [27:26]

#define RW_MC_RACING  0xB0
#define RW_MC_IUBZ      0xB4
#define RW_MC_WAITVLD      0xB8

#define RW_MC_DPRE      0xBC

#define RW_MC_DBK_CTRL 0xC0
    #define DBK_EN      (0x1 << 0)
    #define DBK_2MBROW (0x1 << 1)    
    #define DBK_BW_SAVING (0x1 << 5)            
    #define DBK_DISABLE_ECO (0x1 << 6)       
    #define DBK_DISABLE_DELAY (0x1 << 7)          
    #define DBK_GROUP1 (0x1 << 9)                
#define RW_MC_DBK_PICSZ 0xC4
#define RW_MC_DBK_Y 0xC8
#define RW_MC_DBK_C 0xCC
#define RW_MC_DBK_THRSD 0xD0
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

#define RW_MC_CTRL_SIGNAL  0x100
    #define FAST_DAC  (0x1 << 2)
    #define NEW_FSM   (0x1 << 3)


#define RW_MC_GROUP1_EN   0x104

#define RW_MC_COMPACT_FLD   0x108
    #define Y_EN    (0x1 << 0)
    #define C_EN    (0x1 << 1)
    #define Y_BTM    (0x1 << 2)
    #define C_BTM    (0x1 << 3)    

#define RO_MC_READ_ALE       0x10C
#define RO_MC_WRITE_ALE     0x110
#define RO_MC_DRAM_DLE     0x114
#define RO_MC_ABORT_ALE     0x118

#define RW_MC_DRAM_MODE   0x1C8  //read/write Dram address mode

#define RW_MC_HREFAVG     0x1CC

#define RO_MC_IU_STATUS     0x3EC
#define RO_MC_IDCTBUF_STATUS     0x3F0

#define RW_MC_VER     0x3FC
#else

#define RW_MC_DRAM_MODE 0x1C8

#define RW_MC_UMV_W 0x208
#define RW_MC_UMV_H 0x20C
    
#define RO_MC_IDCTBUF_STATUS     0x594

#define RW_MC_RACING  0x63c

#define RW_MC_COMPACT_FLD   0x654
    #define Y_EN    (0x1 << 0)
    #define C_EN    (0x1 << 1)
    #define Y_BTM    (0x1 << 2)
    #define C_BTM    (0x1 << 3)    


#define RW_MC_MCHALT 0x664
    #define HALT_EN    (0x1 << 0)

#define RW_MC_RRCTL  0x668
    #define RR_BYPASS  (0x1 << 0)
    #define RR_EN_Y  (0x1 << 8)
    #define RR_34_Y  (0x1 << 12)
    #define RR_EN_C  (0x1 << 16)
    #define RR_34_C  (0x1 << 20)    

#define RW_MC_PP_RRCTL      0x66C

#define RW_MC_PP_WRAPPER       0x71C
    #define PP_WRAPPER_Y_LINES      (0x8000000)
    #define PP_WRAPPER_CBCR_LINES   (0x80000000)
    
#define RW_MC_RR_FIELDCTL      0x74C
    #define RR_PP_C_FIELD_EN    (0x1<<0)
    #define RR_PP_Y_FIELD_EN    (0x1<<1)
    #define RR_MC_C_FIELD_EN    (0x1<<2)
    #define RR_MC_Y_FIELD_EN    (0x1<<3)


//DDR3 32 byte access
#define RW_MC_32B_MODE_EN1  0x7FC
#define RW_MC_32B_MODE_EN2  0x800
#define RW_MC_PP_OUT_32B_MODE_EN  0x834

#define RW_MC_MC_PITCH  0x980

// MC DRAM measure
#define RO_MC_DECODING_CYCLE    0x778
#define RO_MC_DLE_NUM           0x774
#define RO_MC_CYC_SYS           0x9E0
#define RO_MC_INTRA_CNT         0x9E4
#define RO_MC_Y_BLK_CNT         0x9E8
#define RO_MC_C_BLK_CNT         0x9EC
#define RO_MC_WAIT_CNT          0x9F0
#define RO_MC_REQ_CNT           0x7B4
#define RW_MC_PARA_BEHAVIOR     0x998
    #define PARA4_BEHAVIOR     (1<<0)
    #define PARA8_BEHAVIOR     (1<<4)
#endif
// 
// Video Decoder Macros
// 

#define VLD0_TOP_READ32(offset)     IO_READ32(VLD0_BASE + 0x800, (offset))
#define VLD1_TOP_READ32(offset)     IO_READ32(VLD1_BASE + 0x800, (offset))
#define PP0_READ32(offset)          IO_READ32(IO_VIRT + 0x2F000, (offset))
#define PP1_READ32(offset)          0//IO_READ32(0x30000, (offset))
#define MISC0_READ32(offset)           IO_READ32(IO_VIRT + 0x3000, (offset))
#define MISC1_READ32(offset)           0//IO_READ32(VLD1_BASE, (offset))
#define VLD0_READ32(offset)           IO_READ32(VLD0_BASE, (offset))
#define VLD1_READ32(offset)           IO_READ32(VLD1_BASE, (offset))
#define MC0_READ32(offset)             IO_READ32(MC0_BASE, (offset))
#define MC1_READ32(offset)             IO_READ32(MC1_BASE, (offset))

#define VLD0_TOP_WRITE32(offset, value)     IO_WRITE32(VLD0_BASE + 0x800, (offset), (value))
#define VLD1_TOP_WRITE32(offset, value)     IO_WRITE32(VLD1_BASE + 0x800, (offset), (value))
#define PP0_WRITE32(offset, value)          IO_WRITE32(IO_VIRT + 0x2F000, (offset), (value))
#define PP1_WRITE32(offset, value)          //IO_WRITE32(0x30000, (offset), (value))
#define MISC0_WRITE32(offset, value)          IO_WRITE32(IO_VIRT + 0x3000, (offset), (value))
#define MISC1_WRITE32(offset, value)          //IO_WRITE32(0x3000, (offset), (value))
#define VLD0_WRITE32(offset, value)    IO_WRITE32(VLD0_BASE, (offset), (value))
#define VLD1_WRITE32(offset, value)    IO_WRITE32(VLD1_BASE, (offset), (value))
#define MC0_WRITE32(offset, value)      IO_WRITE32(MC0_BASE, (offset), (value))
#define MC1_WRITE32(offset, value)      IO_WRITE32(MC1_BASE, (offset), (value))

UINT32 VLD_READ32(UCHAR id, UINT16 offset);
    
void VLD_WRITE32(int id, int offset, int value);

UINT32 MC_READ32(UCHAR id, UINT16 offset);
    
void MC_WRITE32(int id, int offset, int value);

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

EXTERN void _MPV_GetHwVer(void);
EXTERN void _MPV_PllInit(void);
EXTERN void _MPV_IsrInit(void);
EXTERN void _MPV_VldHWReset(UCHAR ucEsId, UCHAR ucMpvId);
EXTERN UINT32 _MPV_VldGetBitS(UCHAR ucMpvId, UCHAR ucShiftBit);
EXTERN void _MPV_VldResetMat(UCHAR ucMpvId);
EXTERN void _MPV_VldLoadQMat(UCHAR ucMpvId, const UINT32 *pu4Addr, BOOL fgIntra);
EXTERN void _MPV_VldReadQMat(UCHAR ucMpvId, BOOL fgIntra);
EXTERN void _MPV_VldDecPrmProc(UCHAR ucMpvId, UCHAR ucEsId, MPV_ES_INFO_T *prEsInfo,
                        const FBM_SEQ_HDR_T *prFbmSeqHdr, const FBM_PIC_HDR_T *prFbmPicHdr);
EXTERN void _MPV_VldDec(UCHAR ucMpvId, UCHAR ucEsId, const MPV_ES_INFO_T *prEsInfo);
EXTERN void _MPV_VldPartDec(UCHAR ucMpvId, UCHAR ucDecFld);
EXTERN UINT16 _MPV_VldErrCnt(UCHAR ucMpvId);
EXTERN UINT8 _MPV_VldLastErrRow(UCHAR ucMpvId);
EXTERN UINT32 _MPV_VldLastErrType(UCHAR ucMpvId);
EXTERN UINT32 _MPV_VldErrStatus(UCHAR ucMpvId);
EXTERN UINT32 _MPV_VldErrMb(UCHAR ucMpvId);
EXTERN void _MPV_VldSearchStartCode(UCHAR ucMpvId);
EXTERN BOOL _MPV_VldSearchStartCodeFin(UCHAR ucMpvId);
EXTERN void _MPV_VldSearchStartCodeStop(UCHAR ucMpvId);
EXTERN void _MPV_VldRldQMat(UCHAR ucMpvId, BOOL fgIntra);
EXTERN UINT32 _MPV_VldRead32(UCHAR ucMpvId, UINT16 u2Offset);
EXTERN void _MPV_VldRisCtrlWptr(UCHAR ucMpvId);
EXTERN void _MPV_VldSetWptr(UCHAR ucMpvId, UINT32 u4VldWptr);
EXTERN void _MPV_McSetOutputBuf(UCHAR ucMpvId, UCHAR ucOutBufIdx);
EXTERN void _MPV_McWriteToDigBuf(UCHAR ucMpvId, BOOL fgSwitch);
EXTERN void _MPV_McSetDecFld(UCHAR ucMpvId, UCHAR ucDecFld, UCHAR uc2ndFldSw);
EXTERN void _MPV_McSetFrmBufPtr(UCHAR ucEsId, UCHAR ucMpvId, UCHAR ucBufIdx, UINT32 u4PtrY, UINT32 u4PtrC);
EXTERN void _MPV_McSetBlkMode(UCHAR ucMpvId, UCHAR ucMode);
EXTERN UINT32 _MPV_McRead32(UCHAR ucMpvId, UINT16 u2Offset);
EXTERN UINT32 _MPV_SaveReadPtr(UCHAR ucMpvId);
EXTERN void _MPV_SearchStartCodeInt(UCHAR ucMpvId, UINT32 u4Type);

EXTERN void _MPV_DisableSearchMode(UCHAR ucMpvId);
EXTERN void _MPV_DecodeTimeoutLog(UCHAR ucMpvId);
EXTERN INT32 _MPV_DetectIntra(UCHAR ucMpvId, UCHAR ucEsId, MPV_ES_INFO_T *prEsInfo);
EXTERN void _MPV_VldMvSum(UCHAR ucMpvId, UCHAR ucEsId);

#ifdef CC_MPV_DEBLOCKING
EXTERN void _MPV_SetMcDbkBufAddr(UCHAR ucEsId, UCHAR ucMpvId, UCHAR ucFbgId, UCHAR ucDbkFbId, 
    const FBM_PIC_HDR_T *prFbmPicHdr, MPV_ES_INFO_T *prEsInfo);
EXTERN void _MPV_McGetDbkCnt(UCHAR ucMpvId, UCHAR ucEsId, UINT32* pu4DbkOfstCnt, UINT32* pu4Dbk4pCnt);
#endif

EXTERN void _MPV_HandleDbk(UCHAR ucMpvId, UCHAR ucFbgId, UCHAR ucDecFbId, 
    UCHAR ucEsId, FBM_PIC_HDR_T *prFbmPicHdr, UINT32 u4ErrCnt);


#ifdef MPV_DUMP_REG
EXTERN void _MPV_DumpReg(UCHAR ucMpvId);
EXTERN void _MPV_DumpReg1(UCHAR ucMpvId);
EXTERN VOID _MPV_DumpMCData(UCHAR ucEsId, UCHAR ucMpvId);
#endif

#ifdef MPV_TESTCODE
EXTERN void _MPV_VldQpSum(UCHAR ucMpvId);
#endif

EXTERN BOOL _MPV_ResetSearch(UCHAR ucMpvId, UINT32 u4FifoStart, UINT32 u4FifoEnd, 
                            UINT32 u4SearchStart, UINT32 u4SearchEnd);

#if defined(MPV_CRCTEST) || defined(MULTIMEDIA_AUTOTEST_SUPPORT) || defined(VDEC_CRC_TEST)
EXTERN VOID _MPV_Read_Crc(UCHAR ucEsId, UCHAR ucMpvId, UINT32*Ycrc, UINT32*Ccrc);
EXTERN VOID _MPV_EnableCRC(BOOL fgEn, UCHAR ucEsId, UCHAR ucMpvId);
#endif 


#endif //MPV_HW_H

