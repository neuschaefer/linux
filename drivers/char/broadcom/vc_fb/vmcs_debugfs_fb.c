/*
 * Copyright 2012 Roku Corporation.  All rights reserved.
 */

#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <vc_fb_defs.h>
#include "vc_fb.h"

static void showfixedtype(struct seq_file *s, int t)
{
	seq_printf(s, "    TYPE: ");
	switch (t) {
	case FB_TYPE_PACKED_PIXELS:
		seq_printf(s, "Packed Pixels\n");
		break;
	case FB_TYPE_PLANES:
		seq_printf(s, "Planes\n");
		break;
	case FB_TYPE_INTERLEAVED_PLANES:
		seq_printf(s, "Interleaved Planes\n");
		break;
	case FB_TYPE_TEXT:
		seq_printf(s, "Text\n");
		break;
	case FB_TYPE_VGA_PLANES:
		seq_printf(s, "VGA Planes\n");
		break;
	default:
		seq_printf(s, "unknown type %d\n", t);
	}
}

static void showfixedvisual(struct seq_file *s, int v)
{
	seq_printf(s, "  VISUAL: ");
	switch (v) {
	case FB_VISUAL_MONO01:
		seq_printf(s, "Monochrome 1 = black\n");
		break;
	case FB_VISUAL_MONO10:
		seq_printf(s, "Monochrome 1 = white\n");
		break;
	case FB_VISUAL_TRUECOLOR:
		seq_printf(s, "True Color\n");
		break;
	case FB_VISUAL_PSEUDOCOLOR:
		seq_printf(s, "Pseudo color\n");
		break;
	case FB_VISUAL_DIRECTCOLOR:
		seq_printf(s, "Direct color\n");
		break;
	case FB_VISUAL_STATIC_PSEUDOCOLOR:
		seq_printf(s, "Static pseudo color\n");
		break;
	default:
		seq_printf(s, "Unknown visual type %d\n", v);
	}
}

static void showbitfield(struct seq_file *s, const char *label,
			struct fb_bitfield *p)
{
	seq_printf(s, "%15.15s %2d %d\n", label, p->offset, p->length);
}

static void vc_fb_debugfs_show_var(struct seq_file *s,
				struct fb_var_screeninfo *f)
{
	FB_RECT_T *u;
	FB_RECT_T *t;
	if (!f) {

		seq_printf(s, "      no var screeninfo\n");
		return;
	}
	u = (FB_RECT_T *)&f->reserved[0];
	t = (FB_RECT_T *)&f->reserved[2];
	seq_printf(s, "Frame buffer variable information:\n");
	seq_printf(s,
		"visible %dX x %dY, virtual %dX x %dY offset %dX x %dY\n",
		f->xres, f->yres,
		f->xres_virtual, f->yres_virtual,
		f->xoffset, f->yoffset);
	seq_printf(s, "%d bits per pixel\n", f->bits_per_pixel);
	showbitfield(s, "red", &f->red);
	showbitfield(s, "green", &f->green);
	showbitfield(s, "blue", &f->blue);
	showbitfield(s, "transparency", &f->transp);
	seq_printf(s, "pixclock = %d picoseconds\n", f->pixclock);
	seq_printf(s, "vsync len = %d pixclocks\n", f->vsync_len);
	seq_printf(s, "  Source rectangle is %d x %d @ (%d, %d)\n",
		u->w, u->h, u->x, u->y);
	seq_printf(s, "  Destination rectangle is %d x %d @ (%d, %d)\n",
		t->w, t->h, t->x, t->y);
}

static void vc_fb_debugfs_show_fix(struct seq_file *s,
				struct fb_fix_screeninfo *f)
{
	int i;
	if (!f) {
		seq_printf(s, "      no fix screeninfo\n");
		return;
	}
	seq_printf(s, "Frame buffer fixed information:\n");
	seq_printf(s, "      ID: \"");
	for (i = 0; i < 16; i++) {
		if (!f->id[i])
			break;
		seq_printf(s, "%c", f->id[i]);
	}
	seq_printf(s,"\"\n");
	showfixedtype(s, f->type);
	showfixedvisual(s, f->visual);
	seq_printf(s,	"PHYSICAL: %d bytes at 0x%08lx\n",
		f->smem_len,
		f->smem_start);
	if (f->mmio_start && f->mmio_len)
		seq_printf(s, " VIRTUAL: %d bytes at 0x%08lx\n",
			f->mmio_len,
			f->mmio_start);
	else
		seq_printf(s, " VIRTUAL:  Not yet mapped\n");
	seq_printf(s, "   X PAN: %s hardware support\n",
		f->xpanstep ? "has" : "does not have");
	seq_printf(s, "   Y PAN: %s hardware support\n",
		f->ypanstep ? "has" : "does not have");
	seq_printf(s, "  Y WRAP: %s hardware support\n",
		f->ywrapstep ? "has" : "does not have");
	seq_printf(s, "Line Length: %d bytes\n", f->line_length);
	if (f->accel)
		seq_printf(s, "    ACCEL: %d (look it up)\n", f->accel);
	else
		seq_printf(s, "No hardware accelerator specified.\n");
}

