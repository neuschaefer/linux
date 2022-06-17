// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2008-2009 Nuvoton technology corporation.
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * Copyright (C) 2022 Jonathan Neuschäfer
 * TODO:
 * - merge short functions called from _open into one function
 * - get MAC address from DT
 * - change fields in emc_.x_desc to __le32
 * - move away from __raw_ functions maybe
 * - group members of private struct in some reasonable way
 *   - and remove some
 * - use devm_ as much as possible
 * - add reset controller support - compare with openbmc driver
 * - implement copy-less TX/RX DMA
 * - make it short, maybe 700 lines? :)
 * - use dev_err_probe
 * - less printk
 * - set DMARFC
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
#include <linux/reset.h>

#include <net/ncsi.h>

#define DRV_MODULE_NAME		"nuvoton-emc"
#define DRV_MODULE_VERSION	"1.0"

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

/* CAMCMR - CAM Command Regiser */
#define CAMCMR_AUP		BIT(0)
#define CAMCMR_AMP		BIT(1)
#define CAMCMR_ABP		BIT(2)
#define CAMCMR_CCAM		BIT(3)
#define CAMCMR_ECMP		BIT(4)

/* CAMEN - CAM Enable Register */
#define CAMEN_CAM0EN		BIT(0)

#define CAM_ENTRY_SIZE		0x08

/* MCMDR - MAC Command Register */
#define MCMDR_RXON		BIT(0)
#define MCMDR_ACP		BIT(3)
#define MCMDR_SPCRC		BIT(5)
#define MCMDR_TXON		BIT(8)
#define MCMDR_FDUP		BIT(18)
#define MCMDR_ENMDC		BIT(19)
#define MCMDR_OPMOD		BIT(20)
#define MCMDR_SWR		BIT(24)

/* MIIDA - MII Management Control and Address Register */
#define MIIDA_PHYAD		BIT(8)
#define MIIDA_PHYWR		BIT(16)
#define MIIDA_BUSY		BIT(17)
#define MIIDA_PRESP		BIT(18)
#define MIIDA_MDCON		BIT(19)
#define MIIDA_MDCCR_VAL		(0xa << 20)

/* rx and tx status */
#define TXDS_TXCP		BIT(19)
#define RXDS_CRCE		BIT(17)
#define RXDS_PTLE		BIT(19)
#define RXDS_RXGD		BIT(20)
#define RXDS_ALIE		BIT(21)
#define RXDS_RP			BIT(22)

/* MIEN - MAC Interrupt Enable Register */
#define MIEN_RXINTR		BIT(0)
#define MIEN_RXGD		BIT(4)
#define MIEN_RDU		BIT(10)
#define MIEN_RXBERR		BIT(11)
#define MIEN_TXINTR		BIT(16)
#define MIEN_TXCP		BIT(18)
#define MIEN_EXDEF		BIT(19)
#define MIEN_TXABT		BIT(21)
#define MIEN_TDU		BIT(23)
#define MIEN_TXBERR		BIT(24)

/* MISTA - MAC Interrupt Status Register */
#define MISTA_RXINTR		MIEN_RXINTR
#define MISTA_RXGD		MIEN_RXGD
#define MISTA_RDU		MIEN_RDU
#define MISTA_RXBERR		MIEN_RXBERR
#define MISTA_TXINTR		MIEN_TXINTR
#define MISTA_TXCP		MIEN_TXCP
#define MISTA_EXDEF		MIEN_EXDEF
#define MISTA_TXABT		MIEN_TXABT
#define MISTA_TDU		MIEN_TDU
#define MISTA_TXBERR		MIEN_TXBERR

// TODO: rename descriptor bits
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
#define TXTHD			(0x03 << 8)
#define BLENGTH			(0x01 << 20)

/* global setting for driver */
#define RX_DESC_SIZE		50
#define TX_DESC_SIZE		10
#define MAX_RBUFF_SZ		0x600
#define MAX_TBUFF_SZ		0x600
#define TX_TIMEOUT		(HZ/2)
#define DELAY			1000
#define CAM0			0x0

/* Receive buffer descriptor */
struct emc_rx_desc {
	unsigned int sl;
	unsigned int buffer;
	unsigned int reserved;
	unsigned int next;
};

/* Transmit buffer descriptor */
struct emc_tx_desc {
	unsigned int mode;
	unsigned int buffer;
	unsigned int sl;
	unsigned int next;
};

