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
 * $RCSfile: drvcust_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drvcust_if.h
 *  Interface of driver customization parameter.
 */


#ifndef DRVCUST_IF_H
#define DRVCUST_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_lint.h"
#include "x_printf.h"


//-----------------------------------------------------------------------------
// Constant config
//-----------------------------------------------------------------------------
#define FBM_MEM_CFG_MT5391x128MB            (0x4000000)

#define FBM_MEM_CFG_MT5382pSSFullx64MB     (0x33C0000)  // 5382p 64MB for SS

#define FBM_MEM_CFG_MT5382pSSx64MB     (0x2DC0000)      // 5382p 64MB for SS

#define FBM_MEM_CFG_MT5382px64MB     (0x2AC0000)        // 5382p 64MB

#define FBM_MEM_CFG_MT5382x64MB     (0x2860000)         // 5382 64MB

#define FBM_MEM_CFG_MT5381px32MB     (0x01420000)       // 5381p 32MB

#define FBM_MEM_CFG_MT5381x32MB     (0x01410000)    // 5381 32MB

#define FBM_MEM_CFG_MT5380FHDx32MB (0x0140F000)         // 5380 FHD

#define FBM_MEM_CFG_MT5380x32MB     (0x00E10000)    // 5380 32MB

#define FBM_MEM_CFG_MT5380_3D3D     (0x00D20000)    // 5380 32MB 3D3D Model

#define FBM_MEM_CFG_MT5380x16MB     (0x00730000)    // 5380 16MB

#define FBM_MEM_CFG_MT5337x64MB     (0x02490000)    // 5337 64MB

#define FBM_MEM_CFG_MT5336x32MB     (0x00F10000)    // 5336 32MB

#define FBM_MEM_CFG_MT5336_NOPIP     (0x00950000)    // 5336 32MB (no PIP, Panel=FHD)

#define FBM_MEM_CFG_MT5335x32MB     (0x00E0a000)    // 5335 32MB

#define FBM_MEM_CFG_MT5335_NOVOG     (0x00838000)    // 5335 32MB (no VOG, Panel=1680x1050)

#define FBM_MEM_CFG_MT5382PSLT       (0x3800000)    // 5382p SLT (128MB)

#ifdef __MODEL_slt__
#define FBM_MEM_CFG_MT5382SLT     	 (0x3400000)    // 5382 SLT (128MB)
#else
#define FBM_MEM_CFG_MT5382SLT     	 (0x3300000)    // 5382 SLT (128MB)
#endif

#define FBM_MEM_CFG_MT5380SLT       (0x01480000)    // 5380 SLT (32MB)

// Audio volume define
#define VOL_POS_14_DB               (14*2)
#define VOL_POS_11_DB               (11*2)
#define VOL_POS_10_DB               (10*2)
#define VOL_POS_9_5_DB              (9*2+1)
#define VOL_POS_6_DB                 (6*2)
#define VOL_POS_3_DB                 (3*2)
#define VOL_0_DB                                (0)
#define VOL_NEG_3_DB                    (-3*2)
#define VOL_NEG_6_DB                    (-6*2)
#define VOL_NEG_12_DB               (-12*2)


#define SCPOS_SPECIAL_VGA_DISABLE           0
#define SCPOS_SPECIAL_VGA_USE_DRAM      1   //both Main/Sub go through normal path
#define SCPOS_SPECIAL_VGA_USE_DISPMODE  2   //main=>display mode, sub=>normal mode

#define PANEL_CHANNEL_SWAP_ALWAYS 1
#define PANEL_CHANNEL_SWAP_IN_SIGNAL_PORT 2
#define PANEL_CHANNEL_SWAP_IN_DUAL_PORT 3

#define SCART_FB_IN_SOY0 0
#define SCART_FB_IN_SOY1 1
#define SCART_FB_IN_SOG 2

#define SCART_FULL_TYPE	0 //Scart 1 &2 : CVBS/S-Video/RGB
#define SCART_HALF_TYPE	1 //Scart 1 : CVBS/RGB, Scart2: CVBS/S-Video

//-----------------------------------------------------------------------------
// Enum constant
//-----------------------------------------------------------------------------

#ifdef CC_NAND_BOOT

#include "c_model.h"
#include "../drv_cust/drv_default.h"

#define DRVCUST_InitGet(x)      (x)
#define eDdrClock               (DEFAULT_DDR_CLOCK)
#define eDramType               (DEFAULT_DRAM_TYPE)
#define eDramColAddr            (DEFAULT_DRAM_COLADDR)
#define eFlagDDRQfp             (FLAG_DDR_QFP)
#define eFlagForce32bit         (FLAG_FORCE_32BIT)
#define eFlagSingleDQS          (FLAG_SINGLE_DQS)
#define eFlagFullFreq           (FLAG_FULL_FREQ)
#define eFlagSamsungGDDR        (FLAG_SAMSUNG_GDDR)
#define eDdrCL                  (DEFAULT_DDR_CL)

typedef UINT32 QUERY_TYPE_T;

#else /* CC_NAND_BOOT */

