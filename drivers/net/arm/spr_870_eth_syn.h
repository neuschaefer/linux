/*
* linux/drivers/net/arm/spr_eth_syn.h
*
*  Copyright (C)2006  ST MICROELECTRONICS
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* AUTHOR :  Bhupesh Sharma <bhupesh.sharma@st.com>
*/

#ifndef __SPR_870_ETH_SYN_HEADER
#define __SPR_870_ETH_SYN_HEADER

#define __packed    __attribute__ ((packed))

#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/pci.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/pagemap.h>
#include <linux/dma-mapping.h>
#include <linux/bitops.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/capability.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <net/pkt_sched.h>
#include <linux/list.h>
#include <linux/reboot.h>

#ifdef NETIF_F_TSO
#include <net/checksum.h>
#endif	/*  */
  
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/if_vlan.h>
#include <asm/arch/spear.h>

#include "spr_870_eth_phy.h"

#ifdef NO_CACHE
#define	MAC_ALIAS_MEMORY	0x00000000
#else	/*  */
#define	MAC_ALIAS_MEMORY	0x80000000
#endif	/*  */
  
#define ETH_BUFF_SIZE 2048

/* Error Codes */ 
#define SPEAR_SUCCESS      0
#define SPEAR_ERR_EEPROM   1
#define SPEAR_ERR_PHY      2
#define SPEAR_ERR_CONFIG   3
#define SPEAR_ERR_PARAM    4
#define SPEAR_ERR_MAC_TYPE 5
#define SPEAR_ERR_PHY_TYPE 6

//FIXME:ETH Somehow use proper macro here
//#define CONFIG_ARCH_SPEARBASIC_BA 1
#define ETH_SPEARBASIC_BA 1

//#define DBG
#ifdef DBG
#define DEBUGOUT(S)		printk( S "\n")
#define DEBUGOUT1(S, A...)	printk( S "\n", A)
#else	/*  */
#define DEBUGOUT(S)
#define DEBUGOUT1(S, A...)
#endif	/*  */
  
#define DEBUGFUNC(F) DEBUGOUT(F)
#define DEBUGOUT2 DEBUGOUT1
#define DEBUGOUT3 DEBUGOUT2
#define DEBUGOUT7 DEBUGOUT3

#define HOTPLUG_TMOUT	HZ
#define PHY_POLL_LINK_ON HZ 
#define PHY_POLL_LINK_OFF	(HZ/5)
/* GMAC DMA Descriptor definition*/ 
/* Set DmaDescriptorSkip8 in DmaBusModeInit to skip the extra 5 words in Ring mode 
   8 not 5 to keep all the descriptor word alligned if first descriptor is word alligned */ 
typedef struct 
{
	volatile int   TxRx_Status;
	volatile int DmaMac_Cntl;
	volatile char *DmaMac_Addr;
	volatile int DmaMac_Next;
	volatile int DmaPhy_Addr;
	volatile int DmaPhy_RxAddr;
	volatile char *Eth_VAddr;
	volatile char *Eth_PAddr;
	struct sk_buff *skb;
	volatile int dummy[3];	// just to keep skip length 8
} DmaMacDescr;

typedef enum 
{ 
	spear_eeprom_uninitialized = 0, 
	spear_eeprom_spi, 
	spear_eeprom_microwire, 
	spear_num_eeprom_types, 
	spear_eeprom_I2C 
} spear_eeprom_type;

struct spear_eeprom_info 
{
	spear_eeprom_type type;
	uint16_t word_size;
	uint16_t opcode_bits;
	uint16_t address_bits;
	uint16_t delay_usec;
	uint16_t page_size;
};

enum DUPLEX  
{ 
	HALF_DUPLEX = 0,
	FULL_DUPLEX 
};


#define SPEED_10    10
#define SPEED_100   100
#define SPEED_1000  1000

/* TX/RX descriptor defines */ 
#define SPEAR_DEFAULT_TXD                  CONFIG_870_ETH_SPEAR_MAC_TXB
#define SPEAR_MAX_TXD                      256
#define SPEAR_MIN_TXD                      16

#define SPEAR_DEFAULT_RXD                  CONFIG_870_ETH_SPEAR_MAC_RXB
#define SPEAR_MAX_RXD                      256
#define SPEAR_MIN_RXD                       80

#define RX_MAC_DescrTable_RAM   0x60400	 /* 0x21000400 */
#define TX_MAC_DescrTable_RAM   0x60000	/* 0x21000000 */

#define RX_MAC_DESCR_NUM	SPEAR_DEFAULT_TXD
#define TX_MAC_DESCR_NUM	SPEAR_DEFAULT_RXD
#define MAC_MAX_FRAME_SZ	1528
#define MAC_TRIGGER		8
#define MAC_END_TEST		50

/* Simple sanity check */
#if SPEAR_DEFAULT_TXD > 256 || SPEAR_DEFAULT_RXD > 256 || SPEAR_DEFAULT_RXD <8 || SPEAR_DEFAULT_TXD <8
#error Invalid number of buffer descriptors (greater than 256 or less than 8)
#endif


/* only works for sizes that are powers of 2 */ 
#define SPEAR_ROUNDUP(i, size) ((i) = (((i) + (size) - 1) & ~((size) - 1)))

/* DEFINE OF MAC_TEST_DESCR_NUM FOR ALL TEST */ 
#ifdef _MAC_BROADCAST_DISABLE
#define MAC_TEST_DESCR_NUM	    3
#else	/*  */
#define MAC_TEST_DESCR_NUM	    7
#endif	/*  */
  
//#define ETH_Base      IO_ADDRESS(0xB0800000)  
//#define ETH_Base      IO_ADDRESS(0x20000000)  
//#define ETH_Base      IO_ADDRESS(0XE0800000)  
#define ST_PHY_ID		0x1C04  

#ifdef CONFIG_ARCH_SPEARBASIC
#define ETH_Base    VA_SPEARBASIC_START_MAC	
#endif	/*  */
  
#ifdef CONFIG_ARCH_SPEARPLUS
#define ETH_Base    VA_SPEARPLUS_START_GMAC	
#endif	/*  */
  
#define Dma_Base	(ETH_Base + 0x1000)	// I/O Base Address of DMA_MAC 

#define DMA_REG_COUNT	22	
#define MAC_REG_COUNT	48

#define PHY_NUM_0		0
#define PHY_NUM_1		1		/* can be maximum 32 PHYs*/
#define PHY_NUM_9		9		/* can be maximum 32 PHYs*/
#define PHY_NUM_16		16		/* can be maximum 32 PHYs*/
#define PHY_NUM_17		17		/* can be maximum 32 PHYs*/
/*
   enum MiiRegisters
   {
   GEN_ctl   = 0x00,
   GEN_sts   = 0x01,
   GEN_id_hi = 0x02,
   GEN_id_lo = 0x03,
   AN_adv    = 0x04,
   AN_lpa    = 0x05,
   AN_exp    = 0x06,
   AN_np     = 0x07,
   PHY_ctl   = 0x10,
   };
   */ 
