
/*
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
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
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */

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
#include <linux/interrupt.h>

#include <asm/mach/arch.h>
#include <asm/arch/irqs.h>
//#include <asm/arch/spearbasic.h>
#include <asm/arch/spear.h>
#include <asm/arch/misc_regs.h>
#include <linux/platform_device.h>

#include "core.h"
#include "clock.h"

//#define ENABLE_CLCDENABLE
static unsigned int spearbasic_clcd_reg_virtual = 0;
int spearvsyncdone;
#define CLCD_IRQ 30

static void usb_release(struct device *dev)
{
        /* normally not freed */
}


static struct resource udcsyn_resources[] = {
        [0] = {
          .start = (SPEARBASIC_START_USBD_CFG),
          .end   = (SPEARBASIC_START_USBD_CFG + SPEARBASIC_SIZE_USBD_CFG - 1 ),
          .flags = IORESOURCE_MEM,
        },
        [1] = {
          .start = (SPEARBASIC_START_USBD_PLUG),
          .end   = (SPEARBASIC_START_USBD_PLUG + SPEARBASIC_SIZE_USBD_PLUG - 1),
          .flags = IORESOURCE_MEM,
        },
        [2] = {
          .start = (SPEARBASIC_START_USBD_FIFO),
          .end   = (SPEARBASIC_START_USBD_FIFO + SPEARBASIC_SIZE_USBD_FIFO - 1),
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

static struct platform_device *spearbasic_devs[] __initdata = {
        &udcsyn_device
};


/*
 * CLCD handling
 */
#ifdef CONFIG_FB_ARMCLCD
static struct clk spearbasic_clcd_clk = {
	.name   = "CLCDCLK",
	.rate   = 48000000,
	//.params = &spearbasic_oscvco_params,
	//.setvco = spearbasic_oscvco_set,
};


/*
 * CLCD support.
 */
#ifdef CONFIG_FB_ARMCLCD_SHARP_LQ038Q5DR
static struct clcd_panel sharp_LQ038Q5DR_in = {
        .mode           = {
                .name           = "Sharp QVGA",
                .refresh        = 0,    // ????????????????????
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
                .refresh        = 0,    // ????????????????????
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
        .bpp            = 32, // 24 Bpp
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


#ifdef CONFIG_FB_ARMCLCD_INNOLUX_AT043TN24//SNOM
static struct clcd_panel innolux_at043tn24_in = {
        .mode           = {
                .name           = "Innolux AT043TN24 V.5",
                .refresh        = 0,      // ????????????????????
                .xres           = 480,
                .yres           = 272,
                .pixclock       = 48000,  // ?????????????????
                .left_margin    = 1,//must be >6
                .right_margin   = 1, //must be >6
                .upper_margin   = 0,
                .lower_margin   = 0,
                .hsync_len      = 46,
                .vsync_len      = 9,
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

#ifdef CONFIG_FB_ARMCLCD_EASYTEK_EAGT35S
static struct clcd_panel easytek_EAGT35S_in = {
        .mode           = {
                .name           = "EASYTEK EAGT35S",
                .refresh        = 0,    
                .xres           = 320,
                .yres           = 240,
                .pixclock       = 48000, 
                .left_margin    = 68,
                .right_margin   = 20,
                .upper_margin   = 18,
                .lower_margin   = 4,
                .hsync_len      = 12,
                .vsync_len      = 4,
                .sync           = 0,
                .vmode          = FB_VMODE_NONINTERLACED,
        },
        .width          = -1,
        .height         = -1,
        .tim2           = TIM2_IPC | TIM2_IOE | TIM2_CLKSEL| 5,
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
static struct clcd_panel *spearbasic_clcd_panel(void)
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
    
	#ifdef CONFIG_FB_ARMCLCD_INNOLUX_AT043TN24
	panel = &innolux_at043tn24_in;
	#endif
	
	#ifdef CONFIG_FB_ARMCLCD_EASYTEK_EAGT35S
	panel = &easytek_EAGT35S_in;
	#endif
	
    #ifdef CONFIG_FB_ARMCLCD_SAMSUNG_LMS700
    panel = &samsung_LMS700_in;
    #endif

    return panel;  
}

//static unsigned long framesize = SZ_512K;// SZ_1M;
static unsigned long framesize = 0x00180000; /* THis is enough for the size of 800x480 (1.5 MB) */


static int spearbasic_clcd_setup(struct clcd_fb *fb)
{
	dma_addr_t dma;
	
	fb->panel               = spearbasic_clcd_panel();
	
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


static int spearbasic_clcd_check(struct clcd_fb *fb, struct fb_var_screeninfo *var)
{
	var->xres_virtual = var->xres = (var->xres + 15) & ~15;
	var->yres_virtual = var->yres = (var->yres + 1) & ~1;
	
	var->nonstd = 0;
	var->accel_flags = 0;
	
	return 0;
}

static void spearbasic_clcd_remove(struct clcd_fb *fb)
{
        dma_free_writecombine(&fb->dev->dev, fb->fb.fix.smem_len,
                              fb->fb.screen_base, fb->fb.fix.smem_start);
}

static int spearbasic_clcd_mmap(struct clcd_fb *fb, struct vm_area_struct *vma)
{
        return dma_mmap_writecombine(&fb->dev->dev, vma,
                                     fb->fb.screen_base,
                                     fb->fb.fix.smem_start,
                                     fb->fb.fix.smem_len);
}
#ifdef ENABLE_CLCDENABLE
static volatile unsigned int *LCD_GPIO;

static void DisplaySendByte(unsigned char b)
{
    char i;
    for(i=8;i>0;i--)
    {
        LCD_GPIO[0] &= ~0x2; 
        if(b&(1<<(i-1))) 
        {
            LCD_GPIO[0] |= 0x20; udelay(10);
        }
        else
        {
            LCD_GPIO[0] &= ~0x20; udelay(10);
        }
        LCD_GPIO[0] |= 0x2; udelay(10);
    }
}

static void DisplaySPISelect(void)
{
    LCD_GPIO[0] &= ~0x1; udelay(10);
}

static void DisplaySPIDeselect(void)
{
    LCD_GPIO[0] |= 0x1; udelay(10);
}

static void display_spi_transfer_sgd(u16 word, u8 isDataNotRegister)
{
    DisplaySPISelect();
    if (isDataNotRegister)
    {
        DisplaySendByte(0x72);
    }
    else
    {
        DisplaySendByte(0x70);
    }
    DisplaySendByte(word>>8);
    DisplaySendByte(word&0xFF);
    DisplaySPIDeselect();
}

static void display_setReg_sgd(u16 reg, u16 data)
{
  display_spi_transfer_sgd(reg, 0);
  display_spi_transfer_sgd(data, 1);
}

static void spearbasic_clcd_enable(struct clcd_fb *fb)
{
	LCD_GPIO = 0;
	
    printk("spearbasic_clcd_enable\n");
    LCD_GPIO = ioremap(0x99000000, 8);
    printk("99000000 : %x\n",LCD_GPIO[0]);
    printk("99000004 : %x\n",LCD_GPIO[1]);
    LCD_GPIO[0] = 0x3F04;
    LCD_GPIO[1] = 0x2;
    printk("99000000 : %x\n",LCD_GPIO[0]);
    printk("99000004 : %x\n",LCD_GPIO[1]);
	iounmap(LCD_GPIO);
    
    LCD_GPIO = ioremap(0x60000000, 0x20);
    printk("60000000 : %x\n",LCD_GPIO[0]);
    printk("60000004 : %x\n",LCD_GPIO[1]);
    printk("60000008 : %x\n",LCD_GPIO[2]);
    printk("6000000C : %x\n",LCD_GPIO[3]);
    printk("6000001C : %x\n",LCD_GPIO[0x1C/4]);
	iounmap(LCD_GPIO);
    
    
    
    LCD_GPIO = ioremap(0xA90003FC, 8);
    LCD_GPIO[1] |= 0x73;
    LCD_GPIO[0] |= 0x73;
    
    /* reset */
    LCD_GPIO[0] &= ~0x40;
    mdelay(10);
    LCD_GPIO[0] |= 0x40;
    mdelay(100);
    
    printk("0xA90003FC : %x\n",LCD_GPIO[0]);
    printk("0xA9000400 : %x\n",LCD_GPIO[1]);
    
    display_setReg_sgd(0x01, 0x6300);
    display_setReg_sgd(0x02, 0x0200);
    display_setReg_sgd(0x03, 0x6364);
    display_setReg_sgd(0x04, 0x0447);
    display_setReg_sgd(0x05, 0xB0C4);//0xB3C4
    display_setReg_sgd(0x0A, 0x4008);
    display_setReg_sgd(0x0B, 0xD400);
    display_setReg_sgd(0x0D, 0x3229);
    display_setReg_sgd(0x0E, 0x3200);
    display_setReg_sgd(0x0F, 0x0000);
    display_setReg_sgd(0x16, 0x9F80);
    display_setReg_sgd(0x17, 0x0884);
    display_setReg_sgd(0x1E, 0x00DF);

	iounmap(LCD_GPIO);

	return;
}

static void spearbasic_clcd_disable(struct clcd_fb *fb)
{
    printk("spearbasic_clcd_disable\n");

	return;
}
#endif


static struct clcd_board clcd_plat_data = {
        .name           = "spearbasic",
        .check          = spearbasic_clcd_check,
        .decode         = clcdfb_decode,
#ifdef ENABLE_CLCDENABLE
        .enable         = spearbasic_clcd_enable,
        .disable        = spearbasic_clcd_disable,
#endif
        .setup          = spearbasic_clcd_setup,
        .mmap           = spearbasic_clcd_mmap,
        .remove         = spearbasic_clcd_remove,
};

static struct amba_device clcd_device = {
        .dev            = {
                .bus_id = "sh:00",
                .coherent_dma_mask = ~0,
                .platform_data = &clcd_plat_data,
        },
        .res            = {
                .start  = SPEARBASIC_START_CLCD_CTRL,
                .end    = SPEARBASIC_START_CLCD_CTRL + SPEARBASIC_SIZE_CLCD_CTRL - 1,
                .flags  = IORESOURCE_MEM,
        },
        .dma_mask       = ~0,
        .irq            = {CLCD_IRQ  , NO_IRQ },
        //.irq            = { IRQ_BASIC_CLCD, NO_IRQ },
        .periphid       = 0,
};


static irqreturn_t spearbasic_clcd_vert_int(int irq, void *x)
{
	volatile unsigned int *regp;
	unsigned char src_id;
	wait_queue_head_t * wq = (wait_queue_head_t *)x;	
	
	if (spearbasic_clcd_reg_virtual == 0)
	{
		return IRQ_NONE;
	}
	/* Clear all interrupt bits */
	regp = (unsigned int *)(spearbasic_clcd_reg_virtual + 0x28);
	*regp |= 0x1E;

	regp = (unsigned int *)(spearbasic_clcd_reg_virtual + 0x20);
	src_id = (unsigned char)*regp;
	if (src_id)
	{
		printk(KERN_ERR "CLCD:Error in clearing the interrupt\n");
    }
    spearvsyncdone++;
    wake_up_interruptible(wq);
    
	return IRQ_HANDLED;
}

int spearbasic_clcd_enable_vertical_synchro(struct clcd_fb *fb, wait_queue_head_t * wq)
{
	int err;
	volatile unsigned int *regp;

	spearbasic_clcd_reg_virtual = (unsigned int)fb->regs;
	if (spearbasic_clcd_reg_virtual == 0)
	{
		return -EINVAL;
    }
    
	err = request_irq(CLCD_IRQ, spearbasic_clcd_vert_int, 0, "clcd-vertical-interrupt", (void*)wq);
	if (err) 
	{
		printk(KERN_ERR "CLCD:Error! Failed to register IRQ %i, freeing...\n",
		       CLCD_IRQ);
		free_irq(CLCD_IRQ, (void*)wq);
	}

	/* Set vertical IT event */
	regp = (unsigned int *)(spearbasic_clcd_reg_virtual + 0x1c);
	*regp &= 0xffffcfff;

	/* Enable vertical synchro interrupts  */
	regp = (unsigned int *)(spearbasic_clcd_reg_virtual + 0x18);
	*regp |= 0x08;
    return 0;
}

void spearbasic_clcd_disable_vertical_synchro(wait_queue_head_t * wq)
{
	volatile unsigned int *regp;

    if (spearbasic_clcd_reg_virtual == 0)
	{
		return;
    }
	/* Disable vertical synchro interrupts */
	regp = (unsigned int *)(spearbasic_clcd_reg_virtual + 0x18);
	*regp &= ~0x08;
	spearbasic_clcd_reg_virtual = 0;

	free_irq(CLCD_IRQ, (void*)wq);
	return;
}




#endif


static struct amba_device *amba_devs[] __initdata = {
#ifdef CONFIG_FB_ARMCLCD
        &clcd_device
#endif
};


void SPEArBasic_SetOperationMode(void);
static int __init spearbasic_pb_init(void)
{
	unsigned int i, ret, val = 0;

    printk("spearbasic_pb_init\n");
	if (machine_is_spearbasic()) 
	{
#ifdef CONFIG_FB_ARMCLCD
		/*
		 * Set the LCD clock source to 48MHz.
		 */
		val = readl(PERIP_CLK_CFG) & ~0xc;
		writel(val | 0x4, PERIP_CLK_CFG);
		//writel(0x0, CLCD_CLK_SYNT);

		clk_register(&spearbasic_clcd_clk);

#endif

		platform_add_devices(spearbasic_devs, ARRAY_SIZE(spearbasic_devs));

		for (i = 0; i < ARRAY_SIZE(amba_devs); i++) {
			struct amba_device *d = amba_devs[i];

			ret = amba_device_register(d, &iomem_resource);
			if (ret != 0)
				printk ( "AMBA peripheral %s NOT found\n",d->dev.bus_id );
			else
				printk ( "AMBA peripheral %s registered = %d\n",d->dev.bus_id,ret );

		}


	}
	
	//SPEArBasic_SetOperationMode(); 

	return 0;
}

arch_initcall(spearbasic_pb_init);

MACHINE_START(SPEARBASIC, "snom870")
        /* Maintainer: STM Castelletto - GNOida */
//        .phys_io        = 0xf0000000,
 //       .io_pg_offst    = ((0xf0000000) >> 18) & 0xfffc,
        .boot_params    = 0x00000100,
        .map_io         = spearbasic_map_io,
        .init_irq       = spearbasic_init_irq,
        .timer          = &spearbasic_timer,
   //     .init_machine   = spearbasic_pb_init,
MACHINE_END

typedef enum
{
	SBASIC_NAND_MODE = 0,
	SBASIC_NOR_MODE,
	SBASIC_PHOTO_FRAME_MODE,
	SBASIC_LEND_IP_PHONE_MODE,
	SBASIC_HEND_IP_PHONE_MODE,
	SBASIC_LEND_WIFI_PHONE_MODE,
	SBASIC_HEND_WIFI_PHONE_MODE,
	SBASIC_ATA_PABX_wI2S_MODE,
	SBASIC_ATA_PABX_I2S_MODE,
	SBASIC_CAMl_LCDw_MODE = 12,
	SBASIC_CAMu_LCD_MODE,
	SBASIC_CAMu_LCDw_MODE,
	SBASIC_CAMl_LCD_MODE
}t_OperationMode;

typedef enum
{
	SBASIC_FIRDA 			= ~0x00004000,
	SBASIC_I2C 				= ~0x00002000,
	SBASIC_SSP_ENHANCED		= ~0x00001000,
	SBASIC_SSP_BASIC		= ~0x00000800,
	SBASIC_MII				= ~0x00000400,
	SBASIC_LEG_GPIO			= ~0x000003f0,
	SBASIC_UART_ENHANCED	= ~0x00000008,
	SBASIC_UART_BASIC		= ~0x00000004,
	SBASIC_TIMER_B			= ~0x00000002,
	SBASIC_TIMER_A			= ~0x00000001
}t_FixedIp;

void SPEArBasic_SetOperationMode(void)
{
    volatile unsigned int *va_ras1 = NULL;
	volatile unsigned int *va_ras2 = NULL;

    va_ras1 = ioremap(SPEARBASIC_RAS1, 4);// in 2nd arg is ioremap, 4 is the pagesize
    va_ras2 = ioremap(SPEARBASIC_RAS2, 4);
    
    *va_ras1 = 0x00002ff4;	//Enable all IPS of Fixed Part for SNOM

#ifndef CONFIG_SBASIC_FIRDA
    *va_ras1 &= SBASIC_FIRDA;
#endif
#ifndef CONFIG_SBASIC_I2C
    *va_ras1 &= SBASIC_I2C;
#endif
#ifndef CONFIG_SBASIC_SSP_ENHANCED
    *va_ras1 &= SBASIC_SSP_ENHANCED;
#endif
#ifndef CONFIG_SBASIC_SSP_BASIC
    *va_ras1 &= SBASIC_SSP_BASIC;
#endif
#ifndef CONFIG_SBASIC_MII
    *va_ras1 &= SBASIC_MII;
#endif
#ifndef CONFIG_SBASIC_LEG_GPIO
    *va_ras1 &= SBASIC_LEG_GPIO;
#endif
#ifndef CONFIG_SBASIC_UART_ENHANCED
    *va_ras1 &= SBASIC_UART_ENHANCED;
#endif
#ifndef CONFIG_SBASIC_UART_BASIC
    *va_ras1 &= SBASIC_UART_BASIC;
#endif
#ifndef CONFIG_SBASIC_TIMER_B
    *va_ras1 &= SBASIC_TIMER_B;
#endif
#ifndef CONFIG_SBASIC_TIMER_A
    *va_ras1 &= SBASIC_TIMER_A;
#endif

#ifdef CONFIG_SBASIC_NAND_MODE
	*va_ras2 = SBASIC_NAND_MODE;
#endif
#ifdef CONFIG_SBASIC_NOR_MODE
	*va_ras2 = SBASIC_NOR_MODE;
#endif
#ifdef CONFIG_SBASIC_PHOTO_FRAME_MODE
	*va_ras2 = SBASIC_PHOTO_FRAME_MODE;
#endif
#ifdef CONFIG_SBASIC_LEND_IP_PHONE_MODE
	*va_ras2 = SBASIC_LEND_IP_PHONE_MODE;
#endif
#ifdef CONFIG_SBASIC_HEND_IP_PHONE_MODE
	*va_ras2 = SBASIC_HEND_IP_PHONE_MODE;
#endif
#ifdef CONFIG_SBASIC_LEND_WIFI_PHONE_MODE
	*va_ras2 = SBASIC_LEND_WIFI_PHONE_MODE;
#endif
#ifdef CONFIG_SBASIC_HEND_WIFI_PHONE_MODE
	*va_ras2 = SBASIC_HEND_WIFI_PHONE_MODE;
#endif
#ifdef CONFIG_SBASIC_ATA_PABX_wI2S_MODE
	*va_ras2 = SBASIC_ATA_PABX_wI2S_MODE;
#endif
#ifdef CONFIG_SBASIC_ATA_PABX_I2S_MODE
	*va_ras2 = SBASIC_ATA_PABX_I2S_MODE;
#endif
#ifdef CONFIG_SBASIC_CAMl_LCDw_MODE
	*va_ras2 = SBASIC_CAMl_LCDw_MODE;
#endif
#ifdef CONFIG_SBASIC_CAMu_LCD_MODE
	*va_ras2 = SBASIC_CAMu_LCD_MODE;
#endif
#ifdef CONFIG_SBASIC_CAMu_wLCD_MODE
	*va_ras2 = SBASIC_CAMu_LCDw_MODE;
#endif
#ifdef CONFIG_SBASIC_CAMl_LCD_MODE
	*va_ras2 = SBASIC_CAMl_LCD_MODE;
#endif
//    iounmap((uint)va_ras1);
  //  iounmap((uint)va_ras2);
}
