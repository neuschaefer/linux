/*
 *   Copyright (C) STMicroelectronics 2008 by Deepak Sikri
 *   deepak.sikri@st.com
 *
 *   This program is free software; you	can redistribute it and/or modify
 *   it	under the terms	of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later	version.
 *
 *   This program is distributed in the	hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the	implied	warranty of
 *   MERCHANTABILITY or	FITNESS	FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public	License	for more details.
 *
 *   You should	have received a	copy of	the GNU	General	Public License
 *   along with	this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59	Temple Place - Suite 330, Boston, MA  02111-1307, USA.
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
#include <linux/delay.h>
#include <asm/unaligned.h>

#include "spr_870E_eth_syn.h"

#undef ETH_DEBUG_DESCR_INIT
//#define ETH_DEBUG_INT
//#define ETH_DEBUG_RX_INT
//#define ETH_DEBUG_TX_INT
//#define ETH_DEBUG_START_XMIT
//#define ETH_DEBUG_SKB_RX_INT
//#undef ETH_DEBUG_TX_INT
//#undef ETH_DEBUG_START_XMIT
#undef ETH_DEBUG_SKB_START_XMIT
#undef ETH_DEBUG_MDIO_READ
#undef ETH_DEBUG_MDIO_WRITE
#undef ETH_DEBUG_MDIO_PROBE
#undef ETH_DEBUG_MULTICAST
#define	ETH_DEBUG_OPEN
#define	ETH_DEBUG_STOP
#undef ETH_DEBUG_STAT
#undef ETH_DEBUG_TIMEOUT
#undef ETH_DEBUG_IOCTL
#undef ETH_DEBUG_INIT
#undef ETH_DEBUG_CLEANUP
#define	ETH_SPEAR_TEST
#define	SPEAr_ETHTOOL
#define	printk(fmt...)

static void SPEAr_ETH_Tmr_Hotplug_Fun (unsigned	long data);
static void SPEAr_MAC_Cfg (eth_config *	ether_config, struct eth_priv *adap);

#ifdef CONFIG_ARCH_SPEARPLUS
char g_spear_driver_name[] = "spearplus";
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
char g_spear_driver_name[] = "spearbasic";
#endif

static const char g_spear_driver_string[] = "ST(R) Network Driver";
char g_spear_driver_version[] =	"1.1.0.";
static const char spear_copyright[] =	"Copyright (c) 2005-2009 ST";

int SPEAR_ETH_Up (struct eth_priv *adapter);
void SPEAR_ETH_Down (struct eth_priv *adapter);
extern void SPEAR_ETHTOOL_set_opts (struct net_device *netdev);

#ifdef CONFIG_ARCH_SPEARPLUS
#define	__DRIVER_NAME	"ST SPEARPLUS Ethernet Driver version 0.1"
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define	__DRIVER_NAME	"ST SPEARBASIC Ethernet	Driver version 0.1"
#endif

static u8 Phy_num = 0;

#define	DBG_ETHERNET
#ifndef	DBG_ETHERNET
#define	printk(S)
#define	printk(S, A...)
#endif

/*
 *	NAME:SPEAR_ETH_Dump_Registers
 *
 * DESCRIPTION:
 *	This routine dumps Ethernet MAC	HW registers
 *
 * PARAMETERS:
 * IN  : 
 *	net_device
 *
 * OUT :
 *	None
 *
 * RETURN:
 *	void
 *
 * REENTRANCY: NA
*/
void
SPEAR_ETH_Dump_Registers (struct net_device *dev)
{
	int *p;
	int i;
	char *register_names_DMA[DMA_REG_COUNT]	=
	{ 
	 "DMA Bus Mode	Register ", "Transmit POLL Demand Reg ",
	"Receive PLL Demand	Reg",
	"Receive Descriptor	List Address Reg",
	"transmit Descriptor List Address Reg", "Status Register",
	"Operation mode Register",
	"Interrupt Enable Reg",
	"Missed frame & Buffer OV Counter Reg", "Reserved",
	/* Location	    offset 0x1024 */
	"Reserved",	"Reserved", "Reserved",	"Reserved", "Reserved",	"Reserved",
	"Reserved",	"Reserved",
	/* Location	    offset 0x1044 */
	"Current Host Transmit Descriptor Reg ",
	"Current Host Receive Descriptor Reg ",
	"Current Host transmit buffer Address Reg",
	"Current Host Receive Buffer Address Reg",
	};
	
	char *register_names_MAC[MAC_REG_COUNT] =
	{ 
	"ETH Control Reg", "ETH Frame Filter", "Hash Table High Register",
	"Hash Table	Low Register",
	"GMII Address Register", "GMII Data	Regsiter",
	"Flow Control Register", "VLAN Tag Register",
	"Version register",	"Reserved",
	"Wake up frame filter pointer",
	"PMT Control & Status Register", "Reserved",
	/* Location	    offset 0x0030 */
	"Reserved",	"Reserved", "Reserved",	/* Locatino	offset 0x003C */
	"MAC Address0 High Reg ", "MAC Address0 Low	Reg",
	"MAC Address1 High Reg ", "MAC Address1 Low	Reg",
	"MAC Address2 High Reg ", "MAC Address2 Low	Reg",
	"MAC Address3 High Reg ", "MAC Address3 Low	Reg",
	"MAC Address4 High Reg ", "MAC Address4 Low	Reg",
	"MAC Address5 High Reg ", "MAC Address5 Low	Reg",
	"MAC Address6 High Reg ", "MAC Address6 Low	Reg",
	"MAC Address7 High Reg ", "MAC Address7 Low	Reg",
	"MAC Address8 High Reg ", "MAC Address8 Low	Reg",
	"MAC Address9 High Reg ", "MAC Address9 Low	Reg",
	"MAC Address10 High	Reg ", "MAC Address10 Low Reg",
	"MAC Address11 High	Reg ", "MAC Address11 Low Reg",
	"MAC Address12 High	Reg ", "MAC Address12 Low Reg",
	"MAC Address13 High	Reg ", "MAC Address13 Low Reg",
	"MAC Address14 High	Reg ", "MAC Address14 Low Reg",
	"MAC Address15 High	Reg ", "MAC Address15 Low Reg",
    };
    (void)register_names_DMA;(void)register_names_MAC;
	p = (int *) DmaMac_BusMode;
	printk ("\tDMA Registers\n");
	printk ("-----------------------\n");
	for (i = 0; i	< DMA_REG_COUNT; i++, p++)
		printk ("%s:	   0x%08x\n", register_names_DMA[i], *p);
	printk ("\n");
	p = (int *) Gmac_Ctrl;
	printk ("\tMAC Registers\n");
	printk ("-----------------------\n");
	for (i = 0; i	< MAC_REG_COUNT; i++, p++)
		printk ("%s:       0x%08x\n", register_names_MAC[i], *p);
	printk ("\n");
}

