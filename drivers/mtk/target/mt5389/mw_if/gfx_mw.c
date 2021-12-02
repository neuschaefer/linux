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
 * $RCSfile: gfx_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_mw.c
 *  gfx and mw interface
 *  
 */

/*
    lint inhibition(s)
        522 - Expected void type, assignment, increment or 
              decrement [MISRA Rule 53]
        611 - Suspicious cast
        818 - Pointer parameter 'pt_comp_id' (line 572) could be
              declared as pointing to const [MISRA Rule 81]
*/


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

/*lint -save -e522 -e611 -e726 -e818 */

//#include "drv_comp_id.h"
#include "drv_name.h"

#include "x_gfx.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_sys_name.h"
//#include "x_hal_926.h"

#include "gfx_if.h"
#if 0
#include "gfxsc_if.h"
#else
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#endif
#include "osd_drvif.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//#define GFX_DBG_EACH_OP_FLUSH
#define GFX_MW_CHK_8B_ALIGN(val)   ASSERT(!(val&7))


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define GPU_ID                  ((UINT16) 0)
#define GFX_STRETCH_SCALE       128
#define GFX_MIN_STR_BITS        33


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

typedef enum _GFX_ERROR_T
{
    GFX_OK                          =  0,
    GFX_ERROR_BAD_PARAMETER         = -1,
    GFX_ERROR_NO_MEMORY             = -2,
    GFX_ERROR_ACTION_FAILED         = -3,
    GFX_ERROR_UNSUPPORTED_OPERATION = -4,
    GFX_ERROR_INTERNAL              = -5,
    GFX_ERROR_NO_RGNLIST            = -6
} GFX_ERROR_T;

typedef INT32 (*pfGetDispatcher)(
    INT8            icPlane,
    DRV_GET_TYPE_T  e_get_type,
    VOID*           pv_get_info,
    SIZE_T*         pz_get_info_len);

typedef INT32 (*pfSetDispatcher)(
    INT8            icPlane,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_set_info,
    SIZE_T          z_set_info_len);

typedef struct _GFX_DRV_COMP_T
{
    DRV_TYPE_T          e_type;
    UINT16              ui2_id;
    CHAR                szCompName[64];
    pfGetDispatcher     pfGet;
    pfSetDispatcher     pfSet;
} GFX_DRV_COMP_T;

typedef struct _MW_NOTIFY_DATA_T
{
    GFX_COND_T  rCond;
    const VOID *pvTag;
    UINT32      u4Data;
} MW_NOTIFY_DATA_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define GFX_SUCCEEDED(ret)                  \
            ((GFX_ERROR_T) (ret) == GFX_OK)

#define GFX_FAILED(ret)                     \
            ((GFX_ERROR_T) (ret) != GFX_OK)

#define GET_COMMAND(f)                      \
    {                                       \
        if (GFX_SUCCEEDED((f)))             \
        {                                   \
            return RMR_OK;                  \
        }                                   \
        else                                \
        {                                   \
            return RMR_DRV_INV_GET_INFO;    \
        }                                   \
    }

#define SET_COMMAND(f)                      \
    {                                       \
        if (GFX_SUCCEEDED((f)))             \
        {                                   \
            return RMR_OK;                  \
        }                                   \
        else                                \
        {                                   \
            return RMR_DRV_INV_SET_INFO;    \
        }                                   \
    }

#define NULL_PT_CHECK(pt)                   \
    {                                       \
        if ((void*)(pt) == NULL)            \
        {                                   \
            return GFX_ERROR_INTERNAL;      \
        }                                   \
    }

#define XYWH_CHECK(VX,VY,VW,VH)             \
    {                                       \
        if (((INT32)(VX) < 0)     ||        \
            ((INT32)(VY) < 0)     ||        \
            ((INT32)(VW) <= 0)    ||        \
            ((INT32)(VH) <= 0))             \
        {                                   \
            return GFX_ERROR_INTERNAL;      \
        }                                   \
    }


#if defined(CC_MT8223)
#define GFX_STRETCH_BITBLT(GT) _GfxStretchBitblt(GT)
#else
#define GFX_STRETCH_BITBLT(GT) _GfxScale(GT)
#endif

// Turn on each gfx operation internal flush (for debug use)
#if defined(GFX_DBG_EACH_OP_FLUSH)
    #define GFX_DBG_FLUSH()      GFX_Flush()
#else
    #define GFX_DBG_FLUSH()
#endif

#define ALIGN16(X) (((X) + 0xF) & (~0xF))	///< for scaler pitch alignment
#define OP_IMGRZ_SCALING    32
#define GFX_SCALING_MIN_W   (UINT32)4 
#define GFX_SCALING_MIN_H   (UINT32)4
#define GFX_SCALING_MAX_W   (UINT32)4095 
#define GFX_SCALING_MAX_H   (UINT32)4095
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

extern BOOL _IMGRZ_UsedByJpeg(void);
//static INT8 _i1CompCount = 0;

static GFX_DRV_COMP_T *_prDrvComp = NULL;

static BOOL _bInitOK = FALSE;

static GFX_NFY_FCT_T _pfnGfxNfyFct;

static MW_NOTIFY_DATA_T _rGfxNfyData;

#if defined(CC_MT8283_OSD_COMPRESS) && defined(CC_FLIP_MIRROR_SUPPORT)
static UINT8 _u1MirrorFlipState=0;
#endif


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _fnNfyFct
 *  notify function
 *
 */
//-------------------------------------------------------------------------
static void _fnNfyFct(UINT32 u4Data)
{
    if (_pfnGfxNfyFct)
    {
        if (u4Data == (UINT32)E_GFX_UNDEF_ERR)
        {
            _rGfxNfyData.u4Data = GFX_EXEC_FAIL;
        }

        _pfnGfxNfyFct(_rGfxNfyData.rCond, _rGfxNfyData.pvTag, _rGfxNfyData.u4Data);
    }
}


//-------------------------------------------------------------------------
/** _GetCompCount
 *  
 *  
 */
//-------------------------------------------------------------------------
//static INT8 _GetCompCount(void)
//{
//    return (_i1CompCount);
//}


//-------------------------------------------------------------------------
/** _AllocDrvComp
 *  
 *  
 */
//-------------------------------------------------------------------------
static GFX_DRV_COMP_T* _AllocDrvComp(INT8 i1Count)
{
    INT32 i4Count = (INT32)i1Count;
    SIZE_T zSize = (SIZE_T)((UINT32)i4Count * sizeof(GFX_DRV_COMP_T));

    _prDrvComp = (GFX_DRV_COMP_T *)x_mem_alloc(zSize);
    ASSERT(_prDrvComp != NULL);

//    _i1CompCount = i1Count;
    return (_prDrvComp);
}


//-------------------------------------------------------------------------
/** _FreeDrvComp
 *  
 *  
 */
//-------------------------------------------------------------------------
static void _FreeDrvComp(void)
{
    if (_prDrvComp)
    {
        x_mem_free(_prDrvComp);
        _prDrvComp = NULL;
    }
//    _i1CompCount = 0;
}


//-------------------------------------------------------------------------
/** _GetComponent
 *  
 *  
 */
//-------------------------------------------------------------------------
static const GFX_DRV_COMP_T* _GetComponent(INT8 i)
{
    return &(_prDrvComp[i]);
}


//-------------------------------------------------------------------------
/** _IsSameCompId
 *  
 *  
 */
//-------------------------------------------------------------------------
//static BOOL _IsSameCompId(const DRV_COMP_ID_T *ptComp, DRV_TYPE_T e_type, UINT16 ui2_id)
//{
//    if (ptComp == NULL)
//    {
//        return (BOOL)0;
//    }
//    
//    return ((ptComp->e_type == e_type) && (ptComp->ui2_id == ui2_id));
//}


