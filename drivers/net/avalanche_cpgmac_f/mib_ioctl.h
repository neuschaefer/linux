/*
 *
 * mib_ioctl.h
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


#ifndef _MIB_IOCTL_H_
#define _MIB_IOCTL_H_

#include "ioctl_api.h"

typedef struct {
    unsigned long cmd;
    unsigned long port;
    void *data;
} TI_SNMP_CMD_T;

/* Ioctl/Cmd value to be used by snmpd like applications */
#define SIOTIMIB2   SIOCDEVPRIVATE + 1


#endif /* _MIB_IOCTL_H_ */
