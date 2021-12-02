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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_wdt.c $ 
 *
 * Project:
 * --------
 *   MT8226
 *
 * Description:
 * ------------
 *   BIM register definition
 *   
 * Author:
 * -------
 *   Daniel Hsiao
 *
 * Last changed:
 * ------------- 
 * $Author: dtvbm11 $ 
 *
 * $Modtime: 04/05/27 10:47p $    
 *
 * $Revision: #1 $
****************************************************************************/
#include "general.h"
#include "hw_bim.h"
#include "drv_kr.h"
#include "hw_reg.h"
//#include "hw_rs232.h"
#include "stdio.h"
//#include "custom_def.h"
#define TA_PROTECT() {TA=0xAA;TA=0x55;}
extern UINT8 xdata fgRs232Rx;
#if SUPPORT_ERROR_LED_CTRL
extern Error_Code_Ctrl gErrCodeCtrl;
#endif
#if RC56_IR_DRIVER
extern LPIR_struct gLPIR;
extern UINT8 GetColorIdx(UINT8 flag);

extern UINT8 RC6_B2_KEY[18];/* = {0x80,0xC0,0xA0,0xE0,0x90,0xD0,0xB0,0xF0,0x88,0xC8,0xD8,0x9D,0x98,0xf8,0x82,0xc2,0xa8,0x8e};*/
extern UINT8 RC6_B1_KEY[4];/* = {0x0, 0x4C, 0x24,0x58};*/
extern UINT16 RC5_B2B3_KEY[18];/* = {0x0eaa,0x1daa,0x0e6a,0x1d6a,0x0e9a,0x1d9a,0x0e5a,0x1d5a,0x0ea6,0x1da6,0x1d96,0x0eac,0x0e96,0x1d6a,0x0eaa,0x1daa,0x0e6a,0x0ea5};*/
extern UINT8 RC5_BLUE_B1_KEY[18];/* = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xD5,0xAA,0x56,0x6a,0x6a,0x6a,0x6a};*/
extern UINT8 SYS7_B1_KEY[18];/* = 		{0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0x95,0xff,0x95,0xff,0x55,0x55,0x55,0x6a};*/
#endif
#if RW_REG_INTERFACE
UINT8 u1PressChUpCnt;
UINT8 u1PressChDnCnt;
UINT8 u1Serv4Val;
#endif

void vWatchDogTimerInt(void) small interrupt WDINT  
{
    #if   UART_DBG
    UINT32 u4Reg;
    #endif
    TA_PROTECT();
    RWT=1;
    
    TA_PROTECT();
    WDIF=0;


#if 0// RW_REG_INTERFACE
vConfigSrvad(4);

    u1Serv4Val = bReadSrvad(4);

    if((u1Serv4Val > 32) &&  (u1Serv4Val  < 43)) 
    {
        u1PressChUpCnt++ ;
        u1PressChDnCnt = 0;
    }
    else if((u1Serv4Val > 49) &&  (u1Serv4Val  < 59)) 
    {
        u1PressChDnCnt++;
        u1PressChUpCnt = 0;        
    }
    else
    {
        u1PressChDnCnt = 0;
        u1PressChUpCnt = 0;
    }

    if(u1PressChUpCnt > 2)
    {
        _LogS("Rdy to switch to ARM");
        vKrDelay10ms(1);
        SwitchBack_PORT0();//Force switch uart0 to ARM
    }
    else if(u1PressChDnCnt > 2)
    {
        SwitchUartPD_PORT0();//Force switch uart0 to T8032
        vKrDelay10ms(1);
        _LogS("UART0 switch to T8032");
    }

#endif

#if 0// RW_REG_INTERFACE
    _LogS("WDT_");
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
	{
	    _LogS("ARM_PWR_ON");
	}
	else
	{
	    _LogS("ARM_PWR_Off");
	}
#endif
#if   UART_DBG
    LogS("WDT");
    
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
	{
	    u4Reg = u4RegRead4B(PDWNC_T8032_INTEN);
	    LogSD("ARM_PWR_ON", u4Reg);
	}
	else
	{
	    LogSD("ARM_PWR_Off", u4Reg);
	}

#endif

}

void vWatchDogInit(void)
{
#if  defined(CC_MT5365) || defined(CC_MT5395)
    CKCON = 0xc1;  //set (WD1,WD0)=(1,0); movx duration as 3 machine cycles
#else    
    CKCON = 0x81;  //set (WD1,WD0)=(1,0); movx duration as 3 machine cycles
#endif    
     TA_PROTECT();
    RWT=1; //reset watchdog timer
    
    TA_PROTECT();
//    EWT=1; //Enable watchdog reset
    EWDI=1; //Enale watch dog timer interrupt
}

