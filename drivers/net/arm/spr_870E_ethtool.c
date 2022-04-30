/*
 * drivers/net/arm/spr_eth_ethtool.c
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This	program	is free	software; you can redistribute it and/or modify
 * it under the	terms of the GNU General Public	License	as published by
 * the Free Software Foundation; either	version	2 of the License, or
 * (at your option) any	later version.
 *
 * This	program	is distributed in the hope that	it will	be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59	Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com>	
 */

/* ethtool support for spear plus */

#include "spr_870E_eth_syn.h"
#include <asm/uaccess.h>

extern char g_spear_driver_name[];
extern char g_spear_driver_version[];

struct spear_stats {
	char stat_string[ETH_GSTRING_LEN];
	int sizeof_stat;
	int stat_offset;
};

#define	SPEAR_STAT(m) sizeof(((struct eth_priv *)0)->m), \
		      offsetof(struct eth_priv,	m)
static const struct spear_stats	spear_gstrings_stats[] = {
	{ "rx_packets",	SPEAR_STAT(stats.rx_packets) },
	{ "tx_packets",	SPEAR_STAT(stats.tx_packets) },
	{ "rx_bytes", SPEAR_STAT(stats.rx_bytes) },
	{ "tx_bytes", SPEAR_STAT(stats.tx_bytes) },
	{ "rx_errors", SPEAR_STAT(stats.rx_errors) },
	{ "tx_errors", SPEAR_STAT(stats.tx_errors) },
	{ "rx_dropped",	SPEAR_STAT(stats.rx_dropped) },
	{ "tx_dropped",	SPEAR_STAT(stats.tx_dropped) },
	{ "multicast", SPEAR_STAT(stats.multicast) },
	{ "collisions",	SPEAR_STAT(stats.collisions) },
	{ "rx_length_errors", SPEAR_STAT(stats.rx_length_errors) },
	{ "rx_over_errors", SPEAR_STAT(stats.rx_over_errors) },
	{ "rx_crc_errors", SPEAR_STAT(stats.rx_crc_errors) },
	{ "rx_frame_errors", SPEAR_STAT(stats.rx_frame_errors) },
	{ "rx_fifo_errors", SPEAR_STAT(stats.rx_fifo_errors) },
	{ "rx_missed_errors", SPEAR_STAT(stats.rx_missed_errors) },
	{ "tx_aborted_errors", SPEAR_STAT(stats.tx_aborted_errors) },
	{ "tx_carrier_errors", SPEAR_STAT(stats.tx_carrier_errors) },
	{ "tx_fifo_errors", SPEAR_STAT(stats.tx_fifo_errors) },
	{ "tx_heartbeat_errors", SPEAR_STAT(stats.tx_heartbeat_errors) },
	{ "tx_window_errors", SPEAR_STAT(stats.tx_window_errors) },
	{ "tx_deferred_ok", SPEAR_STAT(hw_stats.dc) },
	{ "rx_long_length_errors", SPEAR_STAT(hw_stats.roc) },
	{ "rx_short_length_errors", SPEAR_STAT(hw_stats.ruc) },
	{ "tx_tcp_seg_good", SPEAR_STAT(hw_stats.tsctc)	},
	{ "tx_tcp_seg_failed", SPEAR_STAT(hw_stats.tsctfc) },
	{ "rx_flow_control_xon", SPEAR_STAT(hw_stats.xonrxc) },
	{ "rx_flow_control_xoff", SPEAR_STAT(hw_stats.xoffrxc) },
	{ "tx_flow_control_xon", SPEAR_STAT(hw_stats.xontxc) },
	{ "tx_flow_control_xoff", SPEAR_STAT(hw_stats.xofftxc) },
	{ "rx_long_byte_count",	SPEAR_STAT(hw_stats.gorcl) },
	{ "rx_csum_offload_good", SPEAR_STAT(hw_csum_good) },
	{ "rx_csum_offload_errors", SPEAR_STAT(hw_csum_err) }
};

