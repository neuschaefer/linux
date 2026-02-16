/*
 *
 * puma7_npcpu_appcpu_defs.h
 * Description:
 * Definitions that should be shared between NP-CPU and APP-CPU.
 
  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015-2017 Intel Corporation.

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

  Copyright(c) 2015-2016 Intel Corporation. All rights reserved.

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


#ifndef _PUMA7_NPCPU_APPCPU_DEFS_H_
#define _PUMA7_NPCPU_APPCPU_DEFS_H_

/*! \enum npcpu_appcpu_hw_mbox_tags_e
    \brief Enumerate all HW_MBOX TAG values shared between NP-CPU and APP-CPU.
*/
typedef enum npcpu_appcpu_hw_mbox_tags
{
    NPCPU_APPCPU_HW_MBOX_TAG_INVALID = -1,
    NPCPU_APPCPU_HW_MBOX_TAG_BOOT,
    NPCPU_APPCPU_HW_MBOX_TAG_PM,
    NPCPU_APPCPU_HW_MBOX_TAG_MRPC,
    NPCPU_APPCPU_HW_MBOX_TAG_RPC,
    NPCPU_APPCPU_HW_MBOX_TAG_ITSTORE,
    NPCPU_APPCPU_HW_MBOX_TAG_LAN,
    NPCPU_APPCPU_HW_MBOX_TAG_MOCA,
    NPCPU_APPCPU_HW_MBOX_TAG_NONE,  /* reserved */
    NPCPU_APPCPU_HW_MBOX_TAG_CPPI41_MBX,
    NPCPU_APPCPU_HW_MBOX_TAG_DATAPIPE_MBX,
    NPCPU_APPCPU_HW_MBOX_TAG_WIFI_MBX,
    NPCPU_APPCPU_HW_MBOX_TAG_DPI_MBX,
    NPCPU_APPCPU_HW_MBOX_TAG_EXTSWITCH,
    NPCPU_APPCPU_HW_MBOX_TAG_PUMA_STARTUP,
    NPCPU_APPCPU_HW_MBOX_TAG_SPEEDTEST,
    NPCPU_APPCPU_HW_LAST_ENTRY
} npcpu_appcpu_hw_mbox_tags_e;

#endif /* ! _PUMA7_NPCPU_APPCPU_DEFS_H_ */
