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
#define DEFAULT_DRAM_TYPE           DDR_II_x2
#endif

#ifndef DEFAULT_DRAM_COLADDR
#define DEFAULT_DRAM_COLADDR        (COL_ADDR_BIT_10)
#endif

#ifndef DEFAULT_DRAM_PARAM_E
#define DEFAULT_DRAM_PARAM_E        (0)
#endif

#ifndef FLAG_DDR_QFP
#define FLAG_DDR_QFP                0
#endif

#ifndef DEFAULT_DDR_CLOCK
#define DEFAULT_DDR_CLOCK           (783000000)
#endif

#ifndef DEFAULT_DDR_CL
#if (FLAG_DDR_QFP == 1)
#define DEFAULT_DDR_CL              (6)
#else
#if (DEFAULT_DDR_CLOCK > 800000000)
#define DEFAULT_DDR_CL              (7)
#else
#define DEFAULT_DDR_CL              (6)
#endif
#endif
#endif

#ifndef FLAG_DMPLL_SPECTRUM
#define FLAG_DMPLL_SPECTRUM         (0)
#endif

#ifndef DMPLL_SPECTRUM_PERMILLAGE
#define DMPLL_SPECTRUM_PERMILLAGE   (50)
#endif

#ifndef DRAM_PRIORITY_LIST

#if 0
        // Audio/0 > Demux/PVR/1 > OSD/3 > B2R/5 > VBI/3D/TVE/2 > NR/PSCAN/4 SCALAR/7 > MPEG-MC/8 > VDEC-MISC/9 > CPU/6
        #define DRAM_PRIORITY_LIST          ((UINT8*)"0135247896")
#endif        
//        // Audio/0 > Demux/PVR/1 > OSD/3 > SCALAR/7 > NR/PSCAN/4 > B2R/5 > VBI/3D/TVE/2 > MPEG-MC/8 > VDEC-MISC/9 > CPU/6
//        #define DRAM_PRIORITY_LIST          ((UINT8*)"0162459378")
        
//        // Audio/0 > Demux/PVR/1 > OSD/3 > SCALAR/7 > B2R/5 > NR/PSCAN/4 > VBI/3D/TVE/2 > MPEG-MC/8 > VDEC-MISC/9 > CPU/6
//        #define DRAM_PRIORITY_LIST          ((UINT8*)"0162549378")

        // Audio/0 > Demux/PVR/1 > OSD/3 > B2R/5 > SCALAR/7 > NR/PSCAN/4 > VBI/3D/TVE/2 > MPEG-MC/8 > VDEC-MISC/9 > CPU/6
        #define DRAM_PRIORITY_LIST          ((UINT8*)"0162539478")
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

#ifndef DEFAULT_DRAM_8_BANKS
#define DEFAULT_DRAM_8_BANKS      0
#endif

#ifndef DEFAULT_DRAM_RODT
#define DEFAULT_DRAM_RODT      150
#endif

#ifndef DEFAULT_DRAM_WODT
#if (FLAG_DDR_QFP == 1)
#define DEFAULT_DRAM_WODT      150
#else
#define DEFAULT_DRAM_WODT      50
#endif
#endif

#ifndef FLAG_FORCE_32BIT
#define FLAG_FORCE_32BIT            0
#endif

#ifndef FLAG_FULL_FREQ
#define FLAG_FULL_FREQ              0
#endif

#ifndef FLAG_SINGLE_DQS
#define FLAG_SINGLE_DQS             0
#endif

#ifndef FLAG_RUN_ON_NOR
#define FLAG_RUN_ON_NOR             0
#endif

#ifndef SYS_RESET_TIMER
#define SYS_RESET_TIMER             0xcdfe60 //0xcdfe60 <=> 500ms under 27MHz //0x59019 <=>13.5ms under 27MHz //(0xf00000 / 10) <=>524.3ms under 3Mhz
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

#ifndef LVDS_A_CTRL_EVEN
#define LVDS_A_CTRL_EVEN (0)
#endif
#ifndef LVDS_A_CTRL_ODD
#define LVDS_A_CTRL_ODD (1)
#endif
#ifndef LVDS_B_CTRL_EVEN
#define LVDS_B_CTRL_EVEN (2)
#endif
#ifndef LVDS_B_CTRL_ODD
#define LVDS_B_CTRL_ODD (3)
#endif

//----------------------------------------------------------------------------
// Default usb upgrade setting
//----------------------------------------------------------------------------
#ifndef AUTO_USB_UPGRADE_ENABLE
#define AUTO_USB_UPGRADE_ENABLE     (0)
#endif
#ifndef EEPROM_USB_UPGRADE_ENABLE
#define EEPROM_USB_UPGRADE_ENABLE     (1)
#endif
#ifndef USB_UPGRADE_FILE_NAME
#define USB_UPGRADE_FILE_NAME       ("upgrade.pkg")
#endif
#ifndef USB_UPGRADE_FILE_NAME_PREFIX
#define USB_UPGRADE_FILE_NAME_PREFIX    ("upgrade")
#endif
#ifndef USB_UPGRADE_FILE_NAME_SUFFIX
#define USB_UPGRADE_FILE_NAME_SUFFIX    (".pkg")
#endif
#ifndef LOADER_STANDBY_ENABLE_IF_UPGRADE_BREAK
#define LOADER_STANDBY_ENABLE_IF_UPGRADE_BREAK (0)
#endif
#ifndef LOADER_UPGRADE_BREAK_HANDLE_FUNC
#define LOADER_UPGRADE_BREAK_HANDLE_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_VER_CMP_FUNC
#define LOADER_UPGRADE_VER_CMP_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_CB_FUNC
#define LOADER_UPGRADE_DLM_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC
#define LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC
#define LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC (NULL)
#endif
#ifndef LOADER_UPGRADE_DLM_VER_DATA_OFF
#define LOADER_UPGRADE_DLM_VER_DATA_OFF 12
#endif
#ifndef LOADER_UPGRADE_DLM_VER_DATA_LEN
#define LOADER_UPGRADE_DLM_VER_DATA_LEN 16
#endif
#ifndef LOADER_UPGRADE_DLM_FORCE_VER
#define LOADER_UPGRADE_DLM_FORCE_VER "ffffffff"
#endif
//----------------------------------------------------------------------------
// Loader Misc
//----------------------------------------------------------------------------
#ifndef LOADER_ENV_INIT_FUNC
#define LOADER_ENV_INIT_FUNC        (NULL)
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
#ifndef LOADER_MSG_BACKGROUND
#define LOADER_MSG_BACKGROUND       (0xeeeeee)
#endif

//----------------------------------------------------------------------------
// Driver logo
//----------------------------------------------------------------------------
#ifndef DRV_LOGO_ENABLE
#define DRV_LOGO_ENABLE       (0)
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
/*
#ifndef DEFAULT_TUNER_TYPE_OFST0
#define DEFAULT_TUNER_TYPE_OFST0	(0x109)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST1
#define DEFAULT_TUNER_TYPE_OFST1	(0x4DE)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST2
#define DEFAULT_TUNER_TYPE_OFST2	(0x7F6)
#endif
*/
#ifndef DEFAULT_TUNER_CTRL_ANA
#define DEFAULT_TUNER_CTRL_ANA		(0)
#endif
#ifndef DEFAULT_DEMOD_RESET_GPIO
#define DEFAULT_DEMOD_RESET_GPIO    (GPIO(8))
#endif
//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT        (PANEL_DEFAULT)
#endif
// backlight power on/off gpio pin
// #ifndef BACKLIGHT_GPIO
// #define BACKLIGHT_GPIO              (GPIO(3))
// #endif
// gpio output value when turn on backlight
// #ifndef BACKLIGHT_ON_VAL
// #define BACKLIGHT_ON_VAL            (0)
// #endif
// lvds power on/off gpio pin
// #ifndef PANELPOWER_GPIO
// #define PANELPOWER_GPIO             (GPIO_OPWM2)//(GPIO(3))
// #endif
// gpio output value when turn on lvds
// #ifndef PANELPOWER_ON_VAL
// #define PANELPOWER_ON_VAL           (0)
// #endif

// spread spectrum range, default is 1% for customer model
#ifndef SPREAD_SPECTRUM_PERMILLAGE
#define SPREAD_SPECTRUM_PERMILLAGE  (10)
#endif

//back light dimming (pwm0/1/2)
#ifndef PANEL_BACKLIGHT_PWM_PORT
#define PANEL_BACKLIGHT_PWM_PORT			(1)
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

