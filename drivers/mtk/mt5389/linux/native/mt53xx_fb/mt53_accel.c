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
/*
 * Hardware specific code is based on MT53xx driver
 * Framebuffer framework code is based on documents on net
 *
 */

//---------------------------------------------------------------------------
// Header files
//---------------------------------------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/mm.h>
//#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
//#include <linux/pci.h>
#include <linux/init.h>
//#include <linux/console.h>
#include <linux/pagemap.h>
#include <linux/dma-mapping.h>

#include <asm/io.h>
// #include <asm/irq.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma-mapping.h>

#ifdef CONFIG_MTRR
#include <asm/mtrr.h>
#endif

#include "osd_if.h"
#include "gfx_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "vgfx_drvif.h"

#include "u_handle.h"
#include "mt53_fb.h"
#include "mt53_accel.h"
#include "vgfx_drvif.h"

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

struct mt53fb_blit_unit {
    UINT32 u4Addr;
    UINT32 u4Width;
    UINT32 u4NumLines;
    UINT32 u4Pitch;
};

struct mt53fb_buffer{
    UINT32 u4NumPages;
    UINT32 u4VirtStart;
    UINT32 u4NumUnits;
    UINT32* pu4DmaAddr;
    struct page** pprPage;
    struct mt53fb_blit_unit* prUnit;
};

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
/*
static struct mt53fb_buffer _arBuf[8];
static UINT32 _u4BufBitmap = 0x0;
static UINT32 _u4BlitCount = 0;
static HAL_TIME_T _rTBlit, _rTGetPage, _rTMapDma, _rTReleasePage;*/

static INT32 _i4DstBpp, _i4SrcBpp;

//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------
extern int _mt53fb_get_tmpbufaddr(void);
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

static INT32 _get_colormode(UINT32 u4Depth)
{
    switch (u4Depth)
    {
        case 8:
            return CM_RGB_CLUT8;
        case 16:
            return CM_ARGB4444_DIRECT16;
        case 32:
            return CM_ARGB8888_DIRECT32;
        default:
            return -1;
    }
}

static INT32 _get_bpp_shift(UINT32 u4Bpp)
{
    if (u4Bpp == 32)
    {
        return 2;
    }
    else if (u4Bpp == 16)
    {
        return 1;
    }
    else if (u4Bpp == 8)
    {
        return 0;
    }
    return -1;
}

static INT32 _fillrect_hw(void* pvDst, 
    UINT32 u4X, UINT32 u4Y, UINT32 u4Pitch,
    UINT32 u4Width, UINT32 u4Lines, 
    UINT32 u4Color)
{
    //INT32 i4Ret;
    INT32 i4ColorMode;
    struct fb_info* prFb;

    prFb = mt53fb_get_fb_info();

    if(prFb == NULL)
    {
        printk(KERN_ERR "_fillrect_hw() error: prFb == NULL!\n");    
        return -EFAULT;
    }     
    if(!ADDR_VALID( ((UINT32)pvDst), prFb))
    {
        printk(KERN_WARNING "_fillrect_hw() error: bad dst addr 0x%8x!\n", pvDst);
        return -E_GFXLIB_DST_ADDR_ERR;
    }    
    if(!PITCH_VALID(u4Pitch))
    {
        printk(KERN_WARNING "_fillrect_hw() error: bad dst pitch %d!\n", u4Pitch);
        return -E_GFXLIB_DST_PITCH_ERR;
    }

    if((i4ColorMode = _get_colormode(_i4DstBpp)) < 0)
    {
        printk(KERN_WARNING "_fillrect_hw() error: bad color mode %d!\n", i4ColorMode);   
        return -E_GFXLIB_SET_COLORMODE_ERR;        
    }   

    GFX_SetDst(pvDst, (UINT32)i4ColorMode, u4Pitch);
    GFX_SetColor(u4Color);
    GFX_Fill(u4X, u4Y, u4Width, u4Lines);

    return 0;
}


static INT32 _imageblit_hw(void* pvDst, void* pvSrc, 
    UINT32 u4DstX, UINT32 u4DstY, UINT32 u4DstPitch,     
    UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcPitch, 
    UINT32 u4Width,
    UINT32 u4Lines)
{
    INT32 i4Ret = 0;
    INT32 i4ColorMode = CM_ARGB8888_DIRECT32;
    struct fb_info* prFb;

    prFb = mt53fb_get_fb_info();

    if(prFb == NULL)
    {
        printk(KERN_ERR "_imageblit_hw() error: prFb == NULL!\n");    
        return -EFAULT;
    }     
    if(!ADDR_VALID( ((UINT32)pvDst), prFb))
    {
        printk(KERN_WARNING "_imageblit_hw() error: bad dst addr 0x%8x!\n", pvDst);
        return -E_GFXLIB_DST_ADDR_ERR;
    }
    if(!ADDR_VALID( ((UINT32)pvSrc), prFb))
    {
        printk(KERN_WARNING "_imageblit_hw() error: bad src addr 0x%8x!\n", pvSrc);    
        return -E_GFXLIB_SRC_ADDR_ERR;
    }
    if(!PITCH_VALID(u4DstPitch))
    {
        printk(KERN_WARNING "_imageblit_hw() error: bad dst pitch %d!\n", u4DstPitch);
        return -E_GFXLIB_DST_PITCH_ERR;
    }
    if(!PITCH_VALID(u4SrcPitch))
    {
        printk(KERN_WARNING "_imageblit_hw() error: bad src pitch %d!\n", u4SrcPitch);    
        return -E_GFXLIB_SRC_PITCH_ERR;
    }
    
    if(_i4DstBpp != _i4SrcBpp || ((i4ColorMode = _get_colormode(_i4DstBpp)) < 0))
    {
        printk(KERN_WARNING "_imageblit_hw() error: bad color mode %d!\n", i4ColorMode);   
        return -E_GFXLIB_SET_COLORMODE_ERR;        
    }
            	
    GFX_SetSrc(pvSrc, (UINT32)i4ColorMode, u4SrcPitch);
    GFX_SetDst(pvDst, (UINT32)i4ColorMode, u4DstPitch);
    //need to be confirmed
    GFX_SetBltOpt(0, 0, 0);
    GFX_BitBlt(u4SrcX, u4SrcY, u4DstX, u4DstY, u4Width, u4Lines);
    	
    return i4Ret;
}

static INT32 _alphacomp_hw(void* pvDst, void* pvSrc, 
    UINT32 u4DestX, UINT32 u4DestY, UINT32 u4DestPitch,
    UINT32 u4SrcX, UINT32 u4SrcY, UINT32 u4SrcPitch,
    UINT32 u4Width, UINT32 u4Height, 
    UINT32 u4GlobalAlpha, UINT32 u4PdRule, 
    UINT32 u4RectColor, BOOL bSrcRectColorFlag)
{
    //INT32 i4Ret;
    INT32 i4ColorMode;
    struct fb_info* prFb;

    prFb = mt53fb_get_fb_info();

    if(prFb == NULL)
    {
        printk(KERN_ERR "_alphacomp_hw() error: prFb == NULL!\n");    
        return -EFAULT;
    }     
    if(!ADDR_VALID(((UINT32)pvDst), prFb))
    {
        printk(KERN_WARNING "_alphacomp_hw: bad dst addr 0x%8x!\n", pvDst);
        return -E_GFXLIB_DST_ADDR_ERR;
    }
    if(!bSrcRectColorFlag && !ADDR_VALID( ((UINT32)pvSrc), prFb))
    {
        printk(KERN_WARNING "_alphacomp_hw: bad src addr 0x%8x!\n", pvSrc);    
        return -E_GFXLIB_SRC_ADDR_ERR;
    }
    if(!PITCH_VALID(u4DestPitch))
    {
        printk(KERN_WARNING "_alphacomp_hw: bad dst pitch %d!\n", u4DestPitch);
        return -E_GFXLIB_DST_PITCH_ERR;
    }
    if(!bSrcRectColorFlag && !PITCH_VALID(u4SrcPitch))
    {
        printk(KERN_WARNING "_alphacomp_hw: bad src pitch %d!\n", u4SrcPitch);    
        return -E_GFXLIB_SRC_PITCH_ERR;
    }

    if( (i4ColorMode = _get_colormode(_i4DstBpp)) < 0)
    {
        printk(KERN_WARNING "_alphacomp_hw() error: bad colormode %d!\n", i4ColorMode);   
        return -E_GFXLIB_SET_COLORMODE_ERR;        
    }
    
    /*
      * Alpha composition real op
    **/       
    GFX_SetSrc(pvSrc, (UINT32)i4ColorMode, u4SrcPitch);    
    GFX_SetDst(pvDst, (UINT32)i4ColorMode, u4DestPitch);        
    GFX_SetColor(u4RectColor);    
    GFX_ComposeLoop(u4SrcX, u4SrcY, u4DestX, u4DestY, u4Width, u4Height, 
        u4GlobalAlpha, u4PdRule, (UINT32)bSrcRectColorFlag);
    
    return 0;
}