/*****************************************************************************
* DMA_MAC Registers
 *****************************************************************************/ 

/* DMA_MAC Bus Mode Register Addresses*/ 

#define DmaMac_BusMode		   	((volatile unsigned int *) (Dma_Base + 0x000))
#define DmaMac_BusMode_Msk		0x0001ff7f

/* ... Values */ 
#define DmaMac_SRst					0x00000001
#define DmaMac_Arb					0x00000002
/* Descriptor length : can be any value bw 0 to 31 */ 
#define DmaMac_DescrSkipLen0		0x00000000
#define DmaMac_DescrSkipLen1		0x00000004
#define DmaMac_DescrSkipLen2		0x00000008
#define DmaMac_DescrSkipLen4		0x00000010
#define DmaMac_DescrSkipLen8		0x00000020
#define DmaMac_DescrSkipLen16		0x00000040	
/* Burst length : can be 1,2,4,8,16 or 32 beat only */ 
#define DmaMac_Burst_1				0x00000100
#define DmaMac_Burst_2				0x00000200
#define DmaMac_Burst_4				0x00000400
#define DmaMac_Burst_8				0x00000800
#define DmaMac_Burst_16				0x00001000
#define DmaMac_Burst_32				0x00002000
/* Rx:Tx Priority ratio : valid only when Arbitration bit is Reset */ 
#define DmaMac_RrRxTx_1				0x00000000	//      1:1
#define DmaMac_RrRxTx_2				0x00004000	//      2:1
#define DmaMac_RrRxTx_3				0x00008000	//  3:1
#define DmaMac_RrRxTx_4				0x0000c000	//      4:1
/* Selection between Fixed or non-fixed burst */ 
#define DmaMac_FixedBurst			0x00010000

/* DMA_MAC Poll Demand Register*/ 
#define DmaMac_TxPollReg		((volatile unsigned int *) (Dma_Base + 0x004))
#define DmaMac_TxPollReg_Msk		0xffffffff
#define DmaMac_TxPoll_Data			0xFFFFFFFF

/* DMA_MAC Rx Poll Demand Register*/ 
#define DmaMac_RxPollReg		((volatile unsigned int *) (Dma_Base + 0x008))
#define DmaMac_RxPollReg_Msk		0xffffffff
#define DmaMac_RxPoll_Data			0xFFFFFFFF

/* DMA_MAC TX/RX DMA Descr List Addr Register*/ 
#define DmaMac_RxDescrListAddr	((volatile unsigned int *) (Dma_Base + 0x00C))
#define DmaMac_RxDescrList_Msk		0xfffffffc /* VM modified as bit 1:0 are ignored & readonly*/

#define DmaMac_TxDescrListAddr	((volatile unsigned int *) (Dma_Base + 0x010))
#define DmaMac_TxDescrList_Msk		0xfffffffc /* VM modified as bit 1:0 are ignored & readonly*/

/* DMA_MAC Status Register*/ 
#define DmaMac_StatsReg		((volatile unsigned int *) (Dma_Base + 0x014))
#define DmaMac_StatsReg_Msk			0x1fffe7ff

/** Values */ 
#define DmaMac_TxInt				0x00000001
#define DmaMac_TxProcStop			0x00000002
#define DmaMac_TxBuffUnavl			0x00000004
#define DmaMac_TxJabTout			0x00000008
#define DmaMac_RxOvrflow			0x00000010
#define DmaMac_TxUnderflow			0x00000020
#define DmaMac_RxInt				0x00000040
#define DmaMac_RxBuffUnavail		0x00000080
#define DmaMac_RxProcStop			0x00000100
#define DmaMac_RxWdogTout			0x00000200
#define DmaMac_EarlyTxint			0x00000400
#define DmaMac_FatalBusErrInt		0x00002000
#define DmaMac_EarlyRxint			0x00004000
#define DmaMac_AbIntSumm			0x00008000
#define DmaMac_NorIntSumm			0x00010000
	
/* DMA_MAC Operation Mode Register*/ 
#define DmaMac_OprModeReg		((volatile unsigned int *) (Dma_Base + 0x018))
#define DmaMac_OprModeReg_Msk		0x0031ffde

#define DmaStoreAndForward			0x00200000
#define DmaFlush_TxFIFO				0x00100000
	/* Transmit FIFO size threshold*/ 
#define DmaTxThreshold16			0x0001C000
#define DmaTxThreshold24			0x00018000
#define DmaTxThreshold32			0x00014000
#define DmaTxThreshold40			0x00010000
#define DmaTxThreshold256			0x0000C000
#define DmaTxThreshold192			0x00008000
#define DmaTxThreshold128			0x00004000
#define DmaTxThreshold64			0x00000000

#define	DmaTxStart					0x00002000

	/* Receive FIFO : Flow Control Deactivation Threshold*/ 
#define DmaFC_DeAct_Threshold4K		0x00000600
#define DmaFC_DeAct_Threshold3K		0x00000400
#define DmaFC_DeAct_Threshold2K		0x00000200
#define DmaFC_DeAct_Threshold1K		0x00000000

	/* Receive FIFO : Flow Control Activation Threshold*/ 
#define DmaFC_ActThreshold4K		0x00000600
#define DmaFC_ActThreshold3K		0x00000400
#define DmaFC_ActThreshold2K		0x00000200
#define DmaFC_ActThreshold1K		0x00000000

#define DmaEnable_HW_FC				0x00000100
#define DmaFwdErrorFrames			0x00000080
#define DmaFwdUnderSzFrames			0x00000040

	/* Receive FIFO threshold */ 
#define DmaRxThreshold128			0x00000018
#define DmaRxThreshold96			0x00000010
#define DmaRxThreshold32			0x00000008
#define DmaRxThreshold64			0x00000000

#define DmaTxSecondFrame			0x00000004
#define DmaRxStart					0x00000002
	
/* DMA_MAC Interrupt Sources Enable Register Addresses */ 
#define DmaMac_IntEn		((volatile unsigned int *) (Dma_Base + 0x01c))
#define DmaMac_IntEn_Msk			0x0001e7ff

	/* ... Values */ 
#define DmaIeTxCompleted			0x00000001
#define DmaIeTxStopped				0x00000002
#define DmaIeTxNoBuffer				0x00000004
#define DmaIeTxJabberTO				0x00000008
#define DmaIeRxOverflow				0x00000010
#define DmaIeTxUnderflow			0x00000020
#define DmaIeRxCompleted			0x00000040
#define DmaIeRxNoBuffer				0x00000080
#define DmaIeRxStopped				0x00000100
#define DmaIeRxWdogTO				0x00000200
#define DmaIeEarlyTx				0x00000400

