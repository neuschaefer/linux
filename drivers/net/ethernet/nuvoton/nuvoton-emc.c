// SPDX-License-Identifier: GPL-2.0-only
/*
 * Nuvoton Ethernet MAC Controller (EMC) driver.
 *
 * Copyright (c) 2008-2009 Nuvoton technology corporation.
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * Copyright (C) 2022 Jonathan Neuschäfer
 */

/*
 * Theory of operation:
 *
 * The driver maintains two queues in memory, one for RX and one for TX,
 * which are traversed by the hardware as packets are transmitted/received.
 * Each queue is a cyclical linked list of descriptors, that are owned either
 * by the CPU or the EMC.
 *
 * TX queue:
 *     tx_tail                                            tx_head
 *     v                                                  v
 *    .-----------.    .-----------.    .-----------.    .-----------.
 * .->| OWNER_CPU | .->| OWNER_EMC | .->| OWNER_EMC | .->| OWNER_CPU |
 * |  | buf       | |  | buf       | |  | buf       | |  | buf       |
 * |  | next------|-'  | next------|-'  | next------|-'  | next------|-.
 * |  '-----------'    '-----------'    '-----------'    '-----------' |
 * |                                                                   |
 * '-------------------------------------------------------------------'
 *
 * CPU-owned TX descriptors are checked in the TX ISR and the TX tail pointer
 * is advanced. New TX descriptors are assigned to the EMC at the TX head, when
 * new frames are to be transmitted.
 *
 *
 * RX queue:            rx_tail
 *                      v
 *    .-----------.    .-----------.    .-----------.    .-----------.
 * .->| OWNER_EMC | .->| OWNER_CPU | .->| OWNER_CPU | .->| OWNER_EMC |
 * |  | buf       | |  | buf       | |  | buf       | |  | buf       |
 * |  | next------|-'  | next------|-'  | next------|-'  | next------|-.
 * |  '-----------'    '-----------'    '-----------'    '-----------' |
 * |                                                                   |
 * '-------------------------------------------------------------------'
 *
 * CPU-owned RX descriptors are processed in the RX ISR. They correspond to
 * received frames.
 */

/*
 * Not implemented so far:
 * - copy-less TX/RX DMA
 * - NAPI
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
#include <linux/of_mdio.h>
#include <linux/platform_device.h>
#include <linux/reset.h>
#include <linux/skbuff.h>

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
#define REG_MGSTA		0xb4	/* MAC General Status Register */
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

/* MIEN - MAC Interrupt Enable Register */
#define MIEN_RXINTR		BIT(0)
#define MIEN_PTLE		BIT(3)
#define MIEN_RXGD		BIT(4)
#define MIEN_MMP		BIT(7)
#define MIEN_DFOI		BIT(8)
#define MIEN_RDU		BIT(10)
#define MIEN_RXBERR		BIT(11)
#define MIEN_TXINTR		BIT(16)
#define MIEN_TXCP		BIT(18)
#define MIEN_EXDEF		BIT(19)
#define MIEN_TXABT		BIT(21)
#define MIEN_LC			BIT(22)
#define MIEN_TDU		BIT(23)
#define MIEN_TXBERR		BIT(24)

/* MISTA - MAC Interrupt Status Register */
#define MISTA_RXINTR		MIEN_RXINTR
#define MISTA_PTLE		MIEN_PTLE
#define MISTA_RXGD		MIEN_RXGD
#define MISTA_MMP		MIEN_MMP
#define MISTA_DFOI		MIEN_DFOI
#define MISTA_RDU		MIEN_RDU
#define MISTA_RXBERR		MIEN_RXBERR
#define MISTA_TXINTR		MIEN_TXINTR
#define MISTA_TXCP		MIEN_TXCP
#define MISTA_EXDEF		MIEN_EXDEF
#define MISTA_TXABT		MIEN_TXABT
#define MISTA_LC		MIEN_LC
#define MISTA_TDU		MIEN_TDU
#define MISTA_TXBERR		MIEN_TXBERR
#define MISTA_RX_MASK		0x0000ffff
#define MISTA_RX_ERRORS		(MISTA_TXBERR | MISTA_LC | MISTA_TXABT | MISTA_EXDEF)
#define MISTA_TX_MASK		0xffff0000
#define MISTA_TX_ERRORS		(MISTA_RXBERR | MISTA_DFOI | MISTA_MMP | MISTA_PTLE)

