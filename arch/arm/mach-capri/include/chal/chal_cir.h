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
*  @file    chal_cir.h
*
*  @brief   CIR driver interface header file.
*
*  @note
*
****************************************************************************/
#ifndef _CHAL_CIR_H
#define _CHAL_CIR_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <plat/chal/chal_types.h>

#include <chal/chal_cir_reg.h>

/* --- START: CHAL compatability shim --- */
#include <mach/rdb/brcm_rdb_cir.h>
#include <mach/io_map.h>

static inline void CHAL_REG_SETBIT32(uint32_t addr, unsigned int bits)
{
    iowrite32(ioread32((void *)addr) | bits,  (void *)addr);
}

#define CHAL_REG_WRITE32(x,y)    iowrite32(y,x)
#define CHAL_REG_READ32(x)       ioread32(x)
#define CHAL_DELAY_MS(x)         mdelay(x)

/* --- END:  CHAL compatability shim --- */

/* Bit Masks */
#define CIR_AND_MASK_4_BITS                       (uint32_t)(0x0000000F)
#define CIR_AND_MASK_3_BITS                       (uint32_t)(0x00000007)
#define CIR_AND_MASK_2_BITS                       (uint32_t)(0x00000003)

/* Register Manipulation Macros */
#define CIR_REG_SETBIT32(addr, bit, resvMask) (CHAL_REG_WRITE32(addr, ( \
			(CHAL_REG_READ32(addr) & (resvMask)) | (bit) )))
#define CIR_REG_CLRBIT32(addr, bit, resvMask) (CHAL_REG_WRITE32(addr, ( \
			(CHAL_REG_READ32(addr) & (resvMask)) & (~(bit) ) )))

/* ---- Public Constants and Types --------------------------------------- */

/* Interrupt Bit positions */
/* TODO: REMOVE THESE ONCE BIT DEFINITIONS ARE AVAILABLE */
#define CHAL_CIR_TX_DONE_INTR          (uint32_t)1
#define CHAL_CIR_TX_FIFO_INTR          (uint32_t)2
#define CHAL_CIR_TX_FIFO_UNDERRUN_INTR  (uint32_t)4
#define CHAL_CIR_RX_DONE_INTR          (uint32_t)8
#define CHAL_CIR_RX_FIFO_INTR          (uint32_t)16
#define CHAL_CIR_RX_FIFO_OVERRUN_INTR  (uint32_t)32

/** CIR Handle Data Structure
 * The data structure contains the CIR Handle
 */
typedef struct chal_cir_handle_s {
   uint32_t regBaseAddr; /**< Base address for CIR register accesses */
}CHAL_CIR_HANDLE_t;

/** CIR Transmitter Mode enumeration
 * The enumeration contains the values of the transmitter mode.
 */
typedef enum chal_cir_tx_mode_e {
    CHAL_CIR_TX_MODE_CARRIERLESS=0,
    CHAL_CIR_TX_MODE_CARRIER=1
}CHAL_CIR_TX_MODE_t;

/** CIR Error Codes
 * The enumeration contains the return error codes.
 */
typedef enum chal_cir_err_e {
    CHAL_CIR_ERR_NONE          = 0,      /**< Success */
    CHAL_CIR_ERR_INVALID_PARAM = -1024,/**< Parametres is/are NULL */
    CHAL_CIR_ERR_INVALID_DATA  = -1025,/**< Data supplied is out of range */
    CHAL_CIR_ERR_UNKNOWN       = -2048 /**< Unknown error */
}CHAL_CIR_ERR_t;

/** CIR TX Config Data Structure
 * The data structure contains the Transmitter configuration values
 */
typedef struct chal_cir_tx_config_s {
	CHAL_CIR_TX_MODE_t mode;
		   /**< 0:Carrier-less mode,1:Carrier Mode */
	uint32_t dmaEnable;
		   /**< 1:Enable, 0:Disable */
	uint32_t enable;
		   /**< 1:Enable, 0:Disable */
	uint32_t fifoThreshold;
		   /**< TX FIFO Threshold level(value range: 0-8) */
	uint32_t samplingPeriod;
		   /**< TX Sampling period (used only for Carrierless mode) */
	uint32_t tcOnTime;
		   /**< Carrier modulated off time */
	uint32_t tcOffTime;
		   /**< Carrier modulated off time */
}CHAL_CIR_TX_CONFIG_t;

