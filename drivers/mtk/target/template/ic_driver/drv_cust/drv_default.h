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

#ifndef DEFAULT_H
#define DEFAULT_H


//----------------------------------------------------------------------------
// Default power down support
//----------------------------------------------------------------------------
#ifndef PDWNC_EXT_IMPL
#define PDWNC_EXT_IMPL                  "mtk/mtk_pdwnc_ext.c"
#endif

//----------------------------------------------------------------------------
// Initial settings before dram config
//----------------------------------------------------------------------------
#ifndef DEFAULT_BOOT_VERBOSE
#ifdef NDEBUG
#define DEFAULT_BOOT_VERBOSE        0
#else
#define DEFAULT_BOOT_VERBOSE        1
#endif
#endif /* DEFAULT_BOOT_VERBOSE */

#ifndef DEFAULT_NOR_CPU_CLK
#define DEFAULT_NOR_CPU_CLK         (200000000)
#endif

#ifndef DEFAULT_DRAM_TYPE
#define DEFAULT_DRAM_TYPE           DDR_I_x2
#endif

#ifndef DEFAULT_DRAM_COLADDR
#define DEFAULT_DRAM_COLADDR        (COL_ADDR_BIT_10)
#endif

#ifndef DEFAULT_DRAM_PARAM_E
#define DEFAULT_DRAM_PARAM_E        (0)
#endif

#ifndef DEFAULT_DDR_CLOCK
#define DEFAULT_DDR_CLOCK           (405000000)
#endif

#ifndef DEFAULT_DDR_CL
#define DEFAULT_DDR_CL              (6)
#endif

#ifndef FLAG_DMPLL_SPECTRUM
#define FLAG_DMPLL_SPECTRUM         (0)
#endif

#ifndef DMPLL_SPECTRUM_PERMILLAGE
#define DMPLL_SPECTRUM_PERMILLAGE   (0)
#endif

#ifndef DRAM_PRIORITY_LIST
#define DRAM_PRIORITY_LIST          (NULL)
#endif

#ifndef DRAM_BURSTLEN
#define DRAM_BURSTLEN               15
#endif

#ifndef DRAM_GROUP1ARBITERTIME
#define DRAM_GROUP1ARBITERTIME		5
#endif
#ifndef DRAM_GROUP2ARBITERTIME
#define DRAM_GROUP2ARBITERTIME		4
#endif
#ifndef DRAM_GROUP3ARBITERTIME
#define DRAM_GROUP3ARBITERTIME		15
#endif

#ifndef FLAG_SAMSUNG_GDDR
#define FLAG_SAMSUNG_GDDR           0
#endif

#ifndef FLAG_1GBIT_SUPPORT
#define FLAG_1GBIT_SUPPORT          0
#endif

#ifndef FLAG_FORCE_32BIT
#define FLAG_FORCE_32BIT            0
#endif

#ifndef FLAG_FULL_FREQ
#define FLAG_FULL_FREQ              0
#endif

#ifndef FLAG_DDR_QFP
#define FLAG_DDR_QFP                0
#endif

#ifndef FLAG_SINGLE_DQS
#define FLAG_SINGLE_DQS             0
#endif

#ifndef FLAG_RUN_ON_NOR
#define FLAG_RUN_ON_NOR             0
#endif

#ifndef SYS_RESET_TIMER
#define SYS_RESET_TIMER             (0xf00000)
#endif

#ifndef FBM_MEM_CFG_SIZE
#ifdef __MODEL_slt__
#define FBM_MEM_CFG_SIZE            (0x3000000+0x100000)     // add 1MB  for audio slt
#else
#define FBM_MEM_CFG_SIZE            (0x3000000)     // 49 MB
#endif
#endif

#ifndef PDWNC_POLARITY
#define PDWNC_POLARITY              (0)
#endif

#ifndef WAKEUP_REASON_GPIO
#define WAKEUP_REASON_GPIO          (0)
#endif

#ifndef WAKEUP_REASON_GPIO_POLARITY
#define WAKEUP_REASON_GPIO_POLARITY (0)
#endif

//----------------------------------------------------------------------------
// Default usb upgrade setting
//----------------------------------------------------------------------------
#ifndef AUTO_USB_UPGRADE_ENABLE
#define AUTO_USB_UPGRADE_ENABLE     (0)
#endif
#ifndef USB_UPGRADE_FILE_NAME
#define USB_UPGRADE_FILE_NAME       ("upgrade.pkg")
#endif
#ifndef LOADER_UPGRADE_VER_CMP_FUNC
#define LOADER_UPGRADE_VER_CMP_FUNC (NULL)
#endif

//----------------------------------------------------------------------------
// Loader logo
//----------------------------------------------------------------------------
#ifndef LOADER_LOGO_FLASHADDR
#define LOADER_LOGO_FLASHADDR       (0)
#endif
#ifndef LOADER_LOGO_NUMBER
#define LOADER_LOGO_NUMBER          (0)
#endif
#ifndef LOADER_LOGO_BACKGROUND
#define LOADER_LOGO_BACKGROUND      (0xeeeeee)
#endif


//----------------------------------------------------------------------------
// Default Tuner setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_TUNER_CTRL
#define DEFAULT_TUNER_CTRL			(0)
#endif
#ifndef DEFAULT_TUNER_TYPE_DEFAULT
#define DEFAULT_TUNER_TYPE_DEFAULT	(0)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST0
#define DEFAULT_TUNER_TYPE_OFST0	(0x109)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST1
#define DEFAULT_TUNER_TYPE_OFST1	(0x4DE)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST2
#define DEFAULT_TUNER_TYPE_OFST2	(0x7F6)
#endif
#ifndef DEFAULT_TUNER_CTRL_ANA
#define DEFAULT_TUNER_CTRL_ANA		(0)
#endif

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT        (PANEL_DEFAULT)
#endif
// backlight power on/off gpio pin
#ifndef BACKLIGHT_GPIO
#define BACKLIGHT_GPIO              (GPIO_OPWM2)
#endif
// gpio output value when turn on backlight
#ifndef BACKLIGHT_ON_VAL
#define BACKLIGHT_ON_VAL            (0)
#endif
// lvds power on/off gpio pin
#ifndef PANELPOWER_GPIO
#define PANELPOWER_GPIO             (GPIO(3))
#endif
// gpio output value when turn on lvds
#ifndef PANELPOWER_ON_VAL
#define PANELPOWER_ON_VAL           (0)
#endif