/* MGSTA - MAC General Status Register */
#define MGSTA_RXHA		BIT(1)
#define MGSTA_TXHA		BIT(11)

/* FFTCR - FIFO Threshold Control Register */
#define FFTCR_TXTHD		(0x03 << 8)
#define FFTCR_BLENGTH		(0x01 << 20)

/* RX/TX descriptor, owner */
#define RX_OWNER_MASK		GENMASK(31, 30)
#define RX_OWNER_EMC		BIT(31)
#define TX_OWNER_MASK		BIT(31)
#define TX_OWNER_EMC		BIT(31)
#define TX_OWNER_CPU		0

/* TX descriptor, word 0 bits */
#define TX_PADEN		BIT(0)
#define TX_CRCAPP		BIT(1)
#define TX_INTEN		BIT(2)

/* RX descriptor, status */
#define RX_STATUS_CRCE		BIT(17)
#define RX_STATUS_PTLE		BIT(19)
#define RX_STATUS_RXGD		BIT(20)
#define RX_STATUS_ALIE		BIT(21)
#define RX_STATUS_RP		BIT(22)

/* TX descriptor, status */
#define TX_STATUS_TXCP		BIT(19)

/* Global settings for driver */
#define RX_QUEUE_SIZE		64
#define TX_QUEUE_SIZE		16
#define MAX_RX_FRAME		0x600
#define MAX_TX_FRAME		0x600
#define TX_TIMEOUT		(HZ/2)
#define MDIO_RETRIES		1000

/* Receive buffer descriptor */
struct emc_rx_desc {
	__le32 sl;
	__le32 buffer;
	__le32 reserved;
	__le32 next;
};

/* Transmit buffer descriptor */
struct emc_tx_desc {
	__le32 mode;
	__le32 buffer;
	__le32 sl;
	__le32 next;
};

/* Hardware queue (RX) */
struct emc_rx_queue {
	struct emc_rx_desc desclist[RX_QUEUE_SIZE];
	char buf[RX_QUEUE_SIZE][MAX_RX_FRAME];
};

/* Hardware queue (TX) */
struct emc_tx_queue {
	struct emc_tx_desc desclist[TX_QUEUE_SIZE];
	char buf[TX_QUEUE_SIZE][MAX_TX_FRAME];
};

struct emc_priv {
	/* Subsystem relations and hardware details */
	struct platform_device *pdev;
	struct net_device *netdev;
	struct clk *clk_main;
	struct clk *clk_rmii;
	struct reset_control *reset;
	struct mii_bus *mdio;
	struct phy_device *phy;
	struct ncsi_dev *ncsi;
	int rxirq;
	int txirq;
	void __iomem *reg;

	/* Queues */
	struct emc_rx_queue *rx_queue;
	struct emc_tx_queue *tx_queue;
	dma_addr_t rx_queue_phys;
	dma_addr_t tx_queue_phys;
	unsigned int rx_tail;
	unsigned int tx_head, tx_tail;

	/*
	 * This lock specifically pretects against concurrent changes to the
	 * MCMDR register, especially between reset (in emc_open) and MDIO.
	 */
	struct mutex mcmdr_mutex;
};