#define	SPEAR_STATS_LEN	\
(sizeof	(spear_gstrings_stats) / sizeof	(struct	spear_stats))

static const char spear_gstrings_test[][ETH_GSTRING_LEN] =
	{ "Register	test  (offline)", "Eeprom test	  (offline)"};


#define	SPEAR_TEST_LEN (sizeof(spear_gstrings_test) / ETH_GSTRING_LEN)


static void spear_force_link_update(struct eth_priv *ndev)
{
	netif_carrier_off(ndev->dev);
	if (timer_pending(&ndev->tmr_hotplug))
		mod_timer(&ndev->tmr_hotplug, jiffies +	PHY_POLL_LINK_OFF);
}


/* Ethtool support */

/*
 * NAME:  SPEAR_ETHTOOL_gset
 *
 * DESCRIPTION:
 *    returns the ethernet settings to the user
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_cmd*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

static int
SPEAR_ETHTOOL_gset (struct net_device *netdev, struct ethtool_cmd *ecmd)
{
	struct eth_priv	*adap =	netdev->priv;
	ecmd->supported	= adap->phy.features;
	ecmd->port = PORT_MII;
	ecmd->phy_address = adap->phy.address;
	ecmd->transceiver = XCVR_EXTERNAL;
	local_bh_disable();
	ecmd->advertising = adap->phy.advertising;
	ecmd->autoneg =	adap->phy.autoneg;
	if (netif_carrier_ok (adap->dev))
	{
		ecmd->speed = adap->phy.speed;
		ecmd->duplex = adap->phy.duplex;
	}
	else
	{
		ecmd->speed  =	-1;
		ecmd->duplex = -1;
	}
	local_bh_enable();
	return 0;
}

/*
 * NAME: SPEAR_ETHTOOL_sset
 *
 * DESCRIPTION:
 *    Sets the eeprom with the user settings
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_cmd*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static int
SPEAR_ETHTOOL_sset (struct net_device *netdev, struct ethtool_cmd *ecmd)
{
	struct eth_priv *dev = netdev->priv;
	u32 f =	dev->phy.features;

	/* Basic sanity	checks */
	if (ecmd->autoneg != AUTONEG_ENABLE && ecmd->autoneg !=	AUTONEG_DISABLE)
		return -EINVAL;
	if (ecmd->autoneg == AUTONEG_ENABLE && ecmd->advertising == 0)
		return -EINVAL;
	if (ecmd->duplex != DUPLEX_HALF	&& ecmd->duplex	!= DUPLEX_FULL)
		return -EINVAL;


	if (ecmd->autoneg == AUTONEG_DISABLE) {
		switch (ecmd->speed) {
		case SPEED_10:
			if (ecmd->duplex == DUPLEX_HALF
				&& !(f & SUPPORTED_10baseT_Half))
					return -EINVAL;
			if (ecmd->duplex == DUPLEX_FULL
				&& !(f & SUPPORTED_10baseT_Full))
					return -EINVAL;
			break;
		case SPEED_100:
			if (ecmd->duplex == DUPLEX_HALF
				&& !(f & SUPPORTED_100baseT_Half))
					return -EINVAL;
			if (ecmd->duplex == DUPLEX_FULL
				&& !(f & SUPPORTED_100baseT_Full))
					return -EINVAL;
			break;
		case SPEED_1000:
			if (ecmd->duplex == DUPLEX_HALF
				&& !(f & SUPPORTED_1000baseT_Half))
					return -EINVAL;
			if (ecmd->duplex == DUPLEX_FULL
				&& !(f & SUPPORTED_1000baseT_Full))
					return -EINVAL;
			break;
		default:
			return -EINVAL;
		}

		local_bh_disable();
		dev->phy.def->ops->setup_forced(&dev->phy, ecmd->speed,
						ecmd->duplex);

	} 
	else 
	{
		if (!(f	& SUPPORTED_Autoneg))
			return -EINVAL;

		local_bh_disable();
		dev->phy.def->ops->setup_aneg(&dev->phy,
						(ecmd->advertising & f) |
						(dev->phy.advertising &
						(ADVERTISED_Pause |
						ADVERTISED_Asym_Pause)));
	}
	
	spear_force_link_update(dev);
	local_bh_enable();

	return 0;
}

