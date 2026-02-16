/*
 * ssp_hal.h
 * Description:
 * See below.
 *
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2008-2014 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**************************************************************************
 * FILE PURPOSE :   Header file for SSP HAL APIs and Data Structures.
 **************************************************************************
 * FILE NAME    :   ssp_hal.h
 *
 * DESCRIPTION  :
 *  This file declares the data structures and APIs exposed by the SSP HAL
 *  to the driver. SSP HAL interface is OS and platform agnostic.
 *
 * REVISION HISTORY:
 * 16 Jan 03 - PSP TII
 *    May 05 - PSP IL
 *
 *************************************************************************/

#ifndef __SSP_HAL__
#define __SSP_HAL__

//#include <ssp_config.h>

/**
 * \defgroup SspHal SSP Hardware Abstraction Layer
 * 
 * SSP Hardware Abstraction Layer
 */
/*@{*/

/**
 * \defgroup SspHalReturnCodes SSP HAL Return Codes
 * 
 * SSP HAL Return Codes
 */
/*@{*/

/** \def SSP_HAL_OK SSP HAL API Success */
#define SSP_HAL_OK                      0

/** \def SSP_HAL_ERROR SSP HAL API Failure */
#define SSP_HAL_ERROR                  -1

/*@}*/

/**
 * \defgroup SspHalDevOpMode SSP HAL Device Operating modes
 * 
 * SSP HAL Device Operating modes
 */
/*@{*/

/** \def SSP_HAL_MODE_POLL SSP Device in Polled mode */
#define SSP_HAL_MODE_POLL               0

/** \def SSP_HAL_MODE_INTR SSP Device in Interrupt mode */
#define SSP_HAL_MODE_INTR               1

/*@}*/

/**
 * \defgroup SspHalIntcCtrl SSP HAL Interrupt control actions
 * 
 * SSP HAL Interrupt control actions
 */
/*@{*/

/** \def SSP_HAL_INTR_ACK Acknowledge the current interrupt */
#define SSP_HAL_INTR_ACK                0

/** \def SSP_HAL_INTR_DISABLE Disable Interrupts at device level */
#define SSP_HAL_INTR_DISABLE            1

/** \def SSP_HAL_INTR_ENABLE Enable Interrupts at device level */
#define SSP_HAL_INTR_ENABLE             2

/*@}*/

/**
 *  \brief Information to HAL about I2C Slave
 *
 *  Passed to the HAL layer during the open call of SSP HAL I2C interface.
 *  An updated copy is passed back to the caller by the HAL. The bus_speed
 *  parameter is updated to reflect the exact bus speed obtained on the I2C
 *  bus
 *
 *  \note The driver should update all the fields before passing it to HAL 
 */
typedef struct SSP_HAL_I2C_INFO_tag
{
    UINT32   data_pin;  /**< The pin num of SSP to which I2C data connects */
    UINT32   clock_pin; /**< The pin num of SSP to which I2C clock connects */
    UINT8	 addr;      /**< The I2C Slave address */
    UINT32	 bus_speed; /**< The I2C Bus speed (typically 100 or 400 KHz) */
    UINT32   mode;      /**< The device operating mode. Interrupt or Polled. */
    
}SSP_HAL_I2C_INFO_T;

typedef struct SSP_HAL_LCD_CTRL_INFO_tag
{
    UINT32   data_in_pin;  
    UINT32   data_out_pin; 
    UINT32   clock_pin;    /**< The pin of SSP to which SPI clock connects */
    UINT32   cs_pin;      
    UINT32	 clock_speed;
    UINT32   mode;      /**< The device operating mode. Interrupt or Polled. */   
    
}SSP_HAL_LCD_CTRL_INFO_T;

/**
 *  \brief Information to HAL about SPI Slave
 *
 *  Passed to the HAL layer during the open call of SSP HAL SPI interface.
 *  An updated copy is passed back to the caller by the HAL. The bus_speed
 *  parameter is updated to reflect the exact bus speed obtained on the SPI
 *  bus
 *
 *  \note The driver should update all the fields before passing it to HAL 
 */
