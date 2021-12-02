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
 * $RCSfile: gfx_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
 
/** @file gfx_if.h
 *  gfx driver main interfaces
 *  
 */

#ifndef GFX_IF_H
#define GFX_IF_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------


/*lint -save -e961 */
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
/*lint -restore */


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

/// String size
#define GFX_STR_SIZE            50

/// Bitblt option flag : transparent
#define D_GFXFLAG_TRANSPARENT   (1 << (INT32)E_GFXBLT_TRANSPARENT)

/// Bitblt option flag : keynot
#define D_GFXFLAG_KEYNOT        (1 << (INT32)E_GFXBLT_KEYNOT)

/// Bitblt option flag : color change
#define D_GFXFLAG_COLORCHANGE   (1 << (INT32)E_GFXBLT_COLORCHANGE)

/// Bitblt option flag : color format translation
#define D_GFXFLAG_CFMT_ENA      (1 << (INT32)E_GFXBLT_CFMT_ENA)

/// Bitblt option flag : keysdsel
#define D_GFXFLAG_KEYSDSEL      (1 << (INT32)E_GFXBLT_KEYSDSEL)

/// Bitblt option flag : flip
#define D_GFXFLAG_FLIP          (1 << (INT32)E_GFXBLT_FLIP)

/// Bitblt option flag : mirror
#define D_GFXFLAG_MIRROR        (1 << (INT32)E_GFXBLT_MIRROR)

/// Bitblt option flag : overlap
#define D_GFXFLAG_OVERLAP        (1 << (INT32)E_GFXBLT_OVERLAP)

/// Bitblt option flag : none
#define D_GFXFLAG_NONE          (0)

/// Software mode
#define GFX_HAVE_SW_MOD         (1 << (INT32)E_GFX_SW_MOD)

/// Hardware mode
#define GFX_HAVE_HW_MOD         (1 << (INT32)E_GFX_HW_MOD)

/// FrameBuffer mode
#define GFX_HAVE_FB_MOD         (1 << (INT32)E_GFX_FB_MOD)


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
// gfx command queue info
//  que capacity, que size, is idle, que top, next que (if multiple),
//  previous index, read index, write index.
//  basically, it is a software maintained hardware que information
typedef struct _GFX_CMDQUE_T
{
    INT32 i4ShortQue;
    INT32 i4QueConfig;
    INT32 i4QueCapacity;
    INT32 i4QueSize;
    INT32 i4PrevIndex;
    INT32 i4ReadIndex;
    INT32 i4WriteIndex;
    INT32 i4QueFlushCount;
    INT32 i4QueIntCount;
    UINT8 *pu1PrevAddr;
    UINT8 *pu1ReadAddr;
    UINT8 *pu1WriteAddr;
    volatile UINT64 *pu8QueTop;
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

/// Alpha composition PD rules
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
} GFX_AC_MODE_T;

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
    E_ROP_RESERVED0 = 0,
    E_ROP_RESERVED1,
    E_ROP_RESERVED2,
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

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  GFX_Init
//
/** Initialize gfx driver.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Init(void);

#if defined(CC_MT8283_OSD_COMPRESS) && defined(CC_FLIP_MIRROR_SUPPORT)
//---------------------------------------------------------------------------
//  GFX_GetMirrorFlip
//
/** Initialize gfx driver.
 *
 *  @retval 
 */
//---------------------------------------------------------------------------
UINT8 GFX_GetMirrorFlip(void);
#endif

//---------------------------------------------------------------------------
//  GFX_Reset
//
/** Reset engine.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Reset(void);


//---------------------------------------------------------------------------
//  GFX_Wait
//
/** Wait for flushing engine completely.
 *  
 *  @retval void
 */
//---------------------------------------------------------------------------

extern void GFX_Wait(void);

extern void GFX_Wait_Vdp(void);


extern void GFX_Wait_Timeout(INT32 i4Timeout);


//---------------------------------------------------------------------------
//  GFX_MwFlush
//
/** Flush cmdque and force engine to do operations (for MW use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_MwFlush(void);


//---------------------------------------------------------------------------
//  GFX_Flush
//
/** Flush cmdque and force engine to do operations (for Drv use).
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Flush(void);


//---------------------------------------------------------------------------
//  GFX_Lock
//
/** Take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Lock(void);


//---------------------------------------------------------------------------
//  GFX_TryLock
//
/** Try to take the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_TryLock(void);


//---------------------------------------------------------------------------
//  GFX_Unlock
//
/** Release the gfx resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Unlock(void);


//---------------------------------------------------------------------------
//  GFX_LockCmdque
//
/** Take the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_LockCmdque(void);


//---------------------------------------------------------------------------
//  GFX_UnlockCmdque
//
/** Release the gfx-cmdq resource.
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_UnlockCmdque(void);


//---------------------------------------------------------------------------
//  GFX_QueryHwIdle
//
/** Query engine status.
 *
 *  @retval 1   Idle
 *  @retval 0   Busy
 */
