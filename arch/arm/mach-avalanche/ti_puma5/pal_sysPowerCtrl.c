/*
 *
 * pal_sysPowerCtrl.c 
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

/** \file   pal_sys_power_ctrl.c
    \brief  PAL power control code file
	

    \author     Ajay Singh
    \version    0.1
 */
#include <mach/generic/pal.h>

/*****************************************************************************
 * Power Control Module
 *****************************************************************************/

#define AVALANCHE_GLOBAL_POWER_DOWN_MASK    0x3FFFFFFF /* bit 31, 30 masked */
#define AVALANCHE_GLOBAL_POWER_DOWN_BIT     30         /* shift to bit 30, 31 */

void PAL_sysPowerCtrl(unsigned int module_power_bit, PAL_SYS_POWER_CTRL_T power_ctrl)
{
    volatile unsigned int *power_reg = (unsigned int*)AVALANCHE_POWER_CTRL_PDCR;

    if (power_ctrl == POWER_CTRL_POWER_DOWN)
	{
        /* power down the module */
        *power_reg |= (1 << module_power_bit);
	}
    else
	{
        /* power on the module */
        *power_reg &= (~(1 << module_power_bit));
	}
}

PAL_SYS_POWER_CTRL_T PAL_sysGetPowerStatus(unsigned int module_power_bit)
{
    volatile unsigned int *power_status_reg = (unsigned int*)AVALANCHE_POWER_CTRL_PDCR;

    return (((*power_status_reg) & (1 << module_power_bit)) ? POWER_CTRL_POWER_DOWN : POWER_CTRL_POWER_UP);
}

void PAL_sysSetGlobalPowerMode(PAL_SYS_SYSTEM_POWER_MODE_T power_mode)
{
    volatile unsigned int *power_status_reg = (unsigned int*)AVALANCHE_POWER_CTRL_PDCR;

    *power_status_reg &= AVALANCHE_GLOBAL_POWER_DOWN_MASK;
    *power_status_reg |= ( power_mode << AVALANCHE_GLOBAL_POWER_DOWN_BIT);
}

PAL_SYS_SYSTEM_POWER_MODE_T PAL_sysGetGlobalPowerMode(void)
{
    volatile unsigned int *power_status_reg = (unsigned int*)AVALANCHE_POWER_CTRL_PDCR;

    return((PAL_SYS_SYSTEM_POWER_MODE_T) (((*power_status_reg) & (~AVALANCHE_GLOBAL_POWER_DOWN_MASK)) >> AVALANCHE_GLOBAL_POWER_DOWN_BIT));
}

