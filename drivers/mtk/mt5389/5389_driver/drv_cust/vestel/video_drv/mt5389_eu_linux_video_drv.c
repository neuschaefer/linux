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
#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "pe_if.h"
#include "drv_nr.h"
#include "video_def.h"
#include "drv_video.h"
#include "api_backlt.h"
#include "drv_contrast.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_tvd.h"
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "x_debug.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "source_select.h"
#include "drv_ycproc.h"
#include "drv_contrast.h"
#include "drv_pq_cust.h"
#include "drv_dvi.h"
#include "srm_drvif.h"
#include "drv_meter.h"
#include "drv_od.h"
#include "hw_ospe.h"
#include "hw_scpos.h" //Caihua Add 20120605
#include "osd_drvif.h"//Caihua Add 20120605
#include "drv_tdtv_drvif.h"

// MTK Customized Driver
//#include "\..\mtk_video_drv.c"

/*****************************************************************************************/
/******************************  Local Dimming SPI customization   ******************************/
/*****************************************************************************************/
#include "hw_ycproc.h"
#include "nptv_debug.h"
#include "panel.h"
#include "drv_lcdim.h"
#include "drv_default.h"

extern BOOL _fgLdmISR;

static UINT8 gbSmartPicIndex = 0;
static UINT8 gbSceUi = 0;
static UINT8 gbOdUi = 0;
static UINT8 gbGammaUi = 0;
UINT8 Clarity_value=0;
UINT16 wGammaMute[11][3];

/*****************************************************************************************/
/******************************  Dynamic Backlight Control      *********************************/
/*****************************************************************************************/
UINT16 wPreValue;
UINT16 wCurValue;
UINT16 wFrameCount;

UINT8 bBackLightCtrl;
UINT8 bVideoPwmMin;
UINT8 bVideoPwmMed;
UINT8 bVideoPwmMax;
UINT8 bVideoPwmEco;
UINT8 isAdapDBLEnable;
UINT8 bAutoAPLPercent;
UINT8 bAutoBlackPixelPercent;
UINT8 bAutoPwmMinThd;
UINT8 bOsdEventPwm;
UINT8 bDecreaseCnt;
UINT8 bIncreaseCnt;
UINT8 bPanelPwmMin;
UINT8 bEcoSwitchPwmHiLimit; //high limit pwm of eco switch
UINT8 bEcoSwitchPwmLoLimit; //low limit pwm of eco switch
UINT32 u4DebugCount = 0;

enum
{
    backlight_dimming_low = 0, /* fixed low backlight value */
    backlight_dimming_med = 1,  /* fixed medium backlight value */
    backlight_dimming_high = 2,  /* fixed high backlight value */
    backlight_dimming_auto = 3, /* auto mode implementation, based on vendor capabilities */
    backlight_dimming_off = 4, /* screen off */
    backlight_dimming_eco = 5, /* ice implementation is a fixed backlight value , like in low, mid, high cases*/
    backlight_dimming_ambilight = 6, /*Ambilight Sensor active*/ 
};

UINT8 bBLMode = backlight_dimming_eco;
UINT8 bUiBLset;
UINT8 isMediaBrowserSrc = SV_FALSE;
UINT8 isHdmiTrickMode = SV_FALSE;
UINT8 fgInTrickMode = SV_FALSE;
UINT8 isOsdOnDisplay = SV_FALSE;
UINT8 fgBackLightOnOffSet = SV_TRUE;
UINT8 isTrickModeAvailable;
UINT8 bEcoSwitchMode = SV_FALSE;
UINT8 bEcoSwitchCtrl;
UINT8 bOsdEventCtrl;

#define DBL_BLK_PXL_OFFSET      40

//ECO Switch
#define PERC_RANGE_LOW          45 //around 50
#define PERC_RANGE_HIGH         55 //around 50
#define APL_PERC_THD_ECO        65

UINT16 BlackPixelCnt;
UINT16 wFrameCountNoVideo; //No video ~6s count (50Hz)

void vDrvOsdOnDisplayCheck(void);

UINT32 u4DrvGetAPLPercentage(void)
{
    UINT32 u4APL = (UINT32)bDrvGetAPL();

    // APL normalize to 100
    u4APL = (u4APL > 16) ? (((u4APL-16)*100)/219) : 0; 

    return u4APL;
}

extern UINT16 aUiVqDftBacklight[6][14];

