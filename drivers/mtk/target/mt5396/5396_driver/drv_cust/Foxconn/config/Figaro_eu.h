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
#define DEFAULT_DRAM_TYPE                   DDR_III_x3//DDR_III_x4
#define DEFAULT_DDR_CLOCK                   (1296000000)

//#ifndef DRAM_PRIORITY_LIST
// Audio/0 > B2R/5 > NR/PSCAN/4 > Scpip/7 > OSD/3 > Mjc_in/13 > Mjc_out/14 > VLD1/9 > Vdec_mc/8 > CPU/6 > Gfx/11 > Demux/PVR/1 > VBI/3D/TVE/2 >VENC/12 > Test/15 > Unused/10
//#define DRAM_PRIORITY_LIST                  ((UINT8*)"0c9421a387fbd56e")
//#endif

//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#define TOTAL_MEM_SIZE                      (512 * 1024 * 1024)
#define DIRECT_FB_MEM_SIZE                  (50 * 1024 * 1024)
#define FB_MEM_SIZE                         (0 * 1024 * 1024)
#ifndef FBM_MEM_CFG_SIZE
//#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5396_DDR3x4)
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5396_3DTV)
/*
#ifdef CC_3D_SUPPORT
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5395_3DTV)
#else
#define FBM_MEM_CFG_SIZE                    (FBM_MEM_CFG_MT5395_DDR3x2_EU_NET)
#endif
*/
#endif /* FBM_MEM_CFG_SIZE */

#define LINUX_KERNEL_MEM_SIZE               (TOTAL_MEM_SIZE > (FBM_MEM_CFG_SIZE + DIRECT_FB_MEM_SIZE + FB_MEM_SIZE)) ?\
                                            ((TOTAL_MEM_SIZE - FBM_MEM_CFG_SIZE - DIRECT_FB_MEM_SIZE - FB_MEM_SIZE) & 0xfff00000) : (54 * 1024 * 1024)

#define DEFAULT_TARGET_COUNTRY              (COUNTRY_EU)  

//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT                (PANEL_DEFAULT)
#endif
#define BACKLIGHT_GPIO                      (GPIO(121))//david
#define BACKLIGHT_ON_VAL                    (1)//david
#define PANELPOWER_GPIO                     (GPIO(45))//david
#define PANELPOWER_ON_VAL                   (1)//david

// disable spread spectrum for vga output
#define SPREAD_SPECTRUM_PERMILLAGE          (0)

// pwm backlight adjustment setting
#define PANEL_BACKLIGHT_PWM_PORT            (2)//david

//#define PDWNC_POLARITY                      (1)//david
#define DEFAULT_TUNER_CTRL1                  (42)//TUNER RESET GPIO
//----------------------------------------------------------------------------
// Boot-up Logo Customization
//----------------------------------------------------------------------------
//#define DRV_LOGO_ENABLE                         (1)
#define DRV_LOGO_H_FILE                         "Foxconn/Foxconn_DrvLogo.h"
//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_INCLUDE_FILE                  "Foxconn/Figaro_aud.h"
//AUDIO_PD GPIO 206 (HIGH: MUTE) SP
//HP_MUTE GPIO 48 (HIGH: MUTE)
//SCART_MUTE GPIO 200 (HIGH: MUTE)
//#define AUDIO_Amp_Gpio_Num                (OPCTRL(6))
//#define AUDIO_AMP_GPIO_POLARITY           AUD_AMP_GPIO_HIGH_ENALBE
#define AUDIO_CUST_HP_GPIO_CTRL        (GPIO(48))
#define AUDIO_CUST_HP_POLARITY          AUD_AMP_GPIO_LOW_ENALBE
#define AUDIO_CUST_SPEC_GPIO_CTRL       (GPIO(200))
#define AUDIO_CUST_SPEC_POLARITY          AUD_AMP_GPIO_LOW_ENALBE
#define AUDIO_DIGITAL_AMP_RESET_GPIO        (GPIO(115))
#define AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY   AUD_GPIO_LOW_ENALBE
#define AUDIO_HP_PLUG_IN_GPIO               (GPIO(49))
#define AUDIO_HP_PLUG_IN_POLARITY        AUD_GPIO_HIGH_ENALBE

