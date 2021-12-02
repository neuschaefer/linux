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
 * $Date  $
 * $RCSfile: eepdef.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _EEPDEF_H
#define _EEPDEF_H

#include "vendor.h"
#include "feature.h"
#include "video_timing.h"

#include "x_lint.h"

//lint --e{960}
//lint --e{961}
/*===================================================================*/
/* EEPROM position layout */
/*===================================================================*/

/*============================================================*/
/** BLock size definition */
/*============================================================*/
#define EEP_ADC_SIZE			16
#define EEP_AUTOCOLOR_SIZE		32
#ifdef CC_MT5363 
#define EEP_ETH_MAC_SIZE		14
#endif

#ifdef THE_3RD_PARTY_SW_SUPPORT
#define EEP_REV1_SIZE   		32
#define EEP_YDELAY_SIZE			16
#define EEP_REV2_SIZE		    16
#define EEP_UVDELAY_SIZE		16
#define EEP_REV3_SIZE		    16

#define EEP_BLUE_STRETCH_SIZE	16
#define EEP_ADAPTIVE_LUMN_SIZE	16
#define EEP_BWS_SIZE	        16
#define EEP_AUTOTV_NR_SIZE		128
#define EEP_AUTOTV_BL_SIZE		64
#define EEP_REGION_OFFSET       144 
#define EEP_REV4_SIZE 		     64
#define EEP_DEMO_START          (0x630 - 0x380)

enum
{
    EEP_DEMO_CONTRAST,
    EEP_DEMO_DCTRST,
    EEP_DEMO_SAT,
    EEP_DEMO_NOISE,
    EEP_DEMO_MOTION,
    EEP_DEMO_NUM
};

#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT

/**
 *  Description :
 *  Allocation LayOut of 8205 Eeprom, Each Item's Length is counted by UINT8
*/
enum
{
#ifdef CC_MT5363 
    /*============================================================*/
    /* MAC address for ethernet, 6byte +1byte checksum,                                    */
    /* To prevent the location change, put it on NPTV eeprom head                        */
    /* Use 2 set for user modify and default value.(14 bytes)                                 */
    /*============================================================*/

    EEP_ETH_MAC_ADDR_START = 0x00, //use 14 byte to store MAC address
    EEP_ETH_MAC_ADDR_END = EEP_ETH_MAC_ADDR_START + EEP_ETH_MAC_SIZE -1,
#endif
    /*============================================================*/
    /** TV settings, most for Setup->TV menus */
    /*============================================================*/

    // TVD segment.
#ifdef CC_MT5363 
    EEP_ADC_START,
#else
    EEP_ADC_START = 0x00,
#endif
    EEP_ADC_OFFSET_TV_CLAMP_DONE = EEP_ADC_START,
    EEP_ADC_INPUT_OFFSET_TV_CH0_CLAMP,
    EEP_ADC_OUTPUT_OFFSET_TV_CH0_CLAMP,
    EEP_ADC_INPUT_OFFSET_TV_CH1_CLAMP,
    EEP_ADC_OUTPUT_OFFSET_TV_CH1_CLAMP,
    EEP_ADC_DFE_LEVEL_TV_DCLAMP,
    EEP_ADC_OFFSET_CLAMP_BUT_LOW,

    EEP_ADC_AUTOCOLOR_OVER_RULE_EFUSE,
    EEP_ADC_VGA_MAX_CH0,
    EEP_ADC_VGA_MAX_CH1,       
    EEP_ADC_VGA_MAX_CH2,
    EEP_ADC_HDTV_MAX_CH0,
    EEP_ADC_HDTV_MAX_CH1,       
    EEP_ADC_HDTV_MAX_CH2,

    EEP_ADC_END = EEP_ADC_START + EEP_ADC_SIZE - 1,

    // Auto color segment.
    EEP_VIDEO_AUTO_COLOR_START, // 0x10

	EEP_VIDEO_AUTO_COLOR_HDTV_GAIN0 = EEP_VIDEO_AUTO_COLOR_START, //0x10
	EEP_VIDEO_AUTO_COLOR_HDTV_GAIN1,     //0x11
	EEP_VIDEO_AUTO_COLOR_HDTV_GAIN2,     //0x12

	EEP_VIDEO_AUTO_COLOR_VGA_GAIN0,      //0x13
	EEP_VIDEO_AUTO_COLOR_VGA_GAIN1,      //0x14
	EEP_VIDEO_AUTO_COLOR_VGA_GAIN2,      //0x15

	EEP_VIDEO_AUTO_COLOR_SCART_GAIN0,    //0x16
	EEP_VIDEO_AUTO_COLOR_SCART_GAIN1,    //0x17
	EEP_VIDEO_AUTO_COLOR_SCART_GAIN2,    //0x18

