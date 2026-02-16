/*
 *
 * pal_sys.h
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


/** \file   pal_sys.h
    \brief  PAL SoC level API header file

    This file defines data types and services (macros as well as functions)
    that are applicable to the abstracted h/w system (SoC/Board).


    \author     PSP Architecture Team
    \version    1.0
 */

#ifndef __PAL_SYS_H__
#define __PAL_SYS_H__

#define BOARD_TYPE_UNKNOWN 0xFF
/*****************************************************************************
 * Reset Control Module
 *****************************************************************************/

typedef enum PAL_SYS_RESET_CTRL_tag
{
    IN_RESET        = 0,
    OUT_OF_RESET,
    CLK_DISABLE,
    CLK_ENABLE
} PAL_SYS_RESET_CTRL_T;

typedef enum PAL_SYS_SYSTEM_RST_MODE_tag
{
    RESET_SOC_WITH_MEMCTRL      = 1,    /* SW0 bit in SWRCR register */
    RESET_SOC_WITHOUT_MEMCTRL   = 2     /* SW1 bit in SWRCR register */
} PAL_SYS_SYSTEM_RST_MODE_T;

typedef enum PAL_SYS_SYSTEM_RESET_STATUS_tag
{
    HARDWARE_RESET = 0,		/**< Power On Reset */
    SOFTWARE_RESET0,   		/**< External Warm Reset*/
    WATCHDOG_RESET,			/**< Maximum Reset - this could be from watchdog or emulation */
	SOFTWARE_RESET1,		/**< System/Chip Reset */
	RST_STAT_END
} PAL_SYS_SYSTEM_RESET_STATUS_T;

void PAL_sysResetCtrl(unsigned int reset_module,PAL_SYS_RESET_CTRL_T reset_ctrl);
PAL_SYS_RESET_CTRL_T PAL_sysGetResetStatus(unsigned int reset_module);
void PAL_sysSystemReset(PAL_SYS_SYSTEM_RST_MODE_T mode);

typedef void (*REMOTE_VLYNQ_DEV_RESET_CTRL_FN)(unsigned int reset_module,
                                               PAL_SYS_RESET_CTRL_T reset_ctrl);


/*****************************************************************************
 * Power Control Module
 *****************************************************************************/
/** \enum PAL_SYS_POWER_CTRL_tag
	\brief Enum for power control states
	\TODO Move this to pal_sys.h
*/
typedef enum PAL_SYS_POWER_CTRL_tag
{
	/* these enums below are used for modules alone */
	PSC_SW_RST_DISABLE=0,	/**< Completely OFF (IN RESET) state - reset asserted and clock gated */
	PSC_SYNC_RESET,			/**< Sync reset - reset asserted and clock running */
	PSC_DISABLE, 			/**< Low power mode - Reset deasserted and clock gated */
	PSC_ENABLE,				/**< Completely ON (OUT OF RESET) state - reset deasserted and clock running */

	/* These enums should be used only for domains or from pal_sysPowerCtrl.c */
    POWER_CTRL_POWER_UP,		/**< Power On */
    POWER_CTRL_POWER_DOWN,			/**< Power Off */
} PAL_SYS_POWER_CTRL_T;


typedef enum PAL_SYS_SYSTEM_POWER_MODE_tag
{
    GLOBAL_POWER_MODE_RUN       = 0,    /* All system is up */
    GLOBAL_POWER_MODE_IDLE,             /* MIPS is power down, all peripherals working */
    GLOBAL_POWER_MODE_STANDBY,          /* Chip in power down, but clock to ADSKL subsystem is running */
    GLOBAL_POWER_MODE_POWER_DOWN        /* Total chip is powered down */
} PAL_SYS_SYSTEM_POWER_MODE_T;

void PAL_sysPowerCtrl(unsigned int power_module,  PAL_SYS_POWER_CTRL_T power_ctrl);

/*****************************************************************************
 * Wakeup Control
 *****************************************************************************/

typedef enum PAL_SYS_WAKEUP_INTERRUPT_tag
{
    WAKEUP_INT0 = 1,
    WAKEUP_INT1 = 2,
    WAKEUP_INT2 = 4,
    WAKEUP_INT3 = 8
} PAL_SYS_WAKEUP_INTERRUPT_T;

typedef enum PAL_SYS_WAKEUP_CTRL_tag
{
    WAKEUP_DISABLED = 0,
    WAKEUP_ENABLED
} PAL_SYS_WAKEUP_CTRL_T;

