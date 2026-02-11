/*****************************************************************************
 * Copyright 2011 Broadcom Corporation.  All rights reserved.
 *
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2, available at
 * http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a
 * license other than the GPL, without Broadcom's express prior written
 * consent.
 *****************************************************************************/

#ifndef _VC_FB_H_
#define _VC_FB_H_

#include <linux/fb.h>
#include <linux/proc_fs.h>

// Uncomment the following line to enable debug messages
//#define ENABLE_LOG_DBG

// Logging macros (for remapping to other logging mechanisms)
#ifdef ENABLE_LOG_DBG
#define LOG_DBG(fmt, arg... )  printk(KERN_INFO "[D] " "%s:"fmt"\n", __func__, ##arg )
#else
#define LOG_DBG(fmt, arg... )
#endif
#define LOG_INFO(fmt, arg... )  printk(KERN_INFO "[I] " "%s:"fmt "\n", __func__, ##arg )
#define LOG_ERR(fmt, arg... )  printk(KERN_ERR  "[E] " "%s:"fmt "\n", __func__, ##arg )

//#define VC_ROUND_UP_WH(wh)  (((wh) + 15) & ~15)

//TODO add to linux.config!!
#define CONFIG_FB_VC_DEFAULT_SCREEN_WIDTH     (800)
#define CONFIG_FB_VC_DEFAULT_SCREEN_HEIGHT    (480)
#define CONFIG_FB_VC_NUM_FRAMES               (3)

// Default values for framebuffer creation modifiable parameters
#define DEFAULT_ALPHA            (255)
#define DEFAULT_ALPHA_PER_PIXEL  (1)
#define DEFAULT_BITS_PER_PIXEL   (32)
#define DEFAULT_KEEP_RESOURCE    (0)
#define DEFAULT_SCALE            (1)
#define DEFAULT_Z_ORDER          (50)
#define Z_ORDER_MAX               (11)

#define PROC_WRITE_BUF_SIZE      128

//Actions to take
#define CHANGE_FB_Y_VIRTUAL      (1 << 1)
#define CHANGE_FB_RESOLUTION     (1 << 2)
#define CHANGE_BIT_DEPTH         (1 << 3)
#define FB_CHANGE_ACTIONS_MASK   (0x0000000F)

#define FB_FORMAT_IS_ANY_YUV(a)   ( (a == FB_FORMAT_YUVNV12) || \
                                   (a == FB_FORMAT_YUVNV21) )
#define FB_FORMAT_IS_ANY_RGB(a)   ( (a == FB_FORMAT_RGB565) || \
                                   (a == FB_FORMAT_RGB888) || \
                                   (a == FB_FORMAT_RGBA32) || \
                                   (a == FB_FORMAT_BGR565) || \
                                   (a == FB_FORMAT_BGR888) )
                                   


typedef struct
{
   // Proc entries corresponding to the modifiable parameters
   struct proc_dir_entry  *fb_cfg_directory;  // Per framebuffer device proc directory
   struct proc_dir_entry  *alpha_cfg_entry;
   struct proc_dir_entry  *alpha_per_pixel_cfg_entry;
   struct proc_dir_entry  *bpp_override_cfg_entry;
   struct proc_dir_entry  *keep_resource_cfg_entry;
   struct proc_dir_entry  *res_override_cfg_entry;
   struct proc_dir_entry  *scale_cfg_entry;
   struct proc_dir_entry  *z_order_cfg_entry;
   struct proc_dir_entry  *pixfmt_cfg_entry;
   struct proc_dir_entry  *colspace_cfg_entry;
   struct proc_dir_entry  *buffers_cfg_entry;
}SCRN_PROC_ENTRY_T;

typedef struct
{
   VC_FB_SCRN        scrn;
   uint8_t           fb_num;
   uint32_t          user_cnt;          // Number of active users of this screen
   struct mutex      user_cnt_mutex;    // Mutex to protect user count
   struct fb_info    fb_info;           // Kernel framebuffer info
   wait_queue_head_t wait_for_vc_status;
	spinlock_t        lock;
   int32_t           is_suspended;
   uint16_t          fb_update_count;
   uint16_t          vc_update_count;
   uint32_t          res_handle;        // Videocore resource handle
   uint32_t          cmap[16];          // Color map
   int32_t           is_yuv;
   uint32_t          pending_actions;
   uint32_t          vc_addr;
   
   // Modifiable paramters for framebuffer creation (upon fb_open call)
   uint32_t          alpha;             // Alpha value to use when alpha_per_pixel=0
   uint32_t          alpha_per_pixel;   // 1 = Each pixel has its own alpha value
   uint32_t          bpp_override;      // Bits per pixel override
   uint32_t          keep_resource;     // Keep resource open even if release is called
   uint32_t          scale;             // Scale the image to fit the screen
   uint32_t          z_order;           // Z-order of framebuffer
   uint32_t          width_override;    // Width override (resolution override)
   uint32_t          height_override;   // Height override (resolution override)
   uint16_t          pixel_fmt;         // Pixel format (RGB/YUV)
   uint16_t          color_space;
	uint16_t          buffers;  // Single or double buffered
   SCRN_PROC_ENTRY_T proc_interface;
   struct dentry *debugfs_entry;
   void*             instance;
} SCRN_INFO_T;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} FB_RECT_T;

int vc_fb_create_per_scrn_proc_entries(SCRN_INFO_T *scrn_info, struct proc_dir_entry* global_proc_entry);
void vc_fb_remove_per_scrn_proc_entries(SCRN_INFO_T *scrn_info, struct proc_dir_entry* global_proc_entry);

int vc_fb_create_per_scrn_debugfs_entry(SCRN_INFO_T *scrn_info,
					struct dentry *fb_debug_fs_dir);
void vc_fb_remove_per_scrn_debugfs_entry(SCRN_INFO_T *scrn_info);

#endif // _VC_FB_H_
