/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2009 by Bhupesh Sharma  				*/
/*   bhupesh.sharma@st.com								*/
/*											*/
/*   This program is free software; you can redistribute it and/or modify		*/
/*   it under the terms of the GNU General Public License as published by		*/
/*   the Free Software Foundation; either version 2 of the License, or			*/
/*   (at your option) any later version.						*/
/*											*/
/*   This program is distributed in the hope that it will be useful,			*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 			*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*/
/*   GNU General Public License for more details.					*/
/*											*/
/*   You should have received a copy of the GNU General Public License			*/
/*   along with this program; if not, write to the					*/
/*   Free Software Foundation, Inc.,							*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.				*/
/********************************************************************************/

/* Necessary Includes */
/*********************/
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
#include "spr_870_eth_phy.h"
#include "spr_870_eth_syn.h"
#include "rtl8363_asicdrv.h"

/* Definitions and Macros */
/**************************/
//#define ETH_DEBUG_TX_INT
//#define ETH_DEBUG_START_XMIT
//#define ETH_DEBUG_SKB_RX_INT
//#undef ETH_DEBUG_TX_INT
//#undef ETH_DEBUG_START_XMIT
#undef ETH_DEBUG_STAT
#undef ETH_DEBUG_IOCTL
#define ETH_SPEAR_TEST
//#define	SPEAr_ETHTOOL

//FIXME-ETH_SWITCH: Define at a proper place
#define P0_ENABLE 

#define printk(fmt...) 
static void SPEAr_ETH_Tmr_Hotplug_Fun (unsigned long data);
static void SPEAr_MAC_Cfg(eth_config  *ether_config,struct eth_priv *adap);

#ifdef CONFIG_ARCH_SPEARPLUS
char            spear_rtl_driver_name[] = "spearplus";
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
char            spear_rtl_driver_name[] = "spearbasic";
#endif

char            spear_driver_string[] = "ST(R) Network Driver";
char            spear_rtl_driver_version[] = "1.1.0.";
char            spear_copyright[] = "Copyright (c) 2005-2009 ST";
int             SPEAR_ETH_Up(struct eth_priv *adapter);
void            SPEAR_ETH_Down(struct eth_priv *adapter);

#ifdef SPEAr_ETHTOOL
extern void set_ethtool_ops (struct net_device *netdev);
#endif

#ifdef CONFIG_ARCH_SPEARPLUS
#define __DRIVER_NAME	"ST SPEARPLUS Ethernet Driver version 0.1"
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define __DRIVER_NAME	"ST SPEARBASIC Ethernet Driver version 0.1"
#endif

static u8       Phy_num = 0;
static u8       isCpuPortTaggingEnabled = 1; /* enabled by default */