/*
 * NAME:  SPEAR_ETH_dump_dma_descriptors
 *
 * DESCRIPTION:
 *	This routine dumps dma descriptors
 *
 * PARAMETERS:
 * IN  :
 *	net_device
 *
 * OUT :
 *	None
 *
 * RETURN:
 *	void
 *
 * REENTRANCY: 
 * NA
*/
#ifndef	SPR_DMA_IMPLICIT_CHAIN
void
SPEAR_ETH_dump_dma_descriptors (struct net_device *dev)
{
	struct eth_priv	*priv	= (struct eth_priv *) dev->priv;
	DmaMacDescr *DmaDesc_p;
	int i;
	printk ("\nTX	descriptors [status   cntl     addr    next    skb]\n");
	DmaDesc_p	= (DmaMacDescr *) &	(priv->TX_MAC_DescrTable);
	
	for (i = 0; i	< TX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		printk ("%d	%08x = %08x %08x %08x %08x %08x\n", i,
			(int) DmaDesc_p, DmaDesc_p->TxRx_Status,
			DmaDesc_p->DmaMac_Cntl,	(int) DmaDesc_p->DmaMac_Addr,
			DmaDesc_p->DmaMac_Next,	(int) DmaDesc_p->skb);
	
	printk ("\nRX	 descriptors [status  cntl     addr	   next	   skb]\n");
	DmaDesc_p	= (DmaMacDescr *) &	(priv->RX_MAC_DescrTable);
	
	for (i = 0; i	< RX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		printk ("%d	%08x = %08x %08x %08x %08x %08x\n", i,
		(int) DmaDesc_p, DmaDesc_p->TxRx_Status,
		DmaDesc_p->DmaMac_Cntl,	(int) DmaDesc_p->DmaMac_Addr,
		DmaDesc_p->DmaMac_Next,	(int) DmaDesc_p->skb);
}
#endif

/*
 * NAME:	    SPEAR_ETH_dump_skb
 *
 * DESCRIPTION:
 *	This routine dumps skb
 *
 * PARAMETERS:
 * IN  : sk_buff*, int contents
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/
void
SPEAR_ETH_dump_skb (struct sk_buff *skb, int contents)
{
	int len;

	printk
	("skb: %08x, head: %08x, data: %08x, len: %08x, tail: %08x,end: %08x\n",
	(int) (skb), (int)	skb->head, (int) skb->data, (int) skb->len,
	(int) skb->tail, (int) skb->end);
	
	if (contents)
	{
		for (len = 0; len	< skb->len; len++)
		{
			if (!(len & 15))
				printk ("\n%04x: ",	len);
			printk ("%02x	", ((char *) skb->data)[len]);
		} 
		printk ("\n");
	}
}

/*
 * NAME:	  SPEAR_ETH_dump_dma_skb
 *
 * DESCRIPTION:
 *	This routine dumps skb
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
void
SPEAR_ETH_dump_dma_skb (struct net_device *dev)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	DmaMacDescr *DmaDesc_p;
	int i;
	
	printk ("RX descriptors sockets\n");
	DmaDesc_p = (DmaMacDescr *) &	(priv->RX_MAC_DescrTable);
	for (i = 0; i	< RX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		SPEAR_ETH_dump_skb (DmaDesc_p->skb,	0);
	
	printk ("TX descriptors sockets\n");
	DmaDesc_p = (DmaMacDescr *) &	(priv->TX_MAC_DescrTable);
	for (i = 0; i	< TX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		SPEAR_ETH_dump_skb (DmaDesc_p->skb,	0);
}

/*
 * NAME: SPEAR_ETH_Skb_Prepare
 *
 *
 * DESCRIPTION:
 *	This routine prepares the skb for sending the data
 *
 * PARAMETERS:
 *	IN  : net_device*,DmaMacDescr*
 *
 *	OUT :
 *	None
 *
 *	RETURN:
 *	void
 *
 *	REENTRANCY: NA
*/

/*
 *	Avoid memcpy in	RX handler by pre-allocating the socket	buffers
 */

static int
SPEAR_ETH_Skb_Prepare (struct net_device *dev, volatile	DmaMacDescr * pRxFD)
{
	struct sk_buff *skb;
#ifdef SPR_DMA_IMPLICIT_CHAIN
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
#endif
#ifdef JUMBO_FRAME_SUPPORT
	if (!(skb = dev_alloc_skb (priv->rx_skb_size)))
#else
	if (!(skb = dev_alloc_skb (ETH_BUFF_SIZE)))
#endif
	{
		return -1;
	}
	skb_reserve (skb, 4);
	skb->dev = dev;

#ifndef	JUMBO_FRAME_SUPPORT
//	consistent_sync (skb->data, ETH_BUFF_SIZE, DMA_FROM_DEVICE);
#else
//	consistent_sync (skb->data, priv->rx_skb_size, DMA_FROM_DEVICE);
#endif
	barrier ();
	/* attach	skb	to FD */
	pRxFD->skb = skb;
	pRxFD->DmaMac_Addr = __virt_to_bus (skb->data);
#ifdef SPR_DMA_IMPLICIT_CHAIN
	if (priv->rx_skb_size	> MAC_MAX_FRAME_SZ)
		pRxFD->DmaMac_Addr1 = __virt_to_bus(skb->data + MAC_MAX_FRAME_SZ);
#endif
  return 0;
}

/*
 * NAME:	   SPEAR_ETH_Init_Tx_Descrs
 *
 * DESCRIPTION:
 *	This routine initializes the transmit descriptors
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
SPEAR_ETH_Init_Tx_Descrs (struct net_device *dev)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	DmaMacDescr 	*TX_DmaDesc_p;
	int 		i;
	int 		size = TX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

	
#ifndef	SPR_DMA_IMPLICIT_CHAIN
	dma_addr_t eth_pa_bp;
	unsigned char	*eth_va_bp;
#endif
	
	priv->TX_MAC_DescrTable =
		dma_alloc_coherent(NULL, size, &priv->DMA_TX_Descrp, GFP_DMA);

	/* Build the Frame Descriptor table for sending */
	for (i = 0; i < TX_MAC_DESCR_NUM; i++)
	{
		TX_DmaDesc_p = (DmaMacDescr *) & (priv->TX_MAC_DescrTable[i]);
#ifndef	SPR_DMA_IMPLICIT_CHAIN
		TX_DmaDesc_p->DmaMac_Next = (((u32)priv->DMA_TX_Descrp +
		((u32) & priv->TX_MAC_DescrTable[(i< TX_MAC_DESCR_NUM - 1) ? 
		(i + 1) :0] - (u32) & priv->TX_MAC_DescrTable[0])));
	
	      if (TX_DmaDesc_p->Eth_VAddr == 0)
		{
			eth_va_bp = 
			dma_alloc_coherent (NULL, ETH_BUFF_SIZE, &eth_pa_bp, GFP_DMA);
			if (eth_va_bp == NULL)
				return -1;
			TX_DmaDesc_p->Eth_VAddr = eth_va_bp;
			TX_DmaDesc_p->Eth_PAddr = (volatile char *) eth_pa_bp;
		}
#endif
		TX_DmaDesc_p->skb = NULL;
#ifdef ETH_ARCH_SPEARBASIC_BA
		TX_DmaDesc_p->TxRx_Status &=  ~(DescTxIntEnable | DescTxLast 
			| DescTxFirst | DescTxDisableCrc | DescTxCheckInsCtrl
			| DescTxEndOfRing | DescTxDisablePadd);
#ifndef	SPR_DMA_IMPLICIT_CHAIN
		TX_DmaDesc_p->TxRx_Status = DescTxChain |	DescTxIntEnable;
#else
		TX_DmaDesc_p->TxRx_Status = DescTxIntEnable;
#endif
		TX_DmaDesc_p->TxRx_Status &= (~(DescTxStatusMask | DescOwnByDma));	/* CPU owner */
		TX_DmaDesc_p->DmaMac_Cntl	= 0;
#else
#ifdef SPR_DMA_IMPLICIT_CHAIN
		TX_DmaDesc_p->DmaMac_Cntl	= DescTxIntEnable;
#else
		TX_DmaDesc_p->DmaMac_Cntl	= DescChain | DescTxIntEnable;
#endif
		TX_DmaDesc_p->TxRx_Status	= 0;	/* CPU owner */
#endif
	}
	priv->TX_MAC_WinSize = TX_MAC_DESCR_NUM;
	ETH_Set_TxDescrListAddr ((u32) priv->DMA_TX_Descrp);
	return 0;
}

/*
 * NAME:	     SPEAR_ETH_Init_Rx_Descrs
 *
 * DESCRIPTION:
 *	This routine initializes the recive descriptors
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
SPEAR_ETH_Init_Rx_Descrs (struct net_device *dev)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	DmaMacDescr 	*RX_DmaDesc_p;
	int 		i;
	int 		size = RX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

	priv->RX_MAC_DescrTable =
		dma_alloc_coherent(NULL, size, &priv->DMA_RX_Descrp, GFP_DMA);

#ifdef JUMBO_FRAME_SUPPORT
	u32 temp, temp1;

	if (SPEAR_DMA_ALIGN (dev->mtu	+ SPEAR_MTU_OVERHEAD) <= MAC_MAX_FRAME_SZ)
	{
		temp = SPEAR_DMA_ALIGN (dev->mtu + SPEAR_MTU_OVERHEAD);
		temp1 = 0;
	}
	else
	{
		temp = MAC_MAX_FRAME_SZ;
		temp1 = SPEAR_DMA_ALIGN	((dev->mtu - MAC_MAX_FRAME_SZ) 
					+ SPEAR_MTU_OVERHEAD);
	}
#endif //(JUMBO_FRAME_SUPPORT)

	/* Build the Frame Descriptor	table for receiving	*/
	for (i = 0; i	< RX_MAC_DESCR_NUM; i++)
	{
		RX_DmaDesc_p = (DmaMacDescr *) & (priv->RX_MAC_DescrTable[i]);
#ifndef	SPR_DMA_IMPLICIT_CHAIN
		RX_DmaDesc_p->DmaMac_Next = (((u32)priv->DMA_RX_Descrp +
		((u32) & priv->RX_MAC_DescrTable[(i < RX_MAC_DESCR_NUM - 1)?(i + 1):0] 
		- (u32) & priv->RX_MAC_DescrTable[0])));
#endif

#ifndef	JUMBO_FRAME_SUPPORT
#ifdef ETH_ARCH_SPEARBASIC_BA
		RX_DmaDesc_p->DmaMac_Cntl =(MAC_MAX_FRAME_SZ & DescSize1Mask) | 
						DescRxChain;
#else
		RX_DmaDesc_p->DmaMac_Cntl =(MAC_MAX_FRAME_SZ & DescSize1Mask) | 
						DescChain;
#endif
#else
#ifdef ETH_ARCH_SPEARBASIC_BA
		RX_DmaDesc_p->DmaMac_Cntl = (temp & DescSize1Mask) |
			((temp1	<< DescSize2Shift) & DescSize2Mask);
		if (i == RX_MAC_DESCR_NUM- 1)
			RX_DmaDesc_p->DmaMac_Cntl |= DescRxEndOfRing;
#else
		RX_DmaDesc_p->DmaMac_Cntl = (temp & DescSize1Mask) |
		((temp1	<< DescSize2Shift) & DescSize2Mask);
		
		if (i == RX_MAC_DESCR_NUM- 1)
			RX_DmaDesc_p->DmaMac_Cntl |= DescEndOfRing;
#endif
#endif
		if (SPEAR_ETH_Skb_Prepare	(dev, RX_DmaDesc_p) < 0)
			return -1;
#ifdef ETH_ARCH_SPEARBASIC_BA
		RX_DmaDesc_p->TxRx_Status	= DescOwnByDma;	/* DMA owner */
#else
		RX_DmaDesc_p->TxRx_Status	= DescOwnByDma;	/* DMA owner */
#endif
	}

	/* The Physical address needs	to be passed to	the	DMA	*/
	/*DmaMac_RxDescrListAddr = (u32) priv->DMA_RX_Descrp;*/
	/* priv->DMA_RX_Descrp	contains the physical address of Rxdescriptor
	chain, set in init function */
	
	ETH_Set_RxDescrListAddr ((u32) priv->DMA_RX_Descrp);
	return 0;
}

