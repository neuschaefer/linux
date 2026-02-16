/*
 * Copyright (C) 2009 - 2010, CyberTAN Corporation
 *
 * All Rights Reserved.
 * You can redistribute it and/or modify it under the terms of the GPL v2
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND CYBERTAN GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. CYBERTAN
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NON INFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 ******************************************************************************************************
 Includes Intel Corporation's changes/modifications
 Changed/modified portions :
 * -  X-tables API adapted for kernel 2.6.39
 * -  Added IPT_TRIGGER_BOTH_PROTOCOLS (matches both TCP and UDP)
 * -  Added, improved Debug prints
 * -  Formatting improvements and general code cleanup
 *
 * Copyright © 2011-2016, Intel Corporation.
 ******************************************************************************************************
 */

#ifndef _IPT_TRIGGER_H_target
#define _IPT_TRIGGER_H_target
#define IPT_TRIGGER_BOTH_PROTOCOLS 253  /* unassigned number */

enum ipt_trigger_type
{
    IPT_TRIGGER_DNAT = 1,
    IPT_TRIGGER_IN = 2,
    IPT_TRIGGER_OUT = 3
};

struct ipt_trigger_ports {
    u_int16_t mport[2];    /* Related destination port range */
    u_int16_t rport[2];    /* Port range to map related destination port range to */
};

struct ipt_trigger_info {
    enum ipt_trigger_type type;
    u_int16_t proto;       /* Related protocol */
    struct ipt_trigger_ports ports;
};

#endif /*_IPT_TRIGGER_H_target*/
