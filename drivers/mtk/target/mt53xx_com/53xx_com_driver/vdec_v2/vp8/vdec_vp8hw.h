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
 * $RCSfile: vdec_vp8hwhc,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp8hw.h
 *  This source file include vp8 hw setting  function 
 */

#ifndef VDEC_VP8_HW_H
#define VDEC_VP8_HW_H

#include "x_lint.h"
#include "x_hal_5381.h"
#include "x_bim.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END
;

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
// *********************************************************************
// Video Decoder Registers define
// *********************************************************************

#define VLD_TOP_BASE    (IO_VIRT + 0x2800)
#define VLD_BASE        (IO_VIRT + 0x02000)
#define MC_BASE         (IO_VIRT + 0x0a000)
#define PP_BASE         (IO_VIRT + 0x2f000)
#define VP6_VLD_BASE    (IO_VIRT + 0xB000)
#define VDEC_MISC_BASE  (IO_VIRT + 0x3000)
#define AVS_VLD_BASE    (IO_VIRT + 0x9000)
#define VP8_MV_BASE     (IO_VIRT + 0x31000)
#define WEBP_IMGRZ_BASE (IO_VIRT + 0x20000)
#define VLD2_BASE       (VP6_VLD_BASE+0X800)
#define VP8_VLD_BASE    (AVS_VLD_BASE+0x800)

// *********************************************************************
// VLD Registers define
// *********************************************************************
#define RO_VLD_BARL      0x00

#define RW_VLD_VDOUFM    0x84
    #define VLD_VDOUFM       ((UINT32)0x1 << 0)
    #define VLD_MXOFF        ((UINT32)0x1 << 8)
    #define VLD_ENSTCNT      ((UINT32)0x1 << 9)
    #define VLD_AATO         ((UINT32)0x1 << 10)
    #define VLD_RAAF         ((UINT32)0x1 << 11)

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

#define RW_VLD_RPTR      0xB0
    #define RPTR_ALIGN   16
    #define WPTR_OFFSET  512
#define RW_VLD_VSTART    0xB4
#define RW_VLD_VEND      0xB8

#define RW_VLD_RDY_SWTICH   0xC0
   #define READY_TO_RISC       (0x1 << 17)
   #define READY_TO_RISC_1     (0x1 << 18)
   #define READY_TO_RISC_2     (0x1 << 20)

#define RW_VLD_PIC_W_MB     0xC8

#define RW_VLD_ASYNC   0xD0
    #define WPTR_SETBY_RISC_EN ((UINT32)0x1 << 18)

#define RO_VLD_FETCHOK   0xE8
    #define VLD_FETCH_OK        ((UINT32)0x1 << 0)
    #define VLD_DRAM_REQ_FIN    ((UINT32)0x1 << 2)

#define RO_VLD_VBAR      0xEC
#define RO_VLD_SRAMCTRL  0xF4
    #define AA_FIT_TARGET_SCLK (1<<0)
    #define AA_FIT_TARGET_D (1<<1)
    #define PROCESS_FLAG (1<<15)
#define RO_VLD_VWPTR     0xF8
#define RO_VLD_VRPTR     0xFC
#define RO_VLD_CS        0x100
#define WO_VLD_SRST      0x108
#define WO_VLD_WPTR      0x110
    #define WPTR_LSH_BITS  3
    #define VLD_EWPL        ((UINT32)0x1 << 2)
    #define VLD_RISC_WR_EN      ((UINT32)0x1 << 1)
    #define VLD_CLEAR_PROCESS_EN ((UINT32)0x1 << 0)
    #define WPTR_ALIGN   0x10
    #define WPTR_ALIGN_MARK 0xf
    
#define RO_VLD_SUM              0x114
#define RO_VLD_STATUS           0x11c
#define RW_VLD_ABS  0x23C

#define RW_VLD_SCL_ADDR      0x260 
#define RW_VLD_SCL_DATA      0x264

#define RW_VLD_MBDRAM_SEL    0x324

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
#define RW_MC_PP_DBLK_Y_ADDR 0x2C8 // 178x4
#define RW_MC_PP_DBLK_C_ADDR 0x2CC // 179x4
#define RW_MC_P_LIST0       0x3DC // ~0x458
#define RW_MC_B_LIST0       0x45C // ~0x4d8
#define RW_MC_B_LIST1       0x4DC // ~0x558