//-------------------------------------------------------------------------
/** _GfxGetFontMode
 *  Get gfx driver font mode
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GfxGetFontMode(GFX_COLORMODE_T rMwColorMode)
{
    switch (rMwColorMode)
    {
    case GFX_COLORMODE_AYCbCr_CLUT2:
    case GFX_COLORMODE_ARGB_CLUT2:
        return (UINT32)E_BMP_CM_2BIT;
        
    case GFX_COLORMODE_AYCbCr_CLUT4:
    case GFX_COLORMODE_ARGB_CLUT4:
        return (UINT32)E_BMP_CM_4BIT;
        
    case GFX_COLORMODE_AYCbCr_CLUT8:
    case GFX_COLORMODE_ARGB_CLUT8:
        return (UINT32)E_BMP_CM_8BIT;

    default:
        return (UINT32)E_BMP_CM_1BIT;
    }
}


//-------------------------------------------------------------------------
/** _GfxGetColorMode
 *  Get gfx driver color mode
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GfxGetColorMode(GFX_COLORMODE_T rMwColorMode)
{
    switch (rMwColorMode)
    {
    case GFX_COLORMODE_AYCbCr_CLUT8:
        return (UINT32)CM_YCbCr_CLUT8;
        
    case GFX_COLORMODE_CbYCrY_16:
        return (UINT32)CM_CbYCrY422_DIRECT16;
        
    case GFX_COLORMODE_YCbYCr_16:
        return (UINT32)CM_YCbYCr422_DIRECT16;
        
    case GFX_COLORMODE_AYCbCr_D8888:
        return (UINT32)CM_ARGB8888_DIRECT32;    // workaround
        
    case GFX_COLORMODE_ARGB_CLUT8:
        return (UINT32)CM_RGB_CLUT8;
        
    case GFX_COLORMODE_RGB_D565:
        return (UINT32)CM_RGB565_DIRECT16;
        
    case GFX_COLORMODE_ARGB_D1555:
        return (UINT32)CM_ARGB1555_DIRECT16;
        
    case GFX_COLORMODE_ARGB_D4444:
        return (UINT32)CM_ARGB4444_DIRECT16;
        
    case GFX_COLORMODE_ARGB_D8888:
        return (UINT32)CM_ARGB8888_DIRECT32;

    default:
        return (UINT32)CM_ARGB8888_DIRECT32;
    }
}

#if 0
//-------------------------------------------------------------------------
/** _GfxScGetColorMode
 *  Get gfx-scalar driver color mode
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GfxScGetColorMode(GFX_COLORMODE_T rMwColorMode)
{
    switch (rMwColorMode)
    {
    case GFX_COLORMODE_AYCbCr_CLUT8:
        return (UINT32)CM_SC_SINGLE;

    case GFX_COLORMODE_ARGB_CLUT8:
        return (UINT32)CM_SC_SINGLE;
        
    case GFX_COLORMODE_RGB_D565:
        return (UINT32)CM_RGB565_DIRECT16;
        
    case GFX_COLORMODE_ARGB_D1555:
        return (UINT32)CM_ARGB1555_DIRECT16;
        
    case GFX_COLORMODE_ARGB_D4444:
        return (UINT32)CM_ARGB4444_DIRECT16;
        
    case GFX_COLORMODE_ARGB_D8888:
    case GFX_COLORMODE_AYCbCr_D8888:
        return (UINT32)CM_ARGB8888_DIRECT32;

    default:
        return (UINT32)CM_ARGB8888_DIRECT32;
    }
}
#endif

//-------------------------------------------------------------------------
/** _GfxGetYcFormat
 *  Get gfx driver YC format
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GfxGetYcFormat(GFX_YCBCR_FORMAT_T eMwYbrFormat)
{
//    IC_VERSION_T eVer;
//    eVer = BSP_GetIcVersion();

    if (0) // (eVer < IC_VER_AC)   // without 422 linear mode
    {
        switch (eMwYbrFormat)
        {
        case GFX_YCBCR_420_MB:
            return (UINT32)E_YCFMT_420MB;

        case GFX_YCBCR_420_LNR:
            return (UINT32)E_YCFMT_420LINEAR;
            
        default:
            return (UINT32)E_YCFMT_RES_2;
        }
    }
    else                    // with 422 linear mode
    {
        switch (eMwYbrFormat)
        {
        case GFX_YCBCR_420_MB:
            return (UINT32)E_YCFMT_420MB;

        case GFX_YCBCR_420_LNR:
            return (UINT32)E_YCFMT_420LINEAR;

        case GFX_YCBCR_422_LNR:
            return (UINT32)E_YCFMT_422LINEAR;
            
        default:
            return (UINT32)E_YCFMT_RES_2;
        }
    }
}


//-------------------------------------------------------------------------
/** _GfxGetRopCode
 *  Get gfx driver ROP code
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GfxGetRopCode(GFX_ROP_TYPE_T eMwRopType)
{
    switch (eMwRopType)
    {
    case GFX_ROP_NOT_SRC:
        return (UINT32)E_ROP_NOT_SRC;
        
    case GFX_ROP_NOT_DST:
        return (UINT32)E_ROP_NOT_DST;
        
    case GFX_ROP_SRC_XOR_DST:
        return (UINT32)E_ROP_SRC_XOR_DST;
        
    case GFX_ROP_SRC_XNOR_DST:
        return (UINT32)E_ROP_SRC_XNOR_DST;
        
    case GFX_ROP_SRC_AND_DST:
        return (UINT32)E_ROP_SRC_AND_DST;
        
    case GFX_ROP_NOT_SRC_AND_DST:
        return (UINT32)E_ROP_NOT_SRC_AND_DST;
        
    case GFX_ROP_SRC_AND_NOT_DST:
        return (UINT32)E_ROP_SRC_AND_NOT_DST;
    
    case GFX_ROP_NOT_SRC_AND_NOT_DST:
        return (UINT32)E_ROP_NOT_SRC_AND_NOT_DST;
        
    case GFX_ROP_SRC_OR_DST:
        return (UINT32)E_ROP_SRC_OR_DST;
        
    case GFX_ROP_NOT_SRC_OR_DST:
        return (UINT32)E_ROP_NOT_SRC_OR_DST;
        
    case GFX_ROP_SRC_OR_NOT_DST:
        return (UINT32)E_ROP_SRC_OR_NOT_DST;
        
    case GFX_ROP_NOT_SRC_OR_NOT_DST:
        return (UINT32)E_ROP_NOT_SRC_OR_NOT_DST;
        
    default:
		#if defined(CC_MT5389)
		return (UINT32)E_ROP_MAX;
		#else
        return (UINT32)E_ROP_RESERVED0;
		#endif
    }
}


//-------------------------------------------------------------------------
/** _GetGfxBitsPerPixel
 *  Get graphics bits/pixel
 *
 */
//-------------------------------------------------------------------------
static UINT32 _GetGfxBitsPerPixel(UINT32 u4DrvColorMode)
{
    switch (u4DrvColorMode)
    {
    case CM_YCbCr_CLUT8:
    case CM_RGB_CLUT8:
        return 8;
        
    case CM_CbYCrY422_DIRECT16:
    case CM_YCbYCr422_DIRECT16:
    case CM_RGB565_DIRECT16:
    case CM_ARGB1555_DIRECT16:
    case CM_ARGB4444_DIRECT16:
        return 16;
        
    case CM_AYCbCr8888_DIRECT32:
    case CM_ARGB8888_DIRECT32:
    default:
        return 32;
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
    // rule:0 - index to direct color mode
    if (u4Op == (UINT32)OP_IDX_TO_DIR_BITBLT)
    {
        if (((u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT2) ||
             (u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT4) ||
             (u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT2)   ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT4)   ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT8))     &&
            ((u4Dst == (UINT32)GFX_COLORMODE_RGB_D565)      ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D1555)    ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D4444)    ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888)))
        {
            return (INT32)GFX_OK;
        }

        return (INT32)GFX_ERROR_INTERNAL;
    }

    // rule:1 - do NOT support clut2 and clut4      (SRC)
    if ((u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT2)   ||
        (u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT4)   ||
        (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT2)     ||
        (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT4))
    {
        return (INT32)GFX_ERROR_INTERNAL;
    }

    // rule:1 - do NOT support clut2 and clut4      (DST)
    if ((u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_CLUT2)   ||
        (u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_CLUT4)   ||
        (u4Dst == (UINT32)GFX_COLORMODE_ARGB_CLUT2)     ||
        (u4Dst == (UINT32)GFX_COLORMODE_ARGB_CLUT4))
    {
        return (INT32)GFX_ERROR_INTERNAL;
    }
    
    // rule:2 - do NOT support YCbYCr or CbYCrY mode    (SRC, DST)
    if ((u4Src == (UINT32)GFX_COLORMODE_CbYCrY_16)  ||
        (u4Src == (UINT32)GFX_COLORMODE_YCbYCr_16)  ||
        (u4Dst == (UINT32)GFX_COLORMODE_CbYCrY_16)  ||
        (u4Dst == (UINT32)GFX_COLORMODE_YCbYCr_16))
    {
        return (INT32)GFX_ERROR_INTERNAL;
    }

    // rule:3 - normal bitblt modes
    if (u4Op == (UINT32)OP_BITBLT)
    {
        if (((u4Src == (UINT32)GFX_COLORMODE_RGB_D565)     ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_D1555)   ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_D4444)   ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_D8888)   ||
             (u4Src == (UINT32)GFX_COLORMODE_AYCbCr_D8888))  &&
            ((u4Dst == (UINT32)GFX_COLORMODE_RGB_D565)     ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D1555)   ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D4444)   ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888)   ||
             (u4Src == (UINT32)GFX_COLORMODE_AYCbCr_D8888)))
        {
            return (INT32)GFX_OK;
        }
        
        if ((u4Src == u4Dst)    && 
            ((u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) || 
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_CLUT8)))
        {
            return (INT32)GFX_OK;
        }

        return (INT32)GFX_ERROR_INTERNAL;

    }

    // rule:4 - the mode without source
    if ((u4Op == (UINT32)OP_RECT_FILL)  ||
        (u4Op == (UINT32)OP_DRAW_HLINE) ||
        (u4Op == (UINT32)OP_DRAW_VLINE) ||
        (u4Op == (UINT32)OP_GRAD_FILL))
    {
        if ((u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_CLUT8)   ||
            (u4Dst == (UINT32)GFX_COLORMODE_RGB_D565)     ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D1555)   ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D4444)   ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888)   ||
            (u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_D8888))
        {
            if ((u4Op == (UINT32)OP_GRAD_FILL) &&
                ((u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) || 
                 (u4Dst == (UINT32)GFX_COLORMODE_ARGB_CLUT8)))
            {
                return (INT32)GFX_ERROR_INTERNAL;
            }

            return (INT32)GFX_OK;
        }

        return (INT32)GFX_ERROR_INTERNAL;
    }

    // rule:5 - dma mode
    if (u4Op == (UINT32)OP_DMA)
    {
        if ((u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_D8888)   || 
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888))
        {
            return (INT32)GFX_OK;
        }

        return (INT32)GFX_ERROR_INTERNAL;
    }

    // rule:6 - ybr2rgb mode
    if (u4Op == (UINT32)OP_YCBCR_TO_RGB)
    {
        if (((u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT8))    &&
            ((u4Dst == (UINT32)GFX_COLORMODE_RGB_D565)     ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D1555)   ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D4444)   ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888)))
        {
            return (INT32)GFX_OK;
        }

        return (INT32)GFX_ERROR_INTERNAL;
    }
    
    // rule:7 - alphamap mode
    if (u4Op == (UINT32)OP_ALPHA_MAP_BITBLT)
    {
        if (((u4Src == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) ||
             (u4Src == (UINT32)GFX_COLORMODE_ARGB_CLUT8))    &&
            ((u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888)   ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D4444)   ||
             (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D1555)))
        {
            return (INT32)GFX_OK;
        }

        return (INT32)GFX_ERROR_INTERNAL;
    }
    
    // rule:8 - all other mode
    if (u4Src == u4Dst)
    {
        if ((u4Dst == (UINT32)GFX_COLORMODE_RGB_D565)   ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D1555) ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D4444) ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_D8888) ||
            (u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_D8888))
        {
            return (INT32)GFX_OK;
        }

        if ((u4Dst == (UINT32)GFX_COLORMODE_AYCbCr_CLUT8) ||
            (u4Dst == (UINT32)GFX_COLORMODE_ARGB_CLUT8))
        {
            if ((u4Op == (UINT32)OP_STRETCH_BITBLT)     ||
                (u4Op == (UINT32)OP_BITBLT)             ||
                (u4Op == (UINT32)OP_LINE_HORI_TO_VERT)  ||
                (u4Op == (UINT32)OP_ROP_BITBLT)         ||
                (u4Op == (UINT32)OP_NEW_COMPRESS_BITBLT)||
                (u4Op == (UINT32)OP_IMGRZ_SCALING))
            {
                return (INT32)GFX_OK;
            }
        }
    }
    
    if (u4Op == (UINT32)OP_IMGRZ_SCALING)
    {
       return (INT32)GFX_OK;
    }

    return (INT32)GFX_ERROR_INTERNAL;
}