	EEP_VIDEO_AUTO_COLOR_VGA_COMP_GAIN0, //0x19
	EEP_VIDEO_AUTO_COLOR_VGA_COMP_GAIN1, //0x1A
	EEP_VIDEO_AUTO_COLOR_VGA_COMP_GAIN2, //0x1B

	EEP_VIDEO_AUTO_COLOR_HDTV_DONE,      //0x1C
	EEP_VIDEO_AUTO_COLOR_VGA_DONE,       //0x1D
	EEP_VIDEO_AUTO_COLOR_SCART_DONE,     //0x1E
	EEP_VIDEO_AUTO_COLOR_VGA_COMP_DONE,  //0x1F

    EEP_ADC_OFFSET_START,       // 0x20
	EEP_ADC_OFFSET_HDTV_CLAMP0 = EEP_ADC_OFFSET_START,    //0x20
	EEP_ADC_OFFSET_HDTV_CLAMP1,          //0x21
	EEP_ADC_OFFSET_HDTV_CLAMP2,          //0x22

	EEP_ADC_OFFSET_VGA_CLAMP0,           //0x23
	EEP_ADC_OFFSET_VGA_CLAMP1,           //0x24
	EEP_ADC_OFFSET_VGA_CLAMP2,           //0x25

	EEP_ADC_OFFSET_SCART_CLAMP0,         //0x26
	EEP_ADC_OFFSET_SCART_CLAMP1,         //0x27
	EEP_ADC_OFFSET_SCART_CLAMP2,         //0x28

	EEP_ADC_OFFSET_VGA_COMP_CLAMP0,      //0x29
	EEP_ADC_OFFSET_VGA_COMP_CLAMP1,      //0x2A
	EEP_ADC_OFFSET_VGA_COMP_CLAMP2,      //0X2B

    EEP_ON_CHIP_AUTO_COLOR_GAIN_DONE,     //Add for ON_CHIP_AUTO_COLOR  //0x2c
    //EEP_ON_CHIP_AUTO_COLOR_OFFSET_DONE,   //Add for ON_CHIP_AUTO_COLOR
	//EEP_VIDEO_AUTO_COLOR_DIFF_1,
    //EEP_VIDEO_AUTO_COLOR_DIFF_2,
    //EEP_VIDEO_AUTO_COLOR_DIFF_3,
    EEP_ON_CHIP_AUTO_COLOR_OFFSET_ADDRESS_START,      //0x2D ~0x3F (only 3 location)
    EEP_VIDEO_AUTO_COLOR_END = EEP_VIDEO_AUTO_COLOR_START + EEP_AUTOCOLOR_SIZE - 1,


#ifdef THE_3RD_PARTY_SW_SUPPORT
    EEP_REV1_START, //30
    EEP_REV1_END=EEP_REV1_START + EEP_REV1_SIZE -1,

    // YUV delay.
    EEP_YDELAY_START,           // 0x50,
    EEP_YDELAY_RF_NTSC = EEP_YDELAY_START,
    EEP_YDELAY_RF_PAL_M,
    EEP_YDELAY_RF_PAL_N,
    EEP_YDELAY_CVBS_NTSC358,
    EEP_YDELAY_CVBS_NTSC443,
    EEP_YDELAY_CVBS_PAL,
    EEP_YDELAY_CVBS_PAL_M,
    EEP_YDELAY_CVBS_PAL_N,
    EEP_YDELAY_YC_NTSC358,
    EEP_YDELAY_YC_NTSC443,
    EEP_YDELAY_YC_PAL,
    EEP_YDELAY_YC_PAL_M,
    EEP_YDELAY_YC_PAL_N,
    EEP_YDELAY_END = EEP_YDELAY_START + EEP_YDELAY_SIZE - 1,

    //Reserve
    EEP_REV2_START, // 0x60
    EEP_REV2_END = EEP_REV2_START + EEP_REV2_SIZE -1,

    //UV Delay
    EEP_UVDELAY_START,          // 0x70,
    EEP_UVDELAY_RF_NTSC = EEP_UVDELAY_START,
    EEP_UVDELAY_RF_PAL_M,
    EEP_UVDELAY_RF_PAL_N,
    EEP_UVDELAY_CVBS_NTSC358,
    EEP_UVDELAY_CVBS_NTSC443,
    EEP_UVDELAY_CVBS_PAL,
    EEP_UVDELAY_CVBS_PAL_M,
    EEP_UVDELAY_CVBS_PAL_N,
    EEP_UVDELAY_YC_NTSC358,
    EEP_UVDELAY_YC_NTSC443,
    EEP_UVDELAY_YC_PAL,
    EEP_UVDELAY_YC_PAL_M,
    EEP_UVDELAY_YC_PAL_N,
    EEP_UVDELAY_END = EEP_UVDELAY_START + EEP_UVDELAY_SIZE - 1,

