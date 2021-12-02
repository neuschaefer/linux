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
/*-----------------------------------------------------------------------------
 *
 * $Author: muran.xiang $
 * $Date: 2012/07/18 $
 * $RCSfile: x_pdwnc.h,v $
 * $Revision: #6 $
 *
 *---------------------------------------------------------------------------*/

/** @file x_pdwnc.h
 *  x_pdwnc.h The PDWNC (Power Down Interface Module) interface header file
 */


#ifndef X_PDWNC_H
#define X_PDWNC_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_hal_5381.h"
#include "x_gpio.h"
#include "x_lint.h"
#include "hw_vdoin.h"
#include "hw_pdwnc.h"
LINT_EXT_HEADER_BEGIN
#ifdef CC_MTK_LOADER
#ifndef CC_UBOOT
#ifndef __GCC__   /*gcc compile */
typedef VOID (*x_os_isr_fct) (UINT16  ui2_vector_id);
#endif
#endif /* CC_UBOOT */
#else
#ifndef CC_SECURE_WORLD
#include "x_os.h"
#endif
#endif
LINT_EXT_HEADER_END

#ifdef CC_5360_LOADER
#define x_crit_start() 0
#define x_crit_end(x) UNUSED(x)
#define CRIT_STATE_T UINT8
#else
#ifndef CC_SECURE_WORLD
#include "x_os.h"
#include "sif_if.h"
#endif
#endif


//===========================================================================
// Macros for register read/write
/*
#define PDWNC_READ8(offset)         IO_READ8(PDWNC_BASE, (offset))
#define PDWNC_READ16(offset)        IO_READ16(PDWNC_BASE, (offset))
#define PDWNC_READ32(offset)        IO_READ32(PDWNC_BASE, (offset))

#define PDWNC_WRITE8(offset, value) IO_WRITE8(PDWNC_BASE, (offset), (value))
#define PDWNC_WRITE16(offset,value) IO_WRITE16(PDWNC_BASE, (offset), (value))
#define PDWNC_WRITE32(offset,value) IO_WRITE32(PDWNC_BASE, (offset), (value))

#define PDWNC_REG8(offset)          IO_REG8(PDWNC_BASE, (offset))
#define PDWNC_REG16(offset)         IO_REG16(PDWNC_BASE, (offset))
#define PDWNC_REG32(offset)         IO_REG32(PDWNC_BASE, (offset))
*/

//from 5389 not use PDWNC_INTNO_xxxx
#if defined(CC_MT5368)
	#define PDWNC_INTNO_OPCTRL0    		0	// OPCTRL 0 interrupt number
	#define PDWNC_INTNO_OPCTRL1    		1	// OPCTRL 1 interrupt number
	#define PDWNC_INTNO_OPCTRL2    		2	// OPCRTL 2 interrupt number
	#define PDWNC_INTNO_OPCTRL3    		3	// OPCTRL 3 interrupt number
	#define PDWNC_INTNO_OPCTRL4    		4	// OPCTRL 4 interrupt number
	#define PDWNC_INTNO_OPCTRL5    		5	// OPCTRL 5 interrupt number
	#define PDWNC_INTNO_OPCTRL6    		6	// OPCTRL 6 interrupt number
	#define PDWNC_INTNO_OPCTRL7    		7	// OPCTRL 7 interrupt number
	#define PDWNC_INTNO_SIFM 			8	// SIFM0 interrupt number
#else
	#define PDWNC_INTNO_OPCTRL0    		0	// OPCTRL 0 interrupt number
	#define PDWNC_INTNO_OPCTRL1    		1	// OPCTRL 1 interrupt number
	#define PDWNC_INTNO_OPCTRL2    		2	// OPCRTL 2 interrupt number
	#define PDWNC_INTNO_OPCTRL3    		3	// OPCTRL 3 interrupt number
	#define PDWNC_INTNO_HDMI_HPD2    	4	// HDMI HPD2 interrupt number
	#define PDWNC_INTNO_PWR5V_2    		5	// PWR5V_2 interrupt number
	#define PDWNC_INTNO_HDMI_SDA2    	6	// HDMI_SDA2 interrupt number
	#define PDWNC_INTNO_HDMI_SCL2    	7	// HDMI_SCL2 interrupt number
	#define PDWNC_INTNO_SIFM 			8	// SIF master interrupt number
#endif
	#define PDWNC_INTNO_UART_DBG 		9	// UART debug interrupt number
#if defined(CC_MT5387) && !defined(CC_MT5363)

#elif defined(CC_MT5363)
	#define PDWNC_INTNO_AVLINK			10	// AVLINK interrupt number
#else//for 5365/95...
	#define PDWNC_INTNO_AVLINK0			10	// AVLINK0 interrupt number
#endif
	#define PDWNC_INTNO_CEC      		11  // CECINT0 interrupt number
	#define PDWNC_INTNO_RTC      		12  // RTC interrupt number
	#define PDWNC_INTNO_IRRX     		13  // IRRX interrupt number
	#define PDWNC_INTNO_ARM_INT    		14  // ARM interrupt number
	#define PDWNC_INTNO_AUX_INT     	15  // AUX interrupt number
#if defined(CC_MT5368)	 
	#define PDWNC_INTNO_VGA_SDA			16
	#define PDWNC_INTNO_VGA_SCL			17
#elif defined(CC_MT5365) || defined(CC_MT5395)
	#define PDWNC_INTNO_SIFSR			16	//Only MT5365/MT5395 have this bit
	#define PDWNC_INTNO_SIFSW			17	//Only MT5365/MT5395 have this bit
#endif	
#if defined(CC_MT5368) 
	#define PDWNC_INTNO_LED_PWM0	 	18 	 // LED_PWM0 interrupt number
	#define PDWNC_INTNO_LED_PWM1	 	19 	 // LED_PWM1 interrupt number
	#define PDWNC_INTNO_SIFM1		 	20 	 // SIFM1 interrupt number
	#define PDWNC_INTNO_DDCCI	 	 	21 	 // DDCCI interrupt number
	#define PDWNC_INTNO_AVLINKINT1   	22 	 // AVLINKINT1 interrupt number
	#define PDWNC_INTNO_CECINT1	 	 	23	 // CECINT1 interrupt number	
#elif defined(CC_MT5396) 
#elif defined(CC_MT5365) || defined(CC_MT5395)
	#define PDWNC_INTNO_SRVAD2   		18  // SRVAD 2 interrupt number
    #define PDWNC_INTNO_SRVAD3   		19  // SRVAD 3 interrupt number
    #define PDWNC_INTNO_SRVAD4   		20  // SRVAD 4 interrupt number
    #define PDWNC_INTNO_SRVAD5   		21  // SRVAD 5 interrupt number
    #define PDWNC_INTNO_CECINT1  		22  // SRVAD 4 interrupt number
    #define PDWNC_INTNO_AVLINKINT1  	23  // SRVAD 5 interrupt number
#else
    #define PDWNC_INTNO_SRVAD0   		18   // SRVAD 0 interrupt number
    #define PDWNC_INTNO_SRVAD1   		19   // SRVAD 1 interrupt number
    #define PDWNC_INTNO_SRVAD2   		20   // SRVAD 2 interrupt number
    #define PDWNC_INTNO_SRVAD3   		21   // SRVAD 3 interrupt number
    #define PDWNC_INTNO_SRVAD4   		22   // SRVAD 4 interrupt number
    #define PDWNC_INTNO_SRVAD5   		23   // SRVAD 5 interrupt number
#endif

#if defined(CC_MT5368) 
	#define PDWNC_INTNO_OIRI			25 	 // OIRI interrupt number
	#define PDWNC_INTNO_STB_SDA	 		26	 // STB_SDA interrupt number
	#define PDWNC_INTNO_STB_SCL	 		27 	 // STB_SCL interrupt number
	#define PDWNC_INTNO_SRVAD4			28 	 // SRVAD 4 interrupt number
	#define PDWNC_INTNO_SRVAD5	 		29 	 // SRVAD 5 interrupt number
	#define PDWNC_INTNO_SRVAD6	 		30 	 // SRVAD 6 interrupt number
	#define PDWNC_INTNO_SRVAD7	 		31 	 // SRVAD 7 interrupt number	
#elif defined(CC_MT5396) 
#elif defined(CC_MT5365) || defined(CC_MT5395)
	#define PDWNC_INTNO_HDMI_SDA 		24	 // OPWR0 interrupt number
	#define PDWNC_INTNO_HDMI_CEC 		25	 // OPRW1 interrupt number
	#define PDWNC_INTNO_VGA_SCL 		26	 // OPWR2 interrupt number
	#define PDWNC_INTNO_VGA_SDA	 		27 	 // T8032 interrupt number
	#define PDWNC_INTNO_U0RX			28
	#define PDWNC_INTNO_UOTX			29	 // OPRW1 interrupt number
	#define PDWNC_INTNO_OIRI			30	 // OPWR2 interrupt number
	#define PDWNC_INTNO_HDMI_SCL		31	 // OPWR2 interrupt number