typedef enum {
    eZeroStart,                 // Default Value

    // Common information
    eBuildDate,                 // __DATE__
    eBuildTime,                 // __TIME__
    eBrdCstIf,                  // $(BRDCST_IF)
    eDrvCust,                   // $(DRV_CUST)

    // Initial configuration
    eFlagBootVerbose,           // 0
    eNorCpuClock,               // 162000000
    eDramType,                  // DDR1x2
    eDramColAddr,               // COL_ADDR_BIT_10
    eDramParamE,                // 0
    eDdrClock,                  // 297000000
    eDdrCL,                     // 6
    eFlagDmpllSpectrum,         // 0
    eDmpllSpectrumPermillage,   // 0
    eFlagSamsungGDDR,           // 0
    eFlag1GBitSupport,          // 0
    eFlagForce32bit,            // 0
    eFlagFullFreq,              // 1
    eFlagDDRQfp,                // 0
    eFlagSingleDQS,             // 1
    eSysResetTimer,             // 0xf00000.  (15/27 second)

    // Tuner driver customization.
    eTunerCtrl,
    eTunerCtrlAna,
    eTunerTypeDefault,
    eTunerTypeOfst0,
    eTunerTypeOfst1,
    eTunerTypeOfst2,
    eDemodResetGpio,

    // Dram setting
    eDramAgentPriority,         // priority list
    eDramBurstLen,              // 15
    eDramGroup1ArbiterTime,     // 5
    eDramGroup2ArbiterTime,     // 4
    eDramGroup3ArbiterTime,     // 15

    // Release version setting
    eReleaseEnableWatchDog,     // 0, only enable at MP.
    eReleaseWatchdogTimeout,    // 5 seconds.
    eReleaseWatchdogPriority,   // 100

    // Memory setting
    eFbmMemAddr,                // g_u4DramSize - FBM_MEM_CFG_SIZE - 0x4000
    eFbmMemSize,                // 0x1300000

    // PDWNC setting
    ePdwncPolarity,             // 0
    ePcdWakeupTheKey,           // XXX
    eWakeupReasonGpio,          // 0, 0 is disable.
    eWakeupReasonGpioPolarity,  // 0
    eSWPowerDetectVoltage,  

    // USB upgrade setting
    eAutoUsbUpgradeEnable,      // 0
    eUsbUpgradeFileName,        // upgrade.pkg
    eLoaderUpgradeVerCmpFunc,   // NULL

    // Loader logo
    eLoaderLogoNumber,          // 0
    eLoaderLogoBackground,      // 0xeeeeee
    eLoaderLogoFlashAddr,       // 0
    eLoaderLogoColorMode,       // 0xd
    eLoaderLogoWidthArray,      // NULL
    eLoaderLogoHightArray,      // NULL
    eLoaderLogoDataIdx0,        // LOADER_LOGO_DATAIDX0 in XXX_bootlogo.h
    eLoaderLogoDataIdx1,        // LOADER_LOGO_DATAIDX1 in XXX_bootlogo.h
    eLoaderLogoDataIdx2,        // LOADER_LOGO_DATAIDX2 in XXX_bootlogo.h
    eLoaderLogoDataIdx3,        // LOADER_LOGO_DATAIDX3 in XXX_bootlogo.h
    eLoaderLogoDataIdx4,        // LOADER_LOGO_DATAIDX4 in XXX_bootlogo.h
    eLoaderLogoDataIdx5,        // LOADER_LOGO_DATAIDX5 in XXX_bootlogo.h
    eLoaderLogoDataIdx6,        // LOADER_LOGO_DATAIDX6 in XXX_bootlogo.h
    eLoaderLogoDataIdx7,        // LOADER_LOGO_DATAIDX7 in XXX_bootlogo.h
    eLoaderLogoDataIdx8,        // LOADER_LOGO_DATAIDX8 in XXX_bootlogo.h
    eLoaderLogoDataIdx9,        // LOADER_LOGO_DATAIDX9 in XXX_bootlogo.h

    // Panel customization
    eDefaultPanelSelect,        // PANEL_DEFAULT
    eLvdsMsbSw,                 // option
    eBackLightGpio,             // 69
    eBackLightGpio2,            // option
    eBackLightOnVal,            // 0
    ePanelPowerGpio,            // Gpio 3
    ePanelPowerOnVal,           // 0
    ePanelDCRGpio,              // Gpio 5
    ePanelDCROnVal,             // 1
    ePanelPdpVsGpio,            // Gpio 3
    ePanelPdpVsOnVal,           // 0
    eTimerDimmingFreq,          // 180~260 (Hz)
    eDimmingFreq60,             // hz
    eDimmingFreq50,             // hz
    eBacklightHigh,             // 0~255
    eBacklightMiddle,           // 0~255
    eBacklightLow,              // 0~255
    eSpreadSpectrumPermillage,  // 0~1000 (0/00)
    ePanelPwmPolarity,          // PWM_LOW_PANEL_BRIGHT
    ePanelPwmPrescalar,         // set bTimeBase to control PWM freq
    ePanelIndexFromEeprom,      // option
    ePanelIndexOffset1,         // option
    ePanelIndexOffset2,         // option
    ePanelIndexOffset3,         // option
    ePanelDisableDimming,       // disable using PWM1, let gpio68 free
    ePanelChannelSwap,          // change even and odd channel
    ePanelIndexFromGpioNum,     // option
    ePanelIndexFromGpioIdx,     // option
    ePanelIndexFromGpioMap,     // option
    ePanelTTLOutput,            // option
    ePanelLvds8bitPinMap,       // option
    ePanelLvds10bitPinMap,      // option
    ePanelFlashTableSize,       // option
    ePanelFlashTableAddress,    // option
    ePanelEepromToFlashIndexMap,// option
    eLvdsDrivingCurrent,        // option
    ePanelBacklightPwmPort,

    // Audio customization
    eAudioOutPadNormal,
    eAudioOutPadDrvCur,
    eAudioSpkType,
    eAudioSurroundSpkType,
    eAudioCenterSpkType,
    eAudioSubwooferOn,
    eAudioBassMngCutOffFreq,
    eAudioSoftMuteOrder,
    eAudioBbeLevel,
    eAudioBbeProcess,
    eAudioBbeLoContour,
    eAudioBbe3dGain,
    eAudioBypassPostProc,
    eAudioDacMclk,
    eAudioDacUsage,
    eAudioInterDacChlConfig,
    eAudioAoutPadConfig,
    eAudioOpctrlMuteGpioNum,
    eAudioOpctrlMuteGpioPolarity,
    eAudioMuteGpioNum,
    eAudioAmpEnableGpioNum,
    eAudioMuteGpioPolarity,
    eAudioAmpGpioPolarity,
    eAudioInputSwGpioSel,
    eAudioInputMuxSelTable,
    eAudioOutFmtTable,
    eAudioInFmtTable,
    eAudioInputVolTable,
    eAudioAvInputSrcVolTable,
    eAudioAvChlEqTable,
    eAudioInputSrcAgcFlag,
    eAudioInputSrcAgcVol,
    eAudioVolBaseLevel,
    eAudioVolTable,
    eAudioChlVolExtraGain,
    eAudioChannelDelay,
    eAudioTableCfg,
    eAudioAdacCfg,
    eAudioHeadphonePlugInGpio,
    eAudioHeadPhonePlugInPolarity,
    eAudioHeadPhonePlugInLineOutMute,
    eAudioDcDetectGpio,
    eAudioDcDetectPolarity,
    eAudioLineOutUseInterDac,
    eAudioMuteAllSource,
    eAudioDvdGpio,
    eAudioDvdPolarity,
    eAudioDvdInputChl,
    eAudioSpkAutoOffWhenVolMute,
    eAudioCustSpecGpioCtrl,
    eAudioCustSpec2GpioCtrl,
    eAudioCustHpGpioCtrl,
    eAudioSpecGpioPolarity,
    eAudioSpec2GpioPolarity,
    eAudioHpGpioPolarity,
    eAudioSpkPath,
    eAudioAvcTarget,
    eAudioAvcGainUp,
    eAudioAvcAttackThres,
    eAudioAvcAdjustRate,
    eAudioMT8292PGA0GAIN,
    eAudioMT8292PGA1GAIN,
    eAudioMT8292PGA2GAIN,
    eAudioMT8292HPGAIN,
    eAudioLineInLrSwap,
    eAudioMtsFactory,
    eAudioA2Factory,
    eAudioPalFactory,
    eAudioNvmEnable,
    eAudioNvmLength,
    eAudioNvmData,
    eAudioProfileDefault,
    eAudioProfileStart,
    eAudioProfileEnd,
    eAudioLimiterMode,                          // 0: off, 1: Adaptive mode, 2: Fix mode
    eAudioDigitalDataConfig,
    eAudioTunerATD,
    eAudioSTA335ResetGpio,
    eAudioSTA335ResetGpioPolarity,
    eAudioAL1AR1PhaseInverse,
    eAudioAL2AR2PhaseInverse,
    eAudioDigiAMPResetGpio,
    eAudioDigiAMPResetGpioPolarity,
    

    // Other GPIO customization
    eExternalStatus,            // 0    bool, 1 to enable loader standby at AC on.
    eStandbySetGpio,            // 207  AC-on detect circuit, only work on IC_AB.
    ePowerStatusGpio,           // 206  AC-on detect circuit, only work on IC_AB.
    eStdbyUsePowerBtnCount,     // 0    Use Power button State to set standby
    eStdbyUsePrevPowerState,    // 0    Use previous power state to set standby
    eStdbyUseVGAStatus,         // 0    Use VGA status to enter standby mode when power key trigger.

    // Boot/PowerDown setup GPIOs, boot set OnVal, PowerDown set !OnVal.
    ePowerLedGpio,              // NULL
    ePowerLedOnVal,             // 0
    eBackupLedGpio,             // NULL
    eBackupLedOnVal,            // 0
    ePowerCtrlDelayMs,          // 5 ms.
    ePowerCtrlGpio,             // NULL
    ePowerCtrlOnVal,            // 0
    eDvdCtrlGpio,               // NULL
    eDvdCtrlOnVal,              // 0
    eSupportDarkLed,            // 0
    ePowerLedDarkVal,           // XXX
    eBackupLedDarkVal,          // XXX
    eSpecialWakeupGpio,         // XXX
    eSpecialWakeupOnVal,        // XXX
    eGpioPowerOnFunc,           // GPIO_MtkPowerOnFunc()
    eGpioPowerOffFunc,          // GPIO_MtkPowerOffFunc()
    eGpioLedCtrlFunc,           // GPIO_MtkLedCtrlFunc()
    eFlagSupportPowerLedOff,    // 0
    eCompanionChipGpio,         // 0    Check Companion chip existed gpio
    eCompanionChipGpioPolarity, // 0

    // MT8292 GPIO support
    eFlagMt8292GpioSupport,         // 0
    eMt8292ResetGpio,
    eMt8292MuteGpio,

    // MT8280 GPIO
    eMt8280ResetGpio,               // 0

    // EEPROM customization
    eSystemEepromSize,          // 0x800 => 2048 KB.
    eSysEepromWPGpio,           // GPIO(1)
    eSysEepromWPEnablePolarity,
    eEdidEepromWPGpio,          // GPIO(1)
    eEdidEepromWPEnablePolarity,
    eSystemEepromClkDiv,        // 0x200 => 27 Mhz/512.
    eEepromHdmiHdcpOffset ,
    eEepromHdmiHdcpSize ,
    eEepromDtvDriverOffset,
    eEepromDtvDriverSize,
    eEepromNptvDriverOffset,
    eEepromNptvDriverSize,
    eEepromDriverSize,
    eEepromErrorCodeOffset,
    eEepromErrorCodeSize,
    eEepromStateCodeOffset,
    eEepromStateCodeSize,

    // NOR flash customization
    eNorFlashTotalSize,             // 4*1024*1024 = 4 Mbyte
    eNorFlashLoaderSize,          // 512*1024 =  512 Kbyte
    eNorFlashAppImageSize,    // 3*1024*1024 = 3 Mbyte
    eNorFlashDynamicPartitionSize,    // 3*1024*1024 = 3 Mbyte
    eNorFlashStaticPartitionSize,    // 3*1024*1024 = 3 Mbyte
    eHDCPNorPartId,                 // 0, if 0/1, HDCP on EEPROM, otherwise on NORFlash.

    // NOR flash partition size customization
    eNorFlashPartSize0,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize1,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize2,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize3,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize4,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize5,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize6,             // if size == 64KB, this partition type = dynamic type.
    eNorFlashPartSize7,             // if size == 64KB, this partition type = dynamic type.

    // USB VBUS0 enable
    eUSBVbus0Gpio,
    eUSBVbus0EnablePolarity,

    // USB Full speed only enable
    eUSBFullSpeedOnlyEanble,

    // FCI card detect pin
    eFCICardDetectGpio,

    // Button customization
    eFlagButtonSupport,         // 0
    eSingleGpioKeyList,         // NULL
    eSingleServoKeyList,        // NULL
    eGreedGpioKeyList,          // NULL
    eMultiKeyList,
    eKeyPadWakeupBit,           // DKBIT_POWER
    eKeyPadPollingTime,         // 50ms
    eKeyPadCheckTime,           // 2

    // P/I Gain customization
    ePIGainSetting, //PIGain table Modify by W.C Shih
    ePIGainTLSize,  //PIGain table Modify by W.C Shih

    // Auto Check Polarity
    ePolitaryCheck, // VGA Politary Check Modify by W.C Shih2006/10/30

    eVGAADSpec, // VGA AD Spec Modify by W.C Shih 2006/12/01
    eVGAHFreqSpecHeight, // VGA H-Frequence Modify by W.C Shih 2007/01/08
    eVGAHFreqSpecLow,// VGA H-Frequence Modify by W.C Shih 2007/01/08
    eVGAVFreqSpecHeight, // VGA H-Frequence Modify by W.C Shih 2007/01/08
    eVGAVFreqSpecLow,// VGA H-Frequence Modify by W.C Shih 2007/01/08

    // IRRX customization
    eIrrxType,                  // NEC
    eIrrxKeyUpTimeout,          // 200 ms.
    eIrrxRepeatValidTime,       // 200 ms.
    eIrrxNecKeyMap,             // All NEC key maps
    eIrrxNecMapArraySize,       //
    eIrrxNecPulse1,             // 8, 0xff to disable
    eIrrxNecPulse2,             // 0, 0xff to disable
    eIrrxNecPulse3,             // 0, 0xff to disable
    eIrrxRc5KeyMap,             // All RC5 key maps
    eIrrxRc5MapArraySize,       //
    eIrrxRc5KeyMapFunc,         // NULL, Key map function
    eIrrxRc6KeyMapFunc,         // NULL, Key map function
    eIrrxXferUsrToCrystalFunc,  // NULL, Raw data to Key xfer function.
    eIrrxSetUsrWakeupKeyFunc,   // NULL, Set wakeup key in PDWNC register
    eIrrxFlagBlinking,          // 0, flag to blink led when ir receiving.
    eIrrxFlagPrgUpDownWakeup,   // 0, flag to enable IR CH+/- wakeup.
    eIrrxFlagAllWakeup,         // 0, flag to enable all key wakeup.
    eIrrxUsrConfig,             // 0x4121
    eIrrxUsrSaperiod,           // 0x0032
    eIrrxUsrThreshold,          // 0x0001
    eIrrxRepeatPulseUp,         // 5
    eIrrxRepeatPulseDown,       // 2

    // Factory
    eFlagFactoryEdid,           // 0
    eFlagFactoryMode2,          // 0
    eMagicMaxInputForceFactory,    // NULL, define the magic char to force factory mode. must > 0x80
    eMagicMinInputForceFactory,    // NULL, define the magic char to force factory mode. must < 0x20

    // SCPOS_SUPPORT_SPECIAL_VGA
    eFlagScposSpecalVGA,        // 0

    // Non-linear Scaling Factor
    eFlagScposFixedNonLinearFactor,
    eFlagScposNonLinearSlope,
    eFlagScposNonLinearEndPointRatio,

    // VGA Max Pixel Rate for Dispmode
    eVGAMaxRateForDispmode,
    
    // VGA Wakeup setting by the external controller
    eVGAMaxHSyncFreqForExtCtrl,
    eVGAMinHSyncFreqForExtCtrl,
    eVGAMaxVSyncFreqForExtCtrl,
    eVGAMinVSyncFreqForExtCtrl,

    // Quality customization
    eFlagSCCGamma,
    eFlagDirectShiftHue,
    eFlagLinkAdpLumaWithAdpBacklight,
    eAdpBacklightAplDark,
    eAdpBacklightAplBright,
    eFlagVGAColorMatrix,    //VGA Color Process
    eFlagAutoNR,
    eFlagFlashPqEnable,
    eFlashPqBaseAddress,
    eFlashPqOffset,
    eFlashPqBlockSize,
    eFlagFlashPqMultiGamma,
    eFlagFlashPqMultiSCE,
    eFlagFlashPqMultiQty,
	eFlagFlashPqMultiSmartPic,
	eFlagFlashPqSmartPicOffset,
    eFlagGain1AsColorTemp,

    // RTC external H/W Option
    eRtcHw,
    eRtcExtIntGpio,
    eRtcExtClkDiv,

    // SCART FS Status Servo ADC
    eScart1FSServoAdc,
    eScart2FSServoAdc,
     //CEC
     eCECFunctionPinmuxEn,
    // HDMI
    eHDMISupportExtSwitch,
    eHDMIBypassInitialFlow,
    eHDMIOffOnMuteCount,
    eDVIWaitStableCount,
    eDVIWaitNosignalCount,
    eDVIWaitSCDTStableCount,
    eHDMIEQZEROVALUE,  // josh
    eHDMIEQBOOSTVALUE, // josh
    // Run on NOR flash or not
    eFlagRunOnNor,

    // TVD
    eTVDSupportNTSCOnly,
    eTVDCCNRLThresholdBlock,    //block CC if NRL beyong this,  (range: 0~80)
    eTVDCCNRLThresholdUnblock,  //unblock CC if NRL below this, (range: 0~eTVDCCNRLThresholdBlock)
    eTVDCCPERThresholdBlock,    //block CC if PER beyong this,  (range: 0~128)
    eTVDCCPERThresholdUnblock,  //unblock CC if PER below this, (range: 0~eTVDCCPERThresholdBlock)

    // SCART
    eSCARTFBInSelect,
    eScartType,

    // MT8295
    eMT8295ResetGpio,
    eCI_PMX_CFG,
    eMT8295IntGpio,

    // UART
    eUART1Support,

   // SCART RGB
    eSCARTRGBOrder,
    eSCARTScRExt,

   // ON_CHIP_AUTO_COLOR
    eOnChipAutoColor,
    eMtkPublicVersion,
    eOnChipYPbPrCounterAddr,
    eOnChipVGACounterAddr,
    eOnChipOffsetCounter,
   

    eLastOne
} QUERY_TYPE_T;