#define DmaIeBusError				0x00002000
#define DmaIeEarlyRx				0x00004000
#define DmaIeAbnormal				0x00008000
#define DmaIeNormal					0x00010000

	//For Driver to be independent of h/w
	enum SpearDmaIntEnum /* Intrerrupt types */ 
{ 
	SpearDmaRxNormal	= 0x01, /* normal receiver interrupt */ 
	SpearDmaRxAbnormal	= 0x02, /* abnormal receiver interrupt */ 
	SpearDmaRxStopped	= 0x04, /* receiver stopped */ 
	SpearDmaTxNormal	= 0x08, /* normal transmitter interrupt */ 
	SpearDmaTxAbnormal	= 0x10, /* abnormal transmitter interrupt */ 
	SpearDmaTxStopped	= 0x20, /* transmitter stopped */ 
	SpearDmaError		= 0x80, /* Dma engine error */ 
};

/* Missed Frame & Buffer overflow Counter Register*/ 

#define DmaMac_MissedFrm_Buff_OV_Cntr		((volatile unsigned int *) (Dma_Base + 0x020))
#define DmaMac_MissedFrm_Buff_OV_Cntr_Msk	0x1fffffff	/* VM modified*/

/* Current Host Tx Descr Register*/ 

#define DmaMac_CurrHostTxDescrAddr		((volatile unsigned int *) (Dma_Base + 0x048))
#define DmaMac_CurrHostTxDescrAddr_Msk	0xffffffff /* VM modified*/

/* Current Host Rx Descr Register*/ 

#define DmaMac_CurrHostRxDescrAddr		((volatile unsigned int *) (Dma_Base + 0x04c))
#define DmaMac_CurrHostRxDescrAddr_Msk	0xffffffff /* VM modified*/

/* Current Host Tx Buffer Addr Register*/ 

#define DmaMac_CurrHostTxBuffAddr		((volatile unsigned int *) (Dma_Base + 0x050))
#define DmaMac_CurrHostTxBuffAddr_Msk	0xffffffff /* VM modified*/

/* Current Host Rx Buffer Addr Register*/ 

#define DmaMac_CurrHostRxBuffAddr		((volatile unsigned int *) (Dma_Base + 0x054))
#define DmaMac_CurrHostRxBuffAddr_Msk	0xffffffff /* VM modified*/

/*****************************************************************************
* Ethernet IEEE802.3 definitions
   *****************************************************************************/ 

/* MAC Frame Format*/ 
/*
   typedef struct
   {
   char MAC_DstAddr[6];
   char MAC_SrcAddr[6];
   short MAC_Len;
   } MACFrame;

#define	MACHEADERSZ	14
#define	MACCRCSZ	4
*/ 

/*****************************************************************************
* Ethernet ETH core Registers
 *****************************************************************************/ 

/*****************************************************************************
* Ethernet GMAC110 core Registers
   *****************************************************************************/ 

/* ETH Control Register Addresses*/ 

#define Gmac_Ctrl				((volatile unsigned int *) (ETH_Base + 0x000))
#define Gmac_Ctrl_Msk				0x00febefc

/* ... Values */ 
#define Gmac_RxEn					0x00000004
#define Gmac_TxEn					0x00000008
#define Gmac_DfrChk					0x00000010
#define Gmac_BckOff_10				0x00000000
#define Gmac_BckOff_08				0x00000020
#define Gmac_BckOff_04				0x00000040
#define Gmac_BckOff_01				0x00000060
#define Gmac_AutoPadStrip			0x00000080
#define Gmac_LinkUpDown				0x00000100
#define Gmac_NoRty					0x00000200
#define Gmac_IPCSoffload			0x00000400
#define Gmac_FullDplxMode			0x00000800
#define Gmac_LoopMode				0x00001000
#define Gmac_DisableRxOwn			0x00002000
#define Gmac_EnableRxOwn			0x0
#define Gmac_FastEtherSpeed			0x00004000
#define Gmac_Gmii_PortSelect        0x0              /* Select GMII mode  */ 
#define Gmac_MII_PortSelect			0x00008000
#define Gmac_DCRS				0x00010000 /* Added */ 
#define Gmac_InterFrameGap_96		0x00000000
#define Gmac_InterFrameGap_88		0x00020000
#define Gmac_InterFrameGap_80		0x00040000
#define Gmac_InterFrameGap_72		0x00060000
#define Gmac_InterFrameGap_64		0x00080000
#define Gmac_InterFrameGap_56		0x000A0000
#define Gmac_InterFrameGap_48		0x000C0000
#define Gmac_InterFrameGap_40		0x000E0000
#define Gmac_JumboFrameEnable		0x00100000
#define Gmac_FrameBurstEnable		0x00200000
#define Gmac_JabberDisable			0x00400000
#define Gmac_WatchdogDisable		0x00800000

/* ETH Frame Filter Register Addresses*/ 

#define Gmac_FrameFilter		((volatile unsigned int *) (ETH_Base + 0x004))
#define Gmac_FrameFilter_Msk		0x800003ff

#define Gmac_PrmscMode				0x00000001
#define Gmac_HashUniCast			0x00000002
#define Gmac_HashMultiCast			0x00000004
#define Gmac_DAInvFilter			0x00000008
#define Gmac_PassAllMCast			0x00000010
#define Gmac_DisableBrdCast			0x00000020
#define Gmac_PassCntrlFrms_0		0x00000000
#define Gmac_PassCntrlFrms_1		0x00000080
#define Gmac_PassCntrlFrms_2		0x000000c0
#define Gmac_SAInvFltr				0x00000100
#define Gmac_SAFltrEn				0x00000200
#define Gmac_RxAll					0x80000000

/*
#define Gmac_NoRxBrd		0x00000800
#define Gmac_LateColCtrl		0x00001000
#define Gmac_ImperfFiltON	0x00002000
#define Gmac_PassBadFrms		0x00010000
#define Gmac_LoopMode_int	0x00200000
#define Gmac_LoopMode_ext	0x00400000
#define Gmac_NoRxOwn		0x00800000
#define Gmac_PortSlct		0x08000000
#define Gmac_NoHeartBeat		0x10000000
#define Gmac_BigEndMode		0x40000000
*/ 

/* ETH Multicast Hash Table Hi Register */ 

#define Gmac_Hash_Table_HI		((volatile unsigned int *) (ETH_Base + 0x008))
#define Gmac_Hash_Table_HI_Msk		0xffffffff

/* ETH Multicast Hash Table Lo Register */ 

#define Gmac_Hash_Table_LO		((volatile unsigned int *) (ETH_Base + 0x00c))
#define Gmac_Hash_Table_LO_Msk		0xffffffff

/* ETH MII Registers*/ 

#define GMii_AddrReg			((volatile unsigned int *) (ETH_Base + 0x010))
#define GMii_AddrReg_Msk			0x0000ffde

/* ... Values */ 