#else
	#define PDWNC_INTNO_HDMI_HPD1		24   // OPWR0 interrupt number
	#define PDWNC_INTNO_PWR5V_1 		25   // OPRW1 interrupt number
	#define PDWNC_INTNO_HDMI_SDA1 		26   // OPWR2 interrupt number
	#define PDWNC_INTNO_HDMI_SCL1   	27   // T8032 interrupt number
	#define PDWNC_INTNO_HDMI_HPD0		28
	#define PDWNC_INTNO_PWR5V_0 		29   // OPRW1 interrupt number
	#define PDWNC_INTNO_HDMI_SDA0 		30   // OPWR2 interrupt number
	#define PDWNC_INTNO_OIRI	 		31   // OPWR2 interrupt number
#endif
	#define MAX_PDWNC_INT_ID     		32   // Maximum value of PDWNC interrupt id


#define PDWNC_OK                0
#define PDWNC_FAIL              1
#if defined(CC_MT5368)
	#define TOTAL_PDWNC_GPIO_NUM    37
#elif defined(CC_MT5396)	
	#define TOTAL_PDWNC_GPIO_NUM    41
#elif defined(CC_MT5389) 
	#define TOTAL_PDWNC_GPIO_NUM    19
#elif defined(CC_MT5365) 
    #define TOTAL_PDWNC_GPIO_NUM    22
#elif defined(CC_MT5395) 
    #define TOTAL_PDWNC_GPIO_NUM    23
#else
    #define TOTAL_PDWNC_GPIO_NUM    20
#endif
#define TOTAL_PDWNC_DEDICATE_GPI_NUM    7//check 2809c[7:0], only the first 8 pwdnc gpio(EXTIO) can be set as wakeup pins
#define TOTAL_SERVOADC_NUM      TOTAL_SERVOADC_NUMBER// servoadc ch6,7 is dedicated used power detection
//david add below

#define PDANC_PAD_SERVOADC_EN(x)  (1U << (x))    // SERVO ADC PAD output enable, not use from 5365...    
#define PDWNC_PAD_SRVIO_OUT(x)    (0x1U << (x))    
#if defined(CC_MT5387) && !defined(CC_MT5363) 
    #define FLD_HDMI_SDA0_MASK	((0x3fU) << 16)
    #define FLD_HDMI_SCL0_MASK	((0x3fU) << 24)    
#endif
    #define FLD_HDMI_SDA1_MASK	((0x3fU) << 16)
    #define FLD_HDMI_SCL1_MASK	((0x3fU) << 24)    
    #define FLD_HDMI_SDA2_MASK	((0x3fU) << 16)
    #define FLD_HDMI_SCL2_MASK	((0x3fU) << 24)    

#define FLD_HDMI_SDA_MASK  ((0x3fU) << 16)
#define FLD_HDMI_SCL_MASK  ((0x3fU) << 24) 

    #define FLD_WDT0_RST (1U << 24)
    #define FLD_WDT1_RST (1U << 25)
    #define FLD_WDT2_RST (1U << 26)    
#ifdef CC_PDWNCONOFF_PATCH
    #define FLD_WDTALL_RST (FLD_WDT0_RST | FLD_WDT1_RST)    
#else
    #define FLD_WDTALL_RST (FLD_WDT0_RST | FLD_WDT1_RST | FLD_WDT2_RST)    
#endif
      
    #define PDWNC_WAK_GPIOX(x)    (1U << (x))             // Set GPIO wakeup bit.
    #define PDWNC_GPIO_WAKEN(x)   (((x) & 0xff) << 0)     // GPIO wakeup enable bits
    #define PDWNC_WAK_ALLGPIO     GPIO_WAKEN(0xff)
    #define PDWNC_IRRX_WAKEN(x)   (((x) & 1) << 8)        // IRRX wakeup enable bits
    #define PDWNC_VGA_WAKEN(x)    (((x) & 1) << 9)        // VGA Sync wakeup enable bits
    #define PDWNC_RTC_WAKEN(x)    (((x) & 1) << 15)       // RTC wakeup enable bits
    #define PDWNC_UART_WAKEN(x)    (((x) & 1) << 16)       // RTC wakeup enable bits
    #define PDWNC_WAK_SRVADX(x)   (1U << (x))             // Set ServoADC wakeup bit.
    
    #define PDWNC_SRVAD_WAKEN(x)  (((x) & 0xff) << 20)    // Servo ADC wakeup enable bits    
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
	#define PDWNC_WAK_ALLSRVAD    PDWNC_SRVAD_WAKEN(0xff)
#else
	#define PDWNC_WAK_ALLSRVAD	  PDWNC_SRVAD_WAKEN(0x3f)
#endif
    #define PDWNC_SRVCH_EN_SRVCFG     (0xffU)    // Servo ADC channel enable bits.
    #define PDWNC_SRVCH_EN_CH(x)      ((1 << (x)) & SRVCH_EN_SRVCFG)
#define PDWNC_SRVIO_GP_O(x)     (1U << (x))  // Servo ADC PAD used to be GPIO                      
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
		#define ADC2GPIO_CH_ID_MAX	7
		#define ADC2GPIO_CH_ID_MIN	4
#else
		#define ADC2GPIO_CH_ID_MAX	5    
		#define ADC2GPIO_CH_ID_MIN	2
#endif
    #define PDWNC_PAD_SRVIO_IN(x)     (0x1U << (x))        
    #define PDWNC_SRVIO_GP(x)     (1U << (x))  // Servo ADC PAD used to be GPIO            
#define REG_RW_CLKPWD       0x0000      // Clock Power Down Register
		#define PWRDWN_CLOCK_LOCK (1U << 6)
//#define REG_RW_PWDIOCFG     0x009c      // Power Down IO Configuration Register
    #define GPIO0_POL       (1U << 0)   // GPIO 0 wake-up polarity, 0 active low
    #define GPIO1_POL       (1U << 1)   // GPIO 1 wake-up polarity, 0 active low
    #define GPIO2_POL       (1U << 2)   // GPIO 2 wake-up polarity, 0 active low
    #define GPIO3_POL       (1U << 3)   // GPIO 3 wake-up polarity, 0 active low
    #define GPIO4_POL       (1U << 4)   // GPIO 4 wake-up polarity, 0 active low
    #define GPIO5_POL       (1U << 5)   // GPIO 5 wake-up polarity, 0 active low
    #define GPIO6_POL       (1U << 6)   // GPIO 6 wake-up polarity, 0 active low
    #define GPIO7_POL       (1U << 7)   // GPIO 7 wake-up polarity, 0 active low
    #define GPIOX_POL(x)    (1U << (x)) // GPIO x wake-up polarity
    #define GPIOALL_POL     (0xffU)     // GPIO all wake-up polarity, all active low
    #define GPIO_DEGCNT(x)  (((x) & 0x0ff) << 8)    // debounce period default 60ms   

//#define REG_RW_WATCHDOG_TMR 0x0104      // Watchdog Timer Register
//#define REG_RW_PDENTRY_CODE 0x0124      // Power Down Entry Code Register, 0x5381->0x1835 to power down
    #define PWRDWN_CODE1    0x14
    #define PWRDWN_CODE2    0x04
//#define REG_RW_AUX_DATA 0x180//mt5387
// PDWNC Interrupt status/enable/clear registers
//#define REG_RO_PDWNC_INTST  0x0040      // PDWNC Interrupt Status Register

	#define PINTNO_OPCTRL0	  0 		// OPCTRL 0 interrupt number
	#define PINTNO_OPCTRL1	  1 		// OPCTRL 1 interrupt number
	#define PINTNO_OPCTRL2	  2 		// OPCRTL 2 interrupt number
#if defined(CC_MT5368) || defined(CC_MT5396) 
	#define PINTNO_OPCTRL3	  3 		// OPCTRL 3 interrupt number
	#define PINTNO_OPCTRL4    4			// OPCTRL 4 interrupt number
	#define PINTNO_OPCTRL5    5			// OPCTRL 5 interrupt number
	#define PINTNO_OPCTRL6    6			// OPCTRL 6 interrupt number
	#define PINTNO_OPCTRL7    7			// OPCTRL 7 interrupt number
#elif defined(CC_MT5389)
	#define PINTNO_VGA_SDA	  3 		   // VGA SDA interrupt number
	#define PINTNO_VGA_SCL	  4 		   // VGA SCL interrupt number
	#define PINTNO_OIRI 	  5 		   // OIRI interrupt number
	#define PINTNO_SRVAD4	  6 		   // SRVAD4 interrupt number
	#define PINTNO_SRVAD5	  7 		 // SRVAD5 interrupt number
#elif defined(CC_MT5365) || defined(CC_MT5395)
	#define PINTNO_OPCTRL3	  3 		// OPCTRL 3 interrupt number
	#define PINTNO_OPCTRL4	  4 		  // OPCTRL 3 interrupt number
	#define PINTNO_OPCTRL5	  5 		  // OPCTRL 3 interrupt number	
	#define PINTNO_PWR5V	  6 		  // OPCTRL 5 interrupt number
	#define PINTNO_HDMI_HPD	  7			// OPCTRL 7 interrupt number	
#else
	#define PINTNO_OPCTRL3	  3 		// OPCTRL 3 interrupt number
   	#define PINTNO_HDMI_HPD2  4           // OPCTRL 4 interrupt number
    #define PINTNO_PWR5V_2    5           // OPCTRL 5 interrupt number
    #define PINTNO_HDMI_SDA2  6           // OPCTRL 6 interrupt number
    #define PINTNO_HDMI_SCL2  7           // OPCTRL 7 interrupt number    
#endif
	#define PINTNO_SIF		  8 		// SIFM0 interrupt number	
	#define PINTNO_UART_DBG	  9
