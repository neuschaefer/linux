
/*
 *
 *  Copyright (C)2006  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Arif Khan <arif.khan@st.com> */

#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <asm/hardware/icst307.h>

#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>

#include <asm/mach/arch.h>
#include <asm/arch/irqs.h>
//#include <asm/arch/spearplus.h>
#include <asm/arch/spear.h>
#include <asm/arch/misc_regs.h>
#include <linux/platform_device.h>

#include "core.h"
#include "clock.h"

static void usb_release(struct device *dev)
{
        /* normally not freed */
}


static struct resource udcsyn_resources[] = {
        [0] = {
          .start = (SPEARPLUS_START_USBD_CFG),
          .end   = (SPEARPLUS_START_USBD_CFG + SPEARPLUS_SIZE_USBD_CFG - 1 ),
          .flags = IORESOURCE_MEM,
        },
        [1] = {
          .start = (SPEARPLUS_START_USBD_PLUG),
          .end   = (SPEARPLUS_START_USBD_PLUG + SPEARPLUS_SIZE_USBD_PLUG - 1),
          .flags = IORESOURCE_MEM,
        },
        [2] = {
          .start = (SPEARPLUS_START_USBD_FIFO),
          .end   = (SPEARPLUS_START_USBD_FIFO + SPEARPLUS_SIZE_USBD_FIFO - 1),
          .flags = IORESOURCE_MEM,
        },
        [3] = {
          .start        = IRQ_USB_DEV,
          .end          = IRQ_USB_DEV,
          .flags        = IORESOURCE_IRQ,
        },
};



static u64 udc_dma_mask = ~(u32)0;

static struct platform_device udcsyn_device = {
	.name           = "spear_udc",
	.id             = 1,
	.dev            =  {
	        .release        = usb_release,
	        .dma_mask       = &udc_dma_mask,
	        .coherent_dma_mask      = 0xffffffff,
	},
	.num_resources  = ARRAY_SIZE(udcsyn_resources),
	.resource       = udcsyn_resources,
};

static struct platform_device *spearplus_devs[] __initdata = {
        &udcsyn_device
};


/*
 * CLCD handling
 */
#ifdef CONFIG_FB_ARMCLCD
static struct clk spearplus_clcd_clk = {
	.name   = "CLCDCLK",
	.rate   = 48000000,
	//.params = &spearplus_oscvco_params,
	//.setvco = spearplus_oscvco_set,
};


/*
 * CLCD support.
 */
#ifdef CONFIG_FB_ARMCLCD_SHARP_LQ038Q5DR
static struct clcd_panel sharp_LQ038Q5DR_in = {
        .mode           = {
                .name           = "Sharp QVGA",
                .refresh        = 0,      // ????????????????????
                .xres           = 320,
                .yres           = 240,
                .pixclock       = 10000,  // ?????????????????
                .left_margin    = 60,
                .right_margin   = 8,
                .upper_margin   = 2,
                .lower_margin   = 17,
                .hsync_len      = 12,
                .vsync_len      = 4,
                .sync           = FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
                .vmode          = FB_VMODE_NONINTERLACED,
        },
        .width          = -1,
        .height         = -1,
        .tim2           = TIM2_CLKSEL| 6,
        .cntl           = CNTL_LCDTFT | CNTL_BGR,
        .bpp            = 16,
};
#endif

#ifdef CONFIG_FB_ARMCLCD_SHARP_LQ043T3DX0A
static struct clcd_panel sharp_LQ043T3DX0A_in = {
        .mode           = {
                .name           = "Sharp LQ043T3DX0A",
                .refresh        = 0,      // ????????????????????
                .xres           = 480,
                .yres           = 272,
                .pixclock       = 48000,  // ?????????????????
                .left_margin    = 2,
                .right_margin   = 2,
                .upper_margin   = 2,
                .lower_margin   = 2,
                .hsync_len      = 41,
                .vsync_len      = 11,
                .sync           = 0,
                .vmode          = FB_VMODE_NONINTERLACED,
        },
        .width          = -1,
        .height         = -1,
        .tim2           = TIM2_IOE | TIM2_CLKSEL | 3,
        .cntl           = CNTL_LCDTFT | CNTL_BGR,
        .bpp            = 32, //Bits per pixel
};
#endif