#define AUDIO_INPUT_MUX_SEL_TABLE       (&_rFigaroAudInputMuxTable)
#define AUDIO_ADAC_USAGE                      (&_arFigaroAdacUsage)
#define AUDIO_INTER_DAC_USAGE              (&_arFigaroInterDacUsage)
//#define AUDIO_Digital_Data_TABLE             (&_afgFigaroAudDigitalDataUsage)

//----------------------------------------------------------------------------
// Other GPIO customization
#define DEFAULT_TUNER_RFAGC_SWITCH_GPIO (OPCTRL(8))//MT5396 m2v1 use OPCTRL8
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
#define SYSTEM_EEPROM_SIZE                  (0x4000)//david
#define SYSTEM_EEPROM_WP_GPIO               (GPIO(55)) //david// OPCTRL9.
#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (1)//david
//#define EDID_EEPROM_WP_GPIO               (GPIO(4))   // GPIO14.
//#define EDID_EEPROM_WP_ENABLE_POLARITY    (1)

//----------------------------------------------------------------------------
// European CI (Common Interface) customization
//----------------------------------------------------------------------------
//#define CI_8295_RESET_GPIO                (GPIO(35))  // It's GPIO_3 on schematic, but we should use 54. (reference to x_pinmux.h)
//#define CI_8295_RESET_POLARITY            (0)
#define CI_EXTERNAL_IC_INT_GPIO             (GPIO(84))  //david // Set:Interrupt Mode, UnSet:Polling Mode
//#define CI_5V_OC_GPIO                      (MT5135_GPIO(16)) // PAD_IF_AGC of mt5135
#define CI_5V_POWERON_GPIO                  (GPIO(48))//david
#define CI_5V_POWERON_POLARITY              (1)//david 
//#define CI_5V_OC_POLARITY                 (0)
#define CI_EXTERNAL_IC_DRIVING_CURRENT      (2)         // 2mA
#define CI_SYSTEM_CLOCK                     (72)        // 72 MHz
#define CI_TS_CLOCK                         (108)       // 108 MHz
#define CI_SPI_CLOCK                        (27)        // 72 MHz
//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
#define USB0_VBUS_GPIO                      GPIO(120)    // GPIO43.
#define USB0_VBUS_GPIO_POLARITY		          (0)
#define USB1_VBUS_GPIO                      GPIO(120)    // GPIO44
#define USB1_VBUS_GPIO_POLARITY		          (0)
/*#define USB2_VBUS_GPIO                      OPCTRL(11)  // OPCTRL11
#define USB2_VBUS_GPIO_POLARITY		          (1)
#define USB3_VBUS_GPIO                      OPCTRL(10)  // OPCTRL10.
#define USB3_VBUS_GPIO_POLARITY		          (1)
*/
#define USB0_OC_GPIO                        GPIO(118)    // ADIN 4.
#define USB0_OC_GPIO_POLARITY		            (0)
#define USB1_OC_GPIO                        GPIO(118)    // ADIN 4.
#define USB1_OC_GPIO_POLARITY		            (0)
/*#define USB2_OC_GPIO                        ADIN(5)     // ADIN 5.
#define USB2_OC_GPIO_POLARITY		            (0)
#define USB3_OC_GPIO                        ADIN(5)     // ADIN 5.
#define USB3_OC_GPIO_POLARITY		            (0)*/

//#define USB_DISABLE_SUSPEND_MODE /* Disable USB suspend, share 240 Mhz with H.264.*/
//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#define FLAG_BUTTON_SUPPORT                 (0)
#define BUTTON_INCLUDE_FILE                     "Foxconn/Figaro_button.h"
#ifndef T8032UP_INCLUDE_FILE
#define T8032UP_INCLUDE_FILE                "mtk/mtk_t8032fw.h"//"Foxconn/Figaro_t8032fw.h"
#define T8032UP_OFFSET                      au1T8032FW
#define T8032UP_SIZE                        24576//16384
#endif