/****************************************************************************/
/* NAME:  SPEAR_RTL_ETH_Dump_Registers    		                                 */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine dumps registers                                        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                         */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_RTL_ETH_Dump_Registers(struct net_device *dev)
{
	int            *p;
	int             i;
	
	char           *register_names_DMA[DMA_REG_COUNT] =
	{ "DMA Bus Mode Register ", "Transmit POLL Demand Reg ", "Receive PLL Demand Reg",
		"Receive Descriptor List Address Reg", "transmit Descriptor List Address Reg", "Status Register", "Operation mode Register",
		"Interrupt Enable Reg",
		"Missed frame & Buffer OV Counter Reg", "Reserved",	/* Location offset 0x1024 */
		"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",	/* Location offset 0x1044 */
		"Current Host Transmit Descriptor Reg ", "Current Host Receive Descriptor Reg ",
		"Current Host transmit buffer Address Reg", "Current Host Receive Buffer Address Reg",
	};
    char           *register_names_MAC[MAC_REG_COUNT] =
	{ "ETH Control Reg", "ETH Frame Filter", "Hash Table High Register", "Hash Table Low Register",
		"GMII Address Register", "GMII Data Regsiter", "Flow Control Register", "VLAN Tag Register", "Version register", "Reserved",
		"Wake up frame filter pointer",
		"PMT Control & Status Register", "Reserved",	/* Location offset 0x0030 */
		"Reserved", "Reserved", "Reserved",	/* Locatino offset 0x003C */
		"MAC Address0 High Reg ", "MAC Address0 Low Reg", "MAC Address1 High Reg ", "MAC Address1 Low Reg",
		"MAC Address2 High Reg ", "MAC Address2 Low Reg", "MAC Address3 High Reg ", "MAC Address3 Low Reg",
		"MAC Address4 High Reg ", "MAC Address4 Low Reg", "MAC Address5 High Reg ", "MAC Address5 Low Reg",
		"MAC Address6 High Reg ", "MAC Address6 Low Reg", "MAC Address7 High Reg ", "MAC Address7 Low Reg",
		"MAC Address8 High Reg ", "MAC Address8 Low Reg", "MAC Address9 High Reg ", "MAC Address9 Low Reg",
		"MAC Address10 High Reg ", "MAC Address10 Low Reg", "MAC Address11 High Reg ", "MAC Address11 Low Reg",
		"MAC Address12 High Reg ", "MAC Address12 Low Reg", "MAC Address13 High Reg ", "MAC Address13 Low Reg",
		"MAC Address14 High Reg ", "MAC Address14 Low Reg", "MAC Address15 High Reg ", "MAC Address15 Low Reg",
	};
    (void)register_names_DMA;(void)register_names_MAC;

	p = (int *) DmaMac_BusMode;
	printk("\tDMA Registers\n");
	printk("-----------------------\n");
	for (i = 0; i < DMA_REG_COUNT; i++, p++)
		printk("%s:       0x%08x\n", register_names_DMA[i], *p);
	printk("\n");
	p = (int *) Gmac_Ctrl;
	printk("\tMAC Registers\n");
	printk("-----------------------\n");
	for (i = 0; i < MAC_REG_COUNT; i++, p++)
		printk("%s:       0x%08x\n", register_names_MAC[i], *p);
	printk("\n");
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_dump_dma_descriptors                          */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine dumps dma descriptors                                */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                         */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_ETH_dump_dma_descriptors(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    *DmaDesc_p;
	int             i;
	printk("\nTX descriptors [status   cntl     addr    next    skb]\n");
	DmaDesc_p = (DmaMacDescr *) & (priv->TX_MAC_DescrTable);
	for (i = 0; i < TX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		printk("%d %08x = %08x %08x %08x %08x %08x\n", i,
				(int) DmaDesc_p, DmaDesc_p->TxRx_Status, DmaDesc_p->DmaMac_Cntl,
				(int) DmaDesc_p->DmaMac_Addr, DmaDesc_p->DmaMac_Next, (int) DmaDesc_p->skb);
	printk("\nRX descriptors [status  cntl     addr  	  next    skb]\n");
	DmaDesc_p = (DmaMacDescr *) & (priv->RX_MAC_DescrTable);
	for (i = 0; i < RX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		printk("%d %08x = %08x %08x %08x %08x %08x\n", i,
				(int) DmaDesc_p, DmaDesc_p->TxRx_Status, DmaDesc_p->DmaMac_Cntl,
				(int) DmaDesc_p->DmaMac_Addr, DmaDesc_p->DmaMac_Next, (int) DmaDesc_p->skb);
}

/****************************************************************************/
/* NAME:			SPEAR_ETH_dump_skb                          */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine dumps skb                                */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : sk_buff*, int contents                                                         */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_ETH_dump_skb(struct sk_buff *skb, int contents)
{
	int             len;

	printk
		("skb: %08x, head: %08x, data: %08x, len: %08x, tail: %08x, end: %08x\n",
		 (int) (skb), (int) skb->head, (int) skb->data, (int) skb->len, (int) skb->tail, (int) skb->end);
	if (contents)

	{
		for (len = 0; len < skb->len; len++)

		{
			if (!(len & 15))
				printk("\n%04x: ", len);
			printk("%02x ", ((char *) skb->data)[len]);
		} printk("\n");
	}
}

/****************************************************************************/
/* NAME:		  SPEAR_ETH_dump_dma_skb                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine dumps skb                                              */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_ETH_dump_dma_skb(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    *DmaDesc_p;
	int             i;
	printk("RX descriptors sockets\n");
	DmaDesc_p = (DmaMacDescr *) & (priv->RX_MAC_DescrTable);
	for (i = 0; i < RX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		SPEAR_ETH_dump_skb(DmaDesc_p->skb, 0);
	printk("TX descriptors sockets\n");
	DmaDesc_p = (DmaMacDescr *) & (priv->TX_MAC_DescrTable);
	for (i = 0; i < TX_MAC_DESCR_NUM; i++, DmaDesc_p++)
		SPEAR_ETH_dump_skb(DmaDesc_p->skb, 0);
}



/**
 ** Avoid memcpy in RX handler by pre-allocating the socket buffers
 **/
/***************************************************************************/
/* NAME:                    SPEAR_ETH_Skb_Prepare                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine prepares the skb for sending the data                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*,DmaMacDescr*                                           */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_Skb_Prepare(struct net_device *dev, volatile DmaMacDescr * pRxFD)
{
	struct sk_buff *skb;

	if (!(skb = dev_alloc_skb(ETH_BUFF_SIZE)))

	{
		return -1;
	}
	skb_reserve(skb, 4);
	skb->dev = dev;
	consistent_sync(skb->data, ETH_BUFF_SIZE, DMA_FROM_DEVICE);

	/* attach skb to FD */
	pRxFD->skb = skb;
	pRxFD->DmaMac_Addr = __virt_to_bus(skb->data);
	return 0;
}

/****************************************************************************/
/* NAME: 		   SPEAR_ETH_Init_Tx_Descrs		            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine initializes the transmit descriptors                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*       status                                                             */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_Init_Tx_Descrs(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    *TX_DmaDesc_p;
	int             i;
	dma_addr_t      eth_pa_bp;
	unsigned char  *eth_va_bp;
	int 		size = TX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

	priv->TX_MAC_DescrTable =
		dma_alloc_coherent(NULL, size, &priv->DMA_TX_Descrp, GFP_DMA);

	/* Build the Frame Descriptor table for sending */
	for (i = 0; i < TX_MAC_DESCR_NUM; i++)
	{
		TX_DmaDesc_p = (DmaMacDescr *) & (priv->TX_MAC_DescrTable[i]);

		TX_DmaDesc_p->DmaMac_Next =
			(((u32) priv->DMA_TX_Descrp +
			  ((u32) & priv->TX_MAC_DescrTable[(i < TX_MAC_DESCR_NUM - 1) ? (i + 1) : 0] - (u32) & priv->TX_MAC_DescrTable[0])));

		if (TX_DmaDesc_p->Eth_VAddr == 0)

		{
			eth_va_bp = dma_alloc_coherent(NULL, ETH_BUFF_SIZE, &eth_pa_bp, GFP_DMA);
			if (eth_va_bp == NULL)
				return -1;
			TX_DmaDesc_p->Eth_VAddr = eth_va_bp;
			TX_DmaDesc_p->Eth_PAddr = (volatile char *) eth_pa_bp;
		}
		TX_DmaDesc_p->skb = NULL;

#ifdef ETH_SPEARBASIC_BA
		TX_DmaDesc_p->TxRx_Status &= ~(DescTxIntEnable | DescTxLast | DescTxFirst | DescTxDisableCrc \
				| DescTxCheckInsCtrl | DescTxEndOfRing | DescTxDisablePadd);
		TX_DmaDesc_p->TxRx_Status =   DescTxChain | DescTxIntEnable;
		TX_DmaDesc_p->TxRx_Status &= (~(DescTxStatusMask|DescOwnByDma));	/* CPU owner */
		TX_DmaDesc_p->DmaMac_Cntl = 0;
#else
		TX_DmaDesc_p->DmaMac_Cntl =   DescChain | DescTxIntEnable;
		TX_DmaDesc_p->TxRx_Status = 0;	/* CPU owner */
#endif //(ETH_SPEARBASIC_BA)
	} 
	priv->TX_MAC_WinSize = TX_MAC_DESCR_NUM;
	ETH_RTL_Set_TxDescrListAddr((u32) priv->DMA_TX_Descrp);
	
	return 0;
}

/****************************************************************************/
/* NAME:	         SPEAR_ETH_Init_Rx_Descrs                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine initializes the recive descriptors                     */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        status                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_Init_Rx_Descrs(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    *RX_DmaDesc_p;
	int             i;
	int 		size = RX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

	priv->RX_MAC_DescrTable =
		dma_alloc_coherent(NULL, size, &priv->DMA_RX_Descrp, GFP_DMA);

	/* Build the Frame Descriptor table for receiving */
	for (i = 0; i < RX_MAC_DESCR_NUM; i++)

	{
		RX_DmaDesc_p = (DmaMacDescr *) & (priv->RX_MAC_DescrTable[i]);

		RX_DmaDesc_p->DmaMac_Next =
			(((u32) priv->DMA_RX_Descrp +
			  ((u32) & priv->RX_MAC_DescrTable[(i < RX_MAC_DESCR_NUM - 1) ? (i + 1) : 0] - (u32) & priv->RX_MAC_DescrTable[0])));
#ifdef ETH_SPEARBASIC_BA
		RX_DmaDesc_p->DmaMac_Cntl = (MAC_MAX_FRAME_SZ & DescSize1Mask) | DescRxChain;
#else
		RX_DmaDesc_p->DmaMac_Cntl = (MAC_MAX_FRAME_SZ & DescSize1Mask) | DescChain;
#endif //(ETH_SPEARBASIC_BA)
		if (SPEAR_ETH_Skb_Prepare(dev, RX_DmaDesc_p) < 0)
			return -1;
#ifdef ETH_SPEARBASIC_BA
		RX_DmaDesc_p->TxRx_Status = DescOwnByDma;	/* DMA owner */
#else
		RX_DmaDesc_p->TxRx_Status = DescOwnByDma;	/* DMA owner */
#endif //(ETH_SPEARBASIC_BA)
	}

	/* The Physical address needs to be passed to the DMA */
	ETH_RTL_Set_RxDescrListAddr((u32) priv->DMA_RX_Descrp); 
	
	return 0;
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_Free_Tx_Descrs                                          */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine frees the Transmit Descriptors                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_ETH_Free_Tx_Descrs(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    *DmaDesc_p;
	int             i;
	int 		size = TX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

	/* Free Rx/Tx skbuff */
	for (i = 0; i < TX_MAC_DESCR_NUM; i++)
	{
		DmaDesc_p = (DmaMacDescr *) & (priv->TX_MAC_DescrTable[i]);

		if (DmaDesc_p->Eth_VAddr)
		{
			dma_free_coherent(NULL, ETH_BUFF_SIZE, (void*)DmaDesc_p->Eth_VAddr, (dma_addr_t)DmaDesc_p->Eth_PAddr);
			DmaDesc_p->Eth_VAddr = 0;
			DmaDesc_p->Eth_PAddr = 0;
		}
		if (DmaDesc_p->skb)
			dev_kfree_skb_irq(DmaDesc_p->skb);
		memset(DmaDesc_p, 0, sizeof(DmaMacDescr));
	}
	/* Free the TX Descriptor table */
	dma_free_coherent(NULL, size, priv->TX_MAC_DescrTable,priv->DMA_TX_Descrp);
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_Free_Rx_Descrs                                          */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine frees the Receive Descriptors                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_ETH_Free_Rx_Descrs(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    *DmaDesc_p;
	int             i;
	int 		size = RX_MAC_DESCR_NUM * sizeof(DmaMacDescr);

	for (i = 0; i < RX_MAC_DESCR_NUM; i++)
	{
		DmaDesc_p = (DmaMacDescr *) & (priv->RX_MAC_DescrTable[i]);

		if (DmaDesc_p->skb)
			dev_kfree_skb(DmaDesc_p->skb);
		memset(DmaDesc_p, 0, sizeof(DmaMacDescr));
	}
	/* Free the RX Descriptor table */
	dma_free_coherent(NULL, size,priv->RX_MAC_DescrTable,priv->DMA_RX_Descrp);
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_rx_int                                                  */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is the receive interrupt handler for ethernet driver           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_ETH_rx_int(unsigned long d)
{
    struct net_device *dev = (struct net_device *)d;
	struct sk_buff *skb;
	unsigned int    DmaStat;
	unsigned int    len = 0;
	DmaMacDescr    *DmaDesc_p;
	struct eth_priv *priv = (struct eth_priv *) dev->priv;

	DmaDesc_p = &(priv->RX_MAC_DescrTable[priv->RX_MAC_currDescNum]);

	/* Read the Control and Status field of the current DMA descriptor */
	DmaStat = DmaDesc_p->TxRx_Status;

	/* Check if the owner is the CPU */
	while (!(DmaStat & DescOwnByDma))
	{
		if(!ETH_RTL_DmaRxValid(DmaStat))
		{	
			priv->stats.rx_errors++;
			priv->stats.collisions += ETH_RTL_SpearDmaRxCollisions(DmaStat);
			priv->stats.rx_crc_errors += ETH_RTL_SpearDmaRxCrc(DmaStat);
			priv->stats.rx_frame_errors += ETH_RTL_SpearDmaRxFrame(DmaStat);
			priv->stats.rx_length_errors += ETH_RTL_SpearDmaRxLengthError(DmaStat); 
		}
		else
		{
			len = (DmaStat & DescFrameLengthMask) >> DescFrameLengthShift;
			skb = DmaDesc_p->skb;

#ifdef ETH_DEBUG_SKB_RX_INT
			printk("\nInside Receive tasklet: Dump the receive skb\n");
			SPEAR_ETH_dump_skb (skb, 1);
#endif //(ETH_DEBUG_SKB_RX_INT)
			
			SPEAR_ETH_Skb_Prepare(dev, DmaDesc_p);

#ifdef CONFIG_ETH_SPEAR_CHECKSUM_OFFLOAD
			skb_put(skb, len -6);
			skb->protocol = eth_type_trans(skb, dev);	  
			if(skb->protocol == __constant_htons(ETH_P_IP)) 
			{
				skb->ip_summed = CHECKSUM_COMPLETE; 
				skb->csum = get_unaligned((u16 *) (skb->tail+4));
				skb->csum = (~ skb->csum) << 16;
			}
#else
			/* Reserve two words used by protocol layers */
			skb_put(skb, len - 4);
			skb->protocol = eth_type_trans(skb, dev);
#endif //(CONFIG_ETH_SPEAR_CHECKSUM_OFFLOAD)
			
			consistent_sync(skb->data, len, DMA_FROM_DEVICE);
			dev->last_rx = jiffies;

			priv->stats.rx_packets++;
			priv->stats.rx_bytes += len;
			netif_rx(skb);
		}
		DmaDesc_p->TxRx_Status |= DescOwnByDma;	/* DMA owner */

		/* Test the wrap-around condition. */
		if (++priv->RX_MAC_currDescNum >= RX_MAC_DESCR_NUM)
			priv->RX_MAC_currDescNum = 0;
		DmaDesc_p = &(priv->RX_MAC_DescrTable[priv->RX_MAC_currDescNum]);

		/* Read the Control and Status field of the current DMA descriptor */
		DmaStat = DmaDesc_p->TxRx_Status;

	}
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_tx_int                                                  */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is the transmit interrupt handler for ethernet driver          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void     SPEAR_ETH_tx_int(unsigned long d)
{    
    struct net_device *dev = (struct net_device *)d;
	unsigned int    DmaStat;
	u32 		len;
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	DmaMacDescr    	*prev_DmaDesc_p;

	prev_DmaDesc_p = &(priv->TX_MAC_DescrTable[priv->TX_MAC_prevDescNum]);

	/* Read the Control and Status field of the current DMA descriptor */
	DmaStat = prev_DmaDesc_p->TxRx_Status;

	/* Check if the owner is the CPU */
	while (prev_DmaDesc_p->skb && !(DmaStat & DescOwnByDma))
	{
#ifdef ETH_DEBUG_TX_INT
		printk("\n%s: tx irq: TX DMA Descriptor %i, status 0x%08x\n", dev->name, priv->TX_MAC_currDescNum, DmaStat);
#endif // (ETH_DEBUG_TX_INT)

		len=(prev_DmaDesc_p->DmaMac_Cntl & DescSize1Mask) >> DescSize1Shift;
		if( ETH_RTL_SpearDmaTxValid(DmaStat) )
		{
			priv->stats.tx_bytes += len;
			priv->stats.tx_packets++;
		}
		else
		{
			priv->stats.tx_errors++;
			priv->stats.tx_aborted_errors += ETH_RTL_SpearDmaTxAborted(DmaStat);
			priv->stats.tx_carrier_errors += ETH_RTL_SpearDmaTxCarrier(DmaStat);
		}
		priv->stats.collisions += ETH_RTL_SpearDmaTxCollisions(DmaStat);
		dev_kfree_skb(prev_DmaDesc_p->skb);
		prev_DmaDesc_p->DmaMac_Addr = NULL;
		prev_DmaDesc_p->skb = NULL;

		/* Test the wrap-around condition. */
		if (++priv->TX_MAC_prevDescNum >= TX_MAC_DESCR_NUM)
			priv->TX_MAC_prevDescNum = 0;
		++priv->TX_MAC_WinSize;
		prev_DmaDesc_p = &(priv->TX_MAC_DescrTable[priv->TX_MAC_prevDescNum]);

		/* Read the Control and Status field of the current DMA descriptor */
		DmaStat = prev_DmaDesc_p->TxRx_Status;
	}
	if (priv->TX_MAC_WinSize > 0)
		netif_wake_queue(dev);
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_int                                                	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      Main Ethernet driver interrupt handler				    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : irq:num of interrupt,dev_id: device id, pt_regs: ptr to registers  */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        irqreturn_t                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static irqreturn_t SPEAR_ETH_int(int irq, void *dev_id, struct pt_regs *regs)
{
	struct net_device *dev = (struct net_device *) dev_id;
	struct eth_priv	  *priv = (struct eth_priv *) dev->priv;
	unsigned 	  int_type=0;

	int_type = ETH_RTL_DmaIntType(); 
	while(int_type != 0)
	{ 
		if (int_type & SpearDmaRxNormal)
		{
			tasklet_schedule(&priv->rx_tasklet);
		}

		if (int_type & SpearDmaTxNormal)
		{
			tasklet_schedule(&priv->tx_tasklet);
		}
	/* Look for any other interrupts came while handling ints */
		int_type = ETH_RTL_DmaIntType();
	}        
	return IRQ_HANDLED;
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_Start_Xmit                                              */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Starts Transmission of data                                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : sk_buff*, net_device*                                              */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int  SPEAR_ETH_Start_Xmit(struct sk_buff *skb, struct net_device *dev)
{
	int             len;
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	unsigned int    DmaStat;
	DmaMacDescr    *DmaDesc_p;
	unsigned char  *va_data;
        
        /* static unsigned int pktCounter=0; */
        struct sk_buff *newskb;

        const char cpu_tag[] = {0x88, 0x99, 0xb0, 0x01};
        
        spin_lock(&priv->eth_lock);

        /* ############################# BEGIN OF ADDING CPU TAG TO EACH PACKET ############################################ */
        /* Description of skb handling: http://oss.org.cn/ossdocs/gnu_linux/kernel-api/c7136.html                            */
        
        if (isCpuPortTaggingEnabled) {

          /* if (pktCounter > 0) { */
          /*   printk("\nxxxxxxxxxxxxxxxxxxxxxxxxxx UNPATCHED No: %d xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n", pktCounter); */
          /*   SPEAR_ETH_dump_skb (skb, 1); */
          /*   printk("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"); */
          /* }         */

          if (skb_headroom(skb) < 4) {
            newskb = skb_realloc_headroom(skb, 4);
            if (!newskb) {
              printk(KERN_ALERT "SPEAR_ETH_Start_Xmit(): skb_realloc_headroom runs out of memory\n");
              return -ENOMEM;
            } else {
              kfree_skb(skb);
              skb = newskb;
            }
          }

          skb->nh.raw = skb_push(skb, 4);
          memmove((void *)skb->data, (const void *)skb->data+4, (size_t)12);
          memcpy((void *)skb->data+12, cpu_tag, (size_t)4);

          /* if (pktCounter > 0) { */
          /*   printk("\nxxxxxxxxxxxxxxxxxxxxxxxxxx PATCHED No: %d xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n", pktCounter); */
          /*   SPEAR_ETH_dump_skb (skb, 1); */
          /*   printk("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"); */
          /*   if (pktCounter) pktCounter++; */
          /* }         */

        }

        /* ############################### END OF ADDING CPU TAG TO EACH PACKET ############################################ */

	len = skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;
	dev->trans_start = jiffies;
	DmaDesc_p = (DmaMacDescr *) & (priv->TX_MAC_DescrTable[priv->TX_MAC_currDescNum]);

	DmaStat = DmaDesc_p->TxRx_Status;
	if (DmaStat & DescOwnByDma)	/* transmission not possible if valid bit is set for current Tx descriptor
					   means if the control is with DMA */
	{
		spin_unlock(&priv->eth_lock);
		return -EBUSY;
	}

#ifdef ETH_DEBUG_START_XMIT
	printk("\n%s: hard_start_xmit: size(decimal) %d, tx desc(decimal)%d\n", dev->name, skb->len, priv->TX_MAC_currDescNum);
#endif //(ETH_DEBUG_START_XMIT)

	DmaDesc_p->skb = skb;

	/* No need of word alligning the data buffer in ETH, to be deleted later*/
	va_data = (unsigned char *) ((u32) skb->data /* & ~0x3 */ );
	consistent_sync(va_data, len, DMA_TO_DEVICE);
	DmaDesc_p->DmaMac_Addr = (char *) (__virt_to_bus(va_data));

#ifdef ETH_DEBUG_START_XMIT
	printk("\nskb_data=0X %x", skb->data);
	printk("\nDmaDesc_p->DmaMac_Addr =0X %x", DmaDesc_p->DmaMac_Addr);
#endif
	DmaDesc_p->DmaMac_Cntl = 0x00;

#ifdef ETH_SPEARBASIC_BA
	DmaDesc_p->DmaMac_Cntl |= ((len << DescSize1Shift) & DescSize1Mask);
	DmaDesc_p->TxRx_Status  |= DescTxLast | DescTxFirst | DescTxIntEnable |DescTxChain;
	DmaDesc_p->TxRx_Status	&=  ~(DescTxStatusMask);
	DmaDesc_p->TxRx_Status |= DescOwnByDma;
#else
	DmaDesc_p->DmaMac_Cntl |= ((len << DescSize1Shift) & DescSize1Mask) | DescTxLast | DescTxFirst | DescTxIntEnable |DescChain;

	DmaDesc_p->TxRx_Status = DescOwnByDma;	/* DMA owner */
#endif //(ETH_SPEARBASIC_BA)

#ifdef ETH_DEBUG_START_XMIT
	printk("\nDmaDesc_p->TxRxStatus =0X %x", DmaDesc_p->TxRx_Status);
	printk("\nDmaDesc_p->DmaMac_Cntl =0X %x", DmaDesc_p->DmaMac_Cntl);
#endif //(ETH_DEBUG_START_XMIT)

	if (++(priv->TX_MAC_currDescNum) >= TX_MAC_DESCR_NUM)	
	{
		/*increment the current descriptor number and test condition */
		priv->TX_MAC_currDescNum = 0;
	}

	if (--priv->TX_MAC_WinSize <= 0)
	{	
		/* Stop network interface if all desccriptors are busy */
		priv->TX_MAC_WinSize = 0;
		netif_stop_queue(dev);
	}
	/* Resume the suspended transmission, if it is done before wrap-around condition, 
	   interrupt can occur in b/w (in case of short frame transmission) & update the variables (TX_MAC_WinSize)
	*/
	ETH_RTL_TxResume();

	spin_unlock(&priv->eth_lock);
	return 0;
}

#ifdef NO_INTERRUPT_COMMENT
static struct irqaction __irqaction = { name: "eth", flags: SA_INTERRUPT, handler:SPEAR_ETH_int,};
#endif /* NO_INTERRUPT_COMMENT */

/****************************************************************************/
/* NAME:  SPEAR_ETH_Reset                                                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Resets the ethernet driver                                            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : eth_priv*                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_ETH_Reset(struct eth_priv *adapter)
{
	ETH_RTL_Reset();
	/* Reset the counters */
	adapter->RX_MAC_currDescNum = 0;
	adapter->TX_MAC_currDescNum = 0;
	adapter->TX_MAC_prevDescNum = 0;
	adapter->TX_MAC_WinSize = TX_MAC_DESCR_NUM;
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_Down                                          	    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Brings down the ethernet interface                                    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : eth_priv*                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void  SPEAR_ETH_Down(struct eth_priv *adapter)
{
	/* Remove the Hot Plug Timer */
	del_timer_sync (&adapter->tmr_hotplug);
	/* Stop the stack from transmitting */
	netif_carrier_off(adapter->dev);
	netif_stop_queue(adapter->dev);
	/* Reset the MAC controller, and various rx/tx counters*/
	SPEAR_ETH_Reset(adapter);
	/* Free the interrupt handler */
	free_irq(adapter->dev->irq, adapter->dev);
	/* Free the rx/Tx Descriptors*/
	SPEAR_ETH_Free_Tx_Descrs(adapter->dev);
	SPEAR_ETH_Free_Rx_Descrs(adapter->dev);
	/* Kill the Rx/Tx tasklets*/
	tasklet_kill(&adapter->rx_tasklet);
	tasklet_kill(&adapter->tx_tasklet);
}

/****************************************************************************/
/* NAME:  SPEAr_ETH_Tmr_Hotplug_Fun                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    This routine is scheduled to run every HOTPLUG_TMOUT system ticks.    */
/*    It checks the link status						    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : unsigned long data						    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAr_ETH_Tmr_Hotplug_Fun (unsigned long data) 
{
	struct net_device *dev = (struct net_device *) data;
	int link_poll_interval;

	s8 status;
	u8 link_status;

	status = rtl8363_getAsicPHYLinkStatus(2, &link_status);
	if(status < 0)
	{
		printk("Get PHY Link Status for P2 failed\n");
	}

	if (link_status == 1)
	{
		netif_carrier_on(dev);
		link_poll_interval = PHY_POLL_LINK_ON;
	} 
	else 
	{
		printk("\nIn HotPlug::Link Status=0\n");
		netif_carrier_off(dev);
		link_poll_interval = PHY_POLL_LINK_OFF;
	} 

	mod_timer (&((struct eth_priv *) dev->priv)->tmr_hotplug, jiffies + link_poll_interval);

} 

/****************************************************************************/
/* NAME:  SPEAr_MAC_Cfg							    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*       Ether Configuration parameters are set by this routine             */
/*									    */
/* PARAMETERS:                                                              */
/* IN  : ether_config*, struct eth_priv *				    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAr_MAC_Cfg(eth_config  *ether_config,struct eth_priv *adap)
{
	ether_config->dplx = FULL_DUPLEX;
	ether_config->speed_100 = 1;
	printk("\nSpeed 100, Duplex Full is set\n");
}

/****************************************************************************/
/* NAME:  SPEAR_ETH_Up                                                      */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Called from open() function to start the ethernet with new settings   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : eth_priv*                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
int  SPEAR_ETH_Up(struct eth_priv *adapter)
{
	unsigned long     status;
	eth_config	  ether_config={0,0,0};
	struct net_device *dev = adapter->dev;
	u32	          addr_hi,addr_lo ;
	int 		  link_poll_interval;
	u8 		  link_status;
	u16 		  regval;

	/**
	 ** install ISR
	 **/
	status = request_irq(dev->irq, (void *) &SPEAR_ETH_int, 0, dev->name, dev);
	if (status)
	{
		printk(KERN_ERR "%s: unabled to hook irq %d for ethernet\n", dev->name, dev->irq);
		return status;
	}

	local_bh_disable();	

	/* Get P2 Link Speed */
	status = rtl8363_getAsicReg(2, 0, MACPAG , 0, &regval);//get mac2 link speed	
	if(status < 0)
	{
		printk("Read MII interface speed for P2 failed\n");
		return -1;
	}
	printk("rtl 8363s MII interface speed  = 0x%x\n",regval );

	status = rtl8363_getAsicReg(1, 23, MACPAG , 0, &regval);
	if(status < 0)
	{
		printk("Read MII Port Setting for P2 failed\n");
		return -1;
	}
	printk("rtl8363s MII Port setting value = 0x%x\n",regval );

	status = rtl8363_getAsicPHYLinkStatus(2, &link_status);
	if(status < 0)
	{
		printk("Get PHY Link Status for P2 failed\n");
		return -1;
	}
	printk("PHY 2 link status= %d\n",link_status);

	if (link_status == 1)
	{
		printk("\n===============================");
		printk("\nIn Eth_Up::Link Status=1\n");
		rtl8368_printEthernetPHYRegs(Phy_1);
		rtl8368_printEthernetPHYRegs(Phy_0);
		printk("================================\n");
		netif_carrier_on(dev);
		link_poll_interval = PHY_POLL_LINK_ON;
	} 
	else 
	{
		printk("\nIn Eth_Up::Link Status=0\n");
		netif_carrier_off(dev);
		link_poll_interval = PHY_POLL_LINK_OFF;
	} 

	if(ETH_RTL_Reset() < 0)
	{
		printk("reset fail \n");
		free_irq(dev->irq,dev);
		return -1;
	}

	status = rtl8363_setAsicCPUPort(2, 0); // Port 2 adds CPU tags by default
	if(status < 0)
	{
		printk("Specifying Asic CPU port to be P2 failed\n");
		return -1;
	}

	// Program the MAC Dst Address
	addr_hi = dev->dev_addr[4] | (dev->dev_addr[5] << 8);;
	addr_lo = dev->dev_addr[0] | (dev->dev_addr[1] << 8) | (dev->dev_addr[2] << 16) | (dev->dev_addr[3] << 24);;
	ETH_RTL_Set_Mac_Address(addr_hi,addr_lo);

	/* Setup the bottom half rx ring refill handler	*/
	tasklet_init(&((struct eth_priv *) dev->priv)->rx_tasklet, SPEAR_ETH_rx_int, (unsigned long) dev);
	tasklet_init(&((struct eth_priv *) dev->priv)->tx_tasklet, SPEAR_ETH_tx_int, (unsigned long) dev);

	SPEAr_MAC_Cfg(&ether_config,adapter);  
	SPEAR_ETH_Init_Tx_Descrs(dev);	
	SPEAR_ETH_Init_Rx_Descrs(dev);	
	ETH_RTL_Dma_Init(&ether_config);

	*Gmac_MMC_RxIntrMask=0x00ffffff;
	*Gmac_MMC_TxIntrMask=0x00ffffff;

	ETH_RTL_Mac_Init(&ether_config);

	/*
	* Setup the hotplugging timer routine
	*/ 
	init_timer (&((struct eth_priv *) dev->priv)->tmr_hotplug);
	((struct eth_priv *) dev->priv)->tmr_hotplug.data = (unsigned long) dev;
	((struct eth_priv *) dev->priv)->tmr_hotplug.function = &SPEAr_ETH_Tmr_Hotplug_Fun; /* timer handler */

	mod_timer (&((struct eth_priv *) dev->priv)->tmr_hotplug,jiffies + link_poll_interval);
	ETH_RTL_Rxstart();			/* Start reception */
	ETH_RTL_Txstart();			/* Start Transmission */
	if(link_status==0)
		netif_carrier_off(dev);
	else	
		netif_carrier_on(dev);

	netif_start_queue(dev);
	local_bh_enable();
	
	return 0;
}

/****************************************************************************/
/* NAME:  spear_open                                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Opens the ethernet device                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_Open(struct net_device *dev)
{
	struct eth_priv *adapter = (struct eth_priv *) dev->priv;

	ETH_RTL_Reset();
	
	if (SPEAR_ETH_Up(adapter) < 0)
	{
		printk("SPEAR_ETH_Up failed \n");
		SPEAR_ETH_Down(adapter);
		return 1;
	}
	return 0;
}

/****************************************************************************/
/* NAME:  spear_stop                                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Stops the ethernet                                                  */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                         */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_Stop(struct net_device *dev)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;

	SPEAR_ETH_Down(priv);
	return 0;
}

/****************************************************************************/
/* NAME:  spear_get_stats                                                   */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Gets the stats of ethernet                                            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                         */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
struct net_device_stats *SPEAR_ETH_Get_Stats(struct net_device *dev)
{
#ifdef ETH_DEBUG_STAT
	printk("%s: get_stats\n", dev->name);
	printk
		("TxPackets=%d RxError=%d RxDropped=%d RxFrmErr=%d RxCollisions=%d\n",
		 ((struct eth_priv *) dev->priv)->stats.rx_packets,
		 ((struct eth_priv *) dev->priv)->stats.rx_errors,
		 ((struct eth_priv *) dev->priv)->stats.rx_dropped,
		 ((struct eth_priv *) dev->priv)->stats.rx_frame_errors, ((struct eth_priv *) dev->priv)->stats.collisions);
	printk("RxCrc=%d RxMissed=%d RxLenErr=%d \n",
			((struct eth_priv *) dev->priv)->stats.rx_crc_errors,
			((struct eth_priv *) dev->priv)->stats.rx_missed_errors, ((struct eth_priv *) dev->priv)->stats.rx_length_errors);
	printk
		("TxPackets=%d TxError=%d txDropped=%d TxCarrier=%d RxCollisions=%d\n",
		 ((struct eth_priv *) dev->priv)->stats.tx_packets,
		 ((struct eth_priv *) dev->priv)->stats.tx_errors,
		 ((struct eth_priv *) dev->priv)->stats.tx_dropped,
		 ((struct eth_priv *) dev->priv)->stats.tx_carrier_errors, ((struct eth_priv *) dev->priv)->stats.collisions);
	printk("TxAborted=%d \n", ((struct eth_priv *) dev->priv)->stats.tx_aborted_errors);
#endif /* ETH_DEBUG_STAT */
	
	return &((struct eth_priv *) dev->priv)->stats;
}

/****************************************************************************/
/* NAME:                 SPEAR_ETH_Tx_Timeout                               */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/* Time out fun called when there is a tx time out by higher layers 	    */
/*									    */
/* PARAMETERS:                                                              */
/*									    */
/* IN  : net_device*,struct ifreq*,int cmd                                  */
/*									    */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_ETH_Tx_Timeout(struct net_device *dev)
{
	if (netif_carrier_ok(dev))
		netif_wake_queue(dev);
}

/****************************************************************************/
/* NAME:              SPEAR_ETH_ioctl                                       */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    Implements the ioctl for ethernet			            	    */
/*									    */
/* PARAMETERS:                                                              */
/* IN  : net_device*,struct ifreq*,int cmd                                  */
/*									    */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct eth_priv *priv = (struct eth_priv *) dev->priv;
	struct mii_ioctl_data *data = (struct mii_ioctl_data *) &rq->ifr_data;
	u32             frm_len;
	static char     frm_addr[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


	switch (cmd)

	{
#ifdef ETH_SPEAR_TEST
	case SIOCDEVPRIVATE:		/* Store the Ethernet Address */

		{
			frm_addr[0] = rq->ifr_ifru.ifru_slave[0];
			frm_addr[1] = rq->ifr_ifru.ifru_slave[1];
			frm_addr[2] = rq->ifr_ifru.ifru_slave[2];
			frm_addr[3] = rq->ifr_ifru.ifru_slave[3];
			frm_addr[4] = rq->ifr_ifru.ifru_slave[4];
			frm_addr[5] = rq->ifr_ifru.ifru_slave[5];
		}
		return 0;
	case SIOCDEVPRIVATE + 1:	/* Send 1 frame of 2K bytes. */

		{
			struct sk_buff *skb;

#ifdef ETH_DEBUG_IOCTL
			printk(KERN_INFO "%s: do SIOCDEVPRIVATE\n", dev->name);
#endif /* ETH_DEBUG_IOCTL */
			
			frm_len = (u32) rq->ifr_ifru.ifru_ivalue;

			/* ALLOC SKB */
			if (!(skb = dev_alloc_skb(frm_len)))
			{

#ifdef ETH_DEBUG_IOCTL
				printk("%s: unable to allocate skb...", dev->name);
#endif /* ETH_DEBUG_IOCTL */
				
				return ENOMEM;
			}
			skb->dev = dev;
			skb->len = frm_len;
			memcpy(skb->data, frm_addr, 6);
			SPEAR_ETH_Start_Xmit(skb, dev);
		}
		return 0;
#endif /* ETH_SPEAR_TEST */
	case SIOCDEVPRIVATE + 2:	/* enable adding CPU TAG to each outgoing packet */
		{
			isCpuPortTaggingEnabled = 1;
			return 0;
		}
	case SIOCDEVPRIVATE + 3:	/* disable adding CPU TAG to each outgoing packet */
		{
			isCpuPortTaggingEnabled = 0;
			return 0;
		}
	default:
		return generic_mii_ioctl(&priv->mii, data, cmd, NULL);
	}
}

/****************************************************************************/
/* NAME:              enable_mac_ras		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*  	temporary function that enables RAS for ETH operation in PHOTOFRAME */
/*	mode							            */
/* 
PARAMETERS:                                                                 */
/* IN  :  None					                            */

/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        None				                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
/* FIXME:ETH - Replace this RAS initi Function with something which initializes RAS at GLOBAL level */
void enable_mac_ras(void)
{
#if 1
	unsigned int va_ras1 = 0;
	unsigned int va_ras2 = 0;
	va_ras1 = (unsigned int)ioremap(SPEARBASIC_RAS1, 4);// in 2nd arg is ioremap, 4 is the pagesize
	va_ras2 = (unsigned int)ioremap(SPEARBASIC_RAS2, 4);
	writel(0xBFFF, va_ras1 );
	writel(0x2, va_ras2 );
#else
	//Enables MAC
	u32 *val;
	val = (0xc4802000);
	*val= 0xBfff;
#endif
}


/****************************************************************************/
/* NAME:  SPEAR_ETH_Init                                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*  initializes the ethernet driver                                         */
/*  The init function, invoked by register_netdev()
*/
/* PARAMETERS:                                                              */
/* IN  : net_device*                                                        */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        int                                                               */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ETH_Init(struct net_device *dev)
{
	struct eth_priv *priv;
	unsigned int value;
	int err;

	printk("%s: net_device init: %04x%08x\n", dev->name, *Gmac_Addr0HI, *Gmac_Addr0LO);

	/* Extract priv data structure */
	priv = netdev_priv(dev);
	memset(priv, 0, sizeof(struct eth_priv));

	/* Assign some of the fields */
	ether_setup(dev);

	/* Assign net_device methods */
	dev->open = SPEAR_ETH_Open;
	dev->stop = SPEAR_ETH_Stop;
	dev->do_ioctl = SPEAR_ETH_ioctl;
	dev->get_stats = SPEAR_ETH_Get_Stats;
	dev->tx_timeout = SPEAR_ETH_Tx_Timeout;
	dev->hard_start_xmit = SPEAR_ETH_Start_Xmit;

#ifdef SPEAr_ETHTOOL
	set_ethtool_ops (dev);
#endif

#ifdef CONFIG_ARCH_SPEARPLUS
	dev->irq = IRQ_GMAC_2 /*IRQ_MAC */ ;
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
	dev->irq = IRQ_MAC_2 /*IRQ_MAC */ ;
#endif
	dev->tx_queue_len = TX_MAC_DESCR_NUM;
	dev->dma = 0;
	dev->watchdog_timeo = HZ;
	value = ETH_RTL_Read_Mac_Address_Lo();  
	dev->dev_addr[0] = (value & 0x000000FF);
	dev->dev_addr[1] = (value & 0x0000FF00) >> 8;
	dev->dev_addr[2] = (value & 0x00FF0000) >> 16;
	dev->dev_addr[3] = (value & 0xFF000000) >> 24;
	value = ETH_RTL_Read_Mac_Address_Hi();
	dev->dev_addr[4] = (value & 0x000000FF);
	dev->dev_addr[5] = (value & 0x0000FF00) >> 8;

	// SET_MODULE_OWNER(dev);
	priv->dev = dev;
	priv->rx_ring_count = (uint32_t) SPEAR_DEFAULT_TXD;
	priv->tx_ring_count = (uint32_t) SPEAR_DEFAULT_RXD;

	/* 
	   We still need to access P0, P1 and P2 information through MDIO regs.
	   We don't write any MDIO regs,just read them. So, we need to enable ethernet in RAS.
	   1. No need to tell that Port 2 is the external CPU Port
	   2. No need to call RTL switch Init
	   3. No need to write any PHY regs for P0 and P1 and assume that hardware-strapping does the trick !!
	   4. No need to populate the priv structure here as the same is hardware strapped to 100Mbps/Full for P2	
	   */
	// enable_mac_ras();
	
    rtl8363_setAsicSoftReset();

#ifdef P0_ENABLE
	/* WorkAround: Just to ensure that P0 is powered up and supports AutoNEG */
	//rtl8363_setAsicReg(0, 0, UTPPAG, 0, 0x1140);  // Added to bring P0 out of PWR DWN
	//rtl8363_setAsicRegBit(0, 0, 9, MACPAG, 0, 1); //Enable/Disable AutoNEG for P0
#endif // (P0_ENABLE)

	/* Everything is up, register netdev now */
	err = register_netdev(dev);
	if (err)
		goto fail;


	netif_carrier_off(dev);
	netif_stop_queue(dev);

	/*  initializing the priv structure  */
	spin_lock_init(&priv->eth_lock);

	return 0;
fail:
	free_netdev(dev);
	return err;

}

/****************************************************************************/
/* NAME:  SPEAr_get_link_status
*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*    get the link status                                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : net_device* dev                                                    */
/*                                                                          */
/* OUT :                                                                    */
/*        None                                                              */
/*                                                                          */
/* RETURN:                                                                  */
/*        void                                                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

uint32_t SPEAr_get_link_status(struct net_device *dev)
{
	uint32_t mii_status;
	
	mii_status = ETH_RTL_Mdio_Read(Phy_num, MII_BMSR); 

	return (mii_status & BMSR_LSTATUS);
}

/* Points to the SPEAR net device */
static struct net_device *this_device;

/****************************************************************************/
/* NAME:           SPEAR_ETH_Init_module                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine registers the Ethernet driver 			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None		 						    */
/* OUT : None                                                               */
/*									    */
/* RETURN: status						    	    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAR_ETH_Init_module(void)
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

/****************************************************************************/
/* NAME:           SPEAR_ETH_Cleanup                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine unregisters the Ethernet driver			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None		 						    */
/* OUT : None                                                               */
/*									    */
/* RETURN: status						    	    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void __exit SPEAR_ETH_Cleanup(void)
{
	unregister_netdev(this_device);
	free_netdev(this_device);
	return;
}


module_init(SPEAR_ETH_Init_module);
module_exit(SPEAR_ETH_Cleanup);
#ifdef CONFIG_ARCH_SPEARPLUS
MODULE_DESCRIPTION("ST SPEARPLUS ethernet driver");
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
MODULE_DESCRIPTION("ST SPEARBASIC ethernet driver");
#endif

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(SPEAR_RTL_ETH_Dump_Registers);
EXPORT_SYMBOL(spear_rtl_driver_name);
EXPORT_SYMBOL(spear_rtl_driver_version);
EXPORT_SYMBOL(SPEAr_get_link_status);