/** CIR Receiver Active State enumeration
 * The enumeration contains the values of the receiver active condition
 */
typedef enum chal_cir_rx_ip_state_e {
    CHAL_CIR_RX_IP_ACTIVE_LOW=0,
    CHAL_CIR_RX_IP_ACTIVE_HIGH=1
}CHAL_CIR_RX_IP_STATE_t;

/** CIR RX Config Data Structure
 * The data structure contains the Receiver configuration values
 */
typedef struct chal_cir_rx_config_s {
	CHAL_CIR_RX_IP_STATE_t activeState;
					  /**< recever input active condition */
   uint32_t dmaEnable;     /**< 1:Enable, 0:Disable */
   uint32_t enable;        /**< 1:Enable, 0:Disable */
   uint32_t fifoThreshold; /**< RX FIFO Threshold level(value range: 0-8)  */
   uint32_t samplingPeriod;/**< RX Sampling period */
   uint32_t pwNoise;       /**< Noise rejection logic */
   uint16_t minPulseWidth; /**< Minimum Pulse width */
   uint16_t maxPulseWidth; /**< Maximum Pulse width */
}CHAL_CIR_RX_CONFIG_t;

/** CIR CONFIG Data Structure
 * The data structure contains the configuration values for the CIR Hardware
 */
typedef struct chal_cir_config_s {
   CHAL_CIR_TX_CONFIG_t txConfig;  /**< Transmitter configuration */
   CHAL_CIR_RX_CONFIG_t rxConfig;  /**< Receiver configuration */
   uint32_t             intrConfig;/**< Enable/Disable Interrupts
                                        To enable multiple interrupts
                                        use combinations like
                                        'CHAL_CIR_TX_DONE_INTR|CHAL_CIR_RX_DONE_INTR'*/
}CHAL_CIR_CONFIG_t;

/** CIR FIFO Status Data Structure
 * The data structure contains the FIFO Status values
 */
typedef struct chal_cir_fifo_status_s {
   uint32_t depth;    /**< FIFO Depth */
   uint32_t readPtr; /**< Read Pointer value */
   uint32_t writePtr;/**< Write Pointer value */
}CHAL_CIR_FIFO_STATUS_t;

/* ---- Public Variable Externs ------------------------------------------ */

/* ---- Public Function Prototypes --------------------------------------- */

/****************************************************************************/
/**
*  @brief   Initialize CIR hardware operations
*
*  @param   handle (in)  CIR handle
*  @param   config (in)  CIR hardware configuration
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note    The 'handle->regBaseAddr' needs to be initialized with the base
*           address of the CIR.
****************************************************************************/
CHAL_CIR_ERR_t chal_cir_init(CHAL_CIR_HANDLE_t *handle,
			     const CHAL_CIR_CONFIG_t *config);

/****************************************************************************/
/**
*  @brief   Shutdown CIR hardware operations
*
*  @param   handle (in)  CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
****************************************************************************/
CHAL_CIR_ERR_t chal_cir_exit(CHAL_CIR_HANDLE_t *handle);

/****************************************************************************/
/**
*  @brief   Configure only the TX CIR block
*
*  @param   handle   (in) CIR handle
*  @param   txConfig (in) TX Block Configuration
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
****************************************************************************/
CHAL_CIR_ERR_t chal_cir_tx_config(const CHAL_CIR_HANDLE_t *handle,
				  const CHAL_CIR_TX_CONFIG_t *txConfig);

/****************************************************************************/
/**
*  @brief   Configure only the RX CIR block
*
*  @param   handle   (in) CIR handle
*  @param   rxConfig (in) RX Block Configuration
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
****************************************************************************/
CHAL_CIR_ERR_t chal_cir_rx_config(const CHAL_CIR_HANDLE_t *handle,
				  const CHAL_CIR_RX_CONFIG_t *rxConfig);

