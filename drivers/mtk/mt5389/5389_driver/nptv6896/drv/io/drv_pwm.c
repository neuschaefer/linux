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
 * $Author: dtvbm11 $
 * $Date  $
 * $RCSfile: drv_pwm.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "drv_pwm.h"
#include "x_hal_5381.h"
#include "x_pinmux.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "drv_tdtv_drvif.h"
#include "hw_tcon.h"
#include "hw_mlvds.h"
#include "drv_display.h"
#include "sv_const.h"

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
EXTERN UINT8 bSI_DISPLAY_DCLK_TYPE;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** Brief
 * This driver control the PWM0, PWM1 & PWM2 output frequence and duty cycle
 *
 * duty cycle = bDutyON/(bDutyAll+1);
 * PWM freq.(in 5368/96...) = (BUS Clock) / ((PWMP+1)x(PWMRSN+1))
 *
 * @param bSrc      PWM source (SrcPWM0/1/2)
 * @param bTimeBase PWMP, PWM basic unit = (0~0xfffff) * BUS Clock
 * @param bDutyON   PWMH, ON duty width = (0~0xfff) * bTimeBase
 * @param bDutyAll  PWMRSN, Period width = (0~0xffe) * bTimeBase
 * @return void
 *
 * @example  vDrvSetPWM(SrcPWM0, 6, u4DutyON, 254);
 *           	 Duty Cycle = u4DutyON/(254+1)
 */

void vDrvSetPWM(UINT32 u4Src, UINT32 u4TimeBase, UINT32 u4DutyON,
                UINT32 u4DutyAll)
{
    UINT32 u4Reg = REG_RW_PWM1, u4Value;

	if(u4DutyON > (u4DutyAll+1))
	{
		Printf("PWM Error: u4DutyON and u4DutyAll= [0x %x,0x %x], u4DutyON is too large\n", u4DutyON, u4DutyAll);
	}
#if defined(CC_MT5396)	
    switch (u4Src)
    {
    case SrcPWM0:
        u4Reg = REG_RW_PWM0;
        if (DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_PWM0)
    	{
			UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION2)); 
    	}			
        break;
    case SrcPWM1:
        u4Reg = REG_RW_PWM1;
       if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_PWM1) 
        {
            UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION2)); //Tuner 30V Boost-up PWM in BGA
        }
	   else if(DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_GPIO48)
		{
			UNUSED(BSP_PinSet(PIN_GPIO48, PINMUX_FUNCTION1)); 
    	}
        break;
    case SrcPWM2:
        u4Reg = REG_RW_PWM2;
        if (DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_PWM2) 
        {
            UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION2)); //LED_CTL in BGA
        }
        break;
    default:
        ASSERT(u4Src <= SrcPWM2);
    }
#elif defined(CC_MT5389)
	switch (u4Src)
    {
    case SrcPWM0:
        u4Reg = REG_RW_PWM0;
        if (DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_GPIO3)
    	{
			UNUSED(BSP_PinSet(PIN_GPIO3, PINMUX_FUNCTION1)); 
    	}
		else if(DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_OSDA0)
		{
			UNUSED(BSP_PinSet(PIN_OSDA0, PINMUX_FUNCTION3)); 
		}
        break;
    case SrcPWM1:
        u4Reg = REG_RW_PWM1;
       if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_IF_AGC) 
        {
            UNUSED(BSP_PinSet(PIN_IF_AGC, PINMUX_FUNCTION2));
        }
	   else if(DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_OSCL0)
		{
			UNUSED(BSP_PinSet(PIN_OSCL0, PINMUX_FUNCTION3)); 
    	}
        break;
    case SrcPWM2:
        u4Reg = REG_RW_PWM2;
        if (DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_PWM0) 
        {
            UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION1)); //LED_CTL in BGA
        }
        break;
    default:
        ASSERT(u4Src <= SrcPWM2);
    }
