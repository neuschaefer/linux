/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 On chip PWM driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#include <asm/ioctl.h>
#include <asm/arch/map.h>

#if !defined (AESSPWMDRV_H)
#define AESSPWMDRV_H

#ifdef AESSPWMDRV_C

/* type define */
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

/* TBD: For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1

/* ioctl definitions */
#define AESS_PWMDRV_IOC_MAGIC           0xBE

#define AESS_PWM_CONFIG_INIT             _IOWR(AESS_PWMDRV_IOC_MAGIC, 0, sPWMDevConfig)
#define AESS_PWM_CONFIG_SET             _IOWR(AESS_PWMDRV_IOC_MAGIC, 1, sPWMDevConfig)
#define AESS_PWM_CONFIG_INFO             _IOWR(AESS_PWMDRV_IOC_MAGIC, 2, sPWMDevConfig)
#define AESS_PWM_CONFIG_DEBUG             _IOWR(AESS_PWMDRV_IOC_MAGIC, 3, sPWMDevConfig)

/* init flag */
#define AESSPWM_NOT_INIT 0
#define AESSPWM_INIT_OK  1

/* PWM ABP clock */
#define WPCM450_ABP_CLOCK			55000 //55kHz
/* PDID */
#define Z2_VERSION_NUM       0x03926450

/* PWM port base address */
#define WPCM450_BASE_CTRL_REG    WPCM450_VA_PWM
#define WPCM450_GLOBAL_CTRL_REG    WPCM450_VA_GCR

#define GLOBAL_REG_PDID_REG          (WPCM450_GLOBAL_CTRL_REG+0x0)
#define GLOBAL_REG_PIN_SELECT2_ADDR  (WPCM450_GLOBAL_CTRL_REG + 0x10)

#define PWM_REG_PRESCALE_ADDR    			(WPCM450_BASE_CTRL_REG + 0)
#define PWM_REG_CLOCK_SELECTOR_ADDR   (WPCM450_BASE_CTRL_REG + 0x4)
#define PWM_REG_CONTROL_ADDR          (WPCM450_BASE_CTRL_REG + 0x8)
#define PWM_REG_COUNTER_ADDR(PORT)    (WPCM450_BASE_CTRL_REG + 0xc + (12 * PORT))
#define PWM_REG_COMPARATOR_ADDR(PORT) (WPCM450_BASE_CTRL_REG + 0x10 + (12 * PORT))
#define PWM_REG_DATA_ADDR(PORT)    		(WPCM450_BASE_CTRL_REG + 0x14 + (12 * PORT))
#define PWM_REG_TIMER_INT_ENABLE_ADDR (WPCM450_BASE_CTRL_REG + 0x3c)
#define PWM_REG_TIMER_INT_IDENTIFICATION_ADDR  (WPCM450_BASE_CTRL_REG + 0x40)

#define PWM_CTRL_CH0_MODE_BIT 3
#define PWM_CTRL_CH1_MODE_BIT 11
#define PWM_CTRL_CH2_MODE_BIT 15
#define PWM_CTRL_CH3_MODE_BIT 19

#define PWM_CTRL_CH0_ENABLE_BIT 0
#define PWM_CTRL_CH1_ENABLE_BIT 8
#define PWM_CTRL_CH2_ENABLE_BIT 12
#define PWM_CTRL_CH3_ENABLE_BIT 16

#define PWM_CLOCK_SELECTOR_MASK 0x7
#define PWM_CLOCK_CH0_SELECTOR_BIT 0
#define PWM_CLOCK_CH1_SELECTOR_BIT 4
#define PWM_CLOCK_CH2_SELECTOR_BIT 8
#define PWM_CLOCK_CH3_SELECTOR_BIT 12

#define PWM_PRESCALE_MASK 0xff
#define PWM_PRESCALE_CH01_BIT 0
#define PWM_PRESCALE_CH23_BIT 8

#define PWM_PIN_SELECT_CH0_BIT 16
#define PWM_PIN_SELECT_CH0_GPIO_NUM 80
#define PWM_PIN_ENABLE_VALUE 1

/* GPIO command type */
#define GPIO_CONFIG    0 
#define GPIO_WRITE     1
#define GPIO_READ      2

#define GPIO_SELECTED_OUTPUT     0x9

/* Define the maximum PWM channel number */
#define PWM_MAX_CHN_NUM  8
#define PWM_MAX_CHN_NUM_IN_A_MODULE  4

/* Define the Counter Register, value = 100 for match 100% */
#define PWM_COUNTER_DEFALUT0_NUM  63
#define PWM_COUNTER_DEFALUT1_NUM  255
#define PWM_COMPARATOR_DEFALUT_NUM  50
#define PWM_CLOCK_SELE_DEFALUT_NUM  4
#define PWM_PRESCALE_DEFALUT_NUM  1

/******************************************************************************
*   STRUCT      :   sPWMDevConfig
******************************************************************************/
/**
 *  @brief   Structure to PWM driver config.
 *
 *****************************************************************************/
typedef struct
{    
	/* PWM Channel number */
	UINT8 u8PWMChannelNum;
	
	/* PWM Base Cycle Frequency */
	UINT8 u8PWMBaseCycleFrequency;
	
	/* PWM Frequency Divider */
	UINT8 u8PWMFrequencyDivider; 

	/* PWM Duty Cycle */
	UINT8 u8PWMDutyCycle; 
	
} sPWMDevConfig;

static int aess_pwm_config_init(sPWMDevConfig *PWMDevConfig);
static int aess_pwm_config_set(sPWMDevConfig *PWMDevConfig);
static int aess_pwm_config_info(sPWMDevConfig *PWMDevConfig);
static int aess_pwm_config_debug(sPWMDevConfig *PWMDevConfig);

#endif   /* AESSPWMDRV_C */
#endif   /* AESSPWMDRV_H */