typedef enum PAL_SYS_WAKEUP_POLARITY_tag
{
    WAKEUP_ACTIVE_HIGH = 0,
    WAKEUP_ACTIVE_LOW
} PAL_SYS_WAKEUP_POLARITY_T;


/*****************************************************************************
 * GPIO Control
 *****************************************************************************/

typedef enum PAL_SYS_GPIO_PIN_MODE_tag
{
    FUNCTIONAL_PIN = 0,
    GPIO_PIN = 1
} PAL_SYS_GPIO_PIN_MODE_T;

typedef enum PAL_SYS_GPIO_PIN_DIRECTION_tag
{
    GPIO_OUTPUT_PIN = 0,
    GPIO_INPUT_PIN = 1

} PAL_SYS_GPIO_PIN_DIRECTION_T;

typedef enum { GPIO_FALSE, GPIO_TRUE } PAL_SYS_GPIO_BOOL_T;

typedef enum
{
    GPIO_PIN_DISABLE = 0,
    GPIO_PIN_ENABLE
}PAL_SYS_GPIO_PIN_STATUS_T;

typedef enum
{
    GPIO_PIN_LOW = 0,
    GPIO_PIN_HIGH
}PAL_SYS_GPIO_PIN_POLARITY_T;

#ifdef CONFIG_ARM_AVALANCHE_SOC
INT32 PAL_sysGpioInBit(UINT32 gpio_pin);
INT32 PAL_sysGpioOutBit(UINT32 gpio_pin, INT32 value);
#if (PUMA7_OR_NEWER_SOC_TYPE)
INT32 PAL_sysGpioSetStatus(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_STATUS_T status);
INT32 PAL_sysGpioGetStatus(UINT32 gpio_pin);
INT32 PAL_sysGpioSetPolarity(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_POLARITY_T polarity);
INT32 PAL_sysGpioResetAVB(void);
INT32 PAL_sysGpioAVBconfig(UINT32 gpio_pin);
INT32 PAL_sysGpioGetAVBcounterValue(void);
#endif
#if  defined (CONFIG_MACH_PUMA5)
INT32 PAL_sysGpioCtrl(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_MODE_T pin_mode, PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction);
INT32 PAL_sysGpioOutValue(UINT32 out_val, UINT32 out_mask,UINT32 reg_index);
#else
PAL_SYS_GPIO_PIN_DIRECTION_T PAL_sysGpioCtrlGetDir(UINT32 gpio_pin);
INT32 PAL_sysGpioCtrlSetDir(UINT32 gpio_pin, PAL_SYS_GPIO_PIN_DIRECTION_T pin_direction);
#endif
/*****************************************************************************
 * CLKC Control
 *****************************************************************************/

void PAL_sysClkcInit(void* param);
int PAL_sysClkcSetFreq(PAL_SYS_CLKC_ID_T clk_id, unsigned int output_freq);
int PAL_sysClkcGetFreq(PAL_SYS_CLKC_ID_T clk_id);
#endif
/*****************************************************************************
 * MISC
 *****************************************************************************/

unsigned int PAL_sysGetChipVersionInfo(void);

typedef struct module_info {
        Uint32 version;
        Uint32 base_addr;
} MOD_INFO_T;

#if defined (CONFIG_MACH_PUMA5)
typedef struct board_info {
        MOD_INFO_T modules[MAX_MODULES];
}BOARD_INFO_T;

extern BOARD_INFO_T soc[];

PAL_Result PAL_sysProbeAndPrep(Uint32 version, Uint32 base_addr, void *param);
PAL_Result avalanche_device_prepare(Uint32 module_id, Uint32 base_addr, BOARD_ID board_variant, void *param);
#endif

#ifdef CONFIG_ARM_AVALANCHE_SOC
/*****************************************************************************
 * CACHE
 *****************************************************************************/

int PAL_sysCacheInvalidate(PAL_OsMemAddrSpace addrSpace,
                             void *mem_start_ptr,
                             unsigned int num_bytes);

int PAL_sysCacheFlush(PAL_OsMemAddrSpace addrSpace,
                        	void *mem_start_ptr,
                        	unsigned int num_bytes);

int PAL_sysCacheFlushAndInvalidate(PAL_OsMemAddrSpace addrSpace,
                             void *mem_start_ptr,
                             unsigned int num_bytes);
#if defined (CONFIG_MACH_PUMA5)
#include <asm-arm/arch-avalanche/generic/pal_sysPsc.h>
#endif
#include "pal_sysWdtimer.h"
#include "pal_sysTimer16.h"
#endif
#endif
