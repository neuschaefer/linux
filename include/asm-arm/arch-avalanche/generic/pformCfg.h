/*
 *
 * pformCfg.h
 * Description:
 * platform configuration file
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

#ifndef __PFORM_CFG_H__
#define __PFORM_CFG_H__

#include <linux/module.h>

/* Common #includes for Puma5/6/7 */
#include <asm-arm/arch-avalanche/generic/soc.h>
#include <asm-arm/arch-avalanche/generic/ti_gpl_wrapper.h>
#include <asm-arm/arch-avalanche/generic/ramtest.h>
#include <asm-arm/arch-avalanche/generic/avalanche_io_power_cntl.h>
#include <mach/puma.h>
#if defined(CONFIG_ARM_AVALANCHE_INTC)
#include <asm-arm/arch-avalanche/generic/puma_intc.h>
#endif

#if defined(CONFIG_MACH_PUMA5)      /* For Puma-5 SoC */
#include <asm-arm/arch-avalanche/puma5/puma5_clk_cntl.h>
#include <asm-arm/arch-avalanche/puma5/puma5_boards.h>
#include <asm-arm/arch-avalanche/puma5/puma5_cppi.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/puma5/puma5_spi.h>
#endif

#if defined(CONFIG_MACH_PUMA6)     /* For Puma-6 SoC */
#include <asm-arm/arch-avalanche/puma6/puma6_clk_cntl.h>
#include <asm-arm/arch-avalanche/puma6/puma6_boards.h>
#include <asm-arm/arch-avalanche/puma6/puma6_cppi.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/puma6/puma6_spi.h>
#include <asm-arm/arch-avalanche/puma6/puma6_bootcfg_ctrl.h>
#endif

#if defined(CONFIG_MACH_PUMA7)     /* For Puma-7 SoC */
#include <asm-arm/arch-avalanche/puma7/puma7_clk_cntl.h>
#include <asm-arm/arch-avalanche/puma7/puma7_boards.h>
#include <asm-arm/arch-avalanche/puma7/puma7_cppi.h>
#include <asm-arm/arch-avalanche/generic/pal_cppi41.h>
#include <asm-arm/arch-avalanche/puma7/puma7_spi.h>
#include <asm-arm/arch-avalanche/puma7/spi_master/puma_spi_master.h>
#include <asm-arm/arch-avalanche/puma7/puma7_bootcfg_ctrl.h>
#endif 


#if defined(CONFIG_ARM_AVALANCHE_INTC)
#include <asm-arm/arch-avalanche/generic/avalanche_intc.h>
#include <asm-arm/arch-avalanche/generic/avalanche_intd.h>
#endif

#if defined(CONFIG_ARM_AVALANCHE_PCI)
#include <asm/arch/generic/avalanche_pci.h>
#endif

#if defined(CONFIG_ARM_AVALANCHE_QUICK_IIC)
#include <asm/arch/generic/avalanche_i2c.h>
#endif

#include <asm-arm/arch-avalanche/generic/haltypes.h>

#if defined(CONFIG_ARM_AVALANCHE_LED)
#include <asm/arch/generic/led_ioctl.h>
#include <asm/arch/generic/led_config.h>
#include <asm-arm/arch-avalanche/generic/led_hal.h>
#endif

#if defined(CONFIG_ARM_AVALANCHE_COLORED_LED)
#include <asm-arm/arch-avalanche/generic/led_hal.h>
#endif

#if defined(CONFIG_DMA_NONCOHERENT)
#ifdef CONFIG_K0_COHERENCY_ALGO_WT_WA
#define MIPS_4KC
#else
#define MIPS_4KEC
#endif
#endif

#if defined(CONFIG_CPU_BIG_ENDIAN)
#define PAL_NATIVE_ENDIAN_BIG
#endif


#if !defined(CONFIG_DMA_NONCOHERENT) && !defined(CONFIG_MIPS_UNCACHED)
#error "Error in MIPS cache configuration. PAL SYS cache APIs will not work."
#endif

#endif /* __PFORM_CFG_H__ */
