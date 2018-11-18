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

#ifndef __BH1690_H__
#define __BH1690_H__

#include <linux/types.h>
#include <linux/ioctl.h>
#ifdef __cplusplus
extern "C" {
#endif

#define BH1690_IOC_MAGIC 'l'
#define BH1690_IOC_GET_DELAY	_IOR(BH1690_IOC_MAGIC, 0, __s32)
#define BH1690_IOC_SET_DELAY	_IOW(BH1690_IOC_MAGIC, 1, __s32)
#define BH1690_IOC_GET_ACTIVE	_IOR(BH1690_IOC_MAGIC, 2, __s32)
#define BH1690_IOC_SET_ACTIVE	_IOW(BH1690_IOC_MAGIC, 3, __s32)
#define BH1690_IOC_MAXNR ( 4 )

#ifdef __cplusplus
}
#endif

#endif /* __BH1690_H__ */

