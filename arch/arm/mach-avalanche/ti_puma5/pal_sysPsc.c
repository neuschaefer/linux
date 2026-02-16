/*
 *
 * pal_sysPsc.c
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

/** \file   pal_sysPsc.c
    \brief  PAL APIs for Power and Sleep Controller
	
    \author     Mansoor Ahamed (mansoor.ahamed@ti.com)
    \version    0.1
 */

#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm-arm/arch-avalanche/generic/pal_sysPsc.h>

volatile static PAL_SYS_PSC_REGMAP_T *psc = NULL;
static INT32 ccm_cntrl = 1;
static INT32 psc_bypassed = 0;

/*! \fn PAL_Result PAL_sysGetDomainInfo(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_PSC_DOMINFO_T *info)
    \brief This API is used to retrieve the current status of the domain.
    \param domain_id Unique domain id whose information has to be retrieved.
    \param info This structure holds the following domain information,
				domain state (on or off),
				always on or not, 
				error info,
				domain status reg content,
				memory sleep/wake info,
				Proxy GEM,
				IcePick support,
				External Power Control Pending state
				.
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysGetDomainInfo(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_PSC_DOMINFO_T *info)
{
	INT32 domain_reg 	= domain_id / 32;
	UINT32 domain_bit	= (1 << (domain_id % 32));

	if((info == NULL) || (domain_id >= PAL_SYS_MAX_PSC_DOMAINS))
		return -1;

	/* populate module configuration flags */
	info->flags = psc->pdcfg[domain_id] & 0x1f;

        /* Check Power Up/Down state (ignore trans states) */
        info->state = psc->pdstat[domain_id] & 0x1f;

	/* identify power error condition */
	if(psc->perrpr[domain_reg] & domain_bit)
		info->flags |= PSC_FLAG_ERROR;

	/* populate EPC flag */
	if(psc->epcpr[domain_reg] & domain_bit)
		info->flags |= PSC_FLAG_EPC_PENDING;	

	/* pdstat can be used by user to get the cause of the error, if any */
	info->pdstat = psc->pdstat[domain_id];

	return 0;
}


/*! \fn PAL_Result PAL_sysPscGetModuleInfo(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_PSC_MODINFO_T *info)
    \brief This API is used to retrieve the current status of the module.
    \param module_id Unique module id whose information has to be retrieved.
    \param info This structure holds the following module information,
				power domain information,
				present module state (Disable		-reset deasserted and clock gated, 
									  Enable		-reset deasserted and clock running,
									  SwRstDisable	-reset asserted and clock gated
									  SyncReset		-reset asserted and clock running), 
				error info,
				module status reg contents,
				No of mod clocks supported on LPSC (1 - 4),
				soft reset blocked or not
				.
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscGetModuleInfo(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_PSC_MODINFO_T *info)
{
	INT32 module_reg 	= module_id / 32;
	UINT32 module_bit	= (1 << (module_id % 32));

	if((info == NULL) || (module_id >= PAL_SYS_MAX_PSC_MODULES))
		return -1;

	/* identify the domain to which this module belongs to */
	info->domain = (psc->mdcfg[module_id] & 0x001F0000) >> 16;

	/* read the current module state */
	info->state = psc->mdstat[module_id] & 0x3F;

	/* identify module error condition */
	if(psc->merrpr[module_reg] & module_bit)
		info->flags |= PSC_FLAG_ERROR;

	/* mdstat can be used by user to get the cause of the error, if any */
	info->mdstat = psc->mdstat[module_id];

	/* number of clocks suppoted by this LPSC */
	info->numclk = (psc->mdcfg[module_id] & 0x3) + 1;

	return 0;
}

/*! \fn static PAL_Result PAL_sysPscWait(INT32 domain_reg, UINT32 domain_bit)
    \brief This API is used poll for the transition completion bit
    \param domain_reg which domain register to use
    \param domain_bit which domain bit in the register should be polled for
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscWait(INT32 domain_reg, UINT32 domain_bit)
{
	/* wait till GOSTATx becomes zero */
	while(psc->ptstat[domain_reg] & domain_bit) {
        udelay(1000);
    }

	return 0;
}

