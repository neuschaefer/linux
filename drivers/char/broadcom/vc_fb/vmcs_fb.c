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

/*
 * Description:
 *    This is the videocore framebuffer driver. A framebuffer device for each
 *    of the supported screens is registered with the Linux system. The actual
 *    allocation of the framebuffer memory is not done until someone invokes
 *    the open command on the framebuffer device. This driver uses the
 *    framebuffer service, which provides (but not limited) the following:
 *       - information (resolution, bpp) for each attached screen
 *       - allocation of memory for framebuffer on videocore
 *       - panning/swapping of framebuffers
 *    Since the allocation is not done until the device open is invoked, users
 *    can modify certain parameters that affects the framebuffer via proc
 *    entries. The following properties can be modified:
 *       - alpha per pixel
 *       - default alpha (if alpha per pixel is not used)
 *       - h/w scaling
 *       - resolution override (this is the resolution reported to linux, which
 *         can be smaller or greater than the actual resolution of the screen)
 *       - z-ordering of the framebuffer
 *
 * Notes:
 *    1. Terminology:
 *       - display: display device that has one or more screens attached
 *       - screen: attached to a display that shows actual content, e.g. LCD
 *    2. Currently only one instance of videocore is supported
 *    3. STR not yet supported
 *    4. Rotation not yet supported
 *    5. TODO: Add a 'info' proc entry to show number of active users, current
 *       resolution, memory address/length, dev path (/dev/fb0), etc.
 */

/*************** Include files ***********************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/semaphore.h>
#include <linux/dma-mapping.h>
#include <linux/pfn.h>
#include <linux/hugetlb.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/mm.h>
#include <linux/debugfs.h>
#include <linux/time.h>

#include <vc_mem.h>
#include <vc_fb_defs.h>
#include "vc_fb.h"
#include <vc_vchi_fb.h>
/*************** End of Include files ****************************************/


/*************** Private constants *******************************************/
#define VC_WAIT_VSYNC_TIMEOUT_MS (100)
#define VC_WAIT_VSYNC_TIMEOUT_J  (msecs_to_jiffies(VC_WAIT_VSYNC_TIMEOUT_MS))
#define FB_VC_COUNTS_EQUAL(a)    (a->fb_update_count == a->vc_update_count)
/*************** End of Private constants ************************************/


/*************** Private types ***********************************************/
typedef struct {
	int fb_inited;
	SCRN_INFO_T * scrn_info[MAX_NUM_FB];
	struct proc_dir_entry *cfg_directory;
	struct dentry *debugfs_dir;
} FB_STATE_T;

typedef struct {
	uint16_t fmt;
	uint16_t color_space;
} YUV_PIXFMT_T;

/*************** End of Private types ****************************************/

/*************** Private variables *******************************************/
static FB_STATE_T *fb_state;
/*************** End of Private variables ************************************/

/*************** Private functions *******************************************/
static int vc_fb_pan_scrn(struct fb_var_screeninfo *var,
			struct fb_info *fb_info);

static inline void set_default_rectangles(struct fb_var_screeninfo *var)
{
	FB_RECT_T *s = (FB_RECT_T *)&var->reserved[0];
	FB_RECT_T *t = (FB_RECT_T *)&var->reserved[2];

	s->x = s->y = t->x = t->y = 0;
	s->w = t->w = var->xres;
	s->h = t->h = var->yres;
	LOG_DBG("set default rectangles to %d x %d\n", s->w, s->h);
}

static int check_bounds(FB_RECT_T *r, struct fb_var_screeninfo *var)
{
	return (((r->x + r->w) > var->xres_virtual)
		|| ((r->y + r->h) > var->yres_virtual));
}

static inline int is_rect_set(struct fb_var_screeninfo *var, FB_RECT_T *s,
			FB_RECT_T *t)
{
	if (s->w && s->h && t->w && t->h) goto out;
	if (var->nonstd) {
		LOG_ERR("YUV Framebuffer must set src/target rectangles");
		return 0;
	}
	set_default_rectangles(var);
out:
	return 1;
}

static inline SCRN_INFO_T *to_scrn_info(struct fb_info *fb_info)
{
	return container_of(fb_info, SCRN_INFO_T, fb_info);
}

static inline uint32_t convert_bitfield(int val,
					struct fb_bitfield *bf)
{
	unsigned int mask = (1 << bf->length) - 1;
	return (val >> (16 - bf->length) & mask) << bf->offset;
}

static int vc_fb_set_pixfmt(struct fb_var_screeninfo *var, int pixel_fmt)
{
	switch (pixel_fmt) {
	case FB_FORMAT_RGB565:
		var->red.offset = 11;
		var->red.length = 5;
		var->green.offset = 5;
		var->green.length = 6;
		var->blue.offset = 0;
		var->blue.length = 5;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	case FB_FORMAT_RGB888:
		var->red.offset = 16;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	case FB_FORMAT_MIN:	/* default to RGBA32 */
	case FB_FORMAT_RGBA32:
		var->red.offset = 16;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 0;
		var->blue.length = 8;
		var->transp.offset = 24;
		var->transp.length = 8;
		break;

	case FB_FORMAT_BGR565:
		var->red.offset = 0;
		var->red.length = 5;
		var->green.offset = 5;
		var->green.length = 6;
		var->blue.offset = 11;
		var->blue.length = 5;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	case FB_FORMAT_BGR888:
		var->red.offset = 10;
		var->red.length = 8;
		var->green.offset = 8;
		var->green.length = 8;
		var->blue.offset = 16;
		var->blue.length = 8;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	case FB_FORMAT_YUVNV12:
	case FB_FORMAT_YUVNV21:
		var->red.offset = 0;
		var->red.length = 0;
		var->green.offset = 0;
		var->green.length = 0;
		var->blue.offset = 0;
		var->blue.length = 0;
		var->transp.offset = 0;
		var->transp.length = 0;
		break;

	default:
		LOG_ERR("%s: Incorrect standard pixel format", __func__);
		return -EINVAL;
	}
	return 0;
}

