// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2008-2009 Nuvoton technology corporation.
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * Copyright (C) 2020 Jonathan Neuschäfer
 */

#include <linux/clk.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/gfp.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/mii.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/platform_device.h>
#include <linux/skbuff.h>
#include <linux/of_mdio.h>

#define DRV_MODULE_NAME		"nuvoton-emc"
#define DRV_MODULE_VERSION	"0.2"

/* Ethernet MAC Registers */
#define REG_CAMCMR		0x00	/* CAM Command Register */
#define REG_CAMEN		0x04	/* CAM Enable Register */
#define REG_CAMM_BASE		0x08	/* CAM Most Significant Word Register base */
#define REG_CAML_BASE		0x0c	/* CAM Least Significant Word Register base */
#define REG_TXDLSA		0x88	/* Transmit Descriptor Link List Start Address Register */
#define REG_RXDLSA		0x8C	/* Receive Descriptor Link List Start Address Register */
#define REG_MCMDR		0x90	/* MAC Command Register */
#define REG_MIID		0x94	/* MII Management Data Register */
#define REG_MIIDA		0x98	/* MII Management Control and Address Register */
#define REG_FFTCR		0x9C	/* FIFO Threshold Control Register */
#define REG_TSDR		0xa0	/* Transmit Start Demand Register */
#define REG_RSDR		0xa4	/* Receive Start Demand Register */
#define REG_DMARFC		0xa8	/* Maximum Receive Frame Control Register */
#define REG_MIEN		0xac	/* MAC Interrupt Enable Register */
#define REG_MISTA		0xb0	/* MAC Interrupt Status Register */
#define REG_CTXDSA		0xcc	/* Current Transmit Descriptor Start Address Register */
#define REG_CTXBSA		0xd0	/* Current Transmit Buffer Start Address Register */
#define REG_CRXDSA		0xd4	/* Current Receive Descriptor Start Address Register */
#define REG_CRXBSA		0xd8	/* Current Receive Buffer Start Address Register */

/* mac controller bit */
#define MCMDR_RXON		0x01
#define MCMDR_ACP		(0x01 << 3)
#define MCMDR_SPCRC		(0x01 << 5)
#define MCMDR_TXON		(0x01 << 8)
#define MCMDR_FDUP		(0x01 << 18)
#define MCMDR_ENMDC		(0x01 << 19)
#define MCMDR_OPMOD		(0x01 << 20)
#define SWR			(0x01 << 24)

/* cam command regiser */
#define CAMCMR_AUP		0x01
#define CAMCMR_AMP		(0x01 << 1)
#define CAMCMR_ABP		(0x01 << 2)
#define CAMCMR_CCAM		(0x01 << 3)
#define CAMCMR_ECMP		(0x01 << 4)
#define CAM0EN			0x01

/* mac mii controller bit */
#define MDCCR			(0x0a << 20)
#define PHYAD			(0x01 << 8)
#define PHYWR			(0x01 << 16)
#define PHYBUSY			(0x01 << 17)
#define PHYPRESP		(0x01 << 18)
#define CAM_ENTRY_SIZE		0x08

/* rx and tx status */
#define TXDS_TXCP		(0x01 << 19)
#define RXDS_CRCE		(0x01 << 17)
#define RXDS_PTLE		(0x01 << 19)
#define RXDS_RXGD		(0x01 << 20)
#define RXDS_ALIE		(0x01 << 21)
#define RXDS_RP			(0x01 << 22)

/* mac interrupt status*/
#define MISTA_EXDEF		(0x01 << 19)
#define MISTA_TXBERR		(0x01 << 24)
#define MISTA_TDU		(0x01 << 23)
#define MISTA_RDU		(0x01 << 10)
#define MISTA_RXBERR		(0x01 << 11)

