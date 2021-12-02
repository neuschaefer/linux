/*==========================================================================*/
/*
;     SOURCE_FILE:    imvideo_p.h
;     APPLICATION:    Funai2k9M
;     COMPONENT:      Video
;     VERSION:        %version: 101L_FB3 %
;     DATE:           %date_created: Thu Dec 24 11:03:10 2009 %
;     MODIFIER:       %derived_by: xuemd %
;
;     %full_filespec: imvideo_p.h,101L_FB3:incl:sgp88mtk#2 %
;
;     (C)2007 Philips Consumer Electronics - LoB Mainstream Displays
;
;     All rights are reserved. Reproduction in whole or in part is
;     prohibited without the prior written consent of the copyright
;     owner. The information presented in this document does not
;     form part of any quotation or contract, is believed to be
;     accurate and reliable and may be changed without notice.
;     No liability will be accepted by the publisher for any
;     consequence of its use. Publication thereof does not convey
;     nor imply any license under patent- or other industrial or
;     intellectual property rights.
*/
#ifndef _IMVIDEO_P_H
#define _IMVIDEO_P_H

/*==========================================================================*/
/*     Includes                                                             */
/*==========================================================================*/
//#include "../mcommon/mtypedef.h"
#include "mtypedef.h"
/*==========================================================================*/
/*     Typedefs, enums, defines                                             */
/*==========================================================================*/
#if 0
enum e_anr_source {
    PHI_SRC_ANA_TV_PAL = 0,     // Analog TV-PAL
    PHI_SRC_ANA_TV_SECAM,       // Analog TV-SECAM
    PHI_SRC_DTV_SD,             // Digital TV-SD
    PHI_SRC_DTV_HD,             // Digital TV-HD
    PHI_SRC_CVBS_SVHS_PAL,      // CVBS/SVHS (PAL/NTSC)
    PHI_SRC_CVBS_SVHS_SECAM,    // CVBS/SVHS (SECAM)
    PHI_SRC_RGB_SCART,          // RGB Scart
    PHI_SRC_YPBPR_480_576,      // YPbPr SD 480i/p, 576i/p
    PHI_SRC_YPBPR_720_1080,     // YPbPr HD 1080i/p, 720p
    PHI_SRC_HDMI_SD,            // HDMI SD 480i/p, 576i/p
    PHI_SRC_HDMI_HD,            // HDMI HD 1080i/p, 720p
    PHI_SRC_PC_DIGITAL,         // PC Digital (HDMI PC)
    PHI_MAX_SRC
};
#endif

// source list is updated based on PQ Video Processing HSI document by Win Naing
// source list is updated based on Dali 2K10 PQ Video Processing HSI document by Win Naing on 2009.12.9
enum e_anr_source {
    PHI_SRC_ANA_TV_PAL = 0,     // Analog TV-PAL
    PHI_SRC_ANA_TV ,            // Analog TV-NTSC
    PHI_SRC_DTV_SD,             // Digital TV-SD
    PHI_SRC_DTV_HD,             // Digital TV-HD
    PHI_SRC_CVBS_SVHS,          // CVBS/SVHS (PAL/NTSC)
    PHI_SRC_YPBPR_480_576,      // YPbPr SD 480i/p, 576i/p
    PHI_SRC_YPBPR_720_1080,     // YPbPr HD 1080i/p, 720p
    PHI_SRC_HDMI_SD,            // HDMI SD 480i/p, 576i/p
    PHI_SRC_HDMI_HD,            // HDMI HD 1080i/p, 720p
    PHI_SRC_PC_DIGITAL,         // PC Digital (HDMI PC)
    PHI_SRC_USB_VIDEO_PLAYER_SD,// USB Video Player SD
    PHI_SRC_USB_VIDEO_PLAYER_HD,// USB Video Player HD
    PHI_MAX_SRC
};

#define PWM_FREQ_50HZ   0
#define PWM_FREQ_60HZ   1

