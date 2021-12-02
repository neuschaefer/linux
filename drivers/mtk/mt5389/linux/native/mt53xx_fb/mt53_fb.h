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

#if ! defined(__MT53_FB__)
#define __MT53_FB__

#define FB_INIT_BY_AP 1
#define FB_DBG 0
#define FB_PROF 0
#define USE_FBM_PREALLOCATION 1

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
#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB 
#define FBIO_GETIMAGEBUFFER2 0x4633
#endif /* CC_FBM_TWO_FBP_SHARED_WITH_DFB  */
#define FBIO_GETFRAMEBUFFER 0x4634

// For user mode cmdque
#define FBIO_GFX_FLUSH 0x4640
#define FBIO_GFX_WAIT 0x4641

#define FBIO_GET_PLANE_ORDER_ARRAY 0x4642
#define FBIO_SET_PLANE_ORDER_ARRAY 0x4643
#define FBIO_SET_OSD_PLANE_ORDER_ARRAY 0x4644
#define FBIO_GET 0x4645

// For DirectFB & FB share lock
#define FBIO_GFX_LOCK 0x4646
#define FBIO_GFX_UNLOCK 0x4647
#define FBIO_GFX_STRETCH 0x4648
#define FBIO_WAITLOGO 0x4649
#define FBIO_GFX_WAIT_VDP 0x464A
#define FBIO_IMGRZ_STRETCH_ALCOM 0x464B

#define ROP_CLEAR       0
#define ROP_DST_IN      1
#define ROP_DST_OUT     2
#define ROP_DST_OVER    3
#define ROP_SRC         4
#define ROP_SRC_IN      5
#define ROP_SRC_OUT     6
#define ROP_SRC_OVER    7
#define ROP_DST         8
#define ROP_SRC_ATOP    9
#define ROP_DST_ATOP    10
#define ROP_XOR_GFX     11

#if FB_INIT_BY_AP
#define FBIO_INIT 0x4630
#endif
#if FB_DBG
#define FBIO_GETTIME 0x4630
#endif

#ifdef CC_TDTV_3D_OSD_ENABLE
#define FBIO_GET_SBSOUTPUT 0x464C
#define FBIO_GET_TBBOUTPUT 0x464D
#define FBIO_GET_3DOSD_SHIFT 0x464E
#endif

#define FBIO_VGFX_STRETCH_ALCOM 	  0x4650
#define FBIO_VGFX_SCALER	  0x4651

#if 1 //def CC_DFB_SUPPORT_VDP_LAYER
#define FBIO_VDP_INIT 0x4660
#define FBIO_VDP_FLIP 0x4661
#define FBIO_VDP_SET 0x4662
#define FBIO_VDP_ROTATE 0x4663
#define FBIO_VDP_GETBUFFER 0x4664
#define FBIO_VDP_GET_CROP_RECT 0x4665
#define FBIO_VDP_SET_CROP_RECT 0x4666
#define FBIO_VDP_STRETCH_BLIT   0x4667
#endif

#define FBIO_GFX_FLUSH_DCACHE   0x4700
#define FBIO_MMU_GETPGD                             0x4670               
#define FBIO_MMU_PINMEM                             0x4671               
#define FBIO_MMU_UNPINMEM                           0x4672           
#define FBIO_MMU_MAPSINGLE                          0x4673             
#define FBIO_MMU_UNMAPSINGLE                        0x4674


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
/* This is for middleware private gl operations.*/
#define FBIO_MW_OP  0x5000


struct mt53fb_imagebuffer
{
    unsigned int u4Size;
    unsigned int u4VirtAddr;
    unsigned int u4PhyAddr;
};
#ifdef __KERNEL__

#define ENABLE_FB_LOG

#include "panel.h"

#ifdef LINUX_TURNKEY_SOLUTION /* MHF Linux: CHUN for res*/
#define MAX_X_RES		1280
#define MAX_Y_RES		720
#else
#define MAX_X_RES		960
#define MAX_Y_RES		540
#endif
#define MAX_PIXEL_BYTES	4

#define PMX_X_OFFSET	((TV_WIDTH - MAX_X_RES) >> 1)
#define PMX_Y_OFFSET     ((TV_HEIGHT - MAX_Y_RES) >> 1)

/* video dram size */
#define MAX_SIZE (MAX_X_RES * MAX_Y_RES * MAX_PIXEL_BYTES * 2)