#define GMii_Busy					0x00000001
#define GMii_Write					0x00000002
/* MDC Clock Selection */ 
#define GMii_ClkRange_60_100M		0x00000000	/* MDC = Clk/42 */
#define GMii_ClkRange_100_150M		0x00000004	/* MDC = Clk/62 */
#define GMii_ClkRange_20_35M		0x00000008	/* MDC = Clk/16 */
#define GMii_ClkRange_35_60M		0x0000000C	/* MDC = Clk/26 */
#define GMii_ClkRange_150_250M		0x00000010	/* MDC = Clk/102 */
#define GMii_ClkRange_250_300M		0x00000014	/* MDC = Clk/122 */

#define GmiiAddrShift				11
#define GmiiRegShift				6
#define GMii_RegisterMsk			0x000007c0
#define GPhy_AddrMsk				0x0000f800

/*
#define GMii_RegisterU		0x00000040
#define Phy_AddrU		0x00000800
*/ 

#define GMii_DataReg			((volatile unsigned int *) (ETH_Base + 0x014))
#define GMii_DataReg_Msk			0x0000ffff

/* ETH Flow Control Registers*/ 

#define Gmac_FlwCtrlReg			((volatile unsigned int *) (ETH_Base + 0x018))
#define Gmac_FlwCtrlReg_Msk			0xffff003f

/* ... Values */ 
#define Gmac_FlwCtrlBusy			0x00000001
#define Gmac_FlwCtrlTxEn			0x00000002
#define Gmac_FlwCtrlRxEn			0x00000004
#define Gmac_UniCastPFrm			0x00000008
#define Gmac_PauseLwTh_4TS			0x00000000
#define Gmac_PauseLwTh_28TS			0x00000010
#define Gmac_PauseLwTh_144TS		0x00000020
#define Gmac_PauseLwTh_256TS		0x00000030
#define Gmac_PauseTimeMsk			0xffff0000

/* ETH VLAN Tag Register */ 
#define ETH_VlanTag_Reg		((volatile unsigned int *) (ETH_Base + 0x01C))

/* ETH Address0_HI Register Addresses*/ 

#define Gmac_Addr0HI			((volatile unsigned int *) (ETH_Base + 0x0040))
#define Gmac_Addr0HI_Msk			0x8000ffff

/* ETH Address0_LO Register Addresses*/ 

#define Gmac_Addr0LO			((volatile unsigned int *) (ETH_Base + 0x0044))
#define Gmac_Addr0LO_Msk			0xffffffff
#define Gmac_Addr0LO_Rst			0xffffffff

/* ETH Address1_HI Register Addresses*/ 
#define Gmac_Addr1HI			((volatile unsigned int *) (ETH_Base + 0x0048))
#define Gmac_Addr1HI_Msk			0xff00ffff
#define Gmac_Addr1HI_Rst			0x0000ffff

/* Bit values to be defined for GMAC ADDRESS1 to 15 registers */ 

/* GMAC Address1_LO Register Addresses*/ 
#define Gmac_Addr1LO			((volatile unsigned int *) (ETH_Base + 0x004C))
#define Gmac_Addr1LO_Msk			0xffffffff
#define Gmac_Addr1LO_Rst			0xffffffff

/* GMAC Address2_HI Register Addresses*/ 
#define Gmac_Addr2HI			((volatile unsigned int *) (ETH_Base + 0x0050))
#define Gmac_Addr2HI_Msk			0xff00ffff
#define Gmac_Addr2HI_Rst			0x0000ffff

/* GMAC Address2_LO Register Addresses*/ 
#define Gmac_Addr2LO			((volatile unsigned int *) (ETH_Base + 0x0054))
#define Gmac_Addr2LO_Msk			0xffffffff
#define Gmac_Addr2LO_Rst			0xffffffff

/* GMAC Address3_HI Register Addresses*/ 
#define Gmac_Addr3HI			((volatile unsigned int *) (ETH_Base + 0x0058))
#define Gmac_Addr3HI_Msk			0xff00ffff
#define Gmac_Addr3HI_Rst			0x0000ffff

/* GMAC Address3_LO Register Addresses*/ 
#define Gmac_Addr3LO			((volatile unsigned int *) (ETH_Base + 0x005C))
#define Gmac_Addr3LO_Msk			0xffffffff
#define Gmac_Addr3LO_Rst			0xffffffff

/* GMAC Address4_HI Register Addresses*/ 
#define Gmac_Addr4HI			((volatile unsigned int *) (ETH_Base + 0x0060))
#define Gmac_Addr4HI_Msk			0xff00ffff
#define Gmac_Addr4HI_Rst			0x0000ffff

/* GMAC Address4_LO Register Addresses*/ 
#define Gmac_Addr4LO			((volatile unsigned int *) (ETH_Base + 0x0064))
#define Gmac_Addr4LO_Msk			0xffffffff
#define Gmac_Addr4LO_Rst			0xffffffff

/* GMAC Address5_HI Register Addresses*/ 
#define Gmac_Addr5HI			((volatile unsigned int *) (ETH_Base + 0x0068))
#define Gmac_Addr5HI_Msk			0xff00ffff
#define Gmac_Addr5HI_Rst			0x0000ffff

/* GMAC Address5_LO Register Addresses*/ 
#define Gmac_Addr5LO			((volatile unsigned int *) (ETH_Base + 0x006C))
#define Gmac_Addr5LO_Msk			0xffffffff
#define Gmac_Addr5LO_Rst			0xffffffff

/* GMAC Address6_HI Register Addresses*/ 
#define Gmac_Addr6HI			((volatile unsigned int *) (ETH_Base + 0x0070))
#define Gmac_Addr6HI_Msk			0xff00ffff
#define Gmac_Addr6HI_Rst			0x0000ffff

/* GMAC Address6_LO Register Addresses*/ 
#define Gmac_Addr6LO			((volatile unsigned int *) (ETH_Base + 0x0074))
#define Gmac_Addr6LO_Msk			0xffffffff
#define Gmac_Addr6LO_Rst			0xffffffff

/* GMAC Address7_HI Register Addresses*/ 
#define Gmac_Addr7HI			((volatile unsigned int *) (ETH_Base + 0x0078))
#define Gmac_Addr7HI_Msk			0xff00ffff
#define Gmac_Addr7HI_Rst			0x0000ffff

/* GMAC Address7_LO Register Addresses*/ 
#define Gmac_Addr7LO			((volatile unsigned int *) (ETH_Base + 0x007C))
#define Gmac_Addr7LO_Msk			0xffffffff
#define Gmac_Addr7LO_Rst			0xffffffff

/* GMAC Address8_HI Register Addresses*/ 
#define Gmac_Addr8HI			((volatile unsigned int *) (ETH_Base + 0x0080))
#define Gmac_Addr8HI_Msk			0xff00ffff
#define Gmac_Addr8HI_Rst			0x0000ffff

