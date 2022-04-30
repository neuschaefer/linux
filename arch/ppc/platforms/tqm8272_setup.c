/*
 * TQM8272 platform support
 *
 * Author: Heiko Schocher hs@denx.de
 * Derived from: tqm8260_setup.c
 *
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

#include <asm/machdep.h>
#include <asm/io.h>
#include <asm/mpc8260.h>
#include <asm/cpm2.h>
#include <asm/immap_cpm2.h>
#include <asm/irq.h>
#include <asm/ppc_sys.h>
#include <asm/ppcboot.h>
#include <linux/fs_uart_pd.h>

/* FCC2 Clock Source Configuration.  These can be redefined in the board specific file.
   Can only choose from CLK13-16 */
#define F2_RXCLK	13
#define F2_TXCLK	14

extern int ds1337_set_rtc_time(unsigned long nowtime);
extern ulong ds1337_get_rtc_time(void);

static void init_smc1_uart_ioports(struct fs_uart_platform_info*);
static void init_smc2_uart_ioports(struct fs_uart_platform_info*);
static void init_fcc2_ioports(struct fs_platform_info*);
#if defined(CONFIG_FS_ENET_HAS_SCC)
static void setup_scc1_ioports(struct fs_platform_info*);
#endif

static struct fs_mii_bb_platform_info m82xx_mii_bb_pdata = {
	.mdio_dat.bit	= 16,
	.mdio_dir.bit	= 16,
	.mdc_dat.bit	= 17,
	.delay		= 1,
};

static struct fs_uart_platform_info tqm8272_uart_pdata[] = {
	[fsid_smc1_uart] = {
		.init_ioports 	= init_smc1_uart_ioports,
		.fs_no		= 1,
		.fs_type	= "SMC",
		.brg		= 1,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
	[fsid_smc2_uart] = {
		.init_ioports 	= init_smc2_uart_ioports,
		.fs_no		= 2,
		.fs_type	= "SMC",
		.brg		= 2,
		.tx_num_fifo	= 4,
		.tx_buf_size	= 32,
		.rx_num_fifo	= 4,
		.rx_buf_size	= 32,
	},
};

static struct fs_platform_info mpc82xx_enet_pdata[] = {
	[fsid_fcc2] = {
		.fs_no		= fsid_fcc2,
		.cp_page	= CPM_CR_FCC2_PAGE,
		.cp_block 	= CPM_CR_FCC2_SBLOCK,
		.clk_trx 	= (PC_F2RXCLK | PC_F2TXCLK),
		.clk_route	= CMX2_CLK_ROUTE,
		.clk_mask	= CMX2_CLK_MASK,
		.init_ioports	= init_fcc2_ioports,

		.mem_offset	= FCC2_MEM_OFFSET,

		.rx_ring	= 32,
		.tx_ring	= 32,
		.rx_copybreak	= 240,
		.use_napi	= 0,
		.napi_weight	= 17,
		.bus_id		= "0:00",
	},
#if defined(CONFIG_FS_ENET_HAS_SCC)
	[fsid_scc1] = {
		.rx_ring = 64,
		.tx_ring = 8,
		.rx_copybreak = 240,
		.use_napi = 1,
		.napi_weight = 17,


		.init_ioports = setup_scc1_ioports,

		.bus_id = "fixed@100:1",
		.has_phy = 1,
	},
#endif
};

static void init_fcc2_ioports(struct fs_platform_info *pointer)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	struct io_port *io;
	u32 tempval;

	immap = cpm2_immr;

	io = &immap->im_ioport;

	/* FCC2 are port B/C. */
	/* Configure port A and C pins for FCC2 Ethernet. */
	tempval = in_be32(&io->iop_pdirb);
	tempval &= ~PB2_DIRB0;
	tempval |= PB2_DIRB1;
	out_be32(&io->iop_pdirb, tempval);

	tempval = in_be32(&io->iop_psorb);
	tempval &= ~PB2_PSORB0;
	tempval |= PB2_PSORB1;
	out_be32(&io->iop_psorb, tempval);

	setbits32(&io->iop_pparb,PB2_DIRB0 | PB2_DIRB1);

	tempval = PC_F2RXCLK|PC_F2TXCLK;

	/* Alter clocks */
	clrbits32(&io->iop_psorc,tempval);
	clrbits32(&io->iop_pdirc,tempval);
	setbits32(&io->iop_pparc,tempval);

	clrbits32(&immap->im_cpmux.cmx_fcr, CMX2_CLK_MASK);
	setbits32(&immap->im_cpmux.cmx_fcr, CMX2_CLK_ROUTE);
	iounmap(immap);
}

