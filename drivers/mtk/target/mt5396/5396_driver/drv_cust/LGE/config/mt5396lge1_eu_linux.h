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
#define DEFAULT_DRAM_TYPE               DDR_III_x4
#define DEFAULT_DDR_CLOCK               (1404000000)

#ifndef DRAM_PRIORITY_LIST
// Audio/0 > B2R/5 > VBI/3D/TVE/2 > NR/PSCAN/4 > SCPOS/7 > OSD/OD/MMU/3 > MJC_IN/13 > MJC_OUT/14 > CPU/6 > DEMUX/GCPU/1 > VLD1/9 > VDEC_MC/8 > GFX/11 > VENC/12 > Test0/15 > 3DGFX/10
#define DRAM_PRIORITY_LIST          ((UINT8*)"09253184bafcd67e")
#endif

//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#define TOTAL_MEM_SIZE              (512 * 1024 * 1024)
#define DIRECT_FB_MEM_SIZE          (1 * 1024 * 1024) //(50 * 1024 * 1024)
#define FB_MEM_SIZE                 (76 * 1024 * 1024) //(2 * 1024 * 1024)

#ifndef FBM_MEM_CFG_SIZE
#ifdef CC_3D_SUPPORT
#define FBM_MEM_CFG_SIZE            (FBM_MEM_CFG_MT5396_3DTV)
//#define FBM_MEM_CFG_SIZE            (FBM_MEM_CFG_MT5369_3DTV_REDUCED)
#else
#define FBM_MEM_CFG_SIZE            (FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
#endif
#endif /* FBM_MEM_CFG_SIZE */
#define LINUX_KERNEL_MEM_SIZE       ((TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE - DIRECT_FB_MEM_SIZE - FB_MEM_SIZE) & 0xfff00000)

#define DEFAULT_TARGET_COUNTRY  (COUNTRY_EU)  

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#define PANEL_INDEX_FROM_EEPROM         (0)
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT            (PANEL_LG_32_EUD_SDA1)
#endif

// disable spread spectrum for vga output
#define SPREAD_SPECTRUM_PERMILLAGE      (0)
// pwm backlight adjustment setting
#define PANEL_BACKLIGHT_PWM_PORT        (2)
// pwm backlight is controlled by app layer through mtal
#define PANEL_DISABLE_DIMMING           (1)

// Enable control matrix 
#define PANEL_LVDS_10BIT_PIN_MAP        (_pu4PanelLvds10bitPinMap)
#define PANEL_LVDS_10BIT_PN_SWAP        (_pu4PanelLvds10bitPNSwap)

// Customers' header file (PCB specific)
#define LVDS_A_CTRL_EVEN (1)
#define LVDS_A_CTRL_ODD (0)
#define LVDS_B_CTRL_EVEN (3)
#define LVDS_B_CTRL_ODD (2)

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_INCLUDE_FILE              "LGE/LGE_aud.h"
#define AUDIO_INPUT_MUX_SEL_TABLE       (&_rLgeAudInputMuxTable)
#define AUDIO_INTER_DAC_USAGE           (&_arLgeInterDacUsage)
#define AUDIO_Digital_Data_TABLE        (&_afgLgeAudDigitalDataUsage)
#define AUDIO_ADAC_USAGE                (&_arLgeAdacUsage)
#define AUDIO_INPUT_SRC_AGC_FLAG        (&_aafgLgeAudInputSrcAgcFlag)
//#define AUDIO_HP_PLUG_IN_GPIO           OPCTRL(6)
//#define AUDIO_HP_PLUG_IN_POLARITY       (AUD_GPIO_HIGH_ENALBE)
#define AUDIO_Arc_Gpio_Num              (GPIO(8))
#define AUDIO_ARC_GPIO_POLARITY         AUD_GPIO_HIGH_ENALBE
#define AUDIO_HEAAC_MULTICHL_DDCO       (0)
#define AUDIO_TUNER_ATD                 (0)
#define AUDIO_DOWNMIX_CHANNEL_POSITION  (DOWNMIX_CH_BEFORE_AVC)
 //----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#define FLAG_EXT_STATUS                 (0)

