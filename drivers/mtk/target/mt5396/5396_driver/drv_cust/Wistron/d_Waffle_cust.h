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
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: d_Waffle_cust.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *---------------------------------------------------------------------------*/

#ifndef _D_WAFFLE_CUST_H_
#define _D_WAFFLE_CUST_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/* TV Source */
#define UART_WAKEUP_SUB_REASON_TV_CH_MIN 0
#define UART_WAKEUP_SUB_REASON_TV_CH_MAX 199

/* AV Source */
#define UART_WAKEUP_SUB_REASON_AV_START  UART_WAKEUP_SUB_REASON_CVBS_1
#define UART_WAKEUP_SUB_REASON_CVBS_1    200
#define UART_WAKEUP_SUB_REASON_CVBS_2    201
#define UART_WAKEUP_SUB_REASON_CVBS_3    202
#define UART_WAKEUP_SUB_REASON_CVBS_4    203
#define UART_WAKEUP_SUB_REASON_HDMI_1    204
#define UART_WAKEUP_SUB_REASON_HDMI_2    205
#define UART_WAKEUP_SUB_REASON_HDMI_3    206
#define UART_WAKEUP_SUB_REASON_HDMI_4    207
#define UART_WAKEUP_SUB_REASON_YPBPR_1   208
#define UART_WAKEUP_SUB_REASON_YPBPR_2   209
#define UART_WAKEUP_SUB_REASON_YPBPR_3   210
#define UART_WAKEUP_SUB_REASON_YPBPR_4   211
#define UART_WAKEUP_SUB_REASON_PC_1      212
#define UART_WAKEUP_SUB_REASON_AV_END    UART_WAKEUP_SUB_REASON_PC_1

// PCB configuration
#define DRV_CUSTOM_MODEL_60HZ_PANEL                 (0)
#define DRV_CUSTOM_MODEL_120HZ_PANEL                (1 << 0)
#define DRV_CUSTOM_MODEL_AUD_NORMAL                 (0 << 1)
#define DRV_CUSTOM_MODEL_AUD_5_1_CHANEL             (1 << 1)
#define DRV_CUSTOM_MODEL_FACTORY_WISTRON            (0 << 2)
#define DRV_CUSTOM_MODEL_FACTORY_SONY               (1 << 2)
#define DRV_CUSTOM_MODEL_WXGA_PANEL                 (0 << 3)
#define DRV_CUSTOM_MODEL_FHD_PANEL                  (1 << 3)
#define DRV_CUSTOM_MODEL_CCFL_PANEL                 (0 << 4)
#define DRV_CUSTOM_MODEL_LED_PANEL                  (1 << 4)
#define DRV_CUSTOM_MODEL_NO_DVD                     (0 << 5)
#define DRV_CUSTOM_MODEL_WITH_DVD                   (1 << 5)
#define DRV_CUSTOM_MODEL_AUD_NO_WOOFER              (0 << 6)
#define DRV_CUSTOM_MODEL_AUD_WOOFER                 (1 << 6)
#define DRV_CUSTOM_MODEL_1H			    (0 << 7)
#define DRV_CUSTOM_MODEL_2H			    (1 << 7)

#define DRV_CUSTOM_LOGO_ACTIVE                      (1 << 0)
#define DRV_CUSTOM_LOGO_DISABLE                     (0 << 0)
#define DRV_CUSTOM_LOGO_MUSIC_ACTIVE                (1 << 1)
#define DRV_CUSTOM_LOGO_MUSIC_DISABLE               (0 << 1)

// Country code
#define DRV_CUSTOM_COUNTRY_EU                       0
#define DRV_CUSTOM_COUNTRY_PAA                      1
#define DRV_CUSTOM_COUNTRY_PAD                      2
#define DRV_CUSTOM_COUNTRY_COL                      3
#define DRV_CUSTOM_COUNTRY_US                       4
#define DRV_CUSTOM_COUNTRY_UNKOWN                   0xFFFFFFFF

