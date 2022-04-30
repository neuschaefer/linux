    /*
    * Copyright c                  Realtek Semiconductor Corporation, 2007 
    * All rights reserved.
    * 
    * Program : RTL8363 switch low-level API
    * Abstract : 
    * Author : Robin Zheng-bei Xing                
    *  $Id: rtl8363_asicdrv.c,v 1.53 2008-05-07 11:41:41 robin_xing Exp $
    */

/*	@doc RTL8363_ASICDRV_API

	@module Rtl8363_AsicDrv.c - RTL8213M/RTL8203M/RTL8363H/RTL8303H/RTL8363S Switch asic driver API documentation	|
	This document explains API serface of the asic. 
	@normal robin_xing <date>

	Copyright <cp>2008 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.

 	@head3 List of Symbols |
 	Here is a list of all functions and variables in this module.

 	@index | RTL8363_ASICDRIVER_API
*/
    

#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/mii.h>
#include <linux/crc32.h>
#include <asm/irq.h>
#include <asm/arch/hardware.h>
#include <asm/arch/memory.h>
#include <linux/dma-mapping.h>
#include <linux/types.h>
#include <linux/delay.h>

#include "rtl8363_asicdrv.h"
#include "spr_870_eth_syn.h"

/*
@func int8 | rtl8363_setAsicSoftReset | Set Asic software reset
@parm uint8 | port | port number (0~2).
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure.
@comm
*/
 
s8 rtl8363_setAsicSoftReset(void)
{
 
    rtl8363_setAsicRegBit(0, 16, 15, MACPAG, 0, 1);
    mdelay(60);	// 60ms wait after reset.
    return SUCCESS;
}

void mac2index(u8 *macAddr, u32 *pIndex)
{
    u8 idx;
    u8 i;
    
    idx = (macAddr[0] ^  macAddr[1]^macAddr[2]^macAddr[3]^macAddr[4]^macAddr[5]);
    *pIndex = 0 ;
    for ( i = 0; i < 8; i ++)
    {
        *pIndex |= (idx & 0x1) ? (1 << (7 - i)) :0;
        idx >>= 1;
        if (!idx)
            break;
    }
        
}

void ip2index(u32 sip, u32 dip, u32 *pIndex)
{
    u8 idx;
    u8 ip[4];

    ip[0] = (u8)((dip & 0x00FF0000) >> 16);
    ip[1] = (u8)((dip & 0xFF00) >> 8 );
    ip[2] = (u8)(dip & 0xFF);
    idx = ip[0] ^ ip[1] ^ ip[2];
    ip[0] = (u8)((sip & 0xFF000000) >> 24);
    ip[1] = (u8)((sip & 0x00FF0000) >> 16);
    ip[2] = (u8)((sip & 0xFF00) >> 8);
    ip[3] = (u8)(sip & 0xFF);
    idx ^= ip[0]^ip[1]^ip[2]^ip[3];
    *pIndex = idx;

}

/*
@func s8 | rtl8363_setAsicReg | Set Asic registers
@parm u8 | phyad | PHY address (0~2).
@parm u8 | regad |  Register address(0~31)
@parm u8 | pagtype | Register Page Type
@parm u8 | npage | Register Page Number
@parm u16 | value | Write value to register
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
pagtype has four Macros to be chosen, 
MACPAG represents MAC page;
UTPPAG  represents PHY page;
SERLPAG represents Serdes low page;
SERHPAG represents Serdes high page;
*/

s8 rtl8363_setAsicReg(u8 phyad, u8 regad, u8 pagtype,  u8 npage, u16 value)
{
    u32 regval;
    
    if ( (phyad > 2) || (regad > 31) || (pagtype > SERHPAG) || (npage > 15) )
        return FAILED;
    regval = 0;
    
     /*regad is 31*/
		if (regad == 31)
		{
            ETH_RTL_Mdio_write(phyad, regad, value & 0xFFFF);
			return SUCCESS;
		}
			
            /*Page Select*/			
            if (phyad == 0 || phyad == 1)
            {
               regval = (pagtype << 4) |npage;
               ETH_RTL_Mdio_write(phyad, 31, regval);
            }
            else if ((phyad == 2) && (pagtype == MACPAG) && (npage < 8) )
            {
               regval=ETH_RTL_Mdio_Read(phyad, 31);
               regval &= 0xFFF8;
               regval |= npage; 
               ETH_RTL_Mdio_write(phyad, 31, regval);
            }
            else 
            {
                return FAILED;
            }
        
            /*Write value to the register*/
            ETH_RTL_Mdio_write(phyad, regad, value & 0xFFFF);

    return SUCCESS;
}


