
/*
 *  kernel/arm_atom_mbx.h
 *
  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014 Intel Corporation.

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

  Copyright(c) 2014 Intel Corporation. All rights reserved.

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
#ifndef _CPU_MBX_H_
#define _CPU_MBX_H_

#define MBX_DRIVER_DEV_NAME "/dev/arm_atom_mbx"
#define DEVICE_NAME "arm_atom_mbx"
#define DIGITS 80

/* RPC-IF structure */
struct npcpu_rpc_info
{
        unsigned int npcpu_ipv4_addr;
        unsigned int appcpu_ipv4_addr;
        unsigned int netmask;
        unsigned int vlan_id;
}__attribute__((packed));

struct arm11_atom_mbx_user
{
	unsigned short  eventId;
    unsigned short  isParamRequired;
	struct npcpu_rpc_info parameter;
	unsigned int resv[2];		/* Reserved */
}__attribute__((packed));


enum arm11_mbx_event_id
{
    ARM11_EVENT_GPIO_INIT_EXIT     = 0x0001,
    ARM11_EVENT_SPI_INIT_EXIT      = 0x0002,
    ARM11_EVENT_EMMC_INIT_EXIT     = 0x0004, 
    NPCPU_EVENT_RPC_IF_OBTAIN_ADDR = 0x0008, 
    ARM11_EVENT_EMMC_ADVANCE_INIT_EXIT = 0x0010,
    ARM11_MBX_ATOM_RESET_SYNC = 0x0020
};

enum atom_mbx_event_id
{
    ATOM_EVENT_RSVD   = 0x0001,
    ATOM_EVENT_SPI_ADVANCE_EXIT    = 0x0002,
    ATOM_EVENT_EMMC_ADVANCE_EXIT   = 0x0004,
      
};

#define MBX_MODULE_ID 1
#define	MBX_SEND_EVENT_CMD           _IOW(MBX_MODULE_ID, 1, struct arm11_atom_mbx_user)
#define	MBX_GET_EVENT_CMD            _IOR(MBX_MODULE_ID, 2, struct arm11_atom_mbx_user)
#define	MBX_SEND_ACK_CMD             _IOW(MBX_MODULE_ID, 3, struct arm11_atom_mbx_user)
#define	MBX_RECEIVE_ACK_CMD          _IOR(MBX_MODULE_ID, 4, struct arm11_atom_mbx_user)

#ifdef __KERNEL__

long arm_atom_mbx_receive_event_notification(unsigned short eventId, unsigned int *param, unsigned int timeout);
long arm_atom_mbx_receive_specific_ack(unsigned short eventId);
long arm_atom_mbx_send_ack(unsigned short eventID);
long arm_atom_mbx_send_notification(unsigned short eventID, unsigned int *paramPtr);
unsigned int arm_atom_mbx_get_arm_reg();


#endif
#endif	/* _CPU_MBX_H_ */
