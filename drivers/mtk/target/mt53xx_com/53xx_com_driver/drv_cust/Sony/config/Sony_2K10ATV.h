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

#define CUSTOMER_NAME		"sony"

//----------------------------------------------------------------------------
// Default Tuner setting
//----------------------------------------------------------------------------

/*
#define cALPS_TEQE3       0x7B (for GA)
#define cALPS_TDAU4D01A   0x7C (for ATSC model)
#define cALPS_TEQH3L01A   0x7D (for Latim)
*/

#define DEFAULT_TUNER_CTRL			(0)
#define DEFAULT_TUNER_TYPE_DEFAULT	(0x7B)
#define DEFAULT_TUNER_TYPE_OFST0	(0x4CD)
#define DEFAULT_TUNER_TYPE_OFST1	(0x4CE)
#define DEFAULT_TUNER_TYPE_OFST2	(0x4CF)
#define DEFAULT_TUNER_CTRL_ANA		(0)

#define EEPROM_NVRAM_VERION_OFFSET	0x393 //(DRVCUST_EEPROM_DRIVER_SIZE - 13) last (6~13) is edid version
#define EEPROM_NVRAM_VERION_LENGTH	(8)  
#define EEPROM_EDID_VERION_OFFSET	0x38B //(DRVCUST_EEPROM_DRIVER_SIZE - 21) last (14~21) is edid version

//----------------------------------------------------------------------------
// flash layout setting
//----------------------------------------------------------------------------
#ifdef CC_CVT
#define NAND_FLASH_TOTAL_SIZE               (64*1024*1024)
#else
#define NAND_FLASH_TOTAL_SIZE               (32*1024*1024)
#endif

#ifndef SUPPORT_FLASH_PQ
#define SUPPORT_FLASH_PQ
#endif

#define FLASH_PQ_BASE_PARTITION                 (7)
#define FLASH_PQ_BASE_ADDRESS                   (0)
#define FLASH_PQ_BASE_OFFSET				    (0)
#define FLASH_PQ_BLOCK_SIZE					    (1)
#define HDCP_NAND_FLASH_PART_ID                 (0)
#define FLASH_AQ_BASE_PARTITION                 (10)

#ifdef CC_CVT
#define NAND_FLASH_PQ_SIZE                      (0x200000)
#define NAND_FLASH_SINGLE_CHANNELLIST_SIZE      (128*1024)
#define NAND_FLASH_APPIMAGE_SIZE                (0x900000)
#else
#define NAND_FLASH_PQ_SIZE                      (0x1A0000)
#define NAND_FLASH_SINGLE_CHANNELLIST_SIZE      (128*1024)
#define NAND_FLASH_APPIMAGE_SIZE                (0x600000)
#endif



#define NAND_PART_SIZE_0        (NAND_FLASH_LOADER_SIZE)                /* loader */
#define NAND_PART_SIZE_1        (NAND_FLASH_APPIMAGE_SIZE)              /* appimg */
#define NAND_PART_SIZE_2        (0)                                     /* up image */
#ifdef CC_DUAL_IMAGE_FLASH
#define NAND_PART_SIZE_3        (NAND_FLASH_APPIMAGE_SIZE)              /* reserved */
#else
#define NAND_PART_SIZE_3        (0)      // reserved
#endif
#define NAND_PART_SIZE_4        (0)                                     /* up image */
#define NAND_PART_SIZE_5        (NAND_FLASH_SINGLE_CHANNELLIST_SIZE)    /* channel list */
#define NAND_PART_SIZE_6        (NAND_FLASH_SINGLE_CHANNELLIST_SIZE)    /* channel list */
#define NAND_PART_SIZE_7        (NAND_FLASH_PQ_SIZE)                    /* flash PQ */
#define NAND_PART_SIZE_8        (0x1600000)                             /* flash fs: 22MB */
#define NAND_PART_SIZE_9        (0x10000)                                /* HDCP: 4KB */
#define NAND_PART_SIZE_10       (0x10000)                               /* flash AQ:  2KB*/

//----------------------------------------------------------------------------
// Initial dram settings
//----------------------------------------------------------------------------

#ifndef CC_DRAM_STRAP_MODE
#define CC_DRAM_STRAP_MODE             (2) //0: Dynamic detect Dram Strap; 1: single Dram; 2: Dual Dram
#endif