/*
@func s8 | rtl8363_getAsicReg | Set Asic registers
@parm u8 | phyad | PHY address (0~2).
@parm u8 | regad |  Register address(0~31)
@parm u8 | pagtype | Register Page Type
@parm u8 | npage | Register Page Number
@parm u16* | pvalue |  Register value to register
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
pagtype has four Macros to be chosen, 
MACPAG represents MAC page;
UTPPAG  represents PHY page;
SERLPAG represents Serdes low page;
SERHPAG represents Serdes high page;
*/

s8 rtl8363_getAsicReg(u8 phyad, u8 regad, u8 pagtype,  u8 npage, u16 *pvalue)
{
    u32 regval;
    
    regval = 0;
    if ( (phyad > 2) || (regad > 31) || (pagtype > SERHPAG) ||
        (npage > 15) || (pvalue == NULL))
        return FAILED;
    
        /*regad is 31*/		
        if (regad == 31)
        {
             regval = ETH_RTL_Mdio_Read(phyad, regad);
	      return SUCCESS;
	 }
        
	  /*Page Select*/		
        if (phyad == 0 || phyad == 1)
        {
            regval = (pagtype << 4) |npage;
            ETH_RTL_Mdio_write(phyad, 31, regval);
        }
        else if ((phyad == 2) && (pagtype == MACPAG) && (npage < 8) )
        {
            regval=ETH_RTL_Mdio_Read(phyad, 31);
            regval &= 0xFFF8;
            regval |= npage; 
            ETH_RTL_Mdio_write(phyad, 31, regval);
        }
        else 
        {
            return FAILED;
        }
        
        /*Read regiter valuer*/
        regval = ETH_RTL_Mdio_Read(phyad, regad);
	*pvalue = (u16)regval;
        return SUCCESS;
    
}

/*
@func s8 | rtl8363_setAsicRegBit | Set Asic register bit value
@parm u8 | phyad | PHY address (0~2).
@parm u8 | regad |  Register address(0~31)
@parm u8 | bitpos | Register bit position(0~15)
@parm u8 | pagtype | Register Page Type
@parm u8 | npage | Register Page Number
@parm u16 | value | Write value to register
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
pagtype has four Macros to be chosen, 
MACPAG represents MAC page;
UTPPAG  represents PHY page;
SERLPAG represents Serdes low page;
SERHPAG represents Serdes high page;
*/
s8 rtl8363_setAsicRegBit(u8 phyad, u8 regad, u8 bitpos, u8 pagtype, u8 npage,  u16 value) 
{
	u16 rdata;

	if (rtl8363_getAsicReg(phyad, regad,  pagtype, npage, &rdata) != SUCCESS)
		return FAILED;
	if (value) 
		rtl8363_setAsicReg(phyad, regad, pagtype, npage, rdata | (1 << bitpos));
	else
		rtl8363_setAsicReg(phyad, regad, pagtype, npage, rdata & (~(1 << bitpos)));
	return SUCCESS;
}

/*
@func s8 | rtl8363_getAsicRegBit | Set Asic register bit value
@parm u8 | phyad | PHY address (0~2).
@parm u8 | regad |  Register address(0~31)
@parm u8 | pagtype | Register Page Type
@parm u8 | npage | Register Page Number
@parm u16* | pvalue |  Register value to register
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
pagtype has four Macros to be chosen, 
MACPAG represents MAC page;
UTPPAG  represents PHY page;
SERLPAG represents Serdes low page;
SERHPAG represents Serdes high page;
*/

s8 rtl8363_getAsicRegBit(u8 phyad, u8 regad, u8 bitpos, u8 pagtype, u8 npage,  u16 * pvalue) 
{
	u16 rdata;

	if (rtl8363_getAsicReg(phyad, regad,  pagtype, npage, &rdata) != SUCCESS)
		return FAILED;    
	if (rdata & (1 << bitpos))
		*pvalue =1;
	else 
		*pvalue =0;
		
	return SUCCESS;
}




