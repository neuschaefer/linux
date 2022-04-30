// **************************************************************************
// * Title         : STM8 Micro Controller - low level drivers
// *               : --------------------------------------------------------
// * Description   : Library routines to configure STM8 registers
// * Author        : CPG VoIP Team
// * Company       : STMicroelectronics CPG - Beijing (China)
// * Last update   : Nov 5, 2008
// * File name     : stm8.h
// **************************************************************************
// * Copyright (c) 2006 STMicroelectronics. All rights reserved. 
// **************************************************************************
//
//   This program is provided "as is" without warranty of any kind, either
//   expressed or implied, including but not limited to, the implied warranty
//   of merchantability and fitness for a particular purpose. the entire risk
//   as to the quality and performance of the program is with you. should the
//   program prove defective, you assume the cost of all necessary servicing,
//   repair or correction.
//
// **************************************************************************


#ifndef STM8_H
#define STM8_H

//#define STM8_DEBUG_READ_COMMAND
//#define STM8_DEBUG_WRITE_COMMAND

#define GPIO_BASE 0xA9000400

#define STM8_HOOK_KEYCODE 0xEE

#define STM8_MANAGE_HOOK_EVENTS
#define STM8_MANAGE_WAKEUP_EVENTS

#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>	
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <linux/mutex.h>

/******************************************************************************
                            GENERAL DEFINITIONS
******************************************************************************/
// STM8 device address
#define STM8_I2C_DEVICE_ADDR_WRITE  0x51

// STM8 register address
#define STATUS_CONTROL_MODULE 0x1E00	  // 30
#define STATUS_CONTROL_CTL (STATUS_CONTROL_MODULE + 0x0)		
#define STATUS_CONTROL_STATUS (STATUS_CONTROL_MODULE + 0x1)	
#define STATUS_CONTROL_ERR_FLAGS (STATUS_CONTROL_MODULE + 0x3)

#define POWER_DOWN_CONTROL_MODULE 0x1F00  // 31
#define POWER_DOWN_CONTROL_CTL (POWER_DOWN_CONTROL_MODULE + 0x0)
#define POWER_DOWN_CONTROL_WAKEUP_SOURCE1 (POWER_DOWN_CONTROL_MODULE + 0x1)
#define POWER_DOWN_CONTROL_WAKEUP_MASK1	(POWER_DOWN_CONTROL_MODULE + 0x2)

#define TOUCH_KEYBOARD_FIFO_MODULE 0x2000 // 32
#define TOUCH_KEYBOARD_FIFO_CTL  (TOUCH_KEYBOARD_FIFO_MODULE + 0x0)
#define TOUCH_KEYBOARD_FIFO_STATUS  (TOUCH_KEYBOARD_FIFO_MODULE + 0x1)
#define TOUCH_KEYBOARD_FIFO_MASK_OR_LOCK  (TOUCH_KEYBOARD_FIFO_MODULE + 0x2)
#define TOUCH_KEYBOARD_FIFO_DATA_FIELD  (TOUCH_KEYBOARD_FIFO_MODULE + 0x3)

#define TOUCH_CALIBRATION_SETTING_MODULE 0x2100 // 33
#define TOUCH_CALIBRATION_SETTING_CTL  (TOUCH_CALIBRATION_SETTING_MODULE + 0x0)
#define TOUCH_CALIBRATION_SETTING_STATUS  (TOUCH_CALIBRATION_SETTING_MODULE + 0x1)
#define TOUCH_CALIBRATION_SETTING_MASK  (TOUCH_CALIBRATION_SETTING_MODULE + 0x2)
#define TOUCH_CALIBRATION_SETTING_MAX_X  (TOUCH_CALIBRATION_SETTING_MODULE + 0x3)
#define TOUCH_CALIBRATION_SETTING_MAX_Y  (TOUCH_CALIBRATION_SETTING_MODULE + 0x5)
#define TOUCH_CALIBRATION_SETTING_CALIBRATION_CROSS_X  (TOUCH_CALIBRATION_SETTING_MODULE + 0x7)
#define TOUCH_CALIBRATION_SETTING_CALIBRATION_CROSS_Y  (TOUCH_CALIBRATION_SETTING_MODULE + 0x9)
#define TOUCH_CALIBRATION_SETTING_SENSITIVITY  (TOUCH_CALIBRATION_SETTING_MODULE + 0xB)