struct emc_rx_descs {
	struct emc_rx_desc desclist[RX_DESC_SIZE];
	char buf[RX_DESC_SIZE][MAX_RBUFF_SZ];
};

struct emc_tx_descs {
	struct emc_tx_desc desclist[TX_DESC_SIZE];
	char buf[TX_DESC_SIZE][MAX_TBUFF_SZ];
};

struct emc_priv {
	struct platform_device *pdev;
	struct resource *res;
	struct clk *clk;
	struct clk *rmiiclk;
	struct reset_control *reset;
	struct mii_bus *mdio;
	struct phy_device *phy;
	struct ncsi_dev *ncsi;

	/* hardware details */
	void __iomem *reg;
	struct emc_rx_descs *rx_descs;
	struct emc_tx_descs *tx_descs;
	dma_addr_t rx_descs_phys;
	dma_addr_t tx_descs_phys;
	int rxirq;
	int txirq;
	unsigned int cur_tx;
	unsigned int cur_rx;
	unsigned int finish_tx;
	unsigned int rx_packets;
	unsigned int rx_bytes;
	unsigned int start_tx_ptr;
	unsigned int start_rx_ptr;
};

static void emc_link_change(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct phy_device *phydev = netdev->phydev;
	unsigned int val;

	val = __raw_readl(priv->reg + REG_MCMDR);

	if (phydev->speed == 100)
		val |= MCMDR_OPMOD;
	else
		val &= ~MCMDR_OPMOD;

	if (phydev->duplex == DUPLEX_FULL)
		val |= MCMDR_FDUP;
	else
		val &= ~MCMDR_FDUP;

	__raw_writel(val, priv->reg + REG_MCMDR);
}

static void emc_write_cam(struct net_device *netdev,
				unsigned int x, const unsigned char *pval)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int msw, lsw;

	msw = (pval[0] << 24) | (pval[1] << 16) | (pval[2] << 8) | pval[3];

	lsw = (pval[4] << 24) | (pval[5] << 16);

	__raw_writel(lsw, priv->reg + REG_CAML_BASE + x * CAM_ENTRY_SIZE);
	__raw_writel(msw, priv->reg + REG_CAMM_BASE + x * CAM_ENTRY_SIZE);
}

static int emc_init_desc(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct emc_tx_desc *tx_desc;
	struct emc_rx_desc *rx_desc;
	struct platform_device *pdev = priv->pdev;
	unsigned int i;

	priv->tx_descs = dma_alloc_coherent(&pdev->dev, sizeof(struct emc_tx_descs),
					  &priv->tx_descs_phys, GFP_KERNEL);
	if (!priv->tx_descs)
		return -ENOMEM;

	priv->rx_descs = dma_alloc_coherent(&pdev->dev, sizeof(struct emc_rx_descs),
					  &priv->rx_descs_phys, GFP_KERNEL);
	if (!priv->rx_descs) {
		dma_free_coherent(&pdev->dev, sizeof(struct emc_tx_descs),
				  priv->tx_descs, priv->tx_descs_phys);
		return -ENOMEM;
	}

	for (i = 0; i < TX_DESC_SIZE; i++) {
		unsigned int offset;

		tx_desc = &(priv->tx_descs->desclist[i]);

		if (i == TX_DESC_SIZE - 1)
			offset = offsetof(struct emc_tx_descs, desclist[0]);
		else
			offset = offsetof(struct emc_tx_descs, desclist[i + 1]);

		tx_desc->next = priv->tx_descs_phys + offset;
		tx_desc->buffer = priv->tx_descs_phys +
			offsetof(struct emc_tx_descs, buf[i]);
		tx_desc->sl = 0;
		tx_desc->mode = 0;
	}

	priv->start_tx_ptr = priv->tx_descs_phys;

	for (i = 0; i < RX_DESC_SIZE; i++) {
		unsigned int offset;

		rx_desc = &(priv->rx_descs->desclist[i]);

		if (i == RX_DESC_SIZE - 1)
			offset = offsetof(struct emc_rx_descs, desclist[0]);
		else
			offset = offsetof(struct emc_rx_descs, desclist[i + 1]);

		rx_desc->next = priv->rx_descs_phys + offset;
		rx_desc->sl = RX_OWEN_DMA;
		rx_desc->buffer = priv->rx_descs_phys +
			offsetof(struct emc_rx_descs, buf[i]);
	}

	priv->start_rx_ptr = priv->rx_descs_phys;

	return 0;
}

