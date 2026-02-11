/*****************************************************************************
* Copyright 2006 - 2011 Broadcom Corporation.  All rights reserved.
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
/**
*
*  @file    island_net.c
*
*  @brief   Island-based Ethernet driver
*
*****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/mdio.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/dma-mapping.h>
#include <linux/proc_fs.h>
#include <linux/kfifo.h>
#include <linux/sysctl.h>
#include <linux/reboot.h>
#include <linux/ctype.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/regulator/consumer.h>

#include <linux/broadcom/knllog.h>
#include <linux/roku.h>

#include <asm/dma.h>
#include <asm/io.h>

#include <mach/clock.h>
#include <mach/io_map.h>
#include <mach/net_platform.h>
#include <mach/irqs.h>
#include <mach/ethHw_dma.h>
#include <mach/ethHw.h>
#include <mach/ethHw_mii.h>
#include <mach/esub_clkmgr_defs.h>
#include <mach/rdb/brcm_rdb_esw_busif_imp.h>
#include <mach/ethHw_reg.h>
#include <chal/chal_otp.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* Include code to re-sync */
#define ISLAND_NET_RECOVER_SYNC 1

/* Include code to dump diagnostic info on re-sync */
//#define ISLAND_NET_DUMP_SYNC 1

/* Byte alignment for RX packets from the switch to align IP header */
#define ETH_OFFSET_ALIGN	0

/* Number of rx dma descriptors */
#define DMA_RX_DESC_NUM		200

#define QUOTA 64

/* 802.3as defines max packet size to be 2000 bytes, size is rounded up
 * to be multiple of 32 to be cache aligned */
#define DMA_RX_BUF_LEN		2016

/* RX buffer alignment in bytes required for DMA and cache line */
#define RXALIGN			32

/* Limit TX DMA chain len */
#define DMA_TX_MAX_CHAIN_LEN	128

/* Must be power of two because of the use of kfifo */
#define DMA_TX_MAX_QUEUE_LEN	(DMA_TX_MAX_CHAIN_LEN * 4)

/* Two descriptors per packet, one each for: config data and payload */
#define DMA_TX_DESC_NUM		(DMA_TX_MAX_QUEUE_LEN*2)

/* All outgoing frames will be marked with the following traffic class */
#define TRAFFIC_CLASS          	ETHHW_TC_HIGHEST

#define MOD_NAME		"island-net"
#define MOD_DESCRIPTION		"Broadcom " MOD_NAME " driver"
#define MOD_VERSION		"0.4.0"

#define RX_DESC(bp, i)		(ETHHW_DMA_DESC *)((char *)(bp) + ((i) * sizeof(ETHHW_DMA_DESC)))
#define TX_DESC			RX_DESC

#define DMA_ESW_MODULE		0
#define DMA_ESW_CH_RX		2	/* PTM direction */
#define DMA_ESW_CH_TX		3	/* MTP direction */

#define DMA_RX_IRQ		(BCM_INT_ID_ESUB_DMAC_0+DMA_ESW_CH_RX)
#define DMA_TX_IRQ		(BCM_INT_ID_ESUB_DMAC_0+DMA_ESW_CH_TX)

#define DMA_BURST_SIZE		8
#define DMA_BLOCK_SIZE		256

#define PORT_PC			ETHHW_PORT_1
#define PORT_LAN		ETHHW_PORT_0
#define PHY_DEV_NUM		ETHHW_PORT_NUM

#define isDaMcast(bp)		(((char *)(bp))[0] == 0x01 )

#define TX_TIMEOUT		(5 * HZ)
#define MIN_FRAME_LEN		60

#define ENCAP_MODE_REMOVE	0  /* Remove Broadcom field */
#define ENCAP_MODE_KEEP_SKIP	1  /* Keep Broadcom field, but bypass in Rx driver */
#define ENCAP_MODE_KEEP_HANDLER	2  /* Keep Broadcom field, but bypass in packet handler */

#define NET_TX_TRACE		0
#if NET_TX_TRACE
#define NET_TX_LOG		KNLLOG
#else
#define NET_TX_LOG(fmt...)
#endif

#define NET_RX_TRACE		0
#if NET_RX_TRACE
#define NET_RX_LOG		KNLLOG
#else
#define NET_RX_LOG(fmt...)
#endif

/* The message level is actually a mask for this driver */
#define MSG_LEVEL_OFF            0x000000000
#define MSG_LEVEL_RX             0x000000001
#define MSG_LEVEL_RX_RAW         0x000000002
#define MSG_LEVEL_RX_VERBOSE     0x000000004
#define MSG_LEVEL_TX             0x000000100
#define MSG_LEVEL_TX_RAW         0x000000200
#define MSG_LEVEL_TX_VERBOSE     0x000000400
#define MSG_LEVEL_STATUS_VERBOSE 0x000001000

#define MSG_LEVEL_RX_RAW_VERBOSE MSG_LEVEL_RX_VERBOSE
#define MSG_LEVEL_TX_RAW_VERBOSE MSG_LEVEL_TX_VERBOSE

#define STR_OCTETS_FORMAT	"%02x %02x %02x %02x %02x %02x %02x %02x"
#define STR_OCTETS_ARGS(bp)	(bp)[0], (bp)[1], (bp)[2], (bp)[3], (bp)[4], (bp)[5], (bp)[6], (bp)[7]

#define FRAME_DUMP_SUPPORT	0
#if FRAME_DUMP_SUPPORT
#define FRAME_DUMP(msgLevel, strp, bufp, len) \
if (sysCtlMsgLevel & (msgLevel)) { \
	printk("%s: len=%i, bufp=0x%08x\n", (strp), (len), (int)(bufp)); \
	if (sysCtlMsgLevel & msgLevel ## _VERBOSE) { \
		printk(STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT " " \
		       STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT " " \
		       STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT "\n", \
		       STR_OCTETS_ARGS(&bufp[0]), STR_OCTETS_ARGS(&bufp[8]), \
		       STR_OCTETS_ARGS(&bufp[16]), STR_OCTETS_ARGS(&bufp[24]), \
		       STR_OCTETS_ARGS(&bufp[32]), STR_OCTETS_ARGS(&bufp[40])); \
	} \
}
#else
#define FRAME_DUMP(msgLevel, strp, bufp, len)
#endif

#define FRAME_DUMP_ALWAYS(strp, bufp, len) \
	printk("%s: len=%i, bufp=0x%08x\n", (strp), (len), (int)(bufp)); \
	printk(STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT " " \
	       STR_OCTETS_FORMAT " ... " STR_OCTETS_FORMAT "\n", \
	       STR_OCTETS_ARGS(&bufp[0]), STR_OCTETS_ARGS(&bufp[8]), \
	       STR_OCTETS_ARGS(&bufp[16]), STR_OCTETS_ARGS(&bufp[len - 8])); \

struct dma_desc
{
	void *descp;		/* Descriptor pointer */
	dma_addr_t addr;	/* Descriptor bus addr */
	int ring_len;		/* Total number of descriptors */
	int alloc_size;		/* Total memory alloc in bytes */
	int index;		/* Current descriptor index */
};

typedef u64 mib_t;

struct bcm_enet_mib
{
	/* Warning: do not change the names of the following members
	 * as they are used in macro expansion to formulate
	 * corresponding data routines.
	 */
	mib_t TxOctets;
	mib_t TxDropPkts;
	mib_t TxBroadcastPkts;
	mib_t TxMulticastPkts;
	mib_t TxUnicastPkts;
	mib_t TxCollisions;
	mib_t TxSingleCollision;
	mib_t TxMultipleCollision;
	mib_t TxDeferredTransmit;
	mib_t TxLateCollision;
	mib_t TxExcessiveCollision;
	mib_t TxFrameInDisc;
	mib_t TxPausePkts;
	mib_t RxOctets;
	mib_t RxUndersizePkts;
	mib_t RxPausePkts;
	mib_t Pkts64Octets;
	mib_t Pkts65To127Octets;
	mib_t Pkts128To255Octets;
	mib_t Pkts256To511Octets;
	mib_t Pkts512To1023Octets;
	mib_t Pkts1024ToMaxPktOctets;
	mib_t RxOversizePkts;
	mib_t RxJabbers;
	mib_t RxAlignmentErrors;
	mib_t RxFcsErrors;
	mib_t RxGoodOctets;
	mib_t RxDropPkts;
	mib_t RxUnicastPkts;
	mib_t RxMulticastPkts;
	mib_t RxBroadcastPkts;
	mib_t RxSaChanges;
	mib_t RxFragments;
	mib_t JumboPktCount;
	mib_t RxSymbolError;
	mib_t RxDiscard;
	mib_t DosAssertCount;
	mib_t DosDropCount;
	mib_t IngressRateAssertCount;
	mib_t IngressRateDropCount;
	mib_t EgressRateAssertCount;
};

#define GETMIB( p, port, name ) do { (p)->name = ethHw_mib ## name(port); } while (0)

typedef struct {
	int phyPort;
	int port_lan;
	int port_pc;

} ETH_PRIV;

struct skb_list_node
{
	struct sk_buff *skb;
	int len;
};

typedef struct dma_priv {
	struct dma_desc rx;
	struct dma_desc tx;

	struct kfifo txfifo;

	unsigned int tx_cfg_alloc_size;	/* in bytes */
	dma_addr_t tx_cfg_dma; 		/* dma address for cfg headers */
	uint64_t *tx_cfg; 		/* pointer to cfg headers */

	int tx_max_pkts; 		/* max number of packets */
	int tx_curr;  			/* current packet index */

	struct skb_list_node *tx_skb_list; /* list of skb given to hw for tx */
	struct skb_list_node *rx_skb_list; /* list of skb given to hw for rx */

} DMA_PRIV;

typedef struct bcm_enet_priv
{
	/* Rx/Tx processing stuff */
	struct napi_struct napi ____cacheline_aligned;
	struct tasklet_struct txtasklet;

	/* Other states and parameters */
	uint32_t msg_enable; /* message filter bit mask */
	struct net_device *ndev; /* net device reference */
	struct platform_device *pdev; /* platform device reference */
	struct island_net_hw_cfg hw_cfg;
	ETH_PRIV eth;
	DMA_PRIV dma;
	int wasRunning;

	/* PHY stuff */
	spinlock_t lock;
	struct mii_bus *mii_bus;
	int mdio_irq[PHY_MAX_ADDR];
	struct phy_device *phydev;
	unsigned int link;
	unsigned int speed;
	unsigned int duplex;
	int num_phys;

} BCMNET_PRIV;

struct bcm_enet_esw_regs
{
	u32 port_ctrl0;
	u32 port_ctrl1;
	u32 imp_port_ctrl;
	u32 switch_mode;
	u32 protected_port_sel;
	u32 link_status;
	u32 link_status_change;
	u32 port_speed;
	u32 mgmt_config;
	u32 arl_rw_ctrl;
	u32 ptm_config;
	u32 ptm_dma_status;
	u32 ptm_fifo_config;
	u32 mtp_config;
	u32 mtp_dma_status;
	u32 mtp_fifo_config;
	u32 imp_phy_misc;
	u32 imp_speed;
};
#define BCM_ENET_REGBLK_SIZE	sizeof(struct bcm_enet_esw_regs)

/* ---- Private Variables ------------------------------------------------ */

static const char banner[] = MOD_DESCRIPTION " " MOD_VERSION "\n";

static unsigned int brcmMode = ENCAP_MODE_REMOVE;
module_param(brcmMode, uint, 0644);
MODULE_PARM_DESC(brcmMode, "Rx Broadcom encapsulation mode");
#define BRCM_ENCAP_MODE    brcmMode

static unsigned char cmdline_mac[6];
static unsigned int cmdline_mac_set;

static const struct {
	const char string[ETH_GSTRING_LEN];
} ethtool_stats_keys[] = {
	{ "TxOctets" },
	{ "TxDropPkts" },
	{ "TxBroadcastPkts" },
	{ "TxMulticastPkts" },
	{ "TxUnicastPkts" },
	{ "TxCollisions" },
	{ "TxSingleCollision" },
	{ "TxMultipleCollision" },
	{ "TxDeferredTransmit" },
	{ "TxLateCollision" },
	{ "TxExcessiveCollision" },
	{ "TxFrameInDisc" },
	{ "TxPausePkts" },
	{ "RxOctets" },
	{ "RxUndersizePkts" },
	{ "RxPausePkts" },
	{ "Pkts64Octets" },
	{ "Pkts65To127Octets" },
	{ "Pkts128To255Octets" },
	{ "Pkts256To511Octets" },
	{ "Pkts512To1023Octets" },
	{ "Pkts1024ToMaxPktOctets" },
	{ "RxOversizePkts" },
	{ "RxJabbers" },
	{ "RxAlignmentErrors" },
	{ "RxFcsErrors" },
	{ "RxGoodOctets" },
	{ "RxDropPkts" },
	{ "RxUnicastPkts" },
	{ "RxMulticastPkts" },
	{ "RxBroadcastPkts" },
	{ "RxSaChanges" },
	{ "RxFragments" },
	{ "JumboPktCount" },
	{ "RxSymbolError" },
	{ "RxDiscard" },
	{ "DosAssertCount" },
	{ "DosDropCount" },
	{ "IngressRateAssertCount" },
	{ "IngressRateDropCount" },
	{ "EgressRateAssertCount" },
};

static int zero = 0;
static int one = 1;

static unsigned int sysCtlMsgLevel;
static unsigned int sysCtlPmDisablePortPc;
static unsigned int sysCtlPmDisablePortLan;
static unsigned int sysCtlPmDisablePortInternal;
static unsigned long sysCtlRxBytes;
static unsigned long sysCtlRxDroppedPkts;
static unsigned long sysCtlRxReSyncs;
static unsigned long sysCtlRxWraparounds;
static unsigned long sysCtlRxSyncChecked;
static unsigned long sysCtlRxSyncDroppedPkts;
static unsigned long sysCtlRxNoSkb;

