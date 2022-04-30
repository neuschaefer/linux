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
 * author: deepak.sikri@st.com
 */

#include "spr_870E_eth_syn.h"

//#define printk(fmt...) 

#define COUNT_TX_DESCR	16	//number of Transmit descriptors, fixed at the start
#define COUNT_RX_DESCR	16	// number of Receive Descriptors, fixed at the start
#define DMA_INT_ENABLE 	(DmaIeTxCompleted |DmaIeRxCompleted | DmaIeNormal)
#define DMA_INT_DISABLE 0

#define DMA_REG_COUNT   22
#define MAC_REG_COUNT   48
#define PHY_WAIT_ITERATIONS	55555/* 1000 iterations * 10uS = 10mS max */
#define PHY_WAIT_MICRO_SECONDS	10

//#define UBOOT_TEST


static void ETH_Mac_write(volatile u32 *Mac_Reg, u32 data)
{
	*Mac_Reg = data;
}

static u32 ETH_Mac_read(volatile u32 *Mac_Reg)
{
	return (*Mac_Reg);
}

static void ETH_Mac_RegBits_Set(volatile u32 *Mac_Reg,u32 data)
{
	*Mac_Reg |= data;
}

#ifdef ETH_UNUSED_FEAT
static void ETH_Mac_RegBits_Clear(volatile u32 *Mac_Reg, u32 data)
{
	*Mac_Reg &= (~data);
}
#endif
static
void ETH_Dma_write(volatile u32 *Dma_Reg, u32 data)
{
	*Dma_Reg = data;
}

static
u32 ETH_Dma_read(volatile u32 *Dma_Reg)
{
	return (*Dma_Reg);
}

static void ETH_Dma_RegBits_Set(volatile u32 *Dma_Reg, u32 data)
{
	*Dma_Reg |= data;
}

#ifdef ETH_UNUSED_FEAT
static void ETH_Dma_RegBits_Clear(volatile u32 *Dma_Reg, u32 data)
{
	*Dma_Reg &= (~data);
}
#endif

void ETH_Dma_IntDisable(void)
{
	ETH_Dma_write(DmaMac_IntEn, DMA_INT_DISABLE);
}

#ifdef RCV_INTR_EN
void ETH_Dma_IntEnable(void)
#else
static void ETH_Dma_IntEnable(void)
#endif
{
	ETH_Dma_write(DmaMac_IntEn, DMA_INT_ENABLE);
}


static void ETH_Dma_IntClear(void )
{				/* Clear all interrupt requests */

	ETH_Dma_write(DmaMac_StatsReg, ETH_Dma_read(DmaMac_StatsReg));
}


static void ETH_Flush_Tx_FIFO(void)
{
	int i;
	ETH_Dma_RegBits_Set(DmaMac_OprModeReg, DmaFlush_TxFIFO);
#if 0
	while (ETH_Dma_read(DmaMac_OprModeReg) & (DmaFlush_TxFIFO));	/* Wait here until this bit gets clear */
#else
	for (i = 0; (ETH_Dma_read(DmaMac_OprModeReg) & (DmaFlush_TxFIFO)); i++) 
	{
		if (i == PHY_WAIT_ITERATIONS) 
		{
			printk("Eth txfifo :times out \n");
			break;
		}
		udelay(PHY_WAIT_MICRO_SECONDS);
	}
#endif
}


static void ETH_Enable_Rx(void)
{
	ETH_Mac_RegBits_Set(Gmac_Ctrl, Gmac_RxEn);
}

static void ETH_Enable_Tx(void)
{
	ETH_Mac_RegBits_Set(Gmac_Ctrl, Gmac_TxEn);
}

#ifdef ETH_UNUSED_FEAT
static void ETH_Promiscuous_Mode_ON(void)
{
	ETH_Mac_RegBits_Set(Gmac_FrameFilter, Gmac_PrmscMode);
}
static void ETH_Loopback_ON(void)
{
	ETH_Mac_RegBits_Set(Gmac_Ctrl, Gmac_LoopMode);
}
#endif
void ETH_Rxstart(void )
{
	ETH_Dma_RegBits_Set(DmaMac_OprModeReg, DmaRxStart);
}
#ifdef ETH_UNUSED_FEAT
static void ETH_RxResume( void)
{
	ETH_Dma_write(DmaMac_RxPollReg, DmaMac_RxPoll_Data);
}
#endif
void ETH_Txstart(void )
{
	ETH_Dma_RegBits_Set(DmaMac_OprModeReg, DmaTxStart);
}


