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

#ifndef CC_5391_LOADER

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

#include "drv_def_file.h"

//----------------------------------------------------------------------------
// Customization include files
//----------------------------------------------------------------------------

#ifndef NO_INCLUDE_FILES

#ifdef RTC_EXT_IMPL
#include RTC_EXT_IMPL
#endif

#ifdef AUDIO_INCLUDE_FILE
#include AUDIO_INCLUDE_FILE
#include "mtk/mtk_aud.h"
#endif

#endif /* NO_INCLUDE_FILES */

#include "drv_default.h"


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
        *pu4Data = (UINT32) DEFAULT_TUNER_CTRL_ANA;
        return 0;

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
#if 0
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
    case eAudioInputSwGpioSel:
        *pu4Data = (UINT32)AUDIO_INPUT_SW_GPIO_SEL;
        return 0;
    case eAudioInputMuxSelTable:
        *pu4Data = (UINT32)AUDIO_INPUT_MUX_SEL_TABLE;
        return 0;
    case eAudioOutFmtTable:
        *pu4Data = (UINT32)AUDIO_OUT_FMT_TABLE;
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
    case eAudioChannelDelay:
        *pu4Data = (UINT32)AUDIO_CHANNEL_DELAY;
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
    case eAudioNvmEnable:
         *pu4Data = (UINT32)AUDIO_NVM_ENABLE;
        return 0;
    case eAudioNvmLength:
         *pu4Data = (UINT32)AUDIO_NVM_LENGTH;
        return 0;
    case eAudioNvmData:
        *pu4Data = (UINT32)AUDIO_NVM_DATA;
        return 0;
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


#endif
    // EDID EEPROM
#ifdef EDID_EEPROM_WP_GPIO
    case eEdidEepromWPGpio:
        *pu4Data = EDID_EEPROM_WP_GPIO;
        return 0;
#endif
    case eEdidEepromWPEnablePolarity:
        *pu4Data = EDID_EEPROM_WP_ENABLE_POLARITY;
        return 0;

    // NOR flash parameter.
    case eNorFlashTotalSize:
        *pu4Data = NOR_FLASH_TOTAL_SIZE;
        return 0;
    case eNorFlashAppImageSize:
        *pu4Data = NOR_FLASH_APPIMAGE_SIZE;
        return 0;
    case eNorFlashDynamicPartitionSize:
        *pu4Data = NOR_FLASH_DYNAMIC_PART_SIZE;
        return 0;
    case eNorFlashStaticPartitionSize:
        *pu4Data = NOR_FLASH_STATIC_PART_SIZE;
        return 0;
    case eHDCPNorPartId:
        *pu4Data = HDCP_NOR_FLASH_PART_ID;
        return 0;

    // NOR flash partition size customization
    case eNorFlashPartSize0:
        *pu4Data = NOR_PART_SIZE_0;
        return 0;
    case eNorFlashPartSize1:
        *pu4Data = NOR_PART_SIZE_1;
        return 0;
    case eNorFlashPartSize2:
        *pu4Data = NOR_PART_SIZE_2;
        return 0;
    case eNorFlashPartSize3:
        *pu4Data = NOR_PART_SIZE_3;
        return 0;
    case eNorFlashPartSize4:
        *pu4Data = NOR_PART_SIZE_4;
        return 0;
    case eNorFlashPartSize5:
        *pu4Data = NOR_PART_SIZE_5;
        return 0;
    case eNorFlashPartSize6:
        *pu4Data = NOR_PART_SIZE_6;
        return 0;
    case eNorFlashPartSize7:
        *pu4Data = NOR_PART_SIZE_7;
        return 0;

#ifdef FCI_CARD_DETECT_GPIO
    case eFCICardDetectGpio:
        *pu4Data = FCI_CARD_DETECT_GPIO;
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
     case eDVIWaitStableCount:
    *pu4Data = (UINT32)DVI_WAIT_STABLE_COUNT;
   	 return 0;
      case eDVIWaitNosignalCount:
    *pu4Data = (UINT32)DVI_WAIT_NOSIGNAL_COUNT;
   	 return 0;
      case eDVIWaitSCDTStableCount:
    *pu4Data = (UINT32)HDMI_WAIT_SCDT_STABLE_COUNT;
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

    case eFlagSCCGamma:
#ifdef FLAG_SCC_GAMMA
        *pu4Data = (UINT32)FLAG_SCC_GAMMA;
