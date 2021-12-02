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
#include "x_hal_5381.h"
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

#ifdef EDID_INCLUDE_FILE
#include EDID_INCLUDE_FILE
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

#ifdef SIF_INCLUDE_FILE
#include SIF_INCLUDE_FILE
#endif

#ifdef TCON_CUST_FILE
#include TCON_CUST_FILE
#endif

#endif /* CC_MTK_PRELOADER */

#ifndef CC_MTK_PRELOADER
#ifdef LOADER_LOGO_H_FILE
#include LOADER_LOGO_H_FILE
#endif /* LOADER_LOGO_H_FILE */
#endif /* CC_MTK_PRELOADER */

#include "drv_default.h"

#ifdef CUSTOM_EEPROM_FILE
#include CUSTOM_EEPROM_FILE
#endif

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
        *pu4Data = (UINT32)DEFAULT_BOOT_VERBOSE;
        return 0;

    case eNorCpuClock:
        *pu4Data = (UINT32)DEFAULT_NOR_CPU_CLK;
        return 0;

    case eDramType:
        *pu4Data = (UINT32)DEFAULT_DRAM_TYPE;
        return 0;

    case eDramColAddr:
        *pu4Data = (UINT32)DEFAULT_DRAM_COLADDR;
        return 0;

    case eDdrClock:
        *pu4Data = (UINT32)DEFAULT_DDR_CLOCK;
        return 0;

    case eDdrCL:
        *pu4Data = (UINT32)DEFAULT_DDR_CL;
        return 0;

    case eDdrBusX8:
        *pu4Data = (UINT32)DEFAULT_DDR_BUS_X8;
        return 0;

    case eDmpllSpectrumPermillage:
        *pu4Data = (UINT32)DMPLL_SPECTRUM_PERMILLAGE;
        return 0;

    case eDmpllSpectrumFrequency:
        *pu4Data = (UINT32)DMPLL_SPECTRUM_FREQUENCY;
        return 0;

    case eFlag1GBitSupport:
        *pu4Data = (UINT32)DEFAULT_DRAM_8_BANKS;
        return 0;

    case eFlagReadODT:
        *pu4Data = (UINT32)DEFAULT_DRAM_RODT;
        return 0;

    case eFlagWriteODT:
        *pu4Data = (UINT32)DEFAULT_DRAM_WODT;
        return 0;

    case eFlagDDRQfp:
        *pu4Data = (UINT32)FLAG_DDR_QFP;
        return 0;

    case eFlagDDR16BitSwap:
        *pu4Data = (UINT32)FLAG_DDR_16BITSWAP;
        return 0;

    case eSysResetTimer:
        *pu4Data = (UINT32)((SYS_RESET_TIMER == 0x1000000) ? (SYS_RESET_TIMER - 1) : SYS_RESET_TIMER);
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

    case eTrustzoneEnable:
#ifdef CC_TRUSTZONE_SUPPORT
        *pu4Data = 1;
#else
        *pu4Data = 0;
#endif
        return 0;

    case eTzCodeBase:
        *pu4Data = (UINT32)TZ_CODE_BASE;
        return 0;


#ifndef CC_MTK_PRELOADER

// Fbm Size needed in loader.
    case eFbmMemSize:
        *pu4Data = (UINT32)FBM_MEM_CFG_SIZE;
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

    case eUsbUpgradeForceDisable:
#ifndef LOADER_UPGRADE_FORCE_DISABLE_OFFSET
        return -1;
#else
        *pu4Data = (UINT32)LOADER_UPGRADE_FORCE_DISABLE_OFFSET;
        return 0;
#endif

    case eLoaderStandbyEnableIfUpgradeBreak:
        *pu4Data = (UINT32)LOADER_STANDBY_ENABLE_IF_UPGRADE_BREAK;
        return 0;

    case eLoaderUpgradeBreakStateOffset:
#ifndef LOADER_UPGRADE_BREAK_STATE_OFFSET
        return -1;
#else
        *pu4Data = (UINT32)LOADER_UPGRADE_BREAK_STATE_OFFSET;
        return 0;
