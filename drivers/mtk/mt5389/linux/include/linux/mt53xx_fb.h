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
#ifndef FB_MT53XX_H
#define FB_MT53XX_H

#define FB_INIT_BY_AP 1
#define FB_DBG 0
#define FB_PROF 0

#if FB_PROF
#undef FB_DBG
#define FB_DBG 1
#endif

#define FBIO_RECTFILL 0x4620
#define FBIO_COPYAREA 0x4621
#define FBIO_IMAGEBLIT 0x4622
#define FBIO_REGISTERBUFFER 0x4623
#define FBIO_UNREGISTERBUFFER 0x4624
#define FBIO_RECTFILLROP 0x4625
#define FBIO_COPYAREAROP 0x4626
#define FBIO_SET 0x4627
#define FBIO_PALETTE 0x4628
#define FBIO_GETIMAGEBUFFER 0x4629
#define FBIO_IMAGEBLITROP 0x462A
#define FBIO_FONTRENDER 0x462B
#define FBIO_SETBLITOPT 0x462c
//new function
#define FBIO_DRAWLINE 0x462D
#define FBIO_DRAWLINEROP 0x462E
#define FBIO_DRAWRECT 0x462F
#define FBIO_DRAWRECTROP 0x4632
#define FBIO_DRAWTEXT 0x4631
// For DirectFB user mode cmdque
#define FBIO_GFX_FLUSH 0x4640
#define FBIO_GFX_WAIT 0x4641

// For get /set plane order array to sync with m/w
#define FBIO_GET_PLANE_ORDER_ARRAY 0x4642
#define FBIO_SET_PLANE_ORDER_ARRAY 0x4643
#define FBIO_SET_OSD_PLANE_ORDER_ARRAY 0x4644
#define FBIO_GET 0x4645

// For DirectFB & FB share lock
#define FBIO_GFX_LOCK 0x4646
#define FBIO_GFX_UNLOCK 0x4647

#if FB_INIT_BY_AP
#define FBIO_INIT 0x4630
#endif
#if FB_DBG
#define FBIO_GETTIME 0x4630
#endif

#define MT53FB_SET_X 0
#define MT53FB_SET_Y 1
#define MT53FB_SET_W 2
#define MT53FB_SET_H 3
#define MT53FB_SET_VIRT_W 4
#define MT53FB_SET_VIRT_H 5	
#define MT53FB_SET_OFFSET_X 6
#define MT53FB_SET_OFFSET_Y 7
#define MT53FB_SET_BPP 8
#define MT53FB_SET_CM 9
#define MT53FB_SET_VISIBLE 10
#define MT53FB_SET_BASE 11
#define MT53FB_SET_PITCH 12
#define MT53FB_SET_ORDER 13
#define MT53FB_SET_OPACITY 14
#define MT53FB_SET_MIXSEL 15
#define MT53FB_SET_COLORKEYEN 16
#define MT53FB_SET_COLORKEY 17
#define MT53FB_SET_PLANE_ID 18


#define MT53FB_SET_MASK_X (1u << MT53FB_SET_X)
#define MT53FB_SET_MASK_Y (1u << MT53FB_SET_Y)
#define MT53FB_SET_MASK_W (1u << MT53FB_SET_W)
#define MT53FB_SET_MASK_H (1u << MT53FB_SET_H)
#define MT53FB_SET_MASK_VIRT_W (1u << MT53FB_SET_VIRT_W)
#define MT53FB_SET_MASK_VIRT_H (1u << MT53FB_SET_VIRT_H)
#define MT53FB_SET_MASK_OFFSET_X (1u << MT53FB_SET_OFFSET_X)
#define MT53FB_SET_MASK_OFFSET_Y (1u << MT53FB_SET_OFFSET_Y)
#define MT53FB_SET_MASK_BPP (1u << MT53FB_SET_BPP)
#define MT53FB_SET_MASK_CM (1u << MT53FB_SET_CM)
#define MT53FB_SET_MASK_VISIBLE (1u << MT53FB_SET_VISIBLE)
#define MT53FB_SET_MASK_BASE (1u << MT53FB_SET_BASE)
#define MT53FB_SET_MASK_PITCH (1u << MT53FB_SET_PITCH)
#define MT53FB_SET_MASK_ORDER (1u << MT53FB_SET_ORDER)
#define MT53FB_SET_MASK_OPACITY (1u << MT53FB_SET_OPACITY)
#define MT53FB_SET_MASK_MIXSEL (1u << MT53FB_SET_MIXSEL)
#define MT53FB_SET_MASK_COLORKEY (1u << MT53FB_SET_COLORKEY)
#define MT53FB_SET_MASK_COLORKEYEN (1u << MT53FB_SET_COLORKEYEN)
#define MT53FB_SET_MASK_PLANE_ID (1u << MT53FB_SET_PLANE_ID)

