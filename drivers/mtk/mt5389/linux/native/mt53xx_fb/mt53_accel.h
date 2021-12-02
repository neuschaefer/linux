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

#define  DFB_GFX_FLUSH_INVALID_DCACHE           0x1
#define  DFB_GFX_INVALID_DCACHE                 0x2

typedef struct _MT53_CACHE_DATA 
{
    unsigned long          ui4_addr;
    unsigned long          ui4_phy;
    unsigned long          ui4_size;
    unsigned long          ui4_flush_fg;
}MT53_CACHE_DATA;

typedef struct _MT53_ACCEL_MMU_T
{
    UINT32 u4_init;                     // force init mmu reg
    UINT32 u4_enable;                   // para: of mmu enable/disable  if do init.
    UINT32 u4_cmprss;                   // set  compress to dst
    UINT32 u4_src_rw_mmu;               // set  src mmu enable/disable
    UINT32 u4_dst_rw_mmu;               // set  dst mmu enable/disable
    UINT32 u4_vgfx_ord;                 // set  vgfx overread 
    UINT32 u4_vgfx_slva;             // para:  vgfx src last valid address
    UINT32 u4_pgt;                        // set  pate table address    
} MT53_ACCEL_MMU_T;

typedef struct _EX_DeviceData 
{
    MT53_CACHE_DATA             cache;
}EX_DeviceData;

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
    int 			i4Rop;
   
    int			i4Bpp;    
};

struct mt53fb_copyarearop 
{
	u32 dx;
	u32 dy;
	u32 width;
	u32 height;
	u32 sx;
	u32 sy;
	u32 rop;	
	u32 alpha;
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


struct mt53fb_StretchBlit
{
    unsigned int u4Src;
    unsigned int u4Dst;
    unsigned int eSrcCm;
    unsigned int eDstCm;   
    unsigned int u4SrcPitch;
    unsigned int u4DstPitch;    
    unsigned int u4SrcX;
    unsigned int u4SrcY;
    unsigned int u4SrcWidth;    
    unsigned int u4SrcHeight;        
    unsigned int u4DstX;       
    unsigned int u4DstY;
    unsigned int u4DstWidth;
    unsigned int u4DstHeight;        

    unsigned int u4ClipX;       
    unsigned int u4ClipY;
    unsigned int u4ClipWidth;
    unsigned int u4ClipHeight;    
 
    unsigned int u4Ar;    
    unsigned int u4Porterdu;    
        
};  

#ifdef CONFIG_OSD_COMPRESSION
struct mt53fb_compress
{
    unsigned int u4Src;
    unsigned int u4Dst;
    unsigned int eCm; 
    unsigned int u4Pitch;
    unsigned int u4SrcX;
    unsigned int u4SrcY;
    unsigned int u4Width;    
    unsigned int u4Height;        
    unsigned int u4DstX;       
    unsigned int u4DstY;  
	MT53_ACCEL_MMU_T mmu;
};  
#endif

#if 1 //def CC_DFB_SUPPORT_VDP_LAYER
#define MT53FB_VDP_ROTATE_90 0 
#define MT53FB_VDP_ROTATE_270 1
#define MT53FB_VDP_ROTATE_180 2

struct mt53fb_vdp_rotate
{
    unsigned int u4Src;
    unsigned int u4Dst;
    unsigned int eSrcCm;
    unsigned int eDstCm;       
    unsigned int u4SrcPitch;
    unsigned int u4DstPitch;    
    unsigned int u4SrcX;
    unsigned int u4SrcY;
    unsigned int u4Width;    
    unsigned int u4Height;        
    unsigned int u4DstX;       
    unsigned int u4DstY;
    unsigned int u4RotateOp;
    unsigned int u4SrcCbCrOffset;
    unsigned int u4DstCbCrOffset;    
};  

typedef struct _mt53fb_vdp_stretch_blit_t
{
    unsigned int src1_buffer_Y_addr;
    unsigned int src1_buffer_C_addr;
    unsigned int src1_buffer_Y_width;
    unsigned int src1_buffer_Y_height;       
    unsigned int src1_buffer_Y_pitch;
    unsigned int src2_buffer_Y_addr;    
    unsigned int src2_buffer_C_addr;
    unsigned int dest_buffer_Y_addr;
    unsigned int dest_buffer_C_addr;    
    unsigned int dest_buffer_Y_width;        
    unsigned int dest_buffer_Y_height;       
    unsigned int dest_buffer_Y_pitch;  
	unsigned int composition_level;
}mt53fb_vdp_stretch_blit;	

#endif

struct mt53fb_set 
{
    struct mt53fb_setting rSetting;
    u32 mask;
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

typedef struct _IOMMU_PIN_RANGE_T
{
    unsigned long start;
    unsigned long size;
    unsigned long pageArray;
    unsigned long nrPages;
} IOMMU_PIN_RANGE_T;

EXTERN HANDLE_T hMt53fbMutex;
EXTERN struct fb_info* mt53fb_get_fb_info(void);
EXTERN struct mt53fb_par* mt53fb_get_par(void);
EXTERN INT32 _mt53fb_vgfx_scaler_ioctl(void* pvArg);
EXTERN INT32 mt53fb_flush_ex_action(void* pvArg);
EXTERN INT32 _mt53fb_vgfx_ioctl(void *pvArg);

#ifdef CC_DFB_SUPPORT_VDP_LAYER
EXTERN INT32 mt53fb_vdp_stretchblit(mt53fb_vdp_stretch_blit * pr_vdp_stretch_blit);
EXTERN INT32 _mt53fb_vdp_rotate_ioctl(void* pvArg);
#endif
EXTERN int _MTMMU_GetPhyPgd(unsigned long arg);
EXTERN int _MTMMU_PinMem(unsigned long arg);
EXTERN int _MTMMU_PinMem(unsigned long arg);
EXTERN int _MTMMU_UnPinMem(unsigned long arg);
EXTERN int _MTMMU_DMA_MAP_SINGLE(unsigned long arg);
EXTERN int _MTMMU_DMA_UNMAP_SINGLE(unsigned long arg);

EXTERN INT32 mt53fb_get_pgt_ioctl(unsigned long pvArg);
EXTERN INT32 mt53fb_pin_mem_ioctl(unsigned long pvArg);
EXTERN INT32 mt53fb_unpin_mem_ioctl(unsigned long pvArg);
EXTERN INT32 mt53fb_dma_map_single_ioctl(unsigned long pvArg);
EXTERN INT32 mt53fb_dma_unmap_single_ioctl(unsigned long pvArg);


