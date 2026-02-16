/*
 *
 * puma_intc.h 
 * Description:
 * puma intc platform data
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

#ifndef _PUMA_INTC_H
#define _PUMA_INTC_H

/* The following defines are  speciifc to PUMA5 RTL/SoC */

#define NUM_SYSTEM_PRIMERY_INTS      ( 64 )
#define NUM_SYSTEM_SECONDARY_INTS    ( 32 )
#define NUM_MAX_SYSTEM_INTS          ( NUM_SYSTEM_PRIMERY_INTS +\
                                       NUM_SYSTEM_SECONDARY_INTS )
#define HOST_MAX_INTERRUPTS          ( 2 )
#define HOST_ARM_IRQ_NUM             ( 1 )  /* ARM nIRQ Number */
/* Number of INTC Registers Mapped in the given RTL/SoC */
#define NUM_PACER_REGS               ( 4 )
#define NUM_RAW_STATUS_REGS          ( 3 )  
#define NUM_ENABLED_STATUS_REGS      ( 3 )
#define NUM_ENABLE_SET_REGS          ( 3 )
#define NUM_ENABLE_CLEAR_REGS        ( 3 )  
#define NUM_CHANNEL_MAP_REGS         ( 24 )
#define NUM_HOST_INT_MAP_REGS        ( 2 )
#define NUM_PRIORITY_HINT_REGS       ( 2 )
#define NUM_POLARITY_REGS            ( 3 ) 
#define NUM_TYPE_REGS                ( 3 )
#define NUM_SECURE_ENABLE_SET_REGS   ( 3 ) 
#define NUM_SECURE_ENABLE_CLEAR_REGS ( 3 )
#define NUM_NESTING_LVL_REGS         ( 2 ) 
#define NUM_ENABLE_HINT_REGS         ( 1 ) 
#endif /*_PUMA_INTC_H */