#define ENSTART			0x01
#define ENRXINTR		0x01
#define ENRXGD			(0x01 << 4)
#define ENRXBERR		(0x01 << 11)
#define ENTXINTR		(0x01 << 16)
#define ENTXCP			(0x01 << 18)
#define ENTXABT			(0x01 << 21)
#define ENTXBERR		(0x01 << 24)
#define ENMDC			(0x01 << 19)
#define PHYBUSY			(0x01 << 17)
#define MDCCR_VAL		0xa00000

/* rx and tx owner bit */
#define RX_OWEN_DMA		(0x01 << 31)
#define RX_OWEN_CPU		(~(0x03 << 30))
#define TX_OWEN_DMA		(0x01 << 31)
#define TX_OWEN_CPU		(~(0x01 << 31))

/* tx frame desc controller bit */
#define MACTXINTEN		0x04
#define CRCMODE			0x02
#define PADDINGMODE		0x01

/* fftcr controller bit */
#define TXTHD 			(0x03 << 8)
#define BLENGTH			(0x01 << 20)

/* global setting for driver */
#define RX_DESC_SIZE		50
#define TX_DESC_SIZE		10
#define MAX_RBUFF_SZ		0x600
#define MAX_TBUFF_SZ		0x600
#define TX_TIMEOUT		(HZ/2)
#define DELAY			1000
#define CAM0			0x0

struct nuvoton_emc_rxbd {
	unsigned int sl;
	unsigned int buffer;
	unsigned int reserved;
	unsigned int next;
};

struct nuvoton_emc_txbd {
	unsigned int mode;
	unsigned int buffer;
	unsigned int sl;
	unsigned int next;
};

struct recv_pdesc {
	struct nuvoton_emc_rxbd desclist[RX_DESC_SIZE];
	char recv_buf[RX_DESC_SIZE][MAX_RBUFF_SZ];
};

struct tran_pdesc {
	struct nuvoton_emc_txbd desclist[TX_DESC_SIZE];
	char tran_buf[TX_DESC_SIZE][MAX_TBUFF_SZ];
};

struct nuvoton_emc {
	struct recv_pdesc *rdesc;
	struct tran_pdesc *tdesc;
	dma_addr_t rdesc_phys;
	dma_addr_t tdesc_phys;
	struct platform_device *pdev;
	struct resource *res;
	struct sk_buff *skb;
	struct clk *clk;
	struct clk *rmiiclk;
	void __iomem *reg;
	int rxirq;
	int txirq;
	struct mii_bus *mdio;
	unsigned int cur_tx;
	unsigned int cur_rx;
	unsigned int finish_tx;
	unsigned int rx_packets;
	unsigned int rx_bytes;
	unsigned int start_tx_ptr;
	unsigned int start_rx_ptr;
};

// TODO: hook up to phylink
static void update_linkspeed_register(struct net_device *dev,
				unsigned int speed, unsigned int duplex)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = __raw_readl(emc->reg + REG_MCMDR);

	if (speed == SPEED_100) {
		/* 100 full/half duplex */
		if (duplex == DUPLEX_FULL) {
			val |= (MCMDR_OPMOD | MCMDR_FDUP);
		} else {
			val |= MCMDR_OPMOD;
			val &= ~MCMDR_FDUP;
		}
	} else {
		/* 10 full/half duplex */
		if (duplex == DUPLEX_FULL) {
			val |= MCMDR_FDUP;
			val &= ~MCMDR_OPMOD;
		} else {
			val &= ~(MCMDR_FDUP | MCMDR_OPMOD);
		}
	}

	__raw_writel(val, emc->reg + REG_MCMDR);
}

static void nuvoton_emc_write_cam(struct net_device *dev,
				unsigned int x, unsigned char *pval)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int msw, lsw;

	msw = (pval[0] << 24) | (pval[1] << 16) | (pval[2] << 8) | pval[3];

	lsw = (pval[4] << 24) | (pval[5] << 16);

	__raw_writel(lsw, emc->reg + REG_CAML_BASE + x * CAM_ENTRY_SIZE);
	__raw_writel(msw, emc->reg + REG_CAMM_BASE + x * CAM_ENTRY_SIZE);
}