static void emc_link_change(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct phy_device *phydev = netdev->phydev;
	unsigned int val;

	mutex_lock(&priv->mcmdr_mutex);
	val = readl(priv->reg + REG_MCMDR);

	if (phydev->speed == 100)
		val |= MCMDR_OPMOD;
	else
		val &= ~MCMDR_OPMOD;

	if (phydev->duplex == DUPLEX_FULL)
		val |= MCMDR_FDUP;
	else
		val &= ~MCMDR_FDUP;

	writel(val, priv->reg + REG_MCMDR);
	mutex_unlock(&priv->mcmdr_mutex);
}

static void emc_hw_set_link_mode_for_ncsi(struct emc_priv *priv)
{
	unsigned int val;

	mutex_lock(&priv->mcmdr_mutex);
	val = readl(priv->reg + REG_MCMDR);

	/* Force 100 Mbit/s and full duplex */
	val |= MCMDR_OPMOD | MCMDR_FDUP;

	writel(val, priv->reg + REG_MCMDR);
	mutex_unlock(&priv->mcmdr_mutex);
}

static void emc_hw_write_cam(struct emc_priv *priv,
			  unsigned int index, const unsigned char *addr)
{
	unsigned int msw, lsw;

	msw = (addr[0] << 24) | (addr[1] << 16) | (addr[2] << 8) | addr[3];
	lsw = (addr[4] << 24) | (addr[5] << 16);

	writel(msw, priv->reg + REG_CAMM_BASE + index * CAM_ENTRY_SIZE);
	writel(lsw, priv->reg + REG_CAML_BASE + index * CAM_ENTRY_SIZE);
}

static int emc_init_desc(struct emc_priv *priv)
{
	struct platform_device *pdev = priv->pdev;
	struct emc_tx_desc *tx_desc;
	struct emc_rx_desc *rx_desc;
	unsigned int i;

	priv->tx_queue = dma_alloc_coherent(&pdev->dev, sizeof(struct emc_tx_queue),
					  &priv->tx_queue_phys, GFP_KERNEL);
	if (!priv->tx_queue)
		return -ENOMEM;

	priv->rx_queue = dma_alloc_coherent(&pdev->dev, sizeof(struct emc_rx_queue),
					  &priv->rx_queue_phys, GFP_KERNEL);
	if (!priv->rx_queue) {
		dma_free_coherent(&pdev->dev, sizeof(struct emc_tx_queue),
				  priv->tx_queue, priv->tx_queue_phys);
		return -ENOMEM;
	}

	for (i = 0; i < TX_QUEUE_SIZE; i++) {
		unsigned int offset;

		tx_desc = &(priv->tx_queue->desclist[i]);

		if (i == TX_QUEUE_SIZE - 1)
			offset = offsetof(struct emc_tx_queue, desclist[0]);
		else
			offset = offsetof(struct emc_tx_queue, desclist[i + 1]);

		tx_desc->next = cpu_to_le32(priv->tx_queue_phys + offset);
		tx_desc->buffer = cpu_to_le32(priv->tx_queue_phys +
			offsetof(struct emc_tx_queue, buf[i]));
		tx_desc->sl = cpu_to_le32(0);
		tx_desc->mode = cpu_to_le32(TX_OWNER_CPU);
	}

	for (i = 0; i < RX_QUEUE_SIZE; i++) {
		unsigned int offset;

		rx_desc = &(priv->rx_queue->desclist[i]);

		if (i == RX_QUEUE_SIZE - 1)
			offset = offsetof(struct emc_rx_queue, desclist[0]);
		else
			offset = offsetof(struct emc_rx_queue, desclist[i + 1]);

		rx_desc->next = cpu_to_le32(priv->rx_queue_phys + offset);
		rx_desc->sl = cpu_to_le32(RX_OWNER_EMC);
		rx_desc->buffer = cpu_to_le32(priv->rx_queue_phys +
			offsetof(struct emc_rx_queue, buf[i]));
	}

	return 0;
}

static void emc_hw_reset(struct emc_priv *priv)
{
	unsigned int val;

	/* Trigger reset */
	val = readl(priv->reg + REG_MCMDR);
	val |= MCMDR_SWR;
	writel(val, priv->reg + REG_MCMDR);

	/* Wait for completion */
	while (readl(priv->reg + REG_MCMDR) & MCMDR_SWR)
		;
}