#endif /* CC_NAND_BOOT */


typedef enum _LVDS_MODE
{
    eLvdsVidNormal        = 0,  // (mode 0) Video normal mode
    eLvdsVidYuv422OnePort = 1,  // (mode 1) Video YUV 422 one-channel
    eLvdsVidRgb444OnePort = 2,  // (mode 2) Video RGB 444 one-channel
    eLvdsVidYuv422TwoPort = 3,  // (mode 3) Video YUV 422 two-channel
    eLvdsVidRgb444TwoPort = 4   // (mode 4) Video RGB 444 two-channel
} LVDS_MODE_T;

typedef enum _LVDS_COLOR_DOMAIN
{
	eColorDomainYUV =1,
	eColorDomainRGB =2
}LVDS_COLOR_DOMAIN_T;

//-----------------------------------------------------------------------------
// Dram enum
//-----------------------------------------------------------------------------

enum {
    COL_ADDR_MIN = 7,
    COL_ADDR_BIT_8,     ///< Dram chip column address 8 bit setting
    COL_ADDR_BIT_9,     ///< Dram chip column address 9 bit setting
    COL_ADDR_BIT_10,    ///< Dram chip column address 10 bit setting
    COL_ADDR_BIT_11,    ///< Dram chip column address 10 bit setting
    COL_ADDR_MAX
};