/*
@func s8 | rtl8363_setAsicEthernetPHY | Set ethernet PHY registers for desired ability.
@parm u8 | phy | PHY number (0~1).
@parm phyCfg_t | cfg | phy configuration
@struct phyCfg_t | This structure describe PHY parameter
@field u8 | AutoNegotiation | AutoNegotiation ability
@field u8 | Speed | force speed, could be PHY_10M, PHY_100M, PHY_1000M
@field u8 | Fullduplex | full or half duplex
@field u8 | Cap_10Half | 10BASE-TX half duplex capable
@field u8 | Cap_10Full  | 10BASE-TX full duplex capable
@field u8 | Cap_100Half | 100BASE-TX half duplex capable
@field u8 | Cap_100Full | 100BASE-TX full duplex capable
@field u8 | FC | flow control capability
@field u8 | AsyFC | asymmetric flow control capability
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
	If Full_1000 bit is set to 1, the AutoNegotiation,Full_100 and Full_10 will be automatic set to 1. While both AutoNegotiation and Full_1000 are set to
	0, the PHY speed and duplex selection will be set as following 100F > 100H > 10F > 10H priority sequence.
*/
s8 rtl8363_setAsicEthernetPHY(u8 phy, phyCfg_t cfg)
{
	u16 phyData;
	
	u16 phyEnMsk0;
	u16 phyEnMsk4;
	u16 phyEnMsk9;
	
	if(phy > 1)
		return FAILED;

	phyEnMsk0 = 0;
	phyEnMsk4 = 0;
	phyEnMsk9 = 0;


      /*PHY speed, [0.6 0.13]:00= 10Mpbs, 01= 100Mpbs,10= 1000Mpbs,11=Reserved */
      if (cfg.Speed == PHY_10M)
      {
	    phyEnMsk0 = phyEnMsk0 & (~(1<<6));
	    phyEnMsk0 = phyEnMsk0 & (~(1<<13));

      }
      else if (cfg.Speed == PHY_100M)
      {
	    phyEnMsk0 = phyEnMsk0 & (~(1<<6));
	    phyEnMsk0 = phyEnMsk0 | (1<<13);
      }
      else if (cfg.Speed == PHY_1000M)
      {
	    phyEnMsk0 = phyEnMsk0 | (1<<6);
	    phyEnMsk0 = phyEnMsk0 & (~(1<<13));
      }

      /*duplex*/
      if (cfg.Fullduplex)
      {
	    /*Full duplex mode in reg 0.8*/
	    phyEnMsk0 = phyEnMsk0 | (1<<8);           
      }

      /*Advertisement Ability*/
	if(cfg.Cap_10Half)
	{
		/*10BASE-TX half duplex capable in reg 4.5*/
		phyEnMsk4 = phyEnMsk4 | (1<<5);

	}

	if(cfg.Cap_10Full)
	{
		/*10BASE-TX full duplex capable in reg 4.6*/
		phyEnMsk4 = phyEnMsk4 | (1<<6);		
	}

	if(cfg.Cap_100Half)
	{
		/*100BASE-TX half duplex capable in reg 4.7*/
		phyEnMsk4 = phyEnMsk4 | (1<<7);
	
	}

	if(cfg.Cap_100Full)
	{
		/*100BASE-TX full duplex capable in reg 4.8*/
		phyEnMsk4 = phyEnMsk4 | (1<<8);
	}
		
	if(cfg.Cap_1000Full)
	{
		/*1000 BASE-T FULL duplex capable setting in reg 9.9*/
		phyEnMsk9 = phyEnMsk9 | (1<<9);
		/*100BASE-TX full duplex capable in reg 4.8*/
		phyEnMsk4 = phyEnMsk4 | (1<<8);
		/*10BASE-TX full duplex capable in reg 4.6*/
		phyEnMsk4 = phyEnMsk4 | (1<<6);

		/*Force Auto-Negotiation setting in reg 0.12*/
		cfg.AutoNegotiation = 1;
             

	}
	
	if(cfg.AutoNegotiation)
	{
		/*Auto-Negotiation setting in reg 0.12*/
		phyEnMsk0 = phyEnMsk0 | (1<<12);
	}

	if(cfg.AsyFC)
	{
		/*Asymetric flow control in reg 4.11*/
		phyEnMsk4 = phyEnMsk4 | (1<<11);
	}
	if(cfg.FC)
	{
		/*Flow control in reg 4.10*/
		phyEnMsk4 = phyEnMsk4 | (1<<10);
	}

	
	/*1000 BASE-T control register setting*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_1000_BASET_CONTROL_REG, UTPPAG, 0, &phyData))
		return FAILED;

	phyData = (phyData & (~0x0200)) | phyEnMsk9 ;
		
	if(SUCCESS != rtl8363_setAsicReg(phy, PHY_1000_BASET_CONTROL_REG, UTPPAG, 0, phyData))
		return FAILED;
    

	/*Auto-Negotiation control register setting*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_AN_ADVERTISEMENT_REG, UTPPAG, 0, &phyData))
		return FAILED;

	phyData = (phyData & (~0x0DE0)) | phyEnMsk4;
		
	if(SUCCESS != rtl8363_setAsicReg(phy, PHY_AN_ADVERTISEMENT_REG, UTPPAG, 0, phyData))
		return FAILED;


	/*Control register setting and restart auto*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_CONTROL_REG, UTPPAG, 0, &phyData))
		return FAILED;
	phyData = (phyData & (~0x3140)) | phyEnMsk0;
	/*If have auto-negotiation capable, then restart auto negotiation*/
	if(cfg.AutoNegotiation)
	{
		phyData = phyData | (1 << 9);
	}
	
	if(SUCCESS != rtl8363_setAsicReg(phy,PHY_CONTROL_REG, UTPPAG, 0, phyData))
		return FAILED;

	return SUCCESS;
}

