/*
 * pal_vlynqDev.h
 * Description:
 * See below.
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
 * FILE PURPOSE:    PAL VLYNQ Device Header File
 ******************************************************************************
 * FILE NAME:       pal_vlynqDev.h
 *
 * DESCRIPTION:     PAL VLYNQ Device Header File
 *
 *******************************************************************************/

#ifndef __PAL_VLYNQ_DEV_H__
#define __PAL_VLYNQ_DEV_H__

/* The vlynq dev handle. */
typedef void PAL_VLYNQ_DEV_HND;

#include "pal_vlynq.h"

#define PAL_VLYNQ_EVENT_LOCAL_ERROR 0
#define PAL_VLYNQ_EVENT_PEER_ERROR  1

#define PAL_VLYNQ_DEV_ADD_IRQ       0
#define PAL_VLYNQ_DEV_REMOVE_IRQ    1

typedef Int32 (*PAL_VLYNQ_DEV_CB_FN)(void*, Uint32 condition, Uint32 value);

PAL_Result PAL_vlynqDevCbRegister(PAL_VLYNQ_DEV_HND   *vlynq_dev, 
	   	                     PAL_VLYNQ_DEV_CB_FN cb_fn,
				     void                *this_driver);

PAL_Result PAL_vlynqDevCbUnregister(PAL_VLYNQ_DEV_HND *vlynq_dev,
     		                       void              *this_driver);

PAL_VLYNQ_DEV_HND *PAL_vlynqDevFind(char *name, Uint8 instance);

PAL_VLYNQ_HND *PAL_vlynqDevGetVlynq(PAL_VLYNQ_DEV_HND *vlynq_dev);

PAL_Result PAL_vlynqDevFindIrq(PAL_VLYNQ_DEV_HND *vlynq_dev, Uint8 *irq, 
		                  Uint32 num_irq);

PAL_Result PAL_vlynqDevGetResetBit(PAL_VLYNQ_DEV_HND *vlynq_dev, 
                                   Uint32 *reset_bit);
           
PAL_VLYNQ_DEV_HND* PAL_vlynqDevCreate(PAL_VLYNQ_HND *vlynq, char *name,
					Uint32 instance, Int32 reset_bit,
					Bool peer);

/* Protected API(s) to be used only by pal_vlynq.c */
PAL_Result pal_vlynq_dev_init(void);

PAL_Result pal_vlynq_dev_handle_event(PAL_VLYNQ_DEV_HND *vlynq_dev,
                                       Uint32 cmd, Uint32 val);

PAL_Result pal_vlynq_dev_ioctl(PAL_VLYNQ_DEV_HND *vlynq_dev, Uint32 cmd, 
		               Uint32 cmd_val);

PAL_Result PAL_vlynqDevDestroy(PAL_VLYNQ_DEV_HND *vlynq_dev);

#endif
