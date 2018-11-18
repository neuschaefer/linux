/*
 * cmv59dx.h - cmv59dx driver header
 *
 * Copyright (C) 2010 Motorola, Inc.
 * Copyright (C) 2010-2012 Sony Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __CMV59DX_H__
#define __CMV59DX_H__

#include <linux/ioctl.h>  /* For IOCTL macros */

#define CMV59DX_IOCTL_SET_MODE		_IOW('o', 1, struct cmv59dx_mode)
#define CMV59DX_IOCTL_INIT_DEV  	_IOW('o', 2, __u32)
#define CMV59DX_IOCTL_SET_FPS	    _IOW('o', 3, __u8)
#define CMV59DX_IOCTL_SET_VIDEO_CHAT             _IOW('o', 4, __u32)

enum cmv59dx_test_pattern {
	TEST_PATTERN_NONE,
	TEST_PATTERN_COLORBARS,
	TEST_PATTERN_CHECKERBOARD
};

struct cmv59dx_otp_data {
	/* Only the first 5 bytes are actually used. */
	__u8 sensor_serial_num[6];
	__u8 part_num[8];
	__u8 lens_id[1];
	__u8 manufacture_id[2];
	__u8 factory_id[2];
	__u8 manufacture_date[9];
	__u8 manufacture_line[2];

	__u32 module_serial_num;
	__u8 focuser_liftoff[2];
	__u8 focuser_macro[2];
	__u8 reserved1[12];
	__u8 shutter_cal[16];
	__u8 reserved2[183];

	/* Big-endian. CRC16 over 0x00-0x41 (inclusive) */
	__u16 crc;
	__u8 reserved3[3];
	__u8 auto_load[2];
} __attribute__ ((packed));

struct cmv59dx_mode {
	int xres;
	int yres;
	__u32 frame_length;
	__u32 coarse_time;
	__u16 gain;
};
#ifdef __KERNEL__
struct cmv59dx_platform_data {
	int (*power_on)(void);
	int (*power_off)(void);

};
#endif /* __KERNEL__ */

#endif  /* __CMV59DX_H__ */