#endif

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

    case eLoaderCECConfigQueryFunc:
        *pu4Data = (UINT32)LOADER_CEC_CONFIG_QUERY_FUNC;
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

    case eNorFlashLoaderSize:
        *pu4Data = (UINT32)NOR_FLASH_LOADER_SIZE;
        return 0;

    case eNorFlashPartOffset0:
        *pu4Data = 0;
        return 0;

    case eNorFlashPartOffset1:
        *pu4Data = (UINT32)NOR_PART_OFFSET_1;
        return 0;

    case eNorFlashPartOffset2:
        *pu4Data = (UINT32)NOR_PART_OFFSET_2;
        return 0;

    case eNorFlashPartOffset3:
        *pu4Data = (UINT32)NOR_PART_OFFSET_3;
        return 0;

    case eNorFlashPartOffset4:
        *pu4Data = (UINT32)NOR_PART_OFFSET_4;
        return 0;

    case eNorFlashPartOffset5:
        *pu4Data = (UINT32)NOR_PART_OFFSET_5;
        return 0;

    case eNorFlashPartOffset6:
        *pu4Data = (UINT32)NOR_PART_OFFSET_6;
        return 0;

    case eNorFlashPartOffset7:
        *pu4Data = (UINT32)NOR_PART_OFFSET_7;
        return 0;

    case eNorFlashPartOffset8:
        *pu4Data = (UINT32)NOR_PART_OFFSET_8;
        return 0;

    case eNorFlashPartOffset9:
        *pu4Data = (UINT32)NOR_PART_OFFSET_9;
        return 0;

    case eNorFlashPartOffset10:
        *pu4Data = (UINT32)NOR_PART_OFFSET_10;
        return 0;

    case eNorFlashPartOffset11:
        *pu4Data = (UINT32)NOR_PART_OFFSET_11;
        return 0;

    case eNorFlashPartOffset12:
        *pu4Data = (UINT32)NOR_PART_OFFSET_12;
        return 0;

    case eNorFlashPartOffset13:
        *pu4Data = (UINT32)NOR_PART_OFFSET_13;
        return 0;

    case eNorFlashPartOffset14:
        *pu4Data = (UINT32)NOR_PART_OFFSET_14;
        return 0;

    case eNorFlashPartOffset15:
        *pu4Data = (UINT32)NOR_PART_OFFSET_15;
        return 0;

    case eNorFlashPartOffset16:
        *pu4Data = (UINT32)NOR_PART_OFFSET_16;
        return 0;

    case eNorFlashPartOffset17:
        *pu4Data = (UINT32)NOR_PART_OFFSET_17;
        return 0;

    case eNorFlashPartOffset18:
        *pu4Data = (UINT32)NOR_PART_OFFSET_18;
        return 0;

    case eNorFlashPartOffset19:
        *pu4Data = (UINT32)NOR_PART_OFFSET_19;
        return 0;

    case eNorFlashPartOffset20:
        *pu4Data = (UINT32)NOR_PART_OFFSET_20;
        return 0;

    case eNorFlashPartOffset21:
        *pu4Data = (UINT32)NOR_PART_OFFSET_21;
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
    case eNorFlashPartSize8:
        *pu4Data = NOR_PART_SIZE_8;
        return 0;

    case eNorFlashPartSize9:
        *pu4Data = NOR_PART_SIZE_9;
        return 0;

    case eNorFlashPartSize10:
        *pu4Data = NOR_PART_SIZE_10;
        return 0;

    case eNorFlashPartSize11:
        *pu4Data = NOR_PART_SIZE_11;
        return 0;

    case eNorFlashPartSize12:
        *pu4Data = NOR_PART_SIZE_12;


    case eVGAEDIDNorPartId:
        *pu4Data = VGAEDID_NOR_FLASH_PART_ID;
        return 0;

    case eVGAEDIDNorOffset:
        *pu4Data = VGAEDID_NOR_FLASH_OFFSET;
        return 0;

    case eHDMIEDIDNorPartId:
        *pu4Data = HDMIEDID_NOR_FLASH_PART_ID;
        return 0;

    case eHDMIEDIDNorOffset:
        *pu4Data = HDMIEDID_NOR_FLASH_OFFSET;
        return 0;

