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
#define DEFAULT_DRAM_TYPE                   DDR_III_x4
#define DEFAULT_DDR_CLOCK                   (1404000000)

//#ifndef DRAM_PRIORITY_LIST
// Audio/0 > B2R/5 > NR/PSCAN/4 > Scpip/7 > OSD/3 > Mjc_in/13 > Mjc_out/14 > VLD1/9 > Vdec_mc/8 > CPU/6 > Gfx/11 > Demux/PVR/1 > VBI/3D/TVE/2 >VENC/12 > Test/15 > Unused/10
//#define DRAM_PRIORITY_LIST                  ((UINT8*)"0bc4219387fad56e")
//#endif

//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#define TOTAL_MEM_SIZE                      (512 * 1024 * 1024)
#define DIRECT_FB_MEM_SIZE                  (50 * 1024 * 1024)
#define FB_MEM_SIZE                         (20 * 1024 * 1024)
#ifndef FBM_MEM_CFG_SIZE
//#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5396_DDR3x4)
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5396_3DTV)

#endif /* FBM_MEM_CFG_SIZE */

#define LINUX_KERNEL_MEM_SIZE               (TOTAL_MEM_SIZE > (FBM_MEM_CFG_SIZE + DIRECT_FB_MEM_SIZE + FB_MEM_SIZE)) ?\
                                            ((TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE - DIRECT_FB_MEM_SIZE - FB_MEM_SIZE) & 0xfff00000) : (54 * 1024 * 1024)

#define DEFAULT_TARGET_COUNTRY              (COUNTRY_EU)  

//#define LINUX_DEMO	//for linux demo program	//move to mt5396m2v1_linux.def

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT                (PANEL_DEFAULT)
#endif
#define BACKLIGHT_GPIO                      (GPIO(47))
#define BACKLIGHT_ON_VAL                    (0)
#define PANELPOWER_GPIO                     (OPCTRL(9))
#define PANELPOWER_ON_VAL                   (1)

// disable spread spectrum for vga output
#define SPREAD_SPECTRUM_PERMILLAGE          (0)

// pwm backlight adjustment setting
#define PANEL_BACKLIGHT_PWM_PORT            (1)

#define PDWNC_POLARITY                      (0)

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_INCLUDE_FILE                  "mtk/mt5396m2v1_aud.h"
#define AUDIO_Mute_Gpio_Num                 (GPIO(51))
//#define AUDIO_Amp_Gpio_Num                (OPCTRL(3))
#define AUDIO_MUTE_GPIO_POLARITY            AUD_MUTE_GPIO_LOW_ENALBE
//#define AUDIO_AMP_GPIO_POLARITY           AUD_AMP_GPIO_HIGH_ENALBE
#define AUDIO_INPUT_MUX_SEL_TABLE           (&_rmt5396m2v1AudInputMuxTable)

//----------------------------------------------------------------------------
// Other GPIO customization
//RF AGC swtich GPIO
#define DEFAULT_TUNER_RFAGC_SWITCH_GPIO (OPCTRL(8))//d20110506_Haibo:MT5396 m2v1 eu use OPCTRL8.Please note:Just EU PCB need this pin
//----------------------------------------------------------------------------
#define FLAG_EXT_STATUS                     (0)
// #define POWER_LED_GPIO                   (OPCTRL(0))
// #define POWER_LED_ON_VAL                 (0)
// #define BACKUP_LED_GPIO                  (OPCTRL(0))
// #define BACKUP_LED_ON_VAL                (0)
// #define POWER_CTRL_GPIO
// #define POWER_CTRL_ON_VAL

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#define SYSTEM_EEPROM_SIZE                  (4096)
#define SYSTEM_EEPROM_WP_GPIO               (GPIO(46))     // OPCTRL9.
#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (1)
//#define EDID_EEPROM_WP_GPIO			    (GPIO(4))       // GPIO14.
//#define EDID_EEPROM_WP_ENABLE_POLARITY    (1)

//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
// 1. USB port using define (1)=using (0)=not using
#define USB0_PORT_USING               (1)
#define USB1_PORT_USING               (1) 
#define USB2_PORT_USING               (1) 
#define USB3_PORT_USING               (1)

// 2. USB +5V control pin define
#define USB0_VBUS_GPIO                      GPIO(43)        // GPIO43.
#define USB0_VBUS_GPIO_POLARITY		        (1)
#define USB1_VBUS_GPIO                      GPIO(44)        // GPIO44
#define USB1_VBUS_GPIO_POLARITY		        (1)
#define USB2_VBUS_GPIO                      OPCTRL(11)        // OPCTRL11
#define USB2_VBUS_GPIO_POLARITY		        (1)
#define USB3_VBUS_GPIO                      OPCTRL(10)        // OPCTRL10.
#define USB3_VBUS_GPIO_POLARITY		        (1)

