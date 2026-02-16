/*
 *
 * avalanche_io_power_cntl.c 
 * Description:
 * APIs for io power control
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

/** \file   avalanche_io_power_cntl.c
    \brief  This file provides avalanche APIs for IO power control
	
	This module controls the power for the I/O layer of the specified module.
 
    \author     Mansoor Ahamed (mansoor.ahamed@ti.com)
    \version    0.1
 */

#include <asm-arm/arch-avalanche/generic/pal.h>

/*! \fn int avalanche_setIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id, AVALANCHE_IO_POWER_MODE_T mode)
    \brief This API ise used to power down or power up a specific module.
    \param io_module_id Unique module id which has to be operated on.
    \param mode The new mode (AVALANCHE_IO_POWER_DOWN or AVALANCHE_IO_POWER_UP).
	\return Returns 0 on success else returns a negative value
*/
int avalanche_setIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id, AVALANCHE_IO_POWER_MODE_T mode)
{
	volatile unsigned int *io_pdcr = (unsigned int*)AVALANCHE_IO_POWER_CTRL_PDCR_BASE;
	
	if(io_module_id >= AVALANCHE_MAX_IOPM_MODULES)
		return -1;

	/* clear the bit first */
	*io_pdcr &= (~(1 << io_module_id));

	if(mode == AVALANCHE_IO_POWER_DOWN)
		*io_pdcr |= (1 << io_module_id);

    return 0;            
}

/*! \fn AVALANCHE_IO_POWER_MODE_T avalanche_getIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id)
    \brief This API ise used to retrieve current mode (power down or up) of a module.
    \param io_module_id Unique module id which has to be operated on.
    \return mode Modules power mode (AVALANCHE_IO_POWER_DOWN or AVALANCHE_IO_POWER_UP).
*/
AVALANCHE_IO_POWER_MODE_T avalanche_getIOPowerMode(AVALANCHE_IO_POWER_MODULE_T io_module_id)
{
	volatile unsigned int *io_pdcr = (unsigned int*)AVALANCHE_IO_POWER_CTRL_PDCR_BASE;
	
	if(io_module_id >= AVALANCHE_MAX_IOPM_MODULES)
		return -1;

	return (((*io_pdcr) & (1 << io_module_id)) ? AVALANCHE_IO_POWER_DOWN : AVALANCHE_IO_POWER_UP);

    return 0;            
}