// spread spectrum range, default is 1% for customer model
#ifndef SPREAD_SPECTRUM_PERMILLAGE
#define SPREAD_SPECTRUM_PERMILLAGE  (10)
#endif

// customer could define second backlight gpio pin
// #define BACKLIGHT_GPIO2            (GPIO_OPWM2)

// customer could define LVDS_MSB_SW to overwrite panel setting
// {MSB 5,4,3,2,1,0,7,6 LSB}
// #define LVDS_MSB_SW                (LVDS_MSB_SW_ON)
// {MSB 7,6,5,4,3,2,1,0 LSB}
// #define LVDS_MSB_SW                (LVDS_MSB_SW_OFF)

// customer could define different pwm polarity
// #define PWM_POLARITY               (PWM_LOW_PANEL_BRIGHT)

// dimming frequency in 50/60hz frame rate, 0 means use system default
#define DIMMING_FREQ_50         (0)
#define DIMMING_FREQ_60         (0)

// customer could have different backlight range request
// #define BACKLIGHT_HIGH             (0x80)
// #define BACKLIGHT_MIDDLE           (0x80)
// #define BACKLIGHT_LOW              (0x80)

// read panel from eeprom
/*
#define PANEL_INDEX_FROM_EEPROM     (1)
#define PANEL_INDEX_OFFSET1         (0x442)
#define PANEL_INDEX_OFFSET2         (0x52f)
#define PANEL_INDEX_OFFSET3         (0x6cf)
*/

#ifndef PANEL_CHANNEL_SWAP
#define PANEL_CHANNEL_SWAP          (0)
#endif

#ifndef PANEL_INDEX_FROM_GPIO_NUM
#define PANEL_INDEX_FROM_GPIO_NUM   (0)
#define PANEL_INDEX_FROM_GPIO_IDX   (NULL)
#define PANEL_INDEX_FROM_GPIO_MAP   (NULL)
#endif

// code segment for turning on the option
/*
static const UINT32 _au4PanelGpioIdx[3] = {13, 14, 15};
static const UINT32 _au4PanelGpioMap[8] = {24, 25, 26, 27, 28, 15, 30, 31};

#define PANEL_INDEX_FROM_GPIO_NUM   (sizeof(_au4PanelGpioIdx)/sizeof(UINT32))
#define PANEL_INDEX_FROM_GPIO_IDX   (&_au4PanelGpioIdx)
#define PANEL_INDEX_FROM_GPIO_MAP   (&_au4PanelGpioMap)
*/

#ifndef PANEL_LVDS_8BIT_PIN_MAP
#define PANEL_LVDS_8BIT_PIN_MAP    (NULL)
#endif

#ifndef PANEL_LVDS_10BIT_PIN_MAP
#define PANEL_LVDS_10BIT_PIN_MAP    (NULL)
#endif

// sample for lvds pin map
/*
5382p verification board:
static const UINT32 _au4PanelLvds8bitPinMap[12] =
{LVDS_A0, LVDS_A1, LVDS_A2, LVDS_A3, LVDS_DEFAULT,
 LVDS_DEFAULT, LVDS_A5, LVDS_A6, LVDS_ACLK2, LVDS_A8,
 LVDS_ACLK1, LVDS_A7};

5382p product board:
static const UINT32 _au4PanelLvds8bitPinMap[12] =
{LVDS_A0, LVDS_A1, LVDS_A2, LVDS_A3, LVDS_DEFAULT,
 LVDS_A5, LVDS_A6, LVDS_A7, LVDS_A8, LVDS_DEFAULT,
 LVDS_ACLK1, LVDS_ACLK2};

#define PANEL_LVDS_8BIT_PIN_MAP   (&_au4PanelLvds8bitPinMap)
#define PANEL_LVDS_10BIT_PIN_MAP   (&_au4PanelLvds10bitPinMap)

*/

// use ttl output instead of LVDS
#ifndef PANEL_TTL_OUTPUT
#define PANEL_TTL_OUTPUT            (0)
#endif

// number of table configuration in flash
#ifndef PANEL_FLASH_TABLE_SIZE
#define PANEL_FLASH_TABLE_SIZE         (0)
#endif
#ifndef PANEL_FLASH_TABLE_ADDRESS
#define PANEL_FLASH_TABLE_ADDRESS      (0x28760100)
#endif
#ifndef PANEL_EEPROM_TO_FLASH_INDEX_MAP
#define PANEL_EEPROM_TO_FLASH_INDEX_MAP (NULL)
#endif

/*
// adjust LVDS driving current, 1 means use default value (3mA)
#ifndef LVDS_DRIVING_CURRENT
#define LVDS_DRIVING_CURRENT        (1)
#endif
*/

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------

#ifndef AUDIO_INCLUDE_FILE
#define AUDIO_INCLUDE_FILE  "mtk/mtk_aud.h"
#endif

#ifndef AUDIO_OUT_PAD_NORMAL
#define AUDIO_OUT_PAD_NORMAL            (0)
#endif
// Output pad driving current in unit of mA, could be set as 2/4/6/8
#ifndef AUDIO_OUT_PAD_DRV_CUR
#define AUDIO_OUT_PAD_DRV_CUR           (8)
#endif
// Adac MCLK clock, MCLK_256FS/MCLK384FS/..
#ifndef AUDIO_DAC_MCLK
#define AUDIO_DAC_MCLK MCLK_256FS
#endif

// Bass management cutoff freqency
#ifndef AUDIO_BASS_MNG_CUTOFF_FREQ
#define AUDIO_BASS_MNG_CUTOFF_FREQ      (100)
#endif
// DSP softmute_num = 128 * 2^(softmute_order)
#ifndef AUDIO_SOFT_MUTE_ORDER
#define AUDIO_SOFT_MUTE_ORDER           (3)
#endif
//BBE level
#ifndef AUDIO_BBE_LEVEL
#define AUDIO_BBE_LEVEL                 (0)
#endif
//BBE Process
#ifndef AUDIO_BBE_PROCESS
#define AUDIO_BBE_PROCESS               (7)
#endif
//BBE Lo Contour
#ifndef AUDIO_BBE_LO_CONTOUR
#define AUDIO_BBE_LO_CONTOUR            (7)
#endif
//BBE 3D gain
#ifndef AUDIO_BBE_3D_GAIN
#define AUDIO_BBE_3D_GAIN               (80)
#endif
// L/R speaker type, large/small
#ifndef AUDIO_SPK_TYPE_LARGE
#define AUDIO_SPK_TYPE_LARGE            (TRUE)
#endif
// LS/RS speaker type, large/small
#ifndef AUDIO_SURROUND_SPK_TYPE_LARGE
#define AUDIO_SURROUND_SPK_TYPE_LARGE   (TRUE)
#endif
// L/R speaker type, large/small
#ifndef AUDIO_CENTER_SPK_TYPE_LARGE
#define AUDIO_CENTER_SPK_TYPE_LARGE     (TRUE)
#endif
#ifndef AUDIO_SUBWOOFER_ON
#define AUDIO_SUBWOOFER_ON              (TRUE)
#endif