#else	//defined(CC_MT5368)
    switch (u4Src)
	{
    case SrcPWM0:
        u4Reg = REG_RW_PWM0;
        if (DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_PWM0)
    	{
			UNUSED(BSP_PinSet(PIN_OPWM0, PINMUX_FUNCTION1)); 
    	}
		else if (DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_GPIO4)
    	{
			UNUSED(BSP_PinSet(PIN_GPIO4, PINMUX_FUNCTION1)); 
    	}
		else if (DRVCUST_InitGet(ePWM0Port)==PAD_PWM0_OSDA0)
    	{
			UNUSED(BSP_PinSet(PIN_OSDA0, PINMUX_FUNCTION3)); 
    	}
        break;
    case SrcPWM1:
        u4Reg = REG_RW_PWM1;
       if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_PWM1) 
        {
            UNUSED(BSP_PinSet(PIN_OPWM1, PINMUX_FUNCTION1)); //Tuner 30V Boost-up PWM in BGA
        }	   
	   else if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_GPIO5)
	   {
		   UNUSED(BSP_PinSet(PIN_GPIO5, PINMUX_FUNCTION1)); 
	   }
	   else if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_GPIO8)
	   {
		   UNUSED(BSP_PinSet(PIN_GPIO8, PINMUX_FUNCTION1)); 
	   }
	   else if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_IF_AGC)
	   {
		   UNUSED(BSP_PinSet(PIN_IF_AGC, PINMUX_FUNCTION2)); 
	   }
	   else if (DRVCUST_InitGet(ePWM1Port)==PAD_PWM1_OSCL0)
	   {
		   UNUSED(BSP_PinSet(PIN_OSCL0, PINMUX_FUNCTION3)); 
	   }
        break;
    case SrcPWM2:
        u4Reg = REG_RW_PWM2;
        if (DRVCUST_InitGet(ePWM2Port)==PAD_PWM2_PWM2) 
        {
            UNUSED(BSP_PinSet(PIN_OPWM2, PINMUX_FUNCTION1)); //LED_CTL in BGA
        }
        break;
    default:
        ASSERT(u4Src <= SrcPWM2);
    }
#endif

    // workaround for full duty drop issue
    if (u4DutyAll==0xff && u4DutyON==0xff)	//we used 0xff at normal
    {
        u4DutyAll = 0xfe;
    }
    if (u4DutyAll==0xfff && u4DutyON==0xfff)
    {
        u4DutyAll = 0xffe;
    }
	
	IO_WRITE32MSK(HPDS_BASE, u4Reg+4, u4TimeBase, 0xFFFFF);
	
    u4Value = (((UINT32)u4DutyAll << 20) | 
				((UINT32)(u4DutyON & 0xFFF) << 4) | 1);	
    IO_WRITE32(HPDS_BASE, u4Reg, u4Value);
}

/**
 * Set PWM sync to LVDS output v-sync signal.
 *
 * @param u4Src      PWM source (SrcPWM0/1/2)
 * @param u4LockON   Lock PWM on LVDS v-sync or not.
 */

void vDrvSetLock(UINT32 u4Src, UINT32 u4LockON)
{
    switch (u4Src)
    {
    default:
    case SrcPWM0:
        IO_WRITE32MSK(
            HPDS_BASE,
            REG_RW_PWM0VSYNC,
            u4LockON? 0x80000000 : 0,
            0xC0000000);
        break;
    case SrcPWM1:
        IO_WRITE32MSK(
            HPDS_BASE,
            REG_RW_PWM1VSYNC,
            u4LockON? 0x80000000 : 0,
            0xC0000000);
        break;
    case SrcPWM2:
        IO_WRITE32MSK(
            HPDS_BASE,
            REG_RW_PWM2VSYNC,
            u4LockON? 0x80000000 : 0,
            0xC0000000);
        break;
    }
}

//-----------------------------------------------------------------------------
/** Brief
 * This driver initialize pwm
 */
void vDrvInitSCANPWM(void)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
	
	#if defined(CC_MT5396)
	 if (BSP_GetIcVersion() >= IC_VER_5396_AB)
	 {
	  vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN_START_MODE);	 
      vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN1_START_HIGH);	 	  
      vIO32WriteFldAlign(PWM_SCAN_06, 0, REG_PWM_SCAN2_START_HIGH);	 	  	  
      vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN3_START_HIGH);	 	  	  	  
      vIO32WriteFldAlign(PWM_SCAN_10, 0, REG_PWM_SCAN4_START_HIGH);	 	  	  	  
      vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN5_START_HIGH);	 	  	  	  	  
	 } 
    #endif	
	
    vDrvSetScanPWMOnOff(SV_ON);
    vDrvSetScanPWMLatchMode(1);
    vDrvSetScanBLOnOff(SV_OFF);
    vDrvSetScanPWMPolarity(DRVCUST_PanelGet(ePanelBacklightPwmPort), 0);
    vDrvSetScanPWMOutBL(DRVCUST_PanelGet(ePanelBacklightPwmPort), 0);