#define DRV_CUSTOM_COUNTRY_OPTION_PAA               0
#define DRV_CUSTOM_COUNTRY_OPTION_EU                4
#define DRV_CUSTOM_COUNTRY_OPTION_PAD               5
#define DRV_CUSTOM_COUNTRY_OPTION_COL               7

#define DRV_CUSTOM_MONITOR_OUT_GPIO                 (24)
#ifndef WAFFLE_PCB_PROTO
#define DRV_CUSTOM_MONITOR_OUT_POLARITY             (1)
#define DRV_CUSTOM_CVBS_IN_POLARITY                 (0)
#else
#define DRV_CUSTOM_MONITOR_OUT_POLARITY             (0)
#define DRV_CUSTOM_CVBS_IN_POLARITY                 (1)
#endif

/* follow AP option bit definition */
#define DRV_CUSTOM_MODELID_REGION      (0x6C7)
#define DRV_CUSTOM_MODELID_DVD         (0x6CA)

typedef enum
{
    DRV_CUSTOM_MODEL_OPTION_E = 0,
    DRV_CUSTOM_MODEL_OPTION_E_PLUS,
    DRV_CUSTOM_MODEL_OPTION_L,
    DRV_CUSTOM_MODEL_OPTION_P,
    DRV_CUSTOM_MODEL_OPTION_C,
    DRV_CUSTOM_MODEL_OPTION_F1,
    DRV_CUSTOM_MODEL_OPTION_F1S,
    DRV_CUSTOM_MODEL_OPTION_LAST_VALID_ENTRY,
} DRV_CUSTOM_CUST_MODEL_TYPE_T;

