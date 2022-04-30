/*
 * linux/include/asm/arch/system.h
 *
 *  Copyright (C)2006 ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General PGublic License as published by
 * the Free Software Foundation; either version 2 of the License , or
 * ( at your option )any later version.
 *
 * This program is distributed in the hope that it will be useful , 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not , write to the Free Software
 * Foundation , Inc. , 59 Temple Place , Suite 330 , Boston , MA  02111-1307 USA
 * AUTHOR : Arif Khan <arif.khan@st.com> */

#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/arch/platform.h>
//#include <asm/arch/spearplus.h>
#include <asm/arch/spear.h>
#include <asm/arch/wdt.h>

static inline void
arch_idle (void)
{
    /*
     * This should do all the clock switching
     * and wait for interrupt tricks
     */
    cpu_do_idle ();
}

static inline void
arch_reset (char mode)
{
#if 0 /* shiraz */
    apb_wdog_t *watchdog = (apb_wdog_t *) SPEARPLUS_START_APB_WDT;

    watchdog->control = (APB_WDOG_CLEAR | APB_WDOG_ENABLE | APB_WDOG_FREEZE);
    watchdog->match = watchdog->count + 10;
#endif
}

#endif

/* End of file - system.h */