// DDR type definitions
enum {
    DDR_MIN_VAL = 0,
    DDR_I_x4,           ///< Dram configuration for DDRI four dram chips
    DDR_I_x2,           ///< Dram configuration for DDRI two dram chips
    DDR_I_x1,           ///< Dram configuration for DDRI one dram chip
    DDR_II_x4,          ///< Dram configuration for DDRII four dram chips
    DDR_II_x2,          ///< Dram configuration for DDRII two dram chips
    DDR_II_x1,          ///< Dram configuration for DDRII one dram chip
    DDR_MAX_VAL
};

enum
{
    LVDS_A0 = 0,
    LVDS_A1 = 1,
    LVDS_A2 = 2,
    LVDS_A3 = 3,
    LVDS_A4 = 4,
    LVDS_A5 = 5,
    LVDS_A6 = 6,
    LVDS_A7 = 7,
    LVDS_A8 = 8,
    LVDS_A9 = 9,
    LVDS_ACLK1 = 0xa,
    LVDS_ACLK2 = 0xb,
    LVDS_DEFAULT = 0xf
};


//-----------------------------------------------------------------------------
// GPIO & ServoADC pins
//-----------------------------------------------------------------------------

// Total number definitions
#define TOTAL_SERVOADC_NUMBER       (5)
#define TOTAL_GPIO_NUMBER           (186)
#define TOTAL_MT8292_GPIO_NUMBER    (10)
#define TOTAL_MT8295_GPIO_NUMBER    (15)
#define TOTAL_OPCTRL_NUMBER         (12)

