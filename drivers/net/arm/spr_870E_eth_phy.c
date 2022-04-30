 /*
 * Spearhead Evaluation	Board Genereic Phy routines
 *
 * This	program	is free	software; you can redistribute it and/or
 * modify it under the terms of	the	GNU	General	Public License as
 * published by	the	Free Software Foundation; either version 2 of
 * the License,	or (at your	option)	any	later version.
 *
 * This	program	is distributed in the hope that	it will	be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A	PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received	a copy of the GNU General Public License
 * along with this program;	if not,	write to the Free Software
 * Foundation, Inc., 59	Temple Place, Suite	330, Boston,
 * MA 02111-1307 USA
 * author: deepak.sikri@st.com
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/delay.h>
#include "spr_870E_eth_phy.h"

#define printk(fmt...) 
static inline int SPEAR_PHY_read(struct	mii_phy	*phy, u8 reg)
{
	return phy->mdio_read(phy->address,	reg);
}

static inline void SPEAR_PHY_Write(struct mii_phy *phy,	u8 reg,	u16	val)
{
	phy->mdio_write(phy->address, reg, val);
}


/*
 * NAME:  SPEAR_MII_reset_phy
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * IN  : mii_phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/

static int SPEAR_MII_reset_phy(struct mii_phy *phy)
{
	u16	val;
	u32	i;

	val = SPEAR_PHY_read(phy, MII_BMCR);
	val &= ~BMCR_ISOLATE;
	val |= BMCR_RESET;
	SPEAR_PHY_Write(phy, MII_BMCR, val);
	udelay(300);

	for(i = 0;(SPEAR_PHY_read(phy, MII_BMCR) & BMCR_RESET); i++) 
	{
		if (i == 10000)	
		{
			printk("Eth	phyreset :times	out	\n");			
			break;
		}
		udelay(5);
	}

	if(i !=10000)
	{
		mdelay(2);	//provide a	delay of 2 msec	in case	the	delay in the for loop was lees than	2 msec.
		return	0;;
	}
	else
		return -1;
}


/*
 * NAME:  SPEAR_MII_setup_aneg
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * IN  : mii_phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/
static int SPEAR_MII_setup_aneg(struct mii_phy *phy, u32 advertise)
{
	short int  adv;
	u16 tmp;	  

	phy->autoneg = AUTONEG_ENABLE;
	phy->speed = SPEED_10;
	phy->duplex	= DUPLEX_HALF;
	phy->pause = phy->asym_pause = 0;
	phy->advertising = advertise;

	/* Setup standard advertise	*/
	adv = SPEAR_PHY_read(phy, MII_ADVERTISE);
	if (adv	< 0)
		return adv;
	adv &= ~(ADVERTISE_ALL | ADVERTISE_100BASE4 | ADVERTISE_PAUSE_CAP |
		 ADVERTISE_PAUSE_ASYM);
	if (advertise &	ADVERTISED_10baseT_Half)
		adv |= ADVERTISE_10HALF;
	if (advertise &	ADVERTISED_10baseT_Full)
		adv |= ADVERTISE_10FULL;
	if (advertise &	ADVERTISED_100baseT_Half)
		adv |= ADVERTISE_100HALF;
	if (advertise &	ADVERTISED_100baseT_Full)
		adv |= ADVERTISE_100FULL;
	if (advertise &	ADVERTISED_Pause)
		adv |= ADVERTISE_PAUSE_CAP;
	if (advertise &	ADVERTISED_Asym_Pause)
		adv |= ADVERTISE_PAUSE_ASYM;
	SPEAR_PHY_Write(phy, MII_ADVERTISE,(u16)adv);

	if (phy->features &
		(SUPPORTED_1000baseT_Full | SUPPORTED_1000baseT_Half)) {
		adv = SPEAR_PHY_read(phy, MII_CTRL1000);
		if (adv	< 0)
			return adv;
		adv &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);
		if (advertise &	ADVERTISED_1000baseT_Full)
			adv |= ADVERTISE_1000FULL;
		if (advertise &	ADVERTISED_1000baseT_Half)
			adv |= ADVERTISE_1000HALF;
		SPEAR_PHY_Write(phy, MII_CTRL1000, (u16)adv);
	}

	/* Start/Restart aneg */
	tmp= SPEAR_PHY_read(phy, MII_BMCR) & (~BMCR_ANENABLE);
	SPEAR_PHY_Write(phy, MII_BMCR,	tmp ); 
	SPEAR_PHY_Write(phy, MII_BMCR,	BMCR_ANENABLE|BMCR_ANRESTART );
	return 0;
}