    EEP_REV3_START, // 0x80
    EEP_REV3_END = EEP_REV3_START + EEP_REV3_SIZE -1,
    
    // Blue stretch.
    EEP_BLUE_STRETCH_START,     // 0x90,
    EEP_BLUE_STRETCH_END = EEP_BLUE_STRETCH_START + EEP_BLUE_STRETCH_SIZE - 1,

    // Adaptive Luma
    EEP_ADAPTIVE_LUMA_START,  //0xA0
    EEP_ADAPTIVE_LUMA_END=EEP_ADAPTIVE_LUMA_START + EEP_ADAPTIVE_LUMN_SIZE-1,

    // BWS
    EEP_BWS_START, //0xB0
    EEP_BWS_END =EEP_BWS_START + EEP_BWS_SIZE-1,

    // AutoTV NR.
    EEP_AUTOTV_NR_START,        // 0xC0,
    EEP_AUTOTV_NR_END = EEP_AUTOTV_NR_START + EEP_AUTOTV_NR_SIZE - 1,

    // AutoTV BL.
    EEP_AUTOTV_BL_START,        // 0x140,
    EEP_AUTOTV_BL_END = EEP_AUTOTV_BL_START + EEP_AUTOTV_BL_SIZE - 1,

#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
#if SUPPORT_VGA_USERMODE
        /** VGA USR: 48 byte rec(6byte*8)*/
    EEP_VGA_USR_START , //6*8 = 48 
    EEP_VGA_USR_END = EEP_VGA_USR_START + (USERMODE_TIMING*sizeof(VGA_USRMODE))-1,
    EEP_VGA_USR_MODE_INDEX,
#endif //#if SUPPORT_VGA_USERMODE    

    EEP_CEC_LOG_ADDR,
	//AV AUTO
    EEP_VIDEO_AUTO_CALIB_TV_GAIN0,
    EEP_VIDEO_AUTO_CALIB_TV_GAIN1,
    EEP_VIDEO_AUTO_CALIB_TV_GAIN2,
    EEP_VIDEO_AUTO_CALIB_AV_GAIN0,
    EEP_VIDEO_AUTO_CALIB_AV_GAIN1,
    EEP_VIDEO_AUTO_CALIB_AV_GAIN2,
    EEP_VIDEO_AUTO_CALIB_TV_MAX,
    EEP_VIDEO_AUTO_CALIB_AV_MAX,
    EEP_VIDEO_AUTO_CALIB_TV_OFFSET,
    EEP_VIDEO_AUTO_CALIB_AV_OFFSET,
	
    EEP_VIDEO_AUTO_CALIB_TV_DONE,
    EEP_VIDEO_AUTO_CALIB_AV_DONE,
    
#ifndef CC_DISABLE_EEP_MAG_CHK    
//Magic String
    EEP_MAGIC0,
    EEP_MAGIC1,
    EEP_VERSION0,
    EEP_VERSION1,
#endif  
     EEP_MUSB_ENABLE,  
    
#ifdef CC_SUPPORT_TVE_CTRL_BY_DRIVER
    EEP_TVE_ATV_SELECT,
#endif

#ifdef SUPPORT_VGA_UI_AMBIGUOUS_TIMING_SELECT   
    EEP_VGA_AMBIGUOUS_DEFAULT_START, //10 group
    EEP_VGA_AMBIGUOUS_DEFAULT_END = EEP_VGA_AMBIGUOUS_DEFAULT_START + 9,
#endif         
     #ifdef CC_BYPASS_AVI_INFOFRAME
         //HDMI AVI infoframe bypass status
    EEP_HDMI_AVIINFO_BYPASS,
    #endif
    EEP_BOOT_MUSIC_ONOFF,

	EEP_GMMMA_CTRL_1,     
	EEP_GMMMA_CTRL_2,     
	EEP_GMMMA_CTRL_3,     
	EEP_GMMMA_CTRL_4,     
	EEP_GMMMA_DELTA_R1,   
	EEP_GMMMA_DELTA_R2,   
	EEP_GMMMA_DELTA_R3,   
	EEP_GMMMA_DELTA_R4,   
	EEP_GMMMA_DELTA_B1,   
	EEP_GMMMA_DELTA_B2,   
	EEP_GMMMA_DELTA_B3,   
	EEP_GMMMA_DELTA_B4,   
    EEP_GMMMA_DELTA_DONE,

    //reserve one byte for panel index before end
    EEP_GEN_SET_END
     

};

#define  EEPROM_GENERAL_SIZE   EEP_GEN_SET_END
#define  EEPROM_END_POS        EEP_GEN_SET_END

#endif /* _EEPDEF_H */