#define CTL_TABLE_ENTRY_BOOL(_procname, _data, _mode) \
   .procname = (_procname), \
   .data = &(_data), \
   .maxlen = sizeof((_data)), \
   .mode = (_mode), \
   .proc_handler = &proc_dointvec_minmax, \
   .extra1 = &zero, \
   .extra2 = &one
/* Note that this makes every entry unsigned */
#define CTL_TABLE_ENTRY_INT(_procname, _data, _mode) \
   .procname = (_procname), \
   .data = &(_data), \
   .maxlen = sizeof((_data)), \
   .mode = (_mode), \
   .proc_handler = &proc_dointvec_minmax, \
   .extra1 = &zero
/* Note that this makes every entry unsigned */
#define CTL_TABLE_ENTRY_LONG(_procname, _data, _mode) \
   .procname = (_procname), \
   .data = &(_data), \
   .maxlen = sizeof((_data)), \
   .mode = (_mode), \
   .proc_handler = &proc_doulongvec_minmax, \
   .extra1 = &zero

static struct ctl_table sysCtlEth[] =
{
	{CTL_TABLE_ENTRY_INT("msgLevel", sysCtlMsgLevel, 0644)},
	{CTL_TABLE_ENTRY_BOOL("pmDisablePortPc", sysCtlPmDisablePortPc, 0644)},
	{CTL_TABLE_ENTRY_BOOL("pmDisablePortLan", sysCtlPmDisablePortLan, 0644)},
	{CTL_TABLE_ENTRY_BOOL("pmDisablePortInternal", sysCtlPmDisablePortInternal, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxBytes", sysCtlRxBytes, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxDroppedPkts", sysCtlRxDroppedPkts, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxReSyncs", sysCtlRxReSyncs, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxWraparounds", sysCtlRxWraparounds, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxSyncChecked", sysCtlRxSyncChecked, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxSyncDroppedPkts", sysCtlRxSyncDroppedPkts, 0644)},
	{CTL_TABLE_ENTRY_LONG("rxNoSkb", sysCtlRxNoSkb, 0644)},
	{}
};

static struct ctl_table sysCtl[] = {
	{
	   .procname = "eth",
	   .mode = 0555,
	   .child = sysCtlEth
	},
	{}
};

static struct ctl_table_header *sysCtlHeaderp;

#ifdef CONFIG_ARCH_CAPRI
static struct clk *esub_clk;
static struct clk *esw_sys_clk;
static struct clk *esub_axi_clk;
#endif

/* required ETH PHY pull-up voltage is 3.3 V */
#define PHY_MICROVOLT 3300000

static struct regulator *phy_regulator;

/* ---- Private Function Prototypes -------------------------------------- */

static void *alloc_rx_skb(int len, struct skb_list_node *node);
static int dmaStart( BCMNET_PRIV *privp );
static int dmaStop( BCMNET_PRIV *privp );
static int ethStart(BCMNET_PRIV *privp);
static int ethStop(BCMNET_PRIV *privp);
static void txClean( BCMNET_PRIV *privp );
static void tx(BCMNET_PRIV *privp);
static void txWork(unsigned long data);

static irqreturn_t ethIsrRx(int irq, void *userData);
static irqreturn_t ethIsrTx(int irq, void *userData);

static int brcmPacketTypeHandler(struct sk_buff *skb, struct net_device *dev,
		struct packet_type *ptype,
		struct net_device *orig_dev);

static int bcm_enet_open(struct net_device *dev);
static int bcm_enet_close(struct net_device *dev);
static int bcm_enet_do_ioctl(struct net_device *dev, struct ifreq *ifr,
		int cmd);
static int bcm_enet_hard_start_xmit(struct sk_buff *skb,
		struct net_device *dev);
static int bcm_enet_poll(struct napi_struct *napi, int quota);
static void bcm_enet_set_multicast_list(struct net_device *dev);
static int bcm_enet_set_mac_address(struct net_device *dev, void *addr);
static struct net_device_stats *bcm_enet_get_stats(struct net_device *dev);
static void bcm_enet_tx_timeout(struct net_device *dev);
#ifdef CONFIG_NET_POLL_CONTROLLER
static void bcm_enet_poll_controller(struct net_device *dev);
#endif

static u32 bcm_enet_get_msglevel( struct net_device *dev );
static void bcm_enet_set_msglevel( struct net_device *dev, u32 value );
static int bcm_enet_get_settings( struct net_device *dev, struct ethtool_cmd *cmd );
static int bcm_enet_set_settings( struct net_device *dev, struct ethtool_cmd *cmd );
static void bcm_enet_get_drvinfo( struct net_device *dev, struct ethtool_drvinfo *info );
static int bcm_enet_get_regs_len( struct net_device *dev );
static void bcm_enet_get_regs( struct net_device *dev,
		struct ethtool_regs *regs, void *buf );
static int bcm_enet_nway_reset( struct net_device *dev );
static void bcm_enet_get_strings( struct net_device *dev, u32 stringset,
		u8 *buf );
static void bcm_enet_get_ringparam( struct net_device *dev,
		struct ethtool_ringparam *ering );
static void bcm_enet_get_ethtool_stats( struct net_device *dev,
		struct ethtool_stats *estats, u64 *tmp_stats );
static int bcm_enet_get_sset_count( struct net_device *dev, int sset );

static int bcm_enet_init(void);
static void bcm_enet_exit(void);
static int bcm_enet_notify_reboot(struct notifier_block *nb,
		unsigned long event, void *ptr);
static int bcm_enet_suspend(struct platform_device *pdev,
		pm_message_t state);
static int bcm_enet_resume(struct platform_device *pdev);

static struct notifier_block bcm_enet_notifier_reboot =
{
	.notifier_call = bcm_enet_notify_reboot,
	.next          = NULL,
	.priority      = 0
};

static const struct net_device_ops bcm_enet_ops =
{
	.ndo_open               = bcm_enet_open,
	.ndo_stop               = bcm_enet_close,
	.ndo_get_stats          = bcm_enet_get_stats,
	.ndo_start_xmit         = bcm_enet_hard_start_xmit,
	.ndo_set_multicast_list = bcm_enet_set_multicast_list,
	.ndo_tx_timeout         = bcm_enet_tx_timeout,
	.ndo_do_ioctl           = bcm_enet_do_ioctl,
	.ndo_set_mac_address    = bcm_enet_set_mac_address,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= eth_change_mtu,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= bcm_enet_poll_controller,
#endif
};

static const struct ethtool_ops bcm_enet_ethtool_ops =
{
	.get_settings		= bcm_enet_get_settings,
	.set_settings		= bcm_enet_set_settings,
	.get_drvinfo		= bcm_enet_get_drvinfo,
	.get_regs_len		= bcm_enet_get_regs_len,
	.get_regs		= bcm_enet_get_regs,
	.get_msglevel		= bcm_enet_get_msglevel,
	.set_msglevel		= bcm_enet_set_msglevel,
	.nway_reset		= bcm_enet_nway_reset,
	.get_link		= ethtool_op_get_link,
	.get_strings		= bcm_enet_get_strings,
	.get_ringparam		= bcm_enet_get_ringparam,
	.get_ethtool_stats	= bcm_enet_get_ethtool_stats,
	.get_sset_count		= bcm_enet_get_sset_count,
};

static struct packet_type brcmPacketType =
{
	.type = cpu_to_be16( ETHHW_BRCM_TYPE ),
	.func = brcmPacketTypeHandler,
};

/* ---- Functions -------------------------------------------------------- */

static inline int ethMiiGet( int port, int addr )
{
	uint32_t val;
	ethHw_miiGet( port, ETHHW_MII_FLAGS_EXT, (uint32_t)addr, &val );
	return val;
}

static inline void ethMiiSet(int port, int addr, int data)
{
	ethHw_miiSet( port, ETHHW_MII_FLAGS_EXT, (uint32_t)addr,
			(uint32_t)data );
}

/* Functions to work with ring buffer indices */
/* All four assume the ring buffer size is fixed at DMA_RX_DESC_NUM */
static inline int prev( int idx )
{
	if ( unlikely( idx == 0 ))
	{
		return DMA_RX_DESC_NUM;
	}
	return idx - 1;
}

static inline int next( int idx )
{
	if ( unlikely( idx == DMA_RX_DESC_NUM ))
	{
		return 0;
	}
	return idx + 1;
}

static inline void inc( int *idx )
{
	(*idx)++;
	if ( unlikely( *idx >= DMA_RX_DESC_NUM ))
	{
		*idx = 0;
	}
}

static inline int sub( int left, int right )
{
	const int raw = left - right;

	if ( raw < 0 )
	{
		return DMA_RX_DESC_NUM + raw;
	}
	return raw;
}

#ifndef CONFIG_ARCH_CAPRI
#define WR_ACCESS_PASSWORD	0x00A5A500
#define WR_ACCESS_ENABLE	0x00000001
static void chal_ccu_esub_pll_init(void)
{
	void __iomem *esub_clk;
	u32 regval;

	esub_clk = ioremap(ESUB_CLK_BASE_ADDR, SZ_4K);
	if ( !esub_clk )
	{
		printk( "%s: Error mapping ESUB_CLK address\n", __func__ );
		return;
	}

	/* enable access */
	regval = readl( esub_clk + ESUB_CLK_MGR_REG_WR_ACCESS_OFFSET ) &
		ESUB_CLK_MGR_REG_WR_ACCESS_PASSWORD_MASK;
	regval |= WR_ACCESS_PASSWORD | WR_ACCESS_ENABLE;
	writel( regval, esub_clk + ESUB_CLK_MGR_REG_WR_ACCESS_OFFSET );

	/* Post divider reset toggle */
	regval = readl( esub_clk + ESUB_CLK_MGR_REG_PLL_POST_RESETB_OFFSET ) &
		~ESUB_CLK_MGR_REG_PLL_POST_RESETB_I_POST_RESETB_MASK;
	writel( regval, esub_clk + ESUB_CLK_MGR_REG_PLL_POST_RESETB_OFFSET );

	/* take PLL out of reset and put into normal mode */
	regval = readl( esub_clk + ESUB_CLK_MGR_REG_PLL_RESETB_OFFSET ) |
		ESUB_CLK_MGR_REG_PLL_RESETB_I_PLL_RESETB_MASK;
	writel( regval, esub_clk + ESUB_CLK_MGR_REG_PLL_RESETB_OFFSET );

	/* wait for PLL lock */
	while ( (readl( esub_clk + ESUB_CLK_MGR_REG_PLL_LOCK_OFFSET ) &
		ESUB_CLK_MGR_REG_PLL_LOCK_PLL_LOCK_MASK) == 0 );

	regval = readl( esub_clk + ESUB_CLK_MGR_REG_PLL_POST_RESETB_OFFSET ) |
		ESUB_CLK_MGR_REG_PLL_POST_RESETB_I_POST_RESETB_MASK;
	writel( regval, esub_clk + ESUB_CLK_MGR_REG_PLL_POST_RESETB_OFFSET );

#ifdef CONFIG_ARCH_CAPRI
	/* Switch esw_sys_clk to use 104MHz clock */
	writel( (readl(esub_clk + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET) &
				~(ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_PLL_SELECT_MASK | ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_DIV_MASK)) |
			(ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_PLL_SELECT_CMD_VAR_208M_CLK << ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_PLL_SELECT_SHIFT) |
			(1 << ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_DIV_SHIFT),
			esub_clk + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET);

	writel( readl(esub_clk + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET) |
			ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_TRIGGER_MASK,
			esub_clk + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET);

	/* Wait for trigger complete */
	while( (readl(esub_clk + ESUB_CLK_MGR_REG_ESW_SYS_DIV_OFFSET) & ESUB_CLK_MGR_REG_ESW_SYS_DIV_ESW_SYS_TRIGGER_MASK) ) {}
#endif

	/* Lock clock manager registers */
	regval = readl( esub_clk + ESUB_CLK_MGR_REG_WR_ACCESS_OFFSET ) &
		ESUB_CLK_MGR_REG_WR_ACCESS_PASSWORD_MASK;
	regval |= WR_ACCESS_PASSWORD;
	writel( regval, esub_clk + ESUB_CLK_MGR_REG_WR_ACCESS_OFFSET );

	iounmap(esub_clk);
}
#endif

static void *alloc_rx_skb(int len, struct skb_list_node *node )
{
	struct sk_buff *skb;
	int offset;

	skb = dev_alloc_skb( len + 2*RXALIGN + ETH_OFFSET_ALIGN );
	if ( unlikely( skb == NULL ))
	{
		return NULL;
	}

	/* Align buffer for DMA requirements */
	offset = (((unsigned long)skb->data + RXALIGN-1 ) & ~( RXALIGN-1 ))
		- (unsigned long)skb->data;
	skb_reserve( skb, offset );

	node->skb = skb;
	node->len = len;

	return skb->data;
}

#if defined(ISLAND_NET_DUMP_SYNC)
static void dump_ring(const char * str, struct dma_desc *dmap )
{
	ETHHW_DMA_DESC *descp, *handlep;
	int i;

	printk("%s DMA descriptor %p:\n", str, dmap);
	printk("\tdescp=%p, addr=%#x, ring_len=%d, alloc_size=%d, index=%d\n", dmap->descp, dmap->addr, dmap->ring_len, dmap->alloc_size, dmap->index);
	for (i=0; i<dmap->ring_len; i++) {
		descp = RX_DESC(dmap->descp, i);
		handlep = RX_DESC(dmap->addr, i);
		printk("\t[%d] descp=%p, handlep=%p\n", i, descp, handlep);
		printk("\t[%d] sar=%#x, dar=%#x, lli=%#x, ctlLo=%#x, ctlHi=%#x, stat1=%#x, stat2=%#x\n", i, descp->sar, descp->dar, descp->lli, descp->ctlLo, descp->ctlHi, descp->stat1, descp->stat2);
	}
}
#endif

