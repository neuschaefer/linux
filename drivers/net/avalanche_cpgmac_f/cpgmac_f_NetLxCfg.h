/*
 *
 * cpmacNetLxCfg.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


/** \file   cpmacNetLxCfg.h
    \brief  CPMAC Linux DDA Configuration Header file

    This file is a configuration header for the device driver adaptation for
    Linux for CPMAC/CPGMAC device based upon PSP Framework architecture.

    Acknowledgements: This DDA implementation for CP(G)MAC device is based upon
    Linux device driver for CP(G)MAC written using HAL 2.0.


    @author     Anant Gole
    @version    0.1

            (ver 0.2)   Sharath Kumar - Incorporated review comments
    @version    0.2

 */

#ifndef _CPMAC_LX_DDA_CFG_H_
#define _CPMAC_LX_DDA_CFG_H_

#include <asm-arm/arch-avalanche/puma5/puma5.h>

#if defined(CONFIG_ARM_AVALANCHE_MARVELL_6063) || defined(CONFIG_ARM_AVALANCHE_MARVELL_6060)
#define EGRESS_TRAILOR_LEN                  4
#else
#define EGRESS_TRAILOR_LEN                  0
#endif

#ifdef CONFIG_ARM_AR7WRD
#define CFG_START_LINK_SPEED                (SNWAY_NOPHY)
#else
#define CFG_START_LINK_SPEED                (SNWAY_AUTOALL)     /* auto nego */
#endif

#if defined (CONFIG_MACH_PUMA5_VOLCANO)
#define 	CPMAC_MDIO_CLOCK_FREQUENCY			0x00000000
#endif

#if defined (CONFIG_MACH_PUMA5EVM)
#define 	CPMAC_MDIO_CLOCK_FREQUENCY			2200000
#endif


#define  	CPMAC_DDA_DEFAULT_VLAN_ENABLE			FALSE
#define		CPMAC_DEFAULT_MLINK_MASK			0
#define		CPMAC_DEFAULT_COPY_MAC_CONTROL_FRAMES_ENABLE	FALSE
#define		CPMAC_DEFAULT_COPY_SHORT_FRAMES_ENABLE		FALSE
#define		CPMAC_DEFAULT_COPY_ERROR_FRAMES_ENABLE		FALSE
#define		CPMAC_DEFAULT_PROMISCOUS_CHANNEL		0
#define		CPMAC_DEFAULT_MULTICAST_CHANNEL			0
#define  	CPMAC_DEFAULT_BROADCAST_CHANNEL			0
#define		CPMAC_DEFAULT_RX_FLOW_THRESHOLD			0
#define		CPMAC_DEFAULT_HI_PRI_THRESHOLD			0
#define		CPMAC_DEFAULT_VLAN_ENABLE			FALSE
#define		CPMAC_DEFAULT_EXT_ENABLE			FALSE
#define		CPMAC_DEFAULT_GIG_FORCE				FALSE
#define		CPMAC_DEFAULT_RX_FIFO_FLOW_ENABLE		FALSE
#define		CPMAC_DEFAULT_TX_SHORT_GAP_ENABLE		FALSE
#define 	LOW_CPMAC_DEFAULT_TX_PACING_ENABLE     	        FALSE
#define 	HIGH_CPMAC_DEFAULT_TX_PACING_ENABLE    	        FALSE

#define CPMAC_DEFAULT_TX_PACING_ENABLE(inst)        \
    ((inst == 0) ? LOW_CPMAC_DEFAULT_TX_PACING_ENABLE : HIGH_CPMAC_DEFAULT_TX_PACING_ENABLE)

#define		CPMAC_DEFAULT_TX_FLOW_ENABLE			FALSE
#define		CPMAC_DEFAULT_RX_FLOW_ENABLE			FALSE
#define		CPMAC_DEFAULT_LOOPBACK_ENABLE			FALSE
#define		CPMAC_DEFAULT_CMD_IDLE			        FALSE
#define		CPMAC_DEFAULT_IFCTL_B				FALSE
#define		CPMAC_DEFAULT_IFCTL_A				FALSE

#define		CPMAC_DEFAULT_VLAN_PORT				0

#define 	CONFIG_CPMAC_MIB_TIMER_TIMEOUT                 	5000    /* 5 seconds should be enough */