#if 1 //get some local values
UINT32 vDrvGetBackLightGlobalValue(UINT32 ui4_picmode,UINT32 ui4_switch)
{
    if(ui4_picmode == PICTURE_MODE_SPORT)
    {
        ui4_picmode -= 1; //neglect PICTURE_MODE_PIXELLENCE, remap PICTURE_MODE_SPORT to PICTURE_MODE_PIXELLENCE
    }
    
    return aUiVqDftBacklight[ui4_picmode][ui4_switch]; 
}
#endif

 UINT32 u4DrvGetWhitePixelPercentage(void)
{
    UINT16 waHistCur[LUMA_HIST_NUM];
    UINT32 u4WhitePixelPerc;

    //Histogran Normalize to 100
    u1DrvGetLumaHist(waHistCur, 100);
    u4WhitePixelPerc = waHistCur[29]+waHistCur[30]+waHistCur[31];

    return u4WhitePixelPerc;
}   

 UINT32 u4DrvGetBlackPixelPercentage(void)
{
    UINT16 waHistCur[LUMA_HIST_NUM];
    UINT32 u4BlackPixelPerc;

    //Histogran Normalize to 100
    u1DrvGetLumaHist(waHistCur, 10000);
    u4BlackPixelPerc = (waHistCur[0]+waHistCur[1]+waHistCur[2])/100;

    return u4BlackPixelPerc;
}  

static UINT32 u4CalPWMByAPL(void)
{      
    UINT32 u4Pwm_APL;
    UINT32 u4APLPerc = u4DrvGetAPLPercentage();
   
    u4Pwm_APL = (u4APLPerc < bAutoAPLPercent) ? ((u4APLPerc*100)/bAutoAPLPercent) : 100;

    if (u4DebugCount == 0)
    {
        LOG(4, "DBL : APL Perc %d  Ratio %d  \n", u4APLPerc, u4Pwm_APL);
    }
    
    return u4Pwm_APL;
}

static UINT32 u4CalPWMByBlackPixel(void)
{
    UINT32 u4PWM_Offset;

    BlackPixelCnt = (UINT16)u4DrvGetBlackPixelPercentage();
    
    u4PWM_Offset = (BlackPixelCnt < bAutoBlackPixelPercent) ? DBL_BLK_PXL_OFFSET
                    : (40*(100-BlackPixelCnt)/(100-bAutoBlackPixelPercent));
    
    if (u4DebugCount == 0)
    {
        LOG(4, "DBL : Black pixel %d  Offset %d  \n", BlackPixelCnt, u4PWM_Offset);
    }
    
    return u4PWM_Offset;
}

UINT32 CalPWMFinalOutput(void)
{ 
    UINT32 u4FinalPwm;
    //UINT32 u4PwmMin, u4PwmOsdEvent;

    u4FinalPwm = ((u4CalPWMByAPL()+u4CalPWMByBlackPixel())*bVideoPwmMax)/100;
    u4FinalPwm = (u4FinalPwm > bVideoPwmMax) ? (bVideoPwmMax) : (u4FinalPwm); 

    return  u4FinalPwm;
}

static void FastDimmingMode(void)
{    
    if ((wPreValue-wCurValue)> 20)
     {
        wCurValue = (((wPreValue*100)-((wPreValue-wCurValue)*7))+50)/100;    
    }
    else
    {
        wCurValue = wPreValue - 1;
    }
}

static void SlowDimmingMode(void)
{
    wCurValue = wPreValue - 1;
}

static void IncreasingMode(void)
{
    if ((wCurValue-wPreValue) >= bIncreaseCnt)
    {
        wCurValue = (wCurValue+wPreValue+1)/2;
    }
}

void vDrvBacklightApplySetting(UINT8 bPwmValue)
{
    wCurValue = bPwmValue;
    vApiSetPanelBright(wCurValue);
    wPreValue = wCurValue;

    wFrameCount = 0;
    wFrameCountNoVideo = 0;
}

