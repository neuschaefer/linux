#ifndef _MTKFB_H_
#define _MTKFB_H_

#include <linux/kernel.h>

#define MTK_DRV_FB_CM_RGB565 0
#define MTK_DRV_FB_CM_ARGB8888 1

typedef struct mtk_drv_fb_
{
    void (*fb_exit)();
    u32 (*get_line_length)(u32 width, u32 bits_per_pix);
    int (*setup_buffer)(
        u32 xres, 
        u32 yres, 
        u32 vyres,
        u32 bits_per_pix, 
        u32 cm,
        void ** p_screen_base,
        unsigned long ** p_phy_start,
        unsigned long ** p_size);
    int (*set_base)(unsigned long base_addr);
}mtk_drv_fb_t;

int mtk_drv_fb_init(mtk_drv_fb_t ** drv);

#endif
