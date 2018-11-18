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
/* EC TTY */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>

#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>

#include <linux/nbx_ec_ipc.h>

#define EC_IPC_CID_IR_REQUEST 0x60
#define EC_IPC_CID_IR_EVENT 0x61
#define EC_IPC_PAYLOAD_LIMIT 128

static struct tty_struct* ec_tty;
static DEFINE_SEMAPHORE(ec_tty_mutex);
static int open_count;

static void ec_tty_receive_func(const uint8_t* buf, int size);

static int ec_tty_open(struct tty_struct* tty, struct file* filp)
{
	int ret = 0;

	if(tty == NULL) return -ENODEV;

	down(&ec_tty_mutex);
	{
		if(open_count <= 0) {
			open_count = 0;

			ec_tty = tty;
			ret = ec_ipc_register_recv_event(EC_IPC_CID_IR_EVENT, ec_tty_receive_func);
		}
		if(ret == 0) open_count++;
	}
	up(&ec_tty_mutex);

	return ret;
}
static void ec_tty_close(struct tty_struct* tty, struct file* filp)
{
	down(&ec_tty_mutex);
	{
		open_count--;
		if(open_count <= 0) {
			open_count = 0;

			ec_ipc_unregister_recv_event(EC_IPC_CID_IR_EVENT);
			ec_tty = NULL;
		}
	}
	up(&ec_tty_mutex);
}

static void ec_tty_write_res_func(const uint8_t* buf, int size, void* priv)
{
	/* nothing to do */
}
static int ec_tty_write(struct tty_struct* tty, const unsigned char* buf, int count)
{
	int ret = 0;
	int size = 0;
	int result = 0;

	while(0 < count) {
		size = (count < EC_IPC_PAYLOAD_LIMIT)? count : EC_IPC_PAYLOAD_LIMIT;
		count -= size;

		result = ec_ipc_send_request_async(EC_IPC_PID_IR, EC_IPC_CID_IR_REQUEST,
						buf, size, ec_tty_write_res_func, NULL);
		if(0 <= result) {
			buf += size;
			ret += size;
		}
		else {
			ret = result;
			break;
		}
	}

	return ret;
}

static void ec_tty_receive_func(const uint8_t* buf, int size)
{
	int i;

	down(&ec_tty_mutex);
	do {
		if((ec_tty == NULL) || (buf == NULL) || (size <= 0)) break;

		for(i = 0; i < size; i++) {
			tty_insert_flip_char(ec_tty, buf[i], TTY_NORMAL);
		}
		tty_flip_buffer_push(ec_tty);
	} while(0);
	up(&ec_tty_mutex);
}

static int ec_tty_write_room(struct tty_struct* tty)
{
	return EC_IPC_PAYLOAD_LIMIT;
}
static void ec_tty_set_termios(struct tty_struct* tty, struct ktermios* old)
{
	/* nothing to do. */
}

static struct tty_driver* ec_tty_driver;
static struct tty_operations ec_tty_ops = {
	.open = ec_tty_open,
	.close = ec_tty_close,
	.write = ec_tty_write,
	.write_room = ec_tty_write_room,
	.set_termios = ec_tty_set_termios,
};

static int __init ec_tty_init(void)
{
	int ret = 0;
	struct device* tty_dev;

	ec_tty_driver = alloc_tty_driver(1);
	if(ec_tty_driver == NULL) {
		return -ENOMEM;
	}

	ec_tty_driver->owner = THIS_MODULE;
	ec_tty_driver->driver_name = "serial_on_nbx_ec";
	ec_tty_driver->name = "ttySIR";
	ec_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	ec_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	ec_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV;
	ec_tty_driver->init_termios = tty_std_termios;
	/* initial baudrate is 9600bps */
	ec_tty_driver->init_termios.c_cflag &= ~CBAUD;
	ec_tty_driver->init_termios.c_cflag |= B9600;
	ec_tty_driver->init_termios.c_ispeed = 9600;
	ec_tty_driver->init_termios.c_ospeed = 9600;

	tty_set_operations(ec_tty_driver, &ec_tty_ops);

	ret = tty_register_driver(ec_tty_driver);
	if(ret != 0) {
		pr_err("register NBX EC tty driver failed.\n");
		put_tty_driver(ec_tty_driver);

		return ret;
	}

	tty_dev = tty_register_device(ec_tty_driver, 0, NULL);
	if(IS_ERR(tty_dev)) {
		pr_err("register NBX EC tty device failed.\n");
		ret = PTR_ERR(tty_dev);
		tty_unregister_driver(ec_tty_driver);
	}

	open_count = 0;

	return ret;
}
static void __exit ec_tty_exit(void)
{
	tty_unregister_device(ec_tty_driver, 0);
	tty_unregister_driver(ec_tty_driver);
}

module_init(ec_tty_init);
module_exit(ec_tty_exit);
