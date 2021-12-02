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
 * $RCSfile: drv_if_gfx.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx.c
 *  Driver interface: GFX part implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_os.h"
#include "x_timer.h"
#include "gfx_if.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define GFX_DBG_EACH_OP_FLUSH


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum _GFX_OPERATION_T
{
    GFX_OP_TEXT_BLT   = 1,
    GFX_OP_BITMAP_BLT = 1,

    GFX_OP_RECT_FILL  = 2,
    GFX_OP_DRAW_HLINE = 3,
    GFX_OP_DRAW_POINT = 3,

    GFX_OP_DRAW_VLINE = 4,
    GFX_OP_GRAD_FILL  = 5,
    GFX_OP_BITBLT     = 6,
    GFX_OP_DMA        = 7,
    GFX_OP_1D_BITBLT  = 7,

    GFX_OP_ALPHA_BITBLT        = 8,
    GFX_OP_ALPHA_COMPOS_BITBLT = 9,
    GFX_OP_YCRCB_RGB_CNV       = 10,
    GFX_OP_STRETCH_BITBLT      = 11,
    GFX_OP_ALPHAMAP_BITBLT     = 12
} GFX_OPERATION_T;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

// Turn on each gfx operation internal flush (for debug use)
#if defined(GFX_DBG_EACH_OP_FLUSH)
    #define GFX_DBG_FLUSH()      GFX_Flush()
#else
    #define GFX_DBG_FLUSH()
#endif


// null pointer check
#define NULL_PT_CHECK(pt)                   \
    {                                       \
        if ((void*)(pt) == NULL)            \
        {                                   \
            return DRV_E_FAIL;      \
        }                                   \
    }


// sx, sy, dx, dy, width, height
#define XYWH_CHECK(VX,VY,VW,VH)             \
    {                                       \
        if (((INT32)(VX) < 0)     ||        \
            ((INT32)(VY) < 0)     ||        \
            ((INT32)(VW) <= 0)    ||        \
            ((INT32)(VH) <= 0))             \
        {                                   \
            return DRV_E_FAIL;      \
        }                                   \
    }


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _GetGfxDrvColorMode
 *  Get gfx driver color mode
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GetGfxDrvColorMode(DRV_COLOR_MODE_T rDrvColorMode)
{
    switch (rDrvColorMode)
    {
    case DRV_CM_YBR_8:
        return (UINT32)CM_YCbCr_CLUT8;
        
    case DRV_CM_BRY_422:
        return (UINT32)CM_CbYCrY422_DIRECT16;
        
    case DRV_CM_YBR_422:
        return (UINT32)CM_YCbYCr422_DIRECT16;
        
    case DRV_CM_AYBR_8888:
        return (UINT32)CM_AYCbCr8888_DIRECT32;
        
    case DRV_CM_RGB_8:
        return (UINT32)CM_RGB_CLUT8;
        
    case DRV_CM_RGB_565:
        return (UINT32)CM_RGB565_DIRECT16;
        
    case DRV_CM_ARGB_1555:
        return (UINT32)CM_ARGB1555_DIRECT16;
        
    case DRV_CM_ARGB_4444:
        return (UINT32)CM_ARGB4444_DIRECT16;
        
    case DRV_CM_ARGB_8888:
        return (UINT32)CM_ARGB8888_DIRECT32;

    default:
        return (UINT32)CM_ARGB8888_DIRECT32;
    }
}


//-------------------------------------------------------------------------
/** _GfxOpModeCheck
 *  Check graphics operation mode
 *
 */