//#if defined(CC_MT5387) && !defined(CC_MT5363) && !defined(CC_MT5301A)/* 5387 only */   
//#else
    #define PINTNO_AVLINK   10          // AVLink interrupt number
//#endif
    #define PINTNO_CEC      11          // CEC interrupt number
    #define PINTNO_RTC      12          // RTC interrupt number
    #define PINTNO_IRRX     13          // IRRX interrupt number
    #define PINTNO_ARM 	    14          // ARM interrupt number
    #define PINTNO_AUX      15           // SRVAD 0 interrupt number
#if defined(CC_MT5368) 	
    #define PINTNO_VGA_SDA   16            // VGA SDA interrupt number
    #define PINTNO_VGA_SCL   17            // VGA SCL interrupt number
    #define PINTNO_LED_PWM0  18            // LED_PWM0 interrupt number
    #define PINTNO_LED_PWM1  19            // LED_PWM1 interrupt number
    #define PINTNO_SIF1      20            // SIF1 interrupt number
    #define PINTNO_DDCCI     21            // DDCCI interrupt number
    #define PINTNO_AVLINK1   22            // AVLINK1 interrupt number
    #define PINTNO_CEC1      23            // CEC1 interrupt number
    #define PINTNO_OIRI      25            // OIRI interrupt number
    #define PINTNO_STB_SDA   26          // STB_SDA interrupt number
    #define PINTNO_STB_SCL   27          // STB_SCL interrupt number
    #define PINTNO_SRVAD4    28            // SRVAD4 interrupt number
    #define PINTNO_SRVAD5    29          // SRVAD5 interrupt number
    #define PINTNO_SRVAD6    30            // SRVAD6 interrupt number
    #define PINTNO_SRVAD7    31            // SRVAD7 interrupt number
#elif defined(CC_MT5389)
	#define PINTNO_DDCCI	 21 		   // DDCCI interrupt number
	#define PINTNO_AVLINK1	 22 		   // AVLINK1 interrupt number
	#define PINTNO_CEC1 	 23 		   // CEC1 interrupt number
#elif defined(CC_MT5396)
	#define PINTNO_SIF1 	 20 		   // SIF1 interrupt number
	#define PINTNO_DDCCI	 21 		   // DDCCI interrupt number
	#define PINTNO_AVLINK1	 22 		   // AVLINK1 interrupt number	
	#define PINTNO_CEC1 	 23 		   // CEC1 interrupt number
	#define PINTNO_OIRI 	 25 		   // OIRI interrupt number
	#define PINTNO_SRVAD4	 28 		   // SRVAD4 interrupt number	
	#define PINTNO_CEC1 	 23 		   // CEC1 interrupt number	
	#define PINTNO_SRVAD4	 28 		   // SRVAD4 interrupt number
	#define PINTNO_SRVAD5	 29 		 // SRVAD5 interrupt number
	#define PINTNO_SRVAD6	 30 		   // SRVAD6 interrupt number
	#define PINTNO_SRVAD7	 31 		   // SRVAD7 interrupt number
#elif defined(CC_MT5365) || defined(CC_MT5395)
	#define PINTNO_SIFSR	16			// SRVAD 2 interrupt number, srvad0,1 can't be used as gpio
	#define PINTNO_SIFSW	17			// SRVAD 2 interrupt number
	#define PINTNO_SRVAD2	18			// SRVAD 2 interrupt number
	#define PINTNO_SRVAD3	19			// SRVAD 3 interrupt number
	#define PINTNO_SRVAD4	20			// SRVAD 4 interrupt number
	#define PINTNO_SRVAD5	21			// SRVAD 5 interrupt number
	#define PINTNO_AVLINK1  22 		 // OPWR0 interrupt number
	#define PINTNO_CEC1		23			// OPRW1 interrupt number
	#define PINTNO_HDMI_SDA 24 		 // OPWR2 interrupt number
	#define PINTNO_HDMI_CEC	25			// T8032 interrupt number
	#define PINTNO_VGA_SCL  26 		 // OPWR0 interrupt number
	#define PINTNO_VGA_SDA  27 		 // OPWR0 interrupt number
	#define PINTNO_UORX		28			// OPRW1 interrupt number
	#define PINTNO_UOTX 	29 		 // OPWR2 interrupt number
	#define PINTNO_OIRI 	30			// OPWR2 interrupt number
	#define PINTNO_HDMI_SCL	31			// OPWR2 interrupt number	
#else
    #define PINTNO_SRVAD0   18          // SRVAD 2 interrupt number, srvad0,1 can't be used as gpio
    #define PINTNO_SRVAD1   19          // SRVAD 2 interrupt number
    #define PINTNO_SRVAD2   20          // SRVAD 2 interrupt number
    #define PINTNO_SRVAD3   21          // SRVAD 3 interrupt number
    #define PINTNO_SRVAD4   22          // SRVAD 4 interrupt number
    #define PINTNO_SRVAD5   23          // SRVAD 5 interrupt number
    #define PINTNO_HDMI_HPD1 24          // OPWR0 interrupt number
    #define PINTNO_OPWR5V_1 25          // OPRW1 interrupt number
    #define PINTNO_HDMI_SDA1 26          // OPWR2 interrupt number
    #define PINTNO_HDMI_SCL1    27          // T8032 interrupt number
    #define PINTNO_HDMI_HPD0 28          // OPWR0 interrupt number
    #define PINTNO_OPWR5V_0 29          // OPRW1 interrupt number
    #define PINTNO_HDMI_SDA0 30          // OPWR2 interrupt number
    #define PINTNO_OIRI 31          // OPWR2 interrupt number
#endif    
#define MAX_PINT_ID 	31			// Maximum value of PDWNC interrupt id

    //#define IS_PINTNO_GPIO(x)     ((x <= PINTNO_GPIO15)?1:0)          // Maximum value of PDWNC interrupt id        
    #define _PINT(v)        (1U << (v))
#if defined(CC_MT5368)
	#define PINT_OPCTRL0	  _PINT(PINTNO_OPCTRL0) 	// OPCTRL 0 interrupt status
	#define PINT_OPCTRL1	  _PINT(PINTNO_OPCTRL1) 	// OPCTRL 1 interrupt status
	#define PINT_OPCTRL2	  _PINT(PINTNO_OPCTRL2) 	// OPCTRL 2 interrupt status
	#define PINT_OPCTRL3	  _PINT(PINTNO_OPCTRL3) 	// OPCTRL 3 interrupt status
	#define PINT_OPCTRL4	  _PINT(PINTNO_OPCTRL4) 	// OPCTRL 3 interrupt status
	#define PINT_OPCTRL5	  _PINT(PINTNO_OPCTRL5) 	// OPCTRL 4 interrupt status
	#define PINT_OPCTRL6	  _PINT(PINTNO_OPCTRL6) 	// OPCTRL 6 interrupt status
	#define PINT_OPCTRL7	  _PINT(PINTNO_OPCTRL7) 	// OPCTRL 7 interrupt status
	#define PINT_SIF		  _PINT(PINTNO_SIF)  // UART debug interrupt status
	#define PINT_UART_DBG	  _PINT(PINTNO_UART_DBG) 	  // SIF interrupt status
	#define PINT_AVLINK 	_PINT(PINTNO_AVLINK)		// CEC interrupt status 
	#define PINT_CEC		_PINT(PINTNO_CEC)		// CEC interrupt status
	#define PINT_RTC		_PINT(PINTNO_RTC)		// RTC interrupt status
	#define PINT_IRRX		_PINT(PINTNO_IRRX)		// IRRX interrupt status bit
	#define PINT_ARM		 _PINT(PINTNO_ARM)// SRVAD 0 interrupt status
	#define PINT_VGA_SDA		   _PINT(PINTNO_VGA_SDA)		// VGA_SDA debug interrupt status
	#define PINT_VGA_SCL		   _PINT(PINTNO_VGA_SCL)		// VGA_SCL debug interrupt status
	#define PINT_LED_PWM0		   _PINT(PINTNO_LED_PWM0)		// LED_PWM0 debug interrupt status
	#define PINT_LED_PWM1		   _PINT(PINTNO_LED_PWM1)		// LED_PWM1 debug interrupt status
	#define PINT_SIF1		   _PINT(PINTNO_SIF1)		// SIF1 debug interrupt status
	#define PINT_DDCCI	   _PINT(PINTNO_DDCCI)// SRVAD 5 interrupt status  
	#define PINT_AVLINK1	   _PINT(PINTNO_AVLINK1)// SRVAD 5 interrupt status  
	#define PINT_CEC1	_PINT(PINTNO_CEC1)	// OPWR0 interrupt status
	#define PINT_OIRI	_PINT(PINTNO_OIRI)	// OPWR2 interrupt status		 
	#define PINT_STB_SDA	_PINT(PINTNO_STB_SDA)	// STB_SDA interrupt status		 
	#define PINT_STB_SCL	_PINT(PINTNO_STB_SCL)	// STB_SCL interrupt status		 
	#define PINT_SRVAD4 	_PINT(PINTNO_SRVAD4)// SRVAD 3 interrupt status
	#define PINT_SRVAD5 	_PINT(PINTNO_SRVAD5)// SRVAD 4 interrupt status
	#define PINT_SRVAD6 	_PINT(PINTNO_SRVAD6)		// SRVAD 6 interrupt status
	#define PINT_SRVAD7 	_PINT(PINTNO_SRVAD7)		// SRVAD 7 interrupt status	