/****************************************************************************/
/**
*  @brief   Enable CIR interrupt(s)
*
*  @param   handle (in) CIR handle
*  @param   intr   (in)  The interrupt index to be enabled. Can be any of the
*                        below or a combination:
*                        CHAL_CIR_TX_DONE_INTR
*                        CHAL_CIR_TX_FIFO_INTR
*                        CHAL_CIR_TX_FIFO_UNDERRUN_INTR
*                        CHAL_CIR_RX_DONE_INTR
*                        CHAL_CIR_RX_FIFO_INTR
*                        CHAL_CIR_RX_FIFO_OVERRUN_INTR
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note: Multiple interrupts can be enabled by specifying a combination of
*   the constants like 'CHAL_CIR_TX_DONE_INTR|CHAL_CIR_TX_FIFO_UNDERRUN_INTR'.
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_interrupt_enable(const CHAL_CIR_HANDLE_t
						       *handle,
						       const uint32_t intr) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_SETBIT32((handle->regBaseAddr +
				  CIR_INT_ENABLE_REG_OFFSET), intr,
				 CIR_INT_ENABLE_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Disable CIR interrupt(s)
*
*  @param   handle (in) CIR handle
*  @param   intr   (in)  The interrupt index to be disabled. Can be any of the
*                        below or a combination:
*                        CHAL_CIR_TX_DONE_INTR
*                        CHAL_CIR_TX_FIFO_INTR
*                        CHAL_CIR_TX_FIFO_UNDERRUN_INTR
*                        CHAL_CIR_RX_DONE_INTR
*                        CHAL_CIR_RX_FIFO_INTR
*                        CHAL_CIR_RX_FIFO_OVERRUN_INTR
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note: Multiple interrupts can be disabled by specifying a combination of
*   the constants like 'CHAL_CIR_TX_DONE_INTR|CHAL_CIR_TX_FIFO_UNDERRUN_INTR'.
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_interrupt_disable(const CHAL_CIR_HANDLE_t
							*handle,
							const uint32_t intr) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;
	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_CLRBIT32((handle->regBaseAddr +
				  CIR_INT_ENABLE_REG_OFFSET), intr,
				 CIR_INT_ENABLE_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Read CIR interrupt enable
*
*  @param   handle (in)  CIR handle
*  @param   buff   (out) Buffer pointer to hold the read value
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_interrupt_read(const CHAL_CIR_HANDLE_t
						     *handle,
						     uint32_t *buff) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if ((NULL == handle) || (NULL == buff)) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		*buff = ((uint32_t) (CHAL_REG_READ32(handle->regBaseAddr +
						   CIR_INT_ENABLE_REG_OFFSET)));
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Clear pending CIR interrupt(s)
*
*  @param   handle (in) CIR handle
*  @param   intr   (in)  The interrupt index to be cleared. Can be any of the
*                        below or a combination:
*                        CHAL_CIR_TX_DONE_INTR
*                        CHAL_CIR_TX_FIFO_INTR
*                        CHAL_CIR_TX_FIFO_UNDERRUN_INTR
*                        CHAL_CIR_RX_DONE_INTR
*                        CHAL_CIR_RX_FIFO_INTR
*                        CHAL_CIR_RX_FIFO_OVERRUN_INTR
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note: Multiple interrupts can be cleared by specifying a combination of
*   the constants like 'CHAL_CIR_TX_DONE_INTR|CHAL_CIR_TX_FIFO_UNDERRUN_INTR'.
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_interrupt_pending_clear(const
							      CHAL_CIR_HANDLE_t
							      *handle,
							      const uint32_t
							      intr) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_SETBIT32((handle->regBaseAddr +
				  CIR_INT_PEND_REG_OFFSET), intr,
				 CIR_INT_PEND_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Read pending CIR interrupt status
*
*  @param   handle (in)  CIR handle
*  @param   buff   (out) Buffer pointer to hold the read value (Pending Interrupt)
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_interrupt_pending_read(const
							     CHAL_CIR_HANDLE_t
							     *handle,
							     uint32_t *buff) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if ((NULL == handle) || (NULL == buff)) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		*buff =
		    (uint32_t)(CHAL_REG_READ32
			       (handle->regBaseAddr + CIR_INT_PEND_REG_OFFSET));
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Enable CIR TX block
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
	static inline CHAL_CIR_ERR_t chal_cir_tx_enable(const CHAL_CIR_HANDLE_t
							*handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

		if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
		} else {
			CIR_REG_SETBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_TX_ENABLE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
      CHAL_DELAY_MS(2);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Disable CIR TX block
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
	static inline CHAL_CIR_ERR_t chal_cir_tx_disable(const CHAL_CIR_HANDLE_t
							 *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

		if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
		} else {
			CIR_REG_CLRBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_TX_ENABLE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Enable CIR RX block
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
	static inline CHAL_CIR_ERR_t chal_cir_rx_enable(const CHAL_CIR_HANDLE_t
							*handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

		if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
		} else {
			CIR_REG_SETBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_RX_ENABLE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
      //CHAL_DELAY_MS(2);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Set the Receiver Input state
*
*  @param   handle    (in) CIR handle
*  @param   rxIpState (in) CIR RX Input State
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_rx_set_ip_state(const CHAL_CIR_HANDLE_t
						      *handle,
						      const
						      CHAL_CIR_RX_IP_STATE_t
						      rxIpState) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		if (CHAL_CIR_RX_IP_ACTIVE_HIGH == rxIpState) {
			CIR_REG_SETBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_RX_IP_STATE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
		} else {
			CIR_REG_CLRBIT32((handle->regBaseAddr +
					  CIR_CTRL_REG_OFFSET),
					 CIR_CTRL_REG_RX_IP_STATE_BIT,
					 CIR_CTRL_REG_RESERVED_AND_MASK);
      }
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Disable CIR RX block
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_rx_disable(const CHAL_CIR_HANDLE_t
						 *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_CLRBIT32((handle->regBaseAddr +
				  CIR_CTRL_REG_OFFSET),
				 CIR_CTRL_REG_RX_ENABLE_BIT,
				 CIR_CTRL_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Reset CIR TX block
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_tx_reset(const CHAL_CIR_HANDLE_t
					       *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_SETBIT32((handle->regBaseAddr + CIR_CTRL_REG_OFFSET),
				 CIR_CTRL_REG_TX_RESET_BIT,
				 CIR_CTRL_REG_RESERVED_AND_MASK);
      CHAL_DELAY_MS (4);   /* 4ms*/
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Reset CIR RX block
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_rx_reset(const CHAL_CIR_HANDLE_t
					       *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_SETBIT32((handle->regBaseAddr + CIR_CTRL_REG_OFFSET),
				 CIR_CTRL_REG_RX_RESET_BIT,
				 CIR_CTRL_REG_RESERVED_AND_MASK);
      CHAL_DELAY_MS (4);   /* 4ms*/
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Enable CIR TX DMA
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_tx_dma_enable(const CHAL_CIR_HANDLE_t
						    *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CHAL_REG_SETBIT32((handle->regBaseAddr + CIR_CTRL_REG_OFFSET),
				  CIR_CTRL_REG_TX_DMA_ENABLE_BIT);
      CHAL_DELAY_MS(2);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Disable CIR TX DMA
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_tx_dma_disable(const CHAL_CIR_HANDLE_t
						     *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_CLRBIT32((handle->regBaseAddr + CIR_CTRL_REG_OFFSET),
				 CIR_CTRL_REG_TX_DMA_ENABLE_BIT,
				 CIR_CTRL_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Enable CIR RX DMA
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_rx_dma_enable(const CHAL_CIR_HANDLE_t
						    *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CHAL_REG_SETBIT32((handle->regBaseAddr + CIR_CTRL_REG_OFFSET),
				  CIR_CTRL_REG_RX_DMA_ENABLE_BIT);
      CHAL_DELAY_MS(2);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Disable CIR RX DMA
*
*  @param   handle (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note:
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_rx_dma_disable(const CHAL_CIR_HANDLE_t
						     *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CIR_REG_CLRBIT32((handle->regBaseAddr + CIR_CTRL_REG_OFFSET),
				 CIR_CTRL_REG_RX_DMA_ENABLE_BIT,
				 CIR_CTRL_REG_RESERVED_AND_MASK);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Get the TX FIFO Status
*
*  @param   handle      (in) CIR handle
*  @param   fifoStatus  (out) Status variable to hold the TX FIFO status
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_tx_fifo_status_get(const CHAL_CIR_HANDLE_t
							 *handle,
							 CHAL_CIR_FIFO_STATUS_t
							 *fifoStatus) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;
   uint32_t       regVal=0;

	if ((NULL == handle) || (NULL == fifoStatus)) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		regVal =
		    (uint32_t)CHAL_REG_READ32(handle->regBaseAddr +
					      CIR_TX_FIFO_STATUS_REG_OFFSET);
		fifoStatus->depth =
		    (((uint32_t)(regVal >> CIR_TX_FIFO_STATUS_REG_DEPTH_OFFSET))
		     & (CIR_AND_MASK_4_BITS));
		fifoStatus->readPtr =
		   (((uint32_t)(regVal >> CIR_TX_FIFO_STATUS_REG_RD_PTR_OFFSET))
		     & (CIR_AND_MASK_3_BITS));
		fifoStatus->writePtr =
		   (((uint32_t)(regVal >> CIR_TX_FIFO_STATUS_REG_WR_PTR_OFFSET))
		     & (CIR_AND_MASK_3_BITS));
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Get the RX FIFO Status
*
*  @param   handle      (in) CIR handle
*  @param   fifoStatus  (out) Status variable to hold the TX FIFO status
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
*
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_rx_fifo_status_get(const CHAL_CIR_HANDLE_t
							 *handle,
							 CHAL_CIR_FIFO_STATUS_t
							 *fifoStatus) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;
   uint32_t       regVal=0;

	if ((NULL == handle) || (NULL == fifoStatus)) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		regVal = (uint32_t)CHAL_REG_READ32(handle->regBaseAddr +
						 CIR_RX_FIFO_STATUS_REG_OFFSET);
		fifoStatus->depth =
		    (((uint32_t)(regVal >> CIR_RX_FIFO_STATUS_REG_DEPTH_OFFSET))
		     & (CIR_AND_MASK_4_BITS));
		fifoStatus->readPtr =
		   (((uint32_t)(regVal >> CIR_RX_FIFO_STATUS_REG_RD_PTR_OFFSET))
		     & (CIR_AND_MASK_3_BITS));
		fifoStatus->writePtr =
		   (((uint32_t)(regVal >> CIR_RX_FIFO_STATUS_REG_WR_PTR_OFFSET))
		     & (CIR_AND_MASK_3_BITS));
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Write TX data into the FIFO
*
*  @param   handle  (in) CIR handle
*  @param   txData  (in) Data to be transmitted
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
*
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_data_write(const CHAL_CIR_HANDLE_t
						 *handle,
						 uint32_t txData) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CHAL_REG_WRITE32((handle->regBaseAddr + CIR_TX_DATA_REG_OFFSET),
				 txData);
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Read data from the RX FIFO
*
*  @param   handle  (in) CIR handle
*  @param   buff   (out) Buffer pointer to store the read value
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
*
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_data_read(const CHAL_CIR_HANDLE_t *handle,
						uint32_t *buff) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if ((NULL == handle) || (NULL == buff)) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		*buff = (CHAL_REG_READ32((handle->regBaseAddr) +
					 CIR_RX_DATA_REG_OFFSET));
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Configure Pulsewidth Noise Rejection
*
*  @param   handle  (in) CIR handle
*  @param   nrValue  (in) CIR handle
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
*
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_config_pw_noise_rejection(const
								CHAL_CIR_HANDLE_t
								*handle,
								const uint32_t
								nrValue)
{
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_PW_NOISE_REG_OFFSET),
				 (nrValue &
				  CIR_PW_NOISE_REG_RESERVED_AND_MASK));
   }
   return errCode;
}

/****************************************************************************/
/**
*  @brief   Configure Pulsewidth Minimum
*
*  @param   handle  (in) CIR handle
*  @param   pwValue  (in) puslse width minimum value
*
*  @return  CHAL_CIR_ERR_NONE if success, <0 otherwise
*
*  @note
*
****************************************************************************/
static inline CHAL_CIR_ERR_t chal_cir_config_pw_min(const CHAL_CIR_HANDLE_t
						    *handle,
						    const uint32_t pwValue) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
		CHAL_REG_WRITE32((handle->regBaseAddr + CIR_PW_REG_OFFSET),
				 (pwValue));
   }
   return errCode;
}

static inline CHAL_CIR_ERR_t chal_cir_txdma_count_set(const CHAL_CIR_HANDLE_t
						      *handle,
						      const uint32_t count) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
      /* Write DMA count */
		CHAL_REG_WRITE32((handle->regBaseAddr +
				  CIR_TX_DMA_COUNT_REG_OFFSET),
				 (count &
				  CIR_TX_DMA_COUNT_REG_RESERVED_AND_MASK));

      /* Cycle the DMA enable bit to load the count */
      (void)chal_cir_tx_dma_disable(handle);
      (void)chal_cir_tx_dma_enable(handle);
   }
   return errCode;
}