typedef struct SSP_HAL_SPI_INFO_tag
{
    UINT32  data_in_pin;  /**< The pin of SSP to which SPI data in connects */
    UINT32  data_out_pin; /**< The pin of SSP to which SPI data out connects */
    UINT32  clock_pin;    /**< The pin of SSP to which SPI clock connects */
    UINT32  cs_pin;       /**< The pin of SSP to which SPI chip sel connects */
    UINT32	clock_speed;  /**< The SPI clock speed */
    UINT32  mode;         /**< The device operating mode. Intr or Polled. */
    
}SSP_HAL_SPI_INFO_T;

/**
 * \defgroup SspHalApi SSP HAL API
 * 
 * SSP HAL API
 */
/*@{*/


/**
 *  \brief SSP interrupt service routine 
 *
 *  This function is the ISR for the SSP driver to be called from the ddc layer
 *  
 *  \param  base_address [IN]       None
 *  
 *  \return  NONE 
 */
void sspIsr (void);

/**
 *  \brief SSP HAL Initialization function 
 *
 *  This function initializes the SSP hardware and HAL specific data 
 *  structures
 *  
 *  \param  base_address [IN]       The base address of the SSP module
 *  \param  module_input_freq [IN]  The frequency of input bus (in Hz)
 *  
 *  \return SSP_HAL_OK or SSP_HAL_ERROR 
 */
INT32 ssp_hal_init
(
    UINT32 base_address,
    UINT32 module_input_freq
);

/**
 *  \brief SSP HAL I2C open function
 *
 *  This function is called to set up SSP for communication I2C slave
 *  
 *  \param  info [IN]   The SSP_HAL_I2C_INFO_T structure
 *  
 *  \return Pointer to valid (updated) SSP_HAL_I2C_INFO_T structure or NULL
 */
SSP_HAL_I2C_INFO_T* ssp_hal_i2c_open
(
    SSP_HAL_I2C_INFO_T info
);

/**
 *  \brief SSP HAL I2C close function. 
 *
 *  This function is called to close an opened I2C slave device.
 *  
 *  \param  info [IN]   Pointer to SSP_HAL_I2C_INFO_T structure obtained 
 *                      through an earlier successful open call
 *                      
 *  \return SSP_HAL_OK
 */
INT32 ssp_hal_i2c_close
(
    SSP_HAL_I2C_INFO_T* info
);

/**
 *  \brief SSP HAL I2C write function. 
 *
 *  This function writes the specified number of bytes after putting the 
 *  start signal and the device address on the bus. After writing the 
 *  specified number of characters, a stop signal is issued.
 *   
 *  \param  info [IN]   Pointer to SSP_HAL_I2C_INFO_T structure obtained 
 *                      through an earlier successful open call
 *  \param  buffer [IN] Pointer to a buffer of (8 bit) characters to write.
 *  \param  len [IN]    Number of characters to write. 
 *  
 *  \return Number of characters written or SSP_HAL_ERROR on error.
 */
INT32 ssp_hal_i2c_write 
(
    SSP_HAL_I2C_INFO_T  *info,
    UINT8               *buffer, 
    UINT32              len
);

/**
 *  \brief SSP HAL I2C read function. 
 *
 *  This function reads the specified number of bytes after putting the 
 *  start signal and the device address on the bus. After reading the 
 *  specified number of characters, a stop signal is issued.
 *   
 *  \param info [IN]    Pointer to SSP_HAL_I2C_INFO_T structure obtained 
 *                      through an earlier successful open call
 *  \param buffer [OUT] Pointer to a buffer of (8 bit) characters to store the
 *                      read data.
 *  \param  len [IN]    Number of characters to read. 
 *  
 *  \return Number of characters read or SSP_HAL_ERROR on error.
 */
INT32 ssp_hal_i2c_read
(
    SSP_HAL_I2C_INFO_T  *info,
    UINT8               *buffer,
    UINT32              len
);

/**
 *  \brief SSP HAL SPI open function. 
 *
 *  This function is called to set up SSP for communication SPI slave
 *  
 *  \param  info [IN]   The SSP_HAL_SPI_INFO_T structure
 *  
 *  \return Pointer to valid (updated) SSP_HAL_SPI_INFO_T structure or NULL
 */
