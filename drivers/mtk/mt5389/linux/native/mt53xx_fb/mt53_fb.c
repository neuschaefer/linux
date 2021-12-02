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
#include <linux/tty.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
#include <linux/platform_device.h>
#else
#include <linux/platform.h>
#endif
#include <asm/cacheflush.h>
//#include <asm/io.h>
// #include <asm/irq.h>
//#include <asm/pgtable.h>
//#include <asm/system.h>
//#include <asm/uaccess.h>

#ifdef CONFIG_MTRR
#include <asm/mtrr.h>
#endif

//#include "osd_if.h"
#include "osd_drvif.h"
#include "gfx_if.h"
#include "pmx_if.h"
#include "pmx_drvif.h"
#include "fbm_drvif.h"
#include "u_handle.h"
#include "img_lib_if.h"
#include "mt53_fb.h"
#include "mt53_accel.h"
#ifdef CC_DFB_SUPPORT_VDP_LAYER
#include "vdo_if.h"
#endif
//#ifdef CONFIG_OSD_COMPRESSION
#include "extmjc_if.h"
//#endif
#include "x_linux.h"
#include "x_debug.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "vdp_if.h"
#endif          

#include "u_drv_cust.h"

#ifdef CC_DFB_SUPPORT_VDP_LAYER
#include "drv_tdtv_drvif.h"
#endif

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#ifndef CC_MT5395
#define ANDROID_FB_CM_ARGB8888
#endif

//#define ANDROID
#ifdef ANDROID
#include "mtkfb.h"
#else
// Define this option if you would like to use 2 OSD plane
#define TWO_OSD_PLANE
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
#define OSD_PLANE1 OSD_PLANE_2
#define OSD_PLANE2 OSD_PLANE_3
#define OSD_SCALER1 OSD_SCALER_2
#define OSD_SCALER2 OSD_SCALER_3
#else
#define OSD_PLANE1 OSD_PLANE_1
#define OSD_PLANE2 OSD_PLANE_2
#define OSD_SCALER1 OSD_SCALER_2
#define OSD_SCALER2 OSD_SCALER_2
#endif

#define MT53XX_FB_VERSION "0.1"
#define PALETTE_SIZE 1024
#define TEXT_SIZE 1024

//#define KERNEL_DRAM_SIZE (num_physpages << PAGE_SHIFT)

// Define this option if you want OSD scaling to full screen
#define OSD_PLANE1_SCALE_TO_FULLSCREEN
#define OSD_PLANE2_SCALE_TO_FULLSCREEN

#define SIZE_PLANE_ORDER_ARRAY 4
#define MIXER_PORT_VIDEO_MAIN 0
#define MIXER_PORT_VIDEO_PIP  1
#define MIXER_PORT_OSD_1 2
#define MIXER_PORT_OSD_2 3
#define MIXER_PORT_NONE 0xFF

#ifdef LINUX_TURNKEY_SOLUTION
/* MHF Linux: CHUN */
#define OSD_BUFFER_SIZE (4 * 1024 * 1024)
#define CC_SAMSUNG_IMAGE_BUFFER_SIZE ((u32)(0xE00000 + 0x400000))

#define FB_HARD_FADING
#endif

#define TMPBUF_WIDTH  1280
#define TMPBUF_HEIGHT 720
#define TMPBUF_BPP    4

#ifdef CC_TDTV_3D_OSD_ENABLE
extern BOOL gfgOsd_Sbs;
extern BOOL gfgOsd_Tbb;
extern UINT8 _u13DOsdShift;
#endif
extern void mtk_pmem_set_memory_regions(u32 start, u32 size);

//extern INT32 OSD_Set_DirectFBMemory(UINT32 mem);

//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------
#ifndef LINUX_TURNKEY_SOLUTION
static int _mt53fb_soft_cursor(struct fb_info *info, struct fb_cursor *cursor);
#endif

static int _mt53fb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
                             u_int trans, struct fb_info *info);

static int _mt53fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info);

static int _mt53fb_set_par(struct fb_info *info);

static int _mt53fb_open(struct fb_info *info, int user);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
static int _mt53fb_mmap(struct fb_info *info, struct vm_area_struct *vma);

static int _mt53fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg);
#else
static int  _mt53fb_mmap(struct fb_info *info, struct file *file, struct vm_area_struct *vma);

static int _mt53fb_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg, struct fb_info *info);
#endif

static void __init _mt53fb_parse_colormode(char *opt);

static void __init _mt53fb_parse_dram(char *opt);

static void __init _mt53fb_init_fbinfo(void);

static int __init _mt53fb_probe(struct device *dev);

static int _mt53fb_set_ioctl(void* pvArg);

static int _mt53fb_init_hw(void* pvArg);

//static int _RecalculateOSD(void);

static int _mt53fb_palette_ioctl(void* pvArg);

static int _mt53fb_drawtext_ioctl(void* pvArg);


static int _mt53fb_get_imagebuffer_ioctl(void* pvArg);

static int _mt53fb_get_framebuffer_ioctl(void* pvArg);
#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB 
static int _mt53fb_get_imagebuffer2_ioctl(void* pvArg);
#endif
#ifdef CC_DFB_SUPPORT_VDP_LAYER
extern UINT32 VDP_SetInput(UCHAR ucVdpId, UCHAR ucEsId, UCHAR ucPort);

static int _mt53fb_vdp_set_ioctl(void* pvArg);
static int _mt53fb_get_vdpbuffer_ioctl(void* pvArg);
static int _mt53fb_get_vdp_crop_rect_ioctl(void* pvArg);
static int _mt53fb_set_vdp_crop_rect_ioctl(void* pvArg);
static INT32 _mt53fb_vdp_stretch_blit_ioctl(void* pvArg);
#endif
static int _mt53fb_get_plane_order_array_ioctl(void* pvArg);

static int _mt53fb_set_plane_order_array_ioctl(void* pvArg);

static int _mt53fb_set_osd_plane_order_ioctl(void* pvArg);

static int _mt53fb_get_ioctl(void* pvArg);

//#ifdef LINUX_TURNKEY_SOLUTION /* MHF Linux - yjg 9x mmp */
unsigned int fb_convert_app_to_phy(unsigned int usr_addr);
//#endif

int _mt53fb_wait_logo(void* pvArg);

int __init _mt53fb_setup(char *options);

int  _mt53fb_get_tmpbufaddr(void);

extern INT32 mt53fb_drawline_ioctl(void * pvArg);

extern INT32 mt53fb_drawlinerop_ioctl(void * pvArg);

extern INT32 mt53fb_drawrect_ioctl(void * pvArg);

extern INT32 mt53fb_drawrectrop_ioctl(void * pvArg);

extern INT32 mt53fb_fillrect_ioctl(void * pvArg);

extern INT32 mt53fb_copyarea_ioctl(void * pvArg);

extern INT32 mt53fb_imageblit_ioctl(void * pvArg);

extern INT32 mt53fb_imageblitrop_ioctl(void * pvArg);

extern INT32 mt53fb_fillrectrop_ioctl(void * pvArg);

extern INT32 mt53fb_copyarearop_ioctl(void * pvArg);

extern INT32 _mt53fb_gfx_stretch_ioctl(void * pvArg);

extern INT32 _mt53fb_imgrz_stretch_alcom_ioctl(void * pvArg);
extern INT32 _mt53fb_vgfx_scaler_ioctl(void* pvArg);
extern INT32 _mt53fb_vgfx_ioctl(void *pvArg);

extern INT32 mt53fb_drawtext(struct fb_info *info, const struct mt53fb_DrawText *prDrawText);
//---------------------------------------------------------------------------
extern INT32 PDWNC_Init(void);
extern void VDEC_Init(void);
extern BOOL VdoMLInit(void);
extern void SIF_Init(void);
extern void RTC_Init(void);
extern void vApiPanelPowerSequence(BOOL fgEnable);
extern INT32 GPIO_Init(void);
extern INT16 EEPROM_Init(VOID);

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

HANDLE_T hMt53fbMutex;

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#if 1 /* MHF Linux - yjg 9x mmp */
static int fb_user_virt_start = 0;
#endif
static struct device _mt53fb_dev;

static struct mt53fb_par _cur_par;
static struct fb_info    _fb_info;
static struct mt53fb_imagebuffer _rImgBufInfo; 

#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB 
static struct mt53fb_imagebuffer _rImgBufInfo2;
#endif
#ifdef CC_DFB_SUPPORT_VDP_LAYER
#define DFB_VDP_LAYER_MAX       0x5
mt53fb_vdp_crop_rect mt53fb_vdp_crop_rect_array[DFB_VDP_LAYER_MAX];
struct mt53fb_imagebuffer _rVdpBufInfo;    
#endif


#ifdef LINUX_TURNKEY_SOLUTION
/* MHF Linux CHUN  */
#define NULL_REGION_IDX 0xffffffff

static UINT32 _u4Region = NULL_REGION_IDX;
static UINT32 _u4RegionList = NULL_REGION_IDX;

static UINT32 _u4Region2 = NULL_REGION_IDX;
static UINT32 _u4RegionList2 = NULL_REGION_IDX;
#else
static UINT32 _u4Region;
static UINT32 _u4RegionList;

static UINT32 _u4Region2;
static UINT32 _u4RegionList2;
#endif

static UINT32 _au4PlaneOrderArray[PMX_MAX_INPORT_NS] = 
{
    PMX_OSD2, 
    PMX_OSD3, 
    PMX_OSD1,
    PMX_PIP,
    PMX_MAIN
}; 

static UINT8 _au1Log2HwPlane[SIZE_PLANE_ORDER_ARRAY] =
{
    PMX_MAIN,
    PMX_PIP,   
#ifdef CC_MT5360
    PMX_OSD1,    	    
    PMX_OSD2    	        
#else
    PMX_OSD2,    	    
    PMX_OSD3    	        
#endif
};

#ifndef LINUX_TURNKEY_SOLUTION
static UINT8 _au1Hw2LogPlane[PMX_MAX_INPORT_NS] =
{
    MIXER_PORT_VIDEO_MAIN,
    MIXER_PORT_VIDEO_PIP,   
#ifdef CC_MT5360
    MIXER_PORT_OSD_1,    	    
    MIXER_PORT_OSD_2    	        
#else
    MIXER_PORT_NONE,    	    
    MIXER_PORT_OSD_1,    	    
    MIXER_PORT_OSD_2
#endif
}; 
#endif

static struct fb_ops _mt53fb_ops =
{
    .owner          = THIS_MODULE,
    .fb_open = _mt53fb_open,
    .fb_check_var   = _mt53fb_check_var,
    .fb_set_par     = _mt53fb_set_par,              // return -EINVAL
    .fb_setcolreg   = _mt53fb_setcolreg,            // return 0
    //.fb_pan_display   = _mt53fb_pan_display,
    .fb_fillrect    = cfb_fillrect,
    .fb_copyarea    = cfb_copyarea,
    .fb_imageblit   = cfb_imageblit,
    .fb_mmap        = _mt53fb_mmap,
#ifdef LINUX_TURNKEY_SOLUTION
//    .fb_cursor      = soft_cursor,
#else
    .fb_cursor      = _mt53fb_soft_cursor,
#endif
    .fb_ioctl       = _mt53fb_ioctl,
};

/*
static struct fb_videomode modedb[] __initdata =
{
    // 960x540 @ 60Hz, x kHz hsync
    {NULL, 60,  960,  540,  27778, 101,  23,  22,  1, 100, 2, 0, FB_VMODE_NONINTERLACED},
    // 1280x720 @ 60Hz, x kHz hsync
    {NULL, 60, 1280,  720,   9090, 186,  96,  38,  1, 160, 3, 0, FB_VMODE_NONINTERLACED}
};


static struct fb_videomode __initdata _mt53fb_default_mode =
{
    .name           = NULL,
    .refresh        = 60,
    .xres           = 960,
    .yres           = 540,
    .pixclock       = 27778,
    .left_margin    = 101,
    .right_margin   = 23,
    .upper_margin   = 22,
    .lower_margin   = 1,
    .hsync_len      = 100,
    .vsync_len      = 2,
    .sync           = 0,
    .vmode          = FB_VMODE_NONINTERLACED
};
*/

static struct options
{
    char *name;
    void (*parse)(char *opt);
} _mt53fb_opt_table[] __initdata =
    {
        { "colormode",  _mt53fb_parse_colormode  },
        { "dram",       _mt53fb_parse_dram       },
        { NULL, NULL }
    };

/*static struct device_driver _mt53fb_driver =
{
    .name   = "mt53fb",
    .bus    = &platform_bus_type,
    .probe  = _mt53fb_probe,
};*/

/*
static u64 _mt53fb_dmamask = ~(u32)0;
*/

/* static struct platform_device _mt53fb_device =
{
    .name   = "mt53fb",
    .id     = 0,
    .dev    =
    {
        .dma_mask           = &_mt53fb_dmamask,
        .coherent_dma_mask  = 0xffffffff,
    },
    .num_resources = 0,
}; */

static unsigned int * _apu4Palette[2];

static unsigned int * _apu4PaletteVirt[2];

static unsigned char* _pu1TextArray = NULL;

static unsigned char* _pu1TextArrayVirt = NULL;

static volatile GFX_CMDQUE_T *_prShmCmdQue = NULL;

static volatile UINT64 *_pu8CmdQueBuf = NULL;

UINT32 _u4TmpBufAddr = 0;
static BOOL _fgOsdScEnable[2] = {TRUE, TRUE};

int osd1_src_width = 0;
int osd1_src_height = 0;
int osd1_output_width = 0;
int osd1_output_height = 0;
int osd2_src_width = 0;
int osd2_src_height = 0;
int osd2_output_width = 0;
int osd2_output_height = 0;

#ifdef ANDROID
#define CHECK_FAIL(stmt, exp_ret) \
do {\
    int _____ret = (stmt);\
    if(_____ret != (exp_ret))\
    {\
        printk("CHECK FAILED %s ret:%d\n", #stmt, _____ret);\
    }\
}while(0)
#define ANDROID_OSD OSD_PLANE_3
#define ANDROID_OSD_SCALER OSD_SCALER2

#define ANDROID_NUMBER_OF_BUFFERS 2
#ifdef ANDROID_FB_CM_ARGB8888
#define ANDROID_BITS_PER_PIXEL 32
#else
#define ANDROID_BITS_PER_PIXEL 16
#endif
/* Modify these macros to suit the hardware */

//static int panel_id = 0;
static uint width = 320;
static uint height = 480;

static mtk_drv_fb_t * drv;
struct mtk_fb *fb;
struct mtk_fb {
    int     rotation;
    struct  fb_info fb;
    u32     cmap[16];
};

static inline u32 convert_bitfield(int val, struct fb_bitfield *bf)
{
    unsigned int mask = (1 << bf->length) - 1;

    return (val >> (16 - bf->length) & mask) << bf->offset;
}


/* set the software color map.  Probably doesn't need modifying. */
static int
mtk_fb_setcolreg(unsigned int regno, unsigned int red, unsigned int green,
         unsigned int blue, unsigned int transp, struct fb_info *info)
{
        struct mtk_fb  *fb = container_of(info, struct mtk_fb, fb);

    if (regno < 16) {
        fb->cmap[regno] = convert_bitfield(transp, &fb->fb.var.transp) |
                  convert_bitfield(blue, &fb->fb.var.blue) |
                  convert_bitfield(green, &fb->fb.var.green) |
                  convert_bitfield(red, &fb->fb.var.red);
        return 0;
    }
    else {
        return 1;
    }
}

/* check var to see if supported by this device.  Probably doesn't
 * need modifying.
 */
static int mtk_fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    if((var->rotate & 1) != (info->var.rotate & 1)) {
        if((var->xres != info->var.yres) ||
           (var->yres != info->var.xres) ||
           (var->xres_virtual != info->var.yres) ||
           (var->yres_virtual > 
            info->var.xres * ANDROID_NUMBER_OF_BUFFERS) ||
           (var->yres_virtual < info->var.xres )) {
            return -EINVAL;
        }
    }
    else {
        if((var->xres != info->var.xres) ||
           (var->yres != info->var.yres) ||
           (var->xres_virtual != info->var.xres) ||
           (var->yres_virtual > 
            info->var.yres * ANDROID_NUMBER_OF_BUFFERS) ||
           (var->yres_virtual < info->var.yres )) {
            return -EINVAL;
        }
    }
    if((var->xoffset != info->var.xoffset) ||
       (var->bits_per_pixel != info->var.bits_per_pixel) ||
       (var->grayscale != info->var.grayscale)) {
        return -EINVAL;
    }
    return 0;
}


/* Handles screen rotation if device supports it. */
static int mtk_fb_set_par(struct fb_info *info)
{
    struct mtk_fb *fb = container_of(info, struct mtk_fb, fb);
    if(fb->rotation != fb->fb.var.rotate) {
        info->fix.line_length = drv->get_line_length(info->var.xres, ANDROID_BITS_PER_PIXEL);
        fb->rotation = fb->fb.var.rotate;
    }
    return 0;
}


/* Pan the display if device supports it. */
static int mtk_fb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
    struct mtk_fb *fb    __attribute__ ((unused)) 
        = container_of(info, struct mtk_fb, fb);
    drv->set_base(fb->fb.fix.smem_start + fb->fb.var.xres * 
       (ANDROID_BITS_PER_PIXEL >> 3) * var->yoffset);

    return 0;
}

static UINT32 rgn = (UINT32)NULL;
static UINT32 rgn_list = (UINT32)NULL;
static UINT32 rgn_2 = (UINT32)NULL;
static UINT32 rgn_list_2 = (UINT32)NULL;
//static BOOL _fgAndroidFBFlip = FALSE;