/*==========================================================================*/
/*     Functions Prototypes                                                 */
/*==========================================================================*/
/*==========================================================================
    FUNCTION NAME   : IMvideoP_VsyncUpdateControl
    DESCRIPTION     : Update BL Control in Vsync
    INPUTS          : nil
    OUTPUTS         : nil
    CALLING SEQUENCE: This functions to be called only after InitWarm and BgTaskBacklightControl are completed during start up
                      IMvideoP_InitWarm -> IMvideoP_BgTaskBackLightControl -> IMvideoP_VsyncUpdateControl
    REMARKS         : called in vsync ISR
    ==========================================================================*/
extern void IMvideoP_VsyncUpdateControl(void);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_BgTaskBackLightControl
    DESCRIPTION     : Main loop for Backlight algo
    INPUTS          : nil
    OUTPUTS         : nil
    CALLING SEQUENCE: called periodic every 20ms
    REMARKS         : void CAlgorithm::Update(void)
  ==========================================================================*/
extern void IMvideoP_BgTaskBackLightControl(void);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_BackLightEnableDisable
    DESCRIPTION     : Enable/disable Backlight Algo
    INPUTS          : on_off
		     =========
                      TRUE - Backlight algo enable
                      FALSE - Backlight algo disable

                      condition
                      =========
                      0: duty cycle = 50 %
                      1: duty cycle = 100 %
                      2: pause update
                      3: Use PWM duty cycle from Auto BL

    OUTPUTS         : nil
    CALLING SEQUENCE: Please see remarks for condition
    REMARKS         : 
                    Auto Backlight is Disable and set at 50% PWM in these scenarios (on_off = FALSE, condition =0)
                    1) When video is muted (Blue Mute) or Raster only 

                    Auto Backlight is Disabled and set 100% PWM in these scenarios  (on_off = FALSE, condition = 1)
                    1) PC mode 
                    2) When in Factory/service mode (SAM/SDM/CSM) 
                    3) View4U/JPEG viewer/USB video player 
                    4) Both Active control and Dynamic Contrast Feature Selection/UI are set to off.
                    5) Cold/Warm start 
                    6) Channel installation (Auto/Manual) 

                    Auto BL dimming is temporarily stop updating and keep the previous setting in these scenarios (on_off = FALSE, condition = 2)
                    1) Channel change period 
                    2) UI (Menu) enable to UI (Menu) disable period. 

                    Auto BL operates as per normal in these scenarios (on_off = TRUE, condition = 3)
                    1) Subtitle/Close Caption 
                    2) View mode change 
                    3) Demo mode 
                    4) Small area OSD and small UI bar 
                    5) All picture submenu items
  ==========================================================================*/
extern void IMvideoP_BackLightEnableDisable(Bool on_off, Byte condition);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_InitWarm
    DESCRIPTION     : initialize MD video components
    INPUTS          : nil
    OUTPUTS         : nil
    CALLING SEQUENCE: called during system initialisation
    REMARKS         : nil
  ==========================================================================*/
extern void IMvideoP_InitWarm(void);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_SetFeatureSelectionBytes 
    DESCRIPTION     : To pass the PQ feature selection bytes to MD video driver layer
    INPUTS          : bFeatureByte1
                        Bits    Features
                        ----    --------------
                        [7]     DNM Level (Min, Max)
                        [6]     DNM (On, Off)
                        [5]     Active Control 
                        [4]     Light Sensors
                        [3]     Color Enhance Level(Min, Max)
                        [2]     Color Enhance (On, Off)
                        [1]     MPEG Artifact Reduction (Deblocking)
                        [0]     Reserved

                      bFeatureByte2
                        Bits    Features
                        ----    --------------
                        [7]     Auto Backlight
                        [6]     Additional Dimming
                        [5]     Contrast Reserve
                        [4]     Backlight Boost
                        [3]     Eco APL Gain 
                        [2]     Eco APL Enable
                        [1]     Blue Stretch
                        [0]     Reserved
                        
                      bFeatureByte3
                        Bits    Features
                        ----    --------------
                        [7]     Dynamic Contrast Level (Min, Max)
                        [6]     Dynamic Contrast (On, Off)
                        [5]     100Hz Clear LCD 
                        [4]     Advanced Sharpness
                        [3]     Auto Backlight(UI) (00:OFF 01:basic 10:best power 11:best picture)
                        [2]     Auto Backlight(UI)
                        [1]     DNR (UI) (00: off 01:min 10:med 11:max)
                        [0]     DNR (UI)
                        
                      bFeatureByte3
                        Bits    Features
                        ----    --------------
                        [7]     Reserved
                        [6]     Reserved
                        [5]     Reserved 
                        [4]     Reserved
                        [3]     Reserved
                        [2]     Reserved
                        [1]     Reserved
                        [0]     Reserved
    OUTPUTS         : nil
    CALLING SEQUENCE: to be called when applying smart picture PQ feature profile
    REMARKS         : refer video - PQ HSI by Win Naing
  ==========================================================================*/