#define KEYBOARD_SETTING_MODULE 0x2200    // 34
#define KEYBOARD_SETTING_CTL (KEYBOARD_SETTING_MODULE + 0x0)
#define KEYBOARD_SETTING_STATUS (KEYBOARD_SETTING_MODULE + 0x1)
#define KEYBOARD_SETTING_MASK_OR_LOCK (KEYBOARD_SETTING_MODULE + 0x2)

#define RTC_UPDATE_MODULE 0x2300	  // 35

#define LED_CONTROL_MODULE 0x2400	  // 36
#define LED_CONTROL_CTL (LED_CONTROL_MODULE + 0x0)
#define LED_CONTROL_STATUS (LED_CONTROL_MODULE + 0x1)
#define LED_CONTROL_MASK (LED_CONTROL_MODULE + 0x2)
#define LED_CONTROL_DUTY_BACKLIGHT (LED_CONTROL_MODULE + 0x3)
//#define LED_CONTROL_DUTY_MSG (LED_CONTROL_MODULE + 0x4)
#define LED_CONTROL_870E_ONOFF (LED_CONTROL_MODULE + 0x4)
#define LED_CONTROL_DUTY_MAIN (LED_CONTROL_MODULE + 0x5)
#define LED_CONTROL_FREQ_BACKLIGHT (LED_CONTROL_MODULE + 0x7)
#define LED_CONTROL_FREQ_MSG (LED_CONTROL_MODULE + 0x8)
#define LED_CONTROL_FREQ_MAIN (LED_CONTROL_MODULE + 0x9)

#define INPUT_SIGNAL_CONTROL_MODULE 0x2500 //37
#define INPUT_SIGNAL_CONTROL_STATUS (INPUT_SIGNAL_CONTROL_MODULE + 0x1)

// STM8 specific flags
#define STM8_FLAG_STATUS_CONTROL_WAKEUP 0x80
#define STM8_FLAG_STATUS_CONTROL_TOUCH_CALIBRATION 0x20
#define STM8_FLAG_STATUS_CONTROL_GLOBAL_ERROR 0x10
#define STM8_FLAG_STATUS_CONTROL_FIFO_NOT_EMPTY 0x08
#define STM8_FLAG_STATUS_EXTERNAL_EVENT 0x04//hook and usb plug

#define STM8_FLAG_POWER_DOWN_CONTROL_ACTIVE 0x80
#define STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_HOOK 0x80
#define STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_TOUCHSCREEN 0x40
#define STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_KEYBOARD 0x20
#define STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_POWER_ON 0x02
#define STM8_FLAG_POWER_DOWN_WAKEUPSOURCE1_USBPLUG 0x01

#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_RELEASE_FIFO 0x02
#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_CONTROL_CLEAR_FIFO 0x01
#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_KEYBOARD 0x80
#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_TOUCHSCREEN 0x40
#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_PRESS 0x20
#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_RELEASE 0x10
#define STM8_FLAG_TOUCH_KEYBOARD_FIFO_STATUS_NOT_EMPTY 0x01

#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_CALIB_REQ 0x80
#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_CROSS_ACK 0x40
#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_CONTROL_TOUCH_DIS 0x01
#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_STATUS_UNCALIBRATED 0x80
#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_STATUS_SHOW_CROSS 0x40
#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_STATUS_CALIBRATION_ERROR 0x20
#define STM8_FLAG_TOUCH_CALIBRATION_SETTING_STATUS_CALIBRATION_FINISH 0x10

#define STM8_FLAG_KEYBOARD_SETTING_CONTROL_KEYBOARD_DIS 0x01

#define STM8_FLAG_LED_CONTROL_LED2_ON 0x04
#define STM8_FLAG_LED_CONTROL_LED1_ON 0x02
#define STM8_FLAG_LED_CONTROL_BACKLIGHT_ON 0x01

#define STM8_FLAG_INPUT_SIGNAL_CONTROL_HOOK 0x80
#define STM8_FLAG_INPUT_SIGNAL_CONTROL_USB 0x01
//used by STM8_SET_LED* ioctls
#define STM8_LED_MAIN 0x1
#define STM8_LED_MSG 0x2
#define STM8_LED_MUTE 0x3
#define STM8_LED_HEADSET 0x4
#define STM8_LED_HANDSFREE 0x5
#define STM8_LED_BACKLIGHT 0x6

// STM8 ioctl code
#define STM8_GET_STATUS 0x01
#define STM8_SET_POWER_DOWM_MODE 0x07
#define STM8_GET_POWER_DOWN_WAKEUPSOURCE 0x08
#define STM8_GET_TOUCH_KEYBOARD_FIFO_STATUS 0x0B
#define STM8_GET_KEY_CODE 0x0E
#define STM8_GET_TOUCH_CORDINATION 0x0F
#define STM8_SET_KEYBOARD_SETTING 0x1A
#define STM8_SET_LED 0x29
#define STM8_870E_SET_LED 0x30

