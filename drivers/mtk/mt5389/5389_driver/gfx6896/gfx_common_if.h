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
 * $Author: cheng.chen $
 * $Date: 2012/07/06 $
 * $RCSfile: gfx_common_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_common.h
 *  This header file declares public function prototypes of GFX.
 */
#ifndef __GFX_COMMON_IF_H__
#define __GFX_COMMON_IF_H__

typedef struct _GFX_MMU_T_
{
    unsigned int u4_init;                     // set: mmu reg to init value
    unsigned int u4_enable;                   // para: of mmu enable/disable  if do init.
    unsigned int u4_cmprss;                   // set:compress to dst
    unsigned int u4_src_rw_mmu;               // set:src mmu enable/disable
    unsigned int u4_dst_rw_mmu;               // set:dst mmu enable/disable
    unsigned int u4_vgfx_ord;                 // set:vgfx overread 
    unsigned int u4_vgfx_slva;             // para:  vgfx src last valid address
    unsigned int u4_pgt;                      // set: pate table address    
} GFX_MMU_T;


typedef struct _VGFX_MATRIX_T
{
    unsigned int          u4_a;
    unsigned int          u4_b;
    unsigned int          u4_c;
    unsigned int          u4_d;
    unsigned int          u4_e;
    unsigned int          u4_f;
    unsigned int          u4_g;
    unsigned int          u4_h;
    unsigned int          u4_i;
}VGFX_MATRIX_T;

typedef struct _VGFX_COEFF_4_TAP_T
{
    unsigned int u4Coef00;
    unsigned int u4Coef01;
    unsigned int u4Coef10;
    unsigned int u4Coef11;
    unsigned int u4Coef20;
    unsigned int u4Coef21;
    unsigned int u4Coef30;
    unsigned int u4Coef31;
    unsigned int u4Coef40;
    unsigned int u4Coef41;
    unsigned int u4Coef50;
    unsigned int u4Coef51;
    unsigned int u4Coef60;
    unsigned int u4Coef61;
    unsigned int u4Coef70;
    unsigned int u4Coef71;
    unsigned int u4Coef80;
    unsigned int u4Coef81;
    unsigned int u4Coef90;
    unsigned int u4Coef91;
    unsigned int u4CoefA0;
    unsigned int u4CoefA1;
    unsigned int u4CoefB0;
    unsigned int u4CoefB1;
    unsigned int u4CoefC0;
    unsigned int u4CoefC1;
    unsigned int u4CoefD0;
    unsigned int u4CoefD1;
    unsigned int u4CoefE0;
    unsigned int u4CoefE1;
    unsigned int u4CoefF0;
    unsigned int u4CoefF1;
}VGFX_COEFF_4_TAP;

typedef enum 
{
    E_AC_CLEAR = 0,
    E_AC_DST_IN,
    E_AC_DST_OUT,
    E_AC_DST_OVER,
    E_AC_SRC,
    E_AC_SRC_IN,
    E_AC_SRC_OUT,
    E_AC_SRC_OVER,
    E_AC_DST,
    E_AC_SRC_ATOP,
    E_AC_DST_ATOP,
    E_AC_XOR,
    E_AC_NONE,
    E_AC_ADD,
} GFX_AC_MODE_T;


typedef enum 
{
    VGX_ALPHA_EDGE_NEAREST,   // argb nearest neighbor
    VGX_ALPHA_EDGE_ZERO,   // a=0, rgb nearest neighbor
    VGX_ALPHA_EDGE_NORMAL,  //normal 
    VGX_ALPHA_EDGE_MAX
} ALPHA_EDGE_MD;              

typedef enum 
{
    VGFX_FILTER_2TAP,   
    VGFX_FILTER_4TAP,  
} VGFX_FILTER_T;   

typedef struct _VGFX_ALPHACOMP_SET_T
{
    unsigned int u4AlphaCompEn;
    unsigned int u4AlphaCompAr;
    unsigned int u4PreSrcRd;
    unsigned int u4PreDstRd;
    unsigned int u4PreDstWt;
    char fgAlphaCompNormal; 
    ALPHA_EDGE_MD _AlphaEdgeMd;
    GFX_AC_MODE_T u4AlphaCompOpCode;
} VGFX_ALPHACOMP_SET_T;