#elif defined(CC_MT5396)	
	#define PINT_OPCTRL0	  _PINT(PINTNO_OPCTRL0) 	// OPCTRL 0 interrupt status
	#define PINT_OPCTRL1	  _PINT(PINTNO_OPCTRL1) 	// OPCTRL 1 interrupt status
	#define PINT_OPCTRL2	  _PINT(PINTNO_OPCTRL2) 	// OPCTRL 2 interrupt status
	#define PINT_OPCTRL3	  _PINT(PINTNO_OPCTRL3) 	// OPCTRL 3 interrupt status
	#define PINT_OPCTRL4	  _PINT(PINTNO_OPCTRL4) 	// OPCTRL 3 interrupt status
	#define PINT_OPCTRL5	  _PINT(PINTNO_OPCTRL5) 	// OPCTRL 4 interrupt status
	#define PINT_OPCTRL6	  _PINT(PINTNO_OPCTRL6) 	// OPCTRL 6 interrupt status
	#define PINT_OPCTRL7	  _PINT(PINTNO_OPCTRL7) 	// OPCTRL 7 interrupt status
	#define PINT_SIF		  _PINT(PINTNO_SIF)  // UART debug interrupt status
	#define PINT_UART_DBG	  _PINT(PINTNO_UART_DBG)	  // SIF interrupt status
	#define PINT_AVLINK 	_PINT(PINTNO_AVLINK)		// CEC interrupt status 
	#define PINT_CEC		_PINT(PINTNO_CEC)		// CEC interrupt status
	#define PINT_RTC		_PINT(PINTNO_RTC)		// RTC interrupt status
	#define PINT_IRRX		_PINT(PINTNO_IRRX)		// IRRX interrupt status bit
	#define PINT_ARM		 _PINT(PINTNO_ARM)// SRVAD 0 interrupt status
	#define PINT_SIF1		   _PINT(PINTNO_SIF1)		// SIF1 debug interrupt status
	#define PINT_DDCCI	   _PINT(PINTNO_DDCCI)// SRVAD 5 interrupt status  
	#define PINT_AVLINK1	   _PINT(PINTNO_AVLINK1)// SRVAD 5 interrupt status  
	#define PINT_CEC1	_PINT(PINTNO_CEC1)	// OPWR0 interrupt status
	#define PINT_SRVAD4 	_PINT(PINTNO_SRVAD4)// SRVAD 3 interrupt status
	#define PINT_SRVAD5 	_PINT(PINTNO_SRVAD5)// SRVAD 4 interrupt status
	#define PINT_SRVAD6 	_PINT(PINTNO_SRVAD6)		// SRVAD 6 interrupt status
	#define PINT_SRVAD7 	_PINT(PINTNO_SRVAD7)		// SRVAD 7 interrupt status 
#elif defined(CC_MT5389)
	#define PINT_OPCTRL0	  _PINT(PINTNO_OPCTRL0) 	// OPCTRL 0 interrupt status
	#define PINT_OPCTRL1	  _PINT(PINTNO_OPCTRL1) 	// OPCTRL 1 interrupt status
	#define PINT_OPCTRL2	  _PINT(PINTNO_OPCTRL2) 	// OPCTRL 2 interrupt status
	#define PINT_VGA_SDA		   _PINT(PINTNO_VGA_SDA)		// VGA_SDA debug interrupt status
	#define PINT_VGA_SCL		   _PINT(PINTNO_VGA_SCL)		// VGA_SCL debug interrupt status
	#define PINT_OIRI	_PINT(PINTNO_OIRI)	// OPWR2 interrupt status		 
	#define PINT_SRVAD4 	_PINT(PINTNO_SRVAD4)// SRVAD 3 interrupt status
	#define PINT_SRVAD5 	_PINT(PINTNO_SRVAD5)// SRVAD 4 interrupt status
	#define PINT_SIF		  _PINT(PINTNO_SIF)  // UART debug interrupt status
	#define PINT_UART_DBG	  _PINT(PINTNO_UART_DBG)	  // SIF interrupt status
	#define PINT_AVLINK 	_PINT(PINTNO_AVLINK)		// CEC interrupt status 
	#define PINT_CEC		_PINT(PINTNO_CEC)		// CEC interrupt status
	#define PINT_RTC		_PINT(PINTNO_RTC)		// RTC interrupt status
	#define PINT_IRRX		_PINT(PINTNO_IRRX)		// IRRX interrupt status bit
	#define PINT_ARM		 _PINT(PINTNO_ARM)// SRVAD 0 interrupt status
	#define PINT_DDCCI	   _PINT(PINTNO_DDCCI)// SRVAD 5 interrupt status  
	#define PINT_AVLINK1	   _PINT(PINTNO_AVLINK1)// SRVAD 5 interrupt status  
	#define PINT_CEC1	_PINT(PINTNO_CEC1)	// OPWR0 interrupt status
#elif defined(CC_MT5365) || defined(CC_MT5395)
	#define PINT_OPCTRL0	  _PINT(PINTNO_OPCTRL0) 	// OPCTRL 0 interrupt status
	#define PINT_OPCTRL1	  _PINT(PINTNO_OPCTRL1) 	// OPCTRL 1 interrupt status
	#define PINT_OPCTRL2	  _PINT(PINTNO_OPCTRL2) 	// OPCTRL 2 interrupt status
	#define PINT_OPCTRL3	  _PINT(PINTNO_OPCTRL3) 	// OPCTRL 3 interrupt status
	#define PINT_OPCTRL4	  _PINT(PINTNO_OPCTRL4) 	// OPCTRL 3 interrupt status
	#define PINT_OPCTRL5	  _PINT(PINTNO_OPCTRL5) 	// OPCTRL 4 interrupt status
	#define PINT_PWR5V		  _PINT(PINTNO_PWR5V) 	// OPCTRL 5 interrupt status
	#define PINT_HDMI_HPD     _PINT(PINTNO_HDMI_HPD) 	// OPCTRL 6 interrupt status
	#define PINT_SIF   		  _PINT(PINTNO_SIF)  // UART debug interrupt status
	#define PINT_UART_DBG		 _PINT(PINTNO_UART_DBG) 	  // SIF interrupt status
	#define PINT_CEC		_PINT(PINTNO_CEC)		// CEC interrupt status
	#define PINT_AVLINK		_PINT(PINTNO_AVLINK)		// CEC interrupt status	
	#define PINT_RTC		_PINT(PINTNO_RTC)		// RTC interrupt status
	#define PINT_IRRX		_PINT(PINTNO_IRRX)		// IRRX interrupt status bit
	#define PINT_ARM	 _PINT(PINTNO_ARM)// SRVAD 0 interrupt status
	#define PINT_SRVAD2 	_PINT(PINTNO_SRVAD2)// SRVAD 1 interrupt status
	#define PINT_SRVAD3 	_PINT(PINTNO_SRVAD3)// SRVAD 2 interrupt status
	#define PINT_SRVAD4 	_PINT(PINTNO_SRVAD4)// SRVAD 3 interrupt status
	#define PINT_SRVAD5 	_PINT(PINTNO_SRVAD5)// SRVAD 4 interrupt status
	#define PINT_AVLINK1	   _PINT(PINTNO_AVLINK1)// SRVAD 5 interrupt status	 
	#define PINT_CEC1	_PINT(PINTNO_CEC1)	// OPWR0 interrupt status
	#define PINT_OIRI	_PINT(PINTNO_OIRI)	// OPWR2 interrupt status
#else
    #define PINT_OPCTRL0      _PINT(PINTNO_OPCTRL0)     // OPCTRL 0 interrupt status
    #define PINT_OPCTRL1      _PINT(PINTNO_OPCTRL1)     // OPCTRL 1 interrupt status
    #define PINT_OPCTRL2      _PINT(PINTNO_OPCTRL2)     // OPCTRL 2 interrupt status
    #define PINT_OPCTRL3      _PINT(PINTNO_OPCTRL3)     // OPCTRL 3 interrupt status
    #define PINT_HDMI_HPD2      _PINT(PINTNO_HDMI_HPD2)     // OPCTRL 4 interrupt status
    #define PINT_PWR5V_2      _PINT(PINTNO_PWR5V_2)     // OPCTRL 5 interrupt status
    #define PINT_HDMI_SDA2      _PINT(PINTNO_HDMI_SDA2)     // OPCTRL 6 interrupt status
    #define PINT_HDMI_SCL2      _PINT(PINTNO_HDMI_SCL2)     // OPCTRL 7 interrupt status 
    #define PINT_SIF   _PINT(PINTNO_SIF)  // UART debug interrupt status
    #define PINT_UART_DBG        _PINT(PINTNO_UART_DBG)       // SIF interrupt status
    #define PINT_CEC        _PINT(PINTNO_CEC)       // CEC interrupt status
	#define PINT_AVLINK		_PINT(PINTNO_AVLINK)		// CEC interrupt status	    
    #define PINT_RTC        _PINT(PINTNO_RTC)       // RTC interrupt status
    #define PINT_IRRX       _PINT(PINTNO_IRRX)      // IRRX interrupt status bit
    #define PINT_ARM     _PINT(PINTNO_ARM)// SRVAD 0 interrupt status
    #define PINT_SRVAD2     _PINT(PINTNO_SRVAD2)// SRVAD 1 interrupt status
    #define PINT_SRVAD3     _PINT(PINTNO_SRVAD3)// SRVAD 2 interrupt status
    #define PINT_SRVAD4     _PINT(PINTNO_SRVAD4)// SRVAD 3 interrupt status
    #define PINT_SRVAD5     _PINT(PINTNO_SRVAD5)// SRVAD 4 interrupt status
    #define PINT_HDMI_HPD1     _PINT(PINTNO_HDMI_HPD1)// SRVAD 5 interrupt status    
    #define PINT_OPWR5V_1   _PINT(PINTNO_OPWR5V_1)  // OPWR0 interrupt status
    #define PINT_HDMI_SDA1   _PINT(PINTNO_HDMI_SDA1)  // OPWR1 interrupt status
    #define PINT_HDMI_SCL1   _PINT(PINTNO_HDMI_SCL1)  // OPWR2 interrupt status    
    
    #define PINT_HDMI_HPD0   _PINT(PINTNO_HDMI_HPD0)  // OPWR2 interrupt status    
    #define PINT_OPWR5V_0   _PINT(PINTNO_OPWR5V_0)  // OPWR2 interrupt status    
    #define PINT_HDMI_SDA0   _PINT(PINTNO_HDMI_SDA0)  // OPWR2 interrupt status        
    #define PINT_OIRI   _PINT(PINTNO_OIRI)  // OPWR2 interrupt status        