#define RW_MC_CBCR_OFFSET        0x55C
#define RW_MC_Y_OUT_ADDR        0x560
#define RW_MC_P_LIST0_FLD        0x564
#define RW_MC_B_LIST0_FLD       0x568
#define RW_MC_B_LIST1_FLD       0x56C

#define RW_MC_BYPASS_DBLK_WRAP    0x57C // 351x4
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

#define RW_MC_NBM_CTRL         0x768
    #define RW_MC_NBM_OFF      (0x1 << 8)
#define RO_MC_DLE_NUM          (0x774)
#define RO_MC_CYCLE_DRAM       (0x778) // 478x4
#define RO_MC_REQ_CNT           0x7B4
#define RW_MC_DDR_CTRL0         0x7FC
#define RW_MC_DDR_CTRL1         0x800
#define RW_MC_DDR3_EN           0x834 // 525x4
#define RW_MC_PITCH             0x980
#define RW_MC_PARA_BEHAVIOR     0x998
#define RO_MC_CYC_SYS           0x9E0
#define RO_MC_INTRA_CNT         0x9E4
#define RO_MC_YBLK_CNT          0x9E8
#define RO_MC_CBLK_CNT          0x9EC
#define RO_MC_WAIT_CNT          0x9F0
// *********************************************************************
// VLD TOP Registers define
// ********************************************************************
//#define VP8_ERROR_DETECT_TYPE (0x00000030)
#define VP8_ERROR_DETECT_TYPE (0x80001030)

#define RW_READY_SWITCH             0x00
#define RW_MDEC_TIMEOUT_CLR         0X04
#define RW_VLD_OK_CLR               0x08
#define RW_PRED_DRAM_B_ADDR         0x28 // 10x4
#define RW_PRED_SRAM_CFG            0x2C // 11x4
   #define RW_PRED_SRAM_CFG_OUTRANGE_SUPPORT_FLAG 31
   #define RW_PRED_SRAM_CFG_SRAM_BYPASS_FLAG 30
#define RW_SEGID_DRAM_ADDR          0x3c // 15x4
#define RW_PRED_MODE                0x40 // 16x4
   #define RW_NUM_ROW_SEGID_DATA_M1_S 0
   #define RW_NUM_ROW_SEGID_DATA_M1_E 2
   #define RW_DRAM_BURST_MODE_S 4
   #define RW_DRAM_BURST_MODE_E 5
   
#define RW_WEBP_CTRL                0x48 // 18x4
        #define RW_BUFCTRL_ON_FLAG  0
		#define RW_RESIZE_INTER_ON_FLAG 4
#define RW_MB_DECSTART              0x4c // 19x4
#define RW_MDEC_TIMEOUT_THRESHOLD   0x50 // 20x4
#define RW_MDEC_TIMEOUT_INT_SWITCH  0x54 // 21x4
#define RW_MC_BUSY_THRESHOLD        0x58 // 22x4
#define RW_ERR_CONCEALMENT          0x5c // 23x4
    #define RW_RETURN_IF_ERROR_FLAG 31
    #define RW_ERROR_SWITCH_S 28
    #define RW_ERROR_SWITCH_E 29
    #define RW_SLICE_RECONCEL_MODE_S 8
    #define RW_SLICE_RECONCEL_MODE_E 9
    #define RW_SLICE_RECONCEL_FLAG 0
    
#define RW_DEDERR_TYPE              0x60 // 24x4
#define RW_ERR_TYPE_ENABLE          0x64 // 25X4
#define RW_PIC_MB_SIZE_M1           0x68
    #define RW_PIC_WIDTH_IN_MBS_S   0
    #define RW_PIC_WIDTH_IN_MBS_E   9
    #define RW_PIC_HEIGHT_IN_MBS_S 16
    #define RW_PIC_HEIGHT_IN_MBS_E 25
#define RW_PIC_PIX_SIZE            0x70

// *********************************************************************
// VLD MISC Registers define
// ********************************************************************
#define RW_TOP_PDN     0x00
#define RW_CRC_CTRL    0x04
#define RW_CRC_Y_0     0x08
#define RW_CRC_Y_1     0x0C
#define RW_CRC_Y_2     0x10
#define RW_CRC_Y_3     0x14
#define RW_CRC_C_0     0x18
#define RW_CRC_C_1     0x1C
#define RW_CRC_C_2     0x20
#define RW_CRC_C_3     0x24
#define RW_MEMERR_CTRL 0x28
#define RW_VDEC_MISC_INT_CFG   0xA4
  #define INT_CFG_INT             (0x1<<16)
  #define INT_CFG_WAIT_BITS_NOP   (0x1<<12)
  #define INT_CFG_MASK            (0x1<<8)
  #define INT_CFG_INT_CLR         (0x1<<4)
  #define INT_CFG_CLR_INT_MD      (0x1<<0)