static int vc_fb_get_info(SCRN_INFO_T *scrn_info)
{
	int ret = 0;
	int32_t success;
	VC_FB_SCRN_INFO_T info;
	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);

	success = vc_vchiq_fb_get_scrn_info(scrn_info, &info);
	if (success != 0) {
		LOG_ERR("fb=%u: failed to get info (success=%d)",
			scrn_info->fb_num, success);
		ret = -EAGAIN;
		goto out;
	} else if ((info.width == 0) || (info.height == 0)) {
		LOG_DBG("fb=%u: could not get wxh.. using defaults",
			scrn_info->fb_num);
		info.width = CONFIG_FB_VC_DEFAULT_SCREEN_WIDTH;
		info.height = CONFIG_FB_VC_DEFAULT_SCREEN_HEIGHT;
	}
	LOG_DBG("fb=%u: scrn_info is width=%d, height=%d, bpp=%d",
		scrn_info->fb_num, info.width, info.height,
		info.bits_per_pixel);

	if (scrn_info->width_override != 0) {
		info.width = scrn_info->width_override;
		LOG_DBG("fb=%u: Width overide = %d\n",
			scrn_info->fb_num, info.width);
	}
	if (scrn_info->height_override != 0) {
		info.height = scrn_info->height_override;
		LOG_DBG("fb=%u: Height overide = %d\n",
			scrn_info->fb_num, info.height);
	}
	if (scrn_info->bpp_override != 0)
		info.bits_per_pixel = scrn_info->bpp_override;

	if (info.bits_per_pixel == 0)
		info.bits_per_pixel = DEFAULT_BITS_PER_PIXEL;
	/* Make sure the buffer count is non-zero */
	if (!scrn_info->buffers)
		scrn_info->buffers = 1;

	if ((info.width % 16)) {
		/* Videocore needs the dimensions to be a multiple of 16 */
		info.width = ((info.width + 15) & ~15);
		LOG_DBG("fb=%u: rounded width = %d\n",
			scrn_info->fb_num, info.width);
	}
	if ((info.height % 16)) {
		/* Videocore needs the dimensions to be a multiple of 16 */
		info.height = ((info.height + 15) & ~15);
		LOG_DBG("fb=%u: rounded height = %d\n",
			scrn_info->fb_num, info.height);
	}

	scrn_info->fb_info.var.xres
		= scrn_info->fb_info.var.xres_virtual = info.width;
	scrn_info->fb_info.var.yres = info.height;

	/* The dimensions (in mm) of the display. We don't know them. */
	scrn_info->fb_info.var.width = 0;
	scrn_info->fb_info.var.height = 0;

	if (FB_FORMAT_IS_ANY_YUV(scrn_info->pixel_fmt)) {
		LOG_DBG("fb=%u: YUV fmt =%d\n", scrn_info->fb_num,
			scrn_info->pixel_fmt);
		scrn_info->is_yuv = 1;
		scrn_info->fb_info.var.bits_per_pixel = 8;
		/* YUV only supports single buffered mode.  make it so */
		scrn_info->fb_info.var.yres_virtual   = info.height;
		scrn_info->color_space = CSC_JPEG_JIF;
		scrn_info->fb_info.fix.visual = FB_VISUAL_MONO01;
	} else if (FB_FORMAT_IS_ANY_RGB(scrn_info->pixel_fmt)
		|| (scrn_info->pixel_fmt == FB_FORMAT_MIN)) {
		LOG_DBG("fb=%u: RGB fmt = %d\n", scrn_info->fb_num,
			scrn_info->pixel_fmt);
		scrn_info->fb_info.var.bits_per_pixel = info.bits_per_pixel;
		scrn_info->fb_info.var.yres_virtual
			= info.height * scrn_info->buffers;
		scrn_info->fb_info.fix.visual = FB_VISUAL_TRUECOLOR;
	} else {
		LOG_DBG("fb=%u: unsupported fmt = %d\n", scrn_info->fb_num,
			scrn_info->pixel_fmt);
		ret = -EINVAL;
		goto out;
	}

	set_default_rectangles(&scrn_info->fb_info.var);
	scrn_info->fb_info.var.reserved[4] = scrn_info->alpha;

	if (vc_fb_set_pixfmt(&scrn_info->fb_info.var,
				scrn_info->pixel_fmt) < 0) {
		LOG_ERR("fb=%u: could not set pixel format for screen %s",
			scrn_info->fb_num, scrn_info->fb_info.fix.id);
		ret = -EINVAL;
		goto out;
	}

	if (scrn_info->pixel_fmt == FB_FORMAT_RGBA32)
		scrn_info->alpha_per_pixel = 1;
	else
		scrn_info->alpha_per_pixel = 0;
	LOG_DBG("fb=%u: Alpha_per_pixel = %d\n", scrn_info->fb_num,
		scrn_info->alpha_per_pixel);
out:
	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);
	return ret;
}
/*************** End of Private functions ************************************/

