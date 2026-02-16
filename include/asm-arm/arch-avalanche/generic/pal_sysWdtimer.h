/*
 *
 * pal_sysWdTimer.h 
 * Description:
 * see below
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
 * FILE PURPOSE:    Watchdog Timer Module Header
 ********************************************************************************
 * FILE NAME:       wdtimer.h
 *
 * DESCRIPTION:     Platform and OS independent API for watchdog timer module
 *
 * REVISION HISTORY:
 * 27 Nov 02 - PSP TII  
 * 
 *******************************************************************************/


#ifndef __WDTIMER_H__
#define __WDTIMER_H__

/* Return Codes */
#define WDTIMER_RET_OK			0
#define WDTIMER_RET_ERR			-1
#define WDTIMER_ERR_INVAL		-2

/****************************************************************************
 * Type:        PLA_SYS_WDTIMER_STRUCT_T
 ****************************************************************************
 * Description: This type defines the hardware configuration of the 
 *              watchdog timer
 ***************************************************************************/
typedef struct PAL_SYS_WDTIMER_STRUCT_tag
{
    UINT32  kick_lock;
    UINT32  kick;
    UINT32  change_lock;
    UINT32  change ; 
    UINT32  disable_lock;
    UINT32  disable;
    UINT32  prescale_lock;
    UINT32  prescale;
} PAL_SYS_WDTIMER_STRUCT_T;


/****************************************************************************
 * Type:        PAL_SYS_WDTIMER_CTRL_T
 ****************************************************************************
 * Description: This type defines start and stop values for the timer. 
 *              
 ***************************************************************************/
typedef enum PAL_SYS_WDTIMER_CTRL_tag
{
    WDTIMER_CTRL_DISABLE = 0,
    WDTIMER_CTRL_ENABLE
} PAL_SYS_WDTIMER_CTRL_T ;

void PAL_sysWdtimerInit(UINT32 base_addr, UINT32 clk_freq);
INT32 PAL_sysWdtimerSetPeriod( UINT32 msec );
INT32 PAL_sysWdtimerCtrl(PAL_SYS_WDTIMER_CTRL_T wd_ctrl);
INT32 PAL_sysWdtimerKick(void);

#endif /* __WDTIMER_H__ */