// Bypass DSP post processing, only used in specified model (ex. SLT, testing)
#ifndef AUDIO_BYPASS_POST_PROC
#define AUDIO_BYPASS_POST_PROC          (FALSE)
#endif

#ifndef AUDIO_ADAC_USAGE
#define AUDIO_ADAC_USAGE                (&_arMtkAdacUsage)
#endif

#ifndef AUDIO_AOUT_PAD_CFG
#define AUDIO_AOUT_PAD_CFG              (&_arMtkAoutPadTable)
#endif

#ifndef AUDIO_INTER_DAC_USAGE
#define AUDIO_INTER_DAC_USAGE           (&_arMtkInterDacUsage)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_Opctrl_Mute_Gpio_Num
#define AUDIO_Opctrl_Mute_Gpio_Num (0xffffffff)
#endif

#ifndef AUDIO_Mute_Gpio_Num
#define AUDIO_Mute_Gpio_Num             (0xffffffff)
#endif

#ifndef AUDIO_Amp_Gpio_Num
#define AUDIO_Amp_Gpio_Num              (0xffffffff)
#endif

#ifndef AUDIO_OPCTRL_MUTE_GPIO_POLARITY
#define AUDIO_OPCTRL_MUTE_GPIO_POLARITY AUD_OPCTRL_MUTE_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_MUTE_GPIO_POLARITY
#define AUDIO_MUTE_GPIO_POLARITY        AUD_MUTE_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_AMP_GPIO_POLARITY
#define AUDIO_AMP_GPIO_POLARITY         AUD_AMP_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_INPUT_SW_GPIO_SEL
#define AUDIO_INPUT_SW_GPIO_SEL         (&_arMtkInputSwGpio)
#endif

#ifndef AUDIO_INPUT_MUX_SEL_TABLE
#define AUDIO_INPUT_MUX_SEL_TABLE       (&_rMtkAudInputMuxTable)
#endif

#ifndef AUDIO_OUT_FMT_TABLE
#define AUDIO_OUT_FMT_TABLE             (&_arMtkAudOutFmtTable)
#endif

#ifndef AUDIO_IN_FMT_TABLE
#define AUDIO_IN_FMT_TABLE              (&_arMtkAudInFmtTable)
#endif

#ifndef AUDIO_INPUT_SRC_VOL
#define AUDIO_INPUT_SRC_VOL             (&_aai2MtkAudInputSrcVol)
#endif

#ifndef AUDIO_AV_INPUT_SRC_VOL
#define AUDIO_AV_INPUT_SRC_VOL          (&_ai2MtkAudAvInputSrcVol)
#endif

#ifndef AUDIO_CHL_EQ_CFG
#define AUDIO_CHL_EQ_CFG                (&_aai1MtkAudChlEqCfg)
#endif

#ifndef AUDIO_INPUT_SRC_AGC_FLAG
#define AUDIO_INPUT_SRC_AGC_FLAG        (&_aafgMtkAudInputSrcAgcFlag)
#endif

#ifndef AUDIO_INPUT_SRC_AGC_VOL
#define AUDIO_INPUT_SRC_AGC_VOL         (&_aau4MtkAudInputSrcAgcVol)
#endif

#ifndef AUDIO_VOL_BASE_LEVEL
#define AUDIO_VOL_BASE_LEVEL            (_au1MtkAudVolBaseLevel)
#endif

#ifndef AUDIO_VOL_TABLE
#define AUDIO_VOL_TABLE                 (&_au4VolumeTable)
#endif

#ifndef AUDIO_AVC_TARGET_LEV
#define AUDIO_AVC_TARGET_LEV            (_ai2MtkAudAvcTarget)
#endif

#ifndef AUDIO_AVC_GAIN_UP
#define AUDIO_AVC_GAIN_UP               (_ai2MtkAudAvcGainUp)
#endif

#ifndef AUDIO_AVC_ATTACK_THRES
#define AUDIO_AVC_ATTACK_THRES          (_ai2MtkAudAvcAttackThres)
#endif

#ifndef AUDIO_AVC_ADJUST_RATE
#define AUDIO_AVC_ADJUST_RATE           (_ai2MtkAudAvcAdjustRate)
#endif

#ifndef AUDIO_CHL_VOL_EXTRA_GAIN
#define AUDIO_CHL_VOL_EXTRA_GAIN        (NULL)
#endif

#ifndef AUDIO_CHANNEL_DELAY
#define AUDIO_CHANNEL_DELAY             (&_au2MtkAudChDelay)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_HP_PLUG_IN_GPIO
#define AUDIO_HP_PLUG_IN_GPIO           (0xffffffff)
#endif

#ifndef AUDIO_HP_PLUG_IN_POLARITY
#define AUDIO_HP_PLUG_IN_POLARITY       (AUD_GPIO_HIGH_ENALBE)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_DC_DETECT_GPIO
#define AUDIO_DC_DETECT_GPIO           (0xffffffff)
#endif

#ifndef AUDIO_DC_DETECT_POLARITY
#define AUDIO_DC_DETECT_POLARITY       AUD_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_HP_PLUG_IN_LINEOUT_MUTE
#define AUDIO_HP_PLUG_IN_LINEOUT_MUTE   (FALSE)
#endif

#ifndef AUDIO_MUTE_ALL_SOURCE
#define AUDIO_MUTE_ALL_SOURCE           (FALSE)
#endif

#ifndef AUDIO_LINEOUT_DAC_USAGE
#define AUDIO_LINEOUT_DAC_USAGE         (FALSE)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_DVD_GPIO
#define AUDIO_DVD_GPIO (0xffffffff)
#endif

