/*****************************************************************************
*  Copyright 2001 - 2011 Broadcom Corporation.  All rights reserved.
*
*  Unless you and Broadcom execute a separate written software license
*  agreement governing use of this software, this software is licensed to you
*  under the terms of the GNU General Public License version 2, available at
*  http://www.gnu.org/licenses/old-license/gpl-2.0.html (the "GPL").
*
*  Notwithstanding the above, under no circumstances may you combine this
*  software in any way with any other Broadcom software provided under a
*  license other than the GPL, without Broadcom's express prior written
*  consent.
*
*****************************************************************************/

/****************************************************************************/
/**
 *  @file    chal_cir.c
 *
 *  @brief   Low level Consumer IR driver routines
 *
 *  @note
 ****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <chal/chal_cir.h>

/* ---- External Function Prototypes ------------------------------------- */

/* ---- External Variable Declarations ----------------------------------- */

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */

/* ==== Public Functions ================================================= */

/****************************************************************************
 * See chal_cir.h for API documentation.
 ***************************************************************************/

/****************************************************************************/
/**
*  @brief   Configure only the TX CIR block
*
*  @param   handle      (in) CIR handle
*  @param   txConfig    (in) TX Block Configuration
*
*  @return  < 0 if an error, >=0 otherwise
*
*  @note
*
****************************************************************************/
CHAL_CIR_ERR_t
chal_cir_tx_config(const CHAL_CIR_HANDLE_t *handle,
		   const CHAL_CIR_TX_CONFIG_t *txConfig)
{
	CHAL_CIR_ERR_t err = CHAL_CIR_ERR_NONE;

	if ((NULL == handle) || (NULL == txConfig)) {
		err = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		(void)chal_cir_tx_reset(handle); /* Reset the Transmitter */

		mdelay(5);	/* Wait some time for the reset */

		/* Set up Fifo Threshold */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_TX_FIFO_CTRL_REG_OFFSET),
				 (((txConfig->fifoThreshold) <<
				   CIR_TX_FIFO_CTRL_THRESHOLD_SHIFT) &
				  CIR_TX_FIFO_CTRL_REG_RESERVED_AND_MASK));

		/* Set Mode */
		if (CHAL_CIR_TX_MODE_CARRIERLESS == txConfig->mode) {
			CIR_REG_CLRBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_TX_MODE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
		} else {
			CIR_REG_SETBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_TX_MODE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
		}

		/* Set up Sampling Frequence */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_TX_PERIOD_REG_OFFSET),
				 ((txConfig->samplingPeriod) &
				  CIR_TX_PERIOD_REG_RESERVED_AND_MASK));

		/* Set up TC_on */
		CHAL_REG_WRITE32((handle->regBaseAddr + CIR_TC_ON_REG_OFFSET),
				 ((txConfig->tcOnTime) &
				  CIR_TC_ON_REG_RESERVED_AND_MASK));

		/* Set up TC_off */
		CHAL_REG_WRITE32((handle->regBaseAddr + CIR_TC_OFF_REG_OFFSET),
				 ((txConfig->tcOffTime) &
				  CIR_TC_OFF_REG_RESERVED_AND_MASK));

		/* DMA Enable - Disable */
		if (0 != txConfig->dmaEnable)
			(void)chal_cir_tx_dma_enable(handle);
		else
			(void)chal_cir_tx_dma_disable(handle);


		/* Finally Enable - Disable */
		if (0 != txConfig->enable)
			(void)chal_cir_tx_enable(handle);
		else
			(void)chal_cir_tx_disable(handle);

	}

	return err;
}
EXPORT_SYMBOL(chal_cir_tx_config);