static void emc_hw_init(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	unsigned int val;

	mutex_lock(&priv->mcmdr_mutex);
	emc_hw_reset(priv);

	/* Set FIFO thresholds */
	val = FFTCR_TXTHD | FFTCR_BLENGTH;
	writel(val, priv->reg + REG_FFTCR);

	/* Enable interrupts */
	val = MIEN_TXINTR | MIEN_RXINTR | MIEN_RXGD | MIEN_TXCP;
	val |= MIEN_TXBERR | MIEN_RXBERR | MIEN_TXABT;
	writel(val, priv->reg + REG_MIEN);

	/* Initialize CAM */
	emc_hw_write_cam(priv, 0, netdev->dev_addr);
	val = CAMEN_CAM0EN;
	writel(val, priv->reg + REG_CAMEN);
	val = CAMCMR_ECMP | CAMCMR_ABP | CAMCMR_AMP;
	writel(val, priv->reg + REG_CAMCMR);

	/* Set max. size of received frames */
	writel(MAX_RX_FRAME, priv->reg + REG_DMARFC);

	/* Initialize MCMDR */
	val = readl(priv->reg + REG_MCMDR);
	val |= MCMDR_SPCRC | MCMDR_ACP;
	writel(val, priv->reg + REG_MCMDR);

	mutex_unlock(&priv->mcmdr_mutex);
}

static void emc_hw_enable_rxtx(struct emc_priv *priv, bool enable)
{
	unsigned int val;

	mutex_lock(&priv->mcmdr_mutex);
	val = readl(priv->reg + REG_MCMDR);

	if (enable)
		val |= MCMDR_RXON | MCMDR_TXON;
	else
		val &= ~(MCMDR_RXON | MCMDR_TXON);

	writel(val, priv->reg + REG_MCMDR);
	mutex_unlock(&priv->mcmdr_mutex);
}

static void emc_hw_trigger_rx(struct emc_priv *priv)
{
	writel(1, priv->reg + REG_RSDR);
}

static void emc_hw_trigger_tx(struct emc_priv *priv)
{
	writel(1, priv->reg + REG_TSDR);
}

static void emc_hw_get_and_clear_int(struct emc_priv *priv,
				  unsigned int mask, unsigned int *val)
{
	*val = readl(priv->reg + REG_MISTA) & mask;
	writel(*val, priv->reg + REG_MISTA);
}

static void emc_hw_enable_mdio(struct emc_priv *priv, bool enable)
{
	unsigned int val;

	val = readl(priv->reg + REG_MCMDR);
	if (enable)
		val |= MCMDR_ENMDC;
	else
		val &= ~MCMDR_ENMDC;
	writel(val, priv->reg + REG_MCMDR);
}

static void emc_set_descriptors(struct emc_priv *priv)
{
	writel(priv->rx_queue_phys, priv->reg + REG_RXDLSA);
	writel(priv->tx_queue_phys, priv->reg + REG_TXDLSA);
}

static int emc_set_mac_address(struct net_device *netdev, void *addr)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct sockaddr *address = addr;

	if (!is_valid_ether_addr(address->sa_data))
		return -EADDRNOTAVAIL;

	dev_addr_set(netdev, address->sa_data);
	emc_hw_write_cam(priv, 0, netdev->dev_addr);

	return 0;
}

static int emc_stop(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;

	if (priv->phy)
		phy_stop(priv->phy);
	else if (priv->ncsi)
		ncsi_stop_dev(priv->ncsi);

	netif_stop_queue(netdev);

	dma_free_coherent(&pdev->dev, sizeof(struct emc_rx_queue),
					priv->rx_queue, priv->rx_queue_phys);
	dma_free_coherent(&pdev->dev, sizeof(struct emc_tx_queue),
					priv->tx_queue, priv->tx_queue_phys);

	clk_disable_unprepare(priv->clk_rmii);
	clk_disable_unprepare(priv->clk_main);

	devm_free_irq(&pdev->dev, priv->txirq, netdev);
	devm_free_irq(&pdev->dev, priv->rxirq, netdev);

	return 0;
}