void vDrvBacklightCtrl(void)
{
    UINT32 u4PwmOsdEvent,u4PwmMin;
    
    if ((bBackLightCtrl) && (!isAdapDBLEnable))
    {
        // Not support Auto backlight, set max backlight
        vDrvBacklightApplySetting(bVideoPwmMax);
        return; 
    }

    if((bBackLightCtrl) && (fgIsMainVga()) &&(bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE))
    {
        //VGA/PC Source will be set to pwm max when signal's detected.
        vDrvBacklightApplySetting(bVideoPwmMax);
        return; 
    }
    
    if ((bGetSignalType(SV_VP_MAIN) == SV_ST_TV) && (_fgAutoSearch == TRUE))
    {
        return;
    }
    
    if((bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
        && (RegReadFldAlign(MUTE_01, B_MUTE_MAIN) != 0)
        && (bBackLightCtrl))
    {
         //if it's blue screen, it will use maximum pwm 
        vDrvBacklightApplySetting(bVideoPwmMax);
        return; 
    }
    
    if ((bBackLightCtrl) && (!_fgAutoSearch))
    {
        /*
        if (bDrvVideoSignalStatus(SV_VP_MAIN) == SV_VDO_STABLE) //Video is detected
        {
            wFrameCountNoVideo = 0;
            
            wCurValue = CalPWMFinalOutput();
            
            if (wPreValue > wCurValue) //Dimming Mode
            {
                wFrameCount = (wFrameCount < 25) ? (wFrameCount+1) : 25;

                if (wFrameCount == 25) 
                {                    
                    if (BlackPixelCnt == 100)
                    {  
                        FastDimmingMode();                                           
                    }
                    else //Not full black, has content in picture
                    {
                        SlowDimmingMode();                        
                    }
                }
                else
                {
                    wCurValue = wPreValue;
                }                
            }
            else //Increase Mode
            {
                wFrameCount = 0;
                IncreasingMode();
            }
        }
        else //video not detected
        {
            wFrameCount = 0;
            
            wFrameCountNoVideo = (wFrameCountNoVideo < 300) ? (wFrameCountNoVideo+1) : 300;
            
            if (wFrameCountNoVideo == 300)
            {
                if(wCurValue > bPanelPwmMin)
                {
                    SlowDimmingMode();
                }
            }                
            else
            {
                wCurValue = wPreValue;
            }  
        }        

        //min protect
        if(u4DrvGetBlackPixelPercentage()==100) //full black
        {
            u4PwmMin = bPanelPwmMin;
        }
        else //not full black
        {
            u4PwmMin = ((UINT32)bAutoPwmMinThd*bVideoPwmMax)/100; 
            u4PwmMin = (u4PwmMin < bPanelPwmMin)? bPanelPwmMin : u4PwmMin;
        }
        wCurValue = (wCurValue<u4PwmMin) ? u4PwmMin : wCurValue;

        //osd event check
        vDrvOsdOnDisplayCheck();
        if(isOsdOnDisplay)
        {
            u4PwmOsdEvent = ((UINT32)bOsdEventPwm*255)/100;
            wCurValue = (wCurValue < u4PwmOsdEvent) ? u4PwmOsdEvent : wCurValue;
        }

        if (u4DebugCount++ > 60)
        {
            u4DebugCount = 0; 
            LOG(3, "DBL : Pre %d  Cur %d  \n", wPreValue, wCurValue);
        }

        vApiSetPanelBright((UINT8)wCurValue);
        wPreValue = wCurValue; 
        */
        vApiSetPanelBright((UINT8)wCurValue);
    }
}

UINT8 vDrvGetHdmiTrickMode(void)
{
    return (isHdmiTrickMode);
}

void vDrvHDMITrickModeCtrl(void)
{
    if(vDrvGetHdmiTrickMode())
    {
        fgInTrickMode = SV_TRUE;
        bBackLightCtrl = SV_FALSE;

        if(fgBackLightOnOffSet)
        {
            vApiBackltONOFF(SV_OFF);
            fgBackLightOnOffSet = SV_FALSE;
        }
    }
    else
    {
        fgBackLightOnOffSet = SV_TRUE;
        
        if(fgInTrickMode)
        {       
            vApiBackltONOFF(SV_ON);
            vApiUpdateBacklightMode(bBLMode);
            fgInTrickMode = SV_FALSE;
        }
    }
}

void vDrvOsdOnDisplayCheck(void)
{    
    if(bOsdEventCtrl)
    {
          //isOsdOnDisplay = OSD_BASE_IsDispalying();
          
          LOG(3,">>>>>vDrvCheckOsdOnDisplay:OSD On Display=%d\n",isOsdOnDisplay);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Some Customization Fuction
static void vApiUpdateEcoSwitchSetting(void)
{
    if(bEcoSwitchMode)
    {
        bEcoSwitchCtrl = SV_TRUE;
        bOsdEventCtrl = SV_TRUE;
    }
    else
    {
        bEcoSwitchCtrl = SV_FALSE;        
        bOsdEventCtrl = SV_FALSE;
        vDrvBacklightApplySetting(bVideoPwmEco);
    }
}

static void vDrvEcoSwitchCtrl(void)
{
    if ((bGetSignalType(SV_VP_MAIN) == SV_ST_TV) && (_fgAutoSearch == TRUE))
    {
        return;
    }
    
    //if it's no signal blue screen, it will use maximum pwm 
    if((bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE)
        && (RegReadFldAlign(MUTE_01, B_MUTE_MAIN) != 0)
        && bEcoSwitchCtrl)
    {
        wCurValue = bVideoPwmMax;
        vApiSetPanelBright((UINT8)wCurValue);
        return;
    }
    
    //ECO Switch Mode
    if(bEcoSwitchCtrl &&( !_fgAutoSearch))
    {
        /*
        UINT32 u4PwmMin;
        UINT32 u4APLPerc = u4DrvGetAPLPercentage();
        UINT32 u4BlackPixelPerc = u4DrvGetBlackPixelPercentage();
        UINT32 u4WhitePixelPerc = u4DrvGetWhitePixelPercentage();

        if(((u4APLPerc >= PERC_RANGE_LOW) && (u4APLPerc <= PERC_RANGE_HIGH))
          &&((u4BlackPixelPerc >= PERC_RANGE_LOW) && (u4BlackPixelPerc <= PERC_RANGE_HIGH))
          &&((u4WhitePixelPerc >= PERC_RANGE_LOW) && (u4WhitePixelPerc <= PERC_RANGE_HIGH)))
        {
            wCurValue = (wCurValue < bVideoPwmMax) ? (wCurValue+1) : (bVideoPwmMax);
            LOG(5,"ECO Switch Mode:True\n");
        }
        else
        {
            UINT16 wAPLRatio = (UINT16)(u4APLPerc * 100)/APL_PERC_THD_ECO;
            UINT16 wCurValueTemp;

            wAPLRatio = (wAPLRatio > 100)? 100 : wAPLRatio;

            wCurValueTemp = (bEcoSwitchPwmHiLimit - bEcoSwitchPwmLoLimit)*wAPLRatio/100 + bEcoSwitchPwmLoLimit;

            //Protect
            wCurValueTemp = (wCurValueTemp < bEcoSwitchPwmLoLimit) ? bEcoSwitchPwmLoLimit : wCurValueTemp;
            wCurValueTemp = (wCurValueTemp > bEcoSwitchPwmHiLimit) ? bEcoSwitchPwmHiLimit : wCurValueTemp;
            LOG(5,"ECO Switch Mode:False, wAPLRatio=%d\n",wAPLRatio);

            if(wPreValue > wCurValueTemp+1)
            {
                wCurValue = wPreValue - 1;
            }
            else
            {
                wCurValue = wCurValueTemp;
            }
        }

        //Osd on screen
        vDrvOsdOnDisplayCheck();
        if(isOsdOnDisplay)
        {
            u4PwmMin = ((UINT32)bOsdEventPwm*255)/100;
            wCurValue = (wCurValue < u4PwmMin) ? u4PwmMin : wCurValue;
            if (u4DebugCount++ > 60)
            {
                u4DebugCount = 0;
                LOG(3,">>>>>Osd on screen:u4PwmMin=%d, bOsdEventPwm=%dn",u4PwmMin,bOsdEventPwm);
            }
        }

        vApiSetPanelBright((UINT8)wCurValue);
        wPreValue = wCurValue;

        if (u4DebugCount++ > 60)
        {
            u4DebugCount = 0;
            LOG(3,">>>>>vDrvEcoSwitchCtrl: u4APLPerc=%d, u4BlackPixelPerc=%d, u4WhitePixelPerc=%d\n",
                u4APLPerc,u4BlackPixelPerc,u4WhitePixelPerc);
            LOG(3,">>>>>vDrvEcoSwitchCtrl:wCurValue=%d\n",wCurValue);
        }
        */
        vApiSetPanelBright((UINT8)wCurValue);
    }
}

void vApiUpdateBacklightMode(UINT8 bMode)
{   
    if(bMode < backlight_dimming_off)
    {
        bUiBLset = bMode;
    }
    
    Printf(">>Backlight mode %d\n", bMode);
    vDrvDBLGetRegister();
    bBLMode = bMode;  

    if(bVideoPwmMax == 0)
    {
        return;
    }
    // 3D
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        LOG(1,"Backlight enter into 3D\n");
        bBackLightCtrl = SV_FALSE;
        bEcoSwitchCtrl = SV_FALSE;
        vDrvBacklightApplySetting(bVideoPwmMax);
        return;
    }
    
    //game mode
    if((vApiFlashPqGetSmartPicId() == PICTURE_MODE_GAME)||(vDrvGetMediaBrowserSource()))
    {
        LOG(1,"MediaBrowser Source Enable=%d(if=0, Game Mode=1)\n",vDrvGetMediaBrowserSource());
        bBackLightCtrl = SV_FALSE;
        bEcoSwitchCtrl = SV_FALSE;
        vDrvBacklightApplySetting(bVideoPwmMax);
        return;
    }

    //Dynamic mode without PowerSave settings
    if(PICTURE_MODE_DYNAMIC == gbSmartPicIndex)
    {
        bMode = bUiBLset;
    }

    switch (bMode)
    {
        case backlight_dimming_eco:
            bBackLightCtrl = SV_FALSE; 
            vApiUpdateEcoSwitchSetting();
            break;
        case backlight_dimming_low: //low
            bBackLightCtrl = SV_FALSE;
            bEcoSwitchCtrl = SV_FALSE;
            bOsdEventCtrl = SV_FALSE;
            vDrvBacklightApplySetting(bVideoPwmMin);
            break;
        case backlight_dimming_med: //med
            bBackLightCtrl = SV_FALSE;
            bEcoSwitchCtrl = SV_FALSE;
            bOsdEventCtrl = SV_FALSE;
            vDrvBacklightApplySetting(bVideoPwmMed);
            break;
        case backlight_dimming_high: //high
            bBackLightCtrl = SV_FALSE;
            bEcoSwitchCtrl = SV_FALSE;
            bOsdEventCtrl = SV_FALSE;
            vDrvBacklightApplySetting(bVideoPwmMax);
            return;        
        case backlight_dimming_auto: //auto
            bBackLightCtrl = SV_TRUE;
            bOsdEventCtrl = SV_TRUE;
            bEcoSwitchCtrl = SV_FALSE;
            break;
        case backlight_dimming_off: //screen off
            bBackLightCtrl = SV_FALSE;
            bEcoSwitchCtrl = SV_FALSE;
            bOsdEventCtrl = SV_FALSE;
            vDrvBacklightApplySetting(0);
            break;
        default:
            LOG(0, "Unknown backlight mode %d\n", bMode);
            break;
    }
}

void vDrvSetMediaBrowserSource(UINT8 bMode)
{
    isMediaBrowserSrc = bMode;
}

UINT8 vDrvGetMediaBrowserSource(void)
{
    return (isMediaBrowserSrc);
}

void vDrvSetHdmiTrickMode(UINT8 bMode)
{
       UINT16 u2BlackPixelCnt;

       LOG(6,">>>>>HDMI Trick Mode:bMode=%d\n",bMode);
       LOG(6,">>>>>HDMI Trick Mode:fgIsMainDVI()=%d\n",fgIsMainDVI());
       LOG(6,">>>>>HDMI Trick Mode:isOsdOnDisplay=%d\n",isOsdOnDisplay);
    
       if((fgIsMainDVI())&& (!isOsdOnDisplay))
       {
              u2BlackPixelCnt = (UINT16)u4DrvGetBlackPixelPercentage();
              LOG(3,">>>>>HDMI Trick Mode:u2BlackPixelCnt=%d(100)\n",u2BlackPixelCnt);
              if (u2BlackPixelCnt == 100)
              {
                    isTrickModeAvailable = SV_TRUE;
                    
                    if(bMode)
                    {
                         isHdmiTrickMode = SV_TRUE;
                         LOG(3,">>>>>HDMI Trick Mode:True\n");
                    }
                    else
                    {
                        isHdmiTrickMode = SV_FALSE;
                        LOG(3,">>>>>HDMI Trick Mode:False\n");
                    }
                    LOG(3,">>>>>HDMI Trick Mode:isTrickModeAvailable\n");   
              }
              else
              {
                   isTrickModeAvailable = SV_FALSE;
                   isHdmiTrickMode = SV_FALSE;
                   LOG(6,">>>>>HDMI Trick Mode:False\n");
              }
       }
       else
       {
              LOG(6,">>>>>HDMI Trick Mode:u2BlackPixelCnt=No need to check\n");
              isTrickModeAvailable = SV_FALSE;
              isHdmiTrickMode = SV_FALSE;
              LOG(6,">>>>>HDMI Trick Mode:False\n");
       }
}

void vDrvSetEcoSwitchMode(UINT8 bMode)
{
    bEcoSwitchMode = bMode;
}
//////////////////////////////////////////////////////////////////////////////////////////////


extern MMAppInfo sMMAppInfo;

UINT16 vApiFlashPqGetSceId(void)
{
    //This is sample code : SCE ID can be decided 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    return gbSceUi;
}

UINT16 vApiFlashPqGetMinSceId(void)
{
    //This is sample code : SCE ID can be decided 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    return (1);
}

void vApiFlashPqSetSceId(UINT8 bIndex)
{
    UINT8 u1SrcTypTmg;
    
    //Get Timing
    u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(SV_VP_MAIN);
    // Find a match SourceTypeTiming in CustomQtyIdx[].    
    u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg); 
    
    switch(u1SrcTypTmg)
    {
        case 0: //Tuner_PAL
        case 1: //Tuner_NTSC   
            bIndex =0;
            break;
        case 13: //DTV_SD
            bIndex =1;
            break;
        case 14: //DTV_HD
            bIndex =2;
            break;
        case 2: //CVBS_PAL
        case 3: //CVBS_NTSC
        case 4: //SCART_CVBS_PAL
        case 5: //SCART_CVBS_NTSC
        case 7: //SV_SCART_SV_50
        case 8: //SV_SCART_SV_60   
            bIndex =3;
            break;
        case 6: //SCART_RGB
            bIndex =4;
            break;
        case 9: //YPBPR_SD
        case 11: //HDMI_SD  
            bIndex =5;
            break;
        case 10: //YPBPR_HD
        case 12: //HDMI_HD
        case 15: //MM
            bIndex =6;
            break;
        case 16: //PC_VGA
            bIndex =7;
            break;
        default:
            LOG(0,"Unknow Source Type Timing u1SrcTypTmg=%d\n",u1SrcTypTmg);
            break;
    }
    gbSceUi = bIndex;
}

UINT16 vApiFlashPqGetOdId(void)
{
    //UINT8 bPanelIdx = GetCurrentPanelIndex();   
    
    return gbOdUi;
}

void vApiFlashPqSetOdId(UINT8 bIndex)
{
    gbOdUi = bIndex;
}

UINT16 vApiFlashPqGetGammaId(void)
{
    //This is sample code : GAMMA ID can be deside 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    switch (gbGammaUi)
    {
        case 1: //Cool 
            return 0;
        case 2: //Normal
            return 1;
        case 3: //Warm
            return 2;
        case 4: //Linear
            return 3;
        case 5: //Cool_3D 
            return 4;
        case 6: //Normal_3D
            return 5;
        case 7: //Warm_3D
            return 6;
        case 8: //Linear_3D
            return 3;
        default:
            return 0;
    }

#if 0 // This is sample code for different gamma table under 3D PR Panel
    switch (gbGammaUi)
    {
        case 1:
            // 3D Using Shutter Glass : IS_LVDS_DISP_3D_SHUTTER
            // 3D Using Polarized Panel : IS_LVDS_DISP_3D_POLARIZED
            // 
            // u4DrvTDTV3DModeQuery() : Query 3D Mode
            
            if((u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF) && (IS_LVDS_DISP_3D_POLARIZED))
            {               
                return 0;       // gamma table for PR panel
            }
            else
            {
                return 1;       // gamma table for normal state
            }
        default:
            return 0;
    }
#endif    
}

void vApiFlashPqSetGammaId(UINT8 bIndex)
{
    gbGammaUi = bIndex;
}

UINT16 vApiFlashPqGetQtyId(UINT8 bIndex)
{
    if (PANEL_GetPanelWidth() <= 1440)
    {
        return (0);//(1);
    }
    else
    {
        return (0);
    }
}

#ifdef __KERNEL__
    extern PE_UI_RANGE_TBL rPEUITable;
#endif

void vApiFlashPqSetSmartPicId(UINT8 bIndex)
{
#if 0 //ndef CC_DRIVER_PROGRAM    
#ifdef __KERNEL__  
    UINT8 i4UiIndex, j;
#endif
#endif
    Printf("Smart Pic XXXXX %d\n", bIndex);

    gbSmartPicIndex = bIndex;

#if 0 //ndef CC_DRIVER_PROGRAM
#ifdef __KERNEL__   
    ASSERT(rPEUITable.ptUiPicMinMaxDftTbl!=NULL && gbSmartPicIndex<rPEUITable.bSmartPicNum);   
    // update all source UI default table, since the table is same
    for (i4UiIndex = 0; i4UiIndex < (UINT8)EVDP_VIS_MAX; i4UiIndex++)
    {
        for(j=0; j< rPEUITable.bUIPQItemNum; j++)
        {
            if(rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType >=PE_ARG_BRIGHTNESS &&
                rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType < PE_ARG_NS)
            {
/*              printf("rPEUITable.ptUiPicMinMaxDftTbl[%d][%d].rPQItemType| %x, Min %x, Max %x, Dft %x \n", gbSmartPicIndex,j,rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType,
                    rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange.i4Min, rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange.i4Max,
                    rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange.i4Dft);
*/
                PE_SetUiRange(i4UiIndex, rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemType, 
                                                         &(rPEUITable.ptUiPicMinMaxDftTbl[gbSmartPicIndex][j].rPQItemRange));
            }
        }
    }
#endif
#endif

}

UINT8 vApiFlashPqGetSmartPicId(void)
{
    return (gbSmartPicIndex);
}

void DRVCUST_HwInit(void)
{
    return;
}

void DRVCUST_VideoInit(void)
{
    DRVCUST_AdaptiveBacklightInit();
    return;
}

void DRVCUST_VdoModeChgDone(UINT8 bPath)
{
    UNUSED(bPath);
    return;
}

void DRVCUST_VideoMainloop(void)
{
    //DRVCUST_AdaptiveBacklightProc();
    //
    vDrvDBLGetRegister();
    vDrvBacklightCtrl();
    vDrvEcoSwitchCtrl();
    //vDrvHDMITrickModeCtrl();
}

void DRVCUST_OutVSyncISR(void)
{
    return;
}

void DRVCUST_VdoInISR(void)
{
    return;
}

UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value)
{
    return u4Value<<2;
}