#endif    
//#define REG_RW_PDWNC_INTEN  0x0044      // PDWNC Interrupt Enable Register
    #define OPCTRL_INTEN(x) (0x1U << (x))
    #define SRVAD_INTEN(x)  (0x1U << (18 + x)) 
	#define PDWNC_INTEN(x)	vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) | _PINT(x)) 
	#define PDWNC_INTDIS(x) vIO32Write4B(PDWNC_ARM_INTEN, u4IO32Read4B(PDWNC_ARM_INTEN) & (~_PINT(x))) 	
#if defined(CC_MT5368)
    #define REG_RW_PDWNC_INTEN_GPIO_MASK 0xfe0f00ff 
#elif defined(CC_MT5396)
	#define REG_RW_PDWNC_INTEN_GPIO_MASK 0xfc0f00ff
#elif defined(CC_MT5389)
	#define REG_RW_PDWNC_INTEN_GPIO_MASK 0x000000ff
#elif defined(CC_MT5365) || defined(CC_MT5395) 
    #define REG_RW_PDWNC_INTEN_GPIO_MASK 0xff3c00ff
#else
    #define REG_RW_PDWNC_INTEN_GPIO_MASK 0xff0000ff
#endif    

#define IS_PINTNO_INTABLE_GPIO(x)	  ((REG_RW_PDWNC_INTEN_GPIO_MASK & (0x1UL << (x))) ? 1 : 0) //

//#define REG_WO_PDWNC_INTCLR 0x0048      // PDWNC Interrupt Clear Register    
//#define REG_RW_IOPADEN      0x0080                  // IO Pad Output Enable Register
    #define PAD_GPIO_OUT_EN     (0xffU << 0)            // GPIO output enable
    #define PAD_SERVOADC_EN_ALL (0x3fU)        // SERVO ADC PAD output enable
    #define PAD_SERVOADC_EN(x)  (1U << (x))    // SERVO ADC PAD output enable    
    #define PAD_UR0_TX_EN       (1U << 15)              // UART 0 TX PAD output enable    
//#define REG_RW_UP_ADDR 0x170
//#define REG_RW_UP_DATA 0x174
//#define REG_RW_UP_CFG 0x178
//    #define UART_EN (1U << 24)
//    #define FAST_EN (1U << 20)
//    #define ENGINE_EN (1U << 16)
//    #define CEC_EN (1U << 12)

    #define RAM_SPL_SEL (3U << 6)    
    #define RAM_SP_SEL (3U << 4)          
    #define XDATA_ACC (1U << 2)    
    #define VIR_EN (1U << 1)        
    #define T8032_RST (1U)    
//    #define SET_UART_EN(x) ((x & 1U) << 24)
//    #define SET_FAST_EN(x) ((x & 1U) << 20)
//    #define SET_ENGINE_EN(x) ((x & 1U) << 16)
//    #define SET_CEC_EN(x) ((x & 1U) << 12)
    #define SET_VIR_EN(x) ((x & 1U) << 1)    
    #define SET_RAM_SPL_SEL(x) ((x & 3U) << 6)    
    #define SET_XDATA_ACC(x) ((x & 1U) << 2)
    #define SET_RAM_SP_SEL(x) ((x & 3U) << 4)          
    #define SET_T8032_RST(x) (x & 1U)    
//#define REG_RW_WATCHDOG_EN  0x0100      // Watchdog Timer Control Register
    #define WDT0_RST (1U << 24)
    #define WDT1_RST (1U << 25)
    #define WDT2_RST (1U << 26)    
    #define WDTALL_RST (WDT0_RST | WDT1_RST | WDT2_RST)    
//#define REG_RW_PDWAKE_EN    0x0120      // Power Down Wakeup Enable Register
    #define WAK_GPIO0       (1U << 0)
    #define WAK_GPIO1       (1U << 1)
    #define WAK_GPIO2       (1U << 2)
    #define WAK_GPIO3       (1U << 3)
    #define WAK_GPIO4       (1U << 4)
    #define WAK_GPIO5       (1U << 5)
    #define WAK_GPIO6       (1U << 6)
    #define WAK_GPIO7       (1U << 7)
    #define WAK_IRRX        (1U << 8)
    #define WAK_VGA         (1U << 9)
    #define WAK_RTC         (1U << 15)    
    #define WAK_UART         (1U << 16)        
    #define WAK_T8032         (1U << 17)
    #define WAK_HDMI         (1U << 18)    
    #define WAK_UNOR        (1U << 19)    
    #define WAK_SRVAD0        (1U << 20)    
    #define WAK_SRVAD1        (1U << 21)        
    #define WAK_SRVAD2        (1U << 22)    
    #define WAK_SRVAD3        (1U << 23)            
    #define WAK_SRVAD4        (1U << 24)            
    #define WAK_SRVAD5        (1U << 25)
	#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
	#define WAK_SRVAD6        (1U << 26)            
    #define WAK_SRVAD7        (1U << 27)
	#endif
    #define WAK_GPIOX(x)    (1U << (x))             // Set GPIO wakeup bit.
    #define GPIO_WAKEN(x)   (((x) & 0xff) << 0)     // GPIO wakeup enable bits
    #define WAK_ALLGPIO     GPIO_WAKEN(0xff)
    #define IRRX_WAKEN(x)   (((x) & 1) << 8)        // IRRX wakeup enable bits
    #define VGA_WAKEN(x)    (((x) & 1) << 9)        // VGA Sync wakeup enable bits
    #define RTC_WAKEN(x)    (((x) & 1) << 15)       // RTC wakeup enable bits
    #define UART_WAKEN(x)    (((x) & 1) << 16)       // RTC wakeup enable bits
    #define WAK_SRVADX(x)   (1U << (x))             // Set ServoADC wakeup bit. 
    
    
	#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)    	
	#define SRVAD_WAKEN(x)  (((x) & 0xff) << 20)    // Servo ADC wakeup enable bits
	#define WAK_ALLSRVAD    SRVAD_WAKEN(0xff) 
	#else	
	#define SRVAD_WAKEN(x)	(((x) & 0x3f) << 20)	// Servo ADC wakeup enable bits
    #define WAK_ALLSRVAD    SRVAD_WAKEN(0x3f)    
	#endif
       

    
    
//#define REG_RO_PDWAKE_STAT  0x0128      // Power Down Wakeup Status Register
    #define WAK_RS232       (1U << 16)

// IRRX readonly registers
//#define IRRX_COUNT_HIGH_REG             0x0200
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    #define IRRX_CH_BITCNT_MASK         0xff
#else
	#define IRRX_CH_BITCNT_MASK 		0x3f
#endif
    #define IRRX_CH_BITCNT_BITSFT       0
    #define IRRX_CH_1ST_PULSE_MASK      0x0000ff00
    #define IRRX_CH_1ST_PULSE_BITSFT    8
    #define IRRX_CH_2ND_PULSE_MASK      0x00ff0000
    #define IRRX_CH_2ND_PULSE_BITSFT    16
    #define IRRX_CH_3RD_PULSE_MASK      0xff000000
    #define IRRX_CH_3RD_PULSE_BITSFT    24