static int _mtk_fb_set_ioctl(void* pvArg)
{
    int i4Ret = 0;
    unsigned short u2PanelWidth;
    unsigned short u2PanelHeight;
    UINT32 u4Region = rgn;
    
    struct mt53fb_set rSet;
	UNUSED(u2PanelWidth);
	UNUSED(u2PanelHeight);
    if(copy_from_user(&rSet, pvArg, sizeof(struct mt53fb_set)))
    {
        printk(KERN_WARNING "_mt53fb_set_ioctl() - copy_from_user() fail\n");
        return -EFAULT;
    }
    
    if(rSet.mask & MT53FB_SET_MASK_OPACITY)
    {
#ifdef FB_HARD_FADING        
       /*CHUN: We don't allow DFB wm change Window layer opacity. FIXME*/
       
  //      if(rSet.rSetting.u4OsdPlaneID != 0)        //for DTV00084581
        {       
#endif
    #ifdef LINUX_TURNKEY_SOLUTION
            if(OSD_PLA_SetFading(ANDROID_OSD, rSet.rSetting.u4Opacity) != OSD_RET_OK)
    #else
            if(OSD_PLA_SetFading(_au1Log2HwPlane[rSet.rSetting.u4OsdPlaneID + 2] - 2, rSet.rSetting.u4Opacity) != OSD_RET_OK)
    #endif
            {
                return -EINVAL;
            }
#ifdef FB_HARD_FADING                
        }
#endif
    }  
    if(rSet.mask & MT53FB_SET_MASK_COLORKEYEN)
    {    
        if(OSD_RGN_Set(u4Region, OSD_RGN_COLOR_KEY_EN, rSet.rSetting.u4ColorKeyEn) != OSD_RET_OK ||
           (rgn_2 && OSD_RGN_Set(rgn_2, OSD_RGN_COLOR_KEY_EN, rSet.rSetting.u4ColorKeyEn) != OSD_RET_OK))
        {
            return -EINVAL;
        }
    }
    
    if(rSet.mask & MT53FB_SET_MASK_COLORKEY)
    {   
        if(OSD_RGN_Set(u4Region, OSD_RGN_COLOR_KEY, rSet.rSetting.u4ColorKey) != OSD_RET_OK ||
           (rgn_2 && OSD_RGN_Set(rgn_2, OSD_RGN_COLOR_KEY, rSet.rSetting.u4ColorKey) != OSD_RET_OK))
        {
            return -EINVAL;
        }
    }       

    return i4Ret;
}

static int _mtk_fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
{
    void __user *pvArg = (void __user *)arg;
    INT32 i4Ret = 0;
    switch(cmd)
    {
        case FBIO_SET:
            i4Ret = _mtk_fb_set_ioctl(pvArg);
            break;
	
        default:
            //printk(KERN_WARNING "_mt53fb_ioctl(): not support ioctl cmd %d\n", cmd);            
            i4Ret = -EINVAL;
            break;
    }
    return i4Ret;
}

static struct fb_ops mtk_fb_ops = {
    .owner          = THIS_MODULE,
    .fb_check_var   = mtk_fb_check_var,
    .fb_set_par     = mtk_fb_set_par,
    .fb_setcolreg   = mtk_fb_setcolreg,
    .fb_pan_display = mtk_fb_pan_display,

    /* These are generic software based fb functions */
    .fb_fillrect    = cfb_fillrect,
    .fb_copyarea    = cfb_copyarea,
    .fb_imageblit   = cfb_imageblit,
    .fb_ioctl       = _mtk_fb_ioctl,
};


static int mtk_fb_setup(void)
{
    int ret;

    printk("MTK FB for Android mtk_fb_probe\n");
    fb = kzalloc(sizeof(*fb), GFP_KERNEL);
    if(fb == NULL) {
        ret = -ENOMEM;
        goto err_fb_alloc_failed;
    }
    
    width = ((osd2_src_width==0)?width:osd2_src_width);   
    height = ((osd2_src_height==0)?height:osd2_src_height);   
 
    fb->fb.fbops            = &mtk_fb_ops;

    /* These modes are the ones currently required by Android */

    fb->fb.flags            = FBINFO_FLAG_DEFAULT;
    fb->fb.pseudo_palette   = fb->cmap;
    fb->fb.fix.type         = FB_TYPE_PACKED_PIXELS;
    fb->fb.fix.visual       = FB_VISUAL_TRUECOLOR;
    fb->fb.fix.line_length  = drv->get_line_length(width, ANDROID_BITS_PER_PIXEL);
    fb->fb.fix.accel        = FB_ACCEL_NONE;
    fb->fb.fix.ypanstep     = 1;

    fb->fb.var.xres         = width;
    fb->fb.var.yres         = height;
    fb->fb.var.xres_virtual = width;
    fb->fb.var.yres_virtual = height * ANDROID_NUMBER_OF_BUFFERS;
    fb->fb.var.bits_per_pixel   = ANDROID_BITS_PER_PIXEL;
    fb->fb.var.activate     = FB_ACTIVATE_NOW;
    fb->fb.var.height       = height;
    fb->fb.var.width        = width;

#ifdef ANDROID_FB_CM_ARGB8888
    fb->fb.var.red.offset   = 16;
    fb->fb.var.red.length   = 8;
    fb->fb.var.green.offset = 8;
    fb->fb.var.green.length = 8;
    fb->fb.var.blue.offset  = 0;
    fb->fb.var.blue.length  = 8;
    fb->fb.var.transp.offset = 24;
    fb->fb.var.transp.length = 8;   

    ret = drv->setup_buffer(
                width,
                height,
                fb->fb.var.yres_virtual,
                ANDROID_BITS_PER_PIXEL,
                MTK_DRV_FB_CM_ARGB8888, 
                (void **)&fb->fb.screen_base,
                (unsigned long **)&fb->fb.fix.smem_start,
                (unsigned long **)&fb->fb.fix.smem_len);
#else
    fb->fb.var.red.offset   = 11;
    fb->fb.var.red.length   = 5;
    fb->fb.var.green.offset = 5;
    fb->fb.var.green.length = 6;
    fb->fb.var.blue.offset  = 0;
    fb->fb.var.blue.length  = 5;
  
    ret = drv->setup_buffer(
                width,
                height,
                fb->fb.var.yres_virtual,
                ANDROID_BITS_PER_PIXEL,
                MTK_DRV_FB_CM_RGB565, 
                (void **)&fb->fb.screen_base,
                (unsigned long **)&fb->fb.fix.smem_start,
                (unsigned long **)&fb->fb.fix.smem_len);
#endif

    if(ret)
        goto err_fb_drv_failed;


    ret = fb_set_var(&fb->fb, &fb->fb.var);
    if(ret)
        goto err_fb_set_var_failed;


    ret = register_framebuffer(&fb->fb);
    if(ret)
        goto err_register_framebuffer_failed;
printk("MTK FB for Android probe successful\n");
    return 0;


err_register_framebuffer_failed:
err_fb_set_var_failed:
err_fb_drv_failed:
    kfree(fb);
err_fb_alloc_failed:
    printk("MTK FB for Android probe failed\n");
    return ret;
}

static void _fb_exit(void)
{
    /* TODO:  Release all resources .... */
	return;
}

static u32 _get_line_length(u32 w, u32 bits_per_pix)
{
    /* Aligned on 16 bytes */
    return (w * bits_per_pix/8 + 0xf) & ~0xf;
}

static UINT32 _get_drv_cm(u32 fb_cm)
{
    switch(fb_cm)
    {
        case MTK_DRV_FB_CM_RGB565:
            return OSD_CM_RGB565_DIRECT16;
        case MTK_DRV_FB_CM_ARGB8888:
            return OSD_CM_ARGB8888_DIRECT32;                
        default:
            printk("NOT SUPPORT CM\n");
    }

	return -1;
}

static int _setup_buffer(
        u32 xres, 
        u32 yres, 
        u32 vyres,
        u32 bits_per_pix, 
        u32 cm,
        void ** p_screen_base,
        unsigned long ** p_phy_start,
        unsigned long ** p_size)
{
    UINT32 pw;
    UINT32 ph;

    *p_phy_start =(unsigned long *) ((BSP_GetFbmMemAddr() - DRVCUST_OptGet((UINT32)eDirectFBMemSize) - DRVCUST_OptGet((UINT32)eFBMemSize)) & PAGE_MASK);
    /* We Don't need too much memory as DTV mw.*/    
    //*p_size = xres * bits_per_pix * vyres / 8;
    *p_size =(unsigned long *)( DRVCUST_OptGet((UINT32)eFBMemSize));
    /* USE DTV-FBM convertion or use ioremap, check whether the core.c mapped 
     * FB memory when system init.
     * */
    *p_screen_base = ioremap((long unsigned int)*p_phy_start, (size_t)*p_size);
    printk("mt5395 fb get buffer :scr base:0x%x, phy start:0x%x, size %d\n",
            *p_screen_base,
            *p_phy_start,
            *p_size);
#ifdef ANDROID_FB_CM_ARGB8888
    memset((void *)*p_screen_base, 0x00, (SIZE_T)*p_size);
#else
    memset((void *)*p_screen_base, 0x00, (SIZE_T)*p_size);
#endif

    CHECK_FAIL(OSD_RGN_LIST_Create(&rgn_list), 0);

    CHECK_FAIL(OSD_RGN_Create(&rgn, xres, yres, 
                      *p_phy_start, 
                      _get_drv_cm(cm), 
                      xres * bits_per_pix / 8,
                      0, 0, xres, yres), 0);
    
    CHECK_FAIL(OSD_RGN_Set(rgn, OSD_RGN_BIG_ENDIAN, 0), 0);

    CHECK_FAIL(OSD_RGN_Insert(rgn, rgn_list), 0);
    
    /* TODO: Which plane?*/
    CHECK_FAIL(OSD_PLA_FlipTo(ANDROID_OSD, rgn_list), 0);
    
    CHECK_FAIL(OSD_RGN_Set(rgn, OSD_RGN_MIX_SEL, OSD_BM_PIXEL), 0);



    CHECK_FAIL(OSD_RGN_LIST_Create(&rgn_list_2), 0);

    CHECK_FAIL(OSD_RGN_Create(&rgn_2, xres, yres, 
                      *p_phy_start, 
                      _get_drv_cm(cm), 
                      xres * bits_per_pix / 8,
                      0, 0, xres, yres), 0);
    
    CHECK_FAIL(OSD_RGN_Set(rgn_2, OSD_RGN_BIG_ENDIAN, 0), 0);

    CHECK_FAIL(OSD_RGN_Insert(rgn_2, rgn_list_2), 0);
    
    
    CHECK_FAIL(OSD_RGN_Set(rgn_2, OSD_RGN_MIX_SEL, OSD_BM_PIXEL), 0);


    pw = PANEL_GetPanelWidth();
    ph = PANEL_GetPanelHeight();

    /* TODO: Which scaler:*/
    CHECK_FAIL(OSD_SC_Scale(ANDROID_OSD_SCALER, TRUE, xres, yres, 
	(osd2_output_width==0)?pw:osd2_output_width, 
	(osd2_output_height==0)?ph:osd2_output_height), 0);

    /* */



    printk("osd2 src_w: %d, src_h: %d, out_w: %d, out_h: %d\n",
xres, yres, (osd2_output_width==0)?pw:osd2_output_width, (osd2_output_height==0)?ph:osd2_output_height); 

// removed by polar
    mtk_pmem_set_memory_regions((u32)((BSP_GetFbmMemAddr() - DRVCUST_OptGet(eDirectFBMemSize)) & PAGE_MASK), (u32)DRVCUST_OptGet(eDirectFBMemSize));
    printk("mt53_fb: 0x%08X, 0x%08X\n", (u32)((BSP_GetFbmMemAddr() - DRVCUST_OptGet(eDirectFBMemSize)) & PAGE_MASK), (u32)DRVCUST_OptGet(eDirectFBMemSize));

    return 0;
}

static int _set_base(unsigned long phy_scr_base)
{
CHECK_FAIL(OSD_RGN_Set(rgn, OSD_RGN_BMP_ADDR, phy_scr_base), 0);
    return 0;
}

static mtk_drv_fb_t mt5395_fb = 
{
    .fb_exit =          _fb_exit,
    .get_line_length =  _get_line_length,
    .setup_buffer =     _setup_buffer,
    .set_base =         _set_base,
};

int mtk_drv_fb_init(mtk_drv_fb_t ** drv)
{
#if 0
    PMX_Init();
    OSD_Init();
    GFX_Init();
    
    GPIO_Init();
    EEPROM_Init();
    FBM_Init();
    LoadPanelIndex();
    PDWNC_Init();
    VDEC_Init(); //???
    VDP_Init();
    VdoMLInit();
    SIF_Init();
    VDP_SetScalerPixelBased(1);
    RTC_Init();
    
    vApiPanelPowerSequence(1);
    OSD_Reset();
#endif    
    OSD_PLA_Enable(ANDROID_OSD, TRUE);
    /*OSD_PLA_Enable(2, TRUE);*/

    *drv = &mt5395_fb;
    return 0;
}

#endif



