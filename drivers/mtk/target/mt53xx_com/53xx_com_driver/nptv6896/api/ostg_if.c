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
 * $Author: binghe.li $
 * $Date: 2013/01/12 $
 * $RCSfile: ostg_if.c,v $
 * $Revision: #11 $
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
#include "hw_tcon.h"
#include "drv_tcon.h"
#include "drvcust_if.h"
#include "x_ckgen.h"
#include "drv_pwm.h"
#include "x_gpio.h"
#include "nptv_debug.h"
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif
#include "drv_vbyone.h"
#include "drv_scpos.h"
#include "pmx_drvif.h"
#include "panel.h"
#include "eeprom_if.h"
#include "pdwnc_if.h"
#ifdef CC_SCPOS_3DTV_SUPPORT
BOOL bInitPWM2 = FALSE;
#endif
/*lint -e717*/
/*
e717 : do ... while(0)
*/
extern UINT32 PANEL_GetBacklightOffDelay(void);
extern UINT32 PANEL_GetLvdsOffDalay(void);
static UINT8 u1DebugLevel;
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
        if(DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 && 
           DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)value);
            if (DRVCUST_PanelQuery(eBackLightGpio2, &gpio) == 0)
            {
                vDrvGpioSet((INT32)gpio, (INT32)value);
            }
#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWERON, 0);
#endif 
        }
    } while (0);
}

void vDrvTurnOffBacklt(void)
{
    do
    {
        UINT32 gpio, value;
        if(DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 && 
           DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)!value);
            if (DRVCUST_PanelQuery(eBackLightGpio2, &gpio) == 0)
            {
                vDrvGpioSet((INT32)gpio, (INT32)!value);
            }
#ifndef CC_MTK_LOADER
            DRVCUST_UpdateBacklightState(BL_STATE_POWEROFF, 0);
#endif  

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

UINT32 _u4BrightLevel = 0x80;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//UINT32 BACKLT_GetCurrentPanelBright(void)
//{
//    return _u4BrightLevel;
//}
 #ifdef CC_SCPOS_3DTV_SUPPORT
void vApiSetScanningPWM(UINT16 u2HTotal, UINT16 u2VTotal, UINT32 u4Start, UINT32 u4High)
{
    if (DRVCUST_PanelGet(ePanelBacklightPwmPort) == 2) //pwm2
    {  
        vIO32WriteFldAlign(PWM_SCAN_03, u4Start*u2HTotal, REG_PWM_SCAN2_START);
        vIO32WriteFldAlign(PWM_SCAN_04, u4High*u2HTotal, REG_PWM_SCAN2_HIGH);
        vIO32WriteFldAlign(PWM_SCAN_05, (u2VTotal-u4Start-u4High)*u2HTotal, REG_PWM_SCAN2_LOW); 
    }
    else //pwm0/1
    {
        vIO32WriteFldAlign(PWM_SCAN_00, u4Start*u2HTotal, REG_PWM_SCAN1_START);
        vIO32WriteFldAlign(PWM_SCAN_01, u4High*u2HTotal, REG_PWM_SCAN1_HIGH);
        vIO32WriteFldAlign(PWM_SCAN_02, (u2VTotal-u4Start-u4High)*u2HTotal, REG_PWM_SCAN1_LOW);
    }

    LOG(1, "Start: %x, High: %x, Low: %x\n", u4Start, u4High, (u2VTotal-u4Start-u4High));
}
#endif

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

    if ((u4Dimming == 256)||(u4Dimming == 255))
    {
        vDrvSetPWM(
            DRVCUST_PanelGet(ePanelBacklightPwmPort),
            ((BSP_GetDomainClock(SRC_BUS_CLK)) / (PWM_DUTY_LESS+1)) / u4Frequency,
            0xFF,
            PWM_DUTY_LESS);
    }
    else
    {
        vDrvSetPWM(DRVCUST_PanelGet(ePanelBacklightPwmPort), 
        (BSP_GetDomainClock(SRC_BUS_CLK) >> 8) / u4Frequency, u4Dimming, PWM_DUTY_ALL);            
    }
}