static int dmaStart( BCMNET_PRIV *privp )
{
	DMA_PRIV *dmap;
	struct net_device *ndev = privp->ndev;
	int rc = 0;
	unsigned int size;
	int i;
	char *bufp;
	ETHHW_DMA_DESC *descp, *handlep;
	dma_addr_t dmaBuf;

	dmap = &privp->dma;

	ethHw_dmaInit( DMA_ESW_MODULE );

	/* Initialize DMA parameters. There is potential to parameterize some
	 * of these values in the future. But for now, just hard-code with
	 * the global definitions
	 */
	dmap->rx.ring_len = DMA_RX_DESC_NUM;
	dmap->tx.ring_len = DMA_TX_DESC_NUM;
	dmap->tx_max_pkts = DMA_TX_MAX_QUEUE_LEN;

	/* Configure rx dma */
	ethHw_dmaDisable( DMA_ESW_MODULE, DMA_ESW_CH_RX );
	ethHw_dmaRxConfig( DMA_BURST_SIZE, DMA_BLOCK_SIZE, ETH_OFFSET_ALIGN );
	ethHw_dmaConfig( DMA_ESW_MODULE, DMA_ESW_CH_RX,
			ETHHW_DMA_PTM_CTL_HI, ETHHW_DMA_PTM_CTL_LO,
			ETHHW_DMA_PTM_CFG_HI, ETHHW_DMA_PTM_CFG_LO,
			ETHHW_DMA_PTM_SSTAT, 0 );

	/* Allocate rx descriptors */
	dmap->rx.index = 0;
	dmap->rx.alloc_size = DMA_RX_DESC_NUM * sizeof(ETHHW_DMA_DESC);
	dmap->rx.descp = dma_alloc_coherent( NULL, dmap->rx.alloc_size,
			&dmap->rx.addr, GFP_KERNEL );
	if ( dmap->rx.descp == NULL )
	{
		netdev_err( ndev, "Cannot allocate rx dma descriptors\n" );
		return -ENOMEM;
	}

	/* Allocate rx skb list */
	size = dmap->rx.ring_len * sizeof(*dmap->rx_skb_list);
	dmap->rx_skb_list = kzalloc( size, GFP_KERNEL);
	if ( dmap->rx_skb_list == NULL )
	{
		netdev_err( ndev, "cannot allocate rx skb list size=%u\n", size );
		rc = -ENOMEM;
		goto out_err;
	}

	/* Setup rx descriptor ring */
	descp = NULL;
	for ( i = 0; i < DMA_RX_DESC_NUM; i++ )
	{
		descp = RX_DESC(dmap->rx.descp, i);
		handlep = RX_DESC(dmap->rx.addr, i);
		bufp = alloc_rx_skb( DMA_RX_BUF_LEN, &dmap->rx_skb_list[i] );
		if ( bufp == NULL )
		{
			netdev_err( ndev, "Failed to alloc rx dmabuf i=%i\n", i );
			rc = -ENOMEM;
			goto out_err;
		}

		/* Transfer buffer ownership to device */
		dmaBuf = virt_to_dma( NULL, bufp );
		dma_sync_single_for_device( NULL, dmaBuf, DMA_RX_BUF_LEN,
				DMA_FROM_DEVICE );

		ETHHW_DMA_DESC_CREATE( descp, ETHHW_DMA_PTM_FIFO_ADDR, dmaBuf,
				handlep,
				/* Enable interrupts per descriptor */
				ETHHW_DMA_PTM_CTL_LO | ETHHW_DMA_CTL_INT_EN,
				DMA_RX_BUF_LEN);
	}
	/* Wrap last descriptor for circular processing */
	ETHHW_DMA_DESC_WRAP(descp, (uint32_t)RX_DESC(dmap->rx.addr, 0));

	rc = request_irq( DMA_RX_IRQ, ethIsrRx, IRQF_DISABLED, "esw_rx", privp );
	if ( rc < 0 )
	{
		netdev_err( ndev, "request_irq(esw_rx) failed, err=%i\n", rc );
		goto out_err;
	}

	ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_RX );
	ethHw_dmaIrqEnable( DMA_ESW_MODULE, DMA_ESW_CH_RX );

	/* Configure tx dma */
	ethHw_dmaDisable( DMA_ESW_MODULE, DMA_ESW_CH_TX );
	ethHw_dmaTxConfig( DMA_BURST_SIZE );
	ethHw_dmaConfig( DMA_ESW_MODULE, DMA_ESW_CH_TX, ETHHW_DMA_MTP_CTL_HI,
			ETHHW_DMA_MTP_CTL_LO, ETHHW_DMA_MTP_CFG_HI,
			ETHHW_DMA_MTP_CFG_LO, 0, ETHHW_DMA_MTP_DSTAT );

	/* Allocate tx config buffers */
	size = dmap->tx_max_pkts * sizeof(*dmap->tx_cfg);
	dmap->tx_cfg = dma_alloc_coherent( NULL, size, &dmap->tx_cfg_dma,
			GFP_KERNEL );
	if ( dmap->tx_cfg == NULL )
	{
		netdev_err( ndev, "cannot alloc tx config bufs, size=%u\n", size );
		rc = -ENOMEM;
		goto out_err;
	}
	memset( dmap->tx_cfg, 0, dmap->tx_cfg_alloc_size );
	dmap->tx_cfg_alloc_size = size;
	dmap->tx_curr = 0;

	/* Allocate tx skb list */
	size = dmap->tx_max_pkts * sizeof(*dmap->tx_skb_list);
	dmap->tx_skb_list = kzalloc( size, GFP_KERNEL );
	if ( dmap->tx_skb_list == NULL )
	{
		netdev_err( ndev, "cannot allocate tx skb list size=%u\n", size );
		rc = -ENOMEM;
		goto out_err;
	}

	dmap->tx.index = 0;
	dmap->tx.alloc_size = dmap->tx.ring_len * sizeof(ETHHW_DMA_DESC);
	dmap->tx.descp = dma_alloc_coherent( NULL, dmap->tx.alloc_size,
			&dmap->tx.addr, GFP_KERNEL );
	if ( dmap->tx.descp == NULL )
	{
		netdev_err( ndev, "Cannot allocate tx dma descriptors.\n" );
		rc = -ENOMEM;
		goto out_err;
	}

	rc = request_irq( DMA_TX_IRQ, ethIsrTx, IRQF_DISABLED, "esw_tx", privp );
	if ( rc < 0 )
	{
		netdev_err( ndev, "request_irq(esw_tx) failed, err=%i\n", rc );
		goto out_err;
	}

	ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_TX );
	ethHw_dmaIrqDisable( DMA_ESW_MODULE, DMA_ESW_CH_TX );

	return 0;

out_err:
	dmaStop( privp );
	return rc;
}


static int dmaStop( BCMNET_PRIV *privp)
{
	DMA_PRIV *dmap;
	struct device *kdev;
	int i;

	dmap = &privp->dma;
	kdev = &privp->pdev->dev;

	/* Stop Rx DMA */

	/* Disable Rx DMA */
	ethHw_dmaDisable( DMA_ESW_MODULE, DMA_ESW_CH_RX );
	ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_RX );
	ethHw_dmaIrqDisable( DMA_ESW_MODULE, DMA_ESW_CH_RX );

	free_irq( DMA_RX_IRQ, privp );

	/* Free pending Rx buffers */
	for ( i = 0; i < DMA_RX_DESC_NUM; i++ )
	{
		if ( dmap->rx_skb_list[i].skb )
		{
			dev_kfree_skb_any( dmap->rx_skb_list[i].skb );
		}
	}

	if ( dmap->rx_skb_list )
	{
		kfree( dmap->rx_skb_list );
		dmap->rx_skb_list = NULL;
	}

	if ( dmap->rx.descp )
	{
		dma_free_coherent( NULL, dmap->rx.alloc_size, dmap->rx.descp,
				dmap->rx.addr);
		dmap->rx.descp = NULL;
	}


	/* Stop Tx DMA */

	ethHw_dmaDisable( DMA_ESW_MODULE, DMA_ESW_CH_TX );
	ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_TX );
	ethHw_dmaIrqDisable( DMA_ESW_MODULE, DMA_ESW_CH_TX );

	free_irq( DMA_TX_IRQ, privp );

	/* Free pending Tx buffers */
	txClean( privp );

	if ( dmap->tx.descp )
	{
		dma_free_coherent( NULL, dmap->tx.alloc_size, dmap->tx.descp,
				dmap->tx.addr);
		dmap->tx.descp = NULL;
	}

	if ( dmap->tx_skb_list )
	{
		kfree( dmap->tx_skb_list );
		dmap->tx_skb_list = NULL;
	}

	if ( dmap->tx_cfg )
	{
		dma_free_coherent( NULL, dmap->tx_cfg_alloc_size,
				dmap->tx_cfg, dmap->tx_cfg_dma );
		dmap->tx_cfg = NULL;
	}

	return 0;
}

static int ethStart( BCMNET_PRIV *privp )
{
	int rc;
	uint32_t reg32;
	ETH_PRIV *ethp  = &privp->eth;
	int portspeed;
	struct device *kdev = &privp->pdev->dev;

#ifdef CONFIG_ARCH_CAPRI
	esub_clk = clk_get( NULL, ESUB_CCU_CLK_NAME_STR );
	esw_sys_clk = clk_get( NULL, ESW_SYS_PERI_CLK_NAME_STR );
	esub_axi_clk = clk_get( NULL, ESUB_AXI_BUS_CLK_NAME_STR );

	if ( IS_ERR( esub_clk) || IS_ERR( esw_sys_clk ) || IS_ERR( esub_axi_clk ))
	{
		dev_err( kdev, "Failed getting esub clocks\n" );
		rc = -EIO;
		goto exit_free_clks;
	}

	rc = clk_set_rate( esw_sys_clk, 104000000 );
	if ( rc )
	{
		/* Warning only, fall through */
		dev_err( kdev, "Failed to set esw_sys clk rate\n" );
	}

	rc = clk_enable( esub_clk );
	if ( rc )
	{
		dev_err( kdev, "Failed enabling esub_clk\n" );
		goto exit_free_clks;
	}

	rc = clk_enable( esw_sys_clk );
	if ( rc )
	{
		/* Warning only, fall through */
		dev_err( kdev, "Failed enabling esw_sys_clk\n" );
	}

	rc = clk_enable( esub_axi_clk );
	if ( rc )
	{
		/* Warning only, fall through */
		dev_err( kdev, "Failed enabling esub_axi_clk\n" );
	}
#endif
	if (privp->hw_cfg.phy_regulator_name) {
		phy_regulator = regulator_get(NULL, privp->hw_cfg.phy_regulator_name);
		if (IS_ERR(phy_regulator)) {
			dev_err(kdev, "Unable to get regulator %s\n",
					privp->hw_cfg.phy_regulator_name);
			phy_regulator = NULL;
		} else {
			rc = regulator_set_voltage(phy_regulator, PHY_MICROVOLT, PHY_MICROVOLT);
			if (rc < 0) {
				dev_err(kdev, "Unable to set voltage for regulator %s\n",
					privp->hw_cfg.phy_regulator_name);
				regulator_put(phy_regulator);
				phy_regulator = NULL;
			} else {
				rc = regulator_enable(phy_regulator);
				if (rc < 0) {
					dev_err(kdev, "Unable to enable regulator %s\n",
						privp->hw_cfg.phy_regulator_name);
					regulator_put(phy_regulator);
					phy_regulator = NULL;
				} else {
					pr_info("Ethernet regulator %s enabled\n",
						privp->hw_cfg.phy_regulator_name);
				}
			}
		}
	}

	if ( privp->hw_cfg.gpioPhy0 > 0 )
	{
		/* Hold PHY in reset */
		rc = gpio_request( privp->hw_cfg.gpioPhy0, "ephy0_rst" );
		if ( rc )
		{
			dev_err( kdev, "%s: failed to request gpio %d for Phy0\n",
					__func__, privp->hw_cfg.gpioPhy0 );
			goto exit_disable_regulator;
		}
		gpio_direction_output( privp->hw_cfg.gpioPhy0, 0 );
	}

	if ( privp->hw_cfg.gpioPhy1 > 0 )
	{
		/* Hold PHY in reset */
		rc = gpio_request( privp->hw_cfg.gpioPhy1, "ephy1_rst" );
		if ( rc )
		{
			dev_err( kdev, "%s: failed to request gpio %d for Phy1\n",
					__func__, privp->hw_cfg.gpioPhy1 );
			goto exit_free_phy0_gpio;
		}
		gpio_direction_output( privp->hw_cfg.gpioPhy1, 0 );
	}

	if ( privp->hw_cfg.gpioPhyPwrEn >= 0 )
	{
		/* Enable PHY power */
		rc = gpio_request(privp->hw_cfg.gpioPhyPwrEn, "phy_pwr_en");
		if ( rc )
		{
			dev_err( kdev, "%s: failed to request gpio %d to enable PHY power\n",
					__func__, privp->hw_cfg.gpioPhyPwrEn );
			goto exit_free_phy1_gpio;
		}
		gpio_direction_output( privp->hw_cfg.gpioPhyPwrEn, 0 );
		mdelay( 20 ); /* Power down for short duration */
		gpio_set_value_cansleep( privp->hw_cfg.gpioPhyPwrEn, 1 );
		mdelay( 100 ); /* Let power settle for some time */
	}

	if ( privp->hw_cfg.gpioPhy0 > 0 )
	{
		/* Take the PHY out of reset */
		gpio_set_value_cansleep( privp->hw_cfg.gpioPhy0, 1 );

		/* Set MDIO address */
		ETHHW_REG_SET( ETHHW_REG32( KONA_ESW_VA + 0x00380 ), privp->hw_cfg.addrPhy0 );

		privp->num_phys++;
	}

	if ( privp->hw_cfg.gpioPhy1 > 0 )
	{
		/* Take the PHY out of reset */
		gpio_set_value_cansleep( privp->hw_cfg.gpioPhy1, 1 );

		/* Set MDIO address */
		ETHHW_REG_SET( ETHHW_REG32( KONA_ESW_VA + 0x00388 ), privp->hw_cfg.addrPhy1 );

		privp->num_phys++;
	}

	if ( privp->num_phys == 0 )
	{
		dev_err( &privp->pdev->dev, "No PHYs configured/available\n" );
		rc = -EFAULT;
		goto exit_free_phypwr_gpio;
	}

#ifdef CONFIG_MACH_CAPRI_FPGA
	/* Limit internal port speed to 100Mbps */
	ethHw_impSpeedSet(100, 100); /* ( Rx, Tx ) */
#else
	/* Configure internal port speed to 1000Mbps */
	ethHw_impSpeedSet(1000, 1000); /* ( Rx, Tx ) */
#ifndef CONFIG_ARCH_CAPRI
	chal_ccu_esub_pll_init();
#endif
#endif

	rc = ethHw_Init();
	pr_info( "Ethernet initialization %s (rc=%i)\n", ETHHW_RC_SUCCESS( rc )
			? "successful" : "failed", rc );
	if ( !ETHHW_RC_SUCCESS( rc ))
	{
		rc = -EPERM;
		goto exit_free_phypwr_gpio;
	}

	/* Disable forwarding to internal port (this must be done before
	 *  forwarding is enabled on the external ports)
	 */
	ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);

	/* STP not used so put external ports in forwarding state */
	/* TODO:  If STP support is required, this state control will need to be
	 *         moved to the STP application
	 */
	ethHw_stpStateSet( ETHHW_PORT_0, ETHHW_STP_STATE_FORWARDING );
	ethHw_stpStateSet( ETHHW_PORT_1, ETHHW_STP_STATE_FORWARDING );

	/* Assign ports */
	ethp->port_lan = PORT_LAN;
	ethp->port_pc = PORT_PC;
	ethp->phyPort = ethp->port_lan;

	/* Disable forwarding to internal port */
	ethHw_impEnableSet( 0 );

	return 0;