void ETH_TxResume( void)
{

	ETH_Dma_write(DmaMac_TxPollReg, (u32) DmaMac_TxPoll_Data);
}


int ETH_Reset(void)
{
	unsigned int    i;
	//*DmaMac_BusMode = DmaMac_SRst; /* Reset ETH*/
	ETH_Dma_write(DmaMac_BusMode, DmaMac_SRst);
	ETH_Mac_write(Gmac_Ctrl, Gmac_MII_PortSelect);
	udelay(2);
#if 0
	while (ETH_Dma_read(DmaMac_BusMode) & (DmaMac_SRst))
#else
	for (i = 0; (ETH_Dma_read(DmaMac_BusMode) & (DmaMac_SRst)); i++)
	{
		if (i == PHY_WAIT_ITERATIONS) 
		{
			printk("Eth reset :times out \n");
			return -1;
			//break;
		}
		udelay(2);
	}

#endif
	udelay(2);
	return 0;
}


void ETH_Dma_Init(eth_config* ether_config)
{
/*	ETH_Reset(); */		/* Reset complete ETH block, will not come in initialization */
				/* as this will reset the start descriptor addresses also set in init_tx/rx_descr(),*/
/*	This function should be called before any hardware initilization as it resets everything. */
#ifdef SPR_DMA_IMPLICIT_CHAIN
	ETH_Dma_write( DmaMac_BusMode,DmaMac_FixedBurst | DmaMac_Burst_8 | DmaMac_DescrSkipLen8 ); 
#else
	ETH_Dma_write(DmaMac_BusMode, DmaMac_FixedBurst /* |DmaMac_Arb */  | DmaMac_Burst_8 /* | DmaMac_DescrSkipLen8 */ );	/* DmaMac_Arb bit set means Rx has priority over Tx */
#endif
	ETH_Flush_Tx_FIFO();		/* Flushes Transmit FIFO */
	/* Enable HW flow control : to be done */
	/* configure fro store & forward, operate on second frame */
#ifdef SPR_DMA_IMPLICIT_CHAIN
	ETH_Dma_write(DmaMac_OprModeReg,DmaTxThreshold256 | DmaRxThreshold128 );
#else
	ETH_Dma_write(DmaMac_OprModeReg, DmaStoreAndForward | DmaTxSecondFrame);
#endif
	ETH_Dma_IntClear();		/* Clear the status register */
	ETH_Dma_IntEnable();		/* Enable desired interrupt */
}


void ETH_Mac_Init(eth_config* ether_config)
{
#ifdef JUMBO_FRAME_SUPPORT
	volatile unsigned int    tmp;
#endif
	if (ether_config->dplx == FULL_DUPLEX)
	{
		if (ether_config->speed_1000)
			ETH_Mac_write(Gmac_Ctrl, Gmac_FrameBurstEnable | 
				Gmac_Gmii_PortSelect | Gmac_FullDplxMode |
				Gmac_EnableRxOwn);/* by default configuration to MII mode & full duplex */
		else
			ETH_Mac_write(Gmac_Ctrl, Gmac_FrameBurstEnable | 
				Gmac_MII_PortSelect | Gmac_FullDplxMode | 
				Gmac_EnableRxOwn);/* by default configuration to MII mode & full duplex */

//	ETH_Mac_write(Gmac_FrameFilter, Gmac_RxAll);
	ETH_Mac_write(Gmac_FrameFilter, 0x00);
//	ETH_Mac_write(Gmac_FlwCtrlReg, Gmac_FlwCtrlTxEn | Gmac_FlwCtrlRxEn );  
	}
	else
	{
		if (ether_config->speed_1000)
			ETH_Mac_write(Gmac_Ctrl, Gmac_FrameBurstEnable |
			 Gmac_Gmii_PortSelect | /*Gmac_FullDplxMode | */ 
			Gmac_DisableRxOwn);	/* by default configuration to MII mode & half duplex */
		else
			ETH_Mac_write(Gmac_Ctrl, Gmac_FrameBurstEnable |
			Gmac_MII_PortSelect | /*Gmac_FullDplxMode | */ 
			Gmac_DisableRxOwn);	/* by default configuration to MII mode & half duplex */
		//ETH_Promiscuous_Mode_ON();
		ETH_Mac_write(Gmac_FrameFilter, 0x00);
	}
	ETH_Mac_write(Gmac_FlwCtrlReg, 0x00);
#if 1
	if((ether_config->rx_pause == 1) && (ether_config->tx_pause == 1))
		ETH_Mac_write(Gmac_FlwCtrlReg, Gmac_FlwCtrlTxEn | Gmac_FlwCtrlRxEn );
	else if((ether_config->tx_pause == 1))
		ETH_Mac_write(Gmac_FlwCtrlReg, Gmac_FlwCtrlTxEn );
#endif

#ifdef CONFIG_ETH_SPEAR_CHECKSUM_OFFLOAD
	{  
		u32 test_mii = 0;
		test_mii = ETH_Mac_read(Gmac_Ctrl); 
		test_mii |= Gmac_IPCSoffload;
		ETH_Mac_write(Gmac_Ctrl,test_mii );
	}
#endif
#ifdef JUMBO_FRAME_SUPPORT
	if(ether_config->jumbo_frame)
	{
		tmp = ETH_Mac_read(Gmac_Ctrl); 
		tmp |= (Gmac_JumboFrameEnable | Gmac_JabberDisable);
		ETH_Mac_write(Gmac_Ctrl,tmp );
	}
#endif
	/* To check the synchonization of MAC Rx/Tx with DMA Rx/Tx */
	ETH_Enable_Rx(); /* Enable ETH reception*/
	ETH_Enable_Tx(); /* Enable ETH transmission*/

}