// 3. USB over current detect pin define
#define USB0_OC_GPIO                        ADIN(4)         // ADIN 4.
#define USB0_OC_GPIO_POLARITY		        (0)
#define USB1_OC_GPIO                        ADIN(4)         // ADIN 4.
#define USB1_OC_GPIO_POLARITY		        (0)
#define USB2_OC_GPIO                        ADIN(4)         // ADIN 4.
#define USB2_OC_GPIO_POLARITY		        (0)
#define USB3_OC_GPIO                        ADIN(4)         // ADIN 4.
#define USB3_OC_GPIO_POLARITY		        (0)

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
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                      "mtk/hdmi_conf.c"
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
#define TCON_CUST_FILE                      "mtk/mt5396m2v1_tcon_cust.c"
#endif

//----------------------------------------------------------------------------
// SCART out configuration
//----------------------------------------------------------------------------
#ifndef TVE_INCLUDE_FILE
#define TVE_INCLUDE_FILE                    "mtk/mtk_tve.c"
#endif

//----------------------------------------------------------------------------
// DMX configuration
//----------------------------------------------------------------------------
#ifndef DMX_INCLUDE_FILE
#define DMX_INCLUDE_FILE               "mtk/mtk_dmx.h"
#endif

//----------------------------------------------------------------------------
// PCIE configuration
//----------------------------------------------------------------------------
#define PCIE_RESET_GPIO  OPCTRL4

//----------------------------------------------------------------------------
// Loader configuration
//----------------------------------------------------------------------------
//#ifdef CC_USE_256MB_NAND
#if 1
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (256*1024*1024)     /* 256 MB */
#else
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (128*1024*1024)     /* 128 MB */
#endif

//----------------------------------------------------------------------------
// NAND customization
//----------------------------------------------------------------------------
//#ifdef CC_USE_256MB_NAND
#if 1
#define NAND_FLASH_TOTAL_SIZE               (256*1024*1024)
#else
#define NAND_FLASH_TOTAL_SIZE               (128*1024*1024)
#endif

#define NAND_PART_END                       (16)                /* total 16 Partitions */

