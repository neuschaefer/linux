/*
 *
 * pal_sysPscPowerCtrl.c
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

/**
 *  \file   pal_sysPowerCtrl.c
 *
 *  \brief  PAL power control APIs
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

extern void avalanche_processor_idle(void);
/*****************************************************************************
 * Power Control Module
 *****************************************************************************/
/*! \fn void PAL_sysPowerCtrl(unsigned int module_power_bit, PAL_SYS_POWER_CTRL_T power_ctrl)
    \brief This API is used to power down or power up a module
    \param module_power_bit Unique module id to be powered down or up
    \param power_ctrl power down or power up (POWER_CTRL_POWER_DOWN, POWER_CTRL_POWER_UP)
*/
void PAL_sysPowerCtrl(unsigned int module_power_bit, PAL_SYS_POWER_CTRL_T power_ctrl)
{
	PAL_SYS_PSC_MODULE_T module_id = (PAL_SYS_PSC_MODULE_T)(module_power_bit);

    if (power_ctrl == POWER_CTRL_POWER_DOWN)
	{
        /* power down the module */
		PAL_sysPscSetModuleState(module_id, PSC_DISABLE);
	}
    else if(power_ctrl == POWER_CTRL_POWER_UP)
	{
        /* power on the module */
		PAL_sysPscSetModuleState(module_id, PSC_ENABLE);
	}
	else
	{
		/* module invoking this API is aware of PSC */
		PAL_sysPscSetModuleState(module_id, power_ctrl);
	}
}

/*! \fn PAL_SYS_POWER_CTRL_T PAL_sysGetPowerStatus(unsigned int module_power_bit)
    \brief This API is used to power down or power up a module
    \param module_power_bit Unique module id whose status has to be returned
    \return power_ctrl power down or power up (POWER_CTRL_POWER_DOWN, POWER_CTRL_POWER_UP)
*/
PAL_SYS_POWER_CTRL_T PAL_sysGetPowerStatus(unsigned int module_power_bit)
{
	PAL_SYS_PSC_MODULE_T module_id = (PAL_SYS_PSC_MODULE_T)(module_power_bit);
	PAL_SYS_PSC_MODINFO_T info;

	PAL_sysPscGetModuleInfo(module_id, &info);
	
	if(info.state == PSC_ENABLE)
		return POWER_CTRL_POWER_UP;
	else
		return POWER_CTRL_POWER_DOWN;
}



/*! \fn void PAL_sysSetGlobalPowerMode(PAL_SYS_SYSTEM_POWER_MODE_T power_mode) 
    \brief This API is used to power down/idle/standby/up complete system
	\param power_mode The next mode to change (GLOBAL_POWER_MODE_RUN, 
		GLOBAL_POWER_MODE_IDLE, GLOBAL_POWER_MODE_STANDBY, GLOBAL_POWER_MODE_POWER_DOWN )

	\TODO These APIs were designed for MIPS core and old clock controllers
			These APIs might become obsolete in future release. 
			Usage of this API id not recommended.
			Instead use the PSC APIs directly.
*/
void PAL_sysSetGlobalPowerMode(PAL_SYS_SYSTEM_POWER_MODE_T power_mode)
{
	PAL_SYS_PSC_DOMAIN_T domain_id = 0;	
	switch(power_mode)
	{
		/* Complete system is up, 
		 * NOTE: there is no direct support in PSC */
		case GLOBAL_POWER_MODE_RUN :
			/* switch on all domains */
			for(domain_id = 0; domain_id < PAL_SYS_MAX_PSC_DOMAINS; domain_id++)
				PAL_sysPscSetDomainState(domain_id, POWER_CTRL_POWER_UP);

			/* processor should automatically come out of idle on the next interrupt */
			break;


		/* Processor is in power down mode, all peripheral working */
		case GLOBAL_POWER_MODE_IDLE :
				/* this function is processor specific (arm/mips) and will be implemented in avalanche_misc.c */
				avalanche_processor_idle();
				break;


		/* TODO: Chip in power down (both core and peripheral), but clock to ADSKL subsystem is running
		 * NOTE: there is no support in PSC */
		case GLOBAL_POWER_MODE_STANDBY :
				break;


		/* TODO: Total chip is powered down
		 * NOTE: there is no direct support in PSC to switch off Always On domain and processor core */
		case GLOBAL_POWER_MODE_POWER_DOWN :
				/* switch off all domains */
				for(domain_id = 0; domain_id < PAL_SYS_MAX_PSC_DOMAINS; domain_id++)
					PAL_sysPscSetDomainState(domain_id, POWER_CTRL_POWER_DOWN);
				break;

			default:
				break;
	}

}

/*! \fn PAL_SYS_SYSTEM_POWER_MODE_T PAL_sysGetGlobalPowerMode(void)
    \brief This API is used to get the power status of the system 
	\return Systems current power mode (GLOBAL_POWER_MODE_RUN, 
		GLOBAL_POWER_MODE_IDLE, GLOBAL_POWER_MODE_STANDBY, 
		GLOBAL_POWER_MODE_POWER_DOWN )

	\TODO These APIs were designed for MIPS core and old clock controllers
			These APIs might become obsolete in future release. 
			Usage of this API id not recommended.
			Instead use the PSC APIs directly.
*/
PAL_SYS_SYSTEM_POWER_MODE_T PAL_sysGetGlobalPowerMode(void)
{
	PAL_SYS_PSC_DOMAIN_T domain_id = 0;	
	PAL_SYS_PSC_DOMINFO_T info;
	int domains_on = 0;

	/* check if any domain is off */
	for(domain_id = 0; domain_id < PAL_SYS_MAX_PSC_DOMAINS; domain_id++) {
		PAL_sysGetDomainInfo(domain_id, &info);
		if(info.state == POWER_CTRL_POWER_UP) {
			domains_on++;
			break;
		}
	}

	/* check if a domain is ON */
	if(domains_on)
		return GLOBAL_POWER_MODE_RUN;
	else
		return GLOBAL_POWER_MODE_POWER_DOWN; /* since domain 0 (which holds the processor) is Always On, this cannot happen*/
}

