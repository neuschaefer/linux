/*
 ***************************************************************************************************
 *                                               LINUX-KERNEL
 *                                   ReuuiMlla Linux Platform Develop Kits
 *                                              Kernel Module
 *
 *                                  (c) Copyright 2006-2011, pannan China
 *                                           All Rights Reserved
 *
 * File    : pmu_dvfs.h
 * By      : pannan
 * Version : v1.0
 * Date    : 2011-09-05
 * Descript: 
 * Update  : date                auther      ver     notes
 ***************************************************************************************************
*/

#ifndef __PMU_DVFS_H__
#define __PMU_DVFS_H__

#include <linux/init.h>
#include <linux/types.h>
#include <mach/platform.h>


#if (0)
    #define DVFS_DBG(format,args...)   printk("[pmu_dvfs] DBG:"format,##args)
#else
    #define DVFS_DBG(format,args...)   do{}while(0)
#endif

#define DVFS_ERR(format,args...)   printk("[pmu_dvfs] ERR:"format,##args)

#define CCU_PLL6            (SW_VA_CCM_IO_BASE + 0x28)  /* speed factor dectector start when set bit31 to 1 */
#define CCU_CPU_AHB_APB     (SW_VA_CCM_IO_BASE + 0x54)  /* select cpu clock source */

extern int __init pmu_dvfs_hw_init(void);
extern int pmu_dvfs_volt_chg_err(void);
extern int pmu_dvfs_clk_swt_err(void);
extern void pmu_dvfs_start_to_finish(void);
extern void pmu_dvfs_clear_pending(void);
extern int pmu_dvfs_suspend(void);
extern int pmu_dvfs_resume(void);

#endif /* __PMU_DVFS_H__ */