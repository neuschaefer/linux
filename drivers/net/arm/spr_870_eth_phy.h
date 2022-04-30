 /*
 * Spearhead Evaluation Board specific info
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 * author: Bhupesh Sharma <bhupesh.sharma@st.com>
 */

#ifndef __SPR_870_ETH_PHY_HEADER_
#define __SPR_870_ETH_PHY_HEADER_

struct mii_phy;

/* Operations supported by any kind of PHY */
struct mii_phy_ops {
	int (*init) (struct mii_phy * phy);
	int (*suspend) (struct mii_phy * phy, int wol_options);
	int (*setup_aneg) (struct mii_phy * phy, u32 advertise);
	int (*setup_forced) (struct mii_phy * phy, int speed, int fd);
	int (*poll_link) (struct mii_phy * phy);
	int (*read_link) (struct mii_phy * phy);
};

/* Structure used to statically define an mii/gii based PHY */
struct mii_phy_def {
	u32 phy_id;			/* Concatenated ID1 << 16 | ID2 */
	u32 phy_id_mask;	/* Significant bits */
	u32 features;		/* Ethtool SUPPORTED_* defines or 
						0 for autodetect */
	int magic_aneg;		/* Autoneg does all speed test for us */
	const char *name;
	const struct mii_phy_ops *ops;
};

/* An instance of a PHY, partially borrowed from mii_if_info */
struct mii_phy {
	struct mii_phy_def *def;
	u32 advertising;		/* Ethtool ADVERTISED_* defines */
	u32 features;			/* Copied from mii_phy_def.features 
								or determined automaticaly */
	u8 address;			/* PHY address */
	int mode;				/* PHY mode */

	/* 1: autoneg enabled, 0: disabled */
	int autoneg;

	/* forced speed & duplex (no autoneg)
	 * partner speed & duplex & pause (autoneg)
	 */
	int speed;
	int duplex;
	int pause;
	int asym_pause;

	/* Provided by host chip */
	struct net_device *dev;
	u16 (*mdio_read) (u8 addr, u8 reg);
	void (*mdio_write) (u8 addr, u8 reg,
			    u16 val);
};

/* Pass in a struct mii_phy with dev, mdio_read and mdio_write
 * filled, the remaining fields will be filled on return
 */
int SPEAr_Phy_Probe(struct mii_phy *phy, u8 address);
int SPEAr_MII_Reset_Phy(struct mii_phy *phy);
int SPEAr_Phy_Feature_Probe(struct mii_phy *phy, u8 address);
s8 rtl8363_setAsicEthernetPHY2(struct mii_phy *phy);
void rtl8368_debugPHY2Regs(struct mii_phy *phy);

#ifdef PHY_DBG  
void Read_PHY_reg(u8 phy);
#endif

#endif	/* __SPR_870_ETH_PHY_HEADER_ */
/* end of file - spr_870_eth_phy.h */