/*************** Driver entry functions **************************************/
static int vc_fb_open(struct fb_info *fb_info, int user)
{
	int ret = 0;
	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);
	struct resource *res;
	uint8_t persist_reserve[sizeof(fb_info->var.reserved)];

	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);
	mutex_lock(&scrn_info->user_cnt_mutex);

	if (!fb_state || !fb_state->fb_inited) {
		LOG_ERR("fb=%u: framebuffer is not initialized properly",
			scrn_info->fb_num);
		ret = -EFAULT;
		goto out;
	}

	/* MJF: What is this check doing? */
	if (user == 0) {
		LOG_ERR("fb=%u: No framebuffer console on overlay "
			"framebuffers",
			scrn_info->fb_num);
		ret = -ENODEV;
		goto out;
	}
	/*
	 * Only allocate the framebuffer if its the first user AND we do
	 * not already have one allocated
	 */
	if ((scrn_info->user_cnt == 0) && (scrn_info->res_handle == 0)) {
		int32_t success;
		VC_FB_ALLOC_T alloc;
		VC_FB_ALLOC_RESULT_T alloc_result;

		/* transfer src/dst rectangles */
		memcpy(persist_reserve, scrn_info->fb_info.var.reserved,
			sizeof(persist_reserve));

		ret = vc_vchiq_fb_open(scrn_info);
		if (ret != 0) {
			LOG_ERR("fb=%u: Could not init vchiq service",
				scrn_info->fb_num);
			ret = -ENODEV;
			goto out;
		}

		ret = vc_fb_get_info(scrn_info);
		if (ret != 0) {
			LOG_ERR("fb=%u: Could not get screen info",
				scrn_info->fb_num);
			ret = -ENODEV;
			goto out;
		}

		/* restore src/dst rectangles */
		memcpy(scrn_info->fb_info.var.reserved, persist_reserve,
			sizeof(persist_reserve));

		alloc.scrn = scrn_info->scrn;
		alloc.width = fb_info->var.xres;
		alloc.height = fb_info->var.yres;
		alloc.bits_per_pixel = fb_info->var.bits_per_pixel;
		alloc.pixel_fmt = scrn_info->pixel_fmt;
		alloc.color_space = scrn_info->color_space;
		alloc.layer = scrn_info->z_order;
		alloc.alpha_per_pixel = scrn_info->alpha_per_pixel;
		alloc.default_alpha = scrn_info->alpha;
		alloc.scale = scrn_info->scale;
		if (scrn_info->buffers)
			alloc.num_frames = scrn_info->buffers;
		else
			alloc.num_frames = 1; /* default */

		LOG_DBG("fb=%u: alloc: %ux%u, bpp=%u, num_frms=%u, z-order=%u,"
			"scale=%u, alpha_per_pixel=%u, default_alpha=%u",
			scrn_info->fb_num, alloc.width, alloc.height,
			alloc.bits_per_pixel, alloc.num_frames, alloc.layer,
			alloc.scale, alloc.alpha_per_pixel,
			alloc.default_alpha);

		success = vc_vchiq_fb_alloc(scrn_info, &alloc, &alloc_result);
		if ((success != 0) || (alloc_result.res_handle == 0)) {
			LOG_ERR("fb=%u: failed to allocate fb buffer "
				"(success=%d)",
				scrn_info->fb_num, success);
			ret = -ENOMEM;
			goto out;
		}

		LOG_DBG("fb=%u: alloc_result: res_handle=0x%08x, "
			"res_mem=0x%p, line_bytes=%u, frame_bytes=%u",
			scrn_info->fb_num, alloc_result.res_handle,
			alloc_result.res_mem, alloc_result.line_bytes,
			alloc_result.frame_bytes);

		scrn_info->res_handle = alloc_result.res_handle;

		scrn_info->vc_addr =
			(uint32_t)alloc_result.res_mem & 0x3FFFFFFF;

		res = request_mem_region(scrn_info->vc_addr
					+ mm_vc_mem_phys_addr,
					alloc_result.frame_bytes
					* alloc.num_frames,
					fb_info->fix.id);
		if (res == NULL) {
			LOG_ERR("fb=%u: failed to request I/O memory region",
				scrn_info->fb_num);
			ret = -ENOMEM;
			goto err_free_fb;
		}

		fb_info->screen_base = ioremap_nocache(res->start,
						resource_size(res));
		if (fb_info->screen_base == NULL)  {
			LOG_ERR("fb=%u: failed to I/O remap framebuffer",
				scrn_info->fb_num);
			ret = -ENOMEM;
			goto err_release_mem_region;
		}

		fb_info->fix.smem_start = res->start;
		fb_info->fix.smem_len = resource_size(res);
		fb_info->fix.line_length = alloc_result.line_bytes;
		LOG_DBG("fb=%u: screen_base=0x%p, smem_start=0x%08x, "
			"smem_len=%u, line_length=%u",
			scrn_info->fb_num, fb_info->screen_base,
			(uint32_t)fb_info->fix.smem_start,
			fb_info->fix.smem_len, fb_info->fix.line_length);
		LOG_DBG("fb=%u: virt_to_phys=0x%p", scrn_info->fb_num,
			(void *)virt_to_phys(fb_info->screen_base));

		scrn_info->fb_update_count = scrn_info->vc_update_count = 0;

		/* Apply src/dst rectangle and pan. */
		vc_fb_pan_scrn(&scrn_info->fb_info.var,&scrn_info->fb_info);
	}

	scrn_info->user_cnt++;

	goto out;

err_release_mem_region:
	release_mem_region(res->start, resource_size(res));

err_free_fb:
	vc_vchiq_fb_free(scrn_info);
	scrn_info->res_handle = 0;

out:
	if ((ret < 0) && scrn_info->instance)
		ret |= vc_vchiq_fb_close(scrn_info);
	mutex_unlock(&scrn_info->user_cnt_mutex);
	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);
	return ret;
}

