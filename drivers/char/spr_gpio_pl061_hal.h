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
#ifndef SPEAR_GPIO_HAL_H
#define SPEAR_GPIO_HAL_H

#include <linux/types.h>
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>

#define SPEAR_GPIO_DEBUG 0

//Registers Base Address Mapping
#define GPIODATA  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0000))
#define GPIODIR	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0400))
#define GPIOIS	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0404))
#define GPIOIBE	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0408))
#define GPIOIEV	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x040C))
#define GPIOIE	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0410))
#define GPIORIS	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0414))
#define GPIOMIS  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0418))
#define GPIOIC	  	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x041C))
#define GPIOAFSEL 	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0420))
#define GPIOPeriphID0	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FE0))
#define GPIOPeriphID1	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FE4))
#define GPIOPeriphID2	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FE8))
#define GPIOPeriphID3	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FEC))
#define GPIOPcellD0	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FF0))
#define GPIOPcellD1	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FF4))
#define GPIOPcellD2	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FF8))
#define GPIOPcellD3	((volatile uint *)(SPEAR_GPIO_APB_BASE + 0x0FFC))


/* Define the Mask for the Pins.*/
#define GPIO_PIN0	0x00000001
#define GPIO_PIN1	0x00000002
#define GPIO_PIN2	0x00000004
#define GPIO_PIN3	0x00000008
#define GPIO_PIN4	0x00000010
#define GPIO_PIN5	0x00000020
#define GPIO_PIN6	0x00000040
#define GPIO_PIN7	0x00000080
#define GPIO_ALLPINS	0x000000FF


/******************************************************************************/
/* Types definitions */
/******************************************************************************/

/* Defining new types */

typedef unsigned char uchar;

//Total Number of GPIO's
#ifdef CONFIG_ARCH_SPEARPLUS
#define GPIO_NUM 3
#define GPIO_PINS 8
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define GPIO_NUM  2
#define GPIO_PINS 8
#endif

//typedef for a pointer to a interrupt handler
typedef void (* t_GPIOIntHandler)(void);


//Define the type used to define various GPIO Id's.
typedef enum
{
	GPIO0,
	GPIO1,
#ifdef CONFIG_ARCH_SPEARPLUS
	GPIO2,
#endif
	GPIO_NONE
}t_GPIOId;

//Define the type used to describe the Mode of Operation of Pins.
typedef enum
{
    SOFTWARE = 0,
    HARDWARE
} t_GPIOMode;


/*
*  Define the type used to describe the direction of pins.
*/
typedef enum
{
    IN = 0,
    OUT
} t_GPIODir;


/*
* Define the type used to describe the values that a pin can have. 
*/
typedef enum
{
    RESET = 0,
    SET
} t_GPIOValue;


/*
* Define the type used to describe the Type of Interrupt.
*/
typedef enum
{
    EDGE = 0,
    LEVEL
} t_GPIOIntConfig;


/*
* Define the type used to describe the Edge Type of Interrupt.
*/
typedef enum
{
    SINGLE_EDGE = 0,
    BOTH_EDGE
} t_GPIOIntEdgeConfig;


/*
* Define the type used to describe the type of Single Edge Triggered Interrupt.
*/
typedef enum
{
    FALLING_EDGE = 0,
    RISING_EDGE,
    NONE
} t_GPIOIntSingleEdgeConfig;


/*
* Define the type used to describe the type of Level Triggered Interrupt.
*/
typedef enum
{
    LOW_LEVEL = 0,
    HIGH_LEVEL
} t_GPIOIntLevelConfig;


/*
* Define the type used to describe the possible errors of
* that can occurin.
*/
typedef enum
{
	GPIO_OK = 0,
	GPIO_INVALID_ID,
	GPIO_INVALID_PIN_DIRECTION,
	GPIO_INVALID_PIN_ID,
	GPIO_INVALID_PIN_VALUE,
	GPIO_INVALID_INTERRUPT_TYPE,
	GPIO_NOT_AN_OUTPUT_PIN,
	GPIO_NOT_AN_INPUT_PIN,
	GPIO_INVALID_PARAMETER,
	GPIO_PIN_NOT_AN_INTERRUPT_LINE,
	GPIO_PIN_NO_INT_HANDLER_REGISTERED, 
	GPIO_INVALID_INTERRUPT_HANDLER,
	GPIO_PIN_NOT_IN_SOFWARE_MODE,
	GPIO_UNKNOWN_ERROR = 13
}t_GPIOError;

/***********************************************************************************
* PUBLIC Functions
***********************************************************************************/
//This API is used to initialize the GPIO.
t_GPIOError GPIO_Init(void);

//This API is used to configure the GPIO to the default settings.
t_GPIOError GPIO_ConfigDefault(t_GPIOId GPIOId);

//This API is used to Get Mode the GPIO pins.
t_GPIOError GPIO_GetMode(t_GPIOId GPIOId, uchar *Mode);

//This API is used to Set Direction of the GPIO pins.
t_GPIOError GPIO_DirSet(t_GPIOId GPIOId, t_GPIODir Dir, uint Mask);

//This API is used to Get Direction of the GPIO pins.
t_GPIOError GPIO_DirGet(t_GPIOId GPIOId, t_GPIODir *Dir,uint Mask);

//This API is used to Set value of the GPIO pins.
t_GPIOError GPIO_ValSet(t_GPIOId GPIOId, t_GPIOValue Val, uint Mask);

//This API is used to Get value of the GPIO pins.
t_GPIOError GPIO_ValGet(t_GPIOId GPIOId, t_GPIOValue *val, uint Mask);

//This API is used to configure Edge Interrupt on the specified GPIO Pins.
t_GPIOError GPIO_EdgeConfig(t_GPIOId GPIOId, t_GPIOIntEdgeConfig IntEdge, t_GPIOIntSingleEdgeConfig IntSingleEdge, uint Mask);

//This API is used to configure Level Interrupt on the specified GPIO Pins.
t_GPIOError GPIO_LevelConfig(t_GPIOId GPIOId, t_GPIOIntLevelConfig IntLevel, uint Mask);

//This API is used to Enable interrupts on specified GPIO Pins. 
t_GPIOError GPIO_IntEnable(t_GPIOId GPIOId, uint Mask);

//This API is used to Disable interrupts on specified GPIO Pins. 
t_GPIOError GPIO_IntDisable(t_GPIOId GPIOId, uint Mask);

//This API is used to Clear interrupts on specified GPIO Pins. 
t_GPIOError GPIO_IntClear(t_GPIOId GPIOId, uint Mask);

//This API is used to Bind interrupt Handler with a specified GPIO Pin.
t_GPIOError GPIO_BindIntHandler(t_GPIOId GPIOId, uint Mask, t_GPIOIntHandler HandlerPtr);

//This API is called whenever a interrupt occurs on any GPIO Pin.
t_GPIOError GPIO_IntHandler(t_GPIOId GPIOId);
#endif // GPIO_H