/*
@func s8 | rtl8363_getAsicEthernetPHY | Get PHY ability through PHY registers.
@parm u8 | phy | PHY number (0~1).
@parm phyCfg_t* | cfg | phy configuration
@struct phyCfg_t | This structure describe PHY parameter
@field u8 | AutoNegotiation | AutoNegotiation ability
@field u8 | Speed | force speed, could be PHY_10M, PHY_100M, PHY_1000M
@field u8 | Fullduplex | full or half duplex
@field u8 | Cap_10Half | 10BASE-TX half duplex capable
@field u8 | Cap_10Full  | 10BASE-TX full duplex capable
@field u8 | Cap_100Half | 100BASE-TX half duplex capable
@field u8 | Cap_100Full | 100BASE-TX full duplex capable
@field u8 | FC | flow control capability
@field u8 | AsyFC | asymmetric flow control capability
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
	Get the capablity of specified PHY.
*/
s8 rtl8363_getAsicEthernetPHY(u8 phy, phyCfg_t* cfg)
{
	u16 phyData0;
	u16 phyData4;
	u16 phyData9;
      u16 phyData17;
	

	if(phy > 1)
		return FAILED;


	/*Control register setting and restart auto*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_CONTROL_REG, UTPPAG, 0, &phyData0))
		return FAILED;

	/*Auto-Negotiation control register setting*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_AN_ADVERTISEMENT_REG, UTPPAG, 0, &phyData4))
		return FAILED;

	/*1000 BASE-T control register setting*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_1000_BASET_CONTROL_REG, UTPPAG, 0, &phyData9))
		return FAILED;

      cfg->Cap_1000Full = (phyData9 & (1<<9)) ? 1 : 0;
      cfg->AsyFC  = (phyData4 & (1<<11)) ? 1:0;
      cfg->FC = (phyData4 & (1<<10)) ? 1:0;
      cfg->Cap_100Full = (phyData4 & (1<<8)) ? 1:0;	
      cfg->Cap_100Half = (phyData4 & (1<<7)) ? 1:0;
      cfg->Cap_10Full = (phyData4 & (1<<6)) ? 1:0;
      cfg->Cap_10Half = (phyData4 & (1<<5)) ? 1:0;     
      cfg->AutoNegotiation = (phyData0 & (1<<12)) ? 1:0;

      /*phy register 0, [bit6, bit13] not reflect real speed after n-way, only reflect speed in force mode, 
      but reg17 [15:14] will reflect real speed after n-way*/
      if(SUCCESS != rtl8363_getAsicReg(phy,17, UTPPAG, 0, &phyData17))
 	     return FAILED;
      cfg->Fullduplex = (phyData17 & (1 << 13)) ? 1:0;
      cfg->Speed = (phyData17 & (0x3<<14)) >> 14;

      /*[S7,S8] could reflect terminal-side/network-side real speed*/
      //rtl8363_getAsicReg(phy?0:1, 23, MACPAG, 6, &phyData0);
      //cfg->Speed = (phyData0 & (0x3 << 3)) >> 3;
      //cfg->Fullduplex = (phyData0 & (0x1 << 2)) ? 1:0;
                 
	return SUCCESS;
}

