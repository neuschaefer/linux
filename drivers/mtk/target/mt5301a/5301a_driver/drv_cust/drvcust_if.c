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

//----------------------------------------------------------------------------
// Include general files
//----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER

// driver general header file
#include "x_typedef.h"
#include "x_assert.h"
#include "x_hal_5381.h"

// driver customized header file
#include "drvcust_if.h"
#include "panel.h"

//----------------------------------------------------------------------------
// Condition include files
//----------------------------------------------------------------------------

#ifdef CUSTOM_CFG_FILE
#include CUSTOM_CFG_FILE
#endif /* CUSTOM_CFG_FILE */

#ifdef NOTIFY_INCLUDE_FILE
#include NOTIFY_INCLUDE_FILE
#endif

#ifdef CI_EEP_OPTIONCODE_INCLUDE_FILE
#include CI_EEP_OPTIONCODE_INCLUDE_FILE
#endif

#include "drv_def_file.h"

//----------------------------------------------------------------------------
// Customization include files
//----------------------------------------------------------------------------

#ifndef NO_INCLUDE_FILES

#ifdef OPCODE_VALUE_QUERY_FILE
#include OPCODE_VALUE_QUERY_FILE
#endif

#ifdef RTC_EXT_IMPL
#include RTC_EXT_IMPL
#endif

#endif /* NO_INCLUDE_FILES */

#include "drv_default.h"

#ifdef AUDIO_INCLUDE_FILE
#include AUDIO_INCLUDE_FILE
#include "mtk/mtk_aud.h"
#endif

#ifdef NONLINEAR_INCLUDE_FILE
#include NONLINEAR_INCLUDE_FILE
#endif

#ifdef POWER_SEQUENCE_EXT_FILE
#include POWER_SEQUENCE_EXT_FILE
#endif

#ifdef DRV_LOGO_H_FILE
#include DRV_LOGO_H_FILE
#endif

#ifdef DRV_POWER_MUSIC_H_FILE
#include DRV_POWER_MUSIC_H_FILE
#endif /* DRV_POWER_MUSIC_H_FILE */

//----------------------------------------------------------------------------
// External variable
//----------------------------------------------------------------------------

#define DRAM_SIZE_64MB (64*1024*1024)

//----------------------------------------------------------------------------
// External variable
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** DRVCUST_OptQuery() return customization paramter.
 */