typedef enum
{
    DRV_CUSTOM_CUST_SPEC_TYPE_BEGIN = 0,

    DRV_CUSTOM_CUST_SPEC_TYPE_DEMO_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_PATH,
    DRV_CUSTOM_CUST_SPEC_TYPE_MOTIONFLOW_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_PICTURE_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_CINE_MOTION_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_GRAPHIC_AREA,
    DRV_CUSTOM_CUST_SPEC_TYPE_DCC,
    DRV_CUSTOM_CUST_SPEC_TYPE_MASK_AREA,
    DRV_CUSTOM_CUST_SPEC_TYPE_INPUT_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_MICRO_P_NVM_UPGRADE,

    DRV_CUSTOM_CUST_SPEC_TYPE_FACTORY_READ_REGISTER,
    DRV_CUSTOM_CUST_SPEC_TYPE_FACTORY_WRITE_REGISTER,
    DRV_CUSTOM_CUST_SPEC_TYPE_FACTORY_EXC_COMMAND,

    DRV_CUSTOM_CUST_SPEC_TYPE_PCB_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_LED_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_STANDBY_LED_DISABLE_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_SELF_DIAG_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_BOOT_COUNT_MODE,
    DRV_CUSTOM_CUST_SPEC_TYPE_LOADER_UPGRADE_MODE,

    DRV_CUSTOM_CUST_SPEC_TYPE_LOGO_STATE,
    DRV_CUSTOM_CUST_SPEC_TYPE_LOGO_DISPLAY,
    DRV_CUSTOM_CUST_SPEC_GET_TAG_VERSION,
    DRV_CUSTOM_CUST_SPEC_TYPE_BACKLIGHT_STATUS,
    DRV_CUSTOM_CUST_SPEC_TYPE_HDMI_EDID,
    DRV_CUSTOM_CUST_SPEC_TYPE_HDMI_HDCP,  //jiewen.hdcp
    DRV_CUSTOM_CUST_SPEC_TYPE_MONITOR_OUT,
    DRV_CUSTOM_CUST_SPEC_TYPE_STARTUP_SOUND_STATUS,   /* Allen Kao: wait for start-up sound playback over */
    DRV_CUSTOM_CUST_SPEC_GET_DIMMING_FREQ_48,
    DRV_CUSTOM_CUST_SPEC_GET_DIMMING_FREQ_50,
    DRV_CUSTOM_CUST_SPEC_GET_DIMMING_FREQ_60,

    DRV_CUSTOM_CUST_SPEC_TYPE_SS_PERMILLAGE,
    DRV_CUSTOM_CUST_SPEC_TYPE_TUNER_CIS,
    DRV_CUSTOM_CUST_SPEC_TYPE_PCB_TYPE,

    /*Customer specific command start*/
    DRV_CUSTOM_CUST_SPEC_TYPE_MMP_MODE,                     /* Ramesh: Set USB Photo/Music/Video/None */
    DRV_CUSTOM_CUST_SPEC_TYPE_MMP_OSD_STATE,                /* Ramesh: Set MMP OSD State */
    DRV_CUSTOM_CUST_SPEC_TYPE_OSD_TYPE,                     /* Ramesh: Set Full Screen OSD YES/NO */
//#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */  
    DRV_CUSTOM_CUST_SPEC_TYPE_FM_RADIO_STATE,               /* Ramesh: Set FM Radio ON/OFF */
//#endif
    DRV_CUSTOM_CUST_SPEC_TYPE_AUTOMAN_SCAN_STATE,           /* Ramesh: Set auto tuning status */
#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */
    DRV_CUSTOM_CUST_SPEC_TYPE_TELETEXT_MODE,                /* Ramesh: Set Teletext ON/OFF */
#endif
    DRV_CUSTOM_CUST_SPEC_TYPE_SCENE_SELECT_MODE,            /* Ramesh: Set scene select mode */
    DRV_CUSTOM_CUST_SPEC_TYPE_MOTIONFLOW_PATTERN,           /* SY: Generate Motion Flow Status */
#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */ 
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_CLK_TIME,                 /* Liew: Set/Get time into Hotel Clock */
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_CLK_CFG,                  /* Liew: Configure Hotel Clock */
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_CLK_MODE,                 /* Liew: Query Hotel Clock module's mode */
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_CLK_VERSION,              /* Liew: Query Hotel Clock module's SW version */
#endif
    DRV_CUSTOM_CUST_SPEC_GET_SONY_LIB_VERSION,              /* SY: Read Sony Library Version */
    DRV_CUSTOM_CUST_SPEC_GET_TEMP_SENSOR_VALUE,             /* SY: Read Temp Sensor Temperature Value */
    DRV_CUSTOM_CUST_SPEC_SET_INTERNAL_SIG_PTN,				      /* Marina: Internal Signal Pattern ( 31 July 2009)*/
#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */ 
    DRV_CUSTOM_CUST_SPEC_GET_SIGNAL_LEVEL,					        /* Azlan : Get signal level value (03/08/2009) */
#endif
    /*Tsen - ADC*/
    DRV_CUSTOM_CUST_SPEC_GET_A_GAIN,                        /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_D_GAIN,                        /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_R_OFFSET,                      /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_G_OFFSET,                      /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_B_OFFSET,                      /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_R_GAIN,                        /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_G_GAIN,                        /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_B_GAIN,                        /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_CVBS_TARGET,                   /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_COMP_CHANNEL1_TARGET,          /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_COMP_CHANNEL2_TARGET,          /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_COMP_CHANNEL3_TARGET,          /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_EFUSE_GAIN,                    /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_YPBPR_VGA_MAX_LVL,                 /* Tsen : AD Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_YPBPR_VGA_BLANK_LVL,               /* Tsen : AD Adjustment for Sony factory */
    /*Tsen - ADC*/
#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */ 
    /* Ooi Start - IP TH */
    DRV_CUSTOM_CUST_SPEC_SET_IP_NOISE_TH,                   /* Ooi : IP Noise threshold to NVM  */
    DRV_CUSTOM_CUST_SPEC_GET_IP_NOISE_TH,                   /* Ooi : IP Noise threshold from NVM  */
    /* Ooi END */
#endif
    /* Ooi Start - SCREEN (11/8/09) */
    DRV_CUSTOM_CUST_SPEC_SET_SOURCE_MAIN,                   /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_SOURCE_MAIN,                   /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_SOURCE_SUB,                    /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_SOURCE_SUB,                    /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_OVERSCAN_MAIN,                 /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_OVERSCAN_MAIN,                 /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_OVERSCAN_SUB,                  /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_OVERSCAN_SUB,                  /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_DISPLAY_MAIN,                  /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_DISPLAY_MAIN,                  /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_DISPLAY_SUB,                   /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_DISPLAY_SUB,                   /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_NONLINEAR_PARAM,               /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_NONLINEAR_SLOPE,               /* Ooi : Screen test for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_NONLINEAR_ENDFACTOR,           /* Ooi : Screen test for Sony factory */
    /* Ooi END */

#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */ 
    DRV_CUSTOM_CUST_SPEC_GET_TUNER_REG,                     /* Chow : Get tuner register for factory application */
    DRV_CUSTOM_CUST_SPEC_SET_TUNER_AGC,                     /* Chow : Tuner registers to NVM */
    DRV_CUSTOM_CUST_SPEC_SET_CIS_AGC,                       /* Chow : Tuner registers to CIS case */
    DRV_CUSTOM_CUST_SPEC_SET_INT_PIC_SIG_BSTR,              /* Chow : Intelligent Booster and Signal Booster relation */
#endif    
    DRV_CUSTOM_CUST_SPEC_GET_DRV_SREG,                      /* Ramesh : Get value Factory app from Sony driver */
    DRV_CUSTOM_CUST_SPEC_SET_DRV_SREG,                      /* Ramesh : Set value Factory app to Sony driver */

    /*>>> START Cheah: Sony GammaWB callback func 07Sep09 */
    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_SETUP,                   /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_SAVE,                    /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_QUIT,                    /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_EXIT,                    /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_RESET,                   /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_SAMPLE,                  /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_SETUP,                      /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_SAVE,                       /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_QUIT,                       /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_EXIT,                       /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_RESET,                      /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_SETLVL,                     /* Cheah : Gamma Adjustment for Sony factory */
    DRV_CUSTOM_CUST_SPEC_SET_WB_SETCLR,                     /* Cheah : Gamma Adjustment for Sony factory */
    /*<<< END Cheah: Sony GammaWB callback func 07Sep09 */
    
#ifndef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* For IRIS only */
    DRV_CUSTOM_CUST_SPEC_GET_DYN_GAMMA_EN,                  /* Cheah : IRIS Dynamic Gamma control */
	DRV_CUSTOM_CUST_SPEC_SET_DYN_GAMMA_EN,					/* Cheah : IRIS Dynamic Gamma control */
#endif

    DRV_CUSTOM_CUST_SPEC_SET_GAMMA_PAT_GEN,                   /* Lim : Set test pattern for Sony factory */
    DRV_CUSTOM_CUST_SPEC_GET_COLOR_LEVEL,                     /* Marina : Get RGB level at a specific point */


    DRV_CUSTOM_CUST_SPEC_GET_PANEL_INCH_SIZE,                /* YiFen 021009 Read Panel Size for product information*/

    DRV_CUSTOM_CUST_SPEC_SET_AUDIO_INTERDAC_MUTE,            /* Gallen : For Ibiza internal dac mute control  */


/* Aizul Start: Hotel Volume */    
#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */ 
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_VOL_MODE,                 /* Aizul: Set/Query Hotel Volume module's mode */
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_VOL_INITVOL,              /* Aizul: Set/Query Hotel Volume initial volume */
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_VOL_STEPTIMER,            /* Aizul: Set/Query Hotel Volume step timer */
    DRV_CUSTOM_CUST_SPEC_TYPE_HTL_VOL_VOLLVL,               /* Aizul: Set/Query Hotel Volume volume level */       
#endif    
/* Aizul End: Hotel Volume */
    DRV_CUSTOM_CUST_SPEC_SET_RGB_GAISOU,        /* Ramesh: Set RGB sensor gaisou for SONY factory and service menu */
    DRV_CUSTOM_CUST_SPEC_GET_RGB_GAISOU,        /* Ramesh: Set RGB sensor gaisou for SONY factory and service menu */
    DRV_CUSTOM_CUST_SPEC_TYPE_DEMO_MODE_USB_VIDEO_MODE,     /* Ramesh: Set for USB demo mode video */

/* Aizul Start: Hotel SI */    
#ifdef THIRD_PARTY_LIB_EMCS_ATV_ONLY   /* Not Used in ATSC Project */ 
    DRV_CUSTOM_CUST_SPEC_TYPE_HTLSI_ONLINEMODE,                 /* Aizul: Set/Query Hotel SI module's online mode */
#endif
/* Aizul End: Hotel SI */    
    DRV_CUSTOM_CUST_SPEC_TYPE_DITHER, /* Hew: Dither on/off 29Oct2009*/

    DRV_CUSTOM_CUST_SPEC_RESET_IRIS_MUTE_PIN,   /*Lim 9nov09: Control IRIS mute pin*/

    DRV_CUSTOM_CUST_SPEC_TYPE_APP_COUNTRY,
    DRV_CUSTOM_CUST_SPEC_TYPE_OSD_STATUS_CHANGE,

    /* VGA wakeup on BGM mode*/
    DRV_CUSTOM_CUST_SPEC_TYPE_VGA_DETECT_WAKEUP,
    DRV_CUSTOM_CUST_SPEC_TYPE_VGA_STATUS,

    DRV_CUSTOM_CUST_SPEC_TYPE_PANEL_ID,

    /* Model ID from AP eeprom */
    DRV_CUSTOM_CUST_SPEC_TYPE_DVD_ENABLE,
    DRV_CUSTOM_CUST_SPEC_TYPE_PCB_REGION,

    /* Signal Diagnostics Status */
    DRV_CUSTOM_CUST_SPEC_TYPE_ATV_STATUS, /*[LINUX] FIXED! NOT IMPLEMENTED YET!*/

    /*Customer specific command END*/
    DRV_CUSTOM_CUST_SPEC_TYPE_END

} DRV_CUSTOM_CUST_SPEC_TYPE_T;