#define STM8_SET_LED_DUTY 0x2A
#define STM8_SET_LED_FREQ 0x2B


#define STM8_GET_NEXT_FIFO_EVENT 0x40

#define STM8_SET_LED_MUTE 0x55
#define STM8_SET_LED_HEADSET 0x56
#define STM8_SET_LED_HANDSFREE 0x57
#define STM8_SET_BACKLIGHT 0x58

#ifdef STM8_DEBUG_READ_COMMAND
	#define STM8_READ_DBG 0xFF
#endif

#ifdef STM8_DEBUG_WRITE_COMMAND
	#define STM8_WRITE_DBG 0xFE
#endif

//Base Address of I2C Devices..
#ifdef CONFIG_ARCH_SPEARPLUS
#define SPEAr_I2C_APB_BASE VA_SPEARPLUS_START_I2C
#define APB_I2C_CFG_BASE   VA_SPEARPLUS_START_I2C + 0xf0
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
#define SPEAr_I2C_APB_BASE VA_SPEARBASIC_START_I2C
#define APB_I2C_CFG_BASE   VA_SPEARBASIC_START_I2C + 0xf0
#endif
#define IC_CLK 182 /* 100 kHz */

//I2C Registers Base Address Mapping
#define IC_CON   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0000))
#define IC_TAR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0004))
#define IC_SAR   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0008))
#define IC_CMD_DATA   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0010))
#define IC_SS_SCL_HCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0014))		// sets the scl clock high =period count for standered speed.
#define IC_SS_SCL_LCNT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0018))	

#define IC_INTR_MASK   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0030))
#define IC_RAW_INTR_STAT ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0034))

#define IC_RX_TL   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0038))
#define IC_TX_TL   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x003C))

//Clear Interupts; Read Only Registers
#define IC_CLR_INTR     ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0040))
#define IC_CLR_RX_UNDER ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0044))
#define IC_CLR_RX_OVER  ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0048))
#define IC_CLR_TX_OVER  ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x004C))
#define IC_CLR_RD_REQ  	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0050))
#define IC_CLR_TX_ABRT 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0054))
#define IC_CLR_RX_DONE 	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0058))
#define IC_CLR_ACTIVITY	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x005C)) 
#define IC_CLR_STOP_DET	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0060)) 
#define IC_CLR_START_DET ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0064)) 
#define IC_CLR_GEN_CALL	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0068)) 

#define IC_ENABLE   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x006C)) //R/W
#define IC_STATUS   	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0070)) //Read only
#define IC_TXABRT_SOURCE	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0080)) //Read/Write 

/* function for disabling and enabling I2C */
#define I2C_Disable()			( *IC_ENABLE &=~IC_ENABLE_0B) 
#define I2C_Enable()			( *IC_ENABLE |= IC_ENABLE_0B ) 
#define I2C_set_CON(x)			( *IC_CON = (unsigned int)x)
#define I2C_TAR(x)			( *IC_TAR = (unsigned int)x)
#define I2C_SAR(x)			( *IC_SAR = (unsigned int)x)
#define I2C_set_SS_SCL_HCNT(x)		( *IC_SS_SCL_HCNT = (unsigned int)x)
#define I2C_set_SS_SCL_LCNT(x)		( *IC_SS_SCL_LCNT = (unsigned int)x)
#define I2C_set_INTR_MASK(x)		( *IC_INTR_MASK = (unsigned int)x )
#define I2C_set_TX_TL(x)		( *IC_TX_TL = (unsigned int)x)
#define I2C_set_RX_TL(x)		( *IC_RX_TL = (unsigned int)x)

#define FIFO_SIZE 7

#define NANO_TO_MICRO 1000		

#define MIN_SS_SCL_HIGHTIME  	5000//4000	 //in nanosec
#define MIN_SS_SCL_LOWTIME   	5000	 //nanosec

#define IC_ABRT_7B_ADDR_NOACK 0x0001
#define IC_ABRT_TXDATA_NOACK  0x0008

//setting cmd bit to '1'  for read
#define IC_CMD 0x0100 

#define IC_ENABLE_0B	0x0001