#ifdef CONFIG_FB_ARMCLCD_SHARP_LQ043T1DG01 
static struct clcd_panel sharp_LQ043T1DG01_in = {
        .mode           = {
                .name           = "Sharp LQ043T1DG01",
                .refresh        = 0,      // ????????????????????
                .xres           = 480,
                .yres           = 272,
                .pixclock       = 48000,  // ?????????????????
                .left_margin    = 2,//must be >6
                .right_margin   = 2, //must be >6
                .upper_margin   = 2,
                .lower_margin   = 2,
                .hsync_len      = 41,
                .vsync_len      = 11,
                .sync           = 0, //FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
                .vmode          = FB_VMODE_NONINTERLACED,
        },
        .width          = -1,
        .height         = -1,
        .tim2           = TIM2_IOE | TIM2_CLKSEL | 3, //TIM2_CLKSEL| 4,
        .cntl           = CNTL_LCDTFT | CNTL_BGR,
        .bpp            = 32,
};
#endif

#ifdef CONFIG_FB_ARMCLCD_SAMSUNG_LMS700
static struct clcd_panel samsung_LMS700_in = {
        .mode           = {
                .name           = "Samsung LMS700",
                .refresh        = 0,      // ????????????????????
                .xres           = 800,
                .yres           = 480,
                .pixclock       = 48000,  // ?????????????????
                .left_margin    = 16,
                .right_margin   = 8,
                .upper_margin   = 6,
                .lower_margin   = 5,
                .hsync_len      = 3,
                .vsync_len      = 2,
                .sync           = 0,
                .vmode          = FB_VMODE_NONINTERLACED,
        },
        .width          = -1,
        .height         = -1,
        .tim2           = TIM2_CLKSEL, // This display requires 24MHz
        .cntl           = CNTL_LCDTFT | CNTL_BGR,
        .bpp            = 32, // 24 Bpp
};
#endif


/*
 * Detect which LCD panel is connected, and return the appropriate
 * clcd_panel structure.  Note: we do not have any information on
 * the required timings for the 8.4in panel, so we presently assume
 * VGA timings.
 */
static struct clcd_panel *spearplus_clcd_panel(void)
{
    struct clcd_panel *panel;

    #ifdef CONFIG_FB_ARMCLCD_SHARP_LQ043T1DG01 
    panel = &sharp_LQ043T1DG01_in;
    #endif
    
    #ifdef CONFIG_FB_ARMCLCD_SHARP_LQ043T3DX0A
    panel = &sharp_LQ043T3DX0A_in;
    #endif
    
    #ifdef CONFIG_FB_ARMCLCD_SHARP_LQ038Q5DR
    panel = &sharp_LQ038Q5DR_in;
    #endif
    
    #ifdef CONFIG_FB_ARMCLCD_SAMSUNG_LMS700
    panel = &samsung_LMS700_in;
    #endif

    return panel;
}

//static unsigned long framesize = SZ_1M;
static unsigned long framesize = 0x00180000; /* THis is enough for the size of 800x480 (1.5 MB) */

static int spearplus_clcd_setup(struct clcd_fb *fb)
{
	dma_addr_t dma;
	
	fb->panel               = spearplus_clcd_panel();
	
	fb->fb.screen_base = dma_alloc_writecombine(&fb->dev->dev, framesize,
														&dma, GFP_KERNEL);
	if (!fb->fb.screen_base) {
		printk(KERN_ERR "CLCD: unable to map framebuffer\n");
		return -ENOMEM;
	}
	
	fb->fb.fix.smem_start   = dma;
	fb->fb.fix.smem_len     = framesize;
	
	return 0;
}


static int spearplus_clcd_check(struct clcd_fb *fb, struct fb_var_screeninfo *var)
{
	var->xres_virtual = var->xres = (var->xres + 15) & ~15;
	var->yres_virtual = var->yres = (var->yres + 1) & ~1;
	
	var->nonstd = 0;
	var->accel_flags = 0;
	
	return 0;
}

static void spearplus_clcd_remove(struct clcd_fb *fb)
{
        dma_free_writecombine(&fb->dev->dev, fb->fb.fix.smem_len,
                              fb->fb.screen_base, fb->fb.fix.smem_start);
}