#define DEFAULT_DRAM_COLADDR                (COL_ADDR_BIT_10)
#define DEFAULT_DRAM_8_BANKS                0 

#if (CC_DRAM_STRAP_MODE==0)
#define IS_DRAM_TWO_CHIP 		    (CKGEN_READ32(0x520) & (1U << 16))  // 0: DDR_II_x1; 1: DDR_II_x2
#else
#define IS_DRAM_TWO_CHIP 	       ((CC_DRAM_STRAP_MODE ==1)? 0:1)   
#endif 

#define DEFAULT_DRAM_TYPE           ( (IS_DRAM_TWO_CHIP )?  DDR_II_x2 : DDR_II_x1) //calvin add 20090609 
#define DEFAULT_DDR_CLOCK           ( (IS_DRAM_TWO_CHIP )?  (783000000): (864000000))


//----------------------------------------------------------------------------
// FBM setup
//----------------------------------------------------------------------------
#ifndef FBM_MEM_CFG_SIZE
#define FBM_MEM_CFG_SIZE           ((IS_DRAM_TWO_CHIP)? FBM_MEM_CFG_MT5387_VERF: FBM_MEM_CFG_MT5388_64MB_ATV_WXGA)
#endif /* FBM_MEM_CFG_SIZE */


//----------------------------------------------------------------------------
// Default Panel setting
//----------------------------------------------------------------------------
#ifndef DEFAULT_PANEL_SELECT
#define DEFAULT_PANEL_SELECT                (PANEL_DEFAULT)
#endif
#define BACKLIGHT_GPIO                      (GPIO(4))   // GPIO_4
#define BACKLIGHT_ON_VAL                    (1)
#define PANELPOWER_GPIO                     (GPIO(5))   // GPIO_5
#define PANELPOWER_ON_VAL                   (1)
#define PANEL_BACKLIGHT_PWM_PORT            (2)
//#define PANEL_BACKLIGHT_PWM_PIN             (PINMUX_GPIO_00)
//#define PANEL_BACKLIGHT_PWM_GPIO            (1)
// disable spread spectrum for vga output
#define CONTROL_BACKLIGHT_PWM               (0)
#ifndef CC_CVT
#define SUPPORT_BACKLIGHT_DIMMING_INVERT    (1)
#else
#define SUPPORT_BACKLIGHT_DIMMING_INVERT    (0)
#endif

#define SPREAD_SPECTRUM_PERMILLAGE          (0)

#define PDWNC_POLARITY                      (0)
#define CC_HVSYNC_ALIGN_WA
                  
#define DDDS_ERROR_LIMIT    (9)
#define DDDS_LOCK_OTHERS   (0)

#ifndef LOAD_PANEL_FROM_FLASH_CUSTOM
//#define LOAD_PANEL_FROM_FLASH_CUSTOM                   ATV_PanelLoadTableFromFlash
#endif

//----------------------------------------------------------------------------
// D-interface Customization
//----------------------------------------------------------------------------
#ifndef DRV_CUSTOM_CUST_FILE
#define DRV_CUSTOM_CUST_FILE                        "Sony/ATV_d_custom.c"
#endif

#ifndef DRV_CUSTOM_CUST_SPEC_SET
#define DRV_CUSTOM_CUST_SPEC_SET                    d_ATV_cust_spec_set
#endif

#ifndef DRV_CUSTOM_CUST_SPEC_GET
#define DRV_CUSTOM_CUST_SPEC_GET                    d_ATV_cust_spec_get
#endif