/*
 * NAME:  SPEAR_ETHTOOL_get_ringparam
 *
 * DESCRIPTION:
 *    get the no of rings
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_ringparam*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/

static void
SPEAR_ETHTOOL_get_ringparam (struct net_device *netdev,
		     struct ethtool_ringparam *ring)
{
    struct eth_priv *adapter	= netdev->priv;
    ring->rx_max_pending	= SPEAR_MAX_RXD;
    ring->tx_max_pending	= SPEAR_MAX_TXD;
    ring->rx_mini_max_pending	= 0;
    ring->rx_jumbo_max_pending	= 0;
    ring->rx_pending		= adapter->rx_ring_count;
    ring->tx_pending		= adapter->tx_ring_count;
    ring->rx_mini_pending	= 0;
    ring->rx_jumbo_pending	= 0;
}

/*
 * NAME:  SPEAR_ETHTOOL_get_pauseparam
 *
 * DESCRIPTION:
 *    Returns the autoneg,tx & rx setting
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_epauseparam*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static void
SPEAR_ETHTOOL_get_pauseparam (struct net_device	*netdev,
			struct ethtool_pauseparam *epause)
{
	struct eth_priv *dev = (struct eth_priv *) netdev->priv;

	local_bh_disable();
	if ((dev->phy.features & SUPPORTED_Autoneg) &&
		(dev->phy.advertising & (ADVERTISED_Pause | ADVERTISED_Asym_Pause)))
		epause->autoneg	= 1;

	if (dev->phy.duplex == DUPLEX_FULL) {
		if (dev->phy.pause)
			epause->rx_pause = epause->tx_pause = 1;
		else if	(dev->phy.asym_pause)
			epause->tx_pause = 1;
	}
	local_bh_enable();
}

/*
 * NAME:  SPEAR_ETHTOOL_get_regs_len
 *
 * DESCRIPTION:
 *    Gets the register	len
 *
 * PARAMETERS:
 * IN  : net_device*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static int
SPEAR_ETHTOOL_get_regs_len (struct net_device *netdev)
{

#define	SPEAR_REGS_LEN 32
	return 0;
}

/*
 * NAME:  SPEAR_ETHTOOL_get_regs
 *
 * DESCRIPTION:
 *    Dump the registers of DMA, MAC & Phy
 *
 * PARAMETERS:
 * IN  : net_device*, data
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

static void
SPEAR_ETHTOOL_get_regs (struct net_device *netdev,struct ethtool_regs *regs,
			void *p)
{
	DEBUGFUNC ("inside gregs\n");
	SPEAR_ETH_Dump_Registers(netdev);
}

/*
 * NAME:  SPEAR_ETHTOOL_nway_reset
 *
 * DESCRIPTION:
 *
 *
 * PARAMETERS:
 * IN  : net_device*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static int SPEAR_ETHTOOL_nway_reset(struct net_device *netdev)
{
	struct eth_priv *dev = (struct eth_priv *) netdev->priv;
	int res	= 0;

	local_bh_disable();
	if (!dev->phy.autoneg) {
		res = -EINVAL;
		goto out;
	}

	dev->phy.def->ops->setup_aneg(&dev->phy, dev->phy.advertising);
	spear_force_link_update(dev);

out:
	local_bh_enable();
	return res;
}

/*
 * NAME:  SPEAR_ETHTOOL_get_stats_cnt
 *
 * DESCRIPTION:
 *    called by	ethtool	-s returns the etherent	status
 *
 * PARAMETERS:
 * IN  : net_device*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/

static int
SPEAR_ETHTOOL_get_stats_cnt (struct net_device *netdev)
{
	return SPEAR_STATS_LEN;
}

/*
 * NAME:  SPEAR_ETHTOOL_get_strings
 *
 * DESCRIPTION:
 *    called by	ethtool	-s returns the etherent	status
 *
 * PARAMETERS:
 * IN  : net_device*, stringset	,.uint8_t*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/
static void
SPEAR_ETHTOOL_get_strings (struct net_device *netdev, uint32_t stringset,
		   uint8_t * data)
{
	int	i;

	DEBUGFUNC ("inside get strings\n");
	switch (stringset)
	{
	case ETH_SS_TEST:
		memcpy (data,	*spear_gstrings_test,
				SPEAR_TEST_LEN * ETH_GSTRING_LEN);
		break;
	case ETH_SS_STATS:
		for (i = 0; i	< SPEAR_STATS_LEN; i++)
		{
			memcpy (data + i * ETH_GSTRING_LEN,
			spear_gstrings_stats[i].stat_string,
			ETH_GSTRING_LEN);
		}
		break;
	}
}

/*
 * NAME:  SPEAR_ETHTOOL_get_stats
 *
 * DESCRIPTION:
 *    called by	ethtool	-s returns the etherent	status
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_stats* ,uint64_t*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/
static void
SPEAR_ETHTOOL_get_stats	(struct	net_device *netdev,
			 struct	ethtool_stats *stats, uint64_t * data)
{
	struct eth_priv *adapter = netdev->priv;
	int	i;

	DEBUGFUNC ("inside get ethtool stats\n");

/*	spear_update_stats(adapter);*/
	for (i = 0; i <	SPEAR_STATS_LEN; i++)
	{
		char *p = (char *) adapter + spear_gstrings_stats[i].stat_offset;
		data[i] = (spear_gstrings_stats[i].sizeof_stat ==
			sizeof (uint64_t)) ? *(uint64_t *) p : *(uint32_t *) p;
	}
}

