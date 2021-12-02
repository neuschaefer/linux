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
 * $Date: 2012/04/27 $
 * $RCSfile: ostg_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "general.h"
#include "drv_video.h"
#include "drv_lvds.h"
#include "panel.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_timer.h"
LINT_EXT_HEADER_END
#include "ostg_if.h"
#include "drv_lvds.h"
#include "drv_display.h"
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "drv_pwm.h"
#include "x_gpio.h"
#include "nptv_debug.h"
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif

/*lint -e717*/
/*
e717 : do ... while(0)
*/

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
void vDrvGpioSet(INT32 dwNum, INT32 dwVal)
{
    do
    {
        dwVal = GPIO_Output(dwNum, &dwVal);
    } while (0);
}

// backlight power-------------------------------------------------------------
void vDrvTurnOnBacklt(void)
{
    do
    {
        UINT32 gpio, value;
        gpio = DRVCUST_PanelGet(eBackLightGpio);
        value = DRVCUST_PanelGet(eBackLightOnVal);
        vDrvGpioSet((INT32)gpio, (INT32)value);
        if (DRVCUST_PanelQuery(eBackLightGpio2, &gpio) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)value);
        }
    } while (0);
}

void vDrvTurnOffBacklt(void)
{
    do
    {
        UINT32 gpio, value;
        gpio = DRVCUST_PanelGet(eBackLightGpio);
        value = DRVCUST_PanelGet(eBackLightOnVal);
        vDrvGpioSet((INT32)gpio, (INT32)!value);
        if (DRVCUST_PanelQuery(eBackLightGpio2, &gpio) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)!value);
        }
    } while (0);
}

// PDP VsON------------------------------------------------------------------
void vDrvTurnOnPDPVs(void)
{
    do
    {
        UINT32 gpio, value;
        if (DRVCUST_PanelQuery(ePanelPdpVsGpio, &gpio) == 0)
        {
            value = DRVCUST_PanelGet(ePanelPdpVsOnVal);
            vDrvGpioSet((INT32)gpio, (INT32)value);
        }
    } while (0);
}

void vDrvTurnOffPDPVs(void){
    do
    {
        UINT32 gpio, value;
        if (DRVCUST_PanelQuery(ePanelPdpVsGpio, &gpio) == 0)
        {
            value = DRVCUST_PanelGet(ePanelPdpVsOnVal);
            vDrvGpioSet((INT32)gpio, (INT32)!value);
        }
    } while (0);
}



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PWM_DUTY_ALL 255
#define PWM_DUTY_LESS 254

#ifdef THE_3RD_PARTY_SW_SUPPORT
	#define DEFAULT_DIMMING_FREQUENCY	180
#else
	#define DEFAULT_DIMMING_FREQUENCY	1300
#endif
#define DEFAULT_BOOSTING_FREQUENCY	10000

static BOOL _fgPanelOn = FALSE;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
BOOL	_fgIsSync = (BOOL)SV_OFF;
UINT16	_wFreq    = (UINT16)2169;
UINT16	_wDuty    = (UINT16)128;

/** Set LVDS driving current
 *  @param bDriving: 0~15
 *  @return
 */
void vApiLVDSSetDriving(UINT8 bDriving)
{
    vDrvLVDSSetDriving(bDriving);
}

/**
 * Set Panel output frame rate
 */
void vApiLCDSetFrameRate(UINT16 wFrameRate)
{
    vDrvLCDSetFrame(wFrameRate);
}

UINT8 bApiLCDGetFrameRate(void)
{
    return vDrvGetLCDFreq();
}

//-----------------------------------------------------------------------------
/** event handler before update output frame rate
 *  @param u4FrameRate: output frame rate
 *  @return
 */
//-----------------------------------------------------------------------------


static UINT32 _u4FrameRate = 60;
UINT32 _u4BrightLevel = 0x80;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//UINT32 BACKLT_GetCurrentPanelBright(void)
//{
//    return _u4BrightLevel;
//}