/****************************************************************************/
/**
*  @brief   Configure only the RX CIR block
*
*  @param   handle      (in) CIR handle
*  @param   rxConfig    (in) RX Block Configuration
*
*  @return  < 0 if an error, >=0 otherwise
*
*  @note
*
****************************************************************************/
CHAL_CIR_ERR_t
chal_cir_rx_config(const CHAL_CIR_HANDLE_t *handle,
		   const CHAL_CIR_RX_CONFIG_t *rxConfig)
{
	CHAL_CIR_ERR_t err = CHAL_CIR_ERR_NONE;

	if ((NULL == handle) || (NULL == rxConfig)) {
		err = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		(void)chal_cir_rx_reset(handle); /* Reset the Transmitter */

		mdelay(5);	/* Wait some time for the reset */

		/* Set up Fifo Threshold */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_RX_FIFO_CTRL_REG_OFFSET),
				 (((rxConfig->fifoThreshold) <<
				   CIR_RX_FIFO_CTRL_THRESHOLD_SHIFT) &
				  CIR_RX_FIFO_CTRL_REG_RESERVED_AND_MASK));

		/* Set up Sampling Frequence */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_RX_PERIOD_REG_OFFSET),
				 ((rxConfig->samplingPeriod) &
				  CIR_RX_PERIOD_REG_RESERVED_AND_MASK));

		/* Pulse Width */
		CHAL_REG_WRITE32((handle->regBaseAddr + CIR_PW_REG_OFFSET),
				 ((((uint32_t)((rxConfig->maxPulseWidth) << 16))
				   & 0xffff0000) | (rxConfig->minPulseWidth)));

		/* Noise Rejection */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_PW_NOISE_REG_OFFSET),
				 ((rxConfig->pwNoise) &
				  CIR_PW_NOISE_REG_RESERVED_AND_MASK));

		/* Set the Receiver Input State */
		chal_cir_rx_set_ip_state(handle, rxConfig->activeState);

		/* DMA Enable - Disable */
		if (0 != rxConfig->dmaEnable)
			(void)chal_cir_rx_dma_enable(handle);
		else
			(void)chal_cir_rx_dma_disable(handle);

		/* Finally Enable - Disable */
		if (0 != rxConfig->enable)
			(void)chal_cir_rx_enable(handle);
		else
			(void)chal_cir_rx_disable(handle);

	}
	return err;
}
EXPORT_SYMBOL(chal_cir_rx_config);

/****************************************************************************/
/**
*  @brief   Initialize CIR hardware operations (both TX and RX)
*
*  @param   handle      (out) CIR handle
*  @param   config      (in)  CIR hardware configuration
*
*  @return  < 0 if an error, >=0 otherwise
*
*  @note
*
****************************************************************************/
CHAL_CIR_ERR_t
chal_cir_init(CHAL_CIR_HANDLE_t *handle, const CHAL_CIR_CONFIG_t *config)
{
	CHAL_CIR_ERR_t err = CHAL_CIR_ERR_NONE;

	if ((NULL == handle) || (NULL == config)) {
		return CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		/* Configure interrupts - Overwrite with supplied value */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_INT_ENABLE_REG_OFFSET),
				 (config->intrConfig &
				  CIR_INT_ENABLE_REG_RESERVED_AND_MASK));

		/* Configure Transmitter */
		chal_cir_tx_config(handle,
				   (const CHAL_CIR_TX_CONFIG_t *)
				   &config->txConfig);

		/* Configure Receiver */
		chal_cir_rx_config(handle,
				   (const CHAL_CIR_RX_CONFIG_t *)
				   &config->rxConfig);
	}

	return err;
}
EXPORT_SYMBOL(chal_cir_init);
CHAL_CIR_ERR_t
chal_cir_exit(CHAL_CIR_HANDLE_t *handle)
{
   CHAL_CIR_ERR_t  err = CHAL_CIR_ERR_NONE;

   if(NULL == handle)
   {
      return CHAL_CIR_ERR_INVALID_PARAM;
   }
   else
   {
      /* Disable all interrupts */
      CHAL_REG_WRITE32((handle->regBaseAddr+CIR_INT_ENABLE_REG_OFFSET), 0);
   }

   return err;
}
EXPORT_SYMBOL(chal_cir_exit);
