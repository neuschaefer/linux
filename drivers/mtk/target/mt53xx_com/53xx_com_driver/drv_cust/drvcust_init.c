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

#if defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER)

#else /* defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER) */

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

#ifndef CC_MTK_PRELOADER

#ifdef PDWNC_EXT_IMPL
#include PDWNC_EXT_IMPL
#endif

#ifdef GPIO_INCLUDE_FILE
#include GPIO_INCLUDE_FILE
#endif

#ifdef IR_INCLUDE_FILE
#include IR_INCLUDE_FILE
#endif

#ifdef BUTTON_INCLUDE_FILE
#include BUTTON_INCLUDE_FILE
#endif

#ifdef T8032UP_INCLUDE_FILE
#include T8032UP_INCLUDE_FILE
#endif

#ifdef POWER_SEQUENCE_EXT_FILE
#include POWER_SEQUENCE_EXT_FILE
#endif

#ifdef SIF_INCLUDE_FILE
#include SIF_INCLUDE_FILE
#endif
#endif /* CC_MTK_PRELOADER */

#if !defined(CC_MTK_PRELOADER) && defined(CC_MTK_LOADER)
#ifdef LOADER_LOGO_NUMBER
#if LOADER_LOGO_NUMBER > 0
#ifdef LOADER_LOGO_H_FILE
#include LOADER_LOGO_H_FILE
#endif /* LOADER_LOGO_H_FILE */
#endif /* LOADER_LOGO_NUMBER > 0 */
#endif /* LOADER_LOGO_NUMBER */
#endif /* !CC_MTK_PRELOADER && CC_MTK_LOADER */

#include "drv_default.h"

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

#ifndef CC_MTK_PRELOADER
    if (DRVCUST_PanelQuery(eQryType, pu4Data)==0)
    {
        return 0;
    }
#endif /* CC_MTK_PRELOADER */

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
        *pu4Data = DEFAULT_DRAM_8_BANKS;
        return 0;
    case eFlagReadODT:
        *pu4Data = DEFAULT_DRAM_RODT;
        return 0;
    case eFlagWriteODT:
        *pu4Data = DEFAULT_DRAM_WODT;
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

#ifndef CC_MTK_PRELOADER

// Fbm Size needed in loader.
    case eFbmMemSize:
        *pu4Data = FBM_MEM_CFG_SIZE;
        return 0;
//Backlight PWM
    case eFlagControlBacklightPWM:
#ifdef CONTROL_BACKLIGHT_PWM
	   *pu4Data = (UINT32)CONTROL_BACKLIGHT_PWM;
#else
	   *pu4Data = (UINT32)0;
#endif // #ifdef CONTROL_BACKLIGHT_PWM
        return 0;

// Usb upgrade
    case eAutoUsbUpgradeEnable:
        *pu4Data = (UINT32)AUTO_USB_UPGRADE_ENABLE;
        return 0;
    case eEepromUsbUpgradeEnable:
        *pu4Data = (UINT32)EEPROM_USB_UPGRADE_ENABLE;
        return 0;
    case eUsbUpgradeFileName:
        *pu4Data = (UINT32)USB_UPGRADE_FILE_NAME;
        return 0;
    case eUsbUpgradeFileNamePrefix:
        *pu4Data = (UINT32)USB_UPGRADE_FILE_NAME_PREFIX;
        return 0;
    case eUsbUpgradeFileNameSuffix:
        *pu4Data = (UINT32)USB_UPGRADE_FILE_NAME_SUFFIX;
        return 0;
    case eLoaderStandbyEnableIfUpgradeBreak:
        *pu4Data = (UINT32)LOADER_STANDBY_ENABLE_IF_UPGRADE_BREAK;
        return 0;
    case eLoaderUpgradeBreakFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_BREAK_HANDLE_FUNC;
        return 0;
    case eLoaderUpgradeVerCmpFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_VER_CMP_FUNC;
        return 0;
    case eLoaderUpgradeDlmCbFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_DLM_CB_FUNC;
        return 0;
    case eLoaderUpgradeDlmGetTagVerCbFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_DLM_GET_TAG_VER_CB_FUNC;
        return 0;
    case eLoaderUpgradeDlmCmpTagVerCbFunc:
        *pu4Data = (UINT32)LOADER_UPGRADE_DLM_CMP_TAG_VER_CB_FUNC;
        return 0;
    case eLoaderUpgradeDlmVerDataOff:
        *pu4Data = (UINT32)LOADER_UPGRADE_DLM_VER_DATA_OFF;
        return 0;
    case eLoaderUpgradeDlmVerDataLen:
        *pu4Data = (UINT32)LOADER_UPGRADE_DLM_VER_DATA_LEN;
        return 0;
    case eLoaderUpgradeDlmForceVer:
        *pu4Data = (UINT32)LOADER_UPGRADE_DLM_FORCE_VER;
        return 0;