#if defined(CONFIG_FS_ENET_HAS_SCC)
static void setup_scc1_ioports(struct fs_platform_info *pointer)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	/* Configure port A pins for Txd and Rxd.
	 */
	/* Disable receive and transmit in case EPPC-Bug started it.
	 */
	setbits16(&immap->im_ioport.iop_papar, PA_ENET_RXD | PA_ENET_TXD);
	clrbits16(&immap->im_ioport.iop_padir, PA_ENET_RXD | PA_ENET_TXD);
	clrbits16(&immap->im_ioport.iop_paodr, PA_ENET_TXD);

	/* Configure port C pins to enable CLSN and RENA.
	 */
	clrbits16(&immap->im_ioport.iop_pcpar, PC_ENET_CLSN | PC_ENET_RENA);
	clrbits16(&immap->im_ioport.iop_pcdir, PC_ENET_CLSN | PC_ENET_RENA);
	setbits16(&immap->im_ioport.iop_pcso, PC_ENET_CLSN | PC_ENET_RENA);
	/* Configure port A for TCLK and RCLK.
	 */
	setbits16(&immap->im_ioport.iop_papar, PA_ENET_TCLK | PA_ENET_RCLK);
	clrbits16(&immap->im_ioport.iop_padir, PA_ENET_TCLK | PA_ENET_RCLK);
	clrbits32(&immap->im_cpm.cp_pbpar, PB_ENET_TENA);
	clrbits32(&immap->im_cpm.cp_pbdir, PB_ENET_TENA);

	/* Configure Serial Interface clock routing.
	 * First, clear all SCC bits to zero, then set the ones we want.
	 */
	clrbits32(&immap->im_cpm.cp_sicr, SICR_ENET_MASK);
	setbits32(&immap->im_cpm.cp_sicr, SICR_ENET_CLKRT);

	/* In the original SCC enet driver the following code is placed at
	the end of the initialization */
	setbits32(&immap->im_cpm.cp_pbpar, PB_ENET_TENA);
	setbits32(&immap->im_cpm.cp_pbdir, PB_ENET_TENA);
	iounmap(immap);
}
#endif

static void __init tqm8272_fixup_enet_pdata(struct platform_device *pdev,
					      int idx)
{
	bd_t* bi = (void*)__res;
	int fs_no = fsid_fcc1+pdev->id-1;

	if(fs_no >= ARRAY_SIZE(mpc82xx_enet_pdata)) {
		return;
	}

	mpc82xx_enet_pdata[fs_no].dpram_offset=
			(u32)cpm2_immr->im_dprambase;
	mpc82xx_enet_pdata[fs_no].fcc_regs_c =
			(u32)cpm2_immr->im_fcc_c;
	memcpy(&mpc82xx_enet_pdata[fs_no].macaddr,bi->bi_enetaddr,6);

	/* prevent dup mac */
	if(fs_no == fsid_fcc2)
		mpc82xx_enet_pdata[fs_no].macaddr[5] ^= 1;

	pdev->dev.platform_data = &mpc82xx_enet_pdata[fs_no];
}


static void __init tqm8272_fixup_mdio_pdata(struct platform_device *pdev,
					      int idx)
{
	m82xx_mii_bb_pdata.irq[0] = PHY_INTERRUPT;
	m82xx_mii_bb_pdata.irq[1] = -1;
	m82xx_mii_bb_pdata.irq[2] = -1;
	m82xx_mii_bb_pdata.irq[3] = -1;
	m82xx_mii_bb_pdata.irq[31] = -1;


	m82xx_mii_bb_pdata.mdio_dat.offset =
				(u32)&cpm2_immr->im_ioport.iop_pdatc;

	m82xx_mii_bb_pdata.mdio_dir.offset =
				(u32)&cpm2_immr->im_ioport.iop_pdirc;

	m82xx_mii_bb_pdata.mdc_dat.offset =
				(u32)&cpm2_immr->im_ioport.iop_pdatc;


	pdev->dev.platform_data = &m82xx_mii_bb_pdata;
}

static void init_smc1_uart_ioports(struct fs_uart_platform_info *pointer)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	setbits32(&immap->im_ioport.iop_ppard,0x00c00000);
	setbits32(&immap->im_ioport.iop_ppard,0x00400000);
	clrbits32(&immap->im_ioport.iop_pdird,0x00800000);
	clrbits32(&immap->im_ioport.iop_psord,0x00c00000);

	/* Wire BRG1 to SMC1 */
	immap->im_cpmux.cmx_smr &= 0x0f;
	iounmap(immap);
}

