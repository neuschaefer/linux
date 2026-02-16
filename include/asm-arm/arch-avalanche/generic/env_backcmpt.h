/*
 *
 * 
 * env_backcmpt.h
 * Description:
 *   This file supports the backward compatibility with Adam2 environment
 *   variables support. This is for OS co-working on boards that run Adam2 as
 *   bootloader.
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

#if 0
#include <sbl/env.h>
#include <sbl/stddef.h>
#endif

/* 
 * This array provides the mapping of the new env variables with the Adam2
 * counter parts.
 */
typedef struct ADAM2ENVDESC {
	char        *new_nm;
	char        *nm;
} ADAM2ENVDESC;
ADAM2ENVDESC env_adam2_alias[] = {
	{"CPUFREQ",      "cpufrequency",        },
	{"MEMSZ",        "memsize",             },
	{"FLASHSZ",      "flashsize",           },
	{"MODETTY0",     "modetty0",            },
	{"MODETTY1",     "modetty1",            },
	{"PROMPT",       "prompt",              },
	{"BOOTCFG",      "bootcfg",              },
	{"HWA_0",        "maca",                },
	{"HWA_1",        "macb",                },
	{"HWA_RNDIS",    "usb_board_mac",       },
	{"HWA_3",        "macc",                },
	{"IPA",          "my_ipaddress",        },
	{"IPA_SVR",      "remote_ipaddress",    },
	{"IPA_GATEWAY",  "ipa_gateway",         },
	{"SUBNET_MASK",  "subnet_mask",         },
	{"BLINE_MAC0",   "bootline1",           },
	{"BLINE_MAC1",   "bootline2",           },
	{"BLINE_RNDIS",  "rndisbootline",       },
	{"BLINE_ATM",    "atmbootline",         },
	{"USB_PID",      "usb_prod_id",         },
	{"USB_VID",      "usb_vend_id",         },
	{"USB_EPPOLLI",  "usb_ep_poll",         },
    {"USB_SERIAL",   "usb_serial",          },
   	{"HWA_HRNDIS",   "usb_rndis_mac",      }
};