#ifndef PANEL_PIXEL_SHIFT_H_MAX
#define PANEL_PIXEL_SHIFT_H_MAX (0)
#endif
#ifndef PANEL_PIXEL_SHIFT_V_MAX
#define PANEL_PIXEL_SHIFT_V_MAX (0)
#endif

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
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
#define AUDIO_BBE_PROCESS               (14)
#endif
//BBE Lo Contour
#ifndef AUDIO_BBE_LO_CONTOUR
#define AUDIO_BBE_LO_CONTOUR            (14)
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
// C speaker type, large/small
#ifndef AUDIO_CENTER_SPK_TYPE_LARGE
#define AUDIO_CENTER_SPK_TYPE_LARGE     (TRUE)
#endif
// CH9/10 speaker type, large/small
#ifndef AUDIO_CH9_10_SPK_TYPE_LARGE
#define AUDIO_CH9_10_SPK_TYPE_LARGE     (TRUE)
#endif
#ifndef AUDIO_SUBWOOFER_ON
#define AUDIO_SUBWOOFER_ON              (TRUE)
#endif


// Audio Description setting
#ifndef AUDIO_AUDIO_DESCRIPTION_FLAG
#define AUDIO_AUDIO_DESCRIPTION_FLAG     0x2  //0x2:If Audio description is on, then Audio description data is only added  in CH9/10
#endif                                                                 //0x4:If Audio descritpion is on , then Audio description data is added for all channel

// Downmix channel(CH9/10) postion setting
#ifndef AUDIO_DOWNMIX_CHANNEL_POSITION
#define AUDIO_DOWNMIX_CHANNEL_POSITION     0x0  //0x0:Downmix channel do all post processing
#endif                                                                      //0x1:doing downmix after input source gain and before AVC
                                                                               //0x3:Downmix channel do not do post processing
// Downmix signal to L/R ?
#ifndef AUDIO_DOWNMIX_LR
#define AUDIO_DOWNMIX_LR     0x0                           //0x0: didn't  Downmix to L/R channel after Reverbation and before BassManagement
#endif                                                                      //0x1:Downmix to L/R channel after Reverbation and before BassManagement

// Downmix signal to LS/RS/C/LFE ?
#ifndef AUDIO_MULTI_PAIR_OUTPUT
#define AUDIO_MULTI_PAIR_OUTPUT     0x0                  //0x0: didn't  Copy Downmix channel to all the other channels(LS/RS/C/SUB)
#endif                                                                      //0x1:Copy Downmix channel to all the other channels(LS/RS/C/SUB)

// BI2S from GPIO 2~5
#ifndef AUDIO_BI2S_FROM_GPIO_2_5
#define AUDIO_BI2S_FROM_GPIO_2_5    (FALSE)
#endif

// BI2S from AI2S
#ifndef AUDIO_BI2S_FROM_AI2S
#define AUDIO_BI2S_FROM_AI2S    (FALSE)
#endif

//AI2S use for speaker
#ifndef AUDIO_AI2S_USE_FOR_SPEAKER
#define AUDIO_AI2S_USE_FOR_SPEAKER    (FALSE)
#endif

// PCM prebuf bank number. 256 sample/bank
#ifndef AUDIO_PCM_PREBUF_BANK_NUM
#define AUDIO_PCM_PREBUF_BANK_NUM       (7)
#endif

// Bypass DSP post processing, only used in specified model (ex. SLT, testing)
#ifndef AUDIO_BYPASS_POST_PROC
#define AUDIO_BYPASS_POST_PROC          (FALSE)
#endif

#ifndef AUDIO_ADAC_USAGE
#define AUDIO_ADAC_USAGE_DEFAULT
#define AUDIO_ADAC_USAGE                (&_arMtkAdacUsage)
#endif

#ifndef AUDIO_AOUT_PAD_CFG
#define AUDIO_AOUT_PAD_CFG_DEFAULT
#define AUDIO_AOUT_PAD_CFG              (&_arMtkAoutPadTable)
#endif

#ifndef AUDIO_INTER_DAC_USAGE
#define AUDIO_INTER_DAC_USAGE_DEFAULT
#define AUDIO_INTER_DAC_USAGE           (&_arMtkInterDacUsage)
#endif

#if 0
#ifndef AUDIO_INTER_DAC_SPECIAL_USAGE
#define AUDIO_INTER_DAC_SPECIAL_USAGE_DEFAULT
#define AUDIO_INTER_DAC_SPECIAL_USAGE           (&_arMtkInterDacSpecialUsage)
#endif
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

#ifndef AUDIO_AMP_RESET_GPIO_NUM
#define AUDIO_AMP_RESET_GPIO_NUM        (0xffffffff)
#endif

#ifndef AUDIO_AMP_RESET_GPIO_POLARITY
#define AUDIO_AMP_RESET_GPIO_POLARITY   AUD_GPIO_LOW_ENALBE
#endif

#ifndef AUDIO_INPUT_SW_GPIO_SEL
#define AUDIO_INPUT_SW_GPIO_SEL_DEFAULT
#define AUDIO_INPUT_SW_GPIO_SEL         (&_arMtkInputSwGpio)
#endif

#ifndef AUDIO_INPUT_MUX_SEL_TABLE
#define AUDIO_INPUT_MUX_SEL_TABLE_DEFAULT
#define AUDIO_INPUT_MUX_SEL_TABLE       (&_rMtkAudInputMuxTable)
#endif

#ifndef AUDIO_OUT_FMT_TABLE
#define AUDIO_OUT_FMT_TABLE_DEFAULT
#define AUDIO_OUT_FMT_TABLE             (&_arMtkAudOutFmtTable)
#endif

#ifndef AUDIO_OUT_FMT_TABLE2
#define AUDIO_OUT_FMT_TABLE2_DEFAULT
#define AUDIO_OUT_FMT_TABLE2            (&_arMtkAudOutFmtTable2)
#endif

// #ifndef AUDIO_SPECIAL_INPUT_MUX_SEL_TABLE
// #define AUDIO_SPECIAL_INPUT_MUX_SEL_TABLE       (&_rMtkAudSpecialInputMuxTable)
// #endif

#ifndef AUDIO_IN_FMT_TABLE
#define AUDIO_IN_FMT_TABLE_DEFAULT
#define AUDIO_IN_FMT_TABLE              (&_arMtkAudInFmtTable)
#endif

#ifndef AUDIO_INPUT_SRC_VOL
#define AUDIO_INPUT_SRC_VOL_DEFAULT
#define AUDIO_INPUT_SRC_VOL             (&_aai2MtkAudInputSrcVol)
#endif

#ifndef AUDIO_AV_INPUT_SRC_VOL
#define AUDIO_AV_INPUT_SRC_VOL_DEFAULT
#define AUDIO_AV_INPUT_SRC_VOL          (&_ai2MtkAudAvInputSrcVol)
#endif

#ifndef AUDIO_INPUT_SRC_LINE_OUT_VOL
#define AUDIO_INPUT_SRC_LINE_OUT_VOL_DEFAULT
#define AUDIO_INPUT_SRC_LINE_OUT_VOL    (&_aai2MtkAudInputSrcLineOutVol)
#endif

#ifndef AUDIO_CHL_EQ_CFG
#define AUDIO_CHL_EQ_CFG_DEFAULT
#define AUDIO_CHL_EQ_CFG                (&_aai1MtkAudChlEqCfg)
#endif

#ifndef AUDIO_CHL_PEQ_FQ_CFG
#define AUDIO_CHL_PEQ_FQ_CFG_DEFAULT
#define AUDIO_CHL_PEQ_FQ_CFG                (&_aai1MtkAudPeqFq)
#endif

#ifndef AUDIO_CHL_PEQ_BW_CFG
#define AUDIO_CHL_PEQ_BW_CFG_DEFAULT
#define AUDIO_CHL_PEQ_BW_CFG                (&_aai1MtkAudPeqBw)
#endif

#ifndef AUDIO_CHL_PEQ_GAIN_CFG
#define AUDIO_CHL_PEQ_GAIN_CFG_DEFAULT
#define AUDIO_CHL_PEQ_GAIN_CFG                (&_aai1MtkAudPeqGain)
#endif

#ifndef AUDIO_CHL_BASS_TREBLE_CFG
#define AUDIO_CHL_BASS_TREBLE_CFG_DEFAULT
#define AUDIO_CHL_BASS_TREBLE_CFG                (&_aai1MtkAudBassTrebleTable)
#endif

#ifndef AUD_CHL_SOUND_MODE_CFG
#define AUD_CHL_SOUND_MODE_CFG_DEFAULT
#define AUD_CHL_SOUND_MODE_CFG                (&_arMtkSoundModeCase)
#endif
#ifndef AUDIO_CHL_VS_MODE_CFG
#define AUDIO_CHL_VS_MODE_CFG_DEFAULT
#define AUDIO_CHL_VS_MODE_CFG                (&_aai1MtkAudVSTable)
#endif