#ifndef AUDIO_DVD_POLARITY
#define AUDIO_DVD_POLARITY AUD_DVD_GPIO_HIGH_ENALBE
#endif

#ifndef AUDIO_DVD_CHANNEL
#define AUDIO_DVD_CHANNEL (0xffffffff)
#endif

#ifndef AUDIO_SPK_AUTO_OFF_WHEN_VOL_MUTE
#define AUDIO_SPK_AUTO_OFF_WHEN_VOL_MUTE (NULL)
#endif

// If GPIO NUM = 0xffffffff, then don't use that
#ifndef AUDIO_CUST_SPEC_GPIO_CTRL
#define AUDIO_CUST_SPEC_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_SPEC_POLARITY
#define AUDIO_CUST_SPEC_POLARITY  AUD_GPIO_HIGH_ENABLE
#endif

#ifndef AUDIO_CUST_SPEC2_GPIO_CTRL
#define AUDIO_CUST_SPEC2_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_SPEC2_POLARITY
#define AUDIO_CUST_SPEC2_POLARITY  AUD_GPIO_HIGH_ENABLE
#endif

#ifndef AUDIO_CUST_HP_GPIO_CTRL
#define AUDIO_CUST_HP_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_HP_POLARITY
#define AUDIO_CUST_HP_POLARITY  AUD_GPIO_HIGH_ENABLE
#endif

#ifndef AUDIO_Digital_Data_TABLE
#define AUDIO_Digital_Data_TABLE 	(&_afgMtkAudDigitalDataUsage)
#endif

#ifndef AUDIO_TUNER_ATD
#define AUDIO_TUNER_ATD     (FALSE)
#endif

// IF path =0, dac0 : -6 dB, path =1, dac1 : -6 dB, path =2, dac0 & dac1 : -6 dB
#ifndef AUDIO_SPK_CODEC_PATH
#define AUDIO_SPK_CODEC_PATH  1
#endif

#ifndef AUDIO_MT8292_PGA0_GAIN
#define AUDIO_MT8292_PGA0_GAIN (0x1d)
#endif

#ifndef AUDIO_MT8292_PGA1_GAIN
#define AUDIO_MT8292_PGA1_GAIN (0x1d)
#endif

#ifndef AUDIO_MT8292_PGA2_GAIN
#define AUDIO_MT8292_PGA2_GAIN (0x1d)
#endif

#ifndef AUDIO_MT8292_HP_GAIN
#define AUDIO_MT8292_HP_GAIN (0x1d)
#endif

#ifndef AUDIO_LINE_IN_LR_SWAP
#define AUDIO_LINE_IN_LR_SWAP (FALSE)
#endif

#ifndef AUDIO_MTS_FACTORY_MODE
#define AUDIO_MTS_FACTORY_MODE        (&_arMtsFactory)
#endif

#ifndef AUDIO_A2_FACTORY_MODE
#define AUDIO_A2_FACTORY_MODE        (&_arA2Factory)
#endif

#ifndef AUDIO_PAL_FACTORY_MODE
#define AUDIO_PAL_FACTORY_MODE        (&_arPalFactory)
#endif

// Limiter mode. 0: Off, 1: Adaptive mode, 2: Fix mode
#ifndef AUDIO_LIMITER_MODE
#define AUDIO_LIMITER_MODE  (1)
#endif

#ifndef AUDIO_NVM_ENABLE
#define AUDIO_NVM_ENABLE (FALSE)
#endif

#ifndef AUDIO_NVM_LENGTH
#define AUDIO_NVM_LENGTH (0)
#endif

#ifndef AUDIO_NVM_DATA
#define AUDIO_NVM_DATA (0)
#endif

#ifndef AUDIO_PROFILE_DEFAULT
#define AUDIO_PROFILE_DEFAULT (0)
#endif

#ifndef AUDIO_PROFILE_START
#define AUDIO_PROFILE_START (0)
#endif

#ifndef AUDIO_PROFILE_END
#define AUDIO_PROFILE_END (0)
#endif

#ifndef AUDIO_AL1AR1_PHASE_INVERSE
#define AUDIO_AL1AR1_PHASE_INVERSE           (FALSE)
#endif

#ifndef AUDIO_AL2AR2_PHASE_INVERSE
#define AUDIO_AL2AR2_PHASE_INVERSE           (FALSE)
#endif

#ifndef AUDIO_DIGITAL_AMP_RESET_GPIO
#define AUDIO_DIGITAL_AMP_RESET_GPIO (0xffffffff)
#endif

#ifndef AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY
#define AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY  AUD_GPIO_HIGH_ENALBE
#endif

//====================================


//----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#ifndef FLAG_EXT_STATUS
#define FLAG_EXT_STATUS             (0)
#endif
#ifndef STANDBY_SET_GPIO
#define STANDBY_SET_GPIO            (PWR_GPIO7)
#endif
#ifndef POWER_STATUS_GPIO
#define POWER_STATUS_GPIO           (PWR_GPIO6)
#endif
#ifndef STDBY_USE_PWR_BTN_CNT
#define STDBY_USE_PWR_BTN_CNT       (0)
#endif
#ifndef STDBY_USE_PREV_PWR_STATE
#define STDBY_USE_PREV_PWR_STATE    (0)
#endif
#ifndef STDBY_USE_VGA_STATUS
#define STDBY_USE_VGA_STATUS        (0)
#endif

// #ifndef POWER_LED_GPIO
// #define POWER_LED_GPIO              (PWR_GPIO0)
// #endif
// #ifndef POWER_LED_ON_VAL
// #define POWER_LED_ON_VAL            (0)
// #endif
// #define BACKUP_LED_GPIO
// #define BACKUP_LED_ON_VAL
// #define POWER_CTRL_GPIO
// #define POWER_CTRL_ON_VAL
#ifndef POWER_CTRL_DELAY_MS
#define POWER_CTRL_DELAY_MS         (5)
#endif
// #define DVD_CTRL_GPIO
// #define DVD_CTRL_ON_VAL
#ifndef FLAG_SUPPORT_DARK_LED
#define FLAG_SUPPORT_DARK_LED        (0)
#endif
// #define POWER_LED_DARK_VAL
// #define BACKUP_LED_DARK_VAL
// #define SPECIAL_WAKEUP_GPIO
// #define SPECIAL_WAKEUP_ON_VAL
// #define COMPANION_CHIP_GPIO_NUM
// #define COMPANION_CHIP_GPIO_POLARITY

