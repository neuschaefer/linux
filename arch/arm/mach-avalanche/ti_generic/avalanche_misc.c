/*
 *
 * avalanche_misc.c
 * Description:
 * miscellaneous APIs (like clk helpers) implementation
 *
 *
 * Copyright (C) 2009, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
#include <asm-arm/arch-avalanche/generic/pal.h>
#include <linux/module.h>
#include <linux/spinlock.h>

#if PUMA5_SOC_TYPE
unsigned int avalanche_vbus_freq;

void avalanche_set_vbus_freq(unsigned int new_vbus_freq)
{
    avalanche_vbus_freq = new_vbus_freq;
}

unsigned int avalanche_get_vbus_freq(void)
{
    return(avalanche_vbus_freq);
}


int avalanche_is_cpmac_on_vbus(void)
{
    if(0x00b != (*((volatile unsigned int*)AVALANCHE_CVR) & 0xffff))
        return (1); /* The SoC which does not have cpmac on Vbus is Apex */
    else
        return (0);
}
#endif

unsigned int avalanche_get_chip_version_info(void)
{
#if PUMA5_SOC_TYPE
    return(*(volatile unsigned int*)AVALANCHE_CVR);
#else
    return (PAL_sysBootCfgCtrl_ReadReg(BOOTCFG_DOCSIS_IP_REV)); /* In Puma6 it is via BootCfg ! */
#endif
}

/*************************************************************************************/
/*                               Common-SoC                                          */
/*                          EXPORT_SYMBOL section                                    */
/*                                                                                   */
/*************************************************************************************/

/* Reset API */
EXPORT_SYMBOL(PAL_sysResetCtrl);
EXPORT_SYMBOL(PAL_sysGetResetStatus);


/* Clock API */
EXPORT_SYMBOL(PAL_sysClkcSetFreq);
EXPORT_SYMBOL(PAL_sysClkcGetFreq);

/* Cache API */
EXPORT_SYMBOL(PAL_sysCacheInvalidate);
EXPORT_SYMBOL(PAL_sysCacheFlush);
EXPORT_SYMBOL(PAL_sysCacheFlushAndInvalidate);

/* Wakeup API */
/* EXPORT_SYMBOL(PAL_sysWakeupCtrl); */

/* cppi4.1 initialization */
extern int avalanche_cppi_init(void);
arch_initcall(avalanche_cppi_init);

/* cppi4.1 APIs */
EXPORT_SYMBOL(PAL_cppi4Init);
#if PUMA7_OR_NEWER_SOC_TYPE
EXPORT_SYMBOL(PAL_cppi4IsDescRegionAllocated);
EXPORT_SYMBOL(PAL_cppi4AllocDescRegion);
EXPORT_SYMBOL(PAL_cppi4GetDescRegionMemInfo);
EXPORT_SYMBOL(PAL_cppi4GetOffChipAddr);
EXPORT_SYMBOL(PAL_cppi4GetOffChipBpoolAddr);
#else
EXPORT_SYMBOL(PAL_cppi4AllocDesc);
#endif
EXPORT_SYMBOL(PAL_cppi4DeallocDesc);
EXPORT_SYMBOL(PAL_cppi4BufPoolInit);
EXPORT_SYMBOL(PAL_cppi4BufPoolDirectInit);
EXPORT_SYMBOL(PAL_cppi4BufPoolSetBuffers);
EXPORT_SYMBOL(PAL_cppi4BufIncRefCnt);
EXPORT_SYMBOL(PAL_cppi4BufDecRefCnt);
EXPORT_SYMBOL(PAL_cppi4BufPopBuf);
EXPORT_SYMBOL(PAL_cppi4BufPoolDestroy);
EXPORT_SYMBOL(PAL_cppi4Control);
EXPORT_SYMBOL(PAL_cppi4Exit);
EXPORT_SYMBOL(PAL_cppi4TxChOpen);
EXPORT_SYMBOL(PAL_cppi4RxChOpen);
EXPORT_SYMBOL(PAL_cppi4EnableTxChannel);
EXPORT_SYMBOL(PAL_cppi4EnableRxChannel);
EXPORT_SYMBOL(PAL_cppi4TxChStatus);
EXPORT_SYMBOL(PAL_cppi4TxChClose);
EXPORT_SYMBOL(PAL_cppi4RxChStatus);
EXPORT_SYMBOL(PAL_cppi4RxChClose);
EXPORT_SYMBOL(PAL_cppi4DisableTxChannel);
EXPORT_SYMBOL(PAL_cppi4DisableRxChannel);
EXPORT_SYMBOL(PAL_cppi4QueueOpen);
EXPORT_SYMBOL(PAL_cppi4QueueOpenNoReset);
EXPORT_SYMBOL(PAL_cppi4QueueClose);
EXPORT_SYMBOL(PAL_cppi4QueuePush);
EXPORT_SYMBOL(PAL_cppi4QueuePop);
EXPORT_SYMBOL(PAL_cppi4QueueGetEntryCount);
EXPORT_SYMBOL(PAL_cppi4GetTdInfo);
EXPORT_SYMBOL(PAL_cppi4RxChDestroy);
EXPORT_SYMBOL(PAL_cppi4TxChDestroy);
EXPORT_SYMBOL(PAL_cppi4AccChGetNextList);
EXPORT_SYMBOL(PAL_cppi4AccChOpen);
EXPORT_SYMBOL(PAL_cppi4AccChEnRateLimit);
#if PUMA7_OR_NEWER_SOC_TYPE
EXPORT_SYMBOL(PAL_cppi4AccChOpenAppCpu);
#endif
EXPORT_SYMBOL(PAL_cppi4AccChClose);
EXPORT_SYMBOL(PAL_cppi4EmbDescRecycle);