static void emc_set_fifo_threshold(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = TXTHD | BLENGTH;
	__raw_writel(val, priv->reg + REG_FFTCR);
}

static void emc_return_default_idle(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = __raw_readl(priv->reg + REG_MCMDR);
	val |= MCMDR_SWR;
	__raw_writel(val, priv->reg + REG_MCMDR);
}

static void emc_trigger_rx(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);

	__raw_writel(1, priv->reg + REG_RSDR);
}

static void emc_trigger_tx(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);

	__raw_writel(1, priv->reg + REG_TSDR);
}

static void emc_enable_mac_interrupt(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = MIEN_TXINTR | MIEN_RXINTR | MIEN_RXGD | MIEN_TXCP;
	val |= MIEN_TXBERR | MIEN_RXBERR | MIEN_TXABT;

	__raw_writel(val, priv->reg + REG_MIEN);
}

static void emc_get_and_clear_int(struct net_device *netdev,
							unsigned int *val)
{
	struct emc_priv *priv = netdev_priv(netdev);

	// TODO: clear interrupts selectively, so we don't clear TX interrupts in the RX ISR
	//       (add mask param)
	*val = __raw_readl(priv->reg + REG_MISTA);
	__raw_writel(*val, priv->reg + REG_MISTA);
}

static void emc_set_global_maccmd(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = __raw_readl(priv->reg + REG_MCMDR);
	val |= MCMDR_SPCRC | MCMDR_ENMDC | MCMDR_ACP;
	__raw_writel(val, priv->reg + REG_MCMDR);
}

static void emc_enable_cam(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	emc_write_cam(netdev, CAM0, netdev->dev_addr);

	val = __raw_readl(priv->reg + REG_CAMEN);
	val |= CAMEN_CAM0EN;
	__raw_writel(val, priv->reg + REG_CAMEN);
}

static void emc_enable_cam_command(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = CAMCMR_ECMP | CAMCMR_ABP | CAMCMR_AMP;
	__raw_writel(val, priv->reg + REG_CAMCMR);
}

static void emc_enable_tx(struct net_device *netdev, unsigned int enable)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = __raw_readl(priv->reg + REG_MCMDR);

	if (enable)
		val |= MCMDR_TXON;
	else
		val &= ~MCMDR_TXON;

	__raw_writel(val, priv->reg + REG_MCMDR);
}

static void emc_enable_rx(struct net_device *netdev, unsigned int enable)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	val = __raw_readl(priv->reg + REG_MCMDR);

	if (enable)
		val |= MCMDR_RXON;
	else
		val &= ~MCMDR_RXON;

	__raw_writel(val, priv->reg + REG_MCMDR);
}

static void emc_set_curdest(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);

	__raw_writel(priv->start_rx_ptr, priv->reg + REG_RXDLSA);
	__raw_writel(priv->start_tx_ptr, priv->reg + REG_TXDLSA);
}

// TODO: consider (1) MCMDR.SWR (software reset), (2) reset controller based reset
// TOOD: rename to reinit or something, don't do a full reset here
static void emc_reset_mac(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);

	// move to probe, add fallback on MCMDR.SWR
	if (priv->reset && false) {
		reset_control_assert(priv->reset);
		reset_control_deassert(priv->reset);
	}

	// TODO: rm hack
	u32 val = __raw_readl(priv->reg + REG_MCMDR);
	val |= MCMDR_ENMDC;
	__raw_writel(val, priv->reg + REG_MCMDR);

	emc_enable_tx(netdev, 0);
	emc_enable_rx(netdev, 0);
	emc_set_fifo_threshold(netdev);
	emc_return_default_idle(netdev);

	if (!netif_queue_stopped(netdev))
		netif_stop_queue(netdev);

	emc_init_desc(netdev);

	netif_trans_update(netdev); /* prevent tx timeout */
	priv->cur_tx = 0x0;
	priv->finish_tx = 0x0;
	priv->cur_rx = 0x0;

	emc_set_curdest(netdev);
	emc_enable_cam(netdev);
	emc_enable_cam_command(netdev);
	emc_enable_mac_interrupt(netdev);
	emc_enable_tx(netdev, 1);
	emc_enable_rx(netdev, 1);
	emc_trigger_tx(netdev);
	emc_trigger_rx(netdev);

	netif_trans_update(netdev); /* prevent tx timeout */

	if (netif_queue_stopped(netdev))
		netif_wake_queue(netdev);
}