// Loader Misc
    case eLoaderEnvInitFunc:
        *pu4Data = (UINT32)LOADER_ENV_INIT_FUNC;
        return 0;
// Loader logo
    case eLoaderLogoNumber:
        *pu4Data = (UINT32)LOADER_LOGO_NUMBER;
        return 0;
    case eLoaderLogoBackground:
        *pu4Data = (UINT32)LOADER_LOGO_BACKGROUND;
        return 0;
    case eLoaderMsgBackground:
        *pu4Data = (UINT32)LOADER_MSG_BACKGROUND;
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
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX0;
        return 0;
#endif

    case eLoaderLogoDataIdx1:
#ifndef LOADER_LOGO_DATAIDX1
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX1;
        return 0;
#endif

    case eLoaderLogoDataIdx2:
#ifndef LOADER_LOGO_DATAIDX2
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX2;
        return 0;
#endif

    case eLoaderLogoDataIdx3:
#ifndef LOADER_LOGO_DATAIDX3
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX3;
        return 0;
#endif

    case eLoaderLogoDataIdx4:
#ifndef LOADER_LOGO_DATAIDX4
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX4;
        return 0;
#endif

    case eLoaderLogoDataIdx5:
#ifndef LOADER_LOGO_DATAIDX5
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX5;
        return 0;
#endif

    case eLoaderLogoDataIdx6:
#ifndef LOADER_LOGO_DATAIDX6
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX6;
        return 0;
#endif

    case eLoaderLogoDataIdx7:
#ifndef LOADER_LOGO_DATAIDX7
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX7;
        return 0;
#endif

    case eLoaderLogoDataIdx8:
#ifndef LOADER_LOGO_DATAIDX8
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX8;
        return 0;
#endif

    case eLoaderLogoDataIdx9:
#ifndef LOADER_LOGO_DATAIDX9
        *pu4Data = (UINT32)0;
        return -1;
#else
        *pu4Data = (UINT32)LOADER_LOGO_DATAIDX9;
        return 0;
#endif

    case eLoaderDualBootOffset:
        *pu4Data = (UINT32)LOADER_DUAL_BOOT_OFFSET;
        return 0;

    case eT8032uPOffset:
        *pu4Data = (UINT32)T8032UP_OFFSET;
        return 0;

    case eT8032uPDualOffset:
        *pu4Data = (UINT32)T8032UP_DUAL_OFFSET;
        return 0;

    case eT8032uPSize:
        *pu4Data = (UINT32)T8032UP_SIZE;
        return 0;

// NOR flash parameter
    case eNorFlashLoaderSize:
        *pu4Data = NOR_FLASH_LOADER_SIZE;
        return 0;
    case eNorFlashPartOffset0:
        *pu4Data = 0;
        return 0;
    case eNorFlashPartOffset1:
        *pu4Data = NOR_PART_OFFSET_1;
        return 0;
    case eNorFlashPartOffset2:
        *pu4Data = NOR_PART_OFFSET_2;
        return 0;
    case eNorFlashPartOffset3:
        *pu4Data = NOR_PART_OFFSET_3;
        return 0;
    case eNorFlashPartOffset4:
        *pu4Data = NOR_PART_OFFSET_4;
        return 0;
    case eNorFlashPartOffset5:
        *pu4Data = NOR_PART_OFFSET_5;
        return 0;
    case eNorFlashPartOffset6:
        *pu4Data = NOR_PART_OFFSET_6;
        return 0;
    case eNorFlashPartOffset7:
        *pu4Data = NOR_PART_OFFSET_7;
        return 0;
    case eNorFlashPartOffset8:
        *pu4Data = NOR_PART_OFFSET_8;
        return 0;
    case eNorFlashPartOffset9:
        *pu4Data = NOR_PART_OFFSET_9;
        return 0;
    case eNorFlashPartOffset10:
        *pu4Data = NOR_PART_OFFSET_10;
        return 0;
    case eNorFlashPartOffset11:
        *pu4Data = NOR_PART_OFFSET_11;
        return 0;
    case eNorFlashPartOffset12:
        *pu4Data = NOR_PART_OFFSET_12;
        return 0;
    case eNorFlashPartOffset13:
        *pu4Data = NOR_PART_OFFSET_13;
        return 0;
    case eNorFlashPartOffset14:
        *pu4Data = NOR_PART_OFFSET_14;
        return 0;
    case eNorFlashPartOffset15:
        *pu4Data = NOR_PART_OFFSET_15;
        return 0;
    case eNorFlashPartOffset16:
        *pu4Data = NOR_PART_OFFSET_16;
        return 0;
    case eNorFlashPartOffset17:
        *pu4Data = NOR_PART_OFFSET_17;
        return 0;
    case eNorFlashPartOffset18:
        *pu4Data = NOR_PART_OFFSET_18;
        return 0;
    case eNorFlashPartOffset19:
        *pu4Data = NOR_PART_OFFSET_19;
        return 0;
    case eNorFlashPartOffset20:
        *pu4Data = NOR_PART_OFFSET_20;
        return 0;
    case eNorFlashPartOffset21:
        *pu4Data = NOR_PART_OFFSET_21;
        return 0;