//----------------------------------------------------------------------------
// Audio Customization
//----------------------------------------------------------------------------
#define AUDIO_INCLUDE_FILE                  "Sony/ATV_aud.h"
#define AUDIO_Mute_Gpio_Num                 (GPIO(29))      // PTSI_D5
#define AUDIO_Amp_Gpio_Num                  (GPIO(30))      // PTSI_D6
#define AUDIO_MUTE_GPIO_POLARITY            AUD_MUTE_GPIO_LOW_ENALBE
#define AUDIO_AMP_GPIO_POLARITY             AUD_AMP_GPIO_HIGH_ENALBE
//#define AUDIO_DIGITAL_AMP_RESET_GPIO        (GPIO(7))
#define AUDIO_DIGITAL_AMP_RESET_GPIO        (0xffffffff)
#define AUDIO_DIGITAL_AMP_RESET_GPIO_POLARITY    1
#define AUDIO_HP_PLUG_IN_GPIO               (GPIO(23))      //PTSI_SYNC
#define AUDIO_HP_PLUG_IN_POLARITY           (1)
#define AUDIO_IEC_COPYRIGHT                 NULL
#define AUDIO_IEC_CATEGORY                  NULL
#define AUDIO_IEC_WORD_LENGTH               NULL
#define AUDIO_ADAC_USAGE                    (&_arSony2K10AdacUsage)
#define AUDIO_INPUT_MUX_SEL_TABLE           (&_rSony2K10AudInputMuxTable)
#define AUDIO_INTER_DAC_USAGE               (&_arSony2K10InterDacUsage)
#define AUDIO_PAL_FACTORY_MODE        (&_arSony2K10PalFactory)
#define AUDIO_MTS_FACTORY_MODE        (&_arSony2K10MtsFactory)

#define AUDIO_INPUT_SRC_LINE_OUT_VOL    (&_aai2SonyAudInputSrcLineOutVol)
#define AUDIO_CHL_PEQ_FQ_CFG            (&_aai1SonyAudPeqFq)
#define AUDIO_CHL_PEQ_BW_CFG            (&_aai1SonyAudPeqBw)
#define AUDIO_CHL_PEQ_GAIN_CFG          (&_aai1SonyAudPeqGain)
#define AUDIO_CHL_BASS_TREBLE_CFG       (&_aai1SonyAudBassTrebleTable)
#define AUDIO_CHL_VS_MODE_CFG           (&_aai1SonyAudVSTable)
#define AUDIO_TV_SYS_FINE_TUNE_VOL      (&_au1SonyTVFineTuneVol)
#define AUDIO_CHANNEL_VOL_TABLE         (&_au4SonyChannelVolumeTable)
#define AUDIO_VOL_TABLE                 (&_au4SonyVolumeTable)
#define AUD_CHL_SOUND_MODE_CFG          (&_arSonySoundModeCase)
#define AUDIO_LIMITER_FIX_THRESHOLD     (0xBD200)
#define AUDIO_INPUT_SRC_VOL             (&_aai2SonyAudInputSrcVol)
#define AUDIO_CHL_VOLUME_OFFSET_MAP     (&_aai4SonyAudVolOffsetMap)
#define AUDIO_LINE_OUT_FIX_VOL          (50)
//#define AUDIO_FLASH_AQ_DATA                 (&arSonyAudNvmData)
//#define AUDIO_FLASH_AQ_DATA_SIZE            (sizeof(arSonyAudNvmData))
#define AUDIO_FLASH_AQ_DATA_DSCRPT          (&_arSonyAudNvmDscrpt)
#define AUDIO_FLASH_AQ_DATA_DSCRPT_SIZE     (u1SonyAudAQDscrptSize)
//----------------------------------------------------------------------------
// Other GPIO customization
//----------------------------------------------------------------------------
#define FLAG_EXT_STATUS                     (1)
#define STDBY_USE_PWR_BTN_CNT               (0)
#define STDBY_USE_PREV_PWR_STATE            (1)


#define POWER_LED_GPIO                      (GPIO(3))
#define POWER_LED_ON_VAL                    (1)
#ifndef CC_CVT
#define BACKUP_LED_GPIO                      ADIN(2)//(SERVO(0))
#else
#define BACKUP_LED_GPIO                     OPCTRL(3)//(SERVO(0))
#endif
#define BACKUP_LED_ON_VAL                (1)

#define T8032_CTRL_ERR_LED                  BACKUP_LED_GPIO
#define T8032_CTRL_ERR_LED_ON_VAL           BACKUP_LED_ON_VAL
// #define POWER_CTRL_GPIO
// #define POWER_CTRL_ON_VAL