//----------------------------------------------------------------------------
INT32 DRVCUST_OptQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data)
{
    ASSERT(pu4Data!=NULL);

    if (DRVCUST_InitQuery(eQryType, pu4Data)==0)
    {
        return 0;
    }

    if (DRVCUST_PanelQuery(eQryType, pu4Data)==0)
    {
        return 0;
    }

    switch(eQryType)
    {
    // Tuner control parameter.
    case eTunerCtrl:
        *pu4Data = (UINT32) DEFAULT_TUNER_CTRL;
        return 0;
    case eTunerTypeDefault:
        *pu4Data = (UINT32) DEFAULT_TUNER_TYPE_DEFAULT;
        return 0;
    case eTunerTypeOfst0:
        *pu4Data = (UINT32) DEFAULT_TUNER_TYPE_OFST0;
        return 0;
    case eTunerTypeOfst1:
        *pu4Data = (UINT32) DEFAULT_TUNER_TYPE_OFST1;
        return 0;
    case eTunerTypeOfst2:
        *pu4Data = (UINT32) DEFAULT_TUNER_TYPE_OFST2;
        return 0;
    case eTunerCtrlAna:
        *pu4Data = (UINT32) DEFAULT_TUNER_CTRL1;
        return 0;
	case eTunerCtrlLna:
        *pu4Data = (UINT32) DEFAULT_TUNER_CTRL_LNA;
        return 0;	
    case eDemodResetGpio:
#ifdef DEFAULT_DEMOD_RESET_GPIO        
        *pu4Data = (UINT32) DEFAULT_DEMOD_RESET_GPIO;
        return 0;
#else
        return -1;
#endif
#ifdef RF_TUNER_B2PIN_GPIO
    case eRfTunerPinGpio:
        *pu4Data = (UINT32) RF_TUNER_B2PIN_GPIO;
        return 0;
#endif
    // Release version setting
    case eReleaseEnableWatchDog:
        *pu4Data = RELEASE_ENABLE_WATCHDOG;
        return 0;
    case eReleaseWatchdogTimeout:
        *pu4Data = RELEASE_WATCHDOG_TIMEOUT;
        return 0;
    case eReleaseWatchdogPriority:
        *pu4Data = RELEASE_WATCHDOG_PRIORITY;
        return 0;

    // Memory setting
    case eFbmMemAddr:
        *pu4Data = BSP_GetFbmMemAddr();
        return 0;

    // Audio
    case eAudioOutPadNormal:
        *pu4Data = AUDIO_OUT_PAD_NORMAL;
        return 0;
    case eAudioOutPadDrvCur:
        *pu4Data = AUDIO_OUT_PAD_DRV_CUR;
        return 0;
    case eAudioBassMngCutOffFreq:
        *pu4Data = AUDIO_BASS_MNG_CUTOFF_FREQ;
        return 0;
    case eAudioSoftMuteOrder:
        *pu4Data = AUDIO_SOFT_MUTE_ORDER;
        return 0;
    case eAudioBbeLevel:
        *pu4Data = AUDIO_BBE_LEVEL;
        return 0;
    case eAudioBbeProcess:
        *pu4Data = AUDIO_BBE_PROCESS;
        return 0;
    case eAudioBbeLoContour:
        *pu4Data = AUDIO_BBE_LO_CONTOUR;
        return 0;
    case eAudioBbe3dGain:
        *pu4Data = AUDIO_BBE_3D_GAIN;
        return 0;
    case eAudioSpkType:
        *pu4Data = AUDIO_SPK_TYPE_LARGE;
        return 0;
    case eAudioSurroundSpkType:
        *pu4Data = AUDIO_SURROUND_SPK_TYPE_LARGE;
        return 0;
    case eAudioCenterSpkType:
        *pu4Data = AUDIO_CENTER_SPK_TYPE_LARGE;
        return 0;
    case eAudioCh910SpkType:
        *pu4Data = AUDIO_CH9_10_SPK_TYPE_LARGE;
        return 0;
    case eAudioSubwooferOn:
        *pu4Data = AUDIO_SUBWOOFER_ON;
        return 0;
    case eAudioBypassPostProc:
        *pu4Data = AUDIO_BYPASS_POST_PROC;
        return 0;

    case eAudioDacMclk:
        *pu4Data = AUDIO_DAC_MCLK;
        return 0;
    case eAudioDacUsage:
        *pu4Data = (UINT32)AUDIO_ADAC_USAGE;
        return 0;
    case eAudioAoutPadConfig:
        *pu4Data = (UINT32)AUDIO_AOUT_PAD_CFG;
        return 0;
    case eAudioInterDacChlConfig:
        *pu4Data = (UINT32)AUDIO_INTER_DAC_USAGE;
        return 0;
#ifdef AUDIO_INTER_DAC_SPECIAL_USAGE
    case eAudioInterDacChlSpecialConfig:
        *pu4Data = (UINT32)AUDIO_INTER_DAC_SPECIAL_USAGE;
        return 0;
#endif /* AUDIO_INTER_DAC_SPECIAL_USAGE */
    case eAudioOpctrlMuteGpioNum:
        *pu4Data = AUDIO_Opctrl_Mute_Gpio_Num;
        return 0;
    case eAudioOpctrlMuteGpioPolarity:
        *pu4Data = AUDIO_OPCTRL_MUTE_GPIO_POLARITY;
        return 0;
    case eAudioMuteGpioNum:
        *pu4Data = AUDIO_Mute_Gpio_Num;
        return 0;
    case eAudioAmpEnableGpioNum:
        *pu4Data = AUDIO_Amp_Gpio_Num;
        return 0;
    case eAudioMuteGpioPolarity:
        *pu4Data = AUDIO_MUTE_GPIO_POLARITY;
        return 0;
    case eAudioAmpGpioPolarity:
        *pu4Data = AUDIO_AMP_GPIO_POLARITY;
        return 0;
    case eAudioAmpResetGpioNum:
        *pu4Data = AUDIO_AMP_RESET_GPIO_NUM;
        return 0;
    case eAudioAmpResetGpioPolarity:
        *pu4Data = AUDIO_AMP_RESET_GPIO_POLARITY;
        return 0;
    case eAudioInputSwGpioSel:
        *pu4Data = (UINT32)AUDIO_INPUT_SW_GPIO_SEL;
        return 0;
    case eAudioInputMuxSelTable:
        *pu4Data = (UINT32)AUDIO_INPUT_MUX_SEL_TABLE;
        return 0;
#ifdef AUDIO_SPECIAL_INPUT_MUX_SEL_TABLE
    case eAudioSpecialInputMuxSelTable:
        *pu4Data = (UINT32)AUDIO_SPECIAL_INPUT_MUX_SEL_TABLE;
        return 0;
#endif /* AUDIO_SPECIAL_INPUT_MUX_SEL_TABLE */
    case eAudioOutFmtTable:
        *pu4Data = (UINT32)AUDIO_OUT_FMT_TABLE;
        return 0;
    case eAudioOutFmtTable2:
        *pu4Data = (UINT32)AUDIO_OUT_FMT_TABLE2;
        return 0;
    case eAudioInFmtTable:
        *pu4Data = (UINT32)AUDIO_IN_FMT_TABLE;
        return 0;
    case eAudioInputVolTable:
        *pu4Data = (UINT32)AUDIO_INPUT_SRC_VOL;
        return 0;
    case eAudioAvInputSrcVolTable:
        *pu4Data = (UINT32)AUDIO_AV_INPUT_SRC_VOL;
        return 0;
    case eAudioAvChlEqTable:
        *pu4Data = (UINT32)AUDIO_CHL_EQ_CFG;
        return 0;
    case eAudioInputSrcAgcFlag:
        *pu4Data = (UINT32)AUDIO_INPUT_SRC_AGC_FLAG;
        return 0;
    case eAudioInputSrcAgcVol:
        *pu4Data = (UINT32)AUDIO_INPUT_SRC_AGC_VOL;
        return 0;
    case eAudioVolBaseLevel:
        *pu4Data = (UINT32)AUDIO_VOL_BASE_LEVEL;
        return 0;
    case eAudioVolTable:
        *pu4Data = (UINT32)AUDIO_VOL_TABLE;
        return 0;
    case eAudioAvcTarget:
        *pu4Data = (UINT32)AUDIO_AVC_TARGET_LEV;
        return 0;
    case eAudioAvcGainUp:
        *pu4Data = (UINT32)AUDIO_AVC_GAIN_UP;
        return 0;
    case eAudioAvcAttackThres:
        *pu4Data = (UINT32)AUDIO_AVC_ATTACK_THRES;
        return 0;
    case eAudioAvcAdjustRate:
        *pu4Data = (UINT32)AUDIO_AVC_ADJUST_RATE;
        return 0;
    case eAudioChlVolExtraGain:
        *pu4Data = (UINT32)AUDIO_CHL_VOL_EXTRA_GAIN;
        return 0;
    case eAudioChlVolExtraGain1:
        *pu4Data = (UINT32)AUDIO_CHL_VOL_EXTRA_GAIN1;
        return 0;
    case eAudioChannelDelay:
        *pu4Data = (UINT32)AUDIO_CHANNEL_DELAY;
        return 0;
    case eAudioIecCopyright:
        *pu4Data = (UINT32)AUDIO_IEC_COPYRIGHT;
        return 0;
    case eAudioIecCategory:
        *pu4Data = (UINT32)AUDIO_IEC_CATEGORY;
        return 0;
    case eAudioIecWordLength:
        *pu4Data = (UINT32)AUDIO_IEC_WORD_LENGTH;
        return 0;
    case eAudioHeadphonePlugInGpio:
        *pu4Data = (UINT32)AUDIO_HP_PLUG_IN_GPIO;
        return 0;
    case eAudioHeadPhonePlugInPolarity:
        *pu4Data = (UINT32)AUDIO_HP_PLUG_IN_POLARITY;
        return 0;
    case eAudioHeadPhonePlugInLineOutMute:
        *pu4Data = (UINT32)AUDIO_HP_PLUG_IN_LINEOUT_MUTE;
        return 0;
    case eAudioDcDetectGpio:
        *pu4Data = (UINT32)AUDIO_DC_DETECT_GPIO;
        return 0;
    case eAudioDcDetectPolarity:
        *pu4Data = (UINT32)AUDIO_DC_DETECT_POLARITY;
        return 0;
    case eAudioLineOutUseInterDac:
        *pu4Data = (UINT32)AUDIO_LINEOUT_DAC_USAGE;
        return 0;
    case eAudioMuteAllSource:
        *pu4Data = (UINT32)AUDIO_MUTE_ALL_SOURCE;
        return 0;
    case eAudioDvdGpio:
        *pu4Data = (UINT32)AUDIO_DVD_GPIO;
        return 0;
    case eAudioDvdPolarity:
        *pu4Data = (UINT32)AUDIO_DVD_POLARITY;
        return 0;
    case eAudioDvdInputChl:
        *pu4Data = (UINT32)AUDIO_DVD_CHANNEL;
        return 0;
        case eAudioSpkAutoOffWhenVolMute:
        *pu4Data = AUDIO_SPK_AUTO_OFF_WHEN_VOL_MUTE;
        return 0;
    case eAudioCustSpecGpioCtrl:
        *pu4Data = (UINT32)AUDIO_CUST_SPEC_GPIO_CTRL;
        return 0;
    case eAudioCustSpec2GpioCtrl:
        *pu4Data = (UINT32)AUDIO_CUST_SPEC2_GPIO_CTRL;
        return 0;
    case eAudioSpecGpioPolarity:
    	 *pu4Data = (UINT32)AUDIO_CUST_SPEC_POLARITY;
        return 0;
    case eAudioSpec2GpioPolarity:
    	 *pu4Data = (UINT32)AUDIO_CUST_SPEC2_POLARITY;
        return 0;
    case eAudioCustHpGpioCtrl:
        *pu4Data = (UINT32)AUDIO_CUST_HP_GPIO_CTRL;
        return 0;
    case eAudioHpGpioPolarity:
    	 *pu4Data = (UINT32)AUDIO_CUST_HP_POLARITY;
        return 0;
    case eAudioAL1AR1PhaseInverse:
        *pu4Data = (UINT32)AUDIO_AL1AR1_PHASE_INVERSE;
        return 0;
    case eAudioAL2AR2PhaseInverse:
        *pu4Data = (UINT32)AUDIO_AL2AR2_PHASE_INVERSE;
        return 0;
    case eAudioAL3AR3PhaseInverse:
        *pu4Data = (UINT32)AUDIO_AL3AR3_PHASE_INVERSE;
        return 0;
    case eAudioAL4AR4PhaseInverse:
         *pu4Data = (UINT32)AUDIO_AL4AR4_PHASE_INVERSE;
         return 0;
    case eAudioSpkPath:
        *pu4Data = AUDIO_SPK_CODEC_PATH;
        return 0;
    case eAudioMT8292PGA0GAIN:
        *pu4Data = AUDIO_MT8292_PGA0_GAIN;
        return 0;
    case eAudioMT8292PGA1GAIN:
        *pu4Data = AUDIO_MT8292_PGA1_GAIN;
        return 0;
    case eAudioMT8292PGA2GAIN:
        *pu4Data = AUDIO_MT8292_PGA2_GAIN;
        return 0;
    case eAudioMT8292HPGAIN:
        *pu4Data = AUDIO_MT8292_HP_GAIN;
        return 0;
    case eAudioLineInLrSwap:
        *pu4Data = AUDIO_LINE_IN_LR_SWAP;
        return 0;
    case eAudioMtsFactory:
        *pu4Data = (UINT32)AUDIO_MTS_FACTORY_MODE;
        return 0;
    case eAudioA2Factory:
        *pu4Data = (UINT32)AUDIO_A2_FACTORY_MODE;
        return 0;
    case eAudioPalFactory:
        *pu4Data = (UINT32)AUDIO_PAL_FACTORY_MODE;
        return 0;
    case eAudioLimiterMode:
        *pu4Data = AUDIO_LIMITER_MODE;
        return 0;
    case eAudioLimiterFixThreshold:
        *pu4Data = AUDIO_LIMITER_FIX_THRESHOLD;
        return 0;
    case eAudioNvmEnable:
        *pu4Data = (UINT32)AUDIO_NVM_ENABLE;
        return 0;
    case eAudioNvmLength:
        *pu4Data = (UINT32)AUDIO_NVM_LENGTH;
        return 0;
    case eAudioNvmData:
        *pu4Data = (UINT32)AUDIO_NVM_DATA;
        return 0;
    case eAudioFlashCommLength:
        *pu4Data = (UINT32)AUDIO_FLASH_COMM_LENGTH;
        return 0;
    case eAudioFlashCommData:
        *pu4Data = (UINT32)AUDIO_FLASH_COMM_DATA;
        return 0;
#ifdef AUDIO_NVM_GET_COUNTRY
    case eAudioNvmGetCountry:
        *pu4Data = (UINT32)AUDIO_NVM_GET_COUNTRY;
        return 0;
#endif /* AUDIO_NVM_GET_COUNTRY */
    case eAudioProfileDefault:
        *pu4Data = (UINT32)AUDIO_PROFILE_DEFAULT;
        return 0;
    case eAudioProfileStart:
        *pu4Data = (UINT32)AUDIO_PROFILE_START;
        return 0;
    case eAudioProfileEnd:
        *pu4Data = (UINT32)AUDIO_PROFILE_END;
        return 0;
    case eAudioDigitalDataConfig:
    	 *pu4Data = (UINT32)AUDIO_Digital_Data_TABLE;
        return 0;
    case eAudioTunerATD:
    	 *pu4Data = (UINT32)AUDIO_TUNER_ATD;
        return 0;
    case eAudioDigiAMPResetGpio:
        *pu4Data = AUDIO_DIGITAL_AMP_RESET_GPIO;
        return 0;
    case eAudioDigiAMPResetGpioPolarity:
        *pu4Data = AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY;
        return 0;
    case eAudioADFlag:
        *pu4Data = AUDIO_AUDIO_DESCRIPTION_FLAG;
        return 0;
    case eAudioDownmixPosition:
        *pu4Data = AUDIO_DOWNMIX_CHANNEL_POSITION;
        return 0;
    case eAudioDownmixLR:
        *pu4Data = AUDIO_DOWNMIX_LR;
        return 0;
    case eAudioMultiPairOutput:
        *pu4Data = AUDIO_MULTI_PAIR_OUTPUT;
        return 0;
    case eAudioBI2SfromGpio25:
        *pu4Data = AUDIO_BI2S_FROM_GPIO_2_5;
        return 0;
    case eAudioBI2SfromAI2S:
        *pu4Data = AUDIO_BI2S_FROM_AI2S;
        return 0;
    case eAudioPcmPrebufBankNum:
        *pu4Data = AUDIO_PCM_PREBUF_BANK_NUM;
        return 0;
    case eAudioAI2SuseforSpeaker:
	    *pu4Data = AUDIO_AI2S_USE_FOR_SPEAKER;
	    return 0;
	case eAudioMultichlDDCO:
	    *pu4Data = AUDIO_HEAAC_MULTICHL_DDCO;
	    return 0;
	case eAudioLoaderMusicData:
#ifndef DRV_LOGO_MUSIC_TABLE
		*pu4Data = (UINT32)0;
		return -1;
#else
		*pu4Data = (UINT32)DRV_LOGO_MUSIC_TABLE;
		return 0;
#endif
	case eAudioCustomerMusicTable:
#ifndef DRV_CUSTOMER_MUSIC_TABLE
		*pu4Data = (UINT32)0;
		return -1;
#else
		*pu4Data = (UINT32)DRV_CUSTOMER_MUSIC_TABLE;
		return 0;
#endif
    case eAudioAmpFadeOutTime:
        *pu4Data = AUDIO_AMP_FADE_OUT_TIME;
        return 0;
#if 0
    case eAudioVsurrSetting:
        *pu4Data = (UINT32)AUDIO_VSURR_SETTING;
        return 0;
#endif
    case eAudioBypassChVol:
        *pu4Data = (UINT32)AUDIO_BYPASS_CH_VOL;
        return 0;
    case eAudioDmxChVol:
        *pu4Data = (UINT32)AUDIO_DMX_CH_VOL;
        return 0;
    case eAudioTvScartChVol:
        *pu4Data = (UINT32)AUDIO_TVSCART_CH_VOL;
        return 0;
    case eAudioFlashAqBase:
        *pu4Data = FLASH_AQ_BASE_OFFSET;
        return 0;          
    case eAudioDataFlashPartId:
        *pu4Data = FLASH_AQ_PART_ID;
        return 0;                  

    // EDID EEPROM
#ifdef EDID_EEPROM_WP_GPIO
    case eEdidEepromWPGpio:
        *pu4Data = EDID_EEPROM_WP_GPIO;
        return 0;
#endif /* EDID_EEPROM_WP_GPIO */
#ifdef EDID_EEPROM_WP_ENABLE_POLARITY
    case eEdidEepromWPEnablePolarity:
        *pu4Data = EDID_EEPROM_WP_ENABLE_POLARITY;
        return 0;
#endif /* EDID_EEPROM_WP_ENABLE_POLARITY */

#if 0
    // SYSTEM EERPOM
    case eSystemEepromSize:
        *pu4Data = SYSTEM_EEPROM_SIZE;
        return 0;
    case eSysEepromWPGpio:
        *pu4Data = SYSTEM_EEPROM_WP_GPIO;
        return 0;
    case eSysEepromWPEnablePolarity:
        *pu4Data = SYSTEM_EEPROM_WP_ENABLE_POLARITY;
        return 0;
    case eSystemEepromClkDiv:
        *pu4Data = SYSTEM_EEPROM_CLKDIV;
        return 0;
    case eSystemEepromBus:
        *pu4Data = (UINT32) SYSTEM_EEPROM_BUS;
        return 0;
    case eSystemEepromAddress:
        *pu4Data = (UINT32) SYSTEM_EEPROM_ADDRESS;
        return 0;

    // HDCP EERPOM
    case eHdcpEepromSize:
        *pu4Data = HDCP_EEPROM_SIZE;
        return 0;
    case eHdcpEepromWPGpio:
        *pu4Data = HDCP_EEPROM_WP_GPIO;
        return 0;
    case eHdcpEepromWPEnablePolarity:
        *pu4Data = HDCP_EEPROM_WP_ENABLE_POLARITY;
        return 0;
    case eHdcpEepromClkDiv:
        *pu4Data = HDCP_EEPROM_CLKDIV;
        return 0;
    case eHdcpEepromBus:
        *pu4Data = (UINT32) HDCP_EEPROM_BUS;
        return 0;
    case eHdcpEepromAddress:
        *pu4Data = (UINT32) HDCP_EEPROM_ADDRESS;
        return 0;
    case eHdcpEepromOffset:
        *pu4Data = (UINT32) HDCP_EEPROM_OFFSET;
        return 0;
#endif

    // NOR flash parameter.
    case eNorFlashTotalSize:
        *pu4Data = (UINT32)NOR_FLASH_TOTAL_SIZE;
        return 0;
    case eNorFlashAppImageSize:
        *pu4Data = (UINT32)NOR_FLASH_APPIMAGE_SIZE;
        return 0;
    case eNorFlashDynamicPartitionSize:
        *pu4Data = (UINT32)NOR_FLASH_DYNAMIC_PART_SIZE;
        return 0;
    case eNorFlashStaticPartitionSize:
        *pu4Data = (UINT32)NOR_FLASH_STATIC_PART_SIZE;
        return 0;
    case eHDCPNorPartId:
        *pu4Data = (UINT32)HDCP_NOR_FLASH_PART_ID;
        return 0;

    // NOR flash partition size customization
    case eNorFlashPartSize0:
        *pu4Data = (UINT32)NOR_PART_SIZE_0;
        return 0;
    case eNorFlashPartSize1:
        *pu4Data = (UINT32)NOR_PART_SIZE_1;
        return 0;
    case eNorFlashPartSize2:
        *pu4Data = (UINT32)NOR_PART_SIZE_2;
        return 0;
    case eNorFlashPartSize3:
        *pu4Data = (UINT32)NOR_PART_SIZE_3;
        return 0;
    case eNorFlashPartSize4:
        *pu4Data = (UINT32)NOR_PART_SIZE_4;
        return 0;
    case eNorFlashPartSize5:
        *pu4Data = (UINT32)NOR_PART_SIZE_5;
        return 0;
    case eNorFlashPartSize6:
        *pu4Data = (UINT32)NOR_PART_SIZE_6;
        return 0;
    case eNorFlashPartSize7:
        *pu4Data = (UINT32)NOR_PART_SIZE_7;
        return 0;
    case eNorFlashPartSize8:
        *pu4Data = (UINT32)NOR_PART_SIZE_8;
        return 0;
    case eNorFlashPartSize9:
        *pu4Data = (UINT32)NOR_PART_SIZE_9;
        return 0;
    case eNorFlashPartSize10:
        *pu4Data = (UINT32)NOR_PART_SIZE_10;
        return 0;
    case eNorFlashPartSize11:
        *pu4Data = (UINT32)NOR_PART_SIZE_11;
        return 0;
    case eNorFlashPartSize12:
        *pu4Data = (UINT32)NOR_PART_SIZE_12;
        return 0;
    case eNorFlashPartSize13:
        *pu4Data = (UINT32)NOR_PART_SIZE_13;
        return 0;
    case eNorFlashPartSize14:
        *pu4Data = (UINT32)NOR_PART_SIZE_14;
        return 0;
    case eNorFlashPartSize15:
        *pu4Data = (UINT32)NOR_PART_SIZE_15;
        return 0;
    case eNorFlashPartSize16:
        *pu4Data = (UINT32)NOR_PART_SIZE_16;
        return 0;
    case eNorFlashPartSize17:
        *pu4Data = (UINT32)NOR_PART_SIZE_17;
        return 0;
    case eNorFlashPartSize18:
        *pu4Data = (UINT32)NOR_PART_SIZE_18;
        return 0;
    case eNorFlashPartSize19:
        *pu4Data = (UINT32)NOR_PART_SIZE_19;
        return 0;
    case eNorFlashPartSize20:
        *pu4Data = (UINT32)NOR_PART_SIZE_20;
        return 0;    
    case eNorFlashPartSize21:
        *pu4Data = (UINT32)NOR_PART_SIZE_21;
        return 0;
        


    // NAND flash parameter.
#if defined(CC_NAND_ENABLE)
    case eNANDFlashTotalSize:
        *pu4Data = (UINT32)NAND_FLASH_TOTAL_SIZE;
        return 0;
    case eNANDFlashAppImageSize:
        *pu4Data = (UINT32)NAND_FLASH_APPIMAGE_SIZE;
        return 0;
    case eNANDFlashDynamicPartitionSize:
        *pu4Data = (UINT32)NAND_FLASH_DYNAMIC_PART_SIZE;
        return 0;
    case eNANDFlashStaticPartitionSize:
        *pu4Data = (UINT32)NAND_FLASH_STATIC_PART_SIZE;
        return 0;
    case eNANDFlashDynamicPartitionUsedSize:
        *pu4Data = (UINT32)NAND_FLASH_DYNAMIC_PART_USED_SIZE;
        return 0;
    case eHDCPNANDPartId:
        *pu4Data = (UINT32)HDCP_NAND_FLASH_PART_ID;
        return 0;
    // NAND flash partition size customization
    case eNANDFlashPartSize0:
        *pu4Data = (UINT32)NAND_PART_SIZE_0;
        return 0;
    case eNANDFlashPartSize1:
        *pu4Data = (UINT32)NAND_PART_SIZE_1;
        return 0;
    case eNANDFlashPartSize2:
        *pu4Data = (UINT32)NAND_PART_SIZE_2;
        return 0;
    case eNANDFlashPartSize3:
        *pu4Data = (UINT32)NAND_PART_SIZE_3;
        return 0;
    case eNANDFlashPartSize4:
        *pu4Data = (UINT32)NAND_PART_SIZE_4;
        return 0;
    case eNANDFlashPartSize5:
        *pu4Data = (UINT32)NAND_PART_SIZE_5;
        return 0;
    case eNANDFlashPartSize6:
        *pu4Data = (UINT32)NAND_PART_SIZE_6;
        return 0;
    case eNANDFlashPartSize7:
        *pu4Data = (UINT32)NAND_PART_SIZE_7;
        return 0;
    case eNANDFlashPartSize8:
        *pu4Data = (UINT32)NAND_PART_SIZE_8;
        return 0;
    case eNANDFlashPartSize9:
        *pu4Data = (UINT32)NAND_PART_SIZE_9;
        return 0;
    case eNANDFlashPartSize10:
        *pu4Data = (UINT32)NAND_PART_SIZE_10;
        return 0;
    case eNANDFlashPartSize11:
        *pu4Data = (UINT32)NAND_PART_SIZE_11;
        return 0;
    case eNANDFlashPartSize12:
        *pu4Data = (UINT32)NAND_PART_SIZE_12;
        return 0;
    case eNANDFlashPartSize13:
        *pu4Data = (UINT32)NAND_PART_SIZE_13;
        return 0;
    case eNANDFlashPartSize14:
        *pu4Data = (UINT32)NAND_PART_SIZE_14;
        return 0;
    case eNANDFlashPartSize15:
        *pu4Data = (UINT32)NAND_PART_SIZE_15;
        return 0;
    case eNANDFlashPartSize16:
        *pu4Data = (UINT32)NAND_PART_SIZE_16;
        return 0;
    case eNANDFlashPartSize17:
        *pu4Data = (UINT32)NAND_PART_SIZE_17;
        return 0;
    case eNANDFlashPartSize18:
        *pu4Data = (UINT32)NAND_PART_SIZE_18;
        return 0;
    case eNANDFlashPartSize19:
        *pu4Data = (UINT32)NAND_PART_SIZE_19;
        return 0;
    case eNANDFlashPartSize20:
        *pu4Data = (UINT32)NAND_PART_SIZE_20;
        return 0;
    case eNANDFlashPartSize21:
        *pu4Data = (UINT32)NAND_PART_SIZE_21;
        return 0;
    case eNANDFlashPartEnd:
        *pu4Data = (UINT32)NAND_PART_END;
        return 0;
#endif
#ifdef FCI_CARD_DETECT_GPIO
    case eFCICardDetectGpio:
        *pu4Data = (UINT32)FCI_CARD_DETECT_GPIO;
        return 0;
#endif

    // Non-linear scaling setting
    case eFlagScposNonLinearSlope:
    *pu4Data = (UINT32)SCPOS_NON_LINEAR_SLOPE;
        return 0;
    case eFlagScposNonLinearEndPointRatio:
    *pu4Data = (UINT32)SCPOS_NON_LINEAR_END_POINT_RATIO;
        return 0;

    case eVGAMaxRateForDispmode:
    *pu4Data = (UINT32)VGA_MAX_SUPPORT_PIXEL_RATE;
        return 0;

    // TVd
    case eTVDSupportNTSCOnly:
        *pu4Data = (UINT32)TVD_ONLY_SUPPORT_NTSC;
        return 0;
    case eTVDSupportPedEn:
        *pu4Data = (UINT32)TVD_SUPPORT_PEDESTAL;
        return 0;

    case eTVDPalNtscMixAutosearch:
        *pu4Data = (UINT32)TVD_PAL_NTSC_MIX_AUTOSEARCH;
        return 0;

    case eTVDLLockFreeRun:
        *pu4Data = (UINT32)TVD_LLOCK_FREERUN;
        return 0;

    case eTVDCCNRLThresholdBlock:
        *pu4Data = (UINT32) TVD_CC_NRL_THRESHOLD_BLOCK;
        return 0;

    case eTVDCCNRLThresholdUnblock:
        *pu4Data = (UINT32) TVD_CC_NRL_THRESHOLD_UNBLOCK;
        return 0;

    case eTVDCCPERThresholdBlock:
        *pu4Data = (UINT32) TVD_CC_PER_THRESHOLD_BLOCK;
        return 0;

    case eTVDCCPERThresholdUnblock:
        *pu4Data = (UINT32) TVD_CC_PER_THRESHOLD_UNBLOCK;
        return 0;
    case eTVDSupportOutput216:
        *pu4Data = (UINT32) TVD_SUPPORT_OUTPUT_216;
        return 0;

   //CEC
     case eCECFunctionPinmuxEn:
    *pu4Data = (UINT32)CEC_FUNCTON_PINMUX_EN;
    return 0;
    // HDMI external switch
    case eHDMISupportExtSwitch:
    *pu4Data = (UINT32)HDMI_SUPPORT_EXT_SWITCH;
    return 0;
     case eHDMIBypassInitialFlow:
    *pu4Data = (UINT32)HDMI_BYPASS_INITIAL_FLOW;
    return 0;
     case eHDMIOffOnMuteCount:
    *pu4Data = (UINT32)HDMI_OFFON_MUTE_COUNT;
    	return 0;
    	case eHDMIEQZEROVALUE: //josh
    *pu4Data = (UINT32)HDMI_TMDS_EQ_ZERO_VALUE;
    	return 0;
    	case eHDMIEQBOOSTVALUE: //josh
    *pu4Data = (UINT32)HDMI_TMDS_EQ_BOOST_VALUE;
    	return 0;
     case eHDMIEQSELVALUE:		
     *pu4Data = (UINT32)HDMI_TMDS_EQ_SEL_VALUE;
     return 0;
     case eHDMIEQGAINVALUE:
     *pu4Data = (UINT32)HDMI_TMDS_EQ_GAIN_VALUE;
     return 0;
     case eHDMILBWVALUE:
     *pu4Data = (UINT32)HDMI_TMDS_HDMI_LBW_VALUE;
     return 0;
     case eDVIWaitStableCount:
    *pu4Data = (UINT32)DVI_WAIT_STABLE_COUNT;
   	 return 0;
      case eDVIWaitNosignalCount:
    *pu4Data = (UINT32)DVI_WAIT_NOSIGNAL_COUNT;
   	 return 0;
      case eDVIWaitSCDTStableCount:
    *pu4Data = (UINT32)HDMI_WAIT_SCDT_STABLE_COUNT;
   	 return 0;
      case eHDMIHDCPMASK1:
    *pu4Data = (UINT32)HDMI_HDCP_Mask1;
   	 return 0;
      case eHDMIHDCPMASK2:
    *pu4Data = (UINT32)HDMI_HDCP_Mask2;
   	 return 0;
    case eMagicMaxInputForceFactory:
#ifndef MAGIC_MAX_INPUT_FORCE_FACTORY
        return -1;
#else
        *pu4Data = (UINT32)MAGIC_MAX_INPUT_FORCE_FACTORY;
        return 0;
#endif

    case eMagicMinInputForceFactory:
#ifndef MAGIC_MIN_INPUT_FORCE_FACTORY
        return -1;
#else
        *pu4Data = (UINT32)MAGIC_MIN_INPUT_FORCE_FACTORY;
        return 0;
#endif

#if 0
    // PIGain Customization
    //PIGain table Modify by W.C Shih
    case ePIGainSetting:
        *pu4Data = (UINT32)PIGAIN_MAP_ARRAY;
        return 0;
    case ePIGainTLSize:
         *pu4Data = (UINT32)PIGAIN_MAP_SIZE;
         return 0;

    case ePolitaryCheck: // VGA Politary Check Modify by W.C Shih2006/10/30
        *pu4Data = (UINT32)FLAG_VGAPOLARITY_CHECK;
         return 0;
#endif

    case eVGAADSpec:
        *pu4Data = (UINT32)VGA_AD_SPEC;
        return 0;

    // VGA H-Frequence Modify by W.C Shih 2007/01/08
    case eVGAHFreqSpecHeight:
    	*pu4Data = (UINT32)VGA_HFH_SPEC;
    	return 0;

    // VGA H-Frequence Modify by W.C Shih 2007/01/08
    case eVGAHFreqSpecLow:
    	*pu4Data = (UINT32)VGA_HFL_SPEC;
    	return 0;

    // VGA H-Frequence Modify by W.C Shih 2007/01/08
    case eVGAVFreqSpecHeight:
    	*pu4Data = (UINT32)VGA_VFH_SPEC;
    	return 0;

    // VGA H-Frequence Modify by W.C Shih 2007/01/08
    case eVGAVFreqSpecLow:
    	*pu4Data = (UINT32)VGA_VFL_SPEC;
    	return 0;

    case eFlagScposSpecalVGA:
        *pu4Data = (UINT32)SCPOS_SUPPORT_SPECIAL_VGA;
        return 0;

    case eVFE_HW_SPECIAL_TARGET:
    	*pu4Data = (UINT32)VFE_HW_SPECIAL_TARGET;  
    	return 0;

    case eVFE_HW_YPbPrMapToVGA:
    	*pu4Data = (UINT32)VFE_HW_YPbPrMapToVGA;  
    	return 0;	

    case eVFE_HW_075ohm:
    	*pu4Data = (UINT32)VFE_HW_075;
    	return 0;


    case eYPBPR_ADC_SUPPORT_120:
    	*pu4Data = (UINT32)YPBPR_ADC_SUPPORT_120;
    	return 0;

    case eDoAutoPhaseAfterAutoColor:
    	*pu4Data = (UINT32)DoAutoPhaseAfterAutoColor;
    	return 0;
    	
    case eSCART_DO_AUTOCOLOR:                                    //MC20081115 add for SCART ***
    	*pu4Data = (UINT32)SCART_DO_AUTOCOLOR;
    	return 0;                                                                  //MC20081115 add for SCART &&&

#if 0
    case eFlagFactoryEdid:
        *pu4Data = (UINT32)FLAG_FACTORY_EDID;
        return 0;
    case eFlagFactoryMode2:
        *pu4Data = (UINT32)FLAG_FACOTRY_MODE2_SUPPORT;
        return 0;
    case eFlagScposSpecalVGA:
        *pu4Data = (UINT32)SCPOS_SUPPORT_SPECIAL_VGA;
        return 0;

    // IC work around.
    case eFlagVgaWakeWorkAround:
        *pu4Data = FLAG_VGA_WAKE_WORKAROUND;
        return 0;
    case eFlagCpupllDmpllWorkAround:
        *pu4Data = FLAG_CPUPLL_DMPLL_WORKAROUND;
        return 0;
#endif

      case eAdpBacklightAplDark:
#ifdef ADAPTIVE_BACKLIGHT_APL_DARK
        *pu4Data = (UINT32)ADAPTIVE_BACKLIGHT_APL_DARK;
#else
        *pu4Data = (UINT32)0x20;
#endif
        return 0;

      case eAdpBacklightAplBright:
#ifdef ADAPTIVE_BACKLIGHT_APL_BRIGHT
        *pu4Data = (UINT32)ADAPTIVE_BACKLIGHT_APL_BRIGHT;
#else
        *pu4Data = (UINT32)0xD0;
#endif
        return 0;

    case eFlagBind2DNRWithMpegNR:
#ifdef BIND_2D_NR_WITH_MPEG_NR    
        *pu4Data = (UINT32)BIND_2D_NR_WITH_MPEG_NR;
#else
        *pu4Data = (UINT32)0;
#endif
        return 0;

    //AdaptiveNR
    case eFlagAutoNR:
#ifdef SUPPORT_AUTO_NR
        *pu4Data = (UINT32)SUPPORT_AUTO_NR;
#else
        *pu4Data = (UINT32)1;
#endif
        return 0;

    case eFlagBindContrastSaturation:
#ifdef BIND_CONTRAST_SATURATION
        *pu4Data = (UINT32)BIND_CONTRAST_SATURATION;
#else
        *pu4Data = (UINT32)0;
#endif
        return 0;        

    case eFlagUIQTYLinearMap:
#ifdef UI_QTY_USE_LINEAR_MAP
        *pu4Data = (UINT32)UI_QTY_USE_LINEAR_MAP;
#else
        *pu4Data = (UINT32)0;
#endif
        return 0;         

    // FlashPQ
    case eFlagFlashPqEnable:
#ifdef SUPPORT_FLASH_PQ
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ
        return 0;
	case eFlashPqUseNandFalsh:
#ifdef FLASH_PQ_USE_NAND_FLASH
		*pu4Data = (UINT32)FLASH_PQ_USE_NAND_FLASH;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef FLASH_PQ_USE_NAND_FLASH
        return 0;
	case eFlashPqBasePartition:
#ifdef FLASH_PQ_BASE_PARTITION
		*pu4Data = (UINT32)FLASH_PQ_BASE_PARTITION;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef FLASH_PQ_BASE_PARTITION
        return 0;
	case eFlashPqBaseAddress:
#ifdef FLASH_PQ_BASE_ADDRESS
		*pu4Data = (UINT32)FLASH_PQ_BASE_ADDRESS&0xFFFFFF;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef FLASH_PQ_BASE_ADDRESS
        return 0;
	case eFlashPqOffset:
#ifdef FLASH_PQ_BASE_OFFSET
		*pu4Data = (UINT32)FLASH_PQ_BASE_OFFSET;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef FLASH_PQ_BASE_OFFSET
		return 0;
	case eFlashPqBlockSize:
#ifdef FLASH_PQ_BLOCK_SIZE
		*pu4Data = (UINT32)FLASH_PQ_BLOCK_SIZE;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef FLASH_PQ_BLOCK_SIZE
        return 0;
	case eFlagFlashPqSmartPicOffset:
#ifdef FLASH_SP_BASE_OFFSET
		*pu4Data = (UINT32)FLASH_SP_BASE_OFFSET;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef FLASH_SP_BASE_OFFSET
		return 0;
	case eFlagFlashPqEnableGamma:
#ifdef SUPPORT_FLASH_PQ_ENABLE_GAMMA
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_ENABLE_GAMMA;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_ENABLE_GAMMA
		return 0;
	case eFlagFlashPqGammaX3:
#ifdef SUPPORT_FLASH_PQ_GAMMA_X3
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_GAMMA_X3;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_GAMMA_X3
		return 0;
	case eFlagFlashPqEnableSCE:
#ifdef SUPPORT_FLASH_PQ_ENABLE_SCE
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_ENABLE_SCE;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_ENABLE_SCE
		return 0;
	case eFlagFlashPqEnableQty:
#ifdef SUPPORT_FLASH_PQ_ENABLE_QTY
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_ENABLE_QTY;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_ENABLE_QTY
		return 0;
	case eFlagFlashPqEnableQtyMaxMin:
#ifdef SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN
		return 0;
	case eFlagFlashPqEnableQtySmartPic:
#ifdef SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC
		return 0;
	case eFlagLinkM4ContrastSaturation:
#ifdef SUPPORT_LINK_M4_CONTRAST_SATURATION
		*pu4Data = (UINT32)SUPPORT_LINK_M4_CONTRAST_SATURATION;
#else
		*pu4Data = (UINT32)1;
#endif // #ifdef SUPPORT_LINK_M4_CONTRAST_SATURATION
		return 0;
	case eFlagBacklightBoostCtrl:
#ifdef SUPPORT_BACKLIGHT_BOOST_CTRL
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_BOOST_CTRL;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_BOOST_CTRL
	case eFlagGain1AsColorTemp:
#ifdef FLAG_GAIN1_AS_COLORTEMP
		*pu4Data = (UINT32)FLAG_GAIN1_AS_COLORTEMP;
#else
		*pu4Data = (UINT32)1;	// Default use RGB gain1 as color temperature control.
#endif // #ifdef FLAG_GAIN1_AS_COLORTEMP
		return 0;
	case eFlagVideoLvlExtPosition:
#ifdef FLAG_MATRIX_AS_VIDEO_EXT
		*pu4Data = (UINT32)FLAG_MATRIX_AS_VIDEO_EXT;
#else
		*pu4Data = (UINT32)0;	// Default use RGB gain1 as video level extend; RGB gain2 as color temp.
#endif // #ifdef FLAG_MATRIX_AS_VIDEO_EXT
		return 0;
       case eBrightInterval:
#ifdef BIRGHT_INTERVAL
		*pu4Data = (UINT32)BIRGHT_INTERVAL;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef BIRGHT_INTERVAL
             return 0;
          case eFlagUseMatrixPQAdj:
#ifdef USE_MATRIX_PQ_ADJ
               *pu4Data = (UINT32)USE_MATRIX_PQ_ADJ;
#else
               *pu4Data = (UINT32)0;
#endif // #ifdef USE_MATRIX_PQ_ADJ
                return 0;
       case eNVMGamma:                  // Read gamma from EERPOM
#ifdef SUPPORT_NVM_GAMMA
		*pu4Data = (UINT32)SUPPORT_NVM_GAMMA;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_NVM_GAMMA
             return 0;             
       case eCustSetGammaTopPoint:
#ifdef SUPPORT_GAMMA_TOP_POINT
		*pu4Data = (UINT32)SUPPORT_GAMMA_TOP_POINT;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_GAMMA_TOP_POINT
             return 0;

    case eFlagFlashPqSCEMax:
#ifdef FLASH_PQ_SCE_MAX
        *pu4Data = (UINT32)FLASH_PQ_SCE_MAX;
#else
        *pu4Data = (UINT32)0;
#endif // #ifdef FLASH_PQ_SCE_MAX
        return 0;        
    // RTC external h/w support related
    case eRtcHw:
        *pu4Data = RTC_HW;
         return 0;
    case eRtcExtIntGpio:
#ifdef RTC_EXT_INTGPIO
        *pu4Data = RTC_EXT_INTGPIO;
         return 0;
#else
         return -1;
#endif
    case eRtcExtClkDiv:
        *pu4Data = RTC_EXT_CLKDIV;
         return 0;

#ifdef SCART1_FS_SERVO_ADC
    case eScart1FSServoAdc:
        *pu4Data = SCART1_FS_SERVO_ADC;
        return 0;
#endif
		
#ifdef SCART2_FS_SERVO_ADC
    case eScart2FSServoAdc:
        *pu4Data = SCART2_FS_SERVO_ADC;
        return 0;
#endif

    // SCART Fast Blanking In Selection
    case eSCART1FBInSelect:
        *pu4Data = (UINT32)SCART1_FB_IN_SELECT;
    return 0;
    case eSCART2FBInSelect:
        *pu4Data = (UINT32)SCART2_FB_IN_SELECT;
    return 0;

    case eScart1Type:
        *pu4Data = SUPPORT_SCART1_TYPE;
    return 0;
    case eScart2Type:
        *pu4Data = SUPPORT_SCART2_TYPE;
    return 0;

    case eScart1IntScCh:
    	  *pu4Data = SCART1_INT_SC_CH;
    return 0;
    case eScart2IntScCh:
    	  *pu4Data = SCART2_INT_SC_CH;
    return 0;


    // Run on NOR flash or not
    case eFlagRunOnNor:
        *pu4Data = FLAG_RUN_ON_NOR;
         return 0;

    // MT8292 GPIO support
    case eFlagMt8292GpioSupport:
        *pu4Data = FLAG_MT8292_GPIO_SUPPORT;
         return 0;
    case eMt8292ResetGpio:
        *pu4Data = MT8292_RESET_GPIO;
         return 0;
    case eMt8292MuteGpio:
        *pu4Data = MT8292_MUTE_GPIO;
         return 0;

    // Smart Card
    case eSMCHotplugGpio:
        *pu4Data = SMC_HOTPLUG_GPIO;
         return 0;
         
    case eSMCHotplugGpioPolarity:
        *pu4Data = SMC_HOTPLUG_GPIO_POLARITY;
         return 0;
  
    // PCD
    case ePcdWakeupTheKey:
#ifndef PCD_WAKEUP_THE_KEY
        return -1;
#else /* PCD_WAKEUP_THE_KEY */
        *pu4Data = (UINT32)PCD_WAKEUP_THE_KEY;
        return 0;
#endif /* PCD_WAKEUP_THE_KEY */
#ifdef WAKEUP_REASON_GPIO
    case eWakeupReasonGpio:
        *pu4Data = (UINT32)WAKEUP_REASON_GPIO;
        return 0;
#endif /* WAKEUP_REASON_GPIO */
#ifdef WAKEUP_REASON_GPIO_POLARITY
    case eWakeupReasonGpioPolarity:
        *pu4Data = (UINT32)WAKEUP_REASON_GPIO_POLARITY;
        return 0;
#endif /* WAKEUP_REASON_GPIO_POLARITY */
#ifdef CI_8295_RESET_GPIO
    case eMT8295ResetGpio:
        *pu4Data = (UINT32)CI_8295_RESET_GPIO;
        return 0;
#endif /* CI_8295_RESET_GPIO */
    case eMT8295DrivingCurrent:
        *pu4Data = (UINT32)CI_8295_DRIVING_CURRENT;
        return 0;
    case eMT8295TSDrivingCurrent:
        *pu4Data = (UINT32)CI_TS_DRIVING_CURRENT;
        return 0;
    case eMT8295PBIDrivingCurrent:
        *pu4Data = (UINT32)CI_PBI_DRIVING_CURRENT;
        return 0;
    case eCI_PMX_CFG:
        *pu4Data = (UINT32)CI_PMX_CFG;
        return 0;
#ifdef CI_8295_INT_GPIO
    case eMT8295IntGpio:
        *pu4Data = (UINT32)CI_8295_INT_GPIO;
        return 0;
#endif /* CI_8295_INT_GPIO */
#ifdef CI_5V_POWERON_GPIO
    case eCI5VPowerOnGpio:
        *pu4Data = (UINT32)CI_5V_POWERON_GPIO;
        return 0;
#endif /* CI_5V_POWERON_GPIO */
#ifdef CI_5V_POWERON_POLARITY
    case eCI5VPowerOnPolarity:
        *pu4Data = (UINT32)CI_5V_POWERON_POLARITY;
        return 0;
#endif /* CI_5V_POWERON_POLARITY */
#ifdef CI_5V_OC_GPIO
    case eCI5VOcGpio:
        *pu4Data = (UINT32)CI_5V_OC_GPIO;
        return 0;
#endif
#ifdef CI_5V_OC_POLARITY
    case eCI5VOcPolarity:
        *pu4Data = (UINT32)CI_5V_OC_POLARITY;
        return 0;
#endif
    case eUART1Support:
        *pu4Data = (UINT32)UART1_SUPPORT_ENABLE;
        return 0;

    case eUART2Support:
        *pu4Data = (UINT32)UART2_SUPPORT_ENABLE;
        return 0;

    case eSCARTRGBOrder:
    *pu4Data = (UINT32)SCART_RGB_ORDER;
        return 0;

    case eSCARTPin8LowThres:
    *pu4Data = (UINT32)SCART_PIN8_LOW_THRES;
    	 return 0;

    case eSCARTPin8HiThres:
    *pu4Data = (UINT32)SCART_PIN8_HI_THRES;
    	 return 0;    	     	 

    case eNotifyMuteFunc:
    	*pu4Data = (UINT32)NOTIFY_MUTE_FUNC;
       return 0;
    case eNotifyMixerVideoLayerFunc:
    	*pu4Data = (UINT32)NOTIFY_MIXER_VIDEOLAYER_FUNC;
       return 0;
    case eNotifyMixerFunc:
    	*pu4Data = (UINT32)NOTIFY_MIXER_FUNC;
       return 0;
    case eNotifyVideoPropertyFunc:
    	*pu4Data = (UINT32)NOTIFY_VIDEO_FUNC;
       return 0;
    case eNotifyVideoAutoDoneFunc:
    	*pu4Data = (UINT32)NOTIFY_VIDEO_AUTO_DONE_FUNC;
       return 0;
#if 0
    case eNotifyFEColorSystem:
    	*pu4Data = (UINT32)NOTIFY_FE_COLORSYSTEM;
       return 0;
    case eNotifyFECombFilter:
    	*pu4Data = (UINT32)NOTIFY_FE_COMBFILTER;
       return 0;
    case eNotifyFESoundSys:
        *pu4Data = (UINT32)0;
       return 0;
#endif 
    case eCiCustomQueryFunc:
    	*pu4Data = (UINT32)CI_CUSTOM_QUERY_FUNC;
       return 0;
#if 0
    case eNotifyAFSoundProperties:
       *pu4Data = (UINT32)NOTIFY_AF_SOUND_PROPERTIES;
       return 0;
    case eNotifyAFDRCAllowed:
       *pu4Data = (UINT32)NOTIFY_AF_DRC_ALLOWED;
       return 0;
#endif

    // Linux memory usage related customization
    case eLinuxKernelMemSize:
    	*pu4Data = (UINT32)LINUX_KERNEL_MEM_SIZE;
       return 0;
    case eTotalMemSize:
    	*pu4Data = (UINT32)TOTAL_MEM_SIZE;
       return 0;
    case eFBMemSize:
    	*pu4Data = (UINT32)FB_MEM_SIZE;
       return 0;
    case eDirectFBMemSize:
    	*pu4Data = (UINT32)DIRECT_FB_MEM_SIZE;
       return 0;
#if 0 
    case eDmxSharedMemSize:
    	*pu4Data = (UINT32)DMX_SHARED_MEM_SIZE;
       return 0;
    case eReservedMemSize:
    	*pu4Data = (UINT32)RESERVED_MEM_SIZE;
       return 0;
#endif

    // PWM / GPIO
    case ePanelBacklightGPIO:
#ifdef PANEL_BACKLIGHT_PWM_GPIO
		*pu4Data = PANEL_BACKLIGHT_PWM_GPIO;
#else
		*pu4Data = 0; // Default using PWM output
#endif /* PANEL_BACKLIGHT_PWM_GPIO */
       return 0;

#ifdef CUSTOMER_PASSWORD
    case eCustomerPassword:
        *pu4Data = (UINT32)CUSTOMER_PASSWORD;
        return 0;
#endif /* CUSTOMER_PASSWORD */

#ifdef CUSTOMER_NAME
    case eCustomerName:
        *pu4Data = (UINT32)CUSTOMER_NAME;
        return 0;
#endif /* CUSTOMER_NAME */

    // SIF
    case eSifEnableSclStretch:
        *pu4Data = (UINT32)SIF_ENABLE_SCL_STRETCH;
        return 0;
    case eSifDisableOpenDrain:
        *pu4Data = (UINT32)SIF_DISABLE_OPEN_DRAIN;
        return 0;
    case eSifEnablePdwncMaster:
        *pu4Data = (UINT32)SIF_ENABLE_PDWNC_MASTER;
        return 0;
	case eSifEnable8295bMaster0:
		*pu4Data = (UINT32)SIF_ENABLE_8295B_MASTER0;
        return 0;
	case eSifEnable8295bMaster1:
		*pu4Data = (UINT32)SIF_ENABLE_8295B_MASTER1;
        return 0;

    case eTVESupportAFD:
        *pu4Data = (UINT32)TVE_SUPPORT_AFD;
        return 0;

    case eCountryFromEEPROM:
#ifdef COUNTRY_FROM_EEPROM
        *pu4Data = (UINT32)COUNTRY_FROM_EEPROM;
        return 0;
#else        
        return -1;
#endif

//Driver logo
    case eDrvLogoEnable:
  #ifdef DRV_LOGO_ENABLE
         *pu4Data = (UINT32)DRV_LOGO_ENABLE;
         return 0;
  #else        
         return -1;
  #endif
    case eDrvLogoWidth:
  #ifdef DRV_LOGO_WIDTH
         *pu4Data = (UINT32)DRV_LOGO_WIDTH;
         return 0;
  #else        
         return -1;
  #endif
  case eDrvLogoHeight:
  #ifdef DRV_LOGO_HIGHT
         *pu4Data = (UINT32)DRV_LOGO_HIGHT;
         return 0;
  #else        
         return -1;
  #endif
  case eDrvLogoColorMode:
  #ifdef DRV_LOGO_COLOR_MODE
         *pu4Data = (UINT32)DRV_LOGO_COLOR_MODE;
         return 0;
  #else        
         return -1;
  #endif
  case eDrvLogoBmpAddr:
  #ifdef DRV_LOGO_BMP_ADR
         *pu4Data = (UINT32)DRV_LOGO_BMP_ADR;
         return 0;
  #else        
         return -1;
  #endif
  case eDrvLogoPalAddr:
  #ifdef DRV_LOGO_PAL_ADR
         *pu4Data = (UINT32)DRV_LOGO_PAL_ADR;
         return 0;
  #else        
         return -1;
  #endif

    case eCountryEEPROMOffset:
#ifdef EEPROM_COUNTRY_OFFSET
        *pu4Data = (UINT32)EEPROM_COUNTRY_OFFSET;
        return 0;
#else        
        return -1;
#endif

    case eEFuseAutocolor:
        *pu4Data = (UINT32)EFUSE_AUTOCOLOR;
      	return 0;
	case eAUTOCOLOR_FAIL_SHOW_PATTERN:
		*pu4Data = (UINT32)AUTOCOLOR_FAIL_SHOW_PATTERN;
		return 0;
    case eFrameTrackMode:
        #ifdef FRAME_TRACK_MODE
        *pu4Data = (UINT32)FRAME_TRACK_MODE;
        #else
        *pu4Data = 1;
        #endif
        return 0;
        
    case eFrameTrackStableCnt:
        #ifdef FRAME_TRACK_STABLE_COUNT
        *pu4Data = (UINT32)FRAME_TRACK_STABLE_COUNT;
        #else
        *pu4Data = 0;
        #endif
        return 0;

    default:
        break;
    }
    return -1;
}

