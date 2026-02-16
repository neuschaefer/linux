/*
 *
 * pp_server.h
 * Description:
 * pp over MRPC
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2016 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#ifndef __HIL_CMDS_H__
#define __HIL_CMDS_H__
#include <linux/mrpc/pp_server.h>

struct hil_funcs {
    AVALANCHE_PP_RET_e (*pp_hil_set_disable_state)(Uint32);
};

int mrpc_hil_cmds_server_register(void *callbacks, int module);
void mrpc_hil_cmds_server_unregister(int module);

#endif /* __HW_MRPC_H__ */