UINT16 DRVCUST_Y2RMap(void)
{
    //HSI SPEC change , Always -16 For video mode / Special Mode , 
    //because of DEMO Mode Support
    return  0;
}

/* @ Auto NR default Initial setting
 *      1: low
 *      2: Middle
 */
UINT8 DRVCUST_AutoNRDefSet(void)
{
    return 2;
}

/* @ Auto NR noise level implement
 * @ return noise level
 */
UINT8 DRVCUST_NoiseLevel(void)
{
#if 0
    UINT8 bNoiseLevel = NM_INVALIDLEVEL;
    UINT8 bRFNMLevel;

    if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
             (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_TV))// TVD RF source
    {

        bRFNMLevel = bTvdSignalStrengthLevel(1);   //RF level 0 ~ 80

        if(bRFNMLevel > 75)
        {
            bNoiseLevel = NM_LEVEL1_3;
        }
        else if(bRFNMLevel > 69)
        {
            bNoiseLevel = NM_LEVEL2;
        }
        else if(bRFNMLevel > 59)
        {
            bNoiseLevel = NM_LEVEL3;
        }
        else if(bRFNMLevel > 55)
        {
            bNoiseLevel = NM_LEVEL4;
        }
        else
        {
            bNoiseLevel = NM_LEVEL5;
        }
    }
    else
    {
        bNoiseLevel = vDrvNMLevel();   //content NR, Noise level 0 ~ 4
    }
    
    return bNoiseLevel;