#ifndef AUDIO_INPUT_SRC_AGC_FLAG
#define AUDIO_INPUT_SRC_AGC_FLAG_DEFAULT
#define AUDIO_INPUT_SRC_AGC_FLAG        (&_aafgMtkAudInputSrcAgcFlag)
#endif

#ifndef AUDIO_INPUT_SRC_AGC_VOL
#define AUDIO_INPUT_SRC_AGC_VOL_DEFAULT
#define AUDIO_INPUT_SRC_AGC_VOL         (&_aau4MtkAudInputSrcAgcVol)
#endif

#ifndef AUDIO_VOL_BASE_LEVEL
#define AUDIO_VOL_BASE_LEVEL            (100)
#endif

#ifndef AUDIO_LINE_OUT_FIX_VOL
#define AUDIO_LINE_OUT_FIX_VOL            (100)
#endif

#ifndef AUDIO_VOL_TABLE
#define AUDIO_VOL_TABLE_DEFAULT
#define AUDIO_VOL_TABLE                 (&_au4VolumeTable)
#endif

#ifndef AUDIO_CHANNEL_VOL_TABLE
#define AUDIO_CHANNEL_VOL_TABLE_DEFAULT
#define AUDIO_CHANNEL_VOL_TABLE         (&_au4ChannelVolumeTable)
#endif


#ifndef AUDIO_AVC_TARGET_LEV
#define AUDIO_AVC_TARGET_LEV_DEFAULT
#define AUDIO_AVC_TARGET_LEV            (_ai2MtkAudAvcTarget)
#endif

#ifndef AUDIO_AVC_GAIN_UP
#define AUDIO_AVC_GAIN_UP_DEFAULT
#define AUDIO_AVC_GAIN_UP               (_ai2MtkAudAvcGainUp)
#endif

#ifndef AUDIO_AVC_ATTACK_THRES
#define AUDIO_AVC_ATTACK_THRES          (0)
#endif

#ifndef AUDIO_AVC_ADJUST_RATE
#define AUDIO_AVC_ADJUST_RATE_DEFAULT
#define AUDIO_AVC_ADJUST_RATE           (_ai2MtkAudAvcAdjRate)
#endif

#ifndef AUDIO_CHL_VOL_EXTRA_GAIN
#define AUDIO_CHL_VOL_EXTRA_GAIN_DEFAULT
#define AUDIO_CHL_VOL_EXTRA_GAIN        (&_aai4MtkAudChVolExtraGain)
#endif

#ifndef AUDIO_CHL_VOLUME_OFFSET_MAP
#define AUDIO_CHL_VOLUME_OFFSET_MAP_DEFAULT
#define AUDIO_CHL_VOLUME_OFFSET_MAP     (&_aai4MtkAudVolOffsetMap)
#endif

#ifndef AUDIO_CHANNEL_DELAY
#define AUDIO_CHANNEL_DELAY_DEFAULT
#define AUDIO_CHANNEL_DELAY             (&_au2MtkAudChDelay)
#endif

#if 1  //MT5387Merge ... check later
#ifndef AUDIO_IEC_COPYRIGHT
#define AUDIO_IEC_COPYRIGHT_DEFAULT
#define AUDIO_IEC_COPYRIGHT             (&_au1MtkAudIecCopyright)
#endif

#ifndef AUDIO_IEC_CATEGORY
#define AUDIO_IEC_CATEGORY_DEFAULT
#define AUDIO_IEC_CATEGORY              (&_au1MtkAudIecCategory)
#endif

#ifndef AUDIO_IEC_WORD_LENGTH
#define AUDIO_IEC_WORD_LENGTH_DEFAULT
#define AUDIO_IEC_WORD_LENGTH           (&_au1MtkAudIecWordLength)
#endif
#endif

#ifndef AUDIO_IEC_CHANNEL_STATUS_BYSOURCE
#define AUDIO_IEC_CHANNEL_STATUS_BYSOURCE_DEFAULT
#define AUDIO_IEC_CHANNEL_STATUS_BYSOURCE             (&_au1MtkAudIecChannelStatusBySource)
#endif

#ifndef AUDIO_IECRAW_DELAY_FLAG
#define AUDIO_IECRAW_DELAY_FLAG           (TRUE) // default raw delay is synced with PCM channel delay
#endif

#ifndef AUDIO_IECRAW_DELAY
#define AUDIO_IECRAW_DELAY               (0) // in ms, default raw delay is 0 ms
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
#define AUDIO_CUST_SPEC_POLARITY  0
#endif

#ifndef AUDIO_CUST_SPEC2_GPIO_CTRL
#define AUDIO_CUST_SPEC2_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_SPEC2_POLARITY
#define AUDIO_CUST_SPEC2_POLARITY  0
#endif

#ifndef AUDIO_CUST_HP_GPIO_CTRL
#define AUDIO_CUST_HP_GPIO_CTRL (0xffffffff)
#endif

#ifndef AUDIO_CUST_HP_POLARITY
#define AUDIO_CUST_HP_POLARITY  0
#endif

#ifndef AUDIO_Digital_Data_TABLE
#define AUDIO_Digital_Data_TABLE_DEFAULT
#define AUDIO_Digital_Data_TABLE 	(&_afgMtkAudDigitalDataUsage)
#endif

#ifndef AUDIO_TUNER_ATD
#define AUDIO_TUNER_ATD     (TRUE)
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

#ifndef AUDIO_TV_SYS_FINE_TUNE_VOL
#define AUDIO_TV_SYS_FINE_TUNE_VOL_DEFAULT
#define AUDIO_TV_SYS_FINE_TUNE_VOL        (&_au1MtkTVFineTuneVol)
#endif

#ifndef AUDIO_MTS_FACTORY_MODE
#define AUDIO_MTS_FACTORY_MODE_DEFAULT
#define AUDIO_MTS_FACTORY_MODE        (&_arMtsFactory)
#endif

#ifndef AUDIO_A2_FACTORY_MODE
#define AUDIO_A2_FACTORY_MODE_DEFAULT
#define AUDIO_A2_FACTORY_MODE        (&_arA2Factory)
#endif

#ifndef AUDIO_PAL_FACTORY_MODE
#define AUDIO_PAL_FACTORY_MODE_DEFAULT
#define AUDIO_PAL_FACTORY_MODE        (&_arPalFactory)
#endif

// Limiter mode. 0: Off, 1: Adaptive mode, 2: Fix mode
#ifndef AUDIO_LIMITER_MODE
#define AUDIO_LIMITER_MODE  (2)
#endif

// Limiter fix mode threshold, the max. value is 0x100000
#ifndef AUDIO_LIMITER_FIX_THRESHOLD
#define AUDIO_LIMITER_FIX_THRESHOLD     (0x100000)
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

#ifndef AUDIO_NVM_GET_COUNTRY
#define AUDIO_NVM_GET_COUNTRY (FALSE)
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

#ifndef AUDIO_AL3AR3_PHASE_INVERSE
#define AUDIO_AL3AR3_PHASE_INVERSE           (FALSE)
#endif

#ifndef AUDIO_InternalLDO_ENABLE
#define AUDIO_InternalLDO_ENABLE           (FALSE)
#endif

#ifndef AUDIO_DIGITAL_AMP_RESET_GPIO
#define AUDIO_DIGITAL_AMP_RESET_GPIO (0xffffffff)
#endif

#ifndef AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY
#define AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY  AUD_GPIO_HIGH_ENALBE
#endif

#ifndef FLASH_AQ_BASE_PARTITION
#define FLASH_AQ_BASE_PARTITION     (0xFF)
#endif

#ifndef AUDIO_FLASH_AQ_DATA
#define AUDIO_FLASH_AQ_DATA         (0)
#endif

#ifndef AUDIO_FLASH_AQ_DATA_SIZE
#define AUDIO_FLASH_AQ_DATA_SIZE    (0)
#endif

#ifndef AUDIO_FLASH_AQ_DATA_DSCRPT
#define AUDIO_FLASH_AQ_DATA_DSCRPT  (0)
#endif

#ifndef AUDIO_FLASH_AQ_DATA_DSCRPT_SIZE
#define AUDIO_FLASH_AQ_DATA_DSCRPT_SIZE (0)
#endif
//====================================


//----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#ifndef FLAG_EXT_STATUS
#define FLAG_EXT_STATUS             (0)
#endif
// #ifndef STANDBY_SET_GPIO
// #define STANDBY_SET_GPIO            (OPCTRL(7))
// #endif
// #ifndef POWER_STATUS_GPIO
// #define POWER_STATUS_GPIO           (OPCTRL(8))
// #endif
#ifndef STDBY_USE_PWR_BTN_CNT
#define STDBY_USE_PWR_BTN_CNT       (0)
#endif
#ifndef STDBY_USE_PREV_PWR_STATE
#define STDBY_USE_PREV_PWR_STATE    (0)
#endif
#ifndef STDBY_USE_VGA_STATUS
#define STDBY_USE_VGA_STATUS        (0)
#endif
#ifndef GPIO_TIMER_INIT_FUNC
#define GPIO_TIMER_INIT_FUNC        (NULL)
#endif