// *********************************************************************
// Interrupt Related Registers define
// *********************************************************************
#define AVLD_VECTOR 24

// *********************************************************************
// Function
// *********************************************************************

#define VDEC_VLD1 (0)
#define VDEC_VLD2 (1)
#define VP8_HWCOEFPROB_ACC_THRESOLD 10000
#define VP8_HWMVPROB_ACC_THRESOLD 10000
#define VP8_USE_SMALQT 

#ifdef VDEC_VP8HW_DEBUG
#define VDEC_VLD_TOP_READ32(offset)          _VDEC_VP8_IO_Read32(VLD_TOP_BASE, offset)
#define VDEC_VLD_TOP_WRITE32(offset,value)   _VDEC_VP8_IO_Write32(VLD_TOP_BASE, offset, value)
#define VDEC_VLD_READ32(offset)              _VDEC_VP8_IO_Read32(VLD_BASE, offset)
#define VDEC_VLD_WRITE32(offset,value)       _VDEC_VP8_IO_Write32(VLD_BASE, offset, value)
#define VDEC_VLD2_READ32(offset)             _VDEC_VP8_IO_Read32(VLD2_BASE, offset)
#define VDEC_VLD2_WRITE32(offset,value)      _VDEC_VP8_IO_Write32(VLD2_BASE, offset, value)
#define VDEC_MC_READ32(offset)               _VDEC_VP8_IO_Read32(MC_BASE, offset)
#define VDEC_MC_WRITE32(offset,value)        _VDEC_VP8_IO_Write32(MC_BASE, offset, value)
#define VDEC_MV_READ32(offset)               _VDEC_VP8_IO_Read32(VP8_MV_BASE, offset)
#define VDEC_MV_WRITE32(offset,value)        _VDEC_VP8_IO_Write32(VP8_MV_BASE, offset, value)
#define VDEC_PP_READ32(offset)               _VDEC_VP8_IO_Read32(PP_BASE,offset)
#define VDEC_PP_WRITE32(offset,value)        _VDEC_VP8_IO_Write32(PP_BASE,offset,value)
#define VP8_VLD_READ32(offset)               _VDEC_VP8_IO_Read32(VP8_VLD_BASE, offset)
#define VP8_VLD_WRITE32(offset, value)       _VDEC_VP8_IO_Write32(VP8_VLD_BASE, offset, value)
#define VP8_MV_READ32(offset)                _VDEC_VP8_IO_Read32(VP8_MV_BASE, offset)
#define VP8_MV_WRITE32(offset, value)        _VDEC_VP8_IO_Write32(VP8_MV_BASE, offset, value)
#define VDEC_MISC_READ32(offset)             _VDEC_VP8_IO_Read32(VDEC_MISC_BASE, offset)
#define VDEC_MISC_WRITE32(offset, value)     _VDEC_VP8_IO_Write32(VDEC_MISC_BASE, offset, value)
#define VDEC_RESIZE_READ32(offset)           _VDEC_VP8_IO_Read32(WEBP_IMGRZ_BASE,offset)
#else 
#define VDEC_VLD_TOP_READ32(offset)          IO_READ32(VLD_TOP_BASE, offset)
#define VDEC_VLD_TOP_WRITE32(offset, value)  IO_WRITE32(VLD_TOP_BASE, offset, value)
#define VDEC_VLD_READ32(offset)              IO_READ32(VLD_BASE, offset)
#define VDEC_VLD_WRITE32(offset, value)      IO_WRITE32(VLD_BASE, offset, value)
#define VDEC_VLD2_READ32(offset)             IO_READ32(VLD2_BASE, offset)
#define VDEC_VLD2_WRITE32(offset, value)     IO_WRITE32(VLD2_BASE, offset, value)
#define VDEC_MC_READ32(offset)               IO_READ32(MC_BASE, offset)
#define VDEC_MC_WRITE32(offset, value)       IO_WRITE32(MC_BASE, offset, value)
#define VDEC_MV_READ32(offset)               IO_READ32(VP8_MV_BASE, offset)
#define VDEC_MV_WRITE32(offset,value)        IO_WRITE32(VP8_MV_BASE, offset, value)
#define VDEC_PP_READ32(offset)               IO_READ32(PP_BASE,offset)
#define VDEC_PP_WRITE32(offset,value)        IO_WRITE32(PP_BASE,offset,value)
#define VP8_VLD_READ32(offset)               IO_READ32(VP8_VLD_BASE, offset)
#define VP8_VLD_WRITE32(offset, value)       IO_WRITE32(VP8_VLD_BASE, offset, value)
#define VP8_MV_READ32(offset)                IO_READ32(VP8_MV_BASE, offset)
#define VP8_MV_WRITE32(offset, value)        IO_WRITE32(VP8_MV_BASE, offset, value)
#define VDEC_MISC_READ32(offset)             IO_READ32(VDEC_MISC_BASE, offset)
#define VDEC_MISC_WRITE32(offset, value)     IO_WRITE32(VDEC_MISC_BASE, offset, value)
#define VDEC_RESIZE_READ32(offset)           IO_READ32(WEBP_IMGRZ_BASE,offset)
#endif
#define VDEC_GET_FLAGVAL(value,flg) (((value)&1)<<flg)
#define VDEC_GET_RANGEVAL(value,s,e) (((value)&((1<<(e-s+1))-1))<<s)
#define VDEC_REG_GET_VALUE(value,s,e) (((value)>>s)&((1<<(e-s+1))-1))
#define VDEC_REG_SET_VALUE(reg,value,s,e) (reg=(reg&(~((1<<(e-s+1))<<s)))|(VDEC_GET_RANGEVAL(value,s,e)))
#define VDEC_REG_GET_FLAG(value,p) (((value)>>p)&0x1)
#define VDEC_REG_SET_FLAG(reg,value,p)  (reg=(reg&(~(1<<p)))|(VDEC_GET_FLAGVAL(value,p)))
#define VDEC_VP8_DQINDEXCLAMP(index) (index>127 ? 127 : (index<0 ? 0 : index))