#endif
    return 0;
}

void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
    UNUSED(u4GammaSel);
}

void DRVCUST_SetPedestalCtrl(UINT8 bLevel)
{    
#if 0 // Below is SAMPLE CODE for function customization.
    if(fgIsMainVga()||fgIsMainDVI())
    {
        SET_MATRIX_PED(SV_OFF);
        
        switch (bLevel)
        {
            case SV_OFF:
                vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_FULL);
                break;
            case SV_ON:
                vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_LIMIT);
                break;
            default:
                vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
                break;                
        }
    }
    else if (fgIsMainTvd3d()||fgIsMainYPbPr())
    {
        vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);

        switch (bLevel)
        {
            case SV_ON:
                SET_MATRIX_PED(SV_ON);
                break;
            case SV_OFF:
            default:
                SET_MATRIX_PED(SV_OFF);
                break;   
        }
    }
    else
    {
        SET_MATRIX_PED(SV_OFF);
        vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
    }
#endif
}

UINT8 DRVCUST_ColorTransform(UINT8 bPath, UINT8 bColorSys, UINT8 bHdtv)
{
    UINT8 bMonColorTransform;
    UINT8 bIsVideoTiming;

    bIsVideoTiming = (bGetSignalType(bPath) == SV_ST_DVI) ? bDviIsVideoTiming() : SV_TRUE;
    
    switch (bColorSys)
    {
        case SV_COLOR_YCBCR:
        case SV_COLOR_YCBCR_RGB:
        case SV_COLOR_YPBPR_ANALOG:
        case SV_COLOR_YPBPR_DIGITAL:
        case SV_COLOR_HDMI_422_601:
        case SV_COLOR_HDMI_444_601:
        case SV_COLOR_HDMI_422_709:
        case SV_COLOR_HDMI_444_709:
            if (bIsScalerInputRGB(bPath))
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR601L_2_RGBF : SV_COLORTRANS_YCBCR601F_2_RGBF;
                }
                else    //HDTV
                {
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR709L_2_RGBF : SV_COLORTRANS_YCBCR709F_2_RGBF;
                }
            }
            else
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR601L_2_YCBCR601L : SV_COLORTRANS_YCBCR601F_2_YCBCR601L;
                }
                else    //HDTV
                {
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR709L_2_YCBCR601L : SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
                }
            }
            break;
        case SV_COLOR_HDMI_RGB:
            if (bIsScalerInputRGB(bPath))
            {
                bMonColorTransform = bIsVideoTiming ? 
                    SV_COLORTRANS_RGBL_2_RGBF : SV_COLORTRANS_RGBF_2_RGBF;
            }
            else
            {
                bMonColorTransform = bIsVideoTiming ? 
                    SV_COLORTRANS_RGBL_2_YCBCR601L : SV_COLORTRANS_RGBF_2_YCBCR601L;
            }
            break;
        case SV_COLOR_RGB:
        default:
            bMonColorTransform = bIsVideoTiming ? 
                SV_COLORTRANS_RGBL_2_RGBF : SV_COLORTRANS_RGBF_2_RGBF;
            break;
    }
    return bMonColorTransform;
}

