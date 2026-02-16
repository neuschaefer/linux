/*
 *
 * pal_sysPsc.h 
 * Description:
 * see below
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

/** \file   pal_sysPsc.h
    \brief  Power and Sleep Controller header file
	
    \author     Mansoor Ahamed (mansoor.ahamed@ti.com)
    \version    0.1
 */

#ifndef __PAL_SYS_PSC_H__
#define __PAL_SYS_PSC_H__

/* Following are the flag used to determine domain/module state */
#define PSC_FLAG_ALWAYS_ON			0x00000001
#define PSC_FLAG_MEM_SLWKP			0x00000002
#define PSC_FLAG_PROXYGEM			0x00000004
#define PSC_FLAG_ICEPICK			0x00000008
#define PSC_FLAG_BLK_RST			0x00000010
#define PSC_FLAG_EPC_PENDING		0x00000020
#define PSC_FLAG_ERROR				0x00000040

/* Some important bit fields used by PSC */
#define PAL_SYS_PSC_CCM				0x00000002
#define PAL_SYS_PSC_OVERRIDE		0x00000001

/** \struct PAL_SYS_PSC_DOMINFO_T
	\brief This structure holds the domain information
*/
typedef struct PAL_SYS_PSC_DOMINFO_tag
{
	PAL_SYS_POWER_CTRL_T state; /**< This field provides the domain state (on or off) */
	UINT32 flags;			/**< This flag provides following information
								 	'Always On' domain or not (PSC_FLAG_ALWAYS_ON),
									If error flag set (PSC_FLAG_ERROR),
									Is this a Memory Sleep/Wake domain (PSC_FLAG_MEM_SLWKP),
									Is this a Proxy GEM domain (PSC_FLAG_PROXYGEM),
									Does it have Ice Pick support (PSC_FLAG_ICEPICK)
									Block all Reset except POR (PSC_FLAG_BLK_RST),
									External Power Control Pending (PSC_FLAG_EPC_PENDING) */
	UINT32 pdstat;			/**< Power Domain Status register contents */
}PAL_SYS_PSC_DOMINFO_T;

/** \struct PAL_SYS_PSC_MODINFO_T
	\brief This structure holds the domain information
*/
typedef struct PAL_SYS_PSC_MODINFO_tag
{
	INT32 domain;					/**< which power domain this module belongs to */
	PAL_SYS_POWER_CTRL_T state; 	/**< current module state 
									  Disable		-reset deasserted and clock gated, 
									  Enable		-reset deasserted and clock running,
									  SwRstDisable	-reset asserted and clock gated
									  SyncReset		-reset asserted and clock running */ 
	INT32	numclk;					/**< No of mod clocks supported on LPSC (1 - 4) */
	UINT32 flags;					/**< This flag provides following information
									  If error flag set (PSC_FLAG_ERROR)
									  Soft reset blocked (PSC_FLAG_BLK_RST) */
	UINT32 mdstat;					/**< Module status register contents */
}PAL_SYS_PSC_MODINFO_T;


/** \enum PAL_SYS_PSC_REGISTER_T
	\brief This structure holds the domain information
*/
typedef enum PAL_SYS_PSC_REGISTER_tag
{
	PAL_SYS_PSC_MERRCR=0, 	/**< module error pending register */
	PAL_SYS_PSC_PERRCR,	  	/**< power error pending register */
	PAL_SYS_PSC_EPCCR,		/**< External power control clear register */
}PAL_SYS_PSC_REGISTER_T;


