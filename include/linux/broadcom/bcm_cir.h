/*****************************************************************************
* Copyright 2004 - 2009 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

/*
 * Public header of the Board Specific Configuration (BSC) driver. The driver
 * provides a "register" and "query" mechanism for users to pass BSC
 * parameters
 */

#ifndef _BCM_CIR_H_
#define _BCM_CIR_H_

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>

#define BCM_CIR_DEV_NAME                 "cir"
#define BCM_CIR_STATIC_DMA_MEMORY        7168	/* 7K */

#define SIRC_TX_CARRIER_FREQ              40000	/* 40 KHz */
#define SIRC_MAX_PULSE_WIDTH              0x7B00
#define SIRC_MIN_PULSE_WIDTH              0x0210

#define NEC_TX_CARRIER_FREQ               38000	/* 38 KHz */
#define NEC_MAX_PULSE_WIDTH               0x7B00
#define NEC_MIN_PULSE_WIDTH               0x0100

#define BCM_CIR_CALC_TON(clkfreq,x)     ((uint32_t) ((clkfreq)/(2 * (x))))
#define BCM_CIR_CALC_TOFF(clkfreq,x)    ((uint32_t) ((clkfreq)/(2 * (x))))
#define BCM_CIR_CALC_SAMPLING_FREQ(clk) ((uint32_t) ((clk/2500000) - 1))	/* Sample at 2.5MHz */

#define KPM_APB1_CLK_FREQ                78000000

#define BCM_CIR_TX_BLOCK                1
#define BCM_CIR_RX_BLOCK                0

#define BCM_INT_ID_CIR                   BCM_INT_ID_RESERVED157

/*
 * IR Protocol type
 */
typedef enum bcm_cir_device {
	BCM_IR_DEV_DISABLED, /**< block disabled */
	BCM_IR_DEV_ENABLED,  /**< block enabled  */
	BCM_IR_DEV_SIRC,     /**< SIRC device    */
	BCM_IR_DEV_NEC,      /**< NEC device     */

	/* For internal use */
	BCM_IR_DEV_CUSTOM,   /**< block status if custom configured */
} BCM_CIR_DEV_T;

/*
 * Block status
 */
typedef struct bcm_cir_mode {
	enum bcm_cir_device tx_mode;
	enum bcm_cir_device rx_mode;
} BCM_CIR_MODE_T;

/*
 * Transmitter configuration
 */
typedef struct bcm_cir_tx_conf {
	uint32_t mode;	         /**< 0:Carrier-less mode,1:Carrier Mode */
	uint32_t samplingPeriod; /**< TX Sampling period (used only for Carrierless mode) */
	uint32_t carrFreq;       /**< Carrier frequency, eg: 40000,38000... */
} BCM_CIR_TX_CONF_T;

/*
 * Receiver configuration
 */
typedef struct bcm_cir_rx_config {
	uint32_t activeState;    /**< recever input active condition */
	uint32_t samplingPeriod; /**< RX Sampling period. Usually 2.5MHz */
	uint16_t minPulseWidth;  /**< Minimum Pulse width */
	uint16_t maxPulseWidth;  /**< Maximum Pulse width. Max pulse width will cause CIR to stop reception */
	uint32_t carrFreq;       /**< '0' for raw mode or Carrier frequency, eg: 40000,38000... */
	uint32_t pwNoise;
	uint32_t debug; 
	uint16_t hi_top;
	uint16_t hi_bottom;
	uint16_t lo_top;
	uint16_t lo_bottom;
	uint16_t dtv_end;
} BCM_CIR_RX_CONF_T;

#define BCM_CIR_MAGIC            'i'

#define BCM_CIR_SET_IR_DEV       0x90
#define BCM_CIR_GET_IR_DEV       0x91
#define BCM_CIR_DISABLE_BLOCK    0x92
#define BCM_CIR_ENABLE_BLOCK     0x93

#define BCM_CIR_CONF_TX          0x94
#define BCM_CIR_SET_TX_DMA_MEM   0x95

#define BCM_CIR_CONF_RX          0x96
#define BCM_CIR_SET_RX_DMA_MEM   0x97

#define BCM_CIR_IOCTL_SET_IR_DEV      _IOW (BCM_CIR_MAGIC, BCM_CIR_SET_IR_DEV,     BCM_CIR_DEV_T)     /**< Set IR device protocol */
#define BCM_CIR_IOCTL_GET_IR_DEV      _IOWR(BCM_CIR_MAGIC, BCM_CIR_GET_IR_DEV,     BCM_CIR_MODE_T)    /**< Get block protocol */
#define BCM_CIR_IOCTL_DISABLE_BLOCK   _IOW (BCM_CIR_MAGIC, BCM_CIR_DISABLE_BLOCK,  char)	      /**< 1-TX, 0-RX: disable block */
#define BCM_CIR_IOCTL_ENABLE_BLOCK    _IOW (BCM_CIR_MAGIC, BCM_CIR_ENABLE_BLOCK,  char)		      /**< 1-TX, 0-RX: disable block */

#define BCM_CIR_IOCTL_CONF_TX         _IOW (BCM_CIR_MAGIC, BCM_CIR_CONF_TX,        BCM_CIR_TX_CONF_T) /**< Configure transmitter */
#define BCM_CIR_IOCTL_SET_TX_DMA_MEM  _IOW (BCM_CIR_MAGIC, BCM_CIR_SET_TX_DMA_MEM, unsigned int)      /**< TX DMA static memory size */

#define BCM_CIR_IOCTL_CONF_RX         _IOW (BCM_CIR_MAGIC, BCM_CIR_CONF_RX,        BCM_CIR_RX_CONF_T) /**< Configure receiver */
#define BCM_CIR_IOCTL_SET_RX_DMA_MEM  _IOW (BCM_CIR_MAGIC, BCM_CIR_SET_RX_DMA_MEM, unsigned int)      /**< RX DMA static memory size */

#endif /* _BCM_CIR_H_ */