typedef enum
{
        DRV_CUSTOM_VGA_ENABLE_DETECTION,
        DRV_CUSTOM_VGA_DISABLE_DETECTION
} DRV_CUSTOM_VGA_DETECT_WAKEUP_T;

typedef enum
{
        DRV_CUSTOM_VGA_SIGNAL_STABLE,
        DRV_CUSTOM_VGA_SIGNAL_NO_SIGNAL,
        DRV_CUSTOM_VGA_SIGNAL_NOT_SUPPORT
}DRV_CUSTOM_VGA_STATUS_T;

typedef enum {
	D_DEMO_OFF = 0,
	D_BRAVIA_ENGINE = 1,
	D_MOTION_FLOW = 3,
	NUM_OF_D_DEMO_MODE
} D_DEMO_MODE;

typedef enum {
	D_FORMAT_422 = 0,
	D_FORMAT_444,
	NUM_OF_D_PATH
} D_PATH;

typedef enum {
	D_MOTION_OFF = 0,
	D_MOTION_STANDARD,
	D_MOTION_HIGH,
	NUM_OF_D_MOTIONFLOW_MODE
} D_MOTIONFLOW_MODE;

typedef enum {
	D_PICTURE_VIVID = 0,
	D_PICTURE_STANDARD,
	D_PICTURE_CUSTOM,
	D_PICTURE_CINEMA1,
	D_PICTURE_CINEMA2,
	D_PICTURE_GAME,
	D_PICTURE_TEXT,
	D_PICTURE_SPORTS,
	D_PICTURE_PHOTO_VIVID,
	D_PICTURE_PHOTO_STANDARD,
	D_PICTURE_PHOTO_CUSTOM,
	D_PICTURE_PHOTO_ORIGINAL,
	D_PICTURE_GAME2,
	NUM_OF_D_PICTURE_MODE
} D_PICTURE_MODE;