//----------------------------------------------------------------------------
// EEPROM customization
//----------------------------------------------------------------------------
#define SYSTEM_EEPROM_BUS                   (0)
#define SYSTEM_EEPROM_ADDRESS               (0xA4)
#define SYSTEM_EEPROM_SIZE                 (16*1024)// (4096)
#define SYSTEM_EEPROM_CLKDIV                (0x100)
#define SYSTEM_EEPROM_PAGESIZE              (32)
#define SYSTEM_EEPROM_WP_GPIO               (GPIO(57))  // OPWM1
#define SYSTEM_EEPROM_WP_ENABLE_POLARITY    (0)
#define SYSTEM_EEPROM_WAIT_TIME              (10)
#define HDCP_EEPROM_BUS                     (0)
#define HDCP_EEPROM_ADDRESS                 (0xA4)
#define EDID_EEPROM_WP_GPIO			        (GPIO(17))  // AOSDATA3
#define EDID_EEPROM_WP_ENABLE_POLARITY      (0)

#define EEPROM_CEC_MENU_LANGUAGE_OFFSET         (0x498)
#define EEPROM_CEC_ENABLE_OFFSET      			(0x49C)
#define EEPROM_CEC_ONE_TOUCH_PLAY_ENABLE_OFFSET (0x49D)

#define PANEL_INDEX_FROM_EEPROM             (1)
//----------------------------------------------------------------------------
/// SIF
//----------------------------------------------------------------------------
#define SIF_ENABLE_SCL_STRETCH              (1)
#define SIF_DISABLE_OPEN_DRAIN              (0)
#define SIF_ENABLE_PDWNC_MASTER             (0)
#define SIF_INCLUDE_FILE               "Sony/ATV_sif_cfg.h"

// [DTV00063436], jiewen
// HDMI EDID 1 EERPOM GPIO pin configuration.
#ifndef SIF_EDID1_SDA
#define SIF_EDID1_SDA  (GPIO(214))
#endif
#ifndef SIF_EDID1_SCL
#define SIF_EDID1_SCL  (GPIO(219))
#endif

// HDMI EDID 2 EERPOM GPIO pin configuration.
#ifndef SIF_EDID2_SDA
#define SIF_EDID2_SDA  (GPIO(206))
#endif
#ifndef SIF_EDID2_SCL
#define SIF_EDID2_SCL  (GPIO(207))
#endif

// HDMI EDID 3 EERPOM GPIO pin configuration.
#ifndef SIF_EDID3_SDA
#define SIF_EDID3_SDA  (GPIO(210))
#endif
#ifndef SIF_EDID3_SCL
#define SIF_EDID3_SCL  (GPIO(211))
#endif
/////////////////////////////////////
//----------------------------------------------------------------------------
// USB Customization
//----------------------------------------------------------------------------
#define USB0_VBUS_GPIO                      (GPIO_USB_PWR_EN0)
#define USB0_VBUS_GPIO_POLARITY		        (1)

#define USB0_OC_GPIO                        (GPIO_USB_PWR_ERR0)
#define USB0_OC_GPIO_POLARITY		        (0)

//#define USB_DISABLE_SUSPEND_MODE /* Disable USB suspend, share 240 Mhz with H.264.*/
//----------------------------------------------------------------------------
// Button Customization
//----------------------------------------------------------------------------
#define FLAG_BUTTON_SUPPORT                 (1)
#define BUTTON_INCLUDE_FILE                 "Sony/ATV_button.h"
#define GPIO_INCLUDE_FILE                   "Sony/ATV_gpio.h"

//----------------------------------------------------------------------------
// PDWN external control
//----------------------------------------------------------------------------
#define PDWNC_EXT_IMPL                      "Sony/ATV_pdwnc_ext.c"
 
//----------------------------------------------------------------------------
// Customer power on/off sequence
//----------------------------------------------------------------------------
#define POWER_SEQUENCE_EXT_FILE             "Sony/ATV_PowerSeq.c"

//----------------------------------------------------------------------------
// Boot-up Logo Customization 
//----------------------------------------------------------------------------
#define DRV_LOGO_H_FILE                 "Sony/ATV_DrvLogo.h"
#define LOADER_MUSIC_H_FILE             "Sony/ATV_DrvLogoMusic.h"


//----------------------------------------------------------------------------
// MTK IR only setting
//----------------------------------------------------------------------------
//#define IR_INCLUDE_FILE                     "mtk/mtk_ir.h"
#define IR_INCLUDE_FILE                     "Sony/mtk_ir_sony.h"