//---------------------------------------------------------------------------
extern INT32 GFX_QueryHwIdle(void);


//---------------------------------------------------------------------------
//  GFX_SetNotify
//
/** Set notify function (for MW use).
 *
 *  @param  pfnNotifyFunc   The callback function pointer
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetNotify(void (*pfnNotifyFunc)(UINT32));


//---------------------------------------------------------------------------
//  GFX_SetDst
//
/** Set destination region.
 *
 *  @param  pu1Base     The base address of the dst region
 *  @param  u4ColorMode The color mode of the dst region
 *  @param  u4Pitch     The pitch of the dst region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetDst(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);


//---------------------------------------------------------------------------
//  GFX_SetSrc
//
/** Set source region.
 *
 *  @param  pu1Base     The base address of the src region
 *  @param  u4ColorMode The color mode of the src region
 *  @param  u4Pitch     The pitch of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetSrc(UINT8 *pu1Base, UINT32 u4ColorMode, UINT32 u4Pitch);


//---------------------------------------------------------------------------
//  GFX_SetAlpha
//
/** Set global alpha.
 *
 *  @param  u4Alpha     The global alpha
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetAlpha(UINT32 u4Alpha);


//---------------------------------------------------------------------------
//  GFX_SetColor
//
/** Set rectangular color.
 *
 *  @param  u4Color     The rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetColor(UINT32 u4Color);


//---------------------------------------------------------------------------
//  GFX_Fill
//
/** Fill rectangle with rectangular color.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the rectangle
 *  @param  u4Height    The height of the rectangle
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Fill(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_HLine
//
/** Draw horizontal line.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the line
 * 
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_HLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width);


//---------------------------------------------------------------------------
//  GFX_VLine
//
/** Draw vertical line.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Height    The height of the line
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_VLine(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_SetBltOpt
//
/** Set bitblt option.
 *
 *  @param  u4Switch    The option flag of BitBlt
 *  @param  u4ColorMin  The dst color key min
 *  @param  u4ColorMax  The dst color key max
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetBltOpt(UINT32 u4Switch, UINT32 u4ColorMin, 
    UINT32 u4ColorMax);


//---------------------------------------------------------------------------
//  GFX_BitBlt
//
/** Bit blit operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_BitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_Blend
//
/** Alpha blending operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Blend(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_AlphaComposePass
//
/** Alpha compose pass operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *  @param  u4Pass      The pass method
 *  @param  u4Param     The global alpha
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_AlphaComposePass(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4Pass, UINT32 u4Param);


//---------------------------------------------------------------------------
//  GFX_SetColCnvFmt
//
/** Set ycbcr to rgb color conversion format.
 *
 *  @param  u4YCFmt     The yc format
 *  @param  u4SwapMode  The swap mode
 *  @param  u4VidStd    The video standard
 *  @param  u4VidSys    The video system
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetColCnvFmt(UINT32 u4YCFmt, UINT32 u4SwapMode, 
    UINT32 u4VidStd, UINT32 u4VidSys);


//---------------------------------------------------------------------------
//  GFX_SetColCnvSrc
//
/** Set color conversion source region.
 *
 *  @param  pu1SrcLuma          The base address of the luma region
 *  @param  u4SrcLumaPitch      The pitch of the luma region
 *  @param  pu1SrcChroma        The base address of the chroma region
 *  @param  u4SrcChromaPitch    The pitch of the chroma region
 *  @param  u4FieldPic          The field mode
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetColCnvSrc(UINT8 *pu1SrcLuma, UINT32 u4SrcLumaPitch, 
    UINT8 *pu1SrcChroma, UINT32 u4SrcChromaPitch, UINT32 u4FieldPic);


//---------------------------------------------------------------------------
//  GFX_ColConv
//
/** Convert ycbcr to rgb.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_ColConv(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height);




//---------------------------------------------------------------------------
//  GFX_StretchBlt
//
/** Stretch bitblt (repeat or drop).
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4SrcW      The width of the src extent
 *  @param  u4SrcH      The height of the src extent
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4DstW      The width of the dst extent
 *  @param  u4DstH      The height of the dst extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_StretchBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH);


//---------------------------------------------------------------------------
//  GFX_AlphaMapBitBlt
//
/** Alpha-map bitblt operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_AlphaMapBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_SetRopOpt
//
/** Set rop bitblt option.
 *
 *  @param  u4RopCode       The rop code
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetRopOpt(UINT32 u4RopCode);


//---------------------------------------------------------------------------
//  GFX_RopBitBlt
//
/** Rop bitblt operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_RopBitBlt(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_ComposeLoop
//
/** Loop mode alpha composition.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *  @param  u4Ar        The global alpha
 *  @param  u4OpCode    The PD rules
 *  @param  u4RectSrc   The flag of src extent is filled rectangular color
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_ComposeLoop(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height, UINT32 u4Ar, 
    UINT32 u4OpCode, UINT32 u4RectSrc);


//---------------------------------------------------------------------------
//  GFX_SetIdx2DirSrc
//
/** Set index to direct color bitblt source region.
 *
 *  @param  pu1Base     The base address of the src region
 *  @param  u4SrcCM     The color mode of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetIdx2DirSrc(UINT8 *pu1Base, UINT32 u4SrcCM);


//---------------------------------------------------------------------------
//  GFX_SetIdx2DirOpt
//
/** Set index to direct color bitblt option.
 *
 *  @param  pu1PaletteBase      The base address of the palette
 *  @param  u4MsbLeft           The flag of left pixel location in one byte
 *  @param  u4StartByteAligned  The flag of src line start byte aligned
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetIdx2DirOpt(UINT8 *pu1PaletteBase, UINT32 u4MsbLeft, 
    UINT32 u4StartByteAligned);


//---------------------------------------------------------------------------
//  GFX_Idx2DirBitBlt
//
/** index to direct color bitblt operation.
 *
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_Idx2DirBitBlt(UINT32 u4DstX, UINT32 u4DstY, UINT32 u4Width, 
    UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_2DMemCompare
//
/** 2D memory compare operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the extent
 *  @param  u4Height    The height of the extent
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_2DMemCompare(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4Width, UINT32 u4Height);


//---------------------------------------------------------------------------
//  GFX_GetMemCompareResult
//
/** Get memory compare result.
 *
 *  @retval 1   With difference
 *  @retval 0   No difference
 */