exit_free_phypwr_gpio:
	if ( privp->hw_cfg.gpioPhyPwrEn >= 0 )
		gpio_free( privp->hw_cfg.gpioPhyPwrEn );
exit_free_phy1_gpio:
	if ( privp->hw_cfg.gpioPhy1 >= 0 ) gpio_free( privp->hw_cfg.gpioPhy1 );
exit_free_phy0_gpio:
	if ( privp->hw_cfg.gpioPhy0 >= 0 ) gpio_free( privp->hw_cfg.gpioPhy0 );
exit_disable_regulator:
	if (phy_regulator) {
		regulator_disable(phy_regulator);
		regulator_put(phy_regulator);
		phy_regulator = NULL;
	}
#ifdef CONFIG_ARCH_CAPRI
	clk_disable( esub_axi_clk );
	clk_disable( esw_sys_clk );
	clk_disable( esub_clk );
exit_free_clks:
	if ( !IS_ERR( esub_clk ))
		clk_put( esub_clk );
	if ( !IS_ERR( esw_sys_clk ))
		clk_put( esw_sys_clk );
	if ( !IS_ERR( esub_axi_clk ))
		clk_put( esub_axi_clk );
#endif
	return rc;
}

static int ethStop( BCMNET_PRIV *privp )
{
	ethHw_macEnableSet( ETHHW_PORT_INT, 0, 0 );
	ethHw_impEnableSet( 0 );

	if ( privp->hw_cfg.gpioPhy0 > 0 )
	{
		/* Hold in reset */
		gpio_direction_output( privp->hw_cfg.gpioPhy0, 0 );
		gpio_free( privp->hw_cfg.gpioPhy0 );
	}

	if ( privp->hw_cfg.gpioPhy1 > 0 )
	{
		/* Hold in reset */
		gpio_direction_output( privp->hw_cfg.gpioPhy1, 0 );
		gpio_free( privp->hw_cfg.gpioPhy1 );
	}

	if ( privp->hw_cfg.gpioPhyPwrEn >= 0 )
	{
		/* Power down */
		gpio_direction_output( privp->hw_cfg.gpioPhyPwrEn, 0 );
		gpio_free( privp->hw_cfg.gpioPhyPwrEn );
	}

	if (phy_regulator) {
		regulator_disable(phy_regulator);
		regulator_put(phy_regulator);
		phy_regulator = NULL;
	}

#ifdef CONFIG_ARCH_CAPRI
	clk_disable( esub_axi_clk );
	clk_disable( esw_sys_clk );
	clk_disable( esub_clk );

	clk_put( esub_axi_clk );
	clk_put( esw_sys_clk );
	clk_put( esub_clk );
#endif

	return 0;
}

static int bcm_enet_mdio_read( struct mii_bus *bp, int mii_id, int reg )
{
	BCMNET_PRIV *privp = bp->priv;
	uint32_t val;
	spin_lock_bh( &privp->lock );
	val = ethMiiGet( mii_id, reg );
	spin_unlock_bh( &privp->lock );
	return val;
}

static int bcm_enet_mdio_write( struct mii_bus *bp, int mii_id,
		int reg, u16 val )
{
	BCMNET_PRIV *privp = bp->priv;
	spin_lock_bh( &privp->lock );
	ethMiiSet( mii_id, reg, val );
	//printk("---PHY bcm_enet_mdio_write(%d-%x-%x)\n", mii_id, reg, val);
	spin_unlock_bh( &privp->lock );
	return 0;
}

static int bcm_enet_mdio_reset( struct mii_bus *bp )
{
	(void)bp;
	return 0;
}

static irqreturn_t ethIsrRx(int irq, void *userdata)
{
	BCMNET_PRIV *privp = userdata;
	(void)irq;

	ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_RX );

	if ( likely( netif_running( privp->ndev )))
	{
		NET_RX_LOG( "in - napi scheduled" );
		ethHw_dmaIrqDisable( DMA_ESW_MODULE, DMA_ESW_CH_RX );
		napi_schedule( &privp->napi );
	}

	return IRQ_HANDLED;
}

/* TX DMA complete interrupt */
static irqreturn_t ethIsrTx( int irq, void *userdata )
{
	BCMNET_PRIV *privp = userdata;
	(void)irq;

	NET_TX_LOG( "in" );
	ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_TX );

	/* trigger tx processing in case packets are waiting */
	tasklet_schedule( &privp->txtasklet );

	if ( unlikely( netif_queue_stopped( privp->ndev )))
	{
		netif_wake_queue( privp->ndev );
	}

	return IRQ_HANDLED;
}

static inline __be16 brcmPacketType_eth_type_trans(struct sk_buff *skb)
{
	__be16 type;

	/* This function is modelled after eth_type_trans() */

	type = *((__be16 *)skb->data);

	/* Move data pointer to start of payload */
	skb_pull(skb, sizeof( __be16 ));

	/* Determine ethernet type.  Please refer to eth_type_trans() for an
	 *  explaination on the logic below
	 */
	if (ntohs(type) < 1536) {
		if (type == 0xFFFF)
			type = htons(ETH_P_802_3);
		else
			type = htons(ETH_P_802_2);
	}

	return type;
}

static inline void __brcmPacketTypeHandler(struct sk_buff *skb)
{
	/* Adjust skb to start of the true ethernet type field */
	skb_pull(skb, ETHHW_BRCM_HDR_LEN - sizeof( __be16 ));

	/* Update ethernet type field in skb */
	skb->protocol = brcmPacketType_eth_type_trans(skb);
}

static int brcmPacketTypeHandler(struct sk_buff *skb, struct net_device *dev,
                                 struct packet_type *ptype,
                                 struct net_device *orig_dev)
{
	/* The intent of the Broadcom ethernet type is to allow a raw socket
	 *  application to control the egress port for tx/send() frames and/or
	 *  determine the ingress port for rx/recv() frames while maintaining
	 *  original compatibility
	 */

	(void)dev;
	(void)ptype;
	(void)orig_dev;

	__brcmPacketTypeHandler(skb);

	/* Re-queue packet to IP stack */
	netif_rx( skb );

	return NET_RX_SUCCESS;
}

static int bcm_enet_open( struct net_device *dev )
{
	BCMNET_PRIV *privp;
	ETH_PRIV *ethp;
	int err;
	struct device *kdev;

	sysCtlMsgLevel = MSG_LEVEL_OFF;
	sysCtlPmDisablePortPc = 1;
	sysCtlPmDisablePortLan = 0;
	sysCtlPmDisablePortInternal = 0;
	sysCtlRxBytes = 0;
	sysCtlRxDroppedPkts = 0;
	sysCtlRxNoSkb = 0;
	sysCtlRxReSyncs = 0;
	sysCtlRxWraparounds = 0;
	sysCtlRxSyncChecked = 0;
	sysCtlRxSyncDroppedPkts = 0;
	sysCtlHeaderp = register_sysctl_table(sysCtl);

	privp = netdev_priv( dev );
	ethp = &privp->eth;
	kdev = &privp->pdev->dev;

	/* Allocate a TX fifo to stash to hold skb pointers */
	err = kfifo_alloc( &privp->dma.txfifo,
			DMA_TX_MAX_QUEUE_LEN*sizeof(void *), GFP_KERNEL );
	if ( err )
	{
		netdev_err( dev, "cannot alloc tx fifo, err=%i\n", err );
		return err;
	}

	/* Start DMA block */
	err = dmaStart( privp );
	if (err)
	{
		netdev_err( dev, "Failed to start DMA block\n" );
		goto out_free_kfifo;
	}

	if ( privp->num_phys == 1 )
	{
		/* Single port configuration */

		/* Power-up the SGMII/PHY */
		ethHw_phyPowerdownSet(privp->eth.phyPort, 0);
	}
	else
	{
		/* Non-single port configuration */

		/* The PHYs cannot be powered-down since the external ports may still be
		 *  bridging network traffic
		 */

		/* TODO: Define other power saving operations for this configuration,
		 *        for example, if only there is only one port has a link, perhaps
		 *        keep PHYs enabled and disable MAC and switch clock.
		 */
	}

	/* FIXME: make this configuration a platform cfg */
	if (BRCM_ENCAP_MODE == ENCAP_MODE_KEEP_HANDLER)
	{
		/* Add packet handler for Broadcom raw socket packet type */
		dev_add_pack( &brcmPacketType );
	}

	phy_start( privp->phydev );

	/* Enable forwarding to internal port */
	ethHw_impEnableSet(1);
	ethHw_macEnableSet(ETHHW_PORT_INT, 1, 1);

	/* Enable napi before rx interrupts */
	napi_enable( &privp->napi );

	ethHw_dmaEnable(DMA_ESW_MODULE, DMA_ESW_CH_RX, RX_DESC(privp->dma.rx.addr, 0));

	netif_start_queue( dev );

	return 0;

out_free_kfifo:
	kfifo_free( &privp->dma.txfifo );

	return err;
}

static int bcm_enet_close(struct net_device *dev)
{
	BCMNET_PRIV *privp;
	ETH_PRIV *ethp;

	privp = netdev_priv(dev);
	ethp = &privp->eth;

	netif_stop_queue( dev );

	ethHw_dmaIrqDisable(DMA_ESW_MODULE, DMA_ESW_CH_RX);
	ethHw_dmaIrqDisable(DMA_ESW_MODULE, DMA_ESW_CH_TX);
	dmaStop( privp );

	napi_disable(&privp->napi);

	/* Disable forwarding to internal port */
	ethHw_macEnableSet( ETHHW_PORT_INT, 0, 0 );
	ethHw_impEnableSet( 0 );

	phy_stop( privp->phydev );

	netif_carrier_off( dev );

	if ( BRCM_ENCAP_MODE == ENCAP_MODE_KEEP_HANDLER )
	{
		dev_remove_pack( &brcmPacketType );
	}

	if ( privp->num_phys == 1 )
	{
		/* Power-down the SGMII/PHY for PC port */
		ethHw_phyPowerdownSet( ethp->phyPort, 1 );
	}
	else
	{
		/* Non-single port configuration */

		/* The PHYs cannot be powered-down since the external ports may still be
		 *  bridging network traffic
		 */

		/* TODO: Define other power saving operations for this configuration,
		 *        for example, if only there is only one port has a link, perhaps
		 *        keep PHYs enabled and disable MAC and switch clock.
		 */
	}

	kfifo_free( &privp->dma.txfifo );

	if ( sysCtlHeaderp )
	{
		unregister_sysctl_table( sysCtlHeaderp );
		sysCtlHeaderp = NULL;
	}

	return 0;
}

static int bcm_enet_do_ioctl( struct net_device *dev, struct ifreq *ifr,
                                int cmd )
{
	BCMNET_PRIV *privp = netdev_priv( dev );
	struct phy_device *phydev = privp->phydev;

	if ( !netif_running( dev ))
		return -EINVAL;

	if ( phydev == NULL )
		return -ENODEV;

	return phy_mii_ioctl( phydev, ifr, cmd );
}

