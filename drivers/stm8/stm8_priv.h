// **************************************************************************
// * Title         : STM8 Micro Controller - low level drivers
// *               : --------------------------------------------------------
// * Description   : Library routines to configure STM8 registers
// * Author        : CPG VoIP Team
// * Company       : STMicroelectronics CPG - Milan (Italy)
// * Last update   : May 21, 2009
// * File name     : stm8_priv.h
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

//NB: some sections commented out and placed in the "public header" linux/stm8.h

#ifndef STM8_PRIV_H
#define STM8_PRIV_H

#define STM8_DRIVER_VERSION "2.0 (SNOM 1.2 + FORCE_AMP + PATCH_CALIB)"

//#define STM8_DEBUG_READ_COMMAND
//#define STM8_DEBUG_WRITE_COMMAND

//#define STM8_HOOK_KEYCODE 0xEE

#define STM8_MANAGE_HOOK_EVENTS
#define STM8_MANAGE_WAKEUP_EVENTS

#include <linux/types.h>
#include <linux/fs.h>	
#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/stm8.h>
#include <linux/cdev.h>

/******************************************************************************
                            GENERAL DEFINITIONS
******************************************************************************/
// STM8 device address
#define STM8_I2C_DEVICE_ADDR_WRITE  0x51

// STM8 register address
#define STATUS_CONTROL_MODULE 0x1E00	  // 30
#define STATUS_CONTROL_CTL (STATUS_CONTROL_MODULE + 0x0)		
#define STATUS_CONTROL_STATUS (STATUS_CONTROL_MODULE + 0x1)
#define STATUS_CONTROL_MASK (STATUS_CONTROL_MODULE + 0x2)
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
#define TOUCH_KEYBOARD_FIFO_DRAG_FIELD  (TOUCH_KEYBOARD_FIFO_MODULE + 0x7)

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
#define LED_CONTROL_DUTY_MSG (LED_CONTROL_MODULE + 0x4)
#define LED_CONTROL_CMD (LED_CONTROL_MODULE + 0x4)
#define LED_CONTROL_DUTY_CALL (LED_CONTROL_MODULE + 0x5)
#define LED_CONTROL_FREQ_BACKLIGHT (LED_CONTROL_MODULE + 0x7)
#define LED_CONTROL_FREQ_MSG (LED_CONTROL_MODULE + 0x8)
#define LED_CONTROL_FREQ_CALL (LED_CONTROL_MODULE + 0x9)

#define INPUT_SIGNAL_CONTROL_MODULE 0x2500 //37
#define INPUT_SIGNAL_CONTROL_CONTROL (INPUT_SIGNAL_CONTROL_MODULE + 0x0)
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

#define STM8_FLAG_LED_CONTROL_LED2_BLINK 0x40
#define STM8_FLAG_LED_CONTROL_LED1_BLINK 0x20
#define STM8_FLAG_LED_CONTROL_LED2_ON 0x04
#define STM8_FLAG_LED_CONTROL_LED1_ON 0x02
#define STM8_FLAG_LED_CONTROL_BACKLIGHT_ON 0x01

#define STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN17_EHS 0x4
#define STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN27_SPI_CK 0x2
#define STM8_FLAG_INPUT_SIGNAL_CONTROL_PIN32_AMP 0x1
#define STM8_FLAG_INPUT_SIGNAL_STATUS_PIN23_EHS 0x2

#define STM8_FLAG_INPUT_SIGNAL_CONTROL_HOOK 0x80
#define STM8_FLAG_INPUT_SIGNAL_CONTROL_USB 0x01

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

#define IC_INTR_STAT ((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x002C))
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
#define IC_TXFLR    	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0074))
#define IC_RXFLR    	((volatile u32 *)(SPEAr_I2C_APB_BASE + 0x0078))
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

#define NANO_TO_MICRO 1000		

#define MIN_SS_SCL_HIGHTIME  	4000 //5000//4000	 //in nanosec
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

struct STM8_device
{
	struct cdev i2c_cdev;
	struct fasync_struct *stm8_async_queue;
};

typedef enum {
	DRAG_ON,
	DRAG_OFF
} stm8_drag_states;

typedef enum {
	LED_ON,
	LED_OFF,
	LED_BLINK
} stm8_led_states;

//tracking drag&drop driver state
struct STM8_drag_configuration{
	stm8_drag_states status;
	unsigned int time_resolution;
	unsigned int pixel_threshold;
};
static struct STM8_drag_configuration stm8_drag_conf; 

static struct stm8_features_status
{
	stm8_ctrl_state call_led;
	stm8_ctrl_state msg_led;
	stm8_ctrl_state pin17_ehs;
	stm8_ctrl_state pin27_spi_ck;
	stm8_ctrl_state pin32_amp;
	stm8_ctrl_state keypad;
	stm8_ctrl_state touchscreen;
	stm8_ctrl_state dragmode;
	stm8_ctrl_state mute_led;
	stm8_ctrl_state headset_led;
	stm8_ctrl_state handsfree_led;
} stm8_features_status;

typedef struct
{
	unsigned int duty_cycle;
	unsigned int frequency;
} stm8_led_conf;

struct keyboard_touch_fifo 
{
	u8 current_status;//read from stm8 hw
	stm8_event_type event_type;
	u8 keyboard_code;
	u8 ts_x_m;
	u8 ts_x_l;
	u8 ts_y_m;
	u8 ts_y_l;
};


#define GPIO_LED_MUTE 0x5
#define GPIO_LED_HEADSET 0x0
#define GPIO_LED_HANDSFREE 0x1
#define GPIO_STM8_RST 0x2

#define STM8_GPIO_SET(pin)		GPIO0_DATA |= (0x1 << pin)
#define STM8_GPIO_RESET(pin)	GPIO0_DATA &= ~(0x1 << pin)

#define GPIO0_DATA mappedGPIO0[0x3FC/4]    
#define GPIO0_DIR mappedGPIO0[0x400/4]

#define GPIO1_DATA mappedGPIO1[0x3FC/4]
#define GPIO1_DIR mappedGPIO1[0x400/4]
#define GPIO1_IS  mappedGPIO1[0x404/4]
#define GPIO1_IEV mappedGPIO1[0x40C/4]
#define GPIO1_IE  mappedGPIO1[0x410/4]
#define GPIO1_MIS  mappedGPIO1[0x418/4]
#define GPIO1_IC  mappedGPIO1[0x41C/4]

#define TIMERB22_CONTROL mappedTIMERB22[0x100/4]
#define TIMERB22_COMPARE mappedTIMERB22[0x108/4]
#define TIMERB22_STATUS_INT_ACK mappedTIMERB22[0x104/4]

#define TIMER_CONTROL_ENABLE_FLAG 0x0020
#define TIMER_COUNTER_IRQ_CLEAR_FLAG 0x1

#define TIMER_CONTROL_FLAG_COMPARATOR (0x1 << 8)
#define TIMER_CONTROL_FLAG_NOCAPTURE (0x0 << 6)
#define TIMER_CONTROL_FLAG_AUTORELOAD (0x0 << 4)
#define TIMER_CONTROL_PRESCALER_DEFAULT (0x6 << 0)
#define TIMER_CONTROL_DEFAULT TIMER_CONTROL_PRESCALER_DEFAULT

#endif