#define RO_VP8_VBSR1 0        // 0--32x4  Barrel shift read
#define RO_VP8_VRLR 0x84     // 33--40x4 read literal
#define RO_VP8_VBSR2 0x200   // 128--160x4 second barrel shift read
#define RW_VP8_CTRL   0xA4   // 41x4 control register
   #define RW_VP8_VP8FLAG 0
   #define RO_VP8_SUM1_S 8
   #define RO_VP8_SUM1_E 13
   #define RO_VP8_SUM2_S 16
   #define RO_VP8_SUM2_E 21

#define RW_VP8_HEADER2 0xA8  // 42x4     
   #define RW_VP8_PICTYPE_FLAG 0
   #define RW_VP8_VERSION_S 1
   #define RW_VP8_VERSION_E 3
   #define RW_VP8_FSTPB_S 8
   #define RW_VP8_FSTPB_E 26

#define RW_VP8_HEADER3 0XAC  // 43x4
   #define RW_VP8_CLTYPE_FLAG 0
   #define RW_VP8_SEGEN_FLAG 4
   #define RW_VP8_UMBSM_FLAG 8
   #define RW_VP8_MTP_S 12
   #define RW_VP8_MTP_E 13
   #define RW_VP8_MBSKIP_FLAG 16

#define RW_VP8_PICPROB 0XB0  // 44x4
   #define RW_VP8_PSKIPF_S 0
   #define RW_VP8_PSKIPF_E 7
   #define RW_VP8_PINTRA_S 8
   #define RW_VP8_PINTRA_E 15
   #define RW_VP8_PLAST_S 16
   #define RW_VP8_PLAST_E 23
   #define RW_VP8_PGF_S 24
   #define RW_VP8_PGF_E 31

#define RW_VP8_FWPROB 0XB4 // 45x4 0--7 bit the probability of firmware read_lit(n)
#define RW_VP8_HDR  0XB8  // 46x4 0 bit triger start decode

#define RW_VP8_COEFR0 0XC0  //48X4
#define RW_VP8_COEFR1 0XC4
#define RW_VP8_COEFR2 0XC8
#define RW_VP8_COEFR3 0XCC
#define RW_VP8_COEFR4 0XD0
#define RW_VP8_COEFR5 0XD4
#define RW_VP8_COEFR6 0XD8
#define RW_VP8_COEFR7 0XDC

