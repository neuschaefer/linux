 /*
 * Spearhead Evaluation Board Genereic Phy routines
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/delay.h>

#include "spr_870_eth_phy.h"

#define printk(fmt...)
  
static inline int SPEAr_Phy_read(struct mii_phy *phy, u8 reg)
{
	return phy->mdio_read(phy->address, reg);
}

static inline void SPEAr_Phy_Write(struct mii_phy *phy, u8 reg, u16 val)
{
	phy->mdio_write(phy->address, reg, val);
}


/****************************************************************************/
/* NAME:  SPEAr_MII_Reset_Phy		                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : mii_phy info													    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

int SPEAr_MII_Reset_Phy(struct mii_phy *phy)
{
	u16 val;
	unsigned int i;

	val = SPEAr_Phy_read(phy, MII_BMCR);
	val &= ~BMCR_ISOLATE;
	val |= BMCR_RESET;
	SPEAr_Phy_Write(phy, MII_BMCR, val);
	udelay(300);

	for (i = 0; (SPEAr_Phy_read(phy, MII_BMCR) & BMCR_RESET); i++) 
	{
		if (i == 10000) 
		{
			printk("Eth phyreset :times out \n");			
			break;
		}
		udelay(5);
	}

	if(i !=10000)
	{
		mdelay(2);	//provide a delay of 2 msec in case the delay in the for loop was lees than 2 msec.
		return  0;;
	}
	else
		return -1;
}


/****************************************************************************/
/* NAME:  SPEAr_MII_Setup_Aneg	                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : mii_phy info													    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAr_MII_Setup_Aneg(struct mii_phy *phy, u32 advertise)
{
	u16 ctl;
	int adv;

	phy->autoneg = AUTONEG_ENABLE;
	phy->speed = SPEED_10;
	phy->duplex = DUPLEX_HALF;
	phy->pause = phy->asym_pause = 0;
	phy->advertising = advertise;

	/* Setup standard advertise */
	adv = SPEAr_Phy_read(phy, MII_ADVERTISE);
	if (adv < 0)
		return adv;
	adv &= ~(ADVERTISE_ALL | ADVERTISE_100BASE4 | ADVERTISE_PAUSE_CAP |
		 ADVERTISE_PAUSE_ASYM);
	if (advertise & ADVERTISED_10baseT_Half)
		adv |= ADVERTISE_10HALF;
	if (advertise & ADVERTISED_10baseT_Full)
		adv |= ADVERTISE_10FULL;
	if (advertise & ADVERTISED_100baseT_Half)
		adv |= ADVERTISE_100HALF;
	if (advertise & ADVERTISED_100baseT_Full)
		adv |= ADVERTISE_100FULL;
	if (advertise & ADVERTISED_Pause)
		adv |= ADVERTISE_PAUSE_CAP;
	if (advertise & ADVERTISED_Asym_Pause)
		adv |= ADVERTISE_PAUSE_ASYM;
	SPEAr_Phy_Write(phy, MII_ADVERTISE, adv);

	if (phy->features &
	    (SUPPORTED_1000baseT_Full | SUPPORTED_1000baseT_Half)) {
		adv = SPEAr_Phy_read(phy, MII_CTRL1000);
		if (adv < 0)
			return adv;
		adv &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);
		if (advertise & ADVERTISED_1000baseT_Full)
			adv |= ADVERTISE_1000FULL;
		if (advertise & ADVERTISED_1000baseT_Half)
			adv |= ADVERTISE_1000HALF;
		SPEAr_Phy_Write(phy, MII_CTRL1000, adv);
	}

	/* Start/Restart aneg */
	ctl = SPEAr_Phy_read(phy, MII_BMCR);
	ctl |= (BMCR_ANENABLE | BMCR_ANRESTART);
	SPEAr_Phy_Write(phy, MII_BMCR, ctl);

	return 0;
}