static int nuvoton_emc_init_desc(struct net_device *dev)
{
	struct nuvoton_emc *emc;
	struct nuvoton_emc_txbd *tdesc;
	struct nuvoton_emc_rxbd *rdesc;
	struct platform_device *pdev;
	unsigned int i;

	emc = netdev_priv(dev);
	pdev = emc->pdev;

	emc->tdesc = dma_alloc_coherent(&pdev->dev, sizeof(struct tran_pdesc),
					  &emc->tdesc_phys, GFP_KERNEL);
	if (!emc->tdesc)
		return -ENOMEM;

	emc->rdesc = dma_alloc_coherent(&pdev->dev, sizeof(struct recv_pdesc),
					  &emc->rdesc_phys, GFP_KERNEL);
	if (!emc->rdesc) {
		dma_free_coherent(&pdev->dev, sizeof(struct tran_pdesc),
				  emc->tdesc, emc->tdesc_phys);
		return -ENOMEM;
	}

	for (i = 0; i < TX_DESC_SIZE; i++) {
		unsigned int offset;

		tdesc = &(emc->tdesc->desclist[i]);

		if (i == TX_DESC_SIZE - 1)
			offset = offsetof(struct tran_pdesc, desclist[0]);
		else
			offset = offsetof(struct tran_pdesc, desclist[i + 1]);

		tdesc->next = emc->tdesc_phys + offset;
		tdesc->buffer = emc->tdesc_phys +
			offsetof(struct tran_pdesc, tran_buf[i]);
		tdesc->sl = 0;
		tdesc->mode = 0;
	}

	emc->start_tx_ptr = emc->tdesc_phys;

	for (i = 0; i < RX_DESC_SIZE; i++) {
		unsigned int offset;

		rdesc = &(emc->rdesc->desclist[i]);

		if (i == RX_DESC_SIZE - 1)
			offset = offsetof(struct recv_pdesc, desclist[0]);
		else
			offset = offsetof(struct recv_pdesc, desclist[i + 1]);

		rdesc->next = emc->rdesc_phys + offset;
		rdesc->sl = RX_OWEN_DMA;
		rdesc->buffer = emc->rdesc_phys +
			offsetof(struct recv_pdesc, recv_buf[i]);
	}

	emc->start_rx_ptr = emc->rdesc_phys;

	return 0;
}

static void nuvoton_emc_set_fifo_threshold(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = TXTHD | BLENGTH;
	__raw_writel(val, emc->reg + REG_FFTCR);
}

static void nuvoton_emc_return_default_idle(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = __raw_readl(emc->reg + REG_MCMDR);
	val |= SWR;
	__raw_writel(val, emc->reg + REG_MCMDR);
}

static void nuvoton_emc_trigger_rx(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	__raw_writel(ENSTART, emc->reg + REG_RSDR);
}

static void nuvoton_emc_trigger_tx(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	__raw_writel(ENSTART, emc->reg + REG_TSDR);
}

static void nuvoton_emc_enable_mac_interrupt(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = ENTXINTR | ENRXINTR | ENRXGD | ENTXCP;
	val |= ENTXBERR | ENRXBERR | ENTXABT;

	__raw_writel(val, emc->reg + REG_MIEN);
}

static void nuvoton_emc_get_and_clear_int(struct net_device *dev,
							unsigned int *val)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	*val = __raw_readl(emc->reg + REG_MISTA);
	__raw_writel(*val, emc->reg + REG_MISTA);
}

static void nuvoton_emc_set_global_maccmd(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = __raw_readl(emc->reg + REG_MCMDR);
	val |= MCMDR_SPCRC | MCMDR_ENMDC | MCMDR_ACP | ENMDC;
	__raw_writel(val, emc->reg + REG_MCMDR);
}

