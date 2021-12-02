
/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright(c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_vp6.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp6.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_VP6_HW_H
#define VDEC_VP6_HW_H

#include "x_lint.h"
#include "x_hal_5381.h"
#include "x_bim.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
#include "vdec_vp6util.h"
LINT_EXT_HEADER_END
;

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
// *********************************************************************
// Video Decoder Registers define
// *********************************************************************
#define VP6_BASE        (0x0B000)

#define VLD_TOP_BASE    (IO_VIRT + 0x02800)
#define VLD_BASE        (IO_VIRT + 0x02000)
#define VLD2_BASE       (IO_VIRT + VP6_BASE + 0x800)
#define MC_BASE         (IO_VIRT + 0x0A000)
#define PP_BASE         (IO_VIRT + 0x2F000)
#define VP6_VLD_BASE    (IO_VIRT + VP6_BASE)
#define VP6_VLD2_BASE   (IO_VIRT + VP6_BASE + 0x400)
#define VP6_MV_BASE     (IO_VIRT + 0x31000)
#define VDEC_MISC_BASE  (IO_VIRT + 0x03000)

#define VLD_TOP_READ32(offset)          _VDEC_VP6_IO_Read32(VLD_TOP_BASE, offset)
#define VLD_TOP_WRITE32(offset, value)  _VDEC_VP6_IO_Write32(VLD_TOP_BASE, offset, value)

#define VLD_READ32(offset)              _VDEC_VP6_IO_Read32(VLD_BASE, offset)
#define VLD_WRITE32(offset, value)      _VDEC_VP6_IO_Write32(VLD_BASE, offset, value)

#define VLD2_READ32(offset)             _VDEC_VP6_IO_Read32(VLD2_BASE, offset)
#define VLD2_WRITE32(offset, value)     _VDEC_VP6_IO_Write32(VLD2_BASE, offset, value)

#define MC_READ32(offset)               _VDEC_VP6_IO_Read32(MC_BASE, offset)
#define MC_WRITE32(offset, value)       _VDEC_VP6_IO_Write32(MC_BASE, offset, value)

#define PP_READ32(offset)               _VDEC_VP6_IO_Read32(PP_BASE, offset)
#define PP_WRITE32(offset, value)       _VDEC_VP6_IO_Write32(PP_BASE, offset, value)

#define VP6_VLD_READ32(offset)          _VDEC_VP6_IO_Read32(VP6_VLD_BASE, offset)
#define VP6_VLD_WRITE32(offset, value)  _VDEC_VP6_IO_Write32(VP6_VLD_BASE, offset, value)

#define VP6_VLD2_READ32(offset)         _VDEC_VP6_IO_Read32(VP6_VLD2_BASE, offset)
#define VP6_VLD2_WRITE32(offset, value) _VDEC_VP6_IO_Write32(VP6_VLD2_BASE, offset, value)

#define VP6_MV_READ32(offset)           _VDEC_VP6_IO_Read32(VP6_MV_BASE, offset)
#define VP6_MV_WRITE32(offset, value)   _VDEC_VP6_IO_Write32(VP6_MV_BASE, offset, value)

#define VDEC_MISC_READ32(offset)         _VDEC_VP6_IO_Read32(VDEC_MISC_BASE, offset)
#define VDEC_MISC_WRITE32(offset, value) _VDEC_VP6_IO_Write32(VDEC_MISC_BASE, offset, value)

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

#define WO_VLD_SRST      0x108
#define WO_VLD_WPTR      0x110
    #define WPTR_LSH_BITS  3
    #define VLD_EWPL        ((UINT32)0x1 << 2)
    #define VLD_RISC_WR_EN      ((UINT32)0x1 << 1)
    #define VLD_CLEAR_PROCESS_EN ((UINT32)0x1 << 0)

#define RO_VLD_SUM              0x114
#define RW_VLD_VP6_ABS  0x23C

#define RW_VLD_SCL_ADDR      0x260 
    #define SCL_WRITE        (0x0ul << 8)
    #define SCL_READ         (0x1ul << 8)
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
#define RW_MC_PP_DBK_FLR         0x658
    #define PARTIAL             (0x1 << 0)
    #define INTERLACE           (0x1 << 1)
    #define ENGR                (0x1 << 2)
    #define SKIPMB              (0x1 << 3)
    #define BYPASSWRAP          (0x1 << 4)
    #define WRAPIDX             (0x1 << 5)
#define RW_MC_VDEC_LARB         0x660
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
#define RW_MC_PP_QPBPT          0x700
#define RW_MC_PP_DBKATHR        0x704
#define RW_MC_WRAPPER   0x71C
    #define PP_WRAPPER_Y_LINES ((UINT32)0x1 << 27)
    #define PP_WRAPPER_C_LINES ((UINT32)0x1 << 31)

#define RW_MC_REQ_MAX_LEN 0x748
    #define MAX_LEN_EN  ((UINT32)0x1 << 20)

#define RW_MC_RR_FMT    0x74C   //field mode
    #define RR_FIELD_DISP_MODE 0xF

#define RW_MC_NBM_CTRL         0x768
    #define RW_MC_NBM_OFF      (0x1 << 8)
#define RO_MC_DLE_NUM           0x774
#define RO_MC_DECODING_CYCLE    0x778
#define RO_MC_REQ_CNT           0x7B4

#define RW_MC_DDR_CTRL0         0x7FC
#define RW_MC_DDR_CTRL1         0x800
#define RW_MC_DDR3_EN           0x834
#define RW_MC_VLD_WRAPPER_ADDR  0x93C
    #define MC_VLD_WRAPPER_READ     (0x0ul<<16)
    #define MC_VLD_WRAPPER_WRITE    (0x1ul<<16)
#define RW_MC_VLD_WRAPPER_DATA  0x940
#define RW_MC_PITCH             0x980

#define RW_MC_PARA_BEHAVIOR     0x998
    #define PARA4_BEHAVIOR      ((UINT32)0x1 << 0)
    #define PARA8_BEHAVIOR      ((UINT32)0x1 << 4)
#define RO_MC_CYC_SYS           0x9E0
#define RO_MC_INTRA_CNT         0x9E4
#define RO_MC_Y_BLK_CNT         0x9E8
#define RO_MC_C_BLK_CNT         0x9EC
#define RO_MC_WAIT_CNT          0x9F0

// *********************************************************************
// VP6 VLD Registers define
// *********************************************************************
//Picture Layer Parameter
// Barral Shifter from 0x00 - 0x80
#define RO_VP6_VLD_BARL             0x00
#define RW_VP6_INIT_BOOL            0x84
    #define RW_VP6_BOOL_EN          (1<<0)
#define RW_VP6_BCPR                 0x88
#define RW_VP6_PCI_PAR              0x8C
    #define RW_VP6_FLAG             (1<<0)
    #define RW_VP6_KEY_FRM          (1<<1)
    #define RW_VP6_RTN_ERR			(1<<2)
    #define RW_VP6_EERE             (1<<3)
    #define RW_VP6_DAEC             (1<<4)
    #define RW_VP6_HUFFMAN          (1<<5)
    #define RW_VP6_MULTI            (1<<6)
#define RW_VP6_GBFR                 0x94
    #define GRDY                    (1<<0)
#define RW_VLD_VP6DEC               0xA0
    #define VP6_DEC_START           (1<<0)
#define RO_VP6_PIC_DEC_END          0xA8
    #define VP6_VPDF                (1<<0)

#define RW_MBT_B_ADDR               0xB0
#define RW_MVD_B_ADDR               0xB4
#define RW_CFM_B_ADDR               0xB8
#define RW_PRED_NR                  0xBC
#define RW_CONTEXT_NR               0xC0

#define RW_VP6_VLD_VICR             0xC4
    #define VP6_VPIC                (1<<0)
#define RW_VP6_VLD_WARR             0xC8
    #define VP6_CTX_WARR_READ       (0x0ul<<8)
    #define VP6_CTX_WARR_WRITE      (0x1ul<<8)
    #define VP6_CTX_WARR_BOOL       (0x0ul<<12)
    #define VP6_CTX_WARR_HUFFMAN    (0x1ul<<12)
#define RW_VP6_VLD_FCVR1            0xCC
#define RW_VP6_VLD_FCVR2            0xD0
#define RW_VP6_VLD_FCVR3            0xD4
#define RW_VP6_VLD_FCVR4            0xD8
#define RW_VP6_VLD_FCVR5            0xDC
#define RW_VP6_VLD_FCVR6            0xE0
#define RW_DCAC_TIMEOUT_THD         0xE4
#define RW_MV_TIMEOUT_THD           0xE8
#define RW_MC_TMIEOUT_THD           0xEC
#define RO_VP6_VLD_VBIW             0xF0
#define RO_VP6_VLD_ERR              0x128
#define RW_VP6_VLD_WRAR             0x12C
#define RO_VP6_VLD_WRD0             0x130
#define RO_VP6_VLD_WRD1             0x134
#define RO_VP6_VLD_WRD2             0x138
#define RO_VP6_VLD_WRD3             0x13C
#define RW_VP6_VLD_PROB_UPD         0x140
#define RO_VP6_VLD_ERR_EX           0x144
#define RW_VP6_PAD_ZERO_MODE        0x14C
#define RW_VP6_PIC_BYTE_CNT         0x150
#define RO_VP6_BYTE_CNT             0x154
#define RW_VP6_VLD_ETCB              0x158
#define RW_VP6_VLD_ETCA              0xDC
    #define ETC_COEFF_ERROR         (0x1<<0)
    #define ETC_MC_TIMEOUT          (0x1<<1)
    #define ETC_MV_TIMEOUT          (0x1<<2)
    #define ETC_DCAC_TIMEOUT        (0x1<<3)
    #define ETC_HUFF_ERR            (0x1<<4)
#define RW_VP6_PAD_ZERO_MODE2       0x164
#define RW_VP6_PIC_BYTE_CNT2        0x168

#define RW_VP6_QUANT_REG            0x500
#define RW_VP6_BKDOOR               0x85C
    #define VP6_ADOBE_EN            (1<<12)
    #define VP6_2FLIMIT_ZERO        (1<<8)
    #define VP6_DBK_START           (1<<4)
    #define VP6_CBCR_MAKE_RND       (1<<0)
#define RW_VP6_LOOPDBK              0x860
#define RW_VP6_MV_TH                0x864
#define RW_VP6_AUTOSEL              0x868
#define RW_VP6_COEF0_P1             0x86C
#define RW_VP6_COEF0_P2             0x870
#define RW_VP6_COEF1_P1             0x874
#define RW_VP6_COEF1_P2             0x878
#define RW_VP6_COEF2_P1             0x87C
#define RW_VP6_COEF2_P2             0x880
#define RW_VP6_COEF3_P1             0x884
#define RW_VP6_COEF3_P2             0x888
    
   
// *********************************************************************
// VLD TOP Registers define
// *********************************************************************
#define RW_PRED_DRAM_B_ADDR         0x28
#define RW_PIC_MB_SIZE_M1           0x68
#define RW_PIC_PIX_SIZE             0x70

// *********************************************************************
// VDEC TOP Registers define
// *********************************************************************
#define RW_VDEC_MISC_CRC_CON         0x4
    #define CRC_CON_ENABLE          (0x1<<0)
    #define CRC_CON_AGENT_MC        (0x0<<1)
    #define CRC_CON_AGENT_PP        (0x1<<1)
#define RW_VDEC_MISC_CRC_YDATA0      0x8
#define RW_VDEC_MISC_CRC_YDATA1      0xC
#define RW_VDEC_MISC_CRC_YDATA2      0x10
#define RW_VDEC_MISC_CRC_YDATA3      0x14
#define RW_VDEC_MISC_CRC_CDATA0      0x18
#define RW_VDEC_MISC_CRC_CDATA1      0x1C
#define RW_VDEC_MISC_CRC_CDATA2      0x20
#define RW_VDEC_MISC_CRC_CDATA3      0x24
#define RW_VDEC_MISC_INT_CFG         0xA4
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
// SRAM Addr
// *********************************************************************
#define VLD_WRAPPER_MP_TYPE_PROB    840
#define VLD_WRAPPER_UPD_MP_TYPE 936
#define VLD_WRAPPER_DC_Y        945
#define VLD_WRAPPER_DC_C        946
#define VLD_WRAPPER_RUN_0       947
#define VLD_WRAPPER_RUN_1       948
#define VLD_WRAPPER_AC_BASE     949 //949~984
#define VLD_WRAPPER_AC_BASE_END 984

#define CTX_BOOL_DC_Y           0
#define CTX_BOOL_DC_C           1
#define CTX_BOOL_RUN_0          2
#define CTX_BOOL_RUN_1          3
#define CTX_BOOL_AC_BASE        4   //4~39

#define CTX_HUFF_AC_BASE        0   //0~23
#define CTX_HUFF_DC_Y           24
#define CTX_HUFF_DC_C           25
#define CTX_HUFF_RUN_0          26
#define CTX_HUFF_RUN_1          27
#define CTX_HUFF_EOB_RUN        28

// *********************************************************************
// Function
// *********************************************************************
extern void _VDEC_VP6_LogSimPat(BOOL bLogEn);
extern UINT32 _VDEC_VP6_IO_Read32(UINT32 u4Base, UINT32 u4Offset);
extern void _VDEC_VP6_IO_Write32(UINT32 u4Base, UINT32 u4Offset, UINT32 u4Value);

extern void vVDecWriteVLDTop(UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVLDTop(UINT32 u4Addr);

extern void vVDecWriteVLD(UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVLD(UINT32 u4Addr);
extern void vVDecWriteVLDEx(UINT32 u4Id, UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVLDEx(UINT32 u4Id, UINT32 u4Addr);
extern void vVDecWriteMC(UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadMC(UINT32 u4Addr);

extern void vVDecWriteVP6VLD(UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVP6VLD(UINT32 u4Addr);
extern void vVDecWriteVP6VLDEx(UINT32 u4Id, UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVP6VLDEx(UINT32 u4Id, UINT32 u4Addr);
extern void vVDecWriteVP6MV(UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVP6MV(UINT32 u4Addr);

extern UINT32 u4VDecVP6VLDGetBits(UINT32 u4Id, UINT32 dShiftBit);
extern UINT32 u4VDecWaitVP6GetBitsReady(void);
extern UINT32 u4VDecVP6BOOLGetBits(UINT32 dShiftBit);
extern void vVDecWriteVP6PP(UINT32 u4Addr, UINT32 u4Val);
extern void vVDecWriteVDECMisc(UINT32 u4Addr, UINT32 u4Val);
extern UINT32 u4VDecReadVDECMisc(UINT32 u4Addr);

extern void vVDecSetVLDVFIFO(UINT32 u4Id, UINT32 u4VFifoSa, UINT32 u4VFifoEa);
extern BOOL fgVDecWaitVldFetchOk(UINT32 u4Id);
extern UINT32 u4VDecReadVP6VldRPtr(UINT32 *pu4Bits);
extern void _VDEC_VP6VLDHWReset(UINT32 u4Id);
extern void _VDEC_SetVP6VldWPtr(UINT32 u4Id, UINT32 u4WPtr);

extern void _VDEC_VP6PowerDownSetting(void);

    #ifdef CC_VP6_EMULATION
extern void _VDEC_VP6DumpChksumReg(void);
    #endif

#endif