//    vDrvSetPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort),vDrvGetPWMDutyCycleRatio(), 0, 0xFF, 0xFF);

    #ifdef CC_SUPPORT_PANEL_PWMSCAN
    vDrvSetScanPWM(SrcPWM3,1,300,200);
	vDrvSetScanPWM(SrcPWM4,120,300,200);
	vDrvSetScanPWM(SrcPWM5,270,300,200);
    #endif
	
    #endif
}

//-----------------------------------------------------------------------------
/** Brief
 * This driver calculates the pwm duty cycle period ratio
 * @return ratio
 */
UINT8 vDrvGetPWMDutyCycleRatio(void)
{
    #if defined(CC_MT5396) || defined(CC_MT5368)
    UINT8 u1Ratio, u1FrameRate;
    
    u1FrameRate = (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_50HZ) ? 50 : 60;
    u1Ratio = (vDrvGetLCDFreq() / u1FrameRate);
    return u1Ratio;	
	#else	
    return 0;
	#endif
}


/** Brief
 * This driver control the scanning PWM1 & PWM2
 * duty cycle = u4DutyON/u4DutyAll;
 * @param bSrc  PWM source (ScanPWM1/2)
 * @param u4Start  start_period (o%) ex: 0 --> 0%
 * @param u4High  high_period (o%) ex: 400 --> 40%
 * @param u4Low  low_period (o%) ex: 600 --> 60%
 * @u4Start + u4High + u4Low = 100%
 * @return void
 */
void vDrvSetScanPWM(UINT32 u4Src, UINT32 u4Start, UINT32 u4High, UINT32 u4Low)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    UINT16 u2HTotal, u2VTotal;
    UINT32 u4Val;
    
    u2HTotal = wDrvGetOutputHTotal();
    u2VTotal = wDrvGetOutputVTotal();
    u4Val = u2HTotal * u2VTotal / 1000;
    
    switch (u4Src)
    {        
        case SrcPWM1:
            vDrvSetScan1Start(u4Start*u4Val);
            vDrvSetScan1High((u4High*u4Val));
            vDrvSetScan1Low(u4Low*u4Val);
            break;
        
        case SrcPWM2:
            vDrvSetScan2Start(u4Start*u4Val);
            vDrvSetScan2High((u4High*u4Val));
            vDrvSetScan2Low(u4Low*u4Val);
            break;

        case SrcPWM3:
            vDrvSetScan3Start(u4Start*u4Val);
            vDrvSetScan3High((u4High*u4Val));
            vDrvSetScan3Low(u4Low*u4Val);
            break;

        case SrcPWM4:
            vDrvSetScan4Start(u4Start*u4Val);
            vDrvSetScan4High((u4High*u4Val));
            vDrvSetScan4Low(u4Low*u4Val);
            break;

        case SrcPWM5:
            vDrvSetScan5Start(u4Start*u4Val);
            vDrvSetScan5High((u4High*u4Val));
            vDrvSetScan5Low(u4Low*u4Val);
            break;
			
        default:
            ASSERT(u4Src <= SrcPWM5);
    }
	#endif
}

/** Brief
 * This driver control the scanning backlight polarify
 * @param bSrc  PWM source (ScanPWM1/2)
 * @param u1blPol  0: Normal/1: Reverse
 * @return void
 */
void vDrvSetScanPWMPolarity(UINT32 u4Src, UINT8 u1blPol)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    switch (u4Src)
    {        
        case SrcPWM1:
            vIO32WriteFldAlign(PWM_SCAN_06, u1blPol, REG_PWM_SCAN1_POL);
            break;
        
        case SrcPWM2:
            vIO32WriteFldAlign(PWM_SCAN_06, u1blPol, REG_PWM_SCAN2_POL);
            break;

        case SrcPWM3:
            vIO32WriteFldAlign(PWM_SCAN_10, u1blPol, REG_PWM_SCAN3_POL);
            break;

        case SrcPWM4:
            vIO32WriteFldAlign(PWM_SCAN_10, u1blPol, REG_PWM_SCAN4_POL);
            break;

        case SrcPWM5:
            vIO32WriteFldAlign(PWM_SCAN_11, u1blPol, REG_PWM_SCAN5_POL);
            break;
			
        default:
            ASSERT(u4Src <= SrcPWM5);
    }
	#endif
}