static void txClean( BCMNET_PRIV *privp )
{
	DMA_PRIV *dmap = &privp->dma;
	const struct net_device *ndev = privp->ndev;
	int pktcurr;

	NET_TX_LOG( "tx_curr=%i tx.index=%i", dmap->tx_curr, dmap->tx.index );

	for ( pktcurr = 0; pktcurr < dmap->tx_curr; pktcurr++ )
	{
		struct sk_buff *skb;
		int len;

		/* payload data */
		skb = dmap->tx_skb_list[pktcurr].skb;
		dmap->tx_skb_list[pktcurr].skb = NULL;
		len = dmap->tx_skb_list[pktcurr].len;
		if ( skb )
		{
			dma_addr_t physAddr = virt_to_dma( NULL, skb->data );
			dma_sync_single_for_cpu( NULL, physAddr, len,
					DMA_TO_DEVICE );
			dev_kfree_skb_any( skb );
		}
		else
		{
			NET_TX_LOG( "skb not found %i", pktcurr );
			netdev_err( ndev, "invalid skb?\n" );
		}
	}

	dmap->tx.index = 0;
	dmap->tx_curr = 0;
}

static void txWork(unsigned long data)
{
	BCMNET_PRIV *privp = (void *)data;

	NET_TX_LOG( "in" );

	if ( ethHw_dmaIsBusy(DMA_ESW_MODULE, DMA_ESW_CH_TX ))
	{
		NET_TX_LOG( "tx dma is busy?" );
		return;
	}

	txClean( privp );
	tx( privp );
}

// poll the PHY registers and over-ride the Capri link status settings
//static int pollLink( char* portp )
static int pollLink( int port )
{
    //eth_port_t port;
    uint32_t override_reg;  /* Override register */
    uint32_t override_settings;
    uint32_t override_mask;
    uint32_t link_speed = 0;    /* in Mbps - valid settings are 0 (no link), 10, 100, 1000 */
    uint32_t full_duplex = 0;   /* 0 = half duplex, 1 = full duplex */
    uint32_t tx_pause = 0;      /* TX Pause resolution */
    uint32_t rx_pause = 0;      /* RX Pause resolution */
    uint32_t phy_data;
    //printk("pollLink()\n");

    //port = (eth_port_t)strtoul(portp, NULL, 0);

    //if (current_phy->phy_flags & PHY_FLAGS_POLL_LINK)
    //{
        /* Get link settings from phy */
        //if (current_phy->phy_id == PHY_BCM5241)
        //{
#if 0
            /* Restart autonegotiation */
            //printk("BCM5421: Restart Auto-Negotiation\n");
            //ethHw_port_phy_get(port, ETH_PORT_PHY_EXT, 0x00, &phy_data);
            ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x00, &phy_data);
            phy_data |= 0x1200; // bit 9 - Restart Auto-Neg, bit 12 - Auto-Neg enable
            //ethHw_port_phy_set(port, ETH_PORT_PHY_EXT, 0x00, phy_data);
            ethHw_miiSet(port, ETHHW_MII_FLAGS_EXT, 0x00, phy_data);

            /* Spin until autonegotiation done - bit 5 of MII Status Register (0x01) */
            //printk("BCM5241: Waiting for auto-negotiation completion\n");
            do
            {
                //ethHw_port_phy_get(port, ETH_PORT_PHY_EXT, 0x01, &phy_data);
                ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x01, &phy_data);
            }
            while (!(phy_data & 0x0020)); // Bit goes from 0->1 when complete
            //printk("BCM5241: Autonegotiation complete\n");
            mdelay( 200 );
#endif
            /* Get latest version of status register */
            //ethHw_port_phy_get(port, ETH_PORT_PHY_EXT, 0x01, &phy_data);
            ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x01, &phy_data);
            if (phy_data & 0x4)
            {
                /* LInk Up */
                /* Read Aux control/status register to get speed statu s*/
                //ethHw_port_phy_get(port, ETH_PORT_PHY_EXT, 0x18, &phy_data);
                ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x18, &phy_data);

                /* Bit 0 - full duplex */
                full_duplex = (phy_data & 0x1) ? 1 : 0;
                /* Bit 1 - speed */
                if (phy_data & 0x2)
                {
                    link_speed = 100;
                }
                else
                {
                    link_speed = 10;
                }

                /* Get Pause capability */
                //ethHw_port_phy_get(port, ETH_PORT_PHY_EXT, 0x19, &phy_data);
                ethHw_miiGet(port, ETHHW_MII_FLAGS_EXT, 0x19, &phy_data);

                if (phy_data & 0x0800) // bit 11
                {
                    tx_pause = 1;
                    rx_pause = 1;
                }
            }
            else
            {
                /* Link down */
                printk("BCM5241: Link down.\n");
                link_speed = 0;
            }
        //}
        /*
        else if (current_phy->phy_id == PHY_...)
        {
        }
        */
        //else
        //{
        //    printf("PHY %s does not have a method ot get link status\n");
        //    link_speed = 0;
        //    full_duplex = 0;
        //    tx_pause = 0;
        //    rx_pause = 0;
        //}

        if (link_speed == 0)
        {
            printk("PHY detected no link\n");
            full_duplex = 0;
            tx_pause = 0;
            rx_pause = 0;
        }
        else
        {
            //printk("PHY settings: link %d Mbps, %s duplex, tx pause %d, rx pause %d\n", link_speed, (full_duplex) ? "full" : "half", tx_pause, rx_pause);
        }

        /* Now configure */
        if (port == 0)
        { /* port 0 */
            override_mask = 0xFFFFFF00; /* Low byte is port 0 */
            switch (link_speed)
            {
                case 1000:
                    override_settings = 0x0; // 00b = 1000Mbps
                    break;

                case 100:
                    override_settings = 0x1; // 01b = 100Mbps
                    break;

                case 10:
                    override_settings = 0x2; // 10b = 10Mbps
                    break;

                default:
                    //printk("Unknown link speed %d\n", link_speed);
                    /* Fall through */
                case 0:
                    override_settings = 0x3; // 11b = link down, reserved
                    break;
            }
            /* Shift bit over as appropriate */
            override_settings <<= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EXT_PORT_LNKSPD_OVERRIDE_SHIFT;
            /* Duplex */
            if (full_duplex)
            {
                override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EN_EXT_PORT_DUPLEX_OVERRIDE_MASK;
            }
            else
            {
                override_settings &= ~(ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EN_EXT_PORT_DUPLEX_OVERRIDE_MASK);
            }
            /* Pause */
            if (tx_pause)
            {
                override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EXT_PORT_TX_PASUE_RESOLUTION_MASK;
            }
            else
            {
                override_settings &= ~(ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EXT_PORT_TX_PASUE_RESOLUTION_MASK);
            }
            if (rx_pause)
            {
                override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EXT_PORT_RX_PASUE_RESOLUTION_MASK;
            }
            else
            {
                override_settings &= ~(ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EXT_PORT_RX_PASUE_RESOLUTION_MASK);
            }
            /* Force override */
            override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_0_EN_EXT_PORT_OVERRIDE_EN_MASK;
        }
        else
        { /* port 1 */
            override_mask = 0xFFFF00FF; /* port 1 */
            switch (link_speed)
            {
                case 1000:
                    override_settings = 0x0; // 00b = 1000Mbps
                    break;

                case 100:
                    override_settings = 0x1; // 01b = 100Mbps
                    break;

                case 10:
                    override_settings = 0x2; // 10b = 10Mbps
                    break;

                default:
                    printk("Unknown link speed %d\n", link_speed);
                    /* Fall through */
                case 0:
                    override_settings = 0x3; // 11b = link down, reserved
                    break;
            }
            /* Shift bit over as appropriate */
            override_settings <<= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EXT_PORT_LNKSPD_OVERRIDE_SHIFT;
            /* Duplex */
            if (full_duplex)
            {
                override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EN_EXT_PORT_DUPLEX_OVERRIDE_MASK;
            }
            else
            {
                override_settings &= ~(ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EN_EXT_PORT_DUPLEX_OVERRIDE_MASK);
            }
            /* Pause */
            if (tx_pause)
            {
                override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EXT_PORT_TX_PASUE_RESOLUTION_MASK;
            }
            else
            {
                override_settings &= ~(ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EXT_PORT_TX_PASUE_RESOLUTION_MASK);
            }
            if (rx_pause)
            {
                override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EXT_PORT_RX_PASUE_RESOLUTION_MASK;
            }
            else
            {
                override_settings &= ~(ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EXT_PORT_RX_PASUE_RESOLUTION_MASK);
            }
            /* Force override */
            override_settings |= ESW_BUSIF_IMP_EXTERNAL_PORT_RGMII_OVERRIDE_PORT_1_EN_EXT_PORT_OVERRIDE_EN_MASK;
        }

        //override_reg = ETH_REG32_GET(ETH_BASE_ADDR | 0xA0098); /* esw_busif_imp.external_port_rgmii_override */
        ETHHW_REG_GET(ethHw_regImpRgmiiOverride, override_reg);
        //printk("DEBUG: override_reg 0x%08X, override_mask 0x%08X, override_settings 0x%08X\n", override_reg, override_mask, override_settings);
        override_reg &= override_mask;
        override_reg |= override_settings;
        //printk("DEBUG: override_reg 0x%08X\n", override_reg);
        //ETH_REG32_SET(ETH_BASE_ADDR | 0xA0098, override_reg);
        ETHHW_REG_SET(ethHw_regImpRgmiiOverride, override_reg);
    //}
    //else
    //{
    //    printf("PHY %s does not require link to be forced\n", current_phy->phy_name);
    //}

    return 0;
}