// ServoADC
#define MAX_SERVOADC_NUM            TOTAL_SERVOADC_NUMBER

// GPIO alias
#define GPIO_OPWM1              (54)
#define GPIO_OPWM2              (55)
#define GPIO(x)                 (x)
#define ADIN(x)                 (SERVO(x))
#define SERVO(x)                (SERVO0 + (x))
#define OPCTRL(x)               (PWR_GPIO0 + (x))
#define MT8292_GPIO(x)          (MT8292_GPIO0 + (x))
#define MT8295_GPIO(x)          (MT8295_GPIO0 + (x))

// PDWNC PWR GPIO 0~7
#define PWR_GPIO0               (200)           ///< define gpio macro for OPCTRL0
#define PWR_GPIO1               (201)           ///< define gpio macro for OPCTRL1
#define PWR_GPIO2               (202)           ///< define gpio macro for OPCTRL2
#define PWR_GPIO3               (203)           ///< define gpio macro for OPCTRL3
#define PWR_GPIO4               (204)           ///< define gpio macro for OPCTRL4
#define PWR_GPIO5               (205)           ///< define gpio macro for OPCTRL5
#define PWR_GPIO6               (206)           ///< define gpio macro for OPCTRL6
#define PWR_GPIO7               (207)           ///< define gpio macro for OPCTRL7
#define PWR_GPIO8               (208)           ///< define gpio macro for OPCTRL8/OPWR2_5V