static void nuvoton_emc_enable_cam(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	nuvoton_emc_write_cam(dev, CAM0, dev->dev_addr);

	val = __raw_readl(emc->reg + REG_CAMEN);
	val |= CAM0EN;
	__raw_writel(val, emc->reg + REG_CAMEN);
}

static void nuvoton_emc_enable_cam_command(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = CAMCMR_ECMP | CAMCMR_ABP | CAMCMR_AMP;
	__raw_writel(val, emc->reg + REG_CAMCMR);
}

static void nuvoton_emc_enable_tx(struct net_device *dev, unsigned int enable)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = __raw_readl(emc->reg + REG_MCMDR);

	if (enable)
		val |= MCMDR_TXON;
	else
		val &= ~MCMDR_TXON;

	__raw_writel(val, emc->reg + REG_MCMDR);
}

static void nuvoton_emc_enable_rx(struct net_device *dev, unsigned int enable)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	unsigned int val;

	val = __raw_readl(emc->reg + REG_MCMDR);

	if (enable)
		val |= MCMDR_RXON;
	else
		val &= ~MCMDR_RXON;

	__raw_writel(val, emc->reg + REG_MCMDR);
}

static void nuvoton_emc_set_curdest(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	__raw_writel(emc->start_rx_ptr, emc->reg + REG_RXDLSA);
	__raw_writel(emc->start_tx_ptr, emc->reg + REG_TXDLSA);
}

static void nuvoton_emc_reset_mac(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	nuvoton_emc_enable_tx(dev, 0);
	nuvoton_emc_enable_rx(dev, 0);
	nuvoton_emc_set_fifo_threshold(dev);
	nuvoton_emc_return_default_idle(dev);

	if (!netif_queue_stopped(dev))
		netif_stop_queue(dev);

	nuvoton_emc_init_desc(dev);

	netif_trans_update(dev); /* prevent tx timeout */
	emc->cur_tx = 0x0;
	emc->finish_tx = 0x0;
	emc->cur_rx = 0x0;

	nuvoton_emc_set_curdest(dev);
	nuvoton_emc_enable_cam(dev);
	nuvoton_emc_enable_cam_command(dev);
	nuvoton_emc_enable_mac_interrupt(dev);
	nuvoton_emc_enable_tx(dev, 1);
	nuvoton_emc_enable_rx(dev, 1);
	nuvoton_emc_trigger_tx(dev);
	nuvoton_emc_trigger_rx(dev);

	netif_trans_update(dev); /* prevent tx timeout */

	if (netif_queue_stopped(dev))
		netif_wake_queue(dev);
}

static int nuvoton_emc_set_mac_address(struct net_device *dev, void *addr)
{
	struct sockaddr *address = addr;

	if (!is_valid_ether_addr(address->sa_data))
		return -EADDRNOTAVAIL;

	memcpy(dev->dev_addr, address->sa_data, dev->addr_len);
	nuvoton_emc_write_cam(dev, CAM0, dev->dev_addr);

	return 0;
}

static int nuvoton_emc_close(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	struct platform_device *pdev;

	pdev = emc->pdev;

	dma_free_coherent(&pdev->dev, sizeof(struct recv_pdesc),
					emc->rdesc, emc->rdesc_phys);
	dma_free_coherent(&pdev->dev, sizeof(struct tran_pdesc),
					emc->tdesc, emc->tdesc_phys);

	netif_stop_queue(dev);

	clk_disable(emc->rmiiclk);
	clk_disable(emc->clk);

	free_irq(emc->txirq, dev);
	free_irq(emc->rxirq, dev);

	return 0;
}

