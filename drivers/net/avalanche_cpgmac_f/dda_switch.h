/*
 *
 * dda_switch.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


#ifndef __DDA_SWITCH_H__
#define __DDA_SWITCH_H__
#include "_tistdtypes.h"
#include <pal_defs.h>

#include "ddc_cpgmac_f_Drv.h"
#include "ddc_switch.h"


typedef enum
{
    SWITCH_GETVERSION = 1,
    SWITCH_SPECCAPSET,
    SWITCH_SPECCAPGET,
    SWITCH_TRAILERSET,
    PORT_STATUSSET,
    PORT_STATUSGET,
    PORT_LINKGET,
    PORT_FCSET,
    PORT_FCGET,
    PORT_DUPLEXSET,
    PORT_DUPLEXGET,
    PORT_SPEEDSET,
    PORT_SPEEDGET,
    PORT_AUTOSET,
    PORT_AUTOGET,
    PORT_STATISTICGET,
    PORT_STATISTICRESET,
    VLAN_CREATE,
    VLAN_DELETE,
    VLAN_PORT_ADD,
    VLAN_PORT_DELETE,
    VLAN_SET_DEF,
    VLAN_PORT_UPDATE,
    VLAN_SET_PORT_PRI,
    VLAN_GET_PORT_PRI,
    DELETE_MAC_ADDR

}SwitchIoctlOpcode;

typedef struct _switchIoctlCmdType_
{
    SwitchIoctlOpcode t_opcode;
    union
    {
        struct
        {
            Uint32          PortNum;
            Uint32          Status;
        }
        PORT_config;

        struct
        {
            Uint32          PortNum;
            MIB2_COUNTER    Counters;
        }
        PORT_counters;

        struct
        {
            Uint32                  data_size;
            SWITCH_SPECIAL_CAPS     SpecialCap;
        }
        SWITCH_specialcap;

        struct
        {
            SW_VER_INFO     verinfo;
        }
        SWITCH_verinfo;

        struct
        {
            Uint32          PortNum;
            Uint32          vlnId;
            VLN_TAG         tag;
            Uint32          priority;
        }
        VLAN_info;

        struct
        {
            char    MacAddr[6];
        }
        MAC_addr;

   }msg;

}
SwitchIoctlType;

typedef struct dda_switch_ingress_port_info_t
{

   unsigned char portId[4];  /*To add the ingress trailer. */

   struct sk_buff *skb;  /*to store the skb from the stack. */

   struct marvell_ingress_port_info_t *next;
   unsigned int port;

}MARVELL_INGRESS_PORT_INFO_T;


int     external_switch_ioctl(SwitchIoctlType *userData);
void    switch_create_proc_entry(void);
void    switch_remove_proc_entry(void);


#endif /* __DDA_SWITCH_H__ */