#ifndef NPTV_SOURCE_TABLE
#define NPTV_SOURCE_TABLE                   "Sony/source_table.c"
#endif
#define FLAG_IR_POWERON_WAKEUP    (1)
//----------------------------------------------------------------------------
// default HDMI GPIO control
//----------------------------------------------------------------------------
#ifndef HDMI_GPIO_CONF
#define HDMI_GPIO_CONF                      "Sony/ATV_hdmi_conf.c"
#endif
//----------------------------------------------------------------------------
// default T8032 FW Properties
//----------------------------------------------------------------------------
#ifndef T8032UP_INCLUDE_FILE
#define T8032UP_INCLUDE_FILE                "Sony/ATV_t8032fw.h"
//#define T8032UP_INCLUDE_FILE                "mtk/mtk_t8032fw.h"
#define T8032UP_OFFSET                      au1T8032FW
#define T8032UP_SIZE                        16384
#endif

/*
 * default VGA table
*/
#ifndef VGA_INCLUDE_FILE
#define VGA_INCLUDE_FILE                    "Sony/ATV_vga.h"
#endif

#ifndef VGA_AUTOCOLOR_TABLE_INCLUDE_FILE
#define VGA_AUTOCOLOR_TABLE_INCLUDE_FILE                    "Sony/ATV_vga_autocolor_table.h"
#endif

/*
 * default TVE File
 */
#if defined(CC_SUPPORT_TVE_CTRL_BY_DRIVER)||defined(CC_SUPPORT_TVE_CTRL_BY_MW)
#define TVE_INCLUDE_FILE                    "mtk/mtk_tve.c"
#endif


/*
 * FlashPQ setting
*/

#if SUPPORT_FLASH_PQ
    	#ifdef CC_NAND_ENABLE 
    		#define FLASH_PQ_USE_NAND_FLASH  		(1)
    	#else
    		#define FLASH_PQ_USE_NAND_FLASH  		(0)
#endif

        #define SUPPORT_FLASH_PQ_ENABLE_GAMMA		    (1)
        #define SUPPORT_FLASH_PQ_GAMMA_X3               (0)
        #define SUPPORT_FLASH_PQ_ENABLE_SCE			    (1)
        #define SUPPORT_FLASH_PQ_ENABLE_QTY			    (1)
        #define SUPPORT_FLASH_PQ_ENABLE_QTY_MAX_MIN	    (1)
        #define SUPPORT_FLASH_PQ_ENABLE_QTY_SMART_PIC	(1)
        #define SUPPORT_LINK_M4_CONTRAST_SATURATION	    (0)
#endif // #if SUPPORT_FLASH_PQ

/*
 * CVBS level 216
*/
#define TVD_SUPPORT_OUTPUT_216 (1)
/*
 * SCE MAX/BASE
*/
#define FLASH_PQ_SCE_MAX  (1)
/*
 * NVM Gamma
*/
#define SUPPORT_NVM_GAMMA (1)

/*
 * Use Linear Interpolation istead of Low/Mid/High Directly for NR
*/
#define UI_QTY_USE_LINEAR_MAP       (1)

/*
 * Gamma Top Point
*/
#define SUPPORT_GAMMA_TOP_POINT (1)

/*
 * default video driver file
*/
#define CUSTOM_VIDEO_DRV_BODY_FILE        "Sony/Sony_video_drv.c"

/*
 * default quality table
*/
#define QTYTBL_INCLUDE_FILE               "Sony/QTY_Sony/Sony_QtyTbl.h"

#ifdef SUPPORT_FLASH_PQ
#endif // #if SUPPORT_FLASH_PQ

/*
 * default color parameter
*/
#define COLOR_PARAM_INCLUDE_FILE            "Sony/PANEL_Sony/color_param.h"
/*
 * dynamic backlight parameter
*/

#define VFE_HW_SONY  1   // 1 : SONY HW is 0,75 ohm
#define VFE_HW_SONY_YPbPrMapToVGA  1   // 1 : SONY YPbPr map to VGA
#define EFUSE_AUTOCOLOR 0 // 1 : Use E Fuse Autocolor
#else /* CONFIG_DEFAULT_H */
#error "Cannot include multi-config files"
#endif /* CONFIG_DEFAULT_H */