/*    
#define IRRX_COUNT_MID_REG              0x0204
#define IRRX_COUNT_LOW_REG              0x0208

// IRRX read/write configuration registers
#define IRRX_CONFIG_HIGH_REG    0x020c
    #define IRRX_CH_HWIR        (1U << 0)
    #define IRRX_CH_IRI         (1U << 1)
    #define IRRX_CH_RC5         (1U << 2)
    #define IRRX_CH_RC5_1ST     (1U << 3)
    #define IRRX_CH_ORDINV      (1U << 4)
    #define IRRX_CH_IGSYN       (1U << 5)
    #define IRRX_CH_DISCL       (1U << 6)
    #define IRRX_CH_DISCH       (1U << 7)
    #define IRRX_CH_END_7       (0U << 8)
    #define IRRX_CH_END_15      (1U << 8)
    #define IRRX_CH_END_23      (2U << 8)
    #define IRRX_CH_END_31      (3U << 8)
    #define IRRX_CH_END_39      (4U << 8)
    #define IRRX_CH_END_47      (5U << 8)
    #define IRRX_CH_END_55      (6U << 8)
    #define IRRX_CH_END_63      (7U << 8)
    #define IRRX_CH_IGB0        (1U << 14)   // Ignore 0 bit count IR pulse to reduce noise
    #define IRRX_CH_DISPD       (1U << 15)   // IR state machine clear disable at deep power down
#define IRRX_CONFIG_LOW_REG     0x0210
#define IRRX_THRESHOLD_REG      0x0214
    #define IRRX_GD_DEL_MASK    0x0300
    #define IRRX_THRESHOLD_MASK 0x7f
#define IRRX_CLEAR_BUSY_REG     0x0218
    #define IRRX_CLR_BSY_IR     0x01


// IR Expect registers
#define IREXP_EN_REG            0x021c      // IR Expectation Enable Register
*/
    #define IREXP_EN(x)         (1U << x)   // Enable IR Expect value register x
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    #define IREXP_BIT_EN        (1U << 16) // Bit count expectation enable bit
#else
	#define IREXP_BIT_EN		(1U << 8)	// Bit count expectation enable bit
#endif
// 5381 New feature: H/W Power down
    #define IREXP_HWPD_EN       (1U << 9)
    #define IREXP_HWPD_EXPEN0   (1U << 10)
    #define IREXP_HWPD_EXPEN1   (1U << 11)

/*    
#define IREXP_M_BITMASK_REG     0x0220      // IR Expect Value Bit Mask Register
#define IREXP_L_BITMASK_REG     0x0224      // IR Expect Value Bit Mask Register
#define IREXP_M0_REG            0x0228      // IRM Expect Value Register 0
#define IREXP_L0_REG            0x022c      // IRL Expect Value Register 0
#define IREXP_M1_REG            0x0230      // IRM Expect Value Register 1
#define IREXP_L1_REG            0x0234      // IRL Expect Value Register 1
#define IREXP_M2_REG            0x0238      // IRM Expect Value Register 2
#define IREXP_L2_REG            0x023c      // IRL Expect Value Register 2
#define IREXP_M3_REG            0x0240      // IRM Expect Value Register 3
#define IREXP_L3_REG            0x0244      // IRL Expect Value Register 3
#define IREXP_M4_REG            0x0248      // IRM Expect Value Register 4
#define IREXP_L4_REG            0x024c      // IRL Expect Value Register 4
#define IREXP_M5_REG            0x0250      // IRM Expect Value Register 5
#define IREXP_L5_REG            0x0254      // IRL Expect Value Register 5
#define IREXP_M6_REG            0x0258      // IRM Expect Value Register 6
#define IREXP_L6_REG            0x025c      // IRL Expect Value Register 6
#define IREXP_M7_REG            0x0260      // IRM Expect Value Register 7
#define IREXP_L7_REG            0x0264      // IRL Expect Value Register 7
#define IREXP_BITNUM_REG        0x0268      // BITCNT Expected Value Register
*/
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
#define MAX_EXP_KEY_NUM         (16)
#else
#define MAX_EXP_KEY_NUM         (8)
#endif
/*
// 5381 New feature: H/W Power down
#define IREXP_M8_REG            0x0270      // IRM Expect Value Register 6
#define IREXP_L8_REG            0x0274      // IRL Expect Value Register 6
#define IREXP_M9_REG            0x0278      // IRM Expect Value Register 7
#define IREXP_L9_REG            0x027c      // IRL Expect Value Register 7
#define IROS_DATA0              0x0290
#define IROS_DATA1              0x0294
#define IROS_DATA2              0x0298
#define IROS_DATA3              0x029c       
*/
// Servo ADC Interface Configuration Register
//#define REG_RW_SRVCFG0              0x0300      // Servo ADC Interface Configuration Register
    #define HWEN_SRVCFG         (1U << 0)   // Hardware audo trigger Servo ADC enable bit
    #define SWEN_SRVCFG         (1U << 1)   // Software trigger Servo ADC enable bit
    #define SRVCH_SEL_MASK  (0x7U << 5)   // Servo ADC channel select.     
    #define SRVCH_SEL_CH(x)  ((UINT32)(x & 0x7) << 5)   // Servo ADC channel select.  
//#define REG_RW_SRVCFG1              0x0304      // Servo ADC Interface Configuration Register 
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    #define SRVCH_EN_SRVCFG     (0xffU)    // Servo ADC channel enable bits.
#else  
	#define SRVCH_EN_SRVCFG 	(0x3fU)    // Servo ADC channel enable bits.
#endif
#define SRVCH_EN_CH(x)		((1 << (x)) & SRVCH_EN_SRVCFG)
//#define REG_RW_SRVSWT              0x030c      // Servo ADC Interface Software Trigger Register
//#define REG_RW_SRVCLR              0x0310      // Servo ADC Interface Clear Register
    #define ADCOUT_CLR          (1U << 1)   // Clear ADC output data and channel status

//#define REG_RW_SRVRAT0              0x0314      // Servo ADC Sampling Rate Register
//#define REG_RW_SRVRAT1              0x0318      // Servo ADC Sampling Rate Register 
//#define REG_RW_SRVTOT              0x031c      // Servo ADC Timeout 
//#define REG_RW_SRVTOTEN              0x0320      // Servo ADC Timeout Control
//#define REG_RW_I2CDAC              0x0324   

//#define REG_RW_WAKTOP0 0x328
//#define REG_RW_WAKBTM0 0x32c
//#define REG_RW_CHCFG0 0x330
//#define REG_RW_WAKTOP1 0x334
//#define REG_RW_WAKBTM1 0x338
//#define REG_RW_CHCFG1 0x33c
//#define REG_RW_WAKTOP2 0x340
//#define REG_RW_WAKBTM2 0x344
//#define REG_RW_CHCFG2 0x348
//#define REG_RW_WAKTOP3 0x34c
//#define REG_RW_WAKBTM3 0x350
//#define REG_RW_CHCFG3 0x354
//#define REG_RW_WAKTOP4 0x358
//#define REG_RW_WAKBTM4 0x35c
//#define REG_RW_CHCFG4 0x360
//#define REG_RW_WAKTOP5 0x364
//#define REG_RW_WAKBTM5 0x368
//#define REG_RW_CHCFG5 0x36c
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    #define CHBTM_MASK          (0xffU)// bottom voltage value
    #define GET_CHBTM(x)        ((x) & 0xffU)
    #define CHTOP_MASK          (0xffU)    // top voltage value
    #define GET_CHTOP(x)        ((x) & 0xffU)
#else
    #define CHBTM_MASK          (0x3fU)    // bottom voltage value
    #define GET_CHBTM(x)        ((x) & 0x3fU)
    #define CHTOP_MASK          (0x3fU)    // top voltage value
    #define GET_CHTOP(x)        ((x) & 0x3fU)
#endif
    #define CHBTM_VAL(x)        ((x) & CHBTM_MASK)
    #define CHTOP_VAL(x)        ((x) & CHTOP_MASK)
    #define CH_MASK             (0x7U)   // interrupt or wakeup mask
    #define CH_WAK_TOP_EN       (1U << 2)
    #define CH_WAK_MID_EN       (1U << 1)
    #define CH_WAK_BTM_EN       (1U << 0)
//#define REG_RW_SRVCST 0x370
//#define REG_RW_ADOUT0 0x374
//#define REG_RW_ADOUT1 0x378
//#define REG_RW_ADOUT2 0x37c
//#define REG_RW_ADOUT3 0x380
//#define REG_RW_ADOUT4 0x384
//#define REG_RW_ADOUT5 0x388
//#define REG_RW_ADCCFG0 0x38c
    #define SRV_OOSEN (1U << 6)
    #define SRV_PDW_B (1U << 5)    
    #define SRV_PDW_CALI (0x1fU)        
//#define REG_RW_ADCCFG1 0x390
    #define IN_BUFF_PD (1U << 1)
    #define IN_BUFF_BP (1U << 0)    
//#define REG_RW_ADPADG 0x394
//#define REG_RW_ADPADI 0x398
//#define REG_RW_ADPADO 0x39c
//#define REG_RW_ADPADE 0x3a0
//#define REG_RW_ADPADSR 0x3a4
    #define SRVIO_PD        (0x1f)
    #define SRVIO_E2        (1U << 5)
    #define SRVIO_E4        (1U << 5)
    #define SRVIO_E8        (1U << 7)
    #define SRVIO_PU        (0x1fU << 8)
    #define SRVIO_SMT       (1U << 13)
    #define SRVIO_SR        (1U << 14)
    #define SRVIO_GP_ALL    (0x3fU)       // Servo ADC PAD used to be GPIO
    #define SRVIO_GP(x)     (1U << (x))  // Servo ADC PAD used to be GPIO    
    #define SRVIO_GP_O(x)     (1U << (x))  // Servo ADC PAD used to be GPIO                  
    #define SRVAD_VREF      (0xfU << 24)        // Servo ADC Voltage reference configuration
    #define SRVAD_BYP       (1U << 28)  // SErvo ADC Bypass
    #define SRVAD_HQB       (1U << 29)  // Servo ADC HQB
    #define SRVAD_PDN       (1U << 31)  // Servo ADC Power Down bit