static void init_smc2_uart_ioports(struct fs_uart_platform_info *pointer)
{
	cpm2_map_t* immap = ioremap(CPM_MAP_ADDR, sizeof(cpm2_map_t));

	/* SMC2 is only on port A */
	setbits32(&immap->im_ioport.iop_ppara,0x00c000000);
	clrbits32(&immap->im_ioport.iop_psora,0x00010000);
	clrbits32(&immap->im_ioport.iop_pdira,0x008000000);
	setbits32(&immap->im_ioport.iop_pdira,0x00400000);

	/* Wire BRG2 to SMC2 */
	immap->im_cpmux.cmx_smr &= 0xf0;
	iounmap(immap);
}


static void tqm8272_fixup_uart_pdata(struct platform_device *pdev,
					      int idx)
{
	bd_t *bd = (bd_t *) __res;
	struct fs_uart_platform_info *pinfo;
	int num = ARRAY_SIZE(tqm8272_uart_pdata);
	int id = fs_uart_id_smc2fsid(idx);

	/* no need to alter anything if console */
	if ((id < num) && (!pdev->dev.platform_data)) {
		pinfo = &tqm8272_uart_pdata[id];
		pinfo->uart_clk = bd->bi_intfreq;
		pdev->dev.platform_data = pinfo;
	}
}

static int tqm8272_platform_notify(struct device *dev)
{
	static const struct platform_notify_dev_map dev_map[] = {
		{
			.bus_id = "fsl-cpm-smc:uart",
			.rtn = tqm8272_fixup_uart_pdata,
		},
		{
			.bus_id = "fsl-cpm-fcc",
			.rtn = tqm8272_fixup_enet_pdata,
		},
		{
			.bus_id = "fsl-bb-mdio",
			.rtn = tqm8272_fixup_mdio_pdata,
		},
		{
			.bus_id = NULL
		}
	};
	platform_notify_map(dev_map,dev);

	return 0;

}

int __init tqm8272_init(void)
{
	printk(KERN_NOTICE "tqm8272: Init\n");

	platform_notify = tqm8272_platform_notify;

	ppc_sys_device_initfunc();

	ppc_sys_device_disable_all();

	ppc_sys_device_enable(MPC82xx_MDIO_BB);

#ifdef CONFIG_SERIAL_CPM_SMC1
	ppc_sys_device_setfunc(MPC82xx_CPM_SMC1, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SMC1);
#endif
#ifdef CONFIG_SERIAL_CPM_SMC2
	ppc_sys_device_setfunc(MPC82xx_CPM_SMC2, PPC_SYS_FUNC_UART);
	ppc_sys_device_enable(MPC82xx_CPM_SMC2);
#endif
	ppc_sys_device_enable(MPC82xx_CPM_FCC2);
#if defined(CONFIG_FS_ENET_HAS_SCC)
	ppc_sys_device_enable(MPC82xx_CPM_SCC1);
#endif
	return 0;
}

arch_initcall(tqm8272_init);

/*
   To prevent confusion, console selection is gross:
   by 0 assumed SCC1 and by 1 assumed SCC4
 */
struct platform_device* early_uart_get_pdev(int index)
{
	bd_t *bd = (bd_t *) __res;
	struct fs_uart_platform_info *pinfo;

	struct platform_device* pdev = NULL;
	if(index) { /*assume SMC2 here*/
		pdev = &ppc_sys_platform_devices[MPC82xx_CPM_SMC2];
		pinfo = &tqm8272_uart_pdata[1];
	} else { /*over SMC1*/
		pdev = &ppc_sys_platform_devices[MPC82xx_CPM_SMC1];
		pinfo = &tqm8272_uart_pdata[0];
	}

	pinfo->uart_clk = bd->bi_intfreq;
	pdev->dev.platform_data = pinfo;
	ppc_sys_fixup_mem_resource(pdev, CPM_MAP_ADDR);
	return NULL;
}

/* -------------- RTC ------------------------ */
ulong tqm8272_get_rtc_time(void)
{
	static	int	calls = 0;
	int	result = ds1337_get_rtc_time();

	if (result == -ENODEV) {
		/* DS1337 not initialized, prevent realtime
		   Clock is stuck warning */
		return calls++;
	}
	return result;
}

void __init
m82xx_board_init(void)
{
	/* Anything special for this platform */
	ppc_md.set_rtc_time	= ds1337_set_rtc_time;
	ppc_md.get_rtc_time	= tqm8272_get_rtc_time;
}