void dfb_malloc(UINT32 *u4PhyAddr, UINT32 *u4VirAddr, INT32 u4Size)
{
#if 1
    *u4PhyAddr = (_fb_info.fix.smem_start + _fb_info.fix.smem_len - u4Size - 0x18FF) & (~0xF);

    if((*u4PhyAddr) < _fb_info.fix.smem_start)
    {
         Printf("WARNING: buffer alloc too large \n");
    }
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
    *u4VirAddr = ioremap_cached((int)(*u4PhyAddr), u4Size);
#else
    *u4VirAddr = __ioremap((int)(*u4PhyAddr), u4Size, L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif

#else
    u4Size = 0;
    *u4PhyAddr = _rImgBufInfo.u4PhyAddr;
    *u4VirAddr = _rImgBufInfo.u4VirtAddr;
#endif

    Printf("dfb_malloc [%p][%p][%d] \n", *u4PhyAddr, *u4VirAddr, u4Size);
}


void dfb_free(UINT32 *u4PhyAddr, UINT32 *u4VirAddr)
{
#if 1
    *u4PhyAddr = 0;
    iounmap((*u4VirAddr));
#else
    *u4PhyAddr = 0;
    *u4VirAddr = 0;
#endif
}


#ifdef CONFIG_OSD_COMPRESSION
//#define MAX_OSD_COMPRESSED_BUFFER_SIZE ((((960 * 540 * 4 >> 1) - 1) >> PAGE_SHIFT << PAGE_SHIFT) + PAGE_SIZE)
#define MAX_OSD_COMPRESSED_BUFFER_SIZE ((((960 * 540 * 4 ) - 1) >> PAGE_SHIFT << PAGE_SHIFT) + PAGE_SIZE)
#define NUM_OF_COMPRESSED_BUFFERS (2)
static UINT8* _apu1CompressedOSDBuf[NUM_OF_COMPRESSED_BUFFERS] = {0, 0};
static BOOL _fgCompressedOSDFlip = 0;
static UINT32 _au4CompressedRegion[NUM_OF_COMPRESSED_BUFFERS] = {NULL_REGION_IDX, NULL_REGION_IDX};
static UINT32 _au4CompressedRegionList[NUM_OF_COMPRESSED_BUFFERS] = {NULL_REGION_IDX, NULL_REGION_IDX};
extern INT32 mt53fb_compress(struct fb_info *info, const struct mt53fb_compress *prCompress);
static struct mt53fb_setting _arSavedSetting[NUM_OF_COMPRESSED_BUFFERS];
static BOOL _afgCompressEnable[NUM_OF_COMPRESSED_BUFFERS] = {FALSE, FALSE};
#elif defined(CC_CONFIG_OSD_SBS)
#define DFB_UI_WIDTH  960
#define DFB_UI_HEIGHT 540
#define MAX_OSD_SBS_BUFFER_SIZE ((((DFB_UI_WIDTH * DFB_UI_HEIGHT * 4 ) - 1) >> PAGE_SHIFT << PAGE_SHIFT) + PAGE_SIZE)
#define NUM_OSD_SBS_BUFFERS (2)
static UINT8* _apu1OSDSbsBuf[NUM_OSD_SBS_BUFFERS] = {0, 0};
static BOOL _fgOSDSbsFlip = 0;
static UINT32 _au4SbsRegion[NUM_OSD_SBS_BUFFERS] = {NULL_REGION_IDX, NULL_REGION_IDX};
static UINT32 _au4SbsRegionList[NUM_OSD_SBS_BUFFERS] = {NULL_REGION_IDX, NULL_REGION_IDX};
static struct mt53fb_setting _arSbsSetting;
#endif
#ifdef CC_DFB_SUPPORT_VDP_LAYER
static BOOL _fgVdpInit = FALSE;
static unsigned char _ucFbgId = 0;
//static unsigned char _aucFbId[2] = {FBM_FB_ID_UNKNOWN, FBM_FB_ID_UNKNOWN};
//static unsigned int _au4FbYAddr[2] = {NULL, NULL};
//static unsigned int _au4FbCAddr[2] = {NULL, NULL};
//static unsigned char _u1FrontFbId = 0;
static E_TDTV_DECODER_INPUT _ePrevDetectedMode = E_TDTV_DECODER_INPUT_2D_P; 
//static unsigned int _u4VdpWidth;
//static unsigned int _u4VdpHeight;
static UINT32 _u4CropBottomOffset = 0;
static UINT32 _u4CropTopOffset = 0;
static UINT32 _u4CropLeftOffset = 0;
static UINT32 _u4CropRightOffset = 0;
//static UCHAR	_fgLastVdpEnableFg = 0;
#endif



//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#ifdef CC_DFB_SUPPORT_VDP_LAYER
static BOOL _3DCropFuncCB(TDTV_3D_CROP_INTO_T* prCropInfo)
{
    prCropInfo->fgIsFixSrcAspectRatio = 1;
    prCropInfo->fgIsFullResolution = 1;    
    prCropInfo->u4CropBottomOffset = _u4CropBottomOffset;
    prCropInfo->u4CropTopOffset = _u4CropTopOffset;    
    prCropInfo->u4CropLeftOffset = _u4CropLeftOffset;        
    prCropInfo->u4CropRightOffset = _u4CropRightOffset;     
	return 0;
}
static BOOL _VDP_FBGInit(unsigned int u4Width, unsigned int u4Height, unsigned int u4Pitch)
{
     unsigned char ucFbgType, ucFbgId;
     FBM_SEQ_HDR_T* prFbmSeqHdr;     
     VDP_REGION_T rRegion;
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
     UNUSED(ucFbgType);
	 UNUSED(ucFbgId);
	 UNUSED(prFbmSeqHdr);
    if(!_fgVdpInit)
    {
        UCHAR ucFbgType, ucFbgId;
        FBM_SEQ_HDR_T* prFbmSeqHdr;

        // Set video src
        bApiVideoSetVideoSrc(0, SV_VS_DT1);
        VDP_SetSrcRegion(0, 1, rRegion);        
        VDP_SetOutRegion(0, 1, rRegion);  
		// VDP_GetEnable(0,&_fgLastVdpEnableFg);
        VDP_SetEnable(0, 1);        
        VDP_SetMode(0, VDP_MODE_BG);
        VDP_SetInput(0, 0, 0);    
   
        ucFbgType = FBM_SelectGroupType(u4Width, u4Height);
        FBM_SetColorMode(FBM_CM_422);
        ucFbgId = FBM_CreateGroup(ucFbgType, FBM_VDEC_JPEG, u4Width, u4Height);
        if(ucFbgId == FBM_FBG_ID_UNKNOWN)
        {
            printk("Create Fbg Fail!! input resolution %dx%d\n", u4Width, u4Height);
            x_sema_unlock(hMt53fbMutex);                 
            return ucFbgId;
        }
        FBM_SetPlayMode(ucFbgId, FBM_FBG_MM_MODE);
        FBM_SetSyncStc(ucFbgId, 0, 0); //none
        FBM_FbgChgNotify(ucFbgId, 0);

		if (u4Pitch == 0)
			u4Pitch = ((u4Width + 15) >> 4) << 4;
    
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
        prFbmSeqHdr->u2HSize = u4Width;
        prFbmSeqHdr->u2VSize = u4Height;
        prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2HSize;
        prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
        prFbmSeqHdr->u2LineSize = u4Pitch;        
        prFbmSeqHdr->fgProgressiveSeq = TRUE;
        prFbmSeqHdr->fgRasterOrder = TRUE;
        prFbmSeqHdr->fg422Mode = TRUE;
        prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;
    
        FBM_SetFrameBufferFlag(ucFbgId, FBM_FLAG_SEQ_CHG);

        // Register 3D crop CB
        fpDrvTDTVReg3DCropCB(E_TDTV_CB_3D_CROP_MM, _3DCropFuncCB);

        _ucFbgId = ucFbgId;
        _fgVdpInit = TRUE;
        
    }
     
     x_sema_unlock(hMt53fbMutex);                  
     return TRUE;
 }


static BOOL _VDP_FBFlip(unsigned char ucFbgId, 
	unsigned char* pu1SrcYAddr, unsigned char* pu1SrcCAddr, BOOL fgIs3DMode, unsigned int ui4_pitch,
	unsigned int u4CropBottomOffset, unsigned int u4CropTopOffset, unsigned int u4CropLeftOffset, unsigned int u4CropRightOffset)
{
    UINT32 u43DInOut, u2InFmt, u2OutFmt;
    FBM_SEQ_HDR_T* prFbmSeqHdr;

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);

    if(_fgVdpInit)
    {
        UCHAR ucFbId;
        UINT32 u4DstYAddr, u4DstCAddr, u4Pitch, u4TotalLine, u4W;
        E_TDTV_DECODER_INPUT eDetectedMode;
        FBM_PIC_HDR_T* prFbmPicHdr;           
        
        //Get back frame buffer
        ucFbId = FBM_GetEmptyFrameBuffer(ucFbgId, (UINT32)1500);
        if (ucFbId == (UCHAR)FBM_FB_ID_UNKNOWN)
        {
            printk("Get empty frame buffer failed.\n");
            x_sema_unlock(hMt53fbMutex);                 
            return FALSE;
        }
        
        if(MT53FB_LOG>=0x2)
        {
            printk("Get empty frame buffer id = %d.\n", ucFbId);  
        }
        
        FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4DstYAddr, &u4DstCAddr);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, ucFbId);
        if ((prFbmPicHdr == NULL) || (u4DstYAddr == 0) || (u4DstCAddr == 0))
        {
            printk("Alloc frame buffer failed.\n");
            x_sema_unlock(hMt53fbMutex);                
            return FALSE;
        }     
        prFbmPicHdr->ucPicStruct = (UCHAR)MPEG_PIC_FRAME;
        prFbmPicHdr->fgProgressiveFrm = TRUE; 
        prFbmPicHdr->fgRepFirstFld = FALSE;
        prFbmPicHdr->fgTopFldFirst = FALSE;
        FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);    //default
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_READY);
        
        //Bit blit input pu1SrcYAddr, pu1SrcCAddr to destination u4DstYAddr, u4DstCAddr
        FBM_GetFrameBufferSize(ucFbgId, &u4Pitch, &u4TotalLine);
       
        u4W = PANEL_GetPanelWidth();
        u4Pitch = ui4_pitch;
        u4TotalLine = PANEL_GetPanelHeight();

		if(MT53FB_LOG>=0x2)
		{
             printk("Alloc frame buffer Yaddr = 0x%8x, Caddr = 0x%8x, pitch = %d, width = %d, height = %d\n", 
                u4DstYAddr, u4DstCAddr, u4Pitch, u4W, u4TotalLine);   
        }           

        //Y
        GFX_Lock();
		GFX_Flush_Invalid_DCache();
        GFX_SetSrc(pu1SrcYAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst((UINT8 *)u4DstYAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_BitBlt(0, 0, 0, 0, u4W, u4TotalLine);
        //C
        GFX_SetSrc(pu1SrcCAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_SetDst((UINT8 *)u4DstCAddr, (UINT32)CM_RGB_CLUT8, u4Pitch);
        GFX_BitBlt(0, 0, 0, 0, u4W, u4TotalLine);
        GFX_Flush();
        GFX_Wait();
        GFX_Unlock();

        if(MT53FB_LOG>=0x2)
        {
            printk("DFB buffer Yaddr = 0x%8x, Caddr = 0x%8x, 3D mode = %d\n", 
                (UINT32)pu1SrcYAddr, (UINT32)pu1SrcCAddr, fgIs3DMode);     
        }

        //Check 3D Type
        if(fgIs3DMode)
        {
            eDetectedMode = E_TDTV_DECODER_INPUT_SBS_P;
        }
        else
        {
            eDetectedMode = E_TDTV_DECODER_INPUT_2D_P;
        }
        u43DInOut = u4DrvTDTVMMModeQuery(eDetectedMode);        
        if(eDetectedMode != _ePrevDetectedMode)
        {
//            u43DInOut = u4DrvTDTVMMModeQuery(eDetectedMode);
            u2InFmt = u43DInOut & 0xFFFF;
            u2OutFmt = (u43DInOut >> 16) & 0xFFFF;
            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);
            
            switch(u2OutFmt)
            {
                case E_TDTV_DECODER_OUTPUT_NATIVE:
                    prFbmSeqHdr->fgB2R3DEnable = FALSE;
                    break;
                case E_TDTV_DECODER_OUTPUT_3D_FS:
                    prFbmSeqHdr->fgB2R3DEnable = TRUE;
                    prFbmSeqHdr->fgB2RForce2D = FALSE;
                    prFbmSeqHdr->u4B2R3DWidthOffset = u4Pitch >> 1;
                    prFbmSeqHdr->u4B2R3DHeightOffset = 0;
                    prFbmSeqHdr->u1B2R3DType = B2R_3D_SIDE_BY_SIDE;
                    break;
                default:
                    break;
            }
            _ePrevDetectedMode = eDetectedMode;
            FBM_SetFrameBufferPicFlag(ucFbgId, ucFbId, FBM_MM_3D_CHG_FLAG);
        }
        
        //Display
        FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_DISPLAYQ);

        // Unmute B2R
        VDP_SetMode(0, VDP_MODE_NORMAL);

    }

    x_sema_unlock(hMt53fbMutex);    
    
    return TRUE;
    
}


static int _mt53fb_vdp_init_ioctl(void* pvArg)
{
    struct mt53fb_vdp_init rInitData;
    
    if(copy_from_user( (void*)&rInitData, pvArg, sizeof(struct mt53fb_vdp_init)))
    {
        printk(KERN_WARNING "_mt53fb_vdp_init_ioctl() - copy_from_user() fail\n");
        return -EFAULT;
    }

    if(!_VDP_FBGInit(PANEL_GetPanelWidth(), PANEL_GetPanelHeight(), 0))
    {
        printk(KERN_WARNING "_mt53fb_vdp_init_ioctl() - fbg init fail\n");
        return -EFAULT;    
    }

    return 0;
}

static int _mt53fb_vdp_flip_ioctl(void* pvArg)
{
    struct mt53fb_vdp_flip rFlipData;
    
    if(copy_from_user( (void*)&rFlipData, pvArg, sizeof(struct mt53fb_vdp_flip)))
    {
        printk(KERN_WARNING "_mt53fb_vdp_flip_ioctl() - copy_from_user() fail\n");
        return -EFAULT;
    }

    if(!_VDP_FBGInit(PANEL_GetPanelWidth(), PANEL_GetPanelHeight(), rFlipData.ui4_pitch))
    {
        printk(KERN_WARNING "_mt53fb_vdp_flip_ioctl() - FBG init fail\n");    
        return -EFAULT;    
    }
    if(!_VDP_FBFlip(_ucFbgId, rFlipData.pu1SrcYAddr, rFlipData.pu1SrcCAddr, rFlipData.fgIs3DMode, rFlipData.ui4_pitch,
    	rFlipData.u4CropBottomOffset, rFlipData.u4CropTopOffset, rFlipData.u4CropLeftOffset, rFlipData.u4CropRightOffset))
    {
        printk(KERN_WARNING "_mt53fb_vdp_flip_ioctl() - FB flip fail\n");    
        return -EFAULT;    
    }

    return 0;
}

static int _mt53fb_vdp_set_ioctl(void* pvArg)
{
    struct mt53fb_vdp_set rSetData;
    
    if(copy_from_user( (void*)&rSetData, pvArg, sizeof(struct mt53fb_vdp_set)))
    {
        printk(KERN_WARNING "_mt53fb_vdp_set_ioctl() - copy_from_user() fail\n");
        return -EFAULT;
    }

    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
      
    //layer release
    if(!rSetData.ucEnable)
    {
        VDP_SetMode(0, VDP_MODE_BG);    
        FBM_ReleaseGroup(_ucFbgId);
        FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);

        fpDrvTDTVReg3DCropCB(E_TDTV_CB_3D_CROP_MM, NULL);
        _fgVdpInit = FALSE;
        // printk(KERN_WARNING "VDP release fbg!\n");        
    }

    x_sema_unlock(hMt53fbMutex);             
    
    return 0;
}
#endif

#ifndef LINUX_TURNKEY_SOLUTION
static int _mt53fb_soft_cursor(struct fb_info *info, struct fb_cursor *cursor)
{
    //do nothing to disable linux frame buffer cursor appearing
       return 0;
}
#endif

static int _mt53fb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
                             u_int trans, struct fb_info *info)
{
    union palette pal;

    if (regno < 16 && info->fix.visual == FB_VISUAL_DIRECTCOLOR)
    {
        u32 pseudo_val;

        pseudo_val  = regno << info->var.red.offset;
        pseudo_val |= regno << info->var.green.offset;
        pseudo_val |= regno << info->var.blue.offset;

        ((u32 *)info->pseudo_palette)[regno] = pseudo_val;
    }

    pal.red   = red   >> 8;
    pal.green = green >> 8;
    pal.blue  = blue  >> 8;

    _cur_par.pal[regno] = pal;

    return 0;
}

static int _mt53fb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
    var->red.msb_right = 0;
    var->green.msb_right = 0;
    var->blue.msb_right = 0;
    var->transp.msb_right = 0;

    switch (var->bits_per_pixel)
    {
    case 1:
    case 2:
    case 4:
    case 8:
        var->red.offset    = 0;
        var->red.length    = var->bits_per_pixel;
        var->green         = var->red;
        var->blue          = var->red;
        var->transp.offset = 0;
        var->transp.length = 0;
        break;
    case 16:
        var->red.offset    = 0;
        var->red.length    = 4;
        var->green.offset  = 4;
        var->green.length  = 4;
        var->blue.offset   = 8;
        var->blue.length   = 4;
        var->transp.offset = 12;
        var->transp.length = 4;
        break;
    case 32:
    	/*
        var->red.offset    = 0;
        var->red.length    = 8;
        var->green.offset  = 8;
        var->green.length  = 8;
        var->blue.offset   = 16;
        var->blue.length   = 8;
        var->transp.offset = 24;
        var->transp.length = 8;
        */
        var->red.offset    = 16;
        var->red.length    = 8;
        var->green.offset  = 8;
        var->green.length  = 8;
        var->blue.offset   = 0;
        var->blue.length   = 8;
        var->transp.offset = 24;
        var->transp.length = 8;
        break;
    default:
        //printk(KERN_WARNING "_mt53fb_check_var() - invalid var->bits_per_pixel = %d\n",
            //var->bits_per_pixel);
        return -EINVAL;
    }
		info->var.red = var->red;
	info->var.green= var->green;
	info->var.blue= var->blue;
	info->var.transp= var->transp;
    return 0;
}


static int _mt53fb_set_par(struct fb_info *info)
{
    switch (info->var.bits_per_pixel)
    {
    case 1:
        break;
    case 2:
        break;
    case 4:
        break;
    case 8:
        break;
    case 16:
        break;
    case 32:
        break;
    default:
        BUG();
        break;
    }

    info->fix.line_length = (info->var.xres * info->var.bits_per_pixel) / 8;
    return 0;
}

static int _mt53fb_open(struct fb_info *info, int user)
{        
    static BOOL fgInit = FALSE;
    if(!fgInit)
    {
        x_sema_create(&hMt53fbMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK);
#ifndef LINUX_TURNKEY_SOLUTION
        x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
        GFX_Init();
        VERIFY(OSD_Reset() == 0);
        x_sema_unlock(hMt53fbMutex);
#endif
        fgInit = TRUE;

#ifndef LINUX_TURNKEY_SOLUTION
        printk("Open MT53_FB successfully\n");
#endif
    }
    else
    {
        x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);        
        x_sema_unlock(hMt53fbMutex);
        printk("Re-open MT53_FB successfully\n");    
    }

    return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