//---------------------------------------------------------------------------
extern INT32 GFX_GetMemCompareResult(void);


//---------------------------------------------------------------------------
//  GFX_SetHoriToVertLineOpt
//
/** Set horizontal line to vertical line option.
 *
 *  @param  u4Is90dCcw      The flag of 90" clockwise or counterclockwise
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetHoriToVertLineOpt(UINT32 u4Is90dCcw);


//---------------------------------------------------------------------------
//  GFX_HoriToVertLine
//
/** Horizontal line to vertical line operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *  @param  u4DstX      The x coordinate of the dst region
 *  @param  u4DstY      The y coordinate of the dst region
 *  @param  u4Width     The width of the horizontal line
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_HoriToVertLine(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4DstX, 
    UINT32 u4DstY, UINT32 u4HoriLineWidth);


//---------------------------------------------------------------------------
//  GFX_SetExtCmdQue
//
/** Horizontal line to vertical line operation.
 *  
 *  @param  u4SrcX      The x coordinate of the src region
 *  @param  u4SrcY      The y coordinate of the src region
 *
 *  @retval void
 */
//---------------------------------------------------------------------------
extern void GFX_SetExtCmdQue(volatile GFX_CMDQUE_T  *prGfxCmdQue,
                     volatile UINT64       *pu8CmdQueBuf, UINT32 u4Config);

extern void GFX_SetDirectFBMem(UINT32 u4Addr, UINT32 u4Size);

extern void GfxSetBurstReadMode(BOOL fgBurstRead);
extern BOOL GfxGetBurstReadMode(VOID);


extern BOOL GFX_ColorConvert(void* pvDstInfo,UINT32 u4SrcAddr,UINT8 u1Alpha);
extern void GFX_NewCompressBlt(UINT8 *pu1SrcBase, UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcCM, 
    UINT32 u4SrcPitch, UINT8 *pu1DstBase, UINT32 u4Width, UINT32 u4Height, UINT32 u4BufHeight,
    BOOL u4RollBackEn, UINT32 u4QualityMode, UINT32 u4BpCompAddrEnd,BOOL fgMirrorFlip,BOOL fgBurstRead,BOOL fgLineSeparate);

extern void GFX_StretchAlphaCom(UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcW, 
    UINT32 u4SrcH, UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstW, UINT32 u4DstH, 
    UINT32 u4Ar, UINT32 u4OpCode);

extern void GFX_SetPremult(UINT32 u4PremultSrcR, UINT32 u4PremultDstR, UINT32 u4PremultDstW, UINT32 u4PremultOvrflw);
	
extern void (*pfnGFX_DMANotify)(UINT32, void*);

extern void GFX_SetDMANotify(void (*pfnNotifyFunc)(UINT32, void*));

extern void GFX_SetDMAStatus(INT32 i4Status);

extern INT32 GFX_IsDMA(void);

extern void GFX_SetDMATag(void *pvTag);

extern void* GFX_GetDMATag(void);


#endif // GFX_IF_H



