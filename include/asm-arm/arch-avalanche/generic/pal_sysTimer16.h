/*
 *
 * pal_sysTimer16.h
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

/******************************************************************************
 * FILE PURPOSE:    16 bit Timer Module Header
 ********************************************************************************
 * FILE NAME:       timer16.h
 *
 * DESCRIPTION:     Platform and OS independent API for 16 bit timer module
 *
 * REVISION HISTORY:
 * 27 Nov 02 - PSP TII  
 * 
 *******************************************************************************/

#ifndef __TIMER16_H__
#define __TIMER16_H__

/****************************************************************************
 * Type:        PAL_SYS_TIMER16_STRUCT_T
 ****************************************************************************
 * Description: This type defines the hardware configuration of the timer
 *              
 ***************************************************************************/
typedef struct PAL_SYS_TIMER16_STRUCT_tag
{
    UINT32 ctrl_reg;
    UINT32 load_reg;
    UINT32 count_reg;
    UINT32 intr_reg;
} PAL_SYS_TIMER16_STRUCT_T;


/****************************************************************************
 * Type:        PAL_SYS_TIMER16_MODE_T
 ****************************************************************************
 * Description: This type defines different timer modes. 
 *              
 ***************************************************************************/
typedef enum PAL_SYS_TIMER16_MODE_tag
{
    TIMER16_CNTRL_ONESHOT  = 0,
    TIMER16_CNTRL_AUTOLOAD = 2
} PAL_SYS_TIMER16_MODE_T;



/****************************************************************************
 * Type:        PAL_SYS_TIMER16_CTRL_T
 ****************************************************************************
 * Description: This type defines start and stop values for the timer. 
 *              
 ***************************************************************************/
typedef enum PAL_SYS_TIMER16_CTRL_tag
{
    TIMER16_CTRL_STOP = 0,
    TIMER16_CTRL_START
} PAL_SYS_TIMER16_CTRL_T ;


void PAL_sysTimer16GetFreqRange(UINT32  refclk_freq,
                            UINT32 *p_max_usec,
                            UINT32 *p_min_usec);                                

INT32 PAL_sysTimer16SetParams(UINT32 base_address,
                         UINT32 refclk_freq,
                         PAL_SYS_TIMER16_MODE_T mode,
                         UINT32 usec,
                         INT32 *err_nsec
                         );

void PAL_sysTimer16Ctrl(UINT32 base_address, PAL_SYS_TIMER16_CTRL_T status);

UINT32 PAL_sysTimer16GetNSecPerCount(UINT32 base_address, UINT32 load_time_nsec);

/****************************************************************************
 * FUNCTION: PAL_sysTimer16GetNsAfterTick
 ****************************************************************************
 * Description: Calculate nSec that passed since the last timer reload
 * param [in] base_address - of specific timer
 * param [in] ns_per_count - nSec per each timer count
 * Return - time passed since last timer reload
 ***************************************************************************/
static UINT32 inline PAL_sysTimer16GetNsAfterTick(UINT32 base_address, UINT32 ns_per_count)
{
    volatile PAL_SYS_TIMER16_STRUCT_T *p_timer;
    UINT32 load;
    UINT32 count;

    if (base_address == 0) 
    {
        return 0;
    }

    p_timer = (PAL_SYS_TIMER16_STRUCT_T *)(base_address);
    /* Must add 1 to both load and count, but since they are subtracted, not necessary on both. */
    load = p_timer->load_reg;
    count = p_timer->count_reg;

    return (load - count) * ns_per_count;
}

/****************************************************************************
 * FUNCTION: PAL_sysTimer16GetLoad
 ****************************************************************************
 * Description: Get number of counts per reload
 * param [in] base_address - of specific timer
 * Return - Number of counts per reload
 ***************************************************************************/
static UINT32 inline PAL_sysTimer16GetLoad(UINT32 base_address)
{
    volatile PAL_SYS_TIMER16_STRUCT_T *p_timer;

    if (base_address == 0) 
    {
        return 0;
    }

    p_timer = (PAL_SYS_TIMER16_STRUCT_T *)(base_address);

    /* Add 1, timer counts 0 too */
    return p_timer->load_reg + 1;
}


#endif /* __TIMER16_H__ */