void BACKLT_SetDimming(UINT32 u4Frequency, UINT32 u4Dimming)
{
    if (u4Frequency == 0)
    {
        u4Frequency = DEFAULT_DIMMING_FREQUENCY;
    }

    // Invert PWM dimming signal. If pin output with a BJT invert circuit.
    if (DRVCUST_PanelGet(eFlagBacklightDimmingInvert))
    {
        if (u4Dimming >= 256)
        {
            u4Dimming = 0;
        }
        else
        {
            u4Dimming =  256 - u4Dimming;
        }
    }
    else
    {
        if (u4Dimming >= 256)
        {
            u4Dimming = 256;
        }
    }

    if (u4Dimming == 256)
    {
        vDrvSetPWM(
            DRVCUST_PanelGet(ePanelBacklightPwmPort),
            (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / u4Frequency,
            0xFF,
            PWM_DUTY_LESS);
    }
    else
    {
        vDrvSetPWM(
            DRVCUST_PanelGet(ePanelBacklightPwmPort),
            (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / u4Frequency,
            u4Dimming,
            PWM_DUTY_ALL);
    }
}


UINT32 BACKLT_GetDimmingFrequency(void)
{
    UINT32 u4Freq = 0;

    if ((_u4FrameRate == 50) || (_u4FrameRate == 25))
    {
        u4Freq = DRVCUST_PanelGet(eDimmingFreq50);
        if (u4Freq == 0)
        {
            u4Freq = PANEL_GetDimmingFrequency50Hz();
        }
    }
    else
    {
        u4Freq = DRVCUST_PanelGet(eDimmingFreq60);
        if (u4Freq == 0)
        {
            u4Freq = PANEL_GetDimmingFrequency60Hz();
        }
    }

    if (u4Freq == 0)
    {
        u4Freq = DEFAULT_DIMMING_FREQUENCY;
    }

    return u4Freq;
}

//-----------------------------------------------------------------------------
/** set panel backlight to a brightness level.
 *  @param u1BrightLevel: Brightness level.(0:dark --> 255:bright)
 *  @return The brightness level
 */
//-----------------------------------------------------------------------------
void vApiSetPanelBright(UINT8 u1BrightLevel)
{
    UINT32 ret = 0;

    ret = (UINT32)DRVCUST_PanelQuery(ePanelDisableDimming, &ret);

    if ((DRVCUST_PanelQuery(ePanelDisableDimming, &ret) == 0) && (ret == 1))
    {
        return;
    }

    // if panel is down, just save backlight level
    _u4BrightLevel = (UINT32)u1BrightLevel;
    if (!_fgPanelOn)
    {
        return;
    }

	BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), (UINT32)u1BrightLevel);
}

/**
 * @brief Set backlight dimming.
 * @param fgIsSync <SV_ON/SV/OFF>: Synchronize to output v-sync or not.
 * @param bFreq: physical PWM frequency.
 * @param bDUty: duty cycle.
 */
void vApiSetDimming(UINT16 u2Freq, UINT16 u2Duty)
{
    _fgIsSync = (BOOL)SV_ON;
    _wFreq = u2Freq;
    _wDuty = u2Duty; // No need to +1.

	BACKLT_SetDimming(_wFreq, _wDuty);
}

