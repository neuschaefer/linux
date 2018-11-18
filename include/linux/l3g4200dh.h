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

#ifndef __L3G4200DH_H__
#define __L3G4200DH_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#ifdef __cplusplus
extern "C" {
#endif

#define L3G4200DH_IOC_MAGIC 'g'
#define L3G4200DH_IOC_GET_DELAY	_IOR(L3G4200DH_IOC_MAGIC, 0, __s32)
#define L3G4200DH_IOC_SET_DELAY	_IOW(L3G4200DH_IOC_MAGIC, 1, __s32)
#define L3G4200DH_IOC_GET_VALUE	_IOR(L3G4200DH_IOC_MAGIC, 2, __s16[3])
#define L3G4200DH_IOC_GET_OFS_DATA	_IOR(L3G4200DH_IOC_MAGIC, 3, __s16[3])
#define L3G4200DH_IOC_SET_OFS_DATA	_IOW(L3G4200DH_IOC_MAGIC, 4, __s16[3])
#define L3G4200DH_IOC_GET_ACTIVE	_IOR(L3G4200DH_IOC_MAGIC, 5, __s32)
#define L3G4200DH_IOC_SET_ACTIVE	_IOW(L3G4200DH_IOC_MAGIC, 6, __s32)
#define L3G4200DH_IOC_MAXNR ( 7 )

#ifdef __KERNEL__
struct l3g4200dh_platform_data {
    s8 transformation_matrix[9];
};
#endif /* __KERNEL__ */


#ifdef __cplusplus
}
#endif

#endif /* __L3G4200DH_H__ */