static void bcm_enet_handle_link_change( struct net_device *dev )
{
	BCMNET_PRIV *privp;
	struct phy_device *phydev;
	int status_change = 0;

	privp = netdev_priv( dev );
	phydev = privp->phydev;

	spin_lock_bh( &privp->lock );

    if (phydev->phy_id == 0x143bc31)
    {
        pollLink(phydev->addr);
    }

	if ( phydev->link )
	{
		if ( phydev->speed != privp->speed )
		{
			int port = phydev->addr;
			uint32_t reg32;

			/* Set internal port to line speed */
#ifdef CONFIG_MACH_CAPRI_FPGA
			ethHw_impSpeedSet( phydev->speed, phydev->speed ); /* rx, tx */
#else
			ethHw_impSpeedSet( phydev->speed, 1000 ); /* rx, tx */
#endif

            if (phydev->phy_id == 0x143bc31)
            {
                //pollLink(phydev->addr);
            }
            else
            {
			if ( phydev->speed == 1000 )
			{
				/* Disable the RGMII transmit timing delay on the external PHY */
				ethHw_miiSet( port, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
				ethHw_miiGet( port, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
				reg32 |= 0x8000; /* write enable */
				reg32 &= ~0x0200; /* turn off bit 9 to disable RGMII Tx delay */
				ethHw_miiSet( port, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
			}
			else
			{
				/* Enable the RGMII transmit timing delay on the external PHY */
				ethHw_miiSet( port, ETHHW_MII_FLAGS_EXT, 0x1c, 0x0c00 );
				ethHw_miiGet( port, ETHHW_MII_FLAGS_EXT, 0x1c, &reg32 );
				reg32 |= 0x8000; /* write enable */
				reg32 |= 0x0200; /* turn on bit 9 to enable RGMII Tx delay */
				ethHw_miiSet( port, ETHHW_MII_FLAGS_EXT, 0x1c, reg32 );
			}
            }

			privp->speed = phydev->speed;
			status_change = 1;
		}

		if ( privp->duplex != phydev->duplex )
		{
			privp->duplex = phydev->duplex;
			status_change = 1;
		}
	}

	if ( phydev->link != privp->link )
	{
		privp->link = phydev->link;
		status_change = 1;
	}

	privp->duplex = phydev->duplex;

	spin_unlock_bh( &privp->lock );

	if ( status_change )
	{
		if ( netif_carrier_ok( dev ))
		{
			netif_info( privp, link, dev, "Link is up at %d Mbps, duplex %s\n",
					privp->speed,
					privp->duplex ? "on" : "off" );
		}
		else
		{
			netif_info( privp, link, dev, "Link is down\n" );
		}
	}
}

/*
*  Ethernet tx processing routine.
*
*  Called within tasklet context when packet(s) is(are) waiting in the
*  fifo.
*/
static void tx( BCMNET_PRIV *privp )
{
	struct dma_priv *dmap = &privp->dma;
	struct net_device *ndev = privp->ndev;
	char *bufp;
	int len;
	dma_addr_t buf_dma, cfg_dma;
	uint64_t *cfgBufp;
	int dmaLen;
	struct sk_buff *skb;
	ETHHW_DMA_DESC *descp;
	ETHHW_DMA_DESC *handlep;
	int portMask;
	int offset;
	int tc;
	int curr, desc_idx;

	descp = NULL;

	/* Build descriptor chain */
	curr = desc_idx = 0;
	while (( len = kfifo_out( &dmap->txfifo, (unsigned char *)&skb,
					sizeof(&skb) )) == sizeof(&skb) )
	{
		bufp = skb->data;
		len = skb->len;

		NET_TX_LOG( "skb->len=%i", len );

		/* Adjust length to meet minimum size */
		if ( unlikely( len < MIN_FRAME_LEN ))
		{
			len = MIN_FRAME_LEN;
		}

		/* Process buffer (frame may require a Broadcom field to be inserted) */
		if ( unlikely( skb_headroom( skb ) < ETHHW_BRCM_HDR_LEN ))
		{
			/* This case should not happen since we initialized the net device to
			*  increase dev->hard_header_len to account for the Broadcom field
			*/
			netdev_err( ndev, "tx requires %i bytes of headroom in "
			         "socket buffer\n", ETHHW_BRCM_HDR_LEN );
			NET_TX_LOG( "tx: insufficient headroom" );
			goto out_free_new_skb;
		}

		FRAME_DUMP( MSG_LEVEL_TX, "Tx", bufp, len );

		/* Allow switch ARL to make forwarding decision */
		portMask = -1;

		if ( isDaMcast( bufp ))
		{
			/* Force all multicast frames from the CPU to both external ports.
			*  Some protocols such as EAPOL and LLDP are terminated at each hop
			*  so they will be discarded by the switch unless we format the frame
			*  to bypass the switch ARL forwarding rules
			*/
			portMask = ETHHW_PORT_MASK(ETHHW_PORT_1) |
			           ETHHW_PORT_MASK(ETHHW_PORT_0);
		}

		/* Format the frame according to ASIC requirements and adjust buffer */
		if ( unlikely(( bufp[12] == 0x81 ) && ( bufp[13] == 0x00 )))
		{
			/* Frame is 802.1pQ tagged so match traffic class with PCP */
			tc = (bufp[14] >> 5) & 0x7;
		}
		else
		{
			/* Use default traffic class */
			tc = TRAFFIC_CLASS;
		}
		offset = ethHw_txFrameFormat( &bufp, &len, portMask, tc, ETHHW_TE_FOLLOW );
		skb_push(skb, offset);

		FRAME_DUMP(MSG_LEVEL_TX_RAW, "Tx Raw", bufp, len);

		/* Get Tx config buffer and configure */
		cfgBufp = &dmap->tx_cfg[curr];

		buf_dma = virt_to_dma( NULL, bufp );
		*cfgBufp = ETHHW_DMA_CFG_OFFSET(buf_dma, len) | ETHHW_DMA_CFG_EOP_MASK;

		descp = TX_DESC(dmap->tx.descp, desc_idx);
		handlep = TX_DESC(dmap->tx.addr, desc_idx);
		desc_idx++;

		/* Tx config descriptor
		*  The config descriptor allows transfer to be purely 64-bit
		*  transactions, spanning an arbitrary number of descriptors,
		*  so information must be provided to define offsets and EOP
		*/
		cfg_dma = dmap->tx_cfg_dma + (unsigned long)cfgBufp -
			(unsigned long)dmap->tx_cfg;
		if ( unlikely( cfg_dma & 7 ))
		{
			netdev_err( ndev, "cfg data address unaligned 0x%lx\n",
					(unsigned long)cfg_dma );
			goto out_free_new_skb;
		}
		dmaLen = ETHHW_DMA_MTP_TRANSACTION_SIZE(cfg_dma, sizeof(uint64_t));
		ETHHW_DMA_DESC_CREATE(descp,
		                      cfg_dma,
		                      ETHHW_DMA_MTP_FIFO_ADDR_CFG,
		                      handlep,
		                      ETHHW_DMA_MTP_CTL_LO,
		                      dmaLen);

		/* Map data buffer to update external memory */
		dma_sync_single_for_device(NULL, buf_dma, len, DMA_TO_DEVICE);

		descp = TX_DESC(dmap->tx.descp, desc_idx);
		handlep = TX_DESC(dmap->tx.addr, desc_idx);
		desc_idx++;

		/* Tx config descriptor
		*  The config descriptor allows transfer to be purely 64-bit
		*  transactions, spanning an arbitrary number of descriptors,
		*  so information must be provided to define offsets and EOP
		*/

		/* 64-bit align address before creating descriptor. Length
		 * adjust accordingly by ETHHW_DMA_MTP_TRANSACTION_SIZE
		 */
		dmaLen = ETHHW_DMA_MTP_TRANSACTION_SIZE(buf_dma, len);
		buf_dma &= ~7;
		ETHHW_DMA_DESC_CREATE(descp,
		                      buf_dma,
		                      ETHHW_DMA_MTP_FIFO_ADDR_DATA,
		                      handlep,
		                      ETHHW_DMA_MTP_CTL_LO,
		                      dmaLen);

		/* Add skb to list */
		dmap->tx_skb_list[curr].skb = skb;
		dmap->tx_skb_list[curr].len = len;

		ndev->stats.tx_packets++;
		ndev->stats.tx_bytes += (len - ETHHW_BRCM_HDR_LEN);

		curr++;
		if ( curr >= DMA_TX_MAX_CHAIN_LEN )
		{
			/* Limit DMA chain */
			break;
		}
	}
	dmap->tx_curr = curr;
	dmap->tx.index = desc_idx;

	if ( descp )
	{
		/* Start Tx DMA transfer */
		ETHHW_DMA_DESC_TX_DONE( descp );
		handlep = TX_DESC( dmap->tx.addr, 0 );
		ethHw_dmaIrqEnable( DMA_ESW_MODULE, DMA_ESW_CH_TX );
		wmb();
		ethHw_dmaEnable( DMA_ESW_MODULE, DMA_ESW_CH_TX, handlep );
	}

	return;

out_free_new_skb:
	NET_TX_LOG( "out_free_new_skb" );
	dev_kfree_skb_any( skb );
	txClean( privp );
	/* re-trigger tx processing */
	tasklet_schedule( &privp->txtasklet );
}

static int bcm_enet_hard_start_xmit(struct sk_buff *skb,
                                       struct net_device *dev)
{
	BCMNET_PRIV *privp;
	int len;
	int rc;

	rc = NETDEV_TX_OK;
	privp = netdev_priv(dev);

	NET_TX_LOG( "skb->len=%i", skb->len );

	/* Insert skb pointer into fifo */
	len = kfifo_in( &privp->dma.txfifo, (unsigned char *)&skb, sizeof(skb) );
	if ( unlikely( len != sizeof(skb) ))
	{
		/* Not enough space, which shouldn't happen since the queue
		 * should have been stopped already
		 */
		netif_stop_queue(dev);
		NET_TX_LOG( "xmit called with no tx desc avail!" );
		rc = NETDEV_TX_BUSY;
	}

	tasklet_schedule( &privp->txtasklet );

	return rc;
}

static int bcm_enet_poll( struct napi_struct *napi, int quota )
{
	BCMNET_PRIV *privp = container_of(napi, BCMNET_PRIV, napi);
	struct dma_priv *const dmap = &privp->dma;
	struct net_device *const ndev = privp->ndev;

	struct sk_buff *skb;
	char *bufp;
	char *newBufp;
	int offset;
	int len;
	int used = 0;
	ETHHW_DMA_DESC *descp;
	dma_addr_t dmaBuf;
	dma_addr_t      physAddr;
	bool wrapped = false;

	static int last_used = 0;
#if 1
	static int prev_used[8] = {0};
	static int prev_idx=0;
#endif

	/* Check the last frame we read - if it's in use, we probably wrapped around */
	if (ETHHW_DMA_PTM_TRANSFER_DONE( RX_DESC( dmap->rx.descp, prev( dmap->rx.index ))))
	{
		netdev_info( ndev, "wraparound likely\n");
		sysCtlRxWraparounds++;
		wrapped = true;
	}

	NET_RX_LOG( "quota=%i", quota );
	while ( used < quota )
	{
		/* Get next frame from Rx DMA */
		descp = RX_DESC( dmap->rx.descp, dmap->rx.index );
		if ( ETHHW_DMA_PTM_TRANSFER_DONE( descp ))
		{
			struct skb_list_node *const node = &dmap->rx_skb_list[dmap->rx.index];

			skb = node->skb;
			skb_reserve( skb, ETH_OFFSET_ALIGN );
			len = ETHHW_DMA_BUF_LEN( descp );

			NET_RX_LOG( "done len=%i index=%i", len, dmap->rx.index );

			/* Rearm descriptor with new buffer */
			newBufp = alloc_rx_skb( DMA_RX_BUF_LEN, node );

			if ( !newBufp )
			{
				/*
				 * No skb available.
				 * Leave the existing one in place.
				 * Explicitly discard the frame by marking the buffer as free,
				 * and incrementing the rx index.
				 * That's the best we can do at this point.
				 */
				descp->stat1 = 0;
				descp->stat2 = 0;
				descp->ctlHi = DMA_RX_BUF_LEN;

				inc( &dmap->rx.index );

				ndev->stats.rx_dropped++;

				sysCtlRxNoSkb++;

				if ( netif_msg_rx_err( privp ) && net_ratelimit() )
				{
					netdev_err( ndev, "rx skb alloc err, used=%d\n", used );
				}

				/* Don't try to read any more frames, just drop out of the loop */
				break;
			}

			/* Set buffer ownership to device and invalidate cache */
			dmaBuf = virt_to_dma( NULL, newBufp );
			dma_sync_single_for_device( NULL, dmaBuf,
							DMA_RX_BUF_LEN, DMA_FROM_DEVICE );

			ETHHW_DMA_DESC_RX_UPDATE( descp, dmaBuf, DMA_RX_BUF_LEN );

			inc( &dmap->rx.index );
		}
		else
		{
			/* No more frames */
			NET_RX_LOG( "no frame available - used=%d, last_used=%d", used, last_used );

			/* When we're interrupt-driven, we should always find at least one frame */
			if ((last_used != quota) && !used) {
				netdev_info( ndev, "interrupt with no work. index=%d, last_used=%d\n", dmap->rx.index, last_used);
#if 1
				netdev_info( ndev, "prev_idx=%d, prev_used[] = {%d, %d, %d, %d, %d, %d, %d, %d}\n", prev_idx%8, prev_used[0], prev_used[1], prev_used[2], prev_used[3], prev_used[4], prev_used[5], prev_used[6], prev_used[7]);
#endif
			}

#if defined(ISLAND_NET_DUMP_SYNC)
			/* If we didn't process any packets last time, we won't learn much from a dump */
			if (!used && last_used)
			{
				int index;

				dump_ring("Rx ring", &dmap->rx);

				for (index=0; index < DMA_RX_DESC_NUM; index++ )
				{
					descp = RX_DESC( dmap->rx.descp, index );
					if ( ETHHW_DMA_PTM_TRANSFER_DONE( descp ))
					{
						const struct skb_list_node *node = &dmap->rx_skb_list[index];

						skb = node->skb;
						skb_reserve( skb, ETH_OFFSET_ALIGN );
						len = ETHHW_DMA_BUF_LEN( descp );

						bufp = skb->data;

						physAddr = virt_to_dma(NULL, bufp );
						dma_sync_single_for_cpu( NULL, physAddr, len, DMA_FROM_DEVICE );

        					printk("%s: [%d] len=%i, bufp=0x%08x\n", "Rogue Rx Raw", index, len, (int)(bufp)); \
                				printk(STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT " " \
                       					STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT " " \
                       					STR_OCTETS_FORMAT " " STR_OCTETS_FORMAT "\n", \
                       					STR_OCTETS_ARGS(&bufp[0]), STR_OCTETS_ARGS(&bufp[8]), \
                       					STR_OCTETS_ARGS(&bufp[16]), STR_OCTETS_ARGS(&bufp[24]), \
                       					STR_OCTETS_ARGS(&bufp[32]), STR_OCTETS_ARGS(&bufp[40])); \
					}
				}
			}
#endif

#if defined(ISLAND_NET_RECOVER_SYNC)
			/*
			 * Check whether sync-up is required
			 * This could be due to exhausting the ring buffer
			 * where the DMA engine will write to one entry again
			 * before the driver has read it the first time.
			 * The clue is that we receive an interrupt but don't find any frames.
			 */
			if (!used)
			{
				ETHHW_DMA_DESC *dp;
				int index;
				bool sync = false;

				sysCtlRxSyncChecked++;

				/* Starting from where we expect the next frame, check each slot */
				for ( index = next( dmap->rx.index); index != dmap->rx.index; inc( &index ))
				{
					dp = RX_DESC(dmap->rx.descp, index);
					if ( ETHHW_DMA_PTM_TRANSFER_DONE( dp ) )
					{
						/* Sync-up required */
						sync = true;
						break;
					}
				}

				if ( unlikely( sync ) )
				{
					/*
					 * Assume that the reason we lost sync is because some packets were
					 * overwritten and hence lost. Add those packets to the statistics.
					 */
					const int lost = sub( index, dmap->rx.index );

					sysCtlRxSyncDroppedPkts += lost;
                        		ndev->stats.rx_dropped += lost;

					netdev_info( ndev, "Re-sync - old=%d, new=%d, last_used=%d\n", dmap->rx.index, index, last_used);
#if 1
					netdev_info( ndev, "prev_idx=%d, prev_used[] = {%d, %d, %d, %d, %d, %d, %d, %d}\n", prev_idx%8, prev_used[0], prev_used[1], prev_used[2], prev_used[3], prev_used[4], prev_used[5], prev_used[6], prev_used[7]);
#endif
					dmap->rx.index = index;
					sysCtlRxReSyncs++;
					//continue;
				}
				/*
				 * We expect to sometimes get no packets when a burst ends and we switch back from
				 * polling to interrupt-driven, but when we are already interrupt-driven, no packets
				 * indicates that something strange is going on.
				 */
				else if ( unlikely( last_used != quota ))
				{
					netdev_info( ndev, "No need to sync - no packets found (last_used=%d)\n", last_used);
				}
			}
#endif
			break;
		}

		bufp = skb->data;

		sysCtlRxBytes += len;

		physAddr = virt_to_dma(NULL, bufp );
		dma_sync_single_for_cpu( NULL, physAddr, len, DMA_FROM_DEVICE );

		FRAME_DUMP(MSG_LEVEL_RX_RAW, "Rx Raw", bufp, len);

		/* Format the frame according to ASIC requirements and adjust buffer */
		if ( likely( BRCM_ENCAP_MODE == ENCAP_MODE_REMOVE))
		{
			offset = ethHw_rxFrameFormat(&bufp, &len);
		}
		else
		{
			offset = ethHw_rxFrameFormatRaw(&bufp, &len);
		}
		skb_reserve( skb, offset );

		FRAME_DUMP(MSG_LEVEL_RX, "Rx", bufp, len);

		if (unlikely(len < MIN_FRAME_LEN)) {
			/* Frame size is invalid, so discard */
			netdev_err( ndev, "bad frame: len=%i mode=%i\n", len,
					BRCM_ENCAP_MODE );
			FRAME_DUMP_ALWAYS( "Invalid frame size received\n",
					bufp, len);
			dev_kfree_skb_any(skb);
			ndev->stats.rx_dropped++;
			sysCtlRxDroppedPkts++;
			continue;
		}

		/* Update remainder of socket buffer information */
		skb_put(skb, len);
		skb->dev = ndev;
		skb->protocol = eth_type_trans(skb, ndev);

		if ( unlikely( BRCM_ENCAP_MODE == ENCAP_MODE_KEEP_SKIP )) {
			__brcmPacketTypeHandler(skb);
		}

		skb->ip_summed = CHECKSUM_NONE;

		if ( unlikely( BRCM_ENCAP_MODE != ENCAP_MODE_REMOVE ))
		{
			/* Report length of Rx data that was received over
			 * the wire, so exclude internal Broadcom tag */
			len -= ETHHW_BRCM_HDR_LEN;
		}
		ndev->stats.rx_bytes += len;
		ndev->stats.rx_packets++;

		NET_RX_LOG( "netif_receive_skb skb->data=0x%lx", (unsigned long)skb->data );
		netif_receive_skb( skb );

		used++;
	}

	/* If quota not fully consumed, exit polling mode */
	if ( likely( used < quota ))
	{
                NET_RX_LOG("exiting polling mode");
		napi_complete( napi );
		ethHw_dmaIrqClear( DMA_ESW_MODULE, DMA_ESW_CH_RX );
		ethHw_dmaIrqEnable( DMA_ESW_MODULE, DMA_ESW_CH_RX );
	}

	last_used = used;
#if 1
	prev_used[prev_idx++%8] = used;
#endif

	return used;
}

static void bcm_enet_set_multicast_list(struct net_device *dev)
{
	/* TODO:  Add multicast list support */
}

static int bcm_enet_set_mac_address(struct net_device *dev, void *addr)
{
	unsigned char *macp;
	int rc;

	rc = eth_mac_addr( dev, addr );
	if ( rc < 0 )
	{
		return rc;
	}

	macp = ((struct sockaddr *)addr)->sa_data;
	ethHw_arlEntrySet( macp, 0, ETHHW_PORT_INT, 0, 1, 1 );

	return 0;
}

static int bcm_enet_set_mac_address_raw(struct net_device *dev, unsigned char *macp)
{
       ethHw_arlEntrySet( macp, 0, ETHHW_PORT_INT, 0, 1, 1 );
       memcpy( dev->dev_addr, macp, dev->addr_len );
       return 0;
}

static struct net_device_stats *bcm_enet_get_stats(struct net_device *dev)
{
	return &dev->stats;
}

static void bcm_enet_tx_timeout(struct net_device *dev)
{
	BCMNET_PRIV *privp;

	privp = netdev_priv(dev);

	netdev_warn( dev, "tx timeout: dma_busy=%i\n",
			ethHw_dmaIsBusy(DMA_ESW_MODULE, DMA_ESW_CH_TX ));

	dev->stats.tx_errors++;
	dev->trans_start = jiffies; /* prevent tx timeout */

	netif_wake_queue( dev );

}

#ifdef CONFIG_NET_POLL_CONTROLLER
/*
 * Polling 'interrupt' - used by things like netconsole to send skbs
 * without having to re-enable interrupts. It's not called while
 * the interrupt routine is executing.
 */
static void bcm_enet_poll_controller(struct net_device *dev)
{
	disable_irq( DMA_TX_IRQ );
	ethIsrTx( DMA_TX_IRQ, netdev_priv( dev ));
	enable_irq( DMA_TX_IRQ );
}
#endif

static u32 bcm_enet_get_msglevel( struct net_device *dev )
{
        struct  bcm_enet_priv *bp = netdev_priv( dev );
        return bp->msg_enable;
}

static void bcm_enet_set_msglevel( struct net_device *dev, u32 value )
{
        struct bcm_enet_priv *bp = netdev_priv( dev );
        bp->msg_enable = value;
}

static int bcm_enet_get_settings( struct net_device *dev,
		struct ethtool_cmd *cmd )
{
	struct bcm_enet_priv *bp = netdev_priv( dev );
	struct phy_device *phydev = bp->phydev;

	if ( phydev == NULL )
		return -ENODEV;

	return phy_ethtool_gset( phydev, cmd );
}

static int bcm_enet_set_settings( struct net_device *dev,
		struct ethtool_cmd *cmd )
{
	struct bcm_enet_priv *bp = netdev_priv( dev );
	struct phy_device *phydev = bp->phydev;

	if ( phydev == NULL )
		return -ENODEV;

	return phy_ethtool_sset( phydev, cmd );
}

static void bcm_enet_get_drvinfo( struct net_device *dev,
			     struct ethtool_drvinfo *info )
{
	strcpy( info->driver, MOD_NAME );
	strcpy( info->version, MOD_VERSION );
	strcpy( info->fw_version, "N/A" );
	strcpy( info->bus_info, "0" );
}


static int bcm_enet_get_regs_len( struct net_device *dev )
{
	(void)dev;
	return BCM_ENET_REGBLK_SIZE;
}

static void bcm_enet_get_regs( struct net_device *dev,
		struct ethtool_regs *regs, void *buf )
{
	struct bcm_enet_esw_regs *rp = buf;

	(void)dev;
	regs->version = 0;

	rp->port_ctrl0 = ETHHW_REG_GET_VAL( ethHw_regCtrlPortCtrl(0) );
	rp->port_ctrl1 = ETHHW_REG_GET_VAL( ethHw_regCtrlPortCtrl(1) );
	rp->imp_port_ctrl = ETHHW_REG_GET_VAL( ethHw_regCtrlImpCtrl );
	rp->switch_mode = ETHHW_REG_GET_VAL( ethHw_regCtrlSwitchMode );
	rp->protected_port_sel =
		ETHHW_REG_GET_VAL( ethHw_regCtrlProtectPortSel );
	rp->link_status = ETHHW_REG_GET_VAL( ethHw_regStatusLinkStatus);
	rp->link_status_change =
		ETHHW_REG_GET_VAL( ethHw_regStatusLinkStatusChange );
	rp->port_speed = ETHHW_REG_GET_VAL( ethHw_regStatusPortSpeed );
	rp->mgmt_config = ETHHW_REG_GET_VAL( ethHw_regMgmtGblMgmtCtrl );
	rp->arl_rw_ctrl = ETHHW_REG_GET_VAL( ethHw_regArlRwCtrl );
	rp->ptm_config = ETHHW_REG_GET_VAL( ethHw_regPtmConfig );
	rp->ptm_dma_status = ETHHW_REG_GET_VAL( ethHw_regPtmDmaStatus );
	rp->ptm_fifo_config = ETHHW_REG_GET_VAL( ethHw_regPtmFifoConfig );
	rp->mtp_config = ETHHW_REG_GET_VAL( ethHw_regMtpConfig );
	rp->mtp_dma_status = ETHHW_REG_GET_VAL( ethHw_regMtpDmaStatus );
	rp->mtp_fifo_config = ETHHW_REG_GET_VAL( ethHw_regMtpFifoConfig );
	rp->imp_phy_misc = ETHHW_REG_GET_VAL( ethHw_regImpPhyMisc );
	rp->imp_speed = ETHHW_REG_GET_VAL( ethHw_regImpSpeed );
}

static int bcm_enet_nway_reset( struct net_device *dev )
{
	struct bcm_enet_priv *bp = netdev_priv( dev );
	int rc = -ENODEV;

	if ( !netif_running( dev ))
		return -EAGAIN;

	if ( bp->phydev )
	{
		rc = phy_start_aneg( bp->phydev );
	}

	return rc;
}

static void bcm_enet_get_strings( struct net_device *dev, u32 stringset,
		u8 *buf )
{
	switch ( stringset )
	{
		case ETH_SS_STATS:
			memcpy( buf, &ethtool_stats_keys,
					sizeof(ethtool_stats_keys) );
			break;
		default:
			WARN_ON( 1 );	/* we need a WARN() */
			break;
	}
}

static void bcm_enet_get_ringparam( struct net_device *dev,
		struct ethtool_ringparam *ering )
{
	(void)dev;

        ering->rx_max_pending = DMA_RX_DESC_NUM;
        ering->rx_mini_max_pending = 0;
        ering->rx_jumbo_max_pending = 0;
        ering->tx_max_pending = DMA_TX_MAX_QUEUE_LEN;

        ering->rx_pending = DMA_RX_DESC_NUM;
        ering->rx_mini_pending = 0;
        ering->rx_jumbo_pending = 0;
        ering->tx_pending = DMA_TX_MAX_QUEUE_LEN;
}

static void bcm_enet_get_ethtool_stats( struct net_device *dev,
		struct ethtool_stats *estats, u64 *tmp_stats )
{
	struct bcm_enet_priv *bp = netdev_priv( dev );
	int port = bp->phydev->addr;
	struct bcm_enet_mib *mibp = (void *)tmp_stats;

	GETMIB( mibp, port, TxOctets );
	GETMIB( mibp, port, TxDropPkts );
	GETMIB( mibp, port, TxBroadcastPkts );
	GETMIB( mibp, port, TxMulticastPkts );
	GETMIB( mibp, port, TxUnicastPkts );
	GETMIB( mibp, port, TxCollisions );
	GETMIB( mibp, port, TxSingleCollision );
	GETMIB( mibp, port, TxMultipleCollision );
	GETMIB( mibp, port, TxDeferredTransmit );
	GETMIB( mibp, port, TxLateCollision );
	GETMIB( mibp, port, TxExcessiveCollision );
	GETMIB( mibp, port, TxFrameInDisc );
	GETMIB( mibp, port, TxPausePkts );
	GETMIB( mibp, port, RxOctets );
	GETMIB( mibp, port, RxUndersizePkts );
	GETMIB( mibp, port, RxPausePkts );
	GETMIB( mibp, port, Pkts64Octets );
	GETMIB( mibp, port, Pkts65To127Octets );
	GETMIB( mibp, port, Pkts128To255Octets );
	GETMIB( mibp, port, Pkts256To511Octets );
	GETMIB( mibp, port, Pkts512To1023Octets );
	GETMIB( mibp, port, Pkts1024ToMaxPktOctets );
	GETMIB( mibp, port, RxOversizePkts );
	GETMIB( mibp, port, RxJabbers );
	GETMIB( mibp, port, RxAlignmentErrors );
	GETMIB( mibp, port, RxFcsErrors );
	GETMIB( mibp, port, RxGoodOctets );
	GETMIB( mibp, port, RxDropPkts );
	GETMIB( mibp, port, RxUnicastPkts );
	GETMIB( mibp, port, RxMulticastPkts );
	GETMIB( mibp, port, RxBroadcastPkts );
	GETMIB( mibp, port, RxSaChanges );
	GETMIB( mibp, port, RxFragments );
	GETMIB( mibp, port, JumboPktCount );
	GETMIB( mibp, port, RxSymbolError );
	GETMIB( mibp, port, RxDiscard );
	GETMIB( mibp, port, DosAssertCount );
	GETMIB( mibp, port, DosDropCount );
	GETMIB( mibp, port, IngressRateAssertCount );
	GETMIB( mibp, port, IngressRateDropCount );
	GETMIB( mibp, port, EgressRateAssertCount );
}

static int bcm_enet_get_sset_count( struct net_device *dev, int sset )
{
	switch ( sset )
	{
		case ETH_SS_STATS:
			return ARRAY_SIZE( ethtool_stats_keys );
		default:
			return -EOPNOTSUPP;
	}
}

static int bcm_enet_mii_probe( struct net_device *dev )
{
	BCMNET_PRIV *privp = netdev_priv( dev );
	struct phy_device *phydev = NULL;
	int phy_addr;
	struct device *kdev = &privp->pdev->dev;
	int rc;

	/* find first phy */
	for ( phy_addr = 0; phy_addr < PHY_MAX_ADDR; phy_addr++ )
	{
		if ( privp->mii_bus->phy_map[phy_addr] )
		{
			phydev = privp->mii_bus->phy_map[phy_addr];
			break;
		}
	}

	if ( phydev == NULL )
	{
		dev_err( kdev, "no PHY found\n" );
		return -ENODEV;
	}
	privp->link = 0;
	privp->speed = 0;
	privp->duplex = -1;
	privp->phydev = phydev;

	/* TODO : add pin_irq */

	dev_info( kdev, "Found first phy: id=0x%x addr=%i\n", phydev->phy_id,
			phydev->addr );

	/* attach the mac to the phy */
	phydev = phy_connect( dev, dev_name( &phydev->dev ),
			&bcm_enet_handle_link_change, 0,
			phydev->phy_id == 0x143bc31 ? PHY_INTERFACE_MODE_MII : PHY_INTERFACE_MODE_RGMII );
	if ( IS_ERR( phydev ))
	{
		dev_err( kdev, "Cannot attach to PHY\n" );
		return PTR_ERR( phydev );
	}

	/* TODO: mask with MAC supported features */
	phydev->supported &= PHY_GBIT_FEATURES;
	phydev->supported |= SUPPORTED_Asym_Pause | SUPPORTED_Pause;
	phydev->advertising = phydev->supported;

	rc = phy_start_aneg( phydev );

	return rc;
}

static int bcm_enet_mii_init( struct net_device *dev )
{
	BCMNET_PRIV *privp = netdev_priv( dev );
	int err, i;

	privp->mii_bus = mdiobus_alloc();
	if ( privp->mii_bus == NULL )
	{
		return -ENOMEM;
	}

	privp->mii_bus->name = "bcm_enet mdio bus";
	snprintf( privp->mii_bus->id, MII_BUS_ID_SIZE, "%x", 0 );
	privp->mii_bus->priv = privp;
	privp->mii_bus->parent = &privp->pdev->dev;
	privp->mii_bus->read = &bcm_enet_mdio_read;
	privp->mii_bus->write = &bcm_enet_mdio_write;
	privp->mii_bus->reset = &bcm_enet_mdio_reset;
	privp->mii_bus->phy_mask = ~0x3; /* FIXME */
	privp->mii_bus->irq = &privp->mdio_irq[0];

	for ( i = 0; i < PHY_MAX_ADDR; i++ )
	{
		privp->mii_bus->irq[i] = PHY_POLL;
	}

	if ( mdiobus_register( privp->mii_bus ))
	{
		err = -ENXIO;
		goto err_out;
	}

	if ( bcm_enet_mii_probe( dev ) != 0 )
	{
		err = -ENXIO;
		goto err_out_unregister_bus;
	}

	return 0;

err_out_unregister_bus:
	mdiobus_unregister( privp->mii_bus );
err_out:
	mdiobus_free( privp->mii_bus );

	return err;
}

/* returns 1 on success */
static int read_macaddr(unsigned char *addr)
{
	int i;
	int is_programmed = 0;
	extern unsigned char _esn_mac[10];
	uint8_t *aes_k = _esn_mac;

	/* check for the existence of non-zero values (is polyfuse programmed?) */
	for (i = 0; i < sizeof(aes_k); ++i) if (aes_k[i]) is_programmed = 1;

	if (is_programmed) {
		unsigned val;
		char serial[16];
		Roku_GetESN(aes_k, serial);
        Roku_GetOUI(serial, addr);
		addr[3] = aes_k[9];
		addr[4] = aes_k[8];
		addr[5] = aes_k[7] & ~1;
		printk("island_net: polyfuse MAC:%02x:%02x:%02x:%02x:%02x:%02x\n",
		                        addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
	} else {
		printk("island_net: polyfuse not programmed");
	}
	return is_programmed;
}

static int bcm_enet_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	BCMNET_PRIV *privp;
	struct island_net_hw_cfg *hw_cfg;
	int rc;
        unsigned char polyfuse_mac[6];

	pr_info( "%s", banner );

	if ( pdev->dev.platform_data == NULL )
	{
		dev_err( &pdev->dev, "Missing platform data\n" );
		return -ENODEV;
	}
	hw_cfg = (struct island_net_hw_cfg *)pdev->dev.platform_data;

	dev_info( &pdev->dev, "net_config: brcmMode=%i, descRx=%i, descTx=%i, "
			"addrPhy0=%i, addrPhy1=%i, gpioPhy0=%i, gpioPhy1=%i, "
			"gpioPhyPwrEn=%i, gpioPhy0_int=%i, gpioPhy1_int=%i\n",
			brcmMode, DMA_RX_DESC_NUM, DMA_TX_DESC_NUM,
			hw_cfg->addrPhy0, hw_cfg->addrPhy1,
			hw_cfg->gpioPhy0, hw_cfg->gpioPhy1, hw_cfg->gpioPhyPwrEn,
			hw_cfg->gpioPhy0_int, hw_cfg->gpioPhy1_int );

	/* Initialize resources required by the driver */
	dev = alloc_etherdev( sizeof(*privp) );
	if ( dev == NULL )
	{
		dev_err( &pdev->dev, "Failed to alloc device\n" );
		return -ENOMEM;
	}

	privp = netdev_priv( dev );
	memset( privp, 0, sizeof(*privp) );
	privp->pdev = pdev;
	privp->ndev = dev;

	spin_lock_init( &privp->lock );
	platform_set_drvdata( pdev, dev );
	SET_NETDEV_DEV( dev, &pdev->dev );

	dev->netdev_ops = &bcm_enet_ops;
	dev->watchdog_timeo = TX_TIMEOUT;
	dev->ethtool_ops = &bcm_enet_ethtool_ops;
	netif_napi_add( dev, &privp->napi, bcm_enet_poll, QUOTA );

	dev->hard_header_len += ETHHW_BRCM_HDR_LEN;
	dev->features &= ~(NETIF_F_SG | NETIF_F_FRAGLIST);
	dev->tx_queue_len = DMA_TX_MAX_QUEUE_LEN;

	memcpy( &privp->hw_cfg, hw_cfg, sizeof(privp->hw_cfg));

	/* Start ethernet block.  This should be done as early as possible to allow
	 *  the switch to begin bridging frames on the external ports
	 */
	rc = ethStart( privp );
	if ( rc )
	{
		dev_err( &pdev->dev, "Failed to start ethernet block\n" );
		goto out_free_netdev;
	}

	/* Clear stats */
	memset( &dev->stats, 0, sizeof(dev->stats) );

	rc = bcm_enet_mii_init( dev );
	if ( rc )
	{
		dev_err( &pdev->dev, "Failed to init mii, rc=%i\n", rc );
		goto out_stop_eth;
	}

	if ( read_macaddr(polyfuse_mac) ) {
		bcm_enet_set_mac_address_raw(dev, polyfuse_mac);
	} else if ( cmdline_mac_set ) {
		bcm_enet_set_mac_address_raw(dev, cmdline_mac);
	}


	tasklet_init( &privp->txtasklet, txWork, (unsigned long)privp );

	rc = register_netdev( dev );
	if ( rc )
	{
		goto out_stop_eth;
	}

	return 0;

out_stop_eth:
	ethStop( privp );
out_free_netdev:
	free_netdev( dev );

	return rc;

}

static int bcm_enet_remove( struct platform_device *pdev )
{
	struct net_device *dev;
	BCMNET_PRIV *privp;

	dev = platform_get_drvdata(pdev);
	privp = netdev_priv( dev );

	phy_disconnect( privp->phydev );
	mdiobus_unregister( privp->mii_bus );
	mdiobus_free( privp->mii_bus );
	if ( privp->hw_cfg.gpioPhy0_int >= 0 )
		gpio_free( privp->hw_cfg.gpioPhy0_int );
	if ( privp->hw_cfg.gpioPhy1_int >= 0 )
		gpio_free( privp->hw_cfg.gpioPhy1_int );

	unregister_netdev( dev );
	platform_set_drvdata( pdev, NULL );
	ethStop( privp );
	free_netdev( dev );

	return 0;
}

static void bcm_enet_shutdown(struct platform_device *pdev)
{
	(void)pdev;

	/* Disable forwarding to CPU from internal port */
	ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);
	ethHw_impEnableSet(0);
}

static int bcm_enet_notify_reboot(struct notifier_block *nb,
                                     unsigned long event, void *ptr)
{
	(void)nb;
	(void)ptr;

	switch (event) {
		case SYS_DOWN:
		case SYS_HALT:
		case SYS_POWER_OFF:
			/* Disable forwarding to CPU from internal port */
			ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);
			ethHw_impEnableSet(0);
			break;

		default:
			break;
	}
	return NOTIFY_DONE;
}

static int bcm_enet_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct net_device *dev;
	BCMNET_PRIV *privp;
	ETH_PRIV *ethp;
	int net_alive;
	int disable_lan, disable_pc, disable_int;

	dev = platform_get_drvdata(pdev);
	privp = netdev_priv(dev);
	ethp = &privp->eth;

	/* Backup current device state */
	privp->wasRunning = netif_running(dev);

	if (privp->wasRunning) {
		/* Shutdown network i/f */
		netif_device_detach(dev);
		phy_stop( privp->phydev );
	}

	net_alive   = 0;/*(pm_net_get_state() == pm_state_alive);*/
	disable_lan = net_alive ? sysCtlPmDisablePortLan : 1;
	disable_pc  = net_alive ? sysCtlPmDisablePortPc : 1;
	disable_int = net_alive ? sysCtlPmDisablePortInternal : 1;

	/* Disable MAC Rx */
	if (disable_lan) {
		ethHw_macEnableSet(ethp->port_lan, 0, 1);
	}
	if ( privp->num_phys > 1 && disable_pc)
	{
		ethHw_macEnableSet(ethp->port_pc, 0, 1);
	}

	/* Let Tx DMA drain */
	if (privp->wasRunning) {
		while (kfifo_len( &privp->dma.txfifo ) != 0)
		;
	}

	/* Disable MAC Tx as well */
	if (disable_lan) {
		ethHw_macEnableSet(ethp->port_lan, 0, 0);
	}
	if ( privp->num_phys > 1 && disable_pc )
	{
		ethHw_macEnableSet(ethp->port_pc, 0, 0);
	}

	/* Disable internal port */
	if (disable_int) {
		ethHw_macEnableSet(ETHHW_PORT_INT, 0, 0);
	}

	/* Power-down PHY(s) */
	if (disable_lan) {
		/*ethHw_phyPowerdownSet(ethp->port_lan, 1);*/
	}
	if ( privp->num_phys > 1 && disable_pc )
	{
		/*ethHw_phyPowerdownSet(ethp->port_pc, 1);*/
	}

	/* Disable switch clock if PHYs are powered down */
	if (( disable_lan && privp->num_phys == 1 ) ||
	    ( disable_lan && disable_pc && privp->num_phys > 1 ))
	{
		/*chipcHw_setClockDisable(chipcHw_CLOCK_ESW);*/
		/*chipcHw_busInterfaceClockDisable(chipcHw_REG_BUS_CLOCK_ESW);*/
	}

	return 0;
}