/*
 * NAME:  SPEAR_MII_setup_forced
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * IN  : mii phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/

static int SPEAR_MII_setup_forced(struct mii_phy *phy, int speed, int fd)
{
	short int ctl=0;
	int	i=0;

	phy->autoneg = AUTONEG_DISABLE;
	phy->speed = speed;
	phy->duplex = fd;
	phy->pause = phy->asym_pause = 0;

	ctl = SPEAR_PHY_read(phy, MII_BMCR);
	if (ctl	< 0)
		return ctl;
	ctl &= ~(BMCR_FULLDPLX | BMCR_SPEED100 | BMCR_ANENABLE | BMCR_ANRESTART);
	ctl |= BMCR_RESET;
	/* First reset the PHY */
	SPEAR_PHY_Write(phy, MII_BMCR, (u16)ctl);
	udelay(300);
	for (i = 0;(SPEAR_PHY_read(phy, MII_BMCR) & BMCR_RESET); i++) 
	{
		if (i == 10000)	
		{
			printk("Eth	phyreset :times	out	\n");			
			break;
		}
		udelay(5);
	}
	if(i !=10000)
	{
		mdelay(2);	//provide a	delay of 2 msec	in case	the	delay in the for loop was lees than	2 msec.
	}
	ctl = 0;
	/* Select speed	& duplex */
	switch (speed) {
	case SPEED_10:
		break;
	case SPEED_100:
		ctl	|= BMCR_SPEED100;
		break;
	case SPEED_1000:
		ctl	|= BMCR_SPEED1000;
		break;
	default:
		return -EINVAL;
	}
	if (fd == DUPLEX_FULL)
		ctl	|= BMCR_FULLDPLX;
	SPEAR_PHY_Write(phy, MII_BMCR, (u16)ctl);

	return 0;
}