static int nuvoton_emc_send_frame(struct net_device *dev,
					unsigned char *data, int length)
{
	struct nuvoton_emc *emc;
	struct nuvoton_emc_txbd *txbd;
	struct platform_device *pdev;
	unsigned char *buffer;

	emc = netdev_priv(dev);
	pdev = emc->pdev;

	txbd = &emc->tdesc->desclist[emc->cur_tx];
	buffer = emc->tdesc->tran_buf[emc->cur_tx];

	if (length > 1514) {
		dev_err(&pdev->dev, "send data %d bytes, check it\n", length);
		length = 1514;
	}

	txbd->sl = length & 0xFFFF;

	memcpy(buffer, data, length);

	txbd->mode = TX_OWEN_DMA | PADDINGMODE | CRCMODE | MACTXINTEN;

	nuvoton_emc_enable_tx(dev, 1);

	nuvoton_emc_trigger_tx(dev);

	if (++emc->cur_tx >= TX_DESC_SIZE)
		emc->cur_tx = 0;

	txbd = &emc->tdesc->desclist[emc->cur_tx];

	if (txbd->mode & TX_OWEN_DMA)
		netif_stop_queue(dev);

	return 0;
}

static int nuvoton_emc_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	if (!(nuvoton_emc_send_frame(dev, skb->data, skb->len))) {
		emc->skb = skb;
		dev_consume_skb_irq(skb);
		return 0;
	}
	return -EAGAIN;
}

static irqreturn_t nuvoton_emc_tx_interrupt(int irq, void *dev_id)
{
	struct nuvoton_emc *emc;
	struct nuvoton_emc_txbd *txbd;
	struct platform_device *pdev;
	struct net_device *dev;
	unsigned int cur_entry, entry, status;

	dev = dev_id;
	emc = netdev_priv(dev);
	pdev = emc->pdev;

	nuvoton_emc_get_and_clear_int(dev, &status);

	cur_entry = __raw_readl(emc->reg + REG_CTXDSA);

	entry = emc->tdesc_phys +
		offsetof(struct tran_pdesc, desclist[emc->finish_tx]);

	while (entry != cur_entry) {
		txbd = &emc->tdesc->desclist[emc->finish_tx];

		if (++emc->finish_tx >= TX_DESC_SIZE)
			emc->finish_tx = 0;

		if (txbd->sl & TXDS_TXCP) {
			dev->stats.tx_packets++;
			dev->stats.tx_bytes += txbd->sl & 0xFFFF;
		} else {
			dev->stats.tx_errors++;
		}

		txbd->sl = 0x0;
		txbd->mode = 0x0;

		if (netif_queue_stopped(dev))
			netif_wake_queue(dev);

		entry = emc->tdesc_phys +
			offsetof(struct tran_pdesc, desclist[emc->finish_tx]);
	}

	if (status & MISTA_EXDEF) {
		dev_err(&pdev->dev, "emc defer exceed interrupt\n");
	} else if (status & MISTA_TXBERR) {
		dev_err(&pdev->dev, "emc bus error interrupt\n");
		nuvoton_emc_reset_mac(dev);
	} else if (status & MISTA_TDU) {
		if (netif_queue_stopped(dev))
			netif_wake_queue(dev);
	}

	return IRQ_HANDLED;
}

