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
#ifndef __EC_IPC_SERIAL_H_INCLUDED__
#define __EC_IPC_SERIAL_H_INCLUDED__

#include <linux/types.h>

void ec_ipc_serial_init(void);
int ec_ipc_serial_open(void);
void ec_ipc_serial_close(void);
ssize_t ec_ipc_serial_read(uint8_t* buf, size_t size, int timeout_ms);
ssize_t ec_ipc_serial_write(const uint8_t* buf, size_t size);

#endif /* __EC_IPC_SERIAL_H_INCLUDED__ */
