
/*
 *  kernel/hw_mutex/h
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011-2014 Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 * The file contains the main data structure and API definitions for Linux Hardware Mutex driver
 * Intel CE processor supports 4 masters and 12 mutexes avalible
 *
 */
#ifndef KERNEL_HW_MUTEX_LLD_H
#define KERNEL_HW_MUTEX_LLD_H

#include <linux/io.h>

#ifdef CONFIG_MACH_PUMA6 /* For Puma-6 SoC */
#include "puma6_hw_mutex_lld.h"
#endif

#ifdef CONFIG_MACH_PUMA7 /* For Puma-7 SoC */
#include "puma7_hw_mutex_lld.h"
#endif

#define HW_MUTEX_DEVICE 0x0949 

#define BIT(i)    ((1 << (i)))

/*
 * Configs Mutex to be in polling or FIFO/NULL scheduler mode
 *
 */
#define SET_HW_MUTEX_POLLING(pMaster) __set_hw_mutex(pMaster, MUTEX_POLLING)
#define SET_HW_MUTEX_FIFO_INTERRUPT(pMaster) __set_hw_mutex(pMaster, MUTEX_FIFO_SCHE)
#define SET_HW_MUTEX_NULL_INTERRUPT(pMaster) __set_hw_mutex(pMaster, MUTEX_NULL_SCHE)



#endif 
/* end of hw_mutex.h */


