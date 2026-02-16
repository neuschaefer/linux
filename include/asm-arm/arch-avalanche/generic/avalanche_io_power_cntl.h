/*
 *
 * avalanche_io_power_cntl.h 
 * Description:
 * io power control header file
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
/** \file   avalanche_io_power_cntl.h
    \brief  avalanche IO power control header file
	
    \author     Mansoor Ahamed (mansoor.ahamed@ti.com)
    \version    0.1
 */


#ifndef __AVALANCHE_IO_POWER_CNTL_H__
#define __AVALANCHE_IO_POWER_CNTL_H__
/** \enum AVALANCHE_IO_POWER_MODULE_T
	\brief Enum for modules which can do IO power management using IO PDCR
	\TODO This enum should be moved to SOC specific header file
*/
typedef enum AVALANCHE_IO_POWER_MODULE_tag 
{
	IOPM_UART = 0,
	IOPM_TDM_CODEC,
	IOPM_GPIO,
	IOPM_I2C,
	IOPM_GMII,
	IOPM_OOB,
	IOPM_EPGA,
	IOPM_BBU,
	IOPM_ASYNC_EMIF,
	IOPM_AGC,
	IOPM_TAGC,
	IOPM_EXT_INT,
	IOPM_CLKOUT0,
	IOPM_CLKOUT1,
	IOPM_VLYNQ,
	AVALANCHE_MAX_IOPM_MODULES
}AVALANCHE_IO_POWER_MODULE_T;

/** \enum AVALANCHE_IO_POWER_MODULE_T
	\brief Enum for modules which can do IO power management using IO PDCR
*/
typedef enum AVALANCHE_IO_POWER_MODE_tag 
{
	AVALANCHE_IO_POWER_DOWN = 0,
	AVALANCHE_IO_POWER_UP
}AVALANCHE_IO_POWER_MODE_T;


/*! \fn int avalanche_setIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id, AVALANCHE_IO_POWER_MODE_T mode)
    \brief This API ise used to power down or power up a specific module.
    \param io_module_id Unique module id which has to be operated on.
    \param mode The new mode (AVALANCHE_IO_POWER_DOWN or AVALANCHE_IO_POWER_UP).
	\return Returns 0 on success else returns a negative value
*/
int avalanche_setIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id, AVALANCHE_IO_POWER_MODE_T mode);

/*! \fn AVALANCHE_IO_POWER_MODE_T avalanche_getIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id)
    \brief This API ise used to retrieve current mode (power down or up) of a module.
    \param io_module_id Unique module id which has to be operated on.
    \return mode Modules power mode (AVALANCHE_IO_POWER_DOWN or AVALANCHE_IO_POWER_UP).
*/
AVALANCHE_IO_POWER_MODE_T avalanche_getIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id);

#endif /* __AVALANCHE_IO_POWER_CNTL_H__ */
