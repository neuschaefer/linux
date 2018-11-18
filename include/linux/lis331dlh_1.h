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
#ifndef __LIS331DLH_1_H__
#define __LIS331DLH_1_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#ifdef __cplusplus
extern "C" {
#endif

#define LIS331DLH_IOC_MAGIC 'a'
#define LIS331DLH_IOC_GET_DELAY		_IOR(LIS331DLH_IOC_MAGIC, 0, __s32)
#define LIS331DLH_IOC_SET_DELAY		_IOW(LIS331DLH_IOC_MAGIC, 1, __s32)
#define LIS331DLH_IOC_GET_OFS_DATA	_IOR(LIS331DLH_IOC_MAGIC, 2, __s16[3])
#define LIS331DLH_IOC_SET_OFS_DATA	_IOW(LIS331DLH_IOC_MAGIC, 3, __s16[3])
#define LIS331DLH_IOC_GET_ACTIVE	_IOR(LIS331DLH_IOC_MAGIC, 4, __s32)
#define LIS331DLH_IOC_SET_ACTIVE	_IOW(LIS331DLH_IOC_MAGIC, 5, __s32)
#define LIS331DLH_IOC_MAXNR ( 6 )

#ifdef __KERNEL__
struct lis331dlh_platform_data {
    s8 transformation_matrix[9];
};
#endif /* __KERNEL__ */


#ifdef __cplusplus
}
#endif

#endif /* __LIS331DLH_1_H__ */