/*
 * NAME:  SPEAR_ETH_Free_Tx_Descrs
 *
 * DESCRIPTION:
 *	This routine frees the Transmit	Descriptors
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

static void
SPEAR_ETH_Free_Tx_Descrs (struct net_device *dev)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	DmaMacDescr 	*DmaDesc_p;
	int 		i;
	int 		size = TX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

  /* Free Rx/Tx	skbuff */
	for (i = 0; i	< TX_MAC_DESCR_NUM; i++)
	{
		DmaDesc_p	= (DmaMacDescr *) & (priv->TX_MAC_DescrTable[i]);
#ifndef	SPR_DMA_IMPLICIT_CHAIN
		if (DmaDesc_p->Eth_VAddr)
		{
			dma_free_coherent (NULL, ETH_BUFF_SIZE,
			(void*)DmaDesc_p->Eth_VAddr, (dma_addr_t)DmaDesc_p->Eth_PAddr);
			
			DmaDesc_p->Eth_VAddr = 0;
			DmaDesc_p->Eth_PAddr = 0;
		}
#endif
		if (DmaDesc_p->skb)
			dev_kfree_skb_irq (DmaDesc_p->skb);
		memset (DmaDesc_p, 0, sizeof (DmaMacDescr));
	}
	/* Free the TX Descriptor table */
	dma_free_coherent(NULL, size, priv->TX_MAC_DescrTable,priv->DMA_TX_Descrp);
}

/*
 * NAME:  SPEAR_ETH_Free_Rx_Descrs
 *
 * DESCRIPTION:
 *	This routine prepares the skb for sending the data
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
static void
SPEAR_ETH_Free_Rx_Descrs (struct net_device *dev)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	DmaMacDescr 	*DmaDesc_p;
	int 		i;
	int 		size = RX_MAC_DESCR_NUM * sizeof(DmaMacDescr);
	
	for (i = 0; i < RX_MAC_DESCR_NUM; i++)
	{
		DmaDesc_p = (DmaMacDescr *) & (priv->RX_MAC_DescrTable[i]);
	
		if (DmaDesc_p->skb)
			dev_kfree_skb (DmaDesc_p->skb);
		memset (DmaDesc_p, 0, sizeof (DmaMacDescr));
    	}
	/* Free the RX Descriptor table */
	dma_free_coherent(NULL, size,priv->RX_MAC_DescrTable,priv->DMA_RX_Descrp);
}

/*
 * NAME:  SPEAR_ETH_rx_int
 *
 * DESCRIPTION:
 *	This is	the receive interrupt for ethernet
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
static void
SPEAR_ETH_rx_int (unsigned long	data)
{
	struct sk_buff *skb;
	struct net_device *dev = (struct net_device *) data;
	unsigned int DmaStat;
	unsigned int len = 0;
	DmaMacDescr *DmaDesc_p;
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;

	unsigned long	flags;
	spin_lock_irqsave (&priv->eth_lock, flags);
	DmaDesc_p = &(priv->RX_MAC_DescrTable[priv->RX_MAC_currDescNum]);

	/* Read the Control and Status field of the current DMA descriptor */
	DmaStat = DmaDesc_p->TxRx_Status;

	spin_unlock_irqrestore (&priv->eth_lock, flags);
	/* Check if the owner	is the CPU */
	while	(!(DmaStat & DescOwnByDma))
	{
		if (!ETH_DmaRxValid (DmaStat))
		{
			priv->stats.rx_errors++;
			
			priv->stats.collisions += 
				ETH_SpearDmaRxCollisions (DmaStat);

			priv->stats.rx_crc_errors += 
				ETH_SpearDmaRxCrc (DmaStat);

			priv->stats.rx_frame_errors += 
				ETH_SpearDmaRxFrame (DmaStat);

			priv->stats.rx_length_errors += 
				ETH_SpearDmaRxLengthError (DmaStat);
		}
		else
		{
			len =	(DmaStat & DescFrameLengthMask)	>>
				DescFrameLengthShift;
			skb =	DmaDesc_p->skb;
		#ifdef ETH_DEBUG_SKB_RX_INT
			printk ("\nInside Receive tasklet: Dump the receive skb\n");
			SPEAR_ETH_dump_skb(skb, 1);
		#endif /*  */
			SPEAR_ETH_Skb_Prepare(dev, DmaDesc_p);
		
		#ifdef CONFIG_ETH_SPEAR_CHECKSUM_OFFLOAD
			#ifdef ETH_ARCH_SPEARBASIC_BA
				skb_put (skb,	len - 4);
				skb->protocol	= eth_type_trans (skb, dev);
				if (skb->protocol == __constant_htons	(ETH_P_IP))
				{
					if(!(DmaStat & 0x00000080))
						skb->ip_summed = CHECKSUM_UNNECESSARY;
				}
			#else
			skb_put (skb,	len - 6);
			skb->protocol	= eth_type_trans (skb, dev);
			if (skb->protocol == __constant_htons	(ETH_P_IP))
			{
				skb->ip_summed = CHECKSUM_COMPLETE;
				skb->csum = get_unaligned((u16 *)(skb->tail + 4));
				skb->csum = (~skb->csum) << 16;
			}
			#endif
		#else
			/* reserve two words used	by protocol	lay:ers	*/
				skb_put (skb, len - 4);
				skb->protocol = eth_type_trans(skb, dev);
		#endif
			consistent_sync(skb->data, len, DMA_FROM_DEVICE);
			dev->last_rx = jiffies;
			priv->stats.rx_packets++;
			priv->stats.rx_bytes += len;
			netif_rx (skb);
		}
		spin_lock_irqsave(&priv->eth_lock, flags);
		DmaDesc_p->TxRx_Status |= DescOwnByDma;	/* DMA owner */
		/* Test the wrap-around condition. */
		if (++priv->RX_MAC_currDescNum >=	RX_MAC_DESCR_NUM)
			priv->RX_MAC_currDescNum = 0;
		
		DmaDesc_p = &(priv->RX_MAC_DescrTable[priv->RX_MAC_currDescNum]);
		
		/* Read the Control and Status field of the current DMA descriptor */
		DmaStat = DmaDesc_p->TxRx_Status;
		spin_unlock_irqrestore (&priv->eth_lock, flags);
	}