typedef struct _VGFX_WB_SET_T
{
    unsigned int u4BaseAddr;
    unsigned int u4ColrMode; 
    unsigned int u4WBWidth;
    unsigned int u4WBHeight;
    unsigned int u4GlAr;
    unsigned int u4Pitch;
} VGFX_WB_SET_T;

typedef struct _VGFX_QUADRILATERAL_SET_T
{
    unsigned int x1;
    unsigned int y1; 
    unsigned int x2;
    unsigned int y2; 
    unsigned int x3;
    unsigned int y3; 
    unsigned int x4;
    unsigned int y4; 
} VGFX_QUADRILATERAL_SET_T;

typedef struct _VGFX_IMGAGE_SET_T
{
    unsigned int u4BaseAddr;
    unsigned int u4ColrMode; 
    unsigned int u4GlAr;
    unsigned int u4Pitch;
} VGFX_IMGAGE_SET_T;

typedef struct _VGFX_TEXTURE_SET_T
{
    unsigned int u4StartX;
    unsigned int u4StartY;
    unsigned int u4EndX;
    unsigned int u4EndY;
} VGFX_TEXTURE_SET_T;

typedef struct _VGFX_BB_SET_T
{
    unsigned int u4StartX;
    unsigned int u4StartY;
    unsigned int u4Width;
    unsigned int u4Height;
} VGFX_BB_SET_T;

typedef struct _VGFX_RECT_SET_T
{
    unsigned int u4StartX;
    unsigned int u4StartY;
    unsigned int u4Width;
    unsigned int u4Height;
} VGFX_RECT_SET_T;

typedef struct _VGFX_REFLECT_SET_T
{
    unsigned int u4RefEn;
    unsigned int u4RefOnly;
    unsigned int u4TextTureHeight;
    unsigned int u4RefRatioInit;
    unsigned int u4RatioLineStep;
    unsigned int u4RefAAEn;
} VGFX_REFLECT_SET_T;

typedef struct _VGFX_FILTER_SET_T
{
    unsigned int u4FilterEn;
    VGFX_FILTER_T _FilterType;
} VGFX_FILTER_SET_T;

typedef struct _VGFX_SCALER_PARAM_SET_T
{
    unsigned int u4AntiAliasing;
    VGFX_WB_SET_T _WBType;
    VGFX_IMGAGE_SET_T _IMAGType;
    VGFX_BB_SET_T _BBType;
    VGFX_MATRIX_T _SetMartix;

    VGFX_QUADRILATERAL_SET_T _SrcQuard;
    VGFX_QUADRILATERAL_SET_T _DstQuard;
    
    VGFX_REFLECT_SET_T _SetReflect;
    VGFX_ALPHACOMP_SET_T _SetAlphaComp;
    VGFX_FILTER_SET_T _SetFilter;
    GFX_MMU_T         mmu;
} VGFX_SCALER_PARAM_SET_T;

#if defined(CC_MT8223)
#define GFX_RISC_MODE
#endif

//#define GFX_RISC_MODE

#define GFX_CC_YC

#if defined(CC_DEBUG)   // debug mode
    #define GFX_DEBUG_MODE
#endif


#if defined(CC_MINI_DRIVER)
    //#define GFX_MINI_DRIVER
    //#define GFX_RISC_MODE
#endif


#if defined(GFX_DEBUG_MODE)
    #define GFX_ENABLE_SW_MODE
    //#define GFX_RISC_MODE
#endif

#define GFX_CC_YC

// #define GFX_MMU_OPEN

#define CC_GFX_COMPOSITION_IMPROVE


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define GFX_CRASH_DETECT_PERIOD    (10000)  // 10 sec


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef struct _GFX_COLOR_CONVERT_T
{
    unsigned char * pu1SrcAddr;
    unsigned char * pu1DstAddr;
    unsigned char * pu1CbCr;
    unsigned int u4Width;
    unsigned int u4Height;
    unsigned int u4DstX;
    unsigned int u4DstY;
    unsigned int u4DstPitch;
    unsigned char  u1Alpha;
    unsigned int u4DstCM;
    GFX_MMU_T t_mmu;
    
} GFX_COLOR_CONVERT_T;