/*
static INT32 _drawVline_hw(void* pu1DstBaseAddr,UINT32 x, UINT32 y, UINT16 u2DstPitch, UINT16 u4Color, UINT16 u2Length,UINT32 u4Bpp)
{
    INT32 i4Ret = 0;
    INT32 i4ColorMode;
    struct fb_info* prFb;

    prFb = mt53fb_get_fb_info();

    if(prFb == NULL)
    {
        printk(KERN_ERR "_drawVline_hw() error: prFb == NULL!\n");    
        return -EFAULT;
    }     
    if(!ADDR_VALID( ((UINT32)pu1DstBaseAddr), prFb))
    {
        printk(KERN_WARNING "_drawVline_hw() error: bad dst addr 0x%8x!\n", pu1DstBaseAddr);
        return -E_GFXLIB_DST_ADDR_ERR;
    }

    if(!PITCH_VALID(u2DstPitch))
    {
        printk(KERN_WARNING "_drawVline_hw() error: bad dst pitch %d!\n", u2DstPitch);
        return -E_GFXLIB_DST_PITCH_ERR;
    }

    if( (i4ColorMode = _get_colormode(_i4DstBpp)) < 0)
    {
        printk(KERN_WARNING "_alphacomp_hw() error: bad colormode %d!\n", i4ColorMode);   
        return -E_GFXLIB_SET_COLORMODE_ERR;        
    }
    
    //printk("draw V line\n");
    GFX_SetDst((UINT8 *)pu1DstBaseAddr,  (UINT32)i4ColorMode, u2DstPitch);
    GFX_SetColor(u4Color);
    GFX_VLine(x, y, u2Length);

    return i4Ret;
}

static INT32 _drawHline_hw(void* pu1DstBaseAddr,UINT32 x, UINT32 y, UINT16 u2DstPitch, UINT16 u4Color, UINT16 u2Length,UINT32 u4Bpp)
{
    INT32 i4Ret = 0;
    INT32 i4ColorMode;
    struct fb_info* prFb;

    prFb = mt53fb_get_fb_info();

    if(prFb == NULL)
    {
        printk(KERN_ERR "_drawVline_hw() error: prFb == NULL!\n");    
        return -EFAULT;
    }     
    if(!ADDR_VALID( ((UINT32)pu1DstBaseAddr), prFb))
    {
        printk(KERN_WARNING "_drawVline_hw() error: bad dst addr 0x%8x!\n", pu1DstBaseAddr);
        return -E_GFXLIB_DST_ADDR_ERR;
    }

    if(!PITCH_VALID(u2DstPitch))
    {
        printk(KERN_WARNING "_drawVline_hw() error: bad dst pitch %d!\n", u2DstPitch);
        return -E_GFXLIB_DST_PITCH_ERR;
    }

    if( (i4ColorMode = _get_colormode(_i4DstBpp)) < 0)
    {
        printk(KERN_WARNING "_alphacomp_hw() error: bad colormode %d!\n", i4ColorMode);   
        return -E_GFXLIB_SET_COLORMODE_ERR;        
    }
    
    //printk("draw H line\n");
    GFX_SetDst(pu1DstBaseAddr,  (UINT32)i4ColorMode, u2DstPitch);
    GFX_SetColor(u4Color);
    GFX_HLine(x, y, u2Length);

    return i4Ret;

}
*/
//---------------------------------------------------------------------------
// Public interfaces
//---------------------------------------------------------------------------

INT32 mt53fb_drawline(struct fb_info *info, const struct mt53fb_DrawLine *prDrawLine)
{
    INT32 i4Ret = 0;
    //UINT32 u4Dst = info->fix.smem_start;
    UINT8 u1Flag = 0; // 0 for vertical line, 1 for horizontal line ,2 for abstrary line
    UINT16 index,x1=0,x2=0,y1=0,y2=0;
    UINT16 u2Length = 0;
    UINT32 x = 0,y = 0;
    //printk("mt53fb_drawline\n");
    if(prDrawLine->u2X1 == prDrawLine->u2X2) {
        u1Flag = 0; //vertical line
        x = prDrawLine->u2X1;
        if(prDrawLine->u2Y2 > prDrawLine->u2Y1) {
            u2Length = prDrawLine->u2Y2 - prDrawLine->u2Y1;
            y =  prDrawLine->u2Y1;
        }
        else {
            u2Length = -(prDrawLine->u2Y2 - prDrawLine->u2Y1);
            y = prDrawLine->u2Y2;
        }
    }
    else if(prDrawLine->u2Y1 == prDrawLine->u2Y2) {
        u1Flag = 1; //horizontal line
        y = prDrawLine->u2Y1;
        if(prDrawLine->u2X2 > prDrawLine->u2X1) {
            u2Length = (prDrawLine->u2X2 - prDrawLine->u2X1);
            x = prDrawLine->u2X1;
        }
        else {
            u2Length = -(prDrawLine->u2X2 - prDrawLine->u2X1);
            x = prDrawLine->u2X2;
        }
    }
    else {
        u1Flag = 2; //abstrary line
        if(prDrawLine->u2X1 > prDrawLine->u2X2) {
            x2 = prDrawLine->u2X1;
            x1 = prDrawLine->u2X2;
            y1 = prDrawLine->u2Y2;
            y2 = prDrawLine->u2Y1;
        }
        else {
            x2 = prDrawLine->u2X2;
            x1 = prDrawLine->u2X1;
            y1 = prDrawLine->u2Y1;
            y2 = prDrawLine->u2Y2;
        }
    }

    _i4DstBpp = prDrawLine->u4Bpp;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
    
    GFX_Lock();
    //printk("x= %d , y= %d , length = %d, flag = %d\n",x,y,u2Length,u1Flag);
    if(u1Flag == 0 ) {
        i4Ret = _fillrect_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch,1,u2Length,prDrawLine->u4color);
        //i4Ret = _drawVline_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch, prDrawLine->u4color,u2Length,prDrawLine->u4Bpp);
    }
    else if(u1Flag == 1 ) {
        i4Ret = _fillrect_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch,u2Length,1,prDrawLine->u4color);
        //i4Ret = _drawHline_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch, prDrawLine->u4color,u2Length,prDrawLine->u4Bpp);
    }
    else {
        for(index=x1; index<=x2 ; index++) {
            x = index;
            y = ((y2-y1)*(x-x1)/(x2-x1))+y1;
            //i4Ret = _drawHline_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch, prDrawLine->u4color,1,prDrawLine->u4Bpp);
            i4Ret = _fillrect_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch,1,1,prDrawLine->u4color);
        }
    }
    HalFlushInvalidateDCache();
    GFX_Flush();
    GFX_Wait();            
    GFX_Unlock();
            
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;
}

INT32 mt53fb_drawlinerop(struct fb_info *info, const struct mt53fb_DrawLineRop *prDrawLineRop)
{
    INT32 i4Ret = 0;
    UINT8 u1Flag = 0; // 0 for vertical line, 1 for horizontal line ,2 for abstrary line
    UINT16 index, x1=0 , x2 = 0 , y1 = 0 , y2 = 0 ;
    UINT16 u2Length = 0;
    UINT32 x = 0, y = 0;
    UINT32 u4Dst = (UINT32)prDrawLineRop->pu1DstBaseAddr;
    UINT32 u4Src = (UINT32)info->fix.smem_start;
    
    //printk("mt53fb_drawlinerop\n");
    
    if(prDrawLineRop->u2X1 == prDrawLineRop->u2X2) {
        u1Flag = 0; //vertical line
        x = prDrawLineRop->u2X1;
        if(prDrawLineRop->u2Y2 > prDrawLineRop->u2Y1) {
            u2Length = prDrawLineRop->u2Y2 - prDrawLineRop->u2Y1;
            y =  prDrawLineRop->u2Y1;
        }
        else {
            u2Length = -(prDrawLineRop->u2Y2 - prDrawLineRop->u2Y1);
            y = prDrawLineRop->u2Y2;
        }
    }
    else if(prDrawLineRop->u2Y1 == prDrawLineRop->u2Y2) {
        u1Flag = 1; //horizontal line
        y = prDrawLineRop->u2Y1;
        if(prDrawLineRop->u2X2 > prDrawLineRop->u2X1) {
            u2Length = (prDrawLineRop->u2X2 - prDrawLineRop->u2X1);
            x = prDrawLineRop->u2X1;
        }
        else {
            u2Length = -(prDrawLineRop->u2X2 - prDrawLineRop->u2X1);
            x = prDrawLineRop->u2X2;
        }
    }
    else {
        u1Flag = 2; //abstrary line
        if(prDrawLineRop->u2X1 > prDrawLineRop->u2X2) {
            x2 = prDrawLineRop->u2X1;
            x1 = prDrawLineRop->u2X2;
            y1 = prDrawLineRop->u2Y2;
            y2 = prDrawLineRop->u2Y1;
        }
        else {
            x2 = prDrawLineRop->u2X2;
            x1 = prDrawLineRop->u2X1;
            y1 = prDrawLineRop->u2Y1;
            y2 = prDrawLineRop->u2Y2;
        }
    }

    //_i4DstBpp = prDrawLineRop->u4Bpp;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
    
    //printk("x= %d , y= %d , length = %d, flag = %d\n",x,y,u2Length,u1Flag);

    switch (prDrawLineRop->u4Rop)
        {
            case ROP_CLEAR:
            case ROP_DST_IN:
            case ROP_DST_OUT:
            case ROP_DST_OVER:
            case ROP_SRC:
            case ROP_SRC_IN:
            case ROP_SRC_OUT:
            case ROP_SRC_OVER:
            case ROP_DST:
            case ROP_SRC_ATOP:
            case ROP_DST_ATOP:
            case ROP_XOR_GFX:
                _i4DstBpp = prDrawLineRop->u4Bpp;            
                GFX_Lock();

                if(u1Flag ==0) {
                i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
                    x, y, prDrawLineRop->u2DstPitch,
                    0, 0, 0,
                    1, u2Length, 
                    prDrawLineRop->u2Alpha, prDrawLineRop->u4Rop, 
                    prDrawLineRop->u4color, 1);
                }
                else if(u1Flag ==1) {
                 i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
                    x, y, prDrawLineRop->u2DstPitch,
                    0, 0, 0,
                    u2Length, 1, 
                    prDrawLineRop->u2Alpha, prDrawLineRop->u4Rop, 
                    prDrawLineRop->u4color, 1);
                }
                else {
                    for(index=x1; index<=x2 ; index++) {
                        x = index;
                        y = ((y2-y1)*(x-x1)/(x2-x1))+y1;
                        //i4Ret = _drawHline_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch, prDrawLine->u4color,1,prDrawLine->u4Bpp);
                        //i4Ret = _fillrect_hw(prDrawLine->pu1DstBaseAddr,x,y,prDrawLine->u2DstPitch,1,1,prDrawLine->u4color);
                        i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
                        x, y, prDrawLineRop->u2DstPitch,
                        0, 0, 0,
                        1, 1, 
                        prDrawLineRop->u2Alpha, prDrawLineRop->u4Rop, 
                        prDrawLineRop->u4color, 1);
                    }

                }
                HalFlushInvalidateDCache();
                GFX_Flush();
                GFX_Wait();            
                GFX_Unlock();
                break;
            default:
                printk(KERN_WARNING "mt53fb_fillrectrop() error: bad rop %d!\n", prDrawLineRop->u4Rop);          
                i4Ret = -E_GFXLIB_ROP_ERR;
                break;
        }
            
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;


}

