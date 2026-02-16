/*
 *
 * ddc_cpgmac_f_Cfg.h
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


/** \file   ddc_cpgmac_f_Cfg.h
    \brief  DDC CPGMAC_F Configuration header file

    This file has the static (compile time) configuration parameters for the
    CPGMAC_F DDC driver.

    @author     Greg Guyotte
 */

#ifndef __DDC_CPGMAC_F_CFG_H__
#define __DDC_CPGMAC_G_CFG_H__

/* Config macros */
#define CPMAC_MAX_INSTANCES                 2       /**< Max CPMAC instances */
#define CPMAC_MIN_ETHERNET_PKT_SIZE         60      /**< Minimum Ethernet packet size */

/* CPMAC hardware specific */
#define CPMAC_MAX_TX_CHANNELS               1       /**< Maximum TX Channels supported by the DDC */
#define CPMAC_MAX_RX_CHANNELS               1       /**< Maximum RX Channels supported by the DDC */
#define CPMAC_MAX_NUM_ADDRESSES             8       /**< Maximum number of MAC address supported for addr filtering. */
#define CPMAC_MIN_FREQUENCY_FOR_10MBPS      5500000     /**< Minimum CPMAC bus frequency for 10   Mbps operation is 5.5 MHz (as per specs) */
#define CPMAC_MIN_FREQUENCY_FOR_100MBPS     55000000    /**< Minimum CPMAC bus frequency for 100  Mbps operation is 55  MHz (as per specs) */
#define CPMAC_MIN_FREQUENCY_FOR_1000MBPS    125000000   /**< Minimum CPMAC bus frequency for 1000 Mbps operation is 125 MHz (as per specs) */

#ifndef PRIVATE
#define PRIVATE   static
#endif

/* New CPGMAC driver configuration (Cppi4.1) */    
#define CPMAC_CPPI41_DEF_RXDESC         CPPI41_DESC_TYPE_HOST       /* Host type */
#define CPMAC_CPPI41_DEF_TXDESC         CPPI41_DESC_TYPE_HOST       /* Host type */
#define CPMAC_CPPI41_NUM_TXQS           2
#define CPGMAC_MAX_TX_QS                2

/* CPPI4 Buffer Descriptor Macros */
#define CPPI4_BD_BUF_SIZE_MASK          0xFFFF
#define CPPI4_BD_PKT_LENGTH_MASK        0x3FFFFF
#ifdef CONFIG_ARM_AVALANCHE_PPD/* Session router specific : word 8 and word 9 also 
 * needs to be invalidated as these words contain
 * EPI
 */
#define CPPI4_BD_LENGTH_FOR_CACHE       64
#else
/* Only CPPI specified bytes need to be invalidated */
#define CPPI4_BD_LENGTH_FOR_CACHE       32
#endif

#if 0

/* CPPI41 related MACROS used by CPGMAC */
#define CPPI4_DESC_TYPE_SHIFT_HOST      27
#define CPPI4_DESC_TYPE_HOST            16
#define CPPI4_PKT_TYPE_SHIFT            26
#define CPPI4_PKT_TYPE_ETH              7
#define CPPI4_PKT_RETPLCY_SHIFT         15
#define CPPI4_PKT_RETPLCY_FULL          0
#define CPPI4_DESC_LOC_SHIFT            14
#define CPPI4_DESC_LOC_OFFCHIP          0
#define CPPI4_PKT_RETQMGR_SHIFT         12
#define CPPI4_PKT_RETQ_SHIFT            0
#define CPPI4_DESC_TYPE_SHIFT_TD        27
#define CPPI4_DESC_TYPE_MASK_TD         (0x1F << CPPI4_DESC_TYPE_SHIFT_TD)
#define CPPI4_DESC_TYPE_TD              19

#endif

/* CPGMAC Configuration */
#define CPGMAC_NUM_RXFDB_QS             1