static inline CHAL_CIR_ERR_t chal_cir_tx_block_enable(const CHAL_CIR_HANDLE_t
						      *handle,
						      const uint32_t dmaCount) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
      /* Reset tx */
      (void)chal_cir_tx_reset(handle);
      (void)chal_cir_tx_dma_disable(handle);
      /* Enable tx */
      (void)chal_cir_tx_enable(handle);
      /* Load count and enable tx dma */
      (void)chal_cir_txdma_count_set(handle, dmaCount);
   }
   return errCode;
}

static inline CHAL_CIR_ERR_t chal_cir_rx_block_enable(const CHAL_CIR_HANDLE_t
						      *handle) {
   CHAL_CIR_ERR_t errCode = CHAL_CIR_ERR_NONE;

	if (NULL == handle) {
      errCode = CHAL_CIR_ERR_INVALID_PARAM;
	} else {
      /* Reset rx */
      (void)chal_cir_rx_reset(handle);
      /* Disable rx dma */
      (void)chal_cir_rx_dma_disable(handle);
      /* Enable rx dma */
      (void)chal_cir_rx_dma_enable(handle);
      /* Enable rx */
      (void)chal_cir_rx_enable(handle);
   }
   return errCode;
}

/*
 * For debugging
 */
static inline CHAL_CIR_ERR_t chal_tx_read_all_fifo_data(const CHAL_CIR_HANDLE_t
							*handle,
							uint32_t *buff)
{
	buff[0] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000100));
	buff[1] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000104));
	buff[2] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000108));
	buff[3] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x0000010c));
	buff[4] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000110));
	buff[5] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000114));
	buff[6] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000118));
	buff[7] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x0000011c));

	buff[8] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000008)); /* TX FIFO Status */
	buff[9] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000018)); /* TX DMA Count */
	buff[10] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000000)); /* CTRL */

   return CHAL_CIR_ERR_NONE;
}
static inline CHAL_CIR_ERR_t chal_rx_read_all_fifo_data(const CHAL_CIR_HANDLE_t
							*handle,
							uint32_t *buff)
{
	buff[0] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000140));
	buff[1] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000144));
	buff[2] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000148));
	buff[3] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x0000014c));
	buff[4] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000150));
	buff[5] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000154));
	buff[6] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000158));
	buff[7] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x0000015c));
	buff[8] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000024)); /* RX FIFO Status */
	buff[10] = (uint32_t)(CHAL_REG_READ32((handle->regBaseAddr) +
					     0x00000000)); /* CTRL */

   return CHAL_CIR_ERR_NONE;
}

#ifdef __cplusplus
}
#endif

#endif /* _CHAL_CIR_H */