INT32 mt53fb_drawrect(struct fb_info *info, const struct mt53fb_DrawRect *rDrawRect)
{
    INT32 i4Ret = 0;
    //UINT32 u4Dst = info->fix.smem_start;
    UINT32 u4Border = rDrawRect->u4BoarderWidth;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    _i4DstBpp = rDrawRect->u4Bpp;
    
    GFX_Lock();    
	HalFlushInvalidateDCache();
    i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, rDrawRect->u2DstX, rDrawRect->u2DstY, rDrawRect->u2DstPitch, rDrawRect->u2Width, u4Border, rDrawRect->u4Color);
    i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, rDrawRect->u2DstX, rDrawRect->u2DstY, rDrawRect->u2DstPitch, u4Border, rDrawRect->u2Height, rDrawRect->u4Color);
    i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, (rDrawRect->u2DstX + rDrawRect->u2Width - u4Border), rDrawRect->u2DstY, rDrawRect->u2DstPitch, u4Border, rDrawRect->u2Height, rDrawRect->u4Color);
    i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, rDrawRect->u2DstX, (rDrawRect->u2DstY + rDrawRect->u2Height - u4Border), rDrawRect->u2DstPitch, rDrawRect->u2Width, u4Border, rDrawRect->u4Color);
    //HalFlushInvalidateDCache();
    GFX_Flush();
    GFX_Wait();            
    GFX_Unlock();
            
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;

}

INT32 mt53fb_drawrectrop(struct fb_info *info, const struct mt53fb_DrawRectRop *prDrawRectRop)
{
    INT32 i4Ret = 0;
    UINT32 u4Dst = (UINT32)prDrawRectRop->pu1DstBaseAddr;
    UINT32 u4Src = info->fix.smem_start;
    UINT32 u4Border = prDrawRectRop->u4BoarderWidth;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    //_i4DstBpp = rDrawRect->u4Bpp;
    //printk("mt53fb_drawrectrop\n");
    switch (prDrawRectRop->u4Rop)
    {
        case ROP_CLEAR:
        case ROP_DST_IN:
        case ROP_DST_OUT:
        case ROP_DST_OVER:
        case ROP_SRC:
        case ROP_SRC_IN:
        case ROP_SRC_OUT:
        case ROP_SRC_OVER:
        case ROP_DST:
        case ROP_SRC_ATOP:
        case ROP_DST_ATOP:
        case ROP_XOR_GFX:       
            _i4DstBpp = prDrawRectRop->u4Bpp;        	
            GFX_Lock();
			GFX_Flush_Invalid_DCache();
            i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
            	prDrawRectRop->u2DstX, prDrawRectRop->u2DstY, prDrawRectRop->u2DstPitch,
            	0, 0, 0,
            	prDrawRectRop->u2Width, u4Border, 
            	prDrawRectRop->u4Alpha, prDrawRectRop->u4Rop, 
            	prDrawRectRop->u4Color, 1);
            i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
            	prDrawRectRop->u2DstX, (prDrawRectRop->u2DstY + u4Border), prDrawRectRop->u2DstPitch,
            	0, 0, 0,
            	u4Border, (prDrawRectRop->u2Height -u4Border), 
            	prDrawRectRop->u4Alpha, prDrawRectRop->u4Rop, 
            	prDrawRectRop->u4Color, 1);
            i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
            	(prDrawRectRop->u2DstX + prDrawRectRop->u2Width - u4Border), (prDrawRectRop->u2DstY + u4Border), prDrawRectRop->u2DstPitch,
            	0, 0, 0,
            	u4Border, (prDrawRectRop->u2Height -u4Border), 
            	prDrawRectRop->u4Alpha, prDrawRectRop->u4Rop, 
            	prDrawRectRop->u4Color, 1);
            i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
            	(prDrawRectRop->u2DstX +u4Border) , (prDrawRectRop->u2DstY + prDrawRectRop->u2Height - u4Border), prDrawRectRop->u2DstPitch,
            	0, 0, 0,
            	(prDrawRectRop->u2Width-(u4Border<<1)), u4Border, 
            	prDrawRectRop->u4Alpha, prDrawRectRop->u4Rop, 
            	prDrawRectRop->u4Color, 1);            	
            //HalFlushInvalidateDCache();
            GFX_Flush();
            GFX_Wait();            
            GFX_Unlock();
            break;
        default:
            printk(KERN_WARNING "mt53fb_drawrectrop() error: bad rop %d!\n", prDrawRectRop->u4Rop);         	
            i4Ret = -E_GFXLIB_ROP_ERR;
            break;
    }
   
    //i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, rDrawRect->u2DstX, rDrawRect->u2DstY, rDrawRect->u2DstPitch, rDrawRect->u2Width, u4Border, rDrawRect->u4Color);
    //i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, rDrawRect->u2DstX, rDrawRect->u2DstY, rDrawRect->u2DstPitch, u4Border, rDrawRect->u2Height, rDrawRect->u4Color);
    //i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, (rDrawRect->u2DstX + rDrawRect->u2Width - u4Border), rDrawRect->u2DstY, rDrawRect->u2DstPitch, u4Border, rDrawRect->u2Height, rDrawRect->u4Color);
    //i4Ret = _fillrect_hw(rDrawRect->pu1DstBaseAddr, rDrawRect->u2DstX, (rDrawRect->u2DstY + rDrawRect->u2Height - u4Border), rDrawRect->u2DstPitch, rDrawRect->u2Width, u4Border, rDrawRect->u4Color);
            
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;


}

INT32 mt53fb_drawtext(struct fb_info *info, const struct mt53fb_DrawText *prDrawText)
{
    INT32 i4Ret = 0;
    UINT32 u4Dst = (UINT32)prDrawText->pu1DstBaseAddr;
    UINT32 u4Src = info->fix.smem_start; 
    UINT32 i,x,y;
    UINT32 u4Size;
    
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
    
    //printk("mt53fb_drawtext : 0x%08x\n",prDrawText->pu1DstBaseAddr);
    
    u4Size = ((prDrawText->u2Width) * (prDrawText->u2Height));

    _i4DstBpp = prDrawText->u4Bpp;
    
    /*for(i = 0 ; i< 10 ; i++) {
        printk("%u ",(unsigned char)prDrawText->pTextArray[i]);
    }*/
    GFX_Lock();
    GFX_Flush_Invalid_DCache();
    
    for(i = 0 ; i< u4Size ; i++) {
        if((unsigned char)(prDrawText->pTextArray[i]) != 0xFF) {
        
            x = prDrawText->u2X + (i%prDrawText->u2Width);
            y = prDrawText->u2Y + (i/prDrawText->u2Width);

            i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
            	x, y, prDrawText->u2DstPitch,
            	0, 0, 0,
            	1, 1, 
            	prDrawText->u4Alpha, ROP_SRC_OVER, 
            	prDrawText->u4Color, 1);
        }
    }
    //HalFlushInvalidateDCache();
    GFX_Flush();
    GFX_Wait();            
    GFX_Unlock();
            
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;

}




INT32 mt53fb_fillrect(struct fb_info *info, const struct mt53fb_fillrect *rect)
{
    INT32 i4Ret = 0;
    //UINT32 u4Dst = info->fix.smem_start;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
    
    //printk("mt53fb_fillrect : 0x%08x\n",rect->pu1DstBaseAddr);
    
    _i4DstBpp = rect->i4Bpp;
    
    GFX_Lock();    
	GFX_Flush_Invalid_DCache();
    i4Ret = _fillrect_hw(rect->pu1DstBaseAddr, rect->u2DstX, rect->u2DstY, rect->u2DstPitch, rect->u2Width, rect->u2Height, rect->i4Color);
    //HalFlushInvalidateDCache();
    GFX_Flush();
    GFX_Wait();            
    GFX_Unlock();
            
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;
}