static void vc_fb_debugfs_show_videomode(struct seq_file *s, struct fb_videomode *f)
{
	if (!f) {
		seq_printf(s, "    no videomode struct\n");
		return;
	}
	if (f->name)
		seq_printf(s, "name = %s\n", f->name);
	seq_printf(s, "xres = %d\n", f->xres);
	seq_printf(s, "yres = %d\n", f->yres);
	seq_printf(s, "vmode = 0x%08x\n", f->vmode);
	seq_printf(s, "flag = 0x%08x\n", f->flag);
}

static void vc_fb_debugfs_show_fb_info(struct seq_file *s, struct fb_info *f)
{
	if (!f) {
		seq_printf(s, "    no fb info\n");
		return;
	}
//	seq_printf(s, "fb%d count = %d\n", f->count);
	seq_printf(s, "     node = %d\n", f->node);
	seq_printf(s, "     flags = 0x%08x\n", f->flags);
	vc_fb_debugfs_show_var(s, &f->var);
	vc_fb_debugfs_show_fix(s, &f->fix);
	vc_fb_debugfs_show_videomode(s, f->mode);
}

static int vc_fb_debugfs_show(struct seq_file *s, void *data)
{
	SCRN_INFO_T *p =  (SCRN_INFO_T *)s->private;
	if (!p) {
		seq_printf(s, "No data\n");
		return -ENODEV;
	}

	seq_printf(s, "fb%d users %d\n", p->fb_num, p->user_cnt);
	seq_printf(s, "     is_suspended = %d\n", p->is_suspended);
	seq_printf(s, "     fb_update_count = %d\n", p->fb_update_count);
	seq_printf(s, "     vc_update_count = %d\n", p->vc_update_count);
	seq_printf(s, "     res_handle = %p\n", (void *)p->res_handle);
	seq_printf(s, "     is_yuv = %d\n", p->is_yuv);
	seq_printf(s, "     pending_actions = %d\n", p->pending_actions);
	seq_printf(s, "     vc_addr = %p\n", (void *)p->vc_addr);
	seq_printf(s, "     alpha = %d\n", p->alpha);
	seq_printf(s, "     alpha_per_pixel = %d\n", p->alpha_per_pixel);
	seq_printf(s, "     bpp_override = %d\n", p->bpp_override);
	seq_printf(s, "     keep_resource = %d\n", p->keep_resource);
	seq_printf(s, "     scale = %d\n", p->scale);
	seq_printf(s, "     z_order = %d\n", p->z_order);
	seq_printf(s, "     width_override = %d\n", p->width_override);
	seq_printf(s, "     height_override = %d\n", p->height_override);
	seq_printf(s, "     pixel_fmt = %d\n", p->pixel_fmt);
	seq_printf(s, "     color_space = %d\n", p->color_space);
	seq_printf(s, "     buffers = %d\n", p->buffers);
	seq_printf(s, "     instance = %p\n", p->instance);

	vc_fb_debugfs_show_fb_info(s, &p->fb_info);
	return 0;
}

static int vc_fb_debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, vc_fb_debugfs_show,
			inode->i_private);
}

static struct file_operations vc_fb_debugfs_fops = {
	.open		= vc_fb_debugfs_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

int vc_fb_create_per_scrn_debugfs_entry(SCRN_INFO_T *scrn_info,
				struct dentry *fb_debugfs_dir)
{
	struct dentry *entry;
	char entry_name[10];
	sprintf(entry_name, "fb%d", scrn_info->fb_num);
	entry = debugfs_create_file(entry_name, S_IFREG | S_IRUGO,
				fb_debugfs_dir,
				scrn_info, &vc_fb_debugfs_fops);
	if (!entry) {
		LOG_ERR("fb=%u: failed to create debugfs file",
			scrn_info->fb_num);
		return -EPERM;
	}
	/* Save it away for a rainy day */
	scrn_info->debugfs_entry = entry;
	return 0;
}

void vc_fb_remove_per_scrn_debugfs_entry(SCRN_INFO_T *scrn_info)
{
	debugfs_remove(scrn_info->debugfs_entry);
}


static const struct file_operations fb_debugfs_fops;
				