static int bcm_enet_resume(struct platform_device *pdev)
{
	struct net_device *dev;
	BCMNET_PRIV *privp;
	ETH_PRIV *ethp;
	int net_alive;
	int disable_lan, disable_pc, disable_int;

	dev = platform_get_drvdata(pdev);
	privp = netdev_priv(dev);
	ethp = &privp->eth;

	net_alive   = 0;/*(pm_net_get_state() == pm_state_alive);*/
	disable_lan = net_alive ? sysCtlPmDisablePortLan : 1;
	disable_pc  = net_alive ? sysCtlPmDisablePortPc : 1;
	disable_int = net_alive ? sysCtlPmDisablePortInternal : 1;

	/* Enable switch clock if necessary */
	if (( disable_lan && privp->num_phys == 1 )
	||  ( disable_lan && disable_pc && privp->num_phys > 1 ))
	{
		/*chipcHw_setClockEnable(chipcHw_CLOCK_ESW);*/
		/*chipcHw_busInterfaceClockEnable(chipcHw_REG_BUS_CLOCK_ESW);*/
	}

	/* Power-up the PHY(s) */
	if ( disable_lan )
	{
		/*ethHw_phyPowerdownSet(ethp->port_lan, 0);*/
	}
	if ( privp->num_phys > 1  && disable_pc )
	{
		/*ethHw_phyPowerdownSet( ethp->port_pc, 0 );*/
	}

	/* Enable internal port */
	if ( disable_int )
	{
		ethHw_macEnableSet( ETHHW_PORT_INT, 1, 1 );
	}

	/* Enable external port */
	if ( disable_lan )
	{
		ethHw_macEnableSet( ethp->port_lan, 1, 1 );
	}
	if ( privp->num_phys > 1 && disable_pc )
	{
		ethHw_macEnableSet( ethp->port_pc, 1, 1 );
	}

	if ( privp->wasRunning )
	{
		/* Restore network i/f */
		phy_start( privp->phydev );
		netif_device_attach( dev );
	}

	return 0;
}