#define 	CPMAC_DEFAULT_PROMISCOUS_ENABLE                	0
#define 	CPMAC_DEFAULT_BROADCAST_ENABLE                 	1
#define 	CPMAC_DEFAULT_MULTICAST_ENABLE                 	1


/* Default values to be passed to CPPI4 init configuration structure. */

#if defined (CONFIG_CPU_BIG_ENDIAN)
#define		CPMAC_DEFAULT_CPPI4_ENDIAN			1       /* 0 = host mode 1= embedded mode */
#else
#define		CPMAC_DEFAULT_CPPI4_ENDIAN			0       /* 0 = host mode 1= embedded mode */
#endif
#define		CPMAC_DEFAULT_CPPI4_MODE			0       /* 0 = little 1 = big   */
#define		CPMAC_DEFAULT_CPPI4_DEST_COPY_BYTE_COUNT	0       /* used for host packet type only */
#define		CPMAC_DEFAULT_CPPI4_ETHERNET_PKT_TYPE		7       /*7 = Ethernet pkt type */
#define		CPMAC_DEFAULT_CPPI4_ERR_HANDLING		0       /* 0 = drop err packet 1 = pass error packet */
#define		CPMAC_DEFAULT_CPPI4_SOP_OFFSET			0

#ifdef CONFIG_ARM_AVALANCHE_PPD
#define         PP_DMA_BLOCK_NUM                                0
#define         CPMAC_INFRA_CH_DMA_BLOCK_NUM                    1
#endif
#define         CPMAC_RX_DMA_BLOCK_NUM                          1
#define         CPMAC_TX_DMA_BLOCK_NUM                          0
#define         LOW_CPMAC_DEFAULT_CPPI4_TX_CH_NUM               CPGMAC_CPPI41_TX_DMA_CHNUM
#define 	LOW_CPMAC_DEFAULT_CPPI4_RX_CH_NUM               CPGMAC_CPPI41_RX_DMA_CHNUM
#define 	LOW_CPMAC_DEFAULT_CPPI4_CQ_INDEX    	        0x0

#define 	LOW_CPMAC_DEFAULT_CPPI4_FBQ_INDEX0		0x0
#define 	LOW_CPMAC_DEFAULT_CPPI4_FBQ_INDEX1		0x0
#define 	LOW_CPMAC_DEFAULT_CPPI4_FBQ_INDEX2		0x0
#define 	LOW_CPMAC_DEFAULT_CPPI4_FBQ_INDEX3		0x0

#define 	LOW_CPMAC_DEFAULT_CPPI4_RQ_INDEX		0x00



#define 	HIGH_CPMAC_DEFAULT_CPPI4_CH_NUM                 17
#define 	HIGH_CPMAC_DEFAULT_CPPI4_CQ_INDEX		0x01

#define 	HIGH_CPMAC_DEFAULT_CPPI4_FBQ_INDEX0		0x01
#define 	HIGH_CPMAC_DEFAULT_CPPI4_FBQ_INDEX1		0x01
#define 	HIGH_CPMAC_DEFAULT_CPPI4_FBQ_INDEX2		0x01
#define 	HIGH_CPMAC_DEFAULT_CPPI4_FBQ_INDEX3		0x01

#define 	HIGH_CPMAC_DEFAULT_CPPI4_RQ_INDEX		0x01


#define		CPMAC_DDA_DEFAULT_MAX_RX_FRAGS			1

