/*
 * linux/arch/arm/mach-mt5396/sleep.c
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

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pm.h>
#include <linux/suspend.h>


#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/wait.h>

#include <asm/io.h>
#include <asm/mach/irq.h>
#include <mach/hardware.h>
#include "pm.h"


/******************************************************************************
* Debug Message Settings
******************************************************************************/

/* Debug message event */
#define DBG_PM_NONE			0x00000000	/* No event */
#define DBG_PM_PMIC			0x00000001	/* PMIC related event */
#define DBG_PM_GPIO			0x00000002	/* GPIO related event */
#define DBG_PM_MSGS			0x00000004	/* MSG related event */
#define DBG_PM_SUSP			0x00000008	/* Suspend related event */
#define DBG_PM_ENTER			0x00000010	/* Function Entry */
#define DBG_PM_ALL			0xffffffff

#define DBG_PM_MASK	   (DBG_PM_ALL)


#if 1
u32 PM_DBG_FLAG = 0xFFFFFFFF;

#define MSG(evt, fmt, args...) \
do {	\
	if (((DBG_PM_##evt) & DBG_PM_MASK) && PM_DBG_FLAG) { \
		printk(fmt, ##args); \
	} \
} while(0)

#define MSG_FUNC_ENTRY(f)	MSG(ENTER, "<PM FUNC>: %s\n", __FUNCTION__)
#else
#define MSG(evt, fmt, args...) do{}while(0)
#define MSG_FUNC_ENTRY(f)	   do{}while(0)
#endif


/*****************************************************************************
 * REGISTER
 ****************************************************************************/


/*****************************************************************************
 * LOCAL CONST DEFINATION
 ****************************************************************************/

/******************************************************************************
 * Gloabal Variable Defination
 *****************************************************************************/

/******************************************************************************
 * Extern FUNCTION DEFINATIONS
 *****************************************************************************/
extern void mt5396_pm_RegDump(void);

/******************************************************************************
 * FUNCTION DEFINATIONS
 *****************************************************************************/
static inline u32 __bim_readl(u32 regaddr32)
{
    return __raw_readl(BIM_VIRT + regaddr32);
}

static inline void __bim_writel(u32 regval32, u32 regaddr32)
{
    __raw_writel(regval32, BIM_VIRT + regaddr32);
}

#if 0
static void setup_fake_source(void)
{
	// fake source #0 -> ice
	// fake source #1
	//disable_irq(VECTOR_RS232);
	//enable_irq(VECTOR_RS232);
	//__bim_writel((0x1<<17), REG_RW_IRQEN);
	//mt53xx_unmask_irq(VECTOR_RS232);
	// fake source #2
	//enable_irq(VECTOR_POST_PROC);
}
#endif

void mt5396_pm_SuspendEnter(void)
{
	//u32 temp = 0;
	u32 irqmask;

	irqmask = __bim_readl(REG_RW_IRQEN);
	printk("mt5396_pm_SuspendEnter WFI v7: 0x%x\n", irqmask);
	__bim_writel(0x1, REG_RW_IRQEN);
	//enable_irq(VECTOR_PDWNC);

	//setup_fake_source();

	//mt5396_pm_RegDump();
	/* enter WFI mode */
	cpu_do_idle();

	__bim_writel(irqmask, REG_RW_IRQEN);
} 

void mt5396_pm_RegDump(void)
{
} 

int _Chip_pm_begin(void)
{
	u32 u4Delay,u32_1;

	MSG_FUNC_ENTRY();	
	/* PMU Sleep mode related setting */
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("_Chip_pm_begin @@@@@@@@@@@@@@@@@@@@@@\n");
	printk(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");    
	return 0;
}

int _Chip_pm_prepare(void)
{
	MSG_FUNC_ENTRY();
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("_Chip_pm_prepare @@@@@@@@@@@@@@@@@@@@@@\n");
	printk(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    return 0;    
}

int _Chip_pm_enter(suspend_state_t state)
{
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("_Chip_pm_enter @@@@@@@@@@@@@@@@@@@@@@\n");
	printk(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

	/* ensure the debug is initialised (if enabled) */
	switch (state) 
	{
		case PM_SUSPEND_ON:
			MSG(SUSP,"_Chip_pm_enter PM_SUSPEND_ON\n\r");
			break;
		case PM_SUSPEND_STANDBY:
			MSG(SUSP,"_Chip_pm_enter PM_SUSPEND_STANDBY\n\r");        
			break;
		case PM_SUSPEND_MEM:
			MSG(SUSP,"_Chip_pm_enter PM_SUSPEND_MEM!!!!\n\r");
			mt5396_pm_SuspendEnter();    		
			break;
		case PM_SUSPEND_MAX:
			MSG(SUSP,"_Chip_pm_enter PM_SUSPEND_MAX\n\r");        
			MSG(SUSP,"Not support for PM_SUSPEND_MAX\n\r");
			break;
		default:
			MSG(SUSP,"_Chip_pm_enter Error state\n\r");
			break;
	}
	return 0;
}
void _Chip_pm_finish(void)
{
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("_Chip_pm_finish @@@@@@@@@@@@@@@@@@@@@@\n");
	printk(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	//mt6573_pm_RegDump();
}
void _Chip_pm_end(void)
{
	MSG_FUNC_ENTRY();
	printk("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk("_Chip_pm_end @@@@@@@@@@@@@@@@@@@@@@@@\n");
	printk(" @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

}
void _Chip_PM_init(void)
{

}    