#define RW_VP8_COEFPB0 0xE0  // 56 X4
#define RW_VP8_COEFPB1 0XE4
#define RW_VP8_COEFPB2 0XE8
#define RW_VP8_COEFPB3 0XEC
#define RW_VP8_COEFPB4 0XF0
#define RW_VP8_COEFPB5 0XF4
#define RW_VP8_COEFPB6 0XF8
#define RW_VP8_COEFPB7 0XFC

#define RW_VP8_BSASET 0X100 // 64X4   0--8 bit   Bus_SRAM address setting
#define RW_VP8_BSDSET 0X104 //65x4    0--31bit  Bus_SRAM write data setting
#define RW_VP8_CSASET 0X108 // 66X4  0--7bit  CTX_SRAM address setting
#define RW_VP8_CSDSET 0X10C // 67x4  0--31bit CTX_SRAM write data setting

#define RO_VP8_BINIT1  0X110 //68x4 0--31bit bool0 init
#define RO_VP8_BINIT2  0X114 //69x4 0--31bit bool1 init
#define RW_VP8_PART_CHG_PAUSE 0x118  //70x4
#define RW_VP8_DEC_RESUME  0X11C  //71x4
#define RW_VP8_MVPROB 0X120  //72x4
    #define RW_VP8_PMVISX_S 0
    #define RW_VP8_PMVISX_E 7
    #define RW_VP8_PMVISY_S 8
    #define RW_VP8_PMVISY_E 15
    #define RW_VP8_PMVSX_S 16
    #define RW_VP8_PMVSX_E 23
    #define RW_VP8_PMVSY_S 24
    #define RW_VP8_PMVSY_E 31

#define RO_VP8_VLD1_BITCNT 0X138  //78x4
#define RO_VP8_PART_BITCNT 0X13C // 79x4

#define RW_VP8_STR 0X140  // 80x4
    #define RW_VP8_STR1_FLAG 0
    #define RW_VP8_STR2_FLAG 8

#define RW_VP8_BCRT 0X144 // 81x4 
    #define RW_VP8_BCRT1_FLAG 0
    #define RW_VP8_BCRT2_FLAG 8
    
#define RW_VP8_CPUT 0X148   //82x4  0bit trigger coef_prob_update
#define RW_VP8_MVPUT 0X14C  //83x4 0bit trigger mv_prob_update 
#define RO_VP8_PICFSM 0X150 //84x4 0--18bit  pic state
#define RO_VP8_MBFSM 0X154  //85x4 0--22 mb_state
#define RO_VP8_COEFFSM 0X158 //86x4 0--2bit coeff_dec_state

#define RO_VP8_ERTR 0X15C  //87x4 0--31 error type register
   #define RO_VP8_ERR_FSTPARTERR 4
   #define RO_VP8_ERR_SECPARTERR 5
   #define RO_VP8_ERR_MC_TIMEOUT    12
   #define RO_VP8_ERR_VDEC_TIMEOUT  31
   
#define RO_VP8_ERRXY 0X160  //88x4  
    #define RO_VP8_ERRY_S 0
    #define RO_VP8_ERRY_E 8
    #define RO_VP8_ERRX_S 16
    #define RO_VP8_ERRX_E 24

#define RO_VP8_RECKSM 0X164 //89x4 0--31bite residual check sum
#define RO_VP8_LTHR1 0X168  //90x4 0--31 barrel_shifter_latch2
#define RO_VP8_LTHR2 0X16C  //91x4 0--31 second barrel_shifter_latch2

#define RO_VP8_VOKR 0X170 //92x4 0bit vld ok :1 means that vld decode whole picture
#define RO_VP8_PFR  0X174 // 93x4 0bit  picture finish: 1 means that vdec decode whole picture

#define RO_VP8_STATUS 0X178 // 94X4

#define RO_VP8_XY 0X17C //95x4
    #define RO_VP8_CURRY_S 0
    #define RO_VP8_CURRY_E 8
    #define RO_VP8_CURRX_S 16
    #define RO_VP8_CURRX_E 24
#define RO_VP8_HWA 0X180 // 96X4 0bit hw is in process

// MV Register
#define RW_VP8_MV_RFSB 0X2D4  // 181x4

