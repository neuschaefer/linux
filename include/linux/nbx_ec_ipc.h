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
#ifndef __NBX_EC_IPC_H_INCLUDED__
#define __NBX_EC_IPC_H_INCLUDED__

#include <linux/types.h>
#include <linux/device.h>

/* send request packet and receive response packet, without timeout version */
#define ec_ipc_send_request(pid, cid, buf, size, response_buf, response_buf_size) \
	ec_ipc_send_request_timeout((pid), (cid), (buf), (size), (response_buf), (response_buf_size), 0)

/*
 *  send request packet and receive response packet.
 *
 *  arg : "pid",               request packet PID.
 *        "cid",               request packet CID.
 *        "buf",               request packet PAYLOAD data.
 *        "size",              request packet PAYLOAD size.
 *        "response_buf",      response packet PAYLOAD data store area.
 *        "response_buf_size", response packet PAYLOAD data store area size limit.
 *        "timeout_ms",        time limit of receive response packet, specifies ms.
 *
 *  ret : received request packet PAYLOAD data size.
 *        if less than 0, error occur.
 */
ssize_t ec_ipc_send_request_timeout(uint8_t pid, uint8_t cid, const uint8_t* buf, int size,
				uint8_t* response_buf, int response_buf_size, int timeout_ms);

/*
 *  register/unregister event packet received callback.
 *
 *  arg : "cid",        callback when same COMID received.
 *        "event_func", address of callback function.
 *          arg : "buf",  event packet PAYLOAD data.
 *                "size", event packet PAYLOAD size.
 *
 *  ret : if non 0, error occur.
 */
int ec_ipc_register_recv_event(uint8_t cid, void (*event_func)(const uint8_t* buf, int size));
int ec_ipc_unregister_recv_event(uint8_t cid);

/*
 *  send request packet async
 *
 *  arg : "pid",              request packet PID.
 *        "cid",              request packet CID.
 *        "buf",              request packet PAYLOAD data.
 *        "size",             request packet PAYLOAD size.
 *        "res_func",         address of callback when response packet received.
 *          arg : "buf",          response packet PAYLOAD data.
 *                "size",         response packet PAYLOAD size.
 *                "private_data", same data as send request.
 *        "private_data", same data in callback argument.
 *
 *        if "res_func" is NULL, send to unnecessary response version.
 *
 *  ret : allocated frame number. use this when cancel.
 *        if less than 0, error occur.
 */
int ec_ipc_send_request_async(uint8_t pid, uint8_t cid, const uint8_t* buf, int size,
			void (*res_func)(const uint8_t* buf, int size, void* private_data),
			void* private_data);

/*
 *  cancel request packet
 *
 *  arg : "pid",        requested packet PID.
 *        "cid",        requested packet CID.
 *        "frame_num",  requested packet FRAME_NUM.
 *                        returned by ec_ipc_send_request_async().
 */
void ec_ipc_cancel_request(uint8_t pid, uint8_t cid, uint8_t frame_num);

/* oneway version */
#define ec_ipc_send_request_oneway(pid, cid, buf, size)			\
	ec_ipc_send_request_async((pid), (cid), (buf), (size), NULL, NULL)


#define EC_IPC_PID_LED          1
#define EC_IPC_PID_LIGHTSENSOR  2
#define EC_IPC_PID_IR           3
#define EC_IPC_PID_LID          4
#define EC_IPC_PID_BATTERY      5
#define EC_IPC_PID_SYSFS        6
#define EC_IPC_PID_POWERSTATE   7
#define EC_IPC_PID_TOPCOVER     8
#define EC_IPC_PID_POWERKEY     9
#define EC_IPC_PID_ECINT       10

/*
 * get EC INT factor at last suspend
 *
 * arg : "factor", query EC INT factor
 *
 * ret : occure EC INT queried factor
 */
int ec_ipc_get_ec_int_factor(uint8_t factor);

#define EC_INT_POWERKEY (1 << 0)
#define EC_INT_LID      (1 << 1)
#define EC_INT_TOPCOVER (1 << 2)
#define EC_INT_SHUTREQ  (1 << 3)
#define EC_INT_AC       (1 << 4)

struct nbx_ec_ipc_device {
	const char* name;
	struct device dev;
};

struct nbx_ec_ipc_driver {
	int (*probe)(struct nbx_ec_ipc_device*);
	int (*remove)(struct nbx_ec_ipc_device*);
	void (*shutdown)(struct nbx_ec_ipc_device*);

	int (*prepare_suspend)(struct nbx_ec_ipc_device*);
	int (*suspend)(struct nbx_ec_ipc_device*);
	int (*suspend_noirq)(struct nbx_ec_ipc_device*);
	int (*resume_noirq)(struct nbx_ec_ipc_device*);
	int (*resume)(struct nbx_ec_ipc_device*);
	void (*complete_resume)(struct nbx_ec_ipc_device*);

	struct device_driver drv;
};

int nbx_ec_ipc_device_register(struct nbx_ec_ipc_device* edev);
void nbx_ec_ipc_device_unregister(struct nbx_ec_ipc_device* edev);
int nbx_ec_ipc_driver_register(struct nbx_ec_ipc_driver* edrv);
void nbx_ec_ipc_driver_unregister(struct nbx_ec_ipc_driver* edrv);

int nbx_ec_ipc_core_device_register(struct nbx_ec_ipc_device* edev);
void nbx_ec_ipc_core_device_unregister(struct nbx_ec_ipc_device* edev);

#endif /* __NBX_EC_IPC_H_INCLUDED__ */
