/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef __NBX_EC_IPC_LIDSW_H_INCLUDED__
#define __NBX_EC_IPC_LIDSW_H_INCLUDED__

#include <linux/types.h>

/*
 *  get current state.
 *
 *  ret : less than 0 is not initialied.
 *        0 is opened.
 *        more than 0 is closed.
 */
int nbx_ec_ipc_lidsw_get_state(void);

/*
 *  register callback function. callback when lid switch changed.
 *  
 *  arg : "changed", address of callback function.
 *
 *  ret : if non 0, error occur.
 */
int nbx_ec_ipc_lidsw_register_callback( void (*changed)(void) );

/*
 *  unregister callback function.
 *  
 *  arg : "changed", address of callback function.
 */
void nbx_ec_ipc_lidsw_unregister_callback( void (*changed)(void) );

#endif /* __NBX_EC_IPC_LIDSW_H_INCLUDED__ */