#else
        *pu4Data = (UINT32)0;
#endif
        return 0;

      case eFlagLinkAdpLumaWithAdpBacklight:
#ifdef FLAG_LINK_ADPLUMA_ADPBACKLIGHT
        *pu4Data = (UINT32)FLAG_LINK_ADPLUMA_ADPBACKLIGHT;
#else
        *pu4Data = (UINT32)0;
#endif
        return 0;

    case eFlagDirectShiftHue:
#ifdef FLAG_DIRECT_SHIFT_HUE
        *pu4Data = (UINT32)FLAG_DIRECT_SHIFT_HUE;
#else
        *pu4Data = (UINT32)0;
#endif
        return 0;

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

    //VGA Color Process
    case eFlagVGAColorMatrix:
#ifdef FLAG_VGA_COLOR_MATRIX
        *pu4Data = (UINT32)FLAG_VGA_COLOR_MATRIX;
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

    // FlashPQ
    case eFlagFlashPqEnable:
#ifdef SUPPORT_FLASH_PQ
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ
        return 0;
	case eFlashPqBaseAddress:
#ifdef FLASH_PQ_BASE_ADDRESS
		*pu4Data = (UINT32)FLASH_PQ_BASE_ADDRESS;
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
	case eFlagFlashPqMultiGamma:
#ifdef SUPPORT_FLASH_PQ_MULTI_GAMMA
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_MULTI_GAMMA;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_MULTI_GAMMA
		break;
	case eFlagFlashPqMultiSCE:
#ifdef SUPPORT_FLASH_PQ_MULTI_SCE
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_MULTI_SCE;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_MULTI_SCE
		break;
	case eFlagFlashPqMultiQty:
#ifdef SUPPORT_FLASH_PQ_MULTI_QTY
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_MULTI_QTY;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_MULTI_QTY
		break;
	case eFlagFlashPqMultiSmartPic:
#ifdef SUPPORT_FLASH_PQ_MULTI_SMART_PIC
		*pu4Data = (UINT32)SUPPORT_FLASH_PQ_MULTI_SMART_PIC;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_FLASH_PQ_MULTI_SMART_PIC
		break;

	case eFlagGain1AsColorTemp:
#ifdef FLAG_GAIN1_AS_COLORTEMP
		*pu4Data = (UINT32)FLAG_GAIN1_AS_COLORTEMP;
#else
		*pu4Data = (UINT32)1;	// Default use RGB gain1 as color temperature control.
#endif // #ifdef FLAG_GAIN1_AS_COLORTEMP
		break;

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

    case eScart1FSServoAdc:
        *pu4Data = SCART1_FS_SERVO_ADC;
        return 0;
    case eScart2FSServoAdc:
        *pu4Data = SCART2_FS_SERVO_ADC;
        return 0;

    // SCART Fast Blanking In Selection
    case eSCARTFBInSelect:
        *pu4Data = (UINT32)SCART_FB_IN_SELECT;
    return 0;

    case eScartType:
        *pu4Data = SUPPORT_SCART_TYPE;        
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

    // PCD
    case ePcdWakeupTheKey:
#ifndef PCD_WAKEUP_THE_KEY
        return -1;
#else /* PCD_WAKEUP_THE_KEY */
        *pu4Data = (UINT32)PCD_WAKEUP_THE_KEY;
        return 0;
#endif /* PCD_WAKEUP_THE_KEY */
    case eWakeupReasonGpio:
        *pu4Data = (UINT32)WAKEUP_REASON_GPIO;
        return 0;
    case eWakeupReasonGpioPolarity:
        *pu4Data = (UINT32)WAKEUP_REASON_GPIO_POLARITY;
        return 0;
    case eMT8295ResetGpio:
        *pu4Data = (UINT32)CI_8295_RESET_GPIO;
        return 0;
    case eCI_PMX_CFG:
        *pu4Data = (UINT32)CI_PMX_CFG;
        return 0;
    case eMT8295IntGpio:
        *pu4Data = (UINT32)CI_8295_INT_GPIO;
        return 0;    // UART
    case eUART1Support:
        *pu4Data = (UINT32)UART1_SUPPORT_ENABLE;
        return 0;
        
    case eSCARTRGBOrder:
    *pu4Data = (UINT32)SCART_RGB_ORDER;
        return 0;
    case eSCARTScRExt:
    *pu4Data = (UINT32)SCART_SC_R_EXTERNAL;
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

#endif /* CC_5391_LOADER */