/****************************************************************************/
/* NAME:  SPEAr_MII_Setup_Forced                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : mii phy info													    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int SPEAr_MII_Setup_Forced(struct mii_phy *phy, int speed, int fd)
{
	int ctl=0;
	int i=0;

	phy->autoneg = AUTONEG_DISABLE;
	phy->speed = speed;
	phy->duplex = fd;
	phy->pause = phy->asym_pause = 0;

	ctl = SPEAr_Phy_read(phy, MII_BMCR);
	if (ctl < 0)
		return ctl;
	ctl &= ~(BMCR_FULLDPLX | BMCR_SPEED100 | BMCR_ANENABLE);

	/* First reset the PHY */
	SPEAr_Phy_Write(phy, MII_BMCR, ctl | BMCR_RESET);
	udelay(300);
	for (i = 0; (SPEAr_Phy_read(phy, MII_BMCR) & BMCR_RESET); i++) 
	{
		if (i == 10000) 
		{
			printk("Eth phyreset :times out \n");			
			break;
		}
		udelay(5);
	}
	if(i !=10000)
	{
		mdelay(2);	//provide a delay of 2 msec in case the delay in the for loop was lees than 2 msec.
	}

	/* Select speed & duplex */
	switch (speed) {
	case SPEED_10:
		break;
	case SPEED_100:
		ctl |= BMCR_SPEED100;
		break;
	case SPEED_1000:
		ctl |= BMCR_SPEED1000;
		break;
	default:
		return -EINVAL;
	}
	if (fd == DUPLEX_FULL)
		ctl |= BMCR_FULLDPLX;
	SPEAr_Phy_Write(phy, MII_BMCR, ctl);

	return 0;
}

/****************************************************************************/
/* NAME:  SPEAr_MII_Poll_Link		                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : mii phy info													    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAr_MII_Poll_Link(struct mii_phy *phy)
{
	int status;
	SPEAr_Phy_read(phy, MII_BMSR);
	status = SPEAr_Phy_read(phy, MII_BMSR);
	if ((status & BMSR_LSTATUS) == 0)
	{
		printk("In poll link inval stat\n");
		return 0;
	}

	if ((phy->autoneg == AUTONEG_ENABLE) && (!(status & BMSR_ANEGCOMPLETE)))
	{
		printk("In poll link val autoneg\n");
		return 0;
	}
//		printk("In poll link in val\n");
		return 1;
}	


/****************************************************************************/
/* NAME:  SPEAr_MII_Read_Link		                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : mii phy info													    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

//static int SPEAr_MII_Read_Link(struct mii_phy *phy)
int SPEAr_MII_Read_Link(struct mii_phy *phy)
{
    volatile int    vola,vola1;
	vola = 0;
	vola1 =0;
	if(phy->autoneg == AUTONEG_ENABLE)
	{
		vola  = SPEAr_Phy_read(phy, MII_LPA);
		if(vola <0)
			return vola;

		if (phy->features &
		    (SUPPORTED_1000baseT_Full | SUPPORTED_1000baseT_Half))
		{
			vola1 = SPEAr_Phy_read(phy, MII_STAT1000);
			if (vola1 <0)
			{
				return vola1;
			}
		}
		phy->speed = SPEED_10;
		phy->duplex = DUPLEX_HALF;
		phy->pause = phy->asym_pause = 0;
		
		if (vola1 & (LPA_1000FULL | LPA_1000HALF))
			{
				phy->speed = SPEED_1000;
				if (vola1 & LPA_1000FULL)
					phy->duplex = DUPLEX_FULL;
			}
		else if (vola & (LPA_100FULL | LPA_100HALF))
		{
			phy->speed = SPEED_100;
			if (vola & LPA_100FULL)
				phy->duplex = DUPLEX_FULL;
		}
		else if (vola & LPA_10FULL)
			phy->duplex = DUPLEX_FULL;

		if (phy->duplex == DUPLEX_FULL) 
		{
			phy->pause = vola & LPA_PAUSE_CAP ? 1 : 0;
			phy->asym_pause = vola & LPA_PAUSE_ASYM ? 1 : 0;
		}
	}
	else
	{
		vola = SPEAr_Phy_read(phy, MII_BMCR);
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
		else if (vola & BMCR_SPEED100)
			phy->speed = SPEED_100;
		else
			phy->speed = SPEED_10;

		phy->pause = phy->asym_pause = 0;
	}
	return 0;
}


/*******************************************************************
		ST-PHY
********************************************************************/
static struct mii_phy_ops st_phy_ops = {
	.setup_aneg		=	SPEAr_MII_Setup_Aneg,
	.setup_forced	=	SPEAr_MII_Setup_Forced,
	.poll_link		=	SPEAr_MII_Poll_Link,
	.read_link		=	SPEAr_MII_Read_Link
};

static struct mii_phy_def st_phy_def = {
	.phy_id			= 0x00001c04,
	.phy_id_mask	= 0x0000ffff,
	.name			= "ST-10/100 Ethernet",
	.ops			= &st_phy_ops
};