static int vc_fb_release(struct fb_info *fb_info, int user)
{
	int ret = 0;
	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);

	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);

	mutex_lock(&scrn_info->user_cnt_mutex);

	/*
	 * Only free the framebuffer if there are no more users AND we
	 * do not want to keep the resource around
	 */
	if ((--scrn_info->user_cnt == 0)
		&& (scrn_info->keep_resource == 0)) {
		int32_t success;

		LOG_DBG("fb=%u: freeing framebuffer fb=%u, vc=%u",
			scrn_info->fb_num, scrn_info->fb_update_count,
			scrn_info->vc_update_count);
#ifndef CONFIG_FB_DEFERRED_PAN
		if (scrn_info->fb_update_count
			&& scrn_info->fb_update_count
			!= scrn_info->vc_update_count) {

			if (scrn_info->vc_update_count
				- scrn_info->fb_update_count > 1) {
				LOG_DBG("fb=%u: release counts out of sync %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
			}
			if (scrn_info->fb_update_count
				- scrn_info->vc_update_count > 2) {
				LOG_DBG("fb=%u: release too many frames %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
			}
			ret = wait_event_timeout(scrn_info->wait_for_vc_status,
						FB_VC_COUNTS_EQUAL(scrn_info),
						VC_WAIT_VSYNC_TIMEOUT_J * 2);
			if (ret == 0) {
				LOG_ERR("fb=%u: vsync timed out %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
				ret = -ETIMEDOUT;
				goto out;
			}
		}
		ret = 0;
#endif

		iounmap(fb_info->screen_base);
		fb_info->screen_base = NULL;

		LOG_DBG("fb=%u: unmapped framebuffer", scrn_info->fb_num);

		release_mem_region(fb_info->fix.smem_start,
				fb_info->fix.smem_len);
		fb_info->fix.smem_start = 0;
		fb_info->fix.smem_len = 0;

		LOG_DBG("fb=%u: released mem region", scrn_info->fb_num);

		if (scrn_info->instance)  {
			success = vc_vchiq_fb_free(scrn_info);
			if (success != 0) {
				LOG_ERR("fb=%u: failed to free framebuffer "
					"(success=%d)",
					scrn_info->fb_num, success);
		/*
		 * Even if we failed to release it, we should continue
		 * on as if it succeeded - this might lead to memory
		 * leaks on the videocore!!
		 */
			}
			LOG_DBG("fb=%u: freed fb", scrn_info->fb_num);
			success = vc_vchiq_fb_close(scrn_info);
			if (success != 0) {
				LOG_ERR("fb=%u: failed to close framebuffer "
					"service (success=%d)",
					scrn_info->fb_num, success);
				ret = -EIO;
			}
		}
		LOG_DBG("fb=%u: closed service", scrn_info->fb_num);
		scrn_info->res_handle = 0;
		scrn_info->fb_update_count = scrn_info->vc_update_count = 0;
	}

out:
	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);
	mutex_unlock(&scrn_info->user_cnt_mutex);
	return ret;
}

static int vc_fb_blank(int blank_mode, struct fb_info *fb_info)
{
	int32_t success;
	int32_t ret = 0;
	VC_FB_CFG_T cfg;
	VC_FB_CFG_RESULT_T cfg_result;

	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);

	if (scrn_info->is_suspended == blank_mode) {
		LOG_DBG("fb=%u: blank mode is already set to (%d)",
			scrn_info->fb_num, blank_mode);
		goto out;
	}
	cfg.res_handle = scrn_info->res_handle;
	cfg.width = fb_info->var.xres;
	cfg.height = fb_info->var.yres;
	cfg.bits_per_pixel = fb_info->var.bits_per_pixel;
	cfg.pixel_fmt = scrn_info->pixel_fmt;
	cfg.color_space = scrn_info->color_space;
	cfg.blank = 1;
	switch (blank_mode) {
	case FB_BLANK_POWERDOWN:
	case FB_BLANK_NORMAL:
		/*
		 * Set opacity to 0 and update the framebuffer again
		 * with all other settings intact
		 */
		cfg.alpha_per_pixel = 0;
		cfg.default_alpha = 0;
		break;

	case FB_BLANK_UNBLANK:
		/*
		 * Set opacity 255 for yuv framebuffer and
		 * var.reserved[4] for RGB and update the framebuffer
		 * again with all other settings intact
		 */
		cfg.alpha_per_pixel = scrn_info->alpha_per_pixel;
		cfg.default_alpha = scrn_info->alpha;
		break;

	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	default:
		ret = -EINVAL;
		goto out;
	}


	if (~(scrn_info->fb_update_count & 0xffff))
		scrn_info->fb_update_count++;
	else
		scrn_info->fb_update_count = 1;

	success = vc_vchiq_fb_cfg(scrn_info, &cfg, &cfg_result);
	if (success != 0) {
		LOG_ERR("fb=%u: failed to config framebuffer (success=%d)",
			scrn_info->fb_num, success);
		ret = -EPERM;
		goto out;
	}
	scrn_info->vc_update_count = cfg_result.update_count;
	scrn_info->is_suspended = blank_mode;
out:
	return ret;
}


/*
 * We are protected against another ioctl by info->lock taken in
 * do_fb_ioctl() so, we are safe against pan_display() for vsync check
 */