// #ifndef POWER_LED_GPIO
// #define POWER_LED_GPIO              (OPCTRL(0))
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

#ifndef FLAG_SUPPORT_POWER_LED_OFF
#define FLAG_SUPPORT_POWER_LED_OFF      (0)
#endif

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SYSTEM EEPROM customization
//----------------------------------------------------------------------------
#ifndef SYSTEM_EEPROM_BUS
#define SYSTEM_EEPROM_BUS                 (0)
#endif
#ifndef SYSTEM_EEPROM_ADDRESS
#define SYSTEM_EEPROM_ADDRESS             (0xA0)
#endif
#ifndef SYSTEM_EEPROM_SIZE
#define SYSTEM_EEPROM_SIZE                (2048)
#endif
#ifndef SYSTEM_EEPROM_CLKDIV
#define SYSTEM_EEPROM_CLKDIV              (0x100)
#endif
#ifndef SYSTEM_EEPROM_PAGESIZE
#define SYSTEM_EEPROM_PAGESIZE              (8)
#endif
// #ifndef SYSTEM_EEPROM_WP_GPIO
// #define SYSTEM_EEPROM_WP_GPIO             (OPCTRL(5))
// #endif
// #ifndef SYSTEM_EEPROM_WP_ENABLE_POLARITY
// #define SYSTEM_EEPROM_WP_ENABLE_POLARITY  (0)
// #endif
#ifndef SYSTEM_EEPROM_WAIT_TIME
#define SYSTEM_EEPROM_WAIT_TIME              (20)
#endif

#ifndef SYSTEM_EEPROM_ENABLE_RETRY
#define SYSTEM_EEPROM_ENABLE_RETRY            (0)
#endif


//----------------------------------------------------------------------------
// EDID EEPROM customization
//----------------------------------------------------------------------------
// #ifndef EDID_EEPROM_WP_GPIO
// #define EDID_EEPROM_WP_GPIO               (OPCTRL(3))
// #endif
// #ifndef EDID_EEPROM_WP_ENABLE_POLARITY
// #define EDID_EEPROM_WP_ENABLE_POLARITY    (0)
// #endif

//----------------------------------------------------------------------------
// HDCP EEPROM customization
//----------------------------------------------------------------------------
#ifndef HDCP_EEPROM_BUS
#define HDCP_EEPROM_BUS                   (0)
#endif
#ifndef HDCP_EEPROM_ADDRESS
#define HDCP_EEPROM_ADDRESS               (0xA0)
#endif
#ifndef HDCP_EEPROM_SIZE
#define HDCP_EEPROM_SIZE                  (2048)
#endif
#ifndef HDCP_EEPROM_CLKDIV
#define HDCP_EEPROM_CLKDIV                (0x100)
#endif
#ifndef HDCP_EEPROM_OFFSET
#define HDCP_EEPROM_OFFSET                  (0)
#endif
// #ifndef HDCP_EEPROM_WP_GPIO
// #define HDCP_EEPROM_WP_GPIO               (OPCTRL(5))
// #endif
// #ifndef HDCP_EEPROM_WP_ENABLE_POLARITY
// #define HDCP_EEPROM_WP_ENABLE_POLARITY    (0)
// #endif

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
#define DRVCUST_EEPROM_DTV_DRIVER_SIZE	((UINT32)96)
#endif
#ifndef DRVCUST_EEPROM_NPTV_DRIVER_OFFSET
#define DRVCUST_EEPROM_NPTV_DRIVER_OFFSET	((UINT32)(DRVCUST_EEPROM_DTV_DRIVER_OFFSET + DRVCUST_EEPROM_DTV_DRIVER_SIZE))
#endif
#ifndef DRVCUST_EEPROM_NPTV_DRIVER_SIZE
#define DRVCUST_EEPROM_NPTV_DRIVER_SIZE	((UINT32)512)
#endif
#ifndef DRVCUST_EEPROM_DRIVER_SIZE
#define DRVCUST_EEPROM_DRIVER_SIZE			((UINT32)(DRVCUST_EEPROM_NPTV_DRIVER_OFFSET + DRVCUST_EEPROM_NPTV_DRIVER_SIZE))
#endif
#ifndef DRVCUST_EEPROM_MW_OFFSET
#define DRVCUST_EEPROM_MW_OFFSET	((UINT32)(DRVCUST_EEPROM_NPTV_DRIVER_OFFSET + DRVCUST_EEPROM_NPTV_DRIVER_SIZE))
#endif
#ifndef DRVCUST_EEPROM_MW_DRIVER_SIZE
#define DRVCUST_EEPROM_MW_DRIVER_SIZE	((UINT32)192)
#endif
#ifndef EEPROM_CEC_MENU_LANGUAGE_OFFSET
#define EEPROM_CEC_MENU_LANGUAGE_OFFSET      (0x0)
#endif
#ifndef EEPROM_CEC_ENABLE_OFFSET
#define EEPROM_CEC_ENABLE_OFFSET      (0x0)
#endif
#ifndef EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE_OFFSET
#define EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE_OFFSET      (0x0)
#endif



// read panel from eeprom
#ifndef PANEL_INDEX_FROM_EEPROM
#define PANEL_INDEX_FROM_EEPROM     (0)
#endif
#ifndef PANEL_INDEX_OFFSET1
#define PANEL_INDEX_OFFSET1         (DRVCUST_EEPROM_DRIVER_SIZE-1)  // store panel index at the last byte of NPTV EEPROM
#endif

#ifndef PANEL_AUTO_DETECT_OFFSET
#define PANEL_AUTO_DETECT_OFFSET    (DRVCUST_EEPROM_DRIVER_SIZE-2)  // store panel index at the last byte of NPTV EEPROM
#endif

#ifndef EEPROM_COUNTRY_OFFSET
#define EEPROM_COUNTRY_OFFSET       (DRVCUST_EEPROM_DRIVER_SIZE-3)  // store it at the last  of NPTV EEPROM
#endif

#ifndef DEFAULT_TUNER_TYPE_OFST0
#define DEFAULT_TUNER_TYPE_OFST0    (DRVCUST_EEPROM_DRIVER_SIZE-4)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST1
#define DEFAULT_TUNER_TYPE_OFST1    (DRVCUST_EEPROM_DRIVER_SIZE-5)
#endif
#ifndef DEFAULT_TUNER_TYPE_OFST2
#define DEFAULT_TUNER_TYPE_OFST2    (DRVCUST_EEPROM_DRIVER_SIZE-6)
#endif
//#define PANEL_INDEX_OFFSET2         (0x52f)
//#define PANEL_INDEX_OFFSET3         (0x6cf)

//todo: 
//#ifdef PANEL_INDEX_MAGIC_ENABLE
//#ifndef PANEL_INDEX_MAGIC
//#define PANEL_INDEX_MAGIC           (PANEL_INDEX_OFFSET1-1)
//#endif
//#ifndef PANEL_INDEX_FLAG_OFFSET
//#define PANEL_INDEX_FLAG_OFFSET     (PANEL_INDEX_MAGIC-1)
//#endif
//#endif /* PANEL_INDEX_MAGIC_ENABLE */

#ifndef FRAME_TRACK_MODE
#define FRAME_TRACK_MODE  (1)
#endif
#ifndef FRAME_TRACK_STABLE_COUNT
#define FRAME_TRACK_STABLE_COUNT  (0)
#endif

//----------------------------------------------------------------------------
// NOR customization
//----------------------------------------------------------------------------
#ifndef NOR_FLASH_TOTAL_SIZE
    #ifdef CC_DUAL_IMAGE_FLASH
        #ifdef NDEBUG
            #define NOR_FLASH_TOTAL_SIZE                (8*1024*1024)
        #else /* NDEBUG */
            #define NOR_FLASH_TOTAL_SIZE                (16*1024*1024)
        #endif /* NDEBUG */
    #else /* CC_DUAL_IMAGE_FLASH */
        #ifdef NDEBUG
            #define NOR_FLASH_TOTAL_SIZE                (4*1024*1024)
        #else /* NDEBUG */
            #define NOR_FLASH_TOTAL_SIZE                (8*1024*1024)
        #endif /* NDEBUG */
    #endif /* CC_DUAL_IMAGE_FLASH */
#endif

#ifndef NOR_FLASH_LOADER_SIZE
    #ifdef LOADER_MAX_SIZE
        #define NOR_FLASH_LOADER_SIZE             (LOADER_MAX_SIZE)
    #else /* LOADER_MAX_SIZE */
        #define NOR_FLASH_LOADER_SIZE             (128*1024)
    #endif /* LOADER_MAX_SIZE */