#define OPCTRL0                 PWR_GPIO0       ///< define gpio macro for OPCTRL0
#define OPCTRL1                 PWR_GPIO1       ///< define gpio macro for OPCTRL1
#define OPCTRL2                 PWR_GPIO2       ///< define gpio macro for OPCTRL2
#define OPCTRL3                 PWR_GPIO3       ///< define gpio macro for OPCTRL3
#define OPCTRL4                 PWR_GPIO4       ///< define gpio macro for OPCTRL4
#define OPCTRL5                 PWR_GPIO5       ///< define gpio macro for OPCTRL5
#define OPCTRL6                 PWR_GPIO6       ///< define gpio macro for OPCTRL6
#define OPCTRL7                 PWR_GPIO7       ///< define gpio macro for OPCTRL7
#define OPWR0_5V                PWR_GPIO6       ///< define gpio macro for OPWR0_5V
#define OPWR1_5V                PWR_GPIO7       ///< define gpio macro for OPWR1_5V
#define OPWR2_5V                PWR_GPIO8       ///< define gpio macro for OPCTRL8/OPWR2_5V

#if 1
#define SERVO0                  (400)
#define SERVO1                  (401)
#define SERVO2                  (402)
#define SERVO3                  (403)
#define SERVO4                  (404)

#define SERVO_GPIO0             (400)
#define SERVO_GPIO1             (401)
#define SERVO_GPIO2             (402)
#define SERVO_GPIO3             (403)
#define SERVO_GPIO4             (404)

#define ADIN0                   SERVO(0)        ///< define ADIN0
#define ADIN1                   SERVO(1)        ///< define ADIN1
#define ADIN2                   SERVO(2)        ///< define ADIN2
#define ADIN3                   SERVO(3)        ///< define ADIN3
#define ADIN4                   SERVO(4)        ///< define ADIN4
#endif

#define MT8292_GPIO0	(600)
#define MT8292_GPIO1	(601)
#define MT8292_GPIO2	(602)
#define MT8292_GPIO3	(603)
#define MT8292_GPIO4	(604)
#define MT8292_GPIO5	(605)
#define MT8292_GPIO6	(606)
#define MT8292_GPIO7	(607)
#define MT8292_GPIO8	(608)
#define MT8292_GPIO9	(609)

//MT8295
#define MT8295_GPIO0	(900)
#define MT8295_GPIO1	(901)
#define MT8295_GPIO2	(902)
#define MT8295_GPIO3	(903)
#define MT8295_GPIO4	(904)
#define MT8295_GPIO5	(905)
#define MT8295_GPIO6	(906)
#define MT8295_GPIO7	(907)
#define MT8295_GPIO8	(908)
#define MT8295_GPIO9	(909)
#define MT8295_GPIO10	(910)
#define MT8295_GPIO11	(911)
#define MT8295_GPIO12	(912)
#define MT8295_GPIO13	(913)
#define MT8295_GPIO14	(914)
#define MT8295_GPIO_MAX	MT8295_GPIO14

#define GPIO_INIT_CUSTOM_MODE0      (0)     // normal mode
#define GPIO_INIT_CUSTOM_MODE1      (1)     // special panel mode
#define GPIO_INIT_CUSTOM_MODE2      (2)     // panel mode
#define GPIO_INIT_CUSTOM_MODE3      (3)     // sdm mode
#define GPIO_INIT_CUSTOM_MASK       (0xfU)
#define GET_INIT_CUSTOM_MODE()      (DRVCUST_GetBootupMode() & GPIO_INIT_CUSTOM_MASK)
#define GPIO_INIT_COMPANION_MASK    (0xffff0000)
#define GPIO_INIT_COMPANION_CHIP    (0x82800000)
#define IS_COMPANION_CHIP_ON()      ((DRVCUST_GetBootupMode() & GPIO_INIT_COMPANION_MASK) == GPIO_INIT_COMPANION_CHIP)