/*---------------------------------------------------------------------------------------
	I2C Control register

	IC_CON [15:7] = Reserved
	IC_CON [6] =	IC_SLAVE_DISABLE
	IC_CON [5] = 	IC_RESTART_ENABLE
	IC_CON [4] = 	IC_10BITADDR_MASTER
	IC_CON [3] = 	IC_10BITADDR_SLAVE
	IC_CON [2:1] =  SPEED
     		   	00 = None
			01 = IC_SPEED_MODE_STANDARD,        	            // Standard I2C mode (SCL < 100 KHz)
			10 = IC_SPEED_MODE_FAST,	           	    // Fast I2C mode (100 KHz <= SCL < 400 KHz)
			03 = IC_MAX_SPEED_MODE				    // 3.4 Mbits		
	IC_CON [0] = 	MASTER_MODE
----------------------------------------------------------------------------------------*/

typedef enum
{
	IC_CON_SD		= 0x0040,			      // IC_SLAVE_DISABLE
	IC_CON_RE		= 0x0020,  			      // IC_RESTART_ENABLE
	IC_CON_10BITADDRMASTER	= 0x0010, 			      // IC_10BITADDR_MASTER
	IC_CON_10BITADDR_SLAVE	= 0x0008, 			      // IC_10BITADDR_SLAVE
	IC_CON_SPH		= 0x0006,	            	      // Control Speed 3: High Speed mode
	IC_CON_SPF		= 0x0004, 			      // Control Speed 2:fast mode
	IC_CON_SPL		= 0x0002, 			      // Control Speed 1: standered mode
	IC_CON_MM 		= 0x0001			      // control master mode, 
}t_I2C_ControlReg;

/*---------------------------------------------------------------------------------------
	I2C Target address register

	IC_TAR [15:13] = Reserved
	IC_TAR [12] =    IC_10BITADDR_MASTER
	IC_TAR [11] = 	 SPECIAL
	IC_TAR [10] =    GC_OR_START // IF 0= General Call setup; 1= StartByte
	IC_TAR [9:0] =   IC_TAR	
----------------------------------------------------------------------------------------*/

typedef enum
{
	IC_TAR_10ADM = 0x1000,//                          
 	IC_TAR_SP    = 0x0800,//                        
 	IC_TAR_GS    = 0x0400,//                                
 	IC_TAR_9B    = 0x0200,//=========================        
 	IC_TAR_8B    = 0x0100,//                                    
 	IC_TAR_7B    = 0x0080,//                                
 	IC_TAR_6B    = 0x0040,//                                 
        IC_TAR_5B    = 0x0020,// 10bit of target address
 	IC_TAR_4B    = 0x0010,// 
 	IC_TAR_3B    = 0x0008,//                         
 	IC_TAR_2B    = 0x0004,//                                 
 	IC_TAR_1B    = 0x0002,// 
 	IC_TAR_0B    = 0x0001,//==========================    
}t_I2C_tar;

/*---------------------------------------------------------------------------------------
	Interrupt Status Register [Read only registers]

	IC_INTR_STAT [15:12]=	Reserevd
        IC_INTR_STAT [11]=	R_GEN_CALL 	
   	IC_INTR_STAT [10]=	R_START_DETECT	
	IC_INTR_STAT [9]=	R_STOP_DETECT	
	IC_INTR_STAT [8]=	R_ACTIVITY	
	IC_INTR_STAT [7]=	R_RX_DONE	
	IC_INTR_STAT [6]=	R_TX_ABRT	
	IC_INTR_STAT [5]=	R_RD_REQ	
	IC_INTR_STAT [4]=	R_TX_EMPTY	     
	IC_INTR_STAT [3]=	R_TX_OVER	
	IC_INTR_STAT [2]=	R_RX_FULL	
	IC_INTR_STAT [1]=	R_RX_OVER	
	IC_INTR_STAT [0]=	R_RX_UNDER	
----------------------------------------------------------------------------------------*/

typedef enum
{

	IC_R_GEN_CALL 	= 0x0800,
	IC_R_START_DET	= 0x0400,
	IC_R_STOP_DET	= 0x0200,
	IC_R_ACTIVITY	= 0x0100,
	IC_R_RX_DONE	= 0x0080,
	IC_R_TX_ABRT	= 0x0040,
	IC_R_RD_REQ	= 0x0020,
	IC_R_TX_EMPTY	= 0x0010,	
	IC_R_TX_OVER	= 0x0008,
	IC_R_RX_FULL	= 0x0004,
	IC_R_RX_OVER	= 0x0002,
	IC_R_RX_UNDER	= 0x0001
    	
}t_I2CIntStatusType;

/*---------------------------------------------------------------------------------------
	I2c Status Register
---------------------------------------------------------------------------------------*/

