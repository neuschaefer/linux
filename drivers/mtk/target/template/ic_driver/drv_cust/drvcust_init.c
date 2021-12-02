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

#ifndef CC_NAND_BOOT

// driver general header file
#include "x_typedef.h"
#include "x_assert.h"
#include "x_serial.h"
// driver customized header file
#include "drvcust_if.h"


//----------------------------------------------------------------------------
// driver customization setting include files
//----------------------------------------------------------------------------

#ifdef CUSTOM_CFG_FILE
#include CUSTOM_CFG_FILE
#endif /* CUSTOM_CFG_FILE */

#include "drv_def_file.h"


//----------------------------------------------------------------------------
// include customized files
//----------------------------------------------------------------------------

#ifndef CC_5391_PRELOADER

#ifdef PDWNC_EXT_IMPL
#include PDWNC_EXT_IMPL
#endif

#ifdef IR_INCLUDE_FILE
//#include "ir_if.h"
#include IR_INCLUDE_FILE
#endif

#ifdef BUTTON_INCLUDE_FILE
#include BUTTON_INCLUDE_FILE
#endif

#ifdef GPIO_INCLUDE_FILE
#include GPIO_INCLUDE_FILE
#endif

#endif /* CC_5391_PRELOADER */

#include "drv_default.h"

#ifndef CC_5391_PRELOADER

#ifdef CC_UBOOT
#ifdef LOADER_LOGO_H_FILE
#if LOADER_LOGO_FLASHADDR
#else /* LOADER_LOGO_FLASHADDR */
#include LOADER_LOGO_H_FILE
#endif /* LOADER_LOGO_FLASHADDR */
#endif /* LOADER_LOGO_H_FILE */
#endif /* CC_UBOOT */

#endif /* CC_5391_PRELOADER */

//----------------------------------------------------------------------------
// Public function implementation
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
/** DRVCUST_InitQuery() return initial customization paramter, use for loader or BSP.
 */