typedef enum {
    eLedZero,
    eLedLoaderPowerOn,
    eLedLoaderPowerOff,
    eLedSystemPowerOn,
    eLedSystemPowerOff,
    eLedDarkLed,
    eLedIrOn,
    eLedIrOff,
    eLedVgaStandby,
    eLedUpgradeBlink,
    eLedLast
} LED_STATUS_T;

//-----------------------------------------------------------------------------
// Driver Key Pad data structure
//-----------------------------------------------------------------------------

#define DKBIT_POWER         (1U << 0)   ///< Driver internal power button id
#define DKBIT_MENU          (1U << 1)   ///< Driver internal menu button id
#define DKBIT_INPUT_SRC     (1U << 2)   ///< Driver internal input soruce button id
#define DKBIT_PRG_UP        (1U << 3)   ///< Driver internal channel up button id
#define DKBIT_PRG_DOWN      (1U << 4)   ///< Driver internal channel down button id
#define DKBIT_VOL_UP        (1U << 5)   ///< Driver internal volume up button id
#define DKBIT_VOL_DOWN      (1U << 6)   ///< Driver internal volume down button id
#define DKBIT_EJECT         (1U << 7)   ///< Driver internal dvd eject button id
#define DKBIT_PLAY_PAUSE    (1U << 8)   ///< Driver internal dvd play/pause button id
#define DKBIT_STOP          (1U << 9)   ///< Driver internal dvd stop button id
#define DKBIT_FACTORY       (1U << 10)  ///< Driver internal factory button id
#define DKBIT_ENTER         (1U << 11)  ///< Driver internal enter button id
#define DKBIT_NONE          (0xffffffff)    ///< Driver internal button id for none

typedef struct _MultiKey {
    UINT32 u4CrystalKey;    ///< System internal key id.
    UINT32 u4DrvKeyBits;    ///< Driver internal button id.
} MULTI_KEY_T;

//-----------------------------------------------------------------------------
// GPIO Key Pad data structure
//-----------------------------------------------------------------------------

typedef struct _SingleGpio {
    UINT32 u4Gpio;          ///< Gpio number
    UINT32 u4Val;           ///< Gpio polarity
    UINT32 u4Key;           ///< Gpio pin/polarity for System internal key id.
} SGL_GPIO_T;


//-----------------------------------------------------------------------------
// ServoADC Key Pad data structure
//-----------------------------------------------------------------------------

typedef struct _SingleServo {
    UINT32 u4Servo;         ///< ServoADC pin
    UINT32 u4Top;           ///< ServoADC top value
    UINT32 u4Btm;           ///< ServoADC button value
    UINT32 u4Key;           ///< ServoADC pin/range for System internal key id.
} SGL_SERVO_T;


//-----------------------------------------------------------------------------
// Greed GPIO Key Pad data structure
//-----------------------------------------------------------------------------

typedef struct _GreedKey {
    UINT32 u4GpioOut;       ///< Greed GPIO output pin
    UINT32 u4Val;           ///< Greed GPIO polarity
    UINT32 u4GpioIn;        ///< Greed GPIO input pin
    UINT32 u4Key;           ///< System key for this Greed GPIO setting.
} GREED_KEY_T;

///< GPIO power function pointer type.
typedef void (* PFN_GPIO_POWER_FUNC)(void);
///< GPIO setup at power on stage function pointer type.
typedef void (* PFN_GPIO_POWERON_FUNC)(void);
///< GPIO setup at power off stage function pointer type.
typedef void (* PFN_GPIO_POWEROFF_FUNC)(UINT32);
///< GPIO setup at led status function pointer type.
typedef void (* PFN_GPIO_LEDCTRL_FUNC)(LED_STATUS_T);

//-----------------------------------------------------------------------------
// IRRX Key Map data structure and Key Map array.
//-----------------------------------------------------------------------------

#define BTN_NONE                ((UINT32) 0xffffffff)

enum {
    IRRX_NEC_TYPE = 1,  ///< IR receiver NEC type setting
    IRRX_RC5_TYPE,      ///< IR receiver RC5 type setting
    IRRX_USR_TYPE,      ///< IR receiver User define type.
    IRRX_END_TYPE
};

/*! type define for IR key unit.
 */
typedef struct _KeyMapUnit {
    UINT32  u4Key;          // Only Use 8bits.
    UINT32  u4Crystal;
} KEYUNIT_T;

/*! type define for key table with group id and key table size.
 */
typedef struct _KeyArray {
    UINT32 u4GroupId;
    UINT32 u4Size;
    const KEYUNIT_T *prKeyMap;
} KEYARRAY_T;

/*! function pointer for user define type.
 */
typedef UINT32 (* PFN_XFER_USR_TO_CRYSTAL)(UINT32 u4Into, const UINT8 *pu1Data);

/*! function type for user define wakeup function pointer.
 */
typedef void (* PFN_SET_USR_WAKEUP_KEY)(void);