static struct platform_driver bcm_enet_driver =
{
	.driver =
	{
		.name = MOD_NAME,
		.owner = THIS_MODULE,
	},
	.probe = bcm_enet_probe,
	.remove = bcm_enet_remove,
	.shutdown = bcm_enet_shutdown,
	.suspend = bcm_enet_suspend,
	.resume = bcm_enet_resume,
};

static int __init bcm_enet_init(void)
{
	int rc;
	rc = register_reboot_notifier( &bcm_enet_notifier_reboot );
	if ( rc )
	{
		return rc;
	}
	rc = platform_driver_register( &bcm_enet_driver );
	if ( rc )
	{
		unregister_reboot_notifier( &bcm_enet_notifier_reboot );
	}
	return rc;
}

static void __exit bcm_enet_exit(void)
{
	unregister_reboot_notifier( &bcm_enet_notifier_reboot );
	platform_driver_unregister( &bcm_enet_driver );
}

static void __init parse_mac_addr(char *macstr)
{
	int i, j;
	unsigned char result, value;

	for ( i = 0; i < 6; i++ )
	{
		result = 0;

		if ( i != 5 && *(macstr + 2) != ':' )
			return;

		for ( j = 0; j < 2; j++ )
		{
			if ( isxdigit(*macstr)
			&& ( value = isdigit(*macstr) ?
				*macstr - '0' :
				toupper(*macstr) - 'A' + 10 ) < 16 )
			{
				result = result * 16 + value;
				macstr++;
			}
			else
			{
				return;
			}
		}
		macstr++;
		cmdline_mac[i] = result;
	}
	cmdline_mac_set = 1;
}

static int __init setup_bcmmac( char *s )
{
	pr_info( "bcm mac = %s\n", s );
	parse_mac_addr( s );
	return 0;
}

__setup( "bcmmac=", setup_bcmmac );

module_init( bcm_enet_init );
module_exit( bcm_enet_exit );

MODULE_DESCRIPTION( MOD_DESCRIPTION );
MODULE_LICENSE( "GPL" );
MODULE_VERSION( MOD_VERSION );
