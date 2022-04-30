/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

#ifndef SPEAR_GPIO_H
#define SPEAR_GPIO_H

#include "spr_gpio_pl061_hal.h"
#include <asm/arch/kernel_defs.h>

#define SPEAR_GPIO_BASE 'G'
#define GPIOIOC_CONFIGDEFAULT _IOWR(SPEAR_GPIO_BASE,1,t_GPIOPinInfo)
#define GPIOIOC_DIRSET _IOWR(SPEAR_GPIO_BASE,2,t_GPIOPinInfo)
#define GPIOIOC_DIRGET _IOWR(SPEAR_GPIO_BASE,3,t_GPIOPinInfo)

#if SPEAR_GPIO_DEBUG
#define GPIOIOC_INT_TESTING _IOWR(SPEAR_GPIO_BASE,4,t_IntInfo)
#endif

#define SPEAR_GPIO_PRINT(x...) printk(KERN_ERR NAME x)
//#define SPEAR_GPIO_PRINT(x...) ;

typedef struct
{
	union
	{
		t_GPIOValue PinVal;	//SET or RESET
		t_GPIODir PinDir;	//IN or OUT
	}PinInfo;

	t_GPIOId GPIOId;		//GPIO Id
	uint PinMask;			// Mask of Pin Identification
}t_GPIOPinInfo;

#if SPEAR_GPIO_DEBUG
typedef struct
{
	t_GPIOId GPIOId;
	uint PinMask;
	uint Command;
	t_GPIOIntConfig IntType;
	t_GPIOIntEdgeConfig EdgeType;
	t_GPIOIntSingleEdgeConfig SEdgeType;
	t_GPIOIntLevelConfig LevelType;
}t_IntInfo;

typedef enum
{
	Int_Enable,	
	Int_Disable,
	Int_Clear,
	Int_Bind,
	Int_Config,
	Reg_Read
}IntCommand;

#endif

/***********************************************************************************
* PUBLIC Functions
***********************************************************************************/
//This API is used to initialize the GPIO.
static int __init SPEAR_GPIO_Init(void);
static void __exit SPEAR_GPIO_Exit(void);

//This API is used to configure the GPIO to the default settings.
t_GPIOError SPEAR_GPIO_ConfigDefault(t_GPIOId GPIOId);

//This API is used to Set Direction of the GPIO pins.
t_GPIOError SPEAR_GPIO_DirSet(t_GPIOId GPIOId, t_GPIODir Dir, uint Mask);

//This API is used to Get Direction of the GPIO pins.
t_GPIOError SPEAR_GPIO_DirGet(t_GPIOId GPIOId, t_GPIODir *Dir,uint Mask);

//This API is used to Set value of the GPIO pins.
t_GPIOError SPEAR_GPIO_ValSet(t_GPIOId GPIOId, t_GPIOValue Val, uint Mask);

//This API is used to Get value of the GPIO pins.
t_GPIOError SPEAR_GPIO_ValGet(t_GPIOId GPIOId, t_GPIOValue *val, uint Mask);

//This API is used to configure Edge Interrupt on the specified GPIO Pins.
t_GPIOError SPEAR_GPIO_EdgeConfig(t_GPIOId GPIOId, t_GPIOIntEdgeConfig IntEdge, t_GPIOIntSingleEdgeConfig IntSingleEdge, uint Mask);

//This API is used to configure Level Interrupt on the specified GPIO Pins.
t_GPIOError SPEAR_GPIO_LevelConfig(t_GPIOId GPIOId, t_GPIOIntLevelConfig IntLevel, uint Mask);

//This API is used to Enable interrupts on specified GPIO Pins. 
t_GPIOError SPEAR_GPIO_IntEnable(t_GPIOId GPIOId, uint Mask);

//This API is used to Disable interrupts on specified GPIO Pins. 
t_GPIOError SPEAR_GPIO_IntDisable(t_GPIOId GPIOId, uint Mask);

//This API is used to Bind an Interrupt Handler with an pin configured as interrupt Enabled.
t_GPIOError SPEAR_GPIO_BindIntHandler(t_GPIOId GPIOId, uint Mask, t_GPIOIntHandler Handler);

 
#endif //SPEAR_GPIO_H