// NAND flash parameter
#if defined(CC_NAND_ENABLE) || defined(CC_EMMC_BOOT)
    case eNANDFlashTotalSize:
        *pu4Data = NAND_FLASH_TOTAL_SIZE;
        return 0;

    case eNANDFlashLoaderSize:
        *pu4Data = (UINT32)NAND_FLASH_LOADER_SIZE;
        return 0;

    case eNANDFlashStaticPartitionSize:
        *pu4Data = (UINT32)NAND_FLASH_STATIC_PART_SIZE;
        return 0;

    case eNANDFlashDynamicPartitionSize:
        *pu4Data = (UINT32)NAND_FLASH_DYNAMIC_PART_SIZE;
        return 0;

    case eNANDFlashStaticPartitionLastID:
        *pu4Data = (UINT32)NAND_FLASH_STATIC_LAST_PART_ID;
        return 0;

    case eNANDFlashDynamicPartitionID:
        *pu4Data = (UINT32)NAND_FLASH_DYNAMIC_PART_ID;
        return 0;

    case eNANDFlashPartIdSysImgA:
        *pu4Data = (UINT32)NAND_PART_ID_SYSIMG_A;
        return 0;

    case eNANDFlashPartIdUpA:
        *pu4Data = (UINT32)NAND_PART_ID_UP_A;
        return 0;

    case eNANDFlashPartIdSysImgB:
        *pu4Data = (UINT32)NAND_PART_ID_SYSIMG_B;
        return 0;

    case eNANDFlashPartIdUpB:
        *pu4Data = (UINT32)NAND_PART_ID_UP_B;
        return 0;

    case eNANDFlashPartIdChannelDataA:
        *pu4Data = (UINT32)NAND_PART_ID_CH_A;
        return 0;

    case eNANDFlashPartIdChannelDataB:
        *pu4Data = (UINT32)NAND_PART_ID_CH_B;
        return 0;

    case eNANDFlashPartIdEepromA:
        *pu4Data = (UINT32)NAND_PART_ID_EEPROM_A;
        return 0;

    case eNANDFlashPartIdEepromB:
        *pu4Data = (UINT32)NAND_PART_ID_EEPROM_B;
        return 0;

    // NAND flash partition offset customization
    case eNANDFlashPartOffset0:
        *pu4Data = 0;
        return 0;

    case eNANDFlashPartOffset1:
        *pu4Data = (UINT32)NAND_PART_OFFSET_1;
        return 0;

    case eNANDFlashPartOffset2:
        *pu4Data = (UINT32)NAND_PART_OFFSET_2;
        return 0;

    case eNANDFlashPartOffset3:
        *pu4Data = (UINT32)NAND_PART_OFFSET_3;
        return 0;

    case eNANDFlashPartOffset4:
        *pu4Data = (UINT32)NAND_PART_OFFSET_4;
        return 0;

    case eNANDFlashPartOffset5:
        *pu4Data = (UINT32)NAND_PART_OFFSET_5;
        return 0;

    case eNANDFlashPartOffset6:
        *pu4Data = (UINT32)NAND_PART_OFFSET_6;
        return 0;

    case eNANDFlashPartOffset7:
        *pu4Data = (UINT32)NAND_PART_OFFSET_7;
        return 0;

    case eNANDFlashPartOffset8:
        *pu4Data = (UINT32)NAND_PART_OFFSET_8;
        return 0;

    case eNANDFlashPartOffset9:
        *pu4Data = (UINT32)NAND_PART_OFFSET_9;
        return 0;

    case eNANDFlashPartOffset10:
        *pu4Data = (UINT32)NAND_PART_OFFSET_10;
        return 0;

    case eNANDFlashPartOffset11:
        *pu4Data = (UINT32)NAND_PART_OFFSET_11;
        return 0;

    case eNANDFlashPartOffset12:
        *pu4Data = (UINT32)NAND_PART_OFFSET_12;
        return 0;

    case eNANDFlashPartOffset13:
        *pu4Data = (UINT32)NAND_PART_OFFSET_13;
        return 0;

    case eNANDFlashPartOffset14:
        *pu4Data = (UINT32)NAND_PART_OFFSET_14;
        return 0;

    case eNANDFlashPartOffset15:
        *pu4Data = (UINT32)NAND_PART_OFFSET_15;
        return 0;

    case eNANDFlashPartOffset16:
        *pu4Data = (UINT32)NAND_PART_OFFSET_16;
        return 0;

    case eNANDFlashPartOffset17:
        *pu4Data = (UINT32)NAND_PART_OFFSET_17;
        return 0;

    case eNANDFlashPartOffset18:
        *pu4Data = (UINT32)NAND_PART_OFFSET_18;
        return 0;

    case eNANDFlashPartOffset19:
        *pu4Data = (UINT32)NAND_PART_OFFSET_19;
        return 0;

    case eNANDFlashPartOffset20:
        *pu4Data = (UINT32)NAND_PART_OFFSET_20;
        return 0;

    case eNANDFlashPartOffset21:
        *pu4Data = (UINT32)NAND_PART_OFFSET_21;
        return 0;

    case eNANDFlashPartOffset22:
        *pu4Data = (UINT32)NAND_PART_OFFSET_22;
        return 0;

    case eNANDFlashPartOffset23:
        *pu4Data = (UINT32)NAND_PART_OFFSET_23;
        return 0;

    case eNANDFlashPartOffset24:
        *pu4Data = (UINT32)NAND_PART_OFFSET_24;
        return 0;

    case eNANDFlashPartOffset25:
        *pu4Data = (UINT32)NAND_PART_OFFSET_25;
        return 0;

    case eNANDFlashPartOffset26:
        *pu4Data = (UINT32)NAND_PART_OFFSET_26;
        return 0;

    case eNANDFlashPartOffset27:
        *pu4Data = (UINT32)NAND_PART_OFFSET_27;
        return 0;

    case eNANDFlashPartOffset28:
        *pu4Data = (UINT32)NAND_PART_OFFSET_28;
        return 0;

    case eNANDFlashPartOffset29:
        *pu4Data = (UINT32)NAND_PART_OFFSET_29;
        return 0;

    case eNANDFlashPartOffset30:
        *pu4Data = (UINT32)NAND_PART_OFFSET_30;
        return 0;

    case eNANDFlashPartOffset31:
        *pu4Data = (UINT32)NAND_PART_OFFSET_31;
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

    case eNANDFlashPartSize22:
        *pu4Data = (UINT32)NAND_PART_SIZE_22;
        return 0;

    case eNANDFlashPartSize23:
        *pu4Data = (UINT32)NAND_PART_SIZE_23;
        return 0;

    case eNANDFlashPartSize24:
        *pu4Data = (UINT32)NAND_PART_SIZE_24;
        return 0;

    case eNANDFlashPartSize25:
        *pu4Data = (UINT32)NAND_PART_SIZE_25;
        return 0;

    case eNANDFlashPartSize26:
        *pu4Data = (UINT32)NAND_PART_SIZE_26;
        return 0;

    case eNANDFlashPartSize27:
        *pu4Data = (UINT32)NAND_PART_SIZE_27;
        return 0;

    case eNANDFlashPartSize28:
        *pu4Data = (UINT32)NAND_PART_SIZE_28;
        return 0;

    case eNANDFlashPartSize29:
        *pu4Data = (UINT32)NAND_PART_SIZE_29;
        return 0;

    case eNANDFlashPartSize30:
        *pu4Data = (UINT32)NAND_PART_SIZE_30;
        return 0;

    case eNANDFlashPartSize31:
        *pu4Data = (UINT32)NAND_PART_SIZE_31;
        return 0;

    // NAND flash partition name customization
    case eNANDFlashPartName0:
        *pu4Data = (UINT32)NAND_PART_NAME_0;
        return 0;

    case eNANDFlashPartName1:
        *pu4Data = (UINT32)NAND_PART_NAME_1;
        return 0;

    case eNANDFlashPartName2:
        *pu4Data = (UINT32)NAND_PART_NAME_2;
        return 0;

    case eNANDFlashPartName3:
        *pu4Data = (UINT32)NAND_PART_NAME_3;
        return 0;

    case eNANDFlashPartName4:
        *pu4Data = (UINT32)NAND_PART_NAME_4;
        return 0;

    case eNANDFlashPartName5:
        *pu4Data = (UINT32)NAND_PART_NAME_5;
        return 0;

    case eNANDFlashPartName6:
        *pu4Data = (UINT32)NAND_PART_NAME_6;
        return 0;

    case eNANDFlashPartName7:
        *pu4Data = (UINT32)NAND_PART_NAME_7;
        return 0;

    case eNANDFlashPartName8:
        *pu4Data = (UINT32)NAND_PART_NAME_8;
        return 0;

    case eNANDFlashPartName9:
        *pu4Data = (UINT32)NAND_PART_NAME_9;
        return 0;

    case eNANDFlashPartName10:
        *pu4Data = (UINT32)NAND_PART_NAME_10;
        return 0;

    case eNANDFlashPartName11:
        *pu4Data = (UINT32)NAND_PART_NAME_11;
        return 0;

    case eNANDFlashPartName12:
        *pu4Data = (UINT32)NAND_PART_NAME_12;
        return 0;

    case eNANDFlashPartName13:
        *pu4Data = (UINT32)NAND_PART_NAME_13;
        return 0;

    case eNANDFlashPartName14:
        *pu4Data = (UINT32)NAND_PART_NAME_14;
        return 0;

    case eNANDFlashPartName15:
        *pu4Data = (UINT32)NAND_PART_NAME_15;
        return 0;

    case eNANDFlashPartName16:
        *pu4Data = (UINT32)NAND_PART_NAME_16;
        return 0;

    case eNANDFlashPartName17:
        *pu4Data = (UINT32)NAND_PART_NAME_17;
        return 0;

    case eNANDFlashPartName18:
        *pu4Data = (UINT32)NAND_PART_NAME_18;
        return 0;

    case eNANDFlashPartName19:
        *pu4Data = (UINT32)NAND_PART_NAME_19;
        return 0;

    case eNANDFlashPartName20:
        *pu4Data = (UINT32)NAND_PART_NAME_20;
        return 0;

    case eNANDFlashPartName21:
        *pu4Data = (UINT32)NAND_PART_NAME_21;
        return 0;

    case eNANDFlashPartName22:
        *pu4Data = (UINT32)NAND_PART_NAME_22;
        return 0;

    case eNANDFlashPartName23:
        *pu4Data = (UINT32)NAND_PART_NAME_23;
        return 0;

    case eNANDFlashPartName24:
        *pu4Data = (UINT32)NAND_PART_NAME_24;
        return 0;

    case eNANDFlashPartName25:
        *pu4Data = (UINT32)NAND_PART_NAME_25;
        return 0;

    case eNANDFlashPartName26:
        *pu4Data = (UINT32)NAND_PART_NAME_26;
        return 0;

    case eNANDFlashPartName27:
        *pu4Data = (UINT32)NAND_PART_NAME_27;
        return 0;

    case eNANDFlashPartName28:
        *pu4Data = (UINT32)NAND_PART_NAME_28;
        return 0;

    case eNANDFlashPartName29:
        *pu4Data = (UINT32)NAND_PART_NAME_29;
        return 0;

    case eNANDFlashPartName30:
        *pu4Data = (UINT32)NAND_PART_NAME_30;
        return 0;

    case eNANDFlashPartName31:
        *pu4Data = (UINT32)NAND_PART_NAME_31;
        return 0;

