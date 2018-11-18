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
/* EC IPC SERIALPORT */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include <asm/termios.h>
#include <asm/ioctls.h>

static struct file* tty_filp;
//static DECLARE_MUTEX(tty_filp_open_mutex);
static DEFINE_SEMAPHORE(tty_filp_open_mutex);

#define LOCKED_STATEMENT(mutex, statement)	\
	down(mutex);				\
	do {					\
		statement;			\
	} while(0);				\
	up(mutex);

static long ec_ipc_tty_ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
	mm_segment_t oldfs;
	long ret = -ENOTTY;

	if(IS_ERR(filp) || (filp == NULL) || (filp->f_op == NULL)) return -ENOTTY;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	if(filp->f_op->unlocked_ioctl != NULL) {
		ret = (filp->f_op->unlocked_ioctl)(filp, cmd, arg);
	}

	set_fs(oldfs);

	return ret;
}

void ec_ipc_serial_init(void)
{
	LOCKED_STATEMENT(&tty_filp_open_mutex,
			tty_filp = NULL;
		);
}

#define ec_ipc_serial_isopen() (!IS_ERR_OR_NULL(tty_filp))

int ec_ipc_serial_open(void)
{
	int ret = 0;

#define EC_SERIAL_PORT_DEVNAME "/dev/ttyHS3"
#define EC_SERIAL_PORT_BAUDRATE B115200

	LOCKED_STATEMENT(&tty_filp_open_mutex,
			if( ec_ipc_serial_isopen() ){
				ret = 0;
				break;
			}

			tty_filp = filp_open(EC_SERIAL_PORT_DEVNAME, O_RDWR, 0);
			if(IS_ERR(tty_filp)) {
				ret = PTR_ERR(tty_filp);
				break;
			}

			{
				struct termios settings;

				/* Setting serial port */
				ec_ipc_tty_ioctl(tty_filp, TCGETS, (unsigned long)&settings);
				settings.c_iflag = 0;
				settings.c_oflag = 0;
				settings.c_lflag = 0;
				settings.c_cflag =
					CS8 | CRTSCTS | CLOCAL |CREAD | EC_SERIAL_PORT_BAUDRATE;
				settings.c_cc[VMIN] = 0;
				settings.c_cc[VTIME] = 0;
				ec_ipc_tty_ioctl(tty_filp, TCSETS, (unsigned long)&settings);

				ec_ipc_tty_ioctl(tty_filp, TIOCEXCL, 0);
			}
		);

	return ret;
}
void ec_ipc_serial_close(void)
{
	LOCKED_STATEMENT(&tty_filp_open_mutex,
			if( ec_ipc_serial_isopen() ){
				filp_close(tty_filp, 0);
				tty_filp = NULL;
			}
		);
}
ssize_t ec_ipc_serial_read(uint8_t* buf, size_t size, int timeout_ms)
{
	ssize_t ret = -EFAULT;
	ssize_t readsize = 0;
	mm_segment_t oldfs;
	struct timeval start, now;
	long elapsed;

	if(buf == NULL) return -EINVAL;
	if(size == 0) return 0;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	do_gettimeofday(&start);

	while(1) {
		if(0 < timeout_ms) {
			do_gettimeofday(&now);
			elapsed = ((now.tv_sec - start.tv_sec) * 1000) + ((now.tv_usec - start.tv_usec) / 1000);
			if(timeout_ms <= elapsed) break;
		}

		ret = -EFAULT;
		LOCKED_STATEMENT(&tty_filp_open_mutex,
				if(!ec_ipc_serial_isopen() ){
					ret = -EFAULT;
					break;
				}
				if((tty_filp->f_op == NULL) || (tty_filp->f_op->read == NULL) ){
					ret = -ENOSYS;
					break;
				}

				tty_filp->f_pos = 0;
				ret = (tty_filp->f_op->read)(tty_filp, &(buf[readsize]), size - readsize, &(tty_filp->f_pos));
			);
		if(ret < 0) {
			/* read error */
			readsize = ret;
			break;
		}
		else {
			readsize += ret;
		}

		if(size <= readsize) {
			/* read done */
			break;
		}

		if(0 < schedule_timeout_interruptible(1)) {
			/* return by signal */
			break;
		}
	}

	set_fs(oldfs);

	return readsize;
}
ssize_t ec_ipc_serial_write(const uint8_t* buf, size_t size)
{
	ssize_t ret = -EFAULT;
	mm_segment_t oldfs;

	if(buf == NULL) return -EINVAL;
	if(size == 0) return 0;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	LOCKED_STATEMENT(&tty_filp_open_mutex,
			if(!ec_ipc_serial_isopen() ){
				ret = -EFAULT;
				break;
			}
			if((tty_filp->f_op == NULL) || (tty_filp->f_op->write == NULL) ){
				ret = -ENOSYS;
				break;
			}

			tty_filp->f_pos = 0;
			ret = (tty_filp->f_op->write)(tty_filp, buf, size, &(tty_filp->f_pos));
		);

	set_fs(oldfs);

	return ret;
}

