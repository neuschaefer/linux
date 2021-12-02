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

#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H


//----------------------------------------------------------------------------
// Initial dram settings
//----------------------------------------------------------------------------
#define DEFAULT_DRAM_TYPE                    DDR_III_x1
#define DEFAULT_DDR_CLOCK                   (1188000000)
#define FLAG_DDR_QFP                         1
//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#define TOTAL_MEM_SIZE                      (256 * 1024 * 1024)
#define DIRECT_FB_MEM_SIZE                  (50 * 1024 * 1024)
#define FB_MEM_SIZE                         (2 * 1024 * 1024)
#ifndef FBM_MEM_CFG_SIZE
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5368_DDR3x1)
/*
#ifdef CC_3D_SUPPORT
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5395_3DTV)
#else
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
#endif
*/
#endif /* FBM_MEM_CFG_SIZE */

#define LINUX_KERNEL_MEM_SIZE               ((TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE - DIRECT_FB_MEM_SIZE - FB_MEM_SIZE) & 0xfff00000)

#define DEFAULT_TARGET_COUNTRY              (COUNTRY_US)  

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT                (PANEL_DEFAULT)
#endif
#define BACKLIGHT_GPIO                      WT6720_GPIO4
#define BACKLIGHT_ON_VAL                    (0)
#define PANELPOWER_GPIO                     WT6720_GPIO5
#define PANELPOWER_ON_VAL                   (1)

// disable spread spectrum for vga output
#define SPREAD_SPECTRUM_PERMILLAGE          (0)

// pwm backlight adjustment setting
#define PANEL_BACKLIGHT_PWM_PORT            (2)

#define PDWNC_POLARITY                      (0)

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_INCLUDE_FILE                  "mtk/mt5368Lm1v1_aud.h"
#define AUDIO_Mute_Gpio_Num                 (OPCTRL(5)) //(GPIO(55))        
#define AUDIO_Amp_Gpio_Num                  (OPCTRL(6))
#define AUDIO_MUTE_GPIO_POLARITY            AUD_MUTE_GPIO_LOW_ENALBE
#define AUDIO_AMP_GPIO_POLARITY             AUD_AMP_GPIO_LOW_ENALBE
#define AUDIO_INPUT_SW_GPIO_SEL             (&_armt5368lm1v1InputSwGpio)
#define AUDIO_INPUT_MUX_SEL_TABLE           (&_rmt5368lm1v1AudInputMuxTable)
#define AUDIO_INTER_DAC_USAGE               (&_armt5368lm1v1InterDacUsage)
#define AUDIO_ADAC_USAGE                    (&_armt5368lm1v1AdacUsage)
#define AUDIO_Digital_Data_TABLE            (&_afgmt5368lm1v1AudDigitalDataUsage)
#define AUDIO_AOUT_PAD_CFG                  (&_armt5368lm1v1AoutPadTable)

//----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#define CC_WT6720	//use ext gpio

#define FLAG_EXT_STATUS                     (0)
// #define POWER_LED_GPIO                   (OPCTRL(0))
// #define POWER_LED_ON_VAL                 (0)
// #define BACKUP_LED_GPIO                  (OPCTRL(0))
// #define BACKUP_LED_ON_VAL                (0)
// #define POWER_CTRL_GPIO
// #define POWER_CTRL_ON_VAL

//----------------------------------------------------------------------------
// SIF configuration
//----------------------------------------------------------------------------
#ifndef SIF_INCLUDE_FILE
#define SIF_INCLUDE_FILE               "mtk/mt5368L_sif_cfg.h"
#endif

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#define SYSTEM_EEPROM_SIZE                  (4096)
//#define SYSTEM_EEPROM_WP_GPIO               (GPIO(9))
//#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (1)
//#define EDID_EEPROM_WP_GPIO			    (GPIO(4))       // GPIO14.
//#define EDID_EEPROM_WP_ENABLE_POLARITY    (1)

//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
#define USB0_VBUS_GPIO                      OPCTRL(0)
#define USB0_VBUS_GPIO_POLARITY             (1)
#define USB1_VBUS_GPIO                      OPCTRL(1)
#define USB1_VBUS_GPIO_POLARITY             (1)
#define USB2_VBUS_GPIO                      OPCTRL(2)
#define USB2_VBUS_GPIO_POLARITY             (1)
#define USB3_VBUS_GPIO                      OPCTRL(3)
#define USB3_VBUS_GPIO_POLARITY             (1)

#define USB0_OC_GPIO                        ADIN(4)     // ADIN 4.
#define USB0_OC_GPIO_POLARITY               (0)
#define USB1_OC_GPIO                        ADIN(4)     // ADIN 4.
#define USB1_OC_GPIO_POLARITY               (0)
#define USB2_OC_GPIO                        ADIN(5)     // ADIN 5.
#define USB2_OC_GPIO_POLARITY               (0)
#define USB3_OC_GPIO                        ADIN(5)     // ADIN 5.
#define USB3_OC_GPIO_POLARITY               (0)