static int emc_set_mac_address(struct net_device *netdev, void *addr)
{
	struct sockaddr *address = addr;

	if (!is_valid_ether_addr(address->sa_data))
		return -EADDRNOTAVAIL;

	dev_addr_set(netdev, address->sa_data);
	emc_write_cam(netdev, CAM0, netdev->dev_addr);

	return 0;
}

static int emc_close(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev;

	pdev = priv->pdev;

	dma_free_coherent(&pdev->dev, sizeof(struct emc_rx_descs),
					priv->rx_descs, priv->rx_descs_phys);
	dma_free_coherent(&pdev->dev, sizeof(struct emc_tx_descs),
					priv->tx_descs, priv->tx_descs_phys);

	netif_stop_queue(netdev);

	if (priv->phy)
		phy_stop(priv->phy);
	else if (priv->ncsi)
		ncsi_stop_dev(priv->ncsi);

	clk_disable_unprepare(priv->rmiiclk);
	clk_disable_unprepare(priv->clk);

	free_irq(priv->txirq, netdev);
	free_irq(priv->rxirq, netdev);

	return 0;
}

static int emc_send_frame(struct net_device *netdev,
					unsigned char *data, int length)
{
	struct emc_priv *priv;
	struct emc_tx_desc *tx_desc;
	struct platform_device *pdev;
	unsigned char *buffer;

	priv = netdev_priv(netdev);
	pdev = priv->pdev;

	tx_desc = &priv->tx_descs->desclist[priv->cur_tx];
	buffer = priv->tx_descs->buf[priv->cur_tx];

	if (length > 1514) {
		dev_err(&pdev->dev, "send data %d bytes, check it\n", length);
		length = 1514;
	}

	tx_desc->sl = length & 0xFFFF;

	memcpy(buffer, data, length);

	tx_desc->mode = TX_OWEN_DMA | PADDINGMODE | CRCMODE | MACTXINTEN;

	emc_enable_tx(netdev, 1);

	emc_trigger_tx(netdev);

	if (++priv->cur_tx >= TX_DESC_SIZE)
		priv->cur_tx = 0;

	tx_desc = &priv->tx_descs->desclist[priv->cur_tx];

	if (tx_desc->mode & TX_OWEN_DMA)
		netif_stop_queue(netdev);

	return 0;
}

static int emc_start_xmit(struct sk_buff *skb, struct net_device *netdev)
{
	if (!(emc_send_frame(netdev, skb->data, skb->len))) {
		dev_consume_skb_irq(skb);
		return 0;
	}
	return -EAGAIN;
}

static irqreturn_t emc_tx_interrupt(int irq, void *dev_id)
{
	struct emc_priv *priv;
	struct emc_tx_desc *tx_desc;
	struct platform_device *pdev;
	struct net_device *netdev = dev_id;
	unsigned int cur_entry, entry, status;

	priv = netdev_priv(netdev);
	pdev = priv->pdev;

	emc_get_and_clear_int(netdev, &status);

	cur_entry = __raw_readl(priv->reg + REG_CTXDSA);

	entry = priv->tx_descs_phys +
		offsetof(struct emc_tx_descs, desclist[priv->finish_tx]);

	while (entry != cur_entry) {
		tx_desc = &priv->tx_descs->desclist[priv->finish_tx];

		if (++priv->finish_tx >= TX_DESC_SIZE)
			priv->finish_tx = 0;

		if (tx_desc->sl & TXDS_TXCP) {
			netdev->stats.tx_packets++;
			netdev->stats.tx_bytes += tx_desc->sl & 0xFFFF;
		} else {
			netdev->stats.tx_errors++;
		}

		tx_desc->sl = 0x0;
		tx_desc->mode = 0x0;

		if (netif_queue_stopped(netdev))
			netif_wake_queue(netdev);

		entry = priv->tx_descs_phys +
			offsetof(struct emc_tx_descs, desclist[priv->finish_tx]);
	}

	if (status & MISTA_EXDEF) {
		dev_err(&pdev->dev, "emc defer exceed interrupt\n");
	} else if (status & MISTA_TXBERR) {
		dev_err(&pdev->dev, "emc bus error interrupt\n");
		emc_reset_mac(netdev);
	} else if (status & MISTA_TDU) {
		if (netif_queue_stopped(netdev))
			netif_wake_queue(netdev);
	}

	return IRQ_HANDLED;
}

