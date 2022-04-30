// **************************************************************************
// * Title         : STM8 Micro Controller - API
// *               : --------------------------------------------------------
// * Description   : Public STM8 API, used to interact with STM8 microcontroller
// * Author        : CPG VoIP Team
// * Company       : STMicroelectronics CPG - Milan (Italy)
// * Last update   : May 21, 2009
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

#include <linux/types.h>

//if Hook events managed as key events (see below)
//this is the corresponding "hook" keycode
#define STM8_HOOK_KEYCODE 0xEE


/******************************************************************************
                            GENERAL DEFINITIONS
******************************************************************************/

//OLD API (new API below)
//present for cross-check

//used by STM8_SET_LED* ioctls (OLD API)
#define STM8_LED_MAIN 0x1
#define STM8_LED_MSG 0x2
#define STM8_LED_MUTE 0x3
#define STM8_LED_HEADSET 0x4
#define STM8_LED_HANDSFREE 0x5

// STM8 ioctl code 
#define STM8_GET_NEXT_FIFO_EVENT 0x40
#define STM8_SET 0x60
#define STM8_GET 0x61
#define STM8_TS_CALIBRATION_INIT 0x70
#define STM8_TS_CALIBRATION_REQ 0x71
#define STM8_TS_CALIBRATION_ACK 0x72

//possibles events
//NB: currently Hook is managed as a key press/release as per SNOM request
typedef enum 
{
	TOUCH_PRESSED,
	TOUCH_RELEASED,
	TOUCH_DRAGGED,
	KEY_PRESSED,
	KEY_RELEASED,
	HOOK_CHANGED//FIXME needed? See above
} stm8_event_type;

//configurable features enumeration
//used in stm8_ctrl to select the driver's features to configure
//CALL_LED and MSG_LED can have 3 states: ON, OFF, BLINK
//other features: if passed ON state then activate feature, else deactivate
typedef enum 
{
	CALL_LED,//blinkable
	MSG_LED,//blinkable
	PIN17_EHS,
	PIN23_EHS,//only STM8_GET supported
	PIN27_SPI_CK,
	PIN32_AMP,
	KEYPAD,
	TOUCHSCREEN,
	DRAGMODE,
	MUTE_LED,
	HEADSET_LED,
	HANDSFREE_LED,
	STM8_870E_LED_CTL,
	STM8_820_LED_CTL,
	BACKLIGHT,
} stm8_ctrl_feature;


typedef enum 
{ 
    OFF,
    ON,
    BLINK //BLINK only for CALL_LED and MSG_LED features
} stm8_ctrl_state;

typedef struct 
{
	stm8_ctrl_feature feature;
	stm8_ctrl_state state;
	unsigned int duty_cycle; // only for state=BLINK
	unsigned int frequency;  // only for state=BLINK
	unsigned int pixels;     // only for feature=DRAGMODE
	unsigned int period;     // only for feature=DRAGMODE
        unsigned int led_870e;   // only for 870E led control
        unsigned int led_820;    // only for 820 led control
} stm8_ctrl;

typedef struct 
{
	char global_status;
	char touchscreen_status;
	unsigned int calibration_cross_x;//where to draw the calibration cross
	unsigned int calibration_cross_y;//where to draw the calibration cross
} stm8_calibration_info;

//Main events:
//all events reported by the driver have this structure
typedef struct keyboard_touch_event
{
	unsigned char current_status;
	stm8_event_type event_type;
	unsigned char keyboard_code;
	unsigned int x;
	unsigned int y;
} keyboard_touch_event;

#endif