// NAND flash parameter
#if defined(CC_NAND_ENABLE)
    case eNANDFlashLoaderSize:
        *pu4Data = NAND_FLASH_LOADER_SIZE;
        return 0;
    case eNANDFlashStaticPartitionSize:
        *pu4Data = NAND_FLASH_STATIC_PART_SIZE;
        return 0;
    case eNANDFlashDynamicPartitionSize:
        *pu4Data = NAND_FLASH_DYNAMIC_PART_SIZE;
        return 0;
    case eNANDFlashStaticPartitionLastID:
        *pu4Data = NAND_FLASH_STATIC_LAST_PART_ID;
        return 0;
    case eNANDFlashDynamicPartitionID:
        *pu4Data = NAND_FLASH_DYNAMIC_PART_ID;
        return 0;

    case eNANDFlashPartIdSysImgA:
        *pu4Data = NAND_PART_ID_SYSIMG_A;
        return 0;
    case eNANDFlashPartIdUpA:
        *pu4Data = NAND_PART_ID_UP_A;
        return 0;
    case eNANDFlashPartIdSysImgB:
        *pu4Data = NAND_PART_ID_SYSIMG_B;
        return 0;
    case eNANDFlashPartIdUpB:
        *pu4Data = NAND_PART_ID_UP_B;
        return 0;
    case eNANDFlashPartIdChannelDataA:
        *pu4Data = NAND_PART_ID_CH_A;
        return 0;
    case eNANDFlashPartIdChannelDataB:
        *pu4Data = NAND_PART_ID_CH_B;
        return 0;

    case eNANDFlashPartOffset0:
        *pu4Data = 0;
        return 0;
    case eNANDFlashPartOffset1:
        *pu4Data = NAND_PART_OFFSET_1;
        return 0;
    case eNANDFlashPartOffset2:
        *pu4Data = NAND_PART_OFFSET_2;
        return 0;
    case eNANDFlashPartOffset3:
        *pu4Data = NAND_PART_OFFSET_3;
        return 0;
    case eNANDFlashPartOffset4:
        *pu4Data = NAND_PART_OFFSET_4;
        return 0;
    case eNANDFlashPartOffset5:
        *pu4Data = NAND_PART_OFFSET_5;
        return 0;
    case eNANDFlashPartOffset6:
        *pu4Data = NAND_PART_OFFSET_6;
        return 0;
    case eNANDFlashPartOffset7:
        *pu4Data = NAND_PART_OFFSET_7;
        return 0;
    case eNANDFlashPartOffset8:
        *pu4Data = NAND_PART_OFFSET_8;
        return 0;
    case eNANDFlashPartOffset9:
        *pu4Data = NAND_PART_OFFSET_9;
        return 0;
    case eNANDFlashPartOffset10:
        *pu4Data = NAND_PART_OFFSET_10;
        return 0;
    case eNANDFlashPartOffset11:
        *pu4Data = NAND_PART_OFFSET_11;
        return 0;
    case eNANDFlashPartOffset12:
        *pu4Data = NAND_PART_OFFSET_12;
        return 0;
    case eNANDFlashPartOffset13:
        *pu4Data = NAND_PART_OFFSET_13;
        return 0;
    case eNANDFlashPartOffset14:
        *pu4Data = NAND_PART_OFFSET_14;
        return 0;
    case eNANDFlashPartOffset15:
        *pu4Data = NAND_PART_OFFSET_15;
        return 0;
    case eNANDFlashPartOffset16:
        *pu4Data = NAND_PART_OFFSET_16;
        return 0;
    case eNANDFlashPartOffset17:
        *pu4Data = NAND_PART_OFFSET_17;
        return 0;
    case eNANDFlashPartOffset18:
        *pu4Data = NAND_PART_OFFSET_18;
        return 0;
    case eNANDFlashPartOffset19:
        *pu4Data = NAND_PART_OFFSET_19;
        return 0;
    case eNANDFlashPartOffset20:
        *pu4Data = NAND_PART_OFFSET_20;
        return 0;
    case eNANDFlashPartOffset21:
        *pu4Data = NAND_PART_OFFSET_21;
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

    // System EEPROM
    case eSystemEepromBus:
        *pu4Data = (UINT32) SYSTEM_EEPROM_BUS;
        return 0;
    case eSystemEepromAddress:
        *pu4Data = (UINT32) SYSTEM_EEPROM_ADDRESS;
        return 0;
    case eSystemEepromSize:
        *pu4Data = SYSTEM_EEPROM_SIZE;
        return 0;
    case eSystemEepromClkDiv:
        *pu4Data = SYSTEM_EEPROM_CLKDIV;
        return 0;
    case eSystemEepromPageSize:
        *pu4Data = SYSTEM_EEPROM_PAGESIZE;
        return 0;