static int _mt53fb_mmap(struct fb_info *info, struct vm_area_struct *vma)
#else
static int  _mt53fb_mmap(struct fb_info *info, struct file *file, struct vm_area_struct *vma)
#endif
{
    unsigned long off, start;
    u32 len;
	unsigned long phys;
    unsigned long size = vma->vm_end - vma->vm_start;
	
    if(_prShmCmdQue == NULL)
    {
        UINT32 i;

        _prShmCmdQue = (GFX_CMDQUE_T *)VIRTUAL(BSP_AllocAlignedDmaMemory(PAGE_SIZE, PAGE_SIZE));
        _pu8CmdQueBuf = (UINT64 *)BSP_AllocAlignedDmaMemory(256 * 1024, PAGE_SIZE);
        
        GFX_SetExtCmdQue(_prShmCmdQue, _pu8CmdQueBuf,  E_GFX_CMDQ_CFG_256KB);
     
        SetPageReserved( virt_to_page(_prShmCmdQue) );
     
        for (i=0; i<256*1024/PAGE_SIZE; i++)
        {
             SetPageReserved( virt_to_page(_prShmCmdQue->pu8QueTop) + i );
        }         
    }
    

    off = vma->vm_pgoff << PAGE_SHIFT;

    start = info->fix.smem_start;
    len = PAGE_ALIGN(start & ~PAGE_MASK) + info->fix.smem_len;
    start &= PAGE_MASK;

    if ((vma->vm_end - vma->vm_start + off) > len)
    {
        if (off == len) {
            if (size != PAGE_SIZE)
                return -EINVAL;

            /* This is an IO map - tell maydump to skip this VMA */
            vma->vm_flags |= VM_IO;

            //vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_MT_BUFFERABLE);
#else	    
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif
            //vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);


            phys = (unsigned long)PHYSICAL((UINT32)_prShmCmdQue);

            //printk("%s: mapping queue structure at 0x%08lx\n", __FUNCTION__, phys);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
            return io_remap_pfn_range( vma, vma->vm_start,
                                        phys >> PAGE_SHIFT, size, vma->vm_page_prot );
#else
            return io_remap_page_range( vma, vma->vm_start,
                                        phys, size, vma->vm_page_prot );
#endif                                        
        }
        if (off == len + PAGE_SIZE) {
            if (size > 256 * 1024)
                return -EINVAL;

            /* This is an IO map - tell maydump to skip this VMA */
            vma->vm_flags |= VM_IO;

            //vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
            //vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_MT_BUFFERABLE);
#else		    
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif


            phys = (unsigned long)_pu8CmdQueBuf;

            //printk("%s: mapping DMA region at 0x%08lx\n", __FUNCTION__, phys);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
            return io_remap_pfn_range( vma, vma->vm_start,
                                        phys >> PAGE_SHIFT, size, vma->vm_page_prot );
#else
            return io_remap_page_range( vma, vma->vm_start,
                                        phys, size, vma->vm_page_prot );
#endif                                        
        }

#ifdef CC_OSD_USE_FBM    
        if (off == len + PAGE_SIZE + 256 * 1024) {
            if (size > _rImgBufInfo.u4Size)
                return -EINVAL;

            /* This is an IO map - tell maydump to skip this VMA */
            vma->vm_flags |= VM_IO;

            //vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
            //vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_MT_BUFFERABLE);
#else	
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif
            phys = (unsigned long)_rImgBufInfo.u4PhyAddr;

            //printk("%s: mapping DMA region at 0x%08lx\n", __FUNCTION__, phys);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
            return io_remap_pfn_range( vma, vma->vm_start,
                                        phys >> PAGE_SHIFT, size, vma->vm_page_prot );
#else
            return io_remap_page_range( vma, vma->vm_start,
                                        phys, size, vma->vm_page_prot );
#endif                                        
        }
#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB
        if (off == len + PAGE_SIZE + 256 * 1024 + _rImgBufInfo.u4Size) {
            if (size > _rImgBufInfo2.u4Size)
                return -EINVAL;

            /* This is an IO map - tell maydump to skip this VMA */
            vma->vm_flags |= VM_IO;

            //vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
            //vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_MT_BUFFERABLE);
#else          
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif
            phys = (unsigned long)_rImgBufInfo2.u4PhyAddr;

            //printk("%s: mapping DMA region at 0x%08lx\n", __FUNCTION__, phys);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
            return io_remap_pfn_range( vma, vma->vm_start,
                                        phys >> PAGE_SHIFT, size, vma->vm_page_prot );
#else
            return io_remap_page_range( vma, vma->vm_start,
                                        phys, size, vma->vm_page_prot );
#endif                                        
        }
#endif /* CC_FBM_TWO_FBP_SHARED_WITH_DFB */
#endif
#ifdef CC_DFB_SUPPORT_VDP_LAYER
       #ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB 
	   if (off == len + PAGE_SIZE + 256 * 1024 + _rImgBufInfo.u4Size + _rImgBufInfo2.u4Size)
       #else
	   if (off == len + PAGE_SIZE + 256 * 1024 + _rImgBufInfo.u4Size)
	   #endif
	   {
            if (size > _rVdpBufInfo.u4Size)
                return -EINVAL;

            /* This is an IO map - tell maydump to skip this VMA */
            vma->vm_flags |= VM_IO;

            //vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
            //vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_MT_BUFFERABLE);
	#else	
            vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
	#endif
            phys = _rVdpBufInfo.u4PhyAddr;

            //printk("%s: mapping DMA region at 0x%08lx\n", __FUNCTION__, phys);
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
            return io_remap_pfn_range( vma, vma->vm_start,
                                        phys >> PAGE_SHIFT, size, vma->vm_page_prot );
	#else
            return io_remap_page_range( vma, vma->vm_start,
                                        phys, size, vma->vm_page_prot );
	#endif										  
        }
#endif /* CC_DFB_SUPPORT_VDP_LAYER	*/

        return -EINVAL;
    }

    off += start;
    vma->vm_pgoff = off >> PAGE_SHIFT;

    /* This is an IO map - tell maydump to skip this VMA */
    vma->vm_flags |= VM_IO;

    //vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35)
    vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_MT_BUFFERABLE);
#else	
    vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) | L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif

    /*
     * Don't alter the page protection flags; we want to keep the area
     * cached for better performance.  This does mean that we may miss
     * some updates to the screen occasionally, but process switches
     * should cause the caches and buffers to be flushed often enough.
     */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)     
    if(io_remap_pfn_range(vma, vma->vm_start, off >> PAGE_SHIFT,
                vma->vm_end - vma->vm_start,
                vma->vm_page_prot))         
#else
    if(io_remap_page_range(vma, vma->vm_start, off,
                vma->vm_end - vma->vm_start,
                vma->vm_page_prot))         
#endif                
    {        
        return -EAGAIN;
    }
#ifdef LINUX_TURNKEY_SOLUTION
    fb_user_virt_start = vma->vm_start;
#endif

    return 0;
}



#ifdef CC_TDTV_3D_OSD_ENABLE
static int _mt53fbGetIsSBSOutput(void* pvArg)
{
    int i4Ret = 0;   
    u8 rGet, data;
  OSD_BASE_Set3DStatus();
  data = gfgOsd_Sbs;
    if(copy_to_user(pvArg, (void *)&data, sizeof(data)))
        {
            printk(KERN_WARNING "_mt53fbGetIsSBSOutput() -  copy_to_user() fail\n");
            return -EFAULT;            
        
    }
    return i4Ret;
}

static int _mt53fbGetIsTBBOutput(void* pvArg)
{
    int i4Ret = 0;
    
    u8 rGet, data;
   OSD_BASE_Set3DStatus();
    data =  gfgOsd_Tbb; //GfxGetIsSBSOutput();         
    if(copy_to_user(pvArg, (void *)&data, sizeof(data)))
        {
            printk(KERN_WARNING "_mt53fbGetIsTBBOutput() -  copy_to_user() fail\n");
            return -EFAULT;            
        
    }
    return i4Ret;
}

static int _mt53fbGet3DOsdShiftt(void* pvArg)
{
	int i4Ret = 0;	
	u8 rGet, data;
	OSD_BASE_Set3DStatus();
	data = _u13DOsdShift; //GfxGetIsSBSOutput();		   
	if(copy_to_user(pvArg, (void *)&data, sizeof(data)))
		{
			printk(KERN_WARNING "_mt53fbGetIsTBBOutput() -	copy_to_user() fail\n");
			return -EFAULT; 		   
			
		}
	return i4Ret;
}


#endif	

INT32 _mt53fb_do_mw_op(void* pvArg)
{
    mw_gl_op_t tOp;
    unsigned long phy_addr;
    
    if (copy_from_user(&tOp, pvArg, sizeof(mw_gl_op_t)))
    {        
        return -EFAULT;
    }

    switch(tOp.u4Op)
    {
        case MW_GL_OP_FILL_RECT:
            phy_addr = fb_convert_app_to_phy( (unsigned int)tOp.u.fill_rect.pv_bits);
            GFX_Lock();    
            GFX_Flush_Invalid_DCache();
            GFX_SetDst((UINT8 *)phy_addr, tOp.u.fill_rect.e_mode, tOp.u.fill_rect.ui4_pitch);
            GFX_SetColor(tOp.u.fill_rect.ui4_color);
            GFX_Fill(tOp.u.fill_rect.i4_x, 
                     tOp.u.fill_rect.i4_y, 
                     tOp.u.fill_rect.ui4_width, 
                     tOp.u.fill_rect.ui4_height);

            GFX_Flush();
            GFX_Wait();            
            GFX_Unlock();
            break;
        case MW_GL_OP_SET_PLANE:
            printk(" FFFFFF Set plane enable by MW OP idx: %d enable:%d\n", tOp.u.set_plane.i4_idx, tOp.u.set_plane.i4_enable);
#ifndef TWO_OSD_PLANE
            if(tOp.u.set_plane.i4_idx + 1 == OSD_PLANE2)
            {
                printk(KERN_WARNING "_mt53fb_do_mw_op() - Not support set plane 2. Plane 2 is for Android\n");            
                return 0;
            }
#endif            
            if(tOp.u.set_plane.i4_idx + 1 == OSD_PLANE2)
            {
                if(tOp.u.set_plane.i4_enable == 1)
                {                
                    VERIFY(OSD_PLA_FlipTo(OSD_PLANE2, _u4RegionList2) == 0);
                }
                else
                {
                    VERIFY(OSD_PLA_FlipToNone(OSD_PLANE2) == 0);
                }
            }
            else if(tOp.u.set_plane.i4_idx + 1 == OSD_PLANE1)
            {
#if defined(CONFIG_OSD_COMPRESSION) || defined(CC_CONFIG_OSD_SBS)               
                if(tOp.u.set_plane.i4_enable == 1)
                {             
                    //VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4CompressedRegionList[_fgCompressedOSDFlip?1:0]) == 0);
                }
                else
                {
                    //VERIFY(OSD_PLA_FlipToNone(OSD_PLANE1) == 0);
                }
#else
                if(tOp.u.set_plane.i4_enable == 1)
                {             
                    VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _u4RegionList) == 0);

                }
                else
                {
                    VERIFY(OSD_PLA_FlipToNone(OSD_PLANE1) == 0);
                }
#endif                
            }
#if defined(CONFIG_OSD_COMPRESSION) || defined(CC_CONFIG_OSD_SBS)
            OSD_PLA_Enable(tOp.u.set_plane.i4_idx + 1, tOp.u.set_plane.i4_enable);
#endif
            break;
        case MW_GL_OP_GET_FBM_BITMAP_INFO:
            tOp.u.get_fbm_bitmap_info.ui4_size = _rImgBufInfo.u4Size;
            tOp.u.get_fbm_bitmap_info.pv_bits = (void*)_rImgBufInfo.u4PhyAddr;
            if (copy_to_user(pvArg, &tOp, sizeof(mw_gl_op_t)))
            {        
                return -EFAULT;
            }            
            break;            
        case MW_GL_OP_ENABLE_SCALER:           
            {
            UINT16 u2PanelW;
            UINT16 u2PanelH;
            u2PanelW = PANEL_GetPanelWidth();
            u2PanelH = PANEL_GetPanelHeight();

#ifndef TWO_OSD_PLANE
            if(tOp.u.enable_scaler.ui4_id == OSD_SCALER2)
            {
                printk(KERN_WARNING "_mt53fb_do_mw_op() - Not support set scaler 2. Scaler 2 is for Android\n");            
                return 0;
            }
#endif    
        
            if (tOp.u.enable_scaler.ui4_enable == 1)
            {
                OSD_SC_Scale(tOp.u.enable_scaler.ui4_id, TRUE, tOp.u.enable_scaler.ui4_width, tOp.u.enable_scaler.ui4_height,
                    u2PanelW, u2PanelH);
                _fgOsdScEnable[tOp.u.enable_scaler.ui4_id] = TRUE;
            }
            else
            {
                OSD_SC_Scale(tOp.u.enable_scaler.ui4_id, TRUE, u2PanelW, u2PanelH, u2PanelW, u2PanelH);           
                _fgOsdScEnable[tOp.u.enable_scaler.ui4_id] = FALSE;                
            }
            }
            break;                        
        default:
            printk("Cannot do MW OP %d\n", tOp.u4Op);
            
    }    return 0;
}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
static int _mt53fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg)
#else
static int _mt53fb_ioctl(struct inode *inode, struct file *file, unsigned int cmd,unsigned long arg, struct fb_info *info)
#endif
{
    void __user *pvArg = (void __user *)arg;
    INT32 i4Ret = 0;
    switch(cmd)
    {
        case FBIO_GFX_FLUSH:
            mt53fb_flush_ex_action(pvArg);
            GFX_Flush();                       
            GFX_Unlock();                        
            break;     
        case FBIO_GFX_LOCK:
            GFX_Lock();
            break;
        case FBIO_GFX_FLUSH_DCACHE:
            GFX_Flush_Invalid_DCache();
            break;
        case FBIO_GFX_UNLOCK:
            GFX_Unlock();
            break;
        case FBIO_GFX_WAIT:
            GFX_Lock();  
            GFX_Wait();
            GFX_Unlock();
            break;  
        case FBIO_MMU_GETPGD:
            i4Ret = mt53fb_get_pgt_ioctl((unsigned long)pvArg);
            break;
        case FBIO_MMU_PINMEM:
            i4Ret = mt53fb_pin_mem_ioctl((unsigned long)pvArg);
            break;
        case FBIO_MMU_UNPINMEM:
            i4Ret = mt53fb_unpin_mem_ioctl((unsigned long)pvArg);
            break;
        case FBIO_MMU_MAPSINGLE:
            i4Ret = mt53fb_dma_map_single_ioctl((unsigned long)pvArg);
            break;
        case FBIO_MMU_UNMAPSINGLE:
            i4Ret = mt53fb_dma_unmap_single_ioctl((unsigned long)pvArg);
            break;
        case FBIO_DRAWLINE:
            i4Ret = mt53fb_drawline_ioctl(pvArg);
            break;
        case FBIO_DRAWLINEROP:
            i4Ret = mt53fb_drawlinerop_ioctl(pvArg);
            break;
        case FBIO_DRAWRECT:
            i4Ret = mt53fb_drawrect_ioctl(pvArg);
            break;
        case FBIO_DRAWRECTROP:
            i4Ret = mt53fb_drawrectrop_ioctl(pvArg);
            break;
        case FBIO_DRAWTEXT:
            i4Ret = _mt53fb_drawtext_ioctl(pvArg);
            break;    
        case FBIO_RECTFILL:
            i4Ret = mt53fb_fillrect_ioctl(pvArg);
            break;
        case FBIO_COPYAREA:
            i4Ret = mt53fb_copyarea_ioctl(pvArg);
            break;
        case FBIO_IMAGEBLIT:
            i4Ret = mt53fb_imageblit_ioctl(pvArg);
            break;
        case FBIO_IMAGEBLITROP:
            i4Ret = mt53fb_imageblitrop_ioctl(pvArg);
            break;                      
        case FBIO_RECTFILLROP:
            i4Ret = mt53fb_fillrectrop_ioctl(pvArg);
            break;
        case FBIO_COPYAREAROP:
            i4Ret = mt53fb_copyarearop_ioctl(pvArg);
            break;       
        case FBIO_SET:
            i4Ret = _mt53fb_set_ioctl(pvArg);
            break;
        case FBIO_GET_PLANE_ORDER_ARRAY:
            i4Ret = _mt53fb_get_plane_order_array_ioctl(pvArg);
            break;            
        case FBIO_SET_PLANE_ORDER_ARRAY:
            i4Ret = _mt53fb_set_plane_order_array_ioctl(pvArg);
            break; 
        case FBIO_SET_OSD_PLANE_ORDER_ARRAY:
            i4Ret = _mt53fb_set_osd_plane_order_ioctl(pvArg);
            break;
        case FBIO_PALETTE:
            i4Ret = _mt53fb_palette_ioctl(pvArg);
            break;
        case FBIO_GET:
            i4Ret = _mt53fb_get_ioctl(pvArg);
            break;            
        case FBIO_GETIMAGEBUFFER:
            i4Ret = _mt53fb_get_imagebuffer_ioctl(pvArg);
            break;
#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB
        case FBIO_GETIMAGEBUFFER2:
            i4Ret = _mt53fb_get_imagebuffer2_ioctl(pvArg);
            break;      
#endif      
        case FBIO_GETFRAMEBUFFER:
			i4Ret = _mt53fb_get_framebuffer_ioctl(pvArg);
			break;
#if FB_INIT_BY_AP
        case FBIO_INIT:
            i4Ret = _mt53fb_init_hw(pvArg);
            break;
#endif
#if FB_DBG
        case FBIO_GETTIME:
            i4Ret = _mt53fb_get_time(pvArg);
            break;
#endif   
        case FBIO_MW_OP:
            _mt53fb_do_mw_op(pvArg);
            break;
        case FBIO_GFX_STRETCH:
            i4Ret = _mt53fb_gfx_stretch_ioctl(pvArg);
            break;
#ifdef LINUX_TURNKEY_SOLUTION
        case FBIO_WAITLOGO:
            i4Ret = _mt53fb_wait_logo(pvArg);
            break;   
#endif
     case FBIO_GFX_WAIT_VDP:
            GFX_Wait_Vdp();
            break;  
        case FBIO_IMGRZ_STRETCH_ALCOM:
            i4Ret = _mt53fb_imgrz_stretch_alcom_ioctl(pvArg);                     
            break;
        case FBIO_VGFX_STRETCH_ALCOM:
            i4Ret = _mt53fb_vgfx_scaler_ioctl(pvArg);                   
            break;  
        case FBIO_VGFX_SCALER:
            i4Ret = _mt53fb_vgfx_ioctl(pvArg);                   
            break;  
#ifdef CC_TDTV_3D_OSD_ENABLE
        case FBIO_GET_SBSOUTPUT:
        i4Ret = _mt53fbGetIsSBSOutput(pvArg);
        break;
      case FBIO_GET_TBBOUTPUT:
        i4Ret = _mt53fbGetIsTBBOutput(pvArg);
        break;
      case FBIO_GET_3DOSD_SHIFT:
        i4Ret = _mt53fbGet3DOsdShiftt(pvArg);
        break;
#endif

#ifdef CC_DFB_SUPPORT_VDP_LAYER
      case FBIO_VDP_INIT:
        i4Ret = _mt53fb_vdp_init_ioctl(pvArg);
        break;
      case FBIO_VDP_FLIP:
        i4Ret = _mt53fb_vdp_flip_ioctl(pvArg);
        break;      
      case FBIO_VDP_SET:        
        i4Ret = _mt53fb_vdp_set_ioctl(pvArg);
        break;                          
      case FBIO_VDP_ROTATE:     
        #ifdef CC_DFB_SUPPORT_VDP_LAYER
        i4Ret = _mt53fb_vdp_rotate_ioctl(pvArg);
        #endif
        break;      
        case FBIO_VDP_GETBUFFER:
            i4Ret = _mt53fb_get_vdpbuffer_ioctl(pvArg);
            break;   
        case FBIO_VDP_SET_CROP_RECT:
            i4Ret = _mt53fb_set_vdp_crop_rect_ioctl(pvArg);
            break;
        case FBIO_VDP_GET_CROP_RECT:
            i4Ret = _mt53fb_get_vdp_crop_rect_ioctl(pvArg);
            break;
        case FBIO_VDP_STRETCH_BLIT:
            i4Ret = _mt53fb_vdp_stretch_blit_ioctl(pvArg);
            break;
#endif

        default:
            //printk(KERN_WARNING "_mt53fb_ioctl(): not support ioctl cmd %d\n", cmd);            
            i4Ret = -EINVAL;
            break;
    }
    return i4Ret;
}