/*
@func s8 | rtl8363_getAsicPHYLinkStatus | Get ethernet PHY linking status
@parm u8 | phy | PHY number (0~1).
@parm u8* | linkStatus | PHY link status 1:link up 0:link down
@rvalue SUCCESS | Success.
@rvalue FAILED | Failure. 
@comm
	Output link status of bit 2 in PHY register 1. API will return status is link up under both auto negotiation complete and link status are set to 1.  
*/
s8 rtl8363_getAsicPHYLinkStatus(u8 phy, u8 *linkStatus)
{
	u16 phyData;

	//if(phy > 1)
	//	return FAILED;

       #if 0 
	/*Get PHY status register, the second read will be stable*/
	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_STATUS_REG, UTPPAG, 0, &phyData))
		return FAILED;
    	if(SUCCESS != rtl8363_getAsicReg(phy,PHY_STATUS_REG, UTPPAG, 0, &phyData))
		return FAILED;
	/*check link status*/
	if(phyData & (1<<2))
	{
		*linkStatus = 1;
	}
	else
	{
		*linkStatus = 0;	
	}
     #endif

     /*Pn_Reg17.10 utp page 0  is always stable */
     rtl8363_getAsicRegBit(phy, 17, 10, UTPPAG, 0, &phyData);
     *linkStatus = phyData ? 1:0;    

	return SUCCESS;
}



/*
@func s8 | rtl8363_setAsicCPUPort | Specify Asic CPU port.
@parm u8 | port | Specify the port.
@parm u8 | enTag | CPU tag insert or not.
@rvalue SUCCESS 
@rvalue FAILED
@comm
If the port is specified RTL8363_NOCPUPORT, it means that no port is assigned as cpu port
*/

s8 rtl8363_setAsicCPUPort(u8 port, u8 enTag) 
{
    u16 regval;

    if (port > RTL8363_NOCPUPORT)
		return FAILED;
    rtl8363_getAsicReg(2, 20, MACPAG, 0, &regval);
    regval &= ~RTL8363_CPUPORT_MASK;
    regval |= (port & 0x3) << RTL8363_CPUPORT_OFFSET;
    regval |= RTL8363_CPUTAGRM | RTL8363_CPUTAGAW;
    regval &= ~RTL8363_CPUTAGIST;
    regval |= (enTag ? RTL8363_CPUTAGIST: 0);
    rtl8363_setAsicReg(2, 20, MACPAG, 0, regval);
    
    /*Disable IEEE802.1x function of CPU Port*/    
    if (port < RTL8363_NOCPUPORT )
   {
        rtl8363_getAsicReg(0, 19, MACPAG, 0, &regval);
        regval &= ~(1 << (8 + port));
        rtl8363_setAsicReg(0, 19, MACPAG, 0, regval);
    }
    
    return SUCCESS;
}

/*
@func s8 | rtl8363_getAsicCPUPort | Get Asic CPU port.
@parm u8* | port |  Which port is cpu port.
@parm u8* | enTag | CPU tag insert or not.
@rvalue SUCCESS 
@rvalue FAILED
@comm
*/

s8 rtl8363_getAsicCPUPort(u8 *pPort, u8 *pEntag)
{
    u16 regval;
    
    if ((pPort == NULL) || (pEntag == NULL))
        return FAILED;
    rtl8363_getAsicReg(2, 20, MACPAG, 0, &regval);
    *pPort = (regval & RTL8363_CPUPORT_MASK) >> RTL8363_CPUPORT_OFFSET;
    *pEntag = (regval & RTL8363_CPUTAGIST) ? TRUE: FALSE;

    return SUCCESS;
}