typedef struct _GFX_FLTR_CBCR_T
{
    unsigned int          ui4_cbcr_addr;     /* cbcr physical addr */
    unsigned int          ui4_src_width;     /* cbcr width  */
    unsigned int          ui4_src_height;    /* cbcr height  */
    unsigned int          ui4_src_pitch;     /* cbcr pitch,must >= 0x20 and divisible  by 0x10  */
    unsigned int          ui4_cb_addr;       /* cb physical addr */
    unsigned int          ui4_cr_addr;       /* cr physical addr */
} GFX_FLTR_CBCR_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

// GFX assertion
#define GFX_ASSERT(x)           ASSERT(x)

// Check pitch align
#define GFX_CHK_PITCH(pitch)    GFX_ASSERT(!(pitch & 0xF))

#define GFX_CHK_64B_PITCH(pitch)    GFX_ASSERT(!(pitch & 0x3F))

//Check pitch align
#define GFX_CHK_8B_ALIGN(val)   GFX_ASSERT(!(val&7))

// Check 32-byte (256-bit) align
#define GFX_CHK_32B_ALIGN(val)    GFX_ASSERT(!(val & 0x1F))
//check 16-byte align
#define GFX_CHK_16B_ALIGN(pitch)    GFX_ASSERT(!(pitch & 0xF))

//cgeck 64-byte align
#define GFX_CHK_64B_ALIGN(val)    GFX_ASSERT(!(val & 0x3F))


// Ignore return value (for lint happy)
#define GFX_UNUSED_RET(X)           \
    {                               \
        int i4Ignore;             \
        i4Ignore = (int)(X);      \
        UNUSED(i4Ignore);           \
    }


/// String size
#define GFX_STR_SIZE            50

/// Bitblt option flag : transparent
#define D_GFXFLAG_TRANSPARENT   (1 << (int)E_GFXBLT_TRANSPARENT)

/// Bitblt option flag : keynot
#define D_GFXFLAG_KEYNOT        (1 << (int)E_GFXBLT_KEYNOT)

/// Bitblt option flag : color change
#define D_GFXFLAG_COLORCHANGE   (1 << (int)E_GFXBLT_COLORCHANGE)

/// Bitblt option flag : color format translation
#define D_GFXFLAG_CFMT_ENA      (1 << (int)E_GFXBLT_CFMT_ENA)

/// Bitblt option flag : keysdsel
#define D_GFXFLAG_KEYSDSEL      (1 << (int)E_GFXBLT_KEYSDSEL)

/// Bitblt option flag : flip
#define D_GFXFLAG_FLIP          (1 << (int)E_GFXBLT_FLIP)

/// Bitblt option flag : mirror
#define D_GFXFLAG_MIRROR        (1 << (int)E_GFXBLT_MIRROR)

/// Bitblt option flag : overlap
#define D_GFXFLAG_OVERLAP        (1 << (int)E_GFXBLT_OVERLAP)

/// Bitblt option flag : none
#define D_GFXFLAG_NONE          (0)

/// Software mode
#define GFX_HAVE_SW_MOD         (1 << (int)E_GFX_SW_MOD)

/// Hardware mode
#define GFX_HAVE_HW_MOD         (1 << (int)E_GFX_HW_MOD)

/// FrameBuffer mode
#define GFX_HAVE_FB_MOD         (1 << (int)E_GFX_FB_MOD)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
// gfx command queue info
//  que capacity, que size, is idle, que top, next que (if multiple),
//  previous index, read index, write index.
//  basically, it is a software maintained hardware que information
typedef struct _GFX_CMDQUE_T
{
    int i4ShortQue;
    int i4QueConfig;
    int i4QueCapacity;
    int i4QueSize;
    int i4PrevIndex;
    int i4ReadIndex;
    int i4WriteIndex;
    int i4QueFlushCount;
    int i4QueIntCount;
    unsigned char  *pu1PrevAddr;
    unsigned char  *pu1ReadAddr;
    unsigned char  *pu1WriteAddr;
    volatile unsigned long long *pu8QueTop;
    struct _GFX_CMDQUE_T *prNext;
} GFX_CMDQUE_T;