#ifdef RCV_INTR_EN
	ETH_Dma_IntEnable();
	return;
#endif
  }

/*
 * NAME:  SPEAR_ETH_tx_int
 *
 * DESCRIPTION:
 *	spear tx interrupt
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
static void
SPEAR_ETH_tx_int (unsigned long	data)
{
	struct net_device *dev = (struct net_device *) data;
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	unsigned int DmaStat;
	u32 len;
	DmaMacDescr *prev_DmaDesc_p;
	unsigned long	flags;
	
	spin_lock_irqsave (&priv->eth_lock, flags);	
	prev_DmaDesc_p = &(priv->TX_MAC_DescrTable[priv->TX_MAC_prevDescNum]);

	/* Read the Control and Status field of the current DMA descriptor */
	DmaStat = prev_DmaDesc_p->TxRx_Status;

	spin_unlock_irqrestore (&priv->eth_lock, flags);	
	/* Check if the owner	is the CPU */
	while	(prev_DmaDesc_p->skb &&	!(DmaStat & DescOwnByDma))
	{
#ifdef ETH_DEBUG_TX_INT
		printk ("\n%s: tx	irq: TX	DMA Descriptor %i, status 0x%08x\n",
			dev->name, priv->TX_MAC_currDescNum, DmaStat);
#endif /*  */
		len = (prev_DmaDesc_p->DmaMac_Cntl & DescSize1Mask) >> DescSize1Shift;
		
		if (ETH_SpearDmaTxValid (DmaStat))
		{
			priv->stats.tx_bytes += len;
			priv->stats.tx_packets++;
		}
		else
		{
			priv->stats.tx_errors++;
			priv->stats.tx_aborted_errors += ETH_SpearDmaTxAborted (DmaStat);
			priv->stats.tx_carrier_errors += ETH_SpearDmaTxCarrier (DmaStat);
		}
		
		priv->stats.collisions +=	ETH_SpearDmaTxCollisions (DmaStat);
		dev_kfree_skb (prev_DmaDesc_p->skb);
		prev_DmaDesc_p->DmaMac_Addr = NULL;
		prev_DmaDesc_p->skb = NULL;

#ifdef SPR_DMA_IMPLICIT_CHAIN
		prev_DmaDesc_p->DmaMac_Addr1 = NULL;
#endif

		spin_lock_irqsave(&priv->eth_lock, flags);
		/* Test the wrap-around condition. */
		if (++priv->TX_MAC_prevDescNum >= TX_MAC_DESCR_NUM)
			priv->TX_MAC_prevDescNum = 0;
		++priv->TX_MAC_WinSize;
		prev_DmaDesc_p = &(priv->TX_MAC_DescrTable[priv->TX_MAC_prevDescNum]);
	
		/* Read the Control and Status field of the current DMA descriptor */
		DmaStat = prev_DmaDesc_p->TxRx_Status;
		spin_unlock_irqrestore (&priv->eth_lock, flags);
	}
	if (priv->TX_MAC_WinSize > 0)
		netif_wake_queue (dev);
}

/*
 * NAME:  SPEAR_ETH_int
 *
 * DESCRIPTION:
 *	spear  interrupt
 *
 * PARAMETERS:
 * IN  : irq:num of interrupt,dev_id: device id, pt_regs: ptr to registers
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/
static irqreturn_t
SPEAR_ETH_int (int irq,	void *dev_id, struct pt_regs *regs)
{
	struct net_device *dev = (struct net_device *) dev_id;
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	unsigned int_type = 0;

#ifdef ETH_DEBUG_INT
	printk ("\n%s: irq handler 0x%08x\n",	dev->name, irq_stat);
#endif /*  */
	
	int_type = ETH_DmaIntType ();
	while	(int_type != 0)
	{
		if (int_type & SpearDmaRxNormal)
		{
			tasklet_schedule (&priv->rx_tasklet);
		}
		
		if (int_type & SpearDmaTxNormal)
		{
			tasklet_schedule (&priv->tx_tasklet);
		}
		int_type = ETH_DmaIntType();	//look for any other interrupts	came while handling	ints   
	}
	return IRQ_HANDLED;
}

/*
 * NAME:  SPEAR_ETH_Start_Xmit
 *
 * DESCRIPTION:
 *    Starts Transmission of data
 *
 * PARAMETERS:
 * IN  : sk_buff*, net_device*
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
SPEAR_ETH_Start_Xmit (struct sk_buff *skb, struct net_device *dev)
{
	int len;
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	unsigned int DmaStat;
	DmaMacDescr *DmaDesc_p;
	unsigned char	*va_data;
	unsigned long	flags;

	spin_lock_irqsave (&priv->eth_lock, flags);
	len =	skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;
	dev->trans_start = jiffies;
	DmaDesc_p =
	  (DmaMacDescr *) & (priv->TX_MAC_DescrTable[priv->TX_MAC_currDescNum]);

	DmaStat = DmaDesc_p->TxRx_Status;
	if (DmaStat &	DescOwnByDma)
	{
		/* transmission	not possible if	valid bit is set for current Tx	descriptor
		means if the control is with DMA */
		spin_unlock_irqrestore (&priv->eth_lock, flags);
		return -EBUSY;
	}
#ifdef ETH_DEBUG_START_XMIT
	printk ("\n%s: hard_start_xmit: size(decimal)	%d, tx desc(decimal)%d\n",
	dev->name, skb->len, priv->TX_MAC_currDescNum);

#endif /*  */
	DmaDesc_p->skb = skb;

	spin_unlock_irqrestore (&priv->eth_lock, flags);
	
	/* No need of word alligning the data buffer in ETH, to be deleted later */
	va_data = (unsigned char *) ((u32) skb->data /* & ~0x3 */ );
	consistent_sync (va_data, len, DMA_TO_DEVICE);
	DmaDesc_p->DmaMac_Addr = (char *) (__virt_to_bus (va_data));

#ifdef SPR_DMA_IMPLICIT_CHAIN
	if (len > MAC_MAX_FRAME_SZ)
		DmaDesc_p->DmaMac_Addr1 =
		(char *) (__virt_to_bus (va_data + MAC_MAX_FRAME_SZ));
#endif

#ifdef ETH_DEBUG_START_XMIT
	printk ("\nskb_data=0X %x", skb->data);
	printk ("\nDmaDesc_p->DmaMac_Addr =0X	%x", DmaDesc_p->DmaMac_Addr);