#ifdef CC_MTD_ENCRYPT_SUPPORT
    // NAND flash partition encypt customization
    case eNANDFlashPartEncypt0:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_0;
        return 0;

    case eNANDFlashPartEncypt1:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_1;
        return 0;

    case eNANDFlashPartEncypt2:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_2;
        return 0;

    case eNANDFlashPartEncypt3:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_3;
        return 0;

    case eNANDFlashPartEncypt4:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_4;
        return 0;

    case eNANDFlashPartEncypt5:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_5;
        return 0;

    case eNANDFlashPartEncypt6:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_6;
        return 0;

    case eNANDFlashPartEncypt7:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_7;
        return 0;

    case eNANDFlashPartEncypt8:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_8;
        return 0;

    case eNANDFlashPartEncypt9:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_9;
        return 0;

    case eNANDFlashPartEncypt10:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_10;
        return 0;

    case eNANDFlashPartEncypt11:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_11;
        return 0;

    case eNANDFlashPartEncypt12:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_12;
        return 0;

    case eNANDFlashPartEncypt13:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_13;
        return 0;

    case eNANDFlashPartEncypt14:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_14;
        return 0;

    case eNANDFlashPartEncypt15:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_15;
        return 0;

    case eNANDFlashPartEncypt16:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_16;
        return 0;

    case eNANDFlashPartEncypt17:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_17;
        return 0;

    case eNANDFlashPartEncypt18:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_18;
        return 0;

    case eNANDFlashPartEncypt19:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_19;
        return 0;

    case eNANDFlashPartEncypt20:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_20;
        return 0;

    case eNANDFlashPartEncypt21:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_21;
        return 0;

    case eNANDFlashPartEncypt22:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_22;
        return 0;

    case eNANDFlashPartEncypt23:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_23;
        return 0;

    case eNANDFlashPartEncypt24:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_24;
        return 0;

    case eNANDFlashPartEncypt25:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_25;
        return 0;

    case eNANDFlashPartEncypt26:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_26;
        return 0;

    case eNANDFlashPartEncypt27:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_27;
        return 0;

    case eNANDFlashPartEncypt28:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_28;
        return 0;

    case eNANDFlashPartEncypt29:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_29;
        return 0;

    case eNANDFlashPartEncypt30:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_30;
        return 0;

    case eNANDFlashPartEncypt31:
        *pu4Data = (UINT32)NAND_PART_ENCRYPT_31;
        return 0;
#endif /* CC_MTD_ENCRYPT_SUPPORT */

    case eNANDFlashPartEnd:
        *pu4Data = (UINT32)NAND_PART_END;
        return 0;

    case eVGAEDIDNANDPartId:
        *pu4Data = VGAEDID_NAND_FLASH_PART_ID;
        return 0 ;

    case eVGAEDIDNANDOffset:
        *pu4Data = VGAEDID_NAND_FLASH_OFFSET;
        return 0 ;

    case eHDMIEDIDNANDPartId:
        *pu4Data = HDMIEDID_NAND_FLASH_PART_ID;
        return 0 ;

    case eHDMIEDIDNANDOffset:
        *pu4Data = HDMIEDID_NAND_FLASH_OFFSET;
        return 0 ;
#endif // CC_NAND_ENABLE

    // System EEPROM
    case eSystemEepromBus:
        *pu4Data = (UINT32) SYSTEM_EEPROM_BUS;
        return 0;

    case eSystemEepromAddress:
        *pu4Data = (UINT32) SYSTEM_EEPROM_ADDRESS;
        return 0;

    case eSystemEepromSize:
        *pu4Data = (UINT32)SYSTEM_EEPROM_SIZE;
        return 0;

    case eSystemEepromClkDiv:
        *pu4Data = (UINT32)SYSTEM_EEPROM_CLKDIV;
        return 0;

    case eSystemEepromPageSize:
        *pu4Data = (UINT32)SYSTEM_EEPROM_PAGESIZE;
        return 0;

#ifdef SYSTEM_EEPROM_WP_GPIO
    case eSysEepromWPGpio:
        *pu4Data = (UINT32)SYSTEM_EEPROM_WP_GPIO;
        return 0;
#endif

#ifdef SYSTEM_EEPROM_WP_ENABLE_POLARITY
    case eSysEepromWPEnablePolarity:
        *pu4Data = (UINT32)SYSTEM_EEPROM_WP_ENABLE_POLARITY;
        return 0;