/* DS Traffic priority */
#define 	CPMAC_DEFAULT_CPPI4_TX_PRI_QUEUE		0       /* Priority queue 0 */
#ifdef CONFIG_ARM_CPMAC_DS_TRAFFIC_PRIORITY
#define     CPMAC_HIGH_PRIORITY_CPPI4_TX_PRI_QUEUE  0       /* High */
#define     CPMAC_LOW_PRIORITY_CPPI4_TX_PRI_QUEUE   1       /* Low */
#define     CPMAC_MIN_FREE_TX_BD               ( CPMAC_TX_BD_NUM - 8 )        /* Minimum free TX BDs */
#define     CPMAC_HIGH_PRIORITY_MAX_QUEUE      ( CPMAC_TX_BD_NUM * 7 / 10 )
#define     CPMAC_LOW_PRIORITY_MAX_QUEUE       ( CPMAC_TX_BD_NUM * 2 / 10 )
#define     CPMAC_HIGH_PRIORITY_QUEUE_STATUS   ( AVALANCHE_NWSS_QSTATUS_RGN_BASE + ( CPMAC_CPPI4x_TX_QNUM( CPMAC_HIGH_PRIORITY_CPPI4_TX_PRI_QUEUE ) << 4 ) )
#define     CPMAC_LOW_PRIORITY_QUEUE_STATUS    ( AVALANCHE_NWSS_QSTATUS_RGN_BASE + ( CPMAC_CPPI4x_TX_QNUM( CPMAC_LOW_PRIORITY_CPPI4_TX_PRI_QUEUE ) << 4 ) )
#endif

#define     CPMAC_QOS_ACTIVE                        (1 << 24)

/* Constants defined for passing it to DDC_cpmacSend from cpamc_dev_tx function as SendArgs */
#define 	CPMAC_DEFAULT_CPPI4_ETHERNET_PKT_TYPE_BD	7       /* Ehernet packet type */
#define		CPMAC_DEFAULT_CPPI4_DESC_TYPE			0       /* descriptor type 0-host 1-embedded */
#define		CPMAC_DEFAULT_CPPI4_ADD_BUFFER_CNT		0       /*  0 for host mode   */
#define 	CPMAC_DEFAULT_PRO_SPEC_REGION_OFF		0       /* 0 for host mode */
#define		CPMAC_DEFAULT_CPPI4_PASS_CRC			0       /* 0- Mac calculates crc */
#define		CPMAC_DEFAULT_CPPI4_VLAN_TAG			0       /* 0-no modification of packet 1- remove vlan tag */



/* Linux invalidate function. This macro is provided for easy configurability */
#define CPMAC_DDA_CACHE_INVALIDATE(addr, size)      dma_cache_inv((unsigned long)addr, size);

/* Linux writeback function. This macro is provided for easy configurability */
#if 0
#define CPMAC_DDA_CACHE_WRITEBACK(addr, size)       dma_cache_wback_inv((unsigned long)skb->data, skb->len);
#else
#define CPMAC_DDA_CACHE_WRITEBACK(addr, size)       dma_cache_wback((unsigned long)addr, size);
#endif

/* NOT EXPLICIT SUPPORT PROVIDED AS OF NOW - Vlan support in the driver */
#define CPMAC_DDA_DEFAULT_VLAN_ENABLE       FALSE

/* System value for ticks per seconds */
#define CPMAC_DDA_TICKS_PER_SEC             HZ

/* CPMAC new Ioctl's created - using a value with a base that can be adjusted as per needs */
#define CPMAC_DDA_IOCTL_BASE                0

/* Filtering IOCTL */
#define CPMAC_DDA_PRIV_FILTERING            (CPMAC_DDA_IOCTL_BASE + 1)

/* Read/Write MII */
#define CPMAC_DDA_PRIV_MII_READ             (CPMAC_DDA_IOCTL_BASE + 2)
#define CPMAC_DDA_PRIV_MII_WRITE            (CPMAC_DDA_IOCTL_BASE + 3)

/* Get/Clear Statistics */
#define CPMAC_DDA_PRIV_GET_STATS            (CPMAC_DDA_IOCTL_BASE + 4)
#define CPMAC_DDA_PRIV_CLR_STATS            (CPMAC_DDA_IOCTL_BASE + 5)

/* External Switch configuration */
#define CPMAC_DDA_EXTERNAL_SWITCH           (CPMAC_DDA_IOCTL_BASE + 6)

/* Change the CPMAC mode from tasklet to interrupt mode */
#define CPMAC_DDA_SET_ISR_MODE              (CPMAC_DDA_IOCTL_BASE + 7)

/* Change the CPMAC mode from interrupt to tasklet mode */
#define CPMAC_DDA_SET_TASKLET_MODE           (CPMAC_DDA_IOCTL_BASE + 8)

/* Add RX buffers descriptor  */
#define CPMAC_DDA_ADD_RX_BD                  (CPMAC_DDA_IOCTL_BASE + 9)

