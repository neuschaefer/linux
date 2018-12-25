#ifndef _PM_CONFIG_H
#define _PM_CONFIG_H

/*
 * Copyright (c) 2011-2015 yanggq.young@newbietech.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
 #ifdef CONFIG_ARCH_SUN5I
 #undef CONFIG_ARCH_SUN5I
 #endif
 
 #define CONFIG_ARCH_SUN5I
 #define ENABLE_SUPER_STANDBY
 #define CHECK_IC_VERSION
 
 //#define RETURN_FROM_RESUME0_WITH_MMU    //suspend: 0xf000, resume0: 0xc010, resume1: 0xf000
//#define RETURN_FROM_RESUME0_WITH_NOMMU // suspend: 0x0000, resume0: 0x4010, resume1: 0x0000
//#define DIRECT_RETURN_FROM_SUSPEND //not support yet
//#define ENTER_SUPER_STANDBY    //suspend: 0xf000, resume0: 0x4010, resume1: 0x0000
#define ENTER_SUPER_STANDBY_WITH_NOMMU //not support yet, suspend: 0x0000, resume0: 0x4010, resume1: 0x0000
//#define WATCH_DOG_RESET

/**start address for function run in sram*/
#define SRAM_FUNC_START     SW_VA_SRAM_BASE
#define SRAM_FUNC_START_PA (0x00000000)

#define DRAM_BASE_ADDR      0xc0000000
#define DRAM_BASE_ADDR_PA      0x40000000
#define DRAM_TRANING_SIZE   (16)

#define DRAM_BACKUP_BASE_ADDR (0xc0100000) //1Mbytes offset
#define DRAM_BACKUP_BASE_ADDR_PA (0x40100000) //1Mbytes offset
#define DRAM_BACKUP_SIZE (0x4000) //16K*4 bytes = 64K.

#define DRAM_BACKUP_BASE_ADDR1 (0xc0110000)
#define DRAM_BACKUP_BASE_ADDR1_PA (0x40110000)
#define DRAM_BACKUP_SIZE1 (0x0100) // 2^8 * 4 = 1K bytes.

#define DRAM_BACKUP_BASE_ADDR2 (0xc0110400)
#define DRAM_BACKUP_BASE_ADDR2_PA (0x40110400)
#define DRAM_BACKUP_SIZE2 (0x0100) // 2^8 * 4 = 1K bytes. 

#define RUNTIME_CONTEXT_SIZE (14 * sizeof(__u32)) //r0-r13

#define DRAM_COMPARE_DATA_ADDR (0xc0100000) //1Mbytes offset
#define DRAM_COMPARE_SIZE (0x10000) //?


//for mem mapping
#define MEM_SW_VA_SRAM_BASE (0x00000000)
#define MEM_SW_PA_SRAM_BASE (0x00000000)

#define AXP_WAKEUP_KEY          	(1<<0)
#define AXP_WAKEUP_LOWBATT      	(1<<1)
#define AXP_WAKEUP_USB          	(1<<2)
#define AXP_WAKEUP_AC           	(1<<3)
#define AXP_WAKEUP_ASCEND       	(1<<4)
#define AXP_WAKEUP_DESCEND      	(1<<5)
#define AXP_WAKEUP_SHORT_KEY    	(1<<6)
#define AXP_WAKEUP_LONG_KEY     	(1<<7)
#define AXP_WAKEUP_GPIO0_FALLING    (1<<8)
#define AXP_WAKEUP_GPIO0_RAISING    (1<<9)
#define AXP_WAKEUP_GPIO1_FALLING    (1<<10)
#define AXP_WAKEUP_GPIO1_RAISING    (1<<11)
#define AXP_WAKEUP_GPIO2_FALLING    (1<<12)
#define AXP_WAKEUP_GPIO2_RAISING    (1<<13)
#define AXP_WAKEUP_GPIO3_FALLING    (1<<14)
#define AXP_WAKEUP_GPIO3_RAISING    (1<<15)

#define AXP_MEM_WAKEUP              (AXP_WAKEUP_LOWBATT | AXP_WAKEUP_USB | AXP_WAKEUP_AC | AXP_WAKEUP_DESCEND | AXP_WAKEUP_ASCEND | AXP_WAKEUP_GPIO2_FALLING)

#define AXP_BOOTFAST_WAKEUP         (AXP_WAKEUP_LOWBATT | AXP_WAKEUP_LONG_KEY)

#endif /*_PM_CONFIG_H*/