/* --------------------------------------------------------------------- */



static void __init _mt53fb_parse_colormode(char *opt)
{
}

static void __init _mt53fb_parse_dram(char *opt)
{
    unsigned int size;

    size = simple_strtoul(opt, &opt, 0);

    if (opt)
    {
        switch (*opt)
        {
        case 'M':
        case 'm':
            size *= 1024 * 1024;
            break;
        case 'K':
        case 'k':
            size *= 1024;
            break;
        default:
            break;
        }
    }

    _cur_par.dram_size = size;
}

static void __init _mt53fb_init_fbinfo(void)
{
    static int first = 1;

    if (!first)
        return;
    first = 0;

    _fb_info.fbops          = &_mt53fb_ops;
    _fb_info.flags          = FBINFO_DEFAULT;
    _fb_info.pseudo_palette = _cur_par.pseudo_palette;

    strcpy(_fb_info.fix.id, "mt53xx");
    _fb_info.fix.type       = FB_TYPE_PACKED_PIXELS;
    _fb_info.fix.type_aux   = 0;
    _fb_info.fix.xpanstep   = 0;
    _fb_info.fix.ypanstep   = 0;
    _fb_info.fix.ywrapstep  = 0;
    _fb_info.fix.line_length = 0;
    _fb_info.fix.accel      = FB_ACCEL_NONE;

    /*
     * setup initial parameters
     */
    memset(&_fb_info.var, 0, sizeof(_fb_info.var));

    _fb_info.var.xres           = MAX_X_RES;
    _fb_info.var.yres           = MAX_Y_RES;
    _fb_info.var.xres_virtual   = MAX_X_RES;
    _fb_info.var.yres_virtual   = MAX_Y_RES * 2;
    _fb_info.var.xoffset        = 0;
    _fb_info.var.yoffset        = 0;
    _fb_info.var.bits_per_pixel = 16;
#ifdef LINUX_TURNKEY_SOLUTION
    _fb_info.var.red.length     = 4;
    _fb_info.var.green.length   = 4;
    _fb_info.var.blue.length    = 4;
    _fb_info.var.transp.length  = 4;
#else
    _fb_info.var.red.length     = 5;
    _fb_info.var.green.length   = 6;
    _fb_info.var.blue.length    = 5;
    _fb_info.var.transp.length  = 0;
#endif
    _fb_info.var.nonstd         = 0;
    _fb_info.var.activate       = FB_ACTIVATE_NOW;
    _fb_info.var.width          = MAX_X_RES;
    _fb_info.var.height         = MAX_Y_RES;
    _fb_info.var.vmode          = FB_VMODE_NONINTERLACED;
    _fb_info.var.accel_flags    = FB_ACCELF_TEXT;

    _cur_par.dram_size          = 0;
}


static int __init _mt53fb_probe(struct device *dev)
{
    unsigned long size;
    char *option = NULL;
    UINT32 u4AddrOsd1, u4AddrOsd4;

    if (fb_get_options("mt53fb", &option))
        return -ENODEV;

    _mt53fb_setup(option);

    _cur_par.dev = dev;

    if (!_cur_par.dram_size)
    {
        size = MAX_SIZE;
    }
    else
    {
        size = _cur_par.dram_size;
        if (size > MAX_SIZE)
        {
            size = MAX_SIZE;
        }
    }

    size = PAGE_ALIGN(size);

    // Image buffer outside FBM's control                            
    // Ensure page alignment
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eFBMemSize) & ~PAGE_MASK) == 0);      
    VERIFY(PAGE_ALIGN(DRVCUST_OptGet(eDirectFBMemSize) & ~PAGE_MASK) == 0);      
    
    _fb_info.fix.smem_start = ((BSP_GetFbmMemAddr() - DRVCUST_OptGet(eDirectFBMemSize)) & PAGE_MASK);
        
    // Let smem_len excludes font-rendering buffer
    _fb_info.fix.smem_len = DRVCUST_OptGet(eDirectFBMemSize);

	GFX_SetDirectFBMem(_fb_info.fix.smem_start, _fb_info.fix.smem_len);

#ifdef CC_OSD_USE_FBM  
#ifndef CC_DFB_SUPPORT_VDP_LAYER
    {
        FBM_POOL_T *prFbmPool;
        FBM_Init();
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPEG_OSD1);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);        
        _rImgBufInfo.u4Size = prFbmPool->u4Size;                
        u4AddrOsd1 = prFbmPool->u4Addr;        
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPEG_OSD2);        
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);                
        _rImgBufInfo.u4Size += prFbmPool->u4Size;        
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPEG_OSD3);        
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);                
        _rImgBufInfo.u4Size += prFbmPool->u4Size;                
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPEG_OSD4);        
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);                
        _rImgBufInfo.u4Size += prFbmPool->u4Size;              
 		u4AddrOsd4 = prFbmPool->u4Addr;        

 	if (u4AddrOsd1 < u4AddrOsd4)
 	{
	 	_rImgBufInfo.u4PhyAddr = u4AddrOsd1;
 	}
 	else
 	{
	 	_rImgBufInfo.u4PhyAddr = u4AddrOsd4; 	
 	}

        _rImgBufInfo.u4VirtAddr = VIRTUAL(_rImgBufInfo.u4PhyAddr);

        printk("DFB JPEG OSD: %x, PHY:%x, SIZE: %x\n",
               _rImgBufInfo.u4VirtAddr,
               _rImgBufInfo.u4PhyAddr,
               _rImgBufInfo.u4Size);        
    }
#else
	{
		FBM_POOL_T *prFbmPool;
		FBM_Init();

		prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN);
		ASSERT(prFbmPool != NULL);
		ASSERT(prFbmPool->u4Addr != (UINT32)NULL);                
		_rImgBufInfo.u4Size = prFbmPool->u4Size;
		_rImgBufInfo.u4PhyAddr = prFbmPool->u4Addr;
		_rImgBufInfo.u4VirtAddr = VIRTUAL(_rImgBufInfo.u4PhyAddr);

		printk("DFB FROM FBM: %x, PHY:%x, SIZE: %x\n",
		       _rImgBufInfo.u4VirtAddr,
		       _rImgBufInfo.u4PhyAddr,
		       _rImgBufInfo.u4Size);           

		prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_JPG_VDP);
		ASSERT(prFbmPool != NULL);
		ASSERT(prFbmPool->u4Addr != (UINT32)NULL);   
		// Handle start offset alignment              
		if((prFbmPool->u4Addr & PAGE_MASK) != 0)
		{
		    _rVdpBufInfo.u4PhyAddr = ((prFbmPool->u4Addr - 1) >> PAGE_SHIFT << PAGE_SHIFT) + PAGE_SIZE;
		}
		else
		{
		    _rVdpBufInfo.u4PhyAddr = prFbmPool->u4Addr;  
		}
		// Handle size alignment
		_rVdpBufInfo.u4Size = (prFbmPool->u4Addr + prFbmPool->u4Size - _rVdpBufInfo.u4PhyAddr) & PAGE_MASK;
		_rVdpBufInfo.u4VirtAddr = VIRTUAL(_rVdpBufInfo.u4PhyAddr);

		printk("DFB JPEG VDP: %x, PHY:%x, SIZE: %x\n",
		       _rVdpBufInfo.u4VirtAddr,
		       _rVdpBufInfo.u4PhyAddr,
		       _rVdpBufInfo.u4Size);              
	}

#endif

#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB 
    {
        FBM_POOL_T *prFbmPool;

        FBM_Init();
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_FEEDER2);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);        
        //_rImgBufInfo2.u4Size = prFbmPool->u4Size;                
        u4AddrOsd1 = prFbmPool->u4Addr;        
        u4AddrOsd4 = prFbmPool->u4Addr + prFbmPool->u4Size;                
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_DMX2);        
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);                
        //_rImgBufInfo2.u4Size += prFbmPool->u4Size;        
        u4AddrOsd1 = (prFbmPool->u4Addr < u4AddrOsd1)?(prFbmPool->u4Addr):u4AddrOsd1;                
        u4AddrOsd4 = (prFbmPool->u4Addr + prFbmPool->u4Size > u4AddrOsd4)?(prFbmPool->u4Addr + prFbmPool->u4Size):u4AddrOsd4;        
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MPEG2);        
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0x0);                
        //_rImgBufInfo2.u4Size += prFbmPool->u4Size;                         
 	 _rImgBufInfo2.u4PhyAddr  = (prFbmPool->u4Addr < u4AddrOsd1)?(prFbmPool->u4Addr):u4AddrOsd1;                
        u4AddrOsd4 = (prFbmPool->u4Addr + prFbmPool->u4Size > u4AddrOsd4)?(prFbmPool->u4Addr + prFbmPool->u4Size):u4AddrOsd4;        
        
        // Round to page align
        _rImgBufInfo2.u4PhyAddr = (((_rImgBufInfo2.u4PhyAddr - 1) >> PAGE_SHIFT) << PAGE_SHIFT) + PAGE_SIZE;
        u4AddrOsd4 &= PAGE_MASK;
        _rImgBufInfo2.u4Size = u4AddrOsd4 - _rImgBufInfo2.u4PhyAddr;
        _rImgBufInfo2.u4VirtAddr = VIRTUAL(_rImgBufInfo2.u4PhyAddr);
       
        printk("DFB 3DMM Buffer: %x, PHY:%x, SIZE: %x\n",
               _rImgBufInfo2.u4VirtAddr,
               _rImgBufInfo2.u4PhyAddr,
               _rImgBufInfo2.u4Size);        
    }
#endif /* CC_FBM_TWO_FBP_SHARED_WITH_DFB */

#else
    _rImgBufInfo.u4PhyAddr = _fb_info.fix.smem_start;
    
    // Real image buffer excludes font-rendering buffer
    _rImgBufInfo.u4Size = _fb_info.fix.smem_len;     
#endif

    // If _fb_info.smem_start is below tha kernel's DRAM limit, FB cannot successfully initialize
//    if(_fb_info.fix.smem_start < KERNEL_DRAM_SIZE)
    if(_fb_info.fix.smem_start < DRVCUST_OptGet(eLinuxKernelMemSize))
    {
        printk(KERN_ERR "_mt53fb_probe() fail: bad framebuffer memory layout! KM:%d\n", DRVCUST_OptGet(eLinuxKernelMemSize));
        return -ENOMEM;
    }            

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)
    _fb_info.screen_base = ioremap_cached((int)_fb_info.fix.smem_start, _fb_info.fix.smem_len);
#else
    _fb_info.screen_base = __ioremap((int)_fb_info.fix.smem_start, _fb_info.fix.smem_len, L_PTE_BUFFERABLE | L_PTE_CACHEABLE);
#endif

    if (_fb_info.screen_base == NULL)
    {
        return -ENOMEM;
    }

#if defined(CC_MT5396)
#ifndef CC_DFB_SUPPORT_VDP_LAYER
    memset(_fb_info.screen_base, 0, _fb_info.fix.smem_len);
    iounmap(_fb_info.screen_base);
#else    
    printk("FB VIRTUAL %x PHYSICAL %x Size %x xxxx\n",_fb_info.screen_base,_fb_info.fix.smem_start, _fb_info.fix.smem_len);
    IMG_SeDirectFBMem((UINT32)_fb_info.fix.smem_start,(UINT32)_fb_info.screen_base, (UINT32)_fb_info.fix.smem_len);
#endif
#endif

#ifdef CONFIG_OSD_COMPRESSION
    #if !defined(CC_MT5396)
    /* clear screen */
    memset(_fb_info.screen_base, 0, (MAX_OSD_COMPRESSED_BUFFER_SIZE * NUM_OF_COMPRESSED_BUFFERS));
    #endif

    _apu1CompressedOSDBuf[0] = (UINT8*)_fb_info.fix.smem_start;
    _apu1CompressedOSDBuf[1] = (UINT8*)_fb_info.fix.smem_start + MAX_OSD_COMPRESSED_BUFFER_SIZE;
    _fb_info.screen_base += (MAX_OSD_COMPRESSED_BUFFER_SIZE * NUM_OF_COMPRESSED_BUFFERS);
    _fb_info.fix.smem_start += (MAX_OSD_COMPRESSED_BUFFER_SIZE * NUM_OF_COMPRESSED_BUFFERS);
    _fb_info.fix.smem_len -= (MAX_OSD_COMPRESSED_BUFFER_SIZE * NUM_OF_COMPRESSED_BUFFERS);    
    printk("DFB OSD COMPRESSION double buffer: buf0 0x%8x, buf 1:0x%8x\n",
    	_apu1CompressedOSDBuf[0],_apu1CompressedOSDBuf[1]); 
#elif defined(CC_CONFIG_OSD_SBS)
  _apu1OSDSbsBuf[0] = _fb_info.fix.smem_start;
  _apu1OSDSbsBuf[1] = _fb_info.fix.smem_start + MAX_OSD_SBS_BUFFER_SIZE;
  _fb_info.screen_base += MAX_OSD_SBS_BUFFER_SIZE;
  _fb_info.fix.smem_start += MAX_OSD_SBS_BUFFER_SIZE;
  _fb_info.fix.smem_len -= MAX_OSD_SBS_BUFFER_SIZE ;    
  printk("DFB OSD SBS  buffer:  0x%8x, 0x%x\n",_apu1OSDSbsBuf[0],_apu1OSDSbsBuf[1]); 
#endif

#ifdef LINUX_TURNKEY_SOLUTION
    printk("DFB screen_base %x, PHY:%x\n",
            _fb_info.screen_base,
            _fb_info.fix.smem_start);
#endif
   OSD_SetMemCallback(dfb_malloc, dfb_free); 

  //OSD_Set_DirectFBMemory(_fb_info.fix.smem_start);  
    
#if 0
    _apu4Palette[OSD_PLANE_1] = (UINT32 *)BSP_AllocAlignedDmaMemory(PALETTE_SIZE, 0x10);
    _apu4Palette[OSD_PLANE_2] = (UINT32 *)BSP_AllocAlignedDmaMemory(PALETTE_SIZE, 0x10);
    
    _pu1TextArray = (unsigned char*)BSP_AllocAlignedDmaMemory(TEXT_SIZE,0x10);

    _apu4PaletteVirt[OSD_PLANE_1] = (UINT32 *)VIRTUAL((UINT32)_apu4Palette[OSD_PLANE_1]);
    _apu4PaletteVirt[OSD_PLANE_2] = (UINT32 *)VIRTUAL((UINT32)_apu4Palette[OSD_PLANE_2]);
    
    _pu1TextArrayVirt = (unsigned char*)VIRTUAL((UINT32)_pu1TextArray);
#endif
    /*
    if (fb_set_var(&_fb_info, &_fb_info.var))
        //printk(KERN_ERR "mt53fb: unable to set display parameters\n");
    */

    if (register_framebuffer(&_fb_info) < 0)
    {
        printk(KERN_ERR "_mt53fb_probe() fail: cannot register mt53fb!\n");
        return -ENXIO;
    }
#if 0
    #if 1 //((!(defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389) ))||defined(CC_DFB_SUPPORT_VDP_LAYER))
    if (_u4TmpBufAddr == 0)
    {
    #ifdef ANDROID
		_u4TmpBufAddr = 0;
	#else
        _u4TmpBufAddr =  (UINT32)BSP_AllocAlignedDmaMemory(TMPBUF_WIDTH*TMPBUF_HEIGHT*TMPBUF_BPP, 16);
        ASSERT(_u4TmpBufAddr != 0);
	#endif
        printk("TmpBuf = 0x%x\n", _u4TmpBufAddr);

    }   
    #endif
#endif
    return 0;
}

static int _mt53fb_drawtext_ioctl(void* pvArg)
{
    struct fb_info* prFb;
    struct mt53fb_DrawText rDrawText;
    struct mt53fb_DrawText* prDrawText;
    unsigned char* pu1Virt = NULL;

    prDrawText = (struct mt53fb_DrawText*)(pvArg);

    pu1Virt = (unsigned char*)VIRTUAL((UINT32)_pu1TextArray);
    
    prFb = mt53fb_get_fb_info();

    //printk("address : %x %x\n",pu1Virt,(prDrawText->pTextArray));
    
    if(copy_from_user(pu1Virt, (prDrawText->pTextArray), sizeof(TEXT_SIZE)))
    {
        //printk(KERN_WARNING "mt53fb_drawtext_ioctl() error: Copy from user error1!\n");
        return -EFAULT;
    }

    rDrawText.pTextArray = (void*)pu1Virt;
    
    if(copy_from_user((void*)&rDrawText, pvArg, sizeof(struct mt53fb_DrawText)))
    {
        //printk(KERN_WARNING "mt53fb_drawtext_ioctl() error: Copy from user error2!\n");
        return -EFAULT;
    }
    
    
    return mt53fb_drawtext(prFb, &rDrawText);   

}