/*
 * NAME:  SPEAR_MII_poll_link
 *
 * DESCRIPTION:
 * Check the link status for generic phy
 * PARAMETERS:
 * IN  : mii phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/
static int SPEAR_MII_poll_link(struct mii_phy *phy)
{
	int	status;
	SPEAR_PHY_read(phy, MII_BMSR);
	status = SPEAR_PHY_read(phy, MII_BMSR);
	if ((status & BMSR_LSTATUS)	== 0)
	{
		printk("In poll	link inval stat\n");
		return 0;
	}

	if ((phy->autoneg == AUTONEG_ENABLE) &&	(!(status & BMSR_ANEGCOMPLETE)))
	{
		return 0;
	}
		return 1;
}	
/*
 * NAME:  SPEAr_MII_national_poll_link
 *
 * DESCRIPTION:
 * Check the link status for national phy
 * PARAMETERS:
 * IN  : mii phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/

static int SPEAr_MII_national_poll_link(struct mii_phy *phy)
{
	int	status;
	SPEAR_PHY_read(phy,	MII_BMSR);
	status = SPEAR_PHY_read(phy, MII_BMSR);

	if ((status & BMSR_LSTATUS)	== 0)
	{
		return 0;
	}

	if ((phy->autoneg == AUTONEG_ENABLE) &&	(!(status & BMSR_ANEGCOMPLETE)))
	{
		return 0;
	}
	else if(!(phy->autoneg == AUTONEG_ENABLE))
	{
		status = SPEAR_PHY_read(phy, 0x11);
		if(!(status	& 0x0004))
		{
			return 0;
		}
	}
		return 1;
}	


/*
 * NAME:  SPEAr_MII_Read_Link
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * IN  : mii phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/

static int SPEAr_MII_Read_Link(struct mii_phy *phy)
{
	volatile int	vola,vola1;
	vola = 0;
	vola1 =0;
	if(phy->autoneg	== AUTONEG_ENABLE)
	{
		vola  =	SPEAR_PHY_read(phy,	MII_LPA);
		if(vola	<0)
			return vola;

		if (phy->features &
			(SUPPORTED_1000baseT_Full | SUPPORTED_1000baseT_Half))
		{
			vola1 =	SPEAR_PHY_read(phy, MII_STAT1000);
			if (vola1 <0)
			{
				return vola1;
			}
		}
		phy->speed = SPEED_10;
		phy->duplex = DUPLEX_HALF;
		phy->pause = phy->asym_pause = 0;
		
		if (vola1 & (LPA_1000FULL |	LPA_1000HALF))
			{
				phy->speed = SPEED_1000;
				if (vola1 & LPA_1000FULL)
					phy->duplex = DUPLEX_FULL;
			}
		else if	(vola &	(LPA_100FULL | LPA_100HALF))
		{
			phy->speed = SPEED_100;
			if (vola & LPA_100FULL)
				phy->duplex = DUPLEX_FULL;
		}
		else if	(vola &	LPA_10FULL)
			phy->duplex = DUPLEX_FULL;

		if (phy->duplex	== DUPLEX_FULL)	
		{
			phy->pause = vola & LPA_PAUSE_CAP ? 1 :	0;
			phy->asym_pause	= vola & LPA_PAUSE_ASYM	? 1 : 0;
		}
	}
	else
	{
		vola = SPEAR_PHY_read(phy, MII_BMCR);
		if (vola <0)
		{
			return vola;
		}
		if (vola & BMCR_FULLDPLX)
			phy->duplex = DUPLEX_FULL;
		else
			phy->duplex = DUPLEX_HALF;
		if (vola & BMCR_SPEED1000)
			phy->speed = SPEED_1000;
		else if	(vola &	BMCR_SPEED100)
			phy->speed = SPEED_100;
		else
			phy->speed = SPEED_10;

		phy->pause = phy->asym_pause = 0;
	}
	return 0;
}


/*
 *		ST-PHY
*/
static struct mii_phy_ops st_phy_ops = {
	.setup_aneg		=	SPEAR_MII_setup_aneg,
	.setup_forced		=	SPEAR_MII_setup_forced,
	.poll_link		=	SPEAR_MII_poll_link,
	.read_link		=	SPEAr_MII_Read_Link
};

static struct mii_phy_def st_phy_def = {
	.phy_id			= 0x00001c04,
	.phy_id_mask		= 0x0000ffff,
	.name			= "ST-10/100 Ethernet",
	.ops			= &st_phy_ops
};

/*
 *			NATIONAL PHY
*/
static struct mii_phy_ops national_phy_ops = {
	.setup_aneg		= SPEAR_MII_setup_aneg,
	.setup_forced		= SPEAR_MII_setup_forced,
	.poll_link		= SPEAr_MII_national_poll_link,
	.read_link		= SPEAr_MII_Read_Link
};

static struct mii_phy_def national_phy_def = {
	.phy_id		= 0x20005C7A,
	.phy_id_mask	= 0xffffffff,
	.name		= "National Giga Ethernet",
	.ops		= &national_phy_ops,
};

/*
 *	GENERIC	PHY
*/
/* Generic implementation for most 10/100/1000 PHYs	*/
static struct mii_phy_ops generic_phy_ops =	{
	.setup_aneg		= SPEAR_MII_setup_aneg,
	.setup_forced		= SPEAR_MII_setup_forced,
	.poll_link		= SPEAR_MII_poll_link,
	.read_link		= SPEAr_MII_Read_Link
};

static struct mii_phy_def genmii_phy_def = {
	.phy_id		= 0x00000000,
	.phy_id_mask	= 0x00000000,
	.name		= "Generic MII",
	.ops		= &generic_phy_ops
};


static struct mii_phy_def *mii_phy_table[] = {
	&st_phy_def,
	&national_phy_def,
	&genmii_phy_def,
	NULL
};