// cmdque size configuration
enum EGFX_CMDQ_SIZE_CONFIG_T
{
    E_GFX_CMDQ_CFG_2KB = 0,
    E_GFX_CMDQ_CFG_4KB,
    E_GFX_CMDQ_CFG_8KB,
    E_GFX_CMDQ_CFG_16KB,
    E_GFX_CMDQ_CFG_32KB,
    E_GFX_CMDQ_CFG_64KB,
    E_GFX_CMDQ_CFG_128KB,
    E_GFX_CMDQ_CFG_256KB,
    E_GFX_CMDQ_CFG_MAX
};

/// Error types
typedef enum 
{
    E_GFX_OK = 0,
    E_GFX_INV_ARG,
    E_GFX_OUT_OF_MEM,
    E_GFX_UNINIT,
    E_GFX_UNDEF_ERR
} GFX_ERR_CODE_T;

/// Color mode
typedef enum 
{
    CM_Reserved0 = 0,
    CM_Reserved1,
    CM_YCbCr_CLUT8,
    CM_Reserved2,
    CM_CbYCrY422_DIRECT16,
    CM_YCbYCr422_DIRECT16,
    CM_AYCbCr8888_DIRECT32,
    CM_Reserved3,
    CM_Reserved4,
    CM_Reserved5,
    CM_RGB_CLUT8,
    CM_RGB565_DIRECT16,
    CM_ARGB1555_DIRECT16,
    CM_ARGB4444_DIRECT16,
    CM_ARGB8888_DIRECT32,
    CM_Reserved6
} GFX_COLOR_MODE_T;

/// Bitblt option
typedef enum 
{
    E_GFXBLT_TRANSPARENT = 0,
    E_GFXBLT_KEYNOT      = 1,
    E_GFXBLT_COLORCHANGE = 2,
    E_GFXBLT_CFMT_ENA    = 4,
    E_GFXBLT_KEYSDSEL    = 5,
    E_GFXBLT_FLIP        = 24,
    E_GFXBLT_MIRROR      = 25,
    E_GFXBLT_OVERLAP      = 26
} GFX_BLT_OPT_T;

/// Video standard mode
typedef enum 
{
    E_VSTD_BT601 = 0,
    E_VSTD_BT709
} GFX_VIDSTD_T;

/// Video system mode
typedef enum 
{
    E_VSYS_VID = 0,
    E_VSYS_COMP
} GFX_VSYS_T;

/// Swap option
typedef enum 
{
    E_SWAP_0        = 0,
    E_SWAP_MERGETOP = 0,
    E_SWAP_1        = 1,
    E_SWAP_SWAP     = 1,
    E_SWAP_2        = 2,
    E_SWAP_BLOCK    = 2,
    E_SWAP_DEF      = 2     //MISRA rule, use 2 instead of E_SWAP_2 here
} GFX_SWAP_MODE_T;

/// YC format
typedef enum 
{
    E_YCFMT_420MB = 0,
    E_YCFMT_420LINEAR,
    E_YCFMT_RES_2,
    E_YCFMT_422LINEAR
} GFX_YCFMT_T;

/// Text and bitmap color mode
typedef enum 
{
    E_BMP_CM_1BIT = 0,
    E_BMP_CM_2BIT,
    E_BMP_CM_4BIT,
    E_BMP_CM_8BIT
} GFX_BMP_COLOR_MODE_T;

/// Gradient mode
typedef enum 
{
    E_GRAD_RESERVED = 0,
    E_GRAD_HOR,
    E_GRAD_VER,
    E_GRAD_BOTH
} GFX_GRAD_MODE_T;