// TODO: reorder or something
static dma_addr_t emc_tx_phys_desc(struct emc_priv *priv, size_t index);
static dma_addr_t emc_tx_phys_buffer(struct emc_priv *priv, size_t index);

static int emc_start_xmit(struct sk_buff *skb, struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;
	struct emc_tx_desc *desc = &priv->tx_queue->desclist[priv->tx_head];
	unsigned char *buffer = priv->tx_queue->buf[priv->tx_head];
	dma_addr_t dma_buf = emc_tx_phys_buffer(priv, priv->tx_head);
	dma_addr_t dma = emc_tx_phys_desc(priv, priv->tx_head);
	unsigned char *data = skb->data;
	int length = skb->len;

	// TODO: wrong place for a magic number
	// TODO: test how large a frame the hardware can actually handle
	if (length > 1514) {
		dev_err(&pdev->dev, "send data %d bytes, check it\n", length);
		length = 1514;
	}

	memcpy(buffer, data, length);

	dma_sync_single_for_device(&pdev->dev, dma_buf, length, DMA_TO_DEVICE);

	desc->sl = cpu_to_le32(length & 0xffff);
	wmb();
	desc->mode = cpu_to_le32(TX_OWNER_EMC | TX_PADEN | TX_CRCAPP | TX_INTEN);
	dma_sync_single_for_device(&pdev->dev, dma, sizeof(*desc), DMA_TO_DEVICE);

	emc_hw_trigger_tx(priv);

	if (++priv->tx_head >= TX_QUEUE_SIZE)
		priv->tx_head = 0;

	desc = &priv->tx_queue->desclist[priv->tx_head];
	if ((le32_to_cpu(desc->mode) & TX_OWNER_MASK) == TX_OWNER_EMC)
		netif_stop_queue(netdev);

	dev_consume_skb_irq(skb);
	return 0;
}

static irqreturn_t emc_tx_isr(int irq, void *dev_id)
{
	struct net_device *netdev = dev_id;
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;
	unsigned int status;

	emc_hw_get_and_clear_int(priv, MISTA_TX_MASK, &status);

	if (status & MISTA_TX_ERRORS)
		dev_err(&pdev->dev, "TX errors: %#08x\n", status);

	return IRQ_WAKE_THREAD;
}

static dma_addr_t emc_tx_phys_desc(struct emc_priv *priv, size_t index)
{
	return priv->tx_queue_phys + offsetof(struct emc_tx_queue, desclist[index]);
}

static dma_addr_t emc_tx_phys_buffer(struct emc_priv *priv, size_t index)
{
	return priv->tx_queue_phys + offsetof(struct emc_tx_queue, buf[index]);
}

static void emc_update_tx_stats(struct net_device *netdev, const struct emc_tx_desc *desc)
{
	if (le32_to_cpu(desc->sl) & TX_STATUS_TXCP) {
		netdev->stats.tx_packets++;
		netdev->stats.tx_bytes += le32_to_cpu(desc->sl) & 0xffff;
	} else {
		netdev->stats.tx_errors++;
	}
}

static void emc_reset_tx_desc(struct emc_tx_desc *desc)
{
	desc->sl = cpu_to_le32(0);
	wmb();
	desc->mode = cpu_to_le32(0);
}