//-----------------------------------------------------------------------------
//PIGain table Modify by W.C Shih
//-----------------------------------------------------------------------------
typedef struct _PIUnit{
    UINT8 LowerBound;
    UINT8 UpperBound;
    UINT8 PValue;
    UINT8 IValue;
}PIUNIT_T;


//-----------------------------------------------------------------------------
// VGA table structure
//-----------------------------------------------------------------------------
#ifndef _VGAMODE_STRUCT_
#define _VGAMODE_STRUCT_
typedef struct VGAMODE      // 14 bytes
{
    UINT16 IHF; // Horizontal Frequency for timing search
    UINT8 IVF; // Vertical Frequency for timing search
    UINT16 ICLK; // Pixel Frequency
    UINT16 IHTOTAL; // H Total
    UINT16 IVTOTAL; // V Total
    UINT16 IPH_STA; // H Back Porch + Sync Width
    UINT16 IPH_WID; // H Resolution
    UINT16 IPH_BP; // H Back Porch
    UINT16 IPV_STA; // V Back Porch + Sync Width
    UINT16 IPV_LEN; // V Resolution
    UINT8 COMBINE; // ??
    //UINT32 CLKIN_CWCODE; // ??
}  VGAMODE ;
#endif

//-----------------------------------------------------------------------------
// Driver customization macro function for key map
//-----------------------------------------------------------------------------

#ifndef NDEBUG
    #define DBGLOG(STR...) Printf(STR)
#else
    #define DBGLOG(STR...)
#endif

/*! Define macro for IR RC5 driver customization.
 */
#define DECLARE_MAP_BEGIN(MAPFUNC) \
    UINT32 MAPFUNC(UINT32 u4GroupCode, UINT32 u4KeyCode) \
    { \
        UINT32 u4CrystalCode = (UINT32)0xffffffff; /* BTN_NONE */ \

    /* the line above is intendedly to be blanc */

#define DECLARE_MAP_END() \
        return u4CrystalCode; \
    } \

    /* the line above is intendedly to be blanc */

#define MAP_GROUP_BEGIN(GROUPCODE) \
    if (u4GroupCode == (GROUPCODE)) \
    { \
        switch (u4KeyCode) \
        { \

    /* the line above is intendedly to be blanc */

#define MAP_GROUP_END() \
        default: \
            DBGLOG("keymap : key none\n"); \
            break; \
        } \
    } \

    /* the line above is intendedly to be blanc */

#define MAP_FACTORY1_END()          \
        default:                    \
            if (u4KeyCode < 128)    \
            {                       \
                u4CrystalCode = BTN_DRIVER1_FACTORY(u4KeyCode); \
                DBGLOG("keymap : factory1 key %d\n", u4KeyCode); \
            }                       \
            else                    \
            {                       \
                DBGLOG("keymap : key none\n"); \
            }                       \
            break; \
        } \
    } \

    /* the line above is intendedly to be blanc */

#define MAP_FACTORY2_END() \
        default: \
            if (u4KeyCode < 128)    \
            {                       \
                u4CrystalCode = BTN_DRIVER2_FACTORY(u4KeyCode); \
                DBGLOG("keymap : factory2 key %d\n", u4KeyCode); \
            }                       \
            else                    \
            {                       \
                DBGLOG("keymap : key none\n"); \
            }                       \
            break; \
        } \
    } \

    /* the line above is intendedly to be blanc */


#define MAP_INSERT(KEYCODE, CRYSTALCODE, KEYDESCRIPTION) \
    case (KEYCODE) : \
        u4CrystalCode = (CRYSTALCODE); \
        DBGLOG("keymap : " KEYDESCRIPTION "\n"); \
        break; \

    /* the line above is intendedly to be blanc */

typedef enum
{
    RTC_INTERNAL = 0,
    RTC_EXT = 1
}   RTC_HW_T;

typedef INT32 (* PFN_VER_CMP_FUNC)(CHAR *szCurVer, CHAR *szUpgVer);

//-----------------------------------------------------------------------------
// Driver customization interface function.
//-----------------------------------------------------------------------------

#ifndef CC_NAND_BOOT

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB at boot stage.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_InitQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB at boot stage.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_InitGet(QUERY_TYPE_T eQryType);

#endif /* CC_NAND_BOOT */

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_OptQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_OptGet(QUERY_TYPE_T eQryType);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @param  pu4Data         The pointer points to the setting data of the type.
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. There is no setting.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_PanelQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data);

//-----------------------------------------------------------------------------
/** Driver customization interface function.
 *  This function is driver module to get different setting of PCB.
 *
 *  @param  eQryType        The query type definition.
 *  @retval The value of the query type setting data.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_PanelGet(QUERY_TYPE_T eQryType);

//-----------------------------------------------------------------------------
/** Set DrvInit Flag
 */
//-----------------------------------------------------------------------------
EXTERN void DRVCUST_SetDrvInit(UINT32 fgFlag);

//-----------------------------------------------------------------------------
/** Get DrvInit Flag
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DRVCUST_GetDrvInit(void);

//-----------------------------------------------------------------------------
/** Get Bootup mode
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DRVCUST_GetBootupMode(void);

//-----------------------------------------------------------------------------
/** Set DRVCUST Bootup Checking.
 */
//-----------------------------------------------------------------------------
EXTERN void DRVCUST_BootupCheck(void);

#endif /* DRVCUST_IF_H */