//----------------------------------------------------------------------------
// SIF customization
//----------------------------------------------------------------------------
#define SIF_INCLUDE_FILE                "LGE/LGE_sif_cfg.h"
#define SIF_ENABLE_SCL_STRETCH          (1)
#define SIF_ENABLE_PDWNC_MASTER         (1)
#define SIF_ENABLE_PDWNC_MASTER1        (1)

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#define SYSTEM_EEPROM_ADDRESS                 (0xA8)
#define SYSTEM_EEPROM_SIZE                    (128*1024) //1Mbit
#define HDCP_EEPROM_ADDRESS                   (SYSTEM_EEPROM_ADDRESS)

//----------------------------------------------------------------------------
// European CI (Common Interface) customization
//----------------------------------------------------------------------------
//#define CI_8295_RESET_GPIO              (GPIO(65))  
//#define CI_8295_RESET_POLARITY       (0)
//#define CI_EXTERNAL_IC_INT_GPIO  (GPIO(84))  // Set:Interrupt Mode, UnSet:Polling Mode
#define CI_5V_POWERON_GPIO              (GPIO(122))
#define CI_5V_POWERON_POLARITY          (1)
#define CI_EXTERNAL_IC_DRIVING_CURRENT      (2)       //8mA
#define CI_SYSTEM_CLOCK                 (72) // 144 MHz
#define CI_TS_CLOCK                         (72)  // 72 MHz
#define CI_SPI_CLOCK                        (27)  // 72 MHz

//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
#define USB0_VBUS_GPIO                      GPIO(42)        // GPIO42
#define USB0_VBUS_GPIO_POLARITY		        (1)
#define USB1_VBUS_GPIO                      GPIO(43)        // GPIO43
#define USB1_VBUS_GPIO_POLARITY		        (1)
#define USB3_VBUS_GPIO                      GPIO(44)        // GPIO44
#define USB3_VBUS_GPIO_POLARITY		        (1)

#define USB0_OC_GPIO                        GPIO(45)         // GPIO45
#define USB0_OC_GPIO_POLARITY		        (0)
#define USB1_OC_GPIO                        GPIO(46)         // GPIO46
#define USB1_OC_GPIO_POLARITY		        (0)
#define USB3_OC_GPIO                        GPIO(47)         // GPIO47
#define USB3_OC_GPIO_POLARITY		        (0)

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
#define NPTV_SOURCE_TABLE                   "LGE/source_table.c"
#endif

//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef GPIO_INCLUDE_FILE
#define GPIO_INCLUDE_FILE                   "LGE/LGE_gpio.h"
#endif

#ifndef PINMUX_INCLUDE_FILE
#define PINMUX_INCLUDE_FILE                 "LGE/LGE_pin_setup.h"
#endif

//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                      "mtk/hdmi_conf.c"
#endif

#ifndef EDID_INCLUDE_FILE
#define EDID_INCLUDE_FILE                   "LGE/LGE_edid.h"
#endif

/*
 * default VGA table
*/
#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE                    "LGE/LGE_vga.h"
#endif

#ifndef VFE_HW_075
#define VFE_HW_075  1   // 1 : HW is 0,75 ohm;  0:H/W is 18,56 ohm. mt5362 default is 18,56 ohm
#endif

//----------------------------------------------------------------------------
// TVD configuration
//----------------------------------------------------------------------------
#define TVD_USE_VPRES4     (0)

//----------------------------------------------------------------------------
// Tcon Customization
//----------------------------------------------------------------------------
// support LVDS and EPI both
#define PANEL_DISPLAY_TYPE_ALLOWED_MASK ((1<<PANEL_DISPLAY_TYPE_LVDS) | (1 << PANEL_DISPLAY_TYPE_EPI))

#ifndef TCON_CUST_FILE
#define TCON_CUST_FILE                      "LGE/LGE_tcon_cust.c"
#endif

//----------------------------------------------------------------------------
// SCART out configuration
//----------------------------------------------------------------------------
#ifndef TVE_INCLUDE_FILE
#define TVE_INCLUDE_FILE                "LGE/LGE_tve.c"
#endif