#endif

#ifndef NOR_FLASH_APPIMAGE_SIZE
    #ifdef CC_DUAL_IMAGE_FLASH
        #ifdef DUAL_CHANNEL_SUPPORT
            #define NOR_FLASH_APPIMAGE_SIZE             (((NOR_FLASH_TOTAL_SIZE >> 1) - NOR_FLASH_LOADER_SIZE) - ((2*2)*(64*1024)))
        #else /* DUAL_CHANNEL_SUPPORT */
            #define NOR_FLASH_APPIMAGE_SIZE             (((NOR_FLASH_TOTAL_SIZE >> 1) - NOR_FLASH_LOADER_SIZE) - ((1*2)*(64*1024)))
        #endif /* DUAL_CHANNEL_SUPPORT */
    #else /* CC_DUAL_IMAGE_FLASH */
        #ifdef DUAL_CHANNEL_SUPPORT
            #define NOR_FLASH_APPIMAGE_SIZE             ((NOR_FLASH_TOTAL_SIZE - NOR_FLASH_LOADER_SIZE) - ((2*2)*(64*1024)))
        #else /* DUAL_CHANNEL_SUPPORT */
            #define NOR_FLASH_APPIMAGE_SIZE             ((NOR_FLASH_TOTAL_SIZE - NOR_FLASH_LOADER_SIZE) - ((1*2)*(64*1024)))
        #endif /* DUAL_CHANNEL_SUPPORT */
    #endif /* CC_DUAL_IMAGE_FLASH */
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
        #define NOR_PART_SIZE_0            (NOR_FLASH_LOADER_SIZE)              // loader
    #endif
    #ifndef NOR_PART_SIZE_1
        #define NOR_PART_SIZE_1            (NOR_FLASH_APPIMAGE_SIZE)          // appimg
    #endif
    #ifndef NOR_PART_SIZE_2
        #define NOR_PART_SIZE_2            (2*64*1024)  // channel list
    #endif
    #ifndef NOR_PART_SIZE_3
        #ifdef CC_DUAL_IMAGE_FLASH
            #define NOR_PART_SIZE_3            (NOR_FLASH_APPIMAGE_SIZE)          // appimg x 2
        #else
            #define NOR_PART_SIZE_3            (0)          // appimg x 2
        #endif
    #endif
    #ifndef NOR_PART_SIZE_4
        #ifdef DUAL_CHANNEL_SUPPORT
            #define NOR_PART_SIZE_4            (2*64*1024)  // channel list
        #else
            #define NOR_PART_SIZE_4            (0)
        #endif
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
    #ifndef NOR_PART_SIZE_8
        #define NOR_PART_SIZE_8            (0)
    #endif
    #ifndef NOR_PART_SIZE_9
        #define NOR_PART_SIZE_9            (0)
    #endif
    #ifndef NOR_PART_SIZE_10
        #define NOR_PART_SIZE_10            (0)
    #endif
    #ifndef NOR_PART_SIZE_11
        #define NOR_PART_SIZE_11            (0)
    #endif
    #ifndef NOR_PART_SIZE_12
        #define NOR_PART_SIZE_12            (0)
    #endif
    #ifndef NOR_PART_SIZE_13
        #define NOR_PART_SIZE_13            (0)
    #endif
    #ifndef NOR_PART_SIZE_14
        #define NOR_PART_SIZE_14            (0)
    #endif
    #ifndef NOR_PART_SIZE_15
        #define NOR_PART_SIZE_15            (0)
    #endif
    #ifndef NOR_PART_SIZE_16
        #define NOR_PART_SIZE_16            (0)
    #endif
    #ifndef NOR_PART_SIZE_17
        #define NOR_PART_SIZE_17            (0)
    #endif
    #ifndef NOR_PART_SIZE_18
        #define NOR_PART_SIZE_18            (0)
    #endif
    #ifndef NOR_PART_SIZE_19
        #define NOR_PART_SIZE_19            (0)
    #endif
    #ifndef NOR_PART_SIZE_20
        #define NOR_PART_SIZE_20            (0)
    #endif
    #ifndef NOR_PART_SIZE_21
        #define NOR_PART_SIZE_21            (0)
    #endif

    #define NOR_PART_OFFSET_1  NOR_FLASH_LOADER_SIZE
    #define NOR_PART_OFFSET_2  (NOR_PART_OFFSET_1+NOR_PART_SIZE_1)
    #define NOR_PART_OFFSET_3  (NOR_PART_OFFSET_2+NOR_PART_SIZE_2)
    #define NOR_PART_OFFSET_4  (NOR_PART_OFFSET_3+NOR_PART_SIZE_3)
    #define NOR_PART_OFFSET_5  (NOR_PART_OFFSET_4+NOR_PART_SIZE_4)
    #define NOR_PART_OFFSET_6  (NOR_PART_OFFSET_5+NOR_PART_SIZE_5)
    #define NOR_PART_OFFSET_7  (NOR_PART_OFFSET_6+NOR_PART_SIZE_6)
    #define NOR_PART_OFFSET_8  (NOR_PART_OFFSET_7+NOR_PART_SIZE_7)
    #define NOR_PART_OFFSET_9  (NOR_PART_OFFSET_8+NOR_PART_SIZE_8)
    #define NOR_PART_OFFSET_10 (NOR_PART_OFFSET_9+NOR_PART_SIZE_9)
    #define NOR_PART_OFFSET_11 (NOR_PART_OFFSET_10+NOR_PART_SIZE_10)
    #define NOR_PART_OFFSET_12 (NOR_PART_OFFSET_11+NOR_PART_SIZE_11)
    #define NOR_PART_OFFSET_13 (NOR_PART_OFFSET_12+NOR_PART_SIZE_12)
    #define NOR_PART_OFFSET_14 (NOR_PART_OFFSET_13+NOR_PART_SIZE_13)
    #define NOR_PART_OFFSET_15 (NOR_PART_OFFSET_14+NOR_PART_SIZE_14)
    #define NOR_PART_OFFSET_16 (NOR_PART_OFFSET_15+NOR_PART_SIZE_15)
    #define NOR_PART_OFFSET_17 (NOR_PART_OFFSET_16+NOR_PART_SIZE_16)
    #define NOR_PART_OFFSET_18 (NOR_PART_OFFSET_16+NOR_PART_SIZE_17)
    #define NOR_PART_OFFSET_19 (NOR_PART_OFFSET_16+NOR_PART_SIZE_18)
    #define NOR_PART_OFFSET_20 (NOR_PART_OFFSET_16+NOR_PART_SIZE_19)
    #define NOR_PART_OFFSET_21 (NOR_PART_OFFSET_16+NOR_PART_SIZE_20)


    // static partition must be the multiple of 64 KB.
    #if ((NOR_PART_SIZE_0 % (64*1024)) ||\
           (NOR_PART_SIZE_1 % (64*1024)) ||\
           (NOR_PART_SIZE_2 % (64*1024)) ||\
           (NOR_PART_SIZE_3 % (64*1024)) ||\
           (NOR_PART_SIZE_4 % (64*1024)) ||\
           (NOR_PART_SIZE_5 % (64*1024)))
    #error "nor Partition must be the multiple of 64 KB"
    #endif

    // check the summation of each partition must larger than total static size.
    #if ((NOR_PART_SIZE_0 + \
            NOR_PART_SIZE_1 + \
            NOR_PART_SIZE_2 + \
            NOR_PART_SIZE_3 + \
            NOR_PART_SIZE_4 + \
            NOR_PART_SIZE_5 + \
            NOR_PART_SIZE_6 + \
            NOR_PART_SIZE_7 + \
            NOR_PART_SIZE_8 + \
            NOR_PART_SIZE_9 + \
            NOR_PART_SIZE_10) \
            > (NOR_FLASH_TOTAL_SIZE))
    #error "nor Partition Size too small !!"
    #endif

#ifndef CC_NAND_LOADER
    #ifndef LOADER_DUAL_BOOT_OFFSET
        #ifdef CC_DUAL_IMAGE_FLASH
            #define LOADER_DUAL_BOOT_OFFSET            (0x640000)
        #else /* CC_DUAL_BANL_FLASH */
            #define LOADER_DUAL_BOOT_OFFSET             (0)
        #endif /* CC_DUAL_BANL_FLASH */
    #endif
#endif
//----------------------------------------------------------------------------
// NAND customization
//----------------------------------------------------------------------------

#ifndef NAND_FLASH_TOTAL_SIZE
    #define NAND_FLASH_TOTAL_SIZE               (16*1024*1024)
#endif

#ifndef NAND_FLASH_LOADER_SIZE
    #ifndef LOADER_MAX_SIZE
        #include "loader_imghdr.h"
    #endif /* LOADER_MAX_SIZE */
        #define NAND_FLASH_LOADER_SIZE          (LOADER_MAX_SIZE)