/* Default max frame size = 1522 = 1500 byte data + 14 byte eth header + 4 byte checksum + 4 byte Vlan tag  + Feature */
/* In case of WAN<->LAN mode we use the EGRESS_TRAILOR_LEN to the header */
#ifdef CONFIG_INTEL_PP_TUNNEL_SUPPORT  // Support 1800 MTU
#define CPMAC_DDA_DEFAULT_MAX_FRAME_SIZE    (1800 + 14 + 4 + 4 + EGRESS_TRAILOR_LEN)
#else
#define CPMAC_DDA_DEFAULT_MAX_FRAME_SIZE    (1500 + 14 + 4 + 4 + EGRESS_TRAILOR_LEN)
#endif

/* Default extra bytes allocated for each RX buffer */
#define CPMAC_DDA_DEFAULT_EXTRA_RXBUF_SIZE      0

/* Default number of TX channels */
#define CPMAC_DDA_DEFAULT_NUM_TX_CHANNELS       1

/* Default TX channel number */
#define CPMAC_DDA_DEFAULT_TX_CHANNEL            0

/* Default TX number of BD's/Buffers */
#define LOW_CPMAC_DDA_DEFAULT_TX_NUM_BD         CPMAC_TX_BD_NUM

#define LOW_CPMAC_DDA_DEFAULT_TX_MAX_SERVICE    (CPGMAC_ACC_TX_MAXENTRIES-1) 

#define HIGH_CPMAC_DDA_DEFAULT_TX_NUM_BD        CPMAC_TX_BD_NUM

#define HIGH_CPMAC_DDA_DEFAULT_TX_MAX_SERVICE   (CPGMAC_ACC_TX_MAXENTRIES-1)

/* Default number of RX channels */
#define CPMAC_DDA_DEFAULT_NUM_RX_CHANNELS       1

/* Default RX channel number */
#define CPMAC_DDA_DEFAULT_RX_CHANNEL            0

#define HIGH_CPMAC_DDA_DEFAULT_RX_NUM_BD        CPMAC_RX_BD_NUM
#define LOW_CPMAC_DDA_DEFAULT_RX_NUM_BD         CPMAC_RX_BD_NUM


/* Default RX max service BD's */
#define LOW_CPMAC_DDA_DEFAULT_RX_MAX_SERVICE    (CPGMAC_ACC_RX_MAXENTRIES-1) 
#define HIGH_CPMAC_DDA_DEFAULT_RX_MAX_SERVICE   (CPGMAC_ACC_RX_MAXENTRIES-1)

/*
 * Size of CPMAC peripheral footprint in memory that needs to be reserved in Linux
 * Note that this value is actually a hardware memory footprint value taken from the specs
 * and ideally should have been in the csl files. Keeping it for convinience since CPMAC
 * peripheral footprint will not change unless the peripheral itself changes drastically
 * and it will be called with a different name and will have a different driver anyway
 */
#define CPMAC_DDA_DEFAULT_CPMAC_SIZE        0x800

/* ENV variable names for obtaining MAC Addresses */
#define CPMAC_DDA_MAC_ADDR_A    "maca"
#define CPMAC_DDA_MAC_ADDR_B    "macb"
#define CPMAC_DDA_MAC_ADDR_C    "macc"
#define CPMAC_DDA_MAC_ADDR_D    "macd"
#define CPMAC_DDA_MAC_ADDR_E    "mace"
#define CPMAC_DDA_MAC_ADDR_F    "macf"

/* ENV variable names for obtaining config string when not using config service */
#define CPMAC_DDA_CONFIG_A      "MACCFG_A"
#define CPMAC_DDA_CONFIG_B      "MACCFG_B"
#define CPMAC_DDA_CONFIG_C      "MACCFG_C"
#define CPMAC_DDA_CONFIG_D      "MACCFG_D"
#define CPMAC_DDA_CONFIG_E      "MACCFG_E"
#define CPMAC_DDA_CONFIG_F      "MACCFG_F"

/* Maximum multicast addresses list to be handled by the driver - If this is not restricted
 * then the driver will spend considerable time in handling multicast lists
 */
#define CPMAC_DDA_DEFAULT_MAX_MULTICAST_ADDRESSES   64

#endif /* _CPMAC_LX_DDA_CFG_H_ */