void vApiSetBooster(UINT16 wLevel)
{
	if (!DRVCUST_PanelGet(eFlagBacklightBoostCtrl))
	{
		return;
	}

    // Invert PWM boost signal. If pin output with a BJT invert circuit.
    if (DRVCUST_PanelGet(eFlagBacklightBoostInvert))
    {
    	if (wLevel >= 256)
    	{
            wLevel = 0;
    	}
        else
        {
            wLevel = 256 - wLevel;
        }
    }
    else
    {
    	if (wLevel >= 256)
    	{
            wLevel = 256;
    	}
    }

    if (wLevel == 256)
    {
        vDrvSetPWM(
            SrcPWM1,
            (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / DEFAULT_BOOSTING_FREQUENCY,
            0xFF,
            PWM_DUTY_LESS);
    }
    else
    {
        vDrvSetPWM(
            SrcPWM1,
            (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / DEFAULT_BOOSTING_FREQUENCY,
            wLevel,
            PWM_DUTY_ALL);
    }
}


//-----------------------------------------------------------------------------
/** turn on/off panel backlight.
 *  @param fgEnable: Control mode. It could be PANEL_ON or PANEL_OFF
 *  @return
 */
//-----------------------------------------------------------------------------

void vApiBackltONOFF(BOOL fgEnable)
{
    if (fgEnable == PANEL_ON)
    {
        vDrvTurnOnBacklt();
    }
    else
    {
        vDrvTurnOffBacklt();
    }
}


//-----------------------------------------------------------------------------
/** turn on/off panel lvds power and backlight.
 *  @param fgEnable: Control mode. It could be PANEL_ON or PANEL_OFF
 *  @return
 */
//-----------------------------------------------------------------------------

void vApiPanelPowerSequence(BOOL fgEnable)
{
    PFN_USR_POWER_SEQUENCE pfnUsrPowerSequence = NULL;

    //static BOOL b_inited = FALSE;

    //if ( IS_COMPANION_CHIP_ON() && !b_inited )
    //{
    //    _fgPanelOn = TRUE;
    //    b_inited = TRUE;
    //}

    if (_fgPanelOn == fgEnable)
    {
        return;
    }

    if (fgEnable == PANEL_ON) //panel power on sequence
    {
        if (GPIO_Enable(DRVCUST_PanelGet(eBackLightGpio), NULL) == 1)
        {
            // it's already output mode.
            if (GPIO_Output(DRVCUST_PanelGet(eBackLightGpio), NULL) == DRVCUST_PanelGet(eBackLightOnVal))
            {
                // already on.
                _fgPanelOn = TRUE;
                return;
            }
        }

        // Check if there is customer-defined power on sequence
        UNUSED(DRVCUST_InitQuery(eCustomerPowerOnSequence,
                                (UINT32 *)(void *)&(pfnUsrPowerSequence)));
        if (pfnUsrPowerSequence)
        {
            pfnUsrPowerSequence();
            _fgPanelOn = TRUE;
            return;
        }

        // normal power sequence, lvds power -> lvds signal -> backlight power
        vDrvTurnOnPanel();

        // the magic 1 makes this delay > 10 * PANEL_GetLvdsOnDalay()
        // otherwise it will less than 10 * PANEL_GetLvdsOnDalay()
        x_thread_delay((10 * PANEL_GetLvdsOnDalay()) + 1);

        #ifdef DRV_SUPPORT_EXTMJC
        if (IS_COMPANION_CHIP_ON())
        {
            UINT8 u1MsgErr;
            d_custom_extmjc_set_fastmute(TRUE, &u1MsgErr);
            //drv_extmjc_set_lvds_on_delay(PANEL_GetLvdsOnDalay());
            drv_extmjc_set_lvds_enable(SV_ON, &u1MsgErr);
            d_custom_extmjc_set_fastmute_timeout(0x20,&u1MsgErr);
        }
        #endif

#ifdef PANEL_LG_26_WX2
        // special for LG26 PWM, lvds power -> dim -> lvds signal
        if (GetCurrentPanelIndex() == PANEL_LG_26_WX2)
        {
            BACKLT_SetDimming(BACKLT_GetDimmingFrequency(),
            (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)?100:0);
#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWERON, (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)?100:0);
#endif
        }
#endif

        vDrvTurnOnPDPVs();
        vDrvLVDSOff();
        vDrvLVDSConfig();
        vDrvLVDSOn();
        #ifdef DRV_SUPPORT_EXTMJC
        if (IS_COMPANION_CHIP_ON())
        {
            	UINT8 u1MsgErr;
        	drv_extmjc_set_RX_reset(&u1MsgErr);
        }
        #endif

#ifndef __MODEL_slt__ // save booting time
        x_thread_delay(10 * PANEL_GetBacklightOnDelay());
#endif
        vDrvTurnOnBacklt();

#ifdef PANEL_LG_26_WX2
        if (GetCurrentPanelIndex() == PANEL_LG_26_WX2)
        {
            // keep 100% duty at least 1000ms after backlight on,
            x_thread_delay(1000);
        }
#endif

        _fgPanelOn = TRUE;
        // after panel on, we could set backlight
        if(DRVCUST_InitGet(eFlagControlBacklightPWM)) 
        {
            vApiSetPanelBright(_u4BrightLevel);
        }
#ifndef CC_MTK_LOADER
        DRVCUST_UpdateBacklightState(BL_STATE_POWERON, _u4BrightLevel);
#endif

        if (DRVCUST_PanelGet(eFlagBacklightDimmingLock))
        {
            vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_ON);
        }
        else
        {
            vDrvSetLock(DRVCUST_PanelGet(ePanelBacklightPwmPort), SV_OFF);
        }
    }
    else // panel power off sequence
    {
        // Check if there is customer-defined power on sequence
        UNUSED(DRVCUST_InitQuery(eCustomerPowerOffSequence,
                                (UINT32 *)(void *)&(pfnUsrPowerSequence)));
        if (pfnUsrPowerSequence)
        {
            pfnUsrPowerSequence();
            _fgPanelOn = FALSE;
            return;
        }

        // backlight power -> lvds sugnal -> lvds power
        vDrvTurnOffBacklt(); // Backlight off
        x_thread_delay(10 * PANEL_GetBacklightOffDelay());

        #ifdef DRV_SUPPORT_EXTMJC
        if (IS_COMPANION_CHIP_ON())
        {
            UINT8 u1MsgErr;
            drv_extmjc_set_lvds_enable(SV_OFF, &u1MsgErr);
        }
        #endif
        vDrvLVDSOff();
        vDrvTurnOffPDPVs();

        x_thread_delay(10 * PANEL_GetLvdsOffDalay());

#ifdef PANEL_LG_26_WX2
        // special for LG26 PWM
        if (GetCurrentPanelIndex() == PANEL_LG_26_WX2)
        {
            BACKLT_SetDimming(BACKLT_GetDimmingFrequency(),
            	(IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)? 0 : 100);
#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWERON, (IsPwmLowPanelBright() == PWM_HIGH_PANEL_BRIGHT)? 0 : 100);
#endif
        }
#endif

        vDrvTurnOffPanel(); // Panel off

        _fgPanelOn = FALSE;
    }
}




void BeforeOutputFrameRateChange(UINT32 u4FrameRate)
{
    LOG(7, "BeforeOutputFrameRateChange, %d->%d\n", _u4FrameRate, u4FrameRate);
    // some panel will crash when changing frame rate,
    // so we reset panel this moment to prevent it
/*
    if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
    {
        vApiPanelPowerSequence(FALSE);
    }
*/
    // update output frame rate for sw pwm frequency control
    _u4FrameRate = u4FrameRate;

    // update dimming frequency
	if(DRVCUST_InitGet(eFlagControlBacklightPWM)) 
	{
		vApiSetPanelBright(_u4BrightLevel);
	}
#ifndef CC_MTK_LOADER
    DRVCUST_UpdateBacklightState(BL_STATE_OUTPUT_FR_CHANGE, _u4BrightLevel);
#endif
}


//-----------------------------------------------------------------------------
/** event handler after update output frame rate
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void AfterOutputFrameRateChange(void)
{
}