#endif

#if defined(CC_NAND_ENABLE)
    #ifndef NAND_FLASH_APPIMAGE_SIZE
        #define NAND_FLASH_APPIMAGE_SIZE         (6*1024*1024)
    #endif
    #ifndef NAND_FLASH_UP_SIZE
        #define NAND_FLASH_UP_SIZE               0
    #endif

    #ifndef NAND_FLASH_SINGLE_CHANNELLIST_SIZE
        #define NAND_FLASH_SINGLE_CHANNELLIST_SIZE  (128*1024)
    #endif

    #ifndef NAND_FLASH_CHANNELLIST_SIZE
        #define NAND_FLASH_CHANNELLIST_SIZE      (2*128*1024)
    #endif

    #ifndef NAND_FLASH_PQ_SIZE
        #define NAND_FLASH_PQ_SIZE               (128*1024)
    #endif

    #ifndef NAND_PART_SIZE_0
        #define NAND_PART_SIZE_0        (NAND_FLASH_LOADER_SIZE)        // loader
    #endif
    #ifndef NAND_PART_SIZE_1
        #define NAND_PART_SIZE_1        (NAND_FLASH_APPIMAGE_SIZE)      // appimg
    #endif
    #ifndef NAND_PART_SIZE_2
        #define NAND_PART_SIZE_2        (NAND_FLASH_UP_SIZE)            // up image
    #endif
    #ifndef NAND_PART_SIZE_3
        #define NAND_PART_SIZE_3        (NAND_FLASH_APPIMAGE_SIZE)      // reserved
    #endif
    #ifndef NAND_PART_SIZE_4
        #define NAND_PART_SIZE_4        (NAND_FLASH_UP_SIZE)            // appimg
    #endif
    #ifndef NAND_PART_SIZE_5
        #define NAND_PART_SIZE_5        (NAND_FLASH_SINGLE_CHANNELLIST_SIZE)   // channel list
    #endif
    #ifndef NAND_PART_SIZE_6
        #define NAND_PART_SIZE_6        (NAND_FLASH_SINGLE_CHANNELLIST_SIZE)   // channel list
    #endif
    #ifndef NAND_PART_SIZE_7
        #define NAND_PART_SIZE_7        (NAND_FLASH_PQ_SIZE)            // flash PQ
    #endif
    #ifndef NAND_PART_SIZE_8
        #define NAND_PART_SIZE_8             (0)
    #endif
    #ifndef NAND_PART_SIZE_9
        #define NAND_PART_SIZE_9             (0)
    #endif
    #ifndef NAND_PART_SIZE_10
        #define NAND_PART_SIZE_10            (0)
    #endif
    #ifndef NAND_PART_SIZE_11
        #define NAND_PART_SIZE_11            (0)
    #endif
    #ifndef NAND_PART_SIZE_12
        #define NAND_PART_SIZE_12            (0)
    #endif
    #ifndef NAND_PART_SIZE_13
        #define NAND_PART_SIZE_13            (0)
    #endif
    #ifndef NAND_PART_SIZE_14
        #define NAND_PART_SIZE_14            (0)
    #endif
    #ifndef NAND_PART_SIZE_15
        #define NAND_PART_SIZE_15            (0)
    #endif
    #ifndef NAND_PART_SIZE_16
        #define NAND_PART_SIZE_16            (0)
    #endif
    #ifndef NAND_PART_SIZE_17
        #define NAND_PART_SIZE_17            (0)
    #endif
    #ifndef NAND_PART_SIZE_18
        #define NAND_PART_SIZE_18            (0)
    #endif
    #ifndef NAND_PART_SIZE_19
        #define NAND_PART_SIZE_19            (0)
    #endif
    #ifndef NAND_PART_SIZE_20
        #define NAND_PART_SIZE_20            (0)
    #endif
    #ifndef NAND_PART_SIZE_21
        #define NAND_PART_SIZE_21            (0)
    #endif

    #define NAND_PART_OFFSET_1  NAND_FLASH_LOADER_SIZE
    #define NAND_PART_OFFSET_2  (NAND_PART_OFFSET_1+NAND_PART_SIZE_1)
    #define NAND_PART_OFFSET_3  (NAND_PART_OFFSET_2+NAND_PART_SIZE_2)
    #define NAND_PART_OFFSET_4  (NAND_PART_OFFSET_3+NAND_PART_SIZE_3)
    #define NAND_PART_OFFSET_5  (NAND_PART_OFFSET_4+NAND_PART_SIZE_4)
    #define NAND_PART_OFFSET_6  (NAND_PART_OFFSET_5+NAND_PART_SIZE_5)
    #define NAND_PART_OFFSET_7  (NAND_PART_OFFSET_6+NAND_PART_SIZE_6)
    #define NAND_PART_OFFSET_8  (NAND_PART_OFFSET_7+NAND_PART_SIZE_7)
    #define NAND_PART_OFFSET_9  (NAND_PART_OFFSET_8+NAND_PART_SIZE_8)
    #define NAND_PART_OFFSET_10 (NAND_PART_OFFSET_9+NAND_PART_SIZE_9)
    #define NAND_PART_OFFSET_11 (NAND_PART_OFFSET_10+NAND_PART_SIZE_10)
    #define NAND_PART_OFFSET_12 (NAND_PART_OFFSET_11+NAND_PART_SIZE_11)
    #define NAND_PART_OFFSET_13 (NAND_PART_OFFSET_12+NAND_PART_SIZE_12)
    #define NAND_PART_OFFSET_14 (NAND_PART_OFFSET_13+NAND_PART_SIZE_13)
    #define NAND_PART_OFFSET_15 (NAND_PART_OFFSET_14+NAND_PART_SIZE_14)
    #define NAND_PART_OFFSET_16 (NAND_PART_OFFSET_15+NAND_PART_SIZE_15)
    #define NAND_PART_OFFSET_17 (NAND_PART_OFFSET_16+NAND_PART_SIZE_16)
    #define NAND_PART_OFFSET_18 (NAND_PART_OFFSET_16+NAND_PART_SIZE_17)
    #define NAND_PART_OFFSET_19 (NAND_PART_OFFSET_16+NAND_PART_SIZE_18)
    #define NAND_PART_OFFSET_20 (NAND_PART_OFFSET_16+NAND_PART_SIZE_19)
    #define NAND_PART_OFFSET_21 (NAND_PART_OFFSET_16+NAND_PART_SIZE_20)

    #ifndef NAND_PART_ID_SYSIMG_A
        #define NAND_PART_ID_SYSIMG_A       (1)
    #endif
    #ifndef NAND_PART_ID_UP_A
        #define NAND_PART_ID_UP_A           (2)
    #endif
    #ifndef NAND_PART_ID_SYSIMG_B
        #define NAND_PART_ID_SYSIMG_B       (3)
    #endif
    #ifndef NAND_PART_ID_UP_B
        #define NAND_PART_ID_UP_B           (4)
    #endif
    #ifndef NAND_PART_ID_CH_A
        #define NAND_PART_ID_CH_A           (5)
    #endif
    #ifndef NAND_PART_ID_CH_B
        #define NAND_PART_ID_CH_B           (6)
    #endif

    #ifndef HDCP_NAND_FLASH_PART_ID
        #define HDCP_NAND_FLASH_PART_ID          0
    #endif

    #ifndef FLASH_DATA_NAND_FLASH_PART_ID
        #define FLASH_DATA_NAND_FLASH_PART_ID    0
    #endif

    #ifndef NAND_FLASH_STATIC_PART_SIZE
        #define NAND_FLASH_STATIC_PART_SIZE \
            (   (NAND_PART_SIZE_0) + \
                (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + \
                (NAND_PART_SIZE_3) + (NAND_PART_SIZE_4))
    #endif

    #ifndef NAND_FLASH_DYNAMIC_PART_SIZE
        #define NAND_FLASH_DYNAMIC_PART_SIZE ((NAND_FLASH_TOTAL_SIZE) - (NAND_FLASH_STATIC_PART_SIZE))
    #endif    

    #ifndef NAND_FLASH_DYNAMIC_PART_USED_SIZE
        #define NAND_FLASH_DYNAMIC_PART_USED_SIZE 0 //Used size should be smaller than dynamic total size (can not be equal!)
    #endif

    #ifndef NAND_FLASH_STATIC_LAST_PART_ID
        #define NAND_FLASH_STATIC_LAST_PART_ID  (4)
    #endif

    #ifndef NAND_FLASH_DYNAMIC_PART_ID
        #define NAND_FLASH_DYNAMIC_PART_ID      (NAND_FLASH_STATIC_LAST_PART_ID+1)
    #endif

    #ifndef NAND_PART_END
        #define NAND_PART_END                (22)// max is(NANDPART_PARTNUM)
    #endif

    // check the summation of each partition must larger than total static size.
    #if ((NAND_PART_SIZE_0  + \
          NAND_PART_SIZE_1  + \
          NAND_PART_SIZE_2  + \
          NAND_PART_SIZE_3  + \
          NAND_PART_SIZE_4  + \
          NAND_PART_SIZE_5  + \
          NAND_PART_SIZE_6  + \
          NAND_PART_SIZE_7  + \
          NAND_PART_SIZE_8  + \
          NAND_PART_SIZE_9  + \
          NAND_PART_SIZE_10 + \
          NAND_PART_SIZE_11 + \
          NAND_PART_SIZE_12 + \
          NAND_PART_SIZE_13 + \
          NAND_PART_SIZE_14 + \
          NAND_PART_SIZE_15 + \
          NAND_PART_SIZE_16 + \
          NAND_PART_SIZE_17 + \
          NAND_PART_SIZE_18 + \
          NAND_PART_SIZE_19 + \
          NAND_PART_SIZE_20 + \
          NAND_PART_SIZE_21)  \
            > (NAND_FLASH_TOTAL_SIZE))
    #error "Partition Size too small !!"
    #endif