#define MT53FB_GET_PANEL_SIZE 0


//new feature
struct mt53fb_DrawLine
{
    unsigned short u2X1;
    unsigned short u2Y1;
    unsigned short u2X2;
    unsigned short u2Y2;
    unsigned short u2DstPitch;
    unsigned int            u4color;
    unsigned char* pu1DstBaseAddr;
    unsigned int             u4Bpp;
};

struct mt53fb_DrawLineRop
{
    unsigned short u2X1;
    unsigned short u2Y1;
    unsigned short u2X2;
    unsigned short u2Y2;
    unsigned short u2DstPitch;
    unsigned int   u4color;
    unsigned int   u4Rop;
    unsigned short u2Alpha;
    unsigned char* pu1DstBaseAddr;
    unsigned int             u4Bpp;
};

struct mt53fb_DrawRect {
    unsigned short                  u2DstX;
    unsigned short                  u2DstY;
    unsigned short                  u2DstPitch;
    // Rectangular's width and height
    unsigned short                  u2Width;
    unsigned short                  u2Height;
    // Destiniation paper
    unsigned char*                  pu1DstBaseAddr;
    // Color
    unsigned int	        	                u4Color;
    unsigned int			                    u4Bpp;
    unsigned int                            u4BoarderWidth;
};

struct mt53fb_DrawRectRop {
    unsigned short                  u2DstX;
    unsigned short                  u2DstY;
    unsigned short                  u2DstPitch;
    // Rectangular's width and height
    unsigned short                  u2Width;
    unsigned short                  u2Height;
    // Destiniation paper
    unsigned char*                  pu1DstBaseAddr;
    // Color
    unsigned int                             u4Color; 
    unsigned int			                    u4Alpha;    
    unsigned int 			                u4Rop;
    unsigned int			                    u4Bpp;    
    unsigned int                             u4BoarderWidth;
};

struct mt53fb_DrawText {
    unsigned short u2X;
    unsigned short u2Y;
    unsigned short u2Width;
    unsigned short u2Height;
    unsigned short u2DstPitch;
    unsigned char*          pTextArray;
    unsigned char*  pu1DstBaseAddr;
    unsigned int   u4Color;
    unsigned int   u4Alpha;
    unsigned int   u4Bpp;
};



//end new feature

struct mt53fb_imagebuffer
{
    unsigned int u4Size;
    unsigned int u4VirtAddr;
    unsigned int u4PhyAddr;
};

struct mt53fb_imageblit
{
    unsigned short       u2SrcX;
    unsigned short       u2SrcY;
    unsigned short       u2SrcPitch;
    unsigned short                  u2DstX;
    unsigned short                  u2DstY;
    unsigned short                  u2DstPitch;
    // Blitting area's width and height
    unsigned short                  u2Width;
    unsigned short                  u2Height;
    // Source paper
    unsigned char*                  pu1SrcBaseAddr;
    // Destination paper
    unsigned char*                  pu1DstBaseAddr;

    int			i4SrcBpp;   
    int			i4DstBpp;     
    
};