//-------------------------------------------------------------------------
/** _GetInterface
 *  
 *  
 */
//-------------------------------------------------------------------------
static INT32 _GetInterface(DRV_COMP_ID_T *pt_comp_id, DRV_GET_TYPE_T e_get_type,
    VOID *pv_get_info, SIZE_T *pz_get_info_len)
{
    INT8 i = 0;

    if (pt_comp_id == NULL)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    if (pz_get_info_len == NULL)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    // has several GFX components
    //for (i = 0; i < _GetCompCount(); i++)
    {
        //if (_IsSameCompId(pt_comp_id, _GetComponent(i)->e_type, 
        //        _GetComponent(i)->ui2_id))
        {
            return ((_GetComponent(i)->pfGet)(i, e_get_type, pv_get_info, 
                pz_get_info_len));
        }
    }

    //return RMR_DRV_INV_GET_INFO;
}


//-------------------------------------------------------------------------
/** _SetInterface
 *  
 *  
 */
//-------------------------------------------------------------------------
static INT32 _SetInterface(DRV_COMP_ID_T *pt_comp_id, DRV_SET_TYPE_T e_set_type, 
    const VOID *pv_set_info, SIZE_T z_set_info_len)
{
    INT8 i = 0;

    if (e_set_type == GFX_SET_TYPE_FLUSH_OPQUEUE)
    {
        if (pt_comp_id == NULL)     // only check pt_comp_id
        {
            return RMR_DRV_INV_SET_INFO;
        }
    }
    else
    {
        if ((pv_set_info == NULL) || (pt_comp_id == NULL))
        {
            return RMR_DRV_INV_SET_INFO;
        }
    }

    if ((z_set_info_len != 0) || (e_set_type == GFX_SET_TYPE_FLUSH_OPQUEUE))
    {
        // has several GFX components
        //for (i = 0; i < _GetCompCount(); i++)
        {
            //if (_IsSameCompId(pt_comp_id, _GetComponent(i)->e_type, 
            //        _GetComponent(i)->ui2_id))
            {
                return ((_GetComponent(i)->pfSet)(i, e_set_type, 
                    (void *)pv_set_info, z_set_info_len));
            }
        }
    }

    return RMR_DRV_INV_SET_INFO;
}


//-------------------------------------------------------------------------
/** _GfxOpen
 *  gfx constructor
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxOpen(void)
{
    // GFX engine initialization
    GFX_Init();
    #if defined(CC_MT8283_OSD_COMPRESS) && defined(CC_FLIP_MIRROR_SUPPORT)
    _u1MirrorFlipState=GFX_GetMirrorFlip();
    #endif
#if 0
    // GFX-SCalar engine initialization
    GFXSC_Init();
#endif
    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxClose
 *  gfx destructor
 *  for future use
 */
//-------------------------------------------------------------------------
/*
static void _GfxClose(void)
{
    return;
}
*/