s8 rtl8363_getAsicChipID(enum ChipID *id, enum VerNum *ver)
{
    u16 regval;


    /*enable sernal read*/
    rtl8363_setAsicRegBit(1, 21, 9, MACPAG, 9, 1);
    
    rtl8363_getAsicReg(1, 20, MACPAG, 9, &regval);
    if (regval & (0x1 << 5))
    {
        if (((regval & (0x7 << 10)) >> 10) == 0x1 )
        {
              *id = RTL8203M;
        }
        else if (((regval & (0x7 << 10)) >> 10) == 0x3 )
        {
              *id = RTL8303H;
        }
        else 
        {
            return FAILED;
        }
        
    }
    else 
    {
        if (((regval & (0x7 << 10)) >> 10) == 0x1 )
        {
              *id = RTL8213M;
        }
        else if (((regval & (0x7 << 10)) >> 10) == 0x2 )
        {
              *id = RTL8363S;
        }
        else if (((regval & (0x7 << 10)) >> 10) == 0x3 )
        {
              *id = RTL8363H;
        }
        else 
        {
            return FAILED;
        }

    }
    
    //rtl8363_getAsicReg(0, 30, MACPAG, 15, &regval);
    /*from B version, version mumber register posion changed*/
    rtl8363_getAsicReg(1, 27, MACPAG, 9, &regval);
    switch(regval)
    {
        case 0:
              *ver = VER_A;
              break;
        case 1:
              *ver = VER_B;
               break;
        default:
              return FAILED;

    }

    /*disable sernal read*/
    rtl8363_setAsicRegBit(1, 21, 9, MACPAG, 9, 0);

    return SUCCESS;
}

/*
@func void | rtl8368_printEthernetPHYRegs | Print PHY registers
@parm u8 | phy | PHY address (0~2).
@rvalue None | Void
*/
void rtl8368_printEthernetPHYRegs(u8 phy)
{
	u16 phyData = 0;
	u8 link_status = 0;
	
	printk("Phy Number=%d\n",phy);
	/*Read Control register and print*/
	rtl8363_getAsicReg(phy,PHY_CONTROL_REG, UTPPAG, 0, &phyData);
	printk("Control register=0x%x\n",phyData);
	rtl8363_getAsicReg(phy,PHY_STATUS_REG, UTPPAG, 0, &phyData);
	printk("Status register=0x%x\n",phyData);
	rtl8363_getAsicReg(phy,PHY_AN_ADVERTISEMENT_REG, UTPPAG, 0, &phyData);
	printk("AN advert=0x%x\n",phyData);
	rtl8363_getAsicReg(phy,PHY_AN_LINKPARTNER_REG, UTPPAG, 0, &phyData);
	printk("AN link partner=0x%x\n",phyData);
	rtl8363_getAsicReg(phy,PHY_1000_BASET_CONTROL_REG, UTPPAG, 0, &phyData);
	printk("1000 control reg=0x%x\n",phyData);
	rtl8363_getAsicReg(phy,PHY_1000_BASET_STATUS_REG, UTPPAG, 0, &phyData);
	printk("1000 status reg=0x%x\n",phyData);
	rtl8363_getAsicPHYLinkStatus(phy, &link_status);
	printk("Link Status=0x%x\n",link_status);
}



/*
@func s8 | rtl8363_setAsicInit | Init Asic
@rvalue SUCCESS 
@rvalue FAILED
@comm
This function must be called during system initialization process
*/

