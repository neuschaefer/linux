/*
 * ius006.h - ius006 driver header
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

#ifndef __IUS006_H__
#define __IUS006_H__

#include <linux/ioctl.h>  /* For IOCTL macros */

#define IUS006_IOCTL_SET_RESOLUTION     _IOW('o', 1, struct ius006_resolution)
#define IUS006_IOCTL_SET_MODE           _IOW('o', 2, struct ius006_mode)
#define IUS006_IOCTL_INIT_DEV           _IOW('o', 3, __u32)
#define IUS006_IOCTL_SET_FPS            _IOW('o', 4, __u8)
#define IUS006_IOCTL_SET_AWB            _IOW('o', 5, __u32)
#define IUS006_IOCTL_SET_EV             _IOW('o', 6, __u32)
#define IUS006_IOCTL_SET_SCENE          _IOW('o', 7, __u32)
#define IUS006_IOCTL_SET_AF             _IOW('o', 8, __u32)
#define IUS006_IOCTL_AF_STATUS          _IOR('o', 9, __u32)

#define IUS006_IOCTL_SET_PREVIEW_RESOLUTION     _IOW('o', 10, struct ius006_resolution)
#define IUS006_IOCTL_SET_PICTURE_RESOLUTION     _IOW('o', 11, struct ius006_resolution)
#define IUS006_IOCTL_SET_VIDEO_RESOLUTION       _IOW('o', 12, struct ius006_resolution)
#define IUS006_IOCTL_SET_VIDEO_MODE             _IOW('o', 13, __u32)

#define IUS006_IOCTL_CANCEL_AUTOFOCUS           _IOW('o', 14, __u32)
#define IUS006_IOCTL_SET_VIDEO_CHAT             _IOW('o', 15, __u32)

#define MODE_MONITORING     0
#define MODE_HALFRELEASE    1
#define MODE_CAPTURE        2
#define MODE_MOVIE          3

struct ius006_otp_data {
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

struct ius006_resolution {
    int xres;
    int yres;
};
struct ius006_mode {
    int current_mode;
    int next_mode;
};
#ifdef __KERNEL__
struct ius006_platform_data {
    int (*power_on)(void);
    int (*power_off)(void);

};
#endif /* __KERNEL__ */

#endif  /* __IUS006_H__ */

