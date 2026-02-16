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
 * Intel CE processor supports 4 masters and 12 mutexes available
 *
 */

#ifndef _PUMA6_HW_MUTEX_LLD_H_
#define _PUMA6_HW_MUTEX_LLD_H_


/* Identification Register */
#define CORE_ID 0x000

/* MUTEX status Register */
#define HW_MUTEX_STATUS             0x004
#define HW_MUTEX_STATUS_BIT(mutex)  BIT(mutex)

/* MUTEX wait Registers */
#define HW_MUTEX_WAIT0          0x008
#define HW_MUTEX_WAIT1          0x00c
#define HW_MUTEX_WAIT2          0x010
#define HW_MUTEX_WAIT3          0x014
#define MUTEX_WAIT_BIT(mutex)   BIT(mutex)

/* MUTEX own registers */
#define HW_MUTEX_OWN0 0x018
#define HW_MUTEX_OWN1 0x01C
#define HW_MUTEX_OWN2 0x020
#define HW_MUTEX_OWN3 0x024

/* MUTEX interrupt register */
#define HW_MUTEX_INTR                   0x028
#define HW_MUTEX_INTR_IC_BIT(master)    BIT(master)

/* MUTEX config register */
#define HW_MUTEX_CFG            0x02C
#define HW_MUTEX_CFG_IP_BIT     BIT(0)

/* MUTEX control register */
#define HW_MUTEX_CNTL0          0x030
#define HW_MUTEX_CNTL1          0x034
#define HW_MUTEX_CNTL2          0x038
#define HW_MUTEX_CNTL3          0x03C
#define HW_MUTEX_CNTL_NF_BIT    BIT(0)

/* MUTEX LOCK/UNLOCK registers */
#define HW_MUTEX0_LOCK              0x100
#define HW_MUTEX1_LOCK              0x180
#define HW_MUTEX2_LOCK              0x200
#define HW_MUTEX3_LOCK              0x280
#define HW_MUTEX_MTX_UNLOCK_BIT     BIT(0)

static const uint32_t hw_mutex_locks[MASTER_TOTAL] = {HW_MUTEX0_LOCK, HW_MUTEX1_LOCK, HW_MUTEX2_LOCK, HW_MUTEX3_LOCK};
static const uint32_t hw_mutex_waits[MASTER_TOTAL] = {HW_MUTEX_WAIT0, HW_MUTEX_WAIT1, HW_MUTEX_WAIT2, HW_MUTEX_WAIT3};
static const uint32_t hw_mutex_owns[MASTER_TOTAL] = {HW_MUTEX_OWN0, HW_MUTEX_OWN1, HW_MUTEX_OWN2, HW_MUTEX_OWN3};
static const uint32_t hw_mutex_cntls[MASTER_TOTAL] = {HW_MUTEX_CNTL0, HW_MUTEX_CNTL1, HW_MUTEX_CNTL2, HW_MUTEX_CNTL3};

/* Defined to perform little endian accesses For ARM11 - If the CPU is running in little endian mode this macro will do nothing ! */
#define HW_MUTEX_CONVERT_FROM_32LE(le_value)     (le32_to_cpu(le_value)) 
#define HW_MUTEX_CONVERT_CPU_TO_32LE(be_value)   (cpu_to_le32(be_value))

static inline uint8_t hw_mutex_read_and_test_bits(void __iomem *reg, uint32_t val)
{
    return  (((__raw_readl(reg)) & HW_MUTEX_CONVERT_CPU_TO_32LE(val)) > 0);
}

static inline void hw_mutex_read_and_set_bits(void __iomem *reg, uint32_t val)
{
    __raw_writel(__raw_readl(reg) | HW_MUTEX_CONVERT_CPU_TO_32LE(val), reg);
}

static inline void hw_mutex_read_and_clr_bits(void __iomem *reg, uint32_t val)
{
    __raw_writel((__raw_readl(reg) & ~(HW_MUTEX_CONVERT_CPU_TO_32LE(val))), reg);
}

static inline void hw_mutex_set_reg(void __iomem *reg, uint32_t val)
{
    __raw_writel(HW_MUTEX_CONVERT_CPU_TO_32LE(val), reg);
}

static inline uint32_t hw_mutex_read_reg(void __iomem *reg)
{
    unsigned int reg_val = __raw_readl(reg);

    return HW_MUTEX_CONVERT_FROM_32LE(reg_val);
}

#endif // !_PUMA6_HW_MUTEX_LLD_H_