#endif

    case eSysEepromResetTime:
        *pu4Data = (UINT32)SYSTEM_EEPROM_WAIT_TIME;
        return 0;

    case eSysEepromEnableRetry:
        *pu4Data = (UINT32)SYSTEM_EEPROM_ENABLE_RETRY;
        return 0;

    // HDCP EEPROM
    case eHdcpEepromBus:
        *pu4Data = (UINT32) HDCP_EEPROM_BUS;
        return 0;

    case eHdcpEepromAddress:
        *pu4Data = (UINT32) HDCP_EEPROM_ADDRESS;
        return 0;

    case eHdcpEepromSize:
        *pu4Data = (UINT32)HDCP_EEPROM_SIZE;
        return 0;

    case eHdcpEepromClkDiv:
        *pu4Data = (UINT32)HDCP_EEPROM_CLKDIV;
        return 0;

    case eHdcpEepromOffset:
        *pu4Data = (UINT32) HDCP_EEPROM_OFFSET;
        return 0;

#ifdef HDCP_EEPROM_WP_GPIO
    case eHdcpEepromWPGpio:
        *pu4Data = (UINT32)HDCP_EEPROM_WP_GPIO;
        return 0;
#endif

#ifdef HDCP_EEPROM_WP_ENABLE_POLARITY
    case eHdcpEepromWPEnablePolarity:
        *pu4Data = (UINT32)HDCP_EEPROM_WP_ENABLE_POLARITY;
        return 0;
#endif

#ifdef DRVCUST_EEPROM_VGA_EDID_OFFSET
    case eEepromVgaEdidOffset:
        *pu4Data = DRVCUST_EEPROM_VGA_EDID_OFFSET;
        return 0;
#endif

#ifdef DRVCUST_VGA_EDID_SIZE
    case eEdidVgaSize:
        *pu4Data = DRVCUST_VGA_EDID_SIZE;
        return 0;
#endif

    case eEdidVgaStorageSize:
    #ifdef CC_NAND_ENABLE
        *pu4Data = HDMIEDID_NAND_FLASH_PART_SIZE;
    #elif defined(DRVCUST_VGA_EDID_STORAGE_SIZE)
        *pu4Data = DRVCUST_VGA_EDID_STORAGE_SIZE;
    #else
        return -1 ;
#endif
        return 0;
#ifdef DRVCUST_EEPROM_HDMI_EDID_OFFSET
    case eEepromHdmiEdidOffset:
        *pu4Data = DRVCUST_EEPROM_HDMI_EDID_OFFSET;
        return 0;
#endif

#ifdef DRVCUST_HDMI_EDID_SIZE
    case eEdidHdmiSize:
        *pu4Data = DRVCUST_HDMI_EDID_SIZE;
        return 0;
#endif

    case eEdidHdmiStorageSize:
    #ifdef CC_NAND_ENABLE
        *pu4Data = HDMIEDID_NAND_FLASH_PART_SIZE;
    #elif defined(DRVCUST_HDMI_EDID_STORAGE_SIZE)
        *pu4Data = DRVCUST_HDMI_EDID_STORAGE_SIZE;
    #else
        return -1;
#endif
        return 0;

#ifdef EEPROM_VGA_EDID_INDEX_OFFSET
    case eEdidVgaIndexOffset:
        *pu4Data = EEPROM_VGA_EDID_INDEX_OFFSET;
        return 0;
#endif

#ifdef EEPROM_HDMI_EDID_INDEX_OFFSET
    case eEdidHdmiIndexOffset:
        *pu4Data = EEPROM_HDMI_EDID_INDEX_OFFSET;
        return 0;
#endif

    // System EEPROM Layout Configuration
#ifdef DRVCUST_EEPROM_HDMI_HDCP_OFFSET
    case eEepromHdmiHdcpOffset:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_HDMI_HDCP_OFFSET;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_HDMI_HDCP_SIZE
    case eEepromHdmiHdcpSize:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_HDMI_HDCP_SIZE;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_DTV_DRIVER_OFFSET
    case eEepromDtvDriverOffset:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_DTV_DRIVER_OFFSET;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_DTV_DRIVER_SIZE
    case eEepromDtvDriverSize:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_DTV_DRIVER_SIZE ;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_NPTV_DRIVER_OFFSET
    case eEepromNptvDriverOffset:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_NPTV_DRIVER_OFFSET ;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_NPTV_DRIVER_SIZE
    case eEepromNptvDriverSize:
         *pu4Data = (UINT32)DRVCUST_EEPROM_NPTV_DRIVER_SIZE ;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_DRIVER_SIZE
    case eEepromDriverSize:
         *pu4Data = (UINT32)DRVCUST_EEPROM_DRIVER_SIZE ;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_MW_OFFSET
    case eEepromMwOffset:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_MW_OFFSET ;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_MW_SIZE
    case eEepromMwSize:
         *pu4Data = (UINT32)DRVCUST_EEPROM_MW_SIZE ;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_ERROR_CODE_OFFSET
    case eEepromErrorCodeOffset:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_ERROR_CODE_OFFSET;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_ERROR_CODE_SIZE
    case eEepromErrorCodeSize:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_ERROR_CODE_SIZE;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_STATE_CODE_OFFSET
    case eEepromStateCodeOffset:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_STATE_CODE_OFFSET;
          return 0;
#endif

#ifdef DRVCUST_EEPROM_STATE_CODE_SIZE
    case eEepromStateCodeSize:
         *pu4Data =  (UINT32)DRVCUST_EEPROM_STATE_CODE_SIZE;
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

    case eEepromDataMemOffset:
        *pu4Data = (UINT32)DRVCUST_EEPROM_DATA_MEM_OFFSET;
         return 0;
#ifndef CC_UBOOT
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

#endif //CC_UBOOT

    // USB Loader polling time in ms.
    // If not define, it will use default: MUSB_LOADER_MAX_WAIT_TIME in musb_if.c.
#ifdef USB_LOADER_WAIT_TIME
    case eUSBLoaderWaitTime:
        *pu4Data = (UINT32)USB_LOADER_WAIT_TIME;
        return 0;
#endif

    // USB0 vbus control.
#ifdef USB0_VBUS_GPIO
    case eUSBVbus0Gpio:
        *pu4Data = (UINT32)USB0_VBUS_GPIO;
        return 0;
#endif