/*
 * NAME:  SPEAR_ETHTOOL_get_drv_info
 *
 * DESCRIPTION:
 *    Returns the driver info
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_drvinfo*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

static void
SPEAR_ETHTOOL_get_drv_info (struct net_device *netdev,
			struct ethtool_drvinfo *drvinfo)
{
	/* struct eth_priv *adapter = netdev->priv;*/

	DEBUGFUNC ("inside gdrv info\n");
	strncpy (drvinfo->driver, g_spear_driver_name, 6);
	strncpy (drvinfo->version, g_spear_driver_version, 21);
	strncpy (drvinfo->fw_version, "N/A", 4);
	strncpy (drvinfo->bus_info,	"N/A", 4);
	drvinfo->n_stats = SPEAR_STATS_LEN;
	drvinfo->testinfo_len = SPEAR_TEST_LEN;
	drvinfo->regdump_len = SPEAR_ETHTOOL_get_regs_len (netdev);
/*    drvinfo->eedump_len = spear_get_eeprom_len (netdev);*/
}

/*
 * NAME:  SPEAR_ETHTOOL_get_tx_chksum
 *
 * DESCRIPTION:
 *    Get the tx CSUM
 *
 * PARAMETERS:
 * IN  : net_device*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static uint32_t
SPEAR_ETHTOOL_get_tx_chksum (struct net_device *netdev)
{
	return (netdev->features & NETIF_F_ALL_CSUM)	!= 0;
}

/*
 * NAME:  SPEAR_ETHTOOL_set_tx_chksum
 *
 * DESCRIPTION:
 *    Set the tx CSUM
 *
 * PARAMETERS:
 * IN  : net_device*, data
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static int
SPEAR_ETHTOOL_set_tx_chksum (struct net_device *netdev,	uint32_t data)
{
	DEBUGFUNC ("inside set tx csum \n");
	if (data)
		netdev->features |= NETIF_F_IP_CSUM;
	else
		netdev->features &= ~NETIF_F_IP_CSUM;
	return 0;
}

/*
 * NAME:  SPEAR_ETHTOOL_diag_tst_cnt
 *
 * DESCRIPTION:
 *    returns the test len
 *
 * PARAMETERS:
 * IN  : net_device*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
static int
SPEAR_ETHTOOL_diag_tst_cnt (struct net_device *netdev)
{
	return SPEAR_TEST_LEN;
}

#ifdef NETIF_F_TSO
static u32
SPEAR_ETHTOOL_get_tso (struct net_device *dev)
{
	return (dev->features & NETIF_F_TSO) != 0;
}
static int
SPEAR_ETHTOOL_set_tso (struct net_device *dev, u32 sum)
{
	DEBUGFUNC ("inside set tso");
	if (sum)
		dev->features |= NETIF_F_TSO;
	else
		dev->features &= ~NETIF_F_TSO;
	return 0;
}


#endif	/* NETIF_F_TSO */
static struct ethtool_ops spear_ethtool_ops =
{
	.get_settings =	SPEAR_ETHTOOL_gset,
	.set_settings =	SPEAR_ETHTOOL_sset,
	.get_drvinfo = SPEAR_ETHTOOL_get_drv_info,