static int vc_fb_ioctl(struct fb_info *fb_info, unsigned int cmd,
		unsigned long arg)
{
	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);
	struct timespec ts1, ts2;
    unsigned long latency;
	int ret;

	switch (cmd) {
	case FBIO_WAITFORVSYNC:
		getnstimeofday(&ts1);

		if (scrn_info->is_suspended == FB_BLANK_POWERDOWN) {
			LOG_ERR("fb=%u: is powered down no need to wait "
				"for vsync",
				scrn_info->fb_num);
			ret = -ENODEV;
			goto out;
		}
		if (scrn_info->fb_update_count != scrn_info->vc_update_count) {
			LOG_DBG("fb=%u: waiting for vsync %u - %u",
				scrn_info->fb_num, scrn_info->fb_update_count,
				scrn_info->vc_update_count);
			if (scrn_info->vc_update_count
				- scrn_info->fb_update_count > 1) {
				LOG_ERR("fb=%u: ioctl counts out of sync %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
			}
			if (scrn_info->fb_update_count
				- scrn_info->vc_update_count > 2) {
				LOG_ERR("fb=%u: ioctl too many frames %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
			}
			ret = wait_event_timeout(scrn_info->wait_for_vc_status,
						FB_VC_COUNTS_EQUAL(scrn_info),
						VC_WAIT_VSYNC_TIMEOUT_J);
			if (ret == 0) {
				LOG_ERR("fb=%u: ioctl vsync timed out",
					scrn_info->fb_num);
				ret = -ETIMEDOUT;
			}
		} else {
			ret = 0;
        }

		getnstimeofday(&ts2);

        latency = ((ts2.tv_sec - ts1.tv_sec) * NSEC_PER_SEC + ts2.tv_nsec - ts1.tv_nsec);

        if(latency > 20000000 && printk_ratelimit())
            LOG_ERR("fb=%u: ioctl vsync took %ld usec", scrn_info->fb_num, latency / 1000);

		break;
	default:
		LOG_ERR("fb=%u: invalid ioctl command",  scrn_info->fb_num);
		ret = -ENOIOCTLCMD;
	}
out:
	return ret;
}

/* MJF: Probably not correct */
static int vc_fb_check_var(struct fb_var_screeninfo *var,
			struct fb_info *fb_info)
{
	int ret;
	FB_RECT_T *src_rect;
	FB_RECT_T *dst_rect;
	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);
	unsigned i;
    ret = -EINVAL;

	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);
	src_rect = (FB_RECT_T *)&(fb_info->var.reserved[0]);
	dst_rect = (FB_RECT_T *)&(fb_info->var.reserved[2]);
	LOG_DBG("fb=%u: preset var: %ux%u, %ux%u, %ux%u, %u-%u, %ux%u, %u-%u, "
		"%ux%u, %u", scrn_info->fb_num,
		fb_info->var.xres, fb_info->var.yres,
		fb_info->var.xres_virtual, fb_info->var.yres_virtual,
		fb_info->var.width, fb_info->var.height,
		src_rect->x, src_rect->y, src_rect->w, src_rect->h,
		dst_rect->x, dst_rect->y, dst_rect->w, dst_rect->h,
		fb_info->var.reserved[4]);
	src_rect = (FB_RECT_T *)&var->reserved[0];
	dst_rect = (FB_RECT_T *)&var->reserved[2];
	LOG_DBG("fb=%u: request var: %ux%u, %ux%u, %ux%u, %u-%u, "
		"%ux%u, %u-%u, %ux%u, %u", scrn_info->fb_num,
		var->xres, var->yres, var->xres_virtual, var->yres_virtual,
		var->width, var->height, src_rect->x, src_rect->y,
		src_rect->w, src_rect->h, dst_rect->x, dst_rect->y,
		dst_rect->w, dst_rect->h, var->reserved[4]);

	if (((var->rotate & 1) != (fb_info->var.rotate & 1))
		|| (var->xres_virtual != fb_info->var.xres_virtual))  {
		LOG_ERR("fb=%u: Rotate/changing xres_virtual is not allowed",
			scrn_info->fb_num);
		goto out;
	}

	if (fb_info->var.yres_virtual != var->yres_virtual)
		scrn_info->pending_actions |= CHANGE_FB_Y_VIRTUAL;

	if ((var->xres != fb_info->var.xres) ||
		(var->yres != fb_info->var.yres))
		scrn_info->pending_actions |= CHANGE_FB_RESOLUTION;
	if (var->bits_per_pixel != fb_info->var.bits_per_pixel)
		scrn_info->pending_actions |= CHANGE_BIT_DEPTH;

#if 0
	if ((fb_info->var.nonstd == 0) && scrn_info->pending_actions) {
		/* TODO: Handle bit depth change for RGB framebuffer */
		LOG_ERR("fb=%u: Does not support any change",
			scrn_info->fb_num);
		goto out;
	}
#endif

	if (scrn_info->pending_actions & CHANGE_FB_Y_VIRTUAL) {
        for (i = 1; i <= scrn_info->buffers; ++i)
            if (var->yres_virtual == fb_info->var.yres * i)
                break;

        if (i > scrn_info->buffers) {
            LOG_ERR("fb=%u: Invalid yres_virtual change "
				"(%d)->(%d)",
				scrn_info->fb_num,
				fb_info->var.yres_virtual,
				var->yres_virtual);
			goto out;
		}

		if (scrn_info->is_yuv) {
			LOG_ERR("fb=%u: Changing only y_virtual for "
				"YUV framebuffer is not	allowed",
				scrn_info->fb_num);
			goto out;
		}
		LOG_DBG("fb=%u: Changing yres_virtual from (%d) to (%d)",
			scrn_info->fb_num, fb_info->var.yres_virtual,
			var->yres_virtual);
	}

	if (scrn_info->pending_actions & CHANGE_FB_RESOLUTION) {
		if ((var->xres > 4096) || (var->yres > 4096)) {
			LOG_ERR("fb=%u: Maximum XxY while changing format",
				scrn_info->fb_num);
			LOG_ERR("fb=%u: Can't set resol'n to (%d)x(%d)",
				scrn_info->fb_num, var->xres, var->yres);
			goto out;
		}

		scrn_info->width_override = var->xres;
		scrn_info->height_override = var->yres;
		LOG_DBG("fb=%u: adjusted to %ux%u", scrn_info->fb_num,
			scrn_info->width_override, scrn_info->height_override);
	}

	ret = 0;
out:
	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);

	return ret;
}

static int vc_fb_set_par(struct fb_info *fb_info)
{
	int ret = 0;
	int32_t success;
	VC_FB_CFG_T cfg;
	VC_FB_CFG_RESULT_T cfg_result;
	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);
	struct resource *res;

	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);

	/*
	 * We are here only if there is a change request from user to
	 * change framebuffer params as check_var is returned success
	 * to linux fb layer.  Unmap the previously allocated
	 * framebuffer, apply changes and remap it again.  If there is
	 * no change in the parameters requested from user,we don't
	 * need to proceed and send message to videocore. In this
	 * case, we return success and linux fb layer calls
	 * vc_fb_pan_scrn() directly
	 */

	if (!(scrn_info->pending_actions & FB_CHANGE_ACTIONS_MASK))
		goto out;


	/*
	 * Do we need to disable framebuffer device to fill black
	 * color on vc_image on videocore? Below is just a placeholder
	 * for it and a new message needs to be implemented from
	 * HOST->VC and VC->HOST
	 */
#if 0
	DisableFBDevice();