//----------------------------------------------------------------------------
// MTK IR only setting
//----------------------------------------------------------------------------
//#define IR_INCLUDE_FILE                     "Foxconn/Figaro_ir.h"
#define IR_INCLUDE_FILE "Foxconn/Figaro_ir.h"
//#define IR_INCLUDE_FILE                     "mtk/mtk_ir.h"
#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE                   "Foxconn/Figaro_source_table.c"
#endif

//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                      "Foxconn/Figaro_hdmi_conf.c"
#endif

/*
 * default VGA table
*/
#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE                    "Foxconn/Figaro_vga.h"
#endif

//----------------------------------------------------------------------------
// SCART out configuration
//----------------------------------------------------------------------------
#ifndef TVE_INCLUDE_FILE
#define TVE_INCLUDE_FILE                    "Foxconn/Figaro_tve.c"
#endif

//----------------------------------------------------------------------------
// DMX configuration
//----------------------------------------------------------------------------
#ifndef DMX_INCLUDE_FILE
#define DMX_INCLUDE_FILE               "Foxconn/Figaro_dmx.h"
#endif

//----------------------------------------------------------------------------
// PCIE configuration
//----------------------------------------------------------------------------
#define PCIE_RESET_GPIO  OPCTRL4

//----------------------------------------------------------------------------
// Loader configuration
//----------------------------------------------------------------------------
#define MAX_UPGRADE_IMAGE_FILE_SIZE         (256*1024*1024)     /* 256 MB */

//----------------------------------------------------------------------------
// Flash Partition configuration
//----------------------------------------------------------------------------
#define PARTITION_TURNKEY "mt53xx-nand:" \
                          "2M(uboot),2M(uboot_env),2M(sigA),2M(sigB),"  \
                          "2M(kernelA),"   \
                          "2M(kernelB),"   \
                          "10M(rootfsA),"   \
                          "10M(rootfsB),"   \
                          "40M(basic)," \
                          "151M(rw_area)," \
                          "5M(perm)," \
                          "128k(reserved)," \
                          "256k(channelsA),256k(channelsB),"   \
                          "25M(pq),1M(aq)," \
                          "1M(panel)," \
                          "128k(ci)"

#define NAND_FLASH_TOTAL_SIZE               (256*1024*1024)

#if defined(DEFAULT_DB_SUPPORT)
    #define NAND_FLASH_DEFAULT_DB_SIZE      (0x0020000)         /* DVB-S default db    */
#endif

    #define NAND_PART_SIZE_0                (0x0200000)         /* loader image              */
    #define NAND_PART_SIZE_1                (0x0200000)         /* nand_1 - uboot env        */
    #define NAND_PART_SIZE_2                (0x0200000)         /* nand_2 - sig A            */
    #define NAND_PART_SIZE_3                (0x0200000)         /* nand_3 - sig B            */
    #define NAND_PART_SIZE_4                (0x0200000)         /* nand_4 - kernel A         */
    #define NAND_PART_SIZE_5                (0x0200000)         /* nand_5 - kernel B         */
    #define NAND_PART_SIZE_6                (0x0A00000)         /* nand_6 - rootfs A         */
    #define NAND_PART_SIZE_7                (0x0A00000)         /* nand_7 - rootfs B         */
    #define NAND_PART_SIZE_8                (0x2800000)         /* nand_8 - basic            */
#define NAND_PART_SIZE_9                    (0x9700000)         /* nand_9 - 3rd              */
    #define NAND_PART_SIZE_10               (0x0500000)         /* nand_10 - perm            */
    #define NAND_PART_SIZE_11               (0x0020000)         /* nand_11 - reserved        */
    #define NAND_PART_SIZE_12               (0x0040000)         /* nand_12 - channel list A  */
    #define NAND_PART_SIZE_13               (0x0040000)         /* nand_13 - channel list B  */
#define NAND_PART_SIZE_14                   (0x1900000)         /* nand_14 - Flash PQ        */
#define NAND_PART_SIZE_15                   (0x0100000)         /* nand_15 - Flash AQ        */
#define NAND_PART_SIZE_16                   (0x0100000)         /* nand_16 - Flash Panel     */
    #if defined(CI_PLUS_SUPPORT)
    #define NAND_PART_SIZE_17               (0x0020000)         /* nand_17 - ci              */
    #else
    #define NAND_PART_SIZE_17               (0)                 /* nand_17 - reserved        */
    #endif