#ifdef SYSTEM_EEPROM_WP_GPIO
    case eSysEepromWPGpio:
        *pu4Data = SYSTEM_EEPROM_WP_GPIO;
        return 0;
#endif
#ifdef SYSTEM_EEPROM_WP_ENABLE_POLARITY
    case eSysEepromWPEnablePolarity:
        *pu4Data = SYSTEM_EEPROM_WP_ENABLE_POLARITY;
        return 0;
#endif

	case eSysEepromResetTime:
		*pu4Data = SYSTEM_EEPROM_WAIT_TIME;
        return 0;
    case eSysEepromEnableRetry:
		*pu4Data = SYSTEM_EEPROM_ENABLE_RETRY;
        return 0;

    // HDCP EEPROM
    case eHdcpEepromBus:
        *pu4Data = (UINT32) HDCP_EEPROM_BUS;
        return 0;
    case eHdcpEepromAddress:
        *pu4Data = (UINT32) HDCP_EEPROM_ADDRESS;
        return 0;
    case eHdcpEepromSize:
        *pu4Data = HDCP_EEPROM_SIZE;
        return 0;
    case eHdcpEepromClkDiv:
        *pu4Data = HDCP_EEPROM_CLKDIV;
        return 0;
    case eHdcpEepromOffset:
        *pu4Data = (UINT32) HDCP_EEPROM_OFFSET;
        return 0;
#ifdef HDCP_EEPROM_WP_GPIO
    case eHdcpEepromWPGpio:
        *pu4Data = HDCP_EEPROM_WP_GPIO;
        return 0;
#endif
#ifdef HDCP_EEPROM_WP_ENABLE_POLARITY
    case eHdcpEepromWPEnablePolarity:
        *pu4Data = HDCP_EEPROM_WP_ENABLE_POLARITY;
        return 0;
#endif

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
#ifdef DRVCUST_EEPROM_MW_OFFSET
    case eEepromMwOffset:
         *pu4Data =  DRVCUST_EEPROM_MW_OFFSET ;
    	  return 0;
#endif
#ifdef DRVCUST_EEPROM_MW_DRIVER_SIZE
    case eEepromMwSize:
         *pu4Data = DRVCUST_EEPROM_MW_DRIVER_SIZE ;
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
    case eEepromCECMenuLanguageOffset:
         *pu4Data = (UINT32)EEPROM_CEC_MENU_LANGUAGE_OFFSET;
         return 0;

    case eEepromCECEnableOffset:
         *pu4Data = (UINT32)EEPROM_CEC_ENABLE_OFFSET;
         return 0;

	  case eEepromCECOneTouchPlayEnableOffset:
         *pu4Data = (UINT32)EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE_OFFSET;
         return 0;

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

    case eGpioLedCtrlFunc:
#ifdef GPIO_LED_CTRL_FUNC
        *pu4Data = (UINT32)GPIO_LED_CTRL_FUNC;
        return 0;
#else /* GPIO_LED_CTRL_FUNC */
        return -1;