//-------------------------------------------------------------------------
/** _GfxFillRect
 *  draw rectangular
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxFillRect(const GFX_FILL_T *prFill)
{
    UINT32 ux, uy;

    NULL_PT_CHECK(prFill);
    XYWH_CHECK(prFill->i4_dst_x, prFill->i4_dst_y, prFill->ui4_width, 
        prFill->ui4_height);

    if (_GfxOpModeCheck((UINT32)OP_RECT_FILL, 
            (UINT32)prFill->e_dst_cm, (UINT32)prFill->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    ux = (UINT32)prFill->i4_dst_x;
    uy = (UINT32)prFill->i4_dst_y;

    GFX_Lock();
    
    GFX_SetDst((UINT8 *)prFill->pv_dst, 
        _GfxGetColorMode(prFill->e_dst_cm), prFill->ui4_dst_pitch);
        
    GFX_SetColor(prFill->ui4_color);

    GFX_Fill(ux, uy, prFill->ui4_width, prFill->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif
    
    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxDrawHLine
 *  draw horizontal line
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxDrawHLine(const GFX_FILL_T *prFill)
{
    UINT32 ux, uy;

    NULL_PT_CHECK(prFill);
    XYWH_CHECK(prFill->i4_dst_x, prFill->i4_dst_y, prFill->ui4_width, 
        prFill->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_DRAW_HLINE, 
            (UINT32)prFill->e_dst_cm, (UINT32)prFill->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    ux = (UINT32)prFill->i4_dst_x;
    uy = (UINT32)prFill->i4_dst_y;

    GFX_Lock();

    GFX_SetDst((UINT8 *)prFill->pv_dst, 
        _GfxGetColorMode(prFill->e_dst_cm), prFill->ui4_dst_pitch);

    GFX_SetColor(prFill->ui4_color);

    GFX_HLine(ux, uy, prFill->ui4_width);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif
    
    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxDrawVLine
 *  draw vertical line
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxDrawVLine(const GFX_FILL_T *prFill)
{
    UINT32 ux, uy;

    NULL_PT_CHECK(prFill);
    XYWH_CHECK(prFill->i4_dst_x, prFill->i4_dst_y, 
        prFill->ui4_width, prFill->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_DRAW_VLINE, 
            (UINT32)prFill->e_dst_cm, (UINT32)prFill->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    ux = (UINT32)prFill->i4_dst_x;
    uy = (UINT32)prFill->i4_dst_y;

    GFX_Lock();

    GFX_SetDst((UINT8 *)prFill->pv_dst, 
        _GfxGetColorMode(prFill->e_dst_cm), prFill->ui4_dst_pitch);

    GFX_SetColor(prFill->ui4_color);

    GFX_VLine(ux, uy, prFill->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxBitblt
 *  Normal bitblt
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxBitblt(const GFX_BITBLT_T *prBitblt)
{
    UINT32 usx, udx, usy, udy;

    NULL_PT_CHECK(prBitblt);
    XYWH_CHECK(prBitblt->i4_src_x, prBitblt->i4_src_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);
    XYWH_CHECK(prBitblt->i4_dst_x, prBitblt->i4_dst_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);
    
    if(_GfxOpModeCheck((UINT32)OP_BITBLT, 
            (UINT32)prBitblt->e_src_cm, (UINT32)prBitblt->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prBitblt->i4_src_x;
    usy = (UINT32)prBitblt->i4_src_y;
    udx = (UINT32)prBitblt->i4_dst_x;
    udy = (UINT32)prBitblt->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8 *)prBitblt->pv_src, 
        _GfxGetColorMode(prBitblt->e_src_cm), prBitblt->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prBitblt->pv_dst, 
        _GfxGetColorMode(prBitblt->e_dst_cm), prBitblt->ui4_dst_pitch);

    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0x00000000, 0xffffffff);

    GFX_SetAlpha(prBitblt->ui1_alpha);

    GFX_BitBlt(usx, usy, udx, udy, prBitblt->ui4_width, prBitblt->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}

static GFX_ERROR_T _GfxOpBitBlt(const GFX_OPBITBLT_T *prBitblt)
{
	UINT32 usx, udx, usy, udy;

    NULL_PT_CHECK(prBitblt);
    XYWH_CHECK(prBitblt->i4_src_x, prBitblt->i4_src_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);
    XYWH_CHECK(prBitblt->i4_dst_x, prBitblt->i4_dst_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);
    
    if(_GfxOpModeCheck((UINT32)OP_BITBLT, 
            (UINT32)prBitblt->e_src_cm, (UINT32)prBitblt->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prBitblt->i4_src_x;
    usy = (UINT32)prBitblt->i4_src_y;
    udx = (UINT32)prBitblt->i4_dst_x;
    udy = (UINT32)prBitblt->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8 *)prBitblt->pv_src, 
        _GfxGetColorMode(prBitblt->e_src_cm), prBitblt->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prBitblt->pv_dst, 
        _GfxGetColorMode(prBitblt->e_dst_cm), prBitblt->ui4_dst_pitch);

    GFX_SetBltOpt(prBitblt->u4Switch, 0x00000000, 0x00000000);

    GFX_SetAlpha(prBitblt->ui1_alpha);

    GFX_BitBlt(usx, usy, udx, udy, prBitblt->ui4_width, prBitblt->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}




//-------------------------------------------------------------------------
/** _GfxTransparentBitblt
 *  Normal bitblt (transparent mode)
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxTransparentBitblt(const GFX_TRANSPARENT_BITBLT_T *prBitblt)
{
    UINT32 usx, udx, usy, udy;

    NULL_PT_CHECK(prBitblt);
    XYWH_CHECK(prBitblt->i4_src_x, prBitblt->i4_src_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);
    XYWH_CHECK(prBitblt->i4_dst_x, prBitblt->i4_dst_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);

    // aditional mode check due to hw
    if (prBitblt->e_src_cm != prBitblt->e_dst_cm)
    {
        return GFX_ERROR_INTERNAL;
    }

    if(_GfxOpModeCheck((UINT32)OP_BITBLT, 
            (UINT32)prBitblt->e_src_cm, (UINT32)prBitblt->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prBitblt->i4_src_x;
    usy = (UINT32)prBitblt->i4_src_y;
    udx = (UINT32)prBitblt->i4_dst_x;
    udy = (UINT32)prBitblt->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8*)prBitblt->pv_src, 
        _GfxGetColorMode(prBitblt->e_src_cm), prBitblt->ui4_src_pitch);

    GFX_SetDst((UINT8*)prBitblt->pv_dst, 
        _GfxGetColorMode(prBitblt->e_dst_cm), prBitblt->ui4_dst_pitch);

    GFX_SetBltOpt(
        ((D_GFXFLAG_TRANSPARENT) | 
         (prBitblt->b_inverse_masking ? D_GFXFLAG_KEYNOT : 0)),
        prBitblt->ui4_color_space_min, prBitblt->ui4_color_space_max);

    GFX_BitBlt(usx, usy, udx, udy, 
        prBitblt->ui4_width, prBitblt->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxTransparentFill
 *  Normal bitblt (color change mode)
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxTransparentFill(const GFX_TRANSPARENT_FILL_T *prFill)
{
    UINT32 usx, udx, usy, udy;

    NULL_PT_CHECK(prFill);
    XYWH_CHECK(prFill->i4_src_x, prFill->i4_src_y, 
        prFill->ui4_width, prFill->ui4_height);
    XYWH_CHECK(prFill->i4_dst_x, prFill->i4_dst_y, 
        prFill->ui4_width, prFill->ui4_height);
        
    // aditional mode check due to hw
    if (prFill->e_src_cm != prFill->e_dst_cm)
    {
        return GFX_ERROR_INTERNAL;
    }

    if(_GfxOpModeCheck((UINT32)OP_BITBLT, 
            (UINT32)prFill->e_src_cm, (UINT32)prFill->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prFill->i4_src_x;
    usy = (UINT32)prFill->i4_src_y;
    udx = (UINT32)prFill->i4_dst_x;
    udy = (UINT32)prFill->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8*)prFill->pv_src, _GfxGetColorMode(prFill->e_src_cm),
        prFill->ui4_src_pitch);

    GFX_SetDst((UINT8*)prFill->pv_dst, _GfxGetColorMode(prFill->e_dst_cm),
        prFill->ui4_dst_pitch);

    GFX_SetBltOpt(
        ((D_GFXFLAG_COLORCHANGE) | 
         (prFill->b_inverse_masking ? D_GFXFLAG_KEYNOT : 0)),
        prFill->ui4_color_space_min, prFill->ui4_color_space_max);

    GFX_SetColor(prFill->ui4_fill_color);

    GFX_BitBlt(usx, usy, udx, udy, prFill->ui4_width, prFill->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxAlphaBlending
 *  Alpha blending
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxAlphaBlending(const GFX_ALPHA_BLEND_T *prBitblt)
{
    UINT32 usx, usy, udx, udy;

    NULL_PT_CHECK(prBitblt);
    XYWH_CHECK(prBitblt->i4_src_x, prBitblt->i4_src_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);
    XYWH_CHECK(prBitblt->i4_dst_x, prBitblt->i4_dst_y, 
        prBitblt->ui4_width, prBitblt->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_ALPHA_BLEND, 
            (UINT32)prBitblt->e_src_cm, (UINT32)prBitblt->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prBitblt->i4_src_x;
    usy = (UINT32)prBitblt->i4_src_y;
    udx = (UINT32)prBitblt->i4_dst_x;
    udy = (UINT32)prBitblt->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8*)prBitblt->pv_src, 
        _GfxGetColorMode(prBitblt->e_src_cm), prBitblt->ui4_src_pitch);

    GFX_SetDst((UINT8*)prBitblt->pv_dst, 
        _GfxGetColorMode(prBitblt->e_dst_cm), prBitblt->ui4_dst_pitch);

    GFX_SetAlpha((UINT32)prBitblt->ui1_alpha);

    GFX_Blend(usx, usy, udx, udy, prBitblt->ui4_width, prBitblt->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxExtAlphaMul
 *  Alpha Composition : Pass 0
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxExtAlphaMul(const GFX_ALPHA_MUL_T *prAlphaMul)
{
    UINT32 usx, usy, udx, udy;

    NULL_PT_CHECK(prAlphaMul);
    XYWH_CHECK(prAlphaMul->i4_src_x, prAlphaMul->i4_src_y, 
        prAlphaMul->ui4_width, prAlphaMul->ui4_height);
    XYWH_CHECK(prAlphaMul->i4_dst_x, prAlphaMul->i4_dst_y, 
        prAlphaMul->ui4_width, prAlphaMul->ui4_height);
    
    if(_GfxOpModeCheck((UINT32)OP_ALPHA_COMPOSE, 
            (UINT32)prAlphaMul->e_src_cm, (UINT32)prAlphaMul->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prAlphaMul->i4_src_x;
    usy = (UINT32)prAlphaMul->i4_src_y;
    udx = (UINT32)prAlphaMul->i4_dst_x;
    udy = (UINT32)prAlphaMul->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8 *)prAlphaMul->pv_src, 
        _GfxGetColorMode(prAlphaMul->e_src_cm), prAlphaMul->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prAlphaMul->pv_dst, 
        _GfxGetColorMode(prAlphaMul->e_dst_cm), prAlphaMul->ui4_dst_pitch);

    GFX_SetAlpha(prAlphaMul->ui1_alpha);

    GFX_AlphaComposePass(usx, usy, udx, udy, prAlphaMul->ui4_width, 
        prAlphaMul->ui4_height, 0, prAlphaMul->ui1_alpha);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxAlphaBitblt
 *  Alpha Composition : Pass 4
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxAlphaBitblt(const GFX_BITBLT_BASE_T *prBltBase)
{
    UINT32 usx, usy, udx, udy;

    NULL_PT_CHECK(prBltBase);
    XYWH_CHECK(prBltBase->i4_src_x, prBltBase->i4_src_y, 
        prBltBase->ui4_width, prBltBase->ui4_height);
    XYWH_CHECK(prBltBase->i4_dst_x, prBltBase->i4_dst_y, 
        prBltBase->ui4_width, prBltBase->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_ALPHA_COMPOSE, 
            (UINT32)prBltBase->e_src_cm, (UINT32)prBltBase->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prBltBase->i4_src_x;
    usy = (UINT32)prBltBase->i4_src_y;
    udx = (UINT32)prBltBase->i4_dst_x;
    udy = (UINT32)prBltBase->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8 *)prBltBase->pv_src, 
        _GfxGetColorMode(prBltBase->e_src_cm), prBltBase->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prBltBase->pv_dst, 
        _GfxGetColorMode(prBltBase->e_dst_cm), prBltBase->ui4_dst_pitch);

    GFX_AlphaComposePass(usx, usy, udx, udy,
        prBltBase->ui4_width, prBltBase->ui4_height, 4, 255);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxColorBitblt
 *  Alpha Composition : Pass 5
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxColorBitblt(const GFX_BITBLT_BASE_T *prBltBase)
{
    UINT32 usx, usy, udx, udy;

    NULL_PT_CHECK(prBltBase);
    XYWH_CHECK(prBltBase->i4_src_x, prBltBase->i4_src_y, 
        prBltBase->ui4_width, prBltBase->ui4_height);
    XYWH_CHECK(prBltBase->i4_dst_x, prBltBase->i4_dst_y, 
        prBltBase->ui4_width, prBltBase->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_ALPHA_COMPOSE, 
            (UINT32)prBltBase->e_src_cm, (UINT32)prBltBase->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prBltBase->i4_src_x;
    usy = (UINT32)prBltBase->i4_src_y;
    udx = (UINT32)prBltBase->i4_dst_x;
    udy = (UINT32)prBltBase->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8 *)prBltBase->pv_src, 
        _GfxGetColorMode(prBltBase->e_src_cm), prBltBase->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prBltBase->pv_dst, 
        _GfxGetColorMode(prBltBase->e_dst_cm), prBltBase->ui4_dst_pitch);

    GFX_AlphaComposePass(usx, usy, udx, udy,
        prBltBase->ui4_width, prBltBase->ui4_height, 5, 255);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxAlphaComposition
 *  Alpha Composition Loop Mode (Pass 0 - Pass 3)
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxAlphaComposition(const GFX_ALPHA_COMPOSITION_T *prAcomp)
{
    UINT32 usx, usy, udx, udy, uar, upd, u4w, u4h;
    UINT32 u4RectSrcFlag;

    NULL_PT_CHECK(prAcomp);
    XYWH_CHECK(prAcomp->i4_src_x, prAcomp->i4_src_y, 
        prAcomp->ui4_width, prAcomp->ui4_height);
    XYWH_CHECK(prAcomp->i4_dst_x, prAcomp->i4_dst_y, 
        prAcomp->ui4_width, prAcomp->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_ALPHA_COMPOSE_LOOP, 
            (UINT32)prAcomp->e_src_cm, (UINT32)prAcomp->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prAcomp->i4_src_x;
    usy = (UINT32)prAcomp->i4_src_y;
    udx = (UINT32)prAcomp->i4_dst_x;
    udy = (UINT32)prAcomp->i4_dst_y;
    uar = (UINT32)prAcomp->ui1_alpha;
    upd = (UINT32)prAcomp->e_rule;
    
    u4w = prAcomp->ui4_width;
    u4h = prAcomp->ui4_height;
    
    u4RectSrcFlag = (UINT32)prAcomp->b_rect_src_option;

    GFX_Lock();

    if (u4RectSrcFlag == 0)
    {
        GFX_SetSrc((UINT8*)prAcomp->pv_src, 
            _GfxGetColorMode(prAcomp->e_src_cm), prAcomp->ui4_src_pitch);
    }
    else
    {
    	GFX_SetSrc((UINT8*)prAcomp->pv_dst,     //  rect src compose, both the src addr and src pitch  are NULL  
            _GfxGetColorMode(prAcomp->e_src_cm), prAcomp->ui4_dst_pitch);  //set src color mode
        GFX_SetColor(prAcomp->ui4_color);    
    }    

    GFX_SetDst((UINT8*)prAcomp->pv_dst, 
        _GfxGetColorMode(prAcomp->e_dst_cm), prAcomp->ui4_dst_pitch);

    GFX_ComposeLoop(usx, usy, udx, udy, u4w, u4h, uar, upd, u4RectSrcFlag);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxYbr2Rgb
 *  YCbCr to RGB conversion
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxYbr2Rgb(const GFX_YCBCR_TO_RGB_T *prYbr2Rgb)
{
    UINT32 udx, udy;
    UINT32 u4YcFmt;

    NULL_PT_CHECK(prYbr2Rgb);
    NULL_PT_CHECK(prYbr2Rgb->pv_src);
    NULL_PT_CHECK(prYbr2Rgb->pv_dst);
    NULL_PT_CHECK(prYbr2Rgb->pv_cbcr);
    XYWH_CHECK(prYbr2Rgb->i4_src_x, prYbr2Rgb->i4_src_y, 
        prYbr2Rgb->ui4_width, prYbr2Rgb->ui4_height);
    XYWH_CHECK(prYbr2Rgb->i4_dst_x, prYbr2Rgb->i4_dst_y, 
        prYbr2Rgb->ui4_width, prYbr2Rgb->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_YCBCR_TO_RGB, 
            (UINT32)prYbr2Rgb->e_src_cm, (UINT32)prYbr2Rgb->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    u4YcFmt = _GfxGetYcFormat(prYbr2Rgb->e_ycbcr_format);
    
    // check valid YC format
    if (u4YcFmt == (UINT32)E_YCFMT_RES_2)
    {
        return GFX_ERROR_INTERNAL;
    }

    // check valid pitch
    if (u4YcFmt == (UINT32)E_YCFMT_420MB)
    {
        if ((prYbr2Rgb->ui4_cbcr_pitch << 1) != prYbr2Rgb->ui4_src_pitch)
        {
            return GFX_ERROR_INTERNAL;
        }
    }
    else    // 420 or 422 linear
    {
        if (prYbr2Rgb->ui4_cbcr_pitch != prYbr2Rgb->ui4_src_pitch)
        {
            return GFX_ERROR_INTERNAL;
        }
    }

    // check engine with 422-linear function
    // To do ....
    // ..........
    
    udx = (UINT32)prYbr2Rgb->i4_dst_x;
    udy = (UINT32)prYbr2Rgb->i4_dst_y;

    GFX_Lock();

    GFX_SetColCnvFmt(u4YcFmt, (UINT32)E_SWAP_BLOCK, (UINT32)E_VSTD_BT709, 
        (UINT32)E_VSYS_COMP);

    GFX_SetColCnvSrc((UINT8*)prYbr2Rgb->pv_src, prYbr2Rgb->ui4_src_pitch,
        (UINT8*)prYbr2Rgb->pv_cbcr, prYbr2Rgb->ui4_cbcr_pitch, 0);

    GFX_SetDst((UINT8*)prYbr2Rgb->pv_dst, 
        _GfxGetColorMode(prYbr2Rgb->e_dst_cm), prYbr2Rgb->ui4_dst_pitch);

    GFX_SetAlpha((UINT32)prYbr2Rgb->ui1_alpha);

    GFX_ColConv(udx, udy, prYbr2Rgb->ui4_width, prYbr2Rgb->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxStretchBitblt
 *  Scaling (repeat and drop)
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxStretchBitblt(const GFX_STRETCH_BITBLT_T *prStretch)
{
    UINT32 usx, udx, usy, udy;
    UINT32 u4wr, u4hr;
    UINT32 u4PixBits;

    NULL_PT_CHECK(prStretch);
    XYWH_CHECK(prStretch->i4_src_x, prStretch->i4_src_y, 
        prStretch->ui4_width, prStretch->ui4_height);
    XYWH_CHECK(prStretch->i4_dst_x, prStretch->i4_dst_y, 
        prStretch->ui4_dst_width, prStretch->ui4_dst_height);

    if(_GfxOpModeCheck((UINT32)OP_STRETCH_BITBLT, 
            (UINT32)prStretch->e_src_cm, (UINT32)prStretch->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prStretch->i4_src_x;
    usy = (UINT32)prStretch->i4_src_y;
    udx = (UINT32)prStretch->i4_dst_x;
    udy = (UINT32)prStretch->i4_dst_y;

    u4wr = (prStretch->ui4_dst_width << 16) / prStretch->ui4_width;
    u4hr = (prStretch->ui4_dst_height << 16) / prStretch->ui4_height;
    
    if ((u4wr > (GFX_STRETCH_SCALE << 16)) || (u4wr < ((1<<16)/GFX_STRETCH_SCALE)))
    {
        return GFX_ERROR_INTERNAL;
    }
    
    if ((u4hr > (GFX_STRETCH_SCALE << 16)) || (u4hr < ((1<<16)/GFX_STRETCH_SCALE)))
    {
        return GFX_ERROR_INTERNAL;
    }

    u4PixBits = _GetGfxBitsPerPixel(_GfxGetColorMode(prStretch->e_src_cm));
    if ((u4PixBits * prStretch->ui4_width) < GFX_MIN_STR_BITS)
    {
        return GFX_ERROR_INTERNAL;
    }

    u4PixBits = _GetGfxBitsPerPixel(_GfxGetColorMode(prStretch->e_dst_cm));
    if ((u4PixBits * prStretch->ui4_dst_width) < GFX_MIN_STR_BITS)
    {
        return GFX_ERROR_INTERNAL;
    }

    GFX_Lock();

    GFX_SetSrc((UINT8*)prStretch->pv_src, 
        _GfxGetColorMode(prStretch->e_src_cm), prStretch->ui4_src_pitch);

    GFX_SetDst((UINT8*)prStretch->pv_dst, 
        _GfxGetColorMode(prStretch->e_dst_cm), prStretch->ui4_dst_pitch);

    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0, 0xffffffff);

    GFX_StretchBlt(usx, usy, prStretch->ui4_width, prStretch->ui4_height,
        udx, udy, prStretch->ui4_dst_width, prStretch->ui4_dst_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxAlphaMapBitblt
 *  Alpha Map Bitblt
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxAlphaMapBitblt(const GFX_ALPHAMAP_BITBLT_T *prAlphaMap)
{
    UINT32 usx, usy, udx, udy;
    UINT32 u4SrcCM;

    NULL_PT_CHECK(prAlphaMap);
    XYWH_CHECK(prAlphaMap->i4_src_x, prAlphaMap->i4_src_y, 
        prAlphaMap->ui4_width, prAlphaMap->ui4_height);
    XYWH_CHECK(prAlphaMap->i4_dst_x, prAlphaMap->i4_dst_y, 
        prAlphaMap->ui4_width, prAlphaMap->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_ALPHA_MAP_BITBLT, 
            (UINT32)prAlphaMap->e_src_cm, (UINT32)prAlphaMap->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prAlphaMap->i4_src_x;
    usy = (UINT32)prAlphaMap->i4_src_y;
    udx = (UINT32)prAlphaMap->i4_dst_x;
    udy = (UINT32)prAlphaMap->i4_dst_y;

    // CM_YCbCr_CLUT8 changed to CM_RGB_CLUT8
    u4SrcCM = _GfxGetColorMode(prAlphaMap->e_src_cm);
    if (u4SrcCM == (UINT32)CM_YCbCr_CLUT8)
    {
        u4SrcCM = (UINT32)CM_RGB_CLUT8;
    }
    
    GFX_Lock();

    GFX_SetSrc((UINT8 *)prAlphaMap->pv_src, 
        u4SrcCM, prAlphaMap->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prAlphaMap->pv_dst, 
        _GfxGetColorMode(prAlphaMap->e_dst_cm), prAlphaMap->ui4_dst_pitch);

    GFX_AlphaMapBitBlt(usx, usy, udx, udy, prAlphaMap->ui4_width, 
        prAlphaMap->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxRopBitblt
 *  ROP bitblt
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxRopBitblt(const GFX_ROP_BITBLT_T *prRop)
{
    UINT32 usx, udx, usy, udy;
    UINT32 u4RopCode;

    NULL_PT_CHECK(prRop);
    XYWH_CHECK(prRop->i4_src_x, prRop->i4_src_y, 
        prRop->ui4_width, prRop->ui4_height);
    XYWH_CHECK(prRop->i4_dst_x, prRop->i4_dst_y, 
        prRop->ui4_width, prRop->ui4_height);
    
    if(_GfxOpModeCheck((UINT32)OP_ROP_BITBLT, 
            (UINT32)prRop->e_src_cm, (UINT32)prRop->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    u4RopCode = _GfxGetRopCode(prRop->e_rop_type);
#if defined(CC_MT5389)
	 if (u4RopCode == (UINT32)E_ROP_MAX)
#else	
    if (u4RopCode == (UINT32)E_ROP_RESERVED0)
#endif
    {
        return GFX_ERROR_INTERNAL;
    }

    usx = (UINT32)prRop->i4_src_x;
    usy = (UINT32)prRop->i4_src_y;
    udx = (UINT32)prRop->i4_dst_x;
    udy = (UINT32)prRop->i4_dst_y;

    GFX_Lock();

    GFX_SetSrc((UINT8 *)prRop->pv_src, 
        _GfxGetColorMode(prRop->e_src_cm), prRop->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prRop->pv_dst, 
        _GfxGetColorMode(prRop->e_dst_cm), prRop->ui4_dst_pitch);

    GFX_SetRopOpt(u4RopCode);

    GFX_RopBitBlt(usx, usy, udx, udy, prRop->ui4_width, prRop->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxRotate90
 *  rotate a bmp (90" counter-clock-wise or clock-wise)
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxRotate90(const GFX_ROTATE_T *prRotate)
{
    INT32 i4Sx, i4Sy, i4Dx, i4Dy;
    UINT32 i;

    NULL_PT_CHECK(prRotate);
    XYWH_CHECK(prRotate->i4_src_x, prRotate->i4_src_y, 
        prRotate->ui4_width, prRotate->ui4_height);
    XYWH_CHECK(prRotate->i4_dst_x, prRotate->i4_dst_y, 
        prRotate->ui4_width, prRotate->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_LINE_HORI_TO_VERT, 
            (UINT32)prRotate->e_src_cm, (UINT32)prRotate->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }
    
    i4Sx = prRotate->i4_src_x;
    i4Sy = prRotate->i4_src_y;
    i4Dx = prRotate->i4_dst_x;
    i4Dy = prRotate->i4_dst_y;
    
    GFX_Lock();

    GFX_SetSrc((UINT8 *)prRotate->pv_src, 
        _GfxGetColorMode(prRotate->e_src_cm), prRotate->ui4_src_pitch);

    GFX_SetDst((UINT8 *)prRotate->pv_dst, 
        _GfxGetColorMode(prRotate->e_dst_cm), prRotate->ui4_dst_pitch);

    if (prRotate->e_direction == GFX_C_CLOCKWISE)
    {
        // rotated by 90" counter-clock-wise
        GFX_SetHoriToVertLineOpt(1);

        for(i = 0; i < prRotate->ui4_height; i++)
        {
            GFX_HoriToVertLine((UINT32)i4Sx, (UINT32)i4Sy, 
                (UINT32)i4Dx, (UINT32)i4Dy, prRotate->ui4_width);

            i4Sy++;
            i4Dx++;
        } // ~for
    }
    else
    {
        // rotated by 90" clock-wise
        GFX_SetHoriToVertLineOpt(0);

        for(i = 0; i < prRotate->ui4_height; i++)
        {
            GFX_HoriToVertLine((UINT32)i4Sx, (UINT32)i4Sy, 
                (UINT32)i4Dx, (UINT32)i4Dy, prRotate->ui4_width);

            i4Sy++;
            i4Dx--;
        } // ~for
    }

    // flush and wait
    GFX_Flush();
    GFX_Wait();

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxIdx2DirBitblt
 *  Index to direct color mode bitblt
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxIdx2DirBitblt(const GFX_PLTBLT_T *prPale)
{
    UINT32 udx, udy;
    UINT32 u4MsbLeftFlag;
    UINT32 u4FontMode;

    NULL_PT_CHECK(prPale);
    XYWH_CHECK(prPale->i4_src_x, prPale->i4_src_y, 
        prPale->ui4_width, prPale->ui4_height);
    XYWH_CHECK(prPale->i4_dst_x, prPale->i4_dst_y, 
        prPale->ui4_width, prPale->ui4_height);

    if(_GfxOpModeCheck((UINT32)OP_IDX_TO_DIR_BITBLT, 
            (UINT32)prPale->e_src_cm, (UINT32)prPale->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    u4FontMode = _GfxGetFontMode(prPale->e_src_cm);
    if (u4FontMode == (UINT32)E_BMP_CM_1BIT)
    {
        return GFX_ERROR_INTERNAL;
    }
    
    udx = (UINT32)prPale->i4_dst_x;
    udy = (UINT32)prPale->i4_dst_y;

    GFX_Lock();

    GFX_SetIdx2DirSrc((UINT8*)prPale->pv_src, u4FontMode);

    GFX_SetDst((UINT8*)prPale->pv_dst, 
        _GfxGetColorMode(prPale->e_dst_cm), prPale->ui4_dst_pitch);

    if (prPale->e_byte_aligned == GFX_PALETTE_MSB)
    {
        u4MsbLeftFlag = (UINT32)E_IDX2DIR_MSB_LEFT_ON;
    }
    else
    {
        u4MsbLeftFlag = (UINT32)E_IDX2DIR_MSB_LEFT_OFF;
    }
    
    GFX_SetIdx2DirOpt((UINT8 *)prPale->h_palette, u4MsbLeftFlag, 0);

    GFX_Idx2DirBitBlt(udx, udy, prPale->ui4_width, prPale->ui4_height);

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxFlushQueue
 *  flush queue
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxFlushQueue(const VOID *pvTag)
{
    GFX_Lock();

    // set MW callback parameters
    _rGfxNfyData.rCond  = GFX_COND_IDLE_IN_THREAD;
    _rGfxNfyData.pvTag  = pvTag;
    _rGfxNfyData.u4Data = GFX_EXEC_OK;
    
    GFX_MwFlush();

    GFX_Unlock();

    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxSetCallBackFct
 *  set graphics call-back function
 *
 */