static void emc_netdev_rx(struct net_device *netdev)
{
	struct emc_priv *priv;
	struct emc_rx_desc *rx_desc;
	struct platform_device *pdev;
	struct sk_buff *skb;
	unsigned char *data;
	unsigned int length, status, val, entry;

	priv = netdev_priv(netdev);
	pdev = priv->pdev;

	rx_desc = &priv->rx_descs->desclist[priv->cur_rx];

	do {
		val = __raw_readl(priv->reg + REG_CRXDSA);

		entry = priv->rx_descs_phys +
			offsetof(struct emc_rx_descs, desclist[priv->cur_rx]);

		if (val == entry)
			break;

		status = rx_desc->sl;
		length = status & 0xFFFF;

		if (status & RXDS_RXGD) {
			data = priv->rx_descs->buf[priv->cur_rx];
			skb = netdev_alloc_skb(netdev, length + 2);
			if (!skb) {
				netdev->stats.rx_dropped++;
				return;
			}

			skb_reserve(skb, 2);
			skb_put(skb, length);
			skb_copy_to_linear_data(skb, data, length);
			skb->protocol = eth_type_trans(skb, netdev);
			netdev->stats.rx_packets++;
			netdev->stats.rx_bytes += length;
			netif_rx(skb);
		} else {
			netdev->stats.rx_errors++;

			if (status & RXDS_RP) {
				dev_err(&pdev->dev, "rx runt err\n");
				netdev->stats.rx_length_errors++;
			} else if (status & RXDS_CRCE) {
				dev_err(&pdev->dev, "rx crc err\n");
				netdev->stats.rx_crc_errors++;
			} else if (status & RXDS_ALIE) {
				dev_err(&pdev->dev, "rx alignment err\n");
				netdev->stats.rx_frame_errors++;
			} else if (status & RXDS_PTLE) {
				dev_err(&pdev->dev, "rx longer err\n");
				netdev->stats.rx_over_errors++;
			}
		}

		rx_desc->sl = RX_OWEN_DMA;
		rx_desc->reserved = 0x0;

		if (++priv->cur_rx >= RX_DESC_SIZE)
			priv->cur_rx = 0;

		rx_desc = &priv->rx_descs->desclist[priv->cur_rx];

	} while (1);
}

static irqreturn_t emc_rx_interrupt(int irq, void *dev_id)
{
	struct net_device *netdev;
	struct emc_priv *priv;
	struct platform_device *pdev;
	unsigned int status;

	netdev = dev_id;
	priv = netdev_priv(netdev);
	pdev = priv->pdev;

	emc_get_and_clear_int(netdev, &status);

	if (status & MISTA_RDU) {
		emc_netdev_rx(netdev);
		emc_trigger_rx(netdev);

		return IRQ_HANDLED;
	} else if (status & MISTA_RXBERR) {
		dev_err(&pdev->dev, "emc rx bus error\n");
		emc_reset_mac(netdev);
	}

	emc_netdev_rx(netdev);
	return IRQ_HANDLED;
}

static int emc_open(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;

	clk_prepare_enable(priv->rmiiclk);
	clk_prepare_enable(priv->clk);

	emc_reset_mac(netdev);
	emc_set_fifo_threshold(netdev);
	emc_set_curdest(netdev);
	emc_enable_cam(netdev);
	emc_enable_cam_command(netdev);
	emc_enable_mac_interrupt(netdev);
	emc_set_global_maccmd(netdev);
	emc_enable_rx(netdev, 1);

	priv->rx_packets = 0x0;
	priv->rx_bytes = 0x0;

	if (request_irq(priv->txirq, emc_tx_interrupt, 0x0, pdev->name, netdev)) {
		dev_err(&pdev->dev, "register irq tx failed\n");
		return -EAGAIN;
	}

	if (request_irq(priv->rxirq, emc_rx_interrupt, 0x0, pdev->name, netdev)) {
		dev_err(&pdev->dev, "register irq rx failed\n");
		free_irq(priv->txirq, netdev);
		return -EAGAIN;
	}

	netif_start_queue(netdev);
	emc_trigger_rx(netdev);

	if (priv->phy)
		phy_start(priv->phy);
	else if (priv->ncsi) {
		netif_carrier_on(netdev);

		int err = ncsi_start_dev(priv->ncsi);
		if (err)
			pr_warn("NC/SI failed, %d\n", err);
	}

	return 0;
}