/*
 * NAME:  SPEAR_MII_phy_probe
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * IN  : mii phy info
 *
 * OUT :
 *		  None
 *
 * RETURN:
 *		  int
 *
 * REENTRANCY: NA
*/

int	SPEAR_MII_phy_probe(struct mii_phy *phy, u8	address)
{
	struct mii_phy_def *def;
	int	i;
	u32	id;

	phy->autoneg = AUTONEG_DISABLE;
	phy->advertising = 0;
	phy->address = address;
	phy->speed = SPEED_10;
	phy->duplex = DUPLEX_HALF;
	phy->pause = phy->asym_pause = 0;

	/* Take	PHY	out	of isolate mode	and	reset it. */
	if (SPEAR_MII_reset_phy(phy))
		return -ENODEV;

	/* Read	ID and find	matching entry */
	id = (SPEAR_PHY_read(phy, MII_PHYSID1) << 16) |	SPEAR_PHY_read(phy,	MII_PHYSID2);
	printk(" id is %x \n",id);
	for(i = 0;(def = mii_phy_table[i]) != NULL; i++)
		if ((id	& def->phy_id_mask) == def->phy_id)
		{
			printk(" phy is	%s\n",def->name);
			break;
		}
	/* Should never	be NULL	(we	have a generic entry), but... */
	if (!def)
		return -ENODEV;

	phy->def = def;

	/* Determine PHY features if needed	*/
	phy->features =	def->features;

	if (!phy->features)	
	{
		u16	bmsr = SPEAR_PHY_read(phy, MII_BMSR);
		
		if (bmsr & BMSR_ANEGCAPABLE)
		{
			printk("setting	phy	features \n");
			phy->features |= SUPPORTED_Autoneg;
		}
		if (bmsr & BMSR_10HALF)
		{
			printk("setting	phy	features -1\n");
			phy->features |= SUPPORTED_10baseT_Half;
		}
		if (bmsr & BMSR_10FULL)
			phy->features |= SUPPORTED_10baseT_Full;
		
		if (bmsr & BMSR_100HALF)
			phy->features |= SUPPORTED_100baseT_Half;
		
		if (bmsr & BMSR_100FULL)
			phy->features |= SUPPORTED_100baseT_Full;
		
		if (bmsr & BMSR_ESTATEN) {
			u16	esr	= SPEAR_PHY_read(phy, MII_ESTATUS);
			if (esr	& ESTATUS_1000_TFULL)
				phy->features |= SUPPORTED_1000baseT_Full;
			if (esr	& ESTATUS_1000_THALF)
				phy->features |= SUPPORTED_1000baseT_Half;
		}
		phy->features |= SUPPORTED_MII;
	}

	/* Setup default advertising */
	phy->advertising = phy->features;

	return 0;
}

#ifdef PHY_DBG
/*-----------------------------	PHY	Registers Reading :	To check the MII interface------------------- */
static void	Read_PHY_reg(u8	Phy_nm)
{
	unsigned short int read	;

	read = SPEAR_PHY_read(phy, MII_BMCR);
	printk("\n GEN_ctl=%X",read);

	read = SPEAR_PHY_read(phy, MII_BMSR);
	printk("\n GEN_sts=%X ",read) ;

	read = SPEAR_PHY_read(phy, MII_PHYSID1);
	printk("\n GEN_id_hi=%X",read);

	read = SPEAR_PHY_read(phy, MII_PHYSID2);
	printk("\n GEN_id_lo=%X",read);

	read = SPEAR_PHY_read(phy, MII_ADVERTISE);
	printk("\n AN_adv=%X",read);

	read = SPEAR_PHY_read(phy, MII_LPA);
	printk("\n AN_lpa=%X",read);

	read = SPEAR_PHY_read(phy, MII_EXPANSION);
	printk("\n AN_exp=%X",read);

	read = SPEAR_PHY_read(phy, MII_CTRL1000);
	printk("\n PHY_1000_CtlReg=%X",read);

	read = SPEAR_PHY_read(phy, MII_STAT1000);
	printk("\n GEN_sts=%X ",read);
}
#endif

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(SPEAR_MII_phy_probe);