INT32 mt53fb_fillrectrop(struct fb_info *info, const struct mt53fb_fillrectrop *rect)
{
    INT32 i4Ret = 0;
    UINT32 u4BPPShift;
    UINT32 u4Dst = info->fix.smem_start;
    UINT32 u4Src = info->fix.smem_start;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    u4BPPShift = _get_bpp_shift(info->var.bits_per_pixel);
    
    //printk("mt53fb_fillrectrop address : 0x%08x 0x%08x\n",u4Dst,rect->pu1DstBaseAddr);

    //u4Dst = (UINT32)rect->pu1DstBaseAddr;
    switch (rect->i4Rop)
    {
        case ROP_CLEAR:
        case ROP_DST_IN:
        case ROP_DST_OUT:
        case ROP_DST_OVER:
        case ROP_SRC:
        case ROP_SRC_IN:
        case ROP_SRC_OUT:
        case ROP_SRC_OVER:
        case ROP_DST:
        case ROP_SRC_ATOP:
        case ROP_DST_ATOP:
        case ROP_XOR_GFX:
            _i4DstBpp = rect->i4Bpp;        	
            GFX_Lock();
			GFX_Flush_Invalid_DCache();
            i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
            	rect->u2DstX, rect->u2DstY, rect->u2DstPitch,
            	0, 0, 0,
            	rect->u2Width, rect->u2Height, 
            	rect->i4Alpha, rect->i4Rop, 
            	rect->i4Color, 1);
            //HalFlushInvalidateDCache();
            GFX_Flush();
            GFX_Wait();            
            GFX_Unlock();
            break;
        default:
            printk(KERN_WARNING "mt53fb_fillrectrop() error: bad rop %d!\n", rect->i4Rop);         	
            i4Ret = -E_GFXLIB_ROP_ERR;
            break;
    }
    
    x_sema_unlock(hMt53fbMutex);
    
    return i4Ret;
}

INT32 mt53fb_copyarearop(struct fb_info *info, const struct mt53fb_copyarearop *area)
{
    UINT32 u4Src;
    UINT32 u4Dst;
    UINT32 u4ResX;
    UINT32 u4ResY;
    INT32 i4Ret = 0;
    UINT32 u4BPPShift;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
    
    u4BPPShift = _get_bpp_shift(info->var.bits_per_pixel);
    
    u4ResX = info->var.xres_virtual;
    u4ResY = info->var.yres_virtual;
    if ((area->sx < 0) || ((area->sx + area->width) > u4ResX) ||
        (area->sy < 0) || ((area->sy + area->height) > u4ResY) ||
        (area->dx < 0) || ((area->dx + area->width) > u4ResX) ||
        (area->dy < 0) || ((area->dy + area->height) > u4ResY))
    {
        i4Ret = -EINVAL;
    }
    else
    {
        //u4Src = info->fix.smem_start + (area->sx + info->var.xres_virtual * area->sy) * info->var.bits_per_pixel / 8;
        //u4Dst = info->fix.smem_start + (area->dx + info->var.xres_virtual * area->dy) * info->var.bits_per_pixel / 8;
        u4Src = info->fix.smem_start;
        u4Dst = info->fix.smem_start;
        switch (area->rop)
        {
            case ROP_CLEAR:
            case ROP_DST_IN:
            case ROP_DST_OUT:
            case ROP_DST_OVER:
            case ROP_SRC:
            case ROP_SRC_IN:
            case ROP_SRC_OUT:
            case ROP_SRC_OVER:
            case ROP_DST:
            case ROP_SRC_ATOP:
            case ROP_DST_ATOP:
            case ROP_XOR_GFX:
                GFX_Lock();                
				GFX_Flush_Invalid_DCache();
                i4Ret = _alphacomp_hw((void *)u4Dst, (void *)u4Src, 
                	area->dx, area->dy, info->var.xres_virtual << u4BPPShift,
                	area->sx, area->sy, info->var.xres_virtual << u4BPPShift,
                	area->width, area->height,
                     area->alpha, area->rop, 0, 0);
                //HalFlushInvalidateDCache();
                GFX_Flush();
                GFX_Wait();        
        
                GFX_Unlock();
                break;
            default:
                printk(KERN_WARNING "mt53fb_copyarearop() error: bad rop %d!\n", area->rop);              
                i4Ret = -E_GFXLIB_ROP_ERR;
                break;
        }
    }

    x_sema_unlock(hMt53fbMutex);    
    
    return i4Ret;
}

INT32 mt53fb_copyarea(struct fb_info *info, const struct fb_copyarea *area)
{
    UINT32 u4Src;
    UINT32 u4Dst;
    UINT32 u4ResX;
    UINT32 u4ResY;
    INT32 i4Ret;
    UINT32 u4BPPShift;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
    
    u4BPPShift = _get_bpp_shift(info->var.bits_per_pixel);
    
    u4ResX = info->var.xres_virtual;
    u4ResY = info->var.yres_virtual;
    if ((area->sx < 0) || ((area->sx + area->width) > u4ResX) ||
        (area->sy < 0) || ((area->sy + area->height) > u4ResY) ||
        (area->dx < 0) || ((area->dx + area->width) > u4ResX) ||
        (area->dy < 0) || ((area->dy + area->height) > u4ResY))
    {
        i4Ret = -EINVAL;
    }
    else
    {
        //u4Src = info->fix.smem_start + (area->sx + info->var.xres_virtual * area->sy) * info->var.bits_per_pixel / 8;
        //u4Dst = info->fix.smem_start + (area->dx + info->var.xres_virtual * area->dy) * info->var.bits_per_pixel / 8;    
        u4Src = info->fix.smem_start;
        u4Dst = info->fix.smem_start;        
        GFX_Lock();
		GFX_Flush_Invalid_DCache();
        i4Ret = _imageblit_hw((void *)u4Dst, (void *)u4Src, 
        	area->dx, area->dy, info->var.xres_virtual << u4BPPShift,
        	area->sx, area->sy, info->var.xres_virtual << u4BPPShift,
        	area->width, area->height);
        //HalFlushInvalidateDCache();
        GFX_Flush();
        GFX_Wait();    
        GFX_Unlock();
    }

    x_sema_unlock(hMt53fbMutex);    
    return i4Ret;
}


INT32 mt53fb_imageblit(struct fb_info *info, const struct mt53fb_imageblit *image)
{
    //UINT32 u4Src;
    //UINT32 u4Dst;
    //UINT32 u4ResX;
    //UINT32 u4ResY;
    INT32 i4Ret;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    _i4DstBpp = image->i4DstBpp;
    _i4SrcBpp = image->i4SrcBpp;
    
    GFX_Lock();
	GFX_Flush_Invalid_DCache();
    i4Ret = _imageblit_hw(image->pu1DstBaseAddr, image->pu1SrcBaseAddr, 
    	image->u2DstX, image->u2DstY, image->u2DstPitch, 
    	image->u2SrcX, image->u2SrcY, image->u2SrcPitch, 
    	image->u2Width, image->u2Height);
    //HalFlushInvalidateDCache();
    GFX_Flush();
    GFX_Wait();    
    GFX_Unlock();

    x_sema_unlock(hMt53fbMutex);    
    return i4Ret;
}

INT32 mt53fb_imageblitrop(struct fb_info *info, const struct mt53fb_imageblitrop *image)
{
    //UINT32 u4Src;
    //UINT32 u4Dst;
    //UINT32 u4ResX;
    //UINT32 u4ResY;
    INT32 i4Ret;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    switch (image->i4Rop)
    {
        case ROP_CLEAR:
        case ROP_DST_IN:
        case ROP_DST_OUT:
        case ROP_DST_OVER:
        case ROP_SRC:
        case ROP_SRC_IN:
        case ROP_SRC_OUT:
        case ROP_SRC_OVER:
        case ROP_DST:
        case ROP_SRC_ATOP:
        case ROP_DST_ATOP:
        case ROP_XOR_GFX:
            _i4DstBpp = image->i4DstBpp;
            _i4SrcBpp = image->i4SrcBpp;        	
            GFX_Lock();        
			GFX_Flush_Invalid_DCache();
            i4Ret = _alphacomp_hw(image->pu1DstBaseAddr, image->pu1SrcBaseAddr, 
            	image->u2DstX, image->u2DstY, image->u2DstPitch, 
            	image->u2SrcX, image->u2SrcY, image->u2SrcPitch, 
            	image->u2Width, image->u2Height,
            	image->i4Alpha, image->i4Rop, 0, 0);
            //HalFlushInvalidateDCache();
            GFX_Flush();
            GFX_Wait();    
            GFX_Unlock();
            break;
        default:
            printk(KERN_WARNING "mt53fb_imageblitrop() error: bad rop %d!\n", image->i4Rop);          	
            i4Ret = -E_GFXLIB_ROP_ERR;
        break;
    }

    x_sema_unlock(hMt53fbMutex);    
    return i4Ret;
}

#ifdef CC_DFB_SUPPORT_VDP_LAYER