#endif
#if defined(CC_NAND_LOADER)
    #ifndef LOADER_DUAL_BOOT_OFFSET
        #ifdef CC_DUAL_IMAGE_FLASH
            #define LOADER_DUAL_BOOT_OFFSET             (0x640000)
        #else /* CC_DUAL_BANL_FLASH */
            #define LOADER_DUAL_BOOT_OFFSET             (0)
        #endif /* CC_DUAL_BANL_FLASH */
    #endif
#endif


//----------------------------------------------------------------------------
// 8032uP Customization
//----------------------------------------------------------------------------
#ifndef T8032UP_OFFSET
#define T8032UP_OFFSET             (0)
#endif
#ifndef T8032UP_DUAL_OFFSET
#define T8032UP_DUAL_OFFSET        (0)
#endif
#ifndef T8032UP_SIZE
#define T8032UP_SIZE        (0)
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
#define VGA_AD_SPEC    1750 //175MHz
#define VGA_HFH_SPEC   1000 //100KHz
#define VGA_HFL_SPEC   130 //13KHz
#define VGA_VFH_SPEC   86 //86Hz
#define VGA_VFL_SPEC   49 //49Hz
#endif

//----------------------------------------------------------------------------
//SCART RGB Order Customerization
//----------------------------------------------------------------------------
#ifndef SCART_RGB_ORDER
#define SCART_RGB_ORDER (2)
#endif

//----------------------------------------------------------------------------
// SCART Pin8 Threshold Servo ADC Customization
//----------------------------------------------------------------------------
#ifndef SCART_PIN8_LOW_THRES
#define SCART_PIN8_LOW_THRES (11)
#endif

#ifndef SCART_PIN8_HI_THRES
#define SCART_PIN8_HI_THRES (32)
#endif
//----------------------------------------------------------------------------
// SCART FS Status Servo ADC Customization
//----------------------------------------------------------------------------
#ifndef SCART1_FS_SERVO_ADC
#define SCART1_FS_SERVO_ADC     0
#endif
//----------------------------------------------------------------------------
// SCART Fast Blanking In Select
//----------------------------------------------------------------------------
#ifndef SCART1_FB_IN_SELECT
#define SCART1_FB_IN_SELECT SCART_FB_IN_SOY0
#endif

#ifndef SCART2_FB_IN_SELECT
#define SCART2_FB_IN_SELECT SCART_FB_IN_NONE
#endif

//----------------------------------------------------------------------------
// Scart Setting
//----------------------------------------------------------------------------
#ifndef SUPPORT_SCART1_TYPE
#define SUPPORT_SCART1_TYPE SCART_FULL_TYPE
#endif

#ifndef SUPPORT_SCART2_TYPE
#define SUPPORT_SCART2_TYPE SCART_HALF_TYPE
#endif

//----------------------------------------------------------------------------
// Scart Setting
//----------------------------------------------------------------------------
#ifndef SCART1_INT_SC_CH
#define SCART1_INT_SC_CH SCART_SC_FROM_CH3
#endif

#ifndef SCART2_INT_SC_CH
#define SCART2_INT_SC_CH SCART_SC_FROM_EXT
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
#define IRRX_USR_SAPERIOD       0x001A
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
#ifndef FLAG_IR_POWERON_WAKEUP
#define FLAG_IR_POWERON_WAKEUP    (0)
#endif
#ifndef FLAG_IR_USER_DEFINE_WAKEUP
#define FLAG_IR_USER_DEFINE_WAKEUP    (0)
#endif
#ifndef IRRX_SET_USR_NOTIFY_FUNC
#define IRRX_SET_USR_NOTIFY_FUNC        (NULL)
#endif

#ifndef SET_SIGNAL_USR_NOTIFY_FUNC //singnal booster add by Hua 0609
#define SET_SIGNAL_USR_NOTIFY_FUNC       (NULL)
#endif


//----------------------------------------------------------------------------
// SIF customization
//----------------------------------------------------------------------------
#ifndef SIF_ENABLE_SCL_STRETCH
#define SIF_ENABLE_SCL_STRETCH      (0)
#endif
#ifndef SIF_DISABLE_OPEN_DRAIN
#define SIF_DISABLE_OPEN_DRAIN      (0)
#endif
#ifndef SIF_ENABLE_PDWNC_MASTER
#define SIF_ENABLE_PDWNC_MASTER      (0)
#endif

#ifndef SIF_ENABLE_8295B_MASTER0
#define SIF_ENABLE_8295B_MASTER0        (0)
#endif

#ifndef SIF_ENABLE_8295B_MASTER1
#define SIF_ENABLE_8295B_MASTER1        (0)
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
#define RELEASE_ENABLE_WATCHDOG     (1)
#endif

#ifndef RELEASE_WATCHDOG_TIMEOUT
#define RELEASE_WATCHDOG_TIMEOUT    (5)
#endif
#ifndef RELEASE_WATCHDOG_PRIORITY
#define RELEASE_WATCHDOG_PRIORITY   (250)//(100)
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
#define SCPOS_NON_LINEAR_END_POINT_RATIO   (1024)
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

#ifndef TVD_SUPPORT_PEDESTAL
#define TVD_SUPPORT_PEDESTAL (0)
#endif

#ifndef TVD_PAL_NTSC_MIX_AUTOSEARCH
#define TVD_PAL_NTSC_MIX_AUTOSEARCH (0)
#endif

#ifndef TVD_LLOCK_FREERUN
#define TVD_LLOCK_FREERUN (0)
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

#ifndef TVD_SUPPORT_OUTPUT_216
#define TVD_SUPPORT_OUTPUT_216     (0)
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
#define HDMI_BYPASS_INITIAL_FLOW (1)
#endif
#ifndef HDMI_OFFON_MUTE_COUNT
#define HDMI_OFFON_MUTE_COUNT (800)	//ms
#endif
#ifndef DVI_WAIT_STABLE_COUNT
#define DVI_WAIT_STABLE_COUNT (160)
#endif
#ifndef DVI_WAIT_NOSIGNAL_COUNT
#define DVI_WAIT_NOSIGNAL_COUNT (12)	//second
#endif
#ifndef HDMI_WAIT_SCDT_STABLE_COUNT
#define HDMI_WAIT_SCDT_STABLE_COUNT (1)	//second
#endif
#ifndef HDMI_TMDS_EQ_ZERO_VALUE  // josh
#define HDMI_TMDS_EQ_ZERO_VALUE (0x1)
#endif
#ifndef HDMI_TMDS_EQ_BOOST_VALUE  
#define HDMI_TMDS_EQ_BOOST_VALUE (0xd)
#endif
#ifndef HDMI_TMDS_EQ_SEL_VALUE
#define HDMI_TMDS_EQ_SEL_VALUE (0xd) //default
#endif
#ifndef HDMI_TMDS_EQ_GAIN_VALUE
#define HDMI_TMDS_EQ_GAIN_VALUE (0x2)
#endif
#ifndef HDMI_TMDS_HDMI_LBW_VALUE
#define HDMI_TMDS_HDMI_LBW_VALUE (0x2)
#endif

#ifndef HDMI_HDCP_Mask1
#define HDMI_HDCP_Mask1 (0xff)
#endif

#ifndef HDMI_HDCP_Mask2
#define HDMI_HDCP_Mask2 (0xc3)
#endif

//----------------------------------------------------------------------------
// RTC HW option flag
//----------------------------------------------------------------------------
#ifndef RTC_HW
#define RTC_HW        (RTC_INTERNAL)
#endif
#ifndef RTC_EXT_CLKDIV
#define RTC_EXT_CLKDIV 0x100
#endif

