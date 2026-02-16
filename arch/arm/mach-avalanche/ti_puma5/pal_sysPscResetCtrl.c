/*
 *
 * pal_sysPscResetCtrl.c 
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
/** \file   pal_sysPscResetCtrl.c
 *  \brief  PAL reset control APIs
 *  
 *  \author     PSP, TI
 *
 *  \note       Set tabstop to 4 (:se ts=4) while viewing this file in an
 *              editor
 *
 *  \version    0.1     Ajay Sing   		Created
 *              0.2     Mansoor Ahamed		For Avalanche3 architecture
 */

#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm-arm/arch-avalanche/generic/pal_sysPsc.h>

REMOTE_VLYNQ_DEV_RESET_CTRL_FN p_remote_vlynq_dev_reset_ctrl = NULL;
extern void avalanche_system_reset(PAL_SYS_SYSTEM_RST_MODE_T mode);
/*****************************************************************************
 * Reset Control Module.
 *****************************************************************************/
/*! \fn void PAL_sysResetCtrl(unsigned int module_reset_bit, PAL_SYS_RESET_CTRL_T reset_ctrl)
    \brief This API is used to assert or de-assert reset for a module
    \param module_reset_bit Unique module id to assert/de-assert reset
    \param reset_ctrl assert/de-assert reset (IN_RESET, OUT_OF_RESET)
*/
void PAL_sysResetCtrl(unsigned int module_reset_bit, PAL_SYS_RESET_CTRL_T reset_ctrl)
{
	PAL_SYS_PSC_MODULE_T module_id = (PAL_SYS_PSC_MODULE_T)(module_reset_bit);

/*HPVL*/
#if 0
    if(module_id == PSC_VLYNQ)
    {
        if(p_remote_vlynq_dev_reset_ctrl)
		{
            p_remote_vlynq_dev_reset_ctrl(module_reset_bit, reset_ctrl);
		}
		else
		{
            return;
		}
    }
#endif

    if(reset_ctrl == OUT_OF_RESET)
	{
		PAL_sysPscSetModuleState(module_id, PSC_ENABLE);
	}
    else
	{
		PAL_sysPscSetModuleState(module_id, PSC_SW_RST_DISABLE);
	}

}

/*! \fn PAL_SYS_RESET_CTRL_T PAL_sysGetResetStatus(unsigned int module_reset_bit)
    \brief This API returns the status reset status of a module
    \param module_reset_bit Unique module id whose reset status has to be read
    \return Reset assert/de-assert (IN_RESET, OUT_OF_RESET)
*/
PAL_SYS_RESET_CTRL_T PAL_sysGetResetStatus(unsigned int module_reset_bit)
{
	PAL_SYS_PSC_MODULE_T module_id = (PAL_SYS_PSC_MODULE_T)(module_reset_bit);
	PAL_SYS_PSC_MODINFO_T info;

	PAL_sysPscGetModuleInfo(module_id, &info);
	
	if(info.state == PSC_ENABLE)
			return OUT_OF_RESET;
	else
			return IN_RESET;
}

/*! \fn void PAL_sysSystemReset(PAL_SYS_SYSTEM_RST_MODE_T mode)
    \brief This API is used the reset the system
    \param mode system reset mode
*/
void PAL_sysSystemReset(PAL_SYS_SYSTEM_RST_MODE_T mode)
{
	/* This is processor specific so should be implemented in avalanche_misc.c */
	avalanche_system_reset(mode);
}

/*! \fn PAL_SYS_SYSTEM_RESET_STATUS_T PAL_sysGetSysLastResetStatus()
    \brief This API is used to retrieve the reason for last system reset
    \return last system reset status 
			HARDWARE_RESET,
			SOFTWARE_RESET0,
			WATCHDOG_RESET,
			SOFTWARE_RESET1)
*/
PAL_SYS_SYSTEM_RESET_STATUS_T PAL_sysGetSysLastResetStatus()
{
    volatile unsigned int *sys_reset_status = (unsigned int*) AVALANCHE_RST_CTRL_RSTYPE_BASE;
	PAL_SYS_SYSTEM_RESET_STATUS_T mode;

	for(mode = (PAL_SYS_SYSTEM_RESET_STATUS_T)(0); mode < RST_STAT_END; mode++){
		if((*sys_reset_status & (1 << mode)))
				break;
	}
    return mode;
}

