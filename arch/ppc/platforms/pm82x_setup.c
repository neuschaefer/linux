/*
 * arch/ppc/platforms/pm82x_setup.c
 *
 * PM82X platform support
 *
 * Author: Heiko Schocher <hs@denx.de>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/ioport.h>
#include <linux/fs_enet_pd.h>
#include <linux/platform_device.h>

#include <asm/io.h>
#include <asm/mpc8260.h>
#include <asm/cpm2.h>
#include <asm/immap_cpm2.h>
#include <asm/irq.h>
#include <asm/ppc_sys.h>
#include <asm/ppcboot.h>
#include <asm/machdep.h>
#include <linux/fs_uart_pd.h>


static void init_smc1_uart_ioports(struct fs_uart_platform_info*);
static void init_smc2_uart_ioports(struct fs_uart_platform_info*);
static void init_scc1_uart_ioports(struct fs_uart_platform_info*);
static void init_scc2_uart_ioports(struct fs_uart_platform_info*);
static void init_scc3_uart_ioports(struct fs_uart_platform_info*);
static void init_scc4_uart_ioports(struct fs_uart_platform_info*);

static int
pm82x_set_rtc_time(unsigned long time)
{
	((cpm2_map_t *)CPM_MAP_ADDR)->im_sit.sit_tmcnt = time;
	((cpm2_map_t *)CPM_MAP_ADDR)->im_sit.sit_tmcntsc = 0x3;
	return(0);
}

static unsigned long
pm82x_get_rtc_time(void)
{
	return ((cpm2_map_t *)CPM_MAP_ADDR)->im_sit.sit_tmcnt;
}

void __init
m82xx_board_init(void)
{
	/* Anything special for this platform */
	ppc_md.set_rtc_time	= pm82x_set_rtc_time;
	ppc_md.get_rtc_time	= pm82x_get_rtc_time;
}

static struct fs_uart_platform_info pm82x_uart_pdata[] = {
	[fsid_smc1_uart] = {
		.init_ioports 	= init_smc1_uart_ioports,
		.fs_no		= 1,
		.fs_type	= "SMC",
		.brg		= 7,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
	[fsid_smc2_uart] = {
		.init_ioports 	= init_smc2_uart_ioports,
		.fs_no		= 2,
		.fs_type	= "SMC",
		.brg		= 8,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
	[fsid_scc1_uart] = {
		.init_ioports 	= init_scc1_uart_ioports,
		.fs_no		= 1,
		.fs_type	= "SCC",
		.brg		= 1,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
	[fsid_scc2_uart] = {
		.init_ioports 	= init_scc2_uart_ioports,
		.fs_no		= 2,
		.fs_type	= "SCC",
		.brg		= 2,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
	[fsid_scc3_uart] = {
		.init_ioports 	= init_scc3_uart_ioports,
		.fs_no		= 3,
		.fs_type	= "SCC",
		.brg		= 3,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
	[fsid_scc4_uart] = {
		.init_ioports 	= init_scc4_uart_ioports,
		.fs_no		= 4,
		.fs_type	= "SCC",
		.brg		= 4,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
};

static void pm82x_fixup_uart_pdata(struct platform_device *pdev,
					      int idx)
{
	bd_t *bd = (bd_t *) __res;
	struct fs_uart_platform_info *pinfo;
	int num = ARRAY_SIZE(pm82x_uart_pdata);
	int id = fs_uart_id_smc2fsid(idx);

	/* no need to alter anything if console */
	if ((id < num) && (!pdev->dev.platform_data)) {
		pinfo = &pm82x_uart_pdata[id];
		pinfo->uart_clk = bd->bi_intfreq;
		pdev->dev.platform_data = pinfo;
	}
}

static void pm82x_fixup_uart_scc_pdata(struct platform_device *pdev,
					      int idx)
{
	bd_t *bd = (bd_t *) __res;
	struct fs_uart_platform_info *pinfo;
	int id = fs_uart_id_scc2fsid(idx);

	pinfo = &pm82x_uart_pdata[id];
	pinfo->uart_clk = bd->bi_intfreq;
	pdev->dev.platform_data = pinfo;
}

static void init_smc1_uart_ioports(struct fs_uart_platform_info *inf)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

        /* SMC1 is only on port D */
	setbits32(&immap->im_ioport.iop_ppard,0x00c00000);
	setbits32(&immap->im_ioport.iop_pdird,0x00400000);
	clrbits32(&immap->im_ioport.iop_pdird,0x00800000);
	clrbits32(&immap->im_ioport.iop_psord,0x00c00000);

        /* Wire BRG7 to SMC1 */
	immap->im_cpmux.cmx_smr &= 0x1f;
	iounmap(immap);
}

static void init_smc2_uart_ioports(struct fs_uart_platform_info *inf)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

        /* SMC2 is only on port A */
	setbits32(&immap->im_ioport.iop_ppard,0x08000000);
	clrbits32(&immap->im_ioport.iop_pdird,0x08000000);
	setbits32(&immap->im_ioport.iop_psord,0x08000000);
	setbits32(&immap->im_ioport.iop_pparc,0x00010000);
	setbits32(&immap->im_ioport.iop_pdirc,0x00010000);
	clrbits32(&immap->im_ioport.iop_psorc,0x00010000);

        /* Wire BRG8 to SMC2 */
	immap->im_cpmux.cmx_smr &= 0xf1;

	iounmap(immap);
}

static void init_scc1_uart_ioports(struct fs_uart_platform_info *inf)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	setbits32(&immap->im_ioport.iop_ppard,0x00000001);
	setbits32(&immap->im_ioport.iop_pparb,0x00000008);
	clrbits32(&immap->im_ioport.iop_psord,0x00000001);
	setbits32(&immap->im_ioport.iop_psorb,0x00000008);
	clrbits32(&immap->im_ioport.iop_pdird,0x00000001);
	setbits32(&immap->im_ioport.iop_pdirb,0x00000008);

        /* Wire BRG1 to SCC1 */
	clrbits32(&immap->im_cpmux.cmx_scr,0x00ffffff);

	iounmap(immap);
}

static void init_scc2_uart_ioports(struct fs_uart_platform_info *inf)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	setbits32(&immap->im_ioport.iop_pparb,0x00010000);
	clrbits32(&immap->im_ioport.iop_pdirb,0x00010000);
	clrbits32(&immap->im_ioport.iop_psorb,0x00010000);
	setbits32(&immap->im_ioport.iop_ppard,0x00000010);
	setbits32(&immap->im_ioport.iop_pdird,0x00000010);
	clrbits32(&immap->im_ioport.iop_psord,0x00000010);

        /* Wire BRG2 to SCC2 */
	clrbits32(&immap->im_cpmux.cmx_scr,0xff00ffff);
	setbits32(&immap->im_cpmux.cmx_scr,0x00090000);

	iounmap(immap);
}

static void init_scc3_uart_ioports(struct fs_uart_platform_info *inf)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	setbits32(&immap->im_ioport.iop_pparb,0x00820000);
	clrbits32(&immap->im_ioport.iop_pdirb,0x00020000);
	clrbits32(&immap->im_ioport.iop_psorb,0x00020000);
	setbits32(&immap->im_ioport.iop_pdirb,0x00800000);
	setbits32(&immap->im_ioport.iop_psord,0x00800000);

        /* Wire BRG3 to SCC3 */
	clrbits32(&immap->im_cpmux.cmx_scr,0xffff00ff);
	setbits32(&immap->im_cpmux.cmx_scr,0x00001200);

	iounmap(immap);
}

static void init_scc4_uart_ioports(struct fs_uart_platform_info *inf)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	setbits32(&immap->im_ioport.iop_ppard,0x00000200);
	clrbits32(&immap->im_ioport.iop_pdird,0x00000200);
	clrbits32(&immap->im_ioport.iop_psord,0x00000200);
	setbits32(&immap->im_ioport.iop_ppard,0x00000400);
	setbits32(&immap->im_ioport.iop_pdird,0x00000400);
	clrbits32(&immap->im_ioport.iop_psord,0x00000400);