static void emc_set_multicast_list(struct net_device *netdev)
{
	struct emc_priv *priv;
	unsigned int rx_mode;

	priv = netdev_priv(netdev);

	if (netdev->flags & IFF_PROMISC)
		rx_mode = CAMCMR_AUP | CAMCMR_AMP | CAMCMR_ABP | CAMCMR_ECMP;
	else if ((netdev->flags & IFF_ALLMULTI) || !netdev_mc_empty(netdev))
		rx_mode = CAMCMR_AMP | CAMCMR_ABP | CAMCMR_ECMP;
	else
		rx_mode = CAMCMR_ECMP | CAMCMR_ABP;
	__raw_writel(rx_mode, priv->reg + REG_CAMCMR);
}

static void emc_get_drvinfo(struct net_device *netdev,
					struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_MODULE_VERSION, sizeof(info->version));
}

static const struct ethtool_ops emc_ethtool_ops = {
	.get_drvinfo	= emc_get_drvinfo,
};

static const struct net_device_ops emc_netdev_ops = {
	.ndo_open		= emc_open,
	.ndo_stop		= emc_close,
	.ndo_start_xmit		= emc_start_xmit,
	.ndo_set_rx_mode	= emc_set_multicast_list,
	.ndo_set_mac_address	= emc_set_mac_address,
};

// TODO: use device_get_ethdev_address
static void get_mac_address(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev;
	char addr[ETH_ALEN];

	pdev = priv->pdev;

	addr[0] = 0x00;
	addr[1] = 0x02;
	addr[2] = 0xac;
	addr[3] = 0x55;
	addr[4] = 0x88;
	addr[5] = 0xa8;

	if (is_valid_ether_addr(addr))
		dev_addr_set(netdev, addr);
	else
		dev_err(&pdev->dev, "invalid mac address\n");
}

static int emc_mdio_write(struct mii_bus *mdio, int phy_id, int reg, u16 data)
{
	struct emc_priv *priv = mdio->priv;
	unsigned int val, i;

	clk_prepare_enable(priv->clk);

	__raw_writel(data, priv->reg + REG_MIID);

	val = (phy_id << 0x08) | reg;
	val |= MIIDA_BUSY | MIIDA_PHYWR | MIIDA_MDCCR_VAL;
	__raw_writel(val, priv->reg + REG_MIIDA);

	for (i = 0; i < DELAY; i++) {
		if ((__raw_readl(priv->reg + REG_MIIDA) & MIIDA_BUSY) == 0)
			break;
	}

	if (i == DELAY)
		dev_warn(&mdio->dev, "mdio write timed out\n");

	clk_disable_unprepare(priv->clk);

	return 0;
}

static int emc_mdio_read(struct mii_bus *mdio, int phy_id, int reg)
{
	struct emc_priv *priv = mdio->priv;
	unsigned int val, i, data;

	clk_prepare_enable(priv->clk);

	val = (phy_id << 0x08) | reg;
	val |= MIIDA_BUSY | MIIDA_MDCCR_VAL;
	__raw_writel(val, priv->reg + REG_MIIDA);

	for (i = 0; i < DELAY; i++) {
		if ((__raw_readl(priv->reg + REG_MIIDA) & MIIDA_BUSY) == 0)
			break;
	}

	if (i == DELAY) {
		dev_warn(&mdio->dev, "mdio read timed out\n");
		data = 0xffff;
	} else {
		data = __raw_readl(priv->reg + REG_MIID);
	}

	clk_disable_unprepare(priv->clk);

	return data;
}

static int emc_setup(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);

	netdev->netdev_ops = &emc_netdev_ops;
	netdev->ethtool_ops = &emc_ethtool_ops;

	netdev->tx_queue_len = 16;
	netdev->dma = 0x0;
	netdev->watchdog_timeo = TX_TIMEOUT;

	get_mac_address(netdev);

	priv->cur_tx = 0x0;
	priv->cur_rx = 0x0;
	priv->finish_tx = 0x0;

	return 0;
}