//-------------------------------------------------------------------------
static GFX_ERROR_T _GfxSetCallBackFct(GFX_NFY_FCT_T pfCallBack)
{
    _pfnGfxNfyFct = pfCallBack;

    GFX_SetNotify(_fnNfyFct);

    return GFX_OK;
}

static GFX_ERROR_T _GfxCompressBitblt(const GFX_COMPRESSED_BITBLT_T *prCompBlt)
{
    UINT32 usx, usy;
    //UINT32 udx, udy;
    #ifdef CC_MT8283_OSD_COMPRESS
    OSD_OP_MODE_T OSD_OP_MODE;
    #endif
    
    NULL_PT_CHECK(prCompBlt);
    XYWH_CHECK(prCompBlt->i4_src_x, prCompBlt->i4_src_y, 
        prCompBlt->ui4_width, prCompBlt->ui4_height);
    XYWH_CHECK(prCompBlt->i4_dst_x, prCompBlt->i4_dst_y, 
        prCompBlt->ui4_width, prCompBlt->ui4_height);
    
    if(_GfxOpModeCheck((UINT32)OP_NEW_COMPRESS_BITBLT, 
            (UINT32)prCompBlt->e_src_cm, (UINT32)prCompBlt->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    //GFX_ASSERT((u4Width % 8) == 0);
    
    usx = (UINT32)prCompBlt->i4_src_x;
    usy = (UINT32)prCompBlt->i4_src_y;
    //udx = (UINT32)prCompBlt->i4_dst_x;
    //udy = (UINT32)prCompBlt->i4_dst_y;

    GFX_Lock();

#ifdef CC_FLIP_MIRROR_SUPPORT_GFX
    GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src, usx, usy, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst,prCompBlt->ui4_width,prCompBlt->ui4_height,prCompBlt->ui4_buffer_height,TRUE,
                        (UINT32)prCompBlt->e_ratio,0xFFFFFFFF, TRUE, FALSE, prCompBlt->b_line_separate);
#else

	#ifdef CC_MT8283_OSD_COMPRESS
	OSD_OP_MODE = OSD_DMA_GetMode();
	if(OSD_OP_MODE==OSD_OP_DMA)
	{
		#if defined(CC_FLIP_MIRROR_SUPPORT)
		if(_u1MirrorFlipState& 0x2 ) // mirror on
		{
      GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src, 0, 0, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst+(prCompBlt->ui4_src_pitch*prCompBlt->ui4_height/2),(prCompBlt->ui4_width/2)+8,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        0,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);

		  GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src+(((prCompBlt->ui4_width/2)-8)*4), 0, 0, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst,(prCompBlt->ui4_width/2)+8,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        0,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);
		}
		else
		{
			GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src, 0, 0, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst,(prCompBlt->ui4_width/2)+8,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        0,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);

		GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src+(((prCompBlt->ui4_width/2)-8)*4), 0, 0, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst+(prCompBlt->ui4_src_pitch*prCompBlt->ui4_height/2),(prCompBlt->ui4_width/2)+8,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        0,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);
		}
			
		#else
			GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src, 0, 0, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst,(prCompBlt->ui4_width/2)+8,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        0,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);

		GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src+(((prCompBlt->ui4_width/2)-8)*4), 0, 0, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst+(prCompBlt->ui4_src_pitch*prCompBlt->ui4_height/2),(prCompBlt->ui4_width/2)+8,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        0,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);
		#endif
	}
	else
	{
		GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src, usx, usy, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst,prCompBlt->ui4_width,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        (UINT32)prCompBlt->e_ratio,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);
	}
	
 	#else

    	GFX_NewCompressBlt((UINT8 *)prCompBlt->pv_src, usx, usy, _GfxGetColorMode(prCompBlt->e_src_cm),prCompBlt->ui4_src_pitch,
                        (UINT8 *)prCompBlt->pv_dst,prCompBlt->ui4_width,prCompBlt->ui4_height, prCompBlt->ui4_buffer_height, TRUE,
                        (UINT32)prCompBlt->e_ratio,0xFFFFFFFF, FALSE, FALSE, prCompBlt->b_line_separate);
       #endif