struct mt53fb_imageblitrop
{
    unsigned short                  u2SrcX;
    unsigned short                  u2SrcY;
    unsigned short                  u2SrcPitch;
    unsigned short                  u2DstX;
    unsigned short                  u2DstY;
    unsigned short                  u2DstPitch;
    // Blitting area's width and height
    unsigned short                  u2Width;
    unsigned short                  u2Height;
    // Source paper
    unsigned char*                  pu1SrcBaseAddr;
    // Destination paper
    unsigned char*                  pu1DstBaseAddr;
        
    int					 i4Alpha;
    int					 i4Rop;   
    
    int			i4SrcBpp;   
    int			i4DstBpp;      
    
    
};

struct mt53fb_fillrect {
    unsigned short                  u2DstX;
    unsigned short                  u2DstY;
    unsigned short                  u2DstPitch;
    // Rectangular's width and height
    unsigned short                  u2Width;
    unsigned short                  u2Height;
    // Destiniation paper
    unsigned char*                  pu1DstBaseAddr;
    // Color
    int			                  i4Color;

    int			i4Bpp;
    
};

struct mt53fb_fillrectrop {
    unsigned short                  u2DstX;
    unsigned short                  u2DstY;
    unsigned short                  u2DstPitch;
    // Rectangular's width and height
    unsigned short                  u2Width;
    unsigned short                  u2Height;
    // Destiniation paper
    unsigned char*                  pu1DstBaseAddr;
    // Color
    int                     i4Color;
       
    int			 i4Alpha;    
    int 		i4Rop;
   
    int			i4Bpp; 
};

struct mt53fb_copyarearop 
{
	unsigned int dx;
	unsigned int dy;
	unsigned int width;
	unsigned int height;
	unsigned int sx;
	unsigned int sy;
	unsigned int rop;	
	unsigned int alpha;
};

struct mt53fb_setting 
{
    unsigned int u4Pitch;
    unsigned int u1CM;	
    unsigned int u2X;
    unsigned int u2Y;
    unsigned int u2W;
    unsigned int u2H;   
    unsigned int u2OffsetX;
    unsigned int u2OffsetY;    
    unsigned int fgVisible;
    unsigned int u4Base;
    unsigned int u4Order;    
    unsigned int u4Opacity;        
    unsigned int u4MixSel;       
    unsigned int u4ColorKey;
    unsigned int u4ColorKeyEn;
    unsigned int u4OsdPlaneID;        
};   

struct mt53fb_set 
{
    struct mt53fb_setting rSetting;
    unsigned int mask;
};

struct mt53fb_get 
{
    unsigned int get_type;
    unsigned int get_size;
    unsigned int *get_data;
};

struct mt53fb_palette
{
    unsigned int plane_id;
    unsigned int palette[256];
};

typedef enum
{
    E_GFXLIB_SRC_X_ERR = 1000,
    E_GFXLIB_SRC_Y_ERR,
    E_GFXLIB_DST_X_ERR,
    E_GFXLIB_DST_Y_ERR,
    E_GFXLIB_WIDTH_ERR,
    E_GFXLIB_HEIGHT_ERR,
    E_GFXLIB_SRC_PITCH_ERR,
    E_GFXLIB_DST_PITCH_ERR,
    E_GFXLIB_SRC_ADDR_ERR,
    E_GFXLIB_DST_ADDR_ERR,
    E_GFXLIB_ROP_ERR,
    E_GFXLIB_PALE_ADDR_ERR,
    
    E_GFXLIB_SET_X_ERR,
    E_GFXLIB_SET_Y_ERR,
    E_GFXLIB_SET_WIDTH_ERR,
    E_GFXLIB_SET_HEIGHT_ERR,
    E_GFXLIB_SET_PITCH_ERR,
    E_GFXLIB_SET_BPP_ERR,    
    E_GFXLIB_SET_ADDR_ERR,
    E_GFXLIB_SET_COLORMODE_ERR,
    E_GFXLIB_SET_ENABLE_ERR,

    E_GFXLIB_ERR_MAX
} E_GFXLIB_T;

#endif /*__MT53_FB__*/