static int _mt53fb_palette_ioctl(void* pvArg)
{
    static unsigned int u4Region;
	unsigned int 		ui4_id = 0x0;
	
    if(copy_from_user(&ui4_id, pvArg,4))
    {
        return -EFAULT;
    } 
	
    if(ui4_id == OSD_PLANE_1) 
    {
        u4Region = _u4Region;
    }
    else if(ui4_id == OSD_PLANE_2) 
    {
        u4Region = _u4Region2;     
    }
    else
    {
        VERIFY(0);
    }

    memcpy(_apu4PaletteVirt[ui4_id], (pvArg+4), PALETTE_SIZE);
        
    if(OSD_RGN_Set(u4Region, OSD_RGN_PAL_ADDR, (UINT32)_apu4Palette[ui4_id]) != OSD_RET_OK)
    {
    	return -E_GFXLIB_PALE_ADDR_ERR;
    }
	
    OSD_RGN_Set(u4Region, OSD_RGN_PAL_LEN, 2);    
    OSD_RGN_Set(u4Region, OSD_RGN_PAL_LOAD, 1);

    return 0;
}


static int _mt53fb_get_ioctl(void* pvArg)
{
    //int i4Ret = 0;

    //UINT32 u4Region = _u4Region;
    
    struct mt53fb_get rGet;

    if(copy_from_user(&rGet, pvArg, sizeof(struct mt53fb_get)))
    {
        //printk(KERN_WARNING "_mt53fb_get_ioctl() - copy_from_user() fail\n");
        return -EFAULT;
    }

    if(rGet.get_type == MT53FB_GET_PANEL_SIZE)
    {
        unsigned int data[2];
        data[0] = (UINT32)PANEL_GetPanelWidth();
        data[1] = (UINT32)PANEL_GetPanelHeight();
        if(copy_to_user((void*)rGet.get_data, data, rGet.get_size))
        {
            //printk(KERN_WARNING "_mt53fb_get_ioctl() - copy_to_user() fail\n");
            return -EFAULT;            
        }
    }
    return 0;
}


static int _mt53fb_set_ioctl(void* pvArg)
{
    int i4Ret = 0;
    unsigned short u2PanelWidth;
    unsigned short u2PanelHeight;
    UINT32 u4Region = _u4Region;
    UINT32 u4_rgn_list	= _u4RegionList;
    #ifdef CONFIG_OSD_COMPRESSION
    UINT32 u4RegionCompress2;
    #endif
    struct mt53fb_set rSet;

    if(copy_from_user(&rSet, pvArg, sizeof(struct mt53fb_set)))
    {
        printk(KERN_WARNING "_mt53fb_set_ioctl() - copy_from_user() fail\n");
        return -EFAULT;
    }

    // Should Check !!
    //PMX_GetResolution(0, &u2PanelWidth, &u2PanelHeight);
    u2PanelWidth = PANEL_GetPanelWidth();
    u2PanelHeight = PANEL_GetPanelHeight();

#if defined(CONFIG_OSD_COMPRESSION) || defined(CC_CONFIG_OSD_SBS)
    x_sema_lock(hMt53fbMutex, X_SEMA_OPTION_WAIT);
#endif

    u4Region = _u4Region;

    if(rSet.mask & MT53FB_SET_MASK_PLANE_ID)
    {
#if 0    
        if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_1) {
#ifdef CONFIG_OSD_COMPRESSION        	
            u4Region = _au4CompressedRegion[_fgCompressedOSDFlip?1:0];
#elif defined(CC_CONFIG_OSD_SBS)
            u4Region = _au4SbsRegion[_fgOSDSbsFlip?1:0];
#else
            u4Region = _u4Region;
#endif
        }
        else if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_2) {
#ifndef TWO_OSD_PLANE
            printk(KERN_WARNING "_mt53fb_set_ioctl() - Not support set plane 2. Plane 2 is for Android\n");
            return 0;
#endif
            u4Region = _u4Region2;
        }
        else
            VERIFY(0);
#else

if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_1) 
{
	#ifdef CONFIG_OSD_COMPRESSION			
	u4Region			= _au4CompressedRegion[0];
	u4RegionCompress2	= _au4CompressedRegion[1];
	u4_rgn_list 		=  _au4CompressedRegionList[_fgCompressedOSDFlip?1:0];;
	#elif defined(CC_CONFIG_OSD_SBS)
	u4Region	= _au4SbsRegion[_fgOSDSbsFlip?1:0];
	u4_rgn_list = _au4SbsRegionList[_fgOSDSbsFlip?1:0];
	#else
	u4Region	= _u4Region;
	u4_rgn_list = _u4RegionList;
	#endif
}
else if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_2) 
{
	#ifndef TWO_OSD_PLANE
	printk(KERN_WARNING "_mt53fb_set_ioctl() - Not support set plane 2. Plane 2 is for Android\n");
	return 0;
	#endif
	u4Region	= _u4Region2;
	u4_rgn_list = _u4RegionList2;
}
else
	VERIFY(0);


#endif
    }
    
    if(rSet.mask & MT53FB_SET_MASK_X)
    {
        if(OSD_RGN_Set(u4Region, OSD_RGN_POS_X, rSet.rSetting.u2X) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_POS_X fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_X_ERR;
        }
#ifdef CONFIG_OSD_COMPRESSION           
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2X = rSet.rSetting.u2X;
#elif defined(CC_CONFIG_OSD_SBS)
       _arSbsSetting.u2X = rSet.rSetting.u2X;
#endif
    }
    if(rSet.mask & MT53FB_SET_MASK_Y)
    {
        if(OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, rSet.rSetting.u2Y) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_POS_Y fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_Y_ERR;
        }
#ifdef CONFIG_OSD_COMPRESSION           
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2Y = rSet.rSetting.u2Y;   
#elif defined(CC_CONFIG_OSD_SBS)
        _arSbsSetting.u2Y = rSet.rSetting.u2Y;

#endif
    }
    if(rSet.mask & MT53FB_SET_MASK_W)
    {
        if(OSD_RGN_Set(u4Region, OSD_RGN_BMP_W, rSet.rSetting.u2W) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_BMP_W fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_WIDTH_ERR;
        }
        else
        {
            if(OSD_RGN_Set(u4Region, OSD_RGN_DISP_W, rSet.rSetting.u2W) != OSD_RET_OK)
            {
                printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_DISP_W fail\n", u4Region);
                i4Ret = -E_GFXLIB_SET_WIDTH_ERR;
            }
            else
            {
                //_fb_info.var.xres = rSet.rSetting.u2W;
            }
        }
#ifdef CONFIG_OSD_COMPRESSION           
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2W = rSet.rSetting.u2W;  
#elif defined(CC_CONFIG_OSD_SBS)
        _arSbsSetting.u2W = rSet.rSetting.u2W;
#endif
    }
    if(rSet.mask & MT53FB_SET_MASK_H)
    {
        if(OSD_RGN_Set(u4Region, OSD_RGN_BMP_H, rSet.rSetting.u2H) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_BMP_H fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_HEIGHT_ERR;
        }
        else
        {
            if(OSD_RGN_Set(u4Region, OSD_RGN_DISP_H, rSet.rSetting.u2H) != OSD_RET_OK)
            {
                printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_DISP_H fail\n", u4Region);
                i4Ret = -E_GFXLIB_SET_HEIGHT_ERR;
            }
            else
            {
                //_fb_info.var.yres = rSet.rSetting.u2H;
            }
        }
#ifdef CONFIG_OSD_COMPRESSION           
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2H = rSet.rSetting.u2H; 
#elif defined(CC_CONFIG_OSD_SBS)
        _arSbsSetting.u2H = rSet.rSetting.u2H;
#endif
    }
    
    if(rSet.mask & MT53FB_SET_MASK_H && rSet.mask & MT53FB_SET_MASK_W)
    {
        UINT16 u2PanelW;
        UINT16 u2PanelH;
        u2PanelW = PANEL_GetPanelWidth();
        u2PanelH = PANEL_GetPanelHeight();
        
        if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_1) 
        {
            if (_fgOsdScEnable[OSD_SCALER1])
            {
#ifdef OSD_PLANE1_SCALE_TO_FULLSCREEN        
            OSD_SC_Scale(OSD_SCALER1, TRUE, rSet.rSetting.u2W, rSet.rSetting.u2H,
                (osd1_output_width==0)?u2PanelW:osd1_output_width, (osd1_output_height==0)?u2PanelH:osd1_output_height);
            printk("mt53fb_set osd w: %d, h: %d\n", rSet.rSetting.u2W, rSet.rSetting.u2H);
#endif
        }
             else
             {
                 OSD_SC_Scale(OSD_SCALER1, TRUE, u2PanelW, u2PanelH, u2PanelW, u2PanelH);             
             }
        }
        else if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_2)
        {
            if (_fgOsdScEnable[OSD_SCALER2])        
            {
#ifdef OSD_PLANE2_SCALE_TO_FULLSCREEN                
            OSD_SC_Scale(OSD_SCALER2, TRUE, rSet.rSetting.u2W, rSet.rSetting.u2H,
                u2PanelW, u2PanelH);    
#endif
        }
        else
        {
                OSD_SC_Scale(OSD_SCALER2, TRUE, u2PanelW, u2PanelH, u2PanelW, u2PanelH);                
            }
        }
        else
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - Error OSD plane ID to change resolution\n");
            i4Ret = -E_GFXLIB_SET_ChangeResolution_ERR;
        }
    }

    if(rSet.mask & MT53FB_SET_MASK_CM)
    {
        static UINT32 u4ColorMode;
        //UINT8 u1Bpp;
        if(rSet.rSetting.u1CM == CM_ARGB8888_DIRECT32)
        {
            u4ColorMode = OSD_CM_ARGB8888_DIRECT32;
        }
        else if(rSet.rSetting.u1CM == CM_ARGB4444_DIRECT16)
        {
            u4ColorMode = OSD_CM_ARGB4444_DIRECT16;         
        }
        else if(rSet.rSetting.u1CM == CM_RGB565_DIRECT16)
        {
            u4ColorMode = OSD_CM_RGB565_DIRECT16;         
        }        
        else if(rSet.rSetting.u1CM == CM_RGB_CLUT8)
        {
            u4ColorMode = OSD_CM_RGB_CLUT8;            
        }
        else if(rSet.rSetting.u1CM == CM_AYCbCr8888_DIRECT32)
        {
            //printk("SET CM TO PLANE AS YCBCR :%d\n", rSet.rSetting.u4OsdPlaneID);
            u4ColorMode = OSD_CM_AYCBCR8888_DIRECT32;          
        }        
        else
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) not support colormode = %d\n", u4Region, u4ColorMode);
            i4Ret = -E_GFXLIB_SET_BPP_ERR;
        }

        if(OSD_RGN_Set(u4Region, OSD_RGN_COLORMODE, u4ColorMode) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_COLORMODE fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_COLORMODE_ERR;
        }
        //_fb_info.var.bits_per_pixel = u1Bpp;       
#ifdef CONFIG_OSD_COMPRESSION           
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u1CM = rSet.rSetting.u1CM;  
#elif defined(CC_CONFIG_OSD_SBS)
        _arSbsSetting.u1CM = rSet.rSetting.u1CM;
#endif
    }
    if(rSet.mask & MT53FB_SET_MASK_PITCH)
    {
        if(OSD_RGN_Set(u4Region, OSD_RGN_BMP_PITCH, rSet.rSetting.u4Pitch) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_BMP_PITCH fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_PITCH_ERR;
        }
        else
        {
            //_fb_info.pixmap.addr = rSet.rSetting.u4Base;
        }
#ifdef CONFIG_OSD_COMPRESSION           
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u4Pitch = rSet.rSetting.u4Pitch;
#elif defined(CC_CONFIG_OSD_SBS)
        _arSbsSetting.u4Pitch = rSet.rSetting.u4Pitch;
#endif
    }        
    if(rSet.mask & MT53FB_SET_MASK_BASE)
    {      
#ifdef CONFIG_OSD_COMPRESSION  
        if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_1) // Only plane 1 use compression
        {
     //#ifdef CC_MT8283_OSD_COMPRESSED
     #if 1
     	     struct mt53fb_compress rCompress;    
     	     OSD_OP_MODE_T OSD_OP_MODE;

     	     OSD_OP_MODE = OSD_DMA_GetMode();

            if(1) // (rSet.rSetting.u4_gfx_mmu)
			{
                rCompress.mmu.u4_init           = 0x0;
                rCompress.mmu.u4_enable         = 0x1;
                rCompress.mmu.u4_cmprss         = 0x1;
                rCompress.mmu.u4_src_rw_mmu     = 0x1;
                rCompress.mmu.u4_dst_rw_mmu     = 0x0;
                rCompress.mmu.u4_vgfx_ord       = 0x0;
                rCompress.mmu.u4_pgt            = 0x1;
            }
			
			
	     if(OSD_OP_MODE==OSD_OP_DMA)
	     {
            // Flip
            _fgCompressedOSDFlip = !_fgCompressedOSDFlip;
            rCompress.eCm = _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u1CM;
            rCompress.u4Src = rSet.rSetting.u4Base;
            rCompress.u4Dst = (unsigned int)_apu1CompressedOSDBuf[_fgCompressedOSDFlip?1:0];// Global
            rCompress.u4DstX = 0;
            rCompress.u4DstY = 0;        
            rCompress.u4SrcX = 0;
            rCompress.u4SrcY = 0;           
            rCompress.u4Width = ((unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2W/2)+8;                
            rCompress.u4Height = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2H;                        
            rCompress.u4Pitch = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u4Pitch;
            
            mt53fb_compress(NULL, &rCompress);
		//printk("--COMP0- src:0x%x, dst:0x%x, W=%d,P=%d\n", rCompress.u4Src,rCompress.u4Dst,rCompress.u4Width,rCompress.u4Pitch);    
            rCompress.u4Src = rSet.rSetting.u4Base+(((unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2W/2)-8)*4;
            rCompress.u4Dst = (unsigned int)_apu1CompressedOSDBuf[_fgCompressedOSDFlip?1:0]+(rCompress.u4Pitch*rCompress.u4Height/2);
            mt53fb_compress(NULL, &rCompress);
            //printk("--COMP1- src:0x%x, dst:0x%x, W=%d\n", rCompress.u4Src,rCompress.u4Dst,rCompress.u4Width);   
            if(!_afgCompressEnable[_fgCompressedOSDFlip?1:0])
            {
                _afgCompressEnable[_fgCompressedOSDFlip?1:0] = TRUE;
                OSD_RGN_Set(_au4CompressedRegion[_fgCompressedOSDFlip?1:0], (INT32)OSD_RGN_DEC_EN, 1);                            
            }
            VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4CompressedRegionList[_fgCompressedOSDFlip?1:0] ) == 0);
            OSD_DMA_TransferRegion(OSD_PLANE1);
         	}
	       else
	       {
	       	       _fgCompressedOSDFlip = !_fgCompressedOSDFlip;
            		rCompress.eCm = _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u1CM;
            		rCompress.u4Src = rSet.rSetting.u4Base;
            		rCompress.u4Dst = (unsigned int)_apu1CompressedOSDBuf[_fgCompressedOSDFlip?1:0];// Global
            		rCompress.u4DstX = 0;
            		rCompress.u4DstY = 0;        
            		rCompress.u4SrcX = 0;
            		rCompress.u4SrcY = 0;           
            		rCompress.u4Width = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2W;                
            		rCompress.u4Height = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2H;                        
            		rCompress.u4Pitch = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u4Pitch;
            		mt53fb_compress(NULL, &rCompress);
            		if(!_afgCompressEnable[_fgCompressedOSDFlip?1:0])
            		{
                		_afgCompressEnable[_fgCompressedOSDFlip?1:0] = TRUE;
                		OSD_RGN_Set(_au4CompressedRegion[_fgCompressedOSDFlip?1:0], (INT32)OSD_RGN_DEC_EN, 1);                            
            		}
            		VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4CompressedRegionList[_fgCompressedOSDFlip?1:0] ) == 0);
	       }
     #else
            struct mt53fb_compress rCompress;                    
            // Flip
            _fgCompressedOSDFlip = !_fgCompressedOSDFlip;
            rCompress.eCm = _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u1CM;
            rCompress.u4Src = rSet.rSetting.u4Base;
            rCompress.u4Dst = _apu1CompressedOSDBuf[_fgCompressedOSDFlip?1:0];// Global
            rCompress.u4DstX = 0;
            rCompress.u4DstY = 0;        
            rCompress.u4SrcX = 0;
            rCompress.u4SrcY = 0;           
            rCompress.u4Width = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2W;                
            rCompress.u4Height = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u2H;                        
            rCompress.u4Pitch = (unsigned int)_arSavedSetting[rSet.rSetting.u4OsdPlaneID].u4Pitch;
            mt53fb_compress(NULL, &rCompress);
            if(!_afgCompressEnable[_fgCompressedOSDFlip?1:0])
            {
                _afgCompressEnable[_fgCompressedOSDFlip?1:0] = TRUE;
                OSD_RGN_Set(_au4CompressedRegion[_fgCompressedOSDFlip?1:0], (INT32)OSD_RGN_DEC_EN, 1);                            
            }
            VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4CompressedRegionList[_fgCompressedOSDFlip?1:0] ) == 0);
     #endif     
        }
        else
        {
            if(OSD_RGN_Set(u4Region, OSD_RGN_BMP_ADDR, rSet.rSetting.u4Base) != OSD_RET_OK)
            {
                printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_BMP_ADDR fail\n", u4Region);
                i4Ret = -E_GFXLIB_SET_ADDR_ERR;
            }
            else
            {
                //_fb_info.pixmap.addr = rSet.rSetting.u4Base;
            }            
        }
        _arSavedSetting[rSet.rSetting.u4OsdPlaneID].u4Base = rSet.rSetting.u4Base;  