UINT32 BACKLT_GetDimmingFrequency(void)
{
    UINT32 u4Freq = 0;

    if ((vDrvGetLCDFreq() == 25) || (vDrvGetLCDFreq() == 50) || (vDrvGetLCDFreq() == 100))
    {
        u4Freq = DRVCUST_PanelGet(eDimmingFreq50);
        if (u4Freq == 0)
        {
            u4Freq = PANEL_GetDimmingFrequency50Hz();
        }
    }
#ifdef SUPPORT_PANEL_48HZ
    else if ((vDrvGetLCDFreq() == 24) || (vDrvGetLCDFreq() == 48) || (vDrvGetLCDFreq() == 96))
    {
        u4Freq = PANEL_GetDimmingFrequency48Hz();
    }
#endif
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

    if(u1DebugLevel++ > 60)
    {
        LOG(3,">>Driver:vApiSetPanelBright:Frequency=%x, BrightLevel=%x\n",BACKLT_GetDimmingFrequency(),(UINT32)u1BrightLevel);
        u1DebugLevel = 0;
    }
    
    BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), (UINT32)u1BrightLevel);
}

extern UINT16 wCurValue;
void vApiSetPanelBrightTemp(UINT8 u1BrightLevel)
{
    wCurValue = u1BrightLevel;
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

    // Enable PWM1 for panel backlight boosting.
    IO_WRITE32MSK(IO_ADDR(0xd400), 0, 1 << 28, 1 << 28);

    if ((wLevel == 256) || (wLevel == 255))
    {
        vDrvSetPWM(
            SrcPWM1,
            ((BSP_GetDomainClock(SRC_BUS_CLK)) / (PWM_DUTY_LESS+1)) / DEFAULT_BOOSTING_FREQUENCY,
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
    static UINT8 u1PowerSequenceGoing = FALSE;
	UINT32 u4BootReason;

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

	u4BootReason = PDWNC_ReadWakeupReason();
	if ((u4BootReason == PDWNC_WAKE_UP_REASON_RTC) && bPDWNC_IsRTCWakeupBackltEnable())
	{
	   LOG(0, "vApiPanelPowerSequence: PDWNC_WAKE_UP_REASON_RTC.return\n");
        return;
    }

    if (fgEnable == PANEL_ON) //panel power on sequence
    {
        LOG(0, "vApiPanelPowerSequence= TRUE (START)");

        if (bIsPanelOn())
        {
            // already on.
            _fgPanelOn = TRUE;
            return;
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
        // aviod loader and system re-entrant before backlight on
        if (u1PowerSequenceGoing == TRUE)
        {
            LOG(0, "vApiPanelPowerSequence = TRUE (Going)");
            return;
        }
        u1PowerSequenceGoing = TRUE;
/*
#if defined(CC_MT5368) || defined(CC_MT5389)
   if(GetCurrentPanelIndex()== PANEL_MLVDS_SS_MB7)
    {		
     //#ifdef CC_SMALL_TCON_BOARD
        x_thread_delay(100);
       vTconCustTGLoadEEP();
       //  #ifndef CC_MTK_LOADER
       // DRVCUST_EepromLoadODTable();
       // DRVCUST_SetODTable();
              
       // vHalVideoGammaTblInit();
       //#endif        
     //#endif
    }
#endif
*/

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
        switch(PANEL_GetDisplayType())
        {
            default:
            case PANEL_DISPLAY_TYPE_MAX:
                // error display type, defaultly do nothing
            break;
            case PANEL_DISPLAY_TYPE_LVDS:
                vDrvLVDSOff();
                vDrvLVDSConfig();
                vDrvLVDSOn();
            break;
            case PANEL_DISPLAY_TYPE_MLVDS:
                vDrvLVDSConfig();
                u1DrvTconInit();                
            break;
#ifdef CC_MT5396
            case PANEL_DISPLAY_TYPE_VB1:
                vDrvLVDSOff();
                vDrvLVDSConfig();
                vDrvVByOneOn();
            break;
#endif
            case PANEL_DISPLAY_TYPE_EPI:
                u1DrvTconInit();                
            break;
        }
        #ifdef DRV_SUPPORT_EXTMJC
        if (IS_COMPANION_CHIP_ON())
        {
            UINT8 u1MsgErr;
            drv_extmjc_set_RX_reset(&u1MsgErr);
        }
        #endif

        // set pwm before enable backlight power supply, so that backlight would be expected pwm level once we supply the power
        BACKLT_SetDimming(BACKLT_GetDimmingFrequency(), _u4BrightLevel);

#ifndef __MODEL_slt__ // save booting time
        x_thread_delay(10 * PANEL_GetBacklightOnDelay());
#endif
        // turn on backlight GPIO, supply backlight power
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
#if 0        
        if(DRVCUST_InitGet(eFlagControlBacklightPWM)) 
        {
            vApiSetPanelBright(_u4BrightLevel);
        }
#endif        
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
        u1PowerSequenceGoing = FALSE;
        LOG(0, "vApiPanelPowerSequence= TRUE (END)");
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
        switch(PANEL_GetDisplayType())
        {
            default:
            case PANEL_DISPLAY_TYPE_MAX:
                // error display type, defaultly do nothing
            break;
            case PANEL_DISPLAY_TYPE_LVDS:
                vDrvLVDSOff();
            break;
            case PANEL_DISPLAY_TYPE_MLVDS:
            break;
#ifdef CC_MT5396
            case PANEL_DISPLAY_TYPE_VB1:
            break;
            case PANEL_DISPLAY_TYPE_EPI:
            break;
#endif
        }
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
    LOG(7, "BeforeOutputFrameRateChange ->%d\n", u4FrameRate);
    // some panel will crash when changing frame rate,
    // so we reset panel this moment to prevent it
/*
    if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
    {
        vApiPanelPowerSequence(FALSE);
    }
*/

    // update dimming frequency
    if(DRVCUST_InitGet(eFlagControlBacklightPWM)) 
    {
        vApiSetPanelBright(_u4BrightLevel);
    }
#ifndef CC_MTK_LOADER
    DRVCUST_UpdateBacklightState(BL_STATE_OUTPUT_FR_CHANGE, _u4BrightLevel);
#endif
}


void vApiVestelPDWNSequence(void)
{
	PDWNC_ConfigRTCWakeupBacklt(1);
	printf("\n-----------vApiVestelPDWNSequence :PDWNC_ConfigRTCWakeupBacklt  = 1\n");

    vDrvTurnOffBacklt();
    x_thread_delay(10 * PANEL_GetBacklightOffDelay());
    vDrvLVDSOff();
    x_thread_delay(10 * PANEL_GetLvdsOffDalay());
    vDrvTurnOffPanel(); // Panel off
    Printf("-test PDWN--#2****LvdsOffDalay(%d)--\n",PANEL_GetLvdsOffDalay());
    Printf("--test PDWN --#1****PANEL_GetBacklightOffDelay(%d)--\n",PANEL_GetBacklightOffDelay());

}

void vApiVestelPUPSequence(void)
{
    Printf("-test PUP --#2****LvdsOnDalay(%d)--\n",PANEL_GetLvdsOnDalay());
    Printf("--test PUP --#1****PANEL_GetBacklightOnDelay(%d)--\n",PANEL_GetBacklightOnDelay());
	PDWNC_ConfigRTCWakeupBacklt(0);
	printf("\n-----------vApiVestelPUPSequence :PDWNC_ConfigRTCWakeupBacklt  = 0\n");

    vDrvTurnOnPanel(); // Panel on
	
	x_thread_delay((10 * PANEL_GetLvdsOnDalay()) + 1);
  
	vDrvTurnOnPDPVs();
	  
	switch(PANEL_GetDisplayType())
	{
		default:
		case PANEL_DISPLAY_TYPE_MAX:
		  // error display type, defaultly do nothing
		break;
		case PANEL_DISPLAY_TYPE_LVDS:
		  vDrvLVDSOff();
		  vDrvLVDSConfig();
		  vDrvLVDSOn();
		break;
		case PANEL_DISPLAY_TYPE_MLVDS:
		  vDrvLVDSConfig();
		  u1DrvTconInit();				  
		break;
#ifdef CC_MT5396
		case PANEL_DISPLAY_TYPE_VB1:
		  vDrvLVDSOff();
		  vDrvLVDSConfig();
		  vDrvVByOneOn();
		break;
#endif
		case PANEL_DISPLAY_TYPE_EPI:
		  u1DrvTconInit();				  
	    break;
	}

    x_thread_delay(10 * PANEL_GetBacklightOnDelay());
	  
    vDrvTurnOnBacklt();
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

//-----------------------------------------------------------------------------
/** check if panel is already lighted on
 *  @param none
 *  @return 0 if not light yet, otherwise already lighted on
 */
//-----------------------------------------------------------------------------
UINT8 bIsPanelOn(void)
{
    UINT32 gpio, value; 
    if (DRVCUST_PanelQuery(eBackLightGpio, &gpio) == 0 &&
        DRVCUST_PanelQuery(eBackLightOnVal, &value) == 0 )
    {
        if((GPIO_Enable(gpio, NULL) == 1) && (GPIO_Output(gpio, NULL) == value))
        {
            return 1;
        }
    }
    else 
    if (DRVCUST_PanelQuery(ePanelPowerGpio, &gpio) == 0 && 
        DRVCUST_PanelQuery(ePanelPowerOnVal, &value) == 0) 
    {
        if((GPIO_Enable(gpio, NULL) == 1) && (GPIO_Output(gpio, NULL) == value))
        {
            return 1;
        }
    }

    if (_fgPanelOn != FALSE)
    {
        return 1;
    }

    return 0;
}

#ifdef CC_FAST_INIT
void ostg_set_fgPanelOn(BOOL onoff)
{
    _fgPanelOn = onoff;
}
#endif

static const UINT32 g_au4PlaneArray[PMX_MAX_INPORT_NS] = {
    PMX_OSD2, 
    PMX_OSD3, 
    PMX_OSD1,
    PMX_PIP,
    PMX_MAIN
}; 

//-----------------------------------------------------------------------------
/** LDR_PmxDisplay(): Setup Panel Pmx/LVDS driver.
 *  This function is going to setup panel pmx and lvds driver with background
 *  color.
 *
 *  @param  u4Background    This is background color setting. 0xRRGGBB.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_PmxDisplay(UINT32 u4Background)
{
    static UINT32 _fgInit = 0;
    UINT32 i;
 
    if (_fgInit)
    {
        PMX_SetBg(u4Background, FALSE);
        return 0;
    }
    _fgInit = 1;
#ifdef CC_MT5391
    vDrvVOPLLAutoKVCOBand();
#endif /* CC_MT5391 */

    PMX_Init();
    LoadPanelIndex();
    // power off panel
    vApiPanelPowerSequence(FALSE);

#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)
    vDrvLVDSInit();
    vErrorHandleInit();	
#endif

    vDrvDisplayInit();
    //vDrvLVDSInit();
    #if 0 // #ifdef CC_DVBT_SUPPORT, Fix me
    vDrvVOPLLSet(PANEL_GetPixelClk50Hz());
    #else
    // vDrvVOPLLSet(PANEL_GetPixelClk60Hz());
    #endif
    //vDrvSetLCDTiming();

#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)

#else
    vDrvLCDFreqSet(SV_DCLK_60HZ, (PANEL_IsSupport120Hz()?120 : 60));
#endif

    // set background color
    PMX_SetBg(u4Background, FALSE);
    // remove vdp plane
    PMX_SetPlaneOrderArray(g_au4PlaneArray);

    for (i=0; i<=PMX_UPDATE_DELAY; i++)
    {
        // Use OutputVSync to update PlaneOrder.
        PMX_OnOutputVSync();
    }
#if defined(CC_MT5395) || defined(CC_MT5365)
    switch(PANEL_GetDisplayType())
    {
        default:
        case PANEL_DISPLAY_TYPE_MAX:
            // error display type, defaultly do nothing
        break;
        case PANEL_DISPLAY_TYPE_LVDS:
            vDrvLVDSInit();
        break;
        case PANEL_DISPLAY_TYPE_MLVDS:
        break;
#ifdef CC_MT5395
        case PANEL_DISPLAY_TYPE_VB1:
        	vVByOneInit();
        break;
        case PANEL_DISPLAY_TYPE_EPI:
        break;
#endif
    }
#else
    #if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)

    #else
    vDrvLVDSInit();
    #endif
#endif

    // power on panel
    vApiPanelPowerSequence(TRUE);
    DumpPanelAttribute(PANEL_DUMP_CURRENT);

    return 0;
}


//-----------------------------------------------------------------------------
/**Vestel_PmxDisplay(): Setup Panel Pmx/LVDS driver.
 *  This function is going to setup panel pmx and lvds driver with background
 *  color.
 
 *  @param  u4Background    This is background color setting. 0xRRGGBB.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 Vestel_PmxDisplay(UINT32 u4Background)
{
    static UINT32 _fgInit = 0;
    UINT32 i;
 
    if (_fgInit)
    {
        PMX_SetBg(u4Background, FALSE);
        return 0;
    }
    _fgInit = 1;

#ifdef CC_MT5391
    vDrvVOPLLAutoKVCOBand();
#endif /* CC_MT5391 */

    PMX_Init();
    LoadPanelIndex();
    // power off panel
    vApiPanelPowerSequence(FALSE);

#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)
    vDrvLVDSInit();
    vErrorHandleInit();	
#endif

    vDrvDisplayInit();

    // set background color
    PMX_SetBg(u4Background, FALSE);
    // remove vdp plane
    PMX_SetPlaneOrderArray(g_au4PlaneArray);

    for (i=0; i<=PMX_UPDATE_DELAY; i++)
    {
        // Use OutputVSync to update PlaneOrder.
        PMX_OnOutputVSync();
    }

    // power on panel
//    vApiPanelPowerSequence(TRUE);

    return 0;
}