s8 rtl8363_setAsicInit(void)
{
    enum ChipID id;
    enum VerNum ver;
    u8 port;
    u16 pvalue;    

    if (SUCCESS != rtl8363_getAsicChipID(&id, &ver))
    {
	printk("rtl8363_getAsicChipID failed\n");
        return FAILED;
    }

    printk("rtl Chip::Chip id = 0x%x, Version = 0x%x\n", id, ver);
    //if ((id == RTL8363S) && (ver == VER_A))
    if (id == RTL8363S)
    {
        for (port = 0; port < 2; port++)
        {
            /*GIGA PHY parameter*/
            rtl8363_setAsicReg(port, 30, UTPPAG, 0, 0x0FA0);
            rtl8363_setAsicReg(port, 22, UTPPAG, 0, 0xFE00);
            rtl8363_setAsicReg(port, 22, UTPPAG, 2, 0x1C01);
            rtl8363_setAsicReg(port, 0, UTPPAG, 2, 0x8502);
            rtl8363_setAsicReg(port, 1, UTPPAG, 2, 0x5011);
            rtl8363_setAsicReg(port, 4, UTPPAG, 2, 0x6100);
            rtl8363_setAsicReg(port, 5, UTPPAG, 2, 0xA8A8);
            rtl8363_setAsicReg(port, 7, UTPPAG, 2, 0x8050);
            rtl8363_setAsicReg(port, 8, UTPPAG, 2, 0x3602);
            rtl8363_setAsicReg(port, 9, UTPPAG, 2, 0x2C00);
            rtl8363_setAsicReg(port, 10, UTPPAG, 2, 0xAC84);
            rtl8363_setAsicReg(port, 12, UTPPAG, 2, 0xA8C3);
            rtl8363_setAsicReg(port, 21, UTPPAG, 2, 0x00A0);
            rtl8363_setAsicReg(port, 23, UTPPAG, 2, 0xB865);
            rtl8363_setAsicReg(port, 24, UTPPAG, 2, 0xB865);
            rtl8363_setAsicReg(port, 25, UTPPAG, 2, 0xB865);
            rtl8363_setAsicReg(port, 26, UTPPAG, 2, 0xB865);
            rtl8363_setAsicReg(port, 27, UTPPAG, 2, 0xB865);
            rtl8363_setAsicReg(port, 28, UTPPAG, 2, 0xB865);

            rtl8363_getAsicReg(0, 25, MACPAG, 15, &pvalue);
            pvalue |= 0x1FF;
            rtl8363_setAsicReg(0, 25, MACPAG, 15, pvalue);

            rtl8363_getAsicReg(0, 26, MACPAG, 15, &pvalue);
            pvalue |= 0x1FF;
            rtl8363_setAsicReg(0, 26, MACPAG, 15, pvalue);

            rtl8363_setAsicReg(port, 18, UTPPAG, 3, 0xF0A8);
            rtl8363_setAsicReg(port, 20, UTPPAG, 3, 0x8496);
            rtl8363_setAsicReg(port, 19, UTPPAG, 3, 0x0700);
            rtl8363_setAsicReg(port, 14, UTPPAG, 1, 0x7BAF);
            rtl8363_setAsicReg(port, 11, UTPPAG, 1, 0x8C9D);
            rtl8363_setAsicReg(port, 20, UTPPAG, 1, 0x3F32);
            rtl8363_setAsicReg(port, 28, UTPPAG, 1, 0xF600);
            rtl8363_setAsicReg(port, 9, UTPPAG, 1, 0x2715);
            rtl8363_setAsicReg(port, 6, UTPPAG, 1, 0x0084);
            rtl8363_setAsicReg(port, 8, UTPPAG, 1, 0x045C);
            rtl8363_setAsicReg(port, 12, UTPPAG, 1, 0xDC09);
            rtl8363_setAsicReg(port, 29, UTPPAG, 1, 0x3D9A);
                                    
        }
        // Added to bring P0 out of PWR DWN
        rtl8363_setAsicReg(0, 0, UTPPAG, 0, 0x1140);
	//rtl8363_setAsicRegBit(0, 0, 11, UTPPAG, 0, 0);
	//Enable/Disable AutoNEG
        rtl8363_setAsicRegBit(0, 0, 9, MACPAG, 0, 1);
        rtl8363_setAsicRegBit(1, 0, 9, MACPAG, 0, 1);

    }

    return SUCCESS;
}

EXPORT_SYMBOL(rtl8363_getAsicReg);
EXPORT_SYMBOL(rtl8363_getAsicPHYLinkStatus);
EXPORT_SYMBOL(rtl8363_setAsicRegBit);
EXPORT_SYMBOL(rtl8363_setAsicReg);
