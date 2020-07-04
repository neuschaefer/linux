/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef __LINUX_BRIDGE_EBT_VLAN_H
#define __LINUX_BRIDGE_EBT_VLAN_H

#include <linux/types.h>

#define EBT_VLAN_ID	0x01
#define EBT_VLAN_PRIO	0x02
#define EBT_VLAN_ENCAP	0x04
#define EBT_VLAN_MASK (EBT_VLAN_ID | EBT_VLAN_PRIO | EBT_VLAN_ENCAP)
#define EBT_VLAN_MATCH "vlan"

/* start of add  to support vlan range */
#define EBT_VLAN_ID_RANGE	0x08
#define EBT_VLAN_PRIO_RANGE	0x10
/* end of add  to support vlan range */

struct ebt_vlan_info {
	__u16 id;		/* VLAN ID {1-4095} */
	__u8 prio;		/* VLAN User Priority {0-7} */
	__be16 encap;		/* VLAN Encapsulated frame code {0-65535} */

/* start of add  to support vlan range */
	__u16 id_min;
	__u16 id_max;
	__u8  prio_min;
	__u8  prio_max;
/* end of add  to support vlan range */
	__u8 bitmask;		/* Args bitmask bit 1=1 - ID arg,
				   bit 2=1 User-Priority arg, bit 3=1 encap*/
	__u8 invflags;		/* Inverse bitmask  bit 1=1 - inversed ID arg, 
				   bit 2=1 - inversed Pirority arg */
};

#endif