#endif

    GFX_DBG_FLUSH();
#if defined(CC_MT8223)  // risc mode
    GFX_Wait();
#endif

    GFX_Unlock();
    
    return GFX_OK;
}


//-------------------------------------------------------------------------
/** _GfxScale
 *  Gfx-Scalar scale (scaling with taps), by imgrz
 *
 */
//-------------------------------------------------------------------------
#ifndef CC_MT8223
static GFX_ERROR_T _GfxScale(const GFX_STRETCH_BITBLT_T *prStretch)
{
    UINT32 usx, udx, usy, udy;
    //UINT32 u4LpfEn = 1;

    UINT32 u4ValidRatio = 65536;//GFXSC_RATIO_LIMIT;
    
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
	E_RZ_INOUT_TYPE_T eRzType;
    
    NULL_PT_CHECK(prStretch);
    XYWH_CHECK(prStretch->i4_src_x, prStretch->i4_src_y, 
        prStretch->ui4_width, prStretch->ui4_height);
    XYWH_CHECK(prStretch->i4_dst_x, prStretch->i4_dst_y, 
        prStretch->ui4_dst_width, prStretch->ui4_dst_height);
    
    usx = (UINT32)prStretch->i4_src_x;
    usy = (UINT32)prStretch->i4_src_y;
    udx = (UINT32)prStretch->i4_dst_x;
    udy = (UINT32)prStretch->i4_dst_y;

    // check scale operation valid color mode
    if(_GfxOpModeCheck((UINT32)OP_IMGRZ_SCALING, 
            (UINT32)prStretch->e_src_cm, (UINT32)prStretch->e_dst_cm))
    {
        return GFX_ERROR_INTERNAL;
    }

    // check valid ratio
    if (prStretch->ui4_width > prStretch->ui4_dst_width)
    {
        if ((prStretch->ui4_width / prStretch->ui4_dst_width) > u4ValidRatio)
        {
            return GFX_ERROR_INTERNAL;
        }
    }
    
    if (prStretch->ui4_width < prStretch->ui4_dst_width)
    {
        if ((prStretch->ui4_dst_width / prStretch->ui4_width) > u4ValidRatio)
        {
            return GFX_ERROR_INTERNAL;
        }
    }

    if (prStretch->ui4_height > prStretch->ui4_dst_height)
    {
        if ((prStretch->ui4_height / prStretch->ui4_dst_height) > u4ValidRatio)
        {
            return GFX_ERROR_INTERNAL;
        }
    }
    
    if (prStretch->ui4_height < prStretch->ui4_dst_height)
    {
        if ((prStretch->ui4_dst_height / prStretch->ui4_height) > u4ValidRatio)
        {
            return GFX_ERROR_INTERNAL;
        }
    }

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
    
    rSclParam.u4SrcBase     = (UINT32) prStretch->pv_src;
    rSclParam.u4IsSrcDirCM  = (UINT32) 1;
    rSclParam.u4SrcCM       = (prStretch->e_src_cm == GFX_COLORMODE_ARGB_D4444)?
                              (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
    rSclParam.u4SrcBufLen   = (UINT32) ALIGN16(prStretch->ui4_src_pitch);
    rSclParam.u4SrcHOffset  = (UINT32) usx;
    rSclParam.u4SrcVOffset  = (UINT32) usy;
    rSclParam.u4SrcW        = (UINT32) prStretch->ui4_width;
    rSclParam.u4SrcH        = (UINT32) prStretch->ui4_height;

    rSclParam.u4TgBase      = (UINT32) prStretch->pv_dst;
    rSclParam.u4TgHOffset   = (UINT32) udx;
    rSclParam.u4TgVOffset   = (UINT32) udy;
    rSclParam.u4TgW         = (UINT32) prStretch->ui4_dst_width;
    rSclParam.u4TgH         = (UINT32) prStretch->ui4_dst_height;
    rSclParam.u4TgCM        = (prStretch->e_dst_cm == GFX_COLORMODE_ARGB_D4444)?
                              (UINT32) E_RZ_OSD_DIR_CM_ARGB4444:(UINT32) E_RZ_OSD_DIR_CM_AYCbCr8888;
    rSclParam.u4TgBufLen    = (UINT32) ALIGN16(prStretch->ui4_dst_pitch);
	eRzType = (prStretch->e_option == GFX_STRETCH_UI) ? E_RZ_INOUT_TYPE_OSDUI : E_RZ_INOUT_TYPE_OSDMD;
    if(_IMGRZ_UsedByJpeg())
    {
       return GFX_ERROR_ACTION_FAILED;
    }
    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    return GFX_OK;
}
#endif

static GFX_ERROR_T _GfxVGfxScaler(const VGFX_SCALER_PARAM_SET_T *prCompBlt)
{
/*
	BOOL _bMatrix = FALSE;
	VGFX_MATRIX_T *prMartrix;
	VGFX_RECT_SET_T _TextRect;

    float f4Xs;
   	float f4Ys;
   	float f4Hs;
    float f4Ws;
    float f4Xd;
    float f4Yd;
    float f4Hd;
    float f4Wd;
    float f4Temp;

    UINT32 u4MatrixA;
    UINT32 u4MatrixB;
    UINT32 u4MatrixC;
    UINT32 u4MatrixD;
    UINT32 u4MatrixE;
    UINT32 u4MatrixF;
    UINT32 u4MatrixG;
    UINT32 u4MatrixH;
    UINT32 u4MatrixI;
	float fSrcX[4] ,fSrcY[4] ,fDstX[4] ,fDstY[4] ,fIMtrix[3][3];

    if (NULL == prScaler)
    {
        return MTR_ERR_INV;
    }

		
	if( MTVGfx_JugRect(prScaler->_SrcQuard) )
	{
		return;
	}
	else
	{
		_TextRect.u4StartX = prScaler->_SrcQuard.x1;
		_TextRect.u4StartY = prScaler->_SrcQuard.y1;
		_TextRect.u4Width = prScaler->_SrcQuard.x3 - prScaler->_SrcQuard.x1;
		_TextRect.u4Height = prScaler->_SrcQuard.y3 - prScaler->_SrcQuard.y1;
	}
	_bMatrix = MTVGfx_JugRect(prScaler->_DstQuard);
	if( _bMatrix )
	{
   		f4Xs = (float)(_TextRect.u4StartX );
   		f4Ys = (float)(_TextRect.u4StartY);
   		f4Hs = (float)(_TextRect.u4Width);
   		f4Ws = (float)(_TextRect.u4Height);
   		f4Xd = (float)(prScaler->_BBType.u4StartX);
   		f4Yd = (float)(prScaler->_BBType.u4StartY);
   		f4Hd = (float)(prScaler->_BBType.u4Height);
   		f4Wd = (float)(prScaler->_BBType.u4Width);

   		f4Temp = (f4Ws-1)/(f4Wd-1);
   		u4MatrixA = MT_fnum_converter(f4Temp) >> 9;
   		u4MatrixB = 0;
   		f4Temp = f4Xs - f4Xd*(f4Ws-1)/(f4Wd-1);
  		u4MatrixC = MT_fnum_converter(f4Temp) >> 9;
   		u4MatrixD= 0;
   		f4Temp = (f4Hs-1)/(f4Hd-1);
   		u4MatrixE =MT_fnum_converter(f4Temp) >> 9;
   		f4Temp = f4Ys-f4Yd*(f4Hs-1)/(f4Hd-1);
  		u4MatrixF =MT_fnum_converter(f4Temp) >> 9;
   		u4MatrixG = 0;
   		u4MatrixH = 0;
   		f4Temp = 1;
   		u4MatrixI = MT_fnum_converter(f4Temp) >> 9;	

		prScaler->_SetMartix.u4_a = u4MatrixA;
		prScaler->_SetMartix.u4_b = u4MatrixB;
		prScaler->_SetMartix.u4_c = u4MatrixC;
		prScaler->_SetMartix.u4_d = u4MatrixD;
		prScaler->_SetMartix.u4_e = u4MatrixE;
		prScaler->_SetMartix.u4_f = u4MatrixF;
		prScaler->_SetMartix.u4_g = u4MatrixG;
		prScaler->_SetMartix.u4_h = u4MatrixH;
		prScaler->_SetMartix.u4_i = u4MatrixI;
	}
	else
	{
        fSrcX[0] = (float)(_SetVgfxScaler->_SrcQuard.x1);
        fSrcX[1] = (float)(_SetVgfxScaler->_SrcQuard.x3);
        fSrcX[2] = (float)(_SetVgfxScaler->_SrcQuard.x3);
        fSrcX[3] = (float)(_SetVgfxScaler->_SrcQuard.x1);
        fSrcY[0] = (float)(_SetVgfxScaler->_SrcQuard.y1);
        fSrcY[1] = (float)(_SetVgfxScaler->_SrcQuard.y1);
        fSrcY[2] = (float)(_SetVgfxScaler->_SrcQuard.y3);
        fSrcY[3] = (float)(_SetVgfxScaler->_SrcQuard.y3);
        fDstX[0] = (float)(_SetVgfxScaler->_DstQuard.x1);
        fDstX[1] = (float)(_SetVgfxScaler->_DstQuard.x2);
        fDstX[2] = (float)(_SetVgfxScaler->_DstQuard.x3);
        fDstX[3] = (float)(_SetVgfxScaler->_DstQuard.x4);
        fDstY[0] = (float)(_SetVgfxScaler->_DstQuard.y1);
        fDstY[1] = (float)(_SetVgfxScaler->_DstQuard.y2);
        fDstY[2] = (float)(_SetVgfxScaler->_DstQuard.y3);
        fDstY[3] = (float)(_SetVgfxScaler->_DstQuard.y4);

        derive_inverse_transform(fSrcX, fSrcY, fDstX, fDstY, fIMtrix);

        _SetVgfxScaler->_SetMartix.u4_a = fnum_converter(fIMtrix[0][0]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_b = fnum_converter(fIMtrix[0][1]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_c = fnum_converter(fIMtrix[0][2]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_d = fnum_converter(fIMtrix[1][0]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_e = fnum_converter(fIMtrix[1][1]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_f = fnum_converter(fIMtrix[1][2]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_g = fnum_converter(fIMtrix[2][0]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_h = fnum_converter(fIMtrix[2][1]) >> 9;
        _SetVgfxScaler->_SetMartix.u4_i = fnum_converter(fIMtrix[2][2]) >> 9;

	}

    _VGfx_SetVGfxScaler(prCompBlt);
   */
	return GFX_OK;
}

//-------------------------------------------------------------------------
/** _GfxGetCommand
 *  
 *  
 */
//-------------------------------------------------------------------------
static INT32 _GfxGetCommand(INT8 icPlane, DRV_GET_TYPE_T e_get_type,
    VOID *pv_get_info, SIZE_T *pz_get_info_len)
{
    GFX_GPU_CAPS_T *prGpuCaps;
    GFX_GPU_CAPS_EX_T *prGpuCapsEx;

    UNUSED(icPlane);
    UNUSED(pz_get_info_len);

    if (NULL == pv_get_info)
    {
        return RMR_DRV_GET_FAILED;
    }

    switch (e_get_type)
    {
        case GFX_GET_TYPE_GPU_CAPS:
            prGpuCaps = (GFX_GPU_CAPS_T *)pv_get_info;
            prGpuCaps->ui4_gfx_op = 
                (GFX_OP_FILLRECT | 
                 GFX_OP_DRAWHLINE | 
                 GFX_OP_DRAWVLINE | 
                 GFX_OP_BITBLT | 
                 GFX_OP_TRANSPARENT_BITBLT | 
                 GFX_OP_TRANSPARENT_FILL | 
                 GFX_OP_ALPHA_BLENDING | 
                 GFX_OP_EXT_ALPHA_MUL | 
                 GFX_OP_ALPHA_BITBLT | 
                 GFX_OP_COLOR_BITBLT | 
                 GFX_OP_ALPHA_COMPOSITION | 
                 GFX_OP_YCBCR_TO_RGB | 
                 GFX_OP_STRETCH_BITBLT | 
                 GFX_OP_ALPHAMAP_BITBLT | 
                 GFX_OP_ROP_BITBLT | 
                 GFX_OP_ROTATE_90 | 
                 GFX_OP_PALETTE_BITBLT | 
                 GFX_OP_ADV_STRETCH_BITBLT);
            break;

        case GFX_GET_TYPE_GPU_CAPS_EX:
            prGpuCapsEx = (GFX_GPU_CAPS_EX_T *)pv_get_info;
            if (prGpuCapsEx->ui4_type == GFX_CAP_SCALING_LIMIT)
            {
                prGpuCapsEx->u.t_scaling_limit.ui4_max_width = GFX_SCALING_MAX_W;
                prGpuCapsEx->u.t_scaling_limit.ui4_min_width = GFX_SCALING_MIN_W;
                prGpuCapsEx->u.t_scaling_limit.ui4_max_height = GFX_SCALING_MAX_H;
                prGpuCapsEx->u.t_scaling_limit.ui4_min_height = GFX_SCALING_MIN_H;
            }
            break;
            
        default:
            return RMR_DRV_INV_GET_INFO;
    }

    return RMR_OK;
}


//-------------------------------------------------------------------------
/** _GfxSetCommand
 *  
 *  
 */
//-------------------------------------------------------------------------
static INT32 _GfxSetCommand(INT8 icPlane, DRV_SET_TYPE_T e_set_type,
    const VOID *pv_set_info, SIZE_T z_set_info_len)
{
    UNUSED(icPlane);
    UNUSED(z_set_info_len);
    
    switch (e_set_type)
    {
        case GFX_SET_TYPE_FILL_RECT:
            SET_COMMAND(_GfxFillRect(pv_set_info))

        case GFX_SET_TYPE_DRAW_LINE_H:
            SET_COMMAND(_GfxDrawHLine(pv_set_info))

        case GFX_SET_TYPE_DRAW_LINE_V:
            SET_COMMAND(_GfxDrawVLine(pv_set_info))

        case GFX_SET_TYPE_BITBLT:
            SET_COMMAND(_GfxBitblt(pv_set_info))

		case GFX_SET_TYPE_OPBITBLT:
            SET_COMMAND(_GfxOpBitBlt(pv_set_info))

        case GFX_SET_TYPE_TRANSPARENT_BITBLT:
            SET_COMMAND(_GfxTransparentBitblt(pv_set_info))

        case GFX_SET_TYPE_TRANSPARENT_FILL:
            SET_COMMAND(_GfxTransparentFill(pv_set_info))

        case GFX_SET_TYPE_ALPHA_BLENDING:
            SET_COMMAND(_GfxAlphaBlending(pv_set_info))

        case GFX_SET_TYPE_EXT_ALPHA_MUL:
            SET_COMMAND(_GfxExtAlphaMul(pv_set_info))

/*
        case GFX_SET_TYPE_SELF_ALPHA_MUL:
        
        case GFX_SET_TYPE_SELF_ALPHA_DIV:

*/        
        case GFX_SET_TYPE_ALPHA_BITBLT:
            SET_COMMAND(_GfxAlphaBitblt(pv_set_info))

        case GFX_SET_TYPE_COLOR_BITBLT:
            SET_COMMAND(_GfxColorBitblt(pv_set_info))

        case GFX_SET_TYPE_ALPHA_COMPOSITION:
            SET_COMMAND(_GfxAlphaComposition(pv_set_info))

        case GFX_SET_TYPE_YCBCR_TO_RGB:
            SET_COMMAND(_GfxYbr2Rgb(pv_set_info))

        case GFX_SET_TYPE_STRETCH_BITBLT:
            SET_COMMAND(_GfxStretchBitblt(pv_set_info))

        case GFX_SET_TYPE_ALPHAMAP_BITBLT:
            SET_COMMAND(_GfxAlphaMapBitblt(pv_set_info))

        case GFX_SET_TYPE_ROP_BITBLT:
            SET_COMMAND(_GfxRopBitblt(pv_set_info))

        case GFX_SET_TYPE_ROTATE_90:
            SET_COMMAND(_GfxRotate90(pv_set_info))

        case GFX_SET_TYPE_PALETTE_BITBLT:
            SET_COMMAND(_GfxIdx2DirBitblt(pv_set_info))

        case GFX_SET_TYPE_FLUSH_OPQUEUE:
            SET_COMMAND(_GfxFlushQueue(pv_set_info))

        case GFX_SET_TYPE_NOTIFY_FCT:
            SET_COMMAND(_GfxSetCallBackFct((GFX_NFY_FCT_T)pv_set_info))

        case GFX_SET_TYPE_ADV_STRETCH_BITBLT:
        {
              INT32 i4Ret=0;

              i4Ret=GFX_STRETCH_BITBLT(pv_set_info);

              if(i4Ret==GFX_ERROR_ACTION_FAILED)
              {
                 return RMR_OUT_OF_RESOURCES;
              }
              else
              {
                  SET_COMMAND(i4Ret)
              }
        }
        case GFX_SET_TYPE_COMPRESSED_BITBLT:
            SET_COMMAND(_GfxCompressBitblt(pv_set_info))
        case GFX_SET_TYPE_OSD_WAIT_VDP:
            GFX_Wait_Vdp();
            SET_COMMAND(GFX_OK);
		case GFX_SET_TYPE_VGFX_SCALER:
            SET_COMMAND(_GfxVGfxScaler(pv_set_info));
					
	  
        default:
            return RMR_DRV_INV_SET_INFO;
    }
}


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** MW_GFX_Init
 *  
 *  
 */
//-------------------------------------------------------------------------
INT32 MW_GFX_Init(void)
{
    GFX_DRV_COMP_T     *ptComp;
    DRV_COMP_FCT_TBL_T  tFctTbl;
    DRV_COMP_REG_T      tCompReg;

    CHAR   *szDriverName = NULL;
    UINT32 i;
    INT8 i1Count;

    if (_bInitOK)
    {
        return 0;
    }

    // one GFX stack
    i1Count = 1;
    ptComp = _AllocDrvComp(i1Count);
    if (!ptComp)
    {
        return -1;
    }

    if (GFX_FAILED(_GfxOpen()))
    {
        _FreeDrvComp();
        return -1;
    }

    // register driver components
    tFctTbl.pf_rm_connect    = NULL;
    tFctTbl.pf_rm_disconnect = NULL;
    tFctTbl.pf_rm_get        = _GetInterface;
    tFctTbl.pf_rm_set        = _SetInterface;

    // register GPU driver component
    tCompReg.e_type           = DRVT_GPU;
    tCompReg.e_id_type        = ID_TYPE_IND;
    tCompReg.u.t_ind.ui2_id   = GPU_ID;
    tCompReg.u.t_ind.pv_tag   = NULL;
    tCompReg.u.t_ind.ui1_port = ALL_PORTS;

    i = 0;
    ptComp[i].e_type = tCompReg.e_type;
    ptComp[i].ui2_id = tCompReg.u.t_ind.ui2_id;
    ptComp[i].pfGet  = _GfxGetCommand;
    ptComp[i].pfSet  = _GfxSetCommand;
    
    szDriverName = DRVN_GPU;
//    if (x_strcpy((CHAR *)(ptComp[i].szCompName), (const CHAR *)szDriverName))
    if (x_strncpy((CHAR *)(ptComp[i].szCompName), (const CHAR *)szDriverName, sizeof(ptComp[i].szCompName)))

    {
        INT32 i4Ret;
        
        // register GPU component
        i4Ret = x_rm_reg_comp(&tCompReg, 1, 1, ptComp[i].szCompName, 0, &tFctTbl, NULL, 0);
        UNUSED(i4Ret);
    }
    i++;

    _bInitOK = TRUE;
    return 0;
}


//-------------------------------------------------------------------------
/** GFX_Uninit
 *  for future use  
 *  
 */
//-------------------------------------------------------------------------
/*
void GFX_Uninit(void)
{
    if (_bInitOK)
    {
        _FreeDrvComp();
        _GfxClose();
        _bInitOK = FALSE;
    }
}
*/

/*lint -restore */