static irqreturn_t emc_tx_threaded_isr(int irq, void *dev_id)
{
	struct net_device *netdev = dev_id;
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;

	while (priv->tx_tail != priv->tx_head) {
		struct emc_tx_desc *desc = &priv->tx_queue->desclist[priv->tx_tail];
		dma_addr_t dma = emc_tx_phys_desc(priv, priv->tx_tail);

		/* Fetch descriptor */
		dma_sync_single_for_cpu(&pdev->dev, dma, sizeof(*desc), DMA_FROM_DEVICE);

		/* End of loop? */
		if ((le32_to_cpu(READ_ONCE(desc->mode)) & TX_OWNER_MASK) == TX_OWNER_EMC)
			break;
		rmb();

		/* Update statistics */
		emc_update_tx_stats(netdev, desc);

		/* Return descriptor */
		emc_reset_tx_desc(desc);
		dma_sync_single_for_device(&pdev->dev, dma, sizeof(*desc), DMA_TO_DEVICE);

		emc_hw_trigger_tx(priv);

		priv->tx_tail = (priv->tx_tail + 1) % TX_QUEUE_SIZE;
	}

	if (netif_queue_stopped(netdev))
		netif_wake_queue(netdev);

	return IRQ_HANDLED;
}

static irqreturn_t emc_rx_isr(int irq, void *dev_id)
{
	struct net_device *netdev = dev_id;
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;
	unsigned int status;

	emc_hw_get_and_clear_int(priv, MISTA_RX_MASK, &status);

	if (status & MISTA_RX_ERRORS)
		dev_err(&pdev->dev, "RX errors: %#08x\n", status);

	return IRQ_WAKE_THREAD;
}

static dma_addr_t emc_rx_phys_desc(struct emc_priv *priv, size_t index)
{
	return priv->rx_queue_phys + offsetof(struct emc_rx_queue, desclist[index]);
}

static void emc_update_rx_error_stats(struct net_device *netdev, u32 status)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;

	netdev->stats.rx_errors++;

	if (status & RX_STATUS_RP) {
		dev_err(&pdev->dev, "rx runt err\n");
		netdev->stats.rx_length_errors++;
	} else if (status & RX_STATUS_CRCE) {
		dev_err(&pdev->dev, "rx crc err\n");
		netdev->stats.rx_crc_errors++;
	} else if (status & RX_STATUS_ALIE) {
		dev_err(&pdev->dev, "rx alignment err\n");
		netdev->stats.rx_frame_errors++;
	} else if (status & RX_STATUS_PTLE) {
		dev_err(&pdev->dev, "rx longer err\n");
		netdev->stats.rx_over_errors++;
	}
}

static void emc_reset_rx_desc(struct emc_rx_desc *desc)
{
	desc->reserved = cpu_to_le32(0);
	rmb();
	desc->sl = cpu_to_le32(RX_OWNER_EMC);
}

static void emc_netdev_rx(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;

	while (true) {
		struct emc_rx_desc *desc = &priv->rx_queue->desclist[priv->rx_tail];
		dma_addr_t dma = emc_rx_phys_desc(priv, priv->rx_tail);
		u32 status, length;

		/* Fetch descriptor */
		dma_sync_single_for_cpu(&pdev->dev, dma, sizeof(*desc), DMA_FROM_DEVICE);
		status = le32_to_cpu(desc->sl);
		length = status & 0xffff;
		rmb();

		/* End of loop? */
		if ((status & RX_OWNER_MASK) == RX_OWNER_EMC)
			break;

		/* Receive packet or report error */
		if (status & RX_STATUS_RXGD) {
			unsigned char *data = priv->rx_queue->buf[priv->rx_tail];
			struct sk_buff *skb = netdev_alloc_skb(netdev, length + 2);

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
			emc_update_rx_error_stats(netdev, status);
		}

		/* Return descriptor */
		emc_reset_rx_desc(desc);
		dma_sync_single_for_device(&pdev->dev, dma, sizeof(*desc), DMA_TO_DEVICE);

		priv->rx_tail = (priv->rx_tail + 1) % RX_QUEUE_SIZE;
	}

	emc_hw_trigger_rx(priv);
}

static irqreturn_t emc_rx_threaded_isr(int irq, void *dev_id)
{
	emc_netdev_rx(dev_id);

	return IRQ_HANDLED;
}