/*******************************************************************
			NATIONAL PHY
*******************************************************************/
static struct mii_phy_ops national_phy_ops = {
	.setup_aneg		= SPEAr_MII_Setup_Aneg,
	.setup_forced	= SPEAr_MII_Setup_Forced,
	.poll_link		= SPEAr_MII_Poll_Link,
	.read_link		= SPEAr_MII_Read_Link
};

static struct mii_phy_def national_phy_def = {
	.phy_id		= 0x20005C7A,
	.phy_id_mask= 0xffffffff,
	.name		= "National Giga Ethernet",
	.ops		= &national_phy_ops,
};


/*******************************************************************
			REALTEK PHY
*******************************************************************/
	static struct mii_phy_ops realtek_phy_ops = {
		.setup_aneg 	= SPEAr_MII_Setup_Aneg,
		.setup_forced	= SPEAr_MII_Setup_Forced,
		.poll_link		= SPEAr_MII_Poll_Link,
		.read_link		= SPEAr_MII_Read_Link
	};
	
	static struct mii_phy_def realtek_phy_def = {
		.phy_id 	= 0x001CC930, 
		.phy_id_mask    = 0xffffffff,
		.name		= "Realtek PHY switch",
		.ops		= &realtek_phy_ops,
	};


/***********************************************************************

***********************************************************************/
/* Generic implementation for most 10/100/1000 PHYs */
static struct mii_phy_ops generic_phy_ops = {
	.setup_aneg	= SPEAr_MII_Setup_Aneg,
	.setup_forced	= SPEAr_MII_Setup_Forced,
	.poll_link	= SPEAr_MII_Poll_Link,
	.read_link	= SPEAr_MII_Read_Link
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
	&realtek_phy_def,
	&genmii_phy_def,
	NULL
};


void rtl8368_debugPHY2Regs(struct mii_phy *phy)
{
	u16 regval;
	regval = phy->mdio_read(2, 0);
	printk("Phy2 Control Reg=0x%x\n",regval);
	regval = phy->mdio_read(2, 1);
	printk("Phy2 Status Reg=0x%x\n",regval);
	regval = phy->mdio_read(2, 4);
	printk("Phy2 Auto Neg Adver Reg=0x%x\n",regval);
}

s8 rtl8363_setAsicEthernetPHY2(struct mii_phy *phy)
{
	u16 regval;
		
	if (phy->duplex == DUPLEX_FULL)
	{
		regval =  phy->mdio_read(2, 0);
		regval |= (1<<8);
		phy->mdio_write (2,0,regval);//Duplex Mode
	}
	if (phy->duplex == DUPLEX_HALF)
	{
		regval =  phy->mdio_read(2, 0);
		regval &= (0<<8);
		phy->mdio_write (2,0,regval);//Duplex Mode
	}


	if (phy->speed == SPEED_1000) 
	{
		printk("PHY 2 speed is 1000\n");	
		regval = phy->mdio_read(2, 0);
		regval |= (1<<6) ;
		regval &= (0<<13);
		phy->mdio_write (2,0,regval);//1000 Mbps
	}

	if (phy->speed == SPEED_100)
	{
		printk("PHY 2 speed is 100\n");	
		regval = phy->mdio_read(2, 0);
		regval |= (1<<13);
		regval &= (0<<6);
		phy->mdio_write  (2,0,regval);//100 Mbps
	}
	
	if (phy->speed == SPEED_10)
	{
		printk("PHY 2 speed is 10\n");	
		regval = phy->mdio_read(2, 0);
		regval &= (0<<6);
		regval &= (0<<13);
		phy->mdio_write  (2,0,regval);//10 Mbps
	}
		/* Disbale Pause*/
		regval = phy->mdio_read(2, 4);
		regval |= (1<<10);
		phy->mdio_write  (2,4,regval);

	
	
	return 0;

}



int SPEAr_Phy_Feature_Probe(struct mii_phy *phy, u8 address)
{
	u16 bmsr = 0;
	phy->advertising = 0;
	phy->address = address;
	phy->pause = phy->asym_pause = 0;

	/* Determine PHY features if needed */
	bmsr = SPEAr_Phy_read(phy, MII_BMSR);
	
	if (bmsr & BMSR_ANEGCAPABLE)
		phy->features |= SUPPORTED_Autoneg;
		
	if (bmsr & BMSR_10HALF)
		phy->features |= SUPPORTED_10baseT_Half;
		
	if (bmsr & BMSR_10FULL)
		phy->features |= SUPPORTED_10baseT_Full;
	
	if (bmsr & BMSR_100HALF)
		phy->features |= SUPPORTED_100baseT_Half;
	
	if (bmsr & BMSR_100FULL)
		phy->features |= SUPPORTED_100baseT_Full;
		
	if (bmsr & BMSR_ESTATEN) 
	{
		u16 esr = SPEAr_Phy_read(phy, MII_ESTATUS);
		if (esr & ESTATUS_1000_TFULL)
			phy->features |= SUPPORTED_1000baseT_Full;
		if (esr & ESTATUS_1000_THALF)
			phy->features |= SUPPORTED_1000baseT_Half;
	}
	
	phy->features |= SUPPORTED_MII;

	/* Setup default advertising */
	phy->advertising = phy->features;

	return 0;

}