u32 ETH_DmaIntType(void)
{
	u32             status = ETH_Dma_read(DmaMac_StatsReg);
	u32             type = 0;
	/*few ints are disabled as they are not being handled*/
	/*if( status & DmaMac_FatalBusErrInt )    
		type |= SpearDmaError;*/
#ifdef RCV_INTR_EN
	if(ETH_Dma_read(DmaMac_IntEn) & DmaIeRxCompleted )
	{
		if( status & DmaMac_RxInt )
		{
			type |= SpearDmaRxNormal;
			ETH_Dma_write(DmaMac_IntEn, (DMA_INT_ENABLE & (~DmaIeRxCompleted)));
		}
	}
#else
	if( status & DmaMac_RxInt )
	    type |= SpearDmaRxNormal;
#endif
	/*  if( status & DmaMac_RxBuffUnavail )     
		type |= SpearDmaRxAbnormal;*/

	/*  if( status & DmaMac_RxProcStop )        
		type |= SpearDmaRxStopped;*/

	if( status & DmaMac_TxInt )
	    type |= SpearDmaTxNormal;

	/*  if( status & DmaMac_TxBuffUnavl )       
		type |= SpearDmaTxAbnormal;*/

	/* if( status & DmaMac_TxProcStop )        
		type |= SpearDmaTxStopped;*/

	ETH_Dma_write(DmaMac_StatsReg, status);
	return type;
}

u32 ETH_Read_Mac_Address_Lo(void)
{
	return ETH_Mac_read(Gmac_Addr0LO);
}

u32 ETH_Read_Mac_Address_Hi(void)
{
	return ETH_Mac_read(Gmac_Addr0HI);
}
void ETH_Set_Mac_Address(u32 AddrHi,u32 AddrLO)
{
	ETH_Mac_write(Gmac_Addr0HI, AddrHi & Gmac_Addr0HI_Msk);
	ETH_Mac_write(Gmac_Addr0LO, AddrLO & Gmac_Addr0LO_Msk);
}
void  ETH_Set_RxDescrListAddr(u32 val)
{
	ETH_Mac_write(DmaMac_RxDescrListAddr, val);
}
void  ETH_Set_TxDescrListAddr(u32 val)
{
	ETH_Mac_write(DmaMac_TxDescrListAddr, val);
}

void ETH_PHY_POWER_DOWN(u8 PHY_Num)
{
	ETH_Mdio_write(PHY_Num,Phy_CtrlReg,Phy_PowerDown);
}