        /* Wire BRG4 to SCC4 */
	clrbits32(&immap->im_cpmux.cmx_scr,0xffffff00);
	setbits32(&immap->im_cpmux.cmx_scr,0x0000001b);

	iounmap(immap);
}

static int pm82x_platform_notify(struct device *dev)
{
	static const struct platform_notify_dev_map dev_map[] = {
		{
			.bus_id = "fsl-cpm-smc:uart",
			.rtn = pm82x_fixup_uart_pdata,
		},
		{
			.bus_id = "fsl-cpm-scc:uart",
			.rtn = pm82x_fixup_uart_scc_pdata,
		},
		{
			.bus_id = NULL
		}
	};
	platform_notify_map(dev_map,dev);

	return 0;
}

int __init pm82x_init(void)
{
	printk(KERN_NOTICE "pm82x: Init\n");
	platform_notify = pm82x_platform_notify;

	ppc_sys_device_initfunc();

	ppc_sys_device_disable_all();

	/* to be ready for console, let's attach pdata here */
#ifdef CONFIG_SERIAL_CPM_SMC1
	ppc_sys_device_setfunc(MPC82xx_CPM_SMC1, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SMC1);
#endif

#ifdef CONFIG_SERIAL_CPM_SMC2
	ppc_sys_device_setfunc(MPC82xx_CPM_SMC2, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SMC2);
#endif
#ifdef CONFIG_SERIAL_CPM_SCC1
	ppc_sys_device_setfunc(MPC82xx_CPM_SCC1, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SCC1);
#endif
#ifdef CONFIG_SERIAL_CPM_SCC2
	ppc_sys_device_setfunc(MPC82xx_CPM_SCC2, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SCC2);
#endif
#ifdef CONFIG_SERIAL_CPM_SCC3
	ppc_sys_device_setfunc(MPC82xx_CPM_SCC3, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SCC3);
#endif
#ifdef CONFIG_SERIAL_CPM_SCC4
	ppc_sys_device_setfunc(MPC82xx_CPM_SCC4, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SCC4);
#endif
	return 0;
}

struct platform_device* early_uart_get_pdev(int index)
{
        bd_t *bd = (bd_t *) __res;
        struct fs_uart_platform_info *pinfo;

        struct platform_device* pdev = NULL;
        if(index) { /*assume SMC2 here*/
                pdev = &ppc_sys_platform_devices[MPC82xx_CPM_SMC2];
                pinfo = &pm82x_uart_pdata[1];
        } else { /*over SMC1*/
                pdev = &ppc_sys_platform_devices[MPC82xx_CPM_SMC1];
                pinfo = &pm82x_uart_pdata[0];
        }

        pinfo->uart_clk = bd->bi_intfreq;
        pdev->dev.platform_data = pinfo;
        ppc_sys_fixup_mem_resource(pdev, CPM_MAP_ADDR);
        return NULL;
}

arch_initcall(pm82x_init);