#ifdef USB0_PORT_USING
    case eUSB0Config:
    *pu4Data = (UINT32)USB0_PORT_USING;
    return 0;
#endif

#ifdef USB1_PORT_USING
    case eUSB1Config:
    *pu4Data = (UINT32)USB1_PORT_USING;
    return 0;
#endif

#ifdef USB2_PORT_USING
    case eUSB2Config:
    *pu4Data = (UINT32)USB2_PORT_USING;
    return 0;
#endif

#ifdef USB3_PORT_USING
    case eUSB3Config:
    *pu4Data = (UINT32)USB3_PORT_USING;
    return 0;
#endif

#ifdef USB0_VBUS_GPIO_POLARITY
    case eUSBVbus0EnablePolarity:
        *pu4Data = (UINT32)USB0_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB1 vbus control.
#ifdef USB1_VBUS_GPIO
    case eUSBVbus1Gpio:
        *pu4Data = (UINT32)USB1_VBUS_GPIO;
        return 0;
#endif

#ifdef USB1_VBUS_GPIO_POLARITY
    case eUSBVbus1EnablePolarity:
        *pu4Data = (UINT32)USB1_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB2 vbus control.
#ifdef USB2_VBUS_GPIO
    case eUSBVbus2Gpio:
        *pu4Data = USB2_VBUS_GPIO;
        return 0;
#endif
#ifdef USB2_VBUS_GPIO_POLARITY
    case eUSBVbus2EnablePolarity:
        *pu4Data = USB2_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB3 vbus control.
#ifdef USB3_VBUS_GPIO
    case eUSBVbus3Gpio:
        *pu4Data = USB3_VBUS_GPIO;
        return 0;
#endif
#ifdef USB3_VBUS_GPIO_POLARITY
    case eUSBVbus3EnablePolarity:
        *pu4Data = USB3_VBUS_GPIO_POLARITY;
        return 0;
#endif

    // USB0 over current flag enable.
#ifdef USB0_OC_GPIO
    case eUSB0OCGpio:
        *pu4Data = (UINT32)USB0_OC_GPIO;
        return 0;
#endif

#ifdef USB0_OC_GPIO_POLARITY
    case eUSB0OCEnablePolarity:
        *pu4Data = (UINT32)USB0_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB1 over current flag enable.
#ifdef USB1_OC_GPIO
    case eUSB1OCGpio:
        *pu4Data = (UINT32)USB1_OC_GPIO;
        return 0;
#endif

#ifdef USB1_OC_GPIO_POLARITY
    case eUSB1OCEnablePolarity:
        *pu4Data = (UINT32)USB1_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB2 over current flag enable.
#ifdef USB2_OC_GPIO
    case eUSB2OCGpio:
        *pu4Data = (UINT32)USB2_OC_GPIO;
        return 0;
#endif

#ifdef USB2_OC_GPIO_POLARITY
    case eUSB2OCEnablePolarity:
        *pu4Data = (UINT32)USB2_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB3 over current flag enable.
#ifdef USB3_OC_GPIO
    case eUSB3OCGpio:
        *pu4Data = (UINT32)USB3_OC_GPIO;
        return 0;
#endif

#ifdef USB3_OC_GPIO_POLARITY
    case eUSB3OCEnablePolarity:
        *pu4Data = (UINT32)USB3_OC_GPIO_POLARITY;
        return 0;
#endif

    // USB HSTERMC impedance control.
#ifdef USB_HSTERMC
    case eUSBHSTERMC:
        *pu4Data = (UINT32)USB_HSTERMC;
        return 0;
#endif

    // USB vbus turn on delay (ms).
#ifdef USB_VBUS_TURN_ON_DELAY
    case eUSBVbusTurnOnDelay:
        *pu4Data = (UINT32)USB_VBUS_TURN_ON_DELAY;
        return 0;
#endif

    // USB Full speed only enable
#ifdef USB_FULL_SPEED_ONLY_ENABLE
    case eUSBFullSpeedOnlyEnable:
        *pu4Data = (UINT32)TRUE;
        return 0;
#endif

    // USB disable suspend mode.
#ifdef USB_DISABLE_SUSPEND_MODE
    case eUSBDisableSuspendMode:
        *pu4Data = (UINT32)TRUE;
        return 0;
#endif

// Irrx settings
    case eIrrxType:
#ifndef DEFAULT_IRRX_TYPE
        #error no define DEFAULT_IRRX_TYPE
#else /* DEFAULT_IRRX_TYPE */
        *pu4Data = (UINT32)DEFAULT_IRRX_TYPE;
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
        *pu4Data = (UINT32)DEFAULT_IRRX_KEY_UP_TIMEOUT;
        return 0;

    case eIrrxRepeatValidTime:
        *pu4Data = (UINT32)DEFAULT_IRRX_REPEAT_VALID_TIME;
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

    case eUsrIRClkSel:
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
     case eSetSignalUsrNotifyFunc:
         *pu4Data = (UINT32)SET_SIGNAL_USR_NOTIFY_FUNC;
        return 0;
#endif

    case eFlagIrrxSoundSupport:
#ifdef FLAG_IRRX_SOUND_SUPPORT
        *pu4Data = FLAG_IRRX_SOUND_SUPPORT;
        return 0;
#else
        return -1;
#endif
    case eFlagIrrxSoundSupportAlwaysEnable:
        *pu4Data = (UINT32)FLAG_IRRX_SOUND_SUPPORT_ALWAYSE_ENABLE;
        return 0;
    case eIrrcIsUsrLongPress:
        *pu4Data = (UINT32)IRRC_IS_USR_LONG_PRESS_FUNC;
        return 0;
    case eIrrcCheckRepeatByRaw:
        *pu4Data = (UINT32)IRRC_CHECK_REPEAT_BY_RAW;
        return 0;
// Keypad
    case eFlagButtonSupport:
        *pu4Data = (UINT32)FLAG_BUTTON_SUPPORT;
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

    case eCombKeyList:
        *pu4Data = (UINT32)COMB_KEY_LIST;
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
        *pu4Data = (UINT32)AUDIO_STA335_RESET_GPIO;
        return 0;
#endif /* AUDIO_STA335_RESET_GPIO */