//----------------------------------------------------------------------------
/*
// RTC
#define REG_RW_RTC_YEAR     0x0700      // RTC Year Register
#define REG_RW_RTC_MONTH    0x0704      // RTC Month Register
#define REG_RW_RTC_DAY      0x0708      // RTC Day Register
#define REG_RW_RTC_WEEK     0x070c      // RTC Week Register
#define REG_RW_RTC_HOUR     0x0710      // RTC Hour Register
#define REG_RW_RTC_MIN      0x0714      // RTC Minute Register
#define REG_RW_RTC_SEC      0x0718      // RTC Second Register
#define REG_RW_RTC_AYEAR    0x071c      // RTC Alarm Year Register
#define REG_RW_RTC_AMONTH   0x0720      // RTC Alarm Month Register
#define REG_RW_RTC_ADAY     0x0724      // RTC Alarm Day Register
#define REG_RW_RTC_AWEEK    0x0728      // RTC Alarm Week Register
#define REG_RW_RTC_AHOUR    0x072c      // RTC Alarm Hour Register
#define REG_RW_RTC_AMIN     0x0730      // RTC Alarm Minute Register
#define REG_RW_RTC_ASEC     0x0734      // RTC Alarm Second Register
#define REG_RW_RTC_ALMINT   0x0738      // RTC Alarm Interrupt Enable Register
*/
    #define RTCALM_FULL_EN  (0xffU << 0)
    #define RTCALM_YEAR_EN  (1U << 7)   // Alarm compare YEAR enable
    #define RTCALM_MONTH_EN (1U << 6)   // Alarm compare MONTH enable
    #define RTCALM_DAY_EN   (1U << 5)   // Alarm compare DAY enable
    #define RTCALM_WEEK_EN  (1U << 4)   // Alarm compare WEEK enable
    #define RTCALM_HOUR_EN  (1U << 3)   // Alarm compare HOUR enable
    #define RTCALM_MINU_EN  (1U << 2)   // Alarm compare MINU enable
    #define RTCALM_SEC_EN   (1U << 1)   // Alarm compare SEC enable
    #define RTCALM_INTR_EN  (1U << 0)   // Alarm interrupt enable
#define REG_RW_RTC_CTRL     0x073c      // RTC Control Register
    #define RTCTRL_STOP     (1U << 0)   // Stop RTC, to save power
    #define RTCTRL_H24      (1U << 1)   // 1 for 24-hour clock mode
    #define RTCTRL_SIM      (1U << 4)   // Pass write protection
    #define RTCTRL_PASS     (1U << 6)   // Pass write protection
    #define RTCTRL_FLAG     (1U << 7)   // Clean wrong access (by writing '0')
//#define REG_RW_RTC_KEY      0x0740      // RTC Key Register - NOT USED
//#define REG_RW_RTC_LEARYEAR 0x0744      // RTC Leap Year    - NOT USED
//#define REG_RW_RTC_PROT     0x0748      // RTC Write Protection Register
    #define RTC_WRITE_PROTECT_CODE1          0x87
    #define RTC_WRITE_PROTECT_CODE2          0x78             


// Error code
#define ERR_NO_ERROR                0
#define ERR_DC_PROTECT              1
#define ERR_POWER_DOWN              2
#define ERR_I2C_STANDBY_CTRL        3
#define ERR_I2C_GENERAL             4
#define ERR_I2C_MT8280              5
#define ERR_I2C_SYS_EEPROM          6
#define ERR_I2C_TUNER               7
#define ERR_I2C_DEMOD               8   // TDA9886
#define ERR_I2C_FPGA                9
#define ERR_I2C_HDMI_SWITCH         13
#define ERR_DRAM_MT8280_1           14
#define ERR_DRAM_MT8280_2           15
#define ERR_I2C_HDMI_EDID           16
#define ERR_I2C_DFI_BOLT_ON         17
#define ERR_I2C_MT5112              18
#define ERR_I2C_PROLDIOM            19
#define ERR_I2C_BDS_BOLT_ON         21

// Power down command with T8032
#define    PDWNC_CMD_ARMT8032_UNKNOWN 0x0 
#define    PDWNC_CMD_ARMT8032_ENTER_STANDBY 0x1
#define    PDWNC_CMD_ARMT8032_ENTER_NORMAL 0x2 
#define    PDWNC_CMD_ARMT8032_QUERY_SW_VERSION 0x3
#define    PDWNC_CMD_ARMT8032_XDATA_SWITCH 0x4
#define    PDWNC_CMD_ARMT8032_READ_WAKEUP_REASON 0x5
    #define    T8032_WAKEUP_REASON_CEC_ONE_TOUCH_PLAY 0x1    
    #define    T8032_WAKEUP_REASON_COLD_REBOOT 0x2
    #define T8032_WAKEUP_REASON_DIGIT_KEY 0x6    
#define    PDWNC_CMD_ARMT8032_SET_LIGHT_GUIDE_LEVEL 0x6
#define    PDWNC_CMD_ARMT8032_ALIVE_CHECK 0x7
#define    PDWNC_CMD_ARMT8032_SET_ERROR_CODE 0x8
#define    PDWNC_CMD_ARMT8032_SET_ERROR_CODE_BLINKING 0x9
#define    PDWNC_CMD_ARMT8032_SET_MENU_LANGUAGE 0xa
#define    PDWNC_CMD_ARMT8032_ENABLE_CEC 0xb
#define    PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY 0xc
#define    PDWNC_CMD_ARMT8032_SET_LED_BLINKER 0xd
#define    PDWNC_CMD_ARMT8032_SET_IR_POWER_DOWN_ENABLE 0xe
#define    PDWNC_CMD_ARMT8032_SET_POWER_DOWN_CONTROL 0xf
#define    PDWNC_CMD_ARMT8032_CLEAR_POWER_DOWN_CONTROL 0x10
#define    PDWNC_CMD_ARMT8032_COLD_REBOOT 0x11
#define    PDWNC_CMD_ARMT8032_UART_PD_BAUDRATE 0x16
#define    PDWNC_CMD_ARMT8032_LED_BLINKING 0x17
#define    PDWNC_CMD_ARMT8032_SET_IR_FILTER_SYSTEM_CODE 0x41
#define    PDWNC_CMD_ARMT8032_ENABLE_USER_CONTROL_PRESSED 0x50
#define    PDWNC_CMD_ARMT8032_SET_VENDOR_ID 0x51
#define    PDWNC_CMD_ARMT8032_SET_CEC_VERSION 0x52
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_1 0x53
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_2 0x54
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_3 0x55
#define    PDWNC_CMD_ARMT8032_SET_OSD_NAME_4 0x56
#define    PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_1 0x57
#define    PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_2 0x58
#define    PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_3 0x59
#define    PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC	  0x5a


#define    PDWNC_CMD_ARMT8032_STOP_POWER_PROTECT   (0x65)
#define    PDWNC_CMD_ARMT8032_RESUME_POWER_PROTECT   (0x66)


#define SIF_PDWNC_TEST    1
#if SIF_PDWNC_TEST
#define    PDWNC_CMD_ARMT8032_PDSIF_READ	  0x60
#define    PDWNC_CMD_ARMT8032_PDSIF_WRITE	  0x61
#define    PDWNC_CMD_ARMT8032_PDSIF_EEP_READ	  0x62
#define    PDWNC_CMD_ARMT8032_PDSIF_EEP_WRITE	  0x63

#endif


#define    PDWNC_CMD_ARMT8032_SET_GPIO_ENABLE 0x80
#define    PDWNC_CMD_ARMT8032_SET_GPIO_OUTPUT 0x81
#define    PDWNC_CMD_ARMT8032_GET_GPIO_INPUT 0x82
#define    PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_ENABLE 0x83
#define    PDWNC_CMD_ARMT8032_SET_SERVO_GPIO_OUTPUT 0x84
#define    PDWNC_CMD_ARMT8032_GET_SERVO_GPIO_INPUT 0x85
#define    PDWNC_CMD_ARMT8032_ENGINE_SWITCH 0x86
#define    PDWNC_CMD_ARMT8032_GET_IR_OKPRESS_STATUS  0x87
#define    PDWNC_CMD_ARMT8032_SET_IR_OKPRESS_HOOKORNOT  0x88
#define    PDWNC_CMD_ARMT8032_GET_LED_STATUS 0x89
#define    PDWNC_CMD_ARMT8032_SET_LED  0x8a
#define    PDWNC_CMD_ARMT8032_SET_SERVOADC 0x8b
#define    PDWNC_CMD_ARMT8032_SETSCARTWAKEUP 0x8c

#define    PDWNC_CMD_ARMT8032_MAX 0x99


#define    PDWNC_POWER_STATE_NORMAL 0x200
#define    PDWNC_POWER_STATE_SEMI_STANDBY 0x100
#define    PDWNC_POWER_STATE_CHAGNE_MASK 0x300
#define    PDWNC_POWER_STATE_EVENT_MASK 0xFF

// Power down event
typedef enum
{
    PDWNC_POWER_DOWN_UNKNOWN = 0, 
    PDWNC_POWER_DOWN_VGA,
    PDWNC_POWER_DOWN_RTC,
    PDWNC_POWER_DOWN_KEYPAD,
    PDWNC_POWER_DOWN_IRRC,
    PDWNC_POWER_DOWN_AC_POWER = 6
} PDWNC_POWER_DOWN_REASON_T;