UINT32 DRVCUST_OptGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret;

 //   VERIFY(DRVCUST_OptQuery(eQryType, &u4Ret) == 0);
    DRVCUST_OptQuery(eQryType, &u4Ret);
    return u4Ret;
}

NONLINEAR_PRM_T* DRVCUST_NonLinearTblGet(void)
{
//  return Nonlinear_Prm_Tbl;
    return NULL;
}

UINT32 DRVCUST_NonLinearTblSizeGet(void)
{
//  return sizeof(Nonlinear_Prm_Tbl)/sizeof(NONLINEAR_PRM_T);
    return 1;
}

#ifndef OPCODE_VALUE_QUERY_FILE
UINT32 DRVCUST_OpCodeQuery(QUERY_TYPE_T eQryType)
{
    UNUSED(eQryType);
    return 1;
}
#endif /* DRVCUST_OPCODE_QUERY_FUNC */

static UINT32 g_fgFlag = 0;

void DRVCUST_SetDrvInit(UINT32 fgFlag)
{
    if (DRVCUST_InitGet(eLoaderLogoNumber))
    {
        g_fgFlag = fgFlag;
    }
}

UINT32 DRVCUST_GetDrvInit()
{
    return g_fgFlag;
}

#endif /* CC_MTK_LOADER */