typedef enum
{
	IC_STATUS_SA	= 0x0040,
	IC_STATUS_MA	= 0x0020,
	IC_STATUS_RFF	= 0x0010,
	IC_STATUS_RFNE	= 0x0008,
	IC_STATUS_TFE	= 0x0004,
	IC_STATUS_TFNF	= 0x0002,
	IC_STATUS_ACT	= 0x0001
}t_I2C_Status;

/*---------------------------------------------------------------------------------------
	Interrupt Status Mask Register [Read/Write  registers]

	IC_INTR_MASK [15:12]=	Reserevd
        IC_INTR_MASK [11]=	M_GEN_CALL 	
   	IC_INTR_MASK [10]=	M_START_DETECT	
	IC_INTR_MASK [9]=	M_STOP_DETECT	
	IC_INTR_MASK [8]=	M_ACTIVITY	
	IC_INTR_MASK [7]=	M_RX_DONE	
	IC_INTR_MASK [6]=	M_TX_ABRT	
	IC_INTR_MASK [5]=	M_RD_REQ	
	IC_INTR_MASK [4]=	M_TX_EMPTY	     
	IC_INTR_MASK [3]=	M_TX_OVER	
	IC_INTR_MASK [2]=	M_RX_FULL	
	IC_INTR_MASK [1]=	M_RX_OVER	
	IC_INTR_MASK [0]=	M_RX_UNDER	
    	
#offset addr= 0x30
----------------------------------------------------------------------------------------*/

typedef enum
{

	IC_M_GEN_CALL 	= 0x0800,
   	IC_M_START_DET	= 0x0400,
	IC_M_STOP_DET	= 0x0200,
	IC_M_ACTIVITY	= 0x0100,
	IC_M_RX_DONE	= 0x0080,
	IC_M_TX_ABRT	= 0x0040,
	IC_M_RD_REQ	= 0x0020,
	IC_M_TX_EMPTY	= 0x0010,	
	IC_M_TX_OVER	= 0x0008,
	IC_M_RX_FULL	= 0x0004,
	IC_M_RX_OVER	= 0x0002,
	IC_M_RX_UNDER	= 0x0001
    	
}t_I2CIntMaskType;


// STM8 device structure
#define STM8_MAJOR_NO 241
static const int stm8_I2C_devno = MKDEV(STM8_MAJOR_NO,0);
struct STM8_device
{
	struct cdev i2c_cdev;
	struct fasync_struct *stm8_async_queue;
};

static struct STM8_device stm8_dev;

static u8 stm8_status;
static u8 stm8_power_down_mode;
static u16 stm8_power_down_wakeupsource;
static u8 stm8_led;
static u8 stm8_keyboard_setting;

static u32 stm8_led_mute;
static u32 stm8_led_headset;
static u32 stm8_led_handsfree;
static u32 stm8_backlight;

struct keyboard_touch_fifo {
	struct list_head list;
	u8 current_status;
	u8 keyboard_code;
	//u32 touchscreen_xy;
	//x and y coordinates, most and least signif. bit
	u8 ts_x_m;
	u8 ts_x_l;
	u8 ts_y_m;
	u8 ts_y_l;
};
//static LIST_HEAD(keyboard_touch_list);

static struct list_head keyboard_touch_list;

struct keyboard_touch_event{
	u8 current_status;
	u8 keyboard_code;
	//u32 touchscreen_xy;
	//x and y coordinates, most and least signif. bit
	u8 ts_x_m;
	u8 ts_x_l;
	u8 ts_y_m;
	u8 ts_y_l;
};

struct stm8_led_setting{
	u8 led;
	u8 param;
};

unsigned int *SPEARBASIC_PL_GPIO_DATA = 0;

#define GPIO_LED_MUTE 0x5
#define GPIO_BACKLIGHT 0x4
//#define GPIO_LED_HEADSET 0x6
//#define GPIO_LED_HANDSFREE 0x7
//modified GPIO numbers, because of multiplexing
//patch by Dusan, to have these on GPIO 0 and 1
#define GPIO_LED_HEADSET 0x0
#define GPIO_LED_HANDSFREE 0x1

//#define GPIO_SET_OUT(pin)	*(unsigned int*)SPEARBASIC_GPIO_BASE |= (1<<pin)
#define STM8_GPIO_SET(pin)		*(unsigned int*)(SPEARBASIC_PL_GPIO_DATA) |= (0x1 << pin)
#define STM8_GPIO_RESET(pin)	*(unsigned int*)(SPEARBASIC_PL_GPIO_DATA) &= ~(0x1 << pin)


#endif