#if defined(DEFAULT_DB_SUPPORT)
    #define NAND_PART_SIZE_18               NAND_FLASH_DEFAULT_DB_SIZE      /* nand_18 - DVB-S default db */
#endif

    #define NAND_FLASH_STATIC_LAST_PART_ID  (11)

    #define NAND_FLASH_STATIC_PART_SIZE \
            ((NAND_PART_SIZE_0) + \
            (NAND_PART_SIZE_1) + (NAND_PART_SIZE_2) + (NAND_PART_SIZE_3) + \
            (NAND_PART_SIZE_4) + (NAND_PART_SIZE_5) + (NAND_PART_SIZE_6) + \
            (NAND_PART_SIZE_7) + (NAND_PART_SIZE_8) + (NAND_PART_SIZE_9) + \
            (NAND_PART_SIZE_10) + (NAND_PART_SIZE_11))

/* The NAND_PART_NAME_0-15 use default names. */
#define NAND_PART_NAME_16                   "panel"
#define NAND_PART_NAME_17                   "ci"

#ifdef LINUX_TURNKEY_SOLUTION
    #if defined(DEFAULT_DB_SUPPORT)
        #define NAND_PART_NAME_18           "dvbsDB"
    #endif
#endif

/*
 * default quality table
*/
#ifndef QTYTBL_INCLUDE_FILE
//#define QTYTBL_INCLUDE_FILE               "mtk/QTY_DEFAULT/mtk_QtyTbl.h"
#endif

#define NAND_FLASH_APPIMAGE_SIZE            (8*1024*1024)//(6*1024*1024)    // temporary solution [DTV00212237]

#ifdef SUPPORT_FLASH_PQ
#ifdef LINUX_TURNKEY_SOLUTION
#define FLASH_PQ_USE_NAND_FLASH             (1)
#define FLASH_PQ_BASE_PARTITION             (14)
#endif
#endif // #if SUPPORT_FLASH_PQ

#ifdef SUPPORT_FLASH_AQ
#ifdef LINUX_TURNKEY_SOLUTION
#define FLASH_AQ_BASE_PARTITION             (15)
#endif
#endif // #if SUPPORT_FLASH_AQ
//#define PDWNC_EXT_IMPL                          "Foxconn/Figaro_pdwnc_ext.c"
#define PDWNC_EXT_IMPL                          "Foxconn/Figaro_pdwnc.h"
/*
 * default color parameter
*/
#ifndef COLOR_PARAM_INCLUDE_FILE
//#define COLOR_PARAM_INCLUDE_FILE          "mtk/PANEL_DEFAULT/color_param.h"
#endif

/*
 * dynamic backlight parameter
*/

/*
 * OD setting
*/

#define SUPPORT_OD                          (0)

#else /* CONFIG_DEFAULT_H */
#error "Cannot include multi-config files"
#endif /* CONFIG_DEFAULT_H */

//----------------------------------------------------------------------------
// D-interface Customization
//----------------------------------------------------------------------------
#ifndef DRV_CUSTOM_CUST_FILE
#define DRV_CUSTOM_CUST_FILE                    "Foxconn/Figaro_d_custom.c"
#endif

#ifndef DRV_CUSTOM_CUST_SPEC_SET
#define DRV_CUSTOM_CUST_SPEC_SET                d_Figaro_cust_spec_set
#endif

#ifndef DRV_CUSTOM_CUST_SPEC_GET
#define DRV_CUSTOM_CUST_SPEC_GET                d_Figaro_cust_spec_get
#endif

#ifndef CUSTOM_VIDEO_DRV_BODY_FILE
#define CUSTOM_VIDEO_DRV_BODY_FILE              "Foxconn/video_drv/Figaro_video_drv.c"
#endif
#define GPIO_INCLUDE_FILE                       "Foxconn/Figaro_gpio.h"

//#define AUDIO_AMP_CUST_FILE_NAME        "mtk/amp_cust.c"


