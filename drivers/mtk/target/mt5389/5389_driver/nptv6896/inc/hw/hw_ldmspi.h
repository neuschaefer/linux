/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2003
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: hw_ycproc.h $
 *
 * Project:
 * --------
 *   MT5381
 *
 * Description:
 * ------------
 *   LCPROC register definition
 *
 * Author:
 * -------
 *   Jeffrey Xuan
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 07/04/06 1:22p $
 *
 * $Revision: #1 $
****************************************************************************/

#ifndef _HW_LDMSPI_H_
#define _HW_LDMSPI_H_

#include "x_typedef.h"
#include "x_os.h"
#include "x_hal_io.h"
#include "x_ckgen.h"

#define SPI_BASE 0xF0029000

/* Local Dimming SPI Protocol Registers*/
//Page LDM_SPI
#define NEW_GROUP (SPI_BASE + 0x408)
    #define PMUX2 Fld(2,16,AC_MSKB2)//[17:16]
#define LED_WADR (SPI_BASE + 0x400)
    #define SRAM_WADR Fld(10,0,AC_MSKW10)//[9:0]
#define LED_RADR (SPI_BASE + 0x404)
    #define SRAM_RADR Fld(10,0,AC_MSKW10)//[9:0]
#define LED_DATA (SPI_BASE + 0x408)
    #define SRAM_DATA Fld(12,0,AC_MSKW10)//[11:0]
#define LED_RADR2 (SPI_BASE + 0x40C)
    #define SRAM_RADR2 Fld(10,0,AC_MSKW10)//[9:0]
#define LED_CKCFG (SPI_BASE + 0x410)
    #define CLK_DIV Fld(12,16,AC_MSKW32)//[27:16]
    #define CPOL Fld(1,4,AC_MSKB0)//[4:4]
    #define CPHA Fld(1,0,AC_MSKB0)//[0:0]
#define LED_VSCFG (SPI_BASE + 0x420)
    #define VSYNC_INV Fld(1,31,AC_MSKB3)//[31:31]
    #define FRAME_RATE Fld(4,24,AC_MSKB3)//[27:24]
    #define VSYNC_LATENCY Fld(24,0,AC_MSKDW)//[23:0]
#define LED_VSACC (SPI_BASE + 0x424)
    #define WAIT_LATENCY Fld(12,16,AC_MSKW32)//[27:16]
    #define VSYNC_HOLD_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define LED_CSCFG (SPI_BASE + 0x430)
    #define CS_INV Fld(1,31,AC_MSKB3)//[31:31]
    #define RDATA_SINGLE_EN Fld(1,8,AC_MSKB1)//[8:8]
    #define NHEAD_SINGLE_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define NDATA_SINGLE_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define NFOOT_SINGLE_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define MHEAD_SINGLE_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define MDATA_SINGLE_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define MFOOT_SINGLE_EN Fld(1,0,AC_MSKB0)//[0:0]
#define LED_CSACC (SPI_BASE + 0x434)
    #define CS_LEAD_LATENCY Fld(12,16,AC_MSKW32)//[27:16]
    #define CS_LAG_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define LED_CSACC2 (SPI_BASE + 0x438)
    #define CS_HOLD_LATENCY Fld(12,0,AC_MSKW10)//[11:0]
#define LED_NOB (SPI_BASE + 0x440)
    #define NHEAD_LEN Fld(2,28,AC_MSKB3)//[29:28]
    #define NDATA_LEN Fld(8,20,AC_MSKW32)//[27:20]
    #define NFOOT_LEN Fld(2,16,AC_MSKB2)//[17:16]
    #define MHEAD_LEN Fld(2,12,AC_MSKB1)//[13:12]
    #define MDATA_LEN Fld(8,4,AC_MSKW10)//[11:4]
    #define MFOOT_LEN Fld(3,0,AC_MSKB0)//[2:0]
#define LED_NOB2 (SPI_BASE + 0x444)
    #define NBYTE_LEN_EXT Fld(2,24,AC_MSKB3)//[25:24]
    #define MBYTE_LEN_EXT Fld(2,20,AC_MSKB2)//[21:20]
    #define RHEAD_LEN Fld(2,12,AC_MSKB1)//[13:12]
    #define RBYTE_LEN Fld(4,4,AC_MSKB0)//[7:4]
    #define RBACK_LEN Fld(2,0,AC_MSKB0)//[1:0]
#define LED_FMT (SPI_BASE + 0x448)
    #define LED_TYPE Fld(2,12,AC_MSKB1)//[13:12]
    #define CUSTOM_FMT Fld(3,8,AC_MSKB1)//[10:8]
    #define BIT_MODE Fld(3,4,AC_MSKB0)//[6:4]
    #define SPI_RGB_MODE Fld(1,1,AC_MSKB0)//[1:1]
    #define LSB_MODE Fld(1,0,AC_MSKB0)//[0:0]
#define LED_CON (SPI_BASE + 0x44C)
    #define OP_MODE Fld(2,12,AC_MSKB1)//[13:12]
    #define SW_SET_DATA Fld(1,9,AC_MSKB1)//[9:9]
    #define TEST_MODE Fld(1,8,AC_MSKB1)//[8:8]
    #define LED_IDLE Fld(1,5,AC_MSKB0)//[5:5]
    #define SRAM_HW_MODE Fld(1,2,AC_MSKB0)//[2:2]
    #define HW_TRIG_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define SW_TRIG Fld(1,0,AC_MSKB0)//[0:0]
