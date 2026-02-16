/*
 *  include/linux/hw_mutex.h
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or 
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2011-2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2011-2015 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _LINUX_HW_MUTEX_H_
#define _LINUX_HW_MUTEX_H_

#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <asm/atomic.h>

#ifdef CONFIG_MACH_PUMA6 /* For Puma-6 SoC */
/* 4 masters */
#define MASTER_TOTAL        (4)
#endif
#ifdef CONFIG_MACH_PUMA7 /* For Puma-7 SoC */
/* 16 masters */
#define MASTER_TOTAL        (16)
#endif

/* HW MUTEX controller working mode */
typedef enum {
	MUTEX_POLLING = 0,
	MUTEX_FIFO_SCHE,
	MUTEX_NULL_SCHE
} hw_mutex_mode_type;

#ifdef CONFIG_MACH_PUMA6
/* Totally 4 masters are supported */
typedef enum {
    MASTER_ARM11 = 0,
    MASTER_PP    = 1,
    MASTER_ATOM  = 2,
    MASTER_RESV
} hw_mutex_master_type;
#endif
#ifdef CONFIG_MACH_PUMA7
/* Totally 16 masters are supported */
typedef enum {
    MASTER_ARM11   = 0,
    MASTER_BBU_C   = 1,
    MASTER_ATOM    = 2,
    MASTER_ARM9    = 3,
    MASTER_PHY_CPU = 4,
    MASTER_CSE     = 5,
    MASTER_SEC     = 6,
    MASTER_POLL0   = 7,  /* use for polling */
    MASTER_POLL1   = 8,  /* use for polling */
    MASTER_MOCA_C  = 9,
    MASTER_RCE     = 10,
    MASTER_RESV,
    MASTER_RESV1,
    MASTER_RESV2,
    MASTER_RESV3,
    MASTER_RESV4
} hw_mutex_master_type;
#endif

#ifdef CONFIG_MACH_PUMA6
/* Devices use HW mutex */
typedef enum {
    HW_MUTEX_NOR_SPI   = 0,
    HW_MUTEX_EMMC      = 1,
    HW_MUTEX_ARM_MBX   = 2,  /* Mutex for protect the ARM Mbox Structure */
    HW_MUTEX_ATOM_MBX  = 3,  /* Mutex for protect the ATOM Mbox Structure */
    HW_MUTEX_GPIO      = 4,  /* Mutex for protect the GPIOs access */
    HW_MUTEX_MAX_VAL
} hw_mutex_device_type;
#endif

#ifdef CONFIG_MACH_PUMA7
/* Devices use HW mutex */
typedef enum {
    HW_MUTEX_NOR_SPI   = 0,
    HW_MUTEX_EMMC      = 1,
    HW_MUTEX_ARM_MBX   = 2,  /* Mutex for protect the ARM Mbox Structure */
    HW_MUTEX_ATOM_MBX  = 3,  /* Mutex for protect the ATOM Mbox Structure */
    HW_MUTEX_GPIO      = 4,  /* Mutex for protect the GPIOs access */
    HW_MUTEX_BOOT_CFG  = 5,  /* Mutex for protect the BootCfg access (The MOCA CPU can also access the SPI Mux reg) */
    /* In Puma7 we have up to 16 Hw-Mutex */
    HW_MUTEX_MAX_VAL
} hw_mutex_device_type;
#endif

#define HW_MUTEX_TOTAL        (HW_MUTEX_MAX_VAL)

/* Each HW mutex is controlled by a software mutex */
struct hw_mutex {
		hw_mutex_device_type lock_name;
		spinlock_t irq_lock;
		struct mutex lock;
		struct thread_info	*owner; /* Which thread owns the MUTEX */
		atomic_t	status;			/* 1: unlocked, 0: requesting, negative: locked, possible waiters */
		
#define	HW_MUTEX_LOCKED  	(-1)	/* The MUTEX is locked 							*/
#define	HW_MUTEX_REQUESTING (0)		/* We've requested for the MUTEX, but not get it yet 	*/
#define	HW_MUTEX_UNLOCKED  	(1)		/* The HW mutex is free 							*/

}__attribute__((aligned(4)));

struct hw_master {
	hw_mutex_master_type master;
	hw_mutex_mode_type mode; 		/* polling , fifo_interrupt, null_interrupt */
	uint32_t irq_num;				/* The irq number used for HW mutex */
	void __iomem *reg_base;			/* Mapped io reg base address */
	struct pci_dev *dev;	
	struct hw_mutex hw_mutexes[HW_MUTEX_TOTAL];
	struct hw_mutex_operations *ops;	
}__attribute__((aligned(4)));

/* Abstraction oprations of a HW mutex */
struct hw_mutex_operations {
	char *name;
	void (*clr_intr)(struct hw_master* pmaster);	
	int (*lock)(struct hw_mutex* hmutex, int force);
	void (*unlock)(struct hw_mutex* hmutex);
	uint8_t (*is_locked)(struct hw_mutex* hmutex);
	uint8_t (*is_waiting)(struct hw_mutex* hmutex);	
}__attribute__((aligned(4)));

 
/*
  * hw_mutex_lock - acquire the mutex
  * @mutex: the mutex to be acquired
  *
  * Lock the mutex exclusively for this task. If the mutex is not
  * available right now, it will sleep until we can get it.
  *
  * The function is non interruptible
  */

extern void hw_mutex_lock(uint8_t mutex);

/*
 * hw_mutex_lock_interruptible - acquire the mutex
 * @mutex: the mutex to be acquired
 *
 * Lock the mutex exclusively for this task. If the mutex is not
 * available right now, it will sleep until it can get it.
 * It can be interruptibed by signal, or exit when timeout
 *
 * Returns 0 if success, negative if interrupted or timeout
 */

extern long __must_check hw_mutex_lock_interruptible(uint8_t mutex);


/* hw_mutex_is_locked - check whether the current master owns the mutex or not
 * @mutex: the mutex number to be checked
 *
 * Return 1, if current master owns the mutex
 * Return 0, if not
 * Return Negative for errors
*/

extern int __must_check hw_mutex_is_locked(uint8_t mutex);

/*
 * hw_mutex_unlock - release the mutex
 * @mutex: the mutex to be released
 *
 * release the mutex got previously
 *
 */
extern void hw_mutex_unlock(uint8_t mutex);


//#define HW_MUTEX_DEBUG 1
#ifdef HW_MUTEX_DEBUG
//#   define DEBUG_PRINT(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#define DEBUG_PRINT printk("\nLine %d.%s: ->\n",__LINE__,__FUNCTION__)
#define DEBUG_PRINTK(fmt, args...) printk("%d@%s " fmt,__LINE__,__FUNCTION__ , ## args)

#else
#define DEBUG_PRINT do{} while (0)
#define DEBUG_PRINTK(fmt, args...) do{} while (0)

#endif

/*
 * try_lock_emmc_access
 * try lock emmc sw mutex 
 * Return 1 for success 
 */
extern int try_lock_emmc_access(void);

/*
 * lock_emmc_access
 * lock emmc access sw mutex
 */
extern void lock_emmc_access(void);

/*
 * unlock_emmc_access
 * unlock emmc access sw mutex
 */
extern void unlock_emmc_access(void);

#endif // !_LINUX_HW_MUTEX_H_