INT32 mt53fb_vdp_stretchblit(mt53fb_vdp_stretch_blit * pr_vdp_stretch_blit)
{
    INT32 i4Ret = 0;
    RZ_VDO_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;
    
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    x_memset(&rSclParam,0,sizeof(RZ_VDO_SCL_PARAM_SET_T));
    

    rSclParam.u4SrcSwap     = 0;
    rSclParam.u4IsRsIn      = 1;
    //1st source vdp Y buffer address
    rSclParam.u4YSrcBase    = pr_vdp_stretch_blit->src1_buffer_C_addr; 
    //1st source vdp Y buffer pitch
    rSclParam.u4YSrcBufLen  = pr_vdp_stretch_blit->src1_buffer_Y_pitch;    
    rSclParam.u4YSrcHOffset = 0;
    rSclParam.u4YSrcVOffset = 0;
    //1st source vdp Y buffer width
    rSclParam.u4YSrcW       = pr_vdp_stretch_blit->src1_buffer_Y_width; 
    //1st source vdp Y buffer height
    rSclParam.u4YSrcH       = pr_vdp_stretch_blit->src1_buffer_Y_height;       
    //1st source vdp C buffer address
    rSclParam.u4CSrcBase    = pr_vdp_stretch_blit->src1_buffer_C_addr;       
    rSclParam.u4CSrcHOffset = 0;
    rSclParam.u4CSrcVOffset = 0;
    //1st source vdp C buffer width (422 may be equals to Y)
    rSclParam.u4CSrcW       = pr_vdp_stretch_blit->src1_buffer_Y_width;       
    //1st source vdp C buffer height (422 may be equals to Y)
    rSclParam.u4CSrcH       = pr_vdp_stretch_blit->src1_buffer_Y_height;       

    rSclParam.u4OutSwap     = 0;
    rSclParam.u4IsRsOut     = 1;
    rSclParam.u4OutMode     = 1;    //422 video format output
    rSclParam.u4IsVdo2Osd   = 0;
    //destination Y buffer address
    rSclParam.u4YTgBase     = pr_vdp_stretch_blit->dest_buffer_Y_addr;       
    rSclParam.u4YTgCM       = 0;    //useful when u4IsVdo2Osd==1
    //destination Y buffer pitch
    rSclParam.u4YTgBufLen   = pr_vdp_stretch_blit->dest_buffer_Y_pitch;      
    rSclParam.u4YTgHOffset  = 0;
    rSclParam.u4YTgVOffset  = 0;
    //destination Y buffer width
    rSclParam.u4YTgW        = pr_vdp_stretch_blit->dest_buffer_Y_width;
    //destination Y buffer height
    rSclParam.u4YTgH        = pr_vdp_stretch_blit->dest_buffer_Y_height;     
    //destination C buffer address
    rSclParam.u4CTgBase     = pr_vdp_stretch_blit->dest_buffer_C_addr; 
    //destination C buffer width    (422 may be equals to Y)
    rSclParam.u4CTgW        = pr_vdp_stretch_blit->dest_buffer_Y_width;     
    //destination C buffer height   (422 may be equals to Y)
    rSclParam.u4CTgH        = pr_vdp_stretch_blit->dest_buffer_Y_height;       
    rSclParam.u4VdoCbCrSwap = 0;  
    
    /* blending level, 0~127, pixel percetage of 1nd cource VDP buffer
         (1st buffer pixel)*rSclParam.u4ABlend + 
         (2nd buffer pixel)*(0x80-rSclParam.u4ABlend)  */
    rSclParam.u4ABlend      = pr_vdp_stretch_blit->composition_level;      
    //2nd source vdp Y buffer                                
    rSclParam.u4PreLoadYAddr = pr_vdp_stretch_blit->src2_buffer_Y_addr;  
    //2nd source vdp C buffer
    rSclParam.u4PreLoadCAddr = pr_vdp_stretch_blit->src2_buffer_C_addr;     
    
    eRzType = E_RZ_INOUT_TYPE_VDOMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    x_sema_unlock(hMt53fbMutex);        

    return i4Ret;

}


#endif

INT32 mt53fb_stretchblit(struct fb_info *info, const struct mt53fb_StretchBlit *prStretch)
{
    INT32 i4Ret = 0;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;
    
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
	GFX_Flush_Invalid_DCache();

#if 0
    GFX_Lock();

    GFX_SetSrc((UINT8*)prStretch->u4Src, 
        prStretch->eSrcCm, prStretch->u4SrcPitch);

    GFX_SetDst((UINT8*)prStretch->u4Dst, 
        prStretch->eDstCm, prStretch->u4DstPitch);

    GFX_SetBltOpt((UINT32)D_GFXFLAG_NONE, 0, 0xffffffff);

    GFX_StretchBlt(prStretch->u4SrcX, prStretch->u4SrcY, prStretch->u4SrcWidth, prStretch->u4SrcHeight,
        prStretch->u4DstX, prStretch->u4DstY, prStretch->u4DstWidth, prStretch->u4DstHeight);

    GFX_Flush();
    GFX_Wait();    
    GFX_Unlock();
#else

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
    
    switch (prStretch->eSrcCm) {
    case CM_RGB565_DIRECT16:
       rSclParam.u4SrcCM  = E_RZ_OSD_DIR_CM_RGB565;
       break;

    case CM_ARGB4444_DIRECT16:
       rSclParam.u4SrcCM  = E_RZ_OSD_DIR_CM_ARGB4444;          
       break;

    case CM_ARGB8888_DIRECT32:
       rSclParam.u4SrcCM  = E_RZ_OSD_DIR_CM_ARGB8888;            
       break;

    default:
       printk(KERN_WARNING "mt53fb_stretchblit() error: bad eSrcCm %d!\n",prStretch->eSrcCm);        
       return E_GFXLIB_STRETCH_ERR;
    }

     switch (prStretch->eDstCm) {
      case CM_RGB565_DIRECT16:
            rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_RGB565;
           break;

      case CM_ARGB4444_DIRECT16:
            rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_ARGB4444;          
           break;

      case CM_ARGB8888_DIRECT32:
            rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_ARGB8888;            
           break;

      default:
            printk(KERN_WARNING "mt53fb_stretchblit() error: bad eDstCm %d!\n",prStretch->eDstCm);        
           return E_GFXLIB_STRETCH_ERR;
     }     

    //src original, absolute coordinate	
    rSclParam.u4SrcBase     = (UINT32) prStretch->u4Src;
    rSclParam.u4IsSrcDirCM  = (UINT32) 1;
    rSclParam.u4SrcBufLen   = (UINT32) prStretch->u4SrcPitch;
    rSclParam.u4SrcHOffset  = (UINT32) prStretch->u4SrcX;
    rSclParam.u4SrcVOffset  = (UINT32) prStretch->u4SrcY;
    rSclParam.u4SrcW        = (UINT32) prStretch->u4SrcWidth;
    rSclParam.u4SrcH        = (UINT32) prStretch->u4SrcHeight;

    //target original	, absolute coordinate
    rSclParam.u4TgBase      = (UINT32) prStretch->u4Dst;
    rSclParam.u4TgHOffset   = (UINT32) prStretch->u4DstX;
    rSclParam.u4TgVOffset   = (UINT32) prStretch->u4DstY;
    rSclParam.u4TgW         = (UINT32) prStretch->u4DstWidth;
    rSclParam.u4TgH         = (UINT32) prStretch->u4DstHeight;
    rSclParam.u4TgBufLen    = (UINT32) prStretch->u4DstPitch;

       //target clip	
       if ( (rSclParam.u4TgW != prStretch->u4ClipWidth) 	|| (rSclParam.u4TgH  != prStretch->u4ClipHeight))
       {
       	rSclParam.u4PartialUpdate = TRUE;

		//relative coordinate
		rSclParam.u4PartialUpdate_ClipTgx = (UINT32) prStretch->u4ClipX;
		rSclParam.u4PartialUpdate_ClipTgy = (UINT32) prStretch->u4ClipY;		
		rSclParam.u4PartialUpdate_ClipTgw = (UINT32) prStretch->u4ClipWidth;
		rSclParam.u4PartialUpdate_ClipTgh = (UINT32) prStretch->u4ClipHeight;
       }

    
    eRzType = E_RZ_INOUT_TYPE_OSDMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();
#endif
    x_sema_unlock(hMt53fbMutex);        

    return i4Ret;

}


INT32 mt53fb_imgrz_stretchalcom(struct fb_info *info, const struct mt53fb_StretchBlit *prStretch)
{
    INT32 i4Ret = 0;
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;
    UINT32 u4TmpBufAddr;

       
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    x_memset(&rSclParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));
    
    switch (prStretch->eSrcCm) {
    case CM_RGB565_DIRECT16:
       rSclParam.u4SrcCM  = E_RZ_OSD_DIR_CM_RGB565;
       break;

    case CM_ARGB4444_DIRECT16:
       rSclParam.u4SrcCM  = E_RZ_OSD_DIR_CM_ARGB4444;          
       break;

    case CM_ARGB8888_DIRECT32:
       rSclParam.u4SrcCM  = E_RZ_OSD_DIR_CM_ARGB8888;            
       break;

    default:
       printk(KERN_WARNING "mt53fb_stretchblit() error: bad eSrcCm %d!\n",prStretch->eSrcCm);        
       return E_GFXLIB_STRETCH_ERR;
    }

     switch (prStretch->eDstCm) {
      case CM_RGB565_DIRECT16:
            rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_RGB565;
           break;

      case CM_ARGB4444_DIRECT16:
            rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_ARGB4444;          
           break;

      case CM_ARGB8888_DIRECT32:
            rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_ARGB8888;            
           break;

      default:
            printk(KERN_WARNING "mt53fb_stretchblit() error: bad eDstCm %d!\n",prStretch->eDstCm);        
           return E_GFXLIB_STRETCH_ERR;
     }     

    //src original, absolute coordinate	
    rSclParam.u4SrcBase     = (UINT32) prStretch->u4Src;
    rSclParam.u4IsSrcDirCM  = (UINT32) 1;
    rSclParam.u4SrcBufLen   = (UINT32) prStretch->u4SrcPitch;
    rSclParam.u4SrcHOffset  = (UINT32) prStretch->u4SrcX;
    rSclParam.u4SrcVOffset  = (UINT32) prStretch->u4SrcY;
    rSclParam.u4SrcW        = (UINT32) prStretch->u4SrcWidth;
    rSclParam.u4SrcH        = (UINT32) prStretch->u4SrcHeight;