/****************************************************************************/
/* NAME:  SPEAr_MII_Read_Link		                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : mii phy info													    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

int SPEAr_Phy_Probe(struct mii_phy *phy, u8 address)
{
	struct mii_phy_def *def;
	int i;
	u32 id;

	phy->autoneg = AUTONEG_DISABLE;
	phy->advertising = 0;
	phy->address = address;
	phy->speed = SPEED_10;
	phy->duplex = DUPLEX_HALF;
	phy->pause = phy->asym_pause = 0;

	/* Take PHY out of isolate mode and reset it. */
	if (SPEAr_MII_Reset_Phy(phy))
		return -ENODEV;

	/* Read ID and find matching entry */
	id = (SPEAr_Phy_read(phy, MII_PHYSID1) << 16) | SPEAr_Phy_read(phy, MII_PHYSID2);
	for (i = 0; (def = mii_phy_table[i]) != NULL; i++)
	{
		if ((id & def->phy_id_mask) == def->phy_id)
			break;
	}

	/* Should never be NULL (we have a generic entry), but... */
	if (!def)
		return -ENODEV;

	phy->def = def;

	/* Determine PHY features if needed */
	phy->features = def->features;

	if (!phy->features) {
		u16 bmsr = SPEAr_Phy_read(phy, MII_BMSR);
		
		if (bmsr & BMSR_ANEGCAPABLE)
			phy->features |= SUPPORTED_Autoneg;
		
		if (bmsr & BMSR_10HALF)
			phy->features |= SUPPORTED_10baseT_Half;
		
		if (bmsr & BMSR_10FULL)
			phy->features |= SUPPORTED_10baseT_Full;
		
		if (bmsr & BMSR_100HALF)
			phy->features |= SUPPORTED_100baseT_Half;
		
		if (bmsr & BMSR_100FULL)
			phy->features |= SUPPORTED_100baseT_Full;
		
		if (bmsr & BMSR_ESTATEN) {
			u16 esr = SPEAr_Phy_read(phy, MII_ESTATUS);
			if (esr & ESTATUS_1000_TFULL)
				phy->features |= SUPPORTED_1000baseT_Full;
			if (esr & ESTATUS_1000_THALF)
				phy->features |= SUPPORTED_1000baseT_Half;
		}
		phy->features |= SUPPORTED_MII;
	}

	/* Setup default advertising */
	phy->advertising = phy->features;

#ifdef PHY_DBG
	Read_PHY_reg(phy);
#endif
	return 0;
}

#ifdef PHY_DBG
/*----------------------------- PHY Registers Reading : To check the MII interface------------------- */
void Read_PHY_reg(struct mii_phy *phy)
{
    unsigned short int read ;

    read = SPEAr_Phy_read(phy, MII_BMCR);
    printk("\n GEN_ctl=%X",read);

    read = SPEAr_Phy_read(phy, MII_BMSR);
    printk("\n GEN_sts=%X ",read) ;

    read = SPEAr_Phy_read(phy, MII_PHYSID1);
    printk("\n GEN_id_hi=%X",read);

    read = SPEAr_Phy_read(phy, MII_PHYSID2);
    printk("\n GEN_id_lo=%X",read);

    read = SPEAr_Phy_read(phy, MII_ADVERTISE);
    printk("\n AN_adv=%X",read);

    read = SPEAr_Phy_read(phy, MII_LPA);
    printk("\n AN_lpa=%X",read);

    read = SPEAr_Phy_read(phy, MII_EXPANSION);
    printk("\n AN_exp=%X",read);

    read = SPEAr_Phy_read(phy, MII_CTRL1000);
    printk("\n PHY_1000_CtlReg=%X",read);

    read = SPEAr_Phy_read(phy, MII_STAT1000);
    printk("\n GEN_sts=%X ",read);

}
#endif