#ifndef GPIO_INCLUDE_FILE
#define GPIO_INCLUDE_FILE       "mtk/mtk_gpio.h"
#endif

#ifndef FLAG_SUPPORT_POWER_LED_OFF
#define FLAG_SUPPORT_POWER_LED_OFF      (0)
#endif

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#ifndef SYSTEM_EEPROM_SIZE
#define SYSTEM_EEPROM_SIZE          (2048)
#endif
#ifndef SYSTEM_EEPROM_WP_GPIO
#define SYSTEM_EEPROM_WP_GPIO       (0)
#endif
#ifndef SYSTEM_EEPROM_WP_ENABLE_POLARITY
#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (0)
#endif
//#ifndef EDID_EEPROM_WP_GPIO
//#define EDID_EEPROM_WP_GPIO         (11)
//#endif
#ifndef EDID_EEPROM_WP_ENABLE_POLARITY
#define EDID_EEPROM_WP_ENABLE_POLARITY  (0)
#endif
#ifndef SYSTEM_EEPROM_CLKDIV
#define SYSTEM_EEPROM_CLKDIV          (0x100)
#endif

//Initially for SGP, now open for all customers
#ifndef DRVCUST_EEPROM_HDMI_HDCP_OFFSET
#define DRVCUST_EEPROM_HDMI_HDCP_OFFSET  ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_HDMI_HDCP_SIZE
#define DRVCUST_EEPROM_HDMI_HDCP_SIZE  ((UINT32)320)
#endif
#ifndef DRVCUST_EEPROM_ERROR_CODE_OFFSET
#define DRVCUST_EEPROM_ERROR_CODE_OFFSET	((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_ERROR_CODE_SIZE
#define DRVCUST_EEPROM_ERROR_CODE_SIZE	    ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_STATE_CODE_OFFSET
#define DRVCUST_EEPROM_STATE_CODE_OFFSET    ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_STATE_CODE_SIZE
#define DRVCUST_EEPROM_STATE_CODE_SIZE      ((UINT32)0)
#endif
#ifndef DRVCUST_EEPROM_DTV_DRIVER_OFFSET
#define DRVCUST_EEPROM_DTV_DRIVER_OFFSET	((UINT32)(DRVCUST_EEPROM_HDMI_HDCP_OFFSET + DRVCUST_EEPROM_HDMI_HDCP_SIZE))
#endif
#ifndef DRVCUST_EEPROM_DTV_DRIVER_SIZE
#define DRVCUST_EEPROM_DTV_DRIVER_SIZE	((UINT32)32)
#endif
#ifndef DRVCUST_EEPROM_NPTV_DRIVER_OFFSET
#define DRVCUST_EEPROM_NPTV_DRIVER_OFFSET	((UINT32)(DRVCUST_EEPROM_DTV_DRIVER_OFFSET + DRVCUST_EEPROM_DTV_DRIVER_SIZE))
#endif
#ifndef DRVCUST_EEPROM_NPTV_DRIVER_SIZE
#define DRVCUST_EEPROM_NPTV_DRIVER_SIZE	((UINT32)(768))
#endif
#ifndef DRVCUST_EEPROM_DRIVER_SIZE
#define DRVCUST_EEPROM_DRIVER_SIZE			((UINT32)(DRVCUST_EEPROM_NPTV_DRIVER_OFFSET + DRVCUST_EEPROM_NPTV_DRIVER_SIZE))
#endif

//----------------------------------------------------------------------------
// NOR customization
//----------------------------------------------------------------------------
#ifndef NOR_FLASH_TOTAL_SIZE
#define NOR_FLASH_TOTAL_SIZE                (4*1024*1024)
#endif
#ifndef NOR_FLASH_LOADER_SIZE
#ifdef LOADER_MAX_SIZE
#define NOR_FLASH_LOADER_SIZE             (LOADER_MAX_SIZE)
#else /* LOADER_MAX_SIZE */
#define NOR_FLASH_LOADER_SIZE             (128*1024)
#endif /* LOADER_MAX_SIZE */
#endif
#ifndef NOR_FLASH_APPIMAGE_SIZE
#define NOR_FLASH_APPIMAGE_SIZE          ((3*1024*1024)+(512*1024))
#endif

// if NOR_FLASH_DYNAMIC_PART_SIZE = 0, all partitions are all static partition type.
#ifndef NOR_FLASH_DYNAMIC_PART_SIZE
#define NOR_FLASH_DYNAMIC_PART_SIZE         (0)
#endif
#ifndef NOR_FLASH_STATIC_PART_SIZE
#define NOR_FLASH_STATIC_PART_SIZE \
    ((NOR_FLASH_TOTAL_SIZE) - (NOR_FLASH_LOADER_SIZE) - \
    (NOR_FLASH_APPIMAGE_SIZE) - (NOR_FLASH_DYNAMIC_PART_SIZE))
#endif

#ifndef HDCP_NOR_FLASH_PART_ID
#define HDCP_NOR_FLASH_PART_ID              (0)
#endif

// size > 64KB: static partition type.
// size = 64KB: dynamic partition type.
// size = 0:       unused partition type.
#ifndef NOR_PART_SIZE_0
#define NOR_PART_SIZE_0            (0)
#endif
#ifndef NOR_PART_SIZE_1
#define NOR_PART_SIZE_1            (0)
#endif
#ifndef NOR_PART_SIZE_2
#define NOR_PART_SIZE_2            (2*64*1024)
#endif
#ifndef NOR_PART_SIZE_3
#define NOR_PART_SIZE_3            (0)
#endif
#ifndef NOR_PART_SIZE_4
#define NOR_PART_SIZE_4            (0)
#endif
#ifndef NOR_PART_SIZE_5
#define NOR_PART_SIZE_5            (0)
#endif
#ifndef NOR_PART_SIZE_6
#define NOR_PART_SIZE_6            (0)
#endif
#ifndef NOR_PART_SIZE_7
#define NOR_PART_SIZE_7            (0)
#endif

// static partition must be the multiple of 64 KB.
#if ((NOR_PART_SIZE_0 % (64*1024)) ||\
       (NOR_PART_SIZE_1 % (64*1024)) ||\
       (NOR_PART_SIZE_2 % (64*1024)) ||\
       (NOR_PART_SIZE_3 % (64*1024)) ||\
       (NOR_PART_SIZE_4 % (64*1024)) ||\
       (NOR_PART_SIZE_5 % (64*1024)))