#endif
	DmaDesc_p->DmaMac_Cntl = 0x00;
#ifdef ETH_ARCH_SPEARBASIC_BA
#ifdef JUMBO_FRAME_SUPPORT
	if (len > MAC_MAX_FRAME_SZ)
	{
		DmaDesc_p->DmaMac_Cntl |=
		((MAC_MAX_FRAME_SZ << DescSize1Shift) &	DescSize1Mask) |
		(((len - MAC_MAX_FRAME_SZ) << DescSize2Shift) &	DescSize2Mask);
	}
	else
	{
		DmaDesc_p->DmaMac_Cntl |=	((len << DescSize1Shift) & DescSize1Mask);
	}
	DmaDesc_p->TxRx_Status |= DescTxLast | DescTxFirst | DescTxIntEnable;
#else
	DmaDesc_p->DmaMac_Cntl |= ((len << DescSize1Shift) & DescSize1Mask);
	DmaDesc_p->TxRx_Status |=
	DescTxLast | DescTxFirst | DescTxIntEnable | DescTxChain;
#endif
	DmaDesc_p->TxRx_Status &= ~(DescTxStatusMask);
	DmaDesc_p->TxRx_Status |= DescOwnByDma;
#else
#ifdef JUMBO_FRAME_SUPPORT
	if (len > MAC_MAX_FRAME_SZ)
	{
		DmaDesc_p->DmaMac_Cntl |=
		((MAC_MAX_FRAME_SZ << DescSize1Shift) &	DescSize1Mask) |
		(((len -  MAC_MAX_FRAME_SZ) <<	DescSize2Shift)	& DescSize2Mask) |
		DescTxLast | DescTxFirst | DescTxIntEnable;
	}
	else
	{
		DmaDesc_p->DmaMac_Cntl |=
		((len << DescSize1Shift) & DescSize1Mask) | DescTxLast |
		DescTxFirst | DescTxIntEnable;
	}
#else
	DmaDesc_p->DmaMac_Cntl |=
	((len << DescSize1Shift) & DescSize1Mask) |	DescTxLast | DescTxFirst
	| DescTxIntEnable |	DescChain;
#endif
	DmaDesc_p->TxRx_Status = DescOwnByDma;	/* DMA owner */
#endif

#ifdef ETH_DEBUG_START_XMIT
	printk ("\nDmaDesc_p->TxRxStatus =0X %x", DmaDesc_p->TxRx_Status);
	printk ("\nDmaDesc_p->DmaMac_Cntl =0X	%x", DmaDesc_p->DmaMac_Cntl);
#endif
  
	/* Test the wrap-around condition. */
	spin_lock_irqsave (&priv->eth_lock, flags);
	
	/* Increment the current descriptor number and test condition */
	if (++(priv->TX_MAC_currDescNum) >= TX_MAC_DESCR_NUM)
#ifdef SPR_DMA_IMPLICIT_CHAIN
	{
		priv->TX_MAC_currDescNum = 0;
	#ifdef ETH_ARCH_SPEARBASIC_BA
		DmaDesc_p->TxRx_Status |=	DescTxEndOfRing;
	#else
		DmaDesc_p->DmaMac_Cntl |=	DescEndOfRing;
	#endif
	}
#else
	priv->TX_MAC_currDescNum = 0;
#endif
	
	if (--priv->TX_MAC_WinSize <=	0)
	{
		/* Stop	network	interface if all desccriptors are busy */
		priv->TX_MAC_WinSize = 0;
		netif_stop_queue (dev);
	}
	
	/* Resume the suspended	transmission, if it is done before wrap-around condition, 
	   interrupt can occur in b/w (in case of short frame transmission) &
   	   update the variables(TX_MAC_WinSize) */
	ETH_TxResume ();
	
	spin_unlock_irqrestore (&priv->eth_lock, flags);
	return 0;
}

#ifdef NO_INTERRUPT_COMMENT
static struct irqaction	__irqaction = {	name: "eth", flags: SA_INTERRUPT, handler:SPEAR_ETH_int,
};
#endif /*  */

/*
 * NAME:  SPEAR_ETH_Reset
 *
 * DESCRIPTION:
 *    Resets the ethernet
 *
 * PARAMETERS:
 * IN  : eth_priv*
 *
 * OUT :
 *	None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/
void
SPEAR_ETH_Reset	(struct	eth_priv *adapter)
{
//	struct net_device	*dev = adapter->dev;

#ifdef ETH_DEBUG_OPEN
	//DEBUGFUNC ("%s:	open\n", dev->name);
	printk ("INSIDE SPEAR	RESET Function");
#endif
	/*
	* install ISR
	*/
	ETH_Reset ();
	ETH_PHY_POWER_DOWN (Phy_num);	//@@--check

	/* Reset the counters	*/
	adapter->RX_MAC_currDescNum =	0;
	adapter->TX_MAC_currDescNum =	0;
	adapter->TX_MAC_prevDescNum =	0;
	adapter->TX_MAC_WinSize = TX_MAC_DESCR_NUM;
}

/*
 * NAME:  SPEAR_ETH_Down
 *
 * DESCRIPTION:
 *    Stops the	transmission of	data
 *
 * PARAMETERS:
 * IN  : eth_priv*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/
void
SPEAR_ETH_Down (struct eth_priv	*adapter)
{
	DEBUGOUT1 ("%s: stop\n", adapter->dev->name);
	del_timer_sync (&adapter->tmr_hotplug);
	netif_carrier_off (adapter->dev);
	netif_stop_queue (adapter->dev);
	SPEAR_ETH_Reset (adapter);
	free_irq (adapter->dev->irq, adapter->dev);
	SPEAR_ETH_Free_Tx_Descrs (adapter->dev);
	SPEAR_ETH_Free_Rx_Descrs (adapter->dev);
	tasklet_kill (&adapter->rx_tasklet);
	tasklet_kill (&adapter->tx_tasklet);
}

/*
 * NAME:  SPEAR_ETH_Find_Phy
 *
 * DESCRIPTION:
 *    Stops the	transmission of	data
 *
 * PARAMETERS:
 * IN  : eth_priv*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

u8
SPEAR_ETH_Find_Phy (struct eth_priv *adapter)
{
	u8 PhyAddr = 0;
	u16 dataW = 0x8000;
	u16 DATA = 0;
	do
	{
		DATA = ETH_Mdio_Read (PhyAddr, Phy_CtrlReg);
		dataW = DATA & 0x1000;
		DATA ^= 0x1000;		/* invert auto_neg */
		ETH_Mdio_write (PhyAddr, Phy_CtrlReg, DATA);
		DATA = ETH_Mdio_Read (PhyAddr, Phy_CtrlReg);
		if ((DATA	& 0x1000) == dataW)
		{
			PhyAddr++;
		}
		else
		{
			DATA ^= 0x1000;
			ETH_Mdio_write (PhyAddr, Phy_CtrlReg,	DATA);
			if (!SPEAR_MII_phy_probe (&adapter->phy, PhyAddr))
			{
			/* Init PHY */
				if (adapter->phy.def->ops->init)
					adapter->phy.def->ops->init (&adapter->phy);
				break;
			}
		}
	} while	(PhyAddr < 32);
	return PhyAddr;
}

#ifdef JUMBO_FRAME_SUPPORT