static int emc_open(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;
	int err;

	priv->tx_head = 0x0;
	priv->tx_tail = 0x0;
	priv->rx_tail = 0x0;

	err = emc_init_desc(priv);
	if (err)
		return err;

	if (devm_request_threaded_irq(&pdev->dev, priv->txirq,
				      emc_tx_isr, emc_tx_threaded_isr, 0, pdev->name, netdev)) {
		dev_err(&pdev->dev, "register irq tx failed\n");
		return -EAGAIN;
	}

	if (devm_request_threaded_irq(&pdev->dev, priv->rxirq,
				      emc_rx_isr, emc_rx_threaded_isr, 0, pdev->name, netdev)) {
		dev_err(&pdev->dev, "register irq rx failed\n");
		free_irq(priv->txirq, netdev);
		return -EAGAIN;
	}

	clk_prepare_enable(priv->clk_main);
	clk_prepare_enable(priv->clk_rmii);

	emc_hw_init(netdev);
	emc_set_descriptors(priv);

	emc_hw_enable_rxtx(priv, true);
	netif_start_queue(netdev);
	emc_hw_trigger_rx(priv);

	if (priv->phy)
		phy_start(priv->phy);
	else if (priv->ncsi) {
		emc_hw_set_link_mode_for_ncsi(priv);
		netif_carrier_on(netdev);

		err = ncsi_start_dev(priv->ncsi);
		if (err)
			dev_warn(&pdev->dev, "NC/SI failed, %d\n", err);
	}

	return 0;
}

static void emc_set_rx_mode(struct net_device *netdev)
{
	struct emc_priv *priv;
	unsigned int rx_mode;

	priv = netdev_priv(netdev);

	if (netdev->flags & IFF_PROMISC) {
		/*
		 * Promiscuous mode:
		 *
		 * Receive all unicast, multicast, broadcast packets, and
		 * packets for our own unicast address.
		 */
		rx_mode = CAMCMR_AUP | CAMCMR_AMP | CAMCMR_ABP | CAMCMR_ECMP;
	} else if ((netdev->flags & IFF_ALLMULTI) || !netdev_mc_empty(netdev)) {
		/*
		 * "Receive all multicast packets" mode:
		 *
		 * Receive all multicast, broadcast packets, and packets for
		 * our own unicast address.
		 */
		rx_mode = CAMCMR_AMP | CAMCMR_ABP | CAMCMR_ECMP;
	} else {
		/*
		 * Default mode:
		 *
		 * Receive all broadcast packets, and packets for our own
		 * unicast address.
		 */
		rx_mode = CAMCMR_ECMP | CAMCMR_ABP;
	}
	writel(rx_mode, priv->reg + REG_CAMCMR);
}

static void emc_get_drvinfo(struct net_device *netdev,
					struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, DRV_MODULE_NAME, sizeof(info->driver));
	strlcpy(info->version, DRV_MODULE_VERSION, sizeof(info->version));
}

static const struct ethtool_ops emc_ethtool_ops = {
	.get_drvinfo = emc_get_drvinfo,
	.get_link = ethtool_op_get_link,
	.get_ts_info = ethtool_op_get_ts_info,
	.get_link_ksettings = phy_ethtool_get_link_ksettings,
	.set_link_ksettings = phy_ethtool_set_link_ksettings,
};

static const struct net_device_ops emc_netdev_ops = {
	.ndo_open		= emc_open,
	.ndo_stop		= emc_stop,
	.ndo_start_xmit		= emc_start_xmit,
	.ndo_set_rx_mode	= emc_set_rx_mode,
	.ndo_set_mac_address	= emc_set_mac_address,
};

static void emc_init_mac_address(struct net_device *netdev)
{
	struct emc_priv *priv = netdev_priv(netdev);
	struct platform_device *pdev = priv->pdev;

	if (device_get_ethdev_address(&pdev->dev, netdev))
		eth_hw_addr_random(netdev);

	emc_hw_write_cam(priv, 0, netdev->dev_addr);
}