#ifdef AUDIO_STA335_RESET_GPIO_POLARITY
    case eAudioSTA335ResetGpioPolarity:
        *pu4Data = (UINT32)AUDIO_STA335_RESET_GPIO_POLARITY;
        return 0;
#endif /* AUDIO_STA335_RESET_GPIO_POLARITY */

    case eAudioMuteGpioNum:
        *pu4Data = AUDIO_Mute_Gpio_Num;
        return 0;

    case eAudioAmpEnableGpioNum:
        *pu4Data = AUDIO_Amp_Gpio_Num;
        return 0;

// Other Gpio
    case eExternalStatus:
        *pu4Data = (UINT32)FLAG_EXT_STATUS;
        return 0;

#ifdef STANDBY_SET_GPIO
    case eStandbySetGpio:
        *pu4Data = (UINT32)STANDBY_SET_GPIO;
        return 0;
#endif /* STANDBY_SET_GPIO */

#ifdef POWER_STATUS_GPIO
    case ePowerStatusGpio:
        *pu4Data = (UINT32)POWER_STATUS_GPIO;
        return 0;
#endif /* POWER_STATUS_GPIO */

    case eStdbyUsePowerBtnCount:
        *pu4Data = (UINT32)STDBY_USE_PWR_BTN_CNT;
        return 0;

    case eStdbyUsePrevPowerState:
        *pu4Data = (UINT32)STDBY_USE_PREV_PWR_STATE;
        return 0;

    case eStdbyUseVGAStatus:
        *pu4Data = (UINT32)STDBY_USE_VGA_STATUS;
        return 0;

    case eGpiotimerInitFunc:
        *pu4Data = (UINT32)GPIO_TIMER_INIT_FUNC;
        break;

    case ePowerCtrlDelayMs:
        *pu4Data = (UINT32)POWER_CTRL_DELAY_MS;
        return 0;

    case ePowerCtrlGpio:
#ifdef POWER_CTRL_GPIO
        *pu4Data = (UINT32)POWER_CTRL_GPIO;
        return 0;
#else
        return -1;
#endif

    case ePowerCtrlOnVal:
#ifdef POWER_CTRL_ON_VAL
        *pu4Data = (UINT32)POWER_CTRL_ON_VAL;
        return 0;
#else
        return -1;
#endif

    case eDvdCtrlGpio:
#ifdef DVD_CTRL_GPIO
        *pu4Data = (UINT32)DVD_CTRL_GPIO;
        return 0;
#else
        return -1;
#endif

    case eDvdCtrlOnVal:
#ifdef DVD_CTRL_ON_VAL
        *pu4Data = (UINT32)DVD_CTRL_ON_VAL;
        return 0;
#else
        return -1;
#endif

    case eSpecialWakeupGpio:
#ifdef SPECIAL_WAKEUP_GPIO
        *pu4Data = (UINT32)SPECIAL_WAKEUP_GPIO;
        return 0;
#else
        return -1;
#endif

    case eSpecialWakeupOnVal:
#ifdef SPECIAL_WAKEUP_ON_VAL
        *pu4Data = (UINT32)SPECIAL_WAKEUP_ON_VAL;
        return 0;
#else
        return -1;
#endif

// Led
    case eFlagBootBlinkingLedSupport:
#ifdef FLAG_BOOT_BLINKING_LED_SUPPORT
        *pu4Data = (UINT32)FLAG_BOOT_BLINKING_LED_SUPPORT;
        return 0;
#else
        return -1;
#endif

    case eBootBlinkingLed:
#ifdef BOOT_BLINKING_LED
        *pu4Data = (UINT32)BOOT_BLINKING_LED;
        return 0;
#else
        return -1;
#endif

    case eBootBlinkingLedOnVal:
#ifdef BOOT_BLINKING_LED_ON_VAL
    *pu4Data = (UINT32)BOOT_BLINKING_LED_ON_VAL;
    return 0;
#else
    return -1;
#endif

    case eBootBlinkingLedPeriod:
#ifdef BOOT_BLINKING_LED_PERIOD
    *pu4Data = (UINT32)BOOT_BLINKING_LED_PERIOD;
    return 0;
#else
    return -1;
#endif

    case eT8032CtrlErrLed:
#ifdef T8032_CTRL_ERR_LED
        *pu4Data = (UINT32)T8032_CTRL_ERR_LED;
        return 0;
#else
        return -1;
#endif

    case eT8032CtrlErrLedOnVal:
#ifdef T8032_CTRL_ERR_LED_ON_VAL
        *pu4Data = (UINT32)T8032_CTRL_ERR_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif

    case ePowerLedGpio:
#ifdef POWER_LED_GPIO
        *pu4Data = (UINT32)POWER_LED_GPIO;
        return 0;
#else
        return -1;
#endif

    case ePowerLedOnVal:
#ifdef POWER_LED_ON_VAL
        *pu4Data = (UINT32)POWER_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif

    case eBackupLedGpio:
#ifdef BACKUP_LED_GPIO
        *pu4Data = (UINT32)BACKUP_LED_GPIO;
        return 0;
#else
        return -1;
#endif

    case eBackupLedOnVal:
#ifdef BACKUP_LED_ON_VAL
        *pu4Data = (UINT32)BACKUP_LED_ON_VAL;
        return 0;
#else
        return -1;
#endif

    case eSupportDarkLed:
        *pu4Data = (UINT32)FLAG_SUPPORT_DARK_LED;
        return 0;

    case ePowerLedDarkVal:
#ifdef POWER_LED_DARK_VAL
        *pu4Data = (UINT32)POWER_LED_DARK_VAL;
        return 0;
#else
        return -1;
#endif

    case eBackupLedDarkVal:
#ifdef BACKUP_LED_DARK_VAL
        *pu4Data = (UINT32)BACKUP_LED_DARK_VAL;
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

#ifndef CC_UBOOT
    case ePWM0Port:
        *pu4Data = (UINT32)PWM0_PORT;
        return 0;

    case ePWM1Port:
        *pu4Data = (UINT32)PWM1_PORT;
        return 0;

    case ePWM2Port:
        *pu4Data = (UINT32)PWM2_PORT;
        return 0;