/// Rop mode
typedef enum 
{
    E_ROP_MAX_VALUE = 0,
    E_ROP_MIN_VALUE,
    E_ROP_MULTI,
    E_ROP_RESERVED3,
    E_ROP_NOT_SRC,
    E_ROP_NOT_DST,
    E_ROP_SRC_XOR_DST,
    E_ROP_SRC_XNOR_DST,
    E_ROP_SRC_AND_DST,
    E_ROP_NOT_SRC_AND_DST,
    E_ROP_SRC_AND_NOT_DST,
    E_ROP_NOT_SRC_AND_NOT_DST,
    E_ROP_SRC_OR_DST,
    E_ROP_NOT_SRC_OR_DST,
    E_ROP_SRC_OR_NOT_DST,
    E_ROP_NOT_SRC_OR_NOT_DST,
    E_ROP_MAX
} GFX_ROP_MODE_T;

/// Index to direct color bitblt option mode
typedef enum 
{
    E_IDX2DIR_LN_ST_BYTE_AL_OFF = 0,
    E_IDX2DIR_LN_ST_BYTE_AL_ON  = 1,
    E_IDX2DIR_MSB_LEFT_OFF      = 0,
    E_IDX2DIR_MSB_LEFT_ON       = 1
} GFX_IDX2DIR_OPT_T;

/// Horizontal line to vertical line option mode
typedef enum 
{
    E_H2VLINE_90D_CLOCKWISE = 0,
    E_H2VLINE_90D_COUNTERCLOCKWISE,
    E_H2VLINE_MAX
} GFX_H2VLINE_OPT_T;

/// Engine operation
typedef enum 
{
    OP_TEXT_BLT             = 1,
    OP_BITMAP_BLT           = 1,

    OP_RECT_FILL            = 2,
    OP_DRAW_HLINE           = 3,
    OP_DRAW_POINT           = 3,

    OP_DRAW_VLINE           = 4,
    OP_GRAD_FILL            = 5,
    OP_BITBLT               = 6,
    OP_DMA                  = 7,
    OP_1D_BITBLT            = 7,

    OP_ALPHA_BLEND          = 8,
    OP_ALPHA_COMPOSE        = 9,
    OP_YCBCR_TO_RGB         = 10,
    OP_STRETCH_BITBLT       = 11,
    OP_ALPHA_MAP_BITBLT     = 12,

    OP_ALPHA_COMPOSE_LOOP   = 13,
    OP_ROP_BITBLT           = 14,
    OP_IDX_TO_DIR_BITBLT    = 15,
    OP_LINE_HORI_TO_VERT    = 16,

    OP_COMPRESS_BITBLT      = 17,

    OP_NEW_COMPRESS_BITBLT      = 18,

    OP_STRETCH_ALCOM      = 19,
    OP_SCALER      = 28,
    OP_YBLOCK_TO_YRASTER           = 29,
    OP_CBCRBLOCK_TO_1CBCRRASTER    = 30,
    OP_CBCRBLOCK_TO_2CBCRRASTER    = 31,
    OP_MODE_MAX             = 255
} GFX_OP_MODE_T;

/// Modules
typedef enum 
{
    E_GFX_SW_MOD = 0,
    E_GFX_HW_MOD,
    E_GFX_FB_MOD,
    E_GFX_MODULE_LAST
} GFX_MODULE_T;

typedef enum 
{
    B_ORDER = 0,
    G_ORDER,
    R_ORDER,
    A_ORDER
} GFX_READ_ARGB_ORDER;


enum EGFX_CMDQ_CAPACITY_T
{
    E_GFX_CMDQ_CAP_2KB   =   256,
    E_GFX_CMDQ_CAP_4KB   =   512,
    E_GFX_CMDQ_CAP_8KB   =  1024,
    E_GFX_CMDQ_CAP_16KB  =  2048,
    E_GFX_CMDQ_CAP_32KB  =  4096,
    E_GFX_CMDQ_CAP_64KB  =  8192,
    E_GFX_CMDQ_CAP_128KB = 16384,
    E_GFX_CMDQ_CAP_256KB = 32768,
    E_GFX_CMDQ_CAP_MAX   = 99999
};


