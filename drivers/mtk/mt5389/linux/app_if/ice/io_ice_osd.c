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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: io_ice_vga.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_ice_vga.c
 *  ICE VGA driver interface.
 */

#include "mttype.h"
#include "ice.h"
#include "ice_osd.h"
#include "mttype.h"
#include "mtgfx.h"
#include "mtosd.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "osd_drvif.h"
//#include "osd_if.h"



//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#if 1

#define NULL_PT_CHECK(pt)                   \
    {                                       \
        if ((void*)(pt) == NULL)            \
        {                                   \
            return MTR_NOT_OK;      \
        }                                   \
    }

#define XYWH_CHECK(VX,VY,VW,VH)             \
    {                                       \
        if (((INT32)(VX) < 0)     ||        \
            ((INT32)(VY) < 0)     ||        \
            ((INT32)(VW) <= 0)    ||        \
            ((INT32)(VH) <= 0))             \
        {                                   \
            return MTR_NOT_OK;      \
        }                                   \
    }

#define COLOR_CHECK(color)                  \
    {                                       \
        if (((INT32)(color) != (INT32)(MTGFX_CM_AYCBCR8888_DIRECT32))    &&        \
            ((INT32)(color) != (INT32)(MTGFX_CM_RGB565_DIRECT16))        &&        \
            ((INT32)(color) != (INT32)(MTGFX_CM_ARGB1555_DIRECT16))      &&        \
            ((INT32)(color) != (INT32)(MTGFX_CM_ARGB4444_DIRECT16))      &&        \
            ((INT32)(color) != (INT32)(MTGFX_CM_ARGB8888_DIRECT32)))               \
        {                                   \
            return MTR_NOT_OK;      \
        }                                   \
    }
#if 0
static UINT32 _GfxGetIMGRZColor(MTGFX_COLORMODE_T rMwColorMode)
{
    switch (rMwColorMode)
    {        
    case MTGFX_CM_AYCBCR8888_DIRECT32:
        return (UINT32)E_RZ_OSD_DIR_CM_AYCbCr8888;    // workaround

    case MTGFX_CM_RGB565_DIRECT16:
        return (UINT32)E_RZ_OSD_DIR_CM_RGB565;
        
    case MTGFX_CM_ARGB1555_DIRECT16:
        return (UINT32)E_RZ_OSD_DIR_CM_ARGB1555;
        
    case MTGFX_CM_ARGB4444_DIRECT16:
        return (UINT32)E_RZ_OSD_DIR_CM_ARGB4444;
        
    case MTGFX_CM_ARGB8888_DIRECT32:
        return (UINT32)E_RZ_OSD_DIR_CM_ARGB8888;

    default:
        return (UINT32)E_RZ_OSD_DIR_CM_AYCbCr8888;
    }
}
#endif

static MT_RESULT_T _ICEGFX_Scaler(unsigned long arg)
{
 #if 0
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    ICE_GFXSCALER_T rScaler;
    ICE_GFXSCALER_T *prScaler;

    prScaler = (ICE_GFXSCALER_T*)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prScaler, ICE_GFXSCALER_T);
    COPY_FROM_USER_ARG(prScaler, rScaler, ICE_GFXSCALER_T);

    prScaler = &rScaler;
	
    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
	
    NULL_PT_CHECK(prScaler);
    XYWH_CHECK(prScaler->i4SrcX, prScaler->i4SrcY,
               prScaler->u4Width, prScaler->u4Height);
    XYWH_CHECK(prScaler->i4DstX, prScaler->i4DstY,
               prScaler->u4DstWidth, prScaler->u4DstHeight);

    COLOR_CHECK(prScaler->eSrcCm);
    COLOR_CHECK(prScaler->eDstCm);

    rSclParam.u4SrcBase     = (UINT32) prScaler->pvSrc;
    rSclParam.u4IsSrcDirCM  = (UINT32) 1;
    rSclParam.u4SrcCM       = (UINT32) _GfxGetIMGRZColor(prScaler->eSrcCm);
    rSclParam.u4SrcBufLen   = (UINT32) D_RZ_ALIGN16(prScaler->u4SrcPitch);
    rSclParam.u4SrcHOffset  = (UINT32) prScaler->i4SrcX;
    rSclParam.u4SrcVOffset  = (UINT32) prScaler->i4SrcY;
    rSclParam.u4SrcW        = (UINT32) prScaler->u4Width;
    rSclParam.u4SrcH        = (UINT32) prScaler->u4Height;

    rSclParam.u4TgBase      = (UINT32) prScaler->pvDst;
    rSclParam.u4TgHOffset   = (UINT32) prScaler->i4DstX;
    rSclParam.u4TgVOffset   = (UINT32) prScaler->i4DstY;
    rSclParam.u4TgW         = (UINT32) prScaler->u4DstWidth;
    rSclParam.u4TgH         = (UINT32) prScaler->u4DstHeight;
    rSclParam.u4TgCM        = (UINT32) _GfxGetIMGRZColor(prScaler->eDstCm);
    rSclParam.u4TgBufLen    = (UINT32) D_RZ_ALIGN16(prScaler->u4DstPitch);
    if ( (rSclParam.u4TgW != prScaler->u4ClipWidth) 	|| (rSclParam.u4TgH  != prScaler->u4ClipHeight))
    {
        rSclParam.u4PartialUpdate = TRUE;

        //relative coordinate
        rSclParam.u4PartialUpdate_ClipTgx = (UINT32) prScaler->u4ClipX;
        rSclParam.u4PartialUpdate_ClipTgy = (UINT32) prScaler->u4ClipY;
        rSclParam.u4PartialUpdate_ClipTgw = (UINT32) prScaler->u4ClipWidth;
        rSclParam.u4PartialUpdate_ClipTgh = (UINT32) prScaler->u4ClipHeight;
    }
    GFX_SetAlpha(prScaler->u1Alpha);

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
#endif
    return MTR_OK;
}