#ifndef LINUX_NAND_USE_SDM
    #define NAND_PART_SIZE_0                (0x0200000)         /* loader image              */
    #define NAND_PART_SIZE_1                (0x0200000)         /* nand_1 - uboot env        */
    #define NAND_PART_SIZE_2                (0x0200000)         /* nand_2 - sig A            */
    #define NAND_PART_SIZE_3                (0x0200000)         /* nand_3 - sig B            */
    #define NAND_PART_SIZE_4                (0x0600000)         /* nand_4 - kernel A         */
    #define NAND_PART_SIZE_5                (0x0600000)         /* nand_5 - kernel B         */
    #define NAND_PART_SIZE_6                (0x0A00000)         /* nand_6 - rootfs A         */
    #define NAND_PART_SIZE_7                (0x0A00000)         /* nand_7 - rootfs B         */
    #define NAND_PART_SIZE_8                (0x1900000)         /* nand_8 - basic            */
    //#ifdef CC_USE_256MB_NAND
    #if 1
    #define NAND_PART_SIZE_9                (0xA800000)         /* nand_9 - 3rd              */
    #else
    #define NAND_PART_SIZE_9                (0x3000000)         /* nand_9 - 3rd              */
    #endif
    #define NAND_PART_SIZE_10               (0x0500000)         /* nand_10 - perm            */
    #define NAND_PART_SIZE_11               (0x0020000)         /* nand_11 - reserved        */
    #define NAND_PART_SIZE_12               (0x0200000)         /* nand_12 - channel list A  */
    #define NAND_PART_SIZE_13               (0x0200000)         /* nand_13 - channel list B  */
    #define NAND_PART_SIZE_14               (0x0200000)         /* nand_14 - Flash PQ        */
    #define NAND_PART_SIZE_15               (0x0200000)         /* nand_15 - Flash AQ        */
    #define NAND_PART_SIZE_16               (0x0300000)         /* nand_16 - Capture Logo    */

    #define NAND_FLASH_STATIC_LAST_PART_ID  (16)
    #define NAND_FLASH_STATIC_PART_SIZE \
           ((NAND_PART_SIZE_0) + \
            (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + \
            (NAND_PART_SIZE_4) + (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + \
            (NAND_PART_SIZE_7) + (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + \
            (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11) + (NAND_PART_SIZE_12) + \
            (NAND_PART_SIZE_13) + (NAND_PART_SIZE_14) + (NAND_PART_SIZE_15) + \
            (NAND_PART_SIZE_16) + (NAND_PART_SIZE_17))
#else
    #define NAND_PART_SIZE_0                (0x0200000)         /* loader image              */
    #define NAND_PART_SIZE_1                (0x0200000)         /* nand_1 - uboot env        */
    #define NAND_PART_SIZE_2                (0x0200000)         /* nand_2 - sig A            */
    #define NAND_PART_SIZE_3                (0x0200000)         /* nand_3 - sig B            */
    #define NAND_PART_SIZE_4                (0x0600000)         /* nand_4 - kernel A         */
    #define NAND_PART_SIZE_5                (0x0600000)         /* nand_5 - kernel B         */
    #define NAND_PART_SIZE_6                (0x0A00000)         /* nand_6 - rootfs A         */
    #define NAND_PART_SIZE_7                (0x0A00000)         /* nand_7 - rootfs B         */
    #define NAND_PART_SIZE_8                (0x1900000)         /* nand_8 - basic            */
    //#ifdef CC_USE_256MB_NAND
    #if 1
    #define NAND_PART_SIZE_9                (0xB300000)         /* nand_9 - 3rd              */
    #else
    #define NAND_PART_SIZE_9                (0x3B00000)         /* nand_9 - 3rd              */
    #endif
    #define NAND_PART_SIZE_10               (0x0500000)         /* nand_10 - perm            */
    #define NAND_PART_SIZE_11               (0x0020000)         /* nand_11 - reserved        */
    #define NAND_PART_SIZE_12               (0x0040000)         /* nand_12 - channel list A  */
    #define NAND_PART_SIZE_13               (0x0040000)         /* nand_13 - channel list B  */
    #define NAND_PART_SIZE_14               (0x0020000)         /* nand_14 - Flash PQ        */
    #define NAND_PART_SIZE_15               (0x0020000)         /* nand_15 - Flash AQ        */
    #define NAND_PART_SIZE_16               (0x0100000)         /* nand_16 - Capture Logo    */

    #define NAND_FLASH_STATIC_LAST_PART_ID  (11)

    #define NAND_FLASH_STATIC_PART_SIZE \
            ((NAND_PART_SIZE_0) + \
            (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + \
            (NAND_PART_SIZE_4) + (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + \
            (NAND_PART_SIZE_7) + (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + \
            (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11))
#endif

//----------------------------------------------------------------------------
// Capture Logo Customization
//----------------------------------------------------------------------------

// driver logo setting
#ifdef CC_BOOT_VDEC_LOGO
#define DRV_LOGO_ENABLE                     (1)
#define BOOT_VDEC_LOGO_PART_SIZE            (0x100000)          /* Partitino Size: 1MB */

#define DRV_LOGO_VDEC_FLASH_ADDR            NAND_PART_OFFSET_16
#define DRV_LOGO_VDEC_FLASH_SIZE            BOOT_VDEC_LOGO_PART_SIZE
#define DRV_LOGO_VDEC_FLASH_SLOT_NUM        (2)
#define DRV_LOGO_VDEC_DEVICE_ID             (16)
#define NAND_PART_SIZE_15                   (3*1024*1024)       /*should shrink to 1MB when SDM is ready*/

#define DRV_LOGO_H_FILE                     "mtk/MediaTek_Mpeg2Logo.h"
#define DRV_POWER_MUSIC_H_FILE              "mtk/mtk_PowerMusic.h"      /* power on music. */
#endif /* CC_BOOT_VDEC_LOGO */

#define NAND_PART_NAME_0			"uboot"
#define NAND_PART_NAME_1			"uboot_env"
#define NAND_PART_NAME_2			"sigA"
#define NAND_PART_NAME_3			"sigB"
#define NAND_PART_NAME_4			"kernelA"
#define NAND_PART_NAME_5			"kernelB"
#define NAND_PART_NAME_6			"rootfsA"
#define NAND_PART_NAME_7			"rootfsB"
#define NAND_PART_NAME_8			"basic"
#define NAND_PART_NAME_9			"rw_area"
#define NAND_PART_NAME_10			"perm"
#define NAND_PART_NAME_11		    "susp"
#define NAND_PART_NAME_12			"channelA"
#define NAND_PART_NAME_13			"channelB"
#define NAND_PART_NAME_14			"pq"
#define NAND_PART_NAME_15			"aq"
#define NAND_PART_NAME_16			"logo"

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