static void netdev_rx(struct net_device *dev)
{
	struct nuvoton_emc *emc;
	struct nuvoton_emc_rxbd *rxbd;
	struct platform_device *pdev;
	struct sk_buff *skb;
	unsigned char *data;
	unsigned int length, status, val, entry;

	emc = netdev_priv(dev);
	pdev = emc->pdev;

	rxbd = &emc->rdesc->desclist[emc->cur_rx];

	do {
		val = __raw_readl(emc->reg + REG_CRXDSA);

		entry = emc->rdesc_phys +
			offsetof(struct recv_pdesc, desclist[emc->cur_rx]);

		if (val == entry)
			break;

		status = rxbd->sl;
		length = status & 0xFFFF;

		if (status & RXDS_RXGD) {
			data = emc->rdesc->recv_buf[emc->cur_rx];
			skb = netdev_alloc_skb(dev, length + 2);
			if (!skb) {
				dev->stats.rx_dropped++;
				return;
			}

			skb_reserve(skb, 2);
			skb_put(skb, length);
			skb_copy_to_linear_data(skb, data, length);
			skb->protocol = eth_type_trans(skb, dev);
			dev->stats.rx_packets++;
			dev->stats.rx_bytes += length;
			netif_rx(skb);
		} else {
			dev->stats.rx_errors++;

			if (status & RXDS_RP) {
				dev_err(&pdev->dev, "rx runt err\n");
				dev->stats.rx_length_errors++;
			} else if (status & RXDS_CRCE) {
				dev_err(&pdev->dev, "rx crc err\n");
				dev->stats.rx_crc_errors++;
			} else if (status & RXDS_ALIE) {
				dev_err(&pdev->dev, "rx alignment err\n");
				dev->stats.rx_frame_errors++;
			} else if (status & RXDS_PTLE) {
				dev_err(&pdev->dev, "rx longer err\n");
				dev->stats.rx_over_errors++;
			}
		}

		rxbd->sl = RX_OWEN_DMA;
		rxbd->reserved = 0x0;

		if (++emc->cur_rx >= RX_DESC_SIZE)
			emc->cur_rx = 0;

		rxbd = &emc->rdesc->desclist[emc->cur_rx];

	} while (1);
}

static irqreturn_t nuvoton_emc_rx_interrupt(int irq, void *dev_id)
{
	struct net_device *dev;
	struct nuvoton_emc *emc;
	struct platform_device *pdev;
	unsigned int status;

	dev = dev_id;
	emc = netdev_priv(dev);
	pdev = emc->pdev;

	nuvoton_emc_get_and_clear_int(dev, &status);

	if (status & MISTA_RDU) {
		netdev_rx(dev);
		nuvoton_emc_trigger_rx(dev);

		return IRQ_HANDLED;
	} else if (status & MISTA_RXBERR) {
		dev_err(&pdev->dev, "emc rx bus error\n");
		nuvoton_emc_reset_mac(dev);
	}

	netdev_rx(dev);
	return IRQ_HANDLED;
}

static int nuvoton_emc_open(struct net_device *dev)
{
	struct nuvoton_emc *emc;
	struct platform_device *pdev;

	emc = netdev_priv(dev);
	pdev = emc->pdev;

	nuvoton_emc_reset_mac(dev);
	nuvoton_emc_set_fifo_threshold(dev);
	nuvoton_emc_set_curdest(dev);
	nuvoton_emc_enable_cam(dev);
	nuvoton_emc_enable_cam_command(dev);
	nuvoton_emc_enable_mac_interrupt(dev);
	nuvoton_emc_set_global_maccmd(dev);
	nuvoton_emc_enable_rx(dev, 1);

	clk_enable(emc->rmiiclk);
	clk_enable(emc->clk);

	emc->rx_packets = 0x0;
	emc->rx_bytes = 0x0;

	if (request_irq(emc->txirq, nuvoton_emc_tx_interrupt,
						0x0, pdev->name, dev)) {
		dev_err(&pdev->dev, "register irq tx failed\n");
		return -EAGAIN;
	}

	if (request_irq(emc->rxirq, nuvoton_emc_rx_interrupt,
						0x0, pdev->name, dev)) {
		dev_err(&pdev->dev, "register irq rx failed\n");
		free_irq(emc->txirq, dev);
		return -EAGAIN;
	}

	netif_start_queue(dev);
	nuvoton_emc_trigger_rx(dev);

	dev_info(&pdev->dev, "%s is OPENED\n", dev->name);

	return 0;
}