#ifdef ENABLE_FB_LOG
#define FB_LOG(msg) fprint(stderr, "[MT53_FB] %s", msg);
#else
#define FB_LOG(msg)
#endif

union palette
{
	u32 color;
	u8 blue, green, red, alpha;
};

struct iospace
{
	volatile u32 __iomem *vbase;
	u32 pbase;
	u32 len;
};

struct mt53fb_par
{
	struct device	*dev;
	/* total dram size allocated when startup */
	unsigned int	dram_size;
	/* single osd region memory base */
	struct
	{
		u8 __iomem *vbase;
		u32		pbase;
		u32     len;
	} mem;
	/* frame buffer hardware io space */
	struct iospace osdio;
	/* graphic accelerator engine io space */
	struct iospace gfxio;
	/* palette */
	union palette pal[256];
	u32 pseudo_palette[256];
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
    E_GFXLIB_STRETCH_ERR,
    
    E_GFXLIB_SET_X_ERR,
    E_GFXLIB_SET_Y_ERR,
    E_GFXLIB_SET_WIDTH_ERR,
    E_GFXLIB_SET_HEIGHT_ERR,
    E_GFXLIB_SET_PITCH_ERR,
    E_GFXLIB_SET_BPP_ERR,        
    E_GFXLIB_SET_ADDR_ERR,
    E_GFXLIB_SET_COLORMODE_ERR,
    E_GFXLIB_SET_ENABLE_ERR,
    E_GFXLIB_SET_ChangeResolution_ERR,
    
    E_GFXLIB_ERR_MAX
} E_GFXLIB_T;
EXTERN INT32 _mt53fb_imageblit_wrp(void* pvArg);

#define MW_GL_OP_FILL_RECT ((unsigned long)0x1)
#define MW_GL_OP_SET_PLANE ((unsigned long)0x2)
#define MW_GL_OP_GET_FBM_BITMAP_INFO ((unsigned long)0x3)
#define MW_GL_OP_ENABLE_SCALER          ((unsigned long)0x4)

typedef struct mw_gl_op_t_
{
    unsigned long u4Op;
    union 
    {
        struct 
        {
            unsigned    e_mode;
            void*       pv_bits;
            unsigned    ui4_pitch;
            int         i4_x;
            int         i4_y;
            unsigned    ui4_width;
            unsigned    ui4_height;
            unsigned    ui4_color;
        }fill_rect;
        struct
        {
            int         i4_idx;
            int         i4_enable;
        }set_plane;
        struct
        {
            unsigned int         ui4_size;
            void*         pv_bits;
        }get_fbm_bitmap_info;        
        struct
        {
            unsigned int    ui4_id;
            unsigned int    ui4_enable;
            unsigned int    ui4_width;
            unsigned int    ui4_height;
        } enable_scaler;
    }u;
}mw_gl_op_t;

#if 1 //def CC_DFB_SUPPORT_VDP_LAYER
struct mt53fb_vdp_init
{
    unsigned int width;
    unsigned int height;
};

struct mt53fb_vdp_flip
{
    unsigned char ucFbgId;
	unsigned int  ui4_pitch;
    unsigned char* pu1SrcYAddr;
    unsigned char* pu1SrcCAddr;
    unsigned char fgIs3DMode; 
    unsigned int u4CropBottomOffset;
    unsigned int u4CropTopOffset;
    unsigned int u4CropLeftOffset;
    unsigned int u4CropRightOffset;       
};

struct mt53fb_vdp_set
{
    unsigned char ucEnable; 
};

typedef struct _mt53fb_vdp_crop_rect
{
    unsigned int                ui4_layer;
	unsigned int                ui4_3d_mode;
    unsigned int                crop_left_offset;       /* pixel width */
    unsigned int                crop_right_offset;      /* pixel height */    
    unsigned int                crop_top_offset;        /* pixel width */
    unsigned int                crop_bottom_offset;     /* pixel height */                                                                                                                      
}mt53fb_vdp_crop_rect;




#endif

//---------------------------------------------------------------------------
// Macro
//---------------------------------------------------------------------------
#define PITCH_VALID(p) (((p & 0xF)==0)?1:0)
#define ADDR_VALID(p, fbp) (((p>=fbp->fix.smem_start)&&(p<fbp->fix.smem_start+fbp->fix.smem_len))?1:0)

#endif /* ifdef __KERNEL__ */

#define MT53FB_LOG				(0x0)

#endif /*__MT53_FB__*/