/** Brief
 * This driver control the scanning PWM enable
 * @param u1OnOff  
 * @return void
 */
void vDrvSetScanPWMOnOff(UINT8 u1OnOff)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vIO32WriteFldAlign(PWM_SCAN_06, u1OnOff, REG_PWM_SCAN_EN);
	
	#if defined(CC_MT5396)
	 if (BSP_GetIcVersion() >= IC_VER_5396_AB)
	 vIO32WriteFldAlign(PWM_SCAN_11, u1OnOff, REG_PWM_SCAN1_EN);	   
	 vIO32WriteFldAlign(PWM_SCAN_11, u1OnOff, REG_PWM_SCAN2_EN);	   	 
	 vIO32WriteFldAlign(PWM_SCAN_11, u1OnOff, REG_PWM_SCAN3_EN);	   	 	 
	 vIO32WriteFldAlign(PWM_SCAN_11, u1OnOff, REG_PWM_SCAN4_EN);	   	 	 
	 vIO32WriteFldAlign(PWM_SCAN_11, u1OnOff, REG_PWM_SCAN5_EN);	   	 	 	 
    #endif
	
	#endif
}

/** Brief
 * This driver control the scanning PWM latch mode
 * @param u1Mode  
 * @param 0: Latch until set_data assert
 * @param 1: Update at once
 * @return void
 */
void vDrvSetScanPWMLatchMode(UINT8 u1Mode)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vIO32WriteFldAlign(PWM_HS_01, u1Mode, REG_PWM_SCAN_LATCH_OFF);
	#endif
}

/** Brief
 * This driver control the output mux for scanning pwm
 * @param bSrc  PWM source (ScanPWM1/2)
 * @param u1OutBL
 * @param 0: output SCAN_PWM
 * @param 1: output TCON_3dbl_out which is from Panel_intf
 * @return void
 */
void vDrvSetScanPWMOutBL(UINT32 u4Src, UINT8 u1OutBL)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    switch (u4Src)
    {        
        case SrcPWM1:
            vIO32WriteFldAlign(PWM_SCAN_06, u1OutBL, REG_PWM_SCAN1_OUTBL);
            break;
        
        case SrcPWM2:
            vIO32WriteFldAlign(PWM_SCAN_06, u1OutBL, REG_PWM_SCAN2_OUTBL);
            break;

        case SrcPWM3:
            vIO32WriteFldAlign(PWM_SCAN_10, u1OutBL, REG_PWM_SCAN3_OUTBL);
            break;

        case SrcPWM4:
            vIO32WriteFldAlign(PWM_SCAN_10, u1OutBL, REG_PWM_SCAN4_OUTBL);
            break;
			
        case SrcPWM5:
            vIO32WriteFldAlign(PWM_SCAN_11, u1OutBL, REG_PWM_SCAN5_OUTBL);
            break;
			
        default:
            ASSERT(u4Src <= SrcPWM5);
    }
	#endif
}

/** Brief
 * This driver control the scanning backlight polarify
 * @param u1blPol  0: Normal/1: Reverse
 * @return void
 */
void vDrvSetScanBLPolarity(UINT8 u1blPol)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vIO32WriteFldAlign(PWM_SCAN_06, u1blPol, REG_PWM_SCAN_BL_POL);
	#endif
}

/** Brief
 * This driver control the scanning backlight enable
 * @param u1OnOff  
 * @return void
 */
void vDrvSetScanBLOnOff(UINT8 u1OnOff)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vIO32WriteFldAlign(PWM_SCAN_06, u1OnOff, REG_PWM_SCAN_BL_EN);

	#if defined(CC_MT5396)
	if (u1OnOff)
	{
	 if (BSP_GetIcVersion() >= IC_VER_5396_AB)
	 vIO32WriteFldAlign(PWM_SCAN_11, 0, REG_PWM_SCAN_START_MODE);
	}
	else
	{
	 if (BSP_GetIcVersion() >= IC_VER_5396_AB)
	 vIO32WriteFldAlign(PWM_SCAN_11, 1, REG_PWM_SCAN_START_MODE);
	}
    #endif	
	
	#endif
}

/** Brief
 * This driver control the scanning backlight latch mode
 * @param 0: Latch BL into a whole frame
 * @param 1: Direct use BL info to mask PWM
 * @return void
 */