static void nuvoton_emc_set_multicast_list(struct net_device *dev)
{
	struct nuvoton_emc *emc;
	unsigned int rx_mode;

	emc = netdev_priv(dev);

	if (dev->flags & IFF_PROMISC)
		rx_mode = CAMCMR_AUP | CAMCMR_AMP | CAMCMR_ABP | CAMCMR_ECMP;
	else if ((dev->flags & IFF_ALLMULTI) || !netdev_mc_empty(dev))
		rx_mode = CAMCMR_AMP | CAMCMR_ABP | CAMCMR_ECMP;
	else
		rx_mode = CAMCMR_ECMP | CAMCMR_ABP;
	__raw_writel(rx_mode, emc->reg + REG_CAMCMR);
}

static void nuvoton_emc_get_drvinfo(struct net_device *dev,
					struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_MODULE_VERSION, sizeof(info->version));
}

static const struct ethtool_ops nuvoton_emc_ethtool_ops = {
	.get_drvinfo	= nuvoton_emc_get_drvinfo,
};

static const struct net_device_ops nuvoton_emc_netdev_ops = {
	.ndo_open		= nuvoton_emc_open,
	.ndo_stop		= nuvoton_emc_close,
	.ndo_start_xmit		= nuvoton_emc_start_xmit,
	.ndo_set_rx_mode	= nuvoton_emc_set_multicast_list,
	.ndo_set_mac_address	= nuvoton_emc_set_mac_address,
};

static void get_mac_address(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);
	struct platform_device *pdev;
	char addr[ETH_ALEN];

	pdev = emc->pdev;

	addr[0] = 0x00;
	addr[1] = 0x02;
	addr[2] = 0xac;
	addr[3] = 0x55;
	addr[4] = 0x88;
	addr[5] = 0xa8;

	if (is_valid_ether_addr(addr))
		memcpy(dev->dev_addr, &addr, ETH_ALEN);
	else
		dev_err(&pdev->dev, "invalid mac address\n");
}

static int nuvoton_emc_mdio_write(struct mii_bus *mdio, int phy_id, int reg, u16 data)
{
	struct nuvoton_emc *emc = mdio->priv;
	unsigned int val, i;

	__raw_writel(data, emc->reg + REG_MIID);

	val = (phy_id << 0x08) | reg;
	val |= PHYBUSY | PHYWR | MDCCR_VAL;
	__raw_writel(val, emc->reg + REG_MIIDA);

	for (i = 0; i < DELAY; i++) {
		if ((__raw_readl(emc->reg + REG_MIIDA) & PHYBUSY) == 0)
			break;
	}

	if (i == DELAY)
		dev_warn(&mdio->dev, "mdio write timed out\n");

	return 0;
}

static int nuvoton_emc_mdio_read(struct mii_bus *mdio, int phy_id, int reg)
{
	struct nuvoton_emc *emc = mdio->priv;
	unsigned int val, i, data;

	val = (phy_id << 0x08) | reg;
	val |= PHYBUSY | MDCCR_VAL;
	__raw_writel(val, emc->reg + REG_MIIDA);

	for (i = 0; i < DELAY; i++) {
		if ((__raw_readl(emc->reg + REG_MIIDA) & PHYBUSY) == 0)
			break;
	}

	if (i == DELAY) {
		dev_warn(&mdio->dev, "mdio read timed out\n");
		data = 0xffff;
	} else {
		data = __raw_readl(emc->reg + REG_MIID);
	}

	return data;
}

static int nuvoton_emc_setup(struct net_device *dev)
{
	struct nuvoton_emc *emc = netdev_priv(dev);

	dev->netdev_ops = &nuvoton_emc_netdev_ops;
	dev->ethtool_ops = &nuvoton_emc_ethtool_ops;

	dev->tx_queue_len = 16;
	dev->dma = 0x0;
	dev->watchdog_timeo = TX_TIMEOUT;

	get_mac_address(dev);

	emc->cur_tx = 0x0;
	emc->cur_rx = 0x0;
	emc->finish_tx = 0x0;

	return 0;
}