/* GMAC Address8_LO Register Addresses*/ 
#define Gmac_Addr8LO			((volatile unsigned int *) (ETH_Base + 0x0084))
#define Gmac_Addr8LO_Msk			0xffffffff
#define Gmac_Addr8LO_Rst			0xffffffff

/* GMAC Address9_HI Register Addresses*/ 
#define Gmac_Addr9HI			((volatile unsigned int *) (ETH_Base + 0x0088))
#define Gmac_Addr9HI_Msk			0xff00ffff
#define Gmac_Addr9HI_Rst			0x0000ffff

/* GMAC Address9_LO Register Addresses*/ 
#define Gmac_Addr9LO			((volatile unsigned int *) (ETH_Base + 0x008C))
#define Gmac_Addr9LO_Msk			0xffffffff
#define Gmac_Addr9LO_Rst			0xffffffff

/* GMAC Address10_HI Register Addresses*/ 
#define Gmac_Addr10HI			((volatile unsigned int *) (ETH_Base + 0x0090))
#define Gmac_Addr10HI_Msk			0xff00ffff
#define Gmac_Addr10HI_Rst			0x0000ffff

/* GMAC Address10_LO Register Addresses*/ 
#define Gmac_Addr10LO			((volatile unsigned int *) (ETH_Base + 0x0094))
#define Gmac_Addr10LO_Msk			0xffffffff
#define Gmac_Addr10LO_Rst			0xffffffff

/* GMAC Address11_HI Register Addresses*/ 
#define Gmac_Addr11HI			((volatile unsigned int *) (ETH_Base + 0x0098))
#define Gmac_Addr11HI_Msk			0xff00ffff
#define Gmac_Addr11HI_Rst			0x0000ffff

/* GMAC Address11_LO Register Addresses*/ 
#define Gmac_Addr11LO			((volatile unsigned int *) (ETH_Base + 0x009C))
#define Gmac_Addr11LO_Msk			0xffffffff
#define Gmac_Addr11LO_Rst			0xffffffff

/* GMAC Address12_HI Register Addresses*/ 
#define Gmac_Addr12HI			((volatile unsigned int *) (ETH_Base + 0x00A0))
#define Gmac_Addr12HI_Msk			0xff00ffff
#define Gmac_Addr12HI_Rst			0x0000ffff

/* GMAC Address12_LO Register Addresses*/ 
#define Gmac_Addr12LO			((volatile unsigned int *) (ETH_Base + 0x00A4))
#define Gmac_Addr12LO_Msk			0xffffffff
#define Gmac_Addr12LO_Rst			0xffffffff

/* GMAC Address13_HI Register Addresses*/ 
#define Gmac_Addr13HI			((volatile unsigned int *) (ETH_Base + 0x00A8))
#define Gmac_Addr13HI_Msk			0xff00ffff
#define Gmac_Addr13HI_Rst			0x0000ffff

/* GMAC Address13_LO Register Addresses*/ 
#define Gmac_Addr13LO			((volatile unsigned int *) (ETH_Base + 0x00AC))
#define Gmac_Addr13LO_Msk			0xffffffff
#define Gmac_Addr13LO_Rst			0xffffffff

/* GMAC Address14_HI Register Addresses*/ 
#define Gmac_Addr14HI			((volatile unsigned int *) (ETH_Base + 0x00B0))
#define Gmac_Addr14HI_Msk			0xff00ffff
#define Gmac_Addr14HI_Rst			0x0000ffff

/* GMAC Address14_LO Register Addresses*/ 
#define Gmac_Addr14LO			((volatile unsigned int *) (ETH_Base + 0x00B4))
#define Gmac_Addr14LO_Msk			0xffffffff
#define Gmac_Addr14LO_Rst			0xffffffff

/* GMAC Address15_HI Register Addresses*/ 
#define Gmac_Addr15HI			((volatile unsigned int *) (ETH_Base + 0x00B8))
#define Gmac_Addr15HI_Msk			0xff00ffff
#define Gmac_Addr15HI_Rst			0x0000ffff

/* GMAC Address15_LO Register Addresses*/ 
#define Gmac_Addr15LO			((volatile unsigned int *) (ETH_Base + 0x00BC))
#define Gmac_Addr15LO_Msk			0xffffffff
#define Gmac_Addr15LO_Rst			0xffffffff

/* GMAC MMC registers */ 
#define Gmac_MMC_Ctrl			((volatile unsigned int *) (ETH_Base + 0x100))
#define Gmac_MMC_RxIntrReg			((volatile unsigned int *) (ETH_Base + 0x104))
#define Gmac_MMC_TxIntrReg			((volatile unsigned int *) (ETH_Base + 0x108))
#define Gmac_MMC_RxIntrMask			((volatile unsigned int *) (ETH_Base + 0x10C))
#define Gmac_MMC_TxIntrMask			((volatile unsigned int *) (ETH_Base + 0x110))
/**********************************************************
* DMA Engine descriptors
 **********************************************************/ 
enum DmaDescriptorStatus    /* status word of DMA descriptor */
{
  DescOwnByDma          = 0x80000000,   /* Descriptor is owned by DMA engine  */
	DescDAFilterFail      = 0x40000000,   /* Rx - DA Filter Fail for the received frame        E  */
	DescFrameLengthMask   = 0x3FFF0000,   /* Receive descriptor frame length */
	DescFrameLengthShift  = 16,

  DescError             = 0x00008000,   /* Error summary bit  - OR of the following bits:    v  */
	
  DescRxTruncated       = 0x00004000,   /* Rx - no more descriptors for receive frame        E  */
	DescSAFilterFail      = 0x00002000,   /* Rx - SA Filter Fail for the received frame        E  */
	DescRxLengthError  	  = 0x00001000,   /* Rx - frame size not matching with length field    E  */
	DescRxDamaged         = 0x00000800,   /* Rx - frame was damaged due to buffer overflow     E  */
	DescRxVLANTag         = 0x00000400,   /* Rx - received frame is a VLAN frame               I  */
	DescRxFirst           = 0x00000200,   /* Rx - first descriptor of the frame                I  */
	DescRxLast            = 0x00000100,   /* Rx - last descriptor of the frame                 I  */
	DescRxIPC_Giant       = 0x00000080,   /* Rx - frame is longer than 1518 bytes              E  */
	DescRxCollision       = 0x00000040,   /* Rx - late collision occurred during reception     E  */
	DescRxFrameEther      = 0x00000020,   /* Rx - Frame type - Ethernet, otherwise 802.3          */
	DescRxWatchdog        = 0x00000010,   /* Rx - watchdog timer expired during reception      E  */
	DescRxMiiError        = 0x00000008,   /* Rx - error reported by MII interface              E  */
	DescRxDribbling       = 0x00000004,   /* Rx - frame contains noninteger multiple of 8 bits    */
	DescRxCrc             = 0x00000002,   /* Rx - CRC error                                    E  */
	