typedef enum {
	D_CINE_MOTION_OFF = 0,
	D_CINE_MOTION_AUTO2,
	D_CINE_MOTION_AUTO1,
	NUM_OF_D_CINE_MOTION
} D_CINE_MOTION_MODE;

typedef struct _SMicroMaskArea
{
    UINT8 u1Index;
    UINT16 u2Width;
    UINT16 u2Height;
    UINT16 u2X;
    UINT16 u2Y;
} D_SMicroMaskArea;


typedef enum {
	D_INPUT_RF = 0,
	D_INPUT_OTHER,
	NUM_OF_D_INPUT_MODE
} D_INPUT_MODE;

typedef struct _SMicroFactory
{
    UINT32 u4InDataSize;
    UINT32 u4OutDataSize;
    UINT8 *pInData;
    UINT8 *pOutData;
} D_SMicroFactory;

/* Start - Hew: Dither on/off 29Oct2009*/
typedef struct _PARAM_DITHER{
	UINT16 write_data;
	UINT8 lsb_off;
	UINT8 round_en;
	UINT8 rdither_en;
	UINT8 edither_en;
	UINT8 dither_lfsr_en;
}PARAM_DITHER;
/* End - Hew: Dither on/off 29Oct2009*/

typedef enum
{
    D_LED_GENERIC_ON,
    D_LED_GENERIC_OFF,
} D_LED_GENERIC_STATE;