//MC Register
#define RW_MC_PIC1Y_ADD 0X3E0  // 248x4
#define RW_MC_PIC2Y_ADD 0X3E4
#define RW_MC_PIC3Y_ADD 0X3E8
#define RW_MC_LUMA_SIZE 0X934  //589x4
#define RW_MC_VP8SETTING 0X97C //607x4
    #define RW_MC_VP8FULLPEL_FLAG 8
    #define RW_MC_BILINEAR_OR_SIXTAP_FLAG 4
    #define RW_MC_SUPERBITSCAN_OFF_FLAG 0

//PP Register
#define RW_PP_VP8PARA  0X20 // 8x4
    #define RW_PP_VP8ENABLE_FLAG 0
    #define RW_PP_FLTTYPE_FLAG 4  // 0:normal 1: simple
    #define RW_PP_MODEREF_DELTA_ENABLE_FLAG 8
    #define RW_PP_FRAMETYPE_FLAG 12 // 0:Key 1:Inter
    #define RW_PP_SHARPLVL_S 16
    #define RW_PP_SHARPLVL_E 18
#define RW_PP_VP8FILTER_LEVEL 0X24 // 9X4
    #define RW_PP_FILTER_LV0_S 0
    #define RW_PP_FILTER_LV0_E 5
    #define RW_PP_FILTER_LV1_S 8
    #define RW_PP_FILTER_LV1_E 13
    #define RW_PP_FILTER_LV2_S 16
    #define RW_PP_FILTER_LV2_E 21
    #define RW_PP_FILTER_LV3_S 24
    #define RW_PP_FILTER_LV3_E 29
#define RW_PP_VP8REFDELTA 0X28 // 10x4
#define RW_PP_VP8MODEDELTA 0X2C // 11x4
    #define RW_PP_DELTA_ID0_S 0
    #define RW_PP_DELTA_ID0_E 6
    #define RW_PP_DELTA_ID1_S 8
    #define RW_PP_DELTA_ID1_E 14
    #define RW_PP_DELTA_ID2_S 16
    #define RW_PP_DELTA_ID2_E 22
    #define RW_PP_DELTA_ID3_S 24
    #define RW_PP_DELTA_ID3_E 30

#define RW_PP_REG_17  0x44  // 17x4
#define RW_PP_BRC_OFFSET 0x70 // 28x4

#ifdef USE_VP68_SW_VLD 
extern VOID _VDEC_VP68_InitSwBoolDecoder(UINT32 u4VldId,UINT32 u4ReadPtr,
   UINT32 u4BufStart,UINT32 u4BufEnd);
extern VOID _VDEC_VP68_InitSwBarrelShifter(UINT32 u4VldId,UINT32 u4ReadPtr,
    UINT32 u4BufStart,UINT32 u4BufEnd);
#endif

#ifdef VDEC_VP8HW_DEBUG
extern VOID _VDEC_VP8_IO_Log(BOOL fgOnOff);
extern BOOL _VDEC_VP8_IO_LogOpen(VOID);
extern VOID _VDEC_VP8_IO_Write32(UINT32 u4Base, UINT32 u4Offset,UINT32 u4Value);
extern UINT32 _VDEC_VP8_IO_Read32(UINT32 u4Base, UINT32 u4Offset);
#endif

extern BOOL   _VDEC_VP8_InitBarrelShifter(UINT32 u4VldId,UINT32 u4ReadPointer, UINT32 u4WritePointer);
extern UINT32 _VDEC_VP8_InitBoolDecoder(UINT32 u4VldId);
extern UINT32 _VDEC_VP8_VLDShiftBit(UINT32 u4VldId,UINT32 u4ShiftBits);
extern UINT32 _VDEC_VP8_VLDReadLiteral(UINT32 u4Bits);
extern UINT32 _VDEC_VP8_VLDReadBit(UINT32 u4Prob);
extern VOID _VDEC_VP8_VLD_PowerOff(VOID);
extern VOID _VDEC_VP8_VLDReset(UINT32 u4VldId);
extern UINT32 _VDEC_VP8_VLDGetReadPtr(UINT32 *pu4Bits);
extern VOID _VDEC_VP8_VLDSetWritePtr(UINT32 u4WritePtr);
extern VOID _VDEC_VP8_HwAccMVProbUpdate(VOID);
extern VOID _VDEC_VP8_HwAccCoefProbUpdate(VOID);

#endif
