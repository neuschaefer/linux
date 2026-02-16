/* 

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


#ifndef __PUMA7_HARDWARE_H
#define __PUMA7_HARDWARE_H

//#include <asm/arch/memory.h>

/*
 * Mapping areas size
 */
#define DEVICE_REGION_1_SIZE    	 (0x10000000)    /* Local NET-IP IO   */
#define IOSF_DEVICE_REGION_SIZE      (0x10000000)    /* IOSF Bridge         */
#define INTC_DEVICE_REGION_SIZE      (0x00100000)    /* DOCSIS IP INTC */

#define DEVICE_FREE_START_1          (0xD0000000)    


#define PCIO_BASE		(0)
#define IO_RELOC_ADDR	(DEVICE_FREE_START)	

/*-------------------------------*/
/*   Puma 7 MMU MAP  */
/*-------------------------------*/
/* 0x5000_0000 - 0x5010_0000 --> INTC         */ 
/* 0xC000_0000 - 0xEFFF_FFFF --> IOSF IO */
/* 0xF000_0000 - 0xFFFF_FFFF --> DOCSIS_IP IO */
/*-------------------------------*/

/* SRAM, BootCFG, UART, WDT, etc., Region */
#define IO_PHY        	(0xF0000000)
#define IO_START		(IO_PHY)
#define IO_SIZE         (DEVICE_REGION_1_SIZE)
#define IO_VIRT         (DEVICE_FREE_START_1)
#define IO_BASE			(IO_VIRT)
#define IO_END			(IO_VIRT + IO_SIZE)
#define IO_PHY_END      ((u64)IO_PHY  + (u64)IO_SIZE)

/* Memory Mapped serial flash 0 Region */
#define IOSF_PHY	    (0xC0000000)
#define IOSF_SIZE	    (IOSF_DEVICE_REGION_SIZE) 
#define IOSF_VIRT	    (IO_END)
#define IOSF_END	    (IOSF_VIRT + IOSF_SIZE)

/* Interrupt controller Region */
#define INTC_PHY		(0x50000000)
#define INTC_SIZE		(INTC_DEVICE_REGION_SIZE)
#define INTC_VIRT		(IOSF_END)
#define INTC_END		(INTC_VIRT + INTC_SIZE)


#define NON_OS_RESERVED_PHY	(0)
#define NON_OS_RESERVED_SIZE	(0x01000000)
#define NON_OS_RESERVED_VIRT	(INTC_END)
#define NON_OS_RESERVED_END	(NON_OS_RESERVED_VIRT + NON_OS_RESERVED_SIZE)

#define IO_PHY_VALID(pa)    ((IO_PHY <= (pa)) && (((u64) pa) < IO_PHY_END))
#define IO_ADDRESS(pa)      (((unsigned int)(pa) - IO_PHY) + IO_VIRT)
#define IO_PHY2VIRT(pa)   	(IO_ADDRESS((unsigned int)(pa)))
#define IO_VIRT2PHY(va)     (((unsigned int)(va) - IO_VIRT) + IO_PHY)

#define IOSF_ADDRESS(pa)    (((unsigned int)(pa) - IOSF_PHY) + IOSF_VIRT)
#define IOSF_PHY2VIRT(pa)   (IOSF_ADDRESS((unsigned int)(pa)))
#define IOSF_VIRT2PHY(va)   (((unsigned int)(va) - IOSF_VIRT) + IOSF_PHY)

#define DOCSIS_DSGx_INTERNAL_ADDR_MASK  ((unsigned long)(1<<19)-1)
#define IO_VIRT2PHY_DSGx(va) ((va) & DOCSIS_DSGx_INTERNAL_ADDR_MASK)

#endif /* __PUMA7_HARDWARE_H */