/*! \fn PAL_Result PAL_sysPscSetDomainState(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_POWER_CTRL_T mode)
    \brief This API is used to switch on or off a power domain.
    \param domain_id Unique domain id whose state has to be changed.
    \param mode New mode (POWER_CTRL_POWER_UP, POWER_CTRL_POWER_DOWN)
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscSetDomainState(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_POWER_CTRL_T mode)
{
	INT32 domain_reg 	= domain_id / 32;
	UINT32 domain_bit	= (1 << (domain_id % 32));

	if(psc_bypassed || ccm_cntrl || (domain_id >= PAL_SYS_MAX_PSC_DOMAINS))
		return -1;	
	
	/* Power State Machine (PSM) supports only On and OFF */
	if((mode < POWER_CTRL_POWER_UP) || (mode > POWER_CTRL_POWER_DOWN))
		return -1;

	/* wait for previous state transition completion */
	PAL_sysPscWait(domain_reg, domain_bit);

	/* clear the bit first */
	psc->pdctl[domain_id] &= (~(1));	

	/* program the next state */
	if(mode == POWER_CTRL_POWER_UP) 
		psc->pdctl[domain_id] |= (1);

	/* start the PSM */
/*	psc->ptcmd[domain_reg] = (1 << domain_bit);  */
	psc->ptcmd[domain_reg] |= domain_bit; 

	/* wait for state transition completion */
	PAL_sysPscWait(domain_reg, domain_bit);

	return 0;
}


/*! \fn PAL_Result PAL_sysPscSetModuleState(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_POWER_CTRL_T mode)
    \brief This API is used to change the state of a module.
    \param domain_id Unique module id whose state has to be changed.
    \param mode New momde (POWER_CTRL_PSC_ENABLE, 
							POWER_CTRL_PSC_DISABLE, 
							POWER_CTRL_PSC_SW_RST_DISABLE,
							POWER_CTRL_PSC_SYNC_RESET)
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscSetModuleState(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_POWER_CTRL_T mode)
{
	PAL_SYS_PSC_DOMAIN_T domain_id;
	INT32 domain_reg;
	UINT32 domain_bit;

	if(psc_bypassed || ccm_cntrl || (module_id >= PAL_SYS_MAX_PSC_MODULES))
		return -1;	
	
	/* Power State Machine (PSM) supports only On and OFF */
	if((mode < PSC_SW_RST_DISABLE) || (mode > PSC_ENABLE))
		return -1;

	/* pick the domain information from module config register */
	domain_id 	= (psc->mdcfg[module_id] & 0x001F0000) >> 16;
	domain_reg 	= domain_id / 32;
	domain_bit	= (1 << (domain_id % 32));

	/* wait for previous state transition completion */
	PAL_sysPscWait(domain_reg, domain_bit);

	/* program the next Module State Machine (MSM) state */
	psc->mdctl[module_id] &= (~0x1F);
	psc->mdctl[module_id] |= mode;

	/* start the PSM */
/*	psc->ptcmd[domain_reg] = (1 << domain_bit);  */
	psc->ptcmd[domain_reg] |= domain_bit; 

	/* wait for state transition completion */
	PAL_sysPscWait(domain_reg, domain_bit);

	return 0;
}

/*! \fn PAL_Result PAL_sysPscClearRegister(PAL_SYS_PSC_DOMAIN_T domain_id, 
  											PAL_SYS_PSC_MODULE_T modudle_id, 
  											PAL_SYS_PSC_REGISTER_T reg)
    \brief This API is used to clear error status (power or module) or EPC pending status for a domain/module.
			The params domain_id/module_id is used based on the register selected.
    \param domain_id Unique domain id whose status has to be cleared
    \param module_id Unique module id whose status has to be cleared
	\param reg Which register has to be cleared 
							module error pending register (PAL_SYS_PSC_MERRCR), 
							power error pending register (PAL_SYS_PSC_PERRCR),
							External power control clear register (PAL_SYS_PSC_EPCCR), 
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscClearRegister(PAL_SYS_PSC_DOMAIN_T domain_id, 
  											PAL_SYS_PSC_MODULE_T module_id, 
  											PAL_SYS_PSC_REGISTER_T reg)
{
	INT32 domain_reg 	= domain_id / 32;
	UINT32 domain_bit	= (1 << (domain_id % 32));
	INT32 module_reg 	= module_id / 32;
	UINT32 module_bit	= (1 << (module_id % 32));

	if(psc_bypassed)
		return -1;	
	
	/* sanity checks */

	if((reg < PAL_SYS_PSC_MERRCR) || (reg > PAL_SYS_PSC_EPCCR))
		return -1;

	if((module_id >= PAL_SYS_MAX_PSC_MODULES) || (domain_id >= PAL_SYS_MAX_PSC_DOMAINS))
		return -1;

	/* clear error status based on register */
	switch(reg) {
		case PAL_SYS_PSC_MERRCR:
			psc->merrcr[module_reg] |= module_bit;
			break;

		case PAL_SYS_PSC_PERRCR:
			psc->perrcr[domain_reg] |= domain_bit;
			break;

		case PAL_SYS_PSC_EPCCR:
			psc->epccr[domain_reg] |= domain_bit;
			break;

		default:
				return -1;
	}

	return 0;
}

