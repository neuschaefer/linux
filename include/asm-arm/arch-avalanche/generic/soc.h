/*
 *
 * soc.h
 * Description:
 * soc Register Read/Write macros, clk helpers
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

#ifndef __SOC_H__
#define __SOC_H__

#include <mach/hardware.h>

#define REG8_ADDR(addr)             (volatile UINT8 *)(IO_ADDRESS(addr))
#define REG8_DATA(addr)             (*(volatile UINT8 *)(IO_ADDRESS(addr)))
#define REG8_WRITE(addr, data)      REG8_DATA(addr) = data;
#define REG8_READ(addr, data)       data = (UINT8) REG8_DATA(addr);

#define REG16_ADDR(addr)            (volatile UINT16 *)(IO_ADDRESS(addr))
#define REG16_DATA(addr)            (*(volatile UINT16 *)(IO_ADDRESS(addr)))
#define REG16_WRITE(addr, data)     REG16_DATA(addr) = data;
#define REG16_READ(addr, data)      data = (UINT16) REG16_DATA(addr);

#define REG32_ADDR(addr)            (volatile UINT32 *)(IO_ADDRESS(addr))
#define REG32_DATA(addr)            (*(volatile UINT32 *)(IO_ADDRESS(addr)))
#define REG32_WRITE(addr, data)     REG32_DATA(addr) = data;
#define REG32_READ(addr, data)      data = (UINT32) REG32_DATA(addr);

#define FREQ_1MHZ               1000000

#if !defined(_ASMLANGUAGE)
        
#include "haltypes.h"

/*****************************************************************************
 * CPU identification
 *****************************************************************************/
#if defined (CONFIG_MACH_PUMA5)

typedef enum AVALANCHE_CPU_TYPE_tag
{
    CPU_AVALANCHE_I         = 0,    
    CPU_AVALANCHE_D         = 2,
    CPU_PUMA                = 4,
    CPU_PUMAS               = 0x0104,
    CPU_SANGAM              = 0x5,
    CPU_TITAN               = 0x7,
    CPU_APEX                = 0xb,
    CPU_YAMUNA              = 0x2b, /* CVR register */
    CPU_PUMA5               = 0x2b, /* CVR register */
    CPU_PUMA3               = 0x0204,
    CPU_UNIDENT             = 0xFF
    
}AVALANCHE_CPU_TYPE_T;

AVALANCHE_CPU_TYPE_T avalanche_get_cpu_type(void);
const char * avalanche_get_cpu_name(AVALANCHE_CPU_TYPE_T cpu_type);

typedef int (*SET_MDIX_ON_CHIP_FN_T)(unsigned int base_addr, unsigned int operation);
int avalanche_set_mdix_on_chip(unsigned int base_addr, unsigned int operation);
unsigned int avalanche_is_mdix_on_chip(void);

unsigned int avalanche_get_vbus_freq(void);
void         avalanche_set_vbus_freq(unsigned int);

extern unsigned int avalanche_arm_freq;
extern unsigned int avalanche_vbus_freq;

#define AVALANCHE_ARM_FREQ     (avalanche_arm_freq)
#define AVALANCHE_VBUS_FREQ     (avalanche_vbus_freq)

#ifdef SEAD2_EMLN
#define AVALANCHE_UART_FREQ     3686400
#else
#define AVALANCHE_UART_FREQ     AVALANCHE_VBUS_FREQ
#endif

#endif

unsigned int avalanche_get_chip_version_info(void);

#endif /* !defined(_ASMLANGUAGE) */

#endif /* __SOC_H__ */