#elif defined(CC_CONFIG_OSD_SBS)
       if(rSet.rSetting.u4OsdPlaneID == OSD_PLANE_1)
		{
		  _fgOSDSbsFlip = !_fgOSDSbsFlip;
		  OSD_PLA_Set3DDFB(OSD_PLANE1, 0, 0, DFB_UI_WIDTH, DFB_UI_HEIGHT, DFB_UI_WIDTH*4, 
						   (UINT32)_apu1OSDSbsBuf[_fgOSDSbsFlip?1:0], OSD_CM_ARGB8888_DIRECT32, rSet.rSetting.u4Base);

		  VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4SbsRegionList[_fgOSDSbsFlip?1:0] ) == 0);   
		}
      else
        {
            if(OSD_RGN_Set(u4Region, OSD_RGN_BMP_ADDR, rSet.rSetting.u4Base) != OSD_RET_OK)
            {
                printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_BMP_ADDR fail\n", u4Region);
                i4Ret = -E_GFXLIB_SET_ADDR_ERR;
            }
            else
            {
                //_fb_info.pixmap.addr = rSet.rSetting.u4Base;
            }            
        }
#else
        if(OSD_RGN_Set(u4Region, OSD_RGN_BMP_ADDR, rSet.rSetting.u4Base) != OSD_RET_OK)
        {
            printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set OSD_RGN_BMP_ADDR fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_ADDR_ERR;
        }
        else
        {
            //_fb_info.pixmap.addr = rSet.rSetting.u4Base;
        }    
#endif        
    }    
    if(rSet.mask & MT53FB_SET_MASK_VISIBLE)
    {
        if( -1 == OSD_PLA_GetFirstRegion(rSet.rSetting.u4OsdPlaneID + 1))
        {
            OSD_PLA_FlipTo((rSet.rSetting.u4OsdPlaneID + 1),u4_rgn_list);
        }    
#ifdef LINUX_TURNKEY_SOLUTION
        if(OSD_PLA_Enable(rSet.rSetting.u4OsdPlaneID + 1, rSet.rSetting.fgVisible) != OSD_RET_OK)
#else
        if(OSD_PLA_Enable(_au1Log2HwPlane[rSet.rSetting.u4OsdPlaneID + 2] - 2, rSet.rSetting.fgVisible) != OSD_RET_OK)
#endif
        {
            //printk(KERN_WARNING "_mt53fb_set_ioctl() - rgn(%d) set MT53FB_SET_MASK_VISIBLE fail\n", u4Region);
            i4Ret = -E_GFXLIB_SET_ENABLE_ERR;
        }
        else
        {
            _fb_info.var.activate = 1;
        }
    }

#if defined(CONFIG_OSD_COMPRESSION) || defined(CC_CONFIG_OSD_SBS)
    x_sema_unlock(hMt53fbMutex);         
#endif
    
    if(rSet.mask & MT53FB_SET_MASK_OPACITY)
    {
#ifdef FB_HARD_FADING        
       /*CHUN: We don't allow DFB wm change Window layer opacity. FIXME*/
       
  //      if(rSet.rSetting.u4OsdPlaneID != 0)        //for DTV00084581
        {       
#endif
    #ifdef LINUX_TURNKEY_SOLUTION
            if(OSD_PLA_SetFading(rSet.rSetting.u4OsdPlaneID + 1, rSet.rSetting.u4Opacity) != OSD_RET_OK)
    #else
            if(OSD_PLA_SetFading(_au1Log2HwPlane[rSet.rSetting.u4OsdPlaneID + 2] - 2, rSet.rSetting.u4Opacity) != OSD_RET_OK)
    #endif
            {
                return -EINVAL;
            }
#ifdef FB_HARD_FADING                
        }
#endif
    }  

    if(rSet.mask & MT53FB_SET_MASK_MIXSEL)
    {    
        if(OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, rSet.rSetting.u4MixSel) != OSD_RET_OK)
        {
            return -EINVAL;
        }
    }
    
    if(rSet.mask & MT53FB_SET_MASK_COLORKEYEN)
    {    
        if(OSD_RGN_Set(u4Region, OSD_RGN_COLOR_KEY_EN, rSet.rSetting.u4ColorKeyEn) != OSD_RET_OK)
        {
            return -EINVAL;
        }
    }
    
    if(rSet.mask & MT53FB_SET_MASK_COLORKEY)
    {   
        if(OSD_RGN_Set(u4Region, OSD_RGN_COLOR_KEY, rSet.rSetting.u4ColorKey) != OSD_RET_OK)
        {
            return -EINVAL;
        }
    }       

#if 0
    if(i4Ret == 0)
    {   
        HalFlushInvalidateDCache();
    }
#endif

    return i4Ret;
}