#ifdef LINUX_TURNKEY_SOLUTION
    u4TmpBufAddr = _mt53fb_get_tmpbufaddr();
    ASSERT(u4TmpBufAddr != 0);
    rSclParam.u4TgBase      = u4TmpBufAddr;       
#endif
    //target original	, absolute coordinate
    rSclParam.u4TgHOffset   = (UINT32) prStretch->u4DstX;
    rSclParam.u4TgVOffset   = (UINT32) prStretch->u4DstY;
    rSclParam.u4TgW         = (UINT32) prStretch->u4DstWidth;
    rSclParam.u4TgH         = (UINT32) prStretch->u4DstHeight;
    rSclParam.u4TgBufLen    = (UINT32) prStretch->u4DstPitch;
    
       //target clip	
       if ( (rSclParam.u4TgW != prStretch->u4ClipWidth) 	|| (rSclParam.u4TgH  != prStretch->u4ClipHeight))
       {
       	rSclParam.u4PartialUpdate = TRUE;

		//relative coordinate
		rSclParam.u4PartialUpdate_ClipTgx = (UINT32) prStretch->u4ClipX;
		rSclParam.u4PartialUpdate_ClipTgy = (UINT32) prStretch->u4ClipY;		
		rSclParam.u4PartialUpdate_ClipTgw = (UINT32) prStretch->u4ClipWidth;
		rSclParam.u4PartialUpdate_ClipTgh = (UINT32) prStretch->u4ClipHeight;
#if 0		
printf("33Clip(%d,",rSclParam.u4PartialUpdate_ClipTgx);
printf("%d,",rSclParam.u4PartialUpdate_ClipTgy);
printf("%d,",rSclParam.u4PartialUpdate_ClipTgw);
printf("%d)",rSclParam.u4PartialUpdate_ClipTgh );		
#endif
       }

#if 0
printf("Src(%d,",rSclParam.u4SrcHOffset );
printf("%d,",rSclParam.u4SrcVOffset);
printf("%d,",rSclParam.u4SrcW);
printf("%d)",rSclParam.u4SrcH );
printf("Dst(%d,",rSclParam.u4TgHOffset );
printf("%d,",rSclParam.u4TgVOffset );
printf("%d,",rSclParam.u4TgW );
printf("%d)\n",rSclParam.u4TgH);
#endif
    
    GFX_Flush_Invalid_DCache();
    eRzType = E_RZ_INOUT_TYPE_OSDMD;

    IMGRZ_Lock();
    IMGRZ_ReInit();
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

    GFX_Lock();

    GFX_SetSrc((UINT8 *)u4TmpBufAddr, (UINT32)prStretch->eDstCm, rSclParam.u4TgBufLen);
    GFX_SetDst((UINT8 *)prStretch->u4Dst, (UINT32)prStretch->eDstCm, prStretch->u4DstPitch);

    GFX_ComposeLoop(prStretch->u4DstX, prStretch->u4DstY, prStretch->u4DstX, prStretch->u4DstY, 
    	prStretch->u4ClipWidth, prStretch->u4ClipHeight, prStretch->u4Ar, prStretch->u4Porterdu, 0);    
    
    GFX_Flush();
    GFX_Wait();    
    GFX_Unlock();
    
    x_sema_unlock(hMt53fbMutex);        

    return i4Ret;

}

#ifdef CONFIG_OSD_COMPRESSION
INT32 mt53fb_compress(struct fb_info *info, const struct mt53fb_compress *prCompress)
{
    INT32 i4Ret = 0;

    GFX_Lock();
    
    GFX_MMU_Set_Cfg((void*)&prCompress->mmu);

    GFX_NewCompressBlt((UINT8 *)prCompress->u4Src, prCompress->u4SrcX, prCompress->u4SrcY, prCompress->eCm, 
    prCompress->u4Pitch, (UINT8 *)prCompress->u4Dst, prCompress->u4Width, prCompress->u4Height, prCompress->u4Height,
    1, 0, 0xFFFFFFFF, 0, 0, 0);

    GFX_Flush();
    GFX_Wait();    
    GFX_Unlock();
    
    return i4Ret;

}
#endif

#if 0
static VOID _rotate_resize_phase(struct mt53fb_vdp_rotate *prBlit, UINT32 u4CM, UINT32 u4DstW, UINT32 u4DstH)
{
    RZ_OSD_SCL_PARAM_SET_T rScaleParam;

    IMGRZ_Lock();
    IMGRZ_ReInit();

    x_memset(&rScaleParam,0,sizeof(RZ_OSD_SCL_PARAM_SET_T));

    rScaleParam.u4SrcBase = prBlit->u4Src;
    rScaleParam.u4IsSrcDirCM = 1;
    rScaleParam.u4SrcCM = u4CM;
    rScaleParam.u4SrcHOffset = prBlit->u4SrcX;//0;
    rScaleParam.u4SrcVOffset = prBlit->u4SrcY;//0;
    rScaleParam.u4SrcW = prBlit->u4Width;
    rScaleParam.u4SrcH = prBlit->u4Height;
    rScaleParam.u4TgBase = (UINT32)prBlit->u4Dst;
    rScaleParam.u4TgCM = E_RZ_OSD_DIR_CM_ARGB4444;

    rScaleParam.u4SrcBufLen = prBlit->u4SrcPitch;
    rScaleParam.u4TgBufLen = prBlit->u4DstPitch;
    rScaleParam.u4TgHOffset = prBlit->u4DstX;//0;
    rScaleParam.u4TgVOffset = prBlit->u4DstY;//0;
    rScaleParam.u4TgW = u4DstW;
    rScaleParam.u4TgH = u4DstH;
    IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_OSDMD);
    IMGRZ_Scale((void *)(&rScaleParam));

    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock();

}
#endif

static VOID _video_resize_phase(UINT32 YSrc, UINT32 Csrc, 
									UINT32 SrcX, UINT32 SrcY, UINT32 SrcPitch, UINT32 SrcWidth, UINT32 SrcHeight,
	    							UINT32 YDst, UINT32 CDst, 
	    							UINT32 DstX, UINT32 DstY, UINT32 DstPitch, UINT32 DstWidth, UINT32 DstHeight)
{
	RZ_VDO_SCL_PARAM_SET_T rSclParam;
	
	IMGRZ_Lock();
	IMGRZ_ReInit();

	x_memset((void *)&rSclParam, 0, sizeof(RZ_VDO_SCL_PARAM_SET_T));

	// set scaling parameters
	rSclParam.u4SrcSwap 	= 0;
	rSclParam.u4IsRsIn		= 1;
	rSclParam.u4YSrcBase	= YSrc;
	rSclParam.u4YSrcBufLen	= SrcPitch;
	rSclParam.u4CSrcBufLen	= SrcPitch;
	rSclParam.u4YSrcHOffset = SrcX;
	rSclParam.u4YSrcVOffset = SrcY;
	rSclParam.u4YSrcW		= SrcWidth;
	rSclParam.u4YSrcH		= SrcHeight;

	rSclParam.u4CSrcBase	= Csrc;
	rSclParam.u4CSrcHOffset = SrcX >> 1;
	rSclParam.u4CSrcVOffset = SrcY;
	rSclParam.u4CSrcW		= SrcWidth >> 1;
	rSclParam.u4CSrcH		= SrcHeight;

	rSclParam.u4OutSwap 	= 0;
	rSclParam.u4IsRsOut 	= 1;
	rSclParam.u4OutMode 	= 1;
	rSclParam.u4IsVdo2Osd	= 0;

	rSclParam.u4YTgBase 	= YDst;
	rSclParam.u4YTgCM		= 0;
	rSclParam.u4YTgBufLen	= DstPitch;
	rSclParam.u4YTgHOffset	= DstX;
	rSclParam.u4YTgVOffset	= DstY;
	rSclParam.u4YTgW		= DstWidth;
	rSclParam.u4YTgH		= DstHeight;

	rSclParam.u4CTgBase 	= CDst;
	rSclParam.u4CTgW		= DstWidth >> 1;
	rSclParam.u4CTgH		= DstHeight;
	rSclParam.u4VdoCbCrSwap = 0;
	rSclParam.u4ABlend 		= (UINT32) 0x80;

	IMGRZ_SetScaleOpt(E_RZ_INOUT_TYPE_VDOMD);
	IMGRZ_Scale((void *)(&rSclParam));

	IMGRZ_Flush();
	IMGRZ_Wait();
	IMGRZ_Unlock();

}