static int emc_mdio_write(struct mii_bus *mdio, int phy_id, int reg, u16 data)
{
	struct emc_priv *priv = mdio->priv;
	unsigned int val, i;

	clk_prepare_enable(priv->clk_main);
	mutex_lock(&priv->mcmdr_mutex);
	emc_hw_enable_mdio(priv, true);

	writel(data, priv->reg + REG_MIID);

	val = (phy_id << 0x08) | reg;
	val |= MIIDA_BUSY | MIIDA_PHYWR | MIIDA_MDCCR_VAL;
	writel(val, priv->reg + REG_MIIDA);

	for (i = 0; i < MDIO_RETRIES; i++) {
		if ((readl(priv->reg + REG_MIIDA) & MIIDA_BUSY) == 0)
			break;
	}

	if (i == MDIO_RETRIES)
		dev_warn(&mdio->dev, "MDIO write timed out\n");

	emc_hw_enable_mdio(priv, false);
	mutex_unlock(&priv->mcmdr_mutex);
	clk_disable_unprepare(priv->clk_main);

	return 0;
}

static int emc_mdio_read(struct mii_bus *mdio, int phy_id, int reg)
{
	struct emc_priv *priv = mdio->priv;
	unsigned int val, i, data;

	clk_prepare_enable(priv->clk_main);
	mutex_lock(&priv->mcmdr_mutex);
	emc_hw_enable_mdio(priv, true);

	val = (phy_id << 0x08) | reg;
	val |= MIIDA_BUSY | MIIDA_MDCCR_VAL;
	writel(val, priv->reg + REG_MIIDA);

	for (i = 0; i < MDIO_RETRIES; i++) {
		if ((readl(priv->reg + REG_MIIDA) & MIIDA_BUSY) == 0)
			break;
	}

	if (i == MDIO_RETRIES) {
		dev_warn(&mdio->dev, "MDIO read timed out\n");
		data = 0xffff;
	} else {
		data = readl(priv->reg + REG_MIID);
	}

	emc_hw_enable_mdio(priv, false);
	mutex_unlock(&priv->mcmdr_mutex);
	clk_disable_unprepare(priv->clk_main);

	return data;
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

	res = devm_of_mdiobus_register(&pdev->dev, mdio, np);
	if (res) {
		dev_info(&pdev->dev, "failed to register MDIO bus: %d\n", res);
		return res;
	}

	return 0;
}

static void emc_ncsi_handler(struct ncsi_dev *ncsi)
{
	if (unlikely(ncsi->state != ncsi_dev_state_functional))
		return;

	netdev_dbg(ncsi->dev, "NCSI interface %s\n", ncsi->link_up ? "up" : "down");
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
	platform_set_drvdata(pdev, netdev);

	priv = netdev_priv(netdev);
	priv->pdev = pdev;
	priv->netdev = netdev;
	mutex_init(&priv->mcmdr_mutex);

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

	// TODO: NULL? or rather a name?
	priv->clk_main = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk_main))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->clk_main), "failed to get ether clock\n");

	priv->clk_rmii = devm_clk_get(&pdev->dev, "RMII");
	if (IS_ERR(priv->clk_rmii))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->clk_rmii), "failed to get RMII clock\n");

	priv->reset = devm_reset_control_get_optional(&pdev->dev, NULL);
	if (IS_ERR(priv->reset))
		return dev_err_probe(&pdev->dev, PTR_ERR(priv->reset), "failed to get reset control\n");

	if (priv->reset) {
		reset_control_assert(priv->reset);
		reset_control_deassert(priv->reset);
	}

	netdev->netdev_ops = &emc_netdev_ops;
	netdev->ethtool_ops = &emc_ethtool_ops;
	netdev->tx_queue_len = TX_QUEUE_SIZE;
	netdev->watchdog_timeo = TX_TIMEOUT;
	emc_init_mac_address(netdev);

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