//----------------------------------------------------------------------------
// DMX configuration
//----------------------------------------------------------------------------
#ifndef DMX_INCLUDE_FILE
#define DMX_INCLUDE_FILE               "mtk/mtk_dmx.h"
#endif

//----------------------------------------------------------------------------
// Loader configuration
//----------------------------------------------------------------------------
#ifdef CC_USE_128MB_NAND
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (128*1024*1024) /* 128 MB */
#else
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (256*1024*1024) /* 256 MB */
#endif

//----------------------------------------------------------------------------
// Flash Partition configuration
//----------------------------------------------------------------------------
#ifdef CC_USE_128MB_NAND
#define NAND_FLASH_TOTAL_SIZE            (128*1024*1024)
#else
#define NAND_FLASH_TOTAL_SIZE            (1024*1024*1024) /* 1G */
#endif

#if !defined(__KERNEL__) && !defined(CC_LOAD_UBOOT)
// for 256Kbyte nand alignment
#define NAND_FLASH_CHANNELLIST_SIZE             (256*1024)
#define NAND_FLASH_PQ_SIZE                      (256*1024)
#define NAND_FLASH_AQ_SIZE                      (256*1024)
#else

#ifndef LINUX_NAND_USE_SDM
    #define NAND_PART_SIZE_0                 (0x0200000)                    /* loader image              */
    #define NAND_PART_SIZE_1                 (0x0200000)                    /* nand_1 - uboot env        */
    #define NAND_PART_SIZE_2                 (0x0200000)                    /* nand_2 - sig A            */
    #define NAND_PART_SIZE_3                 (0x0200000)                    /* nand_3 - sig B            */
    #define NAND_PART_SIZE_4                 (0x0200000)                    /* nand_4 - kernel A         */
    #define NAND_PART_SIZE_5                 (0x0200000)                    /* nand_5 - kernel B         */
    #define NAND_PART_SIZE_6                 (0x0A00000)                    /* nand_6 - rootfs A         */
    #define NAND_PART_SIZE_7                 (0x0A00000)                    /* nand_7 - rootfs B         */
    #define NAND_PART_SIZE_8                 (0x1900000)                    /* nand_8 - basic            */
    #ifdef CC_USE_128MB_NAND
    #define NAND_PART_SIZE_9                 (0x3000000)                    /* nand_9 - 3rd              */
    #else
    #define NAND_PART_SIZE_9                 (0xB000000)                    /* nand_9 - 3rd              */
    #endif
    #define NAND_PART_SIZE_10                (0x0A00000)                    /* nand_10 - perm            */
    #define NAND_PART_SIZE_11                (0x0040000)                    /* nand_11 - reserved        */
    #define NAND_PART_SIZE_12                (0x0200000)                    /* nand_12 - channel list A  */
    #define NAND_PART_SIZE_13                (0x0200000)                    /* nand_13 - channel list B  */
    #define NAND_PART_SIZE_14                (0x0200000)                    /* nand_14 - Flash PQ        */
    #define NAND_PART_SIZE_15                (0x0200000)                    /* nand_15 - Flash AQ        */
    #define NAND_PART_SIZE_16                (0x0300000)                    /* nand_16 - Capture Logo    */
    #if defined(CI_PLUS_SUPPORT)
    #define NAND_PART_SIZE_17                (0x0040000)                    /* nand_17 - ci    */
    #else
    #define NAND_PART_SIZE_17                (0)                            /* nand_17 - reserved    */
    #endif

    #define NAND_FLASH_STATIC_LAST_PART_ID   (17)
    #define NAND_FLASH_STATIC_PART_SIZE \
           ((NAND_PART_SIZE_0) + \
            (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + \
            (NAND_PART_SIZE_4) + (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + \
            (NAND_PART_SIZE_7) + (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + \
            (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11) + (NAND_PART_SIZE_12) + \
            (NAND_PART_SIZE_13) + (NAND_PART_SIZE_14) + (NAND_PART_SIZE_15) + \
            (NAND_PART_SIZE_16) + (NAND_PART_SIZE_17))
#else
    #define NAND_PART_SIZE_0                 (0x0200000)                    /* loader image              */
    #define NAND_PART_SIZE_1                 (0x0200000)                    /* nand_1 - uboot env        */
    #define NAND_PART_SIZE_2                 (0x0200000)                    /* nand_2 - sig A            */
    #define NAND_PART_SIZE_3                 (0x0200000)                    /* nand_3 - sig B            */
    #define NAND_PART_SIZE_4                 (0x0200000)                    /* nand_4 - kernel A         */
    #define NAND_PART_SIZE_5                 (0x0200000)                    /* nand_5 - kernel B         */
    #define NAND_PART_SIZE_6                 (0x0A00000)                    /* nand_6 - rootfs A         */
    #define NAND_PART_SIZE_7                 (0x0A00000)                    /* nand_7 - rootfs B         */
    #define NAND_PART_SIZE_8                 (0x1900000)                    /* nand_8 - basic            */
    #ifdef CC_USE_128MB_NAND
    #define NAND_PART_SIZE_9                 (0x3B00000)                    /* nand_9 - 3rd              */
    #else
    #define NAND_PART_SIZE_9                 (0xBB00000)                    /* nand_9 - 3rd              */
    #endif
    #define NAND_PART_SIZE_10                (0x0A00000)                    /* nand_10 - perm            */
    #define NAND_PART_SIZE_11                (0x0040000)                    /* nand_11 - reserved        */
    #define NAND_PART_SIZE_12                (0x0040000)                    /* nand_12 - channel list A  */
    #define NAND_PART_SIZE_13                (0x0040000)                    /* nand_13 - channel list B  */
    #define NAND_PART_SIZE_14                (0x0040000)                    /* nand_14 - Flash PQ        */
    #define NAND_PART_SIZE_15                (0x0040000)                    /* nand_15 - Flash AQ        */
    #define NAND_PART_SIZE_16                (0x0100000)                    /* nand_16 - Capture Logo    */
    #if defined(CI_PLUS_SUPPORT)
    #define NAND_PART_SIZE_17                (0x0040000)                    /* nand_17 - ci    */
    #else
    #define NAND_PART_SIZE_17                (0)                            /* nand_17 - reserved    */
    #endif

    #define NAND_FLASH_STATIC_LAST_PART_ID   (11)

    #define NAND_FLASH_STATIC_PART_SIZE \
            ((NAND_PART_SIZE_0) + \
            (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + \
            (NAND_PART_SIZE_4) + (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + \
            (NAND_PART_SIZE_7) + (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + \
            (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11))
#endif
#endif

/*
 * default quality table
*/
#ifndef QTYTBL_INCLUDE_FILE
#define QTYTBL_INCLUDE_FILE                 "LGE/QTY_DEFAULT/LGE_QtyTbl.h"
#endif

#define NAND_FLASH_APPIMAGE_SIZE            (8*1024*1024)//(6*1024*1024)  // temporary solution [DTV00212237]

#ifdef SUPPORT_FLASH_PQ
#ifdef LINUX_TURNKEY_SOLUTION
#define FLASH_PQ_USE_NAND_FLASH            (1)
#define FLASH_PQ_BASE_PARTITION            (14)
#endif
#endif // #if SUPPORT_FLASH_PQ

#ifdef SUPPORT_FLASH_AQ
#ifdef LINUX_TURNKEY_SOLUTION
#define FLASH_AQ_BASE_PARTITION            (15)
#endif
#endif // #if SUPPORT_FLASH_AQ

/*
 * default color parameter
*/
#ifndef COLOR_PARAM_INCLUDE_FILE
#define COLOR_PARAM_INCLUDE_FILE            "LGE/PANEL_DEFAULT/color_param.h"
#endif

#ifndef CUSTOM_VIDEO_DRV_BODY_FILE
#define CUSTOM_VIDEO_DRV_BODY_FILE          "LGE/LGE_video_drv.c"
#endif

/*
 * dynamic backlight parameter
*/

#else /* CONFIG_DEFAULT_H */
#error "Cannot include multi-config files"
#endif /* CONFIG_DEFAULT_H */

/*
 * OD setting
*/

#define SUPPORT_OD  (0)