#error "Partition must be the multiple of 64 KB"
#endif

// check the summation of each partition must larger than total static size.
#if ((NOR_PART_SIZE_0 + \
        NOR_PART_SIZE_1 + \
        NOR_PART_SIZE_2+ \
        NOR_PART_SIZE_3+ \
        NOR_PART_SIZE_4+ \
        NOR_PART_SIZE_5)  \
        > (NOR_FLASH_STATIC_PART_SIZE + NOR_FLASH_DYNAMIC_PART_SIZE))
#error "Partition Size too small !!"
#endif

//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#ifndef FLAG_BUTTON_SUPPORT
#define FLAG_BUTTON_SUPPORT         (0)
#endif
#ifndef SINGLE_GPIO_KEY_LIST
#define SINGLE_GPIO_KEY_LIST        (NULL)
#endif
#ifndef SINGLE_SERVO_KEY_LIST
#define SINGLE_SERVO_KEY_LIST       (NULL)
#endif
#ifndef GREED_GPIO_KEY_LIST
#define GREED_GPIO_KEY_LIST         (NULL)
#endif
#ifndef MULTI_KEY_LIST
#define MULTI_KEY_LIST              (NULL)
#endif
#ifndef KEYPAD_WAKEUP_BIT
#define KEYPAD_WAKEUP_BIT           (DKBIT_POWER)
#endif
#ifndef KEYPAD_POLLING_TIME
#define KEYPAD_POLLING_TIME         (50)
#endif
#ifndef KEYPAD_CHECK_TIME
#define KEYPAD_CHECK_TIME           (2)
#endif

//----------------------------------------------------------------------------
// PIGain Customization
//----------------------------------------------------------------------------
#ifndef PIGAIN_MAP_ARRAY
#define PIGAIN_MAP_ARRAY	(NULL)       //PIGain table Modify by W.C Shih
#endif
#ifndef PIGAIN_MAP_SIZE
#define PIGAIN_MAP_SIZE 0
#endif

//----------------------------------------------------------------------------
// VGA Auto Polarity Check Customerization
//----------------------------------------------------------------------------
#ifndef FLAG_VGAPOLARITY_CHECK
#define FLAG_VGAPOLARITY_CHECK 0  // VGA Politary Check Modify by W.C Shih2006/10/30
#endif

#ifndef VGA_AD_SPEC
#define VGA_AD_SPEC	1500//2000 //200 MHz
#define VGA_HFH_SPEC   700 //80KHz
#define VGA_HFL_SPEC   280 //28KHz
#define VGA_VFH_SPEC   86 //86Hz
#define VGA_VFL_SPEC   49 //49Hz
#endif

#ifndef SUPPORT_VGA_AD_SPEC
#define SUPPORT_VGA_AD_SPEC	0
#endif

//----------------------------------------------------------------------------
//SCART RGB Order Customerization
//----------------------------------------------------------------------------
#ifndef SCART_RGB_ORDER
#define SCART_RGB_ORDER (7)
#endif

#ifndef SCART_SC_R_EXTERNAL
#define SCART_SC_R_EXTERNAL (0)
#endif
//----------------------------------------------------------------------------
// SCART FS Status Servo ADC Customization
//----------------------------------------------------------------------------
#ifndef SCART1_FS_SERVO_ADC
#define SCART1_FS_SERVO_ADC     0
#endif
#ifndef SCART2_FS_SERVO_ADC
#define SCART2_FS_SERVO_ADC     1
#endif

//----------------------------------------------------------------------------
// SCART Fast Blanking In Select
//----------------------------------------------------------------------------
#ifndef SCART_FB_IN_SELECT
#define SCART_FB_IN_SELECT SCART_FB_IN_SOY1
#endif

//----------------------------------------------------------------------------
// Scart Setting
//----------------------------------------------------------------------------
#ifndef SUPPORT_SCART_TYPE
#define SUPPORT_SCART_TYPE SCART_HALF_TYPE
#endif

//----------------------------------------------------------------------------
// IRRX customization
//----------------------------------------------------------------------------
#ifndef DEFAULT_IRRX_KEY_UP_TIMEOUT
#define DEFAULT_IRRX_KEY_UP_TIMEOUT         (200)   // ms
#endif
#ifndef DEFAULT_IRRX_REPEAT_VALID_TIME
#define DEFAULT_IRRX_REPEAT_VALID_TIME      (200)   // ms
#endif
#ifndef IRRX_NEC_PULSE1
#define IRRX_NEC_PULSE1             (8)
#endif
#ifndef IRRX_NEC_PULSE2
#define IRRX_NEC_PULSE2             (0)
#endif
#ifndef IRRX_NEC_PULSE3
#define IRRX_NEC_PULSE3             (0)
#endif
#ifndef IRRX_RC5_KEYMAP_FUNC
#define IRRX_RC5_KEYMAP_FUNC            (NULL)
#endif
#ifndef IRRX_RC6_KEYMAP_FUNC
#define IRRX_RC6_KEYMAP_FUNC            (NULL)
#endif
#ifndef IRRX_XFER_USR_TO_CRYSTAL_FUNC
#define IRRX_XFER_USR_TO_CRYSTAL_FUNC   (NULL)
#endif
#ifndef IRRX_SET_USR_WAKEUP_KEY_FUNC
#define IRRX_SET_USR_WAKEUP_KEY_FUNC    (NULL)
#endif
#ifndef IRRX_USR_CONFIG
#define IRRX_USR_CONFIG         0x4121
#endif
#ifndef IRRX_USR_SAPERIOD
#define IRRX_USR_SAPERIOD       0x0037
#endif
#ifndef IRRX_USR_THRESHOLD
#define IRRX_USR_THRESHOLD      0x0001
#endif
#ifndef IRRX_REPEAT_PULSE_UP
#define IRRX_REPEAT_PULSE_UP    5
#endif
#ifndef IRRX_REPEAT_PULSE_DOWN
#define IRRX_REPEAT_PULSE_DOWN  2
#endif
#ifndef IRRX_FLAG_IRRX_BLINKING
#define IRRX_FLAG_IRRX_BLINKING     (0)
#endif
#ifndef FLAG_IR_PRGUPDOWN_WAKEUP
#define FLAG_IR_PRGUPDOWN_WAKEUP    (0)
#endif
#ifndef FLAG_IR_ALL_WAKEUP
#define FLAG_IR_ALL_WAKEUP    (0)
#endif