typedef enum
{
    D_LED_POWER_ON,
    D_LED_POWER_OFF
} D_LED_POWER_STATE;

typedef enum
{
    D_LED_STANDBY_ON,
    D_LED_STANDBY_OFF
} D_LED_STANDBY_STATE;

typedef enum
{
    D_LED_TIMER_ORANGE,
    D_LED_TIMER_GREEN,
    D_LED_TIMER_BOTH,
    D_LED_TIMER_OFF,
} D_LED_TIMER_STATE;

typedef enum
{
    D_LED_POWER = 0,
    D_LED_STANDBY,
    D_LED_TIMER,
    D_LED_TIMER_2,
    D_LED_TOTAL
} D_LED_TYPE;

typedef struct _ATVLedDisplay
{
    D_LED_TYPE eLedType;
    UINT32 eLedState;
} D_LED_DISPLAY;

typedef enum
{
    D_SYSTEM_NORMAL,
    D_SYSTEM_PANEL_ONOFF
} D_SYSTEM_STATE;

typedef enum
{
    D_12V_AMP_ERROR,
    D_12V_SW_ERROR,
    D_24V_PWR_ERROR,
    D_AUD_ERROR,
    D_3V3_5V_ERROR,
    D_PANEL_NVM_ERROR,//atm-liang add
    D_PANEL_POWER_ERROR,
    D_BACKLIGHT_ERROR,
    D_TEMPERATURE_ERROR,
    D_BALANCER_ERROR,
    D_TCON_ERROR,
    D_ALL_ERROR
} D_SELF_DIAG_ERROR_TYPE;

typedef enum {
    D_COUNTRY_UNKNOWN = 0,
    D_COUNTRY_GA,
    D_COUNTRY_INDIA,
    D_COUNTRY_PHILIPPINE,
    D_COUNTRY_CIS,
    D_COUNTRY_CHINA,
    D_COUNTRY_LATIN
} APP_COUNTRY_SEL;

typedef enum
{
    D_STATE_AC_OFF = 0,
    D_STATE_STANDBY,
    D_STATE_POWER_OFF,
    D_STATE_POWER_ON,
    D_STATE_TOTAL
} D_POWER_STATE_TYPE;

typedef struct _SelfDiagState
{
    D_SELF_DIAG_ERROR_TYPE eErrorType;
    BOOL   bErrorOccur;
    BOOL   bResetErrorCnt;
    UINT32 u4ErrorCnt;
} D_SELF_DIAG_STATE;

typedef struct DRV_CUSTOM_TAG_VERSION_TYPE_T
{
    UINT8 ui1_tag_type;                      /* tag*/
    CHAR szVerStr[20];						//version string
}   DRV_CUSTOM_TAG_VERSION_TYPE_T;

/* Liew: Hotel Clock related */
typedef struct _DRV_CUSTOM_HTL_CLK_TIME_T
{
    UINT8 ui1_hour;
    UINT8 ui1_minute;
    UINT8 ui1_second;
    UINT8 ui1_day_of_week;
    UINT8 ui1_is_valid;     /* Only used when we get time from hotel clock */
} DRV_CUSTOM_HTL_CLK_TIME_T;
           