#endif /* GPIO_LED_CTRL_FUNC */

    // USB Loader polling time in ms.
    // If not define, it will use default: MUSB_LOADER_MAX_WAIT_TIME in musb_if.c.
#ifdef USB_LOADER_WAIT_TIME
    case eUSBLoaderWaitTime:
        *pu4Data = USB_LOADER_WAIT_TIME;
        return 0;
#endif

    // USB0 vbus control.
#ifdef USB0_VBUS_GPIO
    case eUSBVbus0Gpio:
        *pu4Data = USB0_VBUS_GPIO;
        return 0;
#endif
#ifdef USB0_VBUS_GPIO_POLARITY
    case eUSBVbus0EnablePolarity:
        *pu4Data = USB0_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB1 vbus control.
#ifdef USB1_VBUS_GPIO
    case eUSBVbus1Gpio:
        *pu4Data = USB1_VBUS_GPIO;
        return 0;
#endif
#ifdef USB1_VBUS_GPIO_POLARITY
    case eUSBVbus1EnablePolarity:
        *pu4Data = USB1_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB0 over current flag enable.
#ifdef USB0_OC_GPIO
    case eUSB0OCGpio:
        *pu4Data = USB0_OC_GPIO;
        return 0;
#endif
#ifdef USB0_OC_GPIO_POLARITY
    case eUSB0OCEnablePolarity:
        *pu4Data = USB0_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB1 over current flag enable.
#ifdef USB1_OC_GPIO
    case eUSB1OCGpio:
        *pu4Data = USB1_OC_GPIO;
        return 0;
#endif
#ifdef USB1_OC_GPIO_POLARITY
    case eUSB1OCEnablePolarity:
        *pu4Data = USB1_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB HSTERMC impedance control.
#ifdef USB_HSTERMC
    case eUSBHSTERMC:
        *pu4Data = USB_HSTERMC;
        return 0;
#endif

    // USB vbus turn on delay (ms).
#ifdef USB_VBUS_TURN_ON_DELAY
    case eUSBVbusTurnOnDelay:
        *pu4Data = USB_VBUS_TURN_ON_DELAY;
        return 0;
#endif

    // USB Full speed only enable
#ifdef USB_FULL_SPEED_ONLY_ENABLE
    case eUSBFullSpeedOnlyEnable:
        *pu4Data = TRUE;
        return 0;
#endif

    // USB disable suspend mode.
#ifdef USB_DISABLE_SUSPEND_MODE
    case eUSBDisableSuspendMode:
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

#ifdef FLAG_IR_POWERON_WAKEUP
       case eIrrxFlagPowerOnWakeup:
        *pu4Data = (UINT32)FLAG_IR_POWERON_WAKEUP;
        return 0;
#endif

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
#ifdef IRRX_CLK_SEL
			*pu4Data = IRRX_CLK_SEL;
			return 0;
#else
			return -1;
#endif         
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
    case eIrrxFlagUserDefineWakeup:
        *pu4Data = (UINT32)FLAG_IR_USER_DEFINE_WAKEUP;
        return 0;
    case eIrrxSetUsrNotifyFunc:
        *pu4Data = (UINT32)IRRX_SET_USR_NOTIFY_FUNC;
        return 0;
    case eIrrxLedBlinkGpio:
#ifdef IRRX_LED_BLINK_GPIO
        *pu4Data = IRRX_LED_BLINK_GPIO;
        return 0;
#else
        return -1;
#endif
    case eIrrxLedBlinkOnVal:
#ifdef IRRX_LED_BLINK_ON_VAL
        *pu4Data = IRRX_LED_BLINK_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case eIrrxLedBlinkDuration:
#ifdef IRRX_LED_BLINK_DURATION
        *pu4Data = IRRX_LED_BLINK_DURATION;
        return 0;
#else
        return -1;
#endif

#ifdef SET_SIGNAL_USR_NOTIFY_FUNC
 	case eSetSignalUsrNotifyFunc://singnal booster add by Hua 0609
 		*pu4Data = (UINT32)SET_SIGNAL_USR_NOTIFY_FUNC;
		return 0;
#endif
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
#ifdef STANDBY_SET_GPIO
    case eStandbySetGpio:
        *pu4Data = STANDBY_SET_GPIO;
        return 0;
#endif /* STANDBY_SET_GPIO */
#ifdef POWER_STATUS_GPIO
    case ePowerStatusGpio:
        *pu4Data = POWER_STATUS_GPIO;
        return 0;
