
/*
 *  kernel/hw_mutex/h
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014 Intel Corporation. All rights reserved.
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
 * Intel CE processor supports 16 masters and 16 mutexes available
 *
 */

#ifndef _PUMA7_HW_MUTEX_LLD_H_
#define _PUMA7_HW_MUTEX_LLD_H_


/* Identification Register */
#define CORE_ID 0x000

/* MUTEX status Register */
#define HW_MUTEX_STATUS             0x004
#define HW_MUTEX_STATUS_BIT(mutex)  BIT(mutex)

/* MUTEX wait Registers */
#define HW_MUTEX_WAIT(master)   (0x0080 + ((master) << 2)) /* 0x0080 + (master * 0x4) */
#define MUTEX_WAIT_BIT(mutex)   BIT(mutex)

/* MUTEX own registers */
#define HW_MUTEX_OWN(master)    (0x0100 + ((master) << 2)) /* 0x0100 + (master * 0x4) */

/* MUTEX interrupt register */
#define HW_MUTEX_INTR                   0x028 
#define HW_MUTEX_INTR_IC_BIT(master)    BIT(master)

/* MUTEX config register */
#define HW_MUTEX_CFG            0x02C 
#define HW_MUTEX_CFG_IP_BIT     BIT(0)

/* MUTEX control register */
#define HW_MUTEX_CNTL(master)   (0x0180 + ((master) << 2)) /* 0x0180 + (master * 0x4) */
#define HW_MUTEX_CNTL_NF_BIT    BIT(0)

/* MUTEX LOCK/UNLOCK registers */
#define HW_MUTEX_LOCK(master, mutex)    (0x1000 + ((master) << 7) + ((mutex) << 2)) /* 0x1000 + (master * 0x80) + (mutex * 0x4) */
#define HW_MUTEX_MTX_UNLOCK_BIT         BIT(0)

/* hw_mutex_locks array specifies the base address for all lock registers for each master [index is Master ID] */
static const uint32_t hw_mutex_locks[MASTER_TOTAL] = { HW_MUTEX_LOCK(0, 0), HW_MUTEX_LOCK(1, 0), HW_MUTEX_LOCK(2, 0), HW_MUTEX_LOCK(3, 0),
                                                       HW_MUTEX_LOCK(4, 0), HW_MUTEX_LOCK(5, 0), HW_MUTEX_LOCK(6, 0), HW_MUTEX_LOCK(7, 0),
                                                       HW_MUTEX_LOCK(8, 0), HW_MUTEX_LOCK(9, 0), HW_MUTEX_LOCK(10, 0), HW_MUTEX_LOCK(11, 0),
                                                       HW_MUTEX_LOCK(12, 0), HW_MUTEX_LOCK(13, 0), HW_MUTEX_LOCK(14, 0), HW_MUTEX_LOCK(15, 0) };

static const uint32_t hw_mutex_waits[MASTER_TOTAL] = { HW_MUTEX_WAIT(0), HW_MUTEX_WAIT(1), HW_MUTEX_WAIT(2), HW_MUTEX_WAIT(3),
                                                       HW_MUTEX_WAIT(4), HW_MUTEX_WAIT(5), HW_MUTEX_WAIT(6), HW_MUTEX_WAIT(7),
                                                       HW_MUTEX_WAIT(8), HW_MUTEX_WAIT(9), HW_MUTEX_WAIT(10), HW_MUTEX_WAIT(11),
                                                       HW_MUTEX_WAIT(12), HW_MUTEX_WAIT(13), HW_MUTEX_WAIT(14), HW_MUTEX_WAIT(15) };

static const uint32_t hw_mutex_owns[MASTER_TOTAL] = { HW_MUTEX_OWN(0), HW_MUTEX_OWN(1), HW_MUTEX_OWN(2), HW_MUTEX_OWN(3),
                                                      HW_MUTEX_OWN(4), HW_MUTEX_OWN(5), HW_MUTEX_OWN(6), HW_MUTEX_OWN(7),
                                                      HW_MUTEX_OWN(8), HW_MUTEX_OWN(9), HW_MUTEX_OWN(10), HW_MUTEX_OWN(11),
                                                      HW_MUTEX_OWN(12), HW_MUTEX_OWN(13), HW_MUTEX_OWN(14), HW_MUTEX_OWN(15) };

static const uint32_t hw_mutex_cntls[MASTER_TOTAL] = { HW_MUTEX_CNTL(0), HW_MUTEX_CNTL(1), HW_MUTEX_CNTL(2), HW_MUTEX_CNTL(3),
                                                       HW_MUTEX_CNTL(4), HW_MUTEX_CNTL(5), HW_MUTEX_CNTL(6), HW_MUTEX_CNTL(7),
                                                       HW_MUTEX_CNTL(8), HW_MUTEX_CNTL(9), HW_MUTEX_CNTL(10), HW_MUTEX_CNTL(11),
                                                       HW_MUTEX_CNTL(12), HW_MUTEX_CNTL(13), HW_MUTEX_CNTL(14), HW_MUTEX_CNTL(15) };

static inline uint8_t hw_mutex_read_and_test_bits(void __iomem *reg, uint32_t val)
{
    return  (((__raw_readl(reg)) & (val)) > 0);
}

static inline void hw_mutex_read_and_set_bits(void __iomem *reg, uint32_t val)
{
    __raw_writel(__raw_readl(reg) | (val), reg);
}

static inline void hw_mutex_read_and_clr_bits(void __iomem *reg, uint32_t val)
{
    __raw_writel((__raw_readl(reg) & ~(val)), reg);
}

static inline void hw_mutex_set_reg(void __iomem *reg, uint32_t val)
{
    __raw_writel(val, reg);
}

static inline uint32_t hw_mutex_read_reg(void __iomem *reg)
{
    unsigned int reg_val = __raw_readl(reg);

    return (reg_val);
}

#endif // !_PUMA7_HW_MUTEX_LLD_H_

