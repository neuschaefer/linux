/*
 * ssp_config.h
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

/*******************************************************************************
 * FILE PURPOSE:    SSP Module Device Driver Configuration File.
 *******************************************************************************
 * FILE NAME:       ssp_config.h
 *
 * DESCRIPTION:     Contains the configuration information for the devices 
 *                  connected to the SSP module. 
 *
 * REVISION HISTORY:
 * 19 Mar 03 - Creation - PSP TII  
 *
 ******************************************************************************/

#ifndef __SSP_CONFIG_H__
#define __SSP_CONFIG_H__

#define SSP_I2C_OUTPUT_CLK_FREQ                 400000    /* 400 KHz */ 
#define SSP_SPI_OUTPUT_CLK_FREQ                 2000000   /* 2 MHz */
#define SSP_UW_OUTPUT_CLK_FREQ                  1000000   /* 1 MHz */
#define SSP_LCDCTRL_OUTPUT_CLK_FREQ             250000    /* 250 KHz */ 


/*
 * The SSP driver draws the configuration it needs to pass 
 * to the HAL from these structures. For I2C devices, The 
 * address parameter defined in this structure is ignored
 * as the information on address passed by the application.
 * Similarly the chip select number is ignored for SPI devices.
 */

#if defined( TNETV1050SDB ) || defined( CONFIG_MIPS_TITAN )
SSP_HAL_I2C_INFO_T ssp_i2c_static_config = {
    1, 0, 0x00, SSP_I2C_OUTPUT_CLK_FREQ, SSP_HAL_MODE_INTR        
};

SSP_HAL_LCD_CTRL_INFO_T ssp_lcd_ctrl_static_config = { 
    0, 0, 0, 4, SSP_LCDCTRL_OUTPUT_CLK_FREQ, SSP_HAL_MODE_INTR 
};

/* TNETV1050SDB does NOT contain SPI device. This is added here for the sake 
 * of avoid  the cluttering of driver code with ifdef statements 
 */
SSP_HAL_SPI_INFO_T ssp_spi_static_config = { 
    0, 0, 1, 2, SSP_SPI_OUTPUT_CLK_FREQ, SSP_HAL_MODE_INTR 
};
#endif

#ifdef TNETV1050VDB
SSP_HAL_I2C_INFO_T ssp_i2c_static_config = {
    0, 1, 0x00, SSP_I2C_OUTPUT_CLK_FREQ, SSP_HAL_MODE_INTR        
};

SSP_HAL_LCD_CTRL_INFO_T ssp_lcd_ctrl_static_config = { 
    0, 0, 0, 4, SSP_LCDCTRL_OUTPUT_CLK_FREQ, SSP_HAL_MODE_INTR 
};

SSP_HAL_SPI_INFO_T ssp_spi_static_config = {
    0, 0, 1, 2, SSP_SPI_OUTPUT_CLK_FREQ, SSP_HAL_MODE_INTR 
};
#endif

#endif /* __SSP_CONFIG_H__ */