/*
 * NAME:  SPEAr_ETH_change_mtu
 *
 * DESCRIPTION:
 *    This routine is used to configure	the MTU	size from
 *    the user prompt
 *
 * PARAMETERS:
 * IN  : net device pointer, mtu size
 *
 * OUT :
 *	  Success/Failure
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

static int
SPEAr_ETH_change_mtu (struct net_device	*dev, int new_mtu)
{
	struct eth_priv *adap	= (struct eth_priv *) dev->priv;

	if (new_mtu <	SPEAR_MIN_MTU || new_mtu > SPEAR_MAX_MTU)
		return -EINVAL;
	dev->mtu = new_mtu;
	adap->rx_skb_size = spear_eth_rx_skb_size (new_mtu);
	adap->rx_sync_size = spear_eth_rx_sync_size (new_mtu);
	printk ("rx skb size is %d \n", adap->rx_skb_size);
	//local_bh_disable();
	if (netif_running (dev))
	{
		SPEAR_ETH_Down (adap);
		if(SPEAR_ETH_Up (adap) < 0)
			SPEAR_ETH_Down (adap);
	}
	return 0;
}

#endif

/*
 * NAME:  SPEAr_ETH_Tmr_Hotplug_Fun
 *
 * DESCRIPTION:
 *    This routine is scheduled	to run every HOTPLUG_TMOUT system ticks.
 * It checks the link status
 *
 * PARAMETERS:
 * IN  : data
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
SPEAr_ETH_Tmr_Hotplug_Fun (unsigned long data)
{
	struct net_device *dev = (struct net_device *) data;
	struct eth_priv *adapter = (struct eth_priv *) dev->priv;
	eth_config ether_config = { 0, 0, 0 };
	int link_poll_interval = 0;
	if (adapter->phy.def->ops->poll_link (&adapter->phy))
	{
		if (!netif_carrier_ok (dev))
		{
			/* Get new link parameters */
			printk (" in timer getting link up \n");
			adapter->phy.def->ops->read_link (&adapter->phy);
			SPEAr_MAC_Cfg	(&ether_config,	adapter);
			ETH_Mac_Init (&ether_config);
			netif_carrier_on (dev);
		}
		link_poll_interval = PHY_POLL_LINK_ON;
	}
	else
	{
		if (netif_carrier_ok (dev))
		{
			printk (" in timer getting link down \n");
			netif_carrier_off (dev);
		}
		link_poll_interval = PHY_POLL_LINK_OFF;
	}
	mod_timer (&((struct eth_priv	*) dev->priv)->tmr_hotplug,
	jiffies + link_poll_interval);
}

/*
 * NAME:  SPEAr_MAC_Cfg
 *
 * DESCRIPTION:
 *
 * PARAMETERS:
 * IN  : Ether Config
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
SPEAr_MAC_Cfg (eth_config * ether_config, struct eth_priv *adap)
{
	struct net_device *dev = adap->dev;
	if (adap->phy.duplex == DUPLEX_FULL)
		ether_config->dplx = FULL_DUPLEX;
	else
		ether_config->dplx = HALF_DUPLEX;

	if (adap->phy.speed == SPEED_1000)
		ether_config->speed_1000 = 1;
	else if ((adap->phy.speed == SPEED_100))
		ether_config->speed_100 = 1;

	if (ether_config->speed_100 == 1)
		printk ("\nspeed 100 is set	");

	if (ether_config->speed_1000 == 1)
		printk ("\nspeed 1000 is set");

	if (adap->phy.pause == 1)
	{
		ether_config->rx_pause = 1;
		ether_config->tx_pause = 1;
	}
	else if ((adap->phy.asym_pause == 1))
		ether_config->tx_pause = 1;
	else
	{
		ether_config->rx_pause = 0;
		ether_config->tx_pause = 0;
	}

	if (dev->mtu > ETH_DATA_LEN)
	{
		ether_config->jumbo_frame	= 1;
		printk ("	jumbo frame is set \n");
	}
	else
		ether_config->jumbo_frame =	0;
	printk ("\nduplex mode:%s\n",	ether_config->dplx ? "Full" : "Half");
}

/*
 * NAME:  SPEAR_ETH_Up
 *
 * DESCRIPTION:
 *    Called from open() function to start the ethernet	with new settings
 *
 * PARAMETERS:
 * IN  : eth_priv*
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  int
 *
 * REENTRANCY: NA
*/

int
SPEAR_ETH_Up (struct eth_priv *adapter)
{
	unsigned long	status;
	eth_config ether_config = { 0, 0, 0 };
	struct net_device *dev = adapter->dev;
	u32 addr_hi, addr_lo;
	int i;
	int link_poll_interval = 0;
	
#ifdef ETH_DEBUG_OPEN
	printk ("%s: open\n",	dev->name);
#endif
/*
 * install ISR
*/
	status = request_irq (dev->irq, (void	*) &SPEAR_ETH_int, 0, dev->name, dev);
	if (status)
	{
		printk (KERN_ERR "%s: unabled to hook irq %d for ethernet\n",
			dev->name, dev->irq);
		return status;
	}
	Phy_num = SPEAR_ETH_Find_Phy (adapter);
	printk ("\n Phy num is %x", Phy_num);

	/* Setup initial link parameters */
	if (adapter->phy.features & SUPPORTED_Autoneg)
	{
		u32 adv;
		adv = adapter->phy.features;
		adv |= ADVERTISED_Pause |	ADVERTISED_Asym_Pause;
		/* Restart autonegotiation */
		adapter->phy.def->ops->setup_aneg	(&adapter->phy,	adv);
	}
	else
	{
		u32 f = adapter->phy.def->features;
		int speed	= SPEED_10, fd = DUPLEX_HALF;

	  /* Select highest supported speed/duplex */
		if (f & SUPPORTED_1000baseT_Full)
		{
			speed	= SPEED_1000;
			fd = DUPLEX_FULL;
		}
		else if (f & SUPPORTED_1000baseT_Half)
			speed =	SPEED_1000;
		else if (f & SUPPORTED_100baseT_Full)
		{
			speed	= SPEED_100;
			fd = DUPLEX_FULL;
		}
		else if (f & SUPPORTED_100baseT_Half)
			speed =	SPEED_100;
		else if (f & SUPPORTED_10baseT_Full)
			fd = DUPLEX_FULL;
		
		/* Force link parameters */
		adapter->phy.def->ops->setup_forced (&adapter->phy, speed, fd);
	}

	for (i = 0; i	< 100; i++)
	{
		if ((ETH_Mdio_Read (Phy_num, Phy_StatReg)	& Phy_AutoNego_Done) !=	0)
		{
			printk ("auto-negotiation complete\n");
			break;
		}
		mdelay (20);
	}

	ETH_Mdio_Read	(Phy_num, MII_BMSR);
	if ((ETH_Mdio_Read (Phy_num, MII_BMSR) & BMSR_LSTATUS) == 0)
	{
		printk ("	no link\n");
	}
	else
		printk (" -	link up\n");

	local_bh_disable ();
	if ((adapter->phy.address < 32))
	{
		int link_poll_interval;
		if (adapter->phy.def->ops->poll_link (&adapter->phy))
		{
			adapter->phy.def->ops->read_link (&adapter->phy);
			netif_carrier_on (dev);
			link_poll_interval = PHY_POLL_LINK_ON;
		}
		else
		{
			netif_carrier_off (dev);
			link_poll_interval = PHY_POLL_LINK_OFF;
		}
	}
	else
	{
	//	free_irq (dev->irq, dev);
		local_bh_enable ();
		return -1;
	}

	if (ETH_Reset	() < 0)
	{
		printk ("reset fail \n");
		local_bh_enable ();
		return -1;
	}
	// Program the MAC Dst Address
	addr_hi = dev->dev_addr[4] | (dev->dev_addr[5] << 8);;
	addr_lo = dev->dev_addr[0] | (dev->dev_addr[1] << 8) | (dev->dev_addr[2] << 16) |
		(dev->dev_addr[3] << 24);
	ETH_Set_Mac_Address (addr_hi,	addr_lo);
	/* Setup the bottom half rx ring refill handler
	*/
	tasklet_init (&((struct eth_priv *) dev->priv)->rx_tasklet,
	SPEAR_ETH_rx_int, (unsigned long) dev);
	tasklet_init (&((struct eth_priv *) dev->priv)->tx_tasklet,
	SPEAR_ETH_tx_int, (unsigned long) dev);

	SPEAr_MAC_Cfg	(&ether_config,	adapter);
	SPEAR_ETH_Init_Tx_Descrs (dev);	
	SPEAR_ETH_Init_Rx_Descrs (dev);	
	ETH_Dma_Init (&ether_config);
	*Gmac_MMC_RxIntrMask = 0x00ffffff;
	*Gmac_MMC_TxIntrMask = 0x00ffffff;
	ETH_Mac_Init (&ether_config);

  /*
   * Setup the hotplugging timer routine
   */
	init_timer (&((struct	eth_priv *) dev->priv)->tmr_hotplug);
	((struct eth_priv *) dev->priv)->tmr_hotplug.data = (unsigned	long) dev;
	((struct eth_priv *) dev->priv)->tmr_hotplug.function	= &SPEAr_ETH_Tmr_Hotplug_Fun;	/* timer handler */

	mod_timer (&((struct eth_priv	*) dev->priv)->tmr_hotplug,
	jiffies + link_poll_interval);
	ETH_Rxstart ();		/* Start reception */
	ETH_Txstart ();		/* Start Transmission */
	netif_start_queue (dev);
	local_bh_enable ();
	return 0;
}