#if 0
//----------------------------------------------------------------------------
// Default NEC IRRX keymap data structure.
//----------------------------------------------------------------------------

#ifndef VENDER_NEC_KEYMAP_ARRAY

#include "mtk/mtk_ir.h"

static const KEYARRAY_T _arKeyArray[] =
{
    { 0x9f20, (sizeof(_arMtkTvKeyMap)/sizeof(KEYUNIT_T)), _arMtkTvKeyMap },
    { 0xe31c, (sizeof(_arMtkTvBigKeyMap)/sizeof(KEYUNIT_T)), _arMtkTvBigKeyMap },
};

#define VENDER_NEC_KEYMAP_ARRAY        (_arKeyArray)
#define VENDER_NEC_MAPARRAY_SIZE       (sizeof(_arKeyArray)/sizeof(KEYARRAY_T))

#endif /* VENDER_NEC_KEYMAP_ARRAY */

#ifndef VENDER_NEC_MAPARRAY_SIZE
#define VENDER_NEC_MAPARRAY_SIZE       (0)
#endif /* VENDER_NEC_MAPARRAY_SIZE */


//----------------------------------------------------------------------------
// Default RC5 IRRX keymap data structure.
//----------------------------------------------------------------------------

#ifndef VENDER_RC5_KEYMAP_ARRAY

LINT_SUPPRESS_NEXT_EXPRESSION(960)
#include "mtk/mtk_ir.h"

#define VENDER_RC5_KEYMAP_ARRAY        (_arRC56KeyMap)
#define VENDER_RC5_KEYMAP_SIZE         (sizeof(_arRC56KeyMap)/sizeof(KEYUNIT_T))

#endif /* VENDER_RC5_KEYMAP_ARRAY */

#ifndef VENDER_RC5_MAPARRAY_SIZE
#define VENDER_RC5_MAPARRAY_SIZE    (0)
#endif /* VENDER_NEC_MAPARRAY_SIZE */

#endif

//----------------------------------------------------------------------------
// IC work around flag
//----------------------------------------------------------------------------
#ifndef FLAG_VGA_WAKE_WORKAROUND
#define FLAG_VGA_WAKE_WORKAROUND        (0)
#endif
#ifndef FLAG_CPUPLL_DMPLL_WORKAROUND
#define FLAG_CPUPLL_DMPLL_WORKAROUND    (0)
#endif


//----------------------------------------------------------------------------
// Other default value
//----------------------------------------------------------------------------

#ifndef FLAG_FACTORY_EDID
#define FLAG_FACTORY_EDID           (0)
#endif
#ifndef FLAG_FACOTRY_MODE2_SUPPORT
#define FLAG_FACOTRY_MODE2_SUPPORT  (0)
#endif

#ifndef RELEASE_ENABLE_WATCHDOG
#define RELEASE_ENABLE_WATCHDOG     (0)
#endif

#ifndef RELEASE_WATCHDOG_TIMEOUT
#define RELEASE_WATCHDOG_TIMEOUT    (5)
#endif
#ifndef RELEASE_WATCHDOG_PRIORITY
#define RELEASE_WATCHDOG_PRIORITY   (100)
#endif

#ifndef SW_POWER_DETECT_VOLTAGE
#define SW_POWER_DETECT_VOLTAGE    (0x0) //0:0V, 0xFF:2.8V
#endif

//--------------------------------------------------------------------------------------------
//#define SCPOS_SPECIAL_VGA_DISABLE		0
//#define SCPOS_SPECIAL_VGA_USE_DRAM		1	:both Main/Sub go through normal path
//#define SCPOS_SPECIAL_VGA_USE_DISPMODE	2	:main=>display mode, sub=>normal mode
#ifndef SCPOS_SUPPORT_SPECIAL_VGA
#define SCPOS_SUPPORT_SPECIAL_VGA   (0)
#endif

//#ifndef SCPOS_FIXED_NON_LINEAR_FACTOR
//#define SCPOS_FIXED_NON_LINEAR_FACTOR   (0)
//#endif
#ifndef SCPOS_NON_LINEAR_SLOPE
#define SCPOS_NON_LINEAR_SLOPE   (10)
#endif
#ifndef SCPOS_NON_LINEAR_END_POINT_RATIO
#define SCPOS_NON_LINEAR_END_POINT_RATIO   (900)
#endif

#ifndef VGA_MAX_SUPPORT_PIXEL_RATE
#define VGA_MAX_SUPPORT_PIXEL_RATE  (0xFFFFFFFF)
#endif

#ifndef VGA_EXT_MAX_HSYNC_FREQ
#define VGA_EXT_MAX_HSYNC_FREQ      (0)
#endif

#ifndef VGA_EXT_MIN_HSYNC_FREQ
#define VGA_EXT_MIN_HSYNC_FREQ      (0)
#endif

#ifndef VGA_EXT_MAX_VSYNC_FREQ
#define VGA_EXT_MAX_VSYNC_FREQ      (0)
#endif

#ifndef VGA_EXT_MIN_VSYNC_FREQ
#define VGA_EXT_MIN_VSYNC_FREQ      (0)
#endif

//----------------------------------------------------------------------------
// TVD option flag
//----------------------------------------------------------------------------
#ifndef TVD_ONLY_SUPPORT_NTSC
#define TVD_ONLY_SUPPORT_NTSC (0)
#endif

#ifndef TVD_CC_NRL_THRESHOLD_BLOCK
#define TVD_CC_NRL_THRESHOLD_BLOCK       (60)
#endif

#ifndef TVD_CC_NRL_THRESHOLD_UNBLOCK
#define TVD_CC_NRL_THRESHOLD_UNBLOCK     (40)
#endif

#ifndef TVD_CC_PER_THRESHOLD_BLOCK
#define TVD_CC_PER_THRESHOLD_BLOCK       (100)
#endif

#ifndef TVD_CC_PER_THRESHOLD_UNBLOCK
#define TVD_CC_PER_THRESHOLD_UNBLOCK     (90)
#endif