static int emc_init_mdio(struct emc_priv *priv, struct device_node *np)
{
	struct platform_device *pdev = priv->pdev;
	struct mii_bus *mdio;
	int res, i;

	if (!np)
		return 0;

	mdio = devm_mdiobus_alloc(&pdev->dev);
	if (!mdio)
		return -ENOMEM;
	priv->mdio = mdio;

	snprintf(mdio->id, MII_BUS_ID_SIZE, "%s", dev_name(&pdev->dev));
	mdio->name = mdio->id;
	mdio->read = emc_mdio_read;
	mdio->write = emc_mdio_write;
	mdio->priv = priv;
	mdio->parent = &pdev->dev;
	for (i = 0; i < PHY_MAX_ADDR; i++)
		mdio->irq[i] = PHY_POLL;

	// TODO: rm hack
	u32 val = __raw_readl(priv->reg + REG_MCMDR);
	val |= MCMDR_ENMDC;
	__raw_writel(val, priv->reg + REG_MCMDR);

	res = of_mdiobus_register(mdio, np);
	if (res) {
		dev_info(&pdev->dev, "failed to register MDIO bus: %d\n", res);
		return res;
	}

	return 0;
}

static void emc_ncsi_handler(struct ncsi_dev *nd)
{
	if (unlikely(nd->state != ncsi_dev_state_functional))
		return;

	netdev_dbg(nd->dev, "NCSI interface %s\n", nd->link_up ? "up" : "down");
}

static int emc_probe(struct platform_device *pdev)
{
	struct emc_priv *priv;
	struct net_device *netdev;
	struct device_node *np = pdev->dev.of_node;
	resource_size_t reg_size;
	int error;

	netdev = devm_alloc_etherdev(&pdev->dev, sizeof(struct emc_priv));
	if (!netdev)
		return -ENOMEM;
	netdev->dev.parent = &pdev->dev;

	priv = netdev_priv(netdev);

	priv->reg = devm_of_iomap(&pdev->dev, np, 0, &reg_size);
	if (IS_ERR(priv->reg))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->reg),
				     "failed to map registers: %pe\n", priv->reg);

	if (reg_size < 0x100)
		return dev_err_probe(&pdev->dev, -EINVAL, "register window is too small: %#x bytes\n", reg_size);

	priv->txirq = platform_get_irq(pdev, 0);
	if (priv->txirq < 0)
		return dev_err_probe(&pdev->dev, priv->txirq, "failed to get ether tx irq\n");

	priv->rxirq = platform_get_irq(pdev, 1);
	if (priv->rxirq < 0)
		return dev_err_probe(&pdev->dev, priv->rxirq, "failed to get ether rx irq\n");

	platform_set_drvdata(pdev, netdev);

	priv->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->clk), "failed to get ether clock\n");

	priv->rmiiclk = devm_clk_get(&pdev->dev, "RMII");
	if (IS_ERR(priv->rmiiclk))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->rmiiclk), "failed to get RMII clock\n");

	priv->reset = devm_reset_control_get_optional(&pdev->dev, NULL);
	if (IS_ERR(priv->reset))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->reset), "failed to get reset control\n");

	priv->pdev = pdev;

	emc_setup(netdev);

	error = devm_register_netdev(&pdev->dev, netdev);
	if (error != 0)
		return dev_err_probe(&pdev->dev, error, "Registering Nuvoton EMC FAILED\n");

	emc_init_mdio(priv, of_get_child_by_name(np, "mdio"));

	if (np && of_get_property(np, "use-ncsi", NULL)) {
		if (!IS_ENABLED(CONFIG_NET_NCSI))
			return dev_err_probe(&pdev->dev, -EINVAL, "NCSI stack not enabled\n");

		dev_info(&pdev->dev, "Using NCSI interface\n");
		priv->ncsi = ncsi_register_dev(netdev, emc_ncsi_handler);
		if (!priv->ncsi) {
			return -EINVAL;
		}
	} else {
		priv->phy = of_phy_get_and_connect(netdev, np, emc_link_change);
		if (!priv->phy)
			dev_err(&pdev->dev, "failed to connect to PHY!\n");
	}

	return 0;
}

static const struct of_device_id emc_of_match[] = {
	{ .compatible = "nuvoton,wpcm450-emc" },
	{}
};
MODULE_DEVICE_TABLE(of, emc_of_match);

static struct platform_driver emc_driver = {
	.probe		= emc_probe,
	.driver		= {
		.name	= "nuvoton-emc",
		.of_match_table = emc_of_match,
	},
};
module_platform_driver(emc_driver);

MODULE_AUTHOR("Jonathan Neuschäfer <j.neuschaefer@gmx.net>");
MODULE_DESCRIPTION("Nuvoton Ethernet MAC Controller (EMC) driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:nuvoton-emc");