#endif /* POWER_STATUS_GPIO */
    case eStdbyUsePowerBtnCount:
        *pu4Data = STDBY_USE_PWR_BTN_CNT;
        return 0;
    case eStdbyUsePrevPowerState:
        *pu4Data = STDBY_USE_PREV_PWR_STATE;
        return 0;
    case eStdbyUseVGAStatus:
        *pu4Data = STDBY_USE_VGA_STATUS;
        return 0;
    case eGpiotimerInitFunc:
        *pu4Data = (UINT32)GPIO_TIMER_INIT_FUNC;
        break;
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
    case eT8032CtrlErrLed:
#ifdef T8032_CTRL_ERR_LED
        *pu4Data = T8032_CTRL_ERR_LED;
        return 0;
#else
        return -1;
#endif

    case eT8032CtrlErrLedOnVal:
#ifdef T8032_CTRL_ERR_LED_ON_VAL
        *pu4Data = T8032_CTRL_ERR_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif

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
    case eMt8280ResetGpioPolarity:
        *pu4Data = MT8280_RESET_GPIO_POLARITY;
        return 0;
    case eMT8280CmdAckGpio:
        *pu4Data = MT8280_CMD_ACK_GPIO;
        return 0;
    case eMT8280CmdAckGpioPolarity:
        *pu4Data = MT8280_CMD_ACK_GPIO_POLARITY;
        return 0;
    case eMT8280SendPanel:
        *pu4Data = MT8280_SEND_PANEL;
        return 0;
    case eDemodResetGpio:
#ifdef DEFAULT_DEMOD_RESET_GPIO
        *pu4Data = (UINT32) DEFAULT_DEMOD_RESET_GPIO;
        return 0;
#else /* DEFAULT_DEMOD_RESET_GPIO */
        return -1;
#endif /* DEFAULT_DEMOD_RESET_GPIO */
#endif /* CC_MTK_PRELOADER */

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

    case eSifCfgAfterAction:
#ifdef SIF_CFG_AFTER_ACTION_FUNC
        *pu4Data = (UINT32)SIF_CFG_AFTER_ACTION_FUNC;
        return 0;
#else
        return -1;
#endif  
    case eSifCfgBeforeAction:
#ifdef SIF_CFG_BEFORE_ACTION_FUNC
        *pu4Data = (UINT32)SIF_CFG_BEFORE_ACTION_FUNC;
        return 0;
#else
        return -1;
#endif

    case eAudioFlashAQPartition:
        *pu4Data = (UINT32)FLASH_AQ_BASE_PARTITION;
        return 0;
#if 0
    case eAudioFlashAQSize:
        *pu4Data = (UINT32)NAND_PART_SIZE_8;
        return 0;
#endif

    case eCustomerPowerOnSequence:
#ifdef CUSTOMER_POWER_ON_SEQUENCE
        *pu4Data = (UINT32)CUSTOMER_POWER_ON_SEQUENCE;
        return 0;
#else
        break;
#endif

    case eCustomerPowerOffSequence:
#ifdef CUSTOMER_POWER_OFF_SEQUENCE
        *pu4Data = (UINT32)CUSTOMER_POWER_OFF_SEQUENCE;
        return 0;
#else
		break;
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
    default:
        break;
    }

    return -1;
}

UINT32 DRVCUST_InitGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret;

    VERIFY(DRVCUST_InitQuery(eQryType, &u4Ret) == 0);
    return u4Ret;
}

#ifndef CC_MTK_PRELOADER
void DRVCUST_BootupCheck()
{
// #if MT8280_RESET_GPIO
//     GPIO_SetOut(MT8280_RESET_GPIO, 1);
// #endif /* MT8280_RESET_GPIO */

#ifdef CUSTOM_BootupCheck
    // Can use GPIO and I2C only in this function.
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

INT32 DRVCUST_IsLogoDisableInEeprom()
{
#ifndef CUSTOM_IsLogoDisableInEeprom
    return 0;
#else /* CUSTMO_IsLogoDisableInEeprom */
    return CUSTOM_IsLogoDisableInEeprom();
#endif /* CUSTMO_IsLogoDisableInEeprom */
}
#endif /* CC_MTK_PRELOADER */

#endif /* defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER) */

#ifndef CC_MTK_LOADER
void DRVCUST_UserLISR(INT32 i4Vector)
{
#ifdef CUSTOM_USER_LISR
    CUSTOM_USER_LISR(i4Vector);
#endif /* CUSTOM_USER_LISR */
}
#endif /* CC_MTK_LOADER */