  DescTxTimeout         = 0x00004000,   /* Tx - Transmit jabber timeout                      E  */
	DescTxFrameFlushed    = 0x00002000,   /* Tx - DMA/MTL flushed the frame due to SW flush    I  */
	DescTxLostCarrier     = 0x00000800,   /* Tx - carrier lost during tramsmission             E  */
	DescTxNoCarrier       = 0x00000400,   /* Tx - no carrier signal from the tranceiver        E  */
	DescTxLateCollision   = 0x00000200,   /* Tx - transmission aborted due to collision        E  */
	DescTxExcCollisions   = 0x00000100,   /* Tx - transmission aborted after 16 collisions     E  */
	DescTxVLANFrame       = 0x00000080,   /* Tx - VLAN-type frame                                 */
	DescTxCollMask        = 0x00000078,   /* Tx - Collision count                                 */
	DescTxCollShift       = 3,
  DescTxExcDeferral     = 0x00000004,   /* Tx - excessive deferral                           E  */
	DescTxUnderflow       = 0x00000002,   /* Tx - late data arrival from the memory            E  */
	DescTxDeferred        = 0x00000001,   /* Tx - frame transmision deferred                      */
};
//#ifdef CONFIG_ARCH_SPEARBASIC_BA//CONFIG_SPEARBASIC_BA
#ifdef ETH_SPEARBASIC_BA
enum DmaDescriptorLength    /* length word of DMA descriptor */
{
	DescTxIntEnable       = 0x40000000,   /* Tx - interrupt on completion                         */
	DescTxLast            = 0x20000000,   /* Tx - Last segment of the frame                       */
	DescTxFirst           = 0x10000000,   /* Tx - First segment of the frame                      */
	DescTxDisableCrc      = 0x08000000,   /* Tx - Add CRC disabled (first segment only)           */
	DescTxDisablePadd     = 0x04000000,   /* disable tx padding, added by - reyaz */
	DescTxCheckInsCtrl    = 0x00C00000,   /* Checksum Insertion Control           */
	DescTxEndOfRing       = 0x00200000,   /* End of Tx descriptors ring                           */
	DescTxChain				= 0x00100000,   /* Second tx buffer address is chain address            */
	DescTxStatusMask      = 0x0001FFFF,
	
  DescRxEndOfRing       = 0x00008000,   /* disable rx padding, added by - reyaz */
	DescRxChain           = 0x00004000,   /* Second rx buffer address is chain address            */
	
  DescSize2Mask         = 0x1FFF0000,   /* Buffer 2 size                                        */
	DescSize2Shift        = 16,
	DescSize1Mask         = 0x00001FFF,   /* Buffer 1 size                                        */
	DescSize1Shift        = 0
};
#else
enum DmaDescriptorLength    /* length word of DMA descriptor */
{
  DescTxIntEnable       = 0x80000000,   /* Tx - interrupt on completion                         */
	DescTxLast            = 0x40000000,   /* Tx - Last segment of the frame                       */
	DescTxFirst           = 0x20000000,   /* Tx - First segment of the frame                      */
	DescTxDisableCrc      = 0x04000000,   /* Tx - Add CRC disabled (first segment only)           */
	DescEndOfRing         = 0x02000000,   /* End of descriptors ring                              */
	DescChain             = 0x01000000,   /* Second buffer address is chain address               */
	DescTxDisablePadd     = 0x00800000,   /* disable padding, added by - reyaz */
	DescSize2Mask         = 0x003FF800,   /* Buffer 2 size                                        */
	DescSize2Shift        = 11,
  DescSize1Mask         = 0x000007FF,   /* Buffer 1 size                                        */
	DescSize1Shift        = 0,
};
#endif
//*****************************************************************************
//* ICS1893 PHY Layer
//*****************************************************************************

/*
* CONTROL Register (0x0)
*
* [15] Reset
*         		1 = PHY reset
*         		0 = normal operation
* [14] Loopback
*         		1 = enable loopback mode
*         		0 = disable loopback mode
* [13] Speed Selection (LSB)
*         		[6]    [13]
*         		-----------
*         		1       1 = Reserved
*         		1       0 = 1000 Mb/s
*         		0       1 = 100 Mb/s
*         		0       0 = 10 Mb/s
* [12] Auto-Negotiation Enable
*         		1 = Enable Auto-Negotiation Process
*         		0 = Disable Auto-Negotiation Process
* [11] Power Down
*         		1 = power down
*         		0 = normal operation
* [10] Isolate
*        		1 = electrically Isolate PHY from MII or GMII
*         		0 = normal operation
* [9] Restart Auto-Negotiation
*         		1 = Restart Auto-Negotiation Process
*         		0 = normal operation
* [8] Duplex Mode
*         		1 = Full Duplex
*         		0 = Half Duplex
* [7] Collision Test
*         		1 = enable COL signal test
*         		0 = disable COL signal test
* [6] Speed Selection (MSB) (see above ...)
* [5:0] Reserved Write as 0, ignore on Read R/W
*
*/ 
#define Phy_CtrlReg				0x00

#define Phy_Reset					0x8000
#define Phy_Loopback				0x4000
#define Phy_Speed_10Mbs				0x0000
#define Phy_Speed_100Mbs			0x2000
#define Phy_Speed_1000Mbs			0x0040
#define Phy_Speed_Msk				0x2040
#define Phy_AutoNego    			0x1000
#define Phy_PowerDown				0x0800
#define Phy_Isolate     			0x0400
#define Phy_AutoNegoRestart			0x0200
#define Phy_DuplexMode				0x0100
#define Phy_CollTest    			0x0080

/*
* STATUS Register (0x1)
*/ 
#define Phy_StatReg             0x01

#define Phy_AutoNego_Done			0x0020
#define LINK      					0x0004   /* Link status */
#define Phy_id_hi               0x02
#define Phy_id_lo               0x03

/*
* AUTO-NEGOTIATION ADVERTISEMENT Register (0x4)
*
* [15] Next Page
* [14] IEEE reserved
* [13] Remote fault Locally
* [12:10] IEEE reserved
* [ 9] 100Base-T4
* [ 8] 100Base-TX
* [ 7] 100Base-TX
* [ 6] 10Base-T
* [ 5] 10Base-T
* [4:0] Selector Field
*/ 
#define Phy_ANADReg				0x04

#define Phy_Next_Page				0x8000
#define Phy_Remote_fault			0x2000
#define Phy_100Base_T4				0x0200
#define Phy_100Base_TX_Full			0x0100
#define Phy_100Base_TX_Half			0x0080
#define Phy_10Base_T_Full			0x0040
#define Phy_10Base_T_Half			0x0020
#define Phy_Selector				0x0001

/*
* AUTO-NEGOTIATION Link Partner Ability Register (0x5)
*/ 
#define Phy_ANLPAReg            0x05