//----------------------------------------------------------------------------
// CEC option flag
//----------------------------------------------------------------------------
#ifndef CEC_FUNCTON_PINMUX_EN
#define CEC_FUNCTON_PINMUX_EN (1)
#endif
//----------------------------------------------------------------------------
// HDMI option flag
//----------------------------------------------------------------------------
#ifndef HDMI_SUPPORT_EXT_SWITCH
#define HDMI_SUPPORT_EXT_SWITCH (0)
#endif
#ifndef HDMI_BYPASS_INITIAL_FLOW
#define HDMI_BYPASS_INITIAL_FLOW (0)
#endif
#ifndef HDMI_OFFON_MUTE_COUNT
#define HDMI_OFFON_MUTE_COUNT (1300)	//ms
#endif
#ifndef DVI_WAIT_STABLE_COUNT
#define DVI_WAIT_STABLE_COUNT (30)
#endif
#ifndef DVI_WAIT_NOSIGNAL_COUNT
#define DVI_WAIT_NOSIGNAL_COUNT (6)	//second
#endif
#ifndef HDMI_WAIT_SCDT_STABLE_COUNT
#define HDMI_WAIT_SCDT_STABLE_COUNT (2)	//second
#endif
#ifndef HDMI_TMDS_EQ_ZERO_VALUE  // josh
#define HDMI_TMDS_EQ_ZERO_VALUE (0x1)	
#endif
#ifndef HDMI_TMDS_EQ_BOOST_VALUE  // josh
#define HDMI_TMDS_EQ_BOOST_VALUE (0xf1)	
#endif
//----------------------------------------------------------------------------
// RTC HW option flag
//----------------------------------------------------------------------------
#ifndef RTC_HW
#define RTC_HW        (RTC_INTERNAL)
#endif
#ifndef RTC_EXT_IMPL
#define RTC_EXT_IMPL	"mtk/mtk_rtc_ext.c"
#endif
// #ifndef RTC_EXT_INTGPIO
// #define RTC_EXT_INTGPIO 204
// #endif
#ifndef RTC_EXT_CLKDIV
#define RTC_EXT_CLKDIV 0x100
#endif

//----------------------------------------------------------------------------
// default NPTV source table.
//----------------------------------------------------------------------------
#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE              "mtk/source_table.c"
#endif

//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF              "mtk/hdmi_conf.c"
#endif

//----------------------------------------------------------------------------
// default VGA table.
//----------------------------------------------------------------------------
#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE "mtk/mtk_vga.h"
#endif

//----------------------------------------------------------------------------
// Default Quality
//----------------------------------------------------------------------------
#ifdef CC_MT5382P
    #ifndef QTYTBL_INCLUDE_FILE
     #define QTYTBL_INCLUDE_FILE "mtk/QTY_DEFAULT_5382P/mtk_QtyTbl.h"
    #endif

    #ifndef COLOR_PARAM_INCLUDE_FILE
     #define COLOR_PARAM_INCLUDE_FILE "mtk/PANEL_DEFAULT_5382P/color_param.h"
    #endif
#else
    #ifndef QTYTBL_INCLUDE_FILE
     #define QTYTBL_INCLUDE_FILE "mtk/QTY_DEFAULT_5381/mtk_QtyTbl.h"
    #endif

    #ifndef COLOR_PARAM_INCLUDE_FILE
     #define COLOR_PARAM_INCLUDE_FILE "mtk/PANEL_DEFAULT_5381/color_param.h"
    #endif
#endif

#ifdef SUPPORT_FLASH_PQ
	#define FLASH_PQ_BASE_ADDRESS				(0x283E0000)
	#define FLASH_PQ_BASE_OFFSET				(0)
	#define FLASH_PQ_BLOCK_SIZE					(1)	// Unit: 64KB.
	#define SUPPORT_FLASH_PQ_MULTI_GAMMA		(1)
	#define SUPPORT_FLASH_PQ_MULTI_SCE			(0)
	#define SUPPORT_FLASH_PQ_MULTI_QTY			(1)
	#define SUPPORT_FLASH_PQ_MULTI_SMART_PIC	(0)
#endif // #if SUPPORT_FLASH_PQ

//----------------------------------------------------------------------------
// Default UART
//----------------------------------------------------------------------------
#ifndef UART1_SUPPORT_ENABLE
#define UART1_SUPPORT_ENABLE    1
#endif

//----------------------------------------------------------------------------
// Default power down support
//----------------------------------------------------------------------------
#ifndef PDWNC_EXT_IMPL
#define PDWNC_EXT_IMPL                  "mtk/mtk_pdwnc_ext.c"
#endif

//----------------------------------------------------------------------------
// MT8292 GPIO support
//----------------------------------------------------------------------------
#ifndef FLAG_MT8292_GPIO_SUPPORT
#define FLAG_MT8292_GPIO_SUPPORT 0
#endif
#ifndef MT8292_RESET_GPIO
#define MT8292_RESET_GPIO (0xFFFFFFFF)
#endif
#ifndef MT8292_MUTE_GPIO
#define MT8292_MUTE_GPIO (0xFFFFFFFF)
#endif
#ifndef MT8280_RESET_GPIO
#define MT8280_RESET_GPIO (0)
#endif

//----------------------------------------------------------------------------
// MT8295 GPIO support
//----------------------------------------------------------------------------
#define INTERNAL_PODCI (0)
#define EXTERNAL_PODCI (1)     //which means 8295
#ifndef CI_8295_RESET_GPIO
#define CI_8295_RESET_GPIO  OPCTRL(2)
#endif
#ifndef CI_PMX_CFG
#define CI_PMX_CFG      (INTERNAL_PODCI)
#endif
#ifndef CI_8295_INT_GPIO
#define CI_8295_INT_GPIO  (4)     //gpio4
#endif
#endif /* DEFAULT_H */

//----------------------------------------------------------------------------
// ON_CHIP_AUTO_COLOR support
//------------------------------------------------------------------------------
#ifndef ON_CHIP_AUTO_COLOR
#define ON_CHIP_AUTO_COLOR    0   // 1:enable  0:disable
#endif
#ifndef MTK_PUBLIC_VERSION_ON_CHIP_AUTO_COLOR
#define MTK_PUBLIC_VERSION_ON_CHIP_AUTO_COLOR  1
#endif
//#define YPBPR_COUNTER_ADDR    0   // define the EEPROM absolute address for the counter of YPBPR online offset calibration
//#define VGA_COUNTER_ADDR      0   // define the EEPROM absolute address for the counter of VGA online offset calibration
//#define OFFSET_CALI_COUNTER   0xff  //0xff means ignore the above two counter