INT32 mt53fb_YC_sep_rotate(struct fb_info *info, struct mt53fb_vdp_rotate *prBlit)
{
    INT32 i4Ret = 0;
    UINT32 i;
    INT32 i4Sx, i4Sy, i4Dx, i4Dy;
    
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    GFX_Lock();
	GFX_Flush_Invalid_DCache();

    GFX_SetSrc((UINT8 *)prBlit->u4Src, CM_RGB_CLUT8, prBlit->u4SrcPitch);
    GFX_SetDst((UINT8 *)prBlit->u4Dst, CM_RGB_CLUT8, prBlit->u4DstPitch);   

    if (prBlit->u4RotateOp == MT53FB_VDP_ROTATE_270)
    {
        // Rotating Y
        i4Sx = (INT32)prBlit->u4SrcX;
        i4Sy = (INT32)prBlit->u4SrcY;
        i4Dx = (INT32)prBlit->u4DstX;
        //i4Dy = (INT32)prBlit->u4DstY + prBlit->u4Width;  
        i4Dy = (INT32)prBlit->u4DstY + prBlit->u4Width - 1; 
        
        // rotated by 90" counter-clock-wise
        GFX_SetHoriToVertLineOpt(1);

        for(i = 0; i < prBlit->u4Height; i++)
        {
            GFX_HoriToVertLine((UINT32)i4Sx, (UINT32)i4Sy, 
                (UINT32)i4Dx, (UINT32)i4Dy, prBlit->u4Width);

            i4Sy++;
            i4Dx++;
        } // ~for
    }
    else if(prBlit->u4RotateOp == MT53FB_VDP_ROTATE_90)
    {
        // Rotating Y
        i4Sx = (INT32)prBlit->u4SrcX;
        i4Sy = (INT32)prBlit->u4SrcY;
        i4Dx = (INT32)prBlit->u4DstX + prBlit->u4Height;
        i4Dy = (INT32)prBlit->u4DstY;               
        
        // rotated by 90" clock-wise
        GFX_SetHoriToVertLineOpt(0);

        for(i = 0; i < prBlit->u4Height; i++)
        {
            GFX_HoriToVertLine((UINT32)i4Sx, (UINT32)i4Sy, 
                (UINT32)i4Dx, (UINT32)i4Dy, prBlit->u4Width);

            i4Sy++;
            i4Dx--;
        } // ~for
    }
    else if(prBlit->u4RotateOp == MT53FB_VDP_ROTATE_180)
    {
        // To use mirror&flip to do rotate 180
      UINT32 u4Switch = 0;
	  u4Switch = u4Switch|D_GFXFLAG_MIRROR|D_GFXFLAG_FLIP;
      #ifdef CC_GFX_FLIP_MIRROR	
      GFX_SetBltOpt(u4Switch, 0, 0);
      GFX_BitBlt(prBlit->u4SrcX, prBlit->u4SrcY, prBlit->u4DstX, prBlit->u4DstY, prBlit->u4Width, prBlit->u4Height);
	  #endif	  
    }

    GFX_Flush();
    GFX_Wait();    
    GFX_Unlock();  
    
    if (prBlit->u4RotateOp == MT53FB_VDP_ROTATE_270)
    {

        struct mt53fb_vdp_rotate rResize;

 //resize to width x height/2, and then rotate to height/2 x width
             //---CbCr rotate
        rResize.u4Src = prBlit->u4Src+prBlit->u4SrcCbCrOffset;
        rResize.u4Dst = _mt53fb_get_tmpbufaddr();
        rResize.u4Width = prBlit->u4Width >> 1;
        rResize.u4Height = prBlit->u4Height;
        rResize.u4DstX = 0;
        rResize.u4DstY = 0;        
        rResize.u4SrcX = prBlit->u4SrcX >> 1;
        rResize.u4SrcY = prBlit->u4SrcY;
        rResize.u4SrcPitch = prBlit->u4SrcPitch;
        rResize.u4DstPitch = prBlit->u4Width << 1;
        //_rotate_resize_phase(&rResize, E_RZ_OSD_DIR_CM_ARGB4444, prBlit->u4Width, prBlit->u4Height >> 1); 
        _video_resize_phase(prBlit->u4Src, prBlit->u4Src+prBlit->u4SrcCbCrOffset, prBlit->u4SrcX, prBlit->u4SrcY, 
        					prBlit->u4SrcPitch, prBlit->u4Width, prBlit->u4Height,
	    					_mt53fb_get_tmpbufaddr()+ prBlit->u4SrcCbCrOffset, _mt53fb_get_tmpbufaddr(), 0, 0, 
	    					rResize.u4DstPitch, rResize.u4DstPitch,prBlit->u4Height>>1);

        GFX_Lock();   
        
        //prBlit->u4Height = prBlit->u4Height >> 1;
        
        GFX_SetSrc((UINT8 *)_mt53fb_get_tmpbufaddr(), CM_RGB565_DIRECT16, rResize.u4DstPitch);
        GFX_SetDst((UINT8 *)(prBlit->u4Dst+prBlit->u4DstCbCrOffset), CM_RGB565_DIRECT16, prBlit->u4DstPitch);    
    
        // Rotating CbCr
        prBlit->u4Height >>= 1;         
        i4Sx = (INT32)0;
        i4Sy = (INT32)0;
        i4Dx = (INT32)prBlit->u4DstX>>1;
        //i4Dy = (INT32)prBlit->u4DstY + prBlit->u4Width; 
		i4Dy = (INT32)prBlit->u4DstY + prBlit->u4Width - 1; 

        // rotated by 90" counter-clock-wise
        GFX_SetHoriToVertLineOpt(1);

        for(i = 0; i < prBlit->u4Height; i++)
        {
            GFX_HoriToVertLine((UINT32)i4Sx, (UINT32)i4Sy, 
                (UINT32)i4Dx, (UINT32)i4Dy, prBlit->u4Width);

            i4Sy++;
            i4Dx++;
        } // ~for

        GFX_Flush();
        GFX_Wait();    
        GFX_Unlock();   
        
    }
    else if(prBlit->u4RotateOp == MT53FB_VDP_ROTATE_90)
    {
    
        struct mt53fb_vdp_rotate rResize;

             //---CbCr rotate
        rResize.u4Src = prBlit->u4Src+prBlit->u4SrcCbCrOffset;
        rResize.u4Dst = _mt53fb_get_tmpbufaddr();
        rResize.u4Width = prBlit->u4Width >> 1;
        rResize.u4Height = prBlit->u4Height;
        rResize.u4DstX = 0;
        rResize.u4DstY = 0;        
        rResize.u4SrcX = prBlit->u4SrcX >> 1;
        rResize.u4SrcY = prBlit->u4SrcY;
        rResize.u4SrcPitch = prBlit->u4SrcPitch;
        rResize.u4DstPitch = prBlit->u4Width << 1;
        //_rotate_resize_phase(&rResize, E_RZ_OSD_DIR_CM_ARGB4444, prBlit->u4Width, prBlit->u4Height >> 1);
        _video_resize_phase(prBlit->u4Src, prBlit->u4Src+prBlit->u4SrcCbCrOffset, prBlit->u4SrcX, prBlit->u4SrcY, 
        					prBlit->u4SrcPitch, prBlit->u4Width, prBlit->u4Height,
	    					_mt53fb_get_tmpbufaddr()+ prBlit->u4SrcCbCrOffset, _mt53fb_get_tmpbufaddr(), 0, 0, 
	    					rResize.u4DstPitch, rResize.u4DstPitch,prBlit->u4Height>>1);
    
        GFX_Lock();   

        //prBlit->u4Height = prBlit->u4Height >> 1;
        
        GFX_SetSrc((UINT8 *)_mt53fb_get_tmpbufaddr(), CM_RGB565_DIRECT16, rResize.u4DstPitch);
        GFX_SetDst((UINT8 *)(prBlit->u4Dst+prBlit->u4DstCbCrOffset), CM_RGB565_DIRECT16, prBlit->u4DstPitch);   
    
        // Rotating CbCr
        prBlit->u4Height >>= 1;         
        i4Sx = (INT32)0;
        i4Sy = (INT32)0;
        i4Dx = ((INT32)prBlit->u4DstX>>1) + prBlit->u4Height;
        i4Dy = (INT32)prBlit->u4DstY; 
        
        // rotated by 90" clock-wise        
        GFX_SetHoriToVertLineOpt(0);

        for(i = 0; i < prBlit->u4Height; i++)
        {
            GFX_HoriToVertLine((UINT32)i4Sx, (UINT32)i4Sy, 
                (UINT32)i4Dx, (UINT32)i4Dy, prBlit->u4Width);

            i4Sy++;
            i4Dx--;
        } // ~for
        
        GFX_Flush();
        GFX_Wait();    
        GFX_Unlock();   
        
    }
    else if(prBlit->u4RotateOp == MT53FB_VDP_ROTATE_180)
    {
        // To use mirror&flip to do rotate 180 
     UINT32 u4Switch = 0;
     //, u4TemAddr = 0;
	      		
	 u4Switch = u4Switch |D_GFXFLAG_MIRROR|D_GFXFLAG_FLIP;
     GFX_Lock();
	 GFX_SetSrc((UINT8 *)(prBlit->u4Src+ prBlit->u4SrcCbCrOffset), CM_ARGB4444_DIRECT16, prBlit->u4SrcPitch);
	 GFX_SetDst((UINT8 *)prBlit->u4Dst+ prBlit->u4DstCbCrOffset, CM_ARGB4444_DIRECT16, prBlit->u4DstPitch);
	 #if 0
	 // CbCr dram address need old address
	  u4TemAddr = (UINT32)(prBlit->u4Dst+ prBlit->u4DstCbCrOffset + 
                            ((prBlit->u4DstY + prBlit->u4Height - 1) * prBlit->u4DstPitch) + 
                            ((prBlit->u4DstX>>1) + prBlit->u4Width - 1) );
	 if(u4TemAddr%2 !=0)
	 {
	   GFX_SetDst((UINT8 *)(prBlit->u4Dst+ prBlit->u4DstCbCrOffset - 1), CM_ARGB4444_DIRECT16, prBlit->u4DstPitch); 
	 }
	 else
	 {
	   GFX_SetDst((UINT8 *)(prBlit->u4Dst+ prBlit->u4DstCbCrOffset), CM_ARGB4444_DIRECT16, prBlit->u4DstPitch);
	 }
	 #endif
#ifdef CC_GFX_FLIP_MIRROR 
	 GFX_SetBltOpt(u4Switch, 0, 0);
	 GFX_BitBlt(prBlit->u4SrcX>>1, prBlit->u4SrcY, prBlit->u4DstX>>1, prBlit->u4DstY, prBlit->u4Width>>1, prBlit->u4Height);
#endif	  
     GFX_Flush();
     GFX_Wait();    
     GFX_Unlock();       

	
    }   
    
    x_sema_unlock(hMt53fbMutex);        

    return i4Ret;

}