/*
* AUTO-NEGOTIATION Expansion Register (0x6)
*/ 
#define Phy_ANEXReg             0x6

#define PHY_1000_CtlReg         0x0A
#define Phy_1000_EXREG			0x0F
#define Phy_1000_EXREG			0x0F
#define EEPROM_CHECKSUM_REG     0xff

#define EEPROM_SUM 0xBA

//Arif Added : to be placed in separate headers later
#define  SPEAR_ETH_LENGTH_OF_ADDRESS   6	/* Individual address currently programmed in MAC */
#define SPEAR_EEPROM_SIZE 255

typedef enum
{
#undef FALSE
	FALSE = 0,
#undef TRUE
	TRUE = 1
} boolean_t;

#define SPEAR_WRITE_REG(a,  value) (__raw_writel((value), (a)))
#define SPEAR_READ_REG(a) ( __raw_readl(a))
/* Enums */
typedef enum
{
	spear_mac_unknown = 0,
	spear_82597,
	spear_num_macs
} spear_mac_type;

/* Types of physical layer modules */
typedef enum
{
	spear_phy_type_unknown = 0,
	spear_phy_type_g6005,	/* 850nm, MM fiber, XPAK transceiver */
	spear_phy_type_g6104,	/* 1310nm, SM fiber, XPAK transceiver */
	spear_phy_type_txn17201,	/* 850nm, MM fiber, XPAK transceiver */
	spear_phy_type_txn17401	/* 1310nm, SM fiber, XENPAK transceiver */
} spear_phy_type;


/* Flow Control Settings */
typedef enum
{
	spear_fc_none = 0,
	spear_fc_rx_pause = 1,
	spear_fc_tx_pause = 2,
	spear_fc_full = 3,
	spear_fc_default = 0xFF
} spear_fc_type;

/* PCI bus types */
typedef enum
{
	spear_bus_type_unknown = 0,
	spear_bus_type_pci,
	spear_bus_type_pcix
} spear_bus_type;

/* PCI bus speeds */
typedef enum
{
	spear_bus_speed_unknown = 0,
	spear_bus_speed_33,
	spear_bus_speed_66,
	spear_bus_speed_100,
	spear_bus_speed_133,
	spear_bus_speed_reserved
} spear_bus_speed;

/* PCI bus widths */
typedef enum
{
	spear_bus_width_unknown = 0,
	spear_bus_width_32,
	spear_bus_width_64
} spear_bus_width;

typedef enum
{
	spear_10_half = 0,
	spear_10_full = 1,
	spear_100_half = 2,
	spear_100_full = 3,
	spear_1000_half = 4,
	spear_1000_full =5
} e1000_speed_duplex_type;

/* Bus parameters */
struct spear_bus
{
	spear_bus_speed speed;
	spear_bus_width width;
	spear_bus_type type;
};


/* Flow control parameters */
struct spear_fc
{
	uint32_t high_water;	/* Flow Control High-water          */
	uint32_t low_water;		/* Flow Control Low-water           */
	uint16_t pause_time;	/* Flow Control Pause timer         */
	boolean_t send_xon;		/* Flow control send XON            */
	spear_fc_type type;		/* Type of flow control             */
};

struct spear_hw
{
	uint8_t __iomem *hw_addr;	/* Base Address of the hardware     */
	void *back;			/* Pointer to OS-dependent struct   */
	struct spear_fc fc;		/* Flow control parameters          */
	struct spear_bus bus;	/* Bus parameters                   */
	uint32_t phy_id;		/* Phy Identifier                   */
	uint32_t phy_addr;		/* XGMII address of Phy             */
	spear_mac_type mac_type;	/* Identifier for MAC controller    */
	spear_phy_type phy_type;	/* Transceiver/phy identifier       */
	uint32_t max_frame_size;	/* Maximum frame size supported     */
	uint32_t mc_filter_type;	/* Multicast filter hash type       */
	uint32_t num_mc_addrs;	/* Number of current Multicast addrs */
	uint8_t curr_mac_addr[SPEAR_ETH_LENGTH_OF_ADDRESS];	/* Individual address currently programmed in MAC */
	uint32_t num_tx_desc;	/* Number of Transmit descriptors   */
	uint32_t num_rx_desc;	/* Number of Receive descriptors    */
	uint32_t rx_buffer_size;	/* Size of Receive buffer           */
	boolean_t link_up;		/* TRUE if link is valid            */
	boolean_t adapter_stopped;	/* State of adapter                 */
	uint16_t device_id;		/* device id from PCI configuration space */
	uint16_t vendor_id;		/* vendor id from PCI configuration space */
	uint8_t revision_id;	/* revision id from PCI configuration space */
	uint16_t subsystem_vendor_id;	/* subsystem vendor id from PCI configuration space */
	uint16_t subsystem_id;	/* subsystem id from PCI configuration space */
	uint32_t bar0;		/* Base Address registers           */
	uint32_t bar1;
	uint32_t bar2;
	uint32_t bar3;
	//      uint16_t eeprom[SPEAR_EEPROM_SIZE];     /* EEPROM contents read at init time  */
	struct spear_eeprom_info eeprom;

	unsigned long io_base;	/* Our I/O mapped location */
	uint16_t autoneg;
	uint16_t autoneg_advertised;
	uint8_t forced_speed_duplex;
};

/* Statistics reported by the hardware */
struct spear_hw_stats
{
	uint64_t tprl;
	uint64_t tprh;
	uint64_t gprcl;
	uint64_t gprch;
	uint64_t bprcl;
	uint64_t bprch;
	uint64_t mprcl;
	uint64_t mprch;
	uint64_t uprcl;
	uint64_t uprch;
	uint64_t vprcl;
	uint64_t vprch;
	uint64_t jprcl;
	uint64_t jprch;
	uint64_t gorcl;
	uint64_t gorch;
	uint64_t torl;
	uint64_t torh;
	uint64_t rnbc;
	uint64_t ruc;
	uint64_t roc;
	uint64_t rlec;
	uint64_t crcerrs;
	uint64_t icbc;
	uint64_t ecbc;
	uint64_t mpc;
	uint64_t tptl;
	uint64_t tpth;
	uint64_t gptcl;
	uint64_t gptch;
	uint64_t bptcl;
	uint64_t bptch;
	uint64_t mptcl;
	uint64_t mptch;
	uint64_t uptcl;
	uint64_t uptch;
	uint64_t vptcl;
	uint64_t vptch;
	uint64_t jptcl;
	uint64_t jptch;
	uint64_t gotcl;
	uint64_t gotch;
	uint64_t totl;
	uint64_t toth;
	uint64_t dc;
	uint64_t plt64c;
	uint64_t tsctc;
	uint64_t tsctfc;
	uint64_t ibic;
	uint64_t rfc;
	uint64_t lfc;
	uint64_t pfrc;
	uint64_t pftc;
	uint64_t mcfrc;
	uint64_t mcftc;
	uint64_t xonrxc;
	uint64_t xontxc;
	uint64_t xoffrxc;
	uint64_t xofftxc;
	uint64_t rjc;
};