typedef enum
{
    PDWNC_WAKE_UP_REASON_UNKNOWN = 0,
    PDWNC_WAKE_UP_REASON_VGA,
    PDWNC_WAKE_UP_REASON_RTC,
    PDWNC_WAKE_UP_REASON_KEYPAD,
    PDWNC_WAKE_UP_REASON_IRRC,
    PDWNC_WAKE_UP_REASON_UART,
    PDWNC_WAKE_UP_REASON_AC_POWER,
    PDWNC_WAKE_UP_REASON_HDMI,
    PDWNC_WAKE_UP_REASON_UART_NORMAL,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_0,  /* Remote controller */
    PDWNC_WAKE_UP_REASON_RC_DIGIT_1,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_2,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_3,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_4,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_5,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_6,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_7,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_8,
    PDWNC_WAKE_UP_REASON_RC_DIGIT_9,
    PDWNC_WAKE_UP_REASON_RC_PRG_UP,
    PDWNC_WAKE_UP_REASON_RC_PRG_DOWN,
    PDWNC_WAKE_UP_REASON_RC_POWER,
    PDWNC_WAKE_UP_REASON_RC_INP_SRC,
    PDWNC_WAKE_UP_REASON_RC_ANALOG,
    PDWNC_WAKE_UP_REASON_RC_DIGITAL,
    PDWNC_WAKE_UP_REASON_RC_DIGITAL_ANALOG,
    PDWNC_WAKE_UP_REASON_FP_PRG_UP,   /* Front panel */
    PDWNC_WAKE_UP_REASON_FP_PRG_DOWN,
    PDWNC_WAKE_UP_REASON_FP_INP_SRC,
    PDWNC_WAKE_UP_REASON_FP_POWER,
    PDWNC_WAKE_UP_REASON_DVD,
    PDWNC_WAKE_UP_REASON_SCART,
    PDWNC_WAKE_UP_REASON_RTC_SPECIAL,
    PDWNC_WAKE_UP_REASON_WATCHDOG,        
    PDWNC_WAKE_UP_REASON_CUSTOM_1 = 61, 
    PDWNC_WAKE_UP_REASON_CUSTOM_2 = 62, 
    PDWNC_WAKE_UP_REASON_CUSTOM_3 = 63, 
    PDWNC_WAKE_UP_REASON_CUSTOM_4 = 64      
}   PDWNC_WAKE_UP_REASON_T;

typedef enum
{
    PDWNC_NORMAL = 0,
    PDWNC_WAIT_CEC_TRANSACTION,
    PDWNC_CEC_TRANSACTION_FINISH
}   PDWNC_CEC_HANDSHAKE_T;

typedef struct
{
    UINT8 u1Cmd;
    UINT8 u1SubCmd;    
    UINT8 au1Data[4];        
} PDWNC_T8032_CMD_T;

typedef struct
{
    UINT8 u1Ack;
    UINT8 au1Data[3];        
} PDWNC_T8032_RESPONSE_T;

typedef struct 
{
    UINT32 u4Reason;
    UINT32 u4Btn;
} PDWNC_SET_POWER_DOWN_REASON_T;

#if defined(CC_MT5363) || defined(CC_MT5387) 
#define PDWNC_T8032CMD_SET_CMD(x) vIO32Write4B(PDWNC_SHREG8, (x))
#define PDWNC_T8032CMD_SET_DATA(x) vIO32Write4B(PDWNC_SHREG9, (x))
#else	//after 5365/95
#define PDWNC_T8032CMD_SET_CMD(x) vIO32Write4B(PDWNC_SHREG1, (x))
#define PDWNC_T8032CMD_SET_DATA(x) vIO32Write4B(PDWNC_SHREG2, (x))
#endif

#define PDWNC_T8032CMD_GET_RESPONSE() u4IO32Read4B(PDWNC_SHREG0) 
#ifdef CC_MT8223
#define MSET_XDATA_TO_ARM_FLAG()
#define MCLR_XDATA_TO_ARM_FLAG()
#define IS_XDATA_AT_ARM()
#else
#define MSET_XDATA_TO_ARM_FLAG() vIO32Write4B(PDWNC_SHREG3, (u4IO32Read4B(PDWNC_SHREG3) & 0xffffff00) | 0x000000a5)
#define MCLR_XDATA_TO_ARM_FLAG() vIO32Write4B(PDWNC_SHREG3, u4IO32Read4B(PDWNC_SHREG3) & 0xffffff00)
#define IS_XDATA_AT_ARM() ((u4IO32Read4B(PDWNC_SHREG3) & 0x000000ff) == 0) ? 0 : 1
#endif

#if defined(CC_MT5387) && !defined(CC_MT5363) && !defined(CC_MT5301A)/* 5387 only */    
    #define XDATA_ADDR          0x4000
#else/* others (5363, 5365, 5395.. later.) */    
    #define XDATA_ADDR          0x8000
#endif 
#define CEC_XDATA_OFFSET    0x200
#define CEC_XDATA_SIZE      0x100

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
typedef void (* PFN_WDT_CALLBACK)(INT32 i4idx);
#endif
// PDWNC functions.
EXTERN INT32 PDWNC_Init(void);
EXTERN void PDWNC_pm_resume(void);
EXTERN void PDWNC_Reboot(void);

EXTERN void PDWNC_pm_suspend(void);
EXTERN void PDWNC_Reboot(void);
EXTERN void PDWNC_WatchDogReboot(void);
EXTERN void PDWNC_T8032Reboot(UINT32 u4Seconds);
EXTERN void PDWNC_IrLedCheck(INT32 fgOn);
EXTERN UINT32 PDWNC_EnterPowerDown(UINT32 u4PowerDownEn, UINT32 u4PowerDownEvent);
EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);
#if (!defined(CC_MT5363)) && (!defined(CC_MT5387)) //change after 5365/95/...
//-----------------------------------------------------------------------------
/** PDWNC_SRVAD_Read : ABIST version of PDWNC_ReadServoADCChannelValue()
 * function name 'PDWNC_SRVAD_Read' is named by ABIST integrator
 *  @param u4Channel the channel number
 *  @retval the channel adc value.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 PDWNC_SRVAD_Read(UINT32 u4Channel);
#endif
EXTERN UINT32 PDWNC_IsWakeupByPowerButton(void);
EXTERN UINT32 PDWNC_IsBootByWakeup(void);
EXTERN UINT32 PDWNC_PowerDownByAcCut(void);
EXTERN void PDWNC_CreateWatchDogThread(void);
EXTERN UINT32 PDWNC_ReadWakeupReason(void);
EXTERN UINT32 PDWNC_SetupPseudoWatchdogAssert(UINT32 *pu4Flag);

// Interrupt functions.
EXTERN BOOL PDWNC_ClearInt(UINT16 u2IntNo);
#ifndef CC_UBOOT
#if !defined(CC_SECURE_WORLD)
EXTERN INT32 PDWNC_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr,
                            x_os_isr_fct *ppfnOld);
#endif
#endif /* CC_UBOOT */

// GPIO functions
EXTERN INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioInput(INT32 i4Gpio);
EXTERN INT32 PDWNC_GpioIntrq(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioQuery(INT32 i4Gpio, INT32 *pi4Intr,
                            INT32 *pi4Fall, INT32 *pi4Rise);
EXTERN INT32 PDWNC_GpioReg(INT32 i4Gpio, GPIO_TYPE eType,
                            PFN_GPIO_CALLBACK pfnCallback);
EXTERN void PDWNC_GPIO_Suspend(void);
EXTERN void PDWNC_GPIO_Resume(void);
EXTERN INT32 PDWNC_GpioResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType);

// Servo used as GPIO
EXTERN INT32 PDWNC_InitServoGpio(void);
EXTERN INT32 PDWNC_ServoGpioEnable(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_ServoGpioOutput(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_ServoGpioInput(INT32 i4Gpio);
EXTERN INT32 PDWNC_ServoGpioIntrq(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_ServoGpioReg(INT32 i4Gpio, GPIO_TYPE eType,
                            PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32 PDWNC_ServoGpioQuery(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);

#ifdef CC_WELTREND_UP_SUPPORT

EXTERN UINT8 PDWNC_WeltrendWrite(UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
EXTERN UINT8 PDWNC_WeltrendRead(UINT8 u1RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);

#endif /* CC_WELTREND_UP_SUPPORT */

EXTERN INT32 PDWNC_WriteErrorCode(UINT8 u1ErrorCode);
EXTERN void PDWNC_WeltrendSystemRecovery(void);
EXTERN void PDWNC_WeltrendCheckBufClear(void);

EXTERN void PDWNC_SetPowerDownFlag(UINT8 u1Flag);
EXTERN UINT8 PDWNC_GetPowerDownFlag(void);
EXTERN UINT32 PDWNC_EnterPowerState(UINT32 u4PowerDownEvent);
EXTERN UINT32 PDWNC_SetPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T rReason);
EXTERN UINT32 PDWNC_QueryPowerDownReason(PDWNC_SET_POWER_DOWN_REASON_T *prReason);
EXTERN void _PdwncSetupVGA(UINT32 u4PowerDownEn);
#if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
EXTERN INT32 PDWNC_T8032G381pioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32 WDT_ISR_Reg(UINT32 u4Idx, PFN_WDT_CALLBACK pfnCallback);
#endif
#endif /* X_PDWNC_H */