static int nuvoton_emc_init_mdio(struct nuvoton_emc *emc, struct device_node *np)
{
	struct mii_bus *mdio;
	int res, i;

	if (!np)
		return 0;

	mdio = devm_mdiobus_alloc(&emc->pdev->dev);
	if (!mdio)
		return -ENOMEM;
	emc->mdio = mdio;

	mdio->name = np->full_name;
	snprintf(mdio->id, MII_BUS_ID_SIZE, "%pOF", np);
	mdio->read = nuvoton_emc_mdio_read;
	mdio->write = nuvoton_emc_mdio_write;
	mdio->priv = emc;
	mdio->parent = &emc->pdev->dev;
	for (i = 0; i < PHY_MAX_ADDR; i++)
		mdio->irq[i] = PHY_POLL;

	res = of_mdiobus_register(mdio, np);
	if (res) {
		dev_info(&mdio->dev, "failed to register MDIO bus: %d\n", res);
		return res;
	}

	return 0;
}

static int nuvoton_emc_probe(struct platform_device *pdev)
{
	struct nuvoton_emc *emc;
	struct net_device *dev;
	struct device_node *np = pdev->dev.of_node;
	resource_size_t reg_size;
	int error;

	dev = devm_alloc_etherdev(&pdev->dev, sizeof(struct nuvoton_emc));
	if (!dev)
		return -ENOMEM;
	dev->dev.parent = &pdev->dev;

	emc = netdev_priv(dev);

	emc->reg = devm_of_iomap(&pdev->dev, np, 0, &reg_size);
	if (IS_ERR(emc->reg)) {
		dev_err(&pdev->dev, "failed to map registers: %pe\n", emc->reg);
		return PTR_ERR(emc->reg);
	}
	if (reg_size < 0x100) {
		dev_err(&pdev->dev, "register window is too small: %#x bytes\n", reg_size);
		return -EINVAL;
	}

	emc->txirq = platform_get_irq(pdev, 0);
	if (emc->txirq < 0) {
		dev_err(&pdev->dev, "failed to get ether tx irq\n");
		return -ENXIO;
	}

	emc->rxirq = platform_get_irq(pdev, 1);
	if (emc->rxirq < 0) {
		dev_err(&pdev->dev, "failed to get ether rx irq\n");
		return -ENXIO;
	}

	platform_set_drvdata(pdev, dev);

	emc->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(emc->clk)) {
		dev_err(&pdev->dev, "failed to get ether clock\n");
		return PTR_ERR(emc->clk);
	}

	emc->rmiiclk = devm_clk_get(&pdev->dev, "RMII");
	if (IS_ERR(emc->rmiiclk)) {
		dev_err(&pdev->dev, "failed to get RMII clock\n");
		return PTR_ERR(emc->rmiiclk);
	}

	emc->pdev = pdev;

	nuvoton_emc_setup(dev);

	dev_err(&pdev->dev, "Registering Nuvoton EMC: %px %px %px %px %px\n",
			&pdev->dev, pdev->dev.parent, dev, &dev->dev, dev->dev.parent);
	error = devm_register_netdev(&pdev->dev, dev);
	if (error != 0) {
		dev_err(&pdev->dev, "Registering Nuvoton EMC FAILED\n");
		return -ENODEV;
	}

	nuvoton_emc_init_mdio(emc, of_get_child_by_name(np, "mdio"));

	return 0;
}

static const struct of_device_id nuvoton_emc_of_match[] = {
	{ .compatible = "nuvoton,wpcm450-emc" },
	{}
};
MODULE_DEVICE_TABLE(of, nuvoton_emc_of_match);

static struct platform_driver nuvoton_emc_driver = {
	.probe		= nuvoton_emc_probe,
	.driver		= {
		.name	= "nuvoton-emc",
		.of_match_table = nuvoton_emc_of_match,
	},
};

module_platform_driver(nuvoton_emc_driver);

MODULE_AUTHOR("Wan ZongShun <mcuos.com@gmail.com>");
MODULE_DESCRIPTION("Nuvoton EMC driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:nuvoton-emc");