typedef struct _DRV_CUSTOM_HTL_CLK_CFG_T
{ 
    UINT8 ui1_display;
    UINT8 ui1_intensity;
    UINT8 ui1_reset;
}  DRV_CUSTOM_HTL_CLK_CFG_T;
/* Liew: End Hotel Clock related */

typedef enum
{
	DRV_CUSTOM_TAG_TYPE_LOADER= 0,  //// loader by luis
	DRV_CUSTOM_TAG_TYPE_FIRMWARE,  // firmware by luis
	DRV_CUSTOM_TAG_TYPE_PQ ,	// PQ data by norman
	DRV_CUSTOM_TAG_TYPE_PANEL,	// panel data by cosh
	DRV_CUSTOM_TAG_TYPE_AQ ,	// AQ data by Andrew
	DRV_CUSTOM_TAG_TYPE_UPEEP , // uP nvm by Cosh
	DRV_CUSTOM_TAG_TYPE_UPIMG ,	// uP image by Cosh	
	DRV_CUSTOM_TAG_TYPE_NVRAM , // eeprom by Jifeng
	DRV_CUSTOM_TAG_TYPE_EDID,  	// EDID by CI
	DRV_CUSTOM_TAG_TYPE_UPBT,  	// uP Bootloader
	DRV_CUSTOM_TAG_TYPE_MAX
}DRV_CUSTOM_TAG_TYPE_T;

typedef enum
{
    D_MMP_NONE = 0,
    D_MMP_PHOTO,
    D_MMP_MUSIC,
    D_MMP_MOVIE,
    NUM_OF_MMP_MODE
} D_MMP_MODE;

typedef enum
{
    D_MMP_OSD_NONE = 0,
    D_MMP_OSD_THUMBNAIL,
    D_MMP_OSD_DEVICE,
    D_MMP_OSD_PLAYBACK,
    NUM_OF_MMP_OSD_STATE
} D_MMP_OSD_STATE;

typedef enum
{
    D_MMP_OSD_NOT_FULL = 0,
    D_MMP_OSD_FULL,
    NUM_OF_OSD_TYPE
} D_MMP_OSD_TYPE;

typedef enum
{
    D_FM_RADIO_OFF = 0,
    D_FM_RADIO_ON,
    NUM_OF_FM_RADIO_STATE
} D_FM_RADIO_STATE;

typedef enum
{
    D_NOT_SCANNING = 0,
    D_IS_SCANNING,
    NUM_OF_SCAN_STATE
} D_AUTOMAN_SCAN_STATE;

typedef enum
{
    D_TELETEXT_OFF = 0,
    D_TELETEXT_ON,
    NUM_OF_TELETEXT_MODE
} D_TELETEXT_MODE;

typedef enum
{
    D_SCENE_AUTO = 0,
    D_SCENE_GENERAL,
    D_SCENE_PHOTO,
    D_SCENE_MUSIC,
    D_SCENE_CINEMA,
    D_SCENE_GAME,
    D_SCENE_GRAPHICS,
    D_SCENE_SPORTS,
    NUM_OF_SCENE_SELECT_MODE
} D_SCENE_SELECT_MODE;

typedef enum
{
    D_AGING_OFF,
    D_AGING_ON,
    NUM_OF_AGING_MODE  
} D_AGING_MODE;

typedef enum
{
    D_RGB_SREG,
    D_DBL_SREG,
    D_SIG_BSTR_SREG,
    D_TUNER_SREG,
    NUM_OF_DRV_SREG  
} D_DRV_SREG;

typedef enum
{
    D_DEMO_MODE_USB_OFF = 0,
    D_DEMO_MODE_USB_ON,
    NUM_OF_DEMO_MODE_USB
} D_DEMO_MODE_USB_VIDEO_MODE;