struct eth_priv 
{

	/* Frame Descriptors */ 
	//DmaMacDescr TX_MAC_DescrTable[TX_MAC_DESCR_NUM];	/* array of TX frame descriptors */
	//DmaMacDescr RX_MAC_DescrTable[RX_MAC_DESCR_NUM];	/* array of RX frame descriptors */
	DmaMacDescr *TX_MAC_DescrTable;	/* array of TX frame descriptors */
	DmaMacDescr *RX_MAC_DescrTable;	/* array of RX frame descriptors */
	dma_addr_t DMA_TX_Descrp;
	dma_addr_t DMA_RX_Descrp;
	volatile unsigned int RX_MAC_currDescNum;
	volatile unsigned int TX_MAC_currDescNum;
	volatile unsigned int TX_MAC_prevDescNum;
	volatile signed int TX_MAC_WinSize;
	int tx_ring_count;
	int rx_ring_count;
	spinlock_t eth_lock;

	/* Tasklets to implement top halves of interrupt handlers */ 
	struct tasklet_struct rx_tasklet;
	struct tasklet_struct tx_tasklet;
	struct timer_list tmr_hotplug;
	unsigned int link_status;

	/* MII interface info */ 
	struct mii_if_info mii;

	/* OS defined structs */ 
	struct net_device *dev;
	//  struct pci_dev *pdev;
	struct net_device_stats stats;

	//Added later
	struct vlan_group *vlgrp;
	//  uint32_t bd_number;
	//  uint32_t rx_buffer_len;
	//  uint32_t part_num;
	//  uint32_t wol;
	//  uint32_t smartspeed;
	//  uint32_t en_mng_pt;
	uint16_t link_speed;
	uint16_t link_duplex;
	//  spinlock_t stats_lock;
	//  atomic_t irq_sem;
	//  struct work_struct tx_timeout_task;
	//  uint8_t fc_autoneg;
	int autoneg_reset;
	uint32_t supported_features;
	uint32_t advertised_features;
	/* TX */
	DmaMacDescr tx_ring;
	uint32_t gotcl;
	/* RX */
	DmaMacDescr rx_ring;
	uint64_t hw_csum_err;
	uint64_t hw_csum_good;
    boolean_t rx_csum;
	uint32_t gorcl;

    /* structs defined in spear_hw.h */
		struct spear_hw hw;
	struct spear_hw_stats hw_stats;
	int msg_enable;
	//Newely Added
	struct mii_phy			phy;
};

/*  ETH Frame Structure */ 
typedef struct __ETHFrame 
{
	u8 m_dstAddr[ETH_ALEN] /*__packed*/;
	u8 m_srcAddr[ETH_ALEN] /*__packed*/;
	u16 m_proto __packed;
	u8 m_payload[ETH_DATA_LEN] /*__packed*/;
} ETHFrame;

typedef struct eth_config
{
	int speed_100;  /* for 100 & 10 */
	int speed_1000;
	int dplx;
	int rx_pause;
	int tx_pause;
} eth_config;

void ETH_RTL_Rxstart(void);
void ETH_RTL_Txstart(void);
void ETH_RTL_TxResume(void);
int ETH_RTL_Reset(void);
void ETH_RTL_Dma_Init(eth_config* ether_config);
void ETH_RTL_Mac_Init(eth_config* ether_config);
u32 ETH_RTL_DmaIntType(void);
u32 ETH_RTL_Read_Mac_Address_Lo(void);
u32 ETH_RTL_Read_Mac_Address_Hi(void);
void ETH_RTL_Set_Mac_Address(u32 AddrHi,u32 AddrLO);
void  ETH_RTL_Set_RxDescrListAddr(u32 val);
void  ETH_RTL_Set_TxDescrListAddr(u32 val);
void ETH_RTL_Mdio_write (u8 PHY_Num,u8 PHY_Reg, u16 data);
u16 ETH_RTL_Mdio_Read( volatile u8 PHY_Num, volatile u8 PHY_Reg );

#ifdef UBOOT_TEST
void dump_registers (void);
#endif //(UBOOT_TEST)

int ETH_RTL_DmaRxValid(u32 Status);
int ETH_RTL_SpearDmaRxCollisions(u32 Status);
int ETH_RTL_SpearDmaRxCrc(u32 Status);
int ETH_RTL_SpearDmaRxFrame(u32 Status);
int ETH_RTL_SpearDmaRxLengthError(u32 Status);
int ETH_RTL_SpearDmaTxValid(u32 Status);
int ETH_RTL_SpearDmaTxCollisions(u32 Status);
int ETH_RTL_SpearDmaTxAborted(u32 Status);
int ETH_RTL_SpearDmaTxCarrier(u32 Status);



u16 ETH_RTL_Mdio_Read( volatile u8 PHY_Num, volatile u8 PHY_Reg );
void ETH_RTL_Mdio_write (u8 PHY_Num,u8 PHY_Reg, u16 data);

void ETH_RTL_Rxstart(void );
void ETH_RTL_Txstart(void );
void ETH_RTL_TxResume( void);
int ETH_RTL_Reset(void);
void ETH_RTL_Dma_Init(eth_config* ether_config);
void ETH_RTL_Mac_Init(eth_config* ether_config);
u32 ETH_RTL_DmaIntType(void);
u32 ETH_RTL_Read_Mac_Address_Lo(void);
u32 ETH_RTL_Read_Mac_Address_Hi(void);
void ETH_RTL_Set_Mac_Address(u32 AddrHi,u32 AddrLO);
void  ETH_RTL_Set_RxDescrListAddr(u32 val);
void  ETH_RTL_Set_TxDescrListAddr(u32 val);
int ETH_RTL_DmaRxValid(u32 Status);
int ETH_RTL_SpearDmaRxCollisions(u32 Status);
int ETH_RTL_SpearDmaRxCrc(u32 Status);
int ETH_RTL_SpearDmaRxFrame(u32 Status);
int ETH_RTL_SpearDmaRxLengthError(u32 Status);
int ETH_RTL_SpearDmaTxValid(u32 Status);
int ETH_RTL_SpearDmaTxCollisions(u32 Status);
int ETH_RTL_SpearDmaTxAborted(u32 Status);
int ETH_RTL_SpearDmaTxCarrier(u32 Status);
void SPEAR_RTL_ETH_Dump_Registers(struct net_device *dev);
s8 rtl8368_populatePriv(struct  eth_priv *priv, u8 phyAddr);
void rtl8368_printPHY2Regs(struct eth_priv *priv);
void enable_mac_ras(void);

#endif	/* __SPR_870_ETH_SYN_HEADER */ 

/* End of file - spr_870_eth_syn.h */ 