/*
 * NAME:  SPEAR_ETH_Open
 *
 * DESCRIPTION:
 *    Resets the ethernet
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
SPEAR_ETH_Open (struct net_device *dev)
{
	struct eth_priv *adapter;
	adapter = (struct eth_priv *)	dev->priv;

	DEBUGOUT1 ("%s: open\n", dev->name);
	if (SPEAR_ETH_Up (adapter) < 0)
	{
		printk ("SPEAR_ETH_Up failed \n");
		SPEAR_ETH_Down (adapter);
		return 1;
	}
	return 0;
}

/*
 * NAME:  SPEAR_ETH_Stop
 *
 * DESCRIPTION:
 *    Stops the	ethernet
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
SPEAR_ETH_Stop (struct net_device *dev)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;

#ifdef ETH_DEBUG_STOP
	printk ("%s: stop\n",	dev->name);

	/*SPEAR_ETH_dump_dma_descriptors(dev); */
#endif /*  */
	SPEAR_ETH_Down (priv);
	return 0;
}

/*
 * NAME:  SPEAR_ETH_Get_Stats
 *
 * DESCRIPTION:
 *    Gets the stats of	ethernet
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
struct net_device_stats	*
SPEAR_ETH_Get_Stats (struct net_device *dev)
{
#ifdef ETH_DEBUG_STAT
	printk ("%s: get_stats\n", dev->name);
	printk
	("TxPackets=%d RxError=%d RxDropped=%d RxFrmErr=%d RxCollisions=%d\n",
	((struct eth_priv *) dev->priv)->stats.rx_packets,
	((struct eth_priv *) dev->priv)->stats.rx_errors,
	((struct eth_priv *) dev->priv)->stats.rx_dropped,
	((struct eth_priv *) dev->priv)->stats.rx_frame_errors,
	((struct eth_priv *) dev->priv)->stats.collisions);
	printk ("RxCrc=%d RxMissed=%d	RxLenErr=%d \n",
	((struct eth_priv *) dev->priv)->stats.rx_crc_errors,
	((struct eth_priv *) dev->priv)->stats.rx_missed_errors,
	((struct eth_priv *) dev->priv)->stats.rx_length_errors);
	printk
	("TxPackets=%d TxError=%d txDropped=%d TxCarrier=%d	RxCollisions=%d\n",
	((struct eth_priv *) dev->priv)->stats.tx_packets,
	((struct eth_priv *) dev->priv)->stats.tx_errors,
	((struct eth_priv *) dev->priv)->stats.tx_dropped,
	((struct eth_priv *) dev->priv)->stats.tx_carrier_errors,
	((struct eth_priv *) dev->priv)->stats.collisions);
	printk ("TxAborted=%d	\n",
	((struct eth_priv *) dev->priv)->stats.tx_aborted_errors);

#endif /*  */
	return &((struct eth_priv *) dev->priv)->stats;
}

/*
 * NAME:		 SPEAR_ETH_Tx_Timeout
 *
 * DESCRIPTION:
 * Time	out fun	called when there is a transmission time out by	higher layers
 * PARAMETERS:
 *
 * IN  : net_device*,struct ifreq*,int cmd
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
SPEAR_ETH_Tx_Timeout (struct net_device	*ndev)
{

#ifdef ETH_DEBUG_TIMEOUT
	printk ("%s: tx_timeout\n", ndev->name);
#endif /*  */

	eth_config ether_config={0,0,0};
	u32 addr_hi,addr_lo ;
	struct eth_priv *dev = (struct eth_priv *) ndev->priv;
	
	ETH_Dma_IntDisable();
	tasklet_disable(&dev->tx_tasklet);
	tasklet_disable(&dev->rx_tasklet);
	ETH_Reset();
	SPEAR_ETH_Free_Tx_Descrs(ndev);
	SPEAR_ETH_Free_Rx_Descrs(ndev);
	dev->TX_MAC_currDescNum =	0;
	dev->TX_MAC_prevDescNum =	0;
	dev->RX_MAC_currDescNum =	0;
	dev->TX_MAC_WinSize 	= TX_MAC_DESCR_NUM;
	addr_hi = ndev->dev_addr[4] | (ndev->dev_addr[5] << 8);;
	addr_lo = ndev->dev_addr[0] | (ndev->dev_addr[1] << 8) | (ndev->dev_addr[2] << 16) | (ndev->dev_addr[3] << 24);;
	ETH_Set_Mac_Address(addr_hi,addr_lo);
	SPEAR_ETH_Init_Tx_Descrs(ndev);	
	SPEAR_ETH_Init_Rx_Descrs(ndev);	
	ETH_Dma_Init(&ether_config);
	*Gmac_MMC_RxIntrMask=0x00ffffff;		
	*Gmac_MMC_TxIntrMask=0x00ffffff;
	tasklet_enable(&dev->tx_tasklet);
	tasklet_enable(&dev->rx_tasklet);
	netif_wake_queue(ndev);
	ETH_Rxstart();			/* Start reception */
	ETH_Txstart();			/* Start Transmission */
}

/*
 * NAME:	      SPEAR_ETH_ioctl
 *
 * DESCRIPTION:
 *
 * implements the ioctl	for ethernet
 * PARAMETERS:
 * IN  : net_device*,struct ifreq*,int cmd
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
SPEAR_ETH_ioctl	(struct	net_device *dev, struct	ifreq *rq, int cmd)
{
	struct eth_priv *priv	= (struct eth_priv *) dev->priv;
	struct mii_ioctl_data	*data =	(struct	mii_ioctl_data *) &rq->ifr_data;
	u32 frm_len;
	static char frm_addr[6] = { 0x00, 0x00, 0x00,	0x00, 0x00, 0x00 };
	int err;

#ifdef ETH_DEBUG_IOCTL
	printk (KERN_INFO "%s: do_ioctl\n", dev->name);

#endif /*  */

#ifdef ETH_SPEAR_TEST
	switch (cmd)
	{
		case SIOCDEVPRIVATE:	/* Store the Ethernet Address */
		{
			frm_addr[0] = rq->ifr_ifru.ifru_slave[0];
			frm_addr[1] = rq->ifr_ifru.ifru_slave[1];
			frm_addr[2] = rq->ifr_ifru.ifru_slave[2];
			frm_addr[3] = rq->ifr_ifru.ifru_slave[3];
			frm_addr[4] = rq->ifr_ifru.ifru_slave[4];
			frm_addr[5] = rq->ifr_ifru.ifru_slave[5];
		}
			return 0;
		case SIOCDEVPRIVATE	+ 1:	/* Send	1 frame	of 2K bytes. */
		{
			struct sk_buff *skb;
#ifdef ETH_DEBUG_IOCTL
			printk (KERN_INFO "%s: do SIOCDEVPRIVATE\n", dev->name);
#endif /*  */
			frm_len	= (u32)	rq->ifr_ifru.ifru_ivalue;
			/* ALLOC SKB */
			if (!(skb = dev_alloc_skb (frm_len)))
			{
#ifdef ETH_DEBUG_IOCTL
				printk ("%s: unable	to allocate skb...", dev->name);
#endif /*  */
				return ENOMEM;
			}
			skb->dev = dev;
			skb->len = frm_len;
			memcpy (skb->data, frm_addr, 6);
			SPEAR_ETH_Start_Xmit (skb, dev);
		}
			return 0;
		default:
			break;
	}