void vDrvSetScanBLLatchMode(UINT8 u1Mode)
{
    #if defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
    vIO32WriteFldAlign(PWM_SCAN_06, u1Mode, REG_PWM_SCAN_BL_DIS_LATCH);
	#endif
}



void vDrvSetISOnOff(UINT8 u1Src, UINT8 u1OnOff)
{
    #if defined(CC_MT5396) || defined(CC_MT5368)
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1OnOff, VINTF_INV_SYNC_START);
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}

void vDrvSetISMaskNum(UINT8 u1Src, UINT8 u1MaskNum)
{
	#if defined(CC_MT5396) || defined(CC_MT5368)
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P21, u1MaskNum, VINTF_IS_MASK_LEN_H);	
            break;
			
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P23, u1MaskNum, VINTF_IS_MASK_LEN_V);
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}

void vDrvSetISInputInverse(UINT8 u1Src, UINT8 u1Enable)
{
	#if defined(CC_MT5396) || defined(CC_MT5368)
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1Enable, VINTF_IS_POL_H);
            break;
        
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u1Enable, VINTF_IS_POL_V);
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}

void vDrvSetISDelayOnOff(UINT8 u1Src, UINT8 u1OnOff)
{
	#if defined(CC_MT5396) || defined(CC_MT5368)
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:       
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1OnOff, VINTF_IS_FORCE_DELAY);
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}

void vDrvSetISDelayNum(UINT8 u1Src, UINT16 u2DelayNum)
{
	#if defined(CC_MT5396) || defined(CC_MT5368)
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:       
            vIO32WriteFldAlign(VINTF_P20, u2DelayNum, VINTF_IS_DELAY_H);			
            break;
			
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u2DelayNum, VINTF_IS_DELAY_V);
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}

void vDrvSetISHoldOnOff(UINT8 u1Src, UINT8 u1OnOff)
{
	#if defined(CC_MT5396) || defined(CC_MT5368)
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:       
            vIO32WriteFldAlign(VINTF_P20, u1OnOff, VINTF_IS_HOLD_EN_H);			
            break;
			
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u1OnOff, VINTF_IS_HOLD_EN_V);
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}

void vDrvSetISRelation(UINT8 u1Src, UINT8 u1SyncCnt, UINT8 u1ISSyncCnt)
{
	#if defined(CC_MT5396) || defined(CC_MT5368)
    UINT32 u4Total =0, u4ISTotal=0;
	UINT8 u1Scatter =0;
	
    if (u1SyncCnt >= 15) u1SyncCnt = 15;
    if (u1ISSyncCnt >= 15) u1ISSyncCnt = 15;	

    if (u1SyncCnt == 0) u1SyncCnt = 1;
    if (u1ISSyncCnt == 0) u1ISSyncCnt = 1;
	
    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:
            u4Total = wDrvGetOutputHTotal();			
            break;
        
        case MINILVDS_IS_VSYNC:
            u4Total = wDrvGetOutputVTotal();			
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }

	u4ISTotal = ( u4Total * u1SyncCnt ) / u1ISSyncCnt;
	u1Scatter = ( u4Total * u1SyncCnt ) % u1ISSyncCnt;

    switch (u1Src)
    {        
        case MINILVDS_IS_HSYNC:
            vIO32WriteFldAlign(VINTF_P20, u1ISSyncCnt, VINTF_IS_DIV_H);
            vIO32WriteFldAlign(VINTF_P20, u1SyncCnt, VINTF_IS_MUL_H);			
            vIO32WriteFldAlign(VINTF_P20, u1Scatter, VINTF_SYNC_TOTAL_PLUS_1_H);						
            vIO32WriteFldAlign(VINTF_P21, u4ISTotal, VINTF_IS_PWM_SYNC_TOTAL_H);			
            break;
        
        case MINILVDS_IS_VSYNC:
            vIO32WriteFldAlign(VINTF_P22, u1ISSyncCnt, VINTF_IS_DIV_V);
            vIO32WriteFldAlign(VINTF_P22, u1SyncCnt, VINTF_IS_MUL_V);			
            vIO32WriteFldAlign(VINTF_P23, u1Scatter, VINTF_IS_LEN_PLUS_1_V);			
            vIO32WriteFldAlign(VINTF_P23, u4ISTotal, VINTF_IS_PWM_SYNC_TOTAL_V);				
            break;
        
        default:
            ASSERT(u1Src <= MINILVDS_IS_VSYNC);
    }
	#endif
}