INT32 _ICEGFX_Scaleralcom(unsigned long arg)
{
  #if 0
    INT32 i4Ret = 0, u4TmpBufAddr = 0;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    ICE_GFXSCALER_T rScaler;
    ICE_GFXSCALER_T *prScaler;

    prScaler = (ICE_GFXSCALER_T*)arg;

    USR_SPACE_ACCESS_VALIDATE_ARG(prScaler, ICE_GFXSCALER_T);
    COPY_FROM_USER_ARG(prScaler, rScaler, ICE_GFXSCALER_T);

    prScaler = &rScaler;

    NULL_PT_CHECK(prScaler);
    XYWH_CHECK(prScaler->i4SrcX, prScaler->i4SrcY,
               prScaler->u4Width, prScaler->u4Height);
    XYWH_CHECK(prScaler->i4DstX, prScaler->i4DstY,
               prScaler->u4DstWidth, prScaler->u4DstHeight);

    COLOR_CHECK(prScaler->eSrcCm);
    COLOR_CHECK(prScaler->eDstCm);

    rSclParam.u4SrcBase     = (UINT32) prScaler->pvSrc;
    rSclParam.u4IsSrcDirCM  = (UINT32) 1;
    rSclParam.u4SrcCM       = (UINT32) _GfxGetIMGRZColor(prScaler->eSrcCm);
    rSclParam.u4SrcBufLen   = (UINT32) D_RZ_ALIGN16(prScaler->u4SrcPitch);
    rSclParam.u4SrcHOffset  = (UINT32) prScaler->i4SrcX;
    rSclParam.u4SrcVOffset  = (UINT32) prScaler->i4SrcY;
    rSclParam.u4SrcW        = (UINT32) prScaler->u4Width;
    rSclParam.u4SrcH        = (UINT32) prScaler->u4Height;
    if (u4TmpBufAddr == 0)
    {
        u4TmpBufAddr =  (UINT32)BSP_AllocAlignedDmaMemory(prScaler->u4DstWidth * prScaler->u4DstHeight * 4, 16);
        ASSERT(u4TmpBufAddr != 0);
        printk("TmpBuf = 0x%x\n", u4TmpBufAddr);
    }

    rSclParam.u4TgBase      = u4TmpBufAddr;//(UINT32) prScaler->pvDst;
    rSclParam.u4TgHOffset   = (UINT32) prScaler->i4DstX;
    rSclParam.u4TgVOffset   = (UINT32) prScaler->i4DstY;
    rSclParam.u4TgW         = (UINT32) prScaler->u4DstWidth;
    rSclParam.u4TgH         = (UINT32) prScaler->u4DstHeight;
    rSclParam.u4TgCM        = (UINT32) _GfxGetIMGRZColor(prScaler->eDstCm);
    rSclParam.u4TgBufLen    = (UINT32) D_RZ_ALIGN16(prScaler->u4DstPitch);
    if ( (rSclParam.u4TgW != prScaler->u4ClipWidth) 	|| (rSclParam.u4TgH  != prScaler->u4ClipHeight))
    {
        rSclParam.u4PartialUpdate = TRUE;

        //relative coordinate
        rSclParam.u4PartialUpdate_ClipTgx = (UINT32) prScaler->u4ClipX;
        rSclParam.u4PartialUpdate_ClipTgy = (UINT32) prScaler->u4ClipY;
        rSclParam.u4PartialUpdate_ClipTgw = (UINT32) prScaler->u4ClipWidth;
        rSclParam.u4PartialUpdate_ClipTgh = (UINT32) prScaler->u4ClipHeight;
    }
    GFX_SetAlpha(prScaler->u1Alpha);

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    GFX_Lock();

    GFX_SetSrc(u4TmpBufAddr, (UINT32)prScaler->eDstCm, rSclParam.u4TgBufLen);
    GFX_SetDst(prScaler->pvDst, (UINT32)prScaler->eDstCm, prScaler->u4DstPitch);

    GFX_ComposeLoop(prScaler->i4DstX, prScaler->i4DstY, prScaler->i4DstX, prScaler->i4DstY,
                    prScaler->u4ClipWidth, prScaler->u4ClipHeight, prScaler->u1Alpha, prScaler->u4Porterdu, 0);

    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();


    return i4Ret;
	#endif
	return MTR_OK;

}
static MT_RESULT_T _ICEREGION_Scaler(unsigned long arg)
{
    MT_RESULT_T eRet=MTR_OK; 
    ICE_IOCTL_12ARG_T rArg;
//    UINT32  u4_first_rgn  = 0x0;
    UINT32  u4_plane;
    OSD_RGN_SC_CLIP_T t_reg_sc_clip ;
    
    printf("_ICEREGION_Scaler Enter\n");

    USR_SPACE_ACCESS_VALIDATE_12ARG(arg);
    COPY_FROM_USER_12ARG(arg,rArg);


    u4_plane = (UINT32)rArg.ai4Arg[0];
    t_reg_sc_clip.ui4_enable = (UINT32)rArg.ai4Arg[1];
    t_reg_sc_clip.ui4_full_fg = (UINT32)rArg.ai4Arg[2];
    t_reg_sc_clip.i4_clip_x = (INT32)rArg.ai4Arg[3];
    t_reg_sc_clip.i4_clip_y = (INT32)rArg.ai4Arg[4];
    t_reg_sc_clip.i4_clip_w = (INT32)rArg.ai4Arg[5];
    t_reg_sc_clip.i4_clip_h = (INT32)rArg.ai4Arg[6];
    t_reg_sc_clip.i4_dst_x = (INT32)rArg.ai4Arg[7];
    t_reg_sc_clip.i4_dst_y = (INT32)rArg.ai4Arg[8];
    t_reg_sc_clip.i4_dst_w = (INT32)rArg.ai4Arg[9];
    t_reg_sc_clip.i4_dst_h = (INT32)rArg.ai4Arg[10];


    OSD_RGN_SC_Clip(u4_plane, (void*)&t_reg_sc_clip);

    printf("[_ICEREGION_Scaler]=====u4_plane %d \n-----input window: ( %d, %d, %d, %d) --output window:(%d, %d, %d, %d)\n",
              u4_plane,
              t_reg_sc_clip.i4_clip_x,
              t_reg_sc_clip.i4_clip_y,
              t_reg_sc_clip.i4_clip_w,
              t_reg_sc_clip.i4_clip_h,
              t_reg_sc_clip.i4_dst_x,
              t_reg_sc_clip.i4_dst_y,
              t_reg_sc_clip.i4_dst_w,
              t_reg_sc_clip.i4_dst_h);

    return eRet;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int ice_ioctl_osd(struct inode *inode, struct file *file, unsigned int cmd,
                  unsigned long arg)
{
    int i4Ret = MTR_NOT_OK;
    switch (cmd)
    {
        case ICE_IO_OSD_GFXSCALER:
            i4Ret = _ICEGFX_Scaler(arg);
            break;
        case ICE_IO_OSD_GFXSCALER_ALCOM:
            i4Ret = _ICEGFX_Scaleralcom(arg);
            break;
        case ICE_IO_OSD_REGIONSCALER:
            i4Ret = _ICEREGION_Scaler(arg);
            break;

        default:
            break;
    }
    return i4Ret;
}
#endif