	.get_regs_len =	SPEAR_ETHTOOL_get_regs_len,
	.get_regs = SPEAR_ETHTOOL_get_regs,

	.nway_reset = SPEAR_ETHTOOL_nway_reset,

	.get_ringparam = SPEAR_ETHTOOL_get_ringparam,
	.get_pauseparam	= SPEAR_ETHTOOL_get_pauseparam,
/*	.set_pauseparam = spear_set_pauseparam,
	.get_rx_csum = emac_ethtool_get_rx_csum,*/
	.get_strings = SPEAR_ETHTOOL_get_strings,
	.get_stats_count = SPEAR_ETHTOOL_get_stats_cnt,
	.get_ethtool_stats = SPEAR_ETHTOOL_get_stats,
	.get_link = ethtool_op_get_link,
	.get_tx_csum = SPEAR_ETHTOOL_get_tx_chksum,
	.set_tx_csum = SPEAR_ETHTOOL_set_tx_chksum,
	.get_sg	= ethtool_op_get_sg,
	.self_test_count = SPEAR_ETHTOOL_diag_tst_cnt,
#ifdef NETIF_F_TSO
	.get_tso = SPEAR_ETHTOOL_get_tso,
	.set_tso = SPEAR_ETHTOOL_set_tso,
#endif
};

void
SPEAR_ETHTOOL_set_opts	(struct	net_device *netdev)
{
    SET_ETHTOOL_OPS(netdev, &spear_ethtool_ops);
}

#if 0
/*
 * NAME:  spear_set_pauseparam
 *
 * DESCRIPTION:
 *    Sets the autoneg,tx & rx setting
 *
 * PARAMETERS:
 * IN  : net_device*, ethtool_pauseparam*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

static int
spear_set_pauseparam (struct net_device	*netdev,
		      struct ethtool_pauseparam	*epause)
{
    struct eth_priv *adapter = netdev->priv;
    struct spear_hw *hw	= &adapter->hw;

    DEBUGFUNC ("inside set pause param \n");

	if (!netif_running (adapter->dev))
		return -EINVAL;
	adapter->hw.autoneg	= epause->autoneg;
	DEBUGOUT1 ("AutoNeg	%d \n",	adapter->fc_autoneg);

	if (epause->rx_pause &&	epause->tx_pause)
		hw->fc.type = spear_fc_full;
	else if (epause->rx_pause && !epause->tx_pause)
		hw->fc.type = spear_fc_rx_pause;
	else if (!epause->rx_pause && epause->tx_pause)
		hw->fc.type = spear_fc_tx_pause;
	else if (!epause->rx_pause && !epause->tx_pause)
		hw->fc.type = spear_fc_none;
	
	SPEAR_ETH_Down (adapter);
	SPEAR_ETH_Up (adapter);
	return 0;
}
#endif

EXPORT_SYMBOL(SPEAR_ETHTOOL_set_opts);