/* Marina - Start : Internal Signal Pattern ( 31 July 2009)*/  
typedef struct DRV_CUSTOM_INT_SIG_PTN
{
    UINT8 path;                      /*path*/
    UINT8 pattern;					 /*pattern*/
    UINT8 tag;                       /*tag*/
    UINT16 gamma_r;                  /*gamma_r_value*/
    UINT16 gamma_g;                  /*gamma_g_value*/
    UINT16 gamma_b;                  /*gamma_b_value*/
} DRV_CUSTOM_INT_SIG_PTN;
/* Marina - End : Internal Signal Pattern ( 31 July 2009)*/  

/* Marina - Start : HDMI EDID Read/Write( 10 August 2009)*/  
typedef enum
{
    D_HDMI_PORT1,
    D_HDMI_PORT2,
    D_HDMI_PORT3,
    D_HDMI_PORT4,
} D_HDMI_PORT_NUM;

typedef struct DRV_CUSTOM_EDID_DATA
{
    D_HDMI_PORT_NUM hdmi_port;                      /*HDMI port*/
    UINT8 edid_buffer[256];               /*HDMI EDID Buffer */
} DRV_CUSTOM_EDID_DATA;
/* Marina - End : HDMI EDID Read/Write( 10 August 2009)*/  

/* Ooi START: callback func for IP (1/9/09) */
typedef struct DRV_CUSTOM_IP_NOISE_TH_T
{
	UINT8 lvl;
	UINT8 data;
}DRV_CUSTOM_IP_NOISE_TH;
/* Ooi END */
/*TSEN start*/
typedef struct DRV_PARAM_ADC
{
    UINT8   adc_input;
    UINT8   adc_ch_num;
    UINT16  adc_ch_value;
}DRV_PARAM_ADC;

typedef struct DRV_PARAM_ADC_LVL
{
    UINT8   adc_ch_num;
    UINT16  adc_ch_value;
}DRV_PARAM_ADC_LVL;
/*TSEN end*/

typedef struct DRV_SREG
{
    D_DRV_SREG  type;
    UINT16      sreg;
    UINT16*     value;
}DRV_SREG;
typedef enum
{
    TU_FreqData,
    TU_StepData,
    TU_BandData,
} DRV_SONY_TUNER_DATA;

/* Marina -Start: Color Level(25 September 09)*/
typedef struct DRV_COLOR_LEVEL
{
    UINT16 h_pixel;
    UINT16 v_pixel;
    UINT16 rgb_level;
    UINT8 rgb_tag;
    UINT16 h_pixel_area;
    UINT16 v_pixel_area;
}DRV_COLOR_LEVEL;
/* Marina End */
/* Chow -Start: Sig bstr IP(09 Oct 09)*/
typedef struct DRV_SIG_BSTR_IP_ST
{
    BOOL IP_status;
    BOOL SigBstr_status;
}DRV_SIG_BSTR_IP_ST;
/* Chow End */
/*-----------------------------------------------------------------------------
                    function declarations
-----------------------------------------------------------------------------*/
/* Customer specific APIs */
extern INT32 d_Waffle_cust_spec_set (
    DRV_CUSTOM_CUST_SPEC_TYPE_T    e_cust_spec_type,
    VOID*                          pv_set_info,
    SIZE_T                         z_size,
    BOOL                           b_store
);

extern INT32 d_Waffle_cust_spec_get (
    DRV_CUSTOM_CUST_SPEC_TYPE_T    e_cust_spec_type,
    VOID*                          pv_get_info,
    SIZE_T*                        pz_size
);

extern void CustomLEDCtrlFunc(
    D_LED_TYPE eLedType,
    UINT32 u4LedState
);

extern void CustomDisableStandbyLEDCtrl(
    UINT8 u1Disable
);

extern UINT32 SelfDiagGetBootCounter(
    UINT32 *pu4BootCounter
);

extern UINT32 SelfDiagGetCounter(
    D_SELF_DIAG_STATE   *prErrorInfo
);

extern void CustomSetLoaderUsbUpgrade(
    BOOL bEnable
);

extern void ADAC_GpioInterDecMute(
    BOOL fgEnable
);

extern BOOL CustomIsLoaderUsbUpgradeEnable(void);
extern UINT32 SelfDiagResetCounter(void);
extern UINT32 SelfDiagResetBootCount(void);

#endif /* _D_WAFFLE_CUST_H_ */