void ETH_Mdio_write (u8 PHY_Num,u8 PHY_Reg, u16 data)
{
	u32 addr;
	int i;
	ETH_Mac_write(GMii_DataReg, (u32) data);
	addr = ((PHY_Num << GmiiAddrShift) & GPhy_AddrMsk) | 
		((PHY_Reg << GmiiRegShift) & GMii_RegisterMsk) | GMii_Write;
//ETH_Mac_write(GMii_AddrReg, (addr | GMii_ClkRange_35_60M | GMii_Busy));
	ETH_Mac_write(GMii_AddrReg, (addr | GMii_ClkRange_150_250M | GMii_Busy));
#if 0
	do
	{
	}while ((ETH_Mac_read(GMii_AddrReg) & GMii_Busy) == GMii_Busy);
#else
	for (i = 0; ((ETH_Mac_read(GMii_AddrReg) & GMii_Busy) == GMii_Busy); i++)
	{
		if (i == PHY_WAIT_ITERATIONS)
			{
				printk("Ethmdiowrite :times out \n");
				break;
			}
		udelay(PHY_WAIT_MICRO_SECONDS);
	}
#endif
}

u16 ETH_Mdio_Read( volatile u8 PHY_Num, volatile u8 PHY_Reg )
{
	u32 addr;
	u16 data=0;
	int i;

	addr = ((PHY_Num << GmiiAddrShift) & GPhy_AddrMsk) | 
	    ((PHY_Reg << GmiiRegShift) & GMii_RegisterMsk);

//	ETH_Mac_write(GMii_AddrReg, (addr | GMii_ClkRange_35_60M | GMii_Busy));
	ETH_Mac_write(GMii_AddrReg, (addr | GMii_ClkRange_150_250M | GMii_Busy));
#if 0
	do
	{ }while ((ETH_Mac_read(GMii_AddrReg) & GMii_Busy) == GMii_Busy);
#else
	for (i = 0; ((ETH_Mac_read(GMii_AddrReg) & GMii_Busy) == GMii_Busy); i++)
	{
		if (i == PHY_WAIT_ITERATIONS)
		{
			printk("Eth mdio rd2 :times out \n");			
			goto out;
		}
		udelay(PHY_WAIT_MICRO_SECONDS);
	}
#endif
	data = (u16) (ETH_Mac_read(GMii_DataReg) & (0xFFFF));
out:
	return (data);
}

#ifdef UBOOT_TEST
void
dump_registers(void)
{
	int *p;
	int i;
	char *register_names_DMA[DMA_REG_COUNT] =
	{
	"DMA Bus Mode Register ",
	"Transmit POLL Demand Reg ",
	"Receive PLL Demand Reg",
	"Receive Descriptor List Address Reg",
	"transmit Descriptor List Address Reg",
	"Status Register",
	"Operation mode Register",
	"Interrupt Enable Reg",
	"Missed frame & Buffer OV Counter Reg",
	"Reserved",			/* Location offset 0x1024 */
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",			/* Location offset 0x1044 */
	"Current Host Transmit Descriptor Reg ",
	"Current Host Receive Descriptor Reg ",
	"Current Host transmit buffer Address Reg",
	"Current Host Receive Buffer Address Reg",
	};
	char *register_names_MAC[MAC_REG_COUNT] =
	{
	"ETH Control Reg",
	"ETH Frame Filter",
	"Hash Table High Register",
	"Hash Table Low Register",
	"GMII Address Register",
	"GMII Data Regsiter",
	"Flow Control Register",
	"VLAN Tag Register",
	"Version register",
	"Reserved",
	"Wake up frame filter pointer",
	"PMT Control & Status Register",
	"Reserved",			/* Location offset 0x0030 */
	"Reserved",
	"Reserved",
	"Reserved",			/* Locatino offset 0x003C */
	"MAC Address0 High Reg ",
	"MAC Address0 Low Reg",
	"MAC Address1 High Reg ",
	"MAC Address1 Low Reg",
	"MAC Address2 High Reg ",
	"MAC Address2 Low Reg",
	"MAC Address3 High Reg ",
	"MAC Address3 Low Reg",
	"MAC Address4 High Reg ",
	"MAC Address4 Low Reg",
	"MAC Address5 High Reg ",
	"MAC Address5 Low Reg",
	"MAC Address6 High Reg ",
	"MAC Address6 Low Reg",
	"MAC Address7 High Reg ",
	"MAC Address7 Low Reg",
	"MAC Address8 High Reg ",
	"MAC Address8 Low Reg",
	"MAC Address9 High Reg ",
	"MAC Address9 Low Reg",
	"MAC Address10 High Reg ",
	"MAC Address10 Low Reg",
	"MAC Address11 High Reg ",
	"MAC Address11 Low Reg",
	"MAC Address12 High Reg ",
	"MAC Address12 Low Reg",
	"MAC Address13 High Reg ",
	"MAC Address13 Low Reg",
	"MAC Address14 High Reg ",
	"MAC Address14 Low Reg",
	"MAC Address15 High Reg ",
	"MAC Address15 Low Reg",
	};
	p = (int *) DmaMac_BusMode;
	printk("\n\tDMA Registers\n");
	printk("-----------------------\n");

	for (i = 0; i < DMA_REG_COUNT; i++, p++)
		printk("%s:       0x%08x\n", register_names_DMA[i], *p);
	printk("\n");
	p = (int *) Gmac_Ctrl;
	printk("\n\tMAC Registers\n");
	printk("-----------------------\n");
	for (i = 0; i < MAC_REG_COUNT; i++, p++)
		printk("%s:       0x%08x\n", register_names_MAC[i], *p);
	printk("\n");
}
#endif