#endif

	/*
	 * We get the updated var screen info from linux fb layer upon
	 * sucecssful check_var()
	 */
	memset(&cfg, 0, sizeof(cfg));
	cfg.res_handle = scrn_info->res_handle;
	cfg.width = fb_info->var.xres;
	cfg.height = fb_info->var.yres;
	cfg.bits_per_pixel = fb_info->var.bits_per_pixel;
	cfg.alpha_per_pixel = scrn_info->alpha_per_pixel;
	cfg.default_alpha = scrn_info->alpha;
	cfg.pixel_fmt = scrn_info->pixel_fmt;
	cfg.color_space = scrn_info->color_space;

	if (~(scrn_info->fb_update_count & 0xffff))
		scrn_info->fb_update_count++;
	else
		scrn_info->fb_update_count = 1;

	success = vc_vchiq_fb_cfg(scrn_info, &cfg, &cfg_result);
	if (success != 0) {
		LOG_ERR("fb=%u: failed to config framebuffer (success=%d)",
			scrn_info->fb_num, success);
		ret = -EPERM;
		goto out;
	}

	LOG_DBG("fb=%u: cfg_result: l_bytes=%u, frm_bytes=%u",
		scrn_info->fb_num, cfg_result.line_bytes,
		cfg_result.frame_bytes);

	/*
	 * Update the fixed variables. line_bytes and frame_bytes are
	 * updated by VC only when we change image type. Do not update
	 * if it is zero
	 */
	if (cfg_result.line_bytes)
		fb_info->fix.line_length = cfg_result.line_bytes;
	if (cfg_result.frame_bytes != fb_info->fix.smem_len) {
		LOG_DBG("fb=%u: IOUNMAP (0x%p)", scrn_info->fb_num,
			fb_info->screen_base);
		iounmap((void *)fb_info->screen_base);

		res = request_mem_region(scrn_info->vc_addr
					+ mm_vc_mem_phys_addr,
					cfg_result.frame_bytes,
					fb_info->fix.id);
		if (res == NULL) {
			LOG_ERR("fb=%u: failed req I/O memory region",
				scrn_info->fb_num);
			ret = -EIO;
			goto out;
		}

		fb_info->screen_base = ioremap_nocache(res->start,
						resource_size(res));
		if (fb_info->screen_base == NULL) {
			LOG_ERR("fb=%u: failed to I/O remap framebuffer",
				scrn_info->fb_num);
			ret = -ENOMEM;
			release_mem_region(res->start, resource_size(res));
			goto out;
		}

		fb_info->fix.smem_start = res->start;
		fb_info->fix.smem_len = resource_size(res);
		LOG_DBG("fb=%u: screen_base=0x%p, smem_start=0x%08x, "
			"smem_len=%u, line_length=%u",
			scrn_info->fb_num, fb_info->screen_base,
			(uint32_t)fb_info->fix.smem_start,
			fb_info->fix.smem_len,
			fb_info->fix.line_length);
		LOG_DBG("fb=%u: virt_to_phys=0x%p screen %s",
			scrn_info->fb_num,
			(void *)virt_to_phys(fb_info->screen_base),
			fb_info->fix.id);
	}

	scrn_info->vc_update_count = cfg_result.update_count;
	scrn_info->pending_actions = 0;
out:
	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);
	return ret;
}

static int vc_fb_setcolreg(unsigned int regno,
			unsigned int red,
			unsigned int green,
			unsigned int blue,
			unsigned int transp,
			struct fb_info *fb_info)
{
	int ret = 0;
	SCRN_INFO_T *scrn_info = to_scrn_info(fb_info);

	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);

	LOG_DBG("fb=%u: regno=%u, red=%u, green=%u, blue=%u, transp=%u,",
		scrn_info->fb_num, regno, red, green, blue, transp);


	if (regno < 16)
		scrn_info->cmap[regno] =
			convert_bitfield(red, &scrn_info->fb_info.var.red) |
			convert_bitfield(green, &scrn_info->fb_info.var.green)
			| convert_bitfield(blue, &scrn_info->fb_info.var.blue)
			| convert_bitfield(transp,
					&scrn_info->fb_info.var.transp);
	else if (regno > 255) {
		LOG_ERR("fb=%u: invalid color register number %u",
			scrn_info->fb_num, regno);
		ret = 1;
	}

	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);

	return ret;
}

static int vc_fb_pan_scrn(struct fb_var_screeninfo *var,
			struct fb_info *fb_info)
{
	int ret = 0;
	int32_t success;
	SCRN_INFO_T *scrn_info;
	VC_FB_PAN_T pan;
	FB_RECT_T *src_rect;
	FB_RECT_T *dst_rect;

	scrn_info = to_scrn_info(fb_info);
	src_rect = (FB_RECT_T *)&var->reserved[0];
	dst_rect = (FB_RECT_T *)&var->reserved[2];

	LOG_DBG("fb=%u: ..Start", scrn_info->fb_num);
	LOG_DBG("fb=%u: src rect (%i, %i, %i, %i)", scrn_info->fb_num,
		src_rect->x, src_rect->y, src_rect->w, src_rect->h);
	LOG_DBG("fb=%u: target rect (%i, %i, %i, %i)", scrn_info->fb_num,
		dst_rect->x, dst_rect->y, dst_rect->w, dst_rect->h);


	if (is_rect_set(var, src_rect, dst_rect)) {
		if (check_bounds(src_rect, var)) {
			LOG_ERR("fb=%u: src/dest rectangles in var are "
				"out of bounds",
				scrn_info->fb_num);
			LOG_ERR("fb=%u: src rect (%i, %i, %i, %i)",
				scrn_info->fb_num,
				src_rect->x, src_rect->y, src_rect->w,
				src_rect->h);
			LOG_ERR("fb=%u: target rect (%i, %i, %i, %i)",
				scrn_info->fb_num,
				dst_rect->x, dst_rect->y,
				dst_rect->w, dst_rect->h);
			ret = -EINVAL;
			goto out;
		}
	} else {
		LOG_ERR("fb=%u: must set src/target rectangles for updates",
			scrn_info->fb_num);
		ret = -EINVAL;
		goto out;
	}

