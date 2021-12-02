/*
 * linux/arch/arm/mach-mt5396/pm.h
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */

#ifndef _MTKPM_H
#define _MTKPM_H


#include <linux/suspend.h>

int _Chip_pm_begin(void);
int _Chip_pm_prepare(void);
int _Chip_pm_enter(suspend_state_t state);
void _Chip_pm_end(void);
void _Chip_pm_finish(void);
void _Chip_PM_init(void);
void mtk_pm_init(void);

#endif