UINT8 DRVCUST_MMAPPGetTiming(UINT8 bOutputTiming)
{
    if (sMMAppInfo.u1AppType == 0xFF) //Pretend for Netflix App
    {
        // Temp threshold
        bOutputTiming = (sMMAppInfo.u4BitRate < 100) ?  
            SOURCE_TYPE_TIMING_MM_720P_SD : SOURCE_TYPE_TIMING_MM_720P_HD;
    }

    LOG(5, "DRVCUST_MMAPPGetTiming %d %d\n", sMMAppInfo.u4BitRate, bOutputTiming);

    return (bOutputTiming);
}


void DRVCUST_SetODTable(void)
{
    //get OD table from flashPQ
    bApiFlashPqUpdateOD();
    vApiRegisterVideoEvent(PE_EVENT_OD, SV_VP_MAIN, SV_ON);
}

UINT32 DRVCUST_DemoRegion(void)
{
    return VDP_PQ_DEMO_ON_RIGHT;//VDP_PQ_DEMO_ON_LEFT;
}

void vDrvWriteGammaBuf(UINT8 bIndex, UINT16 wRal, UINT16 wGal, UINT16 wBal)
{
    wGammaMute[bIndex][0] = wRal;
    wGammaMute[bIndex][1] = wGal;
    wGammaMute[bIndex][2] = wBal;
}
void DRVCUST_SET_GAMMA(void)
{
    /* ============================= Sample code ===================================
    
    1. Read gamma data from EEPROM, maybe need some check flow
        => Example 8bit 6 control points with 12bit gamma data
        INT32 iCtlPos[6] =   {0x000, 0x020, 0x040, 0x080, 0x0C0, 0x0FF};
        INT32 iCtlValue_R[6] = {0x013, 0x233, 0x3E5, 0x7C4, 0xB20, 0xEF4};

    2. Interplaotion to 257 red gamma table
        static INT32 OutputLut[257];
        vDrvGammaInterpolation(6, iCtlPos, iCtlValue_R, OutputLut);        

    3. Copy 257 data to u2GammaDecodedTable[] 
        
    ============================= Sample code End ===================================*/        
}
void DRVCUST_SET_YGAMMA(UINT32 i1Mode)
{
    EXTERN void vDrvADLSetYGamma(UINT8 bIndex);
    
    switch((INT8)i1Mode)
    {
        case -3:
            vDrvADLSetYGamma(9);
            break;
        case -2:
            vDrvADLSetYGamma(8);
            break;

        case -1:
            vDrvADLSetYGamma(7);

            break;
        case 1:
            vDrvADLSetYGamma(1);
            break;

        case 2:
            vDrvADLSetYGamma(3);
            break;
        case 3:
            vDrvADLSetYGamma(5);
            break;

        default:
            vDrvADLSetYGamma(0);
            break;
    }

    return;
}
void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo)
{

}

void DRVCUST_ClarityUpdate(INT8 u1Value)
{
    UNUSED(u1Value);
    return;
}

void DRVCUST_MMAPProc(void)
{
    if (sMMAppInfo.u1AppType == 0xFF) /*Pretend for Netflix App */
    {
        //Update new QtyTbl
        bApiVideoProc(SV_VP_MAIN, PE_ARG_3D_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_MPEG_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_SHARPNESS);
    }
}