#endif

    // MT8292 GPIO support
    case eFlagMt8292GpioSupport:
        *pu4Data = (UINT32)FLAG_MT8292_GPIO_SUPPORT;
        return 0;

    case eMt8292ResetGpio:
        *pu4Data = (UINT32)MT8292_RESET_GPIO;
        return 0;

    case eMt8292MuteGpio:
        *pu4Data = (UINT32)MT8292_MUTE_GPIO;
        return 0;

    case eMt8280ResetGpio:
        *pu4Data = (UINT32)MT8280_RESET_GPIO;
        return 0;

    case eMt8280ResetGpioPolarity:
        *pu4Data = (UINT32)MT8280_RESET_GPIO_POLARITY;
        return 0;

    case eMT8280CmdAckGpio:
        *pu4Data = (UINT32)MT8280_CMD_ACK_GPIO;
        return 0;

    case eMT8280CmdAckGpioPolarity:
        *pu4Data = (UINT32)MT8280_CMD_ACK_GPIO_POLARITY;
        return 0;

    case eMT8280SendPanel:
        *pu4Data = (UINT32)MT8280_SEND_PANEL;
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

    case eSifEnablePdwncMaster1:
        *pu4Data = (UINT32)SIF_ENABLE_PDWNC_MASTER1;
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

    case eSifCfgPinSetFunc:
#ifdef SIF_CFG_PIN_SET_FUNC
        *pu4Data = (UINT32)SIF_CFG_PIN_SET_FUNC;
        return 0;
#else
        return -1;
#endif

    case eSifCfgGetConfigFunc:
#ifdef SIF_CFG_GET_CONFIG_FUNC
        *pu4Data = (UINT32)SIF_CFG_GET_CONFIG_FUNC;
        return 0;
#else
        return -1;
#endif

// SIF SCL stretch workaround
    case eSifCfgTRIDelay:
#ifdef SIF_CFG_TRI_DELAY
        *pu4Data = (UINT32)SIF_CFG_TRI_DELAY;
        return 0;
#else
        return -1;
#endif

    case eSifCfgTRIDelayDevAddr:
#ifdef SIF_CFG_TRI_DELAY_DEV_ADDR
        *pu4Data = (UINT32)SIF_CFG_TRI_DELAY_DEV_ADDR;
        return 0;
#else
        return -1;
#endif

// For device want to use S/W approach to access
    case eSifCfgSWBusId:
#ifdef SIF_CFG_SW_BUS_ID
        *pu4Data = (UINT32)SIF_CFG_SW_BUS_ID;
        return 0;
#else
        return -1;
#endif

    case eSifCfgSWSDA:
#ifdef SIF_CFG_SW_SDA
        *pu4Data = (UINT32)SIF_CFG_SW_SDA;
        return 0;
#else
        return -1;
#endif

    case eSifCfgSWSCL:
#ifdef SIF_CFG_SW_SCL
        *pu4Data = (UINT32)SIF_CFG_SW_SCL;
        return 0;
#else
        return -1;
#endif

    case eSifCfgNumOfSlaveUsingSW:
#ifdef SIF_CFG_NUM_OF_SLAVE_USING_SW
        *pu4Data = (UINT32)SIF_CFG_NUM_OF_SLAVE_USING_SW;
        return 0;
#else
        return -1;
#endif

    case eSifCfgArrayOfSlaveAddrUsingSW:
#ifdef SIF_CFG_ARRAY_OF_SLAVE_ADDR_USING_SW
        *pu4Data = (UINT32)SIF_CFG_ARRAY_OF_SLAVE_ADDR_USING_SW;
        return 0;
#else
        return -1;
#endif
        case eAudioFlashAQPartition:
            *pu4Data = (UINT32)FLASH_AQ_BASE_PARTITION;
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

    case eCustomerPowerOnSequence:
#ifdef CUSTOMER_POWER_ON_SEQUENCE
        *pu4Data = (UINT32)CUSTOMER_POWER_ON_SEQUENCE;
        return 0;
#else
        return -1 ;
#endif

    case eCustomerPowerOffSequence:
#ifdef CUSTOMER_POWER_OFF_SEQUENCE
        *pu4Data = (UINT32)CUSTOMER_POWER_OFF_SEQUENCE;
        return 0;
#else
        return -1 ;
#endif

    case ePowerStateQueryFunc:
#ifdef POWER_STATE_QUERY_FUNC
        *pu4Data = (UINT32)POWER_STATE_QUERY_FUNC;
        return 0;
#else
        return -1;
#endif

    case eCountryDefaultSetting:
        #ifdef DEFAULT_TARGET_COUNTRY
        *pu4Data = (UINT32)DEFAULT_TARGET_COUNTRY;
        return 0;
        #else
        return -1;
        #endif

#ifdef PCIE_RESET_GPIO
    case ePcieResetGpio:
        *pu4Data = (UINT32)PCIE_RESET_GPIO;
        return 0;
#endif
	case eHDMIHPD0GPIONUM:
	 #if  defined(CC_MT5389) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5398)
		   *pu4Data = (UINT32)DEFAULT_HDMIHPD0_GPIONUM;
			return 0;
	 #else
			return -1;
	 #endif
		 
	 case eHDMIHPD1GPIONUM:
	 #if  defined(CC_MT5389) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5398)
			   *pu4Data = (UINT32)DEFAULT_HDMIHPD1_GPIONUM;
				return 0;
	 #else
				return -1;
	 #endif
	 case eHDMIHPD2GPIONUM:
	 #if  defined(CC_MT5389) || defined(CC_MT5396)|| defined(CC_MT5368) || defined(CC_MT5398)
			   *pu4Data = (UINT32)DEFAULT_HDMIHPD2_GPIONUM;
				return 0;
	 #else
				return -1;
	 #endif
	 case eHDMIHPD3GPIONUM:
 #if  defined(CC_MT5396)|| defined(CC_MT5368) || defined(CC_MT5398)
				 *pu4Data = (UINT32)DEFAULT_HDMIHPD3_GPIONUM;
				 return 0;
 #else
				 return -1;
 #endif

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

