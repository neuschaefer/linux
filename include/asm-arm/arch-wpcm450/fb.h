/* linux/include/asm/arch-wpcm450/fb.h
 *
 * Copyright (c) 2006 Winbond Electronics
 *   shirley yu <clyu2@winbond.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 * Changelog:
 *   21-Aug-2006  clyu created file
*/

#ifndef __ASM_ARM_FB_H
#define __ASM_ARM_FB_H

#include <asm/arch/regs-lcd.h>

struct WPCM450fb_val {
	unsigned int	defval;
	unsigned int	min;
	unsigned int	max;
};

struct WPCM450fb_hw {
	unsigned long	lcdcon1;
	unsigned long	lcdcon2;
	unsigned long	lcdcon3;
	unsigned long	lcdcon4;
	unsigned long	lcdcon5;
		unsigned long lcd_dccs;
		unsigned long lcd_device_ctrl;
		unsigned long lcd_mpulcd_cmd;
		unsigned long lcd_int_cs;
		unsigned long lcd_crtc_size;
		unsigned long lcd_crtc_dend;
		unsigned long lcd_crtc_hr;
		unsigned long lcd_crtc_hsync;
		unsigned long lcd_crtc_vr;
		unsigned long lcd_va_baddr0;
		unsigned long lcd_va_baddr1;
		unsigned long lcd_va_fbctrl;
		unsigned long lcd_va_scale;
		unsigned long lcd_va_test;
		unsigned long lcd_va_win;
		unsigned long lcd_va_stuff;
};

struct WPCM450fb_mach_info {
	unsigned char	fixed_syncs;	/* do not update sync/border */

	/* Screen size */
	int		width;
	int		height;

	/* Screen info */
	struct WPCM450fb_val xres;
	struct WPCM450fb_val yres;
	struct WPCM450fb_val bpp;
	
	/* lcd configuration registers */
	struct WPCM450fb_hw  regs;
	
	/* lcd other configuration  */
	u_long		pixclock;
	u_char		hsync_len;
    	u_char		left_margin;
   	u_char		right_margin;

    	u_char		vsync_len;
       u_char		upper_margin;
    	u_char		lower_margin;
    	u_char		sync;

    	u_int		cmap_grayscale:1,
    				cmap_inverse:1,
				cmap_static:1,
		        	unused:29;
    	u_int		state;


};

extern void __init wpcm450_fb_set_platdata(struct WPCM450fb_mach_info *);

#endif /* __ASM_ARM_FB_H */