	if (scrn_info->is_suspended == FB_BLANK_POWERDOWN) {
		LOG_ERR("fb=%u: Display is powered down",
			scrn_info->fb_num);
		ret = -ENODEV;
		goto out;
	}

#ifdef CONFIG_FB_DEFERRED_PAN
	if (scrn_info->fb_update_count &&
		(scrn_info->fb_update_count != scrn_info->vc_update_count)) {
		LOG_DBG("fb=%u: update is pending, fb-%u vc-%u",
			scrn_info->fb_num,
			scrn_info->fb_update_count,
			scrn_info->vc_update_count);
			if (scrn_info->vc_update_count
				- scrn_info->fb_update_count > 1) {
				LOG_ERR("fb=%u: pan counts out of sync %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
			}
			if (scrn_info->fb_update_count
				- scrn_info->vc_update_count > 2) {
				LOG_ERR("fb=%u: pan too many frames %u - %u",
					scrn_info->fb_num,
					scrn_info->fb_update_count,
					scrn_info->vc_update_count);
			}
		ret = wait_event_timeout(scrn_info->wait_for_vc_status,
					FB_VC_COUNTS_EQUAL(scrn_info),
					VC_WAIT_VSYNC_TIMEOUT_J);
		if (ret == 0) {
			LOG_ERR("%s: Wait for vsync timed out ret=%d "
				"fb_update_count=%d, "
				"vc_update_count=%d screen %s", __func__, ret,
				scrn_info->fb_update_count,
				scrn_info->vc_update_count,
				fb_info->fix.id);
			ret = -ETIMEDOUT;
			goto out;
		}
		LOG_DBG("fb=%u: remainig jiffies %u", scrn_info->fb_num, ret);
		ret = 0;
	}
#endif

	memset(&pan, 0, sizeof(pan));
	pan.res_handle = scrn_info->res_handle;
	pan.src_x = src_rect->x;
	pan.src_y = src_rect->y + var->yoffset;
	pan.src_w = src_rect->w;
	pan.src_h = src_rect->h;
	pan.dst_x = dst_rect->x;
	pan.dst_y = dst_rect->y;
	pan.dst_w = dst_rect->w;
	pan.dst_h = dst_rect->h;
	if (scrn_info->is_yuv) {
		scrn_info->alpha  = 255;
	} else {
        scrn_info->alpha = var->reserved[4] & 255;
    }

    if (scrn_info->is_suspended == FB_BLANK_UNBLANK) {
        pan.opacity = scrn_info->alpha;
    } else {
        pan.opacity = 0;
    }

	if (~(scrn_info->fb_update_count & 0xffff))
		scrn_info->fb_update_count++;
	else
		scrn_info->fb_update_count = 1;
	LOG_DBG("fb=%u: fb_count=%u vc_count=%u", scrn_info->fb_num,
		scrn_info->fb_update_count, scrn_info->vc_update_count);

	success = vc_vchiq_fb_pan(scrn_info, &pan);
	if (success != 0)  {
		LOG_ERR("fb=%u: failed to pan screen(success=%d)",
			scrn_info->fb_num, success);
		ret = -EPERM;
		goto out;
	}
out:
	LOG_DBG("fb=%u: ..End (ret=%d)", scrn_info->fb_num, ret);
	return ret;
}

static struct fb_ops vc_fb_ops = {
	.owner          = THIS_MODULE,
	.fb_ioctl       = vc_fb_ioctl,
	.fb_open        = vc_fb_open,
	.fb_release     = vc_fb_release,
	/*   .fb_read        = , */
	/*   .fb_write       = , */
	.fb_check_var   = vc_fb_check_var,
	.fb_set_par     = vc_fb_set_par,
	.fb_setcolreg   = vc_fb_setcolreg,
	.fb_blank       = vc_fb_blank,
	.fb_pan_display = vc_fb_pan_scrn,
	.fb_fillrect    = cfb_fillrect,
	.fb_copyarea    = cfb_copyarea,
	.fb_imageblit   = cfb_imageblit,
	/*   .fb_cursor      = , */
	/*   .fb_rotate      = , */
};
/*************** End of Driver entry functions *******************************/

/*************** Driver module functions *************************************/
static int vc_fb_create_framebuffer(uint32_t num)
{
	int ret = 0;
	static int z_order = MAX_NUM_FB + 1;
	char fb_id[8]; /* don't forget the terminating 0 */

	SCRN_INFO_T *scrn_info;

	LOG_DBG("fb=%u ..Start", num);
	if (fb_state->scrn_info[num] != NULL) {
		LOG_ERR("fb=%u: framebuffer already created", num);
		ret = -EEXIST;
		goto out;
	}

	scrn_info = kzalloc(sizeof(*scrn_info), GFP_KERNEL);
	if (scrn_info == NULL) {
		LOG_ERR("fb=%u: failed to allocate memory", num);
		ret = -ENOMEM;
		goto out;
	}

	/*
	 * We create overlays over HDMI at present. This should be
	 * changed by a kernel config option where we create overlays
	 * for LCD / HDMI
	 */
	if (num >= VC_FB_SCRN_MAX)
		scrn_info->scrn = VC_FB_SCRN_HDMI;
	else
		scrn_info->scrn = num;
	scrn_info->fb_num = num;

	mutex_init(&scrn_info->user_cnt_mutex);

	/*
	 * Fill in most of the information in fb_info, and leave out
	 * the following information that will become available later
	 * on when we ask for it from the framebuffer service:
	 *    - framebuffer memory address and length
	 *    - resolution and bpp
	 * We do not ask the framebuffer service for that information
	 * now because we want the allow the user to override the
	 * resolution via the proc entries prior to invoking fb_open.
	 */
	scrn_info->alpha           = DEFAULT_ALPHA;
	scrn_info->keep_resource   = DEFAULT_KEEP_RESOURCE;
	scrn_info->scale           = DEFAULT_SCALE;
	scrn_info->alpha_per_pixel = DEFAULT_ALPHA_PER_PIXEL;
    scrn_info->is_suspended    = FB_BLANK_UNBLANK;

	scrn_info->fb_info.fbops = &vc_fb_ops;

	scrn_info->fb_info.flags         = FBINFO_FLAG_DEFAULT;
	scrn_info->fb_info.fix.type      = FB_TYPE_PACKED_PIXELS;
	scrn_info->fb_info.var.activate  = FB_ACTIVATE_NOW;
	scrn_info->fb_info.var.vmode     = FB_VMODE_NONINTERLACED;

	scrn_info->fb_info.fix.xpanstep  = 0;
	scrn_info->fb_info.fix.ypanstep  = 1;
	scrn_info->fb_info.fix.ywrapstep = 0;
	scrn_info->fb_info.var.rotate    = 0;
	scrn_info->fb_info.var.grayscale = 0;

	memset(scrn_info->fb_info.var.reserved, 0,
		sizeof(scrn_info->fb_info.var.reserved));
	scrn_info->z_order         = z_order;

	scrn_info->alpha_per_pixel = DEFAULT_ALPHA_PER_PIXEL;
	scrn_info->fb_info.fix.accel     = FB_ACCEL_NONE;
	scrn_info->fb_info.fix.visual    = FB_VISUAL_TRUECOLOR;
	if ((num == VC_FB_SCRN_LCD) || (num == VC_FB_SCRN_HDMI)) {
		scrn_info->fb_info.pseudo_palette = scrn_info->cmap;
		scrn_info->fb_info.var.nonstd    = 0;
		strcpy(scrn_info->fb_info.fix.id, "vc_fb");
	} else {
		scrn_info->fb_info.pseudo_palette = NULL;
		scrn_info->fb_info.node = -1;
		scrn_info->fb_info.var.nonstd    = 1;
		snprintf(fb_id, sizeof( fb_id), "vc_ovl%d", (num - VC_FB_SCRN_HDMI));

		strncpy(scrn_info->fb_info.fix.id, fb_id, sizeof(fb_id));
	}

	ret = vc_fb_create_per_scrn_proc_entries(scrn_info,
						fb_state->cfg_directory);
	if (ret != 0) {
		LOG_ERR("fb=%u: failed to create proc entries (ret=%d)",
			num, ret);
		ret = -ENOMEM;
		goto err_free_mem;
	}

	ret = vc_fb_create_per_scrn_debugfs_entry(scrn_info,
						fb_state->debugfs_dir);
	if (ret) {
		LOG_ERR("fb=%u: failed to created debugfs (ret = %d)",
			num, ret);
		/* May be no debugfs support so keep going. */
	}

	ret = register_framebuffer(&scrn_info->fb_info);
	if (ret != 0)  {
		LOG_ERR("fb=%u: register_framebuffer failed (ret=%d)",
			num, ret);
		ret = -ENOMEM;
		goto err_unregister_framebuffer;
	}

	scrn_info->fb_update_count = scrn_info->vc_update_count = 0;

	init_waitqueue_head(&scrn_info->wait_for_vc_status);

	fb_state->scrn_info[num] = scrn_info;
	--z_order;
	goto out;

err_unregister_framebuffer:
	unregister_framebuffer(&scrn_info->fb_info);
err_free_mem:
	kfree(scrn_info);
out:
	LOG_DBG("fb=%u: ..End (ret=%d)", num, ret);
	return ret;
}

static int vc_fb_remove_framebuffer(uint32_t num)
{
	int ret = 0;
	SCRN_INFO_T *scrn_info;

	LOG_DBG("fb=%u: ..Start", num);

	scrn_info = fb_state->scrn_info[num];
	if (scrn_info == NULL) {
		LOG_DBG("fb=%u: no framebuffer device", num);
		ret = -ENODEV;
		goto out;
	}

	vc_fb_remove_per_scrn_proc_entries(scrn_info, fb_state->cfg_directory);
	vc_fb_remove_per_scrn_debugfs_entry(scrn_info);

	unregister_framebuffer(&scrn_info->fb_info);

	kfree(scrn_info);
	fb_state->scrn_info[num] = NULL;

out:
	LOG_DBG("fb=%u: ..End (ret=%d)", num, ret);
	return ret;
}

static int __init vc_fb_init(void)
{
	int ret = 0;
	int open_count, close_count;

	LOG_DBG("..Start");

	/* TODO Check that there is at least one videocore instance */

    fb_state = kzalloc(sizeof(FB_STATE_T), GFP_KERNEL);
    if (fb_state == NULL) {
        LOG_ERR("failed to allocate memory for fb_state");
        ret = -ENOMEM;
        goto out;
    }

    vc_vchiq_fb_init();

    fb_state->cfg_directory = proc_mkdir("vc-fb", NULL);
    if (fb_state->cfg_directory == NULL) {
        LOG_ERR("failed to create proc directory entry");
        ret = -EPERM;
        goto err_free_mem;
    }

    fb_state->debugfs_dir = debugfs_create_dir("vc-fb", NULL);
    if (!fb_state->debugfs_dir) {
        LOG_ERR("failed to create debugfs directory entry");
        /* May be no debugfs support so keep going */
    }

    for (open_count = 0; open_count < MAX_NUM_FB; open_count++) {
        ret = vc_fb_create_framebuffer(open_count);
        if (ret != 0) {
            LOG_ERR("failed to create fb device for screen %u",
                open_count);
            goto err_remove_framebuffer;
        }
    }

    fb_state->fb_inited = 1;
    goto out;

err_remove_framebuffer:
    for (close_count = 0; close_count < open_count; close_count++)
        vc_fb_remove_framebuffer(close_count);
    remove_proc_entry("vc-fb", NULL);
err_free_mem:
    kfree(fb_state);
out:
    LOG_DBG("..End (ret=%d)", ret);
	return ret;
}

static void __exit vc_fb_exit(void)
{
	int i;
	LOG_DBG("..Start");

	if (fb_state->fb_inited) {
		vc_vchiq_fb_deinit();
		for (i = 0; i < VC_FB_SCRN_MAX; i++)
			vc_fb_remove_framebuffer(i);
		remove_proc_entry("vc-fb", NULL);
		debugfs_remove(fb_state->debugfs_dir);
		
		/* Stop the framebuffer service (MJF: MISSING CODE) */

		kfree(fb_state);
	}
	LOG_DBG("..End");
}
/*************** End of Driver module functions ******************************/

late_initcall(vc_fb_init);
module_exit(vc_fb_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("VC Framebuffer Driver");
MODULE_LICENSE("GPL");