/* Accumulator configuration.
 * Note - The driver assumes 1 interrupt per list page. So care must be taken
 * not to do EOI till complete list page is processed. For example, if
 * txServiceMax value controls number of packets processed per tx interrupt,
 * then it should never be less than CPGMAC_ACC_TX_MAXENTRIES 
 */
#define CPGMAC_ACC_ENTRY_SIZE_VAL       PAL_CPPI41_ACC_ENTRY_TYPE_D
#define CPGMAC_ACC_ENTRY_SIZE           4   /* Number of bytes per entry. 
                                               = 4 for CPGMAC_ACC_ENTRY_D */
#define CPGMAC_ACC_ENTRY_THRSH          16
#define CPGMAC_ACC_LIST_NULL_TERM       0
#define CPGMAC_ACC_TX_MAXENTRIES        32 /*16*/
#define CPGMAC_ACC_RX_MAXENTRIES        32 /*16*/
#define CPGMAC_ACC_PACE_LASTINTR        1
#define CPGMAC_ACC_INTR_DELAY           40
#define CPGMAC_ACC_LIST_DIV             2
#define CPGMAC_ACC_STALL_AVOID          1
#define CPGMAC_ACC_LIST_MODE            CPGMAC_ACC_LIST_NULL_TERM
#define CPGMAC_ACC_INTR_MODE            CPGMAC_ACC_PACE_LASTINTR

/* CPPI41 QM config during PUSH */
#define CPPI41_QM_HDESC_SIZE            40
/* desc_size_val value to be programmed during PUSH: 
 * desc_size_val = (desc_size - 24)/4. Thus, in our case, 
 * (CPPI41_QM_HDESC_SIZE - 24)/4 = 4
 */
#define CPPI41_QM_HDESC_SIZE_VAL        8 //SRCNG -> from 4 to 8

/***
 * The configurations below are indeed system level. For timebeing keeping here
 */ 
#define CPGMAC_INTD_HOST_NUM     0
/* Interrupt configuration for CPGMAC using Accumulator PDSP  <-> intd
 * interface. NOTE since CPMAC_ACC_CHNUM = CPGMAC_ACC_INTD_NUM, the code uses accChanNum
 */
#define CPGMAC_ACCRX_INTD_NUM           CPMAC_ACC_RX_CHNUM
#define CPGMAC_ACCTX_INTD_NUM           CPMAC_ACC_TX_CHNUM 
/* _Remember_ the VEC values directly depend on NUM values */ 
#define CPGMAC_RXINT_VEC                (AVALANCHE_INTD_BASE_INT + CPMAC_ACC_RX_INTV) 
#define CPGMAC_TXINT_VEC                (AVALANCHE_INTD_BASE_INT + CPMAC_ACC_TXCMPL_INTV)
#define AVALANCHE_NWSS_RX0              CPGMAC_RXINT_VEC
#define AVALANCHE_NWSS_TX0              CPGMAC_TXINT_VEC

#ifdef CONFIG_ARM_AVALANCHE_PPD
#define CPGMAC_CPPI41_RX_DMA_CHNUM      CPMAC_CPPI4x_ETH2HOST_PROXY_CHNUM(0)
#define CDMA_CH_INF0                	CPGMAC_CPPI41_RX_DMA_CHNUM
#define CDMA_CH_EMAC0               	8
#define CPGMAC_TX_INFRA_CHNUM           CPMAC_CPPI4x_ETH2HOST_PROXY_CHNUM(0)
#else
#define CPGMAC_CPPI41_RX_DMA_CHNUM         8
#endif
#define CPGMAC_CPPI41_TX_DMA_CHNUM         8
#ifdef CPMAC_POLL_MODE
#define INTDBASE            AVALANCHE_INTD_BASE
#define INTDSTATUS          (INTDBASE+0x204)
#endif

#define EPI_HEADER_LEN      8

#ifndef PRIVATE
#define PRIVATE   static
#endif


#endif /* __DDC_CPGMAC_F_CFG_H__ */