static int _mt53fb_get_imagebuffer_ioctl(void* pvArg)
{
    struct mt53fb_imagebuffer rBuf; 
    rBuf.u4Size = _rImgBufInfo.u4Size;
    rBuf.u4PhyAddr = _rImgBufInfo.u4PhyAddr;
    rBuf.u4VirtAddr = _rImgBufInfo.u4VirtAddr;
    if(copy_to_user(pvArg, (void*)&rBuf, sizeof(struct mt53fb_imagebuffer)))
    {
        //printk(KERN_WARNING "_mt53fb_get_imagebuffer_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    return 0;
}

static int _mt53fb_get_framebuffer_ioctl(void* pvArg)
{
    struct mt53fb_imagebuffer rBuf; 
    rBuf.u4Size = (unsigned int) _fb_info.fix.smem_len;
    rBuf.u4PhyAddr = (unsigned int) _fb_info.fix.smem_start;
    rBuf.u4VirtAddr = (unsigned int) _fb_info.screen_base;
    if(copy_to_user(pvArg, (void*)&rBuf, sizeof(struct mt53fb_imagebuffer)))
    {
        //printk(KERN_WARNING "_mt53fb_get_imagebuffer_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    return 0;
}

#ifdef CC_FBM_TWO_FBP_SHARED_WITH_DFB 
static int _mt53fb_get_imagebuffer2_ioctl(void* pvArg)
{
    struct mt53fb_imagebuffer rBuf; 
    rBuf.u4Size = _rImgBufInfo2.u4Size;
    rBuf.u4PhyAddr = _rImgBufInfo2.u4PhyAddr;
    rBuf.u4VirtAddr = _rImgBufInfo2.u4VirtAddr;
    if(copy_to_user(pvArg, (void*)&rBuf, sizeof(struct mt53fb_imagebuffer)))
    {
        //printk(KERN_WARNING "_mt53fb_get_imagebuffer_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    return 0;
}
#endif /* CC_FBM_TWO_FBP_SHARED_WITH_DFB */
#ifdef CC_DFB_SUPPORT_VDP_LAYER
static int _mt53fb_get_vdpbuffer_ioctl(void* pvArg)
{
    struct mt53fb_imagebuffer rBuf; 
    rBuf.u4Size = _rVdpBufInfo.u4Size;
    rBuf.u4PhyAddr = _rVdpBufInfo.u4PhyAddr;
    rBuf.u4VirtAddr = _rVdpBufInfo.u4VirtAddr;
    if(copy_to_user(pvArg, (void*)&rBuf, sizeof(struct mt53fb_imagebuffer)))
    {
        //printk(KERN_WARNING "_mt53fb_get_imagebuffer_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    return 0;
}

static int _mt53fb_get_vdp_crop_rect_ioctl(void* pvArg)
{
    mt53fb_vdp_crop_rect  t_s_rect; 
    
    if(copy_from_user(&t_s_rect, pvArg, sizeof(mt53fb_vdp_crop_rect)))
    {
        return -EFAULT;
    }
    
    if(t_s_rect.ui4_layer > DFB_VDP_LAYER_MAX )
    {
         return -EFAULT;
    }
    
    if(copy_to_user(pvArg,&mt53fb_vdp_crop_rect_array[t_s_rect.ui4_layer], 
        sizeof(mt53fb_vdp_crop_rect)))
    {
        return -EFAULT;
    }
    
    return 0;
}

static int _mt53fb_set_vdp_crop_rect_ioctl(void* pvArg)
{
    mt53fb_vdp_crop_rect  t_s_rect; 
    
    if(copy_from_user(&t_s_rect, pvArg, sizeof(mt53fb_vdp_crop_rect)))
    {
        return -EFAULT;
    }
    
    if(t_s_rect.ui4_layer > DFB_VDP_LAYER_MAX )
    {
         return -EFAULT;
    }
    
    memcpy(&mt53fb_vdp_crop_rect_array[t_s_rect.ui4_layer], &t_s_rect, sizeof(mt53fb_vdp_crop_rect));

    // Set 3D crop offset
    _u4CropBottomOffset = t_s_rect.crop_bottom_offset;
    _u4CropTopOffset    = t_s_rect.crop_top_offset;        
    _u4CropLeftOffset   = t_s_rect.crop_left_offset;
    _u4CropRightOffset  = t_s_rect.crop_right_offset; 
	
    if(MT53FB_LOG>=0x2)
	{
        printk("Crop Rect[%d,%d,%d,%d]\n",
                _u4CropLeftOffset,_u4CropTopOffset,
                _u4CropRightOffset,_u4CropBottomOffset);
    }
	
    return 0;
}

static INT32 _mt53fb_vdp_stretch_blit_ioctl(void* pvArg)
{
    mt53fb_vdp_stretch_blit rVdpStretchBlit;

    if(copy_from_user((void*)&rVdpStretchBlit, pvArg, sizeof(mt53fb_vdp_stretch_blit)))
    {
        printk(KERN_WARNING "_mt53fb_vdp_stretch_blit_ioctl() error: Copy from user error!\n");
        return -EFAULT;
    }

    return mt53fb_vdp_stretchblit(&rVdpStretchBlit); 
}
#endif


static int _mt53fb_get_plane_order_array_ioctl(void* pvArg)
{
    UINT32 i;
#ifdef LINUX_TURNKEY_SOLUTION
    UINT32 au4PlaneOrderArray[PMX_MAX_INPORT_NS];

    for(i = 0; i < PMX_MAX_INPORT_NS; i++)
    {
        
        au4PlaneOrderArray[i] = _au4PlaneOrderArray[i];
    }
    
    if(copy_to_user(pvArg, (void*)au4PlaneOrderArray, sizeof(UINT32) * PMX_MAX_INPORT_NS))
    {
        //printk(KERN_WARNING "_mt53fb_get_plane_order_array_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
#else
    UINT32 au4PlaneOrderArray[SIZE_PLANE_ORDER_ARRAY];
    for(i = 0; i < SIZE_PLANE_ORDER_ARRAY; i++)
    {
        au4PlaneOrderArray[i] = _au1Hw2LogPlane[_au4PlaneOrderArray[i]];
    }
    if(copy_to_user(pvArg, (void*)au4PlaneOrderArray, sizeof(UINT32) * SIZE_PLANE_ORDER_ARRAY))
    {
        printk(KERN_WARNING "_mt53fb_get_plane_order_array_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
#endif
    return 0;
}

static int _mt53fb_set_plane_order_array_ioctl(void* pvArg)
{
    UINT32 i;
#ifdef LINUX_TURNKEY_SOLUTION
    UINT32 au4PlaneOrderArray[PMX_MAX_INPORT_NS];
    
    if(copy_from_user( (void*)au4PlaneOrderArray, pvArg, sizeof(UINT32) * PMX_MAX_INPORT_NS))
    {
        //printk(KERN_WARNING "_mt53fb_get_plane_order_array_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    for(i = 0; i < PMX_MAX_INPORT_NS; i++)
    {
        //printk("[%d] User: %d,\n",i, au4PlaneOrderArray[i]);
        _au4PlaneOrderArray[i] = au4PlaneOrderArray[i];
    }
#else
    UINT32 au4PlaneOrderArray[SIZE_PLANE_ORDER_ARRAY];

    if(copy_from_user( (void*)au4PlaneOrderArray, pvArg, sizeof(UINT32) * SIZE_PLANE_ORDER_ARRAY))
    {
        printk(KERN_WARNING "_mt53fb_get_plane_order_array_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    for(i = 0; i < SIZE_PLANE_ORDER_ARRAY; i++)
    {
        printk("[%d] User: %d, Kernel %d\n",i, au4PlaneOrderArray[i],_au1Log2HwPlane[au4PlaneOrderArray[i]]);
        _au4PlaneOrderArray[i] = _au1Log2HwPlane[au4PlaneOrderArray[i]];
    }
#endif

    if (PMX_SetPlaneOrderArray(_au4PlaneOrderArray) != PMX_SET_OK)
    {
        //printk(KERN_WARNING "_mt53fb_get_plane_order_array_ioctl() - PMX_SetPlaneOrderArray() fail\n");    
        return -EINVAL;
    }      

    // Enable mixing
    i = IO_READ32(POST_PROC_BASE, 0x5C0);
    #ifdef CC_MT5391
    IO_WRITE32(POST_PROC_BASE, 0x5C0, i | 0x02020202);
    #else
    	#ifdef CC_MT5360
    	IO_WRITE32(POST_PROC_BASE, 0x5C0, i | 0x02020202);
    	#else
    	IO_WRITE32(POST_PROC_BASE, 0x5C0, i | 0x00020202);
    	#endif
    #endif
#ifdef LINUX_TURNKEY_SOLUTION
    //DumpPanelAttribute(PANEL_DUMP_CURRENT);
#endif
    return 0;
}


static int _mt53fb_set_osd_plane_order_ioctl(void* pvArg)
{
    UINT32 i;
    UINT32 au4PlaneOrderArray[SIZE_PLANE_ORDER_ARRAY];
    
    _au4PlaneOrderArray[0] = 5;
    
    if(copy_from_user( (void*)au4PlaneOrderArray, pvArg, sizeof(UINT32) * SIZE_PLANE_ORDER_ARRAY))
    {
        //printk(KERN_WARNING "_mt53fb_set_osd_plane_order_ioctl() - copy_to_user() fail\n");
        return -EFAULT;
    }
    
    for(i = 0; i < SIZE_PLANE_ORDER_ARRAY-1; i++)
    {
        //printk("%d %d\n",au4PlaneOrderArray[i],_au1Log2HwPlane[au4PlaneOrderArray[i]]);
        _au4PlaneOrderArray[i+1] = _au1Log2HwPlane[au4PlaneOrderArray[i]];
    }

    //printk("plane mixer order: %d %d %d %d\n",_au4PlaneOrderArray[0],_au4PlaneOrderArray[1],_au4PlaneOrderArray[2],_au4PlaneOrderArray[3]);

    if (PMX_SetPlaneOrderArray(_au4PlaneOrderArray) != PMX_SET_OK)
    {
        return -EINVAL;
    }
    return 0;
}

#ifdef LINUX_TURNKEY_SOLUTION
/* MHF Linux - CHUN for called by PMX*/
int _mt53fb_get_plane_order_HW_array(void * pvArg)
{
    //UINT32 i;
    
    memcpy(pvArg, (void*)_au4PlaneOrderArray, sizeof(UINT32) * PMX_MAX_INPORT_NS);
/*
    for(i = 0; i < PMX_MAX_INPORT_NS; i++)
    {
        printk("PMX 2 DFB Get ARRAY: %d, %d \n",i, _au4PlaneOrderArray[i]);        
    }
*/
    return 0;
}

int _mt53fb_set_plane_order_HW_array(void * pvArg)
{
    //UINT32 i;
    
    memcpy( (void*)_au4PlaneOrderArray, pvArg, sizeof(UINT32) * PMX_MAX_INPORT_NS);
/* 
    for(i = 0; i < PMX_MAX_INPORT_NS; i++)
    {
        printk("PMX 2 DFB Set ARRAY: %d, %d \n",i, _au4PlaneOrderArray[i]);
        
    }
*/
    if (PMX_SetPlaneOrderArray(_au4PlaneOrderArray) != PMX_SET_OK)
    {
        return -EINVAL;
    }  return 0;    
}
/* MHF Linux - CHUN modify end*/

EXTERN UINT8 bSI_DISPLAY_DCLK_TYPE;

UINT32 VDP_SetArg(UCHAR ucVdpId, UCHAR ucArg, INT16 i2Value);
#endif

static int _mt53fb_init_hw(void* pvArg)
{
    /*
    initialize hardware
    */

    UINT32 u4BytesPP;
    static UINT32 u4ColorMode;
    UINT16 u2PanelW;
    UINT16 u2PanelH;

    struct fb_var_screeninfo rFbVar;

#ifdef LINUX_TURNKEY_SOLUTION
    static int single_inited = 0;
    
/* Chun, for simple init , NULL pvArg is defualt,.
   And we need to support FB driver without DirectFB.
   That means this API could be called multiple times.
*/
    if(pvArg == NULL)
    {
        rFbVar.xres = (osd1_src_width==0)?1280:osd1_src_width;
        rFbVar.yres = (osd1_src_height==0)?720:osd1_src_height;
        rFbVar.xres_virtual = rFbVar.xres;
        rFbVar.yres_virtual = rFbVar.yres;
        rFbVar.xoffset = 0;
        rFbVar.yoffset = 0;
        rFbVar.bits_per_pixel = 32;
        rFbVar.vmode = CM_ARGB8888_DIRECT32;    
    }
    else
    {
#endif
        if(copy_from_user((void*)&rFbVar, pvArg, sizeof(struct fb_var_screeninfo)))
        {
            printk(KERN_ERR "_mt53fb_init_hw() - copy_to_user() fail\n");    	
            VERIFY(0);
        }
#ifdef LINUX_TURNKEY_SOLUTION
    }
#endif
    if(_mt53fb_check_var(&rFbVar, &_fb_info) != 0)
    {
    	printk(KERN_ERR "_mt53fb_check_var() fail\n");   
        VERIFY(0);
    }

    _fb_info.var.xres = rFbVar.xres;
    _fb_info.var.yres = rFbVar.yres;
    _fb_info.var.xres_virtual = rFbVar.xres_virtual;
    _fb_info.var.yres_virtual = rFbVar.yres_virtual;
    _fb_info.var.xoffset = rFbVar.xoffset;
    _fb_info.var.yoffset = rFbVar.yoffset;
    _fb_info.var.bits_per_pixel = rFbVar.bits_per_pixel;
    _fb_info.var.vmode = rFbVar.vmode;
    /*printk(" Init var :%d,%d,%d,%d,%d,%d,%d,%d\n"
        ,rFbVar.xres
        ,rFbVar.yres
        ,rFbVar.xres_virtual
        ,rFbVar.yres_virtual
        ,rFbVar.xoffset
        ,rFbVar.yoffset
        ,rFbVar.bits_per_pixel
        ,rFbVar.vmode);*/
    if(rFbVar.vmode == CM_ARGB8888_DIRECT32)
    {
        u4ColorMode = OSD_CM_ARGB8888_DIRECT32;
        rFbVar.bits_per_pixel = 32;
    }
    else if(rFbVar.vmode == CM_ARGB4444_DIRECT16)
    {
        u4ColorMode = OSD_CM_ARGB4444_DIRECT16;
        rFbVar.bits_per_pixel = 16;            
    }
    else if(rFbVar.vmode == CM_RGB565_DIRECT16)
    {
        u4ColorMode = OSD_CM_RGB565_DIRECT16;
        rFbVar.bits_per_pixel = 16;            
    }        
    else if(rFbVar.vmode == CM_RGB_CLUT8)
    {
        u4ColorMode = OSD_CM_RGB_CLUT8;
        rFbVar.bits_per_pixel = 8;            
    }
    else if(rFbVar.vmode == CM_AYCbCr8888_DIRECT32)
    {
        u4ColorMode = OSD_CM_AYCBCR8888_DIRECT32;
        rFbVar.bits_per_pixel = 32;            
    }        
    else
    {
        //printk(KERN_ERR "_mt53fb_init_hw() - bad color mode = %d\n", rFbVar.vmode);     	
        VERIFY(0);
    }
    _fb_info.fix.line_length=(rFbVar.xres * rFbVar.bits_per_pixel) / 8;
    
    u4BytesPP = rFbVar.bits_per_pixel >> 3;

#ifdef LINUX_TURNKEY_SOLUTION
    if(!single_inited)
    {
#if 0
/* liang.zhang Apr/29/2009, MW_SelectPanel() Firstly */
        printk("Try to light screen :-(\n");
        // Should Check !!
        //#if 1 //#ifdef PANEL_SELECT    
        FBM_Init();
       
        OSD_Init();        
        PMX_Init();

        {
            GPIO_Init();
           	 LoadPanelIndex();
             // it's a very dirty hack here...
             // bcuz vDrvSetLCDTiming wiil choose 50Hz as default
            SelectPanel(0);
            bSI_DISPLAY_DCLK_TYPE = 2;

            WritePanelIndexToEeprom(0);
            vApiPanelPowerSequence(FALSE);

            vDrvDisplayInit();
            vDrvVOFreqSet(PANEL_GetPixelClk60Hz());
            vDrvSetLCDTiming();
            OSTG_OnOutputVSync();
            IGNORE_RET(OSD_UpdateTiming());

            
            vApiPanelPowerSequence(TRUE);

            DumpPanelAttribute(PANEL_DUMP_CURRENT);
            CKGEN_SetVOPLLInputAsDDDS(FALSE);
        }
        single_inited = 1;
        #endif
    }        
#endif

    //PMX_GetResolution(0, &u2PanelW, &u2PanelH);
    u2PanelW = PANEL_GetPanelWidth();
    u2PanelH = PANEL_GetPanelHeight();
    
#ifdef TWO_OSD_PLANE
#ifdef LINUX_TURNKEY_SOLUTION
    printk(" L:::::} %d, %d\n", _u4RegionList2, _u4Region2);
    if(_u4RegionList2 == NULL_REGION_IDX)
    {
        printk(" Create Region List\n");
        VERIFY(OSD_RGN_LIST_Create(&_u4RegionList2) == 0);
    }

    if(_u4Region2 != NULL_REGION_IDX)
    {
        printk(" Remove old Region\n");
        OSD_RGN_Detach(_u4Region2, _u4RegionList2);
        OSD_RGN_Delete(_u4Region2);
    }
    printk(" {XXXXX} %d, %d\n", _u4RegionList2, _u4Region2);    
#endif /* LINUX_TURNKEY_SOLUTION */
    VERIFY(OSD_RGN_Create(&_u4Region2, rFbVar.xres, rFbVar.yres,
       (void *)_fb_info.fix.smem_start,
       u4ColorMode, rFbVar.xres_virtual * (rFbVar.bits_per_pixel >> 3),
       0, 0, rFbVar.xres, rFbVar.yres) == 0);    

    //if(rFbVar.bits_per_pixel == 8)
    {
        VERIFY(OSD_RGN_Set(_u4Region2, OSD_RGN_PAL_ADDR, (UINT32)_apu4Palette[OSD_PLANE_2]) == 0);
    }

    /* Set default to big endian */
    VERIFY(OSD_RGN_Set(_u4Region2, OSD_RGN_BIG_ENDIAN, 0) == 0);

    /* insert region to list*/
    VERIFY(OSD_RGN_Insert(_u4Region2, _u4RegionList2) == 0);

    /* flip plan to list */
    VERIFY(OSD_PLA_FlipTo(OSD_PLANE2, _u4RegionList2) == 0);

    /* Set default alpha to bitsel */
    VERIFY(OSD_RGN_Set(_u4Region2, (INT32)OSD_RGN_MIX_SEL,
                     (UINT32)OSD_BM_PIXEL) == 0);
#endif

#ifdef LINUX_TURNKEY_SOLUTION
    if(_u4RegionList == NULL_REGION_IDX)
    {
        VERIFY(OSD_RGN_LIST_Create(&_u4RegionList) == 0);
    }
    
    if(_u4Region != NULL_REGION_IDX)
    {
        OSD_RGN_Detach(_u4Region, _u4RegionList);
        OSD_RGN_Delete(_u4Region);
    }
#else
    /* new list */
    VERIFY(OSD_RGN_LIST_Create(&_u4RegionList) == 0);
    /* new region */
#endif

    VERIFY(OSD_RGN_Create(&_u4Region, rFbVar.xres, rFbVar.yres,
           (void *)_fb_info.fix.smem_start,
           u4ColorMode, rFbVar.xres_virtual * (rFbVar.bits_per_pixel >> 3),
           0, 0, rFbVar.xres, rFbVar.yres) == 0);

    //if(rFbVar.bits_per_pixel == 8)
    {
        VERIFY(OSD_RGN_Set(_u4Region, OSD_RGN_PAL_ADDR, (UINT32)_apu4Palette[OSD_PLANE_1]) == 0);
    }

    /* Set default to big endian */
    VERIFY(OSD_RGN_Set(_u4Region, OSD_RGN_BIG_ENDIAN, 0) == 0);

    /* insert region to list*/
    VERIFY(OSD_RGN_Insert(_u4Region, _u4RegionList) == 0);

#ifdef CONFIG_OSD_COMPRESSION    
    /* new list */
    VERIFY(OSD_RGN_LIST_Create(&_au4CompressedRegionList[0]) == 0);
    /* new region */

    VERIFY(OSD_RGN_Create(&_au4CompressedRegion[0], rFbVar.xres, rFbVar.yres,
           _apu1CompressedOSDBuf[0],
           u4ColorMode, rFbVar.xres_virtual * (rFbVar.bits_per_pixel >> 3),
           0, 0, rFbVar.xres, rFbVar.yres) == 0);

    /* Set default to big endian */
    VERIFY(OSD_RGN_Set(_au4CompressedRegion[0], OSD_RGN_BIG_ENDIAN, 0) == 0);

    /* insert region to list*/
    VERIFY(OSD_RGN_Insert(_au4CompressedRegion[0], _au4CompressedRegionList[0]) == 0);    	
    
    /* Set default alpha to bitsel */
    VERIFY(OSD_RGN_Set(_au4CompressedRegion[0], (INT32)OSD_RGN_MIX_SEL,
                       (UINT32)OSD_BM_PIXEL) == 0);

    /* new list */
    VERIFY(OSD_RGN_LIST_Create(&_au4CompressedRegionList[1]) == 0);
    /* new region */

    VERIFY(OSD_RGN_Create(&_au4CompressedRegion[1], rFbVar.xres, rFbVar.yres,
           _apu1CompressedOSDBuf[1],
           u4ColorMode, rFbVar.xres_virtual * (rFbVar.bits_per_pixel >> 3),
           0, 0, rFbVar.xres, rFbVar.yres) == 0);

    /* Set default to big endian */
    VERIFY(OSD_RGN_Set(_au4CompressedRegion[1], OSD_RGN_BIG_ENDIAN, 0) == 0);

    /* insert region to list*/
    VERIFY(OSD_RGN_Insert(_au4CompressedRegion[1], _au4CompressedRegionList[1]) == 0);            
    
    /* Set default alpha to bitsel */
    VERIFY(OSD_RGN_Set(_au4CompressedRegion[1], (INT32)OSD_RGN_MIX_SEL,
                       (UINT32)OSD_BM_PIXEL) == 0);    

    VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4CompressedRegionList[0]) == 0);
#elif defined(CC_CONFIG_OSD_SBS)
    
    VERIFY(OSD_RGN_LIST_Create(&_au4SbsRegionList[0]) == 0);
    /* new region */

    VERIFY(OSD_RGN_Create(&_au4SbsRegion[0], rFbVar.xres, rFbVar.yres,
           _apu1OSDSbsBuf[0],
           u4ColorMode, rFbVar.xres_virtual * (rFbVar.bits_per_pixel >> 3),
           0, 0, rFbVar.xres, rFbVar.yres) == 0);

    /* Set default to big endian */
    VERIFY(OSD_RGN_Set(_au4SbsRegion[0], OSD_RGN_BIG_ENDIAN, 0) == 0);

    /* insert region to list*/
    VERIFY(OSD_RGN_Insert(_au4SbsRegion[0], _au4SbsRegionList[0]) == 0);    	
    
    /* Set default alpha to bitsel */
    VERIFY(OSD_RGN_Set(_au4SbsRegion[0], (INT32)OSD_RGN_MIX_SEL,
                       (UINT32)OSD_BM_PIXEL) == 0);
	
	VERIFY(OSD_RGN_LIST_Create(&_au4SbsRegionList[1]) == 0);
    /* new region */

    VERIFY(OSD_RGN_Create(&_au4SbsRegion[1], rFbVar.xres, rFbVar.yres,
           _apu1OSDSbsBuf[1],
           u4ColorMode, rFbVar.xres_virtual * (rFbVar.bits_per_pixel >> 3),
           0, 0, rFbVar.xres, rFbVar.yres) == 0);

    /* Set default to big endian */
    VERIFY(OSD_RGN_Set(_au4SbsRegion[1], OSD_RGN_BIG_ENDIAN, 0) == 0);

    /* insert region to list*/
    VERIFY(OSD_RGN_Insert(_au4SbsRegion[1], _au4SbsRegionList[1]) == 0);    	
    
    /* Set default alpha to bitsel */
    VERIFY(OSD_RGN_Set(_au4SbsRegion[1], (INT32)OSD_RGN_MIX_SEL,
                       (UINT32)OSD_BM_PIXEL) == 0);
	
	VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _au4SbsRegionList[0]) == 0);
#else
    /* flip plan to list */
    VERIFY(OSD_PLA_FlipTo(OSD_PLANE1, _u4RegionList) == 0);
#endif

    VERIFY(OSD_Set_FrameBuffer_RGN_LIST(_u4RegionList) == 0); //notify OSD driver the region list of frame buffer

    /* Set default alpha to bitsel */
    VERIFY(OSD_RGN_Set(_u4Region, (INT32)OSD_RGN_MIX_SEL,
                       (UINT32)OSD_BM_PIXEL) == 0);

    #if !defined(CC_MT5396)
    /* clear screen */
    memset(_fb_info.screen_base, 0, _fb_info.fix.smem_len);
    #endif

#ifdef OSD_PLANE1_SCALE_TO_FULLSCREEN
    OSD_SC_Scale(OSD_SCALER1, TRUE, rFbVar.xres, rFbVar.yres,
                     (osd1_output_width==0)?u2PanelW:osd1_output_width, (osd1_output_height==0)?u2PanelH:osd1_output_height);
    printk("osd1 src_w: %d, src_h: %d, out_w: %d, out_h: %d\n",
rFbVar.xres, rFbVar.yres, (osd1_output_width==0)?u2PanelW:osd1_output_width, (osd1_output_height==0)?u2PanelH:osd1_output_height); 
#endif
#ifdef TWO_OSD_PLANE
#ifdef OSD_PLANE2_SCALE_TO_FULLSCREEN
    OSD_SC_Scale(OSD_SCALER2, TRUE, rFbVar.xres, rFbVar.yres,
                     u2PanelW, u2PanelH);     
#endif
#endif

#ifdef LINUX_TURNKEY_SOLUTION 
    printk("End init hw for FB\n");
#endif

    return 0;
}

//---------------------------------------------------------------------------
// Public interfaces
//---------------------------------------------------------------------------
//#ifdef LINUX_TURNKEY_SOLUTION /* MHF Linux - yjg 9x mmp */
unsigned int fb_convert_app_to_phy(unsigned int usr_addr)
{
	unsigned int kn_addr;
	
    kn_addr = usr_addr - 
					(unsigned int)fb_user_virt_start + 
					(unsigned int)_fb_info.fix.smem_start;
	return kn_addr;
}
//#endif

struct fb_info* mt53fb_get_fb_info(void)
{
    return &_fb_info;
}

struct mt53fb_par* mt53fb_get_par(void)
{
    return &_cur_par;
}

int __init _mt53fb_setup(char *options)
{
    struct options *optp;
    char *opt;

    _mt53fb_init_fbinfo();

    if (!options || !*options)
    {
        return 0;
    }

    while ((opt = strsep(&options, ",")) != NULL)
    {
        if (!*opt)
        {
            continue;
        }

        for (optp = _mt53fb_opt_table; optp->name; optp++)
        {
            int optlen;

            optlen = strlen(optp->name);

            if (strncmp(opt, optp->name, optlen) == 0 && opt[optlen] == ':')
            {
                optp->parse(opt + optlen + 1);
                break;
            }
        }

        if (!optp->name)
        {
            //printk(KERN_ERR "_mt53fb_setup(): unknown parameter: %s\n", opt);
        }
    }
    return 0;
}
  
int __init _mt53fb_init(void)
{
#ifdef LINUX_TURNKEY_SOLUTION
#ifndef TWO_OSD_PLANE    
    GPIO_Init();
    EEPROM_Init();
    FBM_Init();
    LoadPanelIndex();
    PDWNC_Init();
    VDEC_Init(); //???
    VDP_Init();
    VdoMLInit();
    SIF_Init();
#ifndef ANDROID
    /*comment here with ifndef ANDROID for video out issue*/
    VDP_SetScalerPixelBased(1);
#endif  
    RTC_Init();
    
    vApiPanelPowerSequence(1);
#endif
    PMX_Init();
    OSD_Init();
    GFX_Init(); 

    drv_extmjc_init();
    VERIFY(OSD_Reset() == 0);

    _mt53fb_probe(&_mt53fb_dev);
    _mt53fb_init_hw(NULL);
    // No DFB in android, so we disable OSD it
#ifndef ANDROID
    OSD_PLA_Enable(1, FALSE);
#endif
    
#ifdef TWO_OSD_PLANE
    OSD_PLA_Enable(2, FALSE);
#endif
    
#ifdef FB_HARD_FADING    
    /*  CHUN: In phase 2, this is needed for that appliaction which does not specify FADING FIXME */
    OSD_PLA_SetFading(1, 0xff);
    /*OSD_PLA_SetFading(2, 0xe5);*/
#endif
    PMX_SetPlaneOrderArray(_au4PlaneOrderArray);

#ifdef ANDROID
    {
        int ret;
        ret = mtk_drv_fb_init(&drv);
        if(ret)
            return ret;    
        mtk_fb_setup();
    }
#endif
    // Set GPU clock
    IO_WRITE32(0xF000D000, 0x314, 0xD);
    return 0;
#else
    return _mt53fb_probe(&_mt53fb_dev);
#endif
}

#ifdef LINUX_TURNKEY_SOLUTION
#ifdef MT53XX_FB_BUILDIN
module_init(_mt53fb_init);
#endif

int _mt53fb_wait_logo(void* pvArg)
{
    SIZE_T szSize = 0;
    BOOL fgFinished;

    fgFinished = TRUE;
    UNUSED(d_custom_misc_get(DRV_CUSTOM_MISC_TYPE_STARTUP_LOGO_STATUS,
        &fgFinished, &szSize));
    UNUSED(szSize);

    return 0;
}
#endif

int  _mt53fb_get_tmpbufaddr(void)
{
    return _u4TmpBufAddr;
}

int  mtk_fb_get_property(unsigned int* addr, unsigned int* size)
{
    unsigned int smem_start = (BSP_GetFbmMemAddr() - DRVCUST_OptGet(eDirectFBMemSize) - DRVCUST_OptGet(eFBMemSize)) & PAGE_MASK;
    unsigned int smem_len = DRVCUST_OptGet(eDirectFBMemSize) + DRVCUST_OptGet(eFBMemSize);

    if(smem_start == 0 || smem_len == 0)
    {
        return -1;
    }
    *addr = (unsigned int)smem_start;
    *size = (unsigned int)smem_len;
    return 0;
}

EXPORT_SYMBOL(mtk_fb_get_property);