EXPORT_SYMBOL(PAL_cppi4dbgCfg_fops);
EXPORT_SYMBOL(PAL_cppi4dbg_fops);

EXPORT_SYMBOL(PAL_cppi4PM_fops);

/* WD */
EXPORT_SYMBOL(PAL_sysWdtimerInit);
EXPORT_SYMBOL(PAL_sysWdtimerSetPeriod);
EXPORT_SYMBOL(PAL_sysWdtimerCtrl);
EXPORT_SYMBOL(PAL_sysWdtimerKick);


/* Interrupt Distributor */
EXPORT_SYMBOL(avalanche_intd_enable_interrupt);
EXPORT_SYMBOL(avalanche_intd_disable_interrupt);
EXPORT_SYMBOL(avalanche_intd_set_interrupt_status);
EXPORT_SYMBOL(avalanche_intd_clear_interrupt_status);
EXPORT_SYMBOL(avalanche_intd_is_status_set);
EXPORT_SYMBOL(avalanche_intd_is_status_cleared);
EXPORT_SYMBOL(avalanche_intd_read_eoi);
EXPORT_SYMBOL(avalanche_intd_write_eoi);
EXPORT_SYMBOL(avalanche_intd_get_interrupt_count);
EXPORT_SYMBOL(avalanche_intd_set_interrupt_count);

#if defined(CONFIG_ARM_AVALANCHE_TIMER16)
EXPORT_SYMBOL(PAL_sysTimer16SetParams);
EXPORT_SYMBOL(PAL_sysTimer16GetFreqRange);
EXPORT_SYMBOL(PAL_sysTimer16Ctrl);
#endif

#if PUMA5_SOC_TYPE
/*************************************************************************************/
/*                                 PUMA-5                                            */
/*                          EXPORT_SYMBOL section                                    */
/*                                                                                   */
/*************************************************************************************/

/* Misc API */
EXPORT_SYMBOL(PAL_sysGetChipVersionInfo);
EXPORT_SYMBOL(avalanche_get_cpu_type);
EXPORT_SYMBOL(avalanche_get_cpu_name);

EXPORT_SYMBOL(PAL_sysProbeAndPrep);
EXPORT_SYMBOL(avalanche_set_mdix_on_chip);
EXPORT_SYMBOL(avalanche_is_mdix_on_chip);
EXPORT_SYMBOL(avalanche_set_vbus_freq);
EXPORT_SYMBOL(avalanche_get_vbus_freq);
EXPORT_SYMBOL(avalanche_is_cpmac_on_vbus);

/* avalanche I/O power control */
EXPORT_SYMBOL(avalanche_setIOPowerMode);
EXPORT_SYMBOL(avalanche_getIOPowerMode);

/* Power API */
EXPORT_SYMBOL(PAL_sysPowerCtrl);
EXPORT_SYMBOL(PAL_sysPscSetModuleState);
EXPORT_SYMBOL(PAL_sysSystemReset);

#endif /* PUMA5_SOC_TYPE */