//----------------------------------------------------------------------------
INT32 DRVCUST_InitQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data)
{
    static CHAR *szDate = __DATE__;
    static CHAR *szTime = __TIME__;
#ifndef CC_UBOOT
    static CHAR *szBrdCstIf = BRDCST_IF;
    static CHAR *szDrvCustom = DRV_CUSTOM;
#else /* CC_UBOOT */
    static CHAR *szBrdCstIf = "uBoot";
    static CHAR *szDrvCustom = "uBoot";
#endif /* CC_UBOOT */

#ifndef CC_5391_PRELOADER
    if (DRVCUST_PanelQuery(eQryType, pu4Data)==0)
    {
        return 0;
    }
#endif /* CC_5391_PRELOADER */

    ASSERT(pu4Data!=NULL);
    switch(eQryType)
    {
// Common part.
    case eBuildDate:
        LINT_SUPPRESS_NEXT_EXPRESSION(50);
        *pu4Data = (UINT32)szDate;
        return 0;
    case eBuildTime:
        LINT_SUPPRESS_NEXT_EXPRESSION(50);
        *pu4Data = (UINT32)szTime;
        return 0;
    case eBrdCstIf:
        LINT_SUPPRESS_NEXT_EXPRESSION(50);
        *pu4Data = (UINT32)szBrdCstIf;
        return 0;
    case eDrvCust:
        LINT_SUPPRESS_NEXT_EXPRESSION(50);
        *pu4Data = (UINT32)szDrvCustom;
        return 0;
// Boot
    case eFlagBootVerbose:
        *pu4Data = DEFAULT_BOOT_VERBOSE;
        return 0;
    case eNorCpuClock:
        *pu4Data = DEFAULT_NOR_CPU_CLK;
        return 0;
    case eDramType:
        *pu4Data = DEFAULT_DRAM_TYPE;
        return 0;
    case eDramParamE:
        *pu4Data = DEFAULT_DRAM_PARAM_E;
        return 0;
    case eDramColAddr:
        *pu4Data = DEFAULT_DRAM_COLADDR;
        return 0;
    case eDdrClock:
        *pu4Data = DEFAULT_DDR_CLOCK;
        return 0;
    case eDdrCL:
        *pu4Data = DEFAULT_DDR_CL;
        return 0;
    case eFlagDmpllSpectrum:
        *pu4Data = FLAG_DMPLL_SPECTRUM;
        return 0;
    case eDmpllSpectrumPermillage:
        *pu4Data = DMPLL_SPECTRUM_PERMILLAGE;
        return 0;
    case eFlagSamsungGDDR:
        *pu4Data = FLAG_SAMSUNG_GDDR;
        return 0;
    case eFlag1GBitSupport:
        *pu4Data = FLAG_1GBIT_SUPPORT;
        return 0;
    case eFlagForce32bit:
        *pu4Data = FLAG_FORCE_32BIT;
        return 0;
    case eFlagFullFreq:
        *pu4Data = FLAG_FULL_FREQ;
        return 0;
    case eFlagDDRQfp:
        *pu4Data = FLAG_DDR_QFP;
        return 0;
    case eFlagSingleDQS:
        *pu4Data = FLAG_SINGLE_DQS;
        return 0;
    case eSysResetTimer:
        *pu4Data = (SYS_RESET_TIMER == 0x1000000) ? (SYS_RESET_TIMER - 1) : SYS_RESET_TIMER;
        return 0;
    case eDramAgentPriority:
    	*pu4Data = (UINT32)DRAM_PRIORITY_LIST;
    	return 0;
    case eDramBurstLen:
    	*pu4Data = (UINT32)DRAM_BURSTLEN;
    	return 0;
    case eDramGroup1ArbiterTime:
    	*pu4Data = (UINT32)DRAM_GROUP1ARBITERTIME;
    	return 0;
    case eDramGroup2ArbiterTime:
    	*pu4Data = (UINT32)DRAM_GROUP2ARBITERTIME;
    	return 0;
    case eDramGroup3ArbiterTime:
    	*pu4Data = (UINT32)DRAM_GROUP3ARBITERTIME;
    	return 0;

#ifndef CC_5391_PRELOADER

// Fbm Size needed in loader.
    case eFbmMemSize:
        *pu4Data = FBM_MEM_CFG_SIZE;
        return 0;

// Usb upgrade
    case eAutoUsbUpgradeEnable:
        *pu4Data = (UINT32)AUTO_USB_UPGRADE_ENABLE;
        return 0;
    case eUsbUpgradeFileName:
        *pu4Data = (UINT32)USB_UPGRADE_FILE_NAME;
        return 0;
    case eLoaderUpgradeVerCmpFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_VER_CMP_FUNC;
        return 0;
// Loader logo
    case eLoaderLogoNumber:
        *pu4Data = (UINT32)LOADER_LOGO_NUMBER;
        return 0;
    case eLoaderLogoBackground:
        *pu4Data = (UINT32)LOADER_LOGO_BACKGROUND;
        return 0;
    case eLoaderLogoColorMode:
#ifndef LOADER_LOGO_COLOR_MODE
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_COLOR_MODE;
        return 0;
#endif
    case eLoaderLogoWidthArray:
#ifndef LOADER_LOGO_WIDTH_ARRAY
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_WIDTH_ARRAY;
        return 0;
#endif
    case eLoaderLogoHightArray:
#ifndef LOADER_LOGO_HIGHT_ARRAY
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_HIGHT_ARRAY;
        return 0;
#endif
    case eLoaderLogoFlashAddr:
        *pu4Data = (UINT32)LOADER_LOGO_FLASHADDR;
        return 0;
    case eLoaderLogoDataIdx0:
#ifndef LOADER_LOGO_DATAIDX0
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX0;
        return 0;
#endif

    case eLoaderLogoDataIdx1:
#ifndef LOADER_LOGO_DATAIDX1
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX1;
        return 0;
#endif

    case eLoaderLogoDataIdx2:
#ifndef LOADER_LOGO_DATAIDX2
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX2;
        return 0;
#endif

    case eLoaderLogoDataIdx3:
#ifndef LOADER_LOGO_DATAIDX3
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX3;
        return 0;
#endif

    case eLoaderLogoDataIdx4:
#ifndef LOADER_LOGO_DATAIDX4
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX4;
        return 0;
#endif

    case eLoaderLogoDataIdx5:
#ifndef LOADER_LOGO_DATAIDX5
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX5;
        return 0;
#endif

    case eLoaderLogoDataIdx6:
#ifndef LOADER_LOGO_DATAIDX6
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX6;
        return 0;
#endif

    case eLoaderLogoDataIdx7:
#ifndef LOADER_LOGO_DATAIDX7
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX7;
        return 0;
#endif

    case eLoaderLogoDataIdx8:
#ifndef LOADER_LOGO_DATAIDX8
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX8;
        return 0;
#endif

    case eLoaderLogoDataIdx9:
#ifndef LOADER_LOGO_DATAIDX9
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX9;
        return 0;
#endif

// NOR flash parameter
    case eNorFlashLoaderSize:
        *pu4Data = NOR_FLASH_LOADER_SIZE;
        return 0;

// System EEPROM
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

    // System EEPROM Layout Configuration
#ifdef DRVCUST_EEPROM_HDMI_HDCP_OFFSET
    case eEepromHdmiHdcpOffset:
         *pu4Data =  DRVCUST_EEPROM_HDMI_HDCP_OFFSET;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_HDMI_HDCP_SIZE
    case eEepromHdmiHdcpSize:
         *pu4Data =  DRVCUST_EEPROM_HDMI_HDCP_SIZE;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_DTV_DRIVER_OFFSET
    case eEepromDtvDriverOffset:
         *pu4Data =  DRVCUST_EEPROM_DTV_DRIVER_OFFSET;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_DTV_DRIVER_SIZE
    case eEepromDtvDriverSize:
         *pu4Data =  DRVCUST_EEPROM_DTV_DRIVER_SIZE ;
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_NPTV_DRIVER_OFFSET
    case eEepromNptvDriverOffset:
         *pu4Data =  DRVCUST_EEPROM_NPTV_DRIVER_OFFSET ;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_NPTV_DRIVER_SIZE
    case eEepromNptvDriverSize:
         *pu4Data = DRVCUST_EEPROM_NPTV_DRIVER_SIZE ;    	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_DRIVER_SIZE
    case eEepromDriverSize:
         *pu4Data = DRVCUST_EEPROM_DRIVER_SIZE ;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_ERROR_CODE_OFFSET
    case eEepromErrorCodeOffset:
         *pu4Data =  DRVCUST_EEPROM_ERROR_CODE_OFFSET;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_ERROR_CODE_SIZE
    case eEepromErrorCodeSize:
         *pu4Data =  DRVCUST_EEPROM_ERROR_CODE_SIZE;
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_STATE_CODE_OFFSET
    case eEepromStateCodeOffset:
         *pu4Data =  DRVCUST_EEPROM_STATE_CODE_OFFSET;   	
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_STATE_CODE_SIZE
    case eEepromStateCodeSize:
         *pu4Data =  DRVCUST_EEPROM_STATE_CODE_SIZE;
    	  return 0;
#endif

#if 0
    // GPIO hook function
    case eGpioPowerOnFunc:
#ifndef GPIO_POWER_ON_FUNC
#error not define GPIO_POWER_ON_FUNC
#else /* GPIO_POWER_ON_FUNC */
        *pu4Data = (UINT32)GPIO_POWER_ON_FUNC;
#endif /* GPIO_POWER_ON_FUNC */
        return 0;
    case eGpioPowerOffFunc:
#ifndef GPIO_POWER_OFF_FUNC
#error not define GPIO_POWER_OFF_FUNC
#else /* GPIO_POWER_OFF_FUNC */
        *pu4Data = (UINT32)GPIO_POWER_OFF_FUNC;
#endif /* GPIO_POWER_OFF_FUNC */
        return 0;
#endif        
    case eGpioLedCtrlFunc:
#ifdef GPIO_LED_CTRL_FUNC
        *pu4Data = (UINT32)GPIO_LED_CTRL_FUNC;
        return 0;
#else /* GPIO_LED_CTRL_FUNC */
        return -1;
#endif /* GPIO_LED_CTRL_FUNC */

    // USB vbus control.
#ifdef USB_VBUS_GPIO   
    case eUSBVbus0Gpio:
        *pu4Data = USB_VBUS_GPIO;
        return 0;
#endif        
#ifdef USB_VBUS_GPIO_POLARITY
    case eUSBVbus0EnablePolarity:
        *pu4Data = USB_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB Full speed only enable
#ifdef USB_FULL_SPEED_ONLY_ENABLE
    case eUSBFullSpeedOnlyEanble:
        *pu4Data = TRUE;
        return 0;
#endif

// Irrx settings
    case eIrrxType:
#ifndef DEFAULT_IRRX_TYPE
#error no define DEFAULT_IRRX_TYPE
#else /* DEFAULT_IRRX_TYPE */
        *pu4Data = DEFAULT_IRRX_TYPE;
#endif /* DEFAULT_IRRX_TYPE */
        return 0;
    case eIrrxNecKeyMap:
#ifndef IRRX_NEC_KEYMAP_ARRAY
#error no define IRRX_NEC_KEY_MAP_ARRAY
#else /* IRRX_NEC_KEYMAP_ARRAY */
        *pu4Data = (UINT32)IRRX_NEC_KEYMAP_ARRAY;
#endif /* IRRX_NEC_KEYMAP_ARRAY */
        return 0;
    case eIrrxNecMapArraySize:
#ifndef IRRX_NEC_MAPARRAY_SIZE
#error no define IRRX_NEC_MAPARRAY_SIZE
#else /* IRRX_NEC_MAPARRAY_SIZE */
        *pu4Data = (UINT32)IRRX_NEC_MAPARRAY_SIZE;
#endif /* IRRX_NEC_MAPARRAY_SIZE */
        return 0;
    case eIrrxRc5KeyMap:
#ifndef IRRX_RC5_KEYMAP_ARRAY
#error no define IRRX_RC5_KEYMAP_ARRAY
#else /* IRRX_RC5_KEYMAP_ARRAY */
        *pu4Data = (UINT32)IRRX_RC5_KEYMAP_ARRAY;
#endif /* IRRX_RC5_KEYMAP_ARRAY */
        return 0;
    case eIrrxRc5MapArraySize:
#ifndef IRRX_RC5_KEYMAP_SIZE
#error no define IRRX_RC5_KEYMAP_SIZE
#else /* IRRX_RC5_KEYMAP_SIZE */
        *pu4Data = (UINT32)IRRX_RC5_KEYMAP_SIZE;
#endif /* IRRX_RC5_KEYMAP_SIZE */
        return 0;

    case eIrrxKeyUpTimeout:
        *pu4Data = DEFAULT_IRRX_KEY_UP_TIMEOUT;
        return 0;
    case eIrrxRepeatValidTime:
        *pu4Data = DEFAULT_IRRX_REPEAT_VALID_TIME;
        return 0;
    case eIrrxNecPulse1:
        *pu4Data = (UINT32)IRRX_NEC_PULSE1;
        return 0;
    case eIrrxNecPulse2:
        *pu4Data = (UINT32)IRRX_NEC_PULSE2;
        return 0;
    case eIrrxNecPulse3:
        *pu4Data = (UINT32)IRRX_NEC_PULSE3;
        return 0;
    case eIrrxRc5KeyMapFunc:
    	*pu4Data = (UINT32)IRRX_RC5_KEYMAP_FUNC;
    	return 0;
    case eIrrxRc6KeyMapFunc:
    	*pu4Data = (UINT32)IRRX_RC6_KEYMAP_FUNC;
    	return 0;
    case eIrrxXferUsrToCrystalFunc:
        *pu4Data = (UINT32)IRRX_XFER_USR_TO_CRYSTAL_FUNC;
        return 0;
    case eIrrxSetUsrWakeupKeyFunc:
        *pu4Data = (UINT32)IRRX_SET_USR_WAKEUP_KEY_FUNC;
        return 0;
    case eIrrxUsrConfig:
        *pu4Data = (UINT32)IRRX_USR_CONFIG;
        return 0;
    case eIrrxUsrSaperiod:
        *pu4Data = (UINT32)IRRX_USR_SAPERIOD;
        return 0;
    case eIrrxUsrThreshold:
        *pu4Data = (UINT32)IRRX_USR_THRESHOLD;
        return 0;
    case eIrrxRepeatPulseUp:
        *pu4Data = (UINT32)IRRX_REPEAT_PULSE_UP;
        return 0;
    case eIrrxRepeatPulseDown:
        *pu4Data = (UINT32)IRRX_REPEAT_PULSE_DOWN;
        return 0;
    case eIrrxFlagBlinking:
        *pu4Data = (UINT32)IRRX_FLAG_IRRX_BLINKING;
        return 0;
    case eIrrxFlagPrgUpDownWakeup:
        *pu4Data = (UINT32)FLAG_IR_PRGUPDOWN_WAKEUP;
        return 0;
    case eIrrxFlagAllWakeup:
        *pu4Data = (UINT32)FLAG_IR_ALL_WAKEUP;
        return 0;

// Keypad
    case eFlagButtonSupport:
        *pu4Data = FLAG_BUTTON_SUPPORT;
        return 0;
    case eSingleGpioKeyList:
        *pu4Data = (UINT32)SINGLE_GPIO_KEY_LIST;
        return 0;
    case eSingleServoKeyList:
        *pu4Data = (UINT32)SINGLE_SERVO_KEY_LIST;
        return 0;
    case eGreedGpioKeyList:
        *pu4Data = (UINT32)GREED_GPIO_KEY_LIST;
        return 0;
    case eMultiKeyList:
        *pu4Data = (UINT32)MULTI_KEY_LIST;
        return 0;
    case eKeyPadWakeupBit:
        *pu4Data = (UINT32)KEYPAD_WAKEUP_BIT;
        return 0;
    case eKeyPadPollingTime:
        *pu4Data = (UINT32)KEYPAD_POLLING_TIME;
        return 0;
    case eKeyPadCheckTime:
        *pu4Data = (UINT32)KEYPAD_CHECK_TIME;
        return 0;

#ifdef AUDIO_STA335_RESET_GPIO
//STA335
    case eAudioSTA335ResetGpio:
        *pu4Data = AUDIO_STA335_RESET_GPIO;
        return 0;
#endif /* AUDIO_STA335_RESET_GPIO */
#ifdef AUDIO_STA335_RESET_GPIO_POLARITY
    case eAudioSTA335ResetGpioPolarity:
        *pu4Data = AUDIO_STA335_RESET_GPIO_POLARITY;
        return 0;
#endif /* AUDIO_STA335_RESET_GPIO_POLARITY */

// Other Gpio
    case eExternalStatus:
        *pu4Data = FLAG_EXT_STATUS;
        return 0;
    case eStandbySetGpio:
        *pu4Data = STANDBY_SET_GPIO;
        return 0;
    case ePowerStatusGpio:
        *pu4Data = POWER_STATUS_GPIO;
        return 0;
    case eStdbyUsePowerBtnCount:
        *pu4Data = STDBY_USE_PWR_BTN_CNT;
        return 0;
    case eStdbyUsePrevPowerState:
        *pu4Data = STDBY_USE_PREV_PWR_STATE;
        return 0;
    case eStdbyUseVGAStatus:
        *pu4Data = STDBY_USE_VGA_STATUS;
        return 0;
    case ePowerCtrlDelayMs:
        *pu4Data = POWER_CTRL_DELAY_MS;
        return 0;
    case ePowerCtrlGpio:
#ifdef POWER_CTRL_GPIO
        *pu4Data = POWER_CTRL_GPIO;
        return 0;
#else
        return -1;
#endif
    case ePowerCtrlOnVal:
#ifdef POWER_CTRL_ON_VAL
        *pu4Data = POWER_CTRL_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eDvdCtrlGpio:
#ifdef DVD_CTRL_GPIO
        *pu4Data = DVD_CTRL_GPIO;
        return 0;
#else
        return -1;
#endif
    case eDvdCtrlOnVal:
#ifdef DVD_CTRL_ON_VAL
        *pu4Data = DVD_CTRL_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eSpecialWakeupGpio:
#ifdef SPECIAL_WAKEUP_GPIO
        *pu4Data = SPECIAL_WAKEUP_GPIO;
        return 0;
#else
        return -1;
#endif
    case eSpecialWakeupOnVal:
#ifdef SPECIAL_WAKEUP_ON_VAL
        *pu4Data = SPECIAL_WAKEUP_ON_VAL;
        return 0;
#else
        return -1;
#endif

// Led
    case ePowerLedGpio:
#ifdef POWER_LED_GPIO
        *pu4Data = POWER_LED_GPIO;
        return 0;
#else
        return -1;
#endif
    case ePowerLedOnVal:
#ifdef POWER_LED_ON_VAL
        *pu4Data = POWER_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eBackupLedGpio:
#ifdef BACKUP_LED_GPIO
        *pu4Data = BACKUP_LED_GPIO;
        return 0;
#else
        return -1;
#endif
    case eBackupLedOnVal:
#ifdef BACKUP_LED_ON_VAL
        *pu4Data = BACKUP_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eSupportDarkLed:
        *pu4Data = FLAG_SUPPORT_DARK_LED;
        return 0;
    case ePowerLedDarkVal:
#ifdef POWER_LED_DARK_VAL
        *pu4Data = POWER_LED_DARK_VAL;
        return 0;
#else
        return -1;
#endif
    case eBackupLedDarkVal:
#ifdef BACKUP_LED_DARK_VAL
        *pu4Data = BACKUP_LED_DARK_VAL;
        return 0;
#else
        return -1;
#endif
    case eFlagSupportPowerLedOff:
        *pu4Data = (UINT32)FLAG_SUPPORT_POWER_LED_OFF;
        return 0;

#ifdef COMPANION_CHIP_GPIO_NUM
    case eCompanionChipGpio:
        *pu4Data = (UINT32)COMPANION_CHIP_GPIO_NUM;
        return 0;
#endif
#ifdef COMPANION_CHIP_GPIO_POLARITY
    case eCompanionChipGpioPolarity:
        *pu4Data = (UINT32)COMPANION_CHIP_GPIO_POLARITY;
        return 0;
#endif

    case ePdwncPolarity:
        *pu4Data = (UINT32)PDWNC_POLARITY;
        return 0;

#ifdef SW_POWER_DETECT_VOLTAGE        
    case eSWPowerDetectVoltage:
        *pu4Data = (UINT32)SW_POWER_DETECT_VOLTAGE;
        return 0;
#endif

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
    case eMt8280ResetGpio:
        *pu4Data = MT8280_RESET_GPIO;
        return 0;

    case eDemodResetGpio:
#ifdef DEFAULT_DEMOD_RESET_GPIO
        *pu4Data = (UINT32) DEFAULT_DEMOD_RESET_GPIO;
        return 0;
#else /* DEFAULT_DEMOD_RESET_GPIO */
        return -1;
#endif /* DEFAULT_DEMOD_RESET_GPIO */
#endif /* CC_5391_PRELOADER */

    // VGA wakeup setting by external controller
    case eVGAMaxHSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MAX_HSYNC_FREQ;
        return 0;
    case eVGAMinHSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MIN_HSYNC_FREQ;
        return 0;
    case eVGAMaxVSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MAX_VSYNC_FREQ;
        return 0;
    case eVGAMinVSyncFreqForExtCtrl:
        *pu4Data = (UINT32)VGA_EXT_MIN_VSYNC_FREQ;
        return 0;
        
    default:
        return -1;
    }
}

UINT32 DRVCUST_InitGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret;

    VERIFY(DRVCUST_InitQuery(eQryType, &u4Ret) == 0);
    return u4Ret;
}

#ifndef CC_5391_PRELOADER
void DRVCUST_BootupCheck()
{
// #if MT8280_RESET_GPIO
//     GPIO_SetOut(MT8280_RESET_GPIO, 1);
// #endif /* MT8280_RESET_GPIO */

#ifdef CUSTOM_BootupCheck
    CUSTOM_BootupCheck();
#endif /* CUSTOM_BootupCheck */
}

#ifndef CUSTOM_BootupMode
#define CUSTOM_BootupMode       (0)
#endif /* CUSTOM_BootupMode */

INT32 DRVCUST_GetBootupMode()
{
    return CUSTOM_BootupMode;
}
#endif /* CC_5391_PRELOADER */

#endif /* CC_NAND_BOOT */
