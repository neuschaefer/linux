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

#ifndef __AMI304_H__
#define __AMI304_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#ifdef __cplusplus
extern "C" {
#endif

#define AMI304_IOC_MAGIC 'm'
#define AMI304_IOC_DO_CALIBRATION	_IOR(AMI304_IOC_MAGIC, 0, __s16[6])
#define AMI304_IOC_GET_OFS_DATA		_IOR(AMI304_IOC_MAGIC, 1, __s16[3])
#define AMI304_IOC_SET_OFS_DATA		_IOW(AMI304_IOC_MAGIC, 2, __s16[3])
#define AMI304_IOC_GET_B0_DATA		_IOR(AMI304_IOC_MAGIC, 3, __s16[3])
#define AMI304_IOC_SET_B0_DATA		_IOW(AMI304_IOC_MAGIC, 4, __s16[3])
#define AMI304_IOC_GET_DELAY		_IOR(AMI304_IOC_MAGIC, 5, __s32)
#define AMI304_IOC_SET_DELAY		_IOW(AMI304_IOC_MAGIC, 6, __s32)
#define AMI304_IOC_GET_ACTIVE		_IOR(AMI304_IOC_MAGIC, 7, __s32)
#define AMI304_IOC_SET_ACTIVE		_IOW(AMI304_IOC_MAGIC, 8, __s32)
#define AMI304_IOC_MAXNR ( 9 )

#ifdef __KERNEL__
struct ami304_platform_data {
    s8 transformation_matrix[9];
};
#endif /* __KERNEL__ */


#ifdef __cplusplus
}
#endif

#endif /* __AMI304_H__ */