//-------------------------------------------------------------------------
static INT32 _GfxOpModeCheck(UINT32 u4Op, UINT32 u4Src, UINT32 u4Dst)
{
    // rule:1 - do NOT support clut2 and clut4      (SRC)
    if ((u4Src == (UINT32)DRV_CM_YBR_2)   ||
        (u4Src == (UINT32)DRV_CM_YBR_4)   ||
        (u4Src == (UINT32)DRV_CM_RGB_2)     ||
        (u4Src == (UINT32)DRV_CM_RGB_4))
    {
        return (INT32)DRV_E_FAIL;
    }

    // rule:1 - do NOT support clut2 and clut4      (DST)
    if ((u4Dst == (UINT32)DRV_CM_YBR_2)   ||
        (u4Dst == (UINT32)DRV_CM_YBR_4)   ||
        (u4Dst == (UINT32)DRV_CM_RGB_2)     ||
        (u4Dst == (UINT32)DRV_CM_RGB_4))
    {
        return (INT32)DRV_E_FAIL;
    }
    
    // rule:2 - do NOT support YCbYCr or CbYCrY mode    (SRC, DST)
    if ((u4Src == (UINT32)DRV_CM_BRY_422)  ||
        (u4Src == (UINT32)DRV_CM_YBR_422)  ||
        (u4Dst == (UINT32)DRV_CM_BRY_422)  ||
        (u4Dst == (UINT32)DRV_CM_YBR_422))
    {
        return (INT32)DRV_E_FAIL;
    }

    // rule:3 - normal bitblt modes
    if (u4Op == (UINT32)GFX_OP_BITBLT)
    {
        if (((u4Src == (UINT32)DRV_CM_RGB_565)     ||
             (u4Src == (UINT32)DRV_CM_ARGB_1555)   ||
             (u4Src == (UINT32)DRV_CM_ARGB_4444)   ||
             (u4Src == (UINT32)DRV_CM_ARGB_8888)   ||
             (u4Src == (UINT32)DRV_CM_AYBR_8888))  &&
            ((u4Dst == (UINT32)DRV_CM_RGB_565)     ||
             (u4Dst == (UINT32)DRV_CM_ARGB_1555)   ||
             (u4Dst == (UINT32)DRV_CM_ARGB_4444)   ||
             (u4Dst == (UINT32)DRV_CM_ARGB_8888)   ||
             (u4Src == (UINT32)DRV_CM_AYBR_8888)))
        {
            return (INT32)DRV_E_OK;
        }
        
        if ((u4Src == u4Dst)    && 
            ((u4Dst == (UINT32)DRV_CM_YBR_8) || 
             (u4Dst == (UINT32)DRV_CM_RGB_8)))
        {
            return (INT32)DRV_E_OK;
        }

        return (INT32)DRV_E_FAIL;

    }

    // rule:4 - the mode without source
    if ((u4Op == (UINT32)GFX_OP_RECT_FILL)  ||
        (u4Op == (UINT32)GFX_OP_DRAW_HLINE) ||
        (u4Op == (UINT32)GFX_OP_DRAW_VLINE) ||
        (u4Op == (UINT32)GFX_OP_GRAD_FILL))
    {
        if ((u4Dst == (UINT32)DRV_CM_YBR_8) ||
            (u4Dst == (UINT32)DRV_CM_RGB_8)   ||
            (u4Dst == (UINT32)DRV_CM_RGB_565)     ||
            (u4Dst == (UINT32)DRV_CM_ARGB_1555)   ||
            (u4Dst == (UINT32)DRV_CM_ARGB_4444)   ||
            (u4Dst == (UINT32)DRV_CM_ARGB_8888)   ||
            (u4Dst == (UINT32)DRV_CM_AYBR_8888))
        {
            if ((u4Op == (UINT32)GFX_OP_GRAD_FILL) &&
                ((u4Dst == (UINT32)DRV_CM_YBR_8) || 
                 (u4Dst == (UINT32)DRV_CM_RGB_8)))
            {
                return (INT32)DRV_E_FAIL;
            }

            return (INT32)DRV_E_OK;
        }

        return (INT32)DRV_E_FAIL;
    }

    // rule:5 - dma mode
    if (u4Op == (UINT32)GFX_OP_DMA)
    {
        if ((u4Dst == (UINT32)DRV_CM_AYBR_8888)   || 
            (u4Dst == (UINT32)DRV_CM_ARGB_8888))
        {
            return (INT32)DRV_E_OK;
        }

        return (INT32)DRV_E_FAIL;
    }

    // rule:6 - alphamap mode
    if (u4Op == (UINT32)GFX_OP_ALPHAMAP_BITBLT)
    {
        if ((u4Src == (UINT32)DRV_CM_RGB_8) &&
            ((u4Dst == (UINT32)DRV_CM_ARGB_8888)    ||
             (u4Dst == (UINT32)DRV_CM_ARGB_4444)    ||
             (u4Dst == (UINT32)DRV_CM_ARGB_1555)))
            
        {
            return (INT32)DRV_E_OK;
        }

        return (INT32)DRV_E_FAIL;
    }
    
    // rule:7 - all other mode
    if (u4Src == u4Dst)
    {
        if ((u4Dst == (UINT32)DRV_CM_RGB_565)   ||
            (u4Dst == (UINT32)DRV_CM_ARGB_1555) ||
            (u4Dst == (UINT32)DRV_CM_ARGB_4444) ||
            (u4Dst == (UINT32)DRV_CM_ARGB_8888) ||
            (u4Dst == (UINT32)DRV_CM_AYBR_8888))
        {
            return (INT32)DRV_E_OK;
        }

        if ((u4Dst == (UINT32)DRV_CM_YBR_8) ||
            (u4Dst == (UINT32)DRV_CM_RGB_8))
        {
            if ((u4Op == (UINT32)GFX_OP_STRETCH_BITBLT) ||
                (u4Op == (UINT32)GFX_OP_BITBLT))
            {
                return (INT32)DRV_E_OK;
            }
        }
    }

    return (INT32)DRV_E_FAIL;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Exported functions
//-----------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** DRV_GFX_Init
 *  Initialization
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_Init(void)
{
    GFX_Lock();
    
#ifdef USE_GFX_5371_RET_VAL   
    if (GFX_Init())
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
#else    
    GFX_Init();    
#endif    
    GFX_Unlock();
    
    return DRV_E_OK;
}


//-------------------------------------------------------------------------
/** DRV_GFX_Fill
 *  draw rectangular
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_Fill(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_FILL_T* prFill)
{
    UINT32 u4DstX, u4DstY, u4Width, u4Height;
    UINT32 u4Color, u4Pitch;

    // check parameters
    NULL_PT_CHECK(prFill);
    XYWH_CHECK(prFill->u2DstX, prFill->u2DstY, 
               prFill->u2Width, prFill->u2Height);

    if (_GfxOpModeCheck((UINT32)GFX_OP_RECT_FILL, 
                        (UINT32)rColorMode, (UINT32)rColorMode))
    {
        return DRV_E_FAIL;
    }

    u4DstX   = (UINT32)prFill->u2DstX;
    u4DstY   = (UINT32)prFill->u2DstY;
    u4Width  = (UINT32)prFill->u2Width;
    u4Height = (UINT32)prFill->u2Height;
    u4Pitch  = (UINT32)prFill->u2DstPitch;
    u4Color  =         prFill->u4Color;
    
    GFX_Lock();

#ifdef USE_GFX_5371_RET_VAL      
    if (GFX_SetDst((UINT8 *)prFill->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4Pitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
    if (GFX_SetColor(u4Color))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }    
    if (GFX_Fill(u4DstX, u4DstY, u4Width, u4Height))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }    
#else    
    GFX_SetDst((UINT8 *)prFill->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4Pitch);
    GFX_SetColor(u4Color);
    GFX_Fill(u4DstX, u4DstY, u4Width, u4Height);    
#endif

    GFX_DBG_FLUSH();
    GFX_Unlock();

    return DRV_E_OK;
}


//-------------------------------------------------------------------------
/** DRV_GFX_BitBlt
 *  Normal bitblt
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_BitBlt(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_BLIT_T* prBlit)
{
    UINT32 u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height;
    UINT32 u4SrcPitch, u4DstPitch;

    // check parameters
    NULL_PT_CHECK(prBlit);
    XYWH_CHECK(prBlit->u2SrcX, prBlit->u2SrcY, 
               prBlit->u2Width, prBlit->u2Height);
    XYWH_CHECK(prBlit->u2DstX, prBlit->u2DstY, 
               prBlit->u2Width, prBlit->u2Height);
    
    if(_GfxOpModeCheck((UINT32)GFX_OP_BITBLT, 
                       (UINT32)rColorMode, (UINT32)rColorMode))
    {
        return DRV_E_FAIL;
    }

    u4SrcX     = (UINT32)prBlit->u2SrcX;
    u4SrcY     = (UINT32)prBlit->u2SrcY;
    u4DstX     = (UINT32)prBlit->u2DstX;
    u4DstY     = (UINT32)prBlit->u2DstY;
    u4Width    = (UINT32)prBlit->u2Width;
    u4Height   = (UINT32)prBlit->u2Height;
    u4SrcPitch = (UINT32)prBlit->u2SrcPitch;
    u4DstPitch = (UINT32)prBlit->u2DstPitch;

    GFX_Lock();

#ifdef USE_GFX_5371_RET_VAL       
    if (GFX_SetSrc((UINT8 *)prBlit->pu1SrcBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4SrcPitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
    if (GFX_SetDst((UINT8 *)prBlit->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4DstPitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }    
    if (GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }    
    if (GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }    
#else
    GFX_SetSrc((UINT8 *)prBlit->pu1SrcBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4SrcPitch);
    GFX_SetDst((UINT8 *)prBlit->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4DstPitch);
    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);   
    GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
#endif

    GFX_DBG_FLUSH();
    GFX_Unlock();

    return DRV_E_OK;
}


//-------------------------------------------------------------------------
/** DRV_GFX_AlphaBlend
 *  Alpha blending
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_AlphaBlend(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_BLEND_T* prBlend)
{
    UINT32 u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height;
    UINT32 u4Alpha, u4SrcPitch, u4DstPitch;

    // check parameters
    NULL_PT_CHECK(prBlend);
    XYWH_CHECK(prBlend->u2SrcX, prBlend->u2SrcY, 
               prBlend->u2Width, prBlend->u2Height);
    XYWH_CHECK(prBlend->u2DstX, prBlend->u2DstY, 
               prBlend->u2Width, prBlend->u2Height);

    if(_GfxOpModeCheck((UINT32)GFX_OP_ALPHA_BITBLT, 
                       (UINT32)rColorMode, (UINT32)rColorMode))
    {
        return DRV_E_FAIL;
    }

    u4SrcX     = (UINT32)prBlend->u2SrcX;
    u4SrcY     = (UINT32)prBlend->u2SrcY;
    u4DstX     = (UINT32)prBlend->u2DstX;
    u4DstY     = (UINT32)prBlend->u2DstY;
    u4Width    = (UINT32)prBlend->u2Width;
    u4Height   = (UINT32)prBlend->u2Height;
    u4SrcPitch = (UINT32)prBlend->u2SrcPitch;
    u4DstPitch = (UINT32)prBlend->u2DstPitch;

    u4Alpha    =         prBlend->u4Alpha;

    GFX_Lock();

    #ifdef USE_GFX_5371_RET_VAL   
    if (GFX_SetSrc((UINT8*)prBlend->pu1SrcBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4SrcPitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }

    if (GFX_SetDst((UINT8*)prBlend->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4DstPitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }

    if (GFX_SetAlpha(u4Alpha))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }

    if (GFX_Blend(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
    #else
    GFX_SetSrc((UINT8*)prBlend->pu1SrcBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4SrcPitch);
    GFX_SetDst((UINT8*)prBlend->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4DstPitch);

    GFX_SetAlpha(u4Alpha);

    GFX_Blend(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
    #endif
    
    GFX_DBG_FLUSH();
    GFX_Unlock();

    return DRV_E_OK;
}


//-------------------------------------------------------------------------
/** DRV_GFX_Flush
 *  flush commands
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_Flush(void)
{
    GFX_Lock();

#ifdef USE_GFX_5371_RET_VAL    
    if (GFX_Flush())
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
#else
    GFX_Flush();
#endif

    GFX_Wait();

    HalFlushInvalidateDCache();
    
    GFX_Unlock();
    
    return DRV_E_OK;
}

//-------------------------------------------------------------------------
/** DRV_GFX_Wait
 *  wait flush done commands
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_Wait(void)
{
    GFX_Lock();
    
    GFX_Wait();
    
    GFX_Unlock();
    
    return DRV_E_OK;
}

//-------------------------------------------------------------------------
/** DRV_GFX_TransBitBlt
 *  Transparent bitblt
 *
 */
//-------------------------------------------------------------------------
DRV_ERR_CODE_T DRV_GFX_TransBitBlt(DRV_COLOR_MODE_T rColorMode,
    DRV_GFX_TRANSBLIT_T* prBlit)
{
    UINT32 u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height;
    UINT32 u4SrcPitch, u4DstPitch;

    // check parameters
    NULL_PT_CHECK(prBlit);
    XYWH_CHECK(prBlit->u2SrcX, prBlit->u2SrcY, 
               prBlit->u2Width, prBlit->u2Height);
    XYWH_CHECK(prBlit->u2DstX, prBlit->u2DstY, 
               prBlit->u2Width, prBlit->u2Height);

    if(_GfxOpModeCheck((UINT32)GFX_OP_BITBLT, 
                       (UINT32)rColorMode, (UINT32)rColorMode))
    {
        return DRV_E_FAIL;
    }

    u4SrcX     = (UINT32)prBlit->u2SrcX;
    u4SrcY     = (UINT32)prBlit->u2SrcY;
    u4DstX     = (UINT32)prBlit->u2DstX;
    u4DstY     = (UINT32)prBlit->u2DstY;
    u4Width    = (UINT32)prBlit->u2Width;
    u4Height   = (UINT32)prBlit->u2Height;
    u4SrcPitch = (UINT32)prBlit->u2SrcPitch;
    u4DstPitch = (UINT32)prBlit->u2DstPitch;

    GFX_Lock();

#ifdef USE_GFX_5371_RET_VAL    
    if (GFX_SetSrc((UINT8 *)prBlit->pu1SrcBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4SrcPitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
    
    if (GFX_SetDst((UINT8 *)prBlit->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4DstPitch))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
    
    if (GFX_SetBltOpt(
            ((D_GFXFLAG_TRANSPARENT) | 
             (prBlit->u4InverseMask ? D_GFXFLAG_KEYNOT : 0)),
            prBlit->u4SrcColorKeyMin, prBlit->u4SrcColorKeyMax))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
    
    if (GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height))
    {
        GFX_Unlock();
        return DRV_E_FAIL;
    }
#else
    GFX_SetSrc((UINT8 *)prBlit->pu1SrcBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4SrcPitch);
    
    GFX_SetDst((UINT8 *)prBlit->pu1DstBaseAddr, 
                   _GetGfxDrvColorMode(rColorMode), u4DstPitch);
    
    GFX_SetBltOpt(
            ((D_GFXFLAG_TRANSPARENT) | 
             (prBlit->u4InverseMask ? D_GFXFLAG_KEYNOT : 0)),
            prBlit->u4SrcColorKeyMin, prBlit->u4SrcColorKeyMax);
    
    GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Height);
#endif

    
    GFX_DBG_FLUSH();
    GFX_Unlock();

    return DRV_E_OK;
}

