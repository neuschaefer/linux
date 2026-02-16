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

#ifndef __PP_SERVER_H__
#define __PP_SERVER_H__

struct pp_funcs {
    AVALANCHE_PP_RET_e (*avalanche_pp_pid_create)(AVALANCHE_PP_PID_t *, void *);
    AVALANCHE_PP_RET_e (*avalanche_pp_pid_set_flags)(Uint8, Uint32);
    AVALANCHE_PP_RET_e (*avalanche_pp_pid_config_range)(AVALANCHE_PP_PID_RANGE_t *);
    AVALANCHE_PP_RET_e (*avalanche_pp_flush_sessions)(Uint8, PP_LIST_ID_e);
    AVALANCHE_PP_RET_e (*avalanche_pp_hil_set_disable_state)(int);
};

int mrpc_pp_server_register(struct pp_funcs *callbacks);
void mrpc_pp_server_unregister(void);

#endif /* __HW_MRPC_H__ */