#define LED_NHEAD (SPI_BASE + 0x450)
    #define NHEAD_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define NHEAD_0 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_NFOOT (SPI_BASE + 0x460)
    #define NFOOT_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define NFOOT_0 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_MHEAD (SPI_BASE + 0x470)
    #define MHEAD_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define MHEAD_0 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_MFOOT0 (SPI_BASE + 0x480)
    #define MFOOT_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define MFOOT_0 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_MFOOT1 (SPI_BASE + 0x484)
    #define MFOOT_3 Fld(16,16,AC_FULLW32)//[31:16]
    #define MFOOT_2 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_MFOOT2 (SPI_BASE + 0x488)
    #define MFOOT_5 Fld(16,16,AC_FULLW32)//[31:16]
    #define MFOOT_4 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_MFOOT3 (SPI_BASE + 0x48C)
    #define MFOOT_6 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_RHEAD (SPI_BASE + 0x490)
    #define RHEAD_1 Fld(16,16,AC_FULLW32)//[31:16]
    #define RHEAD_0 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_RBUF (SPI_BASE + 0x4A0)
    #define READ_BUF1 Fld(16,16,AC_FULLW32)//[31:16]
    #define READ_BUF0 Fld(16,0,AC_FULLW10)//[15:0]
#define LED_INTR_EN (SPI_BASE + 0x4F0)
    #define LED_DONE_INTR_EN Fld(1,0,AC_MSKB0)//[0:0]
#define LED_INTR (SPI_BASE + 0x4F4)
    #define LED_DONE_INTR Fld(1,0,AC_MSKB0)//[0:0]

/* AMS Registers */
#define LDM_AS3693_CUR_REGULATOR  0x01    // Enables or disables the current regulators,                      see page 13
#define LDM_AS3693_PWM_HTIME1     0x12    // Defines PWM high time, 0x12 ~ 0x30,                              see page 18
#define LDM_AS3693_FBK_CONFIG1    0x04    // Enables and Disables the Different Feedback modes,               see page 10  
#define LDM_AS3693_FBK_CONFIG2    0x58    // Enables and Disables the Different Feedback modes,               see page 10  
#define LDM_AS3693_FEEDBACK1      0x05    // Selects the feedback of the voltage regulators, 0x05 ~ 0x08      see page 12
#define LDM_AS3693_PWM_MODE       0x0f    // Controls the different PWM modes and Internal or external PWM ,  see page 15
#define LDM_AS3693_FAULT_DETECT   0x09    // Open / Short Detection, 0x09 ~ 0x0C,                             see page 13
#define LDM_AS3693_DELAY1         0x32    // Defines the delay time of the PWM, 0x32 ~ 0x50,                  see page 18
#define LDM_AS3693_PWM_PERIOD_LSB 0x10    // Defines the period LSB of the PWM                                see page 18
#define LDM_AS3693_PWM_PERIOD_MSB 0x11    // Defines the period MSB of the PWM                                see page 18

#define LDM_AS3693_WRITE_IDX      0x0
#define LDM_AS3693_READ_IDX       0x1

/* AMS Device IDs*/
#define LDM_AS3693_ADDR1        0x3F


/* ATI Registers */
#define LDM_AAT2439A_DOT            0x00    // Dot Correction: 8bit 256 linear current step, default 0x00 in off status.     see page 17
#define LDM_AAT2439A_PWM_DUTY1_LB   0x40    // Grayscale(40h-7Eh): 10bit PWM duty control. 0x000~0x3FE.                      see page 18
#define LDM_AAT2439A_PWM_DELAY1_LB  0x80    // Grayscale(80h-BEh): 10bit PWM delay control. 0x000~0x3FE.                     see page 19
#define LDM_AAT2439A_CTRL1          0xC0    // Current Sink on/off: Enable of individual current sink, 1~8                   see page 20
#define LDM_AAT2439A_CTRL2          0xC2    // Current Sink on/off: Enable of individual current sink, 9~16                  see page 20
#define LDM_AAT2439A_CTRL3          0xC4    // General Control                                                               see page 21
    #define LDM_AAT2439A_PWM_CTRL3_INVERT      (1<<5)    // invert bit
    #define LDM_AAT2439A_PWM_CTRL3_DOTR        (1<<6)    // DOTR
#define LDM_AAT2439A_Fault_Status   0xE0    // Fault status(E0h-E6h): open/short status                                      see page 22
#define LDM_AAT2439A_OTSL           0xE8    // Over-Temperature Configuration                                                see page 27
#define LDM_AAT2439A_FAULT_TRIGGER  0xF0    // Fault Trigger(F0h-F6h): Enable open/short condition                           see page 29

#define LDM_AAT2439A_WRITE_IDX      0x0
#define LDM_AAT2439A_READ_IDX       0x1


/* AATI Device IDs*/
#define LDM_AAT2439A_DEVICE_ADDR        0x5F

#endif //_HW_LDMSPI_H_
