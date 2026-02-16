/*
 *
 * pal_cppi41cfg.h
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

/** \file   pal_cppi41cfg.h
    \brief  PAL CPPI4 Configuration header file

    This file has the static (compile time) configuration parameters for the
    CPPI4 PAL driver.

    @author     Greg Guyotte
 */

#ifndef __PAL_CPPI4_CFG_H__
#define __PAL_CPPI4_CFG_H__


/* CPPI 4.1 hardware specific */

/**
 * CPPI4 hardware specific configuration 
 */
#define CPPI4_RESET_WAIT                1     /**< Msecs to wait after reset op */

/**
 * CPPI4 Generic configuration 
 */
#define PAL_CPPI41_NUM_TD_DESC               ((PAL_CPPI41_NUM_TOTAL_CHAN) * 2)  /**< Total number of teardown desc. Just a ballpark, no science */

/**
   \brief Macros for Address conversions 
   \note: These need to be ported for a different platform other than MIPS 
 */
#define PAL_CPPI4_VIRT_2_PHYS(addr)     PAL_osMemVirt2Phy((Ptr)(addr))
#define PAL_CPPI4_PHYS_2_VIRT(addr)     PAL_osMemPhy2Virt((Uint32)(addr))
#define PAL_CPPI4_VIRT_NOCACHE(addr)    PAL_osMemVirt2VirtNoCache((Ptr)(addr))

#endif /* __PAL_CPPI4_CFG_H__ */