#ifdef CC_DEBUG
#define GFX_CMD_INFO
#endif

#ifdef GFX_CMD_INFO
typedef struct _GFX_CMD_FLUSH_INFO_T
{
    unsigned int u4_fire_cnt;
    unsigned int u4_src_pitch;
    unsigned int u4_src_width;
    unsigned int u4_src_height;
    unsigned int u4_dst_pitch;
    unsigned int u4_dst_width;
    unsigned int u4_dst_height;
    unsigned int u4_src_addr;
    unsigned int u4_rect_color;
    unsigned int u4_dst_addr;
    unsigned int u4_third_addr;
    unsigned int u4_pal_addr;
    unsigned int u4_third_pitch;
    unsigned int u4_op;
} GFX_CMD_FLUSH_INFO_T;
#endif

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#if defined(GFX_DEBUG_MODE)

    #define GFX_CMDQUE_INC_FLUSH_COUNT()    GFX_CmdQueFlushCount()
    #define GFX_CMDQUE_INC_INT_COUNT()      GFX_CmdQueIntCount()
    
#else       // normal mode

    #define GFX_CMDQUE_INC_FLUSH_COUNT()
    #define GFX_CMDQUE_INC_INT_COUNT()

#endif // #if defined(GFX_DEBUG_MODE)


typedef enum 
{
    VG_CLR_EXP_REPEAT_MSB,
    VG_CLR_EXP_PADDING_ZERO,
} VGFX_CLR_EXP_MD;

typedef enum 
{
    VG_FILTER_TYPE_2TAP=0x0,
    VG_FILTER_TYPE_4TAP=0x1,
    VG_FILTER_TYPE_MAX
} VG_FILTER_TYPE;

#define    VG_COEFF_4_TAP_0_0           0x000
#define    VG_COEFF_4_TAP_0_1           0x100
#define    VG_COEFF_4_TAP_1_0           0x3f4
#define    VG_COEFF_4_TAP_1_1           0x101
#define    VG_COEFF_4_TAP_2_0           0x3ea
#define    VG_COEFF_4_TAP_2_1           0x0ff
#define    VG_COEFF_4_TAP_3_0           0x3e2
#define    VG_COEFF_4_TAP_3_1           0x0f8
#define    VG_COEFF_4_TAP_4_0           0x3dd
#define    VG_COEFF_4_TAP_4_1           0x0ed
#define    VG_COEFF_4_TAP_5_0           0x3d9
#define    VG_COEFF_4_TAP_5_1           0x0df
#define    VG_COEFF_4_TAP_6_0           0x3d8
#define    VG_COEFF_4_TAP_6_1           0x0ce
#define    VG_COEFF_4_TAP_7_0           0x3d9
#define    VG_COEFF_4_TAP_7_1           0x0ba
#define    VG_COEFF_4_TAP_8_0           0x3db
#define    VG_COEFF_4_TAP_8_1           0x0a5
#define    VG_COEFF_4_TAP_9_0           0x3df
#define    VG_COEFF_4_TAP_9_1           0x08e
#define    VG_COEFF_4_TAP_A_0           0x3e4
#define    VG_COEFF_4_TAP_A_1           0x076
#define    VG_COEFF_4_TAP_B_0           0x3e9
#define    VG_COEFF_4_TAP_B_1           0x05f
#define    VG_COEFF_4_TAP_C_0           0x3ee
#define    VG_COEFF_4_TAP_C_1           0x048
#define    VG_COEFF_4_TAP_D_0           0x3f3
#define    VG_COEFF_4_TAP_D_1           0x033
#define    VG_COEFF_4_TAP_E_0           0x3f8
#define    VG_COEFF_4_TAP_E_1           0x01f
#define    VG_COEFF_4_TAP_F_0           0x3fc
#define    VG_COEFF_4_TAP_F_1           0x00f