int ETH_DmaRxValid( u32 Status )
{
#ifdef JUMBO_FRAME_SUPPORT
return ((Status & DescError) == 0);	/* no errors, whole frame is in the buffer */
#else
	return ((Status & DescError) == 0)	/* no errors, whole frame is in the buffer */
	&& ( (Status & DescRxFirst) != 0 )
	&& ( (Status & DescRxLast) != 0 );
#endif
}
int ETH_SpearDmaRxCollisions(  u32 Status)
{
	if( Status & (/*DescRxDamaged |*/ DescRxCollision) ) 
	    return 1;
	return 0;
}

int ETH_SpearDmaRxCrc(  u32 Status)
{
	if( Status & DescRxCrc ) 
	    return 1;
	return 0;
}
int ETH_SpearDmaRxFrame(  u32 Status)
{
	if( Status & DescRxDribbling ) 
	    return 1;
	return 0;
}
int ETH_SpearDmaRxLengthError(  u32 Status)
{
	if( Status & DescRxLengthError ) 
	    return 1;
	return 0;
}
int ETH_SpearDmaTxValid(  u32 Status)/* Test the status word if the descriptor is valid */
{
	return ((Status & DescError) == 0);
}

int ETH_SpearDmaTxCollisions(  u32 Status)
{
	return (Status & DescTxCollMask) >> DescTxCollShift;
}

int ETH_SpearDmaTxAborted(  u32 Status)
{
	if( Status & (DescTxLateCollision | DescTxExcCollisions )) 
	    return 1;
	return 0;
}

int ETH_SpearDmaTxCarrier(  u32 Status)
{
	if( Status & (DescTxLostCarrier | DescTxNoCarrier )) 
	    return 1;
	return 0;
}

EXPORT_SYMBOL(ETH_Set_Mac_Address);
EXPORT_SYMBOL(ETH_DmaIntType);
EXPORT_SYMBOL(ETH_SpearDmaTxCollisions);
EXPORT_SYMBOL(ETH_SpearDmaTxCarrier);
EXPORT_SYMBOL(ETH_SpearDmaTxAborted);
EXPORT_SYMBOL(ETH_SpearDmaTxValid);
EXPORT_SYMBOL(ETH_Dma_IntDisable);
EXPORT_SYMBOL(ETH_Txstart);
EXPORT_SYMBOL(ETH_PHY_POWER_DOWN);
EXPORT_SYMBOL(ETH_Rxstart);
EXPORT_SYMBOL(ETH_Mac_Init);
EXPORT_SYMBOL(ETH_Dma_Init);
EXPORT_SYMBOL(ETH_Reset);
EXPORT_SYMBOL(ETH_Read_Mac_Address_Hi);
EXPORT_SYMBOL(ETH_Read_Mac_Address_Lo);
EXPORT_SYMBOL(ETH_Mdio_write);
EXPORT_SYMBOL(ETH_SpearDmaRxLengthError);
EXPORT_SYMBOL(ETH_SpearDmaRxFrame);
EXPORT_SYMBOL(ETH_SpearDmaRxCrc);
EXPORT_SYMBOL(ETH_SpearDmaRxCollisions);
EXPORT_SYMBOL(ETH_DmaRxValid);
EXPORT_SYMBOL(ETH_Set_RxDescrListAddr);
EXPORT_SYMBOL(ETH_Set_TxDescrListAddr);
EXPORT_SYMBOL(ETH_TxResume);
EXPORT_SYMBOL(ETH_Mdio_Read);
