/*
 *
 * pals_sysResetCtrl.c
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
/** \file   pal_sysResetCtrl.c
    \brief  PAL reset control code file
	

    \author     Ajay Singh
    \version    0.1
 */
#include <asm/arch/generic/pal.h>

REMOTE_VLYNQ_DEV_RESET_CTRL_FN p_remote_vlynq_dev_reset_ctrl = NULL;

/*****************************************************************************
 * Reset Control Module.
 *****************************************************************************/
void PAL_sysResetCtrl(unsigned int module_reset_bit, 
                        PAL_SYS_RESET_CTRL_T reset_ctrl)
{
    volatile unsigned int *reset_reg = (unsigned int*) AVALANCHE_RST_CTRL_PRCR;

    if(module_reset_bit >= 32 && module_reset_bit < 64)
	{
        return;
	}

    if(module_reset_bit >= 64)
    {
        if(p_remote_vlynq_dev_reset_ctrl)
		{
            return(p_remote_vlynq_dev_reset_ctrl(module_reset_bit - 64, reset_ctrl));
		}
		else
		{
            return;
		}
    }
    
    if(reset_ctrl == OUT_OF_RESET)
	{
        *reset_reg |= 1 << module_reset_bit;
	}
    else
	{
        *reset_reg &= ~(1 << module_reset_bit);
	}
}

PAL_SYS_RESET_CTRL_T PAL_sysGetResetStatus(unsigned int module_reset_bit)
{
    volatile unsigned int *reset_reg = (unsigned int*) AVALANCHE_RST_CTRL_PRCR;

    return (((*reset_reg) & (1 << module_reset_bit)) ? OUT_OF_RESET : IN_RESET );
}

void PAL_sysSystemReset(PAL_SYS_SYSTEM_RST_MODE_T mode)
{
    volatile unsigned int *sw_reset_reg = (unsigned int*) AVALANCHE_RST_CTRL_SWRCR;
    *sw_reset_reg =  mode;
}

#define AVALANCHE_RST_CTRL_RSR_MASK 0x3

PAL_SYS_SYSTEM_RESET_STATUS_T PAL_sysGetSysLastResetStatus()
{
    volatile unsigned int *sys_reset_status = (unsigned int*) AVALANCHE_RST_CTRL_RSR;

    return ( (PAL_SYS_SYSTEM_RESET_STATUS_T) (*sys_reset_status & AVALANCHE_RST_CTRL_RSR_MASK) );
}

