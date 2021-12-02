/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: timer.h $
 *
 * Project:
 * --------
 *   MT8205
 *
 * Description:
 * ------------
 * Timer function header
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/28 4:40p $
 *
 * $Revision: #1 $
*************************************************************************/
#ifndef _CUSTOM_DEF_H_
#define _CUSTOM_DEF_H_
#define CC_MT5365
//#define CC_MT5363
//#define CC_MT5387
//#define CC_MT5395
//#define CC_MT8223
#define FW_VERSION 12 //0x88
#define RC56_IR_DRIVER 0
#define CUST_P_DC_ON_SEQUENCE 0 // 1
#define S_HOTEL_MODE_COMMAND 1
#define UART_PD_RX_EN 0// 1
#define UP_ARM_TEST 0
#define LIGHT_GUIDE 0
#define GPIO_CTRL_SUPPORT 0//  1
#define SERVO_CTRL_SUPPORT 0// 1
#define IR_DEBOUNCE 0// 1
#define PDWNC_INT_POLLING 0
#define IR_MULTIKEY_WAKEUP 0
#define ERR_DATA_RESPONSE 0

/***************************
*RW_REG_INTERFACE : by teraterm 
*UART_DBG : by Mtktool
***************************/
#define RW_REG_INTERFACE 0// 1 //for sif read and write test under standby(open it can r/w xdata under standby)
#define UART_DBG 0
#define SIR_DBG 0

#define SUPPORT_ERROR_LED_CTRL 1
//david: always enable #define FW_UPGRATE_PWR_CTRL 0// 1
#define SUPORT_COLD_REBOOT 0
#define SUPORT_DIGIT_KEY_WAKEUP 0 // 1
#define SWITCH_UART_PORT_538x 0
#define SWITCH_UART_PORT_5365 1
#define SWITCH_UART_PORT_5395 0
#define T8032_OCCUPIED_UART0_AT_NORMAL_POWER 0
#define SIMPLE_LED_TOGGLE 0 // 1
#define SIF_DRV_SUPPORT 0// 1
#define SIR_TASK 1 // 1
#define CUSH_S_IR_CHK_SUPPORT 1//only guide pulse check now
#define QUERY_IR_POWER_KEY_STATE_SUPPORT 1

#if SIF_DRV_SUPPORT
    #define SIF_BUFFER_SIZE 0x50     
    #undef EEPROM_DRV_SUPPORT 
    #define EEPROM_DRV_SUPPORT 1
#else
#define EEPROM_DRV_SUPPORT 0
#endif

#if EEPROM_DRV_SUPPORT 
    #undef SIF_PDWNC_TEST
    #define SIF_PDWNC_TEST    1//for sif read and write test
    #define EEPROM_DEV_ADDR 0xA0
    #define EEPROM_SUB_ADDR_SIZE 2//24c02 only need one sub address (so call byte address), 24C128 needs 2 bytes for sub address...
    #define EEPROM_WP_GPIO_MASK (0x1 << 3) //opctrl3 for write protection pin
    #define EEPROM_WP_GPIO_POLARITY 1//HI : WP , LO: WRITE 
    #define EEPROM_CLK_DIV   0x100 //eeprom ==> 100Khz, 27M/0x100 =  100khz      
#else
#define SIF_PDWNC_TEST    0
#endif

#define SWITCH_UART_PORT_538x 0
#define SWITCH_UART_PORT_5365 1
#define SWITCH_UART_PORT_5395 0
//#define SIF_ISR 0
#if S_HOTEL_MODE_COMMAND
    #define UART_TASK 1
    #if UART_TASK
        #define MAX_UART_BUF_LEN 32
    #endif
#endif    

#define IR_TASK 1  //For P_COMP platform, this define must be 1!!!!!!!!!!!!!!

#if SIF_DRV_SUPPORT 
#define SIF_SCL_STRECH 0
#define SIF_ODRAIN 0
#endif

#define SIRC_IR 1
#define NEC_IR 2
#define RC56_IR 3
#define SHARP_IR 4

#define IR_PROTOCOL SIRC_IR

#if (IR_PROTOCOL == SIRC_IR)
typedef UINT8 IR_DATA_TYPE;
#else
typedef UINT32 IR_DATA_TYPE;
#endif

#if CUSH_S_IR_CHK_SUPPORT
	#define GPIO_INTERRUPT_ENABLE 1
#else
	#define GPIO_INTERRUPT_ENABLE 0
#endif


/******* CEC *******************/
//#define MT5387_CEC  1
#define MT5365_CEC  1

#define CEC_FEATURE_1  	0
#define CEC_FEATURE_2  	1
#define CEC_FEATURE_3 	0
#define CEC_FEATURE_4   0

/******* CEC Feature 1 *************/
#if CEC_FEATURE_1
#define ENABLE_CEC 1
#define DISABLE_CEC_WITHOUT_ACK 1  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
#endif
/******* CEC Feature 2 *************/
#if CEC_FEATURE_2
#define ENABLE_CEC 1
#define DISABLE_CEC_WITHOUT_ACK 1  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
#endif
/******* CEC Feature 3 *************/
#if CEC_FEATURE_3
#define ENABLE_CEC 1
#define DISABLE_CEC_WITHOUT_ACK 1  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
//#define CEC_FEATURE_3_1 
#endif
/******* CEC_FEATURE_4 *************/
#if CEC_FEATURE_4
#define ENABLE_CEC 0
#define DISABLE_CEC_WITHOUT_ACK 0  
#define STORE_ALL_MSG_AFTER_WAKEUP 0
#endif

#endif 
