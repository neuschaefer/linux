
/*
 *  kernel/hw_mutex/h
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2011 Intel Corporation. All rights reserved.
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
#ifndef KERNEL_HWMUTEX_H
#define KERNEL_HWMUTEX_H


/* Time out if we cannot get a MUTEX within half minute */
#define IRQ_HW_MUTEX_TIME_OUT (300*HZ)
#define HW_MUTEX_IRQ_NAME "hw_mutex_irq"



//#ifdef CONFIG_SMP
static inline void hw_mutex_set_owner(struct hw_mutex *lock)
{
	lock->owner = current_thread_info();
}

static inline void hw_mutex_clear_owner(struct hw_mutex *lock)
{
	lock->owner = NULL;
}
static inline struct thread_info* hw_mutex_get_owner(struct hw_mutex *lock)
{
	return lock->owner;
}

#endif 
/* end of hw_mutex.h */






