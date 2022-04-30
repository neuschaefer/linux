/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
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
#ifndef SPEAR_ADC_HAL_H
#define SPEAR_ADC_HAL_H

#include <linux/types.h>
#include <asm/arch/hcl_defs.h>
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>

#define SPEAR_ADC_DEBUG 0

//Base Address of ADC..
#ifdef CONFIG_ARCH_SPEARPLUS
#define SPEAR_ADC_APB_BASE VA_SPEARPLUS_START_ICM2_ADC
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define SPEAR_ADC_APB_BASE  VA_SPEARBASIC_START_ICM1_ADC 
#endif

//ADC Registers Base Address Mapping
#define ADCSTATUS 	((volatile uint *)(SPEAR_ADC_APB_BASE + 0x000))
#define ADCAVERAGE	((volatile uint *)(SPEAR_ADC_APB_BASE + 0x004))
#define ADCCLK		((volatile uint *)(SPEAR_ADC_APB_BASE + 0x00C))

#define ADC_MAX_CLK	SPEAR_APB_CLK/3000000
#define ADC_MIN_CLK	SPEAR_APB_CLK/14000000

#ifndef NULL
#define NULL 0
#endif

//Constants for configuring ADC Registers
typedef enum
{
	START_CONVERSION	= 0x00000001,
	STOP_CONVERSION 	= ~START_CONVERSION,
	ADC_ENABLE		= 0x00000010,
	ADC_DISABLE		= ~ADC_ENABLE,
	REF_VOLT_INT		= 0x00000200,
	REF_VOLT_EXT		= ~REF_VOLT_INT,

	//Masks and Shifts
	CHANNEL_SELECT_SHIFT	= 0x1,
	SAMPLE_SIZE_SHIFT	= 0x5,
	SAMPLE_SIZE_SELECT_MASK	= 0x7,
	CONVERSION_COMPLETE_MASK= 0x00000100
}t_ADCStatusReg;

typedef enum
{
	DATA_MASK		= 0x000003ff
}t_ADCAvgReg;

typedef enum
{
	CLK_HIGH_SHIFT		= 0x4
}t_ADCClkReg;

//Channel List
typedef enum
{
	CHANNEL0,
	CHANNEL1,
	CHANNEL2,
	CHANNEL3,
	CHANNEL4,
	CHANNEL5,
	CHANNEL6,
	CHANNEL7
}t_ADCChannelId;

//Sample Rates
typedef enum
{
	SAMPLE1,
	SAMPLE2,
	SAMPLE4,
	SAMPLE8,
	SAMPLE16,
	SAMPLE32,
	SAMPLE64,
	SAMPLE128
}t_ADCAvgSamples;

//Reference voltage
typedef enum
{
	EXTERNAL_VOLT,
	INTERNAL_VOLT
}t_ADCVoltRef;


//Define the type used to describe the possible errors of that can occurin.
typedef enum
{
	ADC_OK = 0,
	ADC_INVALID_CHANNEL_ID,
	ADC_INVALID_SAMPLE_SIZE,
	ADC_INVALID_VOLT_REF,
	ADC_NULL_POINTER,
	ADC_NOT_CONFIGURED,
	ADC_CONVERSION_ONGOING,
    	ADC_INVALID_CLOCK_SCALING,
	ADC_BUSY,
	ADC_ERROR
}t_ADCError;

//Configuration Structure
typedef struct
{
	t_ADCChannelId ChannelId;
	t_ADCAvgSamples AvgSamples;
	t_ADCVoltRef VoltRef;
	uint ClkHigh;
	uint ClkLow;
}t_ADCConfig;

/***********************************************************************************
 * PUBLIC Functions
***********************************************************************************/
//This API is used to Enable the ADC.
t_ADCError ADC_Enable(void);

//This API is used to configure ADC.
t_ADCError ADC_Configure(t_ADCConfig Config);

//This API is used to convert Analog Data to Digital Data.
t_ADCError ADC_StartConversion(void);

//This API is used to get the converted data.
t_ADCError ADC_GetConvertedData(uint *DigitalVolt);

//This API is used to Disable the ADC.
t_ADCError ADC_Disable(void);

#endif // SPEAR_ADC_HAL_H