/*
typedef struct _VGFX_MATRIX_T
{
    unsigned int          u4_a;
    unsigned int          u4_b;
    unsigned int          u4_c;
    unsigned int          u4_d;
    unsigned int          u4_e;
    unsigned int          u4_f;
    unsigned int          u4_g;
    unsigned int          u4_h;
    unsigned int          u4_i;
}VGFX_MATRIX_T;
*/
typedef struct _VG_PARAM_SET_T
{
    unsigned int          u4ImgAddr;
    unsigned int          u4ImgPitch;
    unsigned int          u4ImgClrMode;
    unsigned int          u4ImgGlobalAlpha;
    
    unsigned int          u4TextureHStart;
    unsigned int          u4TextureVStart;
    unsigned int          u4TextureWidth;
    unsigned int          u4TextureHeight;

    unsigned int          u4WBAddr;
    unsigned int          u4WBPitch;
    unsigned int          u4WBXStart;
    unsigned int          u4WBYStart;
    unsigned int          u4WBWidth;
    unsigned int          u4WBHeight;
    unsigned int          u4WBClrMode;
    unsigned int          u4WBGlobalAlpha;
    
    unsigned int          u4PreSrcRd;
    unsigned int          u4PreDstRd;
    unsigned int          u4PreDstWt;
    unsigned int          u4PreOverFlowEn;
    
    unsigned int          u4BBXStart;
    unsigned int          u4BBYStart;
    unsigned int          u4BBWidth;
    unsigned int          u4BBHeight;
//      VGFX_MATRIX_T   t_matrix;

    unsigned int          u4AlphaCompAr;
    unsigned int          u4AlphaCompOpCode;  
    char            fgAlphaCompEn;
    char            fgAlphaCompNormal;

    char            fgAntiAliasing;             //anti-alising          0x300_OK//en
    char            fgFilterEnable;
    ALPHA_EDGE_MD   u4AlphaEdgeMode;
} VG_PARAM_SET_T;

typedef struct _VGFX_REFLECT_T
{
    unsigned int          u4RefEn;
    unsigned int          u4RefOnly;
    unsigned int          u4TextTureHeight;
    unsigned int          u4RefRatioInit;
    
    unsigned int          u4RatioLineStep;
    unsigned int          u4RefAAEn;
}VGFX_REFLECT_T;

typedef struct _VGFX_PARAM_SET_T
{
    unsigned int          u4ImgAddr;
    unsigned int          u4ImgPitch;
    unsigned int          u4ImgClrMode;
    unsigned int          u4ImgGlobalAlpha;
    
    unsigned int          u4TextureHStart;
    unsigned int          u4TextureVStart;
    unsigned int          u4TextureWidth;
    unsigned int          u4TextureHeight;

    unsigned int          u4WBAddr;
    unsigned int          u4WBPitch;
    unsigned int          u4WBXStart;
    unsigned int          u4WBYStart;
    unsigned int          u4WBWidth;
    unsigned int          u4WBHeight;
    unsigned int          u4WBClrMode;
    unsigned int          u4WBGlobalAlpha;
    
    unsigned int          u4PreSrcRd;
    unsigned int          u4PreDstRd;
    unsigned int          u4PreDstWt;
    unsigned int          u4PreOverFlowEn;
    
    unsigned int          u4BBXStart;
    unsigned int          u4BBYStart;
    unsigned int          u4BBWidth;
    unsigned int          u4BBHeight;
    VGFX_MATRIX_T   t_matrix;

    unsigned int          u4AlphaCompAr;
    unsigned int          u4AlphaCompOpCode;  
    char            fgAlphaCompEn;
    char            fgAlphaCompNormal;

    char            fgAntiAliasing;             //anti-alising          0x300_OK//en
    char            fgFilterEnable;
    ALPHA_EDGE_MD   u4AlphaEdgeMode;
    unsigned int          u4AlEdgeRGB0;
    unsigned int          u4AlEdgeThresHold;
    unsigned int          u4FilterType;
    unsigned int          u4Filter4TapDefault;
    VGFX_COEFF_4_TAP    t_coeff_4_tap;
    VGFX_REFLECT_T      t_reflect;
    GFX_MMU_T           mmu;
} VGFX_PARAM_SET_T;



#endif