#endif /*  */
	spin_lock_irq(&priv->eth_lock);
	err = generic_mii_ioctl (&priv->mii, data, cmd, NULL);
	spin_unlock_irq(&priv->eth_lock);

	return err;
	
}


static void
mii_mdio_write(struct net_device *dev, int phy_id, int location, int val)
{
	struct eth_priv *priv;
	
	priv = netdev_priv(dev);
	ETH_Mdio_write(phy_id, location, val); 


}

/*
 * mdio_read: read a single MII register.
 *
 * INPUTS:  dev         = network device
 *          phy_id      = MII PHY address to read
 *          location    = MII register offset
 * RETURNS: value
 */
static int
mii_mdio_read(struct net_device *dev, int phy_id, int location)
{
	
	struct eth_priv *priv;
	u16 data;
	
	priv = netdev_priv(dev);
	data = ETH_Mdio_Read(phy_id, location); 

        return data;
}


/*
 * NAME:  SPEAR_ETH_Init
 *
 * DESCRIPTION:
 *  initializes	the ethernet driver
 *  The	init function, invoked by register_netdev(
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
SPEAR_ETH_Init (struct net_device *dev)
{
	struct eth_priv *priv;
	unsigned int value;
	int err;

	printk ("%s: net_device init:	%04x%08x\n", dev->name,	*Gmac_Addr0HI,*Gmac_Addr0LO);
	
	/* Extract priv data structure */
	priv = netdev_priv(dev);
	memset(priv, 0, sizeof(struct eth_priv));

	/* Assign some of the fields */
	ether_setup (dev);

  	/* Assign net_device methods */
	dev->open = SPEAR_ETH_Open;
	dev->stop = SPEAR_ETH_Stop;
	dev->do_ioctl	= SPEAR_ETH_ioctl;
	dev->get_stats = SPEAR_ETH_Get_Stats;
	dev->tx_timeout = SPEAR_ETH_Tx_Timeout;
	dev->hard_start_xmit = SPEAR_ETH_Start_Xmit;
#ifdef JUMBO_FRAME_SUPPORT
	dev->change_mtu = &SPEAr_ETH_change_mtu;
#endif

#ifdef SPEAr_ETHTOOL
	SPEAR_ETHTOOL_set_opts (dev);
#endif
	DEBUGFUNC ("inside SPEAr init\n");

#ifdef CONFIG_ARCH_SPEARPLUS
	dev->irq = IRQ_GMAC_2	/*IRQ_MAC */ ;
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
	dev->irq = IRQ_MAC_2 /*IRQ_MAC */ ;
#endif
	dev->tx_queue_len = TX_MAC_DESCR_NUM;
	dev->dma = 0;
	dev->watchdog_timeo =	5*HZ;
	value	= ETH_Read_Mac_Address_Lo ();
	dev->dev_addr[0] = (value & 0x000000FF);
	dev->dev_addr[1] = (value & 0x0000FF00) >> 8;
	dev->dev_addr[2] = (value & 0x00FF0000) >> 16;
	dev->dev_addr[3] = (value & 0xFF000000) >> 24;
	value	= ETH_Read_Mac_Address_Hi ();
	dev->dev_addr[4] = (value & 0x000000FF);
	dev->dev_addr[5] = (value & 0x0000FF00) >> 8;

	SET_MODULE_OWNER (dev);

	priv->dev = dev;
	printk ("Default Mtu size is %d \n", dev->mtu);
	priv->rx_ring_count =	(uint32_t) SPEAR_DEFAULT_TXD;
	priv->tx_ring_count =	(uint32_t) SPEAR_DEFAULT_RXD;

#ifdef JUMBO_FRAME_SUPPORT
	priv->rx_skb_size = spear_eth_rx_skb_size (dev->mtu);
	printk ("rx skb size is %d \n", priv->rx_skb_size);
	priv->rx_sync_size = spear_eth_rx_sync_size (dev->mtu);
#endif

	priv->phy.speed = SPEED_100;
	priv->phy.duplex = DUPLEX_FULL;
	priv->phy.autoneg = AUTONEG_DISABLE;
	priv->phy.pause = priv->phy.asym_pause = 0;
	priv->autoneg_reset =	0;
	priv->phy.mdio_read =	ETH_Mdio_Read;
	priv->phy.mdio_write = ETH_Mdio_write;

 	priv->mii.dev = dev;
        priv->mii.mdio_read = mii_mdio_read;
        priv->mii.mdio_write = mii_mdio_write;
        priv->mii.phy_id_mask = 0x1f;
        priv->mii.reg_num_mask = 0x1f;


	Phy_num = SPEAR_ETH_Find_Phy (priv);
	if (Phy_num >	31)
		return -1;
	
	priv->mii.phy_id = Phy_num;

	/* Everything is up, register netdev now */
	err = register_netdev(dev);
	if (err)
		goto fail;

	netif_carrier_off (dev);
	netif_stop_queue (dev);
	/*  initializing the priv structure */
	spin_lock_init (&priv->eth_lock);

	return 0;
	
  fail:
	free_netdev(dev);
	return err;

}

static struct net_device *this_device;

/*
 * NAME:	   SPEAR_ETH_Init_module
 *
 * DESCRIPTION:
 *	This routine registers the Ethernet Structure
 *
 * PARAMETERS:
 * IN  :
 * OUT : None
 *
 * RETURN:
 *
 * REENTRANCY: NA
*/

static int __init
SPEAR_ETH_Init_module (void)
{
	int status = 0;
	struct net_device *spear_netdevs;
	
	printk(KERN_INFO "%s\n", __DRIVER_NAME);
	
	/* Allocate and set up an ethernet device */
	spear_netdevs = alloc_etherdev(sizeof(struct eth_priv));
	if (!spear_netdevs)
		return -ENOMEM;
	
	status = SPEAR_ETH_Init(spear_netdevs);
	if(status != 0)
	{
		printk("SPEAR eth: Error %i registering ETH driver\n", status);
		return -ENXIO;
	}
	else
	{
		this_device = spear_netdevs;
		return status;
	}

}

static void __exit
SPEAR_ETH_Cleanup (void)
{
	unregister_netdev(this_device);
	free_netdev(this_device);
	return;
}

/*
 * NAME:SPEAr_get_link_status
 *
 * DESCRIPTION:
 *    get the link status
 *
 * PARAMETERS:
 * IN  : net_device* dev
 *
 * OUT :
 *	  None
 *
 * RETURN:
 *	  void
 *
 * REENTRANCY: NA
*/

uint32_t
SPEAr_get_link_status (struct net_device * dev)
{
	uint32_t mii_status;
	ETH_Mdio_Read(Phy_num, MII_BMSR); 
	mii_status     =	ETH_Mdio_Read(Phy_num, MII_BMSR);     
	return (mii_status     &	BMSR_LSTATUS);
}

module_init (SPEAR_ETH_Init_module);
module_exit (SPEAR_ETH_Cleanup);
#ifdef CONFIG_ARCH_SPEARPLUS
MODULE_DESCRIPTION ("ST	SPEARPLUS ethernet driver");
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
MODULE_DESCRIPTION ("ST	SPEARBASIC ethernet driver");
#endif

MODULE_LICENSE ("GPL");
EXPORT_SYMBOL(SPEAR_ETH_Dump_Registers);
EXPORT_SYMBOL(g_spear_driver_name);
EXPORT_SYMBOL(g_spear_driver_version);