SSP_HAL_SPI_INFO_T* ssp_hal_spi_open
(
    SSP_HAL_SPI_INFO_T info
);

/**
 *  \brief SSP HAL SPI close function. 
 *
 *  This function is called to close an opened SPI slave device.
 *  
 *  \param  info [IN]   Pointer to SSP_HAL_SPI_INFO_T structure obtained 
 *                      through an earlier successful open call
 *                      
 *  \return SSP_HAL_OK
 */
INT32 ssp_hal_spi_close
(
    SSP_HAL_SPI_INFO_T* info
);

/**
 *  \brief SSP HAL SPI write read function. 
 *
 *  This function writes the specified number of bytes after putting the 
 *  chip select signal to low. Then keeping the chip select signal low, it
 *  reads the specified number of bytes. After the completion of the reading
 *  it makes the chip select signal high again
 *   
 *  \param info [IN]         Pointer to SSP_HAL_SPI_INFO_T structure obtained 
 *                           through an earlier successful open call
 *  \param write_buf [IN] Pointer to a buffer of (8 bit) characters 
 *                           to be written
 *  \param write_len [IN]    Number of characters to write.
 *  \param read_buf [OUT] Pointer to a buffer of (8 bit) characters 
 *                           to be read into
 *  \param read_len [IN]     Number of characters to be read
 *  
 *  \return Total number of characters read and written or SSP_HAL_ERROR
 *          on error.
 */
INT32 ssp_hal_spi_write_read 
(
    SSP_HAL_SPI_INFO_T  *info,
    UINT8               *write_buf, 
    UINT32              write_len,
    UINT8               *read_buf, 
    UINT32              read_len
);

/**
 *  \brief SSP HAL Interrupt control. 
 *
 *  This function enables the driver to control SSP interrupts at the device
 *  level. 
 *   
 *  \param action [IN]  Action to take on device interrupt. The supported
 *                      actions are: Enable, Disable and Acknowledge 
 *                        
 *  \return SSP_HAL_OK on success or SSP_HAL_ERROR on error.
 */
INT32 ssp_hal_intr_ctrl
(
    UINT32 action
);

/**
 *  \brief SSP HAL Transfer complete wait callback 
 *
 *  The HAL issues this callback to the driver in interrupt mode to
 *  enable it to wait for transfer to complete.
 *  level. 
 */
void ssp_drv_cbk_wait_for_xfr_done( void );

/*******************************************************************************
 * FUNCTION NAME : ssp_hal_lcd_open: SSP HAL open function
 *******************************************************************************
 * DESCRIPTION   : The SSP HAL LCD contrast control pulse function.
 *
 * RETURNS: updated INFO structure in case of success, NULL otherwise.
 *
 ******************************************************************************/
SSP_HAL_LCD_CTRL_INFO_T* ssp_hal_lcd_ctrl_open
(
    SSP_HAL_LCD_CTRL_INFO_T    info
);

/*******************************************************************************
 * FUNCTION NAME : ssp_hal_lcd_send: SSP HAL control pulse function
 *******************************************************************************
 * DESCRIPTION   : The SSP HAL LCD contrast control pulse function.
 *
 * RETURNS:      SSP_HAL_OK on success, SSP_HAL_ERROR on  
 *
 ******************************************************************************/
INT32 ssp_hal_lcd_ctrl_send
(
    SSP_HAL_LCD_CTRL_INFO_T    *info,
    BOOL up
);

/*******************************************************************************
 * FUNCTION ssp_hal_lcd_ctrl_close: Close the opened LCD device.
 *******************************************************************************
 * DESCRIPTION: Close the opened LCD device.
 *
 * RETURNS: returns SSP_HAL_OK on success and SSP_HAL_ERROR on failure. 
 ******************************************************************************/
INT32 ssp_hal_lcd_ctrl_close
(
    SSP_HAL_LCD_CTRL_INFO_T* info
);

/*@}*/

/*@}*/

#endif /* ifndef  __SSP_HAL__*/