extern void IMvideoP_SetFeatureSelectionBytes(Byte bFeatureByte1, Byte bFeatureByte2, Byte bFeatureByte3, Byte bFeatureByte4);

/*==========================================================================
    FUNCTION NAME   : Mvideo_anr_SetCurrentSource
    DESCRIPTION     : To safe keep the current source and apply ANR 
    INPUTS          : Pls refer enum e_anr_source
    OUTPUTS         : nil
    CALLING SEQUENCE: to be called whenever a source change event
    REMARKS         : nil
  ==========================================================================*/
extern void IMvideoP_SetCurrentSource(Byte bSource);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_AutoNoiseEnableDisable
    DESCRIPTION     : Enable/disable ANR algo
    INPUTS          : TRUE - ANR Enable
                      FALSE - ANR Disable
    OUTPUTS         : nil
    CALLING SEQUENCE: Please see remarks
    REMARKS         : NR is now completely control by MTK, Do not call this API 
                      This API is no longer applicable for Funai2k9M project
  ==========================================================================*/
extern void IMvideoP_AutoNoiseEnableDisable(Bool on_off);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_GetBackLightPWMFrequency
    DESCRIPTION     : Get current video input signal frequency and return backlight PWM frequency
    INPUTS          : freq
                      0 - PWM_FREQ_50HZ
                      1 - PWM_FREQ_60HZ
    OUTPUTS         : Output PWM freq in Hz
    CALLING SEQUENCE: called when setting of PWM frequency
    REMARKS         : for eg. the output would set to 180 Hz (given input PWM_FREQ_60HZ)
  ==========================================================================*/
extern Byte IMvideoP_GetBackLightPWMFrequency(Byte freq);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_GetPWMdutyCycle 
    DESCRIPTION     : To return the PWM duty Cycle
    INPUTS          : nil
    OUTPUTS         : PWM duty Cycle [min_dimming_pwm .. max_dimming_pwm]
    CALLING SEQUENCE: nil
    REMARKS         : Used by Philips Eco Power component only
  ==========================================================================*/
extern Byte IMvideoP_GetPWMdutyCycle(void);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_GetMinDimmingPWM
    DESCRIPTION     : To return the Min PWM duty Cycle
    INPUTS          : nil
    OUTPUTS         : min_dimming_pwm 
    CALLING SEQUENCE: nil
    REMARKS         : Used by Philips Eco Power Component only
  ==========================================================================*/
extern Byte IMvideoP_GetMinDimmingPWM(void);

/*==========================================================================
    FUNCTION NAME   : IMvideoP_GetMaxDimmingPWM
    DESCRIPTION     : To return the Max PWM duty Cycle
    INPUTS          : nil
    OUTPUTS         : max_dimming_pwm
    CALLING SEQUENCE: nil
    REMARKS         : Used by Philips Eco Power Component only
  ==========================================================================*/
extern Byte IMvideoP_GetMaxDimmingPWM(void);

#endif // _IMVIDEO_P_H