static int spearplus_clcd_mmap(struct clcd_fb *fb, struct vm_area_struct *vma)
{
        return dma_mmap_writecombine(&fb->dev->dev, vma,
                                     fb->fb.screen_base,
                                     fb->fb.fix.smem_start,
                                     fb->fb.fix.smem_len);
}

#if 0
static void spearplus_clcd_enable(struct clcd_fb *fb)
{
	unsigned int cntl = 0, cntl2 = 0;

	/*
	 * Turn the backlight on
	 */
	writel(0x2, VA_SPEARPLUS_START_APB_GPIO1 + 0x400);
	writel(0x2, VA_SPEARPLUS_START_APB_GPIO1 + 0x3FC);

	// read the current CNTL register
	cntl = readl(fb->regs + CLCD_TIM2);

	// Disable it
	cntl2 = cntl & ~TIM2_IOE;
	writel(cntl2, fb->regs + CLCD_TIM2);

	mdelay(50); // wait 20ms

	// Enable it again.
	//cntl |= TIM2_IOE;
	writel(cntl, fb->regs + CLCD_TIM2);

	return;
}

static void spearplus_clcd_disable(struct clcd_fb *fb)
{
	unsigned int cntl = 0;

	// read the current CNTL register
	cntl = readl(fb->regs + CLCD_TIM2);

	// Disable it
	cntl = ~TIM2_IOE;
	writel(cntl, fb->regs + CLCD_TIM2);

	/*
	 * Turn the backlight off
	 */
	writel(0x0, VA_SPEARPLUS_START_APB_GPIO1 + 0x400);
	writel(0x0, VA_SPEARPLUS_START_APB_GPIO1 + 0x3FC);

	return;
}
#endif



static struct clcd_board clcd_plat_data = {
        .name           = "spearplus",
        .check          = spearplus_clcd_check,
        .decode         = clcdfb_decode,
#if 0
        .enable         = spearplus_clcd_enable,
        .disable        = spearplus_clcd_disable,
#endif
        .setup          = spearplus_clcd_setup,
        .mmap           = spearplus_clcd_mmap,
        .remove         = spearplus_clcd_remove,
};

static struct amba_device clcd_device = {
        .dev            = {
                .bus_id = "sh:00",
                .coherent_dma_mask = ~0,
                .platform_data = &clcd_plat_data,
        },
        .res            = {
                .start  = SPEARPLUS_START_CLCD_CTRL,
                .end    = SPEARPLUS_START_CLCD_CTRL + SPEARPLUS_SIZE_CLCD_CTRL - 1,
                .flags  = IORESOURCE_MEM,
        },
        .dma_mask       = ~0,
        .irq            = { IRQ_BASIC_CLCD, NO_IRQ },
        .periphid       = 0,
};
#endif


static struct amba_device *amba_devs[] __initdata = {
#ifdef CONFIG_FB_ARMCLCD
        &clcd_device
#endif
};



static int __init spearplus_pb_init(void)
{
	unsigned int i, ret, val = 0;

	if (machine_is_spearplus()) 
	{
#ifdef CONFIG_FB_ARMCLCD
		/*
		 * Set the LCD clock source to 48MHz.
		 */
		val = readl(PERIP_CLK_CFG) & ~0xc;
		writel(val | 0x4, PERIP_CLK_CFG);
		//writel(0x0, CLCD_CLK_SYNT);

		clk_register(&spearplus_clcd_clk);

#endif

		platform_add_devices(spearplus_devs, ARRAY_SIZE(spearplus_devs));

		for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
			struct amba_device *d = amba_devs[i];

			ret = amba_device_register(d, &iomem_resource);
			if (ret != 0)
				printk ( "AMBA peripheral %s NOT found\n",d->dev.bus_id );
			else
				printk ( "AMBA peripheral %s registered = %d\n",d->dev.bus_id,ret );

		}


	}

	return 0;
}

arch_initcall(spearplus_pb_init);

MACHINE_START(SPEARPLUS, "STM-spearplus")
        /* Maintainer: STM Castelletto - GNOida */
//        .phys_io        = 0xf0000000,
 //       .io_pg_offst    = ((0xf0000000) >> 18) & 0xfffc,
        .boot_params    = 0x00000100,
        .map_io         = spearplus_map_io,
        .init_irq       = spearplus_init_irq,
        .timer          = &spearplus_timer,
   //     .init_machine   = spearplus_pb_init,
MACHINE_END