//#define USB_DISABLE_SUSPEND_MODE /* Disable USB suspend, share 240 Mhz with H.264.*/
//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#define FLAG_BUTTON_SUPPORT                 (0)

#ifndef T8032UP_INCLUDE_FILE
#define T8032UP_INCLUDE_FILE                "mtk/mtk_t8032fw.h"
#define T8032UP_OFFSET                      au1T8032FW
#define T8032UP_SIZE                        24576//16384
#endif

//----------------------------------------------------------------------------
// MTK IR only setting
//----------------------------------------------------------------------------
#define IR_INCLUDE_FILE                     "mtk/mtk_ir.h"

#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE                   "mtk/source_table.c"
#endif

//----------------------------------------------------------------------------
// default PIN Setup
//----------------------------------------------------------------------------
#ifndef PINMUX_INCLUDE_FILE
#define PINMUX_INCLUDE_FILE                 "mtk/mt5368Lm1v1_pin_setup.h"
#endif

//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                      "mtk/hdmi_conf_mt5368LQFP.c"
#endif

/*
 * default VGA table
*/
#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE                    "mtk/mtk_vga.h"
#endif

//----------------------------------------------------------------------------
// Tcon Customization
//----------------------------------------------------------------------------
// support LVDS and mLVDS both
#define PANEL_DISPLAY_TYPE_ALLOWED_MASK ((1<<PANEL_DISPLAY_TYPE_LVDS) | (1 << PANEL_DISPLAY_TYPE_MLVDS))

#ifndef TCON_CUST_FILE
#define TCON_CUST_FILE                      "mtk/mt5368m1v1_tcon_cust.c"
#endif

//----------------------------------------------------------------------------
// SCART out configuration
//----------------------------------------------------------------------------
#ifndef TVE_INCLUDE_FILE
#define TVE_INCLUDE_FILE                    "mtk/mtk_tve.c"
#endif


//----------------------------------------------------------------------------
// Nor  configuration
//----------------------------------------------------------------------------
#define NOR_FLASH_TOTAL_SIZE                (8*1024*1024)
//#define NOR_FLASH_APPIMAGE_SIZE             (7*1024*1024)

//----------------------------------------------------------------------------
// NAND customization
//----------------------------------------------------------------------------
#define NAND_FLASH_TOTAL_SIZE           (32*1024*1024)
#define NAND_FLASH_LOADER_SIZE          (LOADER_MAX_SIZE)
#define NAND_FLASH_APPIMAGE_SIZE        (0xC00000)
//#define LOADER_DUAL_BOOT_OFFSET         (NAND_FLASH_APPIMAGE_SIZE + NAND_FLASH_LOADER_SIZE)

//----------------------------------------------------------------------------
// Capture Logo Customization
//----------------------------------------------------------------------------

#if 0
// driver logo setting
#ifdef CC_BOOT_VDEC_LOGO
#define DRV_LOGO_ENABLE                     (1)
#define BOOT_VDEC_LOGO_PART_SIZE            (0x100000)          /* Partitino Size: 1MB */

#define DRV_LOGO_VDEC_FLASH_ADDR            NAND_PART_OFFSET_16
#define DRV_LOGO_VDEC_FLASH_SIZE            BOOT_VDEC_LOGO_PART_SIZE
#define DRV_LOGO_VDEC_FLASH_SLOT_NUM        (2)
#define DRV_LOGO_VDEC_DEVICE_ID             (16)
//#define NAND_PART_SIZE_15                 (3*1024*1024)       /*should shrink to 1MB when SDM is ready*/
#endif

#define DRV_LOGO_H_FILE                     "mtk/MediaTek_Mpeg2Logo.h"
#define DRV_POWER_MUSIC_H_FILE              "mtk/mtk_PowerMusic.h"      /* power on music. */
#endif /* CC_BOOT_VDEC_LOGO */

/*
 * default quality table
*/
#ifndef QTYTBL_INCLUDE_FILE
//#define QTYTBL_INCLUDE_FILE                   "mtk/QTY_DEFAULT/mtk_QtyTbl.h"
#endif

/*
 * default color parameter
*/
#ifndef COLOR_PARAM_INCLUDE_FILE
//#define COLOR_PARAM_INCLUDE_FILE              "mtk/PANEL_DEFAULT/color_param.h"
#endif

/*
 * dynamic backlight parameter
*/

/*
 * OD setting
*/

#define SUPPORT_OD  (0)

#else /* CONFIG_DEFAULT_H */
#error "Cannot include multi-config files"
#endif /* CONFIG_DEFAULT_H */