//----------------------------------------------------------------------------
// PQ Related Settings
//----------------------------------------------------------------------------
#ifdef SUPPORT_FLASH_PQ
        #ifndef FLASH_PQ_USE_NAND_FLASH
        	#ifdef CC_NAND_ENABLE 
        		#define FLASH_PQ_USE_NAND_FLASH  		(1)
        	#else
        		#define FLASH_PQ_USE_NAND_FLASH  		(0)
        	#endif
	#endif
	#ifndef FLASH_PQ_BASE_PARTITION
		#define FLASH_PQ_BASE_PARTITION                 (7) // Reference to NAND_PART_SIZExxx
    	#endif
    	
    	#ifndef FLASH_PQ_BASE_ADDRESS
        #ifdef CC_NAND_ENABLE
            #define FLASH_PQ_BASE_ADDRESS				    (0x0)
        #else
		#define FLASH_PQ_BASE_ADDRESS				    (0x283D0000)
	#endif
	#endif
	
	#ifndef FLASH_PQ_BASE_OFFSET
		#define FLASH_PQ_BASE_OFFSET				    (0)
	#endif
	#ifndef FLASH_PQ_BLOCK_SIZE
		#define FLASH_PQ_BLOCK_SIZE					    (1)	// Unit: 64KB.
	#endif
	#ifndef SUPPORT_FLASH_PQ_ENABLE_GAMMA
		#define SUPPORT_FLASH_PQ_ENABLE_GAMMA		    (1)
    #endif
	#ifndef SUPPORT_FLASH_PQ_GAMMA_X3
        #define SUPPORT_FLASH_PQ_GAMMA_X3               (1)
	#endif
	#ifndef SUPPORT_FLASH_PQ_ENABLE_SCE
		#define SUPPORT_FLASH_PQ_ENABLE_SCE			    (1)
	#endif
	#ifndef SUPPORT_FLASH_PQ_ENABLE_QTY
		#define SUPPORT_FLASH_PQ_ENABLE_QTY			    (1)
	#endif
	#ifndef SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN
		#define SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN	    (0)
	#endif
	#ifndef SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC
		#define SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC	(0)
	#endif
	#ifndef SUPPORT_LINK_M4_CONTRAST_SATURATION
		#define SUPPORT_LINK_M4_CONTRAST_SATURATION	    (0)
	#endif
    #ifndef CONTROL_BACKLIGHT_PWM
        #define CONTROL_BACKLIGHT_PWM               (1)
    #endif
#endif // #if SUPPORT_FLASH_PQ

#ifndef BIRGHT_INTERVAL
    #define BIRGHT_INTERVAL (2)  // Register Value = 0x400 + (Dummy - 0x80) << (BIRGHT_INTERVAL)
#endif

//----------------------------------------------------------------------------
// Default TVE
//----------------------------------------------------------------------------
#ifndef TVE_SUPPORT_AFD
#define TVE_SUPPORT_AFD (1)
#endif

//----------------------------------------------------------------------------
// Default UART
//----------------------------------------------------------------------------
#ifndef UART1_SUPPORT_ENABLE
#define UART1_SUPPORT_ENABLE    0
#endif

#ifndef UART2_SUPPORT_ENABLE
#define UART2_SUPPORT_ENABLE    0
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
#define MT8280_RESET_GPIO (0xFFFF)
#endif
#ifndef MT8280_RESET_GPIO_POLARITY
#define MT8280_RESET_GPIO_POLARITY (0)
#endif
#ifndef MT8280_CMD_ACK_GPIO 
#define MT8280_CMD_ACK_GPIO  (0xFFFFFFFF)
#endif
#ifndef MT8280_CMD_ACK_GPIO_POLARITY
#define MT8280_CMD_ACK_GPIO_POLARITY (1)
#endif
#ifndef MT8280_SEND_PANEL
#define MT8280_SEND_PANEL   (0)
#endif
//----------------------------------------------------------------------------
// MT8295 GPIO support
//----------------------------------------------------------------------------
// #ifndef CI_8295_RESET_GPIO
// #define CI_8295_RESET_GPIO  OPCTRL(5)
// #endif
#ifndef CI_PMX_CFG
#define CI_PMX_CFG      (mt5360_NAND_PMX)
#endif
#ifndef CI_5V_POWERON_POLARITY
#define CI_5V_POWERON_POLARITY      (1)
#endif
#ifndef CI_5V_POWERON_POLARITY
#define CI_5V_POWERON_POLARITY      (1)
#endif
#ifndef CI_8295_DRIVING_CURRENT
#define CI_8295_DRIVING_CURRENT      (4) // 4mA
#endif
#ifndef CI_TS_DRIVING_CURRENT
#define CI_TS_DRIVING_CURRENT        (6) // 6mA
#endif
#ifndef CI_PBI_DRIVING_CURRENT
#define CI_PBI_DRIVING_CURRENT       (6) // 6mA
#endif
#ifndef CI_CUSTOM_QUERY_FUNC
#define CI_CUSTOM_QUERY_FUNC  (NULL)
#endif

//----------------------------------------------------------------------------
// Smart card support
//----------------------------------------------------------------------------
#ifndef SMC_HOTPLUG_GPIO 
#define SMC_HOTPLUG_GPIO (121)
#endif
#ifndef SMC_HOTPLUG_GPIO_POLARITY
#define SMC_HOTPLUG_GPIO_POLARITY (0)
#endif

//-----------------------------------------------------------------------------
// Notification Function
//-----------------------------------------------------------------------------
#ifndef NOTIFY_MUTE_FUNC
#define NOTIFY_MUTE_FUNC (NULL)
#endif
#ifndef NOTIFY_MIXER_VIDEOLAYER_FUNC
#define NOTIFY_MIXER_VIDEOLAYER_FUNC (NULL)
#endif
#ifndef NOTIFY_MIXER_FUNC
#define NOTIFY_MIXER_FUNC (NULL)
#endif
#ifndef NOTIFY_VIDEO_FUNC
#define NOTIFY_VIDEO_FUNC (NULL)
#endif
#ifndef NOTIFY_VIDEO_AUTO_DONE_FUNC
#define NOTIFY_VIDEO_AUTO_DONE_FUNC (NULL)
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

#ifndef VFE_HW_SONY
#define VFE_HW_SONY  0   // 1 : SONY HW is 0,75 ohm
#endif

#ifndef VFE_HW_SONY_YPbPrMapToVGA
#define VFE_HW_SONY_YPbPrMapToVGA  0   // 1 : SONY YPbPr map to VGA
#endif

#ifndef VFE_HW_075
#define VFE_HW_075  0   // 1 : HW is 0,75 ohm;  0:H/W is 18,56 ohm. mt5362 default is 18,56 ohm
#endif

#ifndef YPBPR_ADC_SUPPORT_120
#define YPBPR_ADC_SUPPORT_120  0   // 1 : support none standard 120IRE;  0: not support 120%
#endif

#ifndef DoAutoPhaseAfterAutoColor
#define DoAutoPhaseAfterAutoColor  0   // 1 : support none standard 120IRE;  0: not support 120%
#endif

#ifndef SCART_DO_AUTOCOLOR             //MC20081115 add for SCART ***
#define SCART_DO_AUTOCOLOR 0
#endif                                                   //MC20081115 add for SCART &&&

#ifndef EFUSE_AUTOCOLOR
#define EFUSE_AUTOCOLOR 0  // 1 :Use E Fuse Autocolor
#endif
//----------------------------------------------------------------------------
// Linux memory usage related customization
//------------------------------------------------------------------------------
#ifndef LINUX_KERNEL_MEM_SIZE 
#define LINUX_KERNEL_MEM_SIZE 54 * 1024 * 1024
#endif
#ifndef TOTAL_MEM_SIZE
#define TOTAL_MEM_SIZE 128 * 1024 * 1024
#endif
#ifndef FB_MEM_SIZE
#define FB_MEM_SIZE 4 * 1024 * 1024
#endif
#ifndef DIRECT_FB_MEM_SIZE
#define DIRECT_FB_MEM_SIZE 8 * 1024 * 1024
#endif

//#define YPBPR_COUNTER_ADDR    0   // define the EEPROM absolute address for the counter of YPBPR online offset calibration
//#define VGA_COUNTER_ADDR      0   // define the EEPROM absolute address for the counter of VGA online offset calibration
//#define OFFSET_CALI_COUNTER   0xff  //0xff means ignore the above two counter


//----------------------------------------------------------------------------
// Tool License Check and Password Protection
//------------------------------------------------------------------------------
#ifndef CUSTOMER_NAME
#define CUSTOMER_NAME		"mtk"
#endif

#ifndef CUSTOMER_PASSWORD
#define CUSTOMER_PASSWORD	""
#endif