INT32 mt53fb_fillrect_ioctl(void* pvArg)
{
    struct fb_info* prFb;
   // struct fb_fillrect rRect;
    struct mt53fb_fillrect rRect;		
    prFb = mt53fb_get_fb_info();
    
    if (copy_from_user(&rRect, pvArg, sizeof(struct fb_fillrect)))
    {
        printk(KERN_WARNING "mt53fb_fillrect_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_fillrect(prFb, &rRect);   
}

INT32 mt53fb_fillrectrop_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_fillrectrop rRect;

    prFb = mt53fb_get_fb_info();
    
    if (copy_from_user(&rRect, pvArg, sizeof(struct mt53fb_fillrectrop)))
    {
        printk(KERN_WARNING "mt53fb_fillrectrop_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_fillrectrop(prFb, &rRect);    
}

INT32 mt53fb_copyarea_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct fb_copyarea rArea;

    prFb = mt53fb_get_fb_info();
    
    if (copy_from_user(&rArea, pvArg, sizeof(struct fb_copyarea)))
    {
        printk(KERN_WARNING "mt53fb_copyarea_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_copyarea(prFb, &rArea);    
}

INT32 mt53fb_copyarearop_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_copyarearop rArea;

    prFb = mt53fb_get_fb_info();
    
    if (copy_from_user(&rArea, pvArg, sizeof(struct mt53fb_copyarearop)))
    {
        printk(KERN_WARNING "mt53fb_copyarearop_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_copyarearop(prFb, &rArea);    
}

INT32 mt53fb_imageblit_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_imageblit rImg;
    
    prFb = mt53fb_get_fb_info();
    
    if(copy_from_user((void*)&rImg, pvArg, sizeof(struct mt53fb_imageblit)))
    {
        printk(KERN_WARNING "mt53fb_imageblit_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_imageblit(prFb, &rImg);   
}
INT32 mt53fb_imageblitrop_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_imageblitrop rImg;
    
    prFb = mt53fb_get_fb_info();
    
    if(copy_from_user((void*)&rImg, pvArg, sizeof(struct mt53fb_imageblitrop)))
    {
        printk(KERN_WARNING "mt53fb_imageblitrop_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_imageblitrop(prFb, &rImg);   
}

INT32 mt53fb_drawrectrop_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_DrawRectRop rDrawRectRop;
    
    prFb = mt53fb_get_fb_info();
    
    if(copy_from_user((void*)&rDrawRectRop, pvArg, sizeof(struct mt53fb_DrawRectRop)))
    {
        printk(KERN_WARNING "mt53fb_drawrectrop_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_drawrectrop(prFb, &rDrawRectRop); 

}

INT32 mt53fb_drawrect_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_DrawRect rDrawRect;
    
    prFb = mt53fb_get_fb_info();
    
    if(copy_from_user((void*)&rDrawRect, pvArg, sizeof(struct mt53fb_DrawRect)))
    {
        printk(KERN_WARNING "mt53fb_drawrect_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_drawrect(prFb, &rDrawRect); 

}

INT32 mt53fb_drawlinerop_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_DrawLineRop rDrawLineRop;
    
    prFb = mt53fb_get_fb_info();
    
    if(copy_from_user((void*)&rDrawLineRop, pvArg, sizeof(struct mt53fb_DrawLineRop)))
    {
        printk(KERN_WARNING "mt53fb_drawlinerop_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_drawlinerop(prFb, &rDrawLineRop); 

}

INT32 mt53fb_drawline_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_DrawLine rDrawLine;
    
    prFb = mt53fb_get_fb_info();
    //printk("drawline_ioctl\n");
    if(copy_from_user((void*)&rDrawLine, pvArg, sizeof(struct mt53fb_DrawLine)))
    {
        printk(KERN_WARNING "mt53fb_drawlinerop_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    return mt53fb_drawline(prFb, &rDrawLine); 

}
INT32 mt53fb_flush_ex_action(void* pvArg)
{
    EX_DeviceData r_ex_data;
     
    if(!pvArg)
    {
        return 0x0;
    }
    
    if(copy_from_user((void*)&r_ex_data, pvArg, sizeof(EX_DeviceData)))
    {
        printk(KERN_WARNING "mt53fb_flush_ex_action() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    if(DFB_GFX_FLUSH_INVALID_DCACHE == r_ex_data.cache.ui4_flush_fg)
    {
        GFX_Flush_Invalid_DCache();
    }
            
    return 0x0;
}

#if FB_DBG
INT32 _mt53fb_get_time(void* pvArg)
{
    HAL_TIME_T rT;
    HAL_GetTime2(&rT);
    copy_to_user(pvArg, &rT, sizeof(HAL_TIME_T));
    return 0;
}
#endif

INT32 mt53fb_get_pgt_ioctl(unsigned long pvArg)
{  
	return _MTMMU_GetPhyPgd(pvArg);
}

INT32 mt53fb_pin_mem_ioctl(unsigned long pvArg)
{   
    return _MTMMU_PinMem(pvArg);
}

INT32 mt53fb_unpin_mem_ioctl(unsigned long pvArg)
{  
	return _MTMMU_UnPinMem(pvArg);
}	

INT32 mt53fb_dma_map_single_ioctl(unsigned long pvArg)
{  
	return _MTMMU_DMA_MAP_SINGLE(pvArg);
}

INT32 mt53fb_dma_unmap_single_ioctl(unsigned long pvArg)
{  
	return _MTMMU_DMA_UNMAP_SINGLE(pvArg);
}

INT32 _mt53fb_gfx_stretch_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_StretchBlit rStretchBlit;

    prFb = mt53fb_get_fb_info();

    if(copy_from_user((void*)&rStretchBlit, pvArg, sizeof(struct mt53fb_StretchBlit)))
    {
        printk(KERN_WARNING "_mt53fb_gfx_stretch_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }

    return mt53fb_stretchblit(prFb, &rStretchBlit); 

}
INT32 _mt53fb_vgfx_ioctl(void *pvArg)
{
    VGFX_SCALER_PARAM_SET_T    rStretchBlit;

    if(copy_from_user((void*)&rStretchBlit, pvArg, sizeof(VGFX_SCALER_PARAM_SET_T)))
    {
        printk(KERN_WARNING "_mt53fb_vgfx_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    GFX_Lock();
    GFX_Flush_Invalid_DCache();
	
    _VGfx_SetVGfxScaler(&rStretchBlit); 
    
    GFX_Flush();          
    GFX_Wait(); 
    GFX_Unlock();
    
    x_sema_unlock(hMt53fbMutex);

    return 0x0;

}

INT32 _mt53fb_vgfx_scaler_ioctl(void* pvArg)
{
    #if (defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) )
    MT53_ACCEL_MMU_T            mmu;
    VGFX_PARAM_SET_T    rStretchBlit;

    if(copy_from_user((void*)&rStretchBlit, pvArg, sizeof(VGFX_PARAM_SET_T)))
    {
        printk(KERN_WARNING "_mt53fb_vgfx_scaler_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }
    
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    GFX_Lock();
    GFX_Flush_Invalid_DCache();
    mmu.u4_init         =0x0;
    mmu.u4_enable       =0x1;
    mmu.u4_cmprss       =0x0;
    mmu.u4_src_rw_mmu   =0x1;
    mmu.u4_dst_rw_mmu   =0x1;
    mmu.u4_vgfx_ord     =0x1;
    mmu.u4_pgt          =0x1;
    mmu.u4_vgfx_slva = (rStretchBlit.u4ImgAddr +
        ((rStretchBlit.u4TextureVStart + rStretchBlit.u4TextureHeight)*
        rStretchBlit.u4ImgPitch*GFX_Get_Cm_Size(rStretchBlit.u4ImgClrMode)));
    
    GFX_MMU_Set_Cfg(&mmu);
    
    
    _VGFX_Scaler(&rStretchBlit); 
    
    GFX_Flush();          
    GFX_Wait(); 
    GFX_Unlock();
    
    x_sema_unlock(hMt53fbMutex);

    return 0x0;
    
    #else
    return EIO;
    #endif
}


INT32 _mt53fb_imgrz_stretch_alcom_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_StretchBlit rStretchBlit;

    prFb = mt53fb_get_fb_info();

    if(copy_from_user((void*)&rStretchBlit, pvArg, sizeof(struct mt53fb_StretchBlit)))
    {
        printk(KERN_WARNING "_mt53fb_gfx_stretch_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }

    return mt53fb_imgrz_stretchalcom(prFb, &rStretchBlit); 

}

#ifdef CC_DFB_SUPPORT_VDP_LAYER
INT32 _mt53fb_vdp_rotate_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_vdp_rotate rVdpBlit;

    prFb = mt53fb_get_fb_info();

    if(copy_from_user((void*)&rVdpBlit, pvArg, sizeof(struct mt53fb_vdp_rotate)))
    {
        printk(KERN_WARNING "_mt53fb_vdp_blit_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }

    return mt53fb_YC_sep_rotate(prFb, &rVdpBlit); 

}
#endif