/** \struct PAL_SYS_PSC_REGMAP_T
	\brief This structure holds the psc register memory map information
*/
typedef struct PAL_SYS_PSC_REGMAP_tag
{
    volatile const UINT32 pid;      /**< Peripheral ID Register */

    volatile UINT32 reserved0[3];   /* -------Reserved 0x004 to 0x00F */

    volatile UINT32 gblctl;         /**< Global Control Register */
    volatile UINT32 gblstat;        /**< Global Status Register */
    volatile UINT32 inteval;        /**< Interrupt Evaluation Register */

    volatile UINT32 reserved1[9];   /* -------Reserved 0x01F to 0x03F */

    volatile UINT32 merrpr[4];      /**< Array of Module Error Pending Registers,
                                        each register can hold 32 modules */
    volatile UINT32 merrcr[4];      /**< Array of Module Error Clear Registers,
                                        each register can hold 32 modules */
    volatile UINT32 perrpr[2];      /**< Power Error Pending Register */
    volatile UINT32 perrcr[2];      /**< Power Error Clear Register */
    volatile UINT32 epcpr[2];       /**< External Power Control Pending Register */
    volatile UINT32 epccr[2];       /**< External Power Control Clear Register */

    volatile UINT32 reserved2[32];  /* --------Reserve 0x07f to 0x100 */

    volatile UINT32 railstat;       /**< Power Rail Status Register */
    volatile UINT32 railctl;        /**< Power Rail Counter Control Register */
    volatile UINT32 railsel;     /**< Power Rail Counter Select */
    volatile UINT32 railctl_1[5];   /**< Power Rail Counter Control Register */
    volatile UINT32 ptcmd[2];       /**< Power Transition Command Register */
    volatile UINT32 ptstat[2];      /**< Power Domain Transition Status Register */

    volatile UINT32 reserved3[52];  /* --------Reserve 0x12f to 0x200 */

    volatile UINT32 pdstat[64];     /**< Power Domain Status Register (one register for each domain) */
    volatile UINT32 pdctl[64];      /**< Power Domain Control Register  (one register for each domain) */
    volatile UINT32 pdcfg[64];      /**< Power Domain Configuration Register  (one register for each domain) */

    volatile UINT32 reserved4[64];  /* --------Reserve 0x5ff to 0x400 */

    volatile const UINT32 mdcfg[128];/**< Module Configuration Register  (one register for each module) */
    volatile UINT32 mdstat[128];     /**< Module Status Register  (one register for each module) */
    volatile UINT32 mdctl[128];     /**< Module Control Register (one register for each module) */

    /* why to waste memory */
/*  volatile UINT32 reserved5[256]; */ /* --------Reserve 0xFFF to 0xC00 */
}PAL_SYS_PSC_REGMAP_T;


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
PAL_Result PAL_sysGetDomainInfo(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_PSC_DOMINFO_T *info);




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
PAL_Result PAL_sysPscGetModuleInfo(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_PSC_MODINFO_T *info); 


/*! \fn PAL_Result PAL_sysPscSetDomainState(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_POWER_CTRL_T mode)
    \brief This API is used to switch on or off a power domain.
    \param domain_id Unique domain id whose state has to be changed.
    \param mode New mode (POWER_CTRL_POWER_UP, POWER_CTRL_POWER_DOWN)
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscSetDomainState(PAL_SYS_PSC_DOMAIN_T domain_id, PAL_SYS_POWER_CTRL_T mode);


/*! \fn PAL_Result PAL_sysPscSetModuleState(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_POWER_CTRL_T mode)
    \brief This API is used to change the state of a module.
    \param domain_id Unique module id whose state has to be changed.
    \param mode New momde (POWER_CTRL_PSC_ENABLE, 
							POWER_CTRL_PSC_DISABLE, 
							POWER_CTRL_PSC_SW_RST_DISABLE,
							POWER_CTRL_PSC_SYNC_RESET)
	\return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscSetModuleState(PAL_SYS_PSC_MODULE_T module_id, PAL_SYS_POWER_CTRL_T mode);

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
  											PAL_SYS_PSC_REGISTER_T reg);




/*! \fn PAL_Result PAL_sysPscInit()
    \brief This API is used to Initilize the PSC to default known values. Also, it checks if the PSC is 
			hardwired for bypass mode
	\param psc_base Virtual base address of PSC module
	\param ccm_controlled A value of one programs the psc such that the states can only be changed by CCM
	\param pid Contents of Peripheral id register
	\return Returns 0 on success and negative value on error;
*/
PAL_Result PAL_sysPscInit(UINT32 psc_base, INT32 ccm_controlled, UINT32 *pid);

/*! \fn PAL_Result PAL_sysPscSetMdctl(PAL_SYS_PSC_MODULE_T module_id, UINT32 value, UINT8 preserve_state)
    \brief This API is used to load the specified value to module control register
    \param module_id Unique module id whose state has to be changed.
    \param value New value to load to MDCTL (Refer PSC spec for MDCTL register bit details)
    \param preserve_state A value of "1" requests the API to update all fields except state field
            and a value of "0" loads the new state also.
    \return Returns 0 on success else returns a negative value
*/
PAL_Result PAL_sysPscSetMdctl(PAL_SYS_PSC_MODULE_T module_id, UINT32 value, UINT8 preserve_state);


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



#endif /* __PAL_SYS_PSC_H__ */