/*! \fn PAL_Result PAL_sysPscInit()
    \brief This API is used to Initilize the PSC to default known values. Also, it checks if the PSC is 
			hardwired for bypass mode
	\param psc_base Virtual base address of PSC module
	\param ccm_controlled A value of one programs the psc such that the states can only be changed by CCM
	\param pid Contents of Peripheral id register
	\return Returns 0 on success and negative value on error;
*/
PAL_Result PAL_sysPscInit(UINT32 psc_base, INT32 ccm_controlled, UINT32 *pid)
{
	psc = (volatile PAL_SYS_PSC_REGMAP_T *)(psc_base);

	if(psc == NULL)
		return -1;

	*pid = psc->pid;

	/* check if psc is bypassed */
	if(psc->gblstat & PAL_SYS_PSC_OVERRIDE) {
		psc_bypassed = 1;
		return -1;
	}	

	/* Should Chip Config Module(CCM) control the PSC or s/w should control ? */
	if(ccm_controlled) {
		psc->gblctl |= PAL_SYS_PSC_CCM;
		ccm_cntrl = 1;
	}
	else {
		psc->gblctl &= (~PAL_SYS_PSC_CCM);
		ccm_cntrl = 0;
	}


	return 0;
}

/*! \fn PAL_Result PAL_sysPscSetMdctl(PAL_SYS_PSC_MODULE_T module_id, UINT32 value, UINT8 preserve_state)
    \brief This API is used to load the specified value to module control register
    \param module_id Unique module id whose state has to be changed.
    \param value New value to load to MDCTL (Refer PSC spec for MDCTL register bit details)
    \param preserve_state A value of "1" requests the API to update all fields except state field
    		and a value of "0" loads the new state also.
    \return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscSetMdctl(PAL_SYS_PSC_MODULE_T module_id, UINT32 value, UINT8 preserve_state)
{
    PAL_SYS_PSC_DOMAIN_T domain_id;
    INT32 domain_reg;
    UINT32 domain_bit;

    if(psc_bypassed || ccm_cntrl || (module_id >= PAL_SYS_MAX_PSC_MODULES))
        return -1;

    /* pick the domain information from module config register */
    domain_id   = (psc->mdcfg[module_id] & 0x001F0000) >> 16;
    domain_reg  = domain_id / 32;
    domain_bit  = (1 << (domain_id % 32));

    /* wait for previous state transition completion */
    PAL_sysPscWait(domain_reg, domain_bit);

    /* preserve old state */    
    if(preserve_state)
    {
    	value &= (~0x1F);
    	value |= (psc->mdstat[module_id] & 0x1F);
    }

    /* load the modified value */
    psc->mdctl[module_id] = value;
    
    /* start the PSM */
    psc->ptcmd[domain_reg] |= domain_bit;

    /* wait for state transition completion */
    PAL_sysPscWait(domain_reg, domain_bit);

    return 0;
}


#if 0 /* TODO: These are optional and of low priority and will be implemented later */

PAL_sysPscSetPowerRail();
PAL_sysPscGetPowerRail();

PAL_sysPscRegisterCallback();

PAL_sysPscISR()
		1. if EPC (External Power Controller support)
			a. call callback - callback should use getdomaininfo() ans see which domain needs EPC service,based
				on direction (on / off) switch on or off EPC and wait for it to settle down and then return
			b. set MDCTL.EPCGOOD = 0 or 1 based on direction
			c. write 1 to EPCCR
		2. if error, clear and notify respective callback

#endif 							


